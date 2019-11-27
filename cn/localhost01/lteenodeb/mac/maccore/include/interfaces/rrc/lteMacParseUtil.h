/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMACParseutil.h
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations for the LTE MAC Layer
 *                     Interface with RRC Layer.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacParseUtil.h,v $
 * Revision 1.2.6.2.4.2.2.1.14.1  2010/11/08 06:06:16  gur10121
 * Semi static changes merging
 *
 * Revision 1.2.6.2.4.2.2.1.8.1  2010/10/25 08:27:31  gur10121
 * Merging the Semi static changes on the latest
 *
 * Revision 1.2.6.2.4.2.2.1.6.1  2010/10/15 13:32:36  gur20439
 * changes to support PUSCH config
 *
 * Revision 1.2.6.2.4.2.2.1  2010/09/21 15:45:55  gur20491
 * FAPI changes
 *
 * Revision 1.2.6.2  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.40.28.2  2010/07/21 13:18:54  gur24420
 *  Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
 *  Overall Changes Cover:-
 *  1. enum UL_LC_RECONFIG_REQ & DL_LC_RECONFIG_REQ removed from RRCTag enumerations.
 *
 * Revision 1.40.28.1  2010/07/21 08:57:57  gur24420
 * Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
 * Overall Changes Cover:-
 * 1. Module ID aligned as agreed with MAC and RRC teams(PHY Module).
 * 2. SI_MSG_INFO_PARAM Enum added for RRC Tag  values as in API doc(2.6)
 *
 * Revision 1.40  2009/10/26 09:06:10  gur18550
 * Function Updated for 500 UEs
 *
 * Revision 1.39  2009/10/21 05:27:51  gur19413
 * socket functionality modified
 *
 * Revision 1.38  2009/09/02 17:26:35  gur19413
 * changed Unsinged to signed.
 *
 * Revision 1.37  2009/09/01 08:05:49  gur20052
 * increased the buffer size to 4096
 *
 * Revision 1.36  2009/09/01 08:03:02  gur19413
 * increased the buffer size
 *
 * Revision 1.35  2009/08/20 07:26:24  gur19413
 * remove function prototypes
 *
 * Revision 1.34  2009/07/22 15:19:52  gur11083
 * changed function argument *origTagLen to origTagLen of parseCreateLcReq
 *
 * Revision 1.33  2009/07/22 11:35:27  gur11083
 * incorporated review comment and change prototype of cqiMode validation
 *
 * Revision 1.32  2009/07/17 06:24:41  gur20439
 * changes related to rrc interface modification incorporated.
 *
 * Revision 1.31  2009/07/16 13:14:48  gur11083
 * changed to 16 bit for cqiPMIConfigIndex,cqiPucchResourceIndex,riConfigIndex, CQI_PMI_CONFIG_INDEX_INFO_LEN
 *
 * Revision 1.30  2009/07/16 08:08:43  gur19413
 * added new flag cqiInfoFlag
 *
 * Revision 1.29  2009/07/14 09:11:46  gur11083
 * added macro NUM_HARQ,INVALID_RB,INVALID_TRANS_MODE,INVLAID_NUM_OF_LAYER,INVALID_CODE_BOOK_INDEX,added validateueNumHarqProcess,modified validateCqiMode
 *
 * Revision 1.28  2009/07/01 04:21:12  gur15697
 * review comments incorporated
 *
 * Revision 1.27  2009/06/24 08:27:09  gur20470
 * incorporated review comments
 *
 * Revision 1.26  2009/06/24 04:39:15  gur20470
 * deleted MAX_MAX_RESOURCE_BLOCKS and MAC_MAX_RANGE_VALUE,MAC_MAX_SFN<MAC_MAX_SF
 *
 * Revision 1.25  2009/06/22 11:11:57  gur11083
 * change MAC_MIN_RANGE_VALUE to 1
 *
 * Revision 1.24  2009/06/22 08:11:58  gur20470
 * changed range value for MAX RES BLOCKS
 *
 * Revision 1.23  2009/06/22 05:28:25  gur20470
 * added cqi indication and cqimode for downlink structure
 *
 * Revision 1.22  2009/06/19 13:25:20  gur15697
 * updated for sendToRRC()
 *
 * Revision 1.21  2009/06/18 09:13:51  gur15697
 * lowerCRNTIBound_g and upperCRNTIBound_g modified to  lowerRNTIBound_g and upperRNTIBound_g
 *
 * Revision 1.20  2009/06/18 06:22:33  gur15697
 * PUCCH functionality added
 *
 * Revision 1.19  2009/06/12 11:14:02  gur11083
 * change for validation and added two new element add in 	ConfigCellReq sturcture for PHICH_CONFIG_INFO
 *
 * Revision 1.18  2009/06/10 10:54:24  gur20470
 * altered prototype for socket creation for OAM interface
 *
 * Revision 1.17  2009/06/09 11:37:54  gur20470
 * updated for mac 1.2
 *
 * Revision 1.16  2009/06/06 15:15:59  gur11083
 * changed for UE reconfig and delete
 *
 * Revision 1.15  2009/06/05 14:03:16  gur18569
 * fixed bugs
 *
 * Revision 1.14  2009/06/04 06:06:08  gur11083
 * changed tag length for LC create
 *
 * Revision 1.13  2009/06/03 11:25:14  gur11083
 * added  member simultaneousACKNACKAndCQI in uplink structure
 *
 * Revision 1.12  2009/06/02 06:57:41  gur20470
 * added MAC_PARTIAL_SUCCESS
 *
 * Revision 1.11  2009/06/01 12:19:26  gur20470
 * initialised globals for ports
 *
 * Revision 1.10  2009/06/01 07:43:01  gur18569
 * commenting lteMacCellConfMgr.h
 *
 * Revision 1.9  2009/06/01 06:39:41  gur18569
 * removed compilation errors
 *
 * Revision 1.8  2009/05/29 10:20:20  gur18569
 * removed compilation error
 *
 * Revision 1.7  2009/05/29 06:25:36  gur18569
 * removed compilations errors
 *
 * Revision 1.6  2009/05/28 13:26:48  gur20470
 * incorporated review comments
 *
 * Revision 1.5  2009/05/21 15:26:52  gur20470
 * resolved errors
 *
 * Revision 1.4  2009/05/19 13:00:05  gur20470
 * *** empty log message ***
 *
 * Revision 1.3  2009/05/18 07:28:58  gur20470
 * range for validations added
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_PARSE_UTIL_H
#define LTE_MAC_PARSE_UTIL_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteLog.h"
#include "lteMsgPool.h"
#include "lteMacSFNMgr.h"
#include "lteMisc.h"
#include "lteMacULUEContext.h"
#include "lteLayer2CommanTypes.h"
#include "lteMacSpsCommonTypes.h"

#include "lteMacDRXMgr.h" /* DRX_CHG */
#include "lteMacMeasGapHandler.h" /* MEAS_GAP_CHG */
/* CA changes start */
#include "lteMacRRCInterface.h"
/* CA changes end */

 
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/
#ifndef MAC_AUT_TEST
extern LTE_SQUEUE* recvDataFromPhyQ_gp[MAX_NUM_CELL];
#endif
/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define PHYSIP "127.0.0.1"
#define PARSE_UTIL "PARSE_UTIL_MODULE"
#define MAX_DATA_SIZE          4096
#define MSGID_SIZE             1
#define IDX_SIZE               2
#define UEID_SIZE              2
#define RSP_SIZE               1
#define PWR_HR_SIZE            2
#define TAG_SIZE               2
#define LENGTH_SIZE            2
/*DRX_MEAS_GAP_CHG*/
#define DRX_CONFIG_TAG_MIN_VALUE            5
#define DRX_CONFIG_TAG_MAX_VALUE            29
#define DRX_SETUP_CONFIG_TAG_MIN_VALUE      10
#define DRX_SETUP_CONFIG_TAG_MAX_VALUE      24
#define DRX_SHORT_CONFIG_TAG_VALUE          6
#define DRX_SHORT_CYCLE_MIN_VALUE           1
#define DRX_SHORT_CYCLE_MAX_VALUE           16
#define MEAS_GAP_CONFIG_TAG_MIN_VALUE       5
#define MEAS_GAP_CONFIG_TAG_MAX_VALUE       21
#define MEAS_GAP_SETUP_TAG_VALUE            6
/*DRX_MEAS_GAP_CHG*/
/*START : DRX_MAC_CE*/
#define DRX_MAC_CE_INFO_TAG_VALUE           8
#define DRX_MAC_CE_TIMER_MIN_VALUE          3
#define DRX_MAC_CE_TIMER_MAX_VALUE          2559
#define DRX_MAC_CE_THRESH_MIN_VALUE         1
#define DRX_MAC_CE_THRESH_MAX_VALUE         2559
/*END   : DRX_MAC_CE*/

/* FAPI CHANGES START */
#define PARAM_REQUEST_WAIT_TIME             10
#define CONFIGCELL_RESPONSE_WAIT_TIME       5
/* PHY Response wait timer (1000ms) for STOP.request response */
#define PHY_STOP_RSP_WAIT_TIME              1
/* FAFI CHANGES END */

#ifdef FDD_CONFIG
#define MAX_CSI_MEASUREMENT_PATTERN 5
#else
#define MAX_CSI_MEASUREMENT_PATTERN 9
#endif

#define GET_CELL_INDEX(data_p,cellIndexOffset,cellIndexValue)\
{\
    data_p += cellIndexOffset;\
    cellIndexValue = (UInt8) *data_p;\
}

#define ASSIGNED_CELL_INDEX(cnfBuff, cellIndexOffset, cellIndexValue, currentOffset) \
{\
    if(cellIndexOffset < currentOffset)\
    {\
        cnfBuff[cellIndexOffset] = cellIndexValue;\
    }\
    else \
    {\
        cnfBuff[currentOffset++] = cellIndexValue;\
    }\
}

/* CA changes start */

#ifndef UE_SIM_TESTING
#define MAC_API_HEADER_LEN         INTERFACE_API_HEADER_LEN 
#else
#define MAC_API_HEADER_LEN     10
#endif

#define OAM_MAC_API_HEADER_LEN     MAC_API_HEADER_LEN
#define PHY_API_HEADER_LEN         MAC_API_HEADER_LEN
#define RRC_MAC_API_HEADER_LEN MAC_API_HEADER_LEN

/* CA changes end */
/* SPR 10100 changes start */
   #define MAC_RRC_SOCKET_FLAG 2 
/* SPR 10100 changes end */
/* for MAX API range */
#define RRC_MAX_REQ_API        100
#define RRC_MAX_CNF_API	       200
#define RRC_MAX_IND_API        300
#define MAC_INTERNAL_ERROR      11
#define TAG_SIZE 2
#define LENGTH_SIZE 2
#define UL_LC_RESP_LENGTH 4
#define DL_LC_RESP_LENGTH 4
#define CRNTI_TAG_LENGTH 6
#define MIN_LC_CREATE_LENGTH 9
#define MAC_DL_THROUGHPUT_LENGTH 4
#define MAC_UL_THROUGHPUT_LENGTH 4
#define MAC_DL_LOG_CH_STATUS_LENGTH 10
#define MAC_UL_LOG_CH_STATUS_LENGTH 10
#define GET_PER_UE_STATUS_LENGTH 6


/* valid ranges */
#define MAC_MIN_RESOURCE_BLOCKS 6
#define MAC_MIN_RANGE_VALUE 1
#define MAC_MIN_UEINDEX 0
#define MAC_MAX_UEINDEX MAX_UE_SUPPORTED
#define MAC_MIN_CRNTI 1
#define MAC_MAX_CRNTI 65523
#define MAC_MIN_UEPRIORITY 0
#define MAC_MAX_UEPRIORITY 3
/* CA TDD Changes Start */
#define MAC_MIN_NUM_HARQ 1 
/* CA TDD Changes Start */
#define MAC_MAX_NUM_HARQ 8
#define MAC_MIN_MAXRB 1
#define MAC_MAX_MAXRB 100
#define MAC_HSCHEME_VALUE 0
#define MAC_MIN_TRANSMISSION_MODE 1
/* Rel9_upgrade_CR396 */
#define MAC_MAX_TRANSMISSION_MODE 8
/* Rel9_upgrade_CR396 */
#define MAC_MIN_LCHID 0
#define MAC_MAX_LCHID 10
#define MAC_MIN_LCGID 0
#define MAC_MAX_LCGID 3
#define MAC_MIN_LCHPRIORITY 1
#define MAC_MAX_LCHPRIORITY 16
#define MAC_MIN_SRPUCCHRESOURCEINDEX 0
#define MAC_MAX_SRPUCCHRESOURCEINDEX 2047
#define MAC_MIN_SRCONFIGURATIONINDEX 0
/* Rel9_upgrade_CR257 */
#define MAC_MAX_SRCONFIGURATIONINDEX 157
/* Rel9_upgrade_CR257 */
/* Rel9_upgrade_CR410 */
#define MAC_MAX_SR_PROHIBIT_TIMER_VAL 7
/* Rel9_upgrade_CR410 */
#define MAC_MIN_CQI_PUCCH_RESOURCE_INDEX 0
#define MAC_MAX_CQI_PUCCH_RESOURCE_INDEX 1185
#define MAC_MIN_CQI_PMI_CONFIG_INDEX 0
#define MAC_MAX_CQI_PMI_CONFIG_INDEX 541
/* SPR 1583 Start */
#define MAC_MAX_CQI_PMI_CONFIG_INDEX_TDD 315
/* SPR 1583 End */
#define MAC_MAX_CQI_PMI_CONFIG_INDEX_3_1_7 317 
#define MAC_MIN_RI_CONFIG_INDEX 0
#define MAC_MAX_RI_CONFIG_INDEX 965 
#define MAC_MIN_CODINGRATE 0
#define MAC_MAX_CODINGRATE 300
#define MAC_MIN_PAGING_FRAME_NUM 0
#define MAC_MAX_PAGING_FRAME_NUM 1023
#define MAC_MIN_PAGINGOCCASSION 0
#define MAC_MAX_PAGINGOCCASSION 9
/* Review comment fix start RJ36 */
#define MAC_INVALID_PRACH_CONFIGURATION_INDEX_30  30
#define MAC_INVALID_PRACH_CONFIGURATION_INDEX_46  46
#define MAC_INVALID_PRACH_CONFIGURATION_INDEX_60  60
#define MAC_INVALID_PRACH_CONFIGURATION_INDEX_61  61
#define MAC_INVALID_PRACH_CONFIGURATION_INDEX_62  62
/* Review comment fix end RJ36 */
#define MAC_MAX_PRACH_CONFIGURATION_INDEX 63
#define MAC_MAX_PRACH_FREQUENCY_OFFSET 94
/* Rel 5.3: Coverity 32364,32363,32362 Fix Start */
/* Rel 5.3: Coverity 32364,32363,32362 Fix End */
#define QAM16 4
#define QAM64 6
#define ZERO 0
#define ONE 1
#define MAX_RLC_MODE 2
#define MAX_LC_TYPE 3
#define MIN_NUM_OF_LAYER 1
#define MAX_NUM_OF_LAYER 2
/*4x4 MIMO CHG START*/
#define MAX_NUM_OF_LAYER_4x4 4
/*4x4 MIMO CHG END*/
#define MIN_PRE_CODING_INDEX 0 
#define MAX_PRE_CODING_INDEX_ANT_2 3
/* 4x2 MIMO S */
#define MAX_PRE_CODING_INDEX_ANT_4 15
/* 4x2 MIMO E */
#define MAC_MAX_NRB_CQI 98
#define MAC_MAX_NCS_AN 7
#define MAC_MAX_N1_PUCCH_AN 2047
#define SERVICE_TYPE_1 1
#define SERVICE_TYPE_2 2
#define SERVICE_TYPE_3 3
#define RANK_INDICATOR_1 1 
#define RANK_INDICATOR_2 2
/*4x4 MIMO CHG START*/
#define RANK_INDICATOR_3 3      
#define RANK_INDICATOR_4 4
/*4x4 MIMO CHG END*/
#define LC_TYPE_1 1
#define LC_TYPE_2 2
#define TRANSPORT_CH_TYPE_0 0
#define TRANSPORT_CH_TYPE_1 1
#define TRANSPORT_CH_TYPE_2 2
#define SIMULTANEOUS_ACK_NACK_CQI_0 0
#define SIMULTANEOUS_ACK_NACK_CQI_1 1
/*L2_FIX_13_start*/
#define PERIODIC_WIDEBAND 0
#define PERIODIC_SUBBAND 1
/*L2_FIX_13_end*/

#define REQUEST_TYPE_0 0
#define REQUEST_TYPE_1 1



#define TTIB_DISABLED 0
#define TTIB_ENABLED 1



#define CQI_INDICATION_0 0
#define CQI_INDICATION_1 1
#define MAX_PREAMBLE_INIT_RX_TARGET_POW 15
#define RA_RESPONSE_WINDOW_SIZE_1 1
#define RA_RESPONSE_WINDOW_SIZE_11 11
#define RA_RESPONSE_WINDOW_SIZE_9 9
#define CONTENTION_RESOLUTION_TIMER_MIN 8
#define MIN_RES_BLOCKS 1
#define MAX_RES_BLOCKS 100
#define MIN_HARQ_TRANSMISSION 1
#define MAX_HARQ_TRANSMISSION 8
/* ULA_CHG */
#define MAX_UL_HARQ_TX_VAL_IDX 14
/* ULA_CHG */
#define MAC_TDD 0
#define MAC_FDD 1
/*HD FDD Changes Start*/
#define MAC_HD_FDD 2
/*HD FDD Changes End*/
#define MIN_RA_RNTI_RANGE 0
#define MAX_RA_RNTI_RANGE 60
#define NUM_OF_TX_ANTENNAS_1 1
#define NUM_OF_TX_ANTENNAS_2 2
/* + TM7_8 Changes Start */
#define NUM_OF_TX_ANTENNAS_3 3
/* - TM7_8 Changes End */
/*4x2 MIMO CR*/
#define NUM_OF_TX_ANTENNAS_4 4
/*4x2 MIMO E*/
#define START_RA_RNTI_RANGE_0 0
#define START_RA_RNTI_RANGE_1 1
#define END_RA_RNTI_RANGE_3 3
#define END_RA_RNTI_RANGE_9 9
#define PHICH_DURATION_0 0
#define PHICH_DURATION_1 1
#define MAX_QPSK_CODING_RATE_INDEX 10
#define MAX_SIXTEEN_QAM_CODING_RATE_INDEX 7
#define MAX_SIXTY_FOUR_QAM_CODING_RATE_INDEX 12
#define MAC_MAX_SUBBAND_CQI_K 4
#define MAC_MIN_SUBBAND_CQI_K 1
#define RI_CONFIG_INDEX_LEN 6
#define PHY_RECONFIG_MIN_LEN 6
#define PMI_INDEX_16 16
#define PMI_INDEX_17 17
#define PMI_INDEX_33 33
#define PMI_INDEX_34 34
#define PMI_INDEX_50 50
#define RI_LAYER_3   3
#define RI_LAYER_4   4
/* Range related to TDD Mode */
#ifdef TDD_CONFIG
    #define TDD_CONFIG_LEN 4
    #define MAC_TDD_MAX_SUBFRAME_ASSIGN 6
    #define MAC_TDD_SPECIAL_SUBFRAME_4   4
    #define MAC_TDD_MAX_SPECIAL_SUBFRAME 8    
    #define MAC_MAX_PRACH_CONFIGURATION_INDEX_FOR_TDD 57
#endif

/*Invalid range */
#define INVALID_RB 0
#define INVALID_TRANS_MODE 0
#define INVLAID_NUM_OF_LAYER 0
#define INVALID_CODE_BOOK_INDEX 100
/* CA changes Start */
#define INVALID_PERIODIC_FORMAT 5
#define INVALID_8_VAL 0xFF
#define INVALID_16_VAL 10000
/* CA changes End */

/* Maximum msg length for every API */
#define TAG_LEN 4
/* ULA_CHG */
/* CONFIG_CELL_REQ_MIN_LEN 61 + 6 for TDD, 69 + 6 for FDD */
/* + 1 Added for additional parameter enableFrequencySelectiveSchedulingUL */
/* in MAC_CONFIG_CELL_REQ */
#ifdef TDD_CONFIG
 /*SPR 3600 Start*/
#define CONFIG_CELL_REQ_MIN_LEN 75 
#else
#define CONFIG_CELL_REQ_MIN_LEN 83 
 /*SPR 3600 End*/
#endif
#define RACH_CONFIG_LEN 11 
#define RACH_CONFIG_WITH_PREAMBLE_GROUPA_CONFIG_LEN 18 
#define PREAMBLE_GROUP_A_CONFIG_LEN 7 
#define PHICH_CONFIG_LEN 2
#define PUCCH_CONFIG_LEN 5
#define MAC_MIB_MSG_LEN 8
#define MAC_SIBTYPE1_MSG_LEN 8
#define MAC_SI_MSG_LEN 15
#define MAC_RESBLOCK_CONFIG_LEN 10
#define RECONFIG_CELL_MIN_LEN 3

#define PREAMBLE_GROUPA_MSG_SIZE_B56  56
#define PREAMBLE_GROUPA_MSG_SIZE_B144 144
#define PREAMBLE_GROUPA_MSG_SIZE_B208 208
#define PREAMBLE_GROUPA_MSG_SIZE_B256 256
 
/*Fix for __SPR_1988__ START*/
#define CREATE_UE_ENTITY_MIN_LEN 21 
/*Fix for __SPR_1988__ END*/
 
#define SR_CONFIG_LEN 5
#define MAC_SR_SETUP_INFO_LEN 7
#define CQI_INFO_LEN 6
#define CQI_PMI_CONFIG_INDEX_INFO_LEN 10
#define CREATE_LC_REQ_MIN_LEN 6 
#define CREATE_LC_REQ_MIN_TAGLEN 11 
#define RECONFIG_LC_REQ_MIN_LEN 5
#if defined(QOS_SI_FWK_TEST) || defined(UTFWK_SIMULATION)
#define UL_LC_CREATE_REQ_LEN 6
/* SPR 3608 changes start */
#define UL_LC_CREATE_REQ_OPT_LEN 11
/* SPR 3608 changes end */
#else
#define UL_LC_CREATE_REQ_LEN 5
/* SPR 3608 changes start */
#define UL_LC_CREATE_REQ_OPT_LEN 10
/* SPR 3608 changes end */
#endif
 /*SPR 2726 Start*/
#define DL_LC_CREATE_REQ_LEN 6
/* SPR 3608 changes start */
#define DL_LC_CREATE_REQ_OPT_LEN (11 + CA_LC_BANDWIDTH_DIST_INFO_TAG_LEN)
/* SPR 3608 changes end */
 /*SPR 2726 End*/
#define UL_LC_RECONFIG_REQ_LEN 4
#define DL_LC_RECONFIG_REQ_LEN 4
#define MAC_POWER_HEADROOM_IND_LEN 5
/* Cell Start Stop Chg */
#define MAC_SFN_INFO_LEN 6
/* Cell Start Stop Chg */
/* SIB8_Code Start */
#define MAC_SF_INFO_LEN 5
/* SIB8_Code_fix2 Start */
#define MAC_TIMESTAMP_INFO_LEN 16 
/* SIB8_Code_fix2 End */
#define MAC_TIMESTAMP_INFO_RQST_TAG_LEN 5 
/* SIB8_Code End */
#define RECONFIG_UE_ENTITY_MIN_LEN 2
#define RECONFIG_UE_INFO_MIN_LEN 4
#define MAC_RECONFIGURE_LC_REQ_LEN 2
#define DELETE_LC_REQ_LEN 6
#define DELETE_UE_ENTITY_REQ_LEN 2
#define MAC_TAG_HEADER_LEN 2
#define CCCH_MSG_LEN 3
#define PCCH_MSG_MIN_LEN 6
#define BCCH_MSG_REQ_MIN_LEN 29
#define UE_DRX_CMD_REQ_LEN 2
#define UE_CON_REJ_REQ_LEN 3
#define DL_MAX_RB_INFO_LEN 5
#define UL_MAX_RB_INFO_LEN 5
#define TX_MODE_INFO_LEN 5
#define NUM_OF_LAYER_INFO_LEN 5
#define CODEBOOK_INDEX_INFO_LEN 5
#define SIMULTANEOUS_ACK_NACK_CQI_INFO_LEN 5
#define SR_SETUP_INFO_LEN 7
/*HD FDD Changes Start*/
/** lenghth of RF_PARAM tlv in Reconfigure ue Request **/
#define RF_PARAMS_LEN 5
/*HD FDD Changes End*/

/*Fix for __SPR_1988__ START*/ 
/* SPR 2063 start */
#define ADD_UE_MANDATORY_LENGTH 19
/* SPR 2063 end */
/*Fix for __SPR_1988__ END*/
 
#define CELL_CONFIG_ERROR_LEN 5
#define APERIODIC_PERIODIC_CQI_INFO_LEN 14 
/* SPR 1595 changes start */
#define PERIODIC_CQI_SETUP_LEN 15
#define PERIODIC_CQI_LEN 10
/*L2_FIX_4_start*/
#define CQI_FORMAT_PERIODIC_SUBBAND_K_LEN 5
/*L2_FIX_4_end*/

/* Rel9_upgrade_CR396 */
#define CQI_REPORT_V920_LEN 9
/* + Coverity_36248 */
#define CQI_MASK_V920_LEN   5 
#define PMI_RI_REPORT_V920_LEN   5
/* - Coverity_36248 */
#define SR_PROHIBIT_TIMER_TAG_LEN 5
/* Rel9_upgrade_CR396 */
 /* SPR 1595 changes end */
#define PUSCH_TAG_LEN 7
/*REL10_CR_CHANGES_START*/
#define PC_MAX_INFO_TAG_LEN 5
#define MAX_UL_HARQ_TX_INFO_TAG_LEN 5
/*REL10_CR_CHANGES_END*/
/* UE_CAPAB_ADD_PARAM Start */
#define UE_CAPABILITY_TAG_LENGTH 5

/*eicic */
#define CSI_SUBFRAME_PATTERN_CONFIG_VR10_TAG_LENGTH 5
#define CSI_MEASUREMENT_SUBSET_VR10_TAG_LENGTH 8
#define CSI_CONFIG_INDEX_VR10_TAG_LENGTH 5
#define CQI_PMI_CONFIGINDEX2_VR10_TAG_LENGTH 6
#define DL_INTERFERENCE_INFO_TAG_LENGTH 5
/*eicic*/

/* UE_CAPAB_ADD_PARAM End */
/* SPR 903 Start*/
#define UE_CATEGORY_TAG_LEN 5
#define MAC_MAX_UE_CATEGORY 5
#define MAC_MIN_UE_CATEGORY 1
/* SPR 903 End*/
/*CA changes start*/
#define MAC_MAX_UE_CATEGORY_V1020 8
#define MAC_MIN_UE_CATEGORY_V1020 6
#define PUCCH_FORMAT_V1020_MIN_LEN 9
#define PUCCH_CHANNEL_SELECTION_CONFIG_V1020_SETUP 1
#define PUCCH_CHANNEL_SELECTION_CONFIG_V1020_RELEASE 0
#define PUCCH_CHANNEL_SELECTION_CONFIG_V1020_MIN_LEN 5
#define PUCCH_AN_CS_LIST_V1020_MIN_LEN 4
#define PUCCH_AN_CS_V1020_LEN 6
#define MAX_N1PUCCH_RESOURCE_VAL 2047
#define MIN_CA_CONFIG_TAG_LEN 4
#define SCELL_DEACTIVATION_TIMER_INFO_TAG_LEN 5
/* + SPR 11784 changes */
#define SCELL_DEACTIVATION_TIMER_VALUE_COUNT 8
/* - SPR 11784 changes */
#define MIN_SCELL_INFO_TAG_LEN 7
#define MIN_SCELL_ID 0
#define MAX_SCELL_ID 7
#define MIN_SCELL_INDEX 1
#define MAX_SCELL_INDEX 7
#define OPERATION_TYPE_ADD 0
#define OPERATION_TYPE_MODIFY 1
#define OPERATION_TYPE_DELETE 2
#define MIMO_CAPABILITY_DL_V1020_TAG_LEN 5
#define NUM_OF_LAYERS_COUNT 3 
#define CODEBOOK_SUBSET_RESTRICTION_V1020_TAG_LEN 19
/* SPR 12240 Changes Start */
#define CODEBOOK_SUBSET_RESTRICTION_V920_TAG_LEN 9
#define CBSR_TYPE_MAX_VAL_V920 2
#define MAX_CBSR_VALUE_COUNT_V920 4
/* SPR 12240 Changes End */
#define CBSR_TYPE_MAX_VAL 9
#define MAX_CBSR_VALUE_COUNT 14
#define APERIODIC_CSI_TRIGGER_R10_TAG_LEN 6
/* CA TDD Changes Start */
#define MIN_DL_NUM_HARQ_PROC_VAL 1
/* CA TDD Changes End */
#define MAX_DL_NUM_HARQ_PROC_VAL 8
#define DL_NUM_HARQ_PROC_TAG_LEN 5
#define DL_INIT_MCS_TAG_LEN 5
#define MAX_DL_INIT_MCS_VAL 28
#define TRIGGER1R10_VAL 64
#define TRIGGER2R10_VAL 192
#define SCELL_CQI_MIN_LEN 4
#define SCELL_CQI_MAX_LEN 70
#define RESET_CA_UE_ENTITY_REQ_LEN 7
#define SCELL_ACTIVATE_STATUS_LEN 5
#define DL_NUM_RB 8
#define DL_SCELL_INFO 1
#define CA_LC_BANDWIDTH_DIST_INFO_TAG_LEN 12 
#define MAX_CA_LC_BANDWIDTH_DIST_VAL 100
#define MAX_CELLS_PER_UE 8
/*CA changes end*/

 /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
#define UPLINK_RAT1_INFO_TAG_LENGTH 5
#endif
 /* SPR 21958 PUSCH RAT1 Support End */
/* FREQ_SELECTIVE CHG */
#define FREQ_SELECTIVE_MIN_TAG_LEN 6
/* FREQ_SELECTIVE CHG */

#define TTI_BUNDLING_INFO_TAG_LEN 5

/* SPR 23402 Changes Start */
/* MOCN Changes Start */ 
#define OPERATOR_ID_INFO_TAG_LEN 5
/* MOCN Changes End */ 
/* SPR 23402 Changes End */

#define MAC_MAX_BETA_OFFSET_ACK_INDEX 15
#define MAC_MAX_BETA_OFFSET_RI_INDEX 15
#define MAC_MAX_BETA_OFFSET_CQI_INDEX 15

#define MAC_DEFAULT_BETA_OFFSET_ACK_INDEX 10
#define MAC_DEFAULT_BETA_OFFSET_RI_INDEX 12 
#define MAC_DEFAULT_BETA_OFFSET_CQI_INDEX 15

#define INVALID_APERIODIC_CQI_MODE 255
/* SPR 19309 + */
#define MANDATORY_LEN_OF_CELL_CONFIG 18+2
/* SPR 19309 - */

/* Length of message left after broadcast
 * message in cell configuration
 */
#ifdef TDD_CONFIG
    #define REM_LEN_AFTER_BROADCAST_MSG 13
#else
    #define REM_LEN_AFTER_BROADCAST_MSG 5
#endif

#define INVALID_SR_REQ_TYPE            255
#define INVALID_PERIODIC_CQI_REQ_TYPE  255

#define MAC_MAX_MOD_PERIOD 4096
#define MAC_SFN_GAP_1 1
#define MAC_SFN_GAP_2 2

/*Fix for __SPR_1988__ START*/
#define TIME_ALIGNMENT_TIMER_INFO_LEN 6
#define MAX_TIME_ALIGNMENT_TIMER_IDX  8
/*Fix for __SPR_1988__ END*/

/* SPR_1987 Start */
#define MAX_UL_HARQ_TX_VAL_IDX 14
/* SPR_1987 End */
 
/* SRS_CHG */
#define SRS_COMMON_CONFIG_MIN_TAG_LEN                   5
#define SRS_COMMON_CONFIG_MAX_TAG_LEN                   17
#define SRS_COMMON_SETUP_INFO_MIN_TAG_LEN               7
#define SRS_COMMON_SETUP_INFO_MAX_TAG_LEN               12
#define SRS_COMMON_CONFIG_MAX_BW_CONFIG                 7 
/*FDD_SRS*/
#ifdef TDD_CONFIG
#define SRS_COMMON_CONFIG_MAX_SUBFRAME_CONFIG           13
#elif FDD_CONFIG
#define SRS_COMMON_CONFIG_MAX_SUBFRAME_CONFIG           14
#endif
/*FDD_SRS*/

#define SRS_DEDICATED_CONFIG_MIN_TAG_LEN                5
#define SRS_DEDICATED_CONFIG_MAX_TAG_LEN                17
#define SRS_DEDICATED_CONFIG_TAG_LEN                    12   
#define SRS_DEDICATED_CONFIG_UPPTS_TAG_LEN              5
#define SRS_DEDICATED_CONFIG_MAX_BANDWIDTH              3
#define SRS_DEDICATED_CONFIG_MAX_HOPPING_BANDWIDTH      3
#define SRS_DEDICATED_CONFIG_MAX_FREQ_DOMAIN_POSITION   23 
#ifdef TDD_CONFIG
#define SRS_DEDICATED_CONFIG_MAX_SRS_CONFIG_INDEX       644
#elif FDD_CONFIG
#define SRS_DEDICATED_CONFIG_MAX_SRS_CONFIG_INDEX       636
#endif
/*FDD_SRS*/

#define SRS_DEDICATED_CONFIG_MAX_TRANSMISSION_COMB      1
#define SRS_DEDICATED_CONFIG_MAX_CYCLIC_SHIFT           7
/* SRS_CHG */
#define MAC_CHANGE_CRNTI_REQ_LEN 6
#define MAC_CHANGE_CRNTI_RESP_LEN 4
#define MAC_HO_RACH_RESOURCE_REQ_LEN 2
#define MAC_HO_RACH_RESOURCE_RESP_LEN 4
/*SPR 3607 Fix Start*/
#define INTRA_ENB_HO_REQUESTED  1
#define MAC_HO_RACH_RESOURCE_OPT_LEN 7
 /*SPR 3607 Fix End*/

#define CRNTI_INFO_LEN 6
#define PREAMBLE_INFO_LEN 6
/* SPR 19066 fix start */
/* SPR 19066 fix end */
#define TIME_ALIGNMENT_TIMER_INFO_LEN 6
#define MAX_TIME_ALIGNMENT_TIMER_IDX  8
#define UINT32_MAX_VALUE  ((UInt32)4294967295U)
/* SPR 15909 fix start */
#define UINT64_MAX_VALUE 0xFFFFFFFFFFFFFFFF 
/* SPR 15909 fix end */
/* +- SPR 19066 */
#define MAC_HO_REL_RACH_RESOURCE_IND_MIN_LEN 2
#define RESET_UE_ENTITY_REQ_LEN 2
#define INVALID_CRNTI 0
/* +- SPR 19066 */
#define MAC_HO_REL_RACH_RES_IND_CRNTI_TAG_LEN 6
#define MAC_REQ_LEN_2 2
/* +- SPR 19066 */
#define CONTENTION_FREE_RACH_TIMER_TAG_MIN_LEN 6
#define MAC_HO_RACH_RESOURCE_REQ_LEN 2
/* +- SPR 19066 */
#define CONTENTION_FREE_RACH_TIMER_REM_LEN 2
/* Cell Start Stop Chg */
#define EXPLICIT_CELL_START_REQUIRED_TAG_LEN 5
/* Cell Start Stop Chg */
#define CHANGE_CRNTI_REQ_MSG_ID 16
#define MAC_MIN_CFR_TIMER_VAL 25 
#define MAC_MAX_CFR_TIMER_VAL 200
/* +- SPR 19066 */
#define MAX_NB_VALUE   7
 /*SPR_3061_FIX*/
#define PUCCH_CQI_SINR_THRESHOLD_REM_LEN 1 
#define PUCCH_CQI_SINR_THRESHOLD_TAG_MIN_LEN 6
#define MAX_PUCCH_CQI_SINR_THRESHOLD 255

/*SPR_3827_FIX_START*/
#define PUCCH_CQI_SINR_THRESHOLD_MIN_LEN 5
/*SPR_3827_FIX_END*/

#define NGAP_TAG_LEN_VALUE 5 
#define NGAP_TAG_REM_LENGTH 1

#define UL_SYNC_LOSS_TIMER_INFO_MIN_LEN 6
#define UL_SYNC_LOSS_TIMER_INFO_REM_LEN 2
#define MAX_UL_SYNC_LOSS_TIMER 10240
 /*SPR_3061_FIX*/

/*SPR 2834 Fix Begins*/
#define QOS_INFO_MIN_LEN  6
/*SPR 2834 Fix Ends*/
#define QOS_QCI_MIN_VAL 1
#define QOS_QCI_MAX_VAL 9
 /*64 bit to 32 bit CR */
#define GBR_INFO_LEN  20       
 /*64 bit to 32 bit CR */

/* SPS_CHG */
#define U16BIT_LEN 2
#define SPS_RNTI_PARAM_LENGTH 8
#define SPS_DL_SCHEDULING_INFO_PER_TTI_LENGTH \
        7 + MAX_SPS_INTERVALS
#define SPS_UL_SCHEDULING_INFO_PER_TTI_LENGTH \
        7 + MAX_SPS_INTERVALS
#define MIN_SPS_RNTI_VALUE 0x003D
#define MAX_SPS_RNTI_VALUE 0xFFF3
#define INVALID_SPS_CRNTI  0
#define MAX_DL_SPS_DCI_PER_TTI_MIN_VALUE 1
#define MAX_DL_SPS_DCI_PER_TTI_MAX_VALUE 10
#define MAX_DL_SPS_OCCASION_PER_TTI_MIN_VALUE 1
#define MAX_DL_SPS_OCCASION_PER_TTI_MAX_VALUE 20
#define MAX_DL_SPS_RBS_PER_TTI_MIN_VALUE 1        
#define MAX_DL_SPS_RBS_PER_TTI_MAX_VALUE 100        
#define MAX_DL_SPS_RBS_PER_TTI_PER_INTV_MIN_VALUE 1        
#define MAX_DL_SPS_RBS_PER_TTI_PER_INTV_MAX_VALUE 100        
#define MAX_UL_SPS_DCI_PER_TTI_MIN_VALUE 1
#define MAX_UL_SPS_DCI_PER_TTI_MAX_VALUE 10
#define MAX_UL_SPS_OCCASION_PER_TTI_MIN_VALUE 1
#define MAX_UL_SPS_OCCASION_PER_TTI_MAX_VALUE 20
#define MAX_UL_SPS_RBS_PER_TTI_MIN_VALUE 1        
#define MAX_UL_SPS_RBS_PER_TTI_MAX_VALUE 100        
#define MAX_UL_SPS_RBS_PER_TTI_PER_INTV_MIN_VALUE 1        
#define MAX_UL_SPS_RBS_PER_TTI_PER_INTV_MAX_VALUE 100       
#define MIN_NUMBER_OF_CONF_SPS_PROCESS 1
#define MAX_NUMBER_OF_CONF_SPS_PROCESS 8
#define MAX_SPS_HARQ_RETX_MIN_VALUE 1
#define MAX_SPS_HARQ_RETX_MAX_VALUE 8
#define MAX_SPS_HARQ_RETX_DEFAULT_VALUE 6
#define EXPLICIT_RELEASE_AFTER_MIN_VALUE 2
#define EXPLICIT_RELEASE_AFTER_MAX_VALUE 8
#define EXPLICIT_RELEASE_AFTER_DEFAULT_VALUE 3
#define N1_PUCCH_AN_PERSISTENT_MAX_VALUE 2047
#define MIN_NUM_OF_PUCCH_AN_PERSISTENT 1  
#define MAX_NUM_OF_PUCCH_AN_PERSISTENT 4 
#define P0_NOMINAL_PUSCH_PERSISTENT_MIN_VALUE -126
#define P0_NOMINAL_PUSCH_PERSISTENT_MAX_VALUE 24
#define P0_UE_PUSCH_PERSISTENT_MIN_VALUE -8 
#define P0_UE_PUSCH_PERSISTENT_MAX_VALUE 7
#define SPS_CONFIG_TAG_MIN_VALUE 4
#define SPS_CONFIG_TAG_MAX_VALUE 71
#define SPS_DL_INFO_TAG_MIN_VALUE 5
#define SPS_DL_INFO_TAG_MAX_VALUE 45
#define SPS_DL_SETUP_INFO_TAG_MIN_VALUE 12
#define SPS_DL_SETUP_INFO_TAG_MAX_VALUE 40
#define SPS_N1_PUCCH_AN_PERSIST_LIST_TAG_VALUE 6
#define SPS_UL_INFO_TAG_MIN_VALUE 5
#define SPS_UL_INFO_TAG_MAX_VALUE 22
#define SPS_UL_SETUP_INFO_TAG_MIN_VALUE 6
#define SPS_UL_SETUP_INFO_TAG_MAX_VALUE 17
#define SPS_UL_P_ZERO_PERSISTENT_TAG_LEN_VALUE 6
#define SPS_UL_TWO_INTERVALS_CONFIG_TAG_LEN_VALUE 5
#define SPS_CRNTI_INFO_TAG_LENGTH           6
#define ROHC_PROFILE_TAG_LENGTH             6
#define ROHC_PROFILE_MIN_VALUE              0
#define ROHC_PROFILE_MAX_VALUE              7
#define RECONFIG_SPS_DL_SCH_INFO_PER_TTI    0x01
#define RECONFIG_SPS_UL_SCH_INFO_PER_TTI    0x02
#define SPS_DL_MAX_HARQ_RETX_TAG_LEN 5
#define SPS_DL_EXPLICIT_RELEASE_AFTER_TAG_LEN 5
#define MAC_RECONFIG_COMPLETE_IND_MSG_LEN     3

/* SPS_CHG */

/*Added for Power Control*/
#define MIN_TPC_RNTI_VALUE 1
#define MIN_TPC_PDCCH_RNTI_VALUE 0x0001
#define MAX_TPC_RNTI_VALUE 0xFFF3
#define TPC_RNTI_PARAM_LENGTH 8
#define PDSCH_CONFIG_COMMON_INFO_PARAM_LENGTH 2
/*SPR 3600 Start*/
#define PUSCH_CONFIG_COMMON_PARAM_LENGTH 1
#define PUSCH_CONFIG_COMMON_TAG_LEN 5
/*SPR 3600 End*/
 
/*CLPC_REWORK_CHG*/
#define MIN_REFERENCE_SIGNAL_POWER_VALUE -60
#define MAX_REFERENCE_SIGNAL_POWER_VALUE 50
/*CLPC_REWORK_CHG*/
#define MIN_PB_VALUE 0
#define MAX_PB_VALUE 3
#define DOWNLINK_POWER_CONTROL_COMMON_PARAM_LENGTH 10
#define MIN_TRANS_POWER_OFFSET 0
#define MAX_TRANS_POWER_OFFSET 10000
#define UPLINK_POWER_CONTROL_COMMON_PARAM_LENGTH 2
#define UPLINK_POWER_CONTROL_COMMON_TAG_MIN_LEN 6
#define MIN_P0_NOMINAL_PUSCH_VALUE -126
#define MAX_P0_NOMINAL_PUSCH_VALUE 24
#define MIN_ALPHA_VALUE 0
#define MAX_ALPHA_VALUE 7
#define MIN_CLOSED_LOOP_POWER_CONTROL_ENABLE_PARAM_LENGTH 7
#define MAX_CLOSED_LOOP_POWER_CONTROL_ENABLE_PARAM_LENGTH 8
#define UPLINK_POWER_CONTROL_DEDICATED_PARAM_LENGTH 8
#define MIN_P0_UE_PUSCH_VALUE -8
#define MAX_P0_UE_PUSCH_VALUE 7
#define MIN_TPC_PDCCH_CONFIG_PARAM_LENGTH 1
#define MAX_TPC_PDCCH_CONFIG_PARAM_LENGTH 9
#define MIN_TPC_INDEX_VALUE_DCI3    1
#define MAX_TPC_INDEX_VALUE_DCI3    15
#define MIN_TPC_INDEX_VALUE_DCI3A   1
#define MAX_TPC_INDEX_VALUE_DCI3A   31
/*CLPC_REWORK_CHG*/
#define MIN_PA_VALUE 0
#define MAX_PA_VALUE 7
/*CLPC_REWORK_CHG*/
#define TPC_RAR_NO_CHANGE_COMMAND 1
#define POWER_CONTROL_ENABLE_PARAM_LENGTH 5
#define MIN_PC_MAX -40
#define MAX_PC_MAX 23
#define PDSCH_CONFIG_COMMON_INFO_TAG_MIN_LEN 6
#define DOWNLINK_POWER_CONTROL_COMMON_TAG_MIN_LEN 14
#define TPC_RNTI_INFO_PARAM_LENGTH 4
#define PDSCH_CONFIG_INFO_PARAM_LENGTH 1
#define ULP_CONTROL_DEDICATED_PARAM_LEN 3
#define P0_UE_PUSCH_LEN 5
#define TPC_PDCCH_CONFIG_LENGTH 5
/* offset in the TPC Bitmap also refferd as TPC Index in specs*/
#define INVALID_TPC_OFFSET    0 
/*DCI Format FIX*/
#define MAC_RRC_DCI_FORMAT_3 0
#define MAC_RRC_DCI_FORMAT_3A 1
/*DCI Format FIX*/
#define RI_CONFIG_INDEX_INFO_LEN 6

#define SI_MSG_DCI_FORMAT_1A 0
#define SI_MSG_DCI_FORMAT_1C 1

#define DCI_1A_CH_RB_MAP_LEN 27
#define DCI_1C_CH_RB_MAP_LEN 32
 /* ICIC changes start */
#define USER_LOCATION_TYPE_LEN 1
 /* ICIC changes end */

/*UL_MU_MIMO_CHG_START*/
#define UL_MU_MIMO_TYPE_LEN 1
 /*UL_MU_MIMO_CHG_END*/

/* SPR 4870 fix start*/
#define DELETE_LC_STATUS_TAG_LEN 6
#define MAC_ACTIVE_LC 1
#define MAC_INACTIVE_LC 2
/* SPR 4870 fix end*/
/*Dynamic Pdcch Changes Start*/
#define TOT_RES_BIT_MAP_COMB_FOR_5_10_15_20_MHZ 8
#define TOT_RES_BIT_MAP_COMB_FOR_3MHZ 15
#define DYNAMIC_PDCCH_INFO_REM_LEN 1
#define DYNAMIC_PDCCH_INFO_TAG_MIN_LEN 5
/*Dynamic Pdcch Changes End*/
/* +DYNAMIC_ICIC*/
#define PHR_TIMER_CONFIG_LEN 1
#define PHR_TIMER_INFO_LEN 2
#define MAX_PHR_TIMER 7
/* -DYNAMIC_ICIC*/
/*SPR 16855 Fix Start*/
#define MAX_CBSR_TYPE_VALUE   8
#define MAX_NUM_OF_TLV        20
/* CA changes start */
#define MAX_NUM_OF_CA_TLV        3      /* MAX Number of TLVs for CA_CONFIG */
#define MAX_NUM_OF_SCELL_TLV        12  /* MAX Number of TLVs for SCELL_INFO */
#define MIN_CQI_APERIODIC_LEN 5
/* CA changes End */
#define UL_RECONFIG_SR_IND                           1
#define UL_RECONFIG_CQI_IND                          2
#define UL_RECONFIG_CQI_PERIODIC_IND                 1
#define UL_RECONFIG_CQI_APERIODIC_IND                2
#define UL_RECONFIG_CQI_PERIODIC_APERIODIC_IND       3
#define UL_RECONFIG_CQI_PERIODIC_SUBBAND_IND         1
#define UL_RECONFIG_CQI_PERIODIC_RI_IND              2
#define MAC_MAX_API                                  17
#define UL_RECONFIG_CLEAR_CQI_PERIODIC_IND           0xFFFFFFFE
#define UL_RECONFIG_CLEAR_CQI_APERIODIC_IND          0xFFFFFFFD
#define UL_RECONFIG_CLEAR_SR_IND                     0xFFFFFFFE
#define UL_RECONFIG_CLEAR_CQI_IND                    0xFFFFFFFD
#define INVALID_SRS_REQ_TYPE                         255
#define CQI_MAP_RECONFIGURE_IND                      1
#define CQI_RESOURCE_INDEX_RECONFIGURE_IND           2
#define NO_CQI_RECONFIGURE_IND                       4
#define RI_MAP_RECONFIGURE_IND                       8
#define RI_RESOURCE_INDEX_RECONFIGURE_IND            16
#define NO_RI_RECONFIGURE_IND                        32
#define CQI_DEFAULT_PARAMETERS_IND                   64
#define CQI_MAP_RECONFIGURE_IND2                      128
#define CQI_RESOURCE_INDEX_RECONFIGURE_IND2           256
#define NO_CQI_RECONFIGURE_IND2                       512
#define RI_MAP_RECONFIGURE_IND2                       1024
#define RI_RESOURCE_INDEX_RECONFIGURE_IND2            2048
#define NO_RI_RECONFIGURE_IND2                        4096
/*SPR 16855 Fix End*/

/* SPR 5713 changes start */
#define RELEASE_8                                    0
#define RELEASE_9                                    1
#define RELEASE_10                                   2
#define INVALID_RELEASE                              255
#define UE_COMPLIANCE_RELEASE_TAG_LEN                5
#define ULGRANT_MIN_DATA_SIZE_FOR_REL_9              4
#define ULGRANT_MIN_DATA_SIZE_FOR_REL_10             7
/* SPR 5713 changes end */
/* + TM7_8 Changes Start */
#define PDSCH_EPRE_TO_UE_RS_RATIO_TAG_LEN            5                    
#define INVALID_PMI_RI_CONF_V920                     255
/* - TM7_8 Changes End */

/* + PRS_CHANGES */
#define PRS_SETUP                                    1
#define PRS_RELEASE                                  0
#define PRS_CONFIG_INFO_MIN_LEN                      5 
#define PRS_CONFIG_INFO_MAX_LEN                      34
#define PRS_CONFIG_SETUP_MIN_VALUE                   22
#define PRS_CONFIG_SETUP_MAX_VALUE                   29
#define PRS_CONFIG_RECONFIG_MIN_VALUE                5
#define PRS_CONFIG_RECONFIG_MAX_VALUE                29
#define PRS_BANDWIDTH_INFO_TAG_LEN                   5
#define PRS_SUBFRAMES_INFO_TAG_LEN                   5
#define PRS_CONFIG_INDEX_INFO_TAG_LEN                6
#define PRS_TRANSMISSION_POWER_INFO_TAG_LEN          6
#define PRS_MUTING_CONFIG_INFO_TAG_LEN               7
#define PRS_CONFIG_INDEX_MAX_VALUE                   2399
#define PRS_TRANSMISSION_POWER_MAX_VALUE             10000
#define PRS_SUBFRAMES_1                              1
#define PRS_SUBFRAMES_2                              2
#define PRS_SUBFRAMES_4                              4
#define PRS_SUBFRAMES_6                              6
#define PRS_MUTING_BITS_TO_BE_READ_2                 2
#define PRS_MUTING_BITS_TO_BE_READ_4                 4
 /* SPR_8112_FIX_START */
#define PRS_MUTING_BITS_TO_BE_READ_8                 8
 /* SPR_8112_FIX_END */
#define PRS_MUTING_BITS_TO_BE_READ_16                16


#define MAX_LOAD_DISPARITY_THRESHOLD                100
#define MAX_MCS_DIFF_THRESHOLD                      28
#define MAX_PRB_USAGE_WATER_MARK                    100
#define MIN_LOAD_CAL_WINDOW_SIZE                    100
#define MAX_LOAD_CAL_WINDOW_SIZE                    5000
#define MAX_DL_DEACT_COMMON_THREAD                  255
#define MAX_DL_DEACT_MCS_LOW_THRESH                 28
#define MAX_DL_QLOAD_UP_THRESH                      255
#define MAX_DL_QLOAD_DOWN_THRESH                    255
#define MAX_ACT_DEACT_TIG_COUNT_THRESH              255
#define MIN_CA_PARAMS_THRESHOLD                     0
/* CA changes end*/
#ifdef LTE_EMBMS_SUPPORTED
 #define MAX_RESERVED_CELL_POWER_OFFSET              3
#endif

/* SPR 22152 Fix Start */
#define INVALID_SCELL_OPERATION_TYPE   0xFF
/* SPR 22152 Fix End */

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* ports for send and receive */
extern UInt16 rxPortMAC_g;
extern UInt16 txPortMacOAM_g;
extern UInt16 txPortMacRRC_g;

 
/* MAC Init Ind */
extern UInt8 rlcInitInd_g;
extern UInt8 pdcpInitInd_g;

typedef enum CsiChoiceT
{
    FDD_CONFIG_TYPE=0,
    TDD_CONFIG1_5=1,
    TDD_CONFIG0=2,
    TDD_CONFIG6=3
}CsiChoice;

typedef enum RankIndicatorT
{
    TX_DIVERSITY=1,
    SPATIAL_MULTIPLEXING=2
}RankIndicator;

typedef enum RequestTypeT
{
    RELEASE=0, 
    SETUP=1,
    INVALID_REQUEST_TYPE
}RequestType;

typedef enum PhichResourceT
{
    PHICH_RESOURCE_0 = 0,
    PHICH_RESOURCE_1,
    PHICH_RESOURCE_2,
    PHICH_RESOURCE_3
}PhichResource;    
/* Coverity 36228 */
/* for RRC API Request */

typedef enum RRCReqAPIT 
{
	MAC_CONFIG_CELL_REQ = 1,
	MAC_SFN_REQ ,
	MAC_RECONFIG_CELL_REQ ,
	MAC_CREATE_UE_ENTITY_REQ ,
	MAC_DELETE_UE_ENTITY_REQ ,
	MAC_RECONFIGURE_UE_ENTITY_REQ ,
	MAC_UE_DRX_CMD_REQ ,
	MAC_BCCH_MSG_REQ ,
	MAC_PCCH_MSG_REQ ,
	MAC_CCCH_MSG_REQ ,
	MAC_UE_CON_REJ_REQ                 ,
	MAC_DELETE_CELL_REQ                , /* Cell Delete = 12*/
    MAC_HO_RACH_RESOURCE_REQ = 0x0D,
    /* +- SPR 19066 */
    MAC_RESET_UE_ENTITY_REQ         = 0x0F,
    MAC_CHANGE_CRNTI_REQ = 0x10,
    MAC_CELL_START_REQ              = 0xCF,
    MAC_CELL_STOP_REQ               = 0xD1,
/* SIB8_Code_fix Start */
/* SIB8_Code_fix Start */

/* for RRC API Response */
    MAC_CONFIG_CELL_CNF = RRC_MAX_REQ_API + 1,
    MAC_SFN_CNF ,
    MAC_RECONFIG_CELL_CNF ,
    MAC_CREATE_UE_ENTITY_CNF ,
    MAC_DELETE_UE_ENTITY_CNF ,
    MAC_RECONFIGURE_UE_ENTITY_CNF                            ,
    MAC_DELETE_CELL_CNF                                      , /* Reconfig Cell Delete Cnf = 107*/ 
    MAC_HO_RACH_RESOURCE_RESP = 0x6C,
    /* +- SPR 19066 */
    MAC_RESET_UE_ENTITY_CNF = 0x6E,
    MAC_CHANGE_CRNTI_CNF = 0x6F,
    MAC_CELL_START_CNF                  = 0xD0,
    MAC_CELL_STOP_CNF                   = 0xD2,
/* for RRC API Indication */

	MAC_UE_ENTITY_POWER_HEADROOM_IND = RRC_MAX_CNF_API + 1,
	MAC_SFN_ERR_IND ,
	MAC_CCCH_MSG_IND,
    MAC_HO_REL_RACH_RESOURCE_IND = 0xCC,
    MAC_RLF_IND = 0xCD,
    /* +- SPR 19066 */
    /*SPR_3065_FIX*/
    MAC_UE_SYNC_STATUS_IND = 0xD3,
    MAC_RECONFIG_COMPLETE_IND = 0xD4
    /*SPR_3065_FIX*/
#ifdef LTE_EMBMS_SUPPORTED
    , MAC_AREA_CONFIG_REQUEST = 0xD5,
    MAC_AREA_CONFIG_CNF = 0xD6,
    MAC_COUNTING_MSG = 0xD7,
    MAC_COUNTING_MSG_CNF = 0xD8,
    MAC_MCCH_IND = 0xD9,
    MAC_RESET_SESSION_REQ = 0xDA,
    MAC_RESET_SESSION_CNF = 0xDB
#endif

}RRCReqAPI;
typedef RRCReqAPI RRCIndAPI;
typedef RRCReqAPI RRCResAPI;

/* Coverity 36228 */

/* for Tags */

typedef enum RRCTagT 
{
    RACH_CONFIG_INFO    = 0x01,
    PHICH_CONFIG_INFO   ,
    MIB_MSG_INFO	    ,
    SIBTYPE1_MSG_INFO   ,
    SI_MSG_INFO	    ,
    SI_MSG		    ,
    CELL_CONFIG_ERROR   ,
    SFN_INFO	    ,
    ADD_UE_INFO	    ,
    CREATE_LC_INFO	    ,
    SR_CONFIG_INFO	    ,
    CQI_INFO	    ,
    UL_LC_CREATE_REQ    ,
    DL_LC_CREATE_REQ    ,
    UL_LC_RECONFIGURE_REQ         ,
    DL_LC_RECONFIGURE_REQ         ,
    SR_SETUP_INFO       ,
    CREATE_LC_ERROR	    ,
    UL_LC_CONFIG_RESP   ,
    DL_LC_CONFIG_RESP   ,
    RECONFIG_UE_INFO    ,
    RECONFIGURE_LC_REQ  ,
    CREATE_LC_REQ       ,
    DELETE_LC_REQ       ,
    DL_MAX_RB_INFO	    ,
    UL_MAX_RB_INFO	    ,
    TX_MODE_INFO        ,
    NUM_OF_LAYER_INFO   ,
    CODEBOOK_INDEX_INFO ,
    SIMULTANEOUS_ACK_NACK_CQI_INFO ,
    CQI_PMI_CONFIG_INDEX_INFO ,
    RECONFIGURE_LC_ERROR   , 
    DELETE_LC_ERROR	,
    PUCCH_CONFIG_INFO   ,
    SI_MSG_INFO_PARAM             ,
    PUSCH_CONFIG_INFO             ,
    TDD_CONFIG_INFO = 0x25,
    TDD_ACK_NACK_FEEDBACK_MODE_INFO,
    CQI_FORMAT_PERIODIC_SUBBAND_K = 39,  /* 39 */
    CQI_APERIODIC_INFO    = 0x32,
    CQI_PERIODIC_INFO     = 0x33,
    RI_CONFIG_INDEX_INFO  = 0x34,
    MOD_PERIOD_INFO  = 0x35,
    SFN_GAP_INFO  = 0x36,
    CRNTI_INFO = 0x37,
    PREAMBLE_INFO = 0x38,
    /* +- SPR 19066 */
    UE_CATEGORY = 0x40,
    /* SRS_CHG */    
    SRS_COMMON_CONFIG = 0x41,
    SRS_COMMON_SETUP_INFO = 0x42,
#ifdef TDD_CONFIG
    SRS_MAX_UPPTS = 0x43,
#endif     
    SRS_DEDICATED_CONFIG = 0x44,
    SRS_DEDICATED_SETUP_INFO = 0x45,
    /* SRS_CHG */    
    /* + CL-MIMO LJA*/
    CODEBOOK_SUBSET_RESTRICTION = 0x46,
    /* - CL-MIMO LJA*/

    /*+ DRX_CHG */
    DRX_CONFIG_INFO       = 0x47,
    DRX_SETUP_CONFIG_INFO = 0x48,
    SHORT_DRX_CONFIG      = 0x49,
    /*- DRX_CHG */
    /* + MEAS_GAP_CHG */
    MEAS_GAP_CONFIG_INFO       = 0x50,
    MEAS_GAP_SETUP_CONFIG_INFO = 0x51,
    /* - MEAS_GAP_CHG */
    CQI_MASK_INFO              = 0x52,
   /* +- SPR 19066 */  
    /*Fix for __SPR_1988__ START*/
    TIME_ALIGNMENT_TIMER_INFO = 0x54,
    /*Fix for __SPR_1988__ END*/
    CONTENTION_FREE_RACH_TIMER_INFO= 0x55,
    FREQUENCY_SELECTIVE_ENABLE = 0x56,
    /* +- SPR 19066 */
    PREAMBLE_GROUP_A_CONFIG       = 0x62,    
    EXPLICIT_CELL_START_REQUIRED  = 0x6f,
    AMBR_QOS_INFO = 0x57,
    QOS_INFO = 0x58,
    GBR_QOS_INFO = 0x5A,
    /* Rel9_upgrade_CR396 */
    SI_MSG_BUFFER = 0x5B,
    CQI_REPORT_CONFIG_V920 = 0x5C,
    CQI_MASK_V920 = 0x5D,
    PMI_RI_REPORT_V920 = 0x5E,
    CODEBOOK_SUBSET_RESTRICTION_V920 = 0x5F,
    /* Rel9_upgrade_CR396 */
    /* Rel9_upgrade_CR410 */
    SR_PROHIBIT_TIMER_V920 = 0x6D,
    NUM_SI_REPETITIONS = 0x60,
    /* Rel9_upgrade_CR410 */
    /*Added for Power Control CLPC_CHG */
    TPC_RNTI_RANGE = 0x63,
    PDSCH_CONFIG_COMMON_INFO = 0x64,
    DOWNLINK_POWER_CONTROL_COMMON = 0x65,
    UPLINK_POWER_CONTROL_COMMON = 0x66,
    POWER_CONTROL_ENABLE = 0x67,
    UPLINK_POWER_CONTROL_DEDICATED = 0x68,
    TPC_PDCCH_CONFIG_PUCCH = 0x69,
    PDSCH_CONFIG_INFO = 0x6A,
    P0_UE_PUSCH = 0x6B,
    TPC_RNTI_INFO = 0x6C,
    TPC_PDCCH_CONFIG_PUSCH = 0x6E,

    /*SPR_3063_FIX*/
    NUM_SI_TRANSMISSIONS = 0x70,
    SI_START_OFFSET = 0x71,
    COMMON_CH_RB_MAP_INFO = 0x72,
    /*SPR_3063_FIX*/
    /*SPR_3061_FIX*/
    UL_SYNC_LOSS_TIMER_INFO = 0x73,
    PUCCH_CQI_SINR_THRESHOLD_INFO = 0x74,
    /*SPR_3061_FIX*/
    RBS_FOR_DCI_1A = 0x81,
    RBS_FOR_DCI_1C = 0x82,
    DCI_FORMAT_FOR_SI_MSG = 0x83,
    /*SPR 3600 Start*/
    PUSCH_CONFIG_COMMON = 0x84,
    /*SPR 3600 End*/

    /*SPR 3608 changes Start*/
    UE_PSR_REQ = 0x85,
    ENB_PSR_REQ = 0x86,
    INTRA_ENB_HO_REQ = 0x87,
    /* ICIC changes start */
    USER_LOCATION_TYPE = 0x88,
    /* ICIC changes end */
    NGAP_VALUE = 0x89,
    SPS_CRNTI_RANGE = 0x8A,
    /*SPR 3608 changes End*/
    /* UE_CAPAB_ADD_PARAM Start */
    UE_CAPABILITY_ADDITIONAL_PARAMETERS = 0x90,
    /* UE_CAPAB_ADD_PARAM End*/
    /* SPR 4870 changes start */
    DELETE_LC_STATUS = 0x91,
    /* SPR 4870 changes end */
    /* SPS_CHG_START */
    SPS_DL_SCHEDULING_INFO_PER_TTI = 0x92,
    SPS_UL_SCHEDULING_INFO_PER_TTI = 0x93,
    SPS_CRNTI_INFO = 0x94,
    SPS_CONFIG = 0x95,
    SPS_DL_INFO = 0x96,
    /*SPR 6861 Fix start*/
    MAC_SPS_DL_SETUP_INFO = 0x97,
    /*SPR 6861 Fix end*/
    SPS_N1_PUCCH_AN_PERSIST_LIST = 0x98,
    SPS_UL_INFO = 0x99,
    /*SPR 6861 Fix start*/
    MAC_SPS_UL_SETUP_INFO = 0x9A,
    /*SPR 6861 Fix end*/
    P_ZERO_PERSISTENT = 0x9B,
    TWO_INTERVALS_CONFIG = 0x9C,
    ROHC_PROFILE = 0x9D,
    /*HD FDD Changes Start*/
    RF_PARAMS = 0x9E,
    /* UL_MU_MIMO_CHG_START*/
    UL_MU_MIMO_INFO = 0xA1,
    /* UL_MU_MIMO_CHG_END*/
    /*HD FDD Changes Start*/
    /* +DYNAMIC_ICIC */
    PHR_TIMER_CONFIG = 0x9F,
    MIN_POWER_UE = 0xA0,
    /* -DYNAMIC_ICIC */
    SPS_DL_MAX_HARQ_RETX = 0xA2,
    SPS_DL_EXPLICIT_RELEASE_AFTER = 0xA3,
    /* SPS_CHG_END */
    /*START : DRX_MAC_CE*/
    DRX_MAC_CE_CMD_TRIGGER_INFO = 0xA4,
    /*END   : DRX_MAC_CE*/
    /*Dynamic Pdcch Changes Start*/
    DYNAMIC_PDCCH_INFO = 0xA5,
    /*Dynamic Pdcch Changes End*/
    /* SPR 5713 changes start */
    UE_COMPLIANCE_RELEASE = 0xAC,
    /* SPR 5713 changes end */
    /* SIB8_Code Start */
    SF_INFO  = 0xA7,
    /* SIB8_Code_fix Start */
    TIMESTAMP_REQD_INFO = 0xA8,
    /* SIB8_Code_fix End */
    MAC_TIMESTAMP_INFO = 0xA9,
    SCHEDULING_REQD_FOR_SIB8_INFO = 0xAA,
    /* SIB8_Code End */
    PHR_TIMER_INFO = 0xAB,
    /* + TM7_8 Changes Start */
    PDSCH_EPRE_TO_UE_RS_RATIO = 0xAF,
    /* - TM7_8 Changes End */
    /* + PRS_CHANGES */ 
    PRS_CONFIG_INFO = 0xB0,
    PRS_BANDWIDTH_INFO = 0xB1,
    PRS_SUBFRAMES_INFO = 0xB2,
    PRS_CONFIG_INDEX_INFO = 0xB3,
    PRS_TRANSMISSION_POWER_INFO = 0xB4,
    PRS_MUTING_CONFIG_INFO = 0xB5,
    TTI_BUNDLING_INFO = 0xB6, 
	/* SPR 23402 Changes Start */    
	/* MOCN Changes Start */
	OPERATOR_ID_INFO = 0xB7,   
	/* MOCN Changes End */
	/* SPR 23402 Changes End */
    MAX_UL_HARQ_TX_INFO = 0xB8,
    PC_MAX_INFO = 0xB9,
        /*REL10_CR_CHANGES_END*/
    /*CA changes start*/
    UE_CATEGORY_V1020 = 0xBA,
    PUCCH_FORMAT_V1020 = 0xBB,
    PUCCH_CHANNEL_SELECTION_CONFIG_V1020 = 0xBC,
    PUCCH_AN_CS_LIST_V1020 = 0xBD,
    PUCCH_AN_CS_V1020 = 0xBE,
    CA_CONFIG = 0xBF,
    SCELL_DEACTIVATION_TIMER_INFO = 0xC0,
    SCELL_INFO = 0xC1,
    MIMO_CAPABILITY_DL_V1020 = 0xC2,
    CODEBOOK_SUBSET_RESTRICTION_V1020 = 0xC3,
    CQI_PMI_CONFIG_INDEX_INFO_v1020 = 0xC4,
    APERIODIC_CSI_TRIGGER_R10 = 0xC5,
    CREATE_CA_CONFIG_ERROR = 0xC6,
    RECONFIGURE_CA_CONFIG_ERROR = 0xC7,
    DELETE_CA_CONFIG_ERROR = 0xC8,
    DL_NUM_HARQ_PROC = 0xC9,
    DL_INIT_MCS = 0xCA,
    SCELL_REL_FLAG = 0xCB,
    SCELL_ACTIVATE_STATUS = 0xCC,
    CA_LC_BANDWIDTH_DIST_INFO = 0xCD,
    DL_INTERFERENCE_INFO = 0xCE,
    CSI_SUBFRAME_PATTERN_CONFIG_VR10 = 0XCF,
    CSI_MEASUREMENT_SUBSET_VR10 = 0XD0,
    CSI_CONFIG_INDEX_VR10 = 0XD1,
    CQI_PMI_CONFIGINDEX2_VR10 = 0XD2,
    CSI_MEASUREMENT_SUBSET2_VR10 = 0XD3,
    SCELL_CANDIDATE_EARFCN = 0xD4,
    MAC_DL_EARFCN = 0xD5,
    LAA_SCELL_CONFIG_INFO = 0xD6,
    DMTC_CONFIG_INFO      = 0xD7
    /* SPR 22152 Fix Start */
    /* INVALID_SCELL_OPERATION_TYPE has been made a macro as it is internal
     * to MAC only and not a part of RRC tag enum */
    /* SPR 22152 Fix End */
    /* SPR 21958 PUSCH RAT1 Support Start */
    ,UPLINK_RAT1_INFO = 0xD8
    /* SPR 21958 PUSCH RAT1 Support End */
#ifdef LTE_EMBMS_SUPPORTED 
    ,SIB13_INFO = 0xD9,
    CREATE_AREA_INFO = 0xDA,
    MCCH_CONFIG = 0xDB,
    MBMS_NOTIFICATION_CONFIG = 0xDC,
    AREA_CONFIG_INFO = 0xDD,
    CSA_PATTERN_LIST = 0xDE,
    PMCH_CONFIG = 0xDF,
    /* SPR 18672 Fix start */
    MAC_CREATE_MBMS_AREA_ENTITY_INFO = 0xE0,
    MAC_DELETE_MBMS_AREA_ENTITY_INFO = 0xE1,
    EMBMS_CONFIG_INFO =  0xE2,
    EMBMS_TRANSMISSION_POWER_INFO = 0xE3,
    RESET_SESSION_INFO = 0xE4,
    PMCH_RESET_LC_INFO = 0xE5,
    EMBMS_SIB2_INFO = 0xE6
    /* SPR 18672 Fix end */
#endif
    ,RACH_CE_LEVEL_INFO = 0x152,
    PRACH_CONFIG_V1310 = 0x153,
    PRACH_HOPPING_OFFSET_R13 = 0x154,
    PUSCH_CONFIG_COMMON_V1310 = 0x155,
    PDSCH_CONFIG_COMMON_V1310 = 0x156,
    PRACH_PARAMETERS_CE_R13 = 0x157,
    CE_LEVEL = 0x158,
    CE_MODE = 0x159,
    SFN_GAP_EMTC = 0x15A,
    PRACH_STARTING_SUBFRAME_R13 = 0x15B,
    FDD_UPLINK_SUBFRAME_BITMAP_BR = 0x15C,
    MODIFICATION_PERIOD_V1310 = 0x15D,
    MAX_NUM_PREAMBLE_ATTEMPT_CE_R13 = 0x15E,
    MPDCCH_START_SF_CSS_RA = 0x15F,
    N1PUCCH_INFO_LIST = 0x160,
    PUCCH_CONFIG_COMMON_V1310 =0x161,
    PUCCH_DEDICATED_CONFIG_R13 = 0x162,
    PUCCH_NUM_REPETITION_CE_R13 = 0x163,
    PUCCH_NUM_REPETITION_CE_SETP_R13 = 0x164,
    ACK_NACK_REPETITION_R13 = 0x165,
    ACK_NACK_REPETITION_SETUP_R13 = 0x166,
    EPDCCH_SET_CONFIG_R11  =  0x167,
    MPDCCH_CONFIG_R13  = 0x168,
    MPDCCH_CONFIG_SETUP_R13 = 0x169,
    SIBTYPE1_BR_MSG_INFO   = 0x16A,
    SI_MSG_BR_INFO = 0x16B,
    SI_MSG_BR  = 0x16C,
    FDD_DOWNLINK_OR_TDD_SUBFRAME_BITMAP_BR = 0x16D,
    UE_CATEGORY_V1310  = 0x16E,
    PAGING_NARROW_BAND = 0x16F,
    MPDCCH_NUM_REPETITION_PAGING = 0x170,
    PDSCH_NUM_REPETITION = 0x171,
    RACH_CONFIGURATION_R13 = 0x172,
    UL_HOPPING_CONFIG_COMMON_MODEA = 0x173,
    EMTC_CONFIG_DATA = 0x174,
    DIRECT_INDICATION = 0x175,  
    PCCH_CONFIG_DATA = 0x176,
    RRC_MAC_SFN_EMTC = 0x177
#if 0
    , NUM_CE_LEVEL = 0x152,  
    PREAMBLE_MAPPING_INFO_R13 = 0x154,
    RESOURCE_BLOCK_ASSIGNMENT_R11 =  0xF1,
    MPDCCH_START_SF_UESS_R13  =  0xF4,
#endif

} RRCTag;

/* For NGAP TAG */
typedef enum NGapTypeT
{
    NGAP1 = 0,
    NGAP2
}NGapType;

/* Enum for errors */

typedef enum ErrorT
{
   MAC_LOG_CH_ERROR ,			/* returned by MAC function call */
   MAC_SFN_SF_ERROR,
   MAC_TICK_ERROR,
   MAC_TAG_NOT_EXISTS,
   MAC_SFN_ERROR
}Error;


typedef enum LCOperationTypeT
{
  ADD = 0,
  RECONFIGURE ,
  DELETE
}LCOperationType; 

typedef enum DeltaPucchShiftT
{
  Ds1 = 1,
  Ds2,
  Ds3
}DeltaPucchShift;
  
/* + CQI_4.1 */
typedef enum cqiModeReportedT
{
    INVALID_CQI_REPORTING_MODE = 0,
    PERIODIC_CQI_REPORTING_MODE,
    APERIODIC_CQI_REPORTING_MODE,
    PERIODIC_APERIODIC_MODE
/* - CQI_4.1 */
}cqiModeReported;

typedef enum MacProcessPhyMsgStateT 
{
    MAC_PHY_CTRL_IDLE_ST = 0,
    WAITING_FOR_PHY_MAC_INIT_LAYER_RES_ST,
    WAITING_FOR_PHY_EXPLICIT_START_RES_ST,
    WAITING_FOR_PHY_START_RES_ST,
    WAITING_FOR_PHY_CONFIG_CELL_RES_ST,
    WAITING_FOR_PHY_RECONFIG_CELL_RES_ST,
    WAITING_FOR_RUNNING_PHY_RECONFIG_CELL_RES_ST,
    WAITING_FOR_PHY_CELL_STOP_RES_ST,
    WAITING_FOR_PHY_DELETE_CELL_RES_ST,
    MAC_PHY_CTRL_MAX_ST
}MacProcessPhyMsgState;



typedef enum MacProcessPhyMsgEventT 
{
    MAC_RECV_MSG_FROM_PHY_EV = 0,
    MAC_REQ_TIMEOUT_RETRY_EV,
    MAC_PHY_CTRL_MAX_EV
}MacProcessPhyMsgEvent;

/*SPR 16855 Fix Start*/
/* Enum to define states to process RRC-OAM interface messages */
typedef enum MacProcessRrcOamMsgStateT
{
    MAC_CTRL_IDLE_ST    = 0,
    WAIT_FOR_MAC_THREAD = 1,
    
    MAC_CTRL_MAX_ST
}MacProcessRrcOamMsgState;

/* Enum to define evenets to process RRC-OAM interface messages */
typedef enum MacProcessRrcOamMsgEventT
{
    MAC_RRC_CREATE_UE_ENTITY_EV   = 0,
    MAC_HP_CREATE_UE_ENTITY_EV,
    MAC_RRC_RECONFIG_UE_ENTITY_EV,
    MAC_HP_RECONFIG_UE_ENTITY_EV,
    MAC_RRC_DELETE_UE_ENTITY_EV,
    MAC_HP_DELETE_UE_ENTITY_EV,
    MAC_RRC_HO_RACH_RESOURCE_EV,
    MAC_HP_HO_RACH_RESOURCE_EV,
    /* SPR 8379 changes start */
    MAC_HP_INTRA_HO_RACH_RESOURCE_EV,
    /* SPR 8379 changes end */
    MAC_RRC_RESET_REQ_EV,
    MAC_HP_RESET_UE_BEFORE_HARQ_RESET_ENTITY_EV,
    MAC_HP_RESET_UE_AFTER_HARQ_RESET_ENTITY_EV,
    MAC_HP_SEND_RESET_REQ_AGAIN,
    MAC_RRC_CHANGE_CRNTI_EV,
    MAC_HP_CHANGE_CRNTI_EV,
    /* SPR 16583 fix start */
    MAC_RRC_HO_REL_RACH_RESOURCE_EV,
    MAC_HP_HO_REL_RACH_RESOURCE_EV,
    /* SPR 16583 fix end */

    MAC_CTRL_MAX_EV
}MacProcessRrcOamMsgEvent;

/* Enum to know that response received from both UL and DL threads */
typedef enum MacRespRcvThrdIdT
{
    MAC_NO_RESPONSE_RCVD       = 0x0,
    MAC_RESPONSE_FRM_DL_THD    = 0x1,
    MAC_RESPONSE_FRM_UL_THD    = 0x2,
    MAC_RESPONSE_FRM_BOTH_THDS = 0x3,

}MacRespRcvThrdId;
/*SPR 16855 Fix End*/

#ifdef LTE_EMBMS_SUPPORTED
typedef enum CellConfigErrorCode_en
{
    INVALID_EMBMS_AREA_PARAM=6
}CellConfigErrorCode;
#endif
/*SPR 3600 Start*/
typedef struct PUSCHConfigCommonInfoT
{
    /*Indicates whether 64QAM is supported by the Cell*/
    UInt8 enable64QAM; /*1-Enabled 0-Disabled*/
}PUSCHConfigCommonInfo;
/*SPR 3600 End*/

/* structure for logical channel information Uplink*/
typedef struct ulLogicalChannelInfoT
{
    LCOperationType     operationType;
    UInt8               lchId;
    UInt8               rlcMode;
    UInt8               lcGId; //if invalid, then Set XX value, where XX>3
    UInt16              result;
    /* Added for QoS*/   
    /* Indicates the QCI of the Logical Channel */
    UInt8 QCI;
    /* Indicates the Type of the channel i.e. GBR or NON-GBR */
    UInt32 channelType;
    /*Indicates the priority of this logical channel*/
    UInt8  logicalChannelPriority;
    /* Indicates the Gauranteed bit Rate */
    UInt32 GBR;
    /* Indicates the Maximum Bit Rate */
    UInt32 MBR;
    /* SRP 3608 changes start */
    UInt8 uePSRReqFlag; 
    /* SPR 3608 changes end */
    /* SPS_CHG */
    /* To check if isRohcProfilePresent at the time of reconfig */
    UInt8 isRohcProfilePresent;
    /* Rohc Profile */
    UInt16 rohcProfile;
    /* SPS_CHG */
} ulLogicalChannelInfo;

/*Structure maintaining all the Logical Channel Configuration Information */

typedef struct LogicalChannelConfigInfoT
{
    LCOperationType operationType;      /* Logical Channel Operation Type */
    UInt8 rlcMode;
    UInt8  lchId;                       /* Logical Channel Identifier */
    UInt8  rlcSNFieldLength;
    UInt8  lchPriority;                 /* Logical Channel Priority */
    UInt16 result;
    /* Indicates the QCI of the Logical Channel */
    UInt8 QCI;
    /* Indicates the Type of the channel i.e. GBR or NON-GBR */
    UInt8 channelType;
    /* Indicates the Latency of the Logical channel */
    UInt16 pktDlyBdgt;
    /* Indicates the Gauranteed bit Rate */
    UInt32 GBR;
    /* Indicates the Maximum Bit Rate */
    UInt32 MBR;
    /* Indicates Packet Error Rate */
    UInt32 pktErrRate;
    /* Indicates Packet Arrival Time Stamp */
    /* SPR 15909 fix start */
    tickType_t pktArrivalTS;
    /* SPR 15909 fix end */
    /* Indicates the size of the Packets */
    UInt16 avgPktSize;
    /* SRP 3608 changes start */
    UInt8 enbPSRReqFlag; 
    /* SPR 3608 changes end */
    /* SPS_CHG */
    /* To check if isRohcProfilePresent at the time of reconfig */
    UInt8 isRohcProfilePresent;
    /* Rohc Profile */
    UInt16 rohcProfile;
    /* SPS_CHG */
    /* CA changes Start */
	UInt8 bandWidthDistribution[MAX_SERVCELL];
    UInt8 isBandWidthTagPresent;
    /* CA changes End */
}LogicalChannelConfigInfo;

/*LC Config Info */
typedef enum LCConfigInfoT
{
    DL_LC_INFO = 1,
    UL_LC_INFO = 2,
    UL_DL_LC_INFO = 3
} LCConfigInfo;


typedef enum UlCyclicPrefixT 
{ 
    len1 = 1, 
    len2 =2 
}UlCyclicPrefix;    

/* + CL-MIMO LJA*/
/*SPR 16855 +-*/
/*CA changes start*/

typedef struct CodebookSubsetRestrictionv1020T
{
	codebookSubsetRestrictionType cbsrType;
	UInt8 cbsrValue[MAX_CBSR_VALUE_COUNT];

}CodebookSubsetRestrictionv1020;
typedef struct CqiInfoT
{
    CQIMode   cqiMode;
    UInt8     cqiIndication;
    UInt8     cqiRequestType;
    UInt16    cqiPMIConfigIndex;
    UInt16    cqiPucchResourceIndex;
    UInt8     cqiformatIndicatorPeriodic;
    UInt8     cqiformatIndicatorPeriodic_subband_k;
    UInt16    riConfigIndex;
    UInt16    cqiPMIConfigIndexV1020; 
    UInt16    cqiPucchResourceIndexV1020;
    UInt8     simultaneousACKNACKAndCQIV1020;
    UInt8     simultaneousACKNACKAndCQI;
    UInt8     cqiformatIndicatorPeriodicV1020;
    UInt8     cqiformatIndicatorPeriodic_subband_kV1020;
    UInt16    riConfigIndexV1020;
    UInt8     cqiMaskV920;
    UInt8     pmiRiReportV920;
    UInt8     cqiInfoFlag;
    /*EICIC Changes*/	
    UInt16    cqiPMIConfigIndex_2; 
    UInt16    riConfigIndex_2;
    UInt8 cqiConfigRequestType;
	
}CqiInfo;
/* Structure to store parameters for SCELL_INFO */
typedef struct ScellInfoT
{ 
    InternalCellIndex internalCellIndex;
    UInt8      sCellIndex;
    UInt8      operationType;
    UInt8      numOfLayer;
    TransmissonMode transmissionMode;
    UInt8      dlNumHarqProcess;
    UInt8      dlInitMCS;
    UInt8      modScheme;
    UInt8      rankIndicator;
    UInt8      precodingIndex;
    UInt8      isScellActivateStatus;
    UInt8 isHDFddFlag;
    UInt8 userLocationType;
    UInt8 pA;
    UInt8 isPAPresent;
    CodebookSubsetRestrictionv1020 codebookSubsetRestrictionv1020;
    CqiInfo   cqiInfo;
}ScellInfo;

typedef struct CarrierAggrConfigT
{
	UInt8 scellDeactivationTimer;
	UInt8 bandWidthDistribution[MAX_SERVCELL];
	UInt8 isBandWidthTagPresent;
	UInt8 scellCount;
	ScellInfo scellInfo[MAX_NUM_SCELL];
}CarrierAggrConfig;

typedef enum ULCAReconfigUeTLVsT
{
    UL_RECONFIG_CA_CONFIG_SCELL_DEACTIVATION_TIMER = 0,
    UL_RECONFIG_CA_CONFIG_SCELL_INFO
}ULCAReconfigUeTLVs;

typedef enum ULSCELLReconfigUeTLVsT
{
    UL_RECONFIG_SCELL_PUCCH_CONFIG_INFO = 0,
    UL_RECONFIG_SCELL_RF_PARAMS,
    UL_RECONFIG_SCELL_ACTIVATE_STATUS
}ULSCELLReconfigUeTLVs;

typedef struct ULUeCategoryV1020T
{
    /* Padding of 1 bytes */
    UInt8 padding;
    UInt8 ueCategoryV1020;
    UInt16 maxTBSize;
}ULUeCategoryV1020;

/*CA changes end*/

typedef struct UeContextForDownlinkT
{
    UInt8 maxRB;
    UInt8 numHarqProcess;
    UInt8 dwlkLogicalChCount;
    UInt8 modScheme;
    UInt8  hScheme;
    UInt8 rankIndicator;
    UInt8 rlcMode;
    UInt8 serviceType;
    UInt8 cqiIndication;
 /* Coverity 32355 Fix Start */
    CQIMode cqiMode;
 /* Coverity 32355 Fix End*/
    UInt8 cqiInfoFlag;
    /*SPR 903 Start */
    UInt8 ueCategory;
    /*SPR 903 End */
    /* SPR 5084 changes Starts */
    /*Indicates whether ueCategory is received from L3 or not*/
    UInt8 ueCategoryReqType;
    UInt8       drxConfigType;
    /* SPR 5084 changes ends */

    UInt16 cRnti;  /*indicates the RNTIs of the UL*/
    UInt16 ueIndex;
    UInt16 rrcUeIndex;
    UInt8  precodingIndex;
  
    /* Coverity 54157 Fix Start */
    TransmissonMode  transmissionMode;
    /* Coverity 54157 Fix End */

    UInt8 uePriority;
    /*indicates the modulation scheme to be used for particular UE*/
    /* SPR 2063 start */
    UInt8       mcsIndex;
    /* + CL-MIMO LJA*/
    /*Represents the codebookSubsetRestrictionConfigured by the upper layers*/
    codebookSubsetRestrictionInfo cbsrInfo;
    DRXConfig   drxConfig;
    /* DRX_CHG */

    /* - CL-MIMO LJA*/
    /* DRX_CHG */
    UInt8       drxRequestType;
    /* DRX_CHG */
    /* MEAS_GAP_CHG */
    UInt8               measGapConfigType;
    UInt8               measGapRequestType;
    MeasGapConfig    measGapConfig;

    LogicalChannelConfigInfo logicalChConfig[10];
    /* MEAS_GAP_CHG */
    /*Fix for __SPR_1988__ START*/
    UInt32 ulSyncTimer;
    /*Fix for __SPR_1988__ END*/
    /*SPR_3061_FIX*/
    /* SPR 15909 fix start */
    tickType_t lastSyncDetectedTick;
    /* SPR 15909 fix end */
    UInt16 timeAlignmentTimer; 
    /*SPR_3061_FIX*/

    /* DL qos strategy change start */
    UInt8  dlAmbrPresent;
    /* L2_PERFORMANCE_OPT_CHANGES_START */
    UInt32 dlAmbr;
    /* L2_PERFORMANCE_OPT_CHANGES_END */
    /* DL qos strategy change stop */
    /*Parameters added for Power Control*/
    /*Indicates the transmission Power of PDSCH in the form of EPRE*/
    UInt8 isPAPresent;
    /*CLPC_REWORK_CHG*/
    UInt8 pA;
    /*CLPC_REWORK_CHG*/
    /*Configuration information of TPC for PUCCH*/
    UInt8 isTpcPdcchConfigPucchPresent;
    TpcPdcchConfig tpcPdcchConfigPucch;

    /* UE_CAPAB_ADD_PARAM Start */
    UInt8 Rat1Support;
    /* UE_CAPAB_ADD_PARAM End */

    /* ICIC changes start */
    UInt8 userLocationType;
    /* ICIC changes end */
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /*This flag will indicate whether UE is HD FDD or not
      1/TRUE - UE is HD FDD
      0/FALSE - UE is FD HDD 
     */
    UInt8 isHDFddFlag;
#endif
    /*HD FDD Changes End*/

    /* SPS_CHG_START */
    UInt8 isSpsDlInfoPresent;
    /* SPS Configuration Information for Downlink */
    spsDlInfo spsDownlinkInfo;
    /* SPS_CHG_END */
    /* + SPR 5953 Fix */
    UInt8 cqiRequestType;
    /* - SPR 5953 Fix */

    /* + TM7_8 Changes Start */
    UInt8 pmiRiReportV920;
    /*Ratio of PDSCH EPRE to UE specific Reference signal
     *Valid only when Transmission Mode is 7 and modulation scheme is QPSK */
    SInt8 pdschEpreToUeRsRatio;
    /* + TM7_8 Changes Start */
    /*CA changes start*/
    UInt8     trigger1r10;
    UInt8     trigger2r10;
    UInt8 ueCategoryV1020;
    UInt8 ueCategoryV1020ReqType;
    PucchFormatR10 pucchFormatR10;
    CarrierAggrConfig carrierAggrConfig;
    UInt8 cqiSubsetConfigRequestType;
    UInt8 cqiConfigRequestType;
    /* EICIC_Coverity CID 69396 Start */
    UserType userType;
    /* EICIC_Coverity CID 69396 End */
    UInt8  userTypePresent;
    /*CA changes end*/
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    UInt8 operatorId;
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
}UeContextForDownlink;


/*Uplink UEContext */

typedef struct UeContextForUplinkT 
{
    UInt8       uePriority;
    UInt16 cRnti;  /*indicates the RNTIs of the UL*/
    UInt8 modScheme;
    /*indicates the modulation scheme to be used for particular UE*/
    /* SPR 2063 start */
    UInt8       mcsIndex;
    /* SPR 2063 end */
    UInt8       numLogicalChannels;
    UInt8       modulationSchemeFactor;
    UInt8       ServiceType;
    UInt8       rlcMode;
    UInt16      ueIdx;   /*indicates the UE ID assigned to this UE*/
    /* SPR 2063 start */
    /* SPR 2063 end */
    UInt8 	simultaneousACKNACKAndCQI;
    UInt8   subbandCQI_k;
    ulLogicalChannelInfo lcInfo[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    UInt8 requestType;
    UInt8 srconfigflag;
    UInt16 srPucchResourceIndex;
    UInt8 srConfigurationIndex;
    /* Rel9_upgrade_CR396 */
    UInt8 cqiMaskV920;
    UInt8 pmiRiReportV920;
    /* Rel9_upgrade_CR396 */
    /* Rel9_upgrade_CR410 */
    UInt16      srProhibitTimerValv920;
    /* Rel9_upgrade_CR410 */
    UInt8 srSetupflag;
    UInt8 cqiRequestType;
    UInt8 cqiinfoflag;
    UInt8 cqiPMIConfigIndexFlag;
    UInt8 cqiIndication;
    UInt8 cqiMode;
/* new valriables added for PUSCH config info */
    UInt8 puschConfigInfoFlag;
    UInt8 betaOffsetAckIndex;
    UInt8 betaOffsetRiIndex;
    UInt8 betaOffsetCQIIndex;
    UInt16 cqiPMIConfigIndex;
    UInt16 cqiPucchResourceIndex;
    UInt16 riConfigIndex;
    /* SRS_CHG */
    /*Indicates whether SRS dedicated config is received from L3 or not*/
    UInt8  srsRequestType;
    UInt8  srsConfigType;
    /*Dedicated SRS config info received from RRC*/
    SRSDedicatedSetupInfo   srsDedicatedSetupInfo;
    /* SRS_CHG */
    /* Variable for TDD */    
#ifdef TDD_CONFIG
    UInt8   tddAckNackFeedbackMode;
#endif        
    /* SS_S2 */

    /* Coverity 32360 32358 32357 32353 Fix Start */
    PeriodicFeedbackType  cqiformatIndicatorPeriodic;
    /* Coverity 32360 32358 32357 32353 Fix End */

    UInt8 cqiformatIndicatorPeriodic_subband_k;
    /* SS_S2 */
    /* ULA_CHG */
    /* Maximum number of transmissions for UL HARQ */
    UInt8 maxUlHARQTx;
    /*indicates the category of this UE*/
    UInt8 ueCategory;
    /* SPR 5084 changes Starts */
    /*Indicates whether ueCategory is received from L3 or not*/
    UInt8 ueCategoryReqType;
    /* SPR 5084 changes ends */
    /* ULA_CHG */
    UInt8  ulAmbrPresent;
/* L2_PERFORMANCE_OPT_CHANGES_START */
    UInt32 ulAmbr;
/* L2_PERFORMANCE_OPT_CHANGES_END */
    
    /*Parameters added for Power Control*/

    /*Configured UE transmitted power according to its power class*/
    SInt8 pcMax;
    /*Contains information elements of dedicated uplink power control.*/
    UInt8 isDedicatedULPowerControlInfoPresent;
    DedicatedULPowerControlInfo dedicatedULPowerControlInfo;

    /*Configuration information of TPC for PUSCH*/
    UInt8 isTpcPdcchConfigPuschPresent;
    TpcPdcchConfig tpcPdcchConfigPusch;

    /* ICIC changes start */
    UInt8 userLocationType;
    /* ICIC changes end */
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
	/*This flag will indicate whether UE is HD FDD or not
     * 1/TRUE - UE is HD FDD
     * 0/FALSE - UE is FD HDD 
     */
    UInt8 isHDFddFlag;
#endif
    /*HD FDD Changes End*/

    /* SPR 5713 changes start */
    UInt8 ueComplianceRelease;
    /* SPR 5713 changes end */

    /* SPS_CHG_START */
    UInt8 isSpsUlInfoPresent;
    /* SPS Configuration Information for Uplink */
    spsUlInfo spsUplinkInfo;
    /* SPS_CHG_END */
    /*UL_MU_MIMO_CHG_START*/
    UInt8 ulMUMIMOStatus;
    /*UL_MU_MIMO_CHG_END*/

    /* +DYNAMIC_ICIC */
    PhrTimerConfig phrTimerCfg;
    UInt32 phrGrant;
    SInt8 minUePower;
    /* -DYNAMIC_ICIC */
    UInt8 ttiBundlingEnabled;
   /*CA changes start */
    UInt8     trigger1r10;
    UInt8     trigger2r10;
    UInt8 ueCategoryV1020;
    UInt8 ueCategoryV1020ReqType;
    PucchFormatR10 pucchFormatR10;
    CarrierAggrConfig carrierAggrConfig;
    /*CA changes end*/
    UInt8 cqiConfigRequestType;
    UInt16 cqiPMIConfigIndex_2;
    UInt16 riConfigIndex_2;
    UInt8 cqiSubsetConfigRequestType;
    UInt8 csiMeasurementSubset1[MAX_CSI_MEASUREMENT_PATTERN];
    UInt8 csiMeasurementSubset2[MAX_CSI_MEASUREMENT_PATTERN];
    /* EICIC_Coverity CID 69397 Start */
    UserType userType;
    /* EICIC_Coverity CID  69397 End */
    UInt8  userTypePresent;
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    UInt8 isUplinkRAT1Supported;
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    UInt8 operatorId;
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
} UeContextForUplink;


/* Following structure defines the SPS common cell config information */

 /* + PRS_CAHNGES */
typedef struct PRSParseConfigInfoT
{ 
    UInt8 updateFlag;
    UInt8 isPRSConfigured;
    DlBandwidth prsBandwidth;
    UInt8 prsSubframes;
    UInt16 prsConfigIndex;
    UInt16 prsTransmissionPower;
    UInt8 bitsTobeRead;
    UInt16 prsMutingConfigIndex;
    UInt8 prsChangeBits; 
}PRSParseConfigInfo;
 /* - PRS_CAHNGES */

typedef struct ConfigCellReqT 
{
    UInt8 duplexingMode;
    UInt8 dlResBlocks;     
    UInt8 ulResBlocks;     
    UInt8 maxHarqRetrans;
    UInt8 numOfTxAntennas;
    UInt8 startRARntiRange;
    UInt8 endRARntiRange;
    UInt8 phichDuration;
    UInt8 phichResource;  	
    UInt8 ulCyclicLenthPrefix;
    UInt8 sfnGap;
    UInt8 nB;
    /* SPR 19309 + */
    UInt8 cfi;
    /* SPR 19309 - */
    /* SPR 19310 + */
    UInt16 numEUL;
    UInt16 sizeEUL;
    /* SPR 19310 - */
    /*Parameters added for Power Control*/
    UInt32 modificationPeriod;

    /*For storing start and end values of TPC RNTI*/
    TPCRntiRange tpcRntiRange;
    
    /*For storing start and end values of TPC RNTI*/
    /*Contains the downlink reference-signal transmit */ 
    /*Contains power offsets for PCFICH, PHICH, PDCCH, PBCH and PCH*/
    CommonDLPowerControlInfo commonDLPowerControlInfo;
    /* power used to derive downlink reference-signal*/
    PDSCHConfigCommonInfo pdschConfigCommonInfo;
    /*Parameters added for Power Control*/

    
    /*Indicates whether closed loop power control feature is enabled or not*/
    PowerControlEnableInfo powerControlEnableInfo;
    /* ULA_CHG */
    /* FREQ_SELECTIVE CHG */
    UInt8 freqSelectiveSchUL;
    UInt8 freqSelectiveSchDL;
    /* FREQ_SELECTIVE CHG */
    /* ULA_CHG */
    /* Cell Start Stop Chg */
    UInt8 explicitStartRequired;

    /*Contains a cell specific nominal component for PUSCH and 
     * a 3-bit cell specific parameter for power control*/
    CommonULPowerControlInfo commonULPowerControlInfo;
     
    /* Indicates SPS parameters in cell setup */
    SpsCommonCellInfo spsCommonCellInfo;
    
    /*SRS common setup config info received from RRC*/
    SRSCommonSetupInfo  srsCommonSetupInfo;
    DlBandwidth dlBandwidth;

    /*SPR 3600 Start*/
    /*Contains information received from RRC/S1AP for PUSCH-ConfigCommon*/
    PUSCHConfigCommonInfo puschConfigCommonInfo;
    /*SPR 3600 End*/
 
#ifdef TDD_CONFIG    
    /*TDDFRAME Config start*/
    UInt16 subFrameAssign;
    UInt16 splSubFramePatterns;
    /*TDDFRAME Config end*/
#endif   
    /* SRS_CHG */
    /*Used for interface validation whether setup / released is received 
    during createCellReq()*/
    UInt8              srsConfigType;  
    /* +- SPR 19066 */
    UInt16 timeAlignmentTimer;
    UInt16 contnFreeRachTimer;
    /* SRS_CHG */
    /*SPR_3061_FIX*/
    UInt8 pucchCQISINRThreshold;
    UInt16 ulSyncLossTimer;
    /*SPR_3061_FIX*/

    UInt8 isDynamicPdcchDisabled;
    PRSParseConfigInfo prsParseConfigInfo;


} ConfigCellReq;
 
/* for phich config */
typedef struct PhichConfigT
{
    UInt8 phich_Duration;
    UInt8 phich_Resource;
}PhichConfig;    

/* for cell config response */

typedef struct ConfigCellResT 
{
    UInt16 transactionId;      /* transaction ID sent by RRC Sim */
    UInt16  response;           /* MAC SUCCESS or FAILURE Coverity-10018 */
    /* CA changes Start */
    UInt16  errorType;           /* MAC_SFN_DOES_NOT_EXIST/SCELL_NOT_RUNNING/SCELL_NOT_EXISTS */
    /* CA changes End */
} ConfigCellRes;


/* for UEEntity Reconfigure response */

typedef struct UEEntityReConfigResT
{
    UInt16 transactionId;      /* transaction ID sent by RRC Sim */
    UInt16  response;           /* Mac SUCCESS or FAILURE */
    /* SPS_CHG_START */
    UInt16 spsCrnti;          /* Optional field send when SPS config recived in UEEntityCreateReq*/
    /* SPS_CHG_END */
    UInt8 Padding;
} UEEntityReconfigRes;


/* for delete UEEntity response */

typedef struct UEEntityDeleteResT {
    UInt16 idx;
    UInt16 response;       /* ACK/NACK Coverity-10018 */
    UInt16 transactionId;
    /*REL10_CR_CHANGES_START*/
    UInt8 deleteLCStatus[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    /*REL10_CR_CHANGES_END*/ 
} UEEntityDeleteRes;



/* for cell reconfig response */

typedef struct ReconfigCellResT 
{
    UInt16 transactionId;      /* transaction ID sent by RRC Sim */
    UInt16  response;           /* MAC SUCCESS or FAILURE Coverity-10018 */
    /* CA changes Start */
    UInt16  errorType;           /* MAC_SFN_DOES_NOT_EXIST/SCELL_NOT_RUNNING/SCELL_NOT_EXISTS */
    /* CA changes End */
} ReconfigCellRes;

/* for cell delete reponse */

typedef struct ReconfigCellResT DeleteCellRes;

/* Cell Start Stop Chg */
typedef struct ReconfigCellResT CellStopRes;
 
/* for create UEEntity response */

typedef struct UEEntityCreateResT 
{
    UInt16 transactionId;      /* transaction ID sent by RRC Sim */
    UInt16 response;           /* MAC SUCCESS or FAILURE Coverity-10020 */
    /* SPS_CHG_START */
    UInt16 spsCrnti;          /* Optional field send when SPS config recived in UEEntityCreateReq*/
    /* SPS_CHG_END */
    UInt8 padding; /* Alignment changes */
} UEEntityCreateRes;
/* SPS_CHG */
/* MAC_RECONFIG_COMPLETE_IND parameters */
typedef struct UEReconfigCompIndT
{
    UInt16 ueIndex;
    UInt8  response;
}UEReconfigCompInd;
/* SPS_CHG */

/* for ul LC create req */

typedef struct UEEntityulLCCreateReqT
{
	
    UInt8 lchId;
    UInt8 rlcMode;
    UInt8 ServiceType;
    UInt8 lcGId;
    UInt8 transportChType;
} UEEntityulLCCreateReq;


/* for dl LC create req	*/
typedef struct UEEntitydlLCCreateReqT
{
	
    UInt8 lchId;
    UInt8 rlcMode;
    UInt8 ServiceType;
    UInt8 lchPriority;
    UInt8 transportChType;
} UEEntitydlLCCreateReq;



/* Logical channels used by the MAC layer */
typedef enum LogicalChannelT
{
    BCCH = 0,
    PCCH,
    CCCH,
    DCCH,
    DTCH
} LogicalChannel;




/* for logical channel configuration response */

typedef struct LCConfigRespT {
    UInt8  status;
    UInt8  opCode;
    UInt16 result;
} LCConfigResp;

typedef struct ScellConfigRespT {
    UInt8  status;
    UInt8  opCode;
    UInt16 result;
    UInt8  cellId;
} ScellConfigResp;


/*structure for sending information to RRC*/
typedef struct rrcInfoDataT {
    UInt16  dataLen;
    UInt8   msgType;     /* CCCH_PDU or CRNTI */
    void   *buffer_p;    /*pointer to the data to be
                           passed to RRC layer from UL de-mux*/
    UInt16  rnti;

    /*Parameters added for Power Control*/

    /*Indicates the value of timing advance sent in RAR 
     * message for the UE*/
    UInt16 timingAdvance;
    /*Value of SINR received with message 3 for the UE*/
    /* + Coverity_31943 */
    UInt16 sinr;
    /* - Coverity_31943 */
}rrcInfoData;


/* for power headroom indication */

typedef struct powerHeadroomInfoT
{
    UInt16  ueIdx;
    UInt16  powerHeadroom;
}powerHeadroomInfo;

	


/* for SFN response */

/* SIB8_Code Start */
typedef struct SFNResT
{
   /* SIB8_Code Start */ 
/* SIB8_Code_fix Start */
    UInt8 sf;	               // sub frame number 
    UInt8 timeStampFlag;	   // timeStampFlag
/* SIB8_Code_fix End */
   /* SIB8_Code End */ 
    UInt16  response;           // MAc SUCCESS or FAILURE Coverity-10018
    UInt16 transactionId;      // transaction ID sent by RRC Sim 
    UInt16 sfn;    	       // starting frame number 

}SFNRes;

/* SIB8_Code End */
/* Cell Start Stop Chg */
typedef struct SFNResT CellStartRes;

/* for SFN error  indication */

typedef struct SFNErrIndT
{
    UInt16 transactionId;      // transaction ID sent by RRC Sim 
    UInt16 SFN;
    UInt8 SF;
}SFNErrInd;

#ifdef LOG_PRINT_ENABLED

enum FailureReasonValues
{
    NO_FAIL=0,
    INVALID_TOTAL_MSG_LEN,
    INVALID_SFN,
    INVALID_REM_LEN_ONE,
    INVALID_REM_LEN_TWO,
    INVALID_REM_LEN_THREE,
    INVALID_PRACH_CFG_FREQ_INDEX,
    INVALID_MSG3_MAX_HARQ_TRANS,
    INVALID_CONTENTION_RES_TIMER_VAL,
    INVALID_RARES_WINDOW_SIZE,
    RACH_CONFIG_FAIL,
    INVALID_UEINDEX_VALUE,
    UPDATE_DRX_UECONTEXT_FAIL,
    INVALID_CELL_MODE,
    INVALID_DL_RES_BLOCKS,
    INVALID_UL_RES_BLOCKS,
    INVALID_HARQ_RETRANSMISSIONS,
    INVALID_TX_ANTENNA,
    INVALID_CYCLIC_PREFIX_LEN,
    INVALID_START_RNTI,
    INVALID_END_RNTI,
    INVALID_MOD_PERIOD,
    INVALID_SFN_GAP,
    INVALID_NB_VALUE,
    INVALID_TAG,
    INVALID_PHICH_TAG,
    INVALID_PHICH_CONFIG_LEN,
    INVALID_PHICH_DURATION,
    INVALID_PHICH_RESOURCE,
    INVALID_PUCCH_CONFIG,
    INVALID_PUCCH_CONFIG_LEN,
    INVALID_PUCCH_DELTA_SHIFT,
    INVALID_PUCCH_nRB_CQI,
    INVALID_PUCCH_nCS_AN,
    INVALID_PUCCH_n1PUCCH_AN,
    INVALID_TAG_LEN,
    INVALID_BCCH_VAL,
    INVALID_SRS_VAL,
    INVALID_SRS_TAG,
    /* SPR 19309 + */
    INVALID_CFI_VALUE,
    /* SPR 19309 - */
    CELL_CONFIG_REQUEST_FAIL,
};


#define MAX_RECONFIG_FAIL_VALUES 7

#define FAIL_TYPE_OFFSET 0

#endif
/*For CHANGE CRNTI Info*/
typedef struct ChangeCrntiInfoT
{
    UInt16 ueIndex;
    UInt16 oldCrnti;
    UInt16 newCrnti;
}ChangeCrntiInfo;
/*for Change Crnti Resp*/
typedef struct ChangeCrntiRespInfoT
{
    UInt16 ueIndex;
    UInt16 resp;
    UInt16 transactionId;
}ChangeCrntiRespInfo;
/* For HO Rach Resources Response */
typedef struct HORachResourceRespInfoT
{
    UInt16 transactionId;
    UInt16 ueIndex;
    UInt16 resp;
    UInt16 tcrnti;
    UInt8 preambleIdx;
    UInt8 prachMaskIdx;
}HORachResourceRespInfo;
/* For UE Inactivity Timer */

/* +- SPR 19066 */ 
/* for RLF Cause */
typedef enum RLFCauseTypeT
{
    UE_UNRECOVERABLE_FROM_UL_SYNC_LOSS
}RLFCauseType;
/* For RLF Ind */
typedef struct RLFInfoT
{
    UInt16 ueIndex;
    UInt16  rlfCause;
}RLFInfo;
/* for Preamble Info */
typedef struct PreambleInfoT
{
    UInt8 preambleIndex;
    UInt8 prachMaskIdx;
}PreambleInfo;
/* For CRNTI Info */
typedef struct CrntiInfoT
{
    UInt16 tcrnti;
}CrntiInfo;
/*SPR_3065_FIX*/
typedef enum SyncIndIdT
{
    OUT_OF_SYNC_INITIAL,
    IN_SYNC_AFTER_OUT_OF_SYNC,
    OUT_OF_SYNC_MAX_RETRIES,
    IN_SYNC
}SyncIndId;
typedef struct SyncIndInfoT
{
    UInt16 ueIndex;
    UInt16 rnti;
    /* Review comment fix start SYNC_LOSS_5 */
    UInt8 syncStatus;
    /* Review comment fix end SYNC_LOSS_5 */
}SyncIndInfo;
/*SPR_3065_FIX*/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129059) */
/* + SPR 17439 */
void initRRCAndOAMInterface(void);
/* - SPR 17439 */
void processOAMAndRRCMsg ( 
        /* +- SPR 17777 */
        UInt8 * msgHdr,UInt8 *msgBuf,SInt32 bytesRead
        );

extern UInt8 * receiveOAMAndRRCMsg(UInt8* msgHdr,SInt32 *bytesRead_output, UInt8 *mem_type);
/* SPR 7435 Changes Starts */
/* Coverity 530 CID 36229 */
SInt32 sendMsgToRRC( UInt32 msgId,
/* Coverity 530 CID 36229 */
/* SPR 5599 changes end (ZIP ID 129059) */
/* SPR 7435 Changes Ends */
                            void *data_p,
                            /* CA Changes start */
                            RrcCellIndex cellIndex
                            /* CA Changes end */
                        );

/* CA Changes start */
 void prepareMacRrcApiHeader(
    UInt8 *msgBuf_p,
                                        UInt16 msgId, 
    UInt16 msgLength,
    RrcCellIndex cellId);
/* CA Changes end */

/*SPR 4016 Fix Start */
/* +- SPR 19066 */
/*Dynamic Pdcch Changes Start*/

/* Start of change for VARIBLE Declaration */
extern UInt8 (*getCountofOverLapRB)(UInt32 , UInt32 *); 
/* End of change for VARIBLE Declaration */

/*Dynamic Pdcch Changes End*/
/*SPR 4016 Fix End */

/* CA Changes start */
MacRetType macValidateUeCategoryV1020(UInt8 ueCategoryV1020
    ,InternalCellIndex internalCellIndex
);
/* CA Changes End */

/*SPR 16855 +-*/

/******************** Enumerated dataTypes *********************/
typedef enum ULReconfigPucchMsgTypeT
{
    UL_RECONFIG_NO_PUCCH_MSG = 0,
    UL_RECONFIG_SR_MSG = 1,
    UL_RECONFIG_CQI_MSG ,
    UL_RECONFIG_SR_CQI_MSG
}ULReconfigPucchMsgType;

typedef enum ULReconfigCQITypeT
{
    UL_RECONFIG_NO_CQI_MSG = 0,
    UL_RECONFIG_CQI_PERIODIC_MSG = 1,
    UL_RECONFIG_CQI_APERIODIC_MSG ,
    UL_RECONFIG_CQI_PERIODIC_APERIODIC_MSG 
}ULReconfigCQIType;

typedef enum ULReconfigPeriodicCQITypeT
{
    UL_RECONFIG_NO_CQI_SUBBAND_RI_MSG = 0,
    UL_RECONFIG_CQI_SUBBAND_K_MSG = 1,
    UL_RECONFIG_CQI_RI_MSG ,
    UL_RECONFIG_CQI_SUBBAND_RI_MSG
}ULReconfigPeriodicCQIType;

typedef enum ULReconfigUeTLVsT
{
    UL_RECONFIG_PUCCH_CONFIG_INFO = 0,
    UL_RECONFIG_UE_CATEGORY,
    UL_RECONFIG_SRS_DEDICATED_CONFIG,
    UL_RECONFIG_AMBR_QOS_INFO,
    UL_RECONFIG_P0_UE_PUSCH,
    UL_RECONFIG_TPC_PDCCH_CONFIG_PUSCH,
    UL_RECONFIG_CONFIGURE_LC_REQ,
    /** HD FDD Changes **/
    UL_RECONFIG_RF_PARAMS,
    /** HD FDD Changes **/
	UL_RECONFIG_SPS_PARAMS
    /*SPR21501 fix start*/
    ,UL_RECONFIG_TTI_BUNDLING
    /*SPR21501 fix end*/
   /* CA changes Start */
    ,UL_RECONFIG_UE_CATEGORY_V1020
    ,UL_RECONFIG_PUCCH_CONFIG_INFO_V1020
    ,UL_RECONFIG_CA_CONFIG_INFO
   /* CA changes End */
    /* + coverity 54155, 54154, 54153 */    
    ,UL_RECONFIG_PC_MAX_INFO
    ,UL_RECONFIG_MAX_UL_HARQ_TX_INFO
    ,UL_RECONFIG_PUSCH_CONFIG_INFO
    ,UL_RECONFIG_INTERFERENCE_INFO
    /* - coverity 54155, 54154, 54153 */    
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    ,UL_RECONFIG_OPERATOR_ID_INFO
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
}ULReconfigUeTLVs;

/* enum to get which map(RI/CQI) to reconfigure on the basis of parameter configured while reconfiguration*/
typedef enum ULReconfigCQIRIMsgTypeT
{
    /*+COVERITY 5.3.0 - 32255*/    
    UL_RECONFIG_DEFAULT_RECONFIG_MSG = 0,
    /*-COVERITY 5.3.0 - 32255*/    
    UL_RECONFIG_CQI_MAP_RECONFIG_MSG = 1,
    UL_RECONFIG_CQI_RESOURCE_INDEX_RECONFIG_MSG = 2,
    UL_RECONFIG_CQI_NO_RECONFIG_MSG = 4,
    UL_RECONFIG_RI_MAP_RECONFIG_MSG = 8,
    UL_RECONFIG_RI_RESOURCE_INDEX_RECONFIG_MSG = 16,
    UL_RECONFIG_RI_NO_RECONFIG_MSG = 32,
    UL_RECONFIG_CQI_RI_MAP_RECONFIG_MSG = 9,
    UL_RECONFIG_CQI_RI_RESOURCE_INDEX_RECONFIG_MSG = 18,
    UL_RECONFIG_CQI_RI_NO_RECONFIG_MSG = 36,
    UL_RECONFIG_CQI_MAP_RI_RESOURCE_INDEX_MSG = 17,
    UL_RECONFIG_CQI_MAP_NO_RI_MSG = 33,
    UL_RECONFIG_CQI_RESOURCE_INDEX_RI_MAP_MSG = 10,
    UL_RECONFIG_CQI_RESOURCE_INDEX_NO_RI_MSG = 34,
    UL_RECONFIG_NO_CQI_RI_MAP_RECONFIG_MSG = 12, 
    UL_RECONFIG_NO_CQI_RI_RESOURCE_INDEX_RECONFIG_MSG = 20,
    /*Klockwork fix start*/
    UL_RECONFIG_CQI_DEFAULT_PARAMETERS_MSG=64
    /*Klockwork fix end*/
}ULReconfigCQIRIMsgType;


typedef enum ULReconfigSRMsgTypeT
{
    UL_RECONFIG_SR_MAP_MSG,
    UL_RECONFIG_SR_RESOURCE_INDEX_MSG,
    UL_RECONFIG_NO_SR_RECONFIGURE_MSG
}ULReconfigSRMsgType;

typedef enum MapReconfigMsgTypeT
{
    UL_RECONFIG_ADD_SHORT_SRS_NODE = 1,
    UL_RECONFIG_ADD_LONG_SRS_NODE =2,
    UL_RECONFIG_DELETE_SHORT_SRS_NODE = 4,
    UL_RECONFIG_DELETE_LONG_SRS_NODE = 8
       
}MapReconfigMsgType;
 typedef enum MapReconfigSRMsgTypeT
{
     UL_RECONFIG_ADD_SHORT_SR_NODE = 1,
     UL_RECONFIG_ADD_LONG_SR_NODE =2,
     UL_RECONFIG_DELETE_SHORT_SR_NODE = 4,
     UL_RECONFIG_DELETE_LONG_SR_NODE = 8,
     UL_RECONFIG_UPDATE_SHORT_SR_NODE = 16,
     UL_RECONFIG_UPDATE_LONG_SR_NODE = 32
}MapReconfigSRMsgType;
/*Reconfig Changes Start */
typedef enum MapReconfigCQIRITypeT
{
    UL_RECONFIG_ADD_CQI_NODE = 1,
    UL_RECONFIG_DELETE_CQI_NODE = 2,
    UL_RECONFIG_UPDATE_CQI_NODE =4,
    UL_RECONFIG_ADD_RI_NODE = 8,
    UL_RECONFIG_DELETE_RI_NODE = 16,
    UL_RECONFIG_UPDATE_RI_NODE =32,
    UL_RECONFIG_ADD_CQI_NODE2 = 128,
    UL_RECONFIG_DELETE_CQI_NODE2 = 256,
    UL_RECONFIG_UPDATE_CQI_NODE2 =512,
    UL_RECONFIG_ADD_RI_NODE2 = 1024,
    UL_RECONFIG_DELETE_RI_NODE2 = 2048,
    UL_RECONFIG_UPDATE_RI_NODE2 =4096
}MapReconfigCQIRIType;
/*Reconfig Changes End */
/******************SR_CONFIG_INFO  *******************************/
typedef struct SRConfigInfoT
{
    /* padding of 1 byte*/
    UInt8                 padding;
    UInt8                 srPeriodicity;
    UInt8                 srOffset;
    UInt16                srPUCCHRBIndex;
    UInt16                srProhibitTimerValv920;
    ULReconfigSRMsgType   ulReconfigSrMsg;
    MapReconfigSRMsgType  mapReconfigSrMsg;
    void                  *headShortSRNode_p;
    /* Review comment fix start PUCCH_ENH_2 */
    UInt8  prbIndexSlot1[MAX_PUCCH_PERIODIC_REPORT];
    UInt8  prbIndexSlot2[MAX_PUCCH_PERIODIC_REPORT];
    /* Review comment fix end PUCCH_ENH_2 */
    UInt8 maxSrWrapAround;
}SRConfigInfo;

/******************CQI_INFO  *******************************/
typedef struct ULCQIAperiodicInfoT
{
 /* Coverity 32355 Fix Start */
    CQIMode cqiAperiodicMode;
 /* Coverity 32355 Fix End */
    /* padding of 3 bytes*/
    UInt8 padding[3];
    /* CA changes Start*/
    UInt8     trigger1r10;
    UInt8     trigger2r10;
    /* CA changes End*/
}ULCQIAperiodicInfo;

typedef struct CQIFormatPeriodicSubbandKT
{
    UInt8 subbandCQI_k;
    /* padding of 3 byte*/
    UInt8 padding[3];
}CQIFormatPeriodicSubbandK;
typedef struct RIConfigIndexInfoT
{
    UInt16 riConfigIndex;
    /* padding of 1 byte*/
    UInt8 padding;
    UInt16 riPeriodicity;
    UInt16 riOffset;
    UInt16 riConfigIndex_2;
    UInt16 riPeriodicity_2;
    UInt16 riOffset_2;
    UInt8 maxRIWrapAround;
    UInt8 maxRIWrapAround2;
}RIConfigIndexInfo;


typedef struct ULCQIPeriodicReqInfoT
{
    ULReconfigPeriodicCQIType cqiPeriodicMsg;
    ULReconfigCQIRIMsgType cqiRiReconfigMsg;
    UInt16 cqiPeriodicity;
    UInt16 cqiPeriodicity_2;
    UInt16 cqiPUCCHRBIndex;
    UInt8  simultaneousAckNackAndCQI;
    UInt8 cqiPmiConfigIndex;
    UInt8 cqiPmiConfigIndex_2;
    /* padding of 2 byte*/
    UInt8 padding[2];
    UInt32 periodicRepMode;
    UInt32 periodicFeedBackType;
    UInt32 widebandPeriodicity;
/* PUCCH TDD ADAPTATION CHANGES start */
    UInt16 cqiOffset;
    UInt16 cqiOffset_2;
/* PUCCH TDD ADAPTATION CHANGES end */
#ifdef FDD_CONFIG
    /* Review comment fix start PUCCH_ENH_2 */
    UInt8  prbIndexSlot1[MAX_PUCCH_PERIODIC_REPORT];
    UInt8  prbIndexSlot2[MAX_PUCCH_PERIODIC_REPORT];
    /* Review comment fix end PUCCH_ENH_2 */
#endif
    UInt8 maxCQIWrapAround;
    UInt8 maxWbCQIWrapAround;
    UInt8 maxCQIWrapAround2;
    UInt8 maxWbCQIWrapAround2;
    /*CQI_FORMAT_PERIODIC_SUBBAND_K */
    CQIFormatPeriodicSubbandK cqiFormatPeriodicSubbandK;
    /*RI_CONFIG_INDEX_INFO */
    RIConfigIndexInfo riConfigIndexInfo;
    /* CA changes Start*/
    UInt16    cqiPMIConfigIndexV1020; 
    UInt16    cqiPucchResourceIndexV1020;
    UInt8 simultaneousACKNACKAndCQIV1020;
    UInt8     cqiformatIndicatorPeriodicV1020;
    UInt8     cqiformatIndicatorPeriodic_subband_kV1020;
    UInt16    riConfigIndexV1020;
    /* CA changes End*/
}ULCQIPeriodicReqInfo;

typedef struct ULCQIPeriodicResInfoT
{
    ULReconfigPeriodicCQIType cqiPeriodicMsg;
}ULCQIPeriodicResInfo;

typedef struct ULCqiReqInfoT
{
    ULReconfigCQIType cqiConfigMsg;
    /* CQI_APERIODIC_CONFIG_INFO */
    ULCQIAperiodicInfo cqiAperiodicInfo;
    /* CQI_PERIODIC_CONFIG_INFO */
    ULCQIPeriodicReqInfo cqiPeriodicReqInfo;
}ULCQIReqInfo;
typedef struct ULCqiResInfoT
{
    ULReconfigCQIType cqiConfigMsg;
    /* CQI_PERIODIC_CONFIG_INFO */
    ULCQIPeriodicResInfo cqiPeriodicResInfo;
}ULCQIResInfo;

/****************** PUCCH_CONFIG_INFO *******************************/
typedef struct RRCInfPUCCHConfigReqInfoT
{
    ULReconfigPucchMsgType pucchConfigMsg;
    UInt32 switchPassiveToActiveFlag;
    UInt32 activeIndexForShortSRMap;
    UInt32 activeIndexForPUCCHMap;
    SRConfigInfo srConfigInfo;
    ULCQIReqInfo ulCqiReqInfo;
}RRCInfPUCCHConfigReqInfo;

typedef struct RRCInfPUCCHConfigResInfoT
{
    ULReconfigPucchMsgType pucchConfigMsg;
    ULReconfigCQIRIMsgType ulReconfigCqiRiMsg;
    ULReconfigSRMsgType ulReconfigSrMsg;
    MapReconfigSRMsgType  mapReconfigSrMsg;
    UInt32 switchActiveToPassiveFlag;
    ULCQIResInfo ulCqiResInfo;
}RRCInfPUCCHConfigResInfo;

/* CA changes Start */

typedef struct SCELLDeactivationT
{
	UInt8 scellDeactivationTimer;
}SCELLDeactivation;

typedef struct BandwidthDistributionT
{
	UInt8 bandWidthDistribution[MAX_SERVCELL];
}BandwidthDistribution;

typedef struct ULSCELLReconfigInfoT
{
    ULSCELLReconfigUeTLVs ueULCAScellInfoReconfigTlvRecieved[MAX_NUM_OF_SCELL_TLV];
    UInt8 ueULCAScellReconfigMsgTlvCount;
#ifdef HD_FDD_CONFIG
     /*RF_PARAM*/
     UInt8 rfParams;
#endif
    InternalCellIndex internalCellIndex;
    UInt8      sCellIndex;
    UInt8 operationType;
    UInt8      isScellActivateStatus;
     /* PUCCH_CONFIG_INFO */
    RRCInfPUCCHConfigReqInfo rrcInfpucchConfigReqInfoScell;
}ULSCELLReconfigInfo;

typedef struct ULCAConfigT
{
    ULCAReconfigUeTLVs ueULCAReconfigTlvRecieved[MAX_NUM_OF_CA_TLV];
    UInt8 ueULCAReconfigMsgTlvCount;
    SCELLDeactivation scellDeactivation;
    ULSCELLReconfigInfo scellReconfigInfoUL[MAX_NUM_SCELL];
}ULCAConfig;

/* CA changes End */


/********************NUM_OF_LAYER_INFO ******************/
typedef struct ULNumOfLayersInfoT
{
    UInt8 numOfLayers;
    /* Padding of 3 bytes */
    UInt8 padding[3];
}ULNumOfLayersInfo;



/********************UE_CATEGORY******************/
typedef struct ULUeCategoryT
{
    /* Padding of 1 bytes */
    UInt8 padding;
    UInt8 ueCategory;
    UInt16 maxTBSize;
}ULUeCategory;

/******************** SRS_DEDICATED_CONFIG *****************/
typedef struct ULSrsDedicatedConfigReqInfoT
{
    /*padding of 3 Byte */
    /* Enum to pass information about which map to configure at HP side*/
    MapReconfigMsgType    mapReconfigMsg;
    UInt8 padding[3];
    UInt8 srsConfigType ;
    UInt32 activeIndexForShortSRSMap;
    UInt32 activeIndexForLongSRSMap;
    UInt32 switchPassiveToActiveSRSFlag;
    /* SRS_DEDICATED_SETUP_INFO -optional*/
    SRSDedicatedSetupInfo srsDedicatedSetupInfo;
    UInt16 srsPeriodicity;
    UInt16 srsOffset;
    void *srsReportHeadNode_p;
}ULSRSDedicatedConfigReqInfo;

typedef struct ULSrsDedicatedConfigResInfoT
{
    MapReconfigMsgType mapReconfigMsg;
    UInt32             switchActiveToPassiveSRSFlag;
}ULSRSDedicatedConfigResInfo;


/****************** P0_UE_PUSCH *******************************/
typedef struct p0UePuschInfoT
{
   SInt8 p0UePusch;
   SInt8 prevPowerCorrectionForPusch;
   SInt8 currPowerCorrectionForPusch;
   SInt8 powerControlPuschTPCValue;

}P0UePuschInfo;

/********************* TPC_PDCCH_CONFIG_PUSCH ***********************/
typedef struct tpcPDCCHConfigPUSCHInfoT
{
    UInt8 release;
    UInt8 tpcIndex;
    UInt16 tpcRnti;

    /* Coverity 54156 Fix Start */
    DCIFormat isConfiguredForDci3Or3A;
    /* Coverity 54156 Fix End */

    /* padding of 3 bytes*/
    UInt8 padding[3];
}ULTPCPDCCHConfigPUSCHInfo;
/********************UL_RECONFIG_LC_REQ***********************/
typedef struct ULReconfigLcT
{
    UInt8 numLogicalChannels;
    ulLogicalChannelInfo lcInfo[MAX_NUMBER_OF_LOGICAL_CHANNEL];
}ULReconfigLc;

/*******************SPS ULInfo***********************/
typedef struct SpsUlInfoSplitT
{
    UInt8  requestType;
    /* SPS scheduling interval in UPlink.*/
    UInt8 semiPersistentSchedIntervalUL;
    /* Number of empty transmission allowed (explicit-release-after) for UL 
     * SPS occasion */
    UInt8 implicitReleaseAfter;
    /* Transmit power Claculation */
    p0Persistent p0PersistentInfo;
    /* present in case of TDD only TRUE/FALSE */
    UInt8 twoIntervalsConfig;
    /* Flag to check whether p0Persistent is present or not*/
    UInt8 isP0TagPresent;
    /* Flag to check whether twoIntervalsConfig is present or not*/
    UInt8 isTwoIntervalConfigPresent;
}SpsUlInfoSplit;

/*******************SPS DLInfo***********************/
typedef struct SpsDlInfoSplitT
{
    UInt8  requestType;
    /* SPS scheduling interval in Downlink.*/
    UInt8 semiPersistentSchedIntervalDL;
    /* Number of configured SPS processes.*/
    UInt8 numberOfConfSpsProcesses;
    /* Maximum number of SPS  harq retransmission. */
    UInt8 maxSpsHarqRetx;
    /* Number of empty transmission allowed (explicit-release-after) for DL 
     * SPS occasion */
    UInt8 explicitReleaseAfter;
    /* SPR 20456 start */
    /*code removed */
    /* SPR 20456 stop */
    /*  N1PucchAnPersistentList parameter */
    N1PucchAnPersistentList N1PucchAnPersistentListVal;
}SpsDlInfoSplit;

typedef struct ULInterferenceConfigT
{
/* EICIC_Coverity CID 69395 and 69394 Start */
     UserType userType;
 /* EICIC_Coverity CID 69395 and 69394 End */
    /* Padding of 3 bytes */
    UInt8 padding[3];
}ULInterferenceConfig;

/********************* UL_RECONFIG_UE ***********************/
        
typedef struct ULReconfigUeReqT
{
     UInt8 padding;
     UInt8 ueReconfigMsgTlvCount;

     UInt16 ueIndex;

     ULReconfigUeTLVs ueReconfigTlvRecieved[MAX_NUM_OF_TLV];

     /* PUCCH_CONFIG_INFO */
     RRCInfPUCCHConfigReqInfo rrcInfpucchConfigReqInfo;

     /* UE_CATEGORY */
     ULUeCategory ueCategoryInfo;
     
     /* SRS_DEDICATED_CONFIG*/
     ULSRSDedicatedConfigReqInfo ulSRSDedicatedConfigReqInfo;

     /* AMBR_QOS_INFO*/
/* L2_PERFORMANCE_OPT_CHANGES_START */
      UInt32 ulAmbr;
/* L2_PERFORMANCE_OPT_CHANGES_END */

     /* P0_UE_PUSCH */
     P0UePuschInfo p0UePuschInfo;

     /*TPC_PDCCH_CONFIG_PUSCH */
     ULTPCPDCCHConfigPUSCHInfo tpcPDCCHConfigPUSCHInfo;

     /* UL_RECONFIG_LC_REQ*/
     ULReconfigLc ulReconfigLcReq;

     /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
     /** RF_PARAM **/
     UInt8 rfParams;
#endif
     /*HD FDD Changes End*/
     SpsUlInfoSplit spsUlInfo;

    /*SPR21501 fix start+-*/
    UInt8 ttiBundlingEnabled;
     /*REL10_CR_CHANGES_START*/
     /* new valriables added for PUSCH config info */
     PuschConfig puschConfigInfo;
     /* Maximum number of transmissions for UL HARQ */
     UInt8 maxUlHARQTx;
     /*Configured UE transmitted power according to its power class*/
     SInt8 pcMax;
     /*REL10_CR_CHANGES_END*/

    /* CA changes Start */
    /* UE_CATEGORY */
    ULUeCategoryV1020 ueCategoryInfov1020;
    PucchFormatR10 pucchFormatR10;
    ULCAConfig caConfigUL;
    ULInterferenceConfig interferenceConfigUL;
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    UInt8 operatorId;
    /* MOCN Changes end */
    /* SPR 23402 Changes End */

}ULReconfigUeReq;

/********************* DL_CQI_INFO ***********************/
typedef struct DLCqiConfigInfoT
{
    UInt8 cqiMode;
    UInt8 modScheme;
    UInt8 cqiTypeIndicator;
    UInt8 padding[1];
    /*CA changes start*/
    UInt8     trigger1r10;
    UInt8     trigger2r10;
    /*CA changes end*/
}DLCQIConfigInfo;

/******************** DL_MAX_RB_INFO******************/
typedef struct DLMaxRBInfoT
{
    UInt8 dlMaxRB;
    /* Padding of 3 bytes */
    UInt8 padding[3];
}DLMaxRBInfo;

/******************** TX_MODE_INFO******************/
typedef struct DLTransModeT
{
    UInt8 transMode;
    /* Padding of 3 bytes */
    UInt8 padding[3];
}DLTransMode;

/********************NUM_OF_LAYER_INFO ******************/
typedef struct DLNumOfLayersInfoT
{
    UInt8 numOfLayers;
    /* Padding of 3 bytes */
    UInt8 padding[3];
}DLNumOfLayersInfo;


/********************CODEBOOK_INDEX_INFO ******************/
typedef struct CodeBookIndexInfoT
{
     UInt8 codeBookIndex;
    /* Padding of 3 bytes */
    UInt8 padding[3];
}CodeBookIndexInfo;


/********************UE_CATEGORY******************/
typedef struct DLUeCategoryT
{
    UInt8 ueCategory;
    UInt16 maxTBSize;   
    UInt16 maxSISOTBSize;
    UInt16 maxMIMOTBSize;
    /*Soft Buffer Changes Start*/
    UInt32 permissibleTBSize;
    UInt32 maxPermissibleTBSize;
    /*Soft Buffer Changes End*/
}DLUeCategory;

/* CA Changes Start */
typedef struct DLUeCategoryV1020T
{
    UInt8 ueCategoryV1020;
    UInt16 maxTBSize;   
    UInt16 maxSISOTBSize;
    UInt16 maxMIMOTBSize;
    /*Soft Buffer Changes Start*/
    UInt32 permissibleTBSize;
    UInt32 maxPermissibleTBSize;
    /*Soft Buffer Changes End*/
}DLUeCategoryV1020;

typedef enum DLCAReconfigUeTLVsT
{
    DL_RECONFIG_CA_CONFIG_SCELL_DEACTIVATION_TIMER = 0,
    DL_RECONFIG_CA_CONFIG_SCELL_INFO,
    DL_RECONFIG_CA_LC_BANDWIDTH_DIST_INFO
}DLCAReconfigUeTLVs;

typedef enum DLSCELLReconfigUeTLVsT
{
    DL_RECONFIG_SCELL_MIMO_CAPABILITY_V1020 = 0,
    DL_RECONFIG_SCELL_TRANS_MODE_INFO,
    DL_RECONFIG_SCELL_CODEBOOK_SUBSET_RESTRICTION_V1020,
    DL_RECONFIG_SCELL_PDSCH_CONFIG_INFO,    
    DL_RECONFIG_SCELL_CQI_INFO,
    DL_RECONFIG_SCELL_NUM_OF_LAYER_INFO,
    DL_RECONFIG_SCELL_CODEBOOK_INDEX_INFO,
    DL_RECONFIG_SCELL_RF_PARAMS,
    DL_RECONFIG_SCELL_ACTIVATE_STATUS
}DLSCELLReconfigUeTLVs;


/* CA Changes End */

/*****i**************DRX_CONFIG_INFO******************/
typedef struct DrxConfigInfoT
{
    UInt8           drxConfigType;
    UInt8           cqiMaskV920;
    UInt8           padding[2];
    DRXConfig       drxConfig;
}DRXConfigInfo;


/******************** MEAS_GAP_CONFIG_INFO ************************/
typedef struct DLmeasGapConfigInfoT
{
    UInt8 measGapConfigType;
    /*padding of 3 Bytes */
    UInt8 padding[3];
    /* MEAS_GAP_SETUP_CONFIG_INFO -optional*/
    MeasGapContextDL  measGapContextDL;
}DLMEASGapConfigInfo;

typedef struct ULmeasGapConfigInfoT
{
    UInt8 measGapConfigType ;
    /*padding of 3 Bytes */
    UInt8 padding[3];
    /* MEAS_GAP_SETUP_CONFIG_INFO -optional*/
    MeasGapContextUL  measGapContextUL ;
}ULMEASGapConfigInfo;

typedef struct RRCMeasGapConfigInfoT
{
    DLMEASGapConfigInfo dlMeasGapConfig;
    ULMEASGapConfigInfo ulMeasGapConfig;
    UInt32          activeIndexFor40MsMeasGapMap;
    UInt32          activeIndexFor80MsMeasGapMap;
#ifdef ENDC_ENABLED
	/*Meas_Gap_Changes_Start*/
    UInt32          activeIndexFor20MsMeasGapMap;
    UInt32          activeIndexFor160MsMeasGapMap;
    /*Meas_Gap_Changes_End*/
#endif
    /* SPR 17944 Start */
    //Code removed
    /* SPR 17944 End */
}RRCMeasGapConfigInfo;


/****************TIME_ALIGNMENT_TIMER_INFO **************/
typedef struct TimeAlignmentTimerInfoT
{
    UInt32 ulSyncTimer;
    UInt32 syncTimerDelta;
}TimeAlignmentTimerInfo;

/**************** CODEBOOK_SUBSET_RESTRICTION_V920 *****************/
typedef struct codebookSubsetRestrictionv920InfoT
{
    UInt8 CbsrValue[MAX_CBSR_SIZE];   /* MAX_CBSR_SIZE = 8*/
    UInt8 CbsrType;
    /*padding of 3 Bytes */
    UInt8 padding[3];
}CBSRestrictionv920Info;


/********************* PDSCH_CONFIG_INFO ***********************/
typedef struct PDSCHConfigInfoT
{
     UInt8 dlPdschTransPower;
    /* padding of 3 bytes*/
    UInt8 padding[3];
}PDSCHConfigInfo;
/********************* TPC_PDCCH_CONFIG_PUCCH ***********************/
typedef struct tpcPDCCHConfigPUCCHInfoT
{
    UInt8 release;
    UInt8 tpcIndex;
    UInt16 tpcRnti;
    /* Coverity 54156 Fix Start */
    DCIFormat isConfiguredForDci3Or3A;
    /* Coverity 54156 Fix End */
    /* padding of 3 bytes*/
    UInt8 padding[3];
}DLTPCPDCCHConfigPUCCHInfo;

/****************** AMBR_QOS_INFO *******************************/
typedef struct ambrQosInfoT
{
/* L2_PERFORMANCE_OPT_CHANGES_START */
    UInt32 DLAMBR;              /* Converted to UInt32 to UInt64*/
/* L2_PERFORMANCE_OPT_CHANGES_END */
}DLAMBRQosInfo;

/********************DL_CREATE_LC_REQ***********************/
typedef struct DLCreateLcReqT
{
    UInt8 lcCount;
    UInt8 lcId[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    UInt8 maxLCId;

}DLCreateLcReq;

/********************DL_RECONFIG_LC_REQ***********************/
typedef struct DLReconfigLcReqT
{
    UInt8 lcCount;
    /* padding of 3 bytes*/
    UInt8 padding[3];
    LogicalChannelConfigInfo logicalChConfig[10];
}DLReconfigLc;
typedef enum DLReconfigUeTLVsT
{
    DL_RECONFIG_CQI_INFO = 0,
    DL_RECONFIG_MAX_RB_INFO,
    DL_RECONFIG_TRANS_MODE_INFO, 
    DL_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_INFO,
    DL_RECONFIG_NUM_OF_LAYERS_INFO,
    DL_RECONFIG_CODEBOOK_INDEX_INFO,
    DL_RECONFIG_UE_CATEGORY,
    DL_RECONFIG_DRX_CONFIG_INFO,
    DL_RECONFIG_MEAS_GAP_CONFIG_INFO,
    DL_RECONFIG_TIME_ALIGNMENT_TIMER_INFO,
    DL_RECONFIG_PDSCH_CONFIG_INFO,
    DL_RECONFIG_TPC_PDCCH_CONFIG_PUCCH,
    DL_RECONFIG_AMBR_QOS_INFO,
    DL_RECONFIG_CONFIGURE_LC_REQ,
    /** HD FDD Changes **/
    DL_RECONFIG_RF_PARAMS,
    /** HD FDD Changes **/
   DL_RECONFIG_SPS_PARAMS,
   /* CA changes Start */
    DL_RECONFIG_UE_CATEGORY_V1020,
    DL_RECONFIG_PUCCH_CONFIG_INFO_V1020,
    DL_RECONFIG_CA_CONFIG_INFO,
    DL_RECONFIG_INTERFERENCE_INFO
   /* CA changes End */
    /* SPR 23402 Changes Start */ 
    /* MOCN Changes Start */
    ,DL_RECONFIG_OPERATOR_ID_INFO
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
}DLReconfigUeTLVs;
typedef enum RRCConfigureDlUEMsgT
{
    RRC_CONFIGURE_NO_DL_MAP = 0,
    RRC_CONFIGURE_MEAS_GAP_80MS_MAP,
    RRC_CONFIGURE_MEAS_GAP_40MS_MAP,
    RRC_CONFIGURE_DRX_MAP = 4,
    RRC_CONFIGURE_DRX_MEAS_GAP_80MS_MAP,
    RRC_CONFIGURE_DRX_MEAS_GAP_40MS_MAP
#ifdef ENDC_ENABLED
	/*Meas_Gap_Changes_Start*/
	,RRC_CONFIGURE_MEAS_GAP_20MS_MAP=16,
	RRC_CONFIGURE_MEAS_GAP_160MS_MAP=32
	/*Meas_Gap_Changes_End*/
#endif
}RRCConfigureDlUEMsg;

typedef struct DLPaInfoT
{
    UInt8 pA;
}DLPaInfo;


/*****************DL_RECONFIG_UE_RESPONSE **********/
typedef struct DLReconfigUeRespT
{
    MacRetType      respCode;
    RRCConfigureDlUEMsg   reconfigUEMsg;
    UInt16          ueIndex;
    UInt8           padding[2];
    DLReconfigLc dlReconfigLcResp;
    /*CA changes start*/
    CarrierAggrConfig carrierAggrConfig;
}DLReconfigUeResp;

typedef struct DLSCELLReconfigInfoT
{
    DLSCELLReconfigUeTLVs ueDLCAScellInfoReconfigTlvRecieved[MAX_NUM_OF_SCELL_TLV];
    UInt8 ueDLCAScellReconfigMsgTlvCount;
#ifdef HD_FDD_CONFIG
     /*RF_PARAM*/
     UInt8 rfParams;
#endif
    /* TX_MODE_INFO */
    DLCQIConfigInfo dlCqiConfigInfo;
    UInt8      sCellIndex;
    InternalCellIndex internalCellIndex;
    UInt8      isScellActivateStatus;
    DLTransMode scellTransMode;
    CodebookSubsetRestrictionv1020 codebookSubsetRestrictionv1020;
    CqiInfo   cqiInfo;
    CodeBookIndexInfo codeBookIndexInfo;
    DLNumOfLayersInfo numOfLayersInfoDL;
    UInt8 operationType;
    UInt8 numOfLayerMIMO;
    /*PDSCH_CONFIG_INFO */
    PDSCHConfigInfo scellPdschConfigInfo;
}DLSCELLReconfigInfo;

typedef struct DLCAConfigT
{
    DLCAReconfigUeTLVs ueDLCAReconfigTlvRecieved[MAX_NUM_OF_CA_TLV];
    UInt8 ueDLCAReconfigMsgTlvCount;
    SCELLDeactivation scellDeactivation;
	BandwidthDistribution dlBandWidthDistribution;
    DLSCELLReconfigInfo scellReconfigInfoDL[MAX_NUM_SCELL];
}DLCAConfig;

/*CA changes end*/

typedef struct DLInterferenceConfigT
{
    /* EICIC_Coverity CID 69393 and 69392 Start */
    UserType userType;
    /* EICIC_Coverity CID 69393 and 69392 End */
    /* Padding of 3 bytes */
    UInt8 padding[3];
}DLInterferenceConfig;
/*****************DL_RECONFIG_UE_REQUEST **********/

typedef struct DLReconfigUeReqT
{
     UInt32 ueReconfigTlvRecieved[MAX_NUM_OF_TLV];      /* MAX_NUM_OF_TLV will be 15 (32 bit align)*/
     UInt8 ueReconfigMsgTlvCount;

     /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
     /*RF_PARAM*/
     UInt8 rfParams;
#endif
     /*HD FDD Changes End*/

     UInt16 ueIndex;
     /* CQI_INFO */
     DLCQIConfigInfo dlCqiConfigInfo;

     /* DL_MAX_RB_INFO */
     DLMaxRBInfo dlMaxRBInfo;

     /* TX_MODE_INFO */
     DLTransMode transMode;

     /* CODEBOOK_SUBSET_RESTRICTION */
     codebookSubsetRestrictionInfo cbsrInfo;

     /* NUM_OF_LAYER_INFO*/
     DLNumOfLayersInfo numOfLayersInfo;

     /* CODEBOOK_INDEX_INFO */
     CodeBookIndexInfo codeBookIndexInfo;

     /* UE_CATEGORY */
     DLUeCategory ueCategoryInfo;

     /* DRX_CONFIG_INFO */
     DRXConfigInfo   drxConfigInfo;

     /* MEAS_GAP_CONFIG_INFO */
     RRCMeasGapConfigInfo rrcMeasGapConfig;
     /* TIME_ALIGNMENT_TIMER_INFO */
     TimeAlignmentTimerInfo timeAlignmentTimerInfo;

     /*PDSCH_CONFIG_INFO */
     PDSCHConfigInfo pdschConfigInfo;

     /*TPC_PDCCH_CONFIG_PUCCH */
     DLTPCPDCCHConfigPUCCHInfo tpcPDCCHConfigPUCCHInfo;

     /* AMBR_QOS_INFO*/
     DLAmbrInfo dlAmbrInfo;

     /*DL_CONFIGURE_LC_REQ */
     DLReconfigLc dlReconfigLcReq;

    /*DL_SPS_INFO*/
    SpsDlInfoSplit spsDlInfo;

    /* CA changes Start */
    /* UE_CATEGORY */
    DLUeCategoryV1020 ueCategoryInfov1020;
    PucchFormatR10 pucchFormatR10;
    DLCAConfig caConfigDL;
    DLInterferenceConfig interferenceConfigDL;
    /* CA changes End */
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    UInt8 operatorId;
    /* MOCN Changes end */
    /* SPR 23402 Changes End */

}DLReconfigUeReq;


/**************************CREATE_UE_REQ**************************/
typedef enum hiPrioThdT
{
    DL_THD = 0,
    UL_THD,
    RACH_MGRQ,
    MAX_THD
}hiPrioThd_en;
typedef enum RRCConfigureUlUEMsgT
{ 
    RRC_CONFIGURE_NO_UL_MAP = 0,               
    RRC_CONFIGURE_SHORT_SR_MAP,
    RRC_CONFIGURE_PUCCH_MAP,
    RRC_CONFIGURE_PUCCH_SHORT_SR_MAP,
    RRC_CONFIGURE_SHORT_SRS_MAP,
    RRC_CONFIGURE_SHORT_SR_SHORT_SRS_MAP,
    RRC_CONFIGURE_PUCCH_SHORT_SRS_MAP,
    RRC_CONFIGURE_PUCCH_SHORT_SR_SHORT_SRS_MAP,
    RRC_CONFIGURE_LONG_SRS_MAP,
    RRC_CONFIGURE_SHORT_SR_LONG_SRS_MAP,
    RRC_CONFIGURE_PUCCH_LONG_SRS_MAP,
    RRC_CONFIGURE_PUCCH_SHORT_SR_LONG_SRS_MAP
}RRCConfigureUlUEMsg;

/* following structure pointer would be sent to UL High priority thread 
 *    For create UE request */
typedef struct ULCreateUeReqT
{
    RRCConfigureUlUEMsg  createUEMsg;
    UInt16          ueIndex;
    UInt8           padding[2];
    UInt32          activeIndexForPUCCHMap;       
    UInt32          activeIndexForShortSRMap;
    UInt32          activeIndexForLongSRSMap;
    UInt32          activeIndexForShortSRSMap;
}ULCreateUeReq;

/* following structure pointer would be sent to DL High priority thread 
 *    For create UE request */
typedef struct DLCreateUeReqT
{
    RRCConfigureDlUEMsg   createUEMsg;
    UInt16          ueIndex;
    UInt8           padding[2];
    DRXConfigInfo   drxConfigInfo;
    UInt32          activeIndexFor40MsMeasGapMap;
    UInt32          activeIndexFor80MsMeasGapMap;
#ifdef ENDC_ENABLED
	/*Meas_Gap_Changes_Start*/
    UInt32          activeIndexFor20MsMeasGapMap;
    UInt32          activeIndexFor160MsMeasGapMap;
    /*Meas_Gap_Changes_End*/
#endif

}DLCreateUeReq;

/* following structure pointer would be sent from UL High priority thread 
 *    For create UE response */
typedef struct ULCreateUeRespT
{
    MacRetType      respCode;
    RRCConfigureUlUEMsg   createUEMsg;
    UInt16          ueIndex;
    UInt8           padding[2];
}ULCreateUeResp;

/* following structure pointer would be sent from UL High priority thread 
 *    For Reconfig UE response */
typedef struct ULReconfigUeRespT
{
    MacRetType      respCode;
    RRCConfigureUlUEMsg   reconfigUEMsg;
    UInt16          ueIndex;
    RRCInfPUCCHConfigResInfo rrcInfpucchConfigResInfo;
    ULSRSDedicatedConfigResInfo ulSRSDedicatedConfigResInfo;
    ULReconfigLc    ulReconfigLcResp;
/** SPR 12320 Fix : START **/
    /*SPR 16855 Fix Start*/
    /* TDD SB_CQI */
    pucchNodeForReport pucchNodesForCqi[MAX_NUM_MAPS];
    pucchNodeForReport pucchNodesForWbCqi[MAX_NUM_MAPS];
    pucchNodeForReport pucchNodesForRI[MAX_NUM_MAPS];
    pucchNodeForReport pucchNodesForCqi2[MAX_NUM_MAPS];
    pucchNodeForReport pucchNodesForWbCqi2[MAX_NUM_MAPS];
    pucchNodeForReport pucchNodesForRI2[MAX_NUM_MAPS];
    /* TDD SB_CQI */
    /*SPR 16855 Fix End*/
/** SPR 12320 Fix : END **/
}ULReconfigUeResp;

/* following structure pointer would be sent from DL High priority thread 
 *    For create UE response */
typedef struct DLCreateUeRespT
{
    MacRetType      respCode;
    RRCConfigureDlUEMsg   createUEMsg;   
    UInt16          ueIndex;
    UInt8           padding[2];
}DLCreateUeResp;

/*****************UL_DELETE_UE_REQUEST **********/
typedef struct ULDeleteUeReqT
{
  RRCConfigureUlUEMsg  deleteUEMsg;
  UInt16               ueIndex;
  UInt8                padding[2];
  UInt32               activeIndexForPUCCHMap;       
  UInt32               activeIndexForShortSRMap;
  UInt32               activeIndexForLongSRSMap;
  UInt32               activeIndexForShortSRSMap;
}ULDeleteUeReq;

/*****************UL_DELETE_UE_RESPONSE **********/
typedef struct ULDeleteUeRespT
{
    MacRetType            respCode;
    RRCConfigureUlUEMsg   deleteUEMsg;
    /*REL10_CR_CHANGES_START*/
    UInt8 deleteLCStatus[MAX_NUMBER_OF_LOGICAL_CHANNEL];   
    /*REL10_CR_CHANGES_END*/
}ULDeleteUeResp;

/*****************DL_DELETE_UE_REQUEST **********/
typedef struct DLDeleteUeReqT
{
  RRCConfigureDlUEMsg  deleteUEMsg;
  UInt16               ueIndex;
  UInt8                padding[2];
  UInt32               activeIndexFor40MsMeasGapMap;
  UInt32               activeIndexFor80MsMeasGapMap;
#ifdef ENDC_ENABLED
	/*Meas_Gap_Changes_Start*/
    UInt32          activeIndexFor20MsMeasGapMap;
    UInt32          activeIndexFor160MsMeasGapMap;
    /*Meas_Gap_Changes_End*/
#endif

}DLDeleteUeReq;

/*****************DL_DELETE_UE_RESPONSE **********/
typedef struct DLDeleteUeRespT
{
    MacRetType            respCode;
    RRCConfigureDlUEMsg   deleteUEMsg;
    /*REL10_CR_CHANGES_START*/
    UInt8 deleteLCStatus[MAX_NUMBER_OF_LOGICAL_CHANNEL];   
    /*REL10_CR_CHANGES_END*/
}DLDeleteUeResp;

typedef struct ULResetUeReqT
{
    UInt16 ueIndex;
    UInt8 pendingDeleteFlag;
    /* CA changes start */
    UInt8 scellRelFlag;
    /* CA changes end */
}ULResetUeReq;

typedef struct DLResetUeReqT
{
    UInt16 ueIndex;
    UInt8 pendingDeleteFlag;
    /* CA changes start */
    UInt8 scellRelFlag;
    /* CA changes end */
}DLResetUeReq;

typedef struct DLResetUeRespT
{
    MacRetType respCode;
    UInt16 ueIndex;
}DLResetUeResp;

typedef struct ULResetUeRespT
{
    MacRetType respCode;
    UInt16 ueIndex;
}ULResetUeResp;

typedef union RrcOamReqRespT
{
    DLCreateUeReq      dlCreateUeReq;
    ULCreateUeReq      ulCreateUeReq;
    DLCreateUeResp     dlCreateUeResp;
    ULCreateUeResp     ulCreateUeResp;
    HORachInfo         hoRachInfo;
    DLReconfigUeReq    dlReconfigUeReq;
    DLReconfigUeResp   dlReconfigUeResp;
    ULReconfigUeReq    ulReconfigUeReq;
    ULReconfigUeResp   ulReconfigUeResp;
    DLDeleteUeReq      dlDeleteUeReq;
    DLDeleteUeResp     dlDeleteUeResp;
    ULDeleteUeReq      ulDeleteUeReq;
    ULDeleteUeResp     ulDeleteUeResp;
    DLResetUeReq       dlResetUeReq;
    ULResetUeReq       ulResetUeReq;
    DLResetUeResp      dlResetUeResp;
    ULResetUeResp      ulResetUeResp;
}RrcOamReqResp;

typedef struct RrcOamHiPrioQNodeT
{
    MacProcessRrcOamMsgEvent eventId;
    UInt32                   respCode;
    UInt16                   rrcMsgId;
    UInt16                   ueIndex;
    RrcOamReqResp            rrcOamReqResp;
}*LP_RrcOamHiPrioQNode, RrcOamHiPrioQNode;

/*Reconfig Changes Start */
typedef struct UeContextForReconfigT
{
    UInt32    cqiOffset;
    UInt32    cqiPeriodicity;
    UInt32    riOffset;
    UInt32    riPeriodicity;
    UInt32    srOffset;
    UInt32    srPeriodicity;
    UInt32    srsOffset;
    UInt32    srsPeriodicity;
    UInt32    cqiRiReconfigBitMap;
    UInt8    srReconfigBitMap;
    UInt8    srsReconfigBitMap;
}UeContextForReconfig;

/* Structure is used for executing pending request after getting confirmation
 * from HP */
typedef struct RrcOamPendingReqT
{
    UeContextForDownlink     dlCreateUEEntityReq;
    UeContextForUplink       ulCreateUEEntityReq;
    UInt32                   deleteDLUEBitMap;
    UInt32                   deleteULUEBitMap;
    UInt16                   transactionId;
    UInt16                   rrcUeIndex;
    UInt16                   response;
    /* respMsgId will be used to identify whether reset ue entity
     * is triggered by MAC_CHANGE_CRNTI_REQ or MAC_RESET_UE_ENTITY_REQ */
    UInt16                   respMsgId; 
    UInt16                   SFN;
    UInt16                   SF;
    UInt8                    spsCrntiFlag;
    UInt8                    gapPatternId;
    UInt8                    measGapBitMask;
    UInt8                    dlscellRelFlag;
    UeContextForReconfig     reconfigInfo;
    /* SPR 9385 changes start */
    ChangeCrntiInfo          changeCrntiInfo;
    /* SPR 9385 changes end */
    /* SPR 16082 fix start */
    MeasGapContextDL dlMeasGapContext;
    MeasGapContextUL ulMeasGapContext;
    UInt8  dlMeasGapConfigType;
    UInt8  ulMeasGapConfigType;
    /* SPR 16082 fix end */
} *LP_RrcOamPendingReq, RrcOamPendingReq;
/*Reconfig Changes End */

/* Structure containing the current state and other structures to configure
 * the messages received from RRC interface */
typedef struct MacRrcOamMsgHandlerT
{
    /* Current state of RRC interface */
    MacProcessRrcOamMsgState currentState;
    /* Bitmask will be set when message is received from the HP thread */
    MacRespRcvThrdId         bitMaskRecvd;
    /* Bitmask will be set when message is sent to the HP thread */
    MacRespRcvThrdId         bitMaskSent;    
    /* Return code from the HP thread */
    MacRetType               retCode;
    /* Temporary context after parsing of message */
    LP_RrcOamPendingReq      rrcOamPendReq_p;
    /* DL Data to sent to/received from HP thread */
    LP_RrcOamHiPrioQNode     dlData_p;
    /* UL Data to sent to/received from HP thread */
    void                     *ulData_p;
} *LP_MacRrcOamMsgHandler, MacRrcOamMsgHandler;

/* Structure used for storing the message in queue to process it later */
typedef struct RrcOamLPPendingNodeT
{
    UInt16                  ueIndex;
    /* Event from RRC which is not processed */
    MacProcessRrcOamMsgEvent eventId;
    /* Result of parsing of a message */
    LP_RrcOamPendingReq      rrcOamPendReq_p;
    /* DL Data to sent to/received from HP thread */
    LP_RrcOamHiPrioQNode     dlData_p;
    /* UL Data to sent to/received from HP thread */
    void                     *ulData_p;
} *LP_RrcOamLPPendingNode, RrcOamLPPendingNode;

/* Queue for pending OAM-RRC messages */
extern RRC_OAM_PENDING_QUEUE_TYPE rrcOamPendingQ_g[MAX_NUM_CELL];

extern MacRetType createUEULConfigCnf( ULCreateUeResp *ulConfigUeResp_p,
                                       UeContextForDownlink  *dwlkUEInfo_p,
                                       UeContextForUplink  *uplkUEInfo_p ,
                                       UInt32 currSFN, UInt32 currSF,
                                       InternalCellIndex internalCellIndex
                                       ); 

extern MacRetType createUEDLConfigCnf (DLCreateUeResp *dlCreateUeResp_p,
                                      UeContextForDownlink  *dwlkUEInfo_p
                                      ,InternalCellIndex internalCellIndex
                                      );

extern MacRetType reconfigUEULConfigCnf(ULReconfigUeResp *ulReconfigUeResp_p,
                                        UeContextForDownlink  *dwlkUEInfo_p,
                                        UeContextForUplink  *uplkUEInfo_p,
                                        UInt32 currSFN, UInt32 currSF,
                                        UeContextForReconfig *reconfigInfo_p,
                                        InternalCellIndex internalCellIndex);

extern MacRetType reconfigUEDLConfigCnf(DLReconfigUeResp *dlReconfigUeResp_p,
                                        UeContextForDownlink  *dwlkUEInfo_p,
                                        /* SPR 16082 fix start */
                                        UInt8 dlMeasGapConfigType,
                                        UInt8 ulMeasGapConfigType,
                                        MeasGapContextDL *tempDLMeasGapContextInfo,
                                        MeasGapContextUL *tempULMeasGapContextInfo,
                                        /* SPR 16082 fix end */
                                        UInt8 gapPatternId,UInt8 *gapBitMap,
                                        /*CA Changes start */
                                        InternalCellIndex internalCellIndex);
                                        /*CA Changes end */

extern UInt32 ( *dlApiHandlerFunc[MAC_MAX_API]) (RrcOamHiPrioQNode *, RrcOamHiPrioQNode *, InternalCellIndex);
extern UInt32 ( *ulApiHandlerFunc[MAC_MAX_API]) (RrcOamHiPrioQNode *, RrcOamHiPrioQNode *, InternalCellIndex);
/* CA changes start */
extern MacProcessRrcOamMsgEvent rrcOamDlEventId_g[MAX_NUM_CELL];
extern MacProcessRrcOamMsgEvent rrcOamUlEventId_g[MAX_NUM_CELL];
extern MacRrcOamMsgHandler rrcOamMsghandler[MAX_NUM_CELL];
/* CA changes end */
UInt32 dlConfigCellReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);
UInt32 dlSFNReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);
UInt32 dlReconfigCellReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *); 
UInt32 dlCreateUeReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *, InternalCellIndex );       
UInt32 dlDeleteUeReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *, InternalCellIndex);      
UInt32 dlReconfigUeReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *, InternalCellIndex);   
UInt32 dlDRXCmdReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);      
UInt32 dlBcchReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);      
UInt32 dlPcchReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);       
UInt32 dlCcchReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);     
UInt32 dlUeConRejReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);       
UInt32 dlDeleteCellReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);    
UInt32 dlHoRachResReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);    
/* +- SPR 19066 */ 
UInt32 dlResetUeReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *,InternalCellIndex);         
UInt32 dlChangeCrntiReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);    
UInt32 ulConfigCellReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);     
UInt32 ulSFNReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);            
UInt32 ulReconfigCellReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);         
UInt32 ulCreateUeReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *,InternalCellIndex);       
UInt32 ulDeleteUeReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *,InternalCellIndex);      
UInt32 ulReconfigUeReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *, InternalCellIndex);    
UInt32 ulDRXCmdReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);        
UInt32 ulBcchReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);        
UInt32 ulPcchReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);       
UInt32 ulCcchReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);         
UInt32 ulUeConRejReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);      
UInt32 ulDeleteCellReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);     
UInt32 ulHoRachResReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);      
/* +- SPR 19066 */ 
UInt32 ulResetUeReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *,InternalCellIndex);        
UInt32 ulChangeCrntiReqHPHandler(RrcOamHiPrioQNode *, RrcOamHiPrioQNode *);    

/****************************************************************************
 * Function Name  : rrcOamCheckHiPrioResp
 * Inputs         : None 
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API will check response fropm HP Thread  
 ****************************************************************************/
void rrcOamCheckHiPrioResp(InternalCellIndex internalCellIndex );

/* This function frees for any entry that is present in RRC mesaage pending 
 * queue. */
void rrcMsgFreePendingQueue(InternalCellIndex internalCellIndex);

/* This method resets the state machine. */
void rrcMsgResetStateMachine(InternalCellIndex internalCellIndex);

/* This function will free all the allocated request buffers for HP thread. */
void rrcFreeReqBuffer(InternalCellIndex internalCellIndex);
/*SPR 16855 +-*/
/* SPR 9385 changes start*/
void  changeCrntiAndResetSplitUeEntity( UInt16 ueIndex , UInt16 msgId,
    UInt16 transactionId,ChangeCrntiInfo *changeCrntiInfo_p,
    InternalCellIndex internalCellIndex);
void changeCrntiReqHandler(UInt16 transactionId,ChangeCrntiInfo changeCrntiInfo,
        InternalCellIndex internalCellIndex);
/* SPR 9385 changes end*/


typedef struct MacPendingRrcOamMsgNodeT
{
    LTE_SNODE   anchorNode;
    void * msg_p;
    SInt32 msgLenth;
    UInt32 delayToPhy;
}MacPendingRrcOamMsgNode;
      
/* SPR 7435 Changes Starts */   
 SInt32 sendToRRC( UeContextForDownlink *dlUEInfo,
/* SPR 7435 Changes Ends */   
        UeContextForUplink *ulUEInfo,
        /* Coverity 530 CID 36230 */
        UInt32 msgId,
        /* Coverity 530 CID 36230 */
        void *data_p
        /* CA changes Start */
        ,RrcCellIndex cellIndex
        /* CA changes End */
        /*SPR 16855 Fix Start*/
        , ULReconfigUeResp *ulReconfigUeResp_p,
        DLReconfigUeResp *dlReconfigUeResp_p
        /*SPR 16855 Fix End*/
        );

#ifdef PERF_STATS
void initPerfStatsPdcchPdschInfo(InternalCellIndex internalCellIndex);
#endif 
/* CA Changes start */
InternalCellIndex macCommonUpdateCellIndexMapping(RrcCellIndex cellIndex);
UInt8  macCommonLookupIfValidCellIndex(RrcCellIndex cellIndex);
RrcCellIndex macCommonGetRRCCellIndex(InternalCellIndex internalCellIndex);
InternalCellIndex macCommonGetInternalCellIndex(RrcCellIndex rrcCellIndex);
void macCommonDeleteCellIndexMapping(RrcCellIndex cellIndex);
void macCommonRevertCellIndexMapping(InternalCellIndex internalIndex);
/* +- SPR 18268 */
void insertNodeInDlCAEventQueueRRCIntf(InternalCellIndex sCellIndex,UInt8 isScellActivateStatus,UInt16 ueIndex,InternalCellIndex internalCellIndex);
/* +- SPR 18268 */
MacRetType macStopReqFreeMemToPools(UInt16 msgId, InternalCellIndex internalCellIndex);
MacRetType macDeleteReqFreeMemToPools(UInt16 msgId, InternalCellIndex internalCellIndex);
#ifdef FDD_CONFIG
MacRetType macParseCAUECqiPMIConfigIndexV1020_dummy(UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        UInt8 transmissionMode,
        InternalCellIndex internalCellIndex
        );
MacRetType  macParseCAReconfigUECqiPMIIndexV1020_dummy(UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        UInt8 transmissionMode,
        InternalCellIndex internalCellIndex,
        ULCQIReqInfo *ulCqiReqInfo_p
        );
void macProcessAndUpdateULReconfigScellInfo_dummy(UInt16 ueIndex,
     ULReconfigUeReq *ulReconfigUeReq_p, ULUEContext *ulUEContext_p,
     UInt8 cellCount,
     ULReconfigUeResp *ulReconfigUeResp_p,
     InternalCellIndex internalCellIndex
     );

void macPopulateScellCqiPeriodicInfo_dummy( PucchConfig *pucchConfig_p,ULCQIPeriodicReqInfo *cqiPeriodicReqInfo_p
        , InternalCellIndex internalCellIndex
        );

/* SPR 16916 fix start */
#ifdef KPI_STATS
void updateNumOfActiveUeDlOnUeDelete(UInt16 ueIndex,InternalCellIndex internalCellIndex);
#endif
#endif
/* SPR 16916 fix end */
UInt32 macSendRrcPhyFailResponse
(
    UInt16 msgId,
    UInt16 transactionId,
    RrcCellIndex cellIndex
);
UInt32 macSendRrcFailResponse
(
    UInt16 msgId,
    UInt16 transactionId,
    RrcCellIndex cellIndex
);

/* CA Changes end */

/* SPR 21819 changes */
void macUpdateControlStateOnSRB1DLPacket (UInt16 ueIndex);
/* SPR 21819 changes */

#endif  /* LTEMAC_PARSE_UTIL_H */
