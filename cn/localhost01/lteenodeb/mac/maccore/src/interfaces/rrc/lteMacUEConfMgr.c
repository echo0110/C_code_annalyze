/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacUEConfMgr.c,v 1.1.1.1.6.2.6.1.10.1 2010/11/08 06:06:27 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains different functions that perform :
 *                     - Creating Downlink & Uplink UE Context, reconfiguring and 
 *                        deleting MAC UE entities and their logical channels.
 *                     - It also update drx Flag in the DL UE Context. 
 *                       
 *
 *  
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacUEConfMgr.c,v $
 * Revision 1.1.1.1.6.2.6.1.11.1  2012/07/20 06:06:27  gur31292
 * Changes related to DCI Format3A
 *
 * Revision 1.1.1.1.6.2.6.1.10.1  2010/11/08 06:06:27  gur10121
 * Semi static changes merging
 *
 * Revision 1.1.1.1.6.2.6.1.4.1  2010/10/25 08:27:38  gur10121
 * Merging the Semi static changes on the latest
 *
 * Revision 1.1.1.1.6.2.6.1.2.1  2010/10/15 13:31:32  gur20439
 * changes to support PUSCH config
 *
 * Revision 1.1.1.1.6.2.6.1  2010/10/11 12:57:04  gur10121
 * SPR #706 fix
 *
 * Revision 1.1.1.1.6.2.10.1  2010/10/08 07:42:48  gur10121
 *  SPR 706 Fix - KlockWork Warnings removed
 *
 * Revision 1.1.1.1.6.2  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.51.28.1  2010/07/26 09:30:35  gur23951
 * Initialized variable newDataIndicator to NEW_TX
 *
 * Revision 1.51  2009/10/21 07:18:13  gur11912
 * optimization changes
 *
 * Revision 1.50  2009/09/16 14:52:05  gur18550
 * removed   ulUEContext_p->isStopAdvancedAllocation = FALSE
 *
 * Revision 1.49  2009/09/01 15:13:31  gur12140
 * Solved Bug Related toUL and DL
 *
 * Revision 1.48  2009/08/31 09:16:05  gur12140
 * Solved Bug
 *
 * Revision 1.47  2009/08/30 14:08:20  gur19413
 * added validation for cqi and sr request type
 *
 * Revision 1.46  2009/08/20 07:13:05  gur19413
 * added cqi related Changes.
 *
 * Revision 1.45  2009/08/08 10:49:10  gur12140
 * Code was removed earlier.Now Again checked in
 *
 * Revision 1.44  2009/07/30 11:25:21  gur15697
* cleanup Bugfix
*
* Revision 1.43  2009/07/28 00:05:54  gur19413
* added validation for PUCCH Resource e ndex related.
*
* Revision 1.42  2009/07/27 13:03:53  gur12140
* Solved Bug
*
* Revision 1.41  2009/07/23 09:51:20  gur12140
* MemSet dlCQIInfo_p->subBandReportInfo_p
*
* Revision 1.40  2009/07/22 14:49:30  gur18569
* changes in logic of deleteing DL harq entity
*
* Revision 1.39  2009/07/21 13:02:22  gur19413
* set default maxLcId to INVALID_LCID
*
* Revision 1.38  2009/07/20 14:58:54  gur18569
* initializing dlCQIInfo_p->subBandReportInfo_p
*
* Revision 1.37  2009/07/17 14:33:49  gur19413
* removed riPUCCHRBIndex field from uplink context
*
* Revision 1.36  2009/07/16 07:54:33  gur19413
* added reconfigure Flag & Macro
*
* Revision 1.35  2009/07/13 04:46:29  gur19413
* removed warnings
*
* Revision 1.34  2009/07/13 04:06:07  gur19413
* log type changed
*
* Revision 1.33  2009/07/09 10:04:12  gur19413
* fixed ut bug
*
* Revision 1.32  2009/07/07 08:09:18  gur20052
* changes related to the HARQ implementation
*
* Revision 1.30  2009/07/03 09:09:28  gur19413
* fixed bugs
*
* Revision 1.29  2009/07/02 06:49:57  gur15697
* CRNTI != rntiInfo_p->rntiFlag changed to CRNTI == rntiInfo_p->rntiFlag
*
* Revision 1.28  2009/07/01 04:20:58  gur15697
* review comments incorporated
*
* Revision 1.27  2009/06/29 08:21:54  gur19413
* initialize the modulation scheme
*
* Revision 1.26  2009/06/24 07:50:26  gur19413
* move initiliaze LC's before call add Lc func.
*
* Revision 1.25  2009/06/23 23:21:27  gur19413
* function name changed initUEConf to initUEConfMgr
*
* Revision 1.24  2009/06/23 22:58:17  gur19413
* added LC related functionality.
*
* Revision 1.23  2009/06/23 12:48:34  gur18550
* Removed numLCsInRLCDataArr
*
* Revision 1.22  2009/06/23 12:26:53  gur19413
* remove harqList
*
* Revision 1.21  2009/06/22 06:07:32  gur19413
* added pucch related function
*
* Revision 1.20  2009/06/18 06:24:35  gur15697
* lteMacParseOAMMsg.h added to remove compilation errors
*
* Revision 1.19  2009/06/17 09:04:05  gur18569
* initializing tx indicator with INVALID_TX
*
* Revision 1.18  2009/06/17 05:28:50  gur18569
* initializing tx indicator and taDrxFlag with invalid enum types
*
* Revision 1.17  2009/06/12 07:13:28  gur11083
* add new parameter dlCodingRate & ulCodingRate
*
* Revision 1.16  2009/06/11 13:39:47  gur19413
* fixed UT bugs
*
* Revision 1.15  2009/06/11 11:30:31  gur19413
* added new function clean;deleteUe
*
* Revision 1.14  2009/06/09 07:13:58  gur18569
* fixed a bug , commented marks free rnti to busy crnti
*
* Revision 1.13  2009/06/08 12:51:44  gur19413
* rename the parameter
*
* Revision 1.12  2009/06/05 13:58:57  gur18569
* fixed bugs
*
* Revision 1.11  2009/06/05 07:07:22  gur19413
* Removed Compilation Erros
*
* Revision 1.10  2009/06/04 11:08:46  gur15697
* freeRNTI(ueId) added
*
* Revision 1.9  2009/06/03 14:39:01  gur19413
* remove compilation error
*
* Revision 1.8  2009/06/02 08:13:37  gur18569
* added definitins of pending delete dl and ul queues
*
* Revision 1.7  2009/06/02 07:44:45  gur18569
* name change of ueNonZeroQueueLoadAndHarqList_p
*
* Revision 1.6  2009/06/01 07:50:19  gur18569
* removed compilation errors
*
* Revision 1.5  2009/05/28 07:35:20  gur20470
* remove compilation error
*
* Revision 1.4  2009/05/28 07:24:02  gur20470
* remove compliation error -M.k
*
* Revision 1.3  2009/05/28 06:16:40  gur19413
* remove compilation error
*
* Revision 1.2  2009/05/13 16:15:52  gur19413
* remove compilation error & added new function
*
* Revision 1.1  2009/04/30 12:10:30  gur19413
* Initial Version
*
*
*****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacTypes.h" /* SS_S2 */
#include "lteMacUEConfMgr.h"
/*This header file is used to get the current Tick */
#include "lteMacSFNMgr.h"
/* This header file used to get the getUeIdxFromRNTIMap() */ 
#include "lteMacRandomAccess.h"
/* This header file used to update the Cell config Structure*/
#include "lteMacCellConfMgr.h"
#include "lteMacParseUtil.h"
#include "lteMacParseOAMMsg.h"
/*This header file used to get Node type of DRX*/
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacDLSpsScheduler.h"
#include "lteMacULSpsScheduler.h"
#include "lteMacSpsCommonTypes.h"
#include "lteMacDLUEContext.h"
#include "lteMacULUEContext.h"
#include "lteMacPUCCH.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
/* + SPS_TDD_Changes*/
#include "lteMacTddMgr.h"
/* - SPS_TDD_Changes*/
#endif
#include "lteMacULQosScheduler.h"
#include "lteMacDLQosScheduler.h"
#include "lteMacDLQosTokenAlgo.h"
#include "lteMacStatsManager.h"
#include "lteMacULPowerControlData.h"
#include "lteMacULCSIManager.h"
#include "lteMacSpsPDSCHmgr.h"
#ifdef OVERSHOOT_CORRECTION
#include "lteCommonStatsManager.h"
#endif
/* ICIC changes start */
#include "lteMacDLResourceInfoMgr.h"
#include "lteMacRRMPeriodicMgr.h"
/* ICIC changes end */
/* SPR 11215 Changes Start */
#if defined(FAPI_1_1_COMPLIANCE) || defined(FAPI_4_0_COMPLIANCE)
/* SPR 11215 Changes End */
#include "lteMacPUSCH.h"
#endif
/* +DYNAMIC_ICIC */
#include "lteMacULResourceInfoMgr.h"
#include "lteMacPuschPowerControl.h"
/* -DYNAMIC_ICIC */
/* + E_CID_5.3 */
#include "lteMacReceiver.h"
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
#include "lteCommonStatsManager.h"
#include "lteMacOAMInterface.h"
/* MOCN Changes End */
/* SPR 23402 Changes End */
/* - E_CID_5.3 */
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/


/*For Downlink UE Context related API */

/* Creating Downlink UE Context */
 STATIC MacRetType createMACDLUEEntity( UeContextForDownlink *ueDlConfigInfo_p,InternalCellIndex internalCellIndex);
/* SPR 20671 fix start */
STATIC  void deletePartialCreatedUplinkUEContext(ULUEContext* ueULContext_p);
/* SPR 20671 fix end */
/* ADD Downlink Logical Channel*/
extern MacRetType createPUCCHMapForReportOrSR(
        /*SPR 16855 +-*/
		UInt16  ueIndex,
		PucchConfig *pucchConfigInfo_p,
        /*SPR 16855 +-*/
		ULUEContext  *currentUEContext_p,
		UInt32 subFrameOffset,
		UInt32 periodicity,
		ReportAndOpportunity reportType,
		UInt8 * maxWrapAround_p
#ifdef FDD_CONFIG
		,UInt32 pucchResourceIndex
		,UInt32 pucchFormat
		,UeCellInfo * ueCellInfo_p
#endif
		,InternalCellIndex internalCellIndex
		);
/* SPR23194 Start */
extern UInt8 kpiLcToQciMapping_g[MAX_LCID_THP];
/* SPR23194 End */
/* Creating Downlink Harq Entity */
/* + SPR 17733 */
 MacRetType createMACDLHarqEntity( DLHARQEntity **harqDLContext_pp,
        /* - SPR 17733 */
        UInt32 numHarqProcess );

/* ReConfigure the Downlink UE Context */
/*SPR 16855 +-*/
STATIC MacRetType reconfigureMACDLUEEntity ( UeContextForDownlink *dwlkUEInfo_p, 
		DLReconfigUeReq *dlReconfigUeReq_p, InternalCellIndex internalCellIndex);
/*SPR 16855 +-*/

/* Delete the Downlink Harq Entity */
/* SPR 5288 Start */
/*CA Changes start  */
 STATIC MacRetType deleteMACDLHarqEntity
(DLUEContext*  ueContext_p, InternalCellIndex internalCellIndex);
/* SPR 5288 End */

/* Resetting Downlink UE Context */
 MacRetType resetMACDLUEEntity(DLUEContext* ueDLContext_p, 
		InternalCellIndex internalCellIndex);
/* Resetting UpLink UE Context */
 void resetMACULUEEntity(ULUEContext* ueULContext_p, 
		InternalCellIndex internalCellIndex);
/*CA Changes end  */


/*revert back if Downlink UE context is added and 
  Uplink UE context does added then downlink UE
  context also removed*/

/* Delete Downlink LC */

/* create Uplink UE Context */
 STATIC MacRetType createMACULUEEntity( UInt32 ueIndex, 
		UInt32 lchCount, 
		UeContextForUplink *ueContextInfo_p,
		/* SPR 6690 fix start */
		UeContextForDownlink *ueDlConfigInfo_p,
		/* SPR 6690 fix end */
		InternalCellIndex internalCellIndex
		);
/* add uplink Logical Channel */


/***********************************************************/
/* For Uplink UE Context related API */

/*SPR 16855 +-*/
 MacRetType configurePucchSRSAndReports( UInt16 ueIndex,
		PucchConfig *tempPucchConfigInfo_p,
		SRSConfigInfo *tempsrsConfigInfo_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink*  uplkUEInfo_p,
		UInt32 *createULUEBitMap_p,
		UInt32 *createDLUEBitMap_p,
		ULUEContext *newULUEContext_p,
		UInt32 currSFN, UInt32 currSF,
		InternalCellIndex internalCellIndex
		);

/*Reconfig Changes Start */
MacRetType reconfigurePUCCHSRSAndReports( 
        /* +- SPR 17777 */
		UInt16 ueIndex,
		PucchConfig *tempPucchConfigInfo_p,
		SRSConfigInfo *tempsrsConfigInfo_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink  *uplkUEInfo_p,
		ULReconfigUeReq *ulReconfigUeReq_p,
		UInt32 currSFN, UInt32 currSF,
		UeContextForReconfig *reconfigInfo_p,
		InternalCellIndex internalCellIndex);
 MacRetType reconfigurePUCCHSRSAndReportsPassiveContext( 
		ULUEContext *ulUeCtx_p,
		UInt16                ueIndex,
		PucchConfig          *tempPucchConfigInfo_p,
		SRSConfigInfo        *tempsrsConfigInfo_p,
        /* +- SPR 17777 */
		UeContextForUplink   *uplkUEInfo_p,
		UInt32 currSFN, UInt32 currSF, 
		UeContextForReconfig *reconfigInfo_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex);
/*CA Changes end  */

extern  MacRetType getCqiRiPeriodicityAndOffset(
		UInt32 cqiPmiConfigIndex,
		UInt32 riConfigIndex,
		UInt32 *cqiPeriodicity,
		UInt32 *nOffsetCQI,
		UInt32 *riPeriodicity,
		UInt32 *nOffsetRI,
		UInt8 H
		);
/*Reconfig Changes End */



 MacRetType configureMeasGap(UeContextForDownlink *dwlkUEInfo_p,
		MeasGapContextDL *dlMeasGapContext_p,
		MeasGapContextUL *ulMeasGapContext_p,
		UInt8 *dlMeasGapConfigType_p,
		UInt8 *ulMeasGapConfigType_p,
		UInt32 *createULUEBitMap_p,
		UInt32 *createDLUEBitMap_p,
		InternalCellIndex internalCellIndex
		);
/*SPR 16855 +-*/

/***********************************************************/
/* For Uplink UE Context related API */

/* ReConfigure Uplink UE Context */
/*SPR 16855 +-*/
/* +- SPR 17777 */
 STATIC MacRetType reconfigureMACULUEEntity(UeContextForUplink *uplkUEInfo_p,
		UInt32 uplkLCCount,ULReconfigUeReq *ulReconfigUeReq_p
		/* CA Changes Start */
		,InternalCellIndex internalCellIndex
		/* CA changes End */
		);
/* +- SPR 17777 */
/*SPR 16855 +-*/
/* ReConfigure the Uplink Logical Channel*/

/* Delete uplink Logical channel */

#ifndef TDD_CONFIG
/*SPR 16855 +-*/
/*CA Changes start  */
 STATIC void deletePeriodicReportsNode( PucchConfig *pucchConfig_p,
		InternalCellIndex internalCellIndex );
/*SPR 16855 +-*/
/*CA Changes end  */
#endif
UInt16 allocateSpsCrnti(InternalCellIndex internalCellIndex);

/* SS_S2 */
/* 4x2 MIMO S */
 UInt8 calculateReportFormatBitWidthPeriodic(ueExpectedReportType feedbackType, 
		/* TM8_4*2_Mimo */
		UInt8 ri,UInt8 ueCategory, UInt8 transMode,
		InternalCellIndex internalCellIndex);
/* TM8_4*2_Mimo */
/* 4x2 MIMO E */
/* + CQI_5.0 */
/* 4x2 MIMO S */
/* SPR 5599 changes start (ZIP ID 129191) */            
UInt8 calculateReportFormatBitWidthAperiodic
/* SPR 5599 changes end (ZIP ID 129191) */            
/* +- SPR 17777 */
(CQIMode cqiMode,ueExpectedReportType *feedBack_p, UInt8 transMode,
 UInt8 *riBits_p,UInt8 *bitWidthiForRIGT_1,UInt8 ueCategory,
 InternalCellIndex internalCellIndex);
/* 4x2 MIMO E */

MacRetType  validateCQIModeForNewTxMode(UInt8 newTransmissionMode,
		UInt8 cqiTypeIndicator,
		PeriodicReportingModes periodicRepMode,
		UInt8 aperiodicRepMode,
		/* + TM7_8 Changes Start */
		UInt8 pmiRiReportV920);
/* - TM7_8 Changes End */

extern MacRetType validateAperiodicModeForTXMode(UInt8 aperiodicCQIMode,
		UInt8 transmissionMode);

/* + TM7_8 Changes Start */
MacRetType validateAperiodicModeForTXMode8(UInt8 aperiodicCQIMode,
		UInt8 pmiRiReportV920);
/* - TM7_8 Changes End */

/* - CQI_5.0 */

/* SS_S2 */
 STATIC void resetUECnf( UInt32 ueIndex,
		UInt32 transactionId
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA Changes end */
		);

/* SPR 2617 Start*/
extern MacRetType initPCellCQIAndRIinPUCCHMap (
        /*SPR 16855 +-*/
		UInt16 ueIndex,
		PucchConfig *pucchConfigInfo_p,
        /*SPR 16855 +-*/
        /* +- SPR 17777 */
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink*  uplkUEInfo_p,
		InternalCellIndex internalCellIndex
		);
/*SPR 2617 End*/

extern MacRetType initSchedReqInPUCCHMap(
        /*SPR 16855 +-*/
		UInt16 ueIndex,
		PucchConfig *pucchConfigInfo_p,
        /*SPR 16855 +-*/
		ULUEContext  *currentUEContext_p,
		UInt32 srConfigurationIndex,
		UInt32 srPucchResourceIndex,
		UInt16 srProhibitTimerValv920,
		InternalCellIndex internalCellIndex
		);

extern void deletePucchShortSRNodesForUE(
		/* PUCCH TDD ADAPTATION CHANGES Start */        
		PucchConfig *pucchConfig_p,UeContextForReconfig *reconfigInfo_p, InternalCellIndex internalCellIndex);
/* PUCCH TDD ADAPTATION CHANGES End */        

STATIC  MacRetType configureTpcPdcchInfoForPucch (
		UInt16 rnti,
		UInt8 tpcBitMapIndex,
		UInt8 dciFormat,
		DLUEContext *ueContext_p
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA Changes end */
		);

STATIC  MacRetType configureTpcPdcchInfoForPusch (
		UInt16 rnti,
		UInt8 tpcBitMapIndex,
		UInt8 dciFormat,
		ULUEContext *ueContext_p
		,InternalCellIndex internalCellIndex);


extern void deleteEntryFromTriggeredUeList(
		TriggeredUEListNode *triggeredUeListNodeInTpcRntiContext_p,
		TpcRntiContext * tpcRntiContext_p);

extern void deleteEntryFromConfiguredUeList(
		ConfiguredUEListNode *configuredUeListNodeInTpcRntiContext_p,
		TpcRntiContext * tpcRntiContext_p);

/* SPR 5599 changes start (ZIP ID 129191, 129075) */            
/*CA Changes start  */
extern UInt8 putEntryInExplicitDLDCIScheduleQueue(UInt16 ueIdx, 
		InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* SPR 5599 changes end (ZIP ID 129191, 129075) */            

/* Jira-429 start*/
/* SPR 21105 fix start */
/* code removed */
/* processPendingDeleteUE func is never referrred so, it and fuction it is
 * referring to  are removed */
/* SPR 21105 fix end */

/* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
/*SPR 16855 +-*/
extern void updateHDFddSemiStaticMap(UInt16 ueIndex, UInt8 reportType );
/*SPR 16855 +-*/
#endif
/* HD FDD Changes End */


/* +DYNAMIC_ICIC */
/*CA Changes start  */
void putEntryInExplicitULDCIAndPHRScheduleQueue(UInt16 ueIndex, 
		ULGrantRequestType ulRequestType, InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* -DYNAMIC_ICIC */

/* SPR 11215 Changes Start */
/* +- SPR 22354 */
#if defined(FAPI_1_1_COMPLIANCE) || defined(FAPI_4_0_COMPLIANCE) 
#ifdef FDD_CONFIG
/*CID 83622 Fix Start*/
 STATIC void fillHarqReleaseinfoForULSCH(UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
		ULUEContext *ueULContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex);
/*CA Changes end  */
#elif TDD_CONFIG
 STATIC void fillHarqReleaseinfoForULSCH(
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
		ULUEContext *ueULContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex);
/*CA Changes end  */
#endif
#endif
/* +- SPR 22354 */
/* SPR 11215 Changes End */

/* Cyclomatic Complexity changes - starts here */
STATIC  MacRetType fillULQosStrategyLcNode (
		UInt8                channelType,
		ULLogicalChannelInfo *lcInfo_p,
		ULUEContext          *newUEContext_p
		);
STATIC  MacRetType fillUlLcgInfo (
		ULLogicalChannelInfo *lcInfo_p,
		ULUEContext          *newUEContext_p,
		ulLogicalChannelInfo *logicalChInfo_p
		);
STATIC  MacRetType fillULSpsLcInfo (
		ULLogicalChannelInfo *lcInfo_p,
		ULUEContext          *newUEContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex
		/*CA Changes end  */
		);
STATIC  MacRetType fillULQosGbrInfo ( 
		ULLogicalChannelInfo *lcInfo_p,
		ULUEContext          *newUEContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex
		/*CA Changes end  */
		);
STATIC  void fillULQosNonGbrInfo ( 
		ULLogicalChannelInfo *lcInfo_p,
		ULUEContext          *newUEContext_p
		);
STATIC  void  fillULQciDeltaPowerMaskInfo (
		ULLogicalChannelInfo *lcInfo_p,
		ULUEContext          *newUEContext_p,
		InternalCellIndex internalCellIndex);

/* SPR 21105 fix start */
/* code removed */
/* processPendingDeleteUE func is never referrred so, it and fuction it is
 * referring to  are removed */
/* SPR 21105 fix end */
STATIC  void checkAndDeleteUEMeasGapConfig (
		ULUEContext *ueULContext_p,
		DLUEContext *ueDLContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex
		/*CA Changes end  */
		);
/* SPR 11215 Changes Start */
/* SPR 21105 fix start */
/* code removed */
/* processPendingDeleteUE func is never referrred so, it and fuction it is
 * referring to  are removed */
/* SPR 21105 fix end */

/*SPR 21103 Start*/
STATIC  void deleteSRSAndPucchConfigAtCreatMACUEFailure(
        ULUEContext *ueULContext_p, 
        InternalCellIndex internalCellIndex,
        UInt8   scellCount);
/*SPR 21103 End*/

/* SPR 21105 fix start */
/* code removed */
/* processPendingDeleteUE func is never referrred so, it and fuction it is
 * referring to  are removed */
/* SPR 21105 fix end */

/*SPR 16855 +-*/
/*Reconfig Changes Start */
STATIC  MacRetType checkAndReconfigureCqiInfo( 
        /* +- SPR 17777 */
		UInt16               ueIndex,
		PucchConfig          *tempPucchConfigInfo_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink   *uplkUEInfo_p,
		ULReconfigUeReq      *ulReconfigUeReq_p,
		UeContextForReconfig *reconfigInfo_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex);
/*CA Changes end  */

/*Complexity Start*/

void updateCqiStats(ULCQIPeriodicReqInfo *cqiPeriodicReqInfo_p, PucchConfig *tempPucchConfigInfo_p,InternalCellIndex internalCellIndex,PeriodicFeedbackType periodicFeedBack,ULReconfigUeReq *ulReconfigUeReq_p);
void updateRiStats(ULCQIPeriodicReqInfo *cqiPeriodicReqInfo_p, PucchConfig *tempPucchConfigInfo_p,InternalCellIndex internalCellIndex,ULReconfigUeReq *ulReconfigUeReq_p);

/*Complexity End*/

STATIC  MacRetType checkAndReconfigureSRInfo( 
        /* +- SPR 17777 */
		UInt16               ueIndex,
		PucchConfig          *tempPucchConfigInfo_p,
		UeContextForUplink   *uplkUEInfo_p,
		ULReconfigUeReq      *ulReconfigUeReq_p,
		UeContextForReconfig *reconfigInfo_p,
		InternalCellIndex internalCellIndex);
STATIC  void checkAndReconfigureSRSDedicatedConfig( 
		UInt16               ueIndex,
		SRSConfigInfo        *tempsrsConfigInfo_p, 
		UeContextForUplink   *uplkUEInfo_p,
		ULReconfigUeReq      *ulReconfigUeReq_p,
		UInt32               currSFN, 
		UInt32               currSF,
		UeContextForReconfig *reconfigInfo_p,
		InternalCellIndex internalCellIndex);
extern  MacRetType reconfigurePCellCqiRiPassiveContext( 
		ULUEContext          *ulUeCtx_p,
		UInt16               ueIndex,
		PucchConfig          *tempPucchConfigInfo_p,
        /* +- SPR 17777 */
		UeContextForUplink   *uplkUEInfo_p,
		UeContextForReconfig *reconfigInfo_p,        
		/*CA Changes start  */
		InternalCellIndex internalCellIndex);
/*CA Changes end  */
extern  MacRetType reconfigureSRPassiveContext( 
		ULUEContext          *ulUeCtx_p,
        /* +- SPR 17777 */
		UInt16               ueIndex,
		PucchConfig          *tempPucchConfigInfo_p,
		UeContextForUplink   *uplkUEInfo_p,
		UeContextForReconfig *reconfigInfo_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex);
/*CA Changes end  */
extern  MacRetType reconfigureSRSPassiveContext(
		UInt16               ueIndex,
		SRSConfigInfo        *tempsrsConfigInfo_p, 
		UeContextForUplink   *uplkUEInfo_p,
		UInt32               currSFN, 
		UInt32               currSF, 
		UeContextForReconfig *reconfigInfo_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex);
/*CA Changes end  */
/*Reconfig Changes End */
/*SPR 16855 +-*/

STATIC  void updateLcgLcParameters(ULUEContext *ueContext_p,
		/*CA Changes start  */
		UInt8 ueLCId, InternalCellIndex internalCellIndex);
/*CA Changes end  */
STATIC  void updateLcInfoDuringUlQosScheduling(ULUEContext *ueContext_p,
		/*CA Changes start  */
		UInt8 ueLCId, InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* +- SPR 17777 */
STATIC  void updateQciPowerMaskAndLcgId(ULUEContext *ueContext_p);
/* +- SPR 17777 */

STATIC  MacRetType fillDLUeContextParams ( 
		UInt32               ueIndex,
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p,
		/* SPR 15909 fix start */
        tickType_t               currentTick
        /* SPR 15909 fix end */
		/* CA Changes start */
		,InternalCellIndex internalCellIndex);
/* CA Changes end */
STATIC  void fillDLCtxtAmbrParams ( 
        /* +- SPR 17777 */
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p
        /* +- SPR 17777 */
		,InternalCellIndex internalCellIndex);
STATIC  void fillDLCtxtCqiInfo ( 
		UInt32               ueIndex,
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p,
		/* SPR 15909 fix start */
        tickType_t               currentTick,
        /* SPR 15909 fix end */
		InternalCellIndex internalCellIndex);
STATIC  void fillDLCtxtSchedParams ( 
        /* +- SPR 17777 */
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p,
		/* SPR 15909 fix start */
        tickType_t               currentTick,
        /* SPR 15909 fix end */
		InternalCellIndex internalCellIndex);
STATIC  MacRetType fillDLCtxtPowerCtrlParams (
        /* +- SPR 17777 */
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA Changes end */
		);
STATIC  MacRetType fillDLCtxtSpsInfo (
		UInt32               ueIndex,
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p
		/* CA changes Start */
		, InternalCellIndex internalCellIndex
		/* CA Changes end */
		);

STATIC  MacRetType fillReconfigPowerCtrlInfo (
		UeContextForDownlink *dwlkUEInfo_p, 
        /* +- SPR 17777 */
		DLReconfigUeReq     *dlReconfigUeReq_p);
        /* +- SPR 17777 */
STATIC  MacRetType fillReconfigUECtxtProvParams (
		UeContextForDownlink *dwlkUEInfo_p, 
		DLUEContext          *newUEcontext_p,
        /* +- SPR 17777 */
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		);
STATIC  MacRetType fillReconfigCqiInfo (
		UeContextForDownlink *dwlkUEInfo_p, 
        /* +- SPR 17777 */
		DLUEContext          *newUEcontext_p
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		);
STATIC  void fillReconfigUECategoryInfo (
        /* +- SPR 17777 */
		DLUEContext          *newUEcontext_p
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		);
STATIC  void fillReconfigDLAmbrInfo (
		UeContextForDownlink *dwlkUEInfo_p 
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		,InternalCellIndex internalCellIndex      
		);
STATIC  MacRetType fillReconfigSpsInfo (
		UeContextForDownlink *dwlkUEInfo_p, 
		DLUEContext          *newUEcontext_p,
        /* +- SPR 17777 */
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		);
STATIC  MacRetType fillReconfigSpsDLRequestType (
		UeContextForDownlink *dwlkUEInfo_p, 
		DLUEContext          *newUEcontext_p,
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		DLReconfigUeReq     *dlReconfigUeReq_p
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		);
        /*SPR 16855 +-*/
STATIC  MacRetType fillReconfigDLSpsSetupInfo (
		UeContextForDownlink *dwlkUEInfo_p, 
        /* +- SPR 17777 */
		DLUEContext          *newUEcontext_p
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		);
/* +- SPR 17777 */
STATIC  MacRetType fillReconfigLogicalChConfig (
		UeContextForDownlink *dwlkUEInfo_p 
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p);
/* +- SPR 17777 */
/* CA Changes end */
STATIC  void fillULSyncLossAndSchedInfo ( ULUEContext  *ulUEContext_p 
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA Changes end */
		);
STATIC  void fillULContextLcInfo ( ULUEContext  *ulUEContext_p );
STATIC  void fillULContextHarqInfo ( ULUEContext        *ulUEContext_p,
		UeContextForUplink *ueContextInfo_p
		);
STATIC  void fillULUECategoryAndMcsInfo (
		UInt32             ueIndex,
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p,
		InternalCellIndex internalCellIndex
		);
STATIC  MacRetType fillULContextAmbrInfo ( UInt32       lchCount,
		ULUEContext        *ulUEContext_p,
		UeContextForUplink *ueContextInfo_p
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA changes End */
		);
STATIC  MacRetType fillULPowerCtrlAndCsiInfo ( 
		ULUEContext        *ulUEContext_p,
		UeContextForUplink *ueContextInfo_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex
		/*CA Changes end  */
		);
STATIC  MacRetType fillULSPSInfo ( 
		UInt32               ueIndex, 
		ULUEContext          *ulUEContext_p,
		UeContextForUplink   *ueContextInfo_p,
		UeContextForDownlink *ueDlConfigInfo_p,
		InternalCellIndex internalCellIndex
		);
STATIC  MacRetType fillULSpsSetupInfo ( 
		UInt32               ueIndex, 
		ULUEContext          *ulUEContext_p,
		UeContextForUplink   *ueContextInfo_p,
		UeContextForDownlink *ueDlConfigInfo_p,
		LP_SpsUlInfo         spsUlInfo_p,
		InternalCellIndex internalCellIndex
		);
STATIC  void fillULPucchConfigInfo (
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p,
		InternalCellIndex internalCellIndex
		);
#ifdef TDD_CONFIG
STATIC  void fillULUeScheduledInfo ( ULUEContext  *ulUEContext_p,
		InternalCellIndex internalCellIndex);
#endif

STATIC  void fillULPuschAndCqiInfo (
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA Changes end */
		);
STATIC  void fillULUserLocTypeAndPhrMimoInfo (
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p,
		/*CA Changes start */
		InternalCellIndex internalCellIndex
		/*CA Changes end */
		);
STATIC  void resetULEcidReportInfo(ULUEContext *ulUEContext_p);

/* CA changes start */
 MacRetType reconfigureSCellCqiRiPassiveContext(
		ULUEContext          *ulUeCtx_p,
		UInt16               ueIndex,
        /* +- SPR 17777 */
		UeContextForUplink   *uplkUEInfo_p,
		UeContextForReconfig *reconfigInfo_p,
		UeCellInfo * ueCellInfo_p,
		UInt8    internalCellIndex
		);
STATIC  void macUpdateCreateDLCAParams(
		DLUEContext        *newUEcontext_p,
		UeContextForDownlink *ueDlConfigInfo_p,
		InternalCellIndex internalCellIndex
		);
STATIC  MacRetType macUpdateCreateDLScellParams(
		DLUEContext        *newUEcontext_p,
		UeContextForDownlink *ueDlConfigInfo_p,
		UInt8 index,
		InternalCellIndex internalCellIndex
		);
STATIC  MacRetType macFillScellDLUeContextParams (
		UInt32               ueIndex,
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p,
		UInt8                index,
		InternalCellIndex internalCellIndex
		);
STATIC  MacRetType macFillScellDLCtxtCqiInfo ( 
		UInt32               ueIndex,
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p,
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
		UInt8                cellId);
STATIC  void macUpdateCreateULCAParams(
		ULUEContext        *ulUEContext_p,
        /* +- SPR 17777 */
		UeContextForUplink *upUEInfo_p
		);
STATIC  MacRetType macUpdateCreateULScellParams(
		ULUEContext        *ulUEContext_p,
		UeContextForUplink *uplkUEInfo_p,
		UInt8 index,
		InternalCellIndex internalCellIndex
		);
STATIC  void macFillScellULUeParams (
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p,
		UInt8 index
		);
STATIC  void macFillScellULUserLocType (
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p,
		UInt8 index,
		InternalCellIndex internalCellIndex
		);
 MacRetType macFillCreateScellCommonParams(
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink  *uplkUEInfo_p,
		ULUEContext *newULUEContext_p,
		DLUEContext *newDLUEContext_p,
		UInt8 scellCount,
		InternalCellIndex internalCellIndex
		);
        /*SPR 16855 +-*/
 MacRetType macConfigureScellPucchSRSAndReports(
		PucchConfig *tempPucchConfigInfo_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink  *uplkUEInfo_p,
		ULUEContext *newULUEContext_p,
		DLUEContext *newDLUEContext_p,
		UInt8 cellId,
		InternalCellIndex internalCellIndex
		);
        /*SPR 16855 +-*/
/* +- SPR 17777 */
STATIC  void macFillScellULPucchConfigInfo (
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p,
		UInt8 cellId
#ifdef TDD_CONFIG
		,InternalCellIndex internalCellIndex
#endif
		);
/* +- SPR 17777 */
STATIC  MacRetType macValidateAndFillDLScellReconfigParams(
		UeContextForDownlink *dwlkUEInfo_p,
		DLUEContext        *dlUEContext_p,
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		);
STATIC  MacRetType macfillDLScellReconfigUECtxtProvParams (
		ScellInfo            *scellTemp_p, 
		DLUESCellContext     *contextScell_p,
		UInt8                scellIndex,
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		);
STATIC  void macfillDLReconfigUECategoryV1020Info (
		UeContextForDownlink *dwlkUEInfo_p, 
		DLUEContext          *newUEcontext_p,
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		);
STATIC  MacRetType macfillDLScellReconfigCqiInfo (
		ScellInfo            *scellTemp_p, 
		DLUESCellContext     *contextScell_p,
		UInt8                scellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		);
STATIC  void macValidateDLScellReconfigRIParams (
		ScellInfo            *scellTemp_p, 
		UInt8                scellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		);
STATIC  MacRetType macValidateAndFillULScellReconfigParams(
		UeContextForUplink *uplkUEInfo_p,
		ULUEContext        *ulUEContext_p,
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,ULReconfigUeReq     *ulReconfigUeReq_p
        /*SPR 16855 +-*/
		);
STATIC  void macValidateULScellReconfigParams (
		ScellInfo            *scellTemp_p, 
		UInt8                scellIndex
        /*SPR 16855 +-*/
		,ULReconfigUeReq     *ulReconfigUeReq_p
        /*SPR 16855 +-*/
		);
STATIC  MacRetType macValidateULReconfigUECategoryV1020Params (
		UeContextForUplink *uplkUEInfo_p,
        /* +- SPR 17777 */
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,ULReconfigUeReq     *ulReconfigUeReq_p
        /*SPR 16855 +-*/
		);
STATIC  MacRetType macReconfigureScellPUCCHSRSAndReports( 
		ULUEContext *ulUeCtx_p,
		UInt8 scellCount,
		PucchConfig          *tempPucchConfigInfo_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink   *uplkUEInfo_p,
		ULReconfigUeReq      *ulReconfigUeReq_p,
		UeContextForReconfig *reconfigInfo_p,
		InternalCellIndex internalCellIndex);

STATIC  MacRetType macCheckAndReconfigureScellCqiInfo( 
		UInt16               ueIndex,
		UInt8                scellCount,
		PucchConfig          *tempPucchConfigInfo_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink   *uplkUEInfo_p,
		ULReconfigUeReq      *ulReconfigUeReq_p,
		UeContextForReconfig *reconfigInfo_p,
		InternalCellIndex internalCellIndex);
STATIC  MacRetType macFillReconfigureScellCommonParams (
		ULUEContext *newULUEContext_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink   *uplkUEInfo_p,
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,ULReconfigUeReq *ulReconfigUeReq_p
		,DLReconfigUeReq *dlReconfigUeReq_p
		,UeContextForReconfig *reconfigInfo_p
        /*SPR 16855 +-*/
		);
/* CA changes End */


/* Cyclomatic Complexity changes - ends here */
typedef enum DlAggregateGbrTriggerT                
{                                                  
	ADD_DL_AGG_GBR_TRIGGER,                        
	DELETE_DL_AGG_GBR_TRIGGER,                     
	INVALID_ADD_DELETE_TRIGGER                                
}DlAggregateGbrTrigger;                            

STATIC   void initUEConfQueuesAndLists(UInt8 numOfCells);

STATIC  void updateDlAggregateGbrPerTTI( DLUEContext* ueDLContext_p,
		DlLcQosInfo* dlLcQosInfo_p,
		DlAggregateGbrTrigger addDeleteTrigger,
		UInt16 numOfTicksPerSecond);
/*CYCLOMATIC_CHANGES_6.1_START*/
STATIC  void determineULMinDataSize(UeContextForUplink *uplkUEInfo_p,
		ULUEContext *ueContext_p);
STATIC  void determineMaxTBSize(UeContextForUplink *uplkUEInfo_p,
		ULReconfigUeReq *ulReconfigUeReq_p);
/*SPR 16855 +-*/
STATIC  void populateUplinkSpsInfo(UeContextForUplink *uplkUEInfo_p,
		ULUEContext *ueContext_p,
		InternalCellIndex internalCellIndex,
		ULReconfigUeReq *ulReconfigUeReq_p);
/*SPR 16855 +-*/

STATIC  MacRetType validateUeCatV1020ReqType(UeContextForUplink *uplkUEInfo_p,
		InternalCellIndex internalCellIndex,
		ULReconfigUeReq *ulReconfigUeReq_p);
STATIC  void populatePuschConfigInfo(UeContextForUplink *uplkUEInfo_p,
		ULReconfigUeReq *ulReconfigUeReq_p);
STATIC  void populateDedicatedULPowerControlInfo(UeContextForUplink *uplkUEInfo_p,
        /* +- SPR 17777 */
		ULReconfigUeReq *ulReconfigUeReq_p);
/* +- SPR 17777 */
/*SPR 16855 +-*/
STATIC  void populateTpcPdcchConfigPuschInfo(UeContextForUplink *uplkUEInfo_p,
		ULReconfigUeReq *ulReconfigUeReq_p);
/*SPR 16855 +-*/
/*SPR 16855 +-*/
STATIC  void populateUplinkLcInfo(UInt32 uplkLCCount, 
		ULReconfigUeReq *ulReconfigUeReq_p,
		UeContextForUplink *uplkUEInfo_p);
/*SPR 16855 +-*/
STATIC  void populateN1PucchListN1PucchCs(ULReconfigUeReq *ulReconfigUeReq_p,
		UeContextForUplink *uplkUEInfo_p);
STATIC  void fillUlQosAmbr(UeContextForUplink *uplkUEInfo_p,
        /*SPR 16855 +-*/
        ULReconfigUeReq *ulReconfigUeReq_p
        /*SPR 16855 +-*/
		);


/*SPR 18564 + */
extern void initDlQosRbRestriction(InternalCellIndex internalCellIndex);
extern void initUlQosRbRestriction(InternalCellIndex internalCellIndex);
/*SPR 18564 + */
/*CYCLOMATIC_CHANGES_6.1_END*/
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
DRX_Q_TYPE *dlDRXQueue_gp[MAX_NUM_CELL];

/*CA Changes start  */
LTE_SQUEUE* pendingUEDeleteQueue_gp[MAX_NUM_CELL] = {PNULL};
LTE_SQUEUE* macResetQueue_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end  */

DLUEContextInfo dlUECtxInfoArr_g[MAX_UE_SUPPORTED];
ULUEContextInfo ulUECtxInfoArr_g[MAX_UE_SUPPORTED];
/* Variable to store the % increment in bler when unsure 
   (other than Harq ack/Nack) received*/
UInt8 pucchBlerUnsureWeightage_g[MAX_NUM_CELL]   = {0};
/* Variable to store the % decrement in bler when Harq ACK/NACK received*/
UInt8 pucchBlerSureWeightage_g[MAX_NUM_CELL]   = {0};
/* CA changes start */
UInt32 ttiCountDl_g[MAX_NUM_CELL];
UInt32 ttiCountUl_g[MAX_NUM_CELL];
/* CA changes end */
/* Variable used for CSI parameters */
/* EICIC +*/
extern UInt8 usableAbsCountNew_g[MAX_NUM_CELL] ;
extern UInt8 usableAbsCountOld_g[MAX_NUM_CELL] ;
extern UInt8	absCountOld_g[MAX_NUM_CELL] ;
extern UInt8	absCountNew_g[MAX_NUM_CELL] ;
/* EICIC -*/


UInt8 temp_g = 0;
UInt8 temp2_g = 0;

extern DeltaBLERToTPCMapForPUCCH deltaBlerToTPCMapForPUCCH_g;
extern TpcRntiContext * tpcPuschRntiContext_gp[MAX_NUM_CELL];
extern TpcRntiContext * tpcPucchRntiContext_gp[MAX_NUM_CELL];

/*SPR 16855 +-*/
/* CA changes Start */
extern RrcOamHiPrioQNode *ulRrcOamDeleteUeQNodeReq_p[MAX_NUM_CELL];
extern RrcOamHiPrioQNode *dlRrcOamDeleteUeQNodeReq_p[MAX_NUM_CELL];
extern RRC_OAM_LPTOHP_THDH_PRIO_TYPE rrcOamLPtoHPThdHPrioQ_g[MAX_NUM_CELL][MAX_THD];
extern RRC_OAM_HPTOLP_THDH_PRIO_TYPE rrcOamHPtoLPThdHPrioQ_g[MAX_NUM_CELL][MAX_THD];
/* CA changes End */
/*SPR 16855 +-*/


/* ICIC changes start */
extern DLResourceAllocationInfo *dlRAInfo_g[MAX_NUM_CELL];
/* ICIC changes end */
/* CA changes start */
extern RRMMACCellGlobal *rrmMacCellGlobal_gp[MAX_NUM_CELL];
/* CA changes end */
SpsCrntiContext spsCrntiMap_g[MAX_NUM_CELL];
/*+ Dyn UEs To Be SchPerTTI:CR +*/
extern UInt8 modifyMaxUeToBeSchdDlUl_g[MAX_NUM_CELL];
/*- Dyn UEs To Be SchPerTTI:CR -*/

/* Memory handling changes start */
#define MAX_RRC_OAM_PENDING_REQ 10
/* Memory handling changes end */
/*BUG:9307:START*/
/*BUG:9307:END*/
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
extern mocnOperatorParams mocnOperatorParams_g;
/* MOCN Changes End */
/* SPR 23402 Changes End */
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/* Table 7.2.3-1 of 3GPP TS 36.213 will be used to find the efficiency against
   the given CQI Index.*/
STATIC UInt8 mcsCQIArr_g[MAX_MCS_INDEX] = { 2 /* 0 */,
	3 /* 1 */,
	3 /* 2 */,
	4 /* 3 */,
	4 /* 4 */,
	5 /* 5 */,
	5 /* 6 */,
	6 /* 7 */,
	6 /* 8 */,
	7 /* 9 */,
	7 /* 10 */,
	7 /* 11 */,
	8 /* 12 */,
	8 /* 13 */,
	9 /* 14 */,
	9 /* 15 */,
	10 /* 16 */,
	10 /* 17 */,
	10 /* 18 */,
	11 /* 19 */,
	11 /* 20 */,
	12 /* 21 */,
	12 /* 22 */,
	13 /* 23 */,
	13 /* 24 */,
	14 /* 25 */,
	14 /* 26 */,
	15/* 27 */, 
	15/* 28 */ };

/* SPS_CHG */
UInt8 ulImplicitReleaseAfterMap[INVALID_IMPLICIT_RELEASE] = {2 /* 0 */,
	3 /* 1 */,
	4 /* 2 */,
	8 /* 3 */};
/*! \code
 * -Map for getting the actual Interval
 * \endcode
 * */
UInt16 spsIntervalMap_g[SPS_INTERVAL_MAX] = {SPS_INTERVAL_10,
	SPS_INTERVAL_20,
	SPS_INTERVAL_32,
	SPS_INTERVAL_40,
	SPS_INTERVAL_64,
	SPS_INTERVAL_80,
	SPS_INTERVAL_128,
	SPS_INTERVAL_160,
	SPS_INTERVAL_320,
	SPS_INTERVAL_640};

/* SPS_CHG */
UInt32 totalDlAggregateGbrPerTTI_g[MAX_VALID_UE_TYPE] = { 0 };     
extern UInt8 updateCQIMapForAbs_g[MAX_NUM_CELL];


/* EICIC +*/

#define  CALCULATE_ABS_FACTOR(internalCellIndex, absCount, absFactor,ueContext_p) \
	if (usableAbsCountNew_g[ ueContext_p->internalCellIndex] > 0) \
{\
	absCount = usableAbsCountNew_g[ ueContext_p->internalCellIndex];\
}\
else if (absCountNew_g[ ueContext_p->internalCellIndex] > 0)\
{\
	absCount = absCountNew_g[ueContext_p->internalCellIndex];\
}\
else\
{\
	absCount = 0;\
}\
if (absCount)\
{\
	if (NON_VICTIM_UE == ueContext_p->userType)\
	{\
		absFactor = (UDouble32) (NUM_BITS_ABS_PATTERN/ (UDouble32) (NUM_BITS_ABS_PATTERN - absCount));\
	}\
	else\
	{\
		absFactor = (UDouble32) (NUM_BITS_ABS_PATTERN/ (UDouble32) (absCount));\
	}\
}\
else\
{\
	absFactor = ONE;\
}

/* EICIC -*/


/****************************************************************************
 * Function Name  : initUEConfMgr 
 * Inputs         : Number of cells configured
 * Outputs        : Initialize the Queue's & List
 * Returns        : None 
 * Description    : This function is used to initialize the Queue's and List
 ****************************************************************************/
/*CA Changes start  */
void initUEConfMgr(UInt8 numOfCells)
	/*CA Changes end  */
{
	/* Review comment fix start MAC_AT_2 */
	UInt16 index = 0;
	/* Review comment fix end MAC_AT_2 */

	/* SPR 13492 Changes Start */
#ifdef KPI_STATS
	UInt8 qci       = INVALID_QCI;
#endif
	/* SPR 13492 Changes End */

	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	/* DL DRX Queue is Initialize*/
	/* Pending UE Delete Queue Initialize */
/* SPR 21105 fix start */
/* code removed */
/* processPendingDeleteUE func is never referrred so, it and fuction it is
 * referring to  are removed */
/* SPR 21105 fix end */
	/*CA Changes start  */
	createMemPool(sizeof(LTE_SQUEUE), numOfCells);
	createMemPool(sizeof(DRX_Q_TYPE), numOfCells);
	createMemPool(sizeof(LTE_SQUEUE), numOfCells);
	/* Cyclomatic Complexity changes start */
	initUEConfQueuesAndLists(numOfCells);
	/* Cyclomatic Complexity changes end */

	/*CA Changes end  */

	/* Review comment fix start MAC_AT_2 */
	for (index = 0; index < MAX_SUPPORTED_USER; index++)
	{
		dlUECtxInfoArr_g[index].pendingDeleteFlag = TRUE;
		ulUECtxInfoArr_g[index].pendingDeleteFlag = TRUE;

		/*CA Changes start  */
		ulUECtxInfoArr_g[index].scellRelFlag = FALSE;
		dlUECtxInfoArr_g[index].scellRelFlag = FALSE;
		/*CA Changes end  */

		/* SPR 13492 Changes Start */
#ifdef KPI_STATS
		for (qci = 0; qci < MAX_QCI ; ++qci)
		{
			dlUECtxInfoArr_g[index]. 
				inSessionTimePerQCIFlag[qci] = FALSE;
			ulUECtxInfoArr_g[index]. 
				inSessionTimePerQCIFlag[qci] = FALSE;
		}
#endif
		/* SPR 13492 Changes End */
	}
	/* Review comment fix end MAC_AT_2 */
	/* CA changes start */
	for(index =0; index < MAX_NUM_CELL; index++)
	{
		ttiCountUl_g[index] = UE_CONTEXT_DEL_DELAY;
		ttiCountDl_g[index] = UE_CONTEXT_DEL_DELAY;
		rrcOamDlEventId_g[index] = MAC_CTRL_MAX_EV;
		rrcOamUlEventId_g[index] = MAC_CTRL_MAX_EV;
	}
	/* CA changes end */

	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
}

/****************************************************************************
 * Function Name  : initUEConfQueuesAndLists 
 * Inputs         : numOfCells : Number of cells configured
 * Outputs        : Initialize the Queue's & List
 * Returns        : None 
 * Description    : This function is used to initialize the Queue's and List
 ****************************************************************************/
STATIC void initUEConfQueuesAndLists(UInt8 numOfCells)
{
	UInt16 cellCount = 0;
	/* CA Changes start */
	UInt16 macMaxNumOfUeSupported = 0;
	/* CA Changes end */
	for (cellCount = 0; cellCount < numOfCells; cellCount++)
	{

		GET_MEM_FROM_POOL(LTE_SQUEUE, pendingUEDeleteQueue_gp[cellCount], sizeof(LTE_SQUEUE), PNULL);
		sQueueInit(pendingUEDeleteQueue_gp[cellCount]);

		GET_MEM_FROM_POOL(DRX_Q_TYPE, dlDRXQueue_gp[cellCount], sizeof(DRX_Q_TYPE), PNULL);
		INIT_DRX_Q(dlDRXQueue_gp[cellCount]);   

		macMaxNumOfUeSupported = cellSpecificParams_g.cellConfigAndInitParams_p\
					 [cellCount]->initParams_p->macMaxNumOfUeSupported; 
		GET_MEM_FROM_POOL(LTE_SQUEUE, macResetQueue_gp[cellCount], sizeof(LTE_SQUEUE), PNULL);
		sQueueInit(macResetQueue_gp[cellCount]);
		/* Start 128 UE: Memory Changes */
		/* Create Mempool for Downlink UE Context */
		createMemPool(sizeof (DLUEContext), (macMaxNumOfUeSupported)); 

		/* Create Mempool for Uplink UE Context */
		createMemPool(sizeof (ULUEContext), (macMaxNumOfUeSupported)); 

		/* Create Mempool for DRX Node*/
		createMemPool(sizeof (DRXQueueNode), (macMaxNumOfUeSupported)); 

		/* Create MemPool for pending delete UE node (Uplink/Downlink)*/
		createMemPool(sizeof (PendingDeleteQueueNode), MULTIPLY_BY_TWO(macMaxNumOfUeSupported));

		/* Added Missing CreateMemPool */
		/* SPR 21381 Fix Start */
        /* In createTempCRNTICtx() creates DL Harq Entity for Temp-CRNTI and
         * fillDLUeContextParams() creates DL Harq Enity for CRNTI ; So 
         * createmempool for DL Harq Enity is moved in initRNTIPoolTbl */
		/* SPR 21381 Fix End */
		createMemPool(sizeof(SubBandReportInfo), (macMaxNumOfUeSupported));

		createMemPool(sizeof(MacResetQueueNode), MAX_NUM_PENDING_RESET_UES);
		/* End 128 UE: Memory Changes */

		/* Memory handling changes start */
		createMemPool(sizeof(DLPriorityQueueNode), (cellSpecificParams_g.cellConfigAndInitParams_p\
					[cellCount]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL));
		createMemPool(sizeof(PCCHInfoNode), macMaxNumOfUeSupported);
		createMemPool(sizeof(rrcInfoData), 1);
		createMemPool((sizeof(MIBMsgFromRRC) * MAX_MIB_FROM_RRC),2);
#ifdef FDD_CONFIG    
		createMemPool(sizeof(ContainerAckQueueNode),          
				(cellSpecificParams_g.cellConfigAndInitParams_p\
				 [cellCount]->initParams_p->ulSchdConfig.maxUEsToBeScheduledUL * 2));
		createMemPool(sizeof(RrcOamPendingReq),MAX_RRC_OAM_PENDING_REQ);
		/* MAX_RRC_OAM_PENDING_REQ is enough to cater the current requirement *
		 * If reqd, the value can be increased */
#endif    
		/* Memory handling changes end */
	}       
}
/* SPR 3271 changes start */
/******************************************************************************
 * Function Name : initPucchBlerContext 
 * Inputs        : DLUEContext *newUEcontext_p:Pointer to UE's DL Context
 *                 internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs       : None 
 * Description   : Initializes Pucch Bler in UE's DL Context 
 *****************************************************************************/
/*CA Changes start  */
void initPucchBlerContext(DLUEContext *newUEcontext_p, 
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
{    
	/* Initialize blerInfoForPUCCH */
	newUEcontext_p->blerInfoForPUCCH.pucchBler = 
		/*CA Changes start  */
		(macReconfigSchedulerParamsDL_gp[internalCellIndex]\
		 + schParamsIndexDL_g[internalCellIndex])->\
		/*CA Changes end  */
		deltaBlerToTPCMapForPUCCH.pucchBLERTarget;
	newUEcontext_p->blerInfoForPUCCH.blerCalculated  = 0;
	newUEcontext_p->blerInfoForPUCCH.blerCalculatedCounter  = 0;
	newUEcontext_p->blerInfoForPUCCH.lastBlerCoolOffCounter = 0;
}
/* SPR 3271 changes end */

/* Coverity 32786 32785 Fix Start  */
/******************************************************************************
 * Function Name  : deleteCreatedDownlinkUEContext
 * Inputs         : ueIndex 
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None 
 * Returns        : None 
 * Description    :The function is used to delete created UE context 
 to release memory allocated for DLUE Context.
 *******************************************************************************/
STATIC  void deleteCreatedDownlinkUEContext(UInt16 ueIndex,
		InternalCellIndex internalCellIndex)
{
	DLUEContext *ueDLContext_p = PNULL;
	UInt8 lcCount=0;
	/* CA changes Start*/
	UInt8 scellId = 1;
	/* CA changes End*/
	DLLogicalChannelInfo *dlLogicalChInfo_p  = PNULL;
	UInt8 harqCount = 0;
	DLHARQProcess *harqProcess_p = PNULL;
	DLHARQEntity *harqEntity_p = PNULL;
	/* SPR 7122 fix start*/
	DLCQIInfo *dlCQIInfo_p = PNULL;
	SpsDlInfo *spsDlInfo_p  = PNULL;
	UInt8 tpcRntiIndex = 0;
	TpcRntiContext *tpcRntiContext_p = PNULL;
	ConfiguredUEListNode *configuredListNode_p = PNULL;     
	UInt8 ueServCellIndex  = 0;
	DLUESCellContext *dlUeScellContext_p = PNULL;
	SCELLDLCQIInfo              *ScelldlCQIInfo_p =  PNULL;
	/* SPR 7122 fix end*/
	/* SPR 13492 Changes Start */
#ifdef KPI_STATS
	UInt8 qci       = INVALID_QCI;
#endif
	/* SPR 13492 Changes End */

	ueDLContext_p  = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

	if(PNULL != ueDLContext_p)
	{
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        updateNumOfActiveUeDlOnUeDelete(ueDLContext_p->ueIndex,internalCellIndex);
#endif
        /* SPR 16916 FIX END */
		if (ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p)
		{
            /* SPR 13562 changes */
            deleteEntryFromUENonZeroQueueLoadAndHarqList(
                    /*  +- SPR 17777 */
                    ueDLContext_p,internalCellIndex);
            /*  +- SPR 17777 */
            /* SPR 13562 changes */
			ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p = PNULL;
		}

		for(lcCount=0;lcCount<MAX_NUMBER_OF_LOGICAL_CHANNEL;lcCount++)
		{
			dlLogicalChInfo_p=&ueDLContext_p->logicalChannel[lcCount];
			if(dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p)
			{
				if(IS_CA_ENABLED())
				{
					for (scellId = MAX_NUM_PCELL; scellId <= MAX_NUM_SCELL; scellId++)
					{
						freeMemPool(dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->\
								dLUESCellStartegyNode_p[scellId]);
						dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->\
							dLUESCellStartegyNode_p[scellId] = PNULL;
					}
				}
				freeMemPool(dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p);
				dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p=PNULL;
			}
		}

		/* CA changes Start */

		for (scellId = MAX_NUM_PCELL; scellId <= MAX_NUM_SCELL; scellId++)
		{
			macDeleteDLScellContext(ueDLContext_p,scellId,internalCellIndex);
		}
		/* CA changes End */

		/* HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
		if(TRUE == ueDLContext_p->isHDFddFlag)
		{
			deleteUeFromHdFddContext(ueDLContext_p,internalCellIndex);
		}
#endif
		/* HD FDD Changes End*/

		harqEntity_p = ueDLContext_p->dlHarqContext_p;

		if (harqEntity_p != PNULL)
		{
			for (;harqCount < harqEntity_p->numHARQProcess; harqCount++)
			{
				harqProcess_p = &harqEntity_p->harqProcess[harqCount];
				/* SPR 5288 Start*/
				freeDLHarqProcess(ueDLContext_p,harqProcess_p, internalCellIndex);
				/* SPR 5288 End */
			}
			/*Coverity Id 58869 Fix Start*/
			freeMemPool(harqEntity_p);
            harqEntity_p = PNULL;
			/*Coverity Id 58869 Fix End*/
		}


		/* SPR 7122: deletion of Stale UE context fix start */
		if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
		{
			dlCQIInfo_p = &ueDLContext_p->dlCQIInfo; 
			if ( PNULL != dlCQIInfo_p->subBandReportInfo_p )
			{
				freeMemPool(dlCQIInfo_p->subBandReportInfo_p);
				dlCQIInfo_p->subBandReportInfo_p = PNULL;
			}
		}
		else
		{
			for(ueServCellIndex = 1 ; ueServCellIndex <= ueDLContext_p->scellCount && \
					ueDLContext_p->dlUeScellContext_p[ueServCellIndex]; ueServCellIndex++)
			{
				dlUeScellContext_p = ueDLContext_p->\
						     dlUeScellContext_p[ueServCellIndex];

				if(PNULL != dlUeScellContext_p)
				{
					ScelldlCQIInfo_p = &dlUeScellContext_p->ScelldlCQIInfo;

					if ( PNULL != ScelldlCQIInfo_p->subBandReportInfo_p )
					{
						freeMemPool(ScelldlCQIInfo_p->subBandReportInfo_p);
						ScelldlCQIInfo_p->subBandReportInfo_p = PNULL;
					}
					break;
				}
				freeMemPool(dlUeScellContext_p);
				ueDLContext_p->dlUeScellContext_p[ueServCellIndex] = PNULL;
			}
		}


		spsDlInfo_p = &ueDLContext_p->spsDlInfo;
		if ( PNULL != spsDlInfo_p->spsDlSetupInfo_p )
		{
			freeMemPool(spsDlInfo_p->spsDlSetupInfo_p);
		}

		/* Updating total number of cell edge user count  
		   in case UE context creation get fail*/

		if(ueDLContext_p->userLocationType == CE_USER)
		{
			--(rrmMacCellGlobal_gp[internalCellIndex]->totalCellEdgeUserCount);
		} 

		/* Removing UE information from the global UE context */
		/* deleteUEPeriodicReportBatchInfo(ueIndex) have been removed due to 
		   SPR fix 10056 */ 

		tpcRntiIndex = ueDLContext_p->tpcInfoForPucch.tpcRntiIndex;
        /* + SPR 22354 */ 
		tpcRntiContext_p = (tpcPucchRntiContext_gp[internalCellIndex] + tpcRntiIndex); 
        /* - SPR 22354 */ 
		configuredListNode_p = ueDLContext_p->tpcInfoForPucch.configuredUeListNodeInTpcRntiContext_p;

		if(configuredListNode_p)
		{
			/* Delete node configuredUeListNodeInTpcRntiContext_p from configuredUeList */
			deleteEntryFromConfiguredUeList(configuredListNode_p, tpcRntiContext_p);
            /* + SPR 22354 */ 
            ueDLContext_p->tpcInfoForPucch.configuredUeListNodeInTpcRntiContext_p = PNULL;
            tpcRntiContext_p->configuredCount--;
            /* - SPR 22354 */ 
		}

		/* SPR 7122 fix end */

		freeMemPool(ueDLContext_p);
		dlUECtxInfoArr_g[ueIndex].dlUEContext_p = PNULL;
		dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag = TRUE;
		dlUECtxInfoArr_g[ueIndex].scellRelFlag = FALSE;
		/* SPR 13492 Changes Start */
#ifdef KPI_STATS
		for (qci = 0; qci < MAX_QCI ; ++qci)
		{
			dlUECtxInfoArr_g[ueIndex]. 
				inSessionTimePerQCIFlag[qci] = FALSE;
		}
#endif
		/* SPR 13492 Changes End */
	}

}
/* Coverity 32786 32785 Fix End */

/****************************************************************************
 * Function Name  : createMACDLUEEntity
 * Inputs         : ueDlConfigInfo_p - Its point to the UeContextForDownlink
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                                      
 * Outputs        : None 
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS, MAC_LOG_CH_ERROR]
 * Description    : The function is used to create a new UE context and add 
 *                  it's entry in the ueDLContextArr_g containig the UE Context 
 *                  pointers. It also increments the global count for the 
 *                  totalActiveUE.
 *                  ueDlConfigInfo_p contains following fields
 *                  - ueIndex -           Index into ueDLContextArr_g
 *                  - ueId -              UE Identifier
 *                  - uePriority -        UE Priority
 *                  - transportFormat_p   Pointer to the Transport Format 
 *                                        structure consisting of the MCSIndex
 *                                        and Modulation Scheme.
 *                  - maxRB -             Maximum number of Resource Blocks 
 *                                        that can be allocated to this UE 
 *                                        per TTI.
 *                                        in case of retransmission.
 *                  - logicalChCount -    Number of logical channels being 
 *                                        currently configured for this UE.
 *                                        different Logical Channel Config-
 ****************************************************************************/

STATIC MacRetType createMACDLUEEntity( UeContextForDownlink* ueDlConfigInfo_p,
		/* CA changes Start */
		InternalCellIndex internalCellIndex)
/* CA changes End */
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	UInt8 index = 0;
	UInt32 ueIndex     = 0;
	/* SPR 15909 fix start */
    tickType_t currentTick = 0;
    /* SPR 15909 fix end */
	DLUEContext *newUEcontext_p  = PNULL;
	currentTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
	ueIndex     = ueDlConfigInfo_p->ueIndex;
	MacRetType  retVal = MAC_SUCCESS; 

	if (  PNULL != dlUECtxInfoArr_g[ueIndex].dlUEContext_p )
	{
		newUEcontext_p = 
			dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

		if(!(newUEcontext_p->tcrntiFlag))
		{

			LOG_MAC_MSG(MAC_UECONTEXT_FAIL_ID,
					LOGWARNING,
					MAC_RRC_INF,
					GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
					ueIndex,
					LINE_NUMBER,
					internalCellIndex,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,
					"MAC_UECONTEXT_EXIST_FLAG_UNSET");

			retVal = MAC_FAILURE; 
			/* SPR 13955 fix start */
			return retVal;    
			/* SPR 13955 fix end */
		}
		/* SPR 2523 and SPR 2516 START */
		else
		{
			/* SPR 3157 changes start */
			newUEcontext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState = 
				UL_SYNC_LOSS_OFF;
			/* SPR 3157 changes end */
		}
		dlUECtxInfoArr_g[ueIndex].dlUEContext_p->rrcUeIndex = ueDlConfigInfo_p->rrcUeIndex;    
		/* SPR 2523 and SPR 2516 END */

	}
	else
	{
		/* Allocating memory for UE Context Info*/
		GET_MEM_FROM_POOL(DLUEContext,newUEcontext_p,sizeof (DLUEContext),PNULL);

		/* Rel 5.3: Coverity Fix Start */
		if (PNULL == newUEcontext_p)
		{
			/* Not able to allocate Memory */
            LOG_MAC_ERROR(MAC_RRC_INF,"Memory Allocation failure for DLUEContext\n");
			return MAC_FAILURE;
		}
		/* Rel 5.3: Coverity Fix End */

		memSet(newUEcontext_p,0,sizeof(DLUEContext));

		/* Coverity 32786 32785 Fix Start  */

		/* Add Ue in global Structure Array */

		dlUECtxInfoArr_g[ueIndex].dlUEContext_p = newUEcontext_p; 
		newUEcontext_p->ulSyncLossOrHOInfoInDL.preambleIdx = INVALID_PREAMBLE_IDX;
		newUEcontext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState = UL_SYNC_LOSS_OFF;

		/* +SPR 8318*/
		newUEcontext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder = FALSE;
		/* -SPR 8318*/

		newUEcontext_p->tcrntiFlag = 0;
		newUEcontext_p->ulSyncLossOrHOInfoInDL.preambleStatus = 
			PREAMBLE_INVALID;

		/* SPR 5051 Fix start */
		newUEcontext_p->rrcUeIndex = ueDlConfigInfo_p->rrcUeIndex;    
		/* CA changes start */
		newUEcontext_p->internalCellIndex = internalCellIndex;
		/* CA changes end */

		/* SPR 5051 Fix end */

		LOG_MAC_MSG(CREATE_DL_UE_CONTXT_ID,
				LOGINFO,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				ueIndex,
				newUEcontext_p->ulSyncLossOrHOInfoInDL.preambleIdx,
				newUEcontext_p->tcrntiFlag,
				internalCellIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,"CREATE_DL_UE_CONTXT_ID");
	}
    /*SPR 21948 Changes Start */
    /* Changes under spr 21762 have been reverted */
    /*SPR 21948 Changes End */
	/* SPR 3608 changes start */
	newUEcontext_p->enbPsrCount = 0;
	/* SPR 3608 changes end */
	/* SPR 6570 changes start */
	newUEcontext_p->isPresentInPriorityQueue = FALSE;
	newUEcontext_p->recvdEnbPsrCount = 0;
	/* SPR 6570 changes end */
    /* + SPS_TDD_Changes */
    memSet(newUEcontext_p->spsDlInfo.ueDrxTTI, 
            DRX_STATE_INVALID, MAX_SPS_INTERVAL );
    /* - SPS_TDD_Changes */
#ifdef UTFWK_SIMULATION 
	/* Initialize DRX and MEAS GAP TTIs for SPS */
	memSet(newUEcontext_p->spsDlInfo.ueDrxTTI, 
			DRX_STATE_INVALID, MAX_SPS_INTERVAL );
	memSet( newUEcontext_p->spsDlInfo.ueMeasGapTTI, 
			MEAS_GAP_OFF, MAX_SPS_INTERVAL );
#endif

	/* SPR 2628 changes End */
	/* PURIFY FIXES 2.4.1 end */

	/* SPR 11199 Fix start */
	DLLogicalChannelInfo *dlLogicalChannel_p = PNULL; 
	DlLcQosInfo          *dlLcQosInfo_p      = PNULL;
	UInt32 tempLogicalChId = 0, lcIndex = 0;

	newUEcontext_p->bandWidthDistribution[PCELL_SERV_CELL_IX] =
		ueDlConfigInfo_p->carrierAggrConfig.bandWidthDistribution[PCELL_SERV_CELL_IX];
	for (lcIndex = 0; lcIndex < ueDlConfigInfo_p->dwlkLogicalChCount; lcIndex++)
	{
		tempLogicalChId = ueDlConfigInfo_p->logicalChConfig[lcIndex].lchId;

		dlLogicalChannel_p = &newUEcontext_p->logicalChannel[tempLogicalChId];
		if (dlLogicalChannel_p)
		{
			dlLcQosInfo_p = &dlLogicalChannel_p->dlLcQosInfo;

			if (PNULL != dlLcQosInfo_p)
			{
				dlLcQosInfo_p->bandWidthDistribution[PCELL_SERV_CELL_IX] =
					ueDlConfigInfo_p->logicalChConfig[lcIndex].bandWidthDistribution[PCELL_SERV_CELL_IX];
			}
		}
	}
	/* SPR 11199 Fix end */
    /* SPR 22075 Changes Start */
    /*SPR 21955 Start */
    /* +- SPR 17777 */
	fillDLCtxtAmbrParams (ueDlConfigInfo_p, newUEcontext_p, 
			internalCellIndex);
    /* +- SPR 17777 */
    /*SPR 21955 End */
    /* SPR 22075 Changes End */

	/* Cyclomatic Complexity changes - starts here */
	if (MAC_SUCCESS != fillDLUeContextParams (ueIndex, ueDlConfigInfo_p, 
				newUEcontext_p, currentTick,internalCellIndex))
	{
		retVal = MAC_FAILURE; 
	}


	/*EICIC Changes Starts*/

	newUEcontext_p->userType = NON_VICTIM_UE;


	if (LTE_TRUE == ueDlConfigInfo_p->userTypePresent)   
	{
		newUEcontext_p->userType = ueDlConfigInfo_p->userType;
	}	
	/*EICIC Changes Ends*/

    /* SPR 22075 Changes Start */
    /* Code moved before filling DL UE Context parameters */
    /* SPR 22075 Changes End */
    {
	fillDLCtxtCqiInfo (ueIndex, ueDlConfigInfo_p, newUEcontext_p, 
			currentTick, internalCellIndex);
	/* Cyclomatic Complexity changes - ends here */

	/*Calculate MCS priority*/
	newUEcontext_p->mcsPriority =
		dlQosGetMcsPriority(newUEcontext_p->dlCQIInfo.mcsIndexCodeWordOne);

	/* Cyclomatic Complexity changes - starts here */
    /* +- SPR 17777 */
	fillDLCtxtSchedParams (ueDlConfigInfo_p, newUEcontext_p, 
			currentTick, internalCellIndex);

	if (MAC_SUCCESS != fillDLCtxtPowerCtrlParams (ueDlConfigInfo_p, 
				newUEcontext_p, internalCellIndex))
        /* +- SPR 17777 */
	{
		retVal = MAC_FAILURE; 
	}
	/* Cyclomatic Complexity changes - ends here */

	/* ICIC changes start */
	/*KLOCKWORK_CHANGES_START_162*/	
	/* SPR 10683 changes start */
	/*
	   addUEPeriodicReportBatchInfo(ueIndex,internalCellIndex);
	 */
	/* SPR 10683 changes End */
	/*KLOCKWORK_CHANGES_STOP_162*/	

	/* Initialize user location type to CC_USER if userLocationType 
	 * is CE type but cellEdgeRegionCount is zero*/
	/* + Coverity 54162 */
	newUEcontext_p->userLocationType = 
		(UserLocation) ueDlConfigInfo_p->userLocationType;
	/* - Coverity 54162 */

	if(ueDlConfigInfo_p->userLocationType == CE_USER)
	{
		++(rrmMacCellGlobal_gp[internalCellIndex]->totalCellEdgeUserCount);
		if (dlResourcePartitionConfig_g[internalCellIndex].cellEdgeRegionCount == 0)
		{
			lteWarning("No cell edge region exist");
			newUEcontext_p->userLocationType = CC_USER;
		}
	}
	/* ICIC changes end */
	/* Cyclomatic Complexity changes - starts here */
	/* SPS_CHG */
	if (MAC_SUCCESS != fillDLCtxtSpsInfo (ueIndex, ueDlConfigInfo_p, 
				newUEcontext_p, internalCellIndex))
	{
		retVal = MAC_FAILURE; 
	}
	/* Cyclomatic Complexity changes - ends here */

	/* CA changes Start */
	macUpdateCreateDLCAParams(newUEcontext_p,
			ueDlConfigInfo_p,internalCellIndex);
	for (index = 0; index < ueDlConfigInfo_p->carrierAggrConfig.scellCount; 
			index++)
	{
		if (MAC_SUCCESS != macUpdateCreateDLScellParams(newUEcontext_p,
					ueDlConfigInfo_p,index,internalCellIndex))
		{
			retVal = MAC_FAILURE; 
		}
	}
	/* CA changes End */
    }// LTE_CELL

	/* Coverity 32786 32785 Fix Start  */
	if(MAC_FAILURE == retVal)
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"deleteCreatedDownlinkUEContext ue %u cell %u\n",
        ueIndex,internalCellIndex);
		deleteCreatedDownlinkUEContext(ueIndex,internalCellIndex);
		return retVal;    
	}

	/* Coverity 32786 32785 Fix End */

	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
	return retVal;    
}

/* Cyclomatic complexity changes - starts here */
/****************************************************************************
 * Function Name  : fillDLUeContextParams 
 * Inputs         : ueIndex - index of ueContext
 *                  ueDlConfigInfo_p - Pointer to UeContextForDownlink
 *                  currentTick - current TTI
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : newUEcontext_p - Pointer to DL UE context 
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS, MAC_LOG_CH_ERROR]
 * Description    : The function is used to fill new DL UE context parameters
 ****************************************************************************/
/* CA Changes start */
STATIC  MacRetType fillDLUeContextParams ( 
		UInt32               ueIndex,
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p,
        /* SPR 15909 fix start */
        tickType_t               currentTick
        /* SPR 15909 fix end */
		,InternalCellIndex internalCellIndex)
/* CA Changes end */
{
	UInt32 n           = 0;
	DLLogicalChannelInfo *dlLogicalChInfo_p     = PNULL;
	DLMIMOInfo  *dlMIMOInfo_p    = PNULL;

	/*Soft Buffer Changes Start*/
	UInt32 totalSoftchannelBytes = 0;
	/*Soft Buffer Changes End*/
	/* + TM7_8 Changes Start */
	UInt8 counter = 0;
	/* - TM7_8 Changes End */
	/* TA Avg change*/
	newUEcontext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
    /*SPR 15208 Fix*/ 
    newUEcontext_p->TA_ScheduleFlag = LTE_FALSE;
    /*SPR 15208 Fix*/ 
	/* TA Avg change*/
	/*HD FDD Changes Start*/
	/* CA Changes start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */
#ifdef HD_FDD_CONFIG
	/* Check whether UE supports HD FDD */
	newUEcontext_p->isHDFddFlag = ueDlConfigInfo_p->isHDFddFlag;
	if(TRUE == newUEcontext_p->isHDFddFlag)
	{
		hdFddCtxConfigReconfigUpdate(ueIndex);
		if(MAC_HD_FDD != cellParams_p->duplexingMode)
		{
			LOG_MAC_MSG(MAC_HD_FDD_CELL_NOT_SUPPORTED,LOGFATAL,MAC_RRC_INF,
					GET_CURRENT_TICK_FOR_CELL(internalCellIndex),ueIndex,
					cellParams_p->duplexingMode,internalCellIndex,
					DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,
					"Cell not supported for HD-FDD");
		}
	}
#endif
	/*HD FDD Changes End */


	/* Logical Channel Structure Initialisation */
	/* Initialising all the Logical Channels with default values */
	for (n = 0; n < MAX_NUMBER_OF_LOGICAL_CHANNEL; n++)
	{
		dlLogicalChInfo_p = &newUEcontext_p->logicalChannel[n]; 

		dlLogicalChInfo_p->logicalChannelId  = INVALID_LCID;
		dlLogicalChInfo_p->lastScheduledTick = currentTick;
		dlLogicalChInfo_p->queueLoad         = 0;
                dlLogicalChInfo_p->rlcMode           = INVALID_RLC_MODE;
		dlLogicalChInfo_p->rlcSNFieldLength  = 0; 
		dlLogicalChInfo_p->logicalChannelPriority = 0;
		/*+COVERITY 5.3.0 - 32250*/
		dlLogicalChInfo_p->dlLcQosInfo.eTokenUpdateFlag = INCREMENT_TOKENS;
		/*-COVERITY 5.3.0 - 32250*/
		/*SPR# 6984 Changes Start */
		dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p = PNULL;
		/*SPR# 6984 Changes End */
		/*SPR# 5343 Changes Starts */
		newUEcontext_p->nonGbrLcInfo[n] = 0;
		/*SPR# 5343 Changes Starts */
	}
	newUEcontext_p->maxLcId = INVALID_LCID;
	/* SPR# 3110 Changes Starts*/
	newUEcontext_p->gbrLcCount = 0;
	newUEcontext_p->nonGbrLcCount = 0;
	newUEcontext_p->scheduledLcCount = 0;
	newUEcontext_p->dlStrategyTxNode_p = NULL;
	listInit(&newUEcontext_p->dlEligibleLcList);
	listInit(&newUEcontext_p->dlScheduledLcList);
	/* SPR# 3110 Changes Ends*/
	newUEcontext_p->ueIndex = ueIndex;
	/* SPR 3607 Fix Start */
	newUEcontext_p->intraEnbHOFlag = FALSE;
	/* SPR 3607 Fix Start */

	/* Coverity 25516 Fix Start*/
	newUEcontext_p->nonGbrSPSLcCount = 0;
	newUEcontext_p->gbrSPSLcCount = 0; 
	/* Coverity 25516 Fix End */

	if ( 0 !=  ueDlConfigInfo_p->dwlkLogicalChCount )
	{
		if(MAC_FAILURE == (addMACDLLogicalChannel(newUEcontext_p,
				ueDlConfigInfo_p->dwlkLogicalChCount,
				ueDlConfigInfo_p->logicalChConfig
				,internalCellIndex)))
        {
            LOG_MAC_ERROR(MAC_RRC_INF,"Add LC Failed\n"); 
		    return MAC_FAILURE;

        }
	}
	newUEcontext_p->dlHarqContext_p = PNULL;
	createMACDLHarqEntity( &newUEcontext_p->dlHarqContext_p,
			ueDlConfigInfo_p->numHarqProcess);

	/* UE_CAPAB_ADD_PARAM Start */
	newUEcontext_p->Rat1Support = ueDlConfigInfo_p->Rat1Support;
	/* UE_CAPAB_ADD_PARAM End */

	/* Initializations */
	/*indicates the C-RNTI assigned to this UE*/
	newUEcontext_p->crnti   = ueDlConfigInfo_p->cRnti;
	/*indicates the priority of this UE*/
	newUEcontext_p->uePriority = ueDlConfigInfo_p->uePriority;
	/*signifies that this max number of resources can be used in 
	  one TTI for this UE*/
	newUEcontext_p->maxNbOfRBs = ueDlConfigInfo_p->maxRB;
	/* Its indicate pdcch is allocated or not. */
	newUEcontext_p->isPdcchAllocatedFlag = FALSE;
	/* This Indicate is the ACK NACK is pending for TB in which DRX has been
	   sent .If true this means we won't sent DRX Again */
	newUEcontext_p->pendingDRXAckNack    = FALSE;
	/* This Indicate is the ACK NACK is pending for TB in which TA has been
	   sent .If true this means we won't sent TA Again */

	/*SPR 4725  Start */
	newUEcontext_p->srbQloadAvailbl = FALSE;
	/*SPR 4725  End */

	/* MIMO related Info */
	dlMIMOInfo_p = &newUEcontext_p->dlMIMOInfo;
	/*SPR 545  Fix starts */
	/* + Coverity 54161 */	
	dlMIMOInfo_p->transmissionMode = (TransmissonMode) ueDlConfigInfo_p->transmissionMode;
	/* - Coverity 54161 */	
	/*SPR 545  Fix ends */

	/* + CQI_5.0 */
	if( (( TX_MODE_3 == dlMIMOInfo_p->transmissionMode) || 
				( TX_MODE_4 == dlMIMOInfo_p->transmissionMode) ||
				/* + TM6_5.2 */
				(TX_MODE_6 == newUEcontext_p->dlMIMOInfo.transmissionMode)) &&
			/* - TM6_5.2 */
			( ! ((2 == cellParams_p->numOfTxAnteenas ) || 
			     (4 == cellParams_p->numOfTxAnteenas )))
	  )	
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"With TransmissionMode %d, number antennas are neither 2 nor 4\n",
				dlMIMOInfo_p->transmissionMode); 
		return MAC_FAILURE;
	}

	/* - CQI_5.0 */
	/* TM6_5.2 */
	newUEcontext_p->tm6DCIFormat = DCI_FORMAT_1A;
	/* TM6_5.2 */

	/* + CL-MIMO LJA*/    
	dlMIMOInfo_p->cbsrInfo = ueDlConfigInfo_p->cbsrInfo;
	/* - CL-MIMO LJA*/        
	dlMIMOInfo_p->precodingIndex   = ueDlConfigInfo_p->precodingIndex;

	/* + TM7_8 Changes Start */
	if(TX_MODE_8 == dlMIMOInfo_p->transmissionMode)
	{
		newUEcontext_p->isPmiRiConfigured_V920 = ueDlConfigInfo_p->pmiRiReportV920;
		if(INVALID_APERIODIC_CQI_MODE != ueDlConfigInfo_p->cqiMode)
		{
			if(MAC_FAILURE == validateAperiodicModeForTXMode8(ueDlConfigInfo_p->cqiMode,
						newUEcontext_p->isPmiRiConfigured_V920))
			{
                LOG_MAC_ERROR(MAC_RRC_INF,"Invalid Aperiodic CQI Mode [%d] for TX Mode 8",
						ueDlConfigInfo_p->cqiMode);
				return MAC_FAILURE;
			}
		}

	}

	newUEcontext_p->pdschEpreToUeRsRatio = ueDlConfigInfo_p->pdschEpreToUeRsRatio;
	newUEcontext_p->beamFormingAlgoToBeUsed = BF_ALGO_NOT_AVAILABLE;

	for(counter = 0; counter < MAX_NUM_ANTENNA ;counter++)
	{
		newUEcontext_p->AoA[counter] = INVALID_VALUE_OF_AOA ;
	}
	/* - TM7_8 Changes End */
	/*+ Dyn UEs To Be SchPerTTI +*/
	newUEcontext_p->isUEConsideredForTotalActiveUeDL = FALSE;
	/*- Dyn UEs To Be SchPerTTI -*/

	/* SPR 903 Fix start*/

	/*Since ueCategory is available during reconfiguration of UE, 
	 * hence assigning minimum value to it*/
	/* SPR 5084 changes starts */
	if(TRUE == ueDlConfigInfo_p->ueCategoryReqType)
	{
		newUEcontext_p->ueCategory = ueDlConfigInfo_p->ueCategory;
	}
	else
	{
		newUEcontext_p->ueCategory  = MAC_MIN_UE_CATEGORY ;
	}
	/* CA Changes Start */
	/* SPR 12246 Start */
	if (TRUE == ueDlConfigInfo_p->ueCategoryV1020ReqType)
	{        
		if ( macValidateUeCategoryV1020(ueDlConfigInfo_p->ueCategoryV1020, internalCellIndex) )
		{
			newUEcontext_p->ueCategoryV1020 = ueDlConfigInfo_p->ueCategoryV1020;
		}
		else
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"macValidateUeCategoryV1020 failed ue %u\n",newUEcontext_p->ueIndex);
			return MAC_FAILURE;
		}        
	}        
	else
	{
		newUEcontext_p->ueCategoryV1020 = newUEcontext_p->ueCategory;
	}

	/* SPR 12246 End */
	/* CA Changes End */
	/* SPR 5084 changes ends */

	/*debug stats*/
	LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UE_CATEGORY(
			newUEcontext_p->ueIndex,
			newUEcontext_p->ueCategory);
	/* SPR 903 Fix start*/

    {
        /*Update the max TB size for the category of the UE  */
        newUEcontext_p->maxTBSize = 
            DIVIDE_BY_EIGHT(cellParams_p->dlUeCategoryInfo[newUEcontext_p->ueCategory - 1].
                    maxBitsDLSCHTb);

        newUEcontext_p->maxSISOTBSize = 
            DIVIDE_BY_EIGHT(cellParams_p->dlUeCategoryInfo[newUEcontext_p->ueCategory - 1].
                    maxBitsDLSCHTb);

        newUEcontext_p->maxMIMOTBSize = 
            DIVIDE_BY_EIGHT(cellParams_p->dlUeCategoryInfo[newUEcontext_p->ueCategory - 1].
                    maxDLSCHTbBits);
        /*Soft Buffer Changes Start*/
        totalSoftchannelBytes = DIVIDE_BY_EIGHT(\
                cellParams_p->dlUeCategoryInfo[newUEcontext_p->ueCategory - 1].totSoftChBits);
        newUEcontext_p->permissibleTBSize = (totalSoftchannelBytes)/MAC_CODING_FACTOR;
        newUEcontext_p->maxPermissibleTBSize = newUEcontext_p->permissibleTBSize;
        /*Soft Buffer Changes End*/
    }
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    if (mocnOperatorParams_g.isMocnEnabled)
    {
        newUEcontext_p->operatorId = ueDlConfigInfo_p->operatorId;
    }
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : fillDLCtxtAmbrParams 
 * Inputs         : ueIndex - index of ueContext
 *                  ueDlConfigInfo_p - Pointer to UeContextForDownlink
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : newUEcontext_p - Pointer to DL UE context 
 * Returns        : None
 * Description    : The function is used to fill the DL AMBR parameters in 
 *                  DL UE context.
 ****************************************************************************/
/* CA Changes start */
STATIC  void fillDLCtxtAmbrParams ( 
        /* +- SPR 17777 */
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p
        /* +- SPR 17777 */
		,InternalCellIndex internalCellIndex)
{
	InitParams *initParams_p = cellSpecificParams_g.\
				   cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
	/*Fill AMBR*/
	if(ueDlConfigInfo_p->dlAmbrPresent == TRUE)
	{
		/*debug stats*/
		LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_AMBR(
				newUEcontext_p->ueIndex,ueDlConfigInfo_p->dlAmbr);

		newUEcontext_p->dlAmbrInfo.AMBR = ueDlConfigInfo_p->dlAmbr;
		/* Calculate DL QOS Ambr Token rate */
		dlQosCalcAmbrTknRate(&newUEcontext_p->dlAmbrInfo,
				initParams_p->dlSchdConfig.dlTokenPeriodicity);

		newUEcontext_p->dlAmbrInfo.totSchdTknInWindow = 0; 
		newUEcontext_p->dlAmbrInfo.curScheduledTkn    = 0; 
		newUEcontext_p->dlAmbrInfo.curTknWindowInd    = 0;
		/*+COVERITY 5.3.0 - 32251*/
		newUEcontext_p->dlAmbrInfo.eTokenUpdateFlag = INCREMENT_TOKENS;
		/*-COVERITY 5.3.0 - 32251*/

		newUEcontext_p->dlAmbrInfo.availAmbrToken = 
			newUEcontext_p->dlAmbrInfo.ambrTokenRate;

		/*Calculate AMBR bucket depth*/
        /* SPR 22075 Changes Start */
        /*SPR 21955 Start */
		newUEcontext_p->dlAmbrInfo.ambrTokenDepth = 
			newUEcontext_p->dlAmbrInfo.ambrTokenRate * initParams_p->dlSchdConfig.ambrBktFactor;
        /*SPR 21955 End */
        /* SPR 22075 Changes End */
		/* DESIGN REVIEW CHANGES INCORPORATION */
		/* Calculate bucket limit(minimum tokens to present in bucket) */
		newUEcontext_p->dlAmbrInfo.ambrTokenLimit = 
			initParams_p-> dlSchdConfig.ambrTokenLimitFactor * 
			newUEcontext_p->dlAmbrInfo.ambrTokenRate;

		/*Update the Stkn(t) with the token rate*/
		newUEcontext_p->dlAmbrInfo.scheduledToken[0] = 0;

		/*Update the lcTknTable in dlAmbrInfo*/
		dlQosUpdateTknTableForLcPriority(
				newUEcontext_p->dlAmbrInfo.lcTknTable,
				newUEcontext_p->dlAmbrInfo.AMBR,
				initParams_p->dlSchdConfig.dlTokenPeriodicity );

		/* CA Changes end */
		newUEcontext_p->dlAmbrInfo.curTknWindowInd = 0;

		newUEcontext_p->dlAmbrInfo.ambrTokenPriority = 5;

		/* calculate ambr token priority */
		/*   newUEcontext_p->dlAmbrInfo.ambrTokenPriority = 
		     dlQosGetTknPriority(&newUEcontext_p->dlAmbrInfo.scheduledToken, 0,
		     &newUEcontext_p->dlAmbrInfo.lcTknTable);
		 */
	}
}

/****************************************************************************
 * Function Name  : fillDLCtxtCqiInfo 
 * Inputs         : ueIndex - index of ueContext
 *                  ueDlConfigInfo_p - Pointer to UeContextForDownlink
 *                  currentTick - current TTI
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : newUEcontext_p - Pointer to DL UE context 
 * Returns        : None
 * Description    : The function is used to fill the DL CQI Info in 
 *                  DL UE context.
 ****************************************************************************/
STATIC  void fillDLCtxtCqiInfo ( 
		UInt32               ueIndex,
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p,
		/* SPR 15909 fix start */
        tickType_t               currentTick,
        /* SPR 15909 fix end */
		InternalCellIndex internalCellIndex)
{
	DLCQIInfo   *dlCQIInfo_p     = PNULL;
	/* + CQI_4.1 */
	UInt8 cqiValue = 0;

	dlCQIInfo_p = &newUEcontext_p->dlCQIInfo;
	dlCQIInfo_p->isSubBandReportAvailable  = FALSE;
	dlCQIInfo_p->isWideBandReportAvailable = FALSE;
	dlCQIInfo_p->modulationSchemeCodeWordOne = 
		dlCQIInfo_p->modulationSchemeCodeWordTwo = ueDlConfigInfo_p->modScheme;
	dlCQIInfo_p->currentDLBLERForCW0 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->currentDLBLERForCW1 = TARGET_DL_BLER_VALUE;
	/* SPR 13571 fix start */
	dlCQIInfo_p->prevDlBlerCalculatedForCW0 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->prevDlBlerCalculatedForCW1 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->prevDlBlerCalculatedForCW0_2 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->prevDlBlerCalculatedForCW1_2 = TARGET_DL_BLER_VALUE;
	/* SPR 13571 fix end */

	dlCQIInfo_p->trigger1r10 = ueDlConfigInfo_p->trigger1r10;
	dlCQIInfo_p->trigger2r10 = ueDlConfigInfo_p->trigger2r10;

	updateDLBlerCW0(newUEcontext_p->ueIndex,TARGET_DL_BLER_VALUE, internalCellIndex);
	updateDLBlerCW1(newUEcontext_p->ueIndex,TARGET_DL_BLER_VALUE, internalCellIndex);
	updateDLBlerCW0_2(newUEcontext_p->ueIndex,TARGET_DL_BLER_VALUE, internalCellIndex);
	updateDLBlerCW1_2(newUEcontext_p->ueIndex,TARGET_DL_BLER_VALUE, internalCellIndex);
	dlCQIInfo_p->previousDLBLERForCW0 = 0;
	dlCQIInfo_p->previousDLBLERForCW1 = 0;
	dlCQIInfo_p->dlBlerCalculatedCounterForCW0 = 0;
	dlCQIInfo_p->dlBlerCalculatedCounterForCW1 = 0;
	/*EICIC*/
	dlCQIInfo_p->currentDLBLERForCW0_2 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->currentDLBLERForCW1_2 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->previousDLBLERForCW0_2 = 0;
	dlCQIInfo_p->previousDLBLERForCW1_2 = 0;
	dlCQIInfo_p->dlBlerCalculatedCounterForCW0_2 = 0;
	dlCQIInfo_p->dlBlerCalculatedCounterForCW1_2 = 0;
	/*EICIC*/
	dlCQIInfo_p->aperiodicCqiRequestTTI = currentTick;
	dlCQIInfo_p->aperiodicCQIRequested     = FALSE;
	/* + CQI_5.0 */ 
	dlCQIInfo_p->subBandReportInfo_p = PNULL;
	dlCQIInfo_p->riUpdationFlag = IMMEDIATE_DL_RI_UPDATE;
	dlCQIInfo_p->wideBandPMIIndex = newUEcontext_p->dlMIMOInfo.precodingIndex;
	newUEcontext_p->actualPMIValueForDLSCH = FALSE;
	dlCQIInfo_p->reportReceivedTTI = 0;
	newUEcontext_p->isSubbandPMIAvailable = FALSE;
	newUEcontext_p->aperiodicMode12Rcvd = FALSE;
	dlCQIInfo_p->mcsIndexCodeWordOne = dlCQIInfo_p->mcsIndexCodeWordTwo =
		ueDlConfigInfo_p->mcsIndex;
	dlCQIInfo_p->cqiCorrectionFactorForCW0 = 0;
	dlCQIInfo_p->cqiCorrectionFactorForCW1 = 0;

	dlCQIInfo_p->cqiTypeIndicator = INVALID_CQI_REPORTING_MODE; 
	dlCQIInfo_p->latestAperiodicReportRcvd = TRUE;
	/* + SPR 5953 Fix */
	dlCQIInfo_p->cqiRequestType = INVALID_PERIODIC_CQI_REQ_TYPE;
	/* - SPR 5953 Fix */
	/*SPR 10634 changes Start*/
	cqiValue = getCQIFromMCS(dlCQIInfo_p->mcsIndexCodeWordOne);
	dlCQIInfo_p->widebandCQICodeWordOne =
		dlCQIInfo_p->widebandCQICodeWordTwo =
		dlCQIInfo_p->previousTimeAveragedWBCQICW0 =
		dlCQIInfo_p->previousTimeAveragedWBCQICW1 =
		dlCQIInfo_p->initialCqiValue = cqiValue;
	dlCQIInfo_p->cqiMode = CQI_MODE_INVALID;
	/*SPR 10634 changes end*/
	if (ueDlConfigInfo_p->cqiInfoFlag)
	{
		/* CQI related Info */
		dlCQIInfo_p->cqiTypeIndicator = ueDlConfigInfo_p->cqiIndication;
		dlCQIInfo_p->cqiMode = ueDlConfigInfo_p->cqiMode;
		/* + SPR 5953 Fix */
		dlCQIInfo_p->cqiRequestType = ueDlConfigInfo_p->cqiRequestType;
		/* - SPR 5953 Fix */
		/* + CQI_4.1 */
		cqiValue = getCQIFromMCS(dlCQIInfo_p->mcsIndexCodeWordOne);

		dlCQIInfo_p->widebandCQICodeWordOne = 
			dlCQIInfo_p->widebandCQICodeWordTwo = 
			dlCQIInfo_p->initialCqiValue = cqiValue;
		/* - CQI_4.1 */

		/* + CQI_5.0 */
		GET_MEM_FROM_POOL(SubBandReportInfo,dlCQIInfo_p->subBandReportInfo_p,
				sizeof(SubBandReportInfo),PNULL);
		/*+COVERITY 5.3.0 - 32729*/
		if(PNULL == dlCQIInfo_p->subBandReportInfo_p)
		{
			ltePanic("Memory Allocation failed in %s\n",__func__);
			return;
		}
		/*+COVERITY 5.3.0 - 32729*/

		/* + Coverity_Stack Fix */
		memSet(dlCQIInfo_p->subBandReportInfo_p,0,sizeof(SubBandReportInfo));
		/* - Coverity_Stack Fix */

		/* - CQI_5.0 */
		/* ICIC changes start */
		updateDLSINRCW0(ueIndex,dlCQIInfo_p->widebandCQICodeWordOne, internalCellIndex);
		updateDLSINRCW1(ueIndex,dlCQIInfo_p->widebandCQICodeWordTwo, internalCellIndex);
		/* ICIC changes end */
		/* SPR 9427 changes start */
		UInt8 counter =  MAX_SUBBANDS;
		if (newUEcontext_p->dlMIMOInfo.transmissionMode == TX_MODE_3 ||
				newUEcontext_p->dlMIMOInfo.transmissionMode == TX_MODE_4)
		{
			while(counter--)
			{    
				dlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].cqiIndexCW1 =  
					dlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].cqiIndexCW2 = cqiValue;

				dlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].mcsIndexCW1 =
					dlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].mcsIndexCW2 = 
					dlCQIInfo_p->mcsIndexCodeWordOne;

				dlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].previousTimeAveragedSBCQICW0 =  
					dlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].previousTimeAveragedSBCQICW1 = 
					cqiValue;
			}

			initSpatialRAT0RAT1Bitmaps(cqiValue,
					&dlCQIInfo_p->subBandReportInfo_p->
					cqiInfo.cqiInfoSpatialMul,
					internalCellIndex);

		}
		else if (newUEcontext_p->dlMIMOInfo.transmissionMode == TX_MODE_1 ||
				newUEcontext_p->dlMIMOInfo.transmissionMode == TX_MODE_2)
		{   

			while(counter--)
			{    
				dlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].cqiIndexCW1 = cqiValue; 

				dlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].mcsIndexCW1 =
					dlCQIInfo_p->mcsIndexCodeWordOne;

				dlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].previousTimeAveragedSBCQICW0 = 
					cqiValue;  
			}

			initNonSpatialRAT0RAT1Bitmaps(cqiValue,
					&dlCQIInfo_p->subBandReportInfo_p->
					cqiInfo.cqiInfoNonSpatialMul,
					internalCellIndex);

		}
		initNumOfSBperBWpart(dlCQIInfo_p,internalCellIndex);
		/* SPR 9427 changes end */
	}
	/* - CQI_4.1 */ 
}

/****************************************************************************
 * Function Name  : fillDLCtxtSchedParams 
 * Inputs         : ueIndex - index of ueContext
 *                  ueDlConfigInfo_p - Pointer to UeContextForDownlink
 *                  currentTick - current TTI
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : newUEcontext_p - Pointer to DL UE context 
 * Returns        : None
 * Description    : The function is used to fill the DL Scheduler parameters 
 *                  in DL UE context.
 ****************************************************************************/
STATIC  void fillDLCtxtSchedParams ( 
        /* +- SPR 17777 */
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p,
		/* SPR 15909 fix start */
        tickType_t               currentTick,
        /* SPR 15909 fix end */
		/*CA Changes start */
		InternalCellIndex internalCellIndex)
/*CA Changes end */
{
	DLSchedulerAlgoInfo  *dLSchedulerAlgoInfo_p = PNULL;
	UInt32 *assignedRBQueueLoad_p = PNULL; 
#ifdef TDD_CONFIG
	UInt8 sfIndex = 0;
	UInt8 containerIndex = 0;
#endif 
	/* Scheduler related Info */
	dLSchedulerAlgoInfo_p = &newUEcontext_p->dlSchedulerAlgoInfo;

	dLSchedulerAlgoInfo_p->modulationSchemeFactor = 0;
	dLSchedulerAlgoInfo_p->uePriorityFactor       = 0;


	newUEcontext_p->lastScheduledTick       = currentTick;
    /* +- SPR 16738 */
	newUEcontext_p->drxFlag                 = FALSE;
	newUEcontext_p->timingAdvanceValue      = INVALID_TA_VALUE;
	/* ICIC changes start */
	/*CA Changes start  */
	updateDLTA(newUEcontext_p->ueIndex,INVALID_TA_VALUE, internalCellIndex);
	/*CA Changes end  */
	newUEcontext_p->isAlreadyInCEPriorityList = FALSE;
	/* ICIC changes end*/
	newUEcontext_p->isAlreadyConsideredFlag = INVALID_ALREADY_CONSIDERED_FLAG;
	newUEcontext_p->dlUENonZeroQueueLoadAndHarqList_p = PNULL;
	/* Initialize Schedule Flag for DL QOS */
	newUEcontext_p->scheduleFlag = DL_QOS_NEW_TRANSMISSION;

	assignedRBQueueLoad_p = &newUEcontext_p->assignedRBQueueLoad[0];

	*assignedRBQueueLoad_p = 0;
	/* COVERITY CID 10173 FIX START */
	/*
	 *(++assignedRBQueueLoad_p) = 0;
	 *(++assignedRBQueueLoad_p) = 0;
	 *(++assignedRBQueueLoad_p) = 0;
	 */
	/* COVERITY CID 10173 FIX END */
	newUEcontext_p->assignedRBQueueLoadSchIndex = 0;
	newUEcontext_p->assignedRBQueueLoadRLCIndex =0;
	newUEcontext_p->effectiveBitRate = 0;
	newUEcontext_p->ueQueueLoad = 0;

	/*SPR_3061_FIX*/ 
	newUEcontext_p->timeAlignmentTimer = ueDlConfigInfo_p->timeAlignmentTimer;
	/* + SPR_3939 */
	newUEcontext_p->syncTimerDelta = newUEcontext_p->timeAlignmentTimer - 
		UL_SYNC_TIMER_DIFF_THRESHOLD_FOR_SENDING_TA;
	newUEcontext_p->lastTATriggeredTick = currentTick;
	/* - SPR_3939 */
	newUEcontext_p->lastSyncDetectedTick = currentTick;
	/*SPR_3061_FIX*/ 
	/*Added for Power Control*/
	newUEcontext_p->pA = ueDlConfigInfo_p->pA;
	/*Fix for __SPR_1988__ END*/
	/*SPR_7217_changes_start*/
#ifdef TDD_CONFIG
	/* + CA_TDD_HARQ_CHNAGES */      
	for(sfIndex =0; sfIndex < MAX_SUB_FRAME; sfIndex++)
	{
		for (containerIndex = 0; containerIndex < MAX_DL_TIMER_CONTAINER; containerIndex++)
		{
			newUEcontext_p->harqProcessId[containerIndex][sfIndex] = INVALID_HARQ_ID;
			/* Its indicate pdcch is allocated or not. */
			newUEcontext_p->tddHarqTimerExpiryNode[containerIndex][sfIndex] = PNULL;
			newUEcontext_p->pdschTxMapContainer[containerIndex][sfIndex] = 0;
		}
		newUEcontext_p->tddDai[sfIndex] = 0;
		/* + SPS_TDD_Changes */
		newUEcontext_p->tddDaiSPS[sfIndex] = 0;
		/* - SPS_TDD_Changes */
		newUEcontext_p->tddDaiLastScheduled[sfIndex] = 0;
		newUEcontext_p->retxQueueNode_p[sfIndex] = PNULL;
	}

	SET_HARQ_RETX_NODE(newUEcontext_p->tempRetxNode);
	newUEcontext_p->nackFlag[PCELL_SERV_CELL_IX] = FALSE;
	newUEcontext_p->nackFlag[START_SCELL_INDEX] = FALSE;
	/* - CA_TDD_HARQ_CHNAGES */      
#endif
	/* CLPC CHANGES */
	newUEcontext_p->averageSinrForPucch  = 
		/*CA Changes start  */
		(macReconfigSchedulerParamsDL_gp[internalCellIndex] +\
		 schParamsIndexDL_g[internalCellIndex])->\
		deltaSinrToTPCMapForPUCCH.pucchSINRTarget; 
	/*CA Changes end  */
	newUEcontext_p->lastSINRCoolOffCounter = 0;
	/* SPR 3271 changes start */
	/* Initialize blerInfoForPUCCH */
	/*CA Changes start  */
	initPucchBlerContext(newUEcontext_p, internalCellIndex);
	/* SPR 3271 changes end */
	pucchBlerUnsureWeightage_g[internalCellIndex] = (100 -\
			(macReconfigSchedulerParamsDL_gp[internalCellIndex] +\
			 schParamsIndexDL_g[internalCellIndex])->\
			deltaBlerToTPCMapForPUCCH.pucchBLERTarget);
	pucchBlerSureWeightage_g[internalCellIndex] = 
		(macReconfigSchedulerParamsDL_gp[internalCellIndex] + \
		 schParamsIndexDL_g[internalCellIndex])->\
		deltaBlerToTPCMapForPUCCH.pucchBLERTarget;
	/*CA Changes end  */

}

/****************************************************************************
 * Function Name  : fillDLCtxtPowerCtrlParams 
 * Inputs         : ueIndex - index of ueContext
 *                  ueDlConfigInfo_p - Pointer to UeContextForDownlink
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : newUEcontext_p - Pointer to DL UE context 
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : The function is used to fill the DL Power Control  
 *                  parameters in DL UE context.
 ****************************************************************************/
STATIC  MacRetType fillDLCtxtPowerCtrlParams (
        /* +- SPR 17777 */
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p,
		/* CA changes Start */
		InternalCellIndex internalCellIndex
		/* CA Changes end */
		)
{
	if( !ueDlConfigInfo_p->tpcPdcchConfigPucch.release )
	{
		if (MAC_SUCCESS != configureTpcPdcchInfoForPucch(
					ueDlConfigInfo_p->tpcPdcchConfigPucch.tpcRnti,
					ueDlConfigInfo_p->tpcPdcchConfigPucch.tpcBitMapIndex,
					ueDlConfigInfo_p->tpcPdcchConfigPucch.isConfiguredForDci3Or3A,
					newUEcontext_p,internalCellIndex))
		{
			return MAC_FAILURE;
		}
	}
	/* SPR 3271 changes start */
	else
	{
		newUEcontext_p->tpcInfoForPucch.tpcRnti = INVALID_TPC_RNTI;
		newUEcontext_p->tpcInfoForPucch.lastAppliedTick = 
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
		newUEcontext_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p = PNULL; 
		newUEcontext_p->tpcInfoForPucch.tpcTriggeredValue = TPC_NO_CHANGE;
	}    
	/* SPR 3271 changes end */

	/* DCI Format 3A changes start*/
	/* Reseting the fakeTpcValue*/
	newUEcontext_p->tpcInfoForPucch.fakeTpcValue = FAKE_TPC_NO_VALUE;
	/* DCI Format 3A changes start*/ 

	newUEcontext_p->currPowerCorrectionForPucch = 0;
	newUEcontext_p->powerControlPucchTpcValue = 0;
	/* SPR 5147 Fix Start */ 
	newUEcontext_p->prioritySchedulingReqDL = FALSE; 
	/* SPR 5147 Fix End */ 
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : fillDLCtxtSpsInfo 
 * Inputs         : ueIndex - index of ueContext
 *                  ueDlConfigInfo_p - Pointer to UeContextForDownlink
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : newUEcontext_p - Pointer to DL UE context 
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : The function is used to fill the DL SPS information 
 *                  in DL UE context.
 ****************************************************************************/
STATIC  MacRetType fillDLCtxtSpsInfo (
		UInt32               ueIndex,
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p
		/* CA changes Start */
		, InternalCellIndex internalCellIndex
		/* CA Changes end */
		)
{
	UInt8 index = 0;
	LP_SpsDlInfo  spsDlInfo_p  = PNULL;
	RNTIInfo *rntiInfo_p        = PNULL;

	/* Initilaize schType to DRA_SCHEDULING by default */
	newUEcontext_p->schType = DRA_SCHEDULING;
	/*SPR 7104 SPS Fix start*/
	newUEcontext_p->spsDlInfo.spsDlSetupInfo_p = PNULL;
	newUEcontext_p->spsDlInfo.sdlOccNode_p     = PNULL;
	newUEcontext_p->spsDlInfo.spsState         = INVALID_STATE;
	newUEcontext_p->spsDlInfo.requestType      = SPS_RELEASE;
	newUEcontext_p->spsDlInfo.SpsCrnti         = INVALID_SPS_CRNTI;
	/*SPR 7104 SPS Fix end*/
	/* Assiging the SPS Config parms to the Ue COntext */
	if (TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
			->initParams_p->spsSysWideParam.enableSps)
	{
		spsDlInfo_p = &newUEcontext_p->spsDlInfo;
		spsDlInfo_p->requestType = 
			ueDlConfigInfo_p->spsDownlinkInfo.requestType;
		/* Initializing SPS STATE to invalid state */
		spsDlInfo_p->spsState = INVALID_STATE;
                    /* SPR 20456 start */
                    /* code removed */
                    /*SPR 20456 end */
		if ( spsDlInfo_p->requestType == SPS_SETUP)
		{
                    /* SPR 20456 start */
                    /* code removed */
                    /*SPR 20456 end */
				if( (ulUECtxInfoArr_g[ueIndex].ulUEContext_p != PNULL) &&
						(SPS_SETUP == ulUECtxInfoArr_g[ueIndex].ulUEContext_p->spsUlInfo.requestType))
				{
					spsDlInfo_p->SpsCrnti = 
						ulUECtxInfoArr_g[ueIndex].ulUEContext_p->spsUlInfo.SpsCrnti;
					/*CA Changes start  */
					rntiInfo_p = getUeIdxFromRNTIMap(
							newUEcontext_p->spsDlInfo.SpsCrnti, internalCellIndex);
					/*CA Changes end  */
					if (PNULL == rntiInfo_p)
					{
						LOG_MAC_MSG( MAC_UE_ID_RNTI_MAP, LOGWARNING, MAC_RRC_INF,
								GET_CURRENT_TICK_FOR_CELL(internalCellIndex), __LINE__, 
								spsDlInfo_p->SpsCrnti, 
								newUEcontext_p->crnti,
								newUEcontext_p->ueIndex, internalCellIndex, 0,0, __func__, "" );
						return MAC_FAILURE;
					}
					rntiInfo_p->rntiFlag = SPS_RNTI;
					rntiInfo_p->index    = newUEcontext_p->ueIndex;
					/*pdcch changes start*/
					newUEcontext_p->spsUeSearchSpaceInfo_p = 
						&(rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex][0]);
					/*pdcch changes end*/
				}
				else
				{
					UInt16  spsCrnti = allocateSpsCrnti(internalCellIndex);
					if( MAC_FAILURE == spsCrnti )
					{
                        LOG_MAC_ERROR(MAC_RRC_INF,"Invalid spsCrnti for UE %u\n",ueIndex);
						return MAC_FAILURE;
					}
					else
					{
						spsDlInfo_p->SpsCrnti = spsCrnti;
						/* Assigning sps crnti in UL context */
						if( ulUECtxInfoArr_g[ueIndex].ulUEContext_p != PNULL)
						{
							ulUECtxInfoArr_g[ueIndex].ulUEContext_p->spsUlInfo.SpsCrnti = spsCrnti;
						}
						/*CA Changes start  */
						rntiInfo_p = getUeIdxFromRNTIMap(
								newUEcontext_p->spsDlInfo.SpsCrnti, internalCellIndex);
						/*CA Changes end  */
						if (PNULL == rntiInfo_p)
						{

							LOG_MAC_MSG( MAC_UE_ID_RNTI_MAP, LOGWARNING, MAC_RRC_INF,
									GET_CURRENT_TICK_FOR_CELL(internalCellIndex), __LINE__, spsCrnti, 
									newUEcontext_p->crnti,
									newUEcontext_p->ueIndex, internalCellIndex, 0,0, __func__, "" );
							return MAC_FAILURE;
						}
						rntiInfo_p->rntiFlag = SPS_RNTI;
						rntiInfo_p->index    = newUEcontext_p->ueIndex;
						newUEcontext_p->spsUeSearchSpaceInfo_p = 
							(&rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex][0]);
					}
				}

				spsDlInfo_p->spsDlSetupInfo_p = PNULL;
				GET_MEM_FROM_POOL(SpsDlSetupInfo,
						spsDlInfo_p->spsDlSetupInfo_p,
						sizeof (SpsDlSetupInfo),PNULL);
				if (spsDlInfo_p->spsDlSetupInfo_p == PNULL)
				{
                    LOG_MAC_ERROR(MAC_RRC_INF,"Memory Allocation failure for spsDlSetupInfo_p for UE %u\n",ueIndex);
					return MAC_FAILURE;
				}
				else
				{
					/* Initializing the Optional parameter 
					 * N1PucchAnPersistent in UE Context 
					 */
					for (index = 0; index < MAX_N1_PUCCH_AN_PERSISTENT; index++)
					{
						spsDlInfo_p->spsDlSetupInfo_p->
							N1PucchAnPersistentListVal.N1PucchAnPersistent[index] = 0;
					}

					spsDlInfo_p->spsDlSetupInfo_p->semiPersistentSchedIntervalDL = 
						ueDlConfigInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.
						semiPersistentSchedIntervalDL;
					spsDlInfo_p->spsDlSetupInfo_p->numberOfConfSpsProcesses = 
						ueDlConfigInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.
						numberOfConfSpsProcesses;

					/* If maxSpsHarqRetx not received then 
					 * intitialize with default value
					 */
					if (TRUE == ueDlConfigInfo_p->spsDownlinkInfo.
							spsDownlinkSetupInfo.isMaxSpsHarqRetxPresent)
					{
						spsDlInfo_p->spsDlSetupInfo_p->maxSpsHarqRetx = 
							ueDlConfigInfo_p->spsDownlinkInfo.
							spsDownlinkSetupInfo.maxSpsHarqRetx;
					}
					else
					{
						spsDlInfo_p->spsDlSetupInfo_p->maxSpsHarqRetx = 
							MAX_SPS_HARQ_RETX_DEFAULT_VALUE;
					}

					/* If explicitReleaseAfter not received then 
					 * intitialize with default value
					 */
					if (TRUE == ueDlConfigInfo_p->spsDownlinkInfo.
							spsDownlinkSetupInfo.isExplicitReleaseAfterPresent)
					{
						spsDlInfo_p->spsDlSetupInfo_p->explicitReleaseAfter =
							ueDlConfigInfo_p->spsDownlinkInfo.
							spsDownlinkSetupInfo.explicitReleaseAfter;
					}
					else
					{
						spsDlInfo_p->spsDlSetupInfo_p->explicitReleaseAfter = 
							EXPLICIT_RELEASE_AFTER_DEFAULT_VALUE;
					}

					spsDlInfo_p->spsDlSetupInfo_p->N1PucchAnPersistentListVal.
						numOfN1PucchAnPersistent = 
						ueDlConfigInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.
						N1PucchAnPersistentListVal.numOfN1PucchAnPersistent;
					index = 0;
					while(index < spsDlInfo_p->spsDlSetupInfo_p->
							N1PucchAnPersistentListVal.numOfN1PucchAnPersistent)
					{ 
						spsDlInfo_p->spsDlSetupInfo_p->
							N1PucchAnPersistentListVal.N1PucchAnPersistent[index] = 
							ueDlConfigInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.
							N1PucchAnPersistentListVal.N1PucchAnPersistent[index];
						index++;
					}
				
			}
		}
		LTE_MAC_UPDATE_DOWNLINK_SPS_UE_EXIST_STATE(newUEcontext_p->ueIndex,
				LTE_TRUE)
#ifdef SSI_DEBUG_STATS
			DLLogicalChannelInfo *spsLchInfo_p = PNULL;    
		if (INVALID_LCID !=  newUEcontext_p->spsDlInfo.lcId)
		{
			spsLchInfo_p = &(newUEcontext_p->logicalChannel[
					newUEcontext_p->spsDlInfo.lcId]);
		}
		LTE_MAC_UPDATE_DOWNLINK_SPS_UE_STATS(newUEcontext_p->ueIndex,
				spsDlInfo_p, spsLchInfo_p)
#endif
	}
	else
	{
		newUEcontext_p->spsDlInfo.spsDlSetupInfo_p = PNULL;
		newUEcontext_p->spsDlInfo.sdlOccNode_p     = PNULL;
		newUEcontext_p->spsDlInfo.spsState         = INVALID_STATE;
		newUEcontext_p->spsDlInfo.requestType      = SPS_RELEASE;
		newUEcontext_p->spsDlInfo.SpsCrnti         = INVALID_SPS_CRNTI;
	}
	return MAC_SUCCESS;
}
/* Cyclomatic complexity changes - ends here */

/****************************************************************************
 * Function Name  : createMACDLUEConnHandOverEntity
 * Inputs         : ueIndex - index of ueContext
 *                  preambleIndex - PreambleIndex to be stored in UE Context
 *                  tcRNTI_p - Pointer to the TCRNTI
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                                      
 * Outputs        : Allocated TCRNTI is strored in 
 *                  Pointer to the TCRNTI
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS, MAC_LOG_CH_ERROR]
 * Description    : The function is used to create a new UE context for 
 *                  connection Hand Over
 ****************************************************************************/
MacRetType createMACDLUEConnHandOverEntity(UInt8 preambleIndex,
		UInt16 ueIndex,
		UInt16 *tcRNTI_p
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA changes End */

		)
{
	DLUEContext *newUEcontext_p  = PNULL;
	UInt16 tcRNTI           = 0;

	if (  PNULL != dlUECtxInfoArr_g[ueIndex].dlUEContext_p )
	{
		LOG_MAC_MSG(MAC_UECONTEXT_FAIL_ID,
				LOGWARNING,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				ueIndex,
				LINE_NUMBER,
				internalCellIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,
				"MAC_UECONTEXT_EXIST");


		return MAC_FAILURE;
	}
    /*SPR 16855 +-*/
	tcRNTI = *tcRNTI_p;
    /*SPR 16855 +-*/
	if (INVALID_RNTI != tcRNTI)
	{
		/* Allocating memory for UE Context Info*/
		GET_MEM_FROM_POOL(DLUEContext,newUEcontext_p,sizeof (DLUEContext),PNULL);

		/* Rel 5.3: Coverity Fix Start */
		if (PNULL == newUEcontext_p)
		{
			/* Not able to allocate Memory */
			return MAC_FAILURE;
		}
		/* Rel 5.3: Coverity Fix End */
		/** SPR 11011 Changes Start **/
		memSet(newUEcontext_p,0,sizeof (DLUEContext));
		/** SPR 11011 Changes End **/

		if(INVALID_PREAMBLE_IDX != preambleIndex)
		{    
			if(PREAMBLE_BUSY != getPreambleStatusInfo(preambleIndex, internalCellIndex))
			{
				LOG_MAC_MSG(PREAMBLE_NOT_BUSY_ID,
						LOGERROR,
						MAC_RRC_INF,
						GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
						ueIndex,
						tcRNTI,
						preambleIndex,
						LINE_NUMBER,
						internalCellIndex,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME,"PREAMBLE_NOT_BUSY_ID");

				/*Coverity 25490 Fix Start*/
				freeMemPool(newUEcontext_p);
				/*Coverity 25490 Fix End*/

				return MAC_FAILURE;
			}
			newUEcontext_p->ulSyncLossOrHOInfoInDL.preambleStatus = 
				PREAMBLE_BUSY;
		}
		else
		{
			LOG_MAC_MSG(INVALID_PREAMBLE_INDEX_ID,
					LOGINFO,
					MAC_RRC_INF,
					GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
					preambleIndex,
					ueIndex,
					tcRNTI,
					LINE_NUMBER,
					internalCellIndex,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"INVALID_PREAMBLE_INDEX_ID");
		}

		/* Initializations */
		/*indicates the TC-RNTI assigned to this UE*/
		newUEcontext_p->crnti   = tcRNTI;
		newUEcontext_p->ueIndex = ueIndex;
		newUEcontext_p->tcrntiFlag = 1;
		newUEcontext_p->ulSyncLossOrHOInfoInDL.preambleIdx = preambleIndex;
		/* SPR 3157 changes start */
		newUEcontext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState = 
			UL_SYNC_LOSS_OFF;
		/* SPR 3157 changes end */
		/* SPR 12971 fix start */
		newUEcontext_p->internalCellIndex = internalCellIndex;
		/* SPR 12971 fix end */
		*tcRNTI_p = tcRNTI;
		dlUECtxInfoArr_g[ueIndex].dlUEContext_p = newUEcontext_p; 
		/* SPR 13667 fix start */
		memSet(&newUEcontext_p->measGapContext,0,sizeof(MeasGapContextDL));
		/* SPR 13667 fix end */

		LOG_MAC_MSG(CREATE_DL_UE_CONTXT_HO_ID,
				LOGWARNING,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				tcRNTI,
				ueIndex,
				preambleIndex,
				newUEcontext_p->tcrntiFlag,
				internalCellIndex,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,"CREATE_DL_UE_CONTXT_HO_ID");

		return MAC_SUCCESS;
	}
	else
	{
		LOG_MAC_MSG(MAC_TCRNTI_FAIL_ID,
				LOGWARNING,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				tcRNTI,
				LINE_NUMBER,
				ueIndex,
				internalCellIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,
				"HO_CRNTI_ALLOC_FAIL");

		return MAC_FAILURE;
	}
}

/****************************************************************************
 * Function Name  : createMACDLHarqEntity
 * Inputs         : dlHharqContext_pp - 
 *                  numHarqProcess - number of harq processes.
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE OR MAC_SUCCESS]
 * Description    : This is called by createMACDLUEEntity while creating the
 *                  UE entities harq context.
 ****************************************************************************/
/* + SPR 17733 */
MacRetType createMACDLHarqEntity( DLHARQEntity **dlHharqContext_pp,
        /* - SPR 17733 */
        UInt32 numHarqProcess )
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	UInt32 n = 0;
	DLHARQEntity  *dlHharqContext_p = PNULL;
	DLHARQProcess *dlHarqProcess_p     = PNULL;
	DLHARQTBInfo *dlHarqProcessTBOne_p = PNULL;
	DLHARQTBInfo *dlHarqProcessTBTwo_p = PNULL;
	/* Rel 5.3: Coverity Fix Start */
	MacRetType retVal                  = MAC_SUCCESS;
	/* Rel 5.3: Coverity Fix End */
	/* Rel 5.3.1: Memory related changes start*/
#if defined (KPI_STATS) || defined (PERF_STATS)
	UInt8 count=0;
#endif      
	/* Rel 5.3.1: Memory related changes end*/

	/* Its calling function set harqDLContext_p = PNULL therefore 
	   no need to check here.*/  
	GET_MEM_FROM_POOL(DLHARQEntity,*dlHharqContext_pp,sizeof (DLHARQEntity),PNULL);

	/* Rel 5.3: Coverity Fix Start */
	if (PNULL != *dlHharqContext_pp)
	{
		dlHharqContext_p = *dlHharqContext_pp;

		/* Number of Harq Processes also validate by lteMacParseUtil.c*/
		dlHharqContext_p->numHARQProcess = numHarqProcess;

		for (n = 0; n < MAX_HARQ_PROCESS_NB ; n++)
		{
			dlHarqProcess_p = &dlHharqContext_p->harqProcess[n];
			dlHarqProcessTBOne_p = &(dlHarqProcess_p->dlHARQTBOneInfo);
			dlHarqProcessTBTwo_p = &(dlHarqProcess_p->dlHARQTBTwoInfo);

			if ( n < dlHharqContext_p->numHARQProcess)
			{
				dlHarqProcess_p->harqProcessId = n;
				dlHarqProcess_p->harqStatus    = HARQ_PROCESS_FREE;
				dlHarqProcess_p->isTBOneValid  = FALSE;     
				dlHarqProcess_p->isTBTwoValid  = FALSE;
				dlHarqProcess_p->scellActBitmask = 0;
				dlHarqProcess_p->scellDeactBitmask = 0;
				/* SPS_CHG */
				dlHarqProcess_p->isSpsOnlyPaddingPresent = FALSE; 
				/* SPS_CHG */
				/* Indicates the sub frame number when the HARQ was sent 
				   to the PHY */
				dlHarqProcess_p->subFrameNum = 0;
				/* indicate sub band, wide band or no frequency selective 
				   type of transmission*/
				dlHarqProcess_p->transmissionType    = 0;
				/* This field will contain the index of the timer node */
				dlHarqProcess_p->dlharqRttExpiryTick = 0;
				dlHarqProcess_p->assignedResourcesNB = 0;            

				dlHarqProcess_p->transmissionBuffer_p = PNULL;           
				/* SPR 4787 Start */ 
				dlHarqProcess_p->transmissionBufferTB2_p = PNULL;           
				/* SPR 4787 End */ 

				dlHarqProcess_p->totalTBSize          = 0;
				/*SPR 3778 Fix Start*/
				dlHarqProcess_p->dciFormat            = MAX_NUM_DCI_FORMAT;
				/*SPR 3778 Fix End*/
				/* Rel 5.3.1: Memory related changes start*/
#if defined (KPI_STATS) || defined (PERF_STATS)
                /* SPR 16916 FIX START */
                /* +- */
                /* SPR 16916 FIX END */
                /* SPR 16613 fix start */
				for(count=0; count<(MAX_LOGICAL_CHANNEL-3); count++)
				{
					dlHarqProcess_p->pdcpBytes[count]=0;
                    dlHarqProcess_p->commonIndex[count]=0;
				}
                /* SPR 16613 fix end */
#endif      
#ifdef  PERF_STATS
				dlHarqProcess_p->totalREsUsed=0;
#endif                
				/* Rel 5.3.1: Memory related changes End*/
				//For First TB 
				dlHarqProcessTBOne_p->taDrxFlag     = INVALID_TA_DRX_MSG;
				dlHarqProcessTBOne_p->txIndicator   = INVALID_TX;
				dlHarqProcessTBOne_p->newDataIndicator   = NEW_TX; /* _NDI_SPR_ */
				dlHarqProcessTBOne_p->current_TX_NB = 0;
				dlHarqProcessTBOne_p->irVersion     = 0;
				dlHarqProcessTBOne_p->mcsIndex      = 0;
				dlHarqProcessTBOne_p->tbSize        = 0;
				dlHarqProcessTBOne_p->retransmissionTBSize = 0;
				dlHarqProcessTBOne_p->tbStartOffset = 0;
				dlHarqProcessTBOne_p->paddingBytes  = 0;
				dlHarqProcessTBOne_p->transmissionAndRLCData_p = PNULL;
				dlHarqProcessTBOne_p->macScellCEFlag = INVALID_SCELL_ACT_DEACT_MSG;
				//For Second TB
				dlHarqProcessTBTwo_p->taDrxFlag     = INVALID_TA_DRX_MSG;
				dlHarqProcessTBTwo_p->txIndicator   = INVALID_TX;
				dlHarqProcessTBTwo_p->newDataIndicator   = NEW_TX; /* _NDI_SPR_ */
				dlHarqProcessTBTwo_p->current_TX_NB = 0;
				dlHarqProcessTBTwo_p->irVersion     = 0;
				dlHarqProcessTBTwo_p->mcsIndex      = 0;
				dlHarqProcessTBTwo_p->tbSize        = 0;
				dlHarqProcessTBTwo_p->retransmissionTBSize = 0;
				dlHarqProcessTBTwo_p->tbStartOffset = 0;
				dlHarqProcessTBTwo_p->paddingBytes  = 0;
				dlHarqProcessTBTwo_p->transmissionAndRLCData_p = PNULL;
				dlHarqProcessTBTwo_p->macScellCEFlag = INVALID_SCELL_ACT_DEACT_MSG;
			}
			else 
			{
				dlHarqProcess_p->harqProcessId = INVALID_HARQ_ID;
				dlHarqProcess_p->harqStatus = HARQ_PROCESS_IN_USE;
			}   
		}

		UInt8 *dlHarqTimerNodeArrrayIndex_p = 
			&dlHharqContext_p->dlHarqTimerNodeArrrayIndex[0];

		*dlHarqTimerNodeArrrayIndex_p = INVALID_HARQ_ID;    
		*(++dlHarqTimerNodeArrrayIndex_p) = INVALID_HARQ_ID;    
		*(++dlHarqTimerNodeArrrayIndex_p) = INVALID_HARQ_ID;    
		*(++dlHarqTimerNodeArrrayIndex_p) = INVALID_HARQ_ID;    
		*(++dlHarqTimerNodeArrrayIndex_p) = INVALID_HARQ_ID;    
		*(++dlHarqTimerNodeArrrayIndex_p) = INVALID_HARQ_ID;    
		*(++dlHarqTimerNodeArrrayIndex_p) = INVALID_HARQ_ID;    
		*(++dlHarqTimerNodeArrrayIndex_p) = INVALID_HARQ_ID;    
		*(++dlHarqTimerNodeArrrayIndex_p) = INVALID_HARQ_ID;    
		*(++dlHarqTimerNodeArrrayIndex_p) = INVALID_HARQ_ID;    
		LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit ",__func__);
	}
	else
	{
		/* Not able to allocate Memory */
		retVal = MAC_FAILURE;
	}
	return retVal;    
	/* Rel 5.3: Coverity Fix End */
}

/****************************************************************************
 * Function Name  : reconfigureMACDLUEEntity
 * Inputs         : dwlkUEInfo_p - Its point to UeContextForDownlink
 *                  dlReconfigUeReq_p - Pointer for reconfiguration request 
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This API is used to fetch the UE Context pointer from the
 *                  global array and then reconfigure the various UE Context 
 *                  parameters for the requested UE.
 ****************************************************************************/
/*SPR 16855 +-*/
STATIC MacRetType reconfigureMACDLUEEntity ( UeContextForDownlink *dwlkUEInfo_p, 
		DLReconfigUeReq *dlReconfigUeReq_p,InternalCellIndex internalCellIndex)
/*SPR 16855 +-*/
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	UInt16 ueIndex = 0;
    /*SPR 16855 +-*/
	DLUEContext *newUEcontext_p = PNULL;
	ueIndex = dwlkUEInfo_p->ueIndex;
	/* Going to search UE context in index table */
	newUEcontext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
	if ( PNULL == newUEcontext_p )
	{
		LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR," Unable to\
				reconfigure the UE Entity. \n");
		return MAC_FAILURE;
	}


	/* Cyclomatic Complexity changes - starts here */
    /* +- SPR 17777 */
	if (MAC_SUCCESS != fillReconfigPowerCtrlInfo (dwlkUEInfo_p,
                /*SPR 16855 +-*/
                /* +- SPR 17777 */
				dlReconfigUeReq_p))
                /* +- SPR 17777 */
                /*SPR 16855 +-*/
	{
		return MAC_FAILURE;
	}
    /*SPR 21023 fix start*/
    dwlkUEInfo_p->timeAlignmentTimer   = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->timeAlignmentTimer ;
    /*SPR 21023 fix end*/
	if (MAC_SUCCESS != fillReconfigUECtxtProvParams (dwlkUEInfo_p, 
                /* +- SPR 17777 */
				newUEcontext_p,internalCellIndex
                /* +- SPR 17777 */
                /*SPR 16855 +-*/
				,dlReconfigUeReq_p
                /*SPR 16855 +-*/
				))
	{
		return MAC_FAILURE;
	}

	if (MAC_SUCCESS != fillReconfigCqiInfo (dwlkUEInfo_p, 
                /* +- SPR 17777 */
				newUEcontext_p
                /*SPR 16855 +-*/
				,dlReconfigUeReq_p
                /*SPR 16855 +-*/
				))
	{
		return MAC_FAILURE;
	}

    /* +- SPR 17777 */
	fillReconfigUECategoryInfo (newUEcontext_p
            /*SPR 16855 +-*/
			,dlReconfigUeReq_p
            /*SPR 16855 +-*/
			);
    /* +- SPR 17777 */

    /* +- SPR 17777 */
	fillReconfigDLAmbrInfo (dwlkUEInfo_p
            /*SPR 16855 +-*/
			,dlReconfigUeReq_p
            /*SPR 16855 +-*/
			,internalCellIndex         
			);
    /* +- SPR 17777 */

	if (MAC_SUCCESS != fillReconfigSpsInfo (dwlkUEInfo_p, 
                /* +- SPR 17777 */
				newUEcontext_p, internalCellIndex
                /*SPR 16855 +-*/
				,dlReconfigUeReq_p
                /*SPR 16855 +-*/
				))
	{
		return MAC_FAILURE;
	}

	/* CA changes Start */
    /* +- SPR 17777 */
	if (MAC_SUCCESS != fillReconfigLogicalChConfig (dwlkUEInfo_p, 
                /*SPR 16855 +-*/
				dlReconfigUeReq_p
                /*SPR 16855 +-*/
				))
                /* +- SPR 17777 */
	{
		return MAC_FAILURE;
	}
	/* Cyclomatic Complexity changes - ends here */

	/* SPR 11112 fix start */
	UInt8 n1PucchList = 0, n1PucchCs = 0;
	for (n1PucchList = 0; n1PucchList < MAX_PUCCH_AN_CS_LIST; n1PucchList++)
	{
		for (n1PucchCs = 0; n1PucchCs < MAX_PUCCH_AN_CS; n1PucchCs++)
		{
			dlReconfigUeReq_p->pucchFormatR10.n1Pucch[n1PucchList][n1PucchCs]=
				dwlkUEInfo_p->pucchFormatR10.n1Pucch[n1PucchList][n1PucchCs];
		}
	}
	macfillDLReconfigUECategoryV1020Info (dwlkUEInfo_p, 
			newUEcontext_p,
			internalCellIndex
            /*SPR 16855 +-*/
			,dlReconfigUeReq_p
            /*SPR 16855 +-*/
			);
	/* SPR 11112 fix end */


	if (MAC_SUCCESS != macValidateAndFillDLScellReconfigParams(dwlkUEInfo_p, 
				newUEcontext_p,internalCellIndex
                /*SPR 16855 +-*/
				,dlReconfigUeReq_p
                /*SPR 16855 +-*/
				))
	{
		return MAC_FAILURE;
	}
	/* CA changes End */
	/*EICIC changes Starts*/
	if ( LTE_TRUE == dwlkUEInfo_p->userTypePresent)
	{
		dlReconfigUeReq_p->interferenceConfigDL.userType = dwlkUEInfo_p->userType;
	}
	/*EICIC Changes Ends*/	

    /* SPR 18709 Fix Start */
    /* Filling the RAT1 Support value received from L3*/
    newUEcontext_p->Rat1Support = dwlkUEInfo_p->Rat1Support; 
    /* SPR 18709 Fix End */

	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);

	return MAC_SUCCESS;
}

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : fillReconfigPowerCtrlInfo 
 * Inputs         : dwlkUEInfo_p - Its point to UeContextForDownlink 
 *                  newUEcontext_p - Pointer to new DL UE context
 *                  dlReconfigUeReq_p - Pointer for reconfiguration request 
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This function fills the reconfigured Power control info
 ****************************************************************************/
STATIC  MacRetType fillReconfigPowerCtrlInfo (
		UeContextForDownlink *dwlkUEInfo_p, 
        /* +- SPR 17777 */
		DLReconfigUeReq     *dlReconfigUeReq_p)
        /* +- SPR 17777 */
{
    /*SPR 16855 +-*/
	/*CLPC Change*/
	if (dwlkUEInfo_p->isPAPresent == TRUE)
	{
        /*SPR 16855 +-*/
		dlReconfigUeReq_p->pdschConfigInfo.dlPdschTransPower = dwlkUEInfo_p->pA;
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] Pdsch Trans Power"\
				"At LP Side = %d \n",
				__func__,dlReconfigUeReq_p->pdschConfigInfo.dlPdschTransPower);
        /*SPR 16855 +-*/
	}

	/* CLPC_REWORK CHG */
	if (dwlkUEInfo_p->isTpcPdcchConfigPucchPresent == TRUE)
	{
        /*SPR 16855 +-*/
		dlReconfigUeReq_p->tpcPDCCHConfigPUCCHInfo.release =
			dwlkUEInfo_p->tpcPdcchConfigPucch.release;
		dlReconfigUeReq_p->tpcPDCCHConfigPUCCHInfo.tpcIndex =
			dwlkUEInfo_p->tpcPdcchConfigPucch.tpcBitMapIndex;
		dlReconfigUeReq_p->tpcPDCCHConfigPUCCHInfo.tpcRnti =
			dwlkUEInfo_p->tpcPdcchConfigPucch.tpcRnti;
		dlReconfigUeReq_p->tpcPDCCHConfigPUCCHInfo.isConfiguredForDci3Or3A =
			dwlkUEInfo_p->tpcPdcchConfigPucch.isConfiguredForDci3Or3A;
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] At LP Side ,release = %d tpcIndex = %d .tpcRnti = %d "\
				"isConfiguredForDci3Or3A = %d \n ",__func__,
				dlReconfigUeReq_p->tpcPDCCHConfigPUCCHInfo.release,
				dlReconfigUeReq_p->tpcPDCCHConfigPUCCHInfo.tpcIndex,
				dlReconfigUeReq_p->tpcPDCCHConfigPUCCHInfo.tpcRnti,
				dlReconfigUeReq_p->tpcPDCCHConfigPUCCHInfo.isConfiguredForDci3Or3A
			      );
        /*SPR 16855 +-*/
	}
	/* CLPC_REWORK_CHG END */
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : fillReconfigUECtxtProvParams
 * Inputs         : dwlkUEInfo_p - Its point to UeContextForDownlink 
 *                  newUEcontext_p - Pointer to new DL UE context
 *                  ueIndex - Stores UE index
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                  dlReconfigUeReq_p - Pointer for reconfiguration request 
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This function fills the UE context provisioned reconfig
 *                  parameters.
 ****************************************************************************/
STATIC  MacRetType fillReconfigUECtxtProvParams (
		UeContextForDownlink *dwlkUEInfo_p, 
		DLUEContext          *newUEcontext_p,
        /* +- SPR 17777 */
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		)
{
	/* CA Changes start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */
	/* + TM7_8 Changes Start */
	UInt8 counter = 0;
	/* - TM7_8 Changes End */
	/* Reconfiguring the provisioned UE Context parameters */
	if ( dwlkUEInfo_p->maxRB )
	{
        /*SPR 16855 +-*/
		dlReconfigUeReq_p->dlMaxRBInfo.dlMaxRB = dwlkUEInfo_p->maxRB;
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
				"[%s] Maximum number of RBs At LP side = %d \n",__func__,
				dlReconfigUeReq_p->dlMaxRBInfo.dlMaxRB);
        /*SPR 16855 +-*/
	}

	/* SPR_4410 Fix Start */
	if (INVALID_TX_MODE !=  dwlkUEInfo_p->transmissionMode )
		/* SPR_4410 Fix End */
	{
		/* SPR 13783 fix start */
		/* code removed */
		/* SPR 13783 fix end */

        /*SPR 16855 +-*/
		dlReconfigUeReq_p->transMode.transMode = dwlkUEInfo_p->transmissionMode;
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
				"[%s] Transmission Mode At LP side = %d \n",__func__,
				dlReconfigUeReq_p->transMode.transMode);
        /*SPR 16855 +-*/
	}

	/* + CQI_5.0 */
	/* SPR 13783 fix start */
	if( ((TX_MODE_3 == dwlkUEInfo_p->transmissionMode) || 
				(TX_MODE_4 == dwlkUEInfo_p->transmissionMode)||
				/* + TM6_5.2 */
				(TX_MODE_6 == dwlkUEInfo_p->transmissionMode)) &&
			/* - TM6_5.2 */
			( !((2 == cellParams_p->numOfTxAnteenas ) || 
			    (4 == cellParams_p->numOfTxAnteenas ))) )	
	{
		lteWarning("With TransmissionMode %d, number antennas are neither 2 nor 4\n",
				dwlkUEInfo_p->transmissionMode); 
		/* SPR 13783 fix end */
		return MAC_FAILURE;

	}
	/* - CQI_5.0 */ 
	/* + CL-MIMO LJA*/
	/* SPR 2454 changes start */
	if ( INVALID_RANK_INDICATOR != dwlkUEInfo_p->rankIndicator )
		/* SPR 2454 changes end */
	{    
		if (dwlkUEInfo_p->cbsrInfo.cbsrType != CBSR_INVALID)
		{
            /*SPR 16855 +-*/
			dlReconfigUeReq_p->cbsrInfo = 
				dwlkUEInfo_p->cbsrInfo;
			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
					"[%s] Code Book Subset Restriction At LP side = %d \n",__func__,
					dlReconfigUeReq_p->cbsrInfo);
            /*SPR 16855 +-*/
		}
	}
	/* - CL-MIMO LJA*/    
	if ( INVALID_CODE_BOOK_INDEX != dwlkUEInfo_p->precodingIndex )
	{
        /*SPR 16855 +-*/
		dlReconfigUeReq_p->codeBookIndexInfo.codeBookIndex = 
			dwlkUEInfo_p->precodingIndex;
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
				"[%s] Codebook Index At LP side = %d \n",__func__,
				dlReconfigUeReq_p->codeBookIndexInfo.codeBookIndex);
        /*SPR 16855 +-*/
	}

	/* SPR 903 Start*/
	if ( (MAC_MAX_UE_CATEGORY >= dwlkUEInfo_p->ueCategory ) &&
			(MAC_MIN_UE_CATEGORY <= dwlkUEInfo_p->ueCategory) )
	{
		dlReconfigUeReq_p->ueCategoryInfo.ueCategory = dwlkUEInfo_p->ueCategory;
	}
	/* CA Changes Start */
	/* SPR 12246 Start */
	if (TRUE == dwlkUEInfo_p->ueCategoryV1020ReqType)
	{        
		if ( macValidateUeCategoryV1020(dwlkUEInfo_p->ueCategoryV1020, internalCellIndex) )
		{
			dlReconfigUeReq_p->ueCategoryInfov1020.ueCategoryV1020 = dwlkUEInfo_p->ueCategoryV1020;
		}
		else
		{
			return MAC_FAILURE;
		}
	}
	/* SPR 12246 End */
	/* CA Changes End */
	/* SPR 903 End*/
	/* + SPR_3939 */
	/*SPR_3061_FIX*/ 
	/*Fix for __SPR_1988__ START*/
	if(0 != dwlkUEInfo_p->timeAlignmentTimer)
	{
        /*SPR 16855 +-*/
		dlReconfigUeReq_p->timeAlignmentTimerInfo.ulSyncTimer = 
			dwlkUEInfo_p->timeAlignmentTimer;
		dlReconfigUeReq_p->timeAlignmentTimerInfo.syncTimerDelta =
			dlReconfigUeReq_p->timeAlignmentTimerInfo.ulSyncTimer - 
			UL_SYNC_TIMER_DIFF_THRESHOLD_FOR_SENDING_TA;

		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] At LP side "\
				"UL Sync Timer = %d \n UL Sync Timer Delta = %d \n",
				__func__, dlReconfigUeReq_p->timeAlignmentTimerInfo.ulSyncTimer,
				dlReconfigUeReq_p->timeAlignmentTimerInfo.syncTimerDelta);
        /*SPR 16855 +-*/
	}
	/*Fix for __SPR_1988__ END*/
	/* - SPR_3939 */
    /*SPR 16855 +-*/
	/* + TM7_8 Changes Start */
	if(TX_MODE_8 == dwlkUEInfo_p->transmissionMode)
	{
		newUEcontext_p->isPmiRiConfigured_V920 = dwlkUEInfo_p->pmiRiReportV920;
	}
	newUEcontext_p->pdschEpreToUeRsRatio = dwlkUEInfo_p->pdschEpreToUeRsRatio;
	for(counter = 0; counter < MAX_NUM_ANTENNA ;counter++)
	{
		newUEcontext_p->AoA[counter] = INVALID_VALUE_OF_AOA ;
	}

	/* - TM7_8 Changes End */

	/* + TM6_5.2 */
	if(TX_MODE_6 == dwlkUEInfo_p->transmissionMode)
	{
		/* + SPR 5953 Fix */ 
		newUEcontext_p->tm6DCIFormat = DCI_FORMAT_1A;

		if((((PERIODIC_CQI_REPORTING_MODE == dwlkUEInfo_p->cqiIndication) &&
						(1 == dwlkUEInfo_p->cqiRequestType)) ||
					(APERIODIC_CQI_REPORTING_MODE == dwlkUEInfo_p->cqiIndication) ||
					(PERIODIC_APERIODIC_MODE == dwlkUEInfo_p->cqiIndication)) &&
				(INVALID_CODE_BOOK_INDEX != dwlkUEInfo_p->precodingIndex))
		{
			newUEcontext_p->tm6DCIFormat = DCI_FORMAT_1B;
		}
		else if((INVALID_CQI_REPORTING_MODE != newUEcontext_p->dlCQIInfo.cqiTypeIndicator)&&
				(INVALID_CQI_REPORTING_MODE == dwlkUEInfo_p->cqiIndication) &&
				(INVALID_CODE_BOOK_INDEX != dwlkUEInfo_p->precodingIndex))
		{
			/* - SPR 5953 Fix */ 
			newUEcontext_p->tm6DCIFormat = DCI_FORMAT_1B;

		}
	}
	/* - TM6_5.2 */
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : fillReconfigCqiInfo
 * Inputs         : dwlkUEInfo_p - Its point to UeContextForDownlink 
 *                  newUEcontext_p - Pointer to new DL UE context
 *                  ueIndex - Stores UE index
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                  dlReconfigUeReq_p - Pointer for reconfiguration request 
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This function fills the reconfigure CQI Info parameters.
 ****************************************************************************/
STATIC  MacRetType fillReconfigCqiInfo (
		UeContextForDownlink *dwlkUEInfo_p, 
		DLUEContext          *newUEcontext_p,
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		DLReconfigUeReq     *dlReconfigUeReq_p
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		)
{
    /*SPR 16855 +-*/
	/* + SPR 5953 Fix */
	newUEcontext_p->dlCQIInfo.cqiRequestType = dwlkUEInfo_p->cqiRequestType;
	/* - SPR 5953 Fix */

    /*SPR 16855 +-*/
	if (dwlkUEInfo_p->cqiInfoFlag)
	{
        /*SPR 16855 +-*/
		dlReconfigUeReq_p->dlCqiConfigInfo.cqiMode = dwlkUEInfo_p->cqiMode;
		dlReconfigUeReq_p->dlCqiConfigInfo.modScheme = dwlkUEInfo_p->modScheme;
		dlReconfigUeReq_p->dlCqiConfigInfo.cqiTypeIndicator = dwlkUEInfo_p->cqiIndication;
		/* SPR 5206 FIX Start */
		if ( INVALID_APERIODIC_CQI_MODE != dwlkUEInfo_p->cqiMode )
		{
			if(INVALID_TX_MODE !=  dwlkUEInfo_p->transmissionMode)
			{
				/* + TM7_8 Changes Start */
				if(TX_MODE_8 == dlReconfigUeReq_p->transMode.transMode)
				{
					if ( MAC_FAILURE ==  
							( validateAperiodicModeForTXMode8(dwlkUEInfo_p->cqiMode,
											  dwlkUEInfo_p->pmiRiReportV920)))
					{    
						lteWarning( "TX mode [%d] is not compatible with CQI aperiodic mode [%d]\n",
								dlReconfigUeReq_p->transMode.transMode,dwlkUEInfo_p->cqiMode);
						return  MAC_FAILURE ;
					}
				}
				/* - TM7_8 Changes End */
				else if ( MAC_FAILURE == (validateAperiodicModeForTXMode(
								dwlkUEInfo_p->cqiMode,
								dlReconfigUeReq_p->transMode.transMode)))
				{
					lteWarning( "TX mode [%d] is not compatible with CQI aperiodic mode [%d]\n",
							newUEcontext_p->dlMIMOInfo.transmissionMode,dwlkUEInfo_p->cqiMode);
					return  MAC_FAILURE ;
				}
			}
			else
			{
				/* + TM7_8 Changes Start */
				if(TX_MODE_8 == newUEcontext_p->dlMIMOInfo.transmissionMode)
				{
					if ( MAC_FAILURE ==  
							( validateAperiodicModeForTXMode8(dwlkUEInfo_p->cqiMode,
											  dwlkUEInfo_p->pmiRiReportV920)))
					{    
						lteWarning( "TX mode [%d] is not compatible with CQI aperiodic mode [%d]\n",
								newUEcontext_p->dlMIMOInfo.transmissionMode,dwlkUEInfo_p->cqiMode);
						return  MAC_FAILURE ;
					}
				}
				/* - TM7_8 Changes End */
				else if ( MAC_FAILURE == (validateAperiodicModeForTXMode(
								dwlkUEInfo_p->cqiMode,
								newUEcontext_p->dlMIMOInfo.transmissionMode) ))
				{
					lteWarning( "TX mode [%d] is not compatible with CQI aperiodic mode [%d]\n",
							newUEcontext_p->dlMIMOInfo.transmissionMode,dwlkUEInfo_p->cqiMode);

					return  MAC_FAILURE ;
				}
			}
		}
		/* SPR 5206 FIX End */
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] CQI Mode At LP side = %d \n",__func__,
				dlReconfigUeReq_p->dlCqiConfigInfo.cqiMode);
    /*SPR 16855 +-*/
	}
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : fillReconfigUECategoryInfo 
 * Inputs         : dwlkUEInfo_p - Its point to UeContextForDownlink 
 *                  newUEcontext_p - Pointer to new DL UE context
 *                  dlReconfigUeReq_p - Pointer for reconfiguration request 
 * Outputs        : None.
 * Returns        : None. 
 * Description    : This function fills the reconfigure UE category Info.
 ****************************************************************************/
STATIC  void fillReconfigUECategoryInfo (
        /* +- SPR 17777 */
		DLUEContext          *newUEcontext_p
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		)
{
	/*Soft Buffer Changes Start*/
	UInt32 totalSoftchannelBytes = 0;
	/*Soft Buffer Changes End*/

	/* CA Changes start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[newUEcontext_p->internalCellIndex]->cellParams_p;
	/* CA Changes end */
	/*Update the max TB size for the category of the UE  */
    /*SPR 16855 +-*/
    /* +- SPR 18478 */
    if (dlReconfigUeReq_p->ueCategoryInfo.ueCategory != 0 )
    {
        dlReconfigUeReq_p->ueCategoryInfo.maxTBSize =
            DIVIDE_BY_EIGHT(cellParams_p->dlUeCategoryInfo[dlReconfigUeReq_p->ueCategoryInfo.ueCategory - 1].
                    maxBitsDLSCHTb);
        dlReconfigUeReq_p->ueCategoryInfo.maxSISOTBSize = 
            DIVIDE_BY_EIGHT(cellParams_p->dlUeCategoryInfo[dlReconfigUeReq_p->ueCategoryInfo.ueCategory - 1].
                    maxBitsDLSCHTb);

        dlReconfigUeReq_p->ueCategoryInfo.maxMIMOTBSize = 
            DIVIDE_BY_EIGHT(cellParams_p->dlUeCategoryInfo[dlReconfigUeReq_p->ueCategoryInfo.ueCategory - 1].
                    maxDLSCHTbBits);
        /*Soft Buffer Changes Start*/
        totalSoftchannelBytes = DIVIDE_BY_EIGHT(\
                cellParams_p->dlUeCategoryInfo[dlReconfigUeReq_p->ueCategoryInfo.ueCategory - 1].totSoftChBits);
        dlReconfigUeReq_p->ueCategoryInfo.permissibleTBSize = (totalSoftchannelBytes)/MAC_CODING_FACTOR 
            -(newUEcontext_p->maxPermissibleTBSize - newUEcontext_p->permissibleTBSize);
        dlReconfigUeReq_p->ueCategoryInfo.maxPermissibleTBSize = (totalSoftchannelBytes)/MAC_CODING_FACTOR; 
        /*Soft Buffer Changes End*/
    }
    /* +- SPR 18478 */

	LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s]At LP side \n"\
			"\n MAX TB Size = %d\n MAX SISO TB Size = %d \n "\
			"MAX MIMO TB Size = %d \n",__func__,
			dlReconfigUeReq_p->ueCategoryInfo.maxTBSize,
			dlReconfigUeReq_p->ueCategoryInfo.maxSISOTBSize,
			dlReconfigUeReq_p->ueCategoryInfo.maxMIMOTBSize);
    /*SPR 16855 +-*/
}

/****************************************************************************
 * Function Name  : fillReconfigDLAmbrInfo 
 * Inputs         : dwlkUEInfo_p - Its point to UeContextForDownlink 
 *                  newUEcontext_p - Pointer to new DL UE context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                  dlReconfigUeReq_p - Pointer for reconfiguration request 
 * Outputs        : None.
 * Returns        : None. 
 * Description    : This function fills the reconfigure DL AMBR Info.
 ****************************************************************************/
/* CA Changes start */
STATIC  void fillReconfigDLAmbrInfo (
		UeContextForDownlink *dwlkUEInfo_p, 
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		DLReconfigUeReq     *dlReconfigUeReq_p
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		,InternalCellIndex internalCellIndex
		)
{
	InitParams *initParams_p = cellSpecificParams_g.\
				   cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
	/*Fill AMBR*/
	if(dwlkUEInfo_p->dlAmbrPresent == TRUE)
	{
        /*SPR 16855 +-*/
		dlReconfigUeReq_p->dlAmbrInfo.AMBR = dwlkUEInfo_p->dlAmbr;
        /*SPR 17444 +-*/
        /*SPR 17444 +-*/
		dlQosCalcAmbrTknRate(&dlReconfigUeReq_p->dlAmbrInfo,
				initParams_p->dlSchdConfig.dlTokenPeriodicity);
		dlReconfigUeReq_p->dlAmbrInfo.curTknWindowInd = 0;

		/*calculate ambr token priority*/
		dlReconfigUeReq_p->dlAmbrInfo.ambrTokenPriority = 5 ;

		/*calculate ambr token priority */
		dlReconfigUeReq_p->dlAmbrInfo.availAmbrToken =
			dlReconfigUeReq_p->dlAmbrInfo.ambrTokenRate;

		/*Calculate AMBR bucket depth*/
		dlReconfigUeReq_p->dlAmbrInfo.ambrTokenDepth =
			dlReconfigUeReq_p->dlAmbrInfo.ambrTokenRate * 
			initParams_p->dlSchdConfig.ambrBktFactor;

		/* Calculate bucket limit(minimum tokens to present in bucket) */
		dlReconfigUeReq_p->dlAmbrInfo.ambrTokenLimit = 
			initParams_p->dlSchdConfig.ambrTokenLimitFactor * 
			dlReconfigUeReq_p->dlAmbrInfo.ambrTokenRate;

		/*Update the Stkn(t) with the token rate*/
		dlReconfigUeReq_p->dlAmbrInfo.scheduledToken[0] = 0;

		/*Update the lcTknTable in dlAmbrInfo*/
		dlQosUpdateTknTableForLcPriority(
				dlReconfigUeReq_p->dlAmbrInfo.lcTknTable,
				dlReconfigUeReq_p->dlAmbrInfo.AMBR,
				initParams_p->dlSchdConfig.dlTokenPeriodicity); 

		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] At LP Side \n"\
				"AMBR Value= %d \n Token Priority = %d \n "\
				"Available Token =%d \n Tokem Depth =%d \n "\
				"Token Limit =%d \n Scheduled Token =%d ",
				__func__, dlReconfigUeReq_p->dlAmbrInfo.AMBR,
				dlReconfigUeReq_p->dlAmbrInfo.ambrTokenPriority,
				dlReconfigUeReq_p->dlAmbrInfo.availAmbrToken,
				dlReconfigUeReq_p->dlAmbrInfo.ambrTokenDepth,
				dlReconfigUeReq_p->dlAmbrInfo.ambrTokenLimit,
				dlReconfigUeReq_p->dlAmbrInfo.scheduledToken[0]);
        /*SPR 16855 +-*/

	}
}

/****************************************************************************
 * Function Name  : fillReconfigSpsInfo 
 * Inputs         : dwlkUEInfo_p - Its point to UeContextForDownlink 
 *                  newUEcontext_p - Pointer to new DL UE context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                  dlReconfigUeReq_p - Pointer for reconfiguration request 
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This function fills the reconfigure SPS Info.
 ****************************************************************************/
STATIC  MacRetType fillReconfigSpsInfo (
		UeContextForDownlink *dwlkUEInfo_p, 
		DLUEContext          *newUEcontext_p,
        /* +- SPR 17777 */
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		)
{
        /*SPR 20456  code removed+-*/
	/* SPS_CHG */
	if (((TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
					->initParams_p->spsSysWideParam.enableSps) && \
				(TRUE == dwlkUEInfo_p->isSpsDlInfoPresent))
#ifdef HD_FDD_CONFIG
			/* Config UE  --   True  False INVALID TRUE
			   Reconfig UE--   True  True  TRUE    INVALID 
			   i.e., UE is going to be set to Half Duplex mode 
			 */
			||
			((((newUEcontext_p->isHDFddFlag == TRUE) && (dwlkUEInfo_p->isHDFddFlag != FALSE)) ||
			  ((newUEcontext_p->isHDFddFlag != TRUE) && (dwlkUEInfo_p->isHDFddFlag == TRUE))) &&
			 /* This UE was in SPS mode, so need to remove the SPS for HD UE now */
			 (newUEcontext_p->spsDlInfo.requestType == SPS_SETUP))
#endif
	   )
	{
		/* SPR 6776 SPS Reconfig Fix start*/
        /*SPR 16855 +-*/
		/* In case of UL/DL running different, store the received HDFDD flag which
		 * will be updated in other operations for both UL and DL UE Context */
		dlReconfigUeReq_p->ueReconfigTlvRecieved[dlReconfigUeReq_p->ueReconfigMsgTlvCount]= DL_RECONFIG_SPS_PARAMS;
		dlReconfigUeReq_p->ueReconfigMsgTlvCount++;
		/* SPR 6776 SPS Reconfig Fix End*/
        /*SPR 16855 +-*/

		/* Reseting the isSpsDlInfoPresent flag */
		dwlkUEInfo_p->isSpsDlInfoPresent = FALSE;
		/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
		/* If HD FDD is enabled, then marking SPS requestType as Release*/
		if (((newUEcontext_p->isHDFddFlag == TRUE) && (dwlkUEInfo_p->isHDFddFlag != FALSE)) ||
				((newUEcontext_p->isHDFddFlag != TRUE) && (dwlkUEInfo_p->isHDFddFlag == TRUE)))
		{
			dwlkUEInfo_p->spsDownlinkInfo.requestType = SPS_RELEASE;
		}
#endif
		/*HD FDD Changes End*/
		if (dwlkUEInfo_p->spsDownlinkInfo.requestType\
				== newUEcontext_p->spsDlInfo.requestType)
		{
			LOG_MAC_MSG(MAC_SPS_RECONFIG, LOGWARNING, MAC_RRC_INF,
					GET_CURRENT_TICK_FOR_CELL(internalCellIndex), __LINE__,newUEcontext_p->ueIndex,
					newUEcontext_p->spsDlInfo.requestType,
					dwlkUEInfo_p->spsDownlinkInfo.requestType,
					internalCellIndex, 0,0, __func__, "" );
		}
		else
		{    
			/* DRX SUPPORT */
			/*!
			 *\code
			 * Long and short DRX cycle can be supported in SPS , 
			 * only if Long DRX cycle is equivalent to UE SPS interval,
			 * otherwise do not allocate SPS-CRNTI
			 *\endcode
			 */
			if (SETUP == dwlkUEInfo_p->drxConfigType &&
					SPS_SETUP == dwlkUEInfo_p->spsDownlinkInfo.requestType && 
					(drxLongCycleTimerConfig_g[dwlkUEInfo_p->drxConfig.
					 longDRXCycleIndex] != spsIntervalMap_g[dwlkUEInfo_p->
					 spsDownlinkInfo.spsDownlinkSetupInfo.
					 semiPersistentSchedIntervalDL])) 
			{
				LOG_MAC_MSG( MAC_DRX_SPS_SUPPORT_UE, LOGWARNING, MAC_RRC_INF,
						GET_CURRENT_TICK_FOR_CELL(internalCellIndex), __LINE__, dwlkUEInfo_p->drxConfig.longDRXCycle, 
						dwlkUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.semiPersistentSchedIntervalDL,
						newUEcontext_p->ueIndex, 0, 0,0, __func__, "" );
        /*SPR 20456  code removed+-*/
			}
			else
			{
				/* SPS Reconfig Fix*/
                /*SPR 16855 +-*/
				/* In case of UL/DL running different, store the received HDFDD flag which
				 * will be updated in other operations for both UL and DL UE Context */
				dlReconfigUeReq_p->ueReconfigTlvRecieved[dlReconfigUeReq_p->ueReconfigMsgTlvCount]= DL_RECONFIG_SPS_PARAMS;
				dlReconfigUeReq_p->ueReconfigMsgTlvCount++;
                /*SPR 16855 +-*/
				/* SPS Reconfig Fix*/
				/* Cyclomatic Complexity changes - starts here */
				if (MAC_SUCCESS != fillReconfigSpsDLRequestType (dwlkUEInfo_p, 
                            /* +- SPR 17777 */
							newUEcontext_p,
                            /*SPR 16855 +-*/
							dlReconfigUeReq_p
                            /* +- SPR 17777 */
                            /*SPR 16855 +-*/
							))
				{
					return MAC_FAILURE;
				}
				/* Cyclomatic Complexity changes - ends here */
			}
		}
	}
	/* SPS_CHG */
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : fillReconfigSpsDLRequestType 
 * Inputs         : dwlkUEInfo_p - Its point to UeContextForDownlink 
 *                  newUEcontext_p - Pointer to new DL UE context
 *                  dlReconfigUeReq_p - Pointer for reconfiguration request 
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This function fills reconfigure SPS DL Request Type Info.
 ****************************************************************************/
STATIC  MacRetType fillReconfigSpsDLRequestType (
		UeContextForDownlink *dwlkUEInfo_p, 
		DLUEContext          *newUEcontext_p,
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
        DLReconfigUeReq     *dlReconfigUeReq_p
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		)
{
    /*SPR 16855 +-*/
	dlReconfigUeReq_p->spsDlInfo.requestType = 
		dwlkUEInfo_p->spsDownlinkInfo.requestType;
    /*SPR 16855 +-*/
	/*!
	 *\code
	 * If spsDownlinkInfo.requestType == SPS_RELEASE
	 *   -Deallocate the SpsCrnti.
	 *   -Update the SPS State. 
	 *   -Free the Memory of spsDlSetupInfo_p.
	 *\endcdoe
	 **/ 
	if (SPS_RELEASE == \
			dwlkUEInfo_p->spsDownlinkInfo.requestType)
	{
    /*SPR 16855 +-*/
	}
	else
	{
		if (MAC_SUCCESS != fillReconfigDLSpsSetupInfo (dwlkUEInfo_p, 
                    /* +- SPR 17777 */
					newUEcontext_p
                    /*SPR 16855 +-*/
					,dlReconfigUeReq_p
                    /*SPR 16855 +-*/
					))
		{
			return MAC_FAILURE;
		}
	}
	return MAC_SUCCESS;
}

/*SPR 16855 +-*/
/****************************************************************************
 * Function Name  : fillReconfigDLSpsSetupInfo
 * Inputs         : dwlkUEInfo_p - Its point to UeContextForDownlink 
 *                  newUEcontext_p - Pointer to new DL UE context
 *                  ueIndex - Stores UE index
 *                  dlReconfigUeReq_p - Pointer for reconfiguration request 
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This function fills reconfigure SPS DL Setup Info.
 ****************************************************************************/
STATIC  MacRetType fillReconfigDLSpsSetupInfo (
		UeContextForDownlink *dwlkUEInfo_p, 
		DLUEContext          *newUEcontext_p,
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		DLReconfigUeReq     *dlReconfigUeReq_p
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		)
{
	/* SPS_CHG */
	UInt8 index = 0;
	/* SPS_CHG */
    /*SPR 16855 +-*/

	/*!
	 * \code
	 * IF Sps Req type is setup
	 *    - Save the sps crnti in Ue context
	 *    -Populating the SPS Info in UE Context recived from L3 
	 * \endcode
	 * */
    /*SPR 16855 +-*/
	dlReconfigUeReq_p->spsDlInfo.semiPersistentSchedIntervalDL = 
		dwlkUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.
		semiPersistentSchedIntervalDL;
	dlReconfigUeReq_p->spsDlInfo.numberOfConfSpsProcesses = 
		dwlkUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.
		numberOfConfSpsProcesses;

	/* If maxSpsHarqRetx not received then 
	 * intitialize with default value
	 */
	if (TRUE == dwlkUEInfo_p->spsDownlinkInfo.\
			spsDownlinkSetupInfo.isMaxSpsHarqRetxPresent)
	{
		dlReconfigUeReq_p->spsDlInfo.maxSpsHarqRetx = 
			dwlkUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.maxSpsHarqRetx;
	}
	else
	{
		dlReconfigUeReq_p->spsDlInfo.maxSpsHarqRetx = 
			MAX_SPS_HARQ_RETX_DEFAULT_VALUE;
	}

	/* If explicitReleaseAfter not received then 
	 * intitialize with default value
	 */
	if (TRUE == dwlkUEInfo_p->spsDownlinkInfo.\
			spsDownlinkSetupInfo.isExplicitReleaseAfterPresent)
	{
		dlReconfigUeReq_p->spsDlInfo.explicitReleaseAfter =
			dwlkUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.explicitReleaseAfter;
	}
	else
	{
		dlReconfigUeReq_p->spsDlInfo.explicitReleaseAfter = 
			EXPLICIT_RELEASE_AFTER_DEFAULT_VALUE;
	}

	/* Initializing the Optional parameter 
	 * N1PucchAnPersistent in UE Context 
	 */
	/* SPR 6776 SPS Reconfig Fix start*/
	if (( PNULL != newUEcontext_p->spsDlInfo.spsDlSetupInfo_p)
			&&(0 == newUEcontext_p->spsDlInfo.spsDlSetupInfo_p->\
				N1PucchAnPersistentListVal. numOfN1PucchAnPersistent &&\
				0 == dwlkUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.\
				N1PucchAnPersistentListVal.numOfN1PucchAnPersistent))
	{
		/* SPR 6776 SPS Reconfig Fix End*/
		for (index = 0; index < MAX_N1_PUCCH_AN_PERSISTENT; index++)
		{
			dlReconfigUeReq_p->spsDlInfo.\
				N1PucchAnPersistentListVal.N1PucchAnPersistent[index] = 0;
		}
	}
	else
	{
		dlReconfigUeReq_p->spsDlInfo.N1PucchAnPersistentListVal.\
			numOfN1PucchAnPersistent = \
			dwlkUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.\
			N1PucchAnPersistentListVal.numOfN1PucchAnPersistent;
		/* SPR 6776 SPS Reconfig Fix Start*/
		while(index <  dwlkUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.\
				N1PucchAnPersistentListVal.numOfN1PucchAnPersistent)
			/* SPR 6776 SPS Reconfig Fix End*/
		{ 
			dlReconfigUeReq_p->spsDlInfo.\
				N1PucchAnPersistentListVal.N1PucchAnPersistent[index] = \
				dwlkUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.\
				N1PucchAnPersistentListVal.N1PucchAnPersistent[index];
			index++;
		}
	}
    /*SPR 16855 +-*/

	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : fillReconfigLogicalChConfig 
 * Inputs         : dwlkUEInfo_p - Its point to UeContextForDownlink 
 *                  newUEcontext_p - Pointer to new DL UE context
 *                  ueIndex - Stores UE index
 *                  dlReconfigUeReq_p -
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This function fills the reconfigure Logical channel 
 *                  configuration.
 ****************************************************************************/
STATIC  MacRetType fillReconfigLogicalChConfig (
		UeContextForDownlink *dwlkUEInfo_p, 
        /* +- SPR 17777 */
        /*SPR 16855 +-*/
		DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
        /* +- SPR 17777 */
		)
{
	UInt32 logicalChCount  = 0;
	LogicalChannelConfigInfo* logicalChConfig_p = PNULL;
    /*SPR 16855 +-*/
	UInt32 channelIndex = 0;
    /*SPR 16855 +-*/

	logicalChConfig_p = dwlkUEInfo_p->logicalChConfig;
	logicalChCount    = dwlkUEInfo_p->dwlkLogicalChCount; 

	if ( PNULL == logicalChConfig_p)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR,
				"Received a NULL Pointer for Logical Channels.\n");
		return MAC_SUCCESS;
    }
    /*SPR 16855 +-*/
	dlReconfigUeReq_p->dlReconfigLcReq.lcCount = logicalChCount;
	for(channelIndex = 0; channelIndex < logicalChCount ; channelIndex++)
    {
		dlReconfigUeReq_p->dlReconfigLcReq.logicalChConfig[channelIndex] =
			dwlkUEInfo_p->logicalChConfig[channelIndex];
    }
    /*SPR 16855 +-*/
    return MAC_SUCCESS;
}

/* Cyclomatic Complexity changes - ends here */
/****************************************************************************
 * Function Name  : deleteMACDLHarqEntity
 * Inputs         : ueContext_p -
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This API is used to delete a harq Context from the MAC 
 *                  system. (Harq Memory free from strategy) 
 ****************************************************************** **********/
/* SPR 5288 Start */
/*CA Changes start  */
STATIC MacRetType deleteMACDLHarqEntity (DLUEContext*  ueContext_p, 
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
/* SPR 5288 End */
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	UInt8 harqCount = 0;
	DLHARQProcess *harqProcess_p = PNULL;
	/* SPR 5288 Start */
	DLHARQEntity *harqEntity_p = ueContext_p->dlHarqContext_p;
	/* SPR 5288 End */
	if (harqEntity_p != PNULL)
	{
		for (;harqCount < harqEntity_p->numHARQProcess; harqCount++)
		{
			harqProcess_p = &harqEntity_p->harqProcess[harqCount];
			/* SPR 5288 Start*/
			/*CA Changes start  */
			freeDLHarqProcess(ueContext_p,harqProcess_p, internalCellIndex);
			/*CA Changes end  */
			/* SPR 5288 End */
		}
	}    
	else
	{
		lteWarning (" Entry does not exists in the harqDLContextArr_g\n");
		LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR," Unable to delete the Entity. \n");
		return MAC_FAILURE;
	}
	freeMemPool(harqEntity_p);\
	harqEntity_p = PNULL;
		LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
	return MAC_SUCCESS;    
}

/****************************************************************************
 * Function Name  : addMACDLLogicalChannel
 * Inputs         : newUEContext_p - pointer to UE context.
 *                  logicalChCount - number of logical channels of UE.
 *                  logicalChConfig_p - A pointer to the LogicalChannelConfig
 *                                      array containing the various
 *                                      configuration parameters for the 
 *                                      logical channel.
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                 
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This API is used to add a logical channel as per the 
 *                  configuration for a given UE.
 *                  If a UE does not exist, for which this API is invoked then
 *                  return MAC_FAILURE.
 ****************************************************************************/
/* CA changes start */
MacRetType addMACDLLogicalChannel( 
		DLUEContext* newUEContext_p,
		UInt32 logicalChCount,
		LogicalChannelConfigInfo* logicalChConfig_p,
		InternalCellIndex internalCellIndex)
/* CA changes end */
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	UInt32 lcIndex     = 0;
	UInt32 tempLogicalChId = 0;
	DLLogicalChannelInfo *dlLogicalChannel_p = PNULL; 
	DLQosStrategyLCNode  *dLQostrategyLCNode_p = PNULL;
	DLUESCellStartegyNode* dLUESCellStartegyNode_p = PNULL;
	DlLcQosInfo          *dlLcQosInfo_p        = PNULL;
	UInt64 bitrate = 0;
	/* SPS_CHG */
	UInt8 qciIndex = 0, index = 0;;
	UInt8 scellId = 1;
    /* +- SPR 17777 */
	/* SPR 11115 Fix Start */
#ifdef FDD_CONFIG
	UInt16 numOfTicksPerSecond = NUM_TICKS_PER_SECOND;
#else
	UInt16 numOfTicksPerSecond = getNumTicksPerSecond(DL, internalCellIndex);
#endif
	/* SPR 11115 Fix End */
	/* SPS_CHG */
	/* calling function verify that newUEContext_p is not NULL*/
	/* Also  the lcId between 0 -10 and logicalChCount should not be greater than
	   10 */
	InitParams *initParams_p = cellSpecificParams_g.\
				   cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
	for (lcIndex = 0; lcIndex < logicalChCount; lcIndex++)
	{
		tempLogicalChId = logicalChConfig_p[lcIndex].lchId;
		dlLogicalChannel_p = &newUEContext_p->logicalChannel[tempLogicalChId];
		dlLcQosInfo_p = &dlLogicalChannel_p->dlLcQosInfo;
		dlLogicalChannel_p->rlcMode = logicalChConfig_p[lcIndex].rlcMode;

		/*SPR 6621 Start*/
		dlLogicalChannel_p->queueLoad = 0;
                /*SPR 6621 End*/

		dlLogicalChannel_p->lastScheduledTick = 0;

		dlLogicalChannel_p->rlcSNFieldLength = 
			logicalChConfig_p[lcIndex].rlcSNFieldLength;

		if(5 == dlLogicalChannel_p->rlcSNFieldLength)
		{
			dlLcQosInfo_p->rlcFixedHeaderSize = 1;
		}
		else
		{
			dlLcQosInfo_p->rlcFixedHeaderSize = 2;
		}

		dlLogicalChannel_p->logicalChannelId = logicalChConfig_p[lcIndex].lchId;

		dlLogicalChannel_p->logicalChannelPriority = 
			logicalChConfig_p[lcIndex].lchPriority;

		/* SPR 3608 changes start */
		dlLogicalChannel_p->enbPSRReqFlag = logicalChConfig_p[lcIndex].enbPSRReqFlag;
		if(TRUE == logicalChConfig_p[lcIndex].enbPSRReqFlag)
		{
			newUEContext_p->enbPsrCount++;
		}
		/* SPR 3608 changes end */

		/* SPS_CHG */
		/*!
		 *\code
		 * Populating the ROHC profile in LC .
		 *\endcode
		 */
		dlLogicalChannel_p->rohcProfile = \
						  logicalChConfig_p[lcIndex].rohcProfile;
		/* SPS_CHG */
		/*Fill Qos Info*/


		if (dlLogicalChannel_p->logicalChannelId == 1 || 
				dlLogicalChannel_p->logicalChannelId == 2)
		{
			/*SRB1 or SRB2 is getting created */
			/* SPR 6430 changes start */
			dlLcQosInfo_p->srbWeight = 600;
			/* SPR 6430 changes end */
			dlLcQosInfo_p->QCI = 0;
			dlLcQosInfo_p->channelType = SRB;
			dlLcQosInfo_p->qciPriority          = 0;
			dlLcQosInfo_p->pktDlyBdgt           = 0;
			dlLcQosInfo_p->pktErrRate           = 0;
			dlLcQosInfo_p->totSchdTknInWindow   = 0;
			dlLcQosInfo_p->curTknWindowInd      = 0;
			dlLcQosInfo_p->strictSchedulingFlag = FALSE;
			dlLcQosInfo_p->scheduledToken[0]    = 0;
			dlLcQosInfo_p->scheduledToken[1]    = 0;
			dlLcQosInfo_p->scheduledToken[2]    = 0;
			dlLcQosInfo_p->scheduledToken[3]    = 0;
			dlLcQosInfo_p->scheduledToken[4]    = 0;
			dlLcQosInfo_p->scheduledToken[5]    = 0;
			dlLcQosInfo_p->scheduledToken[6]    = 0;
			dlLcQosInfo_p->scheduledToken[7]    = 0;
			dlLcQosInfo_p->scheduledToken[8]    = 0;
			dlLcQosInfo_p->scheduledToken[9]    = 0;
			dlLcQosInfo_p->GBR                  = 0;
			dlLcQosInfo_p->MBR                  = 0;
			dlLcQosInfo_p->availToken           = 0; 
			dlLcQosInfo_p->tokenDepth           = 0; 
			dlLcQosInfo_p->tokenPriority        = 0; 
			dlLcQosInfo_p->qciPriority          = 0; 
			dlLcQosInfo_p->pdbPriority          = 0; 
			dlLcQosInfo_p->qLoadPriority        = 0; 
			/*debug stats*/
			LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_SRB_COUNT(
					newUEContext_p->ueIndex);
		}
		else
		{
			dlLcQosInfo_p->srbWeight = 0;
			dlLcQosInfo_p->QCI = 
				logicalChConfig_p[lcIndex].QCI;

			dlLcQosInfo_p->priority = 
				macParams_g.qosQciInfo[dlLcQosInfo_p->QCI - 1].priority;

			dlLcQosInfo_p->qciPriority = 
				dlQosGetQciPriority(dlLcQosInfo_p->priority);

			dlLcQosInfo_p->pktDlyBdgt =
				macParams_g.qosQciInfo[dlLcQosInfo_p->QCI - 1].pktDlyBdgt;

			dlLcQosInfo_p->pktDlyBdgt = dlLcQosInfo_p->pktDlyBdgt - 25;

			dlLcQosInfo_p->pktErrRate =
				macParams_g.qosQciInfo[dlLcQosInfo_p->QCI - 1].pktErrLossRate;

			dlLcQosInfo_p->channelType = 
				logicalChConfig_p[lcIndex].channelType;

			/* SPR 6518 Changes Ends */

			dlLcQosInfo_p->totSchdTknInWindow   = 0;
			dlLcQosInfo_p->curTknWindowInd      = 0;
			dlLcQosInfo_p->strictSchedulingFlag = 0;
			dlLcQosInfo_p->scheduledToken[0]    = 0;
			dlLcQosInfo_p->scheduledToken[1]    = 0;
			dlLcQosInfo_p->scheduledToken[2]    = 0;
			dlLcQosInfo_p->scheduledToken[3]    = 0;
			dlLcQosInfo_p->scheduledToken[4]    = 0;
			dlLcQosInfo_p->scheduledToken[5]    = 0;
			dlLcQosInfo_p->scheduledToken[6]    = 0;
			dlLcQosInfo_p->scheduledToken[7]    = 0;
			dlLcQosInfo_p->scheduledToken[8]    = 0;
			dlLcQosInfo_p->scheduledToken[9]    = 0;
			/* SPS_CHG */
			/*! \code
			 * If LC is with QCI enable 
			 *    - Then mark LC As SPS LC in UE Context
			 *\endcode
			 */
			qciIndex = logicalChConfig_p[lcIndex].QCI - 1;
			if ((TRUE == initParams_p->spsSysWideParam.enableSps) &&
					(TRUE == initParams_p->spsSysWideParam.qciSpslist[qciIndex]))
			{
				if (TRUE == dlLogicalChannel_p->isSpsLc)
				{
					LOG_MAC_MSG( MAC_MAX_SPS_LC_PER_UE, LOGWARNING, MAC_RRC_INF,
							GET_CURRENT_TICK_FOR_CELL(internalCellIndex), __LINE__, lcIndex, qciIndex + 1,
							newUEContext_p->ueIndex, internalCellIndex, 0,0, __func__, "" );
					continue;
				}
				else
				{
					dlLogicalChannel_p->isSpsLc = TRUE;
					/* Store SPS LC Id */
					newUEContext_p->spsDlInfo.lcId = 
						logicalChConfig_p[lcIndex].lchId;
					/* Call State m/c for changing the SPS state after receving sps LC */
					if (newUEContext_p->spsDlInfo.spsState == SPS_IDLE &&
							newUEContext_p->spsDlInfo.requestType == SPS_SETUP)
					{
                        /* +- SPR 17777 */
						/* Call state machine for setting state to SPS_CONFIGURED  */
						if( MAC_FAILURE == dlStateMachine[newUEContext_p->spsDlInfo.\
								spsState][SPS_CONFIG_RECVD]( 
                                    newUEContext_p->ueIndex, 
                                    newUEContext_p->spsDlInfo.spsState,
                                    SPS_CONFIG_RECVD, newUEContext_p, PNULL 
                                    /* + SPS_TDD_Changes*/
#ifdef FDD_CONFIG
                                    ,PHY_DL_DELAY
#else
                                    ,DL_ADVANCE_TICK
#endif
                                    /* - SPS_TDD_Changes*/
                                    ,internalCellIndex ) )
						{
                            /* +- SPR 17777 */
                            LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                                 getCurrentTick(), __LINE__, newUEContext_p->ueIndex,newUEContext_p->spsDlInfo.spsState, SPS_CONFIG_RECVD,
                                 newUEContext_p->spsDlInfo.spsState, 0,0, __func__, "DlStateMachine");
                            /* +- SPR 17777 */
							return MAC_FAILURE;
						}
                        /* +- SPR 17777 */
                        LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                                 getCurrentTick(), __LINE__, newUEContext_p->ueIndex,newUEContext_p->spsDlInfo.spsState, SPS_CONFIG_RECVD,
                                 newUEContext_p->spsDlInfo.spsState, 0,0, __func__, "DlStateMachine");
					} 
				}
			}
			else
			{
				dlLogicalChannel_p->isSpsLc = FALSE;
			}
			/* SPS_CHG */
		}

		if(dlLcQosInfo_p->channelType == QOS_GBR)
		{
			LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_GBR_COUNT(
					newUEContext_p->ueIndex);

			dlLcQosInfo_p->GBR = 
				bitrate = logicalChConfig_p[lcIndex].GBR;

			dlLcQosInfo_p->MBR = 
				logicalChConfig_p[lcIndex].MBR;

			/*Calculate GBR LC token value*/
			dlQosCalcGbrLCTknRate(&dlLogicalChannel_p->dlLcQosInfo,internalCellIndex);

			dlLcQosInfo_p->availToken = 
				dlLcQosInfo_p->tokenRate;

			/*Calculate bucket depth*/
			dlLcQosInfo_p->tokenDepth = 
				initParams_p->dlSchdConfig.gbrBktFactor * 
				dlLcQosInfo_p->tokenRate;

			/* DESIGN REVIEW CHANGES INCORPORATION START */
			/* Calculate bucket limit(minimum tokens to present in bucket) */
			dlLcQosInfo_p->tokenLimit = 
				initParams_p->dlSchdConfig.gbrTokenLimitFactor * dlLcQosInfo_p->tokenRate;

			/* DESIGN REVIEW CHANGES INCORPORATION END */

			dlQosUpdateTknTableForLcPriority (
					dlLcQosInfo_p->lcTknTable, 
					dlLcQosInfo_p->GBR,
					initParams_p->dlSchdConfig.dlTokenPeriodicity);

			/* update the PDB and Qload priority tables*/
			dlQosUpdatePdbTableForLcPriority (
					dlLcQosInfo_p->lcPdbTable,
					dlLcQosInfo_p->pktDlyBdgt);

			dlQosUpdateQloadTableForLcPriority (
					dlLcQosInfo_p->lcQlTable, 
					bitrate,
					dlLcQosInfo_p->pktDlyBdgt);

			/* QOS_UT*/
			newUEContext_p->gbrLcInfo[newUEContext_p->gbrLcCount] = dlLogicalChannel_p->logicalChannelId;
			newUEContext_p->gbrLcCount++;
			/*START : SPS : DRX_MAC_CE*/
			if(dlLogicalChannel_p->isSpsLc == TRUE)
			{
				newUEContext_p->gbrSPSLcCount++;
			}
			/*END   : SPS : DRX_MAC_CE*/
			/* CA changes Start */
			/* + SPR 11917 Changes */
			/* Fill default value bandWidthDistribution if tag is not present */
			/* if (TRUE == logicalChConfig_p[lcIndex].isBandWidthTagPresent)*/
			/* - SPR 11917 Changes */
			{
				/* Currently only 1 scell with scellIndex 1 is expected */
				for (index = PCELL_SERV_CELL_IX; index <= MAX_NUM_SCELL; 
						index++)
				{      
					dlLcQosInfo_p->bandWidthDistribution[index] = 
						newUEContext_p->bandWidthDistribution[index];
				}
			}
			/* CA changes End */

			/* + SPR 11917 Changes */
			updateDlAggregateGbrPerTTI(newUEContext_p, dlLcQosInfo_p ,ADD_DL_AGG_GBR_TRIGGER, numOfTicksPerSecond);
			/* - SPR 11917 Changes */

		}
		else if(dlLcQosInfo_p->channelType == NON_GBR)
		{
			/* SPR 9203 Fix Starts*/
			dlLcQosInfo_p->GBR = 0;
			/* SPR 9203 Fix Ends*/
			/*debug stats*/
			LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_NGBR_COUNT(
					newUEContext_p->ueIndex);

			dlLcQosInfo_p->tokenPriority = 
				newUEContext_p->dlAmbrInfo.ambrTokenPriority;

			bitrate = newUEContext_p->dlAmbrInfo.AMBR;

			/* update the PDB and Qload priority tables*/
			dlQosUpdatePdbTableForLcPriority (
					dlLcQosInfo_p->lcPdbTable,
					dlLcQosInfo_p->pktDlyBdgt);

			dlQosUpdateQloadTableForLcPriority (
					dlLcQosInfo_p->lcQlTable, 
					bitrate,
					dlLcQosInfo_p->pktDlyBdgt);
			/* QOS_UT */
			newUEContext_p->nonGbrLcInfo[newUEContext_p->nonGbrLcCount] = dlLogicalChannel_p->logicalChannelId;
			newUEContext_p->nonGbrLcCount++;
			/*START : SPS : DRX_MAC_CE*/
			if(dlLogicalChannel_p->isSpsLc == TRUE)
			{
				newUEContext_p->nonGbrSPSLcCount++;
			}
			/*END   : SPS : DRX_MAC_CE*/
		}
		/* SPR# 4287 Changes Starts */
		if(DL_QOS_BASED_SCHEDULING == initParams_p->dlSchdConfig.dlSchedulerStrategy)
		{
			/* SPR# 4287 Changes Ends */

			/*Create DLQosStrategyLCNode*/ 
			GET_MEM_FROM_POOL(DLQosStrategyLCNode,dLQostrategyLCNode_p,
					sizeof(DLQosStrategyLCNode), PNULL);

			/* Rel 5.3: Coverity Fix Start */
			if (PNULL == dLQostrategyLCNode_p)
			{
				/* Not able to allocate Memory */
				return MAC_FAILURE;
			}
			/* Rel 5.3: Coverity Fix End */
			/* SPR 11109 FIX START - 22-MAY-2014 */
			memSet(dLQostrategyLCNode_p, 0 , sizeof(DLQosStrategyLCNode));
			/* SPR 11109 FIX END - 22-MAY-2014 */


			/*Fill DLQosStrategyLCNode*/
			dLQostrategyLCNode_p->ueIndex = newUEContext_p->ueIndex;
			dLQostrategyLCNode_p->lcId = logicalChConfig_p[lcIndex].lchId;
			dLQostrategyLCNode_p->lcInfo_p = dlLogicalChannel_p;
			/*Store strategy node in DL LC info*/
			dlLcQosInfo_p->dlQosStrategyLCNode_p = dLQostrategyLCNode_p;

			if(IS_CA_ENABLED())
			{
				for (scellId = MAX_NUM_PCELL; scellId <= MAX_NUM_SCELL; scellId++)
				{
					/* Create Scell StrategyLCNode */
					GET_MEM_FROM_POOL(DLUESCellStartegyNode, dLUESCellStartegyNode_p,
							sizeof(DLUESCellStartegyNode), PNULL);
					if(PNULL == dLUESCellStartegyNode_p)
					{
						/* Not able to allocate Memory */
						return MAC_FAILURE;
					}
					/* Fill DLUESCellStartegyNode */
					dLUESCellStartegyNode_p->internalCellIndex = internalCellIndex;
					dLUESCellStartegyNode_p->schedulableTokensScell = 0; 
					dLUESCellStartegyNode_p->schedulableBytesScell = 0;

					dlLcQosInfo_p->dlQosStrategyLCNode_p->dLUESCellStartegyNode_p[scellId] = 
						dLUESCellStartegyNode_p;
				}
			}
			/*debug stats*/
			LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO(
					newUEContext_p->ueIndex,
					dlLogicalChannel_p->logicalChannelId,
					dlLcQosInfo_p->QCI,
					dlLogicalChannel_p->rlcMode,
					bitrate,
					dlLcQosInfo_p->tokenDepth,
					dlLcQosInfo_p->tokenRate);
		}

		if ( newUEContext_p->maxLcId == INVALID_LCID )
		{
			newUEContext_p->maxLcId = tempLogicalChId ;
		}
		else if ( newUEContext_p->maxLcId < tempLogicalChId )
		{
			newUEContext_p->maxLcId = tempLogicalChId;
		}
		logicalChConfig_p[lcIndex].result = MAC_SUCCESS;
	}
	LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"%s Exit.\n",__func__);

	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
	return MAC_SUCCESS;    
}

/****************************************************************************
 * Function Name  : reconfigureMACDLLogicalChannel
 * Inputs         : newUEcontext_p- 
 *                  logicalChConfig_p - A pointer to the LogicalChannelConfig
 *                                      array containing the various
 *                                      configuration parameters for the 
 *                                      logical channel.
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                 
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This API is used to reconfigure a logical channel as 
 *                  per the configuration for a given UE.
 *                  If a UE does not exist, for which this API is invoked or 
 *                  the logical channel does not exist,then return MAC_FAILURE.
 ****************************************************************************/

MacRetType reconfigureMACDLLogicalChannel (DLUEContext* newUEcontext_p,
		LogicalChannelConfigInfo* logicalChConfig_p
		,InternalCellIndex internalCellIndex)
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	DLLogicalChannelInfo *logicalChannel_p = PNULL;
	DlLcQosInfo    *dlLcQosInfo_p = PNULL;
	DLSchedulerStrategyConfig *dlSchdConfig_p = & cellSpecificParams_g.\
						    cellConfigAndInitParams_p[internalCellIndex]->initParams_p->dlSchdConfig;
	/* Temporary variable for Logical Channel Id to be reconfigured */
	UInt8 tempLogicalChId = 0, index = 0;
	/* SPR 11115 Fix Start */
#ifdef FDD_CONFIG
	UInt16 numOfTicksPerSecond = NUM_TICKS_PER_SECOND;
#else
	UInt16 numOfTicksPerSecond = getNumTicksPerSecond(DL, internalCellIndex);
#endif
	/* SPR 11115 Fix End */

	tempLogicalChId = logicalChConfig_p->lchId;

	if ( INVALID_LCID == newUEcontext_p->
			logicalChannel[tempLogicalChId].logicalChannelId)
	{
		lteWarning ("The Logical Channel requested to be reconfigured \
				has not been added yet. \n");
		logicalChConfig_p->result = MAC_RRC_LOG_CH_NOT_EXISTS;    
		return MAC_FAILURE;
	}
	logicalChannel_p = &newUEcontext_p->logicalChannel[tempLogicalChId];

	/* SPS_CHG */
	/*!
	 *\code
	 * If isRohcProfilePresent is TRUE then Update it in ue context
	 *\endcode
	 */
	if (TRUE == logicalChConfig_p->isRohcProfilePresent)
	{
		logicalChannel_p->rohcProfile = logicalChConfig_p->rohcProfile;
	}
	/* SPS_CHG */


	dlLcQosInfo_p = &logicalChannel_p->dlLcQosInfo;
	if (QOS_GBR == dlLcQosInfo_p->channelType)
	{
		/* DESIGN REVIEW CHANGES INCORPORATION START */
		updateDlAggregateGbrPerTTI(newUEcontext_p, dlLcQosInfo_p, DELETE_DL_AGG_GBR_TRIGGER, numOfTicksPerSecond);
		/* DESIGN REVIEW CHANGES INCORPORATION END */

		/* Reconfigure Lc changes for Qos */

		dlLcQosInfo_p->GBR = logicalChConfig_p->GBR;
		dlLcQosInfo_p->MBR = logicalChConfig_p->MBR;

		/* CA changes Start */
		if ( TRUE == logicalChConfig_p->isBandWidthTagPresent)
		{
			for (index = PCELL_SERV_CELL_IX; index <= MAX_NUM_SCELL; 
					index++)
			{
				dlLcQosInfo_p->bandWidthDistribution[index] = 
					logicalChConfig_p->bandWidthDistribution[index];
			}
		}
		/* CA changes End */

		/*Calculate GBR LC token value*/
		dlQosCalcGbrLCTknRate(dlLcQosInfo_p,internalCellIndex);

		dlLcQosInfo_p->availToken =
			dlLcQosInfo_p->tokenRate;

		/*Calculate bucket depth*/
		dlLcQosInfo_p->tokenDepth = 
			dlSchdConfig_p->gbrBktFactor * 
			dlLcQosInfo_p->tokenRate;

		/* Calculate bucket limit(minimum tokens to present in bucket) */
		dlLcQosInfo_p->tokenLimit = 
			dlSchdConfig_p->gbrTokenLimitFactor * dlLcQosInfo_p->tokenRate;

		dlQosUpdateTknTableForLcPriority (
				dlLcQosInfo_p->lcTknTable, 
				dlLcQosInfo_p->GBR,
				dlSchdConfig_p->dlTokenPeriodicity);

		dlQosUpdateQloadTableForLcPriority (
				dlLcQosInfo_p->lcQlTable, 
				dlLcQosInfo_p->GBR,
				dlLcQosInfo_p->pktDlyBdgt);

		/* Reconfigure Lc changes for Qos */

		/* DESIGN REVIEW CHANGES INCORPORATION START */
		updateDlAggregateGbrPerTTI(newUEcontext_p, dlLcQosInfo_p, ADD_DL_AGG_GBR_TRIGGER, numOfTicksPerSecond);
		/* DESIGN REVIEW CHANGES INCORPORATION END */

	}
	logicalChConfig_p->result = MAC_SUCCESS;
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
	return MAC_SUCCESS;    
}
/* SS_S2 */
/*****************************************************************************
 * Function Name  : getPeriodicReportingMode 
 * Inputs         : transMode  - trasmission mode as received from RRC
 *                  periodicFeedBackType -  wideband/ subband 
 *                  pmiRiReportV920
 * Outputs        : None
 * Returns        : transmission mode
 * Description    : This function returns the periodic transmission mode
 *                  Refer table 7.2.2-1 of 36.213  
 *****************************************************************************/
PeriodicReportingModes getPeriodicReportingMode
(UInt8 transMode, PeriodicFeedbackType periodicFeedBackType,
 /* + TM7_8 Changes Start */
 UInt8 pmiRiReportV920)
/* - TM7_8 Changes End */
{
	LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"Calculating periodic mode for "\
			" Transmission mode = %d, report type = %d\n", transMode, 
			periodicFeedBackType);
	/* SPR 545 Fix Starts */    
	if(TX_MODE_1 == transMode || TX_MODE_2 == transMode || 
			TX_MODE_3 == transMode || TX_MODE_7 == transMode || TX_MODE_ATTACH_NOT_COMPLETE == transMode ||
			/* + TM7_8 Changes Start */
			((TX_MODE_8 == transMode) && (TRUE != pmiRiReportV920)))
		/* - TM7_8 Changes End */
		/* SPR 545 Fix Ends */
	{
		if(PERIODIC_FEEDBACK_WIDEBAND == periodicFeedBackType)
		{
			LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"Periodic Mode calulated as "
					"MODE_1_0");
			return PERIODIC_MODE_1_0; 
		}
		else
		{
			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"Periodic Mode calulated as "\
					"MODE_2_0");
			return PERIODIC_MODE_2_0;
		}
	}
	else if (TX_MODE_4 == transMode || TX_MODE_5 == 
			transMode || TX_MODE_6 == transMode || 
			/* + TM7_8 Changes Start */
			((TX_MODE_8 == transMode)&&(TRUE == pmiRiReportV920)))
		/* - TM7_8 Changes End */
	{
		if(PERIODIC_FEEDBACK_WIDEBAND == periodicFeedBackType)
		{
			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"Periodic Mode calulated as "\
					"MODE_1_1");
			return PERIODIC_MODE_1_1;
		}
		else
		{
			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"Periodic Mode calulated as "\
					"MODE_2_1");
			return PERIODIC_MODE_2_1;
		}
	}
	else
	{
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"Invalid transmission mode %u",
				transMode);
	}
	/*+COVERITY 5.3.0 - 32259*/
	return PERIODIC_MODE_INVALID;
	/*-COVERITY 5.3.0 - 32259*/
}

/*****************************************************************************
 * Function Name  : calculateReportFormatBitWidthPeriodic 
 * Inputs         : feedbackType - type of the feedback
 *                  ri - value of RI   
 *                  ueCategory - value of UE Category
 *                  transMode - value of Trasmission Mode   
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : Bitwith if the report
 * Description    : This function shall calculate and return the bitwidth of the
 *                  report that is expected from the UE (Periodic reports)
 *                  Refer table 7.2.2-3 from 36.213 for bitwidths
 *****************************************************************************/
 UInt8 calculateReportFormatBitWidthPeriodic(ueExpectedReportType feedbackType, 
		/* TM8_4*2_Mimo */
		UInt8 ri,UInt8 ueCategory, UInt8 transMode,
		InternalCellIndex internalCellIndex)
/* TM8_4*2_Mimo */
{
	/* CA Changes start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */
	LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"Calculating Bitwidth for "\
			"feedbackType = %d, RI = %u\n", feedbackType, ri);

	switch(feedbackType)
	{
		case PERIODIC_MODE_1_0_TYPE_4:
		case PERIODIC_MODE_2_0_TYPE_4:
			return PERIODIC_MODE_10_20_TYPE_4_BITS;
			break;

		case PERIODIC_MODE_1_0_TYPE_3:
		case PERIODIC_MODE_1_1_TYPE_3:
		case PERIODIC_MODE_2_0_TYPE_3:
		case PERIODIC_MODE_2_1_TYPE_3:
			/* 4x2 MIMO S */
			/* number of layers can be 4 for TM3/4 only */
			/* TM8_4*2_Mimo */
			if ( (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas) &&
					((TX_MODE_3 == transMode) || (TX_MODE_4 == transMode)) &&
					(UE_CATEGORY_5 == ueCategory))
				/* TM8_4*2_Mimo */
			{
				return PERIODIC_MODE_ALL_TYPE_3_FOR_4_ANT_BITS;
			}
			else 
				return PERIODIC_MODE_ALL_TYPE_3_BITS;
			/* 4x2 MIMO E */
			break;

		case PERIODIC_MODE_1_1_TYPE_2:
		case PERIODIC_MODE_2_1_TYPE_2:
			if (1 == ri)
			{
				if (NUM_OF_TX_ANTENNAS_2 == cellParams_p->numOfTxAnteenas)
				{
					return PERIODIC_MODE_11_21_TYPE_2_RI_1_BITS;
				}
				else if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
				{
					return PERIODIC_MODE_11_21_TYPE_2_RI_1_4_ANT_BITS;
				}
				else
				{
					/* This should not happen, currently setting it to 8 bits */
					return PERIODIC_MODE_11_21_TYPE_2_RI_1_BITS;
				}
			}
			else if (1 < ri)
			{
				if (NUM_OF_TX_ANTENNAS_2 == cellParams_p->numOfTxAnteenas)
				{
					return PERIODIC_MODE_11_21_TYPE_2_RI_GT_1_BITS;
				}
				else if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
				{
					return PERIODIC_MODE_11_21_TYPE_2_RI_GT_1_4_ANT_BITS;
				}
				else
				{
					return PERIODIC_MODE_11_21_TYPE_2_RI_GT_1_BITS;
				}
			}
			else
			{
				return PERIODIC_MODE_11_21_TYPE_2_RI_1_BITS;
			}
			break;

		case PERIODIC_MODE_2_0_TYPE_1:
			return PERIODIC_MODE_20_21_TYPE_1_RI_1_BITS(cellParams_p); 
			break;

		case PERIODIC_MODE_2_1_TYPE_1:
			if(1 == ri)
			{
				return PERIODIC_MODE_20_21_TYPE_1_RI_1_BITS(cellParams_p); 
			}
			else if(1 < ri)
			{
				return PERIODIC_MODE_20_21_TYPE_1_RI_GT_1_BITS(cellParams_p); 
			}
			else
			{
				/* RI is 0. This should not happen. Setting it to  
				 * PERIODIC_MODE_20_21_TYPE_1_RI_1_BITS */
				lteWarning("Invalid RI Value %d\n", ri);
				return PERIODIC_MODE_20_21_TYPE_1_RI_1_BITS(cellParams_p); 
			}
			break;

		default:
			lteWarning("Invalid Periodic feedback type %d\n", feedbackType);
			break;
	}

	return 0;
}

/***************************************************************************
 * Function Name  : calculateReportFormatBitWidthAperiodic 
 * Inputs         : feedBack_p -
 *                  cqiMode - APeriodic CQI Mode   
 *                  ri - value of RI   
 *                  transMode - value of Trasmission Mode   
 *                  riBits_p -
 *                  bitWidthForRIGT_1 -
 *                  ueCategory - value of UE Category
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : bitwidth for RI and CQI/PMI if RI>1
 * Returns        : Bitwith of the report if RI=1
 * Description    : This function shall calculate and return the bitwidth of the
 *                  report that is expected from the UE (Aperiodic reports)
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129191) */            
/* + CQI_5.0 */
UInt8 calculateReportFormatBitWidthAperiodic
/* SPR 5599 changes end (ZIP ID 129191) */            
/* +- SPR 17777 */
(CQIMode cqiMode, ueExpectedReportType *feedBack_p, UInt8 transMode,
 UInt8 *riBits_p,UInt8 *bitWidthForRIGT_1,UInt8 ueCategory,
 InternalCellIndex internalCellIndex)
{
	UInt8  bitWidthForRI_1 = 0;
	/* CA Changes start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */
	switch(cqiMode)
	{
		case CQI_MODE_1_2:

			*feedBack_p = APERIODIC_MODE_1_2;
			/* 4x2 MIMO S */
			if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
			{
				bitWidthForRI_1 = APERIODIC_MODE_12_RI_1_FOR_4_ANT_BITS(cellParams_p);
				*bitWidthForRIGT_1 = APERIODIC_MODE_12_RI_GT_1_FOR_4_ANT_BITS(cellParams_p);
			}
			/* 4x2 MIMO E */
			else
			{
				bitWidthForRI_1 = APERIODIC_MODE_12_RI_1_BITS(cellParams_p);
				/* + CL-MIMO LJA*/
				/*bitWidth = APERIODIC_MODE_12_RI_1_BITS;*/
				*bitWidthForRIGT_1 = APERIODIC_MODE_12_RI_2_BITS(cellParams_p);
				/* - CL-MIMO LJA*/
			}
			/* + TM7_8 Changes Start */
			/* 4x2 MIMO S */
			/* TM8_4*2_Mimo */
			if ((TX_MODE_4 == transMode) &&
					(NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas) &&
					(UE_CATEGORY_5 == ueCategory))
				/* - TM7_8 Changes End */
			{
				*riBits_p = RI_BIT_FOR_4_LAYER;
			}
			else if ((TX_MODE_4 == transMode) || (TX_MODE_8 == transMode))
			{
				/* TM8_4*2_Mimo */
				*riBits_p = RI_BIT_FOR_2_LAYER; /* Considering 2 antenna ports */
				/* 4x2 MIMO E */
			}
			break;

		case CQI_MODE_2_0:

			*feedBack_p = APERIODIC_MODE_2_0; 
			bitWidthForRI_1 = APERIODIC_MODE_20_BITS(cellParams_p);
			*bitWidthForRIGT_1 = APERIODIC_MODE_20_BITS(cellParams_p);

			if (TX_MODE_3 == transMode) 
			{
				/* 4x2 MIMO S */
				/* SPR 11355 Fix Start */
				if ( (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas) &&
						(UE_CAT_5 == ueCategory || ueCategory == UE_CAT_8) )
					/* SPR 11355 Fix End */
				{
					*riBits_p = RI_BIT_FOR_4_LAYER;
				}
				else 
					*riBits_p = RI_BIT_FOR_2_LAYER; /* Considering 2 antenna ports */
				/* 4x2 MIMO E */
			}
			break;

		case CQI_MODE_2_2:

			*feedBack_p = APERIODIC_MODE_2_2; 
			/* 4x2 MIMO S */
			if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
			{
				bitWidthForRI_1 = APERIODIC_MODE_22_RI_1_FOR_4_ANT_BITS(cellParams_p);
				*bitWidthForRIGT_1 = APERIODIC_MODE_22_RI_GT_1_FOR_4_ANT_BITS(cellParams_p);
			}
			/* 4x2 MIMO E */
			else
			{
				bitWidthForRI_1 = APERIODIC_MODE_22_RI_1_BITS(cellParams_p);
				*bitWidthForRIGT_1 = APERIODIC_MODE_22_RI_2_BITS(cellParams_p);
			}
			/* + TM7_8 Changes Start */
			/* 4x2 MIMO S */
			/* TM8_4*2_Mimo */
			if ((TX_MODE_4 == transMode) &&
					(NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas) &&
					(UE_CATEGORY_5 == ueCategory))
				/* - TM7_8 Changes End */

			{
				*riBits_p = RI_BIT_FOR_4_LAYER;
			}
			else if ((TX_MODE_4 == transMode) || (TX_MODE_8 == transMode))
			{
				/* TM8_4*2_Mimo */
				*riBits_p = RI_BIT_FOR_2_LAYER; /* Considering 2 antenna ports */
				/* 4x2 MIMO E */
			}
			break;

		case CQI_MODE_3_0:

			*feedBack_p = APERIODIC_MODE_3_0; 
			bitWidthForRI_1 = APERIODIC_MODE_30_BITS(cellParams_p);
			*bitWidthForRIGT_1 = APERIODIC_MODE_30_BITS(cellParams_p);

			if (TX_MODE_3 == transMode) 
			{
				/* 4x2 MIMO S */
				/* SPR 11355 Fix Start */
				if ( (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas) &&
						(UE_CAT_5 == ueCategory || ueCategory == UE_CAT_8) )
					/* SPR 11355 Fix End */
				{
					*riBits_p = RI_BIT_FOR_4_LAYER;
				}
				else 
					*riBits_p = RI_BIT_FOR_2_LAYER; /* Considering 2 antenna ports */
				/* 4x2 MIMO E */
			}
			break;

		case CQI_MODE_3_1:
			/* 4x2 MIMO S */
			if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
			{
				bitWidthForRI_1 = APERIODIC_MODE_31_RI_1_FOR_4_ANT_BITS(cellParams_p);
				*bitWidthForRIGT_1 = APERIODIC_MODE_31_RI_GT_1_FOR_4_ANT_BITS(cellParams_p);
			}
			/* 4x2 MIMO E */
			else
			{
				/* Assumed 2 antenna ports */
				bitWidthForRI_1 = APERIODIC_MODE_31_RI_1_BITS(cellParams_p);
				*bitWidthForRIGT_1 = APERIODIC_MODE_31_RI_2_BITS(cellParams_p);
			}
			*feedBack_p = APERIODIC_MODE_3_1; 
			/* + TM7_8 Changes Start */
			/* 4x2 MIMO S */
			/* TM8_4*2_Mimo */
			if ((TX_MODE_4 == transMode) &&
					(NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas) &&
					(UE_CATEGORY_5 == ueCategory))
				/* - TM7_8 Changes End */
			{
				*riBits_p = RI_BIT_FOR_4_LAYER;
			}
			else if ((TX_MODE_4 == transMode) || (TX_MODE_8 == transMode))
			{ 
				/* TM8_4*2_Mimo */
				*riBits_p = RI_BIT_FOR_2_LAYER; /* Considering 2 antenna ports */
				/* 4x2 MIMO E */
			}
			break;

		default:
			break;
	}

	return bitWidthForRI_1;

}

/* - CQI_5.0 */
/* SPR 3271 changes start */
/******************************************************************************
 * Function Name : initPuschBlerContext 
 * Inputs        :  ueIndex
 *                  csiContext_p - CSI context o be filled
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs       : None 
 * Return        : None
 * Description   : Initializes Pusch Bler in UE's UL Context 
 *****************************************************************************/
STATIC void initPuschBlerContext(UInt16 ueIndex,
		CSIContext *csiContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
{    
	/* Initialize PUSCH BLER context*/
	csiContext_p->puschBlerInfo.puschBler = MAC_TARGET_BLER;
	/*ICIC change start */
	/*CA Changes start  */
	updateULBler(ueIndex,MAC_TARGET_BLER, internalCellIndex);
	/*CA Changes end  */
	/*ICIC change end */

	csiContext_p->puschBlerInfo.blerCalculated  = 0;
	csiContext_p->puschBlerInfo.blerCalculatedCounter  = 0;
	csiContext_p->puschBlerInfo.lastBlerCalculatedCounter  = 0;
	csiContext_p->puschBlerInfo.sinrCorrectionFactor  = 0;
}

/******************************************************************************
 * Function Name : initMcsToPrbInfoInUEContext 
 * Inputs        : csiContext_p - CSI context o be filled 
 *                 internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs       : None
 * Return        : None
 * Description   : Initializes McsToPrb Info in UE's UL Context 
 *****************************************************************************/
/*CA Changes start  */
STATIC void initMcsToPrbInfoInUEContext(CSIContext *csiContext_p, 
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
{    
	UInt8 prbMapValue = 0;
	MCSToPrbMapInfo *mcsToPrbInfo = PNULL;
	UInt8 iter = 0;
	UInt32 n    = 0;    

	memSet(csiContext_p->mcsToPrbMap_p, \
			0, \
			sizeof (MCSToPrbMapInfo)*MAX_MCS_INDEX);

	/* ULA_UTP */
#define MAC_PUCCH_RESOURCES (3)
	/*CA Changes start  */
	prbMapValue                         = 
		cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
		cellParams_p->ulAvailableRBs - \
		(MAC_PUCCH_RESOURCES<<1);
	/*CA Changes end  */
	/* ULA_UTP */
	mcsToPrbInfo                        = &csiContext_p->mcsToPrbMap_p[0];

	mcsToPrbInfo->startIdx              = MAC_PUCCH_RESOURCES ;/*0; ULA_UTP */
	mcsToPrbInfo->endIdx                = prbMapValue + MAC_PUCCH_RESOURCES - 1;
	mcsToPrbInfo->maxContiguous         = prbMapValue;
	mcsToPrbInfo->maxContiguousStartIdx = 0;

	for(iter = mcsToPrbInfo->startIdx; iter <= mcsToPrbInfo->endIdx;iter++, prbMapValue--)
	{
		mcsToPrbInfo->prbMap[iter] = prbMapValue;
	}
	for(n = 1; n <= csiContext_p->highestReportedMCS; n++)
	{
		csiContext_p->mcsToPrbMap_p[n] = csiContext_p->mcsToPrbMap_p[n-1];
	}
}  

/******************************************************************************
 * Function Name : initPrbToMcsInfoInUEContext 
 * Inputs        : CSIContext *csiContext_p
 *                 minMcsIndex,initSINRValue 
 *                 internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs       : None 
 * Return        : None
 * Description   : Initializes PrbToMcs Info in UE's UL Context 
 *****************************************************************************/
/*CA Changes start  */
STATIC void initPrbToMcsInfoInUEContext(CSIContext *csiContext_p, 
		UInt8 minMcsIndex,
		UInt8 initSINRValue, 
		InternalCellIndex internalCellIndex)
{
	UInt32 n    = 0; 

	/* Initialize prbToMCSMap*/
	for(n = 0; n < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs ; n++)
	{
		csiContext_p->prbToMCSMapArray[n].effectiveMCS = \
								 minMcsIndex;
		csiContext_p->prbToMCSMapArray[n].reportedSINR = initSINRValue;    
	}
}
/*CA Changes end  */

/******************************************************************************
 * Function Name : initUEULCSIContext 
 * Inputs        : ULUEContext *ulUEContext_p:Pointer to UE's UL Context
 *                 minMcsIndex -
 *                 internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs       : None
 * Return        : None
 * Description   : Initializes UE's UL CSI Context 
 *****************************************************************************/
/*CA Changes start  */
void initUEULCSIContext(ULUEContext *ulUEContext_p, UInt8 minMcsIndex, InternalCellIndex internalCellIndex)
	/*CA Changes end  */
{
	CSIContext *csiContext_p = PNULL;
	csiContext_p = &ulUEContext_p->csiContext;
	csiContext_p->averageSINR = mcsSINRArr_g[minMcsIndex];
	/* ICIC changes start */
	/*CA Changes start  */
	updateULSINR(ulUEContext_p->ueIndex,csiContext_p->averageSINR, internalCellIndex);
	/*CA Changes end  */
	/* ICIC changes end */
	csiContext_p->highestReportedMCS = ulUEContext_p->mcsIndex ;
	csiContext_p->lowestReportedMCS  = ulUEContext_p->mcsIndex ;
	/*CA Changes start  */
	if (FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p\
			[internalCellIndex]->cellParams_p->freqSelectiveSchUL)
	{
		initMcsToPrbInfoInUEContext(csiContext_p, internalCellIndex);
	}
	else
	{
		csiContext_p->mcsToPrbMap_p = PNULL;
	}

	initPrbToMcsInfoInUEContext(csiContext_p,minMcsIndex,csiContext_p->\
			averageSINR, internalCellIndex);
	initPuschBlerContext(ulUEContext_p->ueIndex,csiContext_p, internalCellIndex);
	/*CA Changes end  */
	LOG_MAC_MSG(MAC_RRC_INF_UE_UL_CSI_INIT_INFO_ID, LOGDEBUG, MAC_RRC_INF, \
			csiContext_p->puschBlerInfo.puschBler,\
			ulUEContext_p->ueIndex, ulUEContext_p->mcsIndex, \
			csiContext_p->averageSINR, \
			/*CA Changes start  */
			(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
			 cellParams_p->ulAvailableRBs - (MAC_PUCCH_RESOURCES<<1)),\
			cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
			cellParams_p->freqSelectiveSchUL, \
			/*CA Changes end  */
			0, 0, \
			__func__,"UE_UL_CSI_INIT_INFO"); 
}
/* SPR 3271 changes end */

/* SS_S2 */

/* EICIC +*/

/****************************************************************************
 * Function Name  : updateCsiMeasurementSubset
 * Inputs         : U8 *MeasurementSubset1
 *U8 *MeasurementSubset2
 * Outputs        : None
 * Returns        : None
 * Description    : This API converts 40bit measurementsubset information received from RRC tlo 40 byte
 * and saves it is UPLINK ms context.
 ****************************************************************************/

/* +- SPR 17777 */
void updateCsiMeasurementSubset(UInt8 MeasurementSubset1[], UInt8 MeasurementSubset2[]
#ifdef TDD_CONFIG
        ,InternalCellIndex internalCellIndex
#endif
        )
/* +- SPR 17777 */
{
	UInt8 j_loop  = 0;
	UInt8 numBitsCsiPattern;

#ifdef FDD_CONFIG
	numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
#else
	if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
			[internalCellIndex]->cellParams_p->subFrameAssign)
	{
		numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
	}
	else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
			[internalCellIndex]->cellParams_p->subFrameAssign)
	{
		numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
	}
	else
	{
		numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
	}
#endif

	for (j_loop = 0 ; j_loop < numBitsCsiPattern ; j_loop++)
	{

		if ((MeasurementSubset2[(j_loop/NUMBER_OF_BITS_IN_1BYTE)] & (0x80 >> (j_loop%NUMBER_OF_BITS_IN_1BYTE) )) == LTE_FALSE)
		{              

			MeasurementSubset1[j_loop] = LTE_FALSE;

		}
		else
		{
			MeasurementSubset1[j_loop] = LTE_TRUE;

		}

	}

}

/* EICIC -*/

/*SPR 21103 Start*/
/****************************************************************************
 * Function Name  : deleteSRSAndPucchConfigAtCreatMACUEFailure 
 * Inputs         : ueULContext_p - Pointer to ULUEContext 
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                  scellCount - Count of the scell for which PUCCH nodes are
 *                  configured
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Delete the configured SRS and PUCCH
 *                  nodes if the createMacUeEntity fails.
 ****************************************************************************/
STATIC  void deleteSRSAndPucchConfigAtCreatMACUEFailure(
        ULUEContext *ueULContext_p, 
        InternalCellIndex internalCellIndex,
        UInt8   scellCount
        )
{
#ifndef TDD_CONFIG
    UInt8 sCellIndex = 0;
    
    if(PNULL != &ueULContext_p->pucchConfigInfo)
    {
        deletePeriodicReportsNode(&ueULContext_p->pucchConfigInfo, internalCellIndex);
    }
    for(sCellIndex = 1; sCellIndex <= scellCount; sCellIndex++)
    {
        if(PNULL != &ueULContext_p->ulUeScellContext_p[sCellIndex]->scellpucchConfigInfo)
        {
            deletePeriodicReportsNode(&ueULContext_p->ulUeScellContext_p[sCellIndex]->scellpucchConfigInfo,
                    internalCellIndex);
        }
    }
#else
    if(SHORT_SR_PERIODICITY == ueULContext_p->pucchConfigInfo.srPeriodicity)
    {
        deletePucchShortSRNodesForUE(&ueULContext_p->pucchConfigInfo,PNULL, internalCellIndex);
    }
    else
    {
        deletePCellPucchNodesForUE(&ueULContext_p->pucchConfigInfo,SCHEDULING_REQUEST, internalCellIndex);
    }
#endif
	
    if(SRS_DEDICATED_CONFIG_TYPE_SETUP == 
			(ueULContext_p->srsConfigInfo.srsConfigType))
	{
		deleteDedicatedSRSConfig(&ueULContext_p->srsConfigInfo,PNULL,internalCellIndex);
	}

}
/*SPR 21103 End*/
#ifdef TDD_CONFIG

/****************************************************************************
 * Function Name  : createMACUEEntity
 * Inputs         : dwlkUEInfo_p - Pointer to ueContextForDownlink required
 *                                 to configure MAC DL UE Context.
 *                  uplkUEInfo_p - Pointer to ueContextForUplink required
 *                                 to configure MAC UL UE Context.
 *                  uplkLCCount - Number of LCs configured at MAC UL.
 *                  transactionId - transaction Id received in request from RRC
 *                  createULUEBitMap_p -  pointer UL BIT map to be used to update 
 *                                       UL changes in passive context and HP thread 
 *                                       to swap context
 *                  createDLUEBitMap_p - pointer DL BIT map to be used to update 
 *                                       DL changes in passive context and HP thread 
 *                                        to swap context
 *                  currSFN - current System frame number
 *                  currSF - current Subframe number
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCESS or MAC_FAILURE
 * Description    : This API takes pointers to MAC & RLC Info required to 
 *                  configure UE context at Layer2. It returns  SUCCESS
 *                  and ueIdx allocated for UE will be stored in the 
 *                  dwlkUEInfo_p passed as an input arguement.
 ****************************************************************************/
MacRetType createMACUEEntity( UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink*  uplkUEInfo_p,
		UInt32 uplkLCCount,
		/* +- SPR 17777 */
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		, UInt32 *createULUEBitMap_p,
        /*SPR 16855 +-*/
		UInt32 *createDLUEBitMap_p,
		UInt32 currSFN, UInt32 currSF
		) 
/* + coverity 55307 */
{
	/* + coverity 32260 */
	MacRetType  resp   = MAC_FAILURE;
	MacRetType  ulResp = MAC_SUCCESS;
	MacRetType  dlResp = MAC_SUCCESS;
	/* - coverity 32260 */

	UInt32 ueIndex= 0;
	DLUEContext *newDLUEContext_p = PNULL;
	ULUEContext *newULUEContext_p = PNULL;
	RNTIInfo *rntiInfo_p        = PNULL;
	/* TA_CHG */
	TempCRNTICtx *tempCRNTICtx_p = PNULL;
	/* TA_CHG */
	UInt8 absLoop = 0; /* EICIC  - TDD +-*/

    /*SPR 16855 +-*/
	UInt8 scellCount = 0; 
	UInt32 createULUEBitMap = *createULUEBitMap_p;
	UInt32 createDLUEBitMap = *createDLUEBitMap_p;
	RRCRachInterfaceNode *rrcRachInterfaceNode_p = PNULL;
	UInt8 numBitsAbsPattern = 0; /* EICIC +-*/

	GET_MEM_FROM_POOL( RRCRachInterfaceNode,rrcRachInterfaceNode_p,sizeof (RRCRachInterfaceNode),PNULL);
    /*SPR 21103 Start*/
	if(PNULL == rrcRachInterfaceNode_p)
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"Memory Allocation failure for rrcRachInterfaceNode_p\n");
		return MAC_FAILURE;
	}
    /*SPR 21103 End*/
	/** CA-TDD Changes Start **/
    /*SPR 16855 +-*/

	/* Create UE Context at MAC Dwlk & Uplk */
	ueIndex = dwlkUEInfo_p->ueIndex;
	dlResp = createMACDLUEEntity(dwlkUEInfo_p, internalCellIndex);
	if (( MAC_SUCCESS == dlResp) || (MAC_LOG_CH_ERROR == dlResp))
	{
        /* + SPR 18841 Fix */
		rntiInfo_p = getUeIdxFromRNTIMap(dwlkUEInfo_p->cRnti, internalCellIndex);
		newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

		if ((PNULL == rntiInfo_p)||((FALSE == newDLUEContext_p->tcrntiFlag) &&
					(rntiInfo_p->index == INVALID_UE_INDEX)) || 
				(CRNTI == rntiInfo_p->rntiFlag))
		{
            LOG_MAC_ERROR(MAC_RRC_INF, "getUeIdxFromRNTIMap Failed: UE:%u\n",
                    ueIndex );
			deleteCreatedDownlinkUEContext(ueIndex,internalCellIndex);
            /*SPR 21103 Start*/
            freeMemPool(rrcRachInterfaceNode_p);
            /*SPR 21103 End*/
			return MAC_FAILURE;
		}
        /* - SPR 18841 Fix */
		ulResp = createMACULUEEntity(ueIndex, 
				uplkLCCount, uplkUEInfo_p,dwlkUEInfo_p, internalCellIndex);
		if ((MAC_FAILURE == ulResp) || (MAC_INTERNAL_ERROR == ulResp))
        {
            LOG_MAC_ERROR(MAC_RRC_INF, "Error creating MAC UL Entity: UE:%u\n",
                    ueIndex );
            /*SPR 7122 Fix start*/
            /* Deleting Complete Downlink context in case uplink context get fail*/
            deleteCreatedDownlinkUEContext(ueIndex,internalCellIndex);
            /*SPR 7122 fix End*/

        }
		else
		{
            /* SPR 20673 start */
			/* SPR 10683 changes start */
			/* addUEPeriodicReportBatchInfo(ueIndex,internalCellIndex);*/
			/* SPR 10683 changes end */
            /* SPR 20673 end   */

			/* Set DL, UL flags to 1 */
			resp = MAC_SUCCESS;
		}
	}

	if ( MAC_FAILURE != resp) 
	{
		/* Mark entry into crntiToIdxTbl_g */
		/*Set the rntiFag = CRNTI and initialize the ueSearchSpaceInfo_p in 
		 *UE context to &rntiInfo_p->ueSearchSpaceInfo[0]
		 */
        /* +- SPR 18841 Fix */
		newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
		newULUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;

        /* +- SPR 17902 */
		/*debug stats*/
		LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_RNTI(ueIndex,
				rntiInfo_p->rntiFlag);

		/* TA_CHG */
		/*Access the temp context in case of only valid index as in case of HO
		 * there is invalid index is stored in rntiinfo so it may lead to ABR
		 */
		if (rntiInfo_p->index != INVALID_UE_INDEX)
		{
			tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->
				tempCRNTICtxArray[rntiInfo_p->index].tempCRNTICtx_p;
			if (PNULL != tempCRNTICtx_p)
			{
                /* SPR 15682 fix start */
                /* Refer Spec 36.213 Section 4.2.3 for the calculations
	         * NTA = TA ×16 , where TA = 0, 1, 2, ..., 1282*/
                newDLUEContext_p->timingAdvanceAbsValue = 
                    (tempCRNTICtx_p->timingAdvanceAbs << 4);
                /* SPR 15682 fix end */
				newDLUEContext_p->lastTASentTick = tempCRNTICtx_p->
					lastTASentTick;
			}
		}
        /* SPR 16000 Fix Start */
        else
        {
            newDLUEContext_p->lastTASentTick = getCurrentTick();
        }
		rrcRachInterfaceNode_p->rnti = dwlkUEInfo_p->cRnti;
		rrcRachInterfaceNode_p->ueIndex = ueIndex;
        /*SPR 21103 Start*/
		if(MAC_FAILURE == putEntryInPdcchRachInterfaceQueue(CREATE_UE, rrcRachInterfaceNode_p,
				internalCellIndex))
        {
            freeMemPool(rrcRachInterfaceNode_p);
        }
        /*SPR 21103 End*/
		/* + TM6_5.2 */
		if ((TX_MODE_1 ==  dwlkUEInfo_p->transmissionMode) || 
				(TX_MODE_2 == dwlkUEInfo_p->transmissionMode) ||
				(TX_MODE_6 == dwlkUEInfo_p->transmissionMode)||
				(TX_MODE_7 == dwlkUEInfo_p->transmissionMode))
			/* - TM7_8 Changes End */
		{
			dwlkUEInfo_p->rankIndicator = RANK_INDICATOR_1;
		}
		/* - TM6_5.2 */

		/*TDD SB_CQI*/
		/* + CQI_5.0 */
		if ( (APERIODIC_CQI_REPORTING_MODE == 
					newDLUEContext_p->dlCQIInfo.cqiTypeIndicator) ||
				(PERIODIC_APERIODIC_MODE == 
				 newDLUEContext_p->dlCQIInfo.cqiTypeIndicator) ) 
		{    
			newULUEContext_p->aperiodicCQIToBeRequested = TRUE;
		}
		/*TDD SB_CQI*/
		/* + CQI_5.0 */
		newDLUEContext_p->dlMIMOInfo.dlRi = dwlkUEInfo_p->rankIndicator;
		newULUEContext_p->pucchConfigInfo.ueReportedRI = dwlkUEInfo_p->rankIndicator;
		newULUEContext_p->pucchConfigInfo.ueReportedPeriodicRI = dwlkUEInfo_p->rankIndicator;
		/* - CQI_5.0 */

		newULUEContext_p->puschConfigInfo.betaOffsetCQIIndex = uplkUEInfo_p->
			betaOffsetCQIIndex; 
		newULUEContext_p->puschConfigInfo.betaOffsetRiIndex = uplkUEInfo_p->
			betaOffsetRiIndex;


		/* EICIC  - TDD +*/

		newULUEContext_p->pucchConfigInfo.riConfigIndex_2 = uplkUEInfo_p->riConfigIndex_2;
		newULUEContext_p->pucchConfigInfo.cqiPmiConfigIndex_2 = uplkUEInfo_p->cqiPMIConfigIndex_2;
		/*Complexity Start*/
		numBitsAbsPattern=eicicFindNumOfBitsinABSPattern(internalCellIndex);
		/*Complexity End*/


		for(absLoop = 0; absLoop<numBitsAbsPattern; absLoop++)
		{
			newULUEContext_p->pucchConfigInfo.csiMeasurementSubset1[absLoop] = 0;
			newULUEContext_p->pucchConfigInfo.csiMeasurementSubset2[absLoop] = 0;
		}
		if (SETUP >= uplkUEInfo_p->cqiSubsetConfigRequestType)
		{
            /* +- SPR 17777 */
#ifdef TDD_CONFIG
			updateCsiMeasurementSubset(newULUEContext_p->pucchConfigInfo.csiMeasurementSubset1,uplkUEInfo_p->csiMeasurementSubset1,internalCellIndex);
			updateCsiMeasurementSubset(newULUEContext_p->pucchConfigInfo.csiMeasurementSubset2,uplkUEInfo_p->csiMeasurementSubset2,internalCellIndex);
#else
			updateCsiMeasurementSubset(newULUEContext_p->pucchConfigInfo.csiMeasurementSubset1,uplkUEInfo_p->csiMeasurementSubset1);
			updateCsiMeasurementSubset(newULUEContext_p->pucchConfigInfo.csiMeasurementSubset2,uplkUEInfo_p->csiMeasurementSubset2);
#endif
            /* +- SPR 17777 */

		}		

		/* EICIC - TDD -*/

		/* SS_S2 */
		newULUEContext_p->ueSearchSpaceInfo_p = newDLUEContext_p->ueSearchSpaceInfo_p =
			(&rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex][0]);
		/*pdcch changes end*/
		/* SPR 3043 start */
		if(newDLUEContext_p->tcrntiFlag)
		{
			ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag = TRUE;
			dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag = TRUE;
		}
		else
		{
			/* SPR 3043 end */
			ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag = FALSE;
			dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag = FALSE;
			newDLUEContext_p->tcrntiFlag = FALSE;
		}

		newDLUEContext_p->drxConfigType = RELEASE ;
		newULUEContext_p->drxConfigType = RELEASE ;
		newDLUEContext_p->activeUENodeTempDeleted_p = PNULL;
		newULUEContext_p->nonZeroBsrListTempDeleted_p = PNULL;
		newULUEContext_p->ulDrxContext.cqiMask = FALSE;

		newDLUEContext_p->measGapConfigType = RELEASE ;
		newULUEContext_p->measGapConfigType = RELEASE ;
		newDLUEContext_p->measGapContext.isMeasGapPeriodOn = FALSE;
		newULUEContext_p->measGapContext.isMeasGapPeriodOn = FALSE;
		/* SPR 14250 Fix Start */
		newDLUEContext_p->isMACCESendToUE = LTE_TRUE;
		/* SPR 14250 Fix End */

        /*SPR 16855 +-*/
		if ( TRUE == dwlkUEInfo_p->drxRequestType )
		{
			newDLUEContext_p->drxConfigType =  dwlkUEInfo_p->drxConfigType ;
			if ( SETUP == newDLUEContext_p->drxConfigType )
			{
				createDLUEBitMap = createDLUEBitMap | DL_DRX_INDICATION ;
			}
		}
		/* SPR 10800 [BUG 891] FIX START */
		memSet(&(newULUEContext_p->srsConfigInfo), 0, sizeof(SRSConfigInfo));
		/* SPR 10800 [BUG 891] FIX END */

		if ( MAC_FAILURE == configurePucchSRSAndReports( newULUEContext_p->ueIndex,
					&(newULUEContext_p->pucchConfigInfo),
					&(newULUEContext_p->srsConfigInfo),
					dwlkUEInfo_p,uplkUEInfo_p,
					&createULUEBitMap,
					&createDLUEBitMap,
					newULUEContext_p, 
					currSFN, currSF,internalCellIndex
					))
		{
            /* SPR 20671 fix start */
            deleteCreatedDownlinkUEContext(ueIndex,internalCellIndex);
            deletePartialCreatedUplinkUEContext(newULUEContext_p);
            /* SPR 20671 fix end */
			return MAC_FAILURE;
		}
		/* + SPS_TDD_Changes */                       
		memSet( newDLUEContext_p->spsDlInfo.ueMeasGapTTI, 
				MEAS_GAP_OFF, MAX_SPS_INTERVAL );          
		memSet( newULUEContext_p->spsUlInfo.ueMeasGapTTI, 
				MEAS_GAP_OFF, MAX_UL_SPS_INTERVAL );       
		/* - SPS_TDD_Changes */                       

		/* CA tdd pucch changes Start */
		for (scellCount = 0; scellCount<dwlkUEInfo_p->carrierAggrConfig.\
				scellCount; scellCount++)
		{
			if (ADD == dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].\
					operationType)
			{
				if ( MAC_FAILURE == macFillCreateScellCommonParams(
							dwlkUEInfo_p,uplkUEInfo_p,
							newULUEContext_p, 
							newDLUEContext_p,
							scellCount,internalCellIndex ))
				{
                    /*SPR 21103 Start*/
                    resp = MAC_FAILURE;
                    break;
                    /*SPR 21103 End*/
                }
            }
        }

        /*SPR 21103 Start*/
        if(MAC_FAILURE == resp)
        {
            deleteSRSAndPucchConfigAtCreatMACUEFailure(newULUEContext_p, 
                    internalCellIndex,
                    scellCount);
                    /* SPR 20671 fix start */
                    deleteCreatedDownlinkUEContext(ueIndex,internalCellIndex);
                    deletePartialCreatedUplinkUEContext(newULUEContext_p);
                    /* SPR 20671 fix end */
					return MAC_FAILURE;
				}
        /*SPR 21103 End*/

		/* CA tdd changes End */
		if (TRUE == dwlkUEInfo_p->measGapRequestType)
		{
			if ( MAC_FAILURE == configureMeasGap(dwlkUEInfo_p,
						&(newDLUEContext_p->measGapContext),
						&(newULUEContext_p->measGapContext),
						&(newDLUEContext_p->measGapConfigType),
						&(newULUEContext_p->measGapConfigType),
						&createULUEBitMap,
						&createDLUEBitMap,
						internalCellIndex))
			{
                /*SPR 21103 Start*/
                deleteSRSAndPucchConfigAtCreatMACUEFailure(newULUEContext_p, 
                        internalCellIndex,
                        scellCount);
                /*SPR 21103 End*/
                    /* SPR 20671 fix start */
                    deleteCreatedDownlinkUEContext(ueIndex,internalCellIndex);
                    deletePartialCreatedUplinkUEContext(newULUEContext_p);
                    /* SPR 20671 fix end */
                
				return MAC_FAILURE;
			}
		}
#ifdef SSI_DEBUG_STATS
		/* CA Stats Changes Start */
		++gMacStats.pStats->numActiveUe[internalCellIndex];
#endif        
        /*SPR 16855 +-*/
	}
	else
	{
        /*SPR 16855 +-*/
		/*Coverity 25491 Fix Start*/
		freeMemPool(rrcRachInterfaceNode_p);
		/*Coverity 25491 Fix End*/
        /*SPR 16855 +-*/
		lteWarning("UE Context does not exist");
	}

	if (dlResp != MAC_SUCCESS)
	{
		resp = dlResp;
	}
	else if (ulResp != MAC_SUCCESS)
	{
		resp = ulResp;
	}
	else 
	{
		resp = MAC_SUCCESS;
        /*SPR21501 fix start*/
		newULUEContext_p->ttiBundlingEnabled = uplkUEInfo_p->ttiBundlingEnabled;
        /*SPR21501 fix end*/
	}
    /*SPR 16855 +-*/
	*createULUEBitMap_p = createULUEBitMap;
	*createDLUEBitMap_p = createDLUEBitMap;
    /*SPR 16855 +-*/
	/* SPR 2882 fix end */
	return resp;
}
/* - coverity 55307 */

#elif FDD_CONFIG

/****************************************************************************
 * Function Name  : createMACUEEntity
 * Inputs         : dwlkUEInfo_p - Pointer to ueContextForDownlink required
 *                                 to configure MAC DL UE Context.
 *                  uplkUEInfo_p - Pointer to ueContextForUplink required
 *                                 to configure MAC UL UE Context.
 *                  uplkLCCount - Number of LCs configured at MAC UL.
 *                  transactionId - transaction Id received in request from RRC
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                  createULUEBitMap_p -  pointer UL BIT map to be used to update 
 *                                       UL changes in passive context and HP thread 
 *                                       to swap context
 *                  createDLUEBitMap_pp - pointer DL BIT map to be used to update 
 *                                       DL changes in passive context and HP thread 
 *                                        to swap context
 *                  currSFN - current System frame number
 *                  currSF - current Subframe number
 * Outputs        : None
 * Returns        : MAC_SUCESS or MAC_FAILURE
 * Description    : This API takes pointers to MAC & RLC Info required to 
 *                  configure UE context at Layer2. It returns  SUCCESS
 *                  and ueIdx allocated for UE will be stored in the 
 *                  dwlkUEInfo_p passed as an input arguement.
 ****************************************************************************/
MacRetType createMACUEEntity( UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink*  uplkUEInfo_p,
		UInt32 uplkLCCount,
		/* +- SPR 17777 */
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		, UInt32 *createULUEBitMap_p,
		UInt32 *createDLUEBitMap_p,
		UInt32 currSFN, UInt32 currSF
        /*SPR 16855 +-*/
		) 
{
	/*+COVERITY 5.3.0 - 32260*/
	MacRetType resp   = MAC_FAILURE;
	/*-COVERITY 5.3.0 - 32260*/

	/* Coverity 54158 54159 Fix Start */
	MacRetType ulResp = MAC_SUCCESS;
	MacRetType dlResp = MAC_SUCCESS;
	/* Coverity 54158 54159 Fix End */
	/* CA Changes start */
	UInt8 scellCount = 0;
    /*SPR 16855 +-*/
	/* CA Changes end */

	UInt32 ueIndex= 0;
    /*SPR 16855 +-*/
	UInt32 createULUEBitMap = *createULUEBitMap_p;
	UInt32 createDLUEBitMap = *createDLUEBitMap_p;
	RRCRachInterfaceNode *rrcRachInterfaceNode_p = PNULL;
    /*SPR 16855 +-*/
	DLUEContext *newDLUEContext_p = PNULL;
	ULUEContext *newULUEContext_p = PNULL;

	RNTIInfo *rntiInfo_p        = PNULL;
	/* TA_CHG */
	/* + FDD_COMPILATION_WARNING */
	/* + SPR_3939 */
	TempCRNTICtx *tempCRNTICtx_p = PNULL;  
	UInt8 absLoop = 0;
	UInt8 numBitsAbsPattern = 0; /* EICIC +-*/

	/* - SPR_3939 */
	/* - FDD_COMPILATION_WARNING */
    /*SPR 16855 +-*/
	GET_MEM_FROM_POOL( RRCRachInterfaceNode,rrcRachInterfaceNode_p,sizeof (RRCRachInterfaceNode),PNULL);
	/*++klockwork warning */
	if(PNULL == rrcRachInterfaceNode_p)
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"Memory Allocation failure for rrcRachInterfaceNode_p\n");
		return MAC_FAILURE;
	}
	/*--klockwork warning */
    /*SPR 16855 +-*/

	/* TA_CHG */

	/* Create UE Context at MAC Dwlk & Uplk */
	ueIndex = dwlkUEInfo_p->ueIndex;
	dlResp = createMACDLUEEntity(dwlkUEInfo_p,internalCellIndex);
	if (( MAC_SUCCESS == dlResp) || (MAC_LOG_CH_ERROR == dlResp))
	{
        /* + SPR 18841 Fix */
		rntiInfo_p = getUeIdxFromRNTIMap(dwlkUEInfo_p->cRnti, internalCellIndex);
		newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
		if ((PNULL == rntiInfo_p)||
#ifndef UTFWK_SIMULATION 
				((FALSE == newDLUEContext_p->tcrntiFlag) &&
				 (rntiInfo_p->index == INVALID_UE_INDEX)) ||
#endif
				(CRNTI == rntiInfo_p->rntiFlag))
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"getUeIdxFromRNTIMap Failed UE %u\n",
                    ueIndex);
			deleteCreatedDownlinkUEContext(ueIndex,internalCellIndex);
			freeMemPool(rrcRachInterfaceNode_p);
			return MAC_FAILURE;
		}
        /* - SPR 18841 Fix */

		ulResp = createMACULUEEntity(ueIndex, 
				uplkLCCount, uplkUEInfo_p,dwlkUEInfo_p,internalCellIndex);
		if ((MAC_FAILURE == ulResp) || (MAC_INTERNAL_ERROR == ulResp))
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"Error creating MAC UL Entity UE %u\n",ueIndex);
			/*SPR 7122 Fix start*/
			/* Deleting Complete Downlink context in case uplink context get fail*/
			deleteCreatedDownlinkUEContext(ueIndex,internalCellIndex);
			/*SPR 7122 Fix End*/
            /* Coverity 113369 Fixed start*/
            /* code remove */
            /* Coverity 113369 Fixed end*/
        }
		else
		{
			/* SPR 10683 changes start */
            /* SPR 20673 start */
			/* addUEPeriodicReportBatchInfo(ueIndex,internalCellIndex); */
            /* SPR 20673 end   */
			/* SPR 10683 changes end */

			/* Set DL, UL flags to 1 */
			resp = MAC_SUCCESS;
		}
	}
    /* Coverity 113369 Fixed start*/
	if (( MAC_FAILURE != resp)&&(ulResp != MAC_FAILURE )&& (dlResp != MAC_FAILURE )) 
    /* Coverity 113369 Fixed end*/    
	{
		/* Mark entry into crntiToIdxTbl_g */
		/*Set the rntiFag = CRNTI and initialize the ueSearchSpaceInfo_p in 
		 *UE context to &rntiInfo_p->ueSearchSpaceInfo[0]
		 */
        /* +- SPR 18841 Fix */
		newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
		newULUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
        /* +- SPR 17902 */
		/* For TM mode 1,2,6,7, only single layer transmission possible */
		if((TX_MODE_1 == dwlkUEInfo_p->transmissionMode)||
				(TX_MODE_2 == dwlkUEInfo_p->transmissionMode)||
				(TX_MODE_6 == dwlkUEInfo_p->transmissionMode)||
				(TX_MODE_7 == dwlkUEInfo_p->transmissionMode))
			/* - TM7_8 Changes End */
		{
			dwlkUEInfo_p->rankIndicator = RANK_INDICATOR_1;
		}
		/* - TM6_5.2 */

		/* + CQI_5.0 */
		newDLUEContext_p->dlMIMOInfo.dlRi = dwlkUEInfo_p->rankIndicator;
		newULUEContext_p->pucchConfigInfo.ueReportedRI = dwlkUEInfo_p->rankIndicator;
		newULUEContext_p->pucchConfigInfo.ueReportedPeriodicRI = dwlkUEInfo_p->rankIndicator;

		/* TA_CHG */
		/*Access the temp context in case of only valid index as in case of HO
		 * there is invalid index is stored in rntiinfo so it may lead to ABR
		 */
        {
		if (rntiInfo_p->index != INVALID_UE_INDEX)
		{
			/* + SPR_3939 */
			tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->
				tempCRNTICtxArray[rntiInfo_p->index].tempCRNTICtx_p;

			if (PNULL != tempCRNTICtx_p)
			{
                /* SPR 15682 fix start */
                /* Refer Spec 36.213 Section 4.2.3 for the calculations
	         * NTA = TA ×16 , where TA = 0, 1, 2, ..., 1282*/
                 newDLUEContext_p->timingAdvanceAbsValue = 
                     (tempCRNTICtx_p->timingAdvanceAbs << 4);
                /* SPR 15682 fix end */
				newDLUEContext_p->lastTASentTick = tempCRNTICtx_p->
					lastTASentTick;
			}
			/* - SPR_3939 */
		}
        /* SPR 16000 Fix Start */
        else
        {
            newDLUEContext_p->lastTASentTick = getCurrentTick();
        }
        }
		rrcRachInterfaceNode_p->rnti = dwlkUEInfo_p->cRnti;
		rrcRachInterfaceNode_p->ueIndex = ueIndex;
        /*SPR 21103 Start*/
		if(MAC_FAILURE == putEntryInPdcchRachInterfaceQueue(CREATE_UE, rrcRachInterfaceNode_p,
				internalCellIndex))
        {
            freeMemPool(rrcRachInterfaceNode_p);
        }
        /*SPR 21103 End*/
        /*SPR 16855 +-*/
		/* + CQI_5.0 */
        {
		newULUEContext_p->aperiodicCQIToBeRequested = FALSE;
		if ( (APERIODIC_CQI_REPORTING_MODE == 
					newDLUEContext_p->dlCQIInfo.cqiTypeIndicator) ||
				(PERIODIC_APERIODIC_MODE == 
				 newDLUEContext_p->dlCQIInfo.cqiTypeIndicator) ) 
		{    
			newULUEContext_p->aperiodicCQIToBeRequested = TRUE;
			/* SPR 9427 changes start */
			newULUEContext_p->aperiodicCqiRequestTTI = 
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
			/* SPR 9427 changes end */
		}
		/* - CQI_5.0 */

		/* TA_CHG */
		/* SS_S2  */
		newULUEContext_p->puschConfigInfo.betaOffsetCQIIndex = uplkUEInfo_p->
			betaOffsetCQIIndex; 
		newULUEContext_p->puschConfigInfo.betaOffsetRiIndex = uplkUEInfo_p->
			betaOffsetRiIndex;
		/* SS_S2 */
		/* + CL-MIMO LJA*/
		newULUEContext_p->pucchConfigInfo.riConfigIndex = uplkUEInfo_p->riConfigIndex;
		/* - CL-MIMO LJA*/
		/*eicic changes starts*/

		newULUEContext_p->pucchConfigInfo.riConfigIndex_2 = uplkUEInfo_p->riConfigIndex_2;
		newULUEContext_p->pucchConfigInfo.cqiPmiConfigIndex_2 = uplkUEInfo_p->cqiPMIConfigIndex_2;

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

		for(absLoop = 0; absLoop<numBitsAbsPattern; absLoop++)
		{
			newULUEContext_p->pucchConfigInfo.csiMeasurementSubset1[absLoop] = 0;
			newULUEContext_p->pucchConfigInfo.csiMeasurementSubset2[absLoop] = 0;

		}
		if (SETUP >= uplkUEInfo_p->cqiSubsetConfigRequestType)
		{
            /* +- SPR 17777 */
#ifdef TDD_CONFIG
			updateCsiMeasurementSubset(newULUEContext_p->pucchConfigInfo.csiMeasurementSubset1,uplkUEInfo_p->csiMeasurementSubset1,internalCellIndex);
			updateCsiMeasurementSubset(newULUEContext_p->pucchConfigInfo.csiMeasurementSubset2,uplkUEInfo_p->csiMeasurementSubset2,internalCellIndex);
#else
			updateCsiMeasurementSubset(newULUEContext_p->pucchConfigInfo.csiMeasurementSubset1,uplkUEInfo_p->csiMeasurementSubset1);
			updateCsiMeasurementSubset(newULUEContext_p->pucchConfigInfo.csiMeasurementSubset2,uplkUEInfo_p->csiMeasurementSubset2);
#endif
            /* +- SPR 17777 */

		}		

		/*eicic changes ends*/
		/*pdcch changes start*/
		newULUEContext_p->ueSearchSpaceInfo_p = newDLUEContext_p->ueSearchSpaceInfo_p =
			(&rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex][0]);
		setUeSearchSpaceInfoforScell(newULUEContext_p,newDLUEContext_p,rntiInfo_p,internalCellIndex );
		/*pdcch changes end*/
        } //IS_NB_IOT_CELL

		/* SPR 3043 start */
		if(newDLUEContext_p->tcrntiFlag)
		{
			ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag = TRUE;
			dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag = TRUE;
		}
		else
		{
			/* SPR 3043 end */
			ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag = FALSE;
			dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag = FALSE;
		}
		/* +  DRX_CHG  */
		newDLUEContext_p->drxConfigType = RELEASE ;
		newULUEContext_p->drxConfigType = RELEASE ;
		newDLUEContext_p->activeUENodeTempDeleted_p = PNULL;
		newULUEContext_p->nonZeroBsrListTempDeleted_p = PNULL;
		/* Rel9_upgrade_CR396 */
		newULUEContext_p->ulDrxContext.cqiMask = FALSE;
		/* +  DRX_CHG  */

		/* + MEAS_GAP_CHG */
		newDLUEContext_p->measGapConfigType = RELEASE ;
		newULUEContext_p->measGapConfigType = RELEASE ;
		/* SPR 3157 changes start */
		newDLUEContext_p->measGapContext.isMeasGapPeriodOn = FALSE;
		newULUEContext_p->measGapContext.isMeasGapPeriodOn = FALSE;
		/* SPR 3157 changes end */
		/* + MEAS_GAP_CHG */
		/*phase 3 change starts*/
		newDLUEContext_p->isMACCESendToUE = LTE_TRUE;
		/*phase 3 change ends*/

        {
        /*SPR 16855 +-*/
		if ( TRUE == dwlkUEInfo_p->drxRequestType )
		{
			newDLUEContext_p->drxConfigType =  dwlkUEInfo_p->drxConfigType ;
			if ( SETUP == newDLUEContext_p->drxConfigType )
			{
				createDLUEBitMap = createDLUEBitMap | DL_DRX_INDICATION ;
			}
		}

		if ( MAC_FAILURE == configurePucchSRSAndReports( newULUEContext_p->ueIndex,
					&(newULUEContext_p->pucchConfigInfo),
					&(newULUEContext_p->srsConfigInfo),
					dwlkUEInfo_p,uplkUEInfo_p,
					&createULUEBitMap,
					&createDLUEBitMap,
					newULUEContext_p, 
					currSFN, currSF,internalCellIndex
					))
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"configurePucchSRSAndReports Failed UE %u\n",
                    newULUEContext_p->ueIndex);
                    /* SPR 20671 fix start */
                    deleteCreatedDownlinkUEContext(ueIndex,internalCellIndex);
                    deletePartialCreatedUplinkUEContext(newULUEContext_p);
                    /* SPR 20671 fix end */
			return MAC_FAILURE;
		}
#ifdef FDD_CONFIG
		/* CA changes Start */
		for (scellCount = 0; scellCount<dwlkUEInfo_p->carrierAggrConfig.\
				scellCount; scellCount++)
		{
			if (ADD == dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].\
					operationType)
            {
                if ( MAC_FAILURE == macFillCreateScellCommonParams(
                            dwlkUEInfo_p,uplkUEInfo_p,
                            newULUEContext_p, 
                            newDLUEContext_p,
                            scellCount,internalCellIndex ))
                {
                    LOG_MAC_ERROR(MAC_RRC_INF,"macFillCreateScellCommonParams Failed UE :%u\n",
                            newULUEContext_p->ueIndex);
                    
                    /*SPR 21103 Start*/
                    resp = MAC_FAILURE;
                    break;
                    /*SPR 21103 End*/
                }
            }    
        }
		/* CA changes End */
        
        /*SPR 21103 Start*/
        if(MAC_FAILURE == resp)
        {
            deleteSRSAndPucchConfigAtCreatMACUEFailure(newULUEContext_p, 
                    internalCellIndex,
                    scellCount);
                    /* SPR 20671 fix start */
                    deleteCreatedDownlinkUEContext(ueIndex,internalCellIndex);
                    deletePartialCreatedUplinkUEContext(newULUEContext_p);
                    /* SPR 20671 fix end */
            return MAC_FAILURE;
        }
        /*SPR 21103 End*/
#endif        
		memSet( newDLUEContext_p->spsDlInfo.ueMeasGapTTI,
				MEAS_GAP_OFF, MAX_SPS_INTERVAL );
		memSet( newULUEContext_p->spsUlInfo.ueMeasGapTTI,
				MEAS_GAP_OFF, MAX_SPS_INTERVAL );

		if (TRUE == dwlkUEInfo_p->measGapRequestType)
        {
			if ( MAC_FAILURE == configureMeasGap(dwlkUEInfo_p,
						&(newDLUEContext_p->measGapContext),
						&(newULUEContext_p->measGapContext),
						&(newDLUEContext_p->measGapConfigType),
						&(newULUEContext_p->measGapConfigType),
						&createULUEBitMap,
						&createDLUEBitMap,
						internalCellIndex))
            {
                LOG_MAC_ERROR(MAC_RRC_INF,"configureMeasGap failed UE %u\n",
                        newULUEContext_p->ueIndex);
                /*SPR 21103 Start*/
                deleteSRSAndPucchConfigAtCreatMACUEFailure(newULUEContext_p, 
                        internalCellIndex,
                        scellCount);
                /*SPR 21103 End*/
                    /* SPR 20671 fix start */
                    deleteCreatedDownlinkUEContext(ueIndex,internalCellIndex);
                    deletePartialCreatedUplinkUEContext(newULUEContext_p);
                    /* SPR 20671 fix end */
                return MAC_FAILURE;
            }
            /* + SPS_TDD_Changes */
            /* code removed */
            /* - SPS_TDD_Changes */
        }
#ifdef SSI_DEBUG_STATS
		/* CA Stats Changes Start */
		++gMacStats.pStats->numActiveUe[internalCellIndex];
#endif        
        /*SPR 16855 +-*/
        } //IS_NB_IOT_CELL
	}
	else
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"UE %u Context does not exist\n",
                ueIndex);
        /*SPR 16855 +-*/
		/*Coverity 25491 Fix Start*/
		/*Coverity 113369 Fix Start*/
        if( PNULL != rrcRachInterfaceNode_p)
        {
            freeMemPool(rrcRachInterfaceNode_p);
        }
		/*Coverity 113369 Fix End*/
		/*Coverity 25491 Fix End*/
        /*SPR 16855 +-*/
	}

	if (dlResp != MAC_SUCCESS)
	{
		resp = dlResp;
	}
	else if (ulResp != MAC_SUCCESS)
	{
		resp = ulResp;
	}
	else 
	{    
		resp = MAC_SUCCESS;
        /*SPR 16855 +-*/
		*createULUEBitMap_p = createULUEBitMap;
		*createDLUEBitMap_p = createDLUEBitMap;
        /*SPR 16855 +-*/

		/* Coverity 31809 fix start */
#ifdef FDD_CONFIG
		newULUEContext_p->ttiBundlingEnabled = uplkUEInfo_p->ttiBundlingEnabled;
#endif
		/* Coverity 31809 fix End */

	}   


    /*SPR 16855 +-*/
	/* SPR 2882 fix end */
	/* +COVERITY 19396 */
#ifdef SSI_DEBUG_STATS
	if (MAC_SUCCESS == resp)
	{
		LTE_MAC_UPDATE_DL_DRX_MEAS_GAP_TTI( ueIndex, newDLUEContext_p );
	}
#endif
	/* -COVERITY 19396 */
	return resp;
}

#endif

/*SPR 16855 +-*/
/***************************************************************************************
 * Function Name  : configurePucchSRSAndReports 
 * Inputs         : tempPucchConfigInfo_p - pointer to PUCCH context to be configured
 *                  tempsrsConfigInfo_p - Pointer to SRS context to be configured
 *                  dwlkUEInfo_p - Pointer to downlink UE context Info 
 *                  uplkUEInfo_p - Pointer to uplink UE context Info
 *                  createULUEBitMap_p -  pointer UL BIT map to be used to update 
 *                                       UL changes in passive context and HP thread 
 *                                       to swap context
 *                  createDLUEBitMap_pp - pointer DL BIT map to be used to update 
 *                                       DL changes in passive context and HP thread 
 *                                        to swap context
 *                  newULUEContext_p - Pointer to the uplink UE Context
 *                  currSFN - current System frame number
 *                  currSF - current Subframe number
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None 
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will processs the PUCCH and SRS report 
 *                  calls the appropriate function based on report type
 *                   and configure it at time of MAC_CREATE_UE_ENTITY_REQ received from RRC
 ******************************************************************************************/

 MacRetType configurePucchSRSAndReports(UInt16 ueIndex,
		PucchConfig *tempPucchConfigInfo_p,
		SRSConfigInfo *tempsrsConfigInfo_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink  *uplkUEInfo_p,
		UInt32 *createULUEBitMap_p,
		UInt32 *createDLUEBitMap_p,
		ULUEContext *newULUEContext_p,
		UInt32 currSFN, UInt32 currSF,
		InternalCellIndex internalCellIndex
		)
{
	UInt32 createULUEBitMap = *createULUEBitMap_p;
	UInt32 createDLUEBitMap = *createDLUEBitMap_p;
	/* CA changes Start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */

	/* Review comment fix start GG15 */
#ifdef FDD_CONFIG
	/*CA Changes start  */
	if (MAC_FAILURE == macValidateUePucchResources(uplkUEInfo_p, internalCellIndex))
		/*CA Changes end  */
	{
		LOG_MAC_MSG(MAC_VALIDATE_UE_PUCCH_RES_FAILURE_ID,
				LOGERROR,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				ueIndex,
				LINE_NUMBER,
				internalCellIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,
				"PUCCH Resources resquired fro the UE is very high");
		return MAC_FAILURE;
	}
#endif
	/* Review comment fix end GG15 */

	if ( uplkUEInfo_p->requestType == TRUE)
	{
		if ( MAC_FAILURE == initSchedReqInPUCCHMap( 
					ueIndex,
					tempPucchConfigInfo_p,
					newULUEContext_p,
					uplkUEInfo_p->srConfigurationIndex,
					uplkUEInfo_p->srPucchResourceIndex,
					uplkUEInfo_p->srProhibitTimerValv920,
					internalCellIndex
					))

		{
            LOG_MAC_ERROR(MAC_RRC_INF,"initSchedReqInPUCCHMap Failed UE %u\n",
                    ueIndex);
			return MAC_FAILURE;
		}
		if ( tempPucchConfigInfo_p->srPeriodicity <= SHORT_SR_PERIODICITY)
		{    
			createULUEBitMap = createULUEBitMap | UL_SHORT_SR_MAP_INDICATION;
		}    
		else
		{    
			createULUEBitMap = createULUEBitMap | UL_PUCCH_MAP_INDICATION;
		}    
		tempPucchConfigInfo_p->srProhibitExpiryTTI = 0;
	}

	/* cqiRequest - 1 (Periodic CQI ) */
	if ( uplkUEInfo_p->cqiRequestType == TRUE )
	{
		createULUEBitMap = createULUEBitMap | UL_PUCCH_MAP_INDICATION;
		/*SPR 2617 Start*/
		if ( MAC_FAILURE == initPCellCQIAndRIinPUCCHMap ( 
					ueIndex,tempPucchConfigInfo_p,
                    /* +- SPR 17777 */
					dwlkUEInfo_p,
                    /* +- SPR 17777 */
					uplkUEInfo_p,
					internalCellIndex
					))
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"initPCellCQIAndRIinPUCCHMap failed UE %u\n",
                    ueIndex);
			return MAC_FAILURE;
		}
		/*SPR 2617 End*/
		if(PERIODIC_FEEDBACK_SUBBAND == 
				tempPucchConfigInfo_p->cqiPmiRiInfo.periodicFeedBackType &&
				FREQUENCY_SELECTIVE_ON == cellParams_p->freqSelectiveSchDL)
		{    
			initBandwidthPart(newULUEContext_p,internalCellIndex);
		}
	}

	/* + SPR_3366 */
	if( INVALID_APERIODIC_CQI_MODE != uplkUEInfo_p->cqiMode )
	{
		tempPucchConfigInfo_p->cqiPmiRiInfoAperiodic.cqiMode = 
			dwlkUEInfo_p->cqiMode;
	}

	/* - SPR_3366 */

	/*FDD_SRS*/
	/* SRS is supported for FDD mode.*/
	if((TRUE == uplkUEInfo_p->srsRequestType) &&
			(SRS_COMMON_CONFIG_TYPE_SETUP == cellParams_p->srsConfigType))
	{


		if (MAC_FAILURE == configureDedicatedSRSConfig(ueIndex,
					tempsrsConfigInfo_p,
					&uplkUEInfo_p->srsDedicatedSetupInfo, 
					uplkUEInfo_p->srsConfigType, 
					currSFN, currSF, internalCellIndex))
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"Error configuring Dedicated SRS Config UE %u\n",
                    ueIndex);
		}
		else
		{
			/*SPR 9405 (BUG 623) FIX START */
#ifdef FDD_CONFIG
			if ( uplkUEInfo_p->srsDedicatedSetupInfo.srsConfigIndex <= 16 )
				createULUEBitMap = createULUEBitMap | UL_SHORT_SRS_MAP_INDICATION;

			else if((16 < uplkUEInfo_p->srsDedicatedSetupInfo.srsConfigIndex )&&
					(uplkUEInfo_p->srsDedicatedSetupInfo.srsConfigIndex < 637))
				createULUEBitMap = createULUEBitMap | UL_LONG_SRS_MAP_INDICATION;
#else
			if ( uplkUEInfo_p->srsDedicatedSetupInfo.srsConfigIndex <= 9 )
				createULUEBitMap = createULUEBitMap | UL_SHORT_SRS_MAP_INDICATION;

			else 
				createULUEBitMap = createULUEBitMap | UL_LONG_SRS_MAP_INDICATION;
#endif
			/*SPR 9405 (BUG 623) FIX END */
		}

	}

	*createULUEBitMap_p = createULUEBitMap;
	*createDLUEBitMap_p = createDLUEBitMap;
	return MAC_SUCCESS;
}

/************************************************************************************
 * Function Name  : configureMeasGap 
 * Inputs         : dwlkUEInfo_p - Pointer to Downlink UE context received
 *                  createULUEBitMap_p -  pointer UL BIT map to be used to update 
 *                      UL changes in passive context and HP thread to swap context
 *                  createDLUEBitMap_pp - pointer DL BIT map to be used to update 
 *                      DL changes in passive context and HP thread to swap context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : tempDLMeasGapContextInfo_p - Pointer to update Downlink 
 *                       Measurement Gap Context
 *                  ulMeasGapConfigType_p - Pointer to update uplink
 *                         Measurement Gap Context
 *                  tempULMeasGapContextInfo_p - Pointer to update uplink 
 *                       Measurement Gap Context
 *                  dlMeasGapConfigType_p - Pointer to update downlink
 *                         Measurement Gap Context
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function process the Measurement of UE Configuration 
 *                  in MAC_CREATE_UE_ENTITY_REQ received from RRC
 **************************************************************************************/

 MacRetType configureMeasGap(UeContextForDownlink *dwlkUEInfo_p,
		MeasGapContextDL *tempDLMeasGapContextInfo_p,
		MeasGapContextUL *tempULMeasGapContextInfo_p,
		UInt8 *dlMeasGapConfigType_p,
		UInt8 *ulMeasGapConfigType_p,
		UInt32 *createULUEBitMap_p,
		UInt32 *createDLUEBitMap_p,
		InternalCellIndex internalCellIndex)
{
	UInt32 createULUEBitMap = *createULUEBitMap_p;
	UInt32 createDLUEBitMap = *createDLUEBitMap_p;

	*dlMeasGapConfigType_p = RELEASE ;
	*ulMeasGapConfigType_p = RELEASE ;
	/* SPR 3157 changes start */
	tempDLMeasGapContextInfo_p->isMeasGapPeriodOn = FALSE; 
	tempULMeasGapContextInfo_p->isMeasGapPeriodOn = FALSE;
	/* SPR 3157 changes end */

	if (SETUP == dwlkUEInfo_p->measGapConfigType)
	{
		if ( GP0 == dwlkUEInfo_p-> measGapConfig.gapPatternId 
#ifdef ENDC_ENABLED
				/*Meas_Gap_Changes_Start*/
				|| GP7_R15 == dwlkUEInfo_p-> measGapConfig.gapPatternId
				/*Meas_Gap_Changes_End*/
#endif
		   )
			createDLUEBitMap = createDLUEBitMap | DL_MEAS_GAP_40MS_MAP_INDICATION ;
#ifdef ENDC_ENABLED
		/*Meas_Gap_Changes_Start*/
		else if(GP4_R15 == dwlkUEInfo_p-> measGapConfig.gapPatternId
				|| GP6_R15 == dwlkUEInfo_p-> measGapConfig.gapPatternId
				|| GP10_R15 == dwlkUEInfo_p-> measGapConfig.gapPatternId)
		{
			createDLUEBitMap |= DL_MEAS_GAP_20MS_MAP_INDICATION;
		}
		else if(GP5_R15 == dwlkUEInfo_p-> measGapConfig.gapPatternId
				|| GP9_R15 == dwlkUEInfo_p-> measGapConfig.gapPatternId
				|| GP11_R15 == dwlkUEInfo_p-> measGapConfig.gapPatternId)
		{
			createDLUEBitMap |= DL_MEAS_GAP_160MS_MAP_INDICATION;
		}
		/*Meas_Gap_Changes_End*/
#endif
		else
			createDLUEBitMap = createDLUEBitMap | DL_MEAS_GAP_80MS_MAP_INDICATION ;
		processMeasGapConfig(dwlkUEInfo_p, tempDLMeasGapContextInfo_p,
				tempULMeasGapContextInfo_p,dlMeasGapConfigType_p,
				ulMeasGapConfigType_p, internalCellIndex);
	}
	*createULUEBitMap_p = createULUEBitMap;
	*createDLUEBitMap_p = createDLUEBitMap;
	return MAC_SUCCESS;
}

/**********************************************************************************
 * Function Name  : createUEULConfigCnf 
 * Inputs         : ulConfigUeResp_p - Pointer for response received from HP thread 
 *                  dwlkUEInfo_p - Pointer to DL UE context  info
 *                  uplkUEInfo_p - Pointer to Ul UE context info
 *                  currSFN,currSF
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function process configuration of UL UE context of the 
 current passive index once got the successful response 
 from High Priority thread.
 ***********************************************************************************/
 MacRetType createUEULConfigCnf( ULCreateUeResp *ulConfigUeResp_p,
		UeContextForDownlink  *dwlkUEInfo_p,
		UeContextForUplink  *uplkUEInfo_p,
		UInt32 currSFN, UInt32 currSF,
		InternalCellIndex internalCellIndex) 
{
	UInt32 createULUEBitMap = 0;
	UInt32 createDLUEBitMap = 0;
	/* CA changes start */
	/* + CA_TDD_HARQ_CHANGES */    
	UInt8 scellCount =0, sCellIndex =0;
	ScellInfo *scellInfo_p;
	DLUEContext *newDLUEContext_p = PNULL;
	ULUESCellContext **ulScellContext_pp=PNULL;
	newDLUEContext_p = dlUECtxInfoArr_g[ulConfigUeResp_p->ueIndex].dlUEContext_p;
	/* - CA_TDD_HARQ_CHANGES */    
	/* CA changes end */
	ULUEContext *newULUEContext_p = PNULL;
	newULUEContext_p = ulUECtxInfoArr_g[ulConfigUeResp_p->ueIndex].ulUEContext_p;
	PucchConfig *tempPucchConfigInfo = &(newULUEContext_p->pucchConfigInfo);
	SRSConfigInfo *tempsrsConfigInfo = &(newULUEContext_p->srsConfigInfo);

	if ( MAC_FAILURE ==  configurePucchSRSAndReports(ulConfigUeResp_p->ueIndex,
				tempPucchConfigInfo,
				tempsrsConfigInfo,
				dwlkUEInfo_p,
				uplkUEInfo_p,
				&createULUEBitMap,
				&createDLUEBitMap,
				newULUEContext_p, 
				currSFN, currSF,
				internalCellIndex
				))
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"configurePucchSRSAndReports failed UE %u\n",
                ulConfigUeResp_p->ueIndex);
		return MAC_FAILURE;
	}
	if (SETUP >= uplkUEInfo_p->cqiSubsetConfigRequestType)
	{
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
		updateCsiMeasurementSubset(newULUEContext_p->pucchConfigInfo.csiMeasurementSubset1,uplkUEInfo_p->csiMeasurementSubset1,internalCellIndex);
		updateCsiMeasurementSubset(newULUEContext_p->pucchConfigInfo.csiMeasurementSubset2,uplkUEInfo_p->csiMeasurementSubset2,internalCellIndex);
#else
        updateCsiMeasurementSubset(newULUEContext_p->pucchConfigInfo.csiMeasurementSubset1,uplkUEInfo_p->csiMeasurementSubset1);
        updateCsiMeasurementSubset(newULUEContext_p->pucchConfigInfo.csiMeasurementSubset2,uplkUEInfo_p->csiMeasurementSubset2);
#endif
        /* +- SPR 17777 */

	}

	/* + CA_TDD_HARQ_CHANGES */    
	/* CA changes start */
	for (scellCount = 0; scellCount<dwlkUEInfo_p->carrierAggrConfig.\
			scellCount; scellCount++)
	{
		if (ADD == dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].\
				operationType)
		{
			scellInfo_p = &(dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount]);
			sCellIndex = scellInfo_p->sCellIndex;
			ulScellContext_pp = &(newULUEContext_p->ulUeScellContext_p[sCellIndex]);

			if ( MAC_FAILURE == macConfigureScellPucchSRSAndReports(
						&((*ulScellContext_pp)->
							scellpucchConfigInfo),
						dwlkUEInfo_p,uplkUEInfo_p,
						newULUEContext_p, 
						newDLUEContext_p,
						scellCount,internalCellIndex ))
			{
                LOG_MAC_ERROR(MAC_RRC_INF,"macConfigureScellPucchSRSAndReports failed UE %u\n",
                        newDLUEContext_p->ueIndex);
				return MAC_FAILURE;
			}
		}
	}
	/* CA changes end */
	/* - CA_TDD_HARQ_CHANGES */    

	return MAC_SUCCESS;
}

/**********************************************************************************
 * Function Name  : createUEDLConfigCnf 
 * Inputs         : dlCreateUeResp_p - Pointer for response received from HP thread 
 *                  dwlkUEInfo_p - Pointer to DL UE context info
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function process configuration of DL UE context of the 
 current passive index once got the successful response 
 from High Priority thread.
 ***********************************************************************************/
 MacRetType createUEDLConfigCnf (DLCreateUeResp *dlCreateUeResp_p,
		UeContextForDownlink  *dwlkUEInfo_p
		,InternalCellIndex internalCellIndex
		)

{
	ULUEContext *newULUEContext_p = PNULL;
	DLUEContext *newDLUEContext_p = PNULL;
	MeasGapContextDL *dlMeasGapInfo_p = PNULL;
	MeasGapContextUL *ulMeasGapInfo_p = PNULL;
	UInt8 dlMeasGapConfigType = 0;
	UInt8 ulMeasGapConfigType = 0;
	UInt32 createULUEBitMap = 0;
	UInt32 createDLUEBitMap = 0;

	newULUEContext_p = ulUECtxInfoArr_g[dlCreateUeResp_p->ueIndex].ulUEContext_p;
	newDLUEContext_p = dlUECtxInfoArr_g[dlCreateUeResp_p->ueIndex].dlUEContext_p;
	dlMeasGapInfo_p = &newDLUEContext_p->measGapContext;
	ulMeasGapInfo_p = &newULUEContext_p->measGapContext;

	if (TRUE == dwlkUEInfo_p->measGapRequestType)
	{
		if ( MAC_FAILURE ==  configureMeasGap(dwlkUEInfo_p,
					dlMeasGapInfo_p,
					ulMeasGapInfo_p,
					&dlMeasGapConfigType,
					&ulMeasGapConfigType,
					&createULUEBitMap,
					&createDLUEBitMap,
					internalCellIndex                    
					))
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"configureMeasGap failed from createUEDLConfigCnf UE %u\n",
                    dwlkUEInfo_p->ueIndex);
			return MAC_FAILURE;
		}
	}
	return MAC_SUCCESS;
}
/*SPR 16855 +-*/

/****************************************************************************
 * Function Name  : deleteMACUEEntity
 * Inputs         : ueIndex - Index at which UE Context needs to be deleted.
 *                  transactionId - identifier received in request from RRCi
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS, MAC_FAILURE.
 * Description    : This API takes ueIdx rom RRC and request registered Layer2
 *                  modules to delete the UE Context.
 *                  It will delete the ueId context from idx table if all 
 *                  registered layers returns Success otherwise wait for the
 *                  confirmation before freeing the index.
 ****************************************************************************/
/*+COVERITY 5.3.0 - 32260*/
UInt32 deleteMACUEEntity( UInt32 ueIndex, 
		/*-COVERITY 5.3.0 - 32260*/
		UInt32 transactionId,
		InternalCellIndex internalCellIndex
		)
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__); 
	UInt32 resp = MAC_DELETION_PENDING;
	ULUEContext *ulUEContext_p = PNULL;
	DLUEContext *dlUEContext_p = PNULL;
    /* SPR 15909 fix start */
    tickType_t tti = 0;
    /* SPR 15909 fix end */

	dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
	if ( !dlUEContext_p )
	{
		lteWarning("DL - UE Index does not exist\n");
		return MAC_FAILURE;
	}
	/* ICIC changes start*/
	if(dlUEContext_p->userLocationType == CE_USER)
	{
		--(rrmMacCellGlobal_gp[internalCellIndex]->totalCellEdgeUserCount);
	}

	deleteUEPeriodicReportBatchInfo(ueIndex,internalCellIndex);
	/* ICIC changes end*/
	if(dlUEContext_p->tcrntiFlag)
	{
		/*CA Changes start  */
		if(MAC_SUCCESS ==  deleteMACDLUEConnHandOverEntity(ueIndex,\
					internalCellIndex))
			/*CA Changes end  */
		{    
			return resp;
		}
		else
		{
			return MAC_FAILURE;
		}

	}

	ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
	if ( !ulUEContext_p )
	{
		lteWarning("UL -UE Index doest not exist\n");
		return MAC_FAILURE;
	}

	tti = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);

	putEntryInPendingDeleteUEQueue(ueIndex,transactionId,tti,CRNTI  
			,internalCellIndex
			);

	/*CA Changes start  */
	qosProcessTknBktAlgoForUeDelete(ueIndex, internalCellIndex);
	/*CA Changes end  */

	LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UE_ACTIVE(ueIndex, FALSE);

	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
	return resp;
}

/****************************************************************************
 *  Function Name  : resetUECnf
 *  Inputs         : ueIndex, transactionId,
 *                   internalCellIndex - internalCellIndex of cell used at MAC
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This API creates response for the UE reset and send this response to RRC.
 *                  
 ****************************************************************************/

void resetUECnf( UInt32 ueIndex,
		UInt32 transactionId 
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA Changes end */
	       )
{
	UEEntityResetRes resetResp = {0};

	resetResp.response = MAC_SUCCESS;
	/* SPR 5085 Fix Start */
	resetResp.idx = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->rrcUeIndex;
	/* SPR 5085 Fix End */
	resetResp.transactionId = transactionId;
	if(MAC_SEND_ERROR == sendMsgToRRC(MAC_RESET_UE_ENTITY_CNF, &resetResp
				/* CA changes Start */
				,macCommonGetRRCCellIndex(internalCellIndex)
				/* CA Changes end */
				))
	{
		ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
	}
}


/****************************************************************************
 * Function Name  : deleteUECnf
 * Inputs         : ueIndex - Index at which UE Context created.
 *                  transactionId - identifier received in request from RRC
 *                  rrcUeIndex - rrc UE Identifier for multisector
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This API receives deleteCnf from UL & DL and if UE
 *                  context deleted successfully at all modules, it deletes 
 *                  its entry from tables and send cnf to RRC.
 ****************************************************************************/
    /* + SPR_17858_633_CHANGES */
void deleteUECnf( UInt32 ueIndex, 
		UInt32 transactionId,
		UInt32 ueId
        ,UInt16 tempContextIndex
		/* SPR 5051 Fix Start */
		,UInt32 rrcUeIndex
		/* SPR 5051 Fix End */
		/*CA Changes start  */
		,InternalCellIndex internalCellIndex
		/*CA Changes end  */
		)
/*Jira_633_Changes End*/
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	UEEntityDeleteRes deleteResp = {0};
     
	RRCReqAPI msgCnf = MAC_DELETE_UE_ENTITY_CNF;
     
      /*Jira_633_Changes Start*/ 
    /* + SPR_17858_633_CHANGES */
    TempCRNTICtx *tempCRNTICtx_p = PNULL;
    RNTIInfo *rntiInfo_p = PNULL;
    if (INVALID_UE_INDEX != tempContextIndex)
    {
        tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->
            tempCRNTICtxArray[tempContextIndex].tempCRNTICtx_p;
    }
    /* - SPR_17858_633_CHANGES */
	if ((PNULL == dlUECtxInfoArr_g[ueIndex].dlUEContext_p) &&
			(PNULL == ulUECtxInfoArr_g[ueIndex].ulUEContext_p))
	{
    /* + SPR_17858_633_CHANGES */
    
        if (PNULL == tempCRNTICtx_p)
        { 
    /* - SPR_17858_633_CHANGES */
		/* Mark entry into crntiToIdxTbl_g */
		/*CA Changes start  */
		if (MAC_FAILURE == freeRNTIAfterUEDeletion(ueId, internalCellIndex))
			/*CA Changes end  */
		{
			LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR,
					"[%s]: ERROR: Failed to return TCRNTI [%d] to free pool\n",
					__func__,ueId);
		}
             /*Jira_633_Changes Start*/
    /* + SPR_17858_633_CHANGES */
        }
        else
        {
            rntiInfo_p = getUeIdxFromRNTIMap(ueId,internalCellIndex);
            if (rntiInfo_p)
            {
                rntiInfo_p->index = tempContextIndex;
                rntiInfo_p->rntiFlag = TCRNTI;
                ((TempContextTimerNode *)tempCRNTICtx_p->timerNode_p)->\
                    deleteFlag = TRUE;
                LOG_MSG(PREAMBLE_NOT_BUSY_ID,
                        LOGERROR,
                        MAC_RRC_INF,
                        getCurrentTick(),
                        ueIndex,
                        ueId,
                        0,
                        LINE_NUMBER,
                        tempContextIndex,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"TempCTxDelete");

            }
            else
            {
                LOG_MSG(PREAMBLE_NOT_BUSY_ID,
                        LOGERROR,
                        MAC_RRC_INF,
                        getCurrentTick(),
                        ueIndex,
                        ueId,
                        0,
                        LINE_NUMBER,
                        tempContextIndex,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"RntiInfoNull");
            }

        }

    /* - SPR_17858_633_CHANGES */
		deleteResp.response = MAC_SUCCESS;
		/* SPR 5051 Fix Start */
		deleteResp.idx      = rrcUeIndex;
		/* SPR 5051 Fix End */
		deleteResp.transactionId = transactionId;
		/* Total Number Cell Activate updated */
		/*CA Changes start  */
		cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->totalActiveUE--;
		/*CA Changes end  */
#ifdef SSI_DEBUG_STATS
		/* CA Stats Changes Start */
		--gMacStats.pStats->numActiveUe[internalCellIndex];
		/* CA Stats Changes End */
#endif        
		if(MAC_SEND_ERROR == sendMsgToRRC(msgCnf, &deleteResp
					/* CA changes Start */
					,macCommonGetRRCCellIndex(internalCellIndex)
					/* CA Changes end */
					))
		{
			ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
		}
		LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
	}    
}


/****************************************************************************
 * Function Name  : reconfigureMACUEEntity
 * Inputs         : dwlkUEInfo_p - Pointer to MAC DL Info 
 *                  uplkUEInfo_p - Pointer to MAC UL Info 
 *                  uplkLCCount - Number of LCs needs to be reconfigured
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                  ulReconfigUeReq_p - pointer to UL UE Reconfigure request
 *                  dlReconfigUeReq_p -  pointer to UL UE Reconfigure request
 *                  currSFN, currSF,
 *                  gapPatternId - Measurement gap pattern
 *                  gapBitMask - Pointer to measuremnet gap changes info Bitmap
 *                  reconfigInfo_p - Pointer to reconfiguration Info changes
 * Outputs        : None
 * Returns        : MAC_SUCCESS, MAC_FAILURE
 * Description    : This API will reconfigure the UE Context at MAC for DL
 *                  and UL.
 ****************************************************************************/
/*Reconfig Changes Start */
/*+COVERITY 5.3.0 - 32264*/
UInt32 reconfigureMACUEEntity( UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink *uplkUEInfo_p, 
		UInt32 uplkLCCount,
		InternalCellIndex internalCellIndex
		/*-COVERITY 5.3.0 - 32264*/
        /*SPR 16855 +-*/
		,ULReconfigUeReq *ulReconfigUeReq_p,
		DLReconfigUeReq *dlReconfigUeReq_p,
		UInt32 currSFN, UInt32 currSF,
		UInt8 gapPatternId,
		UInt8 *gapBitMask,
		UeContextForReconfig *reconfigInfo_p 
        /*SPR 16855 +-*/
		)
/*Reconfig Changes End */
{
	UInt32 ulResp = MAC_SUCCESS;
	UInt32 dlResp = MAC_SUCCESS;
	DLUEContext *dlUEContext_p = PNULL;
	ULUEContext *ulUEContext_p = PNULL; 
	UInt32 ueIndex = dwlkUEInfo_p->ueIndex;
    /*SPR 16855 +-*/
	ULCQIPeriodicReqInfo        *ulCqiPeriodicReqInfo_p        = PNULL;     
	ULCQIReqInfo                *ulCqiReqInfo_p                = PNULL;     
	RRCInfPUCCHConfigReqInfo    *rrcInfPUCCHConfigReqInfo_p    = PNULL;     
	SRSConfigInfo                *tempsrsConfigInfo            = PNULL;
	/* Review comment fix start PUCCH_ENH_2 */
	PucchConfig                  *tempPucchConfigInfo_p        = PNULL;     
	/* Review comment fix end PUCCH_ENH_2 */
     /* + SPR_16082 */
	MeasGapContextDL             tempDLMeasGapContextInfo; 
	MeasGapContextUL             tempULMeasGapContextInfo;
    memSet(&tempDLMeasGapContextInfo,0,sizeof(MeasGapContextDL));
    memSet(&tempULMeasGapContextInfo,0,sizeof(MeasGapContextUL));
     /* - SPR_16082 */
	ULSRSDedicatedConfigReqInfo *ulSRSDedicatedConfigReqInfo_p = PNULL;
	UInt8                        dlMeasGapConfigType           = 0;
	UInt8                        ulMeasGapConfigType           = 0;
    /*SPR 16855 +-*/
    memSet(&tempDLMeasGapContextInfo,0,sizeof(MeasGapContextDL));
    memSet(&tempULMeasGapContextInfo,0,sizeof(MeasGapContextUL));
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;

	if (( PNULL == dlUEContext_p) || ( PNULL == ulUEContext_p) ) 
	{
		lteWarning("UE does not exist corresponding to this IDx\n");
		return MAC_FAILURE;
	}
	if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag || 
			ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
	{
		lteWarning("Pending Delete Flag is set for this UEIndex. %d\n",ueIndex);
		return MAC_FAILURE;
	}
    /*SPR 16855 +-*/
	ulReconfigUeReq_p->ueIndex = dwlkUEInfo_p->ueIndex;
	rrcInfPUCCHConfigReqInfo_p =  
		&(ulReconfigUeReq_p->rrcInfpucchConfigReqInfo);
	ulCqiReqInfo_p = &(ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.ulCqiReqInfo);

	/* 11541 complete fix start */
	rrcInfPUCCHConfigReqInfo_p->activeIndexForPUCCHMap =
		longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
	/* 11541 complete fix end */

	ulCqiPeriodicReqInfo_p = 
		&(ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.ulCqiReqInfo.cqiPeriodicReqInfo);
	ulSRSDedicatedConfigReqInfo_p = 
		&(ulReconfigUeReq_p->ulSRSDedicatedConfigReqInfo);
	/* Review comment fix start PUCCH_ENH_2 */
	tempPucchConfigInfo_p = &(ulUEContext_p->pucchConfigInfo);
	/* Review comment fix end PUCCH_ENH_2 */
	tempsrsConfigInfo   = &(ulUEContext_p->srsConfigInfo);
	tempDLMeasGapContextInfo = dlUEContext_p->measGapContext;
	tempULMeasGapContextInfo = ulUEContext_p-> measGapContext;
    /*SPR 16855 +-*/
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    if (mocnOperatorParams_g.isMocnEnabled)
    {
        dlReconfigUeReq_p->operatorId = dwlkUEInfo_p->operatorId;
        ulReconfigUeReq_p->operatorId = uplkUEInfo_p->operatorId;
    }
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
	/* + TM7_8 Changes Start */
	if(TX_MODE_8 == dwlkUEInfo_p->transmissionMode)
	{
		if((INVALID_PMI_RI_CONF_V920 != dlUEContext_p->isPmiRiConfigured_V920)&&
				(INVALID_PMI_RI_CONF_V920 == dwlkUEInfo_p->pmiRiReportV920))
		{
			dwlkUEInfo_p->pmiRiReportV920 = dlUEContext_p->isPmiRiConfigured_V920;
			uplkUEInfo_p->pmiRiReportV920 = dlUEContext_p->isPmiRiConfigured_V920;
		}
		if(TRUE != dwlkUEInfo_p->pmiRiReportV920)
		{
			uplkUEInfo_p->riConfigIndex = MAC_RI_CONFIG_INDEX_INVALID;
			ulUEContext_p->pucchConfigInfo.riConfigIndex = MAC_RI_CONFIG_INDEX_INVALID; 
		}
	}
	/* CA Changes start */
	UInt8 scellCount = 0, scellIndex = 0;
	for (scellCount = 0; scellCount < dwlkUEInfo_p->carrierAggrConfig.scellCount
			; scellCount++ )
	{
		if (RECONFIGURE == dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].
				operationType)
		{
			scellIndex = dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].sCellIndex;
			if(TX_MODE_8 == dwlkUEInfo_p->carrierAggrConfig.scellInfo
					[scellCount].transmissionMode)
			{
				if((INVALID_PMI_RI_CONF_V920 != dlUEContext_p->
							dlUeScellContext_p[scellIndex]->isPmiRiConfigured_V920)&&
						(INVALID_PMI_RI_CONF_V920 == dwlkUEInfo_p->
						 carrierAggrConfig.scellInfo[scellCount].cqiInfo.pmiRiReportV920))
				{
					dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].
						cqiInfo.pmiRiReportV920 = dlUEContext_p->dlUeScellContext_p
						[scellIndex]->isPmiRiConfigured_V920;
					uplkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].
						cqiInfo.pmiRiReportV920 = dlUEContext_p->dlUeScellContext_p
						[scellIndex]->isPmiRiConfigured_V920;
				}
				if(TRUE != dwlkUEInfo_p->carrierAggrConfig.
						scellInfo[scellCount].cqiInfo.pmiRiReportV920)
				{
					uplkUEInfo_p->carrierAggrConfig.scellInfo[scellIndex].
						cqiInfo.riConfigIndex = MAC_RI_CONFIG_INDEX_INVALID;
					uplkUEInfo_p->carrierAggrConfig.scellInfo[scellIndex].
						cqiInfo.riConfigIndexV1020 = MAC_RI_CONFIG_INDEX_INVALID;
				}
			}
			/* + CQI_5.0 */
			/* Check if transmission mode is reconfigured and CQI Tag was received in
			 * create but not in reconfigure.*/
			if ((INVALID_TX_MODE != dwlkUEInfo_p->carrierAggrConfig.scellInfo
						[scellCount].transmissionMode) &&
					/* SPR 12554 Fix start */
					/*Check has been limited to PERIODIC CQI MODE. Because if APERIODIC CQI 
					  configuration is not present in reconfigure UE msg and earlier UE CQI mode
					  is configured as APERIODIC then in this case L2 released APERIODIC MODE CQI.*/ 
					(PERIODIC_CQI_REPORTING_MODE == dlUEContext_p->
					 dlUeScellContext_p[scellIndex]->ScelldlCQIInfo.cqiTypeIndicator) &&
					/* SPR 12554 Fix end */
					(INVALID_CQI_REPORTING_MODE == dwlkUEInfo_p->
					 carrierAggrConfig.scellInfo[scellCount].cqiInfo.cqiIndication ))
			{

				if( MAC_FAILURE ==  
						(validateCQIModeForNewTxMode(dwlkUEInfo_p->carrierAggrConfig.scellInfo
									     [scellCount].transmissionMode,
									     dlUEContext_p->
									     dlUeScellContext_p[scellIndex]->ScelldlCQIInfo.cqiTypeIndicator,
									     ulUEContext_p->ulUeScellContext_p[scellIndex]->
									     scellpucchConfigInfo.cqiPmiRiInfo.periodicRepMode,
									     ulUEContext_p->ulUeScellContext_p[scellIndex]->
									     scellpucchConfigInfo.cqiPmiRiInfoAperiodic.cqiMode,
									     /* + TM7_8 Changes Start */
									     dwlkUEInfo_p->carrierAggrConfig.
									     scellInfo[scellCount].cqiInfo.pmiRiReportV920)))
					/* - TM7_8 Changes End */
				{

					lteWarning( "TX mode [%d] is not compatible with CQI modes \n",
							dwlkUEInfo_p->carrierAggrConfig.scellInfo
							[scellCount].transmissionMode);
					return  MAC_FAILURE ;
				}   
			}
			/* - CQI_5.0 */
		}
	}
	/* CA changes end */
	/* - TM7_8 Changes End */

	/* + CQI_5.0 */
	/* Check if transmission mode is reconfigured and CQI Tag was received in
	 * create but not in reconfigure.*/
	if ((INVALID_TX_MODE != dwlkUEInfo_p->transmissionMode) &&
			/*SPR 12554 Fix Start*/
			/*Check has been limited to PERIODIC CQI MODE. Because if APERIODIC CQI 
			  configuration is not present in reconfigure UE msg and earlier UE CQI mode
			  is configured as APERIODIC then in this case L2 released APERIODIC MODE CQI.*/ 
			(PERIODIC_CQI_REPORTING_MODE == dlUEContext_p->dlCQIInfo.cqiTypeIndicator) &&
			/*SPR 12554 Fix End*/
			(INVALID_CQI_REPORTING_MODE == dwlkUEInfo_p->cqiIndication ))
	{

		if( MAC_FAILURE ==  
				(validateCQIModeForNewTxMode(dwlkUEInfo_p->transmissionMode,
							     dlUEContext_p->dlCQIInfo.cqiTypeIndicator,
							     ulUEContext_p->pucchConfigInfo.cqiPmiRiInfo.periodicRepMode,
							     ulUEContext_p->pucchConfigInfo.cqiPmiRiInfoAperiodic.cqiMode,
							     /* + TM7_8 Changes Start */
							     dwlkUEInfo_p->pmiRiReportV920)))
			/* - TM7_8 Changes End */
		{

			lteWarning( "TX mode [%d] is not compatible with CQI modes \n",
					dwlkUEInfo_p->transmissionMode);
			return  MAC_FAILURE ;
		}   
	}
	/* - CQI_5.0 */
	/** HD FDD Changes **/
#ifdef HD_FDD_CONFIG

    /*SPR 16855 +-*/
	/* in case of UL/DL 
	   split, store the params in local data base */
	dlReconfigUeReq_p->rfParams = dwlkUEInfo_p->isHDFddFlag;
	ulReconfigUeReq_p->rfParams = uplkUEInfo_p->isHDFddFlag;
    /*SPR 16855 +-*/
#endif
	/** HD FDD Changes **/

    /*SPR 16855 +-*/
	dlResp = reconfigureMACDLUEEntity(dwlkUEInfo_p, dlReconfigUeReq_p, internalCellIndex);
    /*SPR 16855 +-*/
    /* +- SPR 17777 */
	ulResp = reconfigureMACULUEEntity( uplkUEInfo_p,
			uplkLCCount,ulReconfigUeReq_p,
			/* CA Changes Start */
			internalCellIndex
			/* CA changes End */
			);
    /* +- SPR 17777 */
    /*SPR 16855 +-*/

	/* SPR 11777 Changes Start */
	/*
	 ** Removed the SPR 2454 Changes from here as UE category is already validated and
	 ** populated in ulReconfigUeReq_p inside the reconfigureMACULUEEntity().
	 */
	/* SPR 11777 Changes End */

	/* + TM6_5.2 */
	if ((TX_MODE_1 ==  dwlkUEInfo_p->transmissionMode) || 
			(TX_MODE_2 ==  dwlkUEInfo_p->transmissionMode) ||
			(TX_MODE_6 == dwlkUEInfo_p->transmissionMode)||
			(TX_MODE_7 == dwlkUEInfo_p->transmissionMode))
		/* - TM7_8 Changes End */
	{
		dwlkUEInfo_p->rankIndicator = RANK_INDICATOR_1;
	}
	/* - TM6_5.2 */

	/* + CQI_5.0 */
    /*SPR 16855 +-*/
	dlReconfigUeReq_p->numOfLayersInfo.numOfLayers  = dwlkUEInfo_p->rankIndicator;
    /*SPR 16855 +-*/
	ulSRSDedicatedConfigReqInfo_p->switchPassiveToActiveSRSFlag = 
		INVALID_SRS_REQ_TYPE ;
	/*Reconfig Changes Start */
	if ( MAC_FAILURE == reconfigurePUCCHSRSAndReports(
                /* +- SPR 17777 */
				ulUEContext_p->ueIndex,
				/* Review comment fix start PUCCH_ENH_2 */
				tempPucchConfigInfo_p,
				/* Review comment fix end PUCCH_ENH_2 */
				tempsrsConfigInfo,
				dwlkUEInfo_p,uplkUEInfo_p,
				ulReconfigUeReq_p, currSFN, 
				currSF,reconfigInfo_p,
				internalCellIndex))
		/*Reconfig Changes End */
	{
		return MAC_FAILURE;
	}
	else
	{
		/*eicic changes*/
		if (SETUP >= uplkUEInfo_p->cqiSubsetConfigRequestType)
		{
            /* +- SPR 17777 */
#ifdef TDD_CONFIG
			updateCsiMeasurementSubset(ulUEContext_p->pucchConfigInfo.csiMeasurementSubset1,uplkUEInfo_p->csiMeasurementSubset1,internalCellIndex);
			updateCsiMeasurementSubset(ulUEContext_p->pucchConfigInfo.csiMeasurementSubset2,uplkUEInfo_p->csiMeasurementSubset2,internalCellIndex);
#else
            updateCsiMeasurementSubset(ulUEContext_p->pucchConfigInfo.csiMeasurementSubset1,uplkUEInfo_p->csiMeasurementSubset1);
            updateCsiMeasurementSubset(ulUEContext_p->pucchConfigInfo.csiMeasurementSubset2,uplkUEInfo_p->csiMeasurementSubset2);
#endif
            /* +- SPR 17777 */

        }
		/*eicic changes*/
		ULCQIReqInfo *ulCqiReqInfo_p = &ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.ulCqiReqInfo;
		RRCInfPUCCHConfigReqInfo *rrcInfPUCCHConfigReqInfo_p = &ulReconfigUeReq_p->rrcInfpucchConfigReqInfo;
		if((rrcInfPUCCHConfigReqInfo_p->pucchConfigMsg & UL_RECONFIG_CQI_IND)&&
				(UL_RECONFIG_NO_CQI_MSG == ulCqiReqInfo_p->cqiConfigMsg))
        {
			rrcInfPUCCHConfigReqInfo_p->pucchConfigMsg &=  UL_RECONFIG_CLEAR_CQI_IND;
		}

		if(UL_RECONFIG_NO_PUCCH_MSG != rrcInfPUCCHConfigReqInfo_p->pucchConfigMsg) 
        {
			ulReconfigUeReq_p->ueReconfigTlvRecieved[ulReconfigUeReq_p->ueReconfigMsgTlvCount]=
				UL_RECONFIG_PUCCH_CONFIG_INFO;
			ulReconfigUeReq_p->ueReconfigMsgTlvCount++;
        }
		if(INVALID_SRS_REQ_TYPE != ulSRSDedicatedConfigReqInfo_p->switchPassiveToActiveSRSFlag)
		{
			ulReconfigUeReq_p->ueReconfigTlvRecieved[ulReconfigUeReq_p->ueReconfigMsgTlvCount]=
				UL_RECONFIG_SRS_DEDICATED_CONFIG;
			ulReconfigUeReq_p->ueReconfigMsgTlvCount++;

        }
    }
    /*SPR 16855 +-*/

	/* SRS_CHG */
	/*FDD_SRS*/

	/* +  DRX_CHG  */
	if ( TRUE == dwlkUEInfo_p->drxRequestType )
	{    
        /*SPR 16855 +-*/
		dlReconfigUeReq_p->drxConfigInfo.drxConfigType = dwlkUEInfo_p->drxConfigType;
		dlReconfigUeReq_p->drxConfigInfo.drxConfig = dwlkUEInfo_p->drxConfig;
		dlReconfigUeReq_p->drxConfigInfo.cqiMaskV920 = uplkUEInfo_p->cqiMaskV920;
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] At LP side \n"\
				"isDRXShortCycleConfigured = %d \n"\
				"drxOnDurationTimerIndex = %d \n"\
				"drxinactivityTimerIndex = %d \n"\
				"shortDRXCycleIndex = %d \n "\
				"longDRXCycleIndex = %d\n"\
				"drxOnDurationTimer = %d\n"\
				"drxRetransmissionTimer=%d \n"\
				"drxShortCycleTimer= %d\n"\
				"drxInactivityTimer = %d \n"\
				"shortDRXCycle  = %d \n"\
				"longDRXCycle = %d \n"\
				"drxStartOffset  = %d \n"\
				"CQI Mask = %d \n",__func__,
				dlReconfigUeReq_p->drxConfigInfo.drxConfig.isDRXShortCycleConfigured,
				dlReconfigUeReq_p->drxConfigInfo.drxConfig.drxOnDurationTimerIndex,
				dlReconfigUeReq_p->drxConfigInfo.drxConfig.drxInactivityTimerIndex,
				dlReconfigUeReq_p->drxConfigInfo.drxConfig.shortDRXCycleIndex,
				dlReconfigUeReq_p->drxConfigInfo.drxConfig.longDRXCycleIndex,
				dlReconfigUeReq_p->drxConfigInfo.drxConfig.drxOnDurationTimer,
				dlReconfigUeReq_p->drxConfigInfo.drxConfig.drxRetransmissionTimer,
				dlReconfigUeReq_p->drxConfigInfo.drxConfig.drxShortCycleTimer,
				dlReconfigUeReq_p->drxConfigInfo.drxConfig.drxInactivityTimer,
				dlReconfigUeReq_p->drxConfigInfo.drxConfig.shortDRXCycle,
				dlReconfigUeReq_p->drxConfigInfo.drxConfig.longDRXCycle,
				dlReconfigUeReq_p->drxConfigInfo.drxConfig.drxStartOffset,
				dlReconfigUeReq_p->drxConfigInfo.cqiMaskV920);
    /*SPR 16855 +-*/
	}
	/* -  DRX_CHG  */
	/* + MEAS_GAP_CHG */
	if ( TRUE ==  dwlkUEInfo_p->measGapRequestType )
	{
        /*SPR 16855 +-*/
		dlMeasGapConfigType = dlUEContext_p->measGapConfigType;
		ulMeasGapConfigType = ulUEContext_p->measGapConfigType;
        /* SPR 17944 Start */
        /* copy both the index, 40 and 80 ms meas gap so that the condition
         * of switching from 40 to 80 or vice versa is taken care. */
            /* SPR 16082 fix end */
        dlReconfigUeReq_p->rrcMeasGapConfig.activeIndexFor40MsMeasGapMap =
            measGap40msInfo_gp[internalCellIndex]->passiveIndex;
        dlReconfigUeReq_p->rrcMeasGapConfig.activeIndexFor80MsMeasGapMap =
            measGap80msInfo_gp[internalCellIndex]->passiveIndex;
        /* SPR 17944 End */
#ifdef ENDC_ENABLED
		/*Meas_Gap_Changes_Start*/
		dlReconfigUeReq_p->rrcMeasGapConfig.activeIndexFor20MsMeasGapMap =
            measGap20msInfo_gp[internalCellIndex]->passiveIndex;
        dlReconfigUeReq_p->rrcMeasGapConfig.activeIndexFor160MsMeasGapMap =
            measGap160msInfo_gp[internalCellIndex]->passiveIndex;
            /*Meas_Gap_Changes_End*/
#endif
		processMeasGapReConfig(dwlkUEInfo_p, &tempDLMeasGapContextInfo,
				&tempULMeasGapContextInfo,&dlMeasGapConfigType, 
				&ulMeasGapConfigType,dlUEContext_p->ueIndex,gapPatternId,gapBitMask,
				internalCellIndex);

		dlReconfigUeReq_p->rrcMeasGapConfig.dlMeasGapConfig.measGapConfigType =
			dwlkUEInfo_p->measGapConfigType;
		dlReconfigUeReq_p->rrcMeasGapConfig.dlMeasGapConfig.measGapContextDL = tempDLMeasGapContextInfo;
		dlReconfigUeReq_p->rrcMeasGapConfig.ulMeasGapConfig.measGapConfigType =
			dwlkUEInfo_p->measGapConfigType;
		dlReconfigUeReq_p->rrcMeasGapConfig.ulMeasGapConfig.measGapContextUL = tempULMeasGapContextInfo;
        /*SPR 16855 +-*/
	}        
	/* - MEAS_GAP_CHG */
    /*SPR 16855 +-*/
	if((rrcInfPUCCHConfigReqInfo_p->pucchConfigMsg > UL_RECONFIG_NO_PUCCH_MSG ) &&
			((UL_RECONFIG_CQI_APERIODIC_MSG != 
			  ulCqiReqInfo_p->cqiConfigMsg) ||
			 (UL_RECONFIG_CQI_RI_NO_RECONFIG_MSG != 
			  ulCqiPeriodicReqInfo_p->cqiRiReconfigMsg)))    
	{
		/* This variable will be set only when any of request comes with or without aperiodic CQI.
		 * If only aperiodic CQI comes it will not be set  */ 
		ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.switchPassiveToActiveFlag = 1;

	}
    /*SPR 16855 +-*/
	if(MAC_SUCCESS != dlResp)
	{
		return dlResp;
	}
	else if (MAC_SUCCESS != ulResp)
	{
		return ulResp;
	}

	LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UE_ACTIVE(ueIndex, TRUE);

    /*SPR21501 fix start+-*/
    /*SPR 16855 +-*/
	ulReconfigUeReq_p->ttiBundlingEnabled = uplkUEInfo_p->ttiBundlingEnabled;
    /*SPR 16855 +-*/

	/* CA changes Start */
	if (MAC_SUCCESS != macFillReconfigureScellCommonParams (ulUEContext_p,
				dwlkUEInfo_p,
				uplkUEInfo_p,internalCellIndex
                /*SPR 16855 +-*/
				,ulReconfigUeReq_p
				,dlReconfigUeReq_p
				,reconfigInfo_p)
            /*SPR 16855 +-*/
	   )
	{
		lteWarning("UE Context params filling failed");
		ulResp = MAC_FAILURE; 
	}
	/* CA changes End */
	return ulResp;
}

/* Coverity 32454 Fix Start */
/******************************************************************************
 * Function Name  : deletePartialCreatedUplinkUEContext
 * Inputs         : ueULContext_p - Pointer to UE context Info
 *                                      
 * Outputs        : None 
 * Returns        : None 
 * Description    :The function is used to delete partially created UE context 
 to release memory allocated for ULUE Context.
 *******************************************************************************/
STATIC  void deletePartialCreatedUplinkUEContext(ULUEContext* ueULContext_p)
{
	UInt8 lcCount=0;
	ULLogicalChannelInfo *ulLogicalChInfo_p  = PNULL;
	/* CA changes Start */
	UInt8 scellId = 1;
	/* CA changes End */
	/* SPR 13492 Changes Start */
#ifdef KPI_STATS
	UInt8 qci       = INVALID_QCI;
#endif
	/* SPR 13492 Changes End */

	if(PNULL != ueULContext_p)
	{
		for(lcCount=0;lcCount<MAX_NUMBER_OF_LOGICAL_CHANNEL;lcCount++)
		{
			ulLogicalChInfo_p=&(ueULContext_p->lcInfo[lcCount]);
			if(ulLogicalChInfo_p->ulQosInfo.ulQosStrategyLCNode_p)
			{
				freeMemPool(ulLogicalChInfo_p->ulQosInfo.ulQosStrategyLCNode_p);
				ulLogicalChInfo_p->ulQosInfo.ulQosStrategyLCNode_p=PNULL;
			}
		}
		/* CA changes Start */
		/* Klockwork Changes Start */
		for (scellId = MAX_NUM_PCELL; scellId <= MAX_NUM_SCELL; scellId++)
		{
			/* Klockwork Changes End */
			if (ueULContext_p->ulUeScellContext_p[scellId] != PNULL)
			{
				freeMemPool(ueULContext_p->ulUeScellContext_p[scellId]);
				ueULContext_p->ulUeScellContext_p[scellId] = PNULL;
			}
		}
		/* CA changes End */

		/*Delete UL LC Strategy Node from UL UE's AMBR info*/ 
		if(ueULContext_p->ulAmbrInfo.ulQosStrategyLCNode_p)
		{
			freeMemPool(ueULContext_p->ulAmbrInfo.ulQosStrategyLCNode_p);
			ueULContext_p->ulAmbrInfo.ulQosStrategyLCNode_p=PNULL;
		}

		if (PNULL != ueULContext_p->csiContext.mcsToPrbMap_p)
		{
			freeMemPool(ueULContext_p->csiContext.mcsToPrbMap_p);
			ueULContext_p->csiContext.mcsToPrbMap_p = PNULL;
		}
		/* SPR 13492 Changes Start */
#ifdef KPI_STATS
		for (qci = 0; qci < MAX_QCI ; ++qci)
		{
			ulUECtxInfoArr_g[ueULContext_p->ueIndex]. 
				inSessionTimePerQCIFlag[qci] = FALSE;
		}
#endif
		/* SPR 13492 Changes End */

		freeMemPool(ueULContext_p);
	}

}
/* Coverity 32454 Fix End */

/*****************************************************************************
 * Function Name  : createMACULUEEntity 
 * Inputs         : ueIndex - User Equipment Index
 *                  lchCount - Number of Logical Channels configured for UE
 *                  ueContextInfo_p - Pointer to UE related UL Info
 *                  ueDlConfigInfo_p - Pointer to UE related Dl info
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function creates a UE Context for Uplink at MAC Layer
 *****************************************************************************/

STATIC MacRetType createMACULUEEntity( UInt32 ueIndex, 
		UInt32 lchCount, 
		UeContextForUplink *ueContextInfo_p,
		/* SPR 6690 fix start */
		UeContextForDownlink* ueDlConfigInfo_p,
		/* SPR 6690 fix end */
		/* CA changes Start */
		InternalCellIndex internalCellIndex
		/* CA changes End */
		)
{
    /* SPR 19310 +- */
   CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]-> \
                                    cellParams_p;
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	UInt8 index = 0;
	ULUEContext         *ulUEContext_p    = PNULL;
	MacRetType retValue = MAC_SUCCESS;

	/* Allocate memory for UL Static UE Context */
	GET_MEM_FROM_POOL(ULUEContext,ulUEContext_p,sizeof(ULUEContext), PNULL);

	/* Rel 5.3: Coverity Fix Start */
	if (PNULL == ulUEContext_p)
	{
		/* Not able to allocate Memory for UL UE Entity */
        LOG_MAC_ERROR(MAC_RRC_INF,"Memory Allocation Failure for ulUEContext_p UE :%u\n",ueIndex);
		return MAC_FAILURE;
	}
	/* Rel 5.3: Coverity Fix End */

	memSet(ulUEContext_p,0,sizeof(ULUEContext));

	ulUEContext_p->ueIndex    = ueIndex;

	ulUEContext_p->internalCellIndex = internalCellIndex;
	/* + CQI_5.0 */

	/* SPR 5713 changes start */
	ulUEContext_p->ueComplianceRelease = RELEASE_9;
	ulUEContext_p->ulGrantMinDataSize = ULGRANT_MIN_DATA_SIZE_FOR_REL_9;
//phy release fix
	if((RELEASE_10 == ueContextInfo_p->ueComplianceRelease) 
      )
	{
		ulUEContext_p->ueComplianceRelease = 
			ueContextInfo_p->ueComplianceRelease;
		ulUEContext_p->ulGrantMinDataSize = 
			ULGRANT_MIN_DATA_SIZE_FOR_REL_10;
	}
	/* SPR 5713 changes end */

	ulUEContext_p->crnti = ueContextInfo_p->cRnti;
	/* SR Starvation fix start */
	ulUEContext_p->srNode_p = PNULL;
	/* SR Starvation fix end */

	/*HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
	ulUEContext_p->isHDFddFlag = ueContextInfo_p->isHDFddFlag;
	/*if (TRUE == ulUEContext_p->isHDFddFlag)
	  {
	  hdFddCtxReconfigUpdateReports(ulUEContext_p);
	  }*/
#endif
    /* Initialize force grant parameters */
    {
    ulUEContext_p->numOfForceGrant =  cellParams_p->numEUL;
    }
    /*UL Force Grant Changes Start */
    ulUEContext_p->firstBSRReceived = FALSE;
     /*UL Force Grant Changes End */
	fillULSyncLossAndSchedInfo (ulUEContext_p,internalCellIndex);

	fillULContextLcInfo (ulUEContext_p);

	fillULContextHarqInfo (ulUEContext_p, ueContextInfo_p);
	/* Cyclomatic Complexity changes - ends here */

	/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
	hdUeContextInfo_g.ulSrPresentFlag[ulUEContext_p->ueIndex] = FALSE;
#endif
	/*HD FDD Changes End*/

	/* Cyclomatic Complexity changes - starts here */
	fillULUECategoryAndMcsInfo (ueIndex, ulUEContext_p, ueContextInfo_p,
			internalCellIndex);

	/* Coverity 36237 Fix Start */
    /*SPR21501 fix +-*/
	ulUEContext_p->ttiBundlingEnabled = FALSE;
#ifdef FDD_CONFIG
    /* SPR 19031 fix start */
    ulUEContext_p->aperiodicCQIWithoutDataTTI = 0;
    /* SPR 19031 fix end */
#endif
    {
	if (MAC_SUCCESS != fillULSPSInfo (ueIndex, ulUEContext_p, ueContextInfo_p, 
				ueDlConfigInfo_p,internalCellIndex))
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"fillULSPSInfo failed during createMACULUEEntity UE %u\n",
                ueIndex);
		retValue = MAC_FAILURE;
	}
    }
	/* Coverity 36237 Fix End */


	if (MAC_SUCCESS != fillULContextAmbrInfo (lchCount, ulUEContext_p, 
				ueContextInfo_p
				/* CA changes Start */
				,internalCellIndex
				/* CA changes End */
				))
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"fillULContextAmbrInfo failed during createMACULUEEntity UE %u\n",
        ueIndex);
		retValue = MAC_FAILURE;
	}

    {
	if (MAC_SUCCESS != fillULPowerCtrlAndCsiInfo (ulUEContext_p, ueContextInfo_p, internalCellIndex))
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"fillULPowerCtrlAndCsiInfo failed during createMACULUEEntity UE %u\n",
        ueIndex);
		retValue = MAC_FAILURE;
	}
    }

	/* Coverity 32454 Fix Start */
	if(MAC_FAILURE == retValue)
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"deletePartialCreatedUplinkUEContext or UE %u\n",
        ueIndex);
		deletePartialCreatedUplinkUEContext(ulUEContext_p);
		return retValue;
	}
	/* Coverity 32454  Fix End */

      /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
       /* In case of HandOver , uplink context needs to be filled with pasrsed
        * value for RAT 1 support info which is informed via RRC to mac in 
        * MAC_CREATE_UE_ENTITY_REQ */
       ulUEContext_p->isUplinkRAT1Supported = ueContextInfo_p->isUplinkRAT1Supported;  
#endif
     /* SPR 21958 PUSCH RAT1 Support End */
    {
	/* PUCCH Config*/
	fillULPucchConfigInfo (ulUEContext_p, ueContextInfo_p, internalCellIndex);

#ifdef TDD_CONFIG   
	fillULUeScheduledInfo (ulUEContext_p, internalCellIndex);
#endif

	/* PUSCH Config */
	fillULPuschAndCqiInfo (ulUEContext_p, ueContextInfo_p,internalCellIndex);

	fillULUserLocTypeAndPhrMimoInfo (ulUEContext_p, ueContextInfo_p, internalCellIndex);
	/* CA changes Start */
    /* +- SPR 17777 */
	macUpdateCreateULCAParams (ulUEContext_p,
			ueContextInfo_p);
    }
    /* +- SPR 17777 */
	for (index = 0; index <ueContextInfo_p->carrierAggrConfig.scellCount; index ++)
	{
		if (MAC_SUCCESS != macUpdateCreateULScellParams (ulUEContext_p,
					ueContextInfo_p,index, internalCellIndex))
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"failure in macUpdateCreateULScellParams UE %u\n",
                    ulUEContext_p->ueIndex);
			retValue = MAC_FAILURE; 
		}
	}
	/* CA changes End */

	/* + E_CID_5.3 */
	resetULEcidReportInfo(ulUEContext_p);
	/* - E_CID_5.3 */
	/* Cyclomatic Complexity changes - ends here */
	/*+ Dyn UEs To Be SchPerTTI +*/
	ulUEContext_p->isUEConsideredForTotalActiveUeUL = FALSE;
	/*- Dyn UEs To Be SchPerTTI -*/
    /* + SPR 21811 Fix */
    ulUEContext_p->firstSRSIndRecvd = FALSE;
    /* - SPR 21811 Fix */
	/* Added in the global Array*/
	ulUECtxInfoArr_g[ueIndex].ulUEContext_p = ulUEContext_p;
	/*+ SPR 8564*/
	ulUECtxInfoArr_g[ueIndex].ulUEContext_p->waitForMsg3BeforeResetting = FALSE;
	/*- SPR 8564*/
	/*EICIC Changes Starts*/


	ulUEContext_p->userType = NON_VICTIM_UE;

	if (LTE_TRUE == ueContextInfo_p->userTypePresent)   
	{
		ulUEContext_p->userType = ueContextInfo_p->userType;
	}	
	/*EICIC Changes Ends*/
	/* SPR 23402 Changes Start */
        /* MOCN Changes Start */
	if (mocnOperatorParams_g.isMocnEnabled)
	{
		ulUECtxInfoArr_g[ueIndex].ulUEContext_p->operatorId = ueContextInfo_p->operatorId;
	} 
	/* MOCN Changes End */
    /* SPR 23402 Changes End */
	return retValue;
}

/* Cyclomatic Complexity changes - starts here */
/******************************************************************************
 * Function Name  : fillULSyncLossAndSchedInfo
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the UL Sync Loss and Scheduler related 
 *                  parameters in UL UE context.
 ******************************************************************************/
STATIC  void fillULSyncLossAndSchedInfo ( ULUEContext  *ulUEContext_p,
		/* CA changes Start */
		InternalCellIndex internalCellIndex
		/* CA Changes end */
		)
{
	/* Review comment fix start MAC_RESET_10 */
	UInt8 n = 0; 
	/* Review comment fix end MAC_RESET_10 */
	ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx = INVALID_PREAMBLE_IDX;
	ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState = UL_SYNC_LOSS_OFF;
	ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount = 0;
	ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderTimerNode = PNULL;

	ulUEContext_p->bsrNetTotal = 0;
	ulUEContext_p->ulSchedulerAlgoInfo.uePriorityFactor       = 0;
	ulUEContext_p->ulSchedulerAlgoInfo.modulationSchemeFactor = 0;
	ulUEContext_p->lastScheduledTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
	/* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
	ulUEContext_p->isAlreadyConsideredFlag = INVALID_ALREADY_CONSIDERED_FLAG;
	ulUEContext_p->isPdcchAllocated = FALSE;
#elif TDD_CONFIG
	for (n = 0; n < MAX_SUBFRAME; n++)
	{
		ulUEContext_p->isAlreadyConsideredFlag[n] = 
			INVALID_ALREADY_CONSIDERED_FLAG;
	}
	for (n = 0; n <MAX_HARQ_PROCESS_NB; n++)
	{
		ulUEContext_p->ulresInfo[n].isPdcchAllocated = FALSE;
	}
#endif
	/* TDD Config 0 Changes End */
	ulUEContext_p->scheduleFlag = UL_QOS_NEW_TRANSMISSION;

	ulUEContext_p->nonZeroBsrList_p = PNULL;
	/* Review comment fix start MAC_RESET_10 */
	for (n = 0; n < NUM_OF_LCG; n++)
	{
		ulUEContext_p->bsrNet[n] = 0;
	}
	/* Review comment fix end MAC_RESET_10 */

	/* SPR 5147 Fix Start */ 
	ulUEContext_p->prioritySchedulingReqUL = FALSE; 
	/* SPR 5147 Fix End */ 
}

/*****************************************************************************
 * Function Name  : fillULContextLcInfo 
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the logical channel information in 
 *                  UL UE context.
 *****************************************************************************/
STATIC  void fillULContextLcInfo ( ULUEContext  *ulUEContext_p )
{
	UInt32 n    = 0;
	UInt8  lcgIndex = 0;
	ULLogicalChannelInfo *lcInfo_p        = PNULL;

	for (n = 0; n < MAX_NUMBER_OF_LOGICAL_CHANNEL; n++)
	{
		lcInfo_p = &ulUEContext_p->lcInfo[n];
		lcInfo_p->lchId = INVALID_LC_ID;
		lcInfo_p->lcGId = LCGID_INVALID_VALUE;
		/*SPR 6663 Fix Start*/
		memSet(&lcInfo_p->ulQosInfo,0,sizeof(ULQosInfo));
		/*SPR 6663 Fix End*/
	}

	ulUEContext_p->lcCount[0] = 0;
	ulUEContext_p->lcCount[1] = 0;
	ulUEContext_p->lcCount[2] = 0;
	ulUEContext_p->lcCount[3] = 0;

	/*Initialize LCG info*/ 
	for (lcgIndex = 0; lcgIndex < NUM_OF_LCG; ++lcgIndex)
	{

		ulUEContext_p->ulLCGInfo[lcgIndex].qLoad = 0;
		ulUEContext_p->ulLCGInfo[lcgIndex].qLoadPriority = 0;
		ulUEContext_p->ulLCGInfo[lcgIndex].lastScheduledTick = 0;
		ulUEContext_p->ulLCGInfo[lcgIndex].numLC = 0;
		ulUEContext_p->ulLCGInfo[lcgIndex].isDrbPresent = FALSE;

		/* UL_Power_Mask_Changes Start */
		ulUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[lcgIndex].lcgId = 
			LCGID_INVALID_VALUE;
		ulUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[lcgIndex].maxQciDeltaPowerMask = 
			0;
		/* UL_Power_Mask_Changes End */

		for (n = 0; n < MAX_NUMBER_OF_LOGICAL_CHANNEL; n++)
		{
			ulUEContext_p->ulLCGInfo[lcgIndex].lcId[n] = INVALID_LC_ID;
		}
	}
}

/*****************************************************************************
 * Function Name  : fillULContextHarqInfo 
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 *                  ueContextInfo_p - Pointer to UE related UL Info
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the HARQ information in UL UE context
 *****************************************************************************/
STATIC  void fillULContextHarqInfo ( ULUEContext        *ulUEContext_p,
		UeContextForUplink *ueContextInfo_p
		)
{
	UInt32    n    = 0;
	/* TDD Config 6 Changes Start */
#ifdef TDD_CONFIG
	UInt8 ulHarqCount = 0;
#endif
	/* TDD Config 6 Changes End */
	ULHarqInfo  *ulHarqInfo_p  = PNULL; 
	/* + SPR_5711 */
#if defined(KPI_STATS) || defined(PERF_STATS)
	/* - SPR_5711 */
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT)
	UInt8  lcIndex  = 0;
#endif
#endif

	/* SPR 14235 Changes Starts */
	UInt8 lcgIndex = 0;
	/* SPR 14235 Changes Ends */

	/* SPR_1987 Start */
	ulUEContext_p->maxUlHARQTx = ueContextInfo_p->maxUlHARQTx;
	/* SPR_1987 End */

	for (n = 0; n <MAX_HARQ_PROCESS_NB; n++)
	{
		ulHarqInfo_p = & ulUEContext_p->ulresInfo[n];
		ulHarqInfo_p->riStart  = 0; /*indicates the starting index of the resorces*/
		ulHarqInfo_p->riLength = 0;    /*indicates number of contigiously allocated RBs*/
		ulHarqInfo_p->riv      = 0;
		ulHarqInfo_p->ackNack  = 0;    /*indicates ACK or NACK (0 for NACK /1 for ACK) */
		ulHarqInfo_p->currentRetransCount = 0;/*indicates the retrans count for current TTI/HARQ*/

		ulHarqInfo_p->newDataIndicator    = 0;
		ulHarqInfo_p->txIndicator    =  NEW_TX;
		ulHarqInfo_p->deltabsrLoad         = 0; /*UL_BSR_FIX*/
		ulHarqInfo_p->nextReTransmissionType = 0;
		ulHarqInfo_p->strictRBAllocation     = 0;
		ulHarqInfo_p->harqStatus = HARQ_PROCESS_FREE;
		/* TTIB_Code Start */
#ifdef FDD_CONFIG    
		ulHarqInfo_p->isTTIBundled = FALSE;
		ulHarqInfo_p->isSpsEnabled = FALSE;
		ulHarqInfo_p->isTTIDataRcvd = FALSE;
		ulHarqInfo_p->ttiBundleFirstTick = 0;
		ulUEContext_p->ttiHarqMap[n] = TTI_MAP_FREE ; 
		ulUEContext_p->ttiHarqMap[n + MAC_DEFAULT_NUM_HARQ_PROCESSES] = TTI_MAP_FREE ; 
#endif
		/* TTIB_Code End */


		/* + SPR_5711 */
#if defined(KPI_STATS)  || defined(PERF_STATS)
		/* - SPR_5711 */
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT)
		for ( lcIndex = 0;lcIndex < MAX_NUMBER_OF_LOGICAL_CHANNEL; lcIndex++)
		{
			ulHarqInfo_p->netlength[lcIndex] = 0;
		}
		ulHarqInfo_p->tbSizeNet = 0;
#endif
#endif
		/* SPR 14235 Changes Starts */
		for (lcgIndex = 0; lcgIndex < NUM_OF_LCG; ++lcgIndex)
		{
			ulHarqInfo_p->grantPerLcg[lcgIndex] = 0;
		}
		/* SPR 14235 Changes Ends */

#ifdef FDD_CONFIG
#ifdef ULHARQ_TIMER_PROC
		ulHarqInfo_p->ulHarqProcessId = n;
#endif
#endif
		/* CRC_ULSCH_IND SPR 8722 Fix Start */
#ifdef TDD_CONFIG
		ulHarqInfo_p->crcUlschIndFlag = 0;
#endif
		/* CRC_ULSCH_IND SPR 8722 Fix End */
	}
#ifdef TDD_CONFIG
	/* TDD Config 6 Changes Start */
	for (ulHarqCount = 0; ulHarqCount < MAX_UL_HARQ_TIMER; ulHarqCount++)
	{
		for ( n = 0; n < MAX_SUB_FRAME ; n++)
		{
			ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[ulHarqCount][n] =
				INVALID_HARQ_ID;/*INVALID_HARQ*/
		}
	}
	/* TDD Config 6 Changes End */

#elif FDD_CONFIG
#ifdef ULHARQ_TIMER_PROC
	for(n=0;n<MAX_SUB_FRAME;n++)
	{
		ulUEContext_p->ulHarqTimerNodeArrrayIndex[0][n] = INVALID_HARQ_ID;
		ulUEContext_p->ulHarqTimerNodeArrrayIndex[1][n] = INVALID_HARQ_ID;
	}
#endif
#endif

}

/*****************************************************************************
 * Function Name  : fillULUECategoryAndMcsInfo
 * Inputs         : ueIndex - Stores UE index 
 *                  ulUEContext_p - Pointer to UL UE context
 *                  ueContextInfo_p - Pointer to UE related UL Info
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the UE Category and MCS information in 
 *                  UL UE context.
 *****************************************************************************/
STATIC  void fillULUECategoryAndMcsInfo (
		UInt32             ueIndex,
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p,
		InternalCellIndex internalCellIndex)

{
	/*Update the category of the UE*/
	/* SPR 5084 changes starts */
	/* SPR 8416 changes start */
	if(TRUE == ueContextInfo_p->ueCategoryReqType )
		/* SPR 8416 changes end */
	{
		ulUEContext_p->ueCategory = ueContextInfo_p->ueCategory;
	}
	else
	{
		ulUEContext_p->ueCategory = MAC_MIN_UE_CATEGORY;
	}
	/* CA Changes Start */
	/* SPR 12246 Start */
	if (TRUE == ueContextInfo_p->ueCategoryV1020ReqType) 
	{
		ulUEContext_p->ueCategoryV1020 = ueContextInfo_p->ueCategoryV1020;
	}        
	else
	{
		ulUEContext_p->ueCategoryV1020 = ulUEContext_p->ueCategory;
	}
	/* SPR 12246 End */

	/* CA Changes End */
	/* SPR 5084 changes ends */
	/* SPR 3608 changes start */ 
	ulUEContext_p->uePsrCount = 0;
	/* SPR 3608 changes end */ 

	/*Update the max TB size for the category of the UE */
	/* SPR 19600 Changes start */
	ulUEContext_p->maxTBSize = ulUeCategoryInfo_g[ulUEContext_p->ueCategory - 1].maxBytesULSCHTb;
	/* SPR 19600 Changes end */

	ulUEContext_p->uePriority = ueContextInfo_p->uePriority;
	ulUEContext_p->modulationScheme = ueContextInfo_p->modScheme;  
#ifdef TDD_CONFIG  
	ulUEContext_p->tddAckNackFeedbackMode = ueContextInfo_p->tddAckNackFeedbackMode;
#endif
	/* SPR 2063 start */
	ulUEContext_p->mcsIndex = ueContextInfo_p->mcsIndex;
	/* SPR 2063 end */

	ulUEContext_p->mcsPriority = 
		ulQosGetMcsPriority (ulUEContext_p->mcsIndex);

	/* SPR 3271 changes start */
	ulUEContext_p->initialMcs = ueContextInfo_p->mcsIndex;
    {
	/*CA Changes start  */
	ulUEContext_p->pathLoss = (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])-> \
				  alphaBasedPathLossToTargetSINRMap.defaultPathLoss;
	/*CA Changes end  */
	/* SPR 3271 changes end */
	/* ICIC changes start */
	/*CA Changes start  */
	updateULPathLoss(ueIndex,ulUEContext_p->pathLoss, internalCellIndex);
	/*CA Changes end  */
    }
	ulUEContext_p->isAlreadyInCEPriorityList = FALSE;
	/* ICIC changes end */
}

/*****************************************************************************
 * Function Name  : fillULContextAmbrInfo 
 * Inputs         : lchCount - Number of Logical Channels configured for UE
 *                  ulUEContext_p - Pointer to UL UE context
 *                  ueContextInfo_p - Pointer to UE related UL Info
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function fills the AMBR information in UL UE context.
 *****************************************************************************/
STATIC  MacRetType fillULContextAmbrInfo (UInt32        lchCount,
		ULUEContext        *ulUEContext_p,
		UeContextForUplink *ueContextInfo_p
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA changes End */
		)
{
	ULQosStrategyLCNode  *ulQosStrategyLCNode_p = PNULL;
	/* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
	UInt8 index = 0;
#endif
	/* TDD Config 0 Changes End */

	/*CA Changes start  */
	InitParams *initParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
	/*CA Changes end  */

	ulUEContext_p->ulAmbrInfo.isPresentInEligibleList = FALSE;
	/* SPR# 3110 Changes Starts*/
	ulUEContext_p->ulAmbrInfo.ulQosStrategyLCNode_p = NULL;
	/* SPR# 3110 Changes Ends*/
	/*  SPR_3244_Fix  */
	ulUEContext_p->ulAmbrInfo.isLCNodePresent = FALSE;
	/* SPR_3244_Fix */
	/* this should be initilaized irrespective of AMBR info present */
	/*update the stkn(t) with the token rate*/
	ulUEContext_p->ulAmbrInfo.scheduledToken[0] = 0;

	ulUEContext_p->ulAmbrInfo.curTknWindowInd    = 0;
	ulUEContext_p->ulAmbrInfo.totSchdTknInWindow = 0;
	ulUEContext_p->ulAmbrInfo.minQci             = MAX_QCI;
	ulUEContext_p->ulAmbrInfo.minPdb             = 1000;
	ulUEContext_p->ulAmbrInfo.maxPdb             = 0; 
	/* SPR# 3110 Changes Starts*/
	ulUEContext_p->ulAmbrInfo.lcId               = INVALID_LCID;
	ulUEContext_p->ulAmbrInfo.lcgId               = LCGID_INVALID_VALUE;
	ulUEContext_p->ulStrategyTxNode_p            = NULL;
	listInit(&ulUEContext_p->ulEligibleLcList);
	listInit(&ulUEContext_p->ulScheduledLcList);
	/* SPR# 3110 Changes Starts*/
#ifdef UTFWK_SIMULATION 
	/* Initialize DRX and MEAS GAP TTIs for SPS */
	memSet(ulUEContext_p->spsUlInfo.ueDrxTTI, 
			DRX_STATE_INVALID, MAX_SPS_INTERVAL );
	memSet( ulUEContext_p->spsUlInfo.ueMeasGapTTI, 
			MEAS_GAP_OFF, MAX_SPS_INTERVAL );
#endif

    {
	if (TRUE == ueContextInfo_p->ulAmbrPresent)
	{
		/*fill ambr*/
		ulUEContext_p->ulAmbrInfo.AMBR = ueContextInfo_p->ulAmbr;

		/*debug stats*/
		LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_AMBR(
				ulUEContext_p->ueIndex,ueContextInfo_p->ulAmbr);

		/*calculate the ambr token value*/
		ulQosCalcNonGbrLCTknValue(&ulUEContext_p->ulAmbrInfo
				/* CA changes Start */
				, internalCellIndex
				/* CA changes End */
				);

		ulUEContext_p->ulAmbrInfo.availAmbrToken = 
			ulUEContext_p->ulAmbrInfo.ambrTokenRate;

		/*calculate ambr bucket depth*/
		ulUEContext_p->ulAmbrInfo.ambrTokenDepth = 
			/*CA Changes start  */
			initParams_p->ulSchdConfig.ambrBktFactor * ulUEContext_p->ulAmbrInfo.ambrTokenRate;
		/*CA Changes end  */

		/* DESIGN REVIEW CHANGES INCORPORATION */
		/* Calculate bucket limit(minimum tokens to present in bucket) */
		ulUEContext_p->ulAmbrInfo.ambrTokenLimit =
			/*CA Changes start  */
			initParams_p->ulSchdConfig.ambrTokenLimitFactor * 
			/*CA Changes end  */
			ulUEContext_p->ulAmbrInfo.ambrTokenRate;

		/*update ambr token priority table*/
		ulQosUpdateTknTableForLcPriority(ulUEContext_p->ulAmbrInfo.lcTknTable,
				ulUEContext_p->ulAmbrInfo.AMBR, internalCellIndex);

		ulUEContext_p->ulAmbrInfo.ambrTokenPriority  = 5;
		/*create strategy lc node of type ulqosstrategylcnode for ambr*/
		/* SPR# 4287 Changes Starts */
		/*CA Changes start  */
		if(UL_QOS_SCHEDULING == initParams_p->ulSchdConfig.ulSchedulerStrategy)
			/*CA Changes end  */
			/* SPR# 4287 Changes Ends */
		{

			GET_MEM_FROM_POOL(ULQosStrategyLCNode,
					ulQosStrategyLCNode_p,
					sizeof(ULQosStrategyLCNode),
					PNULL);
			/*+COVERITY 5.3.0 - 32730*/
			if(PNULL == ulQosStrategyLCNode_p)
			{
				ltePanic("Memory Allocation failed in %s\n",__func__);
				/*KLOCKWORK_CHANGES_START_165*/
				return MAC_FAILURE;
				/*KLOCKWORK_CHANGES_STOP_165*/
			}
			/*+COVERITY 5.3.0 - 32730*/

			/*FILLING ulQosStrategyLCNode_p PARAMS*/

			ulQosStrategyLCNode_p->lcgId          = 0;
			ulQosStrategyLCNode_p->lchId          = ulUEContext_p->ulAmbrInfo.lcId;
			/* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
			for (index = 0; index < MAX_UL_SUBFRAME_GRANT; index++)
			{
				ulQosStrategyLCNode_p->scheduledBytes[index] = 0;
			}
#else
			ulQosStrategyLCNode_p->scheduledBytes = 0;
#endif
			/* TDD Config 0 Changes End */
			ulQosStrategyLCNode_p->totLCWght      = 0;
			ulQosStrategyLCNode_p->channelType    = NON_GBR;
			ulQosStrategyLCNode_p->ueId           = ulUEContext_p->ueIndex;
			/* SPR 2863 Fix */
			ulQosStrategyLCNode_p->isPresentInEligibleList = FALSE;
			/* SPR 2863 Fix */
			/* SPR_3244_Fix  */
			ulQosStrategyLCNode_p->isLCNodePresent = FALSE;
			/* SPR_3244_Fix */

			/* SPR# 3110 Changes Starts*/
			ulQosStrategyLCNode_p->isLCScheduled = FALSE;
			/* SPR# 3110 Changes Ends*/

			/*STORE UL STRATEGY NODE IN AMBR INFO*/
			ulUEContext_p->ulAmbrInfo.ulQosStrategyLCNode_p = ulQosStrategyLCNode_p;

			/*Storing the back refernce pointer for UL AMBR Info in Strategic LC node*/
			ulQosStrategyLCNode_p->ulAmbrInfo_p = &ulUEContext_p->ulAmbrInfo;
			ulQosStrategyLCNode_p->lcInfo_p     = PNULL;
		}
	}
	}//LTE_CELL
	/* Configure Logical channels if lchCount > 0 */
	ulUEContext_p->numLogicalChannels = 0;
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT)
	ulUEContext_p->maxConfiguredLcId = 0;
	ulUEContext_p->isDemuxIndexPresent = FALSE;
#endif
	if ( lchCount ) 
	{
		if (MAC_SUCCESS != addULUELogicalChannel(ulUEContext_p, lchCount, 
					ueContextInfo_p->lcInfo,internalCellIndex))
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"addULUELogicalChannel Failed UE %u\n",
                    ulUEContext_p->ueIndex);
			return MAC_FAILURE;
		}
	}
	return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : fillULPowerCtrlAndCsiInfo
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 *                  ueContextInfo_p -
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function fills the Power control information in UL 
 *                  UE context.
 *****************************************************************************/
STATIC  MacRetType fillULPowerCtrlAndCsiInfo ( 
		ULUEContext        *ulUEContext_p,
		UeContextForUplink *ueContextInfo_p,
		/*CA Changes start */
		InternalCellIndex internalCellIndex
		/*CA Changes end */
		)
{
	UInt32 n    = 0;
	CSIContext *csiContext_p = PNULL;

	/* CA changes Start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */
	ulUEContext_p->pcMax = ueContextInfo_p->pcMax;

	ulUEContext_p->minUePower =  ueContextInfo_p->minUePower;

	ulUEContext_p->currPowerCorrectionForPusch = 0;
	ulUEContext_p->prevPowerCorrectionForPusch = 0;
	ulUEContext_p->powerControlPuschTPCValue = 0;

	/*SPR 4026 Start*/
	ulUEContext_p->isDLWideBandCQIReportAvailable = 0;
	/*SPR 4026 End*/

	/* SPR 4609_4610 Start */
	ulUEContext_p->tpcInfoForPusch.maxPowerReachedFlag = FALSE;
	ulUEContext_p->tpcInfoForPusch.minPowerReachedFlag = FALSE;
	/* SPR 4609_4610 End */

	csiContext_p = &ulUEContext_p->csiContext;
	for(n = 0; n < MAX_SUB_FRAME; n++)
	{
		ulUEContext_p->powerControlDeltaTFAndLogFactor[n] = 0;
	}
	if (FREQUENCY_SELECTIVE_ON == cellParams_p->freqSelectiveSchUL)
	{
		GET_MEM_FROM_POOL(MCSToPrbMapInfo, \
				csiContext_p->mcsToPrbMap_p,\
				sizeof (MCSToPrbMapInfo)*MAX_MCS_INDEX,\
				PNULL);
	}
	/* SPR 3271 changes start */
	/*CA Changes start  */
	initUEULCSIContext(ulUEContext_p,ulUEContext_p->initialMcs, internalCellIndex);
	/*CA Changes end  */
	/* SPR 3271 changes end */

	/*Power Control Change CLPC_CHG */
	/*Copy the UL TPC parameters into new ULUEContext*/
	ulUEContext_p->dedicatedULPowerControlInfo = ueContextInfo_p->dedicatedULPowerControlInfo;
	/* Store the pcMax */

	if( !ueContextInfo_p->tpcPdcchConfigPusch.release )
	{
		if (MAC_SUCCESS != configureTpcPdcchInfoForPusch(
					ueContextInfo_p->tpcPdcchConfigPusch.tpcRnti,
					ueContextInfo_p->tpcPdcchConfigPusch.tpcBitMapIndex,
					ueContextInfo_p->tpcPdcchConfigPusch.isConfiguredForDci3Or3A,
					ulUEContext_p,internalCellIndex))
		{
			return MAC_FAILURE;
		}
	}
	/* SPR 3271 changes start */
	else
	{
		ulUEContext_p->tpcInfoForPusch.tpcRnti = INVALID_TPC_RNTI;
		ulUEContext_p->tpcInfoForPusch.triggeredUeListNodeInTpcRntiContext_p = PNULL; 
		ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue = TPC_NO_CHANGE; 
		ulUEContext_p->tpcInfoForPusch.lastAppliedTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);

	}    
	/* SPR 3271 changes end */

	/* DCI Format 3A changes start*/
	/* Reseting the fakeTpcValue*/
	ulUEContext_p->tpcInfoForPusch.fakeTpcValue = FAKE_TPC_NO_VALUE;
	/* DCI Format 3A changes start*/
	/* CLPC_CHG  END*/

	/* UL_Power_Mask_Changes Start */
	ulUEContext_p->qciDeltaPowerMaskInfo.numOfQciInfo = 0;
	ulUEContext_p->maxQciDeltaPowerMask = 0;
	/* UL_Power_Mask_Changes End */
	return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : fillULSPSInfo 
 * Inputs         : ueIndex - Stores UE Index
 *                  ulUEContext_p - Pointer to UL UE context
 *                  ueContextInfo_p - Pointer to UE related UL Info
 *                  ueDlConfigInfo_p - Pointer to UE DL Config Info
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function fills the SPS information in UL UE context.
 *****************************************************************************/
STATIC  MacRetType fillULSPSInfo ( 
		UInt32               ueIndex, 
		ULUEContext          *ulUEContext_p,
		UeContextForUplink   *ueContextInfo_p,
		UeContextForDownlink *ueDlConfigInfo_p,
		InternalCellIndex internalCellIndex
		)
{
	/* SPS_CHG */
	LP_SpsUlInfo  spsUlInfo_p  = PNULL;

	/* Initilaize schType to DRA_SCHEDULING by default */
	ulUEContext_p->schType = DRA_SCHEDULING;

	/* Assiging the SPS Config parms to the Ue COntext */
	if (TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
			->initParams_p->spsSysWideParam.enableSps)
	{
		spsUlInfo_p = &ulUEContext_p->spsUlInfo;
		spsUlInfo_p->requestType =  \
					    ueContextInfo_p->spsUplinkInfo.requestType;
		/* Initializing the UL SPS State to Invalid state */
		spsUlInfo_p->spsState = INVALID_STATE;

		if ( spsUlInfo_p->requestType == SPS_SETUP )
        /*SPR 20456  code removed+-*/
		{
			/* Cyclomatic Complexity changes - starts here */
			if (MAC_SUCCESS != fillULSpsSetupInfo (ueIndex, ulUEContext_p, 
						ueContextInfo_p, ueDlConfigInfo_p, spsUlInfo_p,
						internalCellIndex))
			{
				return MAC_FAILURE;
			}
			/* Cyclomatic Complexity changes - ends here */
		}
		else
		{
			spsUlInfo_p->requestType = SPS_RELEASE; 
		}
		LTE_MAC_UPDATE_UPLINK_SPS_UE_EXIST_STATE(ulUEContext_p->ueIndex,
				LTE_TRUE,GET_CURRENT_TICK_FOR_CELL(internalCellIndex));
		LTE_MAC_UPDATE_UPLINK_SPS_UE_STATS(ulUEContext_p->ueIndex, 
				spsUlInfo_p );
	}
	else
	{
		ulUEContext_p->spsUlInfo.spsUlSetupInfo_p   = PNULL;
		ulUEContext_p->spsUlInfo.sdlOccNode_p       = PNULL;
		ulUEContext_p->spsUlInfo.spsState           = INVALID_STATE;
		ulUEContext_p->spsUlInfo.requestType        = SPS_RELEASE;
		ulUEContext_p->spsUlInfo.SpsCrnti           = INVALID_SPS_CRNTI;
		ulUEContext_p->spsUlInfo.hpId[0].harqId = INVALID_HARQ_ID; 
		ulUEContext_p->spsUlInfo.hpId[1].harqId = INVALID_HARQ_ID;
	}
	/* SPS_CHG */
	return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : fillULSpsSetupInfo 
 * Inputs         : ueIndex - Stores UE Index
 *                  ulUEContext_p - Pointer to UL UE context
 *                  ueContextInfo_p - Pointer to UE related UL Info
 *                  ueDlConfigInfo_p - Pointer to UE DL Config Info
 *                  spsUlInfo_p- Pointer to SPS UL info
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function fills the SPS information in UL UE context.
 *****************************************************************************/
STATIC  MacRetType fillULSpsSetupInfo ( 
		UInt32               ueIndex, 
		ULUEContext          *ulUEContext_p,
		UeContextForUplink   *ueContextInfo_p,
		UeContextForDownlink *ueDlConfigInfo_p,
		LP_SpsUlInfo         spsUlInfo_p,
		InternalCellIndex internalCellIndex
		)
{
	RNTIInfo *rntiInfo_p        = PNULL;

	/* CA changes Start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */
                    /* SPR 20456 start */
                    /* code removed */
                    /*SPR 20456 end */
		/*!
		 * \code
		 * IF Sps Req type is setup
		 *    - Save the sps crnti in Ue context
		 *    -Populating the SPS Info in UE Context recived from L3 
		 * \endcode
		 * */
		if(PNULL != dlUECtxInfoArr_g[ueIndex].dlUEContext_p &&
				SPS_SETUP == dlUECtxInfoArr_g[ueIndex].dlUEContext_p->spsDlInfo.requestType)
		{
			spsUlInfo_p->SpsCrnti = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->
				spsDlInfo.SpsCrnti;
			/*CA Changes start  */
			rntiInfo_p = getUeIdxFromRNTIMap(spsUlInfo_p->SpsCrnti, internalCellIndex);
			/*CA Changes end  */
			if (PNULL == rntiInfo_p)
			{
				LOG_MAC_MSG( MAC_UE_ID_RNTI_MAP, LOGWARNING, MAC_RRC_INF,
						GET_CURRENT_TICK_FOR_CELL(internalCellIndex), __LINE__, 
						spsUlInfo_p->SpsCrnti, 
						ulUEContext_p->crnti,
						ulUEContext_p->ueIndex, internalCellIndex, 0,0, __func__, "" );
				return MAC_FAILURE;
			}
			rntiInfo_p->rntiFlag = SPS_RNTI;
			rntiInfo_p->index    = ulUEContext_p->ueIndex;
			ulUEContext_p->spsUeSearchSpaceInfo_p = (&rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex][0]);
		}
		else
		{
			UInt16  spsCrnti = allocateSpsCrnti(internalCellIndex);
			if( MAC_FAILURE == spsCrnti )
			{
				return MAC_FAILURE;
			}
			else
			{
				spsUlInfo_p->SpsCrnti = spsCrnti;
				/* Assigning SPS Crnti in DL Context */
				if(PNULL != dlUECtxInfoArr_g[ueIndex].dlUEContext_p)
				{
					dlUECtxInfoArr_g[ueIndex].dlUEContext_p->spsDlInfo.SpsCrnti = spsCrnti;
				}

				/*CA Changes start  */
				rntiInfo_p = getUeIdxFromRNTIMap(spsUlInfo_p->SpsCrnti, internalCellIndex);
				/*CA Changes end  */
				if (PNULL == rntiInfo_p)
				{
					LOG_MAC_MSG( MAC_UE_ID_RNTI_MAP, LOGWARNING, MAC_RRC_INF,
							GET_CURRENT_TICK_FOR_CELL(internalCellIndex), __LINE__, spsCrnti, 
							ulUEContext_p->crnti,
							ulUEContext_p->ueIndex, internalCellIndex, 0,0, __func__, "" );
					return MAC_FAILURE;
				}
				rntiInfo_p->rntiFlag = SPS_RNTI;
				rntiInfo_p->index    = ulUEContext_p->ueIndex;
				ulUEContext_p->spsUeSearchSpaceInfo_p = (&rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex][0]);
			}
		}

		spsUlInfo_p->spsUlSetupInfo_p = PNULL;
		GET_MEM_FROM_POOL(spsUlSetupInfo,
				spsUlInfo_p->spsUlSetupInfo_p,
				sizeof (spsUlSetupInfo),PNULL);
		if (spsUlInfo_p->spsUlSetupInfo_p == PNULL)
		{
			return MAC_FAILURE;
		}
		else
		{
			spsUlInfo_p->spsUlSetupInfo_p->\
				semiPersistentSchedIntervalUL = \
				ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
				semiPersistentSchedIntervalUL;
			spsUlInfo_p->spsUlSetupInfo_p->\
				implicitReleaseAfter =\
				ulImplicitReleaseAfterMap[ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
				implicitReleaseAfter];
			if(TRUE == ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isP0TagPresent)
			{
				spsUlInfo_p->spsUlSetupInfo_p->isP0TagPresent = TRUE;
				spsUlInfo_p->spsUlSetupInfo_p->\
					p0PersistentInfo.p0NominalPuschPersistent = \
					ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
					p0PersistentInfo.p0NominalPuschPersistent;
				spsUlInfo_p->spsUlSetupInfo_p->\
					p0PersistentInfo.p0UePuschPersistent = \
					ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
					p0PersistentInfo.p0UePuschPersistent;
			}
			else
			{
				/*Assigning the Normal values */
				spsUlInfo_p->spsUlSetupInfo_p->\
					p0PersistentInfo.p0NominalPuschPersistent = \
					cellParams_p->commonULPowerControlInfo.p0NominalPUSCH;
				spsUlInfo_p->spsUlSetupInfo_p->\
					p0PersistentInfo.p0UePuschPersistent = \
					ueContextInfo_p->dedicatedULPowerControlInfo.p0UePusch;
			}
			if(TRUE == ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isTwoIntervalConfigPresent)
			{
				spsUlInfo_p->spsUlSetupInfo_p->isTwoIntervalConfigPresent = TRUE;
				spsUlInfo_p->spsUlSetupInfo_p->\
					twoIntervalsConfig = \
					ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
					twoIntervalsConfig;
			}
			/* + SPS_TDD_Changes */
			else
			{
				spsUlInfo_p->spsUlSetupInfo_p->isTwoIntervalConfigPresent = FALSE;
				spsUlInfo_p->spsUlSetupInfo_p->twoIntervalsConfig = 0;
			}
			/* - SPS_TDD_Changes */

		}
	return MAC_SUCCESS;
}
/*SPR 16855 +-*/
/*****************************************************************************
 * Function Name  : fillULPucchConfigInfo 
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 *                  ueContextInfo_p - Pointer to UE related UL Info
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the PUCCH config Information in 
 *                  UL UE context.
 *****************************************************************************/
STATIC  void fillULPucchConfigInfo (
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p,
		InternalCellIndex internalCellIndex)
{
	PucchConfig *pucchConfig_p = PNULL;
#ifdef TDD_CONFIG    
	UInt32 n    = 0;
	UInt32 iter = 0;
#endif

#ifdef FDD_CONFIG
	/* PUCCH TDD ADAPTATION CHANGES Start */        
    /*SPR 16855 +-*/
	UInt32 passiveIdx = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
	UInt32 activeIdx = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex;
    /*SPR 16855 +-*/
	/* PUCCH TDD ADAPTATION CHANGES End */        
#endif


	pucchConfig_p = &ulUEContext_p->pucchConfigInfo;
	pucchConfig_p->srPeriodicity  = 0;
	pucchConfig_p->cqiPeriodicity = 0;
	pucchConfig_p->riPeriodicity  = 0;
	pucchConfig_p->srPUCCHRBIndex = 0;
	pucchConfig_p->cqiPUCCHRBIndex= 0;
	/* CA changes start */
	pucchConfig_p->trigger1r10 = FALSE;
	pucchConfig_p->trigger2r10 = FALSE;
	/* CA changes End */

	pucchConfig_p->simultaneousAckNackAndCQI =
		ueContextInfo_p->simultaneousACKNACKAndCQI;
#ifdef FDD_CONFIG
	pucchConfig_p->srOffset       = 0;
	pucchConfig_p->cqiOffset      = 0;
	pucchConfig_p->riOffset       = 0;
    /*SPR 16855 +-*/
	/* Review comment fix start PUCCH_ENH_2 */
	pucchConfig_p->prbIndexSlot1[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex][SCHEDULING_REQUEST]  = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot2[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex][SCHEDULING_REQUEST]  = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot1[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex][RI_REPORT]  = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot2[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex][RI_REPORT]  = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot1[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex][CQI_REPORT] = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot2[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex][CQI_REPORT] = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot1[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex][WIDEBAND_CQI_REPORT] = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot2[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex][WIDEBAND_CQI_REPORT] = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot1[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex][SCHEDULING_REQUEST]  = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot2[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex][SCHEDULING_REQUEST]  = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot1[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex][RI_REPORT]  = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot2[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex][RI_REPORT]  = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot1[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex][CQI_REPORT] = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot2[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex][CQI_REPORT] = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot1[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex][WIDEBAND_CQI_REPORT] = INVALID_PRB_IDX;
	pucchConfig_p->prbIndexSlot2[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex][WIDEBAND_CQI_REPORT] = INVALID_PRB_IDX;
    /*SPR 16855 +-*/
	/* PUCCH TDD ADAPTATION CHANGES Start */        
    /*SPR 16855 +-*/
	/*CA Changes start  */
	/*CA Changes end  */
	/* Initialize passive contexts */
	/* TDD SB_CQI */
	pucchConfig_p->pucchNodesForCqi[passiveIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForCqi[passiveIdx].count = 0;
	pucchConfig_p->pucchNodesForWbCqi[passiveIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForWbCqi[passiveIdx].count = 0;
	pucchConfig_p->pucchNodesForRI[passiveIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForRI[passiveIdx].count = 0;
	pucchConfig_p->pucchNodesForSR[passiveIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForSR[passiveIdx].count = 0;
	pucchConfig_p->pucchNodesForCqi2[passiveIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForCqi2[passiveIdx].count = 0;
	pucchConfig_p->pucchNodesForWbCqi2[passiveIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForWbCqi2[passiveIdx].count = 0;
	pucchConfig_p->pucchNodesForRI2[passiveIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForRI2[passiveIdx].count = 0;

	/* TDD SB_CQI */

	pucchConfig_p->maxCQIWrapAround[passiveIdx] = 0;
	pucchConfig_p->maxWbCQIWrapAround[passiveIdx] = 0;
	pucchConfig_p->maxSrWrapAround[passiveIdx]  = 0; 
	pucchConfig_p->maxRIWrapAround[passiveIdx]  = 0;
	pucchConfig_p->maxCQIWrapAround2[passiveIdx] = 0;
	pucchConfig_p->maxWbCQIWrapAround2[passiveIdx] = 0;
	pucchConfig_p->maxRIWrapAround2[passiveIdx]  = 0;

	/*CA Changes start  */
	pucchConfig_p->pucchShortSRNodeHead_p[shortPeriodicSRMapShortInfo_gp[internalCellIndex]->passiveIndex] = PNULL;
	/*CA Changes end  */

	/* TDD SB_CQI */
	/* Initialize active contexts */
	pucchConfig_p->pucchNodesForCqi[activeIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForCqi[activeIdx].count = 0;
	pucchConfig_p->pucchNodesForWbCqi[activeIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForWbCqi[activeIdx].count = 0;
	pucchConfig_p->pucchNodesForRI[activeIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForRI[activeIdx].count = 0;
	pucchConfig_p->pucchNodesForSR[activeIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForSR[activeIdx].count = 0;
	pucchConfig_p->pucchNodesForCqi2[activeIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForCqi2[activeIdx].count = 0;
	pucchConfig_p->pucchNodesForWbCqi2[activeIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForWbCqi2[activeIdx].count = 0;
	pucchConfig_p->pucchNodesForRI2[activeIdx].pucchNodePtr = PNULL;
	pucchConfig_p->pucchNodesForRI2[activeIdx].count = 0;

	/* TDD SB_CQI */

	pucchConfig_p->maxCQIWrapAround[activeIdx] = 0;
	pucchConfig_p->maxWbCQIWrapAround[activeIdx] = 0;
	pucchConfig_p->maxSrWrapAround[activeIdx]  = 0; 
	pucchConfig_p->maxRIWrapAround[activeIdx]  = 0;
	pucchConfig_p->maxCQIWrapAround2[activeIdx] = 0;
	pucchConfig_p->maxWbCQIWrapAround2[activeIdx] = 0;
	pucchConfig_p->maxRIWrapAround2[activeIdx]  = 0;

	/*CA Changes start  */
	pucchConfig_p->pucchShortSRNodeHead_p[shortPeriodicSRMapShortInfo_gp[internalCellIndex]->activeIndex] = PNULL;
	/*CA Changes end  */
    /*SPR 16855 +-*/
	/* PUCCH TDD ADAPTATION CHANGES End */        
#endif

	/*SPR 7800 Changes Start */
	SRSConfigInfo *srsConfigInfo_p = &ulUEContext_p->srsConfigInfo;
#ifdef TDD_CONFIG
	srsConfigInfo_p->srsULBandwidthCollision = 0;
#endif    
	srsConfigInfo_p->srsConfigType = 0;
	srsConfigInfo_p->srsPeriodicity = 0;
	srsConfigInfo_p->srsSubFrameOffset = 0;
	srsConfigInfo_p->srsDedicatedSetupInfo.srsBandwidth = 0;
	srsConfigInfo_p->srsDedicatedSetupInfo.freqDomainPos = 0;
	srsConfigInfo_p->srsDedicatedSetupInfo.srsHoppingBandwidth = 0;
	srsConfigInfo_p->srsDedicatedSetupInfo.transmissionComb = 0;
	srsConfigInfo_p->srsDedicatedSetupInfo.duration = 0;
	srsConfigInfo_p->srsDedicatedSetupInfo.cyclicShift = 0;
	srsConfigInfo_p->srsDedicatedSetupInfo.srsConfigIndex = 0;
    /*SPR 16855 +-*/
	/*CA Changes start  */
	srsConfigInfo_p->srsReportHeadNode_p[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex] = PNULL;
	srsConfigInfo_p->srsReportHeadNode_p[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex] = PNULL;
	/*CA Changes end  */
    /*SPR 16855 +-*/
	/*SPR 7800 Changes End */
#ifdef TDD_CONFIG   
	for (n = 0; n < 
			MAX_UE_SCHEDULED_INFO_INSTANCES; n++)
	{
		/** SPR 14204 Fix : Start **/
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].flag = UNSET_FLAG;
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].ulCCEIndex = INVALID_CCE_INDEX;
		for (iter = 0; iter < MAX_SUB_FRAME; iter++)
		{
			ulUEContext_p->ueScheduledInfo[internalCellIndex][n].dlCceIndex[iter] = INVALID_CCE_INDEX;
		}
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].transMode = 0;
		/* SPR 7179 Fix Start */
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].ueInsertedInPdcchDlUeInfo = 0;
		/* SPR 7179 Fix End */
		/** SPR 14204 Fix : End **/
	}
	pucchConfig_p->subbandCQI_k = ueContextInfo_p->subbandCQI_k;
#endif
}

#ifdef TDD_CONFIG   
/*****************************************************************************
 * Function Name  : fillULUeScheduledInfo 
 * Inputs         : ulUEContext_p - Pointer to UL UE context,
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the UE Scheduled Information in 
 *                  UL UE context.
 *****************************************************************************/
STATIC  void fillULUeScheduledInfo ( ULUEContext  *ulUEContext_p,
		InternalCellIndex internalCellIndex)
{
	/* SPR 609 changes start */
	UeScheduledInfo *ueScheduledInfo_p = PNULL;

	/** SPR 14204 Fix : Start **/
	ueScheduledInfo_p = &(ulUEContext_p->ueScheduledInfo[internalCellIndex][0]);
	/** SPR 14204 Fix : End **/
	/* SPR 609 changes end */
	/* CA Changes start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */
	switch (cellParams_p->subFrameAssign)
	{
		case UL_DL_CONFIG_0:
			{ 
				/*      UL/DL configuration 0 
				 *      Subframe number
				 *      0   1   2   3   4   5   6   7   8   9
				 *      D   S   U   U   U   D   S   U   U   U
				 *      */

				/** SPR 14204 Fix : Start **/
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_0] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_1] = &ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_5] = &ueScheduledInfo_p[UL_CONTAINER_3];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_6] = &ueScheduledInfo_p[UL_CONTAINER_4];

				/* TDD Config 0 Changes Start */ 
				/* ulUeScheduledInfoPointerArray is made U based*/
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_2] = &ueScheduledInfo_p[UL_CONTAINER_4];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_3] = &ueScheduledInfo_p[UL_CONTAINER_5];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_4] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_7] = &ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_8] = &ueScheduledInfo_p[UL_CONTAINER_2];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_9] = &ueScheduledInfo_p[UL_CONTAINER_3];
				/** SPR 14204 Fix : End **/
				/* TDD Config 0 Changes End */
			}
			break;

		case UL_DL_CONFIG_1:
			{
				/* UL/DL configuration 1 DCI-0 (1,4,6,9)*/
				/* 
				   Subframe number
				   0   1   2   3   4   5   6   7   8   9
				   D   S   U   U   D   D   S   U   U   D
				 */

				/** SPR 14204 Fix : Start **/
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_0] = 
					&ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_1] =
					&ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_4] =
					&ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_5] = 
					&ueScheduledInfo_p[UL_CONTAINER_2];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_6] = 
					&ueScheduledInfo_p[UL_CONTAINER_2];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_9] = 
					&ueScheduledInfo_p[UL_CONTAINER_3];
				/* TDD Config 0 Changes Start */ 
				/* ulUeScheduledInfoPointerArray is made U based*/
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_7] = 
					&ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_8] = 
					&ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_2] = 
					&ueScheduledInfo_p[UL_CONTAINER_2];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_3] = 
					&ueScheduledInfo_p[UL_CONTAINER_3];
				/** SPR 14204 Fix : End **/
				/* TDD Config 0 Changes End */
			}
			break;

		case UL_DL_CONFIG_2:
			{
				/*  UL/DL configuration 2 DCI-0 (3,8)*/
				/*
				   Subframe number
				   0   1   2   3   4   5   6   7   8   9
				   D   S   U   D   D   D   S   U   D   D
				 */

				/** SPR 14204 Fix : Start **/
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_0]
					= &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_1]
					= &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_3] 
					= &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_9] = 
					&ueScheduledInfo_p[UL_CONTAINER_0];

				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_4] =
					&ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_5] = &ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_6] = &ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_8] = &ueScheduledInfo_p[UL_CONTAINER_1];

				/* TDD Config 0 Changes Start */ 
				/* ulUeScheduledInfoPointerArray is made U based*/
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_7] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_2] = &ueScheduledInfo_p[UL_CONTAINER_1];
				/** SPR 14204 Fix : End **/
				/* TDD Config 0 Changes End */
			}     
			break;

		case UL_DL_CONFIG_3:
			{       
				/*  UL/DL configuration 3 (DCI-0, 0,8,9) 
				    Subframe number
				    0   1   2   3   4   5   6   7   8   9
				    D   S   U   U   U   D   D   D   D   D
				 */
				/** SPR 14204 Fix : Start **/
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_1] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_5] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_6] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_7] = &ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_8] = &ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_0] = &ueScheduledInfo_p[UL_CONTAINER_2];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_9] = &ueScheduledInfo_p[UL_CONTAINER_2];

				/* TDD Config 0 Changes Start */ 
				/* ulUeScheduledInfoPointerArray is made U based*/
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_2] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_3] = &ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_4] = &ueScheduledInfo_p[UL_CONTAINER_2];
				/** SPR 14204 Fix : End **/
				/* TDD Config 0 Changes End */
			} 
			break;

		case UL_DL_CONFIG_4:
			{
				/*      UL/DL configuration 4 (DCI-0-> 8,9) 
					Subframe number
					0   1   2   3   4   5   6   7   8   9
					D   S   U   U   D   D   D   D   D   D
				 */
				/** SPR 14204 Fix : Start **/
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_0] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_1] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_4] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_5] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_6] = &ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_7] = &ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_8] = &ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_9] = &ueScheduledInfo_p[UL_CONTAINER_1];

				/* TDD Config 0 Changes Start */ 
				/* ulUeScheduledInfoPointerArray is made U based*/
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_2] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_3] = &ueScheduledInfo_p[UL_CONTAINER_1];
				/** SPR 14204 Fix : End **/
				/* TDD Config 0 Changes End */
			}     
			break;

		case UL_DL_CONFIG_5:
			{
				/*         UL/DL configuration 5  (DCI-0 ->8) 
					   Subframe number
					   0   1   2   3   4   5   6   7   8   9
					   D   S   U   D   D   D   D   D   D   D
				 */

				/** SPR 14204 Fix : Start **/
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_0] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_1] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_3] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_4] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_5] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_6] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_7] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_8] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_9] = &ueScheduledInfo_p[UL_CONTAINER_0];

				/* TDD Config 0 Changes Start */ 
				/* ulUeScheduledInfoPointerArray is made U based*/
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_2] = &ueScheduledInfo_p[UL_CONTAINER_0];
				/** SPR 14204 Fix : End **/
				/* TDD Config 0 Changes End */
			}    
			break;

		case UL_DL_CONFIG_6:
			{
				/*         UL/DL configuration 6 (DCI-0 ->0,1,5,6,9) 

					   Subframe number
					   0  1   2   3   4   5   6   7    8   9
					   D  S   U   U   U   D   S   U    U   D
				 */
				/** SPR 14204 Fix : Start **/
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_0] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_1] = &ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_5] = &ueScheduledInfo_p[UL_CONTAINER_2];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_6] = &ueScheduledInfo_p[UL_CONTAINER_3];
				ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_9] = &ueScheduledInfo_p[UL_CONTAINER_4];

				/* TDD Config 0 Changes Start */ 
				/* ulUeScheduledInfoPointerArray is made U based*/
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_7] = &ueScheduledInfo_p[UL_CONTAINER_0];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_8] = &ueScheduledInfo_p[UL_CONTAINER_1];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_2] = &ueScheduledInfo_p[UL_CONTAINER_2];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_3] = &ueScheduledInfo_p[UL_CONTAINER_3];
				ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][SUB_FRAME_4] = &ueScheduledInfo_p[UL_CONTAINER_4];
				/** SPR 14204 Fix : End **/
				/* TDD Config 0 Changes End */
			} 
			break;

		default:
			ltePanic("Invalid Configuration Recieved\n");
			break;
	}
}
#endif

/*****************************************************************************
 * Function Name  : fillULPuschAndCqiInfo 
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 *                  ueContextInfo_p - Pointer to UE related UL Info
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the PUSCH config and CQI Information
 *                  in UL UE context.
 *****************************************************************************/
STATIC  void fillULPuschAndCqiInfo (
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA Changes end */
		)
{
	PuschConfig *puschConfig_p = PNULL;

	/* + CQI_5.0 */
	ulUEContext_p->periodicCQIDroppedCounter = 0;
	ulUEContext_p->lastTriggeredAperiodicTTI = 0;
	ulUEContext_p->aperiodicCQIToBeRequested = FALSE;
	ulUEContext_p->aperiodicCqiRequestTTI = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
	/* - CQI_5.0 */
	ulUEContext_p->cqiInd      = 0; 

	puschConfig_p = &ulUEContext_p->puschConfigInfo;
	if ( ueContextInfo_p->puschConfigInfoFlag )
	{
		puschConfig_p->betaOffsetAckIndex = 
			ueContextInfo_p->betaOffsetAckIndex;
		puschConfig_p->betaOffsetRiIndex = 
			ueContextInfo_p->betaOffsetRiIndex;
		puschConfig_p->betaOffsetCQIIndex = 
			ueContextInfo_p->betaOffsetCQIIndex;
	}
	else
	{
		/* set default value */
		puschConfig_p->betaOffsetAckIndex = MAC_DEFAULT_BETA_OFFSET_ACK_INDEX;  
		puschConfig_p->betaOffsetRiIndex = MAC_DEFAULT_BETA_OFFSET_RI_INDEX;
		puschConfig_p->betaOffsetCQIIndex = MAC_DEFAULT_BETA_OFFSET_CQI_INDEX;
	}
	/* SPR_4682_FIX start */
	ulUEContext_p->isDLWideBandCQIReportAvailable = FALSE;
	ulUEContext_p->dlWidebandCQICodeWordOne = 2;
	ulUEContext_p->dlWidebandCQICodeWordTwo = 2;
	/* SPR_4682_FIX end */
}

/*****************************************************************************
 * Function Name  : fillULUserLocTypeAndPhrMimoInfo
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 *                  ueContextInfo_p - Pointer to UE related UL Info
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the User location type, PHR and 
 *                  MUMIMO information in UL UE context.
 *****************************************************************************/
STATIC  void fillULUserLocTypeAndPhrMimoInfo (
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
	/* ICIC changes start */
	/* Initialize user location type to CC_USER if userLocationType
	 * is CE type but cellEdgeRegionCount is zero*/
	if((ueContextInfo_p->userLocationType == CE_USER)&&
			/*CA Changes start  */
			(ulResourcePartitionConfig_gp[internalCellIndex]->cellEdgeRegionCount == 0))
		/*CA Changes end  */
	{
		lteWarning("No cell edge region exist");
		ulUEContext_p->userLocationType = CC_USER;
	}
	else
	{
		ulUEContext_p->userLocationType = 
			ueContextInfo_p->userLocationType;
	}

	/* ICIC changes end */
	/* +DYNAMIC_ICIC */
	ulUEContext_p->phrGrant = ueContextInfo_p->phrGrant;
	ulUEContext_p->phrProhibitTimerExpiryTick = 0;
	ulUEContext_p->phrTimerCfg.periodicPHRTimer = 0;
	ulUEContext_p->phrTimerCfg.prohibitPHRTimer = ueContextInfo_p->phrTimerCfg.prohibitPHRTimer;
	/* -DYNAMIC_ICIC*/

	/*UL_MU_MIMO_CHG_START*/
	ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus = ueContextInfo_p->ulMUMIMOStatus;
	ulUEContext_p->ulMUMIMOInfo.isPuschAllocated = FALSE;
	/*UL_MU_MIMO_CHG_END*/
}
/* + E_CID_5.3 */
/*****************************************************************************
 * Function Name  : resetULEcidReportInfo
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the User location type, PHR and 
 *                  MUMIMO information in UL UE context.
 *****************************************************************************/
STATIC  void resetULEcidReportInfo (
		ULUEContext        *ulUEContext_p) 
{
	ulUEContext_p->eCidReportParams.typeofReportsRequired = NO_REPORT_TYPE_REQUIRED;
	ulUEContext_p->eCidReportParams.reportStatus =  INVALID_STATUS_ECID_REPORTS;
	ulUEContext_p->eCidReportParams.type1TAValue = INVALID_TIMING_ADVANCE_R9_VALUE; 
	ulUEContext_p->eCidReportParams.type2TAValue = INVALID_TIMING_ADVANCE_R9_VALUE;
	UInt8 count;
	for(count = 0; count < MAX_NUM_ANTENNA; count++)
	{
		ulUEContext_p->eCidReportParams.angleOfArrival[count] = INVALID_ANGLE_OF_ARRIVAL_VALUE;
	}
	ulUEContext_p->eCidReportParams.dummyDCI0Entry_p = PNULL;
}
/* - E_CID_5.3 */

/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : putEntryInPendingDeleteUEQueue
 * Inputs         : ueIndex - index of ueContext 
 *                  transactionId -identifier received in request from RRC
 *                  tti - tick when this func. called.
 *                  rntiFlag - type of RNTI
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None.
 * Returns        : None
 * Description    : This API is used to delete a UE Context from the MAC 
 *                  system. This API entry the node into the  pending
 *                  Delete Queue and processPendingUE  function
 *                  delete the UE Context.
 ****************************************************************************/
void putEntryInPendingDeleteUEQueue(UInt32 ueIndex, 
		UInt32 transactionId, 
		/* SPR 15909 fix start */
        tickType_t tti,
        /* SPR 15909 fix end */
		/*+COVERITY 5.3.0 - 52874*/
		RNTI rntiFlag
		/*-COVERITY 5.3.0 - 52874*/
		,InternalCellIndex internalCellIndex
		)
{
	PendingDeleteQueueNode *pendingDeleteQueueNode_p = PNULL;
	/*Allocate memory for queueNode_p*/
	/*Populate the parameters in queueNode_p as received */
	GET_MEM_FROM_POOL(PendingDeleteQueueNode,pendingDeleteQueueNode_p,sizeof(PendingDeleteQueueNode),PNULL);

	if (CRNTI == rntiFlag)
	{
		dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag = TRUE;
		ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag = TRUE;
	}

	/* Rel 5.3: Coverity Fix Start */
	if (PNULL != pendingDeleteQueueNode_p)
	{
		pendingDeleteQueueNode_p->ueIndex              = ueIndex;
		pendingDeleteQueueNode_p->transactionId        = transactionId;
		pendingDeleteQueueNode_p->tti = tti + UE_CONTEXT_DEL_DELAY;
		pendingDeleteQueueNode_p->rntiFlag = rntiFlag;
		/*Push Node into the Queue*/
		/*CA Changes start  */
		/* + Coverity 25437 */
		if (pushNode(pendingUEDeleteQueue_gp[internalCellIndex],
					&pendingDeleteQueueNode_p->nodeAnchor))
		{
			freeMemPool(pendingDeleteQueueNode_p);
		}
		/* - Coverity 25437 */
		/*CA Changes end  */
	}
	else
	{
		/* Not able to allocate Memory */
	}
	/* Rel 5.3: Coverity Fix End */
}

/****************************************************************************
 * Function Name  : putEntryInMacResetQueue
 * Inputs         : ueIndex - index of ueContext 
 *                  transactionId -
 *                  tti - tick when this func. called.
 * Outputs        : None.
 * Returns        : void 
 * Description    : This API is used to reset a UE Context from the MAC 
 *                  system. This API entry the node into the  pending
 *                  reset Queue.
 ****************************************************************************/

/* SPR 15909 fix start */
void putEntryInMacResetQueue(UInt32 ueIndex, UInt32 transactionId, tickType_t tti)
/* SPR 15909 fix end */
{
	MacResetQueueNode *MacResetQueueNode_p = PNULL;
	/*Allocate memory for queueNode_p*/
	GET_MEM_FROM_POOL(MacResetQueueNode, MacResetQueueNode_p,\
			sizeof(MacResetQueueNode),PNULL);
	/*Populate the parameters in queueNode_p as received */
	/*Set Pending Delete Flag to TRUE so that this UE will not be operated 
	 * upon by other execution legs as it needs to be reset. After resetting
	 * process is complete, pendingDeleteFlag will be again reset to FALSE*/
	dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag = TRUE;
	ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag = TRUE;

	/* CA changes Start */
	InternalCellIndex internalCellIndex;
	internalCellIndex = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->internalCellIndex;
	/* CA Changes end */

	/* Rel 5.3: Coverity Fix Start */
	if (PNULL != MacResetQueueNode_p)
	{
		MacResetQueueNode_p->ueIndex              = ueIndex;
		MacResetQueueNode_p->transactionId        = transactionId;
		MacResetQueueNode_p->tti = tti + UE_CONTEXT_DEL_DELAY;

		LOG_MAC_MSG(MAC_RESET_NODE_PARAM_VAL,LOGINFO,MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),MacResetQueueNode_p->ueIndex,
				MacResetQueueNode_p->transactionId,MacResetQueueNode_p->tti,
				internalCellIndex,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,"MAC_RESET_NODE_PARAM_VAL");

		/*Push Node into the Queue*/
		/*CA Changes start  */
		/* + Coverity 25436 */
		if (pushNode(macResetQueue_gp[internalCellIndex], 
					&MacResetQueueNode_p->nodeAnchor))
		{
			freeMemPool(MacResetQueueNode_p);
		}
		/* - Coverity 25436 */
		/*CA Changes end  */
	}
	else
	{
		/* Not able to allocate Memory */
	}
	/* Rel 5.3: Coverity Fix End */
}


/****************************************************************************
 * Function Name  : processPendingResetUE
 * Inputs         : currentTTI 
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None.
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API is used to reset a UE Context from the MAC 
 *                  system. 
 ****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
MacRetType processPendingResetUE(tickType_t currentTTI, InternalCellIndex internalCellIndex)
/* SPR 15909 fix end */
	/*CA Changes end  */
{
	/*CA Changes start  */
	UInt8 index = 0;
	/*CA Changes end  */
	UInt32 ueIndex       = 0;
	UInt32 transactionId = 0;
	UInt32 resetUeQueueCount   = 0;
	ULUEContext *ueULContext_p    = PNULL;
	DLUEContext *ueDLContext_p    = PNULL;

	MacResetQueueNode *resetUENode_p = PNULL;
	/*CA Changes start  */
	resetUeQueueCount = sQueueCount(macResetQueue_gp[internalCellIndex]);
	/*CA Changes end  */
	while (resetUeQueueCount)
	{
		/*Only Reading the first Node*/
		/*CA Changes start  */
		resetUENode_p = (MacResetQueueNode *) getFirstNode(macResetQueue_gp[internalCellIndex]);
		/*CA Changes end  */
		if ( resetUENode_p != PNULL )
		{
			if (resetUENode_p->tti <= currentTTI)
			{
				ueIndex = resetUENode_p->ueIndex;
				transactionId = resetUENode_p->transactionId;
				LOG_MAC_MSG(PROCESS_PENDING_RESET_UE_PARAMS,LOGINFO,MAC_RRC_INF,
						GET_CURRENT_TICK_FOR_CELL(internalCellIndex),ueIndex,transactionId, 
						resetUeQueueCount, 
						internalCellIndex, DEFAULT_INT_VALUE, 
						DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME, "PROCESS_PENDING_RESET_UE_VAR");

				ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

				if (PNULL != ueDLContext_p)
				{
					/*Reset DL HARQ Entity for a UE*/
					/* SPR 2446 Fix Begins*/
					resetMACDLHarqEntity(&ueDLContext_p->dlHarqContext_p);
					/* SPR 2446 Fix Ends*/
					/* CA changes start */
					for (index = MAX_NUM_PCELL; index <= MAX_NUM_SCELL; index ++)
					{
						if (PNULL != ueDLContext_p->dlUeScellContext_p[index])
						{
							resetMACDLHarqEntity(&ueDLContext_p->dlUeScellContext_p\
									[index]->ScelldlHarqContext_p);
						}
					}
					/* CA changes end */
					/*Reset DL Context for a UE*/
					/* Review comment fix start MAC_RESET_9 */
					/*CA Changes start  */
					if (MAC_SUCCESS != resetMACDLUEEntity(ueDLContext_p, internalCellIndex))
						/*CA Changes end  */
					{
						LOG_MAC_MSG(MAC_DL_RESET_FAILURE_ID,
								LOGERROR,
								MAC_RRC_INF,
								GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
								ueIndex,
								LINE_NUMBER,
								internalCellIndex,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE,
								FUNCTION_NAME,
								"RESET_MAC_DL_UE_ENTITY_FAILURE");

					}
					/* Review comment fix end MAC_RESET_9 */
				}
				else
				{
					LOG_MAC_MSG(MAC_RESET_DLUECONTEXT_NOT_FOUND_ID,
							LOGERROR,
							MAC_RRC_INF,
							GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
							ueIndex,
							LINE_NUMBER,
							internalCellIndex,
							DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE,
							DEFAULT_FLOAT_VALUE,
							DEFAULT_FLOAT_VALUE,
							FUNCTION_NAME,
							"MAC_DLUECONTEXT_NOT_FOUND");
				}

				ueULContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
				/* Reset the nodes for PUCCH (SR,CQI,RI)*/
				if (PNULL != ueULContext_p)
				{
					/*CA Changes start  */
					resetMACULUEEntity(ueULContext_p, internalCellIndex);
					/*CA Changes end  */
				}
				else
				{
					LOG_MAC_MSG(MAC_RESET_ULUECONTEXT_NOT_FOUND_ID,
							LOGERROR,
							MAC_RRC_INF,
							GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
							ueIndex,
							LINE_NUMBER,
							internalCellIndex,
							DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE,
							DEFAULT_FLOAT_VALUE,
							DEFAULT_FLOAT_VALUE,
							FUNCTION_NAME,
							"MAC_ULUECONTEXT_NOT_FOUND");
				}

				/* Pop the node from the Queue */
				resetUENode_p =
					/*CA Changes start  */
					(MacResetQueueNode *) popNode(macResetQueue_gp[internalCellIndex]);
				/*CA Changes end  */
				/*SPR 4017 PH2 Start*/
				if(resetUENode_p)
				{
					freeMemPool(resetUENode_p);
					resetUENode_p = PNULL;
				}
				/*SPR 4017 PH2 End*/

				/* Review comment fix start */
				LOG_MAC_MSG(PROCESS_PENDING_RESET_UE_PARAMS,LOGINFO,MAC_RRC_INF,
						GET_CURRENT_TICK_FOR_CELL(internalCellIndex),ueIndex,transactionId, 
						resetUeQueueCount, 
						internalCellIndex, DEFAULT_INT_VALUE, 
						DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME, "PROCESS_PENDING_RESET_UE_VAR");
				/* Review comment fix end */
				/*Send the Confirm message to RRC*/
				resetUECnf(ueIndex, transactionId
						/* CA changes Start */
						,internalCellIndex
						/* CA Changes end */
					  );
				/*Set pendingDeleteFlag to FALSE to make this UE active*/
				dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag = FALSE;
				ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag = FALSE;

			}
			else
			{
				return MAC_SUCCESS;
			}
		}
		else
		{
			LOG_MAC_MSG(MAC_INVALID_NODE,LOGWARNING,MAC_RRC_INF,
					GET_CURRENT_TICK_FOR_CELL(internalCellIndex),resetUeQueueCount,internalCellIndex, 
					DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"INVALID_NODE");
			break;
		}
		resetUeQueueCount-- ; 
	}
	return MAC_SUCCESS;
}
/*SPR 16855 +-*/
/****************************************************************************
 * Function Name  : deleteUEPucchSRSContext
 * Inputs         : ueIndex : UE index whose entries are to be deleted.
 *                  *deleteULUEBitMap -
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : deleteULUEBitMap : bitmap of changed maps
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function will remove entries from all the passive maps 
 *                  for a given UE and update the corresponding bitmap as output.
 ****************************************************************************/
/** SPR 12320 Fix : START **/
 UInt32 deleteUEPucchSRSContext (UInt16 ueIndex, 
		UInt32 *deleteULUEBitMap,
		UeCellInfo * ueCellInfo_p,
		InternalCellIndex internalCellIndex)
{
	ULUEContext     *newULUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
	PucchConfig     *pucchConfig_p      = &(newULUEContext_p->pucchConfigInfo);
	SRSConfigInfo   *srsConfigInfo_p    = &(newULUEContext_p->srsConfigInfo);

	if(!deleteULUEBitMap)
	{
		return MAC_FAILURE;
	}
#ifdef FDD_CONFIG
	if ( srsConfigInfo_p->srsPeriodicity <= 10 )
#else
	/*Coverity 65691 Start*/
	if ( srsConfigInfo_p->srsPeriodicity == 2 )
#endif
	/*SPR 7800 Changes End */
	{
		/*CA Changes start  */      
		if (srsConfigInfo_p->srsReportHeadNode_p[shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex])
		{
			deleteDedicatedSRSConfig(srsConfigInfo_p,PNULL,internalCellIndex);
			*deleteULUEBitMap = *deleteULUEBitMap |UL_SHORT_SRS_MAP_INDICATION;
		}
		/*CA Changes end  */
	}
	else
	{
		/*CA Changes start  */
		if (srsConfigInfo_p->srsReportHeadNode_p[longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex])
		{
			deleteDedicatedSRSConfig( srsConfigInfo_p,PNULL,internalCellIndex );
			*deleteULUEBitMap = *deleteULUEBitMap | UL_LONG_SRS_MAP_INDICATION;
		}
		/*CA Changes end  */
	}
	/*Coverity 65691 End*/
	/*TDD SB_CQI */
	/* + CA_TDD_HARQ_CHANGES */    
	if (ueCellInfo_p->isPCell)
		/* - CA_TDD_HARQ_CHANGES */    
	{
		/* Delete the PUCCH context for the UE */
		if (pucchConfig_p->srPeriodicity == SHORT_SR_PERIODICITY)
		{
			/* PUCCH TDD ADAPTATION CHANGES Start */        
			deletePucchShortSRNodesForUE( pucchConfig_p,PNULL,internalCellIndex );
			/* PUCCH TDD ADAPTATION CHANGES End */        
			*deleteULUEBitMap = *deleteULUEBitMap | UL_SHORT_SR_MAP_INDICATION  ;
		}
		else
		{
			/* PUCCH TDD ADAPTATION CHANGES Start */        
			deletePCellPucchNodesForUE(&newULUEContext_p->pucchConfigInfo,
					SCHEDULING_REQUEST,
					internalCellIndex );
			/* PUCCH TDD ADAPTATION CHANGES End */        
		}
		/* PUCCH TDD ADAPTATION CHANGES Start */        
		deletePCellPucchNodesForUE(&newULUEContext_p->pucchConfigInfo,WIDEBAND_CQI_REPORT,
				internalCellIndex);
		deletePCellPucchNodesForUE(&newULUEContext_p->pucchConfigInfo,CQI_REPORT,
				internalCellIndex);
		deletePCellPucchNodesForUE(&newULUEContext_p->pucchConfigInfo,RI_REPORT,
				internalCellIndex );
	}
	/* + CA_TDD_HARQ_CHANGES */    
	/* - CA_TDD_HARQ_CHANGES */    
	else
	{
		pucchConfig_p = &(newULUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]\
				->scellpucchConfigInfo);
		deleteSCellPucchNodesForUE(pucchConfig_p,WIDEBAND_CQI_REPORT,ueCellInfo_p,internalCellIndex,
				ueIndex);
		deleteSCellPucchNodesForUE(pucchConfig_p,CQI_REPORT,ueCellInfo_p,internalCellIndex,
				ueIndex);
		deleteSCellPucchNodesForUE(pucchConfig_p,RI_REPORT,ueCellInfo_p,internalCellIndex,
				ueIndex);
	}
	/* + CA_TDD_HARQ_CHANGES */    
	/* - CA_TDD_HARQ_CHANGES */    
	/** SPR 12320 Fix : END **/
	/* PUCCH TDD ADAPTATION CHANGES End */        
	*deleteULUEBitMap = *deleteULUEBitMap |UL_PUCCH_MAP_INDICATION;
	/*TDD SB_CQI */
	/** SPR 14195 Fix : START **/
	/*Redundant Code Removed*/
	/** SPR 14195 Fix : END **/
	return MAC_SUCCESS;
}
/*BUG:9307:START*/
/****************************************************************************
 * Function Name  : updateTotalActiveUEs
 * Inputs         : ueIndex
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : deleteDLUEBitMap
 * Returns        : MAC_SUCCESS or MAC_FAILURE 
 * Description    : This function will remove entries from passive measgap maps
 *                  for a given UE and update the corresponding bitmap as output.
 ****************************************************************************/
 void updateTotalActiveUEs(UInt16 ueIndex, 
		InternalCellIndex internalCellIndex)
{
	ULUEContext *newULUEContext_p          = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
	DLUEContext *newDLUEContext_p          = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	if(newDLUEContext_p != PNULL)
	{
		if(newDLUEContext_p->isUEConsideredForTotalActiveUeDL == TRUE)
		{
			{
                if(cellParams_p->totalActiveUEsDL > 0)
			    {
				    cellParams_p->totalActiveUEsDL--;
                }
			}
			newDLUEContext_p->isUEConsideredForTotalActiveUeDL =  FALSE;
		}
	}
	if(newULUEContext_p != PNULL)
	{
		if(newULUEContext_p->isUEConsideredForTotalActiveUeUL == TRUE)
		{
			{
			if(cellParams_p->totalActiveUEsUL > 0)
			{
				cellParams_p->totalActiveUEsUL--;
			}
			}
			newULUEContext_p->isUEConsideredForTotalActiveUeUL =  FALSE;
		}
	}
}

/*BUG:9307:END*/
/****************************************************************************
 * Function Name  : deleteUEMeasgapContext
 * Inputs         : ueIndex,
 *                  *deleteDLUEBitMap
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : deleteDLUEBitMap
 * Returns        : MAC_SUCCESS or MAC_FAILURE 
 * Description    : This function will remove entries from passive measgap maps
 *                  for a given UE and update the corresponding bitmap as output.
 ****************************************************************************/
 UInt32 deleteUEMeasgapContext (UInt16 ueIndex, UInt32 *deleteDLUEBitMap,
		InternalCellIndex cellIndex)
{
	ULUEContext *newULUEContext_p          = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
	DLUEContext *newDLUEContext_p          = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
	MeasGapContextDL *dlMeasGapContextInfo = &(newDLUEContext_p->measGapContext);
	MeasGapContextUL *ulMeasGapContextInfo = &(newULUEContext_p->measGapContext);
	UInt8 dlMeasGapConfigType              = 0;
	UInt8 ulMeasGapConfigType              = 0;

	if(!deleteDLUEBitMap)
	{
		return MAC_FAILURE;
	}
	/* + SPR#6153 */
	if(SETUP == newDLUEContext_p->measGapConfigType)
	{
		/* - SPR#6153 */
		if ( GP0 == dlMeasGapContextInfo->measGapConfig.gapPatternId
#ifdef ENDC_ENABLED
			/*Meas_Gap_Changes_Start*/
			|| GP7_R15 == dlMeasGapContextInfo->measGapConfig.gapPatternId
			/*Meas_Gap_Changes_End*/
#endif
			)
		{
			/*CA Changes start  */
			if (dlMeasGapContextInfo->measGapStartNodeDL_p[measGap40msInfo_gp[cellIndex]->passiveIndex])
				/*CA Changes end  */
			{
				deleteUEMeasGapConfig (dlMeasGapContextInfo,
						ulMeasGapContextInfo,
						&dlMeasGapConfigType,
						&ulMeasGapConfigType,
						ueIndex,
						dlMeasGapContextInfo->measGapConfig.gapPatternId,
						cellIndex);
				*deleteDLUEBitMap = DL_MEAS_GAP_40MS_MAP_INDICATION;

			}
		}
		else if(GP1 == dlMeasGapContextInfo->measGapConfig.gapPatternId
#ifdef ENDC_ENABLED
			/*Meas_Gap_Changes_Start*/
			|| GP8_R15 == dlMeasGapContextInfo->measGapConfig.gapPatternId
			/*Meas_Gap_Changes_End*/
#endif
			)
		{
			/*CA Changes start  */
			if (dlMeasGapContextInfo->measGapStartNodeDL_p[measGap80msInfo_gp[cellIndex]->passiveIndex])
				/*CA Changes end  */
			{
				deleteUEMeasGapConfig (dlMeasGapContextInfo,
						ulMeasGapContextInfo,
						&dlMeasGapConfigType,
						&ulMeasGapConfigType,
						ueIndex,
						dlMeasGapContextInfo->measGapConfig.gapPatternId,
						cellIndex);

				*deleteDLUEBitMap = DL_MEAS_GAP_80MS_MAP_INDICATION;


			}
		}
#ifdef ENDC_ENABLED
		/*Meas_Gap_Changes_Start*/
		else if(GP4_R15 == dlMeasGapContextInfo->measGapConfig.gapPatternId
			|| GP6_R15 == dlMeasGapContextInfo->measGapConfig.gapPatternId
			|| GP10_R15 == dlMeasGapContextInfo->measGapConfig.gapPatternId)
		{
			if (dlMeasGapContextInfo->measGapStartNodeDL_p[measGap20msInfo_gp[cellIndex]->passiveIndex])
				/*CA Changes end  */
			{
				deleteUEMeasGapConfig (dlMeasGapContextInfo,
						ulMeasGapContextInfo,
						&dlMeasGapConfigType,
						&ulMeasGapConfigType,
						ueIndex,
						dlMeasGapContextInfo->measGapConfig.gapPatternId,
						cellIndex);

				*deleteDLUEBitMap = DL_MEAS_GAP_20MS_MAP_INDICATION;
			}

		}
		else
		{
			if (dlMeasGapContextInfo->measGapStartNodeDL_p[measGap160msInfo_gp[cellIndex]->passiveIndex])
				/*CA Changes end  */
			{
				deleteUEMeasGapConfig (dlMeasGapContextInfo,
						ulMeasGapContextInfo,
						&dlMeasGapConfigType,
						&ulMeasGapConfigType,
						ueIndex,
						dlMeasGapContextInfo->measGapConfig.gapPatternId,
						cellIndex);

				*deleteDLUEBitMap = DL_MEAS_GAP_160MS_MAP_INDICATION;
			}

		}
		/*Meas_Gap_Changes_End*/
#endif
		/* + SPR#6153 */
	}
	/* - SPR#6153 */    
	return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : deleteUEContextUlHiPrioThd
 * Inputs         : ulDelay,currentGlobalTick 
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : NONE 
 * Returns        : MAC_SUCCESS or MAC_FAILURE 
 * Description    : This function will clean up pending delete by swapping
 *                  active and passive map indeces in UL high priority thread.
 ****************************************************************************/
/* SPR 11215 Changes Start */
#if defined(FAPI_1_1_COMPLIANCE) || defined(FAPI_4_0_COMPLIANCE)
/* SPR 11215 Changes End */
/* +- SPR 17777 */
 UInt32 deleteUEContextUlHiPrioThd (
#ifdef FDD_CONFIG
        UInt8 ulDelay,
#endif
        /* +- SPR 17777 */
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
		InternalCellIndex cellIndex)
#else
 UInt32 deleteUEContextUlHiPrioThd (InternalCellIndex cellIndex)
#endif
{

	/* CA changes Start */
	if(ulRrcOamDeleteUeQNodeReq_p[cellIndex])
		/* CA changes End */
	{
		/** CA-TDD Changes Start **/
		if(ttiCountUl_g[cellIndex])
		{
			ttiCountUl_g[cellIndex]--;
			/** CA-TDD Changes Stop **/
		}
		else
		{
			RrcOamHiPrioQNode *ulRrcOamQNodeResp_p= PNULL;
			/* CA changes Start */
			ULDeleteUeReq   *ulDeleteUeReqNode_p  = &ulRrcOamDeleteUeQNodeReq_p
				[cellIndex]->rrcOamReqResp.ulDeleteUeReq;;
			/* CA changes End */
			ULDeleteUeResp  *ulDeleteUeRespNode_p = PNULL;
			/*SPR 4503 CHANGES*/
			UInt16           ueIndex              = ulDeleteUeReqNode_p->ueIndex;
			ULUEContext     *ueULContext_p        = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
			/*SPR 4503 CHANGES*/
			GET_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(ulRrcOamQNodeResp_p,RrcOamHiPrioQNode);
			if(!ulRrcOamQNodeResp_p)
			{
				ltePanic("allocation fail for dlRrcOamQNodeResp_p");
				/*KLOCKWORK_CHANGES_START_164*/
				return MAC_FAILURE;
				/*KLOCKWORK_CHANGES_STOP_164*/
			}

			ulDeleteUeRespNode_p = &ulRrcOamQNodeResp_p->rrcOamReqResp.ulDeleteUeResp;
			ulDeleteUeRespNode_p->deleteUEMsg     = ulDeleteUeReqNode_p->deleteUEMsg;
            /* +- SPR 22354 */ 
			/*REL10_CR_CHANGES_START*/
			UInt8 loopIdx = 0;
			/*REL10_CR_CHANGES_END*/
            /* +- SPR 22354 */ 
			/* toggle changed maps indices */
			switch(ulDeleteUeReqNode_p->deleteUEMsg)
			{
				case RRC_CONFIGURE_SHORT_SR_MAP:
					{
						/*CA Changes start  */
						shortPeriodicSRMapShortInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRMap;
						shortPeriodicSRMapShortInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRMap ^0x01;
					}
					break;
				case RRC_CONFIGURE_PUCCH_MAP:
					{
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForPUCCHMap;
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForPUCCHMap ^0x01;
					}
					break;
				case RRC_CONFIGURE_PUCCH_SHORT_SR_MAP:
					{
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForPUCCHMap;
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForPUCCHMap ^0x01;
						shortPeriodicSRMapShortInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRMap;
						shortPeriodicSRMapShortInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRMap ^0x01;
					}
					break;
				case RRC_CONFIGURE_SHORT_SRS_MAP:
					{
						shortPeriodicSRSMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRSMap;
						shortPeriodicSRSMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRSMap ^0x01;
					}
					break;
				case RRC_CONFIGURE_SHORT_SR_SHORT_SRS_MAP:
					{
						shortPeriodicSRMapShortInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRMap;
						shortPeriodicSRMapShortInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRMap ^0x01;
						shortPeriodicSRSMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRSMap;
						shortPeriodicSRSMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRSMap ^0x01;
					}
					break;
				case RRC_CONFIGURE_PUCCH_SHORT_SRS_MAP:
					{
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForPUCCHMap;
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForPUCCHMap ^0x01;
						shortPeriodicSRSMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRSMap;
						shortPeriodicSRSMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRSMap ^0x01;
					}
					break;
				case RRC_CONFIGURE_PUCCH_SHORT_SR_SHORT_SRS_MAP:
					{
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForPUCCHMap;
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForPUCCHMap ^0x01;
						shortPeriodicSRMapShortInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRMap;
						shortPeriodicSRMapShortInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRMap ^0x01;
						shortPeriodicSRSMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRSMap;
						shortPeriodicSRSMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRSMap ^0x01;
					}
					break;
				case RRC_CONFIGURE_LONG_SRS_MAP:
					{
						longPeriodicSRSMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForLongSRSMap;
						longPeriodicSRSMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForLongSRSMap ^0x01;
					}
					break;
				case RRC_CONFIGURE_SHORT_SR_LONG_SRS_MAP:
					{
						shortPeriodicSRMapShortInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRMap;
						shortPeriodicSRMapShortInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRMap ^0x01;
						longPeriodicSRSMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForLongSRSMap;
						longPeriodicSRSMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForLongSRSMap ^0x01;
					}
					break;
				case RRC_CONFIGURE_PUCCH_LONG_SRS_MAP:
					{
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForPUCCHMap;
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForPUCCHMap ^0x01;
						longPeriodicSRSMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForLongSRSMap;
						longPeriodicSRSMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForLongSRSMap ^0x01;
					}
					break;
				case RRC_CONFIGURE_PUCCH_SHORT_SR_LONG_SRS_MAP:
					{
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForPUCCHMap;
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForPUCCHMap ^0x01;
						shortPeriodicSRMapShortInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRMap;
						shortPeriodicSRMapShortInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForShortSRMap ^0x01;
						longPeriodicSRSMapInfo_gp[cellIndex]->activeIndex =
							ulDeleteUeReqNode_p->activeIndexForLongSRSMap;
						longPeriodicSRSMapInfo_gp[cellIndex]->passiveIndex =
							ulDeleteUeReqNode_p->activeIndexForLongSRSMap ^0x01;
						/*CA Changes end  */
					}
					break;
				default:break;
			}

            /* SPR 16583 fix start */
            if( PNULL != ueULContext_p )
            { 
            /* SPR 16583 fix end */
                /* SPR 11215 Changes Start */
                /* +- SPR 22354 */
/*SPR 22919 Start*/                

#if   defined(FAPI_1_1_COMPLIANCE) || defined (FAPI_4_0_COMPLIANCE)
                /* SPR 11215 Changes End */
#ifdef FDD_CONFIG
                /*CA Changes start  */
                fillHarqReleaseinfoForULSCH(ulDelay,currentGlobalTick,ueULContext_p,cellIndex);
#elif TDD_CONFIG
                fillHarqReleaseinfoForULSCH(currentGlobalTick,ueULContext_p,cellIndex);
                /*CA Changes end  */
#endif
#endif
/*SPR 22919 End*/                

                /* SPR 4503 CHANGES */
                if(MAC_SUCCESS != deleteTpcPdcchConfigInfoForPusch(DELETE_UE_REQ,
                            /* +- SPR 17777 */
                            /*CA Changes start  */
                            ueULContext_p, cellIndex))
                    /*CA Changes end  */
                {
                    /*Nothing needs to be done in case of UE delete*/
                }
                /* SPR 4503 CHANGES */
                /* +- SPR 16583 */
                for ( loopIdx = 0; loopIdx < MAX_NUMBER_OF_LOGICAL_CHANNEL; loopIdx++)
                {
                    if ( LCGID_INVALID_VALUE != ueULContext_p->lcInfo[loopIdx].lcGId)
                    {

                        if(0 !=  ueULContext_p->bsrNet
                                [ueULContext_p->lcInfo[loopIdx].lcGId]) 
                        {
                            ulDeleteUeRespNode_p->deleteLCStatus[loopIdx] =  MAC_ACTIVE_LC;
                        }
                        else
                        {
                            ulDeleteUeRespNode_p->deleteLCStatus[loopIdx] =  MAC_INACTIVE_LC;
                        }
                    }
                    else
                    {
                        ulDeleteUeRespNode_p->deleteLCStatus[loopIdx] =  MAC_INACTIVE_LC;
                    }

                }
            }
            /* SPR 16583 fix start */
            /* +- SPR 22354 */
			ulRrcOamQNodeResp_p->ueIndex = ulRrcOamDeleteUeQNodeReq_p
				[cellIndex]->ueIndex;
			ulRrcOamQNodeResp_p->rrcMsgId = ulRrcOamDeleteUeQNodeReq_p
				[cellIndex]->rrcMsgId;
			ulRrcOamQNodeResp_p->eventId = ulRrcOamDeleteUeQNodeReq_p
				[cellIndex]->eventId;
            /* SPR 16583 fix end */

			ulRrcOamQNodeResp_p->respCode = MAC_SUCCESS;
			/* CA changes Start */
			/*coverity id 25571*/
			if(!ENQUEUE_RRC_OAM_HPTOLP_THDH_PRIO_Q(&(rrcOamHPtoLPThdHPrioQ_g
							[cellIndex][UL_THD]),
						RrcOamHiPrioQNode, ulRrcOamQNodeResp_p))
			{
				FREE_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(ulRrcOamQNodeResp_p);
			}
			/* free request pointer and send response to LP thread */
			freeMemPool(ulRrcOamDeleteUeQNodeReq_p[cellIndex]);
			ulRrcOamDeleteUeQNodeReq_p[cellIndex] = PNULL;
			/* Send to the LP thread to process it */
			lteMessageQSend( recvMsgFrmHPThdFd_g, 
					(UInt8 *)&(cellIndex), sizeof(InternalCellIndex));
			/* CA changes End */
			/** CA-TDD Changes Start **/ 
			ttiCountUl_g[cellIndex] = UE_CONTEXT_DEL_DELAY;
			/** CA-TDD Changes Stop **/
            /* +- SPR 22354 */
		}
	}
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : deleteUEContextDlHiPrioThd
 * Inputs         : internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : NONE 
 * Returns        : MAC_SUCCESS or MAC_FAILURE 
 * Description    : This function will clean up pending delete by swapping
 *                  active and passive map indeces in DL high priority thread.
 ****************************************************************************/
/*CA Changes start  */
 UInt32 deleteUEContextDlHiPrioThd (InternalCellIndex internalCellIndex)
	/*CA Changes end  */
{
	/* CA changes Start */
	if(dlRrcOamDeleteUeQNodeReq_p[internalCellIndex])
		/* CA changes End */
	{
		/** CA-TDD Changes Start **/
		if(ttiCountDl_g[internalCellIndex])
		{
			ttiCountDl_g[internalCellIndex]--;
			/** CA-TDD Changes Stop **/
		}
		else
		{
			RrcOamHiPrioQNode *dlRrcOamQNodeResp_p= PNULL;
			/* CA changes Start */
			DLDeleteUeReq   *dlDeleteUeReqNode_p  = &dlRrcOamDeleteUeQNodeReq_p
				[internalCellIndex]->rrcOamReqResp.dlDeleteUeReq;;
			/* CA changes End */
			DLDeleteUeResp  *dlDeleteUeRespNode_p = PNULL;
			UInt16           ueIndex              = dlDeleteUeReqNode_p->ueIndex;       
			DLUEContext     *ueDLContext_p        = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
			ULUEContext     *ueULContext_p        = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
			GET_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(dlRrcOamQNodeResp_p,RrcOamHiPrioQNode);

			if(!dlRrcOamQNodeResp_p)
			{
				ltePanic("allocation fail for dlRrcOamQNodeResp_p");
				/*KLOCKWORK_CHANGES_START_163*/
				return MAC_FAILURE;
				/*KLOCKWORK_CHANGES_STOP_163*/
			}

			dlDeleteUeRespNode_p = &dlRrcOamQNodeResp_p->rrcOamReqResp.dlDeleteUeResp;
			dlDeleteUeRespNode_p->deleteUEMsg     = dlDeleteUeReqNode_p->deleteUEMsg;
			/*REL10_CR_CHANGES_START*/
			UInt8 loopIdx = 0;
			/*REL10_CR_CHANGES_END*/

			/* toggle changed maps indices */
			switch(dlDeleteUeReqNode_p->deleteUEMsg)
			{
				case RRC_CONFIGURE_MEAS_GAP_80MS_MAP:
					{
						/*CA Changes start  */
						measGap80msInfo_gp[internalCellIndex]->activeIndex =
							dlDeleteUeReqNode_p->activeIndexFor80MsMeasGapMap;
						measGap80msInfo_gp[internalCellIndex]->passiveIndex =
							/*CA Changes end  */
							dlDeleteUeReqNode_p->activeIndexFor80MsMeasGapMap ^ 0x01;
					}
					break;
				case RRC_CONFIGURE_MEAS_GAP_40MS_MAP:
					{
						/*CA Changes start  */
						measGap40msInfo_gp[internalCellIndex]->activeIndex =
							dlDeleteUeReqNode_p->activeIndexFor40MsMeasGapMap;
						measGap40msInfo_gp[internalCellIndex]->passiveIndex =
							/*CA Changes end  */
							dlDeleteUeReqNode_p->activeIndexFor40MsMeasGapMap ^ 0x01;
					}
					break;
#ifdef ENDC_ENABLED
					/*Meas_Gap_Changes_Start*/
				case RRC_CONFIGURE_MEAS_GAP_20MS_MAP:
					{
						measGap20msInfo_gp[internalCellIndex]->activeIndex =
							dlDeleteUeReqNode_p->activeIndexFor20MsMeasGapMap;
						measGap20msInfo_gp[internalCellIndex]->passiveIndex =
							dlDeleteUeReqNode_p->activeIndexFor20MsMeasGapMap ^ 0x01;
					}
					break;
				case RRC_CONFIGURE_MEAS_GAP_160MS_MAP:
					{
						measGap160msInfo_gp[internalCellIndex]->activeIndex =
							dlDeleteUeReqNode_p->activeIndexFor160MsMeasGapMap;
						measGap160msInfo_gp[internalCellIndex]->passiveIndex =
							dlDeleteUeReqNode_p->activeIndexFor160MsMeasGapMap ^ 0x01;
					}
					break;
					/*Meas_Gap_Changes_End*/
#endif
				default:break;
			}

		{
			/* remove UE nodes from DRX timer lists */
			if(SETUP == ueDLContext_p->drxConfigType)
			{
				/*CA Changes start  */
				deleteDRXConfig(ueDLContext_p, ueULContext_p, internalCellIndex);
			}

			/* delete entry from Qos token list */
			qosProcessTknBktAlgoForUeDelete(ueIndex, internalCellIndex);   
			/*CA Changes end  */

			/* SPR 4503 CHANGES */
			if(MAC_SUCCESS != deleteTpcPdcchConfigInfoForPucch(DELETE_UE_REQ,
                        /* +- SPR 17777 */
						/*CA Changes start  */
						ueDLContext_p, internalCellIndex))
				/*CA Changes end  */
			{
				/* Nothing needs to be done in case of UE Release*/
			}	
		}
			/* SPR 4503 CHANGES */
			/* CA changes Start */
			dlRrcOamQNodeResp_p->ueIndex = dlRrcOamDeleteUeQNodeReq_p
				[internalCellIndex]->ueIndex;
			dlRrcOamQNodeResp_p->rrcMsgId = dlRrcOamDeleteUeQNodeReq_p
				[internalCellIndex]->rrcMsgId;
			dlRrcOamQNodeResp_p->eventId = dlRrcOamDeleteUeQNodeReq_p
				[internalCellIndex]->eventId;
			/* CA changes End */
			/*REL10_CR_CHANGES_START*/
			for ( loopIdx = 0; loopIdx < MAX_NUMBER_OF_LOGICAL_CHANNEL; loopIdx++)
			{
				if(0 !=  ueDLContext_p->logicalChannel[loopIdx].queueLoad)
                               {
					dlDeleteUeRespNode_p->deleteLCStatus[loopIdx] =  MAC_ACTIVE_LC;
				}
				else
				{
					dlDeleteUeRespNode_p->deleteLCStatus[loopIdx] =  MAC_INACTIVE_LC;
				}
			}          
			/*REL10_CR_CHANGES_END*/
			dlRrcOamQNodeResp_p->respCode = MAC_SUCCESS;
			/* CA changes Start */
			/*coverity id 25570*/
			if(!ENQUEUE_RRC_OAM_HPTOLP_THDH_PRIO_Q(&(rrcOamHPtoLPThdHPrioQ_g
							[internalCellIndex][DL_THD]),
						RrcOamHiPrioQNode, dlRrcOamQNodeResp_p))
			{
				FREE_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(dlRrcOamQNodeResp_p);
			}
			/* free request pointer and send response to LP thread */
			freeMemPool(dlRrcOamDeleteUeQNodeReq_p[internalCellIndex]);
			dlRrcOamDeleteUeQNodeReq_p[internalCellIndex] = PNULL;
			/* Send to the LP thread to process it */
			lteMessageQSend( recvMsgFrmHPThdFd_g, 
					(UInt8 *)&(internalCellIndex), sizeof(InternalCellIndex));
			/* CA changes End */
			/** CA-TDD Changes Start **/
			ttiCountDl_g[internalCellIndex] = UE_CONTEXT_DEL_DELAY;
			/** CA-TDD Changes Stop **/
		}
	}
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : deleteUEContextLPThd
 * Inputs         : ueIndex
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : NONE 
 * Returns        : MAC_SUCCESS or MAC_FAILURE 
 * Description    : This function will delete UE and clean up from MAC system
 *                  in low priority thread.
 ****************************************************************************/
 UInt32 deleteUEContextLPThd (UInt16 ueIndex,
		InternalCellIndex internalCellIndex)
{
/*SPR 19288 changes start */
/*SPR 19288 changes end */ 
	ULUEContext                *ueULContext_p                  = PNULL;
	DLUEContext                *ueDLContext_p                  = PNULL;
	RRCRachDeleteUeInterface   *rrcRachDeleteUeInterfaceNode   = PNULL;
	UInt16                     rnti                            = 0;
	UInt8                      preambleIndex                   = 0; 

	ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
	ueULContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;

	/* CA_phase2_harq_code Start */
	UInt8                      servCellIndex                   = 0;
	DLUESCellContext *dlUeScellContext_p = PNULL;
	/* CA_phase2_harq_code End */

	/* CA Changes start */
	UInt8 index = 0;
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

	InitParams *initParams_p = cellSpecificParams_g.\
				   cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
	/* CA Changes end */
	if (PNULL != ueDLContext_p)
	{
		preambleIndex = ueDLContext_p->ulSyncLossOrHOInfoInDL.preambleIdx;
		rnti          = ueDLContext_p->crnti; 
		if (ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p)
		{
			ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p = PNULL;
		}

		/* CA_phase2_harq_code Start */
		/* + SPR_11654 */
		for (servCellIndex = MAX_NUM_PCELL; servCellIndex <= ueDLContext_p->scellCount;\
				servCellIndex++)
		{
			dlUeScellContext_p = 
				ueDLContext_p->dlUeScellContext_p[servCellIndex];
			if (dlUeScellContext_p != PNULL)
			{
				if (MAC_FAILURE == macDeleteScellMACDLHarqEntity(ueDLContext_p,
							servCellIndex,dlUeScellContext_p->internalCellIndex))
				{
					lteWarning ("\n macDeleteScellMACDLHarqEntity() failed for"
							"servCellIndex :%d \n",servCellIndex);
				}
				if(PNULL != ueDLContext_p->dlUeScellContext_p[servCellIndex]->ScelldlCQIInfo.
						subBandReportInfo_p)
				{
					freeMemPool(ueDLContext_p->dlUeScellContext_p[servCellIndex]->\
							ScelldlCQIInfo.subBandReportInfo_p);
					ueDLContext_p->dlUeScellContext_p[servCellIndex]->ScelldlCQIInfo.\
						subBandReportInfo_p = PNULL;
				}
				freeMemPool(ueDLContext_p->dlUeScellContext_p[servCellIndex]);
				ueDLContext_p->dlUeScellContext_p[servCellIndex] = PNULL;
			}
		}
		/* CA_phase2_harq_code End */

		/* - SPR_11654 */
		/* SPR 5288 Start */
		/*CA Changes start  */
		if (MAC_FAILURE == deleteMACDLHarqEntity(ueDLContext_p, internalCellIndex))
			/*CA Changes end  */
			/* SPR 5288 End */
		{
			lteWarning ("deleteMACDLHarqEntity() failed \n");
		}


		if(PNULL != ueDLContext_p->dlCQIInfo.subBandReportInfo_p)
		{
			freeMemPool(ueDLContext_p->dlCQIInfo.subBandReportInfo_p);
			ueDLContext_p->dlCQIInfo.subBandReportInfo_p = PNULL;
		}

/*SPR 19288 changes start */ 
/*SPR 19288 changes end */ 

		/* HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
		if(TRUE == ueDLContext_p->isHDFddFlag)
		{
			deleteUeFromHdFddContext(ueDLContext_p,internalCellIndex);
		}
#endif
		/* HD FDD Changes End*/
		/*! \code
		 * If SPS is enabled in system
		 *    Call dlSpsPendingDeleteHandling.
		 * \endcode
		 * */
	{
		if( TRUE == initParams_p->spsSysWideParam.enableSps )
		{
			dlSpsPendingDeleteHandling( ueDLContext_p, internalCellIndex );
			LTE_MAC_UPDATE_DOWNLINK_SPS_UE_EXIST_STATE(ueIndex,LTE_FALSE); 
			LTE_MAC_UPDATE_SPS_CONFIG_RESP_STATS(LTE_FALSE,ueIndex,0);
		}
	}

		freeMemPool(ueDLContext_p);
        ueDLContext_p = PNULL;
		dlUECtxInfoArr_g[ueIndex].dlUEContext_p = PNULL;
	}

	if (PNULL != ueULContext_p)
	{ 
		if(PNULL != ueULContext_p->csiContext.mcsToPrbMap_p)
		{
			freeMemPool(ueULContext_p->csiContext.mcsToPrbMap_p);
			ueULContext_p->csiContext.mcsToPrbMap_p = PNULL;
		}


/*SPR 19288 changes start */ 
/*SPR 19288 changes end */ 

		/* CA changes Start */
		for (index = MAX_NUM_PCELL; index <= MAX_NUM_SCELL;index ++)
		{
			if (ueULContext_p->ulUeScellContext_p[index] != PNULL)
			{
				freeMemPool(ueULContext_p->ulUeScellContext_p[index]);
				ueULContext_p->ulUeScellContext_p[index] = PNULL;
			}
		}
		/* CA changes End */

		/*! \code
		 * If SPS is enabled in system
		 *    Call dlSpsPendingDeleteHandling.
		 * \endcode
		 * */
	{
		if( TRUE == initParams_p->spsSysWideParam.enableSps )
		{
			/*CA Changes start  */
			ulSpsPendingDeleteHandling( ueULContext_p, internalCellIndex );
			/*CA Changes end  */
			LTE_MAC_UPDATE_UPLINK_SPS_UE_EXIST_STATE(ueIndex,LTE_FALSE,GET_CURRENT_TICK_FOR_CELL(internalCellIndex)); 
			LTE_MAC_UPDATE_SPS_CONFIG_RESP_STATS(LTE_FALSE,ueIndex,0);
		}
	}
		if(ueULContext_p->nonZeroBsrListTempDeleted_p)
		{
			freeMemPool(ueULContext_p->nonZeroBsrListTempDeleted_p);
			ueULContext_p->nonZeroBsrListTempDeleted_p = PNULL;
		}
		freeMemPool(ueULContext_p);
		ulUECtxInfoArr_g[ueIndex].ulUEContext_p = PNULL;
	}
	GET_MEM_FROM_POOL(RRCRachDeleteUeInterface,
			rrcRachDeleteUeInterfaceNode,
			sizeof(RRCRachDeleteUeInterface), PNULL);
	if(rrcRachDeleteUeInterfaceNode == PNULL)
	{
		lteWarning("rnti and preamble free fail due to failure in Getmempool for UEindex: %d", ueIndex);
	}
	else
	{
		rrcRachDeleteUeInterfaceNode->preambleIndex = preambleIndex;
		rrcRachDeleteUeInterfaceNode->rnti          = rnti;
        /*SPR 21103 Start*/
		if(MAC_FAILURE == putEntryInPdcchRachInterfaceQueue(RACH_DELETE_UE, rrcRachDeleteUeInterfaceNode,
				internalCellIndex))
        {
            freeMemPool(rrcRachDeleteUeInterfaceNode);
        }
        /*SPR 21103 End*/
	}
	{
	cellParams_p->totalActiveUE--;
	}
	LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UE_ACTIVE(ueIndex, FALSE);
	return MAC_SUCCESS;
}
/*SPR 16855 +-*/

/* SPR 21105 fix start */
/* code removed */
/* processPendingDeleteUE func is never referrred so, it and fuction it is
 * referring to  are removed */
/* SPR 21105 fix end */

/****************************************************************************
 * Function Name  : checkAndDeleteUEMeasGapConfig 
 * Inputs         : ueULContext_p - Pointer to UE uplink context
 *                  ueDLContext_p - Pointer to UE downlink context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None 
 * Description    : This API is used to check and delete UE Meas Gap config.
 ****************************************************************************/
STATIC  void checkAndDeleteUEMeasGapConfig (
		ULUEContext *ueULContext_p,
		DLUEContext *ueDLContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex
		/*CA Changes end  */
		)
{
	/*MEAS_GAP_CHG */
	if(SETUP == ueDLContext_p->measGapConfigType)
	{
        /*SPR 16855 +-*/
		/*CA Changes start  */
		deleteUEMeasGapConfig(&ueDLContext_p->measGapContext,
				&ueULContext_p->measGapContext,
				&ueDLContext_p->measGapConfigType,
				&ueULContext_p->measGapConfigType,
				ueDLContext_p->ueIndex,
				ueDLContext_p->measGapContext.measGapConfig.gapPatternId,
				internalCellIndex);
        /*SPR 16855 +-*/
	}
	/* MEAS_GAP_CHG */
}
/* SPR 21105 fix start */
/* code removed */
/* processPendingDeleteUE func is never referrred so, it and fuction it is
 * referring to  are removed */
/* SPR 21105 fix end */

/* SPR 21105 fix start */
/* code removed */
/* processPendingDeleteUE func is never referrred so, it and fuction it is
 * referring to  are removed */
/* SPR 21105 fix end */
/* Cyclomatic Complexity changes - ends here */
/****************************************************************************
 * Function Name  : deleteMACDLUEConnHandOverEntity
 * Inputs         : ueIndex - index of ueContext 
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                                      
 * Outputs        : DL Context of the UE deleted 
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS, MAC_LOG_CH_ERROR]
 * Description    : The function is used to delete UE context created for 
 *                  connection Hand Over
 ****************************************************************************/
/*CA Changes start  */
MacRetType deleteMACDLUEConnHandOverEntity(UInt16 ueIndex, InternalCellIndex internalCellIndex)
	/*CA Changes end  */
{
	DLUEContext *ueDLContext_p  = PNULL;
	UInt8 preambleIndex = INVALID_PREAMBLE_IDX;

	if (  PNULL == dlUECtxInfoArr_g[ueIndex].dlUEContext_p )
	{
		LOG_MAC_MSG(DEL_DL_UE_CONTXT_HO_ID,
				LOGWARNING,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				ueIndex,
				preambleIndex,
				internalCellIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,"DEL_DL_UE_CONTXT_HO_FAIL");

		return MAC_FAILURE;
	}
	ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

	preambleIndex = ueDLContext_p->ulSyncLossOrHOInfoInDL.preambleIdx;

	if(INVALID_PREAMBLE_IDX != preambleIndex)
	{
		/*CA Changes start  */
		if(PREAMBLE_BUSY == getPreambleStatusInfo(preambleIndex, internalCellIndex))
			/*CA Changes end  */
		{
			/*CA Changes start  */
			freePreambleIndex(preambleIndex, internalCellIndex);
			/*CA Changes end  */
		}
		else
		{
			LOG_MAC_MSG(PREAMBLE_NOT_BUSY_ID,
					LOGERROR,
					MAC_RACH,
					GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
					ueIndex,
					ueDLContext_p->crnti,
					preambleIndex,
					LINE_NUMBER,
					internalCellIndex,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"PREAMBLE_NOT_BUSY_ID");

		}
	}
	else
	{
		LOG_MAC_MSG(INVALID_PREAMBLE_INDEX_ID,
				LOGINFO,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				preambleIndex,
				ueIndex,
				ueDLContext_p->crnti,
				LINE_NUMBER,
				internalCellIndex,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,"INVALID_PREAMBLE_INDEX_ID");

	}

	LOG_MAC_MSG(DEL_DL_UE_CONTXT_HO_ID,
			LOGWARNING,
			MAC_RRC_INF,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			ueIndex,
			preambleIndex,
			internalCellIndex,
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,
			DEFAULT_FLOAT_VALUE,
			FUNCTION_NAME,"DEL_DL_UE_CONTXT_HO_SUCC");
    /*  +- SPR 16583 */

	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : reconfigureMACULUEEntity
 * Inputs         : uplkUEInfo_p - Pointer to MAC UL Info
 *                  uplkLCCount
 *                  ulReconfigUeReq_p - Pointer for UL UE Reconfiguration Request
 *                  transmissionMode 
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This API is used to fetch the UE Context pointer from the
 *                  Index Table and then reconfigure the various UE Context
 *                  parameters for the requested UE.
 ****************************************************************************/
/*SPR 16855 +-*/
/* +- SPR 17777 */
 STATIC MacRetType reconfigureMACULUEEntity(UeContextForUplink *uplkUEInfo_p,
		UInt32 uplkLCCount,ULReconfigUeReq *ulReconfigUeReq_p,
		/* CA Changes Start */
		InternalCellIndex internalCellIndex
		/* CA changes End */
		)
/* +- SPR 17777 */
/*SPR 16855 +-*/
{
    UInt32 ueIdx = uplkUEInfo_p->ueIdx; 
    ulLogicalChannelInfo *lcInfo_p = uplkUEInfo_p->lcInfo;
    /*SPR 16855 +-*/
    /* Get the pointer to ueContext from static UE context Table */
    ULUEContext *ueContext_p = PNULL;
    /* SPS_CHG */
    LP_SpsUlInfo  spsUlInfo_p  = PNULL;
    /* SPS_CHG */
    ueContext_p = ulUECtxInfoArr_g[ueIdx].ulUEContext_p;
    /* SPR 22515 code deleted */
    if (PNULL == ueContext_p )
    {
        lteWarning (" Entry does not exists in the UEContext Idx Table.\n");
        LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR,
                " Unable to reconfigure the UE Entity. \n");
        return MAC_FAILURE;
    }
    if (PNULL == dlUECtxInfoArr_g[ueIdx].dlUEContext_p)
    {
        lteWarning (" Entry does not exists in the dynUEContext Table.\n");
        LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR,
                " Unable to reconfigure the UE Entity. \n");
        return MAC_FAILURE;
    }
    /*CLPC Change*/
    if ( INVALID_SIMULTANEOUS_ACK_NACK_VALUE != 
            uplkUEInfo_p->simultaneousACKNACKAndCQI )
    { 
        /*SPR 16855 +-*/
        ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.ulCqiReqInfo.cqiPeriodicReqInfo.simultaneousAckNackAndCQI=
            uplkUEInfo_p->simultaneousACKNACKAndCQI;
        /*SPR 16855 +-*/
    }
    if (PNULL == lcInfo_p)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR,
                "Received a NULL Pointer for Logical Channels.\n");
        return MAC_SUCCESS;
    }
    /*SPR 16855 +-*/
    /* spr 21260 + */
    /* SPR 22515 SPR fix start */
    ueContext_p->isCqiDropped = FALSE;
    ueContext_p->cqiDropTick  = 0;
    /* spr 21260 - */
    /* SPR 22515 SPR fix end */
    /*CYCLOMATIC_CHANGES_6.1_START*/
    determineULMinDataSize(uplkUEInfo_p,ueContext_p);
    determineMaxTBSize(uplkUEInfo_p,ulReconfigUeReq_p);
    if (MAC_FAILURE == validateUeCatV1020ReqType(uplkUEInfo_p,internalCellIndex,ulReconfigUeReq_p))
    {
        return MAC_FAILURE;
    }
    /*CYCLOMATIC_CHANGES_6.1_END*/
    /* SPR 11777 fix Changes End */

    /*REL10_CR_CHANGES_START*/
    /*SPR 16855 +-*/
    ulReconfigUeReq_p->maxUlHARQTx = uplkUEInfo_p->maxUlHARQTx;
    ulReconfigUeReq_p->pcMax = uplkUEInfo_p->pcMax;
    /* PUSCH Config */
    /*CYCLOMATIC_CHANGES_6.1_START*/
    populatePuschConfigInfo(uplkUEInfo_p,ulReconfigUeReq_p);
    /*CYCLOMATIC_CHANGES_6.1_END*/
    /*SPR 16855 +-*/
    /*REL10_CR_CHANGES_END*/
    /*CYCLOMATIC_CHANGES_6.1_START*/
    /* +- SPR 17777 */
    populateDedicatedULPowerControlInfo(uplkUEInfo_p,ulReconfigUeReq_p);
    /* +- SPR 17777 */
    /*SPR 16855 +-*/
    populateTpcPdcchConfigPuschInfo(uplkUEInfo_p,ulReconfigUeReq_p);
    /*SPR 16855 +-*/
    /*CYCLOMATIC_CHANGES_6.1_END*/
    /* SPS_CHG */
    /* Initilaize schType to DRA_SCHEDULING by default */
    ueContext_p->schType = DRA_SCHEDULING;

    /* Assiging the SPS Config parms to the Ue COntext */
    if(((TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                    ->initParams_p->spsSysWideParam.enableSps)&&
                (TRUE ==  uplkUEInfo_p->isSpsUlInfoPresent))
#ifdef HD_FDD_CONFIG
            /* Config UE  --   True  False INVALID TRUE
               Reconfig UE--   True  True  TRUE    INVALID
               i.e., UE is going to be set to Half Duplex mode
               */
            ||
            ((((ueContext_p->isHDFddFlag == TRUE) && (uplkUEInfo_p->isHDFddFlag != FALSE)) ||
              ((ueContext_p->isHDFddFlag != TRUE) && (uplkUEInfo_p->isHDFddFlag == TRUE))) &&
             /* This UE was in SPS mode, so need to remove the SPS for HD UE now */
             (ueContext_p->spsUlInfo.requestType == SPS_SETUP))
#endif
      )

    {

        /* SPR 6776 SPS Reconfig Fix Start*/
        /*SPR 16855 +-*/
        /* In case of UL/DL running different, store the received HDFDD flag which
         * will be updated in other operations for both UL and DL UE Context */
        ulReconfigUeReq_p->ueReconfigTlvRecieved[ulReconfigUeReq_p->ueReconfigMsgTlvCount]= UL_RECONFIG_SPS_PARAMS;
        ulReconfigUeReq_p->ueReconfigMsgTlvCount++;
        /*SPR 16855 +-*/
        /* SPR 6776 SPS Reconfig Fix End*/
        spsUlInfo_p = &ueContext_p->spsUlInfo;
        /* Resting the isSpsUlInfoPresent flag */
        uplkUEInfo_p->isSpsUlInfoPresent = FALSE;
        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
        /* If HD FDD is enabled then marking SPS Request Type as SPS_RELEASE*/
        if (((ueContext_p->isHDFddFlag == TRUE) && (uplkUEInfo_p->isHDFddFlag != FALSE)) ||
                ((ueContext_p->isHDFddFlag != TRUE) && (uplkUEInfo_p->isHDFddFlag == TRUE)))
        {
            uplkUEInfo_p->spsUplinkInfo.requestType = SPS_RELEASE;
        }
#endif
        /*HD FDD Changes End*/
        if(spsUlInfo_p->requestType ==
                uplkUEInfo_p->spsUplinkInfo.requestType)
        { 
            LOG_MAC_MSG(MAC_SPS_RECONFIG, LOGWARNING, MAC_RRC_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex), __LINE__,ueContext_p->ueIndex,
                    ueContext_p->spsUlInfo.requestType,
                    uplkUEInfo_p->spsUplinkInfo.requestType,
                    internalCellIndex, 0,0, __func__, "" );

        }
        else
        {
            /*SPR 16855 +-*/
            ulReconfigUeReq_p->spsUlInfo.requestType = uplkUEInfo_p->spsUplinkInfo.requestType;
            /*SPR 16855 +-*/
            if(SPS_RELEASE == \
                    uplkUEInfo_p->spsUplinkInfo.requestType )
            {
                /*SPR 16855 +-*/
            }
            /* SPR 20456 start */
            /* code removed */
            /* SPR 20456 stop */

            else
            {
                /*!
                 * \code
                 * IF Sps Req type is setup
                 *    - Save the sps crnti in Ue context
                 *    -Populating the SPS Info in UE Context recived from L3 
                 * \endcode
                 * */
                /*SPR 16855 +-*/
                /*CYCLOMATIC_CHANGES_6.1_START*/
                populateUplinkSpsInfo(uplkUEInfo_p,ueContext_p,internalCellIndex,ulReconfigUeReq_p);
                /*CYCLOMATIC_CHANGES_6.1_END*/
                /*SPR 16855 +-*/
            }

        }
    }
    else
    {
        /*SPR 16855 +-*/
        ulReconfigUeReq_p->spsUlInfo.requestType = SPS_RELEASE;
        /*SPR 16855 +-*/
    }


    /* SPS_CHG */
    /*SPR 16855 +-*/
    /*CYCLOMATIC_CHANGES_6.1_START*/
    populateUplinkLcInfo(uplkLCCount, ulReconfigUeReq_p,uplkUEInfo_p);
    /*CYCLOMATIC_CHANGES_6.1_END*/
    LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] At LP (UL_RECONFIG_CONFIGURE_LC_REQ)  ,numLogicalChannels = %d \n",__func__
            ,ulReconfigUeReq_p->ulReconfigLcReq.numLogicalChannels ); 
    /*SPR 16855 +-*/
    /*CYCLOMATIC_CHANGES_6.1_START*/ 
    fillUlQosAmbr(uplkUEInfo_p,
            /*SPR 16855 +-*/
            ulReconfigUeReq_p
            /*SPR 16855 +-*/
            );
    /*CYCLOMATIC_CHANGES_6.1_END*/
    LTE_MAC_UPDATE_UPLINK_SPS_UE_STATS(ueContext_p->ueIndex,
            &(ueContext_p->spsUlInfo) );

    /*SPR 16855 +-*/
    /* +DYNAMIC_ICIC */
    ueContext_p->phrGrant = uplkUEInfo_p->phrGrant;
    ueContext_p->phrProhibitTimerExpiryTick = 0;
    ueContext_p->phrTimerCfg.periodicPHRTimer = 0;
    ueContext_p->phrTimerCfg.prohibitPHRTimer = uplkUEInfo_p->phrTimerCfg.prohibitPHRTimer;
    /* -DYNAMIC_ICIC*/

    /*SPR21501 fix +-*/
    /*SPR 16855 +-*/
    ulReconfigUeReq_p->ttiBundlingEnabled = uplkUEInfo_p->ttiBundlingEnabled;
    /*SPR 16855 +-*/
    /* CA changes Start */
    /* SPR 11112 fix start */
    /*CYCLOMATIC_CHANGES_6.1_START*/    
    populateN1PucchListN1PucchCs(ulReconfigUeReq_p,uplkUEInfo_p);
    /*CYCLOMATIC_CHANGES_6.1_END*/
    if (MAC_SUCCESS != macValidateULReconfigUECategoryV1020Params (
                uplkUEInfo_p,
                /* +- SPR 17777 */
                internalCellIndex
                /*SPR 16855 +-*/
                ,ulReconfigUeReq_p
                /*SPR 16855 +-*/
                ))
    {
        return MAC_FAILURE;
    }
    /* SPR 11112 fix end */

    if (MAC_SUCCESS != macValidateAndFillULScellReconfigParams(uplkUEInfo_p, 
                ueContext_p,internalCellIndex
                /*SPR 16855 +-*/
                ,ulReconfigUeReq_p
                /*SPR 16855 +-*/
                ))
    {
        return MAC_FAILURE;
    }
    /* CA changes End */
    /*EICIC changes Starts*/
    if ( LTE_TRUE == uplkUEInfo_p->userTypePresent)
    {
        ulReconfigUeReq_p->interferenceConfigUL.userType = uplkUEInfo_p->userType;
    }
    /*EICIC Changes Ends*/
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    ueContext_p->isUplinkRAT1Supported = uplkUEInfo_p->isUplinkRAT1Supported;
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
    return MAC_SUCCESS;
}


/****************************************************************************
 * Function Name  : addULUELogicalChannel
 * Inputs         : newUEContext_p - Pointer to ue context created
 *                  logicalChCount - Number of logical channels to be configured
 *                  logicalChConfig_p - A pointer to the ulLogicalChannelInfo
 *                                      array containing the various
 *                                      configuration parameters for the
 *                                      logical channel.
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This API is used to add a logical channel as per the
 *                  configuration for a given UE in uplink UE context.
 ****************************************************************************/
MacRetType addULUELogicalChannel( ULUEContext* newUEContext_p, 
		UInt32 logicalChCount,
		ulLogicalChannelInfo* logicalChInfo_p
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA changes End */
		)
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	/* CA changes Start */
	InitParams *initParams_p = cellSpecificParams_g.\
				   cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
	/* CA Changes end */
	UInt32 tempLogicalChId = 0;
	UInt32 n     = 0;
	/* SPS_CHG */
	UInt8 qciIndex = 0;
	/* SPS_CHG */
	ULLogicalChannelInfo *lcInfo_p = PNULL;

	for (n = 0; n < logicalChCount; n++)
	{
		tempLogicalChId = logicalChInfo_p[n].lchId;
		if ( MAC_FAILURE != logicalChInfo_p[n].result)
		{
			lteWarning("Failure in DL LC addition, thus not adding in UL\n");

			continue;
		}   
		/*KLOCKWORK_CHANGES_START_161*/ 
		if ( (newUEContext_p->numLogicalChannels < MAX_NUMBER_OF_LOGICAL_CHANNEL) &&
				(tempLogicalChId < MAX_NUMBER_OF_LOGICAL_CHANNEL) )
			/*KLOCKWORK_CHANGES_STOP_161*/ 
		{
			lcInfo_p = &newUEContext_p->lcInfo[tempLogicalChId];
			lcInfo_p->lchId = logicalChInfo_p[n].lchId;
			lcInfo_p->lcGId = logicalChInfo_p[n].lcGId;
			/* SPR 3608 changes start */
			lcInfo_p->uePSRReqFlag = logicalChInfo_p[n].uePSRReqFlag;
			if(TRUE == logicalChInfo_p[n].uePSRReqFlag)
			{
				newUEContext_p->uePsrCount++;
			}
			/* SPR 3608 changes end */

			lcInfo_p->ulQosInfo.channelType = 
				logicalChInfo_p[n].channelType;

			/* Cyclomatic Complexity changes - starts here */
			if(MAC_SUCCESS != fillUlLcgInfo (lcInfo_p, newUEContext_p, &logicalChInfo_p[n]))
			{
				continue;
			}
			/* Cyclomatic Complexity changes - ends here */

			/* SPS_CHG */
			/*! \code
			 * If LC is with QCI enable 
			 *    - Then mark LC As SPS LC in UL UE Context
			 *\endcode
			 */
			if(lcInfo_p->ulQosInfo.QCI > 0)
			{		
				qciIndex = lcInfo_p->ulQosInfo.QCI  - 1;
				if ((TRUE == initParams_p->spsSysWideParam.enableSps) &&
						(TRUE == initParams_p->spsSysWideParam.qciSpslist[qciIndex]))
				{
					if (TRUE == lcInfo_p->isSpsLc)
					{
						LOG_MAC_MSG( MAC_MAX_SPS_LC_PER_UE, LOGWARNING, MAC_RRC_INF,
								GET_CURRENT_TICK_FOR_CELL(internalCellIndex), __LINE__, n, qciIndex + 1,
								newUEContext_p->ueIndex, internalCellIndex, 0,0, __func__, "" );
						continue;
					}
					else
					{
						/* Cyclomatic Complexity changes - starts here */
						if (MAC_SUCCESS != fillULSpsLcInfo (lcInfo_p, newUEContext_p, internalCellIndex))
						{
                            LOG_MAC_ERROR(MAC_RRC_INF,"fillULSpsLcInfo failed during fillULSpsLcInfo UE %u\n",
                                    newUEContext_p->ueIndex);
							return MAC_FAILURE;
						}
						/* Cyclomatic Complexity changes - ends here */
					}
				}
				else
				{
					lcInfo_p->isSpsLc = FALSE;
				}
			}
			/* SPS_CHG */

			if(QOS_GBR == lcInfo_p->ulQosInfo.channelType)
			{
				/*debug stats*/
				LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_GBR_COUNT(
						newUEContext_p->ueIndex);

				lcInfo_p->ulQosInfo.MBR = logicalChInfo_p[n].MBR;
				lcInfo_p->ulQosInfo.GBR = logicalChInfo_p[n].GBR;

				/* Cyclomatic Complexity changes - starts here */
				if( MAC_SUCCESS != fillULQosGbrInfo (lcInfo_p, newUEContext_p, internalCellIndex))
				{
					continue;
				}
				/* Cyclomatic Complexity changes - ends here */

			}
			else if(NON_GBR == lcInfo_p->ulQosInfo.channelType)
			{
				/*debug stats*/
				LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_NGBR_COUNT(
						newUEContext_p->ueIndex);

				/* Cyclomatic Complexity changes - starts here */
				fillULQosNonGbrInfo (lcInfo_p, newUEContext_p);
				/* Cyclomatic Complexity changes - ends here */
			}
			/*debug stats*/
			LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO(
					newUEContext_p->ueIndex,
					lcInfo_p->lchId,
					lcInfo_p->ulQosInfo.QCI,
					0,/*Mode*/
					lcInfo_p->ulQosInfo.GBR,
					lcInfo_p->ulQosInfo.tokenDepth,
					lcInfo_p->ulQosInfo.tokenRate);

			newUEContext_p->lcCount[logicalChInfo_p[n].lcGId] += 1;

			/* Updating the Logical Channel count */
			newUEContext_p->numLogicalChannels++;

			/* Cyclomatic Complexity changes - starts here */
			fillULQciDeltaPowerMaskInfo (lcInfo_p, newUEContext_p, internalCellIndex);
			/* Cyclomatic Complexity changes - ends here */

#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT)
			/* + SPR_5711 */         
			/*   if(UL_QOS_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p\
			     [internalCellIndex]->initParams_p->ulSchdConfig.ulSchedulerStrategy)
			     {*/
			newUEContext_p->maxConfiguredLcId  = 
				((newUEContext_p->maxConfiguredLcId > lcInfo_p->lchId) ?
				 newUEContext_p->maxConfiguredLcId:lcInfo_p->lchId);
			/* } */

			/* - SPR_5711 */         
#endif
			logicalChInfo_p[n].result = MAC_SUCCESS;        
		}
		else
		{
			LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR,"Unable to add the logical channel \
					(Faulty Configuration).\n");
			LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR,"The tempLogicalChId - %d",tempLogicalChId);
			logicalChInfo_p[n].result = MAC_MAX_ULLOG_CH_CONFIG;
		}
	}

	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);

	return MAC_SUCCESS;
}

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : fillULQosStrategyLcNode 
 * Inputs         : channelType - Indicates the LC channel type - QOS_GBR/SRB
 *                  lcInfo_p - Pointer to ULLogicalChannelInfo
 *                  newUEContext_p - Pointer to UL UE Context
 * Outputs        : None
 * Returns        : MacRetType
 * Description    : This function is used to fill UL QOS strategy LC Node.
 *****************************************************************************/
STATIC  MacRetType fillULQosStrategyLcNode (
		UInt8                channelType,
		ULLogicalChannelInfo *lcInfo_p,
		ULUEContext          *newUEContext_p
		)

{
	/*CA Changes start  */
	InternalCellIndex internalCellIndex;
	internalCellIndex = newUEContext_p->internalCellIndex;
	/*CA Changes end  */

	ULQosStrategyLCNode *ulQosStrategyLCNode_p = PNULL;
	/* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
	UInt8 index = 0;
#endif
	/* TDD Config 0 Changes End */
	/* SPR# 4287 Changes Starts */
	if(UL_QOS_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
			initParams_p->ulSchdConfig.ulSchedulerStrategy)
	{
		/* SPR# 4287 Changes Ends */
		GET_MEM_FROM_POOL(ULQosStrategyLCNode,
				ulQosStrategyLCNode_p,
				sizeof(ULQosStrategyLCNode),
				PNULL);
		/*+COVERITY 5.3.0 - 32731*/
		if(PNULL == ulQosStrategyLCNode_p)
		{
			ltePanic("Memory Allocation failed in %s\n",__func__);
			return MAC_FAILURE;
		}
		/*+COVERITY 5.3.0 - 32731*/

		/*FILLING ulQosStrategyLCNode_p PARAMS*/
		ulQosStrategyLCNode_p->lcgId = lcInfo_p->lcGId;
		ulQosStrategyLCNode_p->lchId = lcInfo_p->lchId;
		/* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
		for (index = 0; index < MAX_UL_SUBFRAME_GRANT; index++)
		{
			ulQosStrategyLCNode_p->scheduledBytes[index] = 0;
		}
#else
		ulQosStrategyLCNode_p->scheduledBytes   = 0;
#endif
		/* TDD Config 0 Changes End */
		ulQosStrategyLCNode_p->totLCWght        = 0;
		ulQosStrategyLCNode_p->channelType      = channelType;
		ulQosStrategyLCNode_p->ueId             = 
			newUEContext_p->ueIndex;
		ulQosStrategyLCNode_p->isLCScheduled = FALSE;
		/* SPR_3244_Fix */
		ulQosStrategyLCNode_p->isLCNodePresent = FALSE;
		/* SPR_3244_Fix */
		/*Storing the back reference pointer for LC Info in strategic 
		 * LC node*/
		ulQosStrategyLCNode_p->lcInfo_p      = lcInfo_p;
		ulQosStrategyLCNode_p->ulAmbrInfo_p  = PNULL;

		/*SPR 4218 Start*/
		ulQosStrategyLCNode_p->isPresentInEligibleList  = FALSE;
		/*SPR 4218 End*/

		/*STORE UL STRATERGY NODE IN LC INFO*/
		lcInfo_p->ulQosInfo.ulQosStrategyLCNode_p = ulQosStrategyLCNode_p;
	}
	return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : fillUlLcgInfo
 * Inputs         : lcInfo_p - Pointer to ULLogicalChannelInfo
 *                  newUEContext_p - Pointer to UL UE Context
 *                  logicalChInfo_p - Pointer to logical channel info
 * Outputs        : None
 * Returns        : MacRetType 
 * Description    : This function is used to fill UL logical channel group info
 ******************************************************************************/
STATIC  MacRetType fillUlLcgInfo (
		ULLogicalChannelInfo *lcInfo_p,
		ULUEContext          *newUEContext_p,
		ulLogicalChannelInfo *logicalChInfo_p
		)
{
	if (lcInfo_p->lchId == 1)
	{
		/*SRB1 or SRB2 is getting created */
		/* SPR 6430 changes start */
		newUEContext_p->ulLCGInfo[lcInfo_p->lcGId].srbWeight = 600;
		/* SPR 6430 changes start */

		/*CREATE STRATEGY LC NODE OF TYPE uLQosStrategyLcNode*/
		/* Cyclomatic Complexity changes - starts here */
		if( MAC_SUCCESS != fillULQosStrategyLcNode (SRB, lcInfo_p, newUEContext_p))
		{
			return MAC_FAILURE;
		}
		/* Cyclomatic Complexity changes - ends here */
	}
	/* SPR 7291 Changes Starts */
	else if(lcInfo_p->lchId == 2)
	{
		lcInfo_p->ulQosInfo.ulQosStrategyLCNode_p = PNULL;
	}    
	/* SPR 7291 Changes ends */

	/*Storing LC id in LCG info in UL UE ctx*/
	newUEContext_p->ulLCGInfo[lcInfo_p->lcGId].
		lcId[lcInfo_p->lchId] = 
		lcInfo_p->lchId;

	++newUEContext_p->ulLCGInfo[lcInfo_p->lcGId].numLC;

	lcInfo_p->ulQosInfo.logicalChannelPriority =
		logicalChInfo_p->logicalChannelPriority;
	lcInfo_p->ulQosInfo.QCI = logicalChInfo_p->QCI;

	if (0 < lcInfo_p->ulQosInfo.QCI && 5 > lcInfo_p->ulQosInfo.QCI)
	{
		lcInfo_p->ulQosInfo.channelType = QOS_GBR;
		newUEContext_p->ulLCGInfo[lcInfo_p->lcGId].srbWeight = 0;
		newUEContext_p->ulLCGInfo[lcInfo_p->lcGId].isDrbPresent = TRUE;
	}
	else if(lcInfo_p->ulQosInfo.QCI > 4)
	{
		lcInfo_p->ulQosInfo.channelType = NON_GBR;
		newUEContext_p->ulLCGInfo[lcInfo_p->lcGId].srbWeight = 0;
		newUEContext_p->ulLCGInfo[lcInfo_p->lcGId].isDrbPresent = TRUE;
	}
	else
	{
		lcInfo_p->ulQosInfo.channelType = SRB;
	}
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : fillULSpsLcInfo 
 * Inputs         : lcInfo_p - Pointer to ULLogicalChannelInfo
 *                  newUEContext_p - Pointer to UL UE Context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS] 
 * Description    : This function is used to fill UL SPS LC information.
 *****************************************************************************/
STATIC  MacRetType fillULSpsLcInfo (
		ULLogicalChannelInfo *lcInfo_p,
		ULUEContext          *newUEContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex
		/*CA Changes end  */
		)
{
	lcInfo_p->isSpsLc = TRUE;
    /* +- SPR 17777 */
	/* Store SPS LC Id */
	newUEContext_p->spsUlInfo.lcId = lcInfo_p->lchId;
	/* Call State m/c for changing the SPS state after receving sps LC */
	if (newUEContext_p->spsUlInfo.spsState == SPS_IDLE &&
			newUEContext_p->spsUlInfo.requestType == SPS_SETUP)
	{
        /* +- SPR 17777 */
		/* Call state machine for setting state to SPS_CONFIGURED  */
		if( MAC_FAILURE == ulStateMachine[newUEContext_p->spsUlInfo.\
				spsState][SPS_CONFIG_RECVD]( 
					newUEContext_p->ueIndex, 
					newUEContext_p->spsUlInfo.spsState,
					SPS_CONFIG_RECVD, newUEContext_p, PNULL 
					/* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
					,PHY_DL_DELAY
#else
					,DL_ADVANCE_TICK
#endif
                    /* - SPS_TDD_Changes */ 
                    /*CA Changes start  */
                    , internalCellIndex ) )
			/*CA Changes end  */
		{
            /* +- SPR 17777 */
            LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, newUEContext_p->ueIndex,newUEContext_p->spsUlInfo.spsState, SPS_CONFIG_RECVD,
                     newUEContext_p->spsUlInfo.spsState, 0,0, __func__, "UlStateMachine");
			return MAC_FAILURE;
		}
        LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, newUEContext_p->ueIndex,newUEContext_p->spsUlInfo.spsState, SPS_CONFIG_RECVD,
                     newUEContext_p->spsUlInfo.spsState, 0,0, __func__, "UlStateMachine");
        /* +- SPR 17777 */
	} 
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : fillULQosGbrInfo
 * Inputs         : lcInfo_p - Pointer to ULLogicalChannelInfo
 *                  newUEContext_p - Pointer to UL UE Context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MacRetType 
 * Description    : This function is used to fill UL Qos GBR information
 *****************************************************************************/
STATIC  MacRetType fillULQosGbrInfo ( 
		ULLogicalChannelInfo *lcInfo_p,
		ULUEContext          *newUEContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex
		/*CA Changes end  */
		)
{
	/* SPR 11115 Fix Start */
#ifdef FDD_CONFIG
	UInt16 numOfTicksPerSecond = NUM_TICKS_PER_SECOND;
#else
	UInt16 numOfTicksPerSecond = getNumTicksPerSecond(UL, internalCellIndex);
#endif
	UDouble32 absFactor = 0;
	UInt8 absCount = 0;

	/* SPR 11115 Fix End */
	/* DESIGN REVIEW CHANGES INCORPORATION START */
	/*CA Changes start  */
	InitParams *initParams_p = cellSpecificParams_g.\
				   cellConfigAndInitParams_p[internalCellIndex]->initParams_p;


	CALCULATE_ABS_FACTOR(internalCellIndex, absCount, absFactor,newUEContext_p)

		ulAggregateGbrPerTTI_g[internalCellIndex][newUEContext_p->userType] += (lcInfo_p->ulQosInfo.GBR / 
				/*CA Changes end  */
				numOfTicksPerSecond) * absFactor;
	/* DESIGN REVIEW CHANGES INCORPORATION END */
	/* EICIC -*/

	/*UPDATE THE STKN(T) WITH THE TOKEN RATE*/
	lcInfo_p->ulQosInfo.scheduledToken[0] = 0;
	lcInfo_p->ulQosInfo.scheduledToken[1] = 0;
	lcInfo_p->ulQosInfo.scheduledToken[2] = 0;
	lcInfo_p->ulQosInfo.scheduledToken[3] = 0;
	lcInfo_p->ulQosInfo.scheduledToken[4] = 0;
	lcInfo_p->ulQosInfo.scheduledToken[5] = 0;
	lcInfo_p->ulQosInfo.scheduledToken[6] = 0;
	lcInfo_p->ulQosInfo.scheduledToken[7] = 0;
	lcInfo_p->ulQosInfo.scheduledToken[8] = 0;
	lcInfo_p->ulQosInfo.scheduledToken[9] = 0;

	lcInfo_p->ulQosInfo.strictSchedulingFlag = FALSE;
	lcInfo_p->ulQosInfo.totSchdTknInWindow   = 0;
	lcInfo_p->ulQosInfo.curTknWindowInd      = 0;
	lcInfo_p->ulQosInfo.pdbPriority          = 0;


	/* Update Priority of Logical Channel */
	lcInfo_p->ulQosInfo.priority = 
		macParams_g.qosQciInfo[lcInfo_p->ulQosInfo.QCI - 1].priority;

	/*GET QCI PRIORITY*/
	lcInfo_p->ulQosInfo.qciPriority =
		ulQosGetQciPriority(lcInfo_p->ulQosInfo.priority);

	/* UPDATE PDB (QCI params) IN LC INFO*/

	lcInfo_p->ulQosInfo.pktDlyBdgt = 
		macParams_g.qosQciInfo[lcInfo_p->ulQosInfo.QCI - 1].pktDlyBdgt;

	lcInfo_p->ulQosInfo.pktDlyBdgt = 
		lcInfo_p->ulQosInfo.pktDlyBdgt - 25;

	lcInfo_p->ulQosInfo.pktErrRate =
		macParams_g.qosQciInfo[lcInfo_p->ulQosInfo.QCI - 1].pktErrLossRate;

	/*CREATE STRATEGY LC NODE OF TYPE uLQosStrategyLcNode*/
	/* Cyclomatic Complexity changes - starts here */
	if(MAC_SUCCESS != fillULQosStrategyLcNode (QOS_GBR, lcInfo_p, newUEContext_p))
	{
		return MAC_FAILURE;
	}
	/* Cyclomatic Complexity changes - ends here */

	ulQosUpdateQloadTableForLcPriority(
			lcInfo_p->ulQosInfo.lcQlTable,
			lcInfo_p->ulQosInfo.GBR, 
			lcInfo_p->ulQosInfo.pktDlyBdgt);

	ulQosUpdatePdbTableForLcPriority(
			lcInfo_p->ulQosInfo.lcPdbTable,
			lcInfo_p->ulQosInfo.pktDlyBdgt);

	ulQosUpdateTknTableForLcPriority(
			lcInfo_p->ulQosInfo.lcTknTable,
			lcInfo_p->ulQosInfo.GBR,
			internalCellIndex);

	/*CALCULATE GBR LC TOKEN VALUE*/
	ulQosCalcGbrLCTknValue(&lcInfo_p->ulQosInfo, internalCellIndex);

	/*CALCULATE GBR BUCKET DEPTH*/
	lcInfo_p->ulQosInfo.tokenDepth = lcInfo_p->ulQosInfo.tokenRate * 
		initParams_p->ulSchdConfig.gbrBktFactor;

	/*INITIALLY TOKEN VALUE WILL BE EQUAL TO TOKEN RATE*/
	lcInfo_p->ulQosInfo.availLcToken = 
		lcInfo_p->ulQosInfo.tokenRate ;

	/* DESIGN REVIEW CHANGES INCORPORATION */
	/* Calculate bucket limit(minimum tokens to present in bucket) */
	lcInfo_p->ulQosInfo.tokenLimit = 
		initParams_p->ulSchdConfig.gbrTokenLimitFactor * 
		lcInfo_p->ulQosInfo.tokenRate;
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : fillULQosNonGbrInfo 
 * Inputs         : lcInfo_p - Pointer to ULLogicalChannelInfo
 *                  newUEContext_p - Pointer to UL UE Context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None 
 * Description    : This function is used to fill UL Qos Non-GBR information
 *****************************************************************************/
/* CA Changes start */
STATIC  void fillULQosNonGbrInfo ( 
		ULLogicalChannelInfo *lcInfo_p,
		ULUEContext          *newUEContext_p
		)
{
	/* UPDATE PDB (QCI params) IN LC INFO*/
	/* SPR 9203 Fix Starts*/
	lcInfo_p->ulQosInfo.GBR = 0; 
	/* SPR 9203 Fix Ends*/
	lcInfo_p->ulQosInfo.pktDlyBdgt = 
		macParams_g.qosQciInfo[lcInfo_p->ulQosInfo.QCI - 1].pktDlyBdgt;

	lcInfo_p->ulQosInfo.pktDlyBdgt = 
		lcInfo_p->ulQosInfo.pktDlyBdgt - 25;

	lcInfo_p->ulQosInfo.pktErrRate =
		macParams_g.qosQciInfo[lcInfo_p->ulQosInfo.QCI - 1].pktErrLossRate;

	/* CA Changes end */
	/*Calculate minPdb*/
	newUEContext_p->ulAmbrInfo.minPdb = 
		(newUEContext_p->ulAmbrInfo.minPdb < 
		 lcInfo_p->ulQosInfo.pktDlyBdgt)   ?
		newUEContext_p->ulAmbrInfo.minPdb :
		lcInfo_p->ulQosInfo.pktDlyBdgt ;

	/*Calculate maxPdb*/
	newUEContext_p->ulAmbrInfo.maxPdb = 
		(newUEContext_p->ulAmbrInfo.maxPdb > 
		 lcInfo_p->ulQosInfo.pktDlyBdgt)   ?
		newUEContext_p->ulAmbrInfo.maxPdb :
		lcInfo_p->ulQosInfo.pktDlyBdgt ;

	/*Calculate minQCI*/

	if (newUEContext_p->ulAmbrInfo.minQci >= 
			lcInfo_p->ulQosInfo.QCI)         
	{
		newUEContext_p->ulAmbrInfo.minQci = 
			lcInfo_p->ulQosInfo.QCI ;

		newUEContext_p->ulAmbrInfo.lcId = lcInfo_p->lchId;    
		newUEContext_p->ulAmbrInfo.lcgId = lcInfo_p->lcGId;    
		/*SPR 7412 Fix start */
		if( newUEContext_p->ulAmbrInfo.ulQosStrategyLCNode_p)
		{
			newUEContext_p->ulAmbrInfo.ulQosStrategyLCNode_p->lchId   =
				lcInfo_p->lchId;
		}
		/*SPR 7412 Fix End */

	}
}

/****************************************************************************
 * Function Name  : fillULQciDeltaPowerMaskInfo
 * Inputs         : lcInfo_p - Pointer to ULLogicalChannelInfo
 *                  newUEContext_p - Pointer to UL UE Context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None 
 * Description    : This function is used to fill UL Qci delta power mask info
 *****************************************************************************/
STATIC  void fillULQciDeltaPowerMaskInfo (
		ULLogicalChannelInfo *lcInfo_p,
		ULUEContext          *newUEContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex
		/*CA Changes end  */
		)
{
	/* +DYNAMIC_ICIC */
	UInt8 qciIndex = 0;
	ULPowerMaskInfo *ulPowerMaskInfo_p = PNULL;
	SInt32 qciDeltaPowerMaskToAdd = INVALID_UL_POWER_MASK;
	SInt32 maxQciDeltaPowerMask = INVALID_UL_POWER_MASK;
	UInt8 numOfQciInfo = 0;
	UInt8 lcCounter = 0;
	UInt8 lcGId = LCGID_INVALID_VALUE;
	UInt8 existingLcgId = 0;
	UInt8 firstEntry = 0;
	UInt8 loopCount = 0;
	UInt8 loopCountInner = 0;
	SInt32 currentQciPowerMaskVal = 0;
	UInt8 currentlcGIdVal = 0; 

	/*CA Changes start  */
	ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + ulPowerMaskIndex_g[internalCellIndex];
	/*CA Changes end  */
	if( SRB != lcInfo_p->ulQosInfo.channelType)
	{
		qciIndex = lcInfo_p->ulQosInfo.QCI - 1;
		qciDeltaPowerMaskToAdd = ulPowerMaskInfo_p->qciDeltaPowerMask[qciIndex];
		numOfQciInfo = newUEContext_p->qciDeltaPowerMaskInfo.numOfQciInfo ;

		if ( !numOfQciInfo )
		{
			newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[0].lcgId = lcInfo_p->lcGId;
			newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[0].maxQciDeltaPowerMask = qciDeltaPowerMaskToAdd;
			newUEContext_p->qciDeltaPowerMaskInfo.numOfQciInfo++;
			firstEntry = 1;
		}
		else
		{
			for ( lcCounter = 0;lcCounter<numOfQciInfo; lcCounter++)
			{
				lcGId = newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[lcCounter].lcgId;
				maxQciDeltaPowerMask = newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[lcCounter].maxQciDeltaPowerMask;
				if(lcInfo_p->lcGId == lcGId)
				{
					if (maxQciDeltaPowerMask < qciDeltaPowerMaskToAdd)
					{
						newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[lcCounter].maxQciDeltaPowerMask =
							qciDeltaPowerMaskToAdd;
					}
					existingLcgId = 1;
					break;
				}
			}
		}
		if ( !existingLcgId && !firstEntry)
		{
			newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[numOfQciInfo].lcgId = lcInfo_p->lcGId;
			newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[numOfQciInfo].maxQciDeltaPowerMask = qciDeltaPowerMaskToAdd;
			newUEContext_p->qciDeltaPowerMaskInfo.numOfQciInfo++;
		}
		if ( newUEContext_p->qciDeltaPowerMaskInfo.numOfQciInfo > 1 )
		{
			for ( loopCount = 1;
					loopCount < newUEContext_p->qciDeltaPowerMaskInfo.numOfQciInfo;
					loopCount++)
			{

				currentQciPowerMaskVal = 
					newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCount].maxQciDeltaPowerMask ;

				currentlcGIdVal =  
					newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCount].lcgId;

				for (loopCountInner = loopCount;
						loopCountInner > 0 && currentQciPowerMaskVal < newUEContext_p->qciDeltaPowerMaskInfo.
						deltaPowerMaskInfo[loopCountInner-1].maxQciDeltaPowerMask;
						loopCountInner--)
				{
					newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCountInner].maxQciDeltaPowerMask =
						newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCountInner-1].maxQciDeltaPowerMask;
					newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCountInner].lcgId =
						newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCountInner-1].lcgId;
				}
				newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCountInner].maxQciDeltaPowerMask = currentQciPowerMaskVal;
				newUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCountInner].lcgId = currentlcGIdVal;
			}
		}
	}
	/* -DYNAMIC_ICIC */
}
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : reconfigULUELogicalChannel
 * Inputs         : ueContext_p - Pointer to UE context for which the 
 *                         logical channel needs to be reconfigured.
 *                  logicalChInfo_p - A pointer to the ulLogicalChannelInfo
 *                                      array containing the various
 *                                      configuration parameters for the
 *                                      logical channel.
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This API is used to reconfigure a logical channel as
 *                  per the configuration for a given UE.
 *                  If the logical channel does not exist, then return 
 *                  MAC_FAILURE.
 ****************************************************************************/

MacRetType reconfigULUELogicalChannel( ULUEContext *ueContext_p,
		ulLogicalChannelInfo* logicalChInfo_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex)
/*CA Changes end  */

{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	ULLogicalChannelInfo *lcInfo_p = PNULL;
	/* Temporary variable for Logical Channel Id to be reconfigured */
	UInt32 tempLogicalChId = 0;
	tempLogicalChId = logicalChInfo_p->lchId;
	/* SPR 11115 Fix Start */
#ifdef FDD_CONFIG
	UInt16 numOfTicksPerSecond = NUM_TICKS_PER_SECOND;
#else
	UInt16 numOfTicksPerSecond = getNumTicksPerSecond(UL, internalCellIndex);
#endif
	UDouble32 absFactor = 0;
	UInt8 absCount = 0;
	/* SPR 11115 Fix End */

	CALCULATE_ABS_FACTOR(internalCellIndex, absCount, absFactor,ueContext_p)

		/* Adding the new Logical Channel in the UE Context */
		if (LCGID_INVALID_VALUE == ueContext_p->lcInfo[tempLogicalChId].lcGId)
		{
			lteWarning ("The Logical Channel requested to be reconfigured \
					has not been added yet. \n");
			logicalChInfo_p->result = MAC_RRC_LOG_CH_NOT_EXISTS;        
			return MAC_FAILURE;
		}

	/* Reconfigure Lc changes for Qos */
	lcInfo_p = &ueContext_p->lcInfo[tempLogicalChId];

	if (QOS_GBR == lcInfo_p->ulQosInfo.channelType)
	{
		/* Coverity 59948 + SPR 9203 Fix Starts*/
		if( ulAggregateGbrPerTTI_g[internalCellIndex][ueContext_p->userType]< 
				/* SPR 11248 Fix Start */
				lcInfo_p->ulQosInfo.GBR / 
				/* SPR 11248 Fix End */
				/* SPR 11115 Fix Start */
				numOfTicksPerSecond )
			/* SPR 11115 Fix End */
		{
			ulAggregateGbrPerTTI_g[internalCellIndex][ueContext_p->userType]= 0;
		}
		else
		{
			/*CA Changes start  */
			ulAggregateGbrPerTTI_g[internalCellIndex][ueContext_p->userType] -= (lcInfo_p->ulQosInfo.GBR / 
					/*CA Changes end  */
					/* SPR 11115 Fix Start */
					numOfTicksPerSecond) * absFactor;
			/* SPR 11115 Fix End */

		}
		/* Coverity 59948 + SPR 9203 Fix Ends*/

		lcInfo_p->ulQosInfo.GBR = logicalChInfo_p->GBR;
		lcInfo_p->ulQosInfo.MBR = logicalChInfo_p->MBR;

		ulQosUpdateQloadTableForLcPriority(
				lcInfo_p->ulQosInfo.lcQlTable,
				lcInfo_p->ulQosInfo.GBR, 
				lcInfo_p->ulQosInfo.pktDlyBdgt);

		ulQosUpdateTknTableForLcPriority(
				lcInfo_p->ulQosInfo.lcTknTable,
				lcInfo_p->ulQosInfo.GBR, internalCellIndex);

		/*CALCULATE GBR LC TOKEN VALUE*/
		ulQosCalcGbrLCTknValue(&lcInfo_p->ulQosInfo, internalCellIndex);

		/*CALCULATE GBR BUCKET DEPTH*/
		lcInfo_p->ulQosInfo.tokenDepth = lcInfo_p->ulQosInfo.tokenRate * 
			/*CA Changes start  */
			cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
			cellParams_p->ulSchdConfig.gbrBktFactor;
		/*CA Changes end  */

		/* DESIGN REVIEW CHANGES INCORPORATION */
		/* Calculate bucket limit(minimum tokens to present in bucket) */
		lcInfo_p->ulQosInfo.tokenLimit = 
			/*CA Changes start  */
			cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
			cellParams_p->ulSchdConfig.gbrTokenLimitFactor * 
			/*CA Changes end  */
			lcInfo_p->ulQosInfo.tokenRate;

		lcInfo_p->ulQosInfo.availLcToken =
			lcInfo_p->ulQosInfo.tokenRate ;

		/* Reconfigure Lc changes for Qos */

		/* DESIGN REVIEW CHANGES INCORPORATION START */
		/*CA Changes start  */
		ulAggregateGbrPerTTI_g[internalCellIndex][ueContext_p->userType]+= ( lcInfo_p->ulQosInfo.GBR / 
				/*CA Changes end  */
				/* SPR 11115 Fix Start */
				numOfTicksPerSecond) * absFactor;
		/* SPR 11115 Fix End */

	}

	/* DESIGN REVIEW CHANGES INCORPORATION END */
	logicalChInfo_p->result = MAC_SUCCESS;        
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : deleteDLUELogicalChannel
 * Inputs         : ueContext_p - Pointer to UE context from which LC
 *                           needs to be deleted.
 *                  ueLCId - The Logical Channel ID that needs to be deleted.
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This API is used to delete a logical channel as per the
 *                  request for a given UE.
 *****************************************************************************/
/* CA Changes start */  
 MacRetType deleteDLUELogicalChannel( DLUEContext *ueContext_p,
		UInt8 ueLCId,
		InternalCellIndex   internalCellIndex
		)
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);

	/*SPR 3674 changes start*/
#ifdef KPI_STATS
	UInt8 qci = 0;
    /* SPR23194 Start */
    UInt16 deleteUEIndex = MAX_UE_SUPPORTED;
    UInt32 activeIdxForThpStats = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForMacThpStats;
    LteMacKPIThpStats * pLteMacKPIThpStats = &(gMacStats.pStats->
            lteCommonUeKPIStats.lteMacKPIThpStats[activeIdxForThpStats]);
    LteMacKPIThpStats *pMacOamKPIThpStats = 
        &(g_kpiScannerObj.kpiStatsContainer[KPI_CONTAINER_THP_OAM_MAC].stats.lteMacKPIThpStats);
    /* SPR23194 End */
#endif
	/*SPR 3674 changes end */
	DLLogicalChannelInfo *dlLogicalChInfo_p     = PNULL;
	SInt32 index = 0;
	UInt8 gbrLcPos = 0;
	UInt8 nonGbrLcPos = 0;
	UInt8 gbrLcCount = ueContext_p->gbrLcCount;
	UInt8 nonGbrLcCount = ueContext_p->nonGbrLcCount;
	dlLogicalChInfo_p = &ueContext_p->logicalChannel[ueLCId];
	DLQosStrategyLCNode  *dlQosStrategyLCNode_p = PNULL;
	UInt8 scellId = 1;
	/* SPR 11115 Fix Start */
#ifdef FDD_CONFIG
	UInt16 numOfTicksPerSecond = NUM_TICKS_PER_SECOND;
#else
	UInt16 numOfTicksPerSecond = getNumTicksPerSecond(DL, internalCellIndex);
#endif
	/* SPR 11115 Fix End */
    /* +- SPR 17777 */
	/* check if max LC ID is equal to delete LC ID */
	/* SPR# 3448 Changes Ends */
	if (dlLogicalChInfo_p->logicalChannelId != INVALID_LCID)
	{
		if ( dlLogicalChInfo_p->logicalChannelId == ueContext_p->maxLcId)
		{
			index = ueContext_p->maxLcId - 1;
			/* identify the next max. Lc Id
			 * after deleting this LCID. */

			/* SPR#706 Changes Start */
			if(index >= MAX_NUMBER_OF_LOGICAL_CHANNEL)
			{
				lteWarning("[%s]: maxLcId is %d. It is greater then MAX limit\n",__func__, index );
				/*SPR 2235 Fix Begin */
				index = MAX_NUMBER_OF_LOGICAL_CHANNEL - 1; 
				/* SPR 2235 Fix End */
			}
			/* SPR#706 Changes End */

			/* SPR 1768 changes start */
			while ( (index >= 0) && INVALID_LCID == ueContext_p->
					logicalChannel[index].logicalChannelId )
				/* SPR 1768 changes end */
			{
				index--;
			}
			if ( index < 0 )
			{
				ueContext_p->maxLcId = INVALID_LCID;
			}
			else
			{
				ueContext_p->maxLcId = index;
			} 
		}
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        qci =  dlLogicalChInfo_p->dlLcQosInfo.QCI;
        /* SPR23194 Start */
        /* Updating the THP KPI counters for the deleted UE */

        if(ueLCId > 2)
        {
            pLteMacKPIThpStats->scheduledIPThpVolDL[deleteUEIndex][ueLCId - 3] +=
                pLteMacKPIThpStats->scheduledIPThpVolDL[ueContext_p->ueIndex][ueLCId - 3];
            pLteMacKPIThpStats->scheduledIPThpVolDL[deleteUEIndex][ueLCId - 3] +=
                pMacOamKPIThpStats->scheduledIPThpVolDL[ueContext_p->ueIndex][ueLCId - 3];
    
            pLteMacKPIThpStats->scheduledIPThpVolUL[deleteUEIndex][ueLCId - 3] +=
                pLteMacKPIThpStats->scheduledIPThpVolUL[ueContext_p->ueIndex][ueLCId - 3];
            pLteMacKPIThpStats->scheduledIPThpVolUL[deleteUEIndex][ueLCId - 3] +=
                pMacOamKPIThpStats->scheduledIPThpVolUL[ueContext_p->ueIndex][ueLCId - 3];
            
            pLteMacKPIThpStats->scheduledIPThpTimeDL[deleteUEIndex][ueLCId - 3] +=
                pLteMacKPIThpStats->scheduledIPThpTimeDL[ueContext_p->ueIndex][ueLCId - 3];
            pLteMacKPIThpStats->scheduledIPThpTimeDL[deleteUEIndex][ueLCId - 3] +=
                pMacOamKPIThpStats->scheduledIPThpTimeDL[ueContext_p->ueIndex][ueLCId - 3];
            
            pLteMacKPIThpStats->scheduledIPThpTimeUL[deleteUEIndex][ueLCId - 3] +=
                pLteMacKPIThpStats->scheduledIPThpTimeUL[ueContext_p->ueIndex][ueLCId - 3];
            pLteMacKPIThpStats->scheduledIPThpTimeUL[deleteUEIndex][ueLCId - 3] +=
                pMacOamKPIThpStats->scheduledIPThpTimeUL[ueContext_p->ueIndex][ueLCId - 3];
            
            pLteMacKPIThpStats->sharedMemoryLatSample[deleteUEIndex][ueLCId - 3] +=
                pLteMacKPIThpStats->sharedMemoryLatSample[ueContext_p->ueIndex][ueLCId - 3];
            pLteMacKPIThpStats->sharedMemoryLatSample[deleteUEIndex][ueLCId - 3] +=
                pMacOamKPIThpStats->sharedMemoryLatSample[ueContext_p->ueIndex][ueLCId - 3];
            
            pLteMacKPIThpStats->SharedMemoryLatTime[deleteUEIndex][ueLCId - 3] +=
                pLteMacKPIThpStats->SharedMemoryLatTime[ueContext_p->ueIndex][ueLCId - 3];
            pLteMacKPIThpStats->SharedMemoryLatTime[deleteUEIndex][ueLCId - 3] +=
                pMacOamKPIThpStats->SharedMemoryLatTime[ueContext_p->ueIndex][ueLCId - 3];

            /* Updating the mapping index of kpiLcToQciMapping_g as the qci value 
             * will be required while sending KPI reports */
            kpiLcToQciMapping_g[ueLCId - 3] = qci;
            /* Reset the kpi_scanner counters for ueIndex which is getting released */
            pMacOamKPIThpStats->scheduledIPThpVolDL[ueContext_p->ueIndex][ueLCId - 3] = 0;
            pMacOamKPIThpStats->scheduledIPThpVolUL[ueContext_p->ueIndex][ueLCId - 3] = 0;
            pMacOamKPIThpStats->scheduledIPThpTimeDL[ueContext_p->ueIndex][ueLCId - 3] = 0;
            pMacOamKPIThpStats->scheduledIPThpTimeUL[ueContext_p->ueIndex][ueLCId - 3] = 0;
            pMacOamKPIThpStats->sharedMemoryLatSample[ueContext_p->ueIndex][ueLCId - 3] = 0;
            pMacOamKPIThpStats->SharedMemoryLatTime[ueContext_p->ueIndex][ueLCId - 3] = 0;

        }
        /* SPR23194 End */
        if ( ( (qci) && (qci <= MAX_QCI)) &&
                ( macIsQciUniqueForDlLc(ueLCId,qci,ueContext_p->ueIndex)) &&
                ( ueContext_p->qciInfoFlag[--qci] ) )
        {
            if ( gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci] )
            {
                gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci]--;
		    /* SPR 23402 Changes Start */
		    /* MOCN Changes Start */
		    /*MOCN SPR21942 Fix Start*/
		    if(mocnOperatorParams_g.isMocnEnabled && ueContext_p->operatorId <  mocnOperatorParams_g.numOfOperator)
		    {
			    if(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciDL[ueContext_p->operatorId][qci]) 
			    {
				    gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciDL[ueContext_p->operatorId][qci]--; 
			    }
		    }
		    /*MOCN SPR21942 Fix End*/
		    /* MOCN Changes End */
		    /* SPR 23402 Changes End */
            }
            resetZeroQloadFlag(ueContext_p->ueIndex,dlLogicalChInfo_p->dlLcQosInfo.QCI);
            ueContext_p->qciInfoFlag[qci] = 0;
        }
#endif
        /* SPR 16916 FIX END */
		/* Fix for SPR 4287 start */
	{
		if(DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p\
				[internalCellIndex]->initParams_p->dlSchdConfig.dlSchedulerStrategy)
		{
			/* Fix for SPR 4287 End */
            /* +- SPR 16320 */
            /*+- SPR 17669*/
            /* SPR 16320 fix start */
            dlQosStrategyLCNode_p = dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p;
            if(dlQosStrategyLCNode_p->isPresentInEligibleList)
            {
                /* SPR 16320 fix end */
				listDeleteNode(&ueContext_p->dlEligibleLcList, &dlQosStrategyLCNode_p->eligibleLcAnchor);

				/* SPR 12115 Changes Start */
				/*CA Changes start  */
				dlQosDeleteEntryFromPrioritisedLcTree(dlQosStrategyLCNode_p, internalCellIndex,getCurrentTick());
				/*CA Changes end  */
				/* SPR 12115 Changes End */
			}
			if(IS_CA_ENABLED())
			{
				for (scellId = MAX_NUM_PCELL; scellId <= MAX_NUM_SCELL; scellId++)
				{
					freeMemPool(dlQosStrategyLCNode_p->dLUESCellStartegyNode_p[scellId]);
					dlQosStrategyLCNode_p->dLUESCellStartegyNode_p[scellId] = PNULL;
				}
			}
			freeMemPool(dlQosStrategyLCNode_p);
			/* Fix for SPR 4287 Start */
		}
		/* Fix for SPR 4287 End */
		while (gbrLcPos < gbrLcCount)
		{
			if (ueContext_p->gbrLcInfo[gbrLcPos] != ueLCId)
			{
				gbrLcPos++;
			}
			else
			{
				while (gbrLcPos < gbrLcCount)
				{
					ueContext_p->gbrLcInfo[gbrLcPos] = ueContext_p->gbrLcInfo[gbrLcPos + 1];
					gbrLcPos++;
				}
				(ueContext_p->gbrLcCount)--;
				gbrLcCount--;
				LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_GBR_COUNT(ueContext_p->ueIndex);
				break;
			}
		}

		while (nonGbrLcPos < nonGbrLcCount)
		{
			if (ueContext_p->nonGbrLcInfo[nonGbrLcPos] != ueLCId)
			{
				nonGbrLcPos++;
			}
			else
			{
				while (nonGbrLcPos < nonGbrLcCount)
				{
					ueContext_p->nonGbrLcInfo[nonGbrLcPos] = ueContext_p->nonGbrLcInfo[nonGbrLcPos + 1];
					nonGbrLcPos++;
				}
				(ueContext_p->nonGbrLcCount)--;
				nonGbrLcCount--;
				LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_NGBR_COUNT(ueContext_p->ueIndex);
				break;
			}
		}

		/* Rest SPS Info incase of SPS LC Deletion */
		if (TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
				->initParams_p->spsSysWideParam.enableSps && 
				ueContext_p->spsDlInfo.lcId == dlLogicalChInfo_p->logicalChannelId)
		{
			if (dlUECtxInfoArr_g[ueContext_p->ueIndex].pendingDeleteFlag != TRUE)
			{
                /* +- SPR 17777 */
				/* Call state machine for setting state to SPS_IDLE  */
				if( MAC_FAILURE == dlStateMachine[ueContext_p->spsDlInfo.\
						spsState][SPS_CONFIG_RECVD]( 
							ueContext_p->ueIndex, 
							ueContext_p->spsDlInfo.spsState,
							SPS_CONFIG_RECVD, ueContext_p, PNULL, 
							 /* + SPS_TDD_Changes*/
#ifdef FDD_CONFIG
							PHY_DL_DELAY ,
#else
							DL_ADVANCE_TICK,
#endif
                            /* - SPS_TDD_Changes */
							internalCellIndex) )
				{
                    /* +- SPR 17777 */
                    LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                            getCurrentTick(), __LINE__, ueContext_p->ueIndex,ueContext_p->spsDlInfo.spsState, SPS_CONFIG_RECVD,
                            ueContext_p->spsDlInfo.spsState, 0,0, __func__, "DlStateMachine");
					return MAC_FAILURE;
				}
                LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, ueContext_p->ueIndex,ueContext_p->spsDlInfo.spsState, SPS_CONFIG_RECVD,
                     ueContext_p->spsDlInfo.spsState, 0,0, __func__, "DlStateMachine");
                    /* +- SPR 17777 */
			}
			/* Reset parameters */
			ueContext_p->spsDlInfo.lcId = INVALID_LCID;
			dlLogicalChInfo_p->isSpsLc = FALSE;
		}

		dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p = PNULL;


		/* SPR 9203 Fix Starts*/
		if (QOS_GBR == dlLogicalChInfo_p->dlLcQosInfo.channelType)
		{
			/* DESIGN REVIEW CHANGES INCORPORATION START */
			updateDlAggregateGbrPerTTI(ueContext_p, &(dlLogicalChInfo_p->dlLcQosInfo),DELETE_DL_AGG_GBR_TRIGGER, numOfTicksPerSecond);

			/* DESIGN REVIEW CHANGES INCORPORATION END */
		}
		/* SPR 9203 Fix Ends*/
	}//LTE_CELL

		dlLogicalChInfo_p->logicalChannelId  = INVALID_LCID;
		/* SPR 4870 fix start*/
		/* queueLoad will be checked before sending response to RRC and
		   is initialized at the time of LC creation */
		/* dlLogicalChInfo_p->queueLoad         = 0;*/
		/* SPR 4870 fix end*/
		dlLogicalChInfo_p->lastScheduledTick = 0;
		dlLogicalChInfo_p->rlcMode           = INVALID_RLC_MODE;
		dlLogicalChInfo_p->rlcSNFieldLength  = 0;
		dlLogicalChInfo_p->logicalChannelPriority = 0;

		/* SPR 3608 changes start */
		if(TRUE == dlLogicalChInfo_p->enbPSRReqFlag)
		{
			dlLogicalChInfo_p->enbPSRReqFlag = FALSE;
			if(0 < ueContext_p->enbPsrCount)
				ueContext_p->enbPsrCount--;
		}
		/* SPR 3608 changes end */

		LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_IS_NOT_VALID(ueContext_p->ueIndex, ueLCId);


	}
	/* SPR# 3448 Changes Ends */
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
	return MAC_SUCCESS;
}
/* CA Changes end */  

/****************************************************************************
 * Function Name  : deleteULUELogicalChannel
 * Inputs         : ueContext_p - Pointer to UE context from which LC
 *                           needs to be deleted.
 *                  ueLCId - The Logical Channel ID that needs to be deleted.
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This API is used to delete a logical channel as per the
 *                  request for a given UE.
 *****************************************************************************/
 MacRetType deleteULUELogicalChannel( ULUEContext *ueContext_p,
		UInt8 ueLCId,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex
		/*CA Changes end  */
		)
{
	/*SPR 3674 changes start */
#ifdef KPI_STATS
	UInt8 qci = 0;
#endif
	/*SPR 3674 changes end */
	/* SPR 11115 Fix Start */
#ifdef FDD_CONFIG
	UInt16 numOfTicksPerSecond = NUM_TICKS_PER_SECOND;
#else
	UInt16 numOfTicksPerSecond = getNumTicksPerSecond(UL, internalCellIndex);
#endif
	UDouble32 absFactor = 0;
	UInt8 absCount = 0;
    /* +- SPR 17777 */
	/* SPR 11115 Fix End */
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);

	CALCULATE_ABS_FACTOR(internalCellIndex, absCount, absFactor, ueContext_p)

		/* Set the logical channel and lcGId to invalid */
		/* SPR# 3147 Changes Starts */
		UInt8 lcGId = ueContext_p->lcInfo[ueLCId].lcGId;
	/* SPR# 3147 Changes Ends */
	ueContext_p->lcInfo[ueLCId].lchId = INVALID_LCID;

	if ( 0 == ueContext_p->lcCount[ueContext_p->lcInfo[ueLCId].lcGId])
	{
		lteWarning("lcCount for this LCG already 0\n");
	}
	else
	{
		ueContext_p->lcCount[ueContext_p->lcInfo[ueLCId].lcGId] -= 1;
		/* SPR# 3147 Changes Starts */
		ueContext_p->ulLCGInfo[lcGId].lcId[ueLCId] = INVALID_LCID;
		ueContext_p->ulLCGInfo[lcGId].numLC--;
		/* SPR# 3147 Changes Ends */
		/* + SPR 8571 Fix */
		if (0 == ueContext_p->ulLCGInfo[lcGId].numLC)
		{
			ueContext_p->bsrNetTotal = ueContext_p->bsrNetTotal -
				ueContext_p->bsrNet[lcGId];
            /* SPR 19702 Changes Start */
            /*
            ** bsrNet is being used to send status of LC (ACTIVE/NOT_ACTIVE) while
            ** deleting ueContext.
            ** this will be re-initialized while creating the context again from
            ** fillULSyncLossAndSchedInfo().
            **SPR 22343 Comments
            **AT the time of reconfigure UE if the LC is deleted and all the LC correpsonding to and LCG are deleted then in that case the bsrNet value for that LCG is updated by the LP thread while sending response to RRC.
            */
            //ueContext_p->bsrNet[lcGId] = 0;
            /* SPR 19702 Changes End */
		}
		/* - SPR 8571 Fix */
	}
	ueContext_p->numLogicalChannels--;
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT)
	UInt8 count = 0;

	/* + SPR_5711 */
	/*    if(UL_QOS_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p\
	      [internalCellIndex]->initParams_p->ulSchdConfig.ulSchedulerStrategy)  
	      {*/
	if(ueContext_p->maxConfiguredLcId == ueLCId)
	{
		ueContext_p->maxConfiguredLcId = 0;
		for(count = 0; count < ueContext_p->numLogicalChannels ; count++)
		{
			/* SPR 4950 Start */
			if ( ueContext_p->lcInfo[count].lchId != INVALID_LCID )
			{            
				/* SPR 4950 End */
				ueContext_p->maxConfiguredLcId = ((ueContext_p->maxConfiguredLcId < ueContext_p->lcInfo[count].lchId)?
						ueContext_p->lcInfo[count].lchId:ueContext_p->maxConfiguredLcId );
				/* SPR 4950 Start */
			}
			/* SPR 4950 End */
		}
	}
	/* }*/
	/* - SPR_5711 */
#endif
	/* +DYNAMIC_ICIC */
    {
	if( SRB != ueContext_p->lcInfo[ueLCId].ulQosInfo.channelType)
	{
		/*Cyclomatic Complexity changes - starts here*/
		/*CA Changes start  */
		updateLcgLcParameters(ueContext_p,ueLCId, internalCellIndex);
		/*CA Changes end  */

        /* +- SPR 17777 */
		updateQciPowerMaskAndLcgId(ueContext_p);
        /* +- SPR 17777 */
		/*Cyclomatic Complexity changes - ends here */
	}
	/* -DYNAMIC_ICIC */

	/* SPR 3608 changes start */
	if(TRUE == ueContext_p->lcInfo[ueLCId].uePSRReqFlag)
	{
		ueContext_p->lcInfo[ueLCId].uePSRReqFlag = FALSE;
		if(0 < ueContext_p->uePsrCount)
			ueContext_p->uePsrCount--;
	}
	/* SPR 3608 changes end */

	/* DESIGN REVIEW CHANGES INCORPORATION START */
	/* + SPR#5367 */
	if(QOS_GBR == ueContext_p->lcInfo[ueLCId].ulQosInfo.channelType)
	{
		/* Coverity 59949 + SPR 9203 Fix Starts*/
		if( ulAggregateGbrPerTTI_g[internalCellIndex][ueContext_p->userType]<
				/* SPR 11248 Fix Start */ 
				ueContext_p->lcInfo[ueLCId].ulQosInfo.GBR / 
				/* SPR 11248 Fix End */ 
				/* SPR 11115 Fix Start */
				numOfTicksPerSecond )
			/* SPR 11115 Fix End */
		{
			ulAggregateGbrPerTTI_g [internalCellIndex][ueContext_p->userType]= 0;
		}
		else
		{
			ulAggregateGbrPerTTI_g[internalCellIndex][ueContext_p->userType] -= 
				(ueContext_p->lcInfo[ueLCId].ulQosInfo.GBR /           
				 /* SPR 11115 Fix Start */
				 numOfTicksPerSecond) * absFactor;
			/* SPR 11115 Fix End */
		}
		/* Coverity 59949 + SPR 9203 Fix Ends*/
	}
	/* - SPR#5367 */
	/* DESIGN REVIEW CHANGES INCORPORATION END */
	/* SPS_CHG */
	/* Rest SPS Info incase of SPS LC Deletion */
	/*CA Changes start  */
	if (TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam.enableSps && 
			/*CA Changes end  */
			ueContext_p->spsUlInfo.lcId == ueLCId)
	{
		if (ulUECtxInfoArr_g[ueContext_p->ueIndex].pendingDeleteFlag != TRUE)
		{
            /* +- SPR 17777 */
			/* Call state machine for setting state to SPS_IDLE  */
			if( MAC_FAILURE == ulStateMachine[ueContext_p->spsUlInfo.\
					spsState][SPS_CONFIG_RECVD]( 
						ueContext_p->ueIndex, 
						ueContext_p->spsUlInfo.spsState,
						SPS_CONFIG_RECVD, ueContext_p, PNULL, 
                        /* + SPS_TDD_Changes */ 
#ifdef FDD_CONFIG
						PHY_DL_DELAY
#else 			
						DL_ADVANCE_TICK
#endif
                        /* - SPS_TDD_Changes */
						/*CA Changes start  */
						,internalCellIndex ) )
				/*CA Changes end  */
			{
                /* +- SPR 17777 */
                LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, ueContext_p->ueIndex,ueContext_p->spsUlInfo.spsState, SPS_CONFIG_RECVD,
                     ueContext_p->spsUlInfo.spsState, 0,0, __func__, "UlStateMachine");
				return MAC_FAILURE;
			}
            LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, ueContext_p->ueIndex,ueContext_p->spsUlInfo.spsState, SPS_CONFIG_RECVD,
                     ueContext_p->spsUlInfo.spsState, 0,0, __func__, "UlStateMachine");
                    /* +- SPR 17777 */
		}
		/* Reset parameters */
		ueContext_p->spsUlInfo.lcId = INVALID_LCID;
		ueContext_p->lcInfo[ueLCId].isSpsLc = FALSE;
	}
	/* SPS_CHG */

	/*SPR 4287 Start*/
	/*CA Changes start  */
	if(UL_QOS_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig.ulSchedulerStrategy)
		/*CA Changes end  */
	{
		/*SPR 4287 End*/
		/*Cyclomatic Complexity changes - starts here*/
		/*CA Changes start  */
		updateLcInfoDuringUlQosScheduling(ueContext_p,ueLCId, internalCellIndex);
		/*CA Changes end  */
		/*Cyclomatic Complexity changes - ends here */
		/* Fix for SPR 4287 start */
	}
    }
	/* Fix for SPR 4287 End */

	/*SPR 3674 changes start */
#ifdef KPI_STATS
	qci = ueContext_p->lcInfo[ueLCId].ulQosInfo.QCI;
    /* SPR 16916 FIX START */
    if ( (qci) && (qci <= MAX_QCI) )
    {
        /* SPR 10925 Start */
        if (TRUE == macIsQciUniqueForUlLc(ueLCId,qci,ueContext_p->ueIndex))
        {
            /* SPR 16548 fix start */
            if ( (ueContext_p->qciInfoFlag[--qci]) &&
                    (gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qci] > 0 ))
            {
                /* SPR 16548 fix end */
                gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qci]--;
                /* SPR 23402 Changes Start */
                /* MOCN Changes Start */
                /*MOCN SPR21942 Fix Start*/
                if(mocnOperatorParams_g.isMocnEnabled && ueContext_p->operatorId < mocnOperatorParams_g.numOfOperator)
                {
                    if(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciUL[ueContext_p->operatorId][qci]) 
                    {
                        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciUL[ueContext_p->operatorId][qci]--; 
                    }
                }
                /*MOCN SPR21942 Fix End*/
                /* MOCN Changes End */
               /* SPR 23402 Changes End */
            }
            ueContext_p->qciInfoFlag[qci] = 0;
        }

        /* SPR 10925 Start */
    }
    /* SPR 10925 End */
#endif
	/*SPR 3674 changes end */
    /* SPR 19702 Changes Start */
    /*
    ** LcGId is being used to send status of LC (ACTIVE/NOT_ACTIVE) while
    ** deleting ueContext.
    ** this will be re-initialized while creating the context again from
    ** fillULContextLcInfo().
    **SPR 22343 Comments
    **AT the time of reconfigure UE if the LC is deleted in that case the lcgID for that LC in reset when the LP thread is marking the status for that LC as ACTIVE or INACTIVE.
    */
    //ueContext_p->lcInfo[ueLCId].lcGId = LCGID_INVALID_VALUE;
    /* SPR 19702 Changes End */
	ueContext_p->lcInfo[ueLCId].ulQosInfo.ulQosStrategyLCNode_p = PNULL;

	LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_IS_NOT_VALID(ueContext_p->ueIndex, ueLCId);


	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
	return MAC_SUCCESS;
}
/*Cyclomatic Complexity changes - starts here*/
/*****************************************************************************
 * Function Name  : updateLcgLcParameters
 * Inputs         : ULUEContext - Poiter to UL Ue context
 *                  ueLCId - LC ID to update info
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the LCG and LC parameters as needed.
 *****************************************************************************/
STATIC  void updateLcgLcParameters(ULUEContext *ueContext_p,
		/*CA Changes start  */
		UInt8 ueLCId, InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
	UInt8 numOfQciInfo = 0;
	UInt8 lcgCounter = 0;
	SInt32 currentMax;
	UInt8 currentLcGId = LCGID_INVALID_VALUE;
	UInt8 lcGId = ueContext_p->lcInfo[ueLCId].lcGId;
	SInt32 deltaPowerMaskToDel = INVALID_UL_POWER_MASK;
	/*CA Changes start  */
	ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + ulPowerMaskIndex_g[internalCellIndex];
	/*CA Changes end  */
	UInt8 totalNumLcInLcg = 0;
	UInt8 localCount = 0;
	UInt8 count = 0;
	UInt8 qciToDel = 0;
	UInt8 qciIndex = 0;/* intialize wid invalid value */

	numOfQciInfo = ueContext_p->qciDeltaPowerMaskInfo.numOfQciInfo;
	qciToDel = ueContext_p->lcInfo[ueLCId].ulQosInfo.QCI - 1;
	deltaPowerMaskToDel = ulPowerMaskInfo_p->qciDeltaPowerMask[qciToDel];
	totalNumLcInLcg = ueContext_p->ulLCGInfo[lcGId].numLC;

	for( lcgCounter = 0;lcgCounter < numOfQciInfo; lcgCounter++)
	{
		currentMax = ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[lcgCounter].maxQciDeltaPowerMask;
		currentLcGId = ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[lcgCounter].lcgId;

		if((lcGId == currentLcGId) && (deltaPowerMaskToDel == currentMax)  )
		{
			if(totalNumLcInLcg == 1)
			{
				localCount = lcgCounter;
				for(count = lcgCounter +1; count < numOfQciInfo ;count ++)
				{
					ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[localCount].maxQciDeltaPowerMask =
						ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[count].maxQciDeltaPowerMask;
					ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[localCount].lcgId =
						ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[count].lcgId;
					localCount++;
				}
				ueContext_p->qciDeltaPowerMaskInfo.numOfQciInfo--;
			}
			else
			{
				currentMax= MIN_PC_MAX - 1;
				for(count = 3 ;count < MAX_NUMBER_OF_LOGICAL_CHANNEL ; count ++)
				{
					if ( ( ueContext_p->lcInfo[count].lchId != INVALID_LCID ) && (currentLcGId == ueContext_p->lcInfo[count].lcGId)  )
					{
						qciIndex = ueContext_p->lcInfo[count].ulQosInfo.QCI - 1;
						if ((qciIndex < 9) && 
								(currentMax < ulPowerMaskInfo_p->qciDeltaPowerMask[qciIndex]) )
						{
							ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[lcgCounter].
								maxQciDeltaPowerMask = ulPowerMaskInfo_p->qciDeltaPowerMask[qciIndex];
							currentMax =  ueContext_p->qciDeltaPowerMaskInfo.
								deltaPowerMaskInfo[lcgCounter].maxQciDeltaPowerMask;
						}
					}
				}
			}
			break;
		}
	}
}
/*****************************************************************************
 * Function Name  : updateQciPowerMaskAndLcgId
 * Inputs         : ULUEContext - Poiter to UL UE context
 *                  ueLCId - LC ID to update info
 * Outputs        : None
 * Returns        : None
 * Description    : This function will update QciPowerMask and LcgId 
 *****************************************************************************/
/* +- SPR 17777 */
STATIC  void updateQciPowerMaskAndLcgId(ULUEContext *ueContext_p)
    /* +- SPR 17777 */
{
	UInt8 loopCount = 0;
	UInt8 loopCountInner = 0;
	UInt8 currentlcGIdVal = 255;
	SInt32 currentQciPowerMaskVal = 0;

	if ( ueContext_p->qciDeltaPowerMaskInfo.numOfQciInfo > 1 )
	{
		for ( loopCount = 1;
				loopCount < ueContext_p->qciDeltaPowerMaskInfo.numOfQciInfo;
				loopCount++)
		{

			currentQciPowerMaskVal = 
				ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCount].maxQciDeltaPowerMask ;
			currentlcGIdVal =  
				ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCount].lcgId;

			for (loopCountInner = loopCount;
					loopCountInner > 0 && currentQciPowerMaskVal < ueContext_p->qciDeltaPowerMaskInfo.
					deltaPowerMaskInfo[loopCountInner-1].maxQciDeltaPowerMask;
					loopCountInner--)
			{
				ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCountInner].maxQciDeltaPowerMask =
					ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCountInner-1].maxQciDeltaPowerMask;
				ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCountInner].lcgId =
					ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCountInner-1].lcgId;
			}
			ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCountInner].maxQciDeltaPowerMask = currentQciPowerMaskVal;
			ueContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[loopCountInner].lcgId = currentlcGIdVal;
		}
	}
}
/*****************************************************************************
 * Function Name  : updateLcInfoDuringUlQosScheduling
 * Inputs         : ULUEContext - Poiter to UL UE context
 *                  ueLCId - LC ID to update info
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the LCG and LC parameters as needed when upliin stragety used is QOS.
 *****************************************************************************/
STATIC  void updateLcInfoDuringUlQosScheduling(ULUEContext *ueContext_p,
		UInt8 ueLCId,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
	ULQosStrategyLCNode  *ulQosStrategyLCNode_p = PNULL;
	UInt8 totalNumLcInLcg = 0;
	UInt8 lcGId = ueContext_p->lcInfo[ueLCId].lcGId;

	totalNumLcInLcg = ueContext_p->ulLCGInfo[lcGId].numLC;
	/* SPR# 3642 Changes Starts */
	if(NON_GBR == ueContext_p->lcInfo[ueLCId].ulQosInfo.channelType)
	{
		ulQosStrategyLCNode_p = ueContext_p->ulAmbrInfo.ulQosStrategyLCNode_p;
	}
	else
	{
		ulQosStrategyLCNode_p = ueContext_p->lcInfo[ueLCId].ulQosInfo.ulQosStrategyLCNode_p;
	}
	/* SPR# 3642 Changes Ends */
	/*QOS_COMPILATION_BREAK*/
	if((QOS_GBR == ueContext_p->lcInfo[ueLCId].ulQosInfo.channelType)||
			(SRB == ueContext_p->lcInfo[ueLCId].ulQosInfo.channelType 
			 /* SPR 2863 Fix */
			 && ulQosStrategyLCNode_p))
		/* SPR 2863 Fix */
		/*QOS_COMPILATION_BREAK*/
	{
		LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_GBR_COUNT(ueContext_p->ueIndex);

		if (ulQosStrategyLCNode_p->isPresentInEligibleList)
		{
			listDeleteNode(&ueContext_p->ulEligibleLcList, 
					&ulQosStrategyLCNode_p->eligibleLcNodeAnchor);

			if(UL_QOS_NO_TRANSMISSION < ueContext_p->scheduleFlag)
			{
				/*CA Changes start  */
				ulQosDeleteEntryFromPrioritisedLcTree(ulQosStrategyLCNode_p, internalCellIndex);
				/*CA Changes end  */
			}
		}
		/* MEM_LEAK_DEBUG FIX START */
		freeMemPool(ulQosStrategyLCNode_p);
        ulQosStrategyLCNode_p = PNULL;
		ueContext_p->lcInfo[ueLCId].ulQosInfo.ulQosStrategyLCNode_p = PNULL;
		/* MEM_LEAK_DEBUG FIX END */
	}
	else if((PNULL != ulQosStrategyLCNode_p) && (NON_GBR == ueContext_p->lcInfo[ueLCId].ulQosInfo.channelType))
	{
		LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_NGBR_COUNT(ueContext_p->ueIndex);
		/* SPR# 4824 Changes Starts  */
		UInt8 lcIndex = 0;
		UInt8 isDrbPresentFlag = FALSE;
		ueContext_p->ulAmbrInfo.minQci = INVALID_QCI;
		for(; lcIndex < MAX_NUMBER_OF_LOGICAL_CHANNEL; lcIndex++)
		{
			if((ueContext_p->lcInfo[lcIndex].lcGId == lcGId)&&(totalNumLcInLcg))
			{
				if(NON_GBR == ueContext_p->lcInfo[lcIndex].ulQosInfo.channelType )
				{
					if((ueContext_p->lcInfo[lcIndex].lchId != ueLCId)&&(INVALID_LCID != ueContext_p->lcInfo[lcIndex].lchId ))         
					{
						/* SPR 13953 fix start */
						totalNumLcInLcg--;
						/* SPR 13953 fix end */
						if(ueContext_p->ulAmbrInfo.minQci >  ueContext_p->lcInfo[lcIndex].ulQosInfo.QCI )
						{
							ueContext_p->ulAmbrInfo.minQci = 
								ueContext_p->lcInfo[lcIndex].ulQosInfo.QCI ;

							ueContext_p->ulAmbrInfo.lcId = ueContext_p->lcInfo[lcIndex].lchId;   
							ulQosStrategyLCNode_p->lchId = ueContext_p->ulAmbrInfo.lcId;
						}
					}
				}
				if((QOS_GBR == ueContext_p->lcInfo[lcIndex].ulQosInfo.channelType)||(INVALID_QCI != ueContext_p->ulAmbrInfo.minQci ))
					isDrbPresentFlag = TRUE;
			}
		}
		if(INVALID_QCI == ueContext_p->ulAmbrInfo.minQci)
		{
			if (ueContext_p->ulAmbrInfo.isPresentInEligibleList)
			{
				listDeleteNode(&ueContext_p->ulEligibleLcList, 
						&ulQosStrategyLCNode_p->eligibleLcNodeAnchor);

				ueContext_p->ulAmbrInfo.isPresentInEligibleList = FALSE;
			}
			/* SPR 7454 Fix start*/
			if(TRUE == ueContext_p->ulAmbrInfo.isLCNodePresent)
			{
				/*CA Changes start  */
				ulQosDeleteEntryFromPrioritisedLcTree(ulQosStrategyLCNode_p, internalCellIndex);
				/*CA Changes end  */
				ueContext_p->ulAmbrInfo.isLCNodePresent = FALSE;
			}
			/* SPR 7454 Fix end*/
			/* MEM_LEAK_DEBUG FIX START */
			freeMemPool(ulQosStrategyLCNode_p);
            ulQosStrategyLCNode_p = PNULL;
			ueContext_p->ulAmbrInfo.ulQosStrategyLCNode_p = PNULL;
			/* MEM_LEAK_DEBUG FIX END */
			if(FALSE == isDrbPresentFlag)
			{
				ueContext_p->ulLCGInfo[lcGId].isDrbPresent = FALSE ;

			}
		}
		/* SPR# 4824 Changes Ends */
	}
}
/*Cyclomatic Complexity changes - ends here */
/*****************************************************************************
 * Function Name  : updateDrxInUeContext
 * Inputs         : ueIndex -
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS, or MAC_FAILURE
 * Description    : This function update the DRX Flag in the DL UE Context.
 *****************************************************************************/
 MacRetType updateDRXInDLUEContext( UInt32 ueIndex,
		InternalCellIndex internalCellIndex)
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__); 
	DLUEContext  *newUEcontext_p = PNULL;    
	DRXQueueNode *drxQueueNode_p = PNULL;        
	newUEcontext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p; 
	if (PNULL == newUEcontext_p)
	{
		lteWarning (" UE Context does not exists.\n");
		return MAC_FAILURE;
	}
	if ( !newUEcontext_p->drxFlag )
	{ 
		GET_MEM_NODE_DRX_Q(drxQueueNode_p,DRXQueueNode);
		/* Rel 5.3: Coverity Fix Start */
		if (PNULL != drxQueueNode_p)
		{
			drxQueueNode_p->ueIdx = ueIndex;

			/* + COVERITY 32458 */
			if (!(ENQUEUE_DRX_Q(dlDRXQueue_gp[internalCellIndex],DRXQueueNode,&drxQueueNode_p->dRXQueueNodeAnchor)))
			{
				FREE_MEM_NODE_DRX_Q(drxQueueNode_p);
				return MAC_FAILURE;
			}
			/* - COVERITY 32458 */

		}
		else
		{
			/* Not able to allocate memory for DRX node */
			return MAC_FAILURE;
		}
		/* Rel 5.3: Coverity Fix End */
	}
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
	return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : cleanupUEConfMgr 
 * Inputs         : internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : 
 * Description    : This function is used to clean up the memory that are used
 *                  is DRX Queue, pendingDLUEDeleteQueue_g, pendingULUEDeleteQueue_g.
 *****************************************************************************/
/* CA Changes start */
void cleanupUEConfMgr(InternalCellIndex internalCellIndex)
	/* CA Changes end */
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	LTE_SNODE *node_p = PNULL;

	/* Check downlink DRX  Queue */
	while(1){
		DEQUEUE_DRX_Q(dlDRXQueue_gp[internalCellIndex],DRXQueueNode,(void **) &node_p);
		if(node_p)
		{
			FREE_MEM_NODE_DRX_Q(node_p);
		}
		else
		{
			break;
		}
	}   

	/* Check Pending UE Deletion Queue */
	/*CA Changes start  */
	while((node_p = popNode(pendingUEDeleteQueue_gp[internalCellIndex]))) {
		/*CA Changes end  */
		freeMemPool(node_p);
		node_p = PNULL;
	}   
/* SPR 21105 fix start */
/* code removed */
/* processPendingDeleteUE func is never referrred so, it and fuction it is
 * referring to  are removed */
/* SPR 21105 fix end */

	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
}

/*coverity changes CID-54597*/
/*****************************************************************************
 * Function Name  : deInitUEConfMgr 
 * Inputs         : internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : 
 * Description    : This function is used to clean up the memory that are used
 *                  is DRX Queue, pendingDLUEDeleteQueue_g, pendingULUEDeleteQueue_g.
 *****************************************************************************/
void deInitUEConfMgr(InternalCellIndex internalCellIndex)
{
	QDEINIT_NODE_DRX_Q(dlDRXQueue_gp[internalCellIndex]);
	freeMemPool(dlDRXQueue_gp[internalCellIndex]);
	dlDRXQueue_gp[internalCellIndex] = PNULL;

}
/*coverity changes CID-54597*/

/*****************************************************************************
 * Function Name  : deleteUEContext 
 * Inputs         : ueIndex
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This function is used to delete UEs Context (both uplink 
 *                  and downlink.) The function is called only in case of cell
 *                  delete scenario.
 *****************************************************************************/
MacRetType deleteUEContext( UInt32 ueIndex
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA changes End */
		)
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	DLUEContext *ueDLContext_p = PNULL;
	ULUEContext *ueULContext_p = PNULL;
	SpsSysWideParam *spsSysWideParam_p = PNULL;
	/* SPR 13492 Changes Start */
#ifdef KPI_STATS
	UInt8 qci       = INVALID_QCI;
#endif
	/* SPR 13492 Changes End */
	/*SPR# 6984 Changes Start */
	{
		spsSysWideParam_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam);
	}
	/*SPR# 6984 Changes End */

	/* Deleting Downlink UE Context */
	ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
	if (PNULL == ueDLContext_p)
	{
		LOG_MAC_MSG(MAC_DLUECONTEXT_NOT_FOUND_ID,
				LOGERROR,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				ueIndex,
				LINE_NUMBER,
				internalCellIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,
				"MAC_DLUECONTEXT_NOT_FOUND");

		LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR,
				" Unable to delete the UE Entity. \n");
		return MAC_FAILURE;
	}

	/*** Coverity 62965 Fix : Start ****/
	ueULContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
	if (PNULL == ueULContext_p)
	{
		LOG_MAC_MSG(MAC_ULUECONTEXT_NOT_FOUND_ID,
				LOGERROR,
				MAC_RRC_INF,
				getCurrentTick(),
				ueIndex,
				LINE_NUMBER,
				internalCellIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,
				"MAC_ULUECONTEXT_NOT_FOUND");
		return MAC_FAILURE;
	}
	/*** Coverity 62965 Fix : End ****/
	/*spr 13127 chngs start*/
	if (ueDLContext_p->internalCellIndex != internalCellIndex)
	{
		return MAC_SUCCESS;
	}
	/*spr 13127 chngs end*/
    /*Jira_651_Changes Start*/
    if (ueDLContext_p->activeUENodeTempDeleted_p !=PNULL)
    {
        freeMemPool(ueDLContext_p->activeUENodeTempDeleted_p);
        ueDLContext_p->activeUENodeTempDeleted_p = PNULL;
    }

    /*Jira_651_Changes End*/  

	if (ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p)
	{
        /* SPR 13562 changes */
        /*  +- SPR 17777 */
        deleteEntryFromUENonZeroQueueLoadAndHarqList(
                ueDLContext_p,internalCellIndex);
        /*  +- SPR 17777 */
        /* SPR 13562 changes */
		ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p = PNULL;
	}

	/*SPR# 6984 Changes Start */
	{
	if(DL_QOS_BASED_SCHEDULING == (cellSpecificParams_g.\
				cellConfigAndInitParams_p[internalCellIndex]->\
				initParams_p->dlSchdConfig.dlSchedulerStrategy) )
	{
		deleteDLUELogicalChannel(ueDLContext_p,1,internalCellIndex);
		deleteDLUELogicalChannel(ueDLContext_p,2,internalCellIndex);
		/*SPR 10262 fix starts*/
		while(ueDLContext_p->gbrLcCount)
		{
			deleteDLUELogicalChannel(ueDLContext_p, 
					ueDLContext_p->gbrLcInfo[0],internalCellIndex);
		}
		while(ueDLContext_p->nonGbrLcCount)
		{
			deleteDLUELogicalChannel(ueDLContext_p, 
					ueDLContext_p->nonGbrLcInfo[0],internalCellIndex);
		}
		/*SPR 10262 fix ends*/
	}
    /* SPR 16916 FIX START */
    else
    {
        /* ActiveUEcount is already updated in QOS for each lc 
         * in function deleteDLUELogicalChannel */
#ifdef KPI_STATS
        updateNumOfActiveUeDlOnUeDelete(ueDLContext_p->ueIndex,internalCellIndex);
#endif
    }
    }
    /* SPR 16916 FIX END */
	/*Delete DL LC Strategy Node*/
	/* Cyclomatic_complexity_changes_start */
	deleteDlLcStrategyNode(ueDLContext_p);
	/* Cyclomatic_complexity_changes_end */
	/*SPR# 6984 Changes End */

	/* SPR 5288 Start */
	/*CA Changes start  */
	if (MAC_FAILURE == deleteMACDLHarqEntity(ueDLContext_p, internalCellIndex))
		/*CA Changes end  */
		/* SPR 5288 End */
	{
		LOG_MAC_MSG(MAC_UEHARQCONTEXT_NOT_FOUND_ID,
				LOGERROR,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				ueIndex,
				LINE_NUMBER,
				internalCellIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,
				"MAC_UEHARQCONTEXT_NOT_FOUND");
		return MAC_FAILURE; 
	}

#ifdef HD_FDD_CONFIG
	if(TRUE == ueDLContext_p->isHDFddFlag)
	{
		deleteUeFromHdFddContext(ueDLContext_p,internalCellIndex);
	}
#endif

	if(PNULL != ueDLContext_p->dlCQIInfo.subBandReportInfo_p)
	{
		freeMemPool(ueDLContext_p->dlCQIInfo.subBandReportInfo_p);
		ueDLContext_p->dlCQIInfo.subBandReportInfo_p = PNULL;
	}

	{
	checkAndDeleteUEMeasGapConfig (ueULContext_p, ueDLContext_p,
			internalCellIndex);
	/* remove UE nodes from DRX timer lists */
	if(SETUP == ueDLContext_p->drxConfigType)
	{
		deleteDRXConfig(ueDLContext_p, ueULContext_p,internalCellIndex);
	}

	/* delete entry from Qos token list */
	qosProcessTknBktAlgoForUeDelete(ueIndex,internalCellIndex);
	}

	if(MAC_SUCCESS != deleteTpcPdcchConfigInfoForPucch(DELETE_UE_REQ,
                /* +- SPR 17777 */
				ueDLContext_p,internalCellIndex))
	{
		/* Nothing needs to be done in case of UE Release*/
	}	
	/* HD FDD Changes End*/
	{
	if (UL_QOS_SCHEDULING != cellSpecificParams_g.\
			cellConfigAndInitParams_p[internalCellIndex]\
			->initParams_p->ulSchdConfig.ulSchedulerStrategy)
	{
		UInt8 lcCount = 1;
		while((lcCount < MAX_LCID) && (ueULContext_p->numLogicalChannels)) 
		{
			if(255 != ueULContext_p->lcInfo[lcCount].lchId)
			{
				deleteULUELogicalChannel(ueULContext_p, 
						ueULContext_p->lcInfo[lcCount].lchId,
						internalCellIndex);
			}
			lcCount++;
		}

	}
	/*! \code
	 * If SPS is enabled in system
	 *    Call dlSpsPendingDeleteHandling.
	 *    Call ulSpsPendingDeleteHandling.
	 * \endcode
	 * */
	if( TRUE == spsSysWideParam_p->enableSps )
	{
		dlSpsPendingDeleteHandling( ueDLContext_p , internalCellIndex);
		LTE_MAC_UPDATE_DOWNLINK_SPS_UE_EXIST_STATE(ueIndex,LTE_FALSE); 
		LTE_MAC_UPDATE_SPS_CONFIG_RESP_STATS(LTE_FALSE,ueIndex,0);

		ulSpsPendingDeleteHandling( ueULContext_p ,internalCellIndex);
		LTE_MAC_UPDATE_UPLINK_SPS_UE_EXIST_STATE(ueIndex,LTE_FALSE,getCurrentTick()); 
		LTE_MAC_UPDATE_SPS_CONFIG_RESP_STATS(LTE_FALSE,ueIndex,0);
	}

	/* SPR_10354_Base_code Start */
	deleteUEPeriodicReportBatchInfo(ueIndex,internalCellIndex);
	/* SPR_10354_Base_code End */
	}

	freeMemPool(ueDLContext_p);
	dlUECtxInfoArr_g[ueIndex].dlUEContext_p = PNULL;
	dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag = TRUE;
	dlUECtxInfoArr_g[ueIndex].scellRelFlag = FALSE;

	/* Deleting Uplink UE Context */

	ueULContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
	/* If UE Context does not exits */
	/* +COVERITY 10046 */
	if (PNULL == ueULContext_p)
		/* -COVERITY 10046 */
	{
		LOG_MAC_MSG(MAC_ULUECONTEXT_NOT_FOUND_ID,
				LOGERROR,
				MAC_RRC_INF,
				getCurrentTick(),
				ueIndex,
				LINE_NUMBER,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,
				"MAC_ULUECONTEXT_NOT_FOUND");

		LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR,
				" Unable to delete the UE Entity. \n");
		return MAC_FAILURE;
	}
	/* Cyclomatic_complexity_changes_start */

	/*SPR# 6984 Changes Start */
     /* SPR 16916 FIX START */
#ifdef KPI_STATS
    updateNumOfActiveUeUlOnUeDelete(ueULContext_p->ueIndex,internalCellIndex);
#endif
     /* SPR 16916 FIX END */
	deleteUlLcStrategyNode(ueULContext_p);
	deleteUlLcStrategyNodeFromAmbrInfo(ueULContext_p);
	/*SPR# 6984 Changes End */
	deleteFromNonZeroBsrList(ueULContext_p,internalCellIndex);
	deleteMcsToPrbMap(ueULContext_p);
	/* Cyclomatic_complexity_changes_end */
	freeMemPool(ueULContext_p);
	ulUECtxInfoArr_g[ueIndex].ulUEContext_p = PNULL;
	ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag = TRUE;
	ulUECtxInfoArr_g[ueIndex].scellRelFlag = FALSE;
	/* SPR 13492 Changes Start */
#ifdef KPI_STATS
	for (qci = 0; qci < MAX_QCI ; ++qci)
	{
		dlUECtxInfoArr_g[ueIndex]. 
			inSessionTimePerQCIFlag[qci] = FALSE;
		ulUECtxInfoArr_g[ueIndex]. 
			inSessionTimePerQCIFlag[qci] = FALSE;
	}
#endif
	/* SPR 13492 Changes End */

    /* SPR 16962 fix start */
    if( cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->totalActiveUE)
    {
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->totalActiveUE--;
#ifdef SSI_DEBUG_STATS
        --gMacStats.pStats->numActiveUe[internalCellIndex];
#endif        
    }
    else
    {
        /* unable to decrement totalActiveUE as count has already reached 
         * zero*/
        LOG_MSG( MAC_TOT_ACTIVE_UE_ZERO, LOGERROR, MAC_RRC_INF,
                getCurrentTick(), __LINE__, ueIndex, 0,0,0, 0,0,
                __func__, "" );
    }
    /* SPR 16962 fix end */
    LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
    return MAC_SUCCESS;
}

#ifndef TDD_CONFIG
/*****************************************************************************
 * Function Name  : deletePeriodicReportsNode
 * Inputs         : pucchConfig_p/currentUEContext_p
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to delete the periodic reports
 *                  sr,cqi,ri nodes.
 *****************************************************************************/
/*SPR 16855 +-*/
/*CA Changes start  */
 STATIC void deletePeriodicReportsNode( PucchConfig *pucchConfig_p,
		InternalCellIndex internalCellIndex )
/*SPR 16855 +-*/
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
    /*SPR 16855 +-*/
	/* PUCCH TDD ADAPTATION CHANGES Start */        
	deletePCellPucchNodesForUE(pucchConfig_p,CQI_REPORT, 
			internalCellIndex);
	/*+SPR #3444*/
	deletePCellPucchNodesForUE(pucchConfig_p,RI_REPORT, 
			internalCellIndex);
	/*-SPR #3444*/
	/*+SPR #3444*/
	deletePCellPucchNodesForUE(pucchConfig_p,SCHEDULING_REQUEST, 
			internalCellIndex);
	/*-SPR #3444*/
	/* Rel9_upgrade_CR257 */
	deletePucchShortSRNodesForUE(pucchConfig_p,PNULL,internalCellIndex);
	/* PUCCH TDD ADAPTATION CHANGES end */        
    /*SPR 16855 +-*/
	/* Rel9_upgrade_CR257 */
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
}
#endif

/*****************************************************************************
 * Function Name  : resetMACULUEEntity 
 * Inputs         : ueULContext_p- Its a pointer to UL UE Context 
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function set the Harq Status for UL as to be reset 
 *                  so that it can be handled by UL Strategy for 
 *                  retransmissions
 *                  
 *****************************************************************************/
/*CA Changes start  */
void resetMACULUEEntity(ULUEContext *ulUEContext_p, InternalCellIndex internalCellIndex)
	/*CA Changes end  */
{
	UInt32 n = 0;
#ifdef TDD_CONFIG    
	UInt32 iter = 0;
/*SPR 19966 fix start*/
	UInt32  indexCount = 0;
/*SPR 19966 fix end*/
#endif    
	ULHarqInfo *ulHarqInfo_p = PNULL;

	for (n = 0; n <MAX_HARQ_PROCESS_NB; n++)
	{
		ulHarqInfo_p = &ulUEContext_p->ulresInfo[n];
		/*Check if UL Harq Process is busy, then only free it*/
		if(HARQ_PROCESS_FREE != ulHarqInfo_p->harqStatus)
		{
			ulHarqInfo_p->harqStatus = HARQ_PROCESS_RESET;
			/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
			ulHarqInfo_p->isPdcchAllocated = FALSE;
#endif
			/*TDD Config 0 Changes End*/
		}
	}

	ulUEContext_p->cqiInd = 0;
	ulUEContext_p->bsrNetTotal = 0;
	/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
	hdUeContextInfo_g.ulSrPresentFlag[ulUEContext_p->ueIndex] = FALSE;
#endif
	/*HD FDD Changes End*/
	ulUEContext_p->ulSchedulerAlgoInfo.uePriorityFactor = 0;
	ulUEContext_p->ulSchedulerAlgoInfo.modulationSchemeFactor = 0;
	ulUEContext_p->lastScheduledTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
	ulUEContext_p->lastActivityTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
	/* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
	ulUEContext_p->isAlreadyConsideredFlag = INVALID_ALREADY_CONSIDERED_FLAG;
#elif TDD_CONFIG
	for (n = 0; n < MAX_SUBFRAME; n++)
	{
/*SPR 19966 fix start*/
        for (indexCount = 0;indexCount < MAX_UL_HARQ_TIMER ; indexCount ++)
        {
            ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[indexCount][n] = INVALID_HARQ;
        }
/*SPR 19966 fix end*/
		ulUEContext_p->isAlreadyConsideredFlag[n] = 
			INVALID_ALREADY_CONSIDERED_FLAG;
	}
#endif
	/* TDD Config 0 Changes End */
	/* ICIC Changes start */
	ulUEContext_p->isAlreadyInCEPriorityList = FALSE;
	/* ICIC Changes end */
	/*TDD Config 0 Changes Start*/
#ifdef FDD_CONFIG
	ulUEContext_p->isPdcchAllocated = FALSE;
#endif
	/*TDD Config 0 Changes End*/
	/* SPR 12809 fix start */
	ulUEContext_p->srNode_p = PNULL;
	/* SPR 12809 fix end */
	/* Review comment fix start MAC_RESET_10 */
	for (n = 0; n < NUM_OF_LCG; n++)
	{
		ulUEContext_p->bsrNet[n] = 0;
		/* SPR 10027 FIX START */
		ulUEContext_p->ulLCGInfo[n].isDrbPresent = FALSE;
		/* SPR 10027 FIX END */
		/* SPR 12809 fix start */
		ulUEContext_p->ulLCGInfo[n].qLoad = 0;
		/* SPR 12809 fix end */
	}
	/* Review comment fix end MAC_RESET_10 */
	/* CLPC CHANGE*/
	/* SPR 3271 changes start */
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT)
	ulUEContext_p->isDemuxIndexPresent = FALSE;
#endif
	ulUEContext_p->mcsIndex =  ulUEContext_p->initialMcs;
	/*SPR 3600 Start*/
	/* Cyclomatic_complexity_changes_start */
	getUlModSchemeFromMcsIndex(ulUEContext_p->mcsIndex,\
			&(ulUEContext_p->modulationScheme),\
			cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->macEnable64QAM,\
			ulUEContext_p->ueCategory);
	/* Cyclomatic_complexity_changes_start */
	/*SPR 3600 End*/

	ulUEContext_p->currPowerCorrectionForPusch = 0;
	ulUEContext_p->prevPowerCorrectionForPusch = 0;
	ulUEContext_p->powerControlPuschTPCValue = 0;
	/*CA Changes start  */
	ulUEContext_p->pathLoss = (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])-> \
				  alphaBasedPathLossToTargetSINRMap.defaultPathLoss;
	/* ICIC changes start */
	updateULPathLoss(ulUEContext_p->ueIndex,ulUEContext_p->pathLoss, internalCellIndex);
	/*CA Changes end  */
	/* ICIC changes end */
	for(n = 0; n < MAX_SUB_FRAME; n++)
	{
		ulUEContext_p->powerControlDeltaTFAndLogFactor[n] = 0;
	}
	/*CA Changes start  */
	initUEULCSIContext(ulUEContext_p,ulUEContext_p->initialMcs, internalCellIndex);
	resetTpcPdcchConfigInfoForPusch (
            /* +- SPR 17777 */
			ulUEContext_p,
			internalCellIndex);
	/*CA Changes end  */
	/* SPR 3271 changes end */

	/* CLPC CHANGE - END*/

	/* SPR 3608 changes start */
	checkAndSetHighPriorityScheduling(ulUEContext_p->ueIndex);
	/* SPR 3608 changes end */
	ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount = 0;
	ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx = INVALID_PREAMBLE_IDX;
	ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState = UL_SYNC_LOSS_OFF;
    /* SPR 16125 Fix Start */
    ulUEContext_p->waitForMsg3BeforeResetting = FALSE;
    /* SPR 16125 Fix End */

#ifdef TDD_CONFIG
	for(n = 0; n < MAX_UE_SCHEDULED_INFO_INSTANCES; n++)
	{
		/** SPR 14204 Fix : Start **/
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].flag = UNSET_FLAG;
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].ulschUciInfoIndex =
			ULSCH_UCI_INVALID_INDEX;
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].ulCCEIndex = INVALID_CCE_INDEX;
		for(iter = 0; iter < MAX_SUB_FRAME; iter++)
		{
			ulUEContext_p->ueScheduledInfo[internalCellIndex][n].dlCceIndex[iter] =
				INVALID_CCE_INDEX;
            /* + Coverity 96934 */
            /* + SPS_TDD_Changes */
            ulUEContext_p->ueScheduledInfo[internalCellIndex][n].isSPSCCE[iter] = LTE_FALSE;
            /* - SPS_TDD_Changes */
            /* - Coverity 96934 */
		}
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].transMode = 0;
		/* SPR 7179 Fix Start */
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].ueInsertedInPdcchDlUeInfo = 0;
		/* SPR 7179 Fix End */
		/** SPR 14204 Fix : End **/
	}
#endif   
    /* SPR 16916 FIX START */
#ifdef KPI_STATS
    updateNumOfActiveUeUlOnUeDelete(ulUEContext_p->ueIndex,internalCellIndex);
#endif
    /* SPR 16916 FIX END */

	if(ulUEContext_p->nonZeroBsrListTempDeleted_p)
	{
		freeMemPool(ulUEContext_p->nonZeroBsrListTempDeleted_p);
		ulUEContext_p->nonZeroBsrListTempDeleted_p = PNULL;
	}
	/* L2_FIX_start */
	else if(PNULL != ulUEContext_p->nonZeroBsrList_p)
	{
		/*CA Changes start  */
		deleteEntryFromNonZeroBSRList(ulUEContext_p,ulUEContext_p->nonZeroBsrList_p, internalCellIndex);
		/*CA Changes end  */
		ulUEContext_p->nonZeroBsrList_p = PNULL;
	}
	/* L2_FIX_end */
    /* spr 21260 + */
    ulUEContext_p->isCqiDropped = FALSE;
    ulUEContext_p->cqiDropTick  = 0;
    /* spr 21260 - */
    /* + SPR 21811 Fix */
    ulUEContext_p->firstSRSIndRecvd = FALSE;
    /* - SPR 21811 Fix */

}
/* + CQI_5.0 */   
/******************************************************************************
 * Function Name : resetCQIInfo 
 * Inputs        : ueDLContext_p,DLCQIInfo
 * Outputs       : None
 * Returns       : None
 * Description   : Initializes Subband and Wideband CQI Info in UE's DL Context 
 *****************************************************************************/
void resetCQIInfo(DLUEContext *ueDLContext_p,
		DLCQIInfo *dlCQIInfo_p,
		InternalCellIndex internalCellIndex)
{ 
	/* SPR 6452 changes start */
	dlCQIInfo_p->aperiodicCQIRequested     = FALSE;
	dlCQIInfo_p->isSubBandReportAvailable  = FALSE;
	dlCQIInfo_p->isWideBandReportAvailable = FALSE;
	dlCQIInfo_p->reportReceivedTTI = 0;
	dlCQIInfo_p->previousDLBLERForCW0 = 0;
	dlCQIInfo_p->previousDLBLERForCW1 = 0;
	dlCQIInfo_p->dlBlerCalculatedCounterForCW0 = 0;
	dlCQIInfo_p->dlBlerCalculatedCounterForCW1 = 0;
	dlCQIInfo_p->cqiCorrectionFactorForCW0 = 0;
	dlCQIInfo_p->cqiCorrectionFactorForCW1 = 0;
	dlCQIInfo_p->currentDLBLERForCW0 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->currentDLBLERForCW1 = TARGET_DL_BLER_VALUE;
	/* SPR 13571 fix start */
	dlCQIInfo_p->prevDlBlerCalculatedForCW0 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->prevDlBlerCalculatedForCW1 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->prevDlBlerCalculatedForCW0_2 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->prevDlBlerCalculatedForCW1_2 = TARGET_DL_BLER_VALUE;
	/* SPR 13571 fix end */
	ueDLContext_p->isSubbandPMIAvailable = FALSE;
	ueDLContext_p->aperiodicMode12Rcvd = FALSE;


	if(dlCQIInfo_p->initialCqiValue < 
			dlCQIInfo_p->widebandCQICodeWordOne)
	{
		dlCQIInfo_p->widebandCQICodeWordOne = 
			dlCQIInfo_p->initialCqiValue;
		dlCQIInfo_p->mcsIndexCodeWordOne = getMCSFromCQI( 
                     dlCQIInfo_p->widebandCQICodeWordOne
                     );
 
	}
	if(dlCQIInfo_p->initialCqiValue < 
			dlCQIInfo_p->widebandCQICodeWordTwo)
	{
		dlCQIInfo_p->widebandCQICodeWordTwo = 
			dlCQIInfo_p->initialCqiValue;
		dlCQIInfo_p->mcsIndexCodeWordTwo = getMCSFromCQI( 
                     dlCQIInfo_p->widebandCQICodeWordTwo
                     );
	}
	/* SPR 14083 fix start */
	if(dlCQIInfo_p->initialCqiValue < 
			dlCQIInfo_p->previousTimeAveragedWBCQICW0)
	{
		dlCQIInfo_p->previousTimeAveragedWBCQICW0 = dlCQIInfo_p->initialCqiValue;
	}
	if(dlCQIInfo_p->initialCqiValue < 
			dlCQIInfo_p->previousTimeAveragedWBCQICW1)
	{
		dlCQIInfo_p->previousTimeAveragedWBCQICW1 = dlCQIInfo_p->initialCqiValue;
	}
	/* SPR 14083 fix end */

	if (PNULL !=  dlCQIInfo_p->subBandReportInfo_p)
	{
		/* + Coverity_Stack Fix */
		memSet(dlCQIInfo_p->subBandReportInfo_p,0,sizeof(SubBandReportInfo));
		/* - Coverity_Stack Fix */
		/* SPR 13967 changes start */
		/*numOfSBperBWpart variable is not being initialised after reestablishment.
		  CQI processing of 12 Subbands is not considered which is leading to 
		  allocation of remaining 4 RBs (remaining 1 Subband) only for that UE*/

		initNumOfSBperBWpart(dlCQIInfo_p,internalCellIndex);
		/* SPR 13967 changes end */

	}
}
/* - CQI_5.0 */


/* SPR 3608 changes start */
/****************************************************************************
 * Function Name  : checkAndSetHighPriorityScheduling 
 * Inputs         : ueIndex - index of ueContext 
 * Outputs        : None
 * Returns        : void 
 * Description    : This function check and set the high priority scheduling flag 
 *                  for a UE. 
 ****************************************************************************/
void checkAndSetHighPriorityScheduling(UInt32 ueIndex)
{
	ULUEContext *ulUEContext_p = PNULL;
	DLUEContext *dlUEContext_p = PNULL;

	ulUEContext_p = (ulUECtxInfoArr_g[ueIndex]).ulUEContext_p;
	dlUEContext_p = (dlUECtxInfoArr_g[ueIndex]).dlUEContext_p;

	if(FALSE != ulUEContext_p->prioritySchedulingReqUL || 
			FALSE != dlUEContext_p->prioritySchedulingReqDL)
	{
		LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR,"priority scheduling flag is set for UE index%u", ueIndex);
	}
	/* resetting flags */
	ulUEContext_p->prioritySchedulingReqUL = FALSE;
	dlUEContext_p->prioritySchedulingReqDL = FALSE;

	if(ulUEContext_p->uePsrCount)
		ulUEContext_p->prioritySchedulingReqUL = TRUE;
	if(dlUEContext_p->enbPsrCount)
		dlUEContext_p->prioritySchedulingReqDL = TRUE;
}
/* SPR 3608 changes end */

/*****************************************************************************
 * Function Name  : resetMACDLUEEntity 
 * Inputs         : ueDLContext_p,
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function set the DL UE context parameters to default 
 *                  values
 *****************************************************************************/
/*CA Changes start  */
MacRetType resetMACDLUEEntity(DLUEContext *ueDLContext_p, 
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
	DLCQIInfo   *dlCQIInfo_p     = PNULL;
	DLSchedulerAlgoInfo  *dLSchedulerAlgoInfo_p = PNULL;
	UInt32 *assignedRBQueueLoad_p = PNULL;
	DLLogicalChannelInfo *dlLogicalChInfo_p     = PNULL;
    /* SPR 15909 fix start */
	tickType_t currentTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
    /* SPR 15909 fix end */
	UInt8 n = 0, index = 0;
	UInt16 lcCount = MAX_NUMBER_OF_LOGICAL_CHANNEL;
	/* Review comment fix start MAC_RESET_6 */
#ifdef TDD_CONFIG
	UInt8 sfIndex = 0;
	UInt8 containerIndex = 0;
	DLUESCellContext *dlUeScellContext_p = PNULL;
#endif  
/*SPR 19288 changes start */ 
/*SPR 19288 changes end */ 
	/* Review comment fix start MAC_RESET_6 */
	/* Logical Channel Structure Initialisation */
	/* Initialising all the Logical Channels lastScheduledTick with default values */
	for (n = 0; n < MAX_NUMBER_OF_LOGICAL_CHANNEL; n++)
	{
		dlLogicalChInfo_p = &ueDLContext_p->logicalChannel[n];
		dlLogicalChInfo_p->lastScheduledTick = currentTick;
	}
	/* It indicates pdcch is allocated or not. */
	ueDLContext_p->isPdcchAllocatedFlag = FALSE;

	/* This Indicate is the ACK NACK is pending for TB in which DRX has been
	   sent .If true this means we won't sent DRX Again */
	ueDLContext_p->pendingDRXAckNack    = FALSE;

	/* SPR 4953 Fix Start */
	ueDLContext_p->scheduleFlag = DL_QOS_NEW_TRANSMISSION;
	/* SPR 4953 Fix End */

	/* Scheduler related Info */
	dLSchedulerAlgoInfo_p = &ueDLContext_p->dlSchedulerAlgoInfo;
	dLSchedulerAlgoInfo_p->modulationSchemeFactor = 0;
	dLSchedulerAlgoInfo_p->uePriorityFactor       = 0;

	/* CQI related Info */
	dlCQIInfo_p = &ueDLContext_p->dlCQIInfo;
	/* + CQI_5.0 */ 
	resetCQIInfo(ueDLContext_p,dlCQIInfo_p,internalCellIndex);
	/* + CQI_5.0 */

	ueDLContext_p->lastScheduledTick       = currentTick;
    /* +- SPR 16738 */
	ueDLContext_p->drxFlag                 = FALSE;
	ueDLContext_p->timingAdvanceValue      = INVALID_TA_VALUE;

	/* ICIC changes start */
	/*CA Changes start  */
	updateDLTA(ueDLContext_p->ueIndex,INVALID_TA_VALUE, internalCellIndex);
	/*CA Changes end  */
	ueDLContext_p->isAlreadyInCEPriorityList = FALSE;
	/* ICIC changes end*/

	ueDLContext_p->isAlreadyConsideredFlag = INVALID_ALREADY_CONSIDERED_FLAG;
	ueDLContext_p->numLCWithNonZeroQueueLoad = 0;

	assignedRBQueueLoad_p = &ueDLContext_p->assignedRBQueueLoad[0];
	*assignedRBQueueLoad_p = 0;
	ueDLContext_p->assignedRBQueueLoadSchIndex = 0;
	ueDLContext_p->assignedRBQueueLoadRLCIndex =0;
	ueDLContext_p->effectiveBitRate = 0;
	ueDLContext_p->ueQueueLoad = 0;
	for(lcCount = 0; lcCount < MAX_NUMBER_OF_LOGICAL_CHANNEL; lcCount++)
	{
		/* Review comment fix start MAC_RESET_5 */
		ueDLContext_p->logicalChannel[lcCount].queueLoad = 0;	
                /* Review comment fix end MAC_RESET_5 */
	}
	ueDLContext_p->lastSyncDetectedTick = currentTick;
	/* + TM6_5.2 */
	if(TX_MODE_6 == ueDLContext_p->dlMIMOInfo.transmissionMode)
	{
		/* + SPR 5953 Fix */ 
		ueDLContext_p->tm6DCIFormat = DCI_FORMAT_1A;

		if((((PERIODIC_CQI_REPORTING_MODE == ueDLContext_p->dlCQIInfo.cqiTypeIndicator) &&
						(1 == ueDLContext_p->dlCQIInfo.cqiRequestType)) ||
					(APERIODIC_CQI_REPORTING_MODE == ueDLContext_p->dlCQIInfo.cqiTypeIndicator) ||
					(PERIODIC_APERIODIC_MODE == ueDLContext_p->dlCQIInfo.cqiTypeIndicator)) &&
				(INVALID_CODE_BOOK_INDEX != ueDLContext_p->dlMIMOInfo.precodingIndex))
			/* - SPR 5953 Fix */ 
		{
			ueDLContext_p->tm6DCIFormat = DCI_FORMAT_1B;
			dlCQIInfo_p->wideBandPMIIndex = ueDLContext_p->dlMIMOInfo.precodingIndex;
		}
	}
	/* - TM6_5.2 */

/*SPR 19288 changes start */ 
/*SPR 19288 changes end */ 



	/* SPR 3607 Fix Start */
	if(!(ueDLContext_p->intraEnbHOFlag))
	{
		/* SPR 3607 Fix End */
        /* SPR 16049 Fix Start */
        /* SPR 16049 Fix End */
		ueDLContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState = UL_SYNC_LOSS_OFF;
		/* SPR 3608 Fix Start */
		checkAndSetHighPriorityScheduling(ueDLContext_p->ueIndex); 
		/* SPR 3608 Fix End */
		/* SPR 3607 Fix Start */
	}
	/* SPR 3607 Fix End */
	ueDLContext_p->lastActivityTick = currentTick;
	/* CLPC CHANGE */
	ueDLContext_p->currPowerCorrectionForPucch = 0;
	ueDLContext_p->powerControlPucchTpcValue = 0;
	ueDLContext_p->averageSinrForPucch = 
		/*CA Changes start  */
		(macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
		deltaSinrToTPCMapForPUCCH.pucchSINRTarget; 
	/*CA Changes end  */
	/* CLPC CHANGE - END */
	/* SPR 3271 changes start */
	/* Initialize blerInfoForPUCCH */
	/*CA Changes start  */
	initPucchBlerContext(ueDLContext_p, internalCellIndex);
	/*CA Changes end  */
	ueDLContext_p->lastSINRCoolOffCounter = 0;
	resetTpcPdcchConfigInfoForPucch (
            /* +- SPR 17777 */
			/*CA Changes start  */
			ueDLContext_p, internalCellIndex);
	/*CA Changes end  */
	/* SPR 3271 changes end */


	/* SPR #3953 changes start */
	/* SPR 5960 Changes Start */
	/* if((ueDLContext_p->activeUENodeTempDeleted_p != PNULL) ||
	   (ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p != PNULL)) 
	   { */
	/* SPR 5960 Changes End */
    /* SPR 16916 FIX START */
#ifdef KPI_STATS
    updateNumOfActiveUeDlOnUeDelete(ueDLContext_p->ueIndex,internalCellIndex);
#endif
    /* SPR 16916 FIX END */
	if (ueDLContext_p->activeUENodeTempDeleted_p !=PNULL)
	{
		freeMemPool(ueDLContext_p->activeUENodeTempDeleted_p);
		ueDLContext_p->activeUENodeTempDeleted_p = PNULL;
		ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p = PNULL;
	}
	else if (PNULL != ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p)
	{
		/* SPR 5960 Changes Start */
        /*  +- SPR 17777 */
		deleteEntryFromUENonZeroQueueLoadAndHarqList(
				/*CA Changes start  */
				ueDLContext_p, internalCellIndex);
        /*  +- SPR 17777 */
		/*CA Changes end  */
		/* SPR 5960 Changes End */
		ueDLContext_p->activeUENodeTempDeleted_p = PNULL;
	}
	/* SPR 5960 Changes Start */
	/*} */
	/* SPR 5960 Changes End */
	/* SPR #3953 changes end */


#ifdef TDD_CONFIG
	for(sfIndex =0; sfIndex < MAX_SUB_FRAME; sfIndex++)
	{
		for (containerIndex = 0; containerIndex < MAX_DL_TIMER_CONTAINER; containerIndex++)
		{
			ueDLContext_p->harqProcessId[containerIndex][sfIndex] = INVALID_HARQ_ID;
			/* Its indicate pdcch is allocated or not. */
			ueDLContext_p->tddHarqTimerExpiryNode[containerIndex][sfIndex] = PNULL;
			ueDLContext_p->pdschTxMapContainer[containerIndex][sfIndex] = 0;
		}
		ueDLContext_p->tddDai[sfIndex] = 0;
		/* + SPS_TDD_Changes */
		ueDLContext_p->tddDaiSPS[sfIndex] = 0;
		/* - SPS_TDD_Changes */
		ueDLContext_p->tddDaiLastScheduled[sfIndex] = 0;
		ueDLContext_p->retxQueueNode_p[sfIndex] = PNULL;
	}

	/* + CA_TDD_HARQ_CHNAGES */      
	SET_HARQ_RETX_NODE(ueDLContext_p->tempRetxNode);
	ueDLContext_p->nackFlag[PCELL_SERV_CELL_IX] = FALSE;
	ueDLContext_p->nackFlag[START_SCELL_INDEX] = FALSE;
	/* - CA_TDD_HARQ_CHNAGES */      
#endif
	/* CA changes start */
	for (index = MAX_NUM_PCELL; index <= MAX_NUM_SCELL; index ++)
	{
		if (PNULL != ueDLContext_p->dlUeScellContext_p[index])
		{
			dlCQIInfo_p = &ueDLContext_p->dlUeScellContext_p[index]->ScelldlCQIInfo;
            macResetScellCQIInfo(ueDLContext_p->dlUeScellContext_p[index],dlCQIInfo_p
                                );
			if(TX_MODE_6 == ueDLContext_p->dlUeScellContext_p[index]->
					ScelldlMIMOInfo.transmissionMode)
			{
				ueDLContext_p->dlUeScellContext_p[index]->Scelltm6DCIFormat = 
					DCI_FORMAT_1A;

				if((((PERIODIC_CQI_REPORTING_MODE == ueDLContext_p->
									dlUeScellContext_p[index]->ScelldlCQIInfo.cqiTypeIndicator) &&
								(1 == ueDLContext_p->dlUeScellContext_p[index]->\
								 ScelldlCQIInfo.cqiRequestType)) ||
							(APERIODIC_CQI_REPORTING_MODE == ueDLContext_p->\
							 dlUeScellContext_p[index]->ScelldlCQIInfo.cqiTypeIndicator) ||
							(PERIODIC_APERIODIC_MODE == ueDLContext_p->\
							 dlUeScellContext_p[index]->ScelldlCQIInfo.\
							 cqiTypeIndicator)) && (INVALID_CODE_BOOK_INDEX != 
							 ueDLContext_p->dlUeScellContext_p[index]->ScelldlMIMOInfo.precodingIndex))
				{
					ueDLContext_p->dlUeScellContext_p[index]->Scelltm6DCIFormat 
						= DCI_FORMAT_1B;
					dlCQIInfo_p->wideBandPMIIndex = 
						ueDLContext_p->dlUeScellContext_p[index]->\
						ScelldlMIMOInfo.precodingIndex;
				}
			}

			/* + CA_TDD_HARQ_CHNAGES */      
#ifdef TDD_CONFIG

			dlUeScellContext_p  = ueDLContext_p->dlUeScellContext_p[index];

			for(sfIndex =0; sfIndex < MAX_SUB_FRAME; sfIndex++)
			{
				for (containerIndex = 0; containerIndex < MAX_DL_TIMER_CONTAINER; containerIndex++)
				{
					dlUeScellContext_p->\
						pdschTxMapContainerForSCell[containerIndex][sfIndex] =  0;
					dlUeScellContext_p->harqProcessSCellId[containerIndex][sfIndex] = 
						INVALID_HARQ_ID;
				}
				dlUeScellContext_p->tddDaiOfSCell[sfIndex] = 0;
				dlUeScellContext_p->tddDaiLastScheduledOfSCell[sfIndex] = 0;
			}
#endif
			/* -  CA_TDD_HARQ_CHANGES */
		}
	}
	/* CA changes end */
	return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : resetMacUEEntity
 * Inputs         : ueIndex - Is the index of UE for which MAC Reset is needed.
 *                  transactionId-  random id generated which should be same for
 *                                  request sent by RRC layer and response sent
 *                                  by MAC layer.
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Output         : None
 * Returns        : MAC_RESET_PENDING/MAC_FAILURE in Success or failure Scenario
 * Description    :  This API gets the DL and UL context put entry in MAC Reset
 *                   Queue.
 *****************************************************************************/

/*+COVERITY 5.3.0 - 32261*/
UInt32 resetMacUEEntity(UInt16 ueIndex, UInt16 transactionId
		/* CA changes Start */
		,InternalCellIndex internalCellIndex
		/* CA Changes end */
		)
/*-COVERITY 5.3.0 - 32261*/
{
	ULUEContext *ulUEContext_p = PNULL;
	DLUEContext *dlUEContext_p = PNULL;
	/* SPR 15909 fix start */
    tickType_t tti = 0;
    /* SPR 15909 fix end */
	/*+COVERITY 5.3.0 - 32261*/
	UInt32 resp = MAC_FAILURE;
	/*-COVERITY 5.3.0 - 32261*/
	ulUEContext_p = (ulUECtxInfoArr_g[ueIndex]).ulUEContext_p;
	dlUEContext_p = (dlUECtxInfoArr_g[ueIndex]).dlUEContext_p;

    /*SPR 20665 +*/
    if ( (ulUEContext_p && dlUEContext_p) && 
            (((FALSE == (dlUECtxInfoArr_g[ueIndex]).pendingDeleteFlag) &&
              (FALSE == (ulUECtxInfoArr_g[ueIndex]).pendingDeleteFlag)) ||
        /* SPR 3043 start */
             (TRUE == dlUECtxInfoArr_g[ueIndex].dlUEContext_p->tcrntiFlag)))
    /*SPR 20665 -*/
        /* SPR 3043 end */

	{
		tti = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
		putEntryInMacResetQueue(ueIndex, transactionId, tti);
		resp = MAC_RESET_PENDING;
	}
	return resp; 
}

/****************************************************************************
 * Function Name  : updateCrntiToIndexTable 
 * Inputs         : changeCrntiInfo - Is a structure variable contains the
 information ueIndex,new and old crnti.
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Output         : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE in Success or failure Scenario
 * Description    : This API updates the Crnti to ueIndex mapping table 
 *                   
 *****************************************************************************/
/*CA Changes start  */
 MacRetType updateCrntiToIndexTable(ChangeCrntiInfo changeCrntiInfo, InternalCellIndex internalCellIndex)
	/*CA Changes end  */
{
	RNTIInfo *oldRntiInfo_p = PNULL;
	RNTIInfo *newRntiInfo_p = PNULL;
	TempCRNTICtx *tempCRNTICtx_p = PNULL;
	DLUEContext *dlUEContext_p 
		= (dlUECtxInfoArr_g[changeCrntiInfo.ueIndex]).dlUEContext_p;
	ULUEContext *ulUEContext_p 
		= (ulUECtxInfoArr_g[changeCrntiInfo.ueIndex]).ulUEContext_p;
	UInt32 n = 0;

	/*CA Changes start  */
	oldRntiInfo_p = getUeIdxFromRNTIMap(changeCrntiInfo.oldCrnti, internalCellIndex);
	newRntiInfo_p = getUeIdxFromRNTIMap(changeCrntiInfo.newCrnti, internalCellIndex);
	/*CA Changes end  */
	/* CA Changes start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */

	LOG_MAC_MSG(MAC_DBG_LOG_ID,LOGWARNING,MAC_L1_INF,
		 GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
		 LINE_NUMBER, changeCrntiInfo.ueIndex, changeCrntiInfo.oldCrnti, changeCrntiInfo.newCrnti, oldRntiInfo_p->rntiFlag,
		 DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
		 FUNCTION_NAME,"INVALID_NODE");

	if((PNULL == oldRntiInfo_p)
		|| (PNULL == newRntiInfo_p))
	{
		//error
		LOG_MAC_MSG(MAC_DBG_LOG_ID,LOGERROR,MAC_L1_INF,
			 GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			 LINE_NUMBER, (PNULL == oldRntiInfo_p), (PNULL == newRntiInfo_p), 0,0,
			 DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			 FUNCTION_NAME,"RNTI not found");

		return MAC_FAILURE;
	}
	else if (changeCrntiInfo.ueIndex != oldRntiInfo_p->index)
	{
		//error
		LOG_MAC_MSG(MAC_DBG_LOG_ID,LOGERROR,MAC_L1_INF,
			 GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			 LINE_NUMBER, changeCrntiInfo.ueIndex, changeCrntiInfo.oldCrnti, changeCrntiInfo.newCrnti, oldRntiInfo_p->index,
			 DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			 FUNCTION_NAME,"UEindex not match old rnti");
		return MAC_FAILURE;
	}
	else
	{
		/*Copy the last TA value received during RACH in DL Ctxt*/
		/* TA_CHG */
		tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->
			tempCRNTICtxArray[newRntiInfo_p->index].tempCRNTICtx_p;
		if (PNULL != tempCRNTICtx_p)
		{
            /* SPR 15682 fix start */
            /* Refer Spec 36.213 Section 4.2.3 for the calculations
	     * NTA = TA ×16 , where TA = 0, 1, 2, ..., 1282*/
            dlUEContext_p->timingAdvanceAbsValue = 
                (tempCRNTICtx_p->timingAdvanceAbs << 4);
            /* SPR 15682 fix end */
			/*CA Changes start  */
			deleteTempCRNTICtx(tempCRNTICtx_p->tcRNTI, internalCellIndex);
			/*CA Changes end  */
		}
		/* SPR 3271 changes start */
		/* Initialize blerInfoForPUCCH */
		if(dlUEContext_p)
		{
			dlUEContext_p->lastSINRCoolOffCounter = 0;
			dlUEContext_p->currPowerCorrectionForPucch = 0;
			dlUEContext_p->powerControlPucchTpcValue = 0;
			dlUEContext_p->averageSinrForPucch = 
				/*CA Changes start  */
				(macReconfigSchedulerParamsDL_gp[internalCellIndex] + 
				 schParamsIndexDL_g[internalCellIndex])->deltaSinrToTPCMapForPUCCH.pucchSINRTarget; 
			/*CA Changes start  */
			initPucchBlerContext(dlUEContext_p, internalCellIndex);
			/*CA Changes end  */
			resetTpcPdcchConfigInfoForPucch (
                    /* +- SPR 17777 */
					dlUEContext_p, internalCellIndex);
			/*CA Changes end  */
			/* + SPR 4455, 5358 Start */
			dlUEContext_p->ueSearchSpaceInfo_p = (&newRntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex][0]);
			/* - SPR 4455, 5358 End */
		}
		/* SPR 3271 changes end */
		/* TA_CHG */
		if(ulUEContext_p)
		{
			ulUEContext_p->currPowerCorrectionForPusch = 0;
			ulUEContext_p->prevPowerCorrectionForPusch = 0;
			ulUEContext_p->powerControlPuschTPCValue = 0;
			/* SPR 3271 changes start */
			ulUEContext_p->mcsIndex = ulUEContext_p->initialMcs;
			/*SPR 3600 Start*/
			/* Cyclomatic_complexity_changes_start */
			getUlModSchemeFromMcsIndex(ulUEContext_p->mcsIndex,\
					&(ulUEContext_p->modulationScheme),\
					cellParams_p->macEnable64QAM,\
					ulUEContext_p->ueCategory);
			/* Cyclomatic_complexity_changes_end */
			/*SPR 3600 End*/
			/*CA Changes start  */
			ulUEContext_p->pathLoss = (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])-> \
						  alphaBasedPathLossToTargetSINRMap.defaultPathLoss;
			/*CA Changes end  */

			/* ICIC changes start */
			/*CA Changes start  */
			updateULPathLoss(changeCrntiInfo.ueIndex,ulUEContext_p->pathLoss, internalCellIndex);
			/*CA Changes end  */
			/* ICIC changes end */
			for(n = 0; n < MAX_SUB_FRAME; n++)
			{
				ulUEContext_p->powerControlDeltaTFAndLogFactor[n] = 0;
			}
			/*CA Changes start  */
			initUEULCSIContext(ulUEContext_p,ulUEContext_p->initialMcs, internalCellIndex);
			resetTpcPdcchConfigInfoForPusch (
                    /* +- SPR 17777 */
					ulUEContext_p,
					internalCellIndex);
			/*CA Changes end  */
			/* SPR 3271 changes end */
			/* + SPR 4455, 5358 Start */
			ulUEContext_p->ueSearchSpaceInfo_p = (&newRntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex][0]);
			/* - SPR 4455, 5358 End */
            /* + SPR 21811 Fix */
            ulUEContext_p->firstSRSIndRecvd = FALSE;
            /* - SPR 21811 Fix */
		}
		/*pdcch changes start*/
		setUeSearchSpaceInfoforScell(ulUEContext_p,dlUEContext_p,newRntiInfo_p,internalCellIndex );
		/*pdcch changes end*/
		/*Update the Info in newCrnti*/
		newRntiInfo_p->index = changeCrntiInfo.ueIndex;
		newRntiInfo_p->rntiFlag = oldRntiInfo_p->rntiFlag;

		if (CRNTI == newRntiInfo_p->rntiFlag)
		{
			macCrntiUeNum_g[internalCellIndex]++;
		}

		/*Free the old crnti to free crnti pool so that it can be allocated 
		  to some other UE*/

		/*CA Changes start  */
		if(MAC_FAILURE == freeRNTI(changeCrntiInfo.oldCrnti, internalCellIndex))
			/*CA Changes end  */
		{
			lteWarning ("freeRNTI() failed \n");
		}

		return MAC_SUCCESS;
	}
	//return MAC_FAILURE;
}

/****************************************************************************
 * Function Name  : configureTpcPdcchInfoForPusch
 * Inputs         : rnti - indicates the tpc rnti configured for UE
 *                  tpcBitMapIndex - indicated the tpc index configured for UE.
 *                  dciFormat - indicates whether tpc rnti is configured with 
 *                  DCI FORMAT 3/3A.
 *                  ueContext_p - Pointer to UE context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Output         : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE in Success or failure Scenario
 * Description    : This API updates updates the tpc info in ue context and 
 *                  tpc rnti corresponding to the tpc rnti configured for the 
 *                  UE.
 *****************************************************************************/
STATIC  MacRetType configureTpcPdcchInfoForPusch (
		UInt16 rnti,
		UInt8 tpcBitMapIndex,
		UInt8 dciFormat,
		ULUEContext *ueContext_p
		,InternalCellIndex internalCellIndex)
{
	UInt8 tpcRntiIndex = 0;
	MacRetType retVal = MAC_FAILURE;
	TpcRntiContext *tpcRntiContext_p = PNULL;

	/* DCI Format 3A changes start*/
	ConfiguredUEListNode * configuredUeNode_p = PNULL;
	/* DCI Format 3A changes end*/

	/* CA changes Start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */
	if ( ueContext_p->dedicatedULPowerControlInfo.accumulationEnabled == TRUE)
	{
		tpcRntiIndex = 
			rnti - cellParams_p->tpcRntiRange.startTpcRntiPusch;
		tpcRntiContext_p = (tpcPuschRntiContext_gp[internalCellIndex] + tpcRntiIndex);
		if (tpcRntiContext_p->configuredCount)
		{
			if (tpcRntiContext_p->isConfiguredForDciFormat3Or3A !=
					dciFormat)
			{
                LOG_MAC_ERROR(MAC_RRC_INF,"Invalid dciFormat %u for 3,3a UE %u\n",
                        dciFormat,ueContext_p->ueIndex);
				return MAC_FAILURE;
			}
			if (((tpcRntiContext_p->isConfiguredForDciFormat3Or3A == DCI_FORMAT_3A) &&
						(tpcRntiContext_p->configuredCount == MAX_TPC_INDEX_FOR_3A)) ||
					((tpcRntiContext_p->isConfiguredForDciFormat3Or3A == DCI_FORMAT_3) &&
					 (tpcRntiContext_p->configuredCount == MAX_TPC_INDEX_FOR_3)))
			{
                LOG_MAC_ERROR(MAC_RRC_INF,"Invalid 3Or3A UE %u\n",ueContext_p->ueIndex);
				return MAC_FAILURE;
			}
		}
		/* DCI Format 3A changes start*/
		if (PNULL == (configuredUeNode_p = addEntryInConfiguredUeList(ueContext_p->ueIndex, 
						&(tpcRntiContext_p->configuredUeList))))
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"Failure in addEntryInConfiguredUeList UE %u\n",
                    ueContext_p->ueIndex);
			return MAC_FAILURE;
		}
		ueContext_p->tpcInfoForPusch.configuredUeListNodeInTpcRntiContext_p = configuredUeNode_p;
		/* DCI Format 3A changes end*/
		tpcRntiContext_p->configuredCount++;
		/* + Coverity 54160 */
		tpcRntiContext_p->isConfiguredForDciFormat3Or3A = (DCIFormat) dciFormat;
        /* +- SPR 22354 */
		ueContext_p->tpcInfoForPusch.tpcRnti = rnti;
		ueContext_p->tpcInfoForPusch.tpcRntiIndex = tpcRntiIndex;
		ueContext_p->tpcInfoForPusch.tpcBitMapIndex = tpcBitMapIndex;
		/* SPR 3271 changes start */
		ueContext_p->tpcInfoForPusch.lastAppliedTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
		ueContext_p->tpcInfoForPusch.tpcTriggeredValue = TPC_NO_CHANGE;
		ueContext_p->tpcInfoForPusch.triggeredUeListNodeInTpcRntiContext_p = PNULL;
		ueContext_p->tpcInfoForPusch.lastTpcSinrCoolOffCounter = 0;
		/* SPR 3271 changes end */

		retVal = MAC_SUCCESS;
	}
	return retVal;
}

/****************************************************************************
 * Function Name  : configureTpcPdcchInfoForPucch
 * Inputs         : rnti - indicates the tpc rnti configured for UE
 *                  tpcBitMapIndex - indicated the tpc index configured for UE.
 *                  dciFormat - indicates whether tpc rnti is configured with 
 *                  DCI FORMAT 3/3A.
 *                  ueContext_p - Pointer to UE context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Output         : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE in Success or failure Scenario
 * Description    : This API updates updates the tpc info in ue context and 
 *                  tpc rnti corresponding to the tpc rnti configured for the 
 *                  UE.
 *****************************************************************************/
STATIC  MacRetType configureTpcPdcchInfoForPucch (
		UInt16 rnti,
		UInt8 tpcBitMapIndex,
		UInt8 dciFormat,
		DLUEContext *ueContext_p
		/* CA changes Start */
		,InternalCellIndex internalCellIndex)
/* CA Changes end */
{
	UInt8 tpcRntiIndex = 0;
	TpcRntiContext *tpcRntiContext_p = PNULL;

	/* DCI Format 3A changes start*/
	ConfiguredUEListNode * configuredUeNode_p = PNULL;
	/* DCI Format 3A changes end*/

	/* CA changes Start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */
	tpcRntiIndex = 
		rnti - cellParams_p->tpcRntiRange.startTpcRntiPucch;
	tpcRntiContext_p = tpcPucchRntiContext_gp[internalCellIndex] + tpcRntiIndex;
	if (tpcRntiContext_p->configuredCount)
	{
		if (tpcRntiContext_p->isConfiguredForDciFormat3Or3A !=
				dciFormat)
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"Invalid dciFormat dciFormat %u for UE %u\n",
                    dciFormat,ueContext_p->ueIndex);
			return MAC_FAILURE;
		}
		if (((tpcRntiContext_p->isConfiguredForDciFormat3Or3A == DCI_FORMAT_3A) &&
					(tpcRntiContext_p->configuredCount == MAX_TPC_INDEX_FOR_3A)) ||
				((tpcRntiContext_p->isConfiguredForDciFormat3Or3A == DCI_FORMAT_3) &&
				 (tpcRntiContext_p->configuredCount == MAX_TPC_INDEX_FOR_3)))
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"Invalid tpc configuration for 3,3a UE %u\n",
                    ueContext_p->ueIndex);
			return MAC_FAILURE;
		}
	}
	/* DCI Format 3A changes start*/
	if (PNULL == (configuredUeNode_p = addEntryInConfiguredUeList(ueContext_p->ueIndex, 
					&(tpcRntiContext_p->configuredUeList))))
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"Failure in addEntryInConfiguredUeList for UE %u\n",
                ueContext_p->ueIndex);
		return MAC_FAILURE;
	}
	ueContext_p->tpcInfoForPucch.configuredUeListNodeInTpcRntiContext_p = configuredUeNode_p;
	/* DCI Format 3A changes end*/
	tpcRntiContext_p->configuredCount++;
	/* + Coverity 54163 */
	tpcRntiContext_p->isConfiguredForDciFormat3Or3A = (DCIFormat) dciFormat;
	/* - Coverity 54163 */
	ueContext_p->tpcInfoForPucch.tpcRntiIndex = tpcRntiIndex;
	ueContext_p->tpcInfoForPucch.tpcBitMapIndex = tpcBitMapIndex;
	ueContext_p->tpcInfoForPucch.tpcRnti = rnti;
	/* SPR 3271 changes start */
	ueContext_p->tpcInfoForPucch.lastAppliedTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
	ueContext_p->tpcInfoForPucch.tpcTriggeredValue = TPC_NO_CHANGE;
	ueContext_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p = PNULL;
	/* SPR 3271 changes end */

	return MAC_SUCCESS;
}

/*SPR 4503 CHANGES*/
/****************************************************************************
 * Function Name  : deleteTpcPdcchConfigInfoForPucch
 * Inputs         : rnti - indicates the tpc rnti configured for UE
 *                  tpcBitMapIndex - indicated the tpc index configured for UE.
 *                  releaseType - Indicates whether is triggered by Reconfigure
 *                  request or delete Ue Request.
 *                  ueContext_p - Pointer to UE context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Output         : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE in Success or failure Scenario
 * Description    : This API updates delete/free the tpc info in ue context and 
 *                  tpc rnti corresponding to the tpc rnti configured for the 
 *                  UE.
 *****************************************************************************/
 MacRetType deleteTpcPdcchConfigInfoForPucch(
		UInt8 releaseType,
        /* +- SPR 17777 */
		DLUEContext *ueContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
	UInt8 tpcRntiIndex = 0;
	TpcRntiContext *tpcRntiContext_p = PNULL;
	TriggeredUEListNode * listNode_p = PNULL;

	/* DCI Format 3A changes*/
	ConfiguredUEListNode * configuredListNode_p = PNULL;
	/* DCI Format 3A changes end*/
	if (ueContext_p->tpcInfoForPucch.tpcRnti == INVALID_TPC_RNTI)
	{
		return MAC_SUCCESS;
	}
	tpcRntiIndex = ueContext_p->tpcInfoForPucch.tpcRntiIndex;
	tpcRntiContext_p = tpcPucchRntiContext_gp[internalCellIndex] + tpcRntiIndex;

	/* DCI Format 3A changes start*/
	/* Get configuredUeListNodeInTpcRntiContext_p from dlUeContext_p */
	configuredListNode_p = ueContext_p->tpcInfoForPucch.configuredUeListNodeInTpcRntiContext_p;
	if (configuredListNode_p)
	{
		/* Delete node configuredUeListNodeInTpcRntiContext_p from configuredUeList */
		deleteEntryFromConfiguredUeList(configuredListNode_p, tpcRntiContext_p);
		ueContext_p->tpcInfoForPucch.configuredUeListNodeInTpcRntiContext_p = PNULL;
		tpcRntiContext_p->configuredCount--;
	}
	else
	{
		LOG_MAC_MSG(MAC_INVALID_NODE,LOGWARNING,MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),tpcRntiContext_p->configuredCount,internalCellIndex, 
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,"INVALID_NODE");

		return MAC_FAILURE;
	}
	/* DCI Format 3A changes end*/

	/* Get triggeredUeListNodeInTpcRntiContext_p from dlUeContext_p */
	listNode_p = ueContext_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p;
	/*SPR 9673 Fix Start*/
	if (listNode_p && tpcRntiContext_p->triggeredCount)
		/*SPR 9673 Fix End*/
	{
		tpcRntiContext_p->triggeredCount--;

		/* Delete node triggeredUeListNodeInTpcRntiContext_p from triggeredUeList */
		deleteEntryFromTriggeredUeList(listNode_p, tpcRntiContext_p);
		ueContext_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p = PNULL;

		/* Triggere Explicit DCI */
		if ((TPC_NO_CHANGE != ueContext_p->tpcInfoForPucch.tpcTriggeredValue) &&
				(tpcRntiContext_p->scheduledState != UNSET_SCHEDULED_STATE)
				&& (RECONFIG_RELEASE_TPC_REQ == releaseType ))
		{
			/*CA Changes start  */
			putEntryInExplicitDLDCIScheduleQueue(ueContext_p->ueIndex, internalCellIndex);
			/*CA Changes end  */
		}
	}
	/*SPR 9673 Fix Start*/
	else
	{
		ueContext_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p = PNULL;
	}
	/*SPR 9673 Fix End*/

	if (!tpcRntiContext_p->configuredCount)
	{
		tpcRntiContext_p->scheduledState = UNSET_SCHEDULED_STATE;
	}
	ueContext_p->tpcInfoForPucch.tpcRnti = INVALID_TPC_RNTI;
	return MAC_SUCCESS;
}
/*SPR 4503 CHANGES*/
/****************************************************************************
 * Function Name  : reconfigureTpcPdcchConfigInfoForPucch
 * Inputs         : rnti - indicates the tpc rnti configured for UE
 *                  tpcBitMapIndex - indicated the tpc index configured for UE.
 *                  dciFormat - indicates whether tpc rnti is configured with 
 *                  DCI FORMAT 3/3A.
 *                  release - Indicates whether tpc info needs to be configured
 *                  released.
 *                  ueContext_p - Pointer to UE context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Output         : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE in Success or failure Scenario
 * Description    : This API updates updates the tpc info in ue context and 
 *                  tpc rnti corresponding to the tpc rnti configured for the 
 *                  UE.
 *****************************************************************************/
 MacRetType reconfigureTpcPdcchConfigInfoForPucch(
		UInt8 release,
		UInt16 rnti,
		UInt8 tpcBitMapIndex,
		UInt8 dciFormat,
		DLUEContext *ueContext_p,
		/*CA Changes start */
		InternalCellIndex internalCellIndex)
/*CA Changes end */
{
	MacRetType retVal = MAC_SUCCESS;
	UInt8 tpcRntiIndex = 0;
	TpcRntiContext *tpcRntiContext_p = PNULL;

	/* DCI Format 3A changes*/
	ConfiguredUEListNode * configuredListNode_p = PNULL;
	/* DCI Format 3A changes end*/
	/* CA Changes start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */

	if ( release )
	{
		/*SPR 4503 CHANGES*/
        /* +- SPR 17777 */
		if(MAC_SUCCESS != deleteTpcPdcchConfigInfoForPucch(
					RECONFIG_RELEASE_TPC_REQ,
					/*CA Changes start  */
					ueContext_p, internalCellIndex))
			/*CA Changes end  */
        /* +- SPR 17777 */
		{
			return MAC_FAILURE;
		}

		/*SPR 4503 CHANGES*/
	}
	else if ( ueContext_p->tpcInfoForPucch.tpcRnti == INVALID_TPC_RNTI)
	{
		tpcRntiIndex = 
			rnti - cellParams_p->tpcRntiRange.startTpcRntiPucch;
		tpcRntiContext_p = tpcPucchRntiContext_gp[internalCellIndex] + tpcRntiIndex;
		if (tpcRntiContext_p->configuredCount)
		{
			if (tpcRntiContext_p->isConfiguredForDciFormat3Or3A !=
					dciFormat)
			{
				return MAC_FAILURE;
			}
			if (((tpcRntiContext_p->isConfiguredForDciFormat3Or3A == DCI_FORMAT_3A) &&
						(tpcRntiContext_p->configuredCount == MAX_TPC_INDEX_FOR_3A)) ||
					((tpcRntiContext_p->isConfiguredForDciFormat3Or3A == DCI_FORMAT_3) &&
					 (tpcRntiContext_p->configuredCount == MAX_TPC_INDEX_FOR_3)))
			{
				return MAC_FAILURE;
			}
		}
		/* DCI Format 3A changes start*/
		if (PNULL == (configuredListNode_p = addEntryInConfiguredUeList(ueContext_p->ueIndex, 
						&(tpcRntiContext_p->configuredUeList))))
		{
			return MAC_FAILURE;
		}
		ueContext_p->tpcInfoForPucch.configuredUeListNodeInTpcRntiContext_p = configuredListNode_p;
		/* DCI Format 3A changes end*/
		tpcRntiContext_p->configuredCount++;
		/* + Coverity 54160 */
		tpcRntiContext_p->isConfiguredForDciFormat3Or3A = (DCIFormat) dciFormat;
        /* +- SPR 22354 */
		ueContext_p->tpcInfoForPucch.tpcRnti = rnti;
		ueContext_p->tpcInfoForPucch.tpcRntiIndex = tpcRntiIndex;
		ueContext_p->tpcInfoForPucch.tpcBitMapIndex = tpcBitMapIndex;

		retVal = MAC_SUCCESS;
	}
	else if((ueContext_p->tpcInfoForPucch.tpcRnti == rnti) &&
			(ueContext_p->tpcInfoForPucch.tpcBitMapIndex == tpcBitMapIndex))
	{
		/* SPR 3154 START */
		tpcRntiIndex =
			rnti - cellParams_p->tpcRntiRange.startTpcRntiPucch;
		tpcRntiContext_p = tpcPucchRntiContext_gp[internalCellIndex] + tpcRntiIndex;
		/* SPR 3154 END */
		if (tpcRntiContext_p->isConfiguredForDciFormat3Or3A !=
				dciFormat)
		{
			return MAC_FAILURE;
		}
	}
	else
	{
		retVal = MAC_FAILURE;
	}
    /*SPR 16855 +-*/
	LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] At HP ,release = %d tpcIndex = %d .tpcRnti = %d "\
			"isConfiguredForDci3Or3A = %d \n ",__func__,release,
			ueContext_p->tpcInfoForPucch.tpcRntiIndex,
			ueContext_p->tpcInfoForPucch.tpcRnti,
			tpcRntiContext_p->isConfiguredForDciFormat3Or3A);
    /*SPR 16855 +-*/

	return retVal;
}

/* SPR 3271 changes start */
/****************************************************************************
 * Function Name  : resetTpcPdcchConfigInfoForPucch
 * Inputs         : rnti - indicates the tpc rnti configured for UE
 *                  tpcBitMapIndex - indicated the tpc index configured for UE.
 *                  ueContext_p - Pointer to UE context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Output         : None
 * Returns        : None 
 * Description    : This API reset the tpc info in ue context and 
 *                  tpc rnti corresponding to the tpc rnti configured for the 
 *                  UE.
 *****************************************************************************/
 void resetTpcPdcchConfigInfoForPucch(
        /* +- SPR 17777 */
		DLUEContext *ueContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
	UInt8 tpcRntiIndex = 0;
	TpcRntiContext *tpcRntiContext_p = PNULL;
	TriggeredUEListNode * listNode_p = PNULL;
	if (ueContext_p->tpcInfoForPucch.tpcRnti == INVALID_TPC_RNTI)
	{
		return;
	}
	/* DCI Format 3A changes start*/
	/* Resetting the fakeTpcValue*/
	ueContext_p->tpcInfoForPucch.fakeTpcValue = FAKE_TPC_NO_VALUE;
	/* DCI Format 3A changes end*/

	tpcRntiIndex = ueContext_p->tpcInfoForPucch.tpcRntiIndex;
	tpcRntiContext_p = tpcPucchRntiContext_gp[internalCellIndex] + tpcRntiIndex;
	/* Get triggeredUeListNodeInTpcRntiContext_p from dlUeContext_p */
	listNode_p = ueContext_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p;
	if (listNode_p)
	{
		tpcRntiContext_p->triggeredCount--;

		/* Delete node triggeredUeListNodeInTpcRntiContext_p from triggeredUeList */
		deleteEntryFromTriggeredUeList(listNode_p, tpcRntiContext_p);  
		ueContext_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p = PNULL; 
		ueContext_p->tpcInfoForPucch.tpcTriggeredValue = TPC_NO_CHANGE; 
		ueContext_p->tpcInfoForPucch.lastAppliedTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
	}
}

/****************************************************************************
 * Function Name  : resetTpcPdcchConfigInfoForPusch
 * Inputs         : rnti - indicates the tpc rnti configured for UE
 *                  tpcBitMapIndex - indicated the tpc index configured for UE.
 *                  ueContext_p - Pointer to UE context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Output         : None
 * Returns        : None 
 * Description    : This API reset the tpc info in ue context and 
 *                  tpc rnti corresponding to the tpc rnti configured for the 
 *                  UE.
 *****************************************************************************/
 void resetTpcPdcchConfigInfoForPusch(
        /* +- SPR 17777 */
		ULUEContext *ueContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
	UInt8 tpcRntiIndex = 0;
	TpcRntiContext *tpcRntiContext_p = PNULL;
	TriggeredUEListNode * listNode_p = PNULL;
	if (ueContext_p->tpcInfoForPusch.tpcRnti == INVALID_TPC_RNTI)
	{
		return;
	}
	/* DCI Format 3A changes start*/
	/*Resetting the fakeTpcValue*/
	ueContext_p->tpcInfoForPusch.fakeTpcValue = FAKE_TPC_NO_VALUE;
	/* DCI Format 3A changes end*/

	/* SPR 4609_4610 Start */
	ueContext_p->tpcInfoForPusch.maxPowerReachedFlag = FALSE;
	ueContext_p->tpcInfoForPusch.minPowerReachedFlag = FALSE;
	/* SPR 4609_4610 End */

	tpcRntiIndex = ueContext_p->tpcInfoForPusch.tpcRntiIndex;
	tpcRntiContext_p = tpcPuschRntiContext_gp[internalCellIndex] + tpcRntiIndex;
	/* Get triggeredUeListNodeInTpcRntiContext_p from dlUeContext_p */
	listNode_p = ueContext_p->tpcInfoForPusch.triggeredUeListNodeInTpcRntiContext_p;
	if (listNode_p)
	{
		tpcRntiContext_p->triggeredCount--;

		/* Delete node triggeredUeListNodeInTpcRntiContext_p from triggeredUeList */
		deleteEntryFromTriggeredUeList(listNode_p, tpcRntiContext_p);  
		ueContext_p->tpcInfoForPusch.triggeredUeListNodeInTpcRntiContext_p = PNULL; 
		ueContext_p->tpcInfoForPusch.tpcTriggeredValue = TPC_NO_CHANGE; 
		ueContext_p->tpcInfoForPusch.lastAppliedTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
		ueContext_p->tpcInfoForPusch.lastTpcSinrCoolOffCounter = 0;
	}
}

/* SPR 3271 changes end */
/*SPR 4503 CHANGES*/
/****************************************************************************
 * Function Name  : deleteTpcPdcchConfigInfoForPusch
 * Inputs         : rnti - indicates the tpc rnti configured for UE
 *                  tpcBitMapIndex - indicated the tpc index configured for UE.
 *                  ueContext_p -  UE Context
 *                  releaseType - Indicates whether is triggered by Reconfigure
 *                  request or delete Ue Request.
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Output         : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE in Success or failure Scenario
 * Description    : This API updates delete/free the tpc info in ue PUSCH context, 
 *                  tpc rnti corresponding to the tpc rnti configured for the 
 *                  UE.
 *****************************************************************************/
 MacRetType deleteTpcPdcchConfigInfoForPusch(
		UInt8 releaseType,
        /* +- SPR 17777 */
		ULUEContext *ueContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
	UInt8 tpcRntiIndex = 0;
	TpcRntiContext *tpcRntiContext_p = PNULL;
	TriggeredUEListNode * listNode_p = PNULL;

	/* DCI Format 3A changes*/
	ConfiguredUEListNode * configuredListNode_p = PNULL;

	if (ueContext_p->tpcInfoForPusch.tpcRnti == INVALID_TPC_RNTI)
	{
		return MAC_SUCCESS;
	}
	tpcRntiIndex = ueContext_p->tpcInfoForPusch.tpcRntiIndex;
	tpcRntiContext_p = tpcPuschRntiContext_gp[internalCellIndex] + tpcRntiIndex;
	/* DCI Format 3A changes start*/
	/* Get configuredUeListNodeInTpcRntiContext_p from dlUeContext_p */
	configuredListNode_p = ueContext_p->tpcInfoForPusch.configuredUeListNodeInTpcRntiContext_p;
	if (configuredListNode_p)
	{
		/* Delete node configuredUeListNodeInTpcRntiContext_p from configuredUeList */
		deleteEntryFromConfiguredUeList(configuredListNode_p, tpcRntiContext_p);
		ueContext_p->tpcInfoForPusch.configuredUeListNodeInTpcRntiContext_p = PNULL;
		tpcRntiContext_p->configuredCount--;
	}
	else
	{
		LOG_MAC_MSG(MAC_INVALID_NODE,LOGWARNING,MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),tpcRntiContext_p->configuredCount,internalCellIndex, 
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,"INVALID_NODE");
		return MAC_FAILURE;
	}
	/* DCI Format 3A changes end*/

	/* Get triggeredUeListNodeInTpcRntiContext_p from dlUeContext_p */
	listNode_p = ueContext_p->tpcInfoForPusch.triggeredUeListNodeInTpcRntiContext_p;
	if (listNode_p)
	{
		tpcRntiContext_p->triggeredCount--;
		/* Delete node triggeredUeListNodeInTpcRntiContext_p from triggeredUeList */
		deleteEntryFromTriggeredUeList(listNode_p, tpcRntiContext_p);
		ueContext_p->tpcInfoForPusch.triggeredUeListNodeInTpcRntiContext_p = PNULL;

		/* Triggere Explicit DCI */
		if ((TPC_NO_CHANGE != ueContext_p->tpcInfoForPusch.tpcTriggeredValue) &&
				(tpcRntiContext_p->scheduledState != UNSET_SCHEDULED_STATE)
				&& (RECONFIG_RELEASE_TPC_REQ== releaseType))
		{
			/* +DYNAMIC_ICIC */
			/*CA Changes start  */
			putEntryInExplicitULDCIAndPHRScheduleQueue(ueContext_p->ueIndex, UL_TPC_WITHOUT_DATA, internalCellIndex);
			/*CA Changes end  */
			/* -DYNAMIC_ICIC */
		}
	}
	if (!tpcRntiContext_p->configuredCount)
	{
		tpcRntiContext_p->scheduledState = UNSET_SCHEDULED_STATE;
	}
	ueContext_p->tpcInfoForPusch.tpcRnti = INVALID_TPC_RNTI;

	return MAC_SUCCESS;

}
/*SPR 4503 CHANGES*/

/****************************************************************************
 * Function Name  : reconfigureTpcPdcchConfigInfoForPusch
 *INPUT           : release - Indicates whether tpc info needs to be configured
 *                  rnti - indicates the tpc rnti configured for UE
 *                  tpcBitMapIndex - indicated the tpc index configured for UE.
 *                  dciFormat - indicates whether tpc rnti is configured with 
 *                  DCI FORMAT 3/3A.
 *                  release - Indicates whether tpc info needs to be configured
 *                  released.
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Output         : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE in Success or failure Scenario
 * Description    : This API updates updates the tpc info in ue context and 
 *                  tpc rnti corresponding to the tpc rnti configured for the 
 *                  UE.
 *****************************************************************************/
 MacRetType reconfigureTpcPdcchConfigInfoForPusch(
		UInt8 release,
		UInt16 rnti,
		UInt8 tpcBitMapIndex,
		/* Coverity 54156 Fix Start */
		DCIFormat dciFormat,
		/* Coverity 54156 Fix End */
		ULUEContext *ueContext_p,
		InternalCellIndex internalCellIndex)
{
	MacRetType retVal = MAC_SUCCESS;
	UInt8 tpcRntiIndex = 0;
	TpcRntiContext *tpcRntiContext_p = PNULL;
	/* CA Changes start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */

	/* DCI Format 3A changes*/
	ConfiguredUEListNode * configuredListNode_p = PNULL;
	/* DCI Format 3A changes end*/


	if ( release )
	{
		/*SPR 4503 CHANGES*/
        /* +- SPR 17777 */
		if(MAC_SUCCESS != deleteTpcPdcchConfigInfoForPusch(
					RECONFIG_RELEASE_TPC_REQ,
					ueContext_p,
					internalCellIndex))
        /* +- SPR 17777 */
		{
			return MAC_FAILURE;
		}
		/*SPR 4503 CHANGES*/
	}
	else if ( ueContext_p->tpcInfoForPusch.tpcRnti == INVALID_TPC_RNTI)
	{
		if (ueContext_p->dedicatedULPowerControlInfo.accumulationEnabled == 
				TRUE)
		{
			tpcRntiIndex = 
				rnti - cellParams_p->tpcRntiRange.startTpcRntiPusch;
			tpcRntiContext_p = tpcPuschRntiContext_gp[internalCellIndex] + tpcRntiIndex;

			if (tpcRntiContext_p->configuredCount)
			{
				if (tpcRntiContext_p->isConfiguredForDciFormat3Or3A !=
						dciFormat)
				{
					return MAC_FAILURE; 
				}

				if (((tpcRntiContext_p->isConfiguredForDciFormat3Or3A == DCI_FORMAT_3A) &&
							(tpcRntiContext_p->configuredCount == MAX_TPC_INDEX_FOR_3A)) ||
						((tpcRntiContext_p->isConfiguredForDciFormat3Or3A == DCI_FORMAT_3) && 
						 (tpcRntiContext_p->configuredCount == MAX_TPC_INDEX_FOR_3)))
				{
					return MAC_FAILURE;
				}
			}

			/* DCI Format 3A changes start*/
			if (PNULL == (configuredListNode_p = addEntryInConfiguredUeList(ueContext_p->ueIndex, 
							&(tpcRntiContext_p->configuredUeList))))
			{
				return MAC_FAILURE;
			}
			ueContext_p->tpcInfoForPusch.configuredUeListNodeInTpcRntiContext_p = configuredListNode_p;
			/* DCI Format 3A changes end*/
			tpcRntiContext_p->configuredCount++;
            /* +- SPR 22354 */
			tpcRntiContext_p->isConfiguredForDciFormat3Or3A = dciFormat;
			ueContext_p->tpcInfoForPusch.tpcRnti = rnti;
			ueContext_p->tpcInfoForPusch.tpcRntiIndex = tpcRntiIndex;
			ueContext_p->tpcInfoForPusch.tpcBitMapIndex = tpcBitMapIndex;

			retVal = MAC_SUCCESS;
		}
		else
		{
			retVal = MAC_FAILURE;
		}
	}
	else if ((ueContext_p->tpcInfoForPusch.tpcRnti == rnti) &&
			(ueContext_p->tpcInfoForPusch.tpcBitMapIndex == tpcBitMapIndex))
	{
		tpcRntiIndex = 
			rnti - cellParams_p->tpcRntiRange.startTpcRntiPusch;
		tpcRntiContext_p = tpcPuschRntiContext_gp[internalCellIndex] + tpcRntiIndex;

		if (tpcRntiContext_p->isConfiguredForDciFormat3Or3A !=
				dciFormat)
		{
			return MAC_FAILURE; 
		}
	}
	else
	{
		retVal = MAC_FAILURE;
	}

	LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] At HP (UL_RECONFIG_TPC_PDCCH_CONFIG_PUSCH) release = %d tpcIndex = %d .tpcRnti = %d "\
			"isConfiguredForDci3Or3A = %d \n ",__func__,release,
			ueContext_p->tpcInfoForPusch.tpcRntiIndex,
			ueContext_p->tpcInfoForPusch.tpcRnti,
			tpcRntiContext_p->isConfiguredForDciFormat3Or3A);


	return retVal;
}

/*SPR 16855 +-*/
/*Reconfig Changes Start */
/*****************************************************************************
 * Function Name  : reconfigurePUCCHSRSAndReports 
 * Inputs         : ulUeCtx_p - Pointer to the UL UE context
 *                  ueIndex - UE ID to reconfigure 
 *                  tempPucchConfigInfo_p - pointer to PUCCH context to be configured
 *                  tempsrsConfigInfo_p - Pointer to SRS context to be configured
 *                  dwlkUEInfo_p - Pointer to downlink UE context Info 
 *                  uplkUEInfo_p - Pointer to uplink UE context Info
 *                  ulReconfigUeReq_p -
 *                  currSFN - current System frame number
 *                  currSF - current Subframe number
 *                  reconfigInfo_p - Pointer for reconfig info changes to be 
 *                     filled.Info used to update the passive context in LP thread
 *                     after Response from HP thread
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None 
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This function will processs the PUCCH and SRS report 
 *                  calls the appropriate function based on report type
 *                  and configure it at time of MAC_RECONFIGURE_UE_ENTITY_REQ 
 *                  received from RRC
 *****************************************************************************/
MacRetType reconfigurePUCCHSRSAndReports( 
        /* +- SPR 17777 */
		UInt16                ueIndex,
		PucchConfig          *tempPucchConfigInfo_p,
		SRSConfigInfo        *tempsrsConfigInfo_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink   *uplkUEInfo_p,
		ULReconfigUeReq      *ulReconfigUeReq_p,
		UInt32 currSFN, UInt32 currSF, 
		UeContextForReconfig *reconfigInfo_p,
		InternalCellIndex internalCellIndex)
{
	/* Review comment fix start GG15 */
#ifdef FDD_CONFIG
	/* CA changes Start */
	if (MAC_FAILURE == macValidateUePucchResources(uplkUEInfo_p, internalCellIndex))
		/* CA Changes end */
	{
		LOG_MAC_MSG(MAC_VALIDATE_UE_PUCCH_RES_FAILURE_ID,
				LOGERROR,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				ueIndex,
				LINE_NUMBER,
				internalCellIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,
				"PUCCH Resources resquired fro the UE is very high");
		return MAC_FAILURE;
	}
#endif
	/* Review comment fix end GG15 */
	/* Cyclomatic Complexity changes - starts here */
	/***************************CQI Parameter Reconfiguration **************/
	if (MAC_SUCCESS != checkAndReconfigureCqiInfo (
                /* +- SPR 17777 */
				ueIndex, 
				tempPucchConfigInfo_p, 
				dwlkUEInfo_p, 
				uplkUEInfo_p, 
				ulReconfigUeReq_p,
				reconfigInfo_p,
				internalCellIndex))
	{
		return MAC_FAILURE;
	}

	/***************************SR Parameters Reconfiguration **************/
	if (MAC_SUCCESS != checkAndReconfigureSRInfo (
                /* +- SPR 17777 */
				ueIndex, 
				tempPucchConfigInfo_p, 
				uplkUEInfo_p, 
				ulReconfigUeReq_p,
				reconfigInfo_p,
				internalCellIndex))
	{
		return MAC_FAILURE;
	}
	/***************************SRS Parameters Reconfiguration **************/
	checkAndReconfigureSRSDedicatedConfig (
			ueIndex, 
			tempsrsConfigInfo_p,
			uplkUEInfo_p, 
			ulReconfigUeReq_p, 
			currSFN,
			currSF,
			reconfigInfo_p,
			internalCellIndex);

	/* Cyclomatic Complexity changes - ends here */
	return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : reconfigurePUCCHSRSAndReportsPassiveContext 
 * Inputs         : ulUeCtx_p - Pointer to the UL UE context
 *                  ueIndex - UE ID to reconfigure 
 *                  tempPucchConfigInfo_p - pointer to PUCCH context to be configured
 *                  tempsrsConfigInfo_p - Pointer to SRS context to be configured
 *                  dwlkUEInfo_p - Pointer to downlink UE context Info 
 *                  uplkUEInfo_p - Pointer to uplink UE context Info
 *                  currSFN - current System frame number
 *                  currSF - current Subframe number
 *                  reconfigInfo_p - Pointer for reconfig info changes done in 
 *                     current active context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None 
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This function will processs the PUCCH and SRS report 
 *                  calls the appropriate function based on report type
 *                  and reconfigure passive context after the confirmation from HP 
 *                  Thread
 *****************************************************************************/
MacRetType reconfigurePUCCHSRSAndReportsPassiveContext( 
		ULUEContext *ulUeCtx_p,
		UInt16                ueIndex,
		PucchConfig          *tempPucchConfigInfo_p,
		SRSConfigInfo        *tempsrsConfigInfo_p,
        /* +- SPR 17777 */
		UeContextForUplink   *uplkUEInfo_p,
		UInt32 currSFN, UInt32 currSF, 
		UeContextForReconfig *reconfigInfo_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
	if (MAC_SUCCESS != reconfigurePCellCqiRiPassiveContext(
				ulUeCtx_p,
				ueIndex, 
				tempPucchConfigInfo_p, 
                /* +- SPR 17777 */
				uplkUEInfo_p, 
				reconfigInfo_p,
				internalCellIndex))
	{
		return MAC_FAILURE;
	}
	if (MAC_SUCCESS != reconfigureSRPassiveContext (
				ulUeCtx_p,
                /* +- SPR 17777 */
				ueIndex, 
				tempPucchConfigInfo_p, 
				uplkUEInfo_p, 
				reconfigInfo_p,
				internalCellIndex))
	{
		return MAC_FAILURE;
	}

	reconfigureSRSPassiveContext(
			ueIndex, 
			tempsrsConfigInfo_p,
			uplkUEInfo_p, 
			currSFN,
			currSF,
			reconfigInfo_p,
			internalCellIndex);
	return MAC_SUCCESS;
}

/* Cyclomatic Complexity changes - starts here */
/*****************************************************************************
 * Function Name  : checkAndReconfigureCqiInfo
 * Inputs         : ulUeCtx_p - Pointer to UL UE context to be updated
 *                  ueIndex - Stores UE index
 *                  tempPucchConfigInfo_p - Pointer to PucchConfig structure
 *                  dwlkUEInfo_p - Pointer to DL UE context info received
 *                  uplkUEInfo_p - Pointer to UL UE context info received
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : ulReconfigUeReq_p - Pointer to UL Reconfig Request
 *                  reconfigInfo_p - Pointer to Reconfig UE info changes
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks and reconfigures CQI Information
 *****************************************************************************/
/*Reconfig Changes Start */
STATIC  MacRetType checkAndReconfigureCqiInfo( 
        /* +- SPR 17777 */
		UInt16               ueIndex,
		PucchConfig          *tempPucchConfigInfo_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink   *uplkUEInfo_p,
		ULReconfigUeReq      *ulReconfigUeReq_p,
		UeContextForReconfig *reconfigInfo_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
/*Reconfig Changes End */
{
	ULCQIReqInfo                *ulCqiReqInfo_p             = PNULL; 
	ULCQIPeriodicReqInfo        *cqiPeriodicReqInfo_p       = PNULL; 
	/*+COVERITY 5.3.0 - 32255/32256*/
	ULReconfigCQIRIMsgType       cqiRiReconfigRetValue      = UL_RECONFIG_DEFAULT_RECONFIG_MSG;
	ULReconfigPeriodicCQIType    cqiPeriodicMsg             = UL_RECONFIG_NO_CQI_SUBBAND_RI_MSG;
	/*-COVERITY 5.3.0 - 32255/32256*/
	MacRetType                   reconfigCQIRIRetValue      = MAC_SUCCESS;
	/* +SPR_10707 Changes */
	PeriodicFeedbackType periodicFeedBack = 
		uplkUEInfo_p->cqiformatIndicatorPeriodic;
	/* -SPR_10707 Changes */

	if (ulReconfigUeReq_p)
	{
		ulCqiReqInfo_p = &ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.ulCqiReqInfo;
		cqiPeriodicReqInfo_p = &ulCqiReqInfo_p->cqiPeriodicReqInfo; 
	}

	if ( uplkUEInfo_p->cqiRequestType == 1)
	{
		/* SPR 2617 Start*/
		/*Reconfig Changes Start */
		reconfigCQIRIRetValue = reConfigPCellRIAndCQIMap (
				/* PUCCH TDD ADAPTATION CHANGES Start */        
				ulUECtxInfoArr_g[ueIndex].ulUEContext_p,
				/* PUCCH TDD ADAPTATION CHANGES End */        
				dwlkUEInfo_p, 
				uplkUEInfo_p,
                /*SPR 16855 +-*/
				&cqiRiReconfigRetValue,
				reconfigInfo_p,
                /*SPR 16855 +-*/
				internalCellIndex );
		/*Reconfig Changes End */

		if(MAC_FAILURE == reconfigCQIRIRetValue)
		{
			return MAC_FAILURE;
		}    
		/* This PNULL check is added as this function is called second time at LP 
		 * after updating UE context at HP and at that time temporary 
		 * ulReconfigUeReq will not be updated */
		if( ulReconfigUeReq_p )
		{

			cqiPeriodicReqInfo_p->cqiRiReconfigMsg = cqiRiReconfigRetValue;

			/* Default configuration parameters for Periodic CQI 
			 * are always over written whenever 
			 * reconfiguration message is recieved */ 

			cqiPeriodicReqInfo_p->cqiRiReconfigMsg |= CQI_DEFAULT_PARAMETERS_IND;
			cqiPeriodicReqInfo_p->periodicFeedBackType = 
				tempPucchConfigInfo_p->cqiPmiRiInfo.periodicFeedBackType;
			cqiPeriodicReqInfo_p->periodicRepMode =  
				tempPucchConfigInfo_p->cqiPmiRiInfo.periodicRepMode;
			cqiPeriodicReqInfo_p->widebandPeriodicity = 
				tempPucchConfigInfo_p->cqiPmiRiInfo.widebandPeriodicity;
			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] AT LP cqiRiReconfigMsg = %d \n",
					__func__,cqiPeriodicReqInfo_p->cqiRiReconfigMsg);

			if (CQI_MAP_RECONFIGURE_IND & cqiPeriodicReqInfo_p->cqiRiReconfigMsg)
			{   
				cqiPeriodicReqInfo_p->cqiPeriodicity = tempPucchConfigInfo_p->cqiPeriodicity;
				/* Review comment fix GK9 start */
				cqiPeriodicReqInfo_p->cqiOffset =      tempPucchConfigInfo_p->cqiOffset;
				cqiPeriodicReqInfo_p->maxCQIWrapAround = 
					tempPucchConfigInfo_p->maxCQIWrapAround[
					longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex];
				/* Review comment fix GK9 end */
				/* +SPR_10707 Changes */
				if( PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack )
				{
					cqiPeriodicReqInfo_p->maxWbCQIWrapAround = 
						tempPucchConfigInfo_p->maxWbCQIWrapAround[
						longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex];
				}
				/* -SPR_10707 Changes */
				cqiPeriodicReqInfo_p->cqiPmiConfigIndex =
					tempPucchConfigInfo_p->cqiPmiConfigIndex;
				cqiPeriodicReqInfo_p->cqiPUCCHRBIndex = 
					uplkUEInfo_p->cqiPucchResourceIndex;
				ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap 
					/*CA Changes start  */
					= longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
				/*CA Changes end  */
			}
			if(RI_MAP_RECONFIGURE_IND & cqiPeriodicReqInfo_p->cqiRiReconfigMsg)
			{
				cqiPeriodicReqInfo_p->riConfigIndexInfo.riPeriodicity=
					tempPucchConfigInfo_p->riPeriodicity;
				/* Review comment fix GK9 start */
				cqiPeriodicReqInfo_p->riConfigIndexInfo.riOffset=
					tempPucchConfigInfo_p->riOffset;
				cqiPeriodicReqInfo_p->riConfigIndexInfo.maxRIWrapAround =
					tempPucchConfigInfo_p->maxRIWrapAround[
					longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex];
				/* Review comment fix GK9 end */
				cqiPeriodicReqInfo_p->riConfigIndexInfo.riConfigIndex =
					tempPucchConfigInfo_p->riConfigIndex;
				ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap 
					/*CA Changes start  */
					= longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
				/*CA Changes end  */
			}
			/*eicic changes*/
			/*Complexity Start*/
			updateCqiStats(cqiPeriodicReqInfo_p, tempPucchConfigInfo_p,internalCellIndex,periodicFeedBack,ulReconfigUeReq_p);
			updateRiStats(cqiPeriodicReqInfo_p, tempPucchConfigInfo_p,internalCellIndex,ulReconfigUeReq_p);
			/*Complexity End*/
			/*eicic changes*/ 
			if((CQI_RESOURCE_INDEX_RECONFIGURE_IND & cqiPeriodicReqInfo_p->cqiRiReconfigMsg) || 
					(RI_RESOURCE_INDEX_RECONFIGURE_IND & cqiPeriodicReqInfo_p->cqiRiReconfigMsg) ||
					(CQI_RESOURCE_INDEX_RECONFIGURE_IND2 & cqiPeriodicReqInfo_p->cqiRiReconfigMsg) || 
					(RI_RESOURCE_INDEX_RECONFIGURE_IND2 & cqiPeriodicReqInfo_p->cqiRiReconfigMsg))
			{
				ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap 
					/*CA Changes start  */
					= longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
				/*CA Changes end  */
				cqiPeriodicReqInfo_p->cqiPUCCHRBIndex = uplkUEInfo_p->cqiPucchResourceIndex;
			}    
			if(UL_RECONFIG_CQI_SUBBAND_RI_MSG == ulCqiReqInfo_p->cqiPeriodicReqInfo.cqiPeriodicMsg ||
					UL_RECONFIG_CQI_SUBBAND_K_MSG == ulCqiReqInfo_p->cqiPeriodicReqInfo.cqiPeriodicMsg)
			{
				cqiPeriodicReqInfo_p->cqiFormatPeriodicSubbandK.subbandCQI_k =
					tempPucchConfigInfo_p->cqiPmiRiInfo.k;
			}       

			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] At LP Side (UL_RECONFIG_PER_CQI_CONFIG_INFO(setup)), \n"\
					"cqiConfigMsg = %d CQIAperiodic Mode = %d cqiPeriodicMsg = %d cqiRiReconfigMsg = %d\n"\
					"cqiPeriodicity = %d, cqiPUCCHRBIndex = %d simultaneousAckNackAndCQI = %d cqiOffset = %d ,\n"\
					"periodicFeedBackType = %d k = %d periodicRepMode = %d cqiPmiConfigIndex = %d ,\n"\
					"activeIndexForPUCCHMap = %d pucchConfigMsg = %d Passive index = %d\n ",__func__,
					ulCqiReqInfo_p-> cqiConfigMsg,
					ulCqiReqInfo_p->cqiAperiodicInfo.cqiAperiodicMode,
					cqiPeriodicReqInfo_p->cqiPeriodicMsg,
					cqiPeriodicReqInfo_p->cqiRiReconfigMsg, 
					cqiPeriodicReqInfo_p->cqiPeriodicity,
					cqiPeriodicReqInfo_p->cqiPUCCHRBIndex,
					cqiPeriodicReqInfo_p->simultaneousAckNackAndCQI,
					cqiPeriodicReqInfo_p->cqiOffset,
					cqiPeriodicReqInfo_p->periodicFeedBackType, 
					cqiPeriodicReqInfo_p->k,cqiPeriodicReqInfo_p->periodicRepMode,
					cqiPeriodicReqInfo_p->cqiPmiConfigIndex,
					ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap,
					ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.pucchConfigMsg,
					/*CA Changes start  */
					longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex);
			/*CA Changes end  */

			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
					"[%s] At LP Side(UL_RECONFIG_PER_CQI_RI_CONFIG_INFO), "\
					"riPeriodicity = %d, riOffset = %d riConfigIndex = %d \n",__func__,
					cqiPeriodicReqInfo_p->riConfigIndexInfo.riPeriodicity,
					cqiPeriodicReqInfo_p->riConfigIndexInfo.riOffset,
					cqiPeriodicReqInfo_p->riConfigIndexInfo.riConfigIndex);
		}
	}
	else if ( uplkUEInfo_p->cqiRequestType == 0)
	{
		/* PUCCH TDD ADAPTATION CHANGES Start */        
		deletePCellPucchNodesForUE( tempPucchConfigInfo_p, WIDEBAND_CQI_REPORT, 
				internalCellIndex);
		deletePCellPucchNodesForUE( tempPucchConfigInfo_p, CQI_REPORT , 
				internalCellIndex);
		deletePCellPucchNodesForUE( tempPucchConfigInfo_p, RI_REPORT , 
				internalCellIndex);
		/* PUCCH TDD ADAPTATION CHANGES End */        

		if(ulReconfigUeReq_p)
		{
			cqiPeriodicMsg = ulCqiReqInfo_p->cqiPeriodicReqInfo.cqiPeriodicMsg;
			memSet(cqiPeriodicReqInfo_p, 0, sizeof(ULCQIPeriodicReqInfo));
			ulCqiReqInfo_p->cqiPeriodicReqInfo.cqiPeriodicMsg = cqiPeriodicMsg;

			ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap = 
				/*CA Changes start  */
				longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
			/*CA Changes end  */
		}

		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
				"[%s] At LP Side (UL_RECONFIG_PER_CQI_CONFIG_INFO (rel)), \n"\
				"cqiPeriodicity = %d, cqiOffset = %d ,\n"\
				"periodicFeedBackType = %d k = %d periodicRepMode = %d cqiPmiConfigIndex = %d ,\n"\
				"activeIndexForPUCCHMap = %d \n ",__func__,
				cqiPeriodicReqInfo_p->cqiPeriodicity,
				cqiPeriodicReqInfo_p->cqiOffset,
				cqiPeriodicReqInfo_p->periodicFeedBackType, 
				cqiPeriodicReqInfo_p->k,cqiPeriodicReqInfo_p->periodicRepMode,
				cqiPeriodicReqInfo_p->cqiPmiConfigIndex,
				ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap);
	}
	else if ( uplkUEInfo_p->cqiConfigRequestType == 0)
	{
		/* PUCCH TDD ADAPTATION CHANGES Start */  
		updateCQIMapForAbs_g[internalCellIndex] = LTE_TRUE;

		deletePCellPucchNodesForUE( tempPucchConfigInfo_p, WIDEBAND_CQI_REPORT, 
				internalCellIndex);
		deletePCellPucchNodesForUE( tempPucchConfigInfo_p, CQI_REPORT , 
				internalCellIndex);
		deletePCellPucchNodesForUE( tempPucchConfigInfo_p, RI_REPORT , 
				internalCellIndex);
		updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
	}

	if( (0 != uplkUEInfo_p->cqiMode) && (TRUE == uplkUEInfo_p->cqiinfoflag)   && (ulReconfigUeReq_p))
	{   
		if(tempPucchConfigInfo_p->cqiPmiRiInfoAperiodic.cqiMode != uplkUEInfo_p->cqiMode) 
		{
			ulCqiReqInfo_p->cqiAperiodicInfo.cqiAperiodicMode = dwlkUEInfo_p->cqiMode;
			/* CA changes start */
			if ((FALSE != uplkUEInfo_p->trigger1r10)&& (FALSE != uplkUEInfo_p->trigger2r10))
			{
				ulCqiReqInfo_p->cqiAperiodicInfo.trigger1r10 = uplkUEInfo_p->trigger1r10;
				ulCqiReqInfo_p->cqiAperiodicInfo.trigger2r10 = uplkUEInfo_p->trigger2r10;
			}
			/* CA changes end */
		}
		else
		{
			ulCqiReqInfo_p->cqiConfigMsg &=  UL_RECONFIG_CLEAR_CQI_APERIODIC_IND;

		}          

		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
				"[%s] At LP Side (UL_RECONFIG_APER_CQI_CONFIG_INFO setup), "\
				"CqiMode = %d \n",
				ulCqiReqInfo_p->cqiAperiodicInfo.cqiAperiodicMode);
	}   
	else if(TRUE != uplkUEInfo_p->cqiinfoflag && ulReconfigUeReq_p)
	{
		ulCqiReqInfo_p->cqiConfigMsg |= UL_RECONFIG_CQI_APERIODIC_IND; 
	} 
	return MAC_SUCCESS;
}
/*Complexity Start*/
/*****************************************************************************
 * Function Name  : updateCqiStats 
 * Inputs         : cqiPeriodicReqInfo_p - Pointer to Uplink CQI Periodic Info
 *                  tempPucchConfigInfo_p
 *                  internalCellIndex
 *                  periodicFeedBackType -  wideband/ subband 
 *                  ulReconfigUeReq_p
 * Outputs        : None
 * Returns        : None
 * Description    : Updates CQI Stats 
 *                  
 *****************************************************************************/
void updateCqiStats(ULCQIPeriodicReqInfo *cqiPeriodicReqInfo_p, 
		PucchConfig *tempPucchConfigInfo_p,
		InternalCellIndex internalCellIndex,
		PeriodicFeedbackType periodicFeedBack,
		ULReconfigUeReq *ulReconfigUeReq_p
		)
{

	if (CQI_MAP_RECONFIGURE_IND2 & cqiPeriodicReqInfo_p->cqiRiReconfigMsg)
	{   
		cqiPeriodicReqInfo_p->cqiPeriodicity_2 = tempPucchConfigInfo_p->cqiPeriodicity_2;

		cqiPeriodicReqInfo_p->cqiOffset_2 =      tempPucchConfigInfo_p->cqiOffset_2;
		cqiPeriodicReqInfo_p->maxCQIWrapAround2 = 
			tempPucchConfigInfo_p->maxCQIWrapAround2[
			longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex];

		if( PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack )
		{
			cqiPeriodicReqInfo_p->maxWbCQIWrapAround2 = 
				tempPucchConfigInfo_p->maxWbCQIWrapAround2[
				longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex];
		}

		cqiPeriodicReqInfo_p->cqiPmiConfigIndex_2 =
			tempPucchConfigInfo_p->cqiPmiConfigIndex_2;

		ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap 

			= longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;

	}
}
/*****************************************************************************
 * Function Name  : updateRiStats 
 * Inputs         : cqiPeriodicReqInfo_p - Pointer to Uplink CQI Periodic Info
 *                  tempPucchConfigInfo_p
 *                  internalCellIndex
 *                  ulReconfigUeReq_p
 * Outputs        : None
 * Returns        : None
 * Description    : Updates RI stats 
 *                  
 *****************************************************************************/
void updateRiStats(ULCQIPeriodicReqInfo *cqiPeriodicReqInfo_p, 
		PucchConfig *tempPucchConfigInfo_p,
		InternalCellIndex internalCellIndex,
		ULReconfigUeReq *ulReconfigUeReq_p
		)
{
	if(RI_MAP_RECONFIGURE_IND2 & cqiPeriodicReqInfo_p->cqiRiReconfigMsg)
	{
		cqiPeriodicReqInfo_p->riConfigIndexInfo.riPeriodicity_2=
			tempPucchConfigInfo_p->riPeriodicity_2;

		cqiPeriodicReqInfo_p->riConfigIndexInfo.riOffset_2=
			tempPucchConfigInfo_p->riOffset_2;
		cqiPeriodicReqInfo_p->riConfigIndexInfo.maxRIWrapAround2 =
			tempPucchConfigInfo_p->maxRIWrapAround2[
			longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex];

		cqiPeriodicReqInfo_p->riConfigIndexInfo.riConfigIndex_2 =
			tempPucchConfigInfo_p->riConfigIndex_2;
		ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap 

			= longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;

	}

}
/*Complexity End*/
/*****************************************************************************
 * Function Name  : checkAndReconfigureSRInfo 
 * Inputs         : ulUeCtx_p - UL UE context
 *                  dwlkUEInfo_p -
 *                  ueIndex
 *                  tempPucchConfigInfo_p - Pointer to PucchConfig structure
 *                  uplkUEInfo_p - Pointer to UL UE context
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : ulReconfigUeReq_p - Pointer to UL Reconfig Request
 *                  reconfigInfo_p - Pointer to Reconfig UE info changes
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks and reconfigures SR Information
 *****************************************************************************/
/*Reconfig Changes Start */
STATIC  MacRetType checkAndReconfigureSRInfo( 
        /* +- SPR 17777 */
		UInt16               ueIndex,
		PucchConfig          *tempPucchConfigInfo_p,
		UeContextForUplink   *uplkUEInfo_p,
		ULReconfigUeReq      *ulReconfigUeReq_p,
		UeContextForReconfig *reconfigInfo_p,
		/*CA Changes start  */
		InternalCellIndex cellIndex)
/*CA Changes end  */
/*Reconfig Changes End */
{
	RRCInfPUCCHConfigReqInfo    *rrcInfPUCCHConfigReqInfo_p = PNULL;
	SRConfigInfo                *srConfigInfo_p             = PNULL;
	MacRetType                   reconfigSRRetValue         = MAC_SUCCESS;
	/*+COVERITY 5.3.0 - 32257*/
	ULReconfigSRMsgType          srReconfigRetValue         = UL_RECONFIG_SR_MAP_MSG;
	/*-COVERITY 5.3.0 - 32257*/
	UInt16                       originalSrPeriodicity     = tempPucchConfigInfo_p->srPeriodicity;

	if (ulReconfigUeReq_p)
	{
		rrcInfPUCCHConfigReqInfo_p = &ulReconfigUeReq_p->rrcInfpucchConfigReqInfo;
		srConfigInfo_p             = &ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.srConfigInfo;
	}

	if (!uplkUEInfo_p->requestType)
	{
		if ((SHORT_SR_PERIODICITY < tempPucchConfigInfo_p->srPeriodicity) &&
				(tempPucchConfigInfo_p->pucchShortSRNodeHead_p[longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex])
		   )

		{
			/* PUCCH TDD ADAPTATION CHANGES Start */        
			deletePCellPucchNodesForUE( tempPucchConfigInfo_p, SCHEDULING_REQUEST,
					cellIndex );
			/* PUCCH TDD ADAPTATION CHANGES End */        
			if( ulReconfigUeReq_p)
			{
				ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap = 
					/*CA Changes start  */
					longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex;
				/*CA Changes end  */
				memSet((srConfigInfo_p), 0, sizeof(SRConfigInfo));
				srConfigInfo_p->mapReconfigSrMsg |= UL_RECONFIG_DELETE_LONG_SR_NODE;
			}
		}
		else if (tempPucchConfigInfo_p->pucchShortSRNodeHead_p[longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex])
		{
			/* PUCCH TDD ADAPTATION CHANGES Start */        
			deletePucchShortSRNodesForUE( tempPucchConfigInfo_p,PNULL, cellIndex );
			/* PUCCH TDD ADAPTATION CHANGES End */        
			if( ulReconfigUeReq_p)
			{
				ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForShortSRMap = 
					/*CA Changes start  */
					shortPeriodicSRMapShortInfo_gp[cellIndex]->passiveIndex;
				/*CA Changes end  */
				memSet((srConfigInfo_p), 0, sizeof(SRConfigInfo));
				srConfigInfo_p->mapReconfigSrMsg |= UL_RECONFIG_DELETE_SHORT_SR_NODE;
			}
		}

		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
				"[%s] At LP Side (UL_RECONFIG_LONG_SR_CONFIG_INFO(rel)), \n"\
				"srPUCCHRBIndex = %d ,srProhibitTimerValv920 =%d,\n"\
				"srPeriodicity = %d ,srOffset = %d,activeIndexForPUCCHMap = %d \n ",__func__,
				srConfigInfo_p->srPUCCHRBIndex,
				srConfigInfo_p->srProhibitTimerValv920 ,
				srConfigInfo_p->srPeriodicity,
				srConfigInfo_p->srOffset,
				ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap);

	}
	else if ( uplkUEInfo_p->requestType == 1)
	{
        /* SPR 16143 fix start */
        /* Store the SR Offset/Periodicity to be later checked */
        reconfigInfo_p->srOffset = tempPucchConfigInfo_p->srOffset;
        reconfigInfo_p->srPeriodicity = tempPucchConfigInfo_p->srPeriodicity; 
        /* SPR 16143 fix end */
		reconfigSRRetValue = reConfigSchedReqInPUCCHMap (
				/* PUCCH TDD ADAPTATION CHANGES Start */        
				ulUECtxInfoArr_g[ueIndex].ulUEContext_p,
                /* +- SPR 17777 */
				/* PUCCH TDD ADAPTATION CHANGES End */        
				uplkUEInfo_p,
				&srReconfigRetValue, 
				reconfigInfo_p,
				cellIndex);
		/*Reconfig Changes End */
		if(MAC_FAILURE == reconfigSRRetValue)
		{
			return MAC_FAILURE;
		}
		if(ulReconfigUeReq_p)
		{
			srConfigInfo_p->ulReconfigSrMsg = srReconfigRetValue; 
			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s]  AT LP SRReconfigMsgType = %d \n",
					__func__,srConfigInfo_p->ulReconfigSrMsg);
			if ( UL_RECONFIG_SR_MAP_MSG == srReconfigRetValue) 
			{
				tempPucchConfigInfo_p->srProhibitExpiryTTI = 0;
				srConfigInfo_p->srPUCCHRBIndex = uplkUEInfo_p->srPucchResourceIndex;
                /* SPR 16143 fix start */
                srConfigInfo_p->srProhibitTimerValv920 = 
                    uplkUEInfo_p->srProhibitTimerValv920 * tempPucchConfigInfo_p->srPeriodicity;
                /* SPR 16143 fix end */
				srConfigInfo_p->srPeriodicity = tempPucchConfigInfo_p->srPeriodicity; 
				srConfigInfo_p->srOffset = tempPucchConfigInfo_p->srOffset;

				if (SHORT_SR_PERIODICITY >= tempPucchConfigInfo_p->srPeriodicity)
				{
					srConfigInfo_p->headShortSRNode_p = 
						tempPucchConfigInfo_p->pucchShortSRNodeHead_p[shortPeriodicSRMapShortInfo_gp[cellIndex]->passiveIndex];

					ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForShortSRMap = 
						shortPeriodicSRMapShortInfo_gp[cellIndex]->passiveIndex;
					srConfigInfo_p->mapReconfigSrMsg |= UL_RECONFIG_ADD_SHORT_SR_NODE;

					if( SHORT_SR_PERIODICITY < originalSrPeriodicity )
					{

						ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap = 
							longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex;
						srConfigInfo_p->mapReconfigSrMsg |= UL_RECONFIG_DELETE_LONG_SR_NODE;
					}
					LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
							"[%s] At LP Side (UL_RECONFIG_SHORT_SR_CONFIG_INFO(setup)), \n"\
							"srPUCCHRBIndex = %d ,srProhibitTimerValv920 =%d,\n"\
							"srPeriodicity = %d ,srOffset = %d,activeIndexForShortSRMap = %d \n ",__func__,
							srConfigInfo_p->srPUCCHRBIndex,
							srConfigInfo_p->srProhibitTimerValv920 ,
							srConfigInfo_p->srPeriodicity,
							srConfigInfo_p->srOffset,
							ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForShortSRMap);
				}
				else
				{

					srConfigInfo_p->maxSrWrapAround = 
						tempPucchConfigInfo_p->maxSrWrapAround[
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex];
					ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap = 
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex;
					srConfigInfo_p->mapReconfigSrMsg |= UL_RECONFIG_ADD_LONG_SR_NODE;

					if( SHORT_SR_PERIODICITY >= originalSrPeriodicity )
					{
						srConfigInfo_p->headShortSRNode_p = 
							tempPucchConfigInfo_p->pucchShortSRNodeHead_p[shortPeriodicSRMapShortInfo_gp[cellIndex]->passiveIndex];

						ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForShortSRMap = 
							shortPeriodicSRMapShortInfo_gp[cellIndex]->passiveIndex;
						srConfigInfo_p->mapReconfigSrMsg |= UL_RECONFIG_DELETE_SHORT_SR_NODE;

					}
					LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
							"[%s] At LP Side (UL_RECONFIG_LONG_SR_CONFIG_INFO(setup)), \n"\
							"srPUCCHRBIndex = %d ,srProhibitTimerValv920 =%d,\n"\
							"srPeriodicity = %d ,srOffset = %d,activeIndexForPUCCHMap = %d \n ",__func__,
							srConfigInfo_p->srPUCCHRBIndex,
							srConfigInfo_p->srProhibitTimerValv920 ,
							srConfigInfo_p->srPeriodicity,
							srConfigInfo_p->srOffset,
							ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap);

				}
                /* SPR 16143 fix start */
                LOG_MAC_MSG( MAC_SR_INFO_RECONFIG, LOGINFO,  MAC_RRC_INF,
                        getCurrentTick(), ueIndex, srConfigInfo_p->srPUCCHRBIndex, 
                        srConfigInfo_p->srPeriodicity, srConfigInfo_p->srOffset,
                        ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap,
                        0,0, __func__, "" );
                /* SPR 16143 fix end */
			}
			else if (UL_RECONFIG_SR_RESOURCE_INDEX_MSG == srReconfigRetValue)
			{
				srConfigInfo_p->srPUCCHRBIndex = uplkUEInfo_p->srPucchResourceIndex;
				if (SHORT_SR_PERIODICITY >= tempPucchConfigInfo_p->srPeriodicity)
				{
					/* Review comment fix start PUCCH_ENH_2 */
					/* Review comment fix end PUCCH_ENH_2 */
					ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForShortSRMap = 
						shortPeriodicSRMapShortInfo_gp[cellIndex]->passiveIndex;
					srConfigInfo_p->mapReconfigSrMsg |= UL_RECONFIG_UPDATE_SHORT_SR_NODE;
				}
				else
				{
					/* Review comment fix start PUCCH_ENH_2 */
					/* Review comment fix end PUCCH_ENH_2 */
					ulReconfigUeReq_p->rrcInfpucchConfigReqInfo.activeIndexForPUCCHMap = 
						longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->passiveIndex;
					srConfigInfo_p->mapReconfigSrMsg |= UL_RECONFIG_UPDATE_LONG_SR_NODE;
				}
			}
			else if ( UL_RECONFIG_NO_SR_RECONFIGURE_MSG == srReconfigRetValue)
			{
				rrcInfPUCCHConfigReqInfo_p->pucchConfigMsg  &=  UL_RECONFIG_CLEAR_SR_IND;
			}
			/*Reconfig Changes Start */
			reconfigInfo_p->srReconfigBitMap |=  srConfigInfo_p->mapReconfigSrMsg;
			/*Reconfig Changes End */

		}
	}
	return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : checkAndReconfigureSRSDedicatedConfig
 * Inputs         : ueIndex - Stores UE index
 *                  tempsrsConfigInfo_p - Pointer to SRSConfigInfo structure
 *                  uplkUEInfo_p - Pointer to UL UE context
 *                  currSFN,currSF
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : ulReconfigUeReq_p - Pointer to UL Reconfig Request
 *                  reconfigInfo_p - Pointer to Reconfig UE info changes
 * Returns        : None
 * Description    : This function checks and reconfigures SRS dedicated Config
 *****************************************************************************/
/*Reconfig Changes Start */
STATIC  void checkAndReconfigureSRSDedicatedConfig( 
		UInt16               ueIndex,
		SRSConfigInfo        *tempsrsConfigInfo_p, 
		UeContextForUplink   *uplkUEInfo_p,
		ULReconfigUeReq      *ulReconfigUeReq_p,
		UInt32               currSFN, 
		UInt32               currSF, 
		UeContextForReconfig *reconfigInfo_p,
		InternalCellIndex internalCellIndex)

{
	ULSRSDedicatedConfigReqInfo *srsDedicatedConfig_p       = PNULL;
	MacRetType                   reconfigSRSRetValue        = MAC_SUCCESS;
	UInt16                       originalSrsPeriodicity     = tempsrsConfigInfo_p->srsPeriodicity;
	UInt8                        originalSrsConfigType      = tempsrsConfigInfo_p->srsConfigType;
	/* CA Changes start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */
	if(ulReconfigUeReq_p)
	{
		srsDedicatedConfig_p = &ulReconfigUeReq_p->ulSRSDedicatedConfigReqInfo;
	}

	if((TRUE == uplkUEInfo_p->srsRequestType) &&
			(SRS_COMMON_CONFIG_TYPE_SETUP == cellParams_p->srsConfigType)&& 
			((SRS_DEDICATED_CONFIG_TYPE_SETUP == originalSrsConfigType) || 
			 (SRS_DEDICATED_CONFIG_TYPE_SETUP ==  uplkUEInfo_p->srsConfigType)))  
	{
		reconfigSRSRetValue = reconfigureDedicatedSRSConfig(ueIndex,tempsrsConfigInfo_p,
				&uplkUEInfo_p->srsDedicatedSetupInfo, uplkUEInfo_p->srsConfigType, 
				currSFN, currSF,reconfigInfo_p, internalCellIndex);
		/*Reconfig Changes End */

		LTE_MAC_UT_LOG(LOG_MINOR, UE_CONF_MGR,
				"[%s]  AT LP SRSReconfigMsgType = %d \n",__func__,
				reconfigSRSRetValue);
		if((MAC_SUCCESS == reconfigSRSRetValue )&& ulReconfigUeReq_p )
		{ 
			srsDedicatedConfig_p->switchPassiveToActiveSRSFlag = TRUE;
			srsDedicatedConfig_p->srsConfigType         =   uplkUEInfo_p->srsConfigType;
			srsDedicatedConfig_p->srsPeriodicity        =   tempsrsConfigInfo_p->srsPeriodicity;
			srsDedicatedConfig_p->srsOffset             =   tempsrsConfigInfo_p->srsSubFrameOffset;
			srsDedicatedConfig_p->srsDedicatedSetupInfo =   uplkUEInfo_p->srsDedicatedSetupInfo;

			if(SRS_DEDICATED_CONFIG_TYPE_SETUP == originalSrsConfigType )
			{
				if(SRS_DEDICATED_CONFIG_TYPE_SETUP == srsDedicatedConfig_p->srsConfigType)
				{
					/* SPR 9869 Start */
#ifdef FDD_CONFIG
					if((10 < originalSrsPeriodicity) && (10 >= tempsrsConfigInfo_p->srsPeriodicity))
#else
						if((2 < originalSrsPeriodicity) && (2 >= tempsrsConfigInfo_p->srsPeriodicity))
#endif
						{
							/* SPR 9869 End */
							LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s]  AT LP long then short\n",__func__);
							srsDedicatedConfig_p->activeIndexForLongSRSMap = 
								longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex;
							srsDedicatedConfig_p->mapReconfigMsg |= UL_RECONFIG_DELETE_LONG_SRS_NODE;

						}
					/* SPR 9869 Start */
#ifdef FDD_CONFIG
					if((10 >= originalSrsPeriodicity) && (10 < tempsrsConfigInfo_p->srsPeriodicity))
#else
						if((2 >= originalSrsPeriodicity) && (2 < tempsrsConfigInfo_p->srsPeriodicity))
#endif
							/* SPR 9869 End */
						{
							LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s]  AT LP short then long\n",__func__);
							srsDedicatedConfig_p->activeIndexForShortSRSMap = 
								/*CA Changes start  */
								shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex;
							/*CA Changes end  */
							srsDedicatedConfig_p->mapReconfigMsg |= UL_RECONFIG_DELETE_SHORT_SRS_NODE;
						}
				}
				else if(SRS_DEDICATED_CONFIG_TYPE_RELEASE == srsDedicatedConfig_p->srsConfigType)
				{
					LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
							"[%s]  AT LP old config SRS_DEDICATED_CONFIG_TYPE_SETUP\n",__func__);
					srsDedicatedConfig_p->srsReportHeadNode_p = PNULL;
					/* SPR 9869 Start */
#ifdef FDD_CONFIG
					if(10 < originalSrsPeriodicity)
#else
						if(2 < originalSrsPeriodicity)
#endif
						{
							/* SPR 9869 End */
							LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s]  AT LP 10 <= originalSrsPeriodicity \n",__func__);
							srsDedicatedConfig_p->activeIndexForLongSRSMap = 
								/*CA Changes start  */
								longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex;
							/*CA Changes end  */
							srsDedicatedConfig_p->mapReconfigMsg |= UL_RECONFIG_DELETE_LONG_SRS_NODE;

						}
						else
						{
							LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
									"[%s]  AT LP 10 > originalSrsPeriodicity \n",__func__);
							srsDedicatedConfig_p->activeIndexForShortSRSMap = 
								/*CA Changes start  */
								shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex;
							/*CA Changes end  */
							srsDedicatedConfig_p->mapReconfigMsg |= UL_RECONFIG_DELETE_SHORT_SRS_NODE;
						}
				}
			}
			/* SPR 9869 Start */
#ifdef FDD_CONFIG
			if(10 >= tempsrsConfigInfo_p->srsPeriodicity &&
#else
					if(2 >= tempsrsConfigInfo_p->srsPeriodicity &&
#endif
						SRS_DEDICATED_CONFIG_TYPE_SETUP == uplkUEInfo_p->srsConfigType)   /*637 < srsConfigIndex < 16)*/
					{
						/* SPR 9869 End */
						/*CA Changes start  */
						srsDedicatedConfig_p->srsReportHeadNode_p =
							tempsrsConfigInfo_p->srsReportHeadNode_p[shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex]; 

						srsDedicatedConfig_p->activeIndexForShortSRSMap = shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex;
						/*CA Changes end  */
						srsDedicatedConfig_p->mapReconfigMsg |= UL_RECONFIG_ADD_SHORT_SRS_NODE;
						/* when LONG SRS is configured at createUe and SHORT SRS is configured at ReconfigUe*/

						LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
								"[%s] In LP (UL_RECONFIG_SHORT_SRS_DEDICATED_CONFIG) \n"\
								"srsConfigType = %d,srsPeriodicity = %d,\n"\
								"offset = %d , srsBandwidth = %d ,FreqDomainPos = %d , srsHoppingBandwidth = %d\n"\
								"TransComb = %d , Duration = %d CycleShift = %d , SrsConfigIndex = %d\n"\
								"ActiveIndexForShortSRS = %d\n",__func__,
								srsDedicatedConfig_p->srsConfigType,
								srsDedicatedConfig_p->srsPeriodicity,srsDedicatedConfig_p->srsOffset,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.srsBandwidth,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.freqDomainPos,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.srsHoppingBandwidth,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.transmissionComb,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.duration,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.cyclicShift,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.srsConfigIndex,
								srsDedicatedConfig_p->activeIndexForShortSRSMap
							      );
					}
			/* SPR 9869 Start */
#ifdef FDD_CONFIG
			if(10 < tempsrsConfigInfo_p->srsPeriodicity &&
#else
					if(2 < tempsrsConfigInfo_p->srsPeriodicity &&
#endif
						SRS_DEDICATED_CONFIG_TYPE_SETUP == uplkUEInfo_p->srsConfigType)   /*637 < srsConfigIndex < 16)*/
					{
						/* SPR 9869 End */
						/*CA Changes start  */
						srsDedicatedConfig_p->srsReportHeadNode_p =
							tempsrsConfigInfo_p->srsReportHeadNode_p[longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex]; 

						srsDedicatedConfig_p->activeIndexForLongSRSMap = longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex;
						/*CA Changes end  */

						srsDedicatedConfig_p->mapReconfigMsg |= UL_RECONFIG_ADD_LONG_SRS_NODE;

						LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
								"[%s] In LP (UL_RECONFIG_LONG_SRS_DEDICATED_CONFIG)\n"\
								"srsConfigType = %d,srsPeriodicity = %d,\n"\
								"offset = %d , srsBandwidth = %d ,FreqDomainPos = %d , srsHoppingBandwidth = %d\n"\
								"TransComb = %d , Duration = %d CycleShift = %d , SrsConfigIndex = %d\n"\
								"ActiveIndexForSRS = %d\n",__func__,
								srsDedicatedConfig_p->srsConfigType,
								srsDedicatedConfig_p->srsPeriodicity,srsDedicatedConfig_p->srsOffset,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.srsBandwidth,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.freqDomainPos,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.srsHoppingBandwidth,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.transmissionComb,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.duration,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.cyclicShift,
								srsDedicatedConfig_p->srsDedicatedSetupInfo.srsConfigIndex,
								srsDedicatedConfig_p->activeIndexForLongSRSMap
							      );
					}
		}
		else if(MAC_PARTIAL_SUCCESS == reconfigSRSRetValue && ulReconfigUeReq_p)
		{
			srsDedicatedConfig_p->switchPassiveToActiveSRSFlag = FALSE;
			srsDedicatedConfig_p->srsConfigType         =   uplkUEInfo_p->srsConfigType;
			srsDedicatedConfig_p->srsPeriodicity        =   tempsrsConfigInfo_p->srsPeriodicity;
			srsDedicatedConfig_p->srsOffset             =   tempsrsConfigInfo_p->srsSubFrameOffset;
			srsDedicatedConfig_p->srsDedicatedSetupInfo =   uplkUEInfo_p->srsDedicatedSetupInfo;
			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
					"[%s] In LP (UL_RECONFIG_LONG_SRS_DEDICATED_CONFIG)\n"\
					"srsConfigType = %d,srsPeriodicity = %d,\n"\
					"offset = %d , srsBandwidth = %d ,FreqDomainPos = %d , srsHoppingBandwidth = %d\n"\
					"TransComb = %d , Duration = %d CycleShift = %d , SrsConfigIndex = %d\n"\
					"ActiveIndexForSRS = %d\n",__func__,
					srsDedicatedConfig_p->srsConfigType,
					srsDedicatedConfig_p->srsPeriodicity,srsDedicatedConfig_p->srsOffset,
					srsDedicatedConfig_p->srsDedicatedSetupInfo.srsBandwidth,
					srsDedicatedConfig_p->srsDedicatedSetupInfo.freqDomainPos,
					srsDedicatedConfig_p->srsDedicatedSetupInfo.srsHoppingBandwidth,
					srsDedicatedConfig_p->srsDedicatedSetupInfo.transmissionComb,
					srsDedicatedConfig_p->srsDedicatedSetupInfo.duration,
					srsDedicatedConfig_p->srsDedicatedSetupInfo.cyclicShift,
					srsDedicatedConfig_p->srsDedicatedSetupInfo.srsConfigIndex,
					srsDedicatedConfig_p->activeIndexForLongSRSMap
				      );
		} 
		else
		{    
			lteWarning("Error re-configuring SRS Dedicated Config\n");
		}
		if(ulReconfigUeReq_p)
		{
			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
					" [%s] AT LP switchPassiveToActiveSRSFlag = %d \n",__func__,
					srsDedicatedConfig_p->switchPassiveToActiveSRSFlag);
			/*Coverity 5.3.0 CID-54490*/
			/*Reconfig Changes Start */
			reconfigInfo_p->srsReconfigBitMap |= srsDedicatedConfig_p->mapReconfigMsg;
			/*Reconfig Changes End */
			/*Coverity 5.3.0 CID-54490*/
		}
	}
}
/* Cyclomatic Complexity changes - ends here */

/*****************************************************************************
 * Function Name  : reconfigUEULConfigCnf 
 * Inputs         : ulReconfigUeResp_p - Pointer to the Reconfig UE Response
 *                  dwlkUEInfo_p - Pointer to downlin UE info
 *                  uplkUEInfo_p - Pointer to uplink UE info
 *                  currSFN,currSF
 *                  reconfigInfo_p - Pointer to Reconfig UE info changes
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This function updates passive context in LP thread once 
 *                  the response is received from HP thread.
 *****************************************************************************/
/*Reconfig Changes Start */
 MacRetType reconfigUEULConfigCnf(ULReconfigUeResp *ulReconfigUeResp_p,
		UeContextForDownlink  *dwlkUEInfo_p,
		UeContextForUplink  *uplkUEInfo_p,
		UInt32 currSFN, UInt32 currSF, 
		UeContextForReconfig *reconfigInfo_p,
		InternalCellIndex internalCellIndex)
{
	ULUEContext *newULUEContext_p = PNULL;
	/* CA changes start */
	/* + CA_TDD_HARQ_CHANGES */    
	/* - CA_TDD_HARQ_CHANGES */    
	/* SPR 11541 Fix : Start */
	ULUESCellContext **ulScellContext_pp=PNULL;
	UInt8 sCellIndex =0;
	ScellInfo *scellInfo_p = PNULL;
	/** SPR 12320 Fix : START **/
	DLUEContext *newDLUEContext_p = dlUECtxInfoArr_g[ulReconfigUeResp_p->ueIndex].dlUEContext_p;
	PucchConfig temp_Pucch_ConfigInfo = {0};
	/** SPR 12320 Fix : END **/
	/* SPR 11541 Fix : End */
	UInt8 scellCount =0;
	UeCellInfo  ueCellInfo = {0};
	/** SPR 12320 Fix : START **/
    /*SPR 16855 +-*/
	UInt32 count = 0;
    /*SPR 16855 +-*/
	/** SPR 12320 Fix : END **/
	/* CA changes end */
	newULUEContext_p = ulUECtxInfoArr_g[ulReconfigUeResp_p->ueIndex].ulUEContext_p;
	PucchConfig *tempPucchConfigInfo = &(newULUEContext_p->pucchConfigInfo);
	SRSConfigInfo *tempsrsConfigInfo = &(newULUEContext_p->srsConfigInfo);

	if ( MAC_FAILURE == reconfigurePUCCHSRSAndReportsPassiveContext(
				newULUEContext_p,
				newULUEContext_p->ueIndex,
				tempPucchConfigInfo,
				tempsrsConfigInfo,
                /* +- SPR 17777 */
				uplkUEInfo_p,
				currSFN, currSF,
				reconfigInfo_p,
				internalCellIndex))
	{
		return MAC_FAILURE;
	}
	/* CA changes start */
	for (scellCount = 0; scellCount<dwlkUEInfo_p->carrierAggrConfig.\
			scellCount; scellCount++)
	{
		if (RECONFIGURE == dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].\
				operationType)
		{
			ueCellInfo.sCellIndex = dwlkUEInfo_p->carrierAggrConfig.\
						scellInfo[scellCount].sCellIndex;
			if (MAC_SUCCESS != reconfigureSCellCqiRiPassiveContext(
						newULUEContext_p,
						newULUEContext_p->ueIndex, 
                        /* +- SPR 17777 */
						uplkUEInfo_p, 
						reconfigInfo_p,
						&ueCellInfo,
						internalCellIndex))
			{
				return MAC_FAILURE;
			}
		}
		/* SPR 11541 Fix : Start */
		if (ADD == dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].\
				operationType)
		{
			scellInfo_p = &(dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount]);
			sCellIndex = scellInfo_p->sCellIndex;
			ulScellContext_pp = &(newULUEContext_p->ulUeScellContext_p[sCellIndex]);

			if ( MAC_FAILURE == macConfigureScellPucchSRSAndReports(
						&((*ulScellContext_pp)->
							scellpucchConfigInfo),
						dwlkUEInfo_p,uplkUEInfo_p,
						newULUEContext_p, 
						newDLUEContext_p,
						scellCount,internalCellIndex ))
			{
				return MAC_FAILURE;
			}
		}
		/* SPR 11541 Fix : End */
		if (DELETE == dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].\
				operationType)
		{
			/** SPR 12320 Fix : Start **/
            /*SPR 16855 +-*/
			for (count = 0;count < MAX_NUM_MAPS;count++)
			{
				memCpy(&(temp_Pucch_ConfigInfo.pucchNodesForCqi[count]),
						&(ulReconfigUeResp_p->pucchNodesForCqi[count]),sizeof(pucchNodeForReport));
				memCpy(&(temp_Pucch_ConfigInfo.pucchNodesForWbCqi[count]),
						&(ulReconfigUeResp_p->pucchNodesForWbCqi[count]),sizeof(pucchNodeForReport));
				memCpy(&(temp_Pucch_ConfigInfo.pucchNodesForRI[count]),
						&(ulReconfigUeResp_p->pucchNodesForRI[count]),sizeof(pucchNodeForReport));
				memCpy(&(temp_Pucch_ConfigInfo.pucchNodesForCqi2[count]),
						&(ulReconfigUeResp_p->pucchNodesForCqi2[count]),sizeof(pucchNodeForReport));
				memCpy(&(temp_Pucch_ConfigInfo.pucchNodesForWbCqi2[count]),
						&(ulReconfigUeResp_p->pucchNodesForWbCqi2[count]),sizeof(pucchNodeForReport));
				memCpy(&(temp_Pucch_ConfigInfo.pucchNodesForRI2[count]),
						&(ulReconfigUeResp_p->pucchNodesForRI2[count]),sizeof(pucchNodeForReport));

			}
            /*SPR 16855 +-*/
			ueCellInfo.isPCell = FALSE;
			ueCellInfo.sCellIndex = dwlkUEInfo_p->carrierAggrConfig.\
						scellInfo[scellCount].sCellIndex;

			deleteSCellPucchNodesForUE(&temp_Pucch_ConfigInfo,WIDEBAND_CQI_REPORT,&ueCellInfo,
					internalCellIndex,newULUEContext_p->ueIndex);
			deleteSCellPucchNodesForUE(&temp_Pucch_ConfigInfo,CQI_REPORT,&ueCellInfo,
					internalCellIndex,newULUEContext_p->ueIndex);
			deleteSCellPucchNodesForUE(&temp_Pucch_ConfigInfo,RI_REPORT,&ueCellInfo,
					internalCellIndex,newULUEContext_p->ueIndex);
			/** SPR 12320 Fix : End **/
			/*eicic changes*/
			updateCQIMapForAbs_g[internalCellIndex] = LTE_TRUE;
			deleteSCellPucchNodesForUE(&temp_Pucch_ConfigInfo,WIDEBAND_CQI_REPORT,&ueCellInfo,
					internalCellIndex,newULUEContext_p->ueIndex);
			deleteSCellPucchNodesForUE(&temp_Pucch_ConfigInfo,CQI_REPORT,&ueCellInfo,
					internalCellIndex,newULUEContext_p->ueIndex);
			deleteSCellPucchNodesForUE(&temp_Pucch_ConfigInfo,RI_REPORT,&ueCellInfo,
					internalCellIndex,newULUEContext_p->ueIndex);
			updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;

		}
	}
	/* CA changes end */
	return MAC_SUCCESS;
}
/*Reconfig Changes End */

/*****************************************************************************
 * Function Name  : reconfigUEDLConfigCnf 
 * Inputs         : dlReconfigUeResp_p - Pointer to the Reconfig UE Response
 *                  dwlkUEInfo_p - Pointer to downlin UE info
 *                  gapPatternId - Measurement Gap  pattern Info (40ms or 80ms)
 *                  mapBitMask - Measurement gap reconfig changes info bit
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This function updates passive context in LP thread once 
 *                  the response is received from HP thread.
 *****************************************************************************/
 MacRetType reconfigUEDLConfigCnf(DLReconfigUeResp *dlReconfigUeResp_p,
		UeContextForDownlink  *dwlkUEInfo_p,
                                        /* SPR 16082 fix start */
                                        UInt8 dlMeasGapConfigType,
                                        UInt8 ulMeasGapConfigType,
                                        MeasGapContextDL *tempDLMeasGapContextInfo,
                                        MeasGapContextUL *tempULMeasGapContextInfo,
                                        /* SPR 16082 fix end */
		UInt8 gapPatternId,UInt8 *mapBitMask,
		/*CA Changes start */
		InternalCellIndex internalCellIndex)
/*CA Changes end */
{
	ULUEContext *newULUEContext_p = PNULL;
	DLUEContext *newDLUEContext_p = PNULL;
	MeasGapContextDL *dlMeasGapInfo_p = PNULL;
	MeasGapContextUL *ulMeasGapInfo_p = PNULL;
    /* +- SPR 16082 */

	newULUEContext_p = ulUECtxInfoArr_g[dlReconfigUeResp_p->ueIndex].ulUEContext_p;
	newDLUEContext_p = dlUECtxInfoArr_g[dlReconfigUeResp_p->ueIndex].dlUEContext_p;


	dlMeasGapInfo_p = &newDLUEContext_p->measGapContext;
	ulMeasGapInfo_p = &newULUEContext_p->measGapContext;
    /* +- SPR 16082 */

	if (TRUE == dwlkUEInfo_p->measGapRequestType)
	{
        /* SPR 16082 fix start */
        processMeasGapReConfig(dwlkUEInfo_p, tempDLMeasGapContextInfo,
                tempULMeasGapContextInfo, &dlMeasGapConfigType, 
				&ulMeasGapConfigType,
				dlReconfigUeResp_p->ueIndex,
				gapPatternId,mapBitMask,
				internalCellIndex);
        if ( GP0 == dwlkUEInfo_p->measGapConfig.gapPatternId 
#ifdef ENDC_ENABLED
			/*Meas_Gap_Changes_Start*/
        	|| GP7_R15 == dwlkUEInfo_p->measGapConfig.gapPatternId
        	/*Meas_Gap_Changes_End*/
#endif
        	)
        {
            ulMeasGapInfo_p->measGapStartNodeUL_p[measGap40msInfo_gp[internalCellIndex]->passiveIndex] =
                tempULMeasGapContextInfo->measGapStartNodeUL_p[measGap40msInfo_gp[internalCellIndex]->passiveIndex];
            ulMeasGapInfo_p->measGapEndNodeUL_p[measGap40msInfo_gp[internalCellIndex]->passiveIndex] =
                tempULMeasGapContextInfo->measGapEndNodeUL_p[measGap40msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapStartNodeDL_p[measGap40msInfo_gp[internalCellIndex]->passiveIndex] =
                tempDLMeasGapContextInfo->measGapStartNodeDL_p[measGap40msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapEndNodeDL_p[measGap40msInfo_gp[internalCellIndex]->passiveIndex] =
                tempDLMeasGapContextInfo->measGapEndNodeDL_p[measGap40msInfo_gp[internalCellIndex]->passiveIndex];
            /* SPR 16530 Fix Start */
            ulMeasGapInfo_p->measGapStartOffsetUL[measGap40msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempULMeasGapContextInfo->measGapStartOffsetUL[measGap40msInfo_gp[internalCellIndex]->passiveIndex];
            ulMeasGapInfo_p->measGapEndOffsetUL[measGap40msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempULMeasGapContextInfo->measGapEndOffsetUL[measGap40msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapStartOffsetDL[measGap40msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempDLMeasGapContextInfo->measGapStartOffsetDL[measGap40msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapEndOffsetDL[measGap40msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempDLMeasGapContextInfo->measGapEndOffsetDL[measGap40msInfo_gp[internalCellIndex]->passiveIndex];
            /* SPR 16530 Fix End */
        }
#ifdef ENDC_ENABLED
		/*Meas_Gap_Changes_Start*/
		else if (GP4_R15 == dwlkUEInfo_p->measGapConfig.gapPatternId
			|| GP6_R15 == dwlkUEInfo_p->measGapConfig.gapPatternId
			|| GP10_R15 == dwlkUEInfo_p->measGapConfig.gapPatternId)
		{
	        ulMeasGapInfo_p->measGapStartNodeUL_p[measGap20msInfo_gp[internalCellIndex]->passiveIndex] =
                tempULMeasGapContextInfo->measGapStartNodeUL_p[measGap20msInfo_gp[internalCellIndex]->passiveIndex];
            ulMeasGapInfo_p->measGapEndNodeUL_p[measGap20msInfo_gp[internalCellIndex]->passiveIndex] =
                tempULMeasGapContextInfo->measGapEndNodeUL_p[measGap20msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapStartNodeDL_p[measGap20msInfo_gp[internalCellIndex]->passiveIndex] =
                tempDLMeasGapContextInfo->measGapStartNodeDL_p[measGap20msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapEndNodeDL_p[measGap20msInfo_gp[internalCellIndex]->passiveIndex] =
                tempDLMeasGapContextInfo->measGapEndNodeDL_p[measGap20msInfo_gp[internalCellIndex]->passiveIndex];
            ulMeasGapInfo_p->measGapStartOffsetUL[measGap20msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempULMeasGapContextInfo->measGapStartOffsetUL[measGap20msInfo_gp[internalCellIndex]->passiveIndex];
            ulMeasGapInfo_p->measGapEndOffsetUL[measGap20msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempULMeasGapContextInfo->measGapEndOffsetUL[measGap20msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapStartOffsetDL[measGap20msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempDLMeasGapContextInfo->measGapStartOffsetDL[measGap20msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapEndOffsetDL[measGap20msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempDLMeasGapContextInfo->measGapEndOffsetDL[measGap20msInfo_gp[internalCellIndex]->passiveIndex];

		}
		else if (GP5_R15 == dwlkUEInfo_p->measGapConfig.gapPatternId
			|| GP9_R15 == dwlkUEInfo_p->measGapConfig.gapPatternId
			|| GP11_R15 == dwlkUEInfo_p->measGapConfig.gapPatternId)
		{
	        ulMeasGapInfo_p->measGapStartNodeUL_p[measGap160msInfo_gp[internalCellIndex]->passiveIndex] =
                tempULMeasGapContextInfo->measGapStartNodeUL_p[measGap160msInfo_gp[internalCellIndex]->passiveIndex];
            ulMeasGapInfo_p->measGapEndNodeUL_p[measGap160msInfo_gp[internalCellIndex]->passiveIndex] =
                tempULMeasGapContextInfo->measGapEndNodeUL_p[measGap160msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapStartNodeDL_p[measGap160msInfo_gp[internalCellIndex]->passiveIndex] =
                tempDLMeasGapContextInfo->measGapStartNodeDL_p[measGap160msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapEndNodeDL_p[measGap160msInfo_gp[internalCellIndex]->passiveIndex] =
                tempDLMeasGapContextInfo->measGapEndNodeDL_p[measGap160msInfo_gp[internalCellIndex]->passiveIndex];
            ulMeasGapInfo_p->measGapStartOffsetUL[measGap160msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempULMeasGapContextInfo->measGapStartOffsetUL[measGap160msInfo_gp[internalCellIndex]->passiveIndex];
            ulMeasGapInfo_p->measGapEndOffsetUL[measGap160msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempULMeasGapContextInfo->measGapEndOffsetUL[measGap160msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapStartOffsetDL[measGap160msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempDLMeasGapContextInfo->measGapStartOffsetDL[measGap160msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapEndOffsetDL[measGap160msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempDLMeasGapContextInfo->measGapEndOffsetDL[measGap160msInfo_gp[internalCellIndex]->passiveIndex];
		}
		/*Meas_Gap_Changes_End*/
#endif
        else
        {
            ulMeasGapInfo_p->measGapStartNodeUL_p[measGap80msInfo_gp[internalCellIndex]->passiveIndex] =
                tempULMeasGapContextInfo->measGapStartNodeUL_p[measGap80msInfo_gp[internalCellIndex]->passiveIndex];
            ulMeasGapInfo_p->measGapEndNodeUL_p[measGap80msInfo_gp[internalCellIndex]->passiveIndex] =
                tempULMeasGapContextInfo->measGapEndNodeUL_p[measGap80msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapStartNodeDL_p[measGap80msInfo_gp[internalCellIndex]->passiveIndex] =
                tempDLMeasGapContextInfo->measGapStartNodeDL_p[measGap80msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapEndNodeDL_p[measGap80msInfo_gp[internalCellIndex]->passiveIndex] =
                tempDLMeasGapContextInfo->measGapEndNodeDL_p[measGap80msInfo_gp[internalCellIndex]->passiveIndex];
            /* SPR 16530 Fix Start */
            ulMeasGapInfo_p->measGapStartOffsetUL[measGap80msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempULMeasGapContextInfo->measGapStartOffsetUL[measGap80msInfo_gp[internalCellIndex]->passiveIndex];
            ulMeasGapInfo_p->measGapEndOffsetUL[measGap80msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempULMeasGapContextInfo->measGapEndOffsetUL[measGap80msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapStartOffsetDL[measGap80msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempDLMeasGapContextInfo->measGapStartOffsetDL[measGap80msInfo_gp[internalCellIndex]->passiveIndex];
            dlMeasGapInfo_p->measGapEndOffsetDL[measGap80msInfo_gp[internalCellIndex]->passiveIndex] = 
                tempDLMeasGapContextInfo->measGapEndOffsetDL[measGap80msInfo_gp[internalCellIndex]->passiveIndex];
            /* SPR 16530 Fix End */
        }
        /* SPR 16082 fix end */
	}
	return MAC_SUCCESS;
}
/*SPR 16855 +-*/

/* + CQI_4.1 */
/****************************************************************************
 * Function Name  :  getCQIFromMCS 
 * Inputs         :  mcsIndex
 * Output         :  None
 * Returns        :  CQI value corresponding to MCS Index
 * Description    :  This function access  mcsCQIArr_g to get CQI value 
 *                   corresponding to MCS Index
 *****************************************************************************/
UInt8 getCQIFromMCS(UInt8 mcsIndex)
{
	return  mcsCQIArr_g[mcsIndex];
}

/****************************************************************************
 * Function Name  : initializeSpsCrntiMap
 * Inputs         : spsCrntiStartRange - SPS CRNTI START RANGE
 *                  spsCrntiEndRange - SPS CRNTI END RANGE
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function allocates initializes SPS array map.
 ****************************************************************************/
MacRetType initializeSpsCrntiMap( 
		UInt16 spsCrntiStartRange, 
		UInt16 spsCrntiEndRange 
		/* CA changes Start */
		, InternalCellIndex internalCellIndex
		/* CA Changes end */
		)
{
	UInt16           idx           = 0;
	/*+COVERITY 5.3.0 - 32263*/
	MacRetType       retVal        = MAC_SUCCESS;
	/*-COVERITY 5.3.0 - 32263*/
	LP_SpsCrntiNode  crntiNode_p  = PNULL;
	UInt16 totCrntiAllocated = (spsCrntiEndRange - spsCrntiStartRange + 1);
	UInt16           rnti          = spsCrntiStartRange;

	spsCrntiMap_g[internalCellIndex].totalSpsCrntiAllocated = totCrntiAllocated;
	spsCrntiMap_g[internalCellIndex].lastCrntiAllocIndex = totCrntiAllocated - 1;

	spsCrntiMap_g[internalCellIndex].spsCrntiList_p = (LP_SpsCrntiNode)getMemFromPool( 
			sizeof(SpsCrntiNode) * totCrntiAllocated, PNULL  );
	if( PNULL == spsCrntiMap_g[internalCellIndex].spsCrntiList_p )
	{
		/*CRITICAL! Memory error*/
		LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex), __LINE__, sizeof(SpsCrntiNode), internalCellIndex,0,
				0, 0,0, __FILE__, __func__);
		ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
				CRITICAL_ALARM);
		ltePanic("getMemFromPool fails %s", __func__);
		return MAC_FAILURE;
	}
	for( idx = 0; idx < totCrntiAllocated; idx++)
	{
		crntiNode_p = &spsCrntiMap_g[internalCellIndex].spsCrntiList_p[idx];
		crntiNode_p->spsCrnti = rnti;
		crntiNode_p->idxStatus = SPS_CRNTI_FREE;
		rnti++;
	}
	return retVal;
}

/****************************************************************************
 * Function Name  : allocateSpsCrnti 
 * Inputs         : internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : SPS - CRNTI or MAC_FAILURE
 * Description    : This function allocates SPS CRNTI from available SPS CRNTIs
 *                  that are not yet allocated to any other UE.
 ****************************************************************************/
UInt16 allocateSpsCrnti(InternalCellIndex internalCellIndex)
{
	UInt16           retVal            = MAC_FAILURE;
	LP_SpsCrntiNode  spsCrntiList_p    = PNULL;
	UInt16           lastCrntiAllocIdx = 0;
	UInt16           ctr               = 0;
	UInt16           totCrntiAllocated = 0;

	/* \code
	 * Check for free SPS CRNTI in the global Map, spsCrntiMap_g.
	 * Stores lastCrntiAllocIndex with index of allocated SPS-CRNTI.
	 * Mark idxStatus to SPS_CRNTI_BUSY.
	 * Returns SPS CRNTI allocated to be stored in UE Context.
	 * \endcode
	 * */
	lastCrntiAllocIdx = spsCrntiMap_g[internalCellIndex].lastCrntiAllocIndex;
	totCrntiAllocated = spsCrntiMap_g[internalCellIndex].totalSpsCrntiAllocated;

	do
	{
		lastCrntiAllocIdx = (lastCrntiAllocIdx + 1 ) % totCrntiAllocated ;

		spsCrntiList_p = &spsCrntiMap_g[internalCellIndex].spsCrntiList_p[lastCrntiAllocIdx];
		/* Coverity 54511*/
		/* Coverity 54511*/

		if( SPS_CRNTI_FREE == spsCrntiList_p->idxStatus )
		{
			retVal = spsCrntiList_p->spsCrnti;
			spsCrntiList_p->idxStatus = SPS_CRNTI_BUSY;
			spsCrntiMap_g[internalCellIndex].lastCrntiAllocIndex = lastCrntiAllocIdx;            
			break;
		}
	} while( totCrntiAllocated >= ++ctr );

	return retVal;
}

/****************************************************************************
 * Function Name  : deAllocateSpsCrnti 
 * Inputs         : spsCrnti - SPS CRNTI that needs to be removed
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function deallocates SPS CRNTI allocated to SPS UE.
 ****************************************************************************/
UInt16 deAllocateSpsCrnti( UInt16 spsCrnti ,InternalCellIndex internalCellIndex)
{
	LP_SpsCrntiNode  spsCrntiList_p    = PNULL;
	UInt16           idx               = 0;
	UInt16           totCrntiAllocated = 0;
	UInt16           startSpsCrnti     = 0;

	/* \code
	 * Check index for SPS CRNTI in the global Map, spsCrntiMap_g with diff 
	 * from startSpsCrntiRange.
	 * Mark idxStatus to SPS_CRNTI_FREE.
	 * Assign lastCrntiAllocIndex with previous index if last SPS-CRNTI 
	 * allocated is freed.
	 * \endcode
	 * */
	/* Update the RntiIDX Table*/
	RNTIInfo *rntiInfo_p = PNULL;
	/*CA Changes start  */
	rntiInfo_p = getUeIdxFromRNTIMap(spsCrnti, internalCellIndex);
	/*CA Changes end  */
	if (rntiInfo_p != PNULL)
	{
		rntiInfo_p->rntiFlag = FREERNTI;
		rntiInfo_p->index    = MAX_UE_SUPPORTED;
		/*pdcch changes start*/
		/* SPR 13539 fix  */
		/*pdcch changes end*/
	}

	totCrntiAllocated = spsCrntiMap_g[internalCellIndex].totalSpsCrntiAllocated;

	/*CA Changes start  */
	startSpsCrnti = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
			spsCommonCellInfo.startSpsCRnti;
	/*CA Changes end  */
	idx = spsCrnti - startSpsCrnti;

	spsCrntiList_p = &(spsCrntiMap_g[internalCellIndex].spsCrntiList_p[idx]);
	/* Coverity 54500*/

	if( SPS_CRNTI_FREE == spsCrntiList_p->idxStatus )
	{
		return MAC_SUCCESS;
	}
	spsCrntiList_p->idxStatus = SPS_CRNTI_FREE;
	/* Last SPS CRNTI allocated is freed */
	if( spsCrntiMap_g[internalCellIndex].lastCrntiAllocIndex == idx )
	{
		if( 0 == spsCrntiMap_g[internalCellIndex].lastCrntiAllocIndex)
		{
			/* Assign lastCrntiAllocIndex to last index of array */
			spsCrntiMap_g[internalCellIndex].lastCrntiAllocIndex = totCrntiAllocated - 1;
		}
		else
		{
			/* Decrement lastCrntiAllocIndex */
			(spsCrntiMap_g[internalCellIndex].lastCrntiAllocIndex)--;
		}
	}

	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  :  initBandwidthPart 
 * Inputs         :  newULUEContext_p - Pointer to UL context 
 *                   internalCellIndex - internalCellIndex of cell used at MAC
 * Output         :  Bandwidthpart
 * Returns        :  None
 * Description    :  This function determine the bandwidthpart of the first
 *                   periodic CQI report after create/reconfig.
 ******************************************************************************/
void initBandwidthPart(ULUEContext *newULUEContext_p,
		InternalCellIndex internalCellIndex
		)
{
	UInt32 currentSFN = 0;
	UInt32 currentSF = 0;
	UInt32 nextCQISFN = 0;
	UInt32 nextCQISF = 0;
	UInt32 lastWidebandSFN = 0;
	UInt32 lastWidebandSF = 0;
	UInt32 nextWidebandTTI = 0;
	UInt32 lastWidebandTTI = 0;
	UInt32 lastWidebandSFN_2 = 0;
	UInt32 lastWidebandSF_2 = 0;
	UInt32 nextWidebandTTI_2 = 0;
	UInt32 lastWidebandTTI_2 = 0;

	UInt8 cqiPeriodicity = newULUEContext_p->pucchConfigInfo.cqiPeriodicity;    
	UInt8 cqiPeriodicity_2 = newULUEContext_p->pucchConfigInfo.cqiPeriodicity_2 ;    /* EICIC +-*/

	UInt32 widebandPeriodicity = newULUEContext_p->pucchConfigInfo.cqiPmiRiInfo.
		widebandPeriodicity;
	UInt32 widebandPeriodicity_2 = newULUEContext_p->pucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity_2; /* EICIC +-*/


	/*CA Changes start  */
	getSFAndSFN(&currentSFN,&currentSF,internalCellIndex);
	/*CA Changes end  */
	/* CA Changes start */
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */
	/*TDD SB_CQI*/
#ifdef FDD_CONFIG
	GET_REQUIRED_TICK(currentSFN,currentSF, UE_ACK_NACK_TTI + PHY_DL_DELAY + ONE_ADVANCE) ;
#else
	GET_REQUIRED_TICK(currentSFN,currentSF,getULSfForUpdatingBandwidthPart(currentSF, internalCellIndex)) ;
#endif
	/*TDD SB_CQI*/

	nextCQISFN = currentSFN;
	nextCQISF = currentSF;

	/*  Calculating next occurance of wideband CQI.
	 *  Ref. spec 36.213 sec 7.2.2. 
	 */
	while (0  != ((MAX_SUB_FRAME * nextCQISFN + nextCQISF - 
					newULUEContext_p->pucchConfigInfo.cqiOffset) % widebandPeriodicity))
	{
		GET_REQUIRED_TICK( nextCQISFN,nextCQISF,1); 
	}

	nextWidebandTTI = (( nextCQISFN * MAX_SUB_FRAME) + nextCQISF);

	newULUEContext_p->nextWidebandOccurrence = nextWidebandTTI;

	nextCQISFN = currentSFN;
	nextCQISF = currentSF;

	/*  Calculating next occurance of subband CQI. 
	 *  Ref. spec 36.213 sec 7.2.2.
	 */
	while (0  != ((MAX_SUB_FRAME * nextCQISFN + nextCQISF - 
					newULUEContext_p->pucchConfigInfo.cqiOffset) % cqiPeriodicity ))
	{
		GET_REQUIRED_TICK( nextCQISFN,nextCQISF,1);   
	}

	newULUEContext_p->nextSubbandOccurrence = 
		((nextCQISFN * MAX_SUB_FRAME) + nextCQISF);

	/*  Calculating last occurance of wideband CQI */
	if ( nextWidebandTTI >= widebandPeriodicity )
	{
		lastWidebandTTI = nextWidebandTTI - widebandPeriodicity;

	}
	else
	{
		if(newULUEContext_p->nextSubbandOccurrence  >= widebandPeriodicity)
		{    
			lastWidebandTTI =   newULUEContext_p->nextSubbandOccurrence - widebandPeriodicity;
		}    
		else
		{
			lastWidebandTTI  = ( MAX_SFN_VALUE * MAX_SUB_FRAME) - 
				(widebandPeriodicity - newULUEContext_p->nextSubbandOccurrence);   
		}

		lastWidebandSFN = lastWidebandTTI / MAX_SUB_FRAME;
		lastWidebandSF = lastWidebandTTI % MAX_SUB_FRAME; 

		while (0  != ((MAX_SUB_FRAME * lastWidebandSFN + lastWidebandSF - 
						newULUEContext_p->pucchConfigInfo.cqiOffset) % widebandPeriodicity))
		{
			GET_REQUIRED_TICK( lastWidebandSFN,lastWidebandSF,1); 
		}
		lastWidebandTTI = (MAX_SUB_FRAME * lastWidebandSFN + lastWidebandSF);

	} 

	/* Calculating Bandwidthpart */
	if ( newULUEContext_p->nextSubbandOccurrence > lastWidebandTTI )
	{
		newULUEContext_p->reportedBandwidthPartNum =
			((newULUEContext_p->nextSubbandOccurrence - lastWidebandTTI ) / cqiPeriodicity);

		newULUEContext_p->reportedBandwidthPartNum  = 
			((newULUEContext_p->reportedBandwidthPartNum - 1) % cellParams_p->bandwidthParts);
	}
	else
	{
		newULUEContext_p->reportedBandwidthPartNum = 0;
	}
	/* EICIC +*/

	nextCQISFN = currentSFN;
	nextCQISF = currentSF;

	if (widebandPeriodicity_2) /* EICIC spr 14996 fix*/
	{
		while (0  != ((MAX_SUB_FRAME * nextCQISFN + nextCQISF - 
						newULUEContext_p->pucchConfigInfo.cqiOffset_2) % widebandPeriodicity_2))
		{
			GET_REQUIRED_TICK( nextCQISFN,nextCQISF,1); 
		}
	}

	nextWidebandTTI_2 = (( nextCQISFN * MAX_SUB_FRAME) + nextCQISF);

	newULUEContext_p->nextWidebandOccurrence_2 = nextWidebandTTI_2;

	nextCQISFN = currentSFN;
	nextCQISF = currentSF;

	if (cqiPeriodicity_2) /* EICIC spr 14996 fix*/
	{
		while (0  != ((MAX_SUB_FRAME * nextCQISFN + nextCQISF - 
						newULUEContext_p->pucchConfigInfo.cqiOffset_2) % cqiPeriodicity_2 ))
		{
			GET_REQUIRED_TICK( nextCQISFN,nextCQISF,1);   
		}
	}

	newULUEContext_p->nextSubbandOccurrence_2 = 
		((nextCQISFN * MAX_SUB_FRAME) + nextCQISF);

	/*  Calculating last occurance of wideband CQI */
	if ( nextWidebandTTI_2 >= widebandPeriodicity_2 )
	{
		lastWidebandTTI_2 = nextWidebandTTI_2 - widebandPeriodicity_2;

	}
	else
	{
		if(newULUEContext_p->nextSubbandOccurrence_2  >= widebandPeriodicity_2)
		{    
			lastWidebandTTI_2 =   newULUEContext_p->nextSubbandOccurrence_2 - widebandPeriodicity_2;
		}    
		else
		{
			lastWidebandTTI_2  = ( MAX_SFN_VALUE * MAX_SUB_FRAME) - 
				(widebandPeriodicity_2 - newULUEContext_p->nextSubbandOccurrence_2);   
		}

		lastWidebandSFN_2 = lastWidebandTTI_2 / MAX_SUB_FRAME;
		lastWidebandSF_2 = lastWidebandTTI_2 % MAX_SUB_FRAME; 

		if (widebandPeriodicity_2) /* EICIC spr 14996 fix*/
		{
			while (0  != ((MAX_SUB_FRAME * lastWidebandSFN_2 + lastWidebandSF_2 - 
							newULUEContext_p->pucchConfigInfo.cqiOffset_2) % widebandPeriodicity_2))
			{
				GET_REQUIRED_TICK( lastWidebandSFN_2,lastWidebandSF_2,1); 
			}
		}
		lastWidebandTTI_2 = (MAX_SUB_FRAME * lastWidebandSFN_2 + lastWidebandSF_2);

	} 

	/* Calculating Bandwidthpart */
	if ( newULUEContext_p->nextSubbandOccurrence_2 > lastWidebandTTI_2 )
	{
		if (cqiPeriodicity_2) /* EICIC spr 14996 fix*/
		{
			newULUEContext_p->reportedBandwidthPartNum_2 =
				((newULUEContext_p->nextSubbandOccurrence_2 - lastWidebandTTI_2 ) / cqiPeriodicity_2);
		}
		if (cellParams_p->bandwidthParts) /* EICIC spr 14996 fix*/
		{
			newULUEContext_p->reportedBandwidthPartNum_2  = 
				((newULUEContext_p->reportedBandwidthPartNum_2 - 1) % cellParams_p->bandwidthParts);
		}
	}
	else
	{
		newULUEContext_p->reportedBandwidthPartNum_2 = 0;
	}

	/* EICIC -*/


	return;
}
/* - CQI_4.1 */

/* + CQI_5.0 */
/****************************************************************************
 * Function Name  :   validateCQIModeForNewTxMode 
 * Inputs         :   newTransmissionMode - Reconfigured transmission Mode
 *                    cqiTypeIndicator -
 *                    periodicRepMode - Periodic mode if configured at
 *                                      create.
 *                    aperiodicRepMode - Aperiodic mode if configured at
 *                                       create.
 *                    pmiRiReportV920 -
 *                    
 * Output         :  None
 * Returns        :  MAC_SUCCESS/ MAC_FAILURE
 * Description    :  This function will validate the CQI modes against new
 *                   transmission mode if CQI modes are not reconfigured.
 *                   If new transmission mode is closed loop MIMO and configured 
 *                   CQI mode is periodic 10/ periodic 20/aperiodic 20/
 *                   aperiodic30 then return failure or 
 *                   if new transmission mode is Open loop MIMO/SISO/
 *                   Transmit diversity and configured CQI mode is periodic 11/
 *                   periodic21/aperiodic 12/aperiodic 22/aperiodic 31 then
 *                   return failure.
 ******************************************************************************/
MacRetType validateCQIModeForNewTxMode(UInt8 newTransmissionMode,
		UInt8    cqiTypeIndicator, 
		PeriodicReportingModes periodicRepMode,
		UInt8 aperiodicRepMode,
		/* + TM7_8 Changes Start */
		UInt8 pmiRiReportV920)
/* + TM7_8 Changes End */
{

	switch(newTransmissionMode)
	{
		case TX_MODE_1:
		case TX_MODE_2:
		case TX_MODE_3:
			/* + TM7_8 Changes Start */
		case TX_MODE_7:
			/* - TM7_8 Changes End */ 
			switch (cqiTypeIndicator)
			{   
				case PERIODIC_CQI_REPORTING_MODE:
					if( (PERIODIC_MODE_1_1  == periodicRepMode) || 
							(PERIODIC_MODE_2_1  == periodicRepMode) )
					{
						return MAC_FAILURE;
					}
					break;

				case APERIODIC_CQI_REPORTING_MODE:
					if( (CQI_MODE_1_2 == aperiodicRepMode) || 
							(CQI_MODE_2_2 == aperiodicRepMode) ||
							(CQI_MODE_3_1 == aperiodicRepMode) ) 
					{
						return MAC_FAILURE;
					}    
					break;

				case PERIODIC_APERIODIC_MODE:

					if( (PERIODIC_MODE_1_1  == periodicRepMode)  || 
							(PERIODIC_MODE_2_1  == periodicRepMode)  ||
							(CQI_MODE_1_2 == aperiodicRepMode)       ||
							(CQI_MODE_2_2 == aperiodicRepMode)       ||
							(CQI_MODE_3_1 == aperiodicRepMode) )
					{
						return MAC_FAILURE;
					}
					break;    

				default:

					lteWarning("Invalid reporting type\n");
					return MAC_FAILURE;
					break;
			}

			break;

		case TX_MODE_4:
			/* + TM6_5.2 */
		case TX_MODE_6:
			/* - TM6_5.2 */
			switch (cqiTypeIndicator)
			{   
				case PERIODIC_CQI_REPORTING_MODE:

					if( (PERIODIC_MODE_1_0  == periodicRepMode) || 
							(PERIODIC_MODE_2_0  == periodicRepMode) )
					{
						return MAC_FAILURE;
					}
					break;

				case APERIODIC_CQI_REPORTING_MODE:
					if( (CQI_MODE_2_0 == aperiodicRepMode) ||
							(CQI_MODE_3_0 == aperiodicRepMode) ) 
					{
						return MAC_FAILURE;
					}    
					break;

				case PERIODIC_APERIODIC_MODE:

					if( (PERIODIC_MODE_1_0  == periodicRepMode)  || 
							(PERIODIC_MODE_2_0  == periodicRepMode)  ||
							(CQI_MODE_2_0 == aperiodicRepMode)       ||
							(CQI_MODE_3_0 == aperiodicRepMode) )
					{
						return MAC_FAILURE;
					}
					break;     

				default:

					lteWarning("Invalid reporting type\n");
					return MAC_FAILURE;

					break;
			} 
			break;
			/* + TM7_8 Changes Start */
		case TX_MODE_8:

			if(TRUE == pmiRiReportV920)
			{
				switch (cqiTypeIndicator)
				{   
					case PERIODIC_CQI_REPORTING_MODE:

						if( (PERIODIC_MODE_1_0  == periodicRepMode) || 
								(PERIODIC_MODE_2_0  == periodicRepMode) )
						{
							return MAC_FAILURE;
						}
						break;

					case APERIODIC_CQI_REPORTING_MODE:
						if( (CQI_MODE_2_0 == aperiodicRepMode) ||
								(CQI_MODE_3_0 == aperiodicRepMode) ) 
						{
							return MAC_FAILURE;
						}    
						break;

					case PERIODIC_APERIODIC_MODE:

						if( (PERIODIC_MODE_1_0  == periodicRepMode)  || 
								(PERIODIC_MODE_2_0  == periodicRepMode)  ||
								(CQI_MODE_2_0 == aperiodicRepMode)       ||
								(CQI_MODE_3_0 == aperiodicRepMode) )
						{
							return MAC_FAILURE;
						}
						break;     

					default:

						lteWarning("Invalid reporting type\n");
						return MAC_FAILURE;

						break;
				} 

			}
			else
			{
				switch (cqiTypeIndicator)
				{   
					case PERIODIC_CQI_REPORTING_MODE:
						if( (PERIODIC_MODE_1_1  == periodicRepMode) || 
								(PERIODIC_MODE_2_1  == periodicRepMode) )
						{
							return MAC_FAILURE;
						}
						break;

					case APERIODIC_CQI_REPORTING_MODE:
						if( (CQI_MODE_1_2 == aperiodicRepMode) || 
								(CQI_MODE_2_2 == aperiodicRepMode) ||
								(CQI_MODE_3_1 == aperiodicRepMode) ) 
						{
							return MAC_FAILURE;
						}    
						break;

					case PERIODIC_APERIODIC_MODE:

						if( (PERIODIC_MODE_1_1  == periodicRepMode)  || 
								(PERIODIC_MODE_2_1  == periodicRepMode)  ||
								(CQI_MODE_1_2 == aperiodicRepMode)       ||
								(CQI_MODE_2_2 == aperiodicRepMode)       ||
								(CQI_MODE_3_1 == aperiodicRepMode) )
						{
							return MAC_FAILURE;
						}
						break;    

					default:

						lteWarning("Invalid reporting type\n");
						return MAC_FAILURE;
						break;
				}

			}
			break;
			/* - TM7_8 Changes End */ 

		default:

			lteWarning("Unsupported/Invalid transmission Mode\n");
			return MAC_FAILURE;
			break;
	}    

	return MAC_SUCCESS;
}
/* - CQI_5.0 */

/* + TM7_8 Changes Start */
/****************************************************************************
 * Function Name  : validateAperiodicModeForTXMode8 
 * Inputs         : aperiodicCqiMode - configured Aperiodic Mode
 *                  pmiRiReportV920 - configured pmiRiReportV920
 *                    
 * Output         : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function will validate the aperiodic CQI mode against 
 *                  transmission mode 8. If PmiRI report is configured 
 *                  and  aperiodic mode 20/ aperiodic 30 is configured then
 *                  return failure or if PmiRI report is not configured
 *                  and aperiodic 12/aperiodic 22/aperiodic 31 is configured
 *                  then return failure.
 *****************************************************************************/


MacRetType validateAperiodicModeForTXMode8(UInt8 aperiodicCQIMode,
		UInt8 pmiRiReportV920)
{
	if(TRUE == pmiRiReportV920)
	{
		if( (CQI_MODE_2_0 == aperiodicCQIMode) || (CQI_MODE_3_0 == aperiodicCQIMode) )
		{
			return MAC_FAILURE;
		}    
	}
	else 
	{
		if( (CQI_MODE_1_2 ==  aperiodicCQIMode) || 
				(CQI_MODE_2_2 == aperiodicCQIMode) ||
				(CQI_MODE_3_1 == aperiodicCQIMode) )
		{
			return MAC_FAILURE;
		}    
	}

	return MAC_SUCCESS;
}    

/* - TM7_8 Changes End */


/** HD FDD Changes **/
#ifdef HD_FDD_CONFIG
/****************************************************************************
 * Function Name  :  deleteUeFromHdFddContext
 * Inputs         :  ueDLContext_p ipointer to Uplink UE context
 *                   internalCellIndex - internalCellIndex of UE
 * Output         :  None
 * Returns        :  None
 * Description    :  This function will delete the entry in HD FDD UE context
 *                   for particular UE.
 ******************************************************************************/
 void deleteUeFromHdFddContext(DLUEContext* ueDLContext_p,
		/* CA changes Start */
		InternalCellIndex internalCellIndex
		/* CA Changes end */
		)
{
	UInt8 ctr;
	UInt8 doShift=FALSE;

	/* If there is any HD UE and the isHDFddFlag is true */
	if((hdUeContextInfo_g.numHdUe) && (TRUE == ueDLContext_p->isHDFddFlag))
	{
		/* loop check the HD UE to delete and shift the HD UE array after deletion of HD UE */
		for(ctr=0; ctr<hdUeContextInfo_g.numHdUe; ctr++)
		{
			if(ueDLContext_p->ueIndex == hdUeContextInfo_g.hdUeIndex[ctr])
			{
				doShift = TRUE;

				/* Delete the UE from HD context */
				hdUeContextInfo_g.numHdUe--;
				gMacStats.pStats->lteMacHdFddStats.numOfHdFddUE--;
				LOG_MAC_MSG(MAC_HD_FDD_UE_DELETED,LOGINFO,MAC_UL_Strategy,
						GET_CURRENT_TICK_FOR_CELL(internalCellIndex),ueDLContext_p->ueIndex,
						internalCellIndex,DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME,
						"MAC_HD_FDD_UE_DELETED");
				RESET_UE_STATS_RESOURCES(ueDLContext_p->ueIndex);
				ctr++;
				break;
			}
		}

		/* If UE context found in list */
		if(TRUE == doShift)
		{
			hdUeContextInfo_g.hdUeIndex[ctr-1] = hdUeContextInfo_g.hdUeIndex[hdUeContextInfo_g.numHdUe];
		}
		else
		{
			lteWarning ("UE Context not found for index:%u\n", ueDLContext_p->ueIndex);
		}
	}

	return;
}

/****************************************************************************
 * Function Name  :  hdFddCtxConfigReconfigUpdate
 * Inputs         :  ueIndex - UE id
 * Output         :  None
 * Returns        :  None
 * Description    :  This update HD FDD UE Context during UE config and 
 *                   Reconfig
 ******************************************************************************/
 void hdFddCtxConfigReconfigUpdate(UInt16 ueIndex)
{
	UInt8 ctr=0;

	/* CA changes Start */
	InternalCellIndex internalCellIndex;
	internalCellIndex = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->internalCellIndex;
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* CA Changes end */
	if(MAC_HD_FDD != cellParams_p->duplexingMode)
	{
		/* Higher layer should not configure such UE 
		 * If Cell do not support HD FDD, then UE will remain same as reported in UE capabilities
		 * Print LOG and process the request */
		LOG_MAC_MSG(MAC_HD_FDD_INVALID_CELL_DUPLEX_MODE,LOGWARNING,MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),ueIndex,
				cellParams_p->duplexingMode,internalCellIndex,
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,
				"Cell do not support HD FDD mode");
	}

	/* Add UE in global table */

	/* Initialize the global data base for the ue-index */
	hdUeContextInfo_g.configChange[ueIndex] = FALSE;
	hdUeContextInfo_g.ulSrPresentFlag[ueIndex] = FALSE;
	/* Coverity 54583 fix Start*/
	memSet ( hdUeContextInfo_g.hdfddUeSchedMap , 0 , 
			(HD_UE_SCHED_MAP_SIZE * sizeof(HdUeSchedMap))); 
	/* Coverity 54583 fix Start*/

	for (ctr=0; ctr<HD_UE_NUM_OF_REPORTS; ctr++)
	{
		hdUeContextInfo_g.hdUeReportingInfo[ueIndex][ctr].oneShot = 0;
		hdUeContextInfo_g.hdUeReportingInfo[ueIndex][ctr].reportOffset = 0;
		hdUeContextInfo_g.hdUeReportingInfo[ueIndex][ctr].periodicity = 0;
		hdUeContextInfo_g.hdUeReportingTimeOuts[ueIndex][ctr] = 0;
	}

	/* Add UE at last */
	hdUeContextInfo_g.hdUeIndex[hdUeContextInfo_g.numHdUe] = ueIndex;
	hdUeContextInfo_g.numHdUe++;
	LOG_MAC_MSG(MAC_HD_FDD_UE_ADDED,LOGINFO,MAC_UL_Strategy,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),ueIndex,
			internalCellIndex,DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			FUNCTION_NAME,
			"MAC_HD_FDD_UE_ADDED");

	/* Update statistics */
	gMacStats.pStats->lteMacHdFddStats.hdFddFlag[ueIndex] = TRUE;
	gMacStats.pStats->lteMacHdFddStats.numOfHdFddUE++;

	return;
}

/****************************************************************************
 * Function Name  :  hdFddCtxReconfigUpdateReports 
 * Inputs         :  ulUEContext_p - pointer to Uplink UE context
 *                   internalCellIndex - Cell-Index at MAC
 * Output         :  None
 * Returns        :  None
 * Description    :  This update HD FDD UE Context reports during UE config and 
 *                   Reconfig. This map is updated here because in some cases it is possible
 that UE is not in HD mode and reports are processed before marking HdFddFlag
 ******************************************************************************/
 void hdFddCtxReconfigUpdateReports(ULUEContext* ulUEContext_p,
		InternalCellIndex internalCellIndex)
{

	/* Update the periodicity maps when Half duplex flag is set for UE */
	/* Update CQI/PMI/RI maps */
	/* if current map is not NULL, means the reports are to be updated 
	 * checking only CQI map not RI as both are deleted at the same time */
    /*SPR 16855 +-*/
	/* PUCCH TDD ADAPTATION CHANGES Start */        
	if (ulUEContext_p->pucchConfigInfo.pucchNodesForCqi[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex].count)
        /* PUCCH TDD ADAPTATION CHANGES End */        
        /*SPR 16855 +-*/
		{
			/* Reconfig all the CQI/PMI/RI parameters in static map */
            /*SPR 16855 +-*/
            updateHDFddSemiStaticMap(ulUEContext_p->ueIndex, HD_UE_STATIC_MAP_CQI_PMI);
			updateHDFddSemiStaticMap(ulUEContext_p->ueIndex, HD_UE_STATIC_MAP_RI);
            /*SPR 16855 +-*/
		}

	/* The SR half duplex map not updated in above conditions because there may be
	 * deletion and updation and will take unnecessary 2 operations */
	/* If SRS periodicity is short */
	if (SHORT_SR_PERIODICITY > ulUEContext_p->pucchConfigInfo.srPeriodicity)
	{
		/* if current map is not NULL, means the reports are to be updated */
        /*SPR 16855 +-*/
		/* PUCCH TDD ADAPTATION CHANGES Start */        
		if (ulUEContext_p->pucchConfigInfo.pucchShortSRNodeHead_p[shortPeriodicSRMapShortInfo_gp[internalCellIndex]->activeIndex])
			/* PUCCH TDD ADAPTATION CHANGES End */        
        /*SPR 16855 +-*/
			{
				/* Reconfig all the SR parameters in static map */
                /*SPR 16855 +-*/
				updateHDFddSemiStaticMap(ulUEContext_p->ueIndex, HD_UE_STATIC_MAP_SR);
                /*SPR 16855 +-*/
			}
	}
	else /* long SR */
	{
		/* if current map is NULL, means the reports are deleted */
        /*SPR 16855 +-*/
		/* PUCCH TDD ADAPTATION CHANGES Start */        
		if (ulUEContext_p->pucchConfigInfo.pucchNodesForSR[longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex].count)
			/* PUCCH TDD ADAPTATION CHANGES End */        
            /*SPR 16855 +-*/
			{
				/* Reconfig all the SR parameters in static map */
                /*SPR 16855 +-*/
				updateHDFddSemiStaticMap(ulUEContext_p->ueIndex, HD_UE_STATIC_MAP_SR);
                /*SPR 16855 +-*/
			}
	}

	/* Update SRS bitmap as well */
	updateHDFddSemiStaticMapSRS(
            /*SPR 16855 +-*/
			ulUEContext_p->ueIndex,
            /*SPR 16855 +-*/
			ulUEContext_p->srsConfigInfo.srsPeriodicity,
            /* +- SPR 17777 */
			ulUEContext_p->srsConfigInfo.srsDedicatedSetupInfo.duration,
			ulUEContext_p->srsConfigInfo.srsConfigType);

	return;
}
#endif
/** HD FDD Changes **/

/*****************************************************************************
 * Function Name  : processUEReconfigCompInd
 * Inputs         : ueReconfigCompInd_p - Pointer to UEReconfigCompInd
 *                : internalCellIndex - Cell-Index at MAC
 * Output         : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This method will parse the passed buffer that is received
 from RRC when SPS reconfiguration is happening in 
 RRCConnectionReconfiguration or RRCConnectionSetup message.
 *****************************************************************************/
/*CA Changes start  */
MacRetType processUEReconfigCompInd(UEReconfigCompInd *ueReconfigCompInd_p, InternalCellIndex internalCellIndex)
	/*CA Changes end  */
{
	UInt16 ueIndex = ueReconfigCompInd_p->ueIndex;
	DLUEContext *dlNewUEContext_p = PNULL;
	ULUEContext *ulNewUEContext_p = PNULL;
    /* +- SPR 17777 */

	/* Coverity_73207 Fix Start */
	if(ueIndex < MAX_UE_SUPPORTED)
	{
		dlNewUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
		ulNewUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
	}
	/* Coverity_73207 Fix End */

	/* Check If UE Context is created or not */
	if (PNULL == dlNewUEContext_p && PNULL == ulNewUEContext_p)
	{
		return MAC_FAILURE;
	}

	if (MAC_SUCCESS == ueReconfigCompInd_p->response)
	{
		/* Check If SPS DL or UL state has to be chnage */
		if (PNULL != dlNewUEContext_p && 
				dlNewUEContext_p->spsDlInfo.spsState == INVALID_STATE &&
				SPS_SETUP == dlNewUEContext_p->spsDlInfo.requestType)
		{
			dlNewUEContext_p->spsDlInfo.spsState = SPS_IDLE;
            /* +- SPR 17777 */
			/* Call state machine for setting state to SPS_CONFIGURED  */
			if( MAC_FAILURE == dlStateMachine[dlNewUEContext_p->spsDlInfo.spsState]
					[SPS_CONFIG_RECVD]( 
						ueIndex, dlNewUEContext_p->spsDlInfo.spsState,
						SPS_CONFIG_RECVD, dlNewUEContext_p, PNULL,
                        /* + SPS_TDD_Changes*/
#ifdef FDD_CONFIG
                        PHY_DL_DELAY
#else
                        DL_ADVANCE_TICK
#endif
                        /* - SPS_TDD_Changes*/
                        ,internalCellIndex) )
			{
                /* +- SPR 17777 */
                LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, ueIndex,dlNewUEContext_p->spsDlInfo.spsState, SPS_CONFIG_RECVD,
                     dlNewUEContext_p->spsDlInfo.spsState, 0,0, __func__, "DlStateMachine");
				return MAC_FAILURE;
			}
            LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                 getCurrentTick(), __LINE__, ueIndex,dlNewUEContext_p->spsDlInfo.spsState, SPS_CONFIG_RECVD,
                 dlNewUEContext_p->spsDlInfo.spsState, 0,0, __func__, "DlStateMachine");
            /* +- SPR 17777 */
		}
		if (PNULL != ulNewUEContext_p && 
				ulNewUEContext_p->spsUlInfo.spsState == INVALID_STATE &&
				SPS_SETUP == ulNewUEContext_p->spsUlInfo.requestType)
		{
			ulNewUEContext_p->spsUlInfo.spsState = SPS_IDLE;
            /* +- SPR 17777 */
			/* Call state machine for setting state to SPS_CONFIGURED  */
			if( MAC_FAILURE == ulStateMachine[ulNewUEContext_p->spsUlInfo.spsState]
					[SPS_CONFIG_RECVD]( 
						ueIndex, ulNewUEContext_p->spsUlInfo.spsState,
						SPS_CONFIG_RECVD,  ulNewUEContext_p, PNULL,
                        /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
                        PHY_DL_DELAY
#else
                        DL_ADVANCE_TICK
#endif
                        /* - SPS_TDD_Changes */
						/*CA Changes start  */
                        , internalCellIndex ) )
				/*CA Changes end  */
			{
                /* +- SPR 17777 */
                LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, ueIndex,ulNewUEContext_p->spsUlInfo.spsState, SPS_CONFIG_RECVD,
                     ulNewUEContext_p->spsUlInfo.spsState, 0,0, __func__, "UlStateMachine");
				return MAC_FAILURE;
			}
            LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, ueIndex,ulNewUEContext_p->spsUlInfo.spsState, SPS_CONFIG_RECVD,
                     ulNewUEContext_p->spsUlInfo.spsState, 0,0, __func__, "UlStateMachine");
            /* +- SPR 17777 */
		}
	}
	else
	{
		/* Check If SPS DL or UL state has to be chnage */
		if (PNULL != dlNewUEContext_p && 
				dlNewUEContext_p->spsDlInfo.spsState == INVALID_STATE &&
				SPS_SETUP == dlNewUEContext_p->spsDlInfo.requestType)
		{
			if (PNULL == ulNewUEContext_p || \
					SPS_RELEASE == ulNewUEContext_p->spsUlInfo.requestType)
			{
				deAllocateSpsCrnti(dlNewUEContext_p->spsDlInfo.SpsCrnti,
						internalCellIndex);
				/* Reset the SPS Crnti in DL UE Context */
				dlNewUEContext_p->spsDlInfo.SpsCrnti = 0;
				dlNewUEContext_p->spsDlInfo.requestType = SPS_RELEASE;
				if (PNULL != ulNewUEContext_p)
				{
					ulNewUEContext_p->spsUlInfo.SpsCrnti = 0;
				}
				freeMemPool( dlNewUEContext_p->spsDlInfo.spsDlSetupInfo_p );
				dlNewUEContext_p->spsDlInfo.spsDlSetupInfo_p = PNULL;
				dlNewUEContext_p->spsDlInfo.spsState = INVALID_STATE;
			}
			else
			{
				/* Reset the SPS Crnti in DL UE Context */
				dlNewUEContext_p->spsDlInfo.SpsCrnti = 0;
				dlNewUEContext_p->spsDlInfo.requestType = SPS_RELEASE;
				freeMemPool( dlNewUEContext_p->spsDlInfo.spsDlSetupInfo_p );
				dlNewUEContext_p->spsDlInfo.spsDlSetupInfo_p = PNULL;
				dlNewUEContext_p->spsDlInfo.spsState = INVALID_STATE;
			}
		}
		if (PNULL != ulNewUEContext_p && 
				ulNewUEContext_p->spsUlInfo.spsState == INVALID_STATE &&
				SPS_SETUP == ulNewUEContext_p->spsUlInfo.requestType)
		{
			if (PNULL == dlNewUEContext_p || \
					SPS_RELEASE == dlNewUEContext_p->spsDlInfo.requestType)
			{
				deAllocateSpsCrnti(ulNewUEContext_p->spsUlInfo.SpsCrnti,internalCellIndex);
				/* Reset the SPS Crnti in DL UE Context */
				ulNewUEContext_p->spsUlInfo.SpsCrnti = 0;
				ulNewUEContext_p->spsUlInfo.requestType = SPS_RELEASE;
				if (PNULL != dlNewUEContext_p)
				{
					dlNewUEContext_p->spsDlInfo.SpsCrnti = 0;
				}
				freeMemPool(ulNewUEContext_p->spsUlInfo.spsUlSetupInfo_p);
				ulNewUEContext_p->spsUlInfo.spsUlSetupInfo_p = PNULL;
				ulNewUEContext_p->spsUlInfo.spsState = INVALID_STATE;
			}
			else
			{
				/* Reset the SPS Crnti in DL UE Context */
				ulNewUEContext_p->spsUlInfo.SpsCrnti = 0;
				ulNewUEContext_p->spsUlInfo.requestType = SPS_RELEASE;
				freeMemPool(ulNewUEContext_p->spsUlInfo.spsUlSetupInfo_p );
				ulNewUEContext_p->spsUlInfo.spsUlSetupInfo_p = PNULL;
				ulNewUEContext_p->spsUlInfo.spsState = INVALID_STATE;
			}
		}
	}
#ifdef SSI_DEBUG_STATS
	if (PNULL != dlNewUEContext_p)
	{
		DLLogicalChannelInfo *spsLchInfo_p = PNULL;    
		if (INVALID_LCID !=  dlNewUEContext_p->spsDlInfo.lcId)
		{
			spsLchInfo_p = &(dlNewUEContext_p->logicalChannel[
					dlNewUEContext_p->spsDlInfo.lcId]);
		}
		LTE_MAC_UPDATE_DOWNLINK_SPS_UE_STATS(dlNewUEContext_p->ueIndex,
				&dlNewUEContext_p->spsDlInfo, spsLchInfo_p)
	}
	if (PNULL != ulNewUEContext_p)
	{
		LTE_MAC_UPDATE_UPLINK_SPS_UE_STATS(ulNewUEContext_p->ueIndex, 
				&ulNewUEContext_p->spsUlInfo );
	}
#endif
	return MAC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : fillHarqReleaseinfoForULSCH 
 * Inputs         : ulDelay - uplink Delay
 *                  currentGlobalTick - current TTI at MAC
 *                  ueULContext_p - Pointer to uplink UE context
 *                : internalCellIndex - CellIndex at AMC
 * Outputs        : None
 * Returns        : None 
 * Description    : Update the HARQ Release information for ULSCH
 *****************************************************************************/
/* SPR 11215 Changes Start */
/*CID 83622 Fix Start*/
/* +- SPR 22354 */
#if defined(FAPI_1_1_COMPLIANCE) || defined(FAPI_4_0_COMPLIANCE)
#ifdef FDD_CONFIG
 STATIC void fillHarqReleaseinfoForULSCH(UInt8 ulDelay,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
		ULUEContext *ueULContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
#elif TDD_CONFIG
 STATIC void fillHarqReleaseinfoForULSCH(
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
		ULUEContext *ueULContext_p,
		/*CA Changes start  */
		InternalCellIndex internalCellIndex)
/*CA Changes end  */
#endif
{
	/* SPR 11215 Changes End */
	UInt16 numberOfInfoPresent = 0;
	UInt8 containerTick = 0;
#ifdef TDD_CONFIG
	containerTick = (currentGlobalTick )% MAX_PDCCH_CONTAINER;
#elif FDD_CONFIG
	containerTick = (currentGlobalTick + ulDelay)% MAX_PDCCH_CONTAINER;
#endif   

	/*CA Changes start  */
	ULSchUciRachPoolInfo* ulschUciRachInfo_p
		= ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
	/*CA Changes end  */
	numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
	ulschUciRachInfo_p->
		ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_HARQ_BUFFER_RELEASE; 
	ulschUciRachInfo_p->
		ulschUciInfo[numberOfInfoPresent].harqReleaseInfo.rnti 
		= ueULContext_p->crnti ;
	ulschUciRachInfo_p->numberOfInfoPresent++;

}
#endif
/*CID 83622 Fix end*/ 
/* +- SPR 22354 */

#ifdef MAC_AUT_TEST
/*****************************************************************************
 * Function Name  : local_createMACDLUEEntity 
 * Inputs         : ueDlConfigInfo_p - Pointer to Downlink UE context
 *                : internalCellIndex - CellIndex at MAC
 * Outputs        : None
 * Returns        : MacRetType(MAC_FAILURE or MAC_SUCCESS) 
 * Description    : create the DL UE context for automation suits to verify 
 *                  the UE context created successfully or not
 *****************************************************************************/
MacRetType local_createMACDLUEEntity( UeContextForDownlink* ueDlConfigInfo_p, InternalCellIndex internalCellIndex)
{
	return createMACDLUEEntity(ueDlConfigInfo_p,internalCellIndex);
}

/*****************************************************************************
 * Function Name  : local_createMACDLHarqEntity 
 * Inputs         : harqDLContext_pp - Pointer to Downlink Harq context
 *                  numHarqProcess
 * Outputs        : None
 * Returns        : MacRetType(MAC_FAILURE or MAC_SUCCESS) 
 * Description    : create the DL HARQentity for automation suits to verify 
 *                  the creation successfull or not
 *****************************************************************************/
MacRetType local_createMACDLHarqEntity( DLHARQEntity **harqDLContext_pp,
		UInt32 numHarqProcess)
{
	return createMACDLHarqEntity( harqDLContext_pp,
			numHarqProcess);
}

/*****************************************************************************
 * Function Name  : local_reconfigureMACDLUEEntity 
 * Inputs         : dwlkUEInfo_p - Pointer to Downlink UE context
 *                  dlReconfigUeReq_p - Downlink UE Reconfig info
 *                : internalCellIndex - CellIndex at MAC
 * Outputs        : None
 * Returns        : MacRetType(MAC_FAILURE or MAC_SUCCESS) 
 * Description    : Reconfigures the DL UE context for automation suits to verify 
 *                  the UE context reconfigured successfully or not
 *****************************************************************************/
MacRetType local_reconfigureMACDLUEEntity ( UeContextForDownlink *dwlkUEInfo_p, 
		DLReconfigUeReq *dlReconfigUeReq_p, InternalCellIndex internalCellIndex)
{
	return reconfigureMACDLUEEntity(dwlkUEInfo_p, dlReconfigUeReq_p, internalCellIndex);
}

/*****************************************************************************
 * Function Name  : local_deletePeriodicReportsNode 
 * Inputs         : pucchConfig_p - Pointer to PUCCH of UE context
 *                : internalCellIndex - internalCellIndex of UE
 * Outputs        : None
 * Returns        : None 
 * Description    : Deletet the pucch nodes configured earlier              
 *****************************************************************************/
void local_deletePeriodicReportsNode(PucchConfig *pucchConfig_p,
		InternalCellIndex internalCellIndex)
{
#ifdef FDD_CONFIG
	/*CA Changes start  */
	deletePeriodicReportsNode(pucchConfig_p,
			internalCellIndex);
	/*CA Changes end  */
#endif


}
#endif

/*+ Dyn UEs To Be SchPerTTI +*/
/*****************************************************************************
 * Function Name  : validateAndUpdateMaxUeToBeSchdDlUl
 * Inputs         : MaxUeToBeSchdDL - maximum UE to be sceduled in DL received from RRM
 *                : MaxUeToBeSchdUL - maximum UE to be sceduled in UL received from RRM
 *                : internalCellIndex - internalCellIndex  at MAC
 * Outputs        : None
 * Returns        : MacRetType(MAC_FAILURE or MAC_SUCCESS) 
 * Description    : validate and update MaxUeToBeSchdDL and MaxUeToBeSchdUL
 *****************************************************************************/
MacRetType validateAndUpdateMaxUeToBeSchdDlUl(UInt8 maxUeToBeSchdDL, UInt8 maxUeToBeSchdUL
		,InternalCellIndex internalCellIndex)
{
	/*+ Dyn UEs To Be SchPerTTI:CR +*/
	if((modifyMaxUeToBeSchdDlUl_g[internalCellIndex] == MODIFY_ONLY)&&
			((cellSpecificParams_g.cellConfigAndInitParams_p\
			  [internalCellIndex]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL \
			  != maxUeToBeSchdDL)||
			 (cellSpecificParams_g.cellConfigAndInitParams_p\
			  [internalCellIndex]->initParams_p->ulSchdConfig.maxUEsToBeScheduledUL != maxUeToBeSchdUL)))
	{

		cellSpecificParams_g.cellConfigAndInitParams_p\
			[internalCellIndex]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL \
			= maxUeToBeSchdDL;
		cellSpecificParams_g.cellConfigAndInitParams_p\
			[internalCellIndex]->initParams_p->ulSchdConfig.maxUEsToBeScheduledUL = maxUeToBeSchdUL;
		if(cellSpecificParams_g.cellConfigAndInitParams_p\
				[internalCellIndex]->initParams_p->dlSchdConfig.maxCellEdgeUEsToBeScheduledDL\
				> cellSpecificParams_g.cellConfigAndInitParams_p\
				[internalCellIndex]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL)
		{
			cellSpecificParams_g.cellConfigAndInitParams_p\
				[internalCellIndex]->initParams_p->dlSchdConfig.maxCellEdgeUEsToBeScheduledDL = 
				cellSpecificParams_g.cellConfigAndInitParams_p\
				[internalCellIndex]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL;
		}
		if(cellSpecificParams_g.cellConfigAndInitParams_p\
				[internalCellIndex]->initParams_p->ulSchdConfig.maxCellEdgeUEsToBeScheduledUL > 
				cellSpecificParams_g.cellConfigAndInitParams_p\
				[internalCellIndex]->initParams_p->ulSchdConfig.maxUEsToBeScheduledUL)
		{
			cellSpecificParams_g.cellConfigAndInitParams_p\
				[internalCellIndex]->initParams_p->ulSchdConfig.maxCellEdgeUEsToBeScheduledUL = 
				cellSpecificParams_g.cellConfigAndInitParams_p\
				[internalCellIndex]->initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
		}
        /*SPR 18564 + */
        initDlQosRbRestriction(internalCellIndex);
        initUlQosRbRestriction(internalCellIndex);
        /*SPR 18564 - */

	} 
	else if(modifyMaxUeToBeSchdDlUl_g[internalCellIndex] == VALIDATE_ONLY) 
	{
		/*- Dyn UEs To Be SchPerTTI:CR -*/
		modifyMaxUeToBeSchdDlUl_g[internalCellIndex] = NO_ACTION;
		if(maxUeToBeSchdDL > 0 && maxUeToBeSchdDL <= 15 &&
				maxUeToBeSchdUL > 0 && maxUeToBeSchdUL <= 15 &&
				maxUeToBeSchdDL + maxUeToBeSchdUL <= 16)
		{
			LOG_MAC_MSG(MAC_RRM_RECONFIG_UE_TO_BE_SCHEDULED_DL_UL,\
					LOGWARNING,\
					MAC_L1_INF,\
					GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
					maxUeToBeSchdDL,\
					/*+BUG:9222*/
					maxUeToBeSchdUL,\
					/*-BUG:9222*/
					MAC_SUCCESS,\
					internalCellIndex,0,0,0,\
					__func__," ");
			return MAC_SUCCESS;
		} 
		LOG_MAC_MSG(MAC_RRM_RECONFIG_UE_TO_BE_SCHEDULED_DL_UL,\
				LOGWARNING,\
				MAC_L1_INF,\
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				maxUeToBeSchdDL,\
				/*+BUG:9222*/
				maxUeToBeSchdUL,\
				/*-BUG:9222*/
				MAC_FAILURE,\
				internalCellIndex,0,0,0,\
				__func__, " ");
		return MAC_FAILURE;

	}
	return MAC_SUCCESS;
}
/*- Dyn UEs To Be SchPerTTI -*/

/* CA changes Start*/
/****************************************************************************
 * Function Name  : macUpdateCreateDLCAParams 
 * Inputs         : newUEcontext_p - Pointer to DL UE context
 *                  ueDlConfigInfo_p - Pointer to UeContextForDownlink
 *                  internalCellIndex - Cell index at MAC
 * Outputs        : None 
 * Returns        : None
 * Description    : The function is used to fill new SCELL DL UE context parameters
 ****************************************************************************/
STATIC  void macUpdateCreateDLCAParams(
		DLUEContext        *newUEcontext_p,
		UeContextForDownlink *ueDlConfigInfo_p,
		InternalCellIndex internalCellIndex
		)
{
	UInt8 n1PucchList = 0, n1PucchCs = 0, index= 0;
	UInt32 totalSoftchannelBytes = 0;
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	SInt8 gbrLcCount = newUEcontext_p->gbrLcCount; 
	UInt8 *gbrLcInfo_p = newUEcontext_p->gbrLcInfo;
	UInt8 idx = 0;
	DlLcQosInfo *dlLcQosInfo_p = PNULL;
	/* populate newly created UE context with values received from L3,
	 * otherwise initialise with default values already present in 
	 * ueDlConfigInfo_p*/
	newUEcontext_p->scellDeactivationTimer = 
		ueDlConfigInfo_p->carrierAggrConfig.scellDeactivationTimer;
	if (TRUE == ueDlConfigInfo_p->carrierAggrConfig.isBandWidthTagPresent)
	{
		/* Currently only 1 scell with scellIndex 1 is expected */
		for (index= PCELL_SERV_CELL_IX; index <= MAX_NUM_SCELL; index++)
		{
			newUEcontext_p->bandWidthDistribution[index] = 
				ueDlConfigInfo_p->carrierAggrConfig.bandWidthDistribution
				[index];
		}

		while(gbrLcCount--)
		{
			if(newUEcontext_p->logicalChannel[gbrLcInfo_p[gbrLcCount]].logicalChannelId != INVALID_LCID)
			{
				dlLcQosInfo_p = &(newUEcontext_p->logicalChannel[gbrLcInfo_p[gbrLcCount]].dlLcQosInfo);
				for(idx = PCELL_SERV_CELL_IX; idx <= MAX_NUM_SCELL; idx++)
				{
					dlLcQosInfo_p->bandWidthDistribution[idx] = newUEcontext_p->bandWidthDistribution[idx];
				}
			}
		}
	}
	newUEcontext_p->pucchFormatR10.requestType = 
		ueDlConfigInfo_p->pucchFormatR10.requestType;
	for (n1PucchList = 0; n1PucchList < MAX_PUCCH_AN_CS_LIST; n1PucchList++)
	{
		for (n1PucchCs = 0; n1PucchCs < MAX_PUCCH_AN_CS; n1PucchCs++)
		{
			newUEcontext_p->pucchFormatR10.n1Pucch[n1PucchList][n1PucchCs]=
				ueDlConfigInfo_p->pucchFormatR10.n1Pucch[n1PucchList][n1PucchCs];
		}
	}
	newUEcontext_p->scellCount = ueDlConfigInfo_p->carrierAggrConfig.scellCount;

	if(TRUE == ueDlConfigInfo_p->ueCategoryV1020ReqType)
	{
		newUEcontext_p->ueCategoryV1020 = ueDlConfigInfo_p->ueCategoryV1020;
		/*debug stats*/
		LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UE_CATEGORY(
				newUEcontext_p->ueIndex,
				newUEcontext_p->ueCategory);

		/*Update the max TB size for the category of the UE  */
		newUEcontext_p->maxTBSize = 
			DIVIDE_BY_EIGHT(cellParams_p->dlUeCategoryInfo[newUEcontext_p->ueCategoryV1020 - 1].
					maxBitsDLSCHTb);

		newUEcontext_p->maxSISOTBSize = 
			DIVIDE_BY_EIGHT(cellParams_p->dlUeCategoryInfo[newUEcontext_p->ueCategoryV1020 - 1].
					maxBitsDLSCHTb);

		newUEcontext_p->maxMIMOTBSize = 
			DIVIDE_BY_EIGHT(cellParams_p->dlUeCategoryInfo[newUEcontext_p->ueCategoryV1020 - 1].
					maxDLSCHTbBits);
		totalSoftchannelBytes = DIVIDE_BY_EIGHT(\
				cellParams_p->dlUeCategoryInfo[newUEcontext_p->ueCategoryV1020 - 1].totSoftChBits);

		newUEcontext_p->permissibleTBSize = (((totalSoftchannelBytes)/MAC_CODING_FACTOR)/ 
				(newUEcontext_p->scellCount +1));
		newUEcontext_p->maxPermissibleTBSize = newUEcontext_p->permissibleTBSize;
	}
}

/****************************************************************************
 * Function Name  : macUpdateCreateDLScellParams 
 * Inputs         : newUEcontext_p - Pointer to DL UE context
 *                  ueDlConfigInfo_p - Pointer to UeContextForDownlink
 *                  index - index having scell information in temperory context
 *                  internalCellIndex - the cell index used at MAC
 * Outputs        : None 
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : The function is used to fill new SCELL DL UE context parameters
 ****************************************************************************/
STATIC  MacRetType macUpdateCreateDLScellParams(
		DLUEContext        *newUEcontext_p,
		UeContextForDownlink *ueDlConfigInfo_p,
		UInt8 index,
		InternalCellIndex internalCellIndex
		)
{
	UInt16 ueIndex = 0;
	/* SPR 15909 fix start */
    tickType_t currentTick = 0;
    /* SPR 15909 fix end */
	ueIndex = newUEcontext_p->ueIndex;
	ScellInfo *scellInfo_p;
	UInt8 sCellIndex = 0;
	DLUESCellContext **dlScellContext_pp=PNULL;
	/* + SPR 11204 Changes */
	RNTIInfo    *rntiInfo_p = PNULL;
	/* - SPR 11204 Changes */
	/** SPR 14277 Changes Start **/
#ifdef TDD_CONFIG
	UInt8 loopIndex = 0;
#endif
	/** SPR 14277 Changes End **/
	currentTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
	sCellIndex = ueDlConfigInfo_p->carrierAggrConfig.scellInfo[index].sCellIndex;
	/* TA - integration fix start */
	UInt32 totalSoftchannelBytes = 0;
	CellConfigParams *sCellParams_p = cellSpecificParams_g.\
					  cellConfigAndInitParams_p[sCellIndex]->cellParams_p;
	/* TA - integration fix end */
	dlScellContext_pp = &(newUEcontext_p->dlUeScellContext_p[sCellIndex]);
	if (  PNULL == *dlScellContext_pp)
	{
		GET_MEM_FROM_POOL(DLUESCellContext,*dlScellContext_pp,
				sizeof (DLUESCellContext),PNULL);
		if (PNULL == *dlScellContext_pp )
		{
			/* Not able to allocate Memory */
			return MAC_FAILURE;
		}
		memSet(*dlScellContext_pp,0,
				sizeof(DLUESCellContext));
	}

	scellInfo_p = &(ueDlConfigInfo_p->carrierAggrConfig.scellInfo[index]);

	if (ADD == scellInfo_p->operationType)
	{
		if (MAC_SUCCESS != macFillScellDLUeContextParams (ueIndex, ueDlConfigInfo_p, 
					newUEcontext_p,index, internalCellIndex))
		{
			lteWarning("UE Context params filling failed");
			return MAC_FAILURE; 
		}
        /* +- SPR 17777 */
		if (MAC_SUCCESS != (macFillScellDLCtxtCqiInfo (ueIndex, ueDlConfigInfo_p, newUEcontext_p, 
						currentTick,index)))
            /* +- SPR 17777 */
		{
			return MAC_FAILURE; 
		}
		/*Added for Power Control*/
		(*dlScellContext_pp)->pA = 
			scellInfo_p->pA;
		(*dlScellContext_pp)->ScelldlMIMOInfo.numOfLayerMIMO = 
			scellInfo_p->numOfLayer;  //DL
	 	/* SPR 16422 START */
                /* Fill CQI Validity timer value from cell which is Scell of the UE */
                InternalCellIndex scellId = ueDlConfigInfo_p->carrierAggrConfig.scellInfo[index].internalCellIndex;
		(*dlScellContext_pp)->cqiValidityTimer  = 
                         macReconfigSchedulerParamsDL_gp[scellId][schParamsIndexDL_g[scellId]].caParams.cqiValidityTimer;
		/* SPR 16422 END */
		/* Initialize user location type to CC_USER if userLocationType 
		 * is CE type but cellEdgeRegionCount is zero*/
		(*dlScellContext_pp)->userLocationType = 
			(UserLocation) scellInfo_p->userLocationType;

		if(scellInfo_p->userLocationType == CE_USER)
		{
			++(rrmMacCellGlobal_gp[(*dlScellContext_pp)->internalCellIndex]->totalCellEdgeUserCount);
			if (dlResourcePartitionConfig_g[(*dlScellContext_pp)->internalCellIndex].
					cellEdgeRegionCount == 0)
			{
				lteWarning("No cell edge region exist");
				(*dlScellContext_pp)->
					userLocationType = CC_USER;
			}
		}
		(*dlScellContext_pp)->scheduleFlag = DL_QOS_NEW_TRANSMISSION;
		/** SPR 12457 Fix : Start**/
		(*dlScellContext_pp)->pucchReqInfo.schedulingStatus = NO_TB;
		/** SPR 12457 Fix : End **/

		/** SPR 14277 Changes Start **/
#ifdef TDD_CONFIG
		for(loopIndex = 0; loopIndex < MAX_SUB_FRAME; loopIndex++)
		{
			(*dlScellContext_pp)->pucchReqInfo.sCellSchInfo[loopIndex] = 0;
		}
#endif
		/** SPR 14277 Changes End **/

		/* SPR 11043 changes start */
		(*dlScellContext_pp)->internalCellIndex = scellInfo_p->internalCellIndex;
		/* SPR 11043 changes end */
		/* + SPR 11202 Changes */
		insertNodeInDlCAEventQueueRRCIntf((*dlScellContext_pp)->internalCellIndex,
				(*dlScellContext_pp)->isScellActivateStatus,
				ueIndex,
				internalCellIndex); 
		/* - SPR 11202 Changes */
		/* Update Scell max TB size for the category of the UE */
		/* TA - integration fix start */
		newUEcontext_p->dlUeScellContext_p[sCellIndex]->maxTBSize = 
			DIVIDE_BY_EIGHT(sCellParams_p->dlUeCategoryInfo[newUEcontext_p->ueCategory - 1].
					maxBitsDLSCHTb);

		totalSoftchannelBytes = DIVIDE_BY_EIGHT(\
				sCellParams_p->dlUeCategoryInfo[newUEcontext_p->ueCategoryV1020 - 1].totSoftChBits);

		/* Update Scell permissibleTBSize for the category of the UE */
		newUEcontext_p->dlUeScellContext_p[sCellIndex]->permissibleTBSize = 
			(((totalSoftchannelBytes)/MAC_CODING_FACTOR) / (newUEcontext_p->scellCount +1));
		/* Update Scell maxPermissibleTBSize for the category of the UE */
		newUEcontext_p->dlUeScellContext_p[sCellIndex]->maxPermissibleTBSize = 
			newUEcontext_p->dlUeScellContext_p[sCellIndex]->permissibleTBSize;
		/* TA - integration fix end */

		/*pdcch changes start*/
		/* + SPR 11204 Changes */
		rntiInfo_p = getUeIdxFromRNTIMap(newUEcontext_p->crnti, newUEcontext_p->internalCellIndex);
		/** SPR 13838 Fix : Start **/
		setUeSearchSpaceInfoforScell(PNULL,newUEcontext_p,rntiInfo_p,newUEcontext_p->internalCellIndex );
		/** SPR 13838 Fix : End **/
		/* - SPR 11204 Changes */
		/*pdcch changes end*/
	}
	else
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"Invalid operationType %u ue %u line %d\n",
                scellInfo_p->operationType,newUEcontext_p->ueIndex,__LINE__);
		freeMemPool(newUEcontext_p->dlUeScellContext_p[sCellIndex]);
        /* spr 21927 fix + */
        newUEcontext_p->dlUeScellContext_p[sCellIndex] = PNULL;
        /* spr 21927 fix - */
		return MAC_FAILURE;
	}
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : macFillScellDLUeContextParams 
 * Inputs         : ueIndex - index of ueContext
 *                  ueDlConfigInfo_p - Pointer to UeContextForDownlink
 *                  newUEcontext_p - Pointer to DLUEContext
 *                  index - Index having scell information
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : The function is used to fill new SCELL DL UE context parameters
 ****************************************************************************/
STATIC  MacRetType macFillScellDLUeContextParams ( 
		UInt32               ueIndex,
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p,
		UInt8                index,
		InternalCellIndex internalCellIndex
		)
{
	UInt8 loopCount =0;
	SCELLDLMIMOInfo  *ScelldlMIMOInfo_p    = PNULL;
	ScellInfo *scellInfoTemp_p = &(ueDlConfigInfo_p->carrierAggrConfig.scellInfo[index]);
	UInt8 scellIndex = scellInfoTemp_p->sCellIndex;
	DLCAEventQueueNode *tempEventQNode_p   = PNULL;
	/** SPR 11985 Fix : Start **/
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[scellInfoTemp_p->internalCellIndex]->cellParams_p;
	/** SPR 11985 Fix : End **/
#ifdef TDD_CONFIG
	UInt8 sfIndex = 0;
	UInt8 containerIndex = 0;
	DLUESCellContext *dlUeScellContext_p = PNULL;
#endif    
#ifdef HD_FDD_CONFIG
	newUEcontext_p->dlUeScellContext_p[scellIndex]->isHDFddFlag = 
		scellInfoTemp_p->isHDFddFlag;
#endif
	newUEcontext_p->dlUeScellContext_p[scellIndex]->ScelldlHarqContext_p = PNULL;
	/*spr fix 11038 fix start*/ 
	newUEcontext_p->dlUeScellContext_p[scellIndex]->isScellActivateStatus = scellInfoTemp_p->isScellActivateStatus;
	newUEcontext_p->dlUeScellContext_p[scellIndex]->scellState = DL_CA_SCELL_INIT;
	/*prepare and insert node in mac event q*/
	GET_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p,DLCAEventQueueNode);
	if(tempEventQNode_p != NULL)
	{
		tempEventQNode_p->ueId         = ueIndex;
		tempEventQNode_p->scellId      = scellIndex;
		tempEventQNode_p->dlCAQosEvent = DL_CA_EVENT_RRC_UE_CREATE;
		if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[newUEcontext_p->internalCellIndex],(void *)tempEventQNode_p))
		{
			FREE_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p);
		}
	}
	/* + SPR 11491 Changes */
	newUEcontext_p->dlUeScellContext_p[scellIndex]->isPdcchAllocatedFlag = FALSE;
	/* - SPR 11491 Changes */
	/*spr fix 11038 fix end*/
	if (MAC_SUCCESS != createMACDLHarqEntity( 
				&newUEcontext_p->dlUeScellContext_p[scellIndex]->ScelldlHarqContext_p,
				scellInfoTemp_p->dlNumHarqProcess))
	{
		LOG_MAC_MSG(CREATE_DL_UE_CONTXT_ID,
				LOGINFO,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				ueIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				internalCellIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,"Creation of HARQ entity for Scell failed");
		return MAC_FAILURE;

	}

	/* Initializations */
	/* MIMO related Info */
	ScelldlMIMOInfo_p = &newUEcontext_p->dlUeScellContext_p[scellIndex]->ScelldlMIMOInfo;
	ScelldlMIMOInfo_p->transmissionMode = 
		(TransmissonMode) scellInfoTemp_p->transmissionMode;

	if( (( TX_MODE_3 == ScelldlMIMOInfo_p->transmissionMode) || 
				( TX_MODE_4 == ScelldlMIMOInfo_p->transmissionMode) ||
				(TX_MODE_6 == ScelldlMIMOInfo_p->transmissionMode)) &&
			( ! ((2 == cellParams_p->numOfTxAnteenas ) || 
			     (4 == cellParams_p->numOfTxAnteenas )))
	  )	
	{
        LOG_MAC_ERROR(MAC_RRC_INF,"With TransmissionMode %d, number antennas are neither 2 nor 4\n",
				ScelldlMIMOInfo_p->transmissionMode); 
		return MAC_FAILURE;
	}

	newUEcontext_p->dlUeScellContext_p[scellIndex]->codebookSubsetRestrictionv1020.
		cbsrType = scellInfoTemp_p->codebookSubsetRestrictionv1020.cbsrType;
	for(loopCount =0;loopCount < MAX_CBSR_VALUE_COUNT;loopCount++)
	{
		newUEcontext_p->dlUeScellContext_p[scellIndex]->codebookSubsetRestrictionv1020.
			cbsrValue[loopCount] = 
			scellInfoTemp_p->codebookSubsetRestrictionv1020.cbsrValue[loopCount];
	}
	ScelldlMIMOInfo_p->precodingIndex   = scellInfoTemp_p->precodingIndex;

	if(TX_MODE_8 == ScelldlMIMOInfo_p->transmissionMode)
	{
		newUEcontext_p->dlUeScellContext_p[scellIndex]->isPmiRiConfigured_V920 = 
			scellInfoTemp_p->cqiInfo.pmiRiReportV920;
		if(INVALID_APERIODIC_CQI_MODE != scellInfoTemp_p->cqiInfo.cqiMode)
		{
			if(MAC_FAILURE == validateAperiodicModeForTXMode8(
						scellInfoTemp_p->cqiInfo.cqiMode, newUEcontext_p->dlUeScellContext_p
						[scellIndex]->isPmiRiConfigured_V920))
			{
                LOG_MAC_ERROR(MAC_RRC_INF,"Invalid Aperiodic CQI Mode [%d] for TX Mode 8",
						scellInfoTemp_p->cqiInfo.cqiMode);
				return MAC_FAILURE;
			}
		}
	}
	/* SPR 12154 Start */    
	else if(TX_MODE_6 == scellInfoTemp_p->transmissionMode)
	{
		newUEcontext_p->dlUeScellContext_p[scellIndex]->Scelltm6DCIFormat = DCI_FORMAT_1A;

		if((((PERIODIC_CQI_REPORTING_MODE == scellInfoTemp_p->cqiInfo.cqiIndication) &&
						(1 == scellInfoTemp_p->cqiInfo.cqiRequestType)) ||
					(APERIODIC_CQI_REPORTING_MODE == scellInfoTemp_p->cqiInfo.cqiIndication) ||
					(PERIODIC_APERIODIC_MODE == scellInfoTemp_p->cqiInfo.cqiIndication)) &&
				(INVALID_CODE_BOOK_INDEX != scellInfoTemp_p->precodingIndex))
		{     
			newUEcontext_p->dlUeScellContext_p[scellIndex]->Scelltm6DCIFormat = DCI_FORMAT_1B;
		}     
	}
	/* SPR 12154 End */    
	/* +  CA_TDD_HARQ_CHANGES */
#ifdef TDD_CONFIG
	dlUeScellContext_p  = newUEcontext_p->dlUeScellContext_p[scellIndex];

	for(sfIndex =0; sfIndex < MAX_SUB_FRAME; sfIndex++)
	{
		for (containerIndex = 0; containerIndex < MAX_DL_TIMER_CONTAINER; containerIndex++)
		{
			dlUeScellContext_p->\
				pdschTxMapContainerForSCell[containerIndex][sfIndex] =  0;
			dlUeScellContext_p->harqProcessSCellId[containerIndex][sfIndex] = 
				INVALID_HARQ_ID;
		}
		dlUeScellContext_p->tddDaiOfSCell[sfIndex] = 0;
		dlUeScellContext_p->tddDaiLastScheduledOfSCell[sfIndex] = 0;
	}
#endif
	/* -  CA_TDD_HARQ_CHANGES */
	return MAC_SUCCESS;
}
#ifdef FDD_CONFIG
MacRetType macFillScellDLCtxtCqiInfo_dummy(UInt32 ueIndex,
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext *newUEcontext_p,
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
		UInt8 cellId)
        /* +- SPR 17777 */
{
	MacRetType resp;
	resp = macFillScellDLCtxtCqiInfo (
			ueIndex,
			ueDlConfigInfo_p,
			newUEcontext_p,
			currentTick,
            /* +- SPR 17777 */
			cellId);
    return resp;
}
#endif

/****************************************************************************
 * Function Name  : macFillScellDLCtxtCqiInfo 
 * Inputs         : ueIndex - index of ueContext
 *                  ueDlConfigInfo_p - Pointer to UeContextForDownlink
 *                  newUEcontext_p - Pointer to DLUEContext
 *                  currentTick - current Tick
 *                  cellId - Scell Index
 *                  internalCellIndex - the cell index used at MAC
 * Outputs        : None 
 * Returns        : MacRetType
 * Description    : The function is used to fill the sCell DL CQI Info in 
 *                  DL UE context.
 ****************************************************************************/
STATIC  MacRetType macFillScellDLCtxtCqiInfo ( 
		UInt32               ueIndex,
		UeContextForDownlink *ueDlConfigInfo_p,
		DLUEContext          *newUEcontext_p,
		/* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
		UInt8                cellId)
        /* +- SPR 17777 */
{
	SCELLDLCQIInfo   *ScelldlCQIInfo_p     = PNULL;
	UInt8 cqiValue = 0;
	UInt8 scellIndex = ueDlConfigInfo_p->carrierAggrConfig.scellInfo[cellId].sCellIndex;
	DLUESCellContext *dlUeScellContext_p = newUEcontext_p->dlUeScellContext_p[scellIndex];
	UInt8 scellInternalCellIndex = ueDlConfigInfo_p->carrierAggrConfig.scellInfo[cellId].internalCellIndex;

	ScelldlCQIInfo_p = &newUEcontext_p->dlUeScellContext_p[scellIndex]->ScelldlCQIInfo;
	ScelldlCQIInfo_p->isSubBandReportAvailable  = FALSE;
	ScelldlCQIInfo_p->isWideBandReportAvailable = FALSE;
	ScelldlCQIInfo_p->modulationSchemeCodeWordOne = 
		ScelldlCQIInfo_p->modulationSchemeCodeWordTwo = 
		ueDlConfigInfo_p->carrierAggrConfig.scellInfo[cellId].modScheme;
	ScelldlCQIInfo_p->currentDLBLERForCW0 = TARGET_DL_BLER_VALUE;
	ScelldlCQIInfo_p->currentDLBLERForCW1 = TARGET_DL_BLER_VALUE;
	/* SPR 13571 fix start */
	ScelldlCQIInfo_p->prevDlBlerCalculatedForCW0 = TARGET_DL_BLER_VALUE;
	ScelldlCQIInfo_p->prevDlBlerCalculatedForCW1 = TARGET_DL_BLER_VALUE;
	ScelldlCQIInfo_p->prevDlBlerCalculatedForCW0_2 = TARGET_DL_BLER_VALUE;
	ScelldlCQIInfo_p->prevDlBlerCalculatedForCW1_2 = TARGET_DL_BLER_VALUE;
	/* SPR 13571 fix end */

	ScelldlCQIInfo_p->previousDLBLERForCW0 = 0;
	ScelldlCQIInfo_p->previousDLBLERForCW1 = 0;
	ScelldlCQIInfo_p->dlBlerCalculatedCounterForCW0 = 0;
	ScelldlCQIInfo_p->dlBlerCalculatedCounterForCW1 = 0;
	/*EICIC*/
	ScelldlCQIInfo_p->currentDLBLERForCW0_2 = TARGET_DL_BLER_VALUE;
	ScelldlCQIInfo_p->currentDLBLERForCW1_2 = TARGET_DL_BLER_VALUE;

	ScelldlCQIInfo_p->previousDLBLERForCW0_2 = 0;
	ScelldlCQIInfo_p->previousDLBLERForCW1_2 = 0;
	ScelldlCQIInfo_p->dlBlerCalculatedCounterForCW0_2 = 0;
	ScelldlCQIInfo_p->dlBlerCalculatedCounterForCW1_2 = 0;
	/*EICIC*/
	ScelldlCQIInfo_p->aperiodicCqiRequestTTI = currentTick;
	ScelldlCQIInfo_p->aperiodicCQIRequested     = FALSE;
	ScelldlCQIInfo_p->subBandReportInfo_p = PNULL;
	ScelldlCQIInfo_p->riUpdationFlag = IMMEDIATE_DL_RI_UPDATE;
	ScelldlCQIInfo_p->wideBandPMIIndex = newUEcontext_p->dlUeScellContext_p
		[scellIndex]->ScelldlMIMOInfo.precodingIndex;
	newUEcontext_p->actualPMIValueForDLSCH = FALSE;
	ScelldlCQIInfo_p->reportReceivedTTI = 0;
	newUEcontext_p->isSubbandPMIAvailable = FALSE;
	newUEcontext_p->dlUeScellContext_p[scellIndex]->aperiodicMode12Rcvd = FALSE;
	ScelldlCQIInfo_p->mcsIndexCodeWordOne = ScelldlCQIInfo_p->mcsIndexCodeWordTwo =
		ueDlConfigInfo_p->carrierAggrConfig.scellInfo[cellId].dlInitMCS;
	ScelldlCQIInfo_p->cqiCorrectionFactorForCW0 = 0;
	ScelldlCQIInfo_p->cqiCorrectionFactorForCW1 = 0;

	ScelldlCQIInfo_p->cqiTypeIndicator = INVALID_CQI_REPORTING_MODE; 
	ScelldlCQIInfo_p->latestAperiodicReportRcvd = TRUE;
	ScelldlCQIInfo_p->cqiRequestType = INVALID_PERIODIC_CQI_REQ_TYPE;
	ScelldlCQIInfo_p->cqiMode = CQI_MODE_INVALID;
	ScelldlCQIInfo_p->widebandCQICodeWordOne =
		ScelldlCQIInfo_p->widebandCQICodeWordTwo =
		ScelldlCQIInfo_p->previousTimeAveragedWBCQICW0 =
		ScelldlCQIInfo_p->previousTimeAveragedWBCQICW1 =
		ScelldlCQIInfo_p->initialCqiValue = 0;
	ScelldlCQIInfo_p->isCQIReportValidTTI = 0xffffffff;

	if (ueDlConfigInfo_p->carrierAggrConfig.scellInfo[cellId].cqiInfo.cqiInfoFlag)
	{
		ScelldlCQIInfo_p->cqiTypeIndicator = ueDlConfigInfo_p->carrierAggrConfig.
			scellInfo[cellId].cqiInfo.cqiIndication;
		ScelldlCQIInfo_p->cqiMode = ueDlConfigInfo_p->carrierAggrConfig.scellInfo
			[cellId].cqiInfo.cqiMode;
		ScelldlCQIInfo_p->cqiRequestType = ueDlConfigInfo_p->carrierAggrConfig.
			scellInfo[cellId].cqiInfo.cqiRequestType;
		cqiValue = getCQIFromMCS(ScelldlCQIInfo_p->mcsIndexCodeWordOne);

		ScelldlCQIInfo_p->widebandCQICodeWordOne = 
			ScelldlCQIInfo_p->widebandCQICodeWordTwo = 
			ScelldlCQIInfo_p->previousTimeAveragedWBCQICW0 =
			ScelldlCQIInfo_p->previousTimeAveragedWBCQICW1 =
			ScelldlCQIInfo_p->initialCqiValue = cqiValue;
		GET_MEM_FROM_POOL(SubBandReportInfo,ScelldlCQIInfo_p->subBandReportInfo_p,
				sizeof(SubBandReportInfo),PNULL);
		if(PNULL == ScelldlCQIInfo_p->subBandReportInfo_p)
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"Memory Allocation failure UE %u line %d\n",ueIndex,__LINE__);
			return MAC_FAILURE;
		}
		memSet(ScelldlCQIInfo_p->subBandReportInfo_p,0,sizeof(SubBandReportInfo));
        /* +- SPR 17777 */
		macUupdateSCellDLSINRCW0(ueIndex,ScelldlCQIInfo_p->widebandCQICodeWordOne,
				cellId);
		macUpdateSCellDLSINRCW1(ueIndex,ScelldlCQIInfo_p->widebandCQICodeWordTwo,
				cellId);
        /* +- SPR 17777 */
		/* SPR 12488 Fix start */
		UInt8 counter =  MAX_SUBBANDS;
		if (dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode == TX_MODE_3 ||
				dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode == TX_MODE_4)
		{
			while(counter--)
			{    
				ScelldlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].cqiIndexCW1 =  
					ScelldlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].cqiIndexCW2 = cqiValue;

				ScelldlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].mcsIndexCW1 =
					ScelldlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].mcsIndexCW2 = 
					ScelldlCQIInfo_p->mcsIndexCodeWordOne;

				ScelldlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].previousTimeAveragedSBCQICW0 =  
					ScelldlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].previousTimeAveragedSBCQICW1 = 
					cqiValue;
			}

			initSpatialRAT0RAT1Bitmaps(cqiValue,
					&ScelldlCQIInfo_p->subBandReportInfo_p->
					cqiInfo.cqiInfoSpatialMul,
					scellInternalCellIndex);

		}
		else if (dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode == TX_MODE_1 ||
				dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode == TX_MODE_2)
		{   

			while(counter--)
			{    
				ScelldlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].cqiIndexCW1 = cqiValue; 

				ScelldlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].mcsIndexCW1 =
					ScelldlCQIInfo_p->mcsIndexCodeWordOne;

				ScelldlCQIInfo_p->subBandReportInfo_p->
					subBandToCQInfo[counter].previousTimeAveragedSBCQICW0 = 
					cqiValue;  
			}

			initNonSpatialRAT0RAT1Bitmaps(cqiValue,
					&ScelldlCQIInfo_p->subBandReportInfo_p->
					cqiInfo.cqiInfoNonSpatialMul,
					scellInternalCellIndex);

		}
		initNumOfSBperBWpart(ScelldlCQIInfo_p,scellInternalCellIndex);
		/* SPR 12488 Fix End */
	}
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : macUpdateCreateULCAParams 
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 *                  upUEInfo_p - Pointer to UeContextForUplink
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None  
 * Returns        : None
 * Description    : The function is used to fill new SCELL UL UE context parameters
 ****************************************************************************/
STATIC  void macUpdateCreateULCAParams(
		ULUEContext        *ulUEContext_p,
		UeContextForUplink *upUEInfo_p
        /* +- SPR 17777 */
		)
{
	UInt8 n1PucchList = 0, n1PucchCs = 0;
	ulUEContext_p->scellDeactivationTimer = 
		upUEInfo_p->carrierAggrConfig.scellDeactivationTimer;
	ulUEContext_p->scellCount = 
		upUEInfo_p->carrierAggrConfig.scellCount;

	ulUEContext_p->pucchFormatR10.requestType = 
		upUEInfo_p->pucchFormatR10.requestType;
	for (n1PucchList = 0; n1PucchList < MAX_PUCCH_AN_CS_LIST; n1PucchList++)
	{
		for (n1PucchCs = 0; n1PucchCs < MAX_PUCCH_AN_CS; n1PucchCs++)
		{
			ulUEContext_p->pucchFormatR10.n1Pucch[n1PucchList][n1PucchCs]=
				upUEInfo_p->pucchFormatR10.n1Pucch[n1PucchList][n1PucchCs];
		}
	}

	if(TRUE == upUEInfo_p->ueCategoryV1020ReqType )
	{
		ulUEContext_p->ueCategoryV1020 = upUEInfo_p->ueCategoryV1020;
	}
	else
	{
		ulUEContext_p->ueCategoryV1020 = MAC_MIN_UE_CATEGORY;
	}
	/*Update the max TB size for the category of the UE */
	/* SPR 19600 Changes start */
	ulUEContext_p->maxTBSize = ulUeCategoryInfo_g[ulUEContext_p->ueCategoryV1020 - 1].maxBytesULSCHTb;
	/* SPR 19600 Changes end */
}

/****************************************************************************
 * Function Name  : macUpdateCreateULScellParams 
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 *                  upUEInfo_p - Pointer to UeContextForUplink
 *                  index - index to store Scell Information
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None  
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : The function is used to fill new SCELL UL UE context parameters
 ****************************************************************************/
STATIC  MacRetType macUpdateCreateULScellParams(
		ULUEContext        *ulUEContext_p,
		UeContextForUplink *upUEInfo_p,
		UInt8 index,
		InternalCellIndex internalCellIndex
		)
{
	ULUESCellContext **ulScellContext_pp=PNULL;
	UInt8 sCellIndex = upUEInfo_p->carrierAggrConfig.scellInfo[index].sCellIndex;
	ulScellContext_pp = &(ulUEContext_p->ulUeScellContext_p[sCellIndex]);
	ScellInfo scellInfo;
	/* + SPR 11204 Changes */
	RNTIInfo    *rntiInfo_p = PNULL;
	/* - SPR 11204 Changes */
	scellInfo = upUEInfo_p->carrierAggrConfig.scellInfo[index];

	if (PNULL == (*ulScellContext_pp))
	{
		GET_MEM_FROM_POOL(ULUESCellContext,(*ulScellContext_pp),
				sizeof (ULUESCellContext),PNULL);
		if (PNULL == *ulScellContext_pp )
		{
			/* Not able to allocate Memory */
        LOG_MAC_ERROR(MAC_RRC_INF,"Memory Allocation failure for ulScellContext_pp UE %u\n",
        ulUEContext_p->ueIndex);
			return MAC_FAILURE;
		}
		memSet((*ulScellContext_pp),0,
				sizeof(ULUESCellContext));
	}
	if (ADD == scellInfo.operationType)
	{
		/* SPR 11043 changes start */
		(*ulScellContext_pp)->internalCellIndex = scellInfo.internalCellIndex;
		/* SPR 11043 changes end */
		macFillScellULUeParams ( ulUEContext_p, 
				upUEInfo_p, index);
		/* PUCCH Config*/
		macFillScellULPucchConfigInfo (ulUEContext_p, upUEInfo_p,index
#ifdef TDD_CONFIG                
                , internalCellIndex
#endif                
                );/*coverity 96762 fix*/
		/** SPR 14204 Fix : Start **/
#ifdef TDD_CONFIG   
		/* + SPR 14324 Changes */
		fillULUeScheduledInfo (ulUEContext_p, scellInfo.internalCellIndex);
		/* - SPR 14324 Changes */
#endif
		/** SPR 14204 Fix : End **/
		macFillScellULUserLocType (ulUEContext_p, upUEInfo_p,index,internalCellIndex);
		/*pdcch changes start*/
		/* + SPR 11204 Changes */
		rntiInfo_p = getUeIdxFromRNTIMap(ulUEContext_p->crnti, ulUEContext_p->internalCellIndex);
		/** SPR 13838 Fix : Start **/
		setUeSearchSpaceInfoforScell(ulUEContext_p,PNULL,rntiInfo_p,ulUEContext_p->internalCellIndex );
		/** SPR 13838 Fix : End **/
		/* - SPR 11204 Changes */
		/*pdcch changes end*/
	}
	else
	{
		freeMemPool(*ulScellContext_pp);
        LOG_MAC_ERROR(MAC_RRC_INF,"Invalid scell operation type %u for UE %u\n",
                scellInfo.operationType,ulUEContext_p->ueIndex);
		return MAC_FAILURE;
	}
	return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : macFillScellULUeParams
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 *                  ueContextInfo_p - Pointer to UE related UL Info
 *                  index - index to store Scell Information
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the UE Category and MCS information in 
 *                  UL UE context.
 *****************************************************************************/
STATIC  void macFillScellULUeParams (
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p,
		UInt8 index
		)

{
	UInt8 scellIndex = ueContextInfo_p->carrierAggrConfig.scellInfo[index].sCellIndex;
#ifdef HD_FDD_CONFIG
	/*Update the category of the UE*/
	ulUEContext_p->ulUeScellContext_p[scellIndex]->isHDFddFlag = ueContextInfo_p->
		carrierAggrConfig.scellInfo[index].isHDFddFlag;
#endif
	ulUEContext_p->ulUeScellContext_p[scellIndex]->modScheme = ueContextInfo_p->
		carrierAggrConfig.scellInfo[index].modScheme;
}

/*****************************************************************************
 * Function Name  : macFillScellULUserLocType
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 *                  ueContextInfo_p - Pointer to UE related UL Info
 *                  index - index to store Scell Information
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the User location type
 *                  information in UL UE context.
 *****************************************************************************/
STATIC  void macFillScellULUserLocType (
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p,
		UInt8 index,
		InternalCellIndex internalCellIndex
		)
{
	/* Initialize user location type to CC_USER if userLocationType
	 * is CE type but cellEdgeRegionCount is zero*/
	UInt8 scellIndex = ueContextInfo_p->carrierAggrConfig.scellInfo[index].sCellIndex;
	if((ueContextInfo_p->carrierAggrConfig.scellInfo[index].userLocationType == 
				CE_USER)&& (ulResourcePartitionConfig_gp[internalCellIndex]->cellEdgeRegionCount == 0))
	{
		lteWarning("No cell edge region exist");
		ulUEContext_p->ulUeScellContext_p[scellIndex]->userLocationType = CC_USER;
	}
	else
	{
		ulUEContext_p->ulUeScellContext_p[scellIndex]->userLocationType = 
			ueContextInfo_p->carrierAggrConfig.scellInfo[index].userLocationType;
	}
}

/****************************************************************************
 * Function Name  : macFillCreateScellCommonParams
 * Inputs         : dwlkUEInfo_p - pointer to UeContextForDownlink
 *                  uplkUEInfo_p - pointer to UeContextForUplink
 *                  newULUEContext_p - pointer to ULUEContext
 *                  newDLUEContext_p - pointer to DLUEContext
 *                  cellId - cell Index
 *                  internalCellIndex - internalCellIndex
 * Outputs        : 
 * Returns        : MacRetType
 * Description    : This function fills common parameters in Scell.
 *
 ****************************************************************************/
 MacRetType macFillCreateScellCommonParams(
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink  *uplkUEInfo_p,
		ULUEContext *newULUEContext_p,
		DLUEContext *newDLUEContext_p,
		UInt8 scellCount,
		InternalCellIndex internalCellIndex
		)
{
	ScellInfo *scellInfo_p;
	scellInfo_p = &(dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount]);
	UInt8 sCellIndex = scellInfo_p->sCellIndex;
	DLUESCellContext **dlScellContext_pp=PNULL;
	ULUESCellContext **ulScellContext_pp=PNULL;
	dlScellContext_pp = &(newDLUEContext_p->dlUeScellContext_p[sCellIndex]);
	ulScellContext_pp = &(newULUEContext_p->ulUeScellContext_p[sCellIndex]);

	if ((TX_MODE_1 ==  scellInfo_p->transmissionMode) || 
			(TX_MODE_2 == scellInfo_p->transmissionMode) ||
			(TX_MODE_6 == scellInfo_p->transmissionMode)||
			(TX_MODE_7 == scellInfo_p->transmissionMode))
	{
		scellInfo_p->rankIndicator = RANK_INDICATOR_1;
	}
	(*dlScellContext_pp)->ScelldlMIMOInfo.dlRi = 
		scellInfo_p->rankIndicator;
	(*ulScellContext_pp)->scellpucchConfigInfo.ueReportedRI 
		= scellInfo_p->rankIndicator;
	(*ulScellContext_pp)->scellpucchConfigInfo.ueReportedPeriodicRI = 
		scellInfo_p->rankIndicator;

	(*ulScellContext_pp)->aperiodicCQIToBeRequested = FALSE;
	if ( (APERIODIC_CQI_REPORTING_MODE == 
				((*dlScellContext_pp)->ScelldlCQIInfo.cqiTypeIndicator)) 
			|| (PERIODIC_APERIODIC_MODE == 
				((*dlScellContext_pp)->ScelldlCQIInfo.cqiTypeIndicator))) 
	{    
		(*ulScellContext_pp)->aperiodicCQIToBeRequested = TRUE;
	}
	(*ulScellContext_pp)->scellpucchConfigInfo.riConfigIndex = 
		scellInfo_p->cqiInfo.riConfigIndex;
	(*ulScellContext_pp)->scellpucchConfigInfo.riConfigIndexV1020 = 
		scellInfo_p->cqiInfo.riConfigIndexV1020;

	if ( MAC_FAILURE == macConfigureScellPucchSRSAndReports(
				&((*ulScellContext_pp)->
					scellpucchConfigInfo),
				dwlkUEInfo_p,uplkUEInfo_p,
				newULUEContext_p, 
				newDLUEContext_p,
				scellCount,internalCellIndex ))
	{
		return MAC_FAILURE;
	}
	return MAC_SUCCESS;
}

/*SPR 16855 +-*/
/****************************************************************************
 * Function Name  : macConfigureScellPucchSRSAndReports  
 * Inputs         : tempPucchConfigInfo_p - pointer to ScellPucchConfig
 *                  dwlkUEInfo_p - pointer to UeContextForDownlink
 *                  uplkUEInfo_p - pointer to UeContextForUplink
 *                  newULUEContext_p - pointer to ULUEContext
 *                  newDLUEContext_p - pointer to DLUEContext
 *                  cellId - cell Index
 *                  internalCellIndex - internalCellIndex
 * Outputs        : 
 * Returns        : MacRetType
 * Description    : This function configure PUCCH SRS in Scell.
 *
 ****************************************************************************/

 MacRetType macConfigureScellPucchSRSAndReports(
		ScellPucchConfig *tempPucchConfigInfo_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink  *uplkUEInfo_p,
		ULUEContext *newULUEContext_p,
		DLUEContext *newDLUEContext_p,
		UInt8 cellId,
		InternalCellIndex internalCellIndex
		)
{
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	UInt16 ueIndex = newULUEContext_p->ueIndex;
	UInt8 scellIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[cellId].sCellIndex;
	UeCellInfo  ueCellInfo = {0};
	ueCellInfo.sCellIndex = scellIndex;

#ifdef FDD_CONFIG //tdd pucch code change
	if (MAC_FAILURE == macValidateUePucchResources(uplkUEInfo_p,internalCellIndex ))
	{
		LOG_MAC_MSG(MAC_VALIDATE_UE_PUCCH_RES_FAILURE_ID,
				LOGERROR,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				ueIndex,
				LINE_NUMBER,
				internalCellIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,
				"PUCCH Resources resquired fro the UE is very high");
		return MAC_FAILURE;
	}
#endif

	/* cqiRequest - 1 (Periodic CQI ) */
	if ( TRUE == uplkUEInfo_p->carrierAggrConfig.scellInfo[cellId].cqiInfo\
			.cqiRequestType)
	{
		if ( MAC_FAILURE == initSCellCQIAndRIinPUCCHMap( 
                    /*SPR 16855 +-*/
					ueIndex,
                    /*SPR 16855 +-*/
                    /* +- SPR 17777 */
					dwlkUEInfo_p,
					uplkUEInfo_p,
					&ueCellInfo,
					internalCellIndex,
					/*SPR 11541 Start*/
					cellId 
					/*SPR 11541 End*/
					))
		{
            LOG_MAC_ERROR(MAC_RRC_INF,"initSCellCQIAndRIinPUCCHMap failed UE %u\n",
                    ueIndex);
			return MAC_FAILURE;
		}
		if(PERIODIC_FEEDBACK_SUBBAND == 
				tempPucchConfigInfo_p->cqiPmiRiInfo.periodicFeedBackType &&
				FREQUENCY_SELECTIVE_ON == cellParams_p->freqSelectiveSchDL)
		{    
			macInitBandwidthScellPart(newULUEContext_p->ulUeScellContext_p[scellIndex]
					,internalCellIndex);
		}
		/* SPR 11633 fix start */
		if ( INVALID_TX_MODE  != dwlkUEInfo_p->carrierAggrConfig.scellInfo[cellId].transmissionMode )
		{
			tempPucchConfigInfo_p->cqiPmiRiInfo.periodicRepMode =
				getPeriodicReportingMode(dwlkUEInfo_p->carrierAggrConfig.scellInfo[cellId].
						transmissionMode,
						tempPucchConfigInfo_p->cqiPmiRiInfo.periodicFeedBackType,
						dwlkUEInfo_p->carrierAggrConfig.scellInfo[cellId].cqiInfo.pmiRiReportV920);

		}
		/* SPR 11633 fix end */
	}

	if( INVALID_APERIODIC_CQI_MODE != uplkUEInfo_p->carrierAggrConfig.
			scellInfo[cellId].cqiInfo.cqiMode )
	{
		tempPucchConfigInfo_p->cqiPmiRiInfoAperiodic.cqiMode = 
			dwlkUEInfo_p->carrierAggrConfig.scellInfo[cellId].cqiInfo.cqiMode;
	}

	if( INVALID_APERIODIC_CQI_MODE != uplkUEInfo_p->
			carrierAggrConfig.scellInfo[cellId].cqiInfo.cqiMode )
	{
		newULUEContext_p->ulUeScellContext_p[scellIndex]->
			scellpucchConfigInfo.cqiPmiRiInfoAperiodic.cqiMode =
			dwlkUEInfo_p->carrierAggrConfig.scellInfo[cellId].cqiInfo.cqiMode;
		newULUEContext_p->ulUeScellContext_p[scellIndex]->
			aperiodicCQIToBeRequested = FALSE;
		if ( (APERIODIC_CQI_REPORTING_MODE == 
					newDLUEContext_p->dlUeScellContext_p[scellIndex]->
					ScelldlCQIInfo.cqiTypeIndicator) ||
				(PERIODIC_APERIODIC_MODE == 
				 newDLUEContext_p->dlUeScellContext_p
				 [scellIndex]->ScelldlCQIInfo.cqiTypeIndicator) ) 
		{    
			newULUEContext_p->ulUeScellContext_p[scellIndex]->
				aperiodicCQIToBeRequested = TRUE;
		}
	}
	return MAC_SUCCESS;
}
/*SPR 16855 +-*/

/****************************************************************************
 * Function Name  :  macInitBandwidthScellPart 
 * Inputs         :  newULUEScellContext_p - Pointer to UL Scell context 
 *                   internalCellIndex - internalCellIndex of cell used at MAC
 * Output         :  Bandwidthpart
 * Returns        :  None
 * Description    :  This function determine the bandwidthpart of the first
 *                   periodic CQI report after create/reconfig.
 ******************************************************************************/
void macInitBandwidthScellPart(ULUESCellContext *newULUEScellContext_p,
		InternalCellIndex internalCellIndex
		)
{
	UInt32 currentSFN = 0;
	UInt32 currentSF = 0;
	UInt32 nextCQISFN = 0;
	UInt32 nextCQISF = 0;
	UInt32 lastWidebandSFN = 0;
	UInt32 lastWidebandSF = 0;
	UInt32 nextWidebandTTI = 0;
	UInt32 lastWidebandTTI = 0;

	UInt8 cqiPeriodicity = newULUEScellContext_p->scellpucchConfigInfo.cqiPeriodicity;    
	UInt32 widebandPeriodicity = newULUEScellContext_p->scellpucchConfigInfo.cqiPmiRiInfo.
		widebandPeriodicity;
	UInt8 cqiPeriodicity_2 = newULUEScellContext_p->scellpucchConfigInfo.cqiPeriodicity_2;    
	UInt32 widebandPeriodicity_2 = newULUEScellContext_p->scellpucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity_2;


	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[newULUEScellContext_p->internalCellIndex]->cellParams_p;

	getSFAndSFN(&currentSFN,&currentSF,internalCellIndex);
#ifdef FDD_CONFIG
	GET_REQUIRED_TICK(currentSFN,currentSF, UE_ACK_NACK_TTI + PHY_DL_DELAY + ONE_ADVANCE) ;
#else
	GET_REQUIRED_TICK(currentSFN,currentSF,getULSfForUpdatingBandwidthPart(currentSF, internalCellIndex)) ;
#endif
	nextCQISFN = currentSFN;
	nextCQISF = currentSF;

	/*  Calculating next occurance of wideband CQI.
	 *  Ref. spec 36.213 sec 7.2.2. 
	 */
	while (0  != ((MAX_SUB_FRAME * nextCQISFN + nextCQISF - 
					newULUEScellContext_p->scellpucchConfigInfo.cqiOffset) % widebandPeriodicity))
	{
		GET_REQUIRED_TICK( nextCQISFN,nextCQISF,1); 
	}

	nextWidebandTTI = (( nextCQISFN * MAX_SUB_FRAME) + nextCQISF);
	/* SPR 12488 Fix start */
	newULUEScellContext_p->nextWidebandOccurrence = nextWidebandTTI;
	/* SPR 12488 Fix end */
	nextCQISFN = currentSFN;
	nextCQISF = currentSF;

	/*  Calculating next occurance of subband CQI. 
	 *  Ref. spec 36.213 sec 7.2.2.
	 */
	while (0  != ((MAX_SUB_FRAME * nextCQISFN + nextCQISF - 
					newULUEScellContext_p->scellpucchConfigInfo.cqiOffset) % cqiPeriodicity ))
	{
		GET_REQUIRED_TICK( nextCQISFN,nextCQISF,1);   
	}
	/* SPR 12488 Fix start */
	newULUEScellContext_p->nextSubbandOccurrence = 
		((nextCQISFN * MAX_SUB_FRAME) + nextCQISF);
	/* SPR 12488 Fix end */

	/*  Calculating last occurance of wideband CQI */
	if ( nextWidebandTTI >= widebandPeriodicity )
	{
		lastWidebandTTI = nextWidebandTTI - widebandPeriodicity;
        /*Coverity 73410 Fix Start*/
		/*lastWidebandSFN = lastWidebandTTI / MAX_SUB_FRAME;
		lastWidebandSF = lastWidebandTTI % MAX_SUB_FRAME;*/
        /*Coverity 73410 Fix End*/
	}
	else
	{
		/* SPR 12488 Fix start */
		if(newULUEScellContext_p->nextSubbandOccurrence  >= widebandPeriodicity)
		{    
			lastWidebandTTI =   newULUEScellContext_p->nextSubbandOccurrence - widebandPeriodicity;
		}    
		else
		{
			lastWidebandTTI  = ( MAX_SFN_VALUE * MAX_SUB_FRAME) - 
				(widebandPeriodicity - newULUEScellContext_p->nextSubbandOccurrence);   
		}
		/* SPR 12488 Fix end */
		lastWidebandSFN = lastWidebandTTI / MAX_SUB_FRAME;
		lastWidebandSF = lastWidebandTTI % MAX_SUB_FRAME; 

		while (0  != ((MAX_SUB_FRAME * lastWidebandSFN + lastWidebandSF - 
						newULUEScellContext_p->scellpucchConfigInfo.cqiOffset) % widebandPeriodicity))
		{
			GET_REQUIRED_TICK( lastWidebandSFN,lastWidebandSF,1); 
		}
		lastWidebandTTI = (MAX_SUB_FRAME * lastWidebandSFN + lastWidebandSF);
	}
	/* SPR 12488 Fix start */
	/* Calculating Bandwidthpart */
	if ( newULUEScellContext_p->nextSubbandOccurrence > lastWidebandTTI )
	{
		newULUEScellContext_p->reportedBandwidthPartNum =
			((newULUEScellContext_p->nextSubbandOccurrence - lastWidebandTTI ) / cqiPeriodicity);

		newULUEScellContext_p->reportedBandwidthPartNum  = 
			((newULUEScellContext_p->reportedBandwidthPartNum - 1) % cellParams_p->bandwidthParts);
	}
	else
	{
		newULUEScellContext_p->reportedBandwidthPartNum = 0;
	}
	/* SPR 12488 Fix end */
	/* EICIC +*/
	if (widebandPeriodicity_2) /* EICIC spr 14996 fix*/
	{
		while (0  != ((MAX_SUB_FRAME * nextCQISFN + nextCQISF - 
						newULUEScellContext_p->scellpucchConfigInfo.cqiOffset_2) % widebandPeriodicity_2))
		{
			GET_REQUIRED_TICK( nextCQISFN,nextCQISF,1); 
		}
	}

	nextWidebandTTI = (( nextCQISFN * MAX_SUB_FRAME) + nextCQISF);
	/* SPR 12488 Fix start */
	newULUEScellContext_p->nextWidebandOccurrence_2 = nextWidebandTTI;
	/* SPR 12488 Fix end */
	nextCQISFN = currentSFN;
	nextCQISF = currentSF;

	/*  Calculating next occurance of subband CQI. 
	 *  Ref. spec 36.213 sec 7.2.2.
	 */
	if (cqiPeriodicity_2)
	{
		while (0  != ((MAX_SUB_FRAME * nextCQISFN + nextCQISF - 
						newULUEScellContext_p->scellpucchConfigInfo.cqiOffset_2) % cqiPeriodicity_2 ))
		{
			GET_REQUIRED_TICK( nextCQISFN,nextCQISF,1);   
		}
	}
	/* SPR 12488 Fix start */
	newULUEScellContext_p->nextSubbandOccurrence_2 = 
		((nextCQISFN * MAX_SUB_FRAME) + nextCQISF);
	/* SPR 12488 Fix end */

	/*  Calculating last occurance of wideband CQI */
	if ( nextWidebandTTI >= widebandPeriodicity_2 )
	{
		/* EICIC_Coverity CID 69340 Start */
		lastWidebandTTI = nextWidebandTTI - widebandPeriodicity_2;
		/* EICIC_Coverity CID 69340 End */

	}
	else
	{
		/* SPR 12488 Fix start */
		if(newULUEScellContext_p->nextSubbandOccurrence_2  >= widebandPeriodicity_2)
		{    
			lastWidebandTTI =   newULUEScellContext_p->nextSubbandOccurrence_2 - widebandPeriodicity_2;
		}    
		else
		{
			lastWidebandTTI  = ( MAX_SFN_VALUE * MAX_SUB_FRAME) - 
				(widebandPeriodicity_2 - newULUEScellContext_p->nextSubbandOccurrence_2);   
		}
		/* SPR 12488 Fix end */
		lastWidebandSFN = lastWidebandTTI / MAX_SUB_FRAME;
		lastWidebandSF = lastWidebandTTI % MAX_SUB_FRAME; 

		if (widebandPeriodicity_2) /* EICIC spr 14996 fix*/
		{
			while (0  != ((MAX_SUB_FRAME * lastWidebandSFN + lastWidebandSF - 
							newULUEScellContext_p->scellpucchConfigInfo.cqiOffset_2) % widebandPeriodicity_2))
			{
				GET_REQUIRED_TICK( lastWidebandSFN,lastWidebandSF,1); 
			}
		}
		lastWidebandTTI = (MAX_SUB_FRAME * lastWidebandSFN + lastWidebandSF);
	}
	/* SPR 12488 Fix start */
	/* Calculating Bandwidthpart */
	if ( newULUEScellContext_p->nextSubbandOccurrence_2 > lastWidebandTTI )
	{
		if (cqiPeriodicity_2) /* EICIC spr 14996 fix*/
		{
			newULUEScellContext_p->reportedBandwidthPartNum_2 =
				((newULUEScellContext_p->nextSubbandOccurrence_2 - lastWidebandTTI ) / cqiPeriodicity_2);
		}

		if (cellParams_p->bandwidthParts) /* EICIC spr 14996 fix*/
		{
			newULUEScellContext_p->reportedBandwidthPartNum_2  = 
				((newULUEScellContext_p->reportedBandwidthPartNum_2 - 1) % cellParams_p->bandwidthParts);
		}
	}
	else
	{
		newULUEScellContext_p->reportedBandwidthPartNum_2 = 0;
	}
	/* SPR 12488 Fix end */

	/* EICIC -*/

	return;
}

/*****************************************************************************
 * Function Name  : macFillScellULPucchConfigInfo 
 * Inputs         : ulUEContext_p - Pointer to UL UE context
 *                  ueContextInfo_p - Pointer to UE related UL Info
 *                  index -  index of Scell in temperory structure
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the PUCCH config Information in 
 *                  UL UE context for Scell.
 *****************************************************************************/
/* +- SPR 17777 */
STATIC  void macFillScellULPucchConfigInfo (
		ULUEContext        *ulUEContext_p, 
		UeContextForUplink *ueContextInfo_p,
		UInt8 index
#ifdef TDD_CONFIG
        ,InternalCellIndex internalCellIndex
#endif
		)
/* +- SPR 17777 */
{
	PucchConfig *pucchConfig_p = PNULL;
	/** SPR 14204 Fix : Start **/
#ifdef TDD_CONFIG    
	UInt32 n    = 0;
	UInt32 iter = 0;
#endif
	/** SPR 14204 Fix : End **/
	UInt8 scellIndex = ueContextInfo_p->carrierAggrConfig.scellInfo[index].sCellIndex;
	ScellInfo *scellInfoTemp_p = &(ueContextInfo_p->carrierAggrConfig.scellInfo[index]);

	pucchConfig_p = &(ulUEContext_p->ulUeScellContext_p[scellIndex]->scellpucchConfigInfo);
	pucchConfig_p->srPeriodicity  = 0;
	pucchConfig_p->cqiPeriodicity = 0;
	pucchConfig_p->riPeriodicity  = 0;
	pucchConfig_p->srPUCCHRBIndex = 0;
	pucchConfig_p->cqiPUCCHRBIndex= 0;

	pucchConfig_p->simultaneousAckNackAndCQI =
		scellInfoTemp_p->cqiInfo.simultaneousACKNACKAndCQI;
	pucchConfig_p->simultaneousAckNackAndCQIV1020 =
		scellInfoTemp_p->cqiInfo.simultaneousACKNACKAndCQIV1020;
#ifdef FDD_CONFIG
	/* PUCCH Maps to be created */
	pucchConfig_p->srOffset       = 0;
	pucchConfig_p->cqiOffset      = 0;
	pucchConfig_p->riOffset       = 0;
#endif
	/** SPR 14204 Fix : Start **/
#ifdef TDD_CONFIG   
	for (n = 0; n < 
			MAX_UE_SCHEDULED_INFO_INSTANCES; n++)
	{
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].flag = UNSET_FLAG;
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].ulCCEIndex = INVALID_CCE_INDEX;
		for (iter = 0; iter < MAX_SUB_FRAME; iter++)
		{
			ulUEContext_p->ueScheduledInfo[internalCellIndex][n].dlCceIndex[iter] = INVALID_CCE_INDEX;
		}
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].transMode = 0;
		/* SPR 7179 Fix Start */
		ulUEContext_p->ueScheduledInfo[internalCellIndex][n].ueInsertedInPdcchDlUeInfo = 0;
		/* SPR 7179 Fix End */
	}
	pucchConfig_p->subbandCQI_k = ueContextInfo_p->subbandCQI_k;
#endif
	/** SPR 14204 Fix : End **/
}

/*****************************************************************************
 * Function Name  : macValidateAndFillDLScellReconfigParams 
 * Inputs         : dwlkUEInfo_p - Pointer to UE related DL Info
 *                  dlUEContext_p - Pointer to DL UE context
 *                  internalCellIndex - internalCellIndex of UE
 *                  dlReconfigUeReq_p -
 * Outputs        : None
 * Returns        : MacRetType
 * Description    : This function fills the CQI params Information in 
 *                  UE context for Scell.
 *****************************************************************************/
STATIC  MacRetType macValidateAndFillDLScellReconfigParams(
		UeContextForDownlink *dwlkUEInfo_p,
		DLUEContext        *dlUEContext_p,
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		)
{
	UInt8 scellCount = 0, scellIndex = 0, index =0;
	ScellInfo            *scellTemp_p; 
	DLUESCellContext     *contextScell_p;
	for ( scellCount= 0; scellCount < dwlkUEInfo_p->carrierAggrConfig.scellCount
			; scellCount++ )
	{
		scellIndex = dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].sCellIndex;
		if(ADD == dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].
				operationType)
		{
			macUpdateCreateDLCAParams(dlUEContext_p,
					dwlkUEInfo_p,internalCellIndex);
			if (MAC_SUCCESS != macUpdateCreateDLScellParams(dlUEContext_p,
						dwlkUEInfo_p,scellCount,internalCellIndex))
			{
				return MAC_FAILURE; 
			}
            /*SPR 16855 +-*/
            dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellCount].
				operationType = ADD;
            /*SPR 16855 +-*/
		}
		else
		{
			if (RECONFIGURE == dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].
					operationType)
			{
                /*SPR 16855 +-*/
				dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellCount].sCellIndex 
					= scellIndex;
				dlReconfigUeReq_p->pucchFormatR10.requestType = 
					dwlkUEInfo_p->pucchFormatR10.requestType;
				/* SPR 11043 changes start */
				dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellCount].internalCellIndex 
					= dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].internalCellIndex;
				/* SPR 11043 changes end */
				/* in case of UL/DL split, store the params in local data base */
#ifdef HD_FDD_CONFIG
				dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellCount].
					rfParams = dwlkUEInfo_p->carrierAggrConfig.scellInfo
					[scellCount].isHDFddFlag;
#endif
				dlReconfigUeReq_p->caConfigDL.scellDeactivation.scellDeactivationTimer = 
					dwlkUEInfo_p->carrierAggrConfig.scellDeactivationTimer;
				if (TRUE == dwlkUEInfo_p->carrierAggrConfig.isBandWidthTagPresent)
				{
					for (index= PCELL_SERV_CELL_IX; index< MAX_SERVCELL; index++)
					{
						dlReconfigUeReq_p->caConfigDL.dlBandWidthDistribution.
							bandWidthDistribution[index] = dwlkUEInfo_p->
							carrierAggrConfig.bandWidthDistribution[index];
					}
				}
                /*SPR 16855 +-*/
				scellTemp_p = &(dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount]);
				contextScell_p = dlUEContext_p->dlUeScellContext_p[scellIndex];
				if (scellTemp_p->isPAPresent == TRUE)
				{
                    /*SPR 16855 +-*/
					dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellCount].
						scellPdschConfigInfo.dlPdschTransPower = scellTemp_p->pA;
					LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] Pdsch Trans Power"\
							"At LP Side = %d \n",
							__func__,dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellCount].
							scellPdschConfigInfo.dlPdschTransPower);
                    /*SPR 16855 +-*/
				}

				if (MAC_SUCCESS != macfillDLScellReconfigUECtxtProvParams (scellTemp_p, 
							contextScell_p,scellCount,internalCellIndex
                            /*SPR 16855 +-*/
							,dlReconfigUeReq_p
                            /*SPR 16855 +-*/
							))
				{
					return MAC_FAILURE;
				}
				if (scellTemp_p->cqiInfo.cqiInfoFlag)
				{
					if (MAC_SUCCESS != macfillDLScellReconfigCqiInfo (scellTemp_p, 
								contextScell_p, scellCount
                                /*SPR 16855 +-*/
								,dlReconfigUeReq_p
                                /*SPR 16855 +-*/
								))
					{
						return MAC_FAILURE;
					}
				}
				macValidateDLScellReconfigRIParams(scellTemp_p, 
						scellCount
                        /*SPR 16855 +-*/
						,dlReconfigUeReq_p
                        /*SPR 16855 +-*/
						);

			}
			else
			{
				if (DELETE == dwlkUEInfo_p->carrierAggrConfig.scellInfo
						[scellCount].operationType)
				{
                    /*SPR 16855 +-*/
					dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellCount].
						operationType = DELETE;
					/* + SPR_13072 */
					dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellCount].
						sCellIndex = scellIndex;
					/* - SPR_13072 */
                    /*SPR 16855 +-*/
				}
			}
		}
	}
	return  MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : macfillDLScellReconfigUECtxtProvParams
 * Inputs         : scellTemp_p - Its point to ScellInfo 
 *                  contextScell_p - Pointer to new DL UE context for Scell
 *                  scellIndex - Scell Index
 *                  internalCellIndex - internalCellIndex of UE
 *                  dlReconfigUeReq_p -
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This function fills the UE context provisioned reconfig
 *                  parameters for scell.
 ****************************************************************************/
STATIC  MacRetType macfillDLScellReconfigUECtxtProvParams (
		ScellInfo            *scellTemp_p, 
		DLUESCellContext     *contextScell_p,
		UInt8                scellIndex,
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		)
{
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
	/* Reconfiguring the provisioned UE Context parameters */
	if (INVALID_TX_MODE !=  scellTemp_p->transmissionMode )
	{
		contextScell_p->ScelldlMIMOInfo.transmissionMode = 
			scellTemp_p->transmissionMode;
        /*SPR 16855 +-*/
		dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellIndex].
			scellTransMode.transMode = scellTemp_p->transmissionMode;
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
				"[%s] Transmission Mode At LP side = %d \n",__func__,
				dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellIndex].
				scellTransMode.transMode);
        /*SPR 16855 +-*/
	}
	dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellIndex].
		isScellActivateStatus = scellTemp_p->isScellActivateStatus;
#ifdef HD_FDD_CONFIG
	contextScell_p->isHDFddFlag = dlReconfigUeReq_p->caConfigDL.
		scellReconfigInfoDL[scellIndex].rfParams;
#endif
	if( ((TX_MODE_3 == contextScell_p->ScelldlMIMOInfo.transmissionMode) || 
				(TX_MODE_4 == contextScell_p->ScelldlMIMOInfo.transmissionMode)||
				(TX_MODE_6 == contextScell_p->ScelldlMIMOInfo.transmissionMode)) &&
			( !((2 == cellParams_p->numOfTxAnteenas ) || 
			    (4 == cellParams_p->numOfTxAnteenas ))) )	
	{
		lteWarning("With TransmissionMode %d, number antennas are neither 2 nor 4\n",
				contextScell_p->ScelldlMIMOInfo.transmissionMode); 
		return MAC_FAILURE;
	}
	if ( INVALID_RANK_INDICATOR != scellTemp_p->rankIndicator)
	{    
		if(scellTemp_p->codebookSubsetRestrictionv1020.cbsrType != CBSR_INVALID)
		{
        /*SPR 16855 +-*/
			dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellIndex].
				codebookSubsetRestrictionv1020 = 
				scellTemp_p->codebookSubsetRestrictionv1020;
			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
					"[%s] Code Book Subset Restriction At LP side = %d \n",__func__,
					dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellIndex].codebookSubsetRestrictionv1020);
        /*SPR 16855 +-*/
		}
	}
	if ( INVALID_CODE_BOOK_INDEX != scellTemp_p->precodingIndex )
	{
        /*SPR 16855 +-*/
		dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellIndex].codeBookIndexInfo.codeBookIndex = 
			scellTemp_p->precodingIndex;
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
				"[%s] Codebook Index At LP side = %d \n",__func__,
				dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellIndex].
				codeBookIndexInfo.codeBookIndex);
        /*SPR 16855 +-*/
	}
	if(TX_MODE_8 == scellTemp_p->transmissionMode)
	{
		contextScell_p->isPmiRiConfigured_V920 = scellTemp_p->cqiInfo.pmiRiReportV920;
	}
	if(TX_MODE_6 == scellTemp_p->transmissionMode)
	{
		contextScell_p->Scelltm6DCIFormat = DCI_FORMAT_1A;

		if((((PERIODIC_CQI_REPORTING_MODE == scellTemp_p->cqiInfo.cqiIndication) &&
						(1 == scellTemp_p->cqiInfo.cqiRequestType)) ||
					(APERIODIC_CQI_REPORTING_MODE == scellTemp_p->cqiInfo.cqiIndication) ||
					(PERIODIC_APERIODIC_MODE == scellTemp_p->cqiInfo.cqiIndication)) &&
				(INVALID_CODE_BOOK_INDEX != scellTemp_p->precodingIndex))
		{
			contextScell_p->Scelltm6DCIFormat = DCI_FORMAT_1B;
		}
		else if((INVALID_CQI_REPORTING_MODE != contextScell_p->ScelldlCQIInfo.
					cqiTypeIndicator)&& (INVALID_CQI_REPORTING_MODE
						== scellTemp_p->cqiInfo.cqiIndication) &&
				(INVALID_CODE_BOOK_INDEX != scellTemp_p->precodingIndex))
		{
			contextScell_p->Scelltm6DCIFormat = DCI_FORMAT_1B;

		}
	}
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : macfillDLReconfigUECategoryV1020Info 
 * Inputs         : dwlkUEInfo_p - Its point to UeContextForDownlink 
 *                  newUEcontext_p - Pointer to new DL UE context
 *                  internalCellIndex -
 *                  dlReconfigUeReq_p -
 * Outputs        : None.
 * Returns        : None. 
 * Description    : This function fills the reconfigure UE category Info for Scell.
 ****************************************************************************/
STATIC  void macfillDLReconfigUECategoryV1020Info (
		UeContextForDownlink *dwlkUEInfo_p, 
		DLUEContext          *newUEcontext_p,
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		)
{
	UInt32 totalSoftchannelBytes = 0;
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[newUEcontext_p->internalCellIndex]->cellParams_p;
    /*SPR 16855 +-*/
	if ( macValidateUeCategoryV1020(dwlkUEInfo_p->ueCategoryV1020,internalCellIndex))
    {
        dlReconfigUeReq_p->ueCategoryInfov1020.ueCategoryV1020 = 
            dwlkUEInfo_p->ueCategoryV1020;
        LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] UE Category At LP side = "\
                "%d \n",__func__,dlReconfigUeReq_p->ueCategoryInfov1020.ueCategoryV1020);
        /* +- SPR 18469 */
        /*Update the max TB size for the category of the UE  */
        dlReconfigUeReq_p->ueCategoryInfov1020.maxTBSize =
            DIVIDE_BY_EIGHT(cellParams_p->dlUeCategoryInfo[dlReconfigUeReq_p->
                    ueCategoryInfov1020.ueCategoryV1020 - 1].maxBitsDLSCHTb);
        dlReconfigUeReq_p->ueCategoryInfov1020.maxSISOTBSize = 
            DIVIDE_BY_EIGHT(cellParams_p->dlUeCategoryInfo[dlReconfigUeReq_p->
                    ueCategoryInfov1020.ueCategoryV1020 - 1].maxBitsDLSCHTb);

        dlReconfigUeReq_p->ueCategoryInfov1020.maxMIMOTBSize = 
            DIVIDE_BY_EIGHT(cellParams_p->dlUeCategoryInfo[dlReconfigUeReq_p->
                    ueCategoryInfov1020.ueCategoryV1020 - 1].maxDLSCHTbBits);
        totalSoftchannelBytes = DIVIDE_BY_EIGHT(\
                cellParams_p->dlUeCategoryInfo[dlReconfigUeReq_p->ueCategoryInfov1020.
                ueCategoryV1020 - 1].totSoftChBits);
        dlReconfigUeReq_p->ueCategoryInfov1020.permissibleTBSize = 
            (totalSoftchannelBytes)/MAC_CODING_FACTOR -(newUEcontext_p->
                    maxPermissibleTBSize - newUEcontext_p->permissibleTBSize);
        dlReconfigUeReq_p->ueCategoryInfov1020.maxPermissibleTBSize = 
            (totalSoftchannelBytes)/MAC_CODING_FACTOR; 
    }
    /* +- SPR 18469 */

	LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s]At LP side \n"\
			"\n MAX TB Size = %d\n MAX SISO TB Size = %d \n "\
			"MAX MIMO TB Size = %d \n",__func__,
			dlReconfigUeReq_p->ueCategoryInfov1020.maxTBSize,
			dlReconfigUeReq_p->ueCategoryInfov1020.maxSISOTBSize,
			dlReconfigUeReq_p->ueCategoryInfov1020.maxMIMOTBSize);
    /*SPR 16855 +-*/
}

/****************************************************************************
 * Function Name  : macfillDLScellReconfigCqiInfo
 * Inputs         : scellTemp_p, 
 *                  contextScell_p,
 *                  scellCount,
 *                  dlReconfigUeReq_p
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This function fills the reconfigure CQI Info parameters.
 ****************************************************************************/
STATIC  MacRetType macfillDLScellReconfigCqiInfo (
		ScellInfo            *scellTemp_p, 
		DLUESCellContext     *contextScell_p,
		UInt8                scellCount
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		)
{
	contextScell_p->ScelldlCQIInfo.cqiRequestType = 
		scellTemp_p->cqiInfo.cqiRequestType;
    /*SPR 16855 +-*/
	dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellCount].cqiInfo.cqiMode = 
		scellTemp_p->cqiInfo.cqiMode;
	dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellCount].cqiInfo.cqiIndication 
		= scellTemp_p->cqiInfo.cqiIndication;
	dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellCount].dlCqiConfigInfo.modScheme 
		= scellTemp_p->modScheme;
	if ( INVALID_APERIODIC_CQI_MODE != scellTemp_p->cqiInfo.cqiMode )
	{
		if(INVALID_TX_MODE !=  scellTemp_p->transmissionMode)
		{
			/* + TM7_8 Changes Start */
			if(TX_MODE_8 == dlReconfigUeReq_p->caConfigDL.
					scellReconfigInfoDL[scellCount].scellTransMode.transMode)
			{
				if ( MAC_FAILURE ==  
						( validateAperiodicModeForTXMode8(
										  scellTemp_p->cqiInfo.cqiMode,
										  scellTemp_p->cqiInfo.pmiRiReportV920)))
				{    
					lteWarning( "TX mode [%d] is not compatible with CQI aperiodic mode [%d]\n",
							dlReconfigUeReq_p->caConfigDL.
							scellReconfigInfoDL[scellCount].scellTransMode.transMode,
							scellTemp_p->cqiInfo.cqiMode);
					return  MAC_FAILURE ;
				}
			}
			/* - TM7_8 Changes End */
			else if ( MAC_FAILURE == (validateAperiodicModeForTXMode(
							scellTemp_p->cqiInfo.cqiMode,
							dlReconfigUeReq_p->caConfigDL.
							scellReconfigInfoDL[scellCount].scellTransMode.transMode)))
			{
				lteWarning( "TX mode [%d] is not compatible with CQI aperiodic mode [%d]\n",
						contextScell_p->ScelldlMIMOInfo.transmissionMode,
						scellTemp_p->cqiInfo.cqiMode);
				return  MAC_FAILURE ;
			}
		}
		else
		{
			if(TX_MODE_8 == contextScell_p->ScelldlMIMOInfo.transmissionMode)
			{
				if ( MAC_FAILURE ==  
						( validateAperiodicModeForTXMode8(scellTemp_p->cqiInfo.cqiMode,
										  scellTemp_p->cqiInfo.pmiRiReportV920)))
				{    
					lteWarning( "TX mode [%d] is not compatible with CQI aperiodic mode [%d]\n",
							contextScell_p->ScelldlMIMOInfo.transmissionMode,
							scellTemp_p->cqiInfo.cqiMode);
					return  MAC_FAILURE ;
				}
			}
			else if ( MAC_FAILURE == (validateAperiodicModeForTXMode(
							scellTemp_p->cqiInfo.cqiMode,
							contextScell_p->ScelldlMIMOInfo.transmissionMode) ))
			{
				lteWarning( "TX mode [%d] is not compatible with CQI aperiodic mode [%d]\n",
						contextScell_p->ScelldlMIMOInfo.transmissionMode,
						scellTemp_p->cqiInfo.cqiMode);

				return  MAC_FAILURE ;
			}
		}
	}
	LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] CQI Mode At LP side = %d \n",__func__,
			dlReconfigUeReq_p->dlCqiConfigInfo.cqiMode);
    /*SPR 16855 +-*/
	return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : macValidateAndFillULScellReconfigParams 
 * Inputs         : uplkUEInfo_p - Pointer to UE related UL Info
 *                  ulUEContext_p - Pointer to UL UE context
 *                  internalCellIndex -  Cell-Index used at MAC
 *                  ulReconfigUeReq_p - Pointer to Reconfig UL UE request 
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the CQI params Information in 
 *                  UE context for Scell.
 *****************************************************************************/
STATIC  MacRetType macValidateAndFillULScellReconfigParams(
		UeContextForUplink       *uplkUEInfo_p,
		ULUEContext          *ulUEContext_p,
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,ULReconfigUeReq     *ulReconfigUeReq_p
        /*SPR 16855 +-*/
		)
{
	UInt8 count = 0, scellIndex = 0;
	ScellInfo            *scellTemp_p;
	/** SPR 12320 Fix : Start **/
	PucchConfig * pucchConfig_p = PNULL;
	UeCellInfo  ueCellInfo = {0};
	/** SPR 12320 Fix : End **/

	for (count = 0; count < uplkUEInfo_p->carrierAggrConfig.scellCount
			; count++ )
	{
		scellIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[count].sCellIndex;
		if(ADD == uplkUEInfo_p->carrierAggrConfig.scellInfo[count].
				operationType)
		{
            /* +- SPR 17777 */
			macUpdateCreateULCAParams (ulUEContext_p,
					uplkUEInfo_p);
            /* +- SPR 17777 */
			if (MAC_SUCCESS != macUpdateCreateULScellParams (ulUEContext_p,
						uplkUEInfo_p, count, internalCellIndex))
			{
				return MAC_FAILURE; 
			}
            /*SPR 16855 +-*/
			ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[count].
				operationType = ADD;
            /*SPR 16855 +-*/
		}
		else
		{
			if (RECONFIGURE == uplkUEInfo_p->carrierAggrConfig.scellInfo[count].
					operationType)
			{
                /*SPR 16855 +-*/
				ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[count].sCellIndex 
					= scellIndex;
				ulReconfigUeReq_p->pucchFormatR10.requestType = 
					uplkUEInfo_p->pucchFormatR10.requestType;
				/* SPR 11043 changes start */
				ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[count].internalCellIndex 
					= uplkUEInfo_p->carrierAggrConfig.scellInfo[count].internalCellIndex;
				/* SPR 11043 changes end */

				/* in case of UL/DL split, store the params in local data base */
#ifdef HD_FDD_CONFIG
				ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[count].
					rfParams = uplkUEInfo_p->carrierAggrConfig.scellInfo[count].
					isHDFddFlag;
#endif
				ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[count].
					isScellActivateStatus = 
					uplkUEInfo_p->carrierAggrConfig.scellInfo[count].
					isScellActivateStatus;
				ulReconfigUeReq_p->caConfigUL.scellDeactivation.
					scellDeactivationTimer = 
					uplkUEInfo_p->carrierAggrConfig.scellDeactivationTimer;
                /*SPR 16855 +-*/
				scellTemp_p = &(uplkUEInfo_p->carrierAggrConfig.scellInfo[count]);

				macValidateULScellReconfigParams (scellTemp_p, 
						count
                        /*SPR 16855 +-*/
						,ulReconfigUeReq_p
                        /*SPR 16855 +-*/
						);

			}
			else
			{
				if (DELETE == uplkUEInfo_p->carrierAggrConfig.scellInfo
						[count].operationType)
				{
					/*spr 12800 fix start*/
					if (ulUEContext_p->ulUeScellContext_p[scellIndex] == PNULL)
					{
						return  MAC_SUCCESS;
					}
					/*spr 12800 fix end*/
                    /*SPR 16855 +-*/
					ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[count].
						operationType = DELETE;
					/** SPR 12320 Fix : START **/
					ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[count].
						sCellIndex = scellIndex;
                    /*SPR 16855 +-*/
					scellTemp_p = &(uplkUEInfo_p->carrierAggrConfig.scellInfo[count]);
					pucchConfig_p = &(ulUEContext_p->ulUeScellContext_p[scellIndex]->scellpucchConfigInfo); 

					ueCellInfo.isPCell = FALSE;
					ueCellInfo.sCellIndex = scellIndex;
					ueCellInfo.sCellInternalCellIndex = scellTemp_p->internalCellIndex;

					deleteSCellPucchNodesForUE(pucchConfig_p,WIDEBAND_CQI_REPORT,&ueCellInfo,
							internalCellIndex,ulUEContext_p->ueIndex);
					deleteSCellPucchNodesForUE(pucchConfig_p,CQI_REPORT,&ueCellInfo,
							internalCellIndex,ulUEContext_p->ueIndex);
					deleteSCellPucchNodesForUE(pucchConfig_p,RI_REPORT,&ueCellInfo,
							internalCellIndex,ulUEContext_p->ueIndex);
					/** SPR 12320 Fix : END **/
				}
			}
		}
	}
	return  MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : macValidateULScellReconfigParams 
 * Inputs         : scellTemp_p - Pointer to Scell DL Info
 *                  scellIndex - Scell Index
 *                  ulReconfigUeReq_p -
 * Outputs        : None
 * Returns        : None
 * Description    : This function validates and fills the CQI params Information in 
 *                  UE context for Scell.
 *****************************************************************************/
STATIC  void  macValidateULScellReconfigParams (
		ScellInfo            *scellTemp_p, 
		UInt8                scellIndex
        /*SPR 16855 +-*/
		,ULReconfigUeReq     *ulReconfigUeReq_p
        /*SPR 16855 +-*/
		)
{
	if ( INVALID_SIMULTANEOUS_ACK_NACK_VALUE != 
			scellTemp_p->cqiInfo.simultaneousACKNACKAndCQI )
	{ 
        /*SPR 16855 +-*/
		ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellIndex].
			rrcInfpucchConfigReqInfoScell.ulCqiReqInfo.cqiPeriodicReqInfo.
			simultaneousAckNackAndCQI=
			scellTemp_p->cqiInfo.simultaneousACKNACKAndCQI;
	}
	if ( INVALID_SIMULTANEOUS_ACK_NACK_VALUE != 
			scellTemp_p->cqiInfo.simultaneousACKNACKAndCQIV1020 )
	{ 
		ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellIndex].
			rrcInfpucchConfigReqInfoScell.ulCqiReqInfo.cqiPeriodicReqInfo.
			simultaneousACKNACKAndCQIV1020=
			scellTemp_p->cqiInfo.simultaneousACKNACKAndCQIV1020;
	}
	ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellIndex].
		rrcInfpucchConfigReqInfoScell.ulCqiReqInfo.cqiPeriodicReqInfo.
		riConfigIndexV1020=scellTemp_p->cqiInfo.riConfigIndexV1020;
    /*SPR 16855 +-*/
}

/*****************************************************************************
 * Function Name  : macValidateULReconfigUECategoryV1020Params 
 * Inputs         : uplkUEInfo_p - Pointer to UE related UL Context
 *                  ulUEContext_p - Pointer to UL Context
 *                  internalCellIndex - cell index used at MAC
 *                  ulReconfigUeReq_p - pointer to Uplink reconfig UE structure
 * Outputs        : None
 * Returns        : MacRetType
 * Description    : This function fills the CQI params Information in 
 *                  UE context for Scell.
 *****************************************************************************/
STATIC  MacRetType macValidateULReconfigUECategoryV1020Params (
		UeContextForUplink *uplkUEInfo_p,
        /* +- SPR 17777 */
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,ULReconfigUeReq     *ulReconfigUeReq_p
        /*SPR 16855 +-*/
		)
{
	if(TRUE == uplkUEInfo_p->ueCategoryV1020ReqType)
	{
		if ( macValidateUeCategoryV1020(uplkUEInfo_p->ueCategoryV1020,internalCellIndex))
		{
            /*SPR 16855 +-*/
			ulReconfigUeReq_p->ueCategoryInfov1020.ueCategoryV1020=
				uplkUEInfo_p->ueCategoryV1020;
			/* SPR 19600 Changes start */
			ulReconfigUeReq_p->ueCategoryInfov1020.maxTBSize=
				ulUeCategoryInfo_g[ulReconfigUeReq_p->ueCategoryInfov1020.ueCategoryV1020 - 1].maxBytesULSCHTb;
			/* SPR 19600 Changes end */
			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] In LP"\
					"(UL_RECONFIG_UE_CATEGORY_V1020), UE Categoryv1020 = %d,"\
					"Max TB size = %d \n",__func__,ulReconfigUeReq_p->ueCategoryInfov1020.
					ueCategoryV1020, ulReconfigUeReq_p->ueCategoryInfov1020.maxTBSize);
            /*SPR 16855 +-*/
		}
		else
		{
			return MAC_FAILURE;
		}
	}
	return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : macValidateDLScellReconfigRIParams 
 * Inputs         : scellTemp_p - Pointer to Scell DL Info
 *                  scellIndex - Scell Index
 *                  dlReconfigUeReq_p -
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the CQI params Information in 
 *                  UE context for Scell.
 *****************************************************************************/
STATIC  void macValidateDLScellReconfigRIParams (
		ScellInfo            *scellTemp_p, 
		UInt8                scellIndex
        /*SPR 16855 +-*/
		,DLReconfigUeReq     *dlReconfigUeReq_p
        /*SPR 16855 +-*/
		)
{
	if ((TX_MODE_1 ==  scellTemp_p->transmissionMode) || 
			(TX_MODE_2 ==  scellTemp_p->transmissionMode) ||
			(TX_MODE_6 == scellTemp_p->transmissionMode)||
			(TX_MODE_7 == scellTemp_p->transmissionMode))
	{
		scellTemp_p->rankIndicator = RANK_INDICATOR_1;
	}
    /*SPR 16855 +-*/
	dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellIndex].numOfLayersInfoDL.
		numOfLayers  = scellTemp_p->rankIndicator;  //DL
	dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellIndex].numOfLayerMIMO = 
		scellTemp_p->numOfLayer;  //DL
    /*SPR 16855 +-*/
}

/*****************************************************************************
 * Function Name  : macFillReconfigureScellCommonParams 
 * Inputs         : newULUEContext_p -
 *                  dwlkUEInfo_p - Pointer to UE related DL Info
 *                  uplkUEInfo_p - Pointer to UE related UL Info
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 *                  ulReconfigUeReq_p - Pointer to UL UE reconfig req
 *                  dlReconfigUeReq_p - Pointer to DL UE reconfig req
 *                  reconfigInfo_p - Pointer to reconfig info structure.
 * Outputs        : None
 * Returns        : MacRetType
 * Description    : This function fills the CQI params Information in 
 *                  UE context for Scell.
 *****************************************************************************/
STATIC  MacRetType macFillReconfigureScellCommonParams (
		ULUEContext *newULUEContext_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink   *uplkUEInfo_p,
		InternalCellIndex internalCellIndex
        /*SPR 16855 +-*/
		,ULReconfigUeReq *ulReconfigUeReq_p
		,DLReconfigUeReq *dlReconfigUeReq_p
		,UeContextForReconfig *reconfigInfo_p
        /*SPR 16855 +-*/
		)
{
	PucchConfig                  *tempScellPucchConfigInfo_p        = PNULL;     
	UInt8 scellCount = 0, scellIndex = 0;
	/* SPR 11541 Fix : Start */
	UInt8 resp = MAC_FAILURE;
	/* SPR 11541 Fix : End */
	if ( macValidateUeCategoryV1020(uplkUEInfo_p->ueCategoryV1020, internalCellIndex) )
	{
        /*SPR 16855 +-*/
		ulReconfigUeReq_p->ueCategoryInfov1020.ueCategoryV1020= 
			uplkUEInfo_p->ueCategoryV1020;
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] In LP (UL_RECONFIG_UE_CATEGORY), UE Category = %d,Max TB size = %d \n",__func__,
				ulReconfigUeReq_p->ueCategoryInfov1020.ueCategory, ulReconfigUeReq_p->ueCategoryInfov1020.maxTBSize);
        /*SPR 16855 +-*/
	}
	/* SPR 11541 Fix : Start */
	/* + CA_TDD_HARQ_CHANGES */    
	DLUEContext *newDLUEContext_p = dlUECtxInfoArr_g[newULUEContext_p->ueIndex].dlUEContext_p;
	/* - CA_TDD_HARQ_CHANGES */    
	/* SPR 11541 Fix : End */
	ScellInfo *scellInfo_p = PNULL;
	ULUESCellContext **ulUeScellContext_pp = PNULL;
	ULCQIPeriodicReqInfo *ulCqiScellPeriodicReqInfo_p = PNULL;     
	for (scellCount = 0; scellCount < dwlkUEInfo_p->carrierAggrConfig.scellCount
			; scellCount++ )
	{
		scellInfo_p = &(dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount]);
		/* SPR 11541 Fix : Start */
		ULSCELLReconfigInfo *scellReconfigInfoUL_p =
			&(ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellCount]);
		RRCInfPUCCHConfigReqInfo *rrcInfScellPUCCHConfigReqInfo_p = 
			&(scellReconfigInfoUL_p->rrcInfpucchConfigReqInfoScell);
		ULCQIReqInfo *ulCqiReqInfo_p = &(rrcInfScellPUCCHConfigReqInfo_p->ulCqiReqInfo);

		/* 11541 complete fix start */
		rrcInfScellPUCCHConfigReqInfo_p->activeIndexForPUCCHMap =
			longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
		/* 11541 complete fix end */

		ulCqiScellPeriodicReqInfo_p = 
			&(ulCqiReqInfo_p->cqiPeriodicReqInfo);
		/* SPR 11541 Fix : End */
		/* + CA_TDD_HARQ_CHANGES */    
		/* - CA_TDD_HARQ_CHANGES */    
		if (ADD == scellInfo_p->operationType)
		{
			scellReconfigInfoUL_p->sCellIndex = scellInfo_p->sCellIndex;
			scellReconfigInfoUL_p->internalCellIndex = scellInfo_p->internalCellIndex;
			resp = macFillCreateScellCommonParams(dwlkUEInfo_p,uplkUEInfo_p,newULUEContext_p, newDLUEContext_p \
					,scellCount, internalCellIndex);            
			rrcInfScellPUCCHConfigReqInfo_p->switchPassiveToActiveFlag = 1;
		}
		/* + CA_TDD_HARQ_CHANGES */    
		/* - CA_TDD_HARQ_CHANGES */    
		if (RECONFIGURE == scellInfo_p->operationType)
		{
			scellIndex = scellInfo_p->sCellIndex;
			ulUeScellContext_pp = &(newULUEContext_p->ulUeScellContext_p[scellIndex]);


			if ((TX_MODE_1 ==  scellInfo_p->transmissionMode) || 
					(TX_MODE_2 ==  scellInfo_p->transmissionMode) ||
					(TX_MODE_6 == scellInfo_p->transmissionMode)||
					(TX_MODE_7 == scellInfo_p->transmissionMode))
			{
				scellInfo_p->rankIndicator = RANK_INDICATOR_1;
			}
            /*SPR 16855 +-*/
			dlReconfigUeReq_p->caConfigDL.scellReconfigInfoDL[scellCount].
				numOfLayersInfoDL.numOfLayers  = scellInfo_p->rankIndicator;
			(*ulUeScellContext_pp)->scellpucchConfigInfo.ueReportedRI = scellInfo_p->rankIndicator;
			(*ulUeScellContext_pp)->scellpucchConfigInfo.ueReportedPeriodicRI = scellInfo_p->rankIndicator;
			tempScellPucchConfigInfo_p = &((*ulUeScellContext_pp)->scellpucchConfigInfo);

			/* SPR 11541 Fix : Start */
			resp  = macReconfigureScellPUCCHSRSAndReports(
					newULUEContext_p,
					scellCount,
					tempScellPucchConfigInfo_p,
					dwlkUEInfo_p,
					uplkUEInfo_p,
					ulReconfigUeReq_p,
					reconfigInfo_p,
					internalCellIndex);

			if (MAC_SUCCESS != resp)
			{
				return MAC_FAILURE;
			}
            /*SPR 16855 +-*/
		}

        /*SPR 16855 +-*/
		if (MAC_SUCCESS == resp)
		{
			if((rrcInfScellPUCCHConfigReqInfo_p->pucchConfigMsg & UL_RECONFIG_CQI_IND)&&
					(UL_RECONFIG_NO_CQI_MSG == ulCqiReqInfo_p->cqiConfigMsg))
			{
				rrcInfScellPUCCHConfigReqInfo_p->pucchConfigMsg &=  UL_RECONFIG_CLEAR_CQI_IND;
			}

			if(UL_RECONFIG_NO_PUCCH_MSG != rrcInfScellPUCCHConfigReqInfo_p->pucchConfigMsg) 
			{
				scellReconfigInfoUL_p->ueULCAScellInfoReconfigTlvRecieved
					[scellReconfigInfoUL_p->ueULCAScellReconfigMsgTlvCount]=
					UL_RECONFIG_SCELL_PUCCH_CONFIG_INFO;
				scellReconfigInfoUL_p->ueULCAScellReconfigMsgTlvCount++;
			}
		}
		if((rrcInfScellPUCCHConfigReqInfo_p->pucchConfigMsg > UL_RECONFIG_NO_PUCCH_MSG ) &&
				((UL_RECONFIG_CQI_APERIODIC_MSG != 
				  ulCqiReqInfo_p->cqiConfigMsg) ||
				 (UL_RECONFIG_CQI_RI_NO_RECONFIG_MSG != 
				  ulCqiScellPeriodicReqInfo_p->cqiRiReconfigMsg)))    
		{
			/* This variable will be set only when any of request comes with or without aperiodic CQI.
			 * If only aperiodic CQI comes it will not be set  */ 
			rrcInfScellPUCCHConfigReqInfo_p->switchPassiveToActiveFlag = 1;
		}
		/* SPR 11541 Fix : End */
        /*SPR 16855 +-*/
	}
	return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : macReconfigureScellPUCCHSRSAndReports
 * Inputs         : ulUeCtx_p - 
 *                  scellCount -
 *                  tempPucchConfigInfo_p - Pointer to PucchConfig structure
 *                  dwlkUEInfo_p - Pointer to DL UE context
 *                  uplkUEInfo_p - Pointer to UL UE context
 *                  ulReconfigUeReq_p - Pointer to UL Reconfig Request
 *                  reconfigInfo_p - Pointer to reconfig info structure.
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks and reconfigures CQI Information
 *****************************************************************************/
STATIC  MacRetType macReconfigureScellPUCCHSRSAndReports( 
		ULUEContext *ulUeCtx_p,
		UInt8 scellCount,
		PucchConfig          *tempPucchConfigInfo_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink   *uplkUEInfo_p,
		ULReconfigUeReq      *ulReconfigUeReq_p,
		UeContextForReconfig *reconfigInfo_p,
		InternalCellIndex internalCellIndex)
{
#ifdef FDD_CONFIG
	if (MAC_FAILURE == macValidateUePucchResources(uplkUEInfo_p, internalCellIndex))
	{
		LOG_MAC_MSG(MAC_VALIDATE_UE_PUCCH_RES_FAILURE_ID,
				LOGERROR,
				MAC_RRC_INF,
				GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				ulUeCtx_p->ueIndex,
				LINE_NUMBER,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,
				"PUCCH Resources required at Scell for the UE is very high");
		return MAC_FAILURE;
	}
#endif
	/***************************CQI Parameter Reconfiguration **************/
	if (MAC_SUCCESS !=macCheckAndReconfigureScellCqiInfo (
				ulUeCtx_p->ueIndex,
				scellCount,
				tempPucchConfigInfo_p, 
				dwlkUEInfo_p, 
				uplkUEInfo_p, 
				ulReconfigUeReq_p,
				reconfigInfo_p,
				internalCellIndex))
	{
		return MAC_FAILURE;
	}
	return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : macCheckAndReconfigureScellCqiInfo
 * Inputs         : ueIndex - Stores UE index
 *                  scellCount -
 *                  tempPucchConfigInfo_p - Pointer to PucchConfig structure
 *                  dwlkUEInfo_p - Pointer to DL UE context
 *                  uplkUEInfo_p - Pointer to UL UE context
 *                  ulReconfigUeReq_p - Pointer to UL Reconfig Request
 *                  reconfigInfo_p - Pointer to reconfig info structure.
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * PUTPUT         : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks and reconfigures CQI Information
 *****************************************************************************/
STATIC  MacRetType macCheckAndReconfigureScellCqiInfo( 
		UInt16               ueIndex,
		UInt8 scellCount,
		PucchConfig          *tempPucchConfigInfo_p,
		UeContextForDownlink *dwlkUEInfo_p,
		UeContextForUplink   *uplkUEInfo_p,
		ULReconfigUeReq      *ulReconfigUeReq_p,
		UeContextForReconfig *reconfigInfo_p,
		InternalCellIndex internalCellIndex)
{
	ULCQIReqInfo                *ulCqiReqInfo_p             = PNULL; 
	ULCQIPeriodicReqInfo        *cqiPeriodicReqInfo_p       = PNULL; 
	ULReconfigCQIRIMsgType       cqiRiReconfigRetValue      = 
		UL_RECONFIG_DEFAULT_RECONFIG_MSG;
	ULReconfigPeriodicCQIType    cqiPeriodicMsg             = 
		UL_RECONFIG_NO_CQI_SUBBAND_RI_MSG;
	/* + CA_TDD_HARQ_CHANGES */    
	MacRetType                   reconfigCQIRIRetValue      = MAC_SUCCESS;
	UeCellInfo  ueCellInfo = {0};
	ueCellInfo.sCellIndex =uplkUEInfo_p->carrierAggrConfig.\
			       scellInfo[scellCount].sCellIndex; 
	/* - CA_TDD_HARQ_CHANGES */    
	if (ulReconfigUeReq_p)
	{
		ulCqiReqInfo_p = &ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL
			[scellCount].rrcInfpucchConfigReqInfoScell.ulCqiReqInfo;
		cqiPeriodicReqInfo_p = &ulCqiReqInfo_p->cqiPeriodicReqInfo; 
	}
	if (uplkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.cqiRequestType== 1)
	{
		/* + CA_TDD_HARQ_CHANGES */    
		reconfigCQIRIRetValue = reConfigSCellRIAndCQIMap (
				ulUECtxInfoArr_g[ueIndex].ulUEContext_p,
				dwlkUEInfo_p,
				uplkUEInfo_p,
                /*SPR 16855 +-*/
				&cqiRiReconfigRetValue,
				reconfigInfo_p,
				&ueCellInfo,
                /*SPR 16855 +-*/
				scellCount );

		if (MAC_FAILURE == reconfigCQIRIRetValue )
		{
			return MAC_FAILURE;
		}    
		/* - CA_TDD_HARQ_CHANGES */    
		if( ulReconfigUeReq_p )
		{
			cqiPeriodicReqInfo_p->cqiRiReconfigMsg = cqiRiReconfigRetValue;
			cqiPeriodicReqInfo_p->cqiRiReconfigMsg |= CQI_DEFAULT_PARAMETERS_IND;
			cqiPeriodicReqInfo_p->periodicFeedBackType = 
				tempPucchConfigInfo_p->cqiPmiRiInfo.periodicFeedBackType;
			cqiPeriodicReqInfo_p->periodicRepMode =  
				tempPucchConfigInfo_p->cqiPmiRiInfo.periodicRepMode;
			cqiPeriodicReqInfo_p->widebandPeriodicity = 
				tempPucchConfigInfo_p->cqiPmiRiInfo.widebandPeriodicity;
			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] AT LP cqiRiReconfigMsg = %d \n",
					__func__,cqiPeriodicReqInfo_p->cqiRiReconfigMsg);
			if (CQI_MAP_RECONFIGURE_IND & cqiPeriodicReqInfo_p->cqiRiReconfigMsg)
			{   
				cqiPeriodicReqInfo_p->cqiPeriodicity = 
					tempPucchConfigInfo_p->cqiPeriodicity;
				/* + CA_TDD_HARQ_CHANGES */    
				cqiPeriodicReqInfo_p->cqiOffset =      
					tempPucchConfigInfo_p->cqiOffset;
				/* - CA_TDD_HARQ_CHANGES */    
				cqiPeriodicReqInfo_p->cqiPmiConfigIndex =
					tempPucchConfigInfo_p->cqiPmiConfigIndex;
				cqiPeriodicReqInfo_p->cqiPMIConfigIndexV1020 =
					tempPucchConfigInfo_p->cqiPmiConfigIndexV1020;
				cqiPeriodicReqInfo_p->cqiPUCCHRBIndex = 
					uplkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.
					cqiPucchResourceIndex;
				cqiPeriodicReqInfo_p->cqiPucchResourceIndexV1020 = 
					uplkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.
					cqiPucchResourceIndexV1020;
				ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellCount].
					rrcInfpucchConfigReqInfoScell.activeIndexForPUCCHMap 
					= longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
			}
			if(RI_MAP_RECONFIGURE_IND & cqiPeriodicReqInfo_p->cqiRiReconfigMsg)
			{
				cqiPeriodicReqInfo_p->riConfigIndexInfo.riPeriodicity=
					tempPucchConfigInfo_p->riPeriodicity;
				cqiPeriodicReqInfo_p->riConfigIndexInfo.riOffset=
					tempPucchConfigInfo_p->riOffset;
				cqiPeriodicReqInfo_p->riConfigIndexInfo.maxRIWrapAround =
					tempPucchConfigInfo_p->maxRIWrapAround[
					longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex];
				cqiPeriodicReqInfo_p->riConfigIndexInfo.riConfigIndex =
					tempPucchConfigInfo_p->riConfigIndex;
				ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellCount].
					rrcInfpucchConfigReqInfoScell.activeIndexForPUCCHMap 
					= longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
			}
			if (CQI_MAP_RECONFIGURE_IND2 & cqiPeriodicReqInfo_p->cqiRiReconfigMsg)
			{   
				cqiPeriodicReqInfo_p->cqiPeriodicity_2 = 
					tempPucchConfigInfo_p->cqiPeriodicity_2;
#ifdef FDD_CONFIG
				cqiPeriodicReqInfo_p->cqiOffset_2 =      
					tempPucchConfigInfo_p->cqiOffset_2;
#endif
				cqiPeriodicReqInfo_p->cqiPmiConfigIndex_2 =
					tempPucchConfigInfo_p->cqiPmiConfigIndex_2;
				cqiPeriodicReqInfo_p->cqiPMIConfigIndexV1020 =
					tempPucchConfigInfo_p->cqiPmiConfigIndexV1020;
				cqiPeriodicReqInfo_p->cqiPUCCHRBIndex = 
					uplkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.
					cqiPucchResourceIndex;
				cqiPeriodicReqInfo_p->cqiPucchResourceIndexV1020 = 
					uplkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.
					cqiPucchResourceIndexV1020;
				ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellCount].
					rrcInfpucchConfigReqInfoScell.activeIndexForPUCCHMap 
					= longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
			}
			if(RI_MAP_RECONFIGURE_IND2 & cqiPeriodicReqInfo_p->cqiRiReconfigMsg)
			{
				cqiPeriodicReqInfo_p->riConfigIndexInfo.riPeriodicity_2=
					tempPucchConfigInfo_p->riPeriodicity_2;
				cqiPeriodicReqInfo_p->riConfigIndexInfo.riOffset_2=
					tempPucchConfigInfo_p->riOffset_2;
				cqiPeriodicReqInfo_p->riConfigIndexInfo.maxRIWrapAround2 =
					tempPucchConfigInfo_p->maxRIWrapAround2[
					longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex];
				cqiPeriodicReqInfo_p->riConfigIndexInfo.riConfigIndex_2 =
					tempPucchConfigInfo_p->riConfigIndex_2;
				ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellCount].
					rrcInfpucchConfigReqInfoScell.activeIndexForPUCCHMap 
					= longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
			}
			if((CQI_RESOURCE_INDEX_RECONFIGURE_IND & 
						cqiPeriodicReqInfo_p->cqiRiReconfigMsg) || (RI_RESOURCE_INDEX_RECONFIGURE_IND
							& cqiPeriodicReqInfo_p->cqiRiReconfigMsg) || (CQI_RESOURCE_INDEX_RECONFIGURE_IND2 & 
								cqiPeriodicReqInfo_p->cqiRiReconfigMsg) || (RI_RESOURCE_INDEX_RECONFIGURE_IND2
									& cqiPeriodicReqInfo_p->cqiRiReconfigMsg))
			{
				ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellCount].
					rrcInfpucchConfigReqInfoScell.activeIndexForPUCCHMap 
					= longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
				cqiPeriodicReqInfo_p->cqiPUCCHRBIndex = uplkUEInfo_p->\
									carrierAggrConfig.scellInfo[scellCount].cqiInfo.cqiPucchResourceIndex;
				cqiPeriodicReqInfo_p->cqiPucchResourceIndexV1020 = uplkUEInfo_p->
					carrierAggrConfig.scellInfo[scellCount].\
					cqiInfo.cqiPucchResourceIndexV1020;
			}    
			if(UL_RECONFIG_CQI_SUBBAND_RI_MSG == ulCqiReqInfo_p->
					cqiPeriodicReqInfo.cqiPeriodicMsg || UL_RECONFIG_CQI_SUBBAND_K_MSG
					== ulCqiReqInfo_p->cqiPeriodicReqInfo.cqiPeriodicMsg)
			{
				cqiPeriodicReqInfo_p->cqiFormatPeriodicSubbandK.subbandCQI_k =
					tempPucchConfigInfo_p->cqiPmiRiInfo.k;
			}       

			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] At LP Side (UL_RECONFIG_PER_CQI_CONFIG_INFO(setup)), \n"\
					"cqiConfigMsg = %d CQIAperiodic Mode = %d cqiPeriodicMsg = %d cqiRiReconfigMsg = %d\n"\
					"cqiPeriodicity = %d, cqiPUCCHRBIndex = %d simultaneousAckNackAndCQI = %d cqiOffset = %d ,\n"\
					"periodicFeedBackType = %d k = %d periodicRepMode = %d cqiPmiConfigIndex = %d ,\n"\
					"activeIndexForPUCCHMap = %d pucchConfigMsg = %d Passive index = %d\n ",__func__,
					ulCqiReqInfo_p-> cqiConfigMsg,
					ulCqiReqInfo_p->cqiAperiodicInfo.cqiAperiodicMode,
					cqiPeriodicReqInfo_p->cqiPeriodicMsg,
					cqiPeriodicReqInfo_p->cqiRiReconfigMsg, 
					cqiPeriodicReqInfo_p->cqiPeriodicity,
					cqiPeriodicReqInfo_p->cqiPUCCHRBIndex,
					cqiPeriodicReqInfo_p->simultaneousAckNackAndCQI,
					cqiPeriodicReqInfo_p->cqiOffset,
					cqiPeriodicReqInfo_p->periodicFeedBackType, 
					cqiPeriodicReqInfo_p->k,cqiPeriodicReqInfo_p->periodicRepMode,
					cqiPeriodicReqInfo_p->cqiPmiConfigIndex,
					ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellCount].rrcInfpucchConfigReqInfoScell.activeIndexForPUCCHMap,
					ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellCount].rrcInfpucchConfigReqInfoScell.pucchConfigMsg,
					longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex);

			LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
					"[%s] At LP Side(UL_RECONFIG_PER_CQI_RI_CONFIG_INFO), "\
					"riPeriodicity = %d, riOffset = %d riConfigIndex = %d \n",__func__,
					cqiPeriodicReqInfo_p->riConfigIndexInfo.riPeriodicity,
					cqiPeriodicReqInfo_p->riConfigIndexInfo.riOffset,
					cqiPeriodicReqInfo_p->riConfigIndexInfo.riConfigIndex);
		}
	}
	else if(uplkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.cqiRequestType == 0)
	{
		/** SPR 12320 Fix : START **/
		/* + CA_TDD_HARQ_CHANGES */    
		deleteSCellPucchNodesForUE( tempPucchConfigInfo_p, WIDEBAND_CQI_REPORT, 
				&ueCellInfo, internalCellIndex, ueIndex);
		deleteSCellPucchNodesForUE( tempPucchConfigInfo_p, CQI_REPORT , 
				&ueCellInfo, internalCellIndex, ueIndex);
		deleteSCellPucchNodesForUE( tempPucchConfigInfo_p, RI_REPORT , 
				&ueCellInfo, internalCellIndex, ueIndex);
		/* - CA_TDD_HARQ_CHANGES */    
		/** SPR 12320 Fix : END **/
		if(ulReconfigUeReq_p)
		{
			cqiPeriodicMsg = ulCqiReqInfo_p->cqiPeriodicReqInfo.cqiPeriodicMsg;
			memSet(cqiPeriodicReqInfo_p, 0, sizeof(ULCQIPeriodicReqInfo));
			ulCqiReqInfo_p->cqiPeriodicReqInfo.cqiPeriodicMsg = cqiPeriodicMsg;
			ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellCount].
				rrcInfpucchConfigReqInfoScell.activeIndexForPUCCHMap = 
				longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
		}

		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
				"[%s] At LP Side (UL_RECONFIG_PER_CQI_CONFIG_INFO (rel)), \n"\
				"cqiPeriodicity = %d, cqiOffset = %d ,\n"\
				"periodicFeedBackType = %d k = %d periodicRepMode = %d cqiPmiConfigIndex = %d ,\n"\
				"activeIndexForPUCCHMap = %d \n ",__func__,
				cqiPeriodicReqInfo_p->cqiPeriodicity,
				cqiPeriodicReqInfo_p->cqiOffset,
				cqiPeriodicReqInfo_p->periodicFeedBackType, 
				cqiPeriodicReqInfo_p->k,cqiPeriodicReqInfo_p->periodicRepMode,
				cqiPeriodicReqInfo_p->cqiPmiConfigIndex,
				ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellCount].
				rrcInfpucchConfigReqInfoScell.activeIndexForPUCCHMap);
	}
	/*eicic changes*/
	else if(uplkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.cqiConfigRequestType== 0)
	{
        /* Coverity 108841 Fix Start */
		/** SPR 12320 Fix : START **/
#ifdef FDD_CONFIG
		updateCQIMapForAbs_g[internalCellIndex] = LTE_TRUE;
        /* Coverity 108841 Fix END */
		deleteSCellPucchNodesForUE( tempPucchConfigInfo_p, WIDEBAND_CQI_REPORT, 
				&ueCellInfo, internalCellIndex, ueIndex);
		deleteSCellPucchNodesForUE( tempPucchConfigInfo_p, CQI_REPORT , 
				&ueCellInfo, internalCellIndex, ueIndex);
		deleteSCellPucchNodesForUE( tempPucchConfigInfo_p, RI_REPORT , 
				&ueCellInfo, internalCellIndex, ueIndex);
#endif
		/** SPR 12320 Fix : END **/
		updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
		if(ulReconfigUeReq_p)
		{

			ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellCount].
				rrcInfpucchConfigReqInfoScell.activeIndexForPUCCHMap = 
				longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
		}

		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
				"[%s] At LP Side (UL_RECONFIG_PER_CQI_CONFIG_INFO (rel)), \n"\
				"cqiPeriodicity = %d, cqiOffset = %d ,\n"\
				"periodicFeedBackType = %d k = %d periodicRepMode = %d cqiPmiConfigIndex = %d ,\n"\
				"activeIndexForPUCCHMap = %d \n ",__func__,
				cqiPeriodicReqInfo_p->cqiPeriodicity,
				cqiPeriodicReqInfo_p->cqiOffset,
				cqiPeriodicReqInfo_p->periodicFeedBackType, 
				cqiPeriodicReqInfo_p->k,cqiPeriodicReqInfo_p->periodicRepMode,
				cqiPeriodicReqInfo_p->cqiPmiConfigIndex,
				ulReconfigUeReq_p->caConfigUL.scellReconfigInfoUL[scellCount].
				rrcInfpucchConfigReqInfoScell.activeIndexForPUCCHMap);

	}

	if( (0 != uplkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.cqiMode) 
			&& (TRUE == uplkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.cqiInfoFlag)
			&& (ulReconfigUeReq_p))
	{   
		if(tempPucchConfigInfo_p->cqiPmiRiInfoAperiodic.cqiMode != 
				uplkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.cqiMode) 
		{
			ulCqiReqInfo_p->cqiAperiodicInfo.cqiAperiodicMode = 
				dwlkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.cqiMode;
		}
		else
		{
			ulCqiReqInfo_p->cqiConfigMsg &=  UL_RECONFIG_CLEAR_CQI_APERIODIC_IND;

		}          
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,
				"[%s] At LP Side (UL_RECONFIG_APER_CQI_CONFIG_INFO setup), "\
				"CqiMode = %d \n",
				ulCqiReqInfo_p->cqiAperiodicInfo.cqiAperiodicMode);
	}   
	else if(TRUE != uplkUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.cqiInfoFlag 
			&& ulReconfigUeReq_p)
	{
		ulCqiReqInfo_p->cqiConfigMsg |= UL_RECONFIG_CQI_APERIODIC_IND; 
	} 
	return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : macDeleteDLScellContext 
 * Inputs         : dlUeContext_P, dl UE context
 *                  scellId -scell Index
 *                  internalCellIndex - internalCellIndex of cell used at MAC
 * Outputs        : None 
 * Returns        : None
 * Description    : This function deletes the scell context from UE context
 *****************************************************************************/
void macDeleteDLScellContext(DLUEContext * dlUeContext_p, 
		UInt8 scellId,
		InternalCellIndex internalCellIndex )
{
	UInt8 harqCount = 0;

	DLUESCellContext **dlUeTempScellContext_pp = &(dlUeContext_p->dlUeScellContext_p[scellId]);

	if (*dlUeTempScellContext_pp != PNULL)
	{
		DLHARQEntity **harqEntity_pp = &((*dlUeTempScellContext_pp)->ScelldlHarqContext_p);

		if (*harqEntity_pp != PNULL)
		{
			for (;harqCount < ((*harqEntity_pp)->numHARQProcess); harqCount++)
			{
				freeDLHarqProcess(dlUeContext_p,&((*harqEntity_pp)->harqProcess[harqCount]),\
						internalCellIndex);
			}
			freeMemPool(*(harqEntity_pp));
			*harqEntity_pp = PNULL;
		}
		if(PNULL != (*dlUeTempScellContext_pp)->ScelldlCQIInfo.
				subBandReportInfo_p)
		{
			freeMemPool((*dlUeTempScellContext_pp)->\
					ScelldlCQIInfo.subBandReportInfo_p);
			(*dlUeTempScellContext_pp)->ScelldlCQIInfo.\
				subBandReportInfo_p = PNULL;
		}
		freeMemPool(*dlUeTempScellContext_pp);
		*dlUeTempScellContext_pp = PNULL;
		/** SPR 12320 Fix : START **/
		dlUeContext_p->scellCount--;
		/** SPR 12320 Fix : END **/
	}
}

/****************************************************************************
 * Function Name  : macDeleteScellMACDLHarqEntity
 * Inputs         : ueContext_p - UE context pointer
 *                  scellIndex - secondry cell Index
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This API is used to delete a harq Context from the MAC 
 *                  system. (Harq Memory free from strategy) 
 ****************************************************************** **********/
MacRetType macDeleteScellMACDLHarqEntity (DLUEContext*  ueContext_p, 
		InternalCellIndex scellIndex, InternalCellIndex internalCellIndex)
{
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Entry. ",__func__);
	UInt8 harqCount = 0;
	SCELLDLHARQEntity **scellHarqEntity_pp = 
		&(ueContext_p->dlUeScellContext_p[scellIndex]->ScelldlHarqContext_p);
	if (*scellHarqEntity_pp != PNULL)
	{
		for (;harqCount < (*scellHarqEntity_pp)->numHARQProcess; harqCount++)
		{
			freeDLHarqProcess(ueContext_p,&((*scellHarqEntity_pp)->harqProcess[harqCount]), 
					internalCellIndex);
		}
	}    
	else
	{
		lteWarning (" Entry does not exists in the scellHarqDLContextArr_g\n");
		LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR," Unable to delete the Entity. \n");
		return MAC_FAILURE;
	}
	freeMemPool(*scellHarqEntity_pp);
	*scellHarqEntity_pp = PNULL;
	LTE_MAC_UT_LOG(LOG_INFO,UE_CONF_MGR,"[%s] Exit. ",__func__);
	return MAC_SUCCESS;    
}

/******************************************************************************
 * Function Name : macResetScellCQIInfo 
 * Inputs        : ueDLScellContext_p, dlCQIInfo_p
 * Outputs       : None
 * Returns       : None
 * Description   : Initializes Subband and Wideband CQI Info in UE's DL Context 
 *****************************************************************************/
void macResetScellCQIInfo(DLUESCellContext *ueDLScellContext_p,
                  DLCQIInfo *dlCQIInfo_p
                  )
{ 
	dlCQIInfo_p->aperiodicCQIRequested     = FALSE;
	dlCQIInfo_p->isSubBandReportAvailable  = FALSE;
	dlCQIInfo_p->isWideBandReportAvailable = FALSE;
	dlCQIInfo_p->reportReceivedTTI = 0;
	dlCQIInfo_p->previousDLBLERForCW0 = 0;
	dlCQIInfo_p->previousDLBLERForCW1 = 0;
	dlCQIInfo_p->dlBlerCalculatedCounterForCW0 = 0;
	dlCQIInfo_p->dlBlerCalculatedCounterForCW1 = 0;
	dlCQIInfo_p->cqiCorrectionFactorForCW0 = 0;
	dlCQIInfo_p->cqiCorrectionFactorForCW1 = 0;
	dlCQIInfo_p->currentDLBLERForCW0 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->currentDLBLERForCW1 = TARGET_DL_BLER_VALUE;
	/* SPR 13571 fix start */
	dlCQIInfo_p->prevDlBlerCalculatedForCW0 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->prevDlBlerCalculatedForCW1 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->prevDlBlerCalculatedForCW0_2 = TARGET_DL_BLER_VALUE;
	dlCQIInfo_p->prevDlBlerCalculatedForCW1_2 = TARGET_DL_BLER_VALUE;
	/* SPR 13571 fix end */
	ueDLScellContext_p->isSubbandPMIAvailable = FALSE;
	ueDLScellContext_p->aperiodicMode12Rcvd = FALSE;

	if(dlCQIInfo_p->initialCqiValue < 
			dlCQIInfo_p->widebandCQICodeWordOne)
	{
		dlCQIInfo_p->widebandCQICodeWordOne = 
			dlCQIInfo_p->initialCqiValue;
		dlCQIInfo_p->mcsIndexCodeWordOne = getMCSFromCQI( 
                     dlCQIInfo_p->widebandCQICodeWordOne
                     );
	}
	if(dlCQIInfo_p->initialCqiValue < 
			dlCQIInfo_p->widebandCQICodeWordTwo)
	{
		dlCQIInfo_p->widebandCQICodeWordTwo = 
			dlCQIInfo_p->initialCqiValue;
		dlCQIInfo_p->mcsIndexCodeWordTwo = getMCSFromCQI( 
                     dlCQIInfo_p->widebandCQICodeWordTwo
                     );
         }

	/* SPR 14083 fix start */
	if(dlCQIInfo_p->initialCqiValue < 
			dlCQIInfo_p->previousTimeAveragedWBCQICW0)
	{
		dlCQIInfo_p->previousTimeAveragedWBCQICW0 = dlCQIInfo_p->initialCqiValue;
	}
	if(dlCQIInfo_p->initialCqiValue < 
			dlCQIInfo_p->previousTimeAveragedWBCQICW1)
	{
		dlCQIInfo_p->previousTimeAveragedWBCQICW1 = dlCQIInfo_p->initialCqiValue;
	}
	/* SPR 14083 fix end */

	if (PNULL !=  dlCQIInfo_p->subBandReportInfo_p)
	{
		memSet(dlCQIInfo_p->subBandReportInfo_p,0,sizeof(SubBandReportInfo));
	}
}

/***********************************************************************
 * Function Name  : updateDlAggregateGbrPerTTI
 * Inputs         : ueDLContext_p
 *                : dlLcQosInfo_p
 *                : addDeleteTrigger
 *                  numOfTicksPerSecond
 * Outputs        :
 * Returns        : None
 * Description    : This function updates dlAggregateGbrPerTTI in respective
 *                  serving cells based on ADD/DELETE triggers and the
 *                  bandwidthDistribution given from RRM.
 *****************************************************************************/
 void updateDlAggregateGbrPerTTI( DLUEContext* ueDLContext_p,
		DlLcQosInfo* dlLcQosInfo_p,
		DlAggregateGbrTrigger addDeleteTrigger,
		UInt16 numOfTicksPerSecond)
{
	InternalCellIndex pCellIndex = ueDLContext_p->internalCellIndex;
	InternalCellIndex sCellIndex = INVALID_CELL_INDEX;
	UInt8 pCellBandWidthDist = dlLcQosInfo_p->bandWidthDistribution[PCELL_SERV_CELL_IX];
	UInt8 sCellBandWidthDist = dlLcQosInfo_p->bandWidthDistribution[START_SCELL_INDEX];

	UDouble32 absFactor = 0;
	UInt8 absCount = 0;
    UInt32 dlAggGbrOnCell = 0;

	/* EICIC +*/
	if (usableAbsCountNew_g[ ueDLContext_p->internalCellIndex] > 0) 
	{
		absCount = usableAbsCountNew_g[ ueDLContext_p->internalCellIndex];
	}
	else if (absCountNew_g[ ueDLContext_p->internalCellIndex] > 0)
	{
		absCount = absCountNew_g[ueDLContext_p->internalCellIndex];
	}
	else
	{
		absCount = 0;
	}

	if (absCount)
	{

		if (NON_VICTIM_UE == ueDLContext_p->userType)
		{
			absFactor = (UDouble32) (NUM_BITS_ABS_PATTERN/ (UDouble32) (NUM_BITS_ABS_PATTERN - absCount));
		}
		else
		{
			absFactor =  (UDouble32) (NUM_BITS_ABS_PATTERN / (UDouble32) (absCount));
		}

	}
	else
	{
		absFactor = ONE;
	}



	if( (IS_CA_ENABLED()) && 
			(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]))
	{
		sCellIndex = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex;
	}
	/* +coverity id 62974*/
	else
	{
		/* + SPR 11917 Changes */
		/* No need to return from here as same condition is getting validated
		   later in macro IS_SCELL_ACTIVE. 
		   On returning from here in case CA is supported but scell is yet to be
		   created, dlAggregateGbrPerTTI_g will not be updated even for Pcell.
		   Coverity Fix is already taken care in macro IS_SCELL_ACTIVE().
		 */
		/* return; */
		/* - SPR 11917 Changes */
	}
	/* -coverity id 62974*/

    if(DELETE_DL_AGG_GBR_TRIGGER == addDeleteTrigger) 
    {
        /* Coverity fix 27-JUN-2014 start */
        if ( (sCellIndex != INVALID_CELL_INDEX)  &&  IS_SCELL_ACTIVE(ueDLContext_p->ueIndex, START_SCELL_INDEX) )    
            /* Coverity fix 27-JUN-2014 end */
        {
            dlAggGbrOnCell = (((dlLcQosInfo_p->GBR / numOfTicksPerSecond) * pCellBandWidthDist)/100 )*(absFactor);
            if(dlAggregateGbrPerTTI_g[pCellIndex][ueDLContext_p->userType] < dlAggGbrOnCell)
            {
                dlAggregateGbrPerTTI_g[pCellIndex][ueDLContext_p->userType] = 0;
            }
            else
            {
                dlAggregateGbrPerTTI_g[pCellIndex][ueDLContext_p->userType] -= dlAggGbrOnCell;
            }
            dlAggGbrOnCell = (((dlLcQosInfo_p->GBR / numOfTicksPerSecond) * sCellBandWidthDist)/100 )*(absFactor);
            if(dlAggregateGbrPerTTI_g[sCellIndex][ueDLContext_p->userType] < dlAggGbrOnCell)
            {
                dlAggregateGbrPerTTI_g[sCellIndex][ueDLContext_p->userType] = 0;
            }
            else
            {
                dlAggregateGbrPerTTI_g[sCellIndex][ueDLContext_p->userType] -= dlAggGbrOnCell;
            }
        }
        else
        {
            dlAggGbrOnCell = (dlLcQosInfo_p->GBR / numOfTicksPerSecond)*(absFactor);
            if(dlAggregateGbrPerTTI_g[pCellIndex][ueDLContext_p->userType] < dlAggGbrOnCell)
            {
                dlAggregateGbrPerTTI_g[pCellIndex][ueDLContext_p->userType] = 0;
            }
            else
            {
                dlAggregateGbrPerTTI_g[pCellIndex][ueDLContext_p->userType] -= dlAggGbrOnCell;
            }
        }
        dlAggGbrOnCell = (dlLcQosInfo_p->GBR / NUM_TICKS_PER_SECOND)*(absFactor);
        if(totalDlAggregateGbrPerTTI_g[ueDLContext_p->userType] < dlAggGbrOnCell)
        {
            totalDlAggregateGbrPerTTI_g[ueDLContext_p->userType] = 0;
        }
        else
        {
            totalDlAggregateGbrPerTTI_g[ueDLContext_p->userType] -= dlAggGbrOnCell;
        }



    }
	if(ADD_DL_AGG_GBR_TRIGGER == addDeleteTrigger)
	{

		/* Coverity fix 27-JUN-2014 start */
		if( (sCellIndex != INVALID_CELL_INDEX) && IS_SCELL_ACTIVE(ueDLContext_p->ueIndex, START_SCELL_INDEX))    
			/* Coverity fix 27-JUN-2014 end */
		{

			dlAggregateGbrPerTTI_g[pCellIndex][ueDLContext_p->userType] += ((((dlLcQosInfo_p->GBR / numOfTicksPerSecond) *
							pCellBandWidthDist)/100) *(absFactor));
			dlAggregateGbrPerTTI_g[sCellIndex][ueDLContext_p->userType] += ((((dlLcQosInfo_p->GBR / numOfTicksPerSecond) *
							sCellBandWidthDist)/100 )*(absFactor));
		}
		else
		{

			dlAggregateGbrPerTTI_g[pCellIndex][ueDLContext_p->userType] += ((dlLcQosInfo_p->GBR / numOfTicksPerSecond)*(absFactor));


		}
		totalDlAggregateGbrPerTTI_g[ueDLContext_p->userType] += ((dlLcQosInfo_p->GBR / numOfTicksPerSecond)*(absFactor));


	}
}

/*****************************************************************************
 * Function Name  : setUeSearchSpaceInfoforScell
 * Inputs         : ueDLContext_p-Pointer to DL UE Context
 *                : ueULContext_p- Pointer to UL UE Context
 *                  internalCellIndex - cell index used at MAC
 * Outputs        :
 * Returns        : None
 * Description    : This function updates the ueSearchSpaceInfo_p in Scell Context 
 of UE .
 *****************************************************************************/
void setUeSearchSpaceInfoforScell(ULUEContext *ULUEContext_p,DLUEContext *DLUEContext_p ,
		RNTIInfo *rntiInfo_p,InternalCellIndex internalCellIndex)
{


	if(internalCellIndex)
	{
		if((rntiInfo_p) && (ULUEContext_p) && (ULUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]))
		{
			/*set ue specific serach space pointer*/
			ULUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->ueSearchSpaceInfo_p =
				(&rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex - 1][0]);
		}
		/* Coverity_63706_Fix Start */
		if( (rntiInfo_p) && (DLUEContext_p) && (DLUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]))
			/* Coverity_63706_Fix End */
		{
			DLUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ueSearchSpaceInfo_p =
				(&rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex - 1][0]);
		}
	}
	else
	{
		if((rntiInfo_p) && (ULUEContext_p) && (ULUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]))
		{

			ULUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->ueSearchSpaceInfo_p =
				(&rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex + 1][0]);
		}
		if((rntiInfo_p) && (DLUEContext_p) && (DLUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]) )
		{

			DLUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ueSearchSpaceInfo_p =
				(&rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex + 1 ][0]);
		}

	}
}
/* Cyclomatic_complexity_changes_start */
/*****************************************************************************
 * Function Name  : deleteDlLcStrategyNode
 * Inputs         : ueDLContext_p-Pointer to DL UE Context
 *                 
 * Outputs        : None.
 * Returns        : None
 * Description    : This function deletes DL LC strategy node.
 ******************************************************************************/

void deleteDlLcStrategyNode(DLUEContext *ueDLContext_p)
{
	UInt8 lcCount=0;
	DLLogicalChannelInfo *dlLogicalChInfo_p  = PNULL;
	UInt8 scellId = 1;

	for(lcCount=0;lcCount<MAX_NUMBER_OF_LOGICAL_CHANNEL;lcCount++)
	{
		dlLogicalChInfo_p=&ueDLContext_p->logicalChannel[lcCount];
		if(dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p)
		{
			if(IS_CA_ENABLED() && (ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]))
			{
				for (scellId = MAX_NUM_PCELL; scellId <= MAX_NUM_SCELL; scellId++)
				{
					freeMemPool(dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->\
							dLUESCellStartegyNode_p[scellId]);
					dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->\
						dLUESCellStartegyNode_p[scellId] = PNULL;
				}
			}
			freeMemPool(dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p);
			dlLogicalChInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p=PNULL;
		}
		/* SPR_10354_Base_Code_Fix Start */
		dlLogicalChInfo_p->logicalChannelId = INVALID_LCID;
		/* SPR_10354_Base_Code_Fix Start */
	}
	return;
}
/*****************************************************************************
 * Function Name  : deleteUlUeScellContext
 * Inputs         : ueDLContext_p-Pointer to DL UE Context
 *
 * Outputs        : None.
 * Returns        : None
 * Description    : This function deletes UE Scll context.
 *******************************************************************************/

void deleteUlUeScellContext(ULUEContext *ueULContext_p)
{
	UInt8 index = 0;

	for (index = MAX_NUM_PCELL; index <= MAX_NUM_SCELL;index ++)
	{
		if (ueULContext_p->ulUeScellContext_p[index] != PNULL)
		{
			freeMemPool(ueULContext_p->ulUeScellContext_p[index]);
			ueULContext_p->ulUeScellContext_p[index] = PNULL;
		}
	}
	return;
}
/*****************************************************************************
 * Function Name  : deleteUlLcStrategyNode
 * Inputs         : ueULContext_p-Pointer to UL UE Context
 * Outputs        : None.
 * Returns        : None
 * Description    : This function deletes UL LC strategy node.
 ******************************************************************************/

void deleteUlLcStrategyNode(ULUEContext *ueULContext_p)
{
	/*Delete UL LC Strategy Node*/
	ULLogicalChannelInfo *ulLogicalChInfo_p  = PNULL;
	UInt8 lcCount = 0;

	for(lcCount=0;lcCount<MAX_NUMBER_OF_LOGICAL_CHANNEL;lcCount++)
	{
		ulLogicalChInfo_p = &(ueULContext_p->lcInfo[lcCount]);
		if(ulLogicalChInfo_p->ulQosInfo.ulQosStrategyLCNode_p)
		{
			freeMemPool(ulLogicalChInfo_p->ulQosInfo.ulQosStrategyLCNode_p);
			ulLogicalChInfo_p->ulQosInfo.ulQosStrategyLCNode_p = PNULL;
		}
	}
	return;
}
/*****************************************************************************
 * Function Name  : deleteUlLcStrategyNodeFromAmbrInfo
 * Inputs         : ueULContext_p-Pointer to UL UE Context
 * Outputs        : None.
 * Returns        : None
 * Description    : This function Delete UL LC Strategy Node from UL UE's AMBR info.
 *******************************************************************************/
void deleteUlLcStrategyNodeFromAmbrInfo(ULUEContext *ueULContext_p)
{

	/*Delete UL LC Strategy Node from UL UE's AMBR info*/ 
	if(ueULContext_p->ulAmbrInfo.ulQosStrategyLCNode_p)
	{
		freeMemPool(ueULContext_p->ulAmbrInfo.ulQosStrategyLCNode_p);
		ueULContext_p->ulAmbrInfo.ulQosStrategyLCNode_p=PNULL;
	}

	return;
}
/*****************************************************************************
 * Function Name  : deleteFromNonZeroBsrList
 * Inputs         : ueULContext_p-Pointer to UL UE Context
 *                  internalCellIndex
 * Outputs        : None.
 * Returns        : None
 * Description    : This function Delete from NonZero Bsr List.
 ********************************************************************************/
void deleteFromNonZeroBsrList(ULUEContext *ueULContext_p,
		InternalCellIndex internalCellIndex)
{
	/*Coverity 65727 Start*/
     /*Jira_732_Changes Start*/
    /* + SPR_17858_732_CHANGES */
    if(PNULL != ueULContext_p->nonZeroBsrListTempDeleted_p)
    {
        freeMemPool(ueULContext_p->nonZeroBsrListTempDeleted_p);
        ueULContext_p->nonZeroBsrListTempDeleted_p = PNULL;
        ueULContext_p->nonZeroBsrList_p = PNULL;
    }
	/* L2_FIX_start */
	else if(PNULL != ueULContext_p->nonZeroBsrList_p)
	{
		/*CA Changes start  */
		deleteEntryFromNonZeroBSRList(ueULContext_p,ueULContext_p->nonZeroBsrList_p,internalCellIndex);
		/*CA Changes end  */
		ueULContext_p->nonZeroBsrList_p = PNULL;
	}
     /*Jira_732_Changes End*/
	/*Coverity 65727 End*/
	/* L2_FIX_end */
	return;
    /* - SPR_17858_732_CHANGES */
}
/*****************************************************************************
 * Function Name  : deleteMcsToPrbMap
 * Inputs         : ueULContext_p-Pointer to UL UE Context
 * Outputs        : None.
 * Returns        : None
 * Description    : This function Delete mcsToPrbMap.
 *********************************************************************************/

void deleteMcsToPrbMap(ULUEContext *ueULContext_p)
{

	if (PNULL != ueULContext_p->csiContext.mcsToPrbMap_p)
	{
		freeMemPool(ueULContext_p->csiContext.mcsToPrbMap_p);
		ueULContext_p->csiContext.mcsToPrbMap_p = PNULL;
	}
	return;
}
/* Cyclomatic_complexity_changes_end */

/*CYCLOMATIC_CHANGES_6.1_START*/
/*****************************************************************************
 * Function Name  : determineULMinDataSize
 * Inputs         : uplkUEInfo_p,ueContext_p
 * Outputs        : Will update the min data size for UE ul grant
 * Returns        : None
 * Description    : This function will update the min data size for uplink grant
 * *                depending on the compliance release
 *********************************************************************************/
STATIC  void determineULMinDataSize(UeContextForUplink *uplkUEInfo_p,
		ULUEContext *ueContext_p)
{
	/* SPR 5713 changes start */
	if((RELEASE_8 == uplkUEInfo_p->ueComplianceRelease) ||
			(RELEASE_9 == uplkUEInfo_p->ueComplianceRelease))
	{
		ueContext_p->ueComplianceRelease = uplkUEInfo_p->ueComplianceRelease;
		ueContext_p->ulGrantMinDataSize =
			ULGRANT_MIN_DATA_SIZE_FOR_REL_9;
	}
//phy release fix
    else if((RELEASE_10 == uplkUEInfo_p->ueComplianceRelease)
           )
	{
		ueContext_p->ueComplianceRelease = uplkUEInfo_p->ueComplianceRelease;
		ueContext_p->ulGrantMinDataSize =
			ULGRANT_MIN_DATA_SIZE_FOR_REL_10;
	}
	/* SPR 5713 changes end */
}
/*****************************************************************************
 * Function Name  : determineMaxTBSize
 * Inputs         : uplkUEInfo_p,ulReconfigUeReq_p
 * Outputs        : Will calculate the max TB size 
 * Returns        : None
 * Description    : This function will determine the max TB size depening upon
 *                  the UE category
 *********************************************************************************/
STATIC  void determineMaxTBSize(UeContextForUplink *uplkUEInfo_p,
		ULReconfigUeReq *ulReconfigUeReq_p)
{
	/* SPR 2873 fix strat*/
	/* SPR 11777 Changes Start */
	/*
	 ** In MAC_RECONFIGURE_UE_ENTITY_REQ message, UE Category parameter
	 ** is not mandatory. If UE category is not present in reconfig
	 ** message then ueCategory will be 0 in the uplkUEInfo_p->ueCategory
	 ** as its memset with 0.
	 **
	 ** Therefore, Validating the UE Category here.
	 */
	if((uplkUEInfo_p->ueCategory >= MAC_MIN_UE_CATEGORY) &&
			(uplkUEInfo_p->ueCategory <= MAC_MAX_UE_CATEGORY))
	{
		ulReconfigUeReq_p->ueCategoryInfo.ueCategory=
			uplkUEInfo_p->ueCategory;
		/*Update the max TB size for the category of the UE
		 * */
                /* SPR 19600 Changes start */
		ulReconfigUeReq_p->ueCategoryInfo.maxTBSize=
			ulUeCategoryInfo_g[ulReconfigUeReq_p->ueCategoryInfo.ueCategory - 1].maxBytesULSCHTb;
                /* SPR 19600 Changes end */
	}
	/* SPR 2873 fix end*/
}
/*****************************************************************************
 * Function Name  : populateUplinkSpsInfo
 * Inputs         : uplkUEInfo_p,ulReconfigUeReq_p,ueContext_p
 * Outputs        : Will update the uiplink SPS information
 * Returns        : None
 * Description    : This function will update the uplink SPS information of the
 *                  UE
 *********************************************************************************/
/*SPR 16855 +-*/
STATIC  void populateUplinkSpsInfo(UeContextForUplink *uplkUEInfo_p,
		ULUEContext *ueContext_p,
		InternalCellIndex internalCellIndex,
		ULReconfigUeReq *ulReconfigUeReq_p)
/*SPR 16855 +-*/
{
	CellConfigParams *cellParams_p = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /*SPR 16855 +-*/
	ulReconfigUeReq_p->spsUlInfo.\
		semiPersistentSchedIntervalUL = \
		uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
		semiPersistentSchedIntervalUL;
	ulReconfigUeReq_p->spsUlInfo.\
		implicitReleaseAfter =\
		ulImplicitReleaseAfterMap[uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
		implicitReleaseAfter];

	if(TRUE == uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isP0TagPresent)
	{
		ulReconfigUeReq_p->spsUlInfo.isP0TagPresent = TRUE;
		ulReconfigUeReq_p->spsUlInfo.\
			p0PersistentInfo.p0NominalPuschPersistent = \
			uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
			p0PersistentInfo.p0NominalPuschPersistent;
		ulReconfigUeReq_p->spsUlInfo.\
			p0PersistentInfo.p0UePuschPersistent = \
			uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
			p0PersistentInfo.p0UePuschPersistent;
	}
	else
	{
		/*Assigning the Normal values */
		ulReconfigUeReq_p->spsUlInfo.\
			p0PersistentInfo.p0NominalPuschPersistent = \
			cellParams_p->commonULPowerControlInfo.p0NominalPUSCH;
		ulReconfigUeReq_p->spsUlInfo.\
			p0PersistentInfo.p0UePuschPersistent = \
			ueContext_p->dedicatedULPowerControlInfo.p0UePusch;
	}
	if(TRUE == uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isTwoIntervalConfigPresent)
	{
		ulReconfigUeReq_p->spsUlInfo.isTwoIntervalConfigPresent = TRUE;
		ulReconfigUeReq_p->spsUlInfo.\
			twoIntervalsConfig = \
			uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
			twoIntervalsConfig;
	}
	/* + SPS_TDD_Changes */
	else
	{
        ulReconfigUeReq_p->spsUlInfo.isTwoIntervalConfigPresent = FALSE;
        ulReconfigUeReq_p->spsUlInfo.twoIntervalsConfig = 0;
	}
	/* - SPS_TDD_Changes */

    /*SPR 16855 +-*/
}
/*****************************************************************************
 * Function Name  : validateUeCatV1020ReqType
 * Inputs         : uplkUEInfo_p,ulReconfigUeReq_p,internalCellIndex
 * Outputs        : Will populate the UeCategory in  ULReconfigUeReq
 * Returns        : None
 * Description    : This function will validate the ueCategory V1020 request
 * type
 *********************************************************************************/
STATIC  MacRetType validateUeCatV1020ReqType(UeContextForUplink *uplkUEInfo_p,
		InternalCellIndex internalCellIndex,
		ULReconfigUeReq *ulReconfigUeReq_p)
{
	/* CA Changes Start */
	/* SPR 12246 Start */
	if (TRUE == uplkUEInfo_p->ueCategoryV1020ReqType)
	{
		if ( macValidateUeCategoryV1020(uplkUEInfo_p->ueCategoryV1020, internalCellIndex) )
		{
			ulReconfigUeReq_p->ueCategoryInfov1020.ueCategoryV1020 = uplkUEInfo_p->ueCategoryV1020;
		}
		else
		{
			return MAC_FAILURE;
		}
	}
	/* SPR 12246 End */
	/* CA Changes End */
	return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : populatePuschConfigInfo
 * Inputs         : uplkUEInfo_p,ulReconfigUeReq_p
 * Outputs        : Will populate the pusch config info in ULReconfigUeReq 
 * Returns        : None
 * Description    : This function will populate the pusch config info in
 * ULReconfigUeReq 
 *********************************************************************************/
STATIC  void populatePuschConfigInfo(UeContextForUplink *uplkUEInfo_p,
		ULReconfigUeReq *ulReconfigUeReq_p)
{
	if (uplkUEInfo_p->puschConfigInfoFlag )
	{
		ulReconfigUeReq_p->puschConfigInfo.betaOffsetAckIndex =
			uplkUEInfo_p->betaOffsetAckIndex;
		ulReconfigUeReq_p->puschConfigInfo.betaOffsetRiIndex =
			uplkUEInfo_p->betaOffsetRiIndex;
		ulReconfigUeReq_p->puschConfigInfo.betaOffsetCQIIndex =
			uplkUEInfo_p->betaOffsetCQIIndex;
	}
}    
/*****************************************************************************
 * Function Name  : populateDedicatedULPowerControlInfo
 * Inputs         : uplkUEInfo_p,ulReconfigUeReq_p,ueContext_p
 * Outputs        : Will populate the power control info in
 * ULReconfigUeReq/ULUEContext 
 * Returns        : None
 * Description    : This function will populate the dedicated power control info
 *********************************************************************************/
/* +- SPR 17777 */
STATIC  void populateDedicatedULPowerControlInfo(UeContextForUplink *uplkUEInfo_p,
		ULReconfigUeReq *ulReconfigUeReq_p)
/* +- SPR 17777 */
{
	if (uplkUEInfo_p->isDedicatedULPowerControlInfoPresent == TRUE)
	{
		/* Update the reconfigured p0UePusch value */
        /*SPR 16855 +-*/
		ulReconfigUeReq_p->p0UePuschInfo.p0UePusch =
			uplkUEInfo_p->dedicatedULPowerControlInfo.p0UePusch;
		ulReconfigUeReq_p->p0UePuschInfo.prevPowerCorrectionForPusch = 0;
		ulReconfigUeReq_p->p0UePuschInfo.currPowerCorrectionForPusch = 0;
		ulReconfigUeReq_p->p0UePuschInfo.powerControlPuschTPCValue = 0;
        /*SPR 16855 +-*/
	}
}
/*****************************************************************************
 * Function Name  : populateTpcPdcchConfigPuschInfo
 * Inputs         : uplkUEInfo_p,ulReconfigUeReq_p,ueContext_p,internalCellIndex
 * Outputs        : Will populate the tpc Pdcch config pusch info
 * Returns        : None
 * Description    : This function will populate the tpc pdcch config pusch info
 *********************************************************************************/
/*SPR 16855 +-*/
STATIC  void populateTpcPdcchConfigPuschInfo(UeContextForUplink *uplkUEInfo_p,
		ULReconfigUeReq *ulReconfigUeReq_p)
/*SPR 16855 +-*/
{
    /*SPR 16855 +-*/
	if (uplkUEInfo_p->isTpcPdcchConfigPuschPresent == TRUE)
	{
        /*SPR 16855 +-*/
		ulReconfigUeReq_p->tpcPDCCHConfigPUSCHInfo.release =
			uplkUEInfo_p->tpcPdcchConfigPusch.release;
		ulReconfigUeReq_p->tpcPDCCHConfigPUSCHInfo.tpcIndex =
			uplkUEInfo_p->tpcPdcchConfigPusch.tpcBitMapIndex;
		ulReconfigUeReq_p->tpcPDCCHConfigPUSCHInfo.tpcRnti =
			uplkUEInfo_p->tpcPdcchConfigPusch.tpcRnti;
		ulReconfigUeReq_p->tpcPDCCHConfigPUSCHInfo.isConfiguredForDci3Or3A =
			uplkUEInfo_p->tpcPdcchConfigPusch.isConfiguredForDci3Or3A;
        /*SPR 16855 +-*/
	}
	/* CLPC_REWORK_CHG END */
}
/*SPR 16855 +-*/
/*****************************************************************************
 * Function Name  : populateUplinkLcInfo
 * Inputs         : uplkUEInfo_p,ulReconfigUeReq_p,uplkLCCount
 * Outputs        : Will populate the uplink lc info
 * Returns        : None
 * Description    : This function will populate the uplink lc info
 *********************************************************************************/
STATIC  void populateUplinkLcInfo(UInt32 uplkLCCount, 
		ULReconfigUeReq *ulReconfigUeReq_p,
		UeContextForUplink *uplkUEInfo_p)
{
	UInt32 channelIndex = 0;
	ulReconfigUeReq_p->ulReconfigLcReq.numLogicalChannels = uplkLCCount;    
	for(channelIndex = 0; channelIndex < uplkLCCount ; channelIndex++)
	{
		ulReconfigUeReq_p->ulReconfigLcReq.lcInfo[channelIndex] =
			uplkUEInfo_p->lcInfo[channelIndex];
		LTE_MAC_UT_LOG(LOG_MINOR,UE_CONF_MGR,"[%s] At LP (UL_RECONFIG_CONFIGURE_LC_REQ) channelIndex = %d operationType = %d "\
				"lchId = %d rlcMode = %d lcGId = %d result = %d QCI = %d channelType = %d logicalChannelPriority = %d "\
				"GBR = %d MBR = %d \n",__func__,channelIndex,ulReconfigUeReq_p->ulReconfigLcReq.lcInfo[channelIndex].operationType,
				ulReconfigUeReq_p->ulReconfigLcReq.lcInfo[channelIndex].lchId,
				ulReconfigUeReq_p->ulReconfigLcReq.lcInfo[channelIndex].rlcMode,
				ulReconfigUeReq_p->ulReconfigLcReq.lcInfo[channelIndex].lcGId,
				ulReconfigUeReq_p->ulReconfigLcReq.lcInfo[channelIndex].result,
				ulReconfigUeReq_p->ulReconfigLcReq.lcInfo[channelIndex].QCI,
				ulReconfigUeReq_p->ulReconfigLcReq.lcInfo[channelIndex].channelType,
				ulReconfigUeReq_p->ulReconfigLcReq.lcInfo[channelIndex].logicalChannelPriority,
				ulReconfigUeReq_p->ulReconfigLcReq.lcInfo[channelIndex].GBR,
				ulReconfigUeReq_p->ulReconfigLcReq.lcInfo[channelIndex].MBR
			      );
	}
}
/*SPR 16855 +-*/
/*****************************************************************************
 * Function Name  : populateN1PucchListN1PucchCs
 * Inputs         : uplkUEInfo_p,ulReconfigUeReq_p
 * Outputs        : Will populate the N1 pucch list and n1 pucch Cs
 * Returns        : None
 * Description    : This function will populate n1 pucch list and n1 pucch Cs
 *********************************************************************************/
STATIC  void populateN1PucchListN1PucchCs(ULReconfigUeReq *ulReconfigUeReq_p,
		UeContextForUplink *uplkUEInfo_p)
{
	UInt8 n1PucchList = 0, n1PucchCs = 0;
	for (n1PucchList = 0; n1PucchList < MAX_PUCCH_AN_CS_LIST; n1PucchList++)
	{
		for (n1PucchCs = 0; n1PucchCs < MAX_PUCCH_AN_CS; n1PucchCs++)
		{
			ulReconfigUeReq_p->pucchFormatR10.n1Pucch[n1PucchList][n1PucchCs]=
				uplkUEInfo_p->pucchFormatR10.n1Pucch[n1PucchList][n1PucchCs];
		}
	}
}
/*****************************************************************************
 * Function Name  : fillUlQosAmbr
 * Inputs         : uplkUEInfo_p,ulReconfigUeReq_p,ueContext_p,internalCellIndex
 * Outputs        : Will populate the uplink QOS ambr info
 * Returns        : None
 * Description    : This function will populate uplink QOS ambr info
 *********************************************************************************/
STATIC  void fillUlQosAmbr(UeContextForUplink *uplkUEInfo_p,
        /*SPR 16855 +-*/
		ULReconfigUeReq *ulReconfigUeReq_p
        /*SPR 16855 +-*/
		)
{
    /*SPR 16855 +-*/
	/*Fill AMBR*/
	if(TRUE == uplkUEInfo_p->ulAmbrPresent)
	{
        /*SPR 16855 +-*/
		ulReconfigUeReq_p->ulAmbr = uplkUEInfo_p->ulAmbr;
        /*SPR 16855 +-*/
	}
}
/*CYCLOMATIC_CHANGES_6.1_END*/

