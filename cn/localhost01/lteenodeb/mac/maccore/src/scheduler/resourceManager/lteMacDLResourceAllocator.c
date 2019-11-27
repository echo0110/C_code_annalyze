/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacDLResourceAllocator.c,v 1.1.1.1.6.1.6.2 2010/10/20 06:09:48 gur20491 Exp $ 
 *
 ******************************************************************************
 *
 *  File Description :  This files contains the Funtions needed for RB  
 *                      Allocation according to RAT 0,RAT 1,RAT 2 Localized 
 *                      or RAT 2 Distributed. for both Sapatial and 
 *                      Non Spatial Multiplexing taking consideration of CQI as
 *                      well as without CQI 
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacDLResourceAllocator.c,v $
 * Revision 1.1.1.1.6.1.6.2  2010/10/20 06:09:48  gur20491
 * Log commented for Compilation Issue
 *
 * Revision 1.1.1.1.6.1.6.1  2010/10/11 12:57:06  gur10121
 * SPR #706 fix
 *
 * Revision 1.1.1.1.6.1.10.1  2010/10/08 07:42:50  gur10121
 *  SPR 706 Fix - KlockWork Warnings removed
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.67  2009/11/01 14:14:01  gur18550
 * Bug related to innerLoopCount in sortSubsetBasedOnCQIFor... got fixed
 *
 * Revision 1.66  2009/10/30 06:32:48  gur11912
 * Fixed for RAT2updation from RAT0 and RAT1
 *
 * Revision 1.65  2009/10/22 11:51:16  gur11912
 * fixed bug while updating RAT2 from RAT0
 *
 * Revision 1.64  2009/09/03 15:32:28  gur20052
 * Added Warnings
 *
 * Revision 1.63  2009/08/31 09:17:48  gur12140
 * Added UT LOGS
 *
 * Revision 1.62  2009/08/09 09:42:02  gur12140
 * Solved Bug
 *
 * Revision 1.61  2009/08/09 09:15:56  gur12140
 * Solved Bug
 *
 * Revision 1.60  2009/08/09 07:46:34  gur12140
 * Solved Bug
 *
 * Revision 1.59  2009/08/08 11:31:45  gur12140
 * Removed Redundant Code
 *
 * Revision 1.58  2009/08/06 09:06:54  gur12140
 * Incorporated Comments
 *
 * Revision 1.57  2009/08/06 05:35:54  gur12140
 * Solved Bug
 *
 * Revision 1.56  2009/08/04 13:09:04  gur12140
 * Bug Solved Related to determine RBs
 *
 * Revision 1.55  2009/08/04 06:28:43  gur12140
 * Incorporated Comments
 *
 * Revision 1.54  2009/08/03 10:28:31  gur12140
 * Incorporated Comments
 *
 * Revision 1.53  2009/08/03 07:28:05  gur12140
 * Solved Bug and Incorporated Comments
 *
 * Revision 1.52  2009/08/02 13:28:28  gur12140
 * Optimized it
 *
 * Revision 1.51  2009/07/31 06:05:20  gur12140
 * Removed Warning
 *
 * Revision 1.50  2009/07/28 13:53:19  gur12140
 * Solved Bug
 *
 * Revision 1.49  2009/07/28 07:09:44  gur12140
 * Solved Bug
 *
 * Revision 1.48  2009/07/27 11:58:00  gur20052
 * Solved Bug
 *
 * Revision 1.47  2009/07/27 06:48:28  gur12140
 * Solved Bug and Added UT_LOGS
 *
 * Revision 1.46  2009/07/24 06:32:15  gur12140
 * Removed Warnings
 *
 * Revision 1.45  2009/07/24 06:28:07  gur12140
 * Solved Bug
 *
 * Revision 1.44  2009/07/22 05:26:39  gur12140
 * Removed Splint Warnings
 *
 * Revision 1.43  2009/07/20 05:11:22  gur12140
 * Replaced MACModuleLogDetail_g with DL_RESOURCE_MGR
 *
 * Revision 1.42  2009/07/15 09:04:05  gur12140
 * Incorporated Comments
 *
 * Revision 1.41  2009/07/14 08:11:32  gur12140
 * Solved Bug
 *
 * Revision 1.40  2009/07/14 04:29:41  gur12140
 * Removed Log
 *
 * Revision 1.39  2009/07/08 13:29:05  gur12140
 * Incorporated Comments
 *
 * Revision 1.38  2009/07/08 10:09:59  gur12140
 * Incorporated Comments
 *
 * Revision 1.36  2009/07/03 13:30:39  gur12140
 * Solved Bug
 *
 * Revision 1.35  2009/07/03 09:07:37  gur12140
 * Incorporated Commnets
 *
 * Revision 1.34  2009/07/03 06:37:39  gur12140
 * Incorporated Comments
 *
 * Revision 1.33  2009/07/02 08:21:52  gur12140
 * Removed Errors
 *
 * Revision 1.32  2009/07/02 08:00:29  gur12140
 * Incorporated Comments
 *
 * Revision 1.31  2009/06/30 09:23:53  gur12140
 * Solved Bug and Incorporated Comments
 *
 * Revision 1.30  2009/06/29 13:45:34  gur12140
 * Incorporated Comments
 *
 * Revision 1.29  2009/06/24 09:18:53  gur12140
 * Incorporated Comments
 *
 * Revision 1.28  2009/06/22 06:06:20  gur12140
 * Solved Bug
 *
 * Revision 1.27  2009/06/20 14:55:26  gur12140
 * Removed Bug and Optimized it
 *
 * Revision 1.26  2009/06/17 12:37:49  gur12140
 * Solved Bug
 *
 * Revision 1.25  2009/06/17 09:58:19  gur12140
 * Incorporated Comments
 *
 * Revision 1.24  2009/06/17 08:54:49  gur12140
 * Removed Compilation Errors
 *
 * Revision 1.23  2009/06/17 06:46:20  gur12140
 * Resolved Bug
 *
 * Revision 1.22  2009/06/16 14:31:49  gur12140
 * Incorporated Comments
 *
 * Revision 1.20  2009/06/16 12:57:52  gur12140
 * Incorporated Comments
 *
 * Revision 1.19  2009/06/12 05:03:25  gur12140
 * Incorporated Comments
 *
 * Revision 1.18  2009/06/10 08:57:06  gur12140
 * Incorporated Comments
 *
 * Revision 1.16  2009/06/08 12:07:32  gur12140
 * Incorporated Comments
 *
 * Revision 1.15  2009/06/08 06:16:48  gur15697
 * allocateResourcesForMIB() added
 *
 * Revision 1.14  2009/06/05 05:50:20  gur12140
 * Statis Declarartion of allocateRBsForCommonChannel Removed
 *
 * Revision 1.13  2009/06/05 05:48:02  gur12140
 * Added Funtionality for RAT 0 RAT 1 Common Channel
 *
 * Revision 1.12  2009/06/05 05:03:49  gur12140
 * Added Funtinonality to allocate RBs for Common Control Channel
 *
 * Revision 1.11  2009/06/01 13:00:16  gur12140
 * Removed Compilation Error
 *
 * Revision 1.9  2009/05/29 15:23:18  gur12140
 * Added Funtionality for RAT 1
 *
 * Revision 1.8  2009/05/28 07:26:38  gur12140
 * Added Funtionality for RAT 2 Localized ,Distributed and RAT 0
 *
 * Revision 1.7  2009/05/25 13:01:24  gur12140
 * Updated the function populateResourceAssignmentInfo
 *
 * Revision 1.5  2009/05/25 10:55:12  gur12140
 * Added Log
 *
 * Revision 1.4  2009/05/25 10:53:09  gur12140
 * RAT 2 Localized and RAT 2 Distributed Code Updated
 *
 * 
 *
 *****************************************************************************/


 /******************************************************************************
  * Standard Library Includes
  *****************************************************************************/

 /******************************************************************************
  * Project Includes
  *****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacDLResourceAllocator.h"
#include "lteMacTBSize.h"
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacStatsManager.h"
/* ICIC changes start */
#include "lteMacDLResourceInfoMgr.h"
/* ICIC changes end */
/*Dynamic PDCCH + 5117 changes starts */
#include "lteMacParseUtil.h"
/*Dynamic PDCCH + 5117 changes ends */
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#endif
#include "lteMacPRSMgr.h"
/* + TM7_8 Changes Start */
#include "lteMacPDSCHMgr.h"
/* - TM7_8 Changes End */
#include "lteMacDLPowerControlData.h"

/* Start of change for VARIBLE Declaration */
UInt32 NumOfOverLappingRbsInfo_g[RES_BIT_MAP_ARRAY_SIZE];
UInt8 (*getCountofOverLapRB)(UInt32, UInt32 *);
/* End of change for VARIBLE Declaration */
/* 4x4 DL MIMO CHG START */
/** klockwork Changes Start **/
void updateMCSRBbelowCoderateForRAT1AndRAT2For4x4Mimo(
                                                DLHARQProcess *harqProcess_p,
                                                /* +- SPR 17777 */
                                                InternalCellIndex internalCellIndex);
void  updateMCSRBbelowCoderateForRAT0For4x4Mimo(UInt8 useLastRBG,
                                                DLHARQProcess *harqProcess_p,
                                                /* +- SPR 17777 */
                                                InternalCellIndex internalCellIndex);
/** klockwork Changes End **/
/* 4x4 DL MIMO CHG END */
 /*Dynamic PDCCH + 5117 changes starts */
MacRetType updateMCSRBbelowUECategoryCoderateForRAT1AndRAT2(DLHARQProcess *harqProcess_p,
        DLUEContext *ueDLContext_p,
        InternalCellIndex internalCellIndex);
MacRetType updateMCSRBbelowUECategoryCoderateForRAT0(UInt8 useLastRBG,
                                                     DLHARQProcess *harqProcess_p,
                                                     DLUEContext *ueDLContext_p,
                                                     InternalCellIndex internalCellIndex);
STATIC  MacRetType updateMCSRBForTbOneAndTbTwo(DLHARQProcess *harqProcess_p,
                                                     UInt8 ueCategory,
                                                     UInt32 permissibleTBSize,
                                                     UInt8 useLastRBG);

MacRetType  checkCodeRateUECategory(
        UInt8 raPolicy, 
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        DLHARQProcess *harqProcess_p,
        DLUEContext *ueContext_p,
        UInt8 ueCategoryChecksRequired,
        UInt32 availableRBGBitMap,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex);

/* + TM7_8 Changes Start */
/* The analysis was done to findout that till which MCS Index coderate is exceeding 
 * on each possible RBCount(MAX_NUM_OF_RB) depeding upon the number of antenna configured,
 * when UE Specific reference signal will be introduces
 * as a part of Transmission mode 7 and 8 implementation.
 * For CFI_ONE_NUMBER_OF_ANTENNA_2, the coderate was exceeding at MCS INDEX 28 for some RBs.
 * For CFI_TWO_NUMBER_OF_ANTENNA_2, the coderate was always exceeding with MCSIndex 28
 * For CFI_THREE, sometimes the coderate was exceeding till MCS Index 26.
 * Thus below array contaion the value for Max Allowed MCS index at particular RBCount
 * to get the best suited TBSize particular RBCount*/
 
UInt8 maxMcsIdxForTM7_8_CFIOneNumOfAntennaTwo[MAX_NUM_OF_RB]={
28,27,27,27,27,28,27,27,27,27,27,28,28,27,27,27,27,27,27,27,28,27,27,28,28,
27,27,27,27,27,27,27,27,27,28,28,27,27,27,28,27,28,27,27,28,27,27,28,27,27,
27,28,27,27,27,28,27,28,27,28,27,28,27,28,27,27,28,27,27,27,27,28,27,27,27,
28,27,27,28,27,28,28,27,27,28,27,28,28,27,27,28,27,27,28,27,27,28,27,27,27};


UInt8 maxMcsIdxForTM7_8_CFIThreeNumOfAntennaTwo[MAX_NUM_OF_RB]={
25,25,25,25,26,25,25,25,25,26,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,26,25,25,25,25,26,25,
25,26,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
25,25,25,25,25,25,25,25,25,25,25,25,26,25,25,25,25,25,25,25,25,25,25,25,25};

void updateMCSIndexAndTBSIzeForTransMode_7_8(
        DLHARQProcess *harqProcess_p,
        InternalCellIndex internalCellIndex);

MacRetType  checkCodeRateUECategoryForTransMode_7_8(
        DLUEContext *ueContext_p,
        DLHARQProcess *harqProcess_p,
        InternalCellIndex internalCellIndex);
/* - TM7_8 Changes End */
extern UInt8 currentCFIValue_g[MAX_NUM_CELL];
extern UInt32 maxNumberOfCodedBitsMap_g[MAX_SFN_INDEX + 1][(MAX_CFI_VALUE + 1)][MAX_MOD_SCHEME][MAX_NUM_OF_RB][7];
 /*Dynamic PDCCH + 5117 changes ends */

#ifdef PERF_STATS
#include "ltePerfStats.h"
#endif

 /******************************************************************************
   Private Types
  *****************************************************************************/
 typedef struct ResourceFormatNodeT
 {
     UInt32 tbSize;
     UInt32 cqiRBGBitmap;
     UInt8 mcsIndex;
     UInt8 prbs;
     /*to be used only in RAT2 Localized*/
     UInt8 startVRBIndex;
     UInt8 pmiIndex; 
     UInt32 subsetIndex;
     UInt32 isNonZeroSpan;
 }ResourceFormatNode;

 typedef struct ResourceFormatMIMONodeT
 {
     UInt32 tbSize;
     UInt32 tbSizeCW1;
     UInt32 tbSizeCW2;
     UInt32 cqiRBGBitmap;
     UInt8  mcsIndexCW1;
     UInt8  mcsIndexCW2;
     UInt8  prbs;
     UInt8  pmiIndex;
     UInt32 subsetIndex;
     UInt32 isNonZeroSpan;
 }ResourceFormatMIMONode;


#define MOD_SCHEME_CNT 3
 /******************************************************************************
   Private Definitions
  *****************************************************************************/
/* + CQI_4.1 */
 UInt32 FFS(UInt32 bitMap);
/* - CQI_4.1 */
/* SPS CHG */

STATIC  void updateRAT2LocalizedInfoFromRAT2DistributedInfo(     
        RAT2DistributedVRBInfo *distributedVRBInfo_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        InternalCellIndex internalCellIndex);

STATIC  void updateRAT0InfoFromRAT2DistributedInfo(
        /* +- SPR 17777 */
        RAT0Info *rat0Info_p,
        RAT2DistributedVRBInfo *distributedVRBInfo_p);

STATIC  void updateRAT1InfoFromRAT2DistributedInfo(
        SubsetInfo *subsetInfo_p,
        /* +- SPR 17777 */
        RAT2DistributedVRBInfo *distributedVRBInfo_p);

STATIC  MacRetType  allocRBRAT2Distributed(
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        UInt32 wideBandMcsIndex,
        DLHARQProcess *harqProcess_p
/* SPR 4184 Chg */
#ifdef TDD_CONFIG
        , UInt8 subFrame
#endif
/* SPR 4184 Chg */
        /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
        ,DLUEContext *dlUEContext_p
#endif
        ,InternalCellIndex internalCellIndex
        /* HD FDD Changes End */
);
/*Dynamic PDCCH + 5117 changes starts */
extern UInt8 (*getCountofOverLapRB)(UInt32, UInt32 *); 
STATIC  UInt8 checkAllocRBLiesInCentralSix(UInt32 prbNumber,
               InternalCellIndex  internalCellIndex);
/*Dynamic PDCCH + 5117 changes ends */
STATIC MacRetType getBestContDistributedVRBReTx(UInt32 requiredRBCount,
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        RAT2DistributedInfo *rat2DistributedInfo_p);

STATIC MacRetType getBestContDistributedVRBNewTx(UInt32 requiredRBCount,
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        RAT2DistributedInfo *rat2DistributedInfo_p);

STATIC  void updateRATInfoFromRAT2DistributedInfo(
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        UInt32 allocatedRBCount,
        InternalCellIndex internalCellIndex);

/* + PRS_CHANGES */
  void updateRAT2DistributedInfoFromRAT2LocalizedInfo(     
        RAT2LocalizedVRBInfo *localizedVRBInfo_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p);
/* - PRS_CHANGES */

STATIC  void updateRAT0InfoFromRAT2LocalizedInfo(
        UInt32 rbgIndexBitMap,
        UInt32 *rat0BitMap_p);

 void updateRAT1InfoFromRAT2LocalizedInfo(
        SubsetInfo *subsetInfo_p,
        UInt32 subsetSize,
        UInt32 prbNumber);
/* - PRS_CHANGES */
/* SPS_CHG */
/* ICIC changes start */
void updateRATInfoFromRAT2LocalizedInfo(
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        UInt32 allocatedRBCount,
        UInt32 updateRAT2DistributedFlag,
        /* +DYNAMIC_ICIC */
        UInt8  calculatedRNTP,
        InternalCellIndex internalCellIndex);
        /* -DYNAMIC_ICIC */

MacRetType  getBestContLocalizedVRBNewTx(UInt32 requiredRBCount,
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        RAT2LocalizedInfo *rat2LocalizedInfo_p,
        UInt8 resourceRegionAllocated,
        UInt32 maxRBs,
        InternalCellIndex internalCellIndex);
        

STATIC MacRetType getBestContLocalizedVRBReTx(UInt32 requiredRBCount,
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        RAT2LocalizedInfo *rat2LocalizedInfo_p,
        UInt8 resourceRegionAllocated);

/*HD FDD Changes Start*/
#ifdef FDD_CONFIG
STATIC  MacRetType allocRBRAT2Localized(
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        /* +DYNAMIC_ICIC */
        DLUEContext *dlUEContext_p,
        /* -DYNAMIC_ICIC */
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode *dlStrategyNode_p
        /*Dynamic Pdcch Changes Start*/
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic Pdcch Changes End*/
        ,InternalCellIndex internalCellIndex
        );
#elif TDD_CONFIG
STATIC  MacRetType allocRBRAT2Localized(
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        /* +DYNAMIC_ICIC */
        DLUEContext *dlUEContext_p,
        /* -DYNAMIC_ICIC */
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode *dlStrategyNode_p
        /*Dynamic PDCCH + 5117 changes starts */
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends */
        ,UInt8 subFrame,
        InternalCellIndex internalCellIndex);
        /*DLUEContext *dlUEContext_p);*/
#endif
/*HD FDD Changes End*/
/* ICIC changes end */

STATIC void initRAT0Info(RAT0Info *rat0Info_p,
        UInt32 dlRBs) ;

STATIC UInt32 calculateMaxPRB(
        UInt32 subsetIndex,
        UInt32 dlRBs,
        UInt32 subsetSize);

STATIC void initbitMappedToPrbNumberArray(
        UInt32 subsetSize,
        UInt32 subsetIndex,
        SubsetInfo *subsetInfo_p,
        UInt32 maxAddressablePRB);

STATIC UInt32 calculateMaxAddressablePRB(
        UInt32 subsetSize,
        UInt32 dlRBs);

STATIC void initRAT1Info(RAT1Info *rat1Info_p,
        UInt32 dlRBs);

STATIC void assignPrb(
        RAT2DistributedVRBInfo *distributedVRBInfo_p,
        UInt32 vrbNum,
        UInt32 nTilledDlVrb,
        UInt32  nRow,
        UInt32  nNull,
        UInt32  nGap);

STATIC void initRAT2DistributedInfo(ResourceAllocationPolicyInfo *
        raPolicyInfo_p,
        UInt32 dlRBs,
        InternalCellIndex internalCellIndex);

STATIC  MacRetType checkDLBandwidthRBsRange(
        UInt32 dlRBs,
        UInt8 *nGap_p,
        UInt32 gapValue);

STATIC void initRAT2LocalizedInfo(ResourceAllocationPolicyInfo *raPolicyInfo_p,
        UInt32 dlRBs);

STATIC void initPrbToVrbInfo(
        RAT2LocalizedInfo *rat2LocalizedInfo,
        RAT2DistributedInfo *rat2DistributedInfo);

STATIC  void  updateRAT2InfoFromRAT0Info(UInt32 rbgIndex ,
        UInt32 rbgSize,
        UInt32 rbsInRBG,
        ResourceAllocationPolicyInfo *raPolicy_p,
        /* +DYNAMIC_ICIC */
        UInt8 calculatedRNTP,
        InternalCellIndex internalCellIndex);
        /* -DYNAMIC_ICIC */

STATIC  void  updateRAT1InfoFromRAT0Info(       
        RAT1Info *rat1Info_p,           
        RBGIndexInfo *rbgIndexInfo_p);

STATIC  void getMcsIndexForCW1( 
        UInt32 *mcsIndexAllocatedCW1_p,    
        UInt32 availableRBGBitMap,
        SubBandReportInfo *subBandReportInfo_p,
        UInt32 prbCount);

STATIC  void getMcsIndexForCW2( 
        UInt32 *mcsIndexAllocatedCW2_p,    
        UInt32 availableRBGBitMap,
        SubBandReportInfo *subBandReportInfo_p,
        UInt32 prbCount);

STATIC  void getMcsIndexForCW1AndCW2(
        UInt32 *mcsIndexAllocatedCW1_p,
        UInt32 *mcsIndexAllocatedCW2_p,
        UInt32 availableRBGBitMap,
        SubBandReportInfo *subBandReportInfo_p,
        UInt32 prbCount);

STATIC UInt32  getRAT0BitMapBothTBNewTx(                
    ResourceAllocatorInput *resrcAllocatorInput_p,
    DLHARQProcess  *harqProcess_p,
    RAT0Info *rat0Info_p,
    DLStrategyTxNode *dlStrategyNode_p,
    /* + TM7_8 Changes Start */
    UInt32 centralRbMask,
    UInt8 transmissionMode,
    /* SPR 15909 fix start */
    tickType_t scheduledTick,
    /* SPR 15909 fix end */
    InternalCellIndex internalCellIndex);
    /* - TM7_8 Changes End */
/* Cyclomatic Complexity Changes -starts here*/
STATIC UInt32 getRAT0BitMapFromSubBand (RAT0Info *rat0Info_p,
                                ResourceAllocatorInput *resrcAllocatorInput_p,
                                DLHARQProcess  *harqProcess_p,
                                 DLStrategyTxNode *dlStrategyNode_p,
                                ResourceFormatMIMONode * previousRsrcFrmt_p,
                                UInt32 centralRbMask,
                                UInt8 transmissionMode,
                                /* SPR 15909 fix start */
                                tickType_t scheduledTick,
                                /* SPR 15909 fix end */
                                InternalCellIndex internalCellIndex);
STATIC  UInt32 getRAT0BitMap( UInt32 cqiBitMapCodeWord1,
        UInt32 cqiBitMapCodeWord2,
        RAT0PmiInfo * rat0PmiInfo_p,
        RAT0Info *rat0Info_p,
        ResourceAllocatorInput * resrcAllocatorInput_p,
        DLHARQProcess * harqProcess_p,
        ResourceFormatMIMONode * previousRsrcFrmt_p,
        DLStrategyTxNode * dlStrategyTxNode_p,
        UInt32 centralRbMask,
        UInt8 transmissionMode,
        /* SPR 15909 fix start */
        tickType_t scheduledTick ,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);

STATIC UInt8 allocateRBRat2Tb1ValidTb2InvalidForSubBand(DLUEContext *dlUEContext_p,
                                                 ResourceAllocatorInput *resourceAllocInput_p,
                                                 ResourceAllocationPolicyInfo *raPolicyInfo_p,
                                                 DLHARQProcess *harqProcess_p,
                                                 BestVRBSpanInfo *bestVRBSpanInfo_p,
                                                 DLStrategyTxNode *dlStrategyNode_p,
                                                 UInt32 * allocatedRBCount,
                                                 InternalCellIndex internalCellIndex
                                                    );
STATIC void updateRAT0SubBandPMIInfo(DLUEContext * dlUEContext_p,
          DLHARQProcess * harqProcess_p,
          ResourceAllocatorInput *resrcAllocatorInput_p,
          ResourceAllocatorOutput *resrcAllocatorOutput_p,
          UInt32 availableRBGBitMap,
          UInt32 tempRBGBitMap,
          UInt32 rbsAllocated
#ifdef TDD_CONFIG
         /*coverity-530-32797*/
         , UInt8 subframe
         /*coverity-530-32797*/
#endif
         ,InternalCellIndex internalCellIndex
              );
STATIC MacRetType allocRBInRAT0SpatialForSubband (DLUEContext * dlUEContext_p,
     DLStrategyTxNode *dlStrategyNode_p,
     DLHARQProcess *harqProcess_p, 
     ResourceAllocatorInput  * resrcAllocatorInput_p,
     ResourceAllocationPolicyInfo *raPolicy_p,
     UInt32 * tempRBGBitMap_p,
     UInt32 availableRBCount,
     SInt8 * allocatedRb_p,
     UInt32 centralRbMask,
     UInt8  transmissionMode,
     /* SPR 15909 fix start */
     tickType_t scheduledTick
     /* SPR 15909 fix end */
#ifdef TDD_CONFIG
        /*coverity-530-32797*/
     ,UInt8 subframe
        /*coverity-530-32797*/
#endif
     ,InternalCellIndex internalCellIndex
     );
STATIC void updateRAT1SubBandPMIInfo(DLUEContext * dlUEContext_p,
                            DLHARQProcess *harqProcess_p,
                            ResourceAllocatorInput * resrcAllocatorInput_p,
                            UInt32 availableRBBitMap,
                            UInt32 tempRBBitMap,
                            UInt32 subsetIndexAllocated,
                            UInt32 isNonZeroSpanConsideredForAlloc );

STATIC  MacRetType allocRBInRAT1SpatialForSubband ( DLHARQProcess * harqProcess_p,
                                     ResourceAllocatorInput * resrcAllocatorInput_p,
                                     RAT1Info * rat1Info_p,
                                     DLStrategyTxNode * dlStrategyTxNode_p,
                                     UInt32 availableRBCount,
                                     UInt32 * subsetIndexAllocated_p,
                                     UInt32 *isNonZeroSpanConsideredForAlloc_p,
                                     UInt8 * allocatedRb_p,
/*Coverity_31964 Fix Start*/
                                     SubsetInfo **subsetInfo_p,
/*Coverity_31964 Fix End*/
                                     UInt32 *tempRBBitMap_p,
                                     /* + TM7_8 Changes Start */
                                     UInt32 centralRbMask[MAX_SUBSETS],
                                     UInt8  transmissionMode,
                                     /* SPR 15909 fix start */
                                     tickType_t scheduledTick
                                     /* SPR 15909 fix end */
                                     /* - TM7_8 Changes End */
#ifdef TDD_CONFIG
                                    /*coverity-530-32798*/
                                    ,UInt8 subframe 
                                    /*coverity-530-32798*/
#endif
                                    ,InternalCellIndex internalCellIndex
                                      );
STATIC  void updatePMIIndexInHarqContext(DLUEContext * dlUEContext_p,
                                        DLHARQProcess * harqProcess_p,
                                        UInt32 macAdvanceFactor,
                                        UInt8 widebandPMI,
                                        InternalCellIndex internalCellIndex,
                                        SInt8 allocatedRb
                                        /* Cyclomatic_complexity_changes_start */
                                        #ifdef TDD_CONFIG
                                        ,UInt8 isSpecialSubframe
                                        #endif
                                        /* Cyclomatic_complexity_changes_end */
                                        ); 

/* Cyclomatic Complexity Changes -ends here*/

STATIC UInt32 getRAT0BitMapTBOneInValidTBTwoNewTx(
    ResourceAllocatorInput *resrcAllocatorInput_p,
    DLHARQProcess  *harqProcess_p,
    RAT0Info *rat0Info_p,
    DLStrategyTxNode *dlStrategyNode_p,
    /* + TM7_8 Changes Start */
    UInt32 centralRbMask,
    UInt8 transmissionMode,
    /* SPR 15909 fix start */
    tickType_t scheduledTick,
    /* SPR 15909 fix end */
    InternalCellIndex internalCellIndex);
    /* - TM7_8 Changes End */

STATIC UInt32 getRAT0BitMapTBOneNewTxTBTwoInValid(
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        RAT0Info *rat0Info_p,
        DLStrategyTxNode *dlStrategyNode_p,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask,
        UInt8 transmissionMode,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);
        /* - TM7_8 Changes End */

STATIC  UInt32 getRAT0BitMapSpatialMux( 
    DLHARQProcess  *harqProcess_p,
    ResourceAllocatorInput *resrcAllocatorInput_p,
    RAT0Info *rat0Info_p,
    DLStrategyTxNode *dlStrategyNode_p,
    /* + TM7_8 Changes Start */
    UInt32 centralRbMask,
    UInt8 transmissionMode,
    /* SPR 15909 fix start */
    tickType_t scheduledTick,
    /* SPR 15909 fix end */
    InternalCellIndex internalCellIndex);
    /* - TM7_8 Changes End */

UInt32 getRAT0BitMapNewTxNonSpatialMux( 
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        DLStrategyTxNode *dlStrategyNode_p,     
        RAT0Info *rat0Info_p
/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
        ,DLUEContext *dlUEContext_p
#endif
        ,InternalCellIndex internalCellIndex
/*HD FDD Changes End*/
        );

/* ICIC changes start */
STATIC  UInt32 getMaxPrbSubsetIndexForNonSpatialMul(
        RAT1Info *rat1Info_p,     
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc,
        UInt32 *availableRBBitMap_p,
        ResourceRegion resourceRegionAllocated);

/* ICIC changes start */
STATIC  UInt32 getMaxPrbSubsetIndexForSpatialMul(
        RAT1Info *rat1Info_p,     
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc,
        UInt32 *availableRBBitMap_p,
        ResourceRegion resourceRegionAllocated,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask[MAX_SUBSETS],
       /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 transmissionMode);
        /* - TM7_8 Changes End */
/* ICIC changes end */
STATIC  UInt32 getRAT1BitMapSpatialMux( 
        RAT1Info *rat1Info_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc_p,
        DLStrategyTxNode *dlStrategyNode_p,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask[MAX_SUBSETS],
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 transmissionMode,
        InternalCellIndex internalCellIndex);
        /* - TM7_8 Changes End */


STATIC  UInt32 getSubsetBitMapTBOneNewTxTBTwoInValid(
        DLHARQProcess  *harqProcess_p,
        UInt32 availableRBBitMap,
        SubsetSpanCQIInfo *subetSpanCQIInfo_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        BitPositionToSubsetAndRBGInfo *bitPositionToSubsetAndRBGInfo_p,
        DLStrategyTxNode *dlStrategyNode_p,
        ResourceFormatNode *previousRsrcFrmt_p,
        UInt32 isNonZeroSpan,
        UInt32 subsetIndex,
        InternalCellIndex internalCellIndex);

STATIC UInt32 getRAT1BitMapTBOneNewTxTBTwoInValid( 
        RAT1Info *rat1Info_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc_p,
        DLStrategyTxNode *dlStrategyNode_p,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask[MAX_SUBSETS],
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 transmissionMode,
        InternalCellIndex internalCellIndex);
        /* - TM7_8 Changes End */

STATIC  UInt32 getSubsetBitMapTBOneInValidTBTwoNewTx(
        DLHARQProcess  *harqProcess_p,
        UInt32 availableRBBitMap,
        SubsetSpanCQIInfo *subetSpanCQIInfo_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        BitPositionToSubsetAndRBGInfo *bitPositionToSubsetAndRBGInfo_p,
        DLStrategyTxNode *dlStrategyNode_p,
        ResourceFormatNode *previousRsrcFrmt_p,
		UInt32 subsetIndex,
		UInt32 isZeroNonZeroSpan,
        InternalCellIndex internalCellIndex);

STATIC UInt32 getRAT1BitMapTBOneInValidTBTwoNewTx( 
        RAT1Info *rat1Info_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc_p,
        DLStrategyTxNode *dlStrategyNode_p,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask[MAX_SUBSETS],
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 transmissionMode,
        InternalCellIndex internalCellIndex);
        /* - TM7_8 Changes End */


STATIC  UInt32  getSubsetBitMapBothTBNewTx(                
        DLHARQProcess  *harqProcess_p,
        UInt32 availableRBBitMap,
        SubsetSpanCQIInfo *subetSpanCQIInfo_p,        
        ResourceAllocatorInput *resrcAllocatorInput_p,
        BitPositionToSubsetAndRBGInfo *bitPositionToSubsetAndRBGInfo_p,
        DLStrategyTxNode *dlStrategyNode_p,
        ResourceFormatMIMONode *previousRsrcFrmt_p,
        UInt32 subsetIndex,
        UInt32 isNonZeroSpan,
        InternalCellIndex internalCellIndex);

STATIC UInt32 getRAT1BitMapBothTBNewTx( 
        RAT1Info *rat1Info_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc_p,
        DLStrategyTxNode *dlStrategyNode_p,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask[MAX_SUBSETS],
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 transmissionMode,
        InternalCellIndex internalCellIndex);
        /* - TM7_8 Changes End */

STATIC  void updateRAT0InfoFromRAT1Info(
        UInt32 rbgIndexBitMap,
        UInt32 *rat0BitMap_p);

STATIC  void  updateRAT2InfoFromRAT1Info(
        UInt32  prbNumber ,
        ResourceAllocationPolicyInfo *raPolicy_p,
        InternalCellIndex internalCellIndex);

STATIC UInt32 getRAT1BitMapNewTxNonSpatialMux(
        RAT1Info *rat1Info_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess *harqProcess_p,
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc_p,
        DLStrategyTxNode *dlStrategyTxNode_p
/*HD FDD Chaanges Start*/
#ifdef HD_FDD_CONFIG
        ,DLUEContext *dlUEContext_p
#endif
        ,InternalCellIndex internalCellIndex
/*HD FDD Changes End*/
        );

STATIC void updateRAT2LocalizedCellEdgeInfo(
        RAT2LocalizedInfo *rat2LocalizedInfo_p,
        UInt8 startRBIndex,
        UInt8 endRBIndex);
/* ICIC changes end */

STATIC  UInt32 getSubSetBitMapNewTxNonSpatialMux( 
        DLHARQProcess  *harqProcess_p,
        UInt32 availableRBBitMap,
        UInt32 *rat1SpanCQIInfo_p,
        UInt32 requiredRBCount,
        UInt16 reportedCQIBitMap,
        DLStrategyTxNode *dlStrategyTxNode_p,
        ResourceFormatNode *previousResrcFmt_p,
	    UInt32 subsetIndex,
	    UInt32 isZeroNonZeroSpan
/*HD FDD Chaanges Start*/
#ifdef HD_FDD_CONFIG
        ,DLUEContext *dlUEContext_p
#endif
        ,InternalCellIndex internalCellIndex
/*HD FDD Changes End*/
        );


 MacRetType  allocRBCommonChannelRAT2Localized(
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DCIFormat format,
        InternalCellIndex internalCellIndex);

 UInt32 getCount(UInt32 bitMap);

STATIC MacRetType validateRBsforMinMaxTBSize(
        UInt32 rbgBitMap,
        RAT0Info *rat0Info_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        ResourceFormatMIMONode *previousRsrcFrmt_p,
        DLStrategyTxNode *dlStrategyNode_p,
        InternalCellIndex internalCellIndex);

STATIC MacRetType validateRAT1RBsforMinMaxTBSize(
        UInt32 rbBitMap,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        ResourceFormatMIMONode *previousRsrcFrmt_p,
        DLStrategyTxNode *dlStrategyNode_p,
        BitPositionToSubsetAndRBGInfo *bitPositionToSubsetAndRBGInfo_p,
        UInt32 subsetIndex,
        UInt32 isNonZeroSpan,
        InternalCellIndex internalCellIndex);
/* QOS PDSCH Change Start */

STATIC MacRetType allocRBInRAT0ForNonSpatialMul(
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQProcess *harqProcess_p,
        DLUEContext *dlUEContext_p,
        ResourceAllocationPolicyInfo *raPolicy_p,
        DLStrategyTxNode *dlStrategyTxNode_p
        /*Dynamic PDCCH + 5117 changes starts */
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends */
#ifdef TDD_CONFIG
      , UInt8 subFrame
#endif
         ,InternalCellIndex internalCellIndex
        );

STATIC MacRetType allocRBInRAT1ForNonSpatialMul(
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQProcess *harqProcess_p,
        DLUEContext *dlUEContext_p,
        ResourceAllocationPolicyInfo *raPolicy_p,
        DLStrategyTxNode *dlStrategyTxNode_p
        /*Dynamic PDCCH + 5117 changes starts */
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends */
#ifdef TDD_CONFIG
      , UInt8 subFrame
#endif
      ,InternalCellIndex internalCellIndex
        );

static void setSubbandtoPMIInfoForResrcAlloc(
        DLUEContext *dlUEContext_p,
        DLHARQProcess *harqProcess_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        UInt8 tempSubbandIndex);

STATIC MacRetType allocRBInRAT0ForSpatialMul(
        /* + CQI_5.0 */
        UInt32 macAdvanceFactor,
        /* - CQI_5.0 */
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQProcess *harqProcess_p,
        DLUEContext *dlUEContext_p,
        ResourceAllocationPolicyInfo *raPolicy_p,
        DLStrategyTxNode *dlStrategyTxNode_p
        /*Dynamic PDCCH + 5117 changes starts */
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends */
#ifdef TDD_CONFIG
        , UInt8 subFrame
#endif
        ,InternalCellIndex internalCellIndex
        );

STATIC MacRetType allocRBInRAT1ForSpatialMul(
        /* + CQI_5.0 */
        UInt32 macAdvanceFactor,
        /* - CQI_5.0 */
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQProcess *harqProcess_p,
        DLUEContext *dlUEContext_p,
        ResourceAllocationPolicyInfo *raPolicy_p,
        DLStrategyTxNode *dlStrategyTxNode_p 
        /*Dynamic PDCCH + 5117 changes starts */
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends */
        ,InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
        , UInt8 subFrame
#endif
        );

/* QOS PDSCH Change End */
/* + CQI_5.0 */
/* spr 23717 fix + */
static  void checkNumOfLayer(
        DLHARQProcess *harqProcess_p
        );

static   void setWideSubbandPmiforTxAnteenas(
/* spr 23717 fix - */
        DLHARQProcess *harqProcess_p,
        UInt8 *widebandPMI_p,
        DLUEContext *ueContext_p,
        ResourceAllocatorInput *resourceAllocInput_p,
        InternalCellIndex internalCellIndex
        );

#ifdef FDD_CONFIG
STATIC  MacRetType  allocRBRAT2LocalizedForSpatial(
        /* + TM6_5.2 */
        UInt32 macAdvanceFactor,
        /* - TM6_5.2 */
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
         DLUEContext *ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode *dlStrategyTxNode_p
         /*Dynamic Pdcch Changes Start*/
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic Pdcch Changes End*/
        ,InternalCellIndex internalCellIndex
        );

STATIC  MacRetType allocRBRAT2LocalizedTB1ValidTB2Invalid(
        /* + TM6_5.2 */
        UInt32 macAdvanceFactor,
        /* - TM6_5.2 */
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        /* +DYNAMIC_ICIC */
        DLUEContext *ueDLContext_p,
        /* -DYNAMIC_ICIC */
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode *dlStrategyNode_p
         /*Dynamic Pdcch Changes Start*/
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic Pdcch Changes End*/
        ,InternalCellIndex internalCellIndex
        );

#elif TDD_CONFIG
STATIC  MacRetType  allocRBRAT2LocalizedForSpatial(
        /* + TM6_5.2 */
        UInt32 macAdvanceFactor,
        /* - TM6_5.2 */
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DLUEContext *ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode *dlStrategyTxNode_p,        
         /*Dynamic Pdcch Changes Start*/
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
       UInt8 *numOfOverLappingRb,
        /*Dynamic Pdcch Changes End*/
        UInt8 subFrame,
        InternalCellIndex internalCellIndex);

STATIC  MacRetType allocRBRAT2LocalizedTB1ValidTB2Invalid(
        /* + TM6_5.2 */
        UInt32 macAdvanceFactor,
        /* - TM6_5.2 */
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        /* +DYNAMIC_ICIC */
        DLUEContext *ueDLContext_p,
        /* -DYNAMIC_ICIC */
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode *dlStrategyNode_p
        /*Dynamic PDCCH + 5117 changes starts */
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends */
        ,UInt8 subFrame,
        InternalCellIndex internalCellIndex);

#endif
/* - CQI_5.0 */
/* SPR 8624 FIX START  */
/* TO detect and surpass truncation for last UE  */
extern UInt8 lastUEDetectFlag_g[MAX_NUM_CELL];
/* SPR 8624 FIX end  */

/* + 2_Aug_2013_Fix */
/* Cyclomatic Complexity changes - starts  here */
STATIC MacRetType allocateRBRat0NonSpatialForSubBand(DLUEContext *dlUEContext_p,
                ResourceAllocatorInput * resrcAllocatorInput_p,
                ResourceAllocationPolicyInfo *raPolicy_p,
                DLHARQProcess *harqProcess_p,
                DLStrategyTxNode *dlStrategyNode_p,
                UInt32 *allocatedRb_p,
                UInt32 availableRBCount,
                UInt32 availableRBGBitMap
#ifdef TDD_CONFIG
                ,UInt8 subFrame
#endif    
                 ,UInt32 *tempRBGBitMap,
                 InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
STATIC UInt8 allocateRBRat2LocalizedForSubBand(
#ifdef HD_FDD_CONFIG
        DLUEContext *dlUEContext_p,
#endif
        /* +- SPR 17777 */
        		ResourceAllocatorInput * resourceAllocInput_p,
		        ResourceAllocationPolicyInfo *raPolicyInfo_p,
                DLHARQProcess * harqProcess_p,
                DLStrategyTxNode *dlStrategyTxNode_p,
                BestVRBSpanInfo *bestVRBSpanInfo_p,
                UInt32 *allocatedRBCount_p,
                UInt32 *availableRBCount_p,
                InternalCellIndex internalCellIndex);

/* - 2_Aug_2013_Fix */
/* Cyclomatic Complexity changes - ends  here */
/* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
STATIC void setTbSizeBasedOnSplSubframe(DLHARQProcess  *hProcess_p,
                                 UInt8          allocRb);

#elif TDD_CONFIG
STATIC void setTbSizeBasedOnSplSubframe(DLHARQProcess  *hProcess_p,
                                 UInt8          allocRb,
                                 UInt8          isSplSubframe
                                 );
#endif
/* SPR 15909 fix start */
STATIC void getNumofOverlappingRb(tickType_t    scheduledTick,
/* SPR 15909 fix end */
                           UInt32           allocatedRBCount,
                           BestVRBSpanInfo  bestVRBSpanInfo,
                           UInt8            *numOfOverLappingRb,
                           InternalCellIndex internalCellIndex);
/* Cyclomatic_complexity_changes_end */

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* ICIC changes start */
STATIC  UInt32 allocatePRBsUsingExcessPool(UInt32 mcsIndex, 
                                          UInt32 availableRBCount,
                                          UInt32 requiredTBSize,
                                          UInt32 allocatedRB,
                                          UInt8  resourceRegionAllocated,
                                          InternalCellIndex internalCellIndex);

/* SPR 5599 changes start (ZIP ID 129101) */
UInt32 allocateRBForMaxTBSize(DLHARQProcess *harqProcess_p, 
/* SPR 5599 changes end (ZIP ID 129101) */
                                            UInt32 allocatedRb, 
                                            UInt32 availableRBCount,
                                            UInt32 maxTBSize,
                                            UInt8  resourceRegionAllocated,
                                            InternalCellIndex internalCellIndex);
/* + coverity 42180 */
#ifdef MAC_AUT_TEST
STATIC  UInt32 allocateRBForMaxTBSizeBothTBValid(
                                                  DLHARQProcess *harqProcess_p, 
                                                  UInt32 allocatedRb, 
                                                  UInt32 availableRBCount,
                                                  UInt32 maxTBSize,
                                                  UInt32 mcsIndexCW1,
                                                  UInt32 mcsIndexCW2,
                                                  UInt8  resourceRegionAllocated,
                                                  InternalCellIndex internalCellIndex);
#endif
/* - coverity 42180 */

/* ICIC changes end */

STATIC  UInt32 reduceRBForMaxTBSizeBothTBValid(
                                DLHARQProcess *harqProcess_p,
                                UInt32 allocatedRB,
                                UInt32 maxTBSize,
                                UInt32 mcsIndexCW1,
                                UInt32 mcsIndexCW2,
                                UInt32 tbSizeCW1,
                                UInt32 tbSizeCW2);

/* ICIC changes start */

STATIC  MacRetType getFirstAvailableReportedVRBNewTx(
                                                BestVRBSpanInfo *bestVRBSpanInfo_p, 
                                                RAT2LocalizedInfo *rat2LocalizedInfo_p,
                                                UInt32 reportedRBGBitmap,
                                                UInt8 resourceRegionAllocated);
/* ICIC changes end */

STATIC  UInt32 selectRAT0SubbandPMIIndexRank2(
                                                UInt32 bitMapToBeReturned, 
                                                RAT0PmiInfo *reportedPmiIndex_p,
                                                UInt32 *bitMapToUse_p,
                                                InternalCellIndex internalCellIndex
                                                );

STATIC  UInt32 selectRAT0SubbandPMIIndexRank1(
                                                UInt32 bitMapToBeReturned, 
                                                RAT0PmiInfo *reportedPmiIndex_p,
                                                UInt32 *bitMapToUse_p,
                                                InternalCellIndex internalCellIndex
                                                );
/*4x2 MIMO S*/
STATIC  UInt32 selectRATSubbandPMIIndexPort4(
                                                UInt32 *bitMapToUse_p,
                                                UInt32 *reportedRBGPMIForPort);
/*4x2 MIMO E*/
STATIC  UInt32 selectRAT1SubbandPMIIndexRank2(
                                            UInt32 bitMapToBeReturned, 
                                            RAT1PmiInfo *reportedPmiIndex_p,
                                            UInt32 *bitMapToUse_p,
                                            UInt32 zeroNonZeroSpan,
                                            InternalCellIndex internalCellIndex
                                           );

STATIC  UInt32 selectRAT1SubbandPMIIndexRank1(
                                                UInt32 bitMapToBeReturned, 
                                                RAT1PmiInfo *reportedPmiIndex_p,
                                                UInt32 *bitMapToUse_p,
                                                UInt32 zeroNonZeroSpan,
                                                InternalCellIndex internalCellIndex
                                                );

/* + CQI_5.0 */
STATIC  UInt32 selectRAT0WBPMIforRank2(UInt32 bitMapToBeReturned,
                           SubBandReportInfo *subBandReportInfo_p);

STATIC  UInt32 selectRAT0WBPMIforRank1(UInt32 bitMapToBeReturned,
                           SubBandReportInfo *subBandReportInfo_p);

STATIC  UInt32 selectRAT1WBPMIforRank2(UInt32 bitMapToBeReturned,
                           UInt32 subsetIndexAllocated,
                           UInt32 isNonZeroSpanConsideredForAlloc,
                           SubBandReportInfo *subBandReportInfo_p);

STATIC  UInt32 selectRAT1WBPMIforRank1(UInt32 bitMapToBeReturned,
                           UInt32 subsetIndexAllocated,
                           UInt32 isNonZeroSpanConsideredForAlloc,
                           SubBandReportInfo *subBandReportInfo_p);
/* - CQI_5.0 */

/* + TM6_5.2 */
STATIC  UInt32 selectRAT2WBPMIIndex(SubBandReportInfo *subBandReportInfo_p);
/* - TM6_5.2 */

/******************************************************************************
 * Private Constants
 *****************************************************************************/
/* SPR 21857 changes */
/* Code Removed */
/* SPR 21857 changes */
/* +DYNAMIC_ICIC */
#define RNTP_ZERO 0
/* -DYNAMIC_ICIC */
/* coverity-530 CID 32865 */
#define DEFAULT_GAP_VALUE 1
/* coverity-530 CID 32865 */

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/*Dynamic PDCCH + 5117 changes starts */
extern McsRBInfo mcsRBMapForCodeRateUeCategory_g[(MAX_CFI_VALUE + 1)];
/*Dynamic PDCCH + 5117 changes ends */
/*ca changes end*/
/* +DYNAMIC_ICIC */
extern ICICReportConfigParams icicReportConfigParams_g;
/* -DYNAMIC_ICIC */
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/* QOS PDSCH Change Start*/
/* ICIC changes start */
/* excessPoolRb_g [0] - excess pool RBs for Cell Centre user
 * excessPoolRb_g [1] - excess pool RBs for Cell Edge user 
 */
SInt8   excessPoolRb_g[MAX_NUM_CELL][MAX_USER_LOCATION];

/* MIB nRB FIX start */
/* Number of cell centre and cell edge RBs overlapping with MIB RBs*/
UInt8   mibOverlapRBs_g[MAX_NUM_CELL][MAX_USER_LOCATION];
/* MIB nRB FIX end */
/* ICIC changes end */
/* QOS PDSCH Change End */

/* +DYNAMIC_ICIC */
/* Array to map P_A enum values to actual power offsets*/
float paValue_g[8] = {-6, -4.77, -3, -1.77, 0, 1 ,2, 3};
/* -DYNAMIC_ICIC */

/* Based on FDD/TDD (check special subframe in TDD) sets corresponding Transpot Block Size*/
#ifdef FDD_CONFIG
#define SET_TB_SIZE_BASED_ON_SPL_SUBFRAME(hProcess_p, allocRb)\
{ \
    (hProcess_p)->dlHARQTBOneInfo.tbSize = determineTBSize(\
                                             (hProcess_p)->dlHARQTBOneInfo.mcsIndex,\
                                             allocRb,\
                                             /*4X4 DL MIMO CHG START*/\
                                             TB_MAPPED_TO_ONE_LAYER\
                                             /*4X4 DL MIMO CHG END*/);\
}
#elif TDD_CONFIG
#define SET_TB_SIZE_BASED_ON_SPL_SUBFRAME(hProcess_p, allocRb, \
                                          isSplSubframe) \
{\
    if (isSplSubframe)\
    {\
       (hProcess_p)->dlHARQTBOneInfo.tbSize = determineTBSize(\
                                                (hProcess_p)->dlHARQTBOneInfo.mcsIndex,\
                                                SP_SUBFRAME_PRB(allocRb),\
                                                /*4X4 DL MIMO CHG START*/\
                                                TB_MAPPED_TO_ONE_LAYER\
                                                /*4X4 DL MIMO CHG END*/);\
    }\
    else\
    {\
        (hProcess_p)->dlHARQTBOneInfo.tbSize = determineTBSize(\
                                                  (hProcess_p)->dlHARQTBOneInfo.mcsIndex,\
                                                  allocRb,\
                                                  /*4X4 DL MIMO CHG START*/\
                                                  TB_MAPPED_TO_ONE_LAYER\
                                                  /*4X4 DL MIMO CHG END*/);\
    }\
}
#endif

/* Based on FDD/TDD (check special subframe in TDD) sets corresponding Transpot Block Size for CW1 and CW2*/
#ifdef FDD_CONFIG
#define SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,\
                                               allocatedRb)\
{/*4X4 DL MIMO CHG START*/\
    if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)\
    {\
        harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(\
                                              harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                              allocatedRb,\
                                              /*4X4 DL MIMO CHG START*/\
                                              TB_MAPPED_TO_TWO_LAYER\
                                              /*4X4 DL MIMO CHG END*/);\
        harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(\
                                              harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                              allocatedRb,\
                                              /*4X4 DL MIMO CHG START*/\
                                              TB_MAPPED_TO_TWO_LAYER\
                                              /*4X4 DL MIMO CHG END*/);\
    }\
    else if(harqProcess_p->numOfLayer == RANK_INDICATOR_3)\
    {\
        harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(\
                                              harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                              allocatedRb,\
                                              /*4X4 DL MIMO CHG START*/\
                                              TB_MAPPED_TO_ONE_LAYER\
                                              /*4X4 DL MIMO CHG END*/);\
        harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(\
                                              harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                              allocatedRb,\
                                              /*4X4 DL MIMO CHG START*/\
                                              TB_MAPPED_TO_TWO_LAYER\
                                              /*4X4 DL MIMO CHG END*/);\
    }/*4X4 DL MIMO CHG END*/\
    else\
    {\
        harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(\
                                                harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                                allocatedRb,\
                                                /*4X4 DL MIMO CHG START*/\
                                                TB_MAPPED_TO_ONE_LAYER\
                                                /*4X4 DL MIMO CHG END*/);\
        harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(\
                                                harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                                allocatedRb,\
                                                /*4X4 DL MIMO CHG START*/\
                                                TB_MAPPED_TO_ONE_LAYER\
                                                /*4X4 DL MIMO CHG END*/);\
    }\
}
#elif TDD_CONFIG
#define SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,\
                                               allocatedRb, isSplSubframe)\
{\
    if (isSplSubframe)\
    {/*4X4 DL MIMO CHG START*/\
        if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)\
        {\
            harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(\
                                                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                                    SP_SUBFRAME_PRB(allocatedRb),\
                                                    /*4X4 DL MIMO CHG START*/\
                                                    TB_MAPPED_TO_TWO_LAYER\
                                                    /*4X4 DL MIMO CHG END*/);\
            harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(\
                                                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                                    SP_SUBFRAME_PRB(allocatedRb),\
                                                    /*4X4 DL MIMO CHG START*/\
                                                    TB_MAPPED_TO_TWO_LAYER\
                                                    /*4X4 DL MIMO CHG END*/);\
        }\
        else if (harqProcess_p->numOfLayer == RANK_INDICATOR_3)\
        {\
            harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(\
                                                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                                    SP_SUBFRAME_PRB(allocatedRb),\
                                                    /*4X4 DL MIMO CHG START*/\
                                                    TB_MAPPED_TO_ONE_LAYER\
                                                    /*4X4 DL MIMO CHG END*/);\
            harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(\
                                                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                                    SP_SUBFRAME_PRB(allocatedRb),\
                                                    /*4X4 DL MIMO CHG START*/\
                                                    TB_MAPPED_TO_TWO_LAYER\
                                                    /*4X4 DL MIMO CHG END*/);\
        }/*4X4 DL MIMO CHG END*/\
        else\
        {\
            harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(\
                                                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                                    SP_SUBFRAME_PRB(allocatedRb),\
                                                    /*4X4 DL MIMO CHG START*/\
                                                    TB_MAPPED_TO_ONE_LAYER\
                                                    /*4X4 DL MIMO CHG END*/);\
            harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(\
                                                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                                    SP_SUBFRAME_PRB(allocatedRb),\
                                                    /*4X4 DL MIMO CHG START*/\
                                                    TB_MAPPED_TO_ONE_LAYER\
                                                    /*4X4 DL MIMO CHG END*/);\
        }\
    }\
    else\
    {/*4X4 DL MIMO CHG START*/\
        if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)\
        {\
            harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(\
                                                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                                    allocatedRb,\
                                                    /*4X4 DL MIMO CHG START*/\
                                                    TB_MAPPED_TO_TWO_LAYER\
                                                    /*4X4 DL MIMO CHG END*/);\
            harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(\
                                                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                                    allocatedRb,\
                                                    /*4X4 DL MIMO CHG START*/\
                                                    TB_MAPPED_TO_TWO_LAYER\
                                                       /*4X4 DL MIMO CHG END*/);\
        }\
        else if (harqProcess_p->numOfLayer == RANK_INDICATOR_3)\
        {\
            harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(\
                                                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                                    allocatedRb,\
                                                    /*4X4 DL MIMO CHG START*/\
                                                    TB_MAPPED_TO_ONE_LAYER\
                                                    /*4X4 DL MIMO CHG END*/);\
            harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(\
                                                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                                    allocatedRb,\
                                                    /*4X4 DL MIMO CHG START*/\
                                                    TB_MAPPED_TO_TWO_LAYER\
                                                       /*4X4 DL MIMO CHG END*/);\
        }/*4X4 DL MIMO CHG END*/\
        else\
        {\
            harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(\
                                                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                                    allocatedRb,\
                                                    /*4X4 DL MIMO CHG START*/\
                                                    TB_MAPPED_TO_ONE_LAYER\
                                                    /*4X4 DL MIMO CHG END*/);\
            harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(\
                                                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                                    allocatedRb,\
                                                    /*4X4 DL MIMO CHG START*/\
                                                    TB_MAPPED_TO_ONE_LAYER\
                                                       /*4X4 DL MIMO CHG END*/);\
        }\
    }\
}
#endif



/** opt **/
resAllocInitInfo_t resAllocInitInfo_g;
/** opt **/

/* +DYNAMIC_ICIC */
/* RNTP calculation is based on 36.213, section 5.2.1.
 * RNTP = 1, if (E_A/E_MAX > RNTP_thres) 
 *      = 0, otherwise
 * E_A = Ref Signal Power + rhoA 
 * rhoA = Dellta Power Offset + pA
 * Dellta Power Offset = 0 except MU-MIMO
 * E_MAX (max Nominal EPRE) = P_MAX / (100 * 12 * 15 * 1000)), where
 * 100 = Num of RBs for 20MHz
 * 12  = Number of subcarriers in a RB
 * 15 * 1000 =  subcarrier spacing in Hz 
 *
 * E_A is  calculated on per Hz basis, Hence we divide E_A  by (15 * 1000)
 * 10log10(15000) = 41.76 
 *
 * Putting all values in dB, 
 * ((RefSigPower + rhoA - 41.76) - (E_MAX) should be <= RNTP_thres
 *
 * Hence POWER_BALANCE_FACTOR = 41.76 
 */
#define POWER_BALANCE_FACTOR 41.76

/******************************************************************************
 * Function Name  : calculateRNTP 
 * Inputs         : pA - UE specific pA value
 *                  harqProcess_p - pointer to DLHARQProcess
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : Calculated RNTP value
 * Description    : This funtion calculates Relative Narrowband Transmission Power
 *                  based on 36.213, section 5.2.1
 ******************************************************************************/
 UInt8  calculateRNTP(UInt8 pA,
        DLHARQProcess *harqProcess_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 calculatedRNTP = 0;
    float paActual = paValue_g[pA];

    /* In case of SISO and QPSK, paQPSK offset is used, else UE specific pA is
     * used
     */
    if (((harqProcess_p->dlHARQTBOneInfo.mcsIndex <= MCS_INDEX_QPSK_9) && 
                (!harqProcess_p->isTBTwoValid)) ||
            ((harqProcess_p->dlHARQTBTwoInfo.mcsIndex <= MCS_INDEX_QPSK_9) && 
            (!harqProcess_p->isTBOneValid)))
    {
        paActual = paValue_g[(macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
            paQpsk];
    }
    if ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->pdschConfigCommonInfo.referenceSigPower + paActual - 
                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.
                maxNominalEPRE - POWER_BALANCE_FACTOR) >= 
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.rntpThreshold)
    {
       calculatedRNTP = 1;
    }
    return calculatedRNTP;
}
/* -DYNAMIC_ICIC */

/******************************************************************************
 * Function Name    : getCount 
 * Inputs           : bitMap - This holds BitMap on which number of set bit need to be counted.
 * Outputs          : None
 * Returns          : countOfSetBit
 * Description      : This funtion will return the number of Set Bit in the given bitmap.
 ******************************************************************************/
 UInt32 getCount(UInt32 bitMap)
{
    UInt32 countOfSetBit;
    bitMap = bitMap - ((bitMap >> 1) & 0x55555555);                 
    bitMap = (bitMap & 0x33333333) + ((bitMap >> 2) & 0x33333333);    
    countOfSetBit = (((bitMap + (bitMap >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24; 
    return countOfSetBit;
}

/******************************************************************************
 * Function Name    : FFS
 * Inputs           : bitMap - This holds BitMap on which position of first set bit from LSB Need to
 *                              to be found.
 * Outputs          : None
 * Returns          : FFS(Position of First Set Bit from LSB).
 * Description      : This funtion will return the position of first set bit from LSB.
 ******************************************************************************/
 UInt32 FFS(UInt32 bitMap)
{
    return ffs_wrapper(bitMap);
}

/*****************************************************************************
 * Function Name    : fillNGapValuesForDCIFormat1C
 * Inputs           : vDlRBs,
 *                    vDlRBsGap1,
 *                    dlRBs, 
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtion will alloate values Ngap1 based on NGapType.
 *****************************************************************************/

STATIC  void fillNGapValuesForDCIFormat1C( 
        UInt8 *vDlRBs, 
        UInt8 *vDlRBsGap1, 
        UInt32 dlRBs,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
{
    /*
     * This is for table 6.2.3.2-1 in sec 6.2.3.2
     * in spec 36.211
     */
    UInt8 RBsGap2 = 0; 
    UInt8 RBsGap1 = 0; 
    UInt8 tmp_gap1 = 0; 
    UInt8 tmp_gap2 = 0; 


    /* CMAS_CODE START*/
    if(dlRBs < 50)
    {
        if(dlRBs < 12)
        {
            if(dlRBs < 11)
            {
                /* SPR 18122 Changes Start*/
                RBsGap1 = ceil_wrapper(DIVIDE_BY_TWO(dlRBs));
                /* SPR 18122 Changes End*/
            }
            else
            {
                RBsGap1 = 4;
            }
        }
        else
        {
            if(dlRBs < 20)
            {
                RBsGap1 = 8;
            }
            else if(dlRBs < 27)
            {
                RBsGap1 = 12;
            }
            else if(dlRBs < 45)
            {
                RBsGap1 = 18;
            }
            else
            {
                RBsGap1 = 27;
            }
        }
    } //<50
    else
    {
        if(dlRBs < 64)
        {
            RBsGap1 = 27;
        }
        else if(dlRBs < 80)
        {
            RBsGap1 = 32;
        }
        else
        {
            RBsGap1 = 48;
        }
    } //>=50 and < =111

    tmp_gap1 = dlRBs - RBsGap1;

    /*
     *  To understand below logic
     *  refer to spec 6.2.3.2
     *  Virtual Resource Block Allocation incase of N_Gap = Gap1
     *  paragraph below Table 6.2.3.2
     */

    if(RBsGap1 < tmp_gap1)
    {
        *vDlRBsGap1 = *vDlRBs = (2 * RBsGap1);
    }
    else
    {
        *vDlRBsGap1 = *vDlRBs = (2 * tmp_gap1);
    }

    if( GAP_VALUE_TWO == cellSpecificParams_g.cellConfigAndInitParams_p\
               [internalCellIndex]->cellParams_p->gapValue )
    {
        if(dlRBs > 50 && dlRBs < 111)
        {
            if(dlRBs < 64)
            {
                RBsGap2 = 9;
            }
            else 
            {
                RBsGap2 = 16;
            }

            /*
             *  To understand below logic
             *  refer to spec 6.2.3.2
             *  Virtual Resource Block Allocation incase of N_Gap = Gap2
             *  paragraph below Table 6.2.3.2
             */

            tmp_gap2 = 2*RBsGap2;
            *vDlRBs = (floor_wrapper(dlRBs/tmp_gap2))*tmp_gap2; 
        }
    }
    /* CMAS_CODE END*/
}

/*****************************************************************************
 * Function Name    : populateResourceAssignmentInfo
 * Inputs           : lengthOfVRB total length of VRB,
 *                    startingVRBIndex - Index of Starting VRB,
 *                    isDistributed - This holds if the VRBs are allocated accoriding
 *                                    to RAT 2 distributed or according to RAT 2 Localized. If set
 *                                    to  one means RAT 2 Distributed else RAT 2 Localized,
 *                    format - format of VRB
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs          : resourceAssignment_p - This holds resource assignment bitmap 
 *                                          which needs to be updated
 * Returns          : None
 * Description      : This funtion will calculate the RIV Value and populate it in  
 *                    the resourceAssignment_p variable.
 *****************************************************************************/
void populateResourceAssignmentInfo( 
        UInt32 *resourceAssignment_p, 
        UInt32 lengthOfVRB,
        UInt32 startingVRBIndex,
        UInt32 isDistributed,
        DCIFormat format,
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* Below Logic is as per description mentioned in 36.213. spec for 
       calculating RIV Value */
    UInt32 dlRBs = 0;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    *resourceAssignment_p = 0;
    dlRBs = cellParams_p->dlAvailableRBs;
    startingVRBIndex--;

    if(DCI_FORMAT_1C == format && isDistributed)
    {
         if ( dlRBs >= 50 )
         {
             if ( GAP_VALUE_TWO == cellParams_p->gapValue )
             {
                 (*resourceAssignment_p) |=  ( 1 << (cellParams_p->resAllocInitInfo.numberOfBitsForAllocationDCI1C - 1));
             } 
         }
        /*
         *  According to 7.1.6.3 for DCI_format_1C
         *  length of reseoucre blocks in following
         * (nRBStep,2nRBStep,3nRBStep,.......)
         * and
         * start RB index will be
         * (0,nRBStep,2nRBStep,.....)
         */

/** opt **/
        startingVRBIndex = lengthOfVRB - cellParams_p->resAllocInitInfo.nRBStepDCI1C;
/** opt **/

        /*
         * According to Table 7.1.6.3 of RAT2 for DCI_FORMAT_1C 
         * Parmeters used for RIV
         */
/** opt **/
         lengthOfVRB = lengthOfVRB/cellParams_p->resAllocInitInfo.nRBStepDCI1C;
         startingVRBIndex = startingVRBIndex/cellParams_p->resAllocInitInfo.nRBStepDCI1C;
         dlRBs = cellParams_p->resAllocInitInfo.vDlRBDCI1C/cellParams_p->resAllocInitInfo.nRBStepDCI1C; 
/** opt **/
    }
    else
    {

        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "cellConfig_g.dlAvailableRBs = %u\n \
                isDistributed = %u\n",dlRBs, 
                isDistributed);
        if  ( isDistributed )
        {
            if ( dlRBs >= 50 )
            {
                if ( GAP_VALUE_TWO == cellParams_p->gapValue )
                {
/** opt **/
                    (*resourceAssignment_p) |=  ( 1 << (cellParams_p->resAllocInitInfo.numberOfBitsForAllocationNonDCI1C - 1));
/** opt **/
                } 
            }
        }
    }

	/* SPR 2921 Fix Start */
/** opt **/
    if ( (lengthOfVRB -1) <=  (dlRBs>>1))
/** opt **/
	/* SPR 2921 Fix End */
    {
        *resourceAssignment_p |= dlRBs * (lengthOfVRB - 1) + startingVRBIndex;
    }
    else
    {
        *resourceAssignment_p |= dlRBs * (dlRBs - lengthOfVRB + 1 ) + 
            (dlRBs - 1 - startingVRBIndex);
    }

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "resourceAssignment = %u\n",*resourceAssignment_p);
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);

}   

/*****************************************************************************
 * Function Name    : updateRAT2LocalizedInfoFromRAT2DistributedInfo
 * Inputs           : distributedVRBInfo_p - This holds pointer to 
 *                                          RAT2DistributedVRBInfo structure,
 *                    raPolicyInfo_p -This holds the pointer to 
 *                                    ResourceAllocationPolicyInfo structure which
 *                                    holds information of RAT 0, RAT 1, RAT 2 Localized and 
 *                                    RAT 2 Distributed information
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This functions will update the RAT2 Localized information
 *                    based on Available RAT 2 Distributed information.
 *****************************************************************************/
STATIC  void updateRAT2LocalizedInfoFromRAT2DistributedInfo(     
        RAT2DistributedVRBInfo *distributedVRBInfo_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        InternalCellIndex internalCellIndex)
{
    UInt32 prbMappedToEvenSlotOfVRB = 0;
    UInt32 prbMappedToOddSlotOfVRB = 0;
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;
    UInt32 previousFreeVRBIndex= 0;
    UInt32 nextFreeVRBIndex = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    localizedVRBInfo_p = raPolicyInfo_p->rat2LocalizedInfo.
        rat2LocalizedVRBInfo;

    /*Get the prbNumber which is mapped to Even Slot of VRB */
    prbMappedToEvenSlotOfVRB  = distributedVRBInfo_p->prbMappedToEvenSlotOfVRB;
    /*Get the prbNumber which is mapped to Odd Slot of VRB */
    prbMappedToOddSlotOfVRB  = distributedVRBInfo_p->prbMappedToOddSlotOfVRB;

    /*Updating the localizedVRBInfo_p array such that a Free VRB knows
      the next Free VRB Index and also previous Free VRB Index*/   
    previousFreeVRBIndex = localizedVRBInfo_p[prbMappedToEvenSlotOfVRB + 1].
        previousVRBfreeIndex;
    nextFreeVRBIndex = localizedVRBInfo_p[prbMappedToEvenSlotOfVRB + 1].
        nextfreeVRBIndex;
    localizedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex = 
        nextFreeVRBIndex;
    localizedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex = 
        previousFreeVRBIndex;
        
    /* ICIC changes start */
    if (CELL_EDGE_ONLY_REGION ==
            (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->\
            rbToResourceTypeMapDL[prbMappedToEvenSlotOfVRB])
    {
        /* If prbMappedToEvenSlotOfVRB belong to cell edge only region, 
         * then update numOfCellEdgeVRBAvailable 
         */
        raPolicyInfo_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable -= 1;
        
        /* check if prbMappedToEvenSlotOfVRB is same as firstCellEdgeFreeIndex 
         * or endCellEdgeFreeIndex, if so update these indices.
         */
        if (raPolicyInfo_p->rat2LocalizedInfo.firstCellEdgeFreeIndex == 
                (prbMappedToEvenSlotOfVRB + 1))
        {
            raPolicyInfo_p->rat2LocalizedInfo.firstCellEdgeFreeIndex = 
                nextFreeVRBIndex;
        }
        if (raPolicyInfo_p->rat2LocalizedInfo.endCellEdgeFreeIndex == 
                (prbMappedToEvenSlotOfVRB + 1))
        {
            raPolicyInfo_p->rat2LocalizedInfo.endCellEdgeFreeIndex = 
                previousFreeVRBIndex;
        }
    }
    /* ICIC changes end */

    if (prbMappedToOddSlotOfVRB != prbMappedToEvenSlotOfVRB)
    {
        /*Updating the localizedVRBInfo_p array such that a Free VRB knows
          the next Free VRB Index and also previous Free VRB Index*/   
        previousFreeVRBIndex = localizedVRBInfo_p[prbMappedToOddSlotOfVRB + 1].
            previousVRBfreeIndex;
        nextFreeVRBIndex = localizedVRBInfo_p[prbMappedToOddSlotOfVRB + 1].
            nextfreeVRBIndex;
        localizedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex = 
            nextFreeVRBIndex;
        localizedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex = 
            previousFreeVRBIndex;
        raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable -= 2;
        
        /* ICIC changes start */
        if (CELL_EDGE_ONLY_REGION ==
                (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->\
                rbToResourceTypeMapDL[prbMappedToOddSlotOfVRB])
        {
            /* If prbMappedToOddSlotOfVRB belong to cell edge only region, 
             * then update numOfCellEdgeVRBAvailable 
             */
            raPolicyInfo_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable -= 1;
            
            /* check if prbMappedToOddSlotOfVRB is same as
             * firstCellEdgeFreeIndex or endCellEdgeFreeIndex, 
             * if so update these indices.
             */
            if (raPolicyInfo_p->rat2LocalizedInfo.firstCellEdgeFreeIndex == 
                    (prbMappedToOddSlotOfVRB + 1))
            {
                raPolicyInfo_p->rat2LocalizedInfo.firstCellEdgeFreeIndex = 
                    nextFreeVRBIndex;
            }
            if (raPolicyInfo_p->rat2LocalizedInfo.endCellEdgeFreeIndex == 
                    (prbMappedToOddSlotOfVRB + 1))
            {
                raPolicyInfo_p->rat2LocalizedInfo.endCellEdgeFreeIndex = 
                    previousFreeVRBIndex;
            }
        }
        /* ICIC changes end */
    }
    else
    {   
        raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable -= 1;
    }

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}

/******************************************************************************
 * Function Name    : updateRAT0InfoFromRAT2DistributedInfo
 * Inputs           : rbgSize -  This holds the RBG Size,
 *                    distributedVRBInfo_p - This is pointer to RAT2DistributedVRBInfo
 *                                           Structure
 * Outputs          : rat0Info_p - This holds pointer to  RAT0Info structure which need to be 
 *                                updated
 * Returns          : None
 * Description      : This funtion will update  the RAT 0 information using RAT 2
 *                    Distributed Information.
 *****************************************************************************/
STATIC  void updateRAT0InfoFromRAT2DistributedInfo(
        /* +- SPR 17777 */
        RAT0Info *rat0Info_p,
        RAT2DistributedVRBInfo *distributedVRBInfo_p)

{
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
/* ICIC changes start */
     /* Updating RAT0 bitmap for cell centre user, bitmap for cell edge user 
      * is derived from cell centre user 
      */
    (rat0Info_p->rat0BitMap[CC_USER]) &= distributedVRBInfo_p->
        rbgIndexBitMapForEvenVRBSlot;

    (rat0Info_p->rat0BitMap[CC_USER]) &= distributedVRBInfo_p->
        rbgIndexBitMapForOddVRBSlot;
/* ICIC changes end */

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);

}

/*****************************************************************************
 * Function Name    : updateRAT1InfoFromRAT2DistributedInfo
 * Inputs           : subsetSize - This holds the Subset Size,
 *                    distributedVRBInfo_p - This holds pointer to  
 *                                          RAT2DistributedVRBInfo structure
 * Outputs          : subsetInfo_p - This holds pointer to SubsetInfo structure on 
 *                                  which RAT 1 Information need to be updated.
 * Returns          : None
 * Description      : This funtion will update  the RAT 1 information using RAT 2 
 *                    Distributed Information.
 *****************************************************************************/
STATIC  void updateRAT1InfoFromRAT2DistributedInfo(
        SubsetInfo *subsetInfo_p,
        /* +- SPR 17777 */
        RAT2DistributedVRBInfo *distributedVRBInfo_p)
{
    UInt32 subsetIndexForEvenVRBSlot = 0;
    UInt32 subsetIndexForOddVRBSlot = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);

    subsetIndexForEvenVRBSlot = distributedVRBInfo_p->subsetIndexForEvenVRBSlot;
    subsetIndexForOddVRBSlot = distributedVRBInfo_p->subsetIndexForOddVRBSlot;

/* ICIC changes start */
     /* Updating subset bitmap for cell centre user, bitmap for cell edge user 
      * is derived from cell centre user bitmap. 
      */
    subsetInfo_p[subsetIndexForEvenVRBSlot].subsetBitMap[CC_USER]
        &= distributedVRBInfo_p->subsetBitMapForEvenVRBSlot;

    subsetInfo_p[subsetIndexForOddVRBSlot].subsetBitMap[CC_USER]
        &= distributedVRBInfo_p->subsetBitMapForOddVRBSlot;
/* ICIC changes end */

    if ( distributedVRBInfo_p->prbMappedToEvenSlotOfVRB != 
            distributedVRBInfo_p->prbMappedToOddSlotOfVRB )
    {
        subsetInfo_p[subsetIndexForEvenVRBSlot].numPRBFree -= 1;
        subsetInfo_p[subsetIndexForOddVRBSlot].numPRBFree -= 1;
    }
    else
    {
        subsetInfo_p[subsetIndexForEvenVRBSlot].numPRBFree -= 1;        
    }

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);

}

/*****************************************************************************
 * Function Name    : updateRATInfoFromRAT2DistributedInfo
 * Inputs           : raPolicyInfo_p - This is pointer to ResourceAllocationPolicyInfo
 *                                     structure which holds the information of all 
 *                                     Resource Allocation Policy,
 *                    allocatedRBCount - This holds number of allocated RB Count
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs          : bestVRBSpanInfo_p - This is pointer to BestVRBSpanInfo structure
 *                                        which holds information of the Cont Span VRBs 
 *                                        Found. It maintains starting and ending VRB
 *                                        Index of the Span.
 * Returns          : None
 * Description      : This funtion will update the RAT 0, RAT 1, RAT 2 Localized 
 *                    Resource Alloction Info using RAT 2 Distributed.
 *****************************************************************************/
STATIC  void updateRATInfoFromRAT2DistributedInfo(
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        UInt32 allocatedRBCount,
        InternalCellIndex internalCellIndex)
{

    SubsetInfo *subSetInfo_p = PNULL;  
    RAT2DistributedVRBInfo *distributedVRBInfo_p = PNULL;    
    UInt32 vrbIndex = 0;       
    UInt32 previousFreeVRBIndex = 0;
    UInt32 nextFreeVRBIndex = 0;
    /* +- SPR 17777 */
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);

    rat0Info_p = &raPolicyInfo_p->rat0Info;
    rat1Info_p = &raPolicyInfo_p->rat1Info;   

    subSetInfo_p = rat1Info_p->subsetInfo; 
    /* +- SPR 17777 */
    vrbIndex = bestVRBSpanInfo_p->startingVRBIndex;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "allocatedRBCount = %u\n",allocatedRBCount);

    distributedVRBInfo_p = raPolicyInfo_p->rat2DistributedInfo.
        rat2DistributedVRBInfo;
    previousFreeVRBIndex = distributedVRBInfo_p[vrbIndex].previousVRBfreeIndex;
    nextFreeVRBIndex = distributedVRBInfo_p[vrbIndex + allocatedRBCount - 1]
        .nextfreeVRBIndex;
    distributedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex = 
        nextFreeVRBIndex;
    distributedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex = 
        previousFreeVRBIndex;

    while ( allocatedRBCount-- )
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "allocatedRBCount = %u\n vrbIndex = %u\n",
                allocatedRBCount,vrbIndex);      

        /* +- SPR 17777 */
        updateRAT0InfoFromRAT2DistributedInfo(rat0Info_p,
        /* +- SPR 17777 */
                &distributedVRBInfo_p[vrbIndex]);
        if ( raPolicyInfo_p->rat2LocalizedInfo.maxLocalizedVRB  > 10 )
        {
            /* +- SPR 17777 */
            updateRAT1InfoFromRAT2DistributedInfo(subSetInfo_p,
                    &distributedVRBInfo_p[vrbIndex]);
            /* +- SPR 17777 */
        }
        updateRAT2LocalizedInfoFromRAT2DistributedInfo(
                &distributedVRBInfo_p[vrbIndex],
                raPolicyInfo_p ,
                internalCellIndex);
        vrbIndex++;

    }    

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}


/*****************************************************************************
 * Function Name    : getBestContDistributedVRBNewTx
 * Inputs           : requiredRBCount - required RB Count, 
 *                    rat2DistributedInfo_p - This holds pointer to RAT2DistributedInfo
 *                                            structure,
 * Outputs          : bestVRBSpanInfo_p - This is pointer to BestVRBSpanInfo structure
 *                                        which holds information of the Cont Span VRBs 
 *                                        Found. It maintains starting and ending VRB
 *                                        Index of the Span.
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will try to find the maximum continous VRB Span 
 *                    in the array of RAT2DistributedInfo structure.
 *****************************************************************************/
STATIC MacRetType getBestContDistributedVRBNewTx(UInt32 requiredRBCount,
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        RAT2DistributedInfo *rat2DistributedInfo_p)
{
    UInt32 availableRBCount =0;
    UInt32 currentFreeVRBIndex= 0;
    BestVRBSpanInfo bestVRBSpanInfo[2]={{0,0},{0,0}};
    UInt32 maxVRBDistributed = 0;
    RAT2DistributedVRBInfo *distributedVRBInfo_p = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);

    /*Rel_523_Coverity_10180 Fix start*/
    distributedVRBInfo_p = &rat2DistributedInfo_p->rat2DistributedVRBInfo[0];
    /*Rel_523_Coverity_10180 Fix end*/
    /*Initialize the maxVRBLocalized Variable to maximum number of 
      Maximum numbe of Localized VRB */
    maxVRBDistributed = rat2DistributedInfo_p->maxDistributedVRB;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "maxVRBDistributed = %u",maxVRBDistributed);
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "requiredRBCount = %u",requiredRBCount);
    /*Initialize the currentFreeVRBIndex with the first free VRB index */
    currentFreeVRBIndex = distributedVRBInfo_p[0].nextfreeVRBIndex;

    /* Comes here when atleast there is One Free VRB */
    bestVRBSpanInfo[0].startingVRBIndex = currentFreeVRBIndex;
    bestVRBSpanInfo[0].endingVRBIndex= currentFreeVRBIndex;
    bestVRBSpanInfo[1].startingVRBIndex = currentFreeVRBIndex;
    bestVRBSpanInfo[1].endingVRBIndex= currentFreeVRBIndex;

    availableRBCount = 1;
    if (!(requiredRBCount - 1))
    {
        bestVRBSpanInfo_p->startingVRBIndex =currentFreeVRBIndex;
        bestVRBSpanInfo_p->endingVRBIndex = currentFreeVRBIndex;
        return MAC_SUCCESS;
    }
    /* SPR#706 Changes Start */
    if((MAX_DISTRIBUTED_VRB + 3) <= currentFreeVRBIndex)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,
                "[%s]:currentFreeVRBIndex  = %d so Exiting. \n"
                ,__func__, currentFreeVRBIndex);
        return MAC_FAILURE;

    }
    /* SPR#706 Changes End */

    /* Below While loop is iterating through each Free VRB Index and checking
     * if the next Free VRB index and current Free VRB Index is consecutive 
     * or not .Now if consecutive ,it will populate the bestSpanInfo[0] with
     * relevant information and then again repeat the logic .If not 
     * consecutive ,it will store the information of best continous VRB found
     * till now in bestSpanInfo[1] and then again repeat the logic.At The end
     * it will compare the continous VRB information stored in bestSpanInfo[0]
     * and bestSpan[1]Info  and return  the largest continous VRB */
    maxVRBDistributed += 1;

    /*Coverity 10180 Fix Start*/
    while ((currentFreeVRBIndex < maxVRBDistributed) && (currentFreeVRBIndex < (MAX_DISTRIBUTED_VRB + 3)))
    /*Coverity 10180 Fix End*/
    {

        /*Rel_523_Coverity_10180 Fix start*/
        distributedVRBInfo_p = &rat2DistributedInfo_p->rat2DistributedVRBInfo[currentFreeVRBIndex];
        /*Rel_523_Coverity_10180 Fix end*/
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "currentFreeVRBIndex = %u\n nextfreeVRBIndex = %u\n",
                currentFreeVRBIndex,distributedVRBInfo_p->nextfreeVRBIndex);
        /* Check if the currentFreeIndex and nextFreeIndex are consecutive or 
           not */
        if ( !(distributedVRBInfo_p->nextfreeVRBIndex -  (
                        currentFreeVRBIndex + 1))) 
        {
            availableRBCount += 1;   
            bestVRBSpanInfo[0].endingVRBIndex = currentFreeVRBIndex + 1;   

            if (availableRBCount >= requiredRBCount )
            {           
                bestVRBSpanInfo_p->startingVRBIndex = bestVRBSpanInfo[0].
                    startingVRBIndex;
                bestVRBSpanInfo_p->endingVRBIndex = bestVRBSpanInfo[0].
                    endingVRBIndex;
                return MAC_SUCCESS;
            }           
            /* Increment the currentFreeIndex so that now it points to next 
               free VRB Index */
            currentFreeVRBIndex++;  
        }
        else
        {    
            if ( (bestVRBSpanInfo[0].endingVRBIndex - bestVRBSpanInfo[0].
                        startingVRBIndex )
                    > (bestVRBSpanInfo[1].endingVRBIndex - bestVRBSpanInfo[1].
                        startingVRBIndex )) 
            {
                bestVRBSpanInfo[1].startingVRBIndex = bestVRBSpanInfo[0].
                    startingVRBIndex;
                bestVRBSpanInfo[1].endingVRBIndex =   bestVRBSpanInfo[0].
                    endingVRBIndex;
            }
            currentFreeVRBIndex = distributedVRBInfo_p->nextfreeVRBIndex;
            /* Intializing the  the bestSpanInfo[0] with the currentFreeVRBIndex */
            bestVRBSpanInfo[0].startingVRBIndex  = currentFreeVRBIndex;
            bestVRBSpanInfo[0].endingVRBIndex  = currentFreeVRBIndex;                    

            availableRBCount = 1;
        }
    }   

    if ( (bestVRBSpanInfo[0].endingVRBIndex - bestVRBSpanInfo[0].
                startingVRBIndex )
            > (bestVRBSpanInfo[1].endingVRBIndex - bestVRBSpanInfo[1].
                startingVRBIndex )) 
    {
        bestVRBSpanInfo_p->startingVRBIndex = bestVRBSpanInfo[0].
            startingVRBIndex;
        bestVRBSpanInfo_p->endingVRBIndex =   bestVRBSpanInfo[0].
            endingVRBIndex;
    }
    else
    {
        bestVRBSpanInfo_p->startingVRBIndex = bestVRBSpanInfo[1].
            startingVRBIndex;
        bestVRBSpanInfo_p->endingVRBIndex =   bestVRBSpanInfo[1].
            endingVRBIndex;        
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s",__func__);
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name    : getBestContDistributedVRBReTx
 * Inputs           : requiredRBCount - required RB Count, 
 *                    rat2DistributedInfo_p - This holds pointer to RAT2DistributedInfo
 *                                            structure
 * Outputs          : bestVRBSpanInfo_p - This is pointer to BestVRBSpanInfo structure
 *                                        which holds information of the Cont Span VRBs Found.
 *                                        It maintains  starting and ending VRB Index of the Span.
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will try to find the maximum continous VRB Span 
 *                    in the array of RAT2LocalizedVRBInfo structure.
 *****************************************************************************/
STATIC MacRetType getBestContDistributedVRBReTx(UInt32 requiredRBCount,
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        RAT2DistributedInfo *rat2DistributedInfo_p)
{
    UInt32 availableRBCount =0;
    UInt32 currentFreeVRBIndex= 0;
    BestVRBSpanInfo bestVRBSpanInfo ={0,0};
    UInt32 maxVRBDistributed = 0;
    RAT2DistributedVRBInfo *distributedVRBInfo_p = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);

    /*Rel_523_Coverity_10181 Fix start*/
    distributedVRBInfo_p = &rat2DistributedInfo_p->rat2DistributedVRBInfo[0];
    /*Rel_523_Coverity_10181 Fix end*/
    /*Initialize the maxVRBLocalized Variable to maximum number of 
      Maximum numbe of Localized VRB */
    maxVRBDistributed = rat2DistributedInfo_p->maxDistributedVRB;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "maxVRBDistributed = %u",maxVRBDistributed);
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "requiredRBCount = %u",requiredRBCount);
    /*Initialize the currentFreeVRBIndex with the first free VRB index */
    bestVRBSpanInfo_p->startingVRBIndex = 0;
    bestVRBSpanInfo_p->endingVRBIndex = 0;
    currentFreeVRBIndex = distributedVRBInfo_p[0].nextfreeVRBIndex;

    /* Comes here when atleast there is One Free VRB */
    bestVRBSpanInfo.endingVRBIndex = currentFreeVRBIndex;
    bestVRBSpanInfo.startingVRBIndex = currentFreeVRBIndex;

    availableRBCount = 1;
    if (!(requiredRBCount - 1))
    {
        bestVRBSpanInfo_p->startingVRBIndex =currentFreeVRBIndex;
        bestVRBSpanInfo_p->endingVRBIndex = currentFreeVRBIndex;
        return MAC_SUCCESS;
    }
    /* SPR#706 Changes Start */
    if((MAX_DISTRIBUTED_VRB + 3) <= currentFreeVRBIndex)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,
                "[%s]:currentFreeVRBIndex  = %d so Exiting. \n"
                ,__func__, currentFreeVRBIndex);
        return MAC_FAILURE;

    }
    /* SPR#706 Changes End */

    /* Below While loop is iterating through each Free VRB Index and checking
     * if the next Free VRB index and current Free VRB Index is consecutive 
     * or not .Now if consecutive ,it will populate the bestSpanInfo[0] with
     * relevant information and then again repeat the logic .If not 
     * consecutive ,it will store the information of best continous VRB found
     * till now in bestSpanInfo[1] and then again repeat the logic.At The end
     * it will compare the continous VRB information stored in bestSpanInfo[0]
     * and bestSpan[1]Info  and return  the largest continous VRB */
    maxVRBDistributed += 1;

    /*Coverity 10181 Fix Start*/
    while ((currentFreeVRBIndex < maxVRBDistributed) && (currentFreeVRBIndex< (MAX_DISTRIBUTED_VRB + 3)))
    /*Coverity 10181 Fix End*/
    {

        /*Rel_523_Coverity_10181 Fix start*/
        distributedVRBInfo_p = &rat2DistributedInfo_p->rat2DistributedVRBInfo[currentFreeVRBIndex];
        /*Rel_523_Coverity_10181 Fix end*/
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "currentFreeVRBIndex = %u\n nextfreeVRBIndex = %u\n",
                currentFreeVRBIndex,distributedVRBInfo_p->nextfreeVRBIndex);
        /* Check if the currentFreeIndex and nextFreeIndex are consecutive or 
           not */
        if ( !(distributedVRBInfo_p->nextfreeVRBIndex -  (
                        currentFreeVRBIndex + 1))) 
        {
            availableRBCount += 1;   
            bestVRBSpanInfo.endingVRBIndex = currentFreeVRBIndex + 1;   

            if (availableRBCount >= requiredRBCount )
            {           
                bestVRBSpanInfo_p->startingVRBIndex = bestVRBSpanInfo.
                    startingVRBIndex;
                bestVRBSpanInfo_p->endingVRBIndex = bestVRBSpanInfo.
                    endingVRBIndex;
                return MAC_SUCCESS;
            }           
            /* Increment the currentFreeIndex so that now it points to next 
               free VRB Index */
            currentFreeVRBIndex++;  
        }
        else
        {    
            currentFreeVRBIndex = distributedVRBInfo_p->nextfreeVRBIndex;
            /* Intializing the  the bestSpanInfo[0] with the 
               currentFreeVRBIndex */
            bestVRBSpanInfo.startingVRBIndex  = currentFreeVRBIndex;

            availableRBCount = 1;          

        }
    }       

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "startingVRBIndex = %u",bestVRBSpanInfo_p->startingVRBIndex );
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "endingVRBIndex = %u",bestVRBSpanInfo_p->endingVRBIndex );
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s",__func__);

    return MAC_SUCCESS;
}
/******************************************************************************
 * Function Name    : allocRBRAT2Distributed
 * Inputs           : resourceAllocInput_p - This is pointer to Resource Allocator 
 *                                             Input Structure,
 *                    raPolicyInfo_p - This holds pointer to 
 *                                     ResourceAllocationPolicyInfo structure,
 *                    wideBandMcsIndex -  This holds the wideBandCQI Index,
 *                    harqProcess_p - This points to DLHARQProcess structure,
 *                    subFrame - current sub frame,
 *                    dlUEContext_p - pointer to DLUEContext
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs          : resourceAllocOutput_p - This is pointer to Resource Allocator 
 *                                              Output on which once Resources are allocated,
 *                                              relevant information can be written.                    
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion allocate the RBs  for New Transmission and 
 *                    Re Transmission according and Re Transmission to RAT 2 
 *                    Distributed and once RBs gets allocated update the RAT 0, 
 *                    RAT 1, RAT 2 Localized Information.
 ******************************************************************************/
STATIC  MacRetType  allocRBRAT2Distributed(
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        UInt32 wideBandMcsIndex,
        DLHARQProcess *harqProcess_p
/* SPR 4184 Chg */
#ifdef TDD_CONFIG 
        ,UInt8 subFrame
#endif
/* SPR 4184 Chg */
        /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
        ,DLUEContext *dlUEContext_p
#endif
        ,InternalCellIndex internalCellIndex
        /* HD FDD Changes End */
)
{
    BestVRBSpanInfo bestVRBSpanInfo ={0,0};
    UInt32 availableRBCount =0;  
    UInt32 tbSize  = 0;
    UInt32 requiredRBCount = 0;
    UInt32 allocatedRBCount = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);
    if (!raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable)
    {
        return MAC_FAILURE;    
    }

    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
   /* SPR 15909 fix start */
    tickType_t tick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
    HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(tick, dlUEContext_p, wideBandMcsIndex);
#endif
    /* HD FDD Changes End */

    if ( NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator)
    {
        /*getBestContDistributedVRBNewTx  funtion will try to find  the maximum
          continousfree VRBs ( equal to resourceAllocInput_p->requiredRB) 
          in the RAT2DistributedVRBInfo array which holds the information of  
          Free VRB .If it's not able to find then it will return  the best 
          continous VRB starting and ending Position in the the same  array 
          and populate it in the bestVRBSpanInfo structure*/

        requiredRBCount = resourceAllocInput_p->requiredRB;
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "requiredRBCount = %u",resourceAllocInput_p->requiredRB);
        /* SPR#706 Changes Start */
        if( MAC_FAILURE == getBestContDistributedVRBNewTx(requiredRBCount, &bestVRBSpanInfo, 
                    &raPolicyInfo_p->rat2DistributedInfo))
        {
            return MAC_FAILURE;    
        }
        /* SPR#706 Changes End */
        availableRBCount = bestVRBSpanInfo.endingVRBIndex - bestVRBSpanInfo.
            startingVRBIndex + 1;

        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "availableRBCount = %u",availableRBCount);
        /*Cov 87048 + */
        if ( availableRBCount < requiredRBCount)
        {
            allocatedRBCount = availableRBCount;
        }
        /*Cov 87048 - */
        else
        {
            allocatedRBCount = requiredRBCount;
        }


/* SPR 4184 Chg */
#ifdef FDD_CONFIG /*Coverity 87048 fix*/
        tbSize = determineTBSize(wideBandMcsIndex, allocatedRBCount,
                                    /*4X4 DL MIMO CHG START*/
                                    TB_MAPPED_TO_ONE_LAYER
                                    /*4X4 DL MIMO CHG END*/);
#elif TDD_CONFIG
        if (checkForSpecialSubframe(subFrame, internalCellIndex))
        {
            tbSize = determineTBSize(wideBandMcsIndex, SP_SUBFRAME_PRB(allocatedRBCount),
                                     /*4X4 DL MIMO CHG START*/
                                     TB_MAPPED_TO_ONE_LAYER
                                     /*4X4 DL MIMO CHG END*/);
        }
        else
        {
            tbSize = determineTBSize(wideBandMcsIndex, allocatedRBCount,
                                     /*4X4 DL MIMO CHG START*/
                                     TB_MAPPED_TO_ONE_LAYER
                                     /*4X4 DL MIMO CHG END*/);
        }
#endif
/* SPR 4184 Chg */
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "tbSize = %u",tbSize);
        if ( tbSize < HARQ_TB_MINIMUM_SIZE_IN_BYTES )
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "Leaving funtion = %s with Failure\n",__func__);
            return MAC_FAILURE;
        }

        raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable -= 
            allocatedRBCount;
        harqProcess_p->dlHARQTBOneInfo.tbSize = tbSize;
        harqProcess_p->dlHARQTBOneInfo.mcsIndex = wideBandMcsIndex;
        harqProcess_p->assignedResourcesNB = allocatedRBCount;
        resourceAllocOutput_p->allocatedRB = allocatedRBCount;
        resourceAllocOutput_p->ratPolicy = RA_POLICY_2_DISTRIBUTED;
        resourceAllocOutput_p->startingVRBIndex = bestVRBSpanInfo.
            startingVRBIndex - 1;
        populateResourceAssignmentInfo(&resourceAllocOutput_p-> 
                resourceAssignmentBitMap, allocatedRBCount, bestVRBSpanInfo.
                startingVRBIndex,1,harqProcess_p->dciFormat,internalCellIndex);           
        updateRATInfoFromRAT2DistributedInfo(&bestVRBSpanInfo, 
                raPolicyInfo_p, allocatedRBCount,internalCellIndex);    
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "Leaving funtion = %s with Success\n",__func__);
        return MAC_SUCCESS;
    }

    else
    {
        /*getBestContDistributedVRBReTx  funtion will try to find  the maximum
          continousfree VRBs ( equal to resourceAllocInput_p->requiredRB) 
          in the RAT2DistributedVRBInfo array which holds the information of  
          Free VRB .If it's not able to find then it will return  the best 
          continous VRB starting and ending Position in the the same  array 
          and populate it in the bestVRBSpanInfo structure*/
        tbSize = harqProcess_p->dlHARQTBOneInfo.tbSize;
        /* + SPR_17858_383_CHANGES */
        requiredRBCount = harqProcess_p->assignedResourcesNB;
        /* - SPR_17858_383_CHANGES */
        if (raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable < 
                requiredRBCount)
        {
            return MAC_FAILURE;    
        }

        /* SPR#706 Changes Start */
        if( MAC_FAILURE == getBestContDistributedVRBReTx(requiredRBCount, &bestVRBSpanInfo, 
                &raPolicyInfo_p->rat2DistributedInfo))
        {
            return MAC_FAILURE;    
        }
        /* SPR#706 Changes End */

        availableRBCount = bestVRBSpanInfo.endingVRBIndex - bestVRBSpanInfo.
            startingVRBIndex + 1;

        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "availableRBCount = %u",availableRBCount);

        if (availableRBCount < requiredRBCount)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "Leaving Funtion with Failure as  availableRBCount <  \
                    = requiredRBCount\n");
            return MAC_FAILURE;   
        }    
        harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize = tbSize;
        raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable -= 
            requiredRBCount;
        resourceAllocOutput_p->allocatedRB = requiredRBCount;
        resourceAllocOutput_p->ratPolicy = RA_POLICY_2_DISTRIBUTED;
        resourceAllocOutput_p->startingVRBIndex = bestVRBSpanInfo.
            startingVRBIndex - 1;
        populateResourceAssignmentInfo(&resourceAllocOutput_p-> 
                resourceAssignmentBitMap, requiredRBCount, bestVRBSpanInfo.
                startingVRBIndex,1,harqProcess_p->dciFormat,internalCellIndex);           
        updateRATInfoFromRAT2DistributedInfo(&bestVRBSpanInfo, 
                raPolicyInfo_p, requiredRBCount,internalCellIndex);    
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "Leaving funtion = %s with Success\n",__func__);
        return MAC_SUCCESS;
    }

}

/*****************************************************************************
 * Function Name    : updateRAT2DistributedInfoFromRAT2LocalizedInfo
 * Inputs           : localizedVRBInfo_p - This is pointer to RAT2LocalizedVRBInfo 
 *                                         Structure,
 *                    raPolicyInfo_p -  This holds the pointer to 
 *                                      ResourceAllocationPolicyInfo structure which holds 
 *                                      information of RAT 0, RAT 1, RAT 2 Localized and 
 *                                      RAT 2 Distributed information
 * Outputs          : None      
 * Returns          : None
 * Description      : This funtion will update  the RAT 2 Distributed information 
 *                    using RAT 2  Localized Information.
 *****************************************************************************/
/* + PRS_CHANGES */
 void updateRAT2DistributedInfoFromRAT2LocalizedInfo(     
        RAT2LocalizedVRBInfo *localizedVRBInfo_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p)
/* - PRS_CHANGES */
{
    UInt32 vrbMappedToOddSlotOfPRB = 0;
    UInt32 vrbMappedToEvenSlotOfPRB = 0;
    RAT2DistributedVRBInfo *distributedVRBInfo_p = PNULL;  
    UInt32 previousFreeVRBIndex= 0;
    UInt32 nextFreeVRBIndex = 0;    

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);

    distributedVRBInfo_p = raPolicyInfo_p->rat2DistributedInfo.
        rat2DistributedVRBInfo;

    /*Get  the vrbNumber which is mapped to even slot of PRB*/
    vrbMappedToEvenSlotOfPRB  = localizedVRBInfo_p->vrbMappedToEvenSlotOfPRB;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "vrbMappedToEvenSlotOfPRB= %u vrbMappedToOddSlotOfPRB= %u\n",
            vrbMappedToEvenSlotOfPRB, vrbMappedToOddSlotOfPRB);
    /*Get  the vrbNumber which is mapped to odd slot of PRB*/
    vrbMappedToOddSlotOfPRB  = localizedVRBInfo_p->vrbMappedToOddSlotOfPRB;
    /*Updating the distributedVRBInfo_p array such that a Free VRB knows
      the next Free VRB Index and also previous Free VRB Index*/     
    previousFreeVRBIndex = distributedVRBInfo_p[vrbMappedToEvenSlotOfPRB].
        previousVRBfreeIndex;
    nextFreeVRBIndex = distributedVRBInfo_p[vrbMappedToEvenSlotOfPRB].
        nextfreeVRBIndex;
    distributedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex = 
        nextFreeVRBIndex;
    distributedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex = 
        previousFreeVRBIndex;

    /* RAT2 distributed support for ICIC needs to be added */


    if ( vrbMappedToEvenSlotOfPRB != vrbMappedToOddSlotOfPRB )
    {

        /*Updating the distributedVRBInfo_p array such that a Free VRB knows
          the next Free VRB Index and also previous Free VRB Index*/          
        previousFreeVRBIndex = distributedVRBInfo_p[vrbMappedToOddSlotOfPRB].
            previousVRBfreeIndex;
        nextFreeVRBIndex = distributedVRBInfo_p[vrbMappedToOddSlotOfPRB].
            nextfreeVRBIndex;
        distributedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex = 
            nextFreeVRBIndex;
        distributedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex = 
            previousFreeVRBIndex;
        raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable -= 2;

        /* RAT2 distributed support for ICIC needs to be added */
    }
    else
    {
        raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable -= 1;
    }   
    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s",__func__);
}


/*****************************************************************************
 * Function Name    : updateRAT0InfoFromRAT2LocalizedInfo
 * Inputs           : rbgIndexBitMap - This holds biMap wrt to RBG Index Allocated
 * Outputs          : rat0BitMap_p - This holds pointer to rat0BitMap Variable which 
 *                                   needs to be updated
 * Returns          : None
 * Description      : This funtion will update  the RAT 0  information using RAT 2 
 *                    Localized Information.
 *****************************************************************************/
STATIC  void updateRAT0InfoFromRAT2LocalizedInfo(
        UInt32 rbgIndexBitMap,
        UInt32 *rat0BitMap_p)
{

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);

    (*rat0BitMap_p) &= rbgIndexBitMap;    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s",__func__);
}


/*****************************************************************************
 * Function Name    : updateRAT1InfoFromRAT2LocalizedInfo
 * Inputs           : subsetBitMap - This holds the subset BitMap in which the bit
 *                                   mapped to allocatd RB is unset,
 *                    subsetIndex - This holds the subset in which the allocated RBGIndexInfo
 *                                  falls
 * Outputs          : subsetInfo_p : This holds pointer to SubsetInfo Structure
 * Returns          : None
 * Description      : This funtion will update  the RAT 1  information using RAT 2 
 *                    Localized Information.
 *****************************************************************************/
/* + PRS_CHANGES */
 void updateRAT1InfoFromRAT2LocalizedInfo(
        SubsetInfo *subsetInfo_p,
        UInt32 subsetBitMap,
        UInt32 subsetIndex)
/* - PRS_CHANGES */
{    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__); 

    subsetInfo_p[subsetIndex].numPRBFree -= 1;

    /* ICIC changes start */
    /* Updating subset bitmap for cell centre user, bitmap for cell edge user 
     * is derived from cell centre user bitmap. 
     */
    subsetInfo_p[subsetIndex].subsetBitMap[CC_USER] &= subsetBitMap;
    /* ICIC changes end */
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s",__func__);
}


/*****************************************************************************
 * Function Name    : updateRATInfoFromRAT2LocalizedInfo
 * Inputs           : raPolicyInfo_p - This is pointer to 
 *                                     ResourceAllocationPolicyInfo structure which
 *                                     holds the information of all Resource 
 *                                     Allocation Policy,
 *                    allocatedRBCount - This stores the count of Allocated RBs,
 *                    calculatedRNTP -
 *                    updateRAT2DistributedFlag - whether to update RAT2 Distributed
 *                                                information or not,
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : bestVRBSpanInfo_p - This is pointer to BestVRBSpanInfo structure
 *                                        which holds information of the Cont Span VRBs Found.
 *                                        It maintains starting and ending VRB Index of the Span.
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will update the RAT 0 ,RAT 1,RAT 2 Distributed 
 *                    Resource Alloction Info using RAT 2 Localized.
 *****************************************************************************/
void updateRATInfoFromRAT2LocalizedInfo(
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        UInt32 allocatedRBCount,
        UInt32 updateRAT2DistributedFlag,
        /* +DYNAMIC_ICIC */
        UInt8 calculatedRNTP,
        InternalCellIndex internalCellIndex)
        /* -DYNAMIC_ICIC */
{
    SubsetInfo *subSetInfo_p = PNULL;  
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;   
    RAT2LocalizedVRBInfo *tempLocalizedVRBInfo_p = PNULL;  
    UInt8 vrbIndex = 0;      
    UInt8 endVrbIndex = 0;      
    UInt8 previousFreeVRBIndex = 0;
    UInt8 nextFreeVRBIndex = 0;
/*    UInt32 rbgSize = 0; TDD Warning Fix */
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;


    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR, 
            "Entering funtion = %s",__func__);

    rat0Info_p = &raPolicyInfo_p->rat0Info;
    rat1Info_p = &raPolicyInfo_p->rat1Info;

    subSetInfo_p = rat1Info_p->subsetInfo;
/*    rbgSize = rat0Info_p->rbgSize;TDD Warning Fix */

    localizedVRBInfo_p = raPolicyInfo_p->rat2LocalizedInfo.rat2LocalizedVRBInfo;
    vrbIndex = bestVRBSpanInfo_p->startingVRBIndex;
    endVrbIndex = bestVRBSpanInfo_p->endingVRBIndex;

    /*Updating the localizedVRBInfo_p array such that a Free VRB knows
      the next Free VRB Index and also previous Free VRB Index*/       
    previousFreeVRBIndex = localizedVRBInfo_p[vrbIndex].previousVRBfreeIndex;
    nextFreeVRBIndex = localizedVRBInfo_p[vrbIndex + allocatedRBCount - 1].
        nextfreeVRBIndex;
    localizedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex =  
        nextFreeVRBIndex;
    localizedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex =  
        previousFreeVRBIndex;

    while ( (allocatedRBCount--) && (vrbIndex <= endVrbIndex) )
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "vrbIndex = %u",vrbIndex);
        tempLocalizedVRBInfo_p = &localizedVRBInfo_p[vrbIndex];
        /* ICIC changes start */
        if (updateRAT2DistributedFlag)
        {
            updateRAT2DistributedInfoFromRAT2LocalizedInfo(
                    tempLocalizedVRBInfo_p,
                    raPolicyInfo_p);
        }
        updateRAT0InfoFromRAT2LocalizedInfo(tempLocalizedVRBInfo_p->
                rbgIndexBitMap, &rat0Info_p->rat0BitMap[CC_USER]);
        /* ICIC changes end */
        if ( raPolicyInfo_p->rat2LocalizedInfo.maxLocalizedVRB  > 10 )
        {
            updateRAT1InfoFromRAT2LocalizedInfo(subSetInfo_p, 
                    tempLocalizedVRBInfo_p->subsetBitMap,
                    tempLocalizedVRBInfo_p->subsetIndex);  
        }
#ifdef PERF_STATS
        /* Overlapping RB global is updated*/
        /** CA-TDD Changes Start **/
	if(perfStatsMibSyncSignalFlag_g[internalCellIndex] && 
			vrbIndex >= central6RBStartIndex_g[internalCellIndex] && 
			vrbIndex <= central6RBStartIndex_g[internalCellIndex] + 5)
        /** CA-TDD Changes Stop **/
	{
		overlapRBsMibSS_g[internalCellIndex]++;
	}
#endif
        /* +DYNAMIC_ICIC */
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd. \
        dlICICReportInfo_p[rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
        activeReportIndex].rntpValuePerPRB[vrbIndex - 1] += calculatedRNTP;
        /* -DYNAMIC_ICIC */
        
        vrbIndex++;
    }    

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s",__func__);
}

/*****************************************************************************
 * Function Name    : getBestContLocalizedVRBNewTx
 * Inputs           : requiredRBCount - required RB Count,
 *                    rat2LocalizedInfo_p - pointer to RAT2LocalizedInfo,
 *                    resourceRegionAllocated - Allocated Resource region,
 *                    maxRBs - Maximum number of RB's
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : bestVRBSpanInfo_p - This is pointer to BestVRBSpanInfo structure
 *                                        which holds information of the Cont Span VRBs Found.
 *                                        It maintains starting and ending VRB Index of the Span.
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will try to find the maximum continous VRB Span 
 *                    in the array of RAT2LocalizedVRBInfo structure.
 *****************************************************************************/
/* ICIC changes start */
MacRetType getBestContLocalizedVRBNewTx(UInt32 requiredRBCount,
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        RAT2LocalizedInfo *rat2LocalizedInfo_p,
        UInt8 resourceRegionAllocated,
        UInt32 maxRBs,
        InternalCellIndex internalCellIndex)
/* ICIC changes end */
{
    UInt32 availableRBCount =0;
    BestVRBSpanInfo bestVRBSpanInfo[2]={{0,0},{0,0}};
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;
    UInt32 currentFreeVRBIndex = 0;
    UInt32 maxVRBLocalized = 0;

    SInt8 excessPoolRBs = excessPoolRb_g[internalCellIndex][resourceRegionAllocated];
    UInt8 extraRequiredRBs = 0;
    UInt8 tempExtraRequiredRBs = 0;
      
    /* For max TB Size*/
    extraRequiredRBs = maxRBs - requiredRBCount;

    if (excessPoolRBs > 0) 
    {
        if(excessPoolRBs < extraRequiredRBs)
        {
            extraRequiredRBs = excessPoolRBs ;
        }    
    }
    else
    {
        extraRequiredRBs = 0;
    }
    extraRequiredRBs++ ;
    tempExtraRequiredRBs = extraRequiredRBs;

    /*Rel_523_Coverity_10182 Fix start*/
    localizedVRBInfo_p = &rat2LocalizedInfo_p->rat2LocalizedVRBInfo[0];
    /*Rel_523_Coverity_10182 Fix end*/

/* ICIC changes start */
    if (CC_CE_REGION == resourceRegionAllocated)
    {
        /*Initialize the currentFreeVRBIndex with the first free VRB index */
        currentFreeVRBIndex = localizedVRBInfo_p[0].nextfreeVRBIndex;
        maxVRBLocalized = rat2LocalizedInfo_p->maxLocalizedVRB;
    }
    else
    {
        currentFreeVRBIndex = rat2LocalizedInfo_p->firstCellEdgeFreeIndex;
        maxVRBLocalized = rat2LocalizedInfo_p->endCellEdgeFreeIndex;
    }
/* ICIC changes end */

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "maxVRBLocalized = %u\n",maxVRBLocalized);
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "requiredRBCount = %u\n",requiredRBCount);
    /* Comes here when atleast there is One Free VRB */
    bestVRBSpanInfo[0].startingVRBIndex = currentFreeVRBIndex;
    bestVRBSpanInfo[0].endingVRBIndex= currentFreeVRBIndex;
    bestVRBSpanInfo[1].startingVRBIndex = currentFreeVRBIndex;
    bestVRBSpanInfo[1].endingVRBIndex= currentFreeVRBIndex;

    availableRBCount = 1;
    if (!(requiredRBCount - 1))
    {
        bestVRBSpanInfo_p->startingVRBIndex =currentFreeVRBIndex;
        bestVRBSpanInfo_p->endingVRBIndex = currentFreeVRBIndex;
        return MAC_SUCCESS;
    }
    /* SPR#706 Changes Start */
    if((MAX_LOCALIZED_VRB + 3) <= currentFreeVRBIndex)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,
                "[%s]:currentFreeVRBIndex  = %d so Exiting. \n"
                ,__func__, currentFreeVRBIndex);
        return MAC_FAILURE;

    }
    /* SPR#706 Changes End */

    /* Below While loop is iterating through each Free VRB Index and checking
     * if the next Free VRB index and current Free VRB Index is consecutive 
     * or not .Now if consecutive ,it will populate the bestSpanInfo[0] with
     * relevant information and then again repeat the logic .If not 
     * consecutive ,it will store the information of best continous VRB found
     * till now in bestSpanInfo[1] and then again repeat the logic.At The end
     * it will compare the continous VRB information stored in bestSpanInfo[0]
     * and bestSpan[1]Info  and return  the largest continous VRB */
    maxVRBLocalized += 1;

    /* Coverity 10182 Fix Start*/
    while ( (currentFreeVRBIndex < maxVRBLocalized) && (currentFreeVRBIndex< (MAX_LOCALIZED_VRB + 3)) )
    /* Coverity 10182 Fix End*/
    {

        /*Rel_523_Coverity_10182 Fix start*/
        localizedVRBInfo_p = &rat2LocalizedInfo_p->rat2LocalizedVRBInfo[currentFreeVRBIndex];
        /*Rel_523_Coverity_10182 Fix end*/
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "currentFreeVRBIndex = %u\n nextfreeVRBIndex = %u\n",
                currentFreeVRBIndex,localizedVRBInfo_p->nextfreeVRBIndex);
        /* Check if the currentFreeIndex and nextFreeIndex are consecutive or 
           not */
        if ( !(localizedVRBInfo_p->nextfreeVRBIndex -  (
                        currentFreeVRBIndex + 1))) 
        {
            availableRBCount += 1;   
            bestVRBSpanInfo[0].endingVRBIndex = currentFreeVRBIndex + 1;   

            if (availableRBCount >= requiredRBCount )
            { 
                /*now try to reach maxRBs */            
                extraRequiredRBs--;
                if ((extraRequiredRBs > 0) && ((currentFreeVRBIndex + 1) < maxVRBLocalized))
                {
                    /* Increment the currentFreeIndex so that now it points to next free VRB Index */
                    currentFreeVRBIndex++;  
                    localizedVRBInfo_p++; 
                    continue;                    
                }
                else
                {
                    bestVRBSpanInfo_p->startingVRBIndex = bestVRBSpanInfo[0].
                        startingVRBIndex;
                    bestVRBSpanInfo_p->endingVRBIndex = bestVRBSpanInfo[0].
                        endingVRBIndex;
                    return MAC_SUCCESS;
                }
            }           
            /* Increment the currentFreeIndex so that now it points to next 
               free VRB Index */
            currentFreeVRBIndex++;  
        }
        else
        {    
            if ( (bestVRBSpanInfo[0].endingVRBIndex - bestVRBSpanInfo[0].
                        startingVRBIndex )
                    > (bestVRBSpanInfo[1].endingVRBIndex - bestVRBSpanInfo[1].
                        startingVRBIndex )) 
            {
                bestVRBSpanInfo[1].startingVRBIndex = bestVRBSpanInfo[0].
                    startingVRBIndex;
                bestVRBSpanInfo[1].endingVRBIndex =   bestVRBSpanInfo[0].
                    endingVRBIndex;
            }

            currentFreeVRBIndex = localizedVRBInfo_p->nextfreeVRBIndex;
            /* Intializing the  the bestSpanInfo[0] with the currentFreeVRBIndex */
            bestVRBSpanInfo[0].startingVRBIndex  = currentFreeVRBIndex;
            bestVRBSpanInfo[0].endingVRBIndex  = currentFreeVRBIndex;                    
            availableRBCount = 1;
            extraRequiredRBs = tempExtraRequiredRBs;
        }
    }   

    if ( (bestVRBSpanInfo[0].endingVRBIndex - bestVRBSpanInfo[0].
                startingVRBIndex )
            > (bestVRBSpanInfo[1].endingVRBIndex - bestVRBSpanInfo[1].
                startingVRBIndex )) 
    {
        bestVRBSpanInfo_p->startingVRBIndex = bestVRBSpanInfo[0].
            startingVRBIndex;
        bestVRBSpanInfo_p->endingVRBIndex =   bestVRBSpanInfo[0].
            endingVRBIndex;
    }
    else
    {
        bestVRBSpanInfo_p->startingVRBIndex = bestVRBSpanInfo[1].
            startingVRBIndex;
        bestVRBSpanInfo_p->endingVRBIndex =   bestVRBSpanInfo[1].
            endingVRBIndex;        
    }
    
    LOG_UT(MAC_RAT2_LOCALIZED_VRB_SPAN,LOGDEBUG,
            MAC_PDSCH,getCurrentTick(),
            (UInt32)bestVRBSpanInfo_p->startingVRBIndex,
            (UInt32)bestVRBSpanInfo_p->endingVRBIndex,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"\nRAT2 Localized VRB span");
    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name    : getBestContLocalizedVRBReTx
 * Inputs           : requiredRBCount - required RB Count, 
 *                    rat2LocalizedInfo_p - pointer to RAT2LocalizedInfo,
 *                    resourceRegionAllocated - Allocated resource region
 * Outputs          : bestVRBSpanInfo_p - This is pointer to BestVRBSpanInfo structure
 *                                        which holds information of the Cont Span VRBs Found.
 *                                        It maintains starting and ending VRB Index of the Span.
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will try to find the maximum continous VRB Span 
 *                    in the array of RAT2LocalizedVRBInfo structure.
 *****************************************************************************/
/* ICIC changes start */
STATIC MacRetType getBestContLocalizedVRBReTx(UInt32 requiredRBCount,
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        RAT2LocalizedInfo *rat2LocalizedInfo_p,
        UInt8 resourceRegionAllocated)
/* ICIC changes end */
{
    UInt32 availableRBCount =0;
    BestVRBSpanInfo bestVRBSpanInfo = {0,0};
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;
    UInt32 currentFreeVRBIndex = 0;
    UInt32 maxVRBLocalized = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);

/* ICIC changes start */
    if (CC_CE_REGION == resourceRegionAllocated)
    {
        /*Initialize the currentFreeVRBIndex with the first free VRB index */
        currentFreeVRBIndex = rat2LocalizedInfo_p->rat2LocalizedVRBInfo[0].
            nextfreeVRBIndex;
        maxVRBLocalized = rat2LocalizedInfo_p->maxLocalizedVRB;
    }
    else
    {
        currentFreeVRBIndex = rat2LocalizedInfo_p->firstCellEdgeFreeIndex;
        maxVRBLocalized = rat2LocalizedInfo_p->endCellEdgeFreeIndex;
    }
/* ICIC changes end */

    bestVRBSpanInfo_p->startingVRBIndex = 0;
    /*Cov 72816 +-*/
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "maxVRBLocalized = %u\n",maxVRBLocalized);
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "requiredRBCount = %u\n",requiredRBCount);

    bestVRBSpanInfo_p->startingVRBIndex = 0;
    bestVRBSpanInfo_p->endingVRBIndex = 0;
 
    /* Comes here when atleast there is One Free VRB */
    bestVRBSpanInfo.endingVRBIndex = currentFreeVRBIndex;
    bestVRBSpanInfo.startingVRBIndex = currentFreeVRBIndex;

    availableRBCount = 1;
    if (!(requiredRBCount - 1))
    {
        bestVRBSpanInfo_p->startingVRBIndex =currentFreeVRBIndex;
        bestVRBSpanInfo_p->endingVRBIndex = currentFreeVRBIndex;
        return MAC_SUCCESS ;
    }
    /* SPR#706 Changes Start */
    if((MAX_LOCALIZED_VRB + 3) <= currentFreeVRBIndex)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,
                "[%s]:currentFreeVRBIndex  = %d so Exiting. \n"
                ,__func__, currentFreeVRBIndex);
        return MAC_FAILURE;

    }
    /* SPR#706 Changes End */


    /* Below While loop is iterating through each Free VRB Index and checking
     * if the next Free VRB index and current Free VRB Index is consecutive 
     * or not .Now if consecutive ,it will populate the bestSpanInfo[0] with
     * relevant information and then again repeat the logic .If not 
     * consecutive ,it will store the information of best continous VRB found
     * till now in bestSpanInfo[1] and then again repeat the logic.At The end
     * it will compare the continous VRB information stored in bestSpanInfo[0]
     * and bestSpan[1]Info  and return  the largest continous VRB */
    maxVRBLocalized += 1;

    /*Coverity 10183 Fix Start*/
    while ((currentFreeVRBIndex < maxVRBLocalized) && (currentFreeVRBIndex< (MAX_LOCALIZED_VRB + 3)) )
    /*Coverity 10183 Fix End*/
    {

    /*Rel_523_Coverity_10183 Fix start*/
        localizedVRBInfo_p = &rat2LocalizedInfo_p->rat2LocalizedVRBInfo[currentFreeVRBIndex];
    /*Rel_523_Coverity_10183 Fix End*/
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "currentFreeVRBIndex = %u\n nextfreeVRBIndex = %u\n",
                currentFreeVRBIndex,localizedVRBInfo_p->nextfreeVRBIndex);
        /* Check if the currentFreeIndex and nextFreeIndex are consecutive or 
           not */
        if ( !(localizedVRBInfo_p->nextfreeVRBIndex -  (
                        currentFreeVRBIndex + 1))) 
        {
            availableRBCount += 1;   
            bestVRBSpanInfo.endingVRBIndex = currentFreeVRBIndex + 1;   

            if (availableRBCount >= requiredRBCount )
            {           
                bestVRBSpanInfo_p->startingVRBIndex = bestVRBSpanInfo.
                    startingVRBIndex;
                bestVRBSpanInfo_p->endingVRBIndex = bestVRBSpanInfo.
                    endingVRBIndex;
                return MAC_SUCCESS;
            }           
            /* Increment the currentFreeIndex so that now it points to next 
               free VRB Index */
            currentFreeVRBIndex++;  
        }
        else
        {               
            currentFreeVRBIndex = localizedVRBInfo_p->nextfreeVRBIndex;            
            bestVRBSpanInfo.startingVRBIndex  = currentFreeVRBIndex;

            availableRBCount = 1;            
        }
    }
    
    LOG_UT(MAC_RAT2_LOCALIZED_VRB_SPAN,LOGDEBUG,
            MAC_PDSCH,getCurrentTick(),
            (UInt32)bestVRBSpanInfo_p->startingVRBIndex,
            (UInt32)bestVRBSpanInfo_p->endingVRBIndex,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"\nRAT2 Localized VRB span");
    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s",__func__);
    return MAC_SUCCESS;
}
/******************************************************************************
 * Function Name    : allocRBRAT2Localized
 * Inputs           : resourceAllocInput_p - This is pointer to Resource Allocator 
 *                                           Input Structure,
 *                    raPolicyInfo_p - This holds pointer to ResourceAllocationPolicyInfo 
 *                                     structure,
 *                    dlUEContext_p -  UE downlink context pointer,
 *                    harqProcess_p - This points to DLHARQProcess structure,
 *                    dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                    scheduledTick - Scheduled TTI,
 *                    numOfOverLappingRb - Number of overlapping RB's
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : resourceAllocOutput_p - This is pointer to Resource Allocator 
 *                                            Output on which once Resources are allocated,
 *                                            relevant information can be written           
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion allocate the RBs  for New Transmission and 
 *                    Re Transmission according and Re Transmission to RAT 2 
 *                    Localized and once RBs gets allocated update the RAT 0, 
 *                    RAT 1, RAT 2 Distributed Information.
 ******************************************************************************/
/*HD FDD Changes Start*/
#ifdef FDD_CONFIG
/* ICIC changes start */
STATIC  MacRetType  allocRBRAT2Localized(
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        /* +DYNAMIC_ICIC */
        DLUEContext *dlUEContext_p,
        /* -DYNAMIC_ICIC */
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode *dlStrategyTxNode_p
        /*Dynamic Pdcch Changes Start*/
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic Pdcch Changes End*/
        ,InternalCellIndex internalCellIndex
        )
#elif TDD_CONFIG
STATIC  MacRetType  allocRBRAT2Localized(
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        /* +DYNAMIC_ICIC */
        DLUEContext *dlUEContext_p,
        /* -DYNAMIC_ICIC */
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode *dlStrategyTxNode_p        
        /*Dynamic PDCCH + 5117 changes starts */
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends */
        ,UInt8 subFrame
        ,InternalCellIndex internalCellIndex)
/* ICIC changes end */
#endif
/*HD FDD Changes End*/
{
    BestVRBSpanInfo bestVRBSpanInfo ={0,0};
    UInt32 availableRBCount =0;  
    UInt32 tbSize  = 0;
    UInt32 requiredRBCount = 0;
    UInt32 allocatedRBCount = 0;

#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe = checkForSpecialSubframe(subFrame, internalCellIndex);
#endif          

    UInt8 isWidebandAllocationRequired = 1;
    UInt32 minTBSize = dlStrategyTxNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyTxNode_p->maxTBSize;
    
    if (maxTBSize < minTBSize)
    {
        lteWarning("[RAT2 NonSpatial] Wrong Max TBSize calculated by DL Strategy\
                     minTBSize [%d] maxTBSize [%d]\n", minTBSize, maxTBSize);
        maxTBSize = minTBSize;
    }       
    
/* ICIC changes start */
    UInt32 resourceRegionAllocated = 
        resourceAllocInput_p->resourceRegionAllocated;
    /* This flag is used to trigger update of RAT2 Distributed information,
     * As of now this is only updated for common channel messages, 
     * as RAT2 distributed is not used for data messages. 
     */
    UInt32 updateRAT2DistributedFlag = FALSE;
    
    /* +DYNAMIC_ICIC */
    /*spr 15832 +*/
    UInt32 wideBandMcsIndex = INVALID_MCS_INDEX; 
    /*spr 15832 -*/

    /* This parameter represents calculated RNTP value of UE getting scheduled 
     * calculation is based on 36.213, section 5.2.1 
     */
    UInt8 calculatedRNTP = 0;
    /* -DYNAMIC_ICIC */
/* ICIC changes end */

    /*spr 15832 +*/
    if(IS_PCELL_OF_UE(dlUEContext_p,internalCellIndex))
    {
        wideBandMcsIndex = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordOne; 
    }
    else
    {
        wideBandMcsIndex = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordOne;
    }
    /*spr 15832 -*/
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);

    if (!raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable)
    {
        return MAC_FAILURE;    
    }

    if ( NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator)
    {
        if ((CE_REGION == resourceRegionAllocated) &&
                (!raPolicyInfo_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable))
        {
            /* No Cell Edge VRB is available for cell edge new transmission */
            UPDATE_EXCESS_POOL_RB(CE_REGION, resourceAllocInput_p->requiredRB, internalCellIndex);
            return MAC_FAILURE;
        }

        requiredRBCount = resourceAllocInput_p->requiredRB;

        /*if subbband pointer is available then this indicates the allocation with Subband CQI*/
        if ( (resourceAllocInput_p->subBandReportInfo_p) && 
             ( (harqProcess_p->dlHARQTBOneInfo.taDrxFlag == INVALID_TA_DRX_MSG) ||
               ( (dlStrategyTxNode_p->msgType == SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL) ||
                 (dlStrategyTxNode_p->msgType == SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL))))
        {
            /* Cyclomatic Complexity changes - starts  here */
            isWidebandAllocationRequired = allocateRBRat2LocalizedForSubBand( 
                    /* +- SPR 17777 */
#ifdef HD_FDD_CONFIG
                    dlUEContext_p,
#endif
                    /* +- SPR 17777 */
                    resourceAllocInput_p,
                    raPolicyInfo_p,
                    harqProcess_p,
                    dlStrategyTxNode_p,
                    &bestVRBSpanInfo,
                    &allocatedRBCount,
                    &availableRBCount,
                    internalCellIndex);
            /* Cyclomatic Complexity Changes -ends here*/
        }

        if (isWidebandAllocationRequired)
        {
            UInt8 maxRBs = 0;
            
            /* As of now, there is no use case for strictAllocSubBand
             * strictAllocSubBand check is commented */

            if (minTBSize < maxTBSize)
            {
                /* SPR 5999 changes start */
                maxRBs = DETERMINE_RBs_DL(wideBandMcsIndex, maxTBSize, internalCellIndex);
                /* SPR 5999 changes end */
            }
            else
            {
                maxRBs = requiredRBCount;
            }

            if( MAC_FAILURE == getBestContLocalizedVRBNewTx(requiredRBCount, 
                                                            &bestVRBSpanInfo, 
                                                            &raPolicyInfo_p->rat2LocalizedInfo,
                                                            resourceRegionAllocated,
                                                            maxRBs,
                                                            internalCellIndex))
            {
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, requiredRBCount, internalCellIndex);
                /* ICIC changes end */
                return MAC_FAILURE;    
            }

            availableRBCount = (bestVRBSpanInfo.endingVRBIndex - 
                                bestVRBSpanInfo.startingVRBIndex) + 1;

            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "availableRBCount = %u",availableRBCount);

            if (availableRBCount < requiredRBCount)
            {
                if (dlStrategyTxNode_p->strictAllocatedFlag)
                {
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, requiredRBCount, internalCellIndex);
                    /* ICIC changes end */
                    return MAC_FAILURE;
                }
            }
            allocatedRBCount = availableRBCount;

            //update excessPool 
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                    (requiredRBCount - allocatedRBCount),
                    internalCellIndex);
            /* ICIC changes end */

            harqProcess_p->dlHARQTBOneInfo.mcsIndex = wideBandMcsIndex;
        }   
        /* SPR 13903 fix start */
        harqProcess_p->assignedResourcesNB = allocatedRBCount;
        /*Dynamic PDCCH + 5117 changes starts */  
        if (MAC_FAILURE == checkCodeRateUECategory(
                RA_POLICY_2_LOCALIZED, 
                scheduledTick,
                harqProcess_p,
                dlUEContext_p,
                TRUE,
                0,
                /* +- SPR 17777 */
                internalCellIndex))
        {
             /* SPR 13903 fix start */
                LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                        MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                        RA_POLICY_2_LOCALIZED,harqProcess_p->assignedResourcesNB,
                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
                /* SPR 13903 fix end */

            return MAC_FAILURE;
        }
        /*Dynamic PDCCH + 5117 changes ends */
        /* SPR 15959 fix start */
        /* +DYNAMIC_ICIC */
        if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
        {
            calculatedRNTP = calculateRNTP(dlUEContext_p->pA, harqProcess_p, internalCellIndex);
        }
        /* -DYNAMIC_ICIC */
        /* SPR 15959 fix end */

        allocatedRBCount = harqProcess_p->assignedResourcesNB;
   
        raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable -= allocatedRBCount;
        /* ICIC changes start */

          /* There is no need to update cell edge information if resource region 
         * allocated is CC_CE_REGION for NEW TX case, in such case allocation 
         * for cell edge users is already done.
         */
        if (CE_REGION == resourceRegionAllocated)
        {
            updateRAT2LocalizedCellEdgeInfo(&raPolicyInfo_p->rat2LocalizedInfo,
                    bestVRBSpanInfo.startingVRBIndex, 
                    bestVRBSpanInfo.endingVRBIndex);
        }
        /* ICIC changes end */
       /* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
        setTbSizeBasedOnSplSubframe(harqProcess_p,
                                          allocatedRBCount);
#elif TDD_CONFIG
        setTbSizeBasedOnSplSubframe(harqProcess_p,
                                          allocatedRBCount,
                                          isSpecialSubframe);
#endif
        /* Cyclomatic_complexity_changes_end */
        /* SPR 13903 fix end */

        resourceAllocOutput_p->allocatedRB = allocatedRBCount;
        resourceAllocOutput_p->ratPolicy = RA_POLICY_2_LOCALIZED;
        resourceAllocOutput_p->startingVRBIndex = bestVRBSpanInfo.
            startingVRBIndex - 1;
        populateResourceAssignmentInfo(&resourceAllocOutput_p-> 
                resourceAssignmentBitMap, allocatedRBCount, bestVRBSpanInfo.
                startingVRBIndex,0,harqProcess_p->dciFormat,internalCellIndex);           
        /* +DYNAMIC_ICIC */
        updateRATInfoFromRAT2LocalizedInfo(&bestVRBSpanInfo, raPolicyInfo_p,
                allocatedRBCount, updateRAT2DistributedFlag, calculatedRNTP, 
                internalCellIndex);    
        /* -DYNAMIC_ICIC */
    }
    else
    {
        /*getBestContLocalizedVRBReTx  funtion will try to find  the maximum
          continousfree VRBs ( equal to resourceAllocInput_p->requiredRB) 
          in the RAT2LocalizedVRBInfo array which holds the information of  
          Free VRB .If it's not able to find then it will return starting 
          and ending VRB Index as Zero*/
        tbSize = harqProcess_p->dlHARQTBOneInfo.tbSize;
        /* +SPR 5228, 5146, 5300 Changes Start*/
        requiredRBCount = harqProcess_p->assignedResourcesNB;
        /* -SPR 5228, 5146, 5300 Changes end */
        if (raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable < 
                requiredRBCount)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "Leaving Funtion with Failure as  numOfVRBAvailable <  \
                    = requiredRBCount\n");
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, requiredRBCount, internalCellIndex);
            /* ICIC changes end */
            return MAC_FAILURE;   
        }

        /* ICIC changes start */
        if ((CE_REGION == resourceAllocInput_p->resourceRegionAllocated) &&
                (raPolicyInfo_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable < 
                 requiredRBCount))
        {
            /* Here retransmission is from cell edge region & suffiecient cell 
             * edge resources are not available. Hence try to allocate from 
             * cell centre region. 
             */
            resourceRegionAllocated = CC_CE_REGION;
        }
        /* SPR#706 Changes Start */
        if( MAC_FAILURE == getBestContLocalizedVRBReTx(requiredRBCount, &bestVRBSpanInfo, 
                &raPolicyInfo_p->rat2LocalizedInfo, resourceRegionAllocated))
        /* ICIC changes end */
        {
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, requiredRBCount, internalCellIndex);
            /* ICIC changes end */
            return MAC_FAILURE;    
        }
        /* SPR#706 Changes End */


        availableRBCount = bestVRBSpanInfo.endingVRBIndex - bestVRBSpanInfo.
            startingVRBIndex + 1;

        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "availableRBCount = %u",availableRBCount);

        if (availableRBCount < requiredRBCount)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "Leaving Funtion with Failure as  availableRBCount <  \
                    = requiredRBCount\n");
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, requiredRBCount, internalCellIndex);
            /* ICIC changes end */
            return MAC_FAILURE;   
        }            
                 /*Dynamic PDCCH + 5117 changes starts */  
        if (MAC_FAILURE == checkCodeRateUECategory(
                RA_POLICY_2_LOCALIZED, 
                scheduledTick,
                harqProcess_p,
                dlUEContext_p,
                TRUE,
                0,
                /* +- SPR 17777 */
                internalCellIndex))
        {
            /* SPR 13903 fix start */
            LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                    MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                    RA_POLICY_2_LOCALIZED,harqProcess_p->assignedResourcesNB,
                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            /* SPR 13903 fix end */

            return MAC_FAILURE;
        }
        /*Dynamic PDCCH + 5117 changes ends */
        /* SPR 15959 fix start */
        /* +DYNAMIC_ICIC */
        if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
        {
            calculatedRNTP = calculateRNTP(dlUEContext_p->pA, harqProcess_p, internalCellIndex);
        }
        /* -DYNAMIC_ICIC */
        /* SPR 15959 fix end */

        requiredRBCount = harqProcess_p->assignedResourcesNB;
        /* SPR 13903 fix end */
        raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable -= requiredRBCount;
        /* ICIC changes start */
        updateRAT2LocalizedCellEdgeInfo(&raPolicyInfo_p->rat2LocalizedInfo,
                bestVRBSpanInfo.startingVRBIndex,
                bestVRBSpanInfo.endingVRBIndex);
        /* ICIC changes end */
        harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize = tbSize;
        resourceAllocOutput_p->allocatedRB = requiredRBCount;
        resourceAllocOutput_p->startingVRBIndex = bestVRBSpanInfo.
            startingVRBIndex - 1;
        resourceAllocOutput_p->ratPolicy = RA_POLICY_2_LOCALIZED;
        populateResourceAssignmentInfo(&resourceAllocOutput_p-> 
                resourceAssignmentBitMap, requiredRBCount, bestVRBSpanInfo.
                startingVRBIndex,0,harqProcess_p->dciFormat,
                internalCellIndex);           
        /* ICIC changes start */
        /* +DYNAMIC_ICIC */
        updateRATInfoFromRAT2LocalizedInfo(&bestVRBSpanInfo, raPolicyInfo_p,
                requiredRBCount, updateRAT2DistributedFlag, calculatedRNTP,
                internalCellIndex);    
        /* -DYNAMIC_ICIC */
        /* ICIC changes end */
    }
    /*Dynamic PDCCH + 5117 changes ends */
    /* SPR 13903 fix start */
    /* +DYNAMIC_ICIC */
    /*Coverity 73571 Changes Start*/
    /*if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
    {
        calculatedRNTP = calculateRNTP(dlUEContext_p->pA, harqProcess_p, internalCellIndex);
    }*/
    /*Coverity 73571 Changes End*/
    /* -DYNAMIC_ICIC */

    /* Cyclomatic_complexity_changes_start */
    getNumofOverlappingRb(scheduledTick,
                               allocatedRBCount,
                               bestVRBSpanInfo,
                               numOfOverLappingRb,
                               internalCellIndex
                               );
    /* Cyclomatic_complexity_changes_end */
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
              "Leaving funtion = %s with Success\n",__func__);
    /* SPR 13903 fix end */
        
    return MAC_SUCCESS;

}

/******************************************************************************
 * Function Name    : allocRBRAT2LocalizedForSpatial
 * Inputs           : resourceAllocInput_p - This is pointer to Resource Allocator 
 *                                             Input Structure,
 *                    raPolicyInfo_p - This holds pointer to 
 *                                     ResourceAllocationPolicyInfo structure,
 *                    ueDLContext_p -  UE downlink context pointer,
 *                    harqProcess_p - This points to DLHARQProcess structure,
 *                    dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                    scheduledTick - Scheduled TTI,
 *                    numOfOverLappingRb - Number of overlapping RB's
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : resourceAllocOutput_p - This is pointer to Resource Allocator 
 *                                            Output on which once Resources are allocated,
 *                                            relevant information can be written
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion allocate the RBs  for New Transmission and 
 *                    Re Transmission according and Re Transmission to RAT 2 
 *                    Localized and once RBs gets allocated update the RAT 0, 
 *                    RAT 1, RAT 2 Distributed Information.
 ******************************************************************************/
#ifdef FDD_CONFIG
STATIC  MacRetType  allocRBRAT2LocalizedForSpatial(
        /* + TM6_5.2 */
        UInt32 macAdvanceFactor,
        /* - TM6_5.2 */
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
         DLUEContext *ueDLContext_p,
         DLHARQProcess *harqProcess_p,
         DLStrategyTxNode *dlStrategyTxNode_p
         /*Dynamic PDCCH + 5117 changes starts */
        /* SPR 15909 fix start */
         ,tickType_t scheduledTick
        /* SPR 15909 fix end */
         ,UInt8 *numOfOverLappingRb
         ,InternalCellIndex internalCellIndex
         /*Dynamic PDCCH + 5117 changes ends */)
#elif TDD_CONFIG
STATIC  MacRetType  allocRBRAT2LocalizedForSpatial(
        /* + TM6_5.2 */
        UInt32 macAdvanceFactor,
        /* - TM6_5.2 */
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DLUEContext *ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode *dlStrategyTxNode_p,        
	/*Dynamic PDCCH + 5117 changes starts */
    /* SPR 15909 fix start */
    tickType_t scheduledTick,
    /* SPR 15909 fix end */
	UInt8 *numOfOverLappingRb,
	/*Dynamic PDCCH + 5117 changes ends */
        UInt8 subFrame,
        InternalCellIndex internalCellIndex)
#endif
{
    /*coverity-530 CID-32308*/
    MacRetType resp = MAC_FAILURE;
    /*coverity-530 CID-32308*/
    if (!raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable)
    {
        return MAC_FAILURE;    
    }
    /* SPR 5720 fix start*/
    if ((harqProcess_p->isTBOneValid)&&(FALSE == harqProcess_p->isTBTwoValid))
    /* SPR 5720 fix end*/
    {
        /* Fill the MCS Index CodeWord1 from the DL UE Context */
        resp = allocRBRAT2LocalizedTB1ValidTB2Invalid(
                /* + TM6_5.2 */
                macAdvanceFactor,
                /* - TM6_5.2 */
                resourceAllocInput_p,
                resourceAllocOutput_p,
                raPolicyInfo_p,
                /* +DYNAMIC_ICIC */
                ueDLContext_p,
                /* -DYNAMIC_ICIC */
                harqProcess_p,
                dlStrategyTxNode_p
                /*Dynamic PDCCH + 5117 changes starts */
                ,scheduledTick
                ,numOfOverLappingRb
                /*Dynamic PDCCH + 5117 changes ends */
#ifdef TDD_CONFIG                
                ,subFrame
#endif          
                /* SPR 11559 Start */      
                ,internalCellIndex
                /* SPR 11559 End */      
                );
    }    

   return resp;

}   
/*Rel 5.3.1 cyclomatic chg start*/ 
/******************************************************************************
 * Function Name : checkNumOfLayer 
 * Inputs  : harqProcess_p - This points to DLHARQProcess structure.
 * Returns  : None
 * Description : This funtion set pmiIndex from numOfLayer in harqProcess 
 ******************************************************************************/
/* spr 23717 fix + */
static  void checkNumOfLayer(DLHARQProcess *harqProcess_p)
/* spr 23717 fix - */
{
    /*4x4 DL MIMO CHG START*/
    if (RI_LAYER_4 == harqProcess_p->numOfLayer) 
    {
        harqProcess_p->pmiIndex = PMI_INDEX_34;
    }
    else
    {
        harqProcess_p->pmiIndex = PMI_INDEX_17;
    }
    /*4x4 DL MIMO CHG END*/
    return;
}

/******************************************************************************
 * Function Name : setWideSubbandPmiforTxAnteenas 
 * Inputs : 1)resourceAllocInput_p - This is pointer to Resource Allocator 
 *            Input Structure.
 *          2)widebandPMI_p - This holds pointer to widebandPMI.
 *          3)dlUEContext_p -  UE downlink context pointer
 *          4)harqProcess_p - This points to DLHARQProcess structure.
 *          5)internalCellIndex -  Cell-Index at MAC
 * Outputs : 1)resourceAllocOutput_p - This is pointer to Resource Allocator 
 *             Output on which once Resources are allocated ,relevant 
 *             information can be written.                    
 * Returns  : None
 * Description : This funtion set the Wideband and Subband Pmi values  
 *               according to the TxAnteenas value.
 ******************************************************************************/
/* spr 23717 fix + */
static  void setWideSubbandPmiforTxAnteenas(DLHARQProcess *harqProcess_p,
/* spr 23717 fix - */
                                           UInt8 *widebandPMI_p,
                                           DLUEContext *ueContext_p,
                                           ResourceAllocatorInput *resourceAllocInput_p,
                                           InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
    {
        if(( 5 != harqProcess_p->pmiIndex ) &&
           ( 6 != harqProcess_p->pmiIndex ) ) 
        {     
            *widebandPMI_p = selectRAT2WBPMIIndex(
                          resourceAllocInput_p->subBandReportInfo_p); 
            harqProcess_p->pmiIndex = *widebandPMI_p;
            ueContext_p->dlMIMOInfo.isAperMode12 = TRUE;
        }
    }
    /* 4x4 DL MIMO CHG START */
    else if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        if (PMI_INDEX_16 <= harqProcess_p->pmiIndex)
        {
            *widebandPMI_p = selectRAT2WBPMIIndex(
                          resourceAllocInput_p->subBandReportInfo_p);
            harqProcess_p->pmiIndex = *widebandPMI_p;
            ueContext_p->dlMIMOInfo.isAperMode12 = TRUE;
        }
    }
    /* 4x4 DL MIMO CHG END */
    return;
}
/*Rel 5.3.1 cyclomatic chg end*/

/******************************************************************************
 * Function Name : allocRBRAT2LocalizedTB1ValidTB2Invalid
 * Inputs           : macAdvanceFactor,
 *                    resourceAllocInput_p - Pointer to ResourceAllocatorInput,
 *                    raPolicyInfo_p - Pointer to ResourceAllocationPolicyInfo,
 *                    dlUEContext_p - Pointer to DLUEContext,
 *                    harqProcess_p - Pointer to DLHARQProcess,
 *                    dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                    scheduledTick - Scheduled TTI,
 *                    numOfOverLappingRb - Number of Overlapping RB's   
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : resourceAllocOutput_p - This is pointer to Resource Allocator 
 *                                            Output on which once Resources are allocated,
 *                                            relevant information can be written           
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion allocate the RBs according to TB 1  for New 
 *                    Transmission and Re Transmission accordingly and Re Transmission 
 *                    to RAT 2 Localized and once RBs gets allocated update the RAT 0, 
 *                    RAT 1, RAT 2 Distributed Information
 ******************************************************************************/
#ifdef FDD_CONFIG
STATIC  MacRetType  allocRBRAT2LocalizedTB1ValidTB2Invalid(
        /* + TM6_5.2 */
        UInt32 macAdvanceFactor,
        /* - TM6_5.2 */
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        /* +DYNAMIC_ICIC */
        DLUEContext *dlUEContext_p,
        /* -DYNAMIC_ICIC */
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode *dlStrategyTxNode_p
        /*Dynamic Pdcch Changes Start*/
       /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic Pdcch Changes End*/
        ,InternalCellIndex internalCellIndex
        )
#elif TDD_CONFIG
STATIC  MacRetType  allocRBRAT2LocalizedTB1ValidTB2Invalid(
        /* + TM6_5.2 */
        UInt32 macAdvanceFactor,
        /* - TM6_5.2 */
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        /* +DYNAMIC_ICIC */
        DLUEContext *dlUEContext_p,
        /* -DYNAMIC_ICIC */
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode *dlStrategyTxNode_p        
        /*Dynamic PDCCH + 5117 changes starts */
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends */
        ,UInt8 subFrame
        ,InternalCellIndex internalCellIndex)
#endif
{
    BestVRBSpanInfo bestVRBSpanInfo ={0,0};
    UInt32 availableRBCount =0;  
    UInt32 tbSize  = 0;
    UInt32 requiredRBCount = 0;
    UInt32 allocatedRBCount = 0; 
#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe = checkForSpecialSubframe(subFrame, internalCellIndex);
#endif          

    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    UInt8 isWidebandAllocationRequired = 1;
    UInt32 minTBSize = dlStrategyTxNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyTxNode_p->maxTBSize;

    /* ICIC changes start */
    UInt8 resourceRegionAllocated = resourceAllocInput_p->resourceRegionAllocated;
    /* This flag is used to trigger update of RAT2 Distributed information,
     * As of now this is only updated for common channel messages,
     * as RAT2 distributed is not used for data messages.
     */
    UInt32 updateRAT2DistributedFlag = FALSE;
    
    /* +DYNAMIC_ICIC */

    /*spr 15832 +*/
    UInt32 wideBandMcsIndex = INVALID_MCS_INDEX;
    /*spr 15832 -*/

    /* This parameter represents calculated RNTP value of UE getting scheduled 
     * calculation is based on 36.213, section 5.2.1 
     */
    UInt8 calculatedRNTP = 0;
    /* -DYNAMIC_ICIC */
    /* ICIC changes end */

    /* + TM6_5.2 */
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *ueContext_p = PNULL;
    dlUEContextInfo_p = &dlUECtxInfoArr_g[dlStrategyTxNode_p->ueIndex];
    ueContext_p = dlUEContextInfo_p->dlUEContext_p;
    UInt32 vrbIndex = 0;      
    UInt8 tempSubbandIndex = 0;
    UInt8 prevSubBandNum = 255;  
    UInt8 newSubBandNum = 255;  
    UInt32 rbgIndex = 0;
    UInt8 widebandPMI = ueContext_p->dlCQIInfo.wideBandPMIIndex;
    /*SubbandPMIInfo *subbandPMIInfo_p = PNULL;
    subbandPMIInfo_p = &(resourceAllocInput_p->subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo);*/
    /* - TM6_5.2 */
   
    /*spr 15832 +*/
    if(IS_PCELL_OF_UE(dlUEContext_p,internalCellIndex))
    {
        wideBandMcsIndex = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordOne; 
    }
    else
    {
        wideBandMcsIndex = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordOne;
    }
    /*spr 15832 -*/
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);

    if (maxTBSize < minTBSize)
    {
        lteWarning("[RAT2 Spatial with TB1 valid] Wrong Max TBSize calculated\
                     by DL Strategy minTBSize [%d] maxTBSize [%d]\n", 
                     minTBSize, maxTBSize);
        
        maxTBSize = minTBSize;
    }

    if ( NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator)
    {
        /* ICIC changes start */
        if ((CE_REGION == resourceRegionAllocated) &&
                (!raPolicyInfo_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable))
        {
            /* No Cell Edge VRB is available for cell edge new transmission */
            UPDATE_EXCESS_POOL_RB(CE_REGION, resourceAllocInput_p->requiredRB, internalCellIndex);
            return MAC_FAILURE;
        }
        /* ICIC changes end */
        
        requiredRBCount = resourceAllocInput_p->requiredRB;

        /*if subbband pointer is available then this indicates the allocation with Subband CQI*/
        if ( (resourceAllocInput_p->subBandReportInfo_p) &&  
             (!resourceAllocInput_p->aperiodicMode12) &&
             ( (harqProcess_p->dlHARQTBOneInfo.taDrxFlag == INVALID_TA_DRX_MSG) ||
               ( (dlStrategyTxNode_p->msgType == SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL) ||
                 (dlStrategyTxNode_p->msgType == SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL))))
        {
            /* Cyclomatic Complexity changes - starts  here */
            isWidebandAllocationRequired =  
                allocateRBRat2Tb1ValidTb2InvalidForSubBand( dlUEContext_p,
                        resourceAllocInput_p, raPolicyInfo_p, harqProcess_p,
                        &bestVRBSpanInfo, dlStrategyTxNode_p, 
                        &allocatedRBCount,
                        internalCellIndex);
            /* Cyclomatic Complexity changes - ends  here */
        }
        if (isWidebandAllocationRequired)
        {
            UInt8 maxRBs = 0;

            /* As of now, there is no use case for strictAllocSubBand
             * strictAllocSubBand check is commented */

            if (minTBSize < maxTBSize)
            {
                /* SPR 5999 changes start */
                maxRBs = DETERMINE_RBs_DL(wideBandMcsIndex, maxTBSize, internalCellIndex);
                /* SPR 5999 changes end */
            }
            else
            {
                maxRBs = requiredRBCount;
            }

            if( MAC_FAILURE == getBestContLocalizedVRBNewTx(requiredRBCount, 
                        &bestVRBSpanInfo, 
                        &raPolicyInfo_p->rat2LocalizedInfo,
                        resourceRegionAllocated,
                        maxRBs,
                        internalCellIndex))
            {
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, requiredRBCount, internalCellIndex);
                /* ICIC changes end */
                return MAC_FAILURE;    
            }

            availableRBCount = (bestVRBSpanInfo.endingVRBIndex - 
                    bestVRBSpanInfo.startingVRBIndex) + 1;

            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "availableRBCount = %u",availableRBCount);

            if (availableRBCount < requiredRBCount)
            {
                if (dlStrategyTxNode_p->strictAllocatedFlag)
                {
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, requiredRBCount, internalCellIndex);
                    /* ICIC changes end */
                    return MAC_FAILURE;
                }
            }
            allocatedRBCount = availableRBCount;

            //update excessPool 
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                    (requiredRBCount - allocatedRBCount),
                    internalCellIndex);
            /* ICIC changes end */

            harqProcess_p->dlHARQTBOneInfo.mcsIndex = wideBandMcsIndex;

            /* + TM6_5.2 */
            if ( (!ueContext_p->dlCQIInfo.latestAperiodicReportRcvd)&&
                    ( UE_ACK_NACK_TTI <= 
                      ((getCurrentTick() + macAdvanceFactor)- 
                       ueContext_p->dlCQIInfo.reportReceivedTTI)) &&
                       /* + TM7_8 Changes Start */
                       (!(TX_MODE_8 == ueContext_p->dlMIMOInfo.transmissionMode)))
                       /* - TM7_8 Changes End */
            {
                /*precoding according to latest PMI reports on PUSCH*/
                /*4x2 MIMO S*/  
                if(cellParams_p->numOfTxAnteenas  == NUM_OF_TX_ANTENNAS_2)
                {
                   harqProcess_p->pmiIndex = 5;
                }
                else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
                {
                    /*Rel 5.3.1 cyclomatic chg start*/ 
                    checkNumOfLayer(harqProcess_p);
                    /*Rel 5.3.1 cyclomatic chg end*/
                }
                /*4x2 MIMO E*/  
                ueContext_p->actualPMIValueForDLSCH = TRUE;             
            }
            else
            {
                harqProcess_p->pmiIndex = widebandPMI;
            }            
        }

         /*++klockwork warning */
        if( (resourceAllocInput_p->subBandReportInfo_p) &&
         /*--klockwork warning */
            (TRUE == ueContext_p->actualPMIValueForDLSCH) &&        
                (TRUE == resourceAllocInput_p->aperiodicMode12) )
        {
            vrbIndex = bestVRBSpanInfo.startingVRBIndex;
            UInt32 rbgSize = raPolicyInfo_p->rat0Info.rbgSize;
            UInt32 tempAllocatedRBCount = allocatedRBCount;
            while(tempAllocatedRBCount--)
            {
                rbgIndex = (vrbIndex - 1)/rbgSize;
                SubBandToCQInfo *subBandToCQInfo_p = 
                    &resourceAllocInput_p->subBandReportInfo_p->subBandToCQInfo[0];  
                /* SPR 18122 Changes Start*/
                newSubBandNum = DIVIDE_BY_TWO(rbgIndex);  
                /* SPR 18122 Changes End*/
                SubBandToCQInfo *tempSubBandToCQInfo_p = &subBandToCQInfo_p[newSubBandNum];
                if(newSubBandNum != prevSubBandNum)
                {    
                    resourceAllocOutput_p->subbandtoPMIInfo[tempSubbandIndex] 
                        = tempSubBandToCQInfo_p->preCodingMatrixIndex;
                    prevSubBandNum = newSubBandNum;
                    tempSubbandIndex++;
                }
                vrbIndex++;
            }
        }
        if (NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator)
        {      /*++klockwork warning */
            if( (resourceAllocInput_p->subBandReportInfo_p) &&
             /*--klockwork warning */
            (TRUE == resourceAllocInput_p->aperiodicMode12)&&
               /* + TM7_8 Changes Start */
               (!(TX_MODE_8 == ueContext_p->dlMIMOInfo.transmissionMode)))
               /* - TM7_8 Changes End */
            {
                /*Rel 5.3.1 cyclomatic chg start*/
                setWideSubbandPmiforTxAnteenas(harqProcess_p,&widebandPMI,
                                              ueContext_p,resourceAllocInput_p,
                                              internalCellIndex);
                /*Rel 5.3.1 cyclomatic chg end*/                               
            }
        }
        ueContext_p->actualPMIValueForDLSCH = FALSE;       
        resourceAllocInput_p->aperiodicMode12 =  FALSE; 
        resourceAllocOutput_p->pmiValue =  harqProcess_p->pmiIndex;
        /* - TM6_5.2 */
    /*Dynamic PDCCH + 5117 changes starts */

    harqProcess_p->assignedResourcesNB = allocatedRBCount;
    /* Need to validate MCS/RB pair for non 0/5
     * Subframes for mCS 28.
     * In case of non0/5 SF and MCS28, we need to 
     * check for coderate and ueCat both while in
     * non 0/5 SF, for other MCS values, 
     * we just need to check uecategory*/
    if (MAC_FAILURE == checkCodeRateUECategory(
                RA_POLICY_2_LOCALIZED, 
                scheduledTick,
                harqProcess_p,
                ueContext_p,
                TRUE,
                0,
                /* +- SPR 17777 */
                internalCellIndex))
    {
        /* SPR 13903 fix start */
        LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                MAC_PDSCH,getCurrentTick(),(UInt32)ueContext_p->ueIndex,
                RA_POLICY_2_LOCALIZED,harqProcess_p->assignedResourcesNB,
                harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        /* SPR 13903 fix end */

        return MAC_FAILURE;
    }
    /* +DYNAMIC_ICIC */
    /* SPR 15959 fix start */
    if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
    {
        calculatedRNTP = calculateRNTP(dlUEContext_p->pA, harqProcess_p, internalCellIndex);
    }
    /* SPR 15959 fix end */
    /* -DYNAMIC_ICIC */
    allocatedRBCount = harqProcess_p->assignedResourcesNB;

    /*Dynamic PDCCH + 5117 changes ends */
     /* Cyclomatic_complexity_changes_start */ 
#ifdef FDD_CONFIG
        setTbSizeBasedOnSplSubframe(harqProcess_p,
                allocatedRBCount);
#elif TDD_CONFIG
        setTbSizeBasedOnSplSubframe(harqProcess_p,
                allocatedRBCount,
                isSpecialSubframe);
#endif
        /* Cyclomatic_complexity_changes_end */

        raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable -= allocatedRBCount;
        harqProcess_p->assignedResourcesNB = allocatedRBCount;
        resourceAllocOutput_p->allocatedRB = allocatedRBCount;
        resourceAllocOutput_p->ratPolicy = RA_POLICY_2_LOCALIZED;
        resourceAllocOutput_p->startingVRBIndex = bestVRBSpanInfo.startingVRBIndex - 1;
        /* + TM6_5.2 */
        resourceAllocOutput_p->numberOfSubbands = tempSubbandIndex;
        /* - TM6_5.2 */
        populateResourceAssignmentInfo(&resourceAllocOutput_p-> 
                resourceAssignmentBitMap, allocatedRBCount, bestVRBSpanInfo.
                startingVRBIndex,0,harqProcess_p->dciFormat,internalCellIndex);         
       
        /* +DYNAMIC_ICIC */
        updateRATInfoFromRAT2LocalizedInfo(&bestVRBSpanInfo, raPolicyInfo_p,
                allocatedRBCount,updateRAT2DistributedFlag, calculatedRNTP,
                internalCellIndex);    
        /* -DYNAMIC_ICIC */
        resourceAllocInput_p->aperiodicMode12 = FALSE;
    }
    else
    {
        /*getBestContLocalizedVRBReTx  funtion will try to find  the maximum
          continousfree VRBs ( equal to resourceAllocInput_p->requiredRB) 
          in the RAT2LocalizedVRBInfo array which holds the information of  
          Free VRB .If it's not able to find then it will return starting 
          and ending VRB Index as Zero*/
        tbSize = harqProcess_p->dlHARQTBOneInfo.tbSize;
        /* +SPR 5228, 5146, 5300 Changes Start*/
        requiredRBCount = harqProcess_p->assignedResourcesNB;
        /* -SPR 5228, 5146, 5300 Changes End*/
        /* + TM6_5.2 */
        harqProcess_p->pmiIndex = widebandPMI;
        /* - TM6_5.2 */

        if (raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable < 
                requiredRBCount)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "Leaving Funtion with Failure as  numOfVRBAvailable <  \
                    = requiredRBCount\n");
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, requiredRBCount, internalCellIndex);
            /* ICIC changes end */
            return MAC_FAILURE;   
        }
        /* ICIC changes start */
        if ((CE_REGION == resourceAllocInput_p->resourceRegionAllocated) &&
                (raPolicyInfo_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable < 
                 requiredRBCount))
        {
            /* Here retransmission is from cell edge region & suffiecient cell 
             * edge resources are not available. Hence try to allocate from 
             * cell centre region. 
             */
            resourceRegionAllocated = CC_CE_REGION;
        }

        /* SPR#706 Changes Start */
        if( MAC_FAILURE == getBestContLocalizedVRBReTx(requiredRBCount, &bestVRBSpanInfo, 
                    &raPolicyInfo_p->rat2LocalizedInfo, resourceRegionAllocated))
        {
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, requiredRBCount, internalCellIndex);
            /* ICIC changes end */
            return MAC_FAILURE;    
        }
        /* SPR#706 Changes End */


        availableRBCount = bestVRBSpanInfo.endingVRBIndex - bestVRBSpanInfo.
            startingVRBIndex + 1;

        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "availableRBCount = %u",availableRBCount);

        if (availableRBCount < requiredRBCount)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "Leaving Funtion with Failure as  availableRBCount <  \
                    = requiredRBCount\n");
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, requiredRBCount, internalCellIndex);
            /* ICIC changes end */
            return MAC_FAILURE;   
        }           
        /* SPR 13903 fix start */ 
        /* Need to validate MCS/RB pair for non 0/5
        * Subframes for mCS 28.
        * In case of non0/5 SF and MCS28, we need to 
        * check for coderate and ueCat both while in
        * non 0/5 SF, for other MCS values, 
        * we just need to check uecategory*/
        if (MAC_FAILURE == checkCodeRateUECategory(
                RA_POLICY_2_LOCALIZED, 
                scheduledTick,
                harqProcess_p,
                ueContext_p,
                TRUE,
                0,
                /* +- SPR 17777 */
                internalCellIndex))
        {
            /* SPR 13903 fix start */
            LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                        MAC_PDSCH,getCurrentTick(),(UInt32)ueContext_p->ueIndex,
                        RA_POLICY_2_LOCALIZED,harqProcess_p->assignedResourcesNB,
                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
            /* SPR 13903 fix end */

            return MAC_FAILURE;
        }
        
        /* +DYNAMIC_ICIC */
        /* SPR 15959 fix start */
        if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
        {
            calculatedRNTP = calculateRNTP(dlUEContext_p->pA, harqProcess_p, internalCellIndex);
        }
        /* SPR 15959 fix end */
        /* -DYNAMIC_ICIC */

        requiredRBCount = harqProcess_p->assignedResourcesNB;
        /* SPR 13903 fix end */ 
 
        raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable -= requiredRBCount;
        harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize = tbSize;
        resourceAllocOutput_p->allocatedRB = requiredRBCount;
        resourceAllocOutput_p->startingVRBIndex = bestVRBSpanInfo.
            startingVRBIndex - 1;
        resourceAllocOutput_p->ratPolicy = RA_POLICY_2_LOCALIZED;

        populateResourceAssignmentInfo(&resourceAllocOutput_p-> 
                resourceAssignmentBitMap, requiredRBCount, bestVRBSpanInfo.
                startingVRBIndex,0,harqProcess_p->dciFormat,internalCellIndex);           
        /* +DYNAMIC_ICIC */
        updateRATInfoFromRAT2LocalizedInfo(&bestVRBSpanInfo, raPolicyInfo_p,
                requiredRBCount,updateRAT2DistributedFlag, calculatedRNTP, 
                internalCellIndex);    
        /* -DYNAMIC_ICIC */
        /* + TM6_5.2 */
        resourceAllocOutput_p->pmiValue =  harqProcess_p->pmiIndex;
        resourceAllocOutput_p->numberOfSubbands = tempSubbandIndex;
        /* - TM6_5.2 */
    }
    /* SPR 13903 fix start */
    /* +DYNAMIC_ICIC */
    /*coverity 73354 fix start*/
    /*if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
    {
        calculatedRNTP = calculateRNTP(dlUEContext_p->pA, harqProcess_p, internalCellIndex);
    }*/
    /*coverity 73354 fix end*/
    /* Cyclomatic_complexity_changes_start */
    getNumofOverlappingRb(scheduledTick,
                               allocatedRBCount,
                               bestVRBSpanInfo,
                               numOfOverLappingRb,
                               internalCellIndex
                               );
    /* Cyclomatic_complexity_changes_end */
     
    /* -DYNAMIC_ICIC */
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "Leaving funtion = %s with Success\n",__func__);
    return MAC_SUCCESS;
    /* SPR 13903 fix end */

}
/* - CQI_5.0 */

/******************************************************************************
 * Function Name    : initRAT0Info
 * Inputs           : rat0Info_p - This holds pointer to  RAT0Info structure,
 *                    dlRBs - This holds the DownLink Bandwitdh in terms of RBs
 * Outputs          : None
 * Returns          : None
 * Description      : This will intialize the RAT 0 BitMap.
 ******************************************************************************/
STATIC void initRAT0Info(RAT0Info *rat0Info_p,
        UInt32 dlRBs)                               
{
    UInt32 rbgSize = 0;
    UInt32 rbgCount = 0;        
    UInt32 rbInLastRBG = 0;
    UInt32  rbStartingPosition = 0;
    UInt32 rbgSizeBitMap = 0;
    UInt32 rbgSizeBitMapForLastRBG = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
        "Entering funtion = %s",__func__);
    rbgSize =  rat0Info_p->rbgSize;
    rbgCount = ceil_wrapper( (UDouble32)dlRBs / rbgSize);
    rbInLastRBG = rbgSize - (rbgCount * rbgSize - dlRBs );

    rbgSizeBitMap = pow_wrapper(2 ,rbgSize) - 1;
    /* ICIC changes start */
    rat0Info_p->rat0BitMap[CC_USER] = 0xFFFFFFFF >> (32 - rbgCount);  

    /* Initialize cell edge bitmap marking all RBGs blocked*/
    rat0Info_p->rat0BitMap[CE_USER] = 0;
    /*Dynamic PDCCH + 5117 changes starts */    
    rat0Info_p->mibResrcAssignementBitMap = 0;
    rat0Info_p->temp = 0;
    /*Dynamic PDCCH + 5117 changes ends */

    LOG_UT(MAC_RAT0_BITMAP,LOGDEBUG,
            MAC_PDSCH,getCurrentTick(),(UInt32)rat0Info_p->rat0BitMap[CC_USER],
            (UInt32)rat0Info_p->rat0BitMap[CE_USER],DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"\nRAT 0 Bitmap for cell centre and cell edge users");
    
    /* ICIC changes end */

    rat0Info_p->rbgCount = rbgCount;
    rat0Info_p->rbgIndexInfo[rbgCount - 1].numOfRBs = rbInLastRBG;
    rat0Info_p->rbgIndexInfo[rbgCount- 1].subsetIndex =  (rbgCount - 1) % rbgSize;
    rbStartingPosition = (rbgCount - 1) / rbgSize;
    rbgSizeBitMapForLastRBG  = pow_wrapper(2, rbInLastRBG) - 1;
    rat0Info_p->rbgIndexInfo[rbgCount- 1].rbgSizeBitMapForSubset = 
        ~(rbgSizeBitMapForLastRBG << ( rbStartingPosition * rbgSize));
    while (--rbgCount)
    {
        rbStartingPosition = (rbgCount - 1) / rbgSize;
        rat0Info_p->rbgIndexInfo[rbgCount- 1].numOfRBs = rbgSize;
        rat0Info_p->rbgIndexInfo[rbgCount- 1].subsetIndex =  (rbgCount - 1) % rbgSize;
        rbStartingPosition =   (rbgCount - 1) / rbgSize;
        rat0Info_p->rbgIndexInfo[rbgCount- 1].rbgSizeBitMapForSubset = ~(rbgSizeBitMap << 
                ( rbStartingPosition * rbgSize));
    }   
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR, 
        "Leaving funtion = %s",__func__);

}    
/******************************************************************************
 * Function Name    : calculateMaxPRB
 * Inputs           : subsetIndex - This holds the subsetIndex on which 
 *                                  maximum numberof RBs need to be calculated,
 *                    dlRBs - This holds the downlink Bandwidth in terms of RBs,
 *                    subsetSize - This holds the subsetSize
 * Outputs          : Max number of  PRB in a subset
 * Returns          : maxAddressablePRBCount
 * Description      : This funtion will calculate maximum number of  PRB in a 
 *                    subset.
 ******************************************************************************/
STATIC UInt32 calculateMaxPRB(UInt32 subsetIndex,
        UInt32 dlRBs,
        UInt32 subsetSize)
{
    UInt32  x1 = 0;
    UInt32 maxAddressablePRBCount= 0;
    UInt32 temp  = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);
    temp = ((dlRBs - 1) / (subsetSize * subsetSize) ) * subsetSize;
    x1 = ((dlRBs - 1) / subsetSize ) % subsetSize;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "SubsetIndex   = %u",subsetIndex);

    /* Below logic is mentioned in specs for calculating maximum number of  
       PRB 
       in a subset */
    if ( subsetIndex < x1)
    {
        maxAddressablePRBCount= temp + subsetSize;    
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "maxAddressablePRBCount   = %u",maxAddressablePRBCount);
        return maxAddressablePRBCount;
    }
    else if(subsetIndex == x1)
    {   
        maxAddressablePRBCount= temp  + (dlRBs - 1) % subsetSize + 1;
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "maxAddressablePRBCount   = %u",maxAddressablePRBCount);
        return maxAddressablePRBCount;

    }
    /* Coverity 72756 Fix Start */
    else
    /* Coverity 72756 Fix End */
    {
        maxAddressablePRBCount= temp ;
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "maxAddressablePRBCount   = %u",maxAddressablePRBCount);
        return maxAddressablePRBCount;
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s",__func__);
    /* Coverity 72756 Fix Start */
    /*return 0;*/
    /* Coverity 72756 Fix End */
}


/******************************************************************************
 * Function Name    : calculateMaxAddressablePRB
 * Inputs           : subsetSize - This holds the Subset Size,
 *                    dlRBs - This holds the downlink bandwitdh in terms of RBs
 * Outputs          : Max Number of PRB that can be given from a subset.
 * Returns          : maxAddressablePRB
 * Description      : This will calculate the Max Number of PRB that can be given 
 *                    from a subset.
 ******************************************************************************/
STATIC UInt32 calculateMaxAddressablePRB(UInt32 subsetSize,
        UInt32 dlRBs)
{
    UInt32 maxAddressablePRB =0;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);
    /* Below logic is mentioned in specs for calculating  maximum number of 
       PRB  which can be allocated from  a subset */
    maxAddressablePRB = ceil_wrapper(((UDouble32)dlRBs / subsetSize)) - 
        ceil_wrapper( log_wrapper(subsetSize) / log_wrapper(2)) - 1;  
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "maxAddressablePRB = %u",maxAddressablePRB);
    return maxAddressablePRB;
}

/******************************************************************************
 * Function Name    : initRAT1Info
 * Inputs           : rat1Info_p - This holds pointer to RAT1Info structure,
 *                    dlRBs - This holds the DownLink Bandwitdh in terms of RBs
 * Outputs          : None
 * Returns          : None
 * Description      : This will intialize the RAT 1 BitMap.
 ******************************************************************************/
STATIC void initRAT1Info(RAT1Info *rat1Info_p,
        UInt32 dlRBs)
{
    SubsetInfo *subsetInfo_p = PNULL;
    UInt32 subsetCount = 0;
    UInt32 maxPRBInSubset = 0;
    UInt32 maxAddressablePRBInSubset = 0;
    UInt32 subsetSize = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "cellConfig_g.dlAvailableRBs = %u",dlRBs);
    /* Below For loop is initializing the relevant information for each subset,
     * which we be used for allocating the RBs.It calculate the maximum number 
     * of PRB that can be availabel in a subset,maximum number of PRB that can
     * be allocated ,the span width ,bit map for span width =0 , bit map for 
     * span width  = span width */
    subsetCount = rat1Info_p->subsetCount;
    subsetSize  = subsetCount;
    subsetInfo_p = &rat1Info_p->subsetInfo[subsetCount - 1];
    maxAddressablePRBInSubset = calculateMaxAddressablePRB(subsetSize, 
            dlRBs);
    rat1Info_p->maxAddressableRBCount = maxAddressablePRBInSubset;
    while ( subsetCount-- )
    {
        maxPRBInSubset = calculateMaxPRB(subsetCount, dlRBs ,subsetSize);
        subsetInfo_p->numPRBFree = maxPRBInSubset;
        subsetInfo_p->maxAddressableRBCount = maxAddressablePRBInSubset;
        /* ICIC changes start */
	/* + Coverity 72756 */
	if ( maxPRBInSubset)
	{
		subsetInfo_p->subsetBitMap[CC_USER] = 0xFFFFFFFF >> (32 - maxPRBInSubset);
	}
	else
	{
		subsetInfo_p->subsetBitMap[CC_USER] = 0;
	} 
	/* - Coverity 72756 */
        
        /* Initialize subset bitmap for cell edge users such that all PRBs are
         * blocked
         */
        subsetInfo_p->subsetBitMap[CE_USER] = 0;
        subsetInfo_p->spanWidth = maxPRBInSubset - maxAddressablePRBInSubset;

        LOG_UT(MAC_RAT1_SUBSET_INFO,LOGDEBUG,
                MAC_PDSCH,getCurrentTick(),
                (UInt32)subsetCount,
                (UInt32)subsetInfo_p->subsetBitMap[CC_USER],
                (UInt32)subsetInfo_p->subsetBitMap[CE_USER],
                (UInt32)subsetInfo_p->spanWidth,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"\nRAT 1 subset Information");

        subsetInfo_p->bitMapWithSpanZero = subsetInfo_p->subsetBitMap[CC_USER] >> 
            subsetInfo_p->spanWidth;
        /* ICIC changes end */
        subsetInfo_p->bitMapWithSpanNonZero = subsetInfo_p->bitMapWithSpanZero 
            <<  subsetInfo_p->spanWidth;
        initbitMappedToPrbNumberArray(subsetSize,
                subsetCount, subsetInfo_p , maxPRBInSubset);

        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "subsetCount = %u maxPRBInSubset = %u numPRBFree = %u" \
                "subsetBitMap = %u spanWidth = %u bitMapWithSpanZero = %u" \
                "bitMapWithSpanNonZero = %u",subsetCount,maxPRBInSubset,
                subsetInfo_p->numPRBFree,subsetInfo_p->subsetBitMap,
                subsetInfo_p->spanWidth,subsetInfo_p->bitMapWithSpanZero,
                subsetInfo_p->bitMapWithSpanNonZero);
        subsetInfo_p--;
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s",__func__);
}

/******************************************************************************
 * Function Name    : initbitMappedToPrbNumberArray
 * Inputs           : subsetSize - This holds subset Size
 *                    subsetIndex - This holds the subset Index
 *                    maxAddressablePRB - This holds Max number of Addressable PRB 
 *                                        in subset
 * Outputs          : subsetInfo_p - This holds pointer to SubsetInfo structure
 *                                   which needs to be updated
 * Returns          : None
 * Description      : This will intialize the RAT 1 BitMap.
 ******************************************************************************/
STATIC  void initbitMappedToPrbNumberArray(
        UInt32 subsetSize,
        UInt32 subsetIndex,
        SubsetInfo *subsetInfo_p,
        UInt32 maxAddressablePRB)
{
    UInt32 prbNumber = 0;
    UInt32 subsetSizeCount = 0;
    UInt32 count = 0;
    UInt32 temp = 0;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,"subsetIndex = %d \n maxAddressablePRB = %d\n",
        subsetIndex, maxAddressablePRB);
    
    temp = subsetSize * (subsetSize - 1);
    prbNumber = subsetSize * subsetIndex;  
    for(count = 0;count < maxAddressablePRB ;)
    {
        subsetSizeCount= subsetSize;
        while(subsetSizeCount--)
        {  
            subsetInfo_p->bitPositionToSubsetAndRBGInfo[count].prbNumber = prbNumber;
            subsetInfo_p->bitPositionToSubsetAndRBGInfo[count].rbgIndex = 
                prbNumber / subsetSize;
            subsetInfo_p->bitPositionToSubsetAndRBGInfo[count].rbgIndexBitMap = 
                ~((UInt32)1 << (prbNumber / subsetSize));       
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR," prbNumber = %d \n \
               rbgIndex = %d \n rbgIndexBitMap = %x \n ",
               subsetInfo_p->bitPositionToSubsetAndRBGInfo[count].prbNumber,
               subsetInfo_p->bitPositionToSubsetAndRBGInfo[count].rbgIndex,
              subsetInfo_p->bitPositionToSubsetAndRBGInfo[count].rbgIndexBitMap);
            prbNumber++;
            count++;
        }     
        prbNumber += temp ;
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s",__func__);
}

/******************************************************************************
 * Function Name    : assignPrb
 * Inputs           : distributedVRBInfo_p - This holds pointer to 
 *                                           RAT2DistributedVRBInfo structure,
 *                    vrbNum - This holds the VRB number,
 *                    nTilledDLVrb - This holds a value which is used for calculation
 *                                   of PRBs mapped to Even and Odd slot of a VRB,
 *                    nRow - This holds a value which is used for calculation of PRBs
 *                           mapped to Even and Odd slot of a VRB,
 *                    nNull -  This holds a value which is used for calculation of PRBs
 *                             mapped to Even and Odd slot of a VRB,
 *                    nGap -  This holds a value which is used for calculation of PRBs
 *                            mapped to Even and Odd slot of a VRB
 * Outputs          : None
 * Returns          : None
 * Description      : This will calculate the PRB which is mapped to Even and Odd
 *                     of a given VRB.
 ******************************************************************************/
STATIC void assignPrb(RAT2DistributedVRBInfo *distributedVRBInfo_p,
        UInt32 vrbNum,
        UInt32 nTilledDlVrb,
        UInt32  nRow,
        UInt32  nNull,
        UInt32  nGap)
{
    UInt32 nTilledPrb=0;
    UInt32  nPrb=0;
    UInt32  nVrb=0;
    UInt32  nDashPrb=0;
    UInt32  nDash2Prb=0;
    UInt32  nTilledVrb=0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);
    /* Below Logic is exmplained in Specs for calculaiting the PRB whose even 
       slot is mapped to given VRB and also the PRB whose odd slot is mapped 
       to given VRB */

    nVrb = vrbNum;
    nTilledVrb = nVrb % nTilledDlVrb;
    /*Below logic is used for calculation of PRB which is mapped to Even Slot 
     * of VRB*/
     /* SPR 18122 Changes Start*/
    nDashPrb = ( 2 * nRow * (MODULO(nTilledVrb,2))) + 
        floor_wrapper((UDouble32)DIVIDE_BY_TWO(nTilledVrb)) +
        nTilledDlVrb * floor_wrapper((UDouble32)nVrb / nTilledDlVrb);

    nDash2Prb = nRow * (MODULO(nTilledVrb,4)) + floor_wrapper((UDouble32)DIVIDE_BY_FOUR(nTilledVrb)) +
        nTilledDlVrb * floor_wrapper((UDouble32)nVrb / nTilledDlVrb);

    if ((0 != nNull) && (nTilledVrb >= (nTilledDlVrb - nNull)) && 
            (MODULO(nTilledVrb,2) == 1))
    {
        nTilledPrb = nDashPrb - nRow;
    }
    else if ((0 != nNull) && (nTilledVrb >= (nTilledDlVrb - nNull))
            &&(MODULO(nTilledVrb,2) == 0))
    {
        nTilledPrb = nDashPrb - nRow + (DIVIDE_BY_TWO(nNull));
    }
    else if ((0 != nNull) && (nTilledVrb < (nTilledDlVrb - nNull)) && 
            (MODULO(nTilledVrb,4) >= 2))
    {
        nTilledPrb = nDash2Prb - (DIVIDE_BY_TWO(nNull));
    }
    /* SPR 18122 Changes End*/
    else
    {
        nTilledPrb = nDash2Prb;
    }

    /* SPR 18122 Changes Start*/
    if (nTilledPrb < (DIVIDE_BY_TWO(nTilledDlVrb) ))
        /* SPR 18122 Changes End*/
    {
        nPrb = nTilledPrb;
    }
    /* SPR 18122 Changes Start*/
    else if(nTilledPrb >= (DIVIDE_BY_TWO(nTilledDlVrb)))
    {
        nPrb = nTilledPrb + nGap - (DIVIDE_BY_TWO(nTilledDlVrb));
    }    
    /* SPR 18122 Changes End*/


    distributedVRBInfo_p->prbMappedToEvenSlotOfVRB= nPrb;

    /*Below logic is used for calculation of PRB which is mapped to Odd Slot 
     * of VRB*/
     /* SPR 18122 Changes Start*/
    nTilledPrb = ((nTilledPrb + (DIVIDE_BY_TWO(nTilledDlVrb))) % nTilledDlVrb) + 
        (nTilledDlVrb * (floor_wrapper((UDouble32)nVrb / nTilledDlVrb)));

    if (nTilledPrb < (DIVIDE_BY_TWO(nTilledDlVrb)))
    {
        nPrb = nTilledPrb;
    }
    else if(nTilledPrb >= (DIVIDE_BY_TWO(nTilledDlVrb)))
    {
        nPrb = nTilledPrb + nGap - (DIVIDE_BY_TWO(nTilledDlVrb));
    }     
    /* SPR 18122 Changes End*/
    else 
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "InValid Case\n");
    }

    distributedVRBInfo_p->prbMappedToOddSlotOfVRB= nPrb;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "leaving funtion = %s",__func__);
}


/******************************************************************************
 * Function Name    : initRAT2DistributedInfo
 * Inputs           : raPolicyInfo_p - This holds pointer to 
 *                                     ResourceAllocationPolicyInfo structure,
 *                    dlRBs - This holds the downlink bandwidth in terms of RBs   
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This will intialize the RAT 2 Distributed Information.
 ******************************************************************************/
STATIC void initRAT2DistributedInfo(
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        UInt32 dlRBs
        ,InternalCellIndex internalCellIndex)
{
    /*Rel_523_Coverity_10056 Fix start*/
    UInt8 nGap =16; /*Setting default value to 16 */
    /*Rel_523_Coverity_10056 Fix End*/

    UInt8 x =0;
    UInt8 nRow =0;
    UInt8 nTilledDlVrb =0;
    UInt8 nNull =0;
    UInt8 min =0;   
    UInt32 gapValue = 0;
    UInt32 maxDistributedVRB = 0;
    UInt32 rbgSize = 0;
    UInt32 prbMappedToEvenSlotOfVRB = 0;
    UInt32 prbMappedToOddSlotOfVRB = 0;
    UInt32 rbgIndex = 0;
    UInt32 prbPositionWithinSubset = 0;
    RAT2DistributedVRBInfo *distributedVRBInfo_p = PNULL;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    gapValue = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                    ->cellParams_p->gapValue;
    /* coverity-530 CID 32865 */
    if( (1 != gapValue) && (2 != gapValue))
    {
        gapValue = DEFAULT_GAP_VALUE;
    }
    /* coverity-530 CID 32865 */
    rbgSize = raPolicyInfo_p->rat0Info.rbgSize;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "gapValue = %u\n dlRBs = %u\n rbgSize = %u\n ",
            gapValue, dlRBs , rbgSize);

    /* Cyclomatic Complexity changes start */
     if(MAC_SUCCESS != checkDLBandwidthRBsRange(dlRBs, &nGap, gapValue))
     {
         return;
     }
    /* Cyclomatic Complexity changes end */

    if ( 1 == gapValue  )
    {
        x = dlRBs - nGap;
        min = (nGap < x)?(nGap):(x);
        maxDistributedVRB = (2) * (min);
        nRow = (ceil_wrapper((UDouble32)maxDistributedVRB / (4 * rbgSize))) * (rbgSize)
            ;     
        nTilledDlVrb = maxDistributedVRB; 
        nNull = ((4 * nRow) - (nTilledDlVrb));
        raPolicyInfo_p->rat2DistributedInfo.maxDistributedVRB = 
            maxDistributedVRB;
        raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable = maxDistributedVRB;
    }
    else if ( 2 == gapValue )
    {
        maxDistributedVRB = floor_wrapper((UDouble32)dlRBs / (2 * nGap)) * (2 * nGap);
        nRow = (ceil_wrapper((UDouble32)(2 * nGap) / (4 * rbgSize))) * (rbgSize);
        nTilledDlVrb = 2 * nGap;
        nNull = ((4 * nRow)-(nTilledDlVrb));
        raPolicyInfo_p->rat2DistributedInfo.maxDistributedVRB = 
            maxDistributedVRB;
        raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable = maxDistributedVRB;
    }
    /* ICIC changes start */
    /* Initialize cell edge VRBs to 0 */
    raPolicyInfo_p->rat2DistributedInfo.numOfCellEdgeVRBAvailable = 0;
    /* ICIC changes end */
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "maxDistributedVRB = %u\n",maxDistributedVRB);
    distributedVRBInfo_p = &raPolicyInfo_p->rat2DistributedInfo.
        rat2DistributedVRBInfo[maxDistributedVRB + 2];

    distributedVRBInfo_p->nextfreeVRBIndex = maxDistributedVRB + 3;
    distributedVRBInfo_p->previousVRBfreeIndex = maxDistributedVRB;    
    distributedVRBInfo_p--;
    distributedVRBInfo_p--;
    distributedVRBInfo_p->nextfreeVRBIndex = maxDistributedVRB + 2;
    distributedVRBInfo_p->previousVRBfreeIndex = maxDistributedVRB - 1;
    /* SPR 1974 changes start */
    assignPrb(distributedVRBInfo_p, maxDistributedVRB - 1, nTilledDlVrb, nRow, nNull, nGap);
    /* SPR 1974 changes end */
    distributedVRBInfo_p--;
    maxDistributedVRB--;   
    /* Below Logic is poluating ratLocalizedInfo_p structure such that 
     * Intializing all VRB as free  and also a Free VRB know the next 
     *  Free VRB Index and also knows previous Free VRB Index */

    while ( maxDistributedVRB )
    {
        distributedVRBInfo_p->nextfreeVRBIndex = maxDistributedVRB + 1;
        distributedVRBInfo_p->previousVRBfreeIndex = maxDistributedVRB - 1;   
        assignPrb(distributedVRBInfo_p, maxDistributedVRB - 1,
                nTilledDlVrb, nRow, nNull, nGap);

        prbMappedToEvenSlotOfVRB = 
            distributedVRBInfo_p->prbMappedToEvenSlotOfVRB;

        prbMappedToOddSlotOfVRB = 
            distributedVRBInfo_p->prbMappedToOddSlotOfVRB;

        rbgIndex = (prbMappedToEvenSlotOfVRB) / rbgSize ;
        distributedVRBInfo_p->rbgIndexBitMapForEvenVRBSlot = 
            ~((UInt32)1 << rbgIndex);
        distributedVRBInfo_p->subsetIndexForEvenVRBSlot = rbgIndex % rbgSize;
        prbPositionWithinSubset =  ((prbMappedToEvenSlotOfVRB / rbgSize) / 
                (rbgSize ) ) * rbgSize +  (prbMappedToEvenSlotOfVRB % rbgSize);

        distributedVRBInfo_p->subsetBitMapForEvenVRBSlot = ~(((UInt32)1) << (
                    prbPositionWithinSubset ));


        rbgIndex = (prbMappedToOddSlotOfVRB) / rbgSize ;
        distributedVRBInfo_p->rbgIndexBitMapForOddVRBSlot = 
            ~((UInt32)1 << rbgIndex);
        distributedVRBInfo_p->subsetIndexForOddVRBSlot = rbgIndex % rbgSize;
        prbPositionWithinSubset =  ((prbMappedToOddSlotOfVRB / rbgSize) / 
                (rbgSize ) ) * rbgSize +  (prbMappedToOddSlotOfVRB % rbgSize);

        distributedVRBInfo_p->subsetBitMapForOddVRBSlot = ~(((UInt32)1) << (
                    prbPositionWithinSubset ));

        if ( prbMappedToOddSlotOfVRB == prbMappedToEvenSlotOfVRB )
        {
            distributedVRBInfo_p->numOfRBNeedToSubtracted = 1;
        }
        else
        {
            distributedVRBInfo_p->numOfRBNeedToSubtracted = 2; 
        }


        distributedVRBInfo_p--;
        maxDistributedVRB--;
    }
    distributedVRBInfo_p->nextfreeVRBIndex = 1;
    distributedVRBInfo_p->previousVRBfreeIndex = 0;  
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);

}

/******************************************************************************
 * Function Name    : checkDLBandwidthRBsRange
 *                    dlRBs - This holds the downlink bandwidth in terms of RBs
 *                    nGap_p 
 *                    gapValue
 * Outputs          : modifies value of nGap_p
 * Returns          : None
 * Description      : This will check the range of DL Bandwidth RBs 
 *                    and sets nGap
 ******************************************************************************/
STATIC  MacRetType checkDLBandwidthRBsRange(
        UInt32 dlRBs,
        UInt8 *nGap_p,
        UInt32 gapValue)
{
    if (dlRBs < 6)
    {
        ltePanic("\n Bandwidth is less than 6 RBs\n");
        return MAC_FAILURE;
    }
    else if ( !(dlRBs - 6) )
    {   
        *nGap_p = 3;   
    }
    else if ( (7 == dlRBs) || (8 == dlRBs) )
    {
        *nGap_p = 4;       
    }
    else if ( (10 == dlRBs) || (9 == dlRBs) )
    {
        *nGap_p = 5;       
    }
    else if ( 11 == dlRBs)
    {
        *nGap_p = 4;
    }
    else if ((dlRBs >= 12) && (dlRBs <= 19))
    {
        *nGap_p = 8;      
    }
    else if ((dlRBs >= 20) && (dlRBs <= 26))
    {
        *nGap_p = 12;       
    }
    else if ((dlRBs >= 27) && (dlRBs <= 44))
    {
        *nGap_p = 18;     
    }
    else if ((dlRBs >= 45) && (dlRBs <= 63))
    {     
        if( 1 == gapValue )
        {
            *nGap_p = 27;
        }
        else
        {
            *nGap_p = 9;      
        }

    }
    else if ((dlRBs >= 64) && (dlRBs <= 79))
    {     
        if( 1 == gapValue )
        {
            *nGap_p = 32;
        }
        else
        {
            *nGap_p = 16;
        }

    }
    /* +COVERITY 10714 */
    else if ((dlRBs >= 80) && (dlRBs <= 100))
    /* -COVERITY 10714 */
    {        
        if(1==gapValue)
        {
            *nGap_p = 48;
        }
        else
        {
            *nGap_p = 16;       
        }

    }  
  /*Rel_523_Coverity_10056 Fix start*/
    else
    {
        ltePanic("\n Invalid Bandwidth %d RBs\n",dlRBs);
        return MAC_FAILURE;
    }
  /*Rel_523_Coverity_10056 Fix End*/
    return MAC_SUCCESS;
}

/******************************************************************************
 * Function Name    : initRAT2LocalizedInfo
 * Inputs           : raPolicyInfo_p - This holds pointer to 
 *                                     ResourceAllocationPolicyInfo structure,
 *                    dlRBs - This holds the downlink bandwidth in terms of RBs   
 * Outputs          : None
 * Returns          : None
 * Description      : This will intialize the RAT 2 Localized Information.
 ******************************************************************************/
STATIC void initRAT2LocalizedInfo(
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        UInt32 dlRBs)
{
    UInt32 maxVRBLocalized = 0;
    UInt32 rbgSize = 0;
    UInt32 rbgIndex = 0;
    UInt32 prbPositionWithinSubset= 0;
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);

    rbgSize = raPolicyInfo_p->rat0Info.rbgSize;
    raPolicyInfo_p->rat2LocalizedInfo.maxLocalizedVRB = dlRBs;
    raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable = dlRBs;

    /* ICIC changes start */
    /* Initialize cell edge VRBs to 0 */
    raPolicyInfo_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable = 0;
    /* ICIC changes end */

    maxVRBLocalized  = dlRBs;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "maxVRBLocalized = %u\n",maxVRBLocalized);
    localizedVRBInfo_p = &raPolicyInfo_p->rat2LocalizedInfo.
        rat2LocalizedVRBInfo[maxVRBLocalized + 2];
    
    localizedVRBInfo_p->nextfreeVRBIndex = maxVRBLocalized + 3;
    localizedVRBInfo_p->previousVRBfreeIndex = maxVRBLocalized; 
    
    localizedVRBInfo_p--;
    localizedVRBInfo_p->nextfreeVRBIndex = maxVRBLocalized + 2;
    localizedVRBInfo_p->previousVRBfreeIndex = maxVRBLocalized; 
    
    localizedVRBInfo_p--;
    /*Below Logic is poluating ratLocalizedInfo_p structure such that 
     * Intializing all VRB as free  and also a Free VRB know the next  
     * Free VRB Index and previous Free VRB Index*/
    localizedVRBInfo_p->nextfreeVRBIndex = maxVRBLocalized + 2;
    localizedVRBInfo_p->previousVRBfreeIndex = maxVRBLocalized - 1;

    rbgIndex = (maxVRBLocalized - 1) / rbgSize;
    localizedVRBInfo_p->rbgIndexBitMap = ~((UInt32)1 << rbgIndex);

    localizedVRBInfo_p->subsetIndex = rbgIndex % rbgSize;
    prbPositionWithinSubset =  (((maxVRBLocalized - 1) / rbgSize) /
                    (rbgSize ) ) * rbgSize +  ((maxVRBLocalized - 1) % rbgSize);
    localizedVRBInfo_p->subsetBitMap = ~(((UInt32)1) << (
                                          prbPositionWithinSubset )); 
    localizedVRBInfo_p--;
    maxVRBLocalized--;        
    while ( maxVRBLocalized )
    {
        localizedVRBInfo_p->nextfreeVRBIndex = maxVRBLocalized + 1;
        localizedVRBInfo_p->previousVRBfreeIndex = maxVRBLocalized - 1;
        rbgIndex = (maxVRBLocalized - 1) / rbgSize ;
        localizedVRBInfo_p->rbgIndexBitMap = ~((UInt32)1 << rbgIndex);
        localizedVRBInfo_p->subsetIndex = rbgIndex % rbgSize;
        prbPositionWithinSubset =  (((maxVRBLocalized - 1) / rbgSize) / 
                (rbgSize ) ) * rbgSize +  ((maxVRBLocalized - 1) % rbgSize);

        localizedVRBInfo_p->subsetBitMap = ~(((UInt32)1) << (
                    prbPositionWithinSubset ));
        localizedVRBInfo_p--;
        maxVRBLocalized--;
    }
    localizedVRBInfo_p->nextfreeVRBIndex = 1;
    localizedVRBInfo_p->previousVRBfreeIndex = 0;    

    initPrbToVrbInfo(&raPolicyInfo_p->rat2LocalizedInfo,&raPolicyInfo_p->
            rat2DistributedInfo);  
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}



/******************************************************************************
 * Function Name    : initPrbToVrbInfo
 * Inputs           : rat2LocalizedInfo - This is pointer to RAT2LocalizedInfo 
 *                                        Structure,
 *                    rat2DistributedInfo - This is pointer to RAT2DistributedInfo 
 *                                          Structure            
 * Outputs          : None                
 * Returns          : None
 * Description      : This function will update the rat2LocalizedInfo structure that
 *                    PRB knows the VRBs Number which is mapped to its even and odd 
 *                    slot.
 ******************************************************************************/
STATIC void initPrbToVrbInfo(RAT2LocalizedInfo *rat2LocalizedInfo,
        RAT2DistributedInfo *rat2DistributedInfo)
{
    UInt32 maxLocalizedVRBCount = 0;
    UInt32 maxDistributedVRBCount = 0;
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;
    RAT2DistributedVRBInfo *distributedVRBInfo_p = PNULL;
    UInt32 prbIndex = 0;
    UInt32 vrbIndex = 0;
    UInt32 prbMappedToEvenSlotOfVRB = 0;
    UInt32 prbMappedToOddSlotOfVRB = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    maxLocalizedVRBCount = rat2LocalizedInfo->maxLocalizedVRB;
    maxDistributedVRBCount = rat2DistributedInfo->maxDistributedVRB;
    localizedVRBInfo_p = rat2LocalizedInfo->rat2LocalizedVRBInfo;
    distributedVRBInfo_p = rat2DistributedInfo->rat2DistributedVRBInfo;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "maxLocalizedVRBCount = %u\n",maxLocalizedVRBCount);
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "maxDistributedVRBCount = %u\n",maxDistributedVRBCount);

    /* Below While loop is finding the VRB whose even slot is mapped to given 
     * PRB and also finding VRB whose odd slot is mapped to given PRB */
    while ( maxLocalizedVRBCount )
    {   
        /*As PRB start from 0 Index ,So Substracting one from it*/ 
        prbIndex = maxLocalizedVRBCount - 1;       
        maxDistributedVRBCount = rat2DistributedInfo->maxDistributedVRB;
        /* Below While loop is iterating through each VRB information and 
         * checking if  it's even slot is mapped to  the PRB having number 
         * prbIndex and also checking if  it's even slot is mapped to the PRB
         * having  number prbIndex */
        while( maxDistributedVRBCount )
        {
            vrbIndex = maxDistributedVRBCount;
            prbMappedToEvenSlotOfVRB = distributedVRBInfo_p[vrbIndex].
                prbMappedToEvenSlotOfVRB;
            prbMappedToOddSlotOfVRB = distributedVRBInfo_p[vrbIndex].
                prbMappedToOddSlotOfVRB;

            if ( prbIndex == prbMappedToEvenSlotOfVRB )
            {
                localizedVRBInfo_p[maxLocalizedVRBCount].
                    vrbMappedToEvenSlotOfPRB = vrbIndex;
            }
            if ( prbIndex == prbMappedToOddSlotOfVRB )
            {
                localizedVRBInfo_p[maxLocalizedVRBCount].
                    vrbMappedToOddSlotOfPRB = vrbIndex;
            }
            maxDistributedVRBCount--;
        }
        maxLocalizedVRBCount--;        
    }   
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);       
}

/******************************************************************************
 * Function Name    : initAllResourceAllocationPolicyInfo
 * Inputs           : internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function will call relevant funtions for intialization 
 *                    of Resource Allocation Information for RAT 0 ,RAT 1, RAT 2
 *                    Localized, RAT 2 Distributed.
 *                    This function is called at cell configuration
 ******************************************************************************/
 void initAllResourceAllocationPolicyInfo(InternalCellIndex internalCellIndex)
{
    ResourceAllocationPolicyInfo *raPolicyInfo_p = PNULL;
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;
    UInt32 dlRBs = 0;
    /** opt **/
    UInt8 vDlRBsGap1;
    UInt8 flrOfvDlRBs;
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt8 dlPRSRATPoliciesIndex = cellParams_p->prsConfigInfo.dlPRSRATPoliciesIndex;

    /** opt **/
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);

    /* The mem pool will be created only first time when cell was configured */ 
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        createMemPool(sizeof(DLResourceAllocationInfo)* MAX_RA_INFO_INDEX, 1);
    }

    GET_MEM_FROM_POOL(DLResourceAllocationInfo,dlRAInfo_gp[internalCellIndex],sizeof(DLResourceAllocationInfo)*MAX_RA_INFO_INDEX, PNULL);
    raPolicyInfo_p = &((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo);
    rat0Info_p = &raPolicyInfo_p->rat0Info;
    rat1Info_p = &raPolicyInfo_p->rat1Info;
    dlRBs = cellParams_p->dlAvailableRBs;

    /* rbgSize_g is set According to TS 36.213 V8.4.0  Table 7.1.6.1-1: 
     *Type 0 Resource Allocation RBG Size vs. Downlink System  Bandwidth*/    
    if(dlRBs <= 10)
    {
        rat0Info_p->rbgSize = 1;  
        rat1Info_p->subsetCount = 1;
        rat1Info_p->bitsCountForSubsetCount = 1;
    }
    else if(dlRBs >= 11 && dlRBs <= 26)
    {
        rat0Info_p->rbgSize = 2;  
        rat1Info_p->subsetCount = 2;
        rat1Info_p->bitsCountForSubsetCount = 1;
    }
    else if(dlRBs >= 27 && dlRBs <=63)
    {
        rat0Info_p->rbgSize = 3;  
        rat1Info_p->subsetCount = 3;
        rat1Info_p->bitsCountForSubsetCount = 2;
    }
    /* +COVERITY 10714 */
    else if(dlRBs >= 64 && dlRBs <= 100)
        /* -COVERITY 10714 */
    {
        rat0Info_p->rbgSize = 4;  
        rat1Info_p->subsetCount = 4;
        rat1Info_p->bitsCountForSubsetCount = 2;
    }
    else
    {
        ltePanic("Resource Blocks are not valid\n");        
        /* Coverity Warning : 60466 */
        freeMemPool(dlRAInfo_gp[internalCellIndex]);
        dlRAInfo_gp[internalCellIndex] = PNULL;
        return ;
        /* Coverity Warning : 60466 */
    } 
    /* Below funtion will intialize  Resource Allocation information of 
       RAT 0 */
    initRAT0Info(rat0Info_p ,dlRBs);
    /* Below funtion will intialize  Resource Allocation information of 
       RAT 1  */
    if ( dlRBs > 10 )
    {
        initRAT1Info(rat1Info_p ,dlRBs);
    }
    /* Below funtion will intialize  Resource Allocation information of 
       RAT 2 Distributed */
    initRAT2DistributedInfo(raPolicyInfo_p ,dlRBs, internalCellIndex);
    /* Below funtion will  Resource Allocation information of 
       RAT 2 Localized  */   
    initRAT2LocalizedInfo( raPolicyInfo_p ,dlRBs);

    memCpy(&((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfoMIBReserved),  
            &((dlRAInfo_gp[internalCellIndex]+ dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo), 
            sizeof(ResourceAllocationPolicyInfo ));

    /* ICIC changes start */
    /* Initialize rbMapToResourceTypeMapDL such that all resources are 
     * in cell centre only region 
     */
    memSet ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->rbToResourceTypeMapDL, 
            CELL_CENTRE_ONLY_REGION,
            dlRBs);

    /* Initialize parameters in structure DLResourceAllocationInfo */
    (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalCellEdgeRB = 0;
    (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB = dlRBs;

    initializeRAPolicyMibScheduled(dlRAInfoIndex_g[internalCellIndex], internalCellIndex);

    /** opt **/

    dlRBs = cellParams_p->dlAvailableRBs;
    /*
     * For DCI_Format 1C according to Table 7.1.6.3-1 in spec 36.213
     */
    cellParams_p->resAllocInitInfo.nRBStepDCI1C = (((dlRBs >= 6)&&(dlRBs <= 49))?2:4);
    /* +- SPR 17777 */
    fillNGapValuesForDCIFormat1C(&(cellParams_p->resAllocInitInfo.vDlRBDCI1C),
            &vDlRBsGap1,dlRBs,internalCellIndex);
    /* +- SPR 17777 */

    /*
     * According to section 5.3.3.1.4 in spec 36.212 
     */
    flrOfvDlRBs  = floor_wrapper(vDlRBsGap1/cellParams_p->resAllocInitInfo.nRBStepDCI1C);
    cellParams_p->resAllocInitInfo.numberOfBitsForAllocationDCI1C = ceil_wrapper(log_wrapper((flrOfvDlRBs * (flrOfvDlRBs + 1) / 2)) / LOG2);
    cellParams_p->resAllocInitInfo.numberOfBitsForAllocationNonDCI1C = ceil_wrapper(log_wrapper((dlRBs * (dlRBs + 1 ) / 2)) / LOG2);
    /** opt **/


    /* Take copy of resource allocation policy, to be used for reconfiguration
     * by RRM 
     */
    memCpy(&((dlRAInfo_gp[internalCellIndex] + !(dlRAInfoIndex_g[internalCellIndex]))->raPolicyInfo),  
            &((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo),
            sizeof(ResourceAllocationPolicyInfo ));

    memCpy(&((dlRAInfo_gp[internalCellIndex] +!(dlRAInfoIndex_g[internalCellIndex]))->raPolicyInfoMIBReserved),  
            &((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfoMIBReserved), 
            sizeof(ResourceAllocationPolicyInfo ));
    /* ICIC changes end */
    /* + PRS_CHANGES */

    memCpy(&((dlRAInfo_gp[internalCellIndex] + dlPRSRATPoliciesIndex)->raPolicyInfoPRSReserved),  
            &((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo), 
            sizeof(ResourceAllocationPolicyInfo ));

    if ((cellParams_p->prsConfigInfo.isPRSConfigured) &&
            (cellParams_p->prsConfigInfo.prsResInfo[dlPRSRATPoliciesIndex].startPRB 
             != cellParams_p->prsConfigInfo.prsResInfo[dlPRSRATPoliciesIndex].endPRB))
    {
        initPRSRATPoliciesBitmaps(&((dlRAInfo_gp[internalCellIndex] +dlPRSRATPoliciesIndex)->\
                    raPolicyInfoPRSReserved),dlPRSRATPoliciesIndex,
                internalCellIndex);
    }
    memCpy(&((dlRAInfo_gp[internalCellIndex] +!(dlPRSRATPoliciesIndex))->raPolicyInfoPRSReserved),  
            &((dlRAInfo_gp[internalCellIndex] + dlPRSRATPoliciesIndex)->raPolicyInfoPRSReserved),
            sizeof(ResourceAllocationPolicyInfo ));
    /* - PRS_CHANGES */
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "leaving funtion = %s\n",__func__);  
}

/******************************************************************************
 * Function Name    : updateRAT1InfoFromRAT0Info
 * Inputs           : rat1Info_p - This holds the pointer to RAT1Info structure,
 *                    rbgIndexInfo_p - This holds the pointer to RBGIndexInfo structure
 * Outputs          : None
 * Returns          : None
 * Description      : This funtion will update the RAT 1 Information based on 
 *                    RAT 0 Information.
 ******************************************************************************/
STATIC  void  updateRAT1InfoFromRAT0Info(       
        RAT1Info *rat1Info_p,           
        RBGIndexInfo *rbgIndexInfo_p)
{    
    SubsetInfo *subSetInfo_p = PNULL;
    subSetInfo_p = &rat1Info_p->subsetInfo[rbgIndexInfo_p->subsetIndex];
    /*Unset the bits corrosponding to the RBG that falls in  the Subset
     * Indexed by subsetIndex*/
    /* ICIC changes start */
    /* Only update subset bitmap for cell centre user, bitmap for cell edge 
     * user is derived from cell centre bitmap 
     */
    subSetInfo_p->subsetBitMap[CC_USER] &= rbgIndexInfo_p->rbgSizeBitMapForSubset;   
    /* ICIC changes end */
    /*Decerementing  the Number of free PRB in subset by rbgSize*/
    subSetInfo_p->numPRBFree -= rbgIndexInfo_p->numOfRBs;
}

/******************************************************************************
 *
 * Function Name    : updateRAT2InfoFromRAT0Info
 * Inputs           : rbgIndex - This holds the RBG Index,
 *                    rbgSize - This holds the RBG Size,
 *                    rbsInRBG
 *                    raPolicy_p - This holds pointer to ResourceAllocationPolicyInfo
 *                                 structure,
 *                    calculatedRNTP - This holds Calculated RNTP
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtion will update the RAT 2 Localized and RAT 2
 *                    Distributed Information based on RAT 0 Information.
 ******************************************************************************/
STATIC  void  updateRAT2InfoFromRAT0Info(UInt32 rbgIndex ,
        UInt32 rbgSize,
        UInt32 rbsInRBG,
        ResourceAllocationPolicyInfo *raPolicy_p,
        /* +DYNAMIC_ICIC */
        UInt8 calculatedRNTP,
        InternalCellIndex internalCellIndex) 
        /* -DYNAMIC_ICIC */
{
    UInt32 prbNumber =0;
    UInt32 previousFreeVRBIndex = 0;
    UInt32 nextFreeVRBIndex = 0;
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL; 
    RAT2LocalizedVRBInfo *tempLocalizedVRBInfo_p = PNULL; 
    UInt32 startingRBNumber = 0;
    UInt32 tempRBsInRBG = 0;
    
    /* RAT2 Distributed information need not be updated for data messages,
     * as RAT2 Distributed is not used for data messages
     */

    localizedVRBInfo_p = raPolicy_p->rat2LocalizedInfo.rat2LocalizedVRBInfo;

    /*Get the starting prbNumber which falls in the RBG having index rbgIndex*/
    prbNumber = rbgIndex * rbgSize;
    startingRBNumber = prbNumber;

    tempRBsInRBG = rbsInRBG;
    /*This while loop is iterating rbgSize number of times .In each iteration
     *it is updating the Localized and Distributed RAT 2 information*/
    while ( rbsInRBG--)
    {

        /* RAT2 Distributed information need not be updated for data messages,
         * as RAT2 Distributed is not used for data messages
         */
        /* ICIC changes start */
        /* update numOfCellEdgeVRBAvailable, if given PRB falls in cell
         * edge region
         */
        if(CELL_EDGE_ONLY_REGION == 
            (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->rbToResourceTypeMapDL[prbNumber])
        {
            raPolicy_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable -= 1;
           
            /* RAT2 distributed support for ICIC needs to be added */
        }
        /* +DYNAMIC_ICIC */
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd. \
            dlICICReportInfo_p[rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            activeReportIndex].rntpValuePerPRB[prbNumber] += calculatedRNTP;
        /* -DYNAMIC_ICIC */
        /* ICIC changes end */
#ifdef PERF_STATS
        /* Overlapping RB global is updated*/
     /** CA-TDD Changes Start **/
	if(perfStatsMibSyncSignalFlag_g[internalCellIndex] && (prbNumber >= central6RBStartIndex_g[internalCellIndex] && 
                                             prbNumber <= central6RBStartIndex_g[internalCellIndex] + 5))
    /** CA-TDD Changes Stop **/
	{
		overlapRBsMibSS_g[internalCellIndex]++;
	}
#endif
        prbNumber++;
    }
    /*Updating the localizedVRBInfo_p array such that a Free VRB knows
     *the next Free VRB Index and also previous Free VRB Index*/
    
    tempLocalizedVRBInfo_p = &localizedVRBInfo_p[startingRBNumber + 1];
    
    previousFreeVRBIndex = tempLocalizedVRBInfo_p->previousVRBfreeIndex;
    nextFreeVRBIndex = tempLocalizedVRBInfo_p[tempRBsInRBG - 1].nextfreeVRBIndex;
    
    /* ICIC changes start */
    if (raPolicy_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable)
    {
        /* Here we are checking, if VRBs corresponding to allocated  
         * RBG are same as firstCellEdgeFreeIndex or endCellEdgeFreeIndex. 
         * If so, we are modifying these indices.
         */
        if ((raPolicy_p->rat2LocalizedInfo.firstCellEdgeFreeIndex < 
                    nextFreeVRBIndex) &&
                (raPolicy_p->rat2LocalizedInfo.firstCellEdgeFreeIndex > 
                 previousFreeVRBIndex))
        {
            raPolicy_p->rat2LocalizedInfo.firstCellEdgeFreeIndex = 
                nextFreeVRBIndex;
        }
        if ((raPolicy_p->rat2LocalizedInfo.endCellEdgeFreeIndex < 
                    nextFreeVRBIndex) &&
                (raPolicy_p->rat2LocalizedInfo.endCellEdgeFreeIndex > 
                 previousFreeVRBIndex))
        {
            raPolicy_p->rat2LocalizedInfo.endCellEdgeFreeIndex = 
                previousFreeVRBIndex;
        }
    }
    /* ICIC changes end */

    localizedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex = 
        nextFreeVRBIndex;
    localizedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex = 
        previousFreeVRBIndex;

    raPolicy_p->rat2LocalizedInfo.numOfVRBAvailable -= tempRBsInRBG;
    return;
}

/******************************************************************************
 * Function Name    : getMcsIndexForCW1
 * Inputs           : availableRBGBitMap - This Holds  the BitMap where each set bit 
 *                                         represents a Free RBG and bit postion represents 
 *                                         RBG Index,
 *                    subBandReportInfo_p - This is pointer to  SubBandReportInfo 
 *                                          structure,
 *                    prbCount - Count of PRB
 * Outputs          : mcsIndexAllocatedCW1_p - This Holds the Lowest MCS Index found 
 *                    in availableRBGBitMap for CodeWord One.
 * Returns          : None
 * Description      : This funtion will find the lowest CQI index found in the 
 *                    availableRBGBitMap BitMap for CodeWord One.
 ******************************************************************************/
STATIC  void getMcsIndexForCW1( 
        UInt32 *mcsIndexAllocatedCW1_p,    
        UInt32 availableRBGBitMap,
        SubBandReportInfo *subBandReportInfo_p,
        UInt32 prbCount)
{
    UInt32 rbgPosition = 0;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    SubBandToCQInfo *tempSubBandToCQInfo_p = PNULL;
    UInt32 lowestCqiIndexCW = MAX_CQI_INDEX;    

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[0];
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "availableRBGBitMap = %u\n",availableRBGBitMap);
    while ((prbCount--) && availableRBGBitMap)
    {
        /*Get the rbgPostion*/
        rbgPosition = FFS( availableRBGBitMap );
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "rbgPosition = %u\n",rbgPosition);
        /*As rbgPostion found start from 1 ,So need to decerement by one
          to make it start from 0*/
        rbgPosition--;
        tempSubBandToCQInfo_p = &subBandToCQInfo_p[rbgPosition];
        if (lowestCqiIndexCW > tempSubBandToCQInfo_p->cqiIndexCW1)
        {
            *mcsIndexAllocatedCW1_p = tempSubBandToCQInfo_p->mcsIndexCW1;
            lowestCqiIndexCW = tempSubBandToCQInfo_p->cqiIndexCW1;
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "lowestCqiIndexCW = %u\n",lowestCqiIndexCW);\
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "mcsIndexAllocatedCW1_p = %u\n",*mcsIndexAllocatedCW1_p);
        }    
        /*Unset the bit whose position is marked by rbgPosition*/
        availableRBGBitMap &= (~((UInt32)(1 << rbgPosition)));
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "availableRBGBitMap = %u\n",availableRBGBitMap);
    }    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}


/******************************************************************************
 * Function Name    : getMcsIndexForCW2
 * Inputs           : availableRBGBitMap - This Holds  the BitMap where each set bit 
 *                                         represents a Free RBG and bit postion represents
 *                                         RBG Index,
 *                    subBandReportInfo_p - This is pointer to  SubBandReportInfo 
 *                                          structure,
 *                    prbCount - Count of PRB
 * Outputs          : mcsIndexAllocatedCW2_p - This Holds the Lowest MCS Index found in
 *                                              availableRBGBitMap for CodeWord Two.    
 * Returns          : None
 * Description      : This funtion will find the lowest CQI index found in the 
 *                    availableRBGBitMap BitMap for CodeWord Two. 
 ******************************************************************************/
STATIC  void getMcsIndexForCW2( 
        UInt32 *mcsIndexAllocatedCW2_p,    
        UInt32 availableRBGBitMap,
        SubBandReportInfo *subBandReportInfo_p,
        UInt32 prbCount)
{
    UInt32 rbgPosition = 0;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    SubBandToCQInfo *tempSubBandToCQInfo_p = PNULL;
    UInt32 lowestCqiIndexCW = MAX_CQI_INDEX;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);

    subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[0];

    while ((prbCount--) && availableRBGBitMap)
    {
        /*Get the rbgPostion*/
        rbgPosition = FFS( availableRBGBitMap );
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "rbgPosition = %u\n",rbgPosition);
        /*As rbgPostion found start from 1 ,So need to decerement by one
          to make it start from 0*/
        rbgPosition--;
        tempSubBandToCQInfo_p = &subBandToCQInfo_p[rbgPosition];
        if (lowestCqiIndexCW > tempSubBandToCQInfo_p->cqiIndexCW2)
        {
            *mcsIndexAllocatedCW2_p = tempSubBandToCQInfo_p->mcsIndexCW2;
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "mcsIndexAllocatedCW2_p = %u\n",*mcsIndexAllocatedCW2_p);
            lowestCqiIndexCW = tempSubBandToCQInfo_p->cqiIndexCW2;
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "lowestCqiIndexCW = %u\n",lowestCqiIndexCW);
        }       
        /*Unset the bit whose position is marked by rbgPosition*/
        availableRBGBitMap &= (~((UInt32)(1 << rbgPosition)));
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "availableRBGBitMap = %u\n",availableRBGBitMap);   
    }    

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}

/******************************************************************************
 * Function Name    : getMcsIndexForCW1AndCW2
 * Inputs           : availableRBGBitMap - This Holds  the BitMap where each set bit 
 *                                         represents a Free RBG and bit postion represents
 *                                         RBG Index,
 *                    subBandReportInfo_p - This is pointer to  SubBandReportInfo 
 *                                          structure,
 *                    prbCount - Count of PRB
 * Outputs          : mcsIndexAllocatedCW1_p - This Holds the Lowest MCS Index found in
 *                                             availableRBGBitMap for CodeWord 0. 
 *                    mcsIndexAllocatedCW2_p - This Holds the Lowest MCS Index found in
 *                                             availableRBGBitMap for CodeWord 1.
 * Returns          : None
 * Description      : This funtion will find the lowest MCS index found in the 
 *                    availableRBGBitMap BitMap for CodeWord 1 and CodeWord 2.
 ******************************************************************************/

STATIC  void getMcsIndexForCW1AndCW2(
        UInt32 *mcsIndexAllocatedCW1_p,
        UInt32 *mcsIndexAllocatedCW2_p,
        UInt32 availableRBGBitMap,
        SubBandReportInfo *subBandReportInfo_p,
        UInt32 prbCount)
{
    UInt32 rbgPosition = 0;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    SubBandToCQInfo *tempSubBandToCQInfo_p = PNULL;
    UInt32 lowestCqiIndexCW1 = MAX_CQI_INDEX;
    UInt32 lowestCqiIndexCW2 = MAX_CQI_INDEX;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[0];

    while((prbCount--) && availableRBGBitMap)
    {
        /*Get the rbgPostion*/
        rbgPosition = FFS( availableRBGBitMap );
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "rbgPosition = %u\n",rbgPosition);
        /*As rbgPostion found start from 1 ,So need to decerement by one
          to make it start from 0*/
        rbgPosition--;
        tempSubBandToCQInfo_p = &subBandToCQInfo_p[rbgPosition];
        if (lowestCqiIndexCW1 > tempSubBandToCQInfo_p->cqiIndexCW1)
        {
            *mcsIndexAllocatedCW1_p = tempSubBandToCQInfo_p->mcsIndexCW1;
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "mcsIndexAllocatedCW1_p = %u\n",*mcsIndexAllocatedCW1_p);
            lowestCqiIndexCW1 = tempSubBandToCQInfo_p->cqiIndexCW1;
        }
        if (lowestCqiIndexCW2 > tempSubBandToCQInfo_p->cqiIndexCW2)
        {
            *mcsIndexAllocatedCW2_p = tempSubBandToCQInfo_p->mcsIndexCW2;
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "mcsIndexAllocatedCW2_p = %u\n",*mcsIndexAllocatedCW2_p);
            lowestCqiIndexCW2 = tempSubBandToCQInfo_p->cqiIndexCW2;
        }
        /*Unset the bit whose position is marked by rbgPosition*/
        availableRBGBitMap &= (~((UInt32)(1 << rbgPosition)));
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "availableRBGBitMap = %u\n",availableRBGBitMap);   
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}

/******************************************************************************
 * Function Name    : getMcsIndexForCW1ForRAT1
 * Inputs           : availableRBGBitMap - This Holds  the BitMap where each set bit 
 *                                         represents a Free RBG and bit postion represents
 *                                         RBG Index,
 *                    subBandReportInfo_p - This is pointer to  SubBandReportInfo 
 *                                          structure,
 *                    bitPositionToSubsetAndRBGInfo_p - This holds address of 
 *                                                      BitPositionToSubsetAndRBGInfo structure,
 *                    rbCount - Count of RB
 * Outputs          : mcsIndexAllocatedCW1_p - This Holds the Lowest MCS Index found 
 *                                             in availableRBGBitMap for CodeWord One.
 * Returns          : None
 * Description      : This funtion will find the lowest CQI index found in the 
 *                    availableRBGBitMap BitMap for CodeWord One.
 ******************************************************************************/
STATIC  void getMcsIndexForCW1ForRAT1( 
        UInt32 *mcsIndexAllocatedCW1_p,    
        UInt32 availableRBGBitMap,
        SubBandReportInfo *subBandReportInfo_p,
        BitPositionToSubsetAndRBGInfo *bitPositionToSubsetAndRBGInfo_p,
        UInt32 rbCount)
{
    UInt32 bitPosition = 0;
    UInt32 rbgIndex = 0;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    SubBandToCQInfo *tempSubBandToCQInfo_p = PNULL;
    UInt32 lowestCqiIndexCW = MAX_CQI_INDEX;    

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[0];
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "availableRBGBitMap = %u\n",availableRBGBitMap);
    
    while (rbCount--)
    {
        /*Get the bitPosition*/
        bitPosition = FFS( availableRBGBitMap );
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "bitPosition = %u\n",bitPosition);
        /*As bitPosition found start from 1 ,So need to decerement by one
          to make it start from 0*/
        bitPosition--;
        rbgIndex = bitPositionToSubsetAndRBGInfo_p[bitPosition].rbgIndex;
        tempSubBandToCQInfo_p = &subBandToCQInfo_p[rbgIndex];
        if (lowestCqiIndexCW > tempSubBandToCQInfo_p->cqiIndexCW1)
        {
            *mcsIndexAllocatedCW1_p = tempSubBandToCQInfo_p->mcsIndexCW1;
            lowestCqiIndexCW = tempSubBandToCQInfo_p->cqiIndexCW1;
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "lowestCqiIndexCW = %u\n",lowestCqiIndexCW);\
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "mcsIndexAllocatedCW1_p = %u\n",*mcsIndexAllocatedCW1_p);
        }    
        /*Unset the bit whose position is marked by rbgPosition*/
        availableRBGBitMap &= (~((UInt32)(1 << bitPosition)));
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "availableRBGBitMap = %u\n",availableRBGBitMap);
    }    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}

/******************************************************************************
 * Function Name    : getMcsIndexForCW2ForRAT1
 * Inputs           : availableRBGBitMap - This Holds  the BitMap where each set bit 
 *                                         represents a Free RBG and bit postion represents
 *                                         RBG Index,
 *                    subBandReportInfo_p - This is pointer to  SubBandReportInfo 
 *                                          structure,
 *                    bitPositionToSubsetAndRBGInfo_p - This holds address of 
 *                                                      BitPositionToSubsetAndRBGInfo structure,
 *                    rbCount - Count of RB
 * Outputs          : mcsIndexAllocatedCW1_p - This Holds the Lowest MCS Index found 
 *                                             in availableRBGBitMap for CodeWord One.
 * Returns          : None
 * Description      : This funtion will find the lowest CQI index found in the 
 *                    availableRBGBitMap BitMap for CodeWord Two.
 ******************************************************************************/
STATIC  void getMcsIndexForCW2ForRAT1( 
        UInt32 *mcsIndexAllocatedCW2_p,    
        UInt32 availableRBGBitMap,
        SubBandReportInfo *subBandReportInfo_p,
        BitPositionToSubsetAndRBGInfo *bitPositionToSubsetAndRBGInfo_p,
        UInt32 rbCount)
{
    UInt32 bitPosition = 0;
    UInt32 rbgIndex = 0;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    SubBandToCQInfo *tempSubBandToCQInfo_p = PNULL;
    UInt32 lowestCqiIndexCW = MAX_CQI_INDEX;    

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[0];
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "availableRBGBitMap = %u\n",availableRBGBitMap);
    
    while (rbCount--)
    {
        /*Get the bitPosition*/
        bitPosition = FFS( availableRBGBitMap );
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "bitPosition = %u\n",bitPosition);
        /*As bitPosition found start from 1 ,So need to decerement by one
          to make it start from 0*/
        bitPosition--;
        rbgIndex = bitPositionToSubsetAndRBGInfo_p[bitPosition].rbgIndex;
        tempSubBandToCQInfo_p = &subBandToCQInfo_p[rbgIndex];
        if (lowestCqiIndexCW > tempSubBandToCQInfo_p->cqiIndexCW2)
        {
            *mcsIndexAllocatedCW2_p = tempSubBandToCQInfo_p->mcsIndexCW2;
            lowestCqiIndexCW = tempSubBandToCQInfo_p->cqiIndexCW2;
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "lowestCqiIndexCW = %u\n",lowestCqiIndexCW);\
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "mcsIndexAllocatedCW2_p = %u\n",
                        *mcsIndexAllocatedCW2_p);
        }    
        /*Unset the bit whose position is marked by rbgPosition*/
        availableRBGBitMap &= (~((UInt32)(1 << bitPosition)));
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "availableRBGBitMap = %u\n",availableRBGBitMap);
    }    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}

/******************************************************************************
 * Function Name    : getMcsIndexForCW1AndCW2RAT1
 * Inputs           : availableRBGBitMap - This Holds  the BitMap where each set bit 
 *                                         represents a Free RBG and bit postion represents
 *                                         RBG Index,
 *                    subBandReportInfo_p - This is pointer to  SubBandReportInfo 
 *                                          structure,
 *                    bitPositionToSubsetAndRBGInfo_p - This holds address of 
 *                                                      BitPositionToSubsetAndRBGInfo structure,
 *                    rbCount - Count of RB,
 * Outputs          : mcsIndexAllocatedCW1_p - This Holds the Lowest MCS Index found
 *                                             in availableRBGBitMap for CodeWord 0,
 *                    mcsIndexAllocatedCW2_p - This Holds the Lowest MCS Index found 
 *                                             in availableRBGBitMap for CodeWord 1
 * Returns          : None
 * Description      : This funtion will find the lowest MCS index found in the 
 *                    availableRBGBitMap BitMap for CodeWord 1 and CodeWord 2. 
******************************************************************************/

STATIC  void getMcsIndexForCW1AndCW2RAT1(
        UInt32 *mcsIndexAllocatedCW1_p,
        UInt32 *mcsIndexAllocatedCW2_p,
        UInt32 availableRBGBitMap,
        SubBandReportInfo *subBandReportInfo_p,
        BitPositionToSubsetAndRBGInfo *bitPositionToSubsetAndRBGInfo_p,
        UInt32 rbCount)
{
    UInt32 bitPosition = 0;
    UInt32 rbgIndex = 0;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;    
    SubBandToCQInfo *tempSubBandToCQInfo_p = PNULL;
    UInt32 lowestCqiIndexCW1 = MAX_CQI_INDEX;
    UInt32 lowestCqiIndexCW2 = MAX_CQI_INDEX; 
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[0];

    while(rbCount--)
    {
        /*Get the rbgPostion*/
        bitPosition = FFS( availableRBGBitMap );
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "bitPosition = %u\n",bitPosition);
        /*As rbgPostion found start from 1 ,So need to decerement by one
          to make it start from 0*/
        bitPosition--;
        rbgIndex = bitPositionToSubsetAndRBGInfo_p[bitPosition].rbgIndex;
        tempSubBandToCQInfo_p = &subBandToCQInfo_p[rbgIndex];
        if (lowestCqiIndexCW1 > tempSubBandToCQInfo_p->cqiIndexCW1)
        {
            *mcsIndexAllocatedCW1_p = tempSubBandToCQInfo_p->mcsIndexCW1;
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "mcsIndexAllocatedCW1_p = %u\n",*mcsIndexAllocatedCW1_p);
            lowestCqiIndexCW1 = tempSubBandToCQInfo_p->cqiIndexCW1;
        }
        if (lowestCqiIndexCW2 > tempSubBandToCQInfo_p->cqiIndexCW2)
        {
            *mcsIndexAllocatedCW2_p = tempSubBandToCQInfo_p->mcsIndexCW2;
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "mcsIndexAllocatedCW2_p = %u\n",*mcsIndexAllocatedCW2_p);
            lowestCqiIndexCW2 = tempSubBandToCQInfo_p->cqiIndexCW2;
        }
        /*Unset the bit whose position is marked by rbgPosition*/
        availableRBGBitMap &= (~((UInt32)(1 << bitPosition)));
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "availableRBGBitMap = %u\n",availableRBGBitMap);   
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}


/******************************************************************************
 * Function Name    : validateRBsforMinMaxTBSize
 * Inputs           : rbgBitMap - This Holds  the BitMap where each set bit 
 *                                represents a Free RBG and bit postion represents RBG Index,
 *                    rat0Info_p - This holds the pointer to RAT0Info structure,
 *                    resrcAllocatorInput_p - Pointer to ResourceAllocatorInput, 
 *                    harqProcess_p - Pointer to DLHARQProcess,
 *                    previousRsrcFrmt_p - Pointer to ResourceFormatMIMONode,
 *                    dlStrategyNode_p - Pointer to DLStrategyTxNode
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will check if the available RB Count is greater 
 *                    than required RB Count .If true it will populate the relevant
 *                    information in harqProcess_p structure.It will also check 
 *                    against HARQ_TB_MINIMUM_SIZE_IN_BYTES for CodeWord One 
 *                    and CodeWordTwo.
 ******************************************************************************/ 
STATIC MacRetType validateRBsforMinMaxTBSize(
        UInt32 rbgBitMap,
        RAT0Info *rat0Info_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        ResourceFormatMIMONode *previousRsrcFrmt_p,
        DLStrategyTxNode *dlStrategyNode_p,
        InternalCellIndex internalCellIndex)
{
    UInt32 availableRBCount = 0;
    UInt32 allocatedRB = 0;
    DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
    DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL;

    UInt32 freqSelectiveMCSIndexCW1 = 0;
    UInt32 freqSelectiveMCSIndexCW2 = 0;

    UInt32 freqSelectiveTBSize = 0;
    UInt32 freqSelectiveTBSizeCW1 = 0;
    UInt32 freqSelectiveTBSizeCW2 = 0;

    SubBandReportInfo *subBandReportInfo_p = resrcAllocatorInput_p->subBandReportInfo_p;
    UInt32 requiredRBCount = resrcAllocatorInput_p->requiredRB; 
    
    UInt32 minTBSize = dlStrategyNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyNode_p->maxTBSize;
    UInt8 excessPRBs = 0;
    
    /* ICIC changse start */
    UInt8 resourceRegionAllocated = resrcAllocatorInput_p->resourceRegionAllocated;
    /* ICIC changse end */
    
    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
   /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *ueContext_p = PNULL;
    dlUEContextInfo_p = &dlUECtxInfoArr_g[dlStrategyNode_p->ueIndex];
    ueContext_p = dlUEContextInfo_p->dlUEContext_p;
#endif

    if (maxTBSize < minTBSize)
    {
        lteWarning("[RAT0 Spatial] Wrong Max TBSize calculated\
                     by DL Strategy minTBSize [%d] maxTBSize [%d]\n", 
                     minTBSize, maxTBSize);
        
        maxTBSize = minTBSize;
    }        

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo;

    /*Get the count of number of Available RBs*/
    availableRBCount = getCount(rbgBitMap) * rat0Info_p->rbgSize;
    if (rbgBitMap & ((UInt32) 1 << (rat0Info_p->rbgCount - 1)))
    {
        availableRBCount = availableRBCount - rat0Info_p->rbgSize +
            rat0Info_p->rbgIndexInfo[rat0Info_p->rbgCount - 1].numOfRBs;
    }
    /* TDD SB_CQI */
    if( (!availableRBCount) || (!requiredRBCount))
    {
      return MAC_FAILURE;
    }
    /* TDD SB_CQI */

    if (availableRBCount >= requiredRBCount)
    {
        allocatedRB = requiredRBCount;              
        
        /*Calling funtion getMcsIndexForCW1AndCW2 which will populate 
                * the Mcs index of CodeWord One and CodeWord Two in 
                *mcsIndexCW1 and mcsIndexCW2*/
         getMcsIndexForCW1AndCW2(&freqSelectiveMCSIndexCW1,&freqSelectiveMCSIndexCW2,
                 rbgBitMap,subBandReportInfo_p, ((requiredRBCount - 1)/rat0Info_p->rbgSize) + 1);

         /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
         HD_GET_REDUCED_MCS_CW1_CW2_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndexCW1, freqSelectiveMCSIndexCW2);
#endif
         /* HD FDD Changes End */

        freqSelectiveTBSizeCW1 = determineTBSize(freqSelectiveMCSIndexCW1,
                                                 requiredRBCount,
                                                 /*4X4 DL MIMO CHG START*/
                                                 TB_MAPPED_TO_ONE_LAYER
                                                 /*4X4 DL MIMO CHG END*/); 
        freqSelectiveTBSizeCW2 = determineTBSize(freqSelectiveMCSIndexCW2,
                                                 requiredRBCount,
                                                 /*4X4 DL MIMO CHG START*/
                                                 TB_MAPPED_TO_ONE_LAYER
                                                 /*4X4 DL MIMO CHG END*/);

        freqSelectiveTBSize = freqSelectiveTBSizeCW1 + freqSelectiveTBSizeCW2;

        /*If TB size on this mcs with requiredRBCount is greater than minimum TB size then 
                 we can go for maximum TB size too if excess Pool has some RBs left. This case means successful as 
                 minimum TB size is always met*/         
        if (freqSelectiveTBSize >= minTBSize)
        {
            if (freqSelectiveTBSize >= maxTBSize)
            {
                allocatedRB = reduceRBForMaxTBSizeBothTBValid(
                                                  harqProcess_p,
                                                  allocatedRB,
                                                  maxTBSize,
                                                  freqSelectiveMCSIndexCW1,
                                                  freqSelectiveMCSIndexCW2,
                                                  freqSelectiveTBSizeCW1,
                                                  freqSelectiveTBSizeCW2);
            }
                    dlHARQTBOneInfo_p->mcsIndex = freqSelectiveMCSIndexCW1;
                    dlHARQTBTwoInfo_p->mcsIndex = freqSelectiveMCSIndexCW2;                
                    
                    harqProcess_p->assignedResourcesNB = allocatedRB;
        
            //update excess PRBs in buffer pool
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                    (requiredRBCount - harqProcess_p->assignedResourcesNB),
                    internalCellIndex);
            /* ICIC changes end */
            return MAC_SUCCESS;
        }
        else
        {
            /*Here allocation can be done only if some RBs are left in excessPool which allows TBSize to be greater
                        than or equal to minimum TB Size. If not possible then store this TBSize and found RBs as later on it will
                        required to allocate if strict subband allocation is required*/
                
            if (excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0)
            {
                getMcsIndexForCW1AndCW2(&freqSelectiveMCSIndexCW1,&freqSelectiveMCSIndexCW2,
                                        rbgBitMap,subBandReportInfo_p, 
                                        ((availableRBCount - 1)/rat0Info_p->rbgSize) + 1);
                
                excessPRBs = excessPoolRb_g[internalCellIndex][resourceRegionAllocated];
                /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                HD_GET_REDUCED_MCS_CW1_CW2_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndexCW1, freqSelectiveMCSIndexCW2);
#endif
                /* HD FDD Changes End */
                
                while (excessPRBs-- && (++allocatedRB <= availableRBCount) )
                {
                    freqSelectiveTBSizeCW1 = determineTBSize(freqSelectiveMCSIndexCW1, allocatedRB,
                                                             /*4X4 DL MIMO CHG START*/
                                                             TB_MAPPED_TO_ONE_LAYER
                                                             /*4X4 DL MIMO CHG END*/);

                    freqSelectiveTBSizeCW2 = determineTBSize(freqSelectiveMCSIndexCW2, allocatedRB,
                                                             /*4X4 DL MIMO CHG START*/
                                                             TB_MAPPED_TO_ONE_LAYER
                                                             /*4X4 DL MIMO CHG END*/);
                    
                    freqSelectiveTBSize = freqSelectiveTBSizeCW1 + freqSelectiveTBSizeCW2;
                
                    if (freqSelectiveTBSize >= minTBSize)
                    {
                        //As now minTBSize is reached hence populate HARQ fields
/*                        dlHARQTBOneInfo_p->tbSize = freqSelectiveTBSizeCW1;
                        dlHARQTBTwoInfo_p->tbSize = freqSelectiveTBSizeCW2;
  */                      
                        dlHARQTBOneInfo_p->mcsIndex = freqSelectiveMCSIndexCW1;
                        dlHARQTBTwoInfo_p->mcsIndex = freqSelectiveMCSIndexCW2;
                        harqProcess_p->assignedResourcesNB = allocatedRB;
        
                        //update excess PRBs in buffer pool
                        /* ICIC changes start */
                        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                (requiredRBCount - harqProcess_p->assignedResourcesNB),
                                internalCellIndex);
                        /* ICIC changes end */
                        return MAC_SUCCESS;       
                    }
                }
                //as allocatedRB count is greater than availableRBCount hence decrement it before storing in previousRsrcFrmt

                if (allocatedRB > availableRBCount)
				{
	                allocatedRB--;
				}

            }
            
            if(previousRsrcFrmt_p->tbSize < freqSelectiveTBSize)
            {
                previousRsrcFrmt_p->tbSize = freqSelectiveTBSize;
                previousRsrcFrmt_p->tbSizeCW1 = freqSelectiveTBSizeCW1;
                previousRsrcFrmt_p->tbSizeCW2 = freqSelectiveTBSizeCW2;
                previousRsrcFrmt_p->mcsIndexCW1 = freqSelectiveMCSIndexCW1;
                previousRsrcFrmt_p->mcsIndexCW2 = freqSelectiveMCSIndexCW2;
                previousRsrcFrmt_p->prbs = allocatedRB;
                previousRsrcFrmt_p->cqiRBGBitmap = rbgBitMap;
                previousRsrcFrmt_p->pmiIndex = harqProcess_p->pmiIndex;
            }               
        } 
    }
    else 
    {
        allocatedRB = availableRBCount;              
       /*Calling funtion getMcsIndexForCW1AndCW2 which will populate 
              * the Mcs index of CodeWord One and CodeWord Two in 
              *mcsIndexCW1 and mcsIndexCW2*/
        getMcsIndexForCW1AndCW2(&freqSelectiveMCSIndexCW1,&freqSelectiveMCSIndexCW2,
                rbgBitMap,subBandReportInfo_p, ((availableRBCount - 1)/rat0Info_p->rbgSize) + 1);
        
        /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
        HD_GET_REDUCED_MCS_CW1_CW2_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndexCW1, freqSelectiveMCSIndexCW2);
#endif
        /* HD FDD Changes End */

        /* Here although available RBs are less than required RBs but still allocation can be done as
                reported MCS can be greater than wideband MCS used by strategy hence min/max TB size can be
                achieved with available RBs*/
        freqSelectiveTBSizeCW1 = determineTBSize(freqSelectiveMCSIndexCW1, availableRBCount,
                                                 /*4X4 DL MIMO CHG START*/
                                                 TB_MAPPED_TO_ONE_LAYER
                                                 /*4X4 DL MIMO CHG END*/);

        freqSelectiveTBSizeCW2 = determineTBSize(freqSelectiveMCSIndexCW2, availableRBCount,
                                                 /*4X4 DL MIMO CHG START*/
                                                 TB_MAPPED_TO_ONE_LAYER
                                                 /*4X4 DL MIMO CHG END*/);
        
        freqSelectiveTBSize = freqSelectiveTBSizeCW1 + freqSelectiveTBSizeCW2;      
        
        if (freqSelectiveTBSize >= minTBSize)
        {
            if (freqSelectiveTBSize >= maxTBSize)
            {
                allocatedRB = reduceRBForMaxTBSizeBothTBValid(
                                                  harqProcess_p,
                                                  allocatedRB,
                                                  maxTBSize,
                                                  freqSelectiveMCSIndexCW1,
                                                  freqSelectiveMCSIndexCW2,
                                                  freqSelectiveTBSizeCW1,
                                                  freqSelectiveTBSizeCW2);            
            }
/* TB size is not set because it will be calculated again after successfull allocation */           
		   dlHARQTBOneInfo_p->mcsIndex = freqSelectiveMCSIndexCW1;
           dlHARQTBTwoInfo_p->mcsIndex = freqSelectiveMCSIndexCW2;
           harqProcess_p->assignedResourcesNB = allocatedRB;

            //update excess PRBs in buffer pool
           /* ICIC changes start */
           UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                   (requiredRBCount - harqProcess_p->assignedResourcesNB),
                   internalCellIndex);
           /* ICIC changes end */
           return MAC_SUCCESS;
        }
        else
        {
            if(previousRsrcFrmt_p->tbSize < freqSelectiveTBSize)
            {
                previousRsrcFrmt_p->tbSize = freqSelectiveTBSize;
                previousRsrcFrmt_p->tbSizeCW1 = freqSelectiveTBSizeCW1;
                previousRsrcFrmt_p->tbSizeCW2 = freqSelectiveTBSizeCW2;
                previousRsrcFrmt_p->mcsIndexCW1 = freqSelectiveMCSIndexCW1;
                previousRsrcFrmt_p->mcsIndexCW2 = freqSelectiveMCSIndexCW2;
                previousRsrcFrmt_p->prbs = allocatedRB;
                previousRsrcFrmt_p->cqiRBGBitmap = rbgBitMap;
                previousRsrcFrmt_p->pmiIndex = harqProcess_p->pmiIndex;
            }
        }  
    }

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s with Failure\n",__func__);
    harqProcess_p->pmiIndex = WIDEBAND_PMI;
    return MAC_FAILURE; 
}

/******************************************************************************
 * Function Name    : getRAT0BitMapBothTBNewTx
 * Inputs           : resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                    harqProcess_p - This Holds Pointer to DLHARQProcess structure,
 *                    rat0Info_p - RAT0 pointer used to fetch rat0bitmap and rbgSize,
 *                    dlStrategyNode_p - pointer to dlstrategy node which will be used
 *                                       for max/min TBSize,
 *                    centralRbMask - Central RB Mask,
 *                    transmissionMode - Mode of Transmission,
 *                    scheduledTick - Scheduled TTI
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None 
 * Returns          : bitMapToBeReturned - This Holds the BitMap which we will get 
 *                                         after ANDing availableRBGBitMap with the subsBand
 *                                         CQI BitMap
 * Description      : This funtion will return the BitMap which we will get after 
 *                    Anding availableRBGBitMap with the subsBand CQI BitMap in case 
 *                    of Spatial Multiplexing and when the Two TB is Valid and TB One
 *                    is New Transmission and TB Two is New Transmission.
 ******************************************************************************/
STATIC UInt32 getRAT0BitMapBothTBNewTx(                
                                ResourceAllocatorInput *resrcAllocatorInput_p,
                                DLHARQProcess  *harqProcess_p,
                                RAT0Info *rat0Info_p,
                                DLStrategyTxNode *dlStrategyNode_p,
                                /* + TM7_8 Changes Start */
                                UInt32 centralRbMask,
                                UInt8 transmissionMode,
                                /* SPR 15909 fix start */
                                tickType_t scheduledTick,
                                /* SPR 15909 fix end */
                                InternalCellIndex internalCellIndex)
                                /* - TM7_8 Changes End */
{
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);

    UInt32 requiredRBCount = resrcAllocatorInput_p->requiredRB; 
    UInt32 bitMapToBeReturned = 0;
    ResourceFormatMIMONode previousRsrcFrmt = {0};
    UInt32 pmiIndex = WIDEBAND_PMI;

    /*mark the harqProcess PMI INdex as wideband PMI, if subbandPMI is available then
      it will be overwritten*/
    harqProcess_p->pmiIndex = pmiIndex;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Start Searching in 64QAM SubBand\n");

    /*Cyclomatic Complexity Changes -starts here*/
    bitMapToBeReturned =  getRAT0BitMapFromSubBand(rat0Info_p,
            resrcAllocatorInput_p,
            harqProcess_p,
            dlStrategyNode_p,
            &previousRsrcFrmt,
            centralRbMask,
            transmissionMode,
            scheduledTick ,
            internalCellIndex);                     
    if(bitMapToBeReturned)
    {
        return bitMapToBeReturned;
    }
    /* Cyclomatic Complexity Changes -ends here */

    /*It means that we are not able to meet minTBSize asked by strategy hence if strict subband allocation
      is required then we will schedule this UE with less than minimum TB Size*/
    if (!dlStrategyNode_p->strictAllocatedFlag)
    {
        /* As of now, there is no use case for strictAllocSubBand
         * strictAllocSubBand check is commented */
        //if (resrcAllocatorInput_p->strictAllocSubBand) 
        //{
        if (previousRsrcFrmt.tbSizeCW1 > HARQ_TB_MINIMUM_SIZE_IN_BYTES  &&
                previousRsrcFrmt.tbSizeCW2 > HARQ_TB_MINIMUM_SIZE_IN_BYTES)
        {
            /*Populate the dlHARQTBOneInfo_p and dlHARQTBTwoInfo_p
             * structure with relevant fields*/
            harqProcess_p->dlHARQTBOneInfo.mcsIndex = previousRsrcFrmt.mcsIndexCW1;
            harqProcess_p->dlHARQTBOneInfo.tbSize = previousRsrcFrmt.tbSizeCW1;

            harqProcess_p->dlHARQTBTwoInfo.mcsIndex = previousRsrcFrmt.mcsIndexCW2;
            harqProcess_p->dlHARQTBTwoInfo.tbSize = previousRsrcFrmt.tbSizeCW2;

            harqProcess_p->assignedResourcesNB = previousRsrcFrmt.prbs; 

            bitMapToBeReturned = previousRsrcFrmt.cqiRBGBitmap;
            /* SPR 5148 changes start */
            //update excess PRBs in buffer pool
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resrcAllocatorInput_p->resourceRegionAllocated, 
                    (requiredRBCount - harqProcess_p->assignedResourcesNB),
                    internalCellIndex);
            /* ICIC changes end */
            /* SPR 5148 changes end */
        }  
        else if ( previousRsrcFrmt.tbSizeCW1 > HARQ_TB_MINIMUM_SIZE_IN_BYTES)
        {
            harqProcess_p->isTBTwoValid = TB_INVALID;

            harqProcess_p->dlHARQTBOneInfo.mcsIndex = previousRsrcFrmt.mcsIndexCW1;
            harqProcess_p->dlHARQTBOneInfo.tbSize = previousRsrcFrmt.tbSizeCW1;

            harqProcess_p->assignedResourcesNB = previousRsrcFrmt.prbs; 

            bitMapToBeReturned = previousRsrcFrmt.cqiRBGBitmap;
            /* SPR 5148 changes start */
            //update excess PRBs in buffer pool
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resrcAllocatorInput_p->resourceRegionAllocated, 
                    (requiredRBCount - harqProcess_p->assignedResourcesNB),
                    internalCellIndex);
            /* ICIC changes end */
            /* SPR 5148 changes end */
        }
        else if ( previousRsrcFrmt.tbSizeCW2> HARQ_TB_MINIMUM_SIZE_IN_BYTES)
        {
            harqProcess_p->dlHARQTBTwoInfo.mcsIndex = previousRsrcFrmt.mcsIndexCW2;
            harqProcess_p->dlHARQTBTwoInfo.tbSize= previousRsrcFrmt.tbSizeCW2;

            harqProcess_p->assignedResourcesNB = previousRsrcFrmt.prbs; 
            harqProcess_p->isTBOneValid = TB_INVALID;
            bitMapToBeReturned = previousRsrcFrmt.cqiRBGBitmap;
            /* SPR 5148 changes start */
            //update excess PRBs in buffer pool
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resrcAllocatorInput_p->resourceRegionAllocated, 
                    (requiredRBCount - harqProcess_p->assignedResourcesNB),
                    internalCellIndex);
            /* ICIC changes end */
            /* SPR 5148 changes end */
        } 
        harqProcess_p->pmiIndex = previousRsrcFrmt.pmiIndex;
        //}

    }

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s with Failure\n",__func__);
    return bitMapToBeReturned;
}

/******************************************************************************
 * Function Name    : getRAT0BitMapTBOneInValidTBTwoNewTx
 * Inputs           : resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                    harqProcess_p - This Holds Pointer to DLHARQProcess structure,
 *                    rat0Info_p - RAT0 pointer used to fetch rat0bitmap and rbgSize,
 *                    dlStrategyNode_p - pointer to dlstrategy node which will be used
 *                                       for max/min TBSize,
 *                    centralRbMask - Central RB Mask,
 *                    transmissionMode - Mode of Transmission,
 *                    scheduledTick - Scheduled TTI
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : bitMapToBeReturned  - This Holds the BitMap which we will get
 *                                           after Anding availableRBGBitMap with the
 *                                           subsBand CQI BitMap.
 * Description      : This funtion will return the BitMap which we will get after 
 *                    Anding availableRBGBitMap with the subsBand CQI BitMap in case
 *                    of Spatial Multiplexing and when the TB One is InValid and TB
 *                    Two is Valid with New Transmission.
 ******************************************************************************/
STATIC UInt32 getRAT0BitMapTBOneInValidTBTwoNewTx(
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        RAT0Info *rat0Info_p,
        DLStrategyTxNode *dlStrategyNode_p,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask,
        UInt8 transmissionMode,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
        /* - TM7_8 Changes End */
{
    RAT0SpatialMulCQIInfo *rat0SpatialMulCQIInfo_p = PNULL;
    DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL;
    UInt32 availableRBCount = 0;
    UInt32 bitMapToBeReturned = 0;

    UInt32 freqSelectiveMCSIndex = 0;
    UInt32 freqSelectiveTBSize = 0;
    /*SPR 19980 +*/
    UInt32 pmiIndex = WIDEBAND_PMI;
    /*SPR 19980 -*/

    SubBandReportInfo *subBandReportInfo_p = resrcAllocatorInput_p->subBandReportInfo_p;
    SubbandPMIInfo *subbandPMIInfo_p = &(subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo);
    /* ICIC changes start */
    UInt8 resourceRegionAllocated = resrcAllocatorInput_p->resourceRegionAllocated;
    UInt32 availableRBGBitMap = rat0Info_p->rat0BitMap[CC_USER] & 
        rat0Info_p->rat0BitMap[resourceRegionAllocated];
    /* ICIC changes end */
           
    /* + TM7_8 Changes Start */
    /* SPR 21631 Start */
        if(CHECK_SF_MAPPED_ON_BCH_PSS_SSS(scheduledTick,transmissionMode))
            /* SPR 21631 End */
    {
        /*block central 6 RBs in temp variable*/
        availableRBGBitMap = availableRBGBitMap &(~centralRbMask);
    }
    /* - TM7_8 Changes End */

    UInt32 requiredRBCount = resrcAllocatorInput_p->requiredRB; 

    UInt32 cntModScheme = MOD_SCHEME_CNT;
    
    UInt32 modSchemeAndAvailableBitmap[MOD_SCHEME_CNT] = {0};
    
    UInt32 minTBSize = dlStrategyNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyNode_p->maxTBSize; 

    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *ueContext_p = PNULL;
    dlUEContextInfo_p = &dlUECtxInfoArr_g[dlStrategyNode_p ->ueIndex];
    ueContext_p = dlUEContextInfo_p->dlUEContext_p;
    /* HD FDD Changes Start */ 
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
#endif
    /* HD FDD Changes End */ 
 
    if (maxTBSize < minTBSize)
    {
        lteWarning("[RAT0 Spatial TB1 Invalid TB2 New] Wrong Max TBSize calculated\
                     by DL Strategy minTBSize [%d] maxTBSize [%d]\n", minTBSize, maxTBSize);
        maxTBSize = minTBSize;
    }       

    ResourceFormatNode previousRsrcFrmt = {0};
    
    dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);

    rat0SpatialMulCQIInfo_p = 
        &subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.rat0SpatialMulCQIInfo;

    modSchemeAndAvailableBitmap[0] = 
        rat0SpatialMulCQIInfo_p->rat0QPSKCQIBitMapCW2 & availableRBGBitMap;
    modSchemeAndAvailableBitmap[1] = 
        rat0SpatialMulCQIInfo_p->rat016QAMCQIBitMapCW2 & availableRBGBitMap;
    modSchemeAndAvailableBitmap[2] = 
        rat0SpatialMulCQIInfo_p->rat064QAMCQIBitMapCW2 & availableRBGBitMap;   

    while (cntModScheme--)
    {
        if ((bitMapToBeReturned = modSchemeAndAvailableBitmap[cntModScheme]))
        {
            if (resrcAllocatorInput_p->subbandPMI)
            {
                if(2 == ueContext_p->dlMIMOInfo.dlRi)
                {
                    subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank1[0] =
                       subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank2[0]; 
                    subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank1[2] =
                       subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank2[1]; 
                    subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank1[1] = 0;
                    subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank1[3] = 0;
                }
                pmiIndex = selectRAT0SubbandPMIIndexRank1(
                                                 bitMapToBeReturned, 
                                                 &subbandPMIInfo_p->rat0PmiInfo,
                                                 &bitMapToBeReturned,
                                                 internalCellIndex
                                                 );
                harqProcess_p->pmiIndex = pmiIndex;                
            }   
            
            /*Get the count of number of Available RBs*/
            availableRBCount = getCount(bitMapToBeReturned) * 
                rat0Info_p->rbgSize;
            if (bitMapToBeReturned & ((UInt32) 1 << (rat0Info_p->rbgCount - 1)))
            {
                availableRBCount = availableRBCount - rat0Info_p->rbgSize +
                    rat0Info_p->rbgIndexInfo[rat0Info_p->rbgCount - 1].numOfRBs;     
            }
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "availableRBCount = %u\n",availableRBCount);  
            if (availableRBCount >= requiredRBCount)
            {
                /*Get Mcs Index of CodeWord Two*/
                getMcsIndexForCW2(&freqSelectiveMCSIndex, bitMapToBeReturned,
                    subBandReportInfo_p, ((requiredRBCount - 1)/rat0Info_p->rbgSize) + 1);
                /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndex);
#endif
                /* HD FDD Changes End */
                freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex,
                                                      requiredRBCount,
                                                      /*4X4 DL MIMO CHG START*/
                                                      TB_MAPPED_TO_ONE_LAYER
                                                      /*4X4 DL MIMO CHG END*/);
                /*If TB size on this mcs with requiredRBCount is greater than minimum TB size then 
                               we can go for maximum TB size too if excess Pool has some RBs left. This case means successful as 
                               minimum TB size is always met*/         
                if (freqSelectiveTBSize >= minTBSize)
                {
                    if (freqSelectiveTBSize >= maxTBSize)
                    {
                        freqSelectiveTBSize = maxTBSize;
                        /* SPR 5999 changes start */
                        harqProcess_p->assignedResourcesNB = DETERMINE_RBs_DL(freqSelectiveMCSIndex,
                                                                              freqSelectiveTBSize,
                                                                              internalCellIndex);                    
                        /* SPR 5999 changes end */
                    }
                    else
                    {
                        harqProcess_p->assignedResourcesNB = requiredRBCount;
                    }
                    
                    dlHARQTBTwoInfo_p->mcsIndex = freqSelectiveMCSIndex;
                    dlHARQTBTwoInfo_p->tbSize = freqSelectiveTBSize;

                    //update excess PRBs in buffer pool
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            (requiredRBCount - harqProcess_p->assignedResourcesNB),
                            internalCellIndex);
                    /* ICIC changes end */
                    return bitMapToBeReturned;
                }
                else
                {
                    /*Here allocation can be done only if some RBs are left in excessPool which allows TBSize to be greater
                                      than or equal to minimum TB Size. If not possible then store this TBSize and found RBs as later on it will
                                      required to allocate if strict subband allocation is required*/

                    /*Get Mcs Index of CodeWord Two*/
                    getMcsIndexForCW2(&freqSelectiveMCSIndex, bitMapToBeReturned,
                        subBandReportInfo_p, ((availableRBCount- 1)/rat0Info_p->rbgSize) + 1);                  
 
                    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                    HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndex);
#endif
                    /* HD FDD Changes End */
                       
                    UInt32 requiredRBsFromPool = 0;
                    if (excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0)
                    {
                        requiredRBsFromPool = allocatePRBsUsingExcessPool(freqSelectiveMCSIndex,
                                                                          availableRBCount,
                                                                          minTBSize,
                                                                          requiredRBCount,
                                                                          resourceRegionAllocated,
                                                                          internalCellIndex);
                        
                        freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex, 
                                                              requiredRBCount+ requiredRBsFromPool,
                                                              /*4X4 DL MIMO CHG START*/
                                                              TB_MAPPED_TO_ONE_LAYER
                                                              /*4X4 DL MIMO CHG END*/);  
                        if (freqSelectiveTBSize >= minTBSize)
                        {
                            dlHARQTBTwoInfo_p->mcsIndex = freqSelectiveMCSIndex;
                            harqProcess_p->assignedResourcesNB = requiredRBCount + requiredRBsFromPool; 
                            dlHARQTBTwoInfo_p->tbSize = freqSelectiveTBSize;

                            //update excess PRBs in buffer pool
                            /* ICIC changes start */
                            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                    (requiredRBCount - harqProcess_p->assignedResourcesNB),
                                    internalCellIndex);
                            /* ICIC changes end */
                    
                            return bitMapToBeReturned;                   
                        }
                    }
                    
                    if(previousRsrcFrmt.tbSize < freqSelectiveTBSize)
                    {
                        previousRsrcFrmt.tbSize = freqSelectiveTBSize;
                        previousRsrcFrmt.mcsIndex = freqSelectiveMCSIndex;
                        previousRsrcFrmt.prbs = requiredRBCount+ requiredRBsFromPool;
                        previousRsrcFrmt.cqiRBGBitmap = bitMapToBeReturned;
                        previousRsrcFrmt.pmiIndex = pmiIndex;
                    }               
                }           
            }
            else
            {
                /*Get Mcs Index of CodeWord Two*/
                getMcsIndexForCW2(&freqSelectiveMCSIndex, bitMapToBeReturned,
                    subBandReportInfo_p, ((availableRBCount - 1)/rat0Info_p->rbgSize) + 1);
                /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndex);
#endif
                /* HD FDD Changes End */
   
                /* Here although available RBs are less than required RBs but still allocation can be done as
                        reported MCS can be greater than wideband MCS used by strategy hence min/max TB size can be
                        achieved with available RBs*/
                freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex,
                                                      availableRBCount,
                                                      /*4X4 DL MIMO CHG START*/
                                                      TB_MAPPED_TO_ONE_LAYER
                                                      /*4X4 DL MIMO CHG END*/);  
                if (freqSelectiveTBSize >= minTBSize)
                {
                    if (freqSelectiveTBSize > maxTBSize)
                    {
                        freqSelectiveTBSize = maxTBSize;
                    }
                    
                    dlHARQTBTwoInfo_p->mcsIndex = freqSelectiveMCSIndex;                
                    /* SPR 5999 changes start */
                    harqProcess_p->assignedResourcesNB = DETERMINE_RBs_DL(freqSelectiveMCSIndex,
                                                                          freqSelectiveTBSize,
                                                                          internalCellIndex);
                    /* SPR 5999 changes end */
                    dlHARQTBTwoInfo_p->tbSize = freqSelectiveTBSize;
                    //update excess PRBs in buffer pool
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            (requiredRBCount - harqProcess_p->assignedResourcesNB),
                            internalCellIndex);
                    /* ICIC changes end */
                    return bitMapToBeReturned;
                }
                else
                {
                    if(previousRsrcFrmt.tbSize < freqSelectiveTBSize)
                    {
                        previousRsrcFrmt.tbSize = freqSelectiveTBSize;
                        previousRsrcFrmt.mcsIndex = freqSelectiveMCSIndex;
                        previousRsrcFrmt.prbs = availableRBCount;
                        previousRsrcFrmt.cqiRBGBitmap = bitMapToBeReturned;
                        previousRsrcFrmt.pmiIndex = pmiIndex;
                    }
                }        
            }
        }
    }

    bitMapToBeReturned = 0;
    /*It means that we are not able to meet minTBSize asked by strategy hence if strict subband allocation
           is required then we will schedule this UE with less than minimum TB Size*/
    if (!dlStrategyNode_p->strictAllocatedFlag)
    {
        /* As of now, there is no use case for strictAllocSubBand
         * strictAllocSubBand check is commented */
        //if (resrcAllocatorInput_p->strictAllocSubBand)
        //{
        if(previousRsrcFrmt.tbSize >= HARQ_TB_MINIMUM_SIZE_IN_BYTES)
        {
            harqProcess_p->assignedResourcesNB = previousRsrcFrmt.prbs;
            dlHARQTBTwoInfo_p->mcsIndex = previousRsrcFrmt.mcsIndex;    
            dlHARQTBTwoInfo_p->tbSize = previousRsrcFrmt.tbSize;
            bitMapToBeReturned = previousRsrcFrmt.cqiRBGBitmap;
            harqProcess_p->pmiIndex = previousRsrcFrmt.pmiIndex;

            //update excess PRBs in buffer pool
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                    (requiredRBCount - harqProcess_p->assignedResourcesNB),
                    internalCellIndex);
            /* ICIC changes end */
        }
        //}
       
    }

  
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    return bitMapToBeReturned;
}

/******************************************************************************
 * Function Name    : getRAT0BitMapTBOneNewTxTBTwoInValid
 * Inputs           : resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                    harqProcess_p - This Holds Pointer to DLHARQProcess structure,
 *                    rat0Info_p - RAT0 pointer used to fetch rat0bitmap and rbgSize,
 *                    dlStrategyNode_p - pointer to dlstrategy node which will be used
 *                                       for max/min TBSize,
 *                    centralRbMask - Central RB Mask,
 *                    transmissionMode - Mode of Transmission,
 *                    scheduledTick - Scheduled TTI
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : bitMapToBeReturned  - This Holds the BitMap which we will get
 *                                           after Anding availableRBGBitMap with the
 *                                           subsBand CQI BitMap.
 * Description      : This funtion will return the BitMap which we will get after 
 *                    Anding availableRBGBitMap with the subsBand CQI BitMap in case
 *                    of Spatial Multiplexing and when the TB Two is InValid and TB
 *                    One is Valid with New Transmission.
 ******************************************************************************/
STATIC UInt32 getRAT0BitMapTBOneNewTxTBTwoInValid(
                        ResourceAllocatorInput *resrcAllocatorInput_p,
                        DLHARQProcess  *harqProcess_p,
                        RAT0Info *rat0Info_p,
                        DLStrategyTxNode *dlStrategyNode_p,
                        /* + TM7_8 Changes Start */
                        UInt32 centralRbMask,
                        UInt8 transmissionMode,
                        /* SPR 15909 fix start */
                        tickType_t scheduledTick,
                        /* SPR 15909 fix end */
                        InternalCellIndex internalCellIndex)
                        /* - TM7_8 Changes End */

{
    RAT0SpatialMulCQIInfo *rat0SpatialMulCQIInfo_p = PNULL;
    DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
    UInt32 availableRBCount = 0;
    UInt32 bitMapToBeReturned = 0;
    /*SPR 19980 +*/
    UInt32 pmiIndex = WIDEBAND_PMI;
    /*SPR 19980 -*/
 
    UInt32 freqSelectiveMCSIndex = 0;
    UInt32 freqSelectiveTBSize = 0;

    SubBandReportInfo *subBandReportInfo_p = resrcAllocatorInput_p->subBandReportInfo_p;
    SubbandPMIInfo *subbandPMIInfo_p = &(subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo);
    
    /* ICIC changes start */
    UInt8 resourceRegionAllocated = resrcAllocatorInput_p->resourceRegionAllocated;
    UInt32 availableRBGBitMap = rat0Info_p->rat0BitMap[CC_USER] & 
        rat0Info_p->rat0BitMap[resourceRegionAllocated];
    /* ICIC changes end */
       
    /* + TM7_8 Changes Start */
/* SPR 21631 Start */
    if(CHECK_SF_MAPPED_ON_BCH_PSS_SSS(scheduledTick,transmissionMode))
        /* SPR 21631 End */
    {
        /*block central 6 RBs in temp variable*/
        availableRBGBitMap = availableRBGBitMap &(~centralRbMask);
    }
    /* - TM7_8 Changes End */

    UInt32 requiredRBCount = resrcAllocatorInput_p->requiredRB; 

    UInt32 cntModScheme = MOD_SCHEME_CNT;
    
    UInt32 modSchemeAndAvailableBitmap[MOD_SCHEME_CNT] = {0};
    
    UInt32 minTBSize = dlStrategyNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyNode_p->maxTBSize;
    
    DLUEContextInfo *dlUEContextInfo_p = PNULL;        
    DLUEContext *ueContext_p = PNULL;
    dlUEContextInfo_p = &dlUECtxInfoArr_g[dlStrategyNode_p ->ueIndex];
    ueContext_p = dlUEContextInfo_p->dlUEContext_p; 
    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
#endif
    /* HD FDD Changes End */
    if (maxTBSize < minTBSize)
    {
        lteWarning("[RAT0 Spatial TB1 New TB2 Invalid] Wrong Max TBSize calculated\
                     by DL Strategy minTBSize [%d] maxTBSize [%d]\n", minTBSize, maxTBSize);
        maxTBSize = minTBSize;
    }       
    

    ResourceFormatNode previousRsrcFrmt = {0};
    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    
    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    
    rat0SpatialMulCQIInfo_p = 
        &subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.rat0SpatialMulCQIInfo;

    /*store 64QAM and available bitmap at MOD_SCHEME_CNT in order to get it first using while (i--) */
    modSchemeAndAvailableBitmap[0] = 
        rat0SpatialMulCQIInfo_p->rat0QPSKCQIBitMapCW1& availableRBGBitMap;
    modSchemeAndAvailableBitmap[1] = 
        rat0SpatialMulCQIInfo_p->rat016QAMCQIBitMapCW1 & availableRBGBitMap;
    modSchemeAndAvailableBitmap[2] = 
        rat0SpatialMulCQIInfo_p->rat064QAMCQIBitMapCW1 & availableRBGBitMap; 

    while (cntModScheme--)
    {
        if ((bitMapToBeReturned = modSchemeAndAvailableBitmap[cntModScheme]))
        {
            if (resrcAllocatorInput_p->subbandPMI)
            {
                if(2 == ueContext_p->dlMIMOInfo.dlRi)
                {
                    subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank1[0] =
                       subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank2[0]; 
                    subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank1[2] =
                       subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank2[1]; 
                    subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank1[1] = 0;
                    subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank1[3] = 0;
                }
                pmiIndex = selectRAT0SubbandPMIIndexRank1(
                                                 bitMapToBeReturned, 
                                                 &subbandPMIInfo_p->rat0PmiInfo,
                                                 &bitMapToBeReturned,
                                                 internalCellIndex
                                                 );
                harqProcess_p->pmiIndex = pmiIndex;                
            }
            
            /*Get the count of number of Available RBs*/
            availableRBCount = getCount(bitMapToBeReturned) * rat0Info_p->rbgSize;
            if (bitMapToBeReturned & ((UInt32) 1 << (rat0Info_p->rbgCount - 1)))
            {
                availableRBCount = availableRBCount - rat0Info_p->rbgSize +
                    rat0Info_p->rbgIndexInfo[rat0Info_p->rbgCount - 1].numOfRBs;  
            }
            
            if (availableRBCount >= requiredRBCount)
            {
                /*Get Mcs Index of CodeWord One*/
                getMcsIndexForCW1(&freqSelectiveMCSIndex, bitMapToBeReturned,
                    subBandReportInfo_p, ((requiredRBCount - 1)/rat0Info_p->rbgSize) + 1);

                /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndex);
#endif
                /* HD FDD Changes End */

                freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex,
                                                      requiredRBCount,
                                                      /*4X4 DL MIMO CHG START*/
                                                       TB_MAPPED_TO_ONE_LAYER
                                                       /*4X4 DL MIMO CHG END*/);
                /*If TB size on this mcs with requiredRBCount is greater than minimum TB size then 
                               we can go for maximum TB size too if excess Pool has some RBs left. This case means successful as 
                               minimum TB size is always met*/         
                if (freqSelectiveTBSize >= minTBSize)
                {
                    if (freqSelectiveTBSize >= maxTBSize)
                    {
                        freqSelectiveTBSize = maxTBSize;
                        /* SPR 5999 changes start */
                        harqProcess_p->assignedResourcesNB = DETERMINE_RBs_DL(freqSelectiveMCSIndex,
                                                                              freqSelectiveTBSize,
                                                                              internalCellIndex);                    
                        /* SPR 5999 changes end */
                    }
                    else
                    {
                        harqProcess_p->assignedResourcesNB = requiredRBCount;
                    }
                    
                    dlHARQTBOneInfo_p->mcsIndex = freqSelectiveMCSIndex;
                    dlHARQTBOneInfo_p->tbSize = freqSelectiveTBSize;

                    //update excess PRBs in buffer pool
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            (requiredRBCount - harqProcess_p->assignedResourcesNB),internalCellIndex);
                    /* ICIC changes end */
                    return bitMapToBeReturned;
                }
                else
                {
                    /*Here allocation can be done only if some RBs are left in excessPool which allows TBSize to be greater
                                     than or equal to minimum TB Size. If not possible then store this TBSize and found RBs as later on it will
                                     required to allocate if strict subband allocation is required*/

                    
                    UInt32 requiredRBsFromPool = 0;
                    if (excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0)
                    {

                    /*Get Mcs Index of CodeWord One*/
                    getMcsIndexForCW1(&freqSelectiveMCSIndex, bitMapToBeReturned,
                        subBandReportInfo_p, ((availableRBCount- 1)/rat0Info_p->rbgSize) + 1);

                    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                    HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndex);
#endif
                    /* HD FDD Changes End */
                        requiredRBsFromPool = allocatePRBsUsingExcessPool(freqSelectiveMCSIndex,
                                                                          availableRBCount,
                                                                          minTBSize,
                                                                          requiredRBCount,
                                                                          resourceRegionAllocated,
                                                                          internalCellIndex);
                        
                        freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex, 
                                                              requiredRBCount+ requiredRBsFromPool,
                                                              /*4X4 DL MIMO CHG START*/
                                                              TB_MAPPED_TO_ONE_LAYER
                                                              /*4X4 DL MIMO CHG END*/);

                        if (freqSelectiveTBSize >= minTBSize)
                        {
                            dlHARQTBOneInfo_p->mcsIndex = freqSelectiveMCSIndex;
                            harqProcess_p->assignedResourcesNB = requiredRBCount + requiredRBsFromPool; 
                            dlHARQTBOneInfo_p->tbSize = freqSelectiveTBSize;

                            //update excess PRBs in buffer pool
                            /* ICIC changes start */
                            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                    (requiredRBCount - harqProcess_p->assignedResourcesNB),internalCellIndex);
                            /* ICIC changes end */
                    
                            return bitMapToBeReturned;                   
                        }
                    }
                    
                    if(previousRsrcFrmt.tbSize < freqSelectiveTBSize)
                    {
                        previousRsrcFrmt.tbSize = freqSelectiveTBSize;
                        previousRsrcFrmt.mcsIndex = freqSelectiveMCSIndex;
                        previousRsrcFrmt.prbs = requiredRBCount+ requiredRBsFromPool;
                        previousRsrcFrmt.cqiRBGBitmap = bitMapToBeReturned;
                        previousRsrcFrmt.pmiIndex = pmiIndex;
                    }               
                }           
            }
            else
            {
                /*Get Mcs Index of CodeWord One*/
                getMcsIndexForCW1(&freqSelectiveMCSIndex, bitMapToBeReturned,
                    subBandReportInfo_p, ((availableRBCount - 1)/rat0Info_p->rbgSize) + 1);
                
                /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndex);
#endif
                /* HD FDD Changes End */
                /* Here although available RBs are less than required RBs but still allocation can be done as
                        reported MCS can be greater than wideband MCS used by strategy hence min/max TB size can be
                        achieved with available RBs*/
                freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex,
                                                      availableRBCount,
                                                      /*4X4 DL MIMO CHG START*/
                                                      TB_MAPPED_TO_ONE_LAYER
                                                      /*4X4 DL MIMO CHG END*/);

                if (freqSelectiveTBSize >= minTBSize)
                {
                    if (freqSelectiveTBSize > maxTBSize)
                    {
                        freqSelectiveTBSize = maxTBSize;
                    }
                    
                    dlHARQTBOneInfo_p->mcsIndex = freqSelectiveMCSIndex;                
                    /* SPR 5999 changes start */
                    harqProcess_p->assignedResourcesNB = DETERMINE_RBs_DL(freqSelectiveMCSIndex,
                                                                          freqSelectiveTBSize,
                                                                          internalCellIndex);
                    /* SPR 5999 changes end */
                    dlHARQTBOneInfo_p->tbSize = freqSelectiveTBSize;
                    //update excess PRBs in buffer pool
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            (requiredRBCount - harqProcess_p->assignedResourcesNB), internalCellIndex);
                    /* ICIC changes end */
                    return bitMapToBeReturned;
                }
                else
                {
                    if(previousRsrcFrmt.tbSize < freqSelectiveTBSize)
                    {
                        previousRsrcFrmt.tbSize = freqSelectiveTBSize;
                        previousRsrcFrmt.mcsIndex = freqSelectiveMCSIndex;
                        previousRsrcFrmt.prbs = availableRBCount;
                        previousRsrcFrmt.cqiRBGBitmap = bitMapToBeReturned;
                        previousRsrcFrmt.pmiIndex = pmiIndex;                        
                    }
                }  
            }
        }
    }
     bitMapToBeReturned = 0;
    /*It means that we are not able to meet minTBSize asked by strategy hence if strict subband allocation
           is required then we will schedule this UE with less than minimum TB Size*/
    if (!dlStrategyNode_p->strictAllocatedFlag)
    {
        /* As of now, there is no use case for strictAllocSubBand
         * strictAllocSubBand check is commented */
        //if (resrcAllocatorInput_p->strictAllocSubBand) 
        //{
        if(previousRsrcFrmt.tbSize >= HARQ_TB_MINIMUM_SIZE_IN_BYTES)
        {
            harqProcess_p->assignedResourcesNB = previousRsrcFrmt.prbs;
            dlHARQTBOneInfo_p->mcsIndex = previousRsrcFrmt.mcsIndex;    
            dlHARQTBOneInfo_p->tbSize = previousRsrcFrmt.tbSize;
            bitMapToBeReturned = previousRsrcFrmt.cqiRBGBitmap;
            harqProcess_p->pmiIndex = previousRsrcFrmt.pmiIndex;

            //update excess PRBs in buffer pool
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                    (requiredRBCount - harqProcess_p->assignedResourcesNB), internalCellIndex);
            /* ICIC changes end */
        }
        //}
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s with Failure\n",__func__);
    return bitMapToBeReturned;
}

/******************************************************************************
 * Function Name    : getRAT0BitMapSpatialMux
 * Inputs           : harqProcess_p - This Holds Pointer to DLHARQProcess structure,
 *                    resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                    rat0Info_p - RAT0 pointer used to fetch rat0bitmap and rbgSize,
 *                    dlStrategyNode_p - pointer to dlstrategy node which will be used
 *                                       for max/min TBSize,
 *                    centralRbMask - Central RB Mask,
 *                    transmissionMode - Mode of Transmission,
 *                    scheduledTick - Scheduled TTI
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : bitMapToBeReturned  - This Holds the BitMap which we will get
 *                                           after Anding availableRBGBitMap with the
 *                                           subsBand CQI BitMap.
 * Description      : This funtion will call relavant funtions depending upon TB
 *                    Validity and also Based on  whether TB is Re-Transmission or TB
 *                    is New Transmission.
 ******************************************************************************/
STATIC  UInt32 getRAT0BitMapSpatialMux( 
        DLHARQProcess  *harqProcess_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,        
        RAT0Info *rat0Info_p,
        DLStrategyTxNode *dlStrategyTxNode_p,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask,
        UInt8 transmissionMode,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
        /* - TM7_8 Changes End */
{
/*    DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL; 
    DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL; TDD Warning Fix */

    UInt32 bitMapToBeReturned = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
/*    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo; TDD Warning Fix */
    
    if ( TB_VALID == harqProcess_p->isTBOneValid && 
            TB_VALID == harqProcess_p->isTBTwoValid )
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "Both TB NEW_TX Case\n");
        bitMapToBeReturned = getRAT0BitMapBothTBNewTx(
                                resrcAllocatorInput_p,
                                harqProcess_p, 
                                rat0Info_p,
                                dlStrategyTxNode_p,     
                                /* + TM7_8 Changes Start */
                                centralRbMask,
                                transmissionMode,
                                scheduledTick,
                                internalCellIndex);
                                /* - TM7_8 Changes End */
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "Leaving funtion = %s\n",__func__);
        return bitMapToBeReturned;
    }       
    else if ( TB_VALID == harqProcess_p->isTBOneValid )
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "TB One is New TX Case\n");
        bitMapToBeReturned = getRAT0BitMapTBOneNewTxTBTwoInValid(
                                resrcAllocatorInput_p,
                                harqProcess_p,
                                rat0Info_p,
                                dlStrategyTxNode_p,
                                /* + TM7_8 Changes Start */
                                centralRbMask,
                                transmissionMode,
                                scheduledTick,
                                internalCellIndex);
                                /* - TM7_8 Changes End */
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "Leaving funtion = %s\n",__func__);
        return bitMapToBeReturned;
    } 
    else if ( TB_VALID == harqProcess_p->isTBTwoValid )
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "TB Two is New TX Case\n");
        bitMapToBeReturned = getRAT0BitMapTBOneInValidTBTwoNewTx(
                                resrcAllocatorInput_p,
                                harqProcess_p,                                
                                rat0Info_p,
                                dlStrategyTxNode_p, 
                                /* + TM7_8 Changes Start */
                                centralRbMask,
                                transmissionMode,
                                scheduledTick,
                                internalCellIndex);
                                /* - TM7_8 Changes End */
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "Leaving funtion = %s\n",__func__);
        return bitMapToBeReturned;
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);

    return bitMapToBeReturned;
}

/* + coverity 42178 */
#ifdef MAC_AUT_TEST
/******************************************************************************
 * Function Name    : populateDLHARQInfoSpatialMul
 * Inputs           : resrcAllocatorInput_p - This holds Pointer to 
 *                                            ResourceAllocatorInput structure,
 *                    dlHARQTBOneInfo_p -  This holds pointer to DLHARQTBInfo structure,
 *                    dlHARQTBTwoInfo_p -  This holds pointer to DLHARQTBInfo structure,
 *                    dlUEContext_p -  This holds pointer to DLUEContext structure,
 *                    availableRBCount - This holds number of RBs count which need to 
 *                                       be allocated,
 *                    harqProcess_p - This stores the pointer to DLHARQProcess structure
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : resrcAllocatorOutput_p - This holds pointer to 
 *                                             ResourceAllocatorOutput structure 
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will  populate the relevant information 
 *                    in DLHARQTBInfo structure based on whether TB is
 *                    New Transmitted TB or Re-Transmitted TB.
 ******************************************************************************/
STATIC  MacRetType populateDLHARQInfoSpatialMul(
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQTBInfo *dlHARQTBOneInfo_p,
        DLHARQTBInfo *dlHARQTBTwoInfo_p,
        DLUEContext *dlUEContext_p,
        UInt32 availableRBCount,
        DLHARQProcess *harqProcess_p,
        InternalCellIndex internalCellIndex)
{
    UInt32 mcsIndexCW1 = 0;
    UInt32 mcsIndexCW2 = 0;
    UInt32 tbSizeCW1 = 0;
    UInt32 tbSizeCW2 = 0;
    UInt32 allocatedRB = availableRBCount;
    (void)resrcAllocatorOutput_p;/*SPR 18490 +-*/
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);   

    if ( harqProcess_p->isTBOneValid && harqProcess_p->isTBTwoValid)
    {
        /*Get the MCS and TB Size of CodeWord One and CodeWord Two*/
        mcsIndexCW1 = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordOne;
        mcsIndexCW2 = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
        if ( NEW_TX == dlHARQTBOneInfo_p->txIndicator &&
                NEW_TX == dlHARQTBTwoInfo_p->txIndicator)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "Both TB New TX \n");
            if ( resrcAllocatorInput_p->requiredRB <= availableRBCount )
            {
                allocatedRB = resrcAllocatorInput_p->requiredRB;
            }
            else
            {
                allocatedRB = availableRBCount;
            }
            harqProcess_p->assignedResourcesNB = allocatedRB;
            tbSizeCW1 = determineTBSize(mcsIndexCW1, allocatedRB,
                                        /*4X4 DL MIMO CHG START*/
                                        TB_MAPPED_TO_ONE_LAYER
                                        /*4X4 DL MIMO CHG END*/);
   
            tbSizeCW2 = determineTBSize(mcsIndexCW2, allocatedRB,
                                        /*4X4 DL MIMO CHG START*/
                                        TB_MAPPED_TO_ONE_LAYER
                                        /*4X4 DL MIMO CHG END*/);

            /* Populate the dlHARQTBOneInfo_p and dlHARQTBTwoInfo_p structures
             * with Relevant Information*/
            if (tbSizeCW1 > HARQ_TB_MINIMUM_SIZE_IN_BYTES && 
                    tbSizeCW2 > HARQ_TB_MINIMUM_SIZE_IN_BYTES)
            {
                dlHARQTBOneInfo_p->mcsIndex = mcsIndexCW1;
                dlHARQTBOneInfo_p->tbSize = tbSizeCW1;
                dlHARQTBTwoInfo_p->mcsIndex = mcsIndexCW2;
                dlHARQTBTwoInfo_p->tbSize = tbSizeCW2;  
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion = %s with Success\n",__func__);
                return MAC_SUCCESS;
            }
            else if ( tbSizeCW1 > HARQ_TB_MINIMUM_SIZE_IN_BYTES )
            {
                dlHARQTBOneInfo_p->mcsIndex = mcsIndexCW1;
                dlHARQTBOneInfo_p->tbSize = tbSizeCW1;
                harqProcess_p->isTBTwoValid = TB_INVALID;
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "Leaving funtion = %s with Success\n",__func__);
                return MAC_SUCCESS;
            }
            else if (tbSizeCW2 > HARQ_TB_MINIMUM_SIZE_IN_BYTES)
            {
                dlHARQTBTwoInfo_p->mcsIndex = mcsIndexCW2;
                dlHARQTBTwoInfo_p->tbSize = tbSizeCW2;  
                if (INVALID_TA_DRX_MSG  == dlHARQTBOneInfo_p->taDrxFlag ) 
                {
                    harqProcess_p->isTBOneValid = TB_INVALID;                               
                }
                else
                {
                    dlHARQTBOneInfo_p->mcsIndex = mcsIndexCW1;
                    dlHARQTBOneInfo_p->tbSize = tbSizeCW1; 
                }                
                return MAC_SUCCESS;
            }        
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "Leaving funtion = %s with Failure\n",__func__);
            return MAC_FAILURE;

        }
        else if ( RE_TX ==dlHARQTBTwoInfo_p->txIndicator &&
                RE_TX ==dlHARQTBOneInfo_p->txIndicator  )
        {
            UInt32 requiredRBTBTwo = 0;
            UInt32 requiredRBTBOne = 0;
            /*Get required Number of RBs for CodeWord Two which can accommodate
             * TB of size dlHARQTBTwoInfo_p->tbSize*/
            tbSizeCW2 = dlHARQTBTwoInfo_p->tbSize;
            /* SPR 5999 changes start */
            requiredRBTBTwo = DETERMINE_RBs_DL(mcsIndexCW2, tbSizeCW2, internalCellIndex);
            /* SPR 5999 changes end */
            if ( requiredRBTBTwo && ( availableRBCount >=  requiredRBTBTwo ) )
            {
                /*Get required Number of RBs for CodeWord One which can 
                * accommodateTB of size dlHARQTBOneInfo_p->tbSize*/
                tbSizeCW1 = dlHARQTBOneInfo_p->tbSize;
                /* SPR 5999 changes start */
                requiredRBTBOne= DETERMINE_RBs_DL(mcsIndexCW1, tbSizeCW1, internalCellIndex);
                /* SPR 5999 changes end */
                if ( requiredRBTBOne && ( availableRBCount >=  
                            requiredRBTBOne ) )
                {
                    /* Populate the dlHARQTBOneInfo_p and dlHARQTBTwoInfo_p with 
                     * relevant information*/
                    dlHARQTBOneInfo_p->mcsIndex = mcsIndexCW1; 
                    dlHARQTBOneInfo_p->retransmissionTBSize = tbSizeCW1;
                    dlHARQTBTwoInfo_p->mcsIndex = mcsIndexCW2;
                    dlHARQTBTwoInfo_p->retransmissionTBSize = tbSizeCW2;
                    harqProcess_p->assignedResourcesNB = (requiredRBTBOne                     
                            > requiredRBTBTwo )?
                        requiredRBTBOne: requiredRBTBTwo;
                    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                            "Leaving funtion = %s with Success\n",
                            __func__);
                    return MAC_SUCCESS;
                }
            } 
        }
        else if ( RE_TX ==dlHARQTBTwoInfo_p->txIndicator )
        {
            UInt32 tbSize = 0;
            UInt32 requiredRB = 0;
            /*Get the number of RBs which can accomodate the TB having size
             *dlHARQTBTwoInfo_p->tbSize*/
            tbSize = dlHARQTBTwoInfo_p->tbSize;
            /* SPR 5999 changes start */
            requiredRB = DETERMINE_RBs_DL(mcsIndexCW2, tbSize, internalCellIndex);
            /* SPR 5999 changes end */
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "requiredRB = %u\n",requiredRB);
            if ( requiredRB && (availableRBCount >= requiredRB) )
            {
                dlHARQTBTwoInfo_p->mcsIndex = mcsIndexCW2;          
                dlHARQTBTwoInfo_p->retransmissionTBSize = tbSize;
                harqProcess_p->assignedResourcesNB = requiredRB;
                tbSize  = determineTBSize(mcsIndexCW1, requiredRB,
                                           /*4X4 DL MIMO CHG START*/
                                           TB_MAPPED_TO_ONE_LAYER
                                           /*4X4 DL MIMO CHG END*/);

                if ( tbSize > HARQ_TB_MINIMUM_SIZE_IN_BYTES )
                {    
                    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion = %s\n",__func__);
                    dlHARQTBOneInfo_p->mcsIndex = mcsIndexCW1;
                    dlHARQTBOneInfo_p->tbSize = tbSize;
                    return MAC_SUCCESS;
                }  
                else
                {
                    if (INVALID_TA_DRX_MSG  == dlHARQTBOneInfo_p->taDrxFlag ) 
                    {
                        harqProcess_p->isTBOneValid = TB_INVALID;
                        return MAC_SUCCESS;
                    }
                    dlHARQTBOneInfo_p->mcsIndex = mcsIndexCW1;
                    dlHARQTBOneInfo_p->tbSize = determineTBSize(mcsIndexCW1, 
                        requiredRB,
                        /*4X4 DL MIMO CHG START*/
                        TB_MAPPED_TO_ONE_LAYER
                        /*4X4 DL MIMO CHG END*/);

                    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion = %s with Success\n",__func__);
                    return  MAC_SUCCESS;
                }
            }
        }
        else
        {
            UInt32 tbSize = 0;
            UInt32 requiredRB = 0;
            /*Get the number of RBs which can accomodate the TB having size
             *dlHARQTBOneInfo_p->tbSize*/
            tbSize = dlHARQTBOneInfo_p->tbSize;
            /* SPR 5999 changes start */
            requiredRB = DETERMINE_RBs_DL(mcsIndexCW1, tbSize, internalCellIndex);
            /* SPR 5999 changes end */
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "requiredRB = %u\n",requiredRB);
            if ( requiredRB && (availableRBCount >= requiredRB) )
            {
                dlHARQTBOneInfo_p->mcsIndex = mcsIndexCW1;          
                dlHARQTBOneInfo_p->retransmissionTBSize = tbSize;
                harqProcess_p->assignedResourcesNB = requiredRB;    
                tbSize  = determineTBSize(mcsIndexCW1, requiredRB,
                                           /*4X4 DL MIMO CHG START*/
                                           TB_MAPPED_TO_ONE_LAYER
                                           /*4X4 DL MIMO CHG END*/);
 
                if ( tbSize > HARQ_TB_MINIMUM_SIZE_IN_BYTES )
                {    
                    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion = %s\n",__func__);
                    dlHARQTBTwoInfo_p->mcsIndex = mcsIndexCW2;
                    dlHARQTBTwoInfo_p->tbSize = tbSize;
                    return MAC_SUCCESS;
                }  
                else
                {
                    if (INVALID_TA_DRX_MSG  == dlHARQTBTwoInfo_p->taDrxFlag ) 
                    {
                        harqProcess_p->isTBTwoValid = TB_INVALID;
                        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                                "Leaving funtion = %s with Success\n",__func__);
                        return MAC_SUCCESS;
                    }
                    dlHARQTBTwoInfo_p->mcsIndex = mcsIndexCW2;
                    dlHARQTBTwoInfo_p->tbSize = determineTBSize(mcsIndexCW2, 
                                                requiredRB,
                                                 /*4X4 DL MIMO CHG START*/
                                                 TB_MAPPED_TO_ONE_LAYER
                                                 /*4X4 DL MIMO CHG END*/);
  
                    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion = %s with Success\n",__func__);
                    return  MAC_SUCCESS;
                    
                }
            }
        }
    }

    if ( harqProcess_p->isTBOneValid)
    {
        /*Get the MCS and TB Size of CodeWord One*/
        mcsIndexCW1 = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordOne;  
        if ( NEW_TX == dlHARQTBOneInfo_p->txIndicator)

        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "TB One New TX \n");  
            tbSizeCW1 = determineTBSize(mcsIndexCW1, availableRBCount,
                                         /*4X4 DL MIMO CHG START*/
                                        TB_MAPPED_TO_ONE_LAYER
                                        /*4X4 DL MIMO CHG END*/);

          harqProcess_p->assignedResourcesNB = allocatedRB;

            /* Populate the dlHARQTBOneInfo_p  structures
             * with Relevant Information*/
            if (tbSizeCW1 > HARQ_TB_MINIMUM_SIZE_IN_BYTES)
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion = %s with Success\n",__func__);
                dlHARQTBOneInfo_p->mcsIndex = mcsIndexCW1;
                dlHARQTBOneInfo_p->tbSize = tbSizeCW1;
                return MAC_SUCCESS;
            }
            else 
            {
                if (INVALID_TA_DRX_MSG == dlHARQTBOneInfo_p->taDrxFlag)
                {
                    return MAC_FAILURE;            
                }
                dlHARQTBOneInfo_p->mcsIndex = mcsIndexCW1;
                dlHARQTBOneInfo_p->tbSize = tbSizeCW1;
                return MAC_SUCCESS;
            }
        }
        else
        {
            /*Get the required number of RBs which can accommodate ReTransmitted
             *TB Size*/
            UInt32 tbSize = 0;
            UInt32 requiredRB = 0;
            tbSize = dlHARQTBOneInfo_p->tbSize;
            /* SPR 5999 changes start */
            requiredRB = DETERMINE_RBs_DL(mcsIndexCW1, tbSize, internalCellIndex); 
            /* SPR 5999 changes end */
            if ( requiredRB && (availableRBCount >= requiredRB) )
            {
                /*Populate the harqProcess_p and dlHARQTBTwoInfo_p with relevant
                 * Information8*/
                harqProcess_p->assignedResourcesNB = requiredRB;
                dlHARQTBOneInfo_p->mcsIndex = mcsIndexCW1;    
                dlHARQTBOneInfo_p->retransmissionTBSize = tbSize;
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion = %s with SUCCESS\n",
                        __func__);
                return MAC_SUCCESS;
            }
        }
    }

    if ( harqProcess_p->isTBTwoValid)
    {
        /*Get the MCS and TB Size of CodeWord Two*/
        mcsIndexCW2 = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordTwo;  
        if ( NEW_TX == dlHARQTBTwoInfo_p->txIndicator)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "TB Two New TX \n");  
            tbSizeCW2 = determineTBSize(mcsIndexCW2, availableRBCount,
                                        /*4X4 DL MIMO CHG START*/
                                        TB_MAPPED_TO_ONE_LAYER
                                        /*4X4 DL MIMO CHG END*/);

            harqProcess_p->assignedResourcesNB = allocatedRB;
            /* Populate the dlHARQTBTwoInfo_p  structures
             * with Relevant Information*/
            if (tbSizeCW2 > HARQ_TB_MINIMUM_SIZE_IN_BYTES)
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion = %s with Success\n",__func__);
                dlHARQTBTwoInfo_p->mcsIndex = mcsIndexCW2;
                dlHARQTBTwoInfo_p->tbSize = tbSizeCW2;
                return MAC_SUCCESS;
            }
            else 
            {
                if (INVALID_TA_DRX_MSG == dlHARQTBTwoInfo_p->taDrxFlag)
                {
                    return MAC_FAILURE;            
                }
                dlHARQTBTwoInfo_p->mcsIndex = mcsIndexCW2;
                dlHARQTBTwoInfo_p->tbSize = tbSizeCW2;
                return MAC_SUCCESS;
            }
        }
        else
        {
            /*Get the required number of RBs which can accommodate ReTransmitted
             *TB Size*/
            UInt32 tbSize = 0;
            UInt32 requiredRB = 0;
            tbSize = dlHARQTBTwoInfo_p->tbSize;
            /* SPR 5999 changes start */
            requiredRB = DETERMINE_RBs_DL(mcsIndexCW2, tbSize, internalCellIndex); 
            /* SPR 5999 changes end */
            if ( requiredRB && (availableRBCount >= requiredRB) )
            {
                /*Populate the harqProcess_p and dlHARQTBTwoInfo_p with relevant
                 * Information8*/
                harqProcess_p->assignedResourcesNB = requiredRB;
                dlHARQTBTwoInfo_p->mcsIndex = mcsIndexCW2;    
                dlHARQTBTwoInfo_p->retransmissionTBSize = tbSize;
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion = %s with SUCCESS\n",
                        __func__);
                return MAC_SUCCESS;
            }
        }
    }
    LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,
            "Leaving funtion = %s with Failure  Either TB has to be New TX\n",
            __func__);
    return MAC_FAILURE;
}
#endif
/* - coverity 42178 */
/******************************************************************************
 * Function Name    : getRAT0BitMapNewTxNonSpatialMux
 * Inputs           : resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                    harqProcess_p -This holds the pointer to DLHARQProcess,
 *                    dlStrategyNode_p  - Pointer to DLStrategyTxNode,   
 *                    rat0Info_p - This holds the RAT0Info structure
 *                    dlUEContext_p (HD_FDD only)
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : bitMapCqiAndAvailalbeRBG
 * Description      : This funtion will return the RBG BitMap which is found by 
 *                    ANDing availableRBGBitMap and available CQI BitMap for New  
 *                    Transmission.
 ******************************************************************************/
UInt32 getRAT0BitMapNewTxNonSpatialMux( 
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        DLStrategyTxNode *dlStrategyNode_p,     
        RAT0Info *rat0Info_p
/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
        ,DLUEContext *dlUEContext_p
#endif
        ,InternalCellIndex internalCellIndex
/*HD FDD Changes End*/
        )
{
    RAT0NonSpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
    DLHARQTBInfo *dlHARQTBInfo_p = PNULL;
    UInt32 cqiIndexCount = 0;
    UInt32 *cqiBitMap_p = PNULL;
    UInt32 bitMapCqiAndAvailalbeRBG = 0;
    UInt32 availableRBCount = 0;

    UInt16 tmpBitMap = 0;
    UInt8  cqiReportIndex = 0;
    UInt32 freqSelectiveTBSize = 0;
    UInt32 freqSelectiveMCSIndex = 0;
    
    UInt32 previousTBSize = 0;
    UInt32 previousMCS = 0;
    UInt32 previousPRBs = 0;
    UInt32 previousCqiRBGBitmap = 0;
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
   /* SPR 15909 fix start */
    tickType_t currTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
#endif
    /*HD FDD Changes End*/
    
    UInt32 minTBSize = dlStrategyNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyNode_p->maxTBSize;

    if (maxTBSize < minTBSize)
    {
        lteWarning("[RAT0 NonSpatial] Wrong Max TBSize calculated by DL Strategy\
                     minTBSize [%d] maxTBSize [%d]\n", minTBSize, maxTBSize);
        maxTBSize = minTBSize;
    }

    SubBandReportInfo *subBandReportInfo_p = resrcAllocatorInput_p->subBandReportInfo_p;
    UInt32 requiredRBCount = resrcAllocatorInput_p->requiredRB;

    /* ICIC changes start */
    UInt8 resourceRegionAllocated = resrcAllocatorInput_p->resourceRegionAllocated;
    UInt32 availableRBGBitMap = rat0Info_p->rat0BitMap[CC_USER] & 
        rat0Info_p->rat0BitMap[resourceRegionAllocated];
    /* ICIC changes end */
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    dlHARQTBInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    rat0CQIInfo_p = &subBandReportInfo_p->cqiInfo.cqiInfoNonSpatialMul.
        rat0NonSpatialMulCQIInfo;    

    tmpBitMap = rat0CQIInfo_p->reportedCQIBitMap;

    if(!tmpBitMap)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,
            "NO CQI BitMap available");
        return 0;
    }

     
    /* Below while loop is iterating x number of times where x is the 
     * difference of the Highest CQI Index and  lowest CQI Index for 
     * Subband whose report is available.For each subband it checks if 
     * the tbSize can be satisfied from it ,if true then it will populate 
     * the relevant information in harqProcess_p structure  else it will
     * jump to next possible Lower CQI Index SubBands.*/ 
    while( tmpBitMap )
    {
        /* Decode the bit map of the reported CQI */
        cqiReportIndex = FFS(tmpBitMap);
        tmpBitMap &= (~((UInt16)1 << (cqiReportIndex-1)));
        cqiIndexCount = (MAX_CQI_INDEX - cqiReportIndex); 

        /*start with the highest reported CQI Index*/
        cqiBitMap_p = &rat0CQIInfo_p->rat0CQIBitMap[cqiIndexCount];

        /*Adding  more Availavle RBG in the bitMapCqiAndAvailalbeRBG bitMap
         *whose report is available*/
        bitMapCqiAndAvailalbeRBG = (*cqiBitMap_p & availableRBGBitMap);
        availableRBCount = getCount(bitMapCqiAndAvailalbeRBG) * 
                                                    rat0Info_p->rbgSize;
        if (!availableRBCount)                               
        {
            continue;
        }

        if (bitMapCqiAndAvailalbeRBG & 
                              ((UInt32) 1 << (rat0Info_p->rbgCount - 1)))
        {
            availableRBCount = availableRBCount - rat0Info_p->rbgSize +
                rat0Info_p->rbgIndexInfo[rat0Info_p->rbgCount - 1].numOfRBs;

        }
        
        freqSelectiveMCSIndex = getMCSFromCQI(cqiIndexCount
                                );


	    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
	    /* FILL the MCS index value from CQI codeword*/
	    HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currTick, dlUEContext_p, freqSelectiveMCSIndex);
#endif
	    /*HD FDD Changes End*/    

        if (availableRBCount >= requiredRBCount)
        {   
            freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex,
                                                  requiredRBCount,
                                                  /*4X4 DL MIMO CHG START*/
                                                  TB_MAPPED_TO_ONE_LAYER
                                                  /*4X4 DL MIMO CHG END*/);

            /*If TB size on this mcs with requiredRBCount is greater than minimum TB size then 
                       we can go for maximum TB size too if excess Pool has some RBs left. This case means successful as 
                       minimum TB size is always met*/         
            if (freqSelectiveTBSize >= minTBSize)
            {
                if (freqSelectiveTBSize >= maxTBSize)
                {
                    freqSelectiveTBSize = maxTBSize;
                    /* SPR 5999 changes start */
                    harqProcess_p->assignedResourcesNB = DETERMINE_RBs_DL(freqSelectiveMCSIndex,
                                                                          freqSelectiveTBSize,
                                                                          internalCellIndex);                 
                    /* SPR 5999 changes end */
                }
                else
                {
                    harqProcess_p->assignedResourcesNB = requiredRBCount;
                }
                
                dlHARQTBInfo_p->mcsIndex = freqSelectiveMCSIndex;

                //update excess PRBs in buffer pool
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                        (requiredRBCount - harqProcess_p->assignedResourcesNB), internalCellIndex);
                /* ICIC changes end */
                
                return bitMapCqiAndAvailalbeRBG;
            }
            else
            {
                /*Here allocation can be done only if some RBs are left in excessPool which allows TBSize to be greater
                              than or equal to minimum TB Size. If not possible then store this TBSize and found RBs as later on it will
                              required to allocate if strict subband allocation is required*/
                    
                UInt32 requiredRBsFromPool = 0;
                if (excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0)
                {
                    requiredRBsFromPool = allocatePRBsUsingExcessPool(freqSelectiveMCSIndex,
                                                                   availableRBCount,
                                                                   minTBSize,
                                                                   requiredRBCount,
                                                                   resourceRegionAllocated,
                                                                   internalCellIndex);
                    
                    freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex, 
                                                          requiredRBCount+ requiredRBsFromPool,
                                                           /*4X4 DL MIMO CHG START*/
                                                           TB_MAPPED_TO_ONE_LAYER
                                                           /*4X4 DL MIMO CHG END*/);
 
                    if (freqSelectiveTBSize >= minTBSize)
                    {
                        dlHARQTBInfo_p->mcsIndex = freqSelectiveMCSIndex;
                        harqProcess_p->assignedResourcesNB = requiredRBCount + requiredRBsFromPool; 

                        //update excess PRBs in buffer pool
                        /* ICIC changes start */
                        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                (requiredRBCount - harqProcess_p->assignedResourcesNB), internalCellIndex);
                        /* ICIC changes end */
                
                        return bitMapCqiAndAvailalbeRBG;                    
                    }
                }
                
                if(previousTBSize < freqSelectiveTBSize)
                {
                    previousTBSize = freqSelectiveTBSize;
                    previousMCS = freqSelectiveMCSIndex;
                    previousPRBs = requiredRBCount+ requiredRBsFromPool;
                    previousCqiRBGBitmap = bitMapCqiAndAvailalbeRBG;
                }               
            }
        }
        else
        {
            /* Here although available RBs are less than required RBs but still allocation can be done as
                    reported MCS can be greater than wideband MCS used by strategy hence min/max TB size can be
                    achieved with available RBs*/
            freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex,
                                                  availableRBCount,
                                                   /*4X4 DL MIMO CHG START*/
                                                   TB_MAPPED_TO_ONE_LAYER
                                                   /*4X4 DL MIMO CHG END*/);
 
            if (freqSelectiveTBSize >= minTBSize)
            {
                if (freqSelectiveTBSize > maxTBSize)
                {
                    freqSelectiveTBSize = maxTBSize;
                }
                
                dlHARQTBInfo_p->mcsIndex = freqSelectiveMCSIndex;               
                /* SPR 5999 changes start */
                harqProcess_p->assignedResourcesNB = DETERMINE_RBs_DL(freqSelectiveMCSIndex,
                                                                      freqSelectiveTBSize,
                                                                      internalCellIndex);
                /* SPR 5999 changes end */
                //update excess PRBs in buffer pool
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                        (requiredRBCount - harqProcess_p->assignedResourcesNB), internalCellIndex);
                /* ICIC changes end */
                return bitMapCqiAndAvailalbeRBG;
            }
            else
            {
                if(previousTBSize < freqSelectiveTBSize)
                {
                    previousTBSize = freqSelectiveTBSize;
                    previousMCS = freqSelectiveMCSIndex;
                    previousPRBs = availableRBCount;
                    previousCqiRBGBitmap = bitMapCqiAndAvailalbeRBG;
                }
            }
        }
    }
    bitMapCqiAndAvailalbeRBG = 0;
    /*It means that we are not able to meet minTBSize asked by strategy hence if strict subband allocation
           is required then we will schedule this UE with less than minimum TB Size*/
    if (!dlStrategyNode_p->strictAllocatedFlag)
    {
        /* As of now, there is no use case for strictAllocSubBand
         * strictAllocSubBand check is commented */
        //if (resrcAllocatorInput_p->strictAllocSubBand)
        //{
            if(previousTBSize >= HARQ_TB_MINIMUM_SIZE_IN_BYTES)
            {
                harqProcess_p->assignedResourcesNB = previousPRBs;
                dlHARQTBInfo_p->mcsIndex = previousMCS;    
                dlHARQTBInfo_p->tbSize = previousTBSize;
                bitMapCqiAndAvailalbeRBG = previousCqiRBGBitmap;

                //update excess PRBs in buffer pool
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                        (requiredRBCount - previousPRBs), internalCellIndex);
                /* ICIC changes end */
            }
        //}
    }

    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    return bitMapCqiAndAvailalbeRBG;
}

/******************************************************************************
 * Function Name    : updateRAT0InfoFromRAT1Info
 * Inputs           : rbgIndexBitMap - This holds the BitMap in which that bit is
 *                                     unset which is mapped to RBG Index
 * Outputs          : rat0BitMap_p - This holds pointer to rat0BitMap which need to 
 *                                   be updated
 * Returns          : None
 * Description      : This funtion will update the RAT 0 Information based on 
 *                    RAT 1 Information.
 ******************************************************************************/
STATIC  void updateRAT0InfoFromRAT1Info(
        UInt32 rbgIndexBitMap,
        UInt32 *rat0BitMap_p) 
{
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    (*rat0BitMap_p) &= rbgIndexBitMap;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}

/******************************************************************************
 * Function Name    : updateRAT2InfoFromRAT1Info
 * Inputs           : prbNumber - This holds the PRB number of PRB which has been
 *                                allocated,
 *                    raPolicy_p - This holds pointer to ResourceAllocationPolicyInfo
 *                                 structure
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtion will update the RAT 2 Localized and RAT 2
 *                    Distributed Information based on RAT 1 Information.
 ******************************************************************************/
STATIC  void  updateRAT2InfoFromRAT1Info(
        UInt32  prbNumber ,
        ResourceAllocationPolicyInfo *raPolicy_p,
        InternalCellIndex internalCellIndex)
{
    UInt32 previousFreeVRBIndex = 0;
    UInt32 nextFreeVRBIndex = 0;
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;   
    RAT2LocalizedVRBInfo *tempLocalizedVRBInfo_p = PNULL;   
    /* RAT2 Distributed information need not be updated for data messages,
     * as RAT2 Distributed is not used for data messages
     */
    
    localizedVRBInfo_p = raPolicy_p->rat2LocalizedInfo.
        rat2LocalizedVRBInfo;

    tempLocalizedVRBInfo_p = &localizedVRBInfo_p[prbNumber + 1];      
    /*Get the VRB Number which is mapped to Even Slot of current PRB
     *having PRB number prbNumber*/
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "vrbMappedToEvenSlotOfPRB = %u vrbMappedToOddSlotOfPRB  =%u\n",
            tempLocalizedVRBInfo_p->vrbMappedToEvenSlotOfPRB, tempLocalizedVRBInfo_p->vrbMappedToOddSlotOfPRB);
    /* RAT2 Distributed information need not be updated for data messages,
     * as RAT2 Distributed is not used for data messages
     */
    /*Updating the localizedVRBInfo_p array such that a Free VRB knows
     *the next Free VRB Index and also previous Free VRB Index*/
    previousFreeVRBIndex = tempLocalizedVRBInfo_p->previousVRBfreeIndex;
    nextFreeVRBIndex = tempLocalizedVRBInfo_p->nextfreeVRBIndex;
    
    localizedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex = 
        nextFreeVRBIndex;
    localizedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex = 
        previousFreeVRBIndex;  
    raPolicy_p->rat2LocalizedInfo.numOfVRBAvailable -= 1;

    /* ICIC changes start */
    if (CELL_EDGE_ONLY_REGION ==
        (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->rbToResourceTypeMapDL[prbNumber])
    {
        raPolicy_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable -= 1;
        
        /* check if prbNumber is same as firstCellEdgeFreeIndex or
         * or endCellEdgeFreeIndex, if so update these indices
         */
        if (raPolicy_p->rat2LocalizedInfo.firstCellEdgeFreeIndex == prbNumber)
        {
            raPolicy_p->rat2LocalizedInfo.firstCellEdgeFreeIndex = nextFreeVRBIndex;
        }
        if (raPolicy_p->rat2LocalizedInfo.endCellEdgeFreeIndex == prbNumber)
        {
            raPolicy_p->rat2LocalizedInfo.endCellEdgeFreeIndex = 
                previousFreeVRBIndex;
        }
      
        /* RAT2 distributed support for ICIC needs to be added */   
    }
    /* ICIC changes end */


#ifdef PERF_STATS
    /* Overlapping RB global is updated*/
    /** CA-TDD Changes Start **/
    if(perfStatsMibSyncSignalFlag_g[internalCellIndex] && 
		    prbNumber >= central6RBStartIndex_g[internalCellIndex] && 
		    prbNumber <= central6RBStartIndex_g[internalCellIndex] + 5)
        /** CA-TDD Changes Stop **/
	{
		overlapRBsMibSS_g[internalCellIndex]++;
	}
#endif


    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}


/******************************************************************************
 * Function Name    : sortSubsetBasedOnCQIForSpatialMul
 * Inputs           : rat1Info_p - This holds pointer to RAT1Info structure,
 *                    subBandReportInfo_p - This is pointer to SubBandReportInfo
 *                                          structure,
 *                    resourceRegionAllocated - Allocated resource region,
 *                    centralRbMask, 
 *                    transmissionMode - Mode of transmission,
 *                    scheduledTick - Scheduled TTI
 * Outputs          : subsetInfoForSorting - This is array of SubSetSpanInfoForSorting
 *                                           structure, which return subset Index and other
 *                                           relevant infomation in the order of increasing
 *                                           Free RB in the Subset
 * Returns          : None
 * Description      : This funtion will first initialize the SubSetSpanInfoForSorting 
 *                    structure and then it sort this array of structure based on 
 *                    numFreeRB.
 ******************************************************************************/
/* ICIC changes start */
void sortSubsetBasedOnCQIForSpatialMul( 
        RAT1Info *rat1Info_p,
        SubBandReportInfo *subBandReportInfo_p,
        SubSetSpanInfoForSorting subsetInfoForSorting[MAX_SUBSETS],
        UInt32 resourceRegionAllocated,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask[MAX_SUBSETS],
        UInt8 transmissionMode,
        /* SPR 15909 fix start */
        tickType_t scheduledTick)
        /* SPR 15909 fix end */
        /* - TM7_8 Changes End */
/* ICIC changes end */

{
    UInt32 outLoopCount = 0;
    UInt32 innerLoopCount = 0;
    UInt32 tempBitMapWithSpanNonZero = 0;
    UInt32 tempBitMapWithSpanZero = 0;
    UInt32 availableRBBitMap = 0;
    UInt32 rbCountNonZeroSpan = 0;
    UInt32 rbCountZeroSpan = 0;
    SubsetInfo    *subsetInfo_p = PNULL;
    RAT1SpatialMulCQIInfo *cqiInfo_p = PNULL;
    SubSetSpanInfoForSorting tempSubSetSpanInfoForSorting = {0,0,0,0};
    

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    subsetInfo_p = rat1Info_p->subsetInfo;
    outLoopCount = rat1Info_p->subsetCount;    
    innerLoopCount = outLoopCount;
    
    /* SPR#706 Changes Start */
    if((!outLoopCount) || (MAX_SUBSETS<outLoopCount))
    {
        return;
    }
    /* SPR#706 Changes End */
    cqiInfo_p = &subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.
        rat1SpatialMulCQIInfo[outLoopCount - 1]; 
    /*Below while Loop Is Initialing subsetInfoForSorting with relevant 
     *Information which will be used for sorting this array of structure
     *based on num Free RB in each indexed structure*/
    while (outLoopCount--)
    {
        subsetInfoForSorting[outLoopCount].subsetIndex = outLoopCount;
        /* ICIC changes start */
        availableRBBitMap = subsetInfo_p[outLoopCount].subsetBitMap[CC_USER] & 
            subsetInfo_p[outLoopCount].subsetBitMap[resourceRegionAllocated];        
        /* ICIC changes end */
 
        /* + TM7_8 Changes Start */
/* SPR 21631 Start */
            if(CHECK_SF_MAPPED_ON_BCH_PSS_SSS(scheduledTick,transmissionMode))
                /* SPR 21631 End */
        {
            /*block central 6 RBs in temp variable*/
            availableRBBitMap = availableRBBitMap &(~centralRbMask[outLoopCount]);
        }

        /* - TM7_8 Changes End */
        
        /*Get the Available Number of RB Bit which falls into subset using
         *LeftSpan*/
        tempBitMapWithSpanZero = availableRBBitMap &
            cqiInfo_p->subsetCQIBitMapALLCQIIndex & 
            subsetInfo_p[outLoopCount].bitMapWithSpanZero;

        /*Get the Available Number of RB Bit which falls into subset using
         *RightSpan*/    
        tempBitMapWithSpanNonZero = availableRBBitMap & 
            cqiInfo_p->subsetCQIBitMapALLCQIIndex &
            subsetInfo_p[outLoopCount].bitMapWithSpanNonZero;

        rbCountNonZeroSpan = getCount(tempBitMapWithSpanNonZero );
        rbCountZeroSpan = getCount(tempBitMapWithSpanZero );
        if ( rbCountNonZeroSpan > rbCountZeroSpan )
        {
            subsetInfoForSorting[outLoopCount].numFreeRB = rbCountNonZeroSpan;
            subsetInfoForSorting[outLoopCount].isNonZeroSpanConsideredForAlloc
                = 1;
            subsetInfoForSorting[outLoopCount].availableRBBitMap = 
                tempBitMapWithSpanNonZero;
        }
        else
        {
            subsetInfoForSorting[outLoopCount].numFreeRB = rbCountZeroSpan;
            subsetInfoForSorting[outLoopCount].isNonZeroSpanConsideredForAlloc 
                = 0;
            subsetInfoForSorting[outLoopCount].availableRBBitMap = 
                tempBitMapWithSpanZero;
        }
        cqiInfo_p--;

    }   

    outLoopCount = innerLoopCount;
    /*Below while Loop Is sorting the subsetInfoForSorting array structure
     *based on num Free RB in each indexed structure*/
    while ( outLoopCount-- )
    {
        /* SPR#706 Changes Start */
        innerLoopCount = rat1Info_p->subsetCount - 1;
        /* SPR#706 Changes End */
        while (innerLoopCount)
        {
            if ( subsetInfoForSorting[innerLoopCount].numFreeRB <
                    subsetInfoForSorting[innerLoopCount - 1].numFreeRB )
            {
                tempSubSetSpanInfoForSorting  =  subsetInfoForSorting
                    [innerLoopCount - 1];
                subsetInfoForSorting[innerLoopCount - 1] = 
                    subsetInfoForSorting[innerLoopCount ];
                subsetInfoForSorting[innerLoopCount ] = tempSubSetSpanInfoForSorting;
            }
            innerLoopCount--;
        }

    }    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}

/******************************************************************************
 * Function Name    : getRAT1BitMapBothTBNewTx
 * Inputs           : rat1Info_p - This holds pointer to  RAT1Info structure,
 *                    resrcAllocatorInput_p -Pointer to ResourceAllocatorInput,
 *                    harqProcess_p - This Holds pointer to DLHARQProcess structure,
 *                    dlStrategyNode_p - Pointer to DLStrategyTxNode,
 *                    centralRbMask,
 *                    scheduledTick - Scheduled TTI,
 *                    transmissionMode - Mode of Transmission
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : subsetIndex_p - This holds the  subset Index of the Subset 
 *                                    which has maximum number of FREE PRBs,
 *                    isSpanZeroConsideredForAlloc_p - This Indicates if the RBs
 *                                                     has been considered using Zero
 *                                                     Span Width or Non Zero Span width. 
 * Returns          : bitMapToBeReturned_p - This Holds the BitMap which we will get 
 *                                           after ANDing availableRBGBitMap with the
 *                                           subBand CQI BitMap.
 * Description      : This funtion will first get subset Index of Subset which has 
 *                    Maximum number of FREE PRBs and then it will call relevant 
 *                    funtions for returning the available RB Bit.
 ******************************************************************************/ 
STATIC UInt32 getRAT1BitMapBothTBNewTx( 
        RAT1Info *rat1Info_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc_p,
        DLStrategyTxNode *dlStrategyNode_p,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask[MAX_SUBSETS],
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 transmissionMode,
        InternalCellIndex internalCellIndex)
        /* - TM7_8 Changes End */
{
/*    SubsetInfo    *subsetInfo_p = PNULL;TDD Warning Fix */    
    UInt32 subsetCount = 0;
    SubSetSpanInfoForSorting subsetInfoForSorting[MAX_SUBSETS] = {{0,0,0 ,0}} ;
    RAT1SpatialMulCQIInfo *cqiInfo_p = PNULL;
    UInt32 subsetIndex= 0;
    UInt32 bitMapToBeReturned = 0;
    UInt32 pmiIndex = WIDEBAND_PMI;
    ResourceFormatMIMONode previousRsrcFrmt = {0};
	UInt32 isNonZeroSpanConsidered =0;	   
 
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR, "Entering funtion = %s\n",__func__);
    
/*    subsetInfo_p = rat1Info_p->subsetInfo; TDD Warning Fix */
    subsetCount = rat1Info_p->subsetCount;    
    cqiInfo_p = 
        resrcAllocatorInput_p->subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.rat1SpatialMulCQIInfo;
    SubbandPMIInfo *subbandPMIInfo_p = 
        &(resrcAllocatorInput_p->subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo);

    /* ICIC changes start */
    sortSubsetBasedOnCQIForSpatialMul(rat1Info_p,
            resrcAllocatorInput_p->subBandReportInfo_p, subsetInfoForSorting,
            resrcAllocatorInput_p->resourceRegionAllocated,
            /* TM7_8 Changes Start */
            centralRbMask,
            transmissionMode,
            scheduledTick);
            /* TM7_8 Changes End */
  /* ICIC changes end */

    /*Below While Loop is called for each subsetIndex starting from subset 
     *which has maximum number of FREE RB taking consideration of Left 
     *and Right Span.For each iteration it call relevant funtion which will 
     *check if it can satisfy the required RB or not*.On Success it return 
     *the bitMapToBeReturned_p with nonZero Value else it will return 
     *Zero Value*/
    while ( subsetCount-- )
    {
        subsetIndex = subsetInfoForSorting[subsetCount].subsetIndex;
        if (subsetInfoForSorting[subsetCount].isNonZeroSpanConsideredForAlloc)
        {
            /*Making isNonZeroSpanConsideredForAlloc to true which will be used
             * while forming resource assignment BitMap to indicate whether RBs
             * are allocated from using Zero Span Width  or RBs are allocated 
             * using None Span Width in the Subset Indexed by maxPRBSubsetIndex.
             * If Set to one this means RBS are allocated using None Zero Span
             * Width else RBs are allocated using  Zero Span Width*/
            isNonZeroSpanConsidered = 1;
            /*select PMI subset*/
            if (resrcAllocatorInput_p->subbandPMI)
            {
                pmiIndex = selectRAT1SubbandPMIIndexRank2(
                                 subsetInfoForSorting[subsetCount].availableRBBitMap, 
                                 &(subbandPMIInfo_p->rat1PmiInfo[subsetCount]),
                                 &(subsetInfoForSorting[subsetCount].availableRBBitMap),
                                 1 /*NonZero Span*/
                                 ,internalCellIndex);

                harqProcess_p->pmiIndex = pmiIndex;
            }
            
            bitMapToBeReturned = getSubsetBitMapBothTBNewTx(
                                    harqProcess_p,
                                    subsetInfoForSorting[subsetCount].availableRBBitMap,                                    
                                    &cqiInfo_p[subsetIndex].subsetNonZeroSpanCQIInfo,
                                    resrcAllocatorInput_p,
                                    rat1Info_p->subsetInfo[subsetIndex].bitPositionToSubsetAndRBGInfo,
                                    dlStrategyNode_p,
                                    &previousRsrcFrmt,subsetIndex,isNonZeroSpanConsidered,
                                    internalCellIndex);
            
            if (bitMapToBeReturned)
            {
				*isNonZeroSpanConsideredForAlloc_p = isNonZeroSpanConsidered; 
               *subsetIndex_p = subsetIndex;
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion Success  = %s\n",__func__);
                return bitMapToBeReturned;     
            }        
        }
        else
        {
            /*Making isNonZeroSpanConsideredForAlloc to false which will be used
             * while forming resource assignment BitMap to indicate whether RBs
             * are allocated from using Zero Span Width  or RBs are allocated 
             * using None Span Width in the Subset Indexed by maxPRBSubsetIndex.
             * If Set to one this means RBS are allocated using None Zero Span
             * Width else RBs are allocated using  Zero Span Width*/

            isNonZeroSpanConsidered = 0;        
            
            /*select PMI subset*/
            if (resrcAllocatorInput_p->subbandPMI)
            {
                pmiIndex = selectRAT1SubbandPMIIndexRank2(
                                 subsetInfoForSorting[subsetCount].availableRBBitMap, 
                                 &(subbandPMIInfo_p->rat1PmiInfo[subsetCount]),
                                 &(subsetInfoForSorting[subsetCount].availableRBBitMap),
                                 0 /*Zero Span*/
                                 ,internalCellIndex);

                harqProcess_p->pmiIndex = pmiIndex;
            }

            bitMapToBeReturned = getSubsetBitMapBothTBNewTx(
                                    harqProcess_p,
                                    subsetInfoForSorting[subsetCount].availableRBBitMap,                                    
                                    &cqiInfo_p[subsetIndex].subsetZeroSpanCQIInfo,                                    
                                    resrcAllocatorInput_p,
                                    rat1Info_p->subsetInfo[subsetIndex].bitPositionToSubsetAndRBGInfo,
                                    dlStrategyNode_p,
                                    &previousRsrcFrmt,subsetIndex,isNonZeroSpanConsidered,
                                    internalCellIndex);
            if (bitMapToBeReturned)
            {
				*isNonZeroSpanConsideredForAlloc_p = isNonZeroSpanConsidered;
                *subsetIndex_p = subsetIndex;
                
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion Success  = %s\n",__func__);
                return bitMapToBeReturned;     
            } 
        }

    }

    bitMapToBeReturned = 0;
    
    /*It means that we are not able to meet minTBSize asked by strategy hence if strict subband allocation
           is required then we will schedule this UE with less than minimum TB Size*/
    if (!dlStrategyNode_p->strictAllocatedFlag)
    {
        /* As of now, there is no use case for strictAllocSubBand
         * strictAllocSubBand check is commented */
        //if (resrcAllocatorInput_p->strictAllocSubBand) 
        //{
            if (previousRsrcFrmt.tbSizeCW1 > HARQ_TB_MINIMUM_SIZE_IN_BYTES  &&
                    previousRsrcFrmt.tbSizeCW2 > HARQ_TB_MINIMUM_SIZE_IN_BYTES)
            {
                /*Populate the dlHARQTBOneInfo_p and dlHARQTBTwoInfo_p
                              * structure with relevant fields*/
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = previousRsrcFrmt.mcsIndexCW1;
                harqProcess_p->dlHARQTBOneInfo.tbSize = previousRsrcFrmt.tbSizeCW1;
                
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex = previousRsrcFrmt.mcsIndexCW2;
                harqProcess_p->dlHARQTBTwoInfo.tbSize = previousRsrcFrmt.tbSizeCW2;
                
                harqProcess_p->assignedResourcesNB = previousRsrcFrmt.prbs; 
                
                bitMapToBeReturned = previousRsrcFrmt.cqiRBGBitmap;
                *isNonZeroSpanConsideredForAlloc_p = previousRsrcFrmt.isNonZeroSpan;
                *subsetIndex_p = previousRsrcFrmt.subsetIndex;
                /* SPR 5148 changes start */
                //update excess PRBs in buffer pool
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resrcAllocatorInput_p->resourceRegionAllocated, 
                       (resrcAllocatorInput_p->requiredRB- harqProcess_p->assignedResourcesNB), internalCellIndex);
                /* ICIC changes end */
                /* SPR 5148 changes end */
            }  
            else if ( previousRsrcFrmt.tbSizeCW1 > HARQ_TB_MINIMUM_SIZE_IN_BYTES)
            {
                harqProcess_p->isTBTwoValid = TB_INVALID;
                
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = previousRsrcFrmt.mcsIndexCW1;
                harqProcess_p->dlHARQTBOneInfo.tbSize = previousRsrcFrmt.tbSizeCW1;
                
                harqProcess_p->assignedResourcesNB = previousRsrcFrmt.prbs; 
                *isNonZeroSpanConsideredForAlloc_p = previousRsrcFrmt.isNonZeroSpan;
                *subsetIndex_p = previousRsrcFrmt.subsetIndex;
                
                bitMapToBeReturned = previousRsrcFrmt.cqiRBGBitmap;
                /* SPR 5148 changes start */
                //update excess PRBs in buffer pool
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resrcAllocatorInput_p->resourceRegionAllocated, 
                       (resrcAllocatorInput_p->requiredRB- harqProcess_p->assignedResourcesNB), internalCellIndex);
                /* ICIC changes end */
                /* SPR 5148 changes end */
            }
            else if ( previousRsrcFrmt.tbSizeCW2> HARQ_TB_MINIMUM_SIZE_IN_BYTES)
            {
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex = previousRsrcFrmt.mcsIndexCW2;
                harqProcess_p->dlHARQTBTwoInfo.tbSize= previousRsrcFrmt.tbSizeCW2;
                *isNonZeroSpanConsideredForAlloc_p = previousRsrcFrmt.isNonZeroSpan;
                *subsetIndex_p = previousRsrcFrmt.subsetIndex;
                
                harqProcess_p->assignedResourcesNB = previousRsrcFrmt.prbs; 
                harqProcess_p->isTBOneValid = TB_INVALID;
                bitMapToBeReturned = previousRsrcFrmt.cqiRBGBitmap;
                /* SPR 5148 changes start */
                //update excess PRBs in buffer pool
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resrcAllocatorInput_p->resourceRegionAllocated, 
                       (resrcAllocatorInput_p->requiredRB- harqProcess_p->assignedResourcesNB), internalCellIndex);
                /* ICIC changes end */
                /* SPR 5148 changes end */
            }           
            harqProcess_p->pmiIndex = previousRsrcFrmt.pmiIndex;
        //}
    }
        
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion with Failure = %s\n",__func__);
    return bitMapToBeReturned;
}

/******************************************************************************
 * Function Name    : getSubsetBitMapBothTBNewTx
 * Inputs           : harqProcess_p -  This holds pointer to DLHARQProcess structure,
 *                    availableRBBitMap -  This Holds Available RB BitMap,
 *                    subetSpanCQIInfo_p - This holds pointer to SubsetSpanCQIInfo 
 *                                         structure,
 *                    resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                    bitPositionToSubsetAndRBGInfo_p - This holds pointer to 
 *                                                      BitPositionToSubsetAndRBGInfo
 *                                                      structure,
 *                    dlStrategyNode_p - Pointer to DLStrategyTxNode,
 *                    previousRsrcFrmt_p - Pointer to ResourceFormatMIMONode,
 *                    subsetIndex - Index of Subset,
 *                    isNonZeroSpan
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : bitMapToBeReturned_p - This Holds the BitMap which we will get
 *                                           after ANDing availableRBGBitMap with the
 *                                           subBand CQI BitMap.
 * Description      : This funtion will first check if the 64QAM Bands can satisfy 
 *                    the required Number of RBs.If successful ,it will populate 
 *                    relevant information in harqProcess_p structure else it will
 *                    go for 16QAM Bands.If 16QAM also don't satisfy the required
 *                  ` Number of RBs ,it will repeat logic for QPSK bands.
 ******************************************************************************/ 
STATIC  UInt32  getSubsetBitMapBothTBNewTx(                
        DLHARQProcess  *harqProcess_p,
        UInt32 availableRBBitMap,
        SubsetSpanCQIInfo *subetSpanCQIInfo_p,        
        ResourceAllocatorInput *resrcAllocatorInput_p,
        BitPositionToSubsetAndRBGInfo *bitPositionToSubsetAndRBGInfo_p,
        DLStrategyTxNode *dlStrategyNode_p,
        ResourceFormatMIMONode *previousRsrcFrmt_p,
        UInt32 subsetIndex,
        UInt32 isNonZeroSpan,
        InternalCellIndex internalCellIndex)
{
/*    DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
    DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL; TDD Warning Fix */

    UInt32 bitMapToBeReturned = 0;    
     
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
/*    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo; TDD Warning Fix*/


    /* We will start from  that CodeWord SubBand BitMap whose number of 64QAM 
       Bands reported are more*/
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "We will start searching 64QAM Bands\n");

    if ( subetSpanCQIInfo_p->count64QAMSubBandCW1 >= 
            subetSpanCQIInfo_p->count64QAMSubBandCW2 )
    {
        bitMapToBeReturned = subetSpanCQIInfo_p->rat164QAMCQIBitMapCW1 &
            availableRBBitMap;
        if (!bitMapToBeReturned)
        {
            bitMapToBeReturned = subetSpanCQIInfo_p->rat164QAMCQIBitMapCW2 &
                availableRBBitMap;            
        }
    }
    else
    {
        bitMapToBeReturned = subetSpanCQIInfo_p->rat164QAMCQIBitMapCW2 &
            availableRBBitMap;
        if (!bitMapToBeReturned)
        {
            bitMapToBeReturned = subetSpanCQIInfo_p->rat164QAMCQIBitMapCW1 &
                availableRBBitMap;            
        }        
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "bitMapToBeReturned_p\n", bitMapToBeReturned);
    /*Check if we any 64QAM subBand available*/
    if (bitMapToBeReturned)
    { 
        if (MAC_SUCCESS == validateRAT1RBsforMinMaxTBSize(
                                       bitMapToBeReturned,
                                       resrcAllocatorInput_p,
                                       harqProcess_p,
                                       previousRsrcFrmt_p,
                                       dlStrategyNode_p,
                                       bitPositionToSubsetAndRBGInfo_p,
                                       subsetIndex,
                                       isNonZeroSpan,
                                       internalCellIndex))
        {
            return bitMapToBeReturned;
        }
    } 

    /* We will start from  that CodeWord SubBand BitMap whose number of 16QAM 
       Bands reported are more*/
    /* + CQI_4.1 */
    if ( subetSpanCQIInfo_p->count16QAMSubBandCW1 >= 
            subetSpanCQIInfo_p->count16QAMSubBandCW2 )
    /* - CQI_4.1 */
    {
        bitMapToBeReturned = subetSpanCQIInfo_p->rat116QAMCQIBitMapCW1 &
            availableRBBitMap;
        if (!bitMapToBeReturned)
        {
            bitMapToBeReturned = subetSpanCQIInfo_p->rat164QAMCQIBitMapCW2 &
                availableRBBitMap;            
        }        
    }
    else
    {
        bitMapToBeReturned = subetSpanCQIInfo_p->rat116QAMCQIBitMapCW2 &
            availableRBBitMap;
        if (!bitMapToBeReturned)
        {
            bitMapToBeReturned = subetSpanCQIInfo_p->rat164QAMCQIBitMapCW1 &
                availableRBBitMap;            
        }        
    }
     
    if (bitMapToBeReturned)
    { 
        if (MAC_SUCCESS == validateRAT1RBsforMinMaxTBSize(
                                        bitMapToBeReturned,
                                        resrcAllocatorInput_p,
                                        harqProcess_p,
                                        previousRsrcFrmt_p,
                                        dlStrategyNode_p,
                                        bitPositionToSubsetAndRBGInfo_p,
                                        subsetIndex,
                                        isNonZeroSpan,
                                        internalCellIndex))
        {
            return bitMapToBeReturned;
        }
    } 

    /* We will start from  that CodeWord SubBand BitMap whose number of QPSK 
       Bands reported are more*/
    /* + CQI_4.1 */
    if ( subetSpanCQIInfo_p->countQPSKSubBandCW1 >= 
            subetSpanCQIInfo_p->countQPSKSubBandCW2 )
    /* - CQI_4.1 */
    {
        bitMapToBeReturned = subetSpanCQIInfo_p->rat1QPSKCQIBitMapCW1 &
            availableRBBitMap;
        if (!bitMapToBeReturned)
        {
            bitMapToBeReturned = subetSpanCQIInfo_p->rat164QAMCQIBitMapCW2 &
                availableRBBitMap;            
        }        
    }
    else
    {
        bitMapToBeReturned = subetSpanCQIInfo_p->rat1QPSKCQIBitMapCW2 &
            availableRBBitMap;
        if (!bitMapToBeReturned)
        {
            bitMapToBeReturned = subetSpanCQIInfo_p->rat164QAMCQIBitMapCW1 &
                availableRBBitMap;            
        }        
    }

    if (bitMapToBeReturned)
    { 
        if (MAC_SUCCESS == validateRAT1RBsforMinMaxTBSize(
                                        bitMapToBeReturned,
                                        resrcAllocatorInput_p,
                                        harqProcess_p,
                                        previousRsrcFrmt_p,
                                        dlStrategyNode_p,
                                        bitPositionToSubsetAndRBGInfo_p,
                                        subsetIndex,
                                        isNonZeroSpan,
                                        internalCellIndex))
        {
            return bitMapToBeReturned;
        }
    }   

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    bitMapToBeReturned = 0;
    return bitMapToBeReturned;

}

/******************************************************************************
 * Function Name    : getRAT1BitMapTBOneInValidTBTwoNewTx
 * Inputs           : rat1Info_p - This holds pointer to  RAT1Info structure,
 *                    resrcAllocatorInput_p - This holds pointer to ResourceAllocatorInput
 *                                            structure,
 *                    harqProcess_p - This Holds Pointer to DLHARQProcess structure,
 *                    dlStrategyNode_p - Pointer to DLStrategyTxNode,
 *                    centralRbMask,
 *                    scheduledTick - Scheduled TTI,
 *                    transmissionMode - Mode of Transmission
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : subsetIndex_p - This holds the  subset Index of the Subset 
 *                                    which has maximum number of FREE PRBs,
 *                    isSpanZeroConsideredForAlloc_p - This Indicates if the RBs
 *                                                     has been considered using Zero Span
 *                                                     Width or Non Zero Span width. 
 * Returns          : bitMapToBeReturned_p - This Holds the BitMap which we will get 
 *                                           after ANDing availableRBGBitMap with the
 *                                           subBand CQI BitMap.
 * Description      : This funtion will call relevant funtion for each subset ,
 *                    starting from the subset which has maximum number of
 *                    fee PRB .Relevant funtion will check if it can satify the 
 *                    the number of RBS which is need for Re-Transmitted TB
 *                    and return the relevant BitMap.
 ******************************************************************************/ 
STATIC UInt32 getRAT1BitMapTBOneInValidTBTwoNewTx( 
        RAT1Info *rat1Info_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc_p,
        DLStrategyTxNode *dlStrategyNode_p,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask[MAX_SUBSETS],
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 transmissionMode,
        InternalCellIndex internalCellIndex)
        /* - TM7_8 Changes End */
{
/*  SubsetInfo *subsetInfo_p = PNULL; TDD Warning Fix */
    UInt32 subsetCount = 0;
    SubSetSpanInfoForSorting subsetInfoForSorting[MAX_SUBSETS] = {{0,0,0 ,0}} ;
    RAT1SpatialMulCQIInfo *cqiInfo_p = PNULL;
    UInt32 subsetIndex= 0;

    UInt32 bitMapToBeReturned = 0;
    ResourceFormatNode previousRsrcFrmt = {0};
    SubbandPMIInfo *subbandPMIInfo_p = PNULL;
    UInt32 pmiIndex = WIDEBAND_PMI;
	UInt32 isNonZeroSpanConsidered = 0;

    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *ueContext_p = PNULL;
    dlUEContextInfo_p = &dlUECtxInfoArr_g[dlStrategyNode_p ->ueIndex];
    ueContext_p = dlUEContextInfo_p->dlUEContext_p;
 
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
/*  subsetInfo_p = rat1Info_p->subsetInfo; TDD Warning Fix */
    subsetCount = rat1Info_p->subsetCount; 

    CQIInfoSpatialMul *cqiSpatial_p = 
        &(resrcAllocatorInput_p->subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul);
    
    cqiInfo_p = cqiSpatial_p->rat1SpatialMulCQIInfo;   
    subbandPMIInfo_p = &(cqiSpatial_p->pmiInfo);    

    /* ICIC changes start */
    sortSubsetBasedOnCQIForSpatialMul(rat1Info_p,
            resrcAllocatorInput_p->subBandReportInfo_p, subsetInfoForSorting,
            resrcAllocatorInput_p->resourceRegionAllocated,
             /* + TM7_8 Changes Start */
            centralRbMask,
            transmissionMode,
            scheduledTick);
            /* - TM7_8 Changes End */
    /* ICIC changes end */

    /*Below While Loop is called for each subsetIndex starting from subset 
     *which has maximum number of FREE RB taking consideration of Left 
     *and Right Span.For each iteration it call relevant funtion which will 
     *check if it can satisfy the required RB or not*.On Success it return 
     *the bitMapToBeReturned_p with nonZero Value else it will return 
     *Zero Value*/
    while ( subsetCount-- )
    {
        subsetIndex = subsetInfoForSorting[subsetCount].subsetIndex;
        if (subsetInfoForSorting[subsetCount].isNonZeroSpanConsideredForAlloc)
        {
            /*Making isNonZeroSpanConsideredForAlloc to true which will be used
             * while forming resource assignment BitMap to indicate whether RBs
             * are allocated from using Zero Span Width  or RBs are allocated 
             * using None Span Width in the Subset Indexed by maxPRBSubsetIndex.
             * If Set to one this means RBS are allocated using None Zero Span
             * Width else RBs are allocated using  Zero Span Width*/

            isNonZeroSpanConsidered = 1;

            /*select PMI subset*/
            if (resrcAllocatorInput_p->subbandPMI)
            {
                if(2 == ueContext_p->dlMIMOInfo.dlRi)
                {
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank1[0] =
                     subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank2[0];
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank1[2] =
                     subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank2[1];
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank1[1] = 0;
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank1[3] = 0;
                }
                pmiIndex = selectRAT1SubbandPMIIndexRank1(
                                 subsetInfoForSorting[subsetCount].availableRBBitMap, 
                                 &(subbandPMIInfo_p->rat1PmiInfo[subsetCount]),
                                 &(subsetInfoForSorting[subsetCount].availableRBBitMap),
                                 1 /*NonZero Span*/
                                 ,internalCellIndex);

                harqProcess_p->pmiIndex = pmiIndex;
            }

            bitMapToBeReturned = getSubsetBitMapTBOneInValidTBTwoNewTx(
                                    harqProcess_p,
                                    subsetInfoForSorting[subsetCount].availableRBBitMap,                    
                                    &cqiInfo_p[subsetIndex].subsetNonZeroSpanCQIInfo,                                  
                                    resrcAllocatorInput_p,
                                    rat1Info_p->subsetInfo[subsetIndex].bitPositionToSubsetAndRBGInfo,
                                    dlStrategyNode_p,
                                    &previousRsrcFrmt,
									subsetIndex,
									isNonZeroSpanConsidered,
                                    internalCellIndex);
            if (bitMapToBeReturned)
            {
				*isNonZeroSpanConsideredForAlloc_p = isNonZeroSpanConsidered;
                *subsetIndex_p = subsetIndex;
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion Success  = %s\n",__func__);
                return bitMapToBeReturned;    
            }        
        }
        else
        {
            /*Making isNonZeroSpanConsideredForAlloc to false which will be used
             * while forming resource assignment BitMap to indicate whether RBs
             * are allocated from using Zero Span Width  or RBs are allocated 
             * using None Span Width in the Subset Indexed by maxPRBSubsetIndex.
             * If Set to one this means RBS are allocated using None Zero Span
             * Width else RBs are allocated using  Zero Span Width*/

            isNonZeroSpanConsidered = 0;        
            /*select PMI subset*/
            if (resrcAllocatorInput_p->subbandPMI)
            {
                if(2 == ueContext_p->dlMIMOInfo.dlRi)
                {
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanZeroPmiInfo.reportedPRBsRank1[0] =
                     subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanZeroPmiInfo.reportedPRBsRank2[0];
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanZeroPmiInfo.reportedPRBsRank1[2] =
                     subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanZeroPmiInfo.reportedPRBsRank2[1];
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank1[1] = 0;
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank1[3] = 0;
                }
                pmiIndex = selectRAT1SubbandPMIIndexRank1(
                                 subsetInfoForSorting[subsetCount].availableRBBitMap, 
                                 &(subbandPMIInfo_p->rat1PmiInfo[subsetCount]),
                                 &(subsetInfoForSorting[subsetCount].availableRBBitMap),
                                 0 /*Zero Span*/
                                 ,internalCellIndex);

                harqProcess_p->pmiIndex = pmiIndex;
            }
            
            bitMapToBeReturned = getSubsetBitMapTBOneInValidTBTwoNewTx(
                                    harqProcess_p,
                                    subsetInfoForSorting[subsetCount].availableRBBitMap,                                    
                                    &cqiInfo_p[subsetIndex].subsetZeroSpanCQIInfo,                    
                                    resrcAllocatorInput_p,
                                    rat1Info_p->subsetInfo[subsetIndex].bitPositionToSubsetAndRBGInfo,
                                    dlStrategyNode_p,
                                    &previousRsrcFrmt,
									subsetIndex,
									isNonZeroSpanConsidered,
                                    internalCellIndex);
            
            if (bitMapToBeReturned)
            {
				*isNonZeroSpanConsideredForAlloc_p = isNonZeroSpanConsidered;
                *subsetIndex_p = subsetIndex;
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion Success  = %s\n",__func__);
                return bitMapToBeReturned;     
            } 
        }

    } 
    bitMapToBeReturned = 0;
    /*It means that we are not able to meet minTBSize asked by strategy hence if strict subband allocation
           is required then we will schedule this UE with less than minimum TB Size*/
    if (!dlStrategyNode_p->strictAllocatedFlag)
    {
        /* As of now, there is no use case for strictAllocSubBand
         * strictAllocSubBand check is commented */
        //if (resrcAllocatorInput_p->strictAllocSubBand)
        //{
        if(previousRsrcFrmt.tbSize >= HARQ_TB_MINIMUM_SIZE_IN_BYTES)
        {
            harqProcess_p->assignedResourcesNB = previousRsrcFrmt.prbs;
            harqProcess_p->dlHARQTBTwoInfo.mcsIndex = previousRsrcFrmt.mcsIndex;    
            harqProcess_p->dlHARQTBTwoInfo.tbSize = previousRsrcFrmt.tbSize;
            bitMapToBeReturned = previousRsrcFrmt.cqiRBGBitmap;
            harqProcess_p->pmiIndex = previousRsrcFrmt.pmiIndex;
			*isNonZeroSpanConsideredForAlloc_p = previousRsrcFrmt.subsetIndex;
            *subsetIndex_p = previousRsrcFrmt.isNonZeroSpan;

            //update excess PRBs in buffer pool
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resrcAllocatorInput_p->resourceRegionAllocated, 
                    (resrcAllocatorInput_p->requiredRB - 
                     harqProcess_p->assignedResourcesNB), internalCellIndex);
            /* ICIC changes end */
        }
        //}
      
    }    

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion with Failure = %s\n",__func__);
    return bitMapToBeReturned;
}

/******************************************************************************
 * Function Name    : getSubsetBitMapTBOneInValidTBTwoNewTx
 * Inputs           : harqProcess_p -  This holds pointer to DLHARQProcess structure,
 *                    availableRBBitMap -  This Holds Available RB BitMap,
 *                    subetSpanCQIInfo_p - This holds pointer to SubsetSpanCQIInfo 
 *                                         structure,
 *                    resrcAllocatorInput_p - Pointer to passed ResourceAllocatorInput,
 *                    bitPositionToSubsetAndRBGInfo_p - This holds pointer to 
 *                                                      BitPositionToSubsetAndRBGInfo structure,
 *                    dlStrategyNode_p - Pointer to downlink strategy node,
 *                    previousRsrcFrmt_p- pointer to ResourceFormatNode for storing tbsize, prbs
 *                                        if minTBSize is not met,
 *                    subsetIndex - iNDEX OF sUBSET,
 *                    isZeroNonZeroSpan
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : bitMapToBeReturned_p - This Holds the BitMap which we will get 
 *                                           after ANDing availableRBGBitMap with the 
 *                                           subBand CQI BitMap. 
 * Description      : This funtion will first check if the 64QAM Bands can satisfy 
 *                    the required Number of RBs.If successful ,it will populate 
 *                    relevant information in harqProcess_p structure else it will
 *                    go for 16QAM Bands.If 16QAM also don't satisfy the required
 *                    Number of RBs ,it will repeat logic for QPSK bands.
 ******************************************************************************/ 
STATIC  UInt32 getSubsetBitMapTBOneInValidTBTwoNewTx(
        DLHARQProcess  *harqProcess_p,
        UInt32 availableRBBitMap,
        SubsetSpanCQIInfo *subetSpanCQIInfo_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        BitPositionToSubsetAndRBGInfo *bitPositionToSubsetAndRBGInfo_p,
        DLStrategyTxNode *dlStrategyNode_p,
        ResourceFormatNode *previousRsrcFrmt_p,
		UInt32 subsetIndex,
		UInt32 isZeroNonZeroSpan,
        InternalCellIndex internalCellIndex)
{
#define MOD_SCHEME_CNT 3

    DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL;
    UInt32 availableRBCount = 0;

    UInt32 bitMapToBeReturned = 0;
    
    UInt32 freqSelectiveMCSIndex = 0;
    UInt32 freqSelectiveTBSize = 0;

    SubBandReportInfo *subBandReportInfo_p = resrcAllocatorInput_p->subBandReportInfo_p;
    UInt32 requiredRBCount = resrcAllocatorInput_p->requiredRB;

    UInt32 cntModScheme = MOD_SCHEME_CNT;

    UInt32 modSchemeAndAvailableBitmap[MOD_SCHEME_CNT] = {0};

    UInt32 minTBSize = dlStrategyNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyNode_p->maxTBSize;
    
    /* ICIC changes start */ 
    UInt32 resourceRegionAllocated =  
        resrcAllocatorInput_p->resourceRegionAllocated;
    /* ICIC changes end */ 

    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *ueContext_p = PNULL;
    dlUEContextInfo_p = &dlUECtxInfoArr_g[dlStrategyNode_p->ueIndex];
    ueContext_p = dlUEContextInfo_p->dlUEContext_p;
#endif
    /* HD FDD Changes End */

    if (maxTBSize < minTBSize)
    {
        lteWarning("[RAT1 Spatial TB1 Invalid TB2 New TX] Wrong Max TBSize calculated by\
                    DL Strategy minTBSize [%d] maxTBSize [%d]\n", minTBSize, maxTBSize);
        maxTBSize = minTBSize;
    }       

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    
    dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo;    
    
    modSchemeAndAvailableBitmap[0] = 
        subetSpanCQIInfo_p->rat1QPSKCQIBitMapCW2 & availableRBBitMap;
    modSchemeAndAvailableBitmap[1] = 
        subetSpanCQIInfo_p->rat116QAMCQIBitMapCW2 & availableRBBitMap;
    modSchemeAndAvailableBitmap[2] = 
        subetSpanCQIInfo_p->rat164QAMCQIBitMapCW2 & availableRBBitMap;      

    while (cntModScheme--)
    {
        if ((bitMapToBeReturned = modSchemeAndAvailableBitmap[cntModScheme]))
        {   
            /*Get Available RB Count */
            availableRBCount = getCount(bitMapToBeReturned);
            
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "availableRBCount = %u\n",availableRBCount);

            if (availableRBCount >= requiredRBCount)
            {
                /*Get Mcs Index of CodeWord Two*/
                getMcsIndexForCW2ForRAT1(&freqSelectiveMCSIndex, 
                                         bitMapToBeReturned,
                                         subBandReportInfo_p, 
                                         bitPositionToSubsetAndRBGInfo_p,
                                         requiredRBCount);

                /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndex);
#endif
                /* HD FDD Changes End */

                freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex,
                                                      requiredRBCount,
                                                       /*4X4 DL MIMO CHG START*/
                                                       TB_MAPPED_TO_ONE_LAYER
                                                        /*4X4 DL MIMO CHG END*/);
 
                /*If TB size on this mcs with requiredRBCount is greater than minimum TB size then 
                              we can go for maximum TB size too if excess Pool has some RBs left. This case means successful as 
                              minimum TB size is always met*/         
                if (freqSelectiveTBSize >= minTBSize)
                {
                   if (freqSelectiveTBSize >= maxTBSize)
                   {
                       freqSelectiveTBSize = maxTBSize;
                       /* SPR 5999 changes start */
                       harqProcess_p->assignedResourcesNB = DETERMINE_RBs_DL(freqSelectiveMCSIndex,
                                                                             freqSelectiveTBSize,
                                                                             internalCellIndex);                    
                       /* SPR 5999 changes end */
                   }
                   else
                   {
                       harqProcess_p->assignedResourcesNB = requiredRBCount;
                   }
                   
                   dlHARQTBTwoInfo_p->mcsIndex = freqSelectiveMCSIndex;
                   dlHARQTBTwoInfo_p->tbSize = freqSelectiveTBSize;

                   //update excess PRBs in buffer pool
                   /* ICIC changes start */
                   UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                           (requiredRBCount - harqProcess_p->assignedResourcesNB), internalCellIndex);
                   /* ICIC changes end */
                   return bitMapToBeReturned;
                }
                else
                {
                   /*Here allocation can be done only if some RBs are left in excessPool which allows TBSize to be greater
                                     than or equal to minimum TB Size. If not possible then store this TBSize and found RBs as later on it will
                                     required to allocate if strict subband allocation is required*/

                   /*Get Mcs Index of CodeWord Two*/
                    getMcsIndexForCW2ForRAT1(&freqSelectiveMCSIndex, 
                                             bitMapToBeReturned,
                                             subBandReportInfo_p, 
                                             bitPositionToSubsetAndRBGInfo_p,
                                             availableRBCount);                 
                    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                    HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndex);
#endif
                    /* HD FDD Changes End */

                      
                   UInt32 requiredRBsFromPool = 0;
                   if (excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0)
                   {
                       requiredRBsFromPool = allocatePRBsUsingExcessPool(freqSelectiveMCSIndex,
                                                                         availableRBCount,
                                                                         minTBSize,
                                                                         requiredRBCount,
                                                                         resourceRegionAllocated,
                                                                         internalCellIndex);
                       
                       freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex, 
                                                             requiredRBCount+ requiredRBsFromPool,
                                                              /*4X4 DL MIMO CHG START*/
                                                              TB_MAPPED_TO_ONE_LAYER
                                                               /*4X4 DL MIMO CHG END*/);
 
                       if (freqSelectiveTBSize >= minTBSize)
                       {
                           dlHARQTBTwoInfo_p->mcsIndex = freqSelectiveMCSIndex;
                           harqProcess_p->assignedResourcesNB = requiredRBCount + requiredRBsFromPool; 
                           dlHARQTBTwoInfo_p->tbSize = freqSelectiveTBSize;

                           //update excess PRBs in buffer pool
                           /* ICIC changes start */
                           UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                   (requiredRBCount - harqProcess_p->assignedResourcesNB),
                                   internalCellIndex);
                           /* ICIC changes end */
                   
                           return bitMapToBeReturned;                   
                       }
                   }
                   
                   if(previousRsrcFrmt_p->tbSize < freqSelectiveTBSize)
                   {
                       previousRsrcFrmt_p->tbSize = freqSelectiveTBSize;
                       previousRsrcFrmt_p->mcsIndex = freqSelectiveMCSIndex;
                       previousRsrcFrmt_p->prbs = requiredRBCount+ requiredRBsFromPool;
                       previousRsrcFrmt_p->cqiRBGBitmap = bitMapToBeReturned;
                       previousRsrcFrmt_p->pmiIndex = harqProcess_p->pmiIndex;                       
					   previousRsrcFrmt_p->subsetIndex = subsetIndex;
					   previousRsrcFrmt_p->isNonZeroSpan = isZeroNonZeroSpan; 
                   }               
               }
           }
           else 
           {
                /*Get Mcs Index of CodeWord Two*/
                getMcsIndexForCW2ForRAT1(&freqSelectiveMCSIndex, 
                                         bitMapToBeReturned,
                                         subBandReportInfo_p, 
                                         bitPositionToSubsetAndRBGInfo_p,
                                         availableRBCount);   

                /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndex);
#endif
                /* HD FDD Changes End */

                /* Here although available RBs are less than required RBs but still allocation can be done as
                                reported MCS can be greater than wideband MCS used by strategy hence min/max TB size can be
                                achieved with available RBs*/
                freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex,
                                                     availableRBCount,
                                                     /*4X4 DL MIMO CHG START*/
                                                     TB_MAPPED_TO_ONE_LAYER
                                                     /*4X4 DL MIMO CHG END*/);
                if (freqSelectiveTBSize >= minTBSize)
                {
                   if (freqSelectiveTBSize > maxTBSize)
                   {
                       freqSelectiveTBSize = maxTBSize;
                   }
                   
                   dlHARQTBTwoInfo_p->mcsIndex = freqSelectiveMCSIndex;                
                   /* SPR 5999 changes start */
                   harqProcess_p->assignedResourcesNB = DETERMINE_RBs_DL(freqSelectiveMCSIndex,
                                                                         freqSelectiveTBSize,
                                                                         internalCellIndex);
                   /* SPR 5999 changes end */
                   dlHARQTBTwoInfo_p->tbSize = freqSelectiveTBSize;
                   //update excess PRBs in buffer pool
                   /* ICIC changes start */
                   UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                           (requiredRBCount - harqProcess_p->assignedResourcesNB),internalCellIndex);
                   /* ICIC changes end */
                   return bitMapToBeReturned;
                }
                else
                {
                   if(previousRsrcFrmt_p->tbSize < freqSelectiveTBSize)
                   {
                       previousRsrcFrmt_p->tbSize = freqSelectiveTBSize;
                       previousRsrcFrmt_p->mcsIndex = freqSelectiveMCSIndex;
                       previousRsrcFrmt_p->prbs = availableRBCount;
                       previousRsrcFrmt_p->cqiRBGBitmap = bitMapToBeReturned;
                       previousRsrcFrmt_p->pmiIndex = harqProcess_p->pmiIndex;                       
					   previousRsrcFrmt_p->subsetIndex = subsetIndex;
					   previousRsrcFrmt_p->isNonZeroSpan = isZeroNonZeroSpan; 
                   }
                }        
           }     
        }
    }

    bitMapToBeReturned = 0;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    return bitMapToBeReturned;
}

/******************************************************************************
 * Function Name    : getRAT1BitMapTBOneNewTxTBTwoInValid
 * Inputs           : rat1Info_p - This holds pointer to  RAT1Info structure,
 *                    resrcAllocatorInput_p - This holds pointer to ResourceAllocatorInput
 *                                            structure,
 *                    harqProcess_p - This Holds Pointer to DLHARQProcess structure,
 *                    dlStrategyNode_p - Pointer to DLStrategyTxNode,
 *                    centralRbMask,
 *                    scheduledTick - Scheduled TTI,
 *                    transmissionMode - Mode of Transmission
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : subsetIndex_p - This holds the  subset Index of the Subset 
 *                                    which has maximum number of FREE PRBs,
 *                    isSpanZeroConsideredForAlloc_p - This Indicates if the RBs
 *                                                     has been considered using Zero Span Width
 *                                                     or Non Zero Span width. 
 * Returns          : bitMapToBeReturned_p - This Holds the BitMap which we will get 
 *                                           after ANDing availableRBGBitMap with the 
 *                                           subBand CQI BitMap.
 * Description      : This funtion will first get subset Index of Subset which has 
 *                    Maximum number of FREE PRBs and then it will call relevant 
 *                    funtions for returning the available RB Bit.
 ******************************************************************************/ 
STATIC UInt32 getRAT1BitMapTBOneNewTxTBTwoInValid( 
        RAT1Info *rat1Info_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc_p,
        DLStrategyTxNode *dlStrategyNode_p,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask[MAX_SUBSETS],
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 transmissionMode,
        InternalCellIndex internalCellIndex)
        /* - TM7_8 Changes End */
{
/*  SubsetInfo *subsetInfo_p = PNULL; TDD Warning Fix */
    UInt32 subsetCount = 0;
    SubSetSpanInfoForSorting subsetInfoForSorting[MAX_SUBSETS] = {{0,0,0 ,0}} ;
    RAT1SpatialMulCQIInfo *cqiInfo_p = PNULL;
    UInt32 subsetIndex= 0;
    SubbandPMIInfo *subbandPMIInfo_p = PNULL;
    UInt32 isNonZeroSpanConsidered =0;

    UInt32 bitMapToBeReturned = 0;
    ResourceFormatNode previousRsrcFrmt = {0};
    UInt32 pmiIndex = WIDEBAND_PMI;
   
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *ueContext_p = PNULL;
    dlUEContextInfo_p = &dlUECtxInfoArr_g[dlStrategyNode_p ->ueIndex];
    ueContext_p = dlUEContextInfo_p->dlUEContext_p;
 
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR, "Entering funtion = %s\n",__func__);
    
/*  subsetInfo_p = rat1Info_p->subsetInfo; TDD Warning Fix */
    subsetCount = rat1Info_p->subsetCount;   
    CQIInfoSpatialMul *cqiSpatial_p = 
        &(resrcAllocatorInput_p->subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul);
    
    cqiInfo_p = cqiSpatial_p->rat1SpatialMulCQIInfo;   
    subbandPMIInfo_p = &(cqiSpatial_p->pmiInfo);

    /* ICIC changes start */
    sortSubsetBasedOnCQIForSpatialMul(rat1Info_p,
            resrcAllocatorInput_p->subBandReportInfo_p, subsetInfoForSorting,
            resrcAllocatorInput_p->resourceRegionAllocated,
            /* + TM7_8 Changes Start */
            centralRbMask,
            transmissionMode,
            scheduledTick);
           /* - TM7_8 Changes End */
 /* ICIC changes end */

    /*Below While Loop is called for each subsetIndex starting from subset 
     *which has maximum number of FREE RB taking consideration of Left 
     *and Right Span.For each iteration it call relevant funtion which will 
     *check if it can satisfy the required RB or not*.On Success it return 
     *the bitMapToBeReturned_p with nonZero Value else it will return 
     *Zero Value*/
    while ( subsetCount-- )
    {
        subsetIndex = subsetInfoForSorting[subsetCount].subsetIndex;
        if (subsetInfoForSorting[subsetCount].isNonZeroSpanConsideredForAlloc)
        {
            /*Making isNonZeroSpanConsideredForAlloc to true which will be used
             * while forming resource assignment BitMap to indicate whether RBs
             * are allocated from using Zero Span Width  or RBs are allocated 
             * using None Span Width in the Subset Indexed by maxPRBSubsetIndex.
             * If Set to one this means RBS are allocated using None Zero Span
             * Width else RBs are allocated using  Zero Span Width*/

            isNonZeroSpanConsidered = 1;
            /*select PMI subset*/
            if (resrcAllocatorInput_p->subbandPMI)
            {
                if(2 == ueContext_p->dlMIMOInfo.dlRi)
                {
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank1[0] =
                     subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank2[0];
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank1[2] =
                     subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank2[1];
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank1[1] = 0;
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank1[3] = 0;

                }
                pmiIndex = selectRAT1SubbandPMIIndexRank1(
                                 subsetInfoForSorting[subsetCount].availableRBBitMap, 
                                 &(subbandPMIInfo_p->rat1PmiInfo[subsetCount]),
                                 &(subsetInfoForSorting[subsetCount].availableRBBitMap),
                                 1 /*NonZero Span*/
                                 ,internalCellIndex);

                harqProcess_p->pmiIndex = pmiIndex;
            }
            
            bitMapToBeReturned = getSubsetBitMapTBOneNewTxTBTwoInValid(
                                    harqProcess_p,
                                    subsetInfoForSorting[subsetCount].availableRBBitMap,                                    
                                    &cqiInfo_p[subsetIndex].subsetNonZeroSpanCQIInfo,                                   
                                    resrcAllocatorInput_p,
                                    rat1Info_p->subsetInfo[subsetIndex].bitPositionToSubsetAndRBGInfo,
                                    dlStrategyNode_p,
                                    &previousRsrcFrmt,isNonZeroSpanConsidered,subsetIndex,
                                    internalCellIndex);
            if (bitMapToBeReturned)
            {
				*isNonZeroSpanConsideredForAlloc_p = isNonZeroSpanConsidered; 
                *subsetIndex_p = subsetIndex;
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion Success  = %s\n",__func__);
                return bitMapToBeReturned;    
            }        
        }
        else
        {
            /*Making isNonZeroSpanConsideredForAlloc to false which will be used
             * while forming resource assignment BitMap to indicate whether RBs
             * are allocated from using Zero Span Width  or RBs are allocated 
             * using None Span Width in the Subset Indexed by maxPRBSubsetIndex.
             * If Set to one this means RBS are allocated using None Zero Span
             * Width else RBs are allocated using  Zero Span Width*/

            isNonZeroSpanConsidered = 0;        
            /*select PMI subset*/
            if (resrcAllocatorInput_p->subbandPMI)
            {
                if(2 == ueContext_p->dlMIMOInfo.dlRi)
                {
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanZeroPmiInfo.reportedPRBsRank1[0] =
                     subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanZeroPmiInfo.reportedPRBsRank2[0];
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanZeroPmiInfo.reportedPRBsRank1[2] =
                     subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanZeroPmiInfo.reportedPRBsRank2[1];
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank1[1] = 0;
                    subbandPMIInfo_p->rat1PmiInfo[subsetCount].
                                      spanNonZeroPmiInfo.reportedPRBsRank1[3] = 0;
                }
                pmiIndex = selectRAT1SubbandPMIIndexRank1(
                                 subsetInfoForSorting[subsetCount].availableRBBitMap, 
                                 &(subbandPMIInfo_p->rat1PmiInfo[subsetCount]),
                                 &(subsetInfoForSorting[subsetCount].availableRBBitMap),
                                 0 /*Zero Span*/
                                 ,internalCellIndex);

                harqProcess_p->pmiIndex = pmiIndex;
            }    
            
            bitMapToBeReturned = getSubsetBitMapTBOneNewTxTBTwoInValid(
                                    harqProcess_p,
                                    subsetInfoForSorting[subsetCount].availableRBBitMap,                                    
                                    &cqiInfo_p[subsetIndex].subsetZeroSpanCQIInfo,                                    
                                    resrcAllocatorInput_p,
                                    rat1Info_p->subsetInfo[subsetIndex].bitPositionToSubsetAndRBGInfo,
                                    dlStrategyNode_p,
                                    &previousRsrcFrmt,isNonZeroSpanConsidered,subsetIndex,
                                    internalCellIndex);
            if (bitMapToBeReturned)
            {
				*isNonZeroSpanConsideredForAlloc_p = isNonZeroSpanConsidered;
                *subsetIndex_p = subsetIndex;
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion Success  = %s\n",__func__);
                return bitMapToBeReturned;     
            } 
        }

    } 
    /*It means that we are not able to meet minTBSize asked by strategy hence if strict subband allocation
           is required then we will schedule this UE with less than minimum TB Size*/
    if (!dlStrategyNode_p->strictAllocatedFlag)
    {
        /* As of now, there is no use case for strictAllocSubBand
         * strictAllocSubBand check is commented */
        //if (resrcAllocatorInput_p->strictAllocSubBand)
        //{
        if(previousRsrcFrmt.tbSize >= HARQ_TB_MINIMUM_SIZE_IN_BYTES)
        {
            harqProcess_p->assignedResourcesNB = previousRsrcFrmt.prbs;
            harqProcess_p->dlHARQTBOneInfo.mcsIndex = previousRsrcFrmt.mcsIndex;    
            harqProcess_p->dlHARQTBOneInfo.tbSize = previousRsrcFrmt.tbSize;
            bitMapToBeReturned = previousRsrcFrmt.cqiRBGBitmap;

            harqProcess_p->pmiIndex = previousRsrcFrmt.pmiIndex;
            *isNonZeroSpanConsideredForAlloc_p = previousRsrcFrmt.isNonZeroSpan;
            *subsetIndex_p = previousRsrcFrmt.subsetIndex;

            //update excess PRBs in buffer pool
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resrcAllocatorInput_p->resourceRegionAllocated, 
                    (resrcAllocatorInput_p->requiredRB - 
                     harqProcess_p->assignedResourcesNB),internalCellIndex);
            /* ICIC changes end */
        }
        //}
      
    }        

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion with Failure = %s\n",__func__);
    return bitMapToBeReturned;
}

/******************************************************************************
 * Function Name    : getSubsetBitMapTBOneNewTxTBTwoInValid
 * Inputs           : harqProcess_p -  This holds pointer to DLHARQProcess structure,
 *                    availableRBBitMap -  This Holds Available RB BitMap,
 *                    subetSpanCQIInfo_p - This holds pointer to SubsetSpanCQIInfo 
 *                                         structure,
 *                    resrcAllocatorInput_p - Pointer to passed ResourceAllocatorInput,
 *                    bitPositionToSubsetAndRBGInfo_p - This holds pointer to 
 *                                                      BitPositionToSubsetAndRBGInfo
 *                                                      structure,
 *                    dlStrategyNode_p - Pointer to downlink strategy node,
 *                    previousRsrcFrmt_p- pointer to ResourceFormatNode for storing tbsize,
 *                                       prbs if minTBSize is not met,
 *                    isNonZeroSpan,
 *                    subsetIndex
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : bitMapToBeReturned_p - This Holds the BitMap which we will get 
 *                                           after ANDing availableRBGBitMap with the
 *                                           subBand CQI BitMap
 * Description      : This funtion will first check if the 64QAM Bands can satisfy 
 *                    the required Number of RBs.If successful ,it will populate 
 *                    relevant information in harqProcess_p structure else it will
 *                    go for 16QAM Bands.If 16QAM also don't satisfy the required
 *                    Number of RBs, it will repeat logic for QPSK bands.
 ******************************************************************************/ 
STATIC  UInt32 getSubsetBitMapTBOneNewTxTBTwoInValid(
        DLHARQProcess  *harqProcess_p,
        UInt32 availableRBBitMap,
        SubsetSpanCQIInfo *subetSpanCQIInfo_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        BitPositionToSubsetAndRBGInfo *bitPositionToSubsetAndRBGInfo_p,
        DLStrategyTxNode *dlStrategyNode_p,
        ResourceFormatNode *previousRsrcFrmt_p,
        UInt32 isNonZeroSpan,
        UInt32 subsetIndex,
        InternalCellIndex internalCellIndex)
{

    DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
    UInt32 availableRBCount = 0;
    
    UInt32 bitMapToBeReturned = 0;
    
    UInt32 freqSelectiveMCSIndex = 0;
    UInt32 freqSelectiveTBSize = 0;

    SubBandReportInfo *subBandReportInfo_p = resrcAllocatorInput_p->subBandReportInfo_p;
    UInt32 requiredRBCount = resrcAllocatorInput_p->requiredRB;

    UInt32 cntModScheme = MOD_SCHEME_CNT;

    UInt32 modSchemeAndAvailableBitmap[MOD_SCHEME_CNT] = {0};

    UInt32 minTBSize = dlStrategyNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyNode_p->maxTBSize; 
    /* ICIC changes start */
    UInt32 resourceRegionAllocated = 
        resrcAllocatorInput_p->resourceRegionAllocated;
    /* ICIC changes end */

    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *ueContext_p = PNULL;
    dlUEContextInfo_p = &dlUECtxInfoArr_g[dlStrategyNode_p->ueIndex];
    ueContext_p = dlUEContextInfo_p->dlUEContext_p;
#endif
    /* HD FDD Changes End */

    if (maxTBSize < minTBSize)
    {
        lteWarning("[RAT1 Spatial TB1 New TB2 Invalid] Wrong Max TBSize calculated by\
                     DL Strategy minTBSize [%d] maxTBSize [%d]\n", minTBSize, maxTBSize);
        maxTBSize = minTBSize;
    }       

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;  

    modSchemeAndAvailableBitmap[0] = 
        subetSpanCQIInfo_p->rat1QPSKCQIBitMapCW1 & availableRBBitMap;
    modSchemeAndAvailableBitmap[1] = 
        subetSpanCQIInfo_p->rat116QAMCQIBitMapCW1 & availableRBBitMap;
    modSchemeAndAvailableBitmap[2] = 
        subetSpanCQIInfo_p->rat164QAMCQIBitMapCW1 & availableRBBitMap;      

    while (cntModScheme--)
    {
        if ((bitMapToBeReturned = modSchemeAndAvailableBitmap[cntModScheme]))
        {   
            /*Get Available RB Count */
            availableRBCount = getCount(bitMapToBeReturned);
            
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "availableRBCount = %u\n",availableRBCount);

            if (availableRBCount >= requiredRBCount)
            {
                 /*Get Mcs Index of CodeWord One*/
                 getMcsIndexForCW1ForRAT1(&freqSelectiveMCSIndex, 
                                          bitMapToBeReturned,
                                          subBandReportInfo_p, 
                                          bitPositionToSubsetAndRBGInfo_p,
                                          requiredRBCount);
                
                 /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                 HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndex);
#endif
                 /* HD FDD Changes End */

                 freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex,
                                                       requiredRBCount,
                                                         /*4X4 DL MIMO CHG START*/
                                                         TB_MAPPED_TO_ONE_LAYER
                                                         /*4X4 DL MIMO CHG END*/);

                 /*If TB size on this mcs with requiredRBCount is greater than minimum TB size then 
                               we can go for maximum TB size too if excess Pool has some RBs left. This case means successful as 
                               minimum TB size is always met*/         
                 if (freqSelectiveTBSize >= minTBSize)
                 {
                    if (freqSelectiveTBSize >= maxTBSize)
                    {
                        freqSelectiveTBSize = maxTBSize;
                        /* SPR 5999 changes start */
                        harqProcess_p->assignedResourcesNB = DETERMINE_RBs_DL(freqSelectiveMCSIndex,
                                                                              freqSelectiveTBSize,
                                                                              internalCellIndex);                    
                        /* SPR 5999 changes end */
                    }
                    else
                    {
                             harqProcess_p->assignedResourcesNB = requiredRBCount;
                    }
                    
                    dlHARQTBOneInfo_p->mcsIndex = freqSelectiveMCSIndex;
                    dlHARQTBOneInfo_p->tbSize = freqSelectiveTBSize;
                
                    //update excess PRBs in buffer pool
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            (requiredRBCount - harqProcess_p->assignedResourcesNB), internalCellIndex);
                    /* ICIC changes end */
                    return bitMapToBeReturned;
                 }
                 else
                 {
                    /*Here allocation can be done only if some RBs are left in excessPool which allows TBSize to be greater
                                      than or equal to minimum TB Size. If not possible then store this TBSize and found RBs as later on it will
                                      required to allocate if strict subband allocation is required*/
                
                    UInt32 requiredRBsFromPool = 0;
                    if (excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0)
                    {

                     /*Get Mcs Index of CodeWord One*/
                     getMcsIndexForCW1ForRAT1(&freqSelectiveMCSIndex, 
                                              bitMapToBeReturned,
                                              subBandReportInfo_p, 
                                              bitPositionToSubsetAndRBGInfo_p,
                                              availableRBCount);                 

                     /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                     HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndex);
#endif
                     /* HD FDD Changes End */

                        requiredRBsFromPool = allocatePRBsUsingExcessPool(freqSelectiveMCSIndex,
                                                                          availableRBCount,
                                                                          minTBSize,
                                                                          requiredRBCount,
                                                                          resourceRegionAllocated,
                                                                          internalCellIndex);
                        
                        freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex, 
                                                              requiredRBCount+ requiredRBsFromPool,
                                                              /*4X4 DL MIMO CHG START*/
                                                              TB_MAPPED_TO_ONE_LAYER
                                                               /*4X4 DL MIMO CHG END*/);
 
                        if (freqSelectiveTBSize >= minTBSize)
                        {
                            dlHARQTBOneInfo_p->mcsIndex = freqSelectiveMCSIndex;
                            harqProcess_p->assignedResourcesNB = requiredRBCount + requiredRBsFromPool; 
                            dlHARQTBOneInfo_p->tbSize = freqSelectiveTBSize;
                
                            //update excess PRBs in buffer pool
                            /* ICIC changes start */
                            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                    (requiredRBCount - harqProcess_p->assignedResourcesNB),
                                    internalCellIndex);
                            /* ICIC changes end */
                    
                            return bitMapToBeReturned;                   
                        }
                    }
                    
                    if(previousRsrcFrmt_p->tbSize < freqSelectiveTBSize)
                    {
                        previousRsrcFrmt_p->tbSize = freqSelectiveTBSize;
                        previousRsrcFrmt_p->mcsIndex = freqSelectiveMCSIndex;
                        previousRsrcFrmt_p->prbs = requiredRBCount+ requiredRBsFromPool;
                        previousRsrcFrmt_p->cqiRBGBitmap = bitMapToBeReturned;
                        previousRsrcFrmt_p->pmiIndex = harqProcess_p->pmiIndex;
                        previousRsrcFrmt_p->subsetIndex = subsetIndex;
                        previousRsrcFrmt_p->isNonZeroSpan = isNonZeroSpan;
                    }               
                }
            }
            else 
            {
                /*Get Mcs Index of CodeWord One*/
                getMcsIndexForCW1ForRAT1(&freqSelectiveMCSIndex, 
                                         bitMapToBeReturned,
                                         subBandReportInfo_p, 
                                         bitPositionToSubsetAndRBGInfo_p,
                                         availableRBCount);   

                /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndex);
#endif
                /* HD FDD Changes End */


                /* Here although available RBs are less than required RBs but still allocation can be done as
                                reported MCS can be greater than wideband MCS used by strategy hence min/max TB size can be
                                achieved with available RBs*/
                freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex,
                                                     availableRBCount,
                                                     /*4X4 DL MIMO CHG START*/
                                                     TB_MAPPED_TO_ONE_LAYER
                                                     /*4X4 DL MIMO CHG END*/); 
                if (freqSelectiveTBSize >= minTBSize)
                {
                   if (freqSelectiveTBSize > maxTBSize)
                   {
                       freqSelectiveTBSize = maxTBSize;
                   }
                   
                   dlHARQTBOneInfo_p->mcsIndex = freqSelectiveMCSIndex;                
                   /* SPR 5999 changes start */
                   harqProcess_p->assignedResourcesNB = DETERMINE_RBs_DL(freqSelectiveMCSIndex,
                                                                         freqSelectiveTBSize,
                                                                         internalCellIndex);
                   /* SPR 5999 changes end */
                   dlHARQTBOneInfo_p->tbSize = freqSelectiveTBSize;
                   //update excess PRBs in buffer pool
                   /* ICIC changes start */
                   UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                           (requiredRBCount - harqProcess_p->assignedResourcesNB),
                           internalCellIndex);
                   /* ICIC changes end */
                   return bitMapToBeReturned;
                }
                else
                {
                   if(previousRsrcFrmt_p->tbSize < freqSelectiveTBSize)
                   {
                       previousRsrcFrmt_p->tbSize = freqSelectiveTBSize;
                       previousRsrcFrmt_p->mcsIndex = freqSelectiveMCSIndex;
                       previousRsrcFrmt_p->prbs = availableRBCount;
                       previousRsrcFrmt_p->cqiRBGBitmap = bitMapToBeReturned;
                       previousRsrcFrmt_p->pmiIndex = harqProcess_p->pmiIndex;                       
                        previousRsrcFrmt_p->subsetIndex = subsetIndex;
                        previousRsrcFrmt_p->isNonZeroSpan = isNonZeroSpan;
                   }
                }        

            }     

        }
    }

     bitMapToBeReturned = 0;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__); 
    
    return bitMapToBeReturned;
}

/******************************************************************************
 * Function Name    : getRAT1BitMapSpatialMux
 * Inputs           : rat1Info_p - This holds pointer to  RAT1Info structure,
 *                    resrcAllocatorInput_p - This holds pointer to ResourceAllocatorInput
 *                                            structure,
 *                    harqProcess_p - This Holds Pointer to DLHARQProcess structure,
 *                    centralRbMask,
 *                    scheduledTick - Scheduled TTI,
 *                    transmissionMode - Mode of Transmission
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : subsetIndex_p - This holds the  subset Index of the Subset 
 *                                    which has maximum number of FREE PRBs,
 *                    isSpanZeroConsideredForAlloc_p - This Indicates if the RBs
 *                                                     has been considered using Zero Span
 *                                                     Width or Non Zero Span width
 * Returns          : bitMapToBeReturned - This Holds the BitMap which we will get 
 *                                         after ANDing availableRBGBitMap with the
 *                                         subBand CQI BitMap.
 * Description      : This funtion will call relavant funtions depending upon TB
 *                    Validity and also Based on  whether TB is Re-Transmission or TB
 *                    is New Transmission.
 ******************************************************************************/ 
STATIC  UInt32 getRAT1BitMapSpatialMux( 
        RAT1Info *rat1Info_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc_p,
        DLStrategyTxNode *dlStrategyNode_p,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask[MAX_SUBSETS],
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 transmissionMode,
        InternalCellIndex internalCellIndex)
        /* - TM7_8 Changes End */
{
/*    DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
    DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL; TDD Warning Fix */

    UInt32 bitMapToBeReturned = 0;

/*    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo; TDD Warning Fix */
    
    if ( TB_VALID == harqProcess_p->isTBOneValid && 
            TB_VALID == harqProcess_p->isTBTwoValid )
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "Both TB NEW_TX Case\n");
        bitMapToBeReturned = getRAT1BitMapBothTBNewTx(
                                    rat1Info_p,
                                    resrcAllocatorInput_p,
                                    harqProcess_p,
                                    subsetIndex_p,
                                    isNonZeroSpanConsideredForAlloc_p,
                                    dlStrategyNode_p,
                                    /* + TM7_8 Changes Start */
                                    centralRbMask,
                                    scheduledTick,
                                    transmissionMode,
                                    internalCellIndex);
                                    /* - TM7_8 Changes End */

}
    else if ( TB_VALID == harqProcess_p->isTBOneValid )
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "TB One New Tx \n");
        bitMapToBeReturned = getRAT1BitMapTBOneNewTxTBTwoInValid(
                                    rat1Info_p,
                                    resrcAllocatorInput_p,
                                    harqProcess_p,                                    
                                    subsetIndex_p,
                                    isNonZeroSpanConsideredForAlloc_p,
                                    dlStrategyNode_p,   
                                     /* + TM7_8 Changes Start */
                                    centralRbMask,
                                    scheduledTick,
                                    transmissionMode,
                                    internalCellIndex);
                                    /* - TM7_8 Changes End */

   }
    else if ( TB_VALID == harqProcess_p->isTBTwoValid )
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "TB Two New Tx \n");
        bitMapToBeReturned = getRAT1BitMapTBOneInValidTBTwoNewTx(
                                    rat1Info_p,
                                    resrcAllocatorInput_p,
                                    harqProcess_p,                                    
                                    subsetIndex_p,
                                    isNonZeroSpanConsideredForAlloc_p,
                                    dlStrategyNode_p,
                                      /* + TM7_8 Changes Start */
                                    centralRbMask,
                                    scheduledTick,
                                    transmissionMode,
                                    internalCellIndex);
                                    /* - TM7_8 Changes End */

   }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    return bitMapToBeReturned;
}

/******************************************************************************
 * Function Name    : getMaxPrbSubsetIndexForNonSpatialMul
 * Inputs           : rat1Info_p - This holds Pointer to RAT1Info structure,
 *                    resourceRegionAllocated - Allocated resource region
 * Outputs          : subsetIndex_p - This holds subset Index of Subset which has 
 *                                    maximum number of FREE PRB considering left
 *                                    or right Span,
 *                    isNonZeroSpanConsideredForAlloc - This will set to one if the 
 *                                                      if Non Zero Span has been chosen
 *                                                      for allocation of RBs else if
 *                                                      set to one this means Zero Span
 *                                                      has been chosen for allocation of RBs,
 *                    availableRBBitMap_p - This holds available RB BitMap after 
 *                                          considering left and right Span
 * Returns          : Count of number of set bits in availableRBBitMap_p
 * Description      : This funtion will find  the subset Index of Subset which has 
 *                    maximum number of FREE PRBs and also find if Non Zero Span
 *                    or Zero Span has been chosen for allocation of RBs alongwith 
 *                    the available RB BitMap for the choosed Span.
 ******************************************************************************/
/* + TM7_8 Changes Start */
/* ICIC changes start */
STATIC  UInt32 getMaxPrbSubsetIndexForNonSpatialMul(
        RAT1Info *rat1Info_p,     
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc,
        UInt32 *availableRBBitMap_p,
        ResourceRegion resourceRegionAllocated)
/* ICIC changes end */
/* - TM7_8 Changes End */
{
    UInt32 outLoopCount = 0;
    UInt32 tempBitMapWithSpanNonZero = 0;
    UInt32 tempBitMapWithSpanZero = 0;
    UInt32 availableRBBitMap = 0;
    UInt32 rbCountNonZeroSpan = 0;
    UInt32 rbCountZeroSpan = 0;
    SubsetInfo *subsetInfo_p = PNULL;
    UInt32 numFreeRB= 0;
    SubSetSpanInfoForSorting subsetInfoForSorting[MAX_SUBSETS] = {{0,0,0 ,0}} ;
    UInt32 maxSubsetIndex= 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    subsetInfo_p = rat1Info_p->subsetInfo;
    outLoopCount = rat1Info_p->subsetCount;    

    while (outLoopCount--)
    {
        subsetInfoForSorting[outLoopCount].subsetIndex = outLoopCount;
        /* ICIC changes start */
        /* For CC_CE_REGION allocated, logical AND will give cell centre subset
         * bitmap. For CE_REGION, it will give cell edge subset bitmap 
         */
        availableRBBitMap = subsetInfo_p[outLoopCount].subsetBitMap[CC_USER] &
            subsetInfo_p[outLoopCount].subsetBitMap[resourceRegionAllocated];
        /* ICIC changes end */

        /*Get the Available Number of RB Bit which falls into subset using
         *LeftSpan*/
        tempBitMapWithSpanZero = availableRBBitMap &
            subsetInfo_p[outLoopCount].bitMapWithSpanZero;

        /*Get the Available Number of RB Bit which falls into subset using
         *RightSpan*/    
        tempBitMapWithSpanNonZero = availableRBBitMap &   
            subsetInfo_p[outLoopCount].bitMapWithSpanNonZero;

        rbCountNonZeroSpan = getCount(tempBitMapWithSpanNonZero );
        rbCountZeroSpan = getCount(tempBitMapWithSpanZero );
        if ( rbCountNonZeroSpan > rbCountZeroSpan )
        {
            subsetInfoForSorting[outLoopCount].numFreeRB = rbCountNonZeroSpan;
            subsetInfoForSorting[outLoopCount].isNonZeroSpanConsideredForAlloc 
                = 1;
            subsetInfoForSorting[outLoopCount].availableRBBitMap 
                = tempBitMapWithSpanNonZero;
        }
        else
        {
            subsetInfoForSorting[outLoopCount].numFreeRB = rbCountZeroSpan;
            subsetInfoForSorting[outLoopCount].isNonZeroSpanConsideredForAlloc 
                = 0;
            subsetInfoForSorting[outLoopCount].availableRBBitMap 
                = tempBitMapWithSpanZero;
        }  

        if ( numFreeRB < subsetInfoForSorting[outLoopCount].numFreeRB )
        {
            numFreeRB = subsetInfoForSorting[outLoopCount].numFreeRB;
            maxSubsetIndex = outLoopCount;
        }

    } 
   

    *subsetIndex_p = maxSubsetIndex;
    *isNonZeroSpanConsideredForAlloc = 
        subsetInfoForSorting[maxSubsetIndex].isNonZeroSpanConsideredForAlloc;
    *availableRBBitMap_p = 
        subsetInfoForSorting[maxSubsetIndex].availableRBBitMap;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    return subsetInfoForSorting[maxSubsetIndex].numFreeRB;
}

/* + TM7_8 Changes Start */
/******************************************************************************
 * Function Name    : getMaxPrbSubsetIndexForSpatialMul
 * Inputs           : rat1Info_p - This holds pointer to RAT1Info structure,
 *                    resourceRegionAllocated - Allocated resource region
 *                    centralRbMask,
 *                    scheduledTick - Scheduled TTI,
 *                    transmissionMode - Mode of Transmission
 * Outputs          : subsetIndex_p - This holds subset Index of Subset which has 
 *                                    maximum number of FREE PRB considering left
 *                                    or right Span,
 *                    isNonZeroSpanConsideredForAlloc - This will set to one if the 
 *                                                      if Non Zero Span has been chosen
 *                                                      for allocation of RBs else if
 *                                                      set to one this means Zero Span
 *                                                      has been chosen for allocation of RBs,
 *                    availableRBBitMap_p - This holds available RB BitMap after 
 *                                          considering left and right Span
 * Returns          : Count of number of set bits in availableRBBitMap_p
 * Description      : This funtion will find  the subset Index of Subset which has 
 *                    maximum number of FREE PRBs and also find if Non Zero Span
 *                    or Zero Span has been chosen for allocation of RBs alongwith 
 *                    the available RB BitMap for the choosed Span.
 ******************************************************************************/
/* ICIC changes start */
STATIC  UInt32 getMaxPrbSubsetIndexForSpatialMul(
        RAT1Info *rat1Info_p,     
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc,
        UInt32 *availableRBBitMap_p,
        ResourceRegion resourceRegionAllocated,
        /* + TM7_8 Changes Start */
        UInt32 centralRbMask[MAX_SUBSETS],
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 transmissionMode)
        /* - TM7_8 Changes End */
/* ICIC changes end */
{
    UInt32 outLoopCount = 0;
    UInt32 tempBitMapWithSpanNonZero = 0;
    UInt32 tempBitMapWithSpanZero = 0;
    UInt32 availableRBBitMap = 0;
    UInt32 rbCountNonZeroSpan = 0;
    UInt32 rbCountZeroSpan = 0;
    SubsetInfo *subsetInfo_p = PNULL;
    UInt32 numFreeRB= 0;
    SubSetSpanInfoForSorting subsetInfoForSorting[MAX_SUBSETS] = {{0,0,0 ,0}} ;
    UInt32 maxSubsetIndex= 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    subsetInfo_p = rat1Info_p->subsetInfo;
    outLoopCount = rat1Info_p->subsetCount;    

    while (outLoopCount--)
    {
        subsetInfoForSorting[outLoopCount].subsetIndex = outLoopCount;
        /* ICIC changes start */
        /* For CC_CE_REGION allocated, logical AND will give cell centre subset
         * bitmap. For CE_REGION, it will give cell edge subset bitmap 
         */
        availableRBBitMap = subsetInfo_p[outLoopCount].subsetBitMap[CC_USER] &
            subsetInfo_p[outLoopCount].subsetBitMap[resourceRegionAllocated];
        /* ICIC changes end */
        /* + TM7_8 Changes Start */
        /* SPR 21631 Start */
        if(CHECK_SF_MAPPED_ON_BCH_PSS_SSS(scheduledTick,transmissionMode))
            /* SPR 21631 End */
        {
            /*block central 6 RBs in temp variable*/
            availableRBBitMap = availableRBBitMap &(~centralRbMask[outLoopCount]);
        }

        /* - TM7_8 Changes End */


        /*Get the Available Number of RB Bit which falls into subset using
         *LeftSpan*/
        tempBitMapWithSpanZero = availableRBBitMap &
            subsetInfo_p[outLoopCount].bitMapWithSpanZero;

        /*Get the Available Number of RB Bit which falls into subset using
         *RightSpan*/    
        tempBitMapWithSpanNonZero = availableRBBitMap &   
            subsetInfo_p[outLoopCount].bitMapWithSpanNonZero;

        rbCountNonZeroSpan = getCount(tempBitMapWithSpanNonZero );
        rbCountZeroSpan = getCount(tempBitMapWithSpanZero );
        if ( rbCountNonZeroSpan > rbCountZeroSpan )
        {
            subsetInfoForSorting[outLoopCount].numFreeRB = rbCountNonZeroSpan;
            subsetInfoForSorting[outLoopCount].isNonZeroSpanConsideredForAlloc 
                = 1;
            subsetInfoForSorting[outLoopCount].availableRBBitMap 
                = tempBitMapWithSpanNonZero;
        }
        else
        {
            subsetInfoForSorting[outLoopCount].numFreeRB = rbCountZeroSpan;
            subsetInfoForSorting[outLoopCount].isNonZeroSpanConsideredForAlloc 
                = 0;
            subsetInfoForSorting[outLoopCount].availableRBBitMap 
                = tempBitMapWithSpanZero;
        }  

        if ( numFreeRB < subsetInfoForSorting[outLoopCount].numFreeRB )
        {
            numFreeRB = subsetInfoForSorting[outLoopCount].numFreeRB;
            maxSubsetIndex = outLoopCount;
        }

    } 
   

    *subsetIndex_p = maxSubsetIndex;
    *isNonZeroSpanConsideredForAlloc = 
        subsetInfoForSorting[maxSubsetIndex].isNonZeroSpanConsideredForAlloc;
    *availableRBBitMap_p = 
        subsetInfoForSorting[maxSubsetIndex].availableRBBitMap;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    return subsetInfoForSorting[maxSubsetIndex].numFreeRB;
}

/******************************************************************************
 * Function Name    : sortSubsetBasedOnCQIForNonSpatialMul
 * Inputs           : rat1Info_p - This holds pointer to RAT1Info structure,
 *                    subBandReportInfo_p - This is pointer to SubBandReportInfo
 *                                          structure,
 *                    resourceRegionAllocated - Allocated resource region
 * Outputs          : subsetInfoForSorting - This is array of SubSetSpanInfoForSorting
 *                    structure,which return subset Index and other relevant infomation
 *                    in the order of increasing Free RB in the Subset.
 * Returns          : None
 * Description      : This funtion will first initialize the SubSetSpanInfoForSorting structure and 
 *                    and then it sort this array of structurre based on numFreeRB.
 ******************************************************************************/
/* ICIC changes start */
void sortSubsetBasedOnCQIForNonSpatialMul( 
        RAT1Info *rat1Info_p,
        SubBandReportInfo *subBandReportInfo_p,
        SubSetSpanInfoForSorting subsetInfoForSorting[MAX_SUBSETS],
        UInt32 resourceRegionAllocated)
/* ICIC changes end */

{
    UInt32 outLoopCount = 0;
    UInt32 innerLoopCount = 0;
    UInt32 temp = 0;
    UInt32 tempBitMapWithSpanNonZero = 0;
    UInt32 tempBitMapWithSpanZero = 0;
    UInt32 availableRBBitMap = 0;
    UInt32 rbCountNonZeroSpan = 0;
    UInt32 rbCountZeroSpan = 0;
    SubsetInfo    *subsetInfo_p = PNULL;
    RAT1NonSpatialMulCQIInfo *cqiInfo_p = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    subsetInfo_p = rat1Info_p->subsetInfo;
    outLoopCount = rat1Info_p->subsetCount;   
    /* SPR#706 Changes Start */
    if((!outLoopCount) || (MAX_SUBSETS<outLoopCount))
    {
        return;
    }
    /* SPR#706 Changes End */
 
    cqiInfo_p = &subBandReportInfo_p->cqiInfo.cqiInfoNonSpatialMul.
        rat1NonSpatialMulCQIInfo[outLoopCount - 1];  
    while (outLoopCount--)
    {
        subsetInfoForSorting[outLoopCount].subsetIndex = outLoopCount;
        /* ICIC changes start */
        availableRBBitMap = subsetInfo_p[outLoopCount].subsetBitMap[CC_USER] & 
            subsetInfo_p[outLoopCount].subsetBitMap[resourceRegionAllocated];        
        /* ICIC changes end */
        /*Get the Available Number of RB Bit which falls into subset using
         *LeftSpan*/
        if ( !availableRBBitMap)
        {
            subsetInfoForSorting[outLoopCount].numFreeRB = 0;
            subsetInfoForSorting[outLoopCount].isNonZeroSpanConsideredForAlloc = 0;
            subsetInfoForSorting[outLoopCount].availableRBBitMap = 0;
            continue;
        }
        tempBitMapWithSpanZero = availableRBBitMap &
            cqiInfo_p->subsetCQIBitMapALLCQIIndex & 
            subsetInfo_p[outLoopCount].bitMapWithSpanZero;

        /*Get the Available Number of RB Bit which falls into subset using
         *RightSpan*/    
        tempBitMapWithSpanNonZero = availableRBBitMap & 
            cqiInfo_p->subsetCQIBitMapALLCQIIndex &
            subsetInfo_p[outLoopCount].bitMapWithSpanNonZero;

        rbCountNonZeroSpan = getCount(tempBitMapWithSpanNonZero );
        rbCountZeroSpan = getCount(tempBitMapWithSpanZero );
        if ( rbCountNonZeroSpan > rbCountZeroSpan )
        {
            subsetInfoForSorting[outLoopCount].numFreeRB = rbCountNonZeroSpan;
            subsetInfoForSorting[outLoopCount].isNonZeroSpanConsideredForAlloc = 1;
            subsetInfoForSorting[outLoopCount].availableRBBitMap = 
                tempBitMapWithSpanNonZero;
        }
        else
        {
            subsetInfoForSorting[outLoopCount].numFreeRB = rbCountZeroSpan;
            subsetInfoForSorting[outLoopCount].isNonZeroSpanConsideredForAlloc = 0;
            subsetInfoForSorting[outLoopCount].availableRBBitMap = 
                tempBitMapWithSpanZero;
        }
        cqiInfo_p--;

    }   
    outLoopCount = rat1Info_p->subsetCount;
    /*Cov 73030 +-*/
    while ( outLoopCount-- )
    {
        /* SPR#706 Changes Start */
        innerLoopCount = rat1Info_p->subsetCount - 1;
        /* SPR#706 Changes End */
        while (innerLoopCount)
        {
            if ( subsetInfoForSorting[innerLoopCount].numFreeRB <
                    subsetInfoForSorting[innerLoopCount - 1].numFreeRB )
            {
                temp  =  subsetInfoForSorting[innerLoopCount - 1].numFreeRB;
                subsetInfoForSorting[innerLoopCount - 1].numFreeRB = 
                    subsetInfoForSorting[innerLoopCount].numFreeRB;
                subsetInfoForSorting[innerLoopCount].numFreeRB = temp;


                temp  =  subsetInfoForSorting[innerLoopCount - 1].subsetIndex;
                subsetInfoForSorting[innerLoopCount - 1].subsetIndex = 
                    subsetInfoForSorting[innerLoopCount].subsetIndex;
                subsetInfoForSorting[innerLoopCount].subsetIndex = temp;               

                temp  =  subsetInfoForSorting[innerLoopCount - 1].
                    isNonZeroSpanConsideredForAlloc;
                subsetInfoForSorting[innerLoopCount - 1].
                    isNonZeroSpanConsideredForAlloc = 
                    subsetInfoForSorting[innerLoopCount].
                    isNonZeroSpanConsideredForAlloc;
                subsetInfoForSorting[innerLoopCount].
                    isNonZeroSpanConsideredForAlloc = temp;

                temp  =  subsetInfoForSorting[innerLoopCount - 1].
                    availableRBBitMap;
                subsetInfoForSorting[innerLoopCount - 1].
                    availableRBBitMap = 
                    subsetInfoForSorting[innerLoopCount].availableRBBitMap;
                subsetInfoForSorting[innerLoopCount].availableRBBitMap = 
                    temp;
            }
            innerLoopCount--;
        }

    }    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}

/******************************************************************************
 * Function Name    : getRAT1BitMapNewTxNonSpatialMux
 * Inputs           : rat1Info_p - Pointer to RAT1Info,
 *                    resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                    harqProcess_p -  This holds pointer to DLHARQProcess structure,
 *                    dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                    dlUEContext_p - Pointer to DLUEContext,
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : subsetIndex_p - This holds the  subset Index of the Subset 
 *                                    which has maximum number of FREE PRBs,
 *                    isSpanZeroConsideredForAlloc_p - This Indicates if the RBs
 *                                                     has been considered using Zero
 *                                                     Span Width or Non Zero Span width
 * Returns          : bitMapToBeReturned - This Holds the BitMap which we will get 
 *                                         after ANDing availableRBGBitMap with the
 *                                         subBand CQI BitMap.
 * Description      : This funtion will process the Bitmap New Tx for Non 
 *                    spatial multiplexing and returns hte RAT1 Bit map
 ******************************************************************************/
STATIC UInt32 getRAT1BitMapNewTxNonSpatialMux(
        RAT1Info *rat1Info_p,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess *harqProcess_p,
        UInt32 *subsetIndex_p,
        UInt32 *isNonZeroSpanConsideredForAlloc_p,
        DLStrategyTxNode *dlStrategyTxNode_p
/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
        ,DLUEContext *dlUEContext_p
#endif
        ,InternalCellIndex internalCellIndex
/*HD FDD Changes End*/
        )
{
/*  SubsetInfo    *subsetInfo_p = PNULL;TDD Warning Fix */
    UInt32 subsetCount = 0;
    UInt32 *subsetZeroNonZeroSpanBitMap_p = 0;
    SubSetSpanInfoForSorting subsetInfoForSorting[MAX_SUBSETS] = {{0,0,0 ,0}} ;
	UInt32 isNonZeroSpanConsidered = 0;

    ResourceFormatNode previousRescrFormat = {0};
    UInt32 bitMapToBeReturned = 0;
    
    RAT1NonSpatialMulCQIInfo *cqiInfo_p = PNULL;
    UInt32 subsetIndex= 0;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);

    SubBandReportInfo *subBandReportInfo_p = resrcAllocatorInput_p->subBandReportInfo_p;
    UInt32 requiredRBCount = resrcAllocatorInput_p->requiredRB;
    
/*  subsetInfo_p = rat1Info_p->subsetInfo; TDD Warning Fix */
    subsetCount = rat1Info_p->subsetCount;    
    cqiInfo_p = subBandReportInfo_p->cqiInfo.cqiInfoNonSpatialMul.
        rat1NonSpatialMulCQIInfo;

    /* ICIC changes start */
    sortSubsetBasedOnCQIForNonSpatialMul(rat1Info_p,
            subBandReportInfo_p, subsetInfoForSorting,
            resrcAllocatorInput_p->resourceRegionAllocated);
    /* ICIC changes end */
   
    /*Below While Loop is called for each subsetIndex starting from subset 
     *which has maximum number of FREE RB taking consideration of Left 
     *and Right Span.For each iteration it call relevant funtion which will 
     *check if it can satisfy the required RB or not*.On Success it return 
     *the bitMapToBeReturned_p with nonZero Value else it will return 
     *Zero Value*/
    while ( subsetCount-- )
    {
        subsetIndex = subsetInfoForSorting[subsetCount].subsetIndex;
        if (subsetInfoForSorting[subsetCount].isNonZeroSpanConsideredForAlloc)
        {
            /*Making isNonZeroSpanConsideredForAlloc to true which will be used
             * while forming resource assignment BitMap to indicate whether RBs
             * are allocated from using Zero Span Width  or RBs are allocated using
             * None Span Width in the Subset Indexed by maxPRBSubsetIndex.If 
             * Set to one this means RBS are allocated using None Zero Span Width
             *else RBs are allocated using  Zero Span Width*/
            isNonZeroSpanConsidered = 1;
            subsetZeroNonZeroSpanBitMap_p = cqiInfo_p[subsetIndex].subsetCQIBitMapNonZeroSpan;
        }
        else
        {
            /*Making isNonZeroSpanConsideredForAlloc to false which will be used
             * while forming resource assignment BitMap to indicate whether RBs
             * are allocated from using Zero Span Width  or RBs are allocated using
             * None Span Width in the Subset Indexed by maxPRBSubsetIndex.If 
             * Set to one this means RBS are allocated using None Zero Span Width
             *else RBs are allocated using  Zero Span Width*/
            isNonZeroSpanConsidered = 0;        
            subsetZeroNonZeroSpanBitMap_p = cqiInfo_p[subsetIndex].subsetCQIBitMapZeroSpan;
        }
        /* + CQI_4.1 */
        bitMapToBeReturned = getSubSetBitMapNewTxNonSpatialMux(
                    harqProcess_p,
                    subsetInfoForSorting[subsetCount].availableRBBitMap,
                    subsetZeroNonZeroSpanBitMap_p,
                    requiredRBCount,
                    cqiInfo_p[subsetIndex].reportedCQIBitMap,
                    dlStrategyTxNode_p,
                    &previousRescrFormat,
	                subsetIndex,
		            isNonZeroSpanConsidered
        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                    ,dlUEContext_p
#endif
                    ,internalCellIndex
        /*HD FDD Changes End*/
                    );

        /* - CQI_4.1 */
        if (bitMapToBeReturned)
        {
			*isNonZeroSpanConsideredForAlloc_p = isNonZeroSpanConsidered;
            *subsetIndex_p = subsetIndex;
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "Leaving funtion Success  = %s\n",__func__);
            return bitMapToBeReturned;    
        } 
    }
bitMapToBeReturned = 0;
    /*It means that we are not able to meet minTBSize asked by strategy hence if strict subband allocation
           is required then we will schedule this UE with less than minimum TB Size*/
    if (!dlStrategyTxNode_p->strictAllocatedFlag)
    {
        /* As of now, there is no use case for strictAllocSubBand
         * strictAllocSubBand check is commented */
        //if (resrcAllocatorInput_p->strictAllocSubBand)
        //{
            if(previousRescrFormat.tbSize >= HARQ_TB_MINIMUM_SIZE_IN_BYTES)
            {
                harqProcess_p->assignedResourcesNB = previousRescrFormat.prbs;
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = previousRescrFormat.mcsIndex;    
                harqProcess_p->dlHARQTBOneInfo.tbSize = previousRescrFormat.tbSize;
                bitMapToBeReturned = previousRescrFormat.cqiRBGBitmap;
				*isNonZeroSpanConsideredForAlloc_p = previousRescrFormat.isNonZeroSpan;
            	*subsetIndex_p = previousRescrFormat.subsetIndex;

                //update excess PRBs in buffer pool
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resrcAllocatorInput_p->resourceRegionAllocated, 
                        (requiredRBCount - previousRescrFormat.prbs),
                        internalCellIndex);
                /* ICIC changes end */
            }
        //}
    }

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion with Failure = %s\n",__func__);
    return bitMapToBeReturned;
}


/******************************************************************************
 * Function Name    : getSubSetBitMapNewTxNonSpatialMux
 * Inputs           : harqProcess_p -  This holds pointer to DLHARQProcess structure,
 *                    availableRBBitMap -  This Holds Available RB BitMap,
 *                    rat1SpanCQIInfo_p - This holds pointer to RAT1SpanNonSpatialCQIInfo structure,
 *                    requiredRBCount - This holds required Number of RBs which need to be allocated,
 *                    reportedCQIBitMap -
 *                    dlStrategyNode_p - Pointer to DLStrategyTxNode,
 *                    previousResrcFmt_p - Pointer to ResourceFormatNode,
 *                    subsetIndex - Index of Subset,
 *                    isZeroNonZeroSpan,
 *                    dlUEContext_p (FDD Only)
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None  
 * Returns          : bitMapCqiAndAvailalbeRBs - This Holds the BitMap which we will get 
 *                                               after ANDing availableRBGBitMap with the
 *                                               subBand CQI BitMap.
 * Description      : This funtion will first check if the 64QAM Bands can satisfy 
 *                    the required Number of RBs.If successful ,it will populate 
 *                    relevant information in harqProcess_p structure else it will
 *                    go for 16QAM Bands.If 16QAM also don't satisfy the required
 *                    Number of RBs, it will repeat logic for QPSK bands.
 ******************************************************************************/ 
STATIC  UInt32 getSubSetBitMapNewTxNonSpatialMux( 
        DLHARQProcess  *harqProcess_p,
        UInt32 availableRBBitMap,
        UInt32 *rat1SpanCQIInfo_p,
        UInt32 requiredRBCount,
        UInt16 reportedCQIBitMap,
        DLStrategyTxNode *dlStrategyNode_p,
        ResourceFormatNode *previousResrcFmt_p,
	    UInt32 subsetIndex,
	    UInt32 isZeroNonZeroSpan
/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
        ,DLUEContext *dlUEContext_p
#endif
        ,InternalCellIndex internalCellIndex
/*HD FDD Changes End*/
        )
{
    DLHARQTBInfo *dlHARQTBInfo_p = PNULL;
    UInt32 cqiIndexCount = 0;
    UInt32 *cqiBitMap_p = PNULL;
    UInt32 bitMapCqiAndAvailalbeRBs = 0;
    UInt32 availableRBCount = 0;
    /* + CQI_4.1 */
    UInt16 tmpBitMap = 0;
    UInt8  cqiReportIndex = 0;

    UInt32 freqSelectiveTBSize = 0;
    UInt32 freqSelectiveMCSIndex = 0;
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
#endif
    /*HD FDD Changes End*/
    
    UInt32 minTBSize = dlStrategyNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyNode_p->maxTBSize;
    /* - CQI_4.1 */
    /* ICIC changes start */
    UInt32 resourceRegionAllocated =
        dlStrategyNode_p->resourceAllocInput.resourceRegionAllocated; 
    /* ICIC changes end */
    
    if (maxTBSize < minTBSize)
    {
        lteWarning("[RAT1 NonSpatial] Wrong Max TBSize calculated by DL Strategy\
                     minTBSize [%d] maxTBSize [%d]\n", minTBSize, maxTBSize);
        maxTBSize = minTBSize;
    }    

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    dlHARQTBInfo_p = &harqProcess_p->dlHARQTBOneInfo;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Start Searching in SubBands \n");
    /* + CQI_4.1 */
    tmpBitMap = reportedCQIBitMap;

    if(!tmpBitMap)  
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,
            "NO CQI BitMap available");
        return 0;
    }
 
    /* Below while loop is iterating x number of times where x is the 
     * difference of the Highest CQI Index and  lowest CQI Index for 
     * Subband whose report is available.For each subband it 
     * checks if the tbSize can be satisfied from it ,if true then it 
     * will populate the relevant information in harqProcess_p structure 
     * else it will jump to next Lower CQI Index SubBands.*/  
    while(tmpBitMap)
    {
        /* Decode the bit map of the reported CQI */
        cqiReportIndex = FFS(tmpBitMap);
        tmpBitMap &= (~((UInt16)1 << (cqiReportIndex-1)));
        cqiIndexCount = (MAX_CQI_INDEX - cqiReportIndex);   

        cqiBitMap_p = &rat1SpanCQIInfo_p[cqiIndexCount];

        /*Checking for available RBG whose cqi report is available and 
         * updating the same in the bitMapCqiAndAvailalbeRBs bitMap */
        bitMapCqiAndAvailalbeRBs = (*cqiBitMap_p & availableRBBitMap);
        availableRBCount = getCount(bitMapCqiAndAvailalbeRBs);
        if (!availableRBCount)                   
        {
            continue;
        }
        
        /*Checking if  the availableRBCount > requiredRBCount.If true ,we
         *will allocate requiredRBCount Number of RBs and populate the 
         *dlHARQTBInfo_p with relevant information and then return from 
         *there */

        freqSelectiveMCSIndex = getMCSFromCQI(cqiIndexCount
                                );
	    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
	    /* FILL the MCS index value from CQI codeword*/
	    HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currTick, dlUEContext_p, freqSelectiveMCSIndex);
#endif
	    /*HD FDD Changes End*/    

        if (availableRBCount >= requiredRBCount)
        {   
            freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex,
                                                  requiredRBCount,
                                                  /*4X4 DL MIMO CHG START*/
                                                  TB_MAPPED_TO_ONE_LAYER
                                                  /*4X4 DL MIMO CHG END*/);  
            /*If TB size on this mcs with requiredRBCount is greater than minimum TB size then 
                we can go for maximum TB size too if excess Pool has some RBs left. This case means successful as 
                minimum TB size is always met*/         
            if (freqSelectiveTBSize >= minTBSize)
            {
                if (freqSelectiveTBSize >= maxTBSize)
                {
                    freqSelectiveTBSize = maxTBSize;
                    /* SPR 5999 changes start */
                    harqProcess_p->assignedResourcesNB = DETERMINE_RBs_DL(freqSelectiveMCSIndex,
                                                                          freqSelectiveTBSize,
                                                                          internalCellIndex);                    
                    /* SPR 5999 changes end */
                }
                else
                {
                    harqProcess_p->assignedResourcesNB = requiredRBCount;
                }
                
                dlHARQTBInfo_p->mcsIndex = freqSelectiveMCSIndex;

                //update excess PRBs in buffer pool
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                        (requiredRBCount - harqProcess_p->assignedResourcesNB),
                        internalCellIndex);
                /* ICIC changes end */
                
                return bitMapCqiAndAvailalbeRBs;
            }
            else
            {
                /*Here allocation can be done only if some RBs are left in excessPool which allows TBSize to be greater
                    than or equal to minimum TB Size. If not possible then store this TBSize and found RBs as later on it will
                    required to allocate if strict subband allocation is required*/
                    
                UInt32 requiredRBsFromPool = 0;
                if (excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0)
                {
                    requiredRBsFromPool = allocatePRBsUsingExcessPool(freqSelectiveMCSIndex,
                                                                      availableRBCount,
                                                                      minTBSize,
                                                                      requiredRBCount,
                                                                      resourceRegionAllocated,
                                                                      internalCellIndex);
                    
                    freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex, 
                                                          requiredRBCount+ requiredRBsFromPool,
                                                          /*4X4 DL MIMO CHG START*/
                                                          TB_MAPPED_TO_ONE_LAYER
                                                          /*4X4 DL MIMO CHG END*/);
                    if (freqSelectiveTBSize >= minTBSize)
                    {
                        dlHARQTBInfo_p->mcsIndex = freqSelectiveMCSIndex;
                        harqProcess_p->assignedResourcesNB = requiredRBCount + requiredRBsFromPool; 

                        //update excess PRBs in buffer pool
                        /* ICIC changes start */
                        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                (requiredRBCount - harqProcess_p->assignedResourcesNB),
                                internalCellIndex);
                        /* ICIC changes end */
                
                        return bitMapCqiAndAvailalbeRBs;
                    }
                }
                
                if(previousResrcFmt_p->tbSize < freqSelectiveTBSize)
                {
                    previousResrcFmt_p->tbSize = freqSelectiveTBSize;
                    previousResrcFmt_p->mcsIndex = freqSelectiveMCSIndex;
                    previousResrcFmt_p->prbs = requiredRBCount+ requiredRBsFromPool;
                    previousResrcFmt_p->cqiRBGBitmap = bitMapCqiAndAvailalbeRBs;
					previousResrcFmt_p->subsetIndex = subsetIndex;
					previousResrcFmt_p->isNonZeroSpan = isZeroNonZeroSpan;

                }               
            }
        }
        else
        {
            /* Here although available RBs are less than required RBs but still allocation can be done as
                    reported MCS can be greater than wideband MCS used by strategy hence min/max TB size can be
                    achieved with available RBs*/
            freqSelectiveTBSize = determineTBSize(freqSelectiveMCSIndex,
                                                  availableRBCount,
                                                  /*4X4 DL MIMO CHG START*/
                                                  TB_MAPPED_TO_ONE_LAYER
                                                  /*4X4 DL MIMO CHG END*/); 
            if (freqSelectiveTBSize >= minTBSize)
            {
                if (freqSelectiveTBSize > maxTBSize)
                {
                    freqSelectiveTBSize = maxTBSize;
                }
                
                dlHARQTBInfo_p->mcsIndex = freqSelectiveMCSIndex;               
                /* SPR 5999 changes start */
                harqProcess_p->assignedResourcesNB = DETERMINE_RBs_DL(freqSelectiveMCSIndex,
                                                                      freqSelectiveTBSize,
                                                                      internalCellIndex);
                /* SPR 5999 changes end */
                //update excess PRBs in buffer pool
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                        (requiredRBCount - harqProcess_p->assignedResourcesNB),
                        internalCellIndex);
                /* ICIC changes end */

                return bitMapCqiAndAvailalbeRBs;
            }
            else
            {
                if(previousResrcFmt_p->tbSize < freqSelectiveTBSize)
                {
                    previousResrcFmt_p->tbSize = freqSelectiveTBSize;
                    previousResrcFmt_p->mcsIndex = freqSelectiveMCSIndex;
                    previousResrcFmt_p->prbs = availableRBCount;
                    previousResrcFmt_p->cqiRBGBitmap = bitMapCqiAndAvailalbeRBs;
					previousResrcFmt_p->subsetIndex = subsetIndex;
					previousResrcFmt_p->isNonZeroSpan = isZeroNonZeroSpan;
                }
            }
        }
    }

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    return 0;
}

/******************************************************************************
 * Function Name    : allocRBCommonChannelRAT2Localized
 * Inputs           : resourceAllocInput_p - This is pointer to Resource Allocator 
 *                                           Input Structure,
 *                    raPolicyInfo_p - This holds pointer to 
 *                                     ResourceAllocationPolicyInfo structure,
 *                    format -
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : resourceAllocOutput_p - This is pointer to Resource Allocator 
 *                                            Output on which once Resources are allocated,
 *                                            relevant information can be written
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion allocate the RBs  for Common Control 
 *                    Channel according to RAT 2 Localized and once RBs gets 
 *                    allocated update the RAT 0, RAT 1, RAT 2 Distributed 
 *                    Information.
 ******************************************************************************/
 MacRetType  allocRBCommonChannelRAT2Localized(
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DCIFormat format,
        InternalCellIndex internalCellIndex)
{
    BestVRBSpanInfo bestVRBSpanInfo ={0,0};
    UInt32 availableRBCount =0;  
    UInt32 requiredRBCount = 0;
    UInt32 allocatedRBCount = 0;
    /* ICIC changes start */
    /* This flag is used to trigger update of RAT2 Distributed information,
     * As of now this is only updated for common channel messages, 
     * as RAT2 distributed is not used for data 
     */
    UInt32 updateRAT2DistributedFlag = TRUE;
    /* ICIC changes end */

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);


    /*getBestContLocalizedVRBNewTx  funtion will try to find  the maximum
      continousfree VRBs ( equal to resourceAllocInput_p->requiredRB) 
      in the RAT2LocalizedVRBInfo array which holds the information of  
      Free VRB .If it's not able to find then it will return  the best 
      continous VRB starting and ending Position in the the same  array 
      and populate it in the bestVRBSpanInfo structure*/
      requiredRBCount = resourceAllocInput_p->requiredRB;
    if  (requiredRBCount > raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable)
    {
        return MAC_FAILURE;    
    }

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "requiredRBCount = %u",resourceAllocInput_p->requiredRB);
    /* SPR#706 Changes Start */
    /* ICIC changes start */
    /* Common channel resources are allocated over entire BW (CC_CE_REGION) */
    if( MAC_FAILURE == getBestContLocalizedVRBNewTx(requiredRBCount, &bestVRBSpanInfo, 
            &raPolicyInfo_p->rat2LocalizedInfo, CC_CE_REGION, requiredRBCount,internalCellIndex))
    /* ICIC changes end */
    {
        return MAC_FAILURE;    
    }
    /* SPR#706 Changes End */


    availableRBCount = bestVRBSpanInfo.endingVRBIndex - bestVRBSpanInfo.
        startingVRBIndex + 1;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "availableRBCount = %u",availableRBCount);

    if ( availableRBCount < requiredRBCount)
    {
        return MAC_FAILURE;
    }
/* SPR 1489 changes start */
    allocatedRBCount = availableRBCount;
/* SPR 1489 changes end */

    raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable -= allocatedRBCount;
    resourceAllocOutput_p->allocatedRB = allocatedRBCount;
    resourceAllocOutput_p->ratPolicy = RA_POLICY_2_LOCALIZED;
    resourceAllocOutput_p->startingVRBIndex = bestVRBSpanInfo.startingVRBIndex;
        
    /* ICIC changes start */
    updateRAT2LocalizedCellEdgeInfo(&raPolicyInfo_p->rat2LocalizedInfo,
            bestVRBSpanInfo.startingVRBIndex,
            bestVRBSpanInfo.endingVRBIndex);
    /* ICIC changes end */

    populateResourceAssignmentInfo(&resourceAllocOutput_p-> 
            resourceAssignmentBitMap, allocatedRBCount, bestVRBSpanInfo.
            startingVRBIndex,0,format,internalCellIndex);           
    /* +DYNAMIC_ICIC */
    /* RNTP is calulated only for UE specific PDSCH */
    updateRATInfoFromRAT2LocalizedInfo(&bestVRBSpanInfo, raPolicyInfo_p,
            allocatedRBCount,updateRAT2DistributedFlag, 0,
            internalCellIndex);    
    /* -DYNAMIC_ICIC */
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s with Success\n",__func__);
    return MAC_SUCCESS;
}


/******************************************************************************
 * Function Name    : allocRBCommonChannelRAT2Distributed
 * Inputs           : resourceAllocInput_p - This is pointer to Resource Allocator 
 *                                           Input Structure,
 *                    raPolicyInfo_p - This holds pointer to 
 *                                     ResourceAllocationPolicyInfo structure,
 *                    format -
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : resourceAllocOutput_p - This is pointer to Resource Allocator 
 *                                            Output on which once Resources are allocated,
 *                                            relevant information can be written
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion allocate the RBs  for Common Control 
 *                    Channel according to RAT 2 Distributed and once RBs gets 
 *                    allocated update the RAT 0, RAT 1, RAT 2 Localized Information.
 ******************************************************************************/
 MacRetType  allocRBCommonChannelRAT2Distributed(
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DCIFormat format,
        InternalCellIndex internalCellIndex)
{

    BestVRBSpanInfo bestVRBSpanInfo ={0,0};
    UInt32 availableRBCount =0;  
    UInt32 requiredRBCount = 0;
    UInt32 allocatedRBCount = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);


    /*getBestContDistributedVRBNewTx  funtion will try to find  the maximum
      continousfree VRBs ( equal to resourceAllocInput_p->requiredRB) 
      in the RAT2LocalizedVRBInfo array which holds the information of  
      Free VRB .If it's not able to find then it will return  the best 
      continous VRB starting and ending Position in the the same  array 
      and populate it in the bestVRBSpanInfo structure*/
      requiredRBCount = resourceAllocInput_p->requiredRB;
    if (requiredRBCount > raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable)
    {
        /* ICIC changes start */
        return MAC_FAILURE;    
        /* ICIC changes end */
    }

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "requiredRBCount = %u",resourceAllocInput_p->requiredRB);
    /* SPR#706 Changes Start */
    if( MAC_FAILURE == getBestContDistributedVRBNewTx(requiredRBCount, &bestVRBSpanInfo, 
                &raPolicyInfo_p->rat2DistributedInfo))
    {
        return MAC_FAILURE;    
    }
    /* SPR#706 Changes End */


    availableRBCount = bestVRBSpanInfo.endingVRBIndex - bestVRBSpanInfo.
        startingVRBIndex + 1;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "availableRBCount = %u",availableRBCount);

    if ( availableRBCount < requiredRBCount)
    {
        return MAC_FAILURE;
    }
    allocatedRBCount = requiredRBCount;      
    raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable -= allocatedRBCount;
    resourceAllocOutput_p->allocatedRB = allocatedRBCount;
    resourceAllocOutput_p->ratPolicy = RA_POLICY_2_DISTRIBUTED;
    resourceAllocOutput_p->startingVRBIndex = bestVRBSpanInfo.
        startingVRBIndex - 1;
    if (GAP_VALUE_ONE == cellSpecificParams_g.cellConfigAndInitParams_p\
                   [internalCellIndex]->cellParams_p->gapValue)
    {
        resourceAllocOutput_p->nGAP = 0;
    }
    else
    {
        resourceAllocOutput_p->nGAP = 1;
    }

    /* ICIC changes start */
    /* RAT2 distributed support for ICIC needs to be added */
    /* ICIC changes end */

    populateResourceAssignmentInfo(&resourceAllocOutput_p-> 
            resourceAssignmentBitMap, allocatedRBCount, bestVRBSpanInfo.
            startingVRBIndex, 1,format,internalCellIndex);           
    updateRATInfoFromRAT2DistributedInfo(&bestVRBSpanInfo, 
            raPolicyInfo_p, allocatedRBCount,internalCellIndex);    
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s with Success\n",__func__);
    return MAC_SUCCESS;
}

/* SPR 3306 Fix Starts */
/*****************************************************************************
 * Function Name  : reserveCommonChannelRB
 * Inputs         : raPolicyInfo_p - pointer to ResourceAllocationPolicyInfo
 *                  internalCellIndex -  Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion will modify the raPolicyInfo_p structure
 *                  such that the center 6 RBs have been reserved in
 *                  RAT 0 ,RAT 1, RAT Localized and RAT 2 Distributed.
 *****************************************************************************/
void reserveCommonChannelRB(ResourceAllocationPolicyInfo *raPolicyInfo_p,
        InternalCellIndex internalCellIndex)
{
    UInt32 dlRBs = 0;
    UInt32 startingRBNumber = 0;
/*  UInt32 endingRBNumber = 0; TDD Warning Fix */
    SubsetInfo *subsetInfo_p = PNULL;
    UInt32 tempBitMap;
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;
    UInt32 rbgIndex = 0;
    UInt32 rbgSize = 0;
    UInt32 maxRBCountForMIB = 6;
    UInt32 subsetIndex= 0;
    UInt32 prbPositionWithinSubset= 0;
    UInt32 previousFreeVRBIndex = 0;
    UInt32 nextFreeVRBIndex = 0;
    /* ICIC changes start */
    UInt32 firstRBInMIBRegion = 0;
    UInt32 lastRBInMIBRegion = 0;
    /* ICIC changes end*/
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;
    RAT2DistributedVRBInfo  *distributedVRBInfo_p = PNULL;
    UInt32 vrbMappedToEvenSlotOfPRB  = 0;
    UInt32 vrbMappedToOddSlotOfPRB  = 0;
    localizedVRBInfo_p = raPolicyInfo_p->rat2LocalizedInfo.
        rat2LocalizedVRBInfo;
    distributedVRBInfo_p = raPolicyInfo_p->rat2DistributedInfo.
        rat2DistributedVRBInfo;
    rat1Info_p = &raPolicyInfo_p->rat1Info;
    rat0Info_p = &raPolicyInfo_p->rat0Info;
    dlRBs = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
            dlAvailableRBs;
    rbgSize = rat0Info_p->rbgSize;
    if (MODULO_TWO(dlRBs))
    {
        startingRBNumber = DIVIDE_BY_TWO(dlRBs) - 1;
/*  endingRBNumber =  startingRBNumber + 5; TDD Warning Fix */
        startingRBNumber--;
    }
    else
    {
        startingRBNumber = DIVIDE_BY_TWO(dlRBs) - 2;
/*  endingRBNumber =  startingRBNumber + 5; TDD Warning Fix */
        startingRBNumber--;
    }
    subsetInfo_p = rat1Info_p->subsetInfo;
    while (maxRBCountForMIB--)
    {
        /* ICIC changes start */
        /* update RA policy information for MIB reserved RBs only if they are
         * Non-Blocked RBs
         */
        if ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->rbToResourceTypeMapDL[startingRBNumber])
        {
            rbgIndex =  (startingRBNumber ) / rbgSize;
            subsetIndex = (rbgIndex ) % rbgSize;
            /* ICIC changes start*/
            /* Find first and last available RB in MIB region to update 
             * RAT2 Localized information.
             */
            
            if (!firstRBInMIBRegion)
            {
                firstRBInMIBRegion = startingRBNumber;
            }
            lastRBInMIBRegion = startingRBNumber;
            
            /* These operations are performed on cell centre bitmaps, bitmaps for 
             * cell edge users are derived from cell centre bitmaps
             */
            rat0Info_p->rat0BitMap[CC_USER] &= (~((UInt32)1 << rbgIndex));
            /* ICIC changes end */

            prbPositionWithinSubset =  ((startingRBNumber / rbgSize) / (rbgSize ) ) *
                rbgSize +  (startingRBNumber % rbgSize);

            tempBitMap = subsetInfo_p[subsetIndex].subsetBitMap[CC_USER];
            subsetInfo_p[subsetIndex].subsetBitMap[CC_USER] &= (~(((UInt32)1) << (
                            prbPositionWithinSubset )));
            if (tempBitMap != subsetInfo_p[subsetIndex].subsetBitMap[CC_USER])
            {
                subsetInfo_p[subsetIndex].numPRBFree -= 1;

                /*Get  the vrbNumber which is mapped to even slot of PRB*/
                vrbMappedToEvenSlotOfPRB  = localizedVRBInfo_p[startingRBNumber + 1].
                    vrbMappedToEvenSlotOfPRB;
                /*Get  the vrbNumber which is mapped to odd slot of PRB*/
                vrbMappedToOddSlotOfPRB  = localizedVRBInfo_p[startingRBNumber + 1].
                    vrbMappedToOddSlotOfPRB;
                if ( vrbMappedToEvenSlotOfPRB != vrbMappedToOddSlotOfPRB )
                {
                    /*Updating the distributedVRBInfo_p array such that a Free VRB knows
                      the next Free VRB Index and also previous Free VRB Index*/
                    previousFreeVRBIndex = distributedVRBInfo_p[vrbMappedToEvenSlotOfPRB].
                        previousVRBfreeIndex;
                    nextFreeVRBIndex = distributedVRBInfo_p[vrbMappedToEvenSlotOfPRB]
                        .nextfreeVRBIndex;
                    distributedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex =
                        nextFreeVRBIndex;
                    distributedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex =
                        previousFreeVRBIndex;
                    raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable -= 1;
                }
                /*Updating the distributedVRBInfo_p array such that a Free VRB knows
                  the next Free VRB Index and also previous Free VRB Index*/
                previousFreeVRBIndex = distributedVRBInfo_p[vrbMappedToOddSlotOfPRB].
                    previousVRBfreeIndex;
                nextFreeVRBIndex = distributedVRBInfo_p[vrbMappedToOddSlotOfPRB].
                    nextfreeVRBIndex;
                distributedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex =
                    nextFreeVRBIndex;
                distributedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex =
                    previousFreeVRBIndex;
                raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable -= 1;

                raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable -= 1;
                if (CELL_EDGE_ONLY_REGION == (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->\
                        rbToResourceTypeMapDL[startingRBNumber])
                {
                    raPolicyInfo_p->rat2LocalizedInfo.
                        numOfCellEdgeVRBAvailable -= 1;
                }
            }
        }
        /* ICIC changes end */
        startingRBNumber++;
    }

    /*Updating the localizedVRBInfo_p array such that a Free VRB knows
      the next Free VRB Index and also previous Free VRB Index*/
    /* ICIC changes start */
    /* Update RAT2 Localized information only if MIB region RBs are non blocked
     * as per current ICIC configuration
     */
    if (firstRBInMIBRegion)
    {
        previousFreeVRBIndex = localizedVRBInfo_p[firstRBInMIBRegion + 1].
            previousVRBfreeIndex;
        nextFreeVRBIndex = localizedVRBInfo_p[lastRBInMIBRegion + 1].
            nextfreeVRBIndex;
        localizedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex =
            nextFreeVRBIndex;
        localizedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex =
            previousFreeVRBIndex;
    }

    if (raPolicyInfo_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable)
    {
        /* update Cell Edge information for RAT2 Localized */ 
        if ((raPolicyInfo_p->rat2LocalizedInfo.firstCellEdgeFreeIndex < 
                    nextFreeVRBIndex) &&
                (raPolicyInfo_p->rat2LocalizedInfo.firstCellEdgeFreeIndex > 
                 previousFreeVRBIndex))
        {
            raPolicyInfo_p->rat2LocalizedInfo.firstCellEdgeFreeIndex = 
                nextFreeVRBIndex;
        }
        if ((raPolicyInfo_p->rat2LocalizedInfo.endCellEdgeFreeIndex < 
                    nextFreeVRBIndex) &&
                (raPolicyInfo_p->rat2LocalizedInfo.endCellEdgeFreeIndex > 
                 previousFreeVRBIndex))
        {
            raPolicyInfo_p->rat2LocalizedInfo.endCellEdgeFreeIndex = 
                previousFreeVRBIndex;
        }
    }
    /* ICIC changes end */

}

/*****************************************************************************
 * Function Name  : unreserveMIBRB
 * Inputs         : raPolicyInfo_p - Pointer to ResourceAllocationPolicyInfo
 *                  internalCellIndex -  Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion will modify the raPolicyInfo_p structure
 *                  such that the center 6 RBs have been unreserved in
 *                  RAT 0 ,RAT 1, RAT Localized and RAT 2 Distributed.
 *****************************************************************************/
void unreserveMIBRB(ResourceAllocationPolicyInfo *raPolicyInfo_p,
        InternalCellIndex internalCellIndex)
{
    UInt32 dlRBs = 0;
    UInt32 startingRBNumber = 0;
    UInt32 endingRBNumber = 0;
    SubsetInfo *subsetInfo_p = PNULL;
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;
    UInt32 rbgIndex = 0;
    UInt32 rbgSize = 0;
    UInt32 maxRBCountForMIB = 6;
    UInt32 subsetIndex= 0;
    UInt32 prbPositionWithinSubset= 0;
    UInt32 previousFreeVRBIndex = 0;
    UInt32 nextFreeVRBIndex = 0;
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;
    RAT2DistributedVRBInfo  *distributedVRBInfo_p = PNULL;
    UInt32 vrbMappedToEvenSlotOfPRB  = 0;
    UInt32 vrbMappedToOddSlotOfPRB  = 0;
    /* ICIC changes start */
    UInt32 firstRBInMIBRegion = 0;
    UInt32 lastRBInMIBRegion = 0;
    /* ICIC changes end */
    localizedVRBInfo_p = raPolicyInfo_p->rat2LocalizedInfo.
        rat2LocalizedVRBInfo;
    distributedVRBInfo_p = raPolicyInfo_p->rat2DistributedInfo.
        rat2DistributedVRBInfo;
    rat1Info_p = &raPolicyInfo_p->rat1Info;
    rat0Info_p = &raPolicyInfo_p->rat0Info;
    dlRBs = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
            dlAvailableRBs;
    rbgSize = rat0Info_p->rbgSize;
    if (MODULO_TWO(dlRBs))
    {
        startingRBNumber = DIVIDE_BY_TWO(dlRBs) - 1;
        endingRBNumber =  startingRBNumber + 5;
        startingRBNumber--;
    }
    else
    {
        startingRBNumber = DIVIDE_BY_TWO(dlRBs) - 2;
        endingRBNumber =  startingRBNumber + 5;
        startingRBNumber--;
    }
    subsetInfo_p = rat1Info_p->subsetInfo;
    while (maxRBCountForMIB--)
    {
        /* ICIC changes start */
        /* update RA policy information for MIB reserved RBs only if they are
         * Non-Blocked RBs
         */
        if ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->rbToResourceTypeMapDL[startingRBNumber])
        {
            rbgIndex =  (startingRBNumber ) / rbgSize;
            subsetIndex = (rbgIndex ) % rbgSize;
            /* ICIC changes start*/
            /* Find first and last available RB in MIB region to update 
             * RAT2 Localized information.
             */
            if (!firstRBInMIBRegion)
            {
                firstRBInMIBRegion = startingRBNumber;
            }
            lastRBInMIBRegion = startingRBNumber;
           
            /* In case of ICIC, we should mark RBG available only if all the 
             * RBs in RBG are unblocked. For this check if RBG was available 
             * in global RAT0 bitmap */ 
            if ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info.
                    rat0BitMap[CC_USER] & ((UInt32)1 << rbgIndex))
            {
                rat0Info_p->rat0BitMap[CC_USER] |= ((UInt32)1 << rbgIndex);
            }

            prbPositionWithinSubset =  ((startingRBNumber / rbgSize) / (rbgSize ) ) *
                rbgSize +  (startingRBNumber % rbgSize);
            subsetInfo_p[subsetIndex].subsetBitMap[CC_USER] |= (((UInt32)1) << (
                        prbPositionWithinSubset ));
            /* ICIC changes end */
            subsetInfo_p[subsetIndex].numPRBFree += 1;

            /*Get  the vrbNumber which is mapped to even slot of PRB*/
            vrbMappedToEvenSlotOfPRB  = localizedVRBInfo_p[startingRBNumber + 1].
                vrbMappedToEvenSlotOfPRB;
            /*Get  the vrbNumber which is mapped to odd slot of PRB*/
            vrbMappedToOddSlotOfPRB  = localizedVRBInfo_p[startingRBNumber + 1].
                vrbMappedToOddSlotOfPRB;
            if ( vrbMappedToEvenSlotOfPRB != vrbMappedToOddSlotOfPRB )
            {
                /*Updating the distributedVRBInfo_p array such that a Free VRB knows
                  the next Free VRB Index and also previous Free VRB Index*/
                previousFreeVRBIndex = distributedVRBInfo_p[vrbMappedToEvenSlotOfPRB].
                    previousVRBfreeIndex;
                nextFreeVRBIndex = distributedVRBInfo_p[vrbMappedToEvenSlotOfPRB]
                    .nextfreeVRBIndex;
                distributedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex =
                    vrbMappedToEvenSlotOfPRB;
                distributedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex =
                    vrbMappedToEvenSlotOfPRB;
                raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable += 1;
            }
            /*Updating the distributedVRBInfo_p array such that a Free VRB knows
              the next Free VRB Index and also previous Free VRB Index*/
            previousFreeVRBIndex = distributedVRBInfo_p[vrbMappedToOddSlotOfPRB].
                previousVRBfreeIndex;
            nextFreeVRBIndex = distributedVRBInfo_p[vrbMappedToOddSlotOfPRB].
                nextfreeVRBIndex;
            distributedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex =
                vrbMappedToOddSlotOfPRB;
            distributedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex =
                vrbMappedToOddSlotOfPRB;
            raPolicyInfo_p->rat2DistributedInfo.numOfVRBAvailable += 1;
            raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable += 1;
            
            if (CELL_EDGE_ONLY_REGION == (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->\
                    rbToResourceTypeMapDL[startingRBNumber])
            {
                raPolicyInfo_p->rat2LocalizedInfo.
                    numOfCellEdgeVRBAvailable += 1;
            }
        }
        /* ICIC changes end */
        startingRBNumber++;
    }
    
    /*Updating the localizedVRBInfo_p array such that a Free VRB knows
      the next Free VRB Index and also previous Free VRB Index*/
    /* ICIC changes start */
    /* Update RAT2 Localized information only if MIB region RBs are non blocked
     * as per current ICIC configuration
     */
    if (firstRBInMIBRegion)
    {
        previousFreeVRBIndex = localizedVRBInfo_p[endingRBNumber - 5].
            previousVRBfreeIndex;
        nextFreeVRBIndex = localizedVRBInfo_p[endingRBNumber].
            nextfreeVRBIndex;
        localizedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex = 
            firstRBInMIBRegion + 1;
        localizedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex =
            lastRBInMIBRegion + 1;
    }
    /* ICIC changes end */
}
/* SPR 3306 Fix Ends */

/*****************************************************************************
 * Function Name  : initializeRAPolicyMibScheduled
 * Inputs         : dlRAInfoIndex
 *                  internalCellIndex -  Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion will initialize the dlRAInfo_g[dlRAInfoIndex]. 
 *                  raPolicyInfoMIBReserved structure such that the center 6 RBs 
 *                  Have been reserved in RAT 0, RAT 1, RAT Localized and 
 *                  RAT 2 Distributed.
 *****************************************************************************/
/* ICIC changes start */
void initializeRAPolicyMibScheduled(UInt8 dlRAInfoIndex,InternalCellIndex internalCellIndex)
/* ICIC changes end */
{
    UInt32 dlRBs = 0;
/*  UInt32 endingRBNumber = 0; TDD Warning Fix*/
    UInt32 startingRBNumber = 0;
    UInt32 rbgIndex = 0;
    UInt32 rbgSize = 0;
    UInt32 maxRBCountForMIB = 6;
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;    
    UInt32 subsetIndex= 0;
    SubsetInfo *subsetInfo_p = PNULL;
    UInt32 prbPositionWithinSubset= 0;
    UInt32 previousFreeVRBIndex = 0;
    UInt32 nextFreeVRBIndex = 0;
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;  
    RAT2DistributedVRBInfo  *distributedVRBInfo_p = PNULL;
    UInt32 vrbMappedToEvenSlotOfPRB  = 0;
    UInt32 vrbMappedToOddSlotOfPRB  = 0;
    /* ICIC changes start */
    UInt32 firstRBInMIBRegion = 0;
    UInt32 lastRBInMIBRegion = 0;
    RAT2LocalizedInfo *rat2LocalizedInfo_p = &((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex)->\
        raPolicyInfoMIBReserved.rat2LocalizedInfo);
        
    localizedVRBInfo_p = ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex)->raPolicyInfoMIBReserved.
        rat2LocalizedInfo.rat2LocalizedVRBInfo);
    distributedVRBInfo_p = ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex)->raPolicyInfoMIBReserved.
        rat2DistributedInfo.rat2DistributedVRBInfo);
    rat1Info_p = &((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex)->raPolicyInfoMIBReserved.rat1Info);
    rat0Info_p = &((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex)->raPolicyInfoMIBReserved.rat0Info);

    /* MIB nRB FIX start */
    mibOverlapRBs_g[internalCellIndex][CC_CE_REGION] = 0;
    mibOverlapRBs_g[internalCellIndex][CE_REGION] = 0;
    /* MIB nRB FIX end */
    /* ICIC changes end */
    
    dlRBs =cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
           dlAvailableRBs;
    rbgSize = rat0Info_p->rbgSize;

    if (MODULO_TWO(dlRBs))
    {
        startingRBNumber = DIVIDE_BY_TWO(dlRBs) - 1;  
/* endingRBNumber =  startingRBNumber + 5; TDD Warning Fix */
        startingRBNumber--;
    }
    else
    {
        startingRBNumber = DIVIDE_BY_TWO(dlRBs) - 2;  
/* endingRBNumber =  startingRBNumber + 5; TDD Warning Fix */
        startingRBNumber--;
    }
#ifdef PERF_STATS
    central6RBStartIndex_g[internalCellIndex] = startingRBNumber;
#endif

    subsetInfo_p = rat1Info_p->subsetInfo;
    while (maxRBCountForMIB--)
    {
        /* ICIC changes start */
        /* update RA policy information for MIB reserved RBs only if they are
         * Non-Blocked RBs
         */
        if ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex)->rbToResourceTypeMapDL[startingRBNumber])
        {
            mibOverlapRBs_g[internalCellIndex][CC_CE_REGION]++; /* MIB nRB FIX */

            rbgIndex =  (startingRBNumber ) / rbgSize;
            subsetIndex = (rbgIndex ) % rbgSize;
            /* ICIC changes start*/
            /* Find first and last available RB in MIB region to update 
             * RAT2 Localized information.
             */
            if (!firstRBInMIBRegion)
            {
                firstRBInMIBRegion = startingRBNumber;
            }
            lastRBInMIBRegion = startingRBNumber;
            
            rat0Info_p->rat0BitMap[CC_USER] &= (~((UInt32)1 << rbgIndex));

            prbPositionWithinSubset =  ((startingRBNumber / rbgSize) / (rbgSize ) ) * 
                rbgSize +  (startingRBNumber % rbgSize);
            subsetInfo_p[subsetIndex].numPRBFree -= 1;
            subsetInfo_p[subsetIndex].subsetBitMap[CC_USER] &= (~(((UInt32)1) << (
                            prbPositionWithinSubset )));  

            /*Get  the vrbNumber which is mapped to even slot of PRB*/
            vrbMappedToEvenSlotOfPRB  = localizedVRBInfo_p[startingRBNumber + 1].
                vrbMappedToEvenSlotOfPRB;
            /*Get  the vrbNumber which is mapped to odd slot of PRB*/
            vrbMappedToOddSlotOfPRB  = localizedVRBInfo_p[startingRBNumber + 1].
                vrbMappedToOddSlotOfPRB;
            if ( vrbMappedToEvenSlotOfPRB != vrbMappedToOddSlotOfPRB )
            {
                /*Updating the distributedVRBInfo_p array such that a Free VRB knows
                  the next Free VRB Index and also previous Free VRB Index*/     
                previousFreeVRBIndex = distributedVRBInfo_p[vrbMappedToEvenSlotOfPRB].
                    previousVRBfreeIndex;
                nextFreeVRBIndex = distributedVRBInfo_p[vrbMappedToEvenSlotOfPRB]
                    .nextfreeVRBIndex;
                distributedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex = 
                    nextFreeVRBIndex;
                distributedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex = 
                    previousFreeVRBIndex;
                (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex)->raPolicyInfoMIBReserved.
                    rat2DistributedInfo.numOfVRBAvailable -= 1;
            }

            /*Updating the distributedVRBInfo_p array such that a Free VRB knows
              the next Free VRB Index and also previous Free VRB Index*/          
            previousFreeVRBIndex = distributedVRBInfo_p[vrbMappedToOddSlotOfPRB].
                previousVRBfreeIndex;
            nextFreeVRBIndex = distributedVRBInfo_p[vrbMappedToOddSlotOfPRB].
                nextfreeVRBIndex;
            distributedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex = 
                nextFreeVRBIndex;
            distributedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex = 
                previousFreeVRBIndex;    

            (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex)->raPolicyInfoMIBReserved.
                rat2DistributedInfo.numOfVRBAvailable -= 1;
            /* SPR 1972 changes start */ 
            (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex)->raPolicyInfoMIBReserved.
                rat2LocalizedInfo.numOfVRBAvailable -= 1;
            /* SPR 1972 changes end */
     
            if (CELL_EDGE_ONLY_REGION == (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex)->\
                    rbToResourceTypeMapDL[startingRBNumber])
            {
                mibOverlapRBs_g[internalCellIndex][CE_REGION]++; /* MIB nRB FIX */

                rat2LocalizedInfo_p->numOfCellEdgeVRBAvailable -= 1;

                /* RAT2 distributed support for ICIC needs to be added */      
            }
        }
        /* ICIC changes end */
        
        startingRBNumber++;
    }

    /*Updating the localizedVRBInfo_p array such that a Free VRB knows
      the next Free VRB Index and also previous Free VRB Index*/       
    /* ICIC changes start */
    /* Update RAT2 Localized information only if MIB region RBs are non blocked
     * as per current ICIC configuration
     */
    if (firstRBInMIBRegion)
    {
        previousFreeVRBIndex = localizedVRBInfo_p[firstRBInMIBRegion + 1].
            previousVRBfreeIndex;
        nextFreeVRBIndex = localizedVRBInfo_p[lastRBInMIBRegion + 1].
            nextfreeVRBIndex;
        localizedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex =  
            nextFreeVRBIndex;
        localizedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex =  
            previousFreeVRBIndex;     
    }
    
    if (rat2LocalizedInfo_p->numOfCellEdgeVRBAvailable)
    {
        /* update Cell Edge information for RAT2 Localized */ 
        if ((rat2LocalizedInfo_p->firstCellEdgeFreeIndex < nextFreeVRBIndex) &&
                (rat2LocalizedInfo_p->firstCellEdgeFreeIndex > 
                 previousFreeVRBIndex))
        {
            rat2LocalizedInfo_p->firstCellEdgeFreeIndex = nextFreeVRBIndex;
        }
        if ((rat2LocalizedInfo_p->endCellEdgeFreeIndex < nextFreeVRBIndex) &&
                (rat2LocalizedInfo_p->endCellEdgeFreeIndex > 
                 previousFreeVRBIndex))
        {
            rat2LocalizedInfo_p->endCellEdgeFreeIndex = previousFreeVRBIndex;
        }
    }
    /*Dynamic PDCCH + 5117 changes starts */
    CAL_RAT0_MIB_RESRC_ASSGN_BIT_MAP(rat0Info_p->mibResrcAssignementBitMap,rat0Info_p);
    /*Dynamic PDCCH + 5117 changes ends */
    /* ICIC changes end */
}
/* + TM7_8 Changes Start */
/*****************************************************************************
 * Function Name  : checkCodeRateUECategoryForTransMode_7_8
 * Inputs         : ueContext_p - Pointer to DLUEContext,
 *                  harqProcess_p - Pointer to DLHARQProcess
 *                  internalCellIndex -  Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function wil Do code rate check for Transmission 
 *                  Mode 7 and 8 
 *****************************************************************************/
MacRetType  checkCodeRateUECategoryForTransMode_7_8(
        DLUEContext *ueContext_p,
        DLHARQProcess *harqProcess_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 ueCategoryIndex          = 0;
    UInt8 mcsOne                   = 0;
    UInt8 mcsTwo                   = 0;
    UInt32 permissibleTBSizeInBytes= 0;
    UInt32 tbOneSize               = 0;
    UInt32 tbTwoSize               = 0;
    UInt32 tbSize                  = 0;
    UInt8 rbCounter                = 0;
    UInt8 allocatedRb              = 0;
    UInt8 mcsIndexToBeUsed         = 0;

    /*Firstly update the MCS indexi and then TBSize ,if needed, depending upon the number of assignedResourcesNB*/
    updateMCSIndexAndTBSIzeForTransMode_7_8(harqProcess_p,
            internalCellIndex);

    /*Now update the number of RBs,if needed, depending upon the permissibleTBSize for the particular ueCategory
     * ueCategoryIndex is one less than actual uecategory*/
    ueCategoryIndex = ueContext_p->ueCategory - 1;
    if (ueCategoryIndex < 3)
    {
        if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex))
        {
            permissibleTBSizeInBytes = ueContext_p->permissibleTBSize;
        }
        else
        {
            permissibleTBSizeInBytes = ueContext_p->dlUeScellContext_p[1]->permissibleTBSize;
        }
        if (harqProcess_p->isTBOneValid != TB_VALID)
        {
            mcsOne = 29;
        }
        else
        {
            mcsOne = harqProcess_p->dlHARQTBOneInfo.mcsIndex; 
        }
        if (harqProcess_p->isTBTwoValid != TB_VALID)
        {
            mcsTwo = 29;
        }
        else
        {
            mcsTwo = harqProcess_p->dlHARQTBTwoInfo.mcsIndex; 
        }
        rbCounter   = ueCategoryMap_g [ueCategoryIndex][mcsOne][mcsTwo];
        allocatedRb = rbCounter;
        if (harqProcess_p->assignedResourcesNB > allocatedRb)
        {
            /*SPR:6799:START*/
            harqProcess_p->assignedResourcesNB = allocatedRb;
            /*SPR:6799:END*/
            if ((harqProcess_p->isTBOneValid == TB_VALID) &&
                (harqProcess_p->isTBTwoValid == TB_VALID))
            {
                /*SPR:6799:START*/
                harqProcess_p->dlHARQTBOneInfo.tbSize = 
                           determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                           harqProcess_p->assignedResourcesNB,
                                           TB_MAPPED_TO_ONE_LAYER);
                harqProcess_p->dlHARQTBTwoInfo.tbSize =
                           determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                           harqProcess_p->assignedResourcesNB,
                                           TB_MAPPED_TO_ONE_LAYER);
                                                                                 
                /*SPR:6799:END*/
                tbOneSize = harqProcess_p->dlHARQTBOneInfo.tbSize;
                tbTwoSize = harqProcess_p->dlHARQTBTwoInfo.tbSize;
                while  ((tbOneSize + tbTwoSize  > permissibleTBSizeInBytes)
                         && (allocatedRb > 0))
                {
                    allocatedRb--;
                    harqProcess_p->assignedResourcesNB    = allocatedRb;
                    harqProcess_p->dlHARQTBOneInfo.tbSize = \
                                       determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex, 
				       allocatedRb,
                                       TB_MAPPED_TO_ONE_LAYER);
                    tbOneSize = harqProcess_p->dlHARQTBOneInfo.tbSize;
                    harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                                       determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex, 
				       allocatedRb,
                                       TB_MAPPED_TO_ONE_LAYER);
                    tbTwoSize = harqProcess_p->dlHARQTBTwoInfo.tbSize;
                }
                if((tbOneSize + tbTwoSize  > permissibleTBSizeInBytes)
                         && (allocatedRb == 0))
                {
                    lteWarning("[%s]:1:RBs to be allocated became ZERO after considering the UE category\n",__func__);
                    return MAC_FAILURE;
                }
            }
            else
            {
                if (harqProcess_p->isTBOneValid == TB_VALID)
                {
                    mcsIndexToBeUsed = harqProcess_p->dlHARQTBOneInfo.mcsIndex;
                    /*SPR:6799:START*/
                    harqProcess_p->dlHARQTBOneInfo.tbSize = 
                                   determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                                   harqProcess_p->assignedResourcesNB,
                                                   TB_MAPPED_TO_ONE_LAYER);
                    /*SPR:6799:END*/
                    tbSize           = harqProcess_p->dlHARQTBOneInfo.tbSize;
                }
                else
                {
                    mcsIndexToBeUsed = harqProcess_p->dlHARQTBTwoInfo.mcsIndex;
                    /*SPR:6799:START*/
                    harqProcess_p->dlHARQTBTwoInfo.tbSize =
                                   determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                                   harqProcess_p->assignedResourcesNB,
                                                   TB_MAPPED_TO_ONE_LAYER);
                                                                                 
                    /*SPR:6799:END*/
                    tbSize           = harqProcess_p->dlHARQTBTwoInfo.tbSize;
                }
                while (tbSize  > permissibleTBSizeInBytes)
                {
                    allocatedRb--;
                    harqProcess_p->assignedResourcesNB = allocatedRb;
                    tbSize = determineTBSize(mcsIndexToBeUsed, allocatedRb,TB_MAPPED_TO_ONE_LAYER);
                }
                if((tbSize  > permissibleTBSizeInBytes)
                         && (allocatedRb == 0))
                {
                    lteWarning("[%s]:2:RBs to be allocated became ZERO after considering the UE category\n",__func__);
                    return MAC_FAILURE;
                }

                if (harqProcess_p->isTBOneValid == TB_VALID)
                {
                    harqProcess_p->dlHARQTBOneInfo.tbSize = tbSize;
                }
                else
                {
                    harqProcess_p->dlHARQTBTwoInfo.tbSize = tbSize;
                }
            }
        }
    }
    return MAC_SUCCESS;
}    
/*****************************************************************************
 * Function Name  : updateMCSIndexAndTBSIzeForTransMode_7_8
 * Inputs         : harqProcess_p - Pointer to DLHARQProcess
 *                  internalCellIndex -  Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Descriptioni   : This function will update MCS Index acc to the number of 
 *                  assignedResourcesNB So that code rate won't exceed and 
 *                  we get the best suited TBSIZE It is analysed that whenever 
 *                  Transmode is 7 or 8 *Always reducing MCS index gives the 
 *                  appropriate TB size.With that analysis various static array 
 *                  was made acc to number Antenna and CFI value which will be used to 
 *                  update the MCS Index
 *****************************************************************************/
void updateMCSIndexAndTBSIzeForTransMode_7_8(
        DLHARQProcess *harqProcess_p,
        InternalCellIndex internalCellIndex)
{
    switch(currentCFIValue_g[internalCellIndex])
    {
        case 1 :
        /*With CFI_ONE and Number of anteena 2 ,code rate 
         *was sometimes exceeding with MCS Index 28 on some RBS*/

        /* Coverity_73546/73464 Fix Start */ 
        if((0 < harqProcess_p->assignedResourcesNB) && (harqProcess_p->assignedResourcesNB < MAX_NUM_OF_RB))
        {
            if((harqProcess_p->isTBOneValid)&&
               (harqProcess_p->dlHARQTBOneInfo.mcsIndex > (maxMcsIdxForTM7_8_CFIOneNumOfAntennaTwo[harqProcess_p->assignedResourcesNB - 1])))
            {
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = 
                     maxMcsIdxForTM7_8_CFIOneNumOfAntennaTwo[harqProcess_p->assignedResourcesNB - 1];
            }
            if((harqProcess_p->isTBTwoValid)&&
               (harqProcess_p->dlHARQTBTwoInfo.mcsIndex > (maxMcsIdxForTM7_8_CFIOneNumOfAntennaTwo[harqProcess_p->assignedResourcesNB - 1])))
            {
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex = 
                     maxMcsIdxForTM7_8_CFIOneNumOfAntennaTwo[harqProcess_p->assignedResourcesNB - 1];
            }
        } 
        /* Coverity_73546/73464 Fix End */ 

        break;
        case 2 :
            /*When CFI_TWO code rate was exceeding on every RB count for MCS index 28*/
            if((harqProcess_p->isTBOneValid)&&
               (harqProcess_p->dlHARQTBOneInfo.mcsIndex == 28))
            {
                harqProcess_p->dlHARQTBOneInfo.mcsIndex -= 1;
            }
            if((harqProcess_p->isTBTwoValid)&&
               (harqProcess_p->dlHARQTBTwoInfo.mcsIndex == 28))
            {
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex -= 1;
            }
            break;
        case 3 :
        /*When CFI_THREE code rate was exceeding on every RB count with MCS index till 26 
         *so with that analysis a array was made which will tell the max MCS index till 
         *which coderate will never excced for each RBCout which is used here*/     

        /* Coverity_73546/73464 Fix Start */ 
        if((0 < harqProcess_p->assignedResourcesNB) && (harqProcess_p->assignedResourcesNB < MAX_NUM_OF_RB))
        {
            if((harqProcess_p->isTBOneValid)&&
               (harqProcess_p->dlHARQTBOneInfo.mcsIndex > (maxMcsIdxForTM7_8_CFIThreeNumOfAntennaTwo[harqProcess_p->assignedResourcesNB - 1])))
            {
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = 
                     maxMcsIdxForTM7_8_CFIThreeNumOfAntennaTwo[harqProcess_p->assignedResourcesNB - 1];
            }
            if((harqProcess_p->isTBTwoValid)&&
               (harqProcess_p->dlHARQTBTwoInfo.mcsIndex > (maxMcsIdxForTM7_8_CFIThreeNumOfAntennaTwo[harqProcess_p->assignedResourcesNB - 1])))
            {
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex = 
                     maxMcsIdxForTM7_8_CFIThreeNumOfAntennaTwo[harqProcess_p->assignedResourcesNB - 1];
            }
        } 
        /* Coverity_73546/73464 Fix End */ 

        break;
        default :
        /*Nothing*/
        break;
    }

    /* Coverity_73546/73464 Fix Start */
    if(0 < harqProcess_p->assignedResourcesNB)
    {    
        harqProcess_p->dlHARQTBOneInfo.tbSize = \
                                                determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex, 
                                                        harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_ONE_LAYER);

        harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                                                determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex, 
                                                        harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_ONE_LAYER);
    }
    /* Coverity_73546/73464 Fix End */


}
/* - TM7_8 Changes End */

/******************************************************************************
 * Function Name    : checkCodeRateUECategoryNewTx
 * Inputs           : raPolicy,
 *                    harqProcess_p - Pointer to DLHARQProcess,
 *                    ueContext_p - Pointer to DLUEContext,
 *                    availableRBGBitMap,
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function find out the best combination that gives the best throughput. 
 *                    This is only needed in case of non0/5 subframes and only when mcs is 28. 
 *                    In 0/5 SF, we always need to reduce MCS and in non0/5, when MCS is not equal to
 *                    28, coderate issues are never seen. This function is used when both TB 
 *                    are new Tx
 ******************************************************************************/ 
static MacRetType  checkCodeRateUECategoryNewTx(
                        UInt8 raPolicy, 
        /* +- SPR 17777 */
		        DLHARQProcess *harqProcess_p,
		        DLUEContext *ueContext_p,
		        UInt32 availableRBGBitMap,
        /* +- SPR 17777 */
                InternalCellIndex internalCellIndex)
{
    UInt8 sizeOfLastRbg = 0;
    UInt8 rbgSize = 0;
    UInt8 numFreePRB = 0;
    UInt8 lastRBGAvailable = 0;
    UInt8 numAvailRBExcludingLastRBG = 0;
    RAT0Info *rat0Info_p = PNULL;
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    if (raPolicy == RA_POLICY_0)
    {
        rat0Info_p =  &((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->\
                    raPolicyInfo.rat0Info);
        rbgSize = rat0Info_p->rbgSize;
        sizeOfLastRbg = (cellParams_p->dlAvailableRBs % rbgSize);

        if (availableRBGBitMap & ((UInt32) 1 << (rat0Info_p->rbgCount - 1)))
        {
            numFreePRB  = getCount(availableRBGBitMap) * rbgSize;
            numFreePRB = numFreePRB - rbgSize +
                rat0Info_p->rbgIndexInfo[rat0Info_p->rbgCount - 1].numOfRBs;
            lastRBGAvailable = TRUE;
        }

        if((0 !=  sizeOfLastRbg) && (lastRBGAvailable))
        {
	    /* SPR 13666 fix start */
 	    numAvailRBExcludingLastRBG = numFreePRB - (sizeOfLastRbg);
	    /* SPR 13666 fix end */
    

            if ((lastRBGAvailable) &&
                    (harqProcess_p->assignedResourcesNB > numAvailRBExcludingLastRBG))
            {
                /* This is the only case when lastRBG will be
                 * allocated.*/
            /*    harqProcess_p->assignedResourcesNB = allocatedRb;
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = mcsIndexForTbOne;
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex = mcsIndexForTbTwo;*/
                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5)  || 
                    (ueContext_p->ueCategory == UE_CAT_8))&&
    	                    ( (harqProcess_p->numOfLayer == RANK_INDICATOR_3) ||
                      (harqProcess_p->numOfLayer == RANK_INDICATOR_4) ))
                /* SPR 11355 Fix End */
    	                {
                        /** klockwork Changes Start **/    
                              updateMCSRBbelowCoderateForRAT0For4x4Mimo(lastRBGAvailable,
                                                                        harqProcess_p,
                                                                        /* +- SPR 17777 */
                                                                        internalCellIndex);
    	                }
                        /** klockwork Changes End **/    
    	                /* 4x4 DL MIMO CHG END */
                else
                { 
                    if(MAC_FAILURE == updateMCSRBbelowUECategoryCoderateForRAT0(lastRBGAvailable,
                                                                                harqProcess_p,
                                                                                ueContext_p,
                                                                                internalCellIndex))
                    {
                        /* SPR 13903 fix start */
                        LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                                MAC_PDSCH,getCurrentTick(),(UInt32)ueContext_p->ueIndex,
                                raPolicy,harqProcess_p->assignedResourcesNB,
                                harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"");
                        /* SPR 13903 fix end */

                        return MAC_FAILURE;
                    }
                }
            }
            else
            {
    	                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) || (ueContext_p->ueCategory == UE_CAT_8)) &&
    	                  ( (harqProcess_p->numOfLayer == RANK_INDICATOR_3) ||(harqProcess_p->numOfLayer == RANK_INDICATOR_4) ))
                /* SPR 11355 Fix End */
    	                {
                        /** klockwork Changes Start **/    
                            updateMCSRBbelowCoderateForRAT0For4x4Mimo(0,
                                                                      harqProcess_p,
                                                                      /* +- SPR 17777 */
                                                                      internalCellIndex);
    	                }
                        /** klockwork Changes End **/    
    	                /* 4x4 DL MIMO CHG END */
                else
                { 
                    /* Last RBG will not be available */
                    if (MAC_FAILURE == updateMCSRBbelowUECategoryCoderateForRAT0(0,
                                                                            harqProcess_p,
                                                                            ueContext_p,
                                                                            internalCellIndex))
                    {
                        /* SPR 13903 fix start */
                        LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                                MAC_PDSCH,getCurrentTick(),(UInt32)ueContext_p->ueIndex,
                                raPolicy,harqProcess_p->assignedResourcesNB,
                                harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"");
                        /* SPR 13903 fix end */

                        return MAC_FAILURE;
                    }
                }
            }
        }
        else
        {
    	            /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
            if( ((ueContext_p->ueCategory == UE_CAT_5) || (ueContext_p->ueCategory == UE_CAT_8)) &&
    	            ( (harqProcess_p->numOfLayer == RANK_INDICATOR_3) ||(harqProcess_p->numOfLayer == RANK_INDICATOR_4) ))
                /* SPR 11355 Fix End */
    	            {
                        /** klockwork Changes Start **/    
                         updateMCSRBbelowCoderateForRAT0For4x4Mimo(0,
                                                                   harqProcess_p,
                                                                   /* +- SPR 17777 */
                                                                   internalCellIndex);
    	            }
                        /** klockwork Changes End **/    
    	            /* 4x4 DL MIMO CHG END */
            else
            { 
                if(MAC_FAILURE == updateMCSRBbelowUECategoryCoderateForRAT0(0,
                                                                            harqProcess_p,
                                                                            ueContext_p,
                                                                            internalCellIndex))
                {
                    /* SPR 13903 fix start */
                    LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                            MAC_PDSCH,getCurrentTick(),(UInt32)ueContext_p->ueIndex,
                            raPolicy,harqProcess_p->assignedResourcesNB,
                            harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                            harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"");
                    /* SPR 13903 fix end */

                    return MAC_FAILURE;
                }
    	            }
        }
    }
    else
    {
                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
        if( ((ueContext_p->ueCategory == UE_CAT_5) || (ueContext_p->ueCategory == UE_CAT_8)) &&
                ( (harqProcess_p->numOfLayer == RANK_INDICATOR_3) ||(harqProcess_p->numOfLayer == RANK_INDICATOR_4) ))
                /* SPR 11355 Fix End */
        /** klockwork Changes Start **/    
        {
                    updateMCSRBbelowCoderateForRAT1AndRAT2For4x4Mimo( harqProcess_p,
                                                                    /* +- SPR 17777 */
                                                                                internalCellIndex);
        }
        /** klockwork Changes End **/    
        /* 4x4 DL MIMO CHG END */
        else
        { 
            /* RAT1/2*/
            if(MAC_FAILURE == updateMCSRBbelowUECategoryCoderateForRAT1AndRAT2(harqProcess_p,
                                                                               ueContext_p,
                                                                               internalCellIndex))
            {
                /* SPR 13903 fix start */
                LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                        MAC_PDSCH,getCurrentTick(),(UInt32)ueContext_p->ueIndex,
                        raPolicy,harqProcess_p->assignedResourcesNB,
                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
                /* SPR 13903 fix end */

                return MAC_FAILURE;
            }
        }
    }
    return MAC_SUCCESS;
}

/******************************************************************************
 * Function Name    : checkCodeRateUECategoryNewTxAndNotReTx
 * Inputs           : raPolicy,
 *                    scheduledTick - Scheduled TTI,
 *                    harqProcess_p - Pointer to DLHARQProcess,
 *                    ueContext_p - Pointer to DLUEContext,
 *                    ueCategoryChecksRequired,
 *                    availableRBGBitMap,
 *                    tempRBGBitMap
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function find out the best combination that gives the best throughput. 
 *                    This is only needed in case of non0/5 subframes and only when mcs is 28. 
 *                    In 0/5 SF, we always need to reduce MCS and in non0/5, when MCS is not equal to
 *                    28, coderate issues are never seen. This function is used when 1 TB is new Tx
 *                    and the other TB is not retx.
 ******************************************************************************/ 
static MacRetType  checkCodeRateUECategoryNewTxAndNotReTx(
                        UInt8 raPolicy, 
                /* +- SPR 17777 */
		        DLHARQProcess *harqProcess_p,
		        DLUEContext *ueContext_p,
                /* +- SPR 17777 */
		        UInt32 availableRBGBitMap,
                /* +- SPR 17777 */
                InternalCellIndex internalCellIndex)
{
    /* SPR 13666 fix start */
    UInt8 sizeOfLastRbg = 0;
    UInt8 rbgSize = 0;
    UInt8 numFreePRB = 0;
    UInt8 lastRBGAvailable = 0;
    UInt8 numAvailRBExcludingLastRBG = 0;
    RAT0Info *rat0Info_p = PNULL;
    /* SPR 13666 fix end */

    if (raPolicy == RA_POLICY_0)
    {
        /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
        if( ((ueContext_p->ueCategory == UE_CAT_5) || (ueContext_p->ueCategory == UE_CAT_8)) &&
                  ( (harqProcess_p->numOfLayer == RANK_INDICATOR_3) ||(harqProcess_p->numOfLayer == RANK_INDICATOR_4) ))
                /* SPR 11355 Fix End */
                  {
                  /** klockwork Changes Start **/    
                       updateMCSRBbelowCoderateForRAT0For4x4Mimo(0,
                                                                 harqProcess_p,
                                                                 /* +- SPR 17777 */
                                                                 internalCellIndex);
                 }
                 /** klockwork Changes End **/    
                 /* 4x4 DL MIMO CHG END */
         else
         {
             /* SPR 13666 fix start */
             rat0Info_p =  &((dlRAInfo_gp[internalCellIndex] +
                         dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info);

             rbgSize = rat0Info_p->rbgSize;
             sizeOfLastRbg =
                 (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                  dlAvailableRBs % rbgSize);
             if (availableRBGBitMap & ((UInt32) 1 << (rat0Info_p->rbgCount - 1)))
             {
                 numFreePRB = getCount(availableRBGBitMap) * rbgSize;
                 numFreePRB = numFreePRB - rbgSize +
                     rat0Info_p->rbgIndexInfo[rat0Info_p->rbgCount - 1].numOfRBs;
                 lastRBGAvailable = TRUE;
             }

             if((0 != sizeOfLastRbg) && (lastRBGAvailable))
             {
                 numAvailRBExcludingLastRBG = numFreePRB - (sizeOfLastRbg);

                 if ((lastRBGAvailable) &&
                         (harqProcess_p->assignedResourcesNB > numAvailRBExcludingLastRBG))
                 {
                     if(MAC_FAILURE == updateMCSRBbelowUECategoryCoderateForRAT0(lastRBGAvailable,
                                 harqProcess_p,
                                 ueContext_p,
                                 internalCellIndex))
                     {
                        /* SPR 13903 fix start */
                        LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                            MAC_PDSCH,getCurrentTick(),(UInt32)ueContext_p->ueIndex,
                            raPolicy,harqProcess_p->assignedResourcesNB,
                            harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                            harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"");
                        /* SPR 13903 fix end */
                         return MAC_FAILURE;
                     }
                 }
                 else
                 {
                     if(MAC_FAILURE == updateMCSRBbelowUECategoryCoderateForRAT0(0,
                                 harqProcess_p,
                                 ueContext_p,
                                 internalCellIndex))
                     {
                    /* SPR 13903 fix start */
                    LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                            MAC_PDSCH,getCurrentTick(),(UInt32)ueContext_p->ueIndex,
                            raPolicy,harqProcess_p->assignedResourcesNB,
                            harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                            harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"");
                    /* SPR 13903 fix end */
                         return MAC_FAILURE;
                     }
                 }
             }
             else
             {
                 if(MAC_FAILURE == updateMCSRBbelowUECategoryCoderateForRAT0(0,
                             harqProcess_p,
                             ueContext_p,
                             internalCellIndex))
                 {
                /* SPR 13903 fix start */
                LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                        MAC_PDSCH,getCurrentTick(),(UInt32)ueContext_p->ueIndex,
                        raPolicy,harqProcess_p->assignedResourcesNB,
                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
                /* SPR 13903 fix end */
                     return MAC_FAILURE;
                 }
             }
             /* SPR 13666 fix end */
         }
    }
    else
    {
            /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
        if( ((ueContext_p->ueCategory == UE_CAT_5) || (ueContext_p->ueCategory == UE_CAT_8)) &&
                ( (harqProcess_p->numOfLayer == RANK_INDICATOR_3) ||(harqProcess_p->numOfLayer == RANK_INDICATOR_4) ))
                /* SPR 11355 Fix End */
                {
                /** klockwork Changes Start **/    
                     updateMCSRBbelowCoderateForRAT1AndRAT2For4x4Mimo ( harqProcess_p,
                                                                    /* +- SPR 17777 */
                                                                                 internalCellIndex);
                /** klockwork Changes End **/    
        }
        /* 4x4 DL MIMO CHG END */
        else
        { 
            /* RAT1/2*/
            if(MAC_FAILURE == updateMCSRBbelowUECategoryCoderateForRAT1AndRAT2(harqProcess_p,
                                                                               ueContext_p,
                                                                               internalCellIndex))
            {
                /* SPR 13903 fix start */
                LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                        MAC_PDSCH,getCurrentTick(),(UInt32)ueContext_p->ueIndex,
                        raPolicy,harqProcess_p->assignedResourcesNB,
                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
                /* SPR 13903 fix end */

                return MAC_FAILURE;
            }
        }
    }
    return MAC_SUCCESS;
}

/******************************************************************************
 * Function Name    : checkCodeRateUECategoryLessThan3
 * Inputs           : 
 *                    harqProcess_p - Pointer to DLHARQProcess,
 *                    ueContext_p - Pointer to DLUEContext,
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function find out the best combination that gives the best throughput. 
 *                    This is only needed in case of non0/5 subframes and only when mcs is 28. 
 *                    In 0/5 SF, we always need to reduce MCS and in non0/5, when MCS is not equal to
 *                    28, coderate issues are never seen. this funcion is only meanr when 
 *                    ue category is involved.
 ******************************************************************************/ 
/** klockwork Changes Start **/
static void   checkCodeRateUECategoryLessThan3(
                /* +- SPR 17777 */
		        DLHARQProcess *harqProcess_p,
		        DLUEContext *ueContext_p,
                /* +- SPR 17777 */
                InternalCellIndex internalCellIndex)
{
    UInt8 mcsOne = 0;
    UInt8 mcsTwo = 0;
    UInt8 rbCounter = 0;
    UInt8 mcsIndexToBeUsed = 0;
    UInt8 allocatedRb = 0;
    UInt8 ueCategoryIndex = ueContext_p->ueCategory;
    /*SPR:6759:START*/
    UInt32 permissibleTBSizeInBytes = 0;
    /*SPR:6759:END*/
    UInt32 tbOneSize = 0;
    UInt32 tbTwoSize = 0;
    UInt32 tbSize =0;
    /* ueCategoryIndex is one less than actual uecategory*/
    ueCategoryIndex = ueCategoryIndex - 1;
    if (ueCategoryIndex < 3)
    {
        /*SPR:6759:START*/
        if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex))
        {
            permissibleTBSizeInBytes = ueContext_p->permissibleTBSize;
        }
        else
        {
            permissibleTBSizeInBytes = ueContext_p->dlUeScellContext_p[1]->permissibleTBSize;
        }
        /*SPR:6759:END*/
        if (harqProcess_p->isTBOneValid != TB_VALID)
        {
            mcsOne = 29;
        }
        else
        {
            mcsOne = harqProcess_p->dlHARQTBOneInfo.mcsIndex; 
        }
        if (harqProcess_p->isTBTwoValid != TB_VALID)
        {
            mcsTwo = 29;
        }
        else
        {
            mcsTwo = harqProcess_p->dlHARQTBTwoInfo.mcsIndex; 
        }
        rbCounter = ueCategoryMap_g [ueCategoryIndex][mcsOne][mcsTwo];
        allocatedRb = rbCounter;
        if (harqProcess_p->assignedResourcesNB > allocatedRb)
        {
            /*SPR 16881 Fix Start*/
            /*Code deleted*/
            /*SPR 16881 Fix End*/
             /*+CSR 55962 Start
             * No change in TBSize required for RETX case*/
            if (((harqProcess_p->isTBOneValid == TB_VALID)&&
                        (NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator)) 
                    &&
                    ((harqProcess_p->isTBTwoValid == TB_VALID)) &&
                    (NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator))
            /*-CSR 55962 End*/
            {
                /* SPR 14890 fix start */
                /*SPR:6799:START*/
                harqProcess_p->assignedResourcesNB = allocatedRb;
                /*SPR:6799:END*/
                /* SPR 14890 fix end */
                /*SPR:6799:START*/
                harqProcess_p->dlHARQTBOneInfo.tbSize = 
                           determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                           harqProcess_p->assignedResourcesNB,
                                           /*4X4 DL MIMO CHG START*/
                                           TB_MAPPED_TO_ONE_LAYER
                                           /*4X4 DL MIMO CHG END*/);
                harqProcess_p->dlHARQTBTwoInfo.tbSize =
                          determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                          harqProcess_p->assignedResourcesNB,
                                          /*4X4 DL MIMO CHG START*/
                                          TB_MAPPED_TO_ONE_LAYER
                                          /*4X4 DL MIMO CHG END*/);
                                                                     
                /*SPR:6799:END*/
                /*SPR:6759:START*/
                tbOneSize = harqProcess_p->dlHARQTBOneInfo.tbSize;
                tbTwoSize = harqProcess_p->dlHARQTBTwoInfo.tbSize;
                while((tbOneSize + tbTwoSize  > permissibleTBSizeInBytes)
                /*SPR:6759:END*/
                      &&(allocatedRb > 0))
                {
                    /*SPR:6759:START*/
                    allocatedRb--;
                    /*SPR:6759:END*/
                    harqProcess_p->assignedResourcesNB = allocatedRb;
                    harqProcess_p->dlHARQTBOneInfo.tbSize = \
                                       determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex, 
    								allocatedRb,
                            /*4X4 DL MIMO CHG START*/
                            TB_MAPPED_TO_ONE_LAYER
                            /*4X4 DL MIMO CHG END*/);
                    tbOneSize = harqProcess_p->dlHARQTBOneInfo.tbSize;
                    harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                                       determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex, 
    								allocatedRb,
                            /*4X4 DL MIMO CHG START*/
                            TB_MAPPED_TO_ONE_LAYER
                            /*4X4 DL MIMO CHG END*/);  
                    tbTwoSize = harqProcess_p->dlHARQTBTwoInfo.tbSize;
                }

            }
            else
            {
                 /*CSR 55962 Start*/
                /* No change in TBSize required for RETX case*/
                if (RE_TX != harqProcess_p->dlHARQTBOneInfo.txIndicator && 
                        RE_TX != harqProcess_p->dlHARQTBTwoInfo.txIndicator)
                {
                    /* SPR 14890 fix start */
                    /*SPR:6799:START*/
                    harqProcess_p->assignedResourcesNB = allocatedRb;
                    /*SPR:6799:END*/
                    /* SPR 14890 fix end */
                    if ((harqProcess_p->isTBOneValid == TB_VALID)&&
                            (NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator))
                    {
                        /*CSR 55962 End*/
                        mcsIndexToBeUsed = harqProcess_p->dlHARQTBOneInfo.mcsIndex;
                        /*SPR:6799:START*/
                        harqProcess_p->dlHARQTBOneInfo.tbSize = 
                               determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                               harqProcess_p->assignedResourcesNB,
                                               /*4X4 DL MIMO CHG START*/
                                               TB_MAPPED_TO_ONE_LAYER
                                               /*4X4 DL MIMO CHG END*/);
                        /*SPR:6799:END*/
                        /*SPR:6759:START*/
                        tbSize           = harqProcess_p->dlHARQTBOneInfo.tbSize;
                        /*SPR:6759:END*/
 
                    }
                    /*CSR 55962 Start*/
                    else if (harqProcess_p->isTBTwoValid == TB_VALID &&
                            NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
                    {
                        /*CSR 55962 End*/
                       mcsIndexToBeUsed = harqProcess_p->dlHARQTBTwoInfo.mcsIndex;
                        /*SPR:6799:START*/
 
                       harqProcess_p->dlHARQTBTwoInfo.tbSize =
                              determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                              harqProcess_p->assignedResourcesNB,
                                              /*4X4 DL MIMO CHG START*/
                                              TB_MAPPED_TO_ONE_LAYER
                                              /*4X4 DL MIMO CHG END*/);                       
                                                                         
                        /*SPR:6799:END*/
                        /*SPR:6759:START*/
                        tbSize           = harqProcess_p->dlHARQTBTwoInfo.tbSize;
                        /*SPR:6759:END*/
 
                    }
                    /*CSR 55962 Start*/
                    else
                    {
                        /** klockwork Changes Start **/
                        return ;
                        /** klockwork Changes End **/
                    }

                    /* softBit checks not needed if the transmission is a RE-Tx
                     * since UE-L1 will combine the TB with already stored TB
                     * and new DLSCH bits are not being transmitted*/
                    /*SPR:6759:START*/
                    while (tbSize  > permissibleTBSizeInBytes)
                        /*SPR:6759:END*/
                    {
                        /*CSR 55962 End*/
                        allocatedRb--;
                        harqProcess_p->assignedResourcesNB = allocatedRb;
                        tbSize = determineTBSize(mcsIndexToBeUsed, allocatedRb,
                                                 /*4X4 DL MIMO CHG START*/
                                                  TB_MAPPED_TO_ONE_LAYER
                                                 /*4X4 DL MIMO CHG END*/); 
                    }
                     /*CSR 55962 Start*/
                    if ((harqProcess_p->isTBOneValid == TB_VALID)&&
                            (NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator))
                    {
                        harqProcess_p->dlHARQTBOneInfo.tbSize = tbSize;
                    }
                    else if (harqProcess_p->isTBTwoValid == TB_VALID &&
                            NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
                    {
                        harqProcess_p->dlHARQTBTwoInfo.tbSize = tbSize;
                    }
                    /*CSR 55962 End*/
                }
            }
        }
    }
    return ;
}
/** klockwork Changes End **/
/*Dynamic PDCCH + 5117 changes starts */
/******************************************************************************
 * Function Name    : checkCodeRateUECategory
 * Inputs           : raPolicy,
 *                    scheduledTick - Scheduled TTI,
 *                    harqProcess_p - Pointer to DLHARQProcess,
 *                    ueContext_p - Pointer to DLUEContext,
 *                    ueCategoryChecksRequired,
 *                    availableRBGBitMap,
 *                    tempRBGBitMap
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function find out the best combination that gives the best throughput. 
 *                    This is only needed in case of non0/5 subframes and only when mcs is 28. 
 *                    In 0/5 SF, we always need to reduce MCS and in non0/5, when MCS is not equal to
 *                    28, coderate issues are never seen.
 ******************************************************************************/ 
MacRetType  checkCodeRateUECategory(
                        UInt8 raPolicy, 
	                /* SPR 15909 fix start */
		        tickType_t scheduledTick,
        	        /* SPR 15909 fix end */
		        DLHARQProcess *harqProcess_p,
		        DLUEContext *ueContext_p,
		        UInt8 ueCategoryChecksRequired,
		        UInt32 availableRBGBitMap,
                /* +- SPR 17777 */
                InternalCellIndex internalCellIndex)
{
    /* SPR 21631 Start */
     /* SPR 23393 Changes Start */
    if (((scheduledTick % MAX_SUB_FRAME) != 5) && ((scheduledTick % MAX_SUB_FRAME) != 0))
     /* SPR 23393 Changes End */
        /* SPR 21631 End */
    {      
        /* all this is to be done in case of non0/5 SF only*/
        if (((28 == harqProcess_p->dlHARQTBOneInfo.mcsIndex)||
                    (28 == harqProcess_p->dlHARQTBTwoInfo.mcsIndex)) &&
                (currentCFIValue_g[internalCellIndex]!= CFI_ONE))
        {
            if (NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator &&
                    NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
            {
    		if (MAC_FAILURE == checkCodeRateUECategoryNewTx(
                            /*spr 12995 fix start*/
					        raPolicy, 
                            /*spr 12995 fix end*/
                            /* +- SPR 17777 */
                			harqProcess_p,
                			ueContext_p,
                			availableRBGBitMap,
                            /* +- SPR 17777 */
                            internalCellIndex))
    		{
                    /* SPR 13903 fix start */
                    LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                            MAC_PDSCH,getCurrentTick(),(UInt32)ueContext_p->ueIndex,
                            raPolicy,harqProcess_p->assignedResourcesNB,
                            harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                            harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"");
                    /* SPR 13903 fix end */

        	    return MAC_FAILURE;
    		}
            }
            else if (((NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator) &&
                        (RE_TX != harqProcess_p->dlHARQTBTwoInfo.txIndicator))
                    ||
                    ((NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)&&
                     (RE_TX != harqProcess_p->dlHARQTBOneInfo.txIndicator)))
            {
    		if (MAC_FAILURE == checkCodeRateUECategoryNewTxAndNotReTx(
                            /*spr 12995 fix start*/
					        raPolicy, 
                            /*spr 12995 fix end*/
                            /* +- SPR 17777 */
                			harqProcess_p,
                			ueContext_p,
                			availableRBGBitMap,
                            /* +- SPR 17777 */
                            internalCellIndex))
					/* SPR 13666 fix end */
    		{
                /* SPR 13903 fix start */
                LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                        MAC_PDSCH,getCurrentTick(),(UInt32)ueContext_p->ueIndex,
                        raPolicy,harqProcess_p->assignedResourcesNB,
                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
                /* SPR 13903 fix end */
        	    return MAC_FAILURE;
    		}
            }
        }
        else
        {
            if (ueCategoryChecksRequired)
            {
                /** klockwork Changes End **/
                checkCodeRateUECategoryLessThan3(
                        /* +- SPR 17777 */
                        harqProcess_p,
                        ueContext_p,
                        /* +- SPR 17777 */
                        internalCellIndex);
            }
        }
    } 
    return MAC_SUCCESS;
}

/*Dynamic PDCCH + 5117 changes ends*/

/*Rel 5.3.1 cyclomatic chg start*/ 
/******************************************************************************
 * Function Name  : setSubbandtoPMIInfoForResrcAlloc
 * 
 * Inputs :1)resrcAllocatorInput_p - This holds Pointer to 
 *           ResourceAllocatorInput structure.        
 *         2)harqProcess_p -  This holds pointer to DLHARQProcessstructure   
 *         3)dlUEContext_p -  This holds pointer to DLUEContext structure 
 *         4)tempSubbandIndex - temp index of SubbandIndex 
 *
 * Outputs : 1)resrcAllocatorOutput_p - This holds pointer to 
 *             ResourceAllocatorOutput structure 
 *
 * Returns : None
 *
 * Description : This function will set the value of PMIInfo from subband   
 *               for resource allocation.
 ******************************************************************************/
static void setSubbandtoPMIInfoForResrcAlloc(
                                             DLUEContext *dlUEContext_p,
                                             DLHARQProcess *harqProcess_p,
                                             ResourceAllocatorOutput *resrcAllocatorOutput_p,
                                             UInt8 tempSubbandIndex)
{

    if( (2 == dlUEContext_p->dlMIMOInfo.dlRi) && 
        (harqProcess_p->isTBOneValid && 
         TB_INVALID == harqProcess_p->isTBTwoValid))
    {
        if(0 == resrcAllocatorOutput_p->
           subbandtoPMIInfo[tempSubbandIndex])
        {
            resrcAllocatorOutput_p->
              subbandtoPMIInfo[tempSubbandIndex] = 0;
        }
        else if(1 == resrcAllocatorOutput_p->
                subbandtoPMIInfo[tempSubbandIndex])
        {
            resrcAllocatorOutput_p->
              subbandtoPMIInfo[tempSubbandIndex] = 2;
        }
    }/* 4x4 DL MIMO CHG START */
    else if( (2 == dlUEContext_p->dlMIMOInfo.dlRi) &&
             (harqProcess_p->isTBOneValid == TB_INVALID)&&
             (harqProcess_p->isTBTwoValid))
    {
        if(0 == resrcAllocatorOutput_p->
            subbandtoPMIInfo[tempSubbandIndex])
        {
            resrcAllocatorOutput_p->
               subbandtoPMIInfo[tempSubbandIndex] = 1;
        }
        else if(1 == resrcAllocatorOutput_p->
                subbandtoPMIInfo[tempSubbandIndex])
        {
            resrcAllocatorOutput_p->
                subbandtoPMIInfo[tempSubbandIndex] = 3;
        }
     /* 4x4 DL MIMO CHG END */   
    }    
return;

}                                             
/*Rel 5.3.1 cyclomatic chg end*/ 

/******************************************************************************
 * Function Name    : allocRBInRAT0ForSpatialMul
 * 
 * Inputs           : macAdvanceFactor,
 *                    resrcAllocatorInput_p - This holds Pointer to 
 *                                            ResourceAllocatorInput structure.        
 *                    harqProcess_p -  This holds pointer to DLHARQProcess structure,
 *                    dlUEContext_p -  This holds pointer to DLUEContext structure,
 *                    raPolicy_p - This holds pointer to ResourceAllocationPolicyInfo 
 *                                 structure,
 *                    dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                    scheduledTick - Scheduled TTI,
 *                    numOfOverLappingRb - Number of overlapping RB's
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : resrcAllocatorOutput_p - This holds pointer to 
 *                                             ResourceAllocatorOutput structure 
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will first get the available RB BitMap and then 
 *                    it will process that bitmap and populate the relevant 
 *                    information in ResourceAllocatorOutput structure. 
 ******************************************************************************/
#ifdef FDD_CONFIG
STATIC MacRetType allocRBInRAT0ForSpatialMul(
        /* + CQI_5.0 */
        UInt32 macAdvanceFactor,
        /* - CQI_5.0 */
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQProcess *harqProcess_p,
        DLUEContext *dlUEContext_p,
        ResourceAllocationPolicyInfo *raPolicy_p,
        DLStrategyTxNode *dlStrategyTxNode_p
        /*Dynamic PDCCH + 5117 changes starts*/
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends*/
        ,InternalCellIndex internalCellIndex
        )
#elif TDD_CONFIG
STATIC MacRetType allocRBInRAT0ForSpatialMul(
        /* + CQI_5.0 */
        UInt32 macAdvanceFactor,
        /* - CQI_5.0 */
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQProcess *harqProcess_p,
        DLUEContext *dlUEContext_p,
        ResourceAllocationPolicyInfo *raPolicy_p,
        DLStrategyTxNode *dlStrategyTxNode_p,
         /*Dynamic PDCCH + 5117 changes starts*/
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 *numOfOverLappingRb,
        /*Dynamic PDCCH + 5117 changes ends*/
        UInt8 subframe,
        InternalCellIndex internalCellIndex
        )
#endif
{
    UInt32 tempRBGBitMap = 0;
    UInt32 availableRBGBitMap = 0;
    UInt32 rbgSize = 0;
    UInt32 availableRBCount = 0;
    SInt32 requiredRBCount = 0;
    UInt32 rbgIndex = 0;
    UInt32 rbgPositionBitMap = 0;
    UInt32 resrcAssignementBitMap = 0;
    UInt32 rbgCount = 0;
    UInt32 rbsInRBG = 0;   
    UInt32 numFreePRB = 0;
    SInt8 allocatedRb = 0;
    DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
    DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL;
    RBGIndexInfo *rbgIndexInfo_p = PNULL;   
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;
    UInt32  rbsAllocated = 0;
    /* Cyclomatic Complexity Changes - starts here */
    /* + coverity 32771 */
    MacRetType  retVal = MAC_SUCCESS;
    /* - coverity 32771 */
    /* Cyclomatic Complexity Changes -ends  here */
    /*    PdschAllocationFailureType allocationFailureCause = 0;TDD Warning Fix */
#ifdef SSI_DEBUG_STATS
    UInt16 ueIndex = dlUEContext_p->ueIndex;
#endif    
    /* + CQI_4.1 */
    UInt8 usePrevRbCoding = FALSE;
    UInt8 lastRBGAvailable = FALSE;
    /* - CQI_4.1 */
    /* + TM7_8 Changes Start */
    UInt8 ix = 0;
    UInt32 temp = 0;
    UInt32 centralRbMask = 0;
    /* - TM7_8 Changes End */
    /* SPR 13666 fix start */
    UInt8 sizeOfLastRBG = 0;
    UInt8 originallyRequiredRBs = 0;
    /* SPR 13666 fix end */

    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;

#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe =checkForSpecialSubframe(subframe, internalCellIndex);
#endif
    /*spr 15832 +*/
    UInt8 widebandMCSIndexCW1 = INVALID_MCS_INDEX;
    UInt8 widebandMCSIndexCW2 = INVALID_MCS_INDEX;
    /*spr 15832 -*/
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
#endif
    /*HD FDD Changes End*/
    UInt8 widebandPMI = dlUEContext_p->dlCQIInfo.wideBandPMIIndex;
    UInt32 minTBSize = dlStrategyTxNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyTxNode_p->maxTBSize;
    /* + CQI_5.0 */
    UInt8 newSubBandNum = 255;  
    UInt8 tempSubbandIndex = 0;
    UInt8 prevSubBandNum = 255;  
    /* - CQI_5.0 */
    /* ICIC changes start */
    UInt32 resourceRegionAllocated =  
        resrcAllocatorInput_p->resourceRegionAllocated;
    /* ICIC changes end */

    UInt8 *centralSixRbsInfo_p = cellSpecificParams_g.\
           cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
           centralSixRbsInfo;
    /* +DYNAMIC_ICIC */
    /* This parameter represents calculated RNTP value of UE getting scheduled 
     * calculation is based on 36.213, section 5.2.1 
     */
    UInt8 calculatedRNTP = 0;
    /* -DYNAMIC_ICIC */

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);

    /*spr 15832 +*/
    if(IS_PCELL_OF_UE(dlUEContext_p,internalCellIndex))
    { 
        widebandMCSIndexCW1 = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordOne;
        widebandMCSIndexCW2 = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
    }   
    else
    {
        widebandMCSIndexCW1 = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordOne;
        widebandMCSIndexCW2 = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordTwo;
    }    
    /*spr 15832 -*/
    rat0Info_p = &raPolicy_p->rat0Info;
    rat1Info_p = &raPolicy_p->rat1Info;

    /* ICIC changes start */
    /* For CC_CE_REGION allocated, logical AND will give cell centre subset
     * bitmap. For CE_REGION, it will give cell edge subset bitmap. 
     */
    availableRBGBitMap = rat0Info_p->rat0BitMap[CC_USER] & 
        rat0Info_p->rat0BitMap[resourceRegionAllocated];
    /* ICIC changes end */
    /*Get the RBG Size*/
    rbgSize = rat0Info_p->rbgSize;
    /*Get the rbgCount */
    rbgCount = rat0Info_p->rbgCount;
    /* SPR 13666 fix start */
    sizeOfLastRBG= cellParams_p->dlAvailableRBs % rbgSize;
    originallyRequiredRBs = harqProcess_p->assignedResourcesNB;
    /* SPR 13666 fix end */
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* Fill the MCS Index CodeWord1 and codeword2 from the DL UE Context */
    HD_GET_REDUCED_MCS_CW1_CW2_DL_UL_SWITCH(currTick,dlUEContext_p,
            widebandMCSIndexCW1,widebandMCSIndexCW2);
#endif
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "rbgCount = %u rbgSize= %u \n",rbgCount,rbgSize);
    
    /* SPR 21631 Start */
    if(CHECK_SF_MAPPED_ON_BCH_PSS_SSS(scheduledTick,dlUEContext_p->dlMIMOInfo.transmissionMode)
        /* SPR 21631 End */
		    /*Coverity 108784 Fix Start*/
		    /*Coverity 108784 Fix End*/
      )
    {    
        for(ix = 0; ix < (MAX_NUM_OVERLAPPING_RB-1) ; ix++)
        {
            centralRbMask = centralRbMask | (1<<(centralSixRbsInfo_p[ix]/(rat0Info_p->rbgSize)));
        }

        /*block central 6 RBs in temp variable*/
        temp = availableRBGBitMap;
        availableRBGBitMap = temp &(~centralRbMask);
    }

    /* Calculate the number of Free RB for RAT0 */ 
    numFreePRB  = getCount(availableRBGBitMap) * rbgSize;
    if (availableRBGBitMap & ((UInt32) 1 << (rat0Info_p->rbgCount - 1)))
    {
        numFreePRB = numFreePRB - rbgSize +
            rat0Info_p->rbgIndexInfo[rat0Info_p->rbgCount - 1].numOfRBs;
        lastRBGAvailable = TRUE;
    }

    if ( !numFreePRB )  
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "numFreePRB = %u  \n Leaving funtion with Failure = %s\n",
                numFreePRB, __func__);

        LOG_MAC_MSG(MAC_NO_RB_LEFT_FOR_ALLOCATION,LOGDEBUG,
                MAC_PDSCH,getCurrentTick(),(UInt32)numFreePRB,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        /*allocationFailureCause = RB_NOT_AVAILABLE;TDD Warning Fix */

        /* CA Stats Changes Start */
        LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(RB_NOT_AVAILABLE,ueIndex,
                internalCellIndex);
        /* CA Stats Changes End */

            return MAC_FAILURE;
    }


    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo;

    availableRBCount = numFreePRB;
    requiredRBCount = resrcAllocatorInput_p->requiredRB;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "TB One Size  = %u  \n Tb Two Size =%u\n",dlHARQTBOneInfo_p->tbSize,            
            dlHARQTBTwoInfo_p->tbSize);

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,"required RB Count = %d \n \
            availableRBCount = %d",resrcAllocatorInput_p->requiredRB, availableRBCount);        

        /* + CQI_4.1 */
        if( (resrcAllocatorInput_p->subBandReportInfo_p) &&
            (!resrcAllocatorInput_p->aperiodicMode12) &&       
            ( ( (INVALID_TA_DRX_MSG == dlHARQTBOneInfo_p->taDrxFlag) &&
                (INVALID_TA_DRX_MSG == dlHARQTBTwoInfo_p->taDrxFlag)) ||
              ( (dlStrategyTxNode_p->msgType == SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL) ||
                (dlStrategyTxNode_p->msgType == SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL))))
        {
            /* Cyclomatic Complexity Changes -starts here*/
            retVal =  allocRBInRAT0SpatialForSubband (dlUEContext_p,
                    dlStrategyTxNode_p,
                    harqProcess_p,
                    resrcAllocatorInput_p,
                    raPolicy_p,
                    &tempRBGBitMap,
                    availableRBCount,
                    &allocatedRb,
                    /* + TM7_8 Changes Start */
                    centralRbMask,
                    dlUEContext_p->dlMIMOInfo.transmissionMode,
                    scheduledTick
                    /* - TM7_8 Changes End */
#ifdef TDD_CONFIG
                    /*coverity-530-32797*/
                    ,subframe
                    /*coverity-530-32797*/
#endif
                    ,internalCellIndex
                    );
            /* Coverity 73546 Fix Start*/
            if(0 == allocatedRb)
            {
                retVal = MAC_FAILURE;
            }
            /* Coverity 73546 Fix End*/

            if(MAC_FAILURE == retVal)
            {
                return retVal;
            }
            /* Cyclomatic Complexity Changes -ends here*/
        }
        else
        {    
            harqProcess_p->pmiIndex = widebandPMI;
            /* ICIC changes start */
            tempRBGBitMap = availableRBGBitMap;
            /* ICIC changes end */
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "TB One Size  = %u  \n Tb Two Size =%u\n",dlHARQTBOneInfo_p->tbSize,            
                    dlHARQTBTwoInfo_p->tbSize);

            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,"required RB Count = %d \n \
                    availableRBCount = %d",resrcAllocatorInput_p->requiredRB,
                    availableRBCount);        

            if ((SInt64)availableRBCount >= requiredRBCount )
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "availableRBCount >= requiredRBCount\n");
                allocatedRb = requiredRBCount;    
                /* Cyclomatic Complexity Changes - starts here */
                if ((RE_TX == dlHARQTBOneInfo_p->txIndicator ||
                            RE_TX == dlHARQTBTwoInfo_p->txIndicator))
                {
                    if(resrcAllocatorInput_p->requiredRB%rbgSize)
                    {
                        /* Cyclomatic Complexity Changes - ends here */
                        if ( lastRBGAvailable )
                        {
                            usePrevRbCoding = TRUE;
                        }
                        else
                        {    
                            /*allocationFailureCause = RETX_RB_NOT_AVAILABLE;TDD Warning Fix */
                            /* CA Stats Changes Start */
                            LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(
                                    RETX_RB_NOT_AVAILABLE,ueIndex, 
                                    internalCellIndex);
                            /* CA Stats Changes End */
                                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                                        "Leaving funtion = %s with Failure as \
                                        case of ReTx and availableRBCount < \
                                        requiredRBCount \n",
                                        __func__);

                            /* ICIC changes start */
                            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                    (resrcAllocatorInput_p->requiredRB),
                                    internalCellIndex);
                            /* ICIC changes end */
                            return MAC_FAILURE;
                        }
                    }
                }
                else
                {       
                    /*This is new transmission and we can allocate more than minTBSize based on excessPool*/
                    /* HD FDD Change Start */
                    harqProcess_p->dlHARQTBOneInfo.mcsIndex = widebandMCSIndexCW1;
                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex = widebandMCSIndexCW2;
                    /* HD FDD Change End */
                    /* ICIC changes start */
                    if ((excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0) && (maxTBSize > minTBSize))
                    {      
                        /* try to reach maxTBSize in case both TBs or single TB is valid*/
                        allocatedRb = allocateRBForMaxTBSize(harqProcess_p, 
                                allocatedRb, 
                                availableRBCount, 
                                maxTBSize,
                                resourceRegionAllocated,
                                internalCellIndex);                       

                        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                (requiredRBCount - allocatedRb),
                                internalCellIndex);
                        /* ICIC changes end */
                    }

                    harqProcess_p->assignedResourcesNB = allocatedRb;
                    /* Cyclomatic Complexity Changes -starts here */
                    /* Cyclomatic_complexity_changes_start */
                    updatePMIIndexInHarqContext(dlUEContext_p,
                            harqProcess_p,
                            macAdvanceFactor,
                            widebandPMI,
                            internalCellIndex,
                            allocatedRb
                            #ifdef TDD_CONFIG
                            ,isSpecialSubframe
                            #endif
                            );
                    /* Cyclomatic_complexity_changes_end */
                    /* Cyclomatic Complexity Changes -ends here */

                    /*In case of New Transmission set the Transport block size in HARQ TB considering the fact
                      that this could be special subframe in case of TDD*/
                }
            }    
            else if (RE_TX == dlHARQTBOneInfo_p->txIndicator ||
                    RE_TX == dlHARQTBTwoInfo_p->txIndicator)
            {
                /* +SPR 5227 Changes Start */
                if (( resrcAllocatorInput_p->requiredRB%rbgSize) )
                {
                    /* Cyclomatic Complexity Changes -starts here */
                    if (!lastRBGAvailable )
                    {    
                        /* CA Stats Changes Start */
                        LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(
                                RETX_RB_NOT_AVAILABLE,ueIndex, 
                                internalCellIndex);
                        /* CA Stats Changes End */
                            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                                    "Leaving funtion = %s with Failure as \
                                    case of ReTx and availableRBCount < \
                                    requiredRBCount \n",
                                    __func__);

                        /* ICIC changes start */
                        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                (resrcAllocatorInput_p->requiredRB),
                                internalCellIndex);
                        /* ICIC changes end */
                        return MAC_FAILURE;
                    }
                    usePrevRbCoding = TRUE;
                    /* Cyclomatic Complexity Changes -Ends here */
                }
                /* -SPR 5227 Changes end */

                /* ICIC changes start */
                /* In case of cell edge retransmission, if availableRBCount count
                 * is not sufficient, we check availableRBCount count from 
                 * cell centre region
                 */
                if (CE_REGION == resourceRegionAllocated)
                {
                    tempRBGBitMap = rat0Info_p->rat0BitMap[CC_USER];
                    /* + TM7_8 Changes Start */
                    /* SPR 21631 Start */
                        if(CHECK_SF_MAPPED_ON_BCH_PSS_SSS(scheduledTick,
                                    dlUEContext_p->dlMIMOInfo.transmissionMode))
                            /* SPR 21631 End */
                    {
                        tempRBGBitMap &= (~centralRbMask);
                    }
                    /* - TM7_8 Changes End */
                    numFreePRB  = getCount(tempRBGBitMap) * rbgSize;
                    if (tempRBGBitMap & ((UInt32) 1 << (rat0Info_p->rbgCount - 1)))
                    {
                        numFreePRB = numFreePRB - rbgSize +
                            rat0Info_p->rbgIndexInfo[rat0Info_p->rbgCount - 1].numOfRBs;
                    }
                }
                /* Cyclomatic Complexity Changes -starts here */
                if ((SInt64)numFreePRB < requiredRBCount)
                    /* Cyclomatic Complexity Changes -ends here */
                {
                    LOG_MAC_MSG(MAC_RB_NOT_AVAILABLE_FOR_RETX,LOGDEBUG,
                            MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                            (UInt32)availableRBCount,(UInt32)requiredRBCount,
                            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"\nRB required more than RB available for retx");

                    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                            "Leaving funtion = %s with Failure as \
                            case of ReTx and availableRBCount < \
                            requiredRBCount \n",
                            __func__);

                    /*        allocationFailureCause = RETX_RB_NOT_AVAILABLE; TDD Warning Fix */
                    /* CA Stats Changes Start */
                    LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(
                            RETX_RB_NOT_AVAILABLE, ueIndex, internalCellIndex);
                    /* CA Stats Changes End */
                        /* ICIC changes start */
                        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                (resrcAllocatorInput_p->requiredRB),
                                internalCellIndex);
                    /* ICIC changes end */

                    lteWarning("[%s] 2. RBs not available, reqd[%d] av[%d] \n", __func__, requiredRBCount,availableRBCount);
                    return MAC_FAILURE;
                }
                /* Cyclomatic Complexity Changes -starts here */
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "availableRBCount >= requiredRBCount\n");
                allocatedRb = requiredRBCount;
                /* Cyclomatic Complexity Changes -ends here */
                /* ICIC changes end */
            }    
            else 
            {
                if (dlStrategyTxNode_p->strictAllocatedFlag)
                {
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            resrcAllocatorInput_p->requiredRB,
                            internalCellIndex);
                    /* ICIC changes end */
                    lteWarning("[%s]3/ RBs not available, reqd[%d] av[%d]\n", __func__, requiredRBCount,availableRBCount);
                    return MAC_FAILURE;
                }
                allocatedRb = availableRBCount;

                //update excessPool but it seems that it is the last UE as available resources are less than required.
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                        (resrcAllocatorInput_p->requiredRB - allocatedRb),
                        internalCellIndex);
                /* ICIC changes end */

                tempRBGBitMap = availableRBGBitMap;

                harqProcess_p->dlHARQTBOneInfo.mcsIndex = widebandMCSIndexCW1;
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex = widebandMCSIndexCW2;
                harqProcess_p->pmiIndex = widebandPMI;
                harqProcess_p->assignedResourcesNB = allocatedRb;

                /* Cyclomatic Complexity Changes -starts here */
                /* Cyclomatic_complexity_changes_start */
                updatePMIIndexInHarqContext(dlUEContext_p,
                        harqProcess_p,
                        macAdvanceFactor,
                        widebandPMI,
                        internalCellIndex,
                        allocatedRb
                        #ifdef TDD_CONFIG
                        ,isSpecialSubframe
                       #endif
                       ) ;
                 /* Cyclomatic_complexity_changes_end */
                /* Cyclomatic Complexity Changes -starts here */

                /*In case of New Transmission set the Transport block size in HARQ TB considering the fact
                  that this could be special subframe in case of TDD*/
            }  
        }  
    /* - CQI_4.1 */

    /* Below For loop is finding the position of first free PRB and then it is 
     * updating  variable availableRBGBitMap such that bit corrosponding to 
     * postion of free PRB is unset.It is also updating the 
     * resrcAssignementBitMap  variable such that bit corrosponding to  postion
     * of free PRB is set.It is also relevant functions which will update the
     * RAT 1 ,RAT 2 Locaized anf RAT 2 Distributed Information */
    /* SPR# 3233 Changes Starts */
    harqProcess_p->assignedResourcesNB = allocatedRb;
    /* SPR# 3233 Changes Ends */

    /* +DYNAMIC_ICIC */
    if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
    {
        calculatedRNTP = calculateRNTP(dlUEContext_p->pA, harqProcess_p, internalCellIndex);
    }
    /* -DYNAMIC_ICIC */
    /* + TM7_8 Changes Start */
    if(!((dlUEContext_p->dlMIMOInfo.transmissionMode == TX_MODE_7)
                ||(dlUEContext_p->dlMIMOInfo.transmissionMode == TX_MODE_8)))
    {
        /* - TM7_8 Changes End */
        /*Dynamic PDCCH + 5117 changes starts */
        /* Need to validate MCS/RB pair for non 0/5
         * Subframes for mCS 28.
         * In case of non0/5 SF and MCS28, we need to 
         * check for coderate and ueCat both while in
         * non 0/5 SF, for other MCS values, 
         * we just need to check uecategory*/
    /* SPR 8624 FIX START  */
    /* SPR 12997 FIX Start */
	if ( (NEW_TX == dlHARQTBOneInfo_p->txIndicator) &&
          	    (NEW_TX == dlHARQTBTwoInfo_p->txIndicator))
    /* SPR 12997 FIX end */
	{
	        /* SPR 13666 fix start */
	        if (FALSE == lastUEDetectFlag_g[internalCellIndex] )
                {
            		/* SPR 9625 Fix Start */
	            /* hardcode value 4 is replaced by rbgSize as different bandwidth have different rbgSize */
	           if((harqProcess_p->assignedResourcesNB)%rbgSize)
        	   {
              		if (( availableRBCount - harqProcess_p->assignedResourcesNB) >=
		                  (rbgSize -((harqProcess_p->assignedResourcesNB)%rbgSize)))
                    {
                        /* SPR 14163 Changes Starts  */
                        /* To check that increased TB SIZE should not exceed the max TB Size as per UE Category */ 
                        UInt8  increasedRb = harqProcess_p->assignedResourcesNB +
                            ( rbgSize-((harqProcess_p->assignedResourcesNB)%rbgSize));  

                        UInt8  decreasedRb = harqProcess_p->assignedResourcesNB -
                            ((harqProcess_p->assignedResourcesNB)%rbgSize);  

                        UInt32 incTb1Size = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                increasedRb,
                                /*4X4 DL MIMO CHG START*/
                                TB_MAPPED_TO_ONE_LAYER
                                /*4X4 DL MIMO CHG END*/);
                        UInt32 incTb2Size = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                increasedRb,
                                /*4X4 DL MIMO CHG START*/
                                TB_MAPPED_TO_ONE_LAYER
                                /*4X4 DL MIMO CHG END*/);
                        if((incTb1Size+incTb2Size) > dlUEContext_p->maxTBSize)
                        {
                            harqProcess_p->assignedResourcesNB = decreasedRb; 	
                        }
                        else
                        {
                            harqProcess_p->assignedResourcesNB = increasedRb; 	
                        }
                        /* SPR 9625 Fix End*/
                        /* SPR 14163 Changes Ends  */
#ifdef FDD_CONFIG
		            SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
                                 harqProcess_p->assignedResourcesNB);
#elif TDD_CONFIG
	                    SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
                                  harqProcess_p->assignedResourcesNB,
                                  isSpecialSubframe);
#endif
		         }
             		 else if ((TRUE == lastRBGAvailable)
                                  && (sizeOfLastRBG != rbgSize))
             		 {
                		if ((availableRBCount - harqProcess_p->assignedResourcesNB) >=
		                   (sizeOfLastRBG -((harqProcess_p->assignedResourcesNB)%rbgSize)))
                		{
		                    harqProcess_p->assignedResourcesNB +=
                	              (sizeOfLastRBG -((harqProcess_p->assignedResourcesNB)%rbgSize));
#ifdef FDD_CONFIG
                    		    SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
                                         harqProcess_p->assignedResourcesNB);
#elif TDD_CONFIG
   		                    SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
                                        harqProcess_p->assignedResourcesNB,
		                        isSpecialSubframe);
#endif
             			}
	     		 }
	   	     }
		}
      		 /* SPR 13666 fix end */
	}
    /* SPR 8624 FIX end  */

        if (MAC_FAILURE == checkCodeRateUECategory(
                    RA_POLICY_0, 
                    scheduledTick,
                    harqProcess_p,
                    dlUEContext_p,
                    TRUE,
                    availableRBGBitMap,
                    /* +- SPR 17777 */
                    internalCellIndex))
        {
            /* SPR 13903 fix start */
            LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                    MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                    RA_POLICY_0,harqProcess_p->assignedResourcesNB,
                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            /* SPR 13903 fix end */

            return MAC_FAILURE;
        }
        /* + TM7_8 Changes Start */
    } 
    else if(((dlUEContext_p->dlMIMOInfo.transmissionMode == TX_MODE_7)&&(harqProcess_p->dciFormat == DCI_FORMAT_1))
            ||((dlUEContext_p->dlMIMOInfo.transmissionMode == TX_MODE_8)&& (harqProcess_p->dciFormat == DCI_FORMAT_2B)))
    {
        if(MAC_FAILURE == checkCodeRateUECategoryForTransMode_7_8(
                    dlUEContext_p,harqProcess_p,internalCellIndex))
        {
            /* SPR 13903 fix start */
            LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                    MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                    RA_POLICY_0,harqProcess_p->assignedResourcesNB,
                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            /* SPR 13903 fix end */

            return MAC_FAILURE;
        }
    }
    /* - TM7_8 Changes End */
    /*Dynamic PDCCH + 5117 changes ends */	

    requiredRBCount = harqProcess_p->assignedResourcesNB; 
    resrcAllocatorOutput_p->allocatedRB = requiredRBCount;
    rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[0];      
    availableRBGBitMap = tempRBGBitMap;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "requiredRBCount = %u\n",requiredRBCount);
    while ( requiredRBCount > 0  )
    {
        /* + CQI_4.1 */
        if (usePrevRbCoding == TRUE)
        {
            rbgIndex = rat0Info_p->rbgCount;
            usePrevRbCoding = FALSE;
        }
        else 
        {
            rbgIndex = FFS(tempRBGBitMap);
        }
        if (rbgIndex ) 
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "rbgIndex = %u\n",rbgIndex);
            rbgIndex--;
            rbsInRBG = rbgIndexInfo_p[rbgIndex].numOfRBs;
            requiredRBCount -= rbsInRBG;
            /*SPR 4086 Changes Start*/
            /*SPR 4906 Fix Start*/
            /* SPR# 13585 fix start */
            if(((requiredRBCount < 0) && (harqProcess_p->assignedResourcesNB > rbsInRBG)))
            /* SPR# 13585 fix end */
            {
                /* SPR 6940 Changes starts */
                UInt8 rbsPermitted = harqProcess_p->assignedResourcesNB + 
                    (harqProcess_p->assignedResourcesNB % rbsInRBG);

                UInt32 tb1Size = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                        harqProcess_p->assignedResourcesNB,
                        /*4X4 DL MIMO CHG START*/
                        TB_MAPPED_TO_ONE_LAYER
                        /*4X4 DL MIMO CHG END*/);
                UInt32 tb2Size = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                        harqProcess_p->assignedResourcesNB,
                        /*4X4 DL MIMO CHG START*/
                        TB_MAPPED_TO_ONE_LAYER
                        /*4X4 DL MIMO CHG END*/);

                UInt32 newTb1Size = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                        rbsPermitted,
                        /*4X4 DL MIMO CHG START*/
                        TB_MAPPED_TO_ONE_LAYER
                        /*4X4 DL MIMO CHG END*/);
                UInt32 newTb2Size = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                        rbsPermitted,
                        /*4X4 DL MIMO CHG START*/
                        TB_MAPPED_TO_ONE_LAYER
                        /*4X4 DL MIMO CHG END*/);
                if((tb1Size+tb2Size) < (newTb1Size+newTb2Size ) )
                {
            	    /* SPR 13196 fix start */
                    break;
            	    /* SPR 13196 fix end */
                }
                /* SPR 6940 Changes ends */
            }
            /*SPR 4906 Changes Ends*/
            /*SPR 4086 Fix End*/
            /* SPR 13196 fix start */
            rbsAllocated += rbsInRBG; 
	    /* code removed */
            /* SPR 13196 fix end */
            rbgPositionBitMap = (UInt32)1 << rbgIndex;
            tempRBGBitMap &=  (~rbgPositionBitMap);
            resrcAssignementBitMap |= ((UInt32)1 << (rbgCount - rbgIndex - 1)); 
            if ( raPolicy_p->rat2LocalizedInfo.maxLocalizedVRB  > 10 )
            {
                updateRAT1InfoFromRAT0Info(rat1Info_p, 
                        &rbgIndexInfo_p[rbgIndex]);
            }
            /* +DYNAMIC_ICIC */
            updateRAT2InfoFromRAT0Info(rbgIndex, rbgSize, 
                    rbsInRBG, raPolicy_p,calculatedRNTP,internalCellIndex);
            /* -DYNAMIC_ICIC */

            /* + CQI_5.0 */
            if( (TRUE == dlUEContext_p->actualPMIValueForDLSCH) &&        
                    (TRUE == resrcAllocatorInput_p->aperiodicMode12) )
            {
                SubBandToCQInfo *subBandToCQInfo_p = 
                    &resrcAllocatorInput_p->subBandReportInfo_p->subBandToCQInfo[0];  
                /* SPR 18122 Changes Start*/
                newSubBandNum = DIVIDE_BY_TWO(rbgIndex); 
                /* SPR 18122 Changes End*/
                SubBandToCQInfo *tempSubBandToCQInfo_p = &subBandToCQInfo_p[newSubBandNum];
                if(newSubBandNum != prevSubBandNum)
                {    
                    resrcAllocatorOutput_p->subbandtoPMIInfo[tempSubbandIndex] = 
                                         tempSubBandToCQInfo_p->preCodingMatrixIndex;
                    prevSubBandNum = newSubBandNum;
                    /* Cyclomatic Complexity Changes -starts here */
                    if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
                    {
                        /* The logic is according to the spec 36.212 V 9.4.0
                         * Tabel 5.3.3.1.5-4 for 2 antenna that first column (5) 
                         * is used if 1st TB transmission otherwise 2nd column (6)
                         * is used for 4 Tx antenna same PMI value which is recevied 
                         * from the UE is used  in function updateRAT0SubBandPMIInfo()*/
                        /*Rel 5.3.1 cyclomatic chg start*/
                        setSubbandtoPMIInfoForResrcAlloc(dlUEContext_p,harqProcess_p,
                                                      resrcAllocatorOutput_p,tempSubbandIndex);
                        /*Rel 5.3.1 cyclomatic chg end*/
                        tempSubbandIndex++;
                    }
                }
                /* Cyclomatic Complexity Changes -ends here */
            }
            /* + CQI_5.0 */
            /* SPR 13196 fix start */
	    /* code  removed */
            /* SPR 13196 fix end */
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,
                    "Error has occured in .FFS should not return 0 %s\n",
                    __func__);
            break;
        }
        /* - CQI_4.1 */
    }

    /* SPR 14890 fix start */
    originallyRequiredRBs = harqProcess_p->assignedResourcesNB;
    /* SPR 14890 fix end */
    /* SPR 13666 fix start */
    if (rbsAllocated > originallyRequiredRBs)
    {
         UPDATE_EXCESS_POOL_RB(resourceRegionAllocated,
                -(rbsAllocated - originallyRequiredRBs),
                internalCellIndex);

        if ( NEW_TX == dlHARQTBOneInfo_p->txIndicator && NEW_TX == dlHARQTBTwoInfo_p->txIndicator)
        {
            harqProcess_p->assignedResourcesNB = rbsAllocated;
#ifdef FDD_CONFIG
            SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
                    rbsAllocated);
#elif TDD_CONFIG
            SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
                    rbsAllocated,
                    isSpecialSubframe);
#endif
        }
        else if ((RE_TX == dlHARQTBOneInfo_p->txIndicator) ||
                 (RE_TX == dlHARQTBTwoInfo_p->txIndicator))
        {
           lteWarning("[%s][%d] [%d] Error in Resource Allocation rbAllocated[%d] requiredRB[%d] \n",
		 __func__, __LINE__, getCurrentTick(), rbsAllocated,harqProcess_p->assignedResourcesNB);
        }
    }
    else if (

            /* Coverity_73546 Fix Start */ 
            (rbsAllocated != 0) &&
            /* Coverity_73546 Fix End */
            (rbsAllocated < originallyRequiredRBs)
            )
    {
        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated,
               (originallyRequiredRBs-rbsAllocated),internalCellIndex);
        if ( NEW_TX == dlHARQTBOneInfo_p->txIndicator && NEW_TX == dlHARQTBTwoInfo_p->txIndicator)
        {
              harqProcess_p->assignedResourcesNB = rbsAllocated;
#ifdef FDD_CONFIG
          SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
                    rbsAllocated);
#elif TDD_CONFIG
          SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
                    rbsAllocated,
                    isSpecialSubframe);
#endif
        }
        else if ((RE_TX == dlHARQTBOneInfo_p->txIndicator) ||
                 (RE_TX == dlHARQTBTwoInfo_p->txIndicator))
        {
           lteWarning("[%s][%d] [%d] Error in Resource Allocation rbAllocated[%d] requiredRB[%d] \n",
		 __func__, __LINE__, getCurrentTick(), rbsAllocated,harqProcess_p->assignedResourcesNB);
        }
    }
/* SPR 13666 fix end */
    /* SPR 9981 FIX  START */
    if (rbsAllocated)
    {
    /*Cyclomatic Complexity Changes -starts here*/
       /* SPR 20524 fix start */
        if ( NEW_TX == dlHARQTBOneInfo_p->txIndicator || NEW_TX == dlHARQTBTwoInfo_p->txIndicator)
        {
    updateRAT0SubBandPMIInfo( dlUEContext_p,
            harqProcess_p,
            resrcAllocatorInput_p,
            resrcAllocatorOutput_p,
            availableRBGBitMap,
            tempRBGBitMap,
            rbsAllocated
#ifdef TDD_CONFIG
            /*coverity-530-32797*/
            ,  subframe
            /*coverity-530-32797*/
#endif
            ,internalCellIndex
            );
    /*Cyclomatic Complexity Changes -ends here*/
        }
       /* SPR 20524 fix end */
    /*Updating the resrcAllocatorOutput_p structure with relevant information*/
    resrcAllocatorOutput_p->allocatedRB = rbsAllocated;                   
    resrcAllocatorOutput_p->resourceAssignmentBitMap = resrcAssignementBitMap;
    resrcAllocatorOutput_p->ratPolicy = RA_POLICY_0;
    resrcAllocatorOutput_p->numberOfSubbands = tempSubbandIndex;
    /* ICIC changes start */
    /* Update RAT0 bitmap for cell centre user, bitmap for cell edge user is 
     * derived from cell edge bitmap */
    rat0Info_p->rat0BitMap[CC_USER] = availableRBGBitMap ^  
        rat0Info_p->rat0BitMap[CC_USER] ^ tempRBGBitMap;
    /* ICIC changes end */
    /*Dynamic PDCCH + 5117 changes starts */
    /* SPR 21631 Start */
     /* SPR 23393 Changes Start */
    if(!(scheduledTick % 5))
     /* SPR 23393 Changes End */
        /* SPR 21631 End */
    {
        *numOfOverLappingRb = calculateNumOfOverLapRbs(resrcAssignementBitMap,rbsAllocated,internalCellIndex);
    }
    /* SPR_7516_FIX */
#ifdef TDD_CONFIG
     /* SPR 23393 Changes Start */
    if(getSubframeType(scheduledTick%MAX_SUB_FRAME, internalCellIndex)== SP)
     /* SPR 23393 Changes End*/
    {
        *numOfOverLappingRb = calculateNumOfOverLapRbs(resrcAssignementBitMap,rbsAllocated,internalCellIndex);
    }
#endif
    /* SPR_7516_FIX */

    /*Dynamic PDCCH + 5117 changes ends */
    }
    /* SPR 9981 FIX  END */

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__); 
    return MAC_SUCCESS;
}



/******************************************************************************
 * Function Name    : allocRBInRAT1ForSpatialMul
 * Inputs           : macAdvanceFactor,
 *                    resrcAllocatorInput_p - This holds Pointer to 
 *                                            ResourceAllocatorInput structure.        
 *                    harqProcess_p -  This holds pointer to DLHARQProcess structure,
 *                    dlUEContext_p -  This holds pointer to DLUEContext structure,
 *                    raPolicy_p - This holds pointer to ResourceAllocationPolicyInfo 
 *                                 structure,
 *                    dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                    scheduledTick - Scheduled TTI,
 *                    numOfOverLappingRb - Number of overlapping RB's
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : resrcAllocatorOutput_p - This holds pointer to 
 *                                             ResourceAllocatorOutput structure 
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will call relevant funtions for getting available
 *                    RB BitMap and then it will process that bitmap and populate 
 *                    the relevant information in ResourceAllocatorOutput structure. 
 *******************************************************************************/
#ifdef FDD_CONFIG
STATIC MacRetType allocRBInRAT1ForSpatialMul(
        /* + CQI_5.0 */
        UInt32 macAdvanceFactor,
        /* - CQI_5.0 */
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQProcess *harqProcess_p,
        DLUEContext *dlUEContext_p,
        ResourceAllocationPolicyInfo *raPolicy_p,
        DLStrategyTxNode *dlStrategyTxNode_p
        /*Dynamic PDCCH + 5117 changes starts*/
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends*/
        ,InternalCellIndex internalCellIndex
        )
#elif TDD_CONFIG
STATIC MacRetType allocRBInRAT1ForSpatialMul(
        /* + CQI_5.0 */
        UInt32 macAdvanceFactor,
        /* - CQI_5.0 */
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQProcess *harqProcess_p,
        DLUEContext *dlUEContext_p,
        ResourceAllocationPolicyInfo *raPolicy_p,
        DLStrategyTxNode *dlStrategyTxNode_p
         /*Dynamic PDCCH + 5117 changes starts*/
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends*/
        ,UInt8 subframe
        ,InternalCellIndex internalCellIndex)
#endif
{
    UInt32 tempRBBitMap = 0;    
    UInt32 subsetIndexAllocated = 0;
    UInt32 isNonZeroSpanConsideredForAlloc = 0;
    UInt32 requiredRBCount = 0;
    UInt32 bitMapForSetBit = 0;
    UInt32 availableRBBitMap = 0;
    SubsetInfo *subsetInfo_p = PNULL;
    UInt32 resrcAssignementBitMap= 0;
    RAT0Info *rat0Info_p = PNULL;
    UInt32 maxNumAddressableRB = 0;
    RAT1Info *rat1Info_p = PNULL;
    UInt32 prbNumber = 0;
    UInt32 rat0BitMap = 0;
    UInt32 availableRBCount = 0;
    UInt32 rbPosition= 0;
    UInt8 allocatedRb = 0;
    DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
    DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL;
#ifdef SSI_DEBUG_STATS
    UInt16 ueIndex = dlUEContext_p->ueIndex;
#endif
    /* ICIC changes start */
    UInt32 resourceRegionAllocated = 
        resrcAllocatorInput_p->resourceRegionAllocated;
    /* ICIC changes end */
    /* + CQI_4.1 */
    /* + coverity 32772 */
    MacRetType  retVal = MAC_FAILURE ;  
    /* - coverity 32772 */

    /* + TM7_8 Changes Start */
    UInt8 ix = 0;
    UInt32 centralRbMask[MAX_SUBSETS] = {0} ;
    /* - TM7_8 Changes End */

    /* +DYNAMIC_ICIC */
    /* This parameter represents calculated RNTP value of UE getting scheduled 
     * calculation is based on 36.213, section 5.2.1 
     */
    UInt8 calculatedRNTP = 0;
    /* -DYNAMIC_ICIC */
#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe =checkForSpecialSubframe(subframe, internalCellIndex);
#endif
    /*spr 15832 +*/
    UInt8 widebandMCSIndexCW1 = INVALID_MCS_INDEX;
    UInt8 widebandMCSIndexCW2 = INVALID_MCS_INDEX;
    /*spr 15832 -*/
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
#endif
    /*HD FDD Changes End*/
    UInt8 widebandPMI = dlUEContext_p->dlCQIInfo.wideBandPMIIndex;    
    UInt32 minTBSize = dlStrategyTxNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyTxNode_p->maxTBSize;
    /* + CQI_5.0 */
    UInt8 newSubBandNum = 255;  
    UInt8 tempSubbandIndex = 0;
    UInt8 prevSubBandNum = 255;  
    /* - CQI_5.0 */

    UInt8 *centralSixRbsInfo_p = cellSpecificParams_g.\
           cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
           centralSixRbsInfo;
    /* - CQI_4.1 */
    
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    rat0Info_p = &raPolicy_p->rat0Info;
    rat1Info_p = &raPolicy_p->rat1Info;
    requiredRBCount = resrcAllocatorInput_p->requiredRB;
    /*spr 15832 +*/
    if(IS_PCELL_OF_UE(dlUEContext_p,internalCellIndex))
    { 
        widebandMCSIndexCW1 = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordOne;
        widebandMCSIndexCW2 = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
    }   
    else
    {
        widebandMCSIndexCW1 = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordOne;
        widebandMCSIndexCW2 = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordTwo;
    }    
    /*spr 15832 -*/

    /* + TM7_8 Changes Start */
    /*get mask for central RBs*/

    /* SPR 21631 Start */
    if(CHECK_SF_MAPPED_ON_BCH_PSS_SSS(scheduledTick,dlUEContext_p->dlMIMOInfo.transmissionMode))
        /* SPR 21631 End */
    {
        for(ix = 0; ix < (MAX_NUM_OVERLAPPING_RB-1) ; ix++)
        {
            UInt8 tempSubSetIndex = 0;
            UInt32 rbPosInSubsetBitmap = 0;
            tempSubSetIndex = (((centralSixRbsInfo_p[ix]/rat0Info_p->rbgSize)) % rat0Info_p->rbgSize);
            rbPosInSubsetBitmap = ((((centralSixRbsInfo_p[ix]/rat0Info_p->rbgSize)/rat0Info_p->rbgSize) *
                        rat0Info_p->rbgSize)
                    + (centralSixRbsInfo_p[ix]%rat0Info_p->rbgSize));
            centralRbMask[tempSubSetIndex] = centralRbMask[tempSubSetIndex] | (1 << rbPosInSubsetBitmap);
        }
    }
    /* - TM7_8 Changes End */


    /* ICIC changes start */
    /*Now we try to allocate RBs according to wideBands*/
    availableRBCount = getMaxPrbSubsetIndexForSpatialMul(rat1Info_p,
            &subsetIndexAllocated,
            &isNonZeroSpanConsideredForAlloc,
            &tempRBBitMap,
            /* + coverity 36197 */
            (ResourceRegion) resourceRegionAllocated,
            /* - coverity 36197 */
            /* + TM7_8 Changes Start */
            centralRbMask,
            scheduledTick,
            dlUEContext_p->dlMIMOInfo.transmissionMode);
    /* - TM7_8 Changes End */
    /* ICIC changes end */

    /* + CQI_4.1 */
    subsetInfo_p = &rat1Info_p->subsetInfo[subsetIndexAllocated];
    if ( !availableRBCount )  
    {
        LOG_MAC_MSG(MAC_NO_RB_LEFT_FOR_ALLOCATION,LOGDEBUG,
                MAC_PDSCH,getCurrentTick(),(UInt32)availableRBCount,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,  
                "Leaving funtion = %s with Failure as \
                Available Rb Count is  0\n",
                __func__);

        /* allocationFailureCause = RB_NOT_AVAILABLE; TDD Warning Fix*/
        /* CA Stats Changes Start */
        LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(RB_NOT_AVAILABLE,
                ueIndex, internalCellIndex);
        /* CA Stats Changes End */
            lteWarning("[%s] Available Rb Count is  0\n",__func__);
        return MAC_FAILURE;
    }
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* Fill the MCS Index CodeWord1 and codeword2 from the DL UE Context */
    HD_GET_REDUCED_MCS_CW1_CW2_DL_UL_SWITCH(currTick,dlUEContext_p,
            widebandMCSIndexCW1,widebandMCSIndexCW2);
#endif
    /*HD FDD Changes End*/

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "availableRBCount = %u ,requiredRBCount = %u\n",
            availableRBCount, requiredRBCount); 

    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo;

    /*Check if SubBand report is available or not */
    if( (resrcAllocatorInput_p->subBandReportInfo_p) &&
        (!resrcAllocatorInput_p->aperiodicMode12) &&       
        ( ( (INVALID_TA_DRX_MSG == dlHARQTBOneInfo_p->taDrxFlag) &&
            (INVALID_TA_DRX_MSG == dlHARQTBTwoInfo_p->taDrxFlag)) ||
          ( (dlStrategyTxNode_p->msgType == SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL) ||
            (dlStrategyTxNode_p->msgType == SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL))))
    {
        /* Cyclomatic Complexity Changes - starts here */
        retVal =  allocRBInRAT1SpatialForSubband(harqProcess_p,
                resrcAllocatorInput_p,
                rat1Info_p,
                dlStrategyTxNode_p,
                availableRBCount,
                &subsetIndexAllocated,
                &isNonZeroSpanConsideredForAlloc,
                &allocatedRb,
                /*Coverity_31964 Fix Start*/
                &subsetInfo_p,
                /*Coverity_31964 Fix End*/
                &tempRBBitMap,
                /* + TM7_8 Changes Start */
                centralRbMask,
                dlUEContext_p->dlMIMOInfo.transmissionMode,
                scheduledTick
                /* - TM7_8 Changes End */
#ifdef TDD_CONFIG
                /*coverity-530-32798*/
                ,subframe 
                /*coverity-530-32798*/
#endif
                ,internalCellIndex

                );
        /* Coverity 73464 Fix Start*/
        if(0 == allocatedRb)
        {
            retVal = MAC_FAILURE;
        }
        /* Coverity 73464 Fix End*/

        if(MAC_FAILURE == retVal)
        {
            return retVal;
        }
        /* Cyclomatic Complexity Changes - ends here */
    }
    else
    {    
        harqProcess_p->pmiIndex = widebandPMI;
        if (availableRBCount >= requiredRBCount )
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "availableRBCount >= requiredRBCount\n");
            allocatedRb = requiredRBCount;
            if (NEW_TX == dlHARQTBOneInfo_p->txIndicator ||
                    NEW_TX == dlHARQTBTwoInfo_p->txIndicator)
            {
                /*This is new transmission and we can allocate more than minTBSize based on excessPool*/
                /* HD FDD Changes Start */
                /* SPR 6136 start */
                //dlHARQTBOneInfo_p->mcsIndex = widebandMCSIndexCW1;
                //dlHARQTBTwoInfo_p->mcsIndex = widebandMCSIndexCW2;
                if(NEW_TX == dlHARQTBOneInfo_p->txIndicator)
                {
                    dlHARQTBOneInfo_p->mcsIndex = widebandMCSIndexCW1;
                }
                /* Cyclomatic_complexity_changes_start */
                if(NEW_TX ==  dlHARQTBTwoInfo_p->txIndicator)
                {
                    dlHARQTBTwoInfo_p->mcsIndex = widebandMCSIndexCW2;
                }	
                /* Cyclomatic_complexity_changes_end */
                /* HD FDD Changes End */
                /* SPR 6136 end */

                /* ICIC changes start */
                if ((excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0) && (maxTBSize > minTBSize))
                {      
                    /* try to reach maxTBSize in case both TBs or single TB is valid*/
                    allocatedRb = allocateRBForMaxTBSize(harqProcess_p, 
                            allocatedRb, 
                            availableRBCount, 
                            maxTBSize,
                            resourceRegionAllocated,
                            internalCellIndex);                      

                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            (requiredRBCount - allocatedRb),
                            internalCellIndex);
                    /* ICIC changes end */
                }
                harqProcess_p->assignedResourcesNB = allocatedRb;
                /* Cyclomatic Complexity Changes -starts here */
                /* Cyclomatic_complexity_changes_start */
                updatePMIIndexInHarqContext(dlUEContext_p,
                        harqProcess_p,
                        macAdvanceFactor,
                        widebandPMI,
                        internalCellIndex,
                        allocatedRb
                        #ifdef TDD_CONFIG
                        ,isSpecialSubframe
                        #endif
                        ) ;
                /* Cyclomatic_complexity_changes_end */
                /* Cyclomatic Complexity Changes -ends here */

                /*In case of New Transmission set the Transport block size in HARQ TB considering the fact
                  that this could be special subframe in case of TDD*/
            }

        }
        else if (RE_TX == dlHARQTBOneInfo_p->txIndicator ||
                RE_TX == dlHARQTBTwoInfo_p->txIndicator)
        {
            /* ICIC changes start */
            /* In case of retransmission, if sufficient resources are not
             * available in cell edge try to allocate from cell center resources 
             */
            if (CE_REGION == resourceRegionAllocated)
            {
                availableRBCount = getMaxPrbSubsetIndexForSpatialMul(rat1Info_p,
                        &subsetIndexAllocated,
                        &isNonZeroSpanConsideredForAlloc,
                        &tempRBBitMap,
                        CC_CE_REGION,
                        /* + TM7_8 Changes Start */
                        centralRbMask,
                        scheduledTick,
                        dlUEContext_p->dlMIMOInfo.transmissionMode);
                /* - TM7_8 Changes End */
            }
            /* Cyclomatic Complexity Changes -starts here */ 
            if (availableRBCount < requiredRBCount)
            {
                LOG_MAC_MSG(MAC_RB_NOT_AVAILABLE_FOR_RETX,LOGDEBUG,
                        MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                        (UInt32)availableRBCount,(UInt32)requiredRBCount,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"\nRB required more than RB available for retx");
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion = %s with Failure as \
                        case of ReTx and availableRBCount < \
                        requiredRBCount \n",
                        __func__);
                /*            allocationFailureCause = RETX_RB_NOT_AVAILABLE; TDD Warning Fix */
                /* CA Stats Changes Start */
                LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(
                        RETX_RB_NOT_AVAILABLE,ueIndex, internalCellIndex);
                /* CA Stats Changes End */
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            (resrcAllocatorInput_p->requiredRB),
                            internalCellIndex);
                /* ICIC changes end */

                lteWarning("[%s] Available Rb[%d] Count is  less than req[%d] \n",__func__, availableRBCount, requiredRBCount);
                return MAC_FAILURE;
            }
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "availableRBCount >= requiredRBCount\n");
            allocatedRb = requiredRBCount;

            /* Cyclomatic Complexity Changes -ends here */ 

            /* ICIC changes end */
        }
        else
        {
            allocatedRb = availableRBCount;
            if (dlStrategyTxNode_p->strictAllocatedFlag)
            {
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                        (resrcAllocatorInput_p->requiredRB),
                        internalCellIndex);
                /* ICIC changes end */
                return MAC_FAILURE;
            }

            //update excessPool but it seems that it is the last UE as available resources are less than required.
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                    (resrcAllocatorInput_p->requiredRB - allocatedRb),
                    internalCellIndex);
            /* ICIC changes end */

            dlHARQTBOneInfo_p->mcsIndex = widebandMCSIndexCW1;
            dlHARQTBTwoInfo_p->mcsIndex = widebandMCSIndexCW2;
            harqProcess_p->assignedResourcesNB = allocatedRb;
            /* Cyclomatic Complexity Changes -starts here */
            /* Cyclomatic_complexity_changes_start */	
            updatePMIIndexInHarqContext(dlUEContext_p,
                    harqProcess_p,
                    macAdvanceFactor,
                    widebandPMI,
                    internalCellIndex,
                    allocatedRb
                    #ifdef TDD_CONFIG
                    ,isSpecialSubframe
                    #endif
                    ) ;
            /* Cyclomatic_complexity_changes_end */
            /* Cyclomatic Complexity Changes -ends here */

            /*In case of New Transmission set the Transport block size in HARQ TB considering the fact
              that this could be special subframe in case of TDD*/
        }
    } //CQI 

    /* SPR# 3233 Changes Starts */
    harqProcess_p->assignedResourcesNB = allocatedRb;
    /* SPR# 3233 Changes Ends */
    /*    rbgSize = rat0Info_p->rbgSize; TDD Warning Fix */
    /* + TM7_8 Changes Start */
    if(((dlUEContext_p->dlMIMOInfo.transmissionMode == TX_MODE_7)&&(harqProcess_p->dciFormat == DCI_FORMAT_1))
            ||((dlUEContext_p->dlMIMOInfo.transmissionMode == TX_MODE_8)&&(harqProcess_p->dciFormat == DCI_FORMAT_2B)))
    {
        if(MAC_FAILURE == checkCodeRateUECategoryForTransMode_7_8(
                    dlUEContext_p,harqProcess_p, internalCellIndex))
        {
            /* SPR 13903 fix start */
            LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                    MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                    RA_POLICY_1,harqProcess_p->assignedResourcesNB,
                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            /* SPR 13903 fix end */

            return MAC_FAILURE;
        }
    }
    else
    {
        /* - TM7_8 Changes End */
        /*Dynamic PDCCH + 5117 changes starts */
        /* Need to validate MCS/RB pair for non 0/5
         * Subframes for mCS 28.
         * In case of non0/5 SF and MCS28, we need to 
         * check for coderate and ueCat both while in
         * non 0/5 SF, for other MCS values, 
         * we just need to check uecategory*/

        if (MAC_FAILURE == checkCodeRateUECategory(
                    RA_POLICY_1, 
                    scheduledTick,
                    harqProcess_p,
                    dlUEContext_p,
                    TRUE,
                    availableRBBitMap,
                    /* +- SPR 17777 */
                    internalCellIndex))
        {
            /* SPR 13903 fix start */
            LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                    MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                    RA_POLICY_1,harqProcess_p->assignedResourcesNB,
                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            /* SPR 13903 fix end */


            return MAC_FAILURE;
        }
        /*Dynamic PDCCH + 5117 changes ends */
        /* + TM7_8 Changes Start */
    }
    /* - TM7_8 Changes End */
    /* ICIC changes start */
    rat0BitMap = rat0Info_p->rat0BitMap[CC_USER];
    /* ICIC changes end */
    /* Poulating the resrcAssignementBitMap such that ceil_wrapper(log_wrapper(rbgSize_g)) bits
     * are used for indicating the SubSet Index, 1 bit is used for indicating 
     * the span and remaining bits are used to address  PRBs with in a SubSet*/    
    resrcAssignementBitMap |= subsetIndexAllocated << (rat0Info_p->rbgCount - 
            rat1Info_p->bitsCountForSubsetCount );
    resrcAssignementBitMap |= (isNonZeroSpanConsideredForAlloc << 
            (rat0Info_p->rbgCount - rat1Info_p->bitsCountForSubsetCount - 1 )); 
    /*Get the required Number of RB Count*/
    requiredRBCount = harqProcess_p->assignedResourcesNB;  
    /*Get Maximum Number of Addressable PRB in a Subset.As tempBitMap is filled
     *in such a fashion that from LSB to MSB ,RBs are  positioned in increasing
     *Frequency ,So while forming the resource Assignemnt Bitmap ,We will use 
     *this variable to actually place  the RBs that have been allocated from 
     *tempBitMap in resource Assignemnt Bitmap such that from MSB to LSB 
     *RBs are positioned in increasing Frequency*/
    maxNumAddressableRB = rat1Info_p->maxAddressableRBCount;
    maxNumAddressableRB--;  

    resrcAllocatorOutput_p->allocatedRB = requiredRBCount;

    /* +DYNAMIC_ICIC */
    if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
    {
        calculatedRNTP = calculateRNTP(dlUEContext_p->pA, harqProcess_p, internalCellIndex);
    }
    /* -DYNAMIC_ICIC */

    /*Below while loop is finding the free RB position in the tempRBBitMap
     *and then is updating the tempRBBitMap  such that Bit corrosponding to
     *RB will be unset.It is also updating the resrcAssignementBitMap,It  Calls
     *relevant funtions which will update the RAT 0 ,RAT 2 Localized anf
     *RAT2 Distributed Inforrmation*/    
    availableRBBitMap = tempRBBitMap;

    while ( requiredRBCount )
    {
        if ((rbPosition = FFS(tempRBBitMap))) 
        {
            requiredRBCount -= 1;     
            rbPosition--;
            bitMapForSetBit =  (UInt32)1 << rbPosition;
            tempRBBitMap &=  (~bitMapForSetBit);            

            if (0 == isNonZeroSpanConsideredForAlloc)
            {
                resrcAssignementBitMap |= ((UInt32)1 <<
                        (maxNumAddressableRB  - rbPosition));
            }
            else
            {
                resrcAssignementBitMap |= ((UInt32)1 <<
                        (maxNumAddressableRB + subsetInfo_p->spanWidth  - rbPosition));
            }

            prbNumber = subsetInfo_p->
                bitPositionToSubsetAndRBGInfo[rbPosition].prbNumber;
            /*Dynamic PDCCH + 5117 changes starts*/
     /* SPR 23393 Changes Start */
            if (!(scheduledTick %5))
     /* SPR 23393 Changes End*/
            {
                if(checkAllocRBLiesInCentralSix(prbNumber,internalCellIndex))
                {
                    (*numOfOverLappingRb)++;
                }
            }
            /* SPR_7516_FIX */
#ifdef TDD_CONFIG
     /* SPR 23393 Changes Start */
            if(getSubframeType(scheduledTick%MAX_SUB_FRAME, internalCellIndex)== SP)
     /* SPR 23393 Changes End*/
            {
                if(checkAllocRBLiesInCentralSix(prbNumber,internalCellIndex))
                {
                    (*numOfOverLappingRb)++;
                }
            }
#endif
            /* SPR_7516_FIX */
            /*Dynamic PDCCH + 5117 changes ends*/
            subsetInfo_p->numPRBFree -= 1;
            updateRAT0InfoFromRAT1Info(subsetInfo_p->
                    bitPositionToSubsetAndRBGInfo[rbPosition].rbgIndexBitMap ,
                    &rat0BitMap);
            updateRAT2InfoFromRAT1Info(prbNumber, raPolicy_p, internalCellIndex);

            /* +DYNAMIC_ICIC */
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd. \
                dlICICReportInfo_p[rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                activeReportIndex].rntpValuePerPRB[prbNumber] += calculatedRNTP;
            /* -DYNAMIC_ICIC */

            /* + CQI_5.0 */
            if( (TRUE == dlUEContext_p->actualPMIValueForDLSCH) &&         
                    (TRUE == resrcAllocatorInput_p->aperiodicMode12) )
            {
                SubBandToCQInfo *subBandToCQInfo_p = 
                    &resrcAllocatorInput_p->subBandReportInfo_p->subBandToCQInfo[0];  
        /*SPR 20899 Fix Start*/
                newSubBandNum =   
                    DIVIDE_BY_TWO(subsetInfo_p->bitPositionToSubsetAndRBGInfo[rbPosition].rbgIndex );  
        /*SPR 20899 Fix Stop*/
                SubBandToCQInfo *tempSubBandToCQInfo_p = &subBandToCQInfo_p[newSubBandNum];
                if(newSubBandNum != prevSubBandNum)
                {    
                    resrcAllocatorOutput_p->subbandtoPMIInfo[tempSubbandIndex] = tempSubBandToCQInfo_p->preCodingMatrixIndex;
                    prevSubBandNum = newSubBandNum;
                    /* Cyclomatic Complexity Changes -starts here */
                    if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
                    {
                        
                        /*Rel 5.3.1 cyclomatic chg start*/
                        /*4x4 DL MIMO CHG START*/
                        setSubbandtoPMIInfoForResrcAlloc(dlUEContext_p,harqProcess_p,                       
                                                      resrcAllocatorOutput_p,tempSubbandIndex);
                        /*4x4 DL MIMO CHG END*/                              
                        /*Rel 5.3.1 cyclomatic chg end*/
                        /* Cyclomatic Complexity Changes -ends here */
                        tempSubbandIndex++;
                    }
                }
            }
            /* - CQI_5.0 */
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,"Error has occured in .FFS should not return 0 %s\n",__func__);
            break;
        }
    }

    /* Cyclomatic Complexity Changes -starts here */
    updateRAT1SubBandPMIInfo(dlUEContext_p,
            harqProcess_p,
            resrcAllocatorInput_p,
            availableRBBitMap,
            tempRBBitMap,
            subsetIndexAllocated,
            isNonZeroSpanConsideredForAlloc );
    /* Cyclomatic Complexity Changes -ends here */

    dlUEContext_p->actualPMIValueForDLSCH = FALSE;        
    resrcAllocatorInput_p->aperiodicMode12 = FALSE;
    resrcAllocatorOutput_p->pmiValue =  harqProcess_p->pmiIndex;
    /* - CQI_5.0 */

    /*Updating the resrcAllocatorOutput_p structure with relevant information*/
    resrcAllocatorOutput_p->resourceAssignmentBitMap = resrcAssignementBitMap;
    resrcAllocatorOutput_p->ratPolicy = RA_POLICY_1;
    resrcAllocatorOutput_p->numberOfSubbands = tempSubbandIndex;
    /* ICIC changes start */
    /* Update bitmaps for cell centre users, bitmap for cell edge users is 
     * is derived from cell centre bitmaps
     */
    rat0Info_p->rat0BitMap[CC_USER] = rat0BitMap;
    subsetInfo_p->subsetBitMap[CC_USER] = availableRBBitMap ^ 
        subsetInfo_p->subsetBitMap[CC_USER] ^ tempRBBitMap;
    /* ICIC changes end */
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    return MAC_SUCCESS;
}
/******************************************************************************
 * Function Name    : allocRBInRAT0ForNonSpatialMul
 * Inputs           : resrcAllocatorInput_p - This holds Pointer to 
 *                                            ResourceAllocatorInput structure,
 *                    harqProcess_p -  This holds pointer to DLHARQProcess structure,
 *                    dlUEContext_p -  This holds pointer to DLUEContext structure,
 *                    raPolicy_p - This holds pointer to ResourceAllocationPolicyInfo 
 *                                 structure,
 *                    dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                    scheduledTick - Scheduled TTI,
 *                    numOfOverLappingRb - Number of Overlapping RB's
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : resrcAllocatorOutput_p - This holds pointer to 
 *                                             ResourceAllocatorOutput structure 
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will first  call relevant funtions for getting
 *                    the BitMap which it will process and then update the relevant
 *                    information in ResourceAllocatorOutput structure.
 ******************************************************************************/
#ifdef FDD_CONFIG
STATIC MacRetType allocRBInRAT0ForNonSpatialMul(
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQProcess *harqProcess_p,
        DLUEContext *dlUEContext_p,
        ResourceAllocationPolicyInfo *raPolicy_p,
        DLStrategyTxNode *dlStrategyTxNode_p
        /*Dynamic PDCCH + 5117 changes starts*/
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends*/
         ,InternalCellIndex internalCellIndex
        )
#elif TDD_CONFIG
STATIC MacRetType allocRBInRAT0ForNonSpatialMul(
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQProcess *harqProcess_p,
        DLUEContext *dlUEContext_p,
        ResourceAllocationPolicyInfo *raPolicy_p,
        DLStrategyTxNode *dlStrategyTxNode_p,
        /*Dynamic PDCCH + 5117 changes starts*/
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 *numOfOverLappingRb,
        /*Dynamic PDCCH + 5117 changes ends*/
        UInt8 subFrame
         ,InternalCellIndex internalCellIndex
         )
#endif
{
    UInt32 tempRBGBitMap = 0;
    UInt32 availableRBGBitMap = 0;
    UInt32 rbgSize = 0;
    UInt32 availableRBCount = 0;
    SInt32 requiredRBCount = 0;
    UInt32 rbgIndex = 0;
    UInt32 rbgPositionBitMap = 0;
    UInt32 resrcAssignementBitMap = 0;
    UInt32 rbgCount = 0;
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;
    RBGIndexInfo *rbgIndexInfo_p = PNULL; 
    UInt32 rbsInRBG = 0;
    UInt32 numFreePRB = 0;
    UInt32 allocatedRb = 0;
    UInt32 rbsAllocated = 0;
    DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
    UInt8 usePrevRbCoding = FALSE;
    UInt8 lastRBGAvailable = FALSE;
#ifdef SSI_DEBUG_STATS
    UInt16 ueIndex = dlUEContext_p->ueIndex;
#endif
    /*HD FDD Changes Start*/
    UInt32 widebandMCSIndex = 0;
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
#endif
    /*HD FDD Changes End*/
    UInt32 maxTBSize = dlStrategyTxNode_p->maxTBSize;
    UInt32 minTBSize = dlStrategyTxNode_p->minTBSize;
    /* ICIC changes start */
    UInt32 resourceRegionAllocated = 
        resrcAllocatorInput_p->resourceRegionAllocated;
    /* ICIC changes end */

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);

    rat0Info_p = &raPolicy_p->rat0Info;
    rat1Info_p = &raPolicy_p->rat1Info;

    /* ICIC changes start */
    /* For CC_CE_REGION allocated, logical AND will give cell centre subset
     * bitmap. For CE_REGION, it will give cell edge subset bitmap 
     */
    availableRBGBitMap = rat0Info_p->rat0BitMap[CC_USER] &
        rat0Info_p->rat0BitMap[resourceRegionAllocated];
    /* ICIC changes end */
    /*Get the RBG Size*/
    rbgSize = rat0Info_p->rbgSize;

    /*Get the rbgCount */
    rbgCount = rat0Info_p->rbgCount;

    /* +DYNAMIC_ICIC */
    /* This parameter represents calculated RNTP value of UE getting scheduled 
     * calculation is based on 36.213, section 5.2.1 
     */
    UInt8 calculatedRNTP = 0;
    /* -DYNAMIC_ICIC */
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "rbgCount = %u rbgSize= %u \n",rbgCount,rbgSize);

    /* Calculate the number of Free RB for RAT0 */ 
    numFreePRB  = getCount(availableRBGBitMap) * rbgSize;
    if (availableRBGBitMap & ((UInt32) 1 << (rat0Info_p->rbgCount - 1)))
    {
        numFreePRB = numFreePRB - rbgSize +
            rat0Info_p->rbgIndexInfo[rat0Info_p->rbgCount - 1].numOfRBs;
        lastRBGAvailable = TRUE;
    }


    if ((!numFreePRB) || (!resrcAllocatorInput_p->requiredRB))  
    {
        LOG_MAC_MSG(MAC_NO_RB_LEFT_FOR_ALLOCATION,LOGDEBUG,
                MAC_PDSCH,getCurrentTick(),(UInt32)numFreePRB,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "numFreePRB = %u  \n Leaving funtion with Failure = %s\n",
                numFreePRB, __func__);

        /* allocationFailureCause = RB_NOT_AVAILABLE;TDD Warning Fix */
        /* CA Stats Changes Start */
        LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(RB_NOT_AVAILABLE,
                ueIndex, internalCellIndex);
        /* CA Stats Changes End */

            return MAC_FAILURE;
    }

    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "rbgCount = %u rbgSize= %u \n",rbgCount,rbgSize);

    /* + CQI_4.1 */
    availableRBCount = numFreePRB;
#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe =checkForSpecialSubframe(subFrame, internalCellIndex);
#endif
    /* Fill the MCS Index CodeWord1 from the DL UE Context */
    /*spr 15832 +*/
    if(IS_PCELL_OF_UE(dlUEContext_p,internalCellIndex))
    {
        widebandMCSIndex = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordOne; 
    }
    else
    {
        widebandMCSIndex = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordOne;
    }
    /*spr 15832 -*/
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currTick, dlUEContext_p, widebandMCSIndex);
#endif
    /*HD FDD Changes End*/

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "availableRBCount= %u\n",availableRBCount); 

    /* If subband reports are available and this is not TA and DRX message then try to 
       allocate UE reported subbands and if not possible then use wideband cqi for allocation*/
    if ( (resrcAllocatorInput_p->subBandReportInfo_p) && 
         ( (harqProcess_p->dlHARQTBOneInfo.taDrxFlag == INVALID_TA_DRX_MSG) ||
           ( (dlStrategyTxNode_p->msgType == SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL) ||
             (dlStrategyTxNode_p->msgType == SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL))))
    {
        /* Cyclomatic Complexity Changes -starts here*/
        if(MAC_FAILURE  == allocateRBRat0NonSpatialForSubBand(dlUEContext_p,
                    resrcAllocatorInput_p,
                    raPolicy_p,
                    harqProcess_p,
                    dlStrategyTxNode_p,
                    &allocatedRb,
                    availableRBCount,
                    availableRBGBitMap
#ifdef TDD_CONFIG
                    ,subFrame
#endif 
                    ,&tempRBGBitMap,
                    internalCellIndex))
        {
            return MAC_FAILURE;
        }
        /* Cyclomatic Complexity Changes -ends here*/
    }
    else
    {
        /*- CQI_4.1 */
        if (availableRBCount >= resrcAllocatorInput_p->requiredRB)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "availableRBCount > requiredRB\n");                    
            tempRBGBitMap = availableRBGBitMap;

            /* SPR 3679 Changes Start*/
            if ((RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator) ||
                    (RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator))
                /* SPR 3679 Changes End*/
            {
                if (( resrcAllocatorInput_p->requiredRB%rbgSize) )
                {
                    if ( lastRBGAvailable )
                    {
                        usePrevRbCoding = TRUE;
                    }
                    else
                    {
                        /* allocationFailureCause = RETX_RB_NOT_AVAILABLE;TDD Warning Fix*/
                        /* CA Stats Changes Start */
                        LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(
                                RETX_RB_NOT_AVAILABLE,ueIndex, internalCellIndex);
                        /* CA Stats Changes End */
                            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                                    "rbg with remaining RBs not free\
                                    cannot alloc prev resources for retx\n");
                        /* ICIC changes start */
                        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                (resrcAllocatorInput_p->requiredRB),
                                internalCellIndex);
                        /* ICIC changes end */
                        return MAC_FAILURE;
                    }
                }
            }
            else
            {       
                /* This is new transmission and we can allocate more than 
                 * minTBSize based on excessPool
                 */
                if ((excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0) && 
                        (maxTBSize > minTBSize))
                {      
                    allocatedRb = allocatePRBsUsingExcessPool(widebandMCSIndex, 
                            availableRBCount,
                            maxTBSize,
                            resrcAllocatorInput_p->requiredRB,
                            resourceRegionAllocated,
                            internalCellIndex);

                    /*First update the excess Pool with the extra RBs required to achieve close to maxTBSize*/
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            (-allocatedRb),
                            internalCellIndex);
                    /* ICIC changes end */
                    allocatedRb += resrcAllocatorInput_p->requiredRB;
                }
                else
                {
                    allocatedRb = resrcAllocatorInput_p->requiredRB;
                }

                harqProcess_p->dlHARQTBOneInfo.mcsIndex = widebandMCSIndex;
                harqProcess_p->assignedResourcesNB = allocatedRb;

                /*In case of New Transmission set the Transport block size in HARQ TB considering the fact
                  that this could be special subframe in case of TDD*/
                  /* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
                setTbSizeBasedOnSplSubframe(harqProcess_p,
                        allocatedRb);
#elif TDD_CONFIG
                setTbSizeBasedOnSplSubframe(harqProcess_p,
                        allocatedRb,
                        isSpecialSubframe);
#endif
                /* Cyclomatic_complexity_changes_end */
            }
        }
        else if (RE_TX ==  harqProcess_p->dlHARQTBOneInfo.txIndicator || 
                RE_TX ==  harqProcess_p->dlHARQTBTwoInfo.txIndicator) /* SPR
                                                                         3679
                                                                         Changes*/
        {
            /* +SPR 5227 Changes Start */
            if (( resrcAllocatorInput_p->requiredRB%rbgSize) )
            {
                if ( lastRBGAvailable )
                {
                    usePrevRbCoding = TRUE;
                }
                else
                {
                    /* CA Stats Changes Start */
                    LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(
                            RETX_RB_NOT_AVAILABLE,ueIndex, internalCellIndex);
                    /* CA Stats Changes End */
                        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                                "rbg with remaining RBs not free\
                                cannot alloc prev resources for retx\n");
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            (resrcAllocatorInput_p->requiredRB),
                            internalCellIndex);
                    /* ICIC changes end */
                    return MAC_FAILURE;
                }
            }
            /* -SPR 5227 Changes end */
            /* ICIC changes start */
            /* In case of cell edge retransmission, if availableRBCount count
             * is not sufficient, we check availableRBCount count from 
             * cell centre region
             */
            if (CE_REGION == resourceRegionAllocated)
            {
                tempRBGBitMap = rat0Info_p->rat0BitMap[CC_USER];
                numFreePRB  = getCount(tempRBGBitMap) * rbgSize;
                if (tempRBGBitMap & ((UInt32) 1 << (rat0Info_p->rbgCount - 1)))
                {
                    numFreePRB = numFreePRB - rbgSize +
                        rat0Info_p->rbgIndexInfo[rat0Info_p->rbgCount - 1].numOfRBs;
                }
            }

            if (numFreePRB >= resrcAllocatorInput_p->requiredRB)
            {
                allocatedRb = resrcAllocatorInput_p->requiredRB;
            }
            else
            {
                LOG_MAC_MSG(MAC_RB_NOT_AVAILABLE_FOR_RETX,LOGDEBUG,
                        MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                        (UInt32)availableRBCount,
                        (UInt32)resrcAllocatorInput_p->requiredRB,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"\nRB required more than RB available for retx");

                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "availableRBCount < requiredRB \n");

                /*           allocationFailureCause = RETX_RB_NOT_AVAILABLE; TDD Warning Fix */
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                        (resrcAllocatorInput_p->requiredRB),
                        internalCellIndex);
                /* ICIC changes end */
                /* CA Stats Changes Start */
                LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(
                        RETX_RB_NOT_AVAILABLE,ueIndex, internalCellIndex);
                /* CA Stats Changes End */

                    return MAC_FAILURE;
            }
            /* ICIC changes end */
        }
        else
        {
            if (dlStrategyTxNode_p->strictAllocatedFlag)
            {
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                        (resrcAllocatorInput_p->requiredRB),
                        internalCellIndex);
                /* ICIC changes end */
                return MAC_FAILURE;
            }
            allocatedRb = availableRBCount;

            //update excessPool but it seems that it is the last UE as available resources are less than required.
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                    (resrcAllocatorInput_p->requiredRB - allocatedRb),
                    internalCellIndex);
            /* ICIC changes end */

            tempRBGBitMap = availableRBGBitMap;

            harqProcess_p->dlHARQTBOneInfo.mcsIndex = widebandMCSIndex;
            harqProcess_p->assignedResourcesNB = allocatedRb;

            /*In case of New Transmission set the Transport block size in HARQ TB considering the fact
              that this could be special subframe in case of TDD*/
              /* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
            setTbSizeBasedOnSplSubframe(harqProcess_p,
                    allocatedRb);
#elif TDD_CONFIG
            setTbSizeBasedOnSplSubframe(harqProcess_p,
                    allocatedRb,
                    isSpecialSubframe);
#endif
            /* Cyclomatic_complexity_changes_end */
        }
    }    
    /*- CQI_4.1 */

    /* Below For loop is finding the position of first free PRB and then it is 
     * updating  variable availableRBGBitMap such that bit corrosponding to 
     * postion of free PRB is unset.It is also updating the 
     * resrcAssignementBitMap  variable such that bit corrosponding to  postion
     * of free PRB is set.It is also relevant functions which will update the
     * RAT 1 ,RAT 2 Locaized anf RAT 2 Distributed Information */

    /* +DYNAMIC_ICIC */
    if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
    {
        calculatedRNTP = calculateRNTP(dlUEContext_p->pA, harqProcess_p, internalCellIndex);
    }
    /* +DYNAMIC_ICIC */
    /*Dynamic PDCCH + 5117 changes starts */
    /* Need to validate MCS/RB pair for non 0/5
     * Subframes for mCS 28.
     * In case of non0/5 SF and MCS28, we need to 
     * check for coderate and ueCat both while in
     * non 0/5 SF, for other MCS values, 
     * we just need to check uecategory*/
    if (MAC_FAILURE == checkCodeRateUECategory(
                RA_POLICY_0, 
                scheduledTick,
                harqProcess_p,
                dlUEContext_p,
                TRUE,
                availableRBGBitMap,
                /* +- SPR 17777 */
                internalCellIndex))
    {
        /* SPR 13903 fix start */
        LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                RA_POLICY_0,harqProcess_p->assignedResourcesNB,
                harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        /* SPR 13903 fix end */

        return MAC_FAILURE;
    }
    requiredRBCount = harqProcess_p->assignedResourcesNB;
    /*Dynamic PDCCH + 5117 changes ends */

    resrcAllocatorOutput_p->allocatedRB = requiredRBCount;
    rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[0];      
    availableRBGBitMap = tempRBGBitMap;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "requiredRBCount = %u\n",requiredRBCount);


    while ( requiredRBCount > 0)
    {
        if (usePrevRbCoding == TRUE)
        {
            rbgIndex = rat0Info_p->rbgCount;
            usePrevRbCoding = FALSE;
        }
        else
        {
            rbgIndex = FFS(tempRBGBitMap);
        }

        if (rbgIndex)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "rbgIndex = %u\n",rbgIndex);
            rbgIndex--;
            rbsInRBG = rbgIndexInfo_p[rbgIndex].numOfRBs;
            requiredRBCount -= rbsInRBG;
			/*CSR 55962 Start
             * L2 should not allocate more than required RBs becuase it can violate
             * UE-Category limitations which have been validated in the function 
             * above*/
            /* SPR# 13585 fix start */
            if(((requiredRBCount < 0) && (harqProcess_p->assignedResourcesNB > rbsInRBG)) 
                    /*Coverity_73523 Fix Start*/ 
                    && (0 < rbsAllocated)
                    /*Coverity_73523 Fix End*/ 
              )
                /* SPR# 13585 fix end */
            {
                /* SPR 10755 changes start */
                harqProcess_p->assignedResourcesNB = rbsAllocated;
                /* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
                setTbSizeBasedOnSplSubframe(harqProcess_p,
                        rbsAllocated);
#elif TDD_CONFIG
                setTbSizeBasedOnSplSubframe(harqProcess_p,
                        rbsAllocated,
                        isSpecialSubframe);
#endif
                /* Cyclomatic_complexity_changes_end */
                /* SPR 10755 changes start */
                break;
            }
			/*CSR 55962 End*/
            rbsAllocated += rbsInRBG; 
            rbgPositionBitMap = (UInt32)1 << rbgIndex;
            tempRBGBitMap &=  (~rbgPositionBitMap);
            resrcAssignementBitMap |= ((UInt32)1 << (rbgCount - rbgIndex - 1)); 
            if ( raPolicy_p->rat2LocalizedInfo.maxLocalizedVRB  > 10 )
            {
                updateRAT1InfoFromRAT0Info(rat1Info_p, 
                        &rbgIndexInfo_p[rbgIndex]);
            }
            /* +DYNAMIC_ICIC */
            updateRAT2InfoFromRAT0Info(rbgIndex, rbgSize, 
                    rbsInRBG, raPolicy_p,calculatedRNTP,internalCellIndex);
            /* -DYNAMIC_ICIC */
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,
                    "Error has occured in .FFS should not return 0 %s\n",
                    __func__);
            break;
        }
    }


    /* This is possible due to the fact we allocate in RB group size and asked resorces may not be 
       in multiple of RB group size hence if this is the case then adjust HARQ fields too*/
    if ( rbsAllocated > harqProcess_p->assignedResourcesNB )
    {
        /* ICIC changes start */
        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                -(rbsAllocated - harqProcess_p->assignedResourcesNB),
                internalCellIndex);
        /* ICIC changes end */

        if ( NEW_TX == dlHARQTBOneInfo_p->txIndicator)
        {
            harqProcess_p->assignedResourcesNB = rbsAllocated;
            /* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
            setTbSizeBasedOnSplSubframe(harqProcess_p,
                    rbsAllocated);
#elif TDD_CONFIG
            setTbSizeBasedOnSplSubframe(harqProcess_p,
                    rbsAllocated,
                    isSpecialSubframe);
#endif
            /* Cyclomatic_complexity_changes_end */
        }
        else
        {
            ltePanic("Invalid Scenario TX Type [%d] Harq PRB [%d] Allocated [%d]\n",
                    dlHARQTBOneInfo_p->txIndicator,
                    harqProcess_p->assignedResourcesNB,
                    rbsAllocated);
        }   
    }
    /* SPR 9625 Fix Start */
    else
    {
	UPDATE_EXCESS_POOL_RB(resourceRegionAllocated,
               (harqProcess_p->assignedResourcesNB-rbsAllocated),
               internalCellIndex);
	harqProcess_p->assignedResourcesNB = rbsAllocated;
    /* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
        /*Coverity_73523 Fix Start*/ 
        if(0 < rbsAllocated)
        {
            setTbSizeBasedOnSplSubframe(harqProcess_p,
                    rbsAllocated);
        }
        /*Coverity_73523 Fix End*/ 
#elif TDD_CONFIG
            setTbSizeBasedOnSplSubframe(harqProcess_p,
                    rbsAllocated,
                    isSpecialSubframe);
#endif
    /* Cyclomatic_complexity_changes_end */
    }
    /* SPR 9625 Fix End */
    /*Updating the resrcAllocatorOutput_p structure with relevant information*/

    resrcAllocatorOutput_p->allocatedRB = rbsAllocated;

    resrcAllocatorOutput_p->resourceAssignmentBitMap = resrcAssignementBitMap;
    resrcAllocatorOutput_p->ratPolicy = RA_POLICY_0;
    /* ICIC changes start */
    /* update RAT0 bitmap for cell centre user, bitmap for cell edge user is 
     * derived from cell centre bitmap 
     */
    rat0Info_p->rat0BitMap[CC_USER] = availableRBGBitMap ^  
        rat0Info_p->rat0BitMap[CC_USER] ^ tempRBGBitMap;
    /* ICIC changes end */
    /*Dynamic PDCCH + 5117 changes starts */
     /* SPR 23393 Changes Start */
    if(!(scheduledTick % 5) &&\
            (NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator))
    {
        if ((!(scheduledTick %10)&& !(cellSpecificParams_g.\
             cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                dlSchWithMIBAllowed)))
        {
            *numOfOverLappingRb = 0;
        }
        else
        {
            *numOfOverLappingRb = calculateNumOfOverLapRbs(resrcAssignementBitMap,rbsAllocated,internalCellIndex);
        }
    }
     /* SPR 23393 Changes End*/
    /* SPR_7516_FIX */
#ifdef TDD_CONFIG
     /* SPR 23393 Changes Start*/
    if(getSubframeType(scheduledTick%MAX_SUB_FRAME, internalCellIndex)== SP)
        /* SPR 23393 Changes End*/
    {
        *numOfOverLappingRb = calculateNumOfOverLapRbs(resrcAssignementBitMap,rbsAllocated,internalCellIndex);
    }
#endif
    /* SPR_7516_FIX */
    /*Dynamic PDCCH + 5117 changes ends */

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    return MAC_SUCCESS;
}


/******************************************************************************
 * Function Name    : allocRBInRAT1ForNonSpatialMul
 * Inputs           : resrcAllocatorInput_p - This holds Pointer to 
 *                                            ResourceAllocatorInput structure,
 *                    harqProcess_p -  This holds pointer to DLHARQProcess structure,
 *                    dlUEContext_p -  This holds pointer to DLUEContext structure,
 *                    raPolicy_p - This holds pointer to ResourceAllocationPolicyInfo 
 *                                 structure,
 *                    dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                    scheduledTick - Scheduled TTI,
 *                    numOfOverLappingRb - Number of Overlapping RB's
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : resrcAllocatorOutput_p - This holds pointer to 
 *                                             ResourceAllocatorOutput structure 
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will call relevant funtions for getting available
 *                    RB BitMap and then it will process that bitmap and populate
 *                    the relevant information in ResourceAllocatorOutput structure.
 ******************************************************************************/
#ifdef FDD_CONFIG
STATIC MacRetType allocRBInRAT1ForNonSpatialMul(
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQProcess *harqProcess_p,
        DLUEContext *dlUEContext_p,
        ResourceAllocationPolicyInfo *raPolicy_p,
        DLStrategyTxNode *dlStrategyTxNode_p
        /*Dynamic PDCCH + 5117 changes starts*/
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends*/
        ,InternalCellIndex internalCellIndex)
#elif TDD_CONFIG
STATIC MacRetType allocRBInRAT1ForNonSpatialMul(
        ResourceAllocatorInput *resrcAllocatorInput_p,
        ResourceAllocatorOutput *resrcAllocatorOutput_p,
        DLHARQProcess *harqProcess_p,
        DLUEContext *dlUEContext_p,
        ResourceAllocationPolicyInfo *raPolicy_p,
        DLStrategyTxNode *dlStrategyTxNode_p,
        /*Dynamic PDCCH + 5117 changes starts*/
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
         /* SPR 15909 fix end */
        UInt8 *numOfOverLappingRb,
        /*Dynamic PDCCH + 5117 changes ends*/
        UInt8 subFrame,
        InternalCellIndex internalCellIndex)
#endif
{
    UInt32 tempRBBitMap = 0;    
    UInt32 subsetIndexAllocated = 0;
    UInt32 isNonZeroSpanConsideredForAlloc = 0;
    UInt32 requiredRBCount = 0;
    UInt32 rbPosition = 0;
    UInt32 bitMapForSetBit = 0;
    UInt32 availableRBBitMap = 0;
    SubsetInfo *subsetInfo_p = PNULL;
    UInt32 resrcAssignementBitMap= 0;
    RAT0Info *rat0Info_p = PNULL;
    UInt32 maxNumAddressableRB = 0;
    RAT1Info *rat1Info_p = PNULL;
    UInt32 prbNumber = 0;
/*  UInt32 rbgSize = 0; TDD Warning Fix */
    UInt32 rat0BitMap = 0;
    UInt32 availableRBCount = 0;
    UInt8 allocatedRb = 0;
    /* PdschAllocationFailureType allocationFailureCause = 0; TDD Warning Fix */

#ifdef SSI_DEBUG_STATS
    UInt16 ueIndex = dlUEContext_p->ueIndex;
#endif    
    DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
    /* + CQI_4.1 */
    UInt32        tempSubBandCQIRBBitMap = 0;
    /*HD FDD Changes Start*/
    UInt8 widebandMCSIndex = 0;
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
#endif
    /*HD FDD Changes End*/

    UInt32 minTBSize = dlStrategyTxNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyTxNode_p->maxTBSize;
    /* - CQI_4.1 */
    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    /* ICIC changes start */
    UInt8 resourceRegionAllocated = 
        resrcAllocatorInput_p->resourceRegionAllocated;
    /* ICIC changes end */
    rat0Info_p = &raPolicy_p->rat0Info;
    rat1Info_p = &raPolicy_p->rat1Info;
    requiredRBCount = resrcAllocatorInput_p->requiredRB;

    /* +DYNAMIC_ICIC */
    /* This parameter represents calculated RNTP value of UE getting scheduled 
     * calculation is based on 36.213, section 5.2.1 
     */
    UInt8 calculatedRNTP = 0;
    /* -DYNAMIC_ICIC */
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);

    /*Now we try to allocate RBs according to wideBands*/
    /* ICIC changes start */
    availableRBCount = getMaxPrbSubsetIndexForNonSpatialMul(rat1Info_p,
            &subsetIndexAllocated,
            &isNonZeroSpanConsideredForAlloc,
            &tempRBBitMap,
            /*coverity-530 CID-32306*/
            (ResourceRegion) resourceRegionAllocated);
            /*coverity-530 CID-32306*/
    /* ICIC changes end */

    if ( !availableRBCount )
    {
        LOG_MAC_MSG(MAC_NO_RB_LEFT_FOR_ALLOCATION,LOGDEBUG,
            MAC_PDSCH,getCurrentTick(),(UInt32)availableRBCount,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"");

        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,  
            "Leaving funtion = %s with Failure as \
            Available Rb Count is  0\n",
            __func__);

        /* allocationFailureCause = RB_NOT_AVAILABLE;TDD Warning Fix */
        /* CA Stats Changes Start */
        LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(RB_NOT_AVAILABLE,
                ueIndex, internalCellIndex);
        /* CA Stats Changes End */

        return MAC_FAILURE;
    }
    subsetInfo_p = &rat1Info_p->subsetInfo[subsetIndexAllocated];
    /*spr 15832 +*/
    if(IS_PCELL_OF_UE(dlUEContext_p,internalCellIndex))
    {
        widebandMCSIndex = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordOne; 
    }
    else
    {
        widebandMCSIndex = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordOne;
    }
    /*spr 15832 -*/
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* Fill the MCS Index CodeWord1 from the DL UE Context */
    HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currTick, dlUEContext_p, widebandMCSIndex);
#endif
    /*HD FDD Changes End*/

#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe =checkForSpecialSubframe(subFrame, internalCellIndex);
#endif

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "availableRBCount = %u ,requiredRBCount = %u\n",
            availableRBCount, requiredRBCount);   

    /* + CQI_4.1 */
    /*Check if SubBand report is available or not */
    if ( (resrcAllocatorInput_p->subBandReportInfo_p) &&
         ( (dlHARQTBOneInfo_p->taDrxFlag == INVALID_TA_DRX_MSG) ||
           ( (dlStrategyTxNode_p->msgType == SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL) ||
             (dlStrategyTxNode_p->msgType == SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL))))
    {
        if ( NEW_TX == dlHARQTBOneInfo_p->txIndicator )
        {
            tempSubBandCQIRBBitMap = getRAT1BitMapNewTxNonSpatialMux(
                                            rat1Info_p,
                                            resrcAllocatorInput_p,
                                            harqProcess_p,
                                            &subsetIndexAllocated,
                                            &isNonZeroSpanConsideredForAlloc,
                                            dlStrategyTxNode_p
                                            /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                                            ,dlUEContext_p
#endif
                                            ,internalCellIndex
                                            /*HD FDD Changes End*/
                                            );
            if (!tempSubBandCQIRBBitMap)
            {
                /* As of now, there is no use case for strictAllocSubBand
                 * strictAllocSubBand check is commented */
                
                /*Now we try to allocate RBs according to wideBands*/
                if (availableRBCount >= resrcAllocatorInput_p->requiredRB)
                {
                    /*Now we are sure that minTBSize would be satisfied as availableRBs are greater than 
                      required. Based on excessPool try to allocate maxTBSize*/
                    allocatedRb = resrcAllocatorInput_p->requiredRB;
                }
                else
                {
                    /* We can not allocate less than minTBSize if strictScheduled is instructed */
                    if (dlStrategyTxNode_p->strictAllocatedFlag)
                    {
                        /* ICIC changes start */
                        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                (resrcAllocatorInput_p->requiredRB),
                                internalCellIndex);
                        /* ICIC changes end */
                    	return MAC_FAILURE;
                    }
                    allocatedRb = availableRBCount;
                    //update excessPool but it seems that it is the last UE as available resources are less than required.
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            (resrcAllocatorInput_p->requiredRB - allocatedRb),
                            internalCellIndex);
                    /* ICIC changes end */
                }
                
		        harqProcess_p->dlHARQTBOneInfo.mcsIndex = widebandMCSIndex;
                harqProcess_p->assignedResourcesNB = allocatedRb;                    

                LOG_MAC_MSG(MAC_SB_CQI_FAILURE_LOG_ID,LOGDEBUG,
                        MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                        (UInt32)availableRBCount,
                        (UInt32)resrcAllocatorInput_p->requiredRB,
                        tempRBBitMap,allocatedRb,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"SB_FAIL_NWTX");
            }
            else
            {
    			subsetInfo_p = &rat1Info_p->subsetInfo[subsetIndexAllocated]; 
                tempRBBitMap = tempSubBandCQIRBBitMap;
                allocatedRb = harqProcess_p->assignedResourcesNB;

                LOG_MAC_MSG(MAC_SB_CQI_SUCCESS_LOG_ID,LOGDEBUG,
                        MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                        (UInt32)availableRBCount,
                        (UInt32)resrcAllocatorInput_p->requiredRB,
                        tempRBBitMap,allocatedRb,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"SB_PASS_NWTX");
            }
            
            /*In case of New Transmission set the Transport block size in HARQ TB considering the fact
                         that this could be special subframe in case of TDD*/
            /* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
            setTbSizeBasedOnSplSubframe(harqProcess_p,
                                             allocatedRb);
#elif TDD_CONFIG
            setTbSizeBasedOnSplSubframe(harqProcess_p,
                                             allocatedRb,
                                             isSpecialSubframe);            
#endif            
           /* Cyclomatic_complexity_changes_end */
        }
    }
    /* - CQI_4.1 */
    else
    {
        if (availableRBCount >= requiredRBCount)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "availableRBCount >= requiredRBCount\n");
            allocatedRb = requiredRBCount;
            if (NEW_TX == dlHARQTBOneInfo_p->txIndicator)
            {       
                /* This is new transmission and we can allocate more than 
                 * minTBSize based on excessPool
                 */
                if ((excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0) && 
                        (maxTBSize > minTBSize))
                {      
                   allocatedRb = allocatePRBsUsingExcessPool(widebandMCSIndex, 
                                                             availableRBCount,
                                                             maxTBSize,
                                                             resrcAllocatorInput_p->requiredRB,
                                                             resourceRegionAllocated,
                                                             internalCellIndex);

                   /*First update the excess Pool with the extra RBs required to achieve close to maxTBSize*/
                   /* ICIC changes start */
                   UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                           (-allocatedRb),
                           internalCellIndex);
                   /* ICIC changes end */
                   allocatedRb += resrcAllocatorInput_p->requiredRB;
                }
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = widebandMCSIndex;
                harqProcess_p->assignedResourcesNB = allocatedRb;

                /*In case of New Transmission set the Transport block size in HARQ TB considering the fact
                   that this could be special subframe in case of TDD*/
                   /* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
                setTbSizeBasedOnSplSubframe(harqProcess_p,
                                                 allocatedRb);
#elif TDD_CONFIG
                setTbSizeBasedOnSplSubframe(harqProcess_p,
                                                 allocatedRb,
                                                 isSpecialSubframe);
#endif
                /* Cyclomatic_complexity_changes_end */
            }           
        }
        else if (RE_TX ==  dlHARQTBOneInfo_p->txIndicator )
        { 
            /* ICIC changes start */
            /* In case of retransmission, if sufficient resources are not
             * available in cell edge try to allocate from cell center resources 
             */
            if (CE_REGION == resourceRegionAllocated)
            {
                availableRBCount = getMaxPrbSubsetIndexForNonSpatialMul(rat1Info_p,
                        &subsetIndexAllocated,
                        &isNonZeroSpanConsideredForAlloc,
                        &tempRBBitMap,
                        CC_CE_REGION);
            }
            
            if (availableRBCount >= requiredRBCount)
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "availableRBCount >= requiredRBCount\n");
                allocatedRb = requiredRBCount;
            }
            else
            {
                LOG_MAC_MSG(MAC_RB_NOT_AVAILABLE_FOR_RETX,LOGDEBUG,
                        MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                        (UInt32)availableRBCount,(UInt32)requiredRBCount,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"\nRB required more than RB available for retx");

                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Leaving funtion = %s with Failure as \
                        case of ReTx and availableRBCount < \
                        requiredRBCount \n",
                        __func__);

            /*allocationFailureCause = RETX_RB_NOT_AVAILABLE; TDD Warning Fix */
                /* CA Stats Changes Start */
                LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(
                        RETX_RB_NOT_AVAILABLE,ueIndex, internalCellIndex);
                /* CA Stats Changes End */
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                        (resrcAllocatorInput_p->requiredRB),
                        internalCellIndex);
                /* ICIC changes end */

                return MAC_FAILURE;                        
            }
            /* ICIC changes end */
        }
        else
        {
            if (dlStrategyTxNode_p->strictAllocatedFlag)
            {
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                        (resrcAllocatorInput_p->requiredRB),
                        internalCellIndex);
                /* ICIC changes end */
                return MAC_FAILURE;
            }        
            allocatedRb = availableRBCount;

            //update excessPool but it seems that it is the last UE as available resources are less than required.
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                    (resrcAllocatorInput_p->requiredRB - allocatedRb),
                    internalCellIndex);
            /* ICIC changes end */

            harqProcess_p->dlHARQTBOneInfo.mcsIndex = widebandMCSIndex;
            harqProcess_p->assignedResourcesNB = allocatedRb;

            /*In case of New Transmission set the Transport block size in HARQ TB considering the fact
               that this could be special subframe in case of TDD*/
               /* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
            setTbSizeBasedOnSplSubframe(harqProcess_p,
                                              allocatedRb);
#elif TDD_CONFIG
            setTbSizeBasedOnSplSubframe(harqProcess_p,
                                              allocatedRb,
                                              isSpecialSubframe);           
#endif
            /* Cyclomatic_complexity_changes_end */
        }
    }

/* SPR# 3233 Changes Starts */
    harqProcess_p->assignedResourcesNB = allocatedRb;


    /*Dynamic PDCCH + 5117 changes starts */
    /* Need to validate MCS/RB pair for non 0/5
     * Subframes for mCS 28.
     * In case of non0/5 SF and MCS28, we need to 
     * check for coderate and ueCat both while in
     * non 0/5 SF, for other MCS values, 
     * we just need to check uecategory*/

      if(MAC_FAILURE ==   checkCodeRateUECategory(
                RA_POLICY_1, 
                scheduledTick,
                harqProcess_p,
                dlUEContext_p,
                TRUE,
                availableRBBitMap,
                /* +- SPR 17777 */
                internalCellIndex))
      {
          /* SPR 13903 fix start */
          LOG_MAC_MSG(MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,
                  MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                  RA_POLICY_1,harqProcess_p->assignedResourcesNB,
                  harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                  harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                  DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                  FUNCTION_NAME,"");
          /* SPR 13903 fix end */


          return MAC_FAILURE;
      }
    /*Dynamic PDCCH + 5117 changes ends */
 
    /* SPR# 3233 Changes Ends */
    /*  rbgSize = rat0Info_p->rbgSize; TDD Warning Fix */
    /* ICIC changes start */
    rat0BitMap = rat0Info_p->rat0BitMap[CC_USER];
    /* ICIC changes end */
    /* Poulating the resrcAssignementBitMap such that ceil_wrapper(log_wrapper(rbgSize_g)) bits
     * are used for indicating the SubSet Index, 1 bit is used for indicating 
     * the span and remaining bits are used to address  PRBs with in a SubSet*/    
    resrcAssignementBitMap |= subsetIndexAllocated << (rat0Info_p->rbgCount - 
            rat1Info_p->bitsCountForSubsetCount );
    resrcAssignementBitMap |= (isNonZeroSpanConsideredForAlloc << 
            (rat0Info_p->rbgCount - rat1Info_p->bitsCountForSubsetCount - 1 )); 
    /*Get the required Number of RB Count*/
    requiredRBCount = harqProcess_p->assignedResourcesNB;  
    /*Get Maximum Number of Addressable PRB in a Subset.As tempBitMap is filled
     *in such a fashion that from LSB to MSB ,RBs are  positioned in increasing
     *Frequency ,So while forming the resource Assignemnt Bitmap ,We will use 
     *this variable to actually place  the RBs that have been allocated from 
     *tempBitMap in resource Assignemnt Bitmap such that from MSB to LSB 
     *RBs are positioned in increasing Frequency*/
    maxNumAddressableRB = rat1Info_p->maxAddressableRBCount;
    maxNumAddressableRB--;  

    resrcAllocatorOutput_p->allocatedRB = requiredRBCount;
    
    /* +DYNAMIC_ICIC */
    if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
    {
        calculatedRNTP = calculateRNTP(dlUEContext_p->pA, harqProcess_p, internalCellIndex);
    }
    /* -DYNAMIC_ICIC */

    /*Below while loop is finding the free RB position in the tempRBBitMap
     *and then is updating the tempRBBitMap  such that Bit corrosponding to
     *RB will be unset.It is also updating the resrcAssignementBitMap,It  Calls
     *relevant funtions which will update the RAT 0 ,RAT 2 Localized anf
     *RAT2 Distributed Inforrmation*/    
    availableRBBitMap = tempRBBitMap;

    while ( requiredRBCount )
    {
        if ((rbPosition = FFS(tempRBBitMap))) 
        {
            requiredRBCount -= 1;     
            rbPosition--;
            bitMapForSetBit =  (UInt32)1 << rbPosition;
            tempRBBitMap &=  (~bitMapForSetBit);            

            if (0 == isNonZeroSpanConsideredForAlloc)
            {
                resrcAssignementBitMap |= ((UInt32)1 <<
                        (maxNumAddressableRB  - rbPosition));
            }
            else
            {
                resrcAssignementBitMap |= ((UInt32)1 <<
                        (maxNumAddressableRB + subsetInfo_p->spanWidth  - rbPosition));
            }

            prbNumber = subsetInfo_p->
                bitPositionToSubsetAndRBGInfo[rbPosition].prbNumber;
            /*Dynamic PDCCH + 5117 changes starts*/
     /* SPR 23393 Changes Start*/
            if ( (NEW_TX == dlHARQTBOneInfo_p->txIndicator) &&!(scheduledTick %5))
            {
                if (!(scheduledTick %10)&&( !cellSpecificParams_g.\
                    cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                           dlSchWithMIBAllowed))
                {
                    *numOfOverLappingRb = 0;
                }
                else
                {
                    if(checkAllocRBLiesInCentralSix(prbNumber,internalCellIndex))
                    {
                        (*numOfOverLappingRb)++;
                    }
                }
            }
     /* SPR 23393 Changes End*/
#ifdef TDD_CONFIG
     /* SPR 23393 Changes Start*/
    if(getSubframeType(scheduledTick%MAX_SUB_FRAME, internalCellIndex)== SP)
     /* SPR 23393 Changes End*/
    {
                    if(checkAllocRBLiesInCentralSix(prbNumber,internalCellIndex))
                    {
                        (*numOfOverLappingRb)++;
                    }
    }
#endif
            /*Dynamic PDCCH + 5117 changes ends*/
            subsetInfo_p->numPRBFree -= 1;
            updateRAT0InfoFromRAT1Info(subsetInfo_p->
                    bitPositionToSubsetAndRBGInfo[rbPosition].rbgIndexBitMap ,
                    &rat0BitMap);
            updateRAT2InfoFromRAT1Info(prbNumber, raPolicy_p, internalCellIndex);
            
            /* +DYNAMIC_ICIC */
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd. \
                dlICICReportInfo_p[rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                activeReportIndex].rntpValuePerPRB[prbNumber] += calculatedRNTP;
            /* -DYNAMIC_ICIC */
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,"Error has occured in .FFS should not return 0 %s\n",__func__);
            break;
        }
    }

    /*Updating the resrcAllocatorOutput_p structure with relevant information*/
    resrcAllocatorOutput_p->resourceAssignmentBitMap = resrcAssignementBitMap;
    resrcAllocatorOutput_p->ratPolicy = RA_POLICY_1;
    /* ICIC changes start */
    /* update bitmaps for cell centre user, bitmap for cell edge user is 
     * derived from cell centre bitmap 
     */
    rat0Info_p->rat0BitMap[CC_USER] = rat0BitMap;
    subsetInfo_p->subsetBitMap[CC_USER] = availableRBBitMap ^ 
        subsetInfo_p->subsetBitMap[CC_USER] ^ tempRBBitMap;
    /* ICIC changes end */
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    return MAC_SUCCESS;
}

/******************************************************************************
 * Function Name  : allocRBInPDSCHForSpatialMul
 * Inputs         : macAdvanceFactor,
 *                  resourceAllocInput_p - This holds Pointer to 
 *                                         ResourceAllocatorInput structure,
 *                  dlUEContext_p -  This holds pointer to DLUEContext structure, 
 *                  harqProcess_p -  This holds pointer to DLHARQProcess structure,   
 *                  raPolicy_p - This holds pointer to ResourceAllocationPolicyInfo 
 *                               structure,
 *                  dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                  scheduledTick - Scheduled TTI,
 *                  numOfOverLappingRb,
 *                  subframe(TDD),
 *                  internalCellIndex -  Cell-Index at MAC
 * Outputs        : resourceAllocOutput_p - This holds pointer to 
 *                                           ResourceAllocatorOutput structure
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This funtion will call relevant funtions depending upon the
 *                  Resource Allocation Policy for allocating the RBs for Spatial
 *                  Multiplexing.
 *******************************************************************************/
 MacRetType allocRBInPDSCHForSpatialMul(
        /* + CQI_5.0 */
        UInt32 macAdvanceFactor,
        /* - CQI_5.0 */
        ResourceAllocatorInput *resourceAllocInput_p,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        DLUEContext *ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DLStrategyTxNode *dlStrategyTxNode_p
        /*Dynamic PDCCH + 5117 changes starts*/
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends*/
#ifdef TDD_CONFIG        
        ,UInt8           subframe
#endif        
        ,InternalCellIndex internalCellIndex
        )    
{
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    switch (resourceAllocInput_p->preferredRATPolicy)
    {
        case RA_POLICY_0:
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Calling funtion allocRBInRAT0ForSpatialMul\n");
                                               /* + CQI_5.0 */
                return allocRBInRAT0ForSpatialMul(macAdvanceFactor,
                                               /* - CQI_5.0 */
                        resourceAllocInput_p,
                        resourceAllocOutput_p,
                        harqProcess_p,
                        ueDLContext_p,
                        raPolicyInfo_p,
                        dlStrategyTxNode_p
                        /*Dynamic PDCCH + 5117 changes starts*/
                        ,scheduledTick
                        ,numOfOverLappingRb
                        /*Dynamic PDCCH + 5117 changes ends*/
#ifdef TDD_CONFIG
                        ,subframe
#endif
                        ,internalCellIndex
                        );


            }
        case  RA_POLICY_1:
            {

                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Calling funtion allocRBInRAT1ForSpatialMul\n");
                                                  /* + CQI_5.0 */
                return allocRBInRAT1ForSpatialMul(macAdvanceFactor,
                                                  /* - CQI_5.0 */
                        resourceAllocInput_p,
                        resourceAllocOutput_p,
                        harqProcess_p,
                        ueDLContext_p,
                        raPolicyInfo_p,
                        dlStrategyTxNode_p
                        /*Dynamic PDCCH + 5117 changes starts*/
                        ,scheduledTick
                        ,numOfOverLappingRb
                        /*Dynamic PDCCH + 5117 changes ends*/
#ifdef TDD_CONFIG
                        ,subframe
#endif
                        ,internalCellIndex
                        );
            }
        case  RA_POLICY_2_LOCALIZED:
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Calling funtion RA_POLICY_2_LOCALIZED\n");
                /* + CQI_5.0 */                        
                                                      /* + TM6_5.2 */
                return allocRBRAT2LocalizedForSpatial(macAdvanceFactor,
                                                      /* - TM6_5.2 */
                        resourceAllocInput_p,
                        resourceAllocOutput_p,                        
                        raPolicyInfo_p,
                        ueDLContext_p,
                        harqProcess_p,
                        dlStrategyTxNode_p
                        /*Dynamic PDCCH + 5117 changes starts*/
                        ,scheduledTick
                        ,numOfOverLappingRb
                        /*Dynamic PDCCH + 5117 changes ends*/
#ifdef TDD_CONFIG
                        ,subframe
#endif
                        ,internalCellIndex
                        );

                /* - CQI_5.0 */   

            }
        default:
            {   
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "INVALID RAT Policy\n");
            }
            return MAC_FAILURE;
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}

/******************************************************************************
 * Function Name    : allocRBInPDSCHForNonSpatialMul
 * Inputs           : resourceAllocInput_p - This holds Pointer to 
 *                                            ResourceAllocatorInput structure,
 *                    ueDLContext_p -  This holds pointer to DLUEContext structure,
 *                    harqProcess_p -  This holds pointer to DLHARQProcess structure,
 *                    raPolicyInfo_p - This holds pointer to ResourceAllocationPolicyInfo 
 *                                     structure,
 *                    dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                    scheduledTick - Scheduled TTI,
 *                    numOfOverLappingRb - Number of Overlapping RB's
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : resourceAllocOutput_p - This holds pointer to 
 *                                             ResourceAllocatorOutput structure 
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will call relevant funtions depending upon the
 *                    Resource Allocation Policy for allocating the RBs for Non
 *                    Spatial Multiplexing.
 ******************************************************************************/
#ifdef FDD_CONFIG
 MacRetType allocRBInPDSCHForNonSpatialMul(
        ResourceAllocatorInput *resourceAllocInput_p,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        DLUEContext *ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DLStrategyTxNode *dlStrategyTxNode_p
        /*Dynamic PDCCH + 5117 changes starts*/
        /* SPR 15909 fix start */
        ,tickType_t scheduledTick
        /* SPR 15909 fix end */
        ,UInt8 *numOfOverLappingRb
        /*Dynamic PDCCH + 5117 changes ends*/
         ,InternalCellIndex internalCellIndex
        )    
#elif TDD_CONFIG
 MacRetType allocRBInPDSCHForNonSpatialMul(
        ResourceAllocatorInput *resourceAllocInput_p,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        DLUEContext *ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DLStrategyTxNode *dlStrategyTxNode_p,
        /*Dynamic PDCCH + 5117 changes starts*/
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt8 *numOfOverLappingRb,
        /*Dynamic PDCCH + 5117 changes ends*/
        UInt8 subFrame    
         ,InternalCellIndex internalCellIndex)
#endif
{
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    switch (resourceAllocInput_p->preferredRATPolicy)
    {
        case RA_POLICY_0:
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Calling funtion allocRBInRAT0ForNonSpatialMul\n");
#ifdef FDD_CONFIG				
                return allocRBInRAT0ForNonSpatialMul(resourceAllocInput_p,
                        resourceAllocOutput_p,
                        harqProcess_p,
                        ueDLContext_p,
                        raPolicyInfo_p,
                        dlStrategyTxNode_p
                        /*Dynamic PDCCH + 5117 changes starts*/
                        ,scheduledTick,
                        numOfOverLappingRb
                        , internalCellIndex
                        /*Dynamic PDCCH + 5117 changes ends*/
                        );
#elif TDD_CONFIG
                return allocRBInRAT0ForNonSpatialMul(resourceAllocInput_p,
                        resourceAllocOutput_p,
                        harqProcess_p,
                        ueDLContext_p,
                        raPolicyInfo_p,
                        dlStrategyTxNode_p,
                        /*Dynamic PDCCH + 5117 changes starts*/
                        scheduledTick,
                        numOfOverLappingRb,
                        /*Dynamic PDCCH + 5117 changes ends*/
                        subFrame
                        ,internalCellIndex);
#endif              


            }
        case  RA_POLICY_1:
            {

                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Calling funtion allocRBInRAT1NonSpatialMul\n");
#ifdef FDD_CONFIG				
                return allocRBInRAT1ForNonSpatialMul(resourceAllocInput_p,
                        resourceAllocOutput_p,
                        harqProcess_p,
                        ueDLContext_p,
                        raPolicyInfo_p,
                        dlStrategyTxNode_p
                        /*Dynamic PDCCH + 5117 changes starts*/
                        ,scheduledTick,
                        numOfOverLappingRb
                        /*Dynamic PDCCH + 5117 changes ends*/
                        ,internalCellIndex
                        );
#elif TDD_CONFIG                
                return allocRBInRAT1ForNonSpatialMul(resourceAllocInput_p,
                        resourceAllocOutput_p,
                        harqProcess_p,
                        ueDLContext_p,
                        raPolicyInfo_p,
                        dlStrategyTxNode_p,
                        /*Dynamic PDCCH + 5117 changes starts*/
                        scheduledTick,
                        numOfOverLappingRb,
                        /*Dynamic PDCCH + 5117 changes ends*/
                        subFrame,
                        internalCellIndex
                        );
#endif              

            }
        case  RA_POLICY_2_LOCALIZED:
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Calling funtion RA_POLICY_2_LOCALIZED\n");
#ifdef FDD_CONFIG  
                /* ICIC changes start */
                return allocRBRAT2Localized(resourceAllocInput_p,
                        resourceAllocOutput_p,                        
                        raPolicyInfo_p,
                        /* +DYNAMIC_ICIC */
                        ueDLContext_p,
                        /* -DYNAMIC_ICIC */
                        harqProcess_p,
                        dlStrategyTxNode_p
                        /*Dynamic Pdcch Changes Start*/
                        ,scheduledTick,
                        numOfOverLappingRb
                        /*Dynamic Pdcch Changes End*/
                        ,internalCellIndex
                        );
#elif TDD_CONFIG                
                return allocRBRAT2Localized(resourceAllocInput_p,
                        resourceAllocOutput_p,                        
                        raPolicyInfo_p,
                        /* +DYNAMIC_ICIC */
                        ueDLContext_p,
                        /* -DYNAMIC_ICIC */
                        harqProcess_p,
                        dlStrategyTxNode_p
                        /*Dynamic PDCCH + 5117 changes starts*/
                        ,scheduledTick,
                        numOfOverLappingRb
                        /*Dynamic PDCCH + 5117 changes ends*/
                        ,subFrame,
                        internalCellIndex);
#endif

            }
        case  RA_POLICY_2_DISTRIBUTED:
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "Calling funtion allocRBRAT2Distributed\n");
/*#ifdef FDD_CONFIG*/
                /*spr 15832 +*/
                UInt32 wideBandMcsIndex = 0;
                if(IS_PCELL_OF_UE(ueDLContext_p,internalCellIndex))
                {
                    wideBandMcsIndex = ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;
                }
                else
                {
                    wideBandMcsIndex = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordOne;
                }    
                /*spr 15832 -*/
                return allocRBRAT2Distributed(resourceAllocInput_p,
                        resourceAllocOutput_p,                        
                        raPolicyInfo_p,
                        /* spr 15832 start */
                        wideBandMcsIndex,
                        /* spr 15832 end */
                        harqProcess_p
#ifdef TDD_CONFIG                
                        ,subFrame
#endif
                        /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                        ,ueDLContext_p
#endif
                        ,internalCellIndex
                        /* HD FDD Changes End */

                        );
            }
        default:
            {   
                LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                        "INVALID RAT Policy\n");
            }
            return MAC_FAILURE;
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
}

/* QOS PDSCH Change End */

/* ICIC changes start */
/******************************************************************************
 * Function Name    : updateRAT2LocalizedCellEdgeInfo 
 * Inputs           : startVRBIndex - start VRB index of RB allocation,
 *                    endVRBIndex -  end VRB index of RB allocation
 * Outputs          : rat2LocalizedInfo_p - pointer to RAT2LocalizedInfo structure 
 * Returns          : None
 * Description      : This funtion will update firstCellEdgeFreeIndex, 
 *                    endCellEdgeFreeIndex and numOfCellEdgeVRBAvailable for RAT2
 *                    Localized.
 ******************************************************************************/
STATIC void updateRAT2LocalizedCellEdgeInfo(
        RAT2LocalizedInfo *rat2LocalizedInfo_p,
        UInt8 startVRBIndex,
        UInt8 endVRBIndex)
{
    if (rat2LocalizedInfo_p->numOfCellEdgeVRBAvailable)
    {
        if (startVRBIndex <= rat2LocalizedInfo_p->firstCellEdgeFreeIndex)
        {
            if (rat2LocalizedInfo_p->firstCellEdgeFreeIndex <= endVRBIndex)
            {
                if (rat2LocalizedInfo_p->endCellEdgeFreeIndex > endVRBIndex)
                {
                    rat2LocalizedInfo_p->numOfCellEdgeVRBAvailable -= 
                        (endVRBIndex - rat2LocalizedInfo_p->
                        firstCellEdgeFreeIndex + 1);
                    rat2LocalizedInfo_p->firstCellEdgeFreeIndex = endVRBIndex + 1;                 
                }
                else
                {
                    rat2LocalizedInfo_p->numOfCellEdgeVRBAvailable = 0;
                }
            }
        }
        else if (rat2LocalizedInfo_p->endCellEdgeFreeIndex > endVRBIndex)
        {
            /* This assumes that startIndex is greater than 
             * firstCellEdgeFreeIndex,other cases handled in above if condition
             */
            rat2LocalizedInfo_p->numOfCellEdgeVRBAvailable -= 
            (endVRBIndex -startVRBIndex + 1);
        }
        else
        {
            if (rat2LocalizedInfo_p->endCellEdgeFreeIndex > startVRBIndex)
            {
                rat2LocalizedInfo_p->numOfCellEdgeVRBAvailable -= 
                    (rat2LocalizedInfo_p->endCellEdgeFreeIndex - startVRBIndex +
                     1);
                rat2LocalizedInfo_p->endCellEdgeFreeIndex = startVRBIndex - 1;
            }
        }
    }
}
/* ICIC changes end */

/* + CQI_5.0 */
/******************************************************************************
 * Function Name    : allocatePRBsUsingExcessPool
 * Inputs           : mcsIndex - Given MCS Index. It could be subband or wideband,
 *                    availableRBCount -  available RBs for allocation in this subframe 
 *                                        w.r.t. RA policy,
 *                    requiredTBSize -  min/max TBSize,
 *                    allocatedRB - Number of asked PRBs by strategy from DL ResourceAllocator,
 *                    resourceRegionAllocated - CC_CE_REGION or CE_REGION,
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None 
 * Returns          : excessPRBs - extra PRBs required to meet requiredTBSize or close to 
 *                                 requiredTBSize
 * Description      : This function will try to find out extraPRBs required to meet asked TBSize i.e. 
 *                    min/max TBSize from the excessPool.
 ******************************************************************************/
/* ICIC changes start */
STATIC  UInt32 allocatePRBsUsingExcessPool(UInt32 mcsIndex, 
                                                        UInt32 availableRBCount,
                                                        UInt32 requiredTBSize,
                                                        UInt32 allocatedRB,
                                                        UInt8 resourceRegionAllocated,
                                                        InternalCellIndex internalCellIndex)
/* ICIC changes end */
{
    SInt32 rbsForMaxTBSize = 0;
    /* ICIC changes start */
    SInt32 excessPRBs = excessPoolRb_g[internalCellIndex][resourceRegionAllocated];
    /* ICIC changes end */
    UInt32 availableTBSize = determineTBSize(mcsIndex, availableRBCount,
                        /*4X4 DL MIMO CHG START*/
                        TB_MAPPED_TO_ONE_LAYER
                        /*4X4 DL MIMO CHG END*/);
    
    if (availableTBSize >= requiredTBSize)
    {
        /* SPR 5999 changes start */
        rbsForMaxTBSize = DETERMINE_RBs_DL(mcsIndex,requiredTBSize, internalCellIndex) - allocatedRB;
        /* SPR 5999 changes end */
        if (excessPRBs >= rbsForMaxTBSize)
        {
            excessPRBs = rbsForMaxTBSize;
        }
    }
    else
    {
        rbsForMaxTBSize = availableRBCount - allocatedRB;
        if (excessPRBs >= rbsForMaxTBSize)
        {
            excessPRBs = rbsForMaxTBSize;
        }
    }
    
    return excessPRBs;
}


/******************************************************************************
 * Function Name    : allocateRBForMaxTBSize
 * Inputs           : harqProcess_p - Allocated HARQ Process pointer,
 *                    allocatedRb -  Number of asked PRBs by strategy from DL ResourceAllocator,
 *                    availableRBCount -  available RBs for allocation in this subframe wrt RA policy,
 *                    maxTBSize -  Maximum TBSize set by DL Strategy,
 *                    resourceRegionAllocated - CC_CE_REGION or CE_REGION
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None 
 * Returns          : allocatedRb - allocated RBs required to meet maxTBSize
 * Description      : This function finds out total RB required to meet maxTBSize or close to TBSize if  
 *                    maxTBSize can not be meet due to unavailablility of available RBs.
 ******************************************************************************/
/* ICIC changes start */
/* SPR 5599 changes start (ZIP ID 129101) */
UInt32 allocateRBForMaxTBSize(DLHARQProcess *harqProcess_p, 
/* SPR 5599 changes start (ZIP ID 129101) */
                                                  UInt32 allocatedRb, 
                                                  UInt32 availableRBCount,
                                                  UInt32 maxTBSize,
                                                  UInt8 resourceRegionAllocated,
                                                  InternalCellIndex internalCellIndex)
/* ICIC changes end */
{
    UInt32 tbSizeCW1 = 0;
    UInt32 tbSizeCW2 = 0;
    UInt32 tbSize = 0;
 
    if ((TB_VALID == harqProcess_p->isTBOneValid) &&
        (TB_VALID == harqProcess_p->isTBTwoValid))
    {
        /* as excessPool RBs are greater than 0 hence we can store it in unsigned*/
        /* ICIC changes start */
        UInt8 excessPRBs = excessPoolRb_g[internalCellIndex][resourceRegionAllocated];
        /* ICIC changes end */
        

        while (excessPRBs-- && (++allocatedRb <= availableRBCount))
        {   /*4X4 DL MIMO CHG START*/
            if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
            {
                tbSizeCW1 = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                            allocatedRb,
                                            /*4X4 DL MIMO CHG START*/
                                            TB_MAPPED_TO_TWO_LAYER
                                            /*4X4 DL MIMO CHG END*/);
                tbSizeCW2 = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                            allocatedRb,
                                            /*4X4 DL MIMO CHG START*/
                                            TB_MAPPED_TO_TWO_LAYER
                                            /*4X4 DL MIMO CHG END*/);
            }           
            else if(harqProcess_p->numOfLayer == RANK_INDICATOR_3)
            {
                tbSizeCW1 = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                            allocatedRb,
                                            /*4X4 DL MIMO CHG START*/
                                            TB_MAPPED_TO_ONE_LAYER
                                            /*4X4 DL MIMO CHG END*/);
                tbSizeCW2 = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                            allocatedRb,
                                            /*4X4 DL MIMO CHG START*/
                                            TB_MAPPED_TO_TWO_LAYER
                                            /*4X4 DL MIMO CHG END*/);
            }/*4X4 DL MIMO CHG END*/
            else
            {
                tbSizeCW1 = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                            allocatedRb,
                                            /*4X4 DL MIMO CHG START*/
                                            TB_MAPPED_TO_ONE_LAYER
                                            /*4X4 DL MIMO CHG END*/);    
                tbSizeCW2 = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                            allocatedRb,
                                            /*4X4 DL MIMO CHG START*/
                                            TB_MAPPED_TO_ONE_LAYER
                                            /*4X4 DL MIMO CHG END*/);
            }
            tbSize = tbSizeCW1 + tbSizeCW2;

            if (tbSize >= maxTBSize)
            {
                /* SPR 4543 changes start */
                allocatedRb--;              
                /* SPR 4543 changes end */
                break;       
            }
        }

        if (allocatedRb > availableRBCount)
        {
            allocatedRb--;
        }
      		harqProcess_p->assignedResourcesNB = allocatedRb;                       

    }
    else if (TB_VALID == harqProcess_p->isTBOneValid)
    {
        /* In any case TBSize would be set after this function call*/
        /* ICIC changes start */
        allocatedRb += allocatePRBsUsingExcessPool(harqProcess_p->dlHARQTBOneInfo.mcsIndex, 
                                                   availableRBCount,
                                                   maxTBSize,
                                                   allocatedRb,
                                                   resourceRegionAllocated,
                                                   internalCellIndex);
        /* ICIC changes end */
        harqProcess_p->assignedResourcesNB = allocatedRb;
    }
    else
    {
        /* ICIC changes start */
        allocatedRb += allocatePRBsUsingExcessPool(harqProcess_p->dlHARQTBTwoInfo.mcsIndex, 
                                                   availableRBCount,
                                                   maxTBSize,
                                                   allocatedRb,
                                                   resourceRegionAllocated,
                                                   internalCellIndex); 
        /* ICIC changes end */
        harqProcess_p->assignedResourcesNB = allocatedRb;
    }
    return allocatedRb;
}


/* + coverity 42180 */
#ifdef MAC_AUT_TEST
/******************************************************************************
 * Function Name    : allocateRBForMaxTBSizeBothTBValid
 * Inputs           : harqProcess_p - Allocated HARQ Process pointer,
 *                    allocatedRb -  Number of asked PRBs by strategy from DL ResourceAllocator,
 *                    availableRBCount -  available RBs for allocation in this
 *                                        subframe w.r.t. RA policy,
 *                    maxTBSize -  Maximum TBSize set by DL Strategy,
 *                    mcsIndexCW1 - mcsIndex of CodeWord1,
 *                    mcsIndexCW2 - mcsIndex of CodeWord2,
 *                    resourceRegionAllocated - CC_CE_REGION or CE_REGION
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None 
 * Returns          : allocatedRb - allocated RBs required to meet maxTBSize
 * Description      : This function finds out total RB required to meet maxTBSize or close to TBSize if  
 *                    maxTBSize can not be meet due to unavailablility of available RBs.
 ******************************************************************************/
/* ICIC changes start */
STATIC  UInt32 allocateRBForMaxTBSizeBothTBValid(
                                                  DLHARQProcess *harqProcess_p, 
                                                  UInt32 allocatedRb, 
                                                  UInt32 availableRBCount,
                                                  UInt32 maxTBSize,
                                                  UInt32 mcsIndexCW1,
                                                  UInt32 mcsIndexCW2,
                                                  UInt8 resourceRegionAllocated,
                                                  InternalCellIndex internalCellIndex)
/* ICIC changes end */
{  
    UInt32 tbSizeCW1 = 0;
    UInt32 tbSizeCW2 = 0;
    UInt32 tbSize = 0;

    DLHARQTBInfo *dlHarqTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    DLHARQTBInfo *dlHarqTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo;    
 
   
    /* ICIC changes start */
    UInt8 excessPRBs = excessPoolRb_g[internalCellIndex][resourceRegionAllocated];
    /* ICIC changes end */

    while (excessPRBs-- && (++allocatedRb <= availableRBCount))
    {   
        /*4X4 DL MIMO CHG START */
        if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
        {
            tbSizeCW1 = determineTBSize(mcsIndexCW1, allocatedRb,
                                   TB_MAPPED_TO_TWO_LAYER);

            tbSizeCW2 = determineTBSize(mcsIndexCW2, allocatedRb,
                                   TB_MAPPED_TO_TWO_LAYER);
        }
        else if (harqProcess_p->numOfLayer == RANK_INDICATOR_3)
        {
            tbSizeCW1 = determineTBSize(mcsIndexCW1, allocatedRb,
                                   TB_MAPPED_TO_ONE_LAYER);
            tbSizeCW2 = determineTBSize(mcsIndexCW2, allocatedRb,
                                   TB_MAPPED_TO_TWO_LAYER);
        }
        else
        {
            tbSizeCW1 = determineTBSize(mcsIndexCW1, allocatedRb,
                                   TB_MAPPED_TO_ONE_LAYER);
            tbSizeCW2 = determineTBSize(mcsIndexCW2, allocatedRb,
                                   TB_MAPPED_TO_ONE_LAYER);
        }
        /*4X4 DL MIMO CHG END */
        tbSize = tbSizeCW1 + tbSizeCW2;

        if (tbSize >= maxTBSize)
        {
            /* SPR 4543 changes start */
            allocatedRb--;              
            /* SPR 4543 changes end */
            break;       
        }
                         
    }

    if (allocatedRb > availableRBCount)
    {
        allocatedRb--;
                       
    }

    dlHarqTBOneInfo_p->tbSize = tbSizeCW1;
    dlHarqTBOneInfo_p->mcsIndex = mcsIndexCW1;
    dlHarqTBTwoInfo_p->tbSize = tbSizeCW2;
    dlHarqTBTwoInfo_p->mcsIndex = mcsIndexCW2;
    harqProcess_p->assignedResourcesNB = allocatedRb; 

    
    return allocatedRb;
}
#endif
/* + coverity 42180 */

/******************************************************************************
 * Function Name      : reduceRBForMaxTBSizeBothTBValid
 * Inputs             : harqProcess_p - Allocated HARQ Process pointer,
 *                      allocatedRb -  Number of asked PRBs by strategy from DL ResourceAllocator,
 *                      maxTBSize -  Maximum TBSize set by DL Strategy,
 *                      mcsIndexCW1 - mcsIndex of CodeWord1,
 *                      mcsIndexCW2 - mcsIndex of CodeWord2,
 *                      tbSizeCW1 - Transport block size of Codeword 1,
 *                      tbSizeCW2 - Transport block size of Codeword 2
 * Outputs            : None 
 * Returns            : allocated RBs required to meet maxTBSize
 * Description        : This function decrements allocated RB to meet close to maxTBSize.
 ******************************************************************************/
STATIC  UInt32 reduceRBForMaxTBSizeBothTBValid(
                                DLHARQProcess *harqProcess_p,
                                UInt32 allocatedRB,
                                UInt32 maxTBSize,
                                UInt32 mcsIndexCW1,
                                UInt32 mcsIndexCW2,
                                UInt32 tbSizeCW1,
                                UInt32 tbSizeCW2)
{
    DLHARQTBInfo *dlHarqTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    DLHARQTBInfo *dlHarqTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo;

    allocatedRB--;                
    UInt32 tbSize = tbSizeCW1 + tbSizeCW2; 
    
    while ((tbSize >= maxTBSize) && allocatedRB)
    {
        /*4X4 DL MIMO CHG START */
        if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
        {
            tbSizeCW1 = determineTBSize(mcsIndexCW1, allocatedRB,
                                   TB_MAPPED_TO_TWO_LAYER);
                                   
            tbSizeCW2 = determineTBSize(mcsIndexCW2, allocatedRB,
                                   TB_MAPPED_TO_TWO_LAYER);
        }
        else if (harqProcess_p->numOfLayer == RANK_INDICATOR_3)
        {   
            tbSizeCW1 = determineTBSize(mcsIndexCW1, allocatedRB,
                                   TB_MAPPED_TO_ONE_LAYER);
            tbSizeCW2 = determineTBSize(mcsIndexCW2, allocatedRB,
                                   TB_MAPPED_TO_TWO_LAYER);
        }
        else
        {
            tbSizeCW1 = determineTBSize(mcsIndexCW1, allocatedRB,
                                   TB_MAPPED_TO_ONE_LAYER);
            tbSizeCW2 = determineTBSize(mcsIndexCW2, allocatedRB,
                                   TB_MAPPED_TO_ONE_LAYER);
        }
        /*4X4 DL MIMO CHG END */
        tbSize = tbSizeCW1 + tbSizeCW2;

        if (tbSize <= maxTBSize)
        {
            //now required TB size is achieved hence fill these details in the HARQ
		    allocatedRB++;
            dlHarqTBOneInfo_p->tbSize = tbSizeCW1;
            dlHarqTBTwoInfo_p->tbSize = tbSizeCW2;
            
            dlHarqTBOneInfo_p->mcsIndex = mcsIndexCW1;
            dlHarqTBTwoInfo_p->mcsIndex = mcsIndexCW2;
            harqProcess_p->assignedResourcesNB = allocatedRB;
            break;       
        }
        else
        {
            allocatedRB--;                        
        }
    }

    if (0 == allocatedRB)
    {
        allocatedRB++;
        dlHarqTBOneInfo_p->tbSize = tbSizeCW1;
        dlHarqTBTwoInfo_p->tbSize = tbSizeCW2;
        
        dlHarqTBOneInfo_p->mcsIndex = mcsIndexCW1;
        dlHarqTBTwoInfo_p->mcsIndex = mcsIndexCW2;
        harqProcess_p->assignedResourcesNB = allocatedRB;                   
    }

    return allocatedRB;
}

/*****************************************************************************
 * Function Name    : determineFreqSelectiveTBSize
 * Inputs           : numOfLayer - This Holds the number of layers for 
 *                                 transmission.
 *                    freqSelectiveMCSIndexCW1 - MCS for CW1
 *                    freqSelectiveMCSIndexCW2 - MCS for CW2
 *                    requiredRBCount - Number of RBs required
 * Outputs          : freqSelectiveTBSizeCW1 - TB size for CW1
 *                    freqSelectiveTBSizeCW2 - TB size for CW2
 * Returns          : Total TB Size
 * Description      : This funtion will return the total TB size for both 
 *                    Codewords from MCS and RB depending on number of layers.
 *****************************************************************************/
UInt32 determineFreqSelectiveTBSize( UInt8 numOfLayer,
      UInt32 freqSelectiveMCSIndexCW1, UInt32 freqSelectiveMCSIndexCW2,
      UInt32 requiredRBCount, UInt32 *freqSelectiveTBSizeCW1_p,
      UInt32 *freqSelectiveTBSizeCW2_p )
{
    UInt32 freqSelectiveTBSize = 0;

    if( numOfLayer == RANK_INDICATOR_4)
    {
        *freqSelectiveTBSizeCW1_p = determineTBSize(freqSelectiveMCSIndexCW1,
                requiredRBCount,
                TB_MAPPED_TO_TWO_LAYER );
        *freqSelectiveTBSizeCW2_p = determineTBSize(freqSelectiveMCSIndexCW2,
                requiredRBCount,
                TB_MAPPED_TO_TWO_LAYER );
    }
    else if( numOfLayer == RANK_INDICATOR_3)
    {
        *freqSelectiveTBSizeCW1_p = determineTBSize(freqSelectiveMCSIndexCW1,
                requiredRBCount,
                TB_MAPPED_TO_ONE_LAYER );
        *freqSelectiveTBSizeCW2_p = determineTBSize(freqSelectiveMCSIndexCW2,
                requiredRBCount,
                TB_MAPPED_TO_TWO_LAYER );
    }
    else 
    {
        *freqSelectiveTBSizeCW1_p = determineTBSize(freqSelectiveMCSIndexCW1,
                requiredRBCount,
                TB_MAPPED_TO_ONE_LAYER );
        *freqSelectiveTBSizeCW2_p = determineTBSize(freqSelectiveMCSIndexCW2,
                requiredRBCount,
                TB_MAPPED_TO_ONE_LAYER );
    }
    freqSelectiveTBSize = 
        *freqSelectiveTBSizeCW1_p + *freqSelectiveTBSizeCW2_p;

    return freqSelectiveTBSize;
}

/******************************************************************************
 * Function Name    : validateRAT1RBsforMinMaxTBSize
 * Inputs           : rbgBitMap - This Holds  the BitMap where each set bit
 *                                represents a Free RBG and bit postion represents RBG Index,
 *                    resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                    harqProcess_p - Pointer to DLHARQProcess,
 *                    previousRsrcFrmt_p - Pointer to ResourceFormatMIMONode,
 *                    dlStrategyNode_p - Pointer to DLStrategyTxNode,
 *                    bitPositionToSubsetAndRBGInfo_p - Pointer to 
 *                                                      BitPositionToSubsetAndRBGInfo,
 *                    subsetIndex - Index of Subset,
 *                    isNonZeroSpan
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will check if the available RB Count is greater 
 *                    than required RB Count .If true it will populate the relevant
 *                    information in harqProcess_p structure.It will also check 
 *                    against HARQ_TB_MINIMUM_SIZE_IN_BYTES for CodeWord One 
 *                    and CodeWordTwo.
 ******************************************************************************/ 
STATIC MacRetType validateRAT1RBsforMinMaxTBSize(
        UInt32 rbBitMap,
        ResourceAllocatorInput *resrcAllocatorInput_p,
        DLHARQProcess  *harqProcess_p,
        ResourceFormatMIMONode *previousRsrcFrmt_p,
        DLStrategyTxNode *dlStrategyNode_p,
        BitPositionToSubsetAndRBGInfo *bitPositionToSubsetAndRBGInfo_p,
        UInt32 subsetIndex,
        UInt32 isNonZeroSpan,
        InternalCellIndex internalCellIndex)
{
    UInt32 availableRBCount = 0;
    UInt32 allocatedRB = 0;
    DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
    DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL;

    UInt32 freqSelectiveMCSIndexCW1 = 0;
    UInt32 freqSelectiveMCSIndexCW2 = 0;

    UInt32 freqSelectiveTBSize = 0;
    UInt32 freqSelectiveTBSizeCW1 = 0;
    UInt32 freqSelectiveTBSizeCW2 = 0;

    SubBandReportInfo *subBandReportInfo_p = resrcAllocatorInput_p->subBandReportInfo_p;
    UInt32 requiredRBCount = resrcAllocatorInput_p->requiredRB; 

    UInt32 minTBSize = dlStrategyNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyNode_p->maxTBSize; 
    UInt8 excessPRBs = 0;
    /* ICIC changes start */
    UInt8 resourceRegionAllocated = resrcAllocatorInput_p->resourceRegionAllocated;
    /* ICIC changes end */
    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *ueContext_p = PNULL;
    dlUEContextInfo_p = &dlUECtxInfoArr_g[dlStrategyNode_p->ueIndex];
    ueContext_p = dlUEContextInfo_p->dlUEContext_p;
#endif
    /* HD FDD Changes End */
    if (maxTBSize < minTBSize)
    {
        lteWarning("[RAT1 Spatial] Wrong Max TBSize calculated by DL Strategy\
                minTBSize [%d] maxTBSize [%d]\n", minTBSize, maxTBSize);
        maxTBSize = minTBSize;
    }       

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo;

    /*Get the count of number of Available RBs*/
    availableRBCount = getCount(rbBitMap);

    if (availableRBCount >= requiredRBCount)
    {
        allocatedRB = requiredRBCount;

        /*Calling funtion getMcsIndexForCW1AndCW2 which will populate 
         * the Mcs index of CodeWord One and CodeWord Two in 
         *mcsIndexCW1 and mcsIndexCW2*/
        getMcsIndexForCW1AndCW2RAT1(&freqSelectiveMCSIndexCW1,
                &freqSelectiveMCSIndexCW2,
                rbBitMap,
                subBandReportInfo_p, 
                bitPositionToSubsetAndRBGInfo_p,
                requiredRBCount);
        /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
        HD_GET_REDUCED_MCS_CW1_CW2_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndexCW1, freqSelectiveMCSIndexCW2);
#endif
        /* Determine Frequency selective TB size */ 
        freqSelectiveTBSize = determineFreqSelectiveTBSize( 
                harqProcess_p->numOfLayer, 
                freqSelectiveMCSIndexCW1, freqSelectiveMCSIndexCW2, 
                requiredRBCount, &freqSelectiveTBSizeCW1, 
                &freqSelectiveTBSizeCW2 ); 

        /*If TB size on this mcs with requiredRBCount is greater than minimum TB size then 
          we can go for maximum TB size too if excess Pool has some RBs left. This case means successful as 
          minimum TB size is always met*/         
        if (freqSelectiveTBSize >= minTBSize)
        {
            if (freqSelectiveTBSize >= maxTBSize)
            {
                allocatedRB = reduceRBForMaxTBSizeBothTBValid(
                        harqProcess_p,
                        allocatedRB,
                        maxTBSize,
                        freqSelectiveMCSIndexCW1,
                        freqSelectiveMCSIndexCW2,
                        freqSelectiveTBSizeCW1,
                        freqSelectiveTBSizeCW2);
            }
            dlHARQTBOneInfo_p->tbSize = freqSelectiveTBSizeCW1;
            dlHARQTBTwoInfo_p->tbSize = freqSelectiveTBSizeCW2;

            dlHARQTBOneInfo_p->mcsIndex = freqSelectiveMCSIndexCW1;
            dlHARQTBTwoInfo_p->mcsIndex = freqSelectiveMCSIndexCW2;                

            harqProcess_p->assignedResourcesNB = allocatedRB;

            //update excess PRBs in buffer pool
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated,
                    (requiredRBCount - harqProcess_p->assignedResourcesNB),
                    internalCellIndex);
            /* ICIC changes end */
            return MAC_SUCCESS;
        }
        else
        {
            /*Here allocation can be done only if some RBs are left in excessPool which allows TBSize to be greater
              than or equal to minimum TB Size. If not possible then store this TBSize and found RBs as later on it will
              required to allocate if strict subband allocation is required*/

            if (excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0)
            {

                /*Calling funtion getMcsIndexForCW1AndCW2 which will populate 
                 * the Mcs index of CodeWord One and CodeWord Two in 
                 *mcsIndexCW1 and mcsIndexCW2*/
                getMcsIndexForCW1AndCW2RAT1(&freqSelectiveMCSIndexCW1,
                        &freqSelectiveMCSIndexCW2,
                        rbBitMap,
                        subBandReportInfo_p, 
                        bitPositionToSubsetAndRBGInfo_p,
                        availableRBCount);
                /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                HD_GET_REDUCED_MCS_CW1_CW2_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndexCW1, freqSelectiveMCSIndexCW2);
#endif
                /* HD FDD Changes End */

                excessPRBs = excessPoolRb_g[internalCellIndex][resourceRegionAllocated]; 
                while ( (excessPRBs--) && (++allocatedRB <= availableRBCount)) 
                {
                    /*4x4 DL MIMO CHG START*/
                    if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
                    {
                        freqSelectiveTBSizeCW1 = determineTBSize(freqSelectiveMCSIndexCW1,
                                requiredRBCount,
                                /*4x4 DL MIMO CHG START*/
                                TB_MAPPED_TO_TWO_LAYER
                                /*4x4 DL MIMO CHG END*/);
                        freqSelectiveTBSizeCW2 = determineTBSize(freqSelectiveMCSIndexCW2,
                                requiredRBCount,
                                /*4x4 DL MIMO CHG START*/
                                TB_MAPPED_TO_TWO_LAYER
                                /*4x4 DL MIMO CHG END*/);
                    }
                    else if(harqProcess_p->numOfLayer == RANK_INDICATOR_3)
                    {
                        freqSelectiveTBSizeCW1 = determineTBSize(freqSelectiveMCSIndexCW1,
                                requiredRBCount,
                                /*4x4 DL MIMO CHG START*/
                                TB_MAPPED_TO_ONE_LAYER
                                /*4x4 DL MIMO CHG END*/);
                        freqSelectiveTBSizeCW2 = determineTBSize(freqSelectiveMCSIndexCW2,
                                requiredRBCount,
                                /*4x4 DL MIMO CHG START*/
                                TB_MAPPED_TO_TWO_LAYER
                                /*4x4 DL MIMO CHG END*/);
                    }/*4x4 DL MIMO CHG END*/
                    else
                    { 
                        freqSelectiveTBSizeCW1 = determineTBSize(freqSelectiveMCSIndexCW1, allocatedRB,
                                /*4x4 DL MIMO CHG START*/
                                TB_MAPPED_TO_ONE_LAYER
                                /*4x4 DL MIMO CHG END*/); 
                        freqSelectiveTBSizeCW2 = determineTBSize(freqSelectiveMCSIndexCW2, allocatedRB,
                                /*4x4 DL MIMO CHG START*/
                                TB_MAPPED_TO_ONE_LAYER
                                /*4x4 DL MIMO CHG END*/);
                    }
                    freqSelectiveTBSize = freqSelectiveTBSizeCW1 + freqSelectiveTBSizeCW2;

                    if (freqSelectiveTBSize >= minTBSize)
                    {
                        //As now minTBSize is reached hence populate HARQ fields
                        dlHARQTBOneInfo_p->tbSize = freqSelectiveTBSizeCW1;
                        dlHARQTBTwoInfo_p->tbSize = freqSelectiveTBSizeCW2;

                        dlHARQTBOneInfo_p->mcsIndex = freqSelectiveMCSIndexCW1;
                        dlHARQTBTwoInfo_p->mcsIndex = freqSelectiveMCSIndexCW2;
                        harqProcess_p->assignedResourcesNB = allocatedRB;

                        //update excess PRBs in buffer pool
                        /* ICIC changes start */
                        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated,
                                (requiredRBCount - harqProcess_p->assignedResourcesNB),
                                internalCellIndex);
                        /* ICIC changes end */
                        return MAC_SUCCESS;       
                    }
                }
                //as allocatedRB count is greater than availableRBCount hence decrement it before storing in previousRsrcFrmt
                if(allocatedRB > availableRBCount)
                {
                    allocatedRB--;
                }  
            }

            if(previousRsrcFrmt_p->tbSize < freqSelectiveTBSize)
            {
                previousRsrcFrmt_p->tbSize = freqSelectiveTBSize;
                previousRsrcFrmt_p->tbSizeCW1 = freqSelectiveTBSizeCW1;
                previousRsrcFrmt_p->tbSizeCW2 = freqSelectiveTBSizeCW2;
                previousRsrcFrmt_p->mcsIndexCW1 = freqSelectiveMCSIndexCW1;
                previousRsrcFrmt_p->mcsIndexCW2 = freqSelectiveMCSIndexCW2;
                previousRsrcFrmt_p->prbs = allocatedRB;
                previousRsrcFrmt_p->cqiRBGBitmap = rbBitMap;
                previousRsrcFrmt_p->pmiIndex = harqProcess_p->pmiIndex;
                previousRsrcFrmt_p->subsetIndex = subsetIndex;
                previousRsrcFrmt_p->isNonZeroSpan = isNonZeroSpan;
            }               
        } 
    }
    else 
    {
        /*Calling funtion getMcsIndexForCW1AndCW2 which will populate 
         * the Mcs index of CodeWord One and CodeWord Two in 
         *mcsIndexCW1 and mcsIndexCW2*/
        allocatedRB = availableRBCount;               
        getMcsIndexForCW1AndCW2RAT1(&freqSelectiveMCSIndexCW1,
                &freqSelectiveMCSIndexCW2,
                rbBitMap,
                subBandReportInfo_p, 
                bitPositionToSubsetAndRBGInfo_p,
                availableRBCount);
        /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
        HD_GET_REDUCED_MCS_CW1_CW2_DL_UL_SWITCH(currentTick, ueContext_p, freqSelectiveMCSIndexCW1, freqSelectiveMCSIndexCW2);
#endif
        /* HD FDD Changes End */

        /* Here although available RBs are less than required RBs but still allocation can be done as
           reported MCS can be greater than wideband MCS used by strategy hence min/max TB size can be
           achieved with available RBs*/
        freqSelectiveTBSizeCW1 = determineTBSize(freqSelectiveMCSIndexCW1, availableRBCount,
                /*4x4 DL MIMO CHG START*/
                TB_MAPPED_TO_ONE_LAYER
                /*4x4 DL MIMO CHG END*/);
        freqSelectiveTBSizeCW2 = determineTBSize(freqSelectiveMCSIndexCW2, availableRBCount,
                /*4x4 DL MIMO CHG START*/
                TB_MAPPED_TO_ONE_LAYER
                /*4x4 DL MIMO CHG END*/);

        freqSelectiveTBSize = freqSelectiveTBSizeCW1 + freqSelectiveTBSizeCW2;      

        if (freqSelectiveTBSize >= minTBSize)
        {
            if (freqSelectiveTBSize >= maxTBSize)
            {
                allocatedRB = reduceRBForMaxTBSizeBothTBValid(
                        harqProcess_p,
                        allocatedRB,
                        maxTBSize,
                        freqSelectiveMCSIndexCW1,
                        freqSelectiveMCSIndexCW2,
                        freqSelectiveTBSizeCW1,
                        freqSelectiveTBSizeCW2);            
            }

            dlHARQTBOneInfo_p->mcsIndex = freqSelectiveMCSIndexCW1;
            dlHARQTBTwoInfo_p->mcsIndex = freqSelectiveMCSIndexCW2;
            harqProcess_p->assignedResourcesNB = allocatedRB;

            //update excess PRBs in buffer pool
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated,
                    (requiredRBCount - harqProcess_p->assignedResourcesNB),
                    internalCellIndex);
            /* ICIC changes end */
            return MAC_SUCCESS;
        }
        else
        {
            if(previousRsrcFrmt_p->tbSize < freqSelectiveTBSize)
            {
                previousRsrcFrmt_p->tbSize = freqSelectiveTBSize;
                previousRsrcFrmt_p->tbSizeCW1 = freqSelectiveTBSizeCW1;
                previousRsrcFrmt_p->tbSizeCW2 = freqSelectiveTBSizeCW2;
                previousRsrcFrmt_p->mcsIndexCW1 = freqSelectiveMCSIndexCW1;
                previousRsrcFrmt_p->mcsIndexCW2 = freqSelectiveMCSIndexCW2;
                previousRsrcFrmt_p->prbs = allocatedRB;
                previousRsrcFrmt_p->cqiRBGBitmap = rbBitMap;
                previousRsrcFrmt_p->pmiIndex = harqProcess_p->pmiIndex;
                previousRsrcFrmt_p->subsetIndex = subsetIndex;
                previousRsrcFrmt_p->isNonZeroSpan = isNonZeroSpan;
            }
        }  
    }

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s with Failure\n",__func__);
    harqProcess_p->pmiIndex = WIDEBAND_PMI;
    return MAC_FAILURE; 
}


/******************************************************************************
 * Function Name    : getReportedLocalizedVRBNewTx
 * Inputs           : rat2LocalizedInfo_p - This holds the pointer to RAT2 Localized structure,
 *                    minRBs - VRBs required for minTBSize allocation,
 *                    maxRBs - VRBs required for maxTBSize allocation,
 *                    reportedRBGBitmap - reported subband bitmap,
 *                    resourceRegionAllocated - CC_CE_REGION or CE_REGION
 * Outputs          : bestVRBSpanInfo_p with start and endVRB position in case of MAC_SUCCESS
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will check if both VRB is available and reported then it will 
 *                    start counting these VRBs which are contiguous until minRBs are not reached.
 *                    In case minRBs are reached then based on excessPool it tries to reach close to 
 *                    maxRBs.
 ******************************************************************************/
/* ICIC changes start */
/* SPR 7435 Changes Starts */
MacRetType getReportedLocalizedVRBNewTx(                                                                                                
/* SPR 7435 Changes Ends */
                                                BestVRBSpanInfo *bestVRBSpanInfo_p, 
                                                RAT2LocalizedInfo *rat2LocalizedInfo_p,
                                                UInt32 minRBs,
                                                UInt32 maxRBs,
                                                UInt32 reportedRBGBitmap,
                                                UInt8 resourceRegionAllocated)
/* ICIC changes end */
{
    UInt32 currentFreeVRBIndex= 0;

    UInt32 maxVRBLocalized = 0;
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;
    UInt32 vrbToRBGIndex = 0;
    UInt8 nextFreeVRBIndex = 0;
    UInt8 isContiguousSubband = 0;
    UInt8 extraRequiredSubbandRBs = 0;
    UInt8 availableSubbandRBCount = 1;
    UInt8 lastFoundContSubbandVRB = 0;
    UInt8 lastFoundSubbandRBCount = 1;
    UInt8 tempExtraRequiredRBCount = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);
    localizedVRBInfo_p = rat2LocalizedInfo_p->rat2LocalizedVRBInfo;

    /* ICIC changes start */
    if (CC_CE_REGION == resourceRegionAllocated)
    {
        maxVRBLocalized = rat2LocalizedInfo_p->maxLocalizedVRB;

        /*Initialize the currentFreeVRBIndex with the first free VRB index */
        currentFreeVRBIndex = localizedVRBInfo_p[0].nextfreeVRBIndex;
    }
    else
    {
        maxVRBLocalized = rat2LocalizedInfo_p->endCellEdgeFreeIndex;
        currentFreeVRBIndex = rat2LocalizedInfo_p->firstCellEdgeFreeIndex;
    }
    /* ICIC changes end */

    /* SPR 13921 Changes Starts */
    if((MAX_LOCALIZED_VRB + 3) <= currentFreeVRBIndex)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,
                "[%s]:currentFreeVRBIndex  = %d so Exiting. \n"
                ,__func__, currentFreeVRBIndex);
        return MAC_FAILURE;

    }
    /* SPR 13921 Changes Ends */
      
    /* For subband max TB Size*/
    extraRequiredSubbandRBs = maxRBs - minRBs;

    
    if(minRBs > 1)
    {
    //+1 due to == of minRBs
      extraRequiredSubbandRBs++;
    }
    tempExtraRequiredRBCount = extraRequiredSubbandRBs; 
    
    /* Below While loop is iterating through each Free VRB Index and checking
         * if the next Free VRB index and current Free VRB Index is consecutive 
         * and both reported or not .Now if consecutive ,it will populate the bestSpanInfo[0] with
         * relevant information and then again repeat the logic .If not 
         * consecutive ,it will store the information of best continous VRB found
         * till now in bestSpanInfo[1] and then again repeat the logic.At The end
         * it will compare the continous VRB information stored in bestSpanInfo[0]
         * and bestSpan[1]Info  and return  the largest continous VRB */
     
    maxVRBLocalized += 1;
    localizedVRBInfo_p = localizedVRBInfo_p + currentFreeVRBIndex;
    /* SPR 13921 Changes Starts */ 
    while ( (currentFreeVRBIndex < maxVRBLocalized) && (currentFreeVRBIndex < (MAX_LOCALIZED_VRB + 3)))
    {
        /* SPR 13921 Changes Ends */ 
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "currentFreeVRBIndex = %u\n nextfreeVRBIndex = %u\n",
                currentFreeVRBIndex,localizedVRBInfo_p->nextfreeVRBIndex);

        vrbToRBGIndex = ~(localizedVRBInfo_p->rbgIndexBitMap);
        nextFreeVRBIndex = localizedVRBInfo_p->nextfreeVRBIndex;

        /*check whether current VRB is reported or not*/
        if (vrbToRBGIndex & reportedRBGBitmap)
        {
            /*update this reported and available VRB into foundSubbandVRB*/
            lastFoundContSubbandVRB = currentFreeVRBIndex;                    
            
            /* Check if the currentFreeIndex and nextFreeIndex are consecutive or not */
            if ( !(nextFreeVRBIndex - (currentFreeVRBIndex + 1))) 
            {
                  /* check nextFreeVRB is reported too*/
                    vrbToRBGIndex = ~(rat2LocalizedInfo_p->rat2LocalizedVRBInfo[nextFreeVRBIndex].rbgIndexBitMap);
                    if (vrbToRBGIndex & reportedRBGBitmap)
                    {
                        /*this means both current and next are contiguous*/
                        isContiguousSubband = 1;
                    }           
            }
        }
    
        if (isContiguousSubband)
        {
            availableSubbandRBCount += 1;     

            if (availableSubbandRBCount >= minRBs)
            {
                /*now try to reach maxRBs */            
                extraRequiredSubbandRBs--;
                if ((extraRequiredSubbandRBs > 0) && ((currentFreeVRBIndex + 1) < maxVRBLocalized))
                {
                    /* Increment the currentFreeIndex so that now it points to next free VRB Index */
                    currentFreeVRBIndex++;  
                    localizedVRBInfo_p++; 
                    isContiguousSubband = 0;
                    continue;                    
                }
                else
                {   
                    /* -2 due to (+1 for current and +1 for next) */
                    bestVRBSpanInfo_p->startingVRBIndex = 
                                  currentFreeVRBIndex - (availableSubbandRBCount - 2);
                    bestVRBSpanInfo_p->endingVRBIndex = currentFreeVRBIndex + 1;
                    return MAC_SUCCESS;
                }
            }           
            /* Increment the currentFreeIndex so that now it points to next free VRB Index */
            currentFreeVRBIndex++;  
            localizedVRBInfo_p++;
            isContiguousSubband = 0;
        }                    
        else
        {
            /*check whether we have largest available and reported set if not then update it */
            if (availableSubbandRBCount > lastFoundSubbandRBCount) 
            {
                lastFoundSubbandRBCount = availableSubbandRBCount;
                bestVRBSpanInfo_p->startingVRBIndex = currentFreeVRBIndex - (availableSubbandRBCount - 1);
                bestVRBSpanInfo_p->endingVRBIndex =   currentFreeVRBIndex ;
 
            }               
      
            availableSubbandRBCount = 1;
                                                             
            currentFreeVRBIndex = localizedVRBInfo_p->nextfreeVRBIndex;                    
            localizedVRBInfo_p = &rat2LocalizedInfo_p->rat2LocalizedVRBInfo[currentFreeVRBIndex];            
            extraRequiredSubbandRBs = tempExtraRequiredRBCount;
        }
    }   

    if ( availableSubbandRBCount >= lastFoundSubbandRBCount) 
    {        
        if (!lastFoundContSubbandVRB)
        {
            /*this indicates contiguous & reported VRBs are not found*/
            return MAC_FAILURE;
        }
        else if (1 == availableSubbandRBCount)
        {
            /*this is case when only one reported VRB was found*/
            bestVRBSpanInfo_p->startingVRBIndex = lastFoundContSubbandVRB;
            bestVRBSpanInfo_p->endingVRBIndex = lastFoundContSubbandVRB;
        }
        else
        {
            bestVRBSpanInfo_p->startingVRBIndex = 
                           (currentFreeVRBIndex - 1) - (availableSubbandRBCount - 2);
            bestVRBSpanInfo_p->endingVRBIndex =   currentFreeVRBIndex - 1;
        }
    }

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Leaving funtion = %s\n",__func__);
    return MAC_SUCCESS;

}

/******************************************************************************
 * Function Name    : getFirstAvailableReportedVRBNewTx
 * Inputs           : rat2LocalizedInfo_p - This holds the pointer to RAT2 Localized structure,
 *                    reportedRBGBitmap - reported subband bitmap,
 *                    resourceRegionAllocated - CC_CE_REGION or CE_REGION
 * Outputs          : bestVRBSpanInfo_p with start and endVRB position in case of MAC_SUCCESS
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtion will check if current VRB is available and reported then it will 
 *                    return Success otherwise continue to find a VRB which is available and both
 *                    reported.
 ******************************************************************************/
/* ICIC changes start */
 MacRetType getFirstAvailableReportedVRBNewTx(
                                                BestVRBSpanInfo *bestVRBSpanInfo_p, 
                                                RAT2LocalizedInfo *rat2LocalizedInfo_p,
                                                UInt32 reportedRBGBitmap,
                                                UInt8 resourceRegionAllocated)
/* ICIC changes end */
{
    UInt32 currentFreeVRBIndex= 0;

    UInt32 maxVRBLocalized = 0;
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;
    UInt32 vrbToRBGIndex = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s",__func__);
    localizedVRBInfo_p = rat2LocalizedInfo_p->rat2LocalizedVRBInfo;

    /* ICIC changes start */
    if (CC_CE_REGION == resourceRegionAllocated)
    {
        maxVRBLocalized = rat2LocalizedInfo_p->maxLocalizedVRB;

        /*Initialize the currentFreeVRBIndex with the first free VRB index */
        currentFreeVRBIndex = localizedVRBInfo_p[0].nextfreeVRBIndex;
    }
    else
    {
        maxVRBLocalized = rat2LocalizedInfo_p->endCellEdgeFreeIndex;
        currentFreeVRBIndex = rat2LocalizedInfo_p->firstCellEdgeFreeIndex;
    }
    /* ICIC changes end */

    /* SPR 13921 Changes Starts */
    if((MAX_LOCALIZED_VRB + 3) <= currentFreeVRBIndex)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DL_RESOURCE_MGR,
                "[%s]:currentFreeVRBIndex  = %d so Exiting. \n"
                ,__func__, currentFreeVRBIndex);
        return MAC_FAILURE;

    }
    /* SPR 13921 Changes Ends */
      
    /* Below While loop is iterating through each Free VRB Index and checking 
         * whether it is reported or not . If yes then it will populate the bestSpanInfo with
         * relevant information*/
     
    maxVRBLocalized += 1;
    localizedVRBInfo_p = localizedVRBInfo_p + currentFreeVRBIndex;
    /* SPR 13921 Changes Starts */ 
    while ((currentFreeVRBIndex < maxVRBLocalized)&& (currentFreeVRBIndex < (MAX_LOCALIZED_VRB + 3)))
    {
        /* SPR 13921 Changes Ends */ 
        LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "currentFreeVRBIndex = %u\n nextfreeVRBIndex = %u\n",
                currentFreeVRBIndex,localizedVRBInfo_p->nextfreeVRBIndex);

        vrbToRBGIndex = ~(localizedVRBInfo_p->rbgIndexBitMap);

        /*check whether current VRB is reported or not*/
        if (vrbToRBGIndex & reportedRBGBitmap)
        {
            bestVRBSpanInfo_p->startingVRBIndex = currentFreeVRBIndex;
            bestVRBSpanInfo_p->endingVRBIndex = currentFreeVRBIndex;
            return MAC_SUCCESS;                        
        }
        
        /* Increment the currentFreeIndex so that now it points to next free VRB Index */
        currentFreeVRBIndex = localizedVRBInfo_p->nextfreeVRBIndex;                    
        localizedVRBInfo_p = &rat2LocalizedInfo_p->rat2LocalizedVRBInfo[currentFreeVRBIndex];            
    }
    return MAC_FAILURE;
}

/******************************************************************************
 * Function Name     : selectRAT0SubbandPMIIndexRank2
 * Inputs            : bitMapToBeReturned - This holds the RBGs which are reported and available too
 *                     RAT0PmiInfo - reported RBG bitmap per each PMI,
 *                     internalCellIndex -  Cell-Index at MAC
 * Outputs           : bitMapToUse_p - contains bitmap of reported RBG on selected PMI
 * Returns           : pmiIndex - selected PMI Index
 * Description       : This funtion will check which PMI Index has maximum number of reported PRBs
 *                     and based on this give the available & reported RBG Bitmap on this PMI index.
 ******************************************************************************/
STATIC  UInt32 selectRAT0SubbandPMIIndexRank2(
                                                UInt32 bitMapToBeReturned, 
                                                RAT0PmiInfo *rat0PmiInfo_p,
                                                UInt32 *bitMapToUse_p,
                                                InternalCellIndex internalCellIndex
                                                )
{
    UInt32 pmiIndex = 0;

    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;

    if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
    {
        /*store the number of reported RBG which are available with same PMI index*/
        UInt32 pmiReportedSubband_1 = 
            bitMapToBeReturned & rat0PmiInfo_p->reportedRBGRank2[0];

        UInt32 pmiReportedSubband_2 = 
            bitMapToBeReturned & rat0PmiInfo_p->reportedRBGRank2[1];

        if (getCount(pmiReportedSubband_1) >= getCount(pmiReportedSubband_2))
        {
            pmiIndex = 0;
            *bitMapToUse_p = pmiReportedSubband_1;
        }
        else
        {
            pmiIndex = 1;
            *bitMapToUse_p = pmiReportedSubband_2;
        }

        if (0 == *bitMapToUse_p)
        {
            *bitMapToUse_p = bitMapToBeReturned;
            /* setting WIDEBAND_PMI is just a indication that here wideband
             * PMI has to be used alongwith resource allocation.
             * This is not the actual wideband PMI value reported by UE*/
            pmiIndex = WIDEBAND_PMI;
        }
    }
    else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        pmiIndex = selectRATSubbandPMIIndexPort4( bitMapToUse_p,
                                   rat0PmiInfo_p->reportedRBGPMIForPort4 );
    }    
    return pmiIndex;
}

/******************************************************************************
 * Function Name    : selectRAT0SubbandPMIIndexRank1
 * Inputs           : bitMapToBeReturned - This holds the RBGs which are reported and available too,
 *                    rat0PmiInfo_p - reported RBG bitmap per each PMI
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : bitMapToUse_p - contains bitmap of reported RBG on selected PMI
 * Returns          : pmiIndex - selected PMI Index
 * Description      : This funtion will check which PMI Index has maximum number of reported PRBs
 *                    and based on this give the available & reported RBG Bitmap on this PMI index.
 ******************************************************************************/
STATIC  UInt32 selectRAT0SubbandPMIIndexRank1(
                                                UInt32 bitMapToBeReturned, 
                                                RAT0PmiInfo *rat0PmiInfo_p,
                                                UInt32 *bitMapToUse_p,
                                                InternalCellIndex internalCellIndex
                                                )
{
    UInt32 pmiIndex = 0;

    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
    {
        UInt32 pmiReportedSubband_1 = 
            bitMapToBeReturned & rat0PmiInfo_p->reportedRBGRank1[0];
    
        UInt32 foundRBGCntPmi1 = getCount(pmiReportedSubband_1);
    
        UInt32 pmiReportedSubband_2 = 
            bitMapToBeReturned & rat0PmiInfo_p->reportedRBGRank1[1];

        UInt32 foundRBGCntPmi2 = getCount(pmiReportedSubband_2);

        UInt32 pmiReportedSubband_3 = 
            bitMapToBeReturned & rat0PmiInfo_p->reportedRBGRank1[2];

        UInt32 foundRBGCntPmi3 = getCount(pmiReportedSubband_3);

        UInt32 pmiReportedSubband_4 = 
            bitMapToBeReturned & rat0PmiInfo_p->reportedRBGRank1[3];

        UInt32 foundRBGCntPmi4 = getCount(pmiReportedSubband_4);

        UInt32 maxNumberOfRBG12 = 
            (foundRBGCntPmi1 >= foundRBGCntPmi2)? foundRBGCntPmi1 : foundRBGCntPmi2;
        UInt32 maxNumberOfRBG34 = 
            (foundRBGCntPmi3 >= foundRBGCntPmi4)? foundRBGCntPmi3 : foundRBGCntPmi4;  
        UInt32 maxNumberOfRBG = 
            (maxNumberOfRBG12 >= maxNumberOfRBG34)? maxNumberOfRBG12 : maxNumberOfRBG34;
        
        if (maxNumberOfRBG == foundRBGCntPmi1)
        {
            pmiIndex = 1;
            *bitMapToUse_p = pmiReportedSubband_1;
        }
        else if(maxNumberOfRBG == foundRBGCntPmi2)
        {
            pmiIndex = 2;
            *bitMapToUse_p = pmiReportedSubband_2;            
        }
        else if (maxNumberOfRBG == foundRBGCntPmi3)
        {
            pmiIndex = 3;
            *bitMapToUse_p = pmiReportedSubband_3;                    
        }
        else if(maxNumberOfRBG == foundRBGCntPmi4)
        {
            pmiIndex = 4;
            *bitMapToUse_p = pmiReportedSubband_4;                    
        }
        else
        {  
            /* pmiIndex is by default set to 0 indicating precoding 
             * according to transmit diversity */
            *bitMapToUse_p = bitMapToBeReturned;
        } 

        if (0 == *bitMapToUse_p)
        {
            *bitMapToUse_p = bitMapToBeReturned;
            /*pmiIndex set to transmit diversity*/
            pmiIndex = 0;
        }
    }
    /* 4x2 MIMO S */
    else if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        pmiIndex = selectRATSubbandPMIIndexPort4( bitMapToUse_p,
                                   rat0PmiInfo_p->reportedRBGPMIForPort4);
    }    
    /* 4x2 MIMO E */
    return pmiIndex;
}

/******************************************************************************
 * Function Name    : selectRAT1SubbandPMIIndexRank2
 * Inputs           : bitMapToBeReturned - This holds the PRBs which are reported and available too,
 *                    subsetIndex_p - selected subset for resource allocation,
 *                    zeroNonZeroSpan - indication of right or left span of subset
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : bitMapToUse_p - contains bitmap of reported PRB on selected PMI
 * Returns          : pmiIndex - selected PMI Index
 * Description      : This funtion will check which PMI Index has maximum number of reported PRBs
 *                    and based on this give the available & reported PRBs Bitmap on this PMI index.
 ******************************************************************************/
STATIC  UInt32 selectRAT1SubbandPMIIndexRank2(
                                            UInt32 bitMapToBeReturned, 
                                            RAT1PmiInfo *subsetIndex_p,
                                            UInt32 *bitMapToUse_p,
                                            UInt32 zeroNonZeroSpan,
                                            InternalCellIndex internalCellIndex
                                           )
{
    UInt32 pmiIndex = 0;

    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
    {
        UInt32 pmiReportedSubband_1 = 0;
        UInt32 pmiReportedSubband_2 = 0;
        /*store the number of reported PRBs which are available with same PMI index*/

        if (zeroNonZeroSpan)
        {
            pmiReportedSubband_1 = bitMapToBeReturned &
             subsetIndex_p->spanNonZeroPmiInfo.reportedPRBsRank2[0];

            pmiReportedSubband_2 = bitMapToBeReturned &
             subsetIndex_p->spanNonZeroPmiInfo.reportedPRBsRank2[1];
        }
        else
        {
            pmiReportedSubband_1 = bitMapToBeReturned & 
             subsetIndex_p->spanZeroPmiInfo.reportedPRBsRank2[0];

            pmiReportedSubband_2 = bitMapToBeReturned &
             subsetIndex_p->spanZeroPmiInfo.reportedPRBsRank2[1];
        }

        if (getCount(pmiReportedSubband_1) >= getCount(pmiReportedSubband_2))
        {
            pmiIndex = 0;
            *bitMapToUse_p = pmiReportedSubband_1;
        }
        else
        {
            pmiIndex = 1;
            *bitMapToUse_p = pmiReportedSubband_2;
        }

        if (0 == *bitMapToUse_p)
        {
            *bitMapToUse_p = bitMapToBeReturned;
            /* setting WIDEBAND_PMI is just a indication that here wideband
             * PMI has to be used alongwith resource allocation.
             * This is not the actual wideband PMI value reported by UE*/
            pmiIndex = WIDEBAND_PMI;
        }
    }
    /* 4x2 MIMO S */
    else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        if (zeroNonZeroSpan)
        {
            pmiIndex = selectRATSubbandPMIIndexPort4( bitMapToUse_p,
                                 subsetIndex_p->spanNonZeroPmiInfo.\
                                 reportedRBGPMIForPort4 /*NonZero Span*/);
        }
        else
        {
            pmiIndex = selectRATSubbandPMIIndexPort4( bitMapToUse_p,
                                 subsetIndex_p->spanZeroPmiInfo.\
                                 reportedRBGPMIForPort4 /*Zero Span*/);
        }
    }    
    /* 4x2 MIMO E */
    return pmiIndex;

}

/******************************************************************************
 * Function Name    : selectRAT1SubbandPMIIndexRank1
 * Inputs           : bitMapToBeReturned - This holds the PRBs which are reported and available too,
 *                    subsetIndex_p - selected subset for resource allocation,
 *                    zeroNonZeroSpan - indication of right or left span of subset
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : bitMapToUse_p - contains bitmap of reported PRB on selected PMI
 * Returns          : pmiIndex - selected PMI Index
 * Description      : This funtion will check which PMI Index has maximum number of reported PRBs
 *                    and based on this give the available & reported PRB Bitmap on this PMI index.
 ******************************************************************************/
STATIC  UInt32 selectRAT1SubbandPMIIndexRank1(
                                                UInt32 bitMapToBeReturned, 
                                                RAT1PmiInfo *subsetIndex_p,
                                                UInt32 *bitMapToUse_p,
                                                UInt32 zeroNonZeroSpan,
                                                InternalCellIndex internalCellIndex
                                                )
{
    UInt32 pmiIndex = 0;

    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
    {
        UInt32 pmiReportedSubband_1 = 0;
        UInt32 pmiReportedSubband_2 = 0;
        UInt32 pmiReportedSubband_3 = 0;
        UInt32 pmiReportedSubband_4 = 0;

        UInt32 foundRBGCntPmi1 = 0;
        UInt32 foundRBGCntPmi2 = 0;
        UInt32 foundRBGCntPmi3 = 0;
        UInt32 foundRBGCntPmi4 = 0;

        UInt32 maxNumberOfRBG12 = 0;
        UInt32 maxNumberOfRBG34 = 0;
        UInt32 maxNumberOfRBG = 0;

        if (zeroNonZeroSpan)
        {
            /*store the number of reported PRB which are available with same PMI index*/
            pmiReportedSubband_1 = bitMapToBeReturned & 
            subsetIndex_p->spanNonZeroPmiInfo.reportedPRBsRank1[0];
        
            foundRBGCntPmi1 = getCount(pmiReportedSubband_1);

            pmiReportedSubband_2 = bitMapToBeReturned & 
            subsetIndex_p->spanNonZeroPmiInfo.reportedPRBsRank1[1];

            foundRBGCntPmi2 = getCount(pmiReportedSubband_2);

            pmiReportedSubband_3 = bitMapToBeReturned & 
            subsetIndex_p->spanNonZeroPmiInfo.reportedPRBsRank1[2];

            foundRBGCntPmi3 = getCount(pmiReportedSubband_3);

            pmiReportedSubband_4 = bitMapToBeReturned & 
            subsetIndex_p->spanNonZeroPmiInfo.reportedPRBsRank1[3];

            foundRBGCntPmi4 = getCount(pmiReportedSubband_4);
        }
        else
        {
            /*store the number of reported PRB which are available with same PMI index*/
            pmiReportedSubband_1 = bitMapToBeReturned & 
            subsetIndex_p->spanZeroPmiInfo.reportedPRBsRank1[0];
        
            foundRBGCntPmi1 = getCount(pmiReportedSubband_1);

            pmiReportedSubband_2 = bitMapToBeReturned & 
            subsetIndex_p->spanZeroPmiInfo.reportedPRBsRank1[1];

            foundRBGCntPmi2 = getCount(pmiReportedSubband_2);

            pmiReportedSubband_3 = bitMapToBeReturned & 
            subsetIndex_p->spanZeroPmiInfo.reportedPRBsRank1[2];

            foundRBGCntPmi3 = getCount(pmiReportedSubband_3);

            pmiReportedSubband_4 = bitMapToBeReturned & 
            subsetIndex_p->spanZeroPmiInfo.reportedPRBsRank1[3];

            foundRBGCntPmi4 = getCount(pmiReportedSubband_4);    
        }

        maxNumberOfRBG12 = 
        (foundRBGCntPmi1 >= foundRBGCntPmi2)? foundRBGCntPmi1 : foundRBGCntPmi2;
        maxNumberOfRBG34 = 
        (foundRBGCntPmi3 >= foundRBGCntPmi4)? foundRBGCntPmi3 : foundRBGCntPmi4;  
        maxNumberOfRBG = 
        (maxNumberOfRBG12 >= maxNumberOfRBG34)? maxNumberOfRBG12 : maxNumberOfRBG34;
        
        if (maxNumberOfRBG == foundRBGCntPmi1)
        {
            pmiIndex = 1;
            *bitMapToUse_p = pmiReportedSubband_1;
        }
        else if(maxNumberOfRBG == foundRBGCntPmi2)
        {
            pmiIndex = 2;
            *bitMapToUse_p = pmiReportedSubband_2;            
        }
        else if(maxNumberOfRBG == foundRBGCntPmi3)
        {
            pmiIndex = 3;
            *bitMapToUse_p = pmiReportedSubband_3;                    
        }
        else if(maxNumberOfRBG == foundRBGCntPmi4)
        {
            pmiIndex = 4;
            *bitMapToUse_p = pmiReportedSubband_4;                    
        }
        else
        { 
            *bitMapToUse_p = bitMapToBeReturned;
        }

        if (0 == *bitMapToUse_p)
        {
            *bitMapToUse_p = bitMapToBeReturned;
            /* setting WIDEBAND_PMI is just a indication that here wideband
             * PMI has to be used alongwith resource allocation.
             * This is not the actual wideband PMI value reported by UE*/
            pmiIndex = 0;
        }
    }
    /* 4x2 MIMO S */
    else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        if (zeroNonZeroSpan)
        {
             pmiIndex = selectRATSubbandPMIIndexPort4( bitMapToUse_p,
                                         subsetIndex_p->spanNonZeroPmiInfo.\
                                         reportedRBGPMIForPort4); /*NonZero Span*/
        }
        else
        {
            pmiIndex = selectRATSubbandPMIIndexPort4( bitMapToUse_p,
                                         subsetIndex_p->spanZeroPmiInfo.\
                                         reportedRBGPMIForPort4); /*Zero Span*/
        }
    }    
    /* 4x2 MIMO E */    
    return pmiIndex;
}

/*4x2 MIMO CR*/
/******************************************************************************
 * Function Name    : selectRATSubbandPMIIndexPort4
 * Inputs           : reportedRBGPMIForPort4_p
 * Outputs          : bitMapToUse_p - contains bitmap of reported PRB on selected PMI
 * Returns          : pmiIndex - selected PMI Index
 * Description      : This funtion will check which PMI Index has maximum number of reported PRBs
 *                    and based on this give the available & reported PRB Bitmap on this PMI index.
 ******************************************************************************/
STATIC  UInt32 selectRATSubbandPMIIndexPort4(
                                                UInt32 *bitMapToUse_p,
                                                UInt32 *reportedRBGPMIForPort4_p )
{
    UInt32 pmiIndex = 0;
    UInt8  indx ;
    UInt32 foundRBGCntPmiTmp = 0;
    UInt32 foundRBGCntPmi =0;
    UInt32 pmiReportedSubband =0;
    UInt32 bitMapToBeReturned = *bitMapToUse_p;

    /*store the number of reported RBG which are available with same PMI index*/
    for (indx=0; indx < PMI_INDEX_RANK; indx++)
    {
        pmiReportedSubband = bitMapToBeReturned & (reportedRBGPMIForPort4_p[indx]);

        foundRBGCntPmi = getCount(pmiReportedSubband);
        if (foundRBGCntPmiTmp < foundRBGCntPmi)
        {
            foundRBGCntPmiTmp = foundRBGCntPmi;
            pmiIndex = indx+1;
        }
    }
    /* if found any highest value */
    if (pmiIndex)
    {
        *bitMapToUse_p = bitMapToBeReturned & (reportedRBGPMIForPort4_p[pmiIndex-1]);
    }

    if (0 == *bitMapToUse_p)
    {
        /*pmiIndex set to transmit diversity*/
        pmiIndex = 0;
    }
    return pmiIndex;
}


/*****************************************************************************
 * Function Name    : selectRAT0WBPMIforRank2
 * Inputs           : bitMapToBeReturned - 
 *                    subBandReportInfo_p - Pointer to SubBandReportInfo
 * Outputs          : bitMapToUse_p - contains bitmap of reported PRB on selected PMI
 * Returns          : pmiIndex - selected PMI Index
 * Description      : This funtion will check which PMI Index has maximum number of 
 *                    reported PRBs and based on that fill the WB PMI Index.
 *****************************************************************************/
STATIC  UInt32 selectRAT0WBPMIforRank2(UInt32 bitMapToBeReturned,
                           SubBandReportInfo *subBandReportInfo_p)
{
    SubbandPMIInfo *subbandPMIInfo_p = 
        &(subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo);
    RAT0PmiInfo *rat0PmiInfo_p = 
        &subbandPMIInfo_p->rat0PmiInfo;
    UInt32 wideBandPMIIndex = 0;

    UInt32 pmiReportedSubband_1 = 
        bitMapToBeReturned & rat0PmiInfo_p->reportedRBGRank2[0];

    UInt32 pmiReportedSubband_2 = 
        bitMapToBeReturned & rat0PmiInfo_p->reportedRBGRank2[1];

    if (getCount(pmiReportedSubband_1) >= getCount(pmiReportedSubband_2))
    {
        wideBandPMIIndex = 0;
    }    
    else
    {
        wideBandPMIIndex = 1;
    }    
    return wideBandPMIIndex;
} 

/*****************************************************************************
 * Function Name     : selectRAT0WBPMIforRank1
 * Inputs            : bitMapToBeReturned - 
 *                     subBandReportInfo_p - Pointer to SubBandReportInfo
 * Outputs           : bitMapToUse_p - contains bitmap of reported PRB on selected PMI
 * Returns           : pmiIndex - selected PMI Index
 * Description       : This funtion will check which PMI Index has maximum number of 
 *                     reported PRBs and based on that fill the WB PMI Index.
 *****************************************************************************/
STATIC  UInt32 selectRAT0WBPMIforRank1(UInt32 bitMapToBeReturned,
                           SubBandReportInfo *subBandReportInfo_p)
{
    SubbandPMIInfo *subbandPMIInfo_p = 
        &(subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo);
    RAT0PmiInfo *rat0PmiInfo_p = &subbandPMIInfo_p->rat0PmiInfo;
    UInt32 wideBandPMIIndex = 0;

    UInt32 pmiReportedSubband_1 = 
        bitMapToBeReturned & rat0PmiInfo_p->reportedRBGRank1[0];

    UInt32 foundRBGCntPmi1 = getCount(pmiReportedSubband_1);

    UInt32 pmiReportedSubband_2 = 
        bitMapToBeReturned & rat0PmiInfo_p->reportedRBGRank1[1];

    UInt32 foundRBGCntPmi2 = getCount(pmiReportedSubband_2);

    UInt32 pmiReportedSubband_3 = 
        bitMapToBeReturned & rat0PmiInfo_p->reportedRBGRank1[2];

    UInt32 foundRBGCntPmi3 = getCount(pmiReportedSubband_3);

    UInt32 pmiReportedSubband_4 = 
        bitMapToBeReturned & rat0PmiInfo_p->reportedRBGRank1[3];

    UInt32 foundRBGCntPmi4 = getCount(pmiReportedSubband_4);

    UInt32 maxNumberOfRBG12 = 
        (foundRBGCntPmi1 >= foundRBGCntPmi2)? foundRBGCntPmi1 : foundRBGCntPmi2;
    UInt32 maxNumberOfRBG34 = 
        (foundRBGCntPmi3 >= foundRBGCntPmi4)? foundRBGCntPmi3 : foundRBGCntPmi4;  
    UInt32 maxNumberOfRBG = 
        (maxNumberOfRBG12 >= maxNumberOfRBG34)? maxNumberOfRBG12 : maxNumberOfRBG34;
        
    if (maxNumberOfRBG == foundRBGCntPmi1)
    {
        wideBandPMIIndex = 1;
    }
    else if(maxNumberOfRBG == foundRBGCntPmi2)
    {
        wideBandPMIIndex = 2;
    }
    else if (maxNumberOfRBG == foundRBGCntPmi3)
    {
        wideBandPMIIndex = 3;
    }
    else if(maxNumberOfRBG == foundRBGCntPmi4)
    {
        wideBandPMIIndex = 4;
    }
    return wideBandPMIIndex;
}

/*****************************************************************************
 * Function Name    : selectRAT1WBPMIforRank2
 * Inputs           : bitMapToBeReturned - 
 *                    subsetIndexAllocated - Allocated Subset Index
 *                    isNonZeroSpanConsideredForAlloc,
 *                    subBandReportInfo_p - Pointer to SubBandReportInfo
 * Outputs          : bitMapToUse_p - contains bitmap of reported PRB on selected PMI
 * Returns          : pmiIndex - selected PMI Index
 * Description      : This funtion will check which PMI Index has maximum number of 
 *                    reported PRBs and based on that fill the WB PMI Index.
 *****************************************************************************/
STATIC  UInt32 selectRAT1WBPMIforRank2(UInt32 bitMapToBeReturned,
                           UInt32 subsetIndexAllocated,
                           UInt32 isNonZeroSpanConsideredForAlloc,
                           SubBandReportInfo *subBandReportInfo_p)
{
    SubbandPMIInfo *subbandPMIInfo_p = 
        &(subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo);
    RAT1PmiInfo *rat1PmiInfo_p = 
        &subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated];
    UInt32 pmiReportedSubband_1 = 0;
    UInt32 pmiReportedSubband_2 = 0;
    UInt32 wideBandPMIIndex = 0;

    if(isNonZeroSpanConsideredForAlloc)
    {
        pmiReportedSubband_1 = 
            bitMapToBeReturned & rat1PmiInfo_p->spanNonZeroPmiInfo.reportedPRBsRank2[0]; 
        pmiReportedSubband_2 = 
            bitMapToBeReturned & rat1PmiInfo_p->spanNonZeroPmiInfo.reportedPRBsRank2[1]; 
    }
    else
    {
        pmiReportedSubband_1 = 
            bitMapToBeReturned & rat1PmiInfo_p->spanZeroPmiInfo.reportedPRBsRank2[0]; 
        pmiReportedSubband_2 = 
            bitMapToBeReturned & rat1PmiInfo_p->spanZeroPmiInfo.reportedPRBsRank2[1]; 
    }

    if (getCount(pmiReportedSubband_1) >= getCount(pmiReportedSubband_2))
    {
        wideBandPMIIndex = 0;
    }    
    else
    {
        wideBandPMIIndex = 1;
    }    
    return wideBandPMIIndex;
} 

/*****************************************************************************
 * Function Name    : selectRAT1WBPMIforRank1
 * Inputs           : bitMapToBeReturned - 
 *                    subsetIndexAllocated - Allocated Subset Index,
 *                    isNonZeroSpanConsideredForAlloc,
 *                    subBandReportInfo_p - Pointer to SubBandReportInfo
 * Outputs          : bitMapToUse_p - contains bitmap of reported PRB on selected PMI
 * Returns          : pmiIndex - selected PMI Index
 * Description      : This funtion will check which PMI Index has maximum number of 
 *                    reported PRBs and based on that fill the WB PMI Index.
 *****************************************************************************/
STATIC  UInt32 selectRAT1WBPMIforRank1(UInt32 bitMapToBeReturned,
                           UInt32 subsetIndexAllocated,
                           UInt32 isNonZeroSpanConsideredForAlloc,
                           SubBandReportInfo *subBandReportInfo_p)
{
    SubbandPMIInfo *subbandPMIInfo_p = 
        &(subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo);
    RAT1PmiInfo *rat1PmiInfo_p = 
        &subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated];
    UInt32 pmiReportedSubband_1 = 0;
    UInt32 pmiReportedSubband_2 = 0;
    UInt32 pmiReportedSubband_3 = 0;
    UInt32 pmiReportedSubband_4 = 0;
    UInt32 wideBandPMIIndex = 0;

    UInt32 foundRBGCntPmi1 = 0;
    UInt32 foundRBGCntPmi2 = 0;
    UInt32 foundRBGCntPmi3 = 0;
    UInt32 foundRBGCntPmi4 = 0;

    if(isNonZeroSpanConsideredForAlloc)
    {
        pmiReportedSubband_1 = 
            bitMapToBeReturned & rat1PmiInfo_p->spanNonZeroPmiInfo.reportedPRBsRank1[0]; 

        foundRBGCntPmi1 = getCount(pmiReportedSubband_1);

        pmiReportedSubband_2 = 
            bitMapToBeReturned & rat1PmiInfo_p->spanNonZeroPmiInfo.reportedPRBsRank1[1]; 

        foundRBGCntPmi2 = getCount(pmiReportedSubband_2);

        pmiReportedSubband_3 = 
            bitMapToBeReturned & rat1PmiInfo_p->spanNonZeroPmiInfo.reportedPRBsRank1[2]; 

        foundRBGCntPmi3 = getCount(pmiReportedSubband_3);

        pmiReportedSubband_4 = 
            bitMapToBeReturned & rat1PmiInfo_p->spanNonZeroPmiInfo.reportedPRBsRank1[3]; 

        foundRBGCntPmi4 = getCount(pmiReportedSubband_4);
    }
    else
    {
        pmiReportedSubband_1 = 
            bitMapToBeReturned & rat1PmiInfo_p->spanZeroPmiInfo.reportedPRBsRank1[0]; 

        foundRBGCntPmi1 = getCount(pmiReportedSubband_1);

        pmiReportedSubband_2 = 
            bitMapToBeReturned & rat1PmiInfo_p->spanZeroPmiInfo.reportedPRBsRank1[1]; 

        foundRBGCntPmi2 = getCount(pmiReportedSubband_2);

        pmiReportedSubband_3 = 
            bitMapToBeReturned & rat1PmiInfo_p->spanZeroPmiInfo.reportedPRBsRank1[2]; 

        foundRBGCntPmi3 = getCount(pmiReportedSubband_3);

        pmiReportedSubband_4 = 
            bitMapToBeReturned & rat1PmiInfo_p->spanZeroPmiInfo.reportedPRBsRank1[3]; 

        foundRBGCntPmi4 = getCount(pmiReportedSubband_4);
    }

    UInt32 maxNumberOfRBG12 = 
        (foundRBGCntPmi1 >= foundRBGCntPmi2)? foundRBGCntPmi1 : foundRBGCntPmi2;
    UInt32 maxNumberOfRBG34 = 
        (foundRBGCntPmi3 >= foundRBGCntPmi4)? foundRBGCntPmi3 : foundRBGCntPmi4;  
    UInt32 maxNumberOfRBG = 
        (maxNumberOfRBG12 >= maxNumberOfRBG34)? maxNumberOfRBG12 : maxNumberOfRBG34;
        
    if (maxNumberOfRBG == foundRBGCntPmi1)
    {
        wideBandPMIIndex = 1;
    }
    else if(maxNumberOfRBG == foundRBGCntPmi2)
    {
        wideBandPMIIndex = 2;
    }
    else if (maxNumberOfRBG == foundRBGCntPmi3)
    {
        wideBandPMIIndex = 3;
    }
    else if(maxNumberOfRBG == foundRBGCntPmi4)
    {
        wideBandPMIIndex = 4;
    }
    return wideBandPMIIndex;
}
/*Dynamic PDCCH + 5117 changes starts*/
/*****************************************************************************
 * Function Name    : calculateNumOfOverLapRbs
 * Inputs           : resrcAssignementBitMap,
 *                    rbsAllocated
 *                    internalCellIndex -  Cell-Index at MAC
 * Outputs          : None
 * Returns          : NumOfOverLappingRbs
 * Description      : This funtion will return num of over lapping RBs based on
 *                    resource assignment bitmap.
 *****************************************************************************/
 UInt8 calculateNumOfOverLapRbs(UInt32 resrcAssignementBitMap,UInt32 rbsAllocated,
        InternalCellIndex internalCellIndex)
{

/* SPR_5384_Fix Start */
    if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->dlAvailableRBs <=10)
    {
/*SPR 5412 Fix Start*/
        return rbsAllocated;
/*SPR 5412 Fix End*/
    }
    else
    {
/* SPR_5384_Fix End*/
    /* + Coverity_Stack Fix */
    ResourceAllocationPolicyInfo *raPolicyInfo_p = PNULL;
    raPolicyInfo_p = &(dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfoMIBReserved;
    resrcAssignementBitMap &= raPolicyInfo_p->rat0Info.mibResrcAssignementBitMap;
    /* - Coverity_Stack Fix */
    UInt8 i = 0;
    i = (*getCountofOverLapRB)(resrcAssignementBitMap,cellSpecificParams_g.\
        cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->NumOfOverLappingRbsInfo);  
    return i;
    }
}
/*Dynamic PDCCH + 5117 changes ends*/
/*****************************************************************************
 * Function Name     : calNumOfRbFromBitmapfor20And15Mhz
 * Inputs            : resrcAssignementBitMap
 *                     numOfOverLappingRbsInfo_p
 * Outputs           : None
 * Returns           : NumOfOverLappingRbs
 * Description       : This funtion will return num of over lapping RBs based on
 *                     resource assignment bitmap.
 *****************************************************************************/
UInt8 calNumOfRbFromBitmapfor20And15Mhz(UInt32 resrcAssignementBitMap,
                                   UInt32 *numOfOverLappingRbsInfo_p)
{
    UInt8 i = 0;
    for (i = 0; i < TOT_RES_BIT_MAP_COMB_FOR_5_10_15_20_MHZ; i++)
    {
	    if (numOfOverLappingRbsInfo_p[i] == resrcAssignementBitMap)
	    {
	        break;
	    }
    }
    if (i == 3|| i == 7)
    {
	    i -= 2;
    }
    return i;
}
/*****************************************************************************
 * Function Name    : calNumOfRbFromBitmapfor10Mhz
 * Inputs           : resrcAssignementBitMap,
 *                    numOfOverLappingRbsInfo_p
 * Outputs          : None
 * Returns          : NumOfOverLappingRbs
 * Description      : This funtion will return num of over lapping RBs based on
 *                    resource assignment bitmap.
 *****************************************************************************/
UInt8 calNumOfRbFromBitmapfor10Mhz(UInt32 resrcAssignementBitMap,
                            UInt32 *numOfOverLappingRbsInfo_p)
{
    UInt8 i = 0;
    for (i = 0; i < TOT_RES_BIT_MAP_COMB_FOR_5_10_15_20_MHZ; i++)
    {
	    if (numOfOverLappingRbsInfo_p[i] == resrcAssignementBitMap)
	    {
	        break;
	    }
    }
    if (i == 7)
    {
	    i -= 4;
    }
    return i;
}
/*****************************************************************************
 * Function Name     : calNumOfRbFromBitmapfor5Mhz
 * Inputs            : resrcAssignementBitMap,
 *                     numOfOverLappingRbsInfo_p
 * Outputs           : None
 * Returns           : NumOfOverLappingRbs
 * Description       : This funtion will return num of over lapping RBs based on
 *                     resource assignment bitmap.
 *****************************************************************************/
UInt8 calNumOfRbFromBitmapfor5Mhz(UInt32 resrcAssignementBitMap,
                                 UInt32 *numOfOverLappingRbsInfo_p)
{
    UInt8 i = 0;
    for (i = 0; i < TOT_RES_BIT_MAP_COMB_FOR_5_10_15_20_MHZ; i++)
    {
	    if (numOfOverLappingRbsInfo_p[i] == resrcAssignementBitMap)
	    {
	        break;
	    }
    }
    if (i == 1||(i == 3))
	    i++;
    else if (i == 5||(i == 7))
	    i -= 3;
    return i;
}
/*****************************************************************************
 * Function Name  : calNumOfRbFromBitmapfor3Mhz
 * Inputs         : resrcAssignementBitMap,
 *                  numOfOverLappingRbsInfo_p
 * Outputs        : None
 * Returns        : NumOfOverLappingRbs
 * Description    : This funtion will return num of over lapping RBs based on
 *                  resource assignment bitmap.
 *****************************************************************************/
UInt8 calNumOfRbFromBitmapfor3Mhz(UInt32 resrcAssignementBitMap,
                                 UInt32 *numOfOverLappingRbsInfo_p)
{
    UInt8 i = 0;
    for (i = 0; i < TOT_RES_BIT_MAP_COMB_FOR_3MHZ; i++)
    {
	    if (numOfOverLappingRbsInfo_p[i] == resrcAssignementBitMap)
	    {
	        break;
	    }
    }
    if (i>6&&i<12)
    i -= 6;
    else if (i>12)
    i -= 10;
    
    return i;
}
/*****************************************************************************
 * Function Name  : checkAllocRBLiesInCentralSix
 * Inputs         : prbNumber
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This funtion will check the prb Number whether it is  
 *                  belongs to central six RBs or not and return MAC_SUCCESS
 *                  if prb is one of the central six RBs.
 *****************************************************************************/
STATIC  UInt8 checkAllocRBLiesInCentralSix(UInt32 prbNumber,
                              InternalCellIndex  internalCellIndex)
{
    SInt8 first = 0;
    SInt8 last = MAX_NUM_OVERLAPPING_RB - 1;
    /* SPR 18122 Changes Start*/
    SInt8 middle = DIVIDE_BY_TWO(first+last);
    /* SPR 18122 Changes End*/
    UInt8 *centralSixRbsInfo_p = cellSpecificParams_g.\
           cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
           centralSixRbsInfo;

    while(first <= last)
    {
        if (centralSixRbsInfo_p[middle]<prbNumber)
            first = middle + 1;
        else if (centralSixRbsInfo_p[middle] == prbNumber)
            break;
        else
            last = middle - 1;
            /* SPR 18122 Changes Start*/
        middle = DIVIDE_BY_TWO(first+last);
        /* SPR 18122 Changes End*/
    }
    if (first > last)
        return MAC_FAILURE;
    return MAC_SUCCESS;
}
/*Dynamic PDCCH + 5117 changes starts*/
/*****************************************************************************
 * Function Name  : updateMCSRBbelowUECategoryCoderateForRAT1AndRAT2
 * Inputs         : harqProcess_p -  Pointer to DLHARQProcess,
 *                  ueDLContext_p - Pointer to DLUEContext
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks the TB size with the permissible TB size
 *                  if TB size is more then permissible then it reduce TB size by
 *                  decreasing MCS and it will update MCS to be used.
 *****************************************************************************/

MacRetType updateMCSRBbelowUECategoryCoderateForRAT1AndRAT2(
                            DLHARQProcess *harqProcess_p,
                            DLUEContext *ueDLContext_p,
                            InternalCellIndex internalCellIndex)
{
    UInt8 itbsIndex = 0;
    UInt8 modScheme = 0;
    UInt8 maxAllocatedRB = harqProcess_p->assignedResourcesNB - 1;
    UInt8 mcsIndexForTbOne = 0;
    UInt8 mcsIndexForTbTwo = 0;
    UInt8 ueCategory = ueDLContext_p->ueCategory;
    UInt16 numOfCodeBlock = 0;
    UInt32 tbTwoSize=0;
    UInt32 tbOneSize=0;
    UInt32 crcBits = PDSCH_CRC_BITS;
    UInt32 maxNumberOfCodedBits = 0;
    UInt32 permissibleTBSizeInBits=0;
    UInt32 permissibleTBSize = 0;

    McsRBInfoForSpatialMultiplex *mcsRBInfoForSpatialMultiplex_p = PNULL;
    McsRBInfoForNonSpatialMultiplex *mcsRBInfoForNonSpatialMultiplex_p = PNULL;

    if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
    {
        permissibleTBSize = ueDLContext_p->permissibleTBSize;
    }
    else
    {
        permissibleTBSize = ueDLContext_p->dlUeScellContext_p[1]->permissibleTBSize;
    }
    if((harqProcess_p->isTBOneValid) && 
            (harqProcess_p->isTBTwoValid))
    {
        mcsIndexForTbOne = harqProcess_p->dlHARQTBOneInfo.mcsIndex;
        mcsIndexForTbTwo = harqProcess_p->dlHARQTBTwoInfo.mcsIndex;

        if( NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator &&
                NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator ) 

        {   

            if (mcsIndexForTbTwo == 28)
            {
                mcsRBInfoForSpatialMultiplex_p =
                    &((*mcsRBTBSIzeInfoForSpatialMultiplex_p)[ueCategory - 1]
					[mcsIndexForTbOne][harqProcess_p->assignedResourcesNB - 1]);
                mcsIndexForTbTwo =
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbOneRat1AndRat2;
                mcsIndexForTbOne = 
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbTwoRat1AndRat2;

            }
            else
            {
                mcsRBInfoForSpatialMultiplex_p =
                    &((*mcsRBTBSIzeInfoForSpatialMultiplex_p)[ueCategory - 1]
					[mcsIndexForTbTwo][harqProcess_p->assignedResourcesNB - 1]);
                mcsIndexForTbOne =
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbOneRat1AndRat2;
                mcsIndexForTbTwo = 
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbTwoRat1AndRat2;
            }
            maxAllocatedRB =
                mcsRBInfoForSpatialMultiplex_p->numberOfresourceBlocksForRat1AndRat2 - 1;

            permissibleTBSizeInBits = MULTIPLY_BY_EIGHT(permissibleTBSize);

            itbsIndex = TBSArr[mcsIndexForTbOne];
            tbOneSize = TBSizeArr[itbsIndex][maxAllocatedRB];
            /*4x4 DL MIMO CHG START*/
            tbOneSize = MULTIPLY_BY_EIGHT(tbOneSize);
            /*4x4 DL MIMO CHG END*/
            itbsIndex = TBSArr[mcsIndexForTbTwo];
            tbTwoSize = TBSizeArr[itbsIndex][maxAllocatedRB];
            /*4x4 DL MIMO CHG START*/
            tbTwoSize = MULTIPLY_BY_EIGHT(tbTwoSize);
            /*4x4 DL MIMO CHG END*/
	    if((tbOneSize + tbTwoSize) > permissibleTBSizeInBits)
            {
                do
                {
                    if(mcsIndexForTbOne >= 1)
                    {    
                        mcsIndexForTbOne--;
                    }

                    itbsIndex = TBSArr[mcsIndexForTbOne];
                    tbOneSize = TBSizeArr[itbsIndex][maxAllocatedRB];
                    /*4x4 DL MIMO CHG START*/
                    tbOneSize = MULTIPLY_BY_EIGHT(tbOneSize);
                    /*4x4 DL MIMO CHG END*/  
                    if(mcsIndexForTbTwo >= 1)
                    {    
                        mcsIndexForTbTwo--;
                    }

                    itbsIndex = TBSArr[mcsIndexForTbTwo];
                    tbTwoSize = TBSizeArr[itbsIndex][maxAllocatedRB];
                    /*4x4 DL MIMO CHG START*/
                    tbTwoSize = MULTIPLY_BY_EIGHT(tbTwoSize);
                    /*4x4 DL MIMO CHG END*/

                  }while (((tbOneSize + tbTwoSize) > permissibleTBSizeInBits) &&
                        (mcsIndexForTbOne > 0) &&
                        (mcsIndexForTbTwo > 0));
                  

                if(((tbOneSize + tbTwoSize) > permissibleTBSizeInBits) &&
                       ((mcsIndexForTbOne <= 0)
                       || (mcsIndexForTbTwo <= 0)))
                {  
                    return MAC_FAILURE;    
                }
            }
            harqProcess_p->dlHARQTBOneInfo.mcsIndex = mcsIndexForTbOne;
            harqProcess_p->dlHARQTBTwoInfo.mcsIndex = mcsIndexForTbTwo; 
            harqProcess_p->assignedResourcesNB = (maxAllocatedRB + 1);
            

            harqProcess_p-> 
                dlHARQTBOneInfo.tbSize = 
                determineTBSize(
                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                        harqProcess_p->assignedResourcesNB,
                        /*4x4 DL MIMO CHG START*/
                        TB_MAPPED_TO_ONE_LAYER
                        /*4x4 DL MIMO CHG END*/); 

            harqProcess_p->
                dlHARQTBTwoInfo.tbSize = 
                determineTBSize(
                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                        harqProcess_p->assignedResourcesNB,
                        /*4x4 DL MIMO CHG START*/
                        TB_MAPPED_TO_ONE_LAYER
                        /*4x4 DL MIMO CHG END*/); 

        }

    }
    else if((harqProcess_p->isTBOneValid) &&
            ( NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator))
    {
        mcsIndexForTbOne = harqProcess_p->dlHARQTBOneInfo.mcsIndex;
        mcsRBInfoForNonSpatialMultiplex_p = 
            &((*mcsRBTBSIzeInfoForNonSpatialMultiplex_p)[ueCategory - 1][harqProcess_p->assignedResourcesNB - 1]); 


        mcsIndexForTbOne =
            mcsRBInfoForNonSpatialMultiplex_p->mcsIndexForRat1AndRat2;

        maxAllocatedRB =
            mcsRBInfoForNonSpatialMultiplex_p->numberOfresourceBlocksForRat1AndRat2 - 1;

        permissibleTBSizeInBits = MULTIPLY_BY_EIGHT(permissibleTBSize);

        itbsIndex = TBSArr[mcsIndexForTbOne];
        tbOneSize = TBSizeArr[itbsIndex][maxAllocatedRB];
        /*4x4 DL MIMO CHG START*/
        tbOneSize = MULTIPLY_BY_EIGHT(tbOneSize);
        /*4x4 DL MIMO CHG END*/
        if(tbOneSize  > permissibleTBSizeInBits)
        {
            do
            {
                mcsIndexForTbOne--;
                itbsIndex = TBSArr[mcsIndexForTbOne];
                tbOneSize = TBSizeArr[itbsIndex][maxAllocatedRB];
                /*4x4 DL MIMO CHG START*/
                tbOneSize = MULTIPLY_BY_EIGHT(tbOneSize);
                /*4x4 DL MIMO CHG END*/

                crcBits = PDSCH_CRC_BITS;

                /* Cyclomatic_complexity_changes_start */
                getModSchemeFromMcsIndex(mcsIndexForTbOne, &modScheme);
                /* Cyclomatic_complexity_changes_end */

                maxNumberOfCodedBits = (*maxNumberOfCodedBitsMap_p)[(((modScheme)/QPSK) - 1)][maxAllocatedRB][0];

                if ((tbOneSize + PDSCH_CRC_BITS) > MAX_CODE_BLOCK_SIZE)
                {
                    numOfCodeBlock = ceil_wrapper((tbOneSize+PDSCH_CRC_BITS)/\
                            (MAX_CODE_BLOCK_SIZE - PDSCH_CRC_BITS));
                    crcBits = (numOfCodeBlock*PDSCH_CRC_BITS);
                }

            }while (((tbOneSize  > permissibleTBSizeInBits) ||
                    ((tbOneSize + crcBits) > maxNumberOfCodedBits)) &&
                    (mcsIndexForTbOne > 0));

            if((0 >=  mcsIndexForTbOne) &&
                    (((tbOneSize + crcBits) > maxNumberOfCodedBits) ||
                     (tbOneSize  > permissibleTBSizeInBits)))
            {
                return MAC_FAILURE;    
            }
           
        }
        

        harqProcess_p->dlHARQTBOneInfo.mcsIndex = mcsIndexForTbOne;
        harqProcess_p->assignedResourcesNB = 
            mcsRBInfoForNonSpatialMultiplex_p->numberOfresourceBlocksForRat1AndRat2;

        harqProcess_p-> 
            dlHARQTBOneInfo.tbSize = 
            determineTBSize(
                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                    harqProcess_p->assignedResourcesNB,
                    /*4x4 DL MIMO CHG START*/
                    TB_MAPPED_TO_ONE_LAYER
                    /*4x4 DL MIMO CHG END*/);
    }

    else if((harqProcess_p->isTBTwoValid) &&
            (NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator ))
    {
        mcsIndexForTbTwo = harqProcess_p->dlHARQTBTwoInfo.mcsIndex;

        mcsRBInfoForNonSpatialMultiplex_p = 
            &((*mcsRBTBSIzeInfoForNonSpatialMultiplex_p)[ueCategory - 1][harqProcess_p->assignedResourcesNB - 1]); 


        mcsIndexForTbTwo =
            mcsRBInfoForNonSpatialMultiplex_p->mcsIndexForRat1AndRat2;

        maxAllocatedRB =
            mcsRBInfoForNonSpatialMultiplex_p->numberOfresourceBlocksForRat1AndRat2 - 1;

        permissibleTBSizeInBits = MULTIPLY_BY_EIGHT(permissibleTBSize);

        itbsIndex = TBSArr[mcsIndexForTbTwo];
        tbTwoSize = TBSizeArr[itbsIndex][maxAllocatedRB];
        /*4x4 DL MIMO CHG START*/
        tbTwoSize = MULTIPLY_BY_EIGHT(tbTwoSize);
        /*4x4 DL MIMO CHG END*/

        if(tbTwoSize  > permissibleTBSizeInBits)
        {
            do
            {
                mcsIndexForTbTwo--;
                itbsIndex = TBSArr[mcsIndexForTbTwo];
                tbTwoSize = TBSizeArr[itbsIndex][maxAllocatedRB];
                /*4x4 DL MIMO CHG START*/
                tbTwoSize = MULTIPLY_BY_EIGHT(tbTwoSize);
                /*4x4 DL MIMO CHG END*/
                crcBits = PDSCH_CRC_BITS;

                /* Cyclomatic_complexity_changes_start */
                getModSchemeFromMcsIndex(mcsIndexForTbTwo, &modScheme);
                /* Cyclomatic_complexity_changes_end */
                
                maxNumberOfCodedBits = (*maxNumberOfCodedBitsMap_p)[(((modScheme)/QPSK) - 1)][maxAllocatedRB][0];

                if ((tbTwoSize + PDSCH_CRC_BITS) > MAX_CODE_BLOCK_SIZE)
                {
                    numOfCodeBlock = ceil_wrapper((tbTwoSize+PDSCH_CRC_BITS)/\
                            (MAX_CODE_BLOCK_SIZE - PDSCH_CRC_BITS));
                    crcBits = (numOfCodeBlock*PDSCH_CRC_BITS);
                }
               
            }while (((tbTwoSize > permissibleTBSizeInBits) || 
                    ((tbTwoSize + crcBits) > maxNumberOfCodedBits)) &&
                    (mcsIndexForTbTwo > 0));

            if(((tbTwoSize  > permissibleTBSizeInBits) || 
                    ((tbTwoSize + crcBits) > maxNumberOfCodedBits)) &&
                    (mcsIndexForTbTwo <= 0))
            {

                lteWarning("[%s] checkCodeRateUECategory retunring failure\n", __func__);
                return MAC_FAILURE;
            }

        }
        harqProcess_p->dlHARQTBTwoInfo.mcsIndex = mcsIndexForTbTwo;
        harqProcess_p->assignedResourcesNB = 
            mcsRBInfoForNonSpatialMultiplex_p->numberOfresourceBlocksForRat1AndRat2;

        harqProcess_p->
            dlHARQTBTwoInfo.tbSize = 
            determineTBSize(
                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                    harqProcess_p->assignedResourcesNB,
                    /*4x4 DL MIMO CHG START*/
                    TB_MAPPED_TO_ONE_LAYER
                    /*4x4 DL MIMO CHG END*/);
    }
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : updateMCSRBbelowUECategoryCoderateForRAT0
 * Inputs         : useLastRBG,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  ueContext_p - Pointer to DLUEContext
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks the TB size with the permissible TB size
 *                  if TB size is more then permissible then it reduce TB size by
 *                  decreasing MCS and it will update MCS to be used.
 *****************************************************************************/

MacRetType updateMCSRBbelowUECategoryCoderateForRAT0(UInt8 useLastRBG,
                                                     DLHARQProcess *harqProcess_p,
                                                     DLUEContext *ueContext_p,
                                                     InternalCellIndex internalCellIndex)
{
    UInt8 itbsIndex = 0;
    UInt8 maxAllocatedRB = harqProcess_p->assignedResourcesNB - 1;
    SInt8 mcsIndexForTbOne = 0;
    SInt8 mcsIndexForTbTwo = 0;
    UInt8 ueCategory = ueContext_p->ueCategory;
    UInt32 tbTwoSize=0;
    UInt32 tbOneSize=0;
    UInt32 permissibleTBSizeInBits=0;
    UInt32 permissibleTBSize = 0;
    McsRBInfoForNonSpatialMultiplex *mcsRBInfoForNonSpatialMultiplex_p = PNULL;

    if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex))
    {
        permissibleTBSize = ueContext_p->permissibleTBSize;
    }
    else
    {
        permissibleTBSize = ueContext_p->dlUeScellContext_p[1]->permissibleTBSize;
    }
    if((harqProcess_p->isTBOneValid) && 
            (harqProcess_p->isTBTwoValid))
    {
        /* cyclomatic complexity changes start */
        if(MAC_SUCCESS != updateMCSRBForTbOneAndTbTwo(harqProcess_p,
                    ueCategory, permissibleTBSize, useLastRBG))
        {
            return MAC_FAILURE;
        }
        /* cyclomatic complexity changes end */
    }
    else if((harqProcess_p->isTBOneValid) &&
            ( NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator))
    {
        mcsIndexForTbOne = harqProcess_p->dlHARQTBOneInfo.mcsIndex;
        mcsRBInfoForNonSpatialMultiplex_p = 
            &((*mcsRBTBSIzeInfoForNonSpatialMultiplex_p)[ueCategory - 1][harqProcess_p->assignedResourcesNB - 1]); 

        if(0 == useLastRBG)
        {
            /* SPR 10860 changes start */
            if(mcsRBInfoForNonSpatialMultiplex_p->mcsIndexForRat0 < mcsIndexForTbOne)
            {
                mcsIndexForTbOne =
                    mcsRBInfoForNonSpatialMultiplex_p->mcsIndexForRat0;
            }    
            /* SPR 10860 changes end */

            maxAllocatedRB =
                mcsRBInfoForNonSpatialMultiplex_p->numberOfresourceBlocksForRat0 - 1;
        }
        else
        {
            mcsIndexForTbOne =
                mcsRBInfoForNonSpatialMultiplex_p->mcsIndexForRat0WithLastRBG;

            maxAllocatedRB =
                mcsRBInfoForNonSpatialMultiplex_p->numberOfresourceBlocksForRat0WithLastRBG - 1;

        }

        permissibleTBSizeInBits = MULTIPLY_BY_EIGHT(permissibleTBSize);
            /* SPR 10860 changes start */
            if(mcsRBInfoForNonSpatialMultiplex_p->mcsIndexForRat0 < mcsIndexForTbOne)
            {
                mcsIndexForTbOne =
                    mcsRBInfoForNonSpatialMultiplex_p->mcsIndexForRat0;
            }    
            /* SPR 10860 changes end */
        itbsIndex = TBSArr[mcsIndexForTbOne];
        tbOneSize = TBSizeArr[itbsIndex][maxAllocatedRB];
        /*4x4 DL MIMO CHG START*/
        tbOneSize = MULTIPLY_BY_EIGHT(tbOneSize);
        /*4x4 DL MIMO CHG END*/ 
	if(tbOneSize  > permissibleTBSizeInBits)
        {
            do
            {
                mcsIndexForTbOne--;
                itbsIndex = TBSArr[mcsIndexForTbOne];
                tbOneSize = TBSizeArr[itbsIndex][maxAllocatedRB];
                /*4x4 DL MIMO CHG START*/
                tbOneSize = MULTIPLY_BY_EIGHT(tbOneSize);
                /*4x4 DL MIMO CHG END*/
            }while ((tbOneSize  > permissibleTBSizeInBits)&& 
                    (mcsIndexForTbOne > 0));

            if((0 >=  mcsIndexForTbOne) &&
                     (tbOneSize  > permissibleTBSizeInBits))
            {
                lteWarning("[%s] failure.. mcsTBOne[%d] tbs[%d] permissible[%d]\n", __func__, 
				mcsIndexForTbOne, tbOneSize, permissibleTBSizeInBits);
                return MAC_FAILURE;    
            }

        }

        harqProcess_p->dlHARQTBOneInfo.mcsIndex = mcsIndexForTbOne;
        harqProcess_p->assignedResourcesNB = maxAllocatedRB + 1; 

        harqProcess_p-> 
            dlHARQTBOneInfo.tbSize = 
            determineTBSize(
                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                    harqProcess_p->assignedResourcesNB,
                    /*4x4 DL MIMO CHG START*/
                    TB_MAPPED_TO_ONE_LAYER
                    /*4x4 DL MIMO CHG START*/);

    }
    else if((harqProcess_p->isTBTwoValid) &&
            (NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator ))
    {
        /*Cov 72958 + -*/
        mcsRBInfoForNonSpatialMultiplex_p = 
            &((*mcsRBTBSIzeInfoForNonSpatialMultiplex_p)[ueCategory - 1][harqProcess_p->assignedResourcesNB - 1]); 


        if(0 == useLastRBG)
        {
            mcsIndexForTbTwo =
                mcsRBInfoForNonSpatialMultiplex_p->mcsIndexForRat0;

            maxAllocatedRB =
                mcsRBInfoForNonSpatialMultiplex_p->numberOfresourceBlocksForRat0 - 1;
        }
        else
        {
            mcsIndexForTbTwo =
                mcsRBInfoForNonSpatialMultiplex_p->mcsIndexForRat0WithLastRBG;

            maxAllocatedRB =
                mcsRBInfoForNonSpatialMultiplex_p->numberOfresourceBlocksForRat0WithLastRBG - 1;

        }

        permissibleTBSizeInBits = MULTIPLY_BY_EIGHT(permissibleTBSize);

        itbsIndex = TBSArr[mcsIndexForTbTwo];
        tbTwoSize = TBSizeArr[itbsIndex][maxAllocatedRB];
        /*4x4 DL MIMO CHG START*/
        tbTwoSize = MULTIPLY_BY_EIGHT(tbTwoSize);
        /*4x4 DL MIMO CHG END*/
        if(tbTwoSize  > permissibleTBSizeInBits)
        {
            do
            {
                mcsIndexForTbTwo--;
                itbsIndex = TBSArr[mcsIndexForTbTwo];
                tbTwoSize = TBSizeArr[itbsIndex][maxAllocatedRB];
                /*4x4 DL MIMO CHG START*/
                tbTwoSize = MULTIPLY_BY_EIGHT(tbTwoSize);
                /*4x4 DL MIMO CHG END*/ 
            }while ((tbTwoSize > permissibleTBSizeInBits) && 
                    (mcsIndexForTbTwo > 0));

            if((tbTwoSize  > permissibleTBSizeInBits) && 
                    (mcsIndexForTbTwo <= 0))
            {
                lteWarning("[%s] failure.. mcsTBTwo[%d] tbs[%d] permissible[%d]\n", __func__,
			 mcsIndexForTbTwo, tbTwoSize, permissibleTBSizeInBits);
                return MAC_FAILURE;
            }
        }
        harqProcess_p->dlHARQTBTwoInfo.mcsIndex = mcsIndexForTbTwo;
        harqProcess_p->assignedResourcesNB = maxAllocatedRB + 1; 

        harqProcess_p->
            dlHARQTBTwoInfo.tbSize = 
            determineTBSize(
                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                    harqProcess_p->assignedResourcesNB,
                    /*4x4 DL MIMO CHG START*/
                    TB_MAPPED_TO_ONE_LAYER
                    /*4x4 DL MIMO CHG START*/); 
    }
    return MAC_SUCCESS;
}
/*Dynamic PDCCH + 5117 changes ends */
/*****************************************************************************
 * Function Name  : updateMCSRBForTbOneAndTbTwo 
 * Inputs         : harqProcess_p - Pointer to DLHARQProcess,
 *                  ueCategory,
 *                  permissibleTBSize,
 *                  useLastRBG
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function updates MCS for TbOne and TbTwo
 *****************************************************************************/

STATIC  MacRetType updateMCSRBForTbOneAndTbTwo(DLHARQProcess *harqProcess_p,
                                                     UInt8 ueCategory,
                                                     UInt32 permissibleTBSize,
                                                     UInt8 useLastRBG)
{
    SInt8 mcsIndexForTbOne = 0;
    SInt8 mcsIndexForTbTwo = 0;
    McsRBInfoForSpatialMultiplex *mcsRBInfoForSpatialMultiplex_p = PNULL;
    UInt32 permissibleTBSizeInBits=0;
    UInt8 itbsIndex = 0;
    UInt32 tbTwoSize=0;
    UInt32 tbOneSize=0;
    UInt8 maxAllocatedRB = harqProcess_p->assignedResourcesNB - 1;

    mcsIndexForTbOne = harqProcess_p->dlHARQTBOneInfo.mcsIndex;
    mcsIndexForTbTwo = harqProcess_p->dlHARQTBTwoInfo.mcsIndex;

    if( NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator &&
            NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator ) 

    {
        if (mcsIndexForTbTwo == 28)
        {
            mcsRBInfoForSpatialMultiplex_p = 
                &((*mcsRBTBSIzeInfoForSpatialMultiplex_p)[ueCategory - 1]
                        [mcsIndexForTbOne][harqProcess_p->assignedResourcesNB - 1]);
            if(0 == useLastRBG)
            {
                mcsIndexForTbTwo =
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbOneRat0;
                mcsIndexForTbOne = 
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbTwoRat0;
                maxAllocatedRB =
                    mcsRBInfoForSpatialMultiplex_p->numberOfresourceBlocksForRat0 - 1;
            }
            else
            {
                mcsIndexForTbTwo =
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbOneRat0WithLastRBG;
                mcsIndexForTbOne = 
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbTwoRat0WithLastRBG;
                maxAllocatedRB =
                    mcsRBInfoForSpatialMultiplex_p->numberOfresourceBlocksForRat0WithLastRBG - 1;
            }
        }
        else
        {
            mcsRBInfoForSpatialMultiplex_p = 
                &((*mcsRBTBSIzeInfoForSpatialMultiplex_p)[ueCategory - 1]
                        [mcsIndexForTbTwo][harqProcess_p->assignedResourcesNB - 1]);
            if(0 == useLastRBG)
            {
                mcsIndexForTbOne =
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbOneRat0;
                mcsIndexForTbTwo = 
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbTwoRat0;
                maxAllocatedRB =
                    mcsRBInfoForSpatialMultiplex_p->numberOfresourceBlocksForRat0 - 1;
            }
            else
            {
                mcsIndexForTbOne =
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbOneRat0WithLastRBG;
                mcsIndexForTbTwo = 
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbTwoRat0WithLastRBG;
                maxAllocatedRB =
                    mcsRBInfoForSpatialMultiplex_p->numberOfresourceBlocksForRat0WithLastRBG - 1;
            }

        }



        permissibleTBSizeInBits = MULTIPLY_BY_EIGHT(permissibleTBSize);

        itbsIndex = TBSArr[mcsIndexForTbOne];
        tbOneSize = TBSizeArr[itbsIndex][maxAllocatedRB];
        /*4x4 DL MIMO CHG START*/
        tbOneSize = MULTIPLY_BY_EIGHT(tbOneSize);
        /*4x4 DL MIMO CHG END*/
        itbsIndex = TBSArr[mcsIndexForTbTwo];
        tbTwoSize = TBSizeArr[itbsIndex][maxAllocatedRB];
        /*4x4 DL MIMO CHG START*/
        tbTwoSize = MULTIPLY_BY_EIGHT(tbTwoSize);
        /*4x4 DL MIMO CHG END*/
        if((tbOneSize + tbTwoSize) > permissibleTBSizeInBits)
        {
            do
            {
                if(mcsIndexForTbOne >= 1)
                {    
                    mcsIndexForTbOne--;
                }

                itbsIndex = TBSArr[mcsIndexForTbOne];
                tbOneSize = TBSizeArr[itbsIndex][maxAllocatedRB];
                /*4x4 DL MIMO CHG START*/
                tbOneSize = MULTIPLY_BY_EIGHT(tbOneSize);
                /*4x4 DL MIMO CHG END*/
                if(mcsIndexForTbTwo >= 1)
                {    
                    mcsIndexForTbTwo--;
                }

                itbsIndex = TBSArr[mcsIndexForTbTwo];
                tbTwoSize = TBSizeArr[itbsIndex][maxAllocatedRB];
                /*4x4 DL MIMO CHG START*/
                tbTwoSize = MULTIPLY_BY_EIGHT(tbTwoSize);
                /*4x4 DL MIMO CHG END*/
            }while (((tbOneSize + tbTwoSize) > (permissibleTBSizeInBits)) &&
                    (mcsIndexForTbOne > 0) && 
                    (mcsIndexForTbTwo > 0));

            if(((tbOneSize + tbTwoSize) > (permissibleTBSizeInBits)) &&
                    ((mcsIndexForTbOne <= 0) 
                     ||(mcsIndexForTbTwo <= 0)))
            {
                return MAC_FAILURE;    
            }
        }
        harqProcess_p->dlHARQTBOneInfo.mcsIndex = mcsIndexForTbOne;
        harqProcess_p->dlHARQTBTwoInfo.mcsIndex = mcsIndexForTbTwo; 
        harqProcess_p->assignedResourcesNB = (maxAllocatedRB + 1);

        harqProcess_p-> 
            dlHARQTBOneInfo.tbSize = 
            determineTBSize(
                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                    harqProcess_p->assignedResourcesNB,
                    /*4x4 DL MIMO CHG START*/
                    TB_MAPPED_TO_ONE_LAYER
                    /*4x4 DL MIMO CHG END*/);

        harqProcess_p->
            dlHARQTBTwoInfo.tbSize = 
            determineTBSize(
                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                    harqProcess_p->assignedResourcesNB,
                    /*4x4 DL MIMO CHG START*/
                    TB_MAPPED_TO_ONE_LAYER
                    /*4x4 DL MIMO CHG END*/); 

    }
    return MAC_SUCCESS;
}

/* 4x4 DL MIMO CHG START */
/*****************************************************************************
 * Function Name  : updateMCSRBbelowCoderateForRAT0For4x4Mimo 
 * Inputs         : useLastRBG,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  ueDLContext_p - Pointer to DLUEContext
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : Nothing
 * Description    : This function update the MCS and RB's of belwe code rate 
 *                  for RAT0
 *****************************************************************************/
/** klockwork Changes Start **/
void updateMCSRBbelowCoderateForRAT0For4x4Mimo(UInt8 useLastRBG,
                                                     DLHARQProcess *harqProcess_p,
                                                     /* +- SPR 17777 */
                                                     InternalCellIndex internalCellIndex)
{
    SInt8 mcsIndexForTbOne = 0;
    SInt8 mcsIndexForTbTwo = 0;
    UInt8 maxAllocatedRB = harqProcess_p->assignedResourcesNB - 1;
    McsRBInfoForSpatialMultiplex *mcsRBInfoForSpatialMultiplex_p = PNULL;

    if((harqProcess_p->isTBOneValid) && 
            (harqProcess_p->isTBTwoValid))
    {
        mcsIndexForTbOne = harqProcess_p->dlHARQTBOneInfo.mcsIndex;
        mcsIndexForTbTwo = harqProcess_p->dlHARQTBTwoInfo.mcsIndex;
        
        /* klockwork warning fix start */
        if ( mcsIndexForTbOne > 28 || mcsIndexForTbTwo > 28 )
        {
            return;
        }
        /* klockwork warning fix end */

        if( NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator &&
                NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator && 
                mcsIndexForTbOne < MAX_MCS_INDEX && harqProcess_p->assignedResourcesNB <= MAX_NUM_OF_RB) 
        {
            if (mcsIndexForTbTwo == 28)
            {
		if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
	        {
		    mcsRBInfoForSpatialMultiplex_p = 
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)[LAYER_4_IDX_1][MCS_28_IDX_1]\
			[mcsIndexForTbOne][harqProcess_p->assignedResourcesNB - 1]);    
		}
        /*coverity 41625*/
		else /*if (harqProcess_p->numOfLayer == RANK_INDICATOR_3)*/
        /*coverity 41625*/
	        {
		    mcsRBInfoForSpatialMultiplex_p = 
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)[LAYER_3_IDX_0][MCS_28_IDX_1]\
			[mcsIndexForTbOne][harqProcess_p->assignedResourcesNB - 1]); 
	        }
            }
	    else if ((currentCFIValue_g[internalCellIndex] == 3) && (mcsIndexForTbTwo == 27))
            {
                if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
	        {
                    mcsRBInfoForSpatialMultiplex_p = 
                       &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)[LAYER_4_IDX_1][MCS_27_IDX_0]
					[mcsIndexForTbOne][harqProcess_p->assignedResourcesNB - 1]);
		}
        /*coverity 41625*/
		else /*if (harqProcess_p->numOfLayer == RANK_INDICATOR_3)*/
        /*coverity 41625*/
	        {
                    mcsRBInfoForSpatialMultiplex_p = 
                       &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)[LAYER_3_IDX_0][MCS_27_IDX_0]
					[mcsIndexForTbOne][harqProcess_p->assignedResourcesNB - 1]);
	        }
            }
            else if (mcsIndexForTbOne == 28)
            {
		if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
	        {
                    mcsRBInfoForSpatialMultiplex_p = 
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)[LAYER_4_IDX_1][MCS_28_IDX_1]
					[mcsIndexForTbTwo][harqProcess_p->assignedResourcesNB - 1]);
		}
        /*coverity 41625*/
                else/* if (harqProcess_p->numOfLayer == RANK_INDICATOR_3)*/
        /*coverity 41625*/
	        {
                    mcsRBInfoForSpatialMultiplex_p = 
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)[LAYER_3_IDX_0][MCS_28_IDX_1]
					[mcsIndexForTbTwo][harqProcess_p->assignedResourcesNB - 1]);
		}
            }
	    else if((currentCFIValue_g[internalCellIndex] == 3) && (mcsIndexForTbOne == 27))
            {
                if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
	        {
                    mcsRBInfoForSpatialMultiplex_p = 
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)[LAYER_4_IDX_1][MCS_27_IDX_0]
					[mcsIndexForTbTwo][harqProcess_p->assignedResourcesNB - 1]);
		}
        /*coverity 41625*/
                else /*if (harqProcess_p->numOfLayer == RANK_INDICATOR_3)*/
        /*coverity 41625*/
	        {
                    mcsRBInfoForSpatialMultiplex_p = 
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)[LAYER_3_IDX_0][MCS_27_IDX_0]
					[mcsIndexForTbTwo][harqProcess_p->assignedResourcesNB - 1]);
		}
            }
            /*coverity 41625*/
            if(mcsRBInfoForSpatialMultiplex_p)
            {
            /*coverity 41625*/
                if(0 == useLastRBG)
                {
                    mcsIndexForTbOne =
                        mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbOneRat0;
                    mcsIndexForTbTwo = 
                        mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbTwoRat0;
                    maxAllocatedRB =
                        mcsRBInfoForSpatialMultiplex_p->numberOfresourceBlocksForRat0 - 1;
                }
                else
                {
                    mcsIndexForTbOne =
                        mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbOneRat0WithLastRBG;
                    mcsIndexForTbTwo = 
                        mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbTwoRat0WithLastRBG;
                    maxAllocatedRB =
                        mcsRBInfoForSpatialMultiplex_p->numberOfresourceBlocksForRat0WithLastRBG - 1;
                }

                harqProcess_p->dlHARQTBOneInfo.mcsIndex = mcsIndexForTbOne;
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex = mcsIndexForTbTwo; 
                harqProcess_p->assignedResourcesNB = (maxAllocatedRB + 1);
                if(harqProcess_p->numOfLayer == RANK_INDICATOR_3)
                {
                    harqProcess_p-> 
                        dlHARQTBOneInfo.tbSize = 
                            determineTBSize(
                            harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                            harqProcess_p->assignedResourcesNB,
                            TB_MAPPED_TO_ONE_LAYER);
	            }
                else
                {
                    harqProcess_p-> 
                        dlHARQTBOneInfo.tbSize = 
                            determineTBSize(
                            harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                            harqProcess_p->assignedResourcesNB,
                            TB_MAPPED_TO_TWO_LAYER);
                }
                harqProcess_p->
                    dlHARQTBTwoInfo.tbSize = 
                    determineTBSize(
                            harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                            harqProcess_p->assignedResourcesNB,
                            TB_MAPPED_TO_TWO_LAYER);
            }
        }
    }
}
/** klockwork Changes End **/

/*****************************************************************************
 * Function Name  : updateMCSRBbelowCoderateForRAT1AndRAT2For4x4Mimo
 * Inputs         : harqProcess_p - Pointer to DLHARQProcess,
 *                  ueDLContext_p - Pointer to DLUEContext
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the MCS and RB's of belwe code rate 
 *                  for RAT1 and RAT2
 *****************************************************************************/
/** klockwork Changes Start **/    
void updateMCSRBbelowCoderateForRAT1AndRAT2For4x4Mimo(
                                                DLHARQProcess *harqProcess_p,
                                                /* +- SPR 17777 */
                                                InternalCellIndex internalCellIndex)
{
    UInt8 mcsIndexForTbOne = 0;
    UInt8 mcsIndexForTbTwo = 0;
    UInt8 maxAllocatedRB = harqProcess_p->assignedResourcesNB - 1;
    McsRBInfoForSpatialMultiplex *mcsRBInfoForSpatialMultiplex_p = PNULL;

    if((harqProcess_p->isTBOneValid) && 
            (harqProcess_p->isTBTwoValid))
    {
        mcsIndexForTbOne = harqProcess_p->dlHARQTBOneInfo.mcsIndex;
        mcsIndexForTbTwo = harqProcess_p->dlHARQTBTwoInfo.mcsIndex;
        /* klockwork warning fix start */
        if ( mcsIndexForTbOne > 28 || mcsIndexForTbTwo > 28 )
        {
            return;
        }
        /* klockwork warning fix end */
        if( NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator &&
                NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator &&
                mcsIndexForTbOne < MAX_MCS_INDEX && harqProcess_p->assignedResourcesNB <= MAX_NUM_OF_RB) 

        {   
            if (mcsIndexForTbTwo == 28)
            {
		if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
	        {
                    mcsRBInfoForSpatialMultiplex_p =
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)
                        [LAYER_4_IDX_1][MCS_28_IDX_1]
			[mcsIndexForTbOne][harqProcess_p->assignedResourcesNB - 1]);
		}
        /*coverity 41627*/
		else/* if(harqProcess_p->numOfLayer == RANK_INDICATOR_3)*/
        /*coverity 41627*/
	        {
                    mcsRBInfoForSpatialMultiplex_p =
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)
                         [LAYER_3_IDX_0][MCS_28_IDX_1]
			[mcsIndexForTbOne][harqProcess_p->assignedResourcesNB - 1]);
		}
            }
	    else if ((currentCFIValue_g[internalCellIndex] == 3) && (mcsIndexForTbTwo == 27))
            {
		if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
	        {
                    mcsRBInfoForSpatialMultiplex_p =
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)
                         [LAYER_4_IDX_1][MCS_27_IDX_0]
			[mcsIndexForTbOne][harqProcess_p->assignedResourcesNB - 1]);
		}
        /*coverity 41627*/
		else /*if(harqProcess_p->numOfLayer == RANK_INDICATOR_3)*/
        /*coverity 41627*/
	        {
                    mcsRBInfoForSpatialMultiplex_p =
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)
                         [LAYER_3_IDX_0][MCS_27_IDX_0]
			[mcsIndexForTbOne][harqProcess_p->assignedResourcesNB - 1]);
		}
            }
            else if (mcsIndexForTbOne == 28)
            {
		if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
                    mcsRBInfoForSpatialMultiplex_p =
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)
                         [LAYER_4_IDX_1][MCS_28_IDX_1]
			[mcsIndexForTbTwo][harqProcess_p->assignedResourcesNB - 1]);
        /*coverity 41627*/
		else/* if (harqProcess_p->numOfLayer == RANK_INDICATOR_3)*/
        /*coverity 41627*/
                    mcsRBInfoForSpatialMultiplex_p =
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)
                         [LAYER_3_IDX_0][MCS_28_IDX_1]
			[mcsIndexForTbTwo][harqProcess_p->assignedResourcesNB - 1]);
            }
	    else if ((currentCFIValue_g[internalCellIndex] == 3) && (mcsIndexForTbOne == 27))
            {
                if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
                    mcsRBInfoForSpatialMultiplex_p =
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)
                          [LAYER_4_IDX_1][MCS_27_IDX_0]
			[mcsIndexForTbTwo][harqProcess_p->assignedResourcesNB - 1]);
        /*coverity 41627*/
		else /*if (harqProcess_p->numOfLayer == RANK_INDICATOR_3)*/
        /*coverity 41627*/
                    mcsRBInfoForSpatialMultiplex_p =
                        &((*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)
                         [LAYER_3_IDX_0][MCS_27_IDX_0]
			[mcsIndexForTbTwo][harqProcess_p->assignedResourcesNB - 1]);
            }
            /*coverity 41627*/
            if(mcsRBInfoForSpatialMultiplex_p)
            /*coverity 41627*/
            {
                mcsIndexForTbOne =
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbOneRat1AndRat2;
                mcsIndexForTbTwo = 
                    mcsRBInfoForSpatialMultiplex_p->mcsIndexForTbTwoRat1AndRat2;
                maxAllocatedRB =
                    mcsRBInfoForSpatialMultiplex_p->numberOfresourceBlocksForRat1AndRat2 - 1;

                harqProcess_p->dlHARQTBOneInfo.mcsIndex = mcsIndexForTbOne;
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex = mcsIndexForTbTwo; 
                harqProcess_p->assignedResourcesNB = (maxAllocatedRB + 1);
                if (harqProcess_p->numOfLayer == RANK_INDICATOR_3)
                {
                    harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(
                            harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                            harqProcess_p->assignedResourcesNB,
                            TB_MAPPED_TO_ONE_LAYER);
                }
                else
                {
                    harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(
                            harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                            harqProcess_p->assignedResourcesNB,
                            TB_MAPPED_TO_TWO_LAYER);
                }
                harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(
                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                        harqProcess_p->assignedResourcesNB,
                        TB_MAPPED_TO_TWO_LAYER);
            }
        }
    }
}
/** klockwork Changes End **/    
/* 4x4 DL MIMO CHG END */

/* + TM6_5.2 */
/*****************************************************************************
 * Function Name  : selectRAT2WBPMIIndex
 * Inputs         : subBandReportInfo_p - Pointer to SubBandReportInfo
 * Outputs        : None 
 * Returns        : wideBandPMIIndex
 * Description    : This funtion will check which PMI Index has maximum number of 
 *                  reported PRBs and based on that fill the WB PMI Index.
 *****************************************************************************/


STATIC  UInt32 selectRAT2WBPMIIndex(
                           SubBandReportInfo *subBandReportInfo_p)
{
    SubbandPMIInfo *subbandPMIInfo_p = 
        &(subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo);
    RAT0PmiInfo *rat0PmiInfo_p = &subbandPMIInfo_p->rat0PmiInfo;
    UInt32 wideBandPMIIndex = 0;

    UInt32 pmiReportedSubband_1 = 
                           rat0PmiInfo_p->reportedRBGRank1[0];

    UInt32 foundRBGCntPmi1 = getCount(pmiReportedSubband_1);

    UInt32 pmiReportedSubband_2 = 
                           rat0PmiInfo_p->reportedRBGRank1[1];

    UInt32 foundRBGCntPmi2 = getCount(pmiReportedSubband_2);

    UInt32 pmiReportedSubband_3 = 
                           rat0PmiInfo_p->reportedRBGRank1[2];

    UInt32 foundRBGCntPmi3 = getCount(pmiReportedSubband_3);

    UInt32 pmiReportedSubband_4 = 
                           rat0PmiInfo_p->reportedRBGRank1[3];

    UInt32 foundRBGCntPmi4 = getCount(pmiReportedSubband_4);

    UInt32 maxNumberOfRBG12 = 
        (foundRBGCntPmi1 >= foundRBGCntPmi2)? foundRBGCntPmi1 : foundRBGCntPmi2;
    UInt32 maxNumberOfRBG34 = 
        (foundRBGCntPmi3 >= foundRBGCntPmi4)? foundRBGCntPmi3 : foundRBGCntPmi4;  
    UInt32 maxNumberOfRBG = 
        (maxNumberOfRBG12 >= maxNumberOfRBG34)? maxNumberOfRBG12 : maxNumberOfRBG34;
        
    if (maxNumberOfRBG == foundRBGCntPmi1)
    {
        wideBandPMIIndex = 1;
    }
    else if(maxNumberOfRBG == foundRBGCntPmi2)
    {
        wideBandPMIIndex = 2;
    }
    else if (maxNumberOfRBG == foundRBGCntPmi3)
    {
        wideBandPMIIndex = 3;
    }
    else if(maxNumberOfRBG == foundRBGCntPmi4)
    {
        wideBandPMIIndex = 4;
    }
    return wideBandPMIIndex;
}
/* Cyclomatic Complexity changes - starts  here */
/*****************************************************************************
 * Function Name  : allocateRBRat2LocalizedForSubBand
 * Inputs           dlUEContext_p - Pointer to DLUEContext,
 *                  resourceAllocInput_p - Pointer to ResourceAllocatorInput,
 *                  raPolicyInfo_p - Pointer to ResourceAllocationPolicyInfo,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                  bestVRBSpanInfo_p - Pointer to BestVRBSpanInfo,
 *                  allocatedRBCount_p,
 *                  availableRBCount_p
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None 
 * Returns        : isWidebandAllocationRequired 
 * Description    : This function finds out total RB required to meet maxTBSize
 *                  or close to TBSize if maxTBSize can not be meet due to 
 *                  unavailablility of available RBs.
 *****************************************************************************/

/* +- SPR 17777 */
STATIC UInt8 allocateRBRat2LocalizedForSubBand(
#ifdef HD_FDD_CONFIG
        DLUEContext *dlUEContext_p,
#endif
        /* +- SPR 17777 */
		ResourceAllocatorInput * resourceAllocInput_p,
		ResourceAllocationPolicyInfo *raPolicyInfo_p,
                DLHARQProcess * harqProcess_p,
                DLStrategyTxNode *dlStrategyTxNode_p,
                BestVRBSpanInfo *bestVRBSpanInfo_p,
                UInt32 *allocatedRBCount_p,
                UInt32 *availableRBCount_p,
                InternalCellIndex internalCellIndex)
{
    RAT0NonSpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
    UInt32 cqiIndexCount = 0;
    UInt32 cqiBitMap = 0;
    UInt16 reportedCQIBitMap = 0;
    UInt8  cqiReportIndex = 0;  

    UInt8 reportedMCSIndex = 0;
    UInt8 minRBs = 0;
    UInt8 maxRBs = 0;
    ResourceFormatNode previousRsrcFrmt = {0};
    UInt32 foundTBSize = 0;            
    UInt32 totalRequiredRBs  = 0;
    /* ICIC changes start */
    /* Cyclomatic Complexity changes - starts  here */
    UInt32 requiredRBCount = 0;
    UInt32 resourceRegionAllocated = 
        resourceAllocInput_p->resourceRegionAllocated;

    requiredRBCount = resourceAllocInput_p->requiredRB;
    UInt32 minTBSize = dlStrategyTxNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyTxNode_p->maxTBSize;
    UInt8 isWidebandAllocationRequired = 1;
    /* Cyclomatic Complexity changes - ends  here */

    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
#endif
    /*HD FDD Changes End*/

    rat0CQIInfo_p = &resourceAllocInput_p->subBandReportInfo_p->cqiInfo.cqiInfoNonSpatialMul.
        rat0NonSpatialMulCQIInfo;    

    reportedCQIBitMap = rat0CQIInfo_p->reportedCQIBitMap;

    while (reportedCQIBitMap)
    {
        /* find out first reported CQI */
        cqiReportIndex = FFS(reportedCQIBitMap);
        reportedCQIBitMap &= (~((UInt16)1 << (cqiReportIndex-1)));
        /*klockworks warning fix*/
        if(cqiReportIndex > 0)
        {
            cqiIndexCount = (MAX_CQI_INDEX - cqiReportIndex); 
	}

        reportedMCSIndex = getMCSFromCQI(cqiIndexCount
                                );


        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
        /* FILL the MCS index value from CQI codeword*/
        HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currTick,dlUEContext_p, reportedMCSIndex);
#endif
        /*HD FDD Changes End*/

        /*start with the highest reported CQI Index*/
        cqiBitMap = rat0CQIInfo_p->rat0CQIBitMap[cqiIndexCount];

        /*min and max RBs required at reported CQI for min and max TB size as instructed 
          by DL Strategy*/
        /* SPR 5999 changes start */
        minRBs = DETERMINE_RBs_DL(reportedMCSIndex, minTBSize, internalCellIndex);
        maxRBs = DETERMINE_RBs_DL(reportedMCSIndex, maxTBSize, internalCellIndex);
        /* SPR 5999 changes end */

        if (1 == maxRBs)
        {
            /* ICIC changes start */
            if( MAC_SUCCESS == getFirstAvailableReportedVRBNewTx(
                        bestVRBSpanInfo_p, 
                        &raPolicyInfo_p->rat2LocalizedInfo,                                                                                                                                        
                        cqiBitMap,
                        resourceRegionAllocated))
                /* ICIC changes end */
            {
                *allocatedRBCount_p = 1;

                /*as soon as allocation from Subband is done, mark wideband allocation 
                  to false*/
                isWidebandAllocationRequired = 0;
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = reportedMCSIndex;
                /* ICIC changes start */
                UPDATE_EXCESS_POOL_RB(resourceRegionAllocated,
                        (requiredRBCount - *allocatedRBCount_p),
                        internalCellIndex);
                /* ICIC changes end */
                break;
            }
            else
            {
                continue;
            }
        }
        else 
        {
            /*check min/maxRBs against allocated RBs to findout extra required RBs 
              from excessPool*/
            totalRequiredRBs = requiredRBCount + excessPoolRb_g[internalCellIndex][resourceRegionAllocated];
            if (minRBs <= requiredRBCount)
            {
                if (maxRBs > requiredRBCount)
                {
                    maxRBs = requiredRBCount;
                }                    
            }
            else
            {
                if (minRBs > totalRequiredRBs)
                {
                    minRBs = totalRequiredRBs;
                }
                maxRBs = minRBs;
            }

            /*for each reported CQI and their corresponding reported subbands try to find out
              contiguous VRBs which are available and reported too*/                
            /* ICIC changes start */
            if( MAC_SUCCESS == getReportedLocalizedVRBNewTx(
                        bestVRBSpanInfo_p, 
                        &raPolicyInfo_p->rat2LocalizedInfo,
                        minRBs,
                        maxRBs,
                        cqiBitMap,
                        resourceRegionAllocated))
                /* ICIC changes end */
            {
                *availableRBCount_p = (bestVRBSpanInfo_p->endingVRBIndex - 
                        bestVRBSpanInfo_p->startingVRBIndex) + 1; 

                if (*availableRBCount_p >= minRBs)
                {                        
                    *allocatedRBCount_p = *availableRBCount_p;

                    /*as soon as allocation from Subband is done, mark wideband allocation 
                      to false*/
                    isWidebandAllocationRequired = 0;
                    harqProcess_p->dlHARQTBOneInfo.mcsIndex = reportedMCSIndex;
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated,
                            (requiredRBCount - *allocatedRBCount_p),
                            internalCellIndex);
                    /* ICIC changes end */
                    break;
                }
                else
                {
                    /*store this found TB Size later on it will be used, if minRBs are not satisfied*/
                    foundTBSize = determineTBSize(reportedMCSIndex, *availableRBCount_p,
                                                  /*4x4 DL MIMO CHG START*/
                                                  TB_MAPPED_TO_ONE_LAYER 
                                                  /*4x4 DL MIMO CHG END*/
                                                    );
                    if (previousRsrcFrmt.tbSize < foundTBSize)
                    {
                        previousRsrcFrmt.tbSize = foundTBSize;
                        previousRsrcFrmt.mcsIndex = reportedMCSIndex;
                        previousRsrcFrmt.prbs = *availableRBCount_p;
                        previousRsrcFrmt.startVRBIndex = bestVRBSpanInfo_p->startingVRBIndex;
                    }
                }
            }
        }  
  }
    return isWidebandAllocationRequired;
}
/*****************************************************************************
 * Function Name  : allocateRBRat0NonSpatialForSubBand 
 * Inputs         : dlUEContext_p - Pointer to DLUEContext,
 *                  resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                  raPolicy_p - Pointer to ResourceAllocationPolicyInfo,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                  allocatedRb_p,
 *                  availableRBCount,
 *                  availableRBGBitMap,
 *                  subFrame(TDD),
 *                  tempRBGBitMap
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None  
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function finds out total RB required to meet maxTBSize
 *                  or close to TBSize if maxTBSize can not be meet due to 
 *                  unavailablility of available RBs.
 *****************************************************************************/
STATIC  MacRetType allocateRBRat0NonSpatialForSubBand(DLUEContext *dlUEContext_p,
		ResourceAllocatorInput * resrcAllocatorInput_p,
		ResourceAllocationPolicyInfo *raPolicy_p,
                DLHARQProcess *harqProcess_p,
                DLStrategyTxNode *dlStrategyTxNode_p,
                UInt32 *allocatedRb_p,
                UInt32 availableRBCount,
                UInt32 availableRBGBitMap
#ifdef TDD_CONFIG
                ,UInt8 subFrame
#endif 
                ,UInt32 *tempRBGBitMap
                ,InternalCellIndex internalCellIndex)
{
   /* Cyclomatic Complexity Changes - starts here */
    RAT0Info *rat0Info_p = PNULL;
    rat0Info_p = &raPolicy_p->rat0Info;
    /* Fill the MCS Index CodeWord1 from the DL UE Context */
    UInt32 widebandMCSIndex = 0;
    /*spr 15832 +*/
    if(IS_PCELL_OF_UE(dlUEContext_p,internalCellIndex))
    {
        widebandMCSIndex = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordOne; 
    }
    else
    {
        widebandMCSIndex = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordOne;
    }
    /*spr 15832 -*/
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
    HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(currTick, dlUEContext_p, widebandMCSIndex);
#endif
    /*HD FDD Changes End*/
    /* ICIC changes start */
    UInt32 resourceRegionAllocated = 
        resrcAllocatorInput_p->resourceRegionAllocated;
    /* ICIC changes end */
#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe =checkForSpecialSubframe(subFrame, internalCellIndex);
#endif
   /* Cyclomatic Complexity Changes - ends here */

        if ( NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator )
        {
            /* check from all  reported CQI index to achieve the min/max TB Size which 
                        can be allocated to this UE based on preferred subbands*/
            *tempRBGBitMap = getRAT0BitMapNewTxNonSpatialMux(
                                                resrcAllocatorInput_p,
                                                harqProcess_p,
                                                dlStrategyTxNode_p,
                                                rat0Info_p
						                        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                                                ,dlUEContext_p
#endif
	                                            /*HD FDD Changes End*/
                                                ,internalCellIndex
                                                );
            if (!*tempRBGBitMap)
            {
                /* As of now, there is no use case for strictAllocSubBand
                 * strictAllocSubBand check is commented */
            
                /* Allocate as Wide Band */   
                if (availableRBCount >= resrcAllocatorInput_p->requiredRB)
                {
                    /*Now we are sure that minTBSize would be satisfied as availableRBs are greater than 
                      required. Based on excessPool try to allocate maxTBSize*/
                    *allocatedRb_p = resrcAllocatorInput_p->requiredRB;
                }
                else
                {
                    /* We can not allocate less than minTBSize if strictScheduled is instructed */
                    if (dlStrategyTxNode_p->strictAllocatedFlag)
                    {
                        /* ICIC changes start */
                        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                                (resrcAllocatorInput_p->requiredRB),
                                internalCellIndex);
                        /* ICIC changes end */
                        return MAC_FAILURE;
                    }
                    *allocatedRb_p = availableRBCount;
                    //update excessPool but it seems that it is the last UE as available resources are less than required.
                    /* ICIC changes start */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            (resrcAllocatorInput_p->requiredRB - *allocatedRb_p),
                            internalCellIndex);
                    /* ICIC changes end */
                }
                
                *tempRBGBitMap = availableRBGBitMap;
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = widebandMCSIndex;             
                harqProcess_p->assignedResourcesNB = *allocatedRb_p;
                
                LOG_MAC_MSG(MAC_SB_CQI_FAILURE_LOG_ID,LOGDEBUG,
                        MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                        (UInt32)availableRBCount,
                        (UInt32)resrcAllocatorInput_p->requiredRB,
                       *tempRBGBitMap,*allocatedRb_p,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"SB_FAIL_NWTX");
            }
            else
            {
                *allocatedRb_p = harqProcess_p->assignedResourcesNB;

                LOG_MAC_MSG(MAC_SB_CQI_SUCCESS_LOG_ID,LOGDEBUG,
                       MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
                       (UInt32)availableRBCount,
                       (UInt32)resrcAllocatorInput_p->requiredRB,
                       *tempRBGBitMap,*allocatedRb_p,
                       DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                       FUNCTION_NAME,"SB_PASS_NWTX");
            }

            /*In case of New Transmission set the Transport block size in HARQ TB considering the fact
                        that this could be special subframe in case of TDD*/
            /* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
            setTbSizeBasedOnSplSubframe(harqProcess_p,
                                             *allocatedRb_p);
#elif TDD_CONFIG
            setTbSizeBasedOnSplSubframe(harqProcess_p,
                                             *allocatedRb_p,
                                             isSpecialSubframe);
#endif
            /* Cyclomatic_complexity_changes_end */
        }
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : allocateRBRat2Tb1ValidTb2InvalidForSubBand 
 * Inputs         : dlUEContext_p - Pointer to DLUEContext,
 *                  resourceAllocInput_p - Pointer to ResourceAllocatorInput,
 *                  raPolicyInfo_p - Pointer to ResourceAllocationPolicyInfo,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  bestVRBSpanInfo_p - Pointer to BestVRBSpanInfo,
 *                  dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                  allocatedRBCount_p
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : isWidebandAllocationRequired 
 * Description    : This funtion allocate the RBs according to TB 1 and TB2 for New 
 *                    Transmission and Re Transmission accordingly
 *****************************************************************************/
STATIC UInt8 allocateRBRat2Tb1ValidTb2InvalidForSubBand(DLUEContext *dlUEContext_p,
                                                 ResourceAllocatorInput *resourceAllocInput_p,
                                                 ResourceAllocationPolicyInfo *raPolicyInfo_p,
                                                 DLHARQProcess *harqProcess_p,
                                                 BestVRBSpanInfo *bestVRBSpanInfo_p,
                                                  DLStrategyTxNode *dlStrategyTxNode_p,
                                                 UInt32 * allocatedRBCount_p,
                                                 InternalCellIndex internalCellIndex
                                                    )
{
	RAT0SpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
        
	UInt32 cqiBitMap = 0;
	UInt32 reportedMCSIndex = 0;
	UInt8 minRBs = 0;
	UInt8 maxRBs = 0;
    UInt32 availableRBCount = 0;
	ResourceFormatNode previousRsrcFrmt = {0};
	UInt32 foundTBSize = 0;            
	UInt32 totalRequiredRBs  = 0;
    /* Cyclomatic Complexity Changes -starts here */
    UInt32 cntModScheme = MOD_SCHEME_CNT;
    UInt32 requiredRBCount = 0;
	UInt32 modSchemeAndAvailableBitmap[MOD_SCHEME_CNT] = {0};
    UInt32 minTBSize = dlStrategyTxNode_p->minTBSize;
    UInt32 maxTBSize = dlStrategyTxNode_p->maxTBSize;
    UInt8 widebandPMI = dlUEContext_p->dlCQIInfo.wideBandPMIIndex;
    UInt8 isWidebandAllocationRequired = 1;
#ifdef HD_FDD_CONFIG                      
	/* HD FDD Changes Start */
    /* SPR 15909 fix start */
	tickType_t  tick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
#endif
	/* ICIC changes start */
   UInt8 resourceRegionAllocated = resourceAllocInput_p->resourceRegionAllocated;
   requiredRBCount = resourceAllocInput_p->requiredRB;
        
        /* Cyclomatic Complexity Changes -starts here */

	RAT0Info *rat0Info_p = PNULL;
	rat0Info_p = &raPolicyInfo_p->rat0Info;

	rat0CQIInfo_p = 
		&resourceAllocInput_p->subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.rat0SpatialMulCQIInfo;

	modSchemeAndAvailableBitmap[0] = rat0CQIInfo_p->rat0QPSKCQIBitMapCW1;
	modSchemeAndAvailableBitmap[1] = rat0CQIInfo_p->rat016QAMCQIBitMapCW1;
	modSchemeAndAvailableBitmap[2] = rat0CQIInfo_p->rat064QAMCQIBitMapCW1;   

	/*store 64QAM and available bitmap at MOD_SCHEME_CNT in order to get it first using while (i--) */
	while (cntModScheme--)
	{
		if ((cqiBitMap = modSchemeAndAvailableBitmap[cntModScheme]))
		{
			/* + TM6_5.2 */ 
			harqProcess_p->pmiIndex = widebandPMI;                
			/* - TM6_5.2 */ 

			/*Get Mcs Index of CodeWord One*/
			getMcsIndexForCW1(&reportedMCSIndex, cqiBitMap,resourceAllocInput_p->subBandReportInfo_p, 
					((requiredRBCount - 1)/rat0Info_p->rbgSize) + 1);
			/*min and max RBs required at reported CQI for min and max TB size as instructed 
			  by DL Strategy*/
#ifdef HD_FDD_CONFIG                      
			HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(tick, dlUEContext_p, reportedMCSIndex);
#endif

			/* HD FDD Changes End */
			/* SPR 5999 changes start */
			minRBs = DETERMINE_RBs_DL(reportedMCSIndex, minTBSize, internalCellIndex);
			maxRBs = DETERMINE_RBs_DL(reportedMCSIndex, maxTBSize, internalCellIndex);
			/* SPR 5999 changes end */

			if (1 == maxRBs)
			{
				/* ICIC changes start */
				if( MAC_SUCCESS == getFirstAvailableReportedVRBNewTx(
							bestVRBSpanInfo_p, 
							&raPolicyInfo_p->rat2LocalizedInfo,                                                                                                                                        
							cqiBitMap,
							resourceRegionAllocated))
					/* ICIC changes end */
				{
					*allocatedRBCount_p = 1;

					/*as soon as allocation from Subband is done, mark wideband allocation 
					  to false*/
					isWidebandAllocationRequired = 0;
					harqProcess_p->dlHARQTBOneInfo.mcsIndex = reportedMCSIndex;
					/* ICIC changes start */
					UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
							(requiredRBCount - *allocatedRBCount_p),
                            internalCellIndex);
					/* ICIC changes end */
					break;
				}
				else
				{
					continue;
				}
			}
			else 
			{
				/*check min/maxRBs against allocated RBs to findout extra required RBs 
				  from excessPool*/
				totalRequiredRBs = requiredRBCount + excessPoolRb_g[internalCellIndex][resourceRegionAllocated];
				if (minRBs <= requiredRBCount)
				{
					if (maxRBs > requiredRBCount)
					{
						maxRBs = requiredRBCount;
					}                    
				}
				else
				{
					if (minRBs > totalRequiredRBs)
					{
						minRBs = totalRequiredRBs;
					}
					maxRBs = minRBs;

				}

				/*for each reported CQI and their corresponding reported subbands try to find out
				  contiguous VRBs which are available and reported too*/                
				/* ICIC changes start */
				if( MAC_SUCCESS == getReportedLocalizedVRBNewTx(
							bestVRBSpanInfo_p, 
							&raPolicyInfo_p->rat2LocalizedInfo,
							minRBs,
							maxRBs,
							cqiBitMap,
							resourceRegionAllocated))
					/* ICIC changes end */
				{
					availableRBCount = (bestVRBSpanInfo_p->endingVRBIndex - 
							bestVRBSpanInfo_p->startingVRBIndex) + 1; 

					if (availableRBCount >= minRBs)
					{                        
						*allocatedRBCount_p = availableRBCount;

						/*as soon as allocation from Subband is done, mark wideband allocation 
						  to false*/
						isWidebandAllocationRequired = 0;
						harqProcess_p->dlHARQTBOneInfo.mcsIndex = reportedMCSIndex;
						/* ICIC changes start */
						UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
								(requiredRBCount - *allocatedRBCount_p),
                                internalCellIndex);
						/* ICIC changes end */
						break;
					}
					else
					{
						/*store this found TB Size later on it will be used, if minRBs are not satisfied*/
						foundTBSize = determineTBSize(reportedMCSIndex, availableRBCount ,
                                                                              /*4x4 DL MIMO CHG START*/
                                                                              TB_MAPPED_TO_ONE_LAYER
                                                                              /*4x4 DL MIMO CHG END*/);
						if (previousRsrcFrmt.tbSize < foundTBSize)
						{
							previousRsrcFrmt.tbSize = foundTBSize;
							previousRsrcFrmt.mcsIndex = reportedMCSIndex;
							previousRsrcFrmt.prbs = availableRBCount;
							previousRsrcFrmt.startVRBIndex = bestVRBSpanInfo_p->startingVRBIndex;
						}
					}
				}
                /* TDD SB_CQI */
                else
                {  
                    continue;
                }
                /* TDD SB_CQI */
            }

        }  
    }
    /* TDD SB_CQI */
    if(0 == *allocatedRBCount_p)
    {
        /* Sanity CQI Fix */
        return isWidebandAllocationRequired;
    }            
    /* TDD SB_CQI */

    if (isWidebandAllocationRequired && !dlStrategyTxNode_p->strictAllocatedFlag)
    {
	/* As of now, there is no use case for strictAllocSubBand
	 * strictAllocSubBand check is commented */
        /* Bug 13921 Changes Starts */
        UInt8 endIndex = previousRsrcFrmt.startVRBIndex + (previousRsrcFrmt.prbs - 1) ;

        if((0 !=  previousRsrcFrmt.startVRBIndex) && (0 != endIndex))
        { 
            /* Bug 13921 Changes Ends */
            *allocatedRBCount_p = previousRsrcFrmt.prbs;

            /* coverity-530 CID 32864 */
            if(0 == *allocatedRBCount_p)
            {
                lteWarning(" Value of *allocatedRBCount_p is zero [%s] \n",__func__,*allocatedRBCount_p);
                return isWidebandAllocationRequired;
            }
            /* coverity-530 CID 32864 */
            harqProcess_p->dlHARQTBOneInfo.mcsIndex = previousRsrcFrmt.mcsIndex;

            bestVRBSpanInfo_p->startingVRBIndex = previousRsrcFrmt.startVRBIndex;
            bestVRBSpanInfo_p->endingVRBIndex = 
                /* coverity-530 CID 32864 */
                previousRsrcFrmt.startVRBIndex + ((*allocatedRBCount_p) - 1) ;
            /* coverity-530 CID 32864 */
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                    (requiredRBCount - *allocatedRBCount_p),
                     internalCellIndex);
            /* ICIC changes end */
            isWidebandAllocationRequired = 0;
        } 
    }
    /* coverity fix-70268 start */
    return isWidebandAllocationRequired; 
    /* coverity fix-70268 end */
}
/*****************************************************************************
 * Function Name  : getRAT0BitMapFromSubBand 
 * Inputs         : rat0Ino_p - Pointer to RAT0Info,
 *                  resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  dlStrategyNode_p - Pointer to DLStrategyTxNode,
 *                  previousRsrcFrmt_p - Pointer to ResourceFormatMIMONode,
 *                  centralRbMask,
 *                  transmissionMode - Mode of transmission,
 *                  scheduledTick - Scheduled TTI
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : RBG bitmap 
 * Description    : This funtion process and  return the RBG BitMap 
 *****************************************************************************/
STATIC UInt32 getRAT0BitMapFromSubBand (RAT0Info *rat0Info_p,
                                ResourceAllocatorInput *resrcAllocatorInput_p,
                                DLHARQProcess  *harqProcess_p,
                                DLStrategyTxNode *dlStrategyNode_p,
                                ResourceFormatMIMONode * previousRsrcFrmt_p,
                                UInt32 centralRbMask,
                                UInt8 transmissionMode,
                                /* SPR 15909 fix start */
                                tickType_t scheduledTick,
                                /* SPR 15909 fix end */
                                InternalCellIndex internalCellIndex)
{
    RAT0SpatialMulCQIInfo *rat0SpatialMulCQIInfo_p = PNULL;
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Entering funtion = %s\n",__func__);
    SubBandReportInfo *subBandReportInfo_p = resrcAllocatorInput_p->subBandReportInfo_p;
    SubbandPMIInfo *subbandPMIInfo_p = PNULL;

    rat0SpatialMulCQIInfo_p = &subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.
        rat0SpatialMulCQIInfo;
    subbandPMIInfo_p = &subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo;
    /* We will start from  that CodeWord SubBand BitMap whose number of 64QAM 
       Bands reported are more*/
    /*SPR 19631 +*/
    /*Get the bitmap only when atleast count of CW1 or CW2 is non-zero*/
    if(rat0SpatialMulCQIInfo_p->count64QAMSubBandCW1 != 0 || rat0SpatialMulCQIInfo_p->count64QAMSubBandCW2 !=0)
    {
        if ( rat0SpatialMulCQIInfo_p->count64QAMSubBandCW1 >= 
                rat0SpatialMulCQIInfo_p->count64QAMSubBandCW2 )
        {
            /* Cyclomatic Complexity Changes -starts here */
            return getRAT0BitMap(rat0SpatialMulCQIInfo_p->rat064QAMCQIBitMapCW1,
                    rat0SpatialMulCQIInfo_p->rat064QAMCQIBitMapCW2,
                    &subbandPMIInfo_p->rat0PmiInfo,
                    rat0Info_p,
                    resrcAllocatorInput_p,
                    harqProcess_p,
                    previousRsrcFrmt_p,
                    dlStrategyNode_p, 
                    centralRbMask,
                    transmissionMode,
                    scheduledTick,
                    internalCellIndex);
            /* Cyclomatic Complexity Changes -ends here */

        }
        else if (rat0SpatialMulCQIInfo_p->count64QAMSubBandCW1 <
                rat0SpatialMulCQIInfo_p->count64QAMSubBandCW2)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "count64QAMSubBandCW1 < count64QAMSubBandCW2");
            /* Cyclomatic Complexity Changes -starts here */
            return getRAT0BitMap(rat0SpatialMulCQIInfo_p->rat064QAMCQIBitMapCW2,
                    rat0SpatialMulCQIInfo_p->rat064QAMCQIBitMapCW1,
                    &subbandPMIInfo_p->rat0PmiInfo,
                    rat0Info_p,
                    resrcAllocatorInput_p,
                    harqProcess_p,
                    previousRsrcFrmt_p,
                    dlStrategyNode_p,
                    centralRbMask,
                    transmissionMode,
                    scheduledTick ,
                    internalCellIndex);
            /* Cyclomatic Complexity Changes -ends here */
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Start Searching in 16QAM SubBand\n");
    /* We will start from  that CodeWord SubBand BitMap whose number of 16QAM 
       Bands reported are more*/
    if(rat0SpatialMulCQIInfo_p->count16QAMSubBandCW1 != 0 || rat0SpatialMulCQIInfo_p->count16QAMSubBandCW2 !=0)
    {
        if ( rat0SpatialMulCQIInfo_p->count16QAMSubBandCW1 >= 
                rat0SpatialMulCQIInfo_p->count16QAMSubBandCW2 )
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "count16QAMSubBandCW1 > count16QAMSubBandCW2");
            /* Cyclomatic Complexity Changes -starts here */
            return getRAT0BitMap(rat0SpatialMulCQIInfo_p->rat016QAMCQIBitMapCW1,
                    rat0SpatialMulCQIInfo_p->rat016QAMCQIBitMapCW2,
                    &subbandPMIInfo_p->rat0PmiInfo,
                    rat0Info_p,
                    resrcAllocatorInput_p,
                    harqProcess_p,
                    previousRsrcFrmt_p,
                    dlStrategyNode_p, 
                    centralRbMask,
                    transmissionMode,
                    scheduledTick,
                    internalCellIndex);
            /* Cyclomatic Complexity Changes -ends here */
        }
        else if (rat0SpatialMulCQIInfo_p->count16QAMSubBandCW1 <
                rat0SpatialMulCQIInfo_p->count16QAMSubBandCW2)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "count16QAMSubBandCW1 < count16QAMSubBandCW2");
            /* Cyclomatic Complexity Changes -starts here */
            return getRAT0BitMap(rat0SpatialMulCQIInfo_p->rat016QAMCQIBitMapCW2,
                    rat0SpatialMulCQIInfo_p->rat016QAMCQIBitMapCW1,
                    &subbandPMIInfo_p->rat0PmiInfo,
                    rat0Info_p,
                    resrcAllocatorInput_p,
                    harqProcess_p,
                    previousRsrcFrmt_p,
                    dlStrategyNode_p,    
                    centralRbMask,
                    transmissionMode,
                    scheduledTick,
                    internalCellIndex);
            /* Cyclomatic Complexity Changes -ends here */
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
            "Start Searching in QPSK SubBand\n");
    /* We will start from  that CodeWord SubBand BitMap whose number of QPSK 
       Bands reported are more*/
    if(rat0SpatialMulCQIInfo_p->countQPSKSubBandCW1 != 0 || rat0SpatialMulCQIInfo_p->countQPSKSubBandCW2 !=0) 
    {
        if ( rat0SpatialMulCQIInfo_p->countQPSKSubBandCW1 >= 
                rat0SpatialMulCQIInfo_p->countQPSKSubBandCW2 )
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "countQPSKSubBandCW1 > countQPSKSubBandCW2");
            /* Cyclomatic Complexity Changes -starts here */
            return getRAT0BitMap(rat0SpatialMulCQIInfo_p->rat0QPSKCQIBitMapCW1,
                    rat0SpatialMulCQIInfo_p->rat0QPSKCQIBitMapCW2,
                    &subbandPMIInfo_p->rat0PmiInfo,
                    rat0Info_p,
                    resrcAllocatorInput_p,
                    harqProcess_p,
                    previousRsrcFrmt_p,
                    dlStrategyNode_p, 
                    centralRbMask,
                    transmissionMode,
                    scheduledTick,
                    internalCellIndex);
            /* Cyclomatic Complexity Changes -ends here */
        }
        else if (rat0SpatialMulCQIInfo_p->countQPSKSubBandCW1 <
                rat0SpatialMulCQIInfo_p->countQPSKSubBandCW2)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "rat0QPSKCQIBitMapCW1 < rat0QPSKCQIBitMapCW2");
            /* Cyclomatic Complexity Changes -starts here */
            return getRAT0BitMap(rat0SpatialMulCQIInfo_p->rat0QPSKCQIBitMapCW2,
                    rat0SpatialMulCQIInfo_p->rat0QPSKCQIBitMapCW1,
                    &subbandPMIInfo_p->rat0PmiInfo,
                    rat0Info_p,
                    resrcAllocatorInput_p,
                    harqProcess_p,
                    previousRsrcFrmt_p,
                    dlStrategyNode_p, 
                    centralRbMask,
                    transmissionMode,
                    scheduledTick,
                    internalCellIndex);
            /* Cyclomatic Complexity Changes -ends here */
        }
    }
    /*SPR 19631 -*/

    return 0;

}
/*****************************************************************************
 * Function Name  : allocRBInRAT0SpatialForSubband 
 * Inputs         : dlUEContext_p - Pointer to DLUEContext,
 *                  dlStrategyTxNode_p - Pointer toDLStrategyTxNode,
 *                  harqProcess_p - Pointer toDLHARQProcess,
 *                  resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                  raPolicy_p - Pointer to ResourceAllocationPolicyInfo,
 *                  tempRBGBitMap_p,
 *                  availableRBCount,
 *                  allocatedRb_p,
 *                  centralRbMask,
 *                  transmissionMode - Mode of Transmission,
 *                  scheduledTick - Scheduled TTI
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function allocate the RB's and TB size based on 
 *                  special subframe for RAT0
 *****************************************************************************/
STATIC MacRetType allocRBInRAT0SpatialForSubband (DLUEContext * dlUEContext_p,
     DLStrategyTxNode *dlStrategyTxNode_p,
     DLHARQProcess *harqProcess_p, 
     ResourceAllocatorInput  * resrcAllocatorInput_p,
     ResourceAllocationPolicyInfo *raPolicy_p,
     UInt32 * tempRBGBitMap_p,
     UInt32 availableRBCount,
     SInt8 * allocatedRb_p,
     UInt32 centralRbMask,
     UInt8  transmissionMode,
     /* SPR 15909 fix start */
     tickType_t scheduledTick
     /* SPR 15909 fix end */
#ifdef TDD_CONFIG
   /*coverity-530-32797*/
     ,UInt8 subframe
   /*coverity-530-32797*/
#endif 
     ,InternalCellIndex internalCellIndex
)
{
    DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
    DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL;
    RAT0Info *rat0Info_p = PNULL;
    
    /*spr 15832 +*/
    UInt8 widebandMCSIndexCW1 = INVALID_MCS_INDEX;
    UInt8 widebandMCSIndexCW2 = INVALID_MCS_INDEX;
    /*spr 15832 -*/
    UInt8 widebandPMI = dlUEContext_p->dlCQIInfo.wideBandPMIIndex;
    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo;
    rat0Info_p = &raPolicy_p->rat0Info;
    /* ICIC changes start */
    UInt32 resourceRegionAllocated =  
        resrcAllocatorInput_p->resourceRegionAllocated;
    /* ICIC changes end */

    UInt32 availableRBGBitMap = rat0Info_p->rat0BitMap[CC_USER] & 
        rat0Info_p->rat0BitMap[resourceRegionAllocated];

    /*spr 15832 +*/
    if(IS_PCELL_OF_UE(dlUEContext_p,internalCellIndex))
    { 
        widebandMCSIndexCW1 = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordOne;
        widebandMCSIndexCW2 = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
    }   
    else
    {
        widebandMCSIndexCW1 = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordOne;
        widebandMCSIndexCW2 = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordTwo;
    }    
    /*spr 15832 -*/
   /*coverity-530-32797*/
#ifdef TDD_CONFIG
   UInt8 isSpecialSubframe =checkForSpecialSubframe(subframe, internalCellIndex);
#endif 
   /*coverity-530-32797*/
   if ( (NEW_TX == dlHARQTBOneInfo_p->txIndicator) ||
        	(NEW_TX == dlHARQTBTwoInfo_p->txIndicator) ) 
   {
	   *tempRBGBitMap_p = getRAT0BitMapSpatialMux(
			   harqProcess_p,
			   resrcAllocatorInput_p,
			   rat0Info_p,
			   dlStrategyTxNode_p,
                /* + TM7_8 Changes Start */
                centralRbMask,
                transmissionMode,
                scheduledTick,
                internalCellIndex);
                /* - TM7_8 Changes End */

	   if (!(*tempRBGBitMap_p))
	   {
		   LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
				   "strictAllocSubBand= %u \n", resrcAllocatorInput_p->strictAllocSubBand);

		   /* As of now, there is no use case for strictAllocSubBand
		    * strictAllocSubBand check is commented */

		   /* Allocate as Wide Band */   
		   if (availableRBCount >= resrcAllocatorInput_p->requiredRB)
		   {
			   *allocatedRb_p = resrcAllocatorInput_p->requiredRB;
		   }
		   else
		   {
			   /* We can not allocate less than minTBSize if strictScheduled is instructed */
			   if (dlStrategyTxNode_p->strictAllocatedFlag)
			   {
				   /* ICIC changes start */
				   UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
						   resrcAllocatorInput_p->requiredRB,
                           internalCellIndex);
				   /* ICIC changes end */
				   return MAC_FAILURE;
			   }
			   *allocatedRb_p = availableRBCount;

			   //update excessPool but it seems that it is the last UE as available resources are less than required.
			   /* ICIC changes start */
			   UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
					   (resrcAllocatorInput_p->requiredRB - *allocatedRb_p),
                       internalCellIndex);
			   /* ICIC changes end */
		   }
		   harqProcess_p->assignedResourcesNB = *allocatedRb_p;                       
		   *tempRBGBitMap_p = availableRBGBitMap;
		   dlHARQTBOneInfo_p->mcsIndex = widebandMCSIndexCW1;
		   dlHARQTBTwoInfo_p->mcsIndex = widebandMCSIndexCW2;
		   harqProcess_p->pmiIndex = widebandPMI;


		   LOG_MAC_MSG(MAC_SB_CQI_FAILURE_LOG_ID,LOGDEBUG,
				   MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
				   (UInt32)availableRBCount,
				   (UInt32)resrcAllocatorInput_p->requiredRB,
				   *tempRBGBitMap_p,*allocatedRb_p,
				   DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
				   FUNCTION_NAME,"SB_FAIL_NWTX");
	   }
	   else
	   {
		   *allocatedRb_p = harqProcess_p->assignedResourcesNB;

		   if (WIDEBAND_PMI == harqProcess_p->pmiIndex)
		   {
			   harqProcess_p->pmiIndex = widebandPMI;
		   }

		   LOG_MAC_MSG(MAC_SB_CQI_SUCCESS_LOG_ID,LOGDEBUG,
				   MAC_PDSCH,getCurrentTick(),(UInt32)dlUEContext_p->ueIndex,
				   (UInt32)availableRBCount,
				   (UInt32)resrcAllocatorInput_p->requiredRB,
				   *tempRBGBitMap_p,*allocatedRb_p,
				   DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
				   FUNCTION_NAME,"SB_PASS_NWTX");
	   }    

	   /*In case of New Transmission set the Transport block size in HARQ TB considering the fact
	     that this could be special subframe in case of TDD*/
#ifdef FDD_CONFIG
	   SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
			   *allocatedRb_p);
#elif TDD_CONFIG
	   SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
			   *allocatedRb_p,
			   isSpecialSubframe);
#endif
   }
     return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : updateRAT0SubBandPMIInfo 
 * Inputs         : dlUEContext_p - Pointer to DLUEContext,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                  resrcAllocatorOutput_p - Pointer to ResourceAllocatorOutput,
 *                  availableRBGBitMap,
 *                  tempRBGBitMap,
 *                  rbsAllocated,
 *                  subframe
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None 
 * Returns        : None
 * Description    : This function updates the suubband PMI information for RAT0 
 *****************************************************************************/

STATIC void updateRAT0SubBandPMIInfo(DLUEContext * dlUEContext_p,
          DLHARQProcess * harqProcess_p,
          ResourceAllocatorInput *resrcAllocatorInput_p,
          ResourceAllocatorOutput *resrcAllocatorOutput_p,
          UInt32 availableRBGBitMap,
          UInt32 tempRBGBitMap,
          UInt32 rbsAllocated
#ifdef TDD_CONFIG
         /*coverity-530-32797*/
         , UInt8 subframe
         /*coverity-530-32797*/
#endif
         ,InternalCellIndex internalCellIndex
              )
{

   DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
   DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL;

   UInt8 widebandPMI = 0;

    /* ICIC changes start */
    UInt32 resourceRegionAllocated =  
        resrcAllocatorInput_p->resourceRegionAllocated;
    /* ICIC changes end */
    dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo;

    /*coverity-530-32797*/
#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe =checkForSpecialSubframe(subframe, internalCellIndex);
#endif
    /*coverity-530-32797*/
   
    if ( (NEW_TX == dlHARQTBOneInfo_p->txIndicator) ||
            (NEW_TX == dlHARQTBTwoInfo_p->txIndicator))
    {
	/* Coverity 5.0.1 ID 27771 Fix Start */
        if((resrcAllocatorInput_p->subBandReportInfo_p) &&(TRUE == resrcAllocatorInput_p->aperiodicMode12)&&
            /* + TM7_8 Changes Start */
           (!(TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode)))
            /* - TM7_8 Changes End */
        {
            if( (harqProcess_p->isTBOneValid && harqProcess_p->isTBTwoValid)
                    && (2 != harqProcess_p->pmiIndex) )
            {
                widebandPMI = selectRAT0WBPMIforRank2(
                        availableRBGBitMap^tempRBGBitMap,
                        resrcAllocatorInput_p->subBandReportInfo_p); 
                harqProcess_p->pmiIndex = widebandPMI;
            }
            else if( ( (harqProcess_p->isTBOneValid && 
                            TB_INVALID == harqProcess_p->isTBTwoValid) ||
                        (harqProcess_p->isTBTwoValid && 
                         TB_INVALID == harqProcess_p->isTBOneValid) ) && 
                    ( 5 != harqProcess_p->pmiIndex ) &&
                    ( 6 != harqProcess_p->pmiIndex ) ) 
            {     
                    SubbandPMIInfo *subbandPMIInfo_p = 
                        &(resrcAllocatorInput_p->subBandReportInfo_p->
                                cqiInfo.cqiInfoSpatialMul.pmiInfo);
                if((2 == dlUEContext_p->dlMIMOInfo.dlRi) &&
                          (PNULL != subbandPMIInfo_p))
                {
                        subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank1[0] =
                            subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank2[0]; 
                        subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank1[2] =
                            subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank2[1]; 
                        subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank1[1] = 0;
                        subbandPMIInfo_p->rat0PmiInfo.reportedRBGRank1[3] = 0;
                }
                widebandPMI = selectRAT0WBPMIforRank1(
                        availableRBGBitMap^tempRBGBitMap,
                        resrcAllocatorInput_p->subBandReportInfo_p); 
                harqProcess_p->pmiIndex = widebandPMI;
            }
        }
    }

    dlUEContext_p->actualPMIValueForDLSCH = FALSE;       
    resrcAllocatorInput_p->aperiodicMode12 =  FALSE; 
    resrcAllocatorOutput_p->pmiValue =  harqProcess_p->pmiIndex;
    /* - CQI_5.0 */

    /* SPR 4543 changes start */
    if ( rbsAllocated != harqProcess_p->assignedResourcesNB )              
        /* SPR 4543 changes end */
    {
        /* ICIC changes start */
        UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                -(rbsAllocated - harqProcess_p->assignedResourcesNB), internalCellIndex);
        if ( (NEW_TX == dlHARQTBOneInfo_p->txIndicator) ||
                (NEW_TX == dlHARQTBTwoInfo_p->txIndicator))
        {
            harqProcess_p->assignedResourcesNB = rbsAllocated;

#ifdef FDD_CONFIG
            SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
                    rbsAllocated);
#elif TDD_CONFIG
            SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
                    rbsAllocated,
                    isSpecialSubframe);
#endif
        }
        else
        {
            ltePanic("Invalid Scenario TX Type [%d] Harq PRB [%d] Allocated [%d]\n",
                    dlHARQTBOneInfo_p->txIndicator,
                    harqProcess_p->assignedResourcesNB,
                    rbsAllocated);
        }   
    }

}
/*****************************************************************************
 * Function Name  : updateRAT1SubBandPMIInfo
 * Inputs         : dlUEContext_p - Pointer to DLUEContext,
 *                  harqProcess_p  - Pointer to DLHARQProcess,
 *                  resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                  availableRBBitMap,
 *                  tempRBBitMap,
 *                  subsetIndexAllocated - Allocated Subset Index,
 *                  isNonZeroSpanConsideredForAlloc
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates the subband PMI information for RAT1 
 *****************************************************************************/
STATIC void updateRAT1SubBandPMIInfo(DLUEContext * dlUEContext_p,
                            DLHARQProcess *harqProcess_p,
                            ResourceAllocatorInput * resrcAllocatorInput_p,
                            UInt32 availableRBBitMap,
                            UInt32 tempRBBitMap,
                            UInt32 subsetIndexAllocated,
                            UInt32 isNonZeroSpanConsideredForAlloc )
{
   DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
   DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL;
   UInt8 widebandPMI = 0;


   dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
    dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo;
   if ( (NEW_TX == dlHARQTBOneInfo_p->txIndicator) ||
        (NEW_TX == dlHARQTBTwoInfo_p->txIndicator))
   {
       /* SPR 5633 changes start */
       if( (TRUE == resrcAllocatorInput_p->aperiodicMode12) &&         
           (resrcAllocatorInput_p->subBandReportInfo_p) &&
           /* SPR 5633 changes end */
            /* + TM7_8 Changes Start */
           (!(TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode)))
            /* - TM7_8 Changes End */
       {
           if( (harqProcess_p->isTBOneValid && harqProcess_p->isTBTwoValid)
                   && (2 != harqProcess_p->pmiIndex) )
           {
               widebandPMI = selectRAT1WBPMIforRank2(
                       availableRBBitMap^tempRBBitMap,
                       subsetIndexAllocated,
                       isNonZeroSpanConsideredForAlloc,
                       resrcAllocatorInput_p->subBandReportInfo_p); 
               harqProcess_p->pmiIndex = widebandPMI;
           }
           else if( ( (harqProcess_p->isTBOneValid && 
                       TB_INVALID == harqProcess_p->isTBTwoValid) ||
                      (harqProcess_p->isTBTwoValid && 
                       TB_INVALID == harqProcess_p->isTBOneValid) ) &&
                    ( 5 != harqProcess_p->pmiIndex ) &&
                    ( 6 != harqProcess_p->pmiIndex ) )  
           {     
               if(2 == dlUEContext_p->dlMIMOInfo.dlRi)
               {
                   SubbandPMIInfo *subbandPMIInfo_p = 
                       &(resrcAllocatorInput_p->subBandReportInfo_p->
                               cqiInfo.cqiInfoSpatialMul.pmiInfo);
                   if(PNULL != subbandPMIInfo_p)
                   {    
                       if(isNonZeroSpanConsideredForAlloc)
                       {
                           subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated].
                               spanNonZeroPmiInfo.reportedPRBsRank1[0] =
                               subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated].
                               spanNonZeroPmiInfo.reportedPRBsRank2[0];
                           subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated].
                               spanNonZeroPmiInfo.reportedPRBsRank1[2] =
                               subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated].
                               spanNonZeroPmiInfo.reportedPRBsRank2[1];
                           subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated].
                               spanNonZeroPmiInfo.reportedPRBsRank1[1] = 0;
                           subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated].
                               spanNonZeroPmiInfo.reportedPRBsRank1[3] = 0;
                       }
                       else
                       {
                           subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated].
                               spanZeroPmiInfo.reportedPRBsRank1[0] =
                               subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated].
                               spanZeroPmiInfo.reportedPRBsRank2[0];
                           subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated].
                               spanZeroPmiInfo.reportedPRBsRank1[2] =
                               subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated].
                               spanZeroPmiInfo.reportedPRBsRank2[1];
                           subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated].
                               spanZeroPmiInfo.reportedPRBsRank1[1] = 0;
                           subbandPMIInfo_p->rat1PmiInfo[subsetIndexAllocated].
                               spanZeroPmiInfo.reportedPRBsRank1[3] = 0;
                       }
                   }    
               }
               widebandPMI = selectRAT1WBPMIforRank1(
                       availableRBBitMap^tempRBBitMap,
                       subsetIndexAllocated,
                       isNonZeroSpanConsideredForAlloc,
                       resrcAllocatorInput_p->subBandReportInfo_p); 
               harqProcess_p->pmiIndex = widebandPMI;
           }
       }
   }
}
/*****************************************************************************
 * Function Name  : allocRBInRAT1SpatialForSubband 
 * Inputs         : harqProcess_p - Pointer to DLHARQProcess,
 *                  resrcAllocatorInput_p - Pointer to ResourceAllocatorInput,
 *                  rat1Info_p - Pointer to RAT1Info,
 *                  dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                  availableRBCount,
 *                  subsetIndexAllocated_p,
 *                  isNonZeroSpanConsideredForAlloc_p,
 *                  allocatedRb_p,
 *                  subsetInfo_p,
 *                  tempRBBitMap_p,
 *                  centralRbMask,
 *                  transmissionMode - Mode of Transmission,
 *                  scheduledTick - Scheduled TTI,
 *                  subframe
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function allocate the RB's and TB size based on 
 *                  special subframe for RAT0
 *****************************************************************************/
STATIC  MacRetType allocRBInRAT1SpatialForSubband ( DLHARQProcess * harqProcess_p,
                                     ResourceAllocatorInput * resrcAllocatorInput_p,
                                     RAT1Info * rat1Info_p,
                                     DLStrategyTxNode * dlStrategyTxNode_p,
                                     UInt32 availableRBCount,
                                     UInt32 * subsetIndexAllocated_p,
                                     UInt32 *isNonZeroSpanConsideredForAlloc_p,
                                     UInt8 * allocatedRb_p,
/*Coverity_31964 Fix Start*/
                                     SubsetInfo **subsetInfo_p,
/*Coverity_31964 Fix End*/
                                     UInt32 *tempRBBitMap_p,
                                     /* + TM7_8 Changes Start */
                                     UInt32 centralRbMask[MAX_SUBSETS],
                                     UInt8  transmissionMode,
                                     /* SPR 15909 fix start */
                                     tickType_t scheduledTick
                                     /* SPR 15909 fix end */
                                     /* - TM7_8 Changes End */
#ifdef TDD_CONFIG
                                    /*coverity-530-32798*/
                                    ,UInt8 subframe 
                                    /*coverity-530-32798*/
#endif
                                    ,InternalCellIndex internalCellIndex
                                      )
{

	UInt32 tempRBSubBandBitMap = 0;
	UInt8 widebandPMI = 0;
	/* ICIC changes start */
	UInt32 resourceRegionAllocated =  
		resrcAllocatorInput_p->resourceRegionAllocated;
	/* ICIC changes end */
        UInt16 ueIndex = dlStrategyTxNode_p->ueIndex;
        DLUEContext *dlUEContext_p = PNULL;
        dlUEContext_p =  dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
	DLHARQTBInfo *dlHARQTBOneInfo_p = PNULL;
	DLHARQTBInfo *dlHARQTBTwoInfo_p = PNULL;
	/* SPR 21857 changes */
	widebandPMI = dlUEContext_p->dlCQIInfo.wideBandPMIIndex;
	/* SPR 21857 changes */
    /*coverity-530-32798*/
#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe =checkForSpecialSubframe(subframe, internalCellIndex);
#endif
    /*coverity-530-32798*/
        dlHARQTBOneInfo_p = &harqProcess_p->dlHARQTBOneInfo;
        dlHARQTBTwoInfo_p = &harqProcess_p->dlHARQTBTwoInfo;
	if  ( (NEW_TX == dlHARQTBOneInfo_p->txIndicator) ||
			(NEW_TX == dlHARQTBTwoInfo_p->txIndicator) )
	{
		tempRBSubBandBitMap = getRAT1BitMapSpatialMux(
				rat1Info_p,
				resrcAllocatorInput_p,
				harqProcess_p,
				subsetIndexAllocated_p,
				isNonZeroSpanConsideredForAlloc_p,
				dlStrategyTxNode_p,
                /* + TM7_8 Changes Start */
                centralRbMask,
                transmissionMode,
                scheduledTick,
                internalCellIndex);
                /* - TM7_8 Changes End */
		if (!tempRBSubBandBitMap)
		{
			/* As of now, there is no use case for strictAllocSubBand
			 * strictAllocSubBand check is commented */

			/*Now we try to allocate RBs according to wideBands*/
			if (availableRBCount >= resrcAllocatorInput_p->requiredRB)
			{
				*allocatedRb_p = resrcAllocatorInput_p->requiredRB;

			}
			else
			{
				/* We can not allocate less than minTBSize if strictScheduled is instructed */
				if (dlStrategyTxNode_p->strictAllocatedFlag)
				{
					/* ICIC changes start */
					UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
							(resrcAllocatorInput_p->requiredRB), internalCellIndex);
					/* ICIC changes end */
					return MAC_FAILURE;
				}
				*allocatedRb_p = availableRBCount;

				//update excessPool but it seems that it is the last UE as available resources are less than required.
				/* ICIC changes start */
				UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
						(resrcAllocatorInput_p->requiredRB - *allocatedRb_p), internalCellIndex);
				/* ICIC changes end */
			}   
			harqProcess_p->assignedResourcesNB = *allocatedRb_p;
            /*spr 15832 +*/
            if(IS_PCELL_OF_UE(dlUEContext_p,internalCellIndex))
            { 
                dlHARQTBOneInfo_p->mcsIndex = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordOne;
                dlHARQTBTwoInfo_p->mcsIndex = dlUEContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
            }   
            else
            {
                dlHARQTBOneInfo_p->mcsIndex = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordOne;
                dlHARQTBTwoInfo_p->mcsIndex = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo.mcsIndexCodeWordTwo;
            }    
            /*spr 15832 -*/
			harqProcess_p->pmiIndex = widebandPMI;

			LOG_MAC_MSG(MAC_SB_CQI_FAILURE_LOG_ID,LOGDEBUG,
					MAC_PDSCH,getCurrentTick(),(UInt32)dlStrategyTxNode_p->ueIndex,
					(UInt32)availableRBCount,
					(UInt32)resrcAllocatorInput_p->requiredRB,
					*tempRBBitMap_p,*allocatedRb_p,
					DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"SB_FAIL_NWTX");
		}
		else
		{   
/*Coverity_31964 Fix Start*/
			*subsetInfo_p = &rat1Info_p->subsetInfo[(*subsetIndexAllocated_p)];
/*Coverity_31964 Fix End*/
			*allocatedRb_p = harqProcess_p->assignedResourcesNB;
			*tempRBBitMap_p = tempRBSubBandBitMap;

			if (WIDEBAND_PMI == harqProcess_p->pmiIndex)
			{
				harqProcess_p->pmiIndex = widebandPMI;
			}

			LOG_MAC_MSG(MAC_SB_CQI_SUCCESS_LOG_ID,LOGDEBUG,
					MAC_PDSCH,getCurrentTick(),(UInt32)dlStrategyTxNode_p->ueIndex,
					(UInt32)availableRBCount,
					(UInt32)resrcAllocatorInput_p->requiredRB,
					*tempRBBitMap_p,*allocatedRb_p,
					DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"SB_PASS_NWTX");
		}      

		/*In case of New Transmission set the Transport block size in HARQ TB considering the fact
		  that this could be special subframe in case of TDD*/
#ifdef FDD_CONFIG
		SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
				*allocatedRb_p);
#elif TDD_CONFIG
		SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
				*allocatedRb_p,
				isSpecialSubframe);
#endif
	}
	return MAC_SUCCESS ;
}
/*****************************************************************************
 * Function Name  : updatePMIIndexInHarqContext 
 * Inputs         : dlUEContext_p - Pointer to DLUEContext,
 *                  macAdvanceFactor,
 *                  widebandPMI
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : harqProcess_p - Pointer to DLHARQProcess, 
 * Returns        : None
 * Description    : This function updates the PMI indexx informatin in the
 *                  Harq context
 *****************************************************************************/
STATIC  void updatePMIIndexInHarqContext(DLUEContext * dlUEContext_p,
                                        DLHARQProcess * harqProcess_p,
                                        UInt32 macAdvanceFactor,
                                        UInt8 widebandPMI,
                                        InternalCellIndex internalCellIndex,
                                        SInt8 allocatedRb
                                        #ifdef TDD_CONFIG
                                        ,UInt8 isSpecialSubframe
                                        #endif
                                        ) 
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;

    if ( (!dlUEContext_p->dlCQIInfo.latestAperiodicReportRcvd)&&
            ( UE_ACK_NACK_TTI <= 
              ((getCurrentTick() + macAdvanceFactor)- 
               dlUEContext_p->dlCQIInfo.reportReceivedTTI))
            /* + TM7_8 Changes Start */
            && (!(TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode)))
        /* - TM7_8 Changes End */
    {
        if (harqProcess_p->isTBOneValid && harqProcess_p->isTBTwoValid)
        {
            /*2 Layers:precoding according to latest PMI reports on PUSCH*/
            /*4x2 MIMO S*/
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                harqProcess_p->pmiIndex = 2;
                dlUEContext_p->actualPMIValueForDLSCH = TRUE;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                /* 4x4 DL MIMO CHG START */
                /* CA changes Start */
                /* UE category check not required here because the UE supporting
                 * 3 and 4 layers can report these numOfLayer values */
                if (harqProcess_p->numOfLayer == RI_LAYER_4)
                {
                    harqProcess_p->pmiIndex = PMI_INDEX_50;
                }
                else if (harqProcess_p->numOfLayer == RI_LAYER_3)
                {
                    harqProcess_p->pmiIndex = PMI_INDEX_33;
                }
                /* CA changes end */
                else 
                {
                    harqProcess_p->pmiIndex = PMI_INDEX_16;
                }
                /* 4x4 DL MIMO CHG END */
                dlUEContext_p->actualPMIValueForDLSCH = TRUE;
            }
            /*4x2 MIMO E*/
        }
        else if( (harqProcess_p->isTBOneValid && 
                    TB_INVALID == harqProcess_p->isTBTwoValid) ||
                (harqProcess_p->isTBTwoValid && 
                    TB_INVALID == harqProcess_p->isTBOneValid) ) 
        {
            /*4x2 MIMO S*/
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                /* 4x4 DL MIMO CHG START */
                if (harqProcess_p->isTBOneValid)
                    harqProcess_p->pmiIndex = 5;
                else
                    harqProcess_p->pmiIndex = 6;
                /* 4x4 DL MIMO CHG END */
                dlUEContext_p->actualPMIValueForDLSCH = TRUE;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                /* 4x4 DL MIMO CHG START */
                if ((RI_LAYER_3 <= harqProcess_p->numOfLayer) &&
                        (harqProcess_p->isTBTwoValid) &&
                        (harqProcess_p->dlHARQTBTwoInfo.txIndicator == RE_TX ))
                {
                    harqProcess_p->pmiIndex = PMI_INDEX_34;
                }
                else if ((RI_LAYER_4 == harqProcess_p->numOfLayer) &&
                        (harqProcess_p->isTBOneValid) &&
                        (harqProcess_p->dlHARQTBOneInfo.txIndicator == RE_TX ))
                {
                    harqProcess_p->pmiIndex = PMI_INDEX_34;
                }
                else
                    harqProcess_p->pmiIndex = PMI_INDEX_17;
                /* 4x4 DL MIMO CHG END */
                dlUEContext_p->actualPMIValueForDLSCH = TRUE;
            }
            /*4x2 MIMO E*/
        }
    }
    else
    {
        harqProcess_p->pmiIndex = widebandPMI;
    }            
/* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
                SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
                        allocatedRb);
#elif TDD_CONFIG
                SET_MIMO_TB_SIZE_BASED_ON_SPL_SUBFRAME(harqProcess_p,
                        allocatedRb,
                        isSpecialSubframe);
#endif
/* Cyclomatic_complexity_changes_end */
}
/*****************************************************************************
 * Function Name  : getRAT0BitMap 
 * Inputs         : cqiBitMapCodeWord1,
 *                  cqiBitMapCodeWord2,
 *                  rat0PmiInfo_p - Pointer to RAT0PmiInfo,
 *                  rat0Info_p  - Pointer to RAT0Info,
 *                  resrcAllocatorInput_p  - Pointer to ResourceAllocatorInput,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  previousRsrcFrmt_p - Pointer to ResourceFormatMIMONode,
 *                  dlStrategyNode_p  - Pointer to DLStrategyTxNode,
 *                  centralRbMask,
 *                  transmissionMode - Mode of Transmission,
 *                  scheduledTick - Scheduled TTI
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None  
 * Returns        : RBG Bitmap
 * Description    : This function will select the PMI Index for subbandPMI
 *                  and validate the RB's for minumu and maximum TB size.
 *                  Returns the updated RBG Bitmap
 *****************************************************************************/
STATIC  UInt32 getRAT0BitMap( UInt32 cqiBitMapCodeWord1,
        UInt32 cqiBitMapCodeWord2,
        RAT0PmiInfo * rat0PmiInfo_p,
        RAT0Info *rat0Info_p,
        ResourceAllocatorInput * resrcAllocatorInput_p,
        DLHARQProcess * harqProcess_p,
        ResourceFormatMIMONode * previousRsrcFrmt_p,                           
        DLStrategyTxNode * dlStrategyNode_p,
        UInt32 centralRbMask,
        UInt8 transmissionMode,
        /* SPR 15909 fix start */
        tickType_t scheduledTick ,
         /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    UInt8 pmiIndex = WIDEBAND_PMI;
    UInt32 bitMapToBeReturned = 0;
    UInt32 availableRBGBitMap = rat0Info_p->rat0BitMap[CC_USER] & 
        rat0Info_p->rat0BitMap[resrcAllocatorInput_p->resourceRegionAllocated];
    
    /* + TM7_8 Changes */
    /* SPR 21631 Start */
    if(CHECK_SF_MAPPED_ON_BCH_PSS_SSS(scheduledTick,transmissionMode))
    /* SPR 21631 End */
    {
        /*block central 6 RBs in temp variable*/
        availableRBGBitMap = availableRBGBitMap &(~centralRbMask);
    }
    /* - TM7_8 Changes */

    bitMapToBeReturned = cqiBitMapCodeWord1 & availableRBGBitMap;
    if (!bitMapToBeReturned)
    {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                "bitMapToBeReturned_p  is NULL Using bitMapToBeReturned_p to be \
                formed from rat064QAMCQIBitMapCW2");
            bitMapToBeReturned = cqiBitMapCodeWord2 & 
                                              availableRBGBitMap;  
    }
    if (bitMapToBeReturned)
    {
        if (resrcAllocatorInput_p->subbandPMI)
        {
            pmiIndex = selectRAT0SubbandPMIIndexRank2(
                    bitMapToBeReturned,
                    rat0PmiInfo_p,
                    &bitMapToBeReturned,
                    internalCellIndex
                    );
            /*need to check whether pmiIndex is WIDEBAND_PMI then fill wideband pmi value in this*/
            harqProcess_p->pmiIndex = pmiIndex;
        }

        if (MAC_SUCCESS == validateRBsforMinMaxTBSize(
                    bitMapToBeReturned,
                    rat0Info_p,
                    resrcAllocatorInput_p,
                    harqProcess_p,
                    previousRsrcFrmt_p,
                    dlStrategyNode_p,
                    internalCellIndex))
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RESOURCE_MGR,
                    "Leaving funtion = %s with Success\n",__func__);

            return bitMapToBeReturned;
        }
    }
    return 0;
}
/* Cyclomatic Complexity changes - ends  here */

/* - TM6_5.2 */
/* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : setTbSizeBasedOnSplSubframe
 * Inputs         : hProcess_p - Harq Context
 *                  allocRb - allocated Rbs
 * Outputs        : None
 * Returns        : void
 * Description    :Get TB Size and set its value to Harq Context
 ******************************************************************************/
STATIC void setTbSizeBasedOnSplSubframe( DLHARQProcess *hProcess_p,
                                         UInt8 allocRb
                                       )
{
    (hProcess_p)->dlHARQTBOneInfo.tbSize = determineTBSize(
                                             (hProcess_p)->dlHARQTBOneInfo.mcsIndex,
                                             allocRb,
                                             /*4X4 DL MIMO CHG START*/
                                             TB_MAPPED_TO_ONE_LAYER
                                             /*4X4 DL MIMO CHG END*/);
    return;
}
#elif TDD_CONFIG
/*****************************************************************************
 * Function Name  : setTbSizeBasedOnSplSubframe
 * Inputs         : hProcess_p - Harq Context
 *              : allocRb - allocated Rbs
 *                : isSplSubframe - If the subframe is special or not.
 * Outputs        : None
 * Returns        : void
 * Description    :Get TB Size and set its value to Harq Context
 ******************************************************************************/
STATIC void setTbSizeBasedOnSplSubframe( DLHARQProcess  *hProcess_p,
                                         UInt8          allocRb,
                                         UInt8          isSplSubframe
                                       )
{
    if (isSplSubframe)
    {
       (hProcess_p)->dlHARQTBOneInfo.tbSize = determineTBSize(
                                                (hProcess_p)->dlHARQTBOneInfo.mcsIndex,
                                                SP_SUBFRAME_PRB(allocRb),
                                                /*4X4 DL MIMO CHG START*/
                                                TB_MAPPED_TO_ONE_LAYER
                                                /*4X4 DL MIMO CHG END*/);
    }
    else
    {
        (hProcess_p)->dlHARQTBOneInfo.tbSize = determineTBSize(
                                                  (hProcess_p)->dlHARQTBOneInfo.mcsIndex,
                                                  allocRb,
                                                  /*4X4 DL MIMO CHG START*/
                                                  TB_MAPPED_TO_ONE_LAYER
                                                  /*4X4 DL MIMO CHG END*/);
     }

     return;
}
#endif


/*****************************************************************************
 * Function Name  : getNumofOverlappingRb
 * Inputs         : scheduledTick- - Scheduled TTI
 *                  allocatedRBCount - allocated RB count
 *                  bestVRBSpanInfo -
 *                  numOfOverLappingRb- Number of Overlapping RB's
 *
 * Outputs        : None
 * Returns        : void
 * Description    : Get number of overlapping Rbs.
 ******************************************************************************/
 /* SPR 15909 fix start */
STATIC void getNumofOverlappingRb(tickType_t           scheduledTick,
/* SPR 15909 fix end */
                           UInt32           allocatedRBCount,
                           BestVRBSpanInfo  bestVRBSpanInfo,
                           UInt8 *numOfOverLappingRb,
                           InternalCellIndex internalCellIndex)
{

    UInt8 index;
    UInt32 prbNumber;
    UInt8 *centralSixRbsInfo_p = cellSpecificParams_g.\
           cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
           centralSixRbsInfo;
    /* SPR 21631 Start */

 /*Dynamic PDCCH + 5117 changes starts */
     /* SPR 23393 Changes Start*/
            if ((!(scheduledTick %5))&&
     /* SPR 23393 Changes End*/
            (bestVRBSpanInfo.startingVRBIndex <=\
             centralSixRbsInfo_p[MAX_NUM_OVERLAPPING_RB-1])&&\
            (bestVRBSpanInfo.endingVRBIndex >= centralSixRbsInfo_p[0]))
    {
     /* SPR 23393 Changes Start*/
                    if ((!(scheduledTick %10)&&
     /* SPR 23393 Changes End*/
                            !(cellSpecificParams_g.\
                        cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                        dlSchWithMIBAllowed))  )
                    /* SPR 21631 End */
        {
            *numOfOverLappingRb = 0;
        }
        else
        {
            for (index = 0; index < allocatedRBCount; index++)
            {
                prbNumber = bestVRBSpanInfo.startingVRBIndex + index;
                if(checkAllocRBLiesInCentralSix(prbNumber,internalCellIndex))
                {
                    (*numOfOverLappingRb)++;
                }
            }
        }
    }

    /* SPR_7516_FIX */
#ifdef TDD_CONFIG
     /* SPR 23393 Changes Start*/
    if(getSubframeType(scheduledTick%MAX_SUB_FRAME, internalCellIndex)== SP)
     /* SPR 23393 Changes End*/
    {
        for (index = 0; index < allocatedRBCount; index++)
        {
            prbNumber = bestVRBSpanInfo.startingVRBIndex + index;
            if(checkAllocRBLiesInCentralSix(prbNumber,internalCellIndex))
            {
                (*numOfOverLappingRb)++;
            }
        }
    }
#endif
    /* SPR_7516_FIX */
    /*Dynamic PDCCH + 5117 changes ends */
    return;
}
/* Cyclomatic_complexity_changes_end */
