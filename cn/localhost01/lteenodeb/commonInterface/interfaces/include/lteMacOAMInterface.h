/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacOAMInterface.h,v 1.2.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacOAMInterface.h,v $
 * Revision 1.1.1  2012/07/20 14:56:27  gur31292
 * Changes related to DCI Format3A
 *
 * Revision 1.2.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.7  2009/12/17 07:15:27  gur11912
 * removed TICK_IN_MS
 *
 * Revision 1.6  2009/08/28 13:27:53  gur19413
 * added new macro for logger
 *
 * Revision 1.5  2009/07/14 13:02:49  gur11083
 * incorporated review comments
 *
 * Revision 1.4  2009/06/24 10:15:08  gur20470
 * included the header files lteMacParseOAMMsg.h
 *
 * Revision 1.3  2009/06/07 09:07:19  gur19413
 * added new macro
 *
 * Revision 1.2  2009/06/07 08:47:29  gur20470
 * updated for mac 1.2
 *
 * Revision 1.1  2009/05/05 11:16:38  gur19413
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_OAM_INTERFACE_H
#define LTE_MAC_OAM_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* SPR 23209 FIXED START */
#define MAX_NUM_MOCN_OPERATOR 6
#define INVALID_OPERATOR_ID 0xFF
#define MOCN_PRIORITY_FACTOR_0     0
#define MOCN_PRIORITY_FACTOR_600   600
#define MOCN_PRIORITY_FACTOR_1200 1200
#define MOCN_SRB_PRIORITY_FACTOR 1200
/* SPR 23209 FIXED END*/



#define MAC_ROHC_PROFILE_INTF      4
#define MAX_TRANSMISSION_MODES     8

/* This macro is used for declaring array of variable length */
#define MAC_OAM_VAR_SIZE(x) 1

/* Transaction Id used for Phy error indication to be sent to OAM */
#define MAC_PHY_OAM_ERR_IND_TRANS_ID 0xFFFF
/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* Following enum defines the types of errors that needs to be sent to OAM
 * when subframe error is received from PHY */
typedef enum
{
    /* The received message is not valid in the PHY's current state */
    MAC_PHY_OAM_ERR_MSG_INVALID_STATE = 1,
    /* The DL_CONFIG.request was received with a different SFN than the PHY
     * expected. This error is received for more than a threshold  */
    MAC_PHY_OAM_ERR_SFN_OUT_OF_SYNC_BY_MORE_THAN_THRESHOLD,
    /* A BCH PDU was expected in the DL_CONFIG.request message for this
     * subframe, However, it was not present */
    MAC_PHY_OAM_ERR_MSG_BCH_MISSING,
    /* An error was received in HI_DCI.request */
    MAC_PHY_OAM_ERR_MSG_HI_ERR
} MacPhyOamSubframeErrorType;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* MOCN Changes Start */
/* SPR 23402 Changes Start */
typedef struct MocnPerOperatorParamsT
{
    UInt8  operator_id;      //unique id of EPC 
    UInt8 dedicatedDLPRBsPerOperator; //percentage of total RBs
    UInt8 dedicatedULPRBsPerOperator; // percentage of total RBs
    UInt8 reservedDLPRBsPerOperator; //percentage of total RBs
    UInt8 reservedULPRBsPerOperator; //percentage of total RBs
}MocnPerOperatorParams;

typedef struct MocnParamsT {
    UInt8 numOfOperator; /*^ M, 0 ^*/
    UInt16 prbUsageMeasForgettingFactor; /*^ M, 0 ^*/ //Forgetting factor in percentage
    UInt16 prbUsageMeasResetPeriod; /*^ M, 0 ^*/ // millisecond
    MocnPerOperatorParams mocnPerOperatorParams[MAX_NUM_MOCN_OPERATOR]; /*^ M, 0 ^*/
} MocnParams;
/* MOCN Changes End */
/* SPR 23402 Changes End */
/* CA Changes start */

/* Error Codes */
#define MAC_INVALID_UE_ID                                  3
#define MAC_UE_ID_EXISTS                                   4
#define MAC_UE_NOT_EXISTS                                  5
#define MAC_MAX_ACTIVE_UE_REACHED_ERR                      6
#define MAC_CELL_NOT_CONFIGURED                            7
#define MAC_UE_NOT_VALID_ON_CELL                           8


#define MAC_INTERNAL_ERROR                                 11
#define MAC_SYNTAX_ERROR                                   12

#define MAC_LAYER_NOT_INITIALIZED                          14
#define MAC_LAYER_ALREADY_INITIALIZED                      15
#define MAC_RESET_PENDING                                  16
/* CA TDD Changes Start */
/* Code Removed */
/* CA TDD Changes End */
#define MAC_INIT_LAYER_REQ_MULTI_CELL_WITHOUT_DL_UL_SPLIT  18
#define MAC_INIT_LAYER_REQ_CA_INVALID_SCH_STRATEGY         19
#define MAC_INIT_LAYER_REQ_INVALID_NUM_OF_CELLS            20
#define MAC_INIT_CELL_REQ_RNTI_RANGE_CLASHED               21
#define MAC_INIT_LAYER_REQ_MEMORY_FAILURE                  22
#define MAC_INIT_CELL_REQ_MEMORY_FAILURE                   23
#define MAC_LAYER_CELL_IGNORE                              24

#define MAC_MAX_ULLOG_CH_CONFIG                            32
#define MAC_MAX_DLLOG_CH_CONFIG                            33

#define MAC_NO_ACTIVE_UE_IN_SYSTEM                         35


#define CA_NOT_SUPPORTED 0 
#define CA_SUPPORTED     1
#define MAX_COMMCHWITHMIBALLOWED_VALUE 1
#define MAX_DLSCHWITHMIBALLOWED_VALUE 1
#define MIN_IS_LAST_ATTEMPT 0
#define MAX_IS_LAST_ATTEMPT 1
#define MAX_DL_RB_RESTRICTION_FACTOR 3
#define MIN_DL_RB_RESTRICTION_FACTOR 1
#define MAX_UL_RB_RESTRICTION_FACTOR 3
#define MIN_UL_RB_RESTRICTION_FACTOR 1
/*+- SPR 20896 Fix*/
 /*code removed*/
/*+- SPR 20896 Fix*/
#define MAX_QOS_STRATEGY_OVER_ALLOC_FACTOR 100
#define MIN_AMBR_RKT_DEPTH_FACTOR 1
#define MAX_AMBR_RKT_DEPTH_FACTOR 5
#define MIN_GBR_RKT_DEPTH_FACTOR 1
#define MAX_GBR_RKT_DEPTH_FACTOR 5
#define MIN_TOKEN_PERIODICITY   10
#define MAX_TOKEN_PERIODICITY   100
#define MAX_PDB_WEIGHT 10
#define MAX_QCI_WEIGHT 10
#define MAX_TOKEN_WEIGHT 10
#define MAX_QLOAD_WEIGHT 10
#define MAX_MCS_WEIGHT  10
#define DEFAULT_PDB_WEIGHT 10
#define DEFAULT_QCI_WEIGHT 10
#define DEFAULT_TOKEN_WEIGHT 10
#define DEFAULT_QLOAD_WEIGHT 10
#define DEFAULT_MCS_WEIGHT  10
#define DEFAULT_AMBR_RKT_DEPTH_FACTOR 2
#define DEFAULT_GBR_RKT_DEPTH_FACTOR 2
#define DEFAULT_TOKEN_PERIODICITY 20
#define DEFAULT_QOS_STRATEGY_OVER_ALLOC_FACTOR 20
#define MIN_UE_SUPPORTED 1
#define MIN_UL_UE_SCHEDULED 1
#define MIN_DL_UE_SCHEDULED 1 
#define MIN_DL_CE_USERS_SCHEDULED 1
#define MIN_UL_CE_USERS_SCHEDULED 1
/* SPR 12975 Changes Start */ 
#define DEFAULT_DL_GBR_TOKEN_LIMIT_FACTOR -1 
#define DEFAULT_DL_AMBR_TOKEN_LIMIT_FACTOR -1
/* SPR 12975 Changes Start */ 
typedef enum MacInitLayerReqOptionalParamsT
{
  NUM_OF_CELLS_RECEIVED = 0x0001,/* first bit set */
  IS_CA_SUPPORTED_RECEIVED = 0x0002/* second bit set */
}MacInitLayerReqOptionalParams;

typedef enum MacInitOptionalParamsT
{
    DL_SCHEDULAR_STRATEGY          = 0x0001,
    UL_SCHEDULAR_STRATEGY          = 0x0002,
    DL_PFS_PARAMS                  = 0x0004,
    UL_PFS_PARAMS                  = 0x0008,
    DL_QOS_PARAMS                  = 0x0010,
    UL_QOS_PARAMS                  = 0x0020,
    SPS_PARAMS                     = 0x0040,
    ICIC_PARAMS                    = 0x0080,
}MacInitOptionalParams;

typedef struct QosStrategyWeightsT {
    /* Indicates the PDB weight */
    UInt8  pdbWeight; /*^ M, 0 ^*/
    /* Indicates the QCI weight */
    UInt8  qciWeight; /*^ M, 0 ^*/
    /* Indicates the Token weight */
    UInt8 tokenWeight; /*^ M, 0 ^*/
    /* Indicates the QLOAD weight */
    UInt8 qLoadWeight; /*^ M, 0 ^*/
    /* Indicates the MCS weight */
    UInt8 mcsWeight; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8 pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}QosStrategyWeights;

/* The below structure used to store the Qos details for each QCI. 
 * This will be updated during the MAC_INIT API.
 */

typedef enum {
    QOS_GBR = 0,
    NON_GBR = 1,
    SRB  = 2
}LogicalChannelType;

typedef struct QosQciInfoT
{
    /* Indicates the resource Type of the QCI */
    LogicalChannelType eResType; /*^ M, 0 ^*/
    /* indicates the Packet delay budget (Latency) of the QCI */
    UInt16 pktDlyBdgt; /*^ M, 0 ^*/
    /* Indicates the QOC class Indentifier (QCI) */
    UInt8 QCI; /*^ M, 0 ^*/
    /* Indicates the Priority of the QCI */
    UInt8 priority; /*^ M, 0 ^*/
    /* Indicates the Error Loss Rate */
    UInt8 pktErrLossRate; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8 pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}QosQciInfo;

/* Following structure defines the SPS system wide parameters */
typedef struct SpsSysWideParamT
{
    /* list of DL SPS intervals supported. By default only 20ms shall be 
     * enabled and rest of the values disabled. 
     * Values from index 10 to 15 are for future extensions */
    UInt8 supportedSpsDlIntervalList[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* list of UL SPS intervals supported. By default only 20ms shall be 
     * enabled and rest of the values disabled. 
     * Values from index 10 to 15 are for future extensions */
    UInt8 supportedSpsUlIntervalList[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* Array containing list of QCIs for which SPS is enabled. Indices 
     * corresponds to QCI. Value corr to SPS enabled/disabled for that QCI. */
    UInt8  qciSpslist[MAX_QCI] ; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* for enbling/disabling SPS scheduling in MAC */
    UInt8  enableSps; /*^ M, 0 ^*/
    /* Number of times DCI will be sent for SPS (re)/(de) activation */
    UInt8 maxSpsDciAttempt; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8 pading; /*^ M, 0 ^*/
} *LP_SpsSysWideParam, SpsSysWideParam ;

typedef enum JitterHandlingOptionE
{
    NO_HANDLING           = 0,
    MULTI_SDU_SCH_BY_DRA  = 1,
    PACKET_DROP           = 2,
    MAX_JITTER_HANDLING_OPT
}JitterHandlingOption;

typedef struct SpsDLIntervalProfileT
{
    /* When packets accumulate in DL RLC SDU Q, then following options are 
     * supported -
     * 1. Multiple SDUs scheduled in bulk using dynamic grant
     * 2. occasionally drop SDUs from head of the Q
     */
    JitterHandlingOption  dlJitterHandlingOption; /*^ M, 0 ^*/
    /* Indicates the margin to cater packet size variation in DL */
    UInt16 dlTbsizeMarginSps; /*^ M, 0 ^*/
    /* minimum mcs required for SPS in DL */
    UInt8  dlMinMcsSps; /*^ M, 0 ^*/
    /* Inicates MCS degraded by this amount by strategy in DL */
    UInt8  dlMcsMargin; /*^ M, 0 ^*/
    /* Num of RLC SDUs in DL Q, after which dyanmic scheduling shall be 
     * considered. This parameter is used when jitter option 1 is configured.
     */
    UInt8  rlcSduThresForDra; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8 pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
} SpsDLIntervalProfile, *LP_SpsDLIntervalProfile;

typedef struct SpsULIntervalProfileT
{
    /* Indicates the margin to cater packet size variation in UL */
    UInt16 ulTbSizeMarginSps; /*^ M, 0 ^*/
    /* minimum mcs required for SPS in UL */
    UInt8  ulMinMcsSps; /*^ M, 0 ^*/
    /* Inicates MCS degraded by this amount by strategy in UL */
    UInt8  ulMcsMargin; /*^ M, 0 ^*/
}SpsULIntervalProfile, *LP_SpsULIntervalProfile;


/* This structure will be used for Interface Only */
typedef struct SpsCommIntervalProfileIntfT
{
    /* Max PDCP PDU size per ROHC Profile . The unit is bytes.
     * the index for array will support following values -
     *    0 = RTP Profile
     *    1 = IP Profile
     *    2 = UDP Profile
     *    3 = NO ROHC */
    UInt32 maxPdcpPduSizeIpv4[MAC_ROHC_PROFILE_INTF]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* Max PDCP PDU size per ROHC Profile . The unit is bytes.
     * the index for array will support following values -
     *    0 = RTP Profile
     *    1 = IP Profile
     *    2 = UDP Profile
     *    3 = NO ROHC */
    UInt32 maxPdcpPduSizeIpv6[MAC_ROHC_PROFILE_INTF]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* When enabled, if SPS occasion is not utilized by SPS UE (due to zero Q
     * load), then the SPS resources can be allocated to other UE in DL only 
     * for given TTI.*/
    UInt8 overrideSpsOccassionForOtherUe; /*^ M, 0 ^*/
    /* When enabled, data of only SPS LC can be sent in MAC TB 
       for SPS occasion in DL*/
    UInt8 strictSpsResourceUsageForSpsLc; /*^ M, 0 ^*/
    /* When enabled, eNB shall detect silence period and deactivate SPS */
    UInt8 enableSilenceDetection;
    /* Interpacket arrival time of silence packets after which silence 
       period is detected. The unit of parameter is number of SPS interval */
    UInt8 interPktDelayForSilenceDetection; /*^ M, 0 ^*/
    /* If due to MCS improvement or lower packet size, number of RBs that can 
     * be saved is equal to this number than reactivation shall be triggered by
     * SPS strategy */
    UInt8  minRbSavedReactivationThres;  /*^ M, 0 ^*/
    /* The sps BLER Threshold.If SPS BLER is more than this value then, SPS 
     * should be released */
    UInt8  spsBlerThres;  /*^ M, 0 ^*/
    /* Duration, in terms of sps interval, after which a configured SPS 
     * resource should be reevaluated for allocation of SPS resouces, which can
     * lead to saving RBs due to improved channel conditions, reduced SDU size
     * or availablity of a TTI where RBs are free on the UEs better subband. 
     * When 0, no audit shall be done */
    UInt8 auditInterval; /*^ M, 0 ^*/
    /* Number of TTIs in advance before which SPS resources are allocated and
     * indicated to SPS strategy, so that activation/reactivation can be
     * initiated */
    UInt8 spsResAllocTtiAdv;  /*^ M, 0 ^*/
}SpsCommIntervalProfileIntf,  *LP_SpsCommIntervalProfileIntf;

/* Structure definition for API :MAC_INIT_LAYER_REQ
 * Structure name               :MacInitReqParams */
typedef struct MacInitReqParamsT {
    UInt32 fieldBitMask; /*^ M, 0 ^*/
    UInt16 numOfUE; /*^ M, 0 ^*/
    UInt16 rntiStartRange; /*^ M, 0 ^*/
    UInt16 rntiEndRange; /*^ M, 0 ^*/
    /* Indicates Token update periodicity */
    UInt16 tokenPeriodicity; /*^ M, 0 ^*/
    /*number of cells to support at L2*/
    UInt8 numCells; /*^ M, 0 ^*/
    UInt8  dlUeQueueLoadWt; /*^ M, 0 ^*/
    UInt8  dlLcQueueLoadWt; /*^ M, 0 ^*/
    UInt8  dlMsWt; /*^ M, 0 ^*/
    UInt8  dlMs64QAMFactorWt; /*^ M, 0 ^*/
    UInt8  dlMs16QAMFactorWt; /*^ M, 0 ^*/
    UInt8  dlMsQPSKFactorWt; /*^ M, 0 ^*/
    UInt8  dlUePriorityWt; /*^ M, 0 ^*/
    UInt8  dlLcPriorityWt; /*^ M, 0 ^*/
    UInt8  dlUeDelayWt; /*^ M, 0 ^*/
    UInt8  dlLcDelayWt; /*^ M, 0 ^*/
    UInt8  ulMsWt; /*^ M, 0 ^*/
    UInt8  ulMs64QAMFactorWt; /*^ M, 0 ^*/
    UInt8  ulMs16QAMFactorWt; /*^ M, 0 ^*/
    UInt8  ulMsQPSKFactorWt; /*^ M, 0 ^*/
    UInt8  ulUePriorityWt; /*^ M, 0 ^*/
    UInt8  ulUeDelayWt; /*^ M, 0 ^*/
    UInt8  ulUeBsrLoadWt; /*^ M, 0 ^*/
    UInt8  ulLcgPriorityWt; /*^ M, 0 ^*/
    UInt8  logLevel; /*^ M, 0 ^*/
    UInt8  dlSchedulerStrategy; /*^ M, 0 ^*/
    UInt8  ulSchedulerStrategy; /*^ M, 0 ^*/
    UInt8  maxUEsToBeScheduledDL; /*^ M, 0 ^*/
    UInt8  maxUEsToBeScheduledUL; /*^ M, 0 ^*/
    UInt8  ulConfigAdvanceSubFrame; /*^ M, 0 ^*/
    UInt8  CQIThreshold; /*^ M, 0 ^*/
    /*4x4 DL MIMO CHG START*/
    UInt8  CQIThresholdLayer3; /*^ M, 0 ^*/
    UInt8  CQIThresholdLayer4; /*^ M, 0 ^*/
    UInt8  CQIThresholdCellEdgeLayer3; /*^ M, 0 ^*/
    UInt8  CQIThresholdCellEdgeLayer4; /*^ M, 0 ^*/
    /*4x4 DL MIMO CHG END*/
    /* SPR 1571 changes start */
    UInt8  siSchedulingRATPolicy; /*^ M, 0 ^*/
    /* SPR 1571 changes end */
    /* qos changes start */
    /* Qos Related Params */
    /* Indicates the factor for calculating the bucket depth for GBR LC's */
    UInt8 gbrBktDepthFactor; /*^ M, 0 ^*/
    /* Indicates the factor for calculating the bucket depth for AMBR */
    UInt8 ambrBktDepthFactor; /*^ M, 0 ^*/
    /* Indicates the Excess RB's to condsidered for QOS strategy */
    UInt8  qosStrategyOverallocFactor; /*^ M, 0 ^*/
    /* Stores the boolean value for enabling or disabling retraversal  */
    UInt8 enableAllocAfterAllowedBitrate; /*^ M, 0 ^*/
    UInt8 dlRBRestrictionFactor; /*^ M, 0 ^*/
    UInt8 ulRBRestrictionFactor; /*^ M, 0 ^*/
    /* DESIGN REVIEW CHANGES INCORPORATION */
    /* Indicates the factor for limiting the token down fall */
    /* SPR 12975 Changes Start */
    SInt8 dlAmbrTokenLimitFactor; /*^ M, 0 ^*/
    SInt8 ulAmbrTokenLimitFactor; /*^ M, 0 ^*/
    SInt8 dlGbrTokenLimitFactor; /*^ M, 0 ^*/
    SInt8 ulGbrTokenLimitFactor; /*^ M, 0 ^*/
    /* SPR 12975 Changes End */
    /* qos changes stop */
   /* SPR 3306 changes start */
    UInt8 commChWithMIBAllowed; /*^ M, 0 ^*/
    UInt8 dlSchWithMIBAllowed; /*^ M, 0 ^*/
    /* SPR 3306 changes end */    
    /*SPR 3600 Start*/
    /*Maximum value of MCS index when 64QAM is not supported*/
    UInt8 maxMcs16QAM; /*^ M, 0 ^*/
    /*SPR 3600 End*/

    /* ICIC changes start */
    UInt8 maxCellEdgeUEsToBeScheduledDL; /*^ M, 0 ^*/
    UInt8 maxCellEdgeUEsToBeScheduledUL; /*^ M, 0 ^*/
    UInt8 cellEdgeUECQIThreshold; /*^ M, 0 ^*/
    /* ICIC changes end */
    UInt8 avgNumAmLcPerUE; /*^ M, 0 ^*/
    UInt8 avgNumUmLcPerUE; /*^ M, 0 ^*/
    UInt8 preferredPrimaryDCIFormatForSingleTB[MAX_TRANSMISSION_MODES]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* Health monitoring time Interval for Mac */
    UInt16 healthMonitoringTimeInterval; /*^ M, 0 ^*/
    /*Indicates whether CA support is ON or not*/
    UInt8 isCASupported; /*^ M, 0 ^*/
    /*Indicates whether LAA eligible is ON or not*/
    UInt8 isLAASupported; /*^ M, 0 ^*/

    /* Stores the weights for different DL strategy foctors  */
    QosStrategyWeights dlQosStrategyWeights; /*^ M, 0 ^*/
    /* Stores the weights for different DL strategy foctors  */
    QosStrategyWeights ulQosStrategyWeights; /*^ M, 0 ^*/

    /* Contains system wide parameters */
    SpsSysWideParam spsSysWideParams; /*^ M, 0 ^*/
    /* SPS DL profile per Interval */
    SpsDLIntervalProfile spsDLIntervalProfile[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* SPS UL profile per Interval */
    SpsULIntervalProfile spsULIntervalProfile[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* SPS common profile per Interval */
    SpsCommIntervalProfileIntf spsCommIntervalProfile[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* Indicates the QCI details */
    QosQciInfo qosQciInfo[MAX_QCI]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* Spr 23393 Changes Start */
    /* Num Rar message Per TTI to be sent will be provided by OAM /
     * The Value for this parameter is between 1 to 4  */
    UInt8 maxRarPduPerTTI;
    /* Spr 23393 Changes End */
#ifdef LTE_EMBMS_SUPPORTED
    /*max_sync_period */
    /*value range 0 to 10*/
    UInt32 embmsSyncSeqDuration; /*^ M, 0 ^*/
    /*length of sync_seq*/
    /*value range 0 to 59999*/
    UInt16 embmsMaxSyncPeriod; /*^ M, 0 ^*/
    /*N value before which trigger need to be sent */
    UInt16 embmsSyncMarginTimer; /*^ M, 0 ^*/
#endif
/* SPR 23402 Changes Start */
/* MOCN CHANGES START */
    MocnParams mocnParams; /*^ M, 0 ^*/
/* MOCN CHANGES END */
/* SPR 23402 Changes End */
    
} MacInitReqParams; /*^ API, MAC_INIT_LAYER_REQ ^*/

/* CA Changes start */
typedef struct MacInitDLPFSParamsT
{
    UInt8  dlUeQueueLoadWt; /*^ M, 0 ^*/
    UInt8  dlLcQueueLoadWt; /*^ M, 0 ^*/
    UInt8  dlMsWt; /*^ M, 0 ^*/
    UInt8  dlMs64QAMFactorWt; /*^ M, 0 ^*/
    UInt8  dlMs16QAMFactorWt; /*^ M, 0 ^*/
    UInt8  dlMsQPSKFactorWt; /*^ M, 0 ^*/
    UInt8  dlUePriorityWt; /*^ M, 0 ^*/
    UInt8  dlLcPriorityWt; /*^ M, 0 ^*/
    UInt8  dlUeDelayWt; /*^ M, 0 ^*/
    UInt8  dlLcDelayWt; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8  pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}MacInitDLPFSParams;

typedef struct MacInitULPFSParamsT
{
    UInt8  ulMsWt; /*^ M, 0 ^*/
    UInt8  ulMs64QAMFactorWt; /*^ M, 0 ^*/
    UInt8  ulMs16QAMFactorWt; /*^ M, 0 ^*/
    UInt8  ulMsQPSKFactorWt; /*^ M, 0 ^*/
    UInt8  ulUePriorityWt; /*^ M, 0 ^*/
    UInt8  ulUeDelayWt; /*^ M, 0 ^*/
    UInt8  ulUeBsrLoadWt; /*^ M, 0 ^*/
    UInt8  ulLcgPriorityWt; /*^ M, 0 ^*/
}MacInitULPFSParams;

typedef struct MacInitDLQOSParamsT
{
    /* Indicates Token update periodicity */
    UInt16 tokenPeriodicity; /*^ M, 0 ^*/
    /* Indicates the factor for calculating the bucket depth for GBR LC's */
    UInt8 gbrBktDepthFactor; /*^ M, 0 ^*/
    /* Indicates the factor for calculating the bucket depth for AMBR */
    UInt8 ambrBktDepthFactor; /*^ M, 0 ^*/
    /* Stores the weights for different DL strategy foctors  */
    QosStrategyWeights dlQosStrategyWeights; /*^ M, 0 ^*/
    /* Indicates the Excess RB's to condsidered for QOS strategy */
    UInt8  qosStrategyOverallocFactor; /*^ M, 0 ^*/
    /* Stores the boolean value for enabling or disabling retraversal  */
    UInt8 enableAllocAfterAllowedBitrate; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8 pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}MacInitDLQOSParams;

typedef struct MacInitULQOSParamsT
{
    /* Stores the weights for different UL strategy foctors  */
    QosStrategyWeights ulQosStrategyWeights; /*^ M, 0 ^*/
}MacInitULQOSParams;

typedef struct MacInitICICParamsT
{
    UInt8 maxCellEdgeUEsToBeScheduledDL; /*^ M, 0 ^*/
    UInt8 maxCellEdgeUEsToBeScheduledUL; /*^ M, 0 ^*/
    UInt8 cellEdgeUECQIThreshold; /*^ M, 0 ^*/
    UInt8 CQIThresholdCellEdgeLayer3; /*^ M, 0 ^*/
    UInt8 CQIThresholdCellEdgeLayer4; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8 pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}MacInitICICParams;


typedef struct MacInitSPSParamsT
{
    /* Contains system wide parameters */
    SpsSysWideParam spsSysWideParams; /*^ M, 0 ^*/
    /* SPS DL profile per Interval */
    SpsDLIntervalProfile spsDLIntervalProfile[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* SPS UL profile per Interval */
    SpsULIntervalProfile spsULIntervalProfile[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* SPS common profile per Interval */
    SpsCommIntervalProfileIntf spsCommIntervalProfile[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
} MacInitSPSParams;

typedef struct MacInitCellReqParamsT 
{
    UInt32  fieldBitMask; /*^ M, 0 ^*/
    UInt16 numOfUE; /*^ M, 0 ^*/
    UInt16 rntiStartRange; /*^ M, 0 ^*/
    UInt16 rntiEndRange; /*^ M, 0 ^*/
    UInt8  maxUEsToBeScheduledDL; /*^ M, 0 ^*/
    UInt8  maxUEsToBeScheduledUL; /*^ M, 0 ^*/
    UInt8  CQIThreshold; /*^ M, 0 ^*/
    UInt8 CQIThresholdLayer3; /*^ M, 0 ^*/
    UInt8 CQIThresholdLayer4; /*^ M, 0 ^*/
    UInt8 dlRBRestrictionFactor; /*^ M, 0 ^*/
    UInt8 ulRBRestrictionFactor; /*^ M, 0 ^*/
    /* Indicates the factor for limiting the token down fall */
    /* SPR 12975 Changes Start */
    SInt8 dlAmbrTokenLimitFactor; /*^ M, 0 ^*/
    SInt8 ulAmbrTokenLimitFactor; /*^ M, 0 ^*/
    SInt8 dlGbrTokenLimitFactor; /*^ M, 0 ^*/
    SInt8 ulGbrTokenLimitFactor; /*^ M, 0 ^*/
    /* SPR 12975 Changes End */
    UInt8  commChWithMIBAllowed; /*^ M, 0 ^*/
    UInt8  dlSchWithMIBAllowed; /*^ M, 0 ^*/
    /*Maximum value of MCS index when 64QAM is not supported*/
    UInt8 maxMcs16QAM; /*^ M, 0 ^*/
    UInt8 isLastAttempt; /*^ M, 0 ^*/
    UInt8  dlSchedulerStrategy; /*^ M, 0 ^*/
    UInt8  ulSchedulerStrategy; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8 pading; /*^ M, 0 ^*/ 
    MacInitDLPFSParams macInitDLPFSParams; /*^ M, 0 ^*/
    MacInitULPFSParams macInitULPFSParams; /*^ M, 0 ^*/
    MacInitDLQOSParams macInitDLQOSParams; /*^ M, 0 ^*/
    MacInitULQOSParams macInitULQOSParams; /*^ M, 0 ^*/
    MacInitICICParams macInitICICParams; /*^ M, 0 ^*/
    MacInitSPSParams macInitSPSParams; /*^ M, 0 ^*/
    /* SPR 23504/SES-341 Fix Start */
    MocnParams mocnParams; /*^ M, 0 ^*/
    /* SPR 23504/SES-341 Fix End */
} MacInitCellReqParams; /*^ API, MAC_INIT_CELL_REQ ^*/
/* CA Changes end */


/* Structure definition for API :MAC_INIT_LAYER_CNF
 * Structure name               :MacInitRespParams */
typedef struct MacInitRespParamsT {
    UInt16 responseCode; /*^ M, 0 ^*/
#ifdef LTE_EMBMS_SUPPORTED
    UInt16 phyMbsfnCapability; /*^ M, 0 ^*/ 
#endif
} MacInitRespParams; /*^ API, MAC_INIT_LAYER_CNF ^*/ 

/* QOS_OAM_MODIFY_CHANGES_start */
/* Structure definition for API : MAC_MODIFY_LAYER_REQ
 * Structure name               :MacModifyQosPamams */
typedef struct MacModifyQosPamamsT {
    UInt16  modBitMap;       /*^ M, 0 ^*/     // bit map
    UInt8  gbrBktDepthFactor;  /*^ M, 0 ^*/   // GBR bucket depth factor
    UInt8  ambrBktDepthFactor; /*^ M, 0 ^*/    // AMBR bucket depth factor
    UInt16 tokenPeriodicity; /*^ M, 0 ^*/ //token periodicity
    UInt8  qosStrategyOverallocFactor; /*^ M, 0 ^*/ //over allocation factor
    UInt8 enableAllocAfterAllowedBitrate; /*^ M, 0 ^*/ //TRUE/FALSE
    QosStrategyWeights dlQosStrategyWeights; /*^ M, 0 ^*/
    QosStrategyWeights ulQosStrategyWeights; /*^ M, 0 ^*/
} MacModifyQosPamams; /*^ API, MAC_MODIFY_LAYER_REQ ^*/

/* Structure definition for API :GET_MAC_KPI_CNF
 * Structure name               :MacKpiResp */
typedef struct MacKpiRespT {
    UInt16 responseCode; /*^ M, 0 ^*/
    UInt16 numOfActiveUE; /*^ M, 0 ^*/
    UInt32 numOfRachInd; /*^ M, 0 ^*/
} MacKpiResp; /*^ API, MAC_GET_KPI_CNF ^*/ 

/* Structure definition for API :MAC_SET_LOG_LEVEL_REQ
 * Structure name               :MacLogLevel_Req */
typedef struct MacLogLevel_ReqT {
    UInt32 logLevel; /*^ M, 0 ^*/
} MacLogLevel_Req; /*^ API, MAC_SET_LOG_LEVEL_REQ ^*/

/* Structure definition for API :MAC_SET_LOG_LEVEL_CNF
 * Structure name               :MacSetLoglevelResp */
typedef struct MacSetLoglevelRespT {
    UInt16 responseCode; /*^ M, 0 ^*/
} MacSetLoglevelResp; /*^ API, MAC_SET_LOG_LEVEL_CNF ^*/

/* Structure definition for API :MAC_GET_LOG_LEVEL_RESP
 * Structure name              :MacGetLogLevelResp */
typedef struct MacGetLogLevelRespT {
    UInt32 logLevel; /*^ M, 0 ^*/
} MacGetLogLevelResp; /*^ API, MAC_GET_LOG_LEVEL_RESP ^*/

/* Structure definition for API :MAC_ENABLE_LOG_CATEGORY_REQ
 * Structure name               :MacEnableLogCategoryReq */
typedef struct MacEnableLogCategoryReqT {
    UInt64 logCategory; /*^ M, 0 ^*/
} MacEnableLogCategoryReq; /*^ API, MAC_ENABLE_LOG_CATEGORY_REQ ^*/

/* Structure defintion for API  :MAC_DISABLE_LOG_CATEGORY_REQ
 * Structure name               :MacDisableLogCategoryReq */
typedef struct MacDisableLogCategoryReqT {
    UInt64 logCategory; /*^ M, 0 ^*/
} MacDisableLogCategoryReq; /*^ API, MAC_DISABLE_LOG_CATEGORY_REQ ^*/

/* Structure definition for API :MAC_GET_LOG_CATEGORY_RESP
 * Structure name               :MacGetLogCategoryResp */
typedef struct MacGetLogCategoryRespT {
    UInt64 logCategory; /*^ M, 0 ^*/
} MacGetLogCategoryResp; /*^ API, MAC_GET_LOG_CATEGORY_RESP ^*/

/*CLPC_CHG*/
typedef struct MacUeSinrTaReqT
{
    /*ueIndex is the index of the UE for which SINR and TA is requested*/
    UInt16 ueIndex; /*^ M, 0 ^*/
}MacUeSinrTaReq; /*^ API, MAC_UE_SINR_TA_REQ, RRM_MAC_UE_SINR_TA_REQ ^*/

typedef struct MacUeSinrTaRespT
{
    /* SPR 4501 fix */
    UInt16 responseCode; /*^ M, 0 ^*/
    /* SPR 4501 fix */
    /*ueIndex is the index of the UE for which SINR and TA is requested*/
    UInt16 ueIndex; /*^ M, 0 ^*/
    /*timingAdvanceValue is the time advance value requested
     ** (Cumulative value in range of 0.1282)*/
    UInt16 timingAdvanceValue; /*^ M, 0 ^*/
    UInt16 uplinkPathloss; /*^ M, 0 ^*/

    /* SPR 3100 START*/
    /*averageULSINR is the average UL SINR value requested. (Value: 0-255,
     ** representing -64dB to 63.5dB, with 0.5dB step size
     ** (As per FAPI Interface))*/
    UInt8 averageULSINR; /*^ M, 0 ^*/
    /*averageDL SNR is the average DL SNR value requested. (Value: 0-255,
     * representing -64dB to 63.5dB, with 0.5dB step size
     *(As per FAPI Interface))*/
    UInt8 averageDLSINRCW0; /*^ M, 0 ^*/
    UInt8 averageDLSINRCW1; /*^ M, 0 ^*/
    /* SPR 3100 END*/

    UInt8 uplinkBler; /*^ M, 0 ^*/
    UInt8 downlinkBlerCW0; /*^ M, 0 ^*/
    UInt8 downlinkBlerCW1; /*^ M, 0 ^*/
    /* SPR 5033 changes start */
    UInt8 averagePucchSINR; /*^ M, 0 ^*/
    SInt8 pucchBler; /*^ M, 0 ^*/
    /* SPR 5033 changes end */
}MacUeSinrTaResp; /*^ API, MAC_UE_SINR_TA_RESP, RRM_MAC_UE_SINR_TA_RESP ^*/

/* structure definiton for DL_LOG_CH_STATUS */
typedef struct dlLogChannelStatusReqT {
    UInt8   lcId; /*^ M, 0 ^*/
    UInt8   lcPriority; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8   pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt32  queueLoad; /*^ M, 0 ^*/
} dlLogChannelStatusReq;

/* structure definition for UL_LOG_CH_STATUS */
typedef struct ulLogChannelStatusReqT {
    UInt8   lcId; /*^ M, 0 ^*/
    UInt8   lcgId; /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8   pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt32  bsrNetOfLCG; /*^ M, 0 ^*/
} ulLogChannelStatusReq;

/* structure definiton for Statistics of each UE */
typedef struct MacUEStatusReqT {
    UInt8  isUeIdPresent; /*^ M, 0 ^*/
    UInt16 ueId; /*^ M, 0 ^*/
}MacUEStatusReq;

/* Structure definition for API :MAC_GET_STATUS_CNF
 * Structure name               :MacUEStatusResp */
typedef struct MacPerUEStatusRespT {
    UInt16  rnti; /*^ M, 0 ^*/
    UInt8   uePriority; /*^ M, 0 ^*/
    UInt8   numTxAntenna; /*^ M, 0 ^*/
    UInt8   dlTxMode;  /*^ M, 0 ^*/
    UInt8   dlNumOfLayer; /*^ M, 0 ^*/
    UInt8   dlNumOfLCs; /*^ M, 0 ^*/
    UInt8   dlMaxRB; /*^ M, 0 ^*/
    UInt8   dlModSchemeCodeWord0; /*^ M, 0 ^*/
    UInt8   dlModSchemeCodeWord1; /*^ M, 0 ^*/
    /* SPR 2063 start */
    /* SPR 2063 end */
    UInt8   dlMcsIndexCodeWord0; /*^ M, 0 ^*/
    UInt8   dlMcsIndexCodeWord1; /*^ M, 0 ^*/
    UInt32  dlQueueLoad; /*^ M, 0 ^*/
    UInt8   dlCodeBookIndex; /*^ M, 0 ^*/
    UInt8   cqiIndicator; /*^ M, 0 ^*/
    UInt8   simultaneousAckNackAndCQI; /*^ M, 0 ^*/
    UInt8   ulNumOfLCs; /*^ M, 0 ^*/
    UInt16  bsrNet0; /*^ M, 0 ^*/
    UInt16  bsrNet1; /*^ M, 0 ^*/
    UInt16  bsrNet2; /*^ M, 0 ^*/
    UInt16  bsrNet3; /*^ M, 0 ^*/
    /* SPR 2063 start */
    /* SPR 2063 end */
    UInt16  ulMcsIndex; /*^ M, 0 ^*/
    UInt16 tpcPUCCHRNTI; /*^ M, 0 ^*/
    UInt16 tpcPUSCHRNTI; /*^ M, 0 ^*/
    SInt8 absolutePowerCorrectionPUSCH; /*^ M, 0 ^*/
    SInt8 absolutePowerCorrectionPUCCH; /*^ M, 0 ^*/
    dlLogChannelStatusReq    macUeDLLogChStatus[MAX_NUMBER_OF_LOGICAL_CHANNEL]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    ulLogChannelStatusReq    macUeULLogChStatus[MAX_NUMBER_OF_LOGICAL_CHANNEL]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt8   ulModScheme; /*^ M, 0 ^*/
}MacPerUEStatusResp; /*^ API, MAC_GET_STATUS_CNF ^*/


typedef struct MacUEStatusRespT {
    UInt16 responseCode; /*^ M, 0 ^*/
    UInt16  numOfUe; /*^ M, 0 ^*/
    MacPerUEStatusResp macUEStatusInfo[MAC_OAM_VAR_SIZE(numOfUe)]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}MacUEStatusResp;


/* Structure definition for API :MAC_GET_STATS_CNF
 * Structure name               :MacStatsResp */
typedef struct MacStatsRespT {
    UInt16    responseCode; /*^ M, 0 ^*/
    UDouble32 dlThroughPut; /*^ M, 0 ^*/
    UDouble32 ulThroughPut; /*^ M, 0 ^*/
    UDouble32 dlSpectralEfficiency; /*^ M, 0 ^*/
    UDouble32 ulSpectralEfficiency; /*^ M, 0 ^*/
}MacStatsResp; /*^ API, MAC_GET_STATS_CNF ^*/

/* Structure definition for API :MAC_RESET_STATS_CNF
 * Structure name               :MacResetStatsRespT */
typedef struct MacResetStatsRespT {
    UInt16 responseCode; /*^ M, 0 ^*/
} MacResetStatsResp; /*^ API, MAC_RESET_STATS_CNF ^*/

/* Structure definition for API : MAC_CONFIGURE_KPI_STATS_REQ
 * Structure name               :MacConfigureKpiStatsReqParams */
typedef struct MacConfigureKpiStatsReqParamsT {
    UInt16 durationT;   /*^ M, 0 ^*/           // KPI timer value
    UInt16  kpiToReportBitMap; /*^ M, 0 ^*/     // Each bit of this bitmap represents KPI
    UInt8  periodicReporting;  /*^ M, 0 ^*/    // Indicates periodic reporting required or not
} MacConfigureKpiStatsReqParams; /*^ API, MAC_CONFIGURE_KPI_STATS_REQ, RRM_MAC_CONFIGURE_KPI_STATS_REQ ^*/

/* Structure definition for API : MAC_CONFIGURE_KPI_STATS_CNF
 * Structure name               :MacConfigureKpiStatsResp */
typedef struct MacConfigureKpiStatsRespT {
/* SPR 9068 Fix Start */
    UInt16 responseCode;  /*^ M, 0 ^*/           // MAC_SUCCESS or an Error Code
/* SPR 9068 Fix End */
} MacConfigureKpiStatsResp; /*^ API, MAC_CONFIGURE_KPI_STATS_CNF ^*/

/* SPR 23402 Changes Start */
/* MOCN Changes Start */
/* Structure to send per OPERATOR KPIs */
typedef struct MacPerOperatorKpiStatsIndParamsT {
    UInt8 operatorId;
    UInt64 totalPRBUsagePerQciDL[MAX_QCI];
    UInt64 totalPRBUsagePerQciUL[MAX_QCI];
    UInt64 numberOfActiveUEPerQciDL[MAX_QCI];
    UInt64 numberOfActiveUEPerQciUL[MAX_QCI];
} MacPerOperatorKpiStatsIndParams;
/* MOCN Changes End */
/* SPR 23402 Changes End */

/* Structure definition for API : MAC_KPI_STATS_IND
 * Structure name               :MacKpiStatsIndParams */
typedef struct MacKpiStatsIndParamsT {
    UInt64 totalPRBUsageDL; /*^ M, 0 ^*/
    UInt64 totalPRBUsageUL; /*^ M, 0 ^*/
    UInt64 totalPRBUsagePerQciDL[MAX_QCI]; /*^ M, 0, OCTET_STRING, FIXED ^*/ 
    UInt64 totalPRBUsagePerQciUL[MAX_QCI]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt64 numberOfActiveUEPerQciDL[MAX_QCI]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt64 numberOfActiveUEPerQciUL[MAX_QCI]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt64 totalPktDelayPerQciDL[MAX_QCI]; /*^ M, 0, OCTET_STRING, FIXED ^*/
/*KPI_CHANGES*/
    UInt64 rcvdRandomAccessPreamblesContentionFree; /*^ M, 0 ^*/
    UInt64 rcvdRandomAccessPreamblesGroupA; /*^ M, 0 ^*/
    UInt64 rcvdRandomAccessPreamblesGroupB; /*^ M, 0 ^*/
/*KPI_CHANGES*/
    UInt64 totalPktAckedInDLPerQci[MAX_QCI];
    /* SPR 13492 Changes Start */
    UInt64 totalSessionTimeUE; /*^ M, 0 ^*/
    UInt64 totalSessionTimePerQci[MAX_QCI]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* SPR 13492 Changes End */
    UInt32 DurationTTI; /*^ M, 0 ^*/
/* KPI changes start */
    UInt32 totalCQICW1Reported[MAX_CQI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt32 totalCQICW2Reported[MAX_CQI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /*EICIC changes starts*/
    UInt32 AbsSubframeCount; /*^ M, 0 ^*/
    /*EICIC Changes Ends*/
/*spr 19296 fix start*/
   UInt32 totalDlDrbUuLossRatePerQci[MAX_QCI];
   UInt32 totalSduAckRcvdDlperQci[MAX_QCI];
/*spr 19296 fix end*/

/* KPI changes end */
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
    UInt8 numOfOperators; /*^ M, 0 ^*/
    MacPerOperatorKpiStatsIndParams kpiPerOperator[MAX_NUM_MOCN_OPERATOR]; /*^ M, 0, OCTET_STRING, FIXED ^*/
/* MOCN Changes End */
/* SPR 23402 Changes End */

} MacKpiStatsIndParams; /*^ API, MAC_KPI_STATS_IND, RRM_MAC_KPI_STATS_IND, MAC_GET_KPI_STATS_CNF, RRM_MAC_GET_KPI_STATS_CNF ^*/

/*spr 19296 fix start*/
#define MAX_KPI_INDEX 80
/*spr 19296 fix end*/

/* Structure definition for API : MAC_KPI_THP_STATS_IND
 * Structure name               :MacKpiThpStatsIndParams */
typedef struct MacKpiThpStatsIndParamsT {
    UInt32 avgDurationT; /*^ M, 0 ^*/
    /*spr 19296 fix start*/
    UInt16 ueIndex[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt8  qci[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt64 thpVolDl[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt64 thpVolUl[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt32 thpTimeDl[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt32 thpTimeUl[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt32 latTime[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt32 latSample[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt8  endIndicator;
    /*spr 19296 fix end*/
} MacKpiThpStatsIndParams; /*^ API, MAC_KPI_THP_STATS_IND, RRM_MAC_KPI_THP_STATS_IND, MAC_GET_KPI_THP_STATS_CNF, RRM_MAC_GET_KPI_THP_STATS_CNF ^*/

/* Structure definition for API : MAC_GET_KPI_STATS_REQ
 * Structure name               :MacGetKpiStatsReqParams */
typedef struct MacGetKpiStatsReqParamsT {
    UInt16  kpiToReportBitMap; /*^ M, 0 ^*/     // Each bit of this bitmap represents KPI
    UInt8  resetStats; /*^ M, 0 ^*/           // 0 (false) , 1 (true)
} MacGetKpiStatsReqParams; /*^ API, MAC_GET_KPI_STATS_REQ, RRM_MAC_GET_KPI_STATS_REQ ^*/


/* QOS_OAM_MODIFY_CHANGES_start */
typedef struct MacQoSModifyparamsRespT {
        UInt16 responseCode; /*^ M, 0 ^*/         // MAC_SUCCESS or an Error Code
}MacQoSModifyparamsResp;
/* QOS_OAM_MODIFY_CHANGES_end */

/* Structure definition for API : MAC_GET_KPI_STATS_CNF
 * Structure name               :MacGetKpiStatsResp */
typedef MacKpiStatsIndParams MacGetKpiStatsResp;

/* Structure definition for API : MAC_GET_KPI_THP_STATS_CNF
 * Structure name               :MacGetKpiThpStatsResp */
typedef MacKpiThpStatsIndParams MacGetKpiThpStatsResp;

/* Structure definition for API : MAC_PHY_OAM_ERROR_IND
 * Structure name               : MacPhyOamErrorInd */
typedef struct MacPhyOamErrorIndT
{
    /* This field indicates the type of error that is received from OAM */
    UInt32 typeOfError; /*^ M, 0 ^*/
} *LP_MacPhyOamErrorInd, MacPhyOamErrorInd; /*^ API, MAC_PHY_OAM_ERROR_IND ^*/
/*spr 22474 start*/
/* MAC_OAM_GET_DEBUG_INFO_RESP */
typedef struct MacGetDebugInfoReqParamsT {
    /* This field informs the type of debugging information needed */
    UInt16 debugType; /*^ M, 0 ^*/ /* Check Enum GetDebugType */
}MacGetDebugInfoReqParams;

/* MAC_OAM_GET_DEBUG_INFO_RESP */
typedef struct MacGetDebugInfoRespParamsT {
    UInt16 responseCode; /*^ M, 0 ^*/    // MAC_SUCCESS or an Error Code
    /* This field informs the type of debugging information sent */
    UInt16 debugType; /*^ M, 0 ^*/ /* Check Enum GetDebugType */
    union {
         L2MemPoolStats csplMemPoolStats;
         L2MsgPoolStats csplMsgPoolStats;
    }u;
}MacGetDebugInfoRespParams;

/*spr 22474 end*/
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
typedef struct mocnOperatorParamsT
{
    UInt8 isMocnEnabled;
    UInt8 numOfOperator;
    UInt8 prbUsageMeasForgettingFactor; //Forgetting factor in percentage
    UInt16 prbUsageMeasResetPeriod;     // millisecond
    UInt8 dedicatedDLPRBsPerOperator[MAX_NUM_MOCN_OPERATOR]; //percentage of total RBs
    UInt8 dedicatedULPRBsPerOperator[MAX_NUM_MOCN_OPERATOR]; // percentage of total RBs
    UInt8 reservedDLPRBsPerOperator[MAX_NUM_MOCN_OPERATOR]; //percentage of total RBs
    UInt8 reservedULPRBsPerOperator[MAX_NUM_MOCN_OPERATOR]; //percentage of total RBs
    UInt8 totalDedicatedDLPRBs; // sum of all dedicated DL PRBs
    UInt8 totalDedicatedULPRBs; // sum of all dedicated UL PRBs
    UInt8 totalReservedDLPRBs;  // sum of all reserved DL PRBs
    UInt8 totalReservedULPRBs;  // sum of all reserved UL PRBs
    UInt8 dlPrbUsageHist[MAX_NUM_MOCN_OPERATOR];
    UInt8 ulPrbUsageHist[MAX_NUM_MOCN_OPERATOR];
    UInt16 dlPrbUsageDRBPriorityFactor[MAX_NUM_MOCN_OPERATOR];
    UInt16 ulPrbUsageDRBPriorityFactor[MAX_NUM_MOCN_OPERATOR];
    UInt16 prbUsageSRBPriorityFactor;
} mocnOperatorParams;
/* MOCN Changes End */
/* SPR 23402 Changes End */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif  /* LTE_MAC_OAM_INTERFACE_H */
