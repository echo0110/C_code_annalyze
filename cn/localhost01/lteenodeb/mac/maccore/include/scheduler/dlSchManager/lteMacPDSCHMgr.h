/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 * $Id:  
 *
 ****************************************************************************
 *
 *  File Description : THIS FILE CONTAINS THE DATA STRUCTURES AND THE FUNCTION
 *                     DECLEARATION OF PDSCH  MODULE. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacPDSCHMgr.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.13  2009/07/27 08:07:19  gur12140
 * Incorporated Comments
 *
 * Revision 1.12  2009/07/20 05:17:52  gur12140
 * Replaced MACModuleLogDetail_g wih PDSCH_MGR
 *
 * Revision 1.11  2009/06/05 06:20:54  gur12140
 * Comments Incorporated
 *
 * Revision 1.10  2009/06/01 12:57:51  gur12140
 * Removed Compilation Error due to change in cellConfig_g structure
 *
 * Revision 1.8  2009/05/20 12:12:19  gur12140
 * Incorporated Comments
 *
 * Revision 1.7  2009/05/10 11:48:06  gur12140
 * Did Indentation
 *
 * Revision 1.6  2009/05/10 11:43:33  gur12140
 * Removed Warnings
 *
 * Revision 1.5  2009/05/10 09:10:10  gur12140
 * Removed Encoder Structure
 * 
 * *
 ****************************************************************************/

#ifndef LTE_MAC_PDSCH_MGR_H
#define LTE_MAC_PDSCH_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacPDCCHMgr.h"
#include "lteTypes.h"
#include "lteMacDLResourceAllocator.h"
#include "lteMacDLUEContext.h"
#include "lteMacULUEContext.h"
#include "lteMacCQI.h"
#include "lteMacEncoder.h"
#include "lteMacMultiplexer.h"
/* + CQI_5.0 */
#include "lteFapiMacPhyInterface.h"
/* - CQI_5.0 */
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define PDSCH_MGR  "pdsch_mgr"
#define PMI_CONF_ZERO  0
#define PMI_CONF_ONE   1

/* + CQI_5.0 */\
/* SPR 5508 Start */
#define FILL_ACTUAL_PMI_VALUE(dciPduInfo_p,resrcAlloc_p) \
{\
   UInt8 subbandCount = resrcAlloc_p->numberOfSubbands; \
   if ( subbandCount < MAX_SUBBANDS_PMI_INFO )\
   {\
       dciPduInfo_p->numberOfSubbands = resrcAlloc_p->numberOfSubbands; \
       while(subbandCount--)\
       {\
           dciPduInfo_p->subbandtoPMIInfo[subbandCount] = \
           (resrcAlloc_p)->subbandtoPMIInfo[subbandCount]; \
       }\
   }\
   else\
   {\
       dciPduInfo_p->numberOfSubbands = 0; \
       lteWarning("subbandCount [%d] out of range",subbandCount);\
   }\
}\
/* SPR 5508 End */
/* - CQI_5.0 */
#define POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_MUL_COMMON_CHANL_MSG( messageType , \
        numofTB, resrcBlkType,modScheme, dciCCEInfo_p, \
        resrcAlloc,dlMsgInfo_p,txMode,dci1ATpc) \
{ \
    DCIPduInfo *dciPduInfo_p = PNULL; \
    dciPduInfo_p = &dciCCEInfo_p->dciPduInfo; \
    dciCCEInfo_p->msgType  = messageType; \
    dciPduInfo_p->resrcAssgmntBitMap = resrcAlloc.resourceAssignmentBitMap; \
    dciPduInfo_p->nGAP = resrcAlloc.nGAP; \
    dciPduInfo_p->resrcAllocType =  resrcAlloc.ratPolicy; \
    dciPduInfo_p->transmissionMode = txMode; \
    dciPduInfo_p->numTB = numofTB; \
    dciPduInfo_p->virtulResrcBlkType = resrcBlkType; \
    dciPduInfo_p->tb1Info.modulationType = modScheme;  \
    dciPduInfo_p->dataTB_p =  dlMsgInfo_p->data_p; \
    dciPduInfo_p->tb1Info.tbSize = dlMsgInfo_p->dataLen; \
    dciPduInfo_p->dciFormat = dlMsgInfo_p->dciFormat; \
    /* SPR 941 */ \
    dciPduInfo_p->isTBOneValid =  TRUE; \
    dciPduInfo_p->isTBTwoValid =  FALSE; \
    /*SPR 1826 changes start*/\
    dciPduInfo_p->nPRB = 0;\
    /*SPR 1826 changes end*/\
    /*SPR 1848 changes start*/\
    /*SPR 19285 changes start*/\
    dciPduInfo_p->tpc = 1;\
    /*SPR 19285 changes End*/\
    /*SPR 1848 changes end*/\
    /*Cyclomatic Complexity changes - starts here */ \
    /* 4x4 DL MIMO CHG START */\
    fillNumOfLayerAndTxScheme(dciPduInfo_p,1,1,internalCellIndex);\
    /* 4x4 DL MIMO CHG END */\
    /*Cyclomatic Complexity changes - ends here */ \
    /* SPR 1488 + SPR 1589 + SPR 1686 Starts */\
    /*SPR 1492 changes start*/\
/*DCI_1C_Comment Start */\
    if((messageType == BCCH_MSG || messageType == PCCH_MSG || messageType == RAR_MSG) \
            && (DCI_FORMAT_1C != dlMsgInfo_p->dciFormat)) \
    {\
       dciPduInfo_p->tpc = dci1ATpc ;\
       dciPduInfo_p->nPRB = dci1ATpc; \
    }\
/*DCI_1C_Comment End */\
    /*SPR 1492 changes end*/\
    /* SPR 1488 + SPR 1589 + SPR 1686 Ends	*/\
    /* SPR 10411 changes start */\
    dciPduInfo_p->numOfRBs = resrcAlloc.allocatedRB;\
    /* SPR 10411 changes end */\
    /* + SPR 17502 */\
    /* Assign Ue category with MIN value as this will be ignored in case of \
    * common channel */\
     dciPduInfo_p->ueCategory = MAC_MIN_UE_CATEGORY; \
     /* - SPR 17502*/\
}  

/*  SPR  3444 changes start */
#ifdef FDD_CONFIG
#define POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_CCCH_MSG( messageType , \
                numofTB, resrcBlkType,modScheme, dciCCEInfo_p, \
                resrcAlloc,dlMsgInfo_p,txMode, internalCellIndex\
                /* + SPR 17733 */\
                ,harqProcess_p \
                ) \
                /* - SPR 17733 */\
{ \
    DCIPduInfo *dciPduInfo_p = PNULL; \
    dciPduInfo_p = &dciCCEInfo_p->dciPduInfo; \
    dciCCEInfo_p->msgType  = messageType; \
    dciPduInfo_p->resrcAssgmntBitMap = resrcAlloc.resourceAssignmentBitMap; \
    dciPduInfo_p->resrcAllocType =  resrcAlloc.ratPolicy; \
    dciPduInfo_p->transmissionMode = txMode; \
    dciPduInfo_p->numTB = numofTB; \
    dciPduInfo_p->virtulResrcBlkType = resrcBlkType; \
    dciPduInfo_p->tb1Info.modulationType = modScheme;  \
    dciPduInfo_p->dataTB_p =  dlMsgInfo_p->data_p; \
    dciPduInfo_p->tb1Info.tbSize = dlMsgInfo_p->dataLen; \
    dciPduInfo_p->dciFormat = dlMsgInfo_p->dciFormat; \
    dciPduInfo_p->isTBOneValid =  TRUE; \
    dciPduInfo_p->isTBTwoValid =  FALSE; \
    dciPduInfo_p->nPRB = 0;\
    /*SPR 19285 changes start*/\
    dciPduInfo_p->tpc = 1;\
    /*SPR 19285 changes End*/\
    /* + SPR 17733 */\
    if((PNULL != harqProcess_p) && (TRUE==harqProcess_p->isTBOneValid))\
    {\
        dciPduInfo_p->tb1Info.txIndicator = harqProcess_p->dlHARQTBOneInfo.newDataIndicator;\
    }\
    /* - SPR 17733 */\
    /*Cyclomatic Complexity changes - starts here */ \
    /* 4x4 DL MIMO CHG START */\
    fillNumOfLayerAndTxScheme(dciPduInfo_p,1,1,internalCellIndex);\
    /* 4x4 DL MIMO CHG END */\
    /*Cyclomatic Complexity changes - ends here */ \
    /* SPR 10411 changes start */\
    dciPduInfo_p->numOfRBs = resrcAlloc.allocatedRB;\
    /* SPR 10411 changes end */\
    /* + SPR 17502 */\
    /* Assign Ue category with MIN value as this will be ignored in case of \
     * CCCH message */\
    dciPduInfo_p->ueCategory = MAC_MIN_UE_CATEGORY; \
    /* - SPR 17502*/\
}
#elif TDD_CONFIG
#define POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_CCCH_MSG( messageType , \
                numofTB, resrcBlkType,modScheme, dciCCEInfo_p, \
                resrcAlloc,dlMsgInfo_p,txMode, internalCellIndex)\
{ \
        DCIPduInfo *dciPduInfo_p = PNULL; \
        dciPduInfo_p = &dciCCEInfo_p->dciPduInfo; \
        dciCCEInfo_p->msgType  = messageType; \
        dciPduInfo_p->resrcAssgmntBitMap = resrcAlloc.resourceAssignmentBitMap; \
        dciPduInfo_p->resrcAllocType =  resrcAlloc.ratPolicy; \
        dciPduInfo_p->transmissionMode = txMode; \
        dciPduInfo_p->numTB = numofTB; \
        dciPduInfo_p->virtulResrcBlkType = resrcBlkType; \
        dciPduInfo_p->tb1Info.modulationType = modScheme;  \
        dciPduInfo_p->dataTB_p =  dlMsgInfo_p->data_p; \
        dciPduInfo_p->tb1Info.tbSize = dlMsgInfo_p->dataLen; \
        dciPduInfo_p->dciFormat = dlMsgInfo_p->dciFormat; \
        dciPduInfo_p->isTBOneValid =  TRUE; \
        dciPduInfo_p->isTBTwoValid =  FALSE; \
        dciPduInfo_p->nPRB = 0;\
        /*SPR 19285 changes start*/\
        dciPduInfo_p->tpc = 1;\
        /*SPR 19285 changes End*/\
        /*Cyclomatic Complexity changes - starts here */ \
        /* 4x4 DL MIMO CHG START */\
        fillNumOfLayerAndTxScheme(dciPduInfo_p,1,1,internalCellIndex);\
        /* 4x4 DL MIMO CHG END */\
        /*Cyclomatic Complexity changes - ends here */ \
    	/* SPR 10411 changes start */\
    	dciPduInfo_p->numOfRBs = resrcAlloc.allocatedRB;\
    	/* SPR 10411 changes end */\
        /* + SPR 17502 */\
        /* Assign Ue category with MIN value as this will be ignored in case of \
         * CCCH message */\
          dciPduInfo_p->ueCategory = MAC_MIN_UE_CATEGORY; \
          /* - SPR 17502*/\
}
#endif
/*  SPR  3444 changes end */

#define POPULATE_ENCODER_NODE_INFO(encoderNodeInfo ,scheduledTick , \
        dlStrategyTxNode_p, dlMsgInfo_p) \
{ \
    /*Populating the encoderNodeInfo structure */ \
    encoderNodeInfo.harqProcessId = dlStrategyTxNode_p->harqProcessId; \
    encoderNodeInfo.cceIndex = dlMsgInfo_p->cceIndex; \
    encoderNodeInfo.scheduledTick = scheduledTick; \
}
#ifdef FDD_CONFIG
/* + SPR 11208 Changes */
#define POPULATE_MUX_NODE_INFO(scheduledTick , \
        dlStrategyTxNode_p, dlMsgInfo_p, muxQueueNode_p , dlMsgTypeMsgInfo_p, \
        ueContext_p, dciCCEInfo_p, cellIndex) \
{ \
    /*Populating the muxNodeInfo structure */ \
    muxQueueNode_p->harqProcessId = (dlStrategyTxNode_p->harqProcessId); \
    muxQueueNode_p->msgType = dlMsgTypeMsgInfo_p->msgType; \
    muxQueueNode_p->sumLCPriority = dlStrategyTxNode_p->sumLCPriority; \
    muxQueueNode_p->sumWeightedQueueLoad = dlStrategyTxNode_p-> \
    weightedQueueLoad;    \
    muxQueueNode_p->ueIndex = (ueContext_p->ueIndex); \
    muxQueueNode_p->cceIndex = (dciCCEInfo_p->cceIndex) ; \
    muxQueueNode_p->internalCellIndex = cellIndex;\
}
/* - SPR 11208 Changes */
#elif TDD_CONFIG

#define POPULATE_MUX_NODE_INFO(scheduledTick , \
        dlStrategyTxNode_p, dlMsgInfo_p, muxQueueNode_p , dlMsgTypeMsgInfo_p, \
        ueContext_p, dciCCEInfo_p, cellIndex, dlSubframeNum, phyDelay, sysFrame) \
{ \
    /*Populating the muxNodeInfo structure */ \
    muxQueueNode_p->harqProcessId = (dlStrategyTxNode_p->harqProcessId); \
    muxQueueNode_p->msgType = dlMsgTypeMsgInfo_p->msgType; \
    muxQueueNode_p->sumLCPriority = dlStrategyTxNode_p->sumLCPriority; \
    muxQueueNode_p->sumWeightedQueueLoad = dlStrategyTxNode_p-> \
    weightedQueueLoad;    \
    muxQueueNode_p->ueIndex = (ueContext_p->ueIndex); \
    muxQueueNode_p->cceIndex = (dciCCEInfo_p->cceIndex) ; \
    muxQueueNode_p->internalCellIndex = cellIndex;\
    muxQueueNode_p->dlSubframeNum = dlSubframeNum ; \
    muxQueueNode_p->phyDelay = phyDelay ; \
    muxQueueNode_p->sysFrame = sysFrame ; \
}

#endif 
    /* +- SPR 17777 */
#define POPULATE_DCI_CCE_STRUCT_SPATIAL_MUL_DATA_MSG(messageType, \
        dciCCEInfo_p,resrcAlloc_p,dlMsgInfo_p, ueContext_p, harqProcess_p,\
        scheduledTick,internalCellIndex) \
{ \
    CellConfigParams *cellParams_p = \
    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p; \
    DLHARQTBInfo  *dlHARQTBInfo_p = PNULL; \
    TBInfo *tbInfo_p = PNULL; \
    DCIPduInfo *dciPduInfo_p = PNULL; \
    DLMIMOInfo   *dlMIMOInfo_p = PNULL; \
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};\
    dciPduInfo_p = &dciCCEInfo_p->dciPduInfo; \
    /* SPR_11758_Fix Start */ \
    if(internalCellIndex == ueContext_p->internalCellIndex) \
    {\
	    dlMIMOInfo_p = &ueContext_p->dlMIMOInfo; \
	    \
	    /* TPC Commands need to be sent only from PCell*/\
	    powerCorrectionAndTpc = \
	    getTPCForPUCCH(ueContext_p, scheduledTick, dlMsgInfo_p->dciFormat, internalCellIndex); \
	    /* SPR 3529 START */ \
	    /*dl power control changes start*/\
	    dciPduInfo_p->pA = ueContext_p->pA;\
	    /*dl power control changes end*/\
	    /* SPR 3529 END */ \
        dciPduInfo_p->tpc = powerCorrectionAndTpc.tpcCommand;\
                                                       \
    }\
    else\
    {\
	    dlMIMOInfo_p = &ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo; \
	    \
	    /* Send "TPC = No change" from SCell(s) in DCI 1x/2x */\
	    powerCorrectionAndTpc = getAccumulatedPowerForPucch(POWER_VAL_0, dciPduInfo_p->dciFormat);\
	    /* SPR 3529 START */ \
	    /*dl power control changes start*/\
        dciPduInfo_p->pA = ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pA;\
	    /*dl power control changes end*/\
	    /* SPR 3529 END */ \
        /** SPR 12457 Fix : Start**/\
        /** SPR 13831 Fix : Start**/\
        /*ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.tpc = powerCorrectionAndTpc.tpcCommand;\ */\
        ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.tpc = POWER_VAL_0;\
        /** SPR 13831 Fix : End **/\
        /** SPR 12457 Fix : End **/\
        dciPduInfo_p->tpc = POWER_VAL_0;\
    }\
    /* SPR_11758_Fix End */ \
    dciPduInfo_p->numTB = 0; \
    dciCCEInfo_p->msgType  = messageType; \
    dciPduInfo_p->resrcAssgmntBitMap = (resrcAlloc_p)->resourceAssignmentBitMap; \
    dciPduInfo_p->resrcAllocType =  (resrcAlloc_p)->ratPolicy; \
    dciPduInfo_p->transmissionMode = dlMIMOInfo_p->transmissionMode; \
    /* SPR_11758_Fix End */ \
    /*SPR 1830 changes start*/\
    dciPduInfo_p->nPRB = 0;\
    /*SPR 1830 changes end*/\
    /*SPR 1848 changes start*/\
    /*SPR 1848 changes end*/\
    if ( RA_POLICY_2_DISTRIBUTED == (resrcAlloc_p)->ratPolicy ) \
    { \
        dciCCEInfo_p->dciPduInfo.virtulResrcBlkType = 1; \
    } \
    else \
    { \
        dciCCEInfo_p->dciPduInfo.virtulResrcBlkType = 0; \
    } \
    dciPduInfo_p->harqProcessId =  harqProcess_p->harqProcessId; \
    if (harqProcess_p->isTBOneValid ) \
    { \
        dlHARQTBInfo_p = &harqProcess_p->dlHARQTBOneInfo; \
        tbInfo_p = &dciPduInfo_p->tb1Info; \
        tbInfo_p->modulationType = dlHARQTBInfo_p->mcsIndex; \
        tbInfo_p->txIndicator = dlHARQTBInfo_p->txIndicator; \
        dciPduInfo_p->numTB++; \
        /* SPR 941 */ \
        dciPduInfo_p->isTBOneValid =  TRUE; \
    } \
    /* SPR 430 changes start */ \
    else \
    { \
        dciPduInfo_p->isTBOneValid =  FALSE; \
    } \
    /* SPR 430 changes end */ \
    if (harqProcess_p->isTBTwoValid ) \
    { \
        dlHARQTBInfo_p = &harqProcess_p->dlHARQTBTwoInfo; \
        tbInfo_p = &dciPduInfo_p->tb2Info; \
        tbInfo_p->modulationType = dlHARQTBInfo_p->mcsIndex; \
        tbInfo_p->txIndicator = dlHARQTBInfo_p->txIndicator; \
        dciPduInfo_p->numTB++; \
        /* SPR 941 */ \
        dciPduInfo_p->isTBTwoValid =  TRUE; \
    } \
    /* SPR 430 changes start */ \
    else \
    { \
        dciPduInfo_p->isTBTwoValid =  FALSE; \
    } \
    /* SPR 430 changes end */ \
    dciPduInfo_p->dciFormat = dlMsgInfo_p->dciFormat; \
    /* + CQI_5.0 */\
    /*Cyclomatic Complexity changes - starts here */ \
    /* 4x4 DL MIMO CHG START */\
    /* SPR_11758_Fix Start */ \
    /** SPR 14502 changes start **/ \
    fillNumOfLayerAndTxScheme(dciPduInfo_p,dlMIMOInfo_p->dlRi,harqProcess_p->numOfLayer,internalCellIndex);\
    /** SPR 14502 changes end **/ \
    /* SPR_11758_Fix End */ \
    /* 4x4 DL MIMO CHG END */\
    /*Cyclomatic Complexity changes - ends here */ \
    /*4x2 MIMO S*/\
    /* 4x4 DL MIMO CHG START */\
    fillPMIAndCodebookIndex(dciPduInfo_p,dlMIMOInfo_p,\
                            harqProcess_p->numOfLayer,resrcAlloc_p,internalCellIndex);\
    /* 4x4 DL MIMO CHG END */\
    /* - CQI_5.0 */\
    dciPduInfo_p->rnti = ueContext_p->crnti; \
    dciPduInfo_p->ueIndex = ueContext_p->ueIndex; \
    dciPduInfo_p->numAntennaPort  = cellParams_p->numOfTxAnteenas; \
    /* + SPR_14339 */\
    /* - SPR_14339 */\
    /* + TM7_8 Changes Start */\
    if((TX_MODE_7 == dciPduInfo_p->transmissionMode && dlMsgInfo_p->dciFormat == DCI_FORMAT_1)\
        || (TX_MODE_8 == dciPduInfo_p->transmissionMode && dlMsgInfo_p->dciFormat == DCI_FORMAT_2B))\
    {\
        POPULATE_BEAMFORMING_VECTOR_VALUES(ueContext_p, \
                                         dciPduInfo_p);\
    }\
    if((TX_MODE_7 == dciPduInfo_p->transmissionMode) &&\
       (MCS_INDEX_QPSK_9 >= dciPduInfo_p->tb1Info.modulationType))\
    {\
        dciPduInfo_p->pdschEpreToUeRsRatio = ueContext_p->pdschEpreToUeRsRatio;\
    }\
    /* - TM7_8 Changes End */\
    /* SPR 10411 changes start */\
    dciPduInfo_p->numOfRBs = (resrcAlloc_p)->allocatedRB;\
    /* SPR 10411 changes end */\
}  


#define POPULATE_DCI_CCE_STRUCT_NON_SPATIAL_MUL_DATA_MSG(messageType, \
        dciCCEInfo_p,resrcAlloc_p,dlMsgInfo_p, ueContext_p, harqProcess_p, pRi,\
        scheduledTick, internalCellIndex) \
{ \
    CellConfigParams *cellParams_p = \
    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p; \
    DLHARQTBInfo  *dlHARQTBInfo_p = PNULL; \
    TBInfo *tbInfo_p = PNULL; \
    DCIPduInfo *dciPduInfo_p = PNULL; \
    DLMIMOInfo   *dlMIMOInfo_p = PNULL; \
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};\
    dciPduInfo_p = &dciCCEInfo_p->dciPduInfo; \
    /* SPR_11758_Fix Start */ \
    /* CA Changes Start */ \
    if(internalCellIndex == ueContext_p->internalCellIndex) \
    {\
	    dlMIMOInfo_p = &ueContext_p->dlMIMOInfo; \
                                                     \
	    powerCorrectionAndTpc = \
            getTPCForPUCCH(ueContext_p, scheduledTick, dlMsgInfo_p->dciFormat, internalCellIndex); \
                                                                              \
	    /* SPR 3529 END */ \
	    /* SPR 3529 START */ \
	    dciPduInfo_p->pA = ueContext_p->pA;\
	    /*dl power control changes end*/\
        dciPduInfo_p->tpc = powerCorrectionAndTpc.tpcCommand;\
    }\
    else\
    {\
	    dlMIMOInfo_p = &ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo; \
	    /* Send "TPC = No change" from SCell(s) in DCI 1x/2x */\
	    powerCorrectionAndTpc = getAccumulatedPowerForPucch(POWER_VAL_0, dciPduInfo_p->dciFormat);\
                                                                                        \
	    /*dl power control changes start*/\
	    dciPduInfo_p->pA = ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pA; \
	    /*dl power control changes end*/\
	    /* SPR 3529 END */ \
        /** SPR 12457 Fix : Start**/\
        /** SPR 13831 Fix : Start**/\
        /*ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.tpc = powerCorrectionAndTpc.tpcCommand;*/\
        ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.tpc = POWER_VAL_0;\
        /** SPR 13831 Fix : End **/\
        /** SPR 12457 Fix : End **/\
        dciPduInfo_p->tpc = POWER_VAL_0;\
    }\
    /* CA Changes End */ \
    dciPduInfo_p->numTB = 0; \
    dciCCEInfo_p->msgType  = messageType; \
    dciPduInfo_p->resrcAssgmntBitMap = (resrcAlloc_p)->resourceAssignmentBitMap; \
    dciPduInfo_p->resrcAllocType =  (resrcAlloc_p)->ratPolicy; \
    dciPduInfo_p->transmissionMode = dlMIMOInfo_p->transmissionMode; \
    /* SPR_11758_Fix End */ \
    dciPduInfo_p->harqProcessId =  harqProcess_p->harqProcessId; \
    /* SPR 941 */ \
    dciPduInfo_p->isTBTwoValid =  FALSE; \
    /*SPR 1830 changes start*/\
    dciPduInfo_p->nPRB = 0;\
    /*SPR 1830 changes end*/\
    /*SPR 1848 changes start*/\
    /*SPR 1848 changes end*/\
    if ( RA_POLICY_2_DISTRIBUTED == (resrcAlloc_p)->ratPolicy ) \
    { \
        dciCCEInfo_p->dciPduInfo.virtulResrcBlkType = 1; \
    } \
    else \
    { \
        dciCCEInfo_p->dciPduInfo.virtulResrcBlkType = 0; \
    } \
    if (harqProcess_p->isTBOneValid ) \
    { \
        dlHARQTBInfo_p = &harqProcess_p->dlHARQTBOneInfo; \
        tbInfo_p = &dciPduInfo_p->tb1Info; \
        tbInfo_p->modulationType = dlHARQTBInfo_p->mcsIndex; \
        tbInfo_p->txIndicator = dlHARQTBInfo_p->txIndicator; \
        dciPduInfo_p->numTB++; \
        /* SPR 941 */ \
        dciPduInfo_p->isTBOneValid =  TRUE; \
    } \
    dciPduInfo_p->dciFormat = dlMsgInfo_p->dciFormat; \
    /* + CQI_5.0 */\
    /*Cyclomatic Complexity changes - starts here */ \
    /* 4x4 DL MIMO CHG START */\
    /* SPR_11758_Fix Start */ \
    fillNumOfLayerAndTxScheme(dciPduInfo_p,dlMIMOInfo_p->dlRi,1,internalCellIndex);\
    /* SPR_11758_Fix End */ \
    /* 4x4 DL MIMO CHG END */\
    /*Cyclomatic Complexity changes - ends here */ \
    /* - CQI_5.0 */\
    dciPduInfo_p->codeBookIndex = dlMIMOInfo_p->precodingIndex; \
    if ((SPS_ACTIVATION == dciCCEInfo_p->msgType) || \
        (SPS_OCCASION == dciCCEInfo_p->msgType) || \
        (SPS_DEACTIVATION == dciCCEInfo_p->msgType)||\
        (SPS_RETX == dciCCEInfo_p->msgType)) \
    { \
        dciPduInfo_p->rnti = ueContext_p->spsDlInfo.SpsCrnti; \
        /* + TM7_8 Changes Start */\
        if((TX_MODE_7 == dciPduInfo_p->transmissionMode) && \
            (NUM_OF_TX_ANTENNAS_1 < cellParams_p->numOfTxAnteenas) && \
            (BF_ALGO_NOT_AVAILABLE != \
               (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->bfVectorPresentFlag))\
        {\
          dciPduInfo_p->transmissionScheme =  FAPI_SINGLE_ANTENNA_PORT_5; \
          /*SPR 7687 Fix */\
          dciPduInfo_p->numLayer = 1;\
          /*SPR 7687 Fix */\
        }\
        else if((TX_MODE_8 == dciPduInfo_p->transmissionMode) && \
                (NUM_OF_TX_ANTENNAS_1 < cellParams_p->numOfTxAnteenas) && \
                (BF_ALGO_NOT_AVAILABLE != \
                  (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->bfVectorPresentFlag))\
        {\
          dciPduInfo_p->transmissionScheme =  FAPI_SINGLE_ANTENNA_PORT_7; \
          /*SPR 7687 Fix */\
          dciPduInfo_p->numLayer = 1;\
          /*SPR 7687 Fix */\
        }\
        /* - TM7_8 Changes End */\
    } \
    else \
    { \
        dciPduInfo_p->rnti = ueContext_p->crnti; \
    } \
    dciPduInfo_p->ueIndex =ueContext_p->ueIndex; \
    dciPduInfo_p->numAntennaPort  = cellParams_p->numOfTxAnteenas; \
    /* SPR 16261 Fix Start */\
    /* 
       As per the spec 36213 sec 9.2 :
       For the case that the DCI format indicates a semi-persistent downlink scheduling activation,
       the TPC command for PUCCH field shall be used as an index to one of the four PUCCH resource indices
       configured by higher layers, with the mapping defined in Table 9.2-2
       */\
    if (SPS_ACTIVATION == dciCCEInfo_p->msgType)\
    {\
        /* Assigning the last received index value 
         * Also N1PucchAnPersistentList is a mandatory field for 
         * SPS_DL_SETUP we always use index 0 for sps */\
         /* SPR 22396 Fix */\
        dciPduInfo_p->tpc = 0;\
    }\
    /* SPR 20431 fix start \
    else \
    {\
        powerCorrectionAndTpc = \
            getTPCForPUCCH(ueContext_p, scheduledTick ,dciPduInfo_p->dciFormat,internalCellIndex); \
            dciPduInfo_p->tpc = powerCorrectionAndTpc.tpcCommand;\
    }\
    SPR 20431 fix end */ \
    /* SPR 16261 Fix End */\
    /* +DYNAMIC_ICIC */\
    /* COVERITY CID 61069 Fix start */\
    if (harqProcess_p->isTBOneValid && dlHARQTBInfo_p->mcsIndex <= MCS_INDEX_QPSK_9)\
    {\
        dciPduInfo_p->pA = (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
            paQpsk;\
    }\
    /* COVERITY CID 61069 Fix end */\
    /* -DYNAMIC_ICIC */\
    /* + TM7_8 Changes Start */\
    if((TX_MODE_7 == dciPduInfo_p->transmissionMode) &&\
       (MCS_INDEX_QPSK_9 >= dciPduInfo_p->tb1Info.modulationType))\
    {\
        dciPduInfo_p->pdschEpreToUeRsRatio = ueContext_p->pdschEpreToUeRsRatio;\
    }\
    /* - TM7_8 Changes End */\
    /* SPR 10411 changes start */\
    dciPduInfo_p->numOfRBs = (resrcAlloc_p)->allocatedRB;\
    /* SPR 10411 changes end */\
}  

/* SPR 10306 fix start */ 
#define FILL_TPMI_AND_PMI_CONF_VALUE(dciPduInfo_p,dlMIMOInfo_p,resrcAlloc_p,internalCellIndex)\
{\
    dciPduInfo_p->codeBookIndex = (resrcAlloc_p)->pmiValue; \
    /* SPR 10306 fix end */ \
    CellConfigParams *cellParams_p = \
    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p; \
    if((((5 == dciPduInfo_p->codeBookIndex)||(6 == dciPduInfo_p->codeBookIndex))\
        && (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2))\
        ||((PMI_INDEX_16 == dciPduInfo_p->codeBookIndex) &&\
          (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4))) \
    {\
        dciPduInfo_p->pmiConfirmation = PMI_CONF_ONE;\
        dciPduInfo_p->transmittedPMI = 255;\
    }\
    else \
    {\
        dciPduInfo_p->pmiConfirmation = PMI_CONF_ZERO;\
        if( (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2) ||\
            (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4) )\
        {\
            if(dlMIMOInfo_p->isAperMode12) \
            {\
                dciPduInfo_p->transmittedPMI = dciPduInfo_p->codeBookIndex - 1; \
            }\
            else\
            {\
                dciPduInfo_p->transmittedPMI = dciPduInfo_p->codeBookIndex; \
            }\
            dlMIMOInfo_p->isAperMode12 = FALSE; \
        }\
    }\
}   

/*4x2 MIMO E*/
/****************************************************************************
 * Exported Types
 ****************************************************************************/


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern LTE_SQUEUE *pendingCcchQueue_gp[MAX_NUM_CELL];

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
 extern void allocPdschForDL(
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
     ); 
/* DRX_CHG */
extern void putEntryInDRXDLSchedulerEventQueue(UInt16 ueIndex, 
                                               /* SPR 15909 fix start */
                                               tickType_t globalTick,
                                               /* SPR 15909 fix end */
                                               InternalCellIndex internalCellIndex);
/* DRX_CHG */

/* ICIC changes start */
/* SPR 5599 changes start (ZIP ID 129098) */
extern UInt32 getCount(UInt32 bitMap);
/* SPR 5599 changes end (ZIP ID 129098) */
/* ICIC changes end */
/* + TM7_8 Changes Start */
#define POPULATE_BEAMFORMING_VECTOR_VALUES(ueContext_p, \
                                         dciPduInfo_p)\
{\
    UInt8 bfCounter = 0;\
    if (AOA_BASED_BF_ALGO == ueContext_p->beamFormingAlgoToBeUsed)\
    {\
        for(bfCounter =0; bfCounter <cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->numOfTxAnteenas ; bfCounter++)\
        {\
            UInt16 AoaPerAntenna = ueContext_p->AoA[bfCounter];\
            dciPduInfo_p->bfValue[bfCounter] =  \
                (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
            /* SPR 18122 Changes Start*/\
            angleOfArrivalToBeamFormingVectorMapping[DIVIDE_BY_TWO(AoaPerAntenna)];\
            /* SPR 18122 Changes Start*/\
        }\
    }\
    else if((SINR_BASED_BF_ALGO == ueContext_p->beamFormingAlgoToBeUsed) ||\
            (SINR_BASED_BF_ALGO == \
              (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->bfVectorPresentFlag))\
    {\
        dciPduInfo_p->bfValue[NUM_OF_TX_ANTENNAS_1 -1] =  \
            (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
            sinrToBeamFormingVectorMapping[ueContext_p->averageSinrForPucch].beamFormingVectorForA1;\
        dciPduInfo_p->bfValue[NUM_OF_TX_ANTENNAS_2 -1] =  \
            (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
            sinrToBeamFormingVectorMapping[ueContext_p->averageSinrForPucch].beamFormingVectorForA1;\
        dciPduInfo_p->bfValue[NUM_OF_TX_ANTENNAS_3 -1] =  \
            (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
            sinrToBeamFormingVectorMapping[ueContext_p->averageSinrForPucch].beamFormingVectorForA1;\
        dciPduInfo_p->bfValue[NUM_OF_TX_ANTENNAS_4 -1] =  \
            (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
            sinrToBeamFormingVectorMapping[ueContext_p->averageSinrForPucch].beamFormingVectorForA1;\
    }\
}\
/* - TM7_8 Changes End */

/* Cyclomatic Complexity Changes */
/*4x4 DL MIMO CHG START*/
 void fillNumOfLayerAndTxScheme(DCIPduInfo *dciPduInfo_p,UInt8 pRi,UInt8 numOfLayer,
        InternalCellIndex internalCellIndex);
 void fillPMIAndCodebookIndexForDci2Anteena2(DCIPduInfo              *dciPduInfo_p,
                                                   ResourceAllocatorOutput *resrcAlloc_p);

 void fillPMIAndCodebookIndexForDci2Anteena4(DCIPduInfo              *dciPduInfo_p,
                                                   UInt8                   numOfLayer,
                                                   ResourceAllocatorOutput *resrcAlloc_p);

 void fillPMIAndCodebookIndexForDci2A(DCIPduInfo              *dciPduInfo_p,
                                            DLMIMOInfo              *dlMIMOInfo_p,
                                            UInt8                   numOfLayer,
                                            /* +- SPR 17777 */
                                            InternalCellIndex internalCellIndex);
 void fillPMIAndCodebookIndex(DCIPduInfo              *dciPduInfo_p,
                                    DLMIMOInfo              *dlMIMOInfo_p,
                                    UInt8                    numOfLayer,
                                    ResourceAllocatorOutput *resrcAlloc_p,
                                    InternalCellIndex internalCellIndex);

/* + SPR 22895 */
/* SPR 21631 Start */
 void updateRaPolicyInfo(tickType_t nextTick,InternalCellIndex internalCellIndex);
/* SPR 21631 End */
/* - SPR 22895 */

/*4x4 DL MIMO CHG END*/


#endif  

