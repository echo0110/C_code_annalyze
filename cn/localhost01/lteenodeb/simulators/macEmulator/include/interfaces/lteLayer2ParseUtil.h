/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2ParseUtil.h,v 1.1.1.1.6.2.8.1 2010/09/21 15:53:24 gur20491 Exp $
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
 * $Log: lteLayer2ParseUtil.h,v $
 * Revision 1.1.1.1.6.2.8.1  2010/09/21 15:53:24  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.2  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.10  2009/08/20 07:38:46  gur19413
 * added new macro for CQI Info
 *
 * Revision 1.9  2009/07/27 14:57:42  gur20439
 * delete UE support.
 *
 * Revision 1.8  2009/07/17 08:57:53  gur20439
 * MAX_DATA_SIZE macro modified to 4096.
 *
 * Revision 1.7  2009/07/17 06:25:47  gur20439
 * changes related to rrc interface modification incorporated.
 *
 * Revision 1.6  2009/07/16 06:36:07  gur15697
 * bug fix, mac emulator cell config
 *
 * Revision 1.5  2009/07/14 14:45:02  gur20439
 * rrc interface code.
 *
 * Revision 1.4  2009/06/16 14:43:17  gur20439
 * MAC_MODULE_ID modified to 3 to communicate with latest OAM simulator.
 *
 * Revision 1.3  2009/06/15 13:52:01  gur20439
 * modifed for release 1.2.
 *
 * Revision 1.2  2009/04/03 13:40:41  gur20439
 * Macro added to handle sendModuleLogLevelReq.
 *
 * Revision 1.1  2009/03/30 10:20:27  gur19836
 * Initial Version
 *
 * Revision 1.4  2009/03/23 08:32:30  gur19140
 * Integration changes first waves
 *
 * Revision 1.3  2009/03/13 08:00:01  gur19836
 * Modified for OAM and RLC Interface Changes
 *
 * Revision 1.2  2009/01/21 13:58:10  gur19836
 * Error code MAC_CELL_NOT_CONFIGURED added
 *
 * Revision 1.1  2009/01/20 07:51:08  gur19836
 * Initial Version
 *
 *
 ****************************************************************************/

#ifndef LTELAYER2_PARSE_UTIL_H
#define LTELAYER2_PARSE_UTIL_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteLayer2Types.h"
#include "lteMemPool.h"
#include <stdio.h>
#include "lteLayer2RRCInterface.h"
#include "lteLayer2UplinkUEContext.h"
#include "lteLayer2UEContext.h"
#include "lteFapiMacPhyInterface.h"

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
#define MAX_DATA_SIZE          4096 
#define MSGID_SIZE             1
#define IDX_SIZE               2
#define UEID_SIZE              2
#define RSP_SIZE               1
#define PWR_HR_SIZE            2

#define MAC_MODULE_ID          7
#define OAM_MODULE_ID          1
#define RRC_MODULE_ID          3
/* + CA_AB */
#define CELL_INDEX_LENGTH      1
/* UE_TESTING */
//#define MAC_API_HEADER_LEN     10U
/* need to uncomment */
//#define MAC_API_HEADER_LEN     (10 + CELL_INDEX_LENGTH)U
#define MAC_API_HEADER_LEN     16
/* - CA_AB */
#define MAC_PARTIAL_SUCCESS    2

/* Tags */
//#define UL_LC_CREATE_REQ        11
#define UL_LC_DELETE_REQ        12
//#define DL_LC_CREATE_REQ        15
#define DL_LC_DELETE_REQ        16

#define TPC_PUCCH_RNTI          26
#define TPC_PUSCH_RNTI          27

//#define ADD_UE_INFO             28
//#define CQI_INFO                12

/* Error Codes */
#define MAC_INVALID_UE_ID       3
#define MAC_UE_ID_EXISTS        4
#define MAC_UE_NOT_EXISTS       5
#define MAC_MAX_ACTIVE_UE_REACHED_ERR       6
#define MAC_CELL_NOT_CONFIGURED 7

#define MAC_INTERNAL_ERROR      11
#define MAC_SYNTAX_ERROR        12

#define MAC_LOG_CH_ERROR        30
#define MAC_LOG_CH_NOT_EXISTS   31
#define MAC_MAX_ULLOG_CH_CONFIG 32
#define MAC_MAX_DLLOG_CH_CONFIG 33
#define MAC_LOG_CH_EXISTS       34
#define MAC_NO_ACTIVE_UE_IN_SYSTEM 35

#define CQI_APERIODIC_INFO 0x32
#define CQI_PERIODIC_INFO 0x33

/* + CQI_4.1 */
#define CQI_FORMAT_PERIODIC_SUBBAND_K          39
#define RI_CONFIG_INDEX_INFO                   0x34 
#define MAC_MAX_CQI_PMI_CONFIG_INDEX           541
#define MAC_MAX_CQI_PMI_CONFIG_INDEX_3_1_7     317
#define MAC_MAX_CQI_PMI_CONFIG_INDEX_TDD       315
#define MAC_MAX_CQI_PUCCH_RESOURCE_INDEX       1185 
#define SIMULTANEOUS_ACK_NACK_CQI_0            0
#define SIMULTANEOUS_ACK_NACK_CQI_1            1
#define MAC_MAX_RI_CONFIG_INDEX                965 
#define INVALID_CQI_REPORT_MODE                255 
#define MAC_RI_CONFIG_INDEX_INVALID            1000 
/* - CQI_4.1 */

#ifdef UESIM_ENHANCEMENTS
#define MAX_RBs 100
#endif
/* + TM7_8 Changes Start */
#define CQI_REPORT_CONFIG_V920  0x5C
#define CQI_MASK_V920           0x5D
#define PMI_RI_REPORT_V920      0x5E
#define CQI_REPORT_V920_LEN     9
#define INVALID_PMI_RI_CONF_V920 255
/* - TM7_8 Changes End */
/* + SPR_8414 */
#define INVALID_REQUEST_TYPE 255
/* - SPR_8414 */
/* API Ids */
/* EICIC +*/
#define INVALID_PERIODIC_CQI_REQ_TYPE  255

#define CSI_CONFIG_INDEX_VR10_TAG_LENGTH 5
#define CSI_SUBFRAME_PATTERN_CONFIG_VR10_TAG_LENGTH 5
#define CQI_PMI_CONFIGINDEX2_VR10_TAG_LENGTH 6
#define RI_CONFIGINDEX2_VR10_TAG_LENGTH 6
#define CSI_MEASUREMENTSUBSET1_VR10_TAG_LENGTH 13
#define CSI_MEASUREMENTSUBSET2_VR10_TAG_LENGTH 13
#define CSI_MEASUREMENT_SUBSET1_FDD_TAG_LENGTH 9
#define CSI_MEASUREMENT_SUBSET2_FDD_TAG_LENGTH 9

/* EICIC -*/
#if 0
#define MAC_CREATE_UE_ENTITY_REQ        1
#define MAC_CREATE_UE_ENTITY_CNF        2
#define MAC_DELETE_UE_ENTITY_REQ        3
#define MAC_DELETE_UE_ENTITY_CNF        4
#define MAC_RECONFIGURE_UE_ENTITY_REQ   5
#define MAC_RECONFIGURE_UE_ENTITY_CNF   6
#define MAC_CONFIG_CELL_REQ             7
#define MAC_CONFIG_CELL_CNF             8
#define MAC_UE_ENTITY_POWER_HEADROOM_IND        11
#define MAC_CCCH_PDU_IND               12
#define MAC_CRNTI_ELEMENT_IND          13
#endif

/* new API to support cell config and UE Create */
#define MAC_CONFIG_CELL_REQ             1
#define MAC_RECONFIG_CELL_REQ           3
#define MAC_CREATE_UE_ENTITY_REQ        4
#define MAC_DELETE_UE_ENTITY_REQ        5
#define MAC_RECONFIGURE_UE_ENTITY_REQ   6

#define MAC_CONFIG_CELL_CNF             101
#define MAC_CREATE_UE_ENTITY_CNF        104
#define MAC_DELETE_UE_ENTITY_CNF        105
#define MAC_RECONFIGURE_UE_ENTITY_CNF   106
#define RECONFIG_UE_LENGTH 8
#ifndef CLPC_STUB
#define ADD_UE_MANDATORY_LENGTH     19
#else
#define ADD_UE_MANDATORY_LENGTH     17 
#endif


/* MAC OAM API Ids */
/* API Ids 51-100 are reserved for OAM */
#define MAC_INIT_LAYER_REQ          51
#define MAC_INIT_LAYER_CNF          52
#define MAC_CLEANUP_LAYER_REQ       53
#define MAC_CLEANUP_LAYER_CNF       54
#define MAC_GET_BUILD_INFO_REQ      55
#define MAC_GET_BUILD_INFO_CNF      56
#define MAC_SET_LOG_LEVEL_REQ       57
#define MAC_ENABLE_LOG_REQ          58
#define MAC_GET_STATUS_REQ          59
#define MAC_GET_STATUS_CNF          60
#define MAC_RESET_STATS_REQ         61
#define MAC_GET_STATS_REQ           62
#define MAC_GET_STATS_CNF           63
#define MAC_ENABLE_DL_SCH_STATS_REQ 64
#define MAC_ENABLE_UL_SCH_STATS_REQ 65
#define MAC_ENABLE_DISABLE_LOG_REQ  66
/* + CA_AB */
#define MAC_INIT_CELL_REQ           67
#define MAC_INIT_CELL_CNF           68
/* - CA_AB */
/* Tags */
#define GET_PER_UE_STATUS       1
#define MAC_UE_STATUS           2
#define MAC_DL_LOG_CH_STATUS    3
#define MAC_UL_LOG_CH_STATUS    4
#define MAC_DL_THROUGHPUT       5
#define MAC_UL_THROUGHPUT       6



#define TAG_SIZE 2
#define LENGTH_SIZE 2
#define UL_LC_RESP_LENGTH 4
#define DL_LC_RESP_LENGTH 4
#define CRNTI_TAG_LENGTH 6
#define MIN_LC_CREATE_LENGTH 9
/* + CA_AB */
#define MAC_INIT_REQ_MANDATORY_LENGTH 47
#define MAC_INIT_CELL_REQ_MANDATORY_LENGTH 47 
/* - CA_AB */
#define MAC_DL_THROUGHPUT_LENGTH 4
#define MAC_UL_THROUGHPUT_LENGTH 4
#define MAC_DL_LOG_CH_STATUS_LENGTH 10
#define MAC_UL_LOG_CH_STATUS_LENGTH 10
#define GET_PER_UE_STATUS_LENGTH 6
#define MAC_UE_STATUS_LENGTH 51

#define CREATE_LC_REQ_MIN_LEN 6
#define UL_LC_CREATE_REQ_LEN 5
#define DL_LC_CREATE_REQ_LEN 5
#define UL_LC_RECONFIG_REQ_LEN 4
#define DL_LC_RECONFIG_REQ_LEN 4
#define DELETE_LC_REQ_LEN 6
#define MAX_LC_TYPE 3
#define MAC_MAX_LCHID 10
//#define SR_CONFIG_INFO 11
#define SR_CONFIG_LEN 5
#define MAC_SR_SETUP_INFO_LEN 7
#define MAC_TAG_NOT_EXISTS 3
#define CQI_INFO_LEN 6
#define CQI_PMI_CONFIG_INDEX_INFO_LEN 10
#define SR_SETUP_INFO_LEN 7
#define TX_MODE_INFO_LEN 5
#define NUM_OF_LAYER_INFO_LEN 5
#define CODEBOOK_INDEX_INFO_LEN 5

    /** For TTIB_Code */
#define TTI_BUNDLING_INFO_TAG_LEN 5

//#define SR_SETUP_INFO 17
#define RECONFIG_UE_ENTITY_MIN_LEN 2
#define RECONFIG_LC_REQ_MIN_LEN 5
#define RECONFIG_UE_INFO_MIN_LEN 4
#define INVALID_APERIODIC_CQI_MODE 255
 /* + CQI_4.1 */
#define PERIODIC_CQI_SETUP_LEN 15
#define PERIODIC_CQI_LEN 10
 /* + CQI_4.1 */
#define DL_MAX_RB_INFO_LEN 5
#define UL_MAX_RB_INFO_LEN 5

#ifdef TDD_CONFIG
#define TDD_ACK_NACK_MODE_TLV_LEN 5
#endif

#define MAC_DELETION_PENDING 2
#define MAC_MIN_TRANSMISSION_MODE 1
/* + TM7_8 Changes Start */
#define MAC_MAX_TRANSMISSION_MODE 8
/* - TM7_8 Changes End */
#define MIN_NUM_OF_LAYER 1
 /* SPR 8422 Fix Start */
#define MAX_NUM_OF_LAYER 4
 /* SPR 8422 Fix End */
#define MIN_PRE_CODING_INDEX 0
#define MAX_PRE_CODING_INDEX 3
#define REQUEST_TYPE_0 0
#define REQUEST_TYPE_1 1
#define MAC_MIN_SRPUCCHRESOURCEINDEX 0
#define MAC_MAX_SRPUCCHRESOURCEINDEX 2047
#define MAC_MIN_SRCONFIGURATIONINDEX 0
#define MAC_MAX_SRCONFIGURATIONINDEX 154
#define RRC_CONNECTION_REQ 9
#define RRC_NB_CONNECTION_REQ 0x32
#define RRC_CONNECTION_SETUP 10 
#define RRC_CONNECTION_SETUP_NB 51
 

/* Added for Paging Support At MacEmulator */
/*SPR 2457 Fix Begin*/
#define MAC_PCCH_MSG_IND 11
/*SPR 2457 Fix End*/
 /* DRX Timers MAX values*/
#define MAX_ON_DURATION_TIMER_VALUE         15
#define MAX_INACTIVITY_TIMER_VALUE          21
#define MAX_RETRANSMISSION_TIMER_VALUE      7
#define MAX_SHORT_CYCLE_TIMER_VALUE         15
#define MAX_LONG_CYCLE_TIMER_VALUE          15
#define MAX_DRX_START_OFFSET                2550
/* Meas_Gap periodicity*/
#define MEASUREMENT_GAP_PERIODICITY_40      40
#define MEASUREMENT_GAP_PERIODICITY_80      80
/*Meas_Gap_Changes_Start*/
#define MEASUREMENT_GAP_PERIODICITY_20      20
#define MEASUREMENT_GAP_PERIODICITY_160      160
/*Meas_Gap_Changes_End*/
/* RA RNTI RANGE */
#define START_RARNTI 1
#define END_RARNTI 50
/*LTE_REG_FIX*/
#define START_SPS_CRNTI 116
#define END_SPS_CRNTI 180
#define INVALID_RNTI 0
#define MAX_NUM_RA_REQUEST END_RARNTI
/* + E_CID_5.3 */
#define UE_E_CID_FILE_PATH "../cfg/eCIDConfig.cfg"
#define E_CID_MAX_LINE_SIZE 100*MAX_NUM_CELLS
#define MAX_REL9_TA_VALUES_SUPPORTED 10
typedef struct Rel9TAValuesT
{
    UInt16   rel9TARach[MAX_REL9_TA_VALUES_SUPPORTED];
    UInt16   rel9TAUlsch[MAX_REL9_TA_VALUES_SUPPORTED]; 
    UInt16   rel9TACqi[MAX_REL9_TA_VALUES_SUPPORTED]; 
    UInt16   rel9TASrs[MAX_REL9_TA_VALUES_SUPPORTED]; 
    UInt8    numTaRach;
    UInt8    numTaUlSch;
    UInt8    numTaCqi;
    UInt8    numTaSrs; 

} Rel9TAValues;

/* - E_CID_5.3 */

/* + CQI_4.1 */
#define CQI_MAX_LINE_SIZE 250
#define MAX_UE_PROFILE 5
#define UE_CQI_PROFILE_FILE_PATH "../cfg/ueCQIProfile.cfg"
/* EICIC +*/
#define UE_CQI_FILE_PATH_ABS "../cfg/ueCQIConfig_ABS.cfg"
/* EICIC -*/
#define UE_CQI_FILE_PATH "../cfg/ueCQIConfig.cfg"
/* +DYNAMIC_ICIC*/
#define UE_MEAS_REPORT_FILE_PATH "../cfg/l1MeasurementConfig.cfg"
/* -DYNAMIC_ICIC*/
/* + TM7_8 Changes Start */
#define ANGLE_OF_ARRIVAL_VALUE_FILE_PATH "../cfg/ueAngleOfArrivalValues.cfg"
/* - TM7_8 Changes End */
/*TDD SB_CQI*/
#ifdef FDD_CONFIG
#define TOTAL_NUM_ROW_IN_CQI_CONFIG 10
#else
#define TOTAL_NUM_ROW_IN_CQI_CONFIG 7
#endif
/*TDD SB_CQI*/
#define TOTAL_NUM_ROW_IN_RI_CONFIG  6
#define MAX_CQI_UE_SUPPORTED        MAX_UE_SUPPORTED
#define MAX_CQI_PROFILE             8

#define MAX_UE_SELECTED_SUBBAND 7
#define MAX_UE_SELECTED_N 26

#define TWO_BIT_ROTATION( n )\
         ((n&1)<<1)|((n&2)>>1)

#define THREE_BIT_ROTATION( n )\
        ((n&1)<<2)|(n&2)|((n&4)>>2)

#define FOUR_BIT_ROTATION( n )\
        ((n&1)<<3)|((n&2)<<1)|\
        ((n&4)>>1)|((n&8)>>3)
 
#define EIGHT_BIT_ROTATION( n )\
        ((n&1)<<7)|((n&2)<<5)|((n&4)<<3)|((n&8)<<1)|\
        ((n&16)>>1)|((n&32)>>3)|((n&64)>>5)|((n&128)>>7)


/* Enum used to find the Periodic Report Type */
typedef enum periodicReportTypeT  
{
    SUB_BAND_TYPE_1 =1,
    WIDE_BAND_CQI_PMI_TYPE_2,
    RI_TYPE_3,
    WIDE_BAND_CQI_TYPE_4
} periodicReportType;

/* Enum used to find the Periodic Report Type */
typedef enum cqiPeriodicModeTypeT
{
/* Phase 2 Periodic CQi Changes Start*/
    PERIODIC_MODE_INVALID = 0,
/* Phase 2 Periodic CQi Changes End*/
    CQI_PERIODIC_TYPE_10 = 10,
    CQI_PERIODIC_TYPE_11 = 11,
    CQI_PERIODIC_TYPE_20 = 20,
    CQI_PERIODIC_TYPE_21 = 21
} cqiPeriodicModeType;

/* Enum used to find the Periodic Report Type */
typedef enum cqiAperiodicModeTypeT
{
    CQI_APERIODIC_TYPE_12 = 12,
    CQI_APERIODIC_TYPE_20 = 20,
    CQI_APERIODIC_TYPE_22 = 22,
    CQI_APERIODIC_TYPE_30 = 30,
    CQI_APERIODIC_TYPE_31 = 31
} cqiAperiodicModeType;
/* + CA_PHASE2 */
typedef enum TransmissonModeT
{
	/* SPR 545 Fix starts */
	TX_MODE_ATTACH_NOT_COMPLETE = 0,
	/* SPR 545 Fix ends */
	TX_MODE_1 = 1,
	TX_MODE_2,
	TX_MODE_3,
	TX_MODE_4,
	TX_MODE_5,
	TX_MODE_6,
	TX_MODE_7,
	/* + TM7_8 Changes Start */
	TX_MODE_8,
	/* - TM7_8 Changes End */
	INVALID_TX_MODE
}TransmissonMode;
/* - CA_PHASE2 */

#ifdef UESIM_ENHANCEMENTS
/* Enum used to define the RB status */
typedef enum rbStatusT
{
    RB_CAN_BE_ALLOCATED__FREE = 1,
    RB_OUT_OF_SYSYEM_BANDWIDTH,
    RB_CAN_BE_ALLOCATED__BUSY,
    RB_ALLOCATED_FOR_SEMI_STATIC_INFORMATION
} rbStatus;
#endif

typedef struct cqiPmiRiReportMapT
{
    UInt8    reportType[MAX_SF_VALUE][MAX_CQI_UE_SUPPORTED];
}cqiPmiRiReportMap;

typedef struct CqiPmiConfigT
{
    UInt16 cqiPmiConfigStartIndex;
    UInt16 cqiPmiConfigEndIndex;
    UInt8  cqiPeriodicity;
    SInt16 nOffsetCQI;
}CqiPmiConfig;

typedef struct RiConfigT
{
    UInt16 riConfigStartIndex;
    UInt16 riConfigEndIndex;
    UInt8  riPeriodicity;
    SInt16 nOffsetRI;
}RiConfig;

typedef struct cqiUEProfileValuesT
{
    UInt32 ueProfile[MAX_CQI_VALUES_PER_PROFILE];
    UInt8 indexCount;
    UInt32 indexTracker;/*EICIC Enhancement */
}cqiUEProfileValues;

typedef struct posOfMSelSBForAperiodicMode20T
{
    UInt8 subbandNum_1;
    UInt8 subbandNum_2;
    UInt8 subbandNum_3;
    UInt8 subbandNum_4;
    UInt8 subbandNum_5;
    UInt8 subbandNum_6;

}posOfMSelSBForAperiodicMode20;

typedef struct posOfMSelSBForAperiodicMode22RIOneT
{
    UInt8 subbandNum_1;
    UInt8 subbandNum_2;
    UInt8 subbandNum_3;
    UInt8 subbandNum_4;
    UInt8 subbandNum_5;
    UInt8 subbandNum_6;

}posOfMSelSBForAperiodicMode22RIOne;

typedef struct posOfMSelSBForAperiodicMode22RITwoT
{
    UInt8 subbandNum_1;
    UInt8 subbandNum_2;
    UInt8 subbandNum_3;
    UInt8 subbandNum_4;
    UInt8 subbandNum_5;
    UInt8 subbandNum_6;

}posOfMSelSBForAperiodicMode22RITwo;

extern cqiUEProfileValues cqiUEProfileValues_g[MAX_CQI_PROFILE];
/* Aperiodic CQI Changes Start */
extern posOfMSelSBForAperiodicMode20 posOfMSelSBForAperiodicMode20_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern posOfMSelSBForAperiodicMode22RIOne posOfMSelSBForAperiodicMode22RIOne_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern posOfMSelSBForAperiodicMode22RITwo posOfMSelSBForAperiodicMode22RITwo_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
/* Aperiodic CQI Changes Start */
#ifdef UESIM_ENHANCEMENTS
extern UInt8 dlRBInfo_g[MAX_NUM_CELLS][MAX_RBs];
extern UInt8 ulRBInfo_g[MAX_NUM_CELLS][MAX_RBs];
#endif

extern void parseUeCQIProfile();
extern void parseUeCQIConfig();
extern void parseUeCQIConfig_ABS(); /* EICIC +-*/
extern void setActiveCell(UInt8 cellIndex);
MacRetType validateDlCqiProfile(UInt8 cqiProfileId);
void createCqiPmiRiPeriodicityMap(cqiPmiRiReportMap *cqiPmiRiReportMap_p,
                                  UInt16             cqiPmiConfigIndex,
                                  UInt16             riConfigIndex,
                                  UInt8              K,
                                  UInt8              wide_suband,
                                  UInt16             ueIndex,
                                  UInt8              isScell,
                                   UInt8             isAbs); /* EICIC +-*/

UInt32 getCqiRiPeriodicityAndOffset( UInt16 cqiPmiConfigIndex,
                                     UInt16 riConfigIndex,
                                     UInt8  H,
                                     UInt16 *cqiPeriodicity_p,
                                     SInt16 *nOffsetCQI_p,
                                     UInt16 *riPeriodicity_p,
                                     SInt16 *nOffsetRI_p );

/* SPR 10980 Fix Start */
extern cqiPmiRiReportMap cqiPmiRiReportMap_g[MAX_SFN_VALUE];
/* EICIC +*/
extern cqiPmiRiReportMap cqiPmiRiReportMap2_g[MAX_SFN_VALUE];
/* EICIC -*/
/* SPR 10980 Fix End */
/* - CQI_4.1 */

/* HO changes */
/*For CHANGE CRNTI Info*/
#define MAC_EMU_RESET_REQ 0x0F
#define MAC_EMU_RESET_CNF 0x6E 
#define MAC_EMU_HO_BASED_RACH_REQ 0x11
#define MAC_EMU_CHANGE_CRNTI_REQ 0x10
#define MAC_EMU_CHANGE_CRNTI_CNF 0x6F
#define MAC_EMU_CHANGE_CRNTI_REQ_LEN 6

/*CLPC_CHG*/
#define MAX_COUNT_VALUE 64
#define MAX_TIMER_VALUE 65535
#define MAX_PROFILE_VALUE 63
#define PHR_INACTIVE 255
#define TPC_RNTI_COUNT 200
#define START_TPC_RNTI 200
#define END_TPC_RNTI 400
#define MIN_PC_MAX -40
#define MAX_PC_MAX 23
#define MIN_PA_VALUE 0
#define MAX_PA_VALUE 7
#define PDSCH_CONFIG_INFO_PARAM_LENGTH 1
#define TPC_PDCCH_CONFIG_LENGTH 5
#define P0_UE_PUSCH_LEN 5

/* HD FDD Changes Start */
#define RF_PARAMS_LENGTH 5
/* HD FDD Changes End */
/* SPS_CHG */
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
#define SPS_DL_MAX_HARQ_RETX_TAG_LEN 5
#define SPS_DL_EXPLICIT_RELEASE_AFTER_TAG_LEN 5
#define MAX_QCI 9
#define QOS_INFO_MIN_LEN 6
/* + CA_PHASE2 */
#define RF_PARAMS_LEN 5
#define MAX_USER_LOCATION 2
#define USER_LOCATION_TYPE_LEN 1
#define RI_CONFIG_INDEX_INFO_LEN 6
#define PERIODIC_WIDEBAND 0
#define PERIODIC_SUBBAND 1
#define CQI_FORMAT_PERIODIC_SUBBAND_K_LEN 5
#define MAC_MAX_SUBBAND_CQI_K 4
#define MAC_MIN_SUBBAND_CQI_K 1
#define RI_CONFIG_INDEX_LEN 6
#define U16BIT_LEN 2
#define UE_CATEGORY_TAG_LEN 5
#define MAC_MAX_UE_CATEGORY 5
#define MAC_MIN_UE_CATEGORY 1
#define MAC_MAX_UE_CATEGORY_V1020 8
#define MAC_MIN_UE_CATEGORY_V1020 6
#define PUCCH_FORMAT_V1020_MIN_LEN 9
#define PUCCH_CHANNEL_SELECTION_CONFIG_V1020_SETUP 1
#define PUCCH_CHANNEL_SELECTION_CONFIG_V1020_RELEASE 0
#define PUCCH_CHANNEL_SELECTION_CONFIG_V1020_MIN_LEN 5
#define PUCCH_AN_CS_LIST_V1020_MIN_LEN 4
#define PUCCH_AN_CS_V1020_LEN 6
#define MIN_N1PUCCH_RESOURCE_VAL 0
#define MAX_N1PUCCH_RESOURCE_VAL 2047
#define MIN_CA_CONFIG_TAG_LEN 4
#define SCELL_DEACTIVATION_TIMER_INFO_TAG_LEN 5
#define SCELL_DEACTIVATION_TIMER_VALUE_COUNT 7
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
#define CBSR_TYPE_MAX_VAL 9
#define CBSR_TYPE_MIN_VAL 0
#define MAX_CBSR_VALUE_COUNT 14
#define APERIODIC_CSI_TRIGGER_R10_TAG_LEN 6
#define MIN_DL_NUM_HARQ_PROC_VAL 6
#define MAX_DL_NUM_HARQ_PROC_VAL 8
#define DL_NUM_HARQ_PROC_TAG_LEN 5
#define DL_INIT_MCS_TAG_LEN 5
#define MIN_DL_INIT_MCS_VAL 0
#define MAX_DL_INIT_MCS_VAL 28
#define TRIGGER1R10_VAL 64
#define TRIGGER2R10_VAL 192
#define SCELL_CQI_MIN_LEN 9
#define SCELL_CQI_MAX_LEN 76
#define RESET_CA_UE_ENTITY_REQ_LEN 7
#define SCELL_ACTIVATE_STATUS_LEN 5
#define DL_NUM_RB 8
#define DL_SCELL_INFO 1
/* + SPR 11213 Changes */
#define CA_LC_BANDWIDTH_DIST_INFO_TAG_LEN 12
/* - SPR 11213 Changes */
#define MIN_CA_LC_BANDWIDTH_DIST_VAL 0
#define MAX_CA_LC_BANDWIDTH_DIST_VAL 100
/* CA_PHASE2 */

    /** For TTIB_Code */
#define TTIB_DISABLED 0
#define TTIB_ENABLED 1


/* SPS_CHG */

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
/* SPR 18819 fix start */
#define HO_MAX_NUM_UE 64
/* SPR 18819 fix end */
typedef struct hoRachInfoT{
        UInt8  raReqIndex;
        UInt16 cellId;
}hoRachInfo;
typedef enum rachTypeT
{
   /* SPR 19572 changes start */
   CONTENTION_BASED_INTRA_ENB_HO_RACH,
   /* SPR 19572 changes End */
   CONTENTION_BASED_RACH,
   CONTENTION_FREE_HO_RACH,
   CONTENTION_BASED_HO_RACH,
   CONTENTION_FREE_PDCCH_ORDER_RACH,
   CONTENTION_BASED_PDCCH_ORDER_RACH,
   REESTABLISHMENT_RACH,
   RLF_TRIGGER_NO_RACH,
   ULSYNC_START_RACH,
   /* SPR 5313 changes start */
   CONTENTION_FREE_INTRA_ENB_HO_RACH,
   /* SPR 5313 changes end */
}rachType;
hoRachInfo hoInfo_g[MAX_NUM_CELLS][HO_MAX_NUM_UE];
UInt16 ueHoCount_g[MAX_NUM_CELLS];
UInt8 hoRachTriggerFlag[MAX_NUM_CELLS];
/* ---- */

/* ULA_CHG */
#define UE_SINR_FILE_PATH "../cfg/ueSINRConfig.cfg"
#define UE_PROFILE_FILE_PATH "../cfg/ueProfileConfig.cfg"
/* ULA_CHG */

/*Power Control Change*/
#define UE_PHR_FILE_PATH "../cfg/uePHRConfig.cfg"

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/*structure for sending information to RRC*/
typedef struct rrcDataInfoT {
  UInt16  dataLen;
  void   *buffer_p;    
  UInt16  rnti;
}rrcDataInfo;
/*SR Periodicity */
typedef enum SRPeriodicityT
{
    /* SPR 18871 Fix Start */
    SR_PERIODICITY_OFF = 0,
    SR_PERIODICITY_5  = 5,
    SR_PERIODICITY_10 = 10,
    SR_PERIODICITY_20 = 20,
    SR_PERIODICITY_40 = 40,
    SR_PERIODICITY_80 = 80,
    SR_PERIODICITY_2 = 2,
    SR_PERIODICITY_1 = 1
    /* SPR 18871 Fix End */
} SRPeriodicity;

 
/*LC Config Info */
typedef enum LCConfigInfoT
{
    DL_LC_INFO = 1,
    UL_LC_INFO = 2,
    UL_DL_LC_INFO = 3
} LCConfigInfo;


typedef struct CellConfigResT {
    UInt16 transactionID;      /* transaction ID sent by RRC Sim */
    UInt16 response;           /* MAc SUCCESS or FAILURE */
} CellConfigRes;

typedef struct UEEntityCreateResT {
    UInt16 transactionId;
    UInt16 response;
} UEEntityCreateRes;
/*SPR 21660 changes start_Kishore*/
typedef struct NBUEEntityCreateResT {
        UInt16 ueIndex;
            UInt16 response;
}NBUEEntityCreateRes;
/*SPR 21660 changes end_Kishore*/


typedef struct UEEntityDeleteResT {
    UInt16 idx;
    UInt16 response;       /* ACK/NACK */
    UInt16 transactionId;
} UEEntityDeleteRes;


typedef struct UEEntityReconfigResT {
    UInt16 response;     /* ACK/NACK */
    UInt16 transactionId;
} UEEntityReconfigRes;

typedef struct LCConfigRespT {
    UInt8  status;
    UInt8  opCode;
    UInt16 result;
} LCConfigResp;    

typedef struct InitReqParamsT {
    UInt8 dlUeQueueLoadWt;
    UInt8 dlLcQueueLoadWt;
    UInt8 dlUeNackQueueLoadWt;
    UInt8 dlMsWt;
    UInt8 dlMs64QAMFactorWt;  
    UInt8 dlMs16QAMFactorWt;
    UInt8 dlMsQPSKFactorWt;
    UInt8 dlUePriorityWt;
    UInt8 dlLcPriorityWt;
    UInt8 dlUeDelayWt;
    UInt8 dlLcDelayWt;
    //UInt8 dlCQIWt;
    //UInt8 numDemuxThreads;
    //UInt8 numEncoderThreads;
    //UInt8 numMultiplexerThreads;
    UInt8 frequencySelectiveSchedulingEnabled;  
    UInt8 ulMsWt;
    UInt8 ulMs64QAMFactorWt;  
    UInt8 ulMs16QAMFactorWt;
    UInt8 ulMsQPSKFactorWt;
    UInt8 ulUePriorityWt;
    UInt8 ulUeDelayWt;
    UInt8 ulUeBsrLoadWt;
    //UInt8 ulRetransType;
    UInt8 ulLcgPriorityWt;
    UInt8 logLevel;
    UInt8 dlSchedulerStrategy;
    /*it indicates the type of Uplink scheduler Strategy to run i.e. Fair Scehduler Strategy or Round Robin*/
    UInt8 ulSchedulerStrategy;
    /*it indicates the maximum number of UEs to be  scehduled in a TTI in downlink */
    UInt8 maxUEsToBeScheduledDL;
    /*it indicates the maximum number of UEs to be  scehduled in a TTI in Uplink */
    UInt8 maxUEsToBeScheduledUL;
    /* startSpsCRnti indicates the starting value for range of SPS CRNTI */
    UInt16 startSpsCrnti;
    /* The end value for range of SPS CRNTI */
    UInt16 endSpsCrnti;
    /* QCI sps map */
    UInt8  qciSpslist[MAX_QCI];
  
   /* + CA_AB */    
    UInt8 maxNumCells ;
	UInt8 isCASupported ;
   /* + CA_AB */    
#ifdef LTE_EMBMS_SUPPORTED
    UInt8 eMBMSEnable; 
#endif   
} InitReqParams;
/* + CA_AB */
/*typedef struct ueSpsSysWideParamT
{*/
    /* QCI sps map */
   /* UInt8  qciSpslist[MAX_QCI];
    
}ueSpsSysWideParam;*/
/* - CA_AB */


/* HO reset */
typedef struct ResetResT
{
    UInt16 idx;
    UInt16 response;
    UInt16 transactionId;
}ResetRes;
/* HO reset*/

//Pradeep
typedef struct RARequestT
{
        UInt16 RaRnti;
        UInt8  preamble;
        UInt8  state;
        UInt8  rachTypeFlag;
        UInt16 rnti;  
        UInt16 rrcConReqLen;
        UInt8  rrcConnectionReq[100];
}RARequest;

extern RARequest raRntiArr_g[MAX_NUM_CELLS][MAX_NUM_RA_REQUEST];

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
	UL_LC_RECONFIGURE_REQ,
	DL_LC_RECONFIGURE_REQ,
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
#if 0
    UL_LC_RECONFIG_REQ  ,
	DL_LC_RECONFIG_REQ  ,
#endif
    RECONFIGURE_LC_ERROR   , 
	DELETE_LC_ERROR	,
    PUCCH_CONFIG_INFO
#if 1
    ,
    SI_MSG_INFO_PARAM,
    PUSCH_CONFIG_INFO
#endif
    #ifdef TDD_CONFIG
    ,
    TDD_CONFIG_INFO = 0x25,
    TDD_ACK_NACK_FEEDBACK_MODE_INFO
    #endif
#if 1
    ,UE_CATEGORY = 0x40,
     SRS_DEDICATED_CONFIG = 0x44,
     SRS_DEDICATED_SETUP_INFO = 0x45
#endif
    /* + CL-MIMO RSM*/
    ,CODEBOOK_SUBSET_RESTRICTION = 0x46,
    /* - CL-MIMO RSM*/
    /*+ DRX_CHG */
    DRX_CONFIG_INFO       = 0x47,
    DRX_SETUP_CONFIG_INFO = 0x48,
    SHORT_DRX_CONFIG      = 0x49,
    /*- DRX_CHG */
   /* + MEAS_GAP_CHG */
    MEAS_GAP_CONFIG_INFO       = 0x50,
    MEAS_GAP_SETUP_CONFIG_INFO = 0x51,
    CQI_MASK_INFO              = 0x52,
    TIME_ALIGNMENT_TIMER_INFO = 0x54,
    /* - MEAS_GAP_CHG */
    QOS_INFO = 0x58,
    /*CLPC_CHG*/
    UPLINK_POWER_CONTROL_DEDICATED = 0x68,
    TPC_PDCCH_CONFIG_PUCCH = 0x69,
    TPC_PDCCH_CONFIG_PUSCH = 0x6E,
    TPC_RNTI_INFO = 0x6C,
    PDSCH_CONFIG_INFO = 0x6A,
    P0_UE_PUSCH = 0x6B,
    /* HD FDD Changes Start */
    RF_PARAMS = 0x9E,
    /* HD FDD Changes End */
    /*CLPC_CHG*/
     /* SPS_CHG_START */
     SPS_CONFIG = 0x95,
     SPS_DL_INFO = 0x96,
     SPS_DL_SETUP_INFO = 0x97,
     SPS_N1_PUCCH_AN_PERSIST_LIST = 0x98,
     SPS_UL_INFO = 0x99,
     SPS_UL_SETUP_INFO = 0x9A,
     P_ZERO_PERSISTENT = 0x9B,
     TWO_INTERVALS_CONFIG = 0x9C,
     SPS_DL_MAX_HARQ_RETX = 0xA2,
     SPS_DL_EXPLICIT_RELEASE_AFTER = 0xA3
//CODE_COMMENT_L1_FIXED         
    /** For TTIB_Code */
	 ,TTI_BUNDLING_INFO = 0xB6,
	 /* SPS_CHG_END */
	 /* + CA_PHASE2 changes start*/
	 USER_LOCATION_TYPE = 0x88,
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
	 /* - CA_PHASE2 changes end*/
 	/* EICIC +*/
     DL_INTERFERENCE_INFO = 0xCE,
    CSI_SUBFRAME_PATTERN_CONFIG_VR10 = 0XCF,
    CSI_MEASUREMENT_SUBSET_VR10 = 0XD0,
    CSI_CONFIG_INDEX_VR10 = 0XD1,
    CQI_PMI_CONFIGINDEX2_VR10 = 0XD2,
    CSI_MEASUREMENT_SUBSET2_VR10 = 0XD3,
	/* EICIC -*/
} RRCTag;
#define CSI_MEASUREMENT_SUBSET_VR10_TAG_LENGTH 8
#define DL_INTERFERENCE_INFO_TAG_LENGTH 5

typedef enum CsiChoiceT
{
FDD_CONFIG_TYPE=0,
TDD_CONFIG1_5=1,
TDD_CONFIG0=2,
TDD_CONFIG6=3
}CsiChoice;

typedef struct
{
    LTE_SNODE  anchor;
    /* SPR 5493 changes start */
    UInt16      len;
    /* SPR 5493 changes end */
    void      *msg;
} rrcOamMsgNode;
/* Socket FD for receiving messages */
extern SInt32 rxSockFD_g;
/* Socket FD for receiving messages */
extern SInt32 txSockFD_g[MAX_NUM_CELLS];
extern SockAddr tx_addr_g;
extern SInt32 txOAMSockFD_g;
extern SockAddr txOAM_addr_g;
extern UInt8 logLevel_g;

extern UInt32 stoppedProcessing_g[MAX_NUM_CELLS];
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* + CA_AB */
void sendMsgToRRC(UInt16 msgId, void* data_p, ueContextForDownlink *dlUEInfo, ueContextForUplink *ulUEInfo, UInt8 cellIndex);
/* - CA_AB */

extern UInt16 createMACUEEntity(ueContextForDownlink *dwlkUEInfo_p,
                               ueContextForUplink* uplkUEInfo_p,
                               UInt8 uplkLCCount, UInt16 transactionId,
							   UInt8 cellIndex);
extern UInt16 deleteMACUEEntity(UInt16 idx, UInt16 transactionId, UInt8 cellIndex);
extern UInt16 reconfigureMACUEEntity(ueContextForDownlink *dwlkUEInfo_p,
                                    ueContextForUplink *uplkUEInfo_p,
                         UInt32 uplkLCCount,
						 UInt8 cellIndex);
extern MacRetType reconfigureMACDLLogicalChannel (DLUEContext* newUEcontext_p,
                         LogicalChannelConfigInfo* logicalChConfig_p);
void parseRRCConnectionReqMsg(UInt8 *msg_p, UInt16 msgLen, UInt8 cellIndex);
MacRetType fapiPrepareRAInfo(UInt8 raReqIndex, UInt8 cellIndex);
UInt8 getNextPreambleIndex();
extern MacRetType reconfigureMACULUEEntity (UInt16 ueIdx,
                                     ueContextForUplink *uplkUEInfo_p,
                                     UInt32 logicalChCount,
                                     ulLogicalChannelInfo *lcInfo_p,
									 UInt8 cellIndex);

/* ULA_CHG */
typedef struct ULSCHProfileT {
    UInt8 sinrSeq[MAX_ULSCH_SINR_PROFILES];
}ULSCHProfile;

/* CLPC_CHG*/
typedef struct CQIProfileT {
    UInt8 sinrSeq[MAX_CQI_SINR_PROFILES];
}CQIProfile;
/* CLPC_CHG*/

typedef struct SRSProfileT {
    UInt8 startPosition;
    UInt8 srsBandwidth;
    UInt8 hoppingEnable;
    UInt8 sinrSeq[MAX_SRS_SINR_PROFILES];
}SRSProfile;

/*CLPC_CHG*/
typedef struct PHRProfileT
{
    UInt8 phrCount;
    UInt16 phrTimer;
    UInt8 phrValues[MAX_COUNT_VALUE];
}PHRProfile;

typedef struct ueProfileT 
{
    UInt8 srsProfileId;
    UInt8 ulschProfileId;
    UInt8 phrProfileId; /*Added for Power Control*/
    UInt8 cqiProfileId;
    /* + CQI_4.1 */
    UInt8 dlCqiProfileId;
    /* - CQI_4.1 */
}ueProfile;

UInt8  stringTokenArray(char *str, 
        UInt8 *arr, 
        UInt8 numRows, 
        UInt8 numCol);

/*CLPC_CHG*/
typedef struct TpcInfoT
{
    UInt16 ueIndex[16];
    MacRetType isValid[16];
    UInt8 configuredCount;
}TpcInfo;

/*CLPC_CHG*/


void trimSpaces(char *str); /* ULA_UTP */
void parseUeProfileConfig(ueProfile ueProfileInfo_p[][MAX_UE_PROFILES]);
void createUeProfileConfig(ueProfile ueProfileInfo_p[][MAX_UE_PROFILES]);
/* CLPC_CHG*/
void parseUeSINRConfig(SRSProfile *srsProfileInfo_p,
        ULSCHProfile *ulschProfileInfo_p,
        CQIProfile *cqiProfileInfo_p,
        UInt8 srsSinrValues [][MAX_PRB], 
        UInt8 ulschSinrValues [],
        UInt8 cqiSinrValues []);

void createUeSINRConfig(SRSProfile *srsProfileInfo_p,
        ULSCHProfile *ulschProfileInfo_p,
        CQIProfile *cqiProfileInfo_p,
        UInt8 srsSinrValues [][MAX_PRB],
        UInt8 ulschSinrValues [],
        UInt8 cqiSinrValues []);
/* CLPC_CHG*/
MacRetType validateSinrValues (UInt16 sinrVal);
MacRetType validateStartPosition(UInt8 startPos);
MacRetType validateSrsBandwidth(UInt8 srsBandwidth);
MacRetType validateHoppingEnable(UInt8 hoppingEnable);
MacRetType validateSinrSeq(UInt8 sinrSeqVal, 
        UInt8 sinrProfileNum);
MacRetType validateSrsProfile(UInt8 srsProfileId);
MacRetType validateUlschProfile(UInt8 ulschProfileId);
/* CLPC_CHG*/
MacRetType validateCqiProfile(UInt8 cqiProfileId);
/* CLPC_CHG*/
/* ULA_CHG */

/*CLPC_CHG*/
MacRetType validatePHRId(UInt8 phrProfileId);
MacRetType validatePHRCount(UInt8 phrCount);
MacRetType validatePHRTimer(UInt16 phrTimer);
MacRetType validatePHRValues(UInt8 phrValue);

/* + CA_AB */

/*
typedef struct PFSParamsT{

	UInt8 dlUeQueueLoadWt;
	UInt8 dlLcQueueLoadWt;
	UInt8 DlMsWt;
	UInt8 dlMs64QAMFactorWt;
	UInt8 dlMs16QAMFactorWt;
	UInt8 dlMsQPSKFactorWt;
	UInt8 dlLcPriorityWt;
	UInt8 dlUeDelayWt;
	UInt8 dlLcDelayWt;
	UInt8 UlMsWt;
	UInt8 ulMs64QAMFactorWt;
	UInt8 ulMs16QAMFactorWt;
	UInt8 ulMsQPSKFactorWt;
	UInt8 ulUeDelayWt;
	UInt8 ulUeBsrLoadWt;
	UInt8 ulLcgPriorityWt;

} PFSParams ;

typedef struct QosStrategyWeightsT{

	UInt8 pdbWeight;
	UInt8 qciWeight;
	UInt8 tokenWeight;
	UInt8 qLoadWeight;
	UInt8 mcsWeight;

} QosStrategyWeights ;

typedef struct QOSParamsT{

UInt8 siSchedulingRATPolicy;
UInt8 gbrBktDepthFactor;
UInt8 ambrBktDepthFactor;
UInt16 tokenPeriodicity;
UInt8 osStrategyOverallocFactor; 
QosStrategyWeights dlQosStrategyWeights;
QosStrategyWeights ulQosStrategyWeights;
UInt8 enableAllocAfterAllowedBitrate;

} QOSParams ;

typedef struct ICICParamsT{

	UInt8 maxCellEdgeUEsToBeScheduledDL;
	UInt8 maxCellEdgeUEsToBeScheduledUL
	UInt8 cellEdgeUECQIThreshold;

} ICICParams ;
typedef struct SpsSysWideParamsT{

	UInt8 enableSps;
	UInt8 maxSpsDciAttempt;
	UInt8 qciSpslist[MAX_QCI]; 
	UInt8 supportedDlIntervalList[SPS_INTERVAL_LIST]; 
	UInt8 supportedUlIntervalList[SPS_INTERVAL_LIST];

}SpsSysWideParam ;

typedef struct SPSParamsT{

SpsSysWideParam sysWideParams;
SpsDLIntervalProfile spsDlIntervalProfile;
SpsULIntervalProfile spsULIntervalProfile;
SpsCommIntervalProfile spsCommIntervalProfile;

} SPSParams ;
*/
typedef struct InitCellReqParamsT{
	UInt16     numOfUE;
	UInt16 	   rntiStartRange;
	UInt16     rntiEndRange;
	UInt32     fieldBitMask;
	UInt8      maxUEsToBeScheduledDl;
	UInt8      maxUEsToBeScheduledUl;
	UInt8      cqiThreshold;
	UInt8      dlRbRestrictionFactor;
	UInt8      ulRbRestrictionFactor;
	UInt8      ambrTokenLimitFactor;
	UInt8      gbrTokenLimitFactor;
	UInt8      commChWithMibAllowed;
	UInt8      dlSchWithMibAllowed;
	UInt8      maxMcs16Qam;
	UInt8      avgNumAmLcPerUe;
	UInt8      avgNumUmLcPerUE;
	UInt8      IsLastAttempt;
	/* Optional Parameters 
	PFSParams  pfsParams ;
	QOSParams  qosParams ;
	ICICParams icicParams ;
    SPSParams  spsParams ;
    */  
} InitCellReqParams;
/* + CA_PHASE2 */
STATIC inline MacRetType macValidateUeCategoryV1020(UInt8 ueCategoryV1020);

STATIC inline MacRetType parseReconfigPUCCHFormatV1020Info( 
		ueContextForUplink *ulUEInfo_p,
		ueContextForDownlink *dlUEInfo_p,
		UInt16 tagLen,
		UInt8 **msgBuf_pp,
		UInt8 cellIndex
		);
STATIC inline MacRetType parsePucchAnCsV1020 ( UInt8 **msgBuf_pp,
        UInt16 remainingTagLen,
        UInt8 pucchAnCsListElement,
        ueContextForDownlink *dlUEInfo_p,
        ueContextForUplink   *ulUEInfo_p,
        UInt8 cellIndex
        );
STATIC inline MacRetType validateN1Pucch(UInt16 n1Pucch);
STATIC MacRetType parseReconfigCAConfigInfo( 
    ueContextForUplink *ulUEInfo_p,
    ueContextForDownlink *dlUEInfo_p,
    UInt16 tagLen, 
	UInt8 **msgBuf_pp, 
    UInt8 cellIndex);
STATIC inline MacRetType parseScellDeactivationTimer(
    ueContextForUplink *ulUEInfo_p,
    ueContextForDownlink *dlUEInfo_p,
	UInt16 tagLen,
    UInt8 **msgBuf_pp,
    UInt8 cellIndex
    );
STATIC inline MacRetType validateScellDeactivationTimer(
    UInt8 scellDeactivationTimer);
STATIC inline MacRetType parseAndValidateScellInfo(
    ueContextForUplink *ulUEInfo_p,
    ueContextForDownlink *dlUEInfo_p,
    UInt16 tagLen,
    UInt8 **msgBuf_pp,
    UInt8 cellIndex
    );
STATIC inline MacRetType parseScellInfo(
        ueContextForUplink   *ulUEInfo_p,
        ueContextForDownlink *dlUEInfo_p,
        UInt16 tagLen,
		UInt8 **msgBuf_pp,
        UInt8 scellCount,
        UInt8 cellIndex
        ); 
STATIC inline MacRetType parseScellMimoCapabilityV1020(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p
        ); 
STATIC inline MacRetType parseScellTxModeInfo(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p,
        UInt8 cellIndex
        ); 
STATIC inline MacRetType parseScellCodeBookSubsetRestV1020(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p
        );
STATIC inline MacRetType parseScellPDSCHConfigInfo(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p,
        UInt8 cellIndex
        ); 
STATIC inline MacRetType parseCAReconfigUECqiInfo(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        UInt8  scellCount,
        ueContextForDownlink *dlUEInfo_p,
        ueContextForUplink   *ulUEInfo_p,
        UInt8 cellIndex
        ); 
STATIC inline MacRetType parseCAReconfigUECqiAperiodicInfo(
        UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoDl_p,
        CqiInfo *cqiInfoUl_p, 
        UInt8 transmissionMode,
        UInt8 *aperiodicCQIEnabled,
        UInt8 cellIndex,
/* Aperiodic CQI Changes Start */
        UInt8 scellCount,
        ueContextForDownlink *dlUEInfo_p
/* Aperiodic CQI Changes Start */
        );
STATIC inline MacRetType parseCAReconfigUECqiPeriodicInfo(
        UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoDl_p,
        CqiInfo *cqiInfoUl_p,
        UInt8 transmissionMode,
        UInt8 *periodicCQIEnabled_p,
        UInt8 cellIndex
        );
MacRetType validateAperiodicModeForTXMode(UInt8 aperiodicCQIMode,
                                          UInt8 transmissionMode);
STATIC inline MacRetType parseCAReconfigUECqiPMIConfigIndex(
		UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        UInt8 transmissionMode,
        UInt8 cellIndex
);
STATIC inline MacRetType parseCAUECqiFormatPeriodicSubbandK(UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        CellConfig *cellParams_p,
        UInt8 cellIndex
        );
STATIC inline MacRetType validateCqiformatIndicatorPeriodicSubbandK (UInt8 cqiPeriodicSubbandK);
STATIC inline MacRetType parseCAUECqiRIConfigIndex(UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        UInt8 transmissionMode,
        UInt8 internalCellIndex
        );
STATIC inline MacRetType validateCqiPMIConfigIndex(UInt32 cqiPMIConfigIndex
    ,UInt8 cellIndex
);
STATIC inline MacRetType validateCqiPucchResourceIndex(UInt32 cqiPucchResourceIndex
    ,UInt8 cellIndex
);
STATIC inline MacRetType validateCqiformatIndicatorPeriodic (UInt8 cqiformatIndicatorPeriodic);
STATIC inline MacRetType parseCAReconfigUECqiPMIIndexV1020(UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        UInt8 transmissionMode,
        UInt8 cellIndex
        );
STATIC inline MacRetType validateSimultaneousACKNACKAndCQI(UInt8 simultaneousACKNACKAndCQI
        ,UInt8 cellIndex
        );
STATIC inline MacRetType parseCAUECqiFormatPeriodicSubbandKV1020(
        UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        CellConfig *cellParams_p,
        UInt8 internalCellIndex
        );
STATIC inline MacRetType parseCAUECqiRIConfigIndexV1020(UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        UInt8 transmissionMode,
        UInt8 cellIndex
        );
STATIC inline MacRetType parseCALCBandwidthDistanceInfo(UInt16 tagLen,
    UInt8 **msgBuf_pp,
    ueContextForDownlink *dlUEInfo_p
    );
STATIC inline MacRetType validateCALCBandwidthDistribution(
    UInt8 bandWidthDistribution);
STATIC inline MacRetType parseScellDLHarqProc(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p
        ); 
STATIC inline MacRetType parseScellDLInitMCS(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p
        ); 
STATIC inline MacRetType parseScellNumOfLayerInfo(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p,
        UInt8 ueCategoryV1020,
        UInt8 cellIndex
        ); 
STATIC inline MacRetType parseScellCodebookIndexInfo(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p
        ,UInt8 cellIndex
        ); 
STATIC inline MacRetType parseScellUserLocationType(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p
        ); 
STATIC inline MacRetType parseScellRFParams(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p,
        UInt8 cellIndex,
        UInt16 ueIndex
        ); 
STATIC inline MacRetType parseScellActivationStatus(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p,
        UInt8 cellIndex
        ); 
/* - CA_PHASE2 */
/* - CA_AB */
#endif /* Included LTELAYER2_INIT_MAC_H */
