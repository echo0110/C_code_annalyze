/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 * $Id: lteMacCQI.c,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ******************************************************************************
 *
 * File Description : This file handles the processing of CQI Report.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacCQI.c,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.21  2009/11/01 11:40:05  gur11912
 * NUM_CQI_NODES added
 *
 * Revision 1.20  2009/09/03 15:30:51  gur20052
 *  Added WARNING
 *
 * Revision 1.19  2009/08/08 10:02:04  gur12140
 * Solved Bug
 *
 * Revision 1.18  2009/08/06 05:47:03  gur12140
 * Solved Bug
 *
 * Revision 1.17  2009/08/04 05:56:22  gur12140
 * Incorporated Comments
 *
 * Revision 1.16  2009/07/31 06:01:19  gur12140
 * Solved Bug
 *
 * Revision 1.15  2009/07/29 17:18:07  gur12140
 * Removed Warnings
 *
 * Revision 1.14  2009/07/28 13:52:27  gur12140
 * Solved Bug
 *
 * Revision 1.13  2009/07/28 07:10:25  gur12140
 * Solved Bug
 *
 * Revision 1.12 2009/07/20 13:30:24 gur12140
 * Solved Bug
 *
 * Revision 1.11 2009/07/20 08:15:49 gur12140
 * Solved Buy Related to Mempry
 *
 * Revision 1.10 2009/07/20 05:21:14 gur12140
 * Replaced MACModuleLogDetail_g wih CQI_MGR
 *
 * Revision 1.9 2009/07/02 06:40:52 gur12140
 * Incorporated Comments
 *
 * Revision 1.8 2009/06/23 11:59:37 gur12140
 * Incorporated Comments
 *
 * Revision 1.7 2009/06/16 11:53:07 gur12140
 * Changed funtionality wrt to Mode
 *
 * Revision 1.6 2009/06/05 11:01:13 gur12140
 * Removed Linking Errors
 *
 * Revision 1.5 2009/06/03 11:56:27 gur12140
 * Added Funtionality for PMI and RI Processing
 *
 * Revision 1.4 2009/06/03 11:40:10 gur12140
 * Added Funtionnality for CQI Report Processing
 *
 * Revision 1.3 2009/06/02 13:22:04 gur18569 
 * added func definition of pushNodeInCqiQueue
 *
 * Revision 1.2 2009/05/21 08:32:16 gur12140
 * GetMCS Funtion Updated
 *
 * Revision 1.1 2009/05/04 07:16:24 gur12140
 * empty log message
 *
 *
 *****************************************************************************/
/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
#include "lteMacCQI.h"
#include "lteMacDLUEContext.h"
#include "lteMacDLResourceAllocator.h"
#include "lteMacPhyInterface.h"
#include "lteMacReceiver.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacDLHarqMgr.h"
/* ICIC changes start */
#include "lteMacCellConfMgr.h"
#include "lteMacDLResourceInfoMgr.h"
#include "lteMacRRMPeriodicMgr.h"
/* ICIC changes end */
/******************************************************************************
 * Project Includes
 *****************************************************************************/
/******************************************************************************
  Private Definitions
 *****************************************************************************/
/******************************************************************************
  Private Types
 *****************************************************************************/
/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* + CQI_4.1 */

/* SPR 13498 fix start */
/* Corrected signature of functions */
STATIC  UInt8 getEffectiveDLCQI(UInt8 timeAveragedCQI,
                                      /* + CQI_5.0 */
                                      SInt16 cqiCorrectionFactor,
                                      DLUEContext *dlUEContext_p);
                                      /* - CQI_5.0 */

STATIC  UInt8 calculateAverageDLCQI(UInt8 currentCQI, 
                                          UInt8 previousCQI);

/* SPR 9427 changes start */
STATIC  UInt8 calculateAverageDLCQIforAP(UInt8 currentCQI, 
                                               UInt8 previousCQI);
/* SPR 9427 changes end */
/* SPR 13498 fix end */

/* - CQI_4.1 */
/* 4x4 DL MIMO CHG START */
/* +- SPR 17777 */
static  UInt8 isPMIInCodebookSubsetRestriction(
        UInt8 *cbsrValue 
#ifdef CSI_VALIDATION
        ,UInt8 pmiUE,
                                                      UInt8 RI,
                                                      UInt8 ueCategory,
                                                      UInt8 transMode,
        UInt8 numOfTxAnteenas
#endif
                                                      );
/* +- SPR 17777 */
/* 4x4 DL MIMO CHG END */
STATIC  void resetRAT0CW1SpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 oldCqiIndexCW1,
        CQIInfoSpatialMul *cqiInfoSpatialMul_p);
/*+EICIC*/
STATIC  void resetRAT0CW1SpatialMulSubBandInfo_2 (UInt32 subBandNum,
       UInt32 oldCqiIndexCW1,
       CQIInfoSpatialMul *
       cqiInfoSpatialMul_p);

/*-EICIC*/

/* CA Changes start */
STATIC  void updateRAT0CW1SpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 newCqiIndexCW1,
        /* + CQI_5.0 */
        UInt8 subbandPMIValue,
        /* - CQI_5.0 */
        CQIInfoSpatialMul *cqiInfoSpatialMul_p,
        UInt8 numOfTxAnteenas  );
/* CA Changes end */

STATIC  void resetRAT0CW2SpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 oldCqiIndexCW2,
        CQIInfoSpatialMul *cqiInfoSpatialMul_p);

/* CA Changes start */
STATIC  void updateRAT0CW2SpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 newCqiIndexCW2,
        /* + CQI_5.0 */
        UInt8 subbandPMIValue,
        /* - CQI_5.0 */
        CQIInfoSpatialMul *cqiInfoSpatialMul_p,
       UInt8 numOfTxAnteenas);
/* CA Changes end */

STATIC  void updateRAT0CW1NonSpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 newCqiIndexCW1,
        /* + CQI_4.1 */
        UInt32 oldCqiForRBG,
        /* - CQI_4.1 */
        CQIInfoNonSpatialMul *cqiInfoNonSpatialMul_p);

STATIC  void resetRAT0CW1NonSpatialMulSubBandInfo(
        UInt32 subBandNum,
        /* + CQI_4.1 */
        UInt32 newCqiIndexCW1,
        /* - CQI_4.1 */
        UInt32 oldCqiIndexCW1,
        CQIInfoNonSpatialMul *cqiInfoNonSpatialMul_p);

STATIC  void resetRAT1CW1SpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 oldCqiIndexCW1,
        CQIInfoSpatialMul *cqiInfoSpatialMul_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p);


/*+EICIC .*/
STATIC  void resetRAT0CW1NonSpatialMulSubBandInfo_2 (UInt32 subBandNum,
          /* + CQI_4.1 */
          UInt32
          newCqiIndexCW1,
          /* - CQI_4.1 */
          UInt32
          oldCqiIndexCW1,
          CQIInfoNonSpatialMul
          *
          cqiInfoNonSpatialMul_p);

STATIC  void resetRAT1CW1SpatialMulSubBandInfo_2 (UInt32 subBandNum,
       UInt32 oldCqiIndexCW1,
       CQIInfoSpatialMul *
       cqiInfoSpatialMul_p,
       RAT0Info * rat0Info_p,
       RAT1Info *
       rat1Info_p);

STATIC  void updateRAT1CW1NonSpatialMulSubBandInfo_2 (UInt32 rbgIndex,
           UInt32
           newCqiIndexCW1,
           UInt32
           oldCqiForRBG,
           CQIInfoNonSpatialMul
           *
           cqiInfoNonSpatialMul_p,
           RAT0Info *
           rat0Info_p,
           RAT1Info *
           rat1Info_p,
           UInt16 *
           tempReportedCQIBitmapPerSubset_p);
STATIC  void updateRAT0CW1NonSpatialMulSubBandInfo_2 (UInt32 subBandNum,
           UInt32
           newCqiIndexCW1,
           /* + CQI_4.1 */
           UInt32
           oldCqiForRBG,
           /* - CQI_4.1 */
           CQIInfoNonSpatialMul
           *
           cqiInfoNonSpatialMul_p);
STATIC  void resetRAT0CW1SpatialMulSubBandInfo_2 (UInt32 subBandNum,
       UInt32 oldCqiIndexCW1,
       CQIInfoSpatialMul *
       cqiInfoSpatialMul_p);
STATIC  void resetRAT1CW1NonSpatialMulSubBandInfo_2 (UInt32 subBandNum,
          /* + CQI_4.1 */
          UInt32
          newCqiIndexCW1,
          /* - CQI_4.1 */
          UInt32
          oldCqiIndexCW1,
          CQIInfoNonSpatialMul
          *
          cqiInfoNonSpatialMul_p,
          RAT0Info *
          rat0Info_p,
          RAT1Info *
          rat1Info_p);
STATIC  void updateRAT0CW1SpatialMulSubBandInfo_2 (UInt32 subBandNum,
        UInt32
        newCqiIndexCW1,
        /* + CQI_5.0 */
        UInt8
        subbandPMIValue,
        /* - CQI_5.0 */
        CQIInfoSpatialMul *
        cqiInfoSpatialMul_p,
        UInt8
        numOfTxAnteenas);
STATIC  void updateRAT1CW1SpatialMulSubBandInfo_2 (UInt32 subBandNum,
        UInt32
        newCqiIndexCW1,
        /* + CQI_5.0 */
        UInt8
        subbandPMIValue,
        /* - CQI_5.0 */
        CQIInfoSpatialMul *
        cqiInfoSpatialMul_p,
        RAT0Info *
        rat0Info_p,
        RAT1Info *
        rat1Info_p,
        /* CA Changes start */
        UInt8
        numOfTxAnteenas);
  /* CA Changes end */
STATIC  void resetRAT0CW2SpatialMulSubBandInfo_2 (UInt32 subBandNum,
       UInt32 oldCqiIndexCW2,
       CQIInfoSpatialMul *
       cqiInfoSpatialMul_p);
STATIC  void resetRAT1CW2SpatialMulSubBandInfo_2 (UInt32 subBandNum,
       UInt32 oldCqiIndexCW2,
       CQIInfoSpatialMul *
       cqiInfoSpatialMul_p,
       RAT0Info * rat0Info_p,
       RAT1Info *
       rat1Info_p);
STATIC  void updateRAT0CW2SpatialMulSubBandInfo_2 (UInt32 subBandNum,
        UInt32
        newCqiIndexCW2,
        /* + CQI_5.0 */
        UInt8
        subbandPMIValue,
        /* - CQI_5.0 */
        CQIInfoSpatialMul *
        cqiInfoSpatialMul_p,
        UInt8
        numOfTxAnteenas);
/*-EICIC .*/
STATIC  void updateRAT1CW1SpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 newCqiIndexCW1,
        /* + CQI_5.0 */
        UInt8 subbandPMIValue,
        /* - CQI_5.0 */
        CQIInfoSpatialMul *cqiInfoSpatialMul_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p,
         /* CA Changes start */ 
        UInt8 numOfTxAnteenas);
         /* CA Changes end */ 

STATIC  void checkSubBandRangeOfNewCQIIndexCW1(
        UInt32 newCqiIndexCW1,
        UInt32 subsetBitMap,
        UInt32 subsetBitMapZeroSpan,
        UInt32 subsetBitMapNonZeroSpan,
        RAT1SpatialMulCQIInfo *rat1CQIInfo_p);

STATIC  void resetRAT1CW2SpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 oldCqiIndexCW2,
        CQIInfoSpatialMul *cqiInfoSpatialMul_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p);

STATIC  void updateRAT1CW2SpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 newCqiIndexCW2,
        /* + CQI_5.0 */
        UInt8 subbandPMIValue,
        /* - CQI_5.0 */
        CQIInfoSpatialMul *cqiInfoSpatialMul_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p,
         /* CA Changes start */ 
        UInt8 numOfTxAnteenas);
         /* CA Changes end */ 

STATIC  void checkSubBandRangeOfNewCQIIndexCW2(
        UInt32 newCqiIndexCW2,
        UInt32 subsetBitMap,
        UInt32 subsetBitMapZeroSpan,
        UInt32 subsetBitMapNonZeroSpan,
        RAT1SpatialMulCQIInfo *rat1CQIInfo_p);

STATIC  void resetRAT1CW1NonSpatialMulSubBandInfo(
        UInt32 subBandNum,
        /* + CQI_4.1 */
        UInt32 newCqiIndexCW1,
        /* - CQI_4.1 */
        UInt32 oldCqiIndexCW1,
        CQIInfoNonSpatialMul *cqiInfoNonSpatialMul_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p);

/* + CQI_4.1 */
STATIC  void updateRAT1CW1NonSpatialMulSubBandInfo(
        UInt32 rbgIndex,
        UInt32 newCqiIndexCW1,
        UInt32 oldCqiForRBG,
        CQIInfoNonSpatialMul *cqiInfoNonSpatialMul_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p,
        UInt16 *tempReportedCQIBitmapPerSubset_p
        );
/* + CQI_4.1 */

STATIC  void processCQIMode10Type3(
        PeriodicMode10Type3 *periodicMode10Type3_p,
        DLUEContext *dlUEContext_p,InternalCellIndex 
		internalCellIndex);

STATIC  void processCQIMode11Type3(
        PeriodicMode11Type3 *periodicMode11Type3_p,
        DLUEContext *dlUEContext_p,InternalCellIndex
		internalCellIndex);
/* CA Changes start */ 
STATIC  void processCQIMode20Type3(
        PeriodicMode20Type3 *periodicMode20Type3_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex);

STATIC  void processCQIMode21Type3(
        PeriodicMode21Type3 *periodicMode21Type3_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex);

STATIC  void processCQIMode10Type4(
        PeriodicMode10Type4 *periodicMode10Type4_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex);

STATIC  void processCQIMode20Type4(
        PeriodicMode20Type4 *periodicMode20Type4_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex);

STATIC  void processCQIMode11Type2(
        PeriodicMode11Type2 *periodicMode11Type2_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex);
STATIC  void processCQIMode21Type2(
        PeriodicMode21Type2 *periodicMode21Type2_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex);


STATIC  void processCQIMode20Type1(
        PeriodicMode20Type1 *periodicMode20Type1_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex);
STATIC void processCQIMode20(
        AperiodicCQIMode20 *aperiodicCQIMode20_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex);

STATIC void processCQIMode31(
        AperiodicCQIMode31 *qperiodicCQIMode31_p,
        DLUEContext *dlUEContext_p
         ,InternalCellIndex internalCellIndex);

STATIC void checkPmiInCbsrForSubband(
        UInt32 *subbandPMIValue_p,
/* +- SPR 17777 */
#ifdef CSI_VALIDATION
        AperiodicCQIMode22 *aperiodicCQIMode22_p,
#endif
/* +- SPR 17777 */
        DLUEContext *dlUEContext_p,
        SubBandToCQInfo *subBandToCQInfo_p,
        InternalCellIndex internalCellIndex);

STATIC void checkPmiInCbsrForWideband(
        AperiodicCQIMode22 *aperiodicCQIMode22_p,
        DLUEContext *dlUEContext_p,
        DLCQIInfo *dlCQIInfo_p,
        InternalCellIndex internalCellIndex);

STATIC void processCQIMode22(
        AperiodicCQIMode22 *aperiodicCQIMode22_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex);

STATIC void processCQIMode12(
        AperiodicCQIMode12 *periodicCQIMode12_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex);

STATIC void processCQIMode30(
        AperiodicCQIMode30 *aperiodicCQIMode30_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex);

STATIC  void processCQIMode21Type1(
        PeriodicMode21Type1 *periodicMode21Type1_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex);

STATIC void populateSubBandToRBGMap(
        UInt32 nbResourceBlocks, 
        UInt32 periodicSubBandSize,
        UInt32 aperiodicSubBandSize
   ,InternalCellIndex internalCellIndex);
/* + CQI_5.0 */
STATIC  void resetCW2Info(SubBandReportInfo *subBandReportInfo_p
                               ,InternalCellIndex internalCellIndex);
/*  CQI_5.0 */
/* CA Changes end */ 
/*+EICIC .*/
 UInt8 checkAbsCsiReport (/* +- SPR 17777 */
    UInt8 cqiPeriodicity,
    UInt16 cqiSubFrameOffset);
 UInt8 checkAbsRiReport (/* +- SPR 17777 */
          UInt8 cqiPeriodicity,
          UInt16 cqiSubFrameOffset,
          UInt8 riPeriodicity, UInt16 riSubFrameOffset);
extern UInt8 csiMeasurementSubset2_g;
UInt8 aperiodicAbs_g = 0;
UInt32 perSubframe_g=0;
UInt32 perSystemframe_g=0;
STATIC  void checkSubBandRangeOfNewCQIIndexCW1_2 (UInt32 newCqiIndexCW1,
       UInt32 subsetBitMap,
       UInt32
       subsetBitMapZeroSpan,
       UInt32
       subsetBitMapNonZeroSpan,
       RAT1SpatialMulCQIInfo
       * rat1CQIInfo_p);
STATIC  void checkSubBandRangeOfNewCQIIndexCW2_2 (UInt32 newCqiIndexCW2,
       UInt32 subsetBitMap,
       UInt32
       subsetBitMapZeroSpan,
       UInt32
       subsetBitMapNonZeroSpan,
       RAT1SpatialMulCQIInfo
       * rat1CQIInfo_p);
STATIC void checkPmiInCbsrForWideband_2 (
       AperiodicCQIMode22 *aperiodicCQIMode22_p,
      DLUEContext * dlUEContext_p,
      DLCQIInfo * dlCQIInfo_p,
      InternalCellIndex internalCellIndex);
STATIC void checkPmiInCbsrForSubband_2 (UInt32 * subbandPMIValue_p,
/* +- SPR 17777 */
#ifdef CSI_VALIDATION
     AperiodicCQIMode22 *
     aperiodicCQIMode22_p,
#endif
/* +- SPR 17777 */
     DLUEContext * dlUEContext_p,
     SubBandToCQInfo * subBandToCQInfo_p,
     InternalCellIndex internalCellIndex);
STATIC  void resetCW2Info_2 (SubBandReportInfo * subBandReportInfo_p,
       InternalCellIndex internalCellIndex);

#define CHECK_APERIODIC_SECONDARY_CONDITION(ulUEContext_p,internalCellIndex)\
if(((ulUEContext_p->ueComplianceRelease == RELEASE_10))&&\
((((NON_VICTIM_UE == ulUEContext_p->userType)&&( LTE_TRUE== aperiodicAbs_g)))\
||\
  ((VICTIM_UE == ulUEContext_p->userType)&&(LTE_FALSE==  aperiodicAbs_g))))



/*-EICIC .*/

/* EICIC +*/

#define CHECK_PERIODIC_SECONDARY_CONDITION_FOR_RI(ulUEContext_p,internalCellIndex)\
    if(((ulUEContext_p->pucchConfigInfo.riConfigIndex_2  != MAC_RI_CONFIG_INDEX_INVALID)&& \
                (ulUEContext_p->ueComplianceRelease == RELEASE_10))&&\
            (((NON_VICTIM_UE == ulUEContext_p->userType)&&(LTE_TRUE== checkAbsRiReport(\
                                                                                       ulUEContext_p->pucchConfigInfo.cqiPeriodicity_2,\
                                                                                       ulUEContext_p->pucchConfigInfo.cqiOffset_2,\
                                                                                       ulUEContext_p->pucchConfigInfo.riPeriodicity_2,\
                                                                                       ulUEContext_p->pucchConfigInfo.riOffset_2)))\
             ||\
             ((VICTIM_UE == ulUEContext_p->userType)&&(LTE_FALSE== checkAbsRiReport(\
                     ulUEContext_p->pucchConfigInfo.cqiPeriodicity_2,\
                     ulUEContext_p->pucchConfigInfo.cqiOffset_2,\
                     ulUEContext_p->pucchConfigInfo.riPeriodicity_2,\
                     ulUEContext_p->pucchConfigInfo.riOffset_2)))))\


#define CHECK_PERIODIC_SECONDARY_CONDITION_FOR_CSI(ulUEContext_p,internalCellIndex)\
    if(((ulUEContext_p->pucchConfigInfo.cqiPmiConfigIndex_2 != MAC_MAX_CQI_PMI_CONFIG_INDEX + 1)&& \
                (ulUEContext_p->ueComplianceRelease == RELEASE_10))&&\
            (((NON_VICTIM_UE == ulUEContext_p->userType)&&( LTE_TRUE== checkAbsCsiReport(ulUEContext_p->pucchConfigInfo.cqiPeriodicity_2,ulUEContext_p->pucchConfigInfo.cqiOffset_2)))\
             || \
             ((VICTIM_UE == ulUEContext_p->userType)&&(LTE_FALSE == checkAbsCsiReport(ulUEContext_p->pucchConfigInfo.cqiPeriodicity_2,ulUEContext_p->pucchConfigInfo.cqiOffset_2)))))\
/* EICIC -*/

/* Cyclomatic_complexity_changes_start */
void updateSubBandInforForCW0AndCW1SpatialMul(UInt32 rbgIndexCount,
        UInt32 startRBGIndex,
        SubBandReportInfo *subBandReportInfo_p,
        PeriodicMode21Type1 *periodicMode21Type1_p,
        DLCQIInfo *dlCQIInfo_p,
        CQIInfo *cqiInfo_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p,
        UInt8 numOfTxAnteenas,
        UInt8 toProcessCW2,
        DLUEContext *dlUEContext_p
        );
void updateSubBandInforForCW0AndCW1SpatialMul_2 (UInt32 rbgIndexCount,
            UInt32 startRBGIndex,
            SubBandReportInfo *
            subBandReportInfo_p,
            PeriodicMode21Type1 *
            periodicMode21Type1_p,
            DLCQIInfo * dlCQIInfo_p,
            CQIInfo * cqiInfo_p,
            RAT0Info * rat0Info_p,
            RAT1Info * rat1Info_p,
            UInt8 numOfTxAnteenas,
            UInt8 toProcessCW2,
            DLUEContext * dlUEContext_p
            );
/* Cyclomatic_complexity_changes_end */

/******************************************************************************
 * Private Constants
 *****************************************************************************/
/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* ICIC changes start */
/* CA Changes start */
extern DLResourceAllocationInfo *dlRAInfo_gp[MAX_NUM_CELL];
/* CA Changes end */
/* ICIC changes end */

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/* Table 7.2.3-1 of 3GPP TS 36.213 will be used to find the efficiency against
   the given CQI Index. With this Efficiency another Table(MCS Table: HLD) will
   be looked up to give the MCS index.*/
static SInt8 cqiMCSArr_g[MAX_CQI_INDEX] = {/*0.0000*/-1,
    /*0.1523*/0,
    /*0.2344*/ 0,
    /*0.3770*/ 2,
    /*0.6016*/ 4,
    /*0.8770*/ 6,
    /*1.1758*/ 8,
    /*1.4766*/ 11,
    /*1.9141*/ 13,
    /*2.4063*/ 15,
    /*2.7305*/ 18,
    /*3.3223*/ 20,
    /*3.9023*/ 22,
    /*4.5234*/ 24,
    /*5.1152*/ 26,
    /*5.5547*/ 28 };
/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
extern UInt8 hdFddReducedMcsTableArr_g[MAX_MCS_VALUES];
#endif
/*HD FDD Changes End*/
/* CA Changes start */
typedef struct CQIGlobalParamsT
{
    UInt8 numOfSubBandInbandWidthPart;
    /* SPR 18122 Changes Start*/
    SubBandToRBGMap subBandToRBGMapPeriodic[DIVIDE_BY_TWO(MAX_SUBBANDS)];
    /* SPR 18122 Changes End*/
    SubBandToRBGMap subBandToRBGMapAperiodic[MAX_SUBBANDS];

} CQIGlobalParams;

CQIGlobalParams *cqiGlobalParams_gp[MAX_NUM_CELL];
/* CA Changes end */
/* + CQI_5.0 */

/********************************************************************************
 * Function Name  : updateRAT1CW2SpatialMulSubBandInfo_2
 * Inputs         : subBandNum - This holds subBand Number whose Information
 *                               need to be updated,
 *                  newCqiIndexCW2 - This holds Old CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  subbandPMIValue - PMI Value of subband,
 *                  rat0Info_p - This stores the address of RAT0Info structure,
 *                  rat1Info_p - This stores the RAT1Info structure
 *                  numOfTxAnteenas -
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will update  the RAT 1 Information stored in  
 *                  cqiInfoSpatialMul_p structure for  Spatial Multiplexing.
 ********************************************************************************/

static  void
updateRAT1CW2SpatialMulSubBandInfo_2 (UInt32 subBandNum,
          UInt32 newCqiIndexCW2,
          /* + CQI_5.0 */
          UInt8 subbandPMIValue,
          /* + CQI_5.0 */
          CQIInfoSpatialMul * cqiInfoSpatialMul_p,
          RAT0Info * rat0Info_p,
          RAT1Info * rat1Info_p,
          /* CA Changes start */
          UInt8 numOfTxAnteenas)
       /* CA Changes end */
{
  RAT1SpatialMulCQIInfo *rat1CQIInfo_p = PNULL;
  SubsetInfo *subSetInfo_p = PNULL;
  UInt32 subsetBitMap = 0;
  UInt32 subSetIndex = 0;
  UInt32 subsetBitMapZeroSpan = 0;
  UInt32 subsetBitMapNonZeroSpan = 0;
  RBGIndexInfo *rbgIndexInfo_p = PNULL;
  /* + CQI_5.0 */
  RAT1PmiInfo *rat1PmiInfo_p = PNULL;
  UInt32 tempZeroBitMap = 0;
  UInt32 tempNonZeroBitMap = 0;
  /* - CQI_5.0 */
  /* TDD SB_CQI */
  UInt8 iteration = 0;
  /* TDD SB_CQI */

  rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[subBandNum];
  subSetIndex = rbgIndexInfo_p->subsetIndex;
  rat1CQIInfo_p = &cqiInfoSpatialMul_p->rat1SpatialMulCQIInfo[subSetIndex];
  /* + CQI_5.0 */
  rat1PmiInfo_p = &cqiInfoSpatialMul_p->pmiInfo.rat1PmiInfo[subSetIndex];
  /* - CQI_5.0 */
  subSetInfo_p = &rat1Info_p->subsetInfo[subSetIndex];
  subsetBitMap = ~(rbgIndexInfo_p->rbgSizeBitMapForSubset);
  subsetBitMapZeroSpan = subSetInfo_p->bitMapWithSpanZero;
  subsetBitMapNonZeroSpan = subSetInfo_p->bitMapWithSpanNonZero;
  rat1CQIInfo_p->subsetCQIBitMapALLCQIIndex_2 |= subsetBitMap;

  /*Checking in which range new CQI Index falls */
  /* + CQI_4.1 */

  /* + CQI_5.0 */
  if (newCqiIndexCW2)
    {
      /* Cyclomatic Complexity changes start */
      checkSubBandRangeOfNewCQIIndexCW2_2 (newCqiIndexCW2, subsetBitMap,
        subsetBitMapZeroSpan,
        subsetBitMapNonZeroSpan,
        rat1CQIInfo_p);
      /* Cyclomatic Complexity changes end */
    }

  /*4x2 MIMO S */
  /* CA Changes start */
  if ((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2) &&
      (INVALID_PMI_VALUE != subbandPMIValue) &&
      (subbandPMIValue < PMI_INDEX_RANK_2))
    /* CA Changes end */
    {
      tempZeroBitMap = subsetBitMap & subsetBitMapZeroSpan;
      tempNonZeroBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
      if (tempZeroBitMap)
 {
   /* TDD SB_CQI */
   for (iteration = 0; iteration < PMI_INDEX_RANK_2; iteration++)
     {
       if (subbandPMIValue == iteration)
  {
    rat1PmiInfo_p->
      spanZeroPmiInfo.reportedPRBsRank2_2[iteration] |=
      tempZeroBitMap;
  }
       else
  {
    rat1PmiInfo_p->
      spanZeroPmiInfo.reportedPRBsRank2_2[iteration] &=
      (~tempZeroBitMap);
  }
     }
   /* TDD SB_CQI */
 }
      if (tempNonZeroBitMap)
 {
   /* TDD SB_CQI */
   for (iteration = 0; iteration < PMI_INDEX_RANK_2; iteration++)
     {
       if (subbandPMIValue == iteration)
  {
    rat1PmiInfo_p->
      spanNonZeroPmiInfo.reportedPRBsRank2_2[iteration] |=
      tempNonZeroBitMap;
  }
       else
  {
    rat1PmiInfo_p->
      spanNonZeroPmiInfo.reportedPRBsRank2_2[iteration] &=
      (~tempNonZeroBitMap);
  }
     }
   /* TDD SB_CQI */
 }
    }
  /* CA Changes start */
  else if ((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4) &&
    (INVALID_PMI_VALUE != subbandPMIValue) &&
    (subbandPMIValue < PMI_INDEX_RANK))
    /* CA Changes end */
    {
      tempZeroBitMap = subsetBitMap & subsetBitMapZeroSpan;
      tempNonZeroBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
      if (tempZeroBitMap)
 {
   rat1PmiInfo_p->
     spanZeroPmiInfo.reportedRBGPMIForPort4_2[subbandPMIValue] |=
     tempZeroBitMap;
 }
      if (tempNonZeroBitMap)
 {
   rat1PmiInfo_p->
     spanNonZeroPmiInfo.reportedRBGPMIForPort4_2[subbandPMIValue] |=
     tempNonZeroBitMap;
 }
    }
  /*4x2 MIMO E */
  /* - CQI_5.0 */
  return;
}


/*-EICIC .*/
/*KLOCKWORK_CHANGES_START_5_6_7_8*/

/*****************************************************************************
 * Function Name  : getNumOfSubBandInBandwidth
 * Inputs         : dlRBs   - Number of DL RBs
 *		          : periodicSubBandSize -Periodic Sub band Size.
 * Outputs        : numOfSubBandInbandWidthPart_p -  Pointer to the 
			     	number of subbands in BW 
 * Returns        : None
 * Description    : This function determines the number of subbands in Bandwidth 
 *****************************************************************************/
static   void  getNumOfSubBandInBandwidth(UInt8 *numOfSubBandInbandWidthPart_p,
                                       UInt32 dlRBs,
				                       UInt8  periodicSubBandSize)
{
	if( dlRBs <= 10)
	{
		*numOfSubBandInbandWidthPart_p = 
			((( dlRBs - 1)/ periodicSubBandSize) + 1);
	}
	else 
	{
        /*SPR 20899 Fix Start*/
		*numOfSubBandInbandWidthPart_p = 
			( ( (dlRBs - 1) /
			    (periodicSubBandSize * ( DIVIDE_BY_TWO(periodicSubBandSize) ))) + 1 );
        /*SPR 20899 Fix Stop*/
	}
}

/*KLOCKWORK_CHANGES_STOP_5_6_7_8*/

/* CA Changes start */

/*****************************************************************************
 * Function Name  : initCQIMgr
 * Inputs         : numOfCells - number of configured cell
 * Outputs        : None
 * Returns        : None
 * Description    : This function will initialize memory for CQI manager. 
 *                  This function is called at MAC_INIT_LAYER_REQ
 *****************************************************************************/
void initCQIMgr(UInt8 numOfCells)
{
    createMemPool(sizeof(CQIGlobalParams), numOfCells);
    /*CA Changes start  */
    CONSTRUCT_AND_INIT_DL_SCHEDULER_REPORT_QUEUE_Q(numOfCells);
    /*CA Changes end  */
    return;
}

/*****************************************************************************
 * Function Name  : deinitCQIMgr
 * Inputs         : internalCellIndex - internal cell id 
 * Outputs        : None
 * Returns        : None
 * Description    : This function will de-initialize memory for CQI manager. 
 *                  This function is called at MAC_CLEANUP_LAYER_REQ
 *****************************************************************************/
void deinitCQIMgr(InternalCellIndex internalCellIndex)
{
    QDEINIT_NODE_DL_SCHEDULER_REPORT_QUEUE_Q(internalCellIndex);
    return;
}
/* CA Changes end */

/* + CQI_5.0 */

/*****************************************************************************
 * Function Name  : initCQI
 * Inputs         : internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function will initialize the 
 *                  numOfSubBandInbandWidthPart global variable with number
 *                  of subBands in the SubBand in Bandwidth Part
 *                  This function is called at cell Config
 *****************************************************************************/
/* CA Changes start */
void initCQI( InternalCellIndex internalCellIndex)
{
    UInt32 dlRBs = 0;
    UInt8 periodicSubBandSize =0;
    UInt8 aperiodicSubBandSize =0;
    UInt8 subBandIndex = 0;
    dlRBs = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->dlAvailableRBs;

    cqiGlobalParams_gp[internalCellIndex] =
        (CQIGlobalParams *) getMemFromPool(sizeof(CQIGlobalParams),PNULL);     

    if(dlRBs <= 10)
    {
        periodicSubBandSize = 4;
        aperiodicSubBandSize = 2;

        /* + CQI_5.0 */
        getNumOfSubBandInBandwidth(
                &cqiGlobalParams_gp[internalCellIndex]->numOfSubBandInbandWidthPart,
                dlRBs,  periodicSubBandSize);
        /* - CQI_5.0 */
    }
    else if(dlRBs >= 11 && dlRBs <= 26)
    {
        periodicSubBandSize = 4;
        aperiodicSubBandSize = 2;

        /* + CQI_5.0 */
        getNumOfSubBandInBandwidth(
                &cqiGlobalParams_gp[internalCellIndex]->numOfSubBandInbandWidthPart,
                dlRBs, 
                periodicSubBandSize);
        /* - CQI_5.0 */
    }
    else if(dlRBs >= 27 && dlRBs <=63)
    {
        periodicSubBandSize = 6;
        aperiodicSubBandSize = 3;

        /* + CQI_5.0 */
        getNumOfSubBandInBandwidth(
                &cqiGlobalParams_gp[internalCellIndex]->numOfSubBandInbandWidthPart,
                dlRBs, periodicSubBandSize);
        /* - CQI_5.0 */
    } 
    /* +COVERITY 10714 */
    else if(dlRBs >= 64 && dlRBs <= 100)
        /* -COVERITY 10714 */
    { 
        periodicSubBandSize = 8;
        aperiodicSubBandSize = 4;

        /* + CQI_5.0 */
        getNumOfSubBandInBandwidth(
                &cqiGlobalParams_gp[internalCellIndex]->numOfSubBandInbandWidthPart,
                dlRBs, periodicSubBandSize);
        /* - CQI_5.0 */
    } 
    for(subBandIndex = 0; subBandIndex < MAX_SUBBANDS; subBandIndex++)
    { 
        /* SPR 18122 Changes Start*/
        if(subBandIndex < DIVIDE_BY_TWO(MAX_SUBBANDS))
        /* SPR 18122 Changes End*/
        { 
            cqiGlobalParams_gp[internalCellIndex]->\
                subBandToRBGMapPeriodic[subBandIndex].startRBGIndex = -1; 
            cqiGlobalParams_gp[internalCellIndex]->\
                subBandToRBGMapPeriodic[subBandIndex].endRBGIndex = -1;
            cqiGlobalParams_gp[internalCellIndex]->\
                subBandToRBGMapAperiodic[subBandIndex].startRBGIndex = -1;
            cqiGlobalParams_gp[internalCellIndex]->\
                subBandToRBGMapAperiodic[subBandIndex].endRBGIndex = -1;
        }
        else
        {
            cqiGlobalParams_gp[internalCellIndex]->\
                subBandToRBGMapAperiodic[subBandIndex].startRBGIndex = -1;
            cqiGlobalParams_gp[internalCellIndex]->\
                subBandToRBGMapAperiodic[subBandIndex].endRBGIndex = -1;
        }
    }
    if(dlRBs >= 8)
    {
        /* if number of resource blocks is 9 then subBand Indexes will be 
           0, 1, 2 */
        /* subBandIndex 0 - startRBGIndex = 0
           subBandIndex 0 - endRBGIndex = 3
           subBandIndex 1 - startRBGIndex = 4
           subBandIndex 1 - endRBGIndex = 7
           subBandIndex 2 - startRBGIndex = 8
           subBandIndex 2 - endRBGIndex = 8*/ 
        populateSubBandToRBGMap(dlRBs, periodicSubBandSize, 
                aperiodicSubBandSize,internalCellIndex);
    }
}

/*****************************************************************************
 * Function Name  : cleanupCQI
 * Inputs         : internalCellIndex - internal cell id 
 * Outputs        : None
 * Returns        : None
 * Description    : This function will free memory for CQI manager. 
 *                  This function is called at cell stop
 *****************************************************************************/
void cleanupCQI(InternalCellIndex internalCellIndex)
{
    UInt32 count=0,loopVar=0;
    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;

    /* deallocating circular Queue dlSchedulerReportQueue */
    count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo, (void *)&ueReportedCQINodeInfo_p, internalCellIndex);
        if (ueReportedCQINodeInfo_p)
        {
            FREE_MEM_NODE_DL_SCHEDULER_REPORT_QUEUE_Q(ueReportedCQINodeInfo_p);
        }
    }
    return;
}

/*****************************************************************************
 * Function Name  : deInitCQI
 * Inputs         : internalCellIndex - internal cell id 
 * Outputs        : None
 * Returns        : None
 * Description    : This function will free memory for CQI manager. 
 *                  This function is called at cell delete
 *****************************************************************************/
void deInitCQI(InternalCellIndex internalCellIndex)
{
    if (PNULL != cqiGlobalParams_gp[internalCellIndex])
    {
         freeMemPool(cqiGlobalParams_gp[internalCellIndex]);
         cqiGlobalParams_gp[internalCellIndex] = PNULL;
    }   
    return;
}

/* CA Changes end */
/*****************************************************************************
 * Function Name  : populateSubBandToRBGMap 
 * Inputs         : nbResourceBlocks - Number of Resource Blocks configured,  
 *                  periodicSubBandSize - Size of Sub Band in case of Periodic
 *                                        CQI Modes(1-0, 2-0),
 *                  aperiodicSubBandSize- Size of Sub Band in case of Aperiodic
 *                                        CQI Modes(2-0, 3-0)
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : start and end RBG Index with in a subband
 * Returns        : None
 * Description    : This method updates the start and end RBG Index of each 
 *                  sub band with in the specified BandWidth.
 *****************************************************************************/
/* CA Changes start */
STATIC void populateSubBandToRBGMap(
        UInt32 nbResourceBlocks, 
        UInt32 periodicSubBandSize,
        UInt32 aperiodicSubBandSize,
        InternalCellIndex internalCellIndex) 
{
    /**** CQI MODE Periodic (1-0, 2-0)********/ 
    /* and Aperiodic (3-0) - SUBBAND SIZE ****/
    /*****************************************/
    /****System Bandwidth** Subband Size****/
    /*****************************************/
    /* 6 - 7 ** wideband CQI only*/
    /* 8 - 10 ** 4 */
    /* 11 - 26 ** 4 */
    /* 27 - 63 ** 6 */
    /* 64 - 110 ** 8 */
    /*****************************************/ 
    /**** CQI MODE- Aperiodic (2-0)***********/
    /********* SUBBAND SIZE ****/
    /*****************************************/
    /****System Bandwidth** Subband Size****/
    /*****************************************/
    /* 6 - 7 ** NA */
    /* 8 - 10 ** 2 */
    /* 11 - 26 ** 2 */
    /* 27 - 63 ** 3 */
    /* 64 - 110 ** 4 */
    /*****************************************/ 
    UInt8 subBandIndex = 0; 
    UInt8 subBandCount = nbResourceBlocks / periodicSubBandSize ;
    UInt32 rbgSize = (dlRAInfo_gp[internalCellIndex] +
                    dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info.rbgSize;
    UInt32 rbgCount = (dlRAInfo_gp[internalCellIndex] + 
              dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info.rbgCount;
    periodicSubBandSize /= rbgSize; 

   SubBandToRBGMap *subBandToRBGMapPeriodic_p =  cqiGlobalParams_gp\
                               [internalCellIndex]->subBandToRBGMapPeriodic; 
   SubBandToRBGMap *subBandToRBGMapAperiodic_p =  cqiGlobalParams_gp\
                             [internalCellIndex]->subBandToRBGMapAperiodic; 
 
    for(subBandIndex = 0; subBandIndex < subBandCount; subBandIndex++)
    { 
        subBandToRBGMapPeriodic_p[subBandIndex].startRBGIndex = 
            subBandIndex * periodicSubBandSize;
        subBandToRBGMapPeriodic_p[subBandIndex].endRBGIndex = 
            subBandToRBGMapPeriodic_p[subBandIndex].startRBGIndex + 
            periodicSubBandSize - 1;
    }
    if((nbResourceBlocks % (periodicSubBandSize * rbgSize)) != 0)
    {
        subBandToRBGMapPeriodic_p[subBandIndex].startRBGIndex = subBandIndex 
            * periodicSubBandSize;
        subBandToRBGMapPeriodic_p[subBandIndex].endRBGIndex = /*nbResourceBlocks*/
            rbgCount - 1;
    }
    subBandIndex = 0;
    subBandCount = nbResourceBlocks/aperiodicSubBandSize ;
    aperiodicSubBandSize /= rbgSize;
    for(subBandIndex = 0; subBandIndex < subBandCount; subBandIndex++)
    { 
        subBandToRBGMapAperiodic_p[subBandIndex].startRBGIndex = 
            subBandIndex * aperiodicSubBandSize;
        subBandToRBGMapAperiodic_p[subBandIndex].endRBGIndex = 
            subBandToRBGMapAperiodic_p[subBandIndex].startRBGIndex + 
            aperiodicSubBandSize - 1;
    }
    if((nbResourceBlocks % (aperiodicSubBandSize * rbgSize)) != 0)
    {
        subBandToRBGMapAperiodic_p[subBandIndex].startRBGIndex = subBandIndex 
            * aperiodicSubBandSize;
        subBandToRBGMapAperiodic_p[subBandIndex].endRBGIndex = /*nbResourceBlocks*/
            rbgCount - 1;
    }
}
/* CA Changes end */

/*****************************************************************************
 * Function Name  : getMCSFromCQI
 * Inputs         : cqiIndex - channel quality indicator for which MCS is
 *                             required 
 * Outputs        : MCS index 
 * Returns        : MCS index for the given CQI index 
 * Description    : This function returns modulation coding scheme index 
 *                  corresponding to given CQI. 
 *****************************************************************************/
 UInt8 getMCSFromCQI(UInt8 cqiIndex
        )
{
    return cqiMCSArr_g[cqiIndex]; 
} 

/*+EICIC .*/
/*****************************************************************************
 * Function Name  : checkAbsCsiReport
 * Inputs         : cqiPMIConfigIndex_2 
 * Outputs        : true: if sattisfies the criteria of ABS
                         false:if it doesnot sattisfies the criteria
 * Returns        : true or false
 * Description    : This function checks ABS criteria for the given cqiPmiConfiogIndex_2 . 
 *****************************************************************************/
 UInt8 checkAbsCsiReport (/* +- SPR 17777 */
                                UInt8 cqiPeriodicity, UInt16 cqiSubFrameOffset)
{
    UInt32 sysFrameNum = 0;
    UInt32 subFrameNum = 0;

    if (!cqiPeriodicity)
    {
        return LTE_FALSE;
    }

    subFrameNum=perSubframe_g;
    sysFrameNum=perSystemframe_g;

    if(((sysFrameNum * 10) + (subFrameNum ) -cqiSubFrameOffset) !=0)
    {
        if (((sysFrameNum * 10) + (subFrameNum ) -
                    cqiSubFrameOffset) % cqiPeriodicity == 0)
        {
            return LTE_TRUE;
        }
    }

    return LTE_FALSE;

}

/*****************************************************************************
 * Function Name  : checkAbsRiReport
 * Inputs         : cqiPMIConfigIndex_2 
 * Outputs        : true: if sattisfies the criteria of ABS
                         false:if it doesnot sattisfies the criteria
 * Returns        : true or false
 * Description    : This function checks ABS criteria for the given cqiPmiConfiogIndex_2 . 
 *****************************************************************************/
     UInt8
checkAbsRiReport (/* +- SPR 17777 */
    UInt8 cqiPeriodicity,
    UInt16 cqiSubFrameOffset,
    UInt8 riPeriodicity, UInt16 riSubFrameOffset)
{
    /*Cov 69701 + */
    UInt32 sysFrameNum = 0;
    UInt32 subFrameNum = 0;

    if (!riPeriodicity)
    {
        return LTE_FALSE;
    }

    if (!cqiPeriodicity)
    {
        return LTE_FALSE;
    }

    subFrameNum=perSubframe_g;
    sysFrameNum=perSystemframe_g;


    if(((sysFrameNum * 10) + (subFrameNum ) - cqiSubFrameOffset -riSubFrameOffset) !=0)
    {
        if (((sysFrameNum * 10) + (subFrameNum ) - cqiSubFrameOffset -
                    riSubFrameOffset) % (cqiPeriodicity * riPeriodicity) == 0)
        {
            return LTE_TRUE;
        }
    }
    return LTE_FALSE;
    /*Cov 69701 -*/
}

/*-EICIC .*/

/* HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
/*****************************************************************************
 * Function Name  : getHDReducedMCSFromCQI
 * Inputs         : cqiIndex - channel quality indicator for which MCS is
 *                             required 
 * Outputs        : MCS index 
 * Returns        : MCS index for the given CQI index 
 * Description    : This function returns modulation coding scheme index 
 *                  corresponding to given CQI. 
 *****************************************************************************/
 UInt8 getHDReducedMCSFromCQI(UInt8 cqiIndex)
{
    return hdFddReducedMcsTableArr_g[cqiIndex]; 
} 
#endif
/* HD FDD Changes End*/

/*****************************************************************************
 * Function Name  : processFeedBackReportQueue
 * Inputs         : dlSchedulerReportQueueCount - Number of CQI Nodes to be 
 *                                                processed
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion will process the FeedBack Report Queue
 *                  and call relavant funtion depending upon the 
 *                  mode processPMICQIReport.
 *****************************************************************************/
/*CA Changes start  */
void processFeedBackReportQueue(UInt8 dlSchedulerReportQueueCount,
                                 InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    UInt8 scellArrIndex = INVALID_8_VAL;
    LTE_MAC_UT_LOG (LOG_INFO, CQI_MGR, "Entering funtion = %s\n", __func__);
    LTE_MAC_UT_LOG (LOG_INFO, CQI_MGR,
            "Queue Count = %u\n", dlSchedulerReportQueueCount);

    /*+EICIC . */
    ULUEContext *ulUEContext_p = PNULL;
    /* EICIC +*/
    UInt8 numBitsAbsPattern;
    /* EICIC -*/



#ifdef FDD_CONFIG
    numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
    }
    else
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
    }
#endif



    /*-EICIC .*/
    while (dlSchedulerReportQueueCount--)
    {
        /*Pop the Node from Queue*/
        /*CA Changes start  */
        DEQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                (void**)&ueReportedCQINodeInfo_p, internalCellIndex);
        /*CA Changes end  */
        if ( ueReportedCQINodeInfo_p )
        {
            dlUEContextInfo_p = &dlUECtxInfoArr_g
                [ueReportedCQINodeInfo_p->ueIndex]; 
            if (!dlUEContextInfo_p->pendingDeleteFlag)
            {
                dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
                /* CA_phase2_csi_code Start */
                if(dlUEContext_p->internalCellIndex == internalCellIndex)
                {
                    /* Pcell */
                    /* CA_phase2_csi_code end */
                    /* + CQI_5.0 */
                    dlUEContext_p->isSubbandPMIAvailable = FALSE;
                    dlUEContext_p->aperiodicMode12Rcvd = FALSE;
                    /* - CQI_5.0 */

                    /* + TM6_5.2 */
                    if(TX_MODE_6 == dlUEContext_p->dlMIMOInfo.transmissionMode)
                    {
                        dlUEContext_p->tm6DCIFormat = DCI_FORMAT_1B;
                    }
                    /* - TM6_5.2 */
                }
                /* CA_phase2_csi_code Start */
                else
                {
                    /* Scell */
                    scellArrIndex =  getServeCellIndexFromInternalCellId(dlUEContext_p,
                            internalCellIndex);
                    /*klockwork warning fix*/ 
                    if((scellArrIndex <= MAX_NUM_SCELL) && (dlUEContext_p->dlUeScellContext_p[scellArrIndex] != PNULL))
                    {
                        /* + CQI_5.0 */
                        dlUEContext_p->dlUeScellContext_p[scellArrIndex]->isSubbandPMIAvailable = FALSE;
                        dlUEContext_p->dlUeScellContext_p[scellArrIndex]->aperiodicMode12Rcvd = FALSE;
                        /* - CQI_5.0 */

                        /* + TM6_5.2 */
                        if(TX_MODE_6 == dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                                ScelldlMIMOInfo.transmissionMode)
                        {
                            dlUEContext_p->dlUeScellContext_p[scellArrIndex]->Scelltm6DCIFormat =
                                DCI_FORMAT_1B;
                        }
                        /* - TM6_5.2 */
                    }
                    else
                    {
                        continue;
                    }
                }
                /* CA_phase2_csi_code End */
                /*Check the mode and call relevant funtions */


                /* EICIC +*/

                perSubframe_g=ueReportedCQINodeInfo_p->subframe;
                perSystemframe_g= ueReportedCQINodeInfo_p->systemFrame;

                ulUEContext_p =
                    ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
                aperiodicAbs_g =
                    ulUEContext_p->pucchConfigInfo.
                    csiMeasurementSubset1[((ueReportedCQINodeInfo_p->systemFrame* MAX_SUB_FRAME) + ueReportedCQINodeInfo_p->subframe) % numBitsAbsPattern];

                /*- EICIC -*/

                /*Check the mode and call relevant funtions*/
                switch(ueReportedCQINodeInfo_p->mode)
                { 
                    case APERIODIC_CQI_MODE_2_0:
                    {
                        processCQIMode20(
                                &ueReportedCQINodeInfo_p->cqiNode.
                                aperiodicCQIMode20,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case APERIODIC_CQI_MODE_3_0:
                    {
                        processCQIMode30(
                                &ueReportedCQINodeInfo_p->cqiNode.
                                aperiodicCQIMode30,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case APERIODIC_CQI_MODE_3_1:
                    {
                        processCQIMode31(
                                &ueReportedCQINodeInfo_p->cqiNode.
                                aperiodicCQIMode31,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case APERIODIC_CQI_MODE_1_2:
                    {
                        processCQIMode12(
                                &ueReportedCQINodeInfo_p->cqiNode.
                                aperiodicCQIMode12,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case APERIODIC_CQI_MODE_2_2:
                    {
                        processCQIMode22(
                                &ueReportedCQINodeInfo_p->cqiNode.
                                aperiodicCQIMode22,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break; 
                    case PERIODIC_CQI_MODE_1_0_TYPE_3:
                    {
                        processCQIMode10Type3(
                                & ueReportedCQINodeInfo_p->cqiNode.
                                periodicMode10Type3,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case PERIODIC_CQI_MODE_1_0_TYPE_4:
                    {
                        processCQIMode10Type4(
                                &ueReportedCQINodeInfo_p->cqiNode.
                                periodicMode10Type4,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case PERIODIC_CQI_MODE_1_1_TYPE_2:
                    {
                        processCQIMode11Type2(
                                &ueReportedCQINodeInfo_p->cqiNode.
                                periodicMode11Type2,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case PERIODIC_CQI_MODE_1_1_TYPE_3:
                    {
                        processCQIMode11Type3(
                                &ueReportedCQINodeInfo_p->cqiNode.
                                periodicMode11Type3,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case PERIODIC_CQI_MODE_2_0_TYPE_1:
                    {
                        processCQIMode20Type1(
                                & ueReportedCQINodeInfo_p->cqiNode.
                                periodicMode20Type1,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case PERIODIC_CQI_MODE_2_0_TYPE_3:
                    {
                        processCQIMode20Type3(
                                &ueReportedCQINodeInfo_p->cqiNode.
                                periodicMode20Type3,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case PERIODIC_CQI_MODE_2_0_TYPE_4:
                    {
                        processCQIMode20Type4(
                                &ueReportedCQINodeInfo_p->cqiNode.
                                periodicMode20Type4,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case PERIODIC_CQI_MODE_2_1_TYPE_1:
                    {
                        processCQIMode21Type1(
                                &ueReportedCQINodeInfo_p->cqiNode.
                                periodicMode21Type1,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case PERIODIC_CQI_MODE_2_1_TYPE_2:
                    {
                        processCQIMode21Type2(
                                &ueReportedCQINodeInfo_p->cqiNode.
                                periodicMode21Type2,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    case PERIODIC_CQI_MODE_2_1_TYPE_3:
                    {
                        processCQIMode21Type3(
                                & ueReportedCQINodeInfo_p->cqiNode.
                                periodicMode21Type3,
                                dlUEContext_p,
                                internalCellIndex);
                    }
                    break;
                    default:
                    {
                        LTE_MAC_UT_LOG(LOG_INFO,CQI_MGR,
                                "InValid Mode \n");
                    }
                }
                aperiodicAbs_g = 0;
                
                if(( SPS_IDLE != dlUEContext_p->spsDlInfo.spsState   )&&
                        (INVALID_STATE != dlUEContext_p->spsDlInfo.spsState))               
                {
                    LP_SpsDLIntervalProfile   dlSpsIntervalProfile_p = PNULL;
                    UInt16 dlSpsInterval          = 0;
                    UInt8  mcsIndex = 0;
                    dlSpsInterval = dlUEContext_p->spsDlInfo.spsDlSetupInfo_p->\
                                    semiPersistentSchedIntervalDL;
                    dlSpsIntervalProfile_p = &(cellSpecificParams_g.
                            cellConfigAndInitParams_p[internalCellIndex]->initParams_p\
                            ->dlSchdConfig.spsDlIntervalProfile[dlSpsInterval]) ;
                    mcsIndex =  dlUEContext_p->dlCQIInfo.mcsIndexCodeWordOne;
                    if(mcsIndex < (dlSpsIntervalProfile_p->dlMinMcsSps + dlSpsIntervalProfile_p->dlMcsMargin))
                    { 
                        dlUEContext_p->spsDlInfo.isUeDlWbMcsBelowThresh = LTE_TRUE;                
                    }
                }
                {
                    scellStateMngrScheduleMacCEForDeactivationByMCSThres( 
                            dlUEContext_p, internalCellIndex );
                }
            }
            freeMemPool(ueReportedCQINodeInfo_p);
            ueReportedCQINodeInfo_p = PNULL;
            
            continue;
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,CQI_MGR,
            "Leaving funtion = %s\n",__func__);
}

/*****************************************************************************
 * Function Name  : isPMIInCodebookSubsetRestriction
 * Inputs         : pmiUE - This holds pointer to
 *                  RI - Rank indicator reported from UE.
 *                  ueCategory - Ue Category. 
 *                  transMode - Transmission Mode of UE.
 *                  cbsrValue - Pointer to CBSR Value.
 *                  numOfTxAnteenas -  num antennas configured
 * Outputs        : None
 * Returns        : Zero if PMI is wrong else non zero value
 * Description    : This function validate the recived PMI with configured
 *                  CBSR.
 *****************************************************************************/
/* +- SPR 17777 */
static  UInt8 isPMIInCodebookSubsetRestriction(
                                                      UInt8 *cbsrValue
#ifdef CSI_VALIDATION
                                                      ,UInt8 pmiUE,
                                                      UInt8 RI,
                                                      UInt8 ueCategory,
                                                      UInt8 transMode,
                                                      UInt8 numOfTxAnteenas 
#endif
                                                      )
/* +- SPR 17777 */
{
    UInt8 result  = 0;

    /** Coverity 67180_67179_67178_67177 Fix : END **/
    /*cov-id 69709 fix*/
    if (cbsrValue == PNULL)
    {
        return result;
    }    
    /** Coverity 67180_67179_67178_67177 Fix : END **/
#ifdef CSI_VALIDATION
    /* For 4x4 and 4x2 MIMO */
    if(numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        if (TX_MODE_4 == transMode)
        {
            /* for number of layer 4 */
            /* Check for Ue category not required as the value of RI=4 will
             * be reported for UE supporting 4 layer transmission */
            if (RANK_INDICATOR_4 == RI)
            {
               if(pmiUE > PMI_VALUE_7)
                   result = ((0x01 << (pmiUE -
                             NUMBER_OF_BITS_IN_1BYTE))&cbsrValue[CBSR_IDX_0]);
               else
                   result = ((0x01 << (pmiUE))&cbsrValue[CBSR_IDX_1]);
            }
            else if  (RANK_INDICATOR_3 == RI)
            {
               if(pmiUE > PMI_VALUE_7)
                   result = ((0x01 << (pmiUE -
                             NUMBER_OF_BITS_IN_1BYTE))&cbsrValue[CBSR_IDX_2]);
               else
                   result = ((0x01 << (pmiUE))&cbsrValue[CBSR_IDX_3]);
            }
            else if (RANK_INDICATOR_2 == RI) /* 2x2 MIMO*/
            {
               if(pmiUE > PMI_VALUE_7)
                   result = ((0x01 << (pmiUE -
                             NUMBER_OF_BITS_IN_1BYTE))&cbsrValue[CBSR_IDX_4]);
               else
                   result = ((0x01 << (pmiUE))&cbsrValue[CBSR_IDX_5]);
            }
            else /* For single layer reported RI */
            {
               if(pmiUE > PMI_VALUE_7)
                   result = ((0x01 << (pmiUE -
                             NUMBER_OF_BITS_IN_1BYTE))&cbsrValue[CBSR_IDX_6]);
               else
                   result = ((0x01 << (pmiUE))&cbsrValue[CBSR_IDX_7]);
            }
        }
        else if (TX_MODE_6 == transMode)
        {
            if(pmiUE > PMI_VALUE_7)
                result = ((0x01 << (pmiUE -
                           NUMBER_OF_BITS_IN_1BYTE))&cbsrValue[CBSR_IDX_0]);
            else
                result = ((0x01 << (pmiUE))&cbsrValue[CBSR_IDX_1]);
        }
        else if(TX_MODE_8 == transMode)/* transmission mode 8 */
        {
            if (RANK_INDICATOR_2 == RI) /* 2x2 MIMO*/
            {
                if(pmiUE > PMI_VALUE_7)
                    result = ((0x01 << (pmiUE -
                               NUMBER_OF_BITS_IN_1BYTE))&cbsrValue[CBSR_IDX_0]);
                else
                    result = ((0x01 << (pmiUE))&cbsrValue[CBSR_IDX_1]);
            }
            else /* For single layer reported RI */
            {
                if(pmiUE > PMI_VALUE_7)
                    result = ((0x01 << (pmiUE -
                               NUMBER_OF_BITS_IN_1BYTE))&cbsrValue[CBSR_IDX_2]);
                else
                    result = ((0x01 << (pmiUE))&cbsrValue[CBSR_IDX_3]);
            }
        }
        else
        {
            /* wrong trans mode recieved*/
            LOG_MAC_MSG(MAC_RRC_TRANS_MODE_ID,LOGERROR,MAC_L1_INF,\
                    getCurrentTick(),ueCategory,transMode,\
                    RI,pmiUE,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"Wrong transmission mode Received");
        }
    }
    /* for 2x2 MIMO */
    else if (numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
    {
        /* transmission mode 4 or 8 */
        if ((TX_MODE_4 == transMode) || (TX_MODE_8 == transMode))
        {
            if (RANK_INDICATOR_2 == RI)
            {   /*Read bit 7 or 6 in cbsr*/
                result = ((0x01 << (SHIFT_BY_5_BIT+pmiUE))&cbsrValue[CBSR_IDX_0]);
            }
            else
            {   /*Read bit 5, 4, 3 or 2 in cbsrValue*/
                result = ((0x01 << (SHIFT_BY_2_BIT+pmiUE))&cbsrValue[CBSR_IDX_0]);
            }
        }
        else if (TX_MODE_6 == transMode)  /* transmission mode 6 */
        {
            result = ((0x01 << (SHIFT_BY_4_BIT+pmiUE))&cbsrValue[CBSR_IDX_0]);
        }
        else
        {
            /* wrong trans mode recieved*/
            LOG_MAC_MSG(MAC_RRC_TRANS_MODE_ID,LOGERROR,MAC_L1_INF,\
                    getCurrentTick(),ueCategory,transMode,\
                    RI,pmiUE,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"Wrong transmission mode Received");
        }
    }
    else
    {
        /* errorlog for wrong antenna configuration */
        LOG_MAC_MSG(MAC_RRC_NUM_ANT_ID,LOGERROR,MAC_L1_INF,\
                getCurrentTick(),ueCategory,transMode,\
                RI,numOfTxAnteenas,pmiUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"Wrong Antenna Configuration Received");
    }
    LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"isPMIInCodebookSubsetRestriction returned: %d\n",result);
#else
    result = 1;
#endif
    return result;
}
/* 4x4 DL MIMO CHG END */
/*****************************************************************************
 * Function Name  : processCQIMode20
 * Inputs         : aperiodicCQIMode20_p - This holds pointer to 
 *                                         AperiodicCQIMode20 structure.
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure.
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the CQIInfoNonSpatialMul structure
 *                  for CodeWord One. 
 *****************************************************************************/
/* CA Changes start */
STATIC void processCQIMode20(
        AperiodicCQIMode20 *aperiodicCQIMode20_p,
        DLUEContext *dlUEContext_p,
        InternalCellIndex internalCellIndex)
/* CA Changes end */
{
    DLCQIInfo *dlCQIInfo_p = PNULL;
    SubBandReportInfo *subBandReportInfo_p = PNULL;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    /* + CQI_4.1 */  
    UInt8  *subbandInfo20_p = PNULL;
    UInt32 oldCqiForRBG = 0;
    UInt32 xorBitMap = 0;
    UInt32 previousReportedSubbandBitMap = 0;
    UInt32 newReportedSubbandBitMap = 0;
    UInt32 position = 0;
    UInt16 reportedCQIBitMap = 0;
    UInt8 reportedCQI = 0;/* This will hold the reported CQI corresponding 
                           * to the position comes after Xor subband maps*/
    UInt8 subbandNum = 0; /* This will hold the subband Number */
    UInt8 subsetIndex = 0; /* This will hold the subset number */
    UInt8 correctedSubbandCQI = 0;
    UInt8 correctedSubbandMCS = 0;
    UInt16 tempReportedCQIBitmapPerSubset[MAX_SUBSETS] = {0};
    /* - CQI_4.1 */  
    UInt32 subBandCount = 0;
    CQIInfo *cqiInfo_p = PNULL;
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;
    UInt32 startRBGIndex = 0;
    UInt32 endRBGIndex = 0;
    UInt32 rbgIndexCount = 0;
    UInt8 servCellIndex = 1;
    /* CA Changes start */
  ULUEContext *ulUEContext_p = PNULL;
  ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    SubBandToRBGMap *subBandToRBGMapAperiodic_p =  cqiGlobalParams_gp\
                                                   [internalCellIndex]->subBandToRBGMapAperiodic; 
    rat0Info_p = &(dlRAInfo_gp[internalCellIndex] + 
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info;
    rat1Info_p = &(dlRAInfo_gp[internalCellIndex] + 
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat1Info;

    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;

    }
    /* CA Changes end */
    /*+EICIC . */
    CHECK_APERIODIC_SECONDARY_CONDITION (ulUEContext_p, internalCellIndex)
    {    /* ABS Criteria */
        dlCQIInfo_p->isWideBandReportAvailable_2 = 1;
        /* SPR 8175 changes start */
        dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
        /* SPR 8175 changes end */

        /* SPR 4769 changes start */
        /* Time Averaging */
        /* SPR 9427 changes start */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0_2 = calculateAverageDLCQIforAP (
                /* SPR 9427 changes end */
                aperiodicCQIMode20_p->
                widebandCQIValue,
                dlCQIInfo_p->
                previousTimeAveragedWBCQICW0_2);

        /* Correcting wideband CQI and MCS from BLER  */
        dlCQIInfo_p->widebandCQICodeWordOne_2 =
            getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW0_2,
                    dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                    dlUEContext_p);
        dlCQIInfo_p->mcsIndexCodeWordOne_2 =
            getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordOne_2
                          );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start */
        getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordOne_2,
                /*  CA_phase2_csi_code */
                &(dlCQIInfo_p->modulationSchemeCodeWordOne_2));
        /*SPR 6424 Fix End */

        /* ICIC chnages start */
        /* CA Changes start */
        /* CA_phase2_csi_code Start */
        if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
        {
            updateDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex, servCellIndex);
        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */
        /* ICIC changes end */


        LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                getCurrentTick (), dlUEContext_p->ueIndex,
                dlCQIInfo_p->widebandCQICodeWordOne_2,
                dlCQIInfo_p->mcsIndexCodeWordOne_2, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                __func__, "WB_CW0_ABS");

        /* + CQI_5.0 */
        /* CA Chanages start */
        if (FREQUENCY_SELECTIVE_ON ==
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
                cellParams_p->freqSelectiveSchDL)
            /* CA Chanages end */
        {
            /*Get the number of SubBand Count for which the report has came */
            subBandCount = aperiodicCQIMode20_p->numberOfSubbands;

            dlCQIInfo_p->isSubBandReportAvailable_2 = 1;
            subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;

            cqiInfo_p = &subBandReportInfo_p->cqiInfo;

            subbandInfo20_p = &aperiodicCQIMode20_p->subbandInfo20[0];

            previousReportedSubbandBitMap =
                cqiInfo_p->cqiInfoNonSpatialMul.rat0NonSpatialMulCQIInfo.
                reportedSubbandBitMap_2;


            while (subBandCount--)
            {
                subbandNum = subbandInfo20_p[subBandCount];

                /* CA Changes start */
                startRBGIndex =
                    subBandToRBGMapAperiodic_p[subbandNum].startRBGIndex;

                endRBGIndex = subBandToRBGMapAperiodic_p[subbandNum].endRBGIndex;
                /* CA Changes end */
                rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                subBandToCQInfo_p =
                    &subBandReportInfo_p->subBandToCQInfo[startRBGIndex];

                oldCqiForRBG = subBandToCQInfo_p->cqiIndexCW1_2;
                /* SPR 4769 changes start */
                /* Time Averaging */
                /* SPR 9427 changes start */
                subBandToCQInfo_p->previousTimeAveragedSBCQICW0_2 =
                    calculateAverageDLCQIforAP (
                            /* SPR 9427 changes end */
                            aperiodicCQIMode20_p->
                            subbandCQIValue,
                            subBandToCQInfo_p->
                            previousTimeAveragedSBCQICW0_2);

                /* Calculating effective Subband CQI from BLER */
                subBandToCQInfo_p->cqiIndexCW1_2 =
                    getEffectiveDLCQI (subBandToCQInfo_p->
                            previousTimeAveragedSBCQICW0_2,
                            dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                            dlUEContext_p);

                /* Correcting Subband CQI and MCS */
                correctedSubbandCQI = subBandToCQInfo_p->cqiIndexCW1_2;
                correctedSubbandMCS =
                    getMCSFromCQI (subBandToCQInfo_p->cqiIndexCW1_2
                                 );
                /* SPR 4769 changes end */
                /*SPR 6424 Fix Start */
                getModSchemeFromMcsIndex (correctedSubbandMCS,
                        /* CA_phase2_csi_code */
                        &(dlCQIInfo_p->
                            modulationSchemeCodeWordOne_2));
                /*SPR 6424 Fix End */

                /*Update the New MCS Index Information */
                subBandToCQInfo_p->mcsIndexCW1_2 = correctedSubbandMCS;

                while (rbgIndexCount--)
                {
                    /* SPR 3803 changes start */
                    if (oldCqiForRBG)
                    {
                        /* SPR 3803 changes end */
                        resetRAT0CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQI,
                                oldCqiForRBG,
                                &cqiInfo_p->
                                cqiInfoNonSpatialMul);

                        resetRAT1CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQI,
                                oldCqiForRBG,
                                &cqiInfo_p->
                                cqiInfoNonSpatialMul,
                                rat0Info_p,
                                rat1Info_p);
                        /* SPR 3803 changes start */
                    }
                    /* SPR 3803 changes end */

                    updateRAT0CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                            correctedSubbandCQI,
                            oldCqiForRBG,
                            &cqiInfo_p->
                            cqiInfoNonSpatialMul);

                    updateRAT1CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                            correctedSubbandCQI,
                            oldCqiForRBG,
                            &cqiInfo_p->
                            cqiInfoNonSpatialMul,
                            rat0Info_p, rat1Info_p,
                            tempReportedCQIBitmapPerSubset);
                    startRBGIndex++;
                }

                /* Updating the reported Subbands */
                newReportedSubbandBitMap |= (UInt32) 1 << subbandNum;

                /* Updating the reported CQI Index */

                /* We are firstly updating CQI in local variable because it may happen that 
                 * with in a report new CQI of one RBG is same as old CQI of other RBG which
                 * may  cause new CQI to be reset in reset functions.By taking it in local 
                 * variable,information will never reset for the particular report.
                 */
                SET_CQI_REPORT_BIT (reportedCQIBitMap, correctedSubbandCQI);
            }

            /* Xoring previous and newly reported subband bit map */
            xorBitMap = previousReportedSubbandBitMap ^ newReportedSubbandBitMap;

            while (xorBitMap)
            {
                position = FFS (xorBitMap);
                position -= 1;

                reportedCQI =
                    subBandReportInfo_p->subBandToCQInfo[position].cqiIndexCW1_2;
                /* setting bit corresponding to CQI reported for RAT0 */
                SET_CQI_REPORT_BIT (reportedCQIBitMap, reportedCQI);

                /* Setting reported Cqi Bitmap for RAT1 */
                /* calculating start and end rbg Index of subband corresponding to 
                 * position in the xorBitMap */

                startRBGIndex =
                    subBandToRBGMapAperiodic_p[position].startRBGIndex;
                endRBGIndex = subBandToRBGMapAperiodic_p[position].endRBGIndex;
                rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                while (rbgIndexCount--)
                {
                    subsetIndex =
                        rat0Info_p->rbgIndexInfo[startRBGIndex].subsetIndex;

                    SET_CQI_REPORT_BIT (cqiInfo_p->cqiInfoNonSpatialMul.
                            rat1NonSpatialMulCQIInfo[subsetIndex].
                            reportedCQIBitMap_2, reportedCQI);

                    startRBGIndex++;
                }

                /* setting the reported position in the xor bitmap */
                xorBitMap &= (~((UInt32) 1 << position));
            }

            cqiInfo_p->cqiInfoNonSpatialMul.rat0NonSpatialMulCQIInfo.
                reportedCQIBitMap_2 |= reportedCQIBitMap;
            cqiInfo_p->cqiInfoNonSpatialMul.
                rat0NonSpatialMulCQIInfo.reportedSubbandBitMap_2 |=
                newReportedSubbandBitMap;
        }
        /* - CQI_5.0 */
        /* - CQI_4.1 */
        ulUEContext_p->isDLWideBandCQIReportAvailable_2 = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne_2 =
            aperiodicCQIMode20_p->widebandCQIValue;
        ulUEContext_p->dlWidebandCQICodeWordTwo_2 = 0;

    }
    else
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne =
            aperiodicCQIMode20_p->widebandCQIValue;
        ulUEContext_p->dlWidebandCQICodeWordTwo = 0;
        /*-EICIC .*/
        dlCQIInfo_p->isWideBandReportAvailable = 1;
        /* SPR 8175 changes start */
        dlCQIInfo_p->riUpdationFlag = IMMEDIATE_DL_RI_UPDATE;
        /* SPR 8175 changes end */

        /* SPR 4769 changes start */
        /* Time Averaging */
        /* SPR 9427 changes start */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0 = calculateAverageDLCQIforAP(
                /* SPR 9427 changes end */
                aperiodicCQIMode20_p->widebandCQIValue,
                dlCQIInfo_p->previousTimeAveragedWBCQICW0);

        /* Correcting wideband CQI and MCS from BLER  */
        dlCQIInfo_p->widebandCQICodeWordOne = getEffectiveDLCQI(
                dlCQIInfo_p->previousTimeAveragedWBCQICW0,
                dlCQIInfo_p->cqiCorrectionFactorForCW0, 
                dlUEContext_p );
        dlCQIInfo_p->mcsIndexCodeWordOne =
            getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordOne
                         );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start*/

        /* Cyclomatic_complexity_changes_start */
        getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordOne,
                /*  CA_phase2_csi_code */
                &(dlCQIInfo_p->modulationSchemeCodeWordOne));
        /* Cyclomatic_complexity_changes_end */

        /*SPR 6424 Fix End*/

        /* ICIC chnages start */
        /* CA Changes start */
        /* CA_phase2_csi_code Start */
        if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
        {
            updateDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                    internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                    /* +- SPR 17777 */
                    servCellIndex);
        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */
        /* ICIC changes end */
        LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                getCurrentTick(),dlUEContext_p->ueIndex,
                dlCQIInfo_p->widebandCQICodeWordOne,
                dlCQIInfo_p->mcsIndexCodeWordOne,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"WB_CW0");

        /* + CQI_5.0 */
        /* CA Chanages start */
        if (FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p\
                [internalCellIndex]->cellParams_p->freqSelectiveSchDL)
            /* CA Chanages end */
        {
            /*Get the number of SubBand Count for which the report has came*/
            subBandCount = aperiodicCQIMode20_p->numberOfSubbands;

            dlCQIInfo_p->isSubBandReportAvailable = 1;
            subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;

            cqiInfo_p = &subBandReportInfo_p->cqiInfo;

            subbandInfo20_p = &aperiodicCQIMode20_p->subbandInfo20[0]; 

            previousReportedSubbandBitMap = cqiInfo_p->cqiInfoNonSpatialMul.\
                                            rat0NonSpatialMulCQIInfo.reportedSubbandBitMap;


            while (subBandCount--)
            {
                subbandNum = subbandInfo20_p[subBandCount];

                /* CA Changes start */
                startRBGIndex = subBandToRBGMapAperiodic_p[subbandNum].startRBGIndex;

                endRBGIndex = subBandToRBGMapAperiodic_p[subbandNum].endRBGIndex;
                /* CA Changes end */
                rbgIndexCount = endRBGIndex - startRBGIndex + 1; 

                subBandToCQInfo_p = &subBandReportInfo_p->
                    subBandToCQInfo[startRBGIndex];

                oldCqiForRBG = subBandToCQInfo_p->cqiIndexCW1;
                /* SPR 4769 changes start */
                /* Time Averaging */
                /* SPR 9427 changes start */
                subBandToCQInfo_p->previousTimeAveragedSBCQICW0 = calculateAverageDLCQIforAP(
                        /* SPR 9427 changes end */
                        aperiodicCQIMode20_p->subbandCQIValue,
                        subBandToCQInfo_p->previousTimeAveragedSBCQICW0);

                /* Calculating effective Subband CQI from BLER */
                subBandToCQInfo_p->cqiIndexCW1 = getEffectiveDLCQI(
                        subBandToCQInfo_p->previousTimeAveragedSBCQICW0,
                        dlCQIInfo_p->cqiCorrectionFactorForCW0,
                        dlUEContext_p );

                /* Correcting Subband CQI and MCS*/
                correctedSubbandCQI = subBandToCQInfo_p->cqiIndexCW1;
            correctedSubbandMCS = getMCSFromCQI(subBandToCQInfo_p->cqiIndexCW1
                                                );
                /* SPR 4769 changes end */
                /*SPR 6424 Fix Start*/

                /* Cyclomatic_complexity_changes_start */
                getModSchemeFromMcsIndex(correctedSubbandMCS,
                        /* CA_phase2_csi_code */
                        &(dlCQIInfo_p->modulationSchemeCodeWordOne));
                /* Cyclomatic_complexity_changes_end */

                /*SPR 6424 Fix End*/

                /*Update the New MCS Index Information*/
                subBandToCQInfo_p->mcsIndexCW1 = correctedSubbandMCS;

                while(rbgIndexCount--)
                {
                    /* SPR 3803 changes start */  
                    if (oldCqiForRBG)
                    {    
                        /* SPR 3803 changes end */  
                        resetRAT0CW1NonSpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQI,
                                oldCqiForRBG,
                                &cqiInfo_p->cqiInfoNonSpatialMul);

                        resetRAT1CW1NonSpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQI,
                                oldCqiForRBG,
                                &cqiInfo_p->cqiInfoNonSpatialMul,
                                rat0Info_p,
                                rat1Info_p); 
                        /* SPR 3803 changes start */  
                    }
                    /* SPR 3803 changes end */  

                    updateRAT0CW1NonSpatialMulSubBandInfo(
                            startRBGIndex,
                            correctedSubbandCQI,
                            oldCqiForRBG,
                            &cqiInfo_p->cqiInfoNonSpatialMul);

                    updateRAT1CW1NonSpatialMulSubBandInfo(
                            startRBGIndex,
                            correctedSubbandCQI,
                            oldCqiForRBG,
                            &cqiInfo_p->cqiInfoNonSpatialMul,
                            rat0Info_p,
                            rat1Info_p,
                            tempReportedCQIBitmapPerSubset);
                    startRBGIndex++;
                } 

                /* Updating the reported Subbands */
                newReportedSubbandBitMap |= (UInt32)1 << subbandNum;

                /* Updating the reported CQI Index */

                /* We are firstly updating CQI in local variable because it may happen that 
                 * with in a report new CQI of one RBG is same as old CQI of other RBG which
                 * may  cause new CQI to be reset in reset functions.By taking it in local 
                 * variable,information will never reset for the particular report.
                 */
                SET_CQI_REPORT_BIT(reportedCQIBitMap,correctedSubbandCQI);      
            }

            /* Xoring previous and newly reported subband bit map*/
            xorBitMap = previousReportedSubbandBitMap ^ newReportedSubbandBitMap;

            while( xorBitMap )
            {
                position = FFS(xorBitMap);
                position -= 1;

                reportedCQI = subBandReportInfo_p->subBandToCQInfo[position].
                    cqiIndexCW1;
                /* setting bit corresponding to CQI reported for RAT0*/
                SET_CQI_REPORT_BIT(reportedCQIBitMap,reportedCQI);

                /* Setting reported Cqi Bitmap for RAT1 */
                /* calculating start and end rbg Index of subband corresponding to 
                 * position in the xorBitMap */

                startRBGIndex = subBandToRBGMapAperiodic_p[position].startRBGIndex;
                endRBGIndex = subBandToRBGMapAperiodic_p[position].endRBGIndex;
                rbgIndexCount = endRBGIndex - startRBGIndex + 1; 

                while(rbgIndexCount--)
                {
                    subsetIndex = rat0Info_p->rbgIndexInfo[startRBGIndex].subsetIndex; 

                    SET_CQI_REPORT_BIT(cqiInfo_p->cqiInfoNonSpatialMul.
                            rat1NonSpatialMulCQIInfo[subsetIndex].reportedCQIBitMap,
                            reportedCQI);

                    startRBGIndex++;    
                }  

                /* setting the reported position in the xor bitmap */
                xorBitMap &=(~((UInt32)1 << position));
            }    

            cqiInfo_p->cqiInfoNonSpatialMul.rat0NonSpatialMulCQIInfo.reportedCQIBitMap 
                |=  reportedCQIBitMap;
            cqiInfo_p->cqiInfoNonSpatialMul.rat0NonSpatialMulCQIInfo.
                reportedSubbandBitMap |= newReportedSubbandBitMap;
        }
        /* - CQI_5.0 */
        /* - CQI_4.1 */ 
    }
    return;
}

/*****************************************************************************
 * Function Name  : processCQIMode30
 * Inputs         : periodicCQIMode30_p - This holds pointer to 
 *                                        AperiodicCQIMode30 structure,
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the CQIInfoNonSpatialMul structure
 *                  for CodeWord One. 
 *****************************************************************************/
/* CA Changes start */
STATIC void processCQIMode30(
        AperiodicCQIMode30 *aperiodicCQIMode30_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex)
/* CA Changes end */
{
    DLCQIInfo *dlCQIInfo_p = PNULL;
    SubBandReportInfo *subBandReportInfo_p = PNULL;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    UInt8 *subbandInfo30_p = PNULL;
    UInt32 subBandCount = 0;
    CQIInfo *cqiInfo_p = PNULL;
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;
    UInt32 startRBGIndex = 0;
    UInt32 endRBGIndex = 0;
    UInt32 rbgIndexCount = 0;
    /* + CQI_4.1 */  
    UInt32 reportedCQIBitMap = 0;
    UInt32 oldCqiForRBG = 0;
    UInt8 correctedSubbandCQI = 0;
    UInt8 correctedSubbandMCS = 0;
    UInt16 tempReportedCQIBitmapPerSubset[MAX_SUBSETS] = {0};
    UInt8 servCellIndex = 1;
    /* - CQI_4.1 */  
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    /* CA Changes start */
    SubBandToRBGMap *subBandToRBGMapPeriodic_p =  cqiGlobalParams_gp\
                                                  [internalCellIndex]->subBandToRBGMapPeriodic; 
    rat0Info_p = &(dlRAInfo_gp[internalCellIndex] + \
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info;
    rat1Info_p = &(dlRAInfo_gp[internalCellIndex] + \
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat1Info;

    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;

    }
    /* CA Changes end */
    /*+EICIC . */
    CHECK_APERIODIC_SECONDARY_CONDITION (ulUEContext_p, internalCellIndex)
    {    /* ABS Criteria */
        dlCQIInfo_p->isWideBandReportAvailable_2 = 1;

        /* SPR 8175 changes start */
        dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
        /* SPR 8175 changes end */
        /* SPR 4769 changes start */
        /* Time Averaging */
        /* SPR 9427 changes start */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0_2 = calculateAverageDLCQIforAP (
                /* SPR 9427 changes end */
                aperiodicCQIMode30_p->
                widebandCQIValue,
                dlCQIInfo_p->
                previousTimeAveragedWBCQICW0_2);

        /* Correcting wideband CQI and MCS from BLER */
        dlCQIInfo_p->widebandCQICodeWordOne_2 =
            getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW0_2,
                    dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                    dlUEContext_p);

    dlCQIInfo_p->mcsIndexCodeWordOne_2 =
        getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordOne_2
                      );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start */
        getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordOne_2,
                /*CA_phase2_csi_code */
                &(dlCQIInfo_p->modulationSchemeCodeWordOne_2));
        /*SPR 6424 Fix End */

        /* ICIC chnages start */
        /* CA Changes start */
        /* CA_phase2_csi_code Start */
        if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
        {
            updateDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex, servCellIndex);
        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */
        /* ICIC changes end */

        LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                getCurrentTick (), dlUEContext_p->ueIndex,
                dlCQIInfo_p->widebandCQICodeWordOne_2,
                dlCQIInfo_p->mcsIndexCodeWordOne_2, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                __func__, "WB_CW0_ABS");

        /* + CQI_5.0 */
        /* CA Changes start */
        if (FREQUENCY_SELECTIVE_ON ==
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
                cellParams_p->freqSelectiveSchDL)
            /* CA Changes end */
        {
            /*Get the number of SubBand Count for which the report has came */
            subBandCount = aperiodicCQIMode30_p->numberOfSubbands;

            dlCQIInfo_p->isSubBandReportAvailable_2 = 1;
            subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;

            cqiInfo_p = &subBandReportInfo_p->cqiInfo;

            subbandInfo30_p = &aperiodicCQIMode30_p->subbandCQIInfo30[0];

            while (subBandCount--)
            {
                /*  subBandCount value denotes the subband number */
                /* CA Changes start */
                startRBGIndex =
                    subBandToRBGMapPeriodic_p[subBandCount].startRBGIndex;
                endRBGIndex = subBandToRBGMapPeriodic_p[subBandCount].endRBGIndex;
                /* CA Changes end */
                rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                while (rbgIndexCount--)
                {
                    subBandToCQInfo_p =
                        &subBandReportInfo_p->subBandToCQInfo[startRBGIndex];
                    oldCqiForRBG = subBandToCQInfo_p->cqiIndexCW1_2;

                    /* SPR 4769 changes start */
                    /* Time Averaging */
                    subBandToCQInfo_p->previousTimeAveragedSBCQICW0_2 =
                        /* SPR 9427 changes start */
                        calculateAverageDLCQIforAP (subbandInfo30_p[subBandCount],
                                /* SPR 9427 changes end */
                                subBandToCQInfo_p->
                                previousTimeAveragedSBCQICW0_2);

                    /* Correcting Subband CQI and MCS from BLER */
                    subBandToCQInfo_p->cqiIndexCW1_2 =
                        getEffectiveDLCQI (subBandToCQInfo_p->
                                previousTimeAveragedSBCQICW0_2,
                                dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                                dlUEContext_p);
                correctedSubbandCQI = subBandToCQInfo_p->cqiIndexCW1_2;
                correctedSubbandMCS = getMCSFromCQI(correctedSubbandCQI
                                                   ); 
                /*SPR 6424 Fix Start*/

                /* Cyclomatic_complexity_changes_start */
                getModSchemeFromMcsIndex(correctedSubbandMCS,
                            /* CA_phase2_csi_code */
                            &(dlCQIInfo_p->
                                modulationSchemeCodeWordOne_2));
                    /*SPR 6424 Fix End */
                    subBandToCQInfo_p->mcsIndexCW1_2 = correctedSubbandMCS;
                    /* SPR 4769 changes end */

                    /* + CQI_5.0 */
                    subBandToCQInfo_p->initialReportedCQICW1_2 =
                        subbandInfo30_p[subBandCount];
                    /* - CQI_5.0 */

                    /* SPR 3803 changes start */
                    if (oldCqiForRBG)
                    {
                        /* SPR 3803 changes end */
                        resetRAT0CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQI,
                                oldCqiForRBG,
                                &cqiInfo_p->
                                cqiInfoNonSpatialMul);

                        resetRAT1CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQI,
                                oldCqiForRBG,
                                &cqiInfo_p->
                                cqiInfoNonSpatialMul,
                                rat0Info_p,
                                rat1Info_p);
                        /* SPR 3803 changes start */
                    }
                    /* SPR 3803 changes end */

                    updateRAT0CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                            correctedSubbandCQI,
                            oldCqiForRBG,
                            &cqiInfo_p->
                            cqiInfoNonSpatialMul);

                    updateRAT1CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                            correctedSubbandCQI,
                            oldCqiForRBG,
                            &cqiInfo_p->
                            cqiInfoNonSpatialMul,
                            rat0Info_p,
                            rat1Info_p,
                            tempReportedCQIBitmapPerSubset);

                    /* Updating the reported CQI Index */

                    SET_CQI_REPORT_BIT (reportedCQIBitMap, correctedSubbandCQI);
                    startRBGIndex++;
                }

            }

            cqiInfo_p->cqiInfoNonSpatialMul.rat0NonSpatialMulCQIInfo.
                reportedCQIBitMap_2 = reportedCQIBitMap;
        }
        /* - CQI_5.0 */
        /* - CQI_4.1 */
        ulUEContext_p->isDLWideBandCQIReportAvailable_2 = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne_2 =
            aperiodicCQIMode30_p->widebandCQIValue;
        ulUEContext_p->dlWidebandCQICodeWordTwo_2 = 0;

    }
    else
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne =
            aperiodicCQIMode30_p->widebandCQIValue;
        ulUEContext_p->dlWidebandCQICodeWordTwo = 0;
        /*-EICIC .*/
        dlCQIInfo_p->isWideBandReportAvailable = 1;

        /* SPR 8175 changes start */
        dlCQIInfo_p->riUpdationFlag = IMMEDIATE_DL_RI_UPDATE;
        /* SPR 8175 changes end */
        /* SPR 4769 changes start */
        /* Time Averaging */
        /* SPR 9427 changes start */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0 = calculateAverageDLCQIforAP(
                /* SPR 9427 changes end */
                aperiodicCQIMode30_p->widebandCQIValue,
                dlCQIInfo_p->previousTimeAveragedWBCQICW0);

        /* Correcting wideband CQI and MCS from BLER */
        dlCQIInfo_p->widebandCQICodeWordOne =  getEffectiveDLCQI (
                dlCQIInfo_p->previousTimeAveragedWBCQICW0,
                dlCQIInfo_p->cqiCorrectionFactorForCW0,
                dlUEContext_p );

        dlCQIInfo_p->mcsIndexCodeWordOne =
            getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordOne
                         );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start*/

        /* Cyclomatic_complexity_changes_start */
        getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordOne,
                /*CA_phase2_csi_code */
                &(dlCQIInfo_p->modulationSchemeCodeWordOne));
        /* Cyclomatic_complexity_changes_end */

        /*SPR 6424 Fix End*/

        /* ICIC chnages start */
        /* CA Changes start */
        /* CA_phase2_csi_code Start */
        if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
        {
            updateDLSINRCW0(dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                    /* +- SPR 17777 */
                    servCellIndex);
        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */
        /* ICIC changes end */

        LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                getCurrentTick(),dlUEContext_p->ueIndex,
                dlCQIInfo_p->widebandCQICodeWordOne,
                dlCQIInfo_p->mcsIndexCodeWordOne,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"WB_CW0");

        /* + CQI_5.0 */
        /* CA Changes start */
        if (FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p\
                [internalCellIndex]->cellParams_p->freqSelectiveSchDL )
            /* CA Changes end */
        {
            /*Get the number of SubBand Count for which the report has came*/
            subBandCount = aperiodicCQIMode30_p->numberOfSubbands;

            dlCQIInfo_p->isSubBandReportAvailable = 1;
            subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;

            cqiInfo_p = &subBandReportInfo_p->cqiInfo;

            subbandInfo30_p = &aperiodicCQIMode30_p->subbandCQIInfo30[0]; 

            while (subBandCount--)
            {
                /*  subBandCount value denotes the subband number */
                /* CA Changes start */
                startRBGIndex = subBandToRBGMapPeriodic_p[subBandCount].startRBGIndex;
                endRBGIndex = subBandToRBGMapPeriodic_p[subBandCount].endRBGIndex;
                /* CA Changes end */
                rbgIndexCount = endRBGIndex - startRBGIndex + 1; 

                while(rbgIndexCount--)
                {
                    subBandToCQInfo_p = &subBandReportInfo_p->
                        subBandToCQInfo[startRBGIndex];
                    oldCqiForRBG = subBandToCQInfo_p->cqiIndexCW1;

                    /* SPR 4769 changes start */
                    /* Time Averaging */
                    subBandToCQInfo_p->previousTimeAveragedSBCQICW0 =
                        /* SPR 9427 changes start */
                        calculateAverageDLCQIforAP(subbandInfo30_p[subBandCount],
                                /* SPR 9427 changes end */
                                subBandToCQInfo_p->previousTimeAveragedSBCQICW0);

                    /* Correcting Subband CQI and MCS from BLER*/
                    subBandToCQInfo_p->cqiIndexCW1 = getEffectiveDLCQI(
                            subBandToCQInfo_p->previousTimeAveragedSBCQICW0,
                            dlCQIInfo_p->cqiCorrectionFactorForCW0,
                            dlUEContext_p);
                    correctedSubbandCQI = subBandToCQInfo_p->cqiIndexCW1;
                    correctedSubbandMCS = getMCSFromCQI(correctedSubbandCQI
                                                       );
                    /*SPR 6424 Fix Start*/

                    /* Cyclomatic_complexity_changes_start */
                    getModSchemeFromMcsIndex(correctedSubbandMCS,
                            /* CA_phase2_csi_code */
                            &(dlCQIInfo_p->modulationSchemeCodeWordOne));
                    /* Cyclomatic_complexity_changes_end */

                    /*SPR 6424 Fix End*/
                    subBandToCQInfo_p->mcsIndexCW1 = correctedSubbandMCS;
                    /* SPR 4769 changes end */

                    /* + CQI_5.0 */ 
                    subBandToCQInfo_p->initialReportedCQICW1 = subbandInfo30_p[subBandCount];
                    /* - CQI_5.0 */ 

                    /* SPR 3803 changes start */  
                    if ( oldCqiForRBG )
                    {    
                        /* SPR 3803 changes end */  
                        resetRAT0CW1NonSpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQI,
                                oldCqiForRBG,
                                &cqiInfo_p->cqiInfoNonSpatialMul);

                        resetRAT1CW1NonSpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQI,
                                oldCqiForRBG,
                                &cqiInfo_p->cqiInfoNonSpatialMul,
                                rat0Info_p,
                                rat1Info_p); 
                        /* SPR 3803 changes start */  
                    }
                    /* SPR 3803 changes end */  

                    updateRAT0CW1NonSpatialMulSubBandInfo(
                            startRBGIndex,
                            correctedSubbandCQI,
                            oldCqiForRBG,
                            &cqiInfo_p->cqiInfoNonSpatialMul);

                    updateRAT1CW1NonSpatialMulSubBandInfo(
                            startRBGIndex,
                            correctedSubbandCQI,
                            oldCqiForRBG,
                            &cqiInfo_p->cqiInfoNonSpatialMul,
                            rat0Info_p,
                            rat1Info_p,
                            tempReportedCQIBitmapPerSubset);

                    /* Updating the reported CQI Index */

                    SET_CQI_REPORT_BIT(reportedCQIBitMap,correctedSubbandCQI);      
                    startRBGIndex++;
                }

            }

            cqiInfo_p->cqiInfoNonSpatialMul.rat0NonSpatialMulCQIInfo.reportedCQIBitMap 
                = reportedCQIBitMap;
        }
        /* - CQI_5.0 */
        /* - CQI_4.1 */
    }
    return;

}

/*****************************************************************************
 * Function Name  : processCQIMode31
 * Inputs         : periodicCQIMode31_p - This holds pointer to 
 *                                         AperiodicCQIMode31 structure.
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the CQIInfoSpatialMul structure
 *                  for CodeWord One and CodeWord Two.
 *****************************************************************************/
/* CA Changes start */
STATIC void processCQIMode31(
        AperiodicCQIMode31 *aperiodicCQIMode31_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex)  
{
    DLCQIInfo *dlCQIInfo_p = PNULL;
    SubBandReportInfo *subBandReportInfo_p = PNULL;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    SubBandInfo31 *subbandInfo31_p = PNULL;
    UInt32 subBandCount = 0; 
    CQIInfo *cqiInfo_p = PNULL;
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;
    UInt32 startRBGIndex = 0;
    UInt32 endRBGIndex = 0;
    UInt32 rbgIndexCount = 0;
    /* + CQI_4.1 */
    UInt8 correctedSubbandCQICW0 = 0;
    UInt8 correctedSubbandCQICW1 = 0;
    UInt8 correctedSubbandMCSCW0 = 0;
    UInt8 correctedSubbandMCSCW1 = 0;
    UInt8 oldCqiForRBGCW0 = 0;
    UInt8 oldCqiForRBGCW1 = 0;
    UInt8 toProcessCW2 = FALSE;
    UInt8 scellArrIndex = INVALID_8_VAL;
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    /* - CQI_4.1 */
    /* CA Changes start */
    UInt8 numOfTxAnteenas =  cellSpecificParams_g.cellConfigAndInitParams_p\
                             [internalCellIndex]->cellParams_p->numOfTxAnteenas;

    SubBandToRBGMap *subBandToRBGMapPeriodic_p =  cqiGlobalParams_gp\
                                                  [internalCellIndex]->subBandToRBGMapPeriodic; 
    rat0Info_p = &(dlRAInfo_gp[internalCellIndex] + 
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info;
    rat1Info_p = &(dlRAInfo_gp[internalCellIndex] + 
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat1Info;

    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;

    }
    /* CA Changes end */
    /*+EICIC . */
    CHECK_APERIODIC_SECONDARY_CONDITION (ulUEContext_p, internalCellIndex)
    {    /* ABS Criteria */
        dlCQIInfo_p->isWideBandReportAvailable_2 = 1;
        /* SPR 8175 changes start */
        dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
        /* SPR 8175 changes end */
        /* + CQI_5.0 */
        /* Updating RI in DL context */
        /* CA_phase2_csi_code Start */
        if (dlUEContext_p->internalCellIndex == internalCellIndex)
        {
            /* Pcell */
            dlUEContext_p->dlMIMOInfo.dlRi_2 = aperiodicCQIMode31_p->receivedRI;
            /* - CQI_5.0 */
        }
        else
        {
            /* Scell */
            scellArrIndex = getServeCellIndexFromInternalCellId (dlUEContext_p,
                    internalCellIndex);
            /*klockwork warning fix*/
            if(scellArrIndex <= MAX_NUM_SCELL)
            {
                dlUEContext_p->dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo.
                    dlRi_2 = aperiodicCQIMode31_p->receivedRI;
            }
        }
        /* CA_phase2_csi_code End */

        /* SPR 4769 changes start */
        /* Time Averaging */
        /* SPR 9427 changes start */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0_2 = calculateAverageDLCQIforAP (
                /* SPR 9427 changes end */
                aperiodicCQIMode31_p->
                widebandCQIValueCodeword0,
                dlCQIInfo_p->
                previousTimeAveragedWBCQICW0_2);

        /* Correcting wideband CQI and  MCS for CW1 from BLER */
        dlCQIInfo_p->widebandCQICodeWordOne_2 =
            getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW0_2,
                    dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                    dlUEContext_p);

	dlCQIInfo_p->mcsIndexCodeWordOne_2 = 
                       getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordOne_2
                                     );
        /* SPR 4769 changes end */

        /*SPR 6424 Fix Start */
        getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordOne_2,
                /* CA_phase2_csi_code */
                &(dlCQIInfo_p->modulationSchemeCodeWordOne_2));
        /*SPR 6424 Fix End */
        /* ICIC chnages start */
        /* CA Changes start */
        /* CA_phase2_csi_code Start */
        if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
        {
            updateDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex, scellArrIndex);
        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */
        /* ICIC changes end */

        /* SS_S2 */
        if (0 != aperiodicCQIMode31_p->widebandCQIValueCodeword1)
        {
            /* SPR 4769 changes start */
            /* Time Averaging */
            /* SPR 9427 changes start */
            dlCQIInfo_p->previousTimeAveragedWBCQICW1_2 =
                calculateAverageDLCQIforAP (
                        /* SPR 9427 changes end */
                        aperiodicCQIMode31_p->
                        widebandCQIValueCodeword1,
                        dlCQIInfo_p->
                        previousTimeAveragedWBCQICW1_2);

            /* Correcting wideband CQI and MCS for CW2 from BLER */
            dlCQIInfo_p->widebandCQICodeWordTwo_2 =
                getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW1_2,
                        dlCQIInfo_p->cqiCorrectionFactorForCW1_2,
                        dlUEContext_p);

		dlCQIInfo_p->mcsIndexCodeWordTwo_2 = 
                        getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordTwo_2
                                     );
            /* SPR 4769 changes end */

            /*SPR 6424 Fix Start */
            getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordTwo_2,
                    /* CA_phase2_csi_code */
                    &(dlCQIInfo_p->
                        modulationSchemeCodeWordTwo_2));
            /*SPR 6424 Fix End */
            /* ICIC chnages start */
            /* CA Changes start */
            /* CA_phase2_csi_code Start */
            if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
            {
                updateDLSINRCW1_2 (dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordTwo_2,
                        internalCellIndex);
            }
            else
            {
                macUpdateSCellDLSINRCW1_2 (dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordTwo_2,
                        internalCellIndex, scellArrIndex);
            }
            /* CA_phase2_csi_code End */
            /* CA Changes end */
            /* ICIC changes end */

            LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                    getCurrentTick (), dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    dlCQIInfo_p->mcsIndexCodeWordOne_2,
                    dlCQIInfo_p->widebandCQICodeWordTwo_2,
                    dlCQIInfo_p->mcsIndexCodeWordTwo_2,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__,
                    "WB_BOTH_CW_WITH_ABS");

            toProcessCW2 = TRUE;
        }
        else
        {
            LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                    getCurrentTick (), dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,
                    dlCQIInfo_p->mcsIndexCodeWordOne, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE, __func__, "WB_CW0_ABS");

        }
        /* - CQI_4.1 */
        /* CA_phase2_csi_code Start */
        if (dlUEContext_p->internalCellIndex == internalCellIndex)
        {
            /* Pcell */
            /* + TM7_8 Changes Start */
        /* In case of TM8, wideBandPMIIndex is filled from higher layer precoding value 
         * i.e. precodingIndex instead of latest reported PMI value from UE */
            if (TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode)
            {
                dlCQIInfo_p->wideBandPMIIndex_2 =
                    dlUEContext_p->dlMIMOInfo.precodingIndex;
            }
            else
            {
                /* 4x4 DL MIMO CHG START */
                if (isPMIInCodebookSubsetRestriction
                        /* +- SPR 17777 */
                        (
                         dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue
#ifdef CSI_VALIDATION
                         ,aperiodicCQIMode31_p->widebandPMI,
                         aperiodicCQIMode31_p->receivedRI, dlUEContext_p->ueCategory,
                         dlUEContext_p->dlMIMOInfo.transmissionMode,
                         cellSpecificParams_g.
                         cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->
                         numOfTxAnteenas
#endif
                         ))
                    /* +- SPR 17777 */
                {
                    dlCQIInfo_p->wideBandPMIIndex_2 =
                        aperiodicCQIMode31_p->widebandPMI;
                }
                /*KLOCKWORK_CHANGES_START_4 */
#ifdef CSI_VALIDATION
                /*KLOCKWORK_CHANGES_STOP_4 */
                else
                {
                    /* Wrong PMI received */
                    LOG_MAC_MSG (MAC_RECEIVED_PMI_LOG_ID, LOGERROR, MAC_L1_INF,
                            getCurrentTick (), dlUEContext_p->ueCategory,
                            dlUEContext_p->dlMIMOInfo.transmissionMode,
                            aperiodicCQIMode31_p->receivedRI,
                            aperiodicCQIMode31_p->widebandPMI,
                            DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE, __func__,
                            "Wrong Aperiodic PMI Received for Mode30 For ABS");
                }
                /*KLOCKWORK_CHANGES_START_4 */
#endif
                /*KLOCKWORK_CHANGES_STOP_4 */
                /* 4x4 DL MIMO CHG END */
            }
            /* - TM7_8 Changes End */
        }
        else
        {
            /* Scell */
            /* + TM7_8 Changes Start */
			/* + Coverity 67175 */
			if( scellArrIndex <= MAX_NUM_SCELL )
			{
            	if (TX_MODE_8 ==
                    	dlUEContext_p->dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo.
                    	transmissionMode)
            	{
                    /* In case of TM8, wideBandPMIIndex is filled from higher layer precoding value 
                     * i.e. precodingIndex instead of latest reported PMI value from UE */
                	dlCQIInfo_p->wideBandPMIIndex_2 =
                    	dlUEContext_p->
                    	dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo.
                    	precodingIndex;
            	}
            	else
            	{
                	/* 4x4 DL MIMO CHG START */
                	if (isPMIInCodebookSubsetRestriction
                            /* +- SPR 17777 */
                        	(
                         	dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                         	codebookSubsetRestrictionv1020.cbsrValue
#ifdef CSI_VALIDATION
                            ,aperiodicCQIMode31_p->widebandPMI,
                         	aperiodicCQIMode31_p->receivedRI, dlUEContext_p->ueCategory,
                         	dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                         	ScelldlMIMOInfo.transmissionMode,
                         	cellSpecificParams_g.
                         	cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->
                         	numOfTxAnteenas
#endif
                            ))
                        /* +- SPR 17777 */
                	{
                    	dlCQIInfo_p->wideBandPMIIndex_2 =
                        	aperiodicCQIMode31_p->widebandPMI;
                	}
                	/*KLOCKWORK_CHANGES_START_4 */
#ifdef CSI_VALIDATION
                	/*KLOCKWORK_CHANGES_STOP_4 */
                	else
                	{
                    	/* Wrong PMI received */
                    	LOG_MAC_MSG (MAC_RECEIVED_PMI_LOG_ID, LOGERROR, MAC_L1_INF,
                            	getCurrentTick (), dlUEContext_p->ueCategory,
                            	dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                            	ScelldlMIMOInfo.transmissionMode,
                            	aperiodicCQIMode31_p->receivedRI,
                            	aperiodicCQIMode31_p->widebandPMI, DEFAULT_INT_VALUE,
                            	DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__,
                            	"Wrong Aperiodic PMI Received for Mode30 ON ABS");
                	}
                	/*KLOCKWORK_CHANGES_START_4 */
#endif
                	/*KLOCKWORK_CHANGES_STOP_4 */
                	/* 4x4 DL MIMO CHG END */
            	}
            	/* - TM7_8 Changes End */
            }
			/* - Coverity 67175 */
        }

        /* + CQI_5.0 */
        /* CA Changes start */
        if (FREQUENCY_SELECTIVE_ON ==
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
                cellParams_p->freqSelectiveSchDL)
            /* CA Changes end */
        {
            /*Get the number of SubBand Count for which the report has came */
            subBandCount = aperiodicCQIMode31_p->numberOfSubbands;
            dlCQIInfo_p->isSubBandReportAvailable = 1;
            subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;

            cqiInfo_p = &subBandReportInfo_p->cqiInfo;

            subbandInfo31_p =
                &aperiodicCQIMode31_p->subbandInfo31[subBandCount - 1];

            while (subBandCount--)
            {
                /* + CQI_4.1 */
                /* CA Changes start */
                startRBGIndex =
                    subBandToRBGMapPeriodic_p[subBandCount].startRBGIndex;
                endRBGIndex = subBandToRBGMapPeriodic_p[subBandCount].endRBGIndex;
                /* CA Changes end */
                rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                while (rbgIndexCount--)
                {

                    subBandToCQInfo_p =
                        &subBandReportInfo_p->subBandToCQInfo[startRBGIndex];
                    oldCqiForRBGCW0 = subBandToCQInfo_p->cqiIndexCW1_2;

                    /* SPR 4769 changes start */
                    /* Time Averaging */
                    /* SPR 9427 changes start */
                    subBandToCQInfo_p->previousTimeAveragedSBCQICW0_2 =
                        calculateAverageDLCQIforAP (
                                /* SPR 9427 changes end */
                                subbandInfo31_p->
                                subbandCQIValueCodeword0,
                                subBandToCQInfo_p->
                                previousTimeAveragedSBCQICW0_2);

                    /* Correcting Subband CQI and MCS for CW0 from BLER */
                    subBandToCQInfo_p->cqiIndexCW1_2 =
                        getEffectiveDLCQI (subBandToCQInfo_p->
                                previousTimeAveragedSBCQICW0_2,
                                dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                                dlUEContext_p);

                    correctedSubbandCQICW0 = subBandToCQInfo_p->cqiIndexCW1_2;
                    correctedSubbandMCSCW0 =
                        getMCSFromCQI (correctedSubbandCQICW0
                                        );
                    /*SPR 6424 Fix Start */
                    getModSchemeFromMcsIndex (correctedSubbandMCSCW0,
                            /*CA_phase2_csi_code */
                            &(dlCQIInfo_p->
                                modulationSchemeCodeWordOne_2));
                    /*SPR 6424 Fix End */
                    /* SPR 4769 changes end */

                    subBandToCQInfo_p->mcsIndexCW1_2 = correctedSubbandMCSCW0;
                    subBandToCQInfo_p->initialReportedCQICW1_2 =
                        subbandInfo31_p->subbandCQIValueCodeword0;

                    /* SPR 3803 changes start */
                    if (oldCqiForRBGCW0)
                    {
                        /* SPR 3803 changes end */
                        resetRAT0CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                                oldCqiForRBGCW0,
                                &cqiInfo_p->
                                cqiInfoSpatialMul);

                        resetRAT1CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                                oldCqiForRBGCW0,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p);
                        /* SPR 3803 changes start */
                    }
                    /* SPR 3803 changes end */

                    /* Subbands for which subband PMI is not received, 
                     * should be initialized with wideband PMI */
                    updateRAT0CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                            correctedSubbandCQICW0,
                            INVALID_PMI_VALUE,
                            &cqiInfo_p->
                            cqiInfoSpatialMul,
                            /* CA Changes start */
                            numOfTxAnteenas);
                    /* CA Changes end */

                    updateRAT1CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                            correctedSubbandCQICW0,
                            INVALID_PMI_VALUE,
                            &cqiInfo_p->
                            cqiInfoSpatialMul,
                            rat0Info_p, rat1Info_p,
                            /* CA Changes start */
                            numOfTxAnteenas);
                    /* CA Changes end */

                    if (TRUE == toProcessCW2)
                    {
                        oldCqiForRBGCW1 = subBandToCQInfo_p->cqiIndexCW2_2; 

                        /* SPR 4769 changes start */
                        /* Time Averaging */
                        /* SPR 9427 changes start */
                        subBandToCQInfo_p->previousTimeAveragedSBCQICW1_2 =
                            calculateAverageDLCQIforAP (
                                    /* SPR 9427 changes start */
                                    subbandInfo31_p->
                                    subbandCQIValueCodeword1,
                                    subBandToCQInfo_p->
                                    previousTimeAveragedSBCQICW1_2);

                        /* Correcting Subband CQI and MCS for CW1 */
                        subBandToCQInfo_p->cqiIndexCW2_2 =
                            getEffectiveDLCQI (subBandToCQInfo_p->
                                    previousTimeAveragedSBCQICW1_2,
                                    dlCQIInfo_p->
                                    cqiCorrectionFactorForCW1_2,
                                    dlUEContext_p);

                    correctedSubbandCQICW1 = subBandToCQInfo_p->cqiIndexCW2_2;
                    correctedSubbandMCSCW1 = getMCSFromCQI(correctedSubbandCQICW1
                                             );
                        /* SPR 4769 changes end */
                        /*SPR 6424 Fix Start */
                        getModSchemeFromMcsIndex (correctedSubbandMCSCW1,
                                &(dlCQIInfo_p->
                                    modulationSchemeCodeWordTwo_2));
                        /*SPR 6424 Fix End */

                        subBandToCQInfo_p->mcsIndexCW2_2 = correctedSubbandMCSCW1;
                        subBandToCQInfo_p->initialReportedCQICW2_2 =
                            subbandInfo31_p->subbandCQIValueCodeword1;

                        /* SPR 3803 changes start */
                        if (oldCqiForRBGCW1)
                        {
                            /* SPR 3803 changes end */
                            resetRAT0CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                                    oldCqiForRBGCW1,
                                    &cqiInfo_p->
                                    cqiInfoSpatialMul);

                            resetRAT1CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                                    oldCqiForRBGCW1,
                                    &cqiInfo_p->
                                    cqiInfoSpatialMul,
                                    rat0Info_p,
                                    rat1Info_p);
                            /* SPR 3803 changes start */
                        }
                        /* SPR 3803 changes end */

                        /* Subbands for which subband PMI is not received, 
                         * should be initialized with wideband PMI */
                        updateRAT0CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQICW1,
                                INVALID_PMI_VALUE,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */

                        updateRAT1CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQICW1,
                                INVALID_PMI_VALUE,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */
                    }

                    startRBGIndex++;
                }

                /* - CQI_4.1 */
                subbandInfo31_p--;
            }
        }
        /* - CQI_5.0 */
        ulUEContext_p->isDLWideBandCQIReportAvailable_2 = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne_2 =
            aperiodicCQIMode31_p->widebandCQIValueCodeword0;
        ulUEContext_p->dlWidebandCQICodeWordTwo_2 =
            aperiodicCQIMode31_p->widebandCQIValueCodeword1;
    }
    else
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne =
            aperiodicCQIMode31_p->widebandCQIValueCodeword0;
        ulUEContext_p->dlWidebandCQICodeWordTwo =
            aperiodicCQIMode31_p->widebandCQIValueCodeword1;
        dlCQIInfo_p->isWideBandReportAvailable = 1;
        /* SPR 8175 changes start */
        dlCQIInfo_p->riUpdationFlag = IMMEDIATE_DL_RI_UPDATE;
        /* SPR 8175 changes end */

        /* + CQI_5.0 */
        /* Updating RI in DL context*/
        /* CA_phase2_csi_code Start */
        if(dlUEContext_p->internalCellIndex == internalCellIndex)
        {
            /* Pcell */
            dlUEContext_p->dlMIMOInfo.dlRi = aperiodicCQIMode31_p->receivedRI; 
            /* - CQI_5.0 */
        }
        else
        {
            /* Scell */
            scellArrIndex =  getServeCellIndexFromInternalCellId(dlUEContext_p,
                    internalCellIndex);
            /*klockwork warning fix*/
            if(scellArrIndex <= MAX_NUM_SCELL)
            {
                dlUEContext_p->dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo.dlRi = 
                    aperiodicCQIMode31_p->receivedRI;
            }
            /** Coverity 67175 Fix : Start **/
            else
            {
                return ;
            }    
            /** Coverity 67175 Fix : End **/
        }
        /* CA_phase2_csi_code End */

        /* SPR 4769 changes start */
        /* Time Averaging */
        /* SPR 9427 changes start */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0 = calculateAverageDLCQIforAP(
                /* SPR 9427 changes end */
                aperiodicCQIMode31_p->widebandCQIValueCodeword0,
                dlCQIInfo_p->previousTimeAveragedWBCQICW0);

        /* Correcting wideband CQI and  MCS for CW1 from BLER*/
        dlCQIInfo_p->widebandCQICodeWordOne = getEffectiveDLCQI(
                dlCQIInfo_p->previousTimeAveragedWBCQICW0,
                dlCQIInfo_p->cqiCorrectionFactorForCW0,
                dlUEContext_p );

        dlCQIInfo_p->mcsIndexCodeWordOne = 
                        getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordOne
                                     );
        /* SPR 4769 changes end */

        /*SPR 6424 Fix Start*/

        /* Cyclomatic_complexity_changes_start */
        getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordOne,
                /* CA_phase2_csi_code */
                &(dlCQIInfo_p->modulationSchemeCodeWordOne));
        /* Cyclomatic_complexity_changes_end */

        /*SPR 6424 Fix End*/
        /* ICIC chnages start */
        /* CA Changes start */
        /* CA_phase2_csi_code Start */
        if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
        {
            updateDLSINRCW0(dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                    /* +- SPR 17777 */
                    scellArrIndex);
        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */
        /* ICIC changes end */

        /* SS_S2 */
        if (0 != aperiodicCQIMode31_p->widebandCQIValueCodeword1)
        {
            /* SPR 4769 changes start */
            /* Time Averaging */
            /* SPR 9427 changes start */
            dlCQIInfo_p->previousTimeAveragedWBCQICW1 = calculateAverageDLCQIforAP(
                    /* SPR 9427 changes end */
                    aperiodicCQIMode31_p->widebandCQIValueCodeword1,
                    dlCQIInfo_p->previousTimeAveragedWBCQICW1);

            /* Correcting wideband CQI and MCS for CW2 from BLER */
            dlCQIInfo_p->widebandCQICodeWordTwo = getEffectiveDLCQI(
                    dlCQIInfo_p->previousTimeAveragedWBCQICW1,
                    dlCQIInfo_p->cqiCorrectionFactorForCW1,
                    dlUEContext_p );

            dlCQIInfo_p->mcsIndexCodeWordTwo = 
                          getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordTwo
                                        );
            /* SPR 4769 changes end */

            /*SPR 6424 Fix Start*/

            /* Cyclomatic_complexity_changes_start */
            getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordTwo,
                    /* CA_phase2_csi_code */
                    &(dlCQIInfo_p->modulationSchemeCodeWordTwo));
            /* Cyclomatic_complexity_changes_end */

            /*SPR 6424 Fix End*/
            /* ICIC chnages start */
            /* CA Changes start */
            /* CA_phase2_csi_code Start */
            if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
            {
                updateDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo,
                        internalCellIndex);
            }
            else
            {
                macUpdateSCellDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->
                        /* +- SPR 17777 */
                        widebandCQICodeWordTwo,scellArrIndex);
            }
            /* CA_phase2_csi_code End */
            /* CA Changes end */
            /* ICIC changes end */

            LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                    getCurrentTick(),dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,
                    dlCQIInfo_p->mcsIndexCodeWordOne,
                    dlCQIInfo_p->widebandCQICodeWordTwo,
                    dlCQIInfo_p->mcsIndexCodeWordTwo,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,__func__,"WB_BOTH_CW");

            toProcessCW2 = TRUE;
        }
        else
        {
            LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                    getCurrentTick(),dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,
                    dlCQIInfo_p->mcsIndexCodeWordOne,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,__func__,"WB_CW0");

        }    
        /* - CQI_4.1 */  
        /* CA_phase2_csi_code Start */
        if(dlUEContext_p->internalCellIndex == internalCellIndex)
        {
            /* Pcell */
            /* + TM7_8 Changes Start */
            /* In case of TM8, wideBandPMIIndex is filled from higher layer precoding value 
             * i.e. precodingIndex instead of latest reported PMI value from UE */
            if(TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode)
            {
                dlCQIInfo_p->wideBandPMIIndex =  dlUEContext_p->dlMIMOInfo.precodingIndex;
            }
            else
            {
                /* 4x4 DL MIMO CHG START */
                /* +- SPR 17777 */
                if(isPMIInCodebookSubsetRestriction(
                            dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue
#ifdef CSI_VALIDATION
                            ,aperiodicCQIMode31_p->widebandPMI,
                            aperiodicCQIMode31_p->receivedRI,
                            dlUEContext_p->ueCategory,
                            dlUEContext_p->dlMIMOInfo.transmissionMode,
                            cellSpecificParams_g.cellConfigAndInitParams_p\
                            [internalCellIndex]->cellParams_p->numOfTxAnteenas
#endif
                            ))
                    /* +- SPR 17777 */
                {
                    dlCQIInfo_p->wideBandPMIIndex = aperiodicCQIMode31_p->widebandPMI;
                }
                /*KLOCKWORK_CHANGES_START_4*/
#ifdef CSI_VALIDATION
                /*KLOCKWORK_CHANGES_STOP_4*/
                else
                {
                    /* Wrong PMI received */
                    LOG_MAC_MSG(MAC_RECEIVED_PMI_LOG_ID,LOGERROR,MAC_L1_INF,\
                            getCurrentTick(),dlUEContext_p->ueCategory,\
                            dlUEContext_p->dlMIMOInfo.transmissionMode,\
                            aperiodicCQIMode31_p->receivedRI,\
                            aperiodicCQIMode31_p->widebandPMI,\
                            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                            DEFAULT_FLOAT_VALUE,__func__,\
                            "Wrong Aperiodic PMI Received for Mode30");
                }
                /*KLOCKWORK_CHANGES_START_4*/
#endif
                /*KLOCKWORK_CHANGES_STOP_4*/
                /* 4x4 DL MIMO CHG END */
            }
            /* - TM7_8 Changes End */
        }/*klockworks warning fix*/
        else if(scellArrIndex <= MAX_NUM_SCELL)
        {
            /* Scell */
            /* + TM7_8 Changes Start */
            /* In case of TM8, wideBandPMIIndex is filled from higher layer precoding value 
             * i.e. precodingIndex instead of latest reported PMI value from UE */
            if(TX_MODE_8 == dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                    ScelldlMIMOInfo.transmissionMode)
            {
                dlCQIInfo_p->wideBandPMIIndex =  dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                    ScelldlMIMOInfo.precodingIndex;
            }
            else
            {
                /* 4x4 DL MIMO CHG START */
                /* +- SPR 17777 */
                if(isPMIInCodebookSubsetRestriction(
                            dlUEContext_p->dlUeScellContext_p[scellArrIndex]->codebookSubsetRestrictionv1020.cbsrValue
#ifdef CSI_VALIDATION
                            ,aperiodicCQIMode31_p->widebandPMI,
                            aperiodicCQIMode31_p->receivedRI,
                            dlUEContext_p->ueCategory,
                            dlUEContext_p->dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo.transmissionMode,
                            cellSpecificParams_g.cellConfigAndInitParams_p\
                            [internalCellIndex]->cellParams_p->numOfTxAnteenas
#endif
                            ))
                    /* +- SPR 17777 */
                {
                    dlCQIInfo_p->wideBandPMIIndex = aperiodicCQIMode31_p->widebandPMI;
                }
                /*KLOCKWORK_CHANGES_START_4*/
#ifdef CSI_VALIDATION
                /*KLOCKWORK_CHANGES_STOP_4*/
                else
                {
                    /* Wrong PMI received */
                    LOG_MAC_MSG(MAC_RECEIVED_PMI_LOG_ID,LOGERROR,MAC_L1_INF,\
                            getCurrentTick(),dlUEContext_p->ueCategory,\
                            dlUEContext_p->dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo.transmissionMode,\
                            aperiodicCQIMode31_p->receivedRI,\
                            aperiodicCQIMode31_p->widebandPMI,\
                            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                            DEFAULT_FLOAT_VALUE,__func__,\
                            "Wrong Aperiodic PMI Received for Mode30");
                }
                /*KLOCKWORK_CHANGES_START_4*/
#endif
                /*KLOCKWORK_CHANGES_STOP_4*/
                /* 4x4 DL MIMO CHG END */
            }
            /* - TM7_8 Changes End */
        }

        /* + CQI_5.0 */
        /* CA Changes start */
        if ( FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p\
                [internalCellIndex]->cellParams_p->freqSelectiveSchDL )
            /* CA Changes end */
        {
            /*Get the number of SubBand Count for which the report has came*/
            subBandCount = aperiodicCQIMode31_p->numberOfSubbands;
            dlCQIInfo_p->isSubBandReportAvailable = 1; 
            subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;

            cqiInfo_p = &subBandReportInfo_p->cqiInfo;

            subbandInfo31_p = &aperiodicCQIMode31_p->subbandInfo31[subBandCount - 1]; 

            while (subBandCount--)
            {
                /* + CQI_4.1 */  
                /* CA Changes start */
                startRBGIndex = subBandToRBGMapPeriodic_p[subBandCount].startRBGIndex;
                endRBGIndex = subBandToRBGMapPeriodic_p[subBandCount].endRBGIndex;
                /* CA Changes end */
                rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                while(rbgIndexCount--)
                {

                    subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[startRBGIndex];
                    oldCqiForRBGCW0 = subBandToCQInfo_p->cqiIndexCW1;

                    /* SPR 4769 changes start */
                    /* Time Averaging */
                    /* SPR 9427 changes start */
                    subBandToCQInfo_p->previousTimeAveragedSBCQICW0 = calculateAverageDLCQIforAP(
                            /* SPR 9427 changes end */
                            subbandInfo31_p->subbandCQIValueCodeword0,
                            subBandToCQInfo_p->previousTimeAveragedSBCQICW0);

                    /* Correcting Subband CQI and MCS for CW0 from BLER */
                    subBandToCQInfo_p->cqiIndexCW1 = getEffectiveDLCQI(
                            subBandToCQInfo_p->previousTimeAveragedSBCQICW0,
                            dlCQIInfo_p->cqiCorrectionFactorForCW0,
                            dlUEContext_p );

                    correctedSubbandCQICW0 = subBandToCQInfo_p->cqiIndexCW1;
                    correctedSubbandMCSCW0 = getMCSFromCQI(correctedSubbandCQICW0
                                             );
                    /*SPR 6424 Fix Start*/

                    /* Cyclomatic_complexity_changes_start */
                    getModSchemeFromMcsIndex(correctedSubbandMCSCW0,
                            /*CA_phase2_csi_code*/
                            &(dlCQIInfo_p->modulationSchemeCodeWordOne));
                    /* Cyclomatic_complexity_changes_end */

                    /*SPR 6424 Fix End*/
                    /* SPR 4769 changes end */

                    subBandToCQInfo_p->mcsIndexCW1 = correctedSubbandMCSCW0;
                    subBandToCQInfo_p->initialReportedCQICW1 = 
                        subbandInfo31_p->subbandCQIValueCodeword0;

                    /* SPR 3803 changes start */  
                    if (oldCqiForRBGCW0)
                    {    
                        /* SPR 3803 changes end */  
                        resetRAT0CW1SpatialMulSubBandInfo(
                                startRBGIndex,
                                oldCqiForRBGCW0,
                                &cqiInfo_p->cqiInfoSpatialMul);

                        resetRAT1CW1SpatialMulSubBandInfo(
                                startRBGIndex,
                                oldCqiForRBGCW0,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p);
                        /* SPR 3803 changes start */  
                    }
                    /* SPR 3803 changes end */  

                    /* Subbands for which subband PMI is not received, 
                     * should be initialized with wideband PMI */
                    updateRAT0CW1SpatialMulSubBandInfo(
                            startRBGIndex,
                            correctedSubbandCQICW0,
                            INVALID_PMI_VALUE,
                            &cqiInfo_p->cqiInfoSpatialMul,
                            /* CA Changes start */ 
                            numOfTxAnteenas);
                    /* CA Changes end */ 

                    updateRAT1CW1SpatialMulSubBandInfo(
                            startRBGIndex,
                            correctedSubbandCQICW0,
                            INVALID_PMI_VALUE,
                            &cqiInfo_p->cqiInfoSpatialMul,
                            rat0Info_p,
                            rat1Info_p,
                            /* CA Changes start */ 
                            numOfTxAnteenas);
                    /* CA Changes end */ 

                    if (TRUE == toProcessCW2)
                    {    
                        oldCqiForRBGCW1 = subBandToCQInfo_p->cqiIndexCW2;

                        /* SPR 4769 changes start */
                        /* Time Averaging */
                        /* SPR 9427 changes start */
                        subBandToCQInfo_p->previousTimeAveragedSBCQICW1 = calculateAverageDLCQIforAP(
                                /* SPR 9427 changes start */
                                subbandInfo31_p->subbandCQIValueCodeword1,
                                subBandToCQInfo_p->previousTimeAveragedSBCQICW1);

                        /* Correcting Subband CQI and MCS for CW1*/
                        subBandToCQInfo_p->cqiIndexCW2 = getEffectiveDLCQI(
                                subBandToCQInfo_p->previousTimeAveragedSBCQICW1,
                                dlCQIInfo_p->cqiCorrectionFactorForCW1,
                                dlUEContext_p );

                        correctedSubbandCQICW1 = subBandToCQInfo_p->cqiIndexCW2;
                        correctedSubbandMCSCW1 = getMCSFromCQI(correctedSubbandCQICW1
                                                  );
                        /* SPR 4769 changes end */
                        /*SPR 6424 Fix Start*/

                        /* Cyclomatic_complexity_changes_start */
                        getModSchemeFromMcsIndex(correctedSubbandMCSCW1,
                                &(dlCQIInfo_p->modulationSchemeCodeWordTwo));
                        /* Cyclomatic_complexity_changes_end */

                        /*SPR 6424 Fix End*/

                        subBandToCQInfo_p->mcsIndexCW2 = correctedSubbandMCSCW1;
                        subBandToCQInfo_p->initialReportedCQICW2 = 
                            subbandInfo31_p->subbandCQIValueCodeword1;

                        /* SPR 3803 changes start */  
                        if (oldCqiForRBGCW1)
                        {    
                            /* SPR 3803 changes end */  
                            resetRAT0CW2SpatialMulSubBandInfo(
                                    startRBGIndex,
                                    oldCqiForRBGCW1,
                                    &cqiInfo_p->cqiInfoSpatialMul);

                            resetRAT1CW2SpatialMulSubBandInfo(
                                    startRBGIndex,
                                    oldCqiForRBGCW1,
                                    &cqiInfo_p->cqiInfoSpatialMul,
                                    rat0Info_p,
                                    rat1Info_p);
                            /* SPR 3803 changes start */  
                        }
                        /* SPR 3803 changes end */  

                        /* Subbands for which subband PMI is not received, 
                         * should be initialized with wideband PMI */
                        updateRAT0CW2SpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQICW1,
                                INVALID_PMI_VALUE,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                /* CA Changes start */ 
                                numOfTxAnteenas);
                        /* CA Changes end */ 

                        updateRAT1CW2SpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQICW1,
                                INVALID_PMI_VALUE,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p,
                                /* CA Changes start */ 
                                numOfTxAnteenas);
                        /* CA Changes end */ 
                    }

                    startRBGIndex++;
                }

                /* - CQI_4.1 */  
                subbandInfo31_p--;
            }
        }
    }    /* - CQI_5.0 */
    return;
}

/*****************************************************************************
 * Function Name  : processCQIMode12
 * Inputs         : aperiodicCQIMode12_p - This holds pointer to 
 *                                         AperiodicCQIMode12 structure,
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the SubBandToCQInfo structure
 *                  for CodeWord One and CodeWord Two.
 *****************************************************************************/
STATIC void processCQIMode12(
        AperiodicCQIMode12 *aperiodicCQIMode12_p,
        DLUEContext *dlUEContext_p,
        InternalCellIndex internalCellIndex)
/* CA Changes end */ 
{
    DLCQIInfo *dlCQIInfo_p = PNULL;
    SubBandReportInfo *subBandReportInfo_p = PNULL;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    UInt32 subBandCount = 0;
    UInt8 *pmiInfo12_p = PNULL;
    /* CA_phase2_csi_code Start */
    TransmissonMode transmissionMode;
    UInt8 scellArrIndex = INVALID_8_VAL;
    UInt8 *cbsrValue;
    UInt8 *dlRI_p;
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;

    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;
    }

  /*+EICIC . */
  CHECK_APERIODIC_SECONDARY_CONDITION (ulUEContext_p, internalCellIndex)
  {    /* ABS Criteria */
      dlCQIInfo_p->reportReceivedTTI_2 =
          aperiodicCQIMode12_p->reportReceivedTTI; 
      dlCQIInfo_p->latestAperiodicReportRcvd_2 = FALSE;
      if (dlUEContext_p->internalCellIndex == internalCellIndex)
      {
          /* Pcell */
          dlRI_p = &(dlUEContext_p->dlMIMOInfo.dlRi_2);
          transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
          cbsrValue = dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue;
      }
      else
      {
          scellArrIndex = getServeCellIndexFromInternalCellId (dlUEContext_p,
                  internalCellIndex);
          /*klockwork warning fix*/
          if(scellArrIndex <= MAX_NUM_SCELL)
          {
              dlRI_p = &(dlUEContext_p->dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo.dlRi_2);
              transmissionMode =  dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                  ScelldlMIMOInfo.transmissionMode;
              cbsrValue = dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                  codebookSubsetRestrictionv1020.cbsrValue;
          }
          else
          {
              return;
          }
      }
      /* Updating RI in DL context */
      *dlRI_p = aperiodicCQIMode12_p->receivedRI;
      /* CA_phase2_csi_code End */
      CQIInfo *cqiInfo_p = PNULL;
      RAT0Info *rat0Info_p = PNULL;
      RAT1Info *rat1Info_p = PNULL;
      UInt32 startRBGIndex = 0;
      UInt32 endRBGIndex = 0;
      UInt32 rbgIndexCount = 0;
      /* CA Changes start */
      UInt8 numOfTxAnteenas = cellSpecificParams_g.cellConfigAndInitParams_p
          [internalCellIndex]->cellParams_p->numOfTxAnteenas;
      SubBandToRBGMap *subBandToRBGMapPeriodic_p = cqiGlobalParams_gp
          [internalCellIndex]->subBandToRBGMapPeriodic;
      rat0Info_p = &(dlRAInfo_gp[internalCellIndex] +
              dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info;
      rat1Info_p = &(dlRAInfo_gp[internalCellIndex] +
              dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat1Info;
      /* CA Changes end */
      /* -CQI_5.0 */

      dlCQIInfo_p->isWideBandReportAvailable_2 = 1;
      /* SPR 8175 changes start */
      dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
      /* SPR 8175 changes end */
      /* + CQI_4.1 */
      /* Number Of Subband */
      subBandCount = aperiodicCQIMode12_p->numberOfSubbands;

      /* SPR 4769 changes start */
      /* Time Averaging */
      /* SPR 9427 changes start */
      dlCQIInfo_p->previousTimeAveragedWBCQICW0_2 = calculateAverageDLCQIforAP (
              /* SPR 9427 changes end */
              aperiodicCQIMode12_p->
              widebandCQIValueCodeword0,
              dlCQIInfo_p->
              previousTimeAveragedWBCQICW0_2);

      /* Correcting wideband CQI and  MCS for CW1 from BLER */
      dlCQIInfo_p->widebandCQICodeWordOne_2 =
          getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW0_2,
                  dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                  dlUEContext_p);

      dlCQIInfo_p->mcsIndexCodeWordOne_2 =
          getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordOne_2


                         );
      /* SPR 4769 changes end */
      /*SPR 6424 Fix Start */
      getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordOne_2,
              &(dlCQIInfo_p->modulationSchemeCodeWordOne_2));
      /*SPR 6424 Fix End */

      /* ICIC chnages start */
      /* CA Changes start */
      /* CA_phase2_csi_code Start */
      if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
      {
          updateDLSINRCW0_2 (dlUEContext_p->ueIndex,
                  dlCQIInfo_p->widebandCQICodeWordOne_2,
                  internalCellIndex);
      }
      else
      {
          macUupdateSCellDLSINRCW0_2 (dlUEContext_p->ueIndex,
                  dlCQIInfo_p->widebandCQICodeWordOne_2,
                  internalCellIndex, scellArrIndex);
      }
      /* CA_phase2_csi_code End */
      /* CA Changes end */
      /* ICIC changes end */

      if (0 != aperiodicCQIMode12_p->widebandCQIValueCodeword1)
      {
          /* SPR 4769 changes start */
          /* Time Averaging */
          /* SPR 9427 changes start */
          dlCQIInfo_p->previousTimeAveragedWBCQICW1_2 =
              calculateAverageDLCQIforAP (
                      /* SPR 9427 changes start */
                      aperiodicCQIMode12_p->
                      widebandCQIValueCodeword1,
                      dlCQIInfo_p->
                      previousTimeAveragedWBCQICW1_2);

          /* Correcting wideband CQI and MCS for CW1 from BLER */
          dlCQIInfo_p->widebandCQICodeWordTwo_2 =
              getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW1_2,
                      dlCQIInfo_p->cqiCorrectionFactorForCW1_2,
                      dlUEContext_p);

          dlCQIInfo_p->mcsIndexCodeWordTwo_2 =
              getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordTwo_2
                                );
          /* SPR 4769 changes end */
          /*SPR 6424 Fix Start */
          getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordTwo_2,
                  &(dlCQIInfo_p->
                      modulationSchemeCodeWordTwo_2));
          /*SPR 6424 Fix End */
          /* SPR 13855 fix start */
          if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
          {
              updateDLSINRCW1_2(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo_2,
                      internalCellIndex);
          }
          else
          {
              macUpdateSCellDLSINRCW1_2(dlUEContext_p->ueIndex,dlCQIInfo_p->
                      widebandCQICodeWordTwo_2, internalCellIndex,scellArrIndex);
          }

          /* SPR 13855 fix end */

          LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                  getCurrentTick (), dlUEContext_p->ueIndex,
                  dlCQIInfo_p->widebandCQICodeWordOne_2,
                  dlCQIInfo_p->mcsIndexCodeWordOne_2,
                  dlCQIInfo_p->widebandCQICodeWordTwo_2,
                  dlCQIInfo_p->mcsIndexCodeWordTwo_2,
                  DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__,
                  "WB_BOTH_CW_FOR_ABS");

      }
      else
      {
          LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                  getCurrentTick (), dlUEContext_p->ueIndex,
                  dlCQIInfo_p->widebandCQICodeWordOne_2,
                  dlCQIInfo_p->mcsIndexCodeWordOne_2, DEFAULT_INT_VALUE,
                  DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                  DEFAULT_FLOAT_VALUE, __func__, "WB_CW0_ABS");
      }

      dlCQIInfo_p->isSubBandReportAvailable_2 = 0;
      /* CA_phase2_csi_code Start */
      if (dlUEContext_p->internalCellIndex == internalCellIndex)
      {
          dlUEContext_p->aperiodicMode12Rcvd = TRUE;
      }
      else
      {
          dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
              aperiodicMode12Rcvd = TRUE;
      }

      /* + TM7_8 Changes Start */
      if (!(TX_MODE_8 == transmissionMode))
          /* CA_phase2_csi_code End */
      {
          /* - TM7_8 Changes End */
          subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;

          /*  subBandCount value denotes the subband number */
          pmiInfo12_p = &aperiodicCQIMode12_p->pmiInfo12[subBandCount - 1];

          subBandToCQInfo_p =
              &subBandReportInfo_p->subBandToCQInfo[subBandCount - 1];

          /* + CQI_5.0 */
          cqiInfo_p = &subBandReportInfo_p->cqiInfo;
          /* - CQI_5.0 */

          while (subBandCount--)
          {
              /* 4x4 DL MIMO CHG START */
              /* CA_phase2_csi_code start */
              /* +- SPR 17777 */
              if (isPMIInCodebookSubsetRestriction (
                          cbsrValue
#ifdef CSI_VALIDATION
                          ,*pmiInfo12_p,
                          aperiodicCQIMode12_p->
                          receivedRI,
                          dlUEContext_p->ueCategory,
                          transmissionMode,
                          cellSpecificParams_g.
                          cellConfigAndInitParams_p
                          [internalCellIndex]->
                          cellParams_p->
                          numOfTxAnteenas
#endif
                          ))
              /* +- SPR 17777 */
              {
                  subBandToCQInfo_p->preCodingMatrixIndex_2 = *pmiInfo12_p;
              }
              /*KLOCKWORK_CHANGES_START_4 */
#ifdef CSI_VALIDATION
              /*KLOCKWORK_CHANGES_STOP_4 */
              else
              {
                  LOG_MAC_MSG (MAC_RECEIVED_PMI_LOG_ID,LOGERROR,MAC_L1_INF,
                          getCurrentTick (), dlUEContext_p->ueCategory,
                          transmissionMode,
                          aperiodicCQIMode12_p->receivedRI,
                          *pmiInfo12_p,
                          DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                          DEFAULT_FLOAT_VALUE, __func__,
                          "For Mode12 for ABS, Wrong Aperiodic Sub Band PMI Received");
              }
              /* CA_phase2_csi_code end */
              /*KLOCKWORK_CHANGES_START_4 */
#endif
              /*KLOCKWORK_CHANGES_STOP_4 */
              /* 4x4 DL MIMO CHG END */
              /* + CQI_5.0 */
              /* As aperiodic mode 12 is wideband mode, but PMI received
               * for each SB, so all the RBG/PRBs should be initialized
               * with corresponding received subband PMI */
              /* CA Changes start */
              startRBGIndex =
                  subBandToRBGMapPeriodic_p[subBandCount].startRBGIndex;
              endRBGIndex = subBandToRBGMapPeriodic_p[subBandCount].endRBGIndex;
              /* CA Changes end */
              rbgIndexCount = endRBGIndex - startRBGIndex + 1;
              while (rbgIndexCount--)
              {
                  if (1 == *dlRI_p)
                  {
                      updateRAT0CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                              INVALID_CQI_VALUE,
                              subBandToCQInfo_p->
                              preCodingMatrixIndex_2,
                              &cqiInfo_p->
                              cqiInfoSpatialMul,
                              /* CA Changes start */
                              numOfTxAnteenas);
                      /* CA Changes end */

                      updateRAT1CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                              INVALID_CQI_VALUE,
                              subBandToCQInfo_p->
                              preCodingMatrixIndex_2,
                              &cqiInfo_p->
                              cqiInfoSpatialMul,
                              rat0Info_p,
                              rat1Info_p,
                              /* CA Changes start */
                              numOfTxAnteenas);
                      /* CA Changes end */
                  }
                  else if (2 == *dlRI_p)
                  {
                      updateRAT0CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                              INVALID_CQI_VALUE,
                              subBandToCQInfo_p->
                              preCodingMatrixIndex_2,
                              &cqiInfo_p->
                              cqiInfoSpatialMul,
                              /* CA Changes start */
                              numOfTxAnteenas);
                      /* CA Changes end */

                      updateRAT1CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                              INVALID_CQI_VALUE,
                              subBandToCQInfo_p->
                              preCodingMatrixIndex_2,
                              &cqiInfo_p->
                              cqiInfoSpatialMul,
                              rat0Info_p,
                              rat1Info_p,
                              /* CA Changes start */
                              numOfTxAnteenas);
                      /* CA Changes end */
                  }
                  startRBGIndex++;
              }
              subBandToCQInfo_p--;
              pmiInfo12_p--;
          }
          /* - CQI_5.0 */
          /* + TM7_8 Changes Start */
      }
      /* - TM7_8 Changes End */
      ulUEContext_p->isDLWideBandCQIReportAvailable_2 = TRUE;
      ulUEContext_p->dlWidebandCQICodeWordOne_2 =
          aperiodicCQIMode12_p->widebandCQIValueCodeword0;
      ulUEContext_p->dlWidebandCQICodeWordTwo_2 =
          aperiodicCQIMode12_p->widebandCQIValueCodeword1;

  }
  else
  {
      ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
      ulUEContext_p->dlWidebandCQICodeWordOne =
          aperiodicCQIMode12_p->widebandCQIValueCodeword0;
      ulUEContext_p->dlWidebandCQICodeWordTwo =
          aperiodicCQIMode12_p->widebandCQIValueCodeword1;

    /* + CQI_5.0 */ 
    dlCQIInfo_p->reportReceivedTTI = 
        aperiodicCQIMode12_p->reportReceivedTTI; 
    dlCQIInfo_p->latestAperiodicReportRcvd = FALSE;
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        dlRI_p = &(dlUEContext_p->dlMIMOInfo.dlRi);
        transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
        cbsrValue =  dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue;
    }
    else
    {
        scellArrIndex =  getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
        /*klockwork warning fix*/
        if(scellArrIndex <= MAX_NUM_SCELL)
        {
            dlRI_p = &(dlUEContext_p->dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo.dlRi);
            transmissionMode =  dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                ScelldlMIMOInfo.transmissionMode;
            cbsrValue = dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                codebookSubsetRestrictionv1020.cbsrValue;
        }
        else
        {
            return;
        }
    }
    /* Updating RI in DL context*/
    *dlRI_p = aperiodicCQIMode12_p->receivedRI; 
    /* CA_phase2_csi_code End */
    CQIInfo *cqiInfo_p = PNULL;
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;
    UInt32 startRBGIndex = 0;
    UInt32 endRBGIndex = 0;
    UInt32 rbgIndexCount = 0;
    /* CA Changes start */ 
    UInt8 numOfTxAnteenas =  cellSpecificParams_g.cellConfigAndInitParams_p\
                             [internalCellIndex]->cellParams_p->numOfTxAnteenas;
    SubBandToRBGMap *subBandToRBGMapPeriodic_p =  cqiGlobalParams_gp\
                                                  [internalCellIndex]->subBandToRBGMapPeriodic; 
    rat0Info_p = &(dlRAInfo_gp[internalCellIndex] + 
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info;
    rat1Info_p = &(dlRAInfo_gp[internalCellIndex] + 
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat1Info;
    /* CA Changes end */ 
    /* -CQI_5.0 */

    dlCQIInfo_p->isWideBandReportAvailable = 1;
    /* SPR 8175 changes start */
    dlCQIInfo_p->riUpdationFlag = IMMEDIATE_DL_RI_UPDATE;
    /* SPR 8175 changes end */
    /* + CQI_4.1 */
    /* Number Of Subband */
    subBandCount = aperiodicCQIMode12_p->numberOfSubbands;

    /* SPR 4769 changes start */
    /* Time Averaging */
    /* SPR 9427 changes start */
    dlCQIInfo_p->previousTimeAveragedWBCQICW0 = calculateAverageDLCQIforAP(
            /* SPR 9427 changes end */
            aperiodicCQIMode12_p->widebandCQIValueCodeword0,
            dlCQIInfo_p->previousTimeAveragedWBCQICW0);

    /* Correcting wideband CQI and  MCS for CW1 from BLER */
    dlCQIInfo_p->widebandCQICodeWordOne = getEffectiveDLCQI(
            dlCQIInfo_p->previousTimeAveragedWBCQICW0,
            dlCQIInfo_p->cqiCorrectionFactorForCW0,
            dlUEContext_p );

    dlCQIInfo_p->mcsIndexCodeWordOne = 
        getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordOne
                     );
    /* SPR 4769 changes end */
    /*SPR 6424 Fix Start*/

    /* Cyclomatic_complexity_changes_start */
    getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordOne,
            &(dlCQIInfo_p->modulationSchemeCodeWordOne));
    /* Cyclomatic_complexity_changes_end */

    /*SPR 6424 Fix End*/

    /* ICIC chnages start */
    /* CA Changes start */ 
    /* CA_phase2_csi_code Start */
    if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
    {
        updateDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                internalCellIndex);
    }
    else
    {
        macUupdateSCellDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                /* +- SPR 17777 */
                scellArrIndex);
    }
    /* CA_phase2_csi_code End */
    /* CA Changes end */ 
    /* ICIC changes end */

    if (0 != aperiodicCQIMode12_p->widebandCQIValueCodeword1)
    {
        /* SPR 4769 changes start */
        /* Time Averaging */
        /* SPR 9427 changes start */
        dlCQIInfo_p->previousTimeAveragedWBCQICW1 = calculateAverageDLCQIforAP(
                /* SPR 9427 changes start */
                aperiodicCQIMode12_p->widebandCQIValueCodeword1,
                dlCQIInfo_p->previousTimeAveragedWBCQICW1);

        /* Correcting wideband CQI and MCS for CW1 from BLER */
        dlCQIInfo_p->widebandCQICodeWordTwo = getEffectiveDLCQI(
                dlCQIInfo_p->previousTimeAveragedWBCQICW1,
                dlCQIInfo_p->cqiCorrectionFactorForCW1,
                dlUEContext_p );

        dlCQIInfo_p->mcsIndexCodeWordTwo = 
            getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordTwo
                          );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start*/

        /* Cyclomatic_complexity_changes_start */
        getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordTwo,
                &(dlCQIInfo_p->modulationSchemeCodeWordTwo));
        /* Cyclomatic_complexity_changes_end */

        /*SPR 6424 Fix End*/
        /* SPR 13855 fix start */
        if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
        {
            updateDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo,
                    internalCellIndex);
        }
        else
        {
            macUpdateSCellDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo,
                /* +- SPR 17777 */
                    scellArrIndex);
        }
        /* SPR 13855 fix end */

        LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                getCurrentTick(),dlUEContext_p->ueIndex,
                dlCQIInfo_p->widebandCQICodeWordOne,
                dlCQIInfo_p->mcsIndexCodeWordOne,
                dlCQIInfo_p->widebandCQICodeWordTwo,
                dlCQIInfo_p->mcsIndexCodeWordTwo,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,__func__,"WB_BOTH_CW");

    }
    else
    {
        LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                getCurrentTick(),dlUEContext_p->ueIndex,
                dlCQIInfo_p->widebandCQICodeWordOne,
                dlCQIInfo_p->mcsIndexCodeWordOne,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,__func__,"WB_CW0");
    }    

    dlCQIInfo_p->isSubBandReportAvailable = 0;
    /* CA_phase2_csi_code Start */
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        dlUEContext_p->aperiodicMode12Rcvd = TRUE;
    }
    else
    {
        dlUEContext_p->dlUeScellContext_p[scellArrIndex]->aperiodicMode12Rcvd = TRUE;
    }

    /* + TM7_8 Changes Start */
    if(!(TX_MODE_8 == transmissionMode))
        /* CA_phase2_csi_code End */
    {
        /* - TM7_8 Changes End */
        subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;

        /*  subBandCount value denotes the subband number */
        pmiInfo12_p = &aperiodicCQIMode12_p->pmiInfo12[subBandCount - 1];

        subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[subBandCount - 1]; 

        /* + CQI_5.0 */
        cqiInfo_p = &subBandReportInfo_p->cqiInfo;
        /* - CQI_5.0 */

        while (subBandCount--)
        {
            /* 4x4 DL MIMO CHG START */
            /* CA_phase2_csi_code start */
            /* +- SPR 17777 */
            if (isPMIInCodebookSubsetRestriction(
                        cbsrValue
#ifdef CSI_VALIDATION
                        ,*pmiInfo12_p,
                        aperiodicCQIMode12_p->receivedRI,
                        dlUEContext_p->ueCategory,
                        transmissionMode,
                        cellSpecificParams_g.cellConfigAndInitParams_p\
                        [internalCellIndex]->cellParams_p->numOfTxAnteenas
#endif
                        ))
                /* +- SPR 17777 */
            {
                subBandToCQInfo_p->preCodingMatrixIndex = *pmiInfo12_p;
            }
            /*KLOCKWORK_CHANGES_START_4*/
#ifdef CSI_VALIDATION
            /*KLOCKWORK_CHANGES_STOP_4*/
            else
            {
                LOG_MAC_MSG(MAC_RECEIVED_PMI_LOG_ID,LOGERROR,MAC_L1_INF,\
                        getCurrentTick(),dlUEContext_p->ueCategory,\
                        transmissionMode,\
                        aperiodicCQIMode12_p->receivedRI,\
                        *pmiInfo12_p,\
                        DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                        DEFAULT_FLOAT_VALUE,__func__,\
                        "For Mode12, Wrong Aperiodic Sub Band PMI Received");
            }
            /* CA_phase2_csi_code end */
            /*KLOCKWORK_CHANGES_START_4*/
#endif
            /*KLOCKWORK_CHANGES_STOP_4*/
            /* 4x4 DL MIMO CHG END */
            /* + CQI_5.0 */
            /* As aperiodic mode 12 is wideband mode, but PMI received
             * for each SB, so all the RBG/PRBs should be initialized
             * with corresponding received subband PMI */
            /* CA Changes start */ 
            startRBGIndex =
                subBandToRBGMapPeriodic_p[subBandCount].startRBGIndex;
            endRBGIndex =
                subBandToRBGMapPeriodic_p[subBandCount].endRBGIndex;
            /* CA Changes end */ 
            rbgIndexCount = endRBGIndex - startRBGIndex + 1;
            while(rbgIndexCount--)
            {
                if(1 == *dlRI_p)
                {
                    updateRAT0CW1SpatialMulSubBandInfo(
                            startRBGIndex,
                            INVALID_CQI_VALUE,
                            subBandToCQInfo_p->preCodingMatrixIndex,
                            &cqiInfo_p->cqiInfoSpatialMul,
                            /* CA Changes start */
                            numOfTxAnteenas);
                    /* CA Changes end */

                    updateRAT1CW1SpatialMulSubBandInfo(
                            startRBGIndex,
                            INVALID_CQI_VALUE,
                            subBandToCQInfo_p->preCodingMatrixIndex,
                            &cqiInfo_p->cqiInfoSpatialMul,
                            rat0Info_p,
                            rat1Info_p,
                            /* CA Changes start */ 
                            numOfTxAnteenas);
                    /* CA Changes end */ 
                }
                else if(2 == *dlRI_p)
                {
                    updateRAT0CW2SpatialMulSubBandInfo(
                            startRBGIndex,
                            INVALID_CQI_VALUE,
                            subBandToCQInfo_p->preCodingMatrixIndex,
                            &cqiInfo_p->cqiInfoSpatialMul,
                            /* CA Changes start */ 
                            numOfTxAnteenas);
                    /* CA Changes end */ 

                    updateRAT1CW2SpatialMulSubBandInfo(
                            startRBGIndex,
                            INVALID_CQI_VALUE,
                            subBandToCQInfo_p->preCodingMatrixIndex,
                            &cqiInfo_p->cqiInfoSpatialMul,
                            rat0Info_p,
                            rat1Info_p,
                            /* CA Changes start */ 
                            numOfTxAnteenas);
                    /* CA Changes end */ 
                }
                startRBGIndex++;
            }
            subBandToCQInfo_p--;
            pmiInfo12_p--;
        }
        /* - CQI_5.0 */
        /* + TM7_8 Changes Start */
    }
  }
    /* - TM7_8 Changes End */
    return;
}
/* 4x4 DL MIMO CHG START */
/*Rel 5.3.1 cyclomatic chg start*/ 
/*****************************************************************************
 * Function Name  : checkPmiInCbsrForSubband
 * Inputs         : 1)periodicCQIMode22_p - This holds pointer to 
 *                  AperiodicCQIMode22 structure.
 *                  2)dlUEContext_p - This holds pointer to DLUEContext
 *                    structure.
 *                  3)subbandPMIValue_p - This holds pointer to subband pmi
 *                  4)subBandToCQInfo_p - This holds pointer to SubBandToCQInfo   
 *                  5)internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function checks the pmi value in cbsr for subband                  
 *****************************************************************************/
/* +- SPR 17777 */
STATIC void checkPmiInCbsrForSubband(UInt32 *subbandPMIValue_p,
#ifdef CSI_VALIDATION
        AperiodicCQIMode22 *aperiodicCQIMode22_p,
#endif
/* +- SPR 17777 */
                         DLUEContext *dlUEContext_p,SubBandToCQInfo *subBandToCQInfo_p,
                         InternalCellIndex internalCellIndex)
{
    /* CA_phase2_csi_code Start */
    /** Coverity 67177_67178 Fix : START **/
/* +- SPR 17777 */
#ifdef CSI_VALIDATION
    TransmissonMode transmissionMode = INVALID_TX_MODE;
#endif
/* +- SPR 17777 */
    UInt8 *cbsrValue = PNULL;
    /** Coverity 67177_67178 Fix : END **/
    UInt8 scellArrIndex = INVALID_8_VAL;
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {

/* +- SPR 17777 */
#ifdef CSI_VALIDATION
        transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
#endif
/* +- SPR 17777 */
        cbsrValue =  dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue;
    }
    else
    {
        scellArrIndex =  getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
        /*klockwork warning fix*/
        if(scellArrIndex <= MAX_NUM_SCELL)
        { 
/* +- SPR 17777 */
#ifdef CSI_VALIDATION
            transmissionMode =  dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                ScelldlMIMOInfo.transmissionMode;
#endif
/* +- SPR 17777 */
            cbsrValue = dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                codebookSubsetRestrictionv1020.cbsrValue;
        }
        /** Coverity 67178_67177 Fix Start**/
        else
        {
            return;
        }
        /** Coverity 67178_67177 Fix End**/
    }
    /* +- SPR 17777 */
    if (isPMIInCodebookSubsetRestriction(
                cbsrValue
#ifdef CSI_VALIDATION
                ,*subbandPMIValue_p,
                                         aperiodicCQIMode22_p->receivedRI,
                                         dlUEContext_p->ueCategory,
                                         transmissionMode,
                                         cellSpecificParams_g.cellConfigAndInitParams_p\
                [internalCellIndex]->cellParams_p->numOfTxAnteenas
#endif
                ))
        /* +- SPR 17777 */
    {
        subBandToCQInfo_p->preCodingMatrixIndex = *subbandPMIValue_p;
    }
/*KLOCKWORK_CHANGES_START_4*/
#ifdef CSI_VALIDATION
/*KLOCKWORK_CHANGES_STOP_4*/
    else
    {
        *subbandPMIValue_p = subBandToCQInfo_p->preCodingMatrixIndex;

        LOG_MAC_MSG(MAC_RECEIVED_PMI_LOG_ID,LOGERROR,MAC_L1_INF,\
               getCurrentTick(),dlUEContext_p->ueCategory,\
               transmissionMode,\
               aperiodicCQIMode22_p->receivedRI,\
               aperiodicCQIMode22_p->subbandPMIValue,\
               DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
               DEFAULT_FLOAT_VALUE,__func__,\
               "For Mode22, Wrong Aperiodic Sub-Band PMI Received");
    }
    /* CA_phase2_csi_code End */
    TransmissonMode transmissionMode = INVALID_TX_MODE;
    UInt8 *cbsrValue = PNULL;
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {

        transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
        cbsrValue =  dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue;
    }/*klockwork warning fix*/
    else if(scellArrIndex <= MAX_NUM_SCELL)
    {
        transmissionMode =  dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
            ScelldlMIMOInfo.transmissionMode;
        cbsrValue = dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
            codebookSubsetRestrictionv1020.cbsrValue;
    }
/*KLOCKWORK_CHANGES_START_4*/
#endif
/*KLOCKWORK_CHANGES_STOP_4*/
    return;
}

/*+EICIC .*/
/*****************************************************************************
 * Function Name  : checkPmiInCbsrForSubband_2
 * Inputs         : 1)periodicCQIMode22_p - This holds pointer to 
 *                  AperiodicCQIMode22 structure.
 *                  2)dlUEContext_p - This holds pointer to DLUEContext
 *                    structure.
 *                  3)subbandPMIValue_p - This holds pointer to subband pmi
 *                  4)subBandToCQInfo_p - This holds pointer to SubBandToCQInfo   
 *                  5)internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function checks the pmi value in cbsr for subband                  
 *****************************************************************************/
STATIC void
checkPmiInCbsrForSubband_2 (UInt32 * subbandPMIValue_p,
        /* +- SPR 17777 */
#ifdef CSI_VALIDATION
       AperiodicCQIMode22 * aperiodicCQIMode22_p,
#endif
        /* +- SPR 17777 */
       DLUEContext * dlUEContext_p,
       SubBandToCQInfo * subBandToCQInfo_p,
       InternalCellIndex internalCellIndex)
{
    /* CA_phase2_csi_code Start */
    /** Coverity 67177_67178 Fix : START **/
    /* +- SPR 17777 */
#ifdef CSI_VALIDATION
    TransmissonMode transmissionMode = INVALID_TX_MODE;
#endif
    /* +- SPR 17777 */
    UInt8 *cbsrValue = PNULL;
    /** Coverity 67177_67178 Fix : END **/
    UInt8 scellArrIndex = INVALID_8_VAL;
    if (dlUEContext_p->internalCellIndex == internalCellIndex)
    {

    /* +- SPR 17777 */
#ifdef CSI_VALIDATION
        transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
#endif
    /* +- SPR 17777 */
        cbsrValue = dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue;
    }
    else
    {
        scellArrIndex = getServeCellIndexFromInternalCellId (dlUEContext_p,
                internalCellIndex);
        /*klockwork warning fix*/
        if(scellArrIndex <= MAX_NUM_SCELL)
        {
            /* +- SPR 17777 */
#ifdef CSI_VALIDATION
            transmissionMode =  dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                ScelldlMIMOInfo.transmissionMode;
#endif
            /* +- SPR 17777 */
            cbsrValue = dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                codebookSubsetRestrictionv1020.cbsrValue;
        }
    }
    /* +- SPR 17777 */
    if (isPMIInCodebookSubsetRestriction (
                cbsrValue
#ifdef CSI_VALIDATION
                ,*subbandPMIValue_p,
                aperiodicCQIMode22_p->receivedRI,
                dlUEContext_p->ueCategory,
                transmissionMode,
                cellSpecificParams_g.
                cellConfigAndInitParams_p
                [internalCellIndex]->cellParams_p->
                numOfTxAnteenas
#endif
                ))
        /* +- SPR 17777 */
    {
        subBandToCQInfo_p->preCodingMatrixIndex_2 = *subbandPMIValue_p;
    }
    /*KLOCKWORK_CHANGES_START_4*/
#ifdef CSI_VALIDATION
    /*KLOCKWORK_CHANGES_STOP_4*/
    else
    {
        *subbandPMIValue_p = subBandToCQInfo_p->preCodingMatrixIndex;

        LOG_MAC_MSG (MAC_RECEIVED_PMI_LOG_ID, LOGERROR, MAC_L1_INF,
                getCurrentTick (), dlUEContext_p->ueCategory,
                transmissionMode,
                aperiodicCQIMode22_p->receivedRI,
                aperiodicCQIMode22_p->subbandPMIValue,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE, __func__,
                "For Mode22 for ABS, Wrong Aperiodic Sub-Band PMI Received");
    }
    /* CA_phase2_csi_code End */
    TransmissonMode transmissionMode = INVALID_TX_MODE;
    UInt8 *cbsrValue = PNULL;
    if (dlUEContext_p->internalCellIndex == internalCellIndex)
    {

        transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
        cbsrValue = dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue;
    }
    else if(scellArrIndex <= MAX_NUM_SCELL)
    {
        transmissionMode =
            dlUEContext_p->dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo.
            transmissionMode;
        cbsrValue =
            dlUEContext_p->
            dlUeScellContext_p[scellArrIndex]->codebookSubsetRestrictionv1020.
            cbsrValue;
    }
    /*KLOCKWORK_CHANGES_START_4*/
#endif
    /*KLOCKWORK_CHANGES_STOP_4*/
    return;
}

/*-EICIC .*/
/*****************************************************************************
 * Function Name  : checkPmiInCbsrForWideband
 * Inputs         : 1)periodicCQIMode22_p - This holds pointer to 
 *                  AperiodicCQIMode22 structure.
 *                  2)dlUEContext_p - This holds pointer to DLUEContext
 *                    structure.
 *                  3)dlCQIInfo_p - This holds pointer to DLCQIInfo  
 *                  4)internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function check the Pmi value in cbsr for wide band
 *     
 *****************************************************************************/
STATIC void checkPmiInCbsrForWideband(AperiodicCQIMode22 *aperiodicCQIMode22_p,
                                      DLUEContext *dlUEContext_p,DLCQIInfo *dlCQIInfo_p,
                                      InternalCellIndex internalCellIndex)
{
    /* CA_phase2_csi_code Start */
    /** Coverity 67180_67179_67178_67177 Fix : START **/
    /* +- SPR 17777 */
#ifdef CSI_VALIDATION
    TransmissonMode transmissionMode = INVALID_TX_MODE;
#endif
    /* +- SPR 17777 */
    UInt8 *cbsrValue = PNULL;
    /** Coverity 67180_67179_67178_67177 Fix : END **/
    UInt8 scellArrIndex = INVALID_8_VAL;
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {

        /* +- SPR 17777 */
#ifdef CSI_VALIDATION
        transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
#endif
        /* +- SPR 17777 */
        cbsrValue =  dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue;
    }
    else
    {
        scellArrIndex =  getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
        /*klockwork warning fix*/
        if(scellArrIndex <= MAX_NUM_SCELL)
        {
            /* +- SPR 17777 */
#ifdef CSI_VALIDATION
            transmissionMode =  dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                ScelldlMIMOInfo.transmissionMode;
#endif
            /* +- SPR 17777 */
            cbsrValue = dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                codebookSubsetRestrictionv1020.cbsrValue;
        }
        else
        {
            return;
        }
        /** Coverity 67180_67179 Fix End**/
    }

    /* +- SPR 17777 */
    if(isPMIInCodebookSubsetRestriction(
                cbsrValue
#ifdef CSI_VALIDATION
                ,aperiodicCQIMode22_p->widebandPMIValue,
                                        aperiodicCQIMode22_p->receivedRI,
                                        dlUEContext_p->ueCategory,
                                        transmissionMode,
                                        cellSpecificParams_g.cellConfigAndInitParams_p\
                [internalCellIndex]->cellParams_p->numOfTxAnteenas
#endif
                ))
        /* +- SPR 17777 */
    {
        dlCQIInfo_p->wideBandPMIIndex = aperiodicCQIMode22_p->widebandPMIValue;
    }
/*KLOCKWORK_CHANGES_START_4*/
#ifdef CSI_VALIDATION
/*KLOCKWORK_CHANGES_STOP_4*/
    else
    {
        /* Wrong PMI received */
        LOG_MAC_MSG(MAC_RECEIVED_PMI_LOG_ID,LOGERROR,MAC_L1_INF,\
                getCurrentTick(),dlUEContext_p->ueCategory,\
                transmissionMode,\
                aperiodicCQIMode22_p->receivedRI,\
                aperiodicCQIMode22_p->widebandPMIValue,\
                DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,__func__,\
                "Wrong Aperiodic PMI Received for Mode30");
    }
    /* CA_phase2_csi_code end */
/*KLOCKWORK_CHANGES_START_4*/
#endif
/*KLOCKWORK_CHANGES_STOP_4*/
    return;
}    
/*Rel 5.3.1 cyclomatic chg end*/ 

/*Rel 5.3.1 cyclomatic chg end*/
/*+EICIC .*/
/*****************************************************************************
 * Function Name  : checkPmiInCbsrForWideband_2
 * Inputs         : 1)periodicCQIMode22_p - This holds pointer to 
 *                  AperiodicCQIMode22 structure.
 *                  2)dlUEContext_p - This holds pointer to DLUEContext
 *                    structure.
 *                  3)dlCQIInfo_p - This holds pointer to DLCQIInfo  
 *                  4)internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function check the Pmi value in cbsr for wide band
 *     
 *****************************************************************************/
STATIC void checkPmiInCbsrForWideband_2 (AperiodicCQIMode22 * aperiodicCQIMode22_p,
                                         DLUEContext * dlUEContext_p,
                                         DLCQIInfo * dlCQIInfo_p,
                                         InternalCellIndex internalCellIndex)
{
    /* CA_phase2_csi_code Start */
    /** Coverity 67180_67179_67178_67177 Fix : START **/
    /* +- SPR 17777 */
#ifdef CSI_VALIDATION
    TransmissonMode transmissionMode = INVALID_TX_MODE;
#endif
    /* +- SPR 17777 */
    UInt8 *cbsrValue = PNULL;
    /** Coverity 67180_67179_67178_67177 Fix : END **/
    UInt8 scellArrIndex = INVALID_8_VAL;
    if (dlUEContext_p->internalCellIndex == internalCellIndex)
    {

        /* +- SPR 17777 */
#ifdef CSI_VALIDATION
        transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
#endif
        /* +- SPR 17777 */
        cbsrValue = dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue;
    }
    else
    {
        scellArrIndex = getServeCellIndexFromInternalCellId (dlUEContext_p,
                internalCellIndex);
        /*klockwork warning fix*/
        if(scellArrIndex <= MAX_NUM_SCELL)
        {
            /* +- SPR 17777 */
#ifdef CSI_VALIDATION
            transmissionMode =  dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                ScelldlMIMOInfo.transmissionMode;
#endif
            /* +- SPR 17777 */
            cbsrValue = dlUEContext_p->dlUeScellContext_p[scellArrIndex]->
                codebookSubsetRestrictionv1020.cbsrValue;
        }
    }

    if (isPMIInCodebookSubsetRestriction
            /* +- SPR 17777 */
            (
             cbsrValue
#ifdef CSI_VALIDATION
             ,aperiodicCQIMode22_p->widebandPMIValue,
             aperiodicCQIMode22_p->receivedRI, dlUEContext_p->ueCategory,
             transmissionMode,
             cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
             cellParams_p->numOfTxAnteenas
#endif
             ))
        /* +- SPR 17777 */
    {
        dlCQIInfo_p->wideBandPMIIndex_2 =
            aperiodicCQIMode22_p->widebandPMIValue;
    }
    /*KLOCKWORK_CHANGES_START_4*/
#ifdef CSI_VALIDATION
    /*KLOCKWORK_CHANGES_STOP_4*/
    else
    {
        /* Wrong PMI received */
        LOG_MAC_MSG (MAC_RECEIVED_PMI_LOG_ID, LOGERROR, MAC_L1_INF,
                getCurrentTick (), dlUEContext_p->ueCategory,
                transmissionMode,
                aperiodicCQIMode22_p->receivedRI,
                aperiodicCQIMode22_p->widebandPMIValue_2,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE, __func__,
                "Wrong Aperiodic PMI Received for Mode30For ABS");
    }
    /* CA_phase2_csi_code end */
    /*KLOCKWORK_CHANGES_START_4*/
#endif
    /*KLOCKWORK_CHANGES_STOP_4*/
    return;
}

/*Rel 5.3.1 cyclomatic chg end*/
/*-EICIC .*/
/*****************************************************************************
 * Function Name  : processCQIMode22
 * Inputs         : periodicCQIMode22_p - This holds pointer to 
 *                                        AperiodicCQIMode22 structure,
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the CQIInfoSpatialMul structure
 *                  for CodeWord One and CodeWord Two.
 *****************************************************************************/
/* CA Changes start */
STATIC void processCQIMode22(
        AperiodicCQIMode22 *aperiodicCQIMode22_p,
        DLUEContext *dlUEContext_p,
        InternalCellIndex internalCellIndex)
/* CA Changes end */
{
    DLCQIInfo *dlCQIInfo_p = PNULL;
    SubBandReportInfo *subBandReportInfo_p = PNULL;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    UInt8 *subbandInfo22_p = PNULL;
    UInt32 subBandCount = 0; 
    CQIInfo *cqiInfo_p = PNULL;
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;
    UInt32 subbandPMIValue = 0;
    UInt32 startRBGIndex = 0;
    UInt32 endRBGIndex = 0;
    UInt32 rbgIndexCount = 0;
    /* + CQI_4.1 */
    UInt8 subbandNum = 0;
    UInt8 correctedSubbandCQICW0 = 0;
    UInt8 correctedSubbandCQICW1 = 0;
    UInt8 correctedSubbandMCSCW0 = 0;
    UInt8 correctedSubbandMCSCW1 = 0;
    UInt8 oldCqiForRBGCW0 = 0;
    UInt8 oldCqiForRBGCW1 = 0;
    UInt8 toProcessCW2 = FALSE;
    UInt8 scellArrIndex = INVALID_8_VAL;
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    /* - CQI_4.1 */
    /* CA Changes start */
    UInt8 numOfTxAnteenas =  cellSpecificParams_g.cellConfigAndInitParams_p\
                             [internalCellIndex]->cellParams_p->numOfTxAnteenas;
    SubBandToRBGMap *subBandToRBGMapAperiodic_p =  cqiGlobalParams_gp\
                                                   [internalCellIndex]->subBandToRBGMapAperiodic; 
    rat0Info_p = &(dlRAInfo_gp[internalCellIndex] +
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info;
    rat1Info_p = &(dlRAInfo_gp[internalCellIndex] + 
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat1Info;
    /* CA Changes end */
    /* CA_phase2_csi_code Start */
    DLMIMOInfo *dlMimoInfo_p;
    UInt8 *isSubbandPMIAvailable_p;
    /* CA_phase2_csi_code End */
    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;

    }
    /* CA_phase2_csi_code Start */
    /*+EICIC . */
    CHECK_APERIODIC_SECONDARY_CONDITION (ulUEContext_p, internalCellIndex)
    {    /* ABS Criteria */
        if (dlUEContext_p->internalCellIndex == internalCellIndex)
        {
            /* Pcell */
            dlMimoInfo_p = &(dlUEContext_p->dlMIMOInfo);
            isSubbandPMIAvailable_p = &(dlUEContext_p->isSubbandPMIAvailable);
        }
        else
        {
            scellArrIndex = getServeCellIndexFromInternalCellId (dlUEContext_p,
                    internalCellIndex);

            /*klockwork warning fix*/
            if(scellArrIndex <= MAX_NUM_SCELL)
            {
                dlMimoInfo_p = &(dlUEContext_p->dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo);
                isSubbandPMIAvailable_p = &(dlUEContext_p->dlUeScellContext_p[scellArrIndex]->isSubbandPMIAvailable);
            }
            else
            {
                return;
            }
        }
        /* CA_phase2_csi_code End */
        dlCQIInfo_p->isWideBandReportAvailable_2 = 1;
        /* + CQI_5.0 */
        UInt32 apMode22SubBandCount = 0;
        /* SPR 8175 changes start */
        dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
        /* SPR 8175 changes end */

        /* Updating RI in DL context */
        dlMimoInfo_p->dlRi_2 = aperiodicCQIMode22_p->receivedRI;
        /* - CQI_5.0 */

        /* SPR 4769 changes start */
        /* Time Averaging */
        /* SPR 9427 changes start */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0_2 = calculateAverageDLCQIforAP (
                /* SPR 9427 changes end */
                aperiodicCQIMode22_p->
                widebandCQIValueCodeword0,
                dlCQIInfo_p->
                previousTimeAveragedWBCQICW0_2);

        /* Correcting wideband CQI and MCS for CW1 from BLER */
        dlCQIInfo_p->widebandCQICodeWordOne_2 =
            getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW0_2,
                    dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                    dlUEContext_p);

        dlCQIInfo_p->mcsIndexCodeWordOne_2 =
            getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordOne_2
                          );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start */
        getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordOne_2,
                &(dlCQIInfo_p->modulationSchemeCodeWordOne_2));
        /*SPR 6424 Fix End */

        /* ICIC chnages start */
        /* CA Changes start */
        /* CA_phase2_csi_code Start */
        if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
        {
            updateDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex, scellArrIndex);
        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */
        /* ICIC changes end */

        /* SS_S2 */
        if (0 != aperiodicCQIMode22_p->widebandCQIValueCodeword1)
        {
            /* SPR 4769 changes start */
            /* Time Averaging */
            /* SPR 9427 changes start */
            dlCQIInfo_p->previousTimeAveragedWBCQICW1_2 =
                calculateAverageDLCQIforAP (
                        /* SPR 9427 changes start */
                        aperiodicCQIMode22_p->
                        widebandCQIValueCodeword1,
                        dlCQIInfo_p->
                        previousTimeAveragedWBCQICW1_2);

            /* Correcting wideband CQI and MCS for CW2 from BLER */
            dlCQIInfo_p->widebandCQICodeWordTwo_2 =
                getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW1_2,
                        dlCQIInfo_p->cqiCorrectionFactorForCW1_2,
                        dlUEContext_p);

            dlCQIInfo_p->mcsIndexCodeWordTwo_2 =
                getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordTwo_2
                              );
            /* SPR 4769 changes end */

            /*SPR 6424 Fix Start */
            getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordTwo_2,
                    &(dlCQIInfo_p->
                        modulationSchemeCodeWordTwo_2));
            /*SPR 6424 Fix End */
            /* ICIC chnages start */
            /* CA Changes start */
            /* CA_phase2_csi_code Start */
            if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
            {
                updateDLSINRCW1_2 (dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordTwo_2,
                        internalCellIndex);
            }
            else
            {
                macUpdateSCellDLSINRCW1_2 (dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordTwo_2,
                        internalCellIndex, scellArrIndex);
            }
            /* CA_phase2_csi_code End */
            /* CA Changes end */
            /* ICIC changes end */

            LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                    getCurrentTick (), dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    dlCQIInfo_p->mcsIndexCodeWordOne_2,
                    dlCQIInfo_p->widebandCQICodeWordTwo_2,
                    dlCQIInfo_p->mcsIndexCodeWordTwo_2,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__,
                    "WB_BOTH_CW_FOR_ABS");

            toProcessCW2 = TRUE;
        }
        else
        {
            LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                    getCurrentTick (), dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    dlCQIInfo_p->mcsIndexCodeWordOne_2, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE, __func__, "WB_CW0_ABS");

        }

        /* + TM7_8 Changes Start */
        /* In case of TM8, wideBandPMIIndex is filled from higher layer precoding value 
         * i.e. precodingIndex instead of latest reported PMI value from UE */
        if (TX_MODE_8 == dlMimoInfo_p->transmissionMode)
        {
            dlCQIInfo_p->wideBandPMIIndex_2 = dlMimoInfo_p->precodingIndex;
        }
        else
        {
            /* 4x4 DL MIMO CHG START */
            checkPmiInCbsrForWideband_2 (aperiodicCQIMode22_p, dlUEContext_p,
                    dlCQIInfo_p, internalCellIndex);
            /* 4x4 DL MIMO CHG END */
        }
        /* - TM7_8 Changes End */
        /* - CQI_4.1 */

        /* + CQI_5.0 */
        /* CA Changes start */
        if (FREQUENCY_SELECTIVE_ON ==
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
                cellParams_p->freqSelectiveSchDL)
            /* CA Changes end */
        {
            /*Get the number of SubBand Count for which the report has came */
            subBandCount = aperiodicCQIMode22_p->numberOfSubbands;
            dlCQIInfo_p->isSubBandReportAvailable_2 = 1;
            subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;
            cqiInfo_p = &subBandReportInfo_p->cqiInfo;
            subbandInfo22_p = &aperiodicCQIMode22_p->subbandInfo22[0];
            /* + TM7_8 Changes Start */
            if (TX_MODE_8 == dlMimoInfo_p->transmissionMode)
            {
                subbandPMIValue = INVALID_PMI_VALUE;
            }
            else
            {
                subbandPMIValue = aperiodicCQIMode22_p->subbandPMIValue;
            }
            /* - TM7_8 Changes End */


            SubbandPMIInfo tempSubbandPMIInfo;
            memSet(&tempSubbandPMIInfo,0,sizeof(SubbandPMIInfo));
            cqiInfo_p->cqiInfoSpatialMul.pmiInfo = tempSubbandPMIInfo;

            /* This functionality is added so that subbands for which subband PMI 
             * is not received, should be initialized with wideband PMI */
            /* + TM7_8 Changes Start */
            if (TX_MODE_8 == dlMimoInfo_p->transmissionMode)
            {
                *isSubbandPMIAvailable_p = FALSE;
            }
            else
            {
                *isSubbandPMIAvailable_p = TRUE;
            }
            /* - TM7_8 Changes End */

            /* CA Changes start */
            apMode22SubBandCount = cellSpecificParams_g.cellConfigAndInitParams_p
                [internalCellIndex]->cellParams_p->ueSelectedN;
            /* CA Changes end */
            while (apMode22SubBandCount--)
            {
                startRBGIndex =
                    subBandToRBGMapAperiodic_p[apMode22SubBandCount].startRBGIndex;
                endRBGIndex =
                    subBandToRBGMapAperiodic_p[apMode22SubBandCount].endRBGIndex;
                rbgIndexCount = endRBGIndex - startRBGIndex + 1;
                while (rbgIndexCount--)
                {
                    /* EICIC_Coverity CID 69337 Start */
                    if (1 == dlMimoInfo_p->dlRi_2)
                        /* EICIC_Coverity CID 69337 End */ 	
                    {
                        updateRAT0CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                                INVALID_CQI_VALUE,
                                dlCQIInfo_p->
                                wideBandPMIIndex,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */


                        updateRAT1CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                                INVALID_CQI_VALUE,
                                dlCQIInfo_p->
                                wideBandPMIIndex,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */
                    }

                    else if (2 == dlMimoInfo_p->dlRi_2)
                    {
                        updateRAT0CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                                INVALID_CQI_VALUE,
                                dlCQIInfo_p->
                                wideBandPMIIndex,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */

                        updateRAT1CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                                INVALID_CQI_VALUE,
                                dlCQIInfo_p->
                                wideBandPMIIndex,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */
                    }
                    startRBGIndex++;
                }
            }

            while (subBandCount--)
            {
                /* + CQI_4.1 */
                subbandNum = subbandInfo22_p[subBandCount];

                startRBGIndex =
                    subBandToRBGMapAperiodic_p[subbandNum].startRBGIndex;

                endRBGIndex = subBandToRBGMapAperiodic_p[subbandNum].endRBGIndex;
                rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                subBandToCQInfo_p =
                    &subBandReportInfo_p->subBandToCQInfo[startRBGIndex];
                /* 4x4 DL MIMO CHG START */
                checkPmiInCbsrForSubband_2 (&subbandPMIValue,
                        /* +- SPR 17777 */
#ifdef CSI_VALIDATION
                        aperiodicCQIMode22_p,
#endif
                        dlUEContext_p,
                        /* +- SPR 17777 */
                        subBandToCQInfo_p, internalCellIndex);
                /* 4x4 DL MIMO CHG END */
                oldCqiForRBGCW0 = subBandToCQInfo_p->cqiIndexCW1_2;
                oldCqiForRBGCW1 = subBandToCQInfo_p->cqiIndexCW2_2;

                /* SPR 4769 changes start */
                /* Time Averaging */
                /* SPR 9427 changes start */
                subBandToCQInfo_p->previousTimeAveragedSBCQICW0 =
                    calculateAverageDLCQIforAP (
                            /* SPR 9427 changes start */
                            aperiodicCQIMode22_p->
                            cqiValueCodeword0,
                            subBandToCQInfo_p->
                            previousTimeAveragedSBCQICW0);

                /* Correcting Subband cqi for CW0 from BLER */
                subBandToCQInfo_p->cqiIndexCW1 =
                    getEffectiveDLCQI (subBandToCQInfo_p->
                            previousTimeAveragedSBCQICW0,
                            dlCQIInfo_p->cqiCorrectionFactorForCW0,
                            dlUEContext_p);

                /* Average Subband CQI and MCS calculation for CW0 */
                correctedSubbandCQICW0 = subBandToCQInfo_p->cqiIndexCW1_2;
                correctedSubbandMCSCW0 = getMCSFromCQI (correctedSubbandCQICW0
                                                        );
                /* SPR 4769 changes end */
                /*SPR 6424 Fix Start */
                getModSchemeFromMcsIndex (correctedSubbandMCSCW0,
                        &(dlCQIInfo_p->
                            modulationSchemeCodeWordOne_2));
                /*SPR 6424 Fix End */
                /*Update the New CQI Index Information for CW0 */
                subBandToCQInfo_p->mcsIndexCW1 = correctedSubbandMCSCW0;

                if (TRUE == toProcessCW2)
                {
                    /* SPR 4769 changes start */
                    /* Time Averaging */
                    /* SPR 9427 changes start */
                    subBandToCQInfo_p->previousTimeAveragedSBCQICW1 =
                        calculateAverageDLCQIforAP (
                                /* SPR 9427 changes start */
                                aperiodicCQIMode22_p->
                                cqiValueCodeword1,
                                subBandToCQInfo_p->
                                previousTimeAveragedSBCQICW1);

                    /* Correcting Subband cqi for CW1 from BLER */
                    subBandToCQInfo_p->cqiIndexCW2 =
                        getEffectiveDLCQI (subBandToCQInfo_p->previousTimeAveragedSBCQICW1_2,
                                dlCQIInfo_p->cqiCorrectionFactorForCW1_2,
                                dlUEContext_p);

                    /* Average Subband CQI and MCS calculation for CW1 */
                    correctedSubbandCQICW1 = subBandToCQInfo_p->cqiIndexCW2;
                    correctedSubbandMCSCW1 =
                        getMCSFromCQI (correctedSubbandCQICW1
                                       );
                    /*SPR 6424 Fix Start */
                    getModSchemeFromMcsIndex (correctedSubbandMCSCW1,
                            &(dlCQIInfo_p->
                                modulationSchemeCodeWordTwo_2));
                    /*SPR 6424 Fix End */
                    /* SPR 4769 changes end */

                    /*Update the New MCS Index Information for CW1 */
                    subBandToCQInfo_p->mcsIndexCW2 = correctedSubbandMCSCW1;
                }

                while (rbgIndexCount--)
                {
                    /* SPR 3803 changes start */
                    if (oldCqiForRBGCW0)
                    {
                        /* SPR 3803 changes end */
                        resetRAT0CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                                oldCqiForRBGCW0,
                                &cqiInfo_p->
                                cqiInfoSpatialMul);

                        resetRAT1CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                                oldCqiForRBGCW0,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p);
                        /* SPR 3803 changes start */
                    }
                    /* SPR 3803 changes end */

                    if (1 == dlMimoInfo_p->dlRi_2)
                    {
                        /* RBG/PRBs should be initialized with received subband PMI */
                        updateRAT0CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQICW0,
                                subbandPMIValue,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */

                        updateRAT1CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQICW0,
                                subbandPMIValue,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */
                    }
                    else if (2 == dlMimoInfo_p->dlRi_2)
                    {
                        /* RBG/PRBs should be initialized with received subband PMI */
                        updateRAT0CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQICW0,
                                INVALID_PMI_VALUE,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */

                        updateRAT1CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQICW0,
                                INVALID_PMI_VALUE,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */
                    }

                    if (TRUE == toProcessCW2)
                    {
                        /* SPR 3803 changes start */
                        if (oldCqiForRBGCW1)
                        {
                            /* SPR 3803 changes end */
                            resetRAT0CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                                    oldCqiForRBGCW1,
                                    &cqiInfo_p->
                                    cqiInfoSpatialMul);

                            resetRAT1CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                                    oldCqiForRBGCW1,
                                    &cqiInfo_p->
                                    cqiInfoSpatialMul,
                                    rat0Info_p,
                                    rat1Info_p);
                            /* SPR 3803 changes start */
                        }
                        /* SPR 3803 changes end */

                        /* RBG/PRBs should be initialized with received 
                         * subband PMI */
                        updateRAT0CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQICW1,
                                subbandPMIValue,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */

                        updateRAT1CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQICW1,
                                subbandPMIValue,
                                &cqiInfo_p->
                                cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */
                    }
                    startRBGIndex++;
                    /* - CQI_4.1 */
                }
            }
        }
        ulUEContext_p->isDLWideBandCQIReportAvailable_2 = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne_2 =
            aperiodicCQIMode22_p->widebandCQIValueCodeword0;
        ulUEContext_p->dlWidebandCQICodeWordTwo_2 =
            aperiodicCQIMode22_p->widebandCQIValueCodeword0;
    }
    else
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne =
            aperiodicCQIMode22_p->widebandCQIValueCodeword0;
        ulUEContext_p->dlWidebandCQICodeWordTwo =
            aperiodicCQIMode22_p->widebandCQIValueCodeword0;
        /*-EICIC .*/
        if(dlUEContext_p->internalCellIndex == internalCellIndex)
        {
            /* Pcell */
            dlMimoInfo_p = &(dlUEContext_p->dlMIMOInfo);
            isSubbandPMIAvailable_p = &(dlUEContext_p->isSubbandPMIAvailable);
        }
        else
        {
            scellArrIndex =  getServeCellIndexFromInternalCellId(dlUEContext_p,
                    internalCellIndex);
            /*klockwork warning fix*/
            if(scellArrIndex <= MAX_NUM_SCELL)
            {
                dlMimoInfo_p = &(dlUEContext_p->dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo);
                isSubbandPMIAvailable_p = &(dlUEContext_p->dlUeScellContext_p[scellArrIndex]->isSubbandPMIAvailable);
            }
            else
            {
                return;
            }
        }
        /* CA_phase2_csi_code End */
        dlCQIInfo_p->isWideBandReportAvailable = 1;
        /* + CQI_5.0 */
        UInt32 apMode22SubBandCount = 0; 
        /* SPR 8175 changes start */
        dlCQIInfo_p->riUpdationFlag = IMMEDIATE_DL_RI_UPDATE;
        /* SPR 8175 changes end */

        /* Updating RI in DL context*/
        dlMimoInfo_p->dlRi = aperiodicCQIMode22_p->receivedRI; 
        /* - CQI_5.0 */

        /* SPR 4769 changes start */
        /* Time Averaging */
        /* SPR 9427 changes start */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0 =  calculateAverageDLCQIforAP(
                /* SPR 9427 changes end */
                aperiodicCQIMode22_p->widebandCQIValueCodeword0,
                dlCQIInfo_p->previousTimeAveragedWBCQICW0);

        /* Correcting wideband CQI and MCS for CW1 from BLER*/
        dlCQIInfo_p->widebandCQICodeWordOne = getEffectiveDLCQI(
                dlCQIInfo_p->previousTimeAveragedWBCQICW0,
                dlCQIInfo_p->cqiCorrectionFactorForCW0,
                dlUEContext_p );

        dlCQIInfo_p->mcsIndexCodeWordOne =
                        getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordOne
                                     );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start*/

        /* Cyclomatic_complexity_changes_start */
        getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordOne,
                &(dlCQIInfo_p->modulationSchemeCodeWordOne));
        /* Cyclomatic_complexity_changes_end */

        /*SPR 6424 Fix End*/

        /* ICIC chnages start */
        /* CA Changes start */
        /* CA_phase2_csi_code Start */
        if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
        {
            updateDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                    internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                    /* +- SPR 17777 */
                    scellArrIndex);
        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */
        /* ICIC changes end */

        /* SS_S2 */
        if (0 != aperiodicCQIMode22_p->widebandCQIValueCodeword1)
        {
            /* SPR 4769 changes start */
            /* Time Averaging */
            /* SPR 9427 changes start */
            dlCQIInfo_p->previousTimeAveragedWBCQICW1 = calculateAverageDLCQIforAP(
                    /* SPR 9427 changes start */
                    aperiodicCQIMode22_p->widebandCQIValueCodeword1,
                    dlCQIInfo_p->previousTimeAveragedWBCQICW1);

            /* Correcting wideband CQI and MCS for CW2 from BLER */
            dlCQIInfo_p->widebandCQICodeWordTwo = getEffectiveDLCQI(
                    dlCQIInfo_p->previousTimeAveragedWBCQICW1,
                    dlCQIInfo_p->cqiCorrectionFactorForCW1,
                    dlUEContext_p );

            dlCQIInfo_p->mcsIndexCodeWordTwo = 
                         getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordTwo
                                     );
            /* SPR 4769 changes end */

            /*SPR 6424 Fix Start*/

            /* Cyclomatic_complexity_changes_start */
            getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordTwo,
                    &(dlCQIInfo_p->modulationSchemeCodeWordTwo));
            /* Cyclomatic_complexity_changes_end */

            /*SPR 6424 Fix End*/
            /* ICIC chnages start */
            /* CA Changes start */
            /* CA_phase2_csi_code Start */
            if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
            {
                updateDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo,
                        internalCellIndex); 
            }
            else
            {
                macUpdateSCellDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->
                        /* +- SPR 17777 */
                        widebandCQICodeWordTwo,scellArrIndex);
            }
            /* CA_phase2_csi_code End */
            /* CA Changes end */
            /* ICIC changes end */

            LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                    getCurrentTick(),dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,
                    dlCQIInfo_p->mcsIndexCodeWordOne,
                    dlCQIInfo_p->widebandCQICodeWordTwo,
                    dlCQIInfo_p->mcsIndexCodeWordTwo,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,__func__,"WB_BOTH_CW");

            toProcessCW2 = TRUE;
        }
        else
        {
            LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                    getCurrentTick(),dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,
                    dlCQIInfo_p->mcsIndexCodeWordOne,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,__func__,"WB_CW0");

        }   

        /* + TM7_8 Changes Start */
        /* In case of TM8, wideBandPMIIndex is filled from higher layer precoding value 
         * i.e. precodingIndex instead of latest reported PMI value from UE */
        if(TX_MODE_8 == dlMimoInfo_p->transmissionMode)
        {
            dlCQIInfo_p->wideBandPMIIndex =  dlMimoInfo_p->precodingIndex;
        }
        else
        {
            /* 4x4 DL MIMO CHG START */
            checkPmiInCbsrForWideband(aperiodicCQIMode22_p,dlUEContext_p,dlCQIInfo_p,
                    internalCellIndex);
            /* 4x4 DL MIMO CHG END */
        }
        /* - TM7_8 Changes End */
        /* - CQI_4.1 */

        /* + CQI_5.0 */
        /* CA Changes start */
        if ( FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p\
                [internalCellIndex]->cellParams_p->freqSelectiveSchDL )
            /* CA Changes end */
        {
            /*Get the number of SubBand Count for which the report has came*/
            subBandCount = aperiodicCQIMode22_p->numberOfSubbands;
            dlCQIInfo_p->isSubBandReportAvailable = 1;
            subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;
            cqiInfo_p = &subBandReportInfo_p->cqiInfo;
            subbandInfo22_p = &aperiodicCQIMode22_p->subbandInfo22[0]; 
            /* + TM7_8 Changes Start */
            if(TX_MODE_8 == dlMimoInfo_p->transmissionMode)
            {
                subbandPMIValue = INVALID_PMI_VALUE;
            }
            else
            {
                subbandPMIValue = aperiodicCQIMode22_p->subbandPMIValue;
            }
            /* - TM7_8 Changes End */


            SubbandPMIInfo tempSubbandPMIInfo;
            memSet(&tempSubbandPMIInfo,0,sizeof(SubbandPMIInfo));
            cqiInfo_p->cqiInfoSpatialMul.pmiInfo = tempSubbandPMIInfo;

            /* This functionality is added so that subbands for which subband PMI 
             * is not received, should be initialized with wideband PMI */
            /* + TM7_8 Changes Start */
            if(TX_MODE_8 == dlMimoInfo_p->transmissionMode)
            {
                *isSubbandPMIAvailable_p = FALSE;
            }
            else
            {
                *isSubbandPMIAvailable_p = TRUE;
            }
            /* - TM7_8 Changes End */

            /* CA Changes start */
            apMode22SubBandCount = cellSpecificParams_g.cellConfigAndInitParams_p\
                                   [internalCellIndex]->cellParams_p->ueSelectedN;
            /* CA Changes end */
            while (apMode22SubBandCount--)
            {
                startRBGIndex = 
                    subBandToRBGMapAperiodic_p[apMode22SubBandCount].startRBGIndex;
                endRBGIndex = 
                    subBandToRBGMapAperiodic_p[apMode22SubBandCount].endRBGIndex;
                rbgIndexCount = endRBGIndex -startRBGIndex + 1;
                while(rbgIndexCount--)
                {
                    if(1 == dlMimoInfo_p->dlRi)
                    {    
                        updateRAT0CW1SpatialMulSubBandInfo(
                                startRBGIndex,
                                INVALID_CQI_VALUE,
                                dlCQIInfo_p->wideBandPMIIndex,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */


                        updateRAT1CW1SpatialMulSubBandInfo(
                                startRBGIndex,
                                INVALID_CQI_VALUE,
                                dlCQIInfo_p->wideBandPMIIndex,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */
                    }

                    else if(2 == dlMimoInfo_p->dlRi)
                    {
                        updateRAT0CW2SpatialMulSubBandInfo(
                                startRBGIndex,
                                INVALID_CQI_VALUE,
                                dlCQIInfo_p->wideBandPMIIndex,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */

                        updateRAT1CW2SpatialMulSubBandInfo(
                                startRBGIndex,
                                INVALID_CQI_VALUE,
                                dlCQIInfo_p->wideBandPMIIndex,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */
                    }     
                    startRBGIndex++;
                }
            }

            while (subBandCount--)
            {
                /* + CQI_4.1 */
                subbandNum = subbandInfo22_p[subBandCount];

                startRBGIndex = subBandToRBGMapAperiodic_p[subbandNum].startRBGIndex;

                endRBGIndex = subBandToRBGMapAperiodic_p[subbandNum].endRBGIndex;
                rbgIndexCount = endRBGIndex -startRBGIndex + 1;

                subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[startRBGIndex];
                /* 4x4 DL MIMO CHG START */
                /* +- SPR 17777 */
                checkPmiInCbsrForSubband(&subbandPMIValue,
#ifdef CSI_VALIDATION
                        aperiodicCQIMode22_p,
#endif
                        /* +- SPR 17777 */
                        dlUEContext_p,subBandToCQInfo_p,internalCellIndex);
                /* 4x4 DL MIMO CHG END */
                oldCqiForRBGCW0 = subBandToCQInfo_p->cqiIndexCW1;
                oldCqiForRBGCW1 = subBandToCQInfo_p->cqiIndexCW2;

                /* SPR 4769 changes start */
                /* Time Averaging */
                /* SPR 9427 changes start */
                subBandToCQInfo_p->previousTimeAveragedSBCQICW0 = calculateAverageDLCQIforAP(
                        /* SPR 9427 changes start */
                        aperiodicCQIMode22_p->cqiValueCodeword0,
                        subBandToCQInfo_p->previousTimeAveragedSBCQICW0);

                /* Correcting Subband cqi for CW0 from BLER */
                subBandToCQInfo_p->cqiIndexCW1 = getEffectiveDLCQI(
                        subBandToCQInfo_p->previousTimeAveragedSBCQICW0,
                        dlCQIInfo_p->cqiCorrectionFactorForCW0,
                        dlUEContext_p );

                /* Average Subband CQI and MCS calculation for CW0 */
                correctedSubbandCQICW0 = subBandToCQInfo_p->cqiIndexCW1;
                        correctedSubbandMCSCW0 = getMCSFromCQI(correctedSubbandCQICW0
                                                  );
                /* SPR 4769 changes end */
                /*SPR 6424 Fix Start*/

                /* Cyclomatic_complexity_changes_start */
                getModSchemeFromMcsIndex(correctedSubbandMCSCW0,
                        &(dlCQIInfo_p->modulationSchemeCodeWordOne));
                /* Cyclomatic_complexity_changes_end */

                /*SPR 6424 Fix End*/
                /*Update the New CQI Index Information for CW0*/
                subBandToCQInfo_p->mcsIndexCW1 = correctedSubbandMCSCW0;

                if (TRUE == toProcessCW2)
                {    
                    /* SPR 4769 changes start */
                    /* Time Averaging */
                    /* SPR 9427 changes start */
                    subBandToCQInfo_p->previousTimeAveragedSBCQICW1 = calculateAverageDLCQIforAP(
                            /* SPR 9427 changes start */
                            aperiodicCQIMode22_p->cqiValueCodeword1,
                            subBandToCQInfo_p->previousTimeAveragedSBCQICW1);

                    /* Correcting Subband cqi for CW1 from BLER */
                    subBandToCQInfo_p->cqiIndexCW2 = getEffectiveDLCQI(
                            subBandToCQInfo_p->previousTimeAveragedSBCQICW1,
                            dlCQIInfo_p->cqiCorrectionFactorForCW1,
                            dlUEContext_p );

                    /* Average Subband CQI and MCS calculation for CW1 */
                    correctedSubbandCQICW1 = subBandToCQInfo_p->cqiIndexCW2;
                correctedSubbandMCSCW1 = getMCSFromCQI(correctedSubbandCQICW1
                                          );
                    /*SPR 6424 Fix Start*/

                    /* Cyclomatic_complexity_changes_start */
                    getModSchemeFromMcsIndex(correctedSubbandMCSCW1,
                            &(dlCQIInfo_p->modulationSchemeCodeWordTwo));
                    /* Cyclomatic_complexity_changes_end */

                    /*SPR 6424 Fix End*/
                    /* SPR 4769 changes end */

                    /*Update the New MCS Index Information for CW1*/
                    subBandToCQInfo_p->mcsIndexCW2 = correctedSubbandMCSCW1;
                }

                while(rbgIndexCount--)
                {
                    /* SPR 3803 changes start */  
                    if (oldCqiForRBGCW0)
                    {
                        /* SPR 3803 changes end */  
                        resetRAT0CW1SpatialMulSubBandInfo(
                                startRBGIndex,
                                oldCqiForRBGCW0,
                                &cqiInfo_p->cqiInfoSpatialMul);

                        resetRAT1CW1SpatialMulSubBandInfo(
                                startRBGIndex,
                                oldCqiForRBGCW0,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p);
                        /* SPR 3803 changes start */  
                    }
                    /* SPR 3803 changes end */  

                    if(1 == dlMimoInfo_p->dlRi)
                    {
                        /* RBG/PRBs should be initialized with received subband PMI */
                        updateRAT0CW1SpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQICW0,
                                subbandPMIValue,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */

                        updateRAT1CW1SpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQICW0,
                                subbandPMIValue,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */
                    }
                    else if(2 == dlMimoInfo_p->dlRi)   
                    {
                        /* RBG/PRBs should be initialized with received subband PMI */
                        updateRAT0CW1SpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQICW0,
                                INVALID_PMI_VALUE,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */

                        updateRAT1CW1SpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQICW0,
                                INVALID_PMI_VALUE,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */
                    }

                    if (TRUE == toProcessCW2)
                    {
                        /* SPR 3803 changes start */  
                        if (oldCqiForRBGCW1)
                        {
                            /* SPR 3803 changes end */  
                            resetRAT0CW2SpatialMulSubBandInfo(
                                    startRBGIndex,
                                    oldCqiForRBGCW1,
                                    &cqiInfo_p->cqiInfoSpatialMul);

                            resetRAT1CW2SpatialMulSubBandInfo(
                                    startRBGIndex,
                                    oldCqiForRBGCW1,
                                    &cqiInfo_p->cqiInfoSpatialMul,
                                    rat0Info_p,
                                    rat1Info_p);
                            /* SPR 3803 changes start */  
                        }
                        /* SPR 3803 changes end */  

                        /* RBG/PRBs should be initialized with received 
                         * subband PMI */
                        updateRAT0CW2SpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQICW1,
                                subbandPMIValue,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */

                        updateRAT1CW2SpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQICW1,
                                subbandPMIValue,
                                &cqiInfo_p->cqiInfoSpatialMul,
                                rat0Info_p,
                                rat1Info_p,
                                /* CA Changes start */
                                numOfTxAnteenas);
                        /* CA Changes end */
                    }     
                    startRBGIndex++;
                    /* - CQI_4.1 */
                }
            }
        }
    }
    return;
}

/*****************************************************************************
 * Function Name  : processCQIMode10Type3
 * Inputs         : periodicMode10Type3_p - This holds pointer to 
 *                                          PeriodicMode10Type3 structure,
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the rankIndicator in UL context and 
 *                  info of CW1 to CW2.
 *****************************************************************************/
STATIC  void processCQIMode10Type3(
                                    PeriodicMode10Type3 *periodicMode10Type3_p,
                                    DLUEContext *dlUEContext_p,
									InternalCellIndex internalCellIndex)

{
    /* TDD SB_CQI */

    /* + CQI_5.0 */
    DLCQIInfo *dlCQIInfo_p = PNULL;
    /* CA_phase2_csi_code Start */
    DLMIMOInfo *dlMimoInfo_p;
    UInt8 scellArrIndex = INVALID_8_VAL;
    /*+EICIC . */
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    /*-EICIC . */
    /* CA_phase2_csi_code End */
    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;

    }
    /* CA_phase2_csi_code Start */
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        dlMimoInfo_p = &(dlUEContext_p->dlMIMOInfo);
    }
    else
    {
        scellArrIndex =  getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
        /*klockwork warning fix*/
        if(scellArrIndex <= MAX_NUM_SCELL)
        {
            dlMimoInfo_p = &(dlUEContext_p->dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo);
        }
        else
        {
            return;
        }
    }
    /* +EICIC .*/

    CHECK_PERIODIC_SECONDARY_CONDITION_FOR_RI(ulUEContext_p, internalCellIndex)
    {
        if ((RANK_INDICATOR_1 == dlMimoInfo_p->dlRi_2) &&
                /* SPR 21996 Fix Start */
                (RANK_INDICATOR_2 <= periodicMode10Type3_p->riValue))
                /* SPR 21996 Fix End */
        {
            /* RI in DL context will be updated after very next CQI report */
            dlCQIInfo_p->riUpdationFlag_2 = LATER_DL_RI_UPDATE;
	    /* SPR 21996 Fix Start */
	    /* It stores the RI value reported by UE to update until next CQI report on Later updation
	     *  when present RI is 1 and received one is greater than 1*/
            dlCQIInfo_p->dlRiToUpdate = periodicMode10Type3_p->riValue;
	    /* SPR 21996 Fix End */
        }
        else
        {
            dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
            /* + SPR_6854 */
	    /* SPR 21996 Fix Start */
            if ((RANK_INDICATOR_2 <= dlMimoInfo_p->dlRi_2) &&
	    /* SPR 21996 Fix End */
                    (RANK_INDICATOR_1 == periodicMode10Type3_p->riValue))
            {
                dlCQIInfo_p->cqiCorrectionFactorForCW1_2 = 0;
            }
            /* - SPR_6854 */
            /* Updating RI in DL context */
            dlMimoInfo_p->dlRi_2 = periodicMode10Type3_p->riValue;
        }

    }
    else
    {
        /* -EICIC . */
        if (  (RANK_INDICATOR_1 == dlMimoInfo_p->dlRi) && 
                /* SPR 21996 Fix Start */
                (RANK_INDICATOR_2 <= periodicMode10Type3_p->riValue) )
                /* SPR 21996 Fix End */
        {
            /* RI in DL context will be updated after very next CQI report */
            dlCQIInfo_p->riUpdationFlag =  LATER_DL_RI_UPDATE; 
	    /* SPR 21996 Fix Start */
	    /* It stores the RI value reported by UE to update until next CQI report on Later updation
	     *  when present RI is 1 and received one is greater than 1*/
            dlCQIInfo_p->dlRiToUpdate = periodicMode10Type3_p->riValue;
	    /* SPR 21996 Fix End */
        }
        else
        {
            dlCQIInfo_p->riUpdationFlag =  IMMEDIATE_DL_RI_UPDATE; 
            /* + SPR_6854 */
	    /* SPR 21996 Fix Start */
            if ((RANK_INDICATOR_2 <= dlMimoInfo_p->dlRi) &&
	    /* SPR 21996 Fix End */
                    (RANK_INDICATOR_1 == periodicMode10Type3_p->riValue) )
            {
                dlCQIInfo_p->cqiCorrectionFactorForCW1 = 0;
            }
            /* - SPR_6854 */
            /* Updating RI in DL context*/
            dlMimoInfo_p->dlRi = periodicMode10Type3_p->riValue;
        }    
        /* - CQI_5.0 */
    }

}

/*****************************************************************************
 * Function Name  : processCQIMode20Type3
 * Inputs         : periodicMode20Type3_p - This holds pointer to 
 *                                          PeriodicMode20Type3 structure,
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the rankIndicator in UL context and 
 *                  info of CW1 to CW2.
 *****************************************************************************/
/* CA Changes start */ 
STATIC  void processCQIMode20Type3(
                                    PeriodicMode20Type3 *periodicMode20Type3_p,
                                    DLUEContext *dlUEContext_p
                                     ,InternalCellIndex internalCellIndex)
/* CA Changes end */ 
{
    /* + CQI_5.0 */
    DLCQIInfo *dlCQIInfo_p = PNULL;
    SubBandReportInfo *subBandReportInfo_p = PNULL;
    /* CA_phase2_csi_code Start */
    DLMIMOInfo *dlMimoInfo_p;
    UInt8 scellArrIndex = INVALID_8_VAL;
    /* CA_phase2_csi_code End */

    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;

    }
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        dlMimoInfo_p = &(dlUEContext_p->dlMIMOInfo);
    }
    else
    {
        scellArrIndex =  getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
        /*klockwork warning fix*/
        if(scellArrIndex <= MAX_NUM_SCELL)
        {
            dlMimoInfo_p = &(dlUEContext_p->dlUeScellContext_p[scellArrIndex]->ScelldlMIMOInfo);
        }
        else
        {
            return;
        }
    }
    /* +EICIC . */
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    CHECK_PERIODIC_SECONDARY_CONDITION_FOR_RI (ulUEContext_p, internalCellIndex)
    {    /*If ABS Criteria */
        if ((RANK_INDICATOR_1 == dlMimoInfo_p->dlRi_2) &&
                /* SPR 21996 Fix Start */
                (RANK_INDICATOR_2 <= periodicMode20Type3_p->riValue))
                /* SPR 21996 Fix End */
        {
            /* RI in DL context will be updated after very next CQI report */
            dlCQIInfo_p->riUpdationFlag_2 = LATER_DL_RI_UPDATE;
	    /* SPR 21996 Fix Start */
	    /* It stores the RI value reported by UE to update until next CQI report on Later updation
	     *  when present RI is 1 and received one is greater than 1*/
            dlCQIInfo_p->dlRiToUpdate = periodicMode20Type3_p->riValue;
	    /* SPR 21996 Fix End */

        }
        else
        {
            /* Updating subband CQI info if FSS is ON and RI toggled from greater than 1 to 1 */
            /* CA Changes start */
	    /* SPR +- 21996 Fix */
            if (((RANK_INDICATOR_2 <= dlMimoInfo_p->dlRi_2) &&
                        (RANK_INDICATOR_1 == periodicMode20Type3_p->riValue)) &&
                    (FREQUENCY_SELECTIVE_ON ==
                     cellSpecificParams_g.cellConfigAndInitParams_p
                     [internalCellIndex]->cellParams_p->freqSelectiveSchDL))
            {
                subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;
                /* + SPR_6854 */
                dlCQIInfo_p->cqiCorrectionFactorForCW1_2 = 0;
                /* - SPR_6854 */
                resetCW2Info_2 (subBandReportInfo_p, internalCellIndex);
                /* CA Changes end */
            }

            dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
            /* Updating RI in DL context */
            dlMimoInfo_p->dlRi_2 = periodicMode20Type3_p->riValue;
        }
    }
    else
    {

        if (  (RANK_INDICATOR_1 == dlMimoInfo_p->dlRi) && 
                /* SPR 21996 Fix Start */
                (RANK_INDICATOR_2 <= periodicMode20Type3_p->riValue) )
                /* SPR 21996 Fix End */
        {
            /* RI in DL context will be updated after very next CQI report */
            dlCQIInfo_p->riUpdationFlag =  LATER_DL_RI_UPDATE; 
	    /* SPR 21996 Fix Start */
	    /* It stores the RI value reported by UE to update until next CQI report on Later updation
	     *  when present RI is 1 and received one is greater than 1*/
            dlCQIInfo_p->dlRiToUpdate = periodicMode20Type3_p->riValue;
	    /* SPR 21996 Fix End */

        }
        else
        {
            /* Updating subband CQI info if FSS is ON and RI toggled from greater than 1 to 1 */
            /* CA Changes start */ 
	    /* SPR +- 21996 Fix */
            if ( ((RANK_INDICATOR_2 <= dlMimoInfo_p->dlRi) &&
                        (RANK_INDICATOR_1 == periodicMode20Type3_p->riValue)) && 
                    (FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.\
                     cellConfigAndInitParams_p[internalCellIndex]\
                     ->cellParams_p->freqSelectiveSchDL) )
            {
                subBandReportInfo_p =  dlCQIInfo_p->subBandReportInfo_p;
                /* + SPR_6854 */
                dlCQIInfo_p->cqiCorrectionFactorForCW1 = 0;
                /* - SPR_6854 */
                resetCW2Info(subBandReportInfo_p, internalCellIndex);
                /* CA Changes end */ 
            }    

            dlCQIInfo_p->riUpdationFlag =  IMMEDIATE_DL_RI_UPDATE; 
            /* Updating RI in DL context*/
            dlMimoInfo_p->dlRi = periodicMode20Type3_p->riValue;
        }    
    }
    return; 
    /* - CQI_5.0 */
}

/*****************************************************************************
 * Function Name  : processCQIMode11Type3
 * Inputs         : periodicMode11Type3_p - This holds pointer to 
 *                                          PeriodicMode11Type3 structure,
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the rankIndicator in UL context and
 *                  info of CW1 to CW2.
 *****************************************************************************/
STATIC  void processCQIMode11Type3(
                                    PeriodicMode11Type3 *periodicMode11Type3_p,
                                    DLUEContext *dlUEContext_p,
                                    InternalCellIndex internalCellIndex)
{
    /* + CQI_5.0 */
    DLCQIInfo *dlCQIInfo_p = PNULL;
    /* CA_phase2_csi_code Start */
    UInt8 *dlRi_p;
    /* CA_phase2_csi_code End */
    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;
    }

    /*+EICIC .*/
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    CHECK_PERIODIC_SECONDARY_CONDITION_FOR_RI (ulUEContext_p, internalCellIndex)
    {    /*If ABS Criteria */
        /* CA_phase2_csi_code Start */
        if (dlUEContext_p->internalCellIndex == internalCellIndex)
        {
            /* Pcell */
            dlRi_p = &(dlUEContext_p->dlMIMOInfo.dlRi_2);
        }
        else
        {
            /* Scell */
            dlRi_p =
                &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
                        ScelldlMIMOInfo.dlRi_2);
        }
        /* CA_phase2_csi_code End */
        if ((RANK_INDICATOR_1 == *dlRi_p) &&
                /* SPR 21996 Fix Start */
                (RANK_INDICATOR_2 <= periodicMode11Type3_p->riValue))
                /* SPR 21996 Fix End */
        {
            /* RI in DL context will be updated after very next CQI report */
            dlCQIInfo_p->riUpdationFlag_2 = LATER_DL_RI_UPDATE;
	    /* SPR 21996 Fix Start */
	    /* It stores the RI value reported by UE to update until next CQI report on Later updation
	     *  when present RI is 1 and received one is greater than 1*/
            dlCQIInfo_p->dlRiToUpdate = periodicMode11Type3_p->riValue;
	    /* SPR 21996 Fix End */
        }
        else
        {
            dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
            /* + SPR_6854 */
	    /* SPR +- 21996 Fix */
            if ((RANK_INDICATOR_2 <= *dlRi_p) &&
                    (RANK_INDICATOR_1 == periodicMode11Type3_p->riValue))
            {
                dlCQIInfo_p->cqiCorrectionFactorForCW1_2 = 0;
            }
            /* - SPR_6854 */
            /* Updating RI in DL context */
            *dlRi_p = periodicMode11Type3_p->riValue;
        }
    }
    else
    {
        /*-EICIC .*/
        /* CA_phase2_csi_code Start */
        if(dlUEContext_p->internalCellIndex == internalCellIndex)
        {
            /* Pcell */
            dlRi_p = &(dlUEContext_p->dlMIMOInfo.dlRi);
        }
        else
        {
            /* Scell */
            dlRi_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.dlRi);
        }
        /* CA_phase2_csi_code End */

        if ( (RANK_INDICATOR_1 == *dlRi_p) && 
                /* SPR 21996 Fix Start */
                (RANK_INDICATOR_2 <= periodicMode11Type3_p->riValue) )
                /* SPR 21996 Fix End */
        {
            /* RI in DL context will be updated after very next CQI report */
            dlCQIInfo_p->riUpdationFlag =  LATER_DL_RI_UPDATE; 
	    /* SPR 21996 Fix Start */
	    /* It stores the RI value reported by UE to update until next CQI report on Later updation
	     *  when present RI is 1 and received one is greater than 1*/
            dlCQIInfo_p->dlRiToUpdate = periodicMode11Type3_p->riValue;
	    /* SPR 21996 Fix End */
        }
        else
        {
            dlCQIInfo_p->riUpdationFlag =  IMMEDIATE_DL_RI_UPDATE; 
            /* + SPR_6854 */
	    /* SPR +- 21996 Fix */
            if ((RANK_INDICATOR_2 <= *dlRi_p) &&
                    (RANK_INDICATOR_1 == periodicMode11Type3_p->riValue) )
            {
                dlCQIInfo_p->cqiCorrectionFactorForCW1 = 0;
            }
            /* - SPR_6854 */
            /* Updating RI in DL context*/
            *dlRi_p = periodicMode11Type3_p->riValue;
        }    
    }
    return;
    /* - CQI_5.0 */
}

/*****************************************************************************
 * Function Name  : processCQIMode21Type3
 * Inputs         : periodicMode21Type3_p - This holds pointer to 
 *                                          PeriodicMode21Type3 structure,
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the rankIndicator in UL context and 
 *                  info of CW1 to CW2.
 *****************************************************************************/
/* CA Changes start */
STATIC  void processCQIMode21Type3(
                                    PeriodicMode21Type3 *periodicMode21Type3_p,
                                    DLUEContext  *dlUEContext_p
                                     ,InternalCellIndex internalCellIndex)
/* CA Changes end */
{
    /* + CQI_5.0 */
    DLCQIInfo *dlCQIInfo_p = PNULL;
    /* CA_phase2_csi_code Start */
    DLMIMOInfo *dlMimoInfo_p;
    /* CA_phase2_csi_code End */
    SubBandReportInfo *subBandReportInfo_p = PNULL;

    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;

    }
    /* CA_phase2_csi_code Start */
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        dlMimoInfo_p = &(dlUEContext_p->dlMIMOInfo);
    }
    else
    {
        dlMimoInfo_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo);
    }
    /* CA_phase2_csi_code End */
    /* +EICIC . */
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    CHECK_PERIODIC_SECONDARY_CONDITION_FOR_RI (ulUEContext_p, internalCellIndex)
    {    /*ABS Criteria */
        if ((RANK_INDICATOR_1 == dlMimoInfo_p->dlRi_2) &&
                /* SPR 21996 Fix Start */
                (RANK_INDICATOR_2 <= periodicMode21Type3_p->riValue))
                /* SPR 21996 Fix End */
        {
            /* RI in DL context will be updated after very next CQI report */
            dlCQIInfo_p->riUpdationFlag_2 = LATER_DL_RI_UPDATE;
	    /* SPR 21996 Fix Start */
	    /* It stores the RI value reported by UE to update until next CQI report on Later updation
	     *  when present RI is 1 and received one is greater than 1*/
            dlCQIInfo_p->dlRiToUpdate = periodicMode21Type3_p->riValue;
	    /* SPR 21996 Fix End */

        }
        else
        {
            /* Updating subband CQI info if FSS is ON and RI toggled from greater than 1 to 1 */
            /* CA Changes start */
	    /* SPR +- 21996 Fix */
            if (((RANK_INDICATOR_2 <= dlMimoInfo_p->dlRi_2) &&
                        (RANK_INDICATOR_1 == periodicMode21Type3_p->riValue)) &&
                    (FREQUENCY_SELECTIVE_ON ==
                     cellSpecificParams_g.cellConfigAndInitParams_p
                     [internalCellIndex]->cellParams_p->freqSelectiveSchDL))
            {
                subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;
                /* + SPR_6854 */
                dlCQIInfo_p->cqiCorrectionFactorForCW1_2 = 0;
                /* - SPR_6854 */
                resetCW2Info_2 (subBandReportInfo_p, internalCellIndex);
                /* CA Changes end */
            }

            dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
            /* Updating RI in DL context */
            dlMimoInfo_p->dlRi_2 = periodicMode21Type3_p->riValue;
        }
    }
    else
    {
        /* -EICIC . */

        if ( (RANK_INDICATOR_1 == dlMimoInfo_p->dlRi) && 
                /* SPR 21996 Fix Start */
                (RANK_INDICATOR_2 <= periodicMode21Type3_p->riValue) )
                /* SPR 21996 Fix End */
        {
            /* RI in DL context will be updated after very next CQI report */
            dlCQIInfo_p->riUpdationFlag =  LATER_DL_RI_UPDATE; 
	    /* SPR 21996 Fix Start */
	    /* It stores the RI value reported by UE to update until next CQI report on Later updation
	     *  when present RI is 1 and received one is greater than 1*/
            dlCQIInfo_p->dlRiToUpdate = periodicMode21Type3_p->riValue;
	    /* SPR 21996 Fix End */

        }
        else
        {
            /* Updating subband CQI info if FSS is ON and RI toggled from greater than 1 to 1*/
            /* CA Changes start */ 
	    /* SPR +- 21996 Fix */
            if ( ((RANK_INDICATOR_2 <= dlMimoInfo_p->dlRi) &&
                        (RANK_INDICATOR_1 == periodicMode21Type3_p->riValue)) &&
                    (FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.\
                     cellConfigAndInitParams_p[internalCellIndex]\
                     ->cellParams_p->freqSelectiveSchDL) )
            {    
                subBandReportInfo_p =  dlCQIInfo_p->subBandReportInfo_p;
                /* + SPR_6854 */
                dlCQIInfo_p->cqiCorrectionFactorForCW1 = 0;
                /* - SPR_6854 */
                resetCW2Info(subBandReportInfo_p,internalCellIndex);
                /* CA Changes end */ 
            }

            dlCQIInfo_p->riUpdationFlag =  IMMEDIATE_DL_RI_UPDATE; 
            /* Updating RI in DL context*/
            dlMimoInfo_p->dlRi = periodicMode21Type3_p->riValue;
        }    
    }
    return; 
    /* - CQI_5.0 */
}

/*****************************************************************************
 * Function Name  : processCQIMode10Type4
 * Inputs         : periodicMode10Type4_p - This holds pointer to 
 *                                          PeriodicMode10Type4 structure,
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the windBandCQIIndex in 
 *                  DLCQIInfo structure for CodeWord Ones.
 *****************************************************************************/
/* CA Changes start */ 
STATIC  void processCQIMode10Type4(
        PeriodicMode10Type4 *periodicMode10Type4_p,
        DLUEContext *dlUEContext_p
          ,InternalCellIndex internalCellIndex)
/* CA Changes end */ 
{
    DLCQIInfo *dlCQIInfo_p = PNULL;
    /* CA_phase2_csi_code Start */
    DLMIMOInfo *dlMimoInfo_p;
    UInt8 servCellIndex = 1;
    /* CA_phase2_csi_code End */

    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;

    }
    /* CA_phase2_csi_code Start */
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        dlMimoInfo_p = &(dlUEContext_p->dlMIMOInfo);
    }
    else
    {
        dlMimoInfo_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo);
    }
    /* CA_phase2_csi_code End */

    dlCQIInfo_p->isWideBandReportAvailable = 1;
    dlCQIInfo_p->isSubBandReportAvailable = 0;
    /* TDD SB_CQI */
    /* + CQI_4.1 */
    /* SPR 4769 changes start */
    /* Time Averaging */
    /*+EICIC . */
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;


    ulUEContext_p->ueComplianceRelease  = RELEASE_10; /* TO BE REMOVED - EICIC */

    CHECK_PERIODIC_SECONDARY_CONDITION_FOR_CSI (ulUEContext_p,
            internalCellIndex)
    {


        ulUEContext_p->isDLWideBandCQIReportAvailable_2 = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne_2 =
            periodicMode10Type4_p->widebandCQIValueCW0;
        ulUEContext_p->dlWidebandCQICodeWordTwo_2 =
            periodicMode10Type4_p->widebandCQIValueCW1;

        dlCQIInfo_p->isWideBandReportAvailable_2 = 1;
        dlCQIInfo_p->isSubBandReportAvailable_2 = 0;
        dlCQIInfo_p->previousTimeAveragedWBCQICW0_2 =
            calculateAverageDLCQI (periodicMode10Type4_p->widebandCQIValueCW0,
                    dlCQIInfo_p->previousTimeAveragedWBCQICW0_2);

        /* Correcting wideband CQI and MCS from BLER */
        dlCQIInfo_p->widebandCQICodeWordOne_2 =
            getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW0_2,
                    dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                    dlUEContext_p);

        dlCQIInfo_p->mcsIndexCodeWordOne_2 =
            getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordOne_2
                          );
        /* SPR 4769 changes end */

        /*SPR 6424 Fix Start */
        getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordOne_2,
                &(dlCQIInfo_p->modulationSchemeCodeWordOne_2));
        /*SPR 6424 Fix End */
        /* ICIC chnages start */
        /* CA Changes start */
        /* CA_phase2_csi_code Start */
        if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
        {
            updateDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex, servCellIndex);

        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */
        /* ICIC changes end */

        /* if transmission mode is open loop MIMO, reported CQI value of CW0 
         * is same for CW1 ref. Spec 36.213 sec 7.2.2.
         */

        if ((TX_MODE_3 == dlMimoInfo_p->transmissionMode) ||
                /* + TM7_8 Changes Start */
                (TX_MODE_8 == dlMimoInfo_p->transmissionMode))
            /* - TM7_8 Changes End */
        {
            /* + CQI_5.0 */
            /* if riUpdationFlag value is LATER_DL_RI_UPDATE then update dlRi */
            if (LATER_DL_RI_UPDATE == dlCQIInfo_p->riUpdationFlag_2)
            {
                /* SPR 21996 Fix Start */
                dlMimoInfo_p->dlRi_2 = dlCQIInfo_p->dlRiToUpdate; 
                /* SPR 21996 Fix End */
                dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
                /*SPR 9242 FIX START */
                dlCQIInfo_p->previousTimeAveragedWBCQICW1 = dlCQIInfo_p->previousTimeAveragedWBCQICW0_2;
                dlCQIInfo_p->cqiCorrectionFactorForCW1 = dlCQIInfo_p->cqiCorrectionFactorForCW0_2; 
                /*SPR 9242 FIX END */
            }
            if (0 != periodicMode10Type4_p->widebandCQIValueCW1)
            {
                /* SPR 4769 changes start */
                /* Time Averaging */
                dlCQIInfo_p->previousTimeAveragedWBCQICW1_2 =
                    calculateAverageDLCQI (periodicMode10Type4_p->
                            widebandCQIValueCW1,
                            dlCQIInfo_p->
                            previousTimeAveragedWBCQICW1_2);
                dlCQIInfo_p->widebandCQICodeWordTwo =
                    getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW1_2,
                            dlCQIInfo_p->cqiCorrectionFactorForCW1_2,
                            dlUEContext_p);

                dlCQIInfo_p->mcsIndexCodeWordTwo_2 =
                    getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordTwo_2
                                    );
                /* SPR 4769 changes end */
                /*SPR 6424 Fix Start */
                getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordTwo_2,
                        &(dlCQIInfo_p->
                            modulationSchemeCodeWordTwo_2));
                /*SPR 6424 Fix End */
                /* SPR 13855 fix start */
                if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
                {
                    updateDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo,
                            internalCellIndex); 
                }
                else
                {
                    macUpdateSCellDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->
                            /* +- SPR 17777 */
                            widebandCQICodeWordTwo,servCellIndex);
                }

                /* SPR 13855 fix end */

                LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                        getCurrentTick (), dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordOne_2,
                        dlCQIInfo_p->mcsIndexCodeWordOne_2,
                        dlCQIInfo_p->widebandCQICodeWordTwo_2,
                        dlCQIInfo_p->mcsIndexCodeWordTwo_2, DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE, __func__, "WB_BOTH_CW_FOR_ABS");

            }
            else
            {

                LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                        getCurrentTick (), dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordOne_2,
                        dlCQIInfo_p->mcsIndexCodeWordOne_2, DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE, __func__, "WB_CW0_FOR_ABS");
            }
            /* - CQI_5.0 */
        }
        else
        {
            LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                    getCurrentTick (), dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    dlCQIInfo_p->mcsIndexCodeWordOne_2, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE, __func__, "WB_CW0_FOR_ABS");

        }
        /* - CQI_4.1 */
        /* TDD SB_CQI */

    }
    else
    {

        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne =
            periodicMode10Type4_p->widebandCQIValueCW0;
        ulUEContext_p->dlWidebandCQICodeWordTwo =
            periodicMode10Type4_p->widebandCQIValueCW1;
        /*-EICIC . */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0 = calculateAverageDLCQI(
                periodicMode10Type4_p->widebandCQIValueCW0,
                dlCQIInfo_p->previousTimeAveragedWBCQICW0);

        /* Correcting wideband CQI and MCS from BLER */
        dlCQIInfo_p->widebandCQICodeWordOne =  getEffectiveDLCQI (
                dlCQIInfo_p->previousTimeAveragedWBCQICW0,
                dlCQIInfo_p->cqiCorrectionFactorForCW0,
                dlUEContext_p );

        dlCQIInfo_p->mcsIndexCodeWordOne =
                     getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordOne
                                     );
        /* SPR 4769 changes end */

        /*SPR 6424 Fix Start*/

        /* Cyclomatic_complexity_changes_start */
        getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordOne,
                &(dlCQIInfo_p->modulationSchemeCodeWordOne));
        /* Cyclomatic_complexity_changes_end */

        /*SPR 6424 Fix End*/
        /* ICIC chnages start */
        /* CA Changes start */ 
        /* CA_phase2_csi_code Start */
        if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
        {
            updateDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                    internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                    /* +- SPR 17777 */
                    servCellIndex);

        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */ 
        /* ICIC changes end */

        /* if transmission mode is open loop MIMO, reported CQI value of CW0 
         * is same for CW1 ref. Spec 36.213 sec 7.2.2.
         */

        if((TX_MODE_3 == dlMimoInfo_p->transmissionMode) ||
                /* + TM7_8 Changes Start */
                (TX_MODE_8 == dlMimoInfo_p->transmissionMode))
            /* - TM7_8 Changes End */
        {  
            /* + CQI_5.0 */ 
            /* if riUpdationFlag value is LATER_DL_RI_UPDATE then update dlRi */
            if ( LATER_DL_RI_UPDATE == dlCQIInfo_p->riUpdationFlag )
            {
                /* SPR 21996 Fix Start */
                dlMimoInfo_p->dlRi = dlCQIInfo_p->dlRiToUpdate; 
                /* SPR 21996 Fix End */
                dlCQIInfo_p->riUpdationFlag =  IMMEDIATE_DL_RI_UPDATE;
                /*SPR 9242 FIX START*/
                dlCQIInfo_p->previousTimeAveragedWBCQICW1 = dlCQIInfo_p->previousTimeAveragedWBCQICW0;
                dlCQIInfo_p->cqiCorrectionFactorForCW1 = dlCQIInfo_p->cqiCorrectionFactorForCW0; 
                /*SPR 9242 FIX END*/
            }    

            if( 0 !=  periodicMode10Type4_p->widebandCQIValueCW1 )
            {    
                /* SPR 4769 changes start */
                /* Time Averaging */
                dlCQIInfo_p->previousTimeAveragedWBCQICW1 = calculateAverageDLCQI(
                        periodicMode10Type4_p->widebandCQIValueCW1,
                        dlCQIInfo_p->previousTimeAveragedWBCQICW1);
                dlCQIInfo_p->widebandCQICodeWordTwo =  getEffectiveDLCQI (
                        dlCQIInfo_p->previousTimeAveragedWBCQICW1,
                        dlCQIInfo_p->cqiCorrectionFactorForCW1,
                        dlUEContext_p );

                dlCQIInfo_p->mcsIndexCodeWordTwo =
               getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordTwo
                                     );
                /* SPR 4769 changes end */
                /*SPR 6424 Fix Start*/

                /* Cyclomatic_complexity_changes_start */
                getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordTwo,
                        &(dlCQIInfo_p->modulationSchemeCodeWordTwo));
                /* Cyclomatic_complexity_changes_end */

                /*SPR 6424 Fix End*/
                /* SPR 13855 fix start */
                if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
                {
                    updateDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo,
                            internalCellIndex);
                }
                else
                {
                    macUpdateSCellDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo,
                            /* +- SPR 17777 */
                            servCellIndex);

                }
                /* SPR 13855 fix end */

                LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                        getCurrentTick(),dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordOne,
                        dlCQIInfo_p->mcsIndexCodeWordOne,
                        dlCQIInfo_p->widebandCQICodeWordTwo,
                        dlCQIInfo_p->mcsIndexCodeWordTwo,DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,__func__,"WB_BOTH_CW");

            } 
            else 
            {

                LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                        getCurrentTick(),dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordOne,
                        dlCQIInfo_p->mcsIndexCodeWordOne,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,__func__,"WB_CW0");
            }
            /* - CQI_5.0 */ 
        }
        else
        {    
            LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                    getCurrentTick(),dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,
                    dlCQIInfo_p->mcsIndexCodeWordOne,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,__func__,"WB_CW0");

        }
        /* - CQI_4.1 */
        /* TDD SB_CQI */
    }

    return;
}

/*****************************************************************************
 * Function Name  : processCQIMode20Type4
 * Inputs         : periodicMode20Type4_p - This holds pointer to 
 *                                          PeriodicMode20Type4 structure,
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the windBandCQIIndex in 
 *                  DLCQIInfo structure for CodeWord One.
 *****************************************************************************/
/* CA Changes start */ 
STATIC  void processCQIMode20Type4(
        PeriodicMode20Type4 *periodicMode20Type4_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex)
/* CA Changes end */ 
{
    DLCQIInfo *dlCQIInfo_p = PNULL;
    /* CA_phase2_csi_code Start */
    DLMIMOInfo *dlMimoInfo_p;
    UInt8 servCellIndex = 1;
    /* CA_phase2_csi_code End */

    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;

    }
    /* CA_phase2_csi_code Start */
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        dlMimoInfo_p = &(dlUEContext_p->dlMIMOInfo);
    }
    else
    {
        dlMimoInfo_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo);
    }
    /* CA_phase2_csi_code End */
    /*+EICIC . */
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    CHECK_PERIODIC_SECONDARY_CONDITION_FOR_CSI (ulUEContext_p,
            internalCellIndex)
    {    /*If ABS Criteria */
        ulUEContext_p->isDLWideBandCQIReportAvailable_2 = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne_2 =
            periodicMode20Type4_p->widebandCQIValueCW0;
        ulUEContext_p->dlWidebandCQICodeWordTwo_2 =
            periodicMode20Type4_p->widebandCQIValueCW1;
        dlCQIInfo_p->isWideBandReportAvailable_2 = 1;
        dlCQIInfo_p->isSubBandReportAvailable_2 = 0;
        /* SPR 4769 changes start */
        /* Time Averaging */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0_2 =
            calculateAverageDLCQI (periodicMode20Type4_p->widebandCQIValueCW0,
                    dlCQIInfo_p->previousTimeAveragedWBCQICW0_2);

        /* Correcting wideband CQI and MCS from BLER */
        dlCQIInfo_p->widebandCQICodeWordOne_2 =
            getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW0_2,
                    dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                    dlUEContext_p);
        dlCQIInfo_p->mcsIndexCodeWordOne_2 =
            getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordOne_2
                            );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start */
        getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordOne_2,
                &(dlCQIInfo_p->modulationSchemeCodeWordOne_2));
        /*SPR 6424 Fix End */
        /* ICIC chnages start */
        /* CA Changes start */
        /* CA_phase2_csi_code Start */
        if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
        {
            updateDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex, servCellIndex);
        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */
        /* ICIC changes end */


        /* if transmission mode is open loop MIMO, reported CQI value of CW0 
         * is same for CW1 ref. Spec 36.213 sec 7.2.2.
         */

        if ((TX_MODE_3 == dlMimoInfo_p->transmissionMode) ||
                /* + TM7_8 Changes Start */
                (TX_MODE_8 == dlMimoInfo_p->transmissionMode))
            /* - TM7_8 Changes End */
        {
            /* + CQI_5.0 */
            /* if riUpdationFlag value is LATER_DL_RI_UPDATE then update dlRi */
            if (LATER_DL_RI_UPDATE == dlCQIInfo_p->riUpdationFlag_2)
            {
                /* SPR 21996 Fix Start */
                dlMimoInfo_p->dlRi_2 = dlCQIInfo_p->dlRiToUpdate;
                /* SPR 21996 Fix End */
                dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
                /*SPR 9242 FIX START */
                dlCQIInfo_p->previousTimeAveragedWBCQICW1_2 =
                    dlCQIInfo_p->previousTimeAveragedWBCQICW0_2;
                dlCQIInfo_p->cqiCorrectionFactorForCW1_2 =
                    dlCQIInfo_p->cqiCorrectionFactorForCW0_2;
                /*SPR 9242 FIX END */
            }

            if (0 != periodicMode20Type4_p->widebandCQIValueCW1)
            {
                /* SPR 4769 changes start */
                /* Time Averaging */
                dlCQIInfo_p->previousTimeAveragedWBCQICW1_2 =
                    calculateAverageDLCQI (periodicMode20Type4_p->
                            widebandCQIValueCW1,
                            dlCQIInfo_p->
                            previousTimeAveragedWBCQICW1_2);

                /* Correcting wideband CQI and MCS from BLER */
                dlCQIInfo_p->widebandCQICodeWordTwo_2 =
                    getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW1_2,
                            dlCQIInfo_p->cqiCorrectionFactorForCW1_2,
                            dlUEContext_p);
                dlCQIInfo_p->mcsIndexCodeWordTwo_2 =
                    getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordTwo_2
                                  );
                /* SPR 4769 changes end */
                /*SPR 6424 Fix Start */
                getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordTwo_2,
                        &(dlCQIInfo_p->
                            modulationSchemeCodeWordTwo_2));
                /*SPR 6424 Fix End */

                LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                        getCurrentTick (), dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordOne_2,
                        dlCQIInfo_p->mcsIndexCodeWordOne_2,
                        dlCQIInfo_p->widebandCQICodeWordTwo_2,
                        dlCQIInfo_p->mcsIndexCodeWordTwo_2, DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE, __func__, "WB_BOTH_CW_ABS");
            }
            else
            {

                LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                        getCurrentTick (), dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordOne_2,
                        dlCQIInfo_p->mcsIndexCodeWordOne_2, DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE, __func__, "WB_CW0_ABS");
            }
            /* - CQI_5.0 */
        }
        else
        {
            LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                    getCurrentTick (), dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    dlCQIInfo_p->mcsIndexCodeWordOne_2, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE, __func__, "WB_CW0_ABS");
        }
    }
    else
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne =
            periodicMode20Type4_p->widebandCQIValueCW0;
        ulUEContext_p->dlWidebandCQICodeWordTwo =
            periodicMode20Type4_p->widebandCQIValueCW1;

        dlCQIInfo_p->isWideBandReportAvailable = 1;
        dlCQIInfo_p->isSubBandReportAvailable = 0;

        /* SPR 4769 changes start */
        /* Time Averaging */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0 = calculateAverageDLCQI(
                periodicMode20Type4_p->widebandCQIValueCW0,
                dlCQIInfo_p->previousTimeAveragedWBCQICW0);

        /* Correcting wideband CQI and MCS from BLER */
        dlCQIInfo_p->widebandCQICodeWordOne =  getEffectiveDLCQI(
                dlCQIInfo_p->previousTimeAveragedWBCQICW0,
                dlCQIInfo_p->cqiCorrectionFactorForCW0, dlUEContext_p);
    dlCQIInfo_p->mcsIndexCodeWordOne = getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordOne
                                     );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start*/

        /* Cyclomatic_complexity_changes_start */
        getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordOne,
                &(dlCQIInfo_p->modulationSchemeCodeWordOne));
        /* Cyclomatic_complexity_changes_end */

        /*SPR 6424 Fix End*/
        /* ICIC chnages start */
        /* CA Changes start */ 
        /* CA_phase2_csi_code Start */
        if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
        {
            updateDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne
                    ,internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                    /* +- SPR 17777 */
                    servCellIndex);
        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */ 
        /* ICIC changes end */


        /* if transmission mode is open loop MIMO, reported CQI value of CW0 
         * is same for CW1 ref. Spec 36.213 sec 7.2.2.
         */

        if((TX_MODE_3 == dlMimoInfo_p->transmissionMode) ||
                /* + TM7_8 Changes Start */
                (TX_MODE_8 == dlMimoInfo_p->transmissionMode))
            /* - TM7_8 Changes End */
        {   
            /* + CQI_5.0 */ 
            /* if riUpdationFlag value is LATER_DL_RI_UPDATE then update dlRi */
            if ( LATER_DL_RI_UPDATE == dlCQIInfo_p->riUpdationFlag )
            {
                /* SPR 21996 Fix Start */
                dlMimoInfo_p->dlRi = dlCQIInfo_p->dlRiToUpdate;
                /* SPR 21996 Fix End */
                dlCQIInfo_p->riUpdationFlag =  IMMEDIATE_DL_RI_UPDATE;
                /*SPR 9242 FIX START*/
                dlCQIInfo_p->previousTimeAveragedWBCQICW1 = dlCQIInfo_p->previousTimeAveragedWBCQICW0;
                dlCQIInfo_p->cqiCorrectionFactorForCW1 = dlCQIInfo_p->cqiCorrectionFactorForCW0; 
                /*SPR 9242 FIX END*/
            }    

            if (0 != periodicMode20Type4_p->widebandCQIValueCW1)
            {
                /* SPR 4769 changes start */
                /* Time Averaging */
                dlCQIInfo_p->previousTimeAveragedWBCQICW1 = calculateAverageDLCQI(
                        periodicMode20Type4_p->widebandCQIValueCW1,
                        dlCQIInfo_p->previousTimeAveragedWBCQICW1);

                /* Correcting wideband CQI and MCS from BLER */
                dlCQIInfo_p->widebandCQICodeWordTwo =  getEffectiveDLCQI(
                        dlCQIInfo_p->previousTimeAveragedWBCQICW1,
                        dlCQIInfo_p->cqiCorrectionFactorForCW1, dlUEContext_p);
                dlCQIInfo_p->mcsIndexCodeWordTwo =
               getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordTwo
                                     );
                /* SPR 4769 changes end */
                /*SPR 6424 Fix Start*/

                /* Cyclomatic_complexity_changes_start */
                getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordTwo,
                        &(dlCQIInfo_p->modulationSchemeCodeWordTwo));
                /* Cyclomatic_complexity_changes_end */

                /*SPR 6424 Fix End*/
                /* SPR 13855 fix start */
                if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
                {
                    updateDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo,
                            internalCellIndex);
                }
                else
                {
                    macUpdateSCellDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo,
                            /* +- SPR 17777 */
                            servCellIndex);

                }
                /* SPR 13855 fix end */

                LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                        getCurrentTick(),dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordOne,
                        dlCQIInfo_p->mcsIndexCodeWordOne,
                        dlCQIInfo_p->widebandCQICodeWordTwo,
                        dlCQIInfo_p->mcsIndexCodeWordTwo,DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,__func__,"WB_BOTH_CW");
            }
            else 
            {

                LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                        getCurrentTick(),dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordOne,
                        dlCQIInfo_p->mcsIndexCodeWordOne,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,__func__,"WB_CW0");
            }
            /* - CQI_5.0 */ 
        }
        else
        { 
            LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                    getCurrentTick(),dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,
                    dlCQIInfo_p->mcsIndexCodeWordOne,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,__func__,"WB_CW0");
        }
    }
    return;
    /* - CQI_4.1 */
}

/*****************************************************************************
 * Function Name  : processCQIMode11Type2
 * Inputs         : periodicMode11Type2_p - This holds pointer to 
 *                                          PeriodicMode11Type2 structure,
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the windBandCQIIndex in 
 *                  DLCQIInfo structure for CodeWord One and Two.
 *****************************************************************************/
/* CA Changes start */ 
STATIC  void processCQIMode11Type2(
        PeriodicMode11Type2 *periodicMode11Type2_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex)
/* CA Changes end */ 
{
    DLCQIInfo *dlCQIInfo_p = PNULL;
    /* CA_phase2_csi_code Start */
    DLMIMOInfo *dlMimoInfo_p;
    UInt8 *cbsrValue;
    UInt8 servCellIndex = 1;
    /* CA_phase2_csi_code End */

    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;
    }
    /* CA_phase2_csi_code Start */
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        dlMimoInfo_p = &(dlUEContext_p->dlMIMOInfo);
        cbsrValue =  dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue;
    }
    else
    {
        dlMimoInfo_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo);
        cbsrValue = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
            codebookSubsetRestrictionv1020.cbsrValue;
    }
    /* CA_phase2_csi_code End */

    /*+EICIC .*/
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    CHECK_PERIODIC_SECONDARY_CONDITION_FOR_CSI (ulUEContext_p,
            internalCellIndex)
    {
        /*If ABS Criteria */
        dlCQIInfo_p->isWideBandReportAvailable_2 = 1;
        dlCQIInfo_p->isSubBandReportAvailable_2 = 0;
        /* TDD SB_CQI */
        /* + CQI_5.0 */
        /* if riUpdationFlag value is LATER_DL_RI_UPDATE then update dlRi */
        if (LATER_DL_RI_UPDATE == dlCQIInfo_p->riUpdationFlag_2)
        {
            /* SPR 21996 Fix Start */
            dlMimoInfo_p->dlRi_2 = dlCQIInfo_p->dlRiToUpdate;
            /* SPR 21996 Fix End */
            dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
            /*SPR 9242 FIX START */
            dlCQIInfo_p->previousTimeAveragedWBCQICW1_2 =
                dlCQIInfo_p->previousTimeAveragedWBCQICW0_2;
            dlCQIInfo_p->cqiCorrectionFactorForCW1_2 =
                dlCQIInfo_p->cqiCorrectionFactorForCW0_2;
            /*SPR 9242 FIX END */
        }
        /* - CQI_5.0 */

        /* SPR 4769 changes start */
        /* Time Averaging */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0_2 =
            calculateAverageDLCQI (periodicMode11Type2_p->widebandCQIValueCodeword0,
                    dlCQIInfo_p->previousTimeAveragedWBCQICW0_2);

        /* Correcting wideband CQI and MCS for CW1 from BLER */
        dlCQIInfo_p->widebandCQICodeWordOne_2 =
            getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW0_2,
                    dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                    dlUEContext_p);

        dlCQIInfo_p->mcsIndexCodeWordOne_2 =
            getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordOne_2
                          );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start */
        getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordOne_2,
                &(dlCQIInfo_p->modulationSchemeCodeWordOne_2));
        /*SPR 6424 Fix End */

        /* ICIC chnages start */
        /* CA Changes start */
        /* CA_phase2_csi_code Start */
        if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
        {
            updateDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex, servCellIndex);
        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */
        /* ICIC changes end */

        if (0 != periodicMode11Type2_p->widebandCQIValueCodeword1)
        {
            /* SPR 4769 changes start */
            /* Time Averaging */
            dlCQIInfo_p->previousTimeAveragedWBCQICW1_2 =
                calculateAverageDLCQI (periodicMode11Type2_p->
                        widebandCQIValueCodeword1,
                        dlCQIInfo_p->previousTimeAveragedWBCQICW1_2);

            /* Correcting wideband CQI and MCS for CW2 from BLER */
            dlCQIInfo_p->widebandCQICodeWordTwo_2 =
                getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW1_2,
                        dlCQIInfo_p->cqiCorrectionFactorForCW1_2,
                        dlUEContext_p);

            dlCQIInfo_p->mcsIndexCodeWordTwo_2 =
                getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordTwo_2
                              );
            /* SPR 4769 changes end */
            /*SPR 6424 Fix Start */
            getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordTwo_2,
                    &(dlCQIInfo_p->
                        modulationSchemeCodeWordTwo_2));
            /*SPR 6424 Fix End */

            /* ICIC chnages start */
            /* CA Changes start */
            /* CA_phase2_csi_code Start */
            if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
            {
                updateDLSINRCW1_2 (dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordTwo_2,
                        internalCellIndex);
            }
            else
            {
                macUpdateSCellDLSINRCW1_2 (dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordTwo_2,
                        internalCellIndex, servCellIndex);
            }
            /* CA_phase2_csi_code End */
            /* CA Changes end */
            /* ICIC changes end */

            LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                    getCurrentTick (), dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    dlCQIInfo_p->mcsIndexCodeWordOne_2,
                    dlCQIInfo_p->widebandCQICodeWordTwo_2,
                    dlCQIInfo_p->mcsIndexCodeWordTwo_2, DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE, __func__, "WB_BOTH_CW_ABS");
        }
        else
        {
            LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                    getCurrentTick (), dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    dlCQIInfo_p->mcsIndexCodeWordOne_2, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE, __func__, "WB_CW0_ABS");
        }

        /* - CQI_4.1 */
        /* SS_S2 */
        /* + TM7_8 Changes Start */
        /* In case of TM8, wideBandPMIIndex is filled from higher layer precoding value 
         * i.e. precodingIndex instead of latest reported PMI value from UE */
        if (TX_MODE_8 == dlMimoInfo_p->transmissionMode)
        {
            dlCQIInfo_p->wideBandPMIIndex = dlMimoInfo_p->precodingIndex;
        }
        else
        {
            /* 4x4 DL MIMO CHG START */
            if (isPMIInCodebookSubsetRestriction
                    /* +- SPR 17777 */
                    (
                     cbsrValue
#ifdef CSI_VALIDATION
                     ,periodicMode11Type2_p->widebandPMI, dlMimoInfo_p->dlRi_2,
                     dlUEContext_p->ueCategory, dlMimoInfo_p->transmissionMode,
                     cellSpecificParams_g.
                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->
                     numOfTxAnteenas
#endif
                     ))
                /* +- SPR 17777 */
            {
                dlCQIInfo_p->wideBandPMIIndex_2 =
                    periodicMode11Type2_p->widebandPMI;
            }
            /*KLOCKWORK_CHANGES_START_4*/
#ifdef CSI_VALIDATION
            /*KLOCKWORK_CHANGES_START_4*/
            else
            {
                LOG_MAC_MSG (MAC_RECEIVED_PMI_LOG_ID, LOGERROR, MAC_L1_INF,
                        getCurrentTick (), dlUEContext_p->ueCategory,
                        dlMimoInfo_p->transmissionMode,
                        dlMimoInfo_p->dlRi_2,
                        periodicMode11Type2_p->widebandPMI,
                        DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE, __func__,
                        "Wrong Periodic PMI Received for Mode11Type2");
            }
            /*KLOCKWORK_CHANGES_START_4*/
#endif
            /*KLOCKWORK_CHANGES_STOP_4*/
            /* 4x4 DL MIMO CHG END */
        }
        /* - TM7_8 Changes End */

        ulUEContext_p->isDLWideBandCQIReportAvailable_2 = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne_2 =
            periodicMode11Type2_p->widebandCQIValueCodeword0;
        ulUEContext_p->dlWidebandCQICodeWordTwo_2 =
            periodicMode11Type2_p->widebandCQIValueCodeword1;
    }
    else
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne =
            periodicMode11Type2_p->widebandCQIValueCodeword0;
        ulUEContext_p->dlWidebandCQICodeWordTwo =
            periodicMode11Type2_p->widebandCQIValueCodeword1;


        dlCQIInfo_p->isWideBandReportAvailable = 1;
        dlCQIInfo_p->isSubBandReportAvailable = 0;
        /* SS_S2 */

        /* TDD SB_CQI */
        /* + CQI_5.0 */
        /* if riUpdationFlag value is LATER_DL_RI_UPDATE then update dlRi */
        if ( LATER_DL_RI_UPDATE == dlCQIInfo_p->riUpdationFlag )
        {
            /* SPR 21996 Fix Start */
            dlMimoInfo_p->dlRi = dlCQIInfo_p->dlRiToUpdate;
            /* SPR 21996 Fix End */
            dlCQIInfo_p->riUpdationFlag =IMMEDIATE_DL_RI_UPDATE;
            /*SPR 9242 FIX START*/
            dlCQIInfo_p->previousTimeAveragedWBCQICW1 = dlCQIInfo_p->previousTimeAveragedWBCQICW0;
            dlCQIInfo_p->cqiCorrectionFactorForCW1 = dlCQIInfo_p->cqiCorrectionFactorForCW0; 
            /*SPR 9242 FIX END*/
        }   
        /* - CQI_5.0 */

        /* SPR 4769 changes start */
        /* Time Averaging */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0 = calculateAverageDLCQI(
                periodicMode11Type2_p->widebandCQIValueCodeword0,
                dlCQIInfo_p->previousTimeAveragedWBCQICW0);

        /* Correcting wideband CQI and MCS for CW1 from BLER */
        dlCQIInfo_p->widebandCQICodeWordOne = getEffectiveDLCQI(
                dlCQIInfo_p->previousTimeAveragedWBCQICW0,
                dlCQIInfo_p->cqiCorrectionFactorForCW0, dlUEContext_p);

    dlCQIInfo_p->mcsIndexCodeWordOne = getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordOne
                                     );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start*/

        /* Cyclomatic_complexity_changes_start */
        getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordOne,
                &(dlCQIInfo_p->modulationSchemeCodeWordOne));
        /* Cyclomatic_complexity_changes_end */

        /*SPR 6424 Fix End*/

        /* ICIC chnages start */
        /* CA Changes start */ 
        /* CA_phase2_csi_code Start */
        if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
        {
            updateDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                    internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                    /* +- SPR 17777 */
                    servCellIndex);
        }
        /* CA_phase2_csi_code End */
        /* CA Changes end */ 
        /* ICIC changes end */

        if ( 0 != periodicMode11Type2_p->widebandCQIValueCodeword1 )
        {
            /* SPR 4769 changes start */
            /* Time Averaging */
            dlCQIInfo_p->previousTimeAveragedWBCQICW1 = calculateAverageDLCQI(
                    periodicMode11Type2_p->widebandCQIValueCodeword1,
                    dlCQIInfo_p->previousTimeAveragedWBCQICW1);

            /* Correcting wideband CQI and MCS for CW2 from BLER */
            dlCQIInfo_p->widebandCQICodeWordTwo = getEffectiveDLCQI(
                    dlCQIInfo_p->previousTimeAveragedWBCQICW1,
                    dlCQIInfo_p->cqiCorrectionFactorForCW1, dlUEContext_p);

        dlCQIInfo_p->mcsIndexCodeWordTwo = getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordTwo
                                     );
            /* SPR 4769 changes end */
            /*SPR 6424 Fix Start*/

            /* Cyclomatic_complexity_changes_start */
            getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordTwo,
                    &(dlCQIInfo_p->modulationSchemeCodeWordTwo));
            /* Cyclomatic_complexity_changes_end */

            /*SPR 6424 Fix End*/

            /* ICIC chnages start */
            /* CA Changes start */ 
            /* CA_phase2_csi_code Start */
            if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
            {
                updateDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo,
                        internalCellIndex);
            }
            else
            {
                macUpdateSCellDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->
                        /* +- SPR 17777 */
                        widebandCQICodeWordTwo,servCellIndex);
            }
            /* CA_phase2_csi_code End */
            /* CA Changes end */ 
            /* ICIC changes end */

            LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                    getCurrentTick(),dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,
                    dlCQIInfo_p->mcsIndexCodeWordOne,
                    dlCQIInfo_p->widebandCQICodeWordTwo,
                    dlCQIInfo_p->mcsIndexCodeWordTwo,DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,__func__,"WB_BOTH_CW");
        }
        else
        {
            LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                    getCurrentTick(),dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,
                    dlCQIInfo_p->mcsIndexCodeWordOne,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,__func__,"WB_CW0");
        }    

        /* - CQI_4.1 */
        /* SS_S2 */
        /* + TM7_8 Changes Start */
        /* In case of TM8, wideBandPMIIndex is filled from higher layer precoding value 
         * i.e. precodingIndex instead of latest reported PMI value from UE */
        if(TX_MODE_8 == dlMimoInfo_p->transmissionMode)
        {
            dlCQIInfo_p->wideBandPMIIndex = dlMimoInfo_p->precodingIndex; 
        }
        else
        {
            /* 4x4 DL MIMO CHG START */
            /* +- SPR 17777 */
            if(isPMIInCodebookSubsetRestriction(
                        cbsrValue
#ifdef CSI_VALIDATION
                        ,periodicMode11Type2_p->widebandPMI,
                        dlMimoInfo_p->dlRi,
                        dlUEContext_p->ueCategory,
                        dlMimoInfo_p->transmissionMode,
                        cellSpecificParams_g.cellConfigAndInitParams_p\
                        [internalCellIndex]->cellParams_p->numOfTxAnteenas
#endif
                        ))
                /* +- SPR 17777 */
            {
                dlCQIInfo_p->wideBandPMIIndex = periodicMode11Type2_p->widebandPMI; 
            }
            /*KLOCKWORK_CHANGES_START_4*/
#ifdef CSI_VALIDATION
            /*KLOCKWORK_CHANGES_START_4*/
            else
            {
                LOG_MAC_MSG(MAC_RECEIVED_PMI_LOG_ID,LOGERROR,MAC_L1_INF,\
                        getCurrentTick(),dlUEContext_p->ueCategory,\
                        dlMimoInfo_p->transmissionMode,\
                        dlMimoInfo_p->dlRi,\
                        periodicMode11Type2_p->widebandPMI,\
                        DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                        DEFAULT_FLOAT_VALUE,__func__,\
                        "Wrong Periodic PMI Received for Mode11Type2");
            }
            /*KLOCKWORK_CHANGES_START_4*/
#endif
            /*KLOCKWORK_CHANGES_STOP_4*/
            /* 4x4 DL MIMO CHG END */
        }
        /* - TM7_8 Changes End */
    }

    return;
}

/*****************************************************************************
 * Function Name  : processCQIMode21Type2
 * Inputs         : periodicMode21Type2_p - This holds pointer to 
 *                                          PeriodicMode21Type2 structure
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the windBandCQIIndex in 
 *                  DLCQIInfo structure for CodeWord One and Two.
 *****************************************************************************/
/* CA Changes start */ 
STATIC  void processCQIMode21Type2(
        PeriodicMode21Type2 *periodicMode21Type2_p,
        DLUEContext *dlUEContext_p,InternalCellIndex internalCellIndex)
/* CA Changes end */ 
{
    DLCQIInfo *dlCQIInfo_p = PNULL;
    /* CA_phase2_csi_code Start */
    DLMIMOInfo *dlMimoInfo_p;
    UInt8 *cbsrValue;
    UInt8 servCellIndex = 1;
    /* CA_phase2_csi_code End */

	dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
	if(dlCQIInfo_p==PNULL)
	{
		return;

	}
    /* CA_phase2_csi_code Start */
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        dlMimoInfo_p = &(dlUEContext_p->dlMIMOInfo);
        cbsrValue =  dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue;
    }
    else
    {
        dlMimoInfo_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo);
        cbsrValue = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
            codebookSubsetRestrictionv1020.cbsrValue;
    }
    /* CA_phase2_csi_code End */



    /*+EICIC .*/
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    CHECK_PERIODIC_SECONDARY_CONDITION_FOR_CSI (ulUEContext_p,
            internalCellIndex)
    {    /*If ABS Criteria */
        ulUEContext_p->isDLWideBandCQIReportAvailable_2 = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne_2 =
            periodicMode21Type2_p->widebandCQIValueCodeword0;
        ulUEContext_p->dlWidebandCQICodeWordTwo_2 =
            periodicMode21Type2_p->widebandCQIValueCodeword1;
        dlCQIInfo_p->isWideBandReportAvailable_2 = 1;
        dlCQIInfo_p->isSubBandReportAvailable_2 = 0;
        /* + CQI_5.0 */
        /* if riUpdationFlag value is LATER_DL_RI_UPDATE then update dlRi */
        if (LATER_DL_RI_UPDATE == dlCQIInfo_p->riUpdationFlag_2)
        {
            /* SPR 21996 Fix Start */
            dlMimoInfo_p->dlRi_2 = dlCQIInfo_p->dlRiToUpdate;
            /* SPR 21996 Fix Start */
            dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
            /*SPR 9242 FIX START */
            dlCQIInfo_p->previousTimeAveragedWBCQICW1_2 =
                dlCQIInfo_p->previousTimeAveragedWBCQICW0_2;
            dlCQIInfo_p->cqiCorrectionFactorForCW1_2 =
                dlCQIInfo_p->cqiCorrectionFactorForCW0_2;
            /*SPR 9242 FIX END */
        }
        /* - CQI_5.0 */

        /* SPR 4769 changes start */
        /* Time Averaging */
        dlCQIInfo_p->previousTimeAveragedWBCQICW0_2 =
            calculateAverageDLCQI (periodicMode21Type2_p->widebandCQIValueCodeword0,
                    dlCQIInfo_p->previousTimeAveragedWBCQICW0_2);

        /* Correcting wideband CQI and MCS for CW1 from BLER */
        dlCQIInfo_p->widebandCQICodeWordOne_2 =
            getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW0_2,
                    dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                    dlUEContext_p);

        dlCQIInfo_p->mcsIndexCodeWordOne_2 =
            getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordOne_2
                           );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start */
        getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordOne_2,
                &(dlCQIInfo_p->modulationSchemeCodeWordOne_2));
        /*SPR 6424 Fix End */

        /* ICIC chnages start */
        /* CA Changes start */
        /* CA_phase2_csi_code Start */
        if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
        {
            updateDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex);
        }
        else
        {
            macUupdateSCellDLSINRCW0_2 (dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne_2,
                    internalCellIndex, servCellIndex);
        }
    /* CA_phase2_csi_code End */
        /* CA Changes end */
        /* ICIC changes end */

        /* SS_S2 */
        if (0 != periodicMode21Type2_p->widebandCQIValueCodeword1)
        {
            /* SPR 4769 changes start */
            /* Time Averaging */
            dlCQIInfo_p->previousTimeAveragedWBCQICW1_2 =
                calculateAverageDLCQI (periodicMode21Type2_p->
                        widebandCQIValueCodeword1,
                        dlCQIInfo_p->previousTimeAveragedWBCQICW1_2);

            /* Correcting wideband CQI and MCS for CW2 from BLER */
            dlCQIInfo_p->widebandCQICodeWordTwo_2 =
                getEffectiveDLCQI (dlCQIInfo_p->previousTimeAveragedWBCQICW1_2,
                        dlCQIInfo_p->cqiCorrectionFactorForCW1_2,
                        dlUEContext_p);

            dlCQIInfo_p->mcsIndexCodeWordTwo_2 =
                getMCSFromCQI (dlCQIInfo_p->widebandCQICodeWordTwo_2
                              );
            /* SPR 4769 changes end */
            /*SPR 6424 Fix Start */
            getModSchemeFromMcsIndex (dlCQIInfo_p->mcsIndexCodeWordTwo_2,
                    &(dlCQIInfo_p->modulationSchemeCodeWordTwo_2));
            /*SPR 6424 Fix End */

            /* ICIC chnages start */
            /* CA Changes start */
            /* CA_phase2_csi_code Start */
            if (IS_PCELL_OF_UE (dlUEContext_p, internalCellIndex))
            {
                updateDLSINRCW1_2 (dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordTwo_2,
                        internalCellIndex);
            }
            else
            {
                macUpdateSCellDLSINRCW1_2 (dlUEContext_p->ueIndex,
                        dlCQIInfo_p->widebandCQICodeWordTwo_2,
                        internalCellIndex, servCellIndex);
            }
            /* CA_phase2_csi_code End */
            /* CA Changes end */
            /* ICIC changes end */

            LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                    getCurrentTick (), dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,
                    dlCQIInfo_p->mcsIndexCodeWordOne,
                    dlCQIInfo_p->widebandCQICodeWordTwo,
                    dlCQIInfo_p->mcsIndexCodeWordTwo, DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE, __func__, "WB_BOTH_CW");
        }
        else
        {
            LOG_MAC_MSG (MAC_CORRECTED_WIDEBAND_CQI_LOG_ID, LOGINFO, MAC_CQI,
                    getCurrentTick (), dlUEContext_p->ueIndex,
                    dlCQIInfo_p->widebandCQICodeWordOne,
                    dlCQIInfo_p->mcsIndexCodeWordOne, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE, __func__, "WB_CW0");
        }
        /* - CQI_4.1 */
        /* SS_S2 */
        /* + TM7_8 Changes Start */
        /* In case of TM8, wideBandPMIIndex is filled from higher layer precoding value 
         * i.e. precodingIndex instead of latest reported PMI value from UE */
        if (TX_MODE_8 == dlMimoInfo_p->transmissionMode)
        {
            dlCQIInfo_p->wideBandPMIIndex = dlMimoInfo_p->precodingIndex;
        }
        else
        {
            /* 4x4 DL MIMO CHG START */
            if (isPMIInCodebookSubsetRestriction
                    /* +- SPR 17777 */
                    (
                     cbsrValue
#ifdef CSI_VALIDATION
                     ,periodicMode21Type2_p->widebandPMI, dlMimoInfo_p->dlRi_2,
                     dlUEContext_p->ueCategory, dlMimoInfo_p->transmissionMode,
                     cellSpecificParams_g.
                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->
                     numOfTxAnteenas
#endif
                     ))
                /* +- SPR 17777 */
            {
                dlCQIInfo_p->wideBandPMIIndex_2 =
                    periodicMode21Type2_p->widebandPMI;
            }
            /*KLOCKWORK_CHANGES_START_4*/
#ifdef CSI_VALIDATION
            /*KLOCKWORK_CHANGES_STOP_4*/
            else
            {
                LOG_MAC_MSG (MAC_RECEIVED_PMI_LOG_ID, LOGERROR, MAC_L1_INF,
                        getCurrentTick (), dlUEContext_p->ueCategory,
                        dlMimoInfo_p->transmissionMode,
                        dlMimoInfo_p->dlRi,
                        periodicMode21Type2_p->widebandPMI,
                        DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE, __func__,
                        "Wrong Periodic PMI Received for Mode11Type2");
            }
            /*KLOCKWORK_CHANGES_START_4*/
#endif
            /*KLOCKWORK_CHANGES_STOP_4*/
            /* 4x4 DL MIMO CHG END */
        }
        /* - TM7_8 Changes End */
    }
    else
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne =
            periodicMode21Type2_p->widebandCQIValueCodeword0;
        ulUEContext_p->dlWidebandCQICodeWordTwo =
            periodicMode21Type2_p->widebandCQIValueCodeword1;
        /*-EICIC .*/
    dlCQIInfo_p->isWideBandReportAvailable = 1;
    dlCQIInfo_p->isSubBandReportAvailable = 0;

    /* + CQI_5.0 */
    /* if riUpdationFlag value is LATER_DL_RI_UPDATE then update dlRi */
    if ( LATER_DL_RI_UPDATE == dlCQIInfo_p->riUpdationFlag )
    {
	/* SPR 21996 Fix Start */
        dlMimoInfo_p->dlRi = dlCQIInfo_p->dlRiToUpdate;
	/* SPR 21996 Fix End */
        dlCQIInfo_p->riUpdationFlag =IMMEDIATE_DL_RI_UPDATE;
        /*SPR 9242 FIX START*/
        dlCQIInfo_p->previousTimeAveragedWBCQICW1 = dlCQIInfo_p->previousTimeAveragedWBCQICW0;
        dlCQIInfo_p->cqiCorrectionFactorForCW1 = dlCQIInfo_p->cqiCorrectionFactorForCW0;
        /*SPR 9242 FIX END*/
    }   
    /* - CQI_5.0 */

	/* SPR 4769 changes start */
    /* Time Averaging */
    dlCQIInfo_p->previousTimeAveragedWBCQICW0 = calculateAverageDLCQI(
            periodicMode21Type2_p->widebandCQIValueCodeword0,
            dlCQIInfo_p->previousTimeAveragedWBCQICW0);

    /* Correcting wideband CQI and MCS for CW1 from BLER */
    dlCQIInfo_p->widebandCQICodeWordOne = getEffectiveDLCQI(
            dlCQIInfo_p->previousTimeAveragedWBCQICW0,
            dlCQIInfo_p->cqiCorrectionFactorForCW0, dlUEContext_p);

    dlCQIInfo_p->mcsIndexCodeWordOne =
        getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordOne
                                     );
    /* SPR 4769 changes end */
    /*SPR 6424 Fix Start*/

    /* Cyclomatic_complexity_changes_start */
    getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordOne,
               &(dlCQIInfo_p->modulationSchemeCodeWordOne));
    /* Cyclomatic_complexity_changes_end */

    /*SPR 6424 Fix End*/
  
    /* ICIC chnages start */
     /* CA Changes start */ 
    /* CA_phase2_csi_code Start */
    if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
    {
        updateDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne
                ,internalCellIndex);
    }
    else
    {
        macUupdateSCellDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
                /* +- SPR 17777 */
                servCellIndex);
    }
    /* CA_phase2_csi_code End */
       /* CA Changes end */ 
    /* ICIC changes end */

    /* SS_S2 */ 
    if(0 != periodicMode21Type2_p->widebandCQIValueCodeword1)
    {
		/* SPR 4769 changes start */
        /* Time Averaging */
        dlCQIInfo_p->previousTimeAveragedWBCQICW1 = calculateAverageDLCQI(
                periodicMode21Type2_p->widebandCQIValueCodeword1,
                dlCQIInfo_p->previousTimeAveragedWBCQICW1);

        /* Correcting wideband CQI and MCS for CW2 from BLER */
        dlCQIInfo_p->widebandCQICodeWordTwo = getEffectiveDLCQI(
                dlCQIInfo_p->previousTimeAveragedWBCQICW1,
                dlCQIInfo_p->cqiCorrectionFactorForCW1,
                dlUEContext_p );

        dlCQIInfo_p->mcsIndexCodeWordTwo =
            getMCSFromCQI(dlCQIInfo_p->widebandCQICodeWordTwo
                                     );
        /* SPR 4769 changes end */
       /*SPR 6424 Fix Start*/

        /* Cyclomatic_complexity_changes_start */
        getModSchemeFromMcsIndex(dlCQIInfo_p->mcsIndexCodeWordTwo,
              &(dlCQIInfo_p->modulationSchemeCodeWordTwo));
        /* Cyclomatic_complexity_changes_end */

       /*SPR 6424 Fix End*/

        /* ICIC chnages start */
        /* CA Changes start */ 
        /* CA_phase2_csi_code Start */
        if(IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex))
        {
            updateDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo,
                    internalCellIndex);
        }
        else
        {
            macUpdateSCellDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->
                    /* +- SPR 17777 */
                    widebandCQICodeWordTwo,servCellIndex);
        }
        /* CA_phase2_csi_code End */
       /* CA Changes end */ 
        /* ICIC changes end */

        LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                getCurrentTick(),dlUEContext_p->ueIndex,
                dlCQIInfo_p->widebandCQICodeWordOne,
                dlCQIInfo_p->mcsIndexCodeWordOne,
                dlCQIInfo_p->widebandCQICodeWordTwo,
                dlCQIInfo_p->mcsIndexCodeWordTwo,DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,__func__,"WB_BOTH_CW");
    }
    else
    {
        LOG_MAC_MSG(MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,
                getCurrentTick(),dlUEContext_p->ueIndex,
                dlCQIInfo_p->widebandCQICodeWordOne,
                dlCQIInfo_p->mcsIndexCodeWordOne,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,__func__,"WB_CW0");
    }    
    /* - CQI_4.1 */
    /* SS_S2 */ 
    /* + TM7_8 Changes Start */
    /* In case of TM8, wideBandPMIIndex is filled from higher layer precoding value 
     * i.e. precodingIndex instead of latest reported PMI value from UE */
    if(TX_MODE_8 == dlMimoInfo_p->transmissionMode)
    {
        dlCQIInfo_p->wideBandPMIIndex = dlMimoInfo_p->precodingIndex; 
    }
    else
    {
        /* 4x4 DL MIMO CHG START */
        /* +- SPR 17777 */
        if(isPMIInCodebookSubsetRestriction(
                    cbsrValue
#ifdef CSI_VALIDATION
                    ,periodicMode21Type2_p->widebandPMI,
                                 dlMimoInfo_p->dlRi,
                                 dlUEContext_p->ueCategory,
                                 dlMimoInfo_p->transmissionMode,
                                 cellSpecificParams_g.cellConfigAndInitParams_p\
                    [internalCellIndex]->cellParams_p->numOfTxAnteenas
#endif
                    ))
            /* +- SPR 17777 */
        {
            dlCQIInfo_p->wideBandPMIIndex = periodicMode21Type2_p->widebandPMI; 
        }
/*KLOCKWORK_CHANGES_START_4*/
#ifdef CSI_VALIDATION
/*KLOCKWORK_CHANGES_STOP_4*/
        else
        {
            LOG_MAC_MSG(MAC_RECEIVED_PMI_LOG_ID,LOGERROR,MAC_L1_INF,\
                    getCurrentTick(),dlUEContext_p->ueCategory,\
                    dlMimoInfo_p->transmissionMode,\
                    dlMimoInfo_p->dlRi,\
                    periodicMode21Type2_p->widebandPMI,\
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                    DEFAULT_FLOAT_VALUE,__func__,\
                    "Wrong Periodic PMI Received for Mode11Type2");
        }
/*KLOCKWORK_CHANGES_START_4*/
#endif
/*KLOCKWORK_CHANGES_STOP_4*/
        /* 4x4 DL MIMO CHG END */
        }
    }
    /* - TM7_8 Changes End */
    return;
}

/*****************************************************************************
 * Function Name  : processCQIMode20Type1
 * Inputs         : periodicMode20Type1_p - This holds pointer to 
 *                                          PeriodicMode20Type1 structure
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the relevant information in 
 *                  SubBandToCQInfo structure for codeWord One.
 *****************************************************************************/
/* CA Changes start */ 
STATIC  void processCQIMode20Type1(
        PeriodicMode20Type1 *periodicMode20Type1_p,
        DLUEContext *dlUEContext_p,InternalCellIndex internalCellIndex)
/* CA Changes end */ 
{
    DLCQIInfo *dlCQIInfo_p = PNULL;
    SubBandReportInfo *subBandReportInfo_p = PNULL;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    CQIInfo *cqiInfo_p = PNULL;
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;
    UInt32 subBandNum = 0;
    UInt32 startRBGIndex = 0;
    UInt32 endRBGIndex = 0;
    UInt32 rbgIndexCount = 0;
    /* + CQI_4.1 */  
    UInt32 oldCqiForRBG = 0;
    UInt32 xorBitMap = 0;
    UInt32 previousReportedSubbandBitMap = 0;
    UInt32 newReportedSubbandBitMap = 0;
    UInt32 position = 0;
    UInt32 reportedCQIBitMap = 0;
    UInt8 reportedCQI = 0;/* This will hold the reported CQI corresponding 
                           * to the position comes after Xor subband maps*/
    UInt8 subsetIndex = 0; /* This will hold the subset number */
    UInt8 correctedSubbandCQI = 0;
    UInt8 correctedSubbandMCS = 0;
    UInt16 tempReportedCQIBitmapPerSubset[MAX_SUBSETS] = {0};
    /* - CQI_4.1 */  
    /* + CQI_5.0 */
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
    UInt8 subBandValueDiff = 0;
    /* - CQI_5.0 */

    /* CA Changes start */ 
    SubBandToRBGMap *subBandToRBGMapPeriodic_p =  cqiGlobalParams_gp\
                               [internalCellIndex]->subBandToRBGMapPeriodic; 
    SubBandToRBGMap *subBandToRBGMapAperiodic_p =  cqiGlobalParams_gp\
                             [internalCellIndex]->subBandToRBGMapAperiodic; 
    rat0Info_p = &(dlRAInfo_gp[internalCellIndex] + 
              dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info;
    rat1Info_p = &(dlRAInfo_gp[internalCellIndex] + 
             dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat1Info;

	dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
	if(dlCQIInfo_p==PNULL)
	{
		return;

	}
        /* CA Changes end */ 
    /*+EICIC . */
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    CHECK_PERIODIC_SECONDARY_CONDITION_FOR_CSI (ulUEContext_p,
            internalCellIndex)
    {    /*If ABS Criteria */
        dlCQIInfo_p->isWideBandReportAvailable_2 = 0;
        dlCQIInfo_p->isSubBandReportAvailable_2 = 1;
        subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;

        cqiInfo_p = &subBandReportInfo_p->cqiInfo;

        /* CA Changes start */
        /* + CQI_4.1 */
        previousReportedSubbandBitMap =
            cqiInfo_p->cqiInfoNonSpatialMul.rat0NonSpatialMulCQIInfo.
            reportedSubbandBitMap_2;

        /* SPR 9427 changes start */
        if (3 == periodicMode20Type1_p->bandwidthPart)
        {
            /* SPR 9427 changes end */
            subBandNum = periodicMode20Type1_p->bandwidthPart *
                cqiGlobalParams_gp[internalCellIndex]->numOfSubBandInbandWidthPart +
                periodicMode20Type1_p->lBitLabel;
            /* CA Changes end */

        /*SPR 20899 Fix Start*/
            if (MID_SUBBANDS > subBandNum)
        /*SPR 20899 Fix Stop*/
            {
                /*Check if there exists previous report for the reported CQI
                 *Index for CodeWord One.In this case we need to reset old 
                 *information and then update the new information in the relevant
                 *structure*/
                /* CA Changes start */
                startRBGIndex =
                    subBandToRBGMapPeriodic_p[subBandNum].startRBGIndex;
                endRBGIndex = subBandToRBGMapPeriodic_p[subBandNum].endRBGIndex;
                /* CA Changes end */
                rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                /* + CQI_5.0 */
                /* Cyclomatic_complexity_changes_start */
                checkForAperiodicCQITrigger (currentTTI, dlCQIInfo_p,
                        dlUEContext_p,
                        periodicMode20Type1_p->cqiValue,
                        subBandReportInfo_p->
                        subBandToCQInfo
                        [startRBGIndex].initialReportedCQICW1_2,
                        subBandValueDiff,
                        /*CA Changes start  */
                        internalCellIndex);
                /* Cyclomatic_complexity_changes_end */
                /*CA Changes end  */
                /* - CQI_5.0 */

                while (rbgIndexCount--)
                {
                    subBandToCQInfo_p =
                        &subBandReportInfo_p->subBandToCQInfo[startRBGIndex];
                    oldCqiForRBG = subBandToCQInfo_p->cqiIndexCW1_2;

                    /* SPR 4769 changes start */
                    /* Time Averaging */
                    subBandToCQInfo_p->previousTimeAveragedSBCQICW0_2 =
                        calculateAverageDLCQI (periodicMode20Type1_p->cqiValue,
                                subBandToCQInfo_p->
                                previousTimeAveragedSBCQICW0_2);

                    subBandToCQInfo_p->cqiIndexCW1_2 =
                        getEffectiveDLCQI (subBandToCQInfo_p->
                                previousTimeAveragedSBCQICW0_2,
                                dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                                dlUEContext_p);

                    /* Correcting Subband CQI and MCS */
                    correctedSubbandCQI = subBandToCQInfo_p->cqiIndexCW1_2;
                    correctedSubbandMCS = getMCSFromCQI (correctedSubbandCQI
                                                        );
                    /* SPR 4769 changes end */
                    /*SPR 6424 Fix Start */
                    getModSchemeFromMcsIndex (correctedSubbandMCS,
                            &(dlCQIInfo_p->
                                modulationSchemeCodeWordOne_2));
                    /*SPR 6424 Fix End */

                    /*Update the MCS Index for mapped to reported CQI Index 
                     *Information*/
                    subBandToCQInfo_p->mcsIndexCW1_2 = correctedSubbandMCS;

                    /* SPR 3803 changes start */
                    if (oldCqiForRBG)
                    {
                        /* SPR 3803 changes end */
                        resetRAT0CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQI,
                                oldCqiForRBG,
                                &cqiInfo_p->
                                cqiInfoNonSpatialMul);

                        resetRAT1CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                                correctedSubbandCQI,
                                oldCqiForRBG,
                                &cqiInfo_p->
                                cqiInfoNonSpatialMul,
                                rat0Info_p,
                                rat1Info_p);
                        /* SPR 3803 changes start */
                    }
                    /* SPR 3803 changes end */

                    updateRAT0CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                            correctedSubbandCQI,
                            oldCqiForRBG,
                            &cqiInfo_p->
                            cqiInfoNonSpatialMul);

                    updateRAT1CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                            correctedSubbandCQI,
                            oldCqiForRBG,
                            &cqiInfo_p->
                            cqiInfoNonSpatialMul,
                            rat0Info_p,
                            rat1Info_p,
                            tempReportedCQIBitmapPerSubset);

                    /* Updating the reported subbands and reported CQI.
                     * Here for each RBGindex we are updating  newReportedSubbandBitMap
                     * because number of subbands in aperiodic and periodic are different
                     * so whenever there is transition from periodic to aperiodic
                     * all the rbg's have updated informationi.
                     */
                    newReportedSubbandBitMap |= (UInt32) 1 << startRBGIndex;

                    /* We are firstly updating  CQI in local array because it may happen that 
                     * with in a report new CQI of one RBG is same as old CQI of other RBG 
                     * which may cause new CQI to be reset in reset functions.By taking 
                     * it in local array  information will never reset for the particular report.
                     */

                    SET_CQI_REPORT_BIT (reportedCQIBitMap, correctedSubbandCQI);


                    LOG_MAC_MSG (MAC_PERIODIC_UE_SELECTED_MODE_INFO_LOG_ID, LOGINFO,
                            MAC_CQI, getCurrentTick (), dlUEContext_p->ueIndex,
                            periodicMode20Type1_p->bandwidthPart, subBandNum,
                            correctedSubbandCQI, DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__,
                            "SB_CW0_ABS");

                    startRBGIndex++;
                }
                /* SPR 9427 changes start */
            }
        }
        else
        {
            UInt8 subBandCount =
                subBandReportInfo_p->numOfSBperBWpart[periodicMode20Type1_p->
                bandwidthPart];
            while (subBandCount--)
            {
                /*Check if there exists previous report for the reported CQI
                 *Index for CodeWord One.In this case we need to reset old 
                 *information and then update the new information in the relevant
                 *structure*/
                subBandNum = periodicMode20Type1_p->bandwidthPart *
                    (cqiGlobalParams_gp[internalCellIndex]->
                     numOfSubBandInbandWidthPart) + subBandCount;

        /*SPR 20899 Fix Start*/
                if (MID_SUBBANDS > subBandNum)
        /*SPR 20899 Fix Stop*/
                {
                    startRBGIndex =
                        cqiGlobalParams_gp
                        [internalCellIndex]->subBandToRBGMapPeriodic[subBandNum].
                        startRBGIndex;
                    endRBGIndex =
                        cqiGlobalParams_gp
                        [internalCellIndex]->subBandToRBGMapPeriodic[subBandNum].
                        endRBGIndex;
                    rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                    subBandToCQInfo_p =
                        &subBandReportInfo_p->subBandToCQInfo[startRBGIndex];
                    oldCqiForRBG = subBandToCQInfo_p->cqiIndexCW1_2;

                    /* SPR 4769 changes start */
                    /* Time Averaging */
                    subBandToCQInfo_p->previousTimeAveragedSBCQICW0_2 =
                        calculateAverageDLCQI (periodicMode20Type1_p->cqiValue,
                                subBandToCQInfo_p->
                                previousTimeAveragedSBCQICW0_2);

                    subBandToCQInfo_p->cqiIndexCW1_2 =
                        getEffectiveDLCQI (subBandToCQInfo_p->
                                previousTimeAveragedSBCQICW0_2,
                                dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                                dlUEContext_p);

                    /* Correcting Subband CQI and MCS */
                    correctedSubbandCQI = subBandToCQInfo_p->cqiIndexCW1_2;
                    correctedSubbandMCS = getMCSFromCQI (correctedSubbandCQI
                                                        );
                    /* SPR 4769 changes end */
                    /*SPR 6424 Fix Start */
                    getModSchemeFromMcsIndex (correctedSubbandMCS,
                            &(dlCQIInfo_p->
                                modulationSchemeCodeWordOne_2));
                    /*SPR 6424 Fix End */

                    /*Update the MCS Index for mapped to reported CQI Index 
                     *Information*/
                    subBandToCQInfo_p->mcsIndexCW1_2 = correctedSubbandMCS;

                    /* + CQI_5.0 */
                    /* Cyclomatic_complexity_changes_start */
                    checkForAperiodicCQITrigger (currentTTI, dlCQIInfo_p,
                            dlUEContext_p,
                            periodicMode20Type1_p->cqiValue,
                            subBandReportInfo_p->
                            subBandToCQInfo
                            [startRBGIndex].initialReportedCQICW1_2,
                            subBandValueDiff,
                            internalCellIndex);
                    /* Cyclomatic_complexity_changes_end */
                    /* - CQI_5.0 */

                    while (rbgIndexCount--)
                    {
                        /* SPR 3803 changes start */
                        if (oldCqiForRBG)
                        {
                            /* SPR 3803 changes end */
                            resetRAT0CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                                    correctedSubbandCQI,
                                    oldCqiForRBG,
                                    &cqiInfo_p->
                                    cqiInfoNonSpatialMul);

                            resetRAT1CW1NonSpatialMulSubBandInfo_2 (startRBGIndex,
                                    correctedSubbandCQI,
                                    oldCqiForRBG,
                                    &cqiInfo_p->
                                    cqiInfoNonSpatialMul,
                                    rat0Info_p,
                                    rat1Info_p);
                            /* SPR 3803 changes start */
                        }
                        /* SPR 3803 changes end */


                        LOG_MAC_MSG (MAC_PERIODIC_UE_SELECTED_MODE_INFO_LOG_ID,
                                LOGINFO, MAC_CQI, getCurrentTick (),
                                dlUEContext_p->ueIndex,
                                periodicMode20Type1_p->bandwidthPart, subBandNum,
                                correctedSubbandCQI, DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                __func__, "SB_CW0_For_ABS");

                        startRBGIndex++;
                    }
                }
            }
        }
        /* SPR 9427 changes end */
        /*533 coverity warning 61224 fix- code  */
        /* Xoring previous and newly reported subband bit map */
        xorBitMap = previousReportedSubbandBitMap ^ newReportedSubbandBitMap;

        while (xorBitMap)
        {
            position = FFS (xorBitMap);
            position -= 1;

            /* CA Changes start */
            startRBGIndex = subBandToRBGMapAperiodic_p[position].startRBGIndex;
            endRBGIndex = subBandToRBGMapAperiodic_p[position].endRBGIndex;
            /* CA Changes end */
            rbgIndexCount = endRBGIndex - startRBGIndex + 1;

            reportedCQI =
                subBandReportInfo_p->subBandToCQInfo[startRBGIndex].cqiIndexCW1_2;
            /* setting bit corresponding to CQI reported for RAT0 */
            SET_CQI_REPORT_BIT (reportedCQIBitMap, reportedCQI);

            /* Setting reported Cqi Bitmap for RAT1 */
            /* calculating start and end rbg Index of subband corresponding to 
             * position in the xorBitMap */

            while (rbgIndexCount--)
            {
                subsetIndex =
                    rat0Info_p->rbgIndexInfo[startRBGIndex].subsetIndex_2;
                SET_CQI_REPORT_BIT (cqiInfo_p->cqiInfoNonSpatialMul.
                        rat1NonSpatialMulCQIInfo[subsetIndex].
                        reportedCQIBitMap_2, reportedCQI);
                startRBGIndex++;
            }

            /* setting the reported position in the xor bitmap */
            xorBitMap &= (~((UInt32) 1 << position));
        }

        cqiInfo_p->cqiInfoNonSpatialMul.rat0NonSpatialMulCQIInfo.
            reportedCQIBitMap_2 |= reportedCQIBitMap;
        cqiInfo_p->cqiInfoNonSpatialMul.rat0NonSpatialMulCQIInfo.
            reportedSubbandBitMap_2 |= newReportedSubbandBitMap;
        /* - CQI_4.1 */
    }
    else
    {
    dlCQIInfo_p->isWideBandReportAvailable = 0;
    dlCQIInfo_p->isSubBandReportAvailable = 1;
    subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;
    
    cqiInfo_p = &subBandReportInfo_p->cqiInfo;
    
     /* CA Changes start */ 
    /* + CQI_4.1 */
    previousReportedSubbandBitMap = cqiInfo_p->cqiInfoNonSpatialMul.
                                    rat0NonSpatialMulCQIInfo.reportedSubbandBitMap;
    
    /* SPR 9427 changes start */
    if(3 == periodicMode20Type1_p->bandwidthPart)
    {    
        /* SPR 9427 changes end */
    subBandNum = periodicMode20Type1_p->bandwidthPart * 
        cqiGlobalParams_gp[internalCellIndex]->numOfSubBandInbandWidthPart + 
              periodicMode20Type1_p->lBitLabel;
   /* CA Changes end */ 
    
        /* SPR 18122 Changes Start*/
        if( DIVIDE_BY_TWO(MAX_SUBBANDS) > subBandNum )
        /* SPR 18122 Changes End*/
        {
    /*Check if there exists previous report for the reported CQI
     *Index for CodeWord One.In this case we need to reset old 
     *information and then update the new information in the relevant
     *structure*/
    /* CA Changes start */ 
    startRBGIndex = subBandToRBGMapPeriodic_p[subBandNum].startRBGIndex;
    endRBGIndex = subBandToRBGMapPeriodic_p[subBandNum].endRBGIndex;
    /* CA Changes end */ 
    rbgIndexCount = endRBGIndex - startRBGIndex + 1;
    
    /* + CQI_5.0 */
    /* Cyclomatic_complexity_changes_start */
    checkForAperiodicCQITrigger(currentTTI,dlCQIInfo_p,dlUEContext_p,
                      periodicMode20Type1_p->cqiValue,
                      subBandReportInfo_p->subBandToCQInfo[startRBGIndex].
                      initialReportedCQICW1,subBandValueDiff,
                      /*CA Changes start  */
                      internalCellIndex);
     /* Cyclomatic_complexity_changes_end */
                      /*CA Changes end  */
    /* - CQI_5.0 */

    while(rbgIndexCount--)
    {
        subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[startRBGIndex];
        oldCqiForRBG =  subBandToCQInfo_p->cqiIndexCW1;

        /* SPR 4769 changes start */
        /* Time Averaging */
        subBandToCQInfo_p->previousTimeAveragedSBCQICW0 = calculateAverageDLCQI(
                periodicMode20Type1_p->cqiValue,
                subBandToCQInfo_p->previousTimeAveragedSBCQICW0);

        subBandToCQInfo_p->cqiIndexCW1 =  getEffectiveDLCQI(
                subBandToCQInfo_p->previousTimeAveragedSBCQICW0,
                        dlCQIInfo_p->cqiCorrectionFactorForCW0,dlUEContext_p); 

        /* Correcting Subband CQI and MCS*/
        correctedSubbandCQI = subBandToCQInfo_p->cqiIndexCW1;
        correctedSubbandMCS = getMCSFromCQI(correctedSubbandCQI
                                     );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start*/

        /* Cyclomatic_complexity_changes_start */
        getModSchemeFromMcsIndex(correctedSubbandMCS,
                       &(dlCQIInfo_p->modulationSchemeCodeWordOne));
        /* Cyclomatic_complexity_changes_end */

        /*SPR 6424 Fix End*/

        /*Update the MCS Index for mapped to reported CQI Index 
         *Information*/
        subBandToCQInfo_p->mcsIndexCW1 = correctedSubbandMCS;

        /* SPR 3803 changes start */  
        if (oldCqiForRBG)
        {  
        /* SPR 3803 changes end */  
            resetRAT0CW1NonSpatialMulSubBandInfo(
                    startRBGIndex,
                    correctedSubbandCQI,
                    oldCqiForRBG,
                    &cqiInfo_p->cqiInfoNonSpatialMul);

            resetRAT1CW1NonSpatialMulSubBandInfo(
                    startRBGIndex,
                    correctedSubbandCQI,
                    oldCqiForRBG,
                    &cqiInfo_p->cqiInfoNonSpatialMul,
                    rat0Info_p,
                    rat1Info_p);
        /* SPR 3803 changes start */  
        }
        /* SPR 3803 changes end */  

        updateRAT0CW1NonSpatialMulSubBandInfo(
                startRBGIndex,
                correctedSubbandCQI,
                oldCqiForRBG,
                &cqiInfo_p->cqiInfoNonSpatialMul);

        updateRAT1CW1NonSpatialMulSubBandInfo(
                startRBGIndex,
                correctedSubbandCQI,
                oldCqiForRBG,
                &cqiInfo_p->cqiInfoNonSpatialMul,
                rat0Info_p,
                rat1Info_p,
                tempReportedCQIBitmapPerSubset);

        /* Updating the reported subbands and reported CQI.
         * Here for each RBGindex we are updating  newReportedSubbandBitMap
         * because number of subbands in aperiodic and periodic are different
         * so whenever there is transition from periodic to aperiodic
         * all the rbg's have updated informationi.
         */
        newReportedSubbandBitMap |= (UInt32)1 << startRBGIndex;
         
        /* We are firstly updating  CQI in local array because it may happen that 
         * with in a report new CQI of one RBG is same as old CQI of other RBG 
         * which may cause new CQI to be reset in reset functions.By taking 
         * it in local array  information will never reset for the particular report.
         */
 
        SET_CQI_REPORT_BIT(reportedCQIBitMap,correctedSubbandCQI);      
        

        LOG_MAC_MSG(MAC_PERIODIC_UE_SELECTED_MODE_INFO_LOG_ID,LOGINFO,MAC_CQI,
                getCurrentTick(),dlUEContext_p->ueIndex,
                periodicMode20Type1_p->bandwidthPart,subBandNum,
                correctedSubbandCQI,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,__func__,"SB_CW0");

        startRBGIndex++;
    }
            /* SPR 9427 changes start */
        }
    }
    else
    {
        UInt8 subBandCount = subBandReportInfo_p->numOfSBperBWpart[periodicMode20Type1_p->bandwidthPart];
        while(subBandCount--)
        {    
            /*Check if there exists previous report for the reported CQI
             *Index for CodeWord One.In this case we need to reset old 
             *information and then update the new information in the relevant
             *structure*/
            subBandNum = periodicMode20Type1_p->bandwidthPart * 
                (cqiGlobalParams_gp[internalCellIndex]\
                 ->numOfSubBandInbandWidthPart) + subBandCount;
            /* SPR 18122 Changes Start*/
            if( DIVIDE_BY_TWO(MAX_SUBBANDS) > subBandNum )
            /* SPR 18122 Changes End*/
            {
                startRBGIndex = 
                    cqiGlobalParams_gp[internalCellIndex]->\
                    subBandToRBGMapPeriodic[subBandNum].startRBGIndex;
                endRBGIndex = 
                    cqiGlobalParams_gp[internalCellIndex]->\
                    subBandToRBGMapPeriodic[subBandNum].endRBGIndex;
                rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[startRBGIndex];
                oldCqiForRBG =  subBandToCQInfo_p->cqiIndexCW1;

                /* SPR 4769 changes start */
                /* Time Averaging */
                subBandToCQInfo_p->previousTimeAveragedSBCQICW0 = calculateAverageDLCQI(
                        periodicMode20Type1_p->cqiValue,
                        subBandToCQInfo_p->previousTimeAveragedSBCQICW0);

                subBandToCQInfo_p->cqiIndexCW1 =  getEffectiveDLCQI(
                        subBandToCQInfo_p->previousTimeAveragedSBCQICW0,
                        dlCQIInfo_p->cqiCorrectionFactorForCW0,dlUEContext_p); 

                /* Correcting Subband CQI and MCS*/
                correctedSubbandCQI = subBandToCQInfo_p->cqiIndexCW1;
                correctedSubbandMCS = getMCSFromCQI(correctedSubbandCQI
                                     );
                /* SPR 4769 changes end */
                /*SPR 6424 Fix Start*/

                /* Cyclomatic_complexity_changes_start */
                getModSchemeFromMcsIndex(correctedSubbandMCS,
                        &(dlCQIInfo_p->modulationSchemeCodeWordOne));
                /* Cyclomatic_complexity_changes_end */

                /*SPR 6424 Fix End*/

                /*Update the MCS Index for mapped to reported CQI Index 
                 *Information*/
                subBandToCQInfo_p->mcsIndexCW1 = correctedSubbandMCS;

                /* + CQI_5.0 */
                /* Cyclomatic_complexity_changes_start */
                checkForAperiodicCQITrigger(currentTTI,dlCQIInfo_p,dlUEContext_p,
                        periodicMode20Type1_p->cqiValue,
                        subBandReportInfo_p->subBandToCQInfo[startRBGIndex].
                        initialReportedCQICW1,subBandValueDiff,
                        internalCellIndex);
                /* Cyclomatic_complexity_changes_end */
                /* - CQI_5.0 */

                while(rbgIndexCount--)
                {
                    /* SPR 3803 changes start */  
                    if (oldCqiForRBG)
                    {  
                        /* SPR 3803 changes end */  
                        resetRAT0CW1NonSpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQI,
                                oldCqiForRBG,
                                &cqiInfo_p->cqiInfoNonSpatialMul);

                        resetRAT1CW1NonSpatialMulSubBandInfo(
                                startRBGIndex,
                                correctedSubbandCQI,
                                oldCqiForRBG,
                                &cqiInfo_p->cqiInfoNonSpatialMul,
                                rat0Info_p,
                                rat1Info_p);
                        /* SPR 3803 changes start */  
                    }
                    /* SPR 3803 changes end */  

                    updateRAT0CW1NonSpatialMulSubBandInfo(
                        startRBGIndex,
                        correctedSubbandCQI,
                        oldCqiForRBG,
                        &cqiInfo_p->cqiInfoNonSpatialMul);

                    updateRAT1CW1NonSpatialMulSubBandInfo(
                        startRBGIndex,
                        correctedSubbandCQI,
                        oldCqiForRBG,
                        &cqiInfo_p->cqiInfoNonSpatialMul,
                        rat0Info_p,
                        rat1Info_p,
                        tempReportedCQIBitmapPerSubset);

                   /* Updating the reported subbands and reported CQI.
                    * Here for each RBGindex we are updating  newReportedSubbandBitMap
                    * because number of subbands in aperiodic and periodic are different
                    * so whenever there is transition from periodic to aperiodic
                    * all the rbg's have updated informationi.
                    */
                    newReportedSubbandBitMap |= (UInt32)1 << startRBGIndex;

                    /* We are firstly updating  CQI in local array because it may happen that 
                     * with in a report new CQI of one RBG is same as old CQI of other RBG 
                     * which may cause new CQI to be reset in reset functions.By taking 
                     * it in local array  information will never reset for the particular report.
                     */

                    SET_CQI_REPORT_BIT(reportedCQIBitMap,correctedSubbandCQI);      

                    LOG_MAC_MSG(MAC_PERIODIC_UE_SELECTED_MODE_INFO_LOG_ID,LOGINFO,MAC_CQI,
                            getCurrentTick(),dlUEContext_p->ueIndex,
                            periodicMode20Type1_p->bandwidthPart,subBandNum,
                            correctedSubbandCQI,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,__func__,"SB_CW0");

                    startRBGIndex++;
                }
            }  
        }
    }
    /* SPR 9427 changes end */
    /*533 coverity warning 61224 fix- code indented */
    /* Xoring previous and newly reported subband bit map*/
    xorBitMap = previousReportedSubbandBitMap ^ newReportedSubbandBitMap;
      
    while( xorBitMap )
    {
        position = FFS(xorBitMap);
        position -= 1;

        /* CA Changes start */ 
        startRBGIndex = subBandToRBGMapAperiodic_p[position].startRBGIndex;
        endRBGIndex = subBandToRBGMapAperiodic_p[position].endRBGIndex;
        /* CA Changes end */ 
        rbgIndexCount = endRBGIndex - startRBGIndex + 1; 
        
        reportedCQI = subBandReportInfo_p->subBandToCQInfo[startRBGIndex].
                                                                  cqiIndexCW1;
        /* setting bit corresponding to CQI reported for RAT0*/
        SET_CQI_REPORT_BIT(reportedCQIBitMap,reportedCQI);

        /* Setting reported Cqi Bitmap for RAT1 */
        /* calculating start and end rbg Index of subband corresponding to 
         * position in the xorBitMap */

        while(rbgIndexCount--)
        {
            subsetIndex = rat0Info_p->rbgIndexInfo[startRBGIndex].subsetIndex; 
            SET_CQI_REPORT_BIT(cqiInfo_p->cqiInfoNonSpatialMul.
               rat1NonSpatialMulCQIInfo[subsetIndex].reportedCQIBitMap,
                    reportedCQI);
            startRBGIndex++;    
        }  

        /* setting the reported position in the xor bitmap */
        xorBitMap &=(~((UInt32)1 << position));
    }    
    
    cqiInfo_p->cqiInfoNonSpatialMul.rat0NonSpatialMulCQIInfo.reportedCQIBitMap
                                                  |=  reportedCQIBitMap;
    cqiInfo_p->cqiInfoNonSpatialMul.rat0NonSpatialMulCQIInfo.
                             reportedSubbandBitMap |= newReportedSubbandBitMap;
   /* - CQI_4.1 */
   }
}

/********************************************************************************
 * Function Name  : processCQIMode21Type1
 * Inputs         : periodicMode21Type1_p - This holds pointer to 
 *                                          PeriodicMode21Type1 structure,
 *                  dlUEContext_p - This holds pointer to DLUEContext
 *                                  structure
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the relevant information in 
 *                  SubBandToCQInfo structure for codeWord One and CodeWord Two.
 *******************************************************************************/
/* CA Changes start */ 
STATIC  void processCQIMode21Type1(
        PeriodicMode21Type1 *periodicMode21Type1_p,
        DLUEContext *dlUEContext_p
        ,InternalCellIndex internalCellIndex)
/* CA Changes end */ 
{
    DLCQIInfo *dlCQIInfo_p = PNULL;
    SubBandReportInfo *subBandReportInfo_p = PNULL;
    CQIInfo *cqiInfo_p = PNULL;
    RAT0Info *rat0Info_p = PNULL;
    RAT1Info *rat1Info_p = PNULL;
    UInt32 subBandNum = 0;
    UInt32 startRBGIndex = 0;
    UInt32 endRBGIndex = 0;
    UInt32 rbgIndexCount = 0;
    /* + CQI_4.1 */
    UInt8 toProcessCW2 = FALSE;
    /* - CQI_4.1 */
    /* + CQI_5.0 */
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
    UInt8 subBandValueDiff = 0;
    /* - CQI_5.0 */

    /* CA Changes start */ 
    SubBandToRBGMap *subBandToRBGMapPeriodic_p =  cqiGlobalParams_gp\
                                                  [internalCellIndex]->subBandToRBGMapPeriodic; 
    rat0Info_p = &(dlRAInfo_gp[internalCellIndex] + 
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info;
    rat1Info_p = &(dlRAInfo_gp[internalCellIndex] +
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat1Info;
    UInt8 numOfTxAnteenas =  cellSpecificParams_g.cellConfigAndInitParams_p
        [internalCellIndex]->cellParams_p->numOfTxAnteenas;
    /* CA Changes end */ 
    /* CA_phase2_csi_code Start */
    DLMIMOInfo *dlMimoInfo_p;
    /* CA_phase2_csi_code End */

    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p==PNULL)
    {
        return;

    }
    /* CA_phase2_csi_code Start */
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        dlMimoInfo_p = &(dlUEContext_p->dlMIMOInfo);
    }
    else
    {
        dlMimoInfo_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo);
    }
    /* CA_phase2_csi_code End */
    /*+EICIC . */
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;
    CHECK_PERIODIC_SECONDARY_CONDITION_FOR_CSI (ulUEContext_p,
            internalCellIndex)
    {    /*If ABS Criteria */
        dlCQIInfo_p->isWideBandReportAvailable_2 = 0;

        dlCQIInfo_p->isSubBandReportAvailable_2 = 1;
        subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;

        cqiInfo_p = &subBandReportInfo_p->cqiInfo;

        /* + CQI_5.0 */
        /* if riUpdationFlag value is LATER_DL_RI_UPDATE then update dlRi */
        if (LATER_DL_RI_UPDATE == dlCQIInfo_p->riUpdationFlag_2)
        {
	    /* SPR 21996 Fix Start */
            dlMimoInfo_p->dlRi_2 = dlCQIInfo_p->dlRiToUpdate;
	    /* SPR 21996 Fix End */
            dlCQIInfo_p->riUpdationFlag_2 = IMMEDIATE_DL_RI_UPDATE;
        }
        /* - CQI_5.0 */

        /* SPR 9427 changes start */
        if (3 == periodicMode21Type1_p->bandwidthPart)
        {
            /* SPR 9427 changes end */
            subBandNum = periodicMode21Type1_p->bandwidthPart *
                cqiGlobalParams_gp[internalCellIndex]->numOfSubBandInbandWidthPart +
                periodicMode21Type1_p->lBitLabel;

        /*SPR 20899 Fix Start*/
            if (MID_SUBBANDS > subBandNum)
        /*SPR 20899 Fix Stop*/
            {
                /*Check if there exists previous report for the reported CQI
                 *Index for CodeWord One.In this case we need to reset old 
                 *information and then update the new information in the relevant
                 *structure*/
                startRBGIndex =
                    subBandToRBGMapPeriodic_p[subBandNum].startRBGIndex;
                endRBGIndex = subBandToRBGMapPeriodic_p[subBandNum].endRBGIndex;
                /* CA Changes end */
                rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                /* + CQI_5.0 */
                /* Cyclomatic_complexity_changes_start */
                checkForAperiodicCQITrigger (currentTTI, dlCQIInfo_p,
                        dlUEContext_p,
                        periodicMode21Type1_p->
                        cqiValueCodeword0,
                        subBandReportInfo_p->
                        subBandToCQInfo
                        [startRBGIndex].initialReportedCQICW1_2,
                        subBandValueDiff,
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */
                /* Cyclomatic_complexity_changes_end */
                /* - CQI_5.0 */

                if (0 != periodicMode21Type1_p->cqiValueCodeword1)
                {
                    toProcessCW2 = TRUE;

                    /* + CQI_5.0 */
                    /* Cyclomatic_complexity_changes_start */
                    checkForAperiodicCQITrigger (currentTTI, dlCQIInfo_p,
                            dlUEContext_p,
                            periodicMode21Type1_p->
                            cqiValueCodeword1,
                            subBandReportInfo_p->
                            subBandToCQInfo
                            [startRBGIndex].initialReportedCQICW2_2,
                            subBandValueDiff,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    /* Cyclomatic_complexity_changes_end */
                    /* - CQI_5.0 */
                }

                /* Cyclomatic_complexity_changes_start */
                updateSubBandInforForCW0AndCW1SpatialMul_2 (rbgIndexCount,
                        startRBGIndex,
                        subBandReportInfo_p,
                        periodicMode21Type1_p,
                        dlCQIInfo_p,
                        cqiInfo_p,
                        rat0Info_p,
                        rat1Info_p,
                        numOfTxAnteenas,
                        toProcessCW2,
                        dlUEContext_p
                        );
                /* Cyclomatic_complexity_changes_end */

                /* SPR 9427 changes start */
            }
        }
        else
        {
            UInt8 subBandCount =
                subBandReportInfo_p->numOfSBperBWpart[periodicMode21Type1_p->
                bandwidthPart];
            while (subBandCount--)
            {
                /*Check if there exists previous report for the reported CQI
                 *Index for CodeWord One.In this case we need to reset old 
                 *information and then update the new information in the relevant
                 *structure*/

                subBandNum = periodicMode21Type1_p->bandwidthPart *
                    cqiGlobalParams_gp[internalCellIndex]->
                    numOfSubBandInbandWidthPart + subBandCount;

        /*SPR 20899 Fix Start*/
                if (MID_SUBBANDS > subBandNum)
        /*SPR 20899 Fix Stop*/
                {
                    startRBGIndex =
                        cqiGlobalParams_gp
                        [internalCellIndex]->subBandToRBGMapAperiodic[subBandNum].
                        startRBGIndex;
                    endRBGIndex =
                        cqiGlobalParams_gp
                        [internalCellIndex]->subBandToRBGMapAperiodic[subBandNum].
                        endRBGIndex;
                    rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                    /* + CQI_5.0 */
                    /* Cyclomatic_complexity_changes_start */
                    checkForAperiodicCQITrigger (currentTTI, dlCQIInfo_p,
                            dlUEContext_p,
                            periodicMode21Type1_p->
                            cqiValueCodeword0,
                            subBandReportInfo_p->
                            subBandToCQInfo
                            [startRBGIndex].initialReportedCQICW1_2,
                            subBandValueDiff,
                            internalCellIndex);
                    /* Cyclomatic_complexity_changes_end */
                    /* - CQI_5.0 */

                    if (0 != periodicMode21Type1_p->cqiValueCodeword1)
                    {
                        toProcessCW2 = TRUE;

                        /* + CQI_5.0 */
                        /* Cyclomatic_complexity_changes_start */
                        checkForAperiodicCQITrigger (currentTTI, dlCQIInfo_p,
                                dlUEContext_p,
                                periodicMode21Type1_p->
                                cqiValueCodeword1,
                                subBandReportInfo_p->
                                subBandToCQInfo
                                [startRBGIndex].initialReportedCQICW2_2,
                                subBandValueDiff,
                                internalCellIndex);
                        /* Cyclomatic_complexity_changes_end */
                        /* - CQI_5.0 */
                    }

                    /* Cyclomatic_complexity_changes_start */
                    updateSubBandInforForCW0AndCW1SpatialMul_2 (rbgIndexCount,
                            startRBGIndex,
                            subBandReportInfo_p,
                            periodicMode21Type1_p,
                            dlCQIInfo_p,
                            cqiInfo_p,
                            rat0Info_p,
                            rat1Info_p,
                            numOfTxAnteenas,
                            toProcessCW2,
                            dlUEContext_p
                            );
                    /* Cyclomatic_complexity_changes_end */

                }
            }
        }
        /* SPR 9427 changes end */
    }
    else
    {

        dlCQIInfo_p->isWideBandReportAvailable = 0;

        dlCQIInfo_p->isSubBandReportAvailable = 1;
        subBandReportInfo_p = dlCQIInfo_p->subBandReportInfo_p;

        cqiInfo_p = &subBandReportInfo_p->cqiInfo;

        /* + CQI_5.0 */
        /* if riUpdationFlag value is LATER_DL_RI_UPDATE then update dlRi */
        if ( LATER_DL_RI_UPDATE == dlCQIInfo_p->riUpdationFlag )
        {
	    /* SPR 21996 Fix Start */
            dlMimoInfo_p->dlRi = dlCQIInfo_p->dlRiToUpdate;
	    /* SPR 21996 Fix End */
            dlCQIInfo_p->riUpdationFlag =  IMMEDIATE_DL_RI_UPDATE; 
        }   
        /* - CQI_5.0 */

        /* SPR 9427 changes start */
        if(3 == periodicMode21Type1_p->bandwidthPart)
        {    
            /* SPR 9427 changes end */
            subBandNum = periodicMode21Type1_p->bandwidthPart * 
                cqiGlobalParams_gp[internalCellIndex]->numOfSubBandInbandWidthPart + 
                periodicMode21Type1_p->lBitLabel;

            /* SPR 18122 Changes Start*/
            if( DIVIDE_BY_TWO(MAX_SUBBANDS) > subBandNum )
            /* SPR 18122 Changes End*/
            {
                /*Check if there exists previous report for the reported CQI
                 *Index for CodeWord One.In this case we need to reset old 
                 *information and then update the new information in the relevant
                 *structure*/
                startRBGIndex = subBandToRBGMapPeriodic_p[subBandNum].startRBGIndex;
                endRBGIndex = subBandToRBGMapPeriodic_p[subBandNum].endRBGIndex;
                /* CA Changes end */ 
                rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                /* + CQI_5.0 */
                /* Cyclomatic_complexity_changes_start */
                checkForAperiodicCQITrigger(currentTTI,dlCQIInfo_p,dlUEContext_p,
                        periodicMode21Type1_p->cqiValueCodeword0,
                        subBandReportInfo_p->subBandToCQInfo[startRBGIndex].
                        initialReportedCQICW1,subBandValueDiff,
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */
                /* Cyclomatic_complexity_changes_end */
                /* - CQI_5.0 */

                if (0 != periodicMode21Type1_p->cqiValueCodeword1)
                { 
                    toProcessCW2 = TRUE;

                    /* + CQI_5.0 */
                    /* Cyclomatic_complexity_changes_start */
                    checkForAperiodicCQITrigger(currentTTI,dlCQIInfo_p,dlUEContext_p,
                            periodicMode21Type1_p->cqiValueCodeword1,
                            subBandReportInfo_p->subBandToCQInfo[startRBGIndex].
                            initialReportedCQICW2,subBandValueDiff,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    /* Cyclomatic_complexity_changes_end */
                    /* - CQI_5.0 */
                }

                /* Cyclomatic_complexity_changes_start */
                updateSubBandInforForCW0AndCW1SpatialMul(rbgIndexCount,
                        startRBGIndex,
                        subBandReportInfo_p,
                        periodicMode21Type1_p,
                        dlCQIInfo_p,
                        cqiInfo_p,
                        rat0Info_p,
                        rat1Info_p,
                        numOfTxAnteenas,
                        toProcessCW2,
            dlUEContext_p
           );
                /* Cyclomatic_complexity_changes_end */

                /* SPR 9427 changes start */
            }
        }
        else
        {
            UInt8 subBandCount = subBandReportInfo_p->numOfSBperBWpart[periodicMode21Type1_p->bandwidthPart];
            while(subBandCount--)
            {    
                /*Check if there exists previous report for the reported CQI
                 *Index for CodeWord One.In this case we need to reset old 
                 *information and then update the new information in the relevant
                 *structure*/

                subBandNum = periodicMode21Type1_p->bandwidthPart * 
                    cqiGlobalParams_gp[internalCellIndex]\
                    ->numOfSubBandInbandWidthPart+ subBandCount;
                /* SPR 18122 Changes Start*/
                if( DIVIDE_BY_TWO(MAX_SUBBANDS) > subBandNum )
                /* SPR 18122 Changes End*/
                {
                    startRBGIndex = 
                        cqiGlobalParams_gp[internalCellIndex]->\
                        subBandToRBGMapAperiodic[subBandNum].startRBGIndex;
                    endRBGIndex = 
                        cqiGlobalParams_gp[internalCellIndex]->\
                        subBandToRBGMapAperiodic[subBandNum].endRBGIndex;
                    rbgIndexCount = endRBGIndex - startRBGIndex + 1;

                    /* + CQI_5.0 */
                    /* Cyclomatic_complexity_changes_start */
                    checkForAperiodicCQITrigger(currentTTI,dlCQIInfo_p,dlUEContext_p,
                            periodicMode21Type1_p->cqiValueCodeword0,
                            subBandReportInfo_p->subBandToCQInfo[startRBGIndex].
                            initialReportedCQICW1,subBandValueDiff,
                            internalCellIndex);
                    /* Cyclomatic_complexity_changes_end */
                    /* - CQI_5.0 */

                    if (0 != periodicMode21Type1_p->cqiValueCodeword1)
                    { 
                        toProcessCW2 = TRUE;

                        /* + CQI_5.0 */
                        /* Cyclomatic_complexity_changes_start */
                        checkForAperiodicCQITrigger(currentTTI,dlCQIInfo_p,dlUEContext_p,
                                periodicMode21Type1_p->cqiValueCodeword1,
                                subBandReportInfo_p->subBandToCQInfo[startRBGIndex].
                                initialReportedCQICW2,subBandValueDiff,
                                internalCellIndex);
                        /* Cyclomatic_complexity_changes_end */
                        /* - CQI_5.0 */
                    }

                    /* Cyclomatic_complexity_changes_start */
                    updateSubBandInforForCW0AndCW1SpatialMul(rbgIndexCount,
                            startRBGIndex,
                            subBandReportInfo_p,
                            periodicMode21Type1_p,
                            dlCQIInfo_p,
                            cqiInfo_p,
                            rat0Info_p,
                            rat1Info_p,
                            numOfTxAnteenas,
                            toProcessCW2,
                        dlUEContext_p
                        );
                    /* Cyclomatic_complexity_changes_end */

                }
            }
        }    
        /* SPR 9427 changes end */
    }
}

/* Cyclomatic_complexity_changes_start */
/*****************************************************************************
 * Function Name  : updateSubBandInforForCW0AndCW1SpatialMul
 * Inputs         : rbgIndexCount - RBG index count,
 *                  startRBGIndex - Start RBG index
 *                  subBandReportInfo_p - Pointer to subBand report
 *                  periodicMode21Type1_p - Pointer to periodic mode
 *                  dlCQIInfo_p - Pointer to dl CQI info
 *                  cqiInfo_p - Pointer to cqi Info
 *                  rat0Info_p - Pointer to RAT0 info
 *                  rat1Info_p - Pointer to RAT1 info
 *                  numOfTxAnteenas - Number of antennas
 *                  toProcessCW2 - Flag to determine CW2
 *                  dlUEContext_p - Downlink context of UE            
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion updates subBand info for Codeword 0 and 1.
 *****************************************************************************/
void updateSubBandInforForCW0AndCW1SpatialMul(UInt32 rbgIndexCount,
        UInt32 startRBGIndex,
        SubBandReportInfo *subBandReportInfo_p,
        PeriodicMode21Type1 *periodicMode21Type1_p,
        DLCQIInfo *dlCQIInfo_p,
        CQIInfo *cqiInfo_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p,
        UInt8 numOfTxAnteenas,
        UInt8 toProcessCW2,
        DLUEContext *dlUEContext_p
        )
{
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    UInt8 oldCqiForRBGCW0 = 0;
    UInt8 oldCqiForRBGCW1 = 0;
    UInt8 correctedSubbandCQICW0 = 0;
    UInt8 correctedSubbandCQICW1 = 0;
    UInt8 correctedSubbandMCSCW0 = 0;
    UInt8 correctedSubbandMCSCW1 = 0;

    while(rbgIndexCount--)
    {
        subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[startRBGIndex];

        oldCqiForRBGCW0 =  subBandToCQInfo_p->cqiIndexCW1;

        /* SPR 4769 changes start */
        /* Time Averaging */
        subBandToCQInfo_p->previousTimeAveragedSBCQICW0 = calculateAverageDLCQI(
                periodicMode21Type1_p->cqiValueCodeword0,
                subBandToCQInfo_p->previousTimeAveragedSBCQICW0);

        subBandToCQInfo_p->cqiIndexCW1 = getEffectiveDLCQI(
                subBandToCQInfo_p->previousTimeAveragedSBCQICW0,
                dlCQIInfo_p->cqiCorrectionFactorForCW0,dlUEContext_p); 

        /* Correcting Subband CQI and MCS for CW0 from BLER */
        correctedSubbandCQICW0 = subBandToCQInfo_p->cqiIndexCW1;
        correctedSubbandMCSCW0 = getMCSFromCQI(correctedSubbandCQICW0
                                     );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start*/

        /* Cyclomatic_complexity_changes_start */
        getModSchemeFromMcsIndex(correctedSubbandMCSCW0,
                &(dlCQIInfo_p->modulationSchemeCodeWordOne));
        /* Cyclomatic_complexity_changes_end */

        /*SPR 6424 Fix End*/

        /*Update the MCS Index Information for CW0*/
        subBandToCQInfo_p->mcsIndexCW1 = correctedSubbandMCSCW0;

        /* SPR 3803 changes start */  
        if (oldCqiForRBGCW0)
        {    
            /* SPR 3803 changes end */  
            resetRAT0CW1SpatialMulSubBandInfo(
                    startRBGIndex,
                    oldCqiForRBGCW0,
                    &cqiInfo_p->cqiInfoSpatialMul);

            resetRAT1CW1SpatialMulSubBandInfo(
                    startRBGIndex,
                    oldCqiForRBGCW0,
                    &cqiInfo_p->cqiInfoSpatialMul,
                    rat0Info_p,
                    rat1Info_p);
            /* SPR 3803 changes start */  
        }
        /* SPR 3803 changes end */  

        updateRAT0CW1SpatialMulSubBandInfo(
                startRBGIndex,
                correctedSubbandCQICW0,
                /* + CQI_5.0 */
                INVALID_PMI_VALUE,
                /* - CQI_5.0 */
                &cqiInfo_p->cqiInfoSpatialMul,
                numOfTxAnteenas);

        updateRAT1CW1SpatialMulSubBandInfo(
                startRBGIndex,
                correctedSubbandCQICW0,
                /* + CQI_5.0 */
                INVALID_PMI_VALUE,
                /* - CQI_5.0 */
                &cqiInfo_p->cqiInfoSpatialMul,
                rat0Info_p,
                rat1Info_p,
                numOfTxAnteenas);

        if (TRUE == toProcessCW2)
        {
            oldCqiForRBGCW1 = subBandToCQInfo_p->cqiIndexCW2;

            /* SPR 4769 changes start */
            /* Time Averaging */
            subBandToCQInfo_p->previousTimeAveragedSBCQICW1 = calculateAverageDLCQI(
                    periodicMode21Type1_p->cqiValueCodeword1,
                    subBandToCQInfo_p->previousTimeAveragedSBCQICW1);

            subBandToCQInfo_p->cqiIndexCW2 = getEffectiveDLCQI(
                    subBandToCQInfo_p->previousTimeAveragedSBCQICW1,
                    dlCQIInfo_p->cqiCorrectionFactorForCW1,dlUEContext_p); 

            correctedSubbandCQICW1 = subBandToCQInfo_p->cqiIndexCW2;
            correctedSubbandMCSCW1 = getMCSFromCQI(correctedSubbandCQICW1
                                     );
            /* SPR 4769 changes end */
            /*SPR 6424 Fix Start*/

            /* Cyclomatic_complexity_changes_start */
            getModSchemeFromMcsIndex(correctedSubbandMCSCW1,
                    &(dlCQIInfo_p->modulationSchemeCodeWordTwo));
            /* Cyclomatic_complexity_changes_end */

            /*SPR 6424 Fix End*/

            /*Update the MCS Index Information for CW1*/
            subBandToCQInfo_p->mcsIndexCW2 = correctedSubbandMCSCW1;

            /* SPR 3803 changes start */  
            if (oldCqiForRBGCW1)
            {    
                /* SPR 3803 changes end */  
                resetRAT0CW2SpatialMulSubBandInfo(
                        startRBGIndex,
                        oldCqiForRBGCW1,
                        &cqiInfo_p->cqiInfoSpatialMul);

                resetRAT1CW2SpatialMulSubBandInfo(
                        startRBGIndex,
                        oldCqiForRBGCW1,
                        &cqiInfo_p->cqiInfoSpatialMul,
                        rat0Info_p,
                        rat1Info_p);
                /* SPR 3803 changes start */  
            }
            /* SPR 3803 changes end */  

            updateRAT0CW2SpatialMulSubBandInfo(
                    startRBGIndex,
                    correctedSubbandCQICW1,
                    /* + CQI_5.0 */
                    INVALID_PMI_VALUE,
                    /* - CQI_5.0 */
                    &cqiInfo_p->cqiInfoSpatialMul,
                    numOfTxAnteenas);

            updateRAT1CW2SpatialMulSubBandInfo(
                    startRBGIndex,
                    correctedSubbandCQICW1,
                    /* + CQI_5.0 */
                    INVALID_PMI_VALUE,
                    /* - CQI_5.0 */
                    &cqiInfo_p->cqiInfoSpatialMul,
                    rat0Info_p,
                    rat1Info_p,
                    numOfTxAnteenas);
        }

        LOG_MAC_MSG(MAC_PERIODIC_UE_SELECTED_MODE_INFO_LOG_ID,LOGINFO,MAC_CQI,
                getCurrentTick(),dlUEContext_p->ueIndex,
                periodicMode21Type1_p->bandwidthPart,startRBGIndex,
                correctedSubbandCQICW0,correctedSubbandCQICW1,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"SB_BOTH_CW");

        startRBGIndex++;
    }

    return;
}
/* +EICIC */
/*****************************************************************************
 * Function Name  : updateSubBandInforForCW0AndCW1SpatialMul_2
 * Inputs         : rbgIndexCount - RBG index count,
 *                  startRBGIndex - Start RBG index
 *                  subBandReportInfo_p - Pointer to subBand report
 *                  periodicMode21Type1_p - Pointer to periodic mode
 *                  dlCQIInfo_p - Pointer to dl CQI info
 *                  cqiInfo_p - Pointer to cqi Info
 *                  rat0Info_p - Pointer to RAT0 info
 *                  rat1Info_p - Pointer to RAT1 info
 *                  numOfTxAnteenas - Number of antennas
 *                  toProcessCW2 - Flag to determine CW2
 *                  dlUEContext_p - Downlink context of UE            
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion updates subBand info for Codeword 0 and 1.
 *****************************************************************************/
void updateSubBandInforForCW0AndCW1SpatialMul_2 (UInt32 rbgIndexCount,
        UInt32 startRBGIndex,
        SubBandReportInfo *
        subBandReportInfo_p,
        PeriodicMode21Type1 *
        periodicMode21Type1_p,
        DLCQIInfo * dlCQIInfo_p,
        CQIInfo * cqiInfo_p,
        RAT0Info * rat0Info_p,
        RAT1Info * rat1Info_p,
        UInt8 numOfTxAnteenas,
        UInt8 toProcessCW2,
        DLUEContext * dlUEContext_p
        )
{
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    UInt8 oldCqiForRBGCW0 = 0;
    UInt8 oldCqiForRBGCW1 = 0;
    UInt8 correctedSubbandCQICW0 = 0;
    UInt8 correctedSubbandCQICW1 = 0;
    UInt8 correctedSubbandMCSCW0 = 0;
    UInt8 correctedSubbandMCSCW1 = 0;

    while (rbgIndexCount--)
    {
        subBandToCQInfo_p =
            &subBandReportInfo_p->subBandToCQInfo[startRBGIndex];

        oldCqiForRBGCW0 = subBandToCQInfo_p->cqiIndexCW1_2;

        /* SPR 4769 changes start */
        /* Time Averaging */
        subBandToCQInfo_p->previousTimeAveragedSBCQICW0_2 =
            calculateAverageDLCQI (periodicMode21Type1_p->cqiValueCodeword0,
                    subBandToCQInfo_p->
                    previousTimeAveragedSBCQICW0_2);

        subBandToCQInfo_p->cqiIndexCW1_2 =
            getEffectiveDLCQI (subBandToCQInfo_p->previousTimeAveragedSBCQICW0_2,
                    dlCQIInfo_p->cqiCorrectionFactorForCW0_2,
                    dlUEContext_p);

        /* Correcting Subband CQI and MCS for CW0 from BLER */
        correctedSubbandCQICW0 = subBandToCQInfo_p->cqiIndexCW1_2;
        correctedSubbandMCSCW0 = getMCSFromCQI (correctedSubbandCQICW0
                                                );
        /* SPR 4769 changes end */
        /*SPR 6424 Fix Start */

        /* Cyclomatic_complexity_changes_start */
        getModSchemeFromMcsIndex (correctedSubbandMCSCW0,
                &(dlCQIInfo_p->
                    modulationSchemeCodeWordOne_2));
        /* Cyclomatic_complexity_changes_end */

        /*SPR 6424 Fix End */

        /*Update the MCS Index Information for CW0 */
        subBandToCQInfo_p->mcsIndexCW1 = correctedSubbandMCSCW0;

        /* SPR 3803 changes start */
        if (oldCqiForRBGCW0)
        {
            /* SPR 3803 changes end */
            resetRAT0CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                    oldCqiForRBGCW0,
                    &cqiInfo_p->cqiInfoSpatialMul);

            resetRAT1CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                    oldCqiForRBGCW0,
                    &cqiInfo_p->cqiInfoSpatialMul,
                    rat0Info_p, rat1Info_p);
            /* SPR 3803 changes start */
        }
        /* SPR 3803 changes end */

        updateRAT0CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                correctedSubbandCQICW0,
                /* + CQI_5.0 */
                INVALID_PMI_VALUE,
                /* - CQI_5.0 */
                &cqiInfo_p->cqiInfoSpatialMul,
                numOfTxAnteenas);

        updateRAT1CW1SpatialMulSubBandInfo_2 (startRBGIndex,
                correctedSubbandCQICW0,
                /* + CQI_5.0 */
                INVALID_PMI_VALUE,
                /* - CQI_5.0 */
                &cqiInfo_p->cqiInfoSpatialMul,
                rat0Info_p,
                rat1Info_p, numOfTxAnteenas);

        if (TRUE == toProcessCW2)
        {
            oldCqiForRBGCW1 = subBandToCQInfo_p->cqiIndexCW2_2;

            /* SPR 4769 changes start */
            /* Time Averaging */
            subBandToCQInfo_p->previousTimeAveragedSBCQICW1_2 =
                calculateAverageDLCQI (periodicMode21Type1_p->cqiValueCodeword1,
                        subBandToCQInfo_p->
                        previousTimeAveragedSBCQICW1_2);

            subBandToCQInfo_p->cqiIndexCW2 =
                getEffectiveDLCQI (subBandToCQInfo_p->
                        previousTimeAveragedSBCQICW1_2,
                        dlCQIInfo_p->cqiCorrectionFactorForCW1_2,
                        dlUEContext_p);

            correctedSubbandCQICW1 = subBandToCQInfo_p->cqiIndexCW2_2;
            correctedSubbandMCSCW1 = getMCSFromCQI (correctedSubbandCQICW1
                                                    );
            /* SPR 4769 changes end */
            /*SPR 6424 Fix Start */

            /* Cyclomatic_complexity_changes_start */
            getModSchemeFromMcsIndex (correctedSubbandMCSCW1,
                    &(dlCQIInfo_p->
                        modulationSchemeCodeWordTwo_2));
            /* Cyclomatic_complexity_changes_end */

            /*SPR 6424 Fix End */

            /*Update the MCS Index Information for CW1 */
            subBandToCQInfo_p->mcsIndexCW2 = correctedSubbandMCSCW1;

            /* SPR 3803 changes start */
            if (oldCqiForRBGCW1)
            {
                /* SPR 3803 changes end */
                resetRAT0CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                        oldCqiForRBGCW1,
                        &cqiInfo_p->
                        cqiInfoSpatialMul);

                resetRAT1CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                        oldCqiForRBGCW1,
                        &cqiInfo_p->
                        cqiInfoSpatialMul,
                        rat0Info_p, rat1Info_p);
                /* SPR 3803 changes start */
            }
            /* SPR 3803 changes end */

            updateRAT0CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                    correctedSubbandCQICW1,
                    /* + CQI_5.0 */
                    INVALID_PMI_VALUE,
                    /* - CQI_5.0 */
                    &cqiInfo_p->cqiInfoSpatialMul,
                    numOfTxAnteenas);

            updateRAT1CW2SpatialMulSubBandInfo_2 (startRBGIndex,
                    correctedSubbandCQICW1,
                    /* + CQI_5.0 */
                    INVALID_PMI_VALUE,
                    /* - CQI_5.0 */
                    &cqiInfo_p->cqiInfoSpatialMul,
                    rat0Info_p,
                    rat1Info_p, numOfTxAnteenas);
        }

        LOG_MAC_MSG (MAC_PERIODIC_UE_SELECTED_MODE_INFO_LOG_ID, LOGINFO, MAC_CQI,
                getCurrentTick (), dlUEContext_p->ueIndex,
                periodicMode21Type1_p->bandwidthPart, startRBGIndex,
                correctedSubbandCQICW0, correctedSubbandCQICW1,
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                __func__, "SB_BOTH_CW");

        startRBGIndex++;
    }

    return;
}

/*-EICIC*/
/* Cyclomatic_complexity_changes_en */

/*****************************************************************************
 * Function Name  : resetRAT0CW1SpatialMulSubBandInfo
 * Inputs         : rbgIndex - Index of RGB,
 *                  oldCqiIndexCW1 - This holds Old CQI index of the subBand
 *                                   whose Information need to be reset
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will reset the RAT 0 Information stored in 
 *                  cqiInfoSpatialMul_p structure for Spatial Multiplexing.
 *****************************************************************************/
STATIC  void resetRAT0CW1SpatialMulSubBandInfo(
        UInt32 rbgIndex,
        UInt32 oldCqiIndexCW1,
        CQIInfoSpatialMul *cqiInfoSpatialMul_p)
{
    RAT0SpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
    /* + CQI_4.1 */
    UInt32 resetBit = 0;
    rat0CQIInfo_p = &cqiInfoSpatialMul_p->rat0SpatialMulCQIInfo;

    /*Checking the range in which the old CQI Index falls*/
    resetBit = (~((UInt32)1 << rbgIndex));
    if ((oldCqiIndexCW1 >= DL_64QAM_LOWEST_CQI_INDEX) && 
            (oldCqiIndexCW1 <= DL_64QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in 64QAM Bands*/
        rat0CQIInfo_p->count64QAMSubBandCW1--;
        rat0CQIInfo_p->count16QAMSubBandCW1--;
        rat0CQIInfo_p->countQPSKSubBandCW1--;

        /*Resetting the old CQI Index information*/
        rat0CQIInfo_p->rat064QAMCQIBitMapCW1 &= resetBit;
        rat0CQIInfo_p->rat016QAMCQIBitMapCW1 &= resetBit;
        rat0CQIInfo_p->rat0QPSKCQIBitMapCW1 &= resetBit;
    }

    else if ((oldCqiIndexCW1 >= DL_16QAM_LOWEST_CQI_INDEX) && 
                          (oldCqiIndexCW1 <= DL_16QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in 16QAM Bands*/
        rat0CQIInfo_p->count16QAMSubBandCW1--;
        rat0CQIInfo_p->countQPSKSubBandCW1--;

        /*Resetting the old CQI Index information*/
        rat0CQIInfo_p->rat016QAMCQIBitMapCW1 &= resetBit;
        rat0CQIInfo_p->rat0QPSKCQIBitMapCW1 &= resetBit;
    }

    else if ((oldCqiIndexCW1 >= DL_QPSK_LOWEST_CQI_INDEX) && 
                         (oldCqiIndexCW1 <= DL_QPSK_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in QPSK Bands*/
        rat0CQIInfo_p->countQPSKSubBandCW1--;

        /*Resetting the old CQI Index information*/
        rat0CQIInfo_p->rat0QPSKCQIBitMapCW1 &= resetBit;
    }
    /* - CQI_4.1 */
    
    return;
}

/*+EICIC .*/
/*****************************************************************************
 * Function Name  : resetRAT0CW1SpatialMulSubBandInfo
 * Inputs         : rbgIndex - Index of RGB,
 *                  oldCqiIndexCW1 - This holds Old CQI index of the subBand
 *                                   whose Information need to be reset
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will reset the RAT 0 Information stored in 
 *                  cqiInfoSpatialMul_p structure for Spatial Multiplexing.
 *****************************************************************************/
    STATIC  void
resetRAT0CW1SpatialMulSubBandInfo_2 (UInt32 rbgIndex,
        UInt32 oldCqiIndexCW1,
        CQIInfoSpatialMul * cqiInfoSpatialMul_p)
{
    RAT0SpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
    /* + CQI_4.1 */
    UInt32 resetBit = 0;
    rat0CQIInfo_p = &cqiInfoSpatialMul_p->rat0SpatialMulCQIInfo;

    /*Checking the range in which the old CQI Index falls */
    resetBit = (~((UInt32) 1 << rbgIndex));
    if ((oldCqiIndexCW1 >= DL_64QAM_LOWEST_CQI_INDEX) &&
            (oldCqiIndexCW1 <= DL_64QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in 64QAM Bands */
        rat0CQIInfo_p->count64QAMSubBandCW1_2--;
        rat0CQIInfo_p->count16QAMSubBandCW1_2--;
        rat0CQIInfo_p->countQPSKSubBandCW1_2--;

        /*Resetting the old CQI Index information */
        rat0CQIInfo_p->rat064QAMCQIBitMapCW1_2 &= resetBit;
        rat0CQIInfo_p->rat016QAMCQIBitMapCW1_2 &= resetBit;
        rat0CQIInfo_p->rat0QPSKCQIBitMapCW1_2 &= resetBit;
    }

    else if ((oldCqiIndexCW1 >= DL_16QAM_LOWEST_CQI_INDEX) &&
            (oldCqiIndexCW1 <= DL_16QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in 16QAM Bands */
        rat0CQIInfo_p->count16QAMSubBandCW1_2--;
        rat0CQIInfo_p->countQPSKSubBandCW1_2--;

        /*Resetting the old CQI Index information */
        rat0CQIInfo_p->rat016QAMCQIBitMapCW1_2 &= resetBit;
        rat0CQIInfo_p->rat0QPSKCQIBitMapCW1_2 &= resetBit;
    }

    else if ((oldCqiIndexCW1 >= DL_QPSK_LOWEST_CQI_INDEX) &&
            (oldCqiIndexCW1 <= DL_QPSK_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in QPSK Bands */
        rat0CQIInfo_p->countQPSKSubBandCW1_2--;

        /*Resetting the old CQI Index information */
        rat0CQIInfo_p->rat0QPSKCQIBitMapCW1_2 &= resetBit;
    }
    /* - CQI_4.1 */

    return;
}

/*****************************************************************************
 * Function Name  : updateRAT0CW1SpatialMulSubBandInfo_2
 * Inputs         : rbgIndex - Index of RGB,
 *                  subbandPMIValue - This holds subBand PMI Value,
 *                  newCqiIndexCW1 - This holds New CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  numOfTxAnteenas -
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will update the RAT 0 Information stored in 
 *                  cqiInfoSpatialMul_p structure for Spatial Multiplexing.
 *****************************************************************************/
STATIC  void
updateRAT0CW1SpatialMulSubBandInfo_2 (UInt32 rbgIndex, UInt32 newCqiIndexCW1,
          /* + CQI_5.0 */
          UInt8 subbandPMIValue,
          /* + CQI_5.0 */
          CQIInfoSpatialMul * cqiInfoSpatialMul_p,
          /* CA Changes start */
          UInt8 numOfTxAnteenas)
       /* CA Changes end */
{
  RAT0SpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
  /* + CQI_4.1 */
  UInt32 setBit = 0;

  /* TDD SB_CQI */
  UInt8 iteration = 0;
  /* TDD SB_CQI */

  rat0CQIInfo_p = &cqiInfoSpatialMul_p->rat0SpatialMulCQIInfo;
  setBit = ((UInt32) 1 << rbgIndex);
  /* + CQI_5.0 */
  RAT0PmiInfo *rat0PmiInfo_p = &cqiInfoSpatialMul_p->pmiInfo.rat0PmiInfo;

  if (newCqiIndexCW1)
    {
      /* - CQI_5.0 */
      /*Checking the range in which the old CQI Index falls */
      if ((newCqiIndexCW1 >= DL_64QAM_LOWEST_CQI_INDEX) &&
   (newCqiIndexCW1 <= DL_64QAM_HIGHEST_CQI_INDEX))
 {
   /*Comes here when the new CQI Index falls in 64QAM Bands */
   rat0CQIInfo_p->count64QAMSubBandCW1_2++;
   rat0CQIInfo_p->count16QAMSubBandCW1_2++;
   rat0CQIInfo_p->countQPSKSubBandCW1_2++;

   /*Setting the new CQI Index */
   rat0CQIInfo_p->rat064QAMCQIBitMapCW1_2 |= setBit;
   rat0CQIInfo_p->rat016QAMCQIBitMapCW1_2 |= setBit;
   rat0CQIInfo_p->rat0QPSKCQIBitMapCW1_2 |= setBit;

 }

      else if ((newCqiIndexCW1 >= DL_16QAM_LOWEST_CQI_INDEX) &&
        (newCqiIndexCW1 <= DL_16QAM_HIGHEST_CQI_INDEX))
 {
   /*Comes here when the new CQI Index falls in 16QAM Bands */
   rat0CQIInfo_p->count16QAMSubBandCW1_2++;
   rat0CQIInfo_p->countQPSKSubBandCW1_2++;

   /*Setting the new CQI Index */
   rat0CQIInfo_p->rat016QAMCQIBitMapCW1_2 |= setBit;
   rat0CQIInfo_p->rat0QPSKCQIBitMapCW1_2 |= setBit;

 }

      else if ((newCqiIndexCW1 >= DL_QPSK_LOWEST_CQI_INDEX) &&
        (newCqiIndexCW1 <= DL_QPSK_HIGHEST_CQI_INDEX))
 {
   /*Comes here when the new CQI Index falls in QPSK Bands */
   rat0CQIInfo_p->countQPSKSubBandCW1_2++;

   /*Setting the new CQI Index */
   rat0CQIInfo_p->rat0QPSKCQIBitMapCW1_2 |= setBit;
 }
      /* - CQI_4.1 */
      /* + CQI_5.0 */
    }
  /*4x2 MIMO S */
  /* CA Changes start */
  if ((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
      && (INVALID_PMI_VALUE != subbandPMIValue)
      && (subbandPMIValue < PMI_INDEX_RANK_1))
    /* CA Changes end */
    {
      /* TDD SB_CQI */
      for (; iteration < PMI_INDEX_RANK_1; iteration++)
 {
   if (iteration == subbandPMIValue)
     {
       rat0PmiInfo_p->reportedRBGRank1_2[iteration] |= setBit;
     }
   else
     {
       rat0PmiInfo_p->reportedRBGRank1_2[iteration] &= (~setBit);
     }
 }
      /* TDD SB_CQI */
    }
  /* CA Changes start */
  else if ((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    && (INVALID_PMI_VALUE != subbandPMIValue)
    && (subbandPMIValue < PMI_INDEX_RANK))
    /* CA Changes end */
    {
      rat0PmiInfo_p->reportedRBGPMIForPort4_2[subbandPMIValue] |= setBit;
    }
  /*4x2 MIMO E */
  /* - CQI_5.0 */
  return;
}

/*-EICIC .*/

/*****************************************************************************
 * Function Name  : updateRAT0CW1SpatialMulSubBandInfo
 * Inputs         : rbgIndex - Index of RGB,
 *                  subbandPMIValue - This holds subBand PMI Value,
 *                  newCqiIndexCW1 - This holds New CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  numOfTxAnteenas -
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will update the RAT 0 Information stored in 
 *                  cqiInfoSpatialMul_p structure for Spatial Multiplexing.
 *****************************************************************************/
STATIC  void updateRAT0CW1SpatialMulSubBandInfo(
        UInt32 rbgIndex,
        UInt32 newCqiIndexCW1,
        /* + CQI_5.0 */
        UInt8 subbandPMIValue,
        /* + CQI_5.0 */
        CQIInfoSpatialMul *cqiInfoSpatialMul_p,
       /* CA Changes start */
        UInt8 numOfTxAnteenas)
       /* CA Changes end */
{
    RAT0SpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
    /* + CQI_4.1 */
    UInt32 setBit = 0;

    /* TDD SB_CQI */
    UInt8 iteration = 0;
    /* TDD SB_CQI */

    rat0CQIInfo_p = &cqiInfoSpatialMul_p->rat0SpatialMulCQIInfo;
    setBit = ((UInt32)1 << rbgIndex);
    /* + CQI_5.0 */
    RAT0PmiInfo *rat0PmiInfo_p = 
        &cqiInfoSpatialMul_p->pmiInfo.rat0PmiInfo;
   
    if(newCqiIndexCW1)
    {    
    /* - CQI_5.0 */
        /*Checking the range in which the old CQI Index falls*/
        if ((newCqiIndexCW1 >= DL_64QAM_LOWEST_CQI_INDEX) && 
                (newCqiIndexCW1 <= DL_64QAM_HIGHEST_CQI_INDEX))
        {
            /*Comes here when the new CQI Index falls in 64QAM Bands*/
            rat0CQIInfo_p->count64QAMSubBandCW1++;
            rat0CQIInfo_p->count16QAMSubBandCW1++;
            rat0CQIInfo_p->countQPSKSubBandCW1++;

            /*Setting the new CQI Index*/
            rat0CQIInfo_p->rat064QAMCQIBitMapCW1 |= setBit;
            rat0CQIInfo_p->rat016QAMCQIBitMapCW1 |= setBit;
            rat0CQIInfo_p->rat0QPSKCQIBitMapCW1 |= setBit;

        }

        else if ((newCqiIndexCW1 >= DL_16QAM_LOWEST_CQI_INDEX) && 
                (newCqiIndexCW1 <= DL_16QAM_HIGHEST_CQI_INDEX))
        {
            /*Comes here when the new CQI Index falls in 16QAM Bands*/
            rat0CQIInfo_p->count16QAMSubBandCW1++;
            rat0CQIInfo_p->countQPSKSubBandCW1++;

            /*Setting the new CQI Index*/
            rat0CQIInfo_p->rat016QAMCQIBitMapCW1 |= setBit;
            rat0CQIInfo_p->rat0QPSKCQIBitMapCW1 |= setBit;

        }

        else if ((newCqiIndexCW1 >= DL_QPSK_LOWEST_CQI_INDEX) && 
                (newCqiIndexCW1 <= DL_QPSK_HIGHEST_CQI_INDEX))
        {
            /*Comes here when the new CQI Index falls in QPSK Bands*/
            rat0CQIInfo_p->countQPSKSubBandCW1++;

            /*Setting the new CQI Index*/
            rat0CQIInfo_p->rat0QPSKCQIBitMapCW1 |= setBit;
        }
        /* - CQI_4.1 */
    /* + CQI_5.0 */
    }
    /*4x2 MIMO S*/
    /* CA Changes start */
    if((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2
        )&& (INVALID_PMI_VALUE != subbandPMIValue) && 
       (subbandPMIValue < PMI_INDEX_RANK_1))     
    /* CA Changes end */
    {        
        /* TDD SB_CQI */
	    for(;iteration < PMI_INDEX_RANK_1 ; iteration++)
	    {      
		    if(iteration == subbandPMIValue)
		    {
			    rat0PmiInfo_p->reportedRBGRank1[iteration] |= setBit;
		    }
		    else
		    {
			    rat0PmiInfo_p->reportedRBGRank1[iteration] &= (~setBit);
		    }
	    }
        /* TDD SB_CQI */
    }
    /* CA Changes start */
    else if((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4
        )&& (INVALID_PMI_VALUE != subbandPMIValue) && 
       (subbandPMIValue < PMI_INDEX_RANK))     
    /* CA Changes end */
    {        
        rat0PmiInfo_p->reportedRBGPMIForPort4[subbandPMIValue] |= setBit;
    }
    /*4x2 MIMO E*/
    /* - CQI_5.0 */
    return;
}

/*****************************************************************************
 * Function Name  : resetRAT0CW2SpatialMulSubBandInfo
 * Inputs         : rbgIndex - Index of RGB,
 *                  oldCqiIndexCW2 - This holds Old CQI index of the subBand
 *                                   whose Information need to be reset,
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will reset the RAT 0 Information stored in 
 *                  cqiInfoSpatialMul_p structure for Spatial Multiplexing.
 *****************************************************************************/
STATIC  void resetRAT0CW2SpatialMulSubBandInfo(
        UInt32 rbgIndex,
        UInt32 oldCqiIndexCW2,
        CQIInfoSpatialMul *cqiInfoSpatialMul_p)
{
  
    RAT0SpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
    /* + CQI_4.1 */
    UInt32 resetBit = 0;
    rat0CQIInfo_p = &cqiInfoSpatialMul_p->rat0SpatialMulCQIInfo;
    
    resetBit = (~((UInt32)1 << rbgIndex));
    
    /*Checking the range in which the old CQI Index falls*/
    if ((oldCqiIndexCW2 >= DL_64QAM_LOWEST_CQI_INDEX) && 
                     (oldCqiIndexCW2 <= DL_64QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in 64QAM Bands*/
        rat0CQIInfo_p->count64QAMSubBandCW2--;
        rat0CQIInfo_p->count16QAMSubBandCW2--;
        rat0CQIInfo_p->countQPSKSubBandCW2--;

       /*Resetting the old CQI Index information*/
        rat0CQIInfo_p->rat064QAMCQIBitMapCW2 &= resetBit;
        rat0CQIInfo_p->rat016QAMCQIBitMapCW2 &= resetBit;
        rat0CQIInfo_p->rat0QPSKCQIBitMapCW2 &= resetBit;
    }
    else if ((oldCqiIndexCW2 >= DL_16QAM_LOWEST_CQI_INDEX) && 
                         (oldCqiIndexCW2 <= DL_16QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in 16QAM Bands*/
        rat0CQIInfo_p->count16QAMSubBandCW2--;
        rat0CQIInfo_p->countQPSKSubBandCW2--;

        /*Resetting the old CQI Index information*/
        rat0CQIInfo_p->rat016QAMCQIBitMapCW2 &= resetBit;
        rat0CQIInfo_p->rat0QPSKCQIBitMapCW2 &= resetBit;
    }
    else if ((oldCqiIndexCW2 >= DL_QPSK_LOWEST_CQI_INDEX) && 
                          (oldCqiIndexCW2 <= DL_QPSK_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in QPSK Bands*/
        rat0CQIInfo_p->countQPSKSubBandCW2--;

        /*Resetting the old CQI Index information*/
        rat0CQIInfo_p->rat0QPSKCQIBitMapCW2 &= resetBit;
    }
    /* - CQI_4.1 */
    return;
}

/*+EICIC .*/
/*****************************************************************************
 * Function Name  : resetRAT0CW2SpatialMulSubBandInfo_2
 * Inputs         : rbgIndex - Index of RGB,
 *                  oldCqiIndexCW2 - This holds Old CQI index of the subBand
 *                                   whose Information need to be reset,
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will reset the RAT 0 Information stored in 
 *                  cqiInfoSpatialMul_p structure for Spatial Multiplexing.
 *****************************************************************************/
STATIC  void
resetRAT0CW2SpatialMulSubBandInfo_2 (UInt32 rbgIndex,
         UInt32 oldCqiIndexCW2,
         CQIInfoSpatialMul * cqiInfoSpatialMul_p)
{

  RAT0SpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
  /* + CQI_4.1 */
  UInt32 resetBit = 0;
  rat0CQIInfo_p = &cqiInfoSpatialMul_p->rat0SpatialMulCQIInfo;

  resetBit = (~((UInt32) 1 << rbgIndex));

  /*Checking the range in which the old CQI Index falls */
  if ((oldCqiIndexCW2 >= DL_64QAM_LOWEST_CQI_INDEX) &&
      (oldCqiIndexCW2 <= DL_64QAM_HIGHEST_CQI_INDEX))
    {
      /*Comes here when the old CQI Index falls in 64QAM Bands */
      rat0CQIInfo_p->count64QAMSubBandCW2_2--;
      rat0CQIInfo_p->count16QAMSubBandCW2_2--;
      rat0CQIInfo_p->countQPSKSubBandCW2_2--;

      /*Resetting the old CQI Index information */
      rat0CQIInfo_p->rat064QAMCQIBitMapCW2_2 &= resetBit;
      rat0CQIInfo_p->rat016QAMCQIBitMapCW2_2 &= resetBit;
      rat0CQIInfo_p->rat0QPSKCQIBitMapCW2_2 &= resetBit;
    }
  else if ((oldCqiIndexCW2 >= DL_16QAM_LOWEST_CQI_INDEX) &&
    (oldCqiIndexCW2 <= DL_16QAM_HIGHEST_CQI_INDEX))
    {
      /*Comes here when the old CQI Index falls in 16QAM Bands */
      rat0CQIInfo_p->count16QAMSubBandCW2_2--;
      rat0CQIInfo_p->countQPSKSubBandCW2_2--;

      /*Resetting the old CQI Index information */
      rat0CQIInfo_p->rat016QAMCQIBitMapCW2_2 &= resetBit;
      rat0CQIInfo_p->rat0QPSKCQIBitMapCW2_2 &= resetBit;
    }
  else if ((oldCqiIndexCW2 >= DL_QPSK_LOWEST_CQI_INDEX) &&
    (oldCqiIndexCW2 <= DL_QPSK_HIGHEST_CQI_INDEX))
    {
      /*Comes here when the old CQI Index falls in QPSK Bands */
      rat0CQIInfo_p->countQPSKSubBandCW2_2--;

      /*Resetting the old CQI Index information */
      rat0CQIInfo_p->rat0QPSKCQIBitMapCW2_2 &= resetBit;
    }
  /* - CQI_4.1 */
  return;
}

/*****************************************************************************
 * Function Name  : updateRAT0CW2SpatialMulSubBandInfo_2
 * Inputs         : rbgIndex - Index of RGB,
 *                  newCqiIndexCW2 - This holds Old CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  subbandPMIValue - This holds subband PMI Value,
 *                  numOfTxAnteenas -
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will update the RAT 0 Information stored in 
 *                  cqiInfoSpatialMul_p structure for Spatial Multiplexing.
 *****************************************************************************/
STATIC  void updateRAT0CW2SpatialMulSubBandInfo_2 (UInt32 rbgIndex, 
                                       UInt32 newCqiIndexCW2,
                                       /* + CQI_5.0 */
                                       UInt8 subbandPMIValue,
                                      /* + CQI_5.0 */
                                      CQIInfoSpatialMul * cqiInfoSpatialMul_p,
                                     /* CA Changes start */
                                      UInt8 numOfTxAnteenas)
                                     /* CA Changes end */
{
    RAT0SpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
    /* + CQI_4.1 */
    UInt32 setBit = 0;

    /* TDD SB_CQI */
    UInt8 iteration = 0;
    /* TDD SB_CQI */

    rat0CQIInfo_p = &cqiInfoSpatialMul_p->rat0SpatialMulCQIInfo;

    setBit = ((UInt32) 1 << rbgIndex);
    /* + CQI_5.0 */
    RAT0PmiInfo *rat0PmiInfo_p = &cqiInfoSpatialMul_p->pmiInfo.rat0PmiInfo;

    if (newCqiIndexCW2)
    {
        /* - CQI_5.0 */
        /*Checking the range in which the old CQI Index falls */
        if ((newCqiIndexCW2 >= DL_64QAM_LOWEST_CQI_INDEX) &&
                (newCqiIndexCW2 <= DL_64QAM_HIGHEST_CQI_INDEX))
        {
            /*Comes here when the new CQI Index falls in 64QAM Bands */
            rat0CQIInfo_p->count64QAMSubBandCW2_2++;
            rat0CQIInfo_p->count16QAMSubBandCW2_2++;
            rat0CQIInfo_p->countQPSKSubBandCW2_2++;

            /*Setting the new CQI Index */
            rat0CQIInfo_p->rat064QAMCQIBitMapCW2_2 |= setBit;
            rat0CQIInfo_p->rat016QAMCQIBitMapCW2_2 |= setBit;
            rat0CQIInfo_p->rat0QPSKCQIBitMapCW2_2 |= setBit;

        }
        else if ((newCqiIndexCW2 >= DL_16QAM_LOWEST_CQI_INDEX) &&
                (newCqiIndexCW2 <= DL_16QAM_HIGHEST_CQI_INDEX))
        {
            /*Comes here when the new CQI Index falls in 16QAM Bands */
            rat0CQIInfo_p->count16QAMSubBandCW2_2++;
            rat0CQIInfo_p->countQPSKSubBandCW2_2++;

            /*Setting the new CQI Index */
            rat0CQIInfo_p->rat016QAMCQIBitMapCW2_2 |= setBit;
            rat0CQIInfo_p->rat0QPSKCQIBitMapCW2_2 |= setBit;

        }
        else if ((newCqiIndexCW2 >= DL_QPSK_LOWEST_CQI_INDEX) &&
                (newCqiIndexCW2 <= DL_QPSK_HIGHEST_CQI_INDEX))
        {
            /*Comes here when the new CQI Index falls in QPSK Bands */
            rat0CQIInfo_p->countQPSKSubBandCW2_2++;

            /*Setting the new CQI Index */
            rat0CQIInfo_p->rat0QPSKCQIBitMapCW2_2 |= setBit;
        }
        /* - CQI_4.1 */
        /* + CQI_5.0 */
    }
    /*4x2 MIMO S */
    /*4x2 MIMO CR */
    /* CA Changes start */
    if ((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2) &&
            (INVALID_PMI_VALUE != subbandPMIValue) &&
            (subbandPMIValue < PMI_INDEX_RANK_2))
        /* CA Changes end */
    {
        /* TDD SB_CQI */
        for (; iteration < PMI_INDEX_RANK_2; iteration++)
        {
            if (subbandPMIValue == iteration)
            {
                rat0PmiInfo_p->reportedRBGRank2_2[iteration] |= setBit;
            }
            else
            {
                rat0PmiInfo_p->reportedRBGRank2_2[iteration] &= (~setBit);
            }
        }
        /* TDD SB_CQI */
    }
    /* CA Changes start */
    else if ((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4) &&
            (INVALID_PMI_VALUE != subbandPMIValue) &&
            (subbandPMIValue < PMI_INDEX_RANK))
        /* CA Changes end */
    {
        rat0PmiInfo_p->reportedRBGPMIForPort4_2[subbandPMIValue] |= setBit;
    }
    /*4x2 MIMO E */
    /* - CQI_5.0 */

    return;
}


/*-EICIC .*/
/*****************************************************************************
 * Function Name  : updateRAT0CW2SpatialMulSubBandInfo
 * Inputs         : rbgIndex - Index of RGB,
 *                  newCqiIndexCW2 - This holds Old CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  subbandPMIValue - This holds subband PMI Value,
 *                  numOfTxAnteenas -
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will update the RAT 0 Information stored in 
 *                  cqiInfoSpatialMul_p structure for Spatial Multiplexing.
 *****************************************************************************/
STATIC  void updateRAT0CW2SpatialMulSubBandInfo(
        UInt32 rbgIndex,
        UInt32 newCqiIndexCW2,
        /* + CQI_5.0 */
        UInt8 subbandPMIValue,
        /* + CQI_5.0 */
        CQIInfoSpatialMul *cqiInfoSpatialMul_p,
       /* CA Changes start */
        UInt8 numOfTxAnteenas)
    /* CA Changes end */
{
    RAT0SpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
    /* + CQI_4.1 */
    UInt32 setBit = 0;

    /* TDD SB_CQI */
    UInt8 iteration = 0;
    /* TDD SB_CQI */

    rat0CQIInfo_p = &cqiInfoSpatialMul_p->rat0SpatialMulCQIInfo;

    setBit = ((UInt32)1 << rbgIndex);
    /* + CQI_5.0 */
    RAT0PmiInfo *rat0PmiInfo_p = 
        &cqiInfoSpatialMul_p->pmiInfo.rat0PmiInfo;

    if(newCqiIndexCW2)
    {    
        /* - CQI_5.0 */
        /*Checking the range in which the old CQI Index falls*/
        if ((newCqiIndexCW2 >= DL_64QAM_LOWEST_CQI_INDEX) && 
                (newCqiIndexCW2 <= DL_64QAM_HIGHEST_CQI_INDEX))
        {
            /*Comes here when the new CQI Index falls in 64QAM Bands*/
            rat0CQIInfo_p->count64QAMSubBandCW2++;
            rat0CQIInfo_p->count16QAMSubBandCW2++;
            rat0CQIInfo_p->countQPSKSubBandCW2++;

            /*Setting the new CQI Index*/
            rat0CQIInfo_p->rat064QAMCQIBitMapCW2 |= setBit;
            rat0CQIInfo_p->rat016QAMCQIBitMapCW2 |= setBit;
            rat0CQIInfo_p->rat0QPSKCQIBitMapCW2 |= setBit;

        }
        else if ((newCqiIndexCW2 >= DL_16QAM_LOWEST_CQI_INDEX) && 
                (newCqiIndexCW2 <= DL_16QAM_HIGHEST_CQI_INDEX))
        {
            /*Comes here when the new CQI Index falls in 16QAM Bands*/
            rat0CQIInfo_p->count16QAMSubBandCW2++;
            rat0CQIInfo_p->countQPSKSubBandCW2++;

            /*Setting the new CQI Index*/
            rat0CQIInfo_p->rat016QAMCQIBitMapCW2 |= setBit;
            rat0CQIInfo_p->rat0QPSKCQIBitMapCW2 |= setBit;

        }
        else if ((newCqiIndexCW2 >= DL_QPSK_LOWEST_CQI_INDEX) && 
                (newCqiIndexCW2 <= DL_QPSK_HIGHEST_CQI_INDEX))
        {
            /*Comes here when the new CQI Index falls in QPSK Bands*/
            rat0CQIInfo_p->countQPSKSubBandCW2++;

            /*Setting the new CQI Index*/
            rat0CQIInfo_p->rat0QPSKCQIBitMapCW2 |= setBit;
        }
        /* - CQI_4.1 */
        /* + CQI_5.0 */
    }
    /*4x2 MIMO S*/
    /*4x2 MIMO CR*/
    /* CA Changes start */
    if((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2) && \
            (INVALID_PMI_VALUE != subbandPMIValue) && \
            (subbandPMIValue < PMI_INDEX_RANK_2))     
        /* CA Changes end */
    {       
        /* TDD SB_CQI */
        for(;iteration < PMI_INDEX_RANK_2 ; iteration++)
        {
            if(subbandPMIValue == iteration)
            {
                rat0PmiInfo_p->reportedRBGRank2[iteration] |= setBit;
            }
            else
            {
                rat0PmiInfo_p->reportedRBGRank2[iteration] &= (~setBit);
            }
        }
        /* TDD SB_CQI */
    }
    /* CA Changes start */
    else if((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)&& \
            (INVALID_PMI_VALUE != subbandPMIValue) && \
            (subbandPMIValue < PMI_INDEX_RANK))     
        /* CA Changes end */
    {        
        rat0PmiInfo_p->reportedRBGPMIForPort4[subbandPMIValue] |= setBit;
    }
    /*4x2 MIMO E*/
    /* - CQI_5.0 */

    return;
}

/**********************************************************************************
 * Function Name  : updateRAT0CW1NonSpatialMulSubBandInfo
 * Inputs         : subBandNum - This holds subBand Number whose Information
 *                               need to be updated,
 *                  newCqiIndexCW1 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  oldCqiForRBG - This holds Old CQI for RBG,
 * Outputs        : cqiInfoNonSpatialMul_p - This holds pointer to
 *                                           CQIInfoNonSpatialMul structure
 * Returns        : None
 * Description    : This funtion will update the RAT 0 Information stored in  
 *                  cqiInfoNonSpatialMul_p structure for Non Spatial Multiplexing.
 **********************************************************************************/
STATIC  void updateRAT0CW1NonSpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 newCqiIndexCW1,
        UInt32 oldCqiForRBG,
        CQIInfoNonSpatialMul *cqiInfoNonSpatialMul_p)
{
    RAT0NonSpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
    /* + CQI_4.1 */
    UInt32 rbgIndex = 0;
    UInt32 setPosition = 0;
    
    rat0CQIInfo_p = &cqiInfoNonSpatialMul_p->rat0NonSpatialMulCQIInfo;
    
    rbgIndex = subBandNum;
    
    if ( oldCqiForRBG < newCqiIndexCW1 )
    {    
        setPosition = ((UInt32)1 << rbgIndex);
        
        while (newCqiIndexCW1 > oldCqiForRBG)
        {

            rat0CQIInfo_p->rat0CQIBitMap[newCqiIndexCW1] |= setPosition;

            newCqiIndexCW1--;
        }
    }
   /* - CQI_4.1 */

    return;
}

/*+EICIC .*/
/**********************************************************************************
 * Function Name  : updateRAT0CW1NonSpatialMulSubBandInfo
 * Inputs         : subBandNum - This holds subBand Number whose Information
 *                               need to be updated,
 *                  newCqiIndexCW1 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  oldCqiForRBG - This holds Old CQI for RBG,
 * Outputs        : cqiInfoNonSpatialMul_p - This holds pointer to
 *                                           CQIInfoNonSpatialMul structure
 * Returns        : None
 * Description    : This funtion will update the RAT 0 Information stored in  
 *                  cqiInfoNonSpatialMul_p structure for Non Spatial Multiplexing.
 **********************************************************************************/
STATIC  void
updateRAT0CW1NonSpatialMulSubBandInfo_2 (UInt32 subBandNum,
      UInt32 newCqiIndexCW1,
      UInt32 oldCqiForRBG,
      CQIInfoNonSpatialMul *
      cqiInfoNonSpatialMul_p)
{
  RAT0NonSpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
  /* + CQI_4.1 */
  UInt32 rbgIndex = 0;
  UInt32 setPosition = 0;

  rat0CQIInfo_p = &cqiInfoNonSpatialMul_p->rat0NonSpatialMulCQIInfo;

  rbgIndex = subBandNum;

  if (oldCqiForRBG < newCqiIndexCW1)
    {
      setPosition = ((UInt32) 1 << rbgIndex);

      while (newCqiIndexCW1 > oldCqiForRBG)
 {

   rat0CQIInfo_p->rat0CQIBitMap_2[newCqiIndexCW1] |= setPosition;

   newCqiIndexCW1--;
 }
    }
  /* - CQI_4.1 */

  return;
}

/*-EICIC .*/

/**********************************************************************************
 * Function Name  : resetRAT0CW1NonSpatialMulSubBandInfo
 * Inputs         : rbgIndex - This holds rbgIndex Index which will be used to 
 *                             get relevant Information,
 *                  newCqiIndexCW1 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  oldCqiIndexCW1 - This holds Old CQI index of the subBand
 *                                   whose Information need to be updated,  
 * Outputs        : cqiInfoNonSpatialMul_p - This holds pointer to 
 *                                           CQIInfoNonSpatialMul structure
 * Returns        : None
 * Description    : This funtion will reset the RAT 0 Information stored in  
 *                  cqiInfoNonSpatialMul_p structure for Non Spatial Multiplexing.
 **********************************************************************************/
STATIC  void resetRAT0CW1NonSpatialMulSubBandInfo(
        UInt32 rbgIndex,
        UInt32 newCqiIndexCW1,   
        UInt32 oldCqiIndexCW1,
        CQIInfoNonSpatialMul *cqiInfoNonSpatialMul_p)
{
    RAT0NonSpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
    rat0CQIInfo_p = &cqiInfoNonSpatialMul_p->rat0NonSpatialMulCQIInfo;
    /* + CQI_4.1 */
    UInt32 resetPosition = 0; /* It will contains the position to be reset in the
                                 bitmaps */

    resetPosition = (~((UInt32)1 << rbgIndex));
     
    /* It will reset CQI index bitmap's from newCqiIndexCW1 + 1 to oldCQIIndexCW1
     * so that we do not need to update the CQI index bitmaps in 
     * updateRAT0CW1NonSpatialMulSubBandInfo function as lower CQI indexs are 
     * already set for the particlar RBG Index in previous report.
     */

    while ((newCqiIndexCW1++  < oldCqiIndexCW1) && (newCqiIndexCW1 < MAX_CQI_INDEX))/*GCC_4.9_FIX*/
    {
         rat0CQIInfo_p->rat0CQIBitMap[newCqiIndexCW1] &= resetPosition;
    } 

    /* Resetting old Cqi Index from reportedCQIBitMap */
    rat0CQIInfo_p->reportedCQIBitMap &=
                     (~((UInt32)1 << ((MAX_CQI_INDEX - oldCqiIndexCW1) - 1)));  
    
    /* - CQI_4.1 */
    return;
}

/*+EICIC .*/
/**********************************************************************************
 * Function Name  : resetRAT0CW1NonSpatialMulSubBandInfo_2
 * Inputs         : rbgIndex - This holds rbgIndex Index which will be used to 
 *                             get relevant Information,
 *                  newCqiIndexCW1 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  oldCqiIndexCW1 - This holds Old CQI index of the subBand
 *                                   whose Information need to be updated,  
 * Outputs        : cqiInfoNonSpatialMul_p - This holds pointer to 
 *                                           CQIInfoNonSpatialMul structure
 * Returns        : None
 * Description    : This funtion will reset the RAT 0 Information stored in  
 *                  cqiInfoNonSpatialMul_p structure for Non Spatial Multiplexing.
 **********************************************************************************/
STATIC  void
resetRAT0CW1NonSpatialMulSubBandInfo_2 (UInt32 rbgIndex,
     UInt32 newCqiIndexCW1,
     UInt32 oldCqiIndexCW1,
     CQIInfoNonSpatialMul *
     cqiInfoNonSpatialMul_p)
{
  RAT0NonSpatialMulCQIInfo *rat0CQIInfo_p = PNULL;
  rat0CQIInfo_p = &cqiInfoNonSpatialMul_p->rat0NonSpatialMulCQIInfo;
  /* + CQI_4.1 */
  UInt32 resetPosition = 0; /* It will contains the position to be reset in the
       bitmaps */

  resetPosition = (~((UInt32) 1 << rbgIndex));

  /* It will reset CQI index bitmap's from newCqiIndexCW1 + 1 to oldCQIIndexCW1
   * so that we do not need to update the CQI index bitmaps in 
   * updateRAT0CW1NonSpatialMulSubBandInfo function as lower CQI indexs are 
   * already set for the particlar RBG Index in previous report.
   */

  while (newCqiIndexCW1++ < oldCqiIndexCW1)
    {
      rat0CQIInfo_p->rat0CQIBitMap_2[newCqiIndexCW1] &= resetPosition;
    }

  /* Resetting old Cqi Index from reportedCQIBitMap */
  rat0CQIInfo_p->reportedCQIBitMap_2 &=
    (~((UInt32) 1 << ((MAX_CQI_INDEX - oldCqiIndexCW1) - 1)));

  /* - CQI_4.1 */
  return;
}

/*********************************************************************************
 * Function Name  : resetRAT1CW1SpatialMulSubBandInfo_2
 * Inputs         : subBandNum - This holds subBand Number whose Information
 *                               need to be updated,
 *                  oldCqiIndexCW1 - This holds Old CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  rat0Info_p - This stores the address of RAT0Info structure,
 *                  rat1Info_p - This stores the RAT1Info structure
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will reset  the RAT 1 Information stored in  
 *                      cqiInfoSpatialMul_p structure for  Spatial Multiplexing.
 *********************************************************************************/

STATIC  void
resetRAT1CW1SpatialMulSubBandInfo_2 (UInt32 subBandNum,
         UInt32 oldCqiIndexCW1,
         CQIInfoSpatialMul * cqiInfoSpatialMul_p,
         RAT0Info * rat0Info_p,
         RAT1Info * rat1Info_p)
{
  RAT1SpatialMulCQIInfo *rat1CQIInfo_p = PNULL;
  SubsetInfo *subSetInfo_p = PNULL;
  UInt32 subsetBitMap = 0;
  UInt32 subSetIndex = 0;
  UInt32 subsetBitMapZeroSpan = 0;
  UInt32 subsetBitMapNonZeroSpan = 0;
  UInt32 tempBitMap = 0;
  RBGIndexInfo *rbgIndexInfo_p = PNULL;

  rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[subBandNum];
  subSetIndex = rbgIndexInfo_p->subsetIndex;
  rat1CQIInfo_p = &cqiInfoSpatialMul_p->rat1SpatialMulCQIInfo[subSetIndex];
  subSetInfo_p = &rat1Info_p->subsetInfo[subSetIndex];
  subsetBitMap = ~(rbgIndexInfo_p->rbgSizeBitMapForSubset);
  subsetBitMapZeroSpan = subSetInfo_p->bitMapWithSpanZero;
  subsetBitMapNonZeroSpan = subSetInfo_p->bitMapWithSpanNonZero;
  rat1CQIInfo_p->subsetCQIBitMapALLCQIIndex &= (~subsetBitMap);

  /*Checking in which range Old CQI Index falls */

  /* + CQI_4.1 */
  if ((oldCqiIndexCW1 >= DL_64QAM_LOWEST_CQI_INDEX) &&
      (oldCqiIndexCW1 <= DL_64QAM_HIGHEST_CQI_INDEX))
    {
      /*Comes here when the old CQI Index falls in 64QAM SubBands */
      /*Checking if the old CQI Index falls in left Span or not */
      tempBitMap = subsetBitMap & subsetBitMapZeroSpan;
      if (tempBitMap)
 {
   /*Comes here when old CQI Index falls in Left Span */
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.count64QAMSubBandCW1_2--;
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW1_2--;
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW1_2--;

   rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat164QAMCQIBitMapCW1_2
     &= (~subsetBitMap) & subsetBitMapZeroSpan;
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW1_2
     &= (~subsetBitMap) & subsetBitMapZeroSpan;
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1_2
     &= (~subsetBitMap) & subsetBitMapZeroSpan;
 }

      /*Checking if the old CQI Index falls in Right Span or not */
      tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

      if (tempBitMap)
 {
   /*Comes here when old CQI Index falls in Right Span */
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count64QAMSubBandCW1_2--;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW1_2--;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW1_2--;

   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat164QAMCQIBitMapCW1_2
     &= (~subsetBitMap) & subsetBitMapNonZeroSpan;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW1_2
     &= (~subsetBitMap) & subsetBitMapNonZeroSpan;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1_2
     &= (~subsetBitMap) & subsetBitMapNonZeroSpan;
 }
    }

  /*Checking in which range Old CQI Index falls */
  else if ((oldCqiIndexCW1 >= DL_16QAM_LOWEST_CQI_INDEX) &&
    (oldCqiIndexCW1 <= DL_16QAM_HIGHEST_CQI_INDEX))
    {
      /*Comes here when the old CQI Index falls in16QAM SubBands */
      /*Checking if the old CQI Index falls in left Span or not */
      tempBitMap = subsetBitMap & subsetBitMapZeroSpan;
      if (tempBitMap)
 {
   /*Comes here when old CQI Index falls in Left Span */
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW1_2--;
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW1_2--;

   rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW1_2
     &= (~subsetBitMap) & subsetBitMapZeroSpan;
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1_2
     &= (~subsetBitMap) & subsetBitMapZeroSpan;
 }

      /*Checking if the old CQI Index falls in Right Span or not */
      tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

      if (tempBitMap)
 {
   /*Comes here when old CQI Index falls in Right Span */
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW1_2--;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW1_2--;

   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW1_2
     &= (~subsetBitMap) & subsetBitMapNonZeroSpan;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1_2
     &= (~subsetBitMap) & subsetBitMapNonZeroSpan;
 }
    }

  /*Checking in which range Old CQI Index falls */
  else if ((oldCqiIndexCW1 >= DL_QPSK_LOWEST_CQI_INDEX) &&
    (oldCqiIndexCW1 <= DL_QPSK_HIGHEST_CQI_INDEX))
    {
      /*Comes here when the old CQI Index falls in QPSK SubBands */
      /*Checking if the old CQI Index falls in left Span or not */
      tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

      if (tempBitMap)
 {
   /*Comes here when old CQI Index falls in Left Span */
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW1_2--;

   rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1_2
     &= (~subsetBitMap) & subsetBitMapZeroSpan;
 }

      /*Checking if the old CQI Index falls in Right Span or not */
      tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

      if (tempBitMap)
 {
   /*Comes here when old CQI Index falls in Right Span */
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW1_2--;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1_2
     &= (~subsetBitMap) & subsetBitMapNonZeroSpan;
 }
    }
  /* - CQI_4.1 */

  return;
}


/*-EICIC .*/

/*********************************************************************************
 * Function Name  : resetRAT1CW1SpatialMulSubBandInfo
 * Inputs         : subBandNum - This holds subBand Number whose Information
 *                               need to be updated,
 *                  oldCqiIndexCW1 - This holds Old CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  rat0Info_p - This stores the address of RAT0Info structure,
 *                  rat1Info_p - This stores the RAT1Info structure
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will reset  the RAT 1 Information stored in  
 *                      cqiInfoSpatialMul_p structure for  Spatial Multiplexing.
 *********************************************************************************/

STATIC  void resetRAT1CW1SpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 oldCqiIndexCW1,
        CQIInfoSpatialMul *cqiInfoSpatialMul_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p)
{
    RAT1SpatialMulCQIInfo *rat1CQIInfo_p = PNULL;
    SubsetInfo *subSetInfo_p = PNULL;
    UInt32 subsetBitMap = 0;
    UInt32 subSetIndex = 0;
    UInt32 subsetBitMapZeroSpan = 0;
    UInt32 subsetBitMapNonZeroSpan = 0;
    UInt32 tempBitMap = 0;
    RBGIndexInfo *rbgIndexInfo_p = PNULL;
    
    rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[subBandNum];
    subSetIndex = rbgIndexInfo_p->subsetIndex;
    rat1CQIInfo_p = &cqiInfoSpatialMul_p->rat1SpatialMulCQIInfo[subSetIndex];
    subSetInfo_p = &rat1Info_p->subsetInfo[subSetIndex];
    subsetBitMap = ~(rbgIndexInfo_p->rbgSizeBitMapForSubset);
    subsetBitMapZeroSpan = subSetInfo_p->bitMapWithSpanZero;
    subsetBitMapNonZeroSpan = subSetInfo_p->bitMapWithSpanNonZero;
    rat1CQIInfo_p->subsetCQIBitMapALLCQIIndex &= (~subsetBitMap);
    
    /*Checking in which range Old CQI Index falls*/

    /* + CQI_4.1 */
    if ((oldCqiIndexCW1 >= DL_64QAM_LOWEST_CQI_INDEX) && 
                     (oldCqiIndexCW1 <= DL_64QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in 64QAM SubBands*/
        /*Checking if the old CQI Index falls in left Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;
        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Left Span*/
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count64QAMSubBandCW1--;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW1--;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW1--;
            
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat164QAMCQIBitMapCW1 
                &= (~subsetBitMap) & subsetBitMapZeroSpan ;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW1 
                &= (~subsetBitMap) & subsetBitMapZeroSpan ;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1 
                &= (~subsetBitMap) & subsetBitMapZeroSpan ;
        }
    
        /*Checking if the old CQI Index falls in Right Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
        
        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Right Span*/
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count64QAMSubBandCW1--;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW1--;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW1--;
            
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat164QAMCQIBitMapCW1 
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan ;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW1 
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan ;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1 
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan ;
        }
    }

    /*Checking in which range Old CQI Index falls*/
    else if ((oldCqiIndexCW1 >= DL_16QAM_LOWEST_CQI_INDEX) && 
                          (oldCqiIndexCW1 <= DL_16QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in16QAM SubBands*/
        /*Checking if the old CQI Index falls in left Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;
        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Left Span*/
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW1--;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW1--;
            
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW1 
                &= (~subsetBitMap) & subsetBitMapZeroSpan ;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1 
                &= (~subsetBitMap) & subsetBitMapZeroSpan ;
        }
    
        /*Checking if the old CQI Index falls in Right Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
        
        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Right Span*/
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW1--;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW1--;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW1 
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan ;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1 
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan ;
        }
    }

    /*Checking in which range Old CQI Index falls*/
    else if ((oldCqiIndexCW1 >= DL_QPSK_LOWEST_CQI_INDEX) && 
                          (oldCqiIndexCW1 <= DL_QPSK_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in QPSK SubBands*/
        /*Checking if the old CQI Index falls in left Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;
    
        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Left Span*/
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW1--;
            
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1 
                &= (~subsetBitMap) & subsetBitMapZeroSpan ;
        }
        
        /*Checking if the old CQI Index falls in Right Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
        
        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Right Span*/
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW1--;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1 
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan ;
        }
    }
   /* - CQI_4.1 */

    return;
}

/********************************************************************************
 * Function Name  : updateRAT1CW1SpatialMulSubBandInfo
 * Inputs         : subBandNum - This holds subBand Number whose Information
 *                               need to be updated,
 *                  newCqiIndexCW1 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  subbandPMIValue -
 *                  rat0Info_p - This stores the address of RAT0Info structure,
 *                  rat1Info_p - This stores the RAT1Info structure
 *                  numOfTxAnteenas -
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will update  the RAT 1 Information stored in  
 *                  cq1InfoSpatialMul_p structure for  Spatial Multiplexing.
 ********************************************************************************/ 
STATIC  void updateRAT1CW1SpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 newCqiIndexCW1,
        /* + CQI_5.0 */
        UInt8 subbandPMIValue,
        /* + CQI_5.0 */
        CQIInfoSpatialMul *cqiInfoSpatialMul_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p,
        /* CA Changes start */
        UInt8  numOfTxAnteenas)
        /* CA Changes end */
{
    RAT1SpatialMulCQIInfo *rat1CQIInfo_p = PNULL;
    SubsetInfo *subSetInfo_p = PNULL;
    UInt32 subsetBitMap = 0;
    UInt32 subSetIndex = 0;
    UInt32 subsetBitMapZeroSpan = 0;
    UInt32 subsetBitMapNonZeroSpan = 0;
    RBGIndexInfo *rbgIndexInfo_p = PNULL;
    /* + CQI_5.0 */
    RAT1PmiInfo *rat1PmiInfo_p = PNULL; 
    UInt32 tempZeroBitMap = 0;
    UInt32 tempNonZeroBitMap = 0;
    /* - CQI_5.0 */
    
    rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[subBandNum];
    subSetIndex = rbgIndexInfo_p->subsetIndex;
    rat1CQIInfo_p = &cqiInfoSpatialMul_p->rat1SpatialMulCQIInfo[subSetIndex];
    /* + CQI_5.0 */
    rat1PmiInfo_p = 
        &cqiInfoSpatialMul_p->pmiInfo.rat1PmiInfo[subSetIndex];
    /* - CQI_5.0 */
    subSetInfo_p = &rat1Info_p->subsetInfo[subSetIndex];
    subsetBitMap = ~(rbgIndexInfo_p->rbgSizeBitMapForSubset);
    subsetBitMapZeroSpan = subSetInfo_p->bitMapWithSpanZero;
    subsetBitMapNonZeroSpan = subSetInfo_p->bitMapWithSpanNonZero;
    rat1CQIInfo_p->subsetCQIBitMapALLCQIIndex |= subsetBitMap;
    /* TDD SB_CQI */
    UInt8 iteration = 0;
    /* TDD SB_CQI */
    
    /*Checking in which range Old CQI Index falls*/
    /* + CQI_4.1 */
   
    /* + CQI_5.0 */
    if(newCqiIndexCW1)
    {    
        /* Cyclomatic Complexity changes start */ 
        checkSubBandRangeOfNewCQIIndexCW1(newCqiIndexCW1,subsetBitMap, 
                                           subsetBitMapZeroSpan,
                                           subsetBitMapNonZeroSpan,
                                           rat1CQIInfo_p);   
        /* Cyclomatic Complexity changes end */ 
    }
    
    /*4x2 MIMO S*/
    /* CA Changes start */
    if((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2 \
        )&& (INVALID_PMI_VALUE != subbandPMIValue) && 
        (subbandPMIValue < PMI_INDEX_RANK_1))     
    /* CA Changes end */
    {        
        tempZeroBitMap = subsetBitMap & subsetBitMapZeroSpan;
        tempNonZeroBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
        if(tempZeroBitMap)
        {
            /* TDD SB_CQI */
		    for( ; iteration < PMI_INDEX_RANK_1; iteration++)
		    {
			    if(subbandPMIValue == iteration)
			    {
				    rat1PmiInfo_p->spanZeroPmiInfo.
					    reportedPRBsRank1[iteration] |= tempZeroBitMap; 
			    }
			    else
			    {
				    rat1PmiInfo_p->spanZeroPmiInfo.
					    reportedPRBsRank1[iteration] &= (~tempZeroBitMap);
			    }
		    }   
            /* TDD SB_CQI */
        }
        if(tempNonZeroBitMap)
        {    
            /* TDD SB_CQI */
		    for(iteration = 0; iteration < PMI_INDEX_RANK_1; iteration++)
		    {
			    if(subbandPMIValue == iteration)
			    {
				    rat1PmiInfo_p->spanNonZeroPmiInfo.
					    reportedPRBsRank1[iteration] |= tempNonZeroBitMap;
			    }
			    else
			    {
				    rat1PmiInfo_p->spanNonZeroPmiInfo.
					    reportedPRBsRank1[iteration] &= (~tempNonZeroBitMap);
			    }
		    }
            /* TDD SB_CQI */
        }
    }
        /* CA Changes start */
    else if((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4 \
        )&& (INVALID_PMI_VALUE != subbandPMIValue) && 
       (subbandPMIValue < PMI_INDEX_RANK))     
        /* CA Changes end */
    {        
        tempZeroBitMap = subsetBitMap & subsetBitMapZeroSpan;
        tempNonZeroBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
        if(tempZeroBitMap)
        {    
            rat1PmiInfo_p->spanZeroPmiInfo.
                reportedRBGPMIForPort4[subbandPMIValue] |= tempZeroBitMap; 
        }
        if(tempNonZeroBitMap)
        {    
            rat1PmiInfo_p->spanNonZeroPmiInfo.
                reportedRBGPMIForPort4[subbandPMIValue] |= tempNonZeroBitMap;
        }
    }
    /*4x2 MIMO E*/
    /* - CQI_5.0 */
    return;
}

/*+EICIC .*/
/********************************************************************************
 * Function Name  : updateRAT1CW1SpatialMulSubBandInfo_2
 * Inputs         : subBandNum - This holds subBand Number whose Information
 *                               need to be updated,
 *                  newCqiIndexCW1 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  subbandPMIValue -
 *                  rat0Info_p - This stores the address of RAT0Info structure,
 *                  rat1Info_p - This stores the RAT1Info structure
 *                  numOfTxAnteenas -
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will update  the RAT 1 Information stored in  
 *                  cq1InfoSpatialMul_p structure for  Spatial Multiplexing.
 ********************************************************************************/
STATIC  void
updateRAT1CW1SpatialMulSubBandInfo_2 (UInt32 subBandNum,
          UInt32 newCqiIndexCW1,
          /* + CQI_5.0 */
          UInt8 subbandPMIValue,
          /* + CQI_5.0 */
          CQIInfoSpatialMul * cqiInfoSpatialMul_p,
          RAT0Info * rat0Info_p,
          RAT1Info * rat1Info_p,
          /* CA Changes start */
          UInt8 numOfTxAnteenas)
 /* CA Changes end */
{
  RAT1SpatialMulCQIInfo *rat1CQIInfo_p = PNULL;
  SubsetInfo *subSetInfo_p = PNULL;
  UInt32 subsetBitMap = 0;
  UInt32 subSetIndex = 0;
  UInt32 subsetBitMapZeroSpan = 0;
  UInt32 subsetBitMapNonZeroSpan = 0;
  RBGIndexInfo *rbgIndexInfo_p = PNULL;
  /* + CQI_5.0 */
  RAT1PmiInfo *rat1PmiInfo_p = PNULL;
  UInt32 tempZeroBitMap = 0;
  UInt32 tempNonZeroBitMap = 0;
  /* - CQI_5.0 */

  rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[subBandNum];
  subSetIndex = rbgIndexInfo_p->subsetIndex;
  rat1CQIInfo_p = &cqiInfoSpatialMul_p->rat1SpatialMulCQIInfo[subSetIndex];
  /* + CQI_5.0 */
  rat1PmiInfo_p = &cqiInfoSpatialMul_p->pmiInfo.rat1PmiInfo[subSetIndex];
  /* - CQI_5.0 */
  subSetInfo_p = &rat1Info_p->subsetInfo[subSetIndex];
  subsetBitMap = ~(rbgIndexInfo_p->rbgSizeBitMapForSubset);
  subsetBitMapZeroSpan = subSetInfo_p->bitMapWithSpanZero;
  subsetBitMapNonZeroSpan = subSetInfo_p->bitMapWithSpanNonZero;
  rat1CQIInfo_p->subsetCQIBitMapALLCQIIndex_2 |= subsetBitMap;
  /* TDD SB_CQI */
  UInt8 iteration = 0;
  /* TDD SB_CQI */

  /*Checking in which range Old CQI Index falls */
  /* + CQI_4.1 */

  /* + CQI_5.0 */
  if (newCqiIndexCW1)
    {
      /* Cyclomatic Complexity changes start */
      checkSubBandRangeOfNewCQIIndexCW1_2 (newCqiIndexCW1, subsetBitMap,
        subsetBitMapZeroSpan,
        subsetBitMapNonZeroSpan,
        rat1CQIInfo_p);
      /* Cyclomatic Complexity changes end */
    }

  /*4x2 MIMO S */
  /* CA Changes start */
  if ((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
      && (INVALID_PMI_VALUE != subbandPMIValue)
      && (subbandPMIValue < PMI_INDEX_RANK_1))
    /* CA Changes end */
    {
      tempZeroBitMap = subsetBitMap & subsetBitMapZeroSpan;
      tempNonZeroBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
      if (tempZeroBitMap)
 {
   /* TDD SB_CQI */
   for (; iteration < PMI_INDEX_RANK_1; iteration++)
     {
       if (subbandPMIValue == iteration)
  {
    rat1PmiInfo_p->
      spanZeroPmiInfo.reportedPRBsRank1_2[iteration] |=
      tempZeroBitMap;
  }
       else
  {
    rat1PmiInfo_p->
      spanZeroPmiInfo.reportedPRBsRank1_2[iteration] &=
      (~tempZeroBitMap);
  }
     }
   /* TDD SB_CQI */
 }
      if (tempNonZeroBitMap)
 {
   /* TDD SB_CQI */
   for (iteration = 0; iteration < PMI_INDEX_RANK_1; iteration++)
     {
       if (subbandPMIValue == iteration)
  {
    rat1PmiInfo_p->
      spanNonZeroPmiInfo.reportedPRBsRank1_2[iteration] |=
      tempNonZeroBitMap;
  }
       else
  {
    rat1PmiInfo_p->
      spanNonZeroPmiInfo.reportedPRBsRank1_2[iteration] &=
      (~tempNonZeroBitMap);
  }
     }
   /* TDD SB_CQI */
 }
    }
  /* CA Changes start */
  else if ((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    && (INVALID_PMI_VALUE != subbandPMIValue)
    && (subbandPMIValue < PMI_INDEX_RANK))
    /* CA Changes end */
    {
      tempZeroBitMap = subsetBitMap & subsetBitMapZeroSpan;
      tempNonZeroBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
      if (tempZeroBitMap)
 {
   rat1PmiInfo_p->
     spanZeroPmiInfo.reportedRBGPMIForPort4_2[subbandPMIValue] |=
     tempZeroBitMap;
 }
      if (tempNonZeroBitMap)
 {
   rat1PmiInfo_p->
     spanNonZeroPmiInfo.reportedRBGPMIForPort4_2[subbandPMIValue] |=
     tempNonZeroBitMap;
 }
    }
  /*4x2 MIMO E */
  /* - CQI_5.0 */
  return;
}

/********************************************************************************
 * Function Name  : checkSubBandRangeOfNewCQIIndexCW1_2
 *                  newCqiIndexCW1 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated.
 *                  subsetBitMap
 *                  subsetBitMapZeroSpan
 *                  subsetBitMapNonZeroSpan
 * Outputs        : rat1CQIInfo_p  
 * Returns        : None
 * Description    : This funtion will check subband range of new cqi index and 
 *                  update  the RAT 1 Information stored in  
 *                  cq1InfoSpatialMul_p structure for  Spatial Multiplexing.
 ********************************************************************************/
STATIC  void
checkSubBandRangeOfNewCQIIndexCW1_2 (UInt32 newCqiIndexCW1,
         UInt32 subsetBitMap,
         UInt32 subsetBitMapZeroSpan,
         UInt32 subsetBitMapNonZeroSpan,
         RAT1SpatialMulCQIInfo * rat1CQIInfo_p)
{
  UInt32 tempBitMap = 0;
  /* - CQI_5.0 */
  if ((newCqiIndexCW1 >= DL_64QAM_LOWEST_CQI_INDEX) &&
      (newCqiIndexCW1 <= DL_64QAM_HIGHEST_CQI_INDEX))
    {
      /*Comes here when the new CQI Index falls in 64QAM SubBands */
      /*Checking if the old CQI Index falls in left Span or not */
      tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

      if (tempBitMap)
 {
   /*Comes here when new CQI Index falls in Left Span */
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.count64QAMSubBandCW1_2++;
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW1_2++;
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW1_2++;

   rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat164QAMCQIBitMapCW1_2
     |= tempBitMap;
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW1_2
     |= tempBitMap;
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1_2
     |= tempBitMap;
 }

      /*Checking if the new CQI Index falls in Right Span or not */
      tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

      if (subsetBitMap & subsetBitMapNonZeroSpan)
 {
   /*Comes here when new CQI Index falls in Right Span */
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count64QAMSubBandCW1_2++;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW1_2++;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW1_2++;

   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat164QAMCQIBitMapCW1_2
     |= tempBitMap;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW1_2
     |= tempBitMap;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1_2
     |= tempBitMap;

 }
    }
  /*Checking in which range new CQI Index falls */
  else if ((newCqiIndexCW1 >= DL_16QAM_LOWEST_CQI_INDEX) &&
    (newCqiIndexCW1 <= DL_16QAM_HIGHEST_CQI_INDEX))
    {
      /*Comes here when the new CQI Index falls in16QAM SubBands */
      /*Checking if the new CQI Index falls in left Span or not */
      tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

      if (tempBitMap)
 {
   /*Comes here when new CQI Index falls in Left Span */
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW1_2++;
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW1_2++;

   rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW1_2
     |= tempBitMap;
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1_2
     |= tempBitMap;
 }

      /*Checking if the new CQI Index falls in Right Span or not */
      tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

      if (tempBitMap)
 {
   /*Comes here when new CQI Index falls in Right Span */
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW1_2++;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW1_2++;

   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW1_2
     |= tempBitMap;
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1_2
     |= tempBitMap;

 }
    }

  /*Checking in which range new CQI Index falls */
  else if ((newCqiIndexCW1 >= DL_QPSK_LOWEST_CQI_INDEX) &&
    (newCqiIndexCW1 <= DL_QPSK_HIGHEST_CQI_INDEX))
    {
      /*Comes here when the new CQI Index falls in QPSK SubBands */
      /*Checking if the new CQI Index falls in left Span or not */
      tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

      if (tempBitMap)
 {
   /*Comes here when new CQI Index falls in Left Span */
   rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW1_2++;

   rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1_2
     |= tempBitMap;
 }

      /*Checking if the new CQI Index falls in Right Span or not */
      tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

      if (tempBitMap)
 {
   /*Comes here when new CQI Index falls in Right Span */
   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW1_2++;

   rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1_2
     |= tempBitMap;
 }
    }
  /* - CQI_4.1 */
  /* + CQI_5.0 */
}

/*-EICIC .*/
/********************************************************************************
 * Function Name  : checkSubBandRangeOfNewCQIIndexCW1
 *                  newCqiIndexCW1 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated.
 *                  subsetBitMap
 *                  subsetBitMapZeroSpan
 *                  subsetBitMapNonZeroSpan
 * Outputs        : rat1CQIInfo_p  
 * Returns        : None
 * Description    : This funtion will check subband range of new cqi index and 
 *                  update  the RAT 1 Information stored in  
 *                  cq1InfoSpatialMul_p structure for  Spatial Multiplexing.
 ********************************************************************************/ 
STATIC  void checkSubBandRangeOfNewCQIIndexCW1(
        UInt32 newCqiIndexCW1,
        UInt32 subsetBitMap,
        UInt32 subsetBitMapZeroSpan,
        UInt32 subsetBitMapNonZeroSpan,
        RAT1SpatialMulCQIInfo *rat1CQIInfo_p)
{
    UInt32 tempBitMap = 0;
    /* - CQI_5.0 */
    if ((newCqiIndexCW1 >= DL_64QAM_LOWEST_CQI_INDEX) && 
            (newCqiIndexCW1 <= DL_64QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the new CQI Index falls in 64QAM SubBands*/
        /*Checking if the old CQI Index falls in left Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Left Span*/
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count64QAMSubBandCW1++;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW1++;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW1++;

            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat164QAMCQIBitMapCW1 
                |= tempBitMap;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW1
                |= tempBitMap ;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1
                |= tempBitMap ;
        }

        /*Checking if the new CQI Index falls in Right Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

        if (subsetBitMap & subsetBitMapNonZeroSpan)
        {
            /*Comes here when new CQI Index falls in Right Span*/
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count64QAMSubBandCW1++;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW1++;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW1++;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat164QAMCQIBitMapCW1 
                |= tempBitMap;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW1
                |= tempBitMap ;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1
                |=tempBitMap ;

        }       
    }
    /*Checking in which range new CQI Index falls*/
    else if ((newCqiIndexCW1 >= DL_16QAM_LOWEST_CQI_INDEX) && 
            (newCqiIndexCW1 <= DL_16QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the new CQI Index falls in16QAM SubBands*/
        /*Checking if the new CQI Index falls in left Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Left Span*/
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW1++;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW1++;

            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW1 
                |= tempBitMap ;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1
                |= tempBitMap ;
        }

        /*Checking if the new CQI Index falls in Right Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Right Span*/
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW1++;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW1++;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW1 
                |= tempBitMap ;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1
                |=tempBitMap ;

        }       
    }

    /*Checking in which range new CQI Index falls*/
    else if ((newCqiIndexCW1 >= DL_QPSK_LOWEST_CQI_INDEX) && 
            (newCqiIndexCW1 <= DL_QPSK_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the new CQI Index falls in QPSK SubBands*/
        /*Checking if the new CQI Index falls in left Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Left Span*/
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW1++;

            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1 
                |= tempBitMap ;
        }

        /*Checking if the new CQI Index falls in Right Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Right Span*/
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW1++;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW1 
                |=tempBitMap ;
        }
    }
    /* - CQI_4.1 */
    /* + CQI_5.0 */
}

/*****************************************************************************
 * Function Name  : resetRAT1CW2SpatialMulSubBandInfo
 * Inputs         : subBandNum - This holds subBand Number whose Information
 *                               need to be updated,
 *                  oldCqiIndexCW2 - This holds Old CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  rat0Info_p - This stores the address of RAT0Info structure,
 *                  rat1Info_p - This stores the RAT1Info structure
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will reset  the RAT 1 Information stored in  
 *                  cqiInfoSpatialMul_p structure for  Spatial Multiplexing.
 *****************************************************************************/
 
STATIC  void resetRAT1CW2SpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 oldCqiIndexCW2,
        CQIInfoSpatialMul *cqiInfoSpatialMul_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p)
{
    RAT1SpatialMulCQIInfo *rat1CQIInfo_p = PNULL;
    SubsetInfo *subSetInfo_p = PNULL;
    UInt32 subsetBitMap = 0;
    UInt32 subSetIndex = 0;
    UInt32 subsetBitMapZeroSpan = 0;
    UInt32 subsetBitMapNonZeroSpan = 0;
    UInt32 tempBitMap = 0;
    RBGIndexInfo *rbgIndexInfo_p = PNULL;
  
    rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[subBandNum];
    subSetIndex = rbgIndexInfo_p->subsetIndex;
    rat1CQIInfo_p = &cqiInfoSpatialMul_p->rat1SpatialMulCQIInfo[subSetIndex];
    subSetInfo_p = &rat1Info_p->subsetInfo[subSetIndex];
    subsetBitMap = ~(rbgIndexInfo_p->rbgSizeBitMapForSubset);
    subsetBitMapZeroSpan = subSetInfo_p->bitMapWithSpanZero;
    subsetBitMapNonZeroSpan = subSetInfo_p->bitMapWithSpanNonZero;
    rat1CQIInfo_p->subsetCQIBitMapALLCQIIndex &= (~subsetBitMap);
    
    /*Checking in which range Old CQI Index falls*/
    /* + CQI_4.1 */
    
    if ((oldCqiIndexCW2 >= DL_64QAM_LOWEST_CQI_INDEX) && 
                     (oldCqiIndexCW2 <= DL_64QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in 64QAM SubBands*/
        /*Checking if the old CQI Index falls in left Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;
    
        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Left Span*/
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count64QAMSubBandCW2--;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW2--;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2--;
            
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat164QAMCQIBitMapCW2 
                &= (~subsetBitMap) & subsetBitMapZeroSpan ;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW2 
                &= (~subsetBitMap) & subsetBitMapZeroSpan ;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2 
                &= (~subsetBitMap) & subsetBitMapZeroSpan ;
        }
        
        /*Checking if the old CQI Index falls in Right Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
        
        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Right Span*/
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count64QAMSubBandCW2--;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW2--;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2--;
            
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat164QAMCQIBitMapCW2 
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan ;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW2 
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan ;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2 
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan ;
        }
    }

    /*Checking in which range Old CQI Index falls*/
    else if ((oldCqiIndexCW2 >= DL_16QAM_LOWEST_CQI_INDEX) && 
                         (oldCqiIndexCW2 <= DL_16QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in16QAM SubBands*/
        /*Checking if the old CQI Index falls in left Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;
    
        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Left Span*/
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW2--;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2--;
            
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW2 
                &= (~subsetBitMap) & subsetBitMapZeroSpan ;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2 
                &= (~subsetBitMap) & subsetBitMapZeroSpan ;
        }
        
        /*Checking if the old CQI Index falls in Right Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
        
        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Right Span*/
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW2--;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2--;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW2 
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan ;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2 
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan ;
        }
    }

    /*Checking in which range Old CQI Index falls*/
    else if ((oldCqiIndexCW2 >= DL_QPSK_LOWEST_CQI_INDEX) && 
                          (oldCqiIndexCW2 <= DL_QPSK_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in QPSK SubBands*/
        /*Checking if the old CQI Index falls in left Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;
    
        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Left Span*/
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2--;
            
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2 
                &= (~subsetBitMap) & subsetBitMapZeroSpan ;
        }
        
        /*Checking if the old CQI Index falls in Right Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
        
        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Right Span*/
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2--;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2 
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan ;
        }
    }
    /* - CQI_4.1 */

    return;
}

/*+EICIC .*/
/*****************************************************************************
 * Function Name  : resetRAT1CW2SpatialMulSubBandInfo_2
 * Inputs         : subBandNum - This holds subBand Number whose Information
 *                               need to be updated,
 *                  oldCqiIndexCW2 - This holds Old CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  rat0Info_p - This stores the address of RAT0Info structure,
 *                  rat1Info_p - This stores the RAT1Info structure
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will reset  the RAT 1 Information stored in  
 *                  cqiInfoSpatialMul_p structure for  Spatial Multiplexing.
 *****************************************************************************/

 STATIC  void resetRAT1CW2SpatialMulSubBandInfo_2 (UInt32 subBandNum,
                                   UInt32 oldCqiIndexCW2,
                                   CQIInfoSpatialMul * cqiInfoSpatialMul_p,
                                   RAT0Info * rat0Info_p,
                                   RAT1Info * rat1Info_p)
{
    RAT1SpatialMulCQIInfo *rat1CQIInfo_p = PNULL;
    SubsetInfo *subSetInfo_p = PNULL;
    UInt32 subsetBitMap = 0;
    UInt32 subSetIndex = 0;
    UInt32 subsetBitMapZeroSpan = 0;
    UInt32 subsetBitMapNonZeroSpan = 0;
    UInt32 tempBitMap = 0;
    RBGIndexInfo *rbgIndexInfo_p = PNULL;

    rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[subBandNum];
    subSetIndex = rbgIndexInfo_p->subsetIndex;
    rat1CQIInfo_p = &cqiInfoSpatialMul_p->rat1SpatialMulCQIInfo[subSetIndex];
    subSetInfo_p = &rat1Info_p->subsetInfo[subSetIndex];
    subsetBitMap = ~(rbgIndexInfo_p->rbgSizeBitMapForSubset);
    subsetBitMapZeroSpan = subSetInfo_p->bitMapWithSpanZero;
    subsetBitMapNonZeroSpan = subSetInfo_p->bitMapWithSpanNonZero;
    rat1CQIInfo_p->subsetCQIBitMapALLCQIIndex_2 &= (~subsetBitMap);

    /*Checking in which range Old CQI Index falls */
    /* + CQI_4.1 */

    if ((oldCqiIndexCW2 >= DL_64QAM_LOWEST_CQI_INDEX) &&
            (oldCqiIndexCW2 <= DL_64QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in 64QAM SubBands */
        /*Checking if the old CQI Index falls in left Span or not */
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Left Span */
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count64QAMSubBandCW2_2--;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW2_2--;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2_2--;

            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat164QAMCQIBitMapCW2_2
                &= (~subsetBitMap) & subsetBitMapZeroSpan;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW2_2
                &= (~subsetBitMap) & subsetBitMapZeroSpan;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2_2
                &= (~subsetBitMap) & subsetBitMapZeroSpan;
        }

        /*Checking if the old CQI Index falls in Right Span or not */
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Right Span */
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count64QAMSubBandCW2_2--;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW2_2--;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2_2--;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat164QAMCQIBitMapCW2_2
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW2_2
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2_2
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan;
        }
    }

    /*Checking in which range Old CQI Index falls */
    else if ((oldCqiIndexCW2 >= DL_16QAM_LOWEST_CQI_INDEX) &&
            (oldCqiIndexCW2 <= DL_16QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in16QAM SubBands */
        /*Checking if the old CQI Index falls in left Span or not */
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Left Span */
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW2_2--;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2_2--;

            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW2_2
                &= (~subsetBitMap) & subsetBitMapZeroSpan;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2_2
                &= (~subsetBitMap) & subsetBitMapZeroSpan;
        }

        /*Checking if the old CQI Index falls in Right Span or not */
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Right Span */
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW2_2--;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2_2--;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW2_2
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2_2
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan;
        }
    }

    /*Checking in which range Old CQI Index falls */
    else if ((oldCqiIndexCW2 >= DL_QPSK_LOWEST_CQI_INDEX) &&
            (oldCqiIndexCW2 <= DL_QPSK_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the old CQI Index falls in QPSK SubBands */
        /*Checking if the old CQI Index falls in left Span or not */
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Left Span */
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2_2--;

            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2_2
                &= (~subsetBitMap) & subsetBitMapZeroSpan;
        }

        /*Checking if the old CQI Index falls in Right Span or not */
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Right Span */
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2_2--;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2_2
                &= (~subsetBitMap) & subsetBitMapNonZeroSpan;
        }
    }
    /* - CQI_4.1 */

    return;
}

/********************************************************************************
 * Function Name  : updateRAT1CW2SpatialMulSubBandInfo
 * Inputs         : subBandNum - This holds subBand Number whose Information
 *                               need to be updated,
 *                  newCqiIndexCW2 - This holds Old CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  subbandPMIValue - PMI Value of subband,
 *                  rat0Info_p - This stores the address of RAT0Info structure,
 *                  rat1Info_p - This stores the RAT1Info structure
 *                  numOfTxAnteenas -
 * Outputs        : cqiInfoSpatialMul_p - This holds pointer to 
 *                                        CQIInfoSpatialMul structure
 * Returns        : None
 * Description    : This funtion will update  the RAT 1 Information stored in  
 *                  cqiInfoSpatialMul_p structure for  Spatial Multiplexing.
 ********************************************************************************/
 
STATIC  void updateRAT1CW2SpatialMulSubBandInfo(
        UInt32 subBandNum,
        UInt32 newCqiIndexCW2,
        /* + CQI_5.0 */
        UInt8 subbandPMIValue,
        /* + CQI_5.0 */
        CQIInfoSpatialMul *cqiInfoSpatialMul_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p,
        /* CA Changes start */
        UInt8  numOfTxAnteenas)
       /* CA Changes end */
{
    RAT1SpatialMulCQIInfo *rat1CQIInfo_p = PNULL;
    SubsetInfo *subSetInfo_p = PNULL;
    UInt32 subsetBitMap = 0;
    UInt32 subSetIndex = 0;
    UInt32 subsetBitMapZeroSpan = 0;
    UInt32 subsetBitMapNonZeroSpan = 0;
    RBGIndexInfo *rbgIndexInfo_p = PNULL;
    /* + CQI_5.0 */
    RAT1PmiInfo *rat1PmiInfo_p = PNULL; 
    UInt32 tempZeroBitMap = 0;
    UInt32 tempNonZeroBitMap = 0;
    /* - CQI_5.0 */
    /* TDD SB_CQI */
    UInt8 iteration = 0;
    /* TDD SB_CQI */

    rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[subBandNum];
    subSetIndex = rbgIndexInfo_p->subsetIndex;
    rat1CQIInfo_p = &cqiInfoSpatialMul_p->rat1SpatialMulCQIInfo[subSetIndex];
    /* + CQI_5.0 */
    rat1PmiInfo_p = 
        &cqiInfoSpatialMul_p->pmiInfo.rat1PmiInfo[subSetIndex];
    /* - CQI_5.0 */
    subSetInfo_p = &rat1Info_p->subsetInfo[subSetIndex];
    subsetBitMap = ~(rbgIndexInfo_p->rbgSizeBitMapForSubset);
    subsetBitMapZeroSpan = subSetInfo_p->bitMapWithSpanZero;
    subsetBitMapNonZeroSpan = subSetInfo_p->bitMapWithSpanNonZero;
    rat1CQIInfo_p->subsetCQIBitMapALLCQIIndex |= subsetBitMap;
    
    /*Checking in which range new CQI Index falls*/
    /* + CQI_4.1 */
    
    /* + CQI_5.0 */
    if(newCqiIndexCW2)
    {    
        /* Cyclomatic Complexity changes start */ 
        checkSubBandRangeOfNewCQIIndexCW2(newCqiIndexCW2,subsetBitMap, 
                                           subsetBitMapZeroSpan,
                                           subsetBitMapNonZeroSpan,
                                           rat1CQIInfo_p);   
        /* Cyclomatic Complexity changes end */ 
    }

    /*4x2 MIMO S*/
    /* CA Changes start */
    if((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2) &&\
         (INVALID_PMI_VALUE != subbandPMIValue) && 
       (subbandPMIValue < PMI_INDEX_RANK_2))     
    /* CA Changes end */
    {        
        tempZeroBitMap = subsetBitMap & subsetBitMapZeroSpan;
        tempNonZeroBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
        if(tempZeroBitMap)
        {
            /* TDD SB_CQI */
		    for( iteration = 0; iteration < PMI_INDEX_RANK_2; iteration++)
		    {
			    if(subbandPMIValue == iteration)
			    {
				    rat1PmiInfo_p->spanZeroPmiInfo.
					    reportedPRBsRank2[iteration] |= tempZeroBitMap;
			    }
			    else
			    {
				    rat1PmiInfo_p->spanZeroPmiInfo.
					    reportedPRBsRank2[iteration] &= (~tempZeroBitMap);
			    }
		    }
            /* TDD SB_CQI */
        }
        if(tempNonZeroBitMap)
        {    
            /* TDD SB_CQI */
		    for(iteration = 0; iteration < PMI_INDEX_RANK_2; iteration++)
		    {
			    if(subbandPMIValue == iteration)
			    {
				    rat1PmiInfo_p->spanNonZeroPmiInfo.
					    reportedPRBsRank2[iteration] |= tempNonZeroBitMap;
			    }
			    else
			    {
				    rat1PmiInfo_p->spanNonZeroPmiInfo.
					    reportedPRBsRank2[iteration] &= (~tempNonZeroBitMap);
			    }
		    }
            /* TDD SB_CQI */
        }
    }
    /* CA Changes start */
    else if((numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4) &&\
         (INVALID_PMI_VALUE != subbandPMIValue) && 
       (subbandPMIValue < PMI_INDEX_RANK))     
    /* CA Changes end */
    {        
        tempZeroBitMap = subsetBitMap & subsetBitMapZeroSpan;
        tempNonZeroBitMap = subsetBitMap & subsetBitMapNonZeroSpan;
        if(tempZeroBitMap)
        {    
            rat1PmiInfo_p->spanZeroPmiInfo.
                reportedRBGPMIForPort4[subbandPMIValue] |= tempZeroBitMap; 
        }
        if(tempNonZeroBitMap)
        {    
            rat1PmiInfo_p->spanNonZeroPmiInfo.
                reportedRBGPMIForPort4[subbandPMIValue] |= tempNonZeroBitMap;
        }
    }
    /*4x2 MIMO E*/
    /* - CQI_5.0 */
    return;
}

/********************************************************************************
 * Function Name  : checkSubBandRangeOfNewCQIIndexCW2
 *                  newCqiIndexCW2 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated.
 *                  subsetBitMap
 *                  subsetBitMapZeroSpan
 *                  subsetBitMapNonZeroSpan
 *                  rat1CQIInfo_p
 * Returns        : None
 * Description    : This funtion will check subband range of new cqi index and 
 *                  update  the RAT 1 Information stored in  
 *                  cq1InfoSpatialMul_p structure for  Spatial Multiplexing.
 ********************************************************************************/ 
STATIC  void checkSubBandRangeOfNewCQIIndexCW2(
        UInt32 newCqiIndexCW2,
        UInt32 subsetBitMap,
        UInt32 subsetBitMapZeroSpan,
        UInt32 subsetBitMapNonZeroSpan,
        RAT1SpatialMulCQIInfo *rat1CQIInfo_p)
{
    UInt32 tempBitMap = 0;
    /* - CQI_5.0 */
    if ((newCqiIndexCW2 >= DL_64QAM_LOWEST_CQI_INDEX) && 
            (newCqiIndexCW2 <= DL_64QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the new CQI Index falls in 64QAM SubBands*/
        /*Checking if the new CQI Index falls in left Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Left Span*/
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count64QAMSubBandCW2++;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW2++;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2++;

            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat164QAMCQIBitMapCW2 
                |= tempBitMap;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW2
                |= tempBitMap ;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2
                |= tempBitMap ;
        }

        /*Checking if the new CQI Index falls in Right Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

        if (subsetBitMap & subsetBitMapNonZeroSpan)
        {
            /*Comes here when new CQI Index falls in Right Span*/
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count64QAMSubBandCW2++;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW2++;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2++;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat164QAMCQIBitMapCW2
                |= tempBitMap;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW2
                |= tempBitMap ;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2
                |=tempBitMap ;
        }       
    }

    /*Checking in which range new CQI Index falls*/
    else if ((newCqiIndexCW2 >= DL_16QAM_LOWEST_CQI_INDEX) && 
            (newCqiIndexCW2 <= DL_16QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the new CQI Index falls in16QAM SubBands*/
        /*Checking if the new CQI Index falls in left Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Left Span*/
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW2++;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2++;

            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW2 
                |= tempBitMap ;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2
                |= tempBitMap ;
        }

        /*Checking if the new CQI Index falls in Right Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Right Span*/
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW2++;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2++;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW2 
                |= tempBitMap ;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2
                |=tempBitMap ;

        }       
    }

    /*Checking in which range new CQI Index falls*/
    else if ((newCqiIndexCW2 >= DL_QPSK_LOWEST_CQI_INDEX) && 
            (newCqiIndexCW2 <= DL_QPSK_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the new CQI Index falls in QPSK SubBands*/
        /*Checking if the new CQI Index falls in left Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Left Span*/
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2++;

            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2 
                |= tempBitMap ;
        }

        /*Checking if the new CQI Index falls in Right Span or not*/
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Right Span*/
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2++;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2 
                |=tempBitMap ;
        }
    }
    /* - CQI_4.1 */
    /* + CQI_5.0 */
}

/*+EICIC .*/
/********************************************************************************
 * Function Name  : checkSubBandRangeOfNewCQIIndexCW2_2
 *                  newCqiIndexCW2 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated.
 *                  subsetBitMap
 *                  subsetBitMapZeroSpan
 *                  subsetBitMapNonZeroSpan
 *                  rat1CQIInfo_p
 * Returns        : None
 * Description    : This funtion will check subband range of new cqi index and 
 *                  update  the RAT 1 Information stored in  
 *                  cq1InfoSpatialMul_p structure for  Spatial Multiplexing.
 ********************************************************************************/
STATIC  void checkSubBandRangeOfNewCQIIndexCW2_2 (UInt32 newCqiIndexCW2,
                                                        UInt32 subsetBitMap,
                                                        UInt32 subsetBitMapZeroSpan,
                                                        UInt32 subsetBitMapNonZeroSpan,
                                                 RAT1SpatialMulCQIInfo * rat1CQIInfo_p) 
{
    UInt32 tempBitMap = 0;
    /* - CQI_5.0 */
    if ((newCqiIndexCW2 >= DL_64QAM_LOWEST_CQI_INDEX) &&
            (newCqiIndexCW2 <= DL_64QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the new CQI Index falls in 64QAM SubBands */
        /*Checking if the new CQI Index falls in left Span or not */
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when old CQI Index falls in Left Span */
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count64QAMSubBandCW2_2++;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW2_2++;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2_2++;

            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat164QAMCQIBitMapCW2_2
                |= tempBitMap;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW2_2
                |= tempBitMap;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2_2
                |= tempBitMap;
        }

        /*Checking if the new CQI Index falls in Right Span or not */
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

        if (subsetBitMap & subsetBitMapNonZeroSpan)
        {
            /*Comes here when new CQI Index falls in Right Span */
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count64QAMSubBandCW2_2++;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW2_2++;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2_2++;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat164QAMCQIBitMapCW2_2
                |= tempBitMap;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW2_2
                |= tempBitMap;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2_2
                |= tempBitMap;
        }
    }

    /*Checking in which range new CQI Index falls */
    else if ((newCqiIndexCW2 >= DL_16QAM_LOWEST_CQI_INDEX) &&
            (newCqiIndexCW2 <= DL_16QAM_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the new CQI Index falls in16QAM SubBands */
        /*Checking if the new CQI Index falls in left Span or not */
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Left Span */
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW2_2++;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2_2++;

            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW2_2
                |= tempBitMap;
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2_2
                |= tempBitMap;
        }

        /*Checking if the new CQI Index falls in Right Span or not */
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Right Span */
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW2_2++;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2_2++;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW2_2
                |= tempBitMap;
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2_2
                |= tempBitMap;

        }
    }

    /*Checking in which range new CQI Index falls */
    else if ((newCqiIndexCW2 >= DL_QPSK_LOWEST_CQI_INDEX) &&
            (newCqiIndexCW2 <= DL_QPSK_HIGHEST_CQI_INDEX))
    {
        /*Comes here when the new CQI Index falls in QPSK SubBands */
        /*Checking if the new CQI Index falls in left Span or not */
        tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Left Span */
            rat1CQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2_2++;

            rat1CQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2_2
                |= tempBitMap;
        }

        /*Checking if the new CQI Index falls in Right Span or not */
        tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

        if (tempBitMap)
        {
            /*Comes here when new CQI Index falls in Right Span */
            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2_2++;

            rat1CQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2_2
                |= tempBitMap;
        }
    }
    /* - CQI_4.1 */
    /* + CQI_5.0 */
}

/*-EICIC .*/
/************************************************************************************
 * Function Name  : resetRAT1CW1NonSpatialMulSubBandInfo
 * Inputs         : rbgIndex - This holds rbgIndex Number which will be used to get 
 *                              relevant information need to be updated,
 *                  newCqiIndexCW1 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  oldCqiIndexCW1 - This holds Old CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  rat0Info_p - This stores the address of RAT0Info structure,
 *                  rat1Info_p - This stores the RAT1Info structure
 * Outputs        : cqiInfoNonSpatialMul_p - This holds pointer to 
 *                                           CQIInfoNonSpatialMul structure
 * Returns        : None
 * Description    : This funtion will reset  the RAT 1 Information stored in  
 *                  cqiInfoNonSpatialMul_p structure for  Non Spatial Multiplexing.
 ************************************************************************************/
STATIC  void resetRAT1CW1NonSpatialMulSubBandInfo(
        UInt32 rbgIndex,
        UInt32 newCqiIndexCW1,
        UInt32 oldCqiIndexCW1,
        CQIInfoNonSpatialMul *cqiInfoNonSpatialMul_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p)
{
    RAT1NonSpatialMulCQIInfo *rat1CQIInfo_p = PNULL;
    SubsetInfo *subSetInfo_p = PNULL;
    UInt32 subsetBitMap = 0;
    UInt32 subSetIndex = 0;
    UInt32 subsetBitMapZeroSpan = 0;
    UInt32 subsetBitMapNonZeroSpan = 0;
    UInt32 tempBitMap = 0;
    RBGIndexInfo *rbgIndexInfo_p = PNULL;
    /* + CQI_4.1 */
    UInt32 tempNewCqiIndexCW1  =  newCqiIndexCW1;
    /* + CQI_4.1 */
    
    rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[rbgIndex];
    subSetIndex = rbgIndexInfo_p->subsetIndex;
    rat1CQIInfo_p = &cqiInfoNonSpatialMul_p->
        rat1NonSpatialMulCQIInfo[subSetIndex];
    subSetInfo_p = &rat1Info_p->subsetInfo[subSetIndex];
    subsetBitMap = rbgIndexInfo_p->rbgSizeBitMapForSubset;
    subsetBitMapZeroSpan = subSetInfo_p->bitMapWithSpanZero;
    subsetBitMapNonZeroSpan = subSetInfo_p->bitMapWithSpanNonZero;
    tempBitMap = (~subsetBitMap) & subsetBitMapZeroSpan;
    rat1CQIInfo_p->subsetCQIBitMapALLCQIIndex &=  subsetBitMap;
    
    /*Checking if the subBand falls in left Span or Right Span*/
    /* + CQI_4.1 */
    
    if (tempBitMap)
    { 
        /*Comes here when the subBand falls in left Span*/

        /* It will reset CQI index bitmap's from newCqiIndexCW1 + 1 to 
         * oldCQIIndexCW1 so that we do not need to update the CQI index bitmaps
         * in updateRAT1CW1NonSpatialMulSubBandInfo function as lower CQI indexs 
         * are already set for the particlar RBG Index in previous report.
         */
        while ((tempNewCqiIndexCW1++ < oldCqiIndexCW1)&& (tempNewCqiIndexCW1 < MAX_CQI_INDEX))/*GCC_4.9_FIX*/
        {
            /*Resetting the subBand information in subsetCQIBitMap bitmap*/
            rat1CQIInfo_p->subsetCQIBitMapZeroSpan[tempNewCqiIndexCW1] 
                                        &= (subsetBitMap & subsetBitMapZeroSpan);
        }   
    }

    /*Checking if the subBand falls in left Span or Right Span*/
    tempBitMap = (~subsetBitMap) & subsetBitMapNonZeroSpan; 
    
    if (tempBitMap)
    { 
        /*Comes here when the subBand falls in Right Span*/
         
        /* It will reset CQI index bitmap's from newCqiIndexCW1 + 1 to 
         * oldCQIIndexCW1 so that we do not need to update the CQI index bitmaps
         * in updateRAT1CW1NonSpatialMulSubBandInfo function as lower CQI indexs 
         * are already set for the particlar RBG Index in previous report.
         */

        tempNewCqiIndexCW1 = newCqiIndexCW1;

        while (tempNewCqiIndexCW1++  < oldCqiIndexCW1 && (tempNewCqiIndexCW1 < MAX_CQI_INDEX))/*GCC_4.9_FIX*/
        {
           /*Resetting the subBand information in subsetCQIBitMap bitmap*/
           rat1CQIInfo_p->
               subsetCQIBitMapNonZeroSpan[tempNewCqiIndexCW1]
                                 &= (subsetBitMap & subsetBitMapNonZeroSpan);
        }
   }    
   
   /* Resetting old Cqi Index from reportedCQIBitMap */
   rat1CQIInfo_p->reportedCQIBitMap &=
                      (~((UInt32)1 << ((MAX_CQI_INDEX - oldCqiIndexCW1) - 1)));  
   /* - CQI_4.1 */
   return;
}

/*+EICIC .*/
/************************************************************************************
 * Function Name  : resetRAT1CW1NonSpatialMulSubBandInfo_2
 * Inputs         : rbgIndex - This holds rbgIndex Number which will be used to get 
 *                              relevant information need to be updated,
 *                  newCqiIndexCW1 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  oldCqiIndexCW1 - This holds Old CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  rat0Info_p - This stores the address of RAT0Info structure,
 *                  rat1Info_p - This stores the RAT1Info structure
 * Outputs        : cqiInfoNonSpatialMul_p - This holds pointer to 
 *                                           CQIInfoNonSpatialMul structure
 * Returns        : None
 * Description    : This funtion will reset  the RAT 1 Information stored in  
 *                  cqiInfoNonSpatialMul_p structure for  Non Spatial Multiplexing.
 ************************************************************************************/
STATIC  void resetRAT1CW1NonSpatialMulSubBandInfo_2 (UInt32 rbgIndex,
                                                     UInt32 newCqiIndexCW1,
                                                     UInt32 oldCqiIndexCW1,
                              CQIInfoNonSpatialMul *cqiInfoNonSpatialMul_p,
                                                     RAT0Info * rat0Info_p,
                                                      RAT1Info * rat1Info_p)
{
    RAT1NonSpatialMulCQIInfo *rat1CQIInfo_p = PNULL;
    SubsetInfo *subSetInfo_p = PNULL;
    UInt32 subsetBitMap = 0;
    UInt32 subSetIndex = 0;
    UInt32 subsetBitMapZeroSpan = 0;
    UInt32 subsetBitMapNonZeroSpan = 0;
    UInt32 tempBitMap = 0;
    RBGIndexInfo *rbgIndexInfo_p = PNULL;
    /* + CQI_4.1 */
    UInt32 tempNewCqiIndexCW1 = newCqiIndexCW1;
    /* + CQI_4.1 */

    rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[rbgIndex];
    subSetIndex = rbgIndexInfo_p->subsetIndex;
    rat1CQIInfo_p =
        &cqiInfoNonSpatialMul_p->rat1NonSpatialMulCQIInfo[subSetIndex];
    subSetInfo_p = &rat1Info_p->subsetInfo[subSetIndex];
    subsetBitMap = rbgIndexInfo_p->rbgSizeBitMapForSubset;
    subsetBitMapZeroSpan = subSetInfo_p->bitMapWithSpanZero;
    subsetBitMapNonZeroSpan = subSetInfo_p->bitMapWithSpanNonZero;
    tempBitMap = (~subsetBitMap) & subsetBitMapZeroSpan;
    rat1CQIInfo_p->subsetCQIBitMapALLCQIIndex_2 &= subsetBitMap;

    /*Checking if the subBand falls in left Span or Right Span */
    /* + CQI_4.1 */

    if (tempBitMap)
    {
        /*Comes here when the subBand falls in left Span */

        /* It will reset CQI index bitmap's from newCqiIndexCW1 + 1 to 
         * oldCQIIndexCW1 so that we do not need to update the CQI index bitmaps
         * in updateRAT1CW1NonSpatialMulSubBandInfo function as lower CQI indexs 
         * are already set for the particlar RBG Index in previous report.
         */
        while ((tempNewCqiIndexCW1++ < oldCqiIndexCW1) && (tempNewCqiIndexCW1 < MAX_CQI_INDEX))/*GCC_4.9_FIX*/
        {
            /*Resetting the subBand information in subsetCQIBitMap bitmap */
            rat1CQIInfo_p->subsetCQIBitMapZeroSpan_2[tempNewCqiIndexCW1]
                &= (subsetBitMap & subsetBitMapZeroSpan);
        }
    }

    /*Checking if the subBand falls in left Span or Right Span */
    tempBitMap = (~subsetBitMap) & subsetBitMapNonZeroSpan;

    if (tempBitMap)
    {
        /*Comes here when the subBand falls in Right Span */

        /* It will reset CQI index bitmap's from newCqiIndexCW1 + 1 to 
         * oldCQIIndexCW1 so that we do not need to update the CQI index bitmaps
         * in updateRAT1CW1NonSpatialMulSubBandInfo function as lower CQI indexs 
         * are already set for the particlar RBG Index in previous report.
         */

        tempNewCqiIndexCW1 = newCqiIndexCW1;

        while ((tempNewCqiIndexCW1++ < oldCqiIndexCW1) && (tempNewCqiIndexCW1 < MAX_CQI_INDEX))/*GCC_4.9_FIX*/
        {
            /*Resetting the subBand information in subsetCQIBitMap bitmap */
            rat1CQIInfo_p->subsetCQIBitMapNonZeroSpan_2[tempNewCqiIndexCW1]
                &= (subsetBitMap & subsetBitMapNonZeroSpan);
        }
    }

    /* Resetting old Cqi Index from reportedCQIBitMap */
    rat1CQIInfo_p->reportedCQIBitMap_2 &=
        (~((UInt32) 1 << ((MAX_CQI_INDEX - oldCqiIndexCW1) - 1)));
    /* - CQI_4.1 */
    return;
}

/*-EICIC .*/
/***********************************************************************************
 * Function Name  : updateRAT1CW1NonSpatialMulSubBandInfo
 * Inputs         : rbgIndex - This holds subBand Number whose Information
 *                             need to be updated,
 *                  newCqiIndexCW1 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  oldCqiForRBG - This holds Old CQI for RBG
 *                  rat0Info_p - This stores the RAT0Info structure,
 *                  rat1Info_p - This stores the RAT1Info structure
 *                  tempReportedCQIBitmapPerSubset_p -
 * Outputs        : cqiInfoNonSpatialMul_p - This holds pointer to 
 *                                           CQIInfoNonSpatialMul structure
 * Returns        : None
 * Description    : This funtion will update  the RAT 1 Information stored in  
 *                  cqiInfoNonSpatialMul_p structure for  Non Spatial Multiplexing.
 ************************************************************************************/
 
STATIC  void updateRAT1CW1NonSpatialMulSubBandInfo(
        UInt32 rbgIndex,
        UInt32 newCqiIndexCW1,
        UInt32 oldCqiForRBG,
        CQIInfoNonSpatialMul *cqiInfoNonSpatialMul_p,
        RAT0Info *rat0Info_p,
        RAT1Info *rat1Info_p,
        UInt16 *tempReportedCQIBitmapPerSubset_p
        )
{
    RAT1NonSpatialMulCQIInfo *rat1CQIInfo_p = PNULL;
    SubsetInfo *subSetInfo_p = PNULL;
    UInt32 subsetBitMap = 0;
    UInt32 subSetIndex = 0;
    UInt32 subsetBitMapZeroSpan = 0;
    UInt32 subsetBitMapNonZeroSpan = 0;
    UInt32 tempBitMap = 0;
    RBGIndexInfo *rbgIndexInfo_p = PNULL;
    UInt32 tempNewCqiIndexCW1 = newCqiIndexCW1;

    rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[rbgIndex];
    subSetIndex = rbgIndexInfo_p->subsetIndex;
    rat1CQIInfo_p = &cqiInfoNonSpatialMul_p->
        rat1NonSpatialMulCQIInfo[subSetIndex];
    subSetInfo_p = &rat1Info_p->subsetInfo[subSetIndex];
    subsetBitMap = ~(rbgIndexInfo_p->rbgSizeBitMapForSubset);
    subsetBitMapZeroSpan = subSetInfo_p->bitMapWithSpanZero;
    subsetBitMapNonZeroSpan = subSetInfo_p->bitMapWithSpanNonZero;
    rat1CQIInfo_p->subsetCQIBitMapALLCQIIndex |=  subsetBitMap;
    
    /*Checking if the subband falls in left Span or Not*/
    tempBitMap = subsetBitMap & subsetBitMapZeroSpan;
    
    /* + CQI_4.1 */

    /* Updating the reported CQI Index */

    /* We are firstly updating  CQI in local array because it may happen that 
     * with in a report new CQI of one RBG is same as old CQI of other RBG which may
     * cause new CQI to be reset in reset functions.By taking it in local array
     * information will never reset for the particular report.
     */
    SET_CQI_REPORT_BIT(tempReportedCQIBitmapPerSubset_p[subSetIndex],newCqiIndexCW1);
    
    rat1CQIInfo_p->reportedCQIBitMap |= tempReportedCQIBitmapPerSubset_p[subSetIndex];

    if (tempBitMap)
    { 
        if ( oldCqiForRBG < tempNewCqiIndexCW1 )
        {   
            while (tempNewCqiIndexCW1 > oldCqiForRBG) 
            {
                rat1CQIInfo_p->subsetCQIBitMapZeroSpan[tempNewCqiIndexCW1] 
                                                        |= tempBitMap;
                tempNewCqiIndexCW1--;

            }
         }
    }    

    /*Checking if the subband falls in Right Span or Not*/
    tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

    if (tempBitMap)
    { 
        tempNewCqiIndexCW1 = newCqiIndexCW1;

       if ( oldCqiForRBG < tempNewCqiIndexCW1 )
       {    
          while (tempNewCqiIndexCW1 > oldCqiForRBG)
          {
             rat1CQIInfo_p->subsetCQIBitMapNonZeroSpan[tempNewCqiIndexCW1] 
                                                        |= tempBitMap;
             tempNewCqiIndexCW1--;
          }
       }
    }


    /* - CQI_4.1 */
    return;
}

/*+EICIC .*/
/***********************************************************************************
 * Function Name  : updateRAT1CW1NonSpatialMulSubBandInfo_2
 * Inputs         : rbgIndex - This holds subBand Number whose Information
 *                             need to be updated,
 *                  newCqiIndexCW1 - This holds new CQI index of the subBand
 *                                   whose Information need to be updated,
 *                  oldCqiForRBG - This holds Old CQI for RBG
 *                  rat0Info_p - This stores the RAT0Info structure,
 *                  rat1Info_p - This stores the RAT1Info structure
 *                  tempReportedCQIBitmapPerSubset_p -
 * Outputs        : cqiInfoNonSpatialMul_p - This holds pointer to 
 *                                           CQIInfoNonSpatialMul structure
 * Returns        : None
 * Description    : This funtion will update  the RAT 1 Information stored in  
 *                  cqiInfoNonSpatialMul_p structure for  Non Spatial Multiplexing.
 ************************************************************************************/

STATIC  void updateRAT1CW1NonSpatialMulSubBandInfo_2 (UInt32 rbgIndex,
                                         UInt32 newCqiIndexCW1,
                                         UInt32 oldCqiForRBG,
                                         CQIInfoNonSpatialMul *cqiInfoNonSpatialMul_p,
                                         RAT0Info * rat0Info_p,
                                         RAT1Info * rat1Info_p,
                                         UInt16 *tempReportedCQIBitmapPerSubset_p)
{
    RAT1NonSpatialMulCQIInfo *rat1CQIInfo_p = PNULL;
    SubsetInfo *subSetInfo_p = PNULL;
    UInt32 subsetBitMap = 0;
    UInt32 subSetIndex = 0;
    UInt32 subsetBitMapZeroSpan = 0;
    UInt32 subsetBitMapNonZeroSpan = 0;
    UInt32 tempBitMap = 0;
    RBGIndexInfo *rbgIndexInfo_p = PNULL;
    UInt32 tempNewCqiIndexCW1 = newCqiIndexCW1;

    rbgIndexInfo_p = &rat0Info_p->rbgIndexInfo[rbgIndex];
    subSetIndex = rbgIndexInfo_p->subsetIndex;
    rat1CQIInfo_p =
        &cqiInfoNonSpatialMul_p->rat1NonSpatialMulCQIInfo[subSetIndex];
    subSetInfo_p = &rat1Info_p->subsetInfo[subSetIndex];
    subsetBitMap = ~(rbgIndexInfo_p->rbgSizeBitMapForSubset);
    subsetBitMapZeroSpan = subSetInfo_p->bitMapWithSpanZero;
    subsetBitMapNonZeroSpan = subSetInfo_p->bitMapWithSpanNonZero;
    rat1CQIInfo_p->subsetCQIBitMapALLCQIIndex_2 |= subsetBitMap;

    /*Checking if the subband falls in left Span or Not */
    tempBitMap = subsetBitMap & subsetBitMapZeroSpan;

    /* + CQI_4.1 */

    /* Updating the reported CQI Index */

    /* We are firstly updating  CQI in local array because it may happen that 
     * with in a report new CQI of one RBG is same as old CQI of other RBG which may
     * cause new CQI to be reset in reset functions.By taking it in local array
     * information will never reset for the particular report.
     */
    SET_CQI_REPORT_BIT (tempReportedCQIBitmapPerSubset_p[subSetIndex],
            newCqiIndexCW1);

    rat1CQIInfo_p->reportedCQIBitMap_2 |=
        tempReportedCQIBitmapPerSubset_p[subSetIndex];

    if (tempBitMap)
    {
        if (oldCqiForRBG < tempNewCqiIndexCW1)
        {
            while (tempNewCqiIndexCW1 > oldCqiForRBG)
            {
                rat1CQIInfo_p->subsetCQIBitMapZeroSpan_2[tempNewCqiIndexCW1]
                    |= tempBitMap;
                tempNewCqiIndexCW1--;

            }
        }
    }

    /*Checking if the subband falls in Right Span or Not */
    tempBitMap = subsetBitMap & subsetBitMapNonZeroSpan;

    if (tempBitMap)
    {
        tempNewCqiIndexCW1 = newCqiIndexCW1;

        if (oldCqiForRBG < tempNewCqiIndexCW1)
        {
            while (tempNewCqiIndexCW1 > oldCqiForRBG)
            {
                rat1CQIInfo_p->subsetCQIBitMapNonZeroSpan_2[tempNewCqiIndexCW1]
                    |= tempBitMap;
                tempNewCqiIndexCW1--;
            }
        }
    }


    /* - CQI_4.1 */
    return;
}

/*-EICIC .*/
/* + CQI_4.1 */
/*****************************************************************************
 * Function Name  : getEffectiveDLCQI
 * Inputs         : timeAveragedCQI - CQI corrected after time averaging,
 *                  calculatedBlerValue - Calculated BLER,
 *                  dlUEContext_p - Pointer to DLUEContext
 * Outputs        : None
 * Returns        : Effective CQI
 * Description    : This funtion corrects the time averaged CQI from BLER value.
 *****************************************************************************/
				      /* SPR 13498 fix start */
STATIC  UInt8 getEffectiveDLCQI(UInt8 timeAveragedCQI,
				      /* SPR 13498 fix start */
                                      /* + CQI_5.0 */
                                      SInt16 cqiCorrectionFactor,
                                      DLUEContext *dlUEContext_p)
                                      /* - CQI_5.0 */

{
    SInt8 effectiveCQI = 0;
    /* Calculate and Correct CQI */
    /* Absolute value of (BLERtarget - BLER) * DL_RESOLUTION_FACTOR *X(BLER_SCALING_VALUE) should be
     * kept within certain threshold to avoid too much correction */
    /* Where X is scaling factor */

    /* + CQI_5.0 */ 
/* SPR 7981 changes start */ 
/* SPR 13498 fix start */
    effectiveCQI = timeAveragedCQI + (cqiCorrectionFactor/DL_RESOLUTION_FACTOR);
/* SPR 13498 fix end */
/* SPR 7981 changes end */ 
    /* - CQI_5.0 */

    if(effectiveCQI < MIN_CQI_INDEX)
    {
        effectiveCQI = MIN_CQI_INDEX;
    }

    else if(effectiveCQI >= MAX_CQI_INDEX)
    {
        effectiveCQI = MAX_CQI_INDEX - 1;
    }
    /*NEW_LOG_REQ*/
    /* SPR 13498 fix start */
    LOG_MAC_MSG(MAC_EFFECTIVE_CQI_CALC_LOG_ID,LOGINFO,MAC_CQI,\
            getCurrentTick(),timeAveragedCQI,cqiCorrectionFactor,\
            effectiveCQI,dlUEContext_p->ueIndex,DEFAULT_INT_VALUE,\
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
            __func__,"EFFECTIVE_CQI_CALC");
    /* SPR 13498 fix end */

    return ((UInt8)effectiveCQI);
}

/*****************************************************************************
 * Function Name  : calculateAverageDLCQI
 * Inputs         : currentCQI - Reported CQI,
 *                  previousCQI - Previous subband CQI
 * Outputs        : None
 * Returns        : Average DL CQI
 * Description    : This funtion calculate the average CQI from current  
 *                  and last stored CQI.
 *****************************************************************************/

					  /* SPR 13498 fix start */
STATIC  UInt8 calculateAverageDLCQI(UInt8 currentCQI, 
                                          /* SPR 13498 fix end */
                                          UInt8 previousCQI)
{
    UInt8 correctedCQI = 0;

    /* SPR 13498 fix start */
    UInt16 tempCQIValue = (MAC_DL_ALPHA_COEFFICIENT_IIR * previousCQI + \
                                  (1024 - MAC_DL_ALPHA_COEFFICIENT_IIR) * currentCQI);
    
    if ( previousCQI > currentCQI)
    /* SPR 13498 fix end */
    {
        correctedCQI = DIVIDE_BY_1024(tempCQIValue);
    }
    else
    {
        correctedCQI =  (DIVIDE_BY_1024(tempCQIValue - 1)) + 1; 
    }
    return correctedCQI;

}
/* - CQI_4.1 */

/* SPR 9427 changes start */
/*****************************************************************************
 * Function Name  : calculateAverageDLCQIforAP
 * Inputs         : 1)currentCQI - Reported CQI. 
 *                  2)lastSubbandCQI - Last subband CQI
 * Outputs        : 
 * Returns        : Average DL CQI
 * Description    : This funtion calculate the average CQI from current  
 *                  and last stored CQI.
 *****************************************************************************/
STATIC  UInt8 calculateAverageDLCQIforAP(UInt8 currentCQI, 
                                               UInt8 previousCQI)
{
    UInt8 correctedCQI = 0;

    /* SPR 13498 fix start */
    UInt16 tempCQIValue = (((1024 - MAC_DL_ALPHA_COEFFICIENT_IIR) * previousCQI) + \
                           (MAC_DL_ALPHA_COEFFICIENT_IIR * currentCQI));


    if ( previousCQI > currentCQI)
    /* SPR 13498 fix end */
    {
        correctedCQI = DIVIDE_BY_1024(tempCQIValue);
    }
    else
    {
        correctedCQI =  (DIVIDE_BY_1024(tempCQIValue - 1)) + 1; 
    }
    return correctedCQI;

}
/* SPR 9427 changes end */

/* + CQI_5.0 */
/*****************************************************************************
 * Function Name  : resetCW2Info
 * Inputs         : subBandReportInfo_p - pointer to SubBandReportInfo
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : It will reset the CW2 CQI info.
 * Returns        : None
 * Description    : This funtion will update the CW1 info to CW2.
 *****************************************************************************/
/* CA Changes start */ 
STATIC  void resetCW2Info(SubBandReportInfo *subBandReportInfo_p
                                  ,InternalCellIndex internalCellIndex)
/* CA Changes end */ 
{

    CQIInfo *cqiInfo_p = PNULL;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    RAT0SpatialMulCQIInfo *rat0SpatialMulCQIInfo_p  = PNULL;
    RAT1SpatialMulCQIInfo *rat1SpatialMulCQIInfo_p  = PNULL;
    /* CA Changes start */ 
    UInt8 subSetCount = (dlRAInfo_gp[internalCellIndex] +
           dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat1Info.subsetCount;
    UInt8 rbgCount = (dlRAInfo_gp[internalCellIndex] + 
                dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info.rbgCount; 
     /* CA Changes end */ 
    cqiInfo_p = &subBandReportInfo_p->cqiInfo;
 
    /* Updating CQI and MCS of CW1 to CW2 per RBG*/    
    while(rbgCount--)
    {
        subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[rbgCount];
        subBandToCQInfo_p->cqiIndexCW2 =  subBandToCQInfo_p->mcsIndexCW2 = 0;
    }
    
    rat0SpatialMulCQIInfo_p = &cqiInfo_p->cqiInfoSpatialMul.rat0SpatialMulCQIInfo;

    /* Updating RAT0 infomation of CW1 to CW2 */
    rat0SpatialMulCQIInfo_p->count64QAMSubBandCW2 = 
    rat0SpatialMulCQIInfo_p->count16QAMSubBandCW2 = 
    rat0SpatialMulCQIInfo_p->countQPSKSubBandCW2 = 
    rat0SpatialMulCQIInfo_p->rat064QAMCQIBitMapCW2 =  
    rat0SpatialMulCQIInfo_p->rat016QAMCQIBitMapCW2 =  
    rat0SpatialMulCQIInfo_p->rat0QPSKCQIBitMapCW2 = 0; 
    while(subSetCount--)
    {

        rat1SpatialMulCQIInfo_p =
            &cqiInfo_p->cqiInfoSpatialMul.rat1SpatialMulCQIInfo[subSetCount];

        /* Updating RAT1 Zero span information of CW1 to CW2*/
        rat1SpatialMulCQIInfo_p->subsetZeroSpanCQIInfo.count64QAMSubBandCW2 = 
        rat1SpatialMulCQIInfo_p->subsetZeroSpanCQIInfo.count16QAMSubBandCW2 =
        rat1SpatialMulCQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2 =
        rat1SpatialMulCQIInfo_p->subsetZeroSpanCQIInfo.rat164QAMCQIBitMapCW2 =
        rat1SpatialMulCQIInfo_p->subsetZeroSpanCQIInfo.rat116QAMCQIBitMapCW2 =
        rat1SpatialMulCQIInfo_p->subsetZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2 = 0;


        /* Updating RAT1 Non-Zero span information of CW1 to CW2*/
        rat1SpatialMulCQIInfo_p->subsetNonZeroSpanCQIInfo.count64QAMSubBandCW2 =
        rat1SpatialMulCQIInfo_p->subsetNonZeroSpanCQIInfo.count16QAMSubBandCW2 =
        rat1SpatialMulCQIInfo_p->subsetNonZeroSpanCQIInfo.countQPSKSubBandCW2 =
        rat1SpatialMulCQIInfo_p->subsetNonZeroSpanCQIInfo.rat164QAMCQIBitMapCW2 =
        rat1SpatialMulCQIInfo_p->subsetNonZeroSpanCQIInfo.rat116QAMCQIBitMapCW2 =
        rat1SpatialMulCQIInfo_p->subsetNonZeroSpanCQIInfo.rat1QPSKCQIBitMapCW2 = 0;

    }

   return;

}
/* - CQI_5.0 */
/* CA_phase2_csi_code Start */
/*+EICIC .*/
/*****************************************************************************
 * Function Name  : resetCW2Info_2
 * Inputs         : subBandReportInfo_p - pointer to SubBandReportInfo
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : It will reset the CW2 CQI info.
 * Returns        : None
 * Description    : This funtion will update the CW1 info to CW2.
 *****************************************************************************/
/* CA Changes start */
STATIC  void resetCW2Info_2 (SubBandReportInfo * subBandReportInfo_p,
                                    InternalCellIndex internalCellIndex)
/* CA Changes end */
{

    CQIInfo *cqiInfo_p = PNULL;
    SubBandToCQInfo *subBandToCQInfo_p = PNULL;
    RAT0SpatialMulCQIInfo *rat0SpatialMulCQIInfo_p = PNULL;
    RAT1SpatialMulCQIInfo *rat1SpatialMulCQIInfo_p = PNULL;
    /* CA Changes start */
    UInt8 subSetCount = (dlRAInfo_gp[internalCellIndex] +
            dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.
        rat1Info.subsetCount;
    UInt8 rbgCount =
        (dlRAInfo_gp[internalCellIndex] +
         dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info.rbgCount;
    /* CA Changes end */
    cqiInfo_p = &subBandReportInfo_p->cqiInfo;

    /* Updating CQI and MCS of CW1 to CW2 per RBG */
    while (rbgCount--)
    {
        subBandToCQInfo_p = &subBandReportInfo_p->subBandToCQInfo[rbgCount];
        subBandToCQInfo_p->cqiIndexCW2 = subBandToCQInfo_p->mcsIndexCW2 = 0;
    }

    rat0SpatialMulCQIInfo_p =
        &cqiInfo_p->cqiInfoSpatialMul.rat0SpatialMulCQIInfo;

    /* Updating RAT0 infomation of CW1 to CW2 */
    rat0SpatialMulCQIInfo_p->count64QAMSubBandCW2_2 =
        rat0SpatialMulCQIInfo_p->count16QAMSubBandCW2_2 =
        rat0SpatialMulCQIInfo_p->countQPSKSubBandCW2_2 =
        rat0SpatialMulCQIInfo_p->rat064QAMCQIBitMapCW2_2 =
        rat0SpatialMulCQIInfo_p->rat016QAMCQIBitMapCW2_2 =
        rat0SpatialMulCQIInfo_p->rat0QPSKCQIBitMapCW2_2 = 0;
    while (subSetCount--)
    {

        rat1SpatialMulCQIInfo_p =
            &cqiInfo_p->cqiInfoSpatialMul.rat1SpatialMulCQIInfo[subSetCount];

        /* Updating RAT1 Zero span information of CW1 to CW2 */
        rat1SpatialMulCQIInfo_p->subsetZeroSpanCQIInfo.count64QAMSubBandCW2_2 =
            rat1SpatialMulCQIInfo_p->subsetZeroSpanCQIInfo.
            count16QAMSubBandCW2_2 =
            rat1SpatialMulCQIInfo_p->subsetZeroSpanCQIInfo.countQPSKSubBandCW2_2 =
            rat1SpatialMulCQIInfo_p->subsetZeroSpanCQIInfo.
            rat164QAMCQIBitMapCW2_2 =
            rat1SpatialMulCQIInfo_p->subsetZeroSpanCQIInfo.
            rat116QAMCQIBitMapCW2_2 =
            rat1SpatialMulCQIInfo_p->subsetZeroSpanCQIInfo.
            rat1QPSKCQIBitMapCW2_2 = 0;


        /* Updating RAT1 Non-Zero span information of CW1 to CW2 */
        rat1SpatialMulCQIInfo_p->subsetNonZeroSpanCQIInfo.
            count64QAMSubBandCW2_2 =
            rat1SpatialMulCQIInfo_p->subsetNonZeroSpanCQIInfo.
            count16QAMSubBandCW2_2 =
            rat1SpatialMulCQIInfo_p->subsetNonZeroSpanCQIInfo.
            countQPSKSubBandCW2_2 =
            rat1SpatialMulCQIInfo_p->subsetNonZeroSpanCQIInfo.
            rat164QAMCQIBitMapCW2_2 =
            rat1SpatialMulCQIInfo_p->subsetNonZeroSpanCQIInfo.
            rat116QAMCQIBitMapCW2_2 =
            rat1SpatialMulCQIInfo_p->subsetNonZeroSpanCQIInfo.
            rat1QPSKCQIBitMapCW2_2 = 0;

    }

    return;

}
/*-EICIC .*/
/*****************************************************************************
 * Function Name  : resetScellCSIValuesOnCqiReportValidTimerExpiry
 * Inputs         : dlUEContext_p - pointer to Dl UE Context
 *                  scellServCellIndex -  Index of serving cell
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion reset any reported CSI values of the SCell with
 *                  the initial configured MCS of the Scell except initial mcs 
 *                  and last reported periodic RI
 *****************************************************************************/
void resetScellCSIValuesOnCqiReportValidTimerExpiry( DLUEContext *dlUEContext_p,
                                                     ServingCellIndex scellServCellIndex
                                                   )
{
    DLCQIInfo *dlCQIInfo_p = PNULL;
    DLUESCellContext *dlUEScellContext_p = PNULL;
    UInt8 initialMcs;
    UInt8 counter =  MAX_SUBBANDS;
    ULUESCellContext *ulUeScellContext_p  = PNULL;
    ulUeScellContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p->
        ulUeScellContext_p[scellServCellIndex];
    if(dlUEContext_p->dlUeScellContext_p[scellServCellIndex]!= PNULL)
    {
        dlUEScellContext_p = dlUEContext_p->dlUeScellContext_p[scellServCellIndex];
    }
    else
    {
        return;
    }
    dlCQIInfo_p = &dlUEScellContext_p->ScelldlCQIInfo;

    macResetScellCQIInfo(dlUEScellContext_p,dlCQIInfo_p
                         );
    dlCQIInfo_p->riUpdationFlag = IMMEDIATE_DL_RI_UPDATE;
    /* initializing values ueReportedRI and ueReportedPeriodic RI in Scell
     * context with last saved RI value in dlContext */
    ulUeScellContext_p->scellpucchConfigInfo.ueReportedRI = 
        ulUeScellContext_p->scellpucchConfigInfo.ueReportedPeriodicRI =
        dlUEScellContext_p->ScelldlMIMOInfo.dlRi;

    initialMcs = getMCSFromCQI(dlCQIInfo_p->initialCqiValue
                               );
    dlCQIInfo_p->widebandCQICodeWordOne = dlCQIInfo_p->initialCqiValue;
    dlCQIInfo_p->mcsIndexCodeWordOne = initialMcs;
    dlCQIInfo_p->widebandCQICodeWordTwo = dlCQIInfo_p->initialCqiValue;
    dlCQIInfo_p->mcsIndexCodeWordTwo = initialMcs;

    /* Cyclomatic_complexity_changes_start */
    getModSchemeFromMcsIndex(initialMcs,
            &(dlCQIInfo_p->modulationSchemeCodeWordOne));
    /* Cyclomatic_complexity_changes_end */

    dlCQIInfo_p->modulationSchemeCodeWordTwo = dlCQIInfo_p->modulationSchemeCodeWordOne;
    /* wideband PMI index set to initial configured value */ 
    dlCQIInfo_p->wideBandPMIIndex = dlUEScellContext_p->ScelldlMIMOInfo.precodingIndex;

    dlCQIInfo_p->reportReceivedTTI = 0;
    /* +- SPR 17777 */
    macUupdateSCellDLSINRCW0(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordOne,
            scellServCellIndex);
    macUpdateSCellDLSINRCW1(dlUEContext_p->ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo, 
            scellServCellIndex);
    /* +- SPR 17777 */

    /* Initialize subBandReportInfo with initial mcs value */
    if ((dlUEScellContext_p->ScelldlMIMOInfo.transmissionMode == TX_MODE_3 ||
            dlUEScellContext_p->ScelldlMIMOInfo.transmissionMode == TX_MODE_4 ||
            dlUEScellContext_p->ScelldlMIMOInfo.transmissionMode == TX_MODE_6 ||
            dlUEScellContext_p->ScelldlMIMOInfo.transmissionMode == TX_MODE_7 ||
            dlUEScellContext_p->ScelldlMIMOInfo.transmissionMode == TX_MODE_8)  &&
            /*SPR 20864 Fix start*/
            (PNULL != dlCQIInfo_p->subBandReportInfo_p))
            /*SPR 20864 Fix End*/
    {
        while(counter--)
        {    
            dlCQIInfo_p->subBandReportInfo_p->
                subBandToCQInfo[counter].cqiIndexCW1 =  
                dlCQIInfo_p->subBandReportInfo_p->
                subBandToCQInfo[counter].cqiIndexCW2 = dlCQIInfo_p->initialCqiValue;

            dlCQIInfo_p->subBandReportInfo_p->
                subBandToCQInfo[counter].mcsIndexCW1 =
                dlCQIInfo_p->subBandReportInfo_p->
                subBandToCQInfo[counter].mcsIndexCW2 = 
                dlCQIInfo_p->mcsIndexCodeWordOne;

            dlCQIInfo_p->subBandReportInfo_p->
                subBandToCQInfo[counter].previousTimeAveragedSBCQICW0 =  
                dlCQIInfo_p->subBandReportInfo_p->
                subBandToCQInfo[counter].previousTimeAveragedSBCQICW1 = 
                dlCQIInfo_p->initialCqiValue;
        }

        initSpatialRAT0RAT1Bitmaps(dlCQIInfo_p->initialCqiValue,
                &dlCQIInfo_p->subBandReportInfo_p->
                cqiInfo.cqiInfoSpatialMul,
                dlUEScellContext_p->internalCellIndex);

    }
    else if ((dlUEScellContext_p->ScelldlMIMOInfo.transmissionMode == TX_MODE_1 ||
            dlUEScellContext_p->ScelldlMIMOInfo.transmissionMode == TX_MODE_2) &&
            /*SPR 20864 Fix start*/
            (PNULL != dlCQIInfo_p->subBandReportInfo_p ))
            /*SPR 20864 Fix End*/
    {   

        while(counter--)
        {    
            dlCQIInfo_p->subBandReportInfo_p->
                subBandToCQInfo[counter].cqiIndexCW1 = dlCQIInfo_p->initialCqiValue; 

            dlCQIInfo_p->subBandReportInfo_p->
                subBandToCQInfo[counter].mcsIndexCW1 =
                dlCQIInfo_p->mcsIndexCodeWordOne;

            dlCQIInfo_p->subBandReportInfo_p->
                subBandToCQInfo[counter].previousTimeAveragedSBCQICW0 = 
                dlCQIInfo_p->initialCqiValue;  
        }

        initNonSpatialRAT0RAT1Bitmaps(dlCQIInfo_p->initialCqiValue,
                &dlCQIInfo_p->subBandReportInfo_p->
                cqiInfo.cqiInfoNonSpatialMul,
                dlUEScellContext_p->internalCellIndex);

    }
}
/* CA_phase2_csi_code End */
/* Cyclomatic_complexity_changes_start */
/*****************************************************************************
 * Function Name  : checkForAperiodicCQITrigger
 * Inputs         : currentGlobalTick,
                    dlCQIInfo_p,
                    dlUEContext_p,
                    periodicModeCqiValue,
                    initialAperiodicReportedCQI,
                    subBandValueDiff,
                    internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    :          
 ******************************************************************************/

#ifdef FDD_CONFIG
 void checkForAperiodicCQITrigger(/* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        DLCQIInfo *dlCQIInfo_p, 
		DLUEContext *dlUEContext_p,
        UInt8 periodicModeCqiValue, 
		UInt8 initialAperiodicReportedCQI, 
		UInt8 subBandValueDiff, 
		InternalCellIndex internalCellIndex) 
{
    /*  CA_phase2_csi_code Start */ 
    UInt8 bitMaskForAperiodicCSITrigger; 
    if ( (APERIODIC_CQI_REPORTING_MODE == dlCQIInfo_p->cqiTypeIndicator) || 
            (PERIODIC_APERIODIC_MODE == dlCQIInfo_p->cqiTypeIndicator) )  
    {
        if( ((CQI_MODE_3_0 == dlCQIInfo_p->cqiMode) || 
             (CQI_MODE_3_1 == dlCQIInfo_p->cqiMode)) && 
             (0 != initialAperiodicReportedCQI)) 
        {
            if( periodicModeCqiValue > initialAperiodicReportedCQI ) 
            {
                subBandValueDiff = 
                    (periodicModeCqiValue - initialAperiodicReportedCQI); 
            }
            else
            {
                subBandValueDiff = 
                    (initialAperiodicReportedCQI - periodicModeCqiValue); 
            }
            if( (subBandValueDiff > APERIODIC_TRIGGER_CQI_OFFSET) && 
                    (currentGlobalTick > (dlCQIInfo_p->aperiodicCqiRequestTTI + 
                                          APERIODIC_CQI_REQ_OFFSET)) )
            {
                /* CA_phase2_csi_code Start */
                if(dlUEContext_p->internalCellIndex == internalCellIndex)
                {
                    /* Pcell */
                    bitMaskForAperiodicCSITrigger = servingCellBitMask_g[0];
                }
                else
                {
                    /* Scell */
                    bitMaskForAperiodicCSITrigger = servingCellBitMask_g[1];
                }
                /* CA_phase2_csi_code End */
                /*CA Changes start  */ 
                putEntryInULAperiodicCQIQueue(dlUEContext_p->ueIndex, internalCellIndex,
                        bitMaskForAperiodicCSITrigger); 
                /*CA Changes end  */ 
                dlCQIInfo_p->aperiodicCqiRequestTTI = currentGlobalTick;      
            }
        }
    }

    return;
}
#elif TDD_CONFIG
/*****************************************************************************
 * Function Name  : checkForAperiodicCQITrigger
 * Inputs         : currentGlobalTick,
 dlCQIInfo_p,
                    dlUEContext_p,
                    periodicModeCqiValue,
                    initialAperiodicReportedCQI,
                    subBandValueDiff,
                    internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    :          
 ******************************************************************************/
 void checkForAperiodicCQITrigger(/* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        DLCQIInfo *dlCQIInfo_p,
        DLUEContext *dlUEContext_p,
        UInt8 periodicModeCqiValue,
        UInt8 initialAperiodicReportedCQI,
        UInt8 subBandValueDiff,
        InternalCellIndex internalCellIndex)
	
{        /*  CA_phase2_csi_code Start */
    UInt8 bitMaskForAperiodicCSITrigger;
    /* dummy parameter for making compilation through */
    if ( (APERIODIC_CQI_REPORTING_MODE == dlCQIInfo_p->cqiTypeIndicator) || 
            (PERIODIC_APERIODIC_MODE == dlCQIInfo_p->cqiTypeIndicator) )  
    {
        if( ((CQI_MODE_3_0 == dlCQIInfo_p->cqiMode) || 
             (CQI_MODE_3_1 == dlCQIInfo_p->cqiMode)) && 
             (0 != initialAperiodicReportedCQI)) 
        {
            if( periodicModeCqiValue > initialAperiodicReportedCQI ) 
			{
				subBandValueDiff = 
				(periodicModeCqiValue - initialAperiodicReportedCQI); 
			}
			else
			{
				subBandValueDiff = 
				(initialAperiodicReportedCQI - periodicModeCqiValue); 
			}
			if( (subBandValueDiff > APERIODIC_TRIGGER_CQI_OFFSET) && 
					(currentGlobalTick > (dlCQIInfo_p->aperiodicCqiRequestTTI + 
						       APERIODIC_CQI_REQ_OFFSET)) )
			{
				/*CA Changes start  */
                             /*ca-tdd pucch code changes*/  
                             if(dlUEContext_p->internalCellIndex == internalCellIndex)
                             {
                                 /* Pcell */
                                 bitMaskForAperiodicCSITrigger = servingCellBitMask_g[0];
                             }
                             else
                             {
                                 /* Scell */
                                 bitMaskForAperiodicCSITrigger = servingCellBitMask_g[1];
                             }
				putEntryInULAperiodicCQIQueue(dlUEContext_p->ueIndex, internalCellIndex,bitMaskForAperiodicCSITrigger);
				/*CA Changes end  */ 
				dlCQIInfo_p->aperiodicCqiRequestTTI = currentGlobalTick;      
			}
		}
	}

    return;
}
#endif
/* Cyclomatic_complexity_changes_end */
