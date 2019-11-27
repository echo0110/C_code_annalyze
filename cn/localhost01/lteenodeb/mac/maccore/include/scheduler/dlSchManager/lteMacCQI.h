/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacCQI.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacCQI.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.14  2009/11/01 11:41:07  gur11912
 * NUM_CQI_NODES added
 *
 * Revision 1.13  2009/08/04 05:56:01  gur12140
 * Incorporated Comments
 *
 * Revision 1.12  2009/07/20 05:22:04  gur12140
 * Defined CQI_MGR
 *
 * Revision 1.11  2009/07/02 06:41:16  gur12140
 * Incorporated Commnents
 *
 * Revision 1.10  2009/06/24 06:28:50  gur12140
 * Incorporated Comments
 *
 * Revision 1.9  2009/06/16 11:53:28  gur12140
 * Incorporated Comments
 *
 * Revision 1.8  2009/06/05 11:00:50  gur12140
 * Removed Linking Errors
 *
 * Revision 1.7  2009/06/03 12:29:49  gur12140
 * Added Structure for CQI processing
 *
 * Revision 1.6  2009/06/01 07:44:47  gur12140
 * Compilation Error Removed
 *
 * Revision 1.5  2009/05/29 13:51:09  gur12140
 * added func declaration
 *
 * Revision 1.4  2009/05/08 11:11:24  gur12140
 * make it compiled
 *
 * Revision 1.3  2009/05/08 10:18:18  gur12140
 * Make it Compile
 *
 * Revision 1.2  2009/05/08 05:13:42  gur12140
 * Updated File.Added Structure
 *
 * Revision 1.1  2009/05/04 07:13:52  gur12140
 * empty log message
 ****************************************************************************/

#ifndef LTE_MAC_CQI_H
#define LTE_MAC_CQI_H


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
#define CQI_MGR "cqi_mgr"

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* + CQI_5.0 */
/* 4x2 MIMO S */
#define PMI_INDEX_RANK                           16
/* 4x2 MIMO E */
#define PMI_INDEX_RANK_1                         4
#define PMI_INDEX_RANK_2                         2
#define APERIODIC_TRIGGER_CQI_OFFSET             5     
#define INVALID_PMI_VALUE                        255
/* - CQI_5.0 */
/* + CQI_4.1 */
#define DL_BLER_SCALING_VALUE                    15
/* SPR 7981 changes start */
#define DL_RESOLUTION_FACTOR                        10
/* SPR 7981 changes end */
#define TARGET_DL_BLER_VALUE                     10
#define MAC_DL_ALPHA_COEFFICIENT_IIR             820
#define DL_BLER_CORRECTION_FACTOR_TABLE_SIZE     101
#define SET_CQI_REPORT_BIT(reportedCQIBitMap,reportedCQI)\
     reportedCQIBitMap |= ((UInt32) 1<<( (MAX_CQI_INDEX - reportedCQI) - 1));
/* - CQI_4.1 */

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/*This structure holds information of RAT 1 Non Spatial CQI Information
  * for both Zero Span (Left Span) and Non Zero Span(RightSpan)*/
typedef struct RAT1NonSpatialMulCQIInfoT
{
    /* + CQI_4.1 */ 
    /*This holds information of RAT 1 Non Spatial CQI Information 
    * for Non Zero Span */
    UInt32 subsetCQIBitMapNonZeroSpan [MAX_CQI_INDEX];
    /*This holds information of RAT 1 Non Spatial CQI Information 
    * for Zero Span*/
    UInt32 subsetCQIBitMapZeroSpan [MAX_CQI_INDEX];
    /* This holds the reported CQI in the form of set bit at the corresponding
    * position. CQI Index will be set from the LSB i.e. for CQI Index 15, bit 0
    * will be set in reportedCQIBitmap.*/
    UInt16 reportedCQIBitMap;
    /* This holds the reported subband in the form of set bit at the
     * corresponding position. */
    UInt32 subsetCQIBitMapALLCQIIndex;
    /* - CQI_4.1 */ 
    /*+EICIC */
    /* + CQI_4.1 */ 
    /*This holds information of RAT 1 Non Spatial CQI Information 
    * for Non Zero Span */
    UInt32 subsetCQIBitMapNonZeroSpan_2 [MAX_CQI_INDEX];
    /*This holds information of RAT 1 Non Spatial CQI Information 
    * for Zero Span*/
    UInt32 subsetCQIBitMapZeroSpan_2 [MAX_CQI_INDEX];
    /* This holds the reported CQI in the form of set bit at the corresponding
    * position. CQI Index will be set from the LSB i.e. for CQI Index 15, bit 0
    * will be set in reportedCQIBitmap.*/
    UInt16 reportedCQIBitMap_2;
    /* This holds the reported subband in the form of set bit at the
     * corresponding position. */
    UInt32 subsetCQIBitMapALLCQIIndex_2;
     /*-EICIC */
}RAT1NonSpatialMulCQIInfo;
 
 /*This structure holds information of RAT 0 Non Spatial CQI Information*/
typedef struct RAT0NonSpatialMulCQIInfoT
{
    /* + CQI_4.1 */
    /* This holds the reported CQI in the form of set bit at the corresponding
    * position. CQI Index will be set from the LSB i.e. for CQI Index 15, bit 0
    * will be set in reportedCQIBitmap.*/
    UInt16 reportedCQIBitMap;
    /* This holds the reported Subband Number per report*/
    UInt32 reportedSubbandBitMap;
    /* - CQI_4.1 */
    /*This holds CQI BitMap per CQI Index*/
    UInt32 rat0CQIBitMap[MAX_CQI_INDEX];
    /*+EICIC */
    UInt16 reportedCQIBitMap_2;
    UInt32 reportedSubbandBitMap_2;
    UInt32 rat0CQIBitMap_2[MAX_CQI_INDEX];
    /*-EICIC */
}RAT0NonSpatialMulCQIInfo;

 /*This structure holds information of RAT 0 and RAT 1 Non Spatial CQI 
  *Information*/
typedef struct CQIInfoNonSpatialMulT
{
    /*This Structure holds information of  RAT 0 Non Spatial CQI Information*/
    RAT0NonSpatialMulCQIInfo rat0NonSpatialMulCQIInfo;
    /*This Structure holds information of  RAT 1 Non Spatial CQI Information*/
    RAT1NonSpatialMulCQIInfo rat1NonSpatialMulCQIInfo[MAX_SUBSETS]; 
}CQIInfoNonSpatialMul;

 /*This structure holds information of RAT 0  Spatial CQI Information*/
typedef struct RAT0SpatialMulCQIInfoT
{
    /*This stores number of 64QAM Subband whose report is 
     *available for CodeWord One*/
    UInt8 count64QAMSubBandCW1;
    /*This stores number of 16QAM Subband whose report is 
    *available for CodeWord One*/
    UInt8 count16QAMSubBandCW1;
    /*This stores number of QPSk Subband whose report is 
    *available for CodeWord One*/
    UInt8 countQPSKSubBandCW1;

    /*This stores the bitmap in which each set bit represet a RBG 
     * whose 64AM subband report is available for CodeWord One*/
    UInt32 rat064QAMCQIBitMapCW1;
    /*This stores the bitmap in which each set bit represet a RBG 
     * whose 16AM subband report is available for CodeWord One*/
    UInt32 rat016QAMCQIBitMapCW1;
    /*This stores the bitmap in which each set bit represet a RBG 
     * whose QPSK subband report is available for CodeWord One*/
    UInt32 rat0QPSKCQIBitMapCW1;

    /*This stores number of 64QAM Subband whose report is 
    *available for CodeWord Two*/
    UInt8 count64QAMSubBandCW2;
    /*This stores number of 16QAM Subband whose report is 
    *available for CodeWord Two*/
    UInt8 count16QAMSubBandCW2;
    /*This stores number of QPSK Subband whose report is 
    *available for CodeWord Two*/
    UInt8 countQPSKSubBandCW2;
    
     /*This stores the bitmap in which each set bit represet a RBG 
      * whose 64AM subband report is available for CodeWord Two*/
    UInt32 rat064QAMCQIBitMapCW2;
     /*This stores the bitmap in which each set bit represet a RBG 
      * whose 16AM subband report is available for CodeWord Two*/
    UInt32 rat016QAMCQIBitMapCW2;
     /*This stores the bitmap in which each set bit represet a RBG 
      * whose QPSK subband report is available for CodeWord Two*/
    UInt32 rat0QPSKCQIBitMapCW2;
     /*+EICIC */
    /*This stores number of 64QAM Subband whose report is 
     *available for CodeWord One*/
    UInt8 count64QAMSubBandCW1_2;
    /*This stores number of 16QAM Subband whose report is 
    *available for CodeWord One*/
    UInt8 count16QAMSubBandCW1_2;
    /*This stores number of QPSk Subband whose report is 
    *available for CodeWord One*/
    UInt8 countQPSKSubBandCW1_2;

     /*This stores the bitmap in which each set bit represet a RBG 
      * whose 64AM subband report is available for CodeWord One*/
    UInt32 rat064QAMCQIBitMapCW1_2;
     /*This stores the bitmap in which each set bit represet a RBG 
      * whose 16AM subband report is available for CodeWord One*/
    UInt32 rat016QAMCQIBitMapCW1_2;
     /*This stores the bitmap in which each set bit represet a RBG 
      * whose QPSK subband report is available for CodeWord One*/
    UInt32 rat0QPSKCQIBitMapCW1_2;

    /*This stores number of 64QAM Subband whose report is 
     *available for CodeWord Two*/
    UInt8 count64QAMSubBandCW2_2;
    /*This stores number of 16QAM Subband whose report is 
    *available for CodeWord Two*/
    UInt8 count16QAMSubBandCW2_2;
    /*This stores number of QPSK Subband whose report is 
    *available for CodeWord Two*/
    UInt8 countQPSKSubBandCW2_2;
    
     /*This stores the bitmap in which each set bit represet a RBG 
      * whose 64AM subband report is available for CodeWord Two*/
    UInt32 rat064QAMCQIBitMapCW2_2;
     /*This stores the bitmap in which each set bit represet a RBG 
      * whose 16AM subband report is available for CodeWord Two*/
    UInt32 rat016QAMCQIBitMapCW2_2;
     /*This stores the bitmap in which each set bit represet a RBG 
      * whose QPSK subband report is available for CodeWord Two*/
    UInt32 rat0QPSKCQIBitMapCW2_2;  
    /*-EICIC */
}RAT0SpatialMulCQIInfo;


 /*This structure holds information of RAT 1  Spatial CQI Information for 
  *Left Span and Right Span per Subset*/
typedef struct SubsetSpanCQIInfoT
{
    /*This stores number of 64QAM Subband whose report is 
    *available for CodeWord One*/
    UInt8 count64QAMSubBandCW1;
    /*This stores number of 16QAM Subband whose report is 
    *available for CodeWord One*/
    UInt8 count16QAMSubBandCW1;
    /*This stores number of QPSk Subband whose report is 
    *available for CodeWord One*/
    UInt8 countQPSKSubBandCW1;

     /*This stores the bitmap in which each set bit represet a PRB
      * whose 64AM subband report is available for CodeWord One*/
    UInt32 rat164QAMCQIBitMapCW1;
     /*This stores the bitmap in which each set bit represet a PRB 
      * whose 16AM subband report is available for CodeWord One*/
    UInt32 rat116QAMCQIBitMapCW1;
     /*This stores the bitmap in which each set bit represet a PRB 
      * whose QPSK subband report is available for CodeWord One*/
    UInt32 rat1QPSKCQIBitMapCW1;

    /*This stores number of 64QAM Subband whose report is 
    *available for CodeWord Two*/
    UInt8 count64QAMSubBandCW2;
    /*This stores number of 16QAM Subband whose report is 
    *available for CodeWord Two*/
    UInt8 count16QAMSubBandCW2;
    /*This stores number of QPSK Subband whose report is 
    *available for CodeWord Two*/
    UInt8 countQPSKSubBandCW2;

     /*This stores the bitmap in which each set bit represet a PRB
      * whose 64AM subband report is available for CodeWord two*/
    UInt32 rat164QAMCQIBitMapCW2;
     /*This stores the bitmap in which each set bit represet a PRB 
      * whose 16AM subband report is available for CodeWord two*/
    UInt32 rat116QAMCQIBitMapCW2;
     /*This stores the bitmap in which each set bit represet a PRB 
      * whose QPSK subband report is available for CodeWord two*/
    UInt32 rat1QPSKCQIBitMapCW2;
     /*+EICIC */
      	 /*This stores number of 64QAM Subband whose report is 
    *available for CodeWord One*/
    UInt8 count64QAMSubBandCW1_2;
    /*This stores number of 16QAM Subband whose report is 
    *available for CodeWord One*/
    UInt8 count16QAMSubBandCW1_2;
    /*This stores number of QPSk Subband whose report is 
    *available for CodeWord One*/
    UInt8 countQPSKSubBandCW1_2;

     /*This stores the bitmap in which each set bit represet a PRB
      * whose 64AM subband report is available for CodeWord One*/
    UInt32 rat164QAMCQIBitMapCW1_2;
     /*This stores the bitmap in which each set bit represet a PRB 
      * whose 16AM subband report is available for CodeWord One*/
    UInt32 rat116QAMCQIBitMapCW1_2;
     /*This stores the bitmap in which each set bit represet a PRB 
      * whose QPSK subband report is available for CodeWord One*/
    UInt32 rat1QPSKCQIBitMapCW1_2;

    /*This stores number of 64QAM Subband whose report is 
    *available for CodeWord Two*/
    UInt8 count64QAMSubBandCW2_2;
    /*This stores number of 16QAM Subband whose report is 
    *available for CodeWord Two*/
    UInt8 count16QAMSubBandCW2_2;
    /*This stores number of QPSK Subband whose report is 
    *available for CodeWord Two*/
    UInt8 countQPSKSubBandCW2_2;

     /*This stores the bitmap in which each set bit represet a PRB
      * whose 64AM subband report is available for CodeWord two*/
    UInt32 rat164QAMCQIBitMapCW2_2;
     /*This stores the bitmap in which each set bit represet a PRB 
      * whose 16AM subband report is available for CodeWord two*/
    UInt32 rat116QAMCQIBitMapCW2_2;
     /*This stores the bitmap in which each set bit represet a PRB 
      * whose QPSK subband report is available for CodeWord two*/
    UInt32 rat1QPSKCQIBitMapCW2_2;
    /*-EICIC */
}SubsetSpanCQIInfo;

 /*This structure holds information of RAT 1 Spatial CQI Information
  * for both Zero Span (Left Span) and Non Zero Span(RightSpan)*/
typedef struct RAT1SpatialMulCQIInfoT
{
    /*This Structure holds information of RAT 1 Spatial CQI Information 
     * for Zero Span*/
    SubsetSpanCQIInfo subsetZeroSpanCQIInfo;
    /*This Structure holds information of RAT 1 Spatial CQI Information 
     * for Non Zero Span*/
    SubsetSpanCQIInfo subsetNonZeroSpanCQIInfo;
    /*This Holds bitMap in which each set  bit  represents a PRB in Subset 
     *whose subband report is available*/
    UInt32 subsetCQIBitMapALLCQIIndex;
    /*+EICIC */
     /*This Holds bitMap in which each set  bit  represents a PRB in Subset 
     *whose subband report is available*/
    UInt32 subsetCQIBitMapALLCQIIndex_2;
   /*-EICIC */
}RAT1SpatialMulCQIInfo;

/* + CQI_5.0 */
typedef struct RAT0PmiInfoT
{
    /*this will store reported RBG in the form of bitmap for each Rank*/
    UInt32 reportedRBGRank1[PMI_INDEX_RANK_1];
    UInt32 reportedRBGRank2[PMI_INDEX_RANK_2];
    /* 4x2 MIMO S */
    UInt32 reportedRBGPMIForPort4[PMI_INDEX_RANK];
    /* 4x2 MIMO E */
    /*+EICIC */
    UInt32 reportedRBGRank1_2[PMI_INDEX_RANK_1];
    UInt32 reportedRBGRank2_2[PMI_INDEX_RANK_2];
    /* 4x2 MIMO S */
    UInt32 reportedRBGPMIForPort4_2[PMI_INDEX_RANK];
    /* 4x2 MIMO E */
   /*-EICIC */
}
RAT0PmiInfo;

typedef struct SubsetSpanPmiInfoT
{
    /*this will store reported PRB in the form of bitmap for each Rank*/
    UInt32 reportedPRBsRank1[PMI_INDEX_RANK_1];
    UInt32 reportedPRBsRank2[PMI_INDEX_RANK_2];
    /* 4x2 MIMO S */
    UInt32 reportedRBGPMIForPort4[PMI_INDEX_RANK];
    /* 4x2 MIMO E */
    /*+EICIC */
    UInt32 reportedPRBsRank1_2[PMI_INDEX_RANK_1];
    UInt32 reportedPRBsRank2_2[PMI_INDEX_RANK_2];
    /* 4x2 MIMO S */
    UInt32 reportedRBGPMIForPort4_2[PMI_INDEX_RANK];
    
   /*-EICIC */

}SubsetSpanPmiInfo;

typedef struct RAT1PmiInfoT
{
    /*this will store reported PRBs for each PMI Index in Zero/NonZero Span of subset form*/
    SubsetSpanPmiInfo spanZeroPmiInfo;
    SubsetSpanPmiInfo spanNonZeroPmiInfo;
}
RAT1PmiInfo;

typedef struct SubbandPMIInfoT
{
    /*this will store reported RBG/PRBs for each PMI Index in RAT0 and RAT1 form*/
    RAT0PmiInfo rat0PmiInfo;
    RAT1PmiInfo rat1PmiInfo[MAX_SUBSETS];
}
SubbandPMIInfo;

    /*This Structure holds information of  RAT 0  and RAT 1 Spatial CQI Information*/
typedef struct CQIInfoSpatialMulT
{
    /*This Structure holds information of  RAT 0 Spatial CQI Information*/
    RAT0SpatialMulCQIInfo rat0SpatialMulCQIInfo;
    /*This Structure holds information of  RAT 1 Spatial CQI Information*/
    RAT1SpatialMulCQIInfo rat1SpatialMulCQIInfo[MAX_SUBSETS]; 
    /*reported subband info per pmiIndex*/
    SubbandPMIInfo        pmiInfo;    
}CQIInfoSpatialMul;

/* - CQI_5.0 */
  /*This Structure holds information of Subband to CQI Mapping*/
typedef struct SubBandToCQInfoT
{
    /*This holds CQI Index for CodeWord One*/
    UInt8 cqiIndexCW1;
    /*This holds CQI Index for CodeWord Two*/
    UInt8 cqiIndexCW2;
    /*This holds PreCoding Matrix Index*/
    UInt8 preCodingMatrixIndex;
    /*This holds MCS Index for CodeWord One*/
    UInt8 mcsIndexCW1;
    /*This holds MCS Index for CodeWord Two*/
    UInt8 mcsIndexCW2;
    /* + CQI_5.0 */
    /*This holds initial Reported CQI for CodeWord One*/
    UInt8 initialReportedCQICW1;
    /*This holds initial Reported CQI for CodeWord Two*/
    UInt8 initialReportedCQICW2;
    /* - CQI_5.0 */
    /* SPR 4769 changes start */
    UInt8 previousTimeAveragedSBCQICW0;
    UInt8 previousTimeAveragedSBCQICW1;
    /* SPR 4769 changes end */
    /*+EICIC */
/*This holds CQI Index for CodeWord One*/
    UInt8 cqiIndexCW1_2;
    /*This holds CQI Index for CodeWord Two*/
    UInt8 cqiIndexCW2_2;
    /*This holds PreCoding Matrix Index*/
    UInt8 preCodingMatrixIndex_2;
    /*This holds MCS Index for CodeWord One*/
    UInt8 mcsIndexCW1_2;
    /*This holds MCS Index for CodeWord Two*/
    UInt8 mcsIndexCW2_2;
    /* + CQI_5.0 */
    /*This holds initial Reported CQI for CodeWord One*/
    UInt8 initialReportedCQICW1_2;
    /*This holds initial Reported CQI for CodeWord Two*/
    UInt8 initialReportedCQICW2_2;
    /* - CQI_5.0 */
    /* SPR 4769 changes start */
    UInt8 previousTimeAveragedSBCQICW0_2;
    UInt8 previousTimeAveragedSBCQICW1_2;
   /*-EICIC */
}SubBandToCQInfo;

  /*This union holds information of Either Spatial Multiplexing
   *or Non Spatial Multiplexing*/
typedef union CQIInfoT
{
    CQIInfoSpatialMul cqiInfoSpatialMul;
    CQIInfoNonSpatialMul cqiInfoNonSpatialMul;
}CQIInfo;

  /*This Structure  holds information Subband to CQI Information 
   * alongwith CQI Information for Spatial and Non Spatial
   *Multiplexing*/
typedef struct SubBandReportInfoT
{
    /*This is array of SubBandToCQInfo structure*/
    /* + CQI_4.1 */ 
    SubBandToCQInfo subBandToCQInfo[MAX_SUBBANDS];
    /* - CQI_4.1 */ 
    CQIInfo cqiInfo;
    /* SPR 9427 changes start */
    /*SPR-15700 Fix start*/
    UInt8 numOfSBperBWpart[4];
    /* SPR 9427 changes end */
    /*+EICIC */
    UInt8 numOfSBperBWpart_2[4];
    /*SPR-15700 Fix end*/
    /*-EICIC */
}SubBandReportInfo;

typedef struct SubBandToRBGMapT
{
    /* indicates the stating RBG index for particular sub band*/        
    SInt8 startRBGIndex;

    /* indicates the last RBG index for particular sub band. 
       In case a sub band contains only one RBG then this field will contain -1*/
    SInt8 endRBGIndex; 
}SubBandToRBGMap;

/* SPR 5599 changes start (ZIP ID 129059) */
/* SPR 7435 Changes Starts */
extern  UInt8 getMCSFromCQI(UInt8 cqiIndex
              );
/* SPR 7435 Changes Ends */

/*CA Changes start  */
extern void processFeedBackReportQueue(UInt8 numCQINodes, InternalCellIndex cellIndex);
/*CA Changes end  */
/* SPR 5599 changes end (ZIP ID 129059) */
/* HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
extern  UInt8 getHDReducedMCSFromCQI(UInt8 cqiIndex);
#endif
/* HD FDD Changes End*/

void initCQIMgr(UInt8 numOfCells);
void deinitCQIMgr(InternalCellIndex internalCellIndex);

void initCQI( InternalCellIndex internalCellIndex);
void cleanupCQI(InternalCellIndex internalCellIndex);
void deInitCQI(InternalCellIndex internalCellIndex);

#endif

