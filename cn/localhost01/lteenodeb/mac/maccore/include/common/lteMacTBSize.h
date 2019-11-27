/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacTBSize.h,v 1.2.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file defines the functionality for calculating
 *                    the Transport block size for the given MCS Index and 
 *                    number of Resource Block. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacTBSize.h,v $
 * Revision 1.2.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.7  2009/08/03 09:43:19  gur20052
 * Fix for LTE_MAC_UT_LOG
 *
 * Revision 1.6  2009/06/05 11:00:20  gur12140
 * Removed Linking Errors
 *
 * Revision 1.4  2009/05/19 14:32:53  gur20052
 * Updated with DL Fair Scheduler module specific changes
 *
 * Revision 1.3  2009/05/18 06:32:32  gur15697
 * determineRBFromTBSizeForDCIFormate1A() function added
 *
 * Revision 1.2  2009/05/12 12:46:21  gur15697
 * new function added
 *
 * Revision 1.1  2009/05/12 08:36:14  gur19413
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_TB_SIZE_H
#define LTE_MAC_TB_SIZE_H

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


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
/* ULA_CHG */
/* Minimum value of MCS index defind as 2, against which minimum number of Rbs 
allocated by strategy in case of SR grant is decided */
/* SPR 3073 Fix starts */
#define MIN_GRANT_MCS_INDEX 4
/* SPR 3073 Fix ends */
/* ULA_CHG */
#define MAX_MCS_INDEX  29
#define MAC_MAX_COMMON_CH_RB 75
#define INVALID_TRANSMISSION_TYPE 9
#define MAX_RESOURCE_BLOCK 110
#define LOWER_RB_BOUNDARY  50
#define TB_SIZE "tb_size"

/* SPR 5999 changes start */
#define DL_ALLOCATION 1
#define UL_ALLOCATION 2




#define DETERMINE_RBs_DL(mcsIdx, minDataSize,internalCellIndex)\
   determineRBs(mcsIdx, &minDataSize, DL_ALLOCATION,internalCellIndex)
/*4x4 DL MIMO CHG START*/
#define DETERMINE_RBs_ARRL2_DL(mcsIdx, minDataSize,internalCellIndex)\
   determineRBsFromTBSizeArrL2(mcsIdx, &minDataSize, DL_ALLOCATION,internalCellIndex)
/*4x4 DL MIMO CHG END*/

/*CA Changes start  */
#define DETERMINE_RBs_UL(mcsIdx, minDataSize, internalCellIndex)\
   determineRBs(mcsIdx, &minDataSize, UL_ALLOCATION, internalCellIndex)
/*CA Changes end  */
/* SPR 5999 changes end */

/* SPR 3062 Chg */
/*Dynamic PDCCH + 5117 changes starts */
#ifdef TDD_CONFIG
#define MAX_SF_CONFIG 9
#define MAX_CP_VALUE 2
#endif
#define MAX_NUM_OF_SUBFRAME 10
#define MAX_NUM_OF_RB 100
#define MAX_RE_IN_ONE_SYMBOL 12
#ifdef LTE_EMBMS_SUPPORTED
 #define NUM_OF_ERS 18
 #define NUM_OF_SYMBOLS_ON_EMBMS_SF_12 12
#endif
#define CFI_VALUE_1 1
#define LOWER_RB_THRESHOLD_FACTOR 2
/*+As per spec 36.212 sec-5.1.1*/
#define PDSCH_CRC_BITS 24
/*SPR 8451 Changes Start*/
#ifdef ULCODERATE_PLUGIN
#define PUSCH_CRC_BITS 24
#endif
/*SPR 8451 Changes End*/
#define MAX_CODE_BLOCK_SIZE 6144
/*-As per spec 36.212 sec-5.1.1*/
/*4x4 DL MIMO CHG START*/
#define TB_MAPPED_TO_ONE_LAYER 1
#define TB_MAPPED_TO_TWO_LAYER 2
/*4x4 DL MIMO CHG END*/
/* SPR 23017 Fix Start */
#define MAX_NUM_OF_LAYER 2
#define LAYER_ONE_IDX 0
#define LAYER_TWO_IDX 1
/* SPR 23017 Fix End */
#ifdef FDD_CONFIG
#define CALCULATE_MAX_RE_FOR_SF_5(maxResourceElement,maxREPerRB,rbCounter, numOfOverlappingRbs)\
{\
    maxResourceElement = maxREPerRB * (rbCounter+1)-\
            ((MAX_RE_IN_ONE_SYMBOL<<1)*(numOfOverlappingRbs)/*REs for the PSS/CSS*/);\
}
#define CALCULATE_MAX_RE_FOR_SF_0(maxResourceElement,maxREPerRB,rbCounter, numOfOverlappingRbs)\
{\
    maxResourceElement = maxREPerRB * (rbCounter+1)- ((MAX_RE_IN_ONE_SYMBOL << 1) *\
            (numOfOverlappingRbs)/*REs for the PSS/CSS*/)\
                   - ((MAX_RE_IN_ONE_SYMBOL<<2)*(numOfOverlappingRbs)/*REs for MIB*/)\
                     + (numOfOverlappingRbs<<2/*REs for Referece Signal */);\
}
#else
#define CALCULATE_MAX_RE_FOR_SF_5(maxResourceElement,maxREPerRB,rbCounter, numOfOverlappingRbs)\
{\
    maxResourceElement = maxREPerRB * (rbCounter+1)-\
            ((MAX_RE_IN_ONE_SYMBOL)*(numOfOverlappingRbs)/*REs for the PSS/CSS*/);\
}
#define CALCULATE_MAX_RE_FOR_SF_0(maxResourceElement,maxREPerRB,rbCounter, numOfOverlappingRbs)\
{\
    maxResourceElement = maxREPerRB * (rbCounter+1)- ((MAX_RE_IN_ONE_SYMBOL) *\
            (numOfOverlappingRbs)/*REs for the PSS/CSS*/)\
                   - ((MAX_RE_IN_ONE_SYMBOL<<2)*(numOfOverlappingRbs)/*REs for MIB*/)\
                     + (numOfOverlappingRbs<<2/*REs for Referece Signal */);\
}

#endif
/*Dynamic PDCCH + 5117 changes ends */
/* SPR 3062 Chg Ends */

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* 4x4 DL MIMO CHG START */
extern UInt8 maxMCSSubframeMapForCFI_1_4x4[MAX_SFN_INDEX + 1][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB];
extern UInt8 maxMCSSubframeMapForCFI_2_4x4[MAX_SFN_INDEX + 1][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB];
extern UInt8 maxMCSSubframeMapForCFI_3_4x4[MAX_SFN_INDEX + 1][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB];
extern UInt8 maxMCSSubframeMapForCFI_4_4x4[MAX_SFN_INDEX + 1][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB];
#ifdef TDD_CONFIG
/** CA-TDD Changes Start **/
extern UInt8 maxMCSSpecialSFForCFI_1_4x4[MAX_NUM_CELL][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB];
extern UInt8 maxMCSSpecialSFForCFI_2_4x4[MAX_NUM_CELL][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB];
/** CA-TDD Changes Stop **/
#endif
/* 4x4 DL MIMO CHG END */
 
/* SPR 3062 Chg */
/* This table shall filled during cell configuration as vendor specific hence
 * values provided by them */ 
/* SPR 3062 Chg Ends */
/*Dynamic PDCCH + 5117 changes starts */
/* Start of change for VARIBLE Declaration */
/* SPR 23017 Fix Start */
extern UInt8
maxMCSSubframeMapForCFI_1[MAX_SFN_INDEX + 1][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB][MAX_UE_CATEGORY][MAX_NUM_OF_LAYER];
extern UInt8
maxMCSSubframeMapForCFI_2[MAX_SFN_INDEX + 1][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB][MAX_UE_CATEGORY][MAX_NUM_OF_LAYER];
extern UInt8
maxMCSSubframeMapForCFI_3[MAX_SFN_INDEX + 1][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB][MAX_UE_CATEGORY][MAX_NUM_OF_LAYER];
/*SPR 5412 Fix Start*/
extern UInt8
maxMCSSubframeMapForCFI_4[MAX_SFN_INDEX + 1][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB][MAX_UE_CATEGORY][MAX_NUM_OF_LAYER];
/* SPR 23017 Fix End */
extern UInt8 ueCategoryMap_g[3][MAX_MCS_INDEX + 1][MAX_MCS_INDEX + 1];// No need to store for uECategory 4 & 5
/* End of change for VARIBLE Declaration */
/*SPR 5412 Fix End*/
/*Dynamic PDCCH + 5117 changes ends */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* 4x4 DL MIMO CHG START */
UInt32 determineTBSize(UInt32  mcsIndex, UInt32 nbRB, UInt8 tbMappedToLayerNum);
/* 4x4 DL MIMO CHG END */
/* SPR 1868 */
UInt32 determineUlTBSize(UInt32  mcsIndex, UInt32 nbRB);
/* SPR 1868 */

UInt32 getMCSFromModulationAndCodingRate(UInt32 modulationScheme, UInt32 codingRate);

/* SPR 3628 chg */
MacRetType determineRBFromTBSizeForDCIFormate1A(UInt32 buffLenInBytes,
                                                UInt8 *tbSizeFound_p,
                                                UInt8 *numRB_p,
                                                UInt8 *mcsIndex_p
#ifdef TDD_CONFIG
/* Special subframe TB size */
						, UInt8 *tbSizeFoundSF_p,
/* Special subframe Number of RBs */
						UInt8 *numRBSF_p,
/* Special subframe MCS Index */
						UInt8 *mcsIndexSF_p
#endif
);
/* SPR 3628 chg */

/* SPR 3062 Chg */

UInt8 determineTBIndexWithBufferLength(UInt32 buffLenInBytes);
/* SPR 3062 Chg Ends */

UInt32 determineRBs(UInt32 mcsIndex, UInt32 *dataSize_p,
                    /* SPR 5999 changes start */
                    UInt8 value, InternalCellIndex internalCellIndex);
                    /* SPR 5999 changes end */
/*4x4 DL MIMO CHG START*/
UInt32 determineRBsFromTBSizeArrL2(UInt32 mcsIndex, UInt32 *dataSize_p,
                    /* SPR 5999 changes start */
                    UInt8 value,
                    InternalCellIndex internalCellIndex);
                    /* SPR 5999 changes end */
/*4x4 DL MIMO CHG END*/

/*Dynamic PDCCH + 5117 changes starts */
/* + SPR 17439 */
 void createMapForMaxMCS(void);
/* - SPR 17439 */
 void createMCSSubframeMap(UInt8 tempCfiValue,
                                 UInt8 subFramewithDelay,
                                 UInt8 rbCounter,
                                 UInt8 numOfOverlappingRbs,
                                 UInt8 mcsIndex,
                                 /* +- SPR 17777 */
                                 UInt8 ueCategory
#ifdef TDD_CONFIG
                                 ,InternalCellIndex internalCellIndex
#endif
                                 /* +- SPR 17777 */
				 /* SPR 23017 Fix Start */
                                 ,UInt8 mappedLayer);
                                 /* SPR 23017 Fix End */
 UInt8 calculateNumOfCRS(UInt8 tempCfiValue,
        InternalCellIndex internalCellIndex);
/*Dynamic PDCCH + 5117 changes ends */
void macUpdateUECategoryTable (InternalCellIndex cellIx);

/* SPR_10231_FIX_START */
UInt8 getTpcFromMcsTbSize(UInt32 buffLenInBytes);
/* SPR_10231_FIX_END */



/*SPR 8451 Changes Start*/
#ifdef ULCODERATE_PLUGIN
 void calculateUlMCSRBForCodeRateUECategory(InternalCellIndex
        internalCellIndex);
void calculateMaxMcsRbMapForNonSpatialMultiplexUL(UInt8 rbCounter,
                             UInt8 masterMcsIndex, UInt8 modScheme);
#endif
/*SPR 8451 Changes End*/

#ifdef LTE_EMBMS_SUPPORTED

/*SPR 18737 fix start*/
MacRetType embmsCfiValueValidate(UInt8 cfiVal,
                                InternalCellIndex internalCellIndex);
/*SPR 18737 fix end*/

MacRetType checkCodeRateForEmbms(UInt8 cfi,
                                 UInt8 mcs,
                                 InternalCellIndex internalCellIndex);

MacRetType checkCodeRateValidationForEmbms(UInt32 maxResourceElement,
                                   UInt8 mcs,
                                   UInt8 modScheme,
                                   UInt8 rbCounter);
#endif


/* SPR 22845 fix start*/
MacRetType determineRBFromTBSizeForGrant(UInt32 buffLenInBytes,
                                                UInt8 *tbSizeFound_p,
                                                UInt8 *numRB_p,
                                                UInt8 *mcsIndex_p);

/* SPR 22845 fix end*/

#endif  /* LTE_MAC_TB_SIZE_H */
