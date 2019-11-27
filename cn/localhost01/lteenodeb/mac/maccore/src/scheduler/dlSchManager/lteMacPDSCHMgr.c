
/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacPDSCHMgr.c,v 1.1.1.1.6.1.4.2.2.2 2010/10/11 12:57:03 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : THIS FILE CONTAINS THE FUNCTIONS RELATED TO PDSCH 
 *                          Allocation for DL Msg
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 *$Log: lteMacPDSCHMgr.c,v $
 *Revision 1.1.1.1.6.1.4.2.2.2  2010/10/11 12:57:03  gur10121
 *SPR #706 fix
 *
 *Revision 1.1.1.1.6.1.4.2.2.1.4.1  2010/10/08 07:42:47  gur10121
 * SPR 706 Fix - KlockWork Warnings removed
 *
 *Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:46:16  gur20491
 *FAPI changes
 *
 *Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 *FrameworkSetupWith1.0_MAC1.2.4
 *
 *Revision 1.73  2009/11/14 14:29:41  gur11912
 *removed Warnings generated on O2
 *
 *Revision 1.72  2009/11/01 11:40:40  gur11912
 *update Resource Allocation Type
 *
 *Revision 1.71  2009/10/21 06:00:19  gur19413
 *stats bug fixed for lastScheduledTick
 *
 *Revision 1.70  2009/09/03 15:31:50  gur20052
 * Added Warnings LOGS
 *
 *Revision 1.69  2009/09/01 16:57:51  gur20439
 *DL Strategy Panic BUG fix.
 *
 *Revision 1.68  2009/08/09 13:39:29  gur12140
 *Solved Bug
 *
 *Revision 1.67  2009/08/07 12:30:37  gur12140
 *Solved Bug
 *
 *Revision 1.66  2009/08/06 09:07:50  gur12140
 *Incorporated Comments
 *
 *Revision 1.65  2009/08/02 10:46:14  gur12140
 *Incorporated Comments and Solved Bug
 *
 *Revision 1.64  2009/07/30 12:40:19  gur12140
 *Solved Bug
 *
 *Revision 1.63  2009/07/29 17:15:41  gur12140
 *Removed Warnings
 *
 *Revision 1.62  2009/07/29 07:44:10  gur12140
 *Added T_LOGS
 *
 *Revision 1.60  2009/07/28 09:50:53  gur15697
 *freeMemPool(dlBCCHMsgInfo_p->data_p) removed
 *
 *Revision 1.59  2009/07/27 08:07:02  gur12140
 *Incorporated Comments
 *
 *Revision 1.58  2009/07/26 09:01:51  gur15697
 *compilation warnings removed
 *
 *Revision 1.57  2009/07/24 13:32:11  gur15697
 *isPdcchAllocatedFlag = FALSE added for CCCH message
 *
 *Revision 1.56  2009/07/23 12:30:52  gur15697
 *changes for  DL Stats
 *
 *Revision 1.55  2009/07/23 12:00:32  gur15697
 *DL Stats bug fix
 *
 *Revision 1.54  2009/07/21 15:16:04  gur15697
 *UT bugfix
 *
 *Revision 1.53  2009/07/21 05:21:11  gur12140
 *Solved Bug
 *
 *Revision 1.52  2009/07/20 11:15:54  gur12140
 *Solved Bug
 *
 *Revision 1.51  2009/07/20 05:18:07  gur12140
 *Replaced MACModuleLogDetail_g wih PDSCH_MGR
 *
 *Revision 1.50  2009/07/17 05:07:14  gur15697
 *vrb type changed to localized
 *
 *Revision 1.49  2009/07/16 05:01:43  gur12140
 *Solved Bug
 *
 *Revision 1.48  2009/07/14 04:20:53  gur12140
 *Incorporated Comments
 *
 *Revision 1.47  2009/07/14 04:09:34  gur12140
 *Indented Code
 *
 *Revision 1.46  2009/07/08 09:12:22  gur12140
 *Incorporated Comments
 *
 *Revision 1.45  2009/07/07 15:16:24  gur18569
 *changed putEntryInEncoderQueue to putEntryInSchEncoderQueue
 *
 *Revision 1.44  2009/07/07 07:00:54  gur12140
 *Incorporated Comments
 *
 *Revision 1.43  2009/07/06 11:45:56  gur12140
 *Incorporated Comments
 *
 *Revision 1.42  2009/07/03 13:38:40  gur15697
 *fixing the invalid msgType
 *
 *Revision 1.41  2009/07/03 09:08:26  gur12140
 *Incorporated Comments
 *
 *Revision 1.40  2009/07/03 06:38:37  gur12140
 *Incorporated Comments
 *
 *Revision 1.38  2009/06/30 08:16:37  gur12140
 *Solved Bug related to Scheduled Tick in invokeMuxLogic Funtion
 *
 *Revision 1.37  2009/06/29 13:42:04  gur12140
 *Incorporated Comments
 *
 *Revision 1.34  2009/06/25 13:45:00  gur12140
 *Solved Bug
 *
 *Revision 1.33  2009/06/25 09:57:20  gur12140
 *Incorporated Comments
 *
 *Revision 1.32  2009/06/24 09:28:41  gur12140
 *Solved Bug
 *
 *Revision 1.31  2009/06/24 08:13:33  gur12140
 *Solved Bug Related to Filling of HarqProcessId in DCiPDUInfo Structure
 *
 *Revision 1.30  2009/06/22 11:27:08  gur12140
 *Removed Error
 *
 *Revision 1.29  2009/06/22 10:14:06  gur12140
 *Solved Bug
 *
 *Revision 1.28  2009/06/19 13:29:50  gur12140
 *Replaced LTE_LOG with LTE_MAC_UT_LOG
 *
 *Revision 1.27  2009/06/19 09:07:34  gur12140
 *Incorporated Comments
 *
 *Revision 1.26  2009/06/19 08:33:01  gur12140
 *Need to Intialize dciPduInfo_p in
 *POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_MUL_DATA_MSG Macro
 *
 *Revision 1.25  2009/06/19 07:27:04  gur12140
 *Added Macro for filling Spatial and Non Spatial DCI CCE Info Container
 *
 *Revision 1.24  2009/06/19 05:37:07  gur12140
 *Solved Bug and Optimized It
 *
 *Revision 1.23  2009/06/18 15:05:21  gur12140
 *Incorporated Comments for DummyMsg
 *
 *Revision 1.22  2009/06/18 10:41:50  gur12140
 *Solved Bug
 *
 *Revision 1.21  2009/06/18 07:43:31  gur12140
 *Solved Bugs
 *
 *Revision 1.20  2009/06/16 15:16:55  gur12140
 *Removed Compilation Errors
 *
*Revision 1.19  2009/06/16 14:46:31  gur12140
*Incorporated Comments
*
*Revision 1.17  2009/06/08 12:08:40  gur12140
*Removed Compilation Erros
*
*Revision 1.16  2009/06/05 11:01:48  gur12140
*Removed Linking Errors
*
*Revision 1.15  2009/06/05 06:21:10  gur12140
*Comments Incorporated
*
*Revision 1.14  2009/06/01 12:56:50  gur12140
*Removed Compilation Error due to change in cellConfig_g structure
*
*Revision 1.12  2009/05/20 12:10:05  gur12140
*Incorporated Comments
*
*Revision 1.11  2009/05/10 12:12:19  gur12140
*Made Code Optimized
*
*Revision 1.10  2009/05/10 11:43:08  gur12140
*Removed Warnings
*
*Revision 1.9  2009/05/10 11:37:17  gur12140
*Added funtionality for Popualting DCI CCE Control Info
*
*Revision 1.8  2009/05/10 09:53:33  gur12140
*Removed Encoder Depenedency
*
*Revision 1.7  2009/05/10 08:19:17  gur12140
*Removed Warnings and Did Some Indentation
*
*Revision 1.4  2009/05/09 16:42:24  gur12140
*Removed Warnings
*
*
*****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
#include "lteMacPDSCHMgr.h"
#include "lteMacTypes.h"
#include "lteMacCellConfMgr.h"
#include "lteMacMultiplexer.h"
#include "lteMacDLHarqMgr.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacOAMInterface.h"
#include "lteMacPucchPowerControl.h"
#ifdef TDD_CONFIG
    #include "lteMacTDDConfig.h"
#endif    
/* ICIC changes start */
#include "lteMacDLResourceInfoMgr.h"
/* ICIC changes end */
#include "ltePerfStats.h"
/* + PRS_CHANGES */
#include "lteMacPRSMgr.h"
/* - PRS_CHANGES */
#ifdef LTE_EMBMS_SUPPORTED
#include "lteMacEMbmsSchedulerStrategy.h"
#endif
/******************************************************************************
 * Project Includes
 *****************************************************************************/

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/* SPR 3306 Fix Starts */
extern void reserveCommonChannelRB(ResourceAllocationPolicyInfo *raPolicyInfo_p,
        InternalCellIndex internalCellIndex);
extern void unreserveMIBRB(ResourceAllocationPolicyInfo *raPolicyInfo_p,
        InternalCellIndex internalCellIndex);
/* SPR 23402 Changes Start */
/*MOCN SPR 21913 Start*/                                                                      
extern mocnOperatorParams mocnOperatorParams_g;
/*MOCN SPR 21913 End*/       
/* SPR 23402 Changes End */                                                               
/* SPR 3306 Fix Ends */
/* ICIC changes start */
/* SPR 5599 changes start (ZIP ID 129098) */
extern UInt32 allocateRBForMaxTBSize(DLHARQProcess *harqProcess_p, 
/* SPR 5599 changes end (ZIP ID 129098) */
                                            UInt32 allocatedRb,
                                            UInt32 availableRBCount,
                                            UInt32 maxTBSize,
                                            UInt8  resourceRegionAllocated,
                                            InternalCellIndex internalCellIndex
                                            );

/* + TM7_8 Changes Start */
extern void updateMCSIndexAndTBSIzeForTransMode_7_8(
        /*SPR 19029  Start*/
        DLHARQProcess *harqProcess_p , InternalCellIndex internalCellIndex);
        /*SPR 19029 End*/
/* - TM7_8 Changes End */

/* This flag indicates that only RAT2 Localized can be used for common channel
 * messages
 */
/* +- SPR 20073 fix */

/* This flag is used to detect last UE scedued in PDSCH */
UInt8 lastUEDetectFlag_g[MAX_NUM_CELL];
/*SPR 3679 Qos Changes Start*/
UInt32 totalRbMIMO_g[MAX_NUM_CELL];
/*SPR 3679 Qos Changes End*/
/* + SPR 22895 */
static ResourceAllocationPolicyInfo raPolicyInfo_g[MAX_NUM_CELL];
/* - SPR 22895 */

//SPR 8545 FIX START
#define MCS_REDUCTION_FACTOR_FOR_SPL_SUBFRAME 3
//SPR 8545 FIX END

/*eicic changes starts*/
 extern UInt8 absPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN];
 extern UInt8 absPatternIndex_g[MAX_NUM_CELL];
 extern UInt8 usableAbsPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN];
 extern UInt8 usableAbsPatternIndex_g[MAX_NUM_CELL];
UInt8 eicicFindNumOfBitsinABSPattern(InternalCellIndex internalCellIndex);

/*eMTC changes starts*/

/******************************************************************************
  Private Types
 *****************************************************************************/
#ifdef DL_UL_SPLIT
extern LTE_SEM rlcTxOppertunityIndSem_g;
#endif

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
void calculateSubFrameIndex(
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
		UInt8 *sfIndex 
/* SPR 21494 Start */
#if defined (TDD_CONFIG) 
        ,InternalCellIndex internalCellIndex
#endif
/* SPR 21494 End */
        );
        /* +- SPR 17777 */

void updateMcsAndTBSizeforAssignedRBOne(UInt8 subFrameIndex,
        DLHARQProcess *harqProcess_p,
        DLUEContext *ueContext_p);
#if (defined(TDD_CONFIG) && defined(REAL_UE_ENVIORNMENT))
/*SPR 10142 fix start*/
STATIC  void calculateMCSAndTBSizeForTDDSplSubFrameTB1( UInt8 isSplSubFrame, 
                                                           DLHARQProcess *harqProcess_p,
                                                           DLUEContext *ueContext_p);
STATIC  void calculateMCSAndTBSizeForTDDSplSubFrameTB2( UInt8 isSplSubFrame, 
                                                           DLHARQProcess *harqProcess_p,
                                                           DLUEContext *ueContext_p);
/*SPR 10142 fix end*/
#endif

/*SPR 2446 Fix Begins*/
/* SPR 4168  */
STATIC  MacRetType callMuxOrEncoderSpatialMul(
        /* SPR 4168  */
        DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        DLStrategyTxNode *dlStrategyTxNode_p,
        DLDataMsgInfo *dlMsgInfo_p,                                        
        DLHARQProcess *harqProcess_p,
        DciCCEInfo *dciCCEInfo_p,                                       
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        DLUEContext *ueContext_p,
        ResourceAllocatorOutput *resourceAllocOutput_p,
       InternalCellIndex internalCellIndex 
#ifdef TDD_CONFIG                                        
       ,UInt8 dlSubframeNum
       ,UInt32 phyDelay
       ,UInt32 sysFrame
#endif 
        );

STATIC  void processPDSCHFailure(
		DciCCEContainerInfo *dciCCEContaineInfo_p,
		DciCCEInfo *dciCCEInfo_p,
		DLStrategyTxNode *dlStrategyTxNode_p,
		DLHARQProcess *harqProcess_p,
    DLUEContext *ueContext_p
    /* SPR 2446 Fix */
#ifdef FDD_CONFIG    
    /* SPR 2320 Fix Start */
    ,UInt32 containerIndex
    /* SPR 2320 Fix End */
    /* SPR 2656 Start */
#elif TDD_CONFIG
    ,UInt8 subFrame
    /* SPR 2656 End */
#endif   
    ,InternalCellIndex internalCellIndex
    );

 /*SPR 2446 Fix Begins*/
/* SPR 4168  */ 
/* STATIC*/  MacRetType callMuxOrEncoderNonSpatialMul(
/* SPR 4168  */
        DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        DLStrategyTxNode *dlStrategyTxNode_p,
        DLDataMsgInfo *dlMsgInfo_p,                                        
        DLHARQProcess *harqProcess_p,
        DciCCEInfo *dciCCEInfo_p,                                       
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        DLUEContext *ueContext_p,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG                                        
       ,UInt8 dlSubframeNum
       ,UInt32 phyDelay
       ,UInt32 sysFrame
#endif 
        );

STATIC  void fillDCICCEInfoForReTransmission(
        DciCCEInfo *dciCCEInfo_p,                                       
        DLHARQProcess *harqProcess_p,
        DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        DLDataMsgInfo *dlMsgInfo_p,                                        
        DLUEContext *ueContext_p,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        ResourceAllocatorOutput *resourceAllocOutput_p,
        InternalCellIndex internalCellIndex
        );

/* UE_CAPAB_ADD_PARAM Start */
STATIC  void  decideRATPolicy(
        UInt32 requiredRB,
        DCIFormat dciFormat,
        TransmissionType txType,
        RATPolicy *ratPolicy_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DLHARQProcess *harqProcess_p,
        UInt8 Rat1Support);
/* UE_CAPAB_ADD_PARAM End */
/* ICIC changes start */
/* For deciding RAT policy for cell edge users */
/* UE_CAPAB_ADD_PARAM Start */
STATIC  void  decideRATPolicyCellEdge(
        UInt32 requiredRB,
        DCIFormat dciFormat,
        TransmissionType txType,
        RATPolicy *ratPolicy_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DLHARQProcess *harqProcess_p,
        UInt8 Rat1Support);
/* UE_CAPAB_ADD_PARAM End */
/* ICIC changes end */
/* QOS PDSCH Change Start */
/** SPR 13599 Changes Start **/
STATIC  void allocRBInPDSCHForData(
        UInt32 withMuxFlag,
        /* + CQI_5.0 */
        UInt32 macAdvanceFactor,
        /* - CQI_5.0 */
        DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        DciCCEContainerInfo *dciCCEContaineInfo_p,
        UInt32 countMsg,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt32 containerIndex,
        UInt32 msgIndentifier,
        UInt8 allocSPSFirst,
        /* ICIC changes start */
        UInt8 *maxUEsToBeScheduledDL_p
        /* ICIC changes end */
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
        , UInt32 phyDelay
        /* +- SPR 17777 */
        ,UInt8 dlSubframeNum
        ,UInt32 sysFrame
#endif
         ,InternalCellIndex internalCellIndex
        );
/** SPR 13599 Changes End **/
/* QOS PDSCH Change End */
/*Dynamic PDCCH + 5117 changes starts */
/* SPR 15909 fix start */
STATIC  void calculateMCSToBeUsedForTB1(DLHARQProcess *harqProcess_p,tickType_t scheduledTick,\
/* SPR 15909 fix end */
                                       UInt8 numOfOverLappingRb, DLUEContext *ueContext_p,\
                                       UInt8 *mcsToBeUsedCW1_p,\
				/* SPR 8537 Fix Start */
				       UInt8 mcsIndexToBeUsed
				/* SPR 8537 Fix End */
                       ,InternalCellIndex internalCellIndex);
/* SPR 15909 fix start */
STATIC  void calculateMCSToBeUsedForTB2(DLHARQProcess
        *harqProcess_p,tickType_t scheduledTick,\
/* SPR 15909 fix end */        
                                       UInt8 numOfOverLappingRb, DLUEContext *ueContext_p,\
                                       UInt8 *mcsToBeUsedCW2_p,\
				/* SPR 8537 Fix Start */
				       UInt8 mcsIndexToBeUsed,
				/* SPR 8537 Fix End */
                       InternalCellIndex internalCellIndex);
/*Dynamic PDCCH + 5117 changes ends */

/*START : DYN_PDCCH*/
extern UInt8 currentCFIValue_g[MAX_NUM_CELL];
/*END   : DYN_PDCCH*/

/*START_DYN_PDCCH_TDD_CHANGE*/
#ifdef FDD_CONFIG
extern UInt8 maxCCEForCFI_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE];
#else
extern UInt8 maxCCEForCFI_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE][MAX_VALUE_M_I];
#endif
/*END_DYN_PDCCH_TDD_CHANGE*/

/* SPS_CHG */
STATIC void allocSPSRBInPDSCH(
        UInt32 withMuxFlag,
        DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        DciCCEContainerInfo *dciCCEContaineInfo_p,
        UInt32 countMsg,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex
        /* + SPS_TDD */
#ifdef TDD_CONFIG
        ,UInt8 dlSubframeNum
        ,UInt32 phyDelay
        ,UInt32 sysFrame
#endif
        /* - SPS_TDD */
        );
/* SPS_CHG */
/* 4x4 DL MIMO CHG START */
extern mcsRBInfo4x4 mcsRBMapForCodeRate4x4_g[MAX_CFI_VALUE];
/* 4x4 DL MIMO CHG END */
/*Dynamic PDCCH + 5117 changes starts */
#ifdef TDD_CONFIG
/* Start of change for VARIBLE Declaration */
/** CA-TDD Changes Start **/
/* SPR 23017 Fix Start */
extern UInt8 maxMCSSpecialSFForCFI_1[MAX_NUM_CELL][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB][MAX_UE_CATEGORY][MAX_NUM_OF_LAYER];
extern UInt8 maxMCSSpecialSFForCFI_2[MAX_NUM_CELL][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB][MAX_UE_CATEGORY][MAX_NUM_OF_LAYER];
/* SPR 23017 Fix End */
/** CA-TDD Changes Stop **/
/* End of change for VARIBLE Declaration */
#endif
extern UInt32 maxNumberOfCodedBitsMap_g[MAX_SUB_FRAME][(MAX_CFI_VALUE + 1)][MAX_MOD_SCHEME][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB];
extern McsRBInfo mcsRBMapForCodeRateUeCategory_g[(MAX_CFI_VALUE + 1)];

/*UInt8 (*mcsNewTxReTxUnderUECatCodeRate_p)[MAX_UE_CATEGORY][MAX_MCS_INDEX][MAX_NUM_OF_RB] = PNULL;*/
/* 4x4 DL MIMO CHG START */
McsRBInfoForNonSpatialMultiplex (*mcsRBTBSIzeInfoForNonSpatialMultiplex4x4_p)[TB_ON_LAYER_3_OR_4][MCS_27_OR_28][MAX_NUM_OF_RB] = PNULL;
McsRBInfoForSpatialMultiplex (*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)[TB_ON_LAYER_3_OR_4][MCS_27_OR_28][MAX_MCS_INDEX][MAX_NUM_OF_RB] = PNULL;
/* 4x4 DL MIMO CHG END */
McsRBInfoForNonSpatialMultiplex (*mcsRBTBSIzeInfoForNonSpatialMultiplex_p)[MAX_UE_CATEGORY][MAX_NUM_OF_RB] = PNULL;
McsRBInfoForSpatialMultiplex (*mcsRBTBSIzeInfoForSpatialMultiplex_p)[MAX_UE_CATEGORY][MAX_MCS_INDEX][MAX_NUM_OF_RB] = PNULL;
UInt32 (*maxNumberOfCodedBitsMap_p)[MAX_MOD_SCHEME][MAX_NUM_OF_RB][MAX_NUM_OVERLAPPING_RB]= PNULL;

extern UInt8 numOvlp_g;
/*Dynamic PDCCH + 5117 changes ends */


/*Cyclomatic Complexity changes - starts here */

STATIC  MacRetType fillDciCCEStructForSpatialDataMsg(MsgType msgType, 
                                DciCCEInfo *dciCCEInfo_p ,
                                ResourceAllocatorOutput *resourceAllocOutput_p,
                                DLDataMsgInfo *dlMsgInfo_p ,
                                DLUEContext *ueContext_p ,
                                DLHARQProcess *harqProcess_p,
                                /* +- SPR 17777 */
                                /* SPR 15909 fix start */
                                tickType_t scheduledTick,
                                /* SPR 15909 fix end */
                                UInt32 withMuxFlag,
                                MultiplexerQueueNode *muxQueueNode_p,
                                InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG                                        
                                ,UInt8 dlSubframeNum
                                ,UInt32 phyDelay
                                ,UInt32 sysFrame
#endif 
                                );
/* SPR 15909 fix start */
 void updateRaPolicyInfo(tickType_t nextTick,InternalCellIndex internalCellIndex);
STATIC  void updateMcsRBTBSIzeInfoAndMaxNumberOfCodedBitsMap(UInt32 delayToPhy,
        InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */
STATIC  void updateTxMode(TransmissonMode *txMode,InternalCellIndex internalCellIndex);
STATIC  MacRetType processBcchMsg(DLMsgTypeMsgInfo **dlMsgTypeMsgInfo_p,
                                        DciCCEContainerInfo *dciCCEContainerInfo_p,
                                        DCICCEInfoCmnChannel **dciCCEInfoCmnChannel_p,
                                        TransmissonMode txMode,
                                        UInt32 *msgIndentifier,
    /*Coverity_31956 Fix Start*/
                                        DciCCEInfo **dciCCEInfo_p,
    /*Coverity_31956 Fix End*/
                                        InternalCellIndex internalCellIndex);
STATIC  MacRetType processPcchMsg(DLMsgTypeMsgInfo **dlMsgTypeMsgInfo_p,
                                        DciCCEContainerInfo *dciCCEContainerInfo_p,
                                        DCICCEInfoCmnChannel **dciCCEInfoCmnChannel_p,
                                        TransmissonMode txMode,
                                        UInt32 *msgIndentifier,
    /*Coverity_31956 Fix Start*/
                                        DciCCEInfo **dciCCEInfo_p,
        /* + SPR_17858_409_CHANGES */
		tickType_t nextTick,
        /* - SPR_17858_409_CHANGES */
                                        InternalCellIndex internalCellIndex); 
    /*Coverity_31956 Fix End*/

STATIC  MacRetType processRarMsg(DLMsgTypeMsgInfo **dlMsgTypeMsgInfo_p,
                                        DciCCEContainerInfo *dciCCEContainerInfo_p,
                                        DCICCEInfoCmnChannel **dciCCEInfoCmnChannel_p,
                                        TransmissonMode txMode,
                                        UInt32 *msgIndentifier,
    /*Coverity_31956 Fix Start*/
                                        DciCCEInfo **dciCCEInfo_p,
                                        InternalCellIndex internalCellIndex);
    /*Coverity_31956 Fix End*/
/* SPR 15909 fix start */
/* SPR 21631 Start */
STATIC  void updateRaPolicyInfoAgain(tickType_t nextTick 
/* SPR 15909 fix end */        
                                ,InternalCellIndex internalCellIndex);
/* SPR 21631 End */
STATIC  MacRetType processCCCHMsg(DLMsgTypeMsgInfo *dlCCCHMsgTypeMsgInfo_p,
                                        DciCCEContainerInfo *dciCCEContainerInfo_p,
                                        DCICCEInfoCmnChannel **dciCCEInfoCmnChannel_p,
                                        UInt32 *msgIndentifier,
                                        /* +- SPR 17777 */
                                        TransmissonMode txMode
                                        /* SPR 15909 fix start */
                                        /* +- SPR 17777 */
                                        #ifdef FDD_CONFIG
                                        ,UInt32 delayToPhy,
                                        tickType_t nextTick
                                        /* SPR 15909 fix end */
                                        /* +- SPR 17777 */
                                        ,UInt32 subFrame
                                        #endif
                                        ,InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
                                        /* +- SPR 17777 */
                                        ,tickType_t currentGlobalTick
                                        ,UInt8 dlSubframeNum,
                                        UInt32 sysFrame
#endif
                                        );
STATIC  MacRetType processContentionResolutionMsg(DLMsgTypeMsgInfo *dlCCCHMsgTypeMsgInfo_p,
                                                        DciCCEContainerInfo *dciCCEContainerInfo_p,
                                                        DCICCEInfoCmnChannel **dciCCEInfoCmnChannel_p,
                                                        UInt32 *msgIndentifier,
                                                        TransmissonMode txMode,
                                                        InternalCellIndex internalCellIndex
                                                              /*Spr 17733 Merge Fix Start */
#if (defined(FDD_CONFIG) && !defined(DL_UL_SPLIT)) /*SPR 19078 +-*/
                                                        ,UInt32 delayToPhy
#endif
                                                              /*Spr 17733 Merge Fix End */
                                                        );
STATIC  MacRetType processDlCCCHMsg(DLMsgTypeMsgInfo *dlCCCHMsgTypeMsgInfo_p,
                                    DciCCEContainerInfo *dciCCEContainerInfo_p,
                                    DCICCEInfoCmnChannel **dciCCEInfoCmnChannel_p,
                                    UInt32 *msgIndentifier,
                                    /* +- SPR 17777 */
                                    TransmissonMode txMode
                                    /* SPR 15909 fix start */
                                    /* +- SPR 17777 */
                                    #ifdef FDD_CONFIG
                                    ,UInt32 delayToPhy,
                                    tickType_t nextTick
                                    /* SPR 15909 fix end */
                                    /* +- SPR 17777 */
                                    ,UInt32 subFrame
                                    #endif
                                    ,InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
                                    /* +- SPR 17777 */
                                    ,tickType_t currentGlobalTick
                                    ,UInt8 dlSubframeNum,
                                    UInt32 sysFrame
#endif
);

STATIC MacRetType  fillHarqInfoForReTransmission(DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        DLStrategyTxNode *dlStrategyTxNode_p,
        DLDataMsgInfo *dlMsgInfo_p,
        DLHARQProcess *harqProcess_p,
        DciCCEInfo *dciCCEInfo_p,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        DLUEContext *ueContext_p,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
        ,UInt8 dlSubframeNum
        ,UInt32 phyDelay
        ,UInt32 sysFrame
#endif
        );


/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* ICIC changes start */
extern BufferPool dlBufferPool_g[MAX_NUM_CELL];
/* ICIC changes end */

extern LTE_SQUEUE* pendingCcchQueue_gp[MAX_NUM_CELL];
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/********************************************************************************************
 * Function Name    : allocPdschForDL
 * Inputs           : withMuxFlag - This when set to one Indicates that we need to call
 *                                  directly invoke Mux Execution Logic,
 *                    delayToPhy - This stores delay from current module to PHY,
 *                    currentGlobalTick - This stores current Global Tick,
 *                    pdcchDLOutputInfo_p - This stores the pointer to pdcchDLOutputInfo_p
 *                                      which stores the information of those DL Msg for
 *                                      which PDCCH Region has been allocated.
 *                                      Like Message: BCCH, RACH, PCCH, DATA etc,
 *                    containerIndex - This stores the container index which will be used
 *                                     to calculate the container of DCICCEInfo structure
 *                                     and also be passed to encoder for getting the
 *                                     container index on which it need to write relevant 
 *                                     information,
 *                    allocSPSFirst ,
 *                    subFrame - Current Subframe,
 *                    dlSubframeNum - Subframe number w.r.t. delay
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Description      : This funtion will allocate the PDSCH for Control Messages and
 *                    also populate the DCI CCE Info structure.This will also call
 *                    funtion allocPDSCHForDataMsg which will allocate PDSCH for UE.
 *******************************************************************************************/
void allocPdschForDL(
        UInt32 withMuxFlag,
        UInt32 delayToPhy, 
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick ,
        /* SPR 15909 fix end */
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        UInt32 containerIndex,
        UInt8 allocSPSFirst   
#ifdef FDD_CONFIG
        ,UInt32 subFrame
        
#endif
        ,InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
        , UInt8 dlSubframeNum,
        UInt32 sysFrame
#endif
        )
{
    DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p = PNULL; 
    DciCCEInfo *dciCCEInfo_p = PNULL;    
    DCICCEInfoCmnChannel *dciCCEInfoCmnChannel_p = PNULL;
    DciCCEContainerInfo *dciCCEContainerInfo_p = PNULL;
    UInt32 countCommonCntrlMsg = 0;    
    TransmissonMode    txMode = INVALID_TX_MODE;
    UInt32 aggregationLvl = 0;
    UInt32 msgIndentifier = 0;
   /* SPR 3306 Fix start */ 
    UInt8 nonCommonCntrlMsgIndex = 0;
    UInt8 dlCCCHMsgIndex[MAX_DL_UE_SCHEDULED] = {0}; 
    DLMsgTypeMsgInfo *dlCCCHMsgTypeMsgInfo_p = PNULL; 
    UInt32 countNonCommonCntrlMsg = 0;
    /* SPR 15909 fix start */
    tickType_t nextTick = delayToPhy + currentGlobalTick;
    /* SPR 15909 fix end */
   /* SPR 3306 Fix End */ 

    UInt8 numBitsAbsPattern = 0;

    /* ICIC changes start */
    MultiplexerQueueNode *muxQueueNode_p = PNULL;
    /* Type for RAT 2 allocation
     * 0 Indicates RAT 2 Localized
     * 1 Indicates RAT 2 Distributed 
     */
    SInt8 tempExcessPoolRBs = 0;
    UInt8 maxUEsToBeScheduledDL = (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
        dlSchdConfig.maxUEsToBeScheduledDL) 
                                  + numDlSpsUeInTti_g[internalCellIndex];
    /* ICIC changes end */
    /* + PRS_CHANGES */
    PRSConfigInfo *prsConfigInfo_p = PNULL;

    UInt32 sysFrameNum 			= 0;
    UInt32 subFrameNum 			= 0;
    UInt8  absSubFrameNum 		= 0;

    getSFAndSFN(&sysFrameNum, &subFrameNum,internalCellIndex);

    /*Calculate the SFN and SF by considering the delay to PHY*/
    subFrameNum = subFrameNum + delayToPhy;
    
    #ifdef TDD_CONFIG
    if (subFrameNum > dlSubframeNum)
    {
        sysFrameNum++;
    } 
    /*+ Coverity fix 71109*/
    subFrameNum++;
    /*- Coverity fix 71109*/
    #endif
    if(subFrameNum >= MAX_SUB_FRAME)
    {
        subFrameNum -= MAX_SUB_FRAME;
        sysFrameNum++;
        if (sysFrameNum == MAX_SFN_VALUE)
        {
            sysFrameNum = 0;
        }
    }
    /*Complexity Start*/
    numBitsAbsPattern=eicicFindNumOfBitsinABSPattern(internalCellIndex);
    /*Complexity End*/

    absSubFrameNum = ((((sysFrameNum )*MAX_SUB_FRAME) +(subFrameNum)) %numBitsAbsPattern) ;



    /* - PRS_CHANGES */
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR
            ,"Entering function = %s  \n", __func__);
    /* SPR 889 Fix start */ 
    /* SPR 3306 Fix start */ 
    /* It will check for SF 0 & 5 */ 
#ifdef PERF_STATS
    /** CA-TDD Changes Start **/
    perfStatsMibSyncSignalFlag_g[internalCellIndex] = PERF_STATS_NO_OVERLAP;
        if(!(nextTick%5))
    {
	    perfStatsMibSyncSignalFlag_g[internalCellIndex] = PERF_STATS_ONLY_SS;
    }
    /** CA-TDD Changes Stop **/
#endif
    /* ICIC changes start */
        /*+ SPR 22895 */
        /* SPR 21631 Start */
    updateRaPolicyInfo(nextTick,internalCellIndex);
        /* SPR 21631 End */
        /*- SPR 22895 */

    /* + PRS_CHANGES */
     prsConfigInfo_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
             prsConfigInfo);
     UInt8 dlPRSRATPoliciesIndex = prsConfigInfo_p->dlPRSRATPoliciesIndex;

#ifdef TDD_CONFIG
    /* SPR 23393 Changes Start */
    UInt8 isSpecialSubframe = checkForSpecialSubframe((dlSubframeNum + delayToPhy)% MAX_SUB_FRAME, internalCellIndex);
    /* SPR 23393 Changes End */
#endif          
     /* To save overhead a new PRS RAT poicy structure is used in PRS occasion.
      * if it is not special SF (in TDD only) and it is PRS occasion 
      * with both PRS and system bandwidth not equal then assign PRS reserved  
      * structure in raPolicyInfo.
      */  
    if (
#ifdef TDD_CONFIG
        !isSpecialSubframe &&
#endif
        (prsConfigInfo_p->isPRSConfigured) && 
        (prsConfigInfo_p->prsBandwidth != cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
         cellParams_p->dlBandwidth) &&
        (prsConfigInfo_p->prsSubframeCounter))
	{       
		raPolicyInfo_g[internalCellIndex]  = (dlRAInfo_gp[internalCellIndex]\
                    + dlPRSRATPoliciesIndex)->raPolicyInfoPRSReserved;
		allocSPSFirst = FALSE;
	}
    /* - PRS_CHANGES */

#ifdef PERF_STATS
	/* + PERF_CA */
    gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedDLPerfStats.totalDLPRBAvailable += \
	/* - PERF_CA */
                         raPolicyInfo_g[internalCellIndex].rat2LocalizedInfo.numOfVRBAvailable;
#endif
   /* ICIC changes end */
   /* SPR 3306 Fix End */
    dciCCEContainerInfo_p = (dciCCEContainerInfo_gp[internalCellIndex] + containerIndex);
    dciCCEContainerInfo_p->countCmnChannelMsg = 0;
    /*Cyclomatic Complexity changes - starts here */
    updateTxMode(&txMode,internalCellIndex);
    /* SPR 21631 Start */
    updateMcsRBTBSIzeInfoAndMaxNumberOfCodedBitsMap(delayToPhy,internalCellIndex);
    /* SPR 21631 End */
    /*Cyclomatic Complexity changes - ends here*/

    /*Initializing the countCmnChnlmsg to value such that it contain count of
      Control Messages for which PDSCH  region need to be  allocated */
    countCommonCntrlMsg =  pdcchDLOutputInfo_p->countCommonChannelMsg;
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "countCommonCntrlMsg = %u  \n",countCommonCntrlMsg);
    /* SPS_CHG */
    if(allocSPSFirst)
    {

        dlMsgTypeMsgInfo_p = 
                &pdcchDLOutputInfo_p->dlMsgTypeMsgInfo[CC_USER][0];

        allocSPSRBInPDSCH(
            withMuxFlag,
            dlMsgTypeMsgInfo_p,
            dciCCEContainerInfo_p,
            pdcchDLOutputInfo_p->countMsg[CC_USER],
            &raPolicyInfo_g[internalCellIndex],
            (delayToPhy + currentGlobalTick),
            /* +- SPR 17777 */
            internalCellIndex
            /* + SPS_TDD */
#ifdef TDD_CONFIG
            ,dlSubframeNum
            ,delayToPhy
            ,sysFrame
#endif
            /* - SPS_TDD */
            );
    }
    /* SPS_CHG */
    /*Initializing the dlMsgTypeMsgInfo_p pointer with the address of the First
      Msg for which PDSCH Region has to be allocated*/
    /* ICIC changes start */
    dlMsgTypeMsgInfo_p = &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[0];
    /* ICIC changes end */

    /*Initializing the dciCCEInfoCmnChannel_p to addrss of DCICCEInfoCmnChannel
      structure on which once PDSCH is allocated for Message ,relevant
      can be written */
    dciCCEInfoCmnChannel_p = &dciCCEContainerInfo_p->dciCCEInfoCmnChannel[0];

    /*Below While is trying to allocate the PDSCH Region to Common Channel
      Message and if it is successful,it is populating relevant information to 
      dciCCEInfo_p structure*/
    while ( countCommonCntrlMsg-- )
    {
        dciCCEInfo_p = PNULL;
        switch ( dlMsgTypeMsgInfo_p->msgType )
        {
            case BCCH_MSG:
                {   
                    /*Cyclomatic Complexity changes - starts here */
                    if (MAC_SUCCESS == processBcchMsg(&dlMsgTypeMsgInfo_p,
                                                      dciCCEContainerInfo_p,
                                                      &dciCCEInfoCmnChannel_p,
                                                      txMode,
                                                      &msgIndentifier,
    /*Coverity_31956 Fix Start*/
                                                      &dciCCEInfo_p,
    /*Coverity_31956 Fix End*/
                                                      internalCellIndex))
                    {
                        continue;
                    } 
                    /*Cyclomatic Complexity changes - ends here*/
                }
                break;         
            case PCCH_MSG :
                {
                    /*Cyclomatic Complexity changes - starts here */
                    if (MAC_SUCCESS == processPcchMsg(&dlMsgTypeMsgInfo_p,
                                   dciCCEContainerInfo_p,
                                   &dciCCEInfoCmnChannel_p,
                                   txMode,
                                   &msgIndentifier,
    /*Coverity_31956 Fix Start*/
                                   &dciCCEInfo_p,
        /* + SPR_17858_409_CHANGES */
								nextTick,
        /* - SPR_17858_409_CHANGES */
                                   internalCellIndex))
    /*Coverity_31956 Fix End*/
                    {
                        continue;
                    } 
                    /*Cyclomatic Complexity changes - ends here*/
                }
                break;        
            case RAR_MSG :
                {
                    /*Cyclomatic Complexity changes - starts here */
                    if (MAC_SUCCESS == processRarMsg(&dlMsgTypeMsgInfo_p,
                                dciCCEContainerInfo_p,
                                &dciCCEInfoCmnChannel_p,
                                txMode,
                                &msgIndentifier,
                                /*Coverity_31956 Fix Start*/
                                &dciCCEInfo_p,
                                internalCellIndex))
                        /*Coverity_31956 Fix End*/
                    {
                        continue;
                    } 
                    /*Cyclomatic Complexity changes - ends here*/
                }
                break;
            /* SPR 3306 Fix Starts */
            case  CCCH_MSG :
                {
                    dlCCCHMsgIndex[countNonCommonCntrlMsg] =  
                    (UInt8)(dlMsgTypeMsgInfo_p - &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[0]);
                    countNonCommonCntrlMsg++;
                    dlMsgTypeMsgInfo_p++;
                }
                break;
            case  CONTENTION_RESL_MSG:
                {
                    dlCCCHMsgIndex[countNonCommonCntrlMsg] = 
                    (UInt8)(dlMsgTypeMsgInfo_p - &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[0]);
                    countNonCommonCntrlMsg++;
                    dlMsgTypeMsgInfo_p++;
                }
                break;
            default:
                {
                    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Not a \
                            Common Channel Msg\n");
                    continue;
                }
        }
        if(PNULL != dciCCEInfo_p)
        {
            dlMsgTypeMsgInfo_p++;
            aggregationLvl = dciCCEInfo_p->dciPduInfo.aggregationLvl;           
            mac_reset_cce(aggregationLvl, dciCCEInfo_p, internalCellIndex);     
            LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Res ource Allocation  Failed \
                    So resetting the CCEs\n");
        }
    }
    
        /* SPR 21631 Start */
    updateRaPolicyInfoAgain(nextTick,internalCellIndex);
        /* SPR 21631 End */
    while ( nonCommonCntrlMsgIndex < countNonCommonCntrlMsg )
    {
        dlCCCHMsgTypeMsgInfo_p = &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[
                                dlCCCHMsgIndex[nonCommonCntrlMsgIndex]];
        nonCommonCntrlMsgIndex++;
        if(MAC_SUCCESS == processDlCCCHMsg(dlCCCHMsgTypeMsgInfo_p,
                                          dciCCEContainerInfo_p,
                                          &dciCCEInfoCmnChannel_p,
                                          &msgIndentifier,
                                          txMode
                                          #ifdef FDD_CONFIG
                                          ,delayToPhy,
                                          nextTick
                                          ,subFrame
                                          #endif
                                          ,internalCellIndex
#ifdef TDD_CONFIG
                                          ,currentGlobalTick,
                                          dlSubframeNum,
                                          sysFrame
#endif
                                          ))
        {
            continue;
        }
        else
        {
            /* + SPR 22279 fix */
            LOG_MAC_ERROR(MAC_PDSCH,"PDSCH Allocation fails for CCCH Message");
            continue;
            /* - SPR 22279 fix */
        }
    }
    /* SPR 3306 Fix Ends */

    /* Store Cell edge excess Pool RBs temporarily, this will be useful to update
     * cell centreRBs after allocation of cell edge users
     */
    tempExcessPoolRBs = excessPoolRb_g[internalCellIndex][CE_REGION];
    
    /* Calling allocRBInPDSCHForData in following order
     * 1. ReTx Cell Edge 
     * 2. New Tx Cell Edge 
     * 3. ReTx Cell Center 
     * 4. Ner Tx Cell Center */
    if (pdcchDLOutputInfo_p->countPriorityMsg[CE_USER])
    {
        allocRBInPDSCHForData(
                withMuxFlag,
                /* + CQI_5.0 */
                delayToPhy,
                /* - CQI_5.0 */
                &pdcchDLOutputInfo_p->dlMsgTypeMsgInfo[CE_USER][0],
                dciCCEContainerInfo_p,
                pdcchDLOutputInfo_p->countPriorityMsg[CE_USER],
                &raPolicyInfo_g[internalCellIndex],
                (delayToPhy + currentGlobalTick),
                containerIndex,
                msgIndentifier,
                allocSPSFirst,
                &maxUEsToBeScheduledDL
                /* +- SPR 17777 */
#ifdef TDD_CONFIG
                , delayToPhy
                /* +- SPR 17777 */
                , dlSubframeNum
                ,sysFrame
#endif
                 ,internalCellIndex
                );
    }
    /* SPR 23393 Changes Start */
    if (maxUEsToBeScheduledDL && (pdcchDLOutputInfo_p->countMsg[CE_USER] - 
            pdcchDLOutputInfo_p->countPriorityMsg[CE_USER]))
    /* SPR 23393 Changes End */
    {
        /* If No cell centre UE is scheduled in current tick, set 
         * lastUEDetectFlag_g[internalCellIndex] to find last UE sceduled at PDSCH.
         */
        if (!(pdcchDLOutputInfo_p->countMsg[CC_USER] -
                    pdcchDLOutputInfo_p->countPriorityMsg[CC_USER]))
        {
            lastUEDetectFlag_g[internalCellIndex] = TRUE;            
        }
        allocRBInPDSCHForData(
                withMuxFlag,
                /* + CQI_5.0 */
                delayToPhy,
                /* - CQI_5.0 */
                &pdcchDLOutputInfo_p->dlMsgTypeMsgInfo[CE_USER][pdcchDLOutputInfo_p->countPriorityMsg[CE_USER]],
                dciCCEContainerInfo_p,
                (pdcchDLOutputInfo_p->countMsg[CE_USER] - pdcchDLOutputInfo_p->countPriorityMsg[CE_USER]),
                &raPolicyInfo_g[internalCellIndex],
                (delayToPhy + currentGlobalTick),
                containerIndex,
                msgIndentifier,
                allocSPSFirst,
                &maxUEsToBeScheduledDL
                /* +- SPR 17777 */
#ifdef TDD_CONFIG
                , delayToPhy
                /* +- SPR 17777 */
                , dlSubframeNum
                ,sysFrame
#endif
                 ,internalCellIndex
                );
    }
    /* This will update excess Pool RBs for cell centre region, i.e. impact due
     * to cell edge allocation
     */
    UPDATE_CELL_CENTRE_EXCESS_POOL(tempExcessPoolRBs,internalCellIndex);    
    
            /* SPR 23393 Changes Start */
    if (maxUEsToBeScheduledDL && pdcchDLOutputInfo_p->countPriorityMsg[CC_USER])
            /* SPR 23393 Changes End */
    {
        allocRBInPDSCHForData(
                withMuxFlag,
                /* + CQI_5.0 */
                delayToPhy,
                /* - CQI_5.0 */
                &pdcchDLOutputInfo_p->dlMsgTypeMsgInfo[CC_USER][0],
                dciCCEContainerInfo_p,
                pdcchDLOutputInfo_p->countPriorityMsg[CC_USER],
                &raPolicyInfo_g[internalCellIndex],
                (delayToPhy + currentGlobalTick),
                containerIndex,
                msgIndentifier,
                allocSPSFirst,
                &maxUEsToBeScheduledDL
                /* +- SPR 17777 */
#ifdef TDD_CONFIG
                , delayToPhy
                /* +- SPR 17777 */
                , dlSubframeNum
                ,sysFrame
#endif 
                 , internalCellIndex
                );
    }
    /* SPR 23393 Changes Start */
    if (maxUEsToBeScheduledDL &&(pdcchDLOutputInfo_p->countMsg[CC_USER] - 
            pdcchDLOutputInfo_p->countPriorityMsg[CC_USER]))
            /* SPR 23393 Changes End */
    {
        /* set lastUEDetectFlag_g[internalCellIndex] to find last UE scheduled at PDSCH in 
         * called function
         */
        lastUEDetectFlag_g[internalCellIndex] = TRUE;            
        allocRBInPDSCHForData(
                withMuxFlag,
                /* + CQI_5.0 */
                delayToPhy,
                /* - CQI_5.0 */
                &pdcchDLOutputInfo_p->dlMsgTypeMsgInfo[CC_USER][pdcchDLOutputInfo_p->countPriorityMsg[CC_USER]],
                dciCCEContainerInfo_p,
                (pdcchDLOutputInfo_p->countMsg[CC_USER] - pdcchDLOutputInfo_p->countPriorityMsg[CC_USER]),
                &raPolicyInfo_g[internalCellIndex],
                (delayToPhy + currentGlobalTick),
                containerIndex,
                msgIndentifier,
                allocSPSFirst,
                &maxUEsToBeScheduledDL
                /* +- SPR 17777 */
#ifdef TDD_CONFIG
                , delayToPhy
                /* +- SPR 17777 */
                , dlSubframeNum
                ,sysFrame
#endif
                ,internalCellIndex
                );
    }

/* +DYNAMIC_ICIC */
    if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                icicReportConfigParams.cellEdgePrbUsageRequired)
    {
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.dlICICReportInfo_p \
            [rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndex].dlCEPrbUsage += 
            ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalCellEdgeRB - 
             raPolicyInfo_g[internalCellIndex].rat2LocalizedInfo.numOfCellEdgeVRBAvailable);

        if(LTE_FALSE!=rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportConfigParams.absUsageRequired)
        {
            if (LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) /* Work as Victim*/
            {
                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p \
	 	[rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndexEicic].cePRBUsage +=\
               ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalCellEdgeRB -\
                       raPolicyInfo_g[internalCellIndex].rat2LocalizedInfo.numOfCellEdgeVRBAvailable);

                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p \
	 	[rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndexEicic].totalCEPrb +=\
	 	(dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalCellEdgeRB;

                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p \
	 	[rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndexEicic].ccPRBUsage +=\
               ( (((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB) -
               ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalCellEdgeRB)) - 
               ( (raPolicyInfo_g[internalCellIndex].rat2LocalizedInfo.numOfVRBAvailable) - (raPolicyInfo_g[internalCellIndex].rat2LocalizedInfo.numOfCellEdgeVRBAvailable)));

              rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p \
		 	[rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndexEicic].totalCCPrb +=\
	 	(((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB) -
              ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalCellEdgeRB));

          }
        }
	 
        if (LTE_TRUE == absPattern_g[internalCellIndex][absPatternIndex_g[internalCellIndex]][absSubFrameNum]) /* Work as aggressor*/
        {
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.absCountInfo_p \
            [rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndex].absSubframeCount ++;
        }
    }
/* -DYNAMIC_ICIC */
/* + SPR 11209 Changes */
    if(!IS_CA_ENABLED())
    {
            /** CC OPT Changes Start **/
        if (withMuxFlag )
        {
            processDummyNode (delayToPhy + currentGlobalTick, internalCellIndex,internalCellIndex );
        }
        else
        {
            /* Preparing dummy Node SO that Encoder knows that this is last Node*/
            GET_MEM_FROM_POOL(MultiplexerQueueNode , muxQueueNode_p , 
                          sizeof(MultiplexerQueueNode) , PNULL);
            /* coverity-530 CID 24497 */
            if(PNULL == muxQueueNode_p)
            {
                return;
            }
            /* coverity-530 CID 24497 */
            muxQueueNode_p->msgType = DUMMY_MSG;  
            /** CC OPT Changes End **/
            putEntryInMultiplexerQueue(muxQueueNode_p,internalCellIndex );
        }
    }
/* - SPR 11209 Changes */

    /* ICIC changes end */
    
    /* QOS PDSCH Change Start */

    /* We now encode the tpc bitmap for the DCI format 3/3A 
     * for PUCCH power control */
    UInt32 count = pdcchDLOutputInfo_p->countTpcMsg;
    TpcMsgInfoForPucch * dlTpcMsgInfo_p = pdcchDLOutputInfo_p->tpcMsgInfoForPucch;
    encodeTpcBitmapForPucch(count, dlTpcMsgInfo_p, dciCCEContainerInfo_p
#ifdef FDD_CONFIG
            ,delayToPhy+currentGlobalTick
#elif TDD_CONFIG
            ,currentGlobalTick
#endif       
            ,internalCellIndex
            );

#ifdef DL_UL_SPLIT
    semPost(&rlcTxOppertunityIndSem_g);
#endif
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR, "leaving function = %s  \n" \
            ,__func__);

}
/*Cyclomatic Complexity changes - starts here */
/******************************************************************************
 * FUNCTION NAME : updateRaPolicyInfo
 * DESCRIPTION   : This function will update the globally defined rat policy info.
 * INPUT         : nextTick
 *                 internalCellIndex - Cell-Index used at MAC
 * OUTPUTS       : None
 ******************************************************************************/ 
/* CA Changes start */
/* SPR 15909 fix start */
/* SPR 21631 Start */
 void updateRaPolicyInfo(tickType_t nextTick,
/* SPR 15909 fix end */        
                                      InternalCellIndex internalCellIndex)
/* SPR 21631 End */
{

    InitParams	*initParams_p = cellSpecificParams_g.\
               cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    /* SPR 21631 Start */
        if (nextTick % 10)
            /* SPR 21631 End */
        {
            raPolicyInfo_g[internalCellIndex] = (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo;        
        }
    else
    {
        if (initParams_p->commChWithMIBAllowed)
        {
            raPolicyInfo_g[internalCellIndex] = (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo;
        }
        else if (initParams_p->dlSchWithMIBAllowed)
        {
            raPolicyInfo_g[internalCellIndex] = (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfoMIBReserved;
#ifdef PERF_STATS
        /** CA-TDD Changes Start **/
	    perfStatsMibSyncSignalFlag_g[internalCellIndex] = PERF_STATS_MIB_SS;
        /** CA-TDD Changes Stop **/
#endif
        }
        else
        {
            raPolicyInfo_g[internalCellIndex] = (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfoMIBReserved;
        }
    }
}
/******************************************************************************
 * Function Name    : updateTxMode 
 * Inputs           : txMode(this parameter gets updated)
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None 
 * Returns          : None
 * Description      : update the number of transmitting antenna.
 ******************************************************************************/ 
STATIC  void updateTxMode(TransmissonMode *txMode,
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    /*4x2 MIMO S*/
    if ((NUM_OF_TX_ANTENNAS_2 == cellParams_p->numOfTxAnteenas )||
        (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas ))
    {
        *txMode = TX_MODE_2;
    }
    /*4x2 MIMO E*/
    else if( NUM_OF_TX_ANTENNAS_1 == cellParams_p->numOfTxAnteenas  )
    {
        *txMode = TX_MODE_1;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_CRITICAL,PDSCH_MGR,
                "Antenna Port is not configured ");        
    }
}
/****************************************************************************************
 * Function Name    : updateMcsRBTBSIzeInfoAndMaxNumberOfCodedBitsMap
 * Inputs           : delayToPhy
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function will update mcsRBTBSIzeInfoForNonSpatialMultiplex_p,
 *                    mcsRBTBSIzeInfoForSpatialMultiplex_p and maxNumberOfCodedBitsMap_p.
 *****************************************************************************************/ 
/* SPR 21631 Start */
STATIC  void updateMcsRBTBSIzeInfoAndMaxNumberOfCodedBitsMap(UInt32 delayToPhy,
        InternalCellIndex internalCellIndex)
/* SPR 21631 End */
{
    /*Dynamic PDCCH + 5117 changes starts */
    UInt8 subFrameIndex = 0;
  
/* SPR 21631 Start */
    if (0 == ((getSubFrameNumber(internalCellIndex)+ delayToPhy) % MAX_SUB_FRAME))
    {
        subFrameIndex = 0;
    }
    else if(5 == (getSubFrameNumber(internalCellIndex) + delayToPhy))
    {
        subFrameIndex = 1;
    }
#ifdef TDD_CONFIG
    /* SPR 23393 Changes Start */
    else if (1 == (getSubFrameNumber(internalCellIndex) + delayToPhy) )
    /* SPR 23393 Changes End */
    {
	subFrameIndex = 3;
    }
    /* SPR 23393 Changes Start */
    else if (6 == (getSubFrameNumber(internalCellIndex) + delayToPhy))
    /* SPR 23393 Changes End */
    {
	     if((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
              cellParams_p->subFrameAssign <= UL_DL_CONFIG_2) ||
                (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
                [internalCellIndex]->cellParams_p->subFrameAssign))
	     {
		subFrameIndex = 3;
	     }
	     else
	     {
		subFrameIndex = 2;
	     }
    }
#endif
    /* SPR 21631 End */

    else
    {
        subFrameIndex = 2;
    }
    /* + SPR 8649 Fix */
    /* SPR 11788 Start: Out of bound array access fixed here */
    mcsRBTBSIzeInfoForNonSpatialMultiplex_p =
        &(mcsRBMapForCodeRateUeCategory_g[currentCFIValue_g[internalCellIndex] - 1].mcsRBTBSIzeInfoForNonSpatialMultiplex);

    mcsRBTBSIzeInfoForSpatialMultiplex_p =
        &(mcsRBMapForCodeRateUeCategory_g[currentCFIValue_g[internalCellIndex] - 1].mcsRBTBSIzeInfoForSpatialMultiplex);
    /* SPR 11788 End: Out of bound array access fixed here */
    /* 4x4 DL MIMO CHG START */
    mcsRBTBSIzeInfoForNonSpatialMultiplex4x4_p =
        &(mcsRBMapForCodeRate4x4_g[(currentCFIValue_g[internalCellIndex] - 1)].mcsRBTBSIzeInfoForNonSpatialMultiplex);

    mcsRBTBSIzeInfoForSpatialMultiplex4x4_p =
        &(mcsRBMapForCodeRate4x4_g[(currentCFIValue_g[internalCellIndex] - 1)].mcsRBTBSIzeInfoForSpatialMultiplex);
    /* 4x4 DL MIMO CHG END */

    maxNumberOfCodedBitsMap_p =
        &(maxNumberOfCodedBitsMap_g[subFrameIndex][currentCFIValue_g[internalCellIndex]]);
    /*Dynamic PDCCH + 5117 changes ends */
}
/****************************************************************************************
 * Function Name    : processBcchMsg
 * Inputs           : dlMsgTypeMsgInfo_p - Pointer to DLMsgTypeMsgInfo,
 *                    dciCCEContainerInfo_p - Pointer to DciCCEContainerInfo,
 *                    dciCCEInfoCmnChannel_p - Pointer to DCICCEInfoCmnChannel
 *                    txMode - Mode of Transmission,
 *                    msgIndentifier - Message identifier,
 *                    dciCCEInfo_pp - Pointer to DciCCEInfo
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will process BCCH_MSG.
 *****************************************************************************************/ 
STATIC  MacRetType processBcchMsg(DLMsgTypeMsgInfo **dlMsgTypeMsgInfo_p,
                                        DciCCEContainerInfo *dciCCEContainerInfo_p,
                                        DCICCEInfoCmnChannel **dciCCEInfoCmnChannel_p,
                                        TransmissonMode txMode,
                                        UInt32 *msgIndentifier,
    /*Coverity_31956 Fix Start*/
                                        DciCCEInfo **dciCCEInfo_pp,
    /*Coverity_31956 Fix End*/
                                        InternalCellIndex internalCellIndex)
{
    ResourceAllocatorInput *resourceAllocInput_p = PNULL;
    ResourceAllocatorOutput resourceAllocOutput ;            
    /* SPR 12299 changes start */
    UInt8 virtulResrcBlkType = 0;
    /* SPR 12299 changes end */
    MacRetType retType = MAC_SUCCESS;
    /*Coverity_31956 Fix Start*/
    DciCCEInfo *dciCCEInfo_p = PNULL;
    /*Coverity_31956 Fix End*/

    DLBCCHMsgInfo *dlBCCHMsgInfo_p = PNULL;
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                   " MsgType is %u\n",(*dlMsgTypeMsgInfo_p)->msgType);
    dlBCCHMsgInfo_p = &(*dlMsgTypeMsgInfo_p)->dlMsgInfo.
                      dlBCCHMsgInfo;

    /*Coverity_31956 Fix Start*/
    *dciCCEInfo_pp = dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[
                   dlBCCHMsgInfo_p->cceIndex];
    /*Coverity_31956 Fix End*/

    resourceAllocInput_p = &dlBCCHMsgInfo_p->resourceAllocInput;
                    
    /*Coverity_31967 Fix Start*/
    resourceAllocOutput.nGAP = 0;
    /*Coverity_31967 Fix End*/

    /* SPR 1571 changes start */
    /* ICIC changes start */
    /* SPR 20073 fix start */
    if(RAT_DISTRIBUTED == macParams_g.siSchedulingRATPolicy ) 
    {   
    /* ICIC changes end */
        /* SPR 20073 fix end */
        retType = allocRBCommonChannelRAT2Distributed(
                                resourceAllocInput_p,
                                &resourceAllocOutput,
                                &raPolicyInfo_g[internalCellIndex],dlBCCHMsgInfo_p->dciFormat,
                                internalCellIndex);
                        
        /* ICIC changes start */
        virtulResrcBlkType = 1;

        if ( MAC_SUCCESS != retType)
        {
            /* If allocattion failed for RAT2 distributed, 
             * try to allocate resources using RAT2 localized 
             */
            retType = allocRBCommonChannelRAT2Localized(
                                    resourceAllocInput_p,
                                    &resourceAllocOutput,
                                    &raPolicyInfo_g[internalCellIndex],dlBCCHMsgInfo_p->dciFormat,
                                    internalCellIndex);
            virtulResrcBlkType = 0;
        }

        if (MAC_SUCCESS == retType)
        /* ICIC changes end */
        {
#ifdef PERF_STATS
			/* + PERF_CA */
            gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedDLPerfStats.totalDLPRBUsageBCCH += resourceAllocOutput.allocatedRB;
			/* - PERF_CA */
#endif
            /*Below Logic is poplating the dciCCEInfo_p structure*/         
            (*dciCCEInfoCmnChannel_p)->msgType  = BCCH_MSG;
            (*dciCCEInfoCmnChannel_p)->cceIndex = dlBCCHMsgInfo_p->cceIndex;
            dciCCEContainerInfo_p->countCmnChannelMsg++;
            /* + CQI_5.0 */
            /* ICIC changes start */
            POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_MUL_COMMON_CHANL_MSG
                                (BCCH_MSG,1, virtulResrcBlkType, 
                                 resourceAllocInput_p->mcsIndexForCommonChannel ,
                                 dciCCEInfo_p, resourceAllocOutput,dlBCCHMsgInfo_p,
                                 txMode, dlBCCHMsgInfo_p->tpc);
            /* ICIC changes end */
            /* - CQI_5.0 */

	    /*SPR 10411 changes start */
            /* if allocation is done from RAT_DISTRIBUTED, then to calculate the 
 	     * actual number of RBs allocated, we should take care of the situation 
 	     * when prbMappedToEvenSlotOfVRB is not equal to prbMappedToOddSlotOfVRB.
 	     * As, in this case resourceAllocOutput.allocatedRB does not reflect
 	     * actual number of RBs allocated. So, updated numOfRBs in DCI Pdu 
 	     * by finding the number of RBs actually allocated.*/

            if(virtulResrcBlkType)
            {
                dciCCEInfo_p->dciPduInfo.numOfRBs = 
                                        (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB - 
                                        raPolicyInfo_g[internalCellIndex].rat2LocalizedInfo.numOfVRBAvailable;
            } 
	    /*SPR 10411 changes end */ 
            /* _RED_VER_ */
            dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer=dlBCCHMsgInfo_p->redundancyVer;
            (*dciCCEInfoCmnChannel_p)++;
            dciCCEInfo_p->msgIdentifier = *msgIndentifier;
            *msgIndentifier += 1;
            (*dlMsgTypeMsgInfo_p)++;
#ifdef TDD_CONFIG
            /* Update DAI and store it in DCI Info */
            dciCCEInfo_p->dciPduInfo.dai = DAI_VALUE_ONE;
#endif

            return MAC_SUCCESS;
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                           " RB Allocation Failed for BCCH Message for RAT Localized \n");
            return MAC_FAILURE;
        }
    }
    else
    {    
        if ( MAC_SUCCESS == allocRBCommonChannelRAT2Localized(
                                resourceAllocInput_p,
                                &resourceAllocOutput,
                                &raPolicyInfo_g[internalCellIndex],dlBCCHMsgInfo_p->dciFormat,
                                internalCellIndex) )
        {
            /*Below Logic is poplating the dciCCEInfo_p structure*/         
            (*dciCCEInfoCmnChannel_p)->msgType  = BCCH_MSG;
            (*dciCCEInfoCmnChannel_p)->cceIndex = dlBCCHMsgInfo_p->
                                                  cceIndex;
            dciCCEContainerInfo_p->countCmnChannelMsg++;
            /* SPR 3062 Chg */
            /* + CQI_5.0 */
            POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_MUL_COMMON_CHANL_MSG
                                (BCCH_MSG,1,0,
                                 resourceAllocInput_p->mcsIndexForCommonChannel ,
                                 dciCCEInfo_p, resourceAllocOutput,dlBCCHMsgInfo_p,
                                 txMode, dlBCCHMsgInfo_p->tpc);
            /* - CQI_5.0 */
            /* SPR 3062 Chg Ends */

            /* _RED_VER_ */
            dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer=dlBCCHMsgInfo_p->redundancyVer;

            (*dciCCEInfoCmnChannel_p)++;
            dciCCEInfo_p->msgIdentifier = *msgIndentifier;
            *msgIndentifier += 1;
            (*dlMsgTypeMsgInfo_p)++;
#ifdef TDD_CONFIG
            /* Update DAI and store it in DCI Info */
            dciCCEInfo_p->dciPduInfo.dai = DAI_VALUE_ONE;
#endif

            return MAC_SUCCESS;
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                           " RB Allocation Failed for BCCH Message for RAT Localized \n");
            return MAC_FAILURE;
        }
    }
    /* SPR 1571 changes end */
}
/****************************************************************************************
 * Function Name    : processPcchMsg
 * Inputs           : dlMsgTypeMsgInfo_p - Pointer to DLMsgTypeMsgInfo,
 *                    dciCCEContainerInfo_p - Pointer to DciCCEContainerInfo,
 *                    dciCCEInfoCmnChannel_p - Pointer to DCICCEInfoCmnChannel
 *                    txMode - Mode of Transmission,
 *                    msgIndentifier - Message identifier,
 *                    dciCCEInfo_pp - Pointer to DciCCEInfo
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will process PCCH_MSG.
 *****************************************************************************************/ 
STATIC  MacRetType processPcchMsg(DLMsgTypeMsgInfo **dlMsgTypeMsgInfo_p,
                                        DciCCEContainerInfo *dciCCEContainerInfo_p,
                                        DCICCEInfoCmnChannel **dciCCEInfoCmnChannel_p,
                                        TransmissonMode txMode,
                                        UInt32 *msgIndentifier,
    /*Coverity_31956 Fix Start*/
                                        DciCCEInfo **dciCCEInfo_pp,
        /* + SPR_17858_409_CHANGES */
		tickType_t nextTick,
        /* - SPR_17858_409_CHANGES */
                                        InternalCellIndex internalCellIndex) 
    /*Coverity_31956 Fix End*/
{
    ResourceAllocatorInput *resourceAllocInput_p = PNULL;
    ResourceAllocatorOutput resourceAllocOutput ;            
    /* SPR 12299 changes start */
    UInt8 virtulResrcBlkType = 0;
    /*SPR 12299 Changes End*/

        /* + SPR_17858_409_CHANGES */
    UInt32 sfn = 0;
    UInt32 sf = 0;
        /* - SPR_17858_409_CHANGES */
    MacRetType retType = MAC_SUCCESS;
    DLPCCHMsgInfo *dlPCCHMsgInfo_p = PNULL;

    /*Coverity_31956 Fix Start*/
    DciCCEInfo * dciCCEInfo_p = PNULL;
    /*Coverity_31956 Fix End*/

    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR," MsgType is %u\n",
                   (*dlMsgTypeMsgInfo_p)->msgType);

    dlPCCHMsgInfo_p = &(*dlMsgTypeMsgInfo_p)->dlMsgInfo.
                      dlPCCHMsgInfo;         

    /*Coverity_31956 Fix Start*/
    *dciCCEInfo_pp = dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[
                   dlPCCHMsgInfo_p->cceIndex];
    /*Coverity_31956 Fix End*/
    resourceAllocInput_p = &dlPCCHMsgInfo_p->resourceAllocInput;
                   
    /*Coverity_31968 Fix Start*/
    resourceAllocOutput.nGAP = 0;
    /*Coverity_31968 Fix End*/

    /* ICIC changes start */ 
    /* SPR 20073 fix start */
    if(RAT_LOCALIZED == macParams_g.siSchedulingRATPolicy ) 
    {
        /* SPR 20073 fix end */
        retType = allocRBCommonChannelRAT2Localized(
                                resourceAllocInput_p,
                                &resourceAllocOutput,
                                &raPolicyInfo_g[internalCellIndex],dlPCCHMsgInfo_p->dciFormat,
                                internalCellIndex);
                        virtulResrcBlkType = 0;
    }
    else
    {
        retType =  allocRBCommonChannelRAT2Distributed(
                                resourceAllocInput_p,
                                &resourceAllocOutput,
                                &raPolicyInfo_g[internalCellIndex],dlPCCHMsgInfo_p->dciFormat,
                                internalCellIndex);
        virtulResrcBlkType = 1;

        if (MAC_SUCCESS != retType)
        {
            retType = allocRBCommonChannelRAT2Localized(
                                    resourceAllocInput_p,
                                    &resourceAllocOutput,
                                    &raPolicyInfo_g[internalCellIndex],dlPCCHMsgInfo_p->dciFormat,
                                    internalCellIndex);
            virtulResrcBlkType = 0;
        }
    }
                    
    if ( MAC_SUCCESS == retType)
    /* ICIC changes end */ 
    {
#ifdef PERF_STATS
		/* + PERF_CA */
        gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedDLPerfStats.totalDLPRBUsagePCCH += resourceAllocOutput.allocatedRB;
		/* - PERF_CA */
#endif
        /*Below Logic is poplating the dciCCEInfo_p structure*/                  

        (*dciCCEInfoCmnChannel_p)->msgType  = PCCH_MSG;
        (*dciCCEInfoCmnChannel_p)->cceIndex = dlPCCHMsgInfo_p->cceIndex;
        dciCCEContainerInfo_p->countCmnChannelMsg++;
        /* SPR 3062 Chg */
        /* ICIC changes start */ 
        POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_MUL_COMMON_CHANL_MSG
                            (PCCH_MSG,1, virtulResrcBlkType, 
                             resourceAllocInput_p->mcsIndexForCommonChannel ,
                             dciCCEInfo_p, resourceAllocOutput,dlPCCHMsgInfo_p,
                             txMode,dlPCCHMsgInfo_p->tpc);
        /* ICIC changes end */ 
        /* SPR 3062 Chg Ends */
                       
        /* _RED_VER_ */
                       
        if((DCI_FORMAT_1A==dciCCEInfo_p->dciPduInfo.dciFormat)|| (DCI_FORMAT_1C==dciCCEInfo_p->dciPduInfo.dciFormat))
        {
            dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer=RED_VER_0;
        } 
	/*SPR 10411 changes start */
             /* if allocation is done from RAT_DISTRIBUTED, then to calculate the 
 	     * actual number of RBs allocated, we should take care of the situation 
 	     * when prbMappedToEvenSlotOfVRB is not equal to prbMappedToOddSlotOfVRB.
 	     * As, in this case resourceAllocOutput.allocatedRB does not reflect
 	     * actual number of RBs allocated. So, updated numOfRBs in DCI Pdu 
 	     * by finding the number of RBs actually allocated.*/

        if(virtulResrcBlkType)
	{
		dciCCEInfo_p->dciPduInfo.numOfRBs = (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB - 
			raPolicyInfo_g[internalCellIndex].rat2LocalizedInfo.numOfVRBAvailable;
	} 
	/*SPR 10411 changes end */ 
                        
        (*dciCCEInfoCmnChannel_p)++;
        dciCCEInfo_p->msgIdentifier = *msgIndentifier;
        *msgIndentifier += 1;
        (*dlMsgTypeMsgInfo_p)++;
        return MAC_SUCCESS;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                       " RB Allocation Failed for PCCH Message \n");
        /* + SPR_17858_409_CHANGES */
        GET_SFN_SF_FROM_TTI(nextTick, sfn, sf);
        freePagingMsg(sfn, sf,internalCellIndex);
        /* - SPR_17858_409_CHANGES */

        /* SPR 6023 Changes Start */
        msgFree(dlPCCHMsgInfo_p->data_p);
    /*Coverity_31956 Fix Start*/
        dlPCCHMsgInfo_p->data_p = PNULL;
    /*Coverity_31956 Fix End*/
        /* SPR 6023 Changes End */
        return MAC_FAILURE;
    }
}
/****************************************************************************************
 * Function Name    : processRarMsg
 * Inputs           : dlMsgTypeMsgInfo_p - Pointer to DLMsgTypeMsgInfo,
 *                    dciCCEContainerInfo_p - Pointer to DciCCEContainerInfo,
 *                    dciCCEInfoCmnChannel_p - Pointer to DCICCEInfoCmnChannel
 *                    txMode - Mode of Transmission,
 *                    msgIndentifier - Message identifier,
 *                    dciCCEInfo_pp - Pointer to DciCCEInfo
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will process RAR_MSG.
 *****************************************************************************************/ 
STATIC  MacRetType processRarMsg(DLMsgTypeMsgInfo **dlMsgTypeMsgInfo_p,
                                        DciCCEContainerInfo *dciCCEContainerInfo_p,
                                        DCICCEInfoCmnChannel **dciCCEInfoCmnChannel_p,
                                        TransmissonMode txMode,
                                        UInt32 *msgIndentifier,
                                        DciCCEInfo **dciCCEInfo_pp,
                                        InternalCellIndex internalCellIndex)
{
    ResourceAllocatorInput *resourceAllocInput_p = PNULL;
    ResourceAllocatorOutput resourceAllocOutput ;            
    /* SPR 12299 changes end */
    UInt8 virtulResrcBlkType = 0;
    /* SPR 12299 changes end */
    MacRetType retType = MAC_SUCCESS;
    /*Coverity_31956 Fix Start*/
    DciCCEInfo * dciCCEInfo_p = PNULL;
    /*Coverity_31956 Fix End*/

    DLRACHMsgInfo *dlRACHMsgInfo_p = PNULL;
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR," MsgType is %u\n",
                   (*dlMsgTypeMsgInfo_p)->msgType);
    dlRACHMsgInfo_p = &(*dlMsgTypeMsgInfo_p)->dlMsgInfo.
                      dlRACHMsgInfo;

    /*Coverity_31956 Fix Start*/
    *dciCCEInfo_pp = dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[
                   dlRACHMsgInfo_p->cceIndex];
    /*Coverity_31956 Fix End*/
    resourceAllocInput_p = &dlRACHMsgInfo_p->resourceAllocInput;
    /*Coverity_31969 Fix Start*/
    resourceAllocOutput.nGAP = 0;
    /*Coverity_31969 Fix End*/

    /* ICIC changes start */ 
    /* SPR 20073 fix start */
    if(RAT_LOCALIZED == macParams_g.siSchedulingRATPolicy ) 
    {
        /* SPR 20073 fix end */
        retType = allocRBCommonChannelRAT2Localized(
                                resourceAllocInput_p,
                                &resourceAllocOutput,
                                &raPolicyInfo_g[internalCellIndex],dlRACHMsgInfo_p->dciFormat,
                                internalCellIndex);
        virtulResrcBlkType = 0;
    }
    else
    {
        retType =  allocRBCommonChannelRAT2Distributed(
                                resourceAllocInput_p,
                                &resourceAllocOutput,
                                &raPolicyInfo_g[internalCellIndex],dlRACHMsgInfo_p->dciFormat,
                                internalCellIndex);
        virtulResrcBlkType = 1;

        if (MAC_SUCCESS != retType)
        {
            retType = allocRBCommonChannelRAT2Localized(
                                    resourceAllocInput_p,
                                    &resourceAllocOutput,
                                    &raPolicyInfo_g[internalCellIndex],dlRACHMsgInfo_p->dciFormat,
                                    internalCellIndex);
            virtulResrcBlkType = 0;
        }
    }

    if ( MAC_SUCCESS == retType) 
    /* ICIC changes end */ 
    {
        /*Below Logic is poplating the dciCCEInfo_p structure*/                  
        (*dciCCEInfoCmnChannel_p)->msgType  = RAR_MSG;
        (*dciCCEInfoCmnChannel_p)->cceIndex = dlRACHMsgInfo_p->cceIndex;
        dciCCEContainerInfo_p->countCmnChannelMsg++;
        /* SPR 3062 Chg */
        /* ICIC changes start */ 
        POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_MUL_COMMON_CHANL_MSG
                            (RAR_MSG,1, virtulResrcBlkType, 
                             resourceAllocInput_p->mcsIndexForCommonChannel ,
                             dciCCEInfo_p, resourceAllocOutput,dlRACHMsgInfo_p,
                             txMode, dlRACHMsgInfo_p->tpc);
        /* ICIC changes end */ 
        /* SPR 3062 Chg Ends */
        /* _RED_VER_ */
                        
        if((DCI_FORMAT_1A==dciCCEInfo_p->dciPduInfo.dciFormat)|| (DCI_FORMAT_1C==dciCCEInfo_p->dciPduInfo.dciFormat))
        {
            dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer=RED_VER_0;
        }    
	/*SPR 10411 changes start */
 	     /* if allocation is done from RAT_DISTRIBUTED, then to calculate the 
 	     * actual number of RBs allocated, we should take care of the situation 
 	     * when prbMappedToEvenSlotOfVRB is not equal to prbMappedToOddSlotOfVRB.
 	     * As, in this case resourceAllocOutput.allocatedRB does not reflect
 	     * actual number of RBs allocated. So, updated numOfRBs in DCI Pdu 
 	     * by finding the number of RBs actually allocated.*/

     	if(virtulResrcBlkType)
	{
		dciCCEInfo_p->dciPduInfo.numOfRBs = (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB - 
			raPolicyInfo_g[internalCellIndex].rat2LocalizedInfo.numOfVRBAvailable;
	} 
	/*SPR 10411 changes end */ 
                    
        (*dciCCEInfoCmnChannel_p)++;
        dciCCEInfo_p->msgIdentifier = *msgIndentifier;
#ifdef TDD_CONFIG
        /* Update DAI and store it in DCI Info */
        dciCCEInfo_p->dciPduInfo.dai = DAI_VALUE_ONE;
#endif
        *msgIndentifier += 1;
        (*dlMsgTypeMsgInfo_p)++;
        return MAC_SUCCESS;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                       " RB Allocation Failed for RACH Message \n");
        /* SPR 6023 Changes Start */
        msgFree(dlRACHMsgInfo_p->data_p);
        /* SPR 6023 Changes End */
        return MAC_FAILURE;
    }
}
/******************************************************************************
 * Function Name    : updateRaPolicyInfoAgain
 * Inputs           : nextTick
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description     : This function will update globally define ra policy info.
 ******************************************************************************/
/* SPR 15909 fix start */
/* SPR 21631 Start */
STATIC  void updateRaPolicyInfoAgain(tickType_t nextTick 
/* SPR 15909 fix end */        
                                      ,InternalCellIndex internalCellIndex)
/* SPR 21631 End */
{
    InitParams	*initParams_p = cellSpecificParams_g.\
               cellConfigAndInitParams_p[internalCellIndex]->initParams_p;

    /* SPR 21631 Start */
    if (0 == (nextTick % 10))
    /* SPR 21631 End */
    {
        if ((initParams_p->commChWithMIBAllowed) && 
            (!initParams_p->dlSchWithMIBAllowed))
        {
            reserveCommonChannelRB(&raPolicyInfo_g[internalCellIndex],
                    internalCellIndex);
        }
        else if ((initParams_p->dlSchWithMIBAllowed) &&
                   (!initParams_p->commChWithMIBAllowed))
        {
            unreserveMIBRB(&raPolicyInfo_g[internalCellIndex],internalCellIndex);
        }
    }
}
/****************************************************************************************
 * Function Name    : processCCCHMsg
 * Inputs           : dlCCCHMsgTypeMsgInfo_p - Pointer to DLMsgTypeMsgInfo,
 *                    dciCCEContainerInfo_p - Pointer to DciCCEContainerInfo,
 *                    dciCCEInfoCmnChannel_p - Pointer to DCICCEInfoCmnChannel
 *                    msgIndentifier - 
 *                    txMode - Mode of Transmission,
 *                    currentGlobalTick - Current global tick,
 *                    delayToPhy,
 *                    nextTick,
 *                    subFrame - Present subframe
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will process CCCH msg.
 *****************************************************************************************/ 
STATIC  MacRetType processCCCHMsg(DLMsgTypeMsgInfo *dlCCCHMsgTypeMsgInfo_p,
                                  DciCCEContainerInfo *dciCCEContainerInfo_p,
                                  DCICCEInfoCmnChannel **dciCCEInfoCmnChannel_p,
                                  UInt32 *msgIndentifier,
                                  /* +- SPR 17777 */
                                  TransmissonMode txMode
                                  /* SPR 15909 fix start */
                                  /* +- SPR 17777 */
                                  #ifdef FDD_CONFIG
                                  ,UInt32 delayToPhy,
                                  tickType_t nextTick
                                  /* SPR 15909 fix end */
                                  /* +- SPR 17777 */
                                  ,UInt32 subFrame
                                  #endif
                                  ,InternalCellIndex internalCellIndex
                                  #ifdef TDD_CONFIG
                                  /* +- SPR 17777 */
                                  ,tickType_t currentGlobalTick
                                  ,UInt8 dlSubframeNum,
                                  UInt32 sysFrame
                                  #endif
                                  )
{
#ifdef FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t scheduledExpiryTick = 0;
    /* SPR 15909 fix end */
    UInt32 harqSubFrameNum = 0;
    UInt32 subFrameNum = 0;
#endif

    DLCCCHMsgInfo   *dlCCCHMsgInfo_p = PNULL;
    DLUEContextInfo *ueContextInfo_p = PNULL;
    /* SPR 4395 changes start */
    PendingCCCHQueueNode *pendingCcchQueueNode_p  = PNULL;
    /* SPR 4395 changes end */
    ResourceAllocatorInput *resourceAllocInput_p = PNULL;
    ResourceAllocatorOutput resourceAllocOutput ;            
    DciCCEInfo *dciCCEInfo_p = PNULL;    
    UInt32 aggregationLvl = 0;
#ifdef FDD_CONFIG
    /* + SPR 17733 */
    DLHARQProcess *harqProcess_p = PNULL;
    /* - SPR 17733 */
#endif

    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            " MsgType is %u\n",dlCCCHMsgTypeMsgInfo_p->msgType);
    dlCCCHMsgInfo_p = &dlCCCHMsgTypeMsgInfo_p->dlMsgInfo.
        dlCCCHMsgInfo;
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[
        dlCCCHMsgInfo_p->cceIndex];
    resourceAllocInput_p = &dlCCCHMsgInfo_p->resourceAllocInput;

    if ( MAC_SUCCESS == allocRBCommonChannelRAT2Localized(
                resourceAllocInput_p,
                &resourceAllocOutput,
                &raPolicyInfo_g[internalCellIndex],dlCCCHMsgInfo_p->dciFormat,
                internalCellIndex)) 
    {
#ifdef PERF_STATS
		/* + PERF_CA */
        gMacCellPerfStats_p[internalCellIndex]->lteCellMacDLPerfStats.totalDLPRBUsageSRB[0] += resourceAllocOutput.allocatedRB;
		/* - PERF_CA */
#endif
        /*Below Logic is poplating the dciCCEInfo_p structure*/                  

        (*dciCCEInfoCmnChannel_p)->msgType  = CCCH_MSG;
        (*dciCCEInfoCmnChannel_p)->cceIndex = dlCCCHMsgInfo_p->cceIndex;
        /* SPR 20924 fix start */
        /* code removed */
        /* counter for common channel msg should not be increment as ccch
         * message would get scheduled even if the counter is not incremented.
         * */
        /* SPR 20924 fix end */
        /*  SPR  3444 changes start */
        /* + CQI_5.0 */
        POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_CCCH_MSG(\
                CCCH_MSG,1, 0, resourceAllocInput_p->\
                mcsIndexForCommonChannel ,dciCCEInfo_p,\
                resourceAllocOutput, dlCCCHMsgInfo_p,txMode,internalCellIndex
#ifdef FDD_CONFIG
                /* + SPR 17733 */
                ,harqProcess_p 
                /* - SPR 17733 */
#endif
                );
        /* - CQI_5.0 */
        /*  SPR  3444 changes end */
        dciCCEInfo_p->msgIdentifier = *msgIndentifier;
        /* SPR 2889 Fix Start */
        dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer = RED_VER_0; // Added for handing redundancy 
        /* SPR 2889 Fix End */
        dciCCEInfo_p->dciPduInfo.harqProcessId = dlCCCHMsgInfo_p->harqProcessId;
        LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                " #### HarqProcess Id = %d \n",dciCCEInfo_p->dciPduInfo.harqProcessId); 
        if (INVALID_HARQ_ID != dciCCEInfo_p->dciPduInfo.harqProcessId)
        {
            ueContextInfo_p = &dlUECtxInfoArr_g[dciCCEInfo_p->
                dciPduInfo.ueIndex];
            /*START_SPR_5287*/
            if (ueContextInfo_p->dlUEContext_p !=NULL)
            {
                /*END_SPR_5287*/
                /* SPR 903 Start*/
                dciCCEInfo_p->dciPduInfo.ueCategory =
                    ueContextInfo_p->dlUEContext_p->ueCategory;
                /* SPR 903 End */

                /* CLPC_CHG */
                dciCCEInfo_p->dciPduInfo.pA = 
                    ueContextInfo_p->dlUEContext_p->pA;
                /* CLPC_CHG END */
#ifdef TDD_CONFIG
                /* Update DAI and store it in DCI Info */
                dciCCEInfo_p->dciPduInfo.dai = 
                    /* SPR 609 changes start */
                    updateDai(ueContextInfo_p->dlUEContext_p,
                            /* SPR 609 changes end */
                            currentGlobalTick,
    /* SPR 23393 Changes Start */
                            currentGlobalTick % MAX_SUB_FRAME,
                            /* SPR 23393 Changes End */
                            internalCellIndex);
#endif
                if (FALSE == ueContextInfo_p->pendingDeleteFlag)
                {
                    /* + SPR 11491 Changes */
                    if(IS_PCELL_OF_UE(ueContextInfo_p->dlUEContext_p,internalCellIndex))
                    {
                        ueContextInfo_p->dlUEContext_p->isPdcchAllocatedFlag = FALSE;
                    }
                    else
                    {
                        ueContextInfo_p->dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->isPdcchAllocatedFlag = FALSE;
                    }
                    /* - SPR 11491 Changes */
                }
#ifdef FDD_CONFIG
                /*  Wrap around subFrameNum at 10 (MAX_SUB_FRAME) */
                subFrameNum = (subFrame + delayToPhy)% MAX_SUB_FRAME;
                harqSubFrameNum = subFrameNum + 4;
                if (harqSubFrameNum >= NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME)
                {
                    harqSubFrameNum -= NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;    
                }
                scheduledExpiryTick = nextTick + 8;

                dlHarqTimerStart(ueContextInfo_p->dlUEContext_p->ueIndex, dciCCEInfo_p->dciPduInfo.harqProcessId, 
                        harqSubFrameNum, scheduledExpiryTick, DRA_SCHEDULING,internalCellIndex);
#elif TDD_CONFIG
                tddDlHarqTimerStart(ueContextInfo_p->dlUEContext_p->ueIndex,
                        dciCCEInfo_p->dciPduInfo.harqProcessId,
                        dlSubframeNum,
                        currentGlobalTick,
                        sysFrame,
                        /* + SPS_TDD */
                        DRA_SCHEDULING,
                        /* - SPS_TDD */
                        internalCellIndex
                        ); 
#endif           
                /*START_SPR_5287*/
            } 
            /*END_SPR_5287*/
        }

        (*dciCCEInfoCmnChannel_p)++;
        *msgIndentifier += 1;
        return MAC_SUCCESS;
    }
    else
    {
        /* SPR 16552 Fix ++ */
        DLHARQProcess *dlHarqProcess_p = PNULL;
        DLUEContext *dlUEContext_p = PNULL;

        if(!dlUECtxInfoArr_g[dciCCEInfo_p->dciPduInfo.ueIndex].pendingDeleteFlag)
        {
            dlUEContext_p = dlUECtxInfoArr_g[dciCCEInfo_p->dciPduInfo.ueIndex].dlUEContext_p;
            if(INVALID_HARQ_ID != dlCCCHMsgInfo_p->harqProcessId)
            {
                dlHarqProcess_p = &dlUEContext_p->dlHarqContext_p->harqProcess[dlCCCHMsgInfo_p->harqProcessId];
            }
        }
        /* SPR 16552 Fix -- */
        /* SPR 4395 changes start */
        GET_MEM_FROM_POOL(PendingCCCHQueueNode , pendingCcchQueueNode_p,sizeof(PendingCCCHQueueNode),PNULL);

        /* + coverity 32787 */
        if (PNULL == pendingCcchQueueNode_p)
        {
            aggregationLvl = dciCCEInfo_p->dciPduInfo.aggregationLvl;
            mac_reset_cce(aggregationLvl, dciCCEInfo_p, internalCellIndex);
            /* SPR 16552 Fix ++ */
            if(dlHarqProcess_p)
            {
                dlHarqProcess_p->transmissionBuffer_p = PNULL;
                freeDLHarqProcess(dlUEContext_p,dlHarqProcess_p,internalCellIndex);
            }
            /* SPR 16552 Fix -- */

            /* SPR 10156 change start */
            msgFree(dlCCCHMsgInfo_p->data_p);
            dlCCCHMsgInfo_p->data_p = PNULL;
            /* SPR 10156 change end */
            return MAC_FAILURE;
        }
        /* Coverity 32787 end */

        /* SPR 16552 Fix ++ */
        if(dlHarqProcess_p)
        {
            dlHarqProcess_p->transmissionBuffer_p = PNULL;
            freeDLHarqProcess(dlUEContext_p,dlHarqProcess_p,internalCellIndex);
        }
        /* SPR 16552 Fix -- */
        pendingCcchQueueNode_p->dataLen  = dlCCCHMsgInfo_p->dataLen;
        pendingCcchQueueNode_p->rejFlag = dlCCCHMsgInfo_p->rejFlag;
        pendingCcchQueueNode_p->ueId    = dciCCEInfo_p->dciPduInfo.ueIndex;
        pendingCcchQueueNode_p->data_p = dlCCCHMsgInfo_p->data_p;

        if (pushNode(pendingCcchQueue_gp[internalCellIndex], &pendingCcchQueueNode_p->pendingCcchNodeAnchor))
        { 
            lteWarning("[%s]: ERROR: pushNode to pendingCcchQueue_gp failed\n",__func__);
            /* SPR 10156 change start */
            msgFree(dlCCCHMsgInfo_p->data_p);
            dlCCCHMsgInfo_p->data_p = PNULL;
            /* SPR 10156 change end */
            /* Coverity 32787 start */
            freeMemPool(pendingCcchQueueNode_p);
            /* Coverity 32787 end */
            return MAC_FAILURE;
        }

        aggregationLvl = dciCCEInfo_p->dciPduInfo.aggregationLvl;
        /* Cyclomatic Complexity Changes */
        mac_reset_cce(aggregationLvl, dciCCEInfo_p, internalCellIndex);
        /* SPR 4395 changes end */
        LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                "RB Allocation Failed for Contention Res  Message\n");
        return MAC_SUCCESS;
    }
}
/****************************************************************************************
 * Function Name    : processContentionResolutionMsg
 * Inputs           : dlCCCHMsgTypeMsgInfo_p - Pointer to DLMsgTypeMsgInfo,
 *                    dciCCEContainerInfo_p - Pointer to DciCCEContainerInfo,
 *                    dciCCEInfoCmnChannel_p - Pointer to DCICCEInfoCmnChannel
 *                    msgIndentifier - Message identifier,
 *                    txMode - Mode of Transmission,
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will process Contention resolution msg.
 *****************************************************************************************/ 
STATIC  MacRetType processContentionResolutionMsg(DLMsgTypeMsgInfo *dlCCCHMsgTypeMsgInfo_p,
                                                        DciCCEContainerInfo *dciCCEContainerInfo_p,
                                                        DCICCEInfoCmnChannel **dciCCEInfoCmnChannel_p,
                                                        UInt32 *msgIndentifier,
                                                        TransmissonMode txMode,
                                                        InternalCellIndex internalCellIndex
                                                        /*Spr 17733 Merge Fix Start */
#if (defined(FDD_CONFIG) && !defined(DL_UL_SPLIT)) /*SPR 19078 +-*/
                                                        ,UInt32 delayToPhy
#endif
                                                        /*Spr 17733 Merge Fix End */
                                                        )
{
    ResourceAllocatorInput *resourceAllocInput_p = PNULL;
    ResourceAllocatorOutput resourceAllocOutput ;            
    DciCCEInfo *dciCCEInfo_p = PNULL;    
    DLContentionReslMsgInfo *dlContReslMsgInfo_p = PNULL;           
#ifdef FDD_CONFIG
    /* + SPR 17733 */
    TempCRNTICtx *tempUEContext_p = PNULL; 
    DLHARQProcess *harqProcess_p = PNULL;
        /*Spr 17733 Merge Fix Start */
    /*SPR 19078 +*/
#ifndef DL_UL_SPLIT
    UInt64  scheduledExpiryTick = 0;
    UInt8   harqSubFrameNum = 0;
    UInt32 subFrameNum = 0;
    UInt32 sysFrameNum = 0;
#endif    
    /*SPR 19078 -*/
        /*Spr 17733 Merge Fix End */
    /* - SPR 17733 */
#endif

    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR," MsgType is %u\n",
                   dlCCCHMsgTypeMsgInfo_p->msgType);    
    dlContReslMsgInfo_p = &dlCCCHMsgTypeMsgInfo_p->dlMsgInfo.
                   dlContentionReslMsgInfo;               
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[
                   dlContReslMsgInfo_p->cceIndex];
    resourceAllocInput_p = &dlContReslMsgInfo_p->resourceAllocInput;
#ifdef FDD_CONFIG
    /* + SPR 17733 */
    tempUEContext_p =  tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[
        dciCCEInfo_p->dciPduInfo.ueIndex].tempCRNTICtx_p;
    if( PNULL == tempUEContext_p )
    {
        LOG_MSG(MAC_ULSCH_FREQ_SEL_SCH_ID,LOGWARNING,MAC_PUSCH,
                getCurrentTick(), dciCCEInfo_p->dciPduInfo.ueIndex, 0,0,0,0,
                0,0,__func__,"");
        msgFree(dlContReslMsgInfo_p->data_p);
        return MAC_FAILURE;
    }
    /* - SPR 17733 */
#endif
    if ( MAC_SUCCESS == allocRBCommonChannelRAT2Localized(
                                resourceAllocInput_p,
                                &resourceAllocOutput,
                                &raPolicyInfo_g[internalCellIndex],dlContReslMsgInfo_p->dciFormat,
                                internalCellIndex)) 
    {
        /*Below Logic is poplating the dciCCEInfo_p structure*/                  

        (*dciCCEInfoCmnChannel_p)->msgType  = CONTENTION_RESL_MSG;
        (*dciCCEInfoCmnChannel_p)->cceIndex = dlContReslMsgInfo_p->cceIndex;
        /* + SPR 17733 */
#ifdef TDD_CONFIG
        dciCCEContainerInfo_p->countCmnChannelMsg++;
#else
	harqProcess_p = &tempUEContext_p->dlHarqCtx_p->harqProcess[
		dciCCEInfo_p->dciPduInfo.harqProcessId];
#endif
        /*  SPR  3444 changes start */
        /* + CQI_5.0 */
        POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_CCCH_MSG(\
                CONTENTION_RESL_MSG, 1,
                0, resourceAllocInput_p->mcsIndexForCommonChannel,
                dciCCEInfo_p, resourceAllocOutput, dlContReslMsgInfo_p,
                txMode,internalCellIndex
#ifdef FDD_CONFIG
                , harqProcess_p 
#endif
        /* - SPR 17733 */
                );
         /* - CQI_5.0 */
         /*  SPR  3444 changes end */
        dciCCEInfo_p->msgIdentifier = *msgIndentifier;

        /* SPR 17112 Fix Start */
        /* SPR 903 Start*/
        dciCCEInfo_p->dciPduInfo.ueCategory = MAC_MIN_UE_CATEGORY;
        /* SPR 903 End */
        /* SPR 17112 Fix End */

        /* CLPC_CHG */
        /* SPR 17275 Fix Start */
        dciCCEInfo_p->dciPduInfo.pA = DEFAULT_PA_VALUE;
        /* SPR 17275 Fix End */
        /* CLPC_CHG END */
        /*Spr 17733 Merge Fix Start */
#ifdef FDD_CONFIG
#ifndef DL_UL_SPLIT

        getSFAndSFN(&sysFrameNum, &subFrameNum,internalCellIndex);

        scheduledExpiryTick = getCurrentTickForCell(internalCellIndex) + delayToPhy + 8;
        harqSubFrameNum = subFrameNum + delayToPhy + FDD_HARQ_OFFSET;

        if (harqSubFrameNum >= NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME)
        {
            harqSubFrameNum -= NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
        }
        dlHarqTimerStartForMsg4(dciCCEInfo_p->dciPduInfo.ueIndex, 
                dciCCEInfo_p->dciPduInfo.harqProcessId, 
                harqSubFrameNum,
                scheduledExpiryTick,
                internalCellIndex);
#endif
#endif
        /*Spr 17733 Merge Fix End */
#ifdef TDD_CONFIG
        /* Update DAI and store it in DCI Info */
        dciCCEInfo_p->dciPduInfo.dai = DAI_VALUE_ONE;
#endif

        (*dciCCEInfoCmnChannel_p)++;
        *msgIndentifier += 1;
        return MAC_SUCCESS;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                       " RB Allocation Failed for Contention Res  Message \n");
        /* SPR 6023 Changes Start */
        msgFree(dlContReslMsgInfo_p->data_p);
        /* SPR 6023 Changes End */
        return MAC_FAILURE;
    }
}
/*******************************************************************************************************
 * Function Name    : processDlCCCHMsg
 * Inputs           : dlCCCHMsgTypeMsgInfo_p - Pointer to DLMsgTypeMsgInfo,
 *                    dciCCEContainerInfo_p - Pointer to DciCCEContainerInfo,
 *                    dciCCEInfoCmnChannel_p - Pointer to DCICCEInfoCmnChannel
 *                    msgIndentifier - Message identifier,
 *                    txMode - Mode of Transmission,
 *                    currentGlobalTick - Current global tick,
 *                    delayToPhy,
 *                    nextTick,
 *                    subFrame - Present Subframe
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will process dl ccch msg (ccch msg and contention resolution msg).
** *****************************************************************************************************/ 
STATIC  MacRetType processDlCCCHMsg(DLMsgTypeMsgInfo *dlCCCHMsgTypeMsgInfo_p,
                                    DciCCEContainerInfo *dciCCEContainerInfo_p,
                                    DCICCEInfoCmnChannel **dciCCEInfoCmnChannel_p,
                                    UInt32 *msgIndentifier,
                                    /* +- SPR 17777 */
                                    TransmissonMode txMode
                                    /* SPR 15909 fix start */
                                    /* +- SPR 17777 */
                                    #ifdef FDD_CONFIG
                                    ,UInt32 delayToPhy,
                                    tickType_t nextTick
                                    /* SPR 15909 fix end */
                                    /* +- SPR 17777 */
                                    ,UInt32 subFrame
                                    #endif
                                    ,InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
                                    /* +- SPR 17777 */
                                    ,tickType_t currentGlobalTick
                                    ,UInt8 dlSubframeNum,
                                    UInt32 sysFrame
#endif
                                    )
{
    /* SPR 8043 fix start */ 
    DLContentionReslMsgInfo *dlContReslMsgInfo_p = PNULL;
    DciCCEInfo              *dciCCEInfo_p        = PNULL;

    dlContReslMsgInfo_p = &dlCCCHMsgTypeMsgInfo_p->dlMsgInfo.
                            dlContentionReslMsgInfo;

    dciCCEInfo_p       = &dciCCEContainerInfo_p->dciCCEInfo[
                            dlContReslMsgInfo_p->cceIndex];    
    /* SPR 8043 fix end */


    switch ( dlCCCHMsgTypeMsgInfo_p->msgType )
    {
        case  CCCH_MSG :
        {
            /*Cyclomatic Complexity changes - starts here */
            if (MAC_SUCCESS == processCCCHMsg(dlCCCHMsgTypeMsgInfo_p,
                                              dciCCEContainerInfo_p,
                                              dciCCEInfoCmnChannel_p,
                                              msgIndentifier,
                                              /* +- SPR 17777 */
                                              txMode
                                              /* +- SPR 17777 */
#ifdef FDD_CONFIG
                                              ,delayToPhy,
                                              nextTick
                                              /* +- SPR 17777 */
                                              ,subFrame
#endif
                                              ,internalCellIndex
#ifdef TDD_CONFIG
                                              /* +- SPR 17777 */
                                              ,currentGlobalTick
                                              ,dlSubframeNum,
                                              sysFrame
#endif
                                              ))
            {
                return MAC_SUCCESS;
            }
            return MAC_FAILURE;
            /*Cyclomatic Complexity changes - ends here*/
                }
            break;
        case  CONTENTION_RESL_MSG:
        {
            /*Cyclomatic Complexity changes - starts here */
            if (MAC_SUCCESS == processContentionResolutionMsg(dlCCCHMsgTypeMsgInfo_p,
                                                              dciCCEContainerInfo_p,
                                                              dciCCEInfoCmnChannel_p,
                                                              msgIndentifier,
                                                              txMode,
                                                              internalCellIndex
                                                              /*Spr 17733 Merge Fix Start */
#if (defined(FDD_CONFIG) && !defined(DL_UL_SPLIT)) /*SPR 19078 +-*/
                                                              ,delayToPhy

#endif
                                                              /*Spr 17733 Merge Fix End */
                                                              ))
            {
                return MAC_SUCCESS;
            }
            /*SPR 8043 fix start */ 
            else
            {
                mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl, 
                              dciCCEInfo_p,
                              internalCellIndex);
            }    
            /* SPR 8043 fix end */

            /*Cyclomatic Complexity changes - ends here*/
        }
            break;
        default:
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Message \
                    Type Not handled\n");
            /* Klock_report_fix : dciCCEInfo_p will be de-referenced
             * hence loop is continued if it is Message Type is not 
             * handeled
             */
            return MAC_SUCCESS;
        }
    } 
    return MAC_SUCCESS;
}
/*Cyclomatic Complexity changes - ends here*/


/* QOS PDSCH Change Start Start*/
/****************************************************************************************
 * Function Name    : deleteScheduledLCList
 * Inputs           : ueDLContext_p - Pointer to DLUEContext
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function deletes the scheduled LC List for the UE which
 *                    is not scheduled by the PDSCH.
 *****************************************************************************************/ 
void deleteScheduledLCList( DLUEContext *ueDLContext_p,
                            InternalCellIndex internalCellIndex)
{
    void *dlQosStrategyLCNodeVoid_p = PNULL;
    DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
    UInt8 servingCellArrIndex = INVALID_8_VAL;

    dlQosStrategyLCNodeVoid_p = (void *)
        listPopNode(&(ueDLContext_p->dlScheduledLcList));

    while(dlQosStrategyLCNodeVoid_p)
    {
      DL_QOS_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(dlQosStrategyLCNodeVoid_p , 
          dlQosStrategyLCNode_p);

      dlQosStrategyLCNode_p->schedulableBytes = 0;
      dlQosStrategyLCNode_p->schedulableBytesPcell = 0;

      /*SPR 21473 +*/
      /* While deleting schedule LC, requestedDataSize should be reset
       * otherwise it contains invalid value and validation fail during LC
       * multiplexing. In single cell scenario it will be reset during QOS 
       * token update */
      dlQosStrategyLCNode_p->requestedDataSize = 0;
      /*SPR 21473 -*/

      if( (IS_CA_ENABLED()) && 
          (dlQosStrategyLCNode_p->dLUESCellStartegyNode_p[START_SCELL_INDEX]) )
      {
        dlQosStrategyLCNode_p->dLUESCellStartegyNode_p[START_SCELL_INDEX]->schedulableBytesScell = 0;
      }

      LOG_MAC_MSG(MAC_DELETE_SCH_LIST_NODE,LOGBRIEF,
          MAC_MUX,getCurrentTick(), __LINE__,
          (UInt32)ueDLContext_p->ueIndex, dlQosStrategyLCNode_p->lcId,
          ueDLContext_p->scheduledLcCount,
          DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
          DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");


      ueDLContext_p->scheduledLcCount--;

      dlQosStrategyLCNodeVoid_p = (void *)
        listPopNode(&(ueDLContext_p->dlScheduledLcList));
    }

    if(ueDLContext_p->scheduledLcCount)
    {
        ueDLContext_p->scheduledLcCount = 0;
    }
    //fprintf(stderr,"Info at %s %d %llu: schdeuledUeCount %d \n",__func__,__LINE__,getCurrentTick(),ueDLContext_p->scheduledLcCount);

    if(!IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
    {
        servingCellArrIndex = 
            getServeCellIndexFromInternalCellId(ueDLContext_p, internalCellIndex);
    }

    if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
    {
        if (ueDLContext_p->dlStrategyTxNode_p &&
                ueDLContext_p->dlStrategyTxNode_p->transmissionType != RE_TX)
        {
            ueDLContext_p->dlStrategyTxNode_p = PNULL;
        }
    }/*klockwork warning fix*/
    else if(servingCellArrIndex <= MAX_NUM_SCELL)
    {
        if (ueDLContext_p->dlUeScellContext_p[servingCellArrIndex]->dlStrategyTxNode_p &&
                ueDLContext_p->dlUeScellContext_p[servingCellArrIndex]->\
                dlStrategyTxNode_p->transmissionType != RE_TX)
        {
            ueDLContext_p->dlUeScellContext_p[servingCellArrIndex]->\
                dlStrategyTxNode_p = PNULL;
        }
    }
}
/****************************************************************************************
 * Function Name    : isResourceFreeInGLobalRAT
 * Inputs           : bestVRBSpanInfo_p - Pointer to Resources start and end index,
 *                    rat2LocalizedVRBInfo_p - Pointer to RAT2LocalizedVRBInfo
 * Outputs          : None
 * Returns          : TRUE/FALSE
 * Description      : This function check in Global  RAT2LocalizedVRB if
 *                    Resources are free or allocated.
 *****************************************************************************************/ 
UInt8 isResourceFreeInGLobalRAT(
          BestVRBSpanInfo      *bestVRBSpanInfo_p,
          RAT2LocalizedVRBInfo *rat2LocalizedVRBInfo_p)
{
    UInt8 startingVRBIndex = bestVRBSpanInfo_p->startingVRBIndex;
    /* !
     * \code
     * - Check from startingVRBIndex to endingVRBIndex
     *    - For any of VRBIndex 
     *    - if (rat2LocalizedVRBInfo_p[startingVRBIndex].nextfreeVRBIndex 
     *              != (startingVRBIndex + 1))
     *        - Then Resources Are Not Free in Global RAT2 Map.
     *
     * \endcode
     * */
    while(startingVRBIndex !=  bestVRBSpanInfo_p->endingVRBIndex)
    {
        if (rat2LocalizedVRBInfo_p[startingVRBIndex].nextfreeVRBIndex \
                != (startingVRBIndex + 1))
        {
            return FALSE;
        }
        startingVRBIndex++;

    }
    return TRUE;
}
/****************************************************************************************
 * Function Name    : allocRBInPDSCHForSPSData
 * Inputs           : withMuxFlag - This when set to one Indicates that we need to call
 *                                  directly invoke Mux Execution Logic,
 *                    dlMsgTypeMsgInfo_p - This stores DLMsgTypeMsgInfo Pointer which
 *                                         store the information of those UE for which
 *                                         the PDCCH region has been allocated. It is a
 *                                         Pointer to DLMsgTypeMsgInfo,
 *                    dlStrategyTxNode_p - Pointer to Downlink Strategy Tx Node,
 *                    ueContext_p - Pointer to Downlink UE Context,
 *                    harqProcess_p - Harq Process of the UE,
 *                    dciCCEInfo_p - Pointer to DciCCEInfo,
 *                    dlMsgInfo_p - Pointer to DLDataMsgInfo,
 *                    scheduledTick - This holds the Tick Value in which UE is
 *                                    expected to be scheduled,
 *                    allocSPSFirst - 
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : raPolicyInfo_p - Pointer to ResourceAllocationPolicyInfo,
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function Allocates the Resources for SPS based on the
 *                    Resources reserved in SPS Reources Map.
 *****************************************************************************************/ 
 UInt32 allocRBInPDSCHForSPSData(
      UInt32 withMuxFlag,
      DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
      DLStrategyTxNode *dlStrategyTxNode_p,
      DLUEContext *ueContext_p,
      DLHARQProcess *harqProcess_p,
      DciCCEInfo *dciCCEInfo_p,
      DLDataMsgInfo *dlMsgInfo_p,
      ResourceAllocationPolicyInfo *raPolicyInfo_p,
      /* SPR 15909 fix start */
      tickType_t scheduledTick,
      /* SPR 15909 fix end */
      UInt8 allocSPSFirst,
      /* + SPS_TDD */
#ifdef TDD_CONFIG
      UInt8 dlSubframeNum,
      UInt32 phyDelay,
      UInt32 sysFrame,
#endif
      /* - SPS_TDD */
      InternalCellIndex internalCellIndex)
{
    BestVRBSpanInfo             bestVRBSpanInfo;
    ResourceAllocatorOutput     resourceAllocOutput ;    
    LP_SpsResourceAllocatorInfo spsResourceAllocInfo_p = PNULL;   
    UInt8 calculatedRNTP = 0;
    
    if(allocSPSFirst)
    {
        return MAC_SUCCESS;
    }
    if((harqProcess_p->isTBOneValid) && 
                     (! harqProcess_p->isTBTwoValid)) 
    {
        harqProcess_p->dciFormat =  dlMsgInfo_p->dciFormat;
        spsResourceAllocInfo_p = \
            &ueContext_p->spsDlInfo.resAllocInfo;

        /*!
         * \cdoe
         * - Get the resourceAllocOutput From the SPS PDSCH
         * \endcode
         * */
        resourceAllocOutput.ratPolicy = RA_POLICY_2_LOCALIZED;
        resourceAllocOutput.nGAP = 0;
        resourceAllocOutput.allocatedRB = \
            spsResourceAllocInfo_p->allocatedRB;
        resourceAllocOutput.startingVRBIndex = \
            bestVRBSpanInfo.startingVRBIndex = \
            spsResourceAllocInfo_p->startingVRBIndex;
        bestVRBSpanInfo.endingVRBIndex = \
            resourceAllocOutput.allocatedRB + \
            bestVRBSpanInfo.startingVRBIndex - 1;


         /*!
          * \code
          * - Update the Resource Assignment bitmap 
          * \endcoe
          * */
         populateResourceAssignmentInfo(
             &resourceAllocOutput.resourceAssignmentBitMap, 
             resourceAllocOutput.allocatedRB,
             resourceAllocOutput.startingVRBIndex,
             0,
             harqProcess_p->dciFormat,
             internalCellIndex);
         /*!
         * \code
         * Check from Global Rat Info If Resources is free or not 
         * \endcode
         * */
         if(FALSE == isResourceFreeInGLobalRAT(
             &bestVRBSpanInfo,
             raPolicyInfo_p->rat2LocalizedInfo.rat2LocalizedVRBInfo
             ))
         {
             LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                     "ERROR SPS Resource Overlapping DRA Resource\n");
         }
         
         /*!
         * \code
         * Updating the resourceAssignmentBitMap in Ue Context.
         * \endcode
         * */
         spsResourceAllocInfo_p->resourceAssignmentBitMap = \
         resourceAllocOutput.resourceAssignmentBitMap;

         /* !
          * \code
          * Updating the Global RAT Info 
          * \endcode
          * */
         raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable -= \
             resourceAllocOutput.allocatedRB;
        if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
        {
            calculatedRNTP = calculateRNTP(ueContext_p->pA, harqProcess_p, internalCellIndex);
        }
         updateRATInfoFromRAT2LocalizedInfo(
             &bestVRBSpanInfo,
             raPolicyInfo_p ,
             resourceAllocOutput.allocatedRB,
             LTE_FALSE,
             calculatedRNTP,
             internalCellIndex); 
                  

          /*!
           * \code
           * call the callMuxOrEncoderNonSpatialMul function for Data Req.
           *     It will fill the DCI.For SPS_OCCASION Transmitter
           *     will use this only for DL SCH Pdu filling 
           * \endcode
           */
          callMuxOrEncoderNonSpatialMul(
               dlMsgTypeMsgInfo_p,
               dlStrategyTxNode_p,
               dlMsgInfo_p,
               harqProcess_p,
               dciCCEInfo_p,
               withMuxFlag,
               scheduledTick,
               ueContext_p,
               &resourceAllocOutput, 
               internalCellIndex
#ifdef TDD_CONFIG                                        
               /* + SPS_TDD */
               ,dlSubframeNum
               ,phyDelay
               ,sysFrame
               /* - SPS_TDD */
#endif 
               ); 
          /* SPR 18617 Fix Start */
          dciCCEInfo_p->dciPduInfo.ueCategory =
              ueContext_p->ueCategory;
          /* SPR 18617 Fix End */
          
          /* CA Stats Changes Start */
          LTE_MAC_UPDATE_STATS_FOR_SPS_PDSCH_RESERVE( ueContext_p, getCurrentTick(),
                  internalCellIndex);
          /* CA Stats Changes End */
         
          /* CA Stats Changes Start */
          LTE_MAC_UPDATE_STATS_FOR_PDSCH( ueContext_p,
                dlStrategyTxNode_p,
                harqProcess_p,
                dciCCEInfo_p,
                getCurrentTick(),excessPoolRb_g[internalCellIndex][CC_CE_REGION],
                internalCellIndex);
          /* CA Stats Changes End */
        }
        else
        {
            if( ! harqProcess_p->isTBOneValid )
            {
                LOG_UT( MAC_PDSCH_TB1_INVALID, LOGWARNING, MAC_PDCCH,
                        getCurrentTick(), __LINE__, 
                        dlStrategyTxNode_p ->ueIndex,
                        harqProcess_p->isTBOneValid,
                        harqProcess_p->isTBTwoValid, 0, 0, 0,
                        __func__, "");
            }
            else
            {
                LOG_UT( MAC_PDSCH_TB2_INVALID, LOGWARNING, MAC_PDCCH,
                        getCurrentTick(), __LINE__, 
                        dlStrategyTxNode_p ->ueIndex,
                        harqProcess_p->isTBOneValid,
                        harqProcess_p->isTBTwoValid, 0, 0, 0,
                        __func__, "");
            }
        }
    return MAC_SUCCESS;
}
/*Dynamic PDCCH + 5117 changes starts */
/****************************************************************************************
 * Function Name    : checkCodeRateAndUECategoryPostAllocation
 * Inputs           : scheduledTick - Scheduled TTI,
 *                    harqProcess_p - Pointer to DLHARQProcess,
 *                    numOfOverLappingRb - Number of overlapping RB's,
 *                    ueContext_p - Pointer to DLUEContext
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function  checks and pick the best MCS to be used from MCS map
 *                    and also checks the permissible TB size for UE category.
 *****************************************************************************************/ 
MacRetType checkCodeRateAndUECategoryPostAllocation(
                                                    /* SPR 15909 fix start */
                                                     tickType_t scheduledTick,
                                                    /* SPR 15909 fix end */
                                                     DLHARQProcess *harqProcess_p,
                                                     UInt8 numOfOverLappingRb,
                                                     DLUEContext *ueContext_p,
                                                     InternalCellIndex internalCellIndex)
{
    UInt8 mcsToBeUsedCW1 = 0;
    UInt8 mcsToBeUsedCW2 = 0;
    UInt8 subFrameIndex = 0;
    UInt32 totalTbsize = 0;
    /* SPR 12337 Fix  start */
    UInt8 mcsIndexCodeWordOne;
    UInt8 mcsIndexCodeWordTwo;
    /* SPR 12337 Fix end */
    DlUeCategoryInfo *dlUeCategoryInfo_p = cellSpecificParams_g.\
      cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->dlUeCategoryInfo;
    /* SPR 7468 changes start */
#ifdef TDD_CONFIG
    /* SPR 23393 Changes Start */
    UInt8 subframe = (scheduledTick ) % MAX_SUB_FRAME;
    UInt8 isSplSubFrame = checkForSpecialSubframe(subframe, internalCellIndex);
    /* SPR 23393 Changes End */
#endif
    /* SPR 7468 changes end */
    /* Cyclomatic_complexity_changes_start */
    /* +- SPR 17777 */
    calculateSubFrameIndex(scheduledTick, &subFrameIndex
#if defined (TDD_CONFIG) 
            ,internalCellIndex
#endif
            );
    /* +- SPR 17777 */
    
    updateMcsAndTBSizeforAssignedRBOne(subFrameIndex,
                                       harqProcess_p,
                                       ueContext_p);
    /* Cyclomatic_complexity_changes_end */
/*+- SPR_10086_FIX_END */
    {
        /*In case of RETX or in case of SF 0/5 
         * we need to reduce MCS only*/

        if ( NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator)
        {
            /* SPR 12337 Fix start */
            /* SPR23580 Start */
            mcsIndexCodeWordOne = harqProcess_p->dlHARQTBOneInfo.mcsIndex;
            /* SPR23580 End */
            /* SPR 12337 Fix end */
            calculateMCSToBeUsedForTB1(harqProcess_p, subFrameIndex,\
                    numOfOverLappingRb, ueContext_p,\
                    &mcsToBeUsedCW1,\
		    /* SPR 8537 Fix Start */
		    mcsIndexCodeWordOne  
	            /* SPR 8537 Fix End */
            ,internalCellIndex);
            if((mcsToBeUsedCW1 != INVALID_MCS_INDEX)&&\
                    (mcsToBeUsedCW1 != harqProcess_p->dlHARQTBOneInfo.mcsIndex))
            {
                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) ||
                    (ueContext_p->ueCategory == UE_CAT_8))&&
                    (harqProcess_p->numOfLayer == RANK_INDICATOR_4) )
                /* SPR 11355 Fix End */
                {
                    harqProcess_p->dlHARQTBOneInfo.tbSize = \
                                   determineTBSize(mcsToBeUsedCW1, 
						           harqProcess_p->assignedResourcesNB,
				                   TB_MAPPED_TO_TWO_LAYER);
                }
                /*4X4 DL MIMO CHG END*/
		        else
	            {
		            /*4X4 DL MIMO CHG START*/
                    harqProcess_p->dlHARQTBOneInfo.tbSize = \
                                       determineTBSize(mcsToBeUsedCW1, 
					  	               harqProcess_p->assignedResourcesNB,
					                   TB_MAPPED_TO_ONE_LAYER);
                    /*4X4 DL MIMO CHG END*/
		        }
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = mcsToBeUsedCW1;             
            }
#if (defined(TDD_CONFIG) && defined(REAL_UE_ENVIORNMENT))
            /*SPR 10978 Start*/
            /* During SSIT of TDD 0 - 6: 
             * In case of SplSubFrame MCS is getting reduced , Hence due to less tbSIze 
             * thruPut degradation happened to ~5%. So, the below logic has been commented out.
             */
            /*Following function is called for Real UE Env where UE is not able to decode DL data properly in Special subframe*/
            calculateMCSAndTBSizeForTDDSplSubFrameTB1(isSplSubFrame, harqProcess_p, ueContext_p);
            /*SPR 10978 End*/
#endif
        }
        else if ((RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator))
/*SPR_10086_FIX_START*/
/*&&
                (subFrameIndex != 2))
*/
/* SPR_10086_FIX_END */
        {
            /* Its possible that the RETX PDU is now 
             * violating coderate and it was fine when 
             * it was NEW-TX (when RE_TX falls on SF0/5.
             * In this case, omit this PDSCH transmission 
             * put in failure queue*/
            calculateMCSToBeUsedForTB1(harqProcess_p, subFrameIndex,\
                    numOfOverLappingRb, ueContext_p,\
                    &mcsToBeUsedCW1, \
		    /* SPR 8537 Fix Start */
		    harqProcess_p->dlHARQTBOneInfo.mcsIndex
		    /* SPR 8537 Fix End */
            ,internalCellIndex);
            if((mcsToBeUsedCW1 != INVALID_MCS_INDEX)&&\
                    (mcsToBeUsedCW1 < harqProcess_p->dlHARQTBOneInfo.mcsIndex))
            {
                lteWarning("****** Failure for TB1 RETX***** mapMCS[%d] allocated[%d] \n", 
				mcsToBeUsedCW1, harqProcess_p->dlHARQTBOneInfo.mcsIndex);
                return MAC_FAILURE;
            }
 
        }       
        /* SPR 7468 changes start */
#ifdef TDD_CONFIG
        if (isSplSubFrame)
        {
            UInt16 rbUsed = 0;
            /* SPR 18122 Changes Start*/
            rbUsed = DIVIDE_BY_FOUR(harqProcess_p->assignedResourcesNB * 3);
            /* SPR 18122 Changes End*/
            if (rbUsed < 1)
            {
                rbUsed = 1;
            }
                /* SPR 11355 Fix Start */
            if (((ueContext_p->ueCategory == UE_CAT_5)|| 
                (ueContext_p->ueCategory == UE_CAT_8))&&
                (harqProcess_p->numOfLayer == RANK_INDICATOR_4))
                /* SPR 11355 Fix End */
            {
            /*+COVERITY 5.3.0 - 52780*/
            if(mcsToBeUsedCW1 != INVALID_MCS_INDEX)
            /*-COVERITY 5.3.0 - 52780*/
            {
                harqProcess_p->dlHARQTBOneInfo.tbSize = 
                    determineTBSize(mcsToBeUsedCW1, rbUsed, TB_MAPPED_TO_TWO_LAYER);
/* SPR_10084_FIX_START */
            }
            }
            else
            {
       /*+COVERITY 5.3.0 - 52780*/
            if(mcsToBeUsedCW1 != INVALID_MCS_INDEX)
            /*-COVERITY 5.3.0 - 52780*/
            {
                harqProcess_p->dlHARQTBOneInfo.tbSize = 
                    determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex, rbUsed, TB_MAPPED_TO_ONE_LAYER);
/* SPR_10084_FIX_END */
            }

            }                               
        }
#endif
        /* SPR 7468 changes end */
        if(NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
        {
            /* SPR 12337 Fix  start */
            /* SPR23580 Start */
            mcsIndexCodeWordTwo = harqProcess_p->dlHARQTBTwoInfo.mcsIndex;
            /* SPR23580 End */
            /* SPR 12337 Fix end */
            calculateMCSToBeUsedForTB2(harqProcess_p, subFrameIndex,\
                    numOfOverLappingRb, ueContext_p,\
                    &mcsToBeUsedCW2, \
		    /* SPR 8537 Fix Start */
                    mcsIndexCodeWordTwo, 
		    /* SPR 8537 Fix End */
                    internalCellIndex);
            if((mcsToBeUsedCW2 != INVALID_MCS_INDEX)&&\
                    (mcsToBeUsedCW2 != harqProcess_p->dlHARQTBTwoInfo.mcsIndex))
            {
                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) ||
                    (ueContext_p->ueCategory == UE_CAT_8))&& 
                    ( (harqProcess_p->numOfLayer == RANK_INDICATOR_4) ||
                      (harqProcess_p->numOfLayer == RANK_INDICATOR_3) )  )
                /* SPR 11355 Fix End */
                {
                    harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                              determineTBSize(mcsToBeUsedCW2, harqProcess_p->assignedResourcesNB,
					      TB_MAPPED_TO_TWO_LAYER);
                }
		        else
                {
                    /*4X4 DL MIMO CHG START*/
                    harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                              determineTBSize(mcsToBeUsedCW2, harqProcess_p->assignedResourcesNB,
					      TB_MAPPED_TO_ONE_LAYER);
                    /*4X4 DL MIMO CHG END*/
		        }
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex = mcsToBeUsedCW2;
#if (defined(TDD_CONFIG) && defined(REAL_UE_ENVIORNMENT))
                /*SPR 10978 Start*/
                /* During SSIT 0f TDD 0 - 6: 
                 * In case of SplSubFrame MCS is getting reduced , Hence due to less tbSIze 
                 * thruPut degradation happened to ~5%. So, the below logic has been commented out.
                 */
                /*Following function is called for Real UE Env where UE is not able to decode DL data properly in Special subframe*/
                calculateMCSAndTBSizeForTDDSplSubFrameTB2(isSplSubFrame, harqProcess_p, ueContext_p);
                /*SPR 10978 End*/
#endif
            }
        }
        else if ((RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator))
/*SPR_10086_FIX_START */
/*&&
                (subFrameIndex != 2))
*/
/* SPR_10086_FIX_END */

        {
            /* Its possible that the RETX PDU is now 
             * violating coderate and it was fine when 
             * it was NEW-TX (when RE_TX falls on SF0/5.
             * In this case, omit this PDSCH transmission 
             * put in failure queue*/
            calculateMCSToBeUsedForTB2(harqProcess_p, subFrameIndex,\
                    numOfOverLappingRb, ueContext_p,\
                    &mcsToBeUsedCW2,\
		    /* SPR 8537 Fix Start */
		    harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
		    /* SPR 8537 Fix End */
            internalCellIndex);
            if((mcsToBeUsedCW2 != INVALID_MCS_INDEX)&&\
                    (mcsToBeUsedCW2 < harqProcess_p->dlHARQTBTwoInfo.mcsIndex))
            {
                lteWarning("****** Failure for TB2 RETX***** mapMCS[%d] allocated[%d] \n", 
                mcsToBeUsedCW2, harqProcess_p->dlHARQTBTwoInfo.mcsIndex);
                return MAC_FAILURE;
            }
        }
        /* SPR 7468 changes start */
#ifdef TDD_CONFIG
        if (isSplSubFrame)
        {
            UInt16 rbUsed = 0;
            /* SPR 18122 Changes Start*/
            rbUsed = DIVIDE_BY_FOUR(harqProcess_p->assignedResourcesNB * 3);
            /* SPR 18122 Changes End*/
            if (rbUsed < 1)
            {
                rbUsed = 1;
            }
                /* SPR 11355 Fix Start */
            if (((ueContext_p->ueCategory == UE_CAT_5) ||
                (ueContext_p->ueCategory == UE_CAT_8))&&
                (harqProcess_p->numOfLayer == RANK_INDICATOR_4))
                /* SPR 11355 Fix End */
            {
                   if(mcsToBeUsedCW2 != INVALID_MCS_INDEX)
            /*-COVERITY 5.3.0 - 52780*/
            {

/* SPR_10085_FIX_START */
                harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                            determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                            rbUsed,
                                           TB_MAPPED_TO_TWO_LAYER);

            }
            }
            else
            {
                if(mcsToBeUsedCW2 != INVALID_MCS_INDEX)
            /*-COVERITY 5.3.0 - 52780*/
            {
                harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                            determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                            rbUsed,
                                           TB_MAPPED_TO_ONE_LAYER);
/* SPR_10085_FIX_END */
            }
            }                               
        }
#endif
        /* SPR 7468 changes end */
    }
    if(harqProcess_p->isTBOneValid && harqProcess_p->isTBTwoValid)
    {
        ueContext_p->bothTbsize = harqProcess_p->dlHARQTBOneInfo.tbSize + 
					harqProcess_p->dlHARQTBTwoInfo.tbSize;
    }
    /* This case will happen in case of 1TB Ack, 1TB Nack. In this case,
     * coderate & ueCategory has been validated for NEW TB and RETX TB but
     * total DLSCH bits have to be validated here.*/

    /* SPR 6136 start */
    /*if((RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator) || \
            (RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)) */
    if ( ((RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator) &&
            (NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator))
            ||
            ((RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)&&
             (NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator)) )
        /* SPR 6136 end */
    {
            totalTbsize = (harqProcess_p->dlHARQTBOneInfo.tbSize << 3) + \
                          (harqProcess_p->dlHARQTBTwoInfo.tbSize << 3);

        while (totalTbsize > dlUeCategoryInfo_p[ueContext_p->ueCategory-1].maxDLSCHTbBits)
        {

             if(NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator)
             {
                /*coverity 52780*/
                if(harqProcess_p->dlHARQTBOneInfo.mcsIndex)
                /*coverity 52780*/
                {
                     harqProcess_p->dlHARQTBOneInfo.mcsIndex--;
                 }
                /*SPR 20950 Start*/
                else
                {
                    return MAC_FAILURE;
                }
                /*SPR 20950 End*/

                 /* SPR 7468 changes start */
#ifdef TDD_CONFIG
                 if(!isSplSubFrame)
                 {
                /* SPR 11355 Fix Start */
                     if (((ueContext_p->ueCategory == UE_CAT_5) ||
                         (ueContext_p->ueCategory == UE_CAT_8))&&
                          (harqProcess_p->numOfLayer == RANK_INDICATOR_4))
                /* SPR 11355 Fix End */
                     {
                         harqProcess_p->dlHARQTBOneInfo.tbSize = \
                            determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                            harqProcess_p->assignedResourcesNB,
                                            TB_MAPPED_TO_TWO_LAYER);
                     }
                     else
                     {
                         harqProcess_p->dlHARQTBOneInfo.tbSize = \
                                determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                harqProcess_p->assignedResourcesNB,
                                TB_MAPPED_TO_ONE_LAYER);
                     }

                 }
                 else
                 {
                     UInt16 rbUsed = 0;
                    /* SPR 18122 Changes Start*/
                    rbUsed = DIVIDE_BY_FOUR(harqProcess_p->assignedResourcesNB * 3);
                    /* SPR 18122 Changes End*/
                /* SPR 11355 Fix Start */
                     if (((ueContext_p->ueCategory == UE_CAT_5) ||
                         (ueContext_p->ueCategory == UE_CAT_8))&&
                         (harqProcess_p->numOfLayer == RANK_INDICATOR_4))
                /* SPR 11355 Fix End */
                     {
                        harqProcess_p->dlHARQTBOneInfo.tbSize = \
                           determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                           rbUsed,TB_MAPPED_TO_TWO_LAYER);
                     }
                     else
                     {
                        harqProcess_p->dlHARQTBOneInfo.tbSize = \
                           determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                           rbUsed,TB_MAPPED_TO_ONE_LAYER);
                     }
                 }
#else
                /* SPR 11355 Fix Start */
                 if (((ueContext_p->ueCategory == UE_CAT_5) ||
                     (ueContext_p->ueCategory == UE_CAT_8)) &&
                         (harqProcess_p->numOfLayer == RANK_INDICATOR_4))
                /* SPR 11355 Fix End */
                 {
                     harqProcess_p->dlHARQTBOneInfo.tbSize = \
                       determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
		 	            harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_TWO_LAYER);
                 }
                 else
                 {
                     harqProcess_p->dlHARQTBOneInfo.tbSize = \
                       determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
		 	            harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_ONE_LAYER);
                 }
                 /*4X4 DL MIMO CHG END*/
#endif
             }
             else if(NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
             {
                /*coverity 52780*/
                if(harqProcess_p->dlHARQTBTwoInfo.mcsIndex)
                /*coverity 52780*/
                {
                    harqProcess_p->dlHARQTBTwoInfo.mcsIndex--;
                }
                /*SPR 20950 Start*/
                else
                {
                    return MAC_FAILURE;
                }
                /*SPR 20950 End*/
                
                 /* SPR 7468 changes start */
#ifdef TDD_CONFIG
                 if(!isSplSubFrame)
                 {
                /* SPR 11355 Fix Start */
                     if (((ueContext_p->ueCategory == UE_CAT_5) ||
                         (ueContext_p->ueCategory == UE_CAT_8)) &&
                         ((harqProcess_p->numOfLayer == RANK_INDICATOR_4)
                          ||(harqProcess_p->numOfLayer == RANK_INDICATOR_3)))
                /* SPR 11355 Fix End */
                     {
                         harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                         determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                         harqProcess_p->assignedResourcesNB,
                                         TB_MAPPED_TO_TWO_LAYER);
                     }
                     else
                     {
                         harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                         determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                         harqProcess_p->assignedResourcesNB,
                                         TB_MAPPED_TO_ONE_LAYER);
                     }
                 }
                 else
                 {
                     UInt16 rbUsed = 0;
                    /* SPR 18122 Changes Start*/
                    rbUsed = DIVIDE_BY_FOUR(harqProcess_p->assignedResourcesNB * 3);
                    /* SPR 18122 Changes End*/
                /* SPR 11355 Fix Start */
                     if (((ueContext_p->ueCategory == UE_CAT_5) ||
                         (ueContext_p->ueCategory == UE_CAT_8)) &&
                         ((harqProcess_p->numOfLayer == RANK_INDICATOR_4) ||
                          (harqProcess_p->numOfLayer == RANK_INDICATOR_3)))
                /* SPR 11355 Fix End */
                     {
                         harqProcess_p->dlHARQTBTwoInfo.tbSize =
                              determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                              rbUsed,TB_MAPPED_TO_TWO_LAYER);
                     }
                     else
                     {
                         harqProcess_p->dlHARQTBTwoInfo.tbSize = 
                              determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                              rbUsed,TB_MAPPED_TO_ONE_LAYER);
                     }
                 }
#else
                 /* SPR 7468 changes start */
                 /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                 if( ((ueContext_p->ueCategory == UE_CAT_5) ||
                     (ueContext_p->ueCategory == UE_CAT_8)) && 
                     ( (harqProcess_p->numOfLayer == RANK_INDICATOR_4)
                      || (harqProcess_p->numOfLayer == RANK_INDICATOR_3) )  )
                /* SPR 11355 Fix End */
                 {
                     harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                         determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex, 
		                  harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_TWO_LAYER);
                 }
                 /*4X4 DL MIMO CHG END*/
		         else
                 {
                     /*4X4 DL MIMO CHG START*/
                     harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                             determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex, 
                                             harqProcess_p->assignedResourcesNB,
                                             TB_MAPPED_TO_ONE_LAYER);
                 /* SPR 7468 changes end */
                 }
#endif
            }
            totalTbsize = (harqProcess_p->dlHARQTBOneInfo.tbSize << 3) + \
                          (harqProcess_p->dlHARQTBTwoInfo.tbSize << 3);
        }
    }
    ueContext_p->numOfOverLapRbs = numOfOverLappingRb;
    return MAC_SUCCESS;

}
/*Dynamic PDCCH + 5117 changes ends */
/****************************************************************************************
 * Function Name    : allocRBInPDSCHForData
 * Inputs           : withMuxFlag - This when set to one Indicates that we need to call
 *                                  directly invoke Mux Execution Logic,
 *                    macAdvanceFactor,
 *                    dlMsgTypeMsgInfo_p - This stores DLMsgTypeMsgInfo Pointer which
 *                                         store the information of those UE for which
 *                                         the PDCCH region has been allocated. It is a
 *                                         Pointer to DLMsgTypeMsgInfo,
 *                    dciCCEContaineInfo_p - This stores the DciCCEInfo pointer which
 *                                           stores the information of the CCE on
 *                                           which Mux or Encoder. So that they can
 *                                           write relevant information on this,
 *                    countMsg - It stores the count of DL Message whose PDCCH Region
 *                               has been allocated,
 *                    raPolicyInfo_p - Pointer to ResourceAllocationPolicyInfo,
 *                    scheduledTick - This holds the Tick Value in which UE is
 *                                    expected to be scheduled,
 *                    containerIndex - It stores the containerIndex which will be used
 *                                     by Encoder,
 *                    msgIndentifier - It stores the msgIndentifier of the Data Msg,
 *                    allocSPSFirst,
 *                    maxUEsToBeScheduledDL_p - maximum number of UEs can be scheduled
 *                                              in 1 tick for DL,
 *                    phyDelay - PHY Delay,
 *                    subFrame - Current subframe,
 *                    dlSubframeNum - Downlink subframe number,
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : Based on the Message type this funtions will invole funtions for
 *                    allocating RBs and if allocation is successful, this will populate
 *                    the DCI CCE Info Structure.
 *****************************************************************************************/ 
/** SPR 13599 Changes Start **/
STATIC  void allocRBInPDSCHForData(
        UInt32 withMuxFlag,
        /* + CQI_5.0 */
        UInt32 macAdvanceFactor,
        /* - CQI_5.0 */
        DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        DciCCEContainerInfo *dciCCEContaineInfo_p,
        UInt32 countMsg,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt32 containerIndex,
        UInt32 msgIndentifier,
       UInt8 allocSPSFirst,
        /* ICIC changes start */
        UInt8 *maxUEsToBeScheduledDL_p
        /* ICIC changes end */
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
        ,UInt32 phyDelay
/* +- SPR 17777 */
        ,UInt8 dlSubframeNum
        ,UInt32 sysFrame
#endif
        ,InternalCellIndex internalCellIndex
        )
/** SPR 13599 Changes End **/
{
    DLHARQProcess *harqProcess_p = PNULL;   
    ResourceAllocatorInput *resourceAllocInput_p = PNULL;  
    ResourceAllocatorOutput resourceAllocOutput ;    
    RATPolicy ratPolicy ;
    DciCCEInfo *dciCCEInfo_p = PNULL;
    DLDataMsgInfo *dlMsgInfo_p = PNULL;  
    DLUEContext *ueContext_p = PNULL;
    EncoderNodeInfo encoderNodeInfo;
    DLUEContextInfo *dlUEContextInfo_p = PNULL;        
    DLStrategyTxNode *dlStrategyTxNode_p = PNULL;
#ifdef FDD_CONFIG
    ULUEContext *ulUEContext_p = PNULL;
#endif
    /* SPR 4168  */
    MacRetType retType = MAC_SUCCESS;
    /* SPR 23402 Changes Start */ 
    /*SPR 10411 changes start */ 
#ifdef KPI_STATS
    /*MOCN SPR 21913 Start*/                                                                      
    LteMacKPIStats  *pLteMacKPIStats       = PNULL;
    UInt32          activeIndexForMacStats = 0;
    UInt8 qci = 0;
    /*MOCN SPR 21913 End*/                                                                      
#endif
    /*SPR 10411 changes end */ 
    /* SPR 23402 Changes End */
    /* SPR 4168  */
#ifdef OVERSHOOT_CORRECTION
    UInt32 avaialbleRBCount= (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB;
    /* +- SPR 17777 */
#endif
    SInt32 allocatedRBCount = raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable;
#ifdef FDD_CONFIG
    UInt32 harqSubFrameNum  = 0;
#endif
   /* UInt8 allocationFailureCause=0;TDD Warning Fix */
    UInt32 totalRb = 0;
    /* ICIC changes start */
    UInt32 resourceRegionAllocated = CC_CE_REGION;
    UInt32 availableRBs = 0;
    UInt32 maxallocatedRB = 0;
    /* ICIC changes end */
    /*Dynamic PDCCH + 5117 changes starts */
    UInt8 numOfOverLappingRb = 0;
    /*Dynamic PDCCH + 5117 changes ends */
    LP_SpsDlInfo dlSpsInfo_p = PNULL;
    /*START_DRX_OPT*/
    DRXContext   *drxContext_p = PNULL;
    /*END_DRX_OPT*/
    /*SPR 9325 fix start */
    UInt8       ta_drx_extra_hrd = 0;
    /*SPR 9325 fix end */
    /* CA Changes start */
    UInt64 currentDLPRBUsage = 0;
    /* CA Changes end */ 
/* SPR 15446 Fix Start */
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
/* SPR 15446 Fix End */
/* + SPR 11107 Changes */
#ifdef FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t scheduledExpiryTick = 0;
    /* SPR 15909 fix end */
    /* + SPS_TDD */
#endif        
    SchType schUsed = DRA_SCHEDULING;
    /* - SPS_TDD */
/* - SPR 11107 Changes */
#ifdef OVERSHOOT_CORRECTION
     /* CA changes start */ 
     modifiedNoOfUEsAfterOverLoadDL(maxUEsToBeScheduledDL_p, internalCellIndex);
     /* CA changes start */ 
     if(avaialbleRBCount < raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable )
     {
          allocatedRBCount = avaialbleRBCount;
     }
     else
     {
           allocatedRBCount = raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable;
     }
#endif


#ifdef TDD_CONFIG
    /* SPR 23393 Changes Start */
    UInt8 subframe = (scheduledTick ) % MAX_SUB_FRAME;
    UInt8 isSplSubFrame = checkForSpecialSubframe(subframe,internalCellIndex ); 
    /* SPR 23393 Changes End */
#endif
    DLStrategyTxNode** ueDlStrategyTxNode_p = PNULL;
    /* SPR 18298 Fix Start */
    /* +- */
    /* SPR 18298 Fix End */

    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "Entering function = %s  \n",  __func__);
    dlMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.dlDataMsgInfo;      

    totalRb = 0;

    /*Below While Loop is executing number of times = number of UE for which 
      PDCCH Region has been allocated.For Each UE it call funtion
      allocRBInPDSCHForNonSpatialMul ( For UE having TM Mode Other than 3 and 4 )
      or allocRBInPDSCHForSpatialMul (For UE having TM Mode 3 and 4 ) and checks 
      if RBs can be reserved for this UE .If true it will  funtions which will 
      call funtion callMuxOrEncoderSpatialMul or callMuxOrEncoderNonSpatialMul
    */
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "countMsg is= %u\n",countMsg);

    while ( countMsg-- )
    {
        /* SPR 18298 Fix Start */
        /* Reset the ResourceAllocatorOuput structure */
        memSet( &resourceAllocOutput, 0, sizeof(resourceAllocOutput) );
        /* SPR 18298 Fix End */
        dlStrategyTxNode_p = dlMsgInfo_p->dlStrategyTxNode_p;
        dlUEContextInfo_p = &dlUECtxInfoArr_g[dlStrategyTxNode_p->ueIndex];

#ifdef FDD_CONFIG
        ulUEContext_p = ulUECtxInfoArr_g[dlStrategyTxNode_p->ueIndex].ulUEContext_p;
        ulUEContext_p->isPdcchAllocated = 0;
#endif
        LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                "Restting isPdcchAllocated for UE = %d",dlStrategyTxNode_p->ueIndex);
        ueContext_p = dlUEContextInfo_p->dlUEContext_p; 
        ueContext_p->isPdcchAllocatedFlag = 0;                   
        /*START_DRX_OPT*/
        drxContext_p =&(ueContext_p->drxContext);
        /*END_DRX_OPT*/
        dciCCEInfo_p = &dciCCEContaineInfo_p->dciCCEInfo[dlMsgInfo_p->cceIndex];

        if( IS_PCELL_OF_UE(ueContext_p,internalCellIndex ))
        {
            ueDlStrategyTxNode_p = &(ueContext_p->dlStrategyTxNode_p);
            /* + SPR 11469 Changes */
            harqProcess_p = &ueContext_p->dlHarqContext_p->harqProcess[
                dlStrategyTxNode_p->harqProcessId];
            /* - SPR 11469 Changes */
            /* + SPR 11491 Changes */
            ueContext_p->isPdcchAllocatedFlag = 0;                   
            /* - SPR 11491 Changes */
        }
        else
        {
            ueDlStrategyTxNode_p = 
                &(ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p);
            /* + SPR 11469 Changes */
            harqProcess_p = &ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p->harqProcess[
                dlStrategyTxNode_p->harqProcessId];
            /* - SPR 11469 Changes */
            /* + SPR 11491 Changes */
            ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->isPdcchAllocatedFlag = 0;                   
            /* - SPR 11491 Changes */
        }

        /* SPS_CHG_START */
        /* !
         * \ fn static void allocRBInPDSCHForData(
         *         UInt32 withMuxFlag,
         *         DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
         *         DciCCEContainerInfo *dciCCEContaineInfo_p,
         *         UInt32 countMsg,
         *         ResourceAllocationPolicyInfo *raPolicyInfo_p,
         *         tickType_t scheduledTick,
         *         Int32 containerIndex,
         *         UInt32 msgIndentifier)
         * \brief Function Modification
         * \code
         * - Check if((SPS_OCCASION == dlMsgTypeMsgInfo_p->msgType) &&
         *                 (dlMsgInfo_p->cceIndex >= MAX_CCE_INDEX)||
         *                 (SPS_ACTIVATION == dlMsgTypeMsgInfo_p->msgType))
         *          - Call the Function allocRBInPDSCHForSPSData
         *          - allocRBInPDSCHForSPSData Function will 
         *              - Get the SPS Resource Resreved for SPS Occasion
         *              from Ue Context.
         *              - Update the harqProcess
         *              - Update the Resource Assignment bitmap
         *              - Update the Global RAT Info 
         *              - Call the MuxOrEncoderNonSpatialMul function
         *                It will fill the DCI.For SPS_OCCASION Transmitter 
         *                will use this only for DL SCH Pdu filling .
         *
         * \endcode
         * */
        /* SPR 16766 fix Start */
        if (drxContext_p->drxStateVariables.drxCurrentState == DRX_STATE_ON)
        {
            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                    LOGWARNING,MAC_UL_Strategy,\
                    getCurrentTick(),\
                    ueContext_p->ueIndex,\
                    harqProcess_p->dlHARQTBOneInfo.txIndicator,harqProcess_p->dlHARQTBTwoInfo.txIndicator,\
                    harqProcess_p->dlHARQTBOneInfo.current_TX_NB,ueContext_p->crnti,\
                    0.0,0.0,__func__,
                    "UeSchDRXdl_STATE_ON");
        }
        /* SPR 16766 fix End */
        if(((SPS_OCCASION == dlMsgTypeMsgInfo_p->msgType) && 
              (dlMsgInfo_p->cceIndex >= MAX_CCE_INDEX))||
              (SPS_ACTIVATION == dlMsgTypeMsgInfo_p->msgType))
        {
            if (MAC_SUCCESS == allocRBInPDSCHForSPSData(
                        withMuxFlag,
                        dlMsgTypeMsgInfo_p,
                        dlStrategyTxNode_p,
                        ueContext_p,
                        harqProcess_p,
                        dciCCEInfo_p,
                        dlMsgInfo_p,
                        raPolicyInfo_p,
                        scheduledTick,
                        allocSPSFirst,
                        /* + SPS_TDD */
#ifdef TDD_CONFIG
                        dlSubframeNum,
                        phyDelay,
                        sysFrame,
#endif
                        /* - SPS_TDD */
                        internalCellIndex))
            {
                if(SPS_ACTIVATION == dlMsgTypeMsgInfo_p->msgType) 
                {
                    dlSpsInfo_p = &dlUEContextInfo_p->dlUEContext_p->spsDlInfo; 
                    (dlSpsInfo_p->numDciAttempts)++;	
                    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                    /* Update DAI and store it in DCI Info */
                    dciCCEInfo_p->dciPduInfo.dai = 
                        /* SPR 609 changes start */
                        /* + SPS_TDD_Changes */
                        updateDai(ueContext_p,
                                /* SPR 609 changes end */
                                scheduledTick,
                                /* SPR 23393 Changes Start */
                                scheduledTick % MAX_SUB_FRAME,
                                /* SPR 23393 Changes End */
                                internalCellIndex);
#endif
                }
                else
                {
#ifdef TDD_CONFIG
                    /* Update DAI and store it in DCI Info */
                    dciCCEInfo_p->dciPduInfo.dai = 
                        /* SPR 609 changes start */
                        /* + SPS_TDD_Changes */
                        updateDaiSPS(ueContext_p,
                                /* SPR 609 changes end */
                                scheduledTick,
                                /* SPR 23393 Changes Start */
                                scheduledTick % MAX_SUB_FRAME,
                                /* SPR 23393 Changes End */
                                internalCellIndex);
#endif
                }
                /* - SPS_TDD_Changes */

                freeMemPool(dlStrategyTxNode_p);
                /* SPR# 9910 fix start */
                ueContext_p->dlStrategyTxNode_p = PNULL;
                /* SPR# 9910 fix end */
                dciCCEInfo_p->msgIdentifier = msgIndentifier;
                msgIndentifier++;
                /*Incrementing the pointer so that now it will point to
                  next Msg*/
                dlMsgTypeMsgInfo_p++;
                dlMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.
                    dlDataMsgInfo;     
                allocatedRBCount -= \
                                    ueContext_p->spsDlInfo.resAllocInfo.allocatedRB;                           
                totalRb += \
                           ueContext_p->spsDlInfo.resAllocInfo.allocatedRB;

                 /* SPR 15103 changes start */
                 dciCCEInfo_p->dciPduInfo.ueCategory = ueContext_p->ueCategory;
                 /* SPR 15103 changes end */

                (*maxUEsToBeScheduledDL_p)--;
                /*START_DRX_OPT*/
                if (SETUP == ueContext_p->drxConfigType && 
                        SPS_ACTIVATION == dlMsgTypeMsgInfo_p->msgType &&
                        drxContext_p->drxConfig.isdrxMacCeCmdConfigured &&
                        (!drxContext_p->drxStateVariables.isDrxMacCeTimerRunning)
                   )
                /*END_DRX_OPT*/
                {
                    /* SPR 12075 Changes Start */
                    putEntryInDRXDLSchedulerEventQueue
                        (ueContext_p->ueIndex, scheduledTick,
                         ueContext_p->internalCellIndex);
                    /* SPR 12075 Changes End */
                }
                /*START_DRX_OPT*/
                if (SETUP == ueContext_p->drxConfigType)
                {
                    LOG_MAC_MSG(MAC_DRX_EVENT_TRIGGERED_LOG_ID, LOGDEBUG, MAC_DRX,
                    getCurrentTick(), ueContext_p->ueIndex, DRX_EVENT_DL_DATA_SCHEDULED,
                    ueContext_p->drxContext.drxStateVariables.drxCurrentState,
                    ueContext_p->drxContext.drxStateVariables.drxSubState,
                    ueContext_p->drxContext.drxStateVariables.drxNextCycle,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,__func__,"");
                } 
                /*END_DRX_OPT*/
            }
            continue;
        }
        /*! \code
         * - Check if(SPS_DEACTIVATION == dlMsgTypeMsgInfo_p->msgType)
         *     - Populate the DCI CCE Info with relvant Informtion
         *     - Free the dlStrategyTxNode_p.
         *     - Increment the dciCCEInfo_p Message Identifier.
         *     - DCI Pdu Filling will be taken care in Transmitter.
         *     - Decrement the  maxUEsToBeScheduledDL.
         * \endcode
         * */
        else if(SPS_DEACTIVATION == dlMsgTypeMsgInfo_p->msgType)
        {
            if((harqProcess_p->isTBOneValid) && 
                    (! harqProcess_p->isTBTwoValid)) 
            {
                /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                /* Update DAI and store it in DCI Info */
                dciCCEInfo_p->dciPduInfo.dai = 
                    /* SPR 609 changes start */
                /* + SPS_TDD_Changes */
                    updateDai(ueContext_p,
                            /* SPR 609 changes end */
                            scheduledTick,
                            /* SPR 23393 Changes Start */
                            scheduledTick % MAX_SUB_FRAME,
                            /* SPR 23393 Changes End */
                            internalCellIndex);
#endif
                /* - SPS_TDD_Changes */
                resourceAllocOutput.ratPolicy = RA_POLICY_2_LOCALIZED;
                resourceAllocOutput.resourceAssignmentBitMap = 0xFFFFFFFF;

                /* Updating the  harqProcess_p */
                harqProcess_p->ratPolicyUsed = RA_POLICY_2_LOCALIZED;
                harqProcess_p->dlHARQTBOneInfo.tbSize = 0; 
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = 0xFF;
                harqProcess_p->assignedResourcesNB = 0;
                harqProcess_p->dciFormat =  \
                                            dlMsgInfo_p->dciFormat = DCI_FORMAT_1A;
                /* SPR 19704 + */
                harqProcess_p->isSpsOnlyPaddingPresent = LTE_TRUE;
                /* SPR 19704 - */

                /* Populating the Encoder node */
                POPULATE_ENCODER_NODE_INFO(encoderNodeInfo ,
                        scheduledTick , dlStrategyTxNode_p ,dlMsgInfo_p);

                /*Populating the DCI CCE Info with relvant Informtion*/
                POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_MUL_DATA_MSG(
                        dlMsgTypeMsgInfo_p->msgType, dciCCEInfo_p ,
                        (&resourceAllocOutput), dlMsgInfo_p ,ueContext_p ,
                        harqProcess_p,ueContext_p->dlMIMOInfo.dlRi,
                        scheduledTick,internalCellIndex);
                /* SPR 18617 Fix Start */
                dciCCEInfo_p->dciPduInfo.ueCategory =
                    ueContext_p->ueCategory;
                /* SPR 18617 Fix End */
#ifdef LTE_EMBMS_SUPPORTED
                if(isCurrentSfEmbmsReserved_g[internalCellIndex])
                {
                    dciCCEInfo_p->dciPduInfo.pA -= \
                    (macReconfigSchedulerParamsDL_gp[internalCellIndex] + \
                    schParamsIndexDL_g[internalCellIndex])->\
                    eMbmsReservedCellPowerOffset;  
                } 
#endif


                dlSpsInfo_p = &dlUEContextInfo_p->dlUEContext_p->spsDlInfo; 
                (dlSpsInfo_p->numDciAttempts)++;	

                freeMemPool(dlStrategyTxNode_p);
                /* SPR# 9910 fix start */
                ueContext_p->dlStrategyTxNode_p = PNULL;
                /* SPR# 9910 fix end */
                dciCCEInfo_p->msgIdentifier = msgIndentifier;
                msgIndentifier++;
                /* SPR 15103 changes start */
                dciCCEInfo_p->dciPduInfo.ueCategory = ueContext_p->ueCategory;
                /* SPR 15103 changes end */
                /*Incrementing the pointer so that now it will point to
                  next Msg*/
                dlMsgTypeMsgInfo_p++;
                dlMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.
                    dlDataMsgInfo;     
                (*maxUEsToBeScheduledDL_p)--;
            }
            else
            {
                if( ! harqProcess_p->isTBOneValid )
                {
                    LOG_UT( MAC_PDSCH_TB1_INVALID, LOGWARNING, MAC_PDCCH,
                            getCurrentTick(), __LINE__, 
                            dlStrategyTxNode_p->ueIndex,
                            harqProcess_p->isTBOneValid,
                            harqProcess_p->isTBTwoValid, 0, 0, 0,
                            __func__, "");
                }
                else
                {
                    LOG_UT( MAC_PDSCH_TB2_INVALID, LOGWARNING, MAC_PDCCH,
                            getCurrentTick(), __LINE__, 
                            dlStrategyTxNode_p->ueIndex,
                            harqProcess_p->isTBOneValid,
                            harqProcess_p->isTBTwoValid, 0, 0, 0,
                            __func__, "");
                }
            }
#ifdef FDD_CONFIG
            harqSubFrameNum = (scheduledTick + FDD_HARQ_OFFSET) % MAX_SUB_FRAME;
            /* SPR 11804 fix start */
            scheduledExpiryTick = scheduledTick  + 8;

            dlHarqTimerStart( ueContext_p->ueIndex,
                    harqProcess_p->harqProcessId,
                    harqSubFrameNum,
                    scheduledExpiryTick,
                    SPS_SCHEDULING,
                    internalCellIndex);
            /* SPR 11804 fix end */
            /* + SPS_TDD_Changes */
#else
            /* Harqtimer start for sps tdd*/
            tddDlHarqTimerStart( ueContext_p->ueIndex,
                    harqProcess_p->harqProcessId,
                    dlSubframeNum,
                    scheduledTick,
                    sysFrame,
                    SPS_SCHEDULING,
                    internalCellIndex
                    );
            /* - SPS_TDD_Changes */
#endif
            continue;
        }
        if ((allocatedRBCount > 0) && (*maxUEsToBeScheduledDL_p > 0))
        {
            resourceAllocInput_p =  &dlStrategyTxNode_p->resourceAllocInput;
            
            /* ICIC changes start */
            resourceRegionAllocated = 
                resourceAllocInput_p->resourceRegionAllocated;
            /*Calling funtion decideRATPolicy which will return RAT Policy to be
              Used by Resource Allocator*/
            if (CC_CE_REGION == resourceRegionAllocated)
            {
		    /* UE_CAPAB_ADD_PARAM Start */
		    decideRATPolicy(
				    resourceAllocInput_p->requiredRB,
				    dlMsgInfo_p->dciFormat,
				    dlStrategyTxNode_p->transmissionType,
				    &ratPolicy,
				    raPolicyInfo_p,
				    harqProcess_p,
                    ueContext_p->Rat1Support); 
		    /* UE_CAPAB_ADD_PARAM End */
            }
            else
            {
		    /* UE_CAPAB_ADD_PARAM Start */
		    /* For deciding RAT policy for cell edge users */
		    decideRATPolicyCellEdge(
				    resourceAllocInput_p->requiredRB,
				    dlMsgInfo_p->dciFormat,
				    dlStrategyTxNode_p->transmissionType,
				    &ratPolicy,
				    raPolicyInfo_p,
				    harqProcess_p,
                    ueContext_p->Rat1Support); 
		    /* UE_CAPAB_ADD_PARAM End */
            }
            
            /* Check to find last UE in the list to be scheduled 
             * In this case all the available Rbs can be given to this UE 
             * till its max TB size is reached 
             */
            if ((!countMsg) && (TRUE == lastUEDetectFlag_g[internalCellIndex]) &&
                  /* + SPR 5422 Fix */ 
                 (RE_TX != dlStrategyTxNode_p->transmissionType)) 
                  /* - SPR 5422 Fix */ 
            {
                if ((dlStrategyTxNode_p->maxTBSize >
                            dlStrategyTxNode_p->minTBSize) &&
                        (excessPoolRb_g[internalCellIndex][resourceRegionAllocated] > 0))
                {
                    if (CC_CE_REGION == resourceRegionAllocated)
                    {
                        availableRBs = raPolicyInfo_p->rat2LocalizedInfo.
                            numOfVRBAvailable;
                    }
                    else
                    {
                        availableRBs = raPolicyInfo_p->rat2LocalizedInfo.
                            numOfCellEdgeVRBAvailable;
                    }

                    /* For last UE change required RB value such that excess Pool 
                     * RBs are used to satisfy its max TB size
                     */
                    maxallocatedRB = 
                        allocateRBForMaxTBSize(harqProcess_p,
                                resourceAllocInput_p->requiredRB,
                                availableRBs,
                                dlStrategyTxNode_p->maxTBSize,
                                resourceRegionAllocated,
                                /** CA 2 Core Changes Start **/ 
                                internalCellIndex);
                                /** CA 2 Core Changes End**/ 

                    /* Update the excess pool */
                    UPDATE_EXCESS_POOL_RB(resourceRegionAllocated, 
                            (resourceAllocInput_p->requiredRB - maxallocatedRB),internalCellIndex);

                    resourceAllocInput_p->requiredRB = maxallocatedRB;
                }
                lastUEDetectFlag_g[internalCellIndex] = FALSE;
            }
            /* ICIC changes end */
            
            resourceAllocInput_p->preferredRATPolicy = ratPolicy;           
            dciCCEInfo_p->dciPduInfo.resrcAllocType = ratPolicy;

            /* SPR 903 Start*/
            dciCCEInfo_p->dciPduInfo.ueCategory =
                                    ueContext_p->ueCategory;
            /* SPR 903 End */

            LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                    "RAT Policy is= %u\n",ratPolicy);

	    /*Dynamic PDCCH + 5117 changes starts */
            numOfOverLappingRb = 0;
	    /*Dynamic PDCCH + 5117 changes ends */


            switch ( dlMsgTypeMsgInfo_p->msgType )
            {        
                case  SPATIAL_MUL:
                case  TA_WITH_DATA_SPATIAL_MUL:
                case  TA_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL:
                case  SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL:
                /* + SPR 22352 fix */
                case  DRX_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL:
                case  TA_DRX_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL:
                /* - SPR 22352 fix */
                    {


                        /* SPR 3679 changes Start */
                        /* SPR 6936 changes start */
                        if(((harqProcess_p->isTBOneValid) || 
                            (harqProcess_p->isTBTwoValid)) &&
                           (resourceAllocInput_p->requiredRB))
                        /* SPR 6936 changes end */
                        /* SPR 3679 changes End */
                        {
                            /*Calling  allocRBInPDSCHForSpatialMul funtion which will 
                              allocate RBs to UE*/
                            if ( MAC_SUCCESS ==  allocRBInPDSCHForSpatialMul(
                                   /* + CQI_5.0 */
                                   macAdvanceFactor,
                                   /* - CQI_5.0 */
                                    resourceAllocInput_p,
                                    &resourceAllocOutput,
                                    ueContext_p,
                                    harqProcess_p,
                                    raPolicyInfo_p,
                                    dlStrategyTxNode_p
                                    /*Dynamic PDCCH + 5117 changes starts */
                                    ,scheduledTick,
                                    &numOfOverLappingRb
                                    /*Dynamic PDCCH + 5117 changes ends */
#ifdef TDD_CONFIG                                    
                                    ,subframe
#endif                                    
                                        ,internalCellIndex
                                    ) 
                                )
                            {
                                /* SPR 16766 fix Start */
                                /* +- SPR 17779 */
                                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID_1, LOGDEBUG, 
                                        MAC_UL_Strategy, getCurrentTick(),
                                        ueContext_p->ueIndex,
                                        harqProcess_p->dlHARQTBOneInfo.txIndicator,harqProcess_p->dlHARQTBTwoInfo.txIndicator,
                                        harqProcess_p->dlHARQTBOneInfo.current_TX_NB,ueContext_p->crnti,
                                        0,0,__func__, "UeSchDRXdl_2tb");
                                /* SPR 16766 fix end */
                                
                                harqProcess_p->dciFormat =  dlMsgInfo_p->dciFormat;
                                /*Dynamic PDCCH + 5117 changes starts */
                                numOvlp_g = numOfOverLappingRb;
                                /* + TM7_8 Changes Start */
                                if(((ueContext_p->dlMIMOInfo.transmissionMode == TX_MODE_7)&&(harqProcess_p->dciFormat == DCI_FORMAT_1))||
                                        ((ueContext_p->dlMIMOInfo.transmissionMode == TX_MODE_8)&&(harqProcess_p->dciFormat == DCI_FORMAT_2B)))
                                {
                                    /*SPR 19029 Start*/
                                    updateMCSIndexAndTBSIzeForTransMode_7_8(harqProcess_p,internalCellIndex);                               
                                    /*SPR 19029 End*/
                                }
                                else
                                {
                                    /* SPR 15446 Fix Start */
                                    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->dlAvailableRBs <= 50 && ueContext_p->ueCategory == 2)||
                                            /* SPR 21631 Start */
                                        (scheduledTick % MAX_SUB_FRAME == 0) || (scheduledTick % MAX_SUB_FRAME == 5))
                                            /* SPR 21631 End */
                                    {
                                    if(MAC_FAILURE == checkCodeRateAndUECategoryPostAllocation(scheduledTick,
                                                harqProcess_p,
                                                numOfOverLappingRb,
                                                ueContext_p,
                                                internalCellIndex))
                                    {
                                        lteWarning("--------checkCodeRateAndUECategoryPostAllocation Failed------\n");
                                        break;
                                    }
                                    /* + TM7_8 Changes Start */
                                    }
                                    /* SPR 15446 Fix End */
                                }
                                /*Dynamic PDCCH + 5117 changes ends */
                                /*SPR 9325 fix start */
                                
                                /* + SPR 22352 fix */
                                switch (dlMsgTypeMsgInfo_p->msgType)
                                {
                                    case TA_WITH_DATA_SPATIAL_MUL:     
                                    case SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL:     
        	                         ta_drx_extra_hrd = TWO_BYTES; 
                                         break;
                                    case DRX_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL: 
                                         ta_drx_extra_hrd = THREE_BYTES; 
                                         break;
                                    case TA_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL:
                                         ta_drx_extra_hrd = FOUR_BYTES; 
                                         break;
                                    case TA_DRX_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL:
                                         ta_drx_extra_hrd = FIVE_BYTES; 
                                         break;
                                    default: 
                                         ta_drx_extra_hrd = 0;
                                }
                                /* - SPR 22352 fix */

                                if (((harqProcess_p->isTBOneValid)&&
                                     (harqProcess_p->dlHARQTBOneInfo.tbSize <= (UInt32)(MAC_MIN_SDU_SIZE+ta_drx_extra_hrd)) &&
                                     (NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator))||
                                    ((harqProcess_p->isTBTwoValid) &&
                                     (harqProcess_p->dlHARQTBTwoInfo.tbSize <= (UInt32)(MAC_MIN_SDU_SIZE+ta_drx_extra_hrd)) &&
                                     (NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)))
                                {
                                     break;
                                }
                                /*SPR 9325 fix end */
#ifdef PERF_STATS
                                /* Update the Counter for the Number of PRBs used in DL per
                                 * cell for HARQ retransmissions */
                                if (RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator ||
                                        RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
                                {
									/* + PERF_CA */
                                    gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedDLPerfStats.\
                                        totalDLPRBUsageforHarqRetx +=  \
                                        resourceAllocInput_p->requiredRB;
									/* - PERF_CA */
                                    /* SPR 23402 Changes Start */
                                    /*MOCN SPR 21913 Start*/                                                                      
#ifdef KPI_STATS
                                    activeIndexForMacStats = (gMacStats.
                                            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
                                    pLteMacKPIStats = &(gMacStats.
                                            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
                                    for (qci = 1; qci <= MAX_QCI;qci++)
                                    {
                                        pLteMacKPIStats->totalDLPRBUsagePerQCI[qci-1] += ueContext_p->qciPerTBSize[qci-1];
                                        if(mocnOperatorParams_g.isMocnEnabled && (ueContext_p->operatorId < mocnOperatorParams_g.numOfOperator))
                                        {
                                            pLteMacKPIStats->totalDLPRBUsagePerOperatorPerQCI[ueContext_p->operatorId][qci-1] += ueContext_p->qciPerTBSizePerOperatorPerQCI[ueContext_p->operatorId][qci-1] ;
                                        }
                                    }
#endif
                                    /*MOCN SPR 21913 End*/     
                                    /* SPR 23402 Changes End */                                                                 
                                }

                                CALCULATE_DL_RESOURCE_ELEMENTS(resourceAllocOutput.allocatedRB,internalCellIndex);
#endif
                                retType = callMuxOrEncoderSpatialMul(
                                        dlMsgTypeMsgInfo_p, dlStrategyTxNode_p,
                                        dlMsgInfo_p, harqProcess_p,
                                        dciCCEInfo_p, withMuxFlag, 
                                        scheduledTick, ueContext_p,
                                        &resourceAllocOutput,
                                        internalCellIndex
#ifdef TDD_CONFIG                                        
                                        ,dlSubframeNum
                                        ,phyDelay
                                        ,sysFrame
#endif 
                                    );
                                if (MAC_FAILURE == retType)
                                {
                                    if ( (TA_WITH_DATA_SPATIAL_MUL ==
                                            dlMsgTypeMsgInfo_p->msgType) || 
                                         (TA_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL ==
                                            dlMsgTypeMsgInfo_p->msgType))
                                    {
                                        POPULATE_ENCODER_NODE_INFO(encoderNodeInfo ,
                                                scheduledTick , dlStrategyTxNode_p ,dlMsgInfo_p);

                                        putEntryInSchEncoderQueue(dlStrategyTxNode_p->ueIndex, 
                                                &encoderNodeInfo, containerIndex,internalCellIndex);   
                                    }
                                    else
                                    {
                                        break;
                                    }

                               }

                                /* SPR 4168  */

                                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                                        "Updating lastScheduledTick = %d",scheduledTick);
                                /* ICIC changes start */
                                LOG_UT (MAC_PDSCH_BUFF_POOL_ALLOC_DETAILS,LOGDEBUG,MAC_PDSCH,\
                                        getCurrentTick(),\
                                        dlBufferPool_g[internalCellIndex].totalRBs,\
                                        dlBufferPool_g[internalCellIndex].cellEdgeRBs,\
                                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                                        DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                                        "Values of Buffer Pool after RB allocation to UE");

                                LOG_UT (MAC_PDSCH_UE_SCHEDULING_INFO,LOGDEBUG,\
                                        MAC_PDSCH, getCurrentTick(),\
                                        ueContext_p->ueIndex,\
                                        harqProcess_p->harqProcessId,\
                                        resourceAllocOutput.allocatedRB,\
                                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                        DEFAULT_FLOAT_VALUE,\
                                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                                        "UE Scheduling Information");
                                LOG_UT (MAC_PDSCH_UE_SCHEDULING_DETAILS,LOGDEBUG,\
                                        MAC_PDSCH, getCurrentTick(),\
                                        ueContext_p->userLocationType,\
                                        harqProcess_p->dlHARQTBOneInfo.txIndicator,\
                                       (dlRAInfo_gp[internalCellIndex]+
                                       dlRAInfoIndex_g[internalCellIndex])->
                                       raPolicyInfo.rat2LocalizedInfo.numOfCellEdgeVRBAvailable, \
                                      (dlRAInfo_gp[internalCellIndex] + 
                                      dlRAInfoIndex_g[internalCellIndex])->
                                      raPolicyInfo.rat2LocalizedInfo.firstCellEdgeFreeIndex ,\
                                      ( dlRAInfo_gp[internalCellIndex] + 
                                      dlRAInfoIndex_g[internalCellIndex])->\
                                      raPolicyInfo.rat2LocalizedInfo.endCellEdgeFreeIndex,\

                                        DEFAULT_FLOAT_VALUE,\
                                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                                        "UE Scheduling DATAILS");
                                /* CA Stats Changes Start */
                                /* ICIC changes end */
                                LTE_MAC_UPDATE_STATS_FOR_PDSCH( ueContext_p,
                                        dlStrategyTxNode_p,
                                        harqProcess_p,
                                        dciCCEInfo_p,
                                        /*spr 9052 fix start*/
                                        getCurrentTick(),
                                        /*spr 9052 fix end*/ 
                                        excessPoolRb_g[internalCellIndex][resourceRegionAllocated],
                                        internalCellIndex);
                                /* CA Stats Changes End */
                                freeMemPool(dlStrategyTxNode_p);
                                /* SPR 4906 Changes Starts  */
                                dlStrategyTxNode_p = PNULL;
                                *ueDlStrategyTxNode_p = PNULL;
                                /* SPR 4906 Changes Ends  */
                                dciCCEInfo_p->msgIdentifier = msgIndentifier;
                                msgIndentifier++;
                                /*Incrementing the pointer so that now it will point to
                                  next Msg*/
                                dlMsgTypeMsgInfo_p++;
                                dlMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.
                                    dlDataMsgInfo; 
                                allocatedRBCount -= resourceAllocOutput.allocatedRB;          
                                totalRb += resourceAllocOutput.allocatedRB;

                                (*maxUEsToBeScheduledDL_p)--;
                                /*SPR 3679 Qos Changes Start*/
                                totalRbMIMO_g[internalCellIndex] = 0;
                                *ueDlStrategyTxNode_p = PNULL;
                                /*SPR 3679 Qos Changes End*/
                                /*END:6172*/
#ifdef TDD_CONFIG
                               /* + SPR_14039 */
                                if ((!IS_CA_ENABLED()) || ( (IS_CA_ENABLED()) && 
                                        (RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator ||
                                        RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)))
                                {

                                    /* Update DAI and store it in DCI Info */
                                    dciCCEInfo_p->dciPduInfo.dai = 
                                        updateDai(ueContext_p,
                                                scheduledTick,
                                                subframe,
                                                internalCellIndex);
                                }
                              /* - SPR_14039 */
#endif
                                /* DRX_CHG */
                                /*START_DRX_OPT*/
                                if (SETUP == ueContext_p->drxConfigType &&
                                    drxContext_p->drxConfig.isdrxMacCeCmdConfigured &&
                                    (!drxContext_p->drxStateVariables.isDrxMacCeTimerRunning)
                                   )
                                /*END_DRX_OPT*/
                                {
                                    /* SPR 12075 Changes Start */
                                    putEntryInDRXDLSchedulerEventQueue
                                        (ueContext_p->ueIndex, scheduledTick,
                                         ueContext_p->internalCellIndex);
                                    /* SPR 12075 Changes End */
                                }
                                /*START_DRX_OPT*/
                                if (SETUP == ueContext_p->drxConfigType)
                                {
                                    LOG_MAC_MSG(MAC_DRX_EVENT_TRIGGERED_LOG_ID, LOGDEBUG, MAC_DRX,
                                    getCurrentTick(), ueContext_p->ueIndex, DRX_EVENT_DL_DATA_SCHEDULED,
                                    ueContext_p->drxContext.drxStateVariables.drxCurrentState,
                                    ueContext_p->drxContext.drxStateVariables.drxSubState,
                                    ueContext_p->drxContext.drxStateVariables.drxNextCycle,
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,__func__,"");
                                } 
                                /*END_DRX_OPT*/
                                /* DRX_CHG */
                                continue;
                            }   
                        }
                        else
                        {
                            LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                                    "ERROR TB One Invalid\n");
                        }                
                    }
                    break;
                case NON_SPATIAL_MUL:
                case  TA_WITH_DATA_NON_SPATIAL_MUL:
                /* SPS_CHG */
                case SPS_RETX:
                /* SPS_CHG */
/*START : DRX_MAC_CE*/
                case TA_DRX_WITH_DATA_NON_SPATIAL_MUL:
                case DRX_WITH_DATA_NON_SPATIAL_MUL:
/*END   : DRX_MAC_CE*/
                case TA_SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL:
                case SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL:
                /* + SPR 22352 fix */
                case  DRX_SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL:
                case  TA_DRX_SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL:
                /* - SPR 22352 fix */
                    {
                        /* SPR 3679 Changes Start */
                        /* SPR 6936 changes start */
                        if(((harqProcess_p->isTBOneValid) || 
                            (harqProcess_p->isTBTwoValid)) &&
                           (resourceAllocInput_p->requiredRB))
                        /* SPR 6936 changes end */
                        /* SPR 3679 Changes End */
                        {
                            /*Calling  allocRBInPDSCHForNonSpatialMul funtion which will 
                              allocate RBs to UE*/
                            if ( MAC_SUCCESS ==  allocRBInPDSCHForNonSpatialMul(
                                    resourceAllocInput_p,
                                    &resourceAllocOutput,ueContext_p,
                                    harqProcess_p,raPolicyInfo_p,
                                    dlStrategyTxNode_p
                                    /*Dynamic PDCCH + 5117 changes starts */
                                    ,scheduledTick,
                                    &numOfOverLappingRb
                                    /*Dynamic PDCCH + 5117 changes ends */

#ifdef TDD_CONFIG									
                                    , subframe
#endif									
                                        ,internalCellIndex) )
                            {
                                /* SPR 16766 fix Start */
                                /* +- SPR 17779 */
                                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID_1, LOGDEBUG, 
                                        MAC_UL_Strategy, getCurrentTick(),
                                        ueContext_p->ueIndex,
                                        harqProcess_p->dlHARQTBOneInfo.txIndicator,harqProcess_p->dlHARQTBTwoInfo.txIndicator,
                                        harqProcess_p->dlHARQTBOneInfo.current_TX_NB,ueContext_p->crnti,
                                        0,0,__func__, "UeSchDRXdl_1tb");
                                /* SPR 16766 fix end */
                               /*Dynamic PDCCH + 5117 changes starts */
                                numOvlp_g = numOfOverLappingRb;
                                /* + TM7_8 Changes Start */
                                if(((ueContext_p->dlMIMOInfo.transmissionMode == TX_MODE_7)&&(harqProcess_p->dciFormat == DCI_FORMAT_1))||
                                        ((ueContext_p->dlMIMOInfo.transmissionMode == TX_MODE_8) && (harqProcess_p->dciFormat == DCI_FORMAT_2B)))
                                {
                                    /*SPR 19029 Start*/
                                    updateMCSIndexAndTBSIzeForTransMode_7_8(harqProcess_p,internalCellIndex);                               
                                    /*SPR 19029 End*/
                                }
                                else
                                {
                                    /* - TM7_8 Changes End */
                                    /* SPR 15446 Fix Start */
                                    if (!(cellParams_p->dlAvailableRBs <= 50 && ueContext_p->ueCategory == 2)||
                                            /* SPR 23393 Changes Start */
                                         (scheduledTick % MAX_SUB_FRAME == 0) || (scheduledTick % MAX_SUB_FRAME == 5))
                                        /* SPR 23393 Changes End */
                                    {
                                    if(MAC_FAILURE == checkCodeRateAndUECategoryPostAllocation(scheduledTick,
                                                harqProcess_p,
                                                numOfOverLappingRb,
                                                ueContext_p,
                                                internalCellIndex))
                                    {
                                        break;
                                    }
                                    }
                                    /* SPR 15446 Fix End */
                                    /* + TM7_8 Changes Start */
                                }
                                /* - TM7_8 Changes End */

                               /*Dynamic PDCCH + 5117 changes ends */
                                /*SPR 9325 fix start */
                                switch (dlMsgTypeMsgInfo_p->msgType)
                                {
                                    case DRX_WITH_DATA_NON_SPATIAL_MUL:
                                         ta_drx_extra_hrd = ONE_BYTE; 
                                         break;
                                    /* + SPR 22352 fix */
                                    case TA_WITH_DATA_NON_SPATIAL_MUL:     
                                    case SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL:     
                                         ta_drx_extra_hrd = TWO_BYTES; 
                                         break;
                                    case TA_DRX_WITH_DATA_NON_SPATIAL_MUL: 
                                    case DRX_SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL: 
                                         ta_drx_extra_hrd = THREE_BYTES; 
                                         break;
                                    case TA_SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL:
                                         ta_drx_extra_hrd = FOUR_BYTES; 
                                         break;
                                    case TA_DRX_SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL:
                                         ta_drx_extra_hrd = FIVE_BYTES; 
                                         break;
                                    /* - SPR 22352 fix */
                                    default: 
                                         ta_drx_extra_hrd = 0;
                                }
                                if (((harqProcess_p->isTBOneValid)&&
                                     (harqProcess_p->dlHARQTBOneInfo.tbSize <= (UInt32)(MAC_MIN_SDU_SIZE+ta_drx_extra_hrd)) &&
                                     (NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator))||
                                    ((harqProcess_p->isTBTwoValid) &&
                                     (harqProcess_p->dlHARQTBTwoInfo.tbSize <= (UInt32)(MAC_MIN_SDU_SIZE+ta_drx_extra_hrd)) &&
                                     (NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)))
                                {
                                     break;
                                }
                                /*SPR 9325 fix end */
                                harqProcess_p->dciFormat =  dlMsgInfo_p->dciFormat;
#ifdef PERF_STATS
                                /* Update the Counter for the Number of PRBs used in DL per
                                 * cell for HARQ retransmissions */
                                if (RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator ||
                                        RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
                                {
                                    gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedDLPerfStats.\
                                        totalDLPRBUsageforHarqRetx +=  \
                                        resourceAllocInput_p->requiredRB;
                                    /* SPR 23402 Changes Start */
                                    /*MOCN SPR 21913 Start*/                                                                      
#ifdef KPI_STATS
                                    activeIndexForMacStats = (gMacStats.
                                            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
                                    pLteMacKPIStats = &(gMacStats.
                                            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
                                    for (qci = 1; qci <= MAX_QCI;qci++)
                                    {
                                        pLteMacKPIStats->totalDLPRBUsagePerQCI[qci-1] += ueContext_p->qciPerTBSize[qci-1];
                                        if(mocnOperatorParams_g.isMocnEnabled && (ueContext_p->operatorId < mocnOperatorParams_g.numOfOperator))
                                        {
                                            pLteMacKPIStats->totalDLPRBUsagePerOperatorPerQCI[ueContext_p->operatorId][qci-1] += ueContext_p->qciPerTBSizePerOperatorPerQCI[ueContext_p->operatorId][qci-1] ;
                                        }
                                    }
#endif
                                    /*MOCN SPR 21913 End*/  
                                    /* SPR 23402 Changes End */                                                                    
                                }
                                CALCULATE_DL_RESOURCE_ELEMENTS(resourceAllocOutput.allocatedRB,internalCellIndex);
#endif 
/*TDD Tick overshoot changes start*/                
                               /* CA_phase2_harq_code Start */
                                retType = callMuxOrEncoderNonSpatialMul(
                                            dlMsgTypeMsgInfo_p,
                                            dlStrategyTxNode_p,
                                            dlMsgInfo_p,
                                            harqProcess_p,
                                            dciCCEInfo_p,
                                            withMuxFlag,
                                            scheduledTick,
                                            ueContext_p,
                                            &resourceAllocOutput,
                                            internalCellIndex
                                            
#ifdef TDD_CONFIG                                        
                                            ,dlSubframeNum
                                            ,phyDelay
                                            ,sysFrame
#endif 
                                            );
                                if(retType == MAC_FAILURE)
                                {
                                    /* SPR 16831 Fix Start */
                                    if (TA_WITH_DATA_NON_SPATIAL_MUL ==
                                            dlMsgTypeMsgInfo_p->msgType)
                                    {
                                        POPULATE_ENCODER_NODE_INFO(encoderNodeInfo ,
                                                scheduledTick , dlStrategyTxNode_p ,dlMsgInfo_p);

                                        putEntryInSchEncoderQueue(dlStrategyTxNode_p->ueIndex, 
                                                &encoderNodeInfo, containerIndex,internalCellIndex);   
                                    }
                                    else
                                    {
                                        break;
                                    }
                                    /* SPR 16831 Fix End */
                                }
                                /* CA_phase2_harq_code End */
/*TDD Tick overshoot changes end*/

                                /*START : DRX_MAC_CE*/
                                if(harqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG)
                                {
                                    harqProcess_p->dlHARQTBOneInfo.taDrxFlag = INVALID_TA_DRX_MSG;
                                }
                                /*END   : DRX_MAC_CE*/
                                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                                        "Updating lastScheduledTick = %d",scheduledTick);
                                /* ICIC changes start */
                                LOG_UT (MAC_PDSCH_BUFF_POOL_ALLOC_DETAILS,LOGDEBUG,MAC_PDSCH,\
                                        getCurrentTick(),\
                                        dlBufferPool_g[internalCellIndex].totalRBs,\
                                        dlBufferPool_g[internalCellIndex].cellEdgeRBs,\
                                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                                        DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                                        "Values of Buffer Pool after RB allocation to UE");

                                LOG_UT (MAC_PDSCH_UE_SCHEDULING_INFO,LOGDEBUG,\
                                        MAC_PDSCH, getCurrentTick(),\
                                        ueContext_p->ueIndex,\
                                        harqProcess_p->harqProcessId,\
                                        resourceAllocOutput.allocatedRB,\
                                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                        DEFAULT_FLOAT_VALUE,\
                                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                                        "UE Scheduling Information");

                                LOG_UT (MAC_PDSCH_UE_SCHEDULING_DETAILS,LOGDEBUG,\
                                        MAC_PDSCH, getCurrentTick(),\
                                        ueContext_p->userLocationType,\
                                        harqProcess_p->dlHARQTBOneInfo.txIndicator,\
                                        (dlRAInfo_gp[internalCellIndex] + 
                                        dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo. rat2LocalizedInfo.numOfCellEdgeVRBAvailable,\
                                       (dlRAInfo_gp[internalCellIndex] + 
                                       dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat2LocalizedInfo.firstCellEdgeFreeIndex,\
                                        (dlRAInfo_gp[internalCellIndex] + 
                                        dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat2LocalizedInfo.endCellEdgeFreeIndex,\
                                        DEFAULT_FLOAT_VALUE,\
                                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                                        "UE Scheduling DATAILS");
                                /* CA Stats Changes Start */
                                /* ICIC changes end */
                                LTE_MAC_UPDATE_STATS_FOR_PDSCH( ueContext_p,
                                        dlStrategyTxNode_p,
                                        harqProcess_p,
                                        dciCCEInfo_p,
                                        /*spr 9052 fix start*/
                                        getCurrentTick(),
                                        /*spr 9052 fix end*/ 
                                        excessPoolRb_g[internalCellIndex][resourceRegionAllocated],
                                        internalCellIndex);
                                /* CA Stats Changes End */
                                freeMemPool(dlStrategyTxNode_p);
                                /* SPR 4906 Changes Starts  */
                                dlStrategyTxNode_p = PNULL;
                                *ueDlStrategyTxNode_p = PNULL;
                                /* SPR 4906 Changes Ends  */
                                dciCCEInfo_p->msgIdentifier = msgIndentifier;
                                msgIndentifier++;
                                /*Incrementing the pointer so that now it will point to
                                  next Msg*/
                                dlMsgTypeMsgInfo_p++;
                                dlMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.
                                dlDataMsgInfo;     
                                allocatedRBCount -= resourceAllocOutput.allocatedRB;                           
                                totalRb += resourceAllocOutput.allocatedRB;

                                (*maxUEsToBeScheduledDL_p)--;
#ifdef TDD_CONFIG
                                /* + SPR_14039 */
                                if ((!IS_CA_ENABLED()) || ((IS_CA_ENABLED()) && 
                                        (RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator ||
                                        RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)))
                                {

                                    /* Update DAI and staore the updated DAI in DCI Info */
                                    dciCCEInfo_p->dciPduInfo.dai = updateDai(ueContext_p,
                                            scheduledTick,
                                            subframe,
                                            internalCellIndex);
                                }
                              /* - SPR_14039 */
#endif 
                                /* DRX_CHG */
                                /*START_DRX_OPT*/
                                if (SETUP == ueContext_p->drxConfigType &&
                                    drxContext_p->drxConfig.isdrxMacCeCmdConfigured &&
                                    (!drxContext_p->drxStateVariables.isDrxMacCeTimerRunning)
                                   )
                                /*END_DRX_OPT*/
                                {
                                    /* SPR 12075 Changes Start */
                                    putEntryInDRXDLSchedulerEventQueue
                                        (ueContext_p->ueIndex, scheduledTick,
                                         ueContext_p->internalCellIndex);
                                    /* SPR 12075 Changes End */
                                }
                                /*START_DRX_OPT*/
                                if (SETUP == ueContext_p->drxConfigType)
                                {
                                    LOG_MAC_MSG(MAC_DRX_EVENT_TRIGGERED_LOG_ID, LOGDEBUG, MAC_DRX,
                                    getCurrentTick(), ueContext_p->ueIndex, DRX_EVENT_DL_DATA_SCHEDULED,
                                    ueContext_p->drxContext.drxStateVariables.drxCurrentState,
                                    ueContext_p->drxContext.drxStateVariables.drxSubState,
                                    ueContext_p->drxContext.drxStateVariables.drxNextCycle,
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,__func__,"");
                                } 
                                /*END_DRX_OPT*/
                                /* DRX_CHG */

                                continue;
                            }
                        }
                        else
                        {
                            if( ! harqProcess_p->isTBOneValid )
                            {
                                LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                                        "ERROR TB One Invalid \n");
                            }
                            else
                            /*QOS_KLOCKWORK_FIX*/
                            {
                                LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                                        "ERROR TB Two Valid \n");
                            }
                            /*QOS_KLOCKWORK_FIX*/
                        }
                    }
                    break;           
                case  TA_SPATIAL_MUL:
/*START : DRX_MAC_CE*/
                case  TA_DRX_SPATIAL_MUL:
                case  DRX_SPATIAL_MUL:
/*END   : DRX_MAC_CE*/
                /* + SPR 12118 Changes */
                case SCELL_ACT_DEACT_SPATIAL_MUL:
                /* - SPR 12118 Changes */
                    /* SPR 22352 fix */
                case TA_SCELL_ACT_DEACT_SPATIAL_MUL:
                case DRX_SCELL_ACT_DEACT_SPATIAL_MUL:
                case TA_DRX_SCELL_ACT_DEACT_SPATIAL_MUL:
                    /* SPR 22352 fix */
                    {
                        /*checkCodeRateUECategory(
                                raPolicyInfo_p, 
                                scheduledTick,
                                harqProcess_p,
                                ueContext_p,
                                TRUE);*/

                        /*Calling  allocRBInPDSCHForSpatialMul funtion which will 
                          allocate RBs to UE*/
                        /* SPR 6936 changes start */
                        if(((harqProcess_p->isTBOneValid) && 
                            (! harqProcess_p->isTBTwoValid)) &&
                           (resourceAllocInput_p->requiredRB))
                        /* SPR 6936 changes end */
                        {
                            if ( MAC_SUCCESS ==  allocRBInPDSCHForSpatialMul(
                                    /* + CQI_5.0 */
                                    macAdvanceFactor,
                                    /* - CQI_5.0 */
                                    resourceAllocInput_p,
                                    &resourceAllocOutput,ueContext_p,
                                    harqProcess_p,raPolicyInfo_p,
                                    dlStrategyTxNode_p
                                     /*Dynamic PDCCH + 5117 changes starts*/
                                    ,scheduledTick,
                                    &numOfOverLappingRb
                                     /*Dynamic PDCCH + 5117 changes ends */
#ifdef TDD_CONFIG
                                    ,subframe
#endif
                                        ,internalCellIndex
                                    )
                                )

                            {
                                /*Dynamic PDCCH + 5117 changes starts */
                                /* + TM7_8 Changes Start */
                                if(((ueContext_p->dlMIMOInfo.transmissionMode == TX_MODE_7)&&(harqProcess_p->dciFormat == DCI_FORMAT_1))||
                                        ((ueContext_p->dlMIMOInfo.transmissionMode == TX_MODE_8)&&(harqProcess_p->dciFormat == DCI_FORMAT_2B)))
                                {
                                    /*SPR 19029 Start*/
                                    updateMCSIndexAndTBSIzeForTransMode_7_8(harqProcess_p,internalCellIndex);                               
                                    /*SPR 19029 End*/
                                }
                                else
                                {
                                    /* - TM7_8 Changes End */
                                    if(MAC_FAILURE == checkCodeRateAndUECategoryPostAllocation(scheduledTick,
                                                harqProcess_p,
                                                numOfOverLappingRb,
                                                ueContext_p,
                                                internalCellIndex))
                                    {
                                        break;
                                    }
                                    /* + TM7_8 Changes Start */
                                }
                               /* - TM7_8 Changes End */
                               /*Dynamic PDCCH + 5117 changes ends */
#ifdef PERF_STATS
                                /* Update the Counter for the Number of PRBs used in DL per
                                 * cell for HARQ retransmissions */
                                if (RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator ||
                                        RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
                                {
									/* + PERF_CA */
                                    gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedDLPerfStats.\
                                        totalDLPRBUsageforHarqRetx +=  \
                                        resourceAllocInput_p->requiredRB;
									/* - PERF_CA */
                                   /* SPR 23402 Changes Start */ 
                                   /*MOCN SPR 21913 Start*/                                                                      
#ifdef KPI_STATS
                                    activeIndexForMacStats = (gMacStats.
                                            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
                                    pLteMacKPIStats = &(gMacStats.
                                            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
                                    for (qci = 1; qci <= MAX_QCI;qci++)
                                    {
                                        pLteMacKPIStats->totalDLPRBUsagePerQCI[qci-1] += ueContext_p->qciPerTBSize[qci-1];
                                        if(mocnOperatorParams_g.isMocnEnabled && (ueContext_p->operatorId < mocnOperatorParams_g.numOfOperator))
                                        {
                                            pLteMacKPIStats->totalDLPRBUsagePerOperatorPerQCI[ueContext_p->operatorId][qci-1] += ueContext_p->qciPerTBSizePerOperatorPerQCI[ueContext_p->operatorId][qci-1] ;
                                        }
                                    }
#endif
                                    /*MOCN SPR 21913 End*/
                                    /* SPR 23402 Changes End */                                                                      
                                }
                                CALCULATE_DL_RESOURCE_ELEMENTS(resourceAllocOutput.allocatedRB,internalCellIndex);
#endif

                                harqProcess_p->dciFormat =  dlMsgInfo_p->dciFormat;
                                /*Populating the encoderNodeInfo structure which wiill be
                                  be used by callMuxOrEncoderSpatialMul funtion for its 
                                  procesing*/
                                POPULATE_ENCODER_NODE_INFO(encoderNodeInfo ,
                                        scheduledTick , dlStrategyTxNode_p ,dlMsgInfo_p);
                                 /* + CQI_5.0 */
                                /*Populating the the DCI CCE Info with relvant Informtion*/  
                                /*POPULATE_DCI_CCE_STRUCT_SPATIAL_MUL_DATA_MSG(
                                        dlMsgTypeMsgInfo_p->msgType, dciCCEInfo_p ,
                                        (&resourceAllocOutput),dlMsgInfo_p ,ueContext_p ,
                                        harqProcess_p,ueContext_p->dlMIMOInfo.dlRi,
                                        scheduledTick);*/
                                /* - CQI_5.0 */
                                if( harqProcess_p->isTBOneValid && 
                                        NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator)
                                {
#ifdef TDD_CONFIG
                                    /* Cyclomatic_complexity_changes_start */
                                    setSubFrameTypeForDlHarq(isSplSubFrame,harqProcess_p);
                                    /* Cyclomatic_complexity_changes_end */
#endif            
                                    /*SPR 1388 changes start */
                                    dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer = 0; 
                                    if(harqProcess_p->isTBTwoValid)
                                    {    
                                        LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                                                "TB Two is Valid \n");
                                    }    
                                    /*SPR 1388 changes end */
                                    dciCCEInfo_p->dciPduInfo.tb1Info.tbSize = harqProcess_p->
                                        dlHARQTBOneInfo.tbSize;
                                }
                                else
                                {
                                    if ( harqProcess_p->isTBOneValid )
                                    {
                                        LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                                                " Retransmission of TB One \n");
                                    }
                                }

                                putEntryInSchEncoderQueue(dlStrategyTxNode_p->ueIndex, 
                                        &encoderNodeInfo, containerIndex,internalCellIndex); 
                                /* ICIC changes start */
                                LOG_UT (MAC_PDSCH_BUFF_POOL_ALLOC_DETAILS,LOGDEBUG,MAC_PDSCH,\
                                        getCurrentTick(),\
                                        dlBufferPool_g[internalCellIndex].totalRBs,\
                                        dlBufferPool_g[internalCellIndex].cellEdgeRBs,\
                                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                                        DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                                        "Values of Buffer Pool after RB allocation to UE");

                                LOG_UT (MAC_PDSCH_UE_SCHEDULING_INFO,LOGDEBUG,\
                                        MAC_PDSCH, getCurrentTick(),\
                                        ueContext_p->ueIndex,\
                                        harqProcess_p->harqProcessId,\
                                        resourceAllocOutput.allocatedRB,\
                                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                        DEFAULT_FLOAT_VALUE,\
                                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                                        "UE Scheduling Information");

                                LOG_UT (MAC_PDSCH_UE_SCHEDULING_DETAILS,LOGDEBUG,\
                                        MAC_PDSCH, getCurrentTick(),\
                                        ueContext_p->userLocationType,\
                                        harqProcess_p->dlHARQTBOneInfo.txIndicator,\
                                        (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat2LocalizedInfo.numOfCellEdgeVRBAvailable,\
                                        (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.
                                        rat2LocalizedInfo.firstCellEdgeFreeIndex,\
                                       ( dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.
                                        rat2LocalizedInfo.endCellEdgeFreeIndex,\
                                        DEFAULT_FLOAT_VALUE,\
                                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                                        "UE Scheduling DATAILS");
                                /* CA Stats Changes Start */
                                /* ICIC changes end */ 
                                LTE_MAC_UPDATE_STATS_FOR_PDSCH( ueContext_p,
                                        dlStrategyTxNode_p,
                                        harqProcess_p,
                                        dciCCEInfo_p,
                                        /*spr 9052 fix start*/
                                        getCurrentTick(),
                                        /*spr 9052 fix end*/
                                        excessPoolRb_g[internalCellIndex][resourceRegionAllocated],
                                        internalCellIndex);
                                /* CA Stats Changes End */
                                freeMemPool(dlStrategyTxNode_p);
                             /** SPR 14502 changes start **/
                                dlMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.
                                                                          dlDataMsgInfo;           
    /* +- SPR 17777 */
                                POPULATE_DCI_CCE_STRUCT_SPATIAL_MUL_DATA_MSG(
                                                dlMsgTypeMsgInfo_p->msgType,dciCCEInfo_p,
                                                &resourceAllocOutput,dlMsgInfo_p,ueContext_p,
                                                harqProcess_p,
                                                scheduledTick,internalCellIndex);
    /* +- SPR 17777 */
                             /** SPR 14502 changes end **/   

                                dlStrategyTxNode_p = PNULL;
                                *ueDlStrategyTxNode_p = PNULL;
#ifdef LTE_EMBMS_SUPPORTED
                             if(isCurrentSfEmbmsReserved_g[internalCellIndex])
                             {
                                 dciCCEInfo_p->dciPduInfo.pA -= \
                                 (macReconfigSchedulerParamsDL_gp[internalCellIndex] + \
                                 schParamsIndexDL_g[internalCellIndex])->eMbmsReservedCellPowerOffset;  
                             } 
#endif
 
                                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                                        "Updating lastScheduledTick = %d",scheduledTick);
                                dciCCEInfo_p->msgIdentifier = msgIndentifier;
                                msgIndentifier++;
                                /*Incrementing the pointer so that now it will point to
                                  next Msg*/
                                dlMsgTypeMsgInfo_p++;

                                (*maxUEsToBeScheduledDL_p)--;
                                totalRb += resourceAllocOutput.allocatedRB;

                                dlMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.
                                    dlDataMsgInfo;           
                                allocatedRBCount -= resourceAllocOutput.allocatedRB;                            
#ifdef TDD_CONFIG
                             
                               /* + SPR_14039 */
                                /* Update DAI and store the updated DAI in DCI Info */
                                if ((!IS_CA_ENABLED()) || ( (IS_CA_ENABLED()) &&
                                        (RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator ||
                                        RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)))
                                {
                                    dciCCEInfo_p->dciPduInfo.dai = updateDai(ueContext_p,
                                        scheduledTick,
                                        subframe,
                                        internalCellIndex);
                                }
                             /* + SPR_14039 */
#endif
                                /* DRX_CHG */
                                /*START_DRX_OPT*/
                                if (SETUP == ueContext_p->drxConfigType &&
                                    drxContext_p->drxConfig.isdrxMacCeCmdConfigured &&
                                    (!drxContext_p->drxStateVariables.isDrxMacCeTimerRunning)
                                   )
                                /*END_DRX_OPT*/
                                {
                                    /* SPR 12075 Changes Start */
                                    putEntryInDRXDLSchedulerEventQueue
                                        (ueContext_p->ueIndex, scheduledTick,
                                         ueContext_p->internalCellIndex);
                                    /* SPR 12075 Changes End */
                                }
                                /*START_DRX_OPT*/
                                if (SETUP == ueContext_p->drxConfigType)
                                {
                                    LOG_MAC_MSG(MAC_DRX_EVENT_TRIGGERED_LOG_ID, LOGDEBUG, MAC_DRX,
                                    getCurrentTick(), ueContext_p->ueIndex, DRX_EVENT_DL_DATA_SCHEDULED,
                                    ueContext_p->drxContext.drxStateVariables.drxCurrentState,
                                    ueContext_p->drxContext.drxStateVariables.drxSubState,
                                    ueContext_p->drxContext.drxStateVariables.drxNextCycle,
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,__func__,"");
                                } 
                                /*END_DRX_OPT*/
                                /* DRX_CHG */
/* + SPR 11107 Changes */
#ifdef FDD_CONFIG
                                harqSubFrameNum = (scheduledTick + FDD_HARQ_OFFSET )%MAX_SUB_FRAME;
                                scheduledExpiryTick = scheduledTick  + 8;

                                if (SPS_ACTIVATION <= dlMsgTypeMsgInfo_p->msgType && 
                                        SPS_RETX >= dlMsgTypeMsgInfo_p->msgType)
                                {
                                    schUsed = SPS_SCHEDULING;
                                }

                                dlHarqTimerStart(ueContext_p->ueIndex,
                                        harqProcess_p->harqProcessId,harqSubFrameNum,
                                        scheduledExpiryTick,schUsed,internalCellIndex);
                                /* + SPR 11197 Changes */
                                /* In case in future if the SCELL_ACT_DEACT_SPATIAL_MUX and                                  
                                   TA_SCELL_ACT_DEACT_SPATIAL_MUL msg types are supported, 
                                   the call to checkScellActDeactBitmask() will be required.                                  
                                 */
                                /*SPR 16026 changes start*/  
                                checkScellActDeactBitmask(harqProcess_p, ueContext_p);
                                /*SPR 16026 changes end*/  
                                /* - SPR 11197 Changes */
                       
/* - SPR 11107 Changes */
#elif TDD_CONFIG
                                tddDlHarqTimerStart(ueContext_p->ueIndex,
                                        harqProcess_p->harqProcessId,
                                        dlSubframeNum,
                                        scheduledTick-phyDelay,
                                        sysFrame,
                                        /* + SPS_TDD_Changes */
                                        schUsed,
                                        /* - SPS_TDD_Changes */
                                        internalCellIndex
                                        ); 
                                /* SPR 14250 FIX START */
                                /*SPR 17561 FIX*/
                                /* SPR 16026 START */
                                checkScellActDeactBitmask(harqProcess_p, ueContext_p ,dlSubframeNum, scheduledTick-phyDelay);
                                /* SPR 16026 END */
                                /*SPR 17561 FIX*/
                                /* SPR 14250 FIX END */
#endif
                                continue;
			    }
                        }
                        else
                        {
                            if( ! harqProcess_p->isTBOneValid )
                            {
                                LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                                        "ERROR TB One Invalid \n");
                            }
                            else
                            /*QOS_KLOCKWORK_FIX*/
                            {
                                LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                                        "ERROR TB Two Valid \n");
                             }
                            /*QOS_KLOCKWORK_FIX*/
                        }
                    }
                    break;
                case  TA_NON_SPATIAL_MUL:
                /*SPR 3869 Fix Start*/
                case TPC_WITHOUT_DATA:
                /*SPR 3869 Fix End*/
/*START : DRX_MAC_CE*/
                case  TA_DRX_NON_SPATIAL_MUL:
                case  DRX_NON_SPATIAL_MUL:
/*END   : DRX_MAC_CE*/
                case  TA_SCELL_ACT_DEACT_NON_SPATIAL_MUL:
                case  SCELL_ACT_DEACT_NON_SPATIAL_MUL:
                    /* SPR 22352 fix */
                case DRX_SCELL_ACT_DEACT_NON_SPATIAL_MUL:
                case TA_DRX_SCELL_ACT_DEACT_NON_SPATIAL_MUL:
                    /* SPR 22352 fix */
                    {

                        /*Calling  allocateRBsForSaptialMul funtion which will 
                          allocate RBs to UE*/
                        if((harqProcess_p->isTBOneValid) && 
                                (! harqProcess_p->isTBTwoValid))
                        {
                            /*Calling  allocRBInPDSCHForNonSpatialMul funtion which will 
                              allocate RBs to UE*/
                            if ( MAC_SUCCESS ==  allocRBInPDSCHForNonSpatialMul(
                                    resourceAllocInput_p,
                                    &resourceAllocOutput,ueContext_p,
                                    harqProcess_p,raPolicyInfo_p,
                                    dlStrategyTxNode_p
                                     /*Dynamic PDCCH + 5117 changes starts */
                                    ,scheduledTick,
                                    &numOfOverLappingRb
                                     /*Dynamic PDCCH + 5117 changes ends */
#ifdef TDD_CONFIG									
                                    , subframe
#endif									
                                        ,internalCellIndex) )

                            {
#ifdef PERF_STATS
                                /* Update the Counter for the Number of PRBs used in DL per
                                 * cell for HARQ retransmissions */
                                if (RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator ||
                                        RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
                                {
									/* + PERF_CA */
                                    gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedDLPerfStats.\
                                        totalDLPRBUsageforHarqRetx +=  \
                                        resourceAllocInput_p->requiredRB;
									/* - PERF_CA */
                                    /* SPR 23402 Changes Start */
                                    /*MOCN SPR 21913 Start*/                                                                      
#ifdef KPI_STATS
                                    activeIndexForMacStats = (gMacStats.
                                            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
                                    pLteMacKPIStats = &(gMacStats.
                                            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
                                    for (qci = 1; qci <= MAX_QCI;qci++)
                                    {
                                        pLteMacKPIStats->totalDLPRBUsagePerQCI[qci-1] += ueContext_p->qciPerTBSize[qci-1];
                                        if(mocnOperatorParams_g.isMocnEnabled && (ueContext_p->operatorId < mocnOperatorParams_g.numOfOperator))
                                        {
                                            pLteMacKPIStats->totalDLPRBUsagePerOperatorPerQCI[ueContext_p->operatorId][qci-1] += ueContext_p->qciPerTBSizePerOperatorPerQCI[ueContext_p->operatorId][qci-1] ;
                                        }
                                    }
#endif
                                    /*MOCN SPR 21913 End*/  
                                    /* SPR 23402 Changes End */                                                                    
                                }
                                CALCULATE_DL_RESOURCE_ELEMENTS(resourceAllocOutput.allocatedRB,internalCellIndex);
#endif
                                /*Dynamic PDCCH + 5117 changes starts */
                                /* + TM7_8 Changes Start */
                                if(((ueContext_p->dlMIMOInfo.transmissionMode == TX_MODE_7)&&(harqProcess_p->dciFormat == DCI_FORMAT_1))||
                                        ((ueContext_p->dlMIMOInfo.transmissionMode == TX_MODE_8)&&(harqProcess_p->dciFormat == DCI_FORMAT_2B)))
                                {
                                    /*SPR 19029 Start*/
                                    updateMCSIndexAndTBSIzeForTransMode_7_8(harqProcess_p,internalCellIndex);                               
                                    /*SPR 19029 End*/
                                }
                                else
                                {
                                    /* - TM7_8 Changes End */
                                    if(MAC_FAILURE == checkCodeRateAndUECategoryPostAllocation(scheduledTick,
                                                harqProcess_p,
                                                numOfOverLappingRb,
                                                ueContext_p,
                                                internalCellIndex))
                                    {
                                        break;
                                    }
                                    /* + TM7_8 Changes Start */
                                }
                                /* - TM7_8 Changes End */
                                /*Dynamic PDCCH + 5117 changes ends */
                                harqProcess_p->dciFormat =  dlMsgInfo_p->dciFormat;
                                /*Populating the encoderNodeInfo structure which wiill be
                                  be used by callMuxOrEncoderSpatialMul funtion for its 
                                  procesing*/
                                POPULATE_ENCODER_NODE_INFO(encoderNodeInfo ,
                                        scheduledTick , dlStrategyTxNode_p ,dlMsgInfo_p);
                                 /* + CQI_5.0 */
                                /*Populating the the DCI CCE Info with relvant Informtion*/  
                                POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_MUL_DATA_MSG(
                                        dlMsgTypeMsgInfo_p->msgType, dciCCEInfo_p ,
                                        (&resourceAllocOutput), dlMsgInfo_p ,ueContext_p ,
                                        harqProcess_p,ueContext_p->dlMIMOInfo.dlRi,
                                /* - CQI_5.0 */        
                                        scheduledTick,internalCellIndex);
#ifdef LTE_EMBMS_SUPPORTED
                                /* Coverity Fix 93495 start*/
                                if(isCurrentSfEmbmsReserved_g[internalCellIndex])
                                {
                                    dciCCEInfo_p->dciPduInfo.pA -= (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->eMbmsReservedCellPowerOffset;  
                                }   
                                /* Coverity Fix 93495 end*/
#endif

                                if( harqProcess_p->isTBOneValid && 
                                        NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator)
                                {
                                    dciCCEInfo_p->dciPduInfo.tb1Info.tbSize = harqProcess_p->
                                        dlHARQTBOneInfo.tbSize;
                                    /*SPR 1388 changes start */
                                    dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer = 0; 
                                    /*SPR 1388 changes end */

#ifdef TDD_CONFIG
                                    /* Cyclomatic_complexity_changes_start */
                                    setSubFrameTypeForDlHarq(isSplSubFrame,harqProcess_p);
                                    /* Cyclomatic_complexity_changes_end */
#endif            
                                }
                                else
                                {
                                    LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                                            "Retransmission of TB One \n");
                                }
                                putEntryInSchEncoderQueue(dlStrategyTxNode_p->ueIndex,
                                        &encoderNodeInfo, containerIndex, internalCellIndex);   
                                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                                        "Updating lastScheduledTick = %d",scheduledTick);
                                
                                /* ICIC changes start */
                                LOG_UT (MAC_PDSCH_BUFF_POOL_ALLOC_DETAILS,LOGDEBUG,MAC_PDSCH,\
                                        getCurrentTick(),\
                                        dlBufferPool_g[internalCellIndex].totalRBs,\
                                        dlBufferPool_g[internalCellIndex].cellEdgeRBs,\
                                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                                        DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                                        "Values of Buffer Pool after RB allocation to UE");

                                LOG_UT (MAC_PDSCH_UE_SCHEDULING_INFO,LOGDEBUG,\
                                        MAC_PDSCH, getCurrentTick(),\
                                        ueContext_p->ueIndex,\
                                        harqProcess_p->harqProcessId,\
                                        resourceAllocOutput.allocatedRB,\
                                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,\
                                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex,\
                                        DEFAULT_FLOAT_VALUE,\
                                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                                        "UE Scheduling Information");

                                LOG_UT (MAC_PDSCH_UE_SCHEDULING_DETAILS,LOGDEBUG,\
                                        MAC_PDSCH, getCurrentTick(),\
                                        ueContext_p->userLocationType,\
                                        harqProcess_p->dlHARQTBOneInfo.txIndicator,\
                                        (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.
                                        rat2LocalizedInfo.numOfCellEdgeVRBAvailable,\
                                        (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.
                                        rat2LocalizedInfo.firstCellEdgeFreeIndex,\
                                        (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.
                                        rat2LocalizedInfo.endCellEdgeFreeIndex,\
                                        DEFAULT_FLOAT_VALUE,\
                                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                                        "UE Scheduling DATAILS");
                                /* CA Stats Changes Start */
                                /* ICIC changes end */

                                LTE_MAC_UPDATE_STATS_FOR_PDSCH( ueContext_p,
                                        dlStrategyTxNode_p,
                                        harqProcess_p,
                                        dciCCEInfo_p,
                                        /*spr 9052 fix start*/
                                        getCurrentTick(),
                                        /*spr 9052 fix end*/
                                        excessPoolRb_g[internalCellIndex][resourceRegionAllocated],
                                        internalCellIndex);
                                /* CA Stats Changes End */
  
                                freeMemPool(dlStrategyTxNode_p);
  
                                dlStrategyTxNode_p = PNULL;
                                *ueDlStrategyTxNode_p = PNULL;

                                dciCCEInfo_p->msgIdentifier = msgIndentifier;
                                msgIndentifier++;
                                /*Incrementing the pointer so that now it will point to
                                  next Msg*/
                                dlMsgTypeMsgInfo_p++;
                                (*maxUEsToBeScheduledDL_p)--;
                                dlMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.
                                    dlDataMsgInfo; 
                                allocatedRBCount -= resourceAllocOutput.allocatedRB;                           
                                totalRb += resourceAllocOutput.allocatedRB;
#ifdef TDD_CONFIG
                               /* + SPR_14039 */
                                if ((!IS_CA_ENABLED()) || ( (IS_CA_ENABLED()) &&
                                        (RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator ||
                                        RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)))
                                {
                                    /* Update DAI and staore the updated DAI in DCI Info */
                                    dciCCEInfo_p->dciPduInfo.dai = updateDai(ueContext_p,
                                        scheduledTick,
                                        subframe,
                                        internalCellIndex);
                                }
                               /* + SPR_14039 */
#endif

                                /* DRX_CHG */
                                /*START_DRX_OPT*/
                                if (SETUP == ueContext_p->drxConfigType &&
                                    drxContext_p->drxConfig.isdrxMacCeCmdConfigured &&
                                    (!drxContext_p->drxStateVariables.isDrxMacCeTimerRunning)
                                   )
                                /*END_DRX_OPT*/
                                {
                                    /* SPR 12075 Changes Start */
                                    putEntryInDRXDLSchedulerEventQueue
                                        (ueContext_p->ueIndex, scheduledTick,
                                         ueContext_p->internalCellIndex);
                                    /* SPR 12075 Changes End */
                                }
                                /*START_DRX_OPT*/
                                if (SETUP == ueContext_p->drxConfigType)
                                {
                                    LOG_MAC_MSG(MAC_DRX_EVENT_TRIGGERED_LOG_ID, LOGDEBUG, MAC_DRX,
                                    getCurrentTick(), ueContext_p->ueIndex, DRX_EVENT_DL_DATA_SCHEDULED,
                                    ueContext_p->drxContext.drxStateVariables.drxCurrentState,
                                    ueContext_p->drxContext.drxStateVariables.drxSubState,
                                    ueContext_p->drxContext.drxStateVariables.drxNextCycle,
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,__func__,"");
                                } 
                                /*END_DRX_OPT*/
                                /* DRX_CHG */
/* + SPR 11107 Changes */
#ifdef FDD_CONFIG
                                harqSubFrameNum = (scheduledTick + FDD_HARQ_OFFSET )%MAX_SUB_FRAME;
                                scheduledExpiryTick = scheduledTick  + 8;

                                if (SPS_ACTIVATION <= dlMsgTypeMsgInfo_p->msgType && 
                                        SPS_RETX >= dlMsgTypeMsgInfo_p->msgType)
                                {
                                    schUsed = SPS_SCHEDULING;
                                }

                                dlHarqTimerStart(ueContext_p->ueIndex,
                                        harqProcess_p->harqProcessId,harqSubFrameNum,
                                        scheduledExpiryTick,schUsed,internalCellIndex);
                                /* + SPR 11197 Changes */
                                /*SPR 16026 changes start*/ 
                                checkScellActDeactBitmask(harqProcess_p, ueContext_p);
                                /*SPR 16026 changes end*/  
                                /* - SPR 11197 Changes */
                       
/* - SPR 11107 Changes */
#elif TDD_CONFIG
                                tddDlHarqTimerStart(ueContext_p->ueIndex,
                                        harqProcess_p->harqProcessId,
                                        dlSubframeNum,
                                        scheduledTick-phyDelay,
                                        sysFrame,
                                        /* + SPS_TDD_Changes */
                                        schUsed,
                                        /* - SPS_TDD_Changes */
                                        internalCellIndex
                                        ); 
                                /* SPR 14250 FIX START */
                                /*SPR 17561 FIX*/
                                /* SPR 16026 START */
                                checkScellActDeactBitmask(harqProcess_p, ueContext_p ,dlSubframeNum, scheduledTick-phyDelay);
                                /* SPR 16026 END */
                                /*SPR 17561 FIX*/
                                /* SPR 14250 FIX END */
#endif
                                continue;
			    }
			    else
			    {
                                if( ! harqProcess_p->isTBOneValid )
                                {
                                    LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                                        "ERROR TB One Invalid \n");
                                }
                                else
                                /*QOS_KLOCKWORK_FIX*/
                                {
                                    LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                                        "ERROR TB Two Valid \n");
                                }
                                /*QOS_KLOCKWORK_FIX*/
			    }
			}
		    }                           
                    break;                
                default:
                    {
                        lteWarning("Message Type Not handled = MshType = %d\n",
                                dlMsgTypeMsgInfo_p->msgType);
                    }
            }                
            LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                    "Allocate RBs Falied for UE = %d\n",dlStrategyTxNode_p->
                    ueIndex);  
            
            /*Incrementing the pointer so that now it will point to
              next Msg*/
            dlMsgTypeMsgInfo_p++;
            dlMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.dlDataMsgInfo;  
            /* SPR 2446 Fix */
            /* SPR 2320 Fix Start */
            processPDSCHFailure(dciCCEContaineInfo_p,dciCCEInfo_p, dlStrategyTxNode_p ,
                    harqProcess_p ,ueContext_p,
#ifdef FDD_CONFIG
                    containerIndex
#elif TDD_CONFIG
		    dlSubframeNum
#endif
                    ,internalCellIndex
);
            /* SPR 2320 Fix End */

        }
        else
        {   
            LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"No Free RBs are Left \
                    For Data Msg  So resetting the CCEs for UE = %d\n");               
#ifdef SSI_DEBUG_STATS
            UInt16 ueIndex = ueContext_p->ueIndex;
#endif            

/*        allocationFailureCause = RB_NOT_AVAILABLE; TDD Warning Fix */

            /* CA Stats Changes Start */
            LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(RB_NOT_AVAILABLE,ueIndex, 
                    internalCellIndex);
            /* CA Stats Changes End */

            /*Incrementing the pointer so that now it will point to
              next Msg*/
            dlMsgTypeMsgInfo_p++;
            dlMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.dlDataMsgInfo;  
/* SPR 2446 Fix */
            
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(dlStrategyTxNode_p->resourceAllocInput.
                    resourceRegionAllocated, 
                    dlStrategyTxNode_p->resourceAllocInput.requiredRB,
                    internalCellIndex);
            if (CE_REGION == 
                    dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated)
            {
                excessPoolRb_g[internalCellIndex][CC_CE_REGION] += 
                    dlStrategyTxNode_p->resourceAllocInput.requiredRB;
            }
            /* ICIC changes end */
            /* SPR 2320 Fix Start */
            processPDSCHFailure(dciCCEContaineInfo_p,dciCCEInfo_p, dlStrategyTxNode_p ,
                    harqProcess_p ,ueContext_p,
#ifdef FDD_CONFIG
                    containerIndex
#elif TDD_CONFIG
		    dlSubframeNum
#endif
                    ,internalCellIndex
);
            /* SPR 2320 Fix End */
        }
    }
    /* ICIC changes start */
    /* check if total allocated RBs has exceedesd the allowed RBs*/
    if ( totalRb > (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB)
    /* ICIC changes end */
    {
        ltePanic("RB exceeded the limit\n");
    }
    /*updating CA load balancing stats*/
    /*CA chngs start*/
    if (IS_CA_ENABLED())
    {
        currentDLPRBUsage = ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB - allocatedRBCount );

        updateCAPRBUsageStats(getCurrentTick(),currentDLPRBUsage,internalCellIndex,
                (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])\
                ->caParams.loadCalcualtionWindowSize,
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->dlAvailableRBs);
    }
    /*CA chngs end*/
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "Leaving function = %s  \n",__func__);
}
/* QOS PDSCH Change Start End*/

/****************************************************************************************
 * Function Name    : processPDSCHFailure 
 * Inputs           : dciCCEContaineInfo_p - 
 *                    dciCCEInfo_p - Pointer to DciCCEInfo structure on which CCE are
 *                                   being reset,
 *                    dlStrategyTxNode_p - Pointer to Downlink Strategy Tx Node,
 *                    harqProcess_p - Harq Process of the UE,
 *                    ueContext_p - Pointer to Downlink UE Context
 *                    containerIndex - Index o fcontainer,
 *                    subFrame - current subframe
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtions will reset the CCEs and also check if Transmission is
 *                    New or RE_TX, for New TX it wil free Harq Process and for RE it wil
 *                    push into advance Queue.
 *****************************************************************************************/
STATIC  void processPDSCHFailure(
		DciCCEContainerInfo *dciCCEContaineInfo_p,
		DciCCEInfo *dciCCEInfo_p,
		DLStrategyTxNode *dlStrategyTxNode_p,
		DLHARQProcess *harqProcess_p,
    DLUEContext *ueContext_p
    /* SPR 2446 Fix */
#ifdef FDD_CONFIG    
    /* SPR 2320 Fix Start */
    ,UInt32 containerIndex 
    /* SPR 2320 Fix End */
    /* SPR 2656 Start */
#elif TDD_CONFIG
    ,UInt8 subFrame
    /* SPR 2656 End */
#endif   
    ,InternalCellIndex internalCellIndex
    )
{
    UInt32 aggregationLvl = 0;
    /* SPR 2656 Start */
#ifdef TDD_CONFIG    
    UInt16 ueIndex = 0;
    ULUEContext *ulUEContext_p = PNULL;
#endif
    /* SPR 2656 End */
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Entering function = %s  \n", 
            __func__);    
    /*In Failure Case,Free the CCEs and Harq Process*/
    aggregationLvl = dciCCEInfo_p->dciPduInfo.aggregationLvl;

    /* SPR 2320 Fix Start */
#ifdef FDD_CONFIG
    (pdcchULOutputInfo_gp[internalCellIndex]+ containerIndex)->directIndexingUEInfo[
                       ueContext_p->ueIndex].dlCCEIndex = INVALID_CCE_INDEX;
    /* SPR 2656 Start */
#elif TDD_CONFIG
    ueIndex = ueContext_p->ueIndex;
    /** SPR 14204 Fix : Start **/
    ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;    
    if(ulUEContext_p && ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][subFrame])
    {
        ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][subFrame]->ueInsertedInPdcchDlUeInfo--;
        ulUEContext_p->dlUeScheduledInfoPointerArray[internalCellIndex][subFrame]->dlCceIndex[subFrame] = INVALID_CCE_INDEX;
    }
    /** SPR 14204 Fix : End **/
    /* SPR 2656 End */
#endif
    /* SPR 2320 Fix End */

    /* Cyclomatic Complexity Changes */
    mac_reset_cce(aggregationLvl, dciCCEInfo_p,internalCellIndex);                    
    mac_reset_extra_cce(dciCCEContaineInfo_p, dciCCEInfo_p,ueContext_p,internalCellIndex);                    
        
    /* + CQI_4.1 */
    if( RE_TX == dlStrategyTxNode_p->transmissionType )
    /* - CQI_4.1 */    
    {
        if(!(harqProcess_p->isTBOneValid) && !(harqProcess_p->isTBTwoValid) ) 
        {
            freeDLHarqProcess(ueContext_p,harqProcess_p,internalCellIndex);
            freeMemPool(dlStrategyTxNode_p);
            /* SPR# 9910 fix start */
            ueContext_p->dlStrategyTxNode_p = PNULL;
            /* SPR# 9910 fix end */
        }
        else
        {
            putEntryInRetransmissionFailureQueue(dlStrategyTxNode_p,internalCellIndex);
        }
        /* + SPR 13605 Fix */
        if(!IS_PCELL_OF_UE(ueContext_p, internalCellIndex) &&
                            ueContext_p->dlUeScellContext_p[START_SCELL_INDEX])
        {
               ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus = 0x00;
               /** SPR 14277 Changes Start **/
#ifdef TDD_CONFIG
                ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.
                sCellSchInfo[ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.sCellSubFrame] = 0x00;
#endif
               /** SPR 14277 Changes End **/
        }
        /* - SPR 13605 Fix */
    }
    else
    {

       if(DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                initParams_p->dlSchdConfig.dlSchedulerStrategy)
        {
            switch(harqProcess_p->dlHARQTBOneInfo.taDrxFlag)
            {
                case TA_MSG:
                    /* SPR 9546 Fix Start */
                    putEntryInPdcchPdschFailureTAQueue(
                        dlStrategyTxNode_p->ueIndex,
                            ueContext_p->timingAdvanceValue,
                    /* SPR 9546 Fix End */
                        internalCellIndex);
                    break;
                default:
                    break;
                    /* No TA_DRX Confiured. Hence do nothing */
            }

            if( ( (harqProcess_p->isTBOneValid) &&
                  (harqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG)) ||
                ( (harqProcess_p->isTBTwoValid) &&  
                  (harqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG)))
            {
                putEntryInPendingFailureDlCAMacCEQueue( dlStrategyTxNode_p->ueIndex,
                                                        harqProcess_p,
                                                        internalCellIndex);
            }

            freeDLHarqProcess(ueContext_p,harqProcess_p,internalCellIndex);
            freeMemPool(dlStrategyTxNode_p);
            dlStrategyTxNode_p = PNULL;	
            if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex ))
            {
                ueContext_p->dlStrategyTxNode_p = PNULL;  	
            }
            else
            {
                ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p = PNULL;
                /* + SPR 13605 Fix */
                ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus = 0x00;
               /** SPR 14277 Changes Start **/
#ifdef TDD_CONFIG
                ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.
                sCellSchInfo[ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.sCellSubFrame] = 0x00;
#endif
               /** SPR 14277 Changes End **/
                /* - SPR 13605 Fix */
            }
        }
        else
        {

            if((harqProcess_p->dlHARQTBOneInfo.taDrxFlag != INVALID_TA_DRX_MSG) 
                   || (harqProcess_p->dlHARQTBTwoInfo.taDrxFlag != 
                                INVALID_TA_DRX_MSG))                    
            {
                /* +coverity-530-CID 24492*/
                if(MAC_FAILURE == putEntryInAdvanceAllocInfoQueue(dlStrategyTxNode_p,internalCellIndex))
                {
                    freeDLHarqProcess(ueContext_p,harqProcess_p,internalCellIndex);
                    freeMemPool(dlStrategyTxNode_p);
                    if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex ))
                    {
                        ueContext_p->dlStrategyTxNode_p = PNULL;  	
                    }
                    else
                    {
                        ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p = PNULL;
                    }
                }
                /* +coverity-530-CID 24492*/
            }
            else
            {
                freeDLHarqProcess(ueContext_p,harqProcess_p,internalCellIndex);
                freeMemPool(dlStrategyTxNode_p);
                /* SPR# 9910 fix start */
                ueContext_p->dlStrategyTxNode_p = PNULL;
                /* SPR# 9910 fix end */
            }
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Leaving function = %s  \n", 
            __func__);   
}
/****************************************************************************************
 * Function Name    : callMuxOrEncoderSpatialMul
 * Inputs           : dlMsgTypeMsgInfo_p - This stores DLMsgTypeMsgInfo Pointer which
 *                                         store the information of those UE for which
 *                                         the PDCCH region has been allocated. It is a
 *                                         Pointer to DLMsgTypeMsgInfo,
 *                    dlStrategyTxNode_p - Pointer to Downlink Strategy Tx Node,
 *                    dlMsgInfo_p - Pointer to DLDataMsgInfo,
 *                    harqProcess_p - Harq Process of the UE,
 *                    dciCCEInfo_p - Pointer to DciCCEInfo,
 *                    withMuxFlag - This when set to one Indicates that we need to call
 *                                  directly invoke Mux Execution Logic,
 *                    scheduledTick - This holds the Tick Value in which UE is
 *                                    expected to be scheduled,
 *                    ueContext_p - Pointer to Downlink UE Context            
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : resourceAllocOutput_p - Pointer to ResourceAllocatorOutput
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will either invoke Mux Logic or Encoder Logic for 
 *                    Spatial Multiplexing and Pass relevant information to them.
 *****************************************************************************************/ 
/*SPR 2446 Fix Begins*/
/* SPR 4168  */
STATIC  MacRetType callMuxOrEncoderSpatialMul(
/* SPR 4168  */
        DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        DLStrategyTxNode *dlStrategyTxNode_p,
        DLDataMsgInfo *dlMsgInfo_p,                                        
        DLHARQProcess *harqProcess_p,
        DciCCEInfo *dciCCEInfo_p,                                       
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        DLUEContext *ueContext_p,
        ResourceAllocatorOutput *resourceAllocOutput_p,
/*SPR 2446 Fix Ends*/
        InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG                                        
       ,UInt8 dlSubframeNum
       ,UInt32 phyDelay
       ,UInt32 sysFrame
#endif 
        )
{
    UInt32 isReTxFlag;      
    DLHARQTBInfo   *dlHARQTBOneInfo = PNULL;
    DLHARQTBInfo   *dlHARQTBTwoInfo = PNULL;
    static UInt8 sizeofMuxNode = sizeof(MultiplexerQueueNode);
    /* #ifndef DL_UL_SPLIT
       ULUEContext *ulUEContext_p = PNULL;
       ulUEContext_p = ulUECtxInfoArr_g[dlStrategyTxNode_p ->ueIndex].ulUEContext_p;
#endif TDD warning Fix */
    /* SPR 4168  */
    MacRetType retType = MAC_SUCCESS;
    /* SPR 4168  */

    dlHARQTBOneInfo = &harqProcess_p->dlHARQTBOneInfo;
    dlHARQTBTwoInfo = &harqProcess_p->dlHARQTBTwoInfo;
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "Entering function = %s  \n",__func__);
    /** SPR 12348 Changes Start **/ 
    MultiplexerQueueNode  * muxQueueNode_p = PNULL;
    /** SPR 12348 Changes End **/ 

#ifdef TDD_CONFIG
    /*coverity-530-32553*/
    /* SPR 23393 Changes Start */
    UInt8 subframe = (scheduledTick ) % MAX_SUB_FRAME;
    UInt8 isSplSubFrame = checkForSpecialSubframe(subframe, internalCellIndex); 
    /* SPR 23393 Changes End */
    /*coverity-530-32553*/
#endif
    /*Checking if Both TB need to sent */
    if ( harqProcess_p->isTBOneValid && harqProcess_p->isTBTwoValid )
    {
        /*Checking if both the TB  are ReTransmission .If  true in this case
          Request can be directly sent to Encoder to start forming TB One and 
          TB Two*/
        LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                " Both TB Valid\n");
        if ( RE_TX ==  dlHARQTBOneInfo->txIndicator &&
                RE_TX ==  dlHARQTBTwoInfo->txIndicator )
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                    " Both TB Retransmission\n");
            /* SPR 6136 start */ 
            if( (dlHARQTBOneInfo->mcsIndex == 0) &&
                    (1 == ((dlHARQTBOneInfo->irVersion + 1 ) % 4) ) ) 
            {
                dlHARQTBOneInfo->irVersion = MODULO_FOUR((dlHARQTBOneInfo->irVersion + 1));    
            }
            if( (dlHARQTBTwoInfo->mcsIndex == 0) &&
                    (1 == ((dlHARQTBTwoInfo->irVersion + 1 ) % 4) ) ) 
            {
                dlHARQTBTwoInfo->irVersion = MODULO_FOUR((dlHARQTBTwoInfo->irVersion + 1));    
            }
            /* SPR 6136 end */
            dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer =   
                (dlHARQTBOneInfo->irVersion + 1 ) % 4;
            dciCCEInfo_p->dciPduInfo.tb2Info.redundancyVer =   
                (dlHARQTBTwoInfo->irVersion + 1 ) % 4;
            dlHARQTBOneInfo->current_TX_NB++;
            dlHARQTBTwoInfo->current_TX_NB++;
            dlHARQTBOneInfo->irVersion =
                MODULO_FOUR((dlHARQTBOneInfo->irVersion + 1));
            dlHARQTBTwoInfo->irVersion = 
                MODULO_FOUR((dlHARQTBTwoInfo->irVersion + 1));
            dciCCEInfo_p->dciPduInfo.tb1Info.tbSize = dlHARQTBOneInfo->
                retransmissionTBSize;
            dciCCEInfo_p->dciPduInfo.tb2Info.tbSize = dlHARQTBTwoInfo->
                retransmissionTBSize;
            dciCCEInfo_p->dciPduInfo.dataTB_p = harqProcess_p->transmissionBuffer_p; 
            /* SPR 3679 Changes Start */
            dciCCEInfo_p->dciPduInfo.dataTB2_p = harqProcess_p->transmissionBufferTB2_p; 
            /* SPR 3679 Changes Emd */
            /*Cyclomatic Complexity changes - starts here */
            /*Populating the the DCI CCE Info with relvant Informtion*/ 

            /** SPR 12348 Changes Start **/ 
            if(!(IS_PCELL_OF_UE(ueContext_p,internalCellIndex)))
            {
                GET_MEM_FROM_POOL(MultiplexerQueueNode , muxQueueNode_p , sizeofMuxNode , PNULL);

                if( PNULL == muxQueueNode_p )
                {
                    return MAC_FAILURE ;
                }
#ifdef FDD_CONFIG                
                POPULATE_MUX_NODE_INFO(scheduledTick , dlStrategyTxNode_p ,
                        dlMsgInfo_p,muxQueueNode_p , dlMsgTypeMsgInfo_p,
                        ueContext_p, dciCCEInfo_p, 
                        internalCellIndex);     
#elif TDD_CONFIG

                POPULATE_MUX_NODE_INFO(scheduledTick , dlStrategyTxNode_p ,
                        dlMsgInfo_p,muxQueueNode_p , dlMsgTypeMsgInfo_p,
                        ueContext_p, dciCCEInfo_p, internalCellIndex, dlSubframeNum,
                        phyDelay, sysFrame);     
#endif                
            }

            retType =  fillDciCCEStructForSpatialDataMsg(dlMsgTypeMsgInfo_p->msgType,
                    dciCCEInfo_p ,resourceAllocOutput_p,
                    dlMsgInfo_p ,ueContext_p ,
                    /* +- SPR 17777 */
                    harqProcess_p,
                    scheduledTick,withMuxFlag,muxQueueNode_p
                    ,internalCellIndex
#ifdef TDD_CONFIG
                    ,dlSubframeNum
                    ,phyDelay
                    ,sysFrame
#endif                     
                    );
            /** SPR 12348 Changes End **/
            /*Cyclomatic Complexity changes - ends here */
        }
        else
        {
            /* Comes here when either one of condition is meet
             *1) If Both are new Transmissoin
             *2) If one is ReTransmission and other is New Transmission*/  
            if (RE_TX ==  dlHARQTBOneInfo->txIndicator)
            {
                /* SPR 6136 start */  
                if( (dlHARQTBOneInfo->mcsIndex == 0) &&
                        (1 == ((dlHARQTBOneInfo->irVersion + 1 ) % 4) ) ) 
                {
                    dlHARQTBOneInfo->irVersion = MODULO_FOUR((dlHARQTBOneInfo->irVersion + 1));    
                }
                /* SPR 6136 end */
                LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                        "Retransmission of TB One \n");
                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                        " TB One RE_TX and TB Two NEW_TX\n");
                /*SPR 1388 changes start */
                dciCCEInfo_p->dciPduInfo.tb2Info.redundancyVer = 0; 
                /*SPR 1388 changes end */
                dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer =  
                    (dlHARQTBOneInfo->irVersion + 1 ) % 4;
                dciCCEInfo_p->dciPduInfo.tb1Info.tbSize = dlHARQTBOneInfo->
                    retransmissionTBSize;
                dciCCEInfo_p->dciPduInfo.tb2Info.tbSize = dlHARQTBTwoInfo->tbSize;
            }
            else if (RE_TX ==  dlHARQTBTwoInfo->txIndicator)
            {
                /* SPR 6136 start */
                if( (dlHARQTBTwoInfo->mcsIndex == 0) &&
                        (1 == ((dlHARQTBTwoInfo->irVersion + 1 ) % 4) ) ) 
                {
                    dlHARQTBTwoInfo->irVersion = MODULO_FOUR((dlHARQTBTwoInfo->irVersion + 1));    
                }
                /* SPR 6136 end */
                LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                        "Retransmission of TB Two \n");
                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                        " TB TWo RE_TX and TB One NEW_TX\n");
                /*SPR 1388 changes start */
                dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer = 0; 
                /*SPR 1388 changes end */
                dciCCEInfo_p->dciPduInfo.tb2Info.redundancyVer =  
                    (dlHARQTBTwoInfo->irVersion + 1 ) % 4; 
                dciCCEInfo_p->dciPduInfo.tb2Info.tbSize = dlHARQTBTwoInfo->
                    retransmissionTBSize;
                dciCCEInfo_p->dciPduInfo.tb1Info.tbSize = dlHARQTBOneInfo->tbSize;
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                        " TB One NEW_TX and TB Two NEW_TX\n");
                /*SPR 1388 changes start */
                dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer = 0; 
                if(harqProcess_p->isTBTwoValid)
                {    
                    dciCCEInfo_p->dciPduInfo.tb2Info.redundancyVer = 0; 
                }    
                /*SPR 1388 changes end */
                dciCCEInfo_p->dciPduInfo.tb2Info.tbSize = dlHARQTBTwoInfo->tbSize;
                dciCCEInfo_p->dciPduInfo.tb1Info.tbSize = dlHARQTBOneInfo->tbSize;
#ifdef TDD_CONFIG
                setSubFrameTypeForDlHarq(isSplSubFrame,harqProcess_p);
#endif            
            }
            /*Populating the Mux Node Info which will be used by 
              Multiplexer*/
                    GET_MEM_FROM_POOL(MultiplexerQueueNode , muxQueueNode_p , sizeofMuxNode , PNULL);

                /* + Coverity 24499 */
                if( PNULL == muxQueueNode_p )
                {
                    return MAC_FAILURE ;
                }
                /* - Coverity 24499 */
                /* + Coverity_69462 */
#ifdef FDD_CONFIG                
                /* + SPR 11208 Changes */
                POPULATE_MUX_NODE_INFO(scheduledTick , dlStrategyTxNode_p ,
                        dlMsgInfo_p,muxQueueNode_p , dlMsgTypeMsgInfo_p,
                        ueContext_p, dciCCEInfo_p, 
                        internalCellIndex);     
                /* - SPR 11208 Changes */
#elif TDD_CONFIG

                POPULATE_MUX_NODE_INFO(scheduledTick , dlStrategyTxNode_p ,
                        dlMsgInfo_p,muxQueueNode_p , dlMsgTypeMsgInfo_p,
                        ueContext_p, dciCCEInfo_p, internalCellIndex, dlSubframeNum,
                        phyDelay, sysFrame);     
#endif                
                /* - Coverity_69462 */
                /*Cyclomatic Complexity changes - starts here */
                /*Populating the the DCI CCE Info with relvant Informtion*/  
                retType = fillDciCCEStructForSpatialDataMsg( dlMsgTypeMsgInfo_p->msgType,
                        dciCCEInfo_p ,resourceAllocOutput_p,
                        dlMsgInfo_p ,ueContext_p ,
                        /* +- SPR 17777 */
                        harqProcess_p,
                        /* +- SPR 17777 */
                        scheduledTick,withMuxFlag,muxQueueNode_p
                        ,internalCellIndex
#ifdef TDD_CONFIG
                        ,dlSubframeNum
                        ,phyDelay
                        ,sysFrame
#endif                     
                        );
                /*Cyclomatic Complexity changes - ends here */

        }  
    }
    else 
    {
        /*Comes here when one TB is Valid */
        /*Checking if TB One is Valid */
        if ( harqProcess_p->isTBOneValid )
        {
            dciCCEInfo_p->dciPduInfo.tb2Info.redundancyVer =  1;
            dciCCEInfo_p->dciPduInfo.tb2Info.tbSize = 0;
            dciCCEInfo_p->dciPduInfo.tb2Info.modulationType = 0;
            dciCCEInfo_p->dciPduInfo.isTBTwoValid = FALSE;
            /*Comes here when TB One is Valid */
            LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                    "TB One is Valid \n");
            if ( RE_TX ==  dlHARQTBOneInfo->txIndicator ) 
            { 
                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                        "TB One ReTransmission  \n");
                isReTxFlag = RE_TX;
                dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer =  
                    (dlHARQTBOneInfo->irVersion + 1 ) % 4;
                dciCCEInfo_p->dciPduInfo.tb1Info.tbSize = dlHARQTBOneInfo->
                    retransmissionTBSize;
                dciCCEInfo_p->dciPduInfo.dataTB_p = harqProcess_p->transmissionBuffer_p;
                    dlHARQTBOneInfo->current_TX_NB++;
                    dlHARQTBOneInfo->irVersion =
                        MODULO_FOUR((dlHARQTBOneInfo->irVersion + 1));
            }
            else                
            {
                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                        "TB One New Transmission \n"); 
                isReTxFlag = NEW_TX;
                /*SPR 1388 changes start */
                dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer = 0; 
                /*SPR 1388 changes end */

                dciCCEInfo_p->dciPduInfo.tb1Info.tbSize = dlHARQTBOneInfo->tbSize;
                ;
            }                                 
        }
        else if ( harqProcess_p->isTBTwoValid )      
        {
            LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                    "TB Two is Valid \n");
            /*Comes here when TB Two is Valid */
            dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer =  1;
            dciCCEInfo_p->dciPduInfo.tb1Info.tbSize = 0;
            dciCCEInfo_p->dciPduInfo.tb1Info.modulationType = 0;
            dciCCEInfo_p->dciPduInfo.isTBOneValid = FALSE;
            LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                    "TB Two is Valid \n");
            if ( RE_TX ==  dlHARQTBTwoInfo->txIndicator ) 
            { 
                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                        "TB Two ReTransmission \n");
                isReTxFlag = RE_TX;
                dciCCEInfo_p->dciPduInfo.tb2Info.tbSize = dlHARQTBTwoInfo->
                    retransmissionTBSize;
                /* SPR 3679 Changes Start */
                dlHARQTBTwoInfo->current_TX_NB++;
                dlHARQTBTwoInfo->irVersion =
                    MODULO_FOUR((dlHARQTBTwoInfo->irVersion + 1));
                dciCCEInfo_p->dciPduInfo.dataTB2_p = harqProcess_p->transmissionBufferTB2_p;
                /* SPR 3679 Changes End */
            }
            else                
            {
                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                        "TB Two New Transmission \n");
                isReTxFlag = NEW_TX;

                /*SPR 1388 changes start */
                dciCCEInfo_p->dciPduInfo.tb2Info.redundancyVer = 0; 
                /*SPR 1388 changes end */
                dciCCEInfo_p->dciPduInfo.tb2Info.tbSize = dlHARQTBTwoInfo->tbSize;
            }          
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_WARNING,PDSCH_MGR,
                    "TB One And TB Two are invalid \n");
            dciCCEInfo_p->dciPduInfo.isTBOneValid = FALSE;
            dciCCEInfo_p->dciPduInfo.isTBTwoValid = FALSE;
            dciCCEInfo_p->dciPduInfo.tb2Info.redundancyVer =  1;
            dciCCEInfo_p->dciPduInfo.tb2Info.tbSize = 0;
            dciCCEInfo_p->dciPduInfo.tb2Info.modulationType = 0;
            dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer =  1;
            dciCCEInfo_p->dciPduInfo.tb1Info.tbSize = 0;
            dciCCEInfo_p->dciPduInfo.tb1Info.modulationType = 0;
            /* SPR 4168  */
            return MAC_SUCCESS;
            /* SPR 4168  */
        }
        if ( RE_TX == isReTxFlag  )
        {    
            /*Cyclomatic Complexity changes - starts here */
            /*Populating the the DCI CCE Info with relvant Informtion*/ 
            /** SPR 12348 Chanegs Start **/ 
            if(!(IS_PCELL_OF_UE(ueContext_p,internalCellIndex)))
            {
                GET_MEM_FROM_POOL(MultiplexerQueueNode , muxQueueNode_p , sizeofMuxNode , PNULL);

                if( PNULL == muxQueueNode_p )
                {
                    return MAC_FAILURE ;
                }
#ifdef FDD_CONFIG                
/* + SPR 11208 Changes */
                POPULATE_MUX_NODE_INFO(scheduledTick , dlStrategyTxNode_p ,
                        dlMsgInfo_p,muxQueueNode_p , dlMsgTypeMsgInfo_p,
                        ueContext_p, dciCCEInfo_p, 
                        internalCellIndex);     
/* - SPR 11208 Changes */
#elif TDD_CONFIG

                POPULATE_MUX_NODE_INFO(scheduledTick , dlStrategyTxNode_p ,
                        dlMsgInfo_p,muxQueueNode_p , dlMsgTypeMsgInfo_p,
                        ueContext_p, dciCCEInfo_p, internalCellIndex, dlSubframeNum,
                        phyDelay, sysFrame);     
#endif                
            }
            retType = fillDciCCEStructForSpatialDataMsg( dlMsgTypeMsgInfo_p->msgType,
                    dciCCEInfo_p ,resourceAllocOutput_p,
                    dlMsgInfo_p ,ueContext_p ,
                    /* +- SPR 17777 */
                    harqProcess_p,
                    /* +- SPR 17777 */
                    scheduledTick,withMuxFlag,muxQueueNode_p
                    ,internalCellIndex
#ifdef TDD_CONFIG
                    ,dlSubframeNum
                    ,phyDelay
                    ,sysFrame
#endif                     
                    );
            /** SPR 12348 Chanegs End **/ 
            /*Cyclomatic Complexity changes - ends here */
        }
        else
        {
            /*Populating the Mux Node Info which will be used by 
              Multiplexer*/
            GET_MEM_FROM_POOL(MultiplexerQueueNode , muxQueueNode_p , sizeofMuxNode , PNULL);

            /*Coverity-5.3.0 CID-24499 */
            if( PNULL == muxQueueNode_p )
            {
                return MAC_FAILURE ;
            }
            /*Coverity-5.3.0 CID-24499 */
#ifdef TDD_CONFIG
            /* Cyclomatic_complexity_changes_start */
            setSubFrameTypeForDlHarq(isSplSubFrame,harqProcess_p);
            /* Cyclomatic_complexity_changes_end */
#endif            
#ifdef FDD_CONFIG                
            /* + SPR 11208 Changes */
            POPULATE_MUX_NODE_INFO(scheduledTick , dlStrategyTxNode_p ,
                    dlMsgInfo_p,muxQueueNode_p , dlMsgTypeMsgInfo_p,
                    ueContext_p, dciCCEInfo_p, 
                    internalCellIndex);     
            /* - SPR 11208 Changes */
#elif TDD_CONFIG

            POPULATE_MUX_NODE_INFO(scheduledTick , dlStrategyTxNode_p ,
                    dlMsgInfo_p,muxQueueNode_p , dlMsgTypeMsgInfo_p,
                    ueContext_p, dciCCEInfo_p, internalCellIndex, dlSubframeNum,
                    phyDelay, sysFrame);     
#endif                
            /*Cyclomatic Complexity changes - starts here */
            /*Populating the the DCI CCE Info with relvant Informtion*/  
            retType = fillDciCCEStructForSpatialDataMsg(dlMsgTypeMsgInfo_p->msgType, 
                    dciCCEInfo_p ,resourceAllocOutput_p,
                    dlMsgInfo_p ,ueContext_p ,
                    /* +- SPR 17777 */
                    harqProcess_p,
                    /* +- SPR 17777 */
                    scheduledTick,withMuxFlag,muxQueueNode_p
                    , internalCellIndex
#ifdef TDD_CONFIG
                    ,dlSubframeNum
                    ,phyDelay
                    ,sysFrame
#endif                     
                    );
            /*Cyclomatic Complexity changes - ends here */
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "Leaving function = %s   \n",__func__);    
    /* SPR 4168  */
    return retType;
    /* SPR 4168  */
}

/****************************************************************************************
 * Function Name    : callMuxOrEncoderNonSpatialMul
 * Inputs           : dlMsgTypeMsgInfo_p - This stores DLMsgTypeMsgInfo Pointer which
 *                                         store the information of those UE for which
 *                                         the PDCCH region has been allocated. It is a
 *                                         Pointer to DLMsgTypeMsgInfo,
 *                    dlStrategyTxNode_p - Pointer to Downlink Strategy Tx Node,
 *                    dlMsgInfo_p - Pointer to DLDataMsgInfo,
 *                    harqProcess_p - Harq Process of the UE,
 *                    dciCCEInfo_p - Pointer to DciCCEInfo,
 *                    withMuxFlag - This when set to one Indicates that we need to call
 *                                  directly invoke Mux Execution Logic,
 *                    scheduledTick - This holds the Tick Value in which UE is
 *                                    expected to be scheduled,
 *                    ueContext_p - Pointer to Downlink UE Context            
 *                    internalCellIndex - Cell-Index used at MAC
 *                    dlSubframeNum - DL SF 
 *                    phyDelay - Phy delay
 *                    sysFrame - System frame number 
 * Outputs          : resourceAllocOutput_p - Pointer to ResourceAllocatorOutput
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will either invoke Mux Logic or Encoder Logic for 
 *                    Non-Spatial Multiplexing and Pass relevant information to them.
 *****************************************************************************************/ 
/*SPR 2446 Fix Begins*/
/* SPR 4168  */
/*STATIC */  MacRetType callMuxOrEncoderNonSpatialMul(
/* SPR 4168  */
        DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        DLStrategyTxNode *dlStrategyTxNode_p,
        DLDataMsgInfo *dlMsgInfo_p,                                        
        DLHARQProcess *harqProcess_p,
        DciCCEInfo *dciCCEInfo_p,                                       
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        DLUEContext *ueContext_p,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG                                        
       ,UInt8 dlSubframeNum
       ,UInt32 phyDelay
       ,UInt32 sysFrame
#endif 
/*SPR 2446 Fix Ends*/
        )
{      
    /*TDD Tick overshoot changes start*/
#if defined OVERSHOOT_CORRECTION && !defined(DL_UL_SPLIT) && defined(TDD_CONFIG)
    UInt64 rem_cycles = 0;
    UInt64 cyclesReqd = 0;
#ifdef TDD_CONFIG
    UInt32 systemFrameNum = 0;
    UInt32 subFrameNum = 0;
#endif
#endif
    /* + CSR 00058243 Fix*/
#ifdef TDD_CONFIG
    /* - CSR 00058243 Fix*/
    /*coverity-530-32553*/
    /* SPR 23393 Changes Start */
    UInt8 subframe = (scheduledTick ) % MAX_SUB_FRAME;
    UInt8 isSplSubFrame = checkForSpecialSubframe(subframe, internalCellIndex); 
    /* SPR 23393 Changes End */
    /*coverity-530-32553*/
#endif
    /*TDD Tick overshoot changes end*/
    static UInt8 sizeofMuxNode = sizeof(MultiplexerQueueNode);
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "Entering function = %s  \n", __func__);    

    /* CA_phase2_harq_code Start */
    MacRetType ret_type = MAC_SUCCESS;
    /* CA_phase2_harq_code End */


    /*#ifndef DL_UL_SPLIT
      ULUEContext *ulUEContext_p = PNULL;
      ulUEContext_p = ulUECtxInfoArr_g[dlStrategyTxNode_p ->ueIndex].ulUEContext_p;
#endif TDD warning Fix */


    if (  RE_TX ==  harqProcess_p->dlHARQTBOneInfo.txIndicator )
    {

        LTE_MAC_UT_LOG(LOG_MAJOR,PDSCH_MGR,
                " ReTransmission Case \n");

        fillHarqInfoForReTransmission(dlMsgTypeMsgInfo_p, dlStrategyTxNode_p,
                         dlMsgInfo_p, harqProcess_p, dciCCEInfo_p,scheduledTick,
                         ueContext_p,resourceAllocOutput_p, 
                         internalCellIndex
#ifdef TDD_CONFIG
                         ,dlSubframeNum, phyDelay, sysFrame
#endif
                          );
    }
    else
    {

        /*SPR 1388 changes start */
        dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer = 0; 
        /*SPR 1388 changes end */

        LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                " New Transmission Case \n");        
        /*Populating the Mux Node Info which will be used by 
          Multiplexer*/
        MultiplexerQueueNode  * muxQueueNode_p = PNULL;
        GET_MEM_FROM_POOL(MultiplexerQueueNode , muxQueueNode_p , sizeofMuxNode , PNULL);

        /* + Coverity 24498 */
        if( PNULL == muxQueueNode_p )
        {
            return MAC_FAILURE ;
        }
        /* - Coverity 24498 */

        dciCCEInfo_p->dciPduInfo.tb1Info.tbSize = harqProcess_p->
            dlHARQTBOneInfo.tbSize;
#ifdef TDD_CONFIG
        /* Cyclomatic_complexity_changes_start */
        setSubFrameTypeForDlHarq(isSplSubFrame,harqProcess_p);
        /* Cyclomatic_complexity_changes_end */
#endif            
        /* + SPR 11208 Changes */
#ifdef FDD_CONFIG                
/* + SPR 11208 Changes */
                POPULATE_MUX_NODE_INFO(scheduledTick , dlStrategyTxNode_p ,
                    dlMsgInfo_p,muxQueueNode_p , dlMsgTypeMsgInfo_p,
                    ueContext_p, dciCCEInfo_p, 
                    internalCellIndex);     
/* - SPR 11208 Changes */
#elif TDD_CONFIG

                POPULATE_MUX_NODE_INFO(scheduledTick , dlStrategyTxNode_p ,
                    dlMsgInfo_p,muxQueueNode_p , dlMsgTypeMsgInfo_p,
                    ueContext_p, dciCCEInfo_p, internalCellIndex, dlSubframeNum,
                    phyDelay, sysFrame);     
#endif                
        /* + CQI_5.0 */
        /*Populating the the DCI CCE Info with relvant Informtion*/  
        POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_MUL_DATA_MSG(
                dlMsgTypeMsgInfo_p->msgType, dciCCEInfo_p ,
                resourceAllocOutput_p,dlMsgInfo_p ,ueContext_p ,
                harqProcess_p,ueContext_p->dlMIMOInfo.dlRi,
                scheduledTick,internalCellIndex);

        /* - CQI_5.0 */
#ifdef LTE_EMBMS_SUPPORTED
        if(isCurrentSfEmbmsReserved_g[internalCellIndex])
        {
            dciCCEInfo_p->dciPduInfo.pA -= \
            (macReconfigSchedulerParamsDL_gp[internalCellIndex] + \
            schParamsIndexDL_g[internalCellIndex])->eMbmsReservedCellPowerOffset;  
        } 
#endif
 
        LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"sumWeightedQueueLoad = %d \n \
                sumLCPriority =%d",muxQueueNode_p->sumWeightedQueueLoad,
                muxQueueNode_p->sumLCPriority);

        /** CA Phase2 Changes Start **/

        /* CA_phase2_mux start */
        if(!(IS_PCELL_OF_UE(ueContext_p,internalCellIndex)))
        {
            withMuxFlag = FALSE;
        }
        /* CA_phase2_mux end */

        /** CA Phase2 Changes End **/

        if ( withMuxFlag )
        {
            /*TDD Tick overshoot changes start*/                
#if defined OVERSHOOT_CORRECTION && !defined(DL_UL_SPLIT) && defined(TDD_CONFIG)
            GET_PROCESSING_CYCLES_LEFT(rem_cycles);
            /** CA-TDD Changes Start **/
            MIN_CC_REQD_TO_PROCEED_AT_MUX(cyclesReqd,internalCellIndex);
            /** CA-TDD Changes Stop **/

            if ( ( !noTimeToProcessflag_g ) && (
                        rem_cycles >= cyclesReqd ) )
            {
#endif
                /* PDCCH_PDSCH_WITHOUT_MUX Start */
                /*
                 * This function wrapper will call
                 * muxing function based on scheduling policy
                 */
                /*SPR 9325 fix start */
                /* CA_phase2_harq_code Start */
		{
                ret_type = lteLayer2MacLogicalChannelsMuxing(muxQueueNode_p,
                        scheduledTick,
                        ueContext_p,
                        internalCellIndex); 
		}
                /* CA_phase2_harq_code End */

                /* PDCCH_PDSCH_WITHOUT_MUX End */
                /*SPR 9325 fix end */
#if defined OVERSHOOT_CORRECTION && !defined(DL_UL_SPLIT) && defined(TDD_CONFIG)
            }
            else
            {
                /*Overload Detected*/
                noTimeToProcessflag_g = 1;
                currTickOverLoad_g[internalCellIndex] = OVERLOAD_LEVEL_CRITICAL;

                /* CA_phase2_harq_code Start */
                freeDLHarqProcess(ueContext_p,
                        harqProcess_p,internalCellIndex);
                /* CA_phase2_harq_code End */

                LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                        L2_SYS_FAIL,
                        getCurrentTick(),
                        __LINE__,
                        currTickOverLoad_g[internalCellIndex],
                        /** CA-TDD Changes Start **/
                        macOverLoadLevel_g[internalCellIndex],
                        /** CA-TDD Changes Stop **/
                        /*+ Dyn UEs To Be SchPerTTI +*/
                        dlUeSchdIndxInCurrTick_g,ulUeSchdIndxInCurrTick_g,0,0,__func__,
                        /*- Dyn UEs To Be SchPerTTI -*/
                        "Overload level changed.");

                ret_type = MAC_FAILURE;

            }
#endif
            /*TDD Tick overshoot changes end*/                
            freeMemPool(muxQueueNode_p);
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR," Calling funtion \
                    putEntryInMultiplexerQueue funtion");
            /* CA_phase2_harq_code Start */
            putEntryInMultiplexerQueue(muxQueueNode_p,internalCellIndex);
            /* CA_phase2_harq_code End */
        }       
    }

    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "Leaving function = %s   \n", \
            __func__);    
    return ret_type; /* SPR 4168 */     
}

/****************************************************************************************
 * Function Name    : fillDCICCEInfoForReTransmission
 * Inputs           : dciCCEInfo_p - Pointer to DciCCEInfo,
 *                    harqProcess_p - Harq Process of the UE,
 *                    dlMsgTypeMsgInfo_p - This stores DLMsgTypeMsgInfo Pointer which
 *                                         store the information of those UE for which
 *                                         the PDCCH region has been allocated. It is a
 *                                         Pointer to DLMsgTypeMsgInfo,
 *                    dlMsgInfo_p - Pointer to DLDataMsgInfo,
 *                    ueContext_p - Pointer to Downlink UE Context            
 *                    scheduledTick - This holds the Tick Value in which UE is
 *                                    expected to be scheduled,
 *                    resourceAllocOutput_p - Pointer to ResourceAllocatorOutput
 *                    internalCellIndex
 * Outputs          : dciCCEInfo_p - Pointer to DciCCEInfo
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will fill DCI CCE info in case of retransmission 
 *****************************************************************************************/ 
STATIC  void fillDCICCEInfoForReTransmission(
        DciCCEInfo *dciCCEInfo_p,                                       
        DLHARQProcess *harqProcess_p,
        DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        DLDataMsgInfo *dlMsgInfo_p,                                        
        DLUEContext *ueContext_p,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        ResourceAllocatorOutput *resourceAllocOutput_p,
        InternalCellIndex internalCellIndex
        )
{
    dciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer =   
        /* SPR 23393 Changes Start */
        MODULO_FOUR(harqProcess_p->dlHARQTBOneInfo.irVersion + 1 );
        /* SPR 23393 Changes End */
    dciCCEInfo_p->dciPduInfo.tb1Info.tbSize = harqProcess_p->
        dlHARQTBOneInfo.retransmissionTBSize;
    dciCCEInfo_p->dciPduInfo.dataTB_p = harqProcess_p->transmissionBuffer_p;
    harqProcess_p->dlHARQTBOneInfo.current_TX_NB++;
    harqProcess_p->dlHARQTBOneInfo.irVersion =
        MODULO_FOUR((harqProcess_p->dlHARQTBOneInfo.irVersion + 1));

    /* + CQI_5.0 */
    /*Populating the the DCI CCE Info with relvant Informtion*/  
    POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_MUL_DATA_MSG(
            dlMsgTypeMsgInfo_p->msgType, dciCCEInfo_p ,
            resourceAllocOutput_p,dlMsgInfo_p ,ueContext_p ,
            harqProcess_p,ueContext_p->dlMIMOInfo.dlRi,
            scheduledTick,internalCellIndex);
    /* - CQI_5.0 */
#ifdef LTE_EMBMS_SUPPORTED
    if(isCurrentSfEmbmsReserved_g[internalCellIndex])
    {
        dciCCEInfo_p->dciPduInfo.pA -= \
        (macReconfigSchedulerParamsDL_gp[internalCellIndex] + \
        schParamsIndexDL_g[internalCellIndex])->eMbmsReservedCellPowerOffset;  
    } 
#endif
 

}

/*******************************************************************************
 *  Function Name    : decideRATPolicy
 *  Inputs           : requiredRB - This stores the number of RB which need
 *                                  to be reserved,
 *                     dciFormat - This stores the DCI Format for which the
 *                                 PDCCH region has been allocated,
 *                     txType - This stores the Transmission Type,
 *                     raPolicyInfo_p - This holds pointer to
 *                                      ResourceAllocationPolicyInfo,
 *                     harqProcess_p - This holds pointer to DLHARQProcess,
 *                     Rat1Support
 *  Outputs          : ratPolicy_p - This holds the RATPolicy enum selected
 *  Description      : This funtions will return the RAT Policy to be used by 
 *                     Resource allocator.
 ******************************************************************************/
/* UE_CAPAB_ADD_PARAM Start */
STATIC  void  decideRATPolicy(
        UInt32 requiredRB,
        DCIFormat dciFormat,
        TransmissionType txType,
        RATPolicy *ratPolicy_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DLHARQProcess *harqProcess_p,UInt8 Rat1Support)
/* UE_CAPAB_ADD_PARAM End */
{

    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Entering function = %s  \n", 
            __func__);    
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            " dciFormat is %u\n",dciFormat);
    *ratPolicy_p = RA_POLICY_0;

    switch ( dciFormat )
    {
        case DCI_FORMAT_1:
        case DCI_FORMAT_2:
        case DCI_FORMAT_2A:
        /* + TM7_8 Changes Start */
        case DCI_FORMAT_2B:
        /* - TM7_8 Changes End */
        {
            if (  NEW_TX == txType )
            {
                /*Checking if the requiredRB is greater than the max RB of
                 * subset of RAT 1 Policy .If true than return RAT 0 Policy
                 */
                RAT1Info *rat1Info_p = PNULL;
                UInt32 subsetCount = 0;
                rat1Info_p = &raPolicyInfo_p->rat1Info;  
                subsetCount = rat1Info_p->subsetCount;
                if ( raPolicyInfo_p->rat2LocalizedInfo.
                        maxLocalizedVRB <= 10)
                {
                    *ratPolicy_p = RA_POLICY_0;                       
                }   
                else if ( (!(requiredRB % raPolicyInfo_p->rat0Info.rbgSize)) ||
                        (requiredRB > rat1Info_p->subsetInfo[0].maxAddressableRBCount))
                {
                    *ratPolicy_p = RA_POLICY_0;
                    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Giving RAT 0 Policy");
                }
                else
                {
                    /* UE_CAPAB_ADD_PARAM Start */
                    if(TRUE == Rat1Support)
                    {
                        // while loop os subset should be here   
                        while ( subsetCount)
                        {
                            subsetCount--;
                            if ( requiredRB <
                                    rat1Info_p->subsetInfo[subsetCount].numPRBFree)
                            {
                                *ratPolicy_p = RA_POLICY_1;
                                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Giving RAT 1 Policy");
                                break;  
                            }
                        }
                    }
                    /* UE_CAPAB_ADD_PARAM End */
                }                    
            }
            else if ( RE_TX == txType)
            {
                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                        "RE_TX RA Policy =%u  will be used \n",harqProcess_p->ratPolicyUsed);
                *ratPolicy_p = harqProcess_p->ratPolicyUsed;
            }
            harqProcess_p->ratPolicyUsed = *ratPolicy_p;
        }
        break;
        case DCI_FORMAT_1A:
        case DCI_FORMAT_1C:
        case DCI_FORMAT_1B:
        case DCI_FORMAT_1D:
        {
            *ratPolicy_p = RA_POLICY_2_LOCALIZED;
            harqProcess_p->ratPolicyUsed = *ratPolicy_p;
        }
        break;
        default:
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"DCI Format Not \
                    handled \n");
        }        
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Leaving function = %s  \n", \
            __func__);
    return;

}

/*******************************************************************************
 *  Function Name    : decideRATPolicyCellEdge
 *  Inputs           : requiredRB - This stores the number of RB which need
 *                                  to be reserved,
 *                     dciFormat - This stores the DCI Format for which the
 *                                 PDCCH region has been allocated,
 *                     txType - This stores the Transmission Type,
 *                     raPolicyInfo_p - This holds pointer to
 *                                      ResourceAllocationPolicyInfo,
 *                     harqProcess_p - This holds pointer to DLHARQProcess,
 *                     Rat1Support
 *  Outputs          : ratPolicy_p - This holds the RATPolicy enum selected
 *  Description      : This funtions will return the RAT Policy to be used by 
 *                     Resource allocator for cell edge users.
 ******************************************************************************/
/* UE_CAPAB_ADD_PARAM Start */
STATIC  void  decideRATPolicyCellEdge(
        UInt32 requiredRB,
        DCIFormat dciFormat,
        TransmissionType txType,
        RATPolicy *ratPolicy_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DLHARQProcess *harqProcess_p,UInt8 Rat1Support)
/* UE_CAPAB_ADD_PARAM End */
{
    UInt32 numOfCellEdgeFreePRB = 0;
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Entering function = %s  \n", 
            __func__);    
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            " dciFormat is %u\n",dciFormat);
    *ratPolicy_p = RA_POLICY_0;

    switch ( dciFormat )
    {
        case DCI_FORMAT_1:
        case DCI_FORMAT_2:
        case DCI_FORMAT_2A:
        /* + TM7_8 Changes Start */
        case DCI_FORMAT_2B:
        /* - TM7_8 Changes End */
        {
            if (  NEW_TX == txType )
            {
                /*Checking if the requiredRB is greater than the max RB of
                 * subset of RAT 1 Policy .If true than return RAT 0 Policy
                 */
                RAT1Info *rat1Info_p = PNULL;
                UInt32 subsetCount = 0;
                rat1Info_p = &raPolicyInfo_p->rat1Info;  
                subsetCount = rat1Info_p->subsetCount;
                if ( raPolicyInfo_p->rat2LocalizedInfo.
                        maxLocalizedVRB <= 10)
                {
                    *ratPolicy_p = RA_POLICY_0;                       
                }   
                else if ( (!(requiredRB % raPolicyInfo_p->rat0Info.rbgSize)) ||
                        (requiredRB > rat1Info_p->subsetInfo[0].maxAddressableRBCount))
                {
                    *ratPolicy_p = RA_POLICY_0;
                    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Giving RAT 0 Policy");
                }
                else
                {
                    /* UE_CAPAB_ADD_PARAM Start */
                    if(TRUE == Rat1Support)
                    {
                        // while loop os subset should be here   
                        while ( subsetCount)
                        {
                            subsetCount--;
                            numOfCellEdgeFreePRB = getCount( rat1Info_p->
                                    subsetInfo[subsetCount].subsetBitMap[CE_USER] &
                                    rat1Info_p->subsetInfo[subsetCount].
                                    subsetBitMap[CC_USER]);
                            if ( requiredRB < numOfCellEdgeFreePRB)
                            {
                                *ratPolicy_p = RA_POLICY_1;
                                break;  
                            }
                        }                    
                        /* UE_CAPAB_ADD_PARAM End */
                    }
                }                    
            }
            else if ( RE_TX == txType)
            {
                LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
                        "RE_TX RA Policy =%u  will be used \n",harqProcess_p->ratPolicyUsed);
                *ratPolicy_p = harqProcess_p->ratPolicyUsed;
            }
            harqProcess_p->ratPolicyUsed = *ratPolicy_p;
        }
        break;
        case DCI_FORMAT_1A:
        case DCI_FORMAT_1C:
        case DCI_FORMAT_1B:
        case DCI_FORMAT_1D:
        {
            *ratPolicy_p = RA_POLICY_2_LOCALIZED;
            harqProcess_p->ratPolicyUsed = *ratPolicy_p;
        }
        break;
        default:
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"DCI Format Not \
                    handled \n");
        }        
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Leaving function = %s  \n", \
            __func__);
    return;

}

/****************************************************************************************
 * Function Name    : allocSPSRBInPDSCH
 * Inputs           : withMuxFlag - This when set to one Indicates that we need to call
 *                                  directly invoke Mux Execution Logic,
 *                    dlMsgTypeMsgInfo_p - This stores DLMsgTypeMsgInfo Pointer which
 *                                         store the information of those UE for which
 *                                         the PDCCH region has been allocated. It is a
 *                                         Pointer to DLMsgTypeMsgInfo,
 *                    dciCCEContaineInfo_p - This stores the DciCCEInfo pointer which
 *                                           stores the information of the CCE on
 *                                           which Mux or Encoder. So that they can
 *                                           write relevant information on this,
 *                    countMsg - It stores the count of DL Message whose PDCCH Region
 *                               has been allocated,
 *                    raPolicyInfo_p - Pointer to ResourceAllocationPolicyInfo,
 *                    scheduledTick - This holds the Tick Value in which UE is
 *                                    expected to be scheduled,
 *                    containerIndex - It stores the containerIndex which will be used
 *                                     by Encoder,
 *                    msgIndentifier - It stores the msgIndentifier of the Data Msg
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function Allocates the Resources for SPS based on the
 *                    Resources reserved in SPS Reources Map.
 *****************************************************************************************/ 
STATIC void allocSPSRBInPDSCH(
        UInt32 withMuxFlag,
        DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        DciCCEContainerInfo *dciCCEContaineInfo_p,
        UInt32 countMsg,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        ,UInt8 dlSubframeNum
        ,UInt32 phyDelay
        ,UInt32 sysFrame
#endif
        /* - SPS_TDD_Changes */
        )
{
    DLHARQProcess *harqProcess_p = PNULL;   
    DciCCEInfo *dciCCEInfo_p = PNULL;
    DLDataMsgInfo *dlMsgInfo_p = PNULL;  
    DLUEContext *ueContext_p = PNULL;
    DLUEContextInfo *dlUEContextInfo_p = PNULL;        
    DLStrategyTxNode *dlStrategyTxNode_p = PNULL;
    /*warning fix
#ifdef TDD_CONFIG
UInt8 subframe = (scheduledTick ) % MAX_SUB_FRAME;
#endif
     */

    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "Entering function = %s  \n",  __func__);
    dlMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.dlDataMsgInfo;      


    /*Below While Loop is executing number of times = number of UE for which 
      PDCCH Region has been allocated.For Each UE it call funtion
      allocRBInPDSCHForNonSpatialMul ( For UE having TM Mode Other than 3 and 4 )
      or allocRBInPDSCHForSpatialMul (For UE having TM Mode 3 and 4 ) and checks 
      if RBs can be reserved for this UE .If true it will  funtions which will 
      call funtion callMuxOrEncoderSpatialMul or callMuxOrEncoderNonSpatialMul
     */
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "countMsg is= %u\n",countMsg);

    while ( countMsg-- )
    {
        dlStrategyTxNode_p = dlMsgInfo_p->dlStrategyTxNode_p;
        dlUEContextInfo_p = &dlUECtxInfoArr_g[dlStrategyTxNode_p ->ueIndex];
        ueContext_p = dlUEContextInfo_p->dlUEContext_p; 

        harqProcess_p = &ueContext_p->dlHarqContext_p->harqProcess[
            dlStrategyTxNode_p->harqProcessId];


        dciCCEInfo_p = &dciCCEContaineInfo_p->dciCCEInfo[dlMsgInfo_p->cceIndex];

        /* SPS_CHG_START */
        /* !
         * \ fn static void allocRBInPDSCHForData(
         *         UInt32 withMuxFlag,
         *         DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
         *         DciCCEContainerInfo *dciCCEContaineInfo_p,
         *         UInt32 countMsg,
         *         ResourceAllocationPolicyInfo *raPolicyInfo_p,
         *         tickType_t scheduledTick,
         *         Int32 containerIndex,
         *         UInt32 msgIndentifier)
         * \brief Function Modification
         * \code
         * - Check if((SPS_OCCASION == dlMsgTypeMsgInfo_p->msgType) &&
         *                 (dlMsgInfo_p->cceIndex >= MAX_CCE_INDEX)||
         *                 (SPS_ACTIVATION == dlMsgTypeMsgInfo_p->msgType))
         *          - Call the Function allocRBInPDSCHForSPSData
         *          - allocRBInPDSCHForSPSData Function will 
         *              - Get the SPS Resource Resreved for SPS Occasion
         *              from Ue Context.
         *              - Update the harqProcess
         *              - Update the Resource Assignment bitmap
         *              - Update the Global RAT Info 
         *              - Call the MuxOrEncoderNonSpatialMul function
         *                It will fill the DCI.For SPS_OCCASION Transmitter 
         *                will use this only for DL SCH Pdu filling .
         *
         * \endcode
         * */
        if(((SPS_OCCASION == dlMsgTypeMsgInfo_p->msgType) && 
                    (dlMsgInfo_p->cceIndex >= MAX_CCE_INDEX))||
                (SPS_ACTIVATION == dlMsgTypeMsgInfo_p->msgType))
        {
            allocRBInPDSCHForSPSData(
                    withMuxFlag,
                    dlMsgTypeMsgInfo_p,
                    dlStrategyTxNode_p,
                    ueContext_p,
                    harqProcess_p,
                    dciCCEInfo_p,
                    dlMsgInfo_p,
                    raPolicyInfo_p,
                    scheduledTick,0
#ifdef TDD_CONFIG
                    ,dlSubframeNum
                    ,phyDelay
                    ,sysFrame
#endif
                    ,internalCellIndex );
            if (SPS_ACTIVATION == dlMsgTypeMsgInfo_p->msgType)
            {
#ifdef TDD_CONFIG
                /* Update DAI and store it in DCI Info */
                dciCCEInfo_p->dciPduInfo.dai = 
                    /* SPR 609 changes start */
                    updateDai(ueContext_p,
                            /* SPR 609 changes end */
                            scheduledTick,
                            /* SPR 23393 Changes Start */
                            scheduledTick % MAX_SUB_FRAME,
                            /* SPR 23393 Changes End */
                            internalCellIndex);
#endif
            }
            else
            {
#ifdef TDD_CONFIG
                /* Update DAI and store it in DCI Info */
                dciCCEInfo_p->dciPduInfo.dai = 
                    /* SPR 609 changes start */
                    updateDaiSPS(ueContext_p,
                            /* SPR 609 changes end */
                            scheduledTick,
                            /* SPR 23393 Changes Start */
                            scheduledTick % MAX_SUB_FRAME,
                            /* SPR 23393 Changes End */
                            internalCellIndex);
#endif
            }
            /* - SPS_TDD_Changes */
        } 		
        /*Incrementing the pointer so that now it will point to next Msg*/
        dlMsgTypeMsgInfo_p++;
        dlMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.dlDataMsgInfo;
    }
}

/*Dynamic Pdcch Changes Start*/
/*************************************************************************************
 * Function Name     : calculateMCSToBeUsedForTB1
 * Inputs            : harqProcess_p - This holds pointer to DLHARQProcess structure,
 *                     scheduledTick - Scheduled TTI,
 *                     numOfOverLappingRb - This stores the numOfOverlappingRb,
 *                     ueContext_p - Poniter to DLUEContext,
 *                     mcsIndexToBeUsed -
 *                     internalCellIndex - Cell-Index used at MAC
 * Outputs           : mcsToBeUsedCW1_p - it will store the mcs for tb1
 * Returns           : None
 * Description       : This function calculate the MCS to be used for TB1 
 *************************************************************************************/
/* SPR 15909 fix start */
STATIC  void calculateMCSToBeUsedForTB1(DLHARQProcess
        *harqProcess_p,tickType_t scheduledTick,\
/* SPR 15909 fix end */
        UInt8 numOfOverLappingRb, DLUEContext *ueContext_p,\
        UInt8 *mcsToBeUsedCW1_p,\
        /* SPR 8537 Fix Start */
        UInt8 mcsIndexToBeUsed,
        /* SPR 8537 Fix End */
        InternalCellIndex internalCellIndex)
{
    switch(currentCFIValue_g[internalCellIndex])
    {
        case 3:
        {
            /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
            if( ((ueContext_p->ueCategory == UE_CAT_5) || 
                (ueContext_p->ueCategory == UE_CAT_8)) && 
                (harqProcess_p->numOfLayer == RANK_INDICATOR_4) )
                /* SPR 11355 Fix End */
            {
                *mcsToBeUsedCW1_p = ((maxMCSSubframeMapForCFI_3_4x4[\
                            scheduledTick ]\
                            [harqProcess_p->assignedResourcesNB - 1]\
                            [numOfOverLappingRb]) < \
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed)
                        /* SPR 8537 Fix End */
                        ? (maxMCSSubframeMapForCFI_3_4x4[((scheduledTick )\
                                )]\
                            [harqProcess_p->assignedResourcesNB - 1]\
                            [numOfOverLappingRb]):\
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed));
                /* SPR 8537 Fix End */
            }
            /* 4x4 DL MIMO CHG END */
            /*klockworks warning fix*/
            else if(0 < ueContext_p->ueCategory && ueContext_p->ueCategory <= UE_CAT_8)
            {
					/* SPR 23017 Fix Start */ 
					if(harqProcess_p->isTBTwoValid == TB_INVALID)
					{
						*mcsToBeUsedCW1_p = ((maxMCSSubframeMapForCFI_3[\
									scheduledTick ]\
									[harqProcess_p->assignedResourcesNB - 1]\
									[numOfOverLappingRb][ueContext_p->ueCategory -1 ][LAYER_ONE_IDX]) < \
								/* SPR 8537 Fix Start */
								(mcsIndexToBeUsed)
								/* SPR 8537 Fix End */
								? (maxMCSSubframeMapForCFI_3[((scheduledTick )\
										)]\
									[harqProcess_p->assignedResourcesNB - 1]\
									[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]):\
								/* SPR 8537 Fix Start */
								(mcsIndexToBeUsed));
						/* SPR 8537 Fix End */
					}
					else
					{
                *mcsToBeUsedCW1_p = ((maxMCSSubframeMapForCFI_3[\
                            scheduledTick ]\
                            [harqProcess_p->assignedResourcesNB - 1]\
									[numOfOverLappingRb][ueContext_p->ueCategory -1 ][LAYER_TWO_IDX]) < \
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed)
                        /* SPR 8537 Fix End */
                        ? (maxMCSSubframeMapForCFI_3[((scheduledTick )\
                                )]\
                            [harqProcess_p->assignedResourcesNB - 1]\
									[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]):\
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed));
                /* SPR 8537 Fix End */
            }
					/* SPR 23017 Fix End */ 
				}
        }
        break;
        case 2:
        {
#ifdef TDD_CONFIG
            if( 3 == scheduledTick)
            {
                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) ||
                    (ueContext_p->ueCategory == UE_CAT_8)) && 
                    (harqProcess_p->numOfLayer == RANK_INDICATOR_4) )
                /* SPR 11355 Fix End */
                {
                    /** CA-TDD Changes Start **/
                    *mcsToBeUsedCW1_p = ((maxMCSSpecialSFForCFI_2_4x4[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
                                [numOfOverLappingRb]) <
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            /** CA-TDD Changes Start **/
                            ?(maxMCSSpecialSFForCFI_2_4x4[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
                                [numOfOverLappingRb]) :
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
                /* 4x4 DL MIMO CHG END */
                /*klockworks warning fix*/
                else if(0 < ueContext_p->ueCategory && ueContext_p->ueCategory <= UE_CAT_8)
                {
                    /** CA-TDD Changes Start **/
							/* SPR 23017 Fix Start */ 
						if(harqProcess_p->isTBTwoValid == TB_INVALID)
						{
							*mcsToBeUsedCW1_p = ((maxMCSSpecialSFForCFI_2[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
										/** CA-TDD Changes Stop **/
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) <
									/* SPR 8537 Fix Start */
									(mcsIndexToBeUsed)
									/* SPR 8537 Fix End */
									/** CA-TDD Changes Start **/
									?(maxMCSSpecialSFForCFI_2[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
										/** CA-TDD Changes Stop **/
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) :
									/* SPR 8537 Fix Start */
									(mcsIndexToBeUsed));
							/* SPR 8537 Fix End */
						}
						else
						{
                    *mcsToBeUsedCW1_p = ((maxMCSSpecialSFForCFI_2[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) <
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            /** CA-TDD Changes Start **/
                            ?(maxMCSSpecialSFForCFI_2[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) :
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
							/* SPR 23017 Fix End */ 
					}
            }
            else
            {
#endif
                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) ||
                    (ueContext_p->ueCategory == UE_CAT_8)) && 
                    (harqProcess_p->numOfLayer == RANK_INDICATOR_4) )
                /* SPR 11355 Fix End */
                {
                    *mcsToBeUsedCW1_p = ((maxMCSSubframeMapForCFI_2_4x4[\
                                scheduledTick ]\
                                [harqProcess_p->assignedResourcesNB - 1]\
                                [numOfOverLappingRb]) < \
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            ? (maxMCSSubframeMapForCFI_2_4x4[((scheduledTick )\
                                    )]\
                                [harqProcess_p->assignedResourcesNB - 1]\
                                [numOfOverLappingRb]):\
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
                /* 4x4 DL MIMO CHG END */
                /*klockworks warning fix*/
                else if(0 < ueContext_p->ueCategory && ueContext_p->ueCategory <= UE_CAT_8)
                {
							/* SPR 23017 Fix Start */ 
						if(harqProcess_p->isTBTwoValid == TB_INVALID)
						{
							*mcsToBeUsedCW1_p = ((maxMCSSubframeMapForCFI_2[\
										scheduledTick ]\
										[harqProcess_p->assignedResourcesNB - 1]\
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) < \
									/* SPR 8537 Fix Start */
									(mcsIndexToBeUsed)
									/* SPR 8537 Fix End */
									? (maxMCSSubframeMapForCFI_2[((scheduledTick )\
											)]\
										[harqProcess_p->assignedResourcesNB - 1]\
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]):\
									/* SPR 8537 Fix Start */
									(mcsIndexToBeUsed));
							/* SPR 8537 Fix End */
						}
						else
						{
                    *mcsToBeUsedCW1_p = ((maxMCSSubframeMapForCFI_2[\
                                scheduledTick ]\
                                [harqProcess_p->assignedResourcesNB - 1]\
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) < \
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            ? (maxMCSSubframeMapForCFI_2[((scheduledTick )\
                                    )]\
                                [harqProcess_p->assignedResourcesNB - 1]\
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]):\
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
							/* SPR 23017 Fix End */ 
					}
#ifdef TDD_CONFIG
            }
#endif
        }
        break;
        case 1:
        {
#ifdef TDD_CONFIG
            if( 3 == scheduledTick)
            {
                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) ||
                    (ueContext_p->ueCategory == UE_CAT_8)) && 
                    (harqProcess_p->numOfLayer == RANK_INDICATOR_4) )
                /* SPR 11355 Fix End */
                {
                    /** CA-TDD Changes Start **/
                    *mcsToBeUsedCW1_p = ((maxMCSSpecialSFForCFI_1_4x4[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
                                [numOfOverLappingRb]) <
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            /** CA-TDD Changes Start **/
                            ?(maxMCSSpecialSFForCFI_1_4x4[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
                                [numOfOverLappingRb]) :
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
                /* 4x4 DL MIMO CHG END */
                /*klockworks warning fix*/
                else if(0 < ueContext_p->ueCategory && ueContext_p->ueCategory <= UE_CAT_8)
                {
                    /** CA-TDD Changes Start **/
							/* SPR 23017 Fix Start */ 
						if(harqProcess_p->isTBTwoValid == TB_INVALID)
						{
							*mcsToBeUsedCW1_p = ((maxMCSSpecialSFForCFI_1[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
										/** CA-TDD Changes Stop **/
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) <
									/* SPR 8537 Fix Start */
									(mcsIndexToBeUsed)
									/* SPR 8537 Fix End */
									/** CA-TDD Changes Start **/
									?(maxMCSSpecialSFForCFI_1[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
										/** CA-TDD Changes Stop **/
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) :
									/* SPR 8537 Fix Start */
									(mcsIndexToBeUsed));
							/* SPR 8537 Fix End */
						}
						else
						{
                    *mcsToBeUsedCW1_p = ((maxMCSSpecialSFForCFI_1[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) <
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            /** CA-TDD Changes Start **/
                            ?(maxMCSSpecialSFForCFI_1[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) :
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
						/* SPR 23017 Fix End */ 
					}
            }	
            else
            {
#endif
                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) ||
                    (ueContext_p->ueCategory == UE_CAT_8)) && 
                    (harqProcess_p->numOfLayer == RANK_INDICATOR_4) )
                /* SPR 11355 Fix End */
                {
                    *mcsToBeUsedCW1_p = ((maxMCSSubframeMapForCFI_1_4x4[\
                                scheduledTick ]\
                                [harqProcess_p->assignedResourcesNB - 1]\
                                [numOfOverLappingRb]) < \
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            ? (maxMCSSubframeMapForCFI_1_4x4[((scheduledTick )\
                                    )]\
                                [harqProcess_p->assignedResourcesNB - 1]\
                                [numOfOverLappingRb]):\
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
                /* 4x4 DL MIMO CHG END */
                /*klockworks warning fix*/
                else if(0 < ueContext_p->ueCategory && ueContext_p->ueCategory <= UE_CAT_8)
                {
							/* SPR 23017 Fix Start */ 
						if(harqProcess_p->isTBTwoValid == TB_INVALID)
						{
                    *mcsToBeUsedCW1_p = ((maxMCSSubframeMapForCFI_1[\
                                scheduledTick ]\
                                [harqProcess_p->assignedResourcesNB - 1]\
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) < \
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            ? (maxMCSSubframeMapForCFI_1[((scheduledTick )\
                                    )]\
                                [harqProcess_p->assignedResourcesNB - 1]\
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]):\
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
						}
						else
						{
							*mcsToBeUsedCW1_p = ((maxMCSSubframeMapForCFI_1[\
										scheduledTick ]\
										[harqProcess_p->assignedResourcesNB - 1]\
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) < \
									/* SPR 8537 Fix Start */
									(mcsIndexToBeUsed)
                    /* SPR 8537 Fix End */
									? (maxMCSSubframeMapForCFI_1[((scheduledTick )\
											)]\
										[harqProcess_p->assignedResourcesNB - 1]\
										[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]):\
									/* SPR 8537 Fix Start */
									(mcsIndexToBeUsed));
						}
							/* SPR 23017 Fix End */ 
                }
#ifdef TDD_CONFIG
            }
#endif
        }
        break;
        /* SPR_5384_Fix Start */
        case 4:
        {
            /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
            if( ((ueContext_p->ueCategory == UE_CAT_5) ||
                (ueContext_p->ueCategory == UE_CAT_8)) && 
                (harqProcess_p->numOfLayer == RANK_INDICATOR_4) )
                /* SPR 11355 Fix End */
            {
                *mcsToBeUsedCW1_p = ((maxMCSSubframeMapForCFI_4_4x4[\
                            scheduledTick ]\
                            [harqProcess_p->assignedResourcesNB - 1]\
                            [numOfOverLappingRb]) < \
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed)
                        /* SPR 8537 Fix End */
                        ? (maxMCSSubframeMapForCFI_4_4x4[((scheduledTick )\
                                )]\
                            [harqProcess_p->assignedResourcesNB - 1]\
                            [numOfOverLappingRb]):\
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed));
                /* SPR 8537 Fix End */
            }
            /* 4x4 DL MIMO CHG END */
            /*klockworks warning fix*/
            else if(0 < ueContext_p->ueCategory && ueContext_p->ueCategory <= UE_CAT_8)
            {
						/* SPR 23017 Fix Start */ 
					if(harqProcess_p->isTBTwoValid == TB_INVALID)
					{
						/*SPR 5412 Fix Start*/
						*mcsToBeUsedCW1_p = ((maxMCSSubframeMapForCFI_4[\
									scheduledTick ]\
									[harqProcess_p->assignedResourcesNB - 1]\
									[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) < \
								/* SPR 8537 Fix Start */
								(mcsIndexToBeUsed)
								/* SPR 8537 Fix End */
								? (maxMCSSubframeMapForCFI_4[((scheduledTick )\
										)]\
									[harqProcess_p->assignedResourcesNB - 1]\
									[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]):\
								/* SPR 8537 Fix Start */
								(mcsIndexToBeUsed));
						/* SPR 8537 Fix End */
						/*SPR 5412 Fix End*/
					}
					else
					{
                /*SPR 5412 Fix Start*/
                *mcsToBeUsedCW1_p = ((maxMCSSubframeMapForCFI_4[\
                            scheduledTick ]\
                            [harqProcess_p->assignedResourcesNB - 1]\
									[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) < \
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed)
                        /* SPR 8537 Fix End */
                        ? (maxMCSSubframeMapForCFI_4[((scheduledTick )\
                                )]\
                            [harqProcess_p->assignedResourcesNB - 1]\
									[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]):\
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed));
                /* SPR 8537 Fix End */
                /*SPR 5412 Fix End*/
            }
						/* SPR 23017 Fix End */ 
				}
        }
        /* SPR_5384_Fix End */
        break;
        default:
        *mcsToBeUsedCW1_p = INVALID_MCS_INDEX;
        lteWarning("Invalid Cfi\n");
    }
}

/*************************************************************************************
 * Function Name    : calculateMCSToBeUsedForTB2
 * Inputs           : harqProcess_p - This holds pointer to DLHARQProcess structure,
 *                    scheduledTick - Scheduled TTI,
 *                    numOfOverLappingRb - This stores the numOfOverlappingRb,
 *                    ueContext_p - Pointer to DLUEContext,
 *                    mcsIndexToBeUsed -
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs           : mcsToBeUsedCW2_p - it will store the mcs for tb2
 * Returns           : None
 * Description       : This function calculate the MCS to be used for TB2 
 *************************************************************************************/
/* SPR 15909 fix start */
STATIC  void calculateMCSToBeUsedForTB2(DLHARQProcess
        *harqProcess_p,tickType_t scheduledTick,\
/* SPR 15909 fix end */        
        UInt8 numOfOverLappingRb, DLUEContext *ueContext_p,\
        UInt8 *mcsToBeUsedCW2_p,\
        /* SPR 8537 Fix Start */
        UInt8 mcsIndexToBeUsed,
        /* SPR 8537 Fix End */
        InternalCellIndex internalCellIndex)
{
    switch(currentCFIValue_g[internalCellIndex])
    {
        case 3:
        {
            /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
            if( ((ueContext_p->ueCategory == UE_CAT_5) ||
                (ueContext_p->ueCategory == UE_CAT_8)) && 
                    ( (harqProcess_p->numOfLayer == RANK_INDICATOR_4) || (harqProcess_p->numOfLayer == RANK_INDICATOR_3) )  )
                /* SPR 11355 Fix End */
            {
                *mcsToBeUsedCW2_p = ((maxMCSSubframeMapForCFI_3_4x4[\
                            scheduledTick ]\
                            [harqProcess_p->assignedResourcesNB - 1]\
                            [numOfOverLappingRb]) < \
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed)
                        /* SPR 8537 Fix End */
                        ? (maxMCSSubframeMapForCFI_3_4x4[((scheduledTick )\
                                )]\
                            [harqProcess_p->assignedResourcesNB - 1]\
                            [numOfOverLappingRb]):\
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed));
                /* SPR 8537 Fix End */
            }
            /* 4x4 DL MIMO CHG END */
            /*klockworks warning fix*/
            else if(0 < ueContext_p->ueCategory && ueContext_p->ueCategory <= UE_CAT_8)
            {
									/* SPR 23017 Fix Start */ 
									if(harqProcess_p->isTBOneValid == TB_INVALID)
									{
											*mcsToBeUsedCW2_p = ((maxMCSSubframeMapForCFI_3[\
																	scheduledTick ]\
																	[harqProcess_p->assignedResourcesNB - 1]\
																	[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) < \
															/* SPR 8537 Fix Start */
															(mcsIndexToBeUsed)
															/* SPR 8537 Fix End */
															? (maxMCSSubframeMapForCFI_3[((scheduledTick )\
																			)]\
																	[harqProcess_p->assignedResourcesNB - 1]\
																	[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]):\
															/* SPR 8537 Fix Start */
															(mcsIndexToBeUsed));
											/* SPR 8537 Fix End */
									}
									else
									{
                *mcsToBeUsedCW2_p = ((maxMCSSubframeMapForCFI_3[\
                            scheduledTick ]\
                            [harqProcess_p->assignedResourcesNB - 1]\
																	[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) < \
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed)
                        /* SPR 8537 Fix End */
                        ? (maxMCSSubframeMapForCFI_3[((scheduledTick )\
                                )]\
                            [harqProcess_p->assignedResourcesNB - 1]\
																	[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]):\
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed));
                /* SPR 8537 Fix End */
            }
									/* SPR 23017 Fix End */ 
							}
        }
        break;
        case 2:
        {
#ifdef TDD_CONFIG
            if( 3 == scheduledTick)
            {
                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) || (ueContext_p->ueCategory == UE_CAT_8)) && 
                        ( (harqProcess_p->numOfLayer == RANK_INDICATOR_4) || (harqProcess_p->numOfLayer == RANK_INDICATOR_3) )  )
                /* SPR 11355 Fix End */
                {
                    /** CA-TDD Changes Start **/
                    *mcsToBeUsedCW2_p = ((maxMCSSpecialSFForCFI_2_4x4[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
                                [numOfOverLappingRb]) <
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            /** CA-TDD Changes Start **/
                            ?(maxMCSSpecialSFForCFI_2_4x4[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
                                [numOfOverLappingRb]) :
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
                /* 4x4 DL MIMO CHG END */
                /*klockworks warning fix*/
                else if(0 < ueContext_p->ueCategory && ueContext_p->ueCategory <= UE_CAT_8)
                {
											/* SPR 23017 Fix Start */ 
											if(harqProcess_p->isTBOneValid == TB_INVALID) 
											{
                    /** CA-TDD Changes Start **/
                    *mcsToBeUsedCW2_p = ((maxMCSSpecialSFForCFI_2[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) <
																	/* SPR 8537 Fix Start */
																	(mcsIndexToBeUsed)
																	/* SPR 8537 Fix End */
																	/** CA-TDD Changes Start **/
																	?(maxMCSSpecialSFForCFI_2[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
																			/** CA-TDD Changes Stop **/
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) :
																	/* SPR 8537 Fix Start */
																	(mcsIndexToBeUsed));
													/* SPR 8537 Fix End */
											}
											else
											{
													/** CA-TDD Changes Start **/
													*mcsToBeUsedCW2_p = ((maxMCSSpecialSFForCFI_2[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
																			/** CA-TDD Changes Stop **/
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) <
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            /** CA-TDD Changes Start **/
                            ?(maxMCSSpecialSFForCFI_2[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) :
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
											/* SPR 23017 Fix End */ 
									}
            }
            else
            {
#endif
                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) || (ueContext_p->ueCategory == UE_CAT_8)) && 
                        ( (harqProcess_p->numOfLayer == RANK_INDICATOR_4) || (harqProcess_p->numOfLayer == RANK_INDICATOR_3) ) )
                /* SPR 11355 Fix End */
                {
                    *mcsToBeUsedCW2_p = ((maxMCSSubframeMapForCFI_2_4x4[\
                                scheduledTick ]\
                                [harqProcess_p->assignedResourcesNB - 1]\
                                [numOfOverLappingRb]) < \
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            ? (maxMCSSubframeMapForCFI_2_4x4[((scheduledTick )\
                                    )]\
                                [harqProcess_p->assignedResourcesNB - 1]\
                                [numOfOverLappingRb]):\
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
                /* 4x4 DL MIMO CHG END */
                /*klockworks warning fix*/
                else if(0 < ueContext_p->ueCategory && ueContext_p->ueCategory <= UE_CAT_8)
                {
											/* SPR 23017 Fix Start */ 
											if(harqProcess_p->isTBOneValid == TB_INVALID)
											{
													*mcsToBeUsedCW2_p = ((maxMCSSubframeMapForCFI_2[\
																			scheduledTick ]\
																			[harqProcess_p->assignedResourcesNB - 1]\
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) < \
																	/* SPR 8537 Fix Start */
																	(mcsIndexToBeUsed)
																	/* SPR 8537 Fix End */
																	? (maxMCSSubframeMapForCFI_2[((scheduledTick )\
																					)]\
																			[harqProcess_p->assignedResourcesNB - 1]\
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]):\
																	/* SPR 8537 Fix Start */
																	(mcsIndexToBeUsed));
													/* SPR 8537 Fix End */
											}
											else
											{
                    *mcsToBeUsedCW2_p = ((maxMCSSubframeMapForCFI_2[\
                                scheduledTick ]\
                                [harqProcess_p->assignedResourcesNB - 1]\
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) < \
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            ? (maxMCSSubframeMapForCFI_2[((scheduledTick )\
                                    )]\
                                [harqProcess_p->assignedResourcesNB - 1]\
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]):\
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
											/* SPR 23017 Fix End */ 
									}
#ifdef TDD_CONFIG
            }
#endif
        }
        break;
        case 1:
        {

#ifdef TDD_CONFIG
            if( 3 == scheduledTick)
            {
                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) || (ueContext_p->ueCategory == UE_CAT_8)) && 
                        ( (harqProcess_p->numOfLayer == RANK_INDICATOR_4) || (harqProcess_p->numOfLayer == RANK_INDICATOR_3) )  )
                /* SPR 11355 Fix End */
                {
                    /** CA-TDD Changes Start **/
                    *mcsToBeUsedCW2_p = ((maxMCSSpecialSFForCFI_1_4x4[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
                                [numOfOverLappingRb]) <
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            /** CA-TDD Changes Start **/
                            ?(maxMCSSpecialSFForCFI_1_4x4[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
                                [numOfOverLappingRb]) :
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
                /* 4x4 DL MIMO CHG END */
                /*klockworks warning fix*/
                else if(0 < ueContext_p->ueCategory && ueContext_p->ueCategory <= UE_CAT_8)
                {
											/* SPR 23017 Fix Start */ 
											if(harqProcess_p->isTBOneValid == TB_INVALID)
											{
													/** CA-TDD Changes Start **/
													*mcsToBeUsedCW2_p = ((maxMCSSpecialSFForCFI_1[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
																			/** CA-TDD Changes Stop **/
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) <
																	/* SPR 8537 Fix Start */
																	(mcsIndexToBeUsed)
																	/* SPR 8537 Fix End */
																	/** CA-TDD Changes Start **/
																	?(maxMCSSpecialSFForCFI_1[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
																			/** CA-TDD Changes Stop **/
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) :
																	/* SPR 8537 Fix Start */
																	(mcsIndexToBeUsed));
													/* SPR 8537 Fix End */
											}
											else
											{
                    /** CA-TDD Changes Start **/
                    *mcsToBeUsedCW2_p = ((maxMCSSpecialSFForCFI_1[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) <
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            /** CA-TDD Changes Start **/
                            ?(maxMCSSpecialSFForCFI_1[internalCellIndex][harqProcess_p->assignedResourcesNB - 1]
                                /** CA-TDD Changes Stop **/
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) :
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
											/* SPR 23017 Fix End */ 
									}
            }
            else
            {
#endif
                /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) || (ueContext_p->ueCategory == UE_CAT_8)) && 
                        ( (harqProcess_p->numOfLayer == RANK_INDICATOR_4) || (harqProcess_p->numOfLayer == RANK_INDICATOR_3) )  )
                /* SPR 11355 Fix End */
                {
                    *mcsToBeUsedCW2_p = ((maxMCSSubframeMapForCFI_1_4x4[\
                                scheduledTick]\
                                [harqProcess_p->assignedResourcesNB - 1]\
                                [numOfOverLappingRb]) < \
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            ? (maxMCSSubframeMapForCFI_1_4x4[((scheduledTick )\
                                    )]\
                                [harqProcess_p->assignedResourcesNB - 1]\
                                [numOfOverLappingRb]):\
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
                /* 4x4 DL MIMO CHG END */
                /*klockworks warning fix*/
                else if(0 < ueContext_p->ueCategory && ueContext_p->ueCategory <= UE_CAT_8)
                {
											/* SPR 23017 Fix Start */ 
											if(harqProcess_p->isTBOneValid == TB_INVALID)
											{
													*mcsToBeUsedCW2_p = ((maxMCSSubframeMapForCFI_1[\
																			scheduledTick]\
																			[harqProcess_p->assignedResourcesNB - 1]\
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) < \
																	/* SPR 8537 Fix Start */
																	(mcsIndexToBeUsed)
																	/* SPR 8537 Fix End */
																	? (maxMCSSubframeMapForCFI_1[((scheduledTick )\
																					)]\
																			[harqProcess_p->assignedResourcesNB - 1]\
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]):\
																	/* SPR 8537 Fix Start */
																	(mcsIndexToBeUsed));
													/* SPR 8537 Fix End */
											}
											else
											{
                    *mcsToBeUsedCW2_p = ((maxMCSSubframeMapForCFI_1[\
                                scheduledTick]\
                                [harqProcess_p->assignedResourcesNB - 1]\
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) < \
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed)
                            /* SPR 8537 Fix End */
                            ? (maxMCSSubframeMapForCFI_1[((scheduledTick )\
                                    )]\
                                [harqProcess_p->assignedResourcesNB - 1]\
																			[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]):\
                            /* SPR 8537 Fix Start */
                            (mcsIndexToBeUsed));
                    /* SPR 8537 Fix End */
                }
											/* SPR 23017 Fix End */ 
									}
#ifdef TDD_CONFIG
            }
#endif

        }
        break;
        /* SPR_5384_Fix Start */
        case 4:
        {
            /* 4x4 DL MIMO CHG START */
                /* SPR 11355 Fix Start */
            if( ((ueContext_p->ueCategory == UE_CAT_5) || (ueContext_p->ueCategory == UE_CAT_8)) && 
                    ( (harqProcess_p->numOfLayer == RANK_INDICATOR_4) || (harqProcess_p->numOfLayer == RANK_INDICATOR_3) )  )
                /* SPR 11355 Fix End */
            {
                *mcsToBeUsedCW2_p = ((maxMCSSubframeMapForCFI_4_4x4[\
                            scheduledTick ]\
                            [harqProcess_p->assignedResourcesNB - 1]\
                            [numOfOverLappingRb]) < \
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed)
                        /* SPR 8537 Fix End */
                        ? (maxMCSSubframeMapForCFI_4_4x4[((scheduledTick )\
                                )]\
                            [harqProcess_p->assignedResourcesNB - 1]\
                            [numOfOverLappingRb]):\
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed));
                /* SPR 8537 Fix End */
            }
            /* 4x4 DL MIMO CHG END */
            /*klockworks warning fix*/
            else if(0 < ueContext_p->ueCategory && ueContext_p->ueCategory <= UE_CAT_8)
            {
									/* SPR 23017 Fix Start */ 
									if(harqProcess_p->isTBOneValid == TB_INVALID)
									{
											/*SPR 5412 Fix Start*/
											*mcsToBeUsedCW2_p = ((maxMCSSubframeMapForCFI_4[\
																	scheduledTick ]\
																	[harqProcess_p->assignedResourcesNB - 1]\
																	[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]) < \
															/* SPR 8537 Fix Start */
															(mcsIndexToBeUsed)
															/* SPR 8537 Fix End */
															? (maxMCSSubframeMapForCFI_4[((scheduledTick )\
																			)]\
																	[harqProcess_p->assignedResourcesNB - 1]\
																	[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_ONE_IDX]):\
															/* SPR 8537 Fix Start */
															(mcsIndexToBeUsed));
											/* SPR 8537 Fix End */
											/*SPR 5412 Fix End*/
									}
									else
									{
                /*SPR 5412 Fix Start*/
                *mcsToBeUsedCW2_p = ((maxMCSSubframeMapForCFI_4[\
                            scheduledTick ]\
                            [harqProcess_p->assignedResourcesNB - 1]\
																	[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]) < \
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed)
                        /* SPR 8537 Fix End */
                        ? (maxMCSSubframeMapForCFI_4[((scheduledTick )\
                                )]\
                            [harqProcess_p->assignedResourcesNB - 1]\
																	[numOfOverLappingRb][ueContext_p->ueCategory-1][LAYER_TWO_IDX]):\
                        /* SPR 8537 Fix Start */
                        (mcsIndexToBeUsed));
                /* SPR 8537 Fix End */
                /*SPR 5412 Fix End*/
            }
									/* SPR 23017 Fix End */ 
							}
        }
        break;
        /* SPR_5384_Fix End */
        default:
        *mcsToBeUsedCW2_p = INVALID_MCS_INDEX;
        lteWarning("Invalid Cfi\n");
    }

}
/*Dynamic Pdcch Changes End */

/* 4x4 DL MIMO CHG START */
/*Rel 5.3.1 cyclomatic chg start*/
/*******************************************************************************
 * FUNCTION NAME :fillPMIAndCodebookIndexForDci2A
 * INPUTS :1)dciPduInfo_p - This is pointer to DCIPduInfo structure
 *         2)dlMIMOInfo_p - Pointer to DL MIMO info from DL ue context  
 *         3)numOfLayer - Layer used for this transmission.
 *         4)resrcAlloc_p - Pointer to ResourceAllocatorOutput.
 *         5)internalCellIndex - Cell-Index used at MAC
 * OUTPUTS:1)None
 * DESCRIPTION:This funtions will fill PMI and Code book Info for Dci 2A 
 ******************************************************************************/
 void fillPMIAndCodebookIndexForDci2A(DCIPduInfo              *dciPduInfo_p,
        DLMIMOInfo              *dlMIMOInfo_p,
        UInt8                   numOfLayer,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    /* for 4 Tx antenna */
    if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        if (dciPduInfo_p->numTB > 1)
        {
            if (numOfLayer == RI_LAYER_4)
                dciPduInfo_p->codeBookIndex = 2;
            else if (numOfLayer == RI_LAYER_3)
                dciPduInfo_p->codeBookIndex = 1;
            else
                dciPduInfo_p->codeBookIndex = 0;
        }
        else
        { /* This case of ReTx with only one TB enabled */
            if (numOfLayer >= RI_LAYER_3)
                dciPduInfo_p->codeBookIndex = 1;
            else
                dciPduInfo_p->codeBookIndex = 0;

        }
        dciPduInfo_p->numberOfSubbands = 1; 
        dciPduInfo_p->subbandtoPMIInfo[0]= dlMIMOInfo_p->precodingIndex;   
    }
    else
    {
        dciPduInfo_p->codeBookIndex = 0;
        dciPduInfo_p->numberOfSubbands = 0;
    }
    return;
}

/*******************************************************************************
 * FUNCTION NAME :fillPMIAndCodebookIndexForDci2Anteena2
 * INPUTS :1)dciPduInfo_p - This is pointer to DCIPduInfo structure
 *         2)resrcAlloc_p - Pointer to ResourceAllocatorOutput.
 * OUTPUTS:1)None
 * RETURN : None
 * DESCRIPTION:This funtions will fill PMI and Code book Info for TM4 
 *             for DCI format 2 for 2 anteenas    
 ******************************************************************************/
 void fillPMIAndCodebookIndexForDci2Anteena2(DCIPduInfo              *dciPduInfo_p,
        ResourceAllocatorOutput *resrcAlloc_p)
{
    /* SPR_19425 + */
    UInt16 ueIndex = dciPduInfo_p->ueIndex;
    DLUEContext *dlUEContext = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    /* SPR_19425 - */

    if (dciPduInfo_p->numTB > 1)
    {
        if ((resrcAlloc_p)->pmiValue == 2)/* sub band need fill */
        {  
            dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
            FILL_ACTUAL_PMI_VALUE(dciPduInfo_p,resrcAlloc_p); 
        }
        else if ((resrcAlloc_p)->pmiValue == 1) /* According to spec 36.212V9.4.0 Tabel 5.3.3.1.5-4*/
        {
            dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
            dciPduInfo_p->numberOfSubbands = 1;
            dciPduInfo_p->subbandtoPMIInfo[0]= 2;
        }
        else
        {
            dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
            dciPduInfo_p->numberOfSubbands = 1;
            dciPduInfo_p->subbandtoPMIInfo[0]= 1;
        }
    }
    else
    {
        if ((resrcAlloc_p->pmiValue == 5) || 
                (resrcAlloc_p->pmiValue == 6)) /* sub band need fill */
        {
            dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
            FILL_ACTUAL_PMI_VALUE(dciPduInfo_p,resrcAlloc_p); 
        }
        else
        {
            dciPduInfo_p->numberOfSubbands = 1;
            /* SPR_19425 + */
            /* if the UE reported RI = 2, PMI = 1 and only one TB is
             * scheduled, then PMI=2 or 3 can be used according to
             * 36.211 (table 6.3.4.2.3-1). But currently MAC will
             * select PMI=2  */
            if ((dlUEContext) && (1 == dlUEContext ->dlCQIInfo.wideBandPMIIndex) &&
                 (2 == dlUEContext->dlMIMOInfo.dlRi))
            {
                dciPduInfo_p->subbandtoPMIInfo[0]= (resrcAlloc_p)->pmiValue + 1;
                dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue + 2;
            }
            else
            {
            dciPduInfo_p->subbandtoPMIInfo[0]= (resrcAlloc_p)->pmiValue;   
            dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue + 1;
            }
            /* SPR_19425 - */

        }
    }
    return; 
}
/*******************************************************************************
 * FUNCTION NAME :fillPMIAndCodebookIndexForDci2Anteena4
 * INPUTS :1)dciPduInfo_p - This is pointer to DCIPduInfo structure
 *         2)numOfLayer - Layer used for this transmission.
 *         3)resrcAlloc_p - Pointer to ResourceAllocatorOutput.
 * OUTPUTS:1)None
 * RETURN : Nonw
 * DESCRIPTION:This funtions will fill PMI and Code book Info for TM4 
 *             for DCI format 2 for 4 anteenas   
 ******************************************************************************/
 void fillPMIAndCodebookIndexForDci2Anteena4(DCIPduInfo              *dciPduInfo_p,
        UInt8                   numOfLayer,
        ResourceAllocatorOutput *resrcAlloc_p)
{
    if (dciPduInfo_p->numTB > 1) /* For Two TB */
    {
        if (numOfLayer == RI_LAYER_4)
        {
            if ((resrcAlloc_p)->pmiValue == PMI_INDEX_50)/* sub band need fill */
            {
                dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
                FILL_ACTUAL_PMI_VALUE(dciPduInfo_p,resrcAlloc_p);
            }
            else 
            {
                dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
                dciPduInfo_p->numberOfSubbands = 1;
                dciPduInfo_p->subbandtoPMIInfo[0]= dciPduInfo_p->codeBookIndex;
                dciPduInfo_p->codeBookIndex = 
                    (dciPduInfo_p->codeBookIndex + PMI_INDEX_33 + 1);
            }
        }   
        else if(numOfLayer == RI_LAYER_3)
        {
            if ((resrcAlloc_p)->pmiValue == PMI_INDEX_33)/* sub band need fill */
            {
                dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
                FILL_ACTUAL_PMI_VALUE(dciPduInfo_p,resrcAlloc_p);
            }
            else
            {
                dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
                dciPduInfo_p->numberOfSubbands = 1;
                dciPduInfo_p->subbandtoPMIInfo[0]= dciPduInfo_p->codeBookIndex;
                dciPduInfo_p->codeBookIndex = 
                    (dciPduInfo_p->codeBookIndex + PMI_INDEX_16 + 1);
            }
        }   
        else
        {
            if ((resrcAlloc_p)->pmiValue == PMI_INDEX_16)/* sub band need fill */
            {
                dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
                FILL_ACTUAL_PMI_VALUE(dciPduInfo_p,resrcAlloc_p);
            }
            else
            {
                dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
                dciPduInfo_p->numberOfSubbands = 1;
                dciPduInfo_p->subbandtoPMIInfo[0]= dciPduInfo_p->codeBookIndex;
            }
        }   
    }
    else /* For One TB Re transmission case */
    {
        if(numOfLayer == RI_LAYER_4) /* if inital transmission is with 4 Layer and 2 TB */
        {
            if ((resrcAlloc_p)->pmiValue == PMI_INDEX_34)/* sub band need fill */
            {
                dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
                FILL_ACTUAL_PMI_VALUE(dciPduInfo_p,resrcAlloc_p);
            }
            else 
            {
                dciPduInfo_p->numberOfSubbands = 1;
                dciPduInfo_p->subbandtoPMIInfo[0]= (resrcAlloc_p)->pmiValue;
                dciPduInfo_p->codeBookIndex = 
                    ((resrcAlloc_p)->pmiValue + PMI_INDEX_17 + 1);
            }
        }
        else if (numOfLayer == RI_LAYER_3)/* if inital transmission is with 3 Layer and 2 TB */
        {
            if (dciPduInfo_p->isTBTwoValid) /* For TB2 */
            {
                if ((resrcAlloc_p)->pmiValue == PMI_INDEX_34)/* sub band need fill */
                {
                    dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
                    FILL_ACTUAL_PMI_VALUE(dciPduInfo_p,resrcAlloc_p);
                }
                else
                {
                    dciPduInfo_p->numberOfSubbands = 1;
                    dciPduInfo_p->subbandtoPMIInfo[0]= (resrcAlloc_p)->pmiValue;
                    dciPduInfo_p->codeBookIndex =
                        ((resrcAlloc_p)->pmiValue + PMI_INDEX_17 + 1);
                } 
            }
            else /* For TB1 */
            {   
                if ((resrcAlloc_p)->pmiValue == PMI_INDEX_17)
                {
                    dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
                    FILL_ACTUAL_PMI_VALUE(dciPduInfo_p,resrcAlloc_p);
                }
                else
                {
                    dciPduInfo_p->numberOfSubbands = 1;
                    dciPduInfo_p->subbandtoPMIInfo[0]= (resrcAlloc_p)->pmiValue;
                    dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue + 1;
                }    
            }   
        }
        else /* For Layer 1 */
        {
            if ((resrcAlloc_p)->pmiValue == PMI_INDEX_17)/* sub band need fill */
            {
                dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue;
                FILL_ACTUAL_PMI_VALUE(dciPduInfo_p,resrcAlloc_p);
            }
            else
            {
                dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue + 1;
                dciPduInfo_p->numberOfSubbands = 1;
                dciPduInfo_p->subbandtoPMIInfo[0]= (resrcAlloc_p)->pmiValue;
            }
        }
    }
    return; 
}
/*Rel 5.3.1 cyclomatic chg end*/
/*************************************************************************************
 * Function Name    : fillPMIAndCodebookIndex
 * Inputs           : dciPduInfo_p - This is pointer to DCIPduInfo structure,
 *                    dlMIMOInfo_p - Pointer to DL MIMO info from DL ue context,
 *                    numOfLayer - Layer used for this transmission,
 *                    resrcAlloc_p - Pointer to ResourceAllocatorOutput
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtions will fill PMI and Code book Info for TM3 and TM4.
 *************************************************************************************/
 void fillPMIAndCodebookIndex(DCIPduInfo              *dciPduInfo_p,
        DLMIMOInfo              *dlMIMOInfo_p,
        UInt8                   numOfLayer,
        ResourceAllocatorOutput *resrcAlloc_p,
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    /* The value in resAlloc is the PMI received from UE regardless 
     * of the number of layers i.e., for 4 Tx antenna it can have value 0-15 
     * and for 2 antenna 0-3, other values are for Aperiodic report when 
     * sub band PMI is used */
    /* In the function the variable "numOfLayer" is containing the value of 
     * Layer for which inital or re transmision has been take place in the
     * current TTI. This is not the RI value reported from UE. */
    switch(dciPduInfo_p->transmissionMode)
    {
        case TX_MODE_3:
        if (DCI_FORMAT_2A == dciPduInfo_p->dciFormat)
        {   
            /*Rel 5.3.1 cyclomatic chg start*/
            fillPMIAndCodebookIndexForDci2A(dciPduInfo_p,dlMIMOInfo_p,
                    /* +- SPR 17777 */
                    numOfLayer,
                    internalCellIndex);
            /*Rel 5.3.1 cyclomatic chg end*/                                
        }
        else
        {
            dciPduInfo_p->codeBookIndex = 0;
            dciPduInfo_p->numberOfSubbands = 0;
        }
        break;
        case TX_MODE_4:
        if (DCI_FORMAT_2 == dciPduInfo_p->dciFormat)
        {
            if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                /*Rel 5.3.1 cyclomatic chg start*/
                fillPMIAndCodebookIndexForDci2Anteena2(dciPduInfo_p,resrcAlloc_p);
                /*Rel 5.3.1 cyclomatic chg end*/                                
            }
            else if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {

                /*Rel 5.3.1 cyclomatic chg start*/
                fillPMIAndCodebookIndexForDci2Anteena4(dciPduInfo_p,
                        numOfLayer,resrcAlloc_p);
                /*Rel 5.3.1 cyclomatic chg end*/                                
            }
        }
        else
        {
            dciPduInfo_p->codeBookIndex = 0;
            dciPduInfo_p->numberOfSubbands = 0;
        }
        break;
        case TX_MODE_6:
        /* SPR 10306 fix start */ 
        FILL_TPMI_AND_PMI_CONF_VALUE(dciPduInfo_p,dlMIMOInfo_p,resrcAlloc_p, internalCellIndex);
        /* SPR 10306 fix end */ 
        break;
        default:
        break; 
    }
}
/* 4x4 DL MIMO CHG END */
/*Cyclomatic Complexity changes - starts here */
/*******************************************************************************
 * Function Name    : fillNumOfLayerAndTxSchemeTx1
 * Inputs           : dciPduInfo_p - This is pointer to DCIPduInfo structure,
 *                    pRi- This is rankindicator Value,
 *                    numOfLayer - Number of Layers
 * Outputs          : None
 * Returns          : None
 * Description      : This funtions will fill number of layer and transmission scheme 
 *                    for Tx1.
 ******************************************************************************/

static  void fillNumOfLayerAndTxSchemeTx1(DCIPduInfo *dciPduInfo_p,
        /* +- SPR 17777 */
        UInt8      pRi)
/* +- SPR 17777 */
{
    dciPduInfo_p->transmissionScheme =  FAPI_SINGLE_ANTENNA_PORT_0;
    dciPduInfo_p->numLayer = pRi;
}

/*******************************************************************************
 * Function Name    : fillNumOfLayerAndTxSchemeTx2
 * Inputs           : dciPduInfo_p - This is pointer to DCIPduInfo structure,
 *                    pRi- This is rankindicator Value,
 *                    numOfLayer - Number of Layers
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtions will fill number of layer and transmission scheme 
 *                    for Tx2.
 ******************************************************************************/
static  void fillNumOfLayerAndTxSchemeTx2(DCIPduInfo *dciPduInfo_p,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    dciPduInfo_p->transmissionScheme = FAPI_TX_DIVERSITY;
    /* In Tx mode 2, numOfTxAnteenas can be 1 or 2. */
    dciPduInfo_p->numLayer = cellParams_p->numOfTxAnteenas;

}

/*******************************************************************************
 * Function Name    : fillNumOfLayerAndTxSchemeTx3
 * Inputs           : dciPduInfo_p - This is pointer to DCIPduInfo structure,
 *                    pRi- This is rankindicator Value,
 *                    numOfLayer - Number of Layers
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtions will fill number of layer and transmission scheme 
 *                    for Tx3.
 ******************************************************************************/
static  void fillNumOfLayerAndTxSchemeTx3(DCIPduInfo *dciPduInfo_p,
        /* +- SPR 17777 */
        UInt8      numOfLayer,
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    if (DCI_FORMAT_1A == dciPduInfo_p->dciFormat)
    {
        dciPduInfo_p->transmissionScheme = FAPI_TX_DIVERSITY;
        /* 4x2 MIMO S*/
        dciPduInfo_p->numLayer = cellParams_p->numOfTxAnteenas;
        /* 4x2 MIMO E*/
    }
    else if(DCI_FORMAT_2A == dciPduInfo_p->dciFormat)
    {
        dciPduInfo_p->transmissionScheme = FAPI_LARGE_DELAY_CDD;
        /* 4x2 MIMO S*/
        if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            if (dciPduInfo_p->numTB > 1)
            {/*4x4 DL MIMO CHG START*/
                if (numOfLayer >= 3)
                {
                    dciPduInfo_p->numLayer = numOfLayer;
                }
                else/*4x4 DL MIMO CHG END*/
                {
                    dciPduInfo_p->numLayer = dciPduInfo_p->numTB;
                }
            }
            else
                dciPduInfo_p->numLayer = cellParams_p->numOfTxAnteenas;
        }
        else
        {
            dciPduInfo_p->numLayer = cellParams_p->numOfTxAnteenas;
        }
        /* 4x2 MIMO E*/
    }
}

/*******************************************************************************
 * Function Name    : fillNumOfLayerAndTxSchemeTx4
 * Inputs           : dciPduInfo_p - This is pointer to DCIPduInfo structure,
 *                    pRi- This is rankindicator Value,
 *                    numOfLayer - Number of Layers
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtions will fill number of layer and transmission scheme 
 *                    for Tx4.
 ******************************************************************************/
static  void fillNumOfLayerAndTxSchemeTx4(DCIPduInfo *dciPduInfo_p,
        /* +- SPR 17777 */
        UInt8      numOfLayer,
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    UInt8 cw = 0;
    if (DCI_FORMAT_1A == dciPduInfo_p->dciFormat)
    {
        dciPduInfo_p->transmissionScheme = FAPI_TX_DIVERSITY;

        /* In Tx mode 3, numOfTxAnteenas should be 2 */
        dciPduInfo_p->numLayer = cellParams_p->numOfTxAnteenas;
    } 
    else if(DCI_FORMAT_2 == dciPduInfo_p->dciFormat)
    {
        dciPduInfo_p->transmissionScheme = 
            FAPI_CLOSED_LOOP_SPATIAL_MULTIPLEXING;
        /* Number of layers should depend upon the number of valid 
         * codewords.If only TB1 valid or only TB2 valid then it 
         * should be 1 else 2 with 2 antenna port 
         */
        if( dciPduInfo_p->isTBOneValid)
        {
            cw++;
        } 

        if(dciPduInfo_p->isTBTwoValid)
        {
            cw++;
        }
        /* 4x2 MIMO E*/
        if((cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4) && (cw > 1))
        {/*4x4 DL MIMO CHG START*/
            if (numOfLayer >= 3)
            {
                dciPduInfo_p->numLayer = numOfLayer;
            }
            else/*4x4 DL MIMO CHG END*/
            {
                dciPduInfo_p->numLayer =   cw;
            }
        }
        else if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            if ((dciPduInfo_p->isTBOneValid) && (numOfLayer == 3))
                dciPduInfo_p->numLayer = 1;
            else if (numOfLayer >= 3)
                dciPduInfo_p->numLayer = 2;
            else
                dciPduInfo_p->numLayer = 1;
        }
        else
            dciPduInfo_p->numLayer = cw;
        /* 4x2 MIMO E*/
    } 
}

/*******************************************************************************
 * Function Name    : fillNumOfLayerAndTxSchemeTx6
 * Inputs           : dciPduInfo_p - This is pointer to DCIPduInfo structure,
 *                    pRi- This is rankindicator Value,
 *                    numOfLayer - Number of Layers
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtions will fill number of layer and transmission scheme 
 *                    for Tx6.
 ******************************************************************************/
static  void fillNumOfLayerAndTxSchemeTx6(DCIPduInfo *dciPduInfo_p,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    if (DCI_FORMAT_1A == dciPduInfo_p->dciFormat)
    {
        dciPduInfo_p->transmissionScheme = FAPI_TX_DIVERSITY;
        dciPduInfo_p->numLayer = cellParams_p->numOfTxAnteenas;
    }
    else if(DCI_FORMAT_1B == dciPduInfo_p->dciFormat)
    {
        dciPduInfo_p->transmissionScheme = FAPI_CLOSED_LOOP_RANK_1_PRECODING;
        dciPduInfo_p->numLayer = 1;
    }
}

/*******************************************************************************
 * Function Name    : fillNumOfLayerAndTxSchemeTx7
 * Inputs           : dciPduInfo_p - This is pointer to DCIPduInfo structure,
 *                    pRi- This is rankindicator Value,
 *                    numOfLayer - Number of Layers
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtions will fill number of layer and transmission scheme 
 *                    for Tx7.
 ******************************************************************************/
static  void fillNumOfLayerAndTxSchemeTx7(DCIPduInfo *dciPduInfo_p,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
{ 
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    dciPduInfo_p->numLayer = 1;
    if(DCI_FORMAT_1 == dciPduInfo_p->dciFormat)
    {
        dciPduInfo_p->transmissionScheme = FAPI_SINGLE_ANTENNA_PORT_5;
    }
    else if(DCI_FORMAT_1A == dciPduInfo_p->dciFormat)
    {
        if(NUM_OF_TX_ANTENNAS_1 == cellParams_p->numOfTxAnteenas)
        {
            dciPduInfo_p->transmissionScheme = FAPI_SINGLE_ANTENNA_PORT_0;
        }
        else
        {
            dciPduInfo_p->transmissionScheme = FAPI_TX_DIVERSITY;
            dciPduInfo_p->numLayer = cellParams_p->numOfTxAnteenas;
        }
    }

}

/*******************************************************************************
 * Function Name    : fillNumOfLayerAndTxSchemeTx8
 * Inputs           : dciPduInfo_p - This is pointer to DCIPduInfo structure,
 *                    pRi- This is rankindicator Value,
 *                    numOfLayer - Number of Layers
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtions will fill number of layer and transmission scheme 
 *                    for Tx8.
 ******************************************************************************/
static  void fillNumOfLayerAndTxSchemeTx8(DCIPduInfo *dciPduInfo_p,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
{
    UInt8 cw = 0;

    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    if(DCI_FORMAT_2B == dciPduInfo_p->dciFormat)
    {
        if((dciPduInfo_p->isTBOneValid) && (dciPduInfo_p->isTBTwoValid))
        {
            dciPduInfo_p->transmissionScheme = DUAL_LAYER_TX_PORT_7_AND_8;
            cw += 2;
        }
        else if( dciPduInfo_p->isTBOneValid)
        {
            dciPduInfo_p->transmissionScheme = FAPI_SINGLE_ANTENNA_PORT_7;
            cw++;
        }
        else if(dciPduInfo_p->isTBTwoValid)
        {
            dciPduInfo_p->transmissionScheme = FAPI_SINGLE_ANTENNA_PORT_8;
            cw++;
        }
        dciPduInfo_p->numLayer = cw;
    }
    else if(DCI_FORMAT_1A == dciPduInfo_p->dciFormat)
    {
        if(NUM_OF_TX_ANTENNAS_1 == cellParams_p->numOfTxAnteenas)
        {
            dciPduInfo_p->transmissionScheme = FAPI_SINGLE_ANTENNA_PORT_0;
            dciPduInfo_p->numLayer = 1;
        }
        else
        {
            dciPduInfo_p->transmissionScheme = FAPI_TX_DIVERSITY;
            dciPduInfo_p->numLayer = cellParams_p->numOfTxAnteenas;
        }
    }

}

/* 4x4 DL MIMO CHG END */
/*Cyclomatic Complexity changes - starts here */
/*******************************************************************************
 * Function Name    : fillNumOfLayerAndTxScheme
 * Inputs           : dciPduInfo_p - This is pointer to DCIPduInfo structure,
 *                    pRi- This is rankindicator Value,
 *                    numOfLayer - Number of Layers
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtions will fill number of layer and transmission scheme 
 *                    for all transmission modes.
 ******************************************************************************/
 void fillNumOfLayerAndTxScheme(DCIPduInfo *dciPduInfo_p,
        UInt8      pRi,
        UInt8      numOfLayer,
        InternalCellIndex internalCellIndex)
{
    switch(dciPduInfo_p->transmissionMode)
    {
        case TX_MODE_1: 
        case TX_MODE_ATTACH_NOT_COMPLETE: 
        /* +- SPR 17777 */
        fillNumOfLayerAndTxSchemeTx1(dciPduInfo_p, pRi);
        /* +- SPR 17777 */
        break;
        case  TX_MODE_2: 
        /* +- SPR 17777 */
        fillNumOfLayerAndTxSchemeTx2(dciPduInfo_p, internalCellIndex);
        /* +- SPR 17777 */
        break;

        case TX_MODE_3:
        /* +- SPR 17777 */
        fillNumOfLayerAndTxSchemeTx3(dciPduInfo_p,numOfLayer, internalCellIndex);
        /* +- SPR 17777 */
        break;
        case TX_MODE_4:
        /* +- SPR 17777 */
        fillNumOfLayerAndTxSchemeTx4(dciPduInfo_p, numOfLayer, internalCellIndex);
        /* +- SPR 17777 */
        break;

        /* + TM6_5.2 */
        case TX_MODE_6:
        /* +- SPR 17777 */
        fillNumOfLayerAndTxSchemeTx6(dciPduInfo_p, internalCellIndex);
        /* +- SPR 17777 */
        break;

        /* - TM6_5.2 */
        /* + TM7_8 Changes Start */ 
        case TX_MODE_7:
        /* +- SPR 17777 */
        fillNumOfLayerAndTxSchemeTx7(dciPduInfo_p,internalCellIndex);
        /* +- SPR 17777 */
        break;

        case TX_MODE_8:
        /* +- SPR 17777 */
        fillNumOfLayerAndTxSchemeTx8(dciPduInfo_p, internalCellIndex);
        /* +- SPR 17777 */
        break;

        /* - TM7_8 Changes End */ 

        default:
        lteWarning("Wrong case either txmode %d is wrong or " 
                "not supported in current release\n",
                dciPduInfo_p->transmissionMode);
        break;
    }
}

/***********************************************************************************************
 * Function Name    : fillDciCCEStructForSpatialDataMsg
 * Inputs           : msgType - It is a msgtype of MsgType structure,
 *                    dciCCEInfo_p - This stores the DciCCEInfo pointer which is passed to
 *                                   Mux or Encoder, so that they can write relevant
 *                                   information on this.
 *                    dlMsgInfo_p - Pointer to DLDataMsgInfo,
 *                    ueContext_p - Pointer to Downlink UE Context,  
 *                    harqProcess_p - Harq Process of the UE,
 *                    dlRi - This is RankIndicator value,
 *                    scheduledTick - This holds the Tick Value in which UE is
 *                                    expected to be scheduled,
 *                    withMuxFlag - This when set to one Indicates that we need to call
 *                                  directly invoke Mux Execution Logic,
 *                    muxQueueNode_p - This is pointer to the structure MultiplexerQueueNode      
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : resourceAllocOutput_p - Pointer to ResourceAllocatorOutput
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will Populate the the DCI CCE Info with relvant Informtion
 *                    for Spatial Multiplexing.
 ***********************************************************************************************/ 
STATIC  MacRetType fillDciCCEStructForSpatialDataMsg(MsgType msgType, 
        DciCCEInfo *dciCCEInfo_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        DLDataMsgInfo *dlMsgInfo_p ,
        DLUEContext *ueContext_p ,
        DLHARQProcess *harqProcess_p,
        /* +- SPR 17777 */
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        UInt32 withMuxFlag,
        MultiplexerQueueNode *muxQueueNode_p
        ,InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG                                        
        ,UInt8 dlSubframeNum
        ,UInt32 phyDelay
        ,UInt32 sysFrame
#endif 
        )
{
    MacRetType retType = MAC_SUCCESS;

    /* SPR_11050_Fix Start */
#ifdef FDD_CONFIG
    UInt32 harqSubFrameNum = 0;
    /* SPR 15909 fix start */
    tickType_t scheduledExpiryTick = 0;
    /* SPR 15909 fix end */
    /* + SPS_TDD_Changes */
#endif
    /* SPR_11050_Fix End */
    SchType schUsed = DRA_SCHEDULING;
    /* - SPS_TDD_Changes */

    /*TDD Tick overshoot changes start*/
#if defined OVERSHOOT_CORRECTION && !defined(DL_UL_SPLIT) && defined(TDD_CONFIG)
    UInt64 rem_cycles = 0;
    UInt64 cyclesReqd = 0;
#ifdef TDD_CONFIG
    UInt32 systemFrameNum = 0;
    UInt32 subFrameNum = 0;
#endif
#endif
    /*TDD Tick overshoot changes end*/

    /* + CQI_5.0 */   
    /*Populating the the DCI CCE Info with relvant Informtion*/  
    /* +- SPR 17777 */
    POPULATE_DCI_CCE_STRUCT_SPATIAL_MUL_DATA_MSG(
            msgType, dciCCEInfo_p ,
            resourceAllocOutput_p,dlMsgInfo_p ,ueContext_p ,
            harqProcess_p,
            scheduledTick, internalCellIndex);
    /* +- SPR 17777 */
    /* - CQI_5.0 */  
#ifdef LTE_EMBMS_SUPPORTED
    if(isCurrentSfEmbmsReserved_g[internalCellIndex])
    {
        dciCCEInfo_p->dciPduInfo.pA -= \
        (macReconfigSchedulerParamsDL_gp[internalCellIndex] + \
         schParamsIndexDL_g[internalCellIndex])->eMbmsReservedCellPowerOffset;  
    } 
#endif
 
    if(PNULL != muxQueueNode_p)
    {
        LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"sumWeightedQueueLoad = %d \n \
                sumLCPriority =%d",muxQueueNode_p->sumWeightedQueueLoad,
                muxQueueNode_p->sumLCPriority);
        /** CA Phase2 Changes Start **/
        /* CA_phase2_mux start */
        if(!(IS_PCELL_OF_UE(ueContext_p,internalCellIndex)))
        {
            withMuxFlag = FALSE;
        }
        /* CA_phase2_mux end */
        /** CA Phase2 Changes End **/

        if ( withMuxFlag )
        {
            /*TDD Tick overshoot changes start*/
#if defined OVERSHOOT_CORRECTION && !defined(DL_UL_SPLIT) && defined(TDD_CONFIG)

            GET_PROCESSING_CYCLES_LEFT(rem_cycles);
            /** CA-TDD Changes Start **/
            MIN_CC_REQD_TO_PROCEED_AT_MUX(cyclesReqd,internalCellIndex);
            /** CA-TDD Changes Stop **/

            if ( ( !noTimeToProcessflag_g ) && (
                        rem_cycles >= cyclesReqd ) )
            {
#endif
                /* PDCCH_PDSCH_WITHOUT_MUX Start */
                /*
                 * This function wrapper will call
                 * muxing function based on scheduling policy
                 */

                retType = lteLayer2MacLogicalChannelsMuxing(muxQueueNode_p,
                        scheduledTick,
                        ueContext_p,
                        internalCellIndex);

                /* PDCCH_PDSCH_WITHOUT_MUX End */
#if defined OVERSHOOT_CORRECTION && !defined(DL_UL_SPLIT) && defined(TDD_CONFIG)
            }
            else
            {
                /*Overload Detected*/
                retType = MAC_FAILURE;
                noTimeToProcessflag_g = 1;
                currTickOverLoad_g[internalCellIndex] = OVERLOAD_LEVEL_CRITICAL;

                freeDLHarqProcess(ueContext_p,
                        harqProcess_p,internalCellIndex);

                LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                        L2_SYS_FAIL,
                        getCurrentTick(),
                        __LINE__,
                        currTickOverLoad_g[internalCellIndex],
                        /** CA-TDD Changes Start **/
                        macOverLoadLevel_g[internalCellIndex],
                        /** CA-TDD Changes Stop **/
                        /*+ Dyn UEs To Be SchPerTTI +*/
                        dlUeSchdIndxInCurrTick_g,ulUeSchdIndxInCurrTick_g,0,0,__func__,
                        /*- Dyn UEs To Be SchPerTTI -*/
                        "Overload level changed.");
            }
#endif
            /*TDD Tick overshoot changes end*/
            freeMemPool(muxQueueNode_p);
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR," Calling funtion \
                    putEntryInMultiplexerQueue funtion");
            putEntryInMultiplexerQueue(muxQueueNode_p,internalCellIndex);
        }      
    }
    /* SPR_11050_Fix Start */
    else
    {
        if (SPS_ACTIVATION <= msgType && SPS_RETX >= msgType)
        {
            schUsed = SPS_SCHEDULING;
        }

        /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
        harqSubFrameNum = (scheduledTick + FDD_HARQ_OFFSET )%MAX_SUB_FRAME;
        scheduledExpiryTick = scheduledTick  + 8;
        /* - SPS_TDD_Changes */

        dlHarqTimerStart(ueContext_p->ueIndex,
                harqProcess_p->harqProcessId,harqSubFrameNum,
                scheduledExpiryTick,schUsed,internalCellIndex);
#elif TDD_CONFIG
        tddDlHarqTimerStart(ueContext_p->ueIndex,
                harqProcess_p->harqProcessId,
                dlSubframeNum,
                scheduledTick-phyDelay,
                sysFrame,
                /* + SPS_TDD_Changes */
                schUsed,
                /* - SPS_TDD_Changes */
                internalCellIndex
                ); 
#endif
    }
    /* SPR_11050_Fix End */

    return retType;
}

/*Cyclomatic Complexity changes - ends here*/


#if (defined(TDD_CONFIG) && defined(REAL_UE_ENVIORNMENT))
/*SPR 10142 fix start*/
/***********************************************************************************************
 * Function Name    : calculateMCSAndTBSizeForTDDSplSubFrameTB1
 * Inputs           : isSplSubFrame - isSplSubFrame( 1 or 6 )
 *                    harqProcess_p - Pointer to DLHARQProcess
 *                    ueContext_p - Pointer to DLUEContext
 * Outputs          : harqProcess_p - Pointer to DLHARQProcess
 * Returns          : void
 * Description      : This function will calculate the MCS and TB Size to be used for
 *                    TDD Special Subframe.
 ***********************************************************************************************/ 
STATIC  void calculateMCSAndTBSizeForTDDSplSubFrameTB1( UInt8 isSplSubFrame, 
                                                           DLHARQProcess *harqProcess_p,
                                                           DLUEContext *ueContext_p)
{
    UInt16 rbUsed = 0;
    if (isSplSubFrame)
    {
        if (ueContext_p->dlCQIInfo.mcsIndexCodeWordOne >= 27)
        {
            harqProcess_p->dlHARQTBOneInfo.mcsIndex
                = harqProcess_p->dlHARQTBOneInfo.mcsIndex - 1;
        }
        else if (ueContext_p->dlCQIInfo.mcsIndexCodeWordOne == 26)
        {
            harqProcess_p->dlHARQTBOneInfo.mcsIndex  = 22;

        }
        else if ((ueContext_p->dlCQIInfo.mcsIndexCodeWordOne < 26)
                && (ueContext_p->dlCQIInfo.mcsIndexCodeWordOne > 3))
        {
            harqProcess_p->dlHARQTBOneInfo.mcsIndex
                = harqProcess_p->dlHARQTBOneInfo.mcsIndex - 3;
        }
        else if (ueContext_p->dlCQIInfo.mcsIndexCodeWordOne <= 3)
        {
            harqProcess_p->dlHARQTBOneInfo.mcsIndex = 0;
        }
        /* SPR 18122 Changes Start*/
        rbUsed = DIVIDE_BY_FOUR(harqProcess_p->assignedResourcesNB * 3);
        /* SPR 18122 Changes End*/
        if (rbUsed < 1)
        {
            rbUsed = 1;
        }
        /* SPR 11355 Fix Start */
        if( ((ueContext_p->ueCategory == UE_CAT_5) ||
                    (ueContext_p->ueCategory == UE_CAT_8))&&
                (harqProcess_p->numOfLayer == RANK_INDICATOR_4) )
            /* SPR 11355 Fix End */
        {
            harqProcess_p->dlHARQTBOneInfo.tbSize = \
                                                    determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                                            rbUsed,
                                                            TB_MAPPED_TO_TWO_LAYER);
        }
        else
        {
            harqProcess_p->dlHARQTBOneInfo.tbSize = \
                                                    determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                                            rbUsed,
                                                            TB_MAPPED_TO_ONE_LAYER);
        }
    }
}


/***********************************************************************************************
 * Function Name    : calculateMCSAndTBSizeForTDDSplSubFrameTB2
 * Inputs           : isSplSubFrame - isSplSubFrame( 1 or 6 )
 *                    harqProcess_p - Pointer to DLHARQProcess
 *                    ueContext_p - Pointer to DLUEContext
 * Outputs          : harqProcess_p - Pointer to DLHARQProcess
 * Returns          : void
 * Description      : This function will calculate the MCS and TB Size to be used for
 *                    TDD Special Subframe.
 ***********************************************************************************************/ 
STATIC  void calculateMCSAndTBSizeForTDDSplSubFrameTB2( UInt8 isSplSubFrame, 
                                                           DLHARQProcess *harqProcess_p,
                                                           DLUEContext *ueContext_p)
{
    UInt16 rbUsed = 0;
    if (isSplSubFrame)
    {
        if (ueContext_p->dlCQIInfo.mcsIndexCodeWordTwo >= 27)
        {
            harqProcess_p->dlHARQTBTwoInfo.mcsIndex
                = harqProcess_p->dlHARQTBTwoInfo.mcsIndex - 1;
        }
        else if (ueContext_p->dlCQIInfo.mcsIndexCodeWordTwo == 26)
        {
            harqProcess_p->dlHARQTBTwoInfo.mcsIndex  = 22;

        }
        else if ((ueContext_p->dlCQIInfo.mcsIndexCodeWordTwo < 26)
                && (ueContext_p->dlCQIInfo.mcsIndexCodeWordTwo > 3))
        {
            harqProcess_p->dlHARQTBTwoInfo.mcsIndex
                = harqProcess_p->dlHARQTBTwoInfo.mcsIndex - 3;
        }
        else if (ueContext_p->dlCQIInfo.mcsIndexCodeWordTwo <= 3)
        {
            harqProcess_p->dlHARQTBTwoInfo.mcsIndex = 0;
        }
        /* SPR 18122 Changes Start*/
        rbUsed = DIVIDE_BY_FOUR(harqProcess_p->assignedResourcesNB * 3);
        /* SPR 18122 Changes End*/
        if (rbUsed < 1)
        {
            rbUsed = 1;
        }

        /* SPR 11355 Fix Start */
        if( ((ueContext_p->ueCategory == UE_CAT_5) ||
                    (ueContext_p->ueCategory == UE_CAT_8))&&
                ( (harqProcess_p->numOfLayer == RANK_INDICATOR_4) ||
                  (harqProcess_p->numOfLayer == RANK_INDICATOR_3)))
            /* SPR 11355 Fix End */
        {
            harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                                                    determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex, 
                                                            rbUsed,
                                                            TB_MAPPED_TO_TWO_LAYER);
        }
        else
        {
            harqProcess_p->dlHARQTBTwoInfo.tbSize = \
                                                    determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex, rbUsed,
                                                            TB_MAPPED_TO_ONE_LAYER);
        }
    }
}
/*SPR 10142 fix end*/
#endif
/* Cyclomatic_complexity_changes_start */
/***********************************************************************************************
 * Function Name    : calculateSubFrameIndex
 * Inputs           : scheduledTick
 *                    sfIndex- sub Frame Index
 *                    internalCellIndex
 * Outputs          : None.
 * Returns          : void
 * Description      : This function will calculate the Sub rame index.
 ************************************************************************************************/

void calculateSubFrameIndex(/* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
		UInt8 *sfIndex 
#if defined (TDD_CONFIG) 
        ,InternalCellIndex internalCellIndex
#endif
        )
        /* +- SPR 17777 */
{
    UInt8 subFrameIndex = *sfIndex;
/* SPR 21631 Start */
    /* SPR 23393 Changes Start */
    if (0 == (scheduledTick % MAX_SUB_FRAME))
    {
        subFrameIndex = 0;
    }
        /* SPR 23393 Changes Start */
    else if (5 == (scheduledTick % MAX_SUB_FRAME))
        /* SPR 23393 Changes End */
    {
        subFrameIndex = 1;
    }
#ifdef TDD_CONFIG
        /* SPR 23393 Changes Start */
    else if (1 == (scheduledTick % MAX_SUB_FRAME) )
        /* SPR 23393 Changes End */
    {
	    subFrameIndex = 3;
    }
        /* SPR 23393 Changes Start */
    else if (6 == (scheduledTick % MAX_SUB_FRAME) )
        /* SPR 23393 Changes End */
    {
	     if((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
             cellParams_p->subFrameAssign <= UL_DL_CONFIG_2) ||
                (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                ->cellParams_p->subFrameAssign))
	     {
             subFrameIndex = 3;
	     }
	     else
	     {
             subFrameIndex = 2;
	     }
    }
#endif
    else
    {
        subFrameIndex = 2;
    }

    *sfIndex = subFrameIndex;

    return;
}

/***********************************************************************************************
 * Function Name    : updateMcsAndTBSizeforAssignedRBOne
 * Inputs           : subFrameIndex,
 *                    harqProcess_p,
 *                    ueContext_p
 * Outputs          : None.
 * Returns          : void
 * Description      : This function updates MCS and TB size in case of assigned RB is one and
 *                    subframe is not 0 and 5.
 ************************************************************************************************/
void updateMcsAndTBSizeforAssignedRBOne(UInt8 subFrameIndex,
        DLHARQProcess *harqProcess_p,
        DLUEContext *ueContext_p)
{
	/* SPR23580 Start */
	/* code rate can only exceed when MCS=6, RB=1*/
        if (harqProcess_p->dlHARQTBOneInfo.mcsIndex != 28)
	/* SPR23580 End */
        {
            if ((harqProcess_p->isTBOneValid)
                    && (1 == harqProcess_p->assignedResourcesNB)
                    && (6 == harqProcess_p->dlHARQTBOneInfo.mcsIndex))
            {
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = 5;
                /*4x4 DL MIMO CHG START*/
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) || (ueContext_p->ueCategory == UE_CAT_8)) &&
                        (harqProcess_p->numOfLayer == RANK_INDICATOR_4) )
                    /* SPR 11355 Fix End */
                {
                    harqProcess_p->dlHARQTBOneInfo.tbSize =
                        determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_TWO_LAYER);
                }
                else
                { 
                    harqProcess_p->dlHARQTBOneInfo.tbSize =
                        determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_ONE_LAYER);
                }
                /*4x4 DL MIMO CHG END*/
            }
            else
            {
            }
        }
        else
        {
            /* We can only reduce MCS in this case since 
             * TB Size of TBOne can't change*/
        }
        if ((harqProcess_p->dlHARQTBTwoInfo.mcsIndex != 28)) 
        {
            if ((harqProcess_p->isTBTwoValid)
                    && (1 == harqProcess_p->assignedResourcesNB)
                    && (6 == harqProcess_p->dlHARQTBTwoInfo.mcsIndex))
            {
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex = 5;
                /*4X4 DL MIMO CHG START*/
                /* SPR 11355 Fix Start */
                if( ((ueContext_p->ueCategory == UE_CAT_5) ||
                            (ueContext_p->ueCategory == UE_CAT_8)) && 
                        ( (harqProcess_p->numOfLayer == RANK_INDICATOR_4) ||
                          (harqProcess_p->numOfLayer == RANK_INDICATOR_3) )  )
                    /* SPR 11355 Fix End */
                {
                    harqProcess_p->dlHARQTBTwoInfo.tbSize =
                        determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_TWO_LAYER);
                }
                else
                {
                    harqProcess_p->dlHARQTBTwoInfo.tbSize =
                        determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
                                harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_ONE_LAYER);
                }
                /*4X4 DL MIMO CHG END*/ 
            }
            else
            {
            }       
        }
        else
        {

        }
	/* SPR23580 Start */
    return;
	/* SPR23580 End */
}
/* Cyclomatic_complexity_changes_end */

/****************************************************************************************
 * Function Name    : fillHarqInfoForReTransmission
 * Inputs           : dlMsgTypeMsgInfo_p - This stores DLMsgTypeMsgInfo Pointer which
 *                                         store the information of those UE for which
 *                                         the PDCCH region has been allocated. It is a
 *                                         Pointer to DLMsgTypeMsgInfo,
 *                    dlStrategyTxNode_p - Pointer to Downlink Strategy Tx Node,
 *                    dlMsgInfo_p - Pointer to DLDataMsgInfo,
 *                    harqProcess_p - Harq Process of the UE,
 *                    dciCCEInfo_p - Pointer to DciCCEInfo,
 *                    scheduledTick - This holds the Tick Value in which UE is
 *                                    expected to be scheduled,
 *                    ueContext_p - Pointer to Downlink UE Context            
 *                    internalCellIndex - Cell-Index used at MAC
 *                    dlSubframeNum - DL SF 
 *                    phyDelay - Phy delay
 *                    sysFrame - System frame number 
 * Outputs          : None
 * Returns          : MAC_FAILURE/MAC_SUCCESS
 * Description      : This function will fill retx info. Also, if it is PCell of
 *                    UE then start harq timer.
 *****************************************************************************************/ 
STATIC MacRetType   fillHarqInfoForReTransmission(DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
                                DLStrategyTxNode *dlStrategyTxNode_p,
                                DLDataMsgInfo *dlMsgInfo_p,
                                DLHARQProcess *harqProcess_p,
                                DciCCEInfo *dciCCEInfo_p,
                                /* SPR 15909 fix start */
                                tickType_t scheduledTick,
                                /* SPR 15909 fix end */
                                DLUEContext *ueContext_p,
                                ResourceAllocatorOutput *resourceAllocOutput_p,
                                InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
                               ,UInt8 dlSubframeNum
                               ,UInt32 phyDelay
                               ,UInt32 sysFrame
#endif
               )
{

    /* SPR_11050_Fix Start */
#ifdef FDD_CONFIG
    UInt32 harqSubFrameNum = 0;
    /* SPR 15909 fix start */
    tickType_t scheduledExpiryTick = 0;
    /* SPR 15909 fix end */
    /* + SPS_TDD_Changes */
#endif
    /* SPR_11050_Fix End */
    SchType schUsed = DRA_SCHEDULING;
    /* - SPS_TDD_Changes */

    static UInt8 sizeofMuxNode = sizeof(MultiplexerQueueNode);

    /* Cyclomatic Complexity changes start*/
    fillDCICCEInfoForReTransmission(dciCCEInfo_p,harqProcess_p,
            dlMsgTypeMsgInfo_p,dlMsgInfo_p,ueContext_p,
            scheduledTick,resourceAllocOutput_p,
            internalCellIndex);
    /* Cyclomatic Complexity changes end*/

    /* SPR_11050_Fix Start */
    /* + SPS_TDD_Changes */
    if (SPS_ACTIVATION <= dlMsgTypeMsgInfo_p->msgType && 
            SPS_RETX >= dlMsgTypeMsgInfo_p->msgType)
    {
        schUsed = SPS_SCHEDULING;
    }
#ifdef FDD_CONFIG
    harqSubFrameNum = (scheduledTick + FDD_HARQ_OFFSET )%MAX_SUB_FRAME;
    scheduledExpiryTick = scheduledTick  + 8;
    /* - SPS_TDD_Changes */
#endif         
    /** SPR 12348 Changes Start **/ 
    if(!(IS_PCELL_OF_UE(ueContext_p,internalCellIndex)))
    {
        MultiplexerQueueNode  * muxQueueNode_p = PNULL;
        GET_MEM_FROM_POOL(MultiplexerQueueNode , muxQueueNode_p , sizeofMuxNode , PNULL);

        /* + Coverity 24498 */
        if( PNULL == muxQueueNode_p )
        {
            return MAC_FAILURE ;
        }
        /* - Coverity 24498 */
        /* + Coverity_69461 */
#ifdef FDD_CONFIG                
        /* + SPR 11208 Changes */
        POPULATE_MUX_NODE_INFO(scheduledTick , dlStrategyTxNode_p ,
                dlMsgInfo_p,muxQueueNode_p , dlMsgTypeMsgInfo_p,
                ueContext_p, dciCCEInfo_p, 
                internalCellIndex);     
        /* - SPR 11208 Changes */
#elif TDD_CONFIG

        POPULATE_MUX_NODE_INFO(scheduledTick , dlStrategyTxNode_p ,
                dlMsgInfo_p,muxQueueNode_p , dlMsgTypeMsgInfo_p,
                ueContext_p, dciCCEInfo_p, internalCellIndex, dlSubframeNum,
                phyDelay, sysFrame);     
#endif                
        /* - Coverity_69461 */
        putEntryInMultiplexerQueue(muxQueueNode_p,internalCellIndex);
    }
    else
    {

#ifdef FDD_CONFIG
        dlHarqTimerStart(ueContext_p->ueIndex,
                harqProcess_p->harqProcessId,harqSubFrameNum,
                scheduledExpiryTick,schUsed,internalCellIndex);

#elif TDD_CONFIG
        tddDlHarqTimerStart(ueContext_p->ueIndex,
                harqProcess_p->harqProcessId,
                dlSubframeNum,
                scheduledTick-phyDelay,
                sysFrame,
                /* + SPS_TDD_Changes */
                schUsed,
                /* - SPS_TDD_Changes */
                internalCellIndex
                ); 
#endif
    }
    /** SPR 12348 Changes End **/ 
    /* SPR_11050_Fix End */
    return MAC_SUCCESS;

}

