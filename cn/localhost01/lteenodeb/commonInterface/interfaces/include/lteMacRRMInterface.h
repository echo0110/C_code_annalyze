/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: lteMacRRMInterface.h
 *
 *************************************************************************
 *
 * File Description : This file contains the declaration of API stuctures 
 *                    and other constants for MAC-RRM Interface which will 
 *                    be used commonly 
 *************************************************************************/
#ifndef LTE_MAC_RRM_INTERFACE_H
#define LTE_MAC_RRM_INTERFACE_H

/**************************************************************************
 * Project Includes
 *************************************************************************/
#include "lteL2RRMInterface.h"

/**************************************************************************
 * Exported API Ids
 *************************************************************************/
/* API ID */
#define RRM_MAC_REQ_MESSAGE_API_START 1
#define RRM_MAC_CELL_CONFIG_REQ (RRM_MAC_REQ_MESSAGE_API_START + 0)
#define RRM_MAC_SCHEDULER_RECONFIG_REQ (RRM_MAC_REQ_MESSAGE_API_START + 1)
#define RRM_MAC_UE_RECONFIG_REQ (RRM_MAC_REQ_MESSAGE_API_START + 2)
/*RRM KPI STATS*/
#define RRM_MAC_CONFIGURE_KPI_STATS_REQ (RRM_MAC_REQ_MESSAGE_API_START + 3)
#define RRM_MAC_GET_KPI_STATS_REQ (RRM_MAC_REQ_MESSAGE_API_START + 4)
#define RRM_MAC_UE_SINR_TA_REQ (RRM_MAC_REQ_MESSAGE_API_START + 5)
/* + E_CID_5.3 */
#define RRM_MAC_E_CID_MEAS_CONFIG_REQ (RRM_MAC_REQ_MESSAGE_API_START + 6)
/* - E_CID_5.3 */

/* Perf stats RRM-MAC start */
#define RRM_MAC_CONFIGURE_PERF_STATS_REQ (RRM_MAC_REQ_MESSAGE_API_START + 7)
/* Perf stats RRM-MAC end */


#define RRM_MAC_CNF_MESSAGE_API_START 100
#define RRM_MAC_CELL_CONFIG_CNF (RRM_MAC_CNF_MESSAGE_API_START + 0)
#define RRM_MAC_SCHEDULER_RECONFIG_CNF (RRM_MAC_CNF_MESSAGE_API_START + 1)
#define RRM_MAC_UE_RECONFIG_CNF (RRM_MAC_CNF_MESSAGE_API_START + 2)
/*RRM KPI STATS*/
#define RRM_MAC_CONFIGURE_KPI_STATS_CNF (RRM_MAC_CNF_MESSAGE_API_START + 3)
#define RRM_MAC_KPI_STATS_IND           (RRM_MAC_CNF_MESSAGE_API_START + 4)
#define RRM_MAC_GET_KPI_STATS_CNF       (RRM_MAC_CNF_MESSAGE_API_START + 5)
#define RRM_MAC_KPI_THP_STATS_IND       (RRM_MAC_CNF_MESSAGE_API_START + 6)
#define RRM_MAC_GET_KPI_THP_STATS_CNF   (RRM_MAC_CNF_MESSAGE_API_START + 7)
#define RRM_MAC_UE_SINR_TA_RESP         (RRM_MAC_CNF_MESSAGE_API_START + 8)
/* + E_CID_5.3 */
#define MAC_RRM_E_CID_MEAS_REPORT_RESP  (RRM_MAC_CNF_MESSAGE_API_START + 9)
/* - E_CID_5.3 */

/* Perf stats RRM-MAC start */
#define RRM_MAC_CONFIGURE_PERF_STATS_CNF (RRM_MAC_CNF_MESSAGE_API_START + 10)
/* Perf stats RRM-MAC end */


#define RRM_MAC_IND_MESSAGE_API_START 200
#define RRM_MAC_PERIODIC_REPORT_IND (RRM_MAC_IND_MESSAGE_API_START + 0) 
/* +DYNAMIC_ICIC */
#define RRM_MAC_L1_REPORT_IND (RRM_MAC_IND_MESSAGE_API_START + 1) 
#define RRM_MAC_ICIC_REPORT_IND (RRM_MAC_IND_MESSAGE_API_START + 2) 
/* -DYNAMIC_ICIC */

/* Perf stats RRM-MAC start */
#define RRM_MAC_CELL_PERF_STATS_IND (RRM_MAC_IND_MESSAGE_API_START + 4)
/* Perf stats RRM-MAC end */

/* EICIC Change */
#define RRM_MAC_EICIC_REPORT_IND (RRM_MAC_IND_MESSAGE_API_START + 5)
/* EICIC Change */


/* API ID END */

/**************************************************************************
 * Exported macro definitions
 *************************************************************************/

/* This macro is used for declaring array of variable length */
#define VAR_SIZE(x) 1
/* +DYNAMIC_ICIC */
#define MAX_REGIONS 5  
/* -DYNAMIC_ICIC */
#define MAX_MSG_SIZE 6144


/*HD FDD Changes Start*/
/*Range 0-31. Maximum permissible values of MCS , 
 * but only 0-28 are used 29, 30 and 31 are reserved. 
 * Keeping it 32 to be byte aligned
 */
#define MAX_MCS_VALUES 32
/*HD FDD Changes End*/

/* + TM7_8 Changes Start */
#define MAX_NUM_BF_VECTORS_TO_AOA_VAL 360
#define MAX_NUM_BF_VECTORS_TO_SINR_VAL 256
/* - TM7_8 Changes End */

#define MAX_SPS_INTERVALS 16

/* macros for power related parameters */
#define MAX_NUM_PATHLOSS_TO_SINR_COUNT 10
#define MAX_NUM_ALPHA_COUNT 8
#define MAX_NUM_TPC_FOR_PUCCH 4
#define MAX_NUM_TPC_FOR_PUSCH 4
#define NUM_DCI_SIZE_CATEGORY 3
#define MAX_CQI_VAL 15
#define MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT 4

/* Macros related periodic report indication API */
#define MIN_UE_BATCH_SIZE 1
#define MAX_UE_BATCH_SIZE 25
#define DEFAULT_UE_BATCH_SIZE 10

#define MAX_QCI 9
/* +- SPR 19159 */
/* This macro will be used for maximum number of UE's of report indication
 * can be sent to RRM or for reconfiguration of UE's from RRM.*/
#define MAX_NUM_OF_UE 25
/* +- SPR 19159 */

/*START : DYN_PDCCH*/
#define MAX_AGGREGATION_LEVELS 4 
/*END   : DYN_PDCCH*/
/* CA Changes Start */
#define INVALID_CELL_ID_ERROR 200
/* CA Changes End */

/*eicic changes start*/

#define MAX_ABS_PATTERN 9

/*eicic changes end*/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/***************************************************************************
 * Enum Definitions For Error Codes
 **************************************************************************/
typedef enum RrmMacKPIStatErrorCodeT
{
    INVALID_API_LENGTH
}RrmMacKPIStatErrorCode;

typedef enum RrmMacUEReconfigErrorCodeT
{
    UE_DOES_NOT_EXIST_ERROR = 0,
    INVALID_UE_INDEX_ERROR,
    INVALID_USER_LOCATION_TYPE_ERROR,
    INVALID_NUM_OF_UE,
    MAC_RRM_SYSTEM_ERROR,
    INVALID_UE_RECONFIG_API_LENGTH,
    NO_DL_CELL_EDGE_REGION_EXIST,
    NO_UL_CELL_EDGE_REGION_EXIST,
    NO_DL_UL_CELL_EDGE_REGION_EXIST,
    INVALID_MU_MIMO_STATUS_TYPE,
    INVALID_MU_MIMO_NUM_ANTENNA_CONFIG,
    SCELL_NOT_CONFIGURED,
    SCELL_CONFIGURATION_INVALID,
}RrmMacUEReconfigErrorCode;
typedef enum RrmMacCellConfigErrorCodeT
{
    /*coverity-530 CID-32293*/
    RRM_MAC_SUCCESS=1,
    /*coverity-530 CID-32293*/
    PARAMETER_OUT_OF_RANGE=3,
    RETRY_AFTER_SOME_TIME,
    INVALID_NUMBER_OF_CC_REGION ,
    INVALID_NUMBER_OF_CE_REGION ,
    INVALID_TOTAL_RB_COUNT ,
    INVALID_CELL_CONFIG_BITMASK ,
    INVALID_REPORT_REQUIRED_VALUE ,
    INVALID_PERIODICITY ,
    INVALID_UE_BATCH_SIZE, 
    BATCH_SIZE_RECONFIG_NOT_ALLOWED,
    INVALID_PERIODIC_REPORT_CONFIG_BITMASK,
    PERIODICITY_VALUE_NOT_RECEIVED,
    UE_BATCH_SIZE_VALUE_NOT_RECEIVED,
    INVALID_CELL_CONFIG_API_LENGTH,
    START_RB_NOT_IN_INCREASING_ORDER,
    CE_USER_EXIST_CE_REGION_IS_ZERO,
    CC_REGION_OVERLAPPING,
    /* +DYNAMIC_ICIC */
    INVALID_L1_MEAS_REPORT_CONFIG_BITMASK,
    INVALID_L2_ICIC_MEAS_REPORT_CONFIG_BITMASK,
    INVALID_RNTP_REPORT_CONFIG_BITMASK,
    INVALID_AVERAGING_PARAM,
    INVALID_CE_PRB_USAGE_PARAM,
    INVALID_RNTP_THRESHOLD_VALUE,
    INVALID_EPRE_BITMASK,
    POWER_MASK_PARAMETER_OUT_OF_RANGE,
        /* -DYNAMIC_ICIC */
	/*EICIC*/
    INVALID_L2_EICIC_MEAS_REPORT_CONFIG,
    INVALID_ABS_PATTERN_CONFIG,
    INVALID_USABLE_ABS_PATTERN_CONFIG
       /*EICIC*/    

}RrmMacCellConfigErrorCode;

typedef enum RrmMacSchedulerReconfigErrorCodeT
{
    RETRY_SCHEDULER_RECONFIG_AFTER_SOME_TIME=3,
    INVALID_SCHEDULER_RECONFIG_API_LENGTH

}RrmMacSchedulerReconfigErrorCode;


/***************************************************************************
 * Structure Definitions For API Cell Config Req
 **************************************************************************/
/* This structure contains start RB and number of RBs for a particular 
 * resource partition.
 */
typedef struct ResourcePartitionT
{
    UInt32 startRB; /*^ M, 0 ^*/
    UInt32 numOfRB; /*^ M, 0 ^*/
}ResourcePartition;

/* This structure contains information about the Cell Center/Cell Edge Frequency
 * Partitions.
 */

typedef struct ResourcePartitionInfoT
{   
#define CE_RESOURCE_INFO_BITMASK 0x01
    /* represents cell edge region is present or not  */
    UInt32  bitmask; /*^ M, 0 ^*/
    /* indicates number of Cell Center Frequency Partitions */
    UInt32  numOfCellCenterRegion; /*^ M, 0 ^*/
    /*indicates number of Cell Edge Frequency Partitions */
    UInt32  numOfCellEdgeRegion; /*^ M, 0 ^*/
    ResourcePartition CellCenterRegion[MAX_REGIONS]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    ResourcePartition CellEdgeRegion; /*^ M, 0 ^*/

}ResourcePartitionInfo;

/* Configuration of Periodic Report required to be sent  by MAC to RRM. */
typedef struct PeriodicReportConfigT
{
#define PERIODIC_REPORTS_REQUIRED_BITMASK 0x0001
#define PERIODICITY_BITMASK 0x0002
#define UE_BATCH_SIZE_BITMASK 0x0004

    /* Bitmask for optional field */
    UInt32  bitmask; /*^ M, 0 ^*/

    /* indicates whether SINR/TA/Pathloss/BLER Periodic report is enabled or
     * not.
     */
    UInt32  periodicReportRequired; /*^ M, 0 ^*/

    /* indicates the value of periodicity for Periodic Reports in ms */
    UInt32  periodicity; /*^ M, 0 ^*/

    /* Indicates number of UE in one batch */
    UInt32 ueBatchSize; /*^ M, 0 ^*/
}PeriodicReportConfig;

/* +DYNAMIC_ICIC */
/* It contains the configuration of interference reports by MAC to RRM */
typedef struct Layer1MeasReportConfigT
{
#define L1_MEAS_REPORT_REQD_BITMASK 0x0001
#define L1_MEAS_REPORT_AVERAGING_ENABLED_BITMASK 0x0002
#define L1_MEAS_REPORT_PERIODICITY_BITMASK 0x0004
    /* Indicates the presence of bitmap for optional parameters*/
    UInt32 bitmask; /*^ M, 0 ^*/
    /* Indicates if meas report is required to be reported or not */
    UInt32 layer1MeasReportRequired; /*^ M, 0 ^*/
    /* Indicates if averaging is enabled or not , on the basis of the we wil do time averaging */
    UInt32 layer1ReportAveragingEnabled; /*^ M, 0 ^*/
    /* Indicates the value of periodicity for the enabled reports */
    UInt32 periodicity; /*^ M, 0 ^*/
}Layer1MeasReportConfig;

typedef struct DLRntpReportConfigInfoT
{
#define RNTP_THRESHOLD_BITMASK  0x01
#define MAX_NOMINAL_EPRE_BITMASK 0x02
    /* Indicates the presence of optional parameters */
    UInt32 bitmask; /*^ M, 0 ^*/
    /* Indicates whether RNTP reporting is required from L2 or not */
    UInt32 rntpReportRequired; /*^ M, 0 ^*/
    /* Indicates the threshold value range for RNTP , we will use an enum for range check */
    UInt32 rntpThreshold; /*^ M, 0 ^*/
    /* Indicates the max nominal energy per resource element , this parameter is
       mandatory when "rntpReportRequired" is enabled */
    SInt32 maxNominalEPRE; /*^ M, 0 ^*/
}DLRntpReportConfigInfo;

/* It contains the configuration of ICIC related reports by MAC to RRM */
typedef struct Layer2IcicMeasReportConfigT
{
#define CE_PRB_USAGE_REQD_BITMASK 0x0001
#define RNTP_CONFIG_BITMASK 0x0002
#define L2_ICIC_MEAS_REPORT_PERIODICITY_BITMASK 0x0004
    /* Indicates the presence of bitmap for optional parameters*/
    UInt32 bitmask; /*^ M, 0 ^*/
    /* Indicates if Prb usage for CE UEs in both DL & UL is required to be reported */
    UInt32 cellEdgePrbUsageRequired; /*^ M, 0 ^*/
    /* Indicates DL related parameters like RNTP threshold & EPRE value */
    DLRntpReportConfigInfo rntpReportingConfigInfo; /*^ M, 0 ^*/
    /* Indicates the value of periodicity for the enabled reports */
    UInt32 periodicity; /*^ M, 0 ^*/
}Layer2IcicMeasReportConfig;

typedef struct ULPowerMaskT
{
    /* Indicates max absolute UL Tx power with which a CC user can transmit data in UL */
    SInt32 cellCenterUserPowerMask; /*^ M, 0 ^*/
    /* Indicates max absolute UL Tx power with which a CE user can transmit data in UL */
    SInt32 cellEdgeUserPowerMask; /*^ M, 0 ^*/
    /* Indicates delta power mask for UEs based on QCI */
    SInt32 qciDeltaPowerMask[MAX_QCI];/*^ M, 0, OCTET_STRING, FIXED ^*/
}ULPowerMask;

/* -DYNAMIC_ICIC */

/*EICIC*/

typedef struct Layer2EicicMeasReportConfigT
{
    /* Indicates dl abs prb uSAGE */
    UInt32 absUsageRequired;  /*^ M, 0 ^*/
    /* Indicates the value of periodicity for the enabled reports */
    UInt32 periodicity;  /*^ M, 0 ^*/
}Layer2EicicMeasReportConfig;

typedef struct AbsPatternServingCellConfig
{
    /* Indicates abs Usage pattern for FDD*/
    UInt8 absPatternServingCell[MAX_ABS_PATTERN]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* For byte alignment */
    UInt8  pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/

}AbsPatternServingCellConfig;


/*EICIC*/

/* Structure for RRM-MAC API */
typedef struct RrmMacCellConfigReqT
{
#define DL_RESOURCE_INFO_BITMASK 0x01
#define UL_RESOURCE_INFO_BITMASK 0x02
#define PERIODIC_REPORT_CONFIG_BITMASK 0x04
#define L1_REPORT_CONFIG_BITMASK 0x08
#define ICIC_REPORT_CONFIG_BITMASK 0x10
#define CONFIG_UL_POWER_BITMASK 0x20
#define EICIC_REPORT_CONFIG_BITMASK 0x40
#define ABS_PATTERN_CONFIG_BITMASK 0x80
#define USABLE_ABS_PATTERN_CONFIG_BITMASK 0x100

    /* Represents optional field is present or not  */
    UInt32 bitmask;/*^ M, 0 ^*/

    /* Indicates the cell center and cell edge downlink frequency 
     * partitions for the cell */
    ResourcePartitionInfo  dlResourcePartitionInfo; /*^ M, 0 ^*/
    ResourcePartitionInfo  ulResourcePartitionInfo; /*^ M, 0 ^*/

    /* Indicates the configuration of Periodic Reports from MAC */
    PeriodicReportConfig  periodicReportConfig; /*^ M, 0 ^*/

    /* +DYNAMIC_ICIC */   
    /* Stores the measurement reports related to PHY layer like intereference per Prb 
     * and thermal noise etc */ 

    Layer1MeasReportConfig layer1MeasReportConfig; /*^ M, 0 ^*/
    /* Stores the icic measurement related configuration like CE Prb usage and RNTP reports 
     * configuration which will be comming from RRM */
    Layer2IcicMeasReportConfig layer2IcicMeasReportConfig; /*^ M, 0 ^*/
    /* This IE denotes the value with which CC and CE user can be scheduled in uplink */
    ULPowerMask ulPowerMask; /*^ M, 0 ^*/
    /*EICIC*/
    Layer2EicicMeasReportConfig layer2EicicMeasReportConfig; /*^ M, 0 ^*/
    AbsPatternServingCellConfig absPatternServingCellConfig;  /*^ M, 0 ^*/
    AbsPatternServingCellConfig usableAbsPatternServingCellConfig;  /*^ M, 0 ^*/

    /*EICIC*/

    /* -DYNAMIC_ICIC */
}RrmMacCellConfigReq; /*^ API, RRM_MAC_CELL_CONFIG_REQ ^*/ 

/***************************************************************************
 * Structure Definitions For API UE Reconfig Req 
 **************************************************************************/
/*UL_MU_MIMO_CHG Start*/

typedef enum ULMUMIMOAlgoTypeT
{
    UL_MU_MIMO_RANDOM_SELECTION = 0,
    MAX_UL_MU_MIMO_TYPE_VALUE = 1
}ULMUMIMOAlgoType;

typedef enum ULMUMIMOStatusT
{
    UL_MU_MIMO_DISABLED = 0,
    UL_MU_MIMO_ENABLED = 1
}ULMUMIMOStatus;

/*This structure contains information about UE whether UL MU-MIMO is applicable to UE or not.
 * This information is sent whenever there is a change in status*/
typedef struct UEULMUMIMOInfoT
{
    UInt32 ueIndex; /*^ M, 0 ^*/
    /* 0 - DISABLED
     * 1 - ENABLED
     */
    UInt32 ulMUMIMOStatus;/*^ M, 0 ^*/ /*Value will be taken from enum ULMUMIMOStatus*/
}UEULMUMIMOInfo;

typedef UEULMUMIMOInfo UEULMUMIMOInfoNode;

/*This structure contains UEs list for which UL MU-MIMO status is changed*/
typedef struct UEULMUMIMOInfoListT
{
    /*numOfUE indicates the number of UE for which UL MU-MIMO status modification is reported
     * max value of numOfUE is limited to 25*/
    UInt32 numOfUE; /*^ M, 0 ^*/

    UEULMUMIMOInfo ueUlMUMIMOInfo[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UEULMUMIMOInfoList;

/*UL_MU_MIMO_CHG End*/

typedef struct UELocationT
{
    UInt32  ueIndex; /*^ M, 0 ^*/
    UInt32  ueLocationType; /*^ M, 0 ^*/
    /*CA changes start*/
    UInt8   cellBitMask; /*^ M, 0 ^*/
    UInt8   ueLocationPerScell[MAX_SCELLS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /*CA changes end*/
    /* For byte alignment */
    UInt8  pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UELocation;

/* EICIC +-*/
typedef struct UEUserTypeT
{
    UInt32  ueIndex; /*^ M, 0 ^*/
    UInt8    userType; /*^ M, 0 ^*/
}UEUserType;
/* EICIC -*/

/* contains information about UE location(cell centre or cell edge) for multiple
 *  UEs combined in a single message.
 */
typedef struct UELocationInfoT
{
    UInt32  numOfUE; /*^ M, 0 ^*/
    UELocation  ueLocation[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UELocationInfo;


/* + TM7_8 Changes Start*/

typedef struct  UENumOfLayerT
{
    UInt32 ueIndex; /*^ M, 0 ^*/    
    UInt8 numOfLayer; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8  pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UENumOfLayer;

typedef struct  UENumOfLayerInfoT
{
    /* numOfUE indicates the number of UE for which Num Of Layer is reported */
    UInt32 numOfUE; /*^ M, 0 ^*/
    UENumOfLayer   ueNumOfLayer[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UENumOfLayerInfo;
     
typedef UENumOfLayer UENumOfLayerNode;

typedef struct  UEBeamformingAlgoT
{
    UInt32 ueIndex;  /*^ M, 0 ^*/
    /* 0 = SINR, 1 = AOA */
    UInt8 beamformingAlgoToBeUsed;   /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8  pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UEBeamformingAlgo;

typedef struct  UEBeamformingAlgoToBeUsedInfoT
{
    /* numOfUE indicates the number of UE for which Beam forming is reported */
    UInt32 numOfUE;  /*^ M, 0 ^*/
    UEBeamformingAlgo ueBeamformingAlgo[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UEBeamformingAlgoToBeUsedInfo;
     
typedef UEBeamformingAlgo UEBeamformingAlgoNode;

/* - TM7_8 Changes End*/

/* CA changes start*/
/* This structure contains information about UE CA config.*/
typedef struct UEScellSchedulingInfoT
{
    UInt32 ueIndex; /*^ M, 0 ^*/
    UInt8 isMACCESendToUE; /*^ M, 0 ^*/
    UInt8 numOfScellsSchedulingAllowed; /*^ M, 0, READ ^*/
    UInt8 numOfScellsSchedulingNotAllowed; /*^ M, 0, READ ^*/
    UInt8 schedulingActivateScell[MAX_SCELLS]; /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfScellsSchedulingAllowed ^*/
    UInt8 schedulingDeactivateScell[MAX_SCELLS]; /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfScellsSchedulingNotAllowed ^*/
    /* For byte alignment */
    UInt8  pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UEScellSchedulingInfo;

typedef UEScellSchedulingInfo UEScellSchedulingInfoNode;

/*This structure contains UEs list for which CA config info is changed*/
typedef struct UECarrierAggConfigInfoListT
{
    /*numOfUE indicates the number of UE for which CA config info modification
      is reported max value of numOfUE is limited to 25*/
    UInt32 numOfUE; /*^ M, 0 ^*/
    UEScellSchedulingInfo ueScellSchedulingInfo[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UECarrierAggConfigInfoList;

/*This structure contains information about cqi validity time for  UE.*/
typedef struct UECqiValidityTimerInfoT
{
    UInt32 ueIndex; /*^ M, 0 ^*/
    UInt8 cqiValidityTimer; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8  pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UECqiValidityTimerInfo;

typedef UECqiValidityTimerInfo UECqiValidityTimerInfoNode;

/*This structure contains UEs list for which cqi validity timer 
    value is changed*/
typedef struct UECqiValidityTimerInfoListT
{
    /*numOfUE indicates the number of UE for which  cqi validity timer 
      value modification is reported max value of numOfUE is limited to 25*/
    UInt32 numOfUE; /*^ M, 0 ^*/
    UECqiValidityTimerInfo ueCqiValidityTimerInfo[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UECqiValidityTimerInfoList;
/* CA changes end */

/*EICIC Changes starts*/

typedef struct UESevereDlControlChannelInterferenceInfoT
{
    UInt32 ueIndex; /*^ M, 0 ^*/
    UInt8 SevereDlControlChannelInterference; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8  pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}SevereDlControlChannelInterferenceInfo;


typedef struct UESevereDlControlChannelInterferenceInfoListT
{
    /*numOfUE indicates the number of UE for which victim/non-victim status is received 
      value modification is reported max value of numOfUE is limited to 25*/
    UInt32 numOfUE; /*^ M, 0 ^*/
    SevereDlControlChannelInterferenceInfo SevereDlControlChannelInterferenceInfo[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UESevereDlControlChannelInterferenceInfo;

/*EICIC changes Ends*/

/* reconfigure the UE Location i.e.ifUE location changes from Cell Center to
 * Cell Edge UE and vice-versa. Initial User Location type is informed through
 * RRM via RRC during UE admission Procedure.*/
typedef struct RrmMacUeReconfigReqT
{
#define UE_LOCATION_RECONFIG_BIT 0x01
#define UE_ULMUMIMO_RECONFIG_BIT 0x02
/* + TM7_8 Changes Start*/
#define UE_NUM_OF_LAYER_RECONFIG_BIT 0x04
#define UE_BEAM_FORMING_ALGO_RECONFIG_BIT 0x08
/* - TM7_8 Changes End*/
    /* CA changes start*/
#define UE_CA_CONFIG_INFO_LIST_RECONFIG_BIT 0x10
#define UE_CQI_VALIDITY_TIMER_LIST_RECONFIG_BIT 0x20
#define UE_DL_CONTROL_CHANNEL_INTERFERENCE_LIST_RECONFIG_BIT 0x40
    /* CA changes end*/
    UInt32  bitmask; /*^ M, 0 ^*/
    UELocationInfo  ueLocationInfo; /*^ M, 0 ^*/
    /*UL_MU_MIMO_CHG Start*/
    UEULMUMIMOInfoList ueUlMUMIMOInfoList; /*^ M, 0 ^*/
    /*UL_MU_MIMO_CHG End*/
    /* + TM7_8 Changes Start*/
    UENumOfLayerInfo ueNumOfLayerInfo; /*^ M, 0 ^*/
    UEBeamformingAlgoToBeUsedInfo ueBeamformingAlgoToBeUsedInfo; /*^ M, 0 ^*/
    /* - TM7_8 Changes End*/
    /* CA changes start*/
    UECarrierAggConfigInfoList ueCarrierAggConfigInfoList; /*^ M, 0 ^*/
    /* SPR 16422 START */
    /* This field is currently not sent by RRM, instead the global value is sent in 
     * reconfig scheduler API which will be applied to all UE */
    UECqiValidityTimerInfoList ueCqiValidityTimerInfoList; /*^ M, 0 ^*/
    /* SPR 16422 END */
    /*CA changes end*/
    /*EICIC Changes Starts*/
   UESevereDlControlChannelInterferenceInfo ueSevereDlControlChannelInterferenceInfo; /*^ M, 0 ^*/
 /*EICIC Changes Ends*/
}RrmMacUeReconfigReq; /*^ API, RRM_MAC_UE_RECONFIG_REQ ^*/

/***************************************************************************
 * Structure Definitions For API Periodic Report Indication 
 **************************************************************************/
/* contains Uplink/Downlink SNR, Uplink/Downlink BLER,uplink pathloss and Timing
 * Advance information for the UE 
 */
/* + TM7_8 Changes Start */
#define MAX_NUM_ANTENNA 4
/* - TM7_8 Changes End */

/* CA changes start*/
typedef struct SCellPeriodicReportT
{
    UInt32 bitMask; /*^ M, 0 ^*/
    UInt8 downlinkSINRValueCW0; /*^ M, 0 ^*/
    UInt8 downlinkSINRValueCW1; /*^ M, 0 ^*/
    UInt8 downlinkBlerValueCW0; /*^ M, 0 ^*/
    UInt8 downlinkBlerValueCW1; /*^ M, 0 ^*/
    UInt8 downlinkSINRValueCW0Layers2; /*^ M, 0 ^*/
    UInt8 downlinkSINRValueCW1Layers2; /*^ M, 0 ^*/
    UInt8 downlinkBlerValueCW0Layers2; /*^ M, 0 ^*/
    UInt8 downlinkBlerValueCW1Layers2; /*^ M, 0 ^*/

}SCellPeriodicReport;    
/* CA changes end */
/*EICIC*/
typedef struct csiSubframeSet2PeriodicReportT
{
    UInt32 bitMask; /*^ M, 0 ^*/
    UInt8 downlinkSINRValueCW0_2; /*^ M, 0 ^*/
    UInt8 downlinkSINRValueCW1_2; /*^ M, 0 ^*/
    UInt8 downlinkBlerValueCW0_2; /*^ M, 0 ^*/
    UInt8 downlinkBlerValueCW1_2; /*^ M, 0 ^*/
    UInt8 downlinkSINRValueCW0Layers2_2; /*^ M, 0 ^*/
    UInt8 downlinkSINRValueCW1Layers2_2; /*^ M, 0 ^*/
    UInt8 downlinkBlerValueCW0Layers2_2; /*^ M, 0 ^*/
    UInt8 downlinkBlerValueCW1Layers2_2; /*^ M, 0 ^*/

}csiSubframeSet2PeriodicReport;  
/*EICIC*/

typedef struct PeriodicReportInfoT
{
    /* CA changes start */
    UInt32 bitMask; /*^ M, 0 ^*/
    /* CA changes end */
    UInt32  ueIndex; /*^ M, 0 ^*/
    UInt32  uplinkSINRValue; /*^ M, 0 ^*/
    UInt32  downlinkSINRValueCW0; /*^ M, 0 ^*/
    UInt32  downlinkSINRValueCW1; /*^ M, 0 ^*/
    UInt32  uplinkBlerValue; /*^ M, 0 ^*/
    UInt32  downlinkBlerValueCW0; /*^ M, 0 ^*/
    UInt32  downlinkBlerValueCW1; /*^ M, 0 ^*/
    UInt32 uplinkPathLossValue; /*^ M, 0 ^*/
    UInt32 timingAdvanceValue; /*^ M, 0 ^*/
    /*4x4 DL MIMO CHG START*/
    UInt8 downlinkSINRValueCW0Layers2; /*^ M, 0 ^*/
    UInt8 downlinkSINRValueCW1Layers2; /*^ M, 0 ^*/
    UInt8 downlinkBlerValueCW0Layers2; /*^ M, 0 ^*/
    UInt8 downlinkBlerValueCW1Layers2; /*^ M, 0 ^*/
    /*4x4 DL MIMO CHG END*/
    /* SPR 5033 changes start */
    UInt32 pucchSINRValue; /*^ M, 0 ^*/
    UInt32 pucchBlerValue; /*^ M, 0 ^*/
    /* SPR 5033 changes end */
    /* Added for TTI Bundling: Start */
    /* SPR 8104 */
    /* For TTIB_Code */
    /* + SPR 19724 */
    UInt32 uplinkPRB; /*^ M, 0 ^*/
    /* - SPR 19724 */
    /* SPR 8104 */
    /* + TM7_8 Changes Start */
    UInt16 AoA[MAX_NUM_ANTENNA]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* - TM7_8 Changes End */
    /* Added for TTI Bundling: End */
    /* CA changes start */
    UInt8 scellReportBitMask; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8  pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    SCellPeriodicReport scellReport[MAX_SCELLS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* CA changes end*/
    /*EICIC*/
    csiSubframeSet2PeriodicReport csiSubframeSet2Report;  /*^ M, 0 ^*/ 
    /*EICIC*/

}PeriodicReportInfo;

/* provide various stats that include TA, SNR, pathloss, BLER */
typedef struct RrmMacPeriodicReportsIndT
{
    UInt32  numOfUE;  /*^ M, 0 ^*/
    PeriodicReportInfo reportInfo[VAR_SIZE(numOfUE)]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}RrmMacPeriodicReportsInd; /*^ API, RRM_MAC_PERIODIC_REPORT_IND ^*/

/* +DYNAMIC_ICIC */
/* provides various interference related stats coming from L1*/
typedef struct MacRrmLayer1MeasReportIndT
{
#define INTERFERENCE_PER_PRB_BITMASK 0x01
#define THERMAL_NOISE_BITMASK 0x02
#define RS_TX_POWER_BITMASK 0x04
   /* Indicates the presence of optional parameters */
   UInt32 bitmask;  /*^ M, 0 ^*/
   /* Indicates the filtered & time averaged interference per PRB info */
   SInt32 interferencePerPRB[100]; /*^ M, 0, OCTET_STRING, FIXED ^*/
   /* Indicates the thermal noise statistics */
   SInt32 thermalNoise;  /*^ M, 0 ^*/
   /* Indicates DL reference signal transmission power */
   SInt32 dlRsTxPower;  /*^ M, 0 ^*/
}MacRrmLayer1MeasReportInd; /*^ API, RRM_MAC_L1_REPORT_IND ^*/
/* SPR_18211 start */
/*Wiresect Changes*/
/* SPR_18211 stop */
/* SPR_18211 start */
/*Wiresect Changes*//*Done*/
/* SPR_18211 stop */

/* Provides various ICIC realted stats */
typedef struct MacRrmLayer2IcicMeasReportIndT
{
#define RNTP_VAL_BITMASK 0x01
#define DL_CE_PRB_USAGE_BITMASK 0x02
#define UL_CE_PRB_USAGE_BITMASK 0x04
#define ABS_SUBFRAME_COUNT_BITMASK 0x08
    /* Indicates the presence of optional parameters */
    UInt32 bitmask; /*^ M, 0 ^*/
    /* Indicates the rntp value per PRB indication 0 or 1 */
    UInt8  rntpValuePerPRB[100]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* Indicates the downlink CE PRB usage */
    UInt32 dlCEPrbUsage; /*^ M, 0 ^*/
    /* Indicates the uplink CE PRB usage */
    UInt32 ulCEPrbUsage; /*^ M, 0 ^*/
    /*EICIC Changes starts: cout of ABS subframes during reporting period*/
    UInt32 absSubframeCount; /*^ M, 0 ^*/
   /*EICIC Changes Ends*/
}MacRrmLayer2IcicMeasReportInd; /*^ API, RRM_MAC_ICIC_REPORT_IND ^*/
/* -DYNAMIC_ICIC */
/*EICIC Changes Starts*/

typedef struct MacRrMEICICAbsReportIndT
{      
	UInt8 absPrbUsage; /*^ M, 0 ^*/
	UInt8 absPerQci[MAX_QCI]; /*^ M, 0 ^*/
}MacRrMEICICAbsReportInd; /*^ API, RRM_MAC_EICIC_REPORT_IND ^*/

/*EICIC Changes Ends*/
/***************************************************************************
 * Structure Definitions For API Scheduler Reconfig Req 
 **************************************************************************/
/* Structue definition for API element alphaBasedPathLossToTargetSINRMap */
typedef struct PathLossToTargetSINRMapInfoT
{
    /* Starting value of the range of pathloss which maps to targetSINR. 
     * It is always less than or equal to endPL. 
     * Range (-230 to 460 dB in steps of 10).
     */
    SInt32 startPL; /*^ M, 0 ^*/
    /* End value of the range of pathloss which maps to targetSINR. 
     * It is always greater than or equal to startPL. 
     * Range (-230 to 460dB in steps of 10).
     */
    SInt32 endPL; /*^ M, 0 ^*/
    /* Target SINR to be maintained when a UE is experiencing a pathloss given
     * by the range startPL to endPL. 
     * Range (0 to 255) representing -64dBm to 63.5dBm in steps of 0.5.
     */
    UInt32 targetSINR; /*^ M, 0 ^*/
}PathLossToTargetSINRMapInfo;

typedef struct PathLossToTargetSINRMapT
{
    /* This field shall indicate the number of tuples in 
     * pathLossToTargetSINRMapInfo array. Range(1,2,..,9,10)
     */
    UInt32 count; /*^ M, 0 ^*/
    /* Stores the target SINR for a range of pathloss. Number of these
     * ranges of pathloss is given by count above. 
     * Different ranges provided for each target SINR should not overlap
     * and must cover all values from -230 to 460dB. There should not be
     * any gap between two sets of {startPL, endPL}.  Two consecutive sets
     * must represent contiguous values.
     */
    PathLossToTargetSINRMapInfo pathLossToTargetSINRMapInfo[MAX_NUM_PATHLOSS_TO_SINR_COUNT];/*^ M, 0, OCTET_STRING, FIXED ^*/
}PathLossToTargetSINRMap;

typedef struct AlphaBasedPathLossToTargetSINRMapT
{
    /*This pathloss value shall be used a default pathloss in system 
      and and target SINR based on this pathloss 
      value shall be used as initial SINR */
    UInt32  defaultPathLoss; /*^ M, 0 ^*/

    /* This field is an array of 8 elements(one for each value of alpha)
     * where index of the array corresponds to path loss to target SINR 
     * mapping for alpha = 0, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 and 1 respectively.
     */
    PathLossToTargetSINRMap pathLossToTargetSINRMap[MAX_NUM_ALPHA_COUNT]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}AlphaBasedPathLossToTargetSINRMap;

/* Structure definition for API element blerToTPCMapForPUCCH */
typedef struct DeltaBLERToTPCMapForPUCCHInfoT
{
    /* Starting value of the range of delta BLER which maps to pucchTPC. It is 
     * always less than or equal to endDeltaBLER. 
     * Range (-pucchBLERTarget to 100 - pucchBLERTarget in steps of 1).
     */
    SInt32 startBLER; /*^ M, 0 ^*/
    /* End value of the range of delta BLER which maps to pucchTPC. It is 
     * always greater than or equal to startDeltaBLER. 
     * Range (-pucchBLERTarget to (100 - pucchBLERTarget) in steps of 1).
     */
    SInt32 endBLER; /*^ M, 0 ^*/
    /* TPC for PUCCH. Range (-1,0,1,2,3). These are mapped to exact TPC commands
     * by PUCCH power control algorithm.
     */  
    SInt32 pucchTPC; /*^ M, 0 ^*/
}DeltaBLERToTPCMapForPUCCHInfo;

typedef struct DeltaBLERToTPCMapForPUCCHT 
{
    /* Block Error Rate that needs to be maintained on PUCCH. 
     * Range (0,1,...,99,100)
     */
    UInt32 pucchBLERTarget; /*^ M, 0 ^*/
    /* This field indicates the number of valid entries in 
     * deltaBlerToTPCMapForPUCCHInfo array. 
     * Range (1 to 4).
     */
    UInt32 count; /*^ M, 0 ^*/
    /* This field contains the mapping of ranges delta BLER(difference of BLER 
     * measured on PUCCH and pucchBLERTarget) to TPC for PUCCH. 
     * Range of delta bler should cover all values from 
     * [-pucchBLERTarget to (100 - pucchBLERTarget)]. Any two sets of 
     * {startBLER, endBLER} should be continuous ranges without any gap between 
     * the sets.
     */
    DeltaBLERToTPCMapForPUCCHInfo deltaBlerToTPCMapForPUCCHInfo[MAX_NUM_TPC_FOR_PUCCH];  /*^ M, 0, OCTET_STRING, FIXED ^*/
}DeltaBLERToTPCMapForPUCCH;

/* Structue definition for API element pdcchAggregationAndPowerOffset */
typedef struct AggregationPowerOffsetInfoT
{
    /* This field shall indicate the aggregation level to be used for 
     * CCE allocation. Range(1,2,4 or 8).
     */ 
    UInt32   aggregationLevel; /*^ M, 0 ^*/
    /* This field shall indicate the value of delta power offset to be used for
     * PDCCH on top of configured PDCCH power offset. 
     * Range(0,1,2,...,9998,9999,10000) representing -6 dB to 4 dB in 
     * 0.001 dB steps.
     */ 
    UInt32  powerOffset; /*^ M, 0 ^*/
}AggregationPowerOffsetInfo;

typedef struct AggregationPowerOffsetT
{
    /* This field shall indicate the number of possible tuples 
     * that can be tried for aggregationLevel/powerOffset sets for 
     * PDCCH power control. Range(1,2,3,4)
     */
    UInt32 count;/*^ M, 0 ^*/
    /* This is an array of the type AggregationPowerOffsetInfo  which 
     * shall store "n" tuples where n is given by count.
     */
    AggregationPowerOffsetInfo  aggregationPowerOffsetTuples[MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}AggregationPowerOffset;

typedef struct PdcchAggregationAndPowerOffsetInfoT 
{
#define AGGREGATION_POWER_OFFSET_CE_USER 0x01
    /* Bitmask for optional field */
    UInt32 bitmask; /*^ M, 0 ^*/

    /* aggregationPowerOffset two dimensional array specifies the aggregation 
     * level and power offset tuples based on DCI category size(either of 
     * categories SMALL_DCI_STEP_SIZE, MEDIUM_DCI_STEP_SIZE and 
     * BIG_DCI_STEP_SIZE) and wideband CQI(Range 1 to 15).
     */
    AggregationPowerOffset aggregationPowerOffsetCCUser[NUM_DCI_SIZE_CATEGORY][MAX_CQI_VAL]; /*^ M, 0, OCTET_STRING, FIXED ^*/

    AggregationPowerOffset aggregationPowerOffsetCEUser[NUM_DCI_SIZE_CATEGORY][MAX_CQI_VAL]; /*^ M, 0, OCTET_STRING, FIXED ^*/

}PdcchAggregationAndPowerOffsetInfo;

/* Structure definition for API element deltaSinrToTPCMapForPUCCH */
typedef struct DeltaSINRToTPCMapForPUCCHInfoT
{
    /* Starting value of the range of delta SINR which maps to pucchTPC. 
     * It is always less than or equal to endDeltaSINR. 
     * Range (-pucchSINRTarget to (255 - pucchSINRTarget) in steps of 1)
     */
    SInt32 startSINR; /*^ M, 0 ^*/
    /* End value of the range of delta SINR which maps to pucchTPC. 
     * It is always greater than or equal to startDeltaSINR. 
     * Range (-pucchSINRTarget to 255 - pucchSINRTarget in steps of 1)
     */
    SInt32 endSINR; /*^ M, 0 ^*/
    /* TPC for PUCCH. Range (-1,0,1,2,3). These are mapped to exact TPC commands
     * by PUCCH power control algorithm.
     */
    SInt32 pucchTPC; /*^ M, 0 ^*/
}DeltaSINRToTPCMapForPUCCHInfo;

typedef struct DeltaSINRToTPCMapForPUCCHT
{
    /* Target SINR for PUCCH. Possible values are 0 to 255 representing 
     * -64dBm to 63.5dBm in steps of 0.5
     */
    UInt32 pucchSINRTarget; /*^ M, 0 ^*/
    /* This field indicates the number of valid entries in the 
     * deltaSinrToTPCMapForPUCCHInfo array. 
     * Range (1 to 4).
     */
    UInt32 count; /*^ M, 0 ^*/
    /* This field contains the mapping of ranges delta SINR
     * (difference of SINR measured on PUCCH and pucchSINRTarget) 
     * to TPC for PUCCH.
     * Range of delta sinr should cover all values from 
     * [-pucchSINRTarget to (255 - pucchSINRTarget)]. All sets of 
     * {startSINR, endSINR} should be continuous ranges without any gap between 
     * the sets.
     */
    DeltaSINRToTPCMapForPUCCHInfo deltaSinrToTPCMapForPUCCHInfo[MAX_NUM_TPC_FOR_PUCCH]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}DeltaSINRToTPCMapForPUCCH;

/* Structure definition for API element deltaSINRToTPCMapForPUSCH */
typedef struct DeltaSINRToTPCMapForPUSCHInfoT
{
    /* Starting value of the range of delta SINR which maps to puschTPC. 
     * It is always less than or equal to endDeltaSINR. 
     * Range (-255 to 255 in steps of 1).
     */
    SInt32  startDeltaSINR; /*^ M, 0 ^*/
    /* End value of the range of delta SINR which maps to puschTPC. 
     * It is always greater than or equal to startDeltaSINR. 
     * Range (-255 to 255 in steps of 1).
     */
    SInt32 endDeltaSINR; /*^ M, 0 ^*/
    /* TPC for PUSCH. Range (-4 to 4). These are mapped to exact TPC commands by
     * PUSCH power control algorithm based on whether accumulation is 
     * enabled or not.
     */
    SInt32 puschTPC; /*^ M, 0 ^*/
}DeltaSINRToTPCMapForPUSCHInfo;

typedef struct DeltaSINRToTPCMapForPUSCHT
{
    /* This field indicates the number of valid entries in 
     * deltaSINRToTPCMapForPUSCHInfo array. 
     * Range (1 to 4).
     */
    UInt32 count; /*^ M, 0 ^*/
    /* This field contains the mapping of ranges delta SINR to TPC for
     * PUSCH.
     * Different ranges provided for each TPC should not overlap
     * and must cover all values from -255 to 255. There should not be any gap 
     * between two sets of {startDeltaSINR, endDeltaSINR}. Two consecutive sets
     * must represent contiguous values.
     */
    DeltaSINRToTPCMapForPUSCHInfo deltaSINRToTPCMapForPUSCHInfo[MAX_NUM_TPC_FOR_PUSCH]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}DeltaSINRToTPCMapForPUSCH;

/* Structure definition for API element cqiToPHICHPower */
typedef struct CqiToPHICHPowerT
{
    /* Contains the mapping of wideband CQI to delta PHICH power offset to be 
     * used on top of configured PHICH Power. Wideband CQI(range 1 to 15) is 
     * used as index to array cqiToPHICHPowerInfo to get the delta PHICH Power
     * offset in the range 0 to 10000 power offset. 
     * Range(0,1,2,...,9998,9999,10000) representing -6 dB to 4 dB in 
     * 0.001 dB steps such that total PHICH power is in range 0 to 10000.
     */
    UInt32 cqiToPHICHPowerInfo[MAX_CQI_VAL]; /*^ M, 0, OCTET_STRING, FIXED ^*/
} CqiToPHICHPower;

/* SPS_CHG */
/*!
  * \code
  *  typedef struct SpsDlSchedulingInfoPerTtiT
  *  {
  *      Max number of SPS activation, reactivation or deactivation that can be scheduled in a TTI for downlink.
  *      UInt8 maxDlSpsDciPerTti;
  *      Maximum number of SPS occasions that can be configured in a TTI for downlink
  *      UInt8 maxDlSpsOccasionPerTti;
  *      Maximum number of total RBs that can be reserved for SPS per TTI for downlink
  *      UInt8 maxDlSpsRbsPerTti
  *      Maximum number of RBs that can  be reserved for SPS per TTI for downlink for a given interval
  *      UInt8 maxDlSpsRbsPerTtiPerInterval[MAX_SPS_INTERVALS];
  *  }
  * \endcode
  */
typedef struct SpsDlSchedulingInfoPerTtiT 
{
    /* Max number of SPS activation, reactivation or deactivation that can be 
     * scheduled in a TTI for downlink.*/
    UInt32 maxDlSpsDciPerTti; /*^ M, 0 ^*/
    
    /* Maximum number of SPS occasions that can be configured in a TTI for 
     * downlink */
    UInt32 maxDlSpsOccasionPerTti; /*^ M, 0 ^*/
    
    /* Maximum number of total RBs that can be reserved for SPS per TTI for 
     * downlink */
    UInt32 maxDlSpsRbsPerTti; /*^ M, 0 ^*/
    
    /* Maximum number of RBs that can  be reserved for SPS per TTI for downlink
     * for a given interval */
    UInt32 maxDlSpsRbsPerTtiPerInterval[MAX_SPS_INTERVALS]; /*^ M, 0, OCTET_STRING, FIXED ^*/

}SpsDlSchedulingInfoPerTti, *LP_SpsDlSchedulingInfoPerTti;

 /*!
  * \code
  *  typedef struct SpsUlSchedulingInfoPerTtiT
  *  {
  *      Max number of SPS activation, reactivation or deactivation that can be scheduled in a TTI for Uplink.
  *      UInt8 maxUlSpsDciPerTti;
  *      Maximum number of SPS occasions that can be configured in a TTI for Uplink
  *      UInt8 maxUlSpsOccasionPerTti;
  *      Maximum number of total RBs that can be reserved for SPS per TTI for Uplink
  *      UInt8 maxUlSpsRbsPerTti
  *      Maximum number of RBs that can  be reserved for SPS per TTI for Uplink for a given interval
  *      UInt8 maxUlSpsRbsPerTtiPerInterval[MAX_SPS_INTERVALS];
  *  }
  * \endcode
  */
typedef struct SpsUlSchedulingInfoPerTtiT 
{
    /* Max number of SPS activation, reactivation or deactivation that can be
     * scheduled in a TTI for Uplink.*/
    UInt32 maxUlSpsDciPerTti; /*^ M, 0 ^*/
    /* Maximum number of SPS occasions that can be configured in a TTI for 
     * Uplink */
    UInt32 maxUlSpsOccasionPerTti; /*^ M, 0 ^*/
    /* Maximum number of total RBs that can be reserved for SPS per TTI for 
     * Uplink */
    UInt32 maxUlSpsRbsPerTti; /*^ M, 0 ^*/
    /* Maximum number of RBs that can  be reserved for SPS per TTI for Uplink 
     * for a given interval */
    UInt32 maxUlSpsRbsPerTtiPerInterval[MAX_SPS_INTERVALS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}*LP_SpsUlSchedulingInfoPerTti, SpsUlSchedulingInfoPerTti;
/* for UL cell config request */
/* SPS_CHG */

/*START : DYN_PDCCH*/
typedef struct DynamicCFIExtensionParamsT
{
    UInt8 cceCorrectionStepUpFactor[MAX_AGGREGATION_LEVELS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt8 cceCorrectionStepDownFactor[MAX_AGGREGATION_LEVELS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt8 cceAdjustFactor[MAX_AGGREGATION_LEVELS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}DynamicCFIExtensionParams;
/*END   : DYN_PDCCH*/
/* +DYNAMIC_ICIC */
/* This structure contains configurations of ATB related parameters comming 
 * from RRM to MAC */
typedef struct ATBConfigT
{
    /* The value upto which MCS index could be reduced for validation of the maximum 
     * power in the ATB functionality. Its range is (0..28)*/
    UInt32 minMCSIndexForATB; /*^ M, 0 ^*/
    /* The value upto which PRB number could be reduced for validation of the maximum 
     * power in the ATB functionality. Its range is 1..100 */
    UInt32 minPRBValForATB; /*^ M, 0 ^*/
}ATBConfig;
/* -DYNAMIC_ICIC */

/* + TM7_8 Changes Start */
typedef struct sinrToBeamFormingConfigT 
{
    /* Hold bf values for two antennas */
    UInt16 beamFormingVectorForA1; /*^ M, 0 ^*/
}sinrToBeamFormingConfig;
/* - TM7_8 Changes End */

/*+ Dyn UEs To Be SchPerTTI +*/
typedef struct MaxUEsToBeScheduledDlUlT
{
    UInt8  maxUEsToBeScheduledDL; /*^ M, 0 ^*/
    UInt8  maxUEsToBeScheduledUL; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8  pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}MaxUEsToBeScheduledDlUl; 
/*- Dyn UEs To Be SchPerTTI -*/

/* CA changes start*/
typedef struct CAParamsT
{
#define LOAD_CAL_WINDOW_SIZE            0x0001
#define LOAD_DISPARITY_THRESHOLD        0x0002
#define MCS_DIFF_THRESHOLD              0x0004
#define PRB_USAGE_WATER_MARK            0x0008
#define DL_DEACT_COMMON_THREAD          0x0010
#define DL_DEACT_MCS_LOW_THRESH         0x0020
#define DL_QLOAD_UP_THRESH              0x0040
#define DL_QLOAD_DOWN_THRESH            0x0080
#define ACT_DEACT_TIG_COUNT_THRESH      0x0100
/* SPR 16422 START */
#define CQI_VALIDITY_TIMER              0X0200
/* SPR 16422 END */
    UInt32 bitMask; /*^ M, 0 ^*/
    UInt16 loadCalcualtionWindowSize; /*^ M, 0 ^*/
    UInt16 dlQLoadUpThresh; /*^ M, 0 ^*/
    UInt16 dlQLoadLowThresh; /*^ M, 0 ^*/
    UInt8 loadDisparityThreshold; /*^ M, 0 ^*/
    UInt8 mcsDiffThreshold; /*^ M, 0 ^*/
    UInt8 prbUsageWaterMark; /*^ M, 0 ^*/
    UInt8 dlDeactCommandThresh; /*^ M, 0 ^*/
    UInt8 dlDeactMCSLowThresh; /*^ M, 0 ^*/
    UInt8 actDeactTriggerCountThresh; /*^ M, 0 ^*/
    /* SPR 16422 START */
    UInt8 cqiValidityTimer; /*^ M, 0 ^*/
    /* SPR 16422 END */
    /* For byte alignment */
    UInt8  pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}CAParams;
/* CA changes end */
/*CA phase-3 changes */
#define RRM_SCELL_QLOAD_THRESH_MULTIPLIER     100
typedef struct RrmMacReconfigSchedulerReqT
{
#define TPC_FOR_MSG3_BIT 0x0001
#define SRS_SUBBAND_GRP_SIZE_BIT 0x0002
#define TPC_TRIGGER_WAIT_TIME_PUSCH_DCI0_BIT 0x0004
#define TPC_TRIGGER_WAIT_TIME_PUCCH_DCI1x2x_BIT 0x0008
#define TPC_NUM_UE_THRESHOLD_PUSCH_DCI3A 0x0010
#define TPC_NUM_UE_THRESHOLD_PUCCH_DCI3A 0x0020
#define COOL_OFF_PRD_FOR_BLER_BIT 0x0040
#define COOL_OFF_PERIOD_FOR_PUCCH_BIT 0x0080
#define COOL_OFF_PERIOD_PUSCH_BIT 0x0100
#define ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_BIT 0x0200
#define DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_BIT 0x0400
#define BLER_TO_TPC_MAP_FOR_PUCCH 0x800
#define DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_BIT 0x1000
#define PDCCH_AGG_AND_POW_OFF_BIT 0x2000
#define CQI_TO_PHICH_POW_BIT 0x4000
#define MIN_RB_FOR_PL_PHR_CALC_BIT 0x8000
    /* SPS_CHG */
#define SPS_DL_SCHEDULING_INFO_PER_TTI_BIT 0x10000
#define SPS_UL_SCHEDULING_INFO_PER_TTI_BIT 0x20000
#define ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_SPS_BIT 0x40000
    /* SPS_CHG */
    /*HD FDD Changes Start*/
#define HD_REDUCED_MCS_TABLE_BIT 0x00080000
    /*HD FDD Changes End*/
    /*START : DYN_PDCCH*/
#define DYN_PDCCH_EXTENSION_PARAMS_BIT 0x00100000
    /*END   : DYN_PDCCH*/
    /* +DYNAMIC_ICIC */
#define ATB_CONFIG_BIT 0x00200000
    /* -DYNAMIC_ICIC */
    /*UL_MU_MIMO_CHG_START*/
#define UPLINK_MU_MIMO_TYPE_BIT 0x00400000
#define UPLINK_MU_MIMO_MCS_THRESHOLD_BIT 0x00800000
    /*UL_MU_MIMO_CHG_END*/
    /* +DYNAMIC_ICIC */
#define PA_QPSK_BIT 0x01000000
    /* -DYNAMIC_ICIC */

    /* + TM7_8 Changes Start */
#define AOA_TO_BF_VECTOR_BIT 0x02000000
#define SINR_TO_BF_VECTOR_BIT 0x04000000
    /* - TM7_8 Changes End */

    /*+ Dyn UEs To Be SchPerTTI +*/
#define UES_TO_BE_SCHEDULED_DL_UL_PER_TTI 0x08000000
    /*- Dyn UEs To Be SchPerTTI -*/

    /* CA changes start */
#define CA_PARAMS_BIT 0x10000000
    /* CA changes end */
    /* SPR 22761 Fix +- */
#define EMBMS_PARAM_BIT 0x20000000

    /* corresponds to bitmap for optional field */
    UInt32 schedulerParamBitMap; /*^ M, 0 ^*/

    /* Specifies the TPC Command that needs to be sent in RAR for Msg3 power
     * control */
    SInt32 tpcForMsg3; /*^ M, 0 ^*/

    /* used to group PRBs according to received SINR in order to reduce
     * processing in MAC*/
    UInt32 srsSubBandGroupSize; /*^ M, 0 ^*/

    /* Wait time for sending triggered TPC in DCI0 before DCI 3/3A is scheduled
     * */
    UInt32 tpcTriggerWaitTimeForPUSCHDCI0; /*^ M, 0 ^*/
    UInt32 tpcTriggerWaitTimeForPUCCHDCI1x2x; /*^ M, 0 ^*/

    /*Percentage threshold used by MAC to decide whether to schedule DCI 3A TPC command for a TPC-PUSCH-RNTI group.DCI     3 command is scheduled if the following condition holds true. 
     *  (Number of UEs sharing the TPC-PUSCH-RNTI for which TPC 
     *      has been triggered  /  Total number of UEs configured to share 
     *          the TPC- PUSCH- RNTI)  * 100   >= 
     *              tpcNumUeThresholdForPUSCHDCI3A
     */              
    UInt32 tpcNumUeThresholdForPUSCHDCI3A; /*^ M, 0 ^*/

    /* Percentage threshold used by MAC to decide whether to schedule DCI 3A TPC command for a TPC-PUCCH-RNTI group. DCI 3A command is scheduled if the following condition holds true.
       (Number of UEs sharing the TPC-PUCCH-RNTI for which TPC 
       has been triggered  /  Total number of UEs configured to share 
       the TPC- PUCCH- RNTI)  * 100   >= 
       tpcNumUeThresholdForPUCCHDCI3A
     */
    UInt32 tpcNumUeThresholdForPUCCHDCI3A; /*^ M, 0 ^*/


    UInt32  coolOffPeriodForBLERTriggeredPUCCHTPC; /*^ M, 0 ^*/
    UInt32  coolOffPeriodForPUCCHTPC; /*^ M, 0 ^*/
    UInt32  coolOffPeriodForPUSCHTPC; /*^ M, 0 ^*/

    /* specifies the SINR target based on a range of pathloss. */
    AlphaBasedPathLossToTargetSINRMap alphaBasedPathLossToTargetSINRMap; /*^ M, 0 ^*/

    /* specifies the TPC to be used for PUSCH based on delta SINR */
    DeltaSINRToTPCMapForPUSCH deltaSINRToTPCMapForPUSCH; /*^ M, 0 ^*/

    /* specifies the PUCCH target BLER and the mapping of delta BLER to TPC for
     * PUCCH */
    DeltaBLERToTPCMapForPUCCH deltaBlerToTPCMapForPUCCH; /*^ M, 0 ^*/

    /* specifies the PUCCH target SINR and the mapping of delta SINR to TPC for
     * PUCCH */
    DeltaSINRToTPCMapForPUCCH deltaSINRToTPCMapForPUCCH; /*^ M, 0 ^*/

    /* specifies the set of aggregation levels that need to be tried for CCE
     * allocation based on DCI size category and wideband CQI.*/
    PdcchAggregationAndPowerOffsetInfo pdcchAggregationAndPowerOffset; /*^ M, 0 ^*/

    /* specifies the correction to PHICH power offset to be used for PHICH power
     * control based on wideband CQI */
    CqiToPHICHPower  cqiToPHICHPower; /*^ M, 0 ^*/

    UInt32 minRbForPlPhrCalc; /*^ M, 0 ^*/

    /*UL_MU_MIMO_CHG_START*/
    /*This parameter signifies that whether the selection of a buddy UE will be random or
     * any other type (however currently Random selection is usable).
     * 0 - UL MU-MIMO Random Selection is enabled.  Default value of this parameter should be set to 0
     *     (the same will be used if the parameter is not present)
     * Other values are for future use */

    UInt32 ulMUMIMOType; /*^ M, 0 ^*/

    /* Threshold value of MCS required as an input to consider a UE candidate for Uplink MU-MIMO transmission.
     * Range of the IE is from 0 to 28. Default value should be set to 6 */
    UInt32 mcsThresholdULMUMIMO; /*^ M, 0 ^*/
    /*UL_MU_MIMO_CHG_END*/

    /* SPS_CHG */
    /* Maximum number of SPS activation, reactivation or deactivation which 
     * can be scheduled in a TTI for downlink */
    SpsDlSchedulingInfoPerTti spsDlSchInfoPerTti; /*^ M, 0 ^*/

    /* Maximum number of SPS activation, reactivation or deactivation which 
     * can be scheduled in a TTI for uplink */
    SpsUlSchedulingInfoPerTti spsUlSchInfoPerTti; /*^ M, 0 ^*/

    /* Pathloss to target SINR mapping table for different values of alpha. 
     * This table shall be used for PUSCH fractional power control 
     * (TPC commands) when a UE has SPS activated.*/
    AlphaBasedPathLossToTargetSINRMap alphaBasedPathLossToTargetSINRMapSps; /*^ M, 0 ^*/
    /* SPS_CHG */
    /*HD FDD Changes Start*/
    /*Reduced MCS Table used for HD FDD UE in case of DL to UL Switch*/
    UInt8 hdReducedMcsTable[MAX_MCS_VALUES]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /*HD FDD Changes End*/
    /*START : DYN_PDCCH*/
    DynamicCFIExtensionParams dynamicCFIExtensionParams; /*^ M, 0 ^*/
    /*END   : DYN_PDCCH*/

    /* +DYNAMIC_ICIC */   
    ATBConfig atbConfig; /*^ M, 0 ^*/
    /* power offset from reference signal for QPSK */
    UInt32 paQpsk; /*^ M, 0 ^*/
    /* -DYNAMIC_ICIC */   
    /* + TM7_8 Changes Start */
    UInt16 angleOfArrivalToBeamFormingVectorMapping[MAX_NUM_BF_VECTORS_TO_AOA_VAL]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    sinrToBeamFormingConfig sinrToBeamFormingVectorMapping[MAX_NUM_BF_VECTORS_TO_SINR_VAL]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* - TM7_8 Changes End */
    /*+ Dyn UEs To Be SchPerTTI +*/
    MaxUEsToBeScheduledDlUl maxUEsToBeScheduledDlUl; /*^ M, 0 ^*/
    /*- Dyn UEs To Be SchPerTTI -*/

    /* CA changes start*/
    CAParams caParams; /*^ M, 0 ^*/
    /* CA changes end*/
    /* SPR 22761 Fix +- */
    UInt8 eMbmsReservedCellPowerOffset; /*^ M, 0 ^*/
}RrmMacReconfigSchedulerReq; /*^ API, RRM_MAC_SCHEDULER_RECONFIG_REQ ^*/


/*************************************************************************************
* Confirmation structures for RRM-MAC APIs.
************************************************************************************/
/* This structure gives confirmation for cell config request from RRM */

/*UL_MU_MIMO_CHG Start*/
/*This structure contains the ueIndex and Error code of those UEs whose MUMIMO reconfig fails*/
typedef struct UEULMUMIMOErrorT
{
    UInt32                     ueIndex; /*^ M, 0 ^*/
    UInt32                     errorCode;/*^ M, 0 ^*/ /*Value will be taken from enum RrmMacUEReconfigErrorCode*/
}UEULMUMIMOError;

/*This structure contains information about UE UL MU-MIMO update for multiple UEs combined in a single message*/
typedef struct UEULMUMIMOErrorInfoT
{
    UInt32 numOfUE; /*^ M, 0 ^*/
    UEULMUMIMOError ueULMUMIMOError[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UEULMUMIMOErrorInfo;

/*UL_MU_MIMO_CHG End*/

typedef struct RrmMacCellConfigCnfT
{
    /* Return either MAC_SUCCESS or MAC_FAILURE  */
    UInt32 responseCode;  /*^ M, 0 ^*/

}RrmMacCellConfigCnf; /*^ API, RRM_MAC_CELL_CONFIG_CNF ^*/

typedef struct UELocationErrorT
{
    UInt32                     ueIndex;  /*^ M, 0 ^*/
    UInt32                     errorCode;  /*^ M, 0 ^*/
}UELocationError;

typedef struct UELocationErrorInfoT
{
    UInt32 numOfUE;  /*^ M, 0 ^*/
    UELocationError ueLocationError[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UELocationErrorInfo;

/* + TM7_8 Changes Start*/
typedef struct  UENumOfLayerErrorT
{
    UInt32  ueIndex; /*^ M, 0 ^*/
    /*Value will be taken from enum  RrmMacUENumOfLayerErrorCode*/
    UInt32  errorCode; /*^ M, 0 ^*/
}UENumOfLayerError;


typedef struct UENumOfLayerErrorInfoT
{
    UInt32  numOfUE; /*^ M, 0 ^*/
    UENumOfLayerError  ueNumOfLayerError[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UENumOfLayerErrorInfo;

typedef struct  UEBeamFormingAlgoErrorT
{
    UInt32  ueIndex; /*^ M, 0 ^*/
    /* Value will be taken from enum  RrmMacUEBeamFormingAlgoErrorCode */
    UInt32  errorCode;  /*^ M, 0 ^*/
}UEBeamFormingAlgoError;

typedef struct UEBeamformingAlgoErrorInfoT
{
    UInt32  numOfUE; /*^ M, 0 ^*/
    UEBeamFormingAlgoError  ueBeamFormingAlgoError[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UEBeamformingAlgoErrorInfo;

/* - TM7_8 Changes End*/

/* CA changes start*/
/* Enum of possible error code when reconfiguration of UE CA config fails*/
/* This structure contains the ueIndex and Error code of those UEs whose CA reconfig fails*/
typedef struct CarrierAggConfigErrorT
{
    UInt32    ueIndex; /*^ M, 0 ^*/
    UInt32    errorCode;/*^ M, 0 ^*/ /* Value taken from enum RrmMacUECarrierAggConfigErrorCode*/
}UECarrierAggConfigError;

/* This structure contains information about CA config update for multiple UEs 
 * combined in a single message*/
typedef struct UECarrierAggConfigErrorInfoT
{
    UInt32 numOfUE; /*^ M, 0 ^*/
    UECarrierAggConfigError ueCarrierAggConfigError[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UECarrierAggConfigErrorInfo;
/* Enum of possible error code when reconfiguration of UE Cqi validity Timer info fails */
/* This structure contains the ueIndex and Error code of those UEs whose reconfig of cqi
 * validity timer info fails*/
typedef struct CqiValidityTimerErrorT
{
    UInt32                     ueIndex; /*^ M, 0 ^*/
    UInt32                     errorCode; /*^ M, 0 ^*/ /* Value taken from enum RrmMacUEReconfigErrorCode*/
}UECqiValidityTimerError;

/* This structure contains information about cqi validity timer update for multiple
 * UEs combined in a single message*/
typedef struct UECqiValidityTimerErrorInfoT
{
    UInt32 numOfUE; /*^ M, 0 ^*/
    UECqiValidityTimerError ueCqiValidityTimerError[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UECqiValidityTimerErrorInfo;
/* CA changes end */

/*EICIC changes Starts*/
typedef struct UEInterferenceInfoErrorT
{
    UInt32                     ueIndex;  /*^ M, 0 ^*/
    UInt32                     errorCode;  /*^ M, 0 ^*/
}UEInterferenceInfoError;

typedef struct UEDlControlChannelInterferenceErrorInfoT
{
    UInt32 numOfUE;  /*^ M, 0 ^*/
    UEInterferenceInfoError ueInterferenceInfoError[MAX_NUM_OF_UE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}UEDlControlChannelInterferenceErrorInfo;

/*EICIC Changes Ends*/
/* This structure gives confirmation for UE config request from RRM */
typedef struct RrmMacUEReConfigCnfT
{
#define UE_LOCATION_ERROR_INFO_BITMASK 0x01
#define UE_MUMIMO_RECONFIG_ERROR_INFO_BITMASK 0x02
    /* + TM7_8 Changes Start */
#define UE_NUM_OF_LAYER_ERROR_INFO_BITMASK 0x04
#define UE_BEAM_FORMING_ALGO_ERROR_INFO_BITMASK 0x08
    /* - TM7_8 Changes End */
    /* CA changes start*/
#define UE_CA_CONFIG_ERROR_INFO_BITMASK 0x10
#define UE_CQI_VALIDITY_TIMER_ERROR_INFO_BITMASK 0x20
#define UE_CONTROL_CHANNEL_INTERFERENCE_ERROR_INFO_BITMASK 0x40 /* EICIC +-*/
    /* CA changes end*/
    /* Bitmask for optional field */
    UInt32 bitmask; /*^ M, 0 ^*/

    /* Return either MAC_SUCCESS or MAC_FAILURE  */
    UInt32 responseCode; /*^ M, 0 ^*/

    /* Error code associated to each UE if any */
    UELocationErrorInfo ueLocationErrorInfo; /*^ M, 0 ^*/

    /*UL_MU_MIMO_CHG Start*/
    UEULMUMIMOErrorInfo ueULMUMIMOErrorInfo; /*^ M, 0 ^*/
    /*UL_MU_MIMO_CHG End*/

    /* + TM7_8 Changes Start*/
    UENumOfLayerErrorInfo ueNumOfLayerErrorInfo; /*^ M, 0 ^*/
    UEBeamformingAlgoErrorInfo ueBeamFormingAlgoErrorInfo; /*^ M, 0 ^*/
    /* - TM7_8 Changes End*/
    /* CA changes start*/
    UECarrierAggConfigErrorInfo ueCarrierAggConfigErrorInfo; /*^ M, 0 ^*/
    UECqiValidityTimerErrorInfo ueCqiValidityTimerErrorInfo; /*^ M, 0 ^*/
    /* CA changes end */
    /*EICIC changes Starts*/
   UEDlControlChannelInterferenceErrorInfo ueDlControlChannelInterferenceErrorInfo; /*^ M, 0 ^*/
    /*EICIC Changes Ends*/


}RrmMacUEReConfigCnf; /*^ API, RRM_MAC_UE_RECONFIG_CNF ^*/

/* This structure gives confirmation for scheduler reconfig request from RRM */
typedef struct RrmMacReconfigSchedulerCnfT
{
    /* Return either MAC_SUCCESS or MAC_FAILURE  */
    UInt32 responseCode; /*^ M, 0 ^*/
}RrmMacReconfigSchedulerCnf; /*^ API, RRM_MAC_SCHEDULER_RECONFIG_CNF ^*/

/* + E_CID_5.3 */
typedef struct RrmMacEcidMeasConfigReqT
{
    UInt16 ueIndex; /*^ M, 0 ^*/
    UInt8 type1_eNBRxTxRequested; /*^ M, 0 ^*/
    UInt8 type2_eNBRxTxRequested; /*^ M, 0 ^*/
    UInt8 AoARequested; /*^ M, 0 ^*/
    UInt8 eCidMeasId; /*^ M, 0 ^*/
    /* + SPR_8082 */
    UInt16 targetSFNForeNBRxTx; /*^ M, 0 ^*/
    /* - SPR_8082 */
    
}RrmMacEcidMeasConfigReq; /*^ API, RRM_MAC_E_CID_MEAS_CONFIG_REQ ^*/

typedef struct Type1EnbRxTxReportT
{
    UInt16 systemFrameNum; /*^ M, 0 ^*/
    UInt16 type1EnbRxTxDiffValue; /*^ M, 0 ^*/
}Type1EnbRxTxReport;
typedef struct Type2EnbRxTxReportT
{
    UInt16 systemFrameNum; /*^ M, 0 ^*/
    UInt16 type2EnbRxTxDiffValue; /*^ M, 0 ^*/
}Type2EnbRxTxReport;
typedef struct AoAReportT
{
    UInt16 angleOfArrivalValue[MAX_NUM_ANTENNA]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt16 systemFrameNum; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8  pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
} AoAReport;
typedef struct MacRrmEcidMeasurementRespT
{
#define INVALID_E_CID_MEAS_CONFIG_API_LENGTH 3
/* SPR 9927 fix start */
#define TYPE_1_TA_REPORT_BITMASK 0x0001
#define TYPE_2_TA_REPORT_BITMASK 0x0002
#define AOA_REPORT_BITMASK 0x0004
/* SPR 9927 fix end */
    UInt32 bitmask; /*^ M, 0 ^*/
    UInt16 ueIndex; /*^ M, 0 ^*/
    UInt8 responseCode; /*^ M, 0 ^*/
    UInt8 eCidMeasId; /*^ M, 0 ^*/
    Type1EnbRxTxReport type1EnbRxTxReport; /*^ M, 0 ^*/
    Type2EnbRxTxReport type2EnbRxTxReport; /*^ M, 0 ^*/
    AoAReport angleOfArrivalReport; /*^ M, 0 ^*/
}MacRrmEcidMeasurementResp; /*^ API, MAC_RRM_E_CID_MEAS_REPORT_RESP ^*/

/* SPR 9068 Fix Start */
/* Structure definition for API : RRM_MAC_CONFIGURE_KPI_STATS_CNF
 * Structure name               :MacRrmConfigureKpiStatsResp */
typedef struct MacRrmConfigureKpiStatsRespT {
    UInt32 responseCode; /*^ M, 0 ^*/         // MAC_SUCCESS or an Error Code
} MacRrmConfigureKpiStatsResp; /*^ API, RRM_MAC_CONFIGURE_KPI_STATS_CNF ^*/
/* SPR 9068 Fix End */

#ifdef PERF_STATS
/* Perf stats RRM-MAC start */
typedef struct RrmMacConfigPerfStatsReqT
{
    UInt32 duration; /*^ M, 0 ^*/
    UInt32 periodicReporting; /*^ M, 0 ^*/
    UInt32 uePerfStatsToReportBitMap; /*^ M, 0 ^*/
    UInt32 cellPerfStatsToReportBitMap;  /*^ M, 0 ^*/

}RrmMacConfigPerfStatsReq; /*^ API, RRM_MAC_CONFIGURE_PERF_STATS_REQ ^*/

typedef struct RrmMacConfigPerfStatsCnfT
{
    UInt32 responseCode; /*^ M, 0 ^*/
}RrmMacConfigPerfStatsCnf; /*^ API, RRM_MAC_CONFIGURE_PERF_STATS_CNF ^*/
#endif
/* - E_CID_5.3 */
#endif
