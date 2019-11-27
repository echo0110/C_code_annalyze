/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPDCCHMgr.h,v 1.1.1.1.6.1.4.2.2.1.14.1 2010/11/08 06:06:17 gur10121 Exp $
 *
 ****************************************************************************
 *
 *  File Description : It contains macro declarations, function declarations 
 *                     sturucture declarations required by lteMacPDCCHMgr.c
 *             
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacPDCCHMgr.h,v $
 * Revision 1.1.1.1.6.1.4.2.2.1.14.1  2010/11/08 06:06:17  gur10121
 * Semi static changes merging
 *
 * Revision 1.1.1.1.6.1.4.2.2.1.8.1  2010/10/25 08:27:32  gur10121
 * Merging the Semi static changes on the latest
 *
 * Revision 1.1.1.1.6.1.4.2.2.1.6.2  2010/10/15 18:53:12  gur10121
 * Semi staic Changesdone and checked in
 *
 * Revision 1.1.1.1.6.1.4.2.2.1.6.1  2010/10/14 08:23:28  gur10121
 * Semi Static header file changes
 *
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:45:57  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.50  2009/09/16 06:02:23  gur18550
 * Windiows TAB removed
 *
 * Revision 1.49  2009/09/16 06:01:04  gur18550
 * PDCCH Updated
 *
 * Revision 1.48  2009/08/26 12:08:25  gur12140
 * Optimized for DCI CCE Array
 *
 * Revision 1.46  2009/08/04 10:58:40  gur12140
 * Added numPushedInAdvanceQueue member in DLStrategyTxNodeT Struct
 *
 * Revision 1.45  2009/07/26 08:11:28  gur15697
 * compilation warnings removed
 *
 * Revision 1.44  2009/07/20 05:16:06  gur12140
 * defined  PDCCH_MGR
 *
 * Revision 1.43  2009/07/17 09:44:49  gur18550
 * Log Updated
 *
 * Revision 1.42  2009/07/17 09:38:11  gur18550
 * Log Updated
 *
 * Revision 1.41  2009/07/14 05:00:26  gur12140
 * Incorporated Comments
 *
 * Revision 1.40  2009/07/10 06:24:41  gur18550
 * DELAY THREE UPDATED
 *
 * Revision 1.39  2009/07/07 13:15:57  gur18550
 * DELAY_THREE Added
 *
 * Revision 1.38  2009/07/04 07:43:15  gur15697
 * review comments incorporated
 *
 * Revision 1.37  2009/07/02 08:13:49  gur12140
 * Incorporated Comments
 *
 * Revision 1.35  2009/06/29 13:44:00  gur12140
PDCCH * Incorporated Comments
 *
 * Revision 1.34  2009/06/18 07:53:47  gur12140
 * Removed Bug
 *
 * Revision 1.33  2009/06/16 15:24:13  gur12140
 * Removed Compilation Errors
 *
 * Revision 1.32  2009/06/16 14:48:35  gur12140
 * Incorporated Comments
 *
 * Revision 1.31  2009/06/16 14:16:08  gur12140
 * Incorporated Comments
 *
 * Revision 1.29  2009/06/12 05:04:43  gur12140
 * Incorporated Comments
 *
 * Revision 1.28  2009/06/10 08:43:30  gur18550
 * Compilation Error Removed
 *
 * Revision 1.27  2009/06/10 05:01:27  gur12140
 * Removed Linking Errors
 *
 * Revision 1.26  2009/06/09 14:16:38  gur18550
 * UL NACK ADDED
 *
 * Revision 1.25  2009/06/05 16:37:34  gur20052
 * updated the strategy input function protyping
 *
 * Revision 1.24  2009/06/05 07:56:08  gur20052
 * added sumLCPriority & weightedQueueLoad in DLStrategyTxNode
 *
 * Revision 1.23  2009/06/04 14:38:43  gur12140
 * Comments Incorporated
 *
 * Revision 1.22  2009/05/28 07:53:13  gur12140
 * Modified DLUEContext.h header include
 *
 * Revision 1.21  2009/05/27 08:43:26  gur15697
 * RACH functionality added
 *
 * Revision 1.20  2009/05/25 16:24:33  gur20052
 * Updated the ULStrategyTxNode, added tti timer and node msg type
 *
 * Revision 1.19  2009/05/22 06:23:16  gur15697
 * extern  CommonSearchSpaceInfo commonSearchSpaceInfo_g added
 *
 * Revision 1.18  2009/05/21 08:27:59  gur12140
 * Incorporated HARQ Structure Changes
 *
 * Revision 1.15  2009/05/18 08:30:12  gur15697
 * allocatePdcchForBCCH() updated
 *
 * Revision 1.14  2009/05/18 06:47:01  gur15697
 * extern for dciFormatToAggregationLevel_g[] added
 *
 * Revision 1.13  2009/05/12 06:49:47  gur15697
 * CCE allocation for BCCH added
 *
 * Revision 1.12  2009/05/11 11:10:25  gur12140
 * Changed Funtions Prototype
 *
 * Revision 1.11  2009/05/10 07:54:26  gur12140
 * Changed Structure of DCI CCE Info as discussed in Team
 *
 * Revision 1.9  2009/05/08 15:13:42  gur12140
 * Compilation Error Removed
 *
 * Revision 1.8  2009/05/08 11:10:47  gur12140
 * Make it compiled
 *
 * Revision 1.1  2009/04/30 05:50:59  gur12140
 * Initial vertion
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_PDCCH_H
#define LTE_MAC_PDCCH_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacDLResourceAllocator.h"
#include "lteMacDLUEContext.h"
#include "lteMacULUEContext.h"
#include "lteMacCQI.h"
#include "lteMacMeasGapHandler.h" /* MEAS_GAP_CHG */
#include "lteMacPorting.h"
#include "lteCircQueue.h"
#include "lteMacExecutionLegs.h"
#include "lteMacUtilityAndSharedStructs.h"
#ifdef LTE_EMBMS_SUPPORTED
#include "lteMacEMbmsSchedulerStrategy.h"
#endif
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

 /* SPR 3100 START*/
#define GET_DL_CQI_VALUE(cqiValue, dlUEContext_p) \
{\
    if (((TX_MODE_3 == dlUEContext_p->dlMIMOInfo.transmissionMode) ||\
                (TX_MODE_4 == dlUEContext_p->dlMIMOInfo.transmissionMode) ||\
                (TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode)) && \
            (dlUEContext_p->dlCQIInfo.widebandCQICodeWordOne >= \
             dlUEContext_p->dlCQIInfo.widebandCQICodeWordTwo))  \
    { \
        cqiValue = dlUEContext_p->dlCQIInfo.widebandCQICodeWordTwo; \
    } \
    else \
    { \
        cqiValue = dlUEContext_p->dlCQIInfo.widebandCQICodeWordOne; \
    } \
}

 /* SPR 3100 END*/
#define PDCCH_MGR  "pdcch_mgr" 

#define FDD_HARQ_OFFSET 4
/*SPR_3305_FIX_START*/
#define FDD_HARQ_OFFSET_FOR_MSG3 4
/*SPR_3305_FIX_END*/

#define AGGREGATION_LEVEL_ONE 1
#define AGGREGATION_LEVEL_TWO 2
#define AGGREGATION_LEVEL_FOUR 4
#define AGGREGATION_LEVEL_EIGHT 8
#define INVALID_AGGREGATION_LEVEL 10
#ifndef MAX_NUM_RBS
#define MAX_NUM_RBS                 100
#endif
/* ICIC changes start */
/* This gives offset in aggregation level of cell edge user with respect to 
 * cell centre user.
 * 1 - next aggregation level, 2 - next to next aggregation level and so on.
 * min value = 1, max value = 3
 */
#define CELL_EDGE_AGGR_LEVEL_OFFSET 1
/* ICIC changes end */

#ifdef TDD_CONFIG
#define CFI_VALUE_SPECIAL_SUBFRAME 2
#endif

#define NUM_RESOURCE_ELEMENT_FOR_ONE_RB 12
#define NUM_RESOURCE_ELEMENT_FOR_ONE_PHICH_GROUP  12
#define NUM_RESOURCE_ELEMENT_FOR_PCFICH 16

#define NUM_RESOURCE_ELEMENT_FOR_ONE_ANTENNA_PORT_REF_SIGNAL 2
#define NUM_RESOURCE_ELEMENT_FOR_PORT_REF_SIGNAL        2
#define NUM_RESOURCE_ELEMENT_FOR_TWO_ANTENNA_PORT_REF_SIGNAL 4
/* 4x2 MIMO S */
#define NUM_RESOURCE_ELEMENT_FOR_FOUR_ANTENNA_PORT_REF_SIGNAL 8
/* 4x2 MIMO E */
#define NUM_RESOURCE_ELEMENT_IN_ONE_CCE 36

#define NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_ONE_UE_SPACE 6
#define NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_TWO_UE_SPACE 6
#define NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_FOUR_UE_SPACE 2
#define NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_EIGHT_UE_SPACE 2

#define NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_FOUR_COMMON_SPACE 4
#define NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_EIGHT_COMMON_SPACE 2

#define CODING_RATE                          1
#define MAX_RE_IN_CCE                       36
#define RESOURCE_ALLOCATION_HEADER           1

#ifdef TDD_CONFIG
    #define HARQ_PROCESS_NUMBER                  4
#elif FDD_CONFIG   
    #define HARQ_PROCESS_NUMBER                  3
#endif    
#define TB_TO_CODEWORD_SWAP                  1
#define MODULATION_AND_CODING                5
#define NEW_DATA_INDICATOR                   1
#define REDUNDANCY_VERSION                   2
#define PRECODING_INFO                       3
#define TMPI_INFO_PRECODING                  2
#define PMI_CONF_PRECODING                   1
#define VRB_ASSIGN_FLAG                      1
#define FLAG_FORMAT_DIFF                     1
#define HOPPING_FLAG                         1
#define TPC_COMMAND                          2
#define CQI_REQUEST                          1
#define GAP_VALUE                            1
#define TB_SIZE_INDEX                        5
#define TPC_PUCCH                   2
#define CYCLIC_SHIFT_DMRS 3

#ifdef TDD_CONFIG
#define MAX_TDD_UL_PDCCH_CONTAINER           2
#define PRECODING_INFO_2A                    0
#define DAI                                  2
#endif
/* + TM7_8 Changes Start */
#define SCRAMBLING_IDENTITY                  1
/* - TM7_8 Changes Start */
#ifdef TDD_CONFIG
#define CONSTANT_BYTES_OF_DCIFORMAT_2A     RESOURCE_ALLOCATION_HEADER + \
                                           TPC_PUCCH + \
                                           HARQ_PROCESS_NUMBER + \
                                           TB_TO_CODEWORD_SWAP + \
                                           MODULATION_AND_CODING + \
                                           NEW_DATA_INDICATOR + \
                                           REDUNDANCY_VERSION + \
                                           MODULATION_AND_CODING + \
                                           NEW_DATA_INDICATOR + \
                                           REDUNDANCY_VERSION +\
                                           PRECODING_INFO_2A +\
                                           DAI

#define CONSTANT_BYTES_OF_DCIFORMAT_2      RESOURCE_ALLOCATION_HEADER + \
                                           TPC_PUCCH + \
                                           HARQ_PROCESS_NUMBER + \
                                           TB_TO_CODEWORD_SWAP + \
                                           MODULATION_AND_CODING + \
                                           NEW_DATA_INDICATOR + \
                                           REDUNDANCY_VERSION + \
                                           MODULATION_AND_CODING + \
                                           NEW_DATA_INDICATOR + \
                                           REDUNDANCY_VERSION + \
                                           PRECODING_INFO +\
                                           DAI

#define CONSTANT_BYTES_OF_DCIFORMAT_1B     VRB_ASSIGN_FLAG + \
                                           MODULATION_AND_CODING + \
                                           HARQ_PROCESS_NUMBER + \
                                           NEW_DATA_INDICATOR + \
                                           REDUNDANCY_VERSION + \
                                           TMPI_INFO_PRECODING + \
                                           TPC_PUCCH + \
                                           PMI_CONF_PRECODING


#define CONSTANT_BYTES_OF_DCIFORMAT_1A     FLAG_FORMAT_DIFF + \
                                           VRB_ASSIGN_FLAG + \
                                           MODULATION_AND_CODING + \
                                           HARQ_PROCESS_NUMBER + \
                                           NEW_DATA_INDICATOR + \
                                           REDUNDANCY_VERSION + \
                                           TPC_PUCCH +\
                                           DAI


#define CONSTANT_BYTES_OF_DCIFORMAT_1      RESOURCE_ALLOCATION_HEADER + \
                                           MODULATION_AND_CODING + \
                                           HARQ_PROCESS_NUMBER + \
                                           NEW_DATA_INDICATOR + \
                                           REDUNDANCY_VERSION + \
                                           TPC_PUCCH +\
                                           DAI


#define CONSTANT_BYTES_OF_DCIFORMAT_0      FLAG_FORMAT_DIFF + \
                                           HOPPING_FLAG + \
                                           MODULATION_AND_CODING + \
                                           NEW_DATA_INDICATOR + \
                                           TPC_PUCCH + \
                                           CYCLIC_SHIFT_DMRS + \
                                           DAI +\
                                           CQI_REQUEST


/* + TM7_8 Changes Start */
#define CONSTANT_BYTES_OF_DCIFORMAT_2B    RESOURCE_ALLOCATION_HEADER +\
                                          TPC_PUCCH + \
                                          HARQ_PROCESS_NUMBER +\
                                          SCRAMBLING_IDENTITY +\
                                          MODULATION_AND_CODING +\
                                          NEW_DATA_INDICATOR +\
                                          REDUNDANCY_VERSION +\
                                          MODULATION_AND_CODING +\
                                          NEW_DATA_INDICATOR +\
                                          REDUNDANCY_VERSION+\
                                          DAI
/* - TM7_8 Changes End */



                                          
#elif FDD_CONFIG
#define CONSTANT_BYTES_OF_DCIFORMAT_2A     RESOURCE_ALLOCATION_HEADER + \
    TPC_PUCCH + \
    HARQ_PROCESS_NUMBER + \
TB_TO_CODEWORD_SWAP + \
MODULATION_AND_CODING + \
NEW_DATA_INDICATOR + \
REDUNDANCY_VERSION + \
MODULATION_AND_CODING + \
NEW_DATA_INDICATOR + \
REDUNDANCY_VERSION 

#define CONSTANT_BYTES_OF_DCIFORMAT_2      RESOURCE_ALLOCATION_HEADER + \
        TPC_PUCCH + \
    HARQ_PROCESS_NUMBER + \
TB_TO_CODEWORD_SWAP + \
MODULATION_AND_CODING + \
NEW_DATA_INDICATOR + \
REDUNDANCY_VERSION + \
MODULATION_AND_CODING + \
NEW_DATA_INDICATOR + \
REDUNDANCY_VERSION + \
PRECODING_INFO 

#define CONSTANT_BYTES_OF_DCIFORMAT_1B     VRB_ASSIGN_FLAG + \
    MODULATION_AND_CODING + \
HARQ_PROCESS_NUMBER + \
NEW_DATA_INDICATOR + \
REDUNDANCY_VERSION + \
TMPI_INFO_PRECODING + \
TPC_PUCCH + \
PMI_CONF_PRECODING


#define CONSTANT_BYTES_OF_DCIFORMAT_1A     FLAG_FORMAT_DIFF + \
    VRB_ASSIGN_FLAG + \
MODULATION_AND_CODING + \
HARQ_PROCESS_NUMBER + \
NEW_DATA_INDICATOR + \
REDUNDANCY_VERSION + \
TPC_PUCCH


#define CONSTANT_BYTES_OF_DCIFORMAT_1      RESOURCE_ALLOCATION_HEADER + \
    MODULATION_AND_CODING + \
HARQ_PROCESS_NUMBER + \
NEW_DATA_INDICATOR + \
REDUNDANCY_VERSION + \
TPC_PUCCH


#define CONSTANT_BYTES_OF_DCIFORMAT_0      FLAG_FORMAT_DIFF + \
    HOPPING_FLAG + \
MODULATION_AND_CODING + \
NEW_DATA_INDICATOR + \
TPC_PUCCH + \
CYCLIC_SHIFT_DMRS + \
CQI_REQUEST
/* + TM7_8 Changes Start */
#define CONSTANT_BYTES_OF_DCIFORMAT_2B    RESOURCE_ALLOCATION_HEADER +\
                                          TPC_PUCCH + \
                                          HARQ_PROCESS_NUMBER +\
                                          SCRAMBLING_IDENTITY +\
                                          MODULATION_AND_CODING +\
                                          NEW_DATA_INDICATOR +\
                                          REDUNDANCY_VERSION +\
                                          MODULATION_AND_CODING +\
                                          NEW_DATA_INDICATOR +\
                                          REDUNDANCY_VERSION 
                                          
/* - TM7_8 Changes End */


#endif

#define DELAY_NOT_THREE 2
#define DELAY_THREE 3
#define LOG_PDCCH "PDCCH"

/* dl qos changes Start */
#define TA_VALUE_FOR_PDCCH_PDSCH_FAILURE_CASE 1
/* dl qos changes Stop */

 /* CLPC_CHG */
#define MAX_TPC_MSG_PER_TICK 4
/* + TM7_8 Changes Start */
/* Cov_100831_Fix_Start */
#define MAX_DCI_FORMATS  14
/* Cov_100831_Fix_End */
/* - TM7_8 Changes End */
 /* Number of bits in a DCI format is dependent on number of antenna ports
  * configured in the cell. The two variations that cause change in DCI
  * size are 2 and 4 ports.
  **/
 /* +- SPR 18481 */
#define ANTENNA_PORT_VARIATION 4
 /* +- SPR 18481 */
#define DCI_SIZE_THRESHOLD_SMALL 30
#define DCI_SIZE_THRESHOLD_MEDIUM 40
 /* SPS_CHG */
#define RESERVE_SPS_FIRST_FACTOR 2 

/* Checking if MU MIMO is to be done or not.
* As per SPS implementation, no DCI0 (grant) will be given to UE for which SPS is scheduled on a particular Tick
* and therefore will not be considered for CCEs allocation and hence UL-MU-MIMO and vice versa.
* Criteria to be checked for UL MU MIMO:
* 1.Check if pendingDeleteUEFlag is set or not.It is a defensive check.
* UE shall not be considered for scheduling in the current tick if pendingDeleteUEFlag is set
* as it is going to be deleted.
* 2.Check if UL_MU_MIMO_FLAG is set or not.This flag is configured by RRC.
* By default this flag is disabled for all UEs.
* 3.Check if AvgMCS is more than the MCS Threshold set by RRM.
* No UL MU MIMO if AvgMCS is less than threshold value.
* 4.Check if grantType is CR_UL_GRANT_TYPE or CQI_WITHOUT_DATA_UL_GRANT_TYPE.
* No UL MU MIMO for the above mentioned two grants.
* CR_UL_GRANT_TYPE: As cyclic shift is not sent with the UE having CR_ULGRANT_REQ_MSG (nDMRS value 0) so
* UE along with its buddy UE becomes restricted to one orthogonal nDMRS pair, resulting into complexity in code.
* Moreover grant is also very less, so it is not worth sharing resources from the viewpoint of code complexity. 
* CQI_WITHOUT_DATA_UL_GRANT_TYPE: As CQI report contains critical information for the channel conditions.
* Sharing such resources may lead to loss of information due to interference. Secondly grant is also very less
* so it is not worth sharing resources from the viewpoint of code complexity.
* 5.Check if measurement gap is to be ON after 8th tick from the currentTick or not.
* If it is to be ON the UE shall not be selected as UL_MU_MIMO candidate because this will result in retransmission
* and hence UL_MU_MIMO will ultimately not happen.
* If all the criteria for MU MIMO are met then add the ueIndex to
* PdcchULMUMIMOCandidateInfo[userLocation][modulationType]list and reference Index in UEContext. */

/* UL_MU_MIMO_CHG_START */


#define MAC_GET_MOD_TYPE(modScheme) \
{\
    switch(modScheme)\
    {\
        case 2: modScheme = 0;\
                break;\
        case 4: modScheme = 1;\
                break;\
        case 6: modScheme = 2;\
                break;\
        default: LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,"InValid Modulation Sceme \n");\
                break;\
    }\
}

#ifdef FDD_CONFIG
#define UL_MU_MIMO_CANDIDATE_SELECTION(ulUEContextInfo_p,ulUEContext_p, pdcchULNewUEInfo_p,\
	countULNewUE,directIndexingUEInfo_p,retVal,internalCellIndex ) \
{\
    if ((!ulUEContextInfo_p->pendingDeleteFlag) && \
	    (ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus) && \
            pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].avgMCS >=  \
                rrmMacCellGlobal_gp[internalCellIndex]->ulMUMIMORrmConfigInfo.ulMUMIMOMcsThreshold && \
	    (CR_ULGRANT_REQ_MSG != directIndexingUEInfo_p->ulGrantRequestType) && \
	    (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG != directIndexingUEInfo_p->ulGrantRequestType) && \
	    (UL_SPS_ACTIVATION != directIndexingUEInfo_p->ulGrantRequestType) && \
	    (UL_SPS_DEACTIVATION != directIndexingUEInfo_p->ulGrantRequestType) && \
	    (UL_SPS_OCCASION != directIndexingUEInfo_p->ulGrantRequestType) && \
	    (FALSE == isTTIInMeasGapPeriod(ulUEContext_p,(getCurrentTick()+ PHY_DL_DELAY + 8))) && \
            /* For  TTIB_Code */\
        (!ulUEContext_p->ttiBundlingEnabled))\
    {\
	retVal = TRUE;\
    }\
    else\
    {\
	retVal = FALSE;\
    }\
}

#elif TDD_CONFIG
/* TDD Config 0 Changes Start */
#define UL_MU_MIMO_CANDIDATE_SELECTION(ulUEContextInfo_p,ulUEContext_p, avgMCS,\
        ulGrantRequestType,ulSubframeNum,retVal,internalCellIndex ) \
{\
    if ((!ulUEContextInfo_p->pendingDeleteFlag) && \
            (ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus) && \
            (avgMCS >= rrmMacCellGlobal_gp[internalCellIndex]->ulMUMIMORrmConfigInfo.ulMUMIMOMcsThreshold) && \
            (CR_ULGRANT_REQ_MSG != ulGrantRequestType) && \
            (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG != ulGrantRequestType) && \
/* TDD Config 0 Changes End */\
/* CA TDD Changes Start */\
             (FALSE == isTTIInMeasGapPeriod(ulUEContext_p,(getCurrentTick()+ DL_ADVANCE_TICK + roundTripTime(ulSubframeNum, internalCellIndex)))))\
             /* CA TDD Changes End */\
    {\
        retVal = TRUE;\
    }\
    else\
    {\
        retVal = FALSE;\
    }\
}
#endif 

/* UL_MU_MIMO_CHG_END */
/* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
#define UL_GET_BASE_ADDRESS_FROM_STR_TDD0_S_SF_TX_NODE_ANCHOR(ulStrategyNodeVoid_p,\
        ulStrategyNode_p) \
        if (PNULL != ulStrategyNodeVoid_p)\
        {\
            ulStrategyNode_p =  (ULStrategyTxNode * ) \
                ((UInt8 *) ulStrategyNodeVoid_p - \
                offsetof(ULStrategyTxNode  , ulStrategyTdd0SsfnTxNodeAnchor));\
        }
#endif
/* TDD Config 0 Changes End */

/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct DLStrategyTxNodeT
{
    /*data type of anchor*/
    LTE_SNODE dlStrategyTxNodeAnchor;
    /*This structure stores the information needed by Resource Allocator for 
      allocating RBs to a UE */
    ResourceAllocatorInput resourceAllocInput;    
    /*This field store the message type for which the PDCCH region need to be
      allocated .For xample :  Broad Cast Message ,Paging Control Message,Data 
      Message ,Contention Resolution Message ,Data Message ,Data with TA ,
      Data with DRX etx */
    MsgType msgType;
    /*This field stores the DCI Format for which PDCCH will first try to 
      allocate the CCE to it */
    DCIFormat primaryDCIFormat;
    /*This field stores the DCI Format for which PDCCH will try to allocate 
      the CCE to it ,if CCE allocation of primaryDCIFormat failed */
    DCIFormat secondaryDCIFormat;
    /*This field stores the Transmission Type of Data like if this is New
      Transmission or Adaptive Re-Transmission or Non Adaptive Re-Transmission*/
    TransmissionType transmissionType;
    UInt32 sumLCPriority;
    UInt32 weightedQueueLoad;
    UInt16 ueIndex; 
    UInt8 harqProcessId;
    /*This field when set to 1 indicates to Uplink Statergy that CQI request 
      need to be sent to UE in the UpLink Grant Message */
    UInt8 isCQIRequestedULGrantMsg;     
    UInt8 numPushedInAdvanceQueue;
/* SPR 3430 */
    UInt8 allocPushCount;
/* SPR 3430 */

    /* DL qos strategy start */
    DLHARQProcess *harqProcess_p;
    /* if set then allocate min TB size */
    UInt32 strictAllocatedFlag;
    /* Indicates minimum TB size that need to be scheduled */
    UInt16 minTBSize;
    /* Indicates maximum TB size that can be scheduled */
    UInt16 maxTBSize;
    /* DL qos strategy stop */
    /*SPR 3679 Qos Changes Start*/
    UInt32 totalRBMIMO;
    /*SPR 3679 Qos Changes End*/



}DLStrategyTxNode;


/* Ths structure is the node for DRX Trigger queue */
typedef struct DrxTriggerNodeT
{
    LTE_SNODE        DrxTriggerNodeAnchor;
    UInt16           ueIdx;
    /* This signifies the ON/OFF trigger */
    DrxTrigger onOffTrigger;
    /* Next DRX Off Tick (T3) */
    /* SPR 15909 fix start */
    tickType_t    drxNextOffTick;
    /* SPR 15909 fix end */
}DrxTriggerNode;

/* DL UE Schedule Info Node
 * This  structure is used to store the scheduling details of the UE 
 * and these wull be used to traverse throught all the nodes and 
 * push the Strategy Tx Node into PDCCH 
 */

typedef struct DLUeScheduleInfoT
{
    /* pointer to the strategy Tx node of the UE to be pushed into PDCCH */
    DLStrategyTxNode * dLStrategyTxNode_p;
    /* Indicates the UE Index */
    DLUEContext * ueDLContext_p;
    /* Indicates whether TA is scheduled or Not */
    MsgType isTaDrxScheduled;
    /* Pointer to HARQ process that is assigned for current transmission */
    DLHARQProcess *harqProcess_p;
} DLUeScheduleInfo;

/* DL Strategy Schedule Info 
 * This  structure is used to store the  Strategy Scheduled details 
 * and the array in the structure will be traversed and the strategy 
 * and harq nodes will be updated & Strategy Tx Node will be 
 * pushed into PDCCH Queue
 */

typedef struct DLStrategyScheduleInfoT
{
    /* Indicates the number  of UE schduled in the tick */
    UInt8 schdeuledUeCount;
    /* Holds the schduled UE DL Strategy Transmssion Node and 
     * HARQ Process Node */
    DLUeScheduleInfo dlUeScheduleInfo[MAC_MAX_UE_SCHEDULED];
} DLStrategyScheduleInfo;
typedef struct ULStrategyTxNodeT
{
    /*data type of anchor*/
    LTE_SNODE ulStrategyTxNodeAnchor;
    ULGrantRequestType ulGrantRequestType;
    /* SPR 15909 fix start */
    tickType_t  ttiCounter;
    /* SPR 15909 fix end */
    UInt16 ueIndex;
    UInt8 requiredRB;
    /*This flag when set idicates the number of RB allocated for UL has to be
      equal to requiredRB */
    UInt8 strictAllocRB;
#ifdef TDD_CONFIG    
    UInt8 ulHarqProcessId;
    /* TDD Config 0 changes start */
    /* In TDD config 0, S subframe, List will be used for ul Strategy tx node
     * For other cases Queue ds will be used (ulStrategyTxNodeAnchor)  */
    LTE_LIST_NODE ulStrategyTdd0SsfnTxNodeAnchor;
    /* Stores the ulSubframe Number for which scheduling is being done */
    UInt8 ulSubFrameNum;
    /* Denotes the scheduled count of U subframe of the UE in a TTI */
    UInt8 scheduledCount;
    /* TDD Config 0 changes end */
#endif    
/* ULA_CHG */
    UInt8 avgMcs;
    UInt32 minDataSize;
    UInt32 maxDataSize;
/* ULA_CHG */
}ULStrategyTxNode;

typedef struct ULUeScheduleInfoT
{
    /*Pointer to UL ue context*/
    ULUEContext *ueULContext_p;
    /* pointer to the strategy Tx node of the UE to be pushed into PDCCH */
    ULStrategyTxNode * ulStrategyTxNode_p;
} ULUeScheduleInfo;

/* UL Strategy Schedule Info
 * This  structure is used to store the  Strategy Scheduled details
 * and the array in the structure will be traversed and the strategy
 * and harq nodes will be updated & Strategy Tx Node will be
 * pushed into PDCCH Queue
 */

typedef struct ULStrategyScheduleInfoT
{
    /* Indicates the number  of UE schduled in the tick */
    UInt8 scheduledUeCount;
    /* Holds the schduled UE UL Strategy Transmssion Node */
    ULUeScheduleInfo ulUeScheduleInfo[MAX_NUM_UE];
} ULStrategyScheduleInfo;

typedef struct DLDataMsgInfoT
{
    DLStrategyTxNode *dlStrategyTxNode_p;  
    /*This Stores the dciFormat for which PDCCH Region has been allocated */
    DCIFormat       dciFormat;
    /*This field stoes the CCE Index which has been allocated to Data Message*/
    UInt8             cceIndex;
}DLDataMsgInfo;

typedef struct DLBCCHMsgInfoT

{
    /*This is pointer to ResourceAllocatorInput structure which stores the 
      infomation needed by Resource Allocator for allocating RBs */ 
    ResourceAllocatorInput resourceAllocInput;
    /*This stores the dciFormat which will be used for Transmission of this
      Message*/    
    DCIFormat dciFormat;
    /* This field stores the redundancy version */
    RedundancyVerVal redundancyVer;
    UInt8 *data_p; /*SIBType1/SI message buffer*/
    /* SPR 13250 fix start */
    UInt16 dataLen; /*message buffer length*/
    /* SPR 13250 fix end */
    /*This field stoes the CCE Index which has been allocated to Broad
      Cast  Message */
    UInt8 cceIndex;
    /* SPR 3062 Chg */
    /* This field store tpc lsb index for the bcch msg */
    UInt8 tpc;
    /* SPR 3062 Chg Ends */
}DLBCCHMsgInfo;

typedef struct DLPCCHMsgInfoT
{
    /*This is pointer to ResourceAllocatorInput structure which stores the 
      infomation needed by Resource Allocator for allocating RBs */
    ResourceAllocatorInput resourceAllocInput;
    /*This stores the dciFormat which will be used for Transmission of this
     Message*/    
     DCIFormat dciFormat;
    UInt8 *data_p; /*Paging message buffer*/
    UInt8 dataLen; /*Paging message buffer lenght*/
    /*This field stoes the CCE Index which has been allocated to 
      Paging Control  Message */
    UInt8 cceIndex;
    /* SPR 3062 Chg */
    /* This field stores the tpc lsb index values for pcch msg */
    UInt8 tpc;
    /* SPR 3062 Chg Ends */
}DLPCCHMsgInfo;

typedef struct DLRACHMsgInfoT

{
    /*This is pointer to ResourceAllocatorInput structure which stores the 
      infomation needed by Resource Allocator for allocating RBs */
    ResourceAllocatorInput resourceAllocInput;
    /*This stores the dciFormat which will be used for Transmission of this
      Message*/    
    DCIFormat dciFormat;
    UInt16 tempCRNTI;
    UInt16 raRNTI; /*RARNTI for RAR PDU*/
    UInt8 *data_p; /*RAR message buffer*/
    UInt8 dataLen; /*RAR message buffer length*/
    /*This field stoes the CCE Index which has been allocated to PRACH Message */
    UInt8 cceIndex;
    /* SPR 3062 Chg */
    /* This field to send TPC for RACH Msg in DCI 1A */
    UInt8 tpc;
    /* SPR 3062 Chg Ends */
}DLRACHMsgInfo;

typedef struct DLCCCHMsgInfoT

{
    /*This is pointer to ResourceAllocatorInput structure which stores the 
      infomation needed by Resource Allocator for allocating RBs */
    ResourceAllocatorInput resourceAllocInput;
    /*This stores the dciFormat which will be used for Transmission of this
      Message*/    
    DCIFormat dciFormat;
    UInt32 paddingInBytes; /*Not required to be deleted*/
    UInt16 ueId; /*rnti*/
    UInt8 *data_p; /*CCCH message buffer*/
    UInt8 dataLen;/*CCCH message buffer length*/
    UInt8 harqProcessId;
    /* This field stoes the CCE Index which has been allocated to
     *Common Control Channel Message */
    UInt8 cceIndex;
    /* SPR 4395 changes start */
    UInt8 rejFlag;  
    /* SPR 4395 changes end */
}DLCCCHMsgInfo;

typedef struct DLContentionReslMsgInfoT

{
    /*This is pointer to ResourceAllocatorInput structure which stores the 
      infomation needed by Resource Allocator for allocating RBs */
    ResourceAllocatorInput resourceAllocInput;
    /*This stores the dciFormat which will be used for Transmission of this
      Message*/    
    DCIFormat dciFormat;
    UInt16 tempCRNTI; 
    UInt8 *data_p; /*Contention message buffer*/
    UInt8 dataLen; /*Contention message buffer length*/
    /*This field stoes the CCE Index which has been allocated 
     *to Contention Resoultion Message*/
    UInt8 cceIndex;    
    UInt8 harqProcessId;;
}DLContentionReslMsgInfo;

typedef union DLMsgInfoT
{
    /*This is structure which stores information of those Data Messages for 
      which PDSCH Region need to be allocated*/
    DLDataMsgInfo dlDataMsgInfo;
    /*This is structure which stores information of those BCCH Messages for 
      which PDSCH Region need to be allocated*/
    DLBCCHMsgInfo dlBCCHMsgInfo ;
    /*This is structure which stores information of those PCCH Messages for 
      which PDSCH Region need to be allocated*/
    DLPCCHMsgInfo dlPCCHMsgInfo;
    /*This is structure which stores information of those RACH Messages for 
      which PDSCH Region need to be allocated*/
    DLRACHMsgInfo dlRACHMsgInfo;
    /*This is structure which stores information of those CCCH Messages for 
      which PDSCH Region need to be allocated*/
    DLCCCHMsgInfo dlCCCHMsgInfo;
    /*This is structure which stores information of those Contention Resoultion
     * Messages for 
     which PDSCH Region need to be allocated*/
    DLContentionReslMsgInfo dlContentionReslMsgInfo;
} DLMsgInfo;

typedef struct DLMsgTypeMsgInfoT
{
    /*This is structure which stores informatinon of messages like BCCH,DATA
      PCCH ,CCCH etc for which PDSCH Region need to be allocated */
    DLMsgInfo dlMsgInfo;
    /*This field stores the message type for which the PDCCH has been allocated
      and for which PDSCH region need to be allocted .For example it could be 
      Broad Cast Message ,Paging Control Message,Data Messahe ,Contention 
      Resolution Message etc*/
    MsgType msgType; 
}DLMsgTypeMsgInfo;

/* CLPC_CHG */
/* This is structure which stores information of those TPC messages for
 * which tpc bitmap needs to be encoded for PUCCH */
typedef struct TpcMsgInfoForPucchT
{
    /* This stores the dciFormat which will be used for Transmission of tpc msg
     */
    UInt8 dciFormat;

    /* This field stoes the CCE Index which has been allocated. */
    UInt8 cceIndex;

    /* TPC .RNTI for which bitmap needs to be encoded. */
    UInt16 tpcRnti;
    
    /* Identifier for TPC-RNTI-CONTEXT */
    UInt16 tpcRntiIndex;

}TpcMsgInfoForPucch;
/* CLPC_CHG END */

typedef struct PdcchDLOutputInfoT
{
    /* 5492 changes start */
    /* SPR 4063 Start */
    /* Stores sum of count of following scheduled messages
     * 1) Contention Resolution Messages
     * 2) CCCH Messages
     * 3) PDCCH Order
     */
    /* UInt8 countContResCcchPdcchOrderMsg; */
    /* SPR 4063 End */
    /* 5492 changes end */

    /*This stores the number of DL Message  for which PDSCH Region need to be
      allocated.Message could be  BCCH,RACH,CCCH,PCCH and Contention
      Resoultion Message */
    UInt8 countCommonChannelMsg;
    
    /* ICIC changes start */
    /*This stores the number of DL Message for which PDSCH Region need to be 
      allocated.Message could be 
      1)Data Msg
      2)Data with TA
      3)Data with TA and DRX
      4)TA Msg
      5)DRX Msg
      6)TA and DRX Msg */
    /* countMsg[0] gives number of DL Messages for which PDSCH Region need to 
     * be allocated from Cell Centre resource region. 
     * countMsg[1] gives number of DL Messages for which PDSCH Region need to
     * be allocated from Cell Edge resource region 
     */
    UInt8 countMsg[MAX_USER_LOCATION];

    /* This stores number of DL retransmissions for which PDSCH Region need to
     * be allocated. countPriorityMsg[0] is for cell centre resource region &
     * countPriorityMsg[1] is for cell edge resource region 
     */
    UInt8 countPriorityMsg[MAX_USER_LOCATION];
    /* ICIC changes end */
    /* CLPC_CHG */
    /* Stores the number DL messages using DCI Format 3/3A (TPC MSG) for which 
     * bitmap needs to be encoded after PDSCH. */
    UInt8 countTpcMsg;
    /* CLPC_CHG END */

    /* ICIC changes start */
    DLMsgTypeMsgInfo dlMsgTypeCommonMsgInfo[
        MAX_COMMOM_CH_MSG_SCHEDULED +
        MAX_CCCH_UE_SCHEDULED];

    DLMsgTypeMsgInfo dlMsgTypeMsgInfo[MAX_USER_LOCATION]
        [MAX_DL_UE_SCHEDULED];

    /* CLPC_CHG */
    /* This is structure which stores information of those TPC messages for 
     * which tpc bitmap needs to be encoded for PUCCH, */
    TpcMsgInfoForPucch tpcMsgInfoForPucch[MAX_TPC_MSG_PER_TICK];
    /* CLPC_CHG END */

}PdcchDLOutputInfo;


typedef struct ULMsgInfoT
{
    UInt16 ueIndex;
    UInt8 cceIndex;
    UInt8 requiredRB;
    UInt8 strictAllocRB;
}ULMsgInfo;

#ifdef FDD_CONFIG
typedef struct DirectIndexingUEInfoT
{
    ULGrantRequestType ulGrantRequestType;
    /* SPR 15909 fix start */
    tickType_t  ttiCounter;
    /* SPR 15909 fix end */
    UInt8 harqIndex;
    UInt8 dlCCEIndex;
    UInt8 ulCCEIndex;
/*SPS_CHG*/
    UInt8 isSPSCCE;
/*SPS_CHG*/
    UInt8 flag; 
    UInt8 strictRBAllocation;
    UInt8 requiredRB;
/*Semi Static changes Start*/
    DCIFormat dlDciFormat; 
    UInt16 ulschUciInfoIndex;
/* SPR 14877 SRS Changes */
   UInt16 ulschInfoIndex;
/* SPR 14877 SRS Changes */
    UInt8  transMode; /* SS_S2 */ 
    UInt8  ri; /* SS_S2 */ 
/*Semi Static changes End*/
    UInt8 uePosInPdcchULNewUEArray;
/* UL_MU_MIMO_CHG Start */            
    /* This stores the Buddy Ue Index */     
    UInt16  buddyUeIndex;                    
/* UL_MU_MIMO_CHG End */              
}DirectIndexingUEInfo;
#endif

typedef struct PdcchDLUEInfoT
{
    UInt8 count;
    UInt16  pdcchDLUEArray[MAX_DL_UE_SCHEDULED + MAX_CCCH_UE_SCHEDULED];
//hack fix for count being reset to zero
#ifdef TDD_CONFIG
    UInt16 cumulativeUeCountForDlGroup;
    UInt16  pdcchDLUEArrayForDlGroup[(MAX_DL_UE_SCHEDULED + MAX_CCCH_UE_SCHEDULED)*MAX_DL_GROUP_SIZE_TDD];
#endif
}PdcchDLUEInfo;

/*UL New UE infomation from strategy node is populated into 
  this structure*/
typedef struct PdcchULNewUET
{
    UInt16 ueIndex;
    UInt8 avgMCS;
    UInt32 minDataSize;
    UInt32 maxDataSize;
    /* ICIC changes start */
    UInt8 resourceRegionAllocated;
    /* ICIC changes end */
    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    /* Denotes Number of U subframe, PDCCH schedules in a TTI (maximum value 2)
     * 1 indicates scheduled for 1 ulsubframe and 2 indicates scheduled for
     * 2 ulsubframe.
     */
    UInt8 scheduleCount;
    /* SPR 11065 Changes Start */
    /*
     * Indicate whether UE is scheduled for second ulsubframe or not.
     * PUSCH uses this to reset CCE in case of PUSCH resource failure.
     * TRUE -> present in second node, FALSE -> not present in second
     * node
     */
    UInt8 isNodePresentIn2UlSubframe;
    /* SPR 11065 Changes End */
#endif
    /*TDD Config 0 Changes End*/
}PdcchULNewUE;

/*UL NACK UE Info: Non-Adaptive & Adaptive UEs*/
typedef struct PdcchULNackUEInfoT
{
    UInt16 nonAdaptiveCount;
/* ICIC changes start */
    UInt16 adaptiveCount[MAX_USER_LOCATION];
    UInt16 pdcchULNackNonAdaptiveUEArray[MAX_UL_UE_SCHEDULED];
    UInt16 pdcchULNackAdaptiveUEArray[MAX_USER_LOCATION][MAX_UL_UE_SCHEDULED];
/* ICIC changes end */
}PdcchULNackUEInfo;

typedef struct PdcchULNewUEInfoT
{
    UInt8 count;
    PdcchULNewUE pdcchULNewUEArray[MAX_UL_UE_SCHEDULED];
}PdcchULNewUEInfo;

/* SPR 1101 changes start*/
typedef struct TcrntiCCEIndexInfoT 
{
    UInt8 cceIndex;
#ifdef TDD_CONFIG    
    UInt8 dlSubFrame;
#endif    
    UInt16 tcrnti;
}TcrntiCCEIndexInfo;

typedef struct PdcchTemporaryContextUEInfoT 
{
    UInt8 count;
#ifdef FDD_CONFIG    
    TcrntiCCEIndexInfo pdcchTempUEArray[MAX_CONTENTION_UE_SCHEDULED];
    /* SPR 4301 chg */
#elif TDD_CONFIG
    UInt16 cumulativeUeCountForTempUlGroup;
    TcrntiCCEIndexInfo pdcchULUEArrayForTempUlGroup[MAX_CONTENTION_UE_SCHEDULED * MAX_DL_GROUP_SIZE_TDD];
#endif
    /* SPR 4301 chg */
}PdcchTemporaryContextUEInfo;
/* SPR 1101 changes end*/

/* CLPC_CHG */
/* Store the information related to the TPC MSG scheduled */
typedef struct TpcMsgInfoForPuschT 
{
    /* This stores the dciFormat which will be used for Transmission of tpc msg
     */
    UInt8 dciFormat;

    /* This field stoes the CCE Index which has been allocated */
    UInt8 cceIndex;

    /* TPC-RNTI for which bitmap needs to be encoded. */
    UInt16 tpcRntiIndex;

}TpcMsgInfoForPusch;

/* This shall store the information about the TPC-RNTI for which CCEs are 
 * allocated and bitmap needs to be allocated for PUCCH powre control. It shall
 * be used for storing the information of Ues whose TPC needs to be triggered
 * with DCI3/3A*/
typedef struct PdcchTpcMsgInfoForPuschT
{
    /* The number of TPC MSG which needs to be encoded */
    UInt8 count;

    /* This shall be array of four that will store the information related 
     * to the TPC MSH scheduled */
    TpcMsgInfoForPusch tpcMsgInfoForPusch[MAX_TPC_MSG_PER_TICK];

}PdcchTpcMsgInfoForPusch;
/* CLPC_CHG END */


/*UL_MU_MIMO_CHG_START*/

typedef struct PdcchULMUMIMOCandidateUET
{
    UInt16 ueIndex;
  
} PdcchULMUMIMOCandidateUE;

/*This stores the ueIndexes for those UE which are eligible for UL MU-MIMO and their count*/
typedef   struct  PdcchULMUMIMOCandidateInfoT
{
    UInt8 count;
    PdcchULMUMIMOCandidateUE pdcchULMUMIMOCandidateUE[MAX_UL_UE_SCHEDULED];
} PdcchULMUMIMOCandidateInfo;


/*UL_MU_MIMO_CHG_END*/


typedef   struct  PdcchULOutputInfoT
{  
    /*This stores those DL UE information for which PDCCH region has been 
      allocated .PUCCH  will use this for reserving the Resources for DL ACK
      Nackin advance for the DL UE for which PDCCH region has been allocated*/  
    PdcchDLUEInfo pdcchDLUEInfo;
#ifdef FDD_CONFIG
    /*This stores the Information of those UL UE for which PDCCH region has 
      been allocated .This would be used by PDSCH and PUCCH .*/
    DirectIndexingUEInfo directIndexingUEInfo[MAX_UE_SUPPORTED];       
#endif
#ifdef FDD_CONFIG
    /*This stores the Information of those temp UE for which PDCCH region has 
      been allocated .This would be used by PDSCH and PUCCH .*/
    DirectIndexingUEInfo directIndexingTempUEInfo[MAX_UE_SUPPORTED];       
#endif
    /*This stores the ueIndes and cceIndex for those UE for which PDCCH region
      has been allocated */
    PdcchULNackUEInfo pdcchULNackUEInfo;
    /*This stores the ueIndes and cceIndex for those UE with T-CRNTI for which PDCCH region
      has been allocated retransmission of MSG3 */
    PdcchULNackUEInfo pdcchULNackTempUEInfo;
#ifdef FDD_CONFIG
    /* For  TTIB_Code */
    PdcchULNackUEInfo pdcchULNackTtiBundledUEInfo;
    /* + SPS_TDD_Changes */
#endif

    /*This stores the ueIndes and cceIndex for those SPS UE with CRNTI for which PDCCH region
      has been allocated for retransmission of SPS UE */
    PdcchULNackUEInfo pdcchULNackSpsUEInfo;
    /* - SPS_TDD_Changes */

    /* ICIC changes start */
    /*This stores the ueIndes and cceIndex for those UE for which PDCCH region
      has been allocated */
    /*This stores the ueIndes and cceIndex for those Cell Center(index -0) and
     * Cell Edge(index -1) UE for which PDCCH region
      has been allocated */
    PdcchULNewUEInfo pdcchULNewUEInfo[MAX_USER_LOCATION];   
    /* ICIC changes end */

    /* SPR 1101 changes start*/
    /*This stores the TC-RNTI and cceIndex for those UE for which PDCCH region
      for Contention Resolution Message has been allocated  */
    PdcchTemporaryContextUEInfo pdcchTcrntiUEInfo;
    /* SPR 1101 changes end*/

    /* CLPC_CHG */
    /* This structures stores the information about the TPC RNTI for which CCEs
     * are allocated and bitmap needs to be encoded for PUSCH power control */
    PdcchTpcMsgInfoForPusch pdcchTpcMsgInfoForPusch;
    /* CLPC_CHG END */


/*UL_MU_MIMO_CHG_START*/
    /*This stores the ueIndexes for those UE which are eligible for UL MU-MIMO   */
    /*This stores the ueIndexes for those Cell Center(index -0) and
     * Cell Edge(index -1) UE for which PDCCH region has been allocated */
    PdcchULMUMIMOCandidateInfo pdcchULMUMIMOCandidateInfo[MAX_USER_LOCATION][MAC_MAX_NUM_MOD_SCHEME];
/*UL_MU_MIMO_CHG_END*/


}PdcchULOutputInfo ;

typedef struct PDCCHOrderNodeT
{
    LTE_SNODE anchorNode;
    UInt8 preambleIndex;
    /* +- SPR 18268 */
    UInt16 ueIndex;
    /* +- SPR 18268 */
    /* SPR 15909 fix start */
    tickType_t currentGlobalTTI;
    /* SPR 15909 fix end */
}PDCCHOrderNode;

#ifdef DL_UL_SPLIT
// Node Structure to put into the puschSignalQueue_g queue so that 
// PDCCH will put the node into when CCE allocation finished and UL
// thread can start PUSCH processing
typedef struct PuschSignalNode
{
    UInt8 isCCEAllocated;
    LTE_SNODE nodeAnchor;
}PuschSignalNodeP;

#elif DL_UL_SPLIT_TDD
// Node Structure to put into the pdcchSignalQueue_g queue so that 
// PDCCH will put the node into when CCE allocation finished for DL
// and UL thread can start PDCCH allocation based on available CCE
typedef struct PuschSignalNode
{
    UInt32 availableCCE;
}PdcchSignalNodeP;

#endif

/* This structure is retransmission failure queue node 
 */

typedef enum CCEAllocationTypeT
{
    NO_CCE_ALLOCATED = 0,    
    UL_NON_ADAPTIVE_NO_CCE,
    UL_ADAPTIVE_CCE,
    DL_UE_CCE_1A_COMMON_SEARCH_SPACE,
    DL_UE_CCE_1A_UE_SEARCH_SPACE,
    DL_UE_CCE_OTHER_THAN_1A_UE_SEARCH_SPACE,
    UL_DL_CCE_1A_UE_SEARCH_SPACE
}CCEAllocationType;

/*RETX_CHG*/
typedef struct DLRetxFailureQueueNodeT
{
  LTE_SNODE anchorNode;
  UInt8 isNodeInvalid;
  UInt16 ueIndex;
  DLStrategyTxNode *dlStrategyTxNode_p;
}DLRetxFailureQueueNode;
/*RETX_CHG*/

typedef enum DciSizeStepT
{
    SMALL_DCI_STEP_SIZE,
    MEDIUM_DCI_STEP_SIZE,
    BIG_DCI_STEP_SIZE
}DciSizeStep;
#ifdef TDD_CONFIG
/* TDD Config 0 Changes Start */
typedef struct pdcchInfoT
{
    PdcchULOutputInfo* pdcchULOutput_p;
    PdcchULNackUEInfo *nackUEInfo_p;
    PdcchULNackUEInfo *nackTempUEInfo_p;
    /* SPR 18510 - start*/
    /* + SPS_TDD_CHANGES */
    PdcchULNackUEInfo *nackSpsUEInfo_p;
    /* - SPS_TDD_CHANGES */
    /* SPR 18510 - end*/
    UInt8 rbMapNum;
    //Variables Used for Nack Q Processing
    UInt8 localReTxRBMap[MAX_NUM_RBS + 1];
}PdcchInfo;
/* TDD Config 0 Changes End */
#endif
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/* ICIC changes start */
typedef struct DCIFormatToAggregationLevelT
{
    UInt32 dciFormatToAggregationLevel[MAX_USER_LOCATION][MAX_NUM_DCI_FORMAT];
} DCIFormatToAggregationLevel;
extern DCIFormatToAggregationLevel dciFormatToAggrLvlMapping_g[MAX_NUM_CELL];
/* ICIC changes end */
#ifdef TDD_CONFIG
extern PdcchULOutputInfo pdcchULOutputInfo_g[MAX_NUM_CELL][MAX_TDD_UL_PDCCH_CONTAINER][MAX_SUB_FRAME];
#elif FDD_CONFIG
extern PdcchULOutputInfo* pdcchULOutputInfo_gp[MAX_NUM_CELL];
#endif
/* SPR 5333 changes start */
#ifdef TDD_CONFIG
/** CA-TDD Changes Start **/
extern UInt8  miValueForSubframe_g[MAX_NUM_CELL]; 
/** CA-TDD Changes Stop **/
#endif
/* SPR 5333 changes end */
/* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
extern SInt32 availableULRBCount_g[MAX_NUM_CELL];
#endif
/* TDD Config 0 Changes End */
/* +- SPR 18481 */
extern UInt8 dciSize_g[MAX_DCI_FORMATS][MAX_DL_BW][ANTENNA_PORT_VARIATION];
/* +- SPR 18481 */

/* + CQI_5.0 */
extern DCIFormat preferredPrimaryDCIFormatForSingleTB_g[MAX_TRANSMISSION_MODES +  1];

extern DCIFormat preferredSecondaryDCIFormatForSingleTB_g[MAX_TRANSMISSION_MODES + 1];
/* - CQI_5.0 */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
#ifdef TDD_CONFIG
extern UInt8 roundTripTime (UInt8 ulSubframenum,
			    /* CA TDD CHANGES START */
			    InternalCellIndex internalCellIndex
			    /* CA TDD CHANGES END */
                );
#endif
/*pdcch for scell changes start*/
void resetExtraCCEsAllocated( DciCCEContainerInfo *dciCCEContainerInfo_p,
        InternalCellIndex internalCellIndex);
UInt8 extraCCEAllocated(
        DciCCEContainerInfo *dciCCEContainerInfo_p,UInt8 cceIndex);
UESpecificSearchSpaceInfo * getULUEContextSearchSpaceInfo(ULUEContext *ulUEContext_p,
        UInt32 subFrameNum,InternalCellIndex internalCellIndex);
DLMIMOInfo *getdlMIMOInfo( DLUEContext *dlUEContext_p,
		InternalCellIndex internalCellIndex);
UInt8 getUserLocationType( DLUEContext *dlUEContext_p,
		InternalCellIndex internalCellIndex);
UESpecificSearchSpaceInfo * getDLUEContextSearchSpaceInfo(DLUEContext *dlUEContext_p,
        UInt32 subFrameNum,InternalCellIndex internalCellIndex);

/*pdcch for scell changes end*/

extern void freeHARQProcess(
        UInt16 ueIndex,
        DLHARQProcess *harqProcess_p);

extern void pushNodeInCqiQueue(UInt16 ueIndex );
/* CLPC_CHG */
UInt32 allocatePdcchCommonChannel(
#ifdef FDD_CONFIG
#endif
        UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p,
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32     delayToPhy,
        #ifdef TDD_CONFIG
        UInt8 subFrameNum,
        UInt16 sysFrameNum,
        /* +- SPR 17777 */
        tickType_t currentGlobalTick,
        #endif
        PdcchULOutputInfo *pdcchULOutputInfo_p,
         UInt32 *maxUeScheduledInDownlink_p
         /* +- SPR 17777 */
        ,InternalCellIndex internalCellIndex );

extern void allocatePdcchForBCCH(
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        UInt32 *maxCommonSpaceCCE_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p,
        UInt32 sysFrameNum,
        UInt32 subFrameNum,
        InternalCellIndex internalCellIndex);

extern void allocatePdcchForPCCH(
        PdcchDLOutputInfo *pdcchDLOutputInfo_p, 
        UInt32 *maxCommonSpaceCCE_p, 
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p,
        UInt32 sysFrameNum,
        UInt32 subFrameNum,
        InternalCellIndex internalCellIndex);

extern void allocatePdcchForRACH(PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        UInt32 *maxCommonSpaceCCE_p,
        DciCCEInfo *dciCCEInfo_p, 
        UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        /* SPR 15909 fix start */
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
        tickType_t currentGlobalTick,
#endif
        /* SPR 15909 fix end */
        UInt32 *maxUeScheduledInDownlink_p
        /* +- SPR 17777 */
        ,InternalCellIndex internalCellIndex
        );

extern void allocatePdcchForCCCH(
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        UInt32 *maxCommonSpaceCCE_p, 
        DciCCEContainerInfo * dciCCEContainerInfo_p,
        UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        PdcchULOutputInfo *pdcchULOutputInfo_p
        /* 5492 changes start */
        ,UInt32 *maxUeScheduledInDownlink_p
        /* 5492 changes end */
        ,InternalCellIndex internalCellIndex
        );

/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType isCCEAvailableAggrLvlEightUESpace(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt8 *cceIndexArray_p,
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p);

/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType isCCEAvailableAggrLvlFourUESpace(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt8 *cceIndexArray_p,
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p);

/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType isCCEAvailableAggrLvlTwoUESpace(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt8 *cceIndexArray_p,
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p);

/* SPR 5599 changes start (ZIP ID 129059) */
extern  MacRetType isCCEAvailableAggrLvlOneUESpace(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt8 *cceIndexArray_p,
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p, 
        DciCCEInfo *dciCCEInfo_p
        );

/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType isCCEAvailableAggrLvlEightCommonSpace(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        InternalCellIndex internalCellIndex);

/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType isCCEAvailableAggrLvlFourCommonSpace(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        InternalCellIndex internalCellIndex);

/* SPR 6856 changes start */
MacRetType allocatePDCCHForDCIFormatUESpace(
        /*pdcch for scell changes start*/
        DLUEContext *dlUEContext_p,
        /*pdcch for scell changes end*/
        UESpecificSearchSpaceInfo*ueSearchSpaceInfo_p,
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p, 
        DciCCEInfo * dciCCEInfo_p,
        UInt8 aggregationLevel,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        InternalCellIndex internalCellIndex);

MacRetType allocatePDCCHForDCIFormatCommonSpace(
            UInt32 aggregationLevel,
            UInt32 *cceAllocated_p,
            UInt32 *availableCCE_p,
            DciCCEInfo * dciCCEInfo_p,
            InternalCellIndex internalCellIndex);
/* SPR 6856 changes end */

extern UInt32 schedulePCCHMsg(
                      PdcchDLOutputInfo *pdcchDLUEOutputInfo_p,
                      UInt32 *availableCommonCCE_p,
                      SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        UInt32 sysFrameNum,
        InternalCellIndex internalCellIndex);

/*extern UInt32 scheduleBroadcastMsgs(
        PdcchDLOutputInfo  *pdcchDLOutputInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *availableCommonCCE_p, 
        UInt32 sysFrameNum, 
        UInt32 subFrameNum, 
        SInt32 *availableDLRBCount_p);*/

extern void scheduleRACHMsg(PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                     DciCCEInfo *dciCCEInfo_p,
                     UInt32 *availableCCE_p,
                     UInt32 *availableCommonCCE_p, 
                     SInt32 *availableDLRBCount_p,
                     UInt32 subFrameNum,
                     DciCCEContainerInfo *dciCCEContainerInfo_p,
                     PdcchULOutputInfo *pdcchULOutputInfo_p,
                     /* SPR 15909 fix start */
                     /* +- SPR 17777 */
#ifdef TDD_CONFIG
                     tickType_t currentGlobalTick,
#endif
                    /* SPR 15909 fix end */
                     /* 5492 changes start */
                     UInt32 *maxUeScheduledInDownlink_p
                     /* +- SPR 17777 */
                     /* 5492 changes end */
                     ,InternalCellIndex internalCellIndex
                     );

extern void  scheduleCCCHMsg (
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *availableCommonCCE_p,
        UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        PdcchULOutputInfo *pdcchULOutputInfo_p
        /* 5492 changes start */
        ,UInt32 *maxUeScheduledInDownlink_p
        /* 5492 changes end */
        ,InternalCellIndex internalCellIndex
       );

/* SPR 4395 changes start */
extern void  schedulePendingCCCHMsg (
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *availableCommonCCE_p,
        UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        PdcchULOutputInfo *pdcchULOutputInfo_p
        /* 5492 changes start */
        ,UInt32 *maxUeScheduledInDownlink_p
        /* 5492 changes end */
        ,InternalCellIndex internalCellIndex
       );
/* SPR 4395 changes end */

/* SPR 5333 changes start */
#ifdef FDD_CONFIG
extern void initUESecificSearchSpaceOfAllUEs(UInt8 maxNumCCE,UInt8 cellIndex ,
    InternalCellIndex internalCellIndex);
#else
/* +- SPR 17777 */
extern void initUESecificSearchSpaceOfAllUEs(UInt8 cellIndex , 
/* +- SPR 17777 */
    InternalCellIndex internalCellIndex);
#endif
/* SPR 5333 changes end */

/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType putEntryInAdvanceAllocInfoQueue(
/* SPR 5599 changes end (ZIP ID 129059) */
        DLStrategyTxNode *dlStrategyTxNode_p,
        InternalCellIndex internalCellIndex);

/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType putEntryInRetransmissionFailureQueue(
/* SPR 5599 changes end (ZIP ID 129059) */
        DLStrategyTxNode *dlStrategyTxNode_p,
        InternalCellIndex internalCellIndex);

/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType putEntryInULSchedulerNewTxQueue(
/* SPR 5599 changes end (ZIP ID 129059) */
        ULStrategyTxNode *ulStrategyTxNode_p
        ,InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        ,tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
        ,UInt8 ulSubFrameNum
        ,ULUEContext   *ueULContext_p
#endif
        /* TDD Config 0 Changes End */
        );

/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType putEntryInDLSchedulerReTxQueue (
/* SPR 5599 changes end (ZIP ID 129059) */
        DLStrategyTxNode *dlStrategyTxNode_p,
        InternalCellIndex cellIndex);

/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType putEntryInDLSchedulerNewTxQueue(
/* SPR 5599 changes end (ZIP ID 129059) */
        DLStrategyTxNode *dlStrategyTxNode_p,
        InternalCellIndex internalCellIndex);

/*QOS_CHG*/
extern void deleteScheduledLCList( DLUEContext *ueDLContext_p,
                                   InternalCellIndex internalCellIndex);
/*QOS_CHG*/

#ifdef FDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
extern void  allocPdcchPdschForDL(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt32 withMuxFlag,
        /* +- SPR 17777 */
        UInt32 delayToPhy,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
 /*SPR 5117 changes starts */
        UInt32 subFrame,
/*SPR 5117 changes ends */
        InternalCellIndex  cellIndex,
        ExecutionLegFunctionArg* execArgs
        );
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType processULNackQInDelayNot3(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
         DciCCEContainerInfo * dciCCEContainerInfo_p);
#elif TDD_CONFIG

/* SPR 5599 changes start (ZIP ID 129059) */
void  allocPdcchPdschForDCIFormat0AndDLInNormalSF(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubFrameNum,
        InternalCellIndex internalCellIndex,
       /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
        );
/* SPR 5599 changes start (ZIP ID 129059) */
void  allocPdcchPdschForDCIFormat0AndDLInSpecialSF(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubFrameNum,
        InternalCellIndex internalCellIndex,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs);

/*Complexity Start*/
void eicicUpdateSfAndSfn(UInt32 *sysFrameNum,UInt32 *subFrameNumTemp);
UInt8 eicicFindNumOfBitsinABSPattern(InternalCellIndex internalCellIndex);
/*Complexity End*/

/* SPR 5599 changes start (ZIP ID 129059) */
void  allocPdcchPdschForDCIFormat0(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubFrameNum,
        InternalCellIndex internalCellIndex,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
        );
/* SPR 5599 changes start (ZIP ID 129059) */
void  allocPdcchPdschForDLInNormalSF(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick ,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubFrameNum,
        InternalCellIndex internalCellIndex,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
        );
/* SPR 5599 changes start (ZIP ID 129059) */
void  allocPdcchPdschForDLInSpecialSF(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubFrameNum,
        InternalCellIndex internalCellIndex,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        ExecutionLegFunctionArg* execArgs
        );
        /* TDD Config 0 Changes End */

/* SPR 5599 changes start (ZIP ID 129059) */
void  dummyAllocForSpecialSF(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubFrameNum,
        InternalCellIndex internalCellIndex,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
       );

#ifdef DL_UL_SPLIT_TDD
void  allocPdcchForUl(
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubframeNum,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
		);
#endif

/* TDD Config 0 Changes Start */
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType processULNackQinTDD (
/* SPR 5599 changes end (ZIP ID 129059) */
                         PdcchInfo* localPdcchInfoMap[],
                         /* SPR 15909 fix start */
					     tickType_t
					     currentGlobalTickWithDelay,
                         /* SPR 15909 fix end */
					     UInt32 * availableCCE_p,
					     DciCCEInfo * dciCCEInfo_p,
					     DciCCEContainerInfo *dciCCEContainerInfo_p,
                         /* SPR 5798 Fix Start */
                         UInt8 *maxUeScheduledInUplink_p
                         /* SPR 5798 Fix End */
                         ,InternalCellIndex internalCellIndex 
                        );
/* TDD Config 0 Changes End */


UInt8 updateDai(DLUEContext *ueContext_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobaTick,
        /* SPR 15909 fix end */    
        UInt32 subframeWithDelay,
        InternalCellIndex internalCellIndex) ;
/* + SPS_TDD_Changes */
UInt8 updateDaiSPS(DLUEContext *ueContext_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobaTick,
        /* SPR 15909 fix end */    
        UInt32 subframeWithDelay,
        InternalCellIndex internalCellIndex) ;
/* - SPS_TDD_Changes */
			   
UInt8 getDai(UInt16 ueIndex,
             /* SPR 15909 fix start */
             tickType_t currentGlobaTick,
             /* SPR 15909 fix end */
             UInt8 subframeWithDelay,
             InternalCellIndex internalCellIndex) ;
#endif
/* SPR 5599 changes start (ZIP ID 129059) */
/*ca-tdd pucch code changes*/
extern MacRetType putEntryInULAperiodicCQIQueue(UInt32 ueIndex, InternalCellIndex cellIndex
        /* CA_phase2_csi_code Start */
        ,UInt8 bitMaskForAperiodicReport
        /* CA_phase2_csi_code End */
        );
/* SPR 5599 changes end (ZIP ID 129059) */

extern void initPDCCH (InternalCellIndex internalCellIndex);
extern void cleanupPDCCH (InternalCellIndex internalCellIndex);
extern void deInitPDCCH (InternalCellIndex cellIndex);

/*SPR_3305_FIX_START*/
/* SPR 5599 changes start (ZIP ID 129059) */
    /* SPR 15909 fix start */
MacRetType handleNackNodeForTcrnti( UInt16 tempUeIndex, tickType_t tempPhyTti,
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt8 ulDelay, tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        SInt32 *availableULRBCount_p,
        UInt16 *nonAdaptiveArrayIndex_p,
        UInt16 *adaptiveArrayIndex_p,
        /* ICIC changes start */
        UInt8 userLocationType
        /* SPR 5798 Fix Start */
#ifdef FDD_CONFIG
        ,UInt32 *maxUeScheduledInUplink_p
#endif
        /* SPR 5798 Fix End */
        ,InternalCellIndex internalCellIndex
        );
        /* ICIC changes end */

/* msg3_retx_tdd_start */
#ifdef TDD_CONFIG
MacRetType handleNackNodeForTcrntiTdd( 
        UInt8 harqIndex,
        UInt16 tempUeIndex, 
        /* SPR 15909 fix start */
        tickType_t tempPhyTti,
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt8 userLocationType,
        UInt8 *maxUeScheduledInUplink_p,
        UInt8 ulSubFrameNum,
        PdcchInfo* pdcchInfo_p
        ,InternalCellIndex internalCellIndex);
#endif
/* msg3_retx_tdd_end */
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType allocateCCEInCommonSpaceForMsg3ReTx(UInt32 aggregationLevel,
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p,
        DciCCEInfo * dciCCEInfo_p);

/*SPR_3305_FIX_END*/
/*Queue to trigger add/delete UE from DL Scheduler Active List*/
extern UL_SYNC_LOSS_TRIGGERQ_DL_TYPE* ulSyncLossTriggerQDL_gp[MAX_NUM_CELL];
/*Queue to trigger add/delete UE from UL Scheduler Active List*/
extern UL_SYNC_LOSS_TRIGGERQ_UL_TYPE* ulSyncLossTriggerQUL_gp[MAX_NUM_CELL];

/* SPR 15909 fix start */
extern UlSyncLossOnOff  checkUlSyncLossForUE(DLUEContext *dlUEContext_p, tickType_t currentGlobalTTITickCount,InternalCellIndex internalCellInde);
/* SPR 15909 fix end */
extern UlSyncLossOnOff  checkSyncLossTimerExpiry(DLUEContext *dlUEContext_p);
/*SPR_3065_FIX*/
extern void sendSyncIndToRRC(UInt16 ueIndex, UInt16 rnti, SyncIndId syncStatus, InternalCellIndex internalCellIndex);
/*CA Changes end  */
/*SPR_3065_FIX*/
    
/* QOS PDSCH Change Start */
#ifdef FDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
extern  MacRetType allocRBInPDSCHForSpatialMul(
/* SPR 5599 changes end (ZIP ID 129059) */
    /* + CQI_5.0 */
    UInt32 macAdvanceFactor,
    /* - CQI_5.0 */
    ResourceAllocatorInput *resourceAllocInput_p,
    ResourceAllocatorOutput *resourceAllocOutput_p,
    DLUEContext *ueDLContext_p,
    DLHARQProcess *harqProcess_p,
    ResourceAllocationPolicyInfo *raPolicyInfo_p,
    DLStrategyTxNode *dlStrategyTxNode_p
    /*Dynamic PDCCH + 5117 changes Start*/
    /* SPR 15909 fix start */
    ,tickType_t scheduledTick,
    /* SPR 15909 fix end */
    UInt8 *numOfOverLappingRb
    /*Dynamic PDCCH + 5117 changes End*/
    ,InternalCellIndex internalCellIndex
    );
#elif TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
extern  MacRetType allocRBInPDSCHForSpatialMul(
/* SPR 5599 changes end (ZIP ID 129059) */
    /* + CQI_5.0 */
    UInt32 macAdvanceFactor,
    /* - CQI_5.0 */
    ResourceAllocatorInput *resourceAllocInput_p,
    ResourceAllocatorOutput *resourceAllocOutput_p,
    DLUEContext *ueDLContext_p,
    DLHARQProcess *harqProcess_p,
    ResourceAllocationPolicyInfo *raPolicyInfo_p,
    DLStrategyTxNode *dlStrategyTxNode_p
    /*Dynamic PDCCH + 5117 changes Start*/
    /* SPR 15909 fix start */
    ,tickType_t scheduledTick,
    /* SPR 15909 fix end */
    UInt8 *numOfOverLappingRb,
    /*Dynamic PDCCH + 5117 changes End*/
    UInt8 subframe,
    InternalCellIndex internalCellIndex);
#endif

#ifdef FDD_CONFIG
extern MacRetType allocRBInPDSCHForNonSpatialMul(
    ResourceAllocatorInput *resourceAllocInput,
    ResourceAllocatorOutput *resourceAllocOutput,
    DLUEContext *ueDLContext_p,
    DLHARQProcess *harqProcess_p,
    ResourceAllocationPolicyInfo *raPolicyInfo_p,
    DLStrategyTxNode *dlStrategyTxNode_p
    /*Dynamic PDCCH + 5117 changes Start*/
    /* SPR 15909 fix start */
    ,tickType_t scheduledTick,
    /* SPR 15909 fix end */
    UInt8 *numOfOverLappingRb
    ,InternalCellIndex internalCellIndex
    /*Dynamic PDCCH + 5117 changes End*/
    );
#elif TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType allocRBInPDSCHForNonSpatialMul(
/* SPR 5599 changes end (ZIP ID 129059) */
    ResourceAllocatorInput *resourceAllocInput_p,
    ResourceAllocatorOutput *resourceAllocOutput_p,
    DLUEContext *ueDLContext_p,
    DLHARQProcess *harqProcess_p,
    ResourceAllocationPolicyInfo *raPolicyInfo_p,
    DLStrategyTxNode *dlStrategyTxNode_p
    /*Dynamic PDCCH + 5117 changes Start*/
    /* SPR 15909 fix start */
    ,tickType_t scheduledTick,
    /* SPR 15909 fix end */
    UInt8 *numOfOverLappingRb,
    /*Dynamic PDCCH + 5117 changes End*/
    UInt8 subFrame,
    InternalCellIndex internalCellIndex
    );
#endif
/* QOS PDSCH Change End */

/* CLPC_CHG */
/* This function shall process scheduleFormat3Or3AQueue to schedule DCI 
   Format 3/3A. */
void processScheduledFormat3Or3AQueue (
        UInt32* maxCommonSpaceCCE,
        PdcchDLOutputInfo * pdcchDLOutputInfo_p,
        PdcchTpcMsgInfoForPusch * pdcchTpcMsgInfoForPusch_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *availableCCE_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);

/* CLPC_CHG END */

#ifdef LTE_EMBMS_SUPPORTED
    void  allocatePdcchForDCI1CNotification(UInt32 * maxCommonSpaceCCE,
                DciCCEContainerInfo *dciCCEContainerInfo_p,
                UInt32 *availableCCE_p,
                InternalCellIndex internalCellIndex);
#endif

/* SPR 6856 changes end */
MacRetType allocatePdcchForPDCCHOrderMsg(
        DLUEContext *dlUEContext_p, 
        UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
        UInt32 *allocatedCCEIndex_p,
        UInt32 *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        InternalCellIndex internalCellIndex);
/* SPR 6856 changes end */
/* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
MacRetType putEntryInULSchedulerNewTxList(
        UInt8 subFrameNum,
        UInt8 ulSubFrameNum,
        ULStrategyTxNode *ulStrategyTxNode_p,
        ULUEContext   *ueULContext_p,
        /* CA TDD CHANGES START */
        InternalCellIndex internalCellIndex
        /* CA TDD CHANGES END */
        );
MacRetType pushListNodeInULSchedulerNewTxQueue(
        ULStrategyTxNode *ulStrategyTxNode_p,
        InternalCellIndex internalCellIndex);
#endif
/* TDD Config 0 Changes End */

#endif  

