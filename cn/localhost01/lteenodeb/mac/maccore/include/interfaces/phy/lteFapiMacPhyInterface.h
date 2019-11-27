/******************************************************************************
*
*  ARICENT -
*
*  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
*
******************************************************************************
*
*  $Id: lteFapiMacPhyInterface.h,v 1.1.4.1.14.1 2010/11/08 06:06:14 gur10121 Exp $
*
******************************************************************************
*
*  File Description : This file contains the structure, MACROS definition
*                     derived from Femto Forum API Document ver 1.0.
*                     (FF_LTE_L1_API_Specification_1_0.doc) 
*
******************************************************************************
*
* Revision Details
* ----------------
* $Log: lteFapiMacPhyInterface.h,v $
* Revision 1.1.4.1.14.1  2010/11/08 06:06:14  gur10121
* Semi static changes merging
*
* Revision 1.1.4.1.8.1  2010/10/26 10:49:51  gur10121
* Semi static Compilation fix
*
* Revision 1.1.4.1.6.3  2010/10/25 08:15:37  gur10121
* *** empty log message ***
*
* Revision 1.1.4.1.6.2  2010/10/21 11:53:46  gur10121
* Padding added
*
* Revision 1.1.4.1.6.1  2010/10/21 07:54:54  gur10121
* Semi static UT fixes
*
* Revision 1.1.4.1  2010/09/21 15:45:51  gur20491
* FAPI changes
*
* Revision 1.1.2.1  2010/09/07 11:21:07  gur20439
* new files added for FAPI changes
*
* Revision 1.1.2.4  2010/08/31 11:16:26  gur10694
* Removing 
 chars
*
* Revision 1.1.2.3  2010/08/20 14:23:29  gur04640
* *** empty log message ***
*
* Revision 1.1.2.2  2010/08/17 06:22:25  gur04640
* *** empty log message ***
*1,v $
*****************************************************************************/
#ifndef LTE_FAPI_MAC_PHY_INTERFACE_H
#define LTE_FAPI_MAC_PHY_INTERFACE_H

/* Set alignment to 32 bit boundary for this file */
#if  defined(FLEXRAN)
/*SPR 21001 Changes start */
#pragma pack(1)
#else
#pragma pack(4)
/*SPR 21001 Changes end */
#endif
/* + TM7_8_changes Start*/
#include "lteMacTypes.h" 
/* - TM7_8_changes End*/
#if  defined(FLEXRAN)
#define STRUC_ONE_BYTE_PAD1   
#define STRUC_ONE_BYTE_PAD2   
#define STRUC_TWO_BYTE_PAD1   
#define STRUC_TWO_BYTE_PAD2   
#define STRUC_THREE_BYTE_PAD1 
#define STRUC_THREE_BYTE_PAD2 
#else
#define STRUC_ONE_BYTE_PAD1   uint8_t padding1;
#define STRUC_ONE_BYTE_PAD2   uint8_t padding2;
#define STRUC_TWO_BYTE_PAD1   uint8_t padding1[2];
#define STRUC_TWO_BYTE_PAD2   uint8_t padding2[2];
#define STRUC_TWO_BYTE_PAD3   uint8_t padding3[2];
#define STRUC_THREE_BYTE_PAD1 uint8_t padding1[3];
#define STRUC_THREE_BYTE_PAD2 uint8_t padding2[3];
#endif
/*Data Type changed during the Qualcomm Integration
   QC tool-chain already have the same name of the data 
   type but its size is different*/
typedef unsigned int    uint32_T;
/* +DYNAMIC_ICIC */
typedef signed int      int32_T;
/* -DYNAMIC_ICIC */
typedef unsigned short  uint16_t;
typedef unsigned char   uint8_t;


/* + TM7_8_changes Start*/
typedef signed char      int8_t;
/* - TM7_8_changes End*/

/**
 * FAPI_msgIdType_t definition
 */
typedef uint8_t FAPI_msgIdType_t;
/**
 * Possible hashdefs of FAPI_msgIdType_t
 */
#define PHY_PARAM_REQUEST               0x00
#define PHY_PARAM_RESPONSE              0x01
#define PHY_CELL_CONFIG_REQUEST         0x02
#define PHY_CELL_CONFIG_RESPONSE        0x03
#define PHY_START_REQUEST               0x04
#define PHY_STOP_REQUEST                0x05
#define PHY_STOP_INDICATION             0x06
#define PHY_UE_CONFIG_REQUEST           0x07
#define PHY_UE_CONFIG_RESPONSE          0x08
#define PHY_ERROR_INDICATION            0x09
#define PHY_UE_RELEASE_REQ              0x0a
#define PHY_UE_RELEASE_RESP              0x0b


#define PHY_DL_CONFIG_REQUEST          0x80
#define PHY_UL_CONFIG_REQUEST          0x81
#define PHY_UL_SUBFRAME_INDICATION     0x82
#define PHY_DL_HI_DCI0_REQUEST         0x83
#define PHY_DL_TX_REQUEST              0x84
#define PHY_UL_HARQ_INDICATION         0x85
#define PHY_UL_CRC_INDICATION          0x86
#define PHY_UL_RX_ULSCH_INDICATION     0x87
#define PHY_UL_RACH_INDICATION         0x88
#define PHY_UL_SRS_INDICATION          0x89
#define PHY_UL_RX_SR_INDICATION        0x8a
#define PHY_UL_RX_CQI_INDICATION       0x8b
/* +DYNAMIC_ICIC */
#define PHY_MEASUREMENT_REPORT_INDICATION 0x96
/* -DYNAMIC_ICIC */

/* This macro is used for declaring array of variable length */
#define FAPI_VAR_SIZE(x) 1


/**
* Data : MACRO
* Name : RECONFIG_TLV_NUM
* Description : This MACRO describes the number of TLV changes request during
*               PHY Reconfig currently value is 2, later may be increased as
*               number of runtime reconfig parameters supported increases
**/
#define RECONFIG_TLV_NUM 2
/* SPR 19288 change start */
/*  SPR 8128 change Start */
/* L1 Error Indication */
#define      FAPI_MSG_OK                                0
#define      FAPI_MSG_INVALID_STATE                     1
#define      FAPI_MSG_INVALID_CONFIG                    2
#define      FAPI_SFN_OUT_OF_SYNC                       3 
#define      FAPI_MSG_SUBFRAME_ERR                      4
#define      FAPI_MSG_BCH_MISSING                       5
#define      FAPI_MSG_INVALID_SFN                       6 
#define      FAPI_MSG_HI_ERR                            7  
#define      FAPI_MSG_TX_ERR                            8
#ifdef FLEXRAN
#define      FAPI_MSG_SYNTAX_ERR			9	
#define      FAPI_MSG_OUT_OF_TIME_ERR			10
#define      FAPI_MSG_IPC_ERR				11
#endif

/*+ SPR 9285*/
typedef      uint8_t  FAPI_l1ErrorCodes_en;
/*- SPR 9285*/
/* SPR 19288 change end */
#if !defined(FAPI_4_0_COMPLIANCE)&&!defined(FLEXRAN)
/* PHY CELL CONFIG Possible Tags */
#define                FAPI_DUPLEXING_MODE               1 
#define                FAPI_PCFICH_POWER_OFFSET          2
#define                FAPI_P_B                          3
#define                FAPI_DL_CYCLIC_PREFIX_TYPE        4
#define                FAPI_UL_CYCLIC_PREFIX_TYPE        5
/* RF Config */
#define                FAPI_DL_CHANNEL_BANDWIDTH         6   
#define                FAPI_UL_CHANNEL_BANDWIDTH         7
#define                FAPI_REFERENCE_SIGNAL_POWER       8
#define                FAPI_TX_ANTENNA_PORTS             9
#define                FAPI_RX_ANTENNA_PORTS             10
/* RF CONFIG TAGS ENDS */
/* PHICH CONFIG */
#define                FAPI_PHICH_RESOURCE               11
#define                FAPI_PHICH_DURATION               12
#define                FAPI_PHICH_POWER_OFFSET           13
/* PHICH CONFIG ENDS */
/* SCH COnfig */
#define                FAPI_PRIMARY_SYNC_SIGNAL          14
#define                FAPI_SECONDARY_SYNC_SIGNAL        15
#define                FAPI_PHYSICAL_CELL_ID             16
/* SCH COnfig  Ends */
/* PRACH Config */
#define             FAPI_CONFIGURATION_INDEX                     17
#define             FAPI_ROOT_SEQUENCE_INDEX                     18
#define             FAPI_ZERO_CORRELATION_ZONE_CONFIGURATION     19
#define             FAPI_HIGH_SPEED_FLAG                         20
 #define             FAPI_FREQUENCY_OFFSET                       21
/* PRACH Config  Ends */
/* PUSCH Config */
 #define             FAPI_HOPPING_MODE                           22
#define             FAPI_HOPPIG_OFFSET                           23
#define             FAPI_NUM_OF_SUB_BANDS                        24
/* PUSCH Config  Ends */
/* PUCCH Config */
#define            FAPI_DELTA_PUCCH_SHIFT                        25
#define            FAPI_N_CQI_RB                                 26
#define            FAPI_N_AN_CS                                  27
#define            FAPI_N_1_PUCCH_AN                             28
/* PUCCH Config Ends */
/* SRS Config */
#define             FAPI_BANDWIDTH_CONFIGURATION                 29
#define             FAPI_MAX_UP_PTS                              30
#define             FAPI_SRS_SUB_FRAME_CONFIGURATION             31
#define             FAPI_SRS_ACK_NACK_SRS_SIMULTANEOUS_TRANSMISSION   32
/* SRS Config Ends */
/* Uplink reference signal Config */
#define             FAPI_UPLINK_RS_HOPPING                       33
#define             FAPI_GROUP_ASSIGNMENT                        34
 #define             FAPI_CYCLIC_SHIFT_1_FOR_DMRS                35
/* Uplink reference signal Config Ends */
/* TDD Frame Structure Config */
#define             FAPI_SUB_FRAME_ASSIGNMENT                    36
#define             FAPI_SPECIAL_SUB_FRAME_PATTERNS              37
              /* + PRS_CHANGES */
#define             FAPI_PRS_BANDWIDTH                           38
#define             FAPI_PRS_CYCLIC_PREFIX                       39
              /* - PRS_CHANGES */
/* TDD Frame Structure Config Ends */
           
/* below tags are used by L1 to reports its physical capabilities to L2/L3 software */
#define             FAPI_DL_BANDWIDTH_SUPPORT                    40 
#define             FAPI_UL_BANDWIDTH_SUPPORT                    41
#define             FAPI_DL_MODULATION_SUPPORT                   42
#define             FAPI_UL_MODULATION_SUPPORT                   43
 #define             FAPI_PHY_ANTENNA_CAPABILITY                 44
/* below tags are used by L2/L3 software to configure the interaction between L2/L3 and L1 */
           
#define             FAPI_DATA_REPORT_MODE                        50 
#define             FAPI_SFN_SF                                  51
           
/* the below tag is used by L1 to report its current status */
#define              FAPI_PHY_STATE                              60

/* --------------------------------------------- */

#else

#define                FAPI_DUPLEXING_MODE               1 
#define                FAPI_PCFICH_POWER_OFFSET          2
#define                FAPI_P_B                          3
#define                FAPI_DL_CYCLIC_PREFIX_TYPE        4
#define                FAPI_UL_CYCLIC_PREFIX_TYPE        5
/* RF Config */
#define                FAPI_DL_CHANNEL_BANDWIDTH         10   
#define                FAPI_UL_CHANNEL_BANDWIDTH         11
#define                FAPI_REFERENCE_SIGNAL_POWER       12
#define                FAPI_TX_ANTENNA_PORTS             13
#define                FAPI_RX_ANTENNA_PORTS             14
/* RF CONFIG TAGS ENDS */
/* PHICH CONFIG */
#define                FAPI_PHICH_RESOURCE               20
#define                FAPI_PHICH_DURATION               21
#define                FAPI_PHICH_POWER_OFFSET           22
/* PHICH CONFIG ENDS */
/* SCH COnfig */
#define                FAPI_PRIMARY_SYNC_SIGNAL          30
#define                FAPI_SECONDARY_SYNC_SIGNAL        31
#define                FAPI_PHYSICAL_CELL_ID             32
/* SCH COnfig  Ends */
/* PRACH Config */
#define             FAPI_CONFIGURATION_INDEX                     40
#define             FAPI_ROOT_SEQUENCE_INDEX                     41
#define             FAPI_ZERO_CORRELATION_ZONE_CONFIGURATION     42
#define             FAPI_HIGH_SPEED_FLAG                         43
 #define             FAPI_FREQUENCY_OFFSET                       44
/* PRACH Config  Ends */
/* PUSCH Config */
 #define             FAPI_HOPPING_MODE                           50
#define             FAPI_HOPPIG_OFFSET                           51
#define             FAPI_NUM_OF_SUB_BANDS                        52
/* PUSCH Config  Ends */
/* PUCCH Config */
#define            FAPI_DELTA_PUCCH_SHIFT                        60
#define            FAPI_N_CQI_RB                                 61
#define            FAPI_N_AN_CS                                  62
#define            FAPI_N_1_PUCCH_AN                             63
/* PUCCH Config Ends */
/* SRS Config */
#define             FAPI_BANDWIDTH_CONFIGURATION                 70
#define             FAPI_MAX_UP_PTS                              71
#define             FAPI_SRS_SUB_FRAME_CONFIGURATION             72
#define             FAPI_SRS_ACK_NACK_SRS_SIMULTANEOUS_TRANSMISSION   73
/* SRS Config Ends */
/* Uplink reference signal Config */
#define             FAPI_UPLINK_RS_HOPPING                       80
#define             FAPI_GROUP_ASSIGNMENT                        81
 #define             FAPI_CYCLIC_SHIFT_1_FOR_DMRS                82
/* Uplink reference signal Config Ends */
/* TDD Frame Structure Config */
#define             FAPI_SUB_FRAME_ASSIGNMENT                    90
#define             FAPI_SPECIAL_SUB_FRAME_PATTERNS              91
#define             FAPI_MBSFN_AREA_ID                           100
              /* + PRS_CHANGES */
#define             FAPI_PRS_BANDWIDTH                           110
#define             FAPI_PRS_CYCLIC_PREFIX                       111
              /* - PRS_CHANGES */
/* TDD Frame Structure Config Ends */
           
           
/* below tags are used by L1 to reports its physical capabilities to L2/L3 software */
#define             FAPI_DL_BANDWIDTH_SUPPORT                    200
#define             FAPI_UL_BANDWIDTH_SUPPORT                    201
#define             FAPI_DL_MODULATION_SUPPORT                   202
#define             FAPI_UL_MODULATION_SUPPORT                   203
#define             FAPI_PHY_ANTENNA_CAPABILITY                  204
#define             FAPI_PHY_RELEASE_CAPABILITY                 205
#define             FAPI_PHY_MBSFN_CAPABILITY                   206 
/* below tags are used by L2/L3 software to configure the interaction between L2/L3 and L1 */
#define             FAPI_DATA_REPORT_MODE                       240
#define             FAPI_SFN_SF                                 241
/* the below tag is used by L1 to report its current status */
#define              FAPI_PHY_STATE                             250
#ifdef FLEXRAN
#define   FAPI_RACH_AND_SR_REPORT_MODE  0xF3
#define   FAPI_SRS_CONFIG_SUPPORT  0xFB
#define   FAPI_PHY_VERSION_0  0xFC
#define   FAPI_PHY_VERSION_1  0xFD
#define   FAPI_EXTRACTION_WINDOW_MARGIN  0xAA
#define   FAPI_PUCCH_NOISE_ESTIMATION_GAMMA  0xAB
#define   FAPI_PRACH_FORMAT_4_PEAK_RATIO  0xAC
#define   FAPI_PRACH_FORMAT_0_PEAK_RATIO  0xAD
#define   FAPI_DOPPLER_ESTIMATION_COMPENSATION_FACTOR  0xAE
#define   FAPI_PROBABILITY_DTX_ACK_PUSCH  0xAF
#define   FAPI_PROBABILITY_DTX_ACK_PUCCH_FORMAT_1  0xB0
#define   FAPI_UPLINK_PAYLOAD_ALLOCATION_METHOD  0xB1
#endif

#endif
/* --------------------------------------------- */

/* PHY states */
#define   FAPI_IDLE                                               0
#define   FAPI_CONFIGURED                                         1
#define   FAPI_RUNNING                                            2

/* Types of Duplexing Mode */
#define  FAPI_TDD                                                 0
#define  FAPI_FDD                                                 1
#define  FAPI_HD_FDD                                              2

/* Types of Cylic Prefix */
#define  FAPI_CP_NORMAL                                           0
#define  FAPI_CP_EXTENDED                                         1

/* UL/DL Channel Bandwidth */
#define   FAPI_CHANNEL_BW_6RB                                     6
#define   FAPI_CHANNEL_BW_15RB                                    15
#define   FAPI_CHANNEL_BW_25RB                                    25
#define   FAPI_CHANNEL_BW_50RB                                    50
#define   FAPI_CHANNEL_BW_75RB                                    75
#define   FAPI_CHANNEL_BW_100RB                                   100


/* No. of Tx Antenna Ports */
#define    FAPI_TX_ANTENNA_PORT_1                                 1
#define    FAPI_TX_ANTENNA_PORT_2                                 2
#define    FAPI_TX_ANTENNA_PORT_4                                 4


/* No. of Rx Antenna Ports */
#define   FAPI_RX_ANTENNA_PORT_1                                  1
#define   FAPI_RX_ANTENNA_PORT_2                                  2
#define   FAPI_RX_ANTENNA_PORT_4                                  4

/* POSSIBLE PHICH RESOURCE VALUES */
#define  FAPI_PHICH_R_ONE_SIXTH                                   0
#define  FAPI_PHICH_R_HALF                                        1
#define  FAPI_PHICH_R_ONE                                         2
#define  FAPI_PHICH_R_TWO                                         3

/* Phich Duration */
#define    FAPI_PHICH_D_NORMAL                                    0
#define    FAPI_PHICH_D_EXTENDED                                  1


/* High Speed Flag */
#define   FAPI_HS_UNRESTRICTED_SET                                0
#define   FAPI_HS_RESTRICTED_SET                                  1

/* Hopping Mode */
#define  FAPI_HM_INTER_SF                                         0
#define  FAPI_HM_INTRA_INTER_SF                                   1

/* Types of hopping */
#define FAPI_RS_NO_HOPPING                                        0                                       
#define FAPI_RS_GROUP_HOPPING                                     1  
#define FAPI_RS_SEQUENCE_HOPPING                                  2     

/* Physical Antenna Capabililty */
 #define   FAPI_PHY_ANTENNA_CAP_1                                 1
 #define   FAPI_PHY_ANTENNA_CAP_2                                 2
 #define   FAPI_PHY_ANTENNA_CAP_4                                 4


/* AN Repetition Factor */
 #define   FAPI_AN_REPETITION_FACTOR_2                            2
 #define   FAPI_AN_REPETITION_FACTOR_4                            4
 #define   FAPI_AN_REPETITION_FACTOR_6                            6

/* SPS DL Config Scheduling Interval */
#define  FAPI_SPS_DL_CONFIG_SCHD_INTERVAL_10                      10
#define  FAPI_SPS_DL_CONFIG_SCHD_INTERVAL_20                      20
#define  FAPI_SPS_DL_CONFIG_SCHD_INTERVAL_32                      32
#define  FAPI_SPS_DL_CONFIG_SCHD_INTERVAL_40                      40
#define  FAPI_SPS_DL_CONFIG_SCHD_INTERVAL_64                      64
#define  FAPI_SPS_DL_CONFIG_SCHD_INTERVAL_80                      80
#define  FAPI_SPS_DL_CONFIG_SCHD_INTERVAL_128                     128
#define  FAPI_SPS_DL_CONFIG_SCHD_INTERVAL_160                     160
#define  FAPI_SPS_DL_CONFIG_SCHD_INTERVAL_320                     320
#define   FAPI_SPS_DL_CONFIG_SCHD_INTERVAL_640                    640

/* Types of Resource Allocation */
#define  FAPI_RES_ALLOC_TYPE_0                                    0
#define  FAPI_RES_ALLOC_TYPE_1                                    1
#define  FAPI_RES_ALLOC_TYPE_2                                    2
   
/* vRB Assignment Flag */
 #define   FAPI_LOCALISED                                         0
 #define   FAPI_DISTRIBUTED                                       1

/* tb To CodeWordSwap Flag */
 #define   FAPI_NOSWAPPING                                        0
 #define   FAPI_SWAPPED                                           1

/* types of TPC */
 #define   FAPI_TX_POWER_CONTROL_MINUS_4                         -4
 #define   FAPI_TX_POWER_CONTROL_MINUS_1                         -1
 #define   FAPI_TX_POWER_CONTROL_0                                0
 #define   FAPI_TX_POWER_CONTROL_1                                1
 #define   FAPI_TX_POWER_CONTROL_3                                3
 #define   FAPI_TX_POWER_CONTROL_4                                4

/* Types of Transmission Scheme */
#define   FAPI_SINGLE_ANTENNA_PORT_0                              0 
#define   FAPI_TX_DIVERSITY                                       1
#define   FAPI_LARGE_DELAY_CDD                                    2
#define   FAPI_CLOSED_LOOP_SPATIAL_MULTIPLEXING                   3
#define   FAPI_MULTI_USER_MIMO                                    4    
#define   FAPI_CLOSED_LOOP_RANK_1_PRECODING                       5
#define    FAPI_SINGLE_ANTENNA_PORT_5                             6 
    /* + TM7_8 Changes Start */                                
#define   FAPI_SINGLE_ANTENNA_PORT_7                              7
#define   FAPI_SINGLE_ANTENNA_PORT_8                              8
#define   DUAL_LAYER_TX_PORT_7_AND_8                              9
    /* - TM7_8 Changes End */                                  

/* Types of RNTI */
#define   FAPI_C_RNTI                                             1 
#define   FAPI_RA_RNTI_P_RNTI_SI_RNTI                             2
#define   FAPI_SPS_CRNTI                                          3
#define   FAPI_OTHER_CRNTI                                        4
                                                                
                                                                
/* types of AggegationLevel */                                  
 #define   FAPI_AGGEGATION_LEVEL_1                                1
 #define   FAPI_AGGEGATION_LEVEL_2                                2
 #define   FAPI_AGGEGATION_LEVEL_4                                4
 #define   FAPI_AGGEGATION_LEVEL_8                                8
                                                                
/* P-A values */                                                
#define   FAPI_DB_MINUS6                                          0
#define   FAPI_DB_MINUS_4DOT77                                    1  
#define   FAPI_DB_MINUS_3                                         2
#define   FAPI_DB_MINUS_1DOT77                                    3
#define   FAPI_DB0                                                4
#define   FAPI_DB1                                                5
#define   FAPI_DB2                                                6 
#define   FAPI_DB3                                                7
 
/* Types of modulation*/
#define   FAPI_QPSK                                               2
#define   FAPI_16QAM                                              4 
#define   FAPI_64QAM                                              6

/* ul Tx Mode type */
#define   FAPI_ULTX_SISO_SIMO                                     0
#define   FAPI_ULTX_MIMO                                          1

/* Types of HI Values */
#define   FAPI_HI_NACK                                            0
#define   FAPI_HI_ACK                                             1 

/* Types of CQI Request */
#define   FAPI_APERIODIC_CQI_NOT_REQUESTED                        0
#define   FAPI_APERIODIC_CQI_REQUESTED                            1

/* ue Tx Antenna Selection */
#define   FAPI_ANT_PORT_NOT_CONFIGURED                            0
#define   FAPI_CONF_UE_PORT_0                                     1   
#define   FAPI_CONF_UE_PORT_1                                     2

/* types of DL Assignment Index */
#define   FAPI_DL_ASSGN_INDEX_1                                   1
#define   FAPI_DL_ASSGN_INDEX_2                                   2
#define   FAPI_DL_ASSGN_INDEX_3                                   3
#define   FAPI_DL_ASSGN_INDEX_4                                   4 

/* types of HARQ feedback TB1-TB2 */
#define   FAPI_ACK                                                1
#define   FAPI_NACK                                               2
#define   FAPI_ACK_OR_NACK                                        3
#define   FAPI_DTX                                                4
#define   FAPI_ACK_OR_DTX                                         5 
#define   FAPI_NACK_OR_DTX                                        6
#define   FAPI_ACK_OR_NACK_OR_DTX                                 7

/* CRC FLAGS */
#define   FAPI_CRC_CORRECT                                        0
#define   FAPI_CRC_ERROR                                          1
                                                                 
/* + TM7_8 Changes Start */
#define MAX_NUM_ANTENNA 4
/*  SPR 8128 change end */

/* + SPR 9603 Change Start*/
/* SPR 21548 fix start */
#define EPRE_TRANSFORM_BASE  68
/* SPR 21548 fix end */
/* - SPR 9603 Change End*/


/*FAPI2_1 changes start*/
/*default values for REL9 & REL10 params*/
#define  DEFAULT_MCCHFLAG              0
#define  DEFAULT_MCCH_CHANGE_NOTIF    0
#define  DEFAULT_SCRAMBLING_ID        0

#define  DEFAULT_CROSSCARRIER_SCH_FLAG             0
#define  DEFAULT_CARRIER_INDICATOR                 0
#define  DEFAULT_SRS                               0
#define  DEFAULT_SRS_REQUEST                       0
#define  DEFAULT_ANTENNA_SCRAMBLING_NUM_LAYERS     0

#define  DEFAULT_CSIRS_FLAG                        0
#define  DEFAULT_CSIRS_RES_CONFIG_REL10            0
#define  DEFAULT_CSIRS_ZEROTXPOWER_BITMAP          0
#define  DEFAULT_CSIRS_TRANSMISSIONPOWER           0
/*ULSCH */
#define  DEFAULT_ULSCH_RBCODING                    0
#define  DEFAULT_ULSCH_TRANSPORTBLOCK              1
#define  DEFAULT_ULSCH_TRANS_SCHEME                0
#define  DEFAULT_ULSCH_NUM_LAYERS                  1
#define  DEFAULT_ULSCH_CODEBOOK_INDEX              0
#define  DEFAULT_DISABLE_SEQUENCE_HOPPING          0
/*SR INFO Rel 10  & CQI Rel10*/
#define  DEFAULT_NUM_PUCCH_RESOURCE                1
#define  DEFAULT_PUCCHINDEX_P1                     0
/*DCI UL REl10*/
#define DEFAULT_CROSSCARRIER_SCHED_FLAG            0
#define DEFAULT_CARRIER_INDICATOR                  0
#define DEFAULT_SIZEOF_CQICSI                      0
#define DEFAULT_SRS_FLAG                           0
#define DEFAULT_SRS_REQUEST                        0
/*SPR 13892 changes start*/
#define DEFAULT_RESOURCE_ALLOC_FLAG                1 //since NULRB=NDLRB
/*SPR 13892 changes end*/
#define DEFAULT_RESOURCE_ALLOC_TYPE                0
/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
#define RESOURCE_ALLOC_TYPE_ONE                    1
#endif
/* SPR 21958 PUSCH RAT1 Support End */
#define DEFAULT_RESOURCE_BLOCK_CODING              0
#define DEFAULT_MCS2                               0
#define DEFAULT_NEW_DATAINDICATION2                0
#define DEFAULT_TPMI                               0
#define DEFAULT_NUM_ANTENNA_PORTS                  0
/*DCI UL REl10*/
#define DEFAULT_ANTENNA_PORT                       0
/*HI PDU REL10*/
#define DEFAULT_FLAG_TB2                           0
#define DEFAULT_NUM_CC                             1
/*HI PDU REL10*/

/*FAPI2_1 changes end*/


/* --------------------------------------------- */

/* parametrs which can be present in Config.Request */
typedef union FAPI_cellConfigParam_unT
{
     /* Type of duplexing mode 0: TDD,1: FDD,2: HD_FDD */
     uint16_t  duplexingMode;
     /*^ TAG, FAPI_DUPLEXING_MODE, GLOBAL ^*/
	
     /*The power per antenna of the PCFICH with respect to the reference signal.
     Value: 0->10000 represents -6dB to 4dB in steps 0.001dB */
     uint16_t  pcfichPowerOffset;
     /*^ TAG, FAPI_PCFICH_POWER_OFFSET ^*/
            
     /*Refers to downlink power allocation. Value is an index into the 
     referenced table.Value: 0 -> 3*/
     uint16_t  pb;
     /*^ TAG, FAPI_P_B ^*/

     /*Cyclic prefix type, used for DL 0: CP_NORMAL, 1: CP_EXTENDED. */
     uint16_t  dlCyclicPrefixType;
     /*^ TAG, FAPI_DL_CYCLIC_PREFIX_TYPE ^*/
     
     /*Cyclic prefix type, used for UL 0: CP_NORMAL, 1: CP_EXTENDED. */
     uint16_t  ulCyclicPrefixType;
     /*^ TAG, FAPI_UL_CYCLIC_PREFIX_TYPE ^*/

     /* RF Config */
    
     /*Downlink channel bandwidth in resource blocks.
     Value: 6,15, 25, 50, 75, 100 */
     uint16_t  dlChannelBW;
     /*^ TAG, FAPI_DL_CHANNEL_BANDWIDTH ^*/
    
     /*Uplink channel bandwidth in resource blocks.
     Value: 6,15, 25, 50, 75,100 */
     uint16_t  ulChannelBW;
     /*^ TAG, FAPI_UL_CHANNEL_BANDWIDTH ^*/

     /* Normalized value levels (relative) to accommodate different 
     absolute Tx Power used by eNb.
     Value: 0 . 255 Representing 0dB to -63.75dB in -0.25dB step */
     uint16_t  refSignalPower;
     /*^ TAG, FAPI_REFERENCE_SIGNAL_POWER ^*/

     /*No. of cell specific transmit antenna ports.
     Value:1,2,4 */
     uint16_t  txAntennaPort;
     /*^ TAG, FAPI_TX_ANTENNA_PORTS ^*/

     /* No. of cell specific receive antenna ports.
     Value: 1, 2, 4 */
     uint16_t  rxAntennaPort;
     /*^ TAG, FAPI_RX_ANTENNA_PORTS ^*/
     
     /* RF Config Ends */

     /* PHICH Config */

     /* No.of resource element groups used for PHICH
     0: PHICH_R_ONE_SIXTH
     1: PHICH_R_HALF
     2: PHICH_R_ONE
     3: PHICH_R_TWO */
     uint16_t  phichResource;
     /*^ TAG, FAPI_PHICH_RESOURCE ^*/

     /* No. resource element groups used for PHICH
     0: PHICH_D_NORMAL
     1: PHICH_D_EXTENDED  */
     uint16_t  phichDuration;
     /*^ TAG, FAPI_PHICH_DURATION ^*/

     /* The power per antenna of the PHICH with respect to the reference signal:
     Value: 0-> 10000, represents -6dB to 4dB in steps 0.001dB */
     uint16_t  phichPowOffset;
     /*^ TAG, FAPI_PHICH_POWER_OFFSET ^*/

     /* PHICH Config  Ends */
     
     /* SCH Config */

     /* The power of synchronization signal with respect to the reference signal
     Value: 0 . 9999 represents -6dB to 4dB in step 0.001dB */
     uint16_t  primarySyncSignal;
     /*^ TAG, FAPI_PRIMARY_SYNC_SIGNAL ^*/

     /* The power of synchronization signal with respect to the reference signal
     Value: 0 . 9999 represents -6dB to 4dB in step 0.001dB */
     uint16_t  secondarySyncSignal;
     /*^ TAG, FAPI_SECONDARY_SYNC_SIGNAL ^*/

     /* The Cell ID sent with the synchronization signal
     Value: 0 . 503 */
     uint16_t  physicalCellId;
     /*^ TAG, FAPI_PHYSICAL_CELL_ID ^*/

     /* SCH Config Ends */

     /* PRACH Config start*/
     /* Provides information about the location and format of the PRACH. 
     See TS36.211, section 5.7. Table 5.7.1-2 for FDD, Table 5.7.1-3 for TDD.        
     The value is an index into the referenced tables. Value: 0 . 63 */ 
     uint16_t  configurationIndex;
     /*^ TAG, FAPI_CONFIGURATION_INDEX ^*/

     /* PRACH Root sequence index
     Value: 0 . 837 */
     uint16_t  rootSeqIndex;
     /*^ TAG, FAPI_ROOT_SEQUENCE_INDEX ^*/
     
     /* Equivalent to Ncs, 
     TDD: 0 . 6
     FDD: 0 . 15 */
     uint16_t  zeroCorelationZoneConfig;
     /*^ TAG, FAPI_ZERO_CORRELATION_ZONE_CONFIGURATION ^*/

     /* Indicates if unrestricted, or restricted, set of preambles is used
     0: HS_UNRESTRICTED_SET
     1: HS_RESTRICTED_SET */
     uint16_t  highSpeedFlag;
     /*^ TAG, FAPI_HIGH_SPEED_FLAG ^*/

     /* The first physical resource block available for PRACH
     Value: 0 . UL_channel_bandwidth . 6 */
     uint16_t  freqOffset;
     /*^ TAG, FAPI_FREQUENCY_OFFSET ^*/
     /* PRACH Config Ends */

     /* PUSCH Config */

     /* If hopping is enabled indicates the type of hopping used
     0: HM_INTER_SF
     1: HM_INTRA_INTER_SF */
     uint16_t  hoppingMode;
     /*^ TAG, FAPI_HOPPING_MODE ^*/

     /* The offset used if hopping is enabled
     Value: 0 . 98 */
     uint16_t  hoppingOffset;
     /*^ TAG, FAPI_HOPPIG_OFFSET ^*/

     /* The number of sub-bands used for hopping
     Value: 1 . 4 */
     uint16_t  numOfSubBand;
     /*^ TAG, FAPI_NUM_OF_SUB_BANDS ^*/

     /* PUSCH Config Ends */
    
     /* PUCCH Config */

     /* The cyclic shift difference
     Value: 1 . 3 */
     uint16_t  deltaPUCCHShift;
     /*^ TAG, FAPI_DELTA_PUCCH_SHIFT ^*/

     /* The bandwidth, in units of resource blocks, that is available for use 
     by PUCCH formats 2/2a/2b transmission in each slot.
     Value: 0 . 98 */
     uint16_t  nCQIRB;
     /*^ TAG, FAPI_N_CQI_RB ^*/

     /* The number of cyclic shifts used for PUCCH formats 1/1a/1b in a 
     resource block with a mix of formats 1/a/1/ab and 2/2a/2b
     Value: 0 . 7 */
     uint16_t  nAnCs;
     /*^ TAG, FAPI_N_AN_CS ^*/
     
     uint16_t  n1PucchAn;
     /*^ TAG, FAPI_N_1_PUCCH_AN ^*/

     /* PUCCH Config Ends */

     /* SRS Config */
 
     /* The available SRS bandwidth of the cell
     The value is an index into the referenced table.
     Value: 0 . 7 */
     uint16_t  bandWidthConfiguration;
     /*^ TAG, FAPI_BANDWIDTH_CONFIGURATION ^*/

     /*  - Used for TDD only
     - Indicates how SRS operates in UpPTS subframes
     0: Disabled
     1: Enabled */
     uint16_t  macUpPTS;
     /*^ TAG, FAPI_MAX_UP_PTS ^*/

     /* The subframe configuration. 
     Needed if semi-static configuration is held in PHY.
     Value: 0 . 15 */
     uint16_t  SRSSubframeConfiguration;
     /*^ TAG, FAPI_SRS_SUB_FRAME_CONFIGURATION ^*/

     /* Indicates if SRS and ACK/NACK can be received in the same subframe. 
     Needed if semi-static configuration is held in PHY.
     0: no simultaneous transmission
     1: simultaneous transmission */
     uint8_t  srsAckNackSimulTx;
     /*^ TAG, FAPI_SRS_ACK_NACK_SRS_SIMULTANEOUS_TRANSMISSION, PADDING ^*/

     /* SRS Config Ends */

     /* Uplink Reference Signal Config */
     /* Indicates the type of hopping to use.
     0: RS_NO_HOPPING
     1: RS_GROUP_HOPPING
     2: RS_SEQUENCE_HOPPING */
     uint16_t  uplinkRSHoping;
     /*^ TAG, FAPI_UPLINK_RS_HOPPING ^*/

     /* The sequence shift pattern used if group hopping is enabled.
     Values: 0 . 29 */
     uint16_t  groupAssignment;
     /*^ TAG, FAPI_GROUP_ASSIGNMENT ^*/

     /* Specifies the cyclic shift for the reference signal used in the cell.
     The value is an index into the referenced table.
     Value: 0 . 7 */
     uint16_t  cyclicShift1forDMRS;
     /*^ TAG, FAPI_CYCLIC_SHIFT_1_FOR_DMRS ^*/
     /* Uplink Reference Signal Config Ends */

     /* TDD FRAME STRUCTURE CONFIG */

     /* indicates the DL/UL subframe structure.
     Value: 0 . 6 */
     uint16_t  subFrameAssignment;
     /*^ TAG, FAPI_SUB_FRAME_ASSIGNMENT ^*/

     /* Length of fields DwPTS, GP and UpPTS.
     Value: 0 . 8 */
     uint16_t  specialSubFramePatterns;
     /*^ TAG, FAPI_SPECIAL_SUB_FRAME_PATTERNS ^*/

     /* TDD FRAME STRUCTURE CONFIG Ends */

     /* + PRS_CHANGES */
     /*PRS bandwidth in resource blocks.
     Value: 6, 15, 25, 50, 75, 100*/
/* SPR 19288 change start */
#ifndef FLEXRAN
     uint8_t  prsBandWidth;
#endif    
/* SPR 19288 change end */
       /*^ TAG, FAPI_PRS_BANDWIDTH ^*/

     /*The cyclic prefix used for PRS transmission.
     Value: 
     0: normal cyclic prefix
     1: extended cyclic prefix*/
/* SPR 19288 change start */
#ifndef FLEXRAN
     uint8_t  prsCyclicPrifix;
#endif
/* SPR 19288 change end */
     /*^ TAG, FAPI_PRS_CYCLIC_PREFIX ^*/
     /* - PRS_CHANGES */

     /* The data report mode for the uplink data.
     0: A CRC.indication message is sent in every subframe.  
     - If ULSCH data has been processed, the CRC.indication contains 
       CRC results for the subframe. 
     - The CRC results are, also, given in the RX.indication message.
     1: The CRC.indication message is not sent. 
     - The CRC results are given in the RX.indication message. */
     uint16_t  dataReportingMode;
     /*^ TAG, FAPI_DATA_REPORT_MODE ^*/

     /* The future SFN/SF subframe where the TLVs included in the message 
     should be applied.A 16-bit value where,[15:4] SFN, range 0 . 1023
     [3:0] SF, range 0 . 9 */
     uint16_t  sfnsf;
     /*^ TAG, FAPI_SFN_SF ^*/

     /* Below parametrs are used by L1 to report its physical capabilities to 
     the L2/L3 software */

     /* these parameters are used by L1 in Param.response from TDD when PHY
     is in IDLE state */

     /* Indicates the current operational state of the PHY.

     0 = IDLE
     1 = CONFIGURED
     2 = RUNNING   */
     uint16_t    phyState; /* define as macro preprocessor  */
     /*^ TAG, FAPI_PHY_STATE ^*/
     
     /* The PHY downlink channel bandwidth capability (in resource blocks). 
     Value: bitX :0 = no support, 1= support. 
     Bit0: 6
     Bit1: 15
     Bit2: 25
     Bit3: 50
     Bit4: 75
     Bit5: 100     */

     uint16_t    dlBandWidthSupport;
     /*^ TAG, FAPI_DL_BANDWIDTH_SUPPORT ^*/
     
     /* The PHY uplink channel bandwidth capability (in resource blocks). 

     Value: bitX :0 = no support, 1= support.
     Bit0: 6
     Bit1: 15
     Bit2: 25
     Bit3: 50
     Bit4: 75
     Bit5: 100     */
     uint16_t    ulBandWidthSupport;
     /*^ TAG, FAPI_UL_BANDWIDTH_SUPPORT ^*/
    
     /* The PHY downlink modulation capability.
     Value: bitX :0 = no support, 1= support.
     Bit0: QPSK
     Bit1: 16QAM
     Bit2: 64QAM */
     uint16_t    dlModulationSupport;
     /*^ TAG, FAPI_DL_MODULATION_SUPPORT ^*/

    /* The PHY uplink modulation capability.
    Value: bitX :0 = no support, 1= support.
    Bit0: QPSK
    Bit1: 16QAM
    Bit2: 64QAM    */
    uint16_t    ulModulationSupport;
     /*^ TAG, FAPI_UL_MODULATION_SUPPORT ^*/
   
    /* Number of antennas supported.
    Value: 1, 2, 4  */
    uint16_t    phyAntennaCapability;
     /*^ TAG, FAPI_PHY_ANTENNA_CAPABILITY ^*/
#if defined( FAPI_4_0_COMPLIANCE) && !defined (FLEXRAN)
    uint16_t    releaseCapability;
	/*^ TAG , FAPI_PHY_RELEASE_CAPABILITY, GLOBAL ^*/
    /*
     * Indicates which release the PHY supports
     *
     * Value: bitX: 0 = no support, 1 = support.
     * Bit0: Release 8
     * Bit1: Release 9
     * Bit2: Release 10
     */
    uint16_t    mbsfnCapability;
	/*^ TAG, FAPI_PHY_MBSFN_CAPABILITY ^*/
    /*
     * Indicates support for MBSFN features
     *
     * Bit0: 0 = no support, 1= support
     */
    uint16_t    mbsfnAreaID;
     /*^ TAG, FAPI_MBSFN_AREA_ID ^*/
    /*Indicates MBSFN area ID
     * Value: 0 . 255*/

    uint8_t     prsBandwidth;
     /*^ TAG, FAPI_PRS_BANDWIDTH ^*/
    /*
     * PRS bandwidth in resource blocks.
     * Value: 6, 15, 25, 50, 75, 100*/

    uint8_t     prsCyclicPrefixType;
     /*^ TAG, FAPI_PRS_CYCLIC_PREFIX ^*/
    /*
     * The cyclic prefix used for PRS transmission.
     * Value: 
     * 0: normal cyclic prefix
     * 1: extended cyclic prefix
     */
/* SPR 21660_2 changes end */
#endif
#ifdef FLEXRAN
     /* Indicates which release the PHY supports
      * Value: bitX: 0 = no support, 1 = support.
      * Bit0: Release 8
      * Bit1: Release 9
      * Bit2: Release 10 */
     uint16_t         releaseCapability;

     /* Indicates support for MBSFN features
      * Bit0: 0 = no support, 1 = support   */
     uint16_t         mbsfnCapability;

     /* The reporting mode for RACH indication and SR indication.
      * - For subframes where a RACH and/or SR opportunity was scheduled:
      * 0: An indication message was always be produced. In the case of RACH,
      * if no premable was received then the indication will report zero
      * preambles. In the case of SR, if no SR was received then the
      * indication will report zero SR's.
      * 1: A RACH indication will only be generated if one or more preambles
      * was received. A SR indication will only be generated if one or more
      * SR's was received. */
     uint16_t  rachAndSrReportMode;

     /* Bit-map defining which of the SRS configurations defined in 36.211
      * 5.5.3.3-1 (FDD) or 5.5.3.3-2 (TDD) are supported.  Configuration 0 maps
      * to bit 0 and a 1 in the bit-map indicates that the configuration is
      * supported by the PHY.  For example 0x0005 would indicate support for
      * configurations 0 and 2.      */
    uint16_t        srsConfigurationSupport;

    /* Reports the release version of this PHY.
     * Byte 0:  0 for a formal (customer) release, otherwise for internal use.
     * Byte 1:  Patch release number */
    uint16_t        phyVersion0;

    /* Reports the release version of this PHY.
     * Byte 2:  Minor release number
     * Byte 3:  Major release number */
     uint16_t        phyVersion1;

     /* A margin between the end of the extraction window and the expected
      * frame of the last Ts of the SC-FDMA symbol.
      * Value: 0 to 127
      * Defualt Vlaue: 0 */
     uint16_t  extractionWindowMargin;

     /* This is the forgetting factor used in the IIR filtering for PUCCH
      * noise power estimation.
      * Value: 0 to 65535.
      * The 16-bit value is a 16-bit unsigned number with 16 fractional bits.
      * Default Value: 0x2000(corresponding to 1/8) */
     uint16_t  pucchNoiseEstimationGamma;

     /* The ration between the theshold for peak detection in PRACH to the average
      *
      power of the correlator output
      Value: 0 to 65535.
      - The 16-bit value is a 16-bit unsigned number with 16 fractional bits.
      Default Value:
      0x38cc(corresponding to 14.2)for single antenna
      0x22cc(corresponding to 8.7) for two antennas */
     uint16_t  prachFormat4PeakRatio;

     /* The ration between the theshold for peak detection in PRACH to the average
      * power of the correlator output
      * Value: 0 to 65535.
      * - The 16-bit value is a 16-bit unsigned number with 16 fractional bits.
      * Default Value:
      * 0x38cc(corresponding to 14.2)for single antenna
      * 0x2266(corresponding to 8.6) for two antennas */
     uint16_t  prachFormat0PeakRatio;

     /* A compensation factor in the Doppler estimation,which tries to
      * account for the effect of the SNR of the channel estimate based
      * on SRS.
      * - If the estimates of the signal power and noise power are perfect
      * or accurate, the compensation factor of unity is preferred theortically.
      * - However, in case of environment that these power estimates can hardly be
      * accurate, smaller weighting imposed in the SNR of the channel estimate
      * may improve the performance of doppler extimation.
      * Value: 0 to 65535.
      * - The 16-bit value is is a 16-bit unsigned number with 16 fractional bits.
      * Default Value: 0xffff(corresponding to 1-(2^16)) */
     uint16_t  dopplerEstimationCompensationFactor;

     /* The value should be set to target DTX-to-ACK probability.
      * - PHY will use this value to estimate the theshold in ACK_NACK
      * detection algoritm in PUSCH to meet the target DTX-to-ACK probability.
      * Value: 0 to 31
      * Default Value: 12 */
     uint16_t  probabilityDtxAckPusch;

     /* The value should be set to target DTX-to-ACK probability.
      * - PHY will use this value to estimate the theshold in ACK_NACK
      * detection algoritm in PUCCH uint16_t format1, 1a and 1b to meet the
      * target DTX-to-ACK probability.
      * Value: 0 to 31
      *
      Default Value: 12 */
    uint16_t  probabilityDtxAckPucchFormat1;

    /* The value should be used for choosing whether uplink payload will be allocated by L2/L3 or PHY
     * 0 = Allocated by PHY
     * 1 = Allocated by L2/L3 and passing down the pointer address through the handle field in UL_CONFIG.request
     * Default value: 0 */
    uint16_t  uplinkpayloadallocationMethod;
#endif

}FAPI_cellConfigParam_un;

/* parametrs which can be present in Config.Request during PHY Reconfig*/
typedef union FAPI_cellReConfigParam_unT
{
     /* Normalized value levels (relative) to accommodate different 
     absolute Tx Power used by eNb.
     Value: 0 . 255 Representing 0dB to -63.75dB in -0.25dB step */
     uint16_t  refSignalPower;
     /*^ TAG, FAPI_REFERENCE_SIGNAL_POWER ^*/

     /* The future SFN/SF subframe where the TLVs included in the message 
     should be applied.A 16-bit value where,[15:4] SFN, range 0 . 1023
     [3:0] SF, range 0 . 9 */
     uint16_t  sfnsf;
     /*^ TAG, FAPI_SFN_SF ^*/

     /*Added For Power Control*/
     /*Refers to downlink power allocation*/
     uint16_t pB;
     /*^ TAG, FAPI_P_B ^*/
     /*The power per antenna of the PCFICH with respect to the reference 
      * signal*/
     uint16_t pcfichPowerOffset;
     /*^ TAG, FAPI_PCFICH_POWER_OFFSET ^*/
     /*The power per antenna of the PHICH with respect to the reference 
      * signal*/
     uint16_t phichPowOffset;
     /*^ TAG, FAPI_PHICH_POWER_OFFSET ^*/

     uint16_t primarySignal;
     /*^ TAG, FAPI_PRIMARY_SYNC_SIGNAL ^*/

     uint16_t secondarySignal;
     /*^ TAG, FAPI_SECONDARY_SYNC_SIGNAL ^*/

     /* + PRS_CHANGES */
     /*PRS bandwidth in resource blocks.
     Value: 6, 15, 25, 50, 75, 100*/
/* SPR 19288 change start */
      uint8_t  prsBandWidth;
/* SPR 19288 change end */
     /*^ TAG, FAPI_PRS_BANDWIDTH ^*/

     /*The cyclic prefix used for PRS transmission.
     Value: 
     0: normal cyclic prefix
     1: extended cyclic prefix*/
/* SPR 19288 change start */
     uint8_t  prsCyclicPrifix;
/* SPR 19288 change end */
     /*^ TAG, FAPI_PRS_CYCLIC_PREFIX ^*/
     /* - PRS_CHANGES */
}FAPI_cellReConfigParam_un;    
/* ------------------------- */

/* cell Config structure */
typedef struct FAPI_cellConfig_stT 
{
     /* tag values are defined as macro preprocessor */
     uint8_t                   tag; 
     /*^ M, 0, TAG_TYPE ^*/

     /* tag length parameter ensures the complete TLV is a multiple
      * of 4-bytes (32-bits).  */
     uint8_t                   tagLen;
     /*^ M, 0 ^*/

     FAPI_cellConfigParam_un   configParam;     
     /*^ M, 0, UNION ^*/
}FAPI_cellConfig_st;


/*cell ReConfig structure */
typedef struct FAPI_cellReConfig_stT
{
    /*tag values are defined as macro preprocessor */
    uint8_t                   tag;
    /*^ M, 0, TAG_TYPE ^*/
    /* tag length parameter ensures the complete TLV is a multiple
      * of 4-bytes (32-bits).  */
    uint8_t                   tagLen;
    /*^ M, 0 ^*/
    
    FAPI_cellReConfigParam_un configParam;
    /*^ M, 0, UNION ^*/
}FAPI_cellReConfig_st;

/* --------------------------------------------- */

#if !defined(FAPI_4_0_COMPLIANCE)&&!defined(FLEXRAN)
/* UE CNFIG Possible Tags */
#define     FAPI_HANDLE                                  100
#define    FAPI_RNTI                                     101
/* CQI CONFIG */
#define    FAPI_CQI_PUCCH_RESOURCE_INDEX                 102
#define    FAPI_CQI_PMI_CONFIG_INDEX                     103
#define    FAPI_CQI_RI_CONFIG_INDEX                      104
#define    FAPI_CQI_SIMULTANEOUS_ACK_NACK_CQI            105
/* CQI CONFIG ENDS */
/* ACK/NACK CONFIG */
#define    FAPI_AN_REPETITION_FACTOR                     106
#define    FAPI_AN_N1_PUCCH_AN_REP                       107
#define    FAPI_TDD_ACK_NACK_FEEDBACK                    108 
/* ACK/NACK CONFIG  Ends*/
/* SRS CONFIG */
#define    FAPI_SRS_BANDWIDTH                            109
#define    FAPI_SRS_HOPPING_BANDWIDTH                    110
#define    FAPI_FREQUENCY_DOMAIN_POSITION                111
#define    FAPI_SRS_DURATION                             112
#define    FAPI_ISRS_SRS_CONFIG_INDEX                    113
#define    FAPI_TRANSMISSION_COMB                        114
#define    FAPI_SOUNDING_REFERENCE_SYCLIC_SHIFT          115 
/* SRS CONFIG  Ends */
/* SR CONFIG */
#define    FAPI_SR_PUCCH_RESOURCE_INDEX                  116
#define    FAPI_SR_CONFIG_INDEX                          117
/* SR CONFIG Ends */
/* SPS CONFIG */
#define   FAPI_SPS_DL_CONFIG_SCHD_INTERVAL               118
#define   FAPI_SPS_DL_N1_PUCCH_AN_PERSISTENT             119

#else

/* UE CNFIG Possible Tags */
#define    FAPI_HANDLE                                   1
#define    FAPI_RNTI                                     2
#define    FAPI_SIMULTANEOUS_PUCCH_PUSCH_R10             3
/* CQI CONFIG */
#define    FAPI_CQI_PUCCH_RESOURCE_INDEX                 10
#define    FAPI_CQI_PMI_CONFIG_INDEX                     11
#define    FAPI_CQI_RI_CONFIG_INDEX                      12
#define    FAPI_CQI_SIMULTANEOUS_ACK_NACK_CQI            13
#define    FAPI_CQI_PUCCH_RESOURCE_INDEX_P1              14 
/* CQI CONFIG ENDS */
/* ACK/NACK CONFIG */
#define    FAPI_AN_REPETITION_FACTOR                     20
#define    FAPI_AN_N1_PUCCH_AN_REP                       21
#define    FAPI_TDD_ACK_NACK_FEEDBACK                    22
#define    FAPI_NUM_N3PUCCH_AN_LIST_R10                  23
#define    FAPI_N3PUCCH_AN_LIST_R10                      24
#define    FAPI_NUM_N3PUCCH_AN_LISTP1_R10                25
#define    FAPI_N3PUCCH_AN_LISTP1_R10                    26
#define    FAPI_NBR_OF_N1_PUCCH_AN_CS_LIST_R10           27
#define    FAPI_NBR_OF_N1_PUCCH_AN_CS_R10                28
#define    FAPI_N1_PUCCH_AN_CS_R10                       29
#define    FAPI_2_ANTENNA_ACT_PUCCH_1A1B_R10             30
#define    FAPI_N1_PUCCH_AN_REP_P1_R10                   31

/* ACK/NACK CONFIG  Ends*/
/* SRS CONFIG */
#define    FAPI_SRS_BANDWIDTH                            40
#define    FAPI_SRS_HOPPING_BANDWIDTH                    41
#define    FAPI_FREQUENCY_DOMAIN_POSITION                42
#define    FAPI_SRS_DURATION                             43
#define    FAPI_ISRS_SRS_CONFIG_INDEX                    44
#define    FAPI_TRANSMISSION_COMB                        45
#define    FAPI_SOUNDING_REFERENCE_SYCLIC_SHIFT          46
/* SRS CONFIG  Ends */
/* SR CONFIG */
#define    FAPI_SR_PUCCH_RESOURCE_INDEX                  50
#define    FAPI_SR_CONFIG_INDEX                          51
#define    FAPI_SR_PUCCH_RESOURCE_INDEX_P1               52
/* SR CONFIG Ends */
/* SPS CONFIG */
#define   FAPI_SPS_DL_CONFIG_SCHD_INTERVAL               60
#define   FAPI_SPS_DL_N1_PUCCH_AN_PERSISTENT             61
#define   FAPI_SPS_DL_N1_PUCCH_AN_PERSISTENT_P1          62
#endif
/* SPS CONFIG  Ends*/

/* UE Config parametrs which wiil be filled in FAPI_ueConfig_st */

    /* An opaque handling to associate the received information in 
    RX.indication */
  
    /* uint32_T  handle; */

    /* The RNTI used for identifying the UE when receiving the PDU.
    Value: 1 . 65535. */

    /* uint16_t  rnti; */

    /* CQI Config */

    /* PUCCH resource for periodc CQI reporting
    Value: 0 . 1185 */

    /* uint16_t  cqiPucchResourceIndex; */

    /* The periodic PMI reporting configuration.
    Value: 0 . 1023. */

    /* uint16_t  cqiPmiConfigIndex; */

    /* The periodic RI reporting configuration.
    Value: 0 . 1023. */

    /* uint16_t  cqiRiConfigIndex; */

    /* Indicates if simultaneous transmission of CQI and ACK/NACK is allowed.
    Value:
    0: no PUCCH Format 2a/2b
    1: PUCCH Format 2a/2b can be used */

    /* uint16_t  simultaneousAckNackAndCqi; */

    /* CQI Config ENDS */
 
    /* ACK/NACK Config */

    /* The ACK/NACK repetition  factor.
    Value: 2,4,6 */
    
    /* uint8_t   ackNacknRepetitionFactor; */

    /* The ACK/NACK repetition PUCCH resource index.
    Value: 0. 2047 */

    /* uint16_t  anN1PUCCHANRep; */

    /* The TDD ACK/NACK Feedback Mode
    Value:
    0: bundling
    1: multiplexing */

    /* uint8_t   tddAckNackFeedbackMode; */

    /* ACK/NACK Config Ends */

    /* SRS Config */

    /*  SRS Bandwidth. This value is fixed for a UE and allocated in 
    RRC connection setup.
    Value: 0 . 3. */

    /* uint8_t   srSBandWidth; */

    /* Configures the frequency hopping on the SRS. 
    This value is fixed for a UE and allocated in RRC connection setup.
    Value 0 . 3. */

    /* uint8_t   srsHoppingBandwidth; */

    /* Frequency-domain position, NRRC This value is fixed for a UE and 
    allocated in RRC connection setup.
    Value: 0 . 23. */

    /* uint8_t   freqDomainPosition; */

    /* The duration of the SRS configuration
    Value:
    0: once
    1: indefinite */

    /* uint8_t   srsDuration; */

    /* Defines the periodicity and subframe location of the SRS.
    SRS Configuration Index. This value is fixed for a UE and 
    allocated in RRC connection setup
    Value: 0 . 1023. */

    /* uint16_t   srsConfigIndex; */

    /* Configures the frequency location of the SRS. 
    This value is fixed for a UE and allocated in RRC connection setup.
    Value: 0 .  1 */

    /* uint8_t   txComb; */

    /* Configures the SRS sequence generation. 
    This value is fixed for a UE and allocated in RRC connection setup.
    Value: 0 . 7 */

    /* uint8_t   soundingRefCyclicShift; */
    /* SRS Config Ends */

    /* SR Config */

    /* Scheduling request PUCCH resource index
    Value: 0 . 2047. */
 
    /* uint16_t  srPucchResourceIndex; */

    /* The scheduling request configuration index.
    Value: 0 . 155. */

    /* uint8_t   srConfigIndex; */

    /* SR Config Ends */

    /* SPS Config */

    /* SPS Configuration Interval
    Value: 10,20,32,40,64,80,128,160,320,640 */

    /* uint16_t  spsDlConfigSchedInterval; */

    /* The SPS PUCCH AN Resource configuration.
    The TLV can be repeated four times.
    Value: 0 . 2047. */

    /* uint16_t  spsDln1PUCCHANPersistent; */

    /* SPS Config Ends */

/* -------------------------------   */

/* UE Config structure */
typedef struct FAPI_ueConfig_stT 
{
    /* tag define in  FAPI_ueConig_en */
    uint8_t   tag;
    /*^ M, 0 ^*/

    /* tag length parameter ensures the complete TLV is a multiple of 4-bytes 
    (32-bits).  */
    uint8_t   tagLen;
    /*^ M, 0, READ ^*/

    /* The Possible values can be of size uint16_t or uint32_T or uint8_t */
    /* For more details please see above parametrs wise in comment */ 
    uint8_t   value[FAPI_VAR_SIZE(tagLen)];    
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, tagLen ^*/

}FAPI_ueConfig_st;
#ifdef FAPI_4_0_COMPLIANCE
/* UE Release structure */
typedef struct FAPI_ueRelease_stT 
{
    /* tag define in  FAPI_ueRelease_en */
    uint8_t   tag;
    /*^ M, 0 ^*/

    /* tag length parameter ensures the complete TLV is a multiple of 4-bytes 
    (32-bits).  */
    uint8_t   tagLen;
    /*^ M, 0, READ ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

    /* The Possible values can be of size uint16_t or uint32_T or uint8_t */
    /* For more details please see above parametrs wise in comment */ 
    uint8_t   value[FAPI_VAR_SIZE(tagLen)];    
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, tagLen ^*/

}FAPI_ueRelease_st;
#endif


/* --------------------------------- */

typedef struct FAPI_paramResponseTLV_stT
{
     /* tag values are defined as macro preprocessor */
     uint8_t                   tag; 
     /*^ M, 0, TAG_TYPE ^*/

     /* tag length parameter ensures the complete TLV is a multiple of 4-bytes (32-bits).  */
     uint8_t                   tagLen;
     /*^ M, 0 ^*/
     
     FAPI_cellConfigParam_un   configParam;     
     /*^ M, 0, UNION ^*/
    
}FAPI_paramResponseTLV_st;


/* macro preprocessor definition for UL/DL config/tx request */

#define    FAPI_DCI_DL_PDU        0
#define    FAPI_BCH_PDU           1
#define    FAPI_MCH_PDU           2
#define    FAPI_DLSCH_PDU         3
#define    FAPI_PCH_PDU           4
#define    FAPI_PRS_PDU           5/* + PRS_CHANGES */
#define    FAPI_CSI_RS_PDU        6

#define   FAPI_DL_DCI_FORMAT_1    0
#define   FAPI_DL_DCI_FORMAT_1A   1
#define   FAPI_DL_DCI_FORMAT_1B   2
#define   FAPI_DL_DCI_FORMAT_1C   3
#define   FAPI_DL_DCI_FORMAT_1D   4
#define   FAPI_DL_DCI_FORMAT_2    5
#define   FAPI_DL_DCI_FORMAT_2A   6
    /* + TM7_8 Changes Start */
#define   FAPI_DL_DCI_FORMAT_2B   7
    /* - TM7_8 Changes End */
#define   FAPI_DL_DCI_FORMAT_2C  8 

#define	  BUNDLING	    0
#define	  MULTIPLEXING	    1
#define	  SPECIAL_BUNDLING  2 

#define      FAPI_UL_DCI_FORMAT_0     0
#define      FAPI_UL_DCI_FORMAT_3     1
#define      FAPI_UL_DCI_FORMAT_3A    2


#define       FAPI_HI_PDU                 0
#define       FAPI_DCI_UL_PDU             1

#define       FAPI_ULSCH                    0
#define       FAPI_ULSCH_CQI_RI             1
#define       FAPI_ULSCH_HARQ               2
#define       FAPI_ULSCH_CQI_HARQ_RI        3
#define       FAPI_UCI_CQI                  4
#define       FAPI_UCI_SR                   5
#define       FAPI_UCI_HARQ                 6
#define       FAPI_UCI_SR_HARQ              7
#define       FAPI_UCI_CQI_HARQ             8
#define       FAPI_UCI_CQI_SR               9
#define       FAPI_UCI_CQI_SR_HARQ          10
#define       FAPI_SRS                      11
#define       FAPI_HARQ_BUFFER              12
#define       FAPI_ULSCH_UCI_CSI            13
#define       FAPI_ULSCH_UCI_HARQ           14
#define       FAPI_ULSCH_CSI_UCI_HARQ       15
#define       FAPI_INVALID_PDU_TYPE         255

#define       NPUSCH_FORMAT_1             1
#define       NPUSCH_FORMAT_2             2

#ifdef FAPI_4_0_COMPLIANCE
typedef struct FAPI_dciRelease9Param_st
{
    /*Indicates if format 1C is being used to signal a MCCH change notification
     * Valid for DCI formats 1C
     *
     * 0 = MCCH change notification field is not valid
     * 1 = MCCH change notification field is valid
     *
     * */
    uint8_t    mcchFlag;
    /*^ M, 0 ^*/
    /*MCCH Change Notification
     * Valid for DCI format 1C
     *  
     *  Value: 0->255
     *
     * */
    uint8_t    mcchChangeNotification;
    /*^ M, 0 ^*/

    /*Indicates the scrambling identity value NSCID 
     * Valid for DCI format 2B
     *  
     *  Value: 0,1
     *
     * */
    uint8_t    scramblingIndentity;
    /*^ M, 0 ^*/
    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_dciRelease9Param_st;

typedef struct FAPI_dciRelease10Param_st
{
    /*Indicates if cross carrier scheduling has been enabled for the UE
     * receiving this DCI
     * Valid for DCI formats 1,1A,1B, 1D,2,2A,2B,2C
     *
     * 0 = Carrier indicator field is not valid
     * 1 = Carrier indicator field is valid
     *
     * */
    uint8_t   crossCarrierSchedulingFlag;
    /*^ M, 0 ^*/

    /*Serving Cell Index
     * Valid for DCI formats 1,1A,1B,1D, 2,2A,2B,2C if the Cross-Carrier
     * Scheduling flag is enabled 
     *
     * Value: 0->7
     *
     * */
    uint8_t   carrierIndicator;
    /*^ M, 0 ^*/

    /*Indicates if the SRS request parameter is valid
     * Valid for DCI formats 1A ,2B,2C
     *
     * 0 = SRS request field is not valid
     * 1 = SRS request field is valid
     *
     * */
    uint8_t   srsFlag;
    /*^ M, 0 ^*/

    /*SRS request flag 
     * Valid for DCI formats 1A, 2B, 2C if the SRS flag is enabled.
     *
     * 0 = SRS not requested
     * 1= SRS requested
     *
     * */
    uint8_t   srsRequest;
    /*^ M, 0 ^*/

    /*Indicates the Antenna port, scrambling identity value NSCID and number of
     * layers
     * Valid for DCI format 2C
     *  
     *  Value: 0->7
     *
     * */
    uint8_t   antennaPortScramblingAndLayers;
    /*^ M, 0 ^*/

    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_dciRelease10Param_st;

typedef struct FAPI_dlschRelease9Param_st
{
    /*Used with DCI format 2B and 2C.
     * 0: NSCID=0
     * 1: NSCID=1
     *
     * */
    uint8_t   nSCID;
    /*^ M, 0 ^*/
    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_dlschRelease9Param_st;

typedef struct FAPI_dlschRelease10Param_st
{
    uint8_t  csi_rs_flag;
    /*^ M, 0 ^*/
    uint8_t  csi_rs_ResourceConfigR10;
    /*^ M, 0 ^*/
    uint16_t  csi_rs_ZeroTxPowerResConfigBitmapR10;
    /*^ M, 0 ^*/
    uint8_t  csi_rs_num_of_nzp_config;
    /*^ M, 0, READ ^*/
    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
    uint8_t  csi_rs_reference_config[FAPI_VAR_SIZE(csi_rs_num_of_nzp_config)];
	/*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, csi_rs_num_of_nzp_config ^*/
    uint8_t  pdsch_start;
    /*^ M, 0 ^*/
    STRUC_ONE_BYTE_PAD2
    /*^ M, 0, OCTET_STRING, PADDING ^*/

}FAPI_dlschRelease10Param_st;

#endif


/* structure for DCI FORMAT 1 */
typedef struct FAPI_dciFormat1_stT
{
    /* The aggregation level used
    Value: 1,2,4,8  */
    uint8_t         aggregationLevel;
    /*^ M, 0 ^*/

    /* Resource allocation type
    Valid for DCI formats: 1,2,2A
    0=type 0
    1=type 1  */
    uint8_t         resAllocationType;
    /*^ M, 0 ^*/

    /* The modulation and coding scheme for 1st transport block
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 31   */
    uint8_t         mcs_1;
    /*^ M, 0 ^*/

    /* The redundancy version for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 3   */
    uint8_t         redundancyVersion_1;
    /*^ M, 0 ^*/

    /* The encoding for the resource blocks. It's coding is dependent on 
    whether resource allocation type 0, 1, 2 is in use.
    Resource allocation type 0 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 1 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 2 is implicit for DCI formats 1A, 1B, 1C, 1D
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A   */
    uint32_T        rbCoding;
    /*^ M, 0 ^*/

    /* The new data indicator for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A  */
    uint8_t         newDataIndicator_1;
    /*^ M, 0 ^*/

    /* HARQ process number
    Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value: 0 . 7  */
    uint8_t         harqProcessNum;
    /*^ M, 0 ^*/


    /* Tx power control command for PUCCH.
    Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value:  0,1,2,3 as represented in 36.213 Sec 5.1 for TPC Command Field.
    In case of DCI format 1A and RNTI=SI-RNTI,RA-RNTI or P-RNTI 
    the TPC values are either 0,1 representing N_PRB=2 and N_PRB =3 
    respectively. */
    uint8_t         tpc;
    /*^ M, 0 ^*/

    /* The downlink assignment index. Only used in TDD mode, value ignored 
    for FDD.Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value: 1,2,3,4  */
    uint8_t         dlAssignmentIndex;
    /*^ M, 0 ^*/
    
    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps. */
    uint16_t        txPower;
    /*^ M, 0 ^*/

    /* RNTI type Valid for DCI format 1, 1A,2,2A
    1  =  C-RNTI
    2 =  RA-RNTI, P-RNTI, or SI-RNTI.
    3 = SPS-CRNTI   */
    uint8_t         rntiType;
    /*^ M, 0 ^*/

    /* Padding required for 1 bytes */
    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
#ifdef FAPI_4_0_COMPLIANCE
    FAPI_dciRelease9Param_st release9Param;
    /*^ M, 0 ^*/
    FAPI_dciRelease10Param_st release10Param;
    /*^ M, 0 ^*/
#endif


}FAPI_dciFormat1_st; /*^ API_LIKE, FAPI_dciFormat1_st ^*/

/* ---------------------------------- */

/* structure for DCI FORMAT 1A */
typedef struct FAPI_dciFormat1A_stT
{
    /* The aggregation level used
    Value: 1,2,4,8  */
    uint8_t         aggregationLevel;
    /*^ M, 0 ^*/

    /* Type of virtual resource block used
    Valid for DCI formats: 1A,1B,1D

    0 = localized
    1 = distributed  */
    uint8_t         vRBAssignmentFlag;
    /*^ M, 0 ^*/

    /* The modulation and coding scheme for 1st transport block
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 31   */
    uint8_t         mcs_1;
    /*^ M, 0 ^*/

    /* The redundancy version for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 3   */
    uint8_t         redundancyVersion_1;
    /*^ M, 0 ^*/

    /* The encoding for the resource blocks. It's coding is dependent on 
    whether resource allocation type 0, 1, 2 is in use.
    Resource allocation type 0 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 1 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 2 is implicit for DCI formats 1A, 1B, 1C, 1D
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A   */
    uint32_T        rbCoding;
    /*^ M, 0 ^*/

    /* The new data indicator for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A  */
    uint8_t         newDataIndicator_1;
    /*^ M, 0 ^*/

    /* HARQ process number
    Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value: 0 . 7  */
    uint8_t         harqProcessNum;
    /*^ M, 0 ^*/

    /* Tx power control command for PUCCH.
    Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value:  0,1,2,3 as represented in 36.213 Sec 5.1 for TPC Command Field.
    In case of DCI format 1A and RNTI=SI-RNTI,RA-RNTI or P-RNTI 
    the TPC values are either 0,1 representing N_PRB=2 and N_PRB =3 
    respectively. */
    uint8_t         tpc;
    /*^ M, 0 ^*/

    /* The downlink assignment index. Only used in TDD mode, value ignored 
    for FDD.Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value: 1,2,3,4  */
    uint8_t         dlAssignmentIndex;
    /*^ M, 0 ^*/
    
    /* Indicates that PRACH procedure is initiated Valid for DCI formats: 1A
    0 = false
    1=true   */
    uint8_t         allocatePRACHFlag;
    /*^ M, 0 ^*/

    /* The preamble index to be used on the PRACH Valid for DCI formats: 1A
    Value: 0 . 63   */
    uint8_t         preambleIndex;
    /*^ M, 0 ^*/

    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps. */
    uint16_t        txPower;
    /*^ M, 0 ^*/

    /* The mask index to be used on the PRACH Valid for DCI formats: 1A
    Value: 0 . 15  */
    uint8_t         pRACHMaskIndex;
    /*^ M, 0 ^*/

    /* RNTI type Valid for DCI format 1, 1A,2,2A
    1  =  C-RNTI
    2 =  RA-RNTI, P-RNTI, or SI-RNTI.
    3 = SPS-CRNTI   */
    uint8_t         rntiType;
    /*^ M, 0 ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
#ifdef FAPI_4_0_COMPLIANCE
    FAPI_dciRelease9Param_st release9Param;
    /*^ M, 0 ^*/
    FAPI_dciRelease10Param_st release10Param;
    /*^ M, 0 ^*/
#endif


}FAPI_dciFormat1A_st; /*^ API_LIKE, FAPI_dciFormat1A_st ^*/

/* ---------------------------------- */

/* structure for DCI FORMAT 1B */
typedef struct FAPI_dciFormat1B_stT
{
    /* The aggregation level used
    Value: 1,2,4,8  */
    uint8_t         aggregationLevel;
    /*^ M, 0 ^*/

    /* Type of virtual resource block used
    Valid for DCI formats: 1A,1B,1D

    0 = localized
    1 = distributed  */
    uint8_t         vRBAssignmentFlag;
    /*^ M, 0 ^*/

    /* The modulation and coding scheme for 1st transport block
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 31   */
    uint8_t         mcs_1;
    /*^ M, 0 ^*/

    /* The redundancy version for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 3   */
    uint8_t         redundancyVersion_1;
    /*^ M, 0 ^*/

    /* The encoding for the resource blocks. It's coding is dependent on 
    whether resource allocation type 0, 1, 2 is in use.
    Resource allocation type 0 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 1 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 2 is implicit for DCI formats 1A, 1B, 1C, 1D
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A   */
    uint32_T        rbCoding;
    /*^ M, 0 ^*/

    /* The new data indicator for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A  */
    uint8_t         newDataIndicator_1;
    /*^ M, 0 ^*/

    /* HARQ process number
    Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value: 0 . 7  */
    uint8_t         harqProcessNum;
    /*^ M, 0 ^*/

    /* The codebook index to be used for precoding
    Valid for DCI formats: 1B,1D 
    2 antenna_ports: 0 . 3
    4 antenna_ports: 0 . 15  */
    uint8_t         tPMI;
    /*^ M, 0 ^*/

    /* Confirmation for precoding
    Valid for DCI formats: 1B
    0 = use precoding indicated in TPMI field
    1 = use precoding indicated in last PMI report on PUSCH  */
    uint8_t         pmi;
    /*^ M, 0 ^*/

    /* Tx power control command for PUCCH.
    Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value:  0,1,2,3 as represented in 36.213 Sec 5.1 for TPC Command Field.
    In case of DCI format 1A and RNTI=SI-RNTI,RA-RNTI or P-RNTI 
    the TPC values are either 0,1 representing N_PRB=2 and N_PRB =3 
    respectively. */
    uint8_t         tpc;
    /*^ M, 0 ^*/

    /* The downlink assignment index. Only used in TDD mode, value ignored 
    for FDD.Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value: 1,2,3,4  */
    uint8_t         dlAssignmentIndex;
    /*^ M, 0 ^*/
    
    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps. */
    uint16_t        txPower;
    /*^ M, 0 ^*/

    /* Used in virtual resource block distribution
    Valid for DCI formats: 1B,1C,1D
    0= NGAP1
    1=  NGAP2   */
    uint8_t         nGAP;
    /*^ M, 0 ^*/

    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
#ifdef FAPI_4_0_COMPLIANCE
    FAPI_dciRelease9Param_st release9Param;
    /*^ M, 0 ^*/
    FAPI_dciRelease10Param_st release10Param;
    /*^ M, 0 ^*/
#endif

}FAPI_dciFormat1B_st;/*^ API_LIKE, FAPI_dciFormat1B_st ^*/

/* ---------------------------------- */


/* structure for DCI FORMAT 1C */
typedef struct FAPI_dciFormat1C_stT
{
    /* The aggregation level used
    Value: 1,2,4,8  */
    uint8_t         aggregationLevel;
    /*^ M, 0 ^*/

    /* The modulation and coding scheme for 1st transport block
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 31   */
    uint8_t         mcs_1;
    /*^ M, 0 ^*/

    /* The redundancy version for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 3   */
    uint8_t         redundancyVersion_1;
    /*^ M, 0 ^*/

    /* The new data indicator for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A  */
    uint8_t         newDataIndicator_1;
    /*^ M, 0 ^*/

    /* The encoding for the resource blocks. It's coding is dependent on 
    whether resource allocation type 0, 1, 2 is in use.
    Resource allocation type 0 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 1 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 2 is implicit for DCI formats 1A, 1B, 1C, 1D
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A   */
    uint32_T        rbCoding;
    /*^ M, 0 ^*/

    /* Used in virtual resource block distribution
    Valid for DCI formats: 1B,1C,1D
    0= NGAP1
    1=  NGAP2   */
    uint8_t         nGAP;
    /*^ M, 0 ^*/

    /* The transport block size
    Valid for DCI formats: 1C
    Value: 0 . 31   */
    uint8_t         tbSizeIndex;
    /*^ M, 0 ^*/

    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps. */
    uint16_t        txPower;
    /*^ M, 0 ^*/
#ifdef FAPI_4_0_COMPLIANCE
    FAPI_dciRelease9Param_st release9Param;
    /*^ M, 0 ^*/
    FAPI_dciRelease10Param_st release10Param;
    /*^ M, 0 ^*/
#endif

}FAPI_dciFormat1C_st; /*^ API_LIKE, FAPI_dciFormat1C_st ^*/

/* ---------------------------------- */

/* structure for DCI FORMAT 1D */
typedef struct FAPI_dciFormat1D_stT
{
    /* The aggregation level used
    Value: 1,2,4,8  */
    uint8_t         aggregationLevel;
    /*^ M, 0 ^*/

    /* Type of virtual resource block used
    Valid for DCI formats: 1A,1B,1D

    0 = localized
    1 = distributed  */
    uint8_t         vRBAssignmentFlag;
    /*^ M, 0 ^*/

    /* The modulation and coding scheme for 1st transport block
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 31   */
    uint8_t         mcs_1;
    /*^ M, 0 ^*/

    /* The redundancy version for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 3   */
    uint8_t         redundancyVersion_1;
    /*^ M, 0 ^*/

    /* The encoding for the resource blocks. It's coding is dependent on 
    whether resource allocation type 0, 1, 2 is in use.
    Resource allocation type 0 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 1 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 2 is implicit for DCI formats 1A, 1B, 1C, 1D
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A   */
    uint32_T        rbCoding;
    /*^ M, 0 ^*/

    /* The new data indicator for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A  */
    uint8_t         newDataIndicator_1;
    /*^ M, 0 ^*/

    /* HARQ process number
    Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value: 0 . 7  */
    uint8_t         harqProcessNum;
    /*^ M, 0 ^*/

    /* The codebook index to be used for precoding
    Valid for DCI formats: 1B,1D 
    2 antenna_ports: 0 . 3
    4 antenna_ports: 0 . 15  */
    uint8_t         tPMI;
    /*^ M, 0 ^*/

    /* Tx power control command for PUCCH.
    Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value:  0,1,2,3 as represented in 36.213 Sec 5.1 for TPC Command Field.
    In case of DCI format 1A and RNTI=SI-RNTI,RA-RNTI or P-RNTI 
    the TPC values are either 0,1 representing N_PRB=2 and N_PRB =3 
    respectively. */
    uint8_t         tpc;
    /*^ M, 0 ^*/

    /* The downlink assignment index. Only used in TDD mode, value ignored 
    for FDD.Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value: 1,2,3,4  */
    uint8_t         dlAssignmentIndex;
    /*^ M, 0 ^*/
    
    /* Used in virtual resource block distribution
    Valid for DCI formats: 1B,1C,1D
    0= NGAP1
    1=  NGAP2   */
    uint8_t         nGAP;
    /*^ M, 0 ^*/

    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps. */
    uint16_t        txPower;
    /*^ M, 0 ^*/

    /* Indicates the DL power offset type for multi-user MIMO transmission
    Valid for DCI formats: 1D
    Value: 0 . 1   */
    uint8_t         dlPowerOffset;
    /*^ M, 0 ^*/

    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
#ifdef FAPI_4_0_COMPLIANCE
    FAPI_dciRelease9Param_st release9Param;
    /*^ M, 0 ^*/
    FAPI_dciRelease10Param_st release10Param;
    /*^ M, 0 ^*/
#endif

}FAPI_dciFormat1D_st;/*^ API_LIKE, FAPI_dciFormat1D_st ^*/

/* ---------------------------------- */

/* structure for DCI FORMAT 2 */
typedef struct FAPI_dciFormat2_stT
{
    /* The aggregation level used
    Value: 1,2,4,8  */
    uint8_t         aggregationLevel;
    /*^ M, 0 ^*/

    /* Resource allocation type
    Valid for DCI formats: 1,2,2A
    0=type 0
    1=type 1  */
    uint8_t         resAllocationType;
    /*^ M, 0 ^*/

    /* The modulation and coding scheme for 1st transport block
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 31   */
    uint8_t         mcs_1;
    /*^ M, 0 ^*/

    /* The redundancy version for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 3   */
    uint8_t         redundancyVersion_1;
    /*^ M, 0 ^*/

    /* The encoding for the resource blocks. It's coding is dependent on 
    whether resource allocation type 0, 1, 2 is in use.
    Resource allocation type 0 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 1 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 2 is implicit for DCI formats 1A, 1B, 1C, 1D
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A   */
    uint32_T        rbCoding;
    /*^ M, 0 ^*/

    /* The new data indicator for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A  */
    uint8_t         newDataIndicator_1;
    /*^ M, 0 ^*/

    /* Indicates the mapping of transport block to codewords
    Valid for DCI formats: 2,2A
    0 = no swapping
    1 = swapped    */
    uint8_t         tbToCodeWordSwapFlag;
    /*^ M, 0 ^*/

    /* The modulation and coding scheme for 2nd transport block. 
    Valid for DCI formats: 2,2A
    Value: 0 . 31    */
    uint8_t         mcs_2;
    /*^ M, 0 ^*/

    /* The redundancy version for 2nd transport block. 
    Valid for DCI formats: 2,2A
    Value: 0 . 3   */
    uint8_t         redundancyVersion_2;
    /*^ M, 0 ^*/

    /* The new data indicator for 2nd transport block.
    Valid for DCI formats: 2,2A */
    uint8_t         newDataIndicator_2;
    /*^ M, 0 ^*/

    /* HARQ process number
    Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value: 0 . 7  */
    uint8_t         harqProcessNum;
    /*^ M, 0 ^*/

    /* Precoding information
    Valid for DCI formats: 2,2A
    2 antenna_ports: 0 . 7
    4 antenna_ports: 0 . 63   */
    uint8_t         preCodingInfo;
    /*^ M, 0 ^*/

    /* Tx power control command for PUCCH.
    Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value:  0,1,2,3 as represented in 36.213 Sec 5.1 for TPC Command Field.
    In case of DCI format 1A and RNTI=SI-RNTI,RA-RNTI or P-RNTI 
    the TPC values are either 0,1 representing N_PRB=2 and N_PRB =3 
    respectively. */
    uint8_t         tpc;
    /*^ M, 0 ^*/

    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps. */
    uint16_t        txPower;
    /*^ M, 0 ^*/

    /* The downlink assignment index. Only used in TDD mode, value ignored 
    for FDD.Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value: 1,2,3,4  */
    uint8_t         dlAssignmentIndex;
    /*^ M, 0 ^*/
    
    /* RNTI type Valid for DCI format 1, 1A,2,2A
    1  =  C-RNTI
    2 =  RA-RNTI, P-RNTI, or SI-RNTI.
    3 = SPS-CRNTI   */
    uint8_t         rntiType;
    /*^ M, 0 ^*/
#ifdef FAPI_4_0_COMPLIANCE
    FAPI_dciRelease9Param_st release9Param;
    /*^ M, 0  ^*/
    FAPI_dciRelease10Param_st release10Param;
    /*^ M, 0  ^*/
#endif


}FAPI_dciFormat2_st; /*^ API_LIKE, FAPI_dciFormat2_st ^*/

/* ---------------------------------- */

/* structure for DCI FORMAT 2A */
typedef struct FAPI_dciFormat2A_stT
{
    /* The aggregation level used
    Value: 1,2,4,8  */
    uint8_t         aggregationLevel;
    /*^ M, 0 ^*/

    /* Resource allocation type
    Valid for DCI formats: 1,2,2A
    0=type 0
    1=type 1  */
    uint8_t         resAllocationType;
    /*^ M, 0 ^*/

    /* The modulation and coding scheme for 1st transport block
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 31   */
    uint8_t         mcs_1;
    /*^ M, 0 ^*/

    /* The redundancy version for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A
    Value: 0 . 3   */
    uint8_t         redundancyVersion_1;
    /*^ M, 0 ^*/

    /* The encoding for the resource blocks. It's coding is dependent on 
    whether resource allocation type 0, 1, 2 is in use.
    Resource allocation type 0 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 1 is explicitly signalled for DCI formats 1, 2, 2A
    Resource allocation type 2 is implicit for DCI formats 1A, 1B, 1C, 1D
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A   */
    uint32_T        rbCoding;
    /*^ M, 0 ^*/

    /* The new data indicator for 1st transport block.
    Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A  */
    uint8_t         newDataIndicator_1;
    /*^ M, 0 ^*/

    /* Indicates the mapping of transport block to codewords
    Valid for DCI formats: 2,2A
    0 = no swapping
    1 = swapped    */
    uint8_t         tbToCodeWordSwapFlag;
    /*^ M, 0 ^*/

    /* The modulation and coding scheme for 2nd transport block. 
    Valid for DCI formats: 2,2A
    Value: 0 . 31    */
    uint8_t         mcs_2;
    /*^ M, 0 ^*/

    /* The redundancy version for 2nd transport block. 
    Valid for DCI formats: 2,2A
    Value: 0 . 3   */
    uint8_t         redundancyVersion_2;
    /*^ M, 0 ^*/

    /* The new data indicator for 2nd transport block.
    Valid for DCI formats: 2,2A */
    uint8_t         newDataIndicator_2;
    /*^ M, 0 ^*/

    /* HARQ process number
    Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value: 0 . 7  */
    uint8_t         harqProcessNum;
    /*^ M, 0 ^*/

    /* Precoding information
    Valid for DCI formats: 2,2A
    2 antenna_ports: 0 . 7
    4 antenna_ports: 0 . 63   */
    uint8_t         preCodingInfo;
    /*^ M, 0 ^*/

    /* Tx power control command for PUCCH.
    Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value:  0,1,2,3 as represented in 36.213 Sec 5.1 for TPC Command Field.
    In case of DCI format 1A and RNTI=SI-RNTI,RA-RNTI or P-RNTI 
    the TPC values are either 0,1 representing N_PRB=2 and N_PRB =3 
    respectively. */
    uint8_t         tpc;
    /*^ M, 0 ^*/

    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps. */
    uint16_t        txPower;
    /*^ M, 0 ^*/

    /* The downlink assignment index. Only used in TDD mode, value ignored 
    for FDD.Valid for DCI formats: 1,1A,1B,1D,2,2A
    Value: 1,2,3,4  */
    uint8_t         dlAssignmentIndex;
    /*^ M, 0 ^*/
    
    /* RNTI type Valid for DCI format 1, 1A,2,2A
    1  =  C-RNTI
    2 =  RA-RNTI, P-RNTI, or SI-RNTI.
    3 = SPS-CRNTI   */
    uint8_t         rntiType;
    /*^ M, 0 ^*/
#ifdef FAPI_4_0_COMPLIANCE
    FAPI_dciRelease9Param_st release9Param;
    /*^ M, 0 ^*/
    FAPI_dciRelease10Param_st release10Param;
    /*^ M, 0 ^*/
#endif


}FAPI_dciFormat2A_st; /*^ API_LIKE, FAPI_dciFormat2A_st ^*/

/* ---------------------------------- */

/* + TM7_8 Changes Start */
typedef struct FAPI_dciFormat2B_stT
{
    /* The aggregation level used
     * Value: 1,2,4,8  */
    uint8_t         aggregationLevel;
    /*^ M, 0 ^*/

    /* Resource allocation type
     * Valid for DCI formats: 1,2,2A, 2B
     * 0=type 0
     * 1=type 1  */
    uint8_t         resAllocationType;
    /*^ M, 0 ^*/

    /* The modulation and coding scheme for 1st transport block
     * Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A, 2B
     * Value: 0 . 31   */
    uint8_t         mcs_1;
    /*^ M, 0 ^*/

    /* The redundancy version for 1st transport block.
     * Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A, 2B
     * Value: 0 . 3   */
    uint8_t         redundancyVersion_1;
    /*^ M, 0 ^*/

    /* The encoding for the resource blocks. It's coding is dependent on
     * whether resource allocation type 0, 1, 2 is in use.
     * Resource allocation type 0 is explicitly signalled for DCI formats 1, 2, 2A, 2B
     * Resource allocation type 1 is explicitly signalled for DCI formats 1, 2, 2A, 2B
     * Resource allocation type 2 is implicit for DCI formats 1A, 1B, 1C, 1D
     * Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A, 2B   */
    uint32_T        rbCoding;
    /*^ M, 0 ^*/

    /* The new data indicator for 1st transport block.
     * Valid for DCI formats: 1,1A,1B,1C,1D ,2,2A, 2B  */
    uint8_t         newDataIndicator_1;
    /*^ M, 0 ^*/

    /* The modulation and coding scheme for 2nd transport block.
     * Valid for DCI formats: 2,2A, 2B
     * Value: 0 . 31    */
    uint8_t         mcs_2;
    /*^ M, 0 ^*/

    /* The redundancy version for 2nd transport block.
     * Valid for DCI formats: 2,2A, 2B
     * Value: 0 . 3   */
    uint8_t         redundancyVersion_2;
    /*^ M, 0 ^*/

    /* The new data indicator for 2nd transport block.
     * Valid for DCI formats: 2,2A, 2B */
    uint8_t         newDataIndicator_2;
    /*^ M, 0 ^*/

    /* HARQ process number
     * Valid for DCI formats: 1,1A,1B,1D,2,2A, 2B
     * Value: 0 . 7  */
    uint8_t         harqProcessNum;
    /*^ M, 0 ^*/
    /* Scrambling identity */
    uint8_t         scramblingId;
    /*^ M, 0 ^*/

    /* Tx power control command for PUCCH.
     * Valid for DCI formats: 1,1A,1B,1D,2,2A, 2B
     * Value:  0,1,2,3 as represented in 36.213 Sec 5.1 for TPC Command Field.
     * In case of DCI format 1A and RNTI=SI-RNTI,RA-RNTI or P-RNTI
     * the TPC values are either 0,1 representing N_PRB=2 and N_PRB =3
     * respectively. */
    uint8_t         tpc;
    /*^ M, 0 ^*/
    
    /* The downlink assignment index. Only used in TDD mode, value ignored
     * for FDD.Valid for DCI formats: 1,1A,1B,1D,2,2A, 2B
     * Value: 1,2,3,4  */
    uint8_t         dlAssignmentIndex;
    /*^ M, 0 ^*/

    /* Offset to the reference signal power.
     * Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps. */
    uint16_t        txPower;
    /*^ M, 0 ^*/

    /* RNTI type Valid for DCI format 1, 1A,2,2A, 2B
     * 1  =  C-RNTI
     * 2 =  RA-RNTI, P-RNTI, or SI-RNTI.
     * 3 = SPS-CRNTI   */
    uint8_t         rntiType;
    /*^ M, 0 ^*/

    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
#ifdef FAPI_4_0_COMPLIANCE
    FAPI_dciRelease9Param_st release9Param;
    /*^ M, 0 ^*/
    FAPI_dciRelease10Param_st release10Param;
    /*^ M, 0  ^*/
#endif
}FAPI_dciFormat2B_st;/*^ API_LIKE, FAPI_dciFormat2B_st ^*/

/* - TM7_8 Changes End */
/* ---------------------------------- */

/*dissector_fix*/ 

/* DCI PDU Structure */
typedef struct FAPI_dciDLPduInfo_st
{
    /* Format of the DCI
    0 = 1
    1 = 1A
    2 = 1B
    3 = 1C
    4 = 1D
    5 = 2
    6 = 2A 
    10 = 6-1A
    11 = 6-1B*/
    uint8_t         dciFormat; /* defined in  FAPI_dlDCIFormatInfo_en. */
    /*^ M, 0, READ ^*/

    /* CCE index used to send the DCI. 
    Value: 0 . 88 */
    uint8_t         cceIndex;
    /*^ M, 0 ^*/

    /* The RNTI used for identifying the UE when receiving the PDU
    Valid for all DCI formats
    Value: 1 . 65535. */
    uint16_t        rnti;
    /*^ M, 0 ^*/

    /* this dci PDU can be of any structre mentioned above based 
    upon dciFormat type. Parsing entity is required to typecast this 
    with apropriate DCI Format structure */
    uint8_t         dciPdu[FAPI_VAR_SIZE(x)];
	/*^ M, 0, OCTET_STRING, PDU_TYPE_MAP, dciFormat ^*/

}FAPI_dciDLPduInfo_st; 
#if  defined(FLEXRAN)
/*SPR 21001 Changes start */
/* FAPI DCI parameters should be the part of a common structure
 * as supported in Viettel L1 */
typedef struct FAPI_dciDlPdu_st
{
    uint8_t   dciFormat;
    uint8_t   cceIndex;
    uint8_t   aggregationLevel;
    uint16_t  rnti;
    uint8_t   resAllocationType;
    uint8_t   vRBAssignmentFlag;
    uint32_T  rbCoding;
    uint8_t   mcs_1;
    uint8_t   redundancyVersion_1;
    uint8_t   newDataIndicator_1;
    uint8_t   tbToCodeWordSwapFlag;
    uint8_t   mcs_2;
    uint8_t   redundancyVersion_2;
    uint8_t   newDataIndicator_2;
    uint8_t   harqProcessNum;
    uint8_t   tPMI;
    uint8_t   pmi;
    uint8_t   preCodingInfo;
    uint8_t   tpc;
    uint8_t   dlAssignmentIndex;
    uint8_t   nGAP;
    uint8_t   tbSizeIndex;
    uint8_t   dlPowerOffset;
    uint8_t   allocatePRACHFlag; // indicate that PRACH procedure is initiated
    uint8_t   preambleIndex;
    uint8_t   pRACHMaskIndex;
    uint8_t   rntiType;
    uint16_t  txPower;
#if   FLEXRAN
    /*Release 9 parameters*/
    uint8_t                  mcchFlag ;         //Indicates if format 1C is being used to signal a MCCH change notification Valid for DCI formats 1C 0 = MCCH change notification field is not valid    1 = MCCH change notification field is valid
    uint8_t                  mcchChangeNotification;  // MCCH Change Notification Valid for DCI format 1C value: 0->255
    uint8_t                  scramblingId;          //Indicates the scrambling identity value NSCID valid for DCI format 2B Value: 0,1
#else    
	uint8_t dciPdu[FAPI_VAR_SIZE(x)];  
#endif    
}FAPI_dciDlPduInfo_st;

/*SPR 21001 Changes end */ 
#endif

/* ---------------------------------- */

/* BCH PDU structure */
typedef struct FAPI_bchConfigPDUInfo_stT
{
    /* The length (in bytes) of the associated MAC PDU, delivered in 
    TX.request. This should be the actual length of the MAC PDU, 
    which may not be a multiple of 32-bits. */
    uint16_t         bchPduLen;
    /*^ M, 0 ^*/
    
    /* This is a count value which is incremented every time a BCH, MCH, PCH or
    DLSCH PDU is included in the DL_CONFIG.request message.
    This value is repeated in TX.request and associates the control information
    to the data.It is reset to 0 for every subframe.
    Range 0 . 65535 */
    uint16_t         pduIndex;
    /*^ M, 0, READ ^*/

    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps.  */
    uint16_t         txPower;
    /*^ M, 0 ^*/
/*SPR 21001 Changes start */
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_bchConfigPDUInfo_st;/*^ FUNC, global_op_FAPI_bchConfigPDUInfo_st(pduIndex) ^*/

/* ---------------------------------- */

/* MCH CONFIG PDU structure */
typedef struct FAPI_mchConfigPDUInfo_stT
{
    /* The length (in bytes) of the associated MAC PDU, delivered in 
    TX.request.  This should be the actual length of the MAC PDU, 
    which may not be a multiple of 32-bits  */
    uint16_t         mchPduLen;
    /*^ M, 0 ^*/

    /* This is a count value which is incremented every time a BCH, MCH, PCH or
    DLSCH PDU is included in the DL_CONFIG.request message.
    This value is repeated in TX.request and associates the control information
    to the data.It is reset to 0 for every subframe.
    Range 0 . 65535    */
    uint16_t         pduIndex;
    /*^ M, 0, READ ^*/

    /* The RNTI associated with the MCH
    Value: 1 . 65535.  */
    uint16_t         rnti;
    /*^ M, 0 ^*/

    /* Resource allocation type
    0 = type 0
    1 = type 1
    2 = type 2   */
    uint8_t          resAllocationType;
    /*^ M, 0 ^*/
#ifdef FLEXRAN
    /* The encoding for the resource blocks. It's coding is dependent on 
    whether resource allocation type 0, 1, 2 is in use. */
    uint32_T         rbCoding;
    /*^ M, 0 ^*/

    /* The modulation type used in the transport channels
    0: QPSK
    1: QAM16
    2: QAM64  */
    uint8_t          modulationType;
    /*^ M, 0 ^*/
#else
    /* The modulation type used in the transport channels
    0: QPSK
    1: QAM16
    2: QAM64  */
    uint8_t          modulationType;
    /*^ M, 0 ^*/

    /* The encoding for the resource blocks. It's coding is dependent on 
    whether resource allocation type 0, 1, 2 is in use. */
    uint32_T         rbCoding;
    /*^ M, 0 ^*/
#endif
    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps. */
    uint16_t         txPower;
    /*^ M, 0 ^*/
/*SPR 21001 Changes start */
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
    /* Indicates mbsfn areaId.Its value can range from 0 to 255 */
    uint16_t         mbsfnAreaId;
    /*^ M, 0 ^*/
}FAPI_mchConfigPDUInfo_st;  /*^ FUNC, global_op_FAPI_mchConfigPDUInfo_st(pduIndex) ^*/  

/* ---------------------------------- */

/* BEAM Froming Vector Structure */
typedef struct FAPI_beamFormingVectorInfo_stT
{
    /* Index of subband for which the following beam forming vector is
    applied.         */
    uint8_t         subBandIndex;
    /*^ M, 0 ^*/

    /* Number of physical antennas */
    uint8_t         numOfAntenna;
    /*^ M, 0, READ, FUNC,global_function_check_numOfAntenna(&numOfAntenna)^*/

    /* For each physical antenna */
    /* Beam forming vector element for physical antenna 
    #i real 8 bits followed by imaginery 8 bits  */
    uint16_t        bfValue_per_antenna[FAPI_VAR_SIZE(numOfAntenna)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfAntenna ^*/

}FAPI_beamFormingVectorInfo_st;

/* ---------------------------------- */

#ifdef FLEXRAN
typedef struct FAPI_dlSCHConfigPDUInfoPart3_stT
{
	uint8_t                  n_scid;
}FAPI_dlSCHConfigPDUInfoPart3_st;

typedef struct FAPI_dlSCHConfigPDUInfoPart2_stT
{
    /* The UE capabilities category
    Value:1 . 5   */
    uint8_t                  ueCatagoryCapacity;
    /*^ M, 0 ^*/

    /* The ratio of PDSCH EPRE to cell-specific RS EPRE among PDSCH REs 
    in all the OFDM symbols not containing cell-specific RS in dB.
    0: -6dB
    1: -4.77dB
    2: -3dB
    3: -1.77dB
    4: 0dB
    5: 1dB
    6: 2dB
    7: 3dB          */
    uint8_t                  pA;
    /*^ M, 0 ^*/

    /* Delta power offset, value: 0..1, 
    Refer to: Table 7.1.5-1 in [6] for Multi-user MIMO mode. 
    Takes value zero for all other modes. */
    uint8_t                  deltaPowerOffsetAIndex;
    /*^ M, 0 ^*/

    /* Used in virtual resource block distribution
    0= NGAP1
    1=  NGAP2   */
    uint8_t                  nGap;
    /*^ M, 0 ^*/

    /* Used with DCI format 1A and RNTI=SI-RNTI or RA-RNTI. 
    This should match the value sent in the TPC field of the 
    DCI 1A PDU which allocated this grant.
    0: NPRB = 2 
    1: NPRB = 3   */
    uint8_t                  nPRB;
    /*^ M, 0 ^*/

    /* SPR 19288 change start */
    uint8_t transmissionMode;

    STRUC_THREE_BYTE_PAD1
	    /* SPR 19288 change end */
	    /* Number of PRBs that are treated as one subband */
    uint16_t                 numRbPerSubBand;
    /*^ M, 0 ^*/
    
    /* Number of beam forming vectors
    One beam forming vector is specified for each subband */
    uint16_t                 numbfVector;
    /*^ M, 0, READ,FUNC, global_function_check_numbfVector(numbfVector) ^*/

    FAPI_beamFormingVectorInfo_st     bfVector[FAPI_VAR_SIZE(numbfVector)];

    FAPI_dlSCHConfigPDUInfoPart3_st  dlschPart3;
}FAPI_dlSCHConfigPDUInfoPart2_st;
#endif
/* DLSCH CONFIG structure */
typedef struct FAPI_dlSCHConfigPDUInfo_stT
{
    /* The length (in bytes) of the associated MAC PDU, delivered in 
    TX.request. This should be the actual length of the MAC PDU, 
    which may not be a multiple of 32-bits. */
    uint16_t                 dlschPduLen;
    /*^ M, 0 ^*/

    /* This is a count value which is incremented every time a BCH, MCH, PCH 
    or DLSCH PDU is included in the DL_CONFIG.request message.
    This value is repeated in TX.request and associates the control information
    to the data.It is reset to 0 for every subframe. 
    Range 0 . 65535   */
    uint16_t                 pduIndex;
    /*^ M, 0, READ ^*/

    /* The RNTI associated with the UE
    Value: 1 . 65535. */
    uint16_t                 rnti;
    /*^ M, 0, READ ^*/

    /* Resource allocation type
    0 = type 0
    1 = type 1
    2 = type 2  */
    uint8_t                  resAllocationType;
    /*^ M, 0 ^*/

    /* Type of virtual resource block used. This should match the value sent in
    the DCI Format 1A, 1B, 1D PDU which allocated this grant.
    0 = localized
    1 = distributed     */
    uint8_t                  vRBAssignmentFlag;
    /*^ M, 0 ^*/

    /* The encoding for the resource blocks. It's coding is dependent on 
    whether resource allocation type 0, 1, 2 is in use. This should match 
    the value sent in the DCI Format PDU which allocated this grant. */
    uint32_T                 rbCoding;
    /*^ M, 0 ^*/

    /* 2: QPSK
    4: 16QAM
    6: 64QAM   */
    uint8_t                  mcs;
    /*^ M, 0 ^*/

    /* HARQ redundancy version. This should match the value sent in the 
    DCI Format PDU which allocated this grant.
    Value: 0 . 3.   */
    uint8_t                  redundancyVersion;
    /*^ M, 0 ^*/

    /* The transport block transmitted to this RNTI. A value of 2 indicates 
    this is the second transport block for either DCI format 2 or 2A. 
    For other DCI values this field will always be 1.
    Value: 1 . 2  */
    uint8_t                  transportBlocks;
    /*^ M, 0 ^*/

    /* Indicates the mapping of transport block to codewords. 
    This should match the value sent in the DCI Format 2, 2A PDU which 
    allocated this grant.
    0 = no swapping
    1 = swapped      */
    uint8_t                  tbToCodeWordSwapFlag;
    /*^ M, 0 ^*/

    /* The MIMO mode used in the PDU
 
    0: SINGLE_ANTENNA_PORT_0, 
    1: TX_DIVERSITY, 
    2: LARGE_DELAY_CDD, 
    3: CLOSED_LOOP_SPATIAL_MULTIPLEXING, 
    4: MULTI_USER_MIMO, 
    5: CLOSED_LOOP_RANK_1_PRECODING, 
    6: SINGLE_ANTENNA_PORT_5.  */
    uint8_t                  transmissionScheme;
    /*^ M, 0 ^*/

    /* The number of layers used in transmission.
    Value: 1 . 4    */    
    uint8_t                  numOfLayers;
    /*^ M, 0 ^*/

    /* Only valid when transmission scheme = 3, 4, 5.
    Defines the number of subbands and codebooks used for PMI. 
    If value=1 then a single PMI value is supplied which should be 
    used over all RB.
    Value 0 -> 13  */
    uint8_t                  numOfSubBand;
    /*^ M, 0, READ,FUNC, global_function_check_numOfSubBand(numOfSubBand) ^*/

#ifdef FLEXRAN
    uint8_t                  subBandInfo[FAPI_VAR_SIZE(numOfSubBand)];
#endif 
#ifndef FLEXRAN
    /* The UE capabilities category
    Value:1 . 5   */
    uint8_t                  ueCatagoryCapacity;
    /*^ M, 0 ^*/

    /* The ratio of PDSCH EPRE to cell-specific RS EPRE among PDSCH REs 
    in all the OFDM symbols not containing cell-specific RS in dB.
    0: -6dB
    1: -4.77dB
    2: -3dB
    3: -1.77dB
    4: 0dB
    5: 1dB
    6: 2dB
    7: 3dB          */
    uint8_t                  pA;
    /*^ M, 0 ^*/

    /* Delta power offset, value: 0..1, 
    Refer to: Table 7.1.5-1 in [6] for Multi-user MIMO mode. 
    Takes value zero for all other modes. */
    uint8_t                  deltaPowerOffsetAIndex;
    /*^ M, 0 ^*/

    /* Used in virtual resource block distribution
    0= NGAP1
    1=  NGAP2   */
    uint8_t                  nGap;
    /*^ M, 0 ^*/

    /* Used with DCI format 1A and RNTI=SI-RNTI or RA-RNTI. 
    This should match the value sent in the TPC field of the 
    DCI 1A PDU which allocated this grant.
    0: NPRB = 2 
    1: NPRB = 3   */
    uint8_t                  nPRB;
    /*^ M, 0 ^*/
#ifdef FAPI_4_0_COMPLIANCE
    /*The transmission mode associated with the UE
     * See [6] section 7.1.
     *
     * 1: Mode 1 
     * 2: Mode 2 
     * 3: Mode 3 
     * 4: Mode 4 
     * 5: Mode 5 
     * 6: Mode 6 
     * 7: Mode 7
     * 8: Mode 8
     * 9: Mode 9
     *
     * */
/* SPR 21001 change start */
uint8_t                  transmissionMode;
/* SPR 21001 change end */
    /*^ M, 0 ^*/
    /* Number of PRBs that are treated as one subband */
    uint8_t                 numRbPerSubBand;
    /*^ M, 0 ^*/

    /* Number of beam forming vectors
       One beam forming vector is specified for each subband */
    uint8_t                 numbfVector;
    /*^ M, 0, READ,FUNC, global_function_check_numbfVector(numbfVector) ^*/
#else
/* SPR 19288 change start */
uint8_t transmissionMode;

STRUC_THREE_BYTE_PAD1
/* SPR 19288 change end */
    /* Number of PRBs that are treated as one subband */
    uint16_t                 numRbPerSubBand;
    /*^ M, 0 ^*/
    
    /* Number of beam forming vectors
    One beam forming vector is specified for each subband */
    uint16_t                 numbfVector;
    /*^ M, 0, READ,FUNC, global_function_check_numbfVector(numbfVector) ^*/
#endif

    /* + TM7_8 Changes Start */
    /* used with DCI format 2B and 2C , valid valus are 0 and 1 */
#ifdef FAPI_DEVIATIONS
#ifndef FAPI_4_0_COMPLIANCE
    /*nscid is  part of release9Param*/
/* SPR 19288 change start */
    uint8_t                  nScId;
/* SPR 19288 change end */
    /*^ M, 0 ^*/
#endif
#endif 
    /*Ratio of PDSCH EPRE to UE specific Reference signal
     *Valid only when Transmission Mode is 7 and modulation scheme is QPSK */
    /* + TM7_EPRE */
/* SPR 19288 change start */
/* SPR 19288 change end */
    /* - TM7_EPRE */
    /* - TM7_8 Changes End */

#ifdef FAPI_4_0_COMPLIANCE
    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
        FAPI_dlschRelease9Param_st release9Param;
    /*^ M, 0 ^*/
    FAPI_dlschRelease10Param_st release10Param;
    /*^ M, 0 ^*/
#else
#ifdef FAPI_DEVIATIONS
/* SPR 19288 change start */
STRUC_THREE_BYTE_PAD1
/* SPR 19288 change end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
#endif
        /* - TM7_8 Changes End */
#endif

/*SPR 13698 changes start*/
    /* Beam forming vectors */
    FAPI_beamFormingVectorInfo_st     bfVector[FAPI_VAR_SIZE(numbfVector)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, g_numbfVector ^*/
/*SPR 13698 changes end*/
    
        /* subBandInfo contains codebook indices per numOfSubBand */
    /* Only valid when transmission scheme = 3, 4, 5.
    Defines the codebook used.
    When antenna port = 1: NA
    When antenna port = 2: 0..3
    When antenna port = 4: 0..15   */
/* SPR 19288 change start */
/* SPR 21001 change start */
 uint8_t                  subBandInfo[FAPI_VAR_SIZE(numOfSubBand)];
/* SPR 21001 change end */
/* SPR 19288 change end */
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, g_numOfSubBand ^*/
#endif   
#ifdef FLEXRAN
    FAPI_dlSCHConfigPDUInfoPart2_st dlschPart2;
#endif

}FAPI_dlSCHConfigPDUInfo_st;   /*^ FUNC, global_op_FAPI_dlSCHConfigPDUInfo_st(pduIndex, rnti, &offset_counter, pduSize) ^*/

/* ---------------------------------- */


/* Paging config structure */
typedef struct FAPI_pchPduConfigInfo_stT
{
    /* The length (in bytes) of the associated MAC PDU, delivered in 
    TX.request. This should be the actual length of the MAC PDU, 
    which may not be a multiple of 32-bits. */
    uint16_t           pchPduLen;
    /*^ M, 0 ^*/

    /* This is a count value which is incremented every time a BCH, MCH, PCH 
    or DLSCH PDU is included in the DL_CONFIG.request message. This value is 
    repeated in TX.request and associates the control information to the data.
    It is reset to 0 for every subframe.
    Range 0 . 65535   */
    uint16_t           pduIndex;
    /*^ M, 0, READ ^*/

    /* The P-RNTI associated with the paging
     Value: 0xFFFE  */
    uint16_t           pRNTI;
    /*^ M, 0, READ ^*/

    /* Resource allocation type
    2 = type 2  */
    uint8_t            resAllocationType;
    /*^ M, 0 ^*/

    /* Type of virtual resource block used. This should match the value sent 
    in the DCI Format 1A, 1B, 1D PDU which allocated this grant.
    0 = localized
    1 = distributed    */
    uint8_t            vRBAssignmentFlag;
    /*^ M, 0 ^*/

    /* The encoding for the resource blocks. It's coding is dependent on 
    whether resource allocation type 0, 1, 2 is in use. This should match
    the value sent in the DCI Format PDU which allocated this grant. */
    uint32_T           rbCoding;
    /*^ M, 0 ^*/
   
    /* For PCH PDU only QPSK modulation is allowed.
    0: QPSK   */
    uint8_t            mcs;
    /*^ M, 0 ^*/

    /* For PCH PDU only redundancy version 0 is allowed
    Value: 0    */
    uint8_t            redundancyVersion;
    /*^ M, 0 ^*/

    /* The number of transport blocks transmitted to this RNTI. 
    Only 1 transport block is sent on the PCH per subframe.
    Value: 1    */
    uint8_t            numOftransportBlocks;
    /*^ M, 0 ^*/

    /* Reserved.  This parameter is not used on the PCH transport channel */
    uint8_t            tbToCodeWordSwapFlag;
    /*^ M, 0 ^*/
 
    /* The MIMO mode used in the PDU
    0: SINGLE_ANTENNA_PORT_0, 
    1: TX_DIVERSITY, 
    6: SINGLE_ANTENNA_PORT_5.    */
    uint8_t            transmissionScheme;
    /*^ M, 0 ^*/

    /* The number of layers used in transmission
    Value: 1 . 4    */
    uint8_t            numOfLayers;
    /*^ M, 0 ^*/

    /* Reserved.  This parameter is not used on the PCH transport channel. */
    uint8_t            codeBookIndex;
    /*^ M, 0 ^*/

    /* Reserved.  This parameter is not used on the PCH transport channel. */
    uint8_t            ueCatagoryCapacity;
    /*^ M, 0 ^*/

    /* The ratio of PDSCH EPRE to cell-specific RS EPRE among PDSCH REs in 
    all the OFDM symbols not containing cell-specific RS in dB.
    0: -6dB
    1: -4.77dB
    2: -3dB
    3: -1.77dB
    4: 0dB
    5: 1dB
    6: 2dB
    7: 3dB    */
    uint8_t            pA;
    /*^ M, 0 ^*/
#ifdef FLEXRAN
    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps.  */
    uint16_t           txPower;
    /*^ M, 0 ^*/

    /* Used with DCI 1A format. This should match the value sent in the 
    TPC field of the DCI 1A PDU which allocated this grant.
    0: NPRB = 2 
    1: NPRB = 3    */
    uint8_t            nPRB;
    /*^ M, 0 ^*/
#else
    /* Used with DCI 1A format. This should match the value sent in the 
    TPC field of the DCI 1A PDU which allocated this grant.
    0: NPRB = 2 
    1: NPRB = 3    */
    uint8_t            nPRB;
    /*^ M, 0 ^*/

    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps.  */
    uint16_t           txPower;
    /*^ M, 0 ^*/
#endif
/* SPR 19288 change start */
/* SPR 19288 change end */
#if defined(FAPI_4_0_COMPLIANCE) || defined(FLEXRAN)
    uint8_t            nGap;
    /*^ M, 0 ^*/
    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
#endif
}FAPI_pchPduConfigInfo_st;   /*^ FUNC, global_op_FAPI_pchPduConfigInfo_st(pduIndex, pRNTI) ^*/
/* + PRS_CHANGES */
typedef struct FAPI_prsPduConfigInfo_stT
{
    uint16_t transmissionPower;
    /*^ M, 0 ^*/
}FAPI_prsPduConfigInfo_st;
/* - PRS_CHANGES */
#ifdef FAPI_4_0_COMPLIANCE
/* CSI RS config structure */
typedef struct FAPI_csirs_PduConfigInfo_stT
{

    uint8_t csirs_ResourceConfigR10; 
    /*^ M, 0 ^*/
    uint8_t csirs_antennaPortCountR10;
    /*^ M, 0 ^*/
    uint16_t transmissionPower;
    /*^ M, 0 ^*/
    uint16_t CsiRsZeroTxPowerBitmapR10;
    /*^ M, 0 ^*/
    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

}FAPI_csirs_PduConfigInfo_st;
#endif

/* ---------------------------------- */

/* ---------------------------------- */


/* DL CONFIG union */
typedef union FAPI_dlConfigPDUInfo_unT
{
#if  !defined(FLEXRAN)
/*SPR 21001 Changes start */
    FAPI_dciDLPduInfo_st         DCIPdu;
#else
    /* New DCI structure as per the Viettel req. */
    FAPI_dciDlPduInfo_st         DCIPdu;
/*SPR 21001 Changes end */
#endif
    /*^ TAG, FAPI_DCI_DL_PDU ^*/
    FAPI_bchConfigPDUInfo_st     BCHPdu;
    /*^ TAG, FAPI_BCH_PDU ^*/
    FAPI_mchConfigPDUInfo_st     MCHPdu;
    /*^ TAG, FAPI_MCH_PDU ^*/
    FAPI_dlSCHConfigPDUInfo_st   DlSCHPdu;
    /*^ TAG, FAPI_DLSCH_PDU ^*/
    FAPI_pchPduConfigInfo_st     PCHPdu;
    /*^ TAG, FAPI_PCH_PDU ^*/
/* SPR 19288 change start */
    FAPI_prsPduConfigInfo_st     PRSPdu;
/* SPR 19288 change end */
    /*^ TAG, FAPI_PRS_PDU ^*/
#ifdef FAPI_4_0_COMPLIANCE
    FAPI_csirs_PduConfigInfo_st  CSIRSPdu;
    /*^ TAG, FAPI_CSI_RS_PDU ^*/
#endif
} FAPI_dlConfigPDUInfo_un;
/* ---------------------------------- */

/* DL Config PDU Structure */
typedef struct FAPI_dlConfigPDUInfo_stT
{
    /* 0: DCI DL PDU,
       1: BCH PDU, 
       2: MCH PDU,
       3: DLSCH PDU,
       4: PCH PDU,   
       5: PRS PDU
#ifdef IPR_NBIOT
       6: CSI RS PDU,
       7: EPDCCH DL DCI,
       8: MPDCCH,
       9: N-BCH,
       10: NPDCCH DL PDU,
       11: NDLSCH PDU 
#endif
       */
    uint8_t     pduType; /* defined in FAPI_dlPDUTypeInfo_en */
    /*^M, 0, TAG_TYPE ^*/  
    /*Size of the PDU control information (in bytes).
    This length value includes the 2 bytes required for the PDU type 
    and PDU size parameters. The size will always be a multiple of 4 bytes. */
    uint8_t     pduSize;
	/*^ GLOBAL ^*/
/*SPR 21001 Changes start */
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/

    FAPI_dlConfigPDUInfo_un     dlConfigpduInfo;
    /*^M, 0, UNION ^*/  
}FAPI_dlConfigPDUInfo_st;

/* ---------------------------------- */

/* DL TLV Structure */
typedef struct FAPI_dlTLVInfo_stT
{
      /* Range 0 -> 1
      0: Payload is carried directly in the value field
      1: Pointer to payload is in the value field  */
#ifndef REAL_UE_ENVIORNMENT      
   /* CAUTION : Used as rnti; only in simulator enviornment - only for debugging purpose 
      Please take care while updating tag in TLV */
#endif      
      uint16_t      tag;
      /*^ M, 0 ^*/

      /* Length of the payload in bytes */
      uint16_t      tagLen;
      /*^ M, 0, READ ^*/
/*SPR 21001 Changes start */
/*SPR 21001 Changes end */
      /* Always a multiple of 32-bits.
      Tag=0: Only the most significant bytes of the size indicated by .length.
      field are valid. Remaining  bytes are zero padded to the nearest 32-bit 
      boundary.
      Tag=1: Pointer to the payload. Occupies 32-bits  */
#ifdef FLEXRAN
      uint32_T      value;
      /*^ M, 0, NOT_PRESENT_IN_MESSAGE ^*/
#else
      uint32_T      value[FAPI_VAR_SIZE(tagLen)];
      /*^ M, 0, NOT_PRESENT_IN_MESSAGE ^*/
#endif

}FAPI_dlTLVInfo_st;  /*^ FUNC, global_op_FAPI_dlTLVInfo_st(tagLen, offset, &offset_counter, tvb, pinfo, subtree) ^*/

/* ---------------------------------- */

/* DL PDU Info Structure */
typedef struct FAPI_dlPduInfo_stT
{
      /* The total length (in bytes) of the PDU description and PDU data. */
      uint16_t           pduLen;
      /*^ M, 0, READ ^*/
      
      /* This is a count value which starts from 0. It is incremented for each 
      BCH, MCH, PCH or DLSCH PDU. This value was included in TX.request and 
      associates the data to the control information.It is reset to 0 for 
      every subframe.
      Range 0 . 65535 */
      uint16_t           pduIndex;
      /*^ M, 0, READ, FUNC, global_op_FAPI_dlPduInfo_st(pduIndex, pduLen) ^*/

      /* The number of TLVs describing the data of the transport block. */
      uint32_T           numOfTLV;
      /*^ M, 0, READ ^*/
      
      /* Always a multiple of 32-bits */
      FAPI_dlTLVInfo_st  dlTLVInfo[FAPI_VAR_SIZE(numOfTLV)];
      /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfTLV ^*/

}FAPI_dlPduInfo_st;

/* ---------------------------------- *//* ---------------------------------- */
#ifdef FAPI_4_0_COMPLIANCE
/* Release 10 parameters for HI PDU */
typedef struct FAPI_dlHiPduInfoRel10_stT
{
    /*Indicates is HI is present for 
    second transport block*/
    uint8_t flag_TB2;
      /*^ M, 0 ^*/
    /*
     * The PHICH value for a second transport block.
     *
     * 0: HI_NACK
     * 1: HI_ACK
     * */
    uint8_t hiValue2;
      /*^ M, 0 ^*/
    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_dlHiPduInfoRel10_st;

#endif


/* UL HI PDU structure */
typedef struct FAPI_dlHiPduInfo_stT
{
     /* 0: HI PDU */
     uint8_t     pduType;
     /*^ M, 0 ^*/
     
     /* Size of the PDU control information (in bytes).
     This length value includes the 2 bytes required for the PDU type 
     and PDU size parameters.The size will always be a multiple of 4 bytes. */
     uint8_t     hipduSize; 
     /*^ M, 0 ^*/

     /* This value is the starting resource block assigned to the ULSCH grant 
     associated with this HI response. It should match the value sent in the 
     DCI format 0 which allocated the ULSCH grant
     Value: 0 . 100   */
     uint8_t     rbStart;   
     /*^ M, 0 ^*/

     /* This value is the 2nd cyclic shift for DMRS assigned to the ULSCH grant
     associated with this HI response. It should match the value sent in the 
     DCI format 0 which allocated the ULSCH grant
     Value: 0 . 7  */
     uint8_t     cyclicShift2_forDMRS;
     /*^ M, 0 ^*/

     /* The PHICH value which is sent on the resource.
     0: HI_NACK
     1: HI_ACK     */
     uint8_t     hiValue;
     /*^ M, 0 ^*/

     /* Is used in the calculation of the PHICH location. For TDD only.
     1 = TDD subframe configuration 0 is used and the ULSCH grant associated 
         with this HI was received in subframe 4 or 9
     0 = in all other cases   */
     uint8_t     iPHICH;
     /*^ M, 0 ^*/

     /* Offset to the reference signal power.
     Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps. */
     uint16_t    txPower;
     /*^ M, 0 ^*/
#ifdef FAPI_4_0_COMPLIANCE
    FAPI_dlHiPduInfoRel10_st release10Param;
      /*^ M, 0 ^*/
#endif


}FAPI_dlHiPduInfo_st; /*^ API_LIKE ,FAPI_dlHiPduInfo_st ^*/

/* ---------------------------------- */
#ifdef FAPI_4_0_COMPLIANCE
/* SPR 10867 Fix Start */
typedef struct FAPI_dlDCIPduInfoRel10_stT
{
    /*Indicates if cross carrier scheduling has been enabled for the UE
     * receiving this DCI. 
     * Valid for DCI formats 0,4
     *
     * 0 = Carrier indicator field is not valid
     * 1 = Carrier indicator field is valid
     *
     * */
    uint8_t crossCarrierScheduling;
      /*^ M, 0 ^*/

    /*
     * Serving Cell Index 
     * Valid for DCI formats 0,4 if the Cross-Carrier Scheduling flag is enabled 
     *
     * Value: 0->7
     * */
    uint8_t carrierIndicator;
      /*^ M, 0 ^*/


    /*
     * Indicates the size of the CQI/CSI request field
     * Valid for DCI format 0,4
     *
     * 0 = 1 bit
     * 1 = 2 bits
     * */
    uint8_t sizeOfCqiCsiField;
      /*^ M, 0 ^*/

    /*
     * Indicates if the SRS request parameter is present.
     * Valid for DCI format 0
     *
     * 0 = SRS request field is not present
     * 1 = SRS request field is present
     * */
    uint8_t srsFlag;
      /*^ M, 0 ^*/

    /*SRS request 
     * Valid for DCI formats 0,4 under the following conditions
     *
     * DCI format 0: If the SRS flag field indicates this parameter is valid
     * 0 = SRS not requested
     * 1= SRS requested
     *
     * DCI format 4:
     *
     * */
    uint8_t srsRequest;
      /*^ M, 0 ^*/

    /*Indicates if the Resource Allocation Type parameter is valid.
     * Valid for DCI format 0
     *
     * 0 = Resource Allocation Type field is not valid and resource allocation
     * type 0 is used
     * 1 = Resource Allocation Type field is valid
     *
     * */
    uint8_t resourceAllocationFlag;
      /*^ M, 0 ^*/

    /*Resource allocation type/header. Valid for DCI formats 0,4 under the
     * following conditions
     * DCI format 0: If the Resource Allocation flag field indicates this
     * parameter is valid.
     *  DCI format 4: Always valid
     *
     *  0=type 0
     *  1=type 1
     *
     * */
    uint8_t resourceAllocationType;
      /*^ M, 0 ^*/

    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
    /*The encoding for the resource blocks. It's coding is dependent on whether
     * resource allocation type 0, 1 is in use and signalled for DCI formats 0,4
     *
     * For DCI format 0 this field is valid when resource allocation type 1 is
     * signalled.
     * For DCI format 4 this field is always valid.
     *
     * */
    uint32_T rbCoding;
      /*^ M, 0 ^*/

    /*
     * The modulation and redundancy version for the second transport block
     * See [6] section 8.6.
     * Valid for DCI format 4
     *
     * Value: 0 . 31
     * */
    uint8_t mcs_2;
      /*^ M, 0 ^*/

    /*The new data indicator for the second transport block
     * Valid for DCI format 4
     *
     * */
    uint8_t newDataIndicator_2;
      /*^ M, 0 ^*/
    /* Number of antenna ports for this ULSCH allocation*/
    uint8_t numAntennaPorts;
      /*^ M, 0 ^*/

    /*The codebook index to be used for precoding
     * Valid for DCI format 4 
     *
     * 2 antenna_ports: 0 . 7
     * 4 antenna_ports: 0 . 63
     *
     * */
    uint8_t tpmi;
      /*^ M, 0 ^*/
    
}FAPI_dlDCIPduInfoRel10_st;
/* SPR 10867 Fix End */
#endif


/* UL DCI PDU Structure */ 
typedef struct FAPI_dlDCIPduInfo_stT
{
     /* 1: HI PDU */
     uint8_t     pduType;
     /*^ M, 0 ^*/
     
     /* Size of the PDU control information (in bytes).
     This length value includes the 2 bytes required for the PDU type 
     and PDU size parameters.The size will always be a multiple of 4 bytes. */
     uint8_t      ulDCIPDUSize; 
     /*^ M, 0 ^*/

     /* Format of the DCI
     0 = 0
     1 = 3
     2 = 3A  */
     uint8_t     ulDCIFormat;
     /*^ M, 0 ^*/

     /* CCE index used to send the DCI. 
     Value: 0 . 88   */
     uint8_t     cceIndex;
     /*^ M, 0 ^*/
#if   defined(FLEXRAN)
     /* The aggregation level used
     Value: 1,2,4,8   */
     uint8_t     aggLevel;
     /*^ M, 0 ^*/
/*SPR 21001 Changes end */
#endif

     /* The RNTI used for identifying the UE when receiving the PDU
     Valid for all DCI formats
     Value: 1 . 65535.    */
     uint16_t    rnti;
     /*^ M, 0 ^*/
#if  !defined(FLEXRAN)
/*SPR 21001 Changes start */
     /* The aggregation level used
     Value: 1,2,4,8   */
     uint8_t     aggLevel;
     /*^ M, 0 ^*/
     /*SPR 21001 Changes end */
#endif

     /* The starting resource block for this ULSCH allocation.
     Valid for DCI format 0
     Value: 0 . 100     */
     uint8_t     rbStart;
     /*^ M, 0 ^*/

     /* The number of resource blocks allocated to this ULSCH grant.
     Valid for DCI format 0
     Value: 0 . 100      */
     uint8_t     numOfRB;
     /*^ M, 0 ^*/

     /* The modulation and redundancy version.
     Valid for DCI format 0
     Value: 0 . 31    */
     uint8_t     mcs;
     /*^ M, 0 ^*/

     /* The 2nd cyclic shift for DMRS assigned to the UE in the ULSCH grant. 
     Valid for DCI format 0
     Value: 0 . 7    */
     uint8_t     cyclicShift2_forDMRS;
     /*^ M, 0 ^*/

     /* Indicates if hopping is being used.
     Valid for DCI format 0
     0 = no hopping, 1= hopping enabled    */
     uint8_t     freqEnabledFlag;
     /*^ M, 0 ^*/

     /* The frequency hopping bits
     Valid for DCI format 0
     Value: 0 . 3  */
     uint8_t     freqHoppingBits;
     /*^ M, 0 ^*/

     /* The new data indicator for the transport block.
     Valid for DCI format 0    */
     uint8_t     newDataIndication;
     /*^ M, 0 ^*/

     /* Indicates how the CRC is calculated on the PDCCH.
     Valid for DCI format 0
     0 = Not configured;
     1 = Configured and using UE port 0;
     2 = Configured and using UE port 1.   */
     uint8_t     ueTxAntennaSelection;
     /*^ M, 0 ^*/

     /* Tx power control command for PUSCH.
     Valid for DCI format 0
     Value:  0,1,2,3 as represented in 36.213 Sec 5.1 for TPC Command Field.
     */
     uint8_t     tpc;
     /*^ M, 0 ^*/

     /* Aperiodic CQI request flag
     Valid for DCI format 0
     0 = Aperiodic CQI not requested
     1 = Aperiodic CQI requested    */
     uint8_t     cqiRequest;
     /*^ M, 0 ^*/

     /* UL index. Valid for TDD mode only.
     Valid for DCI format 0
     Value: 0,1,2,3    */
     uint8_t     ulIndex;
     /*^ M, 0 ^*/

     /* DL assignment index. Valid for TDD mode only.
     Valid for DCI format 0
     Value: 1,2,3,4     */
     uint8_t     dlAssignmentIndex;
/*SPR 21001 Changes start */
     /*^ M, 0 ^*/
#if (defined( FAPI_4_0_COMPLIANCE) && !defined (FLEXRAN)) || defined( FAPI_DEVIATIONS)
     /* CLPC_REWORK CHG */
    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
     /* Transmission Power for DCI 0/3/3A PDU */
     uint16_t    txPower;
     /*^ M, 0 ^*/
#else
    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
#endif
/*SPR 21001 Changes end */
     /* TPC commands for PUCCH and PUSCH
     Valid for DCI formats: 3,3A   */
     uint32_T    tpcBitMap;
     /*^ M, 0 ^*/
#if  defined(FLEXRAN)
/*SPR 21001 Changes start */
     /* Transmission Power for DCI 0/3/3A PDU */
     uint16_t    txPower;
     /*^ M, 0 ^*/
/*SPR 21001 Changes end */
#endif

     /* CLPC_REWORK_CHG END */
#ifdef FAPI_4_0_COMPLIANCE
/* SPR 10867 Fix Start */
        FAPI_dlDCIPduInfoRel10_st release10Param;
      /*^ M, 0 ^*/
/* SPR 10867 Fix End */
#endif
}FAPI_dlDCIPduInfo_st; /*^ API_LIKE, FAPI_dlDCIPduInfo_st ^*/
/* ---------------------------------- */


#ifdef FAPI_4_0_COMPLIANCE
typedef struct FAPI_srPduInfoRel10_stT
{
    /*
     * A value of 2 indicates that the UE is configured to transmit SR on two
     * antenna ports
     * */
    uint8_t numPucchRes;
      /*^ M, 0 ^*/

    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
    /*The PUCCH Index value for antenna port P1. Only valid when Two Antenna
     * Port Activated is True. 
     *
     * Value: 0 . 2047 for srPdu
     *
     * */
    uint16_t pucchIndexP1;
      /*^ M, 0 ^*/
}FAPI_srPduInfoRel10_st;

typedef struct FAPI_cqiPduInfoRel10_stT
{
    /*
     * A value of 2 indicates that the UE is configured to transmit
     * cqi on two
     * antenna ports
     * */
    uint8_t numPucchRes;
    /*^ M, 0 ^*/

    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
    /*The PUCCH Index value for antenna port P1. Only valid when
     * Two Antenna
     * Port Activated is True.
     *
     * Value: 0 . 1184 for cqiPdu
     *
     * */
     uint16_t pucchIndexP1;
     /*^ M, 0 ^*/
}FAPI_cqiPduInfoRel10_st;

#endif



/* CQI Information */
typedef struct FAPI_cqiPduInfo_stT
{
     /* The PUCCH Index value  
     Value: 0 . 1185    */
     uint16_t   pucchIndex; 
     /*^ M, 0 ^*/

     /* The size of the DL CQI/PMI in bits.
     Value: 0 . 255   */
     uint8_t    dlCqiPmiSize;
     /*^ M, 0 ^*/

/*SPR 21001 Changes start */
    STRUC_ONE_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
        /*FAPI2_1 changes start*/
#ifdef FAPI_4_0_COMPLIANCE
        FAPI_cqiPduInfoRel10_st release10Param;
      /*^ M, 0 ^*/
#endif
    /*FAPI2_1 changes end*/
}FAPI_cqiPduInfo_st;

/* ---------------------------------- */

/* SR Information */
typedef struct FAPI_srPduInfo_stT
{
     /* The PUCCH Index value  
     Value: 0 . 2047    */
     uint16_t   pucchIndex;
     /*^ M, 0 ^*/
#ifdef FAPI_4_0_COMPLIANCE
        FAPI_srPduInfoRel10_st release10Param;
      /*^ M, 0 ^*/
#endif
}FAPI_srPduInfo_st;

/* SR Information Ends */
/* ---------------------------------- */



/* TDD Harq Information */
typedef struct FAPI_tddHarqPduInfo_stT
{
    /* For ACK_NACK Mode 0 (Bundling) and 1 (Multiplexing), 
    this is the size of the ACK/NACK in bits.
    Value: 1 . 4
    For Special Bundling this is the expected number of ACK/NACK 
    responses (UDAI + NSPS).
    Value: 0 . 9     */
    uint8_t   harqSize;
    /*^ M, 0 ^*/

    /* The format of the ACK/NACK response expected. For TDD only.
    0 = BUNDLING
    1 = MULTIPLEXING        */
    uint8_t   acknackMode;
    /*^ M, 0 ^*/

    /* The number of ACK/NACK responses received in this subframe. 
    For TDD only. 
    Value: 0 . 4
    (Value 0 is only valid for Special Bundling.)   */
    uint8_t   numOfPUCCHResource;
    /*^ M, 0 ^*/
/*SPR 21001 Changes start */
    STRUC_ONE_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/

    /* SPR 1680 changes start*/
    /* HARQ resource 0, value: 0 . 2047 */
    uint16_t   n_PUCCH_1_0;
    /*^ M, 0 ^*/

    /* HARQ resource 1, value: 0 . 2047 */
    uint16_t   n_PUCCH_1_1;
    /*^ M, 0 ^*/

    /* HARQ resource 2, value: 0 . 2047 */
    uint16_t   n_PUCCH_1_2;
    /*^ M, 0 ^*/

    /* HARQ resource 3, value: 0 . 2047 */
    uint16_t   n_PUCCH_1_3;
      /*^ M, 0 ^*/
    /* SPR 1680 changes end*/

}FAPI_tddHarqPduInfo_st;

/* TDD Harq Information Ends */
/* ---------------------------------- */

/* FDD Harq Information */
typedef struct FAPI_fddHarqPduInfo_stT
{
    /* The PUCCH Index value for ACK/NACK
    Value: 0 . 2047    */
    uint16_t  pucchIndex;
    /*^ M, 0 ^*/

    /* The PUCCH Index value for ACK/NACK
    Value: 0 . 2047    */
/*SPR 21001 Changes start */
#if ((!defined(FAPI_1_1_COMPLIANCE)) &&  (!defined(FAPI_4_0_COMPLIANCE)))
    uint16_t  pucchIndex1;
    /*^ M, 0 ^*/
#endif
/*SPR 21001 Changes end */
    /*  The size of the ACK/NACK in bits.
Value: 1 . 2   */
    uint8_t   harqSize;
    /*^ M, 0 ^*/
/*SPR 21001 Changes start */
#if ((!defined(FAPI_1_1_COMPLIANCE)) &&  (!defined(FAPI_4_0_COMPLIANCE)))
    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
#else
    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
#endif
/*SPR 21001 Changes end */
}FAPI_fddHarqPduInfo_st;



#ifdef FAPI_4_0_COMPLIANCE
typedef struct FAPI_fddHarqPduInfoR9AndLater_stT
{

    /*   The size of the ACK/NACK in bits.
     *       Value: 1 . 20   */
    uint8_t   harqSize;
      /*^ M, 0 ^*/

    /* The format of the ACK/NACK response expected. 

       0 = Format 1a/1b
       1 = Channel Selection, added in Release 10
       2 = Format 3, added in Release 10 */
    uint8_t   ackNackMode;
      /*^ M, 0 ^*/

    /* The number of PUCCH resources associated with the ACK/NACK response
Value: 1 . 4
If the HARQ are transmitted on two antenna ports then the resource for
the second antenna is in n_PUCCH_1_1 */
    uint8_t   numOfPUCCHResource;
      /*^ M, 0 ^*/

    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

        /*  The PUCCH Index value for ACK/NACK
         *      Value: 0 . 2047 i(ACK_NACK mode = 0 or 1)
         *      Value: 0 . 549 (ACK_NACK mode = 2) */
        uint16_t  n_PUCCH_1_0;
      /*^ M, 0 ^*/
    uint16_t  n_PUCCH_1_1;
      /*^ M, 0 ^*/
    uint16_t  n_PUCCH_1_2;
      /*^ M, 0 ^*/
    uint16_t  n_PUCCH_1_3;
      /*^ M, 0 ^*/

}FAPI_fddHarqPduInfoR9AndLater_st;
#endif


#ifdef FAPI_4_0_COMPLIANCE
typedef struct FAPI_srsPduInfoRel10_st
{
    /*
     * Defines the number of antenna ports used by the UE for the SRS. This
     * value is fixed for a UE and allocated in RRC connection setup.
     *
     * 0 = 1 antenna port
     * 1 = 2 antenna ports
     * 2 = 4 antenna ports
     * */
    uint8_t antennaPort;
      /*^ M, 0 ^*/

    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_srsPduInfoRel10_st;

#endif

/* FDD Harq Information Ends */
/* ---------------------------------- */

/* SRS PDU */
typedef struct FAPI_srsPduInfo_stT
{
    /* An opaque handling returned in the SRS.indication */
    uint32_T      handle;
    /*^ M, 0 ^*/
    
    /* The size of the PDU in bytes. */
    uint16_t      size;
    /*^ M, 0 ^*/
    
    /* The RNTI used for identifying the UE when receiving the PDU
    Value: 1 . 65535.   */
    uint16_t      rnti;
    /*^ M, 0 ^*/

    /* SRS Bandwidth. This value is fixed for a UE and allocated in 
    RRC connection setup.
    Value: 0 . 3   */
    uint8_t       srsBandWidth;
    /*^ M, 0 ^*/

    /* Frequency-domain position, NRRC This value is fixed for a UE and 
    allocated in RRC connection setup.
    Value: 0 . 23    */
    uint8_t       freqDomainPosition;
    /*^ M, 0 ^*/

    /* Configures the frequency hopping on the SRS. This value is fixed 
    for a UE and allocated in RRC connection setup.
    Value 0 . 3    */
    uint8_t       srsHoppingBandWidth;
    /*^ M, 0 ^*/

    /* Configures the frequency location of the SRS. 
    This value is fixed for a UE and allocated in RRC connection setup.
    Value: 0 .  1    */
    uint8_t       transmissionComb;
    /*^ M, 0 ^*/

    /* Defines the periodicity and subframe location of the SRS.
    SRS Configuration Index. This value is fixed for a UE and 
    allocated in RRC connection setup.
    Value: 0 . 1023.   */
    uint16_t       isrsSRSConfigIndex;
    /*^ M, 0 ^*/

    /* Configures the SRS sequence generation. This value is fixed for a UE 
    and allocated in RRC connection setup.
    Value: 0 . 7    */
    uint8_t       soundingRefCyclicShift;
    /*^ M, 0 ^*/
/*SPR 21001 Changes start */
    STRUC_ONE_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
#ifdef FAPI_4_0_COMPLIANCE
        FAPI_srsPduInfoRel10_st release10Param;
      /*^ M, 0 ^*/
#endif

}FAPI_srsPduInfo_st; /*^ API_LIKE, FAPI_srsPduInfo_st ^*/
/* SRS PDU Ends */
/* ---------------------------------- */


#ifdef FAPI_4_0_COMPLIANCE
typedef struct cqiPmiRI_stT
{
    /*
     * The size of the DL CQI/PMI in bits in case of rank 1 report.
     *
     * Value: 0 . 255
     * */
    uint8_t  dlCqiPmiSize;
      /*^ M, 0 ^*/
    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/


}cqiPmiRI_st;
typedef struct dlcqiPmiRISizeAperiodic_stT
{
    /*
     * The size of RI in bits
     *
     * Value:1 . 3
     * */
/*Spr 14379 fix start */
    uint8_t   riSize;
      /*^ M, 0, READ, FUNC, global_function_check_riSize(riSize)^*/
/*Spr 14379 fix end */

    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
    /*
     * The size of the DL CQI/PMI in bits in case of rivalues (from 1 to 2^risize)
     *
     * Value: 0 . 255
     * */
/*Spr 14379 fix start */
    uint8_t   dlCqiPmiSizeRankX[FAPI_VAR_SIZE(riSize)];
      /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, g_riSize ^*/
/*Spr 14379 fix end */

}dlcqiPmiRISizeAperiodic_st;


typedef struct cqiPmiRIAperiodic_stT
{
    /*The number of CC in the aperiodic report
     * Value: 1 . 5
     *
     * */
    uint8_t   numberOfCC;
      /*^ M, 0, READ ^*/
    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
    /*RI Size as well as cqipmi size information */
    dlcqiPmiRISizeAperiodic_st  dlCqiPmiRISize[FAPI_VAR_SIZE(numberOfCC)];
      /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numberOfCC ^*/
}cqiPmiRIAperiodic_st;


typedef union pmiRIReport_st
{
    cqiPmiRI_st periodicReport;
      /*^ TAG, FAPI_APERIODIC_CQI_NOT_REQUESTED ^*/
    cqiPmiRIAperiodic_st aperiodicReport;
      /*^ TAG, FAPI_APERIODIC_CQI_REQUESTED ^*/
}pmiRIReport_st;

/* CQI_RI information */
typedef struct FAPI_cqiRiPduInfoRel9AndLater_stT
{
    /*
     * Type of CSI report
     * 0 = periodic report
     * 1 = aperiodic report
     * */
    uint8_t     reportType;
      /*^ M, 0, TAG_TYPE ^*/

    /*
     * Delta offset for CQI. This value is fixed for a UE and allocated in RRC
     * connection setup.
     * See [6] section 8.6.3
     *
     * Value: 0 . 15
     */
    uint8_t     deltaOffsetCQI;
      /*^ M, 0 ^*/

    /*
     * Delta offset for RI. This value is fixed for a UE and allocated in RRC
     * connection setup.
     * See [6] section 8.6.3
     *
     * Value: 0 . 15
     */
    uint8_t     deltaOffsetRI;
      /*^ M, 0 ^*/

    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

    /* For periodic report, cqiPmiRI_st
    * For aperiodic report, cqiPmiRIAperiodic_st*/
    pmiRIReport_st pmiRIReport;
      /*^ M, 0, UNION ^*/
}FAPI_cqiRiPduInfoRel9AndLater_stT;

typedef struct FAPI_ulschRelease10Param_st
{
    /*
     * Resource allocation type
     * See [6] section 8.1 
     *
     * 0 = type 0
     * 1 = type 1
     * */
    uint8_t   resourceAllocationType;
      /*^ M, 0 ^*/

    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
        /*
         * Used for Resource Allocation type 1
         * The encoding for the resource blocks when resource allocation type is
         * selected. This should match the value sent in the DCI Format PDU which
         * allocated this grant.
         * */
    uint32_T  resourceBlockCoding;
      /*^ M, 0 ^*/

    /*The transport block transmitted from this RNTI. 
     * Added in Release 10 
     * A value of 2 indicates this is the second transport block for DCI format
     * 4. For other DCI values this field will always be 1.
     *
     * Value: 1, 2
     *
     * */
    uint8_t   transportBlocks;
      /*^ M, 0 ^*/

    /*
     * The MIMO mode used in the PDU
     *
     * 0: SINGLE_ANTENNA_PORT_10, 
     * 1: CLOSED_LOOP_SPATIAL_MULTIPLEXING, added in Release 10
     * */
    uint8_t   transmissionScheme;
      /*^ M, 0 ^*/

    /*The number of layers used in transmission
     *
     * Value: 1 - 4
     *
     * */
    uint8_t   numOfLayers;
      /*^ M, 0 ^*/

    /*The codebook used for precoding
     * Only valid when transmission scheme = 1
     * Defines the codebook used.
     *
     * When antenna port = 1: NA
     * When antenna port = 2: 0..5 
     * When antenna port = 4: 0..23
     *
     * */
    uint8_t   codebookIndex;
      /*^ M, 0 ^*/

    /*
     * Indicates if any configured group hopping should be disabled for this
     * UE.
     * 0 = Any configured sequence hopping not disabled
     * 1 = Any configured sequence hopping disabled
     * */
    uint8_t   disableSequenceHopping;
      /*^ M, 0 ^*/
    STRUC_THREE_BYTE_PAD2
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_ulschRelease10Param_st;
#endif

#ifdef FLEXRAN
typedef struct FAPI_cqiPmiRi_sT
{
	uint8_t dlCqiPmiSize;
        uint8_t controlType;
}FAPI_cqiPmiRi_st;
#endif

/* ---------------------------------- */

/* CQI_RI information */
typedef struct FAPI_cqiRiPduInfo_stT
{
#ifdef FLEXRAN
    uint8_t reportType;
#endif
#ifndef FLEXRAN
    /* The size of the DL CQI/PMI in bits in case of rank 1 report.
    Value: 0 . 255   */
    uint8_t     dlCqiPmiSizeRank_1;
      /*^ M, 0 ^*/

    /* The size of the DL CQI/PMI in bits in case of rank>1 report.
    Value: 0 . 255   */
    uint8_t     dlCqiPmiSizeRankGT_1;
      /*^ M, 0 ^*/

    /* The size of RI in bits
    Value:1 . 2    */
    uint8_t     riSize;
      /*^ M, 0 ^*/
#endif
    /* Delta offset for CQI. This value is fixed for a UE and allocated in 
    RRC connection setup.
    Value: 0 . 15    */
    uint8_t     deltaOffsetCQI;
      /*^ M, 0 ^*/

    /* Delta offset for RI. This value is fixed for a UE and allocated in 
    RRC connection setup.
    Value: 0 . 15     */
    uint8_t     deltaOffsetRI;
      /*^ M, 0 ^*/
#ifdef FLEXRAN
    /* Only NON-CA config is supported CRAN, hence using only 1 CC
     * */
    FAPI_cqiPmiRi_st    periodicReport[1];
#endif
/*SPR 21001 Changes start */
    STRUC_THREE_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_cqiRiPduInfo_st;

/* CQI_RI information Ends */
/* ---------------------------------- */

/* UCI_SR PDU */
typedef struct FAPI_uciSrPduInfo_stT
{
       /* An opaque handling returned in the RX_CQI.indication */
       uint32_T            handle;
       /*^ M, 0 ^*/
       
       /* The RNTI used for identifying the UE when receiving the PDU
       Value: 1 . 65535.   */
       uint16_t            rnti;
       /*^ M, 0 ^*/

       FAPI_srPduInfo_st  srInfo;
       /*^ M, 0 ^*/

}FAPI_uciSrPduInfo_st; /*^ API_LIKE, FAPI_uciSrPduInfo_st ^*/

/* UCI_SR PDU Ends */
/* ---------------------------------- */

/* UCI_CQI PDU */
typedef struct FAPI_uciCqiPduInfo_stT
{
       /* An opaque handling returned in the RX_CQI.indication */
       uint32_T            handle;
       /*^ M, 0 ^*/
       
       /* The RNTI used for identifying the UE when receiving the PDU
       Value: 1 . 65535.   */
       uint16_t            rnti;
       /*^ M, 0 ^*/

/*SPR 21001 Changes start */
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/

       FAPI_cqiPduInfo_st cqiInfo;
       /*^ M, 0 ^*/

}FAPI_uciCqiPduInfo_st; /*^ API_LIKE, FAPI_uciCqiPduInfo_st ^*/

/* UCI_CQI PDU Ends */
/* ---------------------------------- */

/* UCI_HARQ PDU */
typedef struct FAPI_uciHarqPduInfo_stT
{
       /* An opaque handling returned in the RX_CQI.indication */
       uint32_T                 handle;
       /*^ M, 0 ^*/
       
       /* The RNTI used for identifying the UE when receiving the PDU
       Value: 1 . 65535.   */
       uint16_t                 rnti;
       /*^ M, 0 ^*/

/*SPR 21001 Changes start */
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
       /* depending upon Mode ( TDD or FDD ) */
#ifdef TDD_CONFIG
       FAPI_tddHarqPduInfo_st     harqInfo;
       /*^ TDD ^*/

#endif
#ifdef FDD_CONFIG

       FAPI_fddHarqPduInfo_st     harqInfo;
       /*^ FDD ^*/
#endif

}FAPI_uciHarqPduInfo_st; /*^ API_LIKE, FAPI_uciHarqPduInfo_st ^*/

#ifdef FAPI_4_0_COMPLIANCE
typedef struct FAPI_uciHarqPduInfoR9_stT
{
    /* An opaque handling returned in the RX_CQI.indication */
    uint32_T                 handle;
      /*^ M, 0 ^*/

    /* The RNTI used for identifying the UE when receiving the PDU
Value: 1 . 65535.   */
    uint16_t                 rnti;
      /*^ M, 0 ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
        /* depending upon Mode ( TDD or FDD ) */
#ifdef TDD_CONFIG
    FAPI_tddHarqPduInfo_st     harqInfo;
      /*^ TDD ^*/

#endif
#ifdef FDD_CONFIG
    FAPI_fddHarqPduInfoR9AndLater_st harqInfo;
      /*^ FDD ^*/
#endif

}FAPI_uciHarqPduInfoR9_st; /*^ API_LIKE, FAPI_uciHarqPduInfoR9_st ^*/
#endif


/* UCI_HARQ PDU Ends */
/* ---------------------------------- */

/* UCI_SR_HARQ PDU */
typedef struct FAPI_uciSrHarqPduInfo_stT
{
       /* An opaque handling returned in the RX_CQI.indication */
       uint32_T                 handle;
       /*^ M, 0 ^*/
       
       /* The RNTI used for identifying the UE when receiving the PDU
       Value: 1 . 65535.   */
       uint16_t                 rnti;
       /*^ M, 0 ^*/
       /* + SPR_15163 */
#ifdef FAPI_4_0_COMPLIANCE
        STRUC_TWO_BYTE_PAD1
       /*^ M, 0, OCTET_STRING, PADDING ^*/
#endif
       /* - SPR_15163 */

       FAPI_srPduInfo_st       srInfo;
       /*^ M, 0 ^*/

       /* depending upon Mode ( TDD or FDD ) */
#ifdef TDD_CONFIG
       FAPI_tddHarqPduInfo_st     harqInfo;
       /*^ TDD ^*/

#endif
#ifdef FDD_CONFIG
       FAPI_fddHarqPduInfo_st     harqInfo;
       /*^ FDD ^*/
#endif

}FAPI_uciSrHarqPduInfo_st; /*^ API_LIKE, FAPI_uciSrHarqPduInfo_st  ^*/

/* UCI_SR_HARQ PDU Ends */

#ifdef FAPI_4_0_COMPLIANCE
/* UCI_SR_HARQ PDU */
typedef struct FAPI_uciSrHarqPduInfoR9_stT
{
    /* An opaque handling returned in the RX_CQI.indication */
    uint32_T                 handle;
      /*^ M, 0 ^*/

    /* The RNTI used for identifying the UE when receiving the PDU
Value: 1 . 65535.   */
    uint16_t                 rnti;
      /*^ M, 0 ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
    FAPI_srPduInfo_st       srInfo;
      /*^ M, 0 ^*/

    /* depending upon Mode ( TDD or FDD ) */
#ifdef TDD_CONFIG
    FAPI_tddHarqPduInfo_st     harqInfo;
      /*^ TDD ^*/

#endif
#ifdef FDD_CONFIG
    FAPI_fddHarqPduInfoR9AndLater_st harqInfo;
      /*^ FDD ^*/
#endif

}FAPI_uciSrHarqPduInfoR9_st; /*^ API_LIKE, FAPI_uciSrHarqPduInfoR9_st  ^*/
 

#endif
/* UCI_SR_HARQ PDU Ends */




/* ---------------------------------- */

/* UCI_CQI_HARQ PDU */

typedef struct FAPI_uciCqiHarqPduInfo_stT
{
       /* An opaque handling returned in the RX_CQI.indication */
       uint32_T                 handle;
       /*^ M, 0 ^*/

#ifdef FLEXRAN
       /* The RNTI used for identifying the UE when receiving the PDU
       Value: 1 . 65535.   */
       uint16_t                 rnti;
       /*^ M, 0 ^*/

       FAPI_cqiPduInfo_st       cqiInfo;
       /*^ M, 0 ^*/
#else       
       FAPI_cqiPduInfo_st       cqiInfo;
       /*^ M, 0 ^*/

       /* The RNTI used for identifying the UE when receiving the PDU
       Value: 1 . 65535.   */
       uint16_t                 rnti;
       /*^ M, 0 ^*/
#endif
/*SPR 21001 Changes start */
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/

       /* depending upon Mode ( TDD or FDD ) */
#ifdef TDD_CONFIG
       FAPI_tddHarqPduInfo_st     harqInfo;
       /*^ TDD ^*/

#endif
#ifdef FDD_CONFIG
       FAPI_fddHarqPduInfo_st     harqInfo;
       /*^ FDD ^*/
#endif

}FAPI_uciCqiHarqPduInfo_st; /*^ API_LIKE, FAPI_uciCqiHarqPduInfo_st ^*/

/* UCI_CQI_HARQ PDU Ends */

#ifdef FAPI_4_0_COMPLIANCE
typedef struct FAPI_uciCqiHarqPduInfoR9_stT
{
    /* An opaque handling returned in the RX_CQI.indication */
    uint32_T                 handle;
      /*^ M, 0 ^*/
    /* The RNTI used for identifying the UE when receiving the PDU
Value: 1 . 65535.   */
    uint16_t                 rnti;
      /*^ M, 0 ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

    FAPI_cqiPduInfo_st       cqiInfo;
      /*^ M, 0 ^*/


    /* depending upon Mode ( TDD or FDD ) */
#ifdef TDD_CONFIG
    FAPI_tddHarqPduInfo_st     harqInfo;
      /*^ TDD ^*/
#endif
#ifdef FDD_CONFIG

    FAPI_fddHarqPduInfoR9AndLater_st harqInfo;
      /*^ FDD ^*/
#endif

}FAPI_uciCqiHarqPduInfoR9_st; /*^ API_LIKE, FAPI_uciCqiHarqPduInfoR9_st ^*/
 

#endif
/* UCI_CQI_HARQ PDU Ends */





/* ---------------------------------- */

/* UCI_CQI_SR PDU */
typedef struct FAPI_uciCqiSrPduInfo_stT
{
       /* An opaque handling returned in the RX_CQI.indication */
       uint32_T                 handle;
       /*^ M, 0 ^*/
       
       /* The RNTI used for identifying the UE when receiving the PDU
       Value: 1 . 65535.   */
       uint16_t                 rnti;
       /*^ M, 0 ^*/

       FAPI_cqiPduInfo_st      cqiInfo;
       /*^ M, 0 ^*/

       FAPI_srPduInfo_st       srInfo;
       /*^ M, 0 ^*/

}FAPI_uciCqiSrPduInfo_st; /*^ API_LIKE, FAPI_uciCqiSrPduInfo_st ^*/

/* UCI_CQI_SR PDU Ends */
/* ---------------------------------- */

/* UCI_CQI_SR HARQ_PDU */
typedef struct FAPI_uciCqiSrHarqPduInfo_stT
{
       /* An opaque handling returned in the RX_CQI.indication */
       uint32_T                 handle;
       /*^ M, 0 ^*/
       
       /* The RNTI used for identifying the UE when receiving the PDU
       Value: 1 . 65535.   */
       uint16_t                 rnti;
       /*^ M, 0 ^*/
       /* + SPR_15163 */
#ifdef FAPI_4_0_COMPLIANCE
       STRUC_TWO_BYTE_PAD1
       /*^ M, 0, OCTET_STRING, PADDING ^*/
#endif 

       /* - SPR_15163 */
       /* SPR 21512 fix start */
       FAPI_cqiPduInfo_st      cqiInfo;
       /*^ M, 0 ^*/
       FAPI_srPduInfo_st       srInfo;
       /*^ M, 0 ^*/

       /* SPR 21512 fix end */
       
       /* depending upon Mode ( TDD or FDD ) */
#ifdef TDD_CONFIG
       FAPI_tddHarqPduInfo_st     harqInfo;
       /*^ TDD ^*/
#endif
#ifdef FDD_CONFIG

       FAPI_fddHarqPduInfo_st     harqInfo;
       /*^ FDD ^*/
#endif

}FAPI_uciCqiSrHarqPduInfo_st; /*^ API_LIKE, FAPI_uciCqiSrHarqPduInfo_st  ^*/ 

/* UCI_CQI_SR HARQ_PDU  Ends */


#ifdef FAPI_4_0_COMPLIANCE
/* UCI_CQI_SR HARQ_PDU */
typedef struct FAPI_uciCqiSrHarqPduInfoR9_stT
{
    /* An opaque handling returned in the RX_CQI.indication */
    uint32_T                 handle;
      /*^ M, 0 ^*/

    /* The RNTI used for identifying the UE when receiving the PDU
Value: 1 . 65535.   */
    uint16_t                 rnti;
      /*^ M, 0 ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

    FAPI_cqiPduInfo_st      cqiInfo;
      /*^ M, 0 ^*/

    FAPI_srPduInfo_st       srInfo;
      /*^ M, 0 ^*/

    /* depending upon Mode ( TDD or FDD ) */
#ifdef TDD_CONFIG
    FAPI_tddHarqPduInfo_st     harqInfo;
      /*^ TDD ^*/

#endif
#ifdef FDD_CONFIG
    FAPI_fddHarqPduInfoR9AndLater_st harqInfo;
      /*^ FDD ^*/
#endif

}FAPI_uciCqiSrHarqPduInfoR9_st;  /*^ API_LIKE, FAPI_uciCqiSrHarqPduInfoR9_st ^*/
#endif

/* UCI_CQI_SR HARQ_PDU  Ends */
/* ---------------------------------- */

/* ULSCH PDU */
typedef struct FAPI_ulSCHPduInfo_stT 
{
     /* An opaque handling returned in the RX.indication */ 
     uint32_T      handle;
     /*^ M, 0 ^*/
     
     /* The size of the ULSCH PDU in bytes as defined by the relevant UL grant.
     The size can be 0 if UCI over ULSCH without data is configured. 
     The size of CQI/RI/HARQ are not added to this element. */
     uint16_t       size;
     /*^ M, 0 ^*/

     /* The RNTI used for identifying the UE when receiving the PDU
     Value: 1 . 65535. */
     uint16_t       rnti;
     /*^ M, 0 ^*/

     /* The starting resource block for this ULSCH allocation. 
     This should match the value sent in the DCI Format 0 PDU 
     which allocated this grant.
     Value: 0 . 99    */
     uint8_t       rbStart;
     /*^ M, 0 ^*/

     /* The number of resource blocks allocated to this ULSCH grant. 
     This should match the value sent in the DCI Format 0 PDU 
     which allocated this grant.
     Value: 1 . 100    */
     uint8_t       numOfRB;
     /*^ M, 0 ^*/

     /* 2: QPSK
     4: 16QAM
     6: 64QAM   */
     uint8_t       modulationType;
     /*^ M, 0 ^*/

     /* The 2nd cyclic shift for DMRS assigned to the UE in the ULSCH grant. 
     This should match the value sent in the DCI Format 0 PDU 
     which allocated this grant.
     Value: 0 . 7 */
     uint8_t       cyclicShift2forDMRS;
     /*^ M, 0 ^*/

     /* Indicates if hopping is being used. This should match the value 
     sent in the DCI Format 0 PDU which allocated this grant.
     0 = no hopping, 1= hopping enabled   */
     uint8_t       freqHoppingenabledFlag;
     /*^ M, 0 ^*/

     /* The frequency hopping bits. This should match the value sent in 
     the DCI Format 0 PDU which allocated this grant.
     Value: 0 . 3      */
     uint8_t       freqHoppingBits;
     /*^ M, 0 ^*/

     /* Specify whether this received PUSCH is a new transmission from UE. 
     This should match the value sent in the DCI Format 0 PDU which 
     allocated this grant. */
     uint8_t       newDataIndication;
     /*^ M, 0 ^*/

     /* Redundancy version
     Value: 0 . 3  */
     uint8_t       redundancyVersion;
     /*^ M, 0 ^*/

     /* HARQ Process number.
     TDD 0 . 15
     FDD 0 . 7   */
     uint8_t       harqProcessNumber;
     /*^ M, 0 ^*/

     /* 0 = SISO/SIMO
     1 = MIMO     */
     uint8_t       ulTxMode;
     /*^ M, 0 ^*/

     /* The current HARQ transmission count of this transport block. 
     Valid if frequency hopping enabled. */
     uint8_t       currentTxNB;
     /*^ M, 0 ^*/

     /* Indicates if the resource blocks allocated for this grant 
     overlap with the SRS configuration.
     0 = no overlap
     1 = overlap     */
     uint8_t       nSRS;
     /*^ M, 0 ^*/
#ifdef FAPI_4_0_COMPLIANCE
    FAPI_ulschRelease10Param_st release10Param;
      /*^ M, 0 ^*/
#endif


}FAPI_ulSCHPduInfo_st;

/* ULSCH PDU Ends */
/* ---------------------------------- */

/* Initial transmission parameters */
typedef struct FAPI_initialTxParam_stT
{
       /* 0 = last OFDM symbol is not punctured
       1 = last OFDM symbol is punctured. */
       uint8_t  nSRSInitial;
       /*^ M, 0 ^*/

       /* The number of resource blocks used in the initial transmission 
       of this transport block.
       Value: 1 . 100    */
       uint8_t  initialNumOfRB;
       /*^ M, 0 ^*/
/*SPR 21001 Changes start */
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_initialTxParam_st;

/* Initial transmission parameters Ends */
/* ---------------------------------- */



typedef struct FAPI_ulSCHHarqInfo_stT
{
    /*The size of the ACK/NACK in bits Value 1-2*/
    uint8_t         harqSize;
    /*^ M, 0 ^*/
    /*Delta offset for HARQ. This value is fixed for a UE and allocated in
     *RRC connection setup. Value: 0-15*/
    uint8_t         deltaOffsetHarq;
    /*^ M, 0 ^*/
    /*The format of the ACK/NACK response expected. For TDD only.
     * 0 = BUNDLING
     * 1 = MULTIPLEXING
     * */
    uint8_t         ackNackMode;
#ifdef FLEXRAN
    uint8_t         ackNackBundlingScrambSeqIndex;
#endif
/*SPR 21001 Changes start */
      /*^ M, 0 ^*/
    STRUC_ONE_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/

} FAPI_ulSCHHarqInfo_st;
#ifdef FAPI_4_0_COMPLIANCE
/* ULSCH_CQI_RI PDU */
typedef struct FAPI_ulSCHCqiRiPduInfoR9_stT
{
    FAPI_ulSCHPduInfo_st           ulSCHPduInfo;
      /*^ M, 0 ^*/
    FAPI_initialTxParam_st         initialTxParamInfo;
      /*^ M, 0 ^*/
    FAPI_cqiRiPduInfoRel9AndLater_stT cqiRiInfo;
      /*^ M, 0 ^*/

}FAPI_ulSCHCqiRiPduInfoR9_st; /*^ API_LIKE,FAPI_ulSCHCqiRiPduInfoR9_st ^*/


/* ---------------------------------- */
/* ULSCH_CQI_HARQ_RI PDU */
typedef struct FAPI_ulSCHCqiHarqRIPduInfoR9_stT
{
    FAPI_ulSCHPduInfo_st      ulSCHPduInfo;
      /*^ M, 0 ^*/

    FAPI_ulSCHHarqInfo_st      harqInfo;
      /*^ M, 0 ^*/

    FAPI_initialTxParam_st    initialTxParamInfo;
      /*^ M, 0 ^*/
    FAPI_cqiRiPduInfoRel9AndLater_stT cqiRiInfo;
      /*^ M, 0 ^*/

}FAPI_ulSCHCqiHarqRIPduInfoR9_st; /*^ API_LIKE, FAPI_ulSCHCqiHarqRIPduInfoR9_st ^*/
 
#endif

/* ---------------------------------- */
/* ULSCH_CQI_HARQ_RI PDU */

typedef struct FAPI_ulSCHCqiHarqRIPduInfo_stT
{
       FAPI_ulSCHPduInfo_st      ulSCHPduInfo;
       /*^ M, 0 ^*/

       FAPI_cqiRiPduInfo_st      cqiRiInfo;
       /*^ M, 0 ^*/

       FAPI_ulSCHHarqInfo_st      harqInfo;
       /*^ M, 0 ^*/

       FAPI_initialTxParam_st    initialTxParamInfo;
       /*^ M, 0 ^*/

}FAPI_ulSCHCqiHarqRIPduInfo_st; /*^ API_LIKE, FAPI_ulSCHCqiHarqRIPduInfo_st ^*/

/* ULSCH_CQI_HARQ_RI PDU Ends */
/* ---------------------------------- */
    
/* ---------------------------------- */
/* ULSCH_HARQ PDU */
typedef struct FAPI_ulSCHHarqPduInfo_stT
{
       FAPI_ulSCHPduInfo_st       ulSCHPduInfo;
       /*^ M, 0 ^*/
       
       FAPI_ulSCHHarqInfo_st      harqInfo;
       /*^ M, 0 ^*/

       FAPI_initialTxParam_st     initialTxParamInfo;
       /*^ M, 0 ^*/

}FAPI_ulSCHHarqPduInfo_st; /*^ API_LIKE, FAPI_ulSCHHarqPduInfo_st ^*/

/* ULSCH_HARQ PDU  Ends */
/* ---------------------------------- */

/* ULSCH_CQI_RI PDU */
typedef struct FAPI_ulSCHCqiRiPduInfo_stT
{
      FAPI_ulSCHPduInfo_st           ulSCHPduInfo;
      /*^ M, 0 ^*/

      FAPI_cqiRiPduInfo_st           cqiRiInfo;
      /*^ M, 0 ^*/

      FAPI_initialTxParam_st         initialTxParamInfo;
      /*^ M, 0 ^*/

}FAPI_ulSCHCqiRiPduInfo_st; /*^ API_LIKE,FAPI_ulSCHCqiRiPduInfo_st ^*/

/* ULSCH_CQI_RI PDU Ends */
/* ---------------------------------- */

/* HARQ BUFFER PDU */
#if  (defined(FAPI_1_1_COMPLIANCE) || defined(FAPI_4_0_COMPLIANCE))
typedef struct FAPI_ulSCHHarqBufferReleasePduInfo_stT
{
    uint32_T     handle;
    /*^ M, 0 ^*/
    
    uint16_t     rnti;
    /*^ M, 0 ^*/
    
    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

}FAPI_ulSCHHarqBufferReleasePduInfo_st;

/* HARQ BUFFER PDU Ends*/

/* ---------------------------------- */
#endif

/* UL CONFIG PDU Structure */
typedef struct FAPI_ulPDUConfigInfo_stT 
{
     /* 0: ULSCH,
     1 : ULSCH_CQI_RI,
     2 : ULSCH_HARQ,
     3 : ULSCH_CQI_HARQ_RI,
     4 : UCI_CQI,
     5 : UCI_SR, 
     6 : UCI_HARQ,
     7 : UCI_SR_HARQ,
     8 : UCI_CQI_HARQ,
     9 : UCI_CQI_SR,
     10: UCI_CQI_SR_HARQ,
     11: SRS 
#ifdef IPR_NBIOT
     12: HARQ_BUFFER,
     13: ULSCH_UCI_CSI,
     14: ULSCH_UCI_HARQ,
     15: ULSCH_CSI_UCI_HARQ,
     16: NULSCH 
#endif
     */
     uint8_t  ulConfigPduType; /*Tags are defined in
                                 FAPI_ulConfigPduInfo_en */
     /*^ M, 0, READ ^*/

     /* Size of the PDU control information (in bytes).
     This length value includes the 2 bytes required for the PDU type 
     and PDU size parameters.The length will always be a multiple of 4 bytes. */
     uint8_t  ulConfigPduSize;
     /*^ M, 0, READ ^*/
/*SPR 21001 Changes start */
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/

     /* need to typecase it with appropriate 
     UL CONFIG PDU TYPE (ulConfigPduType)*/
     uint8_t  ulPduConfigInfo[FAPI_VAR_SIZE(x)];
	/*^ M, 0, OCTET_STRING, PDU_TYPE_MAP, ulConfigPduType ^*/
}FAPI_ulPDUConfigInfo_st; /*^ FUNC, global_op_FAPI_ulPDUConfigInfo_update_counter(&offset_counter, ulConfigPduSize) ^*/


#ifdef FAPI_4_0_COMPLIANCE
typedef struct FAPI_release9Param_st
{
    uint16_t   timingAdvanceR9;
      /*^ M, 0 ^*/
    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_release9Param_st;
/* UL DATA PDU Structure */
typedef struct FAPI_ulDataPduIndicationR9_stT
{
    /* The handle received in the ULSCH PDU. */
    uint32_T     handle;
      /*^ M, 0 ^*/

    /* The RNTI Received  in ULSCH PDU. 
Value: 1 . 65535. */
    uint16_t     rnti;
    /*^ M, 0, READ ^*/

    /* Length of PDU in bytes. */
    uint16_t     length;
    /*^ M, 0, READ ^*/

    /* Gives the PDU#i data address offset from the beginning of the 
       'Number of PDUs' field.An offset of 0 indicates a CRC or decoding error. */
    uint16_t     dataOffset;
      /*^ M, 0 ^*/
    /* SNR 
Value: 0-255, representing -64dB to 63.5dB, with 0.5dB step size. */
    uint8_t      ulCqi;
      /*^ M, 0 ^*/
    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
        /* The timing advance measured for this PDU and UE.
           Value: T_A from 0 to 1282   */
        uint16_t     timingAdvance;
      /*^ M, 0 ^*/

    STRUC_TWO_BYTE_PAD2
    /*^ M, 0, OCTET_STRING, PADDING ^*/
        /* Release9 Parameters in ulsch pdu */
        FAPI_release9Param_st release9Param;
      /*^ M, 0 ^*/
/* SPR_18211 start */
}FAPI_ulDataPduIndicationR9_st; /*^ FUNC, global_op_FAPI_ulDataPduIndication_st(rnti, length, &offset_counter) ^*/
/* SPR_18211 stop */


typedef struct FAPI_rxULSCHIndicationR9_stT
{
    /* The SFN/SF of the SUBFRAME this information was received in.
       A 16-bit value where,[15:4] SFN, range 0 . 1023[3:0] SF, range 0 . 9 */
    uint16_t               sfnsf;
      /*^ M, 0, READ, SFSFN ^*/

    /* Number of PDUs included in this message. */
    uint16_t               numOfPdu;
      /*^ M, 0, READ ^*/

    /* For (Number of PDUs) { */
    FAPI_ulDataPduIndicationR9_st  ulDataPduInfo[FAPI_VAR_SIZE(numOfPdu)];
     /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfPdu ^*/
    /* } */
    /* Contents of PDU#1...PDU#n These will be MAC PDUs. */
    uint8_t                pduBuffer[FAPI_VAR_SIZE(x)];
     /*^ M, 0, NOT_PRESENT_IN_MESSAGE  ^*/
/* SPR_18211 start */
}FAPI_rxULSCHIndicationR9_st;/*^ API, PHY_UL_RX_ULSCH_INDICATION, MAC_INFO_FUNC, global_op_FAPI_rxULSCHIndication_st(pinfo, sf, &offset_counter, subtree, tvb, offset, &local_ptr_to_currently_added_item)   ^*/
/* SPR_18211 stop */
#endif


/* ---------------------------------- */

/* UL DATA PDU Structure */
typedef struct FAPI_ulDataPduIndication_stT
{
    /* The handle received in the ULSCH PDU. */
    uint32_T     handle;
    /*^ M, 0 ^*/

    /* The RNTI passed to the PHY in a DL_CONFIG.request ULSCH PDU 
    Value: 1 . 65535. */
    uint16_t     rnti;
    /*^ M, 0, READ ^*/

    /* Length of PDU in bytes. */
    uint16_t     length;
    /*^ M, 0, READ ^*/
    
    /* Gives the PDU#i data address offset from the beginning of the 
    'Number of PDUs' field.An offset of 0 indicates a CRC or decoding error. */
    uint16_t     dataOffset;
    /*^ M, 0 ^*/
#if  defined(FLEXRAN)
    /* SNR 
    Value: 0-255, representing -64dB to 63.5dB, with 0.5dB step size. */
    uint8_t      ulCqi;
    /*^ M, 0 ^*/
    /* The timing advance measured for this PDU and UE.
    Value: T_A from 0 to 1282   */
    uint16_t     timingAdvance;
    /*^ M, 0 ^*/
    /* The timing advance measured for this PDU and UE.
    Value: T_A from 0 to 1282   */
    uint16_t     timingAdvanceR9;
    /*^ M, 0 ^*/
#else
    /* The timing advance measured for this PDU and UE.
    Value: T_A from 0 to 1282   */
    uint16_t     timingAdvance;
    /*^ M, 0 ^*/

/*SPR 21001 Changes start */
    /* SNR 
    Value: 0-255, representing -64dB to 63.5dB, with 0.5dB step size. */
    uint8_t      ulCqi;
    /*^ M, 0 ^*/

    STRUC_THREE_BYTE_PAD1
/*SPR 21001 Changes end */
#endif /*QC_INT*/
    /*^ M, 0, OCTET_STRING, PADDING ^*/

}FAPI_ulDataPduIndication_st;  /*^ FUNC, global_op_FAPI_ulDataPduIndication_st(rnti, length) ^*/
/*FAPI2_1 changes start*/

//#ifdef FAPI_4_0_COMPLIANCE
#if 1
typedef struct FAPI_fddHarqR9params_st
{
    /* The format of the ACK/NACK response expected. 
       0 = Format 1a/1b
       1 = Channel Selection, added in Release 10
       2 = Format 3, added in Release 10 */
#define MAX_MODE_FDDHARQ_REL9  2
    uint8_t    mode;
      /*^ M, 0 ^*/

    /* The number of ACK/NACK results reported for this UE.   
       Value 1..2 for Format 1a/1b
       Value 1..4 for Channel Selection
       Value 1..10 for Format 3 */
#define MAX_NUMACKNACK_FDDHARQ_REL9 10
/* + Coverity 72757 */
/* There can be maximum 4 HARQ status in FDD when 2 DL CA carriers are used */
#define MAX_NUMACKNACK_FDDHARQ_REL9_SUPPORTED 4
/* - Coverity 72757 */
    uint8_t     numOfAckNack;
      /*^ M, 0,READ ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

        /* HARQ feedback of Nth TB.
           Range 1 . 7
           1 = ACK
           2 = NACK
           3 = ACK or NACK
           4 = DTX
           5 = ACK or DTX
           6 = NACK or DTX
           7 = ACK or NACK or DTX */
        uint8_t      HARQTB[FAPI_VAR_SIZE(numOfAckNack)];
      /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfAckNack ^*/
}FAPI_fddHarqR9params_st;

typedef struct FAPI_fddHarqPduIndicationR9_stT
{
    /* The handle received in the ULSCH PDU or UCI PDU */
    uint32_T handle;
      /*^ M, 0 ^*/

    /* The RNTI passed to the PHY in an uplink subframe configuration PDU. 
    Value: 1 . 65535.  */
    uint16_t   rnti;
      /*^ M, 0 ^*/
    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
        /*HARQ Information for FDD Release 9and 10*/ 
    FAPI_fddHarqR9params_st release9Param;
      /*^ M, 0 ^*/

}FAPI_fddHarqPduIndicationR9_st;

typedef struct FAPI_fddHarqR8params_stT
{
    /* HARQ feedback of 1st TB.
       Range 1 . 7
       1 = ACK
       2 = NACK
       3 = ACK or NACK
       4 = DTX
       5 = ACK or DTX
       6 = NACK or DTX
       7 = ACK or NACK or DTX   */
    uint8_t    harqTB1;
      /*^ M, 0 ^*/

    /* HARQ feedback of 2nd TB. 
       Range 1 . 7
       1 = ACK
       2 = NACK
       3 = ACK or NACK
       4 = DTX
       5 = ACK or DTX
       6 = NACK or DTX
       7 = ACK or NACK or DTX   */
    uint8_t    harqTB2;
      /*^ M, 0 ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_fddHarqR8params_st;

typedef struct FAPI_fddHarqPduIndicationR8_stT
{
    /* The handle received in the ULSCH PDU or UCI PDU */
    uint32_T handle;
      /*^ M, 0 ^*/

    /* The RNTI passed to the PHY in an uplink subframe configuration PDU. 
Value: 1 . 65535. */
    uint16_t   rnti;
      /*^ M, 0 ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

    FAPI_fddHarqR8params_st release8Param;
      /*^ M, 0 ^*/

}FAPI_fddHarqPduIndicationR8_st;

typedef struct FAPI_tddHarqR8params_stT
{

#define MAX_NUMACKNACK_TDDHARQ_REL8  9
    uint8_t     numOfAckNack;
      /*^ M, 0 ^*/
    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

        /* The format of the data is dependent on the HARQ mode; 
           BUNDLING, MULTIPLEXING, or SPECIAL BUNDLING.  
           need to typecast it with appropriate TTD Harq structures
           defined above */
    uint8_t     harqBuffer[FAPI_VAR_SIZE(1)];
      /*^ M, 0, OCTET_STRING,PDU_TYPE_MAP,mode ^*/

}FAPI_tddHarqR8params_st;

typedef struct FAPI_tddHarqPduIndicationR8_stT
{
    /* The handle received in the ULSCH PDU or UCI PDU */
    uint32_T handle;
      /*^ M, 0 ^*/

    /* The RNTI passed to the PHY in an uplink subframe configuration PDU. 
Value: 1 . 65535. */
    uint16_t   rnti;
      /*^ M, 0 ^*/

#define MAX_MODE_TDDHARQ_REL8  2
    uint8_t     mode;
      /*^ GLOBAL ^*/
    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

    FAPI_tddHarqR8params_st release8Param;
      /*^ M, 0 ^*/

}FAPI_tddHarqPduIndicationR8_st;


typedef struct FAPI_tddHarqR9params_stT
{

#define MAX_NUMACKNACK_TDDHARQ_REL9  20
	/* + SPR 13880 Changes */
    uint8_t     numOfAckNack;
    /*^ M, 0, READ, FUNC, global_function_update_numOfAckNack(mode,numOfAckNack)^*/
	/* - SPR 13880 Changes */
    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

        /* The format of the data is dependent on the HARQ mode; 
           BUNDLING, MULTIPLEXING, or SPECIAL BUNDLING.  
           need to typecast it with appropriate TTD Harq structures
           defined above */
	/* + SPR 13880 Changes */
    uint8_t     harqBuffer[FAPI_VAR_SIZE(numOfAckNack)];
	  /*^ FUNC, global_function_fill_harqBuffer(tvb,pinfo,subtree,offset + offset_counter,&offset_counter,&local_ptr_to_currently_added_item,mode) ^*/
	/* - SPR 13880 Changes */

}FAPI_tddHarqR9params_st;

typedef struct FAPI_tddChannelSelectionHarqInfo_stT
{
    /* Number of ACK among multiple ACK/NACK responses 
       0 = 0 or None (UE detect at least one DL assignment is missed)
       1 = 1 or 4 or 7 ACKs reported
       2 = 2 or 5 or 8 ACKs reported
       3 = 3 or 6 or 9 ACKs reported
       4 = DTX (UE did not transmit anything)  
       5 = ACK or DTX
       6 = NACK or DTX
       7 = ACK or NACK or DTX*/
    uint8_t  value0;
      /*^ M, 0 ^*/

    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
/* + SPR 13880 Changes */
}FAPI_tddChannelSelectionHarqInfo_st; /*^ API_LIKE, FAPI_tddChannelSelectionHarqInfo_st ^*/
/* - SPR 13880 Changes */

/* SPR 19679: TDD HARQ Multiplexing Changes Start */
typedef struct FAPI_tddMultiplexingHarqInfoR9_stT
{
    /* indicates HARQ results.
    Range 1. 7
    1 = ACK
    2 = NACK
    3 = ACK or NACK
    4 = DTX
    5 = ACK or DTX
    6 = NACK or DTX
    7 = ACK or NACK or DTX    */

    uint8_t  value0;
      /*^ M, 0 ^*/

    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_tddMultiplexingHarqInfoR9_st; /*^ API_LIKE, FAPI_tddMultiplexingHarqInfoR9_st ^*/

/* SPR 19679: TDD HARQ Multiplexing Changes End */

typedef struct FAPI_tddHarqPduIndicationR9_stT
{
    /* The handle received in the ULSCH PDU or UCI PDU */
    uint32_T handle;
      /*^ M, 0 ^*/

    /* The RNTI passed to the PHY in an uplink subframe configuration PDU. 
Value: 1 . 65535. */
    uint16_t   rnti;
      /*^ M, 0 ^*/
#define MAX_MODE_TDDHARQ_REL9  4
/* + SPR 13880 Changes */
    uint8_t     mode;
	/*^ GLOBAL ^*/
/* - SPR 13880 Changes */
    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

    FAPI_tddHarqR9params_st release9Param;
      /*^ M, 0 ^*/

}FAPI_tddHarqPduIndicationR9_st;

/* TDD_FAPI_2.4 changes  */
/* TDD Bundling type harq info structure */ 
typedef struct FAPI_tddBundlingHarqInfoR9_T
{
    /* Indicates HARQ results. 
    Range 1 . 7
    1 = ACK
    2 = NACK
    3 = ACK or NACK
    4 = DTX
    5 = ACK or DTX
    6 = NACK or DTX
    7 = ACK or NACK or DTX    */
    uint8_t  value0;
    /*^ M, 0 ^*/

    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

}FAPI_tddBundlingHarqInfoR9_st; /*^ API_LIKE, FAPI_tddBundlingHarqInfoR9_st ^*/

/* ---------------------------------- */
/* TDD_FAPI_2.4 changes  */

#endif
/*FAPI2_1 changes end*/


/* ---------------------------------- */
/*SPR 21001 Changes start */
/* FDD HARQ PDU Info Structure */
typedef struct FAPI_fddHarqPduIndication_stT
{
#if  (defined(FAPI_1_1_COMPLIANCE) || defined(FAPI_4_0_COMPLIANCE))  
    uint32_T handle;
    /*^ M, 0 ^*/
#endif    
    /* The RNTI passed to the PHY in an uplink subframe configuration PDU. 
    Value: 1 . 65535.  */
    uint16_t   rnti;
    /*^ M, 0 ^*/

    /* HARQ feedback of 1st TB.
    Range 1 . 7
    1 = ACK
    2 = NACK
    3 = ACK or NACK
    4 = DTX
    5 = ACK or DTX
    6 = NACK or DTX
    7 = ACK or NACK or DTX   */
    uint8_t    harqTB1;
    /*^ M, 0 ^*/
    
    /* HARQ feedback of 2nd TB. 
    Range 1 . 7
    1 = ACK
    2 = NACK
    3 = ACK or NACK
    4 = DTX
    5 = ACK or DTX
    6 = NACK or DTX
    7 = ACK or NACK or DTX   */
    uint8_t    harqTB2;
    /*^ M, 0 ^*/

}FAPI_fddHarqPduIndication_st;
/* ---------------------------------- */

/* TDD Bundling type harq info structure */ 
typedef struct FAPI_tddBundlingHarqInfoT
{
    /* Indicates HARQ results. 
    Range 1 . 7
    1 = ACK
    2 = NACK
    3 = ACK or NACK
    4 = DTX
    5 = ACK or DTX
    6 = NACK or DTX
    7 = ACK or NACK or DTX    */
    uint8_t  value0;
    /*^ M, 0 ^*/

    /* Indicates HARQ results. Range 1 . 7 same as above */
    uint8_t  value1;
    /*^ M, 0 ^*/
/*SPR 21001 Changes start */
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/

}FAPI_tddBundlingHarqInfo_st; /*^ API_LIKE, FAPI_tddBundlingHarqInfo_st ^*/

/* ---------------------------------- */

/* TDD multiplexing type harq info structure */ 
typedef struct FAPI_tddMultiplexingHarqInfo_stT
{
    /* Indicates HARQ results. 
    Range 1 . 7
    1 = ACK
    2 = NACK
    3 = ACK or NACK
    4 = DTX
    5 = ACK or DTX
    6 = NACK or DTX
    7 = ACK or NACK or DTX    */
    uint8_t  value0;
    /*^ M, 0 ^*/

    /* Indicates HARQ results. Range 1 . 7 same as above */
    uint8_t  value1;
    /*^ M, 0 ^*/

    /* Indicates HARQ results. Range 1 . 7 same as above */
    uint8_t  value2;
    /*^ M, 0 ^*/

    /* Indicates HARQ results. Range 1 . 7 same as above */
    uint8_t  value3;
    /*^ M, 0 ^*/

}FAPI_tddMultiplexingHarqInfo_st;  /*^ API_LIKE, FAPI_tddMultiplexingHarqInfo_st  ^*/

/* ---------------------------------- */

/* TDD Special type harq info structure */ 
typedef struct FAPI_tddSpecialBundlingHarqInfo_stT
{
    /* Number of ACK among multiple ACK/NACK responses 
    0 = 0 or None (UE detect at least one DL assignment is missed)
    1 = 1 or 4 or 7 ACKs reported
    2 = 2 or 5 or 8 ACKs reported
    3 = 3 or 6 or 9 ACKs reported
    4 = DTX (UE did not transmit anything)  */
    uint8_t  value0;
    /*^ M, 0 ^*/
/*SPR 21001 Changes start */
    STRUC_THREE_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_tddSpecialBundlingHarqInfo_st; /*^ API_LIKE, FAPI_tddSpecialBundlingHarqInfo_st ^*/

/* ---------------------------------- */

/* TDD HARQ PDU Info Structure */
typedef struct FAPI_tddHarqPduIndication_stT
{
    /* The handle received in the ULSCH PDU or UCI PDU. */
    uint32_T    handle;
    /*^ M, 0 ^*/

    /* The RNTI passed to the PHY in an uplink subframe configuration PDU. 
    Value: 1 . 65535.  */
    uint16_t    rnti;
    /*^ M, 0 ^*/

    /* The format of the ACK/NACK response expected. The bundling and 
    multiplexing options are passed to the PHY in an uplink subframe 
    configuration PDU. If the ACK/NACK is combined with either CQI or SR 
    information then a special ACK/NACK encoding is used which reports 
    the number of ACKs, rather than providing specific ACK/NACK values. 
    This is identified separately and called SPECIAL_BUNDLING in this API. 
    0 = BUNDLING
    1 = MULTIPLEXING   */
    uint8_t     mode;
    /*^ M, 0, READ ^*/

    /* The number of ACK/NACK results reported for this UE.
    Value: 1 . 4 
    For Special Bundling this is the expected number of ACK/NACK 
    responses (UDAI + NSPS).
    Value: 0 . 9  */
    uint8_t     numOfAckNack;
    /*^ M, 0 ^*/
#ifdef FLEXRAN
    uint8_t     rssi;
    uint8_t     sinr;
#endif
    /* The format of the data is dependent on the HARQ mode; 
    BUNDLING, MULTIPLEXING, or SPECIAL BUNDLING.  
    need to typecast it with appropriate TTD Harq structures
    defined above */
    /* SPR 8543 Fix Start */
    #define MAX_HARQ_BUFFER_SIZE 8
    //uint8_t     harqBuffer[MAX_HARQ_BUFFER_SIZE];
	uint8_t     harqBuffer[FAPI_VAR_SIZE(numOfAckNack)];
     /*^ M, 0, OCTET_STRING, PDU_TYPE_MAP, mode ^*/
    /* SPR 8543 Fix End */

}FAPI_tddHarqPduIndication_st;

/* ---------------------------------- */

/* CRC PDU Structure */
typedef struct FAPI_crcPduIndication_stT
{
    /* The handle received in the ULSCH PDU */
    uint32_T    handle;
    /*^ M, 0 ^*/

    /* The RNTI passed to the PHY in an uplink subframe configuration PDU. 
    Value: 1 . 65535.  */
    uint16_t    rnti;
    /*^ M, 0 ^*/

    /* A flag indicating if a CRC error was detected.
    0: CRC_CORRECT
    1:CRC_ERROR   */
    uint8_t     crcFlag;
    /*^ M, 0 ^*/
#ifdef FLEXRAN
    uint8_t     rssi;           
    uint8_t     effective_snr; 
#endif

/*SPR 21001 Changes start */
    STRUC_ONE_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_crcPduIndication_st;

#ifdef FLEXRAN
typedef struct FAPI_crcRbIndication_stT
{
    uint8_t     received_interference_power;
    uint8_t     rssi;  
} __attribute__((__packed__)) FAPI_crcRbIndication_st;
#endif



/* ---------------------------------- */

/* SR PDU Structure */
typedef struct FAPI_srPduIndication_stT
{
    /* The handle received in the ULSCH PDU */
    uint32_T    handle;
    /*^ M, 0 ^*/

    /* The RNTI passed to the PHY in an uplink subframe configuration PDU. 
    Value: 1 . 65535.  */
    uint16_t    rnti;
    /*^ M, 0 ^*/
#ifdef FLEXRAN
    uint8_t     pucch_rssi; 
    uint8_t     pucch_sinr;
#endif

/*SPR 21001 Changes start */
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_srPduIndication_st;
#ifdef FAPI_4_0_COMPLIANCE
typedef struct FAPI_cqiRiInfoR8_stT
{

    /* SNR 
Value: 0-255, representing -64dB to 63.5dB, 
with 0.5dB step size.   */
    uint8_t    ulCqi;
      /*^ M, 0 ^*/

    /* The rank indication reported by the UE on PUSCH
Value: 0..4
0 = RI not received
1..4 = RI value    */
    uint8_t   ri;
      /*^ M, 0 ^*/

    /* The timing advance measured for this PDU and UE.
Value: T_A from 0 to 1282   */
    uint16_t  timingAdvance;
      /*^ M, 0 ^*/

}FAPI_cqiRiInfoR8_st;
typedef struct FAPI_cqiRiInfoR9_stT
{

    /* SNR 
Value: 0-255, representing -64dB to 63.5dB, 
with 0.5dB step size.   */
    uint8_t    ulCqi;
      /*^ M, 0 ^*/

    /*Value 1-5*/
    uint8_t    numberOfCCReported;
      /*^ M, 0, READ ^*/


    /* The timing advance measured for this PDU and UE.
Value: T_A from 0 to 1282   */
    uint16_t  timingAdvance;
      /*^ M, 0 ^*/
    /*Timing advance used for positioning
     * Value: 
     * 0 . 2047 (measured in steps of 2Ts)
     * 2048 . 7690 (measured in steps of 8Ts)
     * */
    uint16_t  timingAdvanceR9;
      /*^ M, 0 ^*/
    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

    /* The rank indication reported by the UE on PUSCH for aperiodic CSI.
       Value: 0..8
       0 = RI not received
       1.. 8 = RI value in Release 10
       1..3 = RI value in Release 9 */
    uint8_t   ri[FAPI_VAR_SIZE(numberOfCCReported)];
      /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numberOfCCReported  ^*/

}FAPI_cqiRiInfoR9_st; /*^ FUNC, global_function_update_offset_counter(numberOfCCReported, &offset_counter) ^*/

/* CQI Indication structure */
typedef struct FAPI_cqiPduIndicationR9_stT
{
    /* The handle received in the ULSCH PDU or UCI PDU.*/
    uint32_T    handle;
      /*^ M, 0 ^*/

    /* The RNTI identifying the UE. For semi-static information held in the MAC
       this will be the value passed to the PHY in a UL_CONFIG.request CQI PDU.  
Value: 1 . 65535.  */
    uint16_t    rnti;
      /*^ M, 0 ^*/

    /* Length of PDU in bytes. */
    uint16_t    length;
      /*^ M, 0, READ ^*/

    /* Gives the PDU#i data address offset from the beginning of the 
       'Number of PDUs' field. An offset of 0 indicates a CRC or decoding error, 
       or only RI received on PUSCH. */
    uint16_t    dataOffset;
      /*^ M, 0 ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

    FAPI_cqiRiInfoR9_st cqiRiInformation;
      /*^ M, 0 ^*/
}FAPI_cqiPduIndicationR9_st; /*^ FUNC, global_op_FAPI_cqiPduIndication_st(length) ^*/

/* CQI Indication structure */
typedef struct FAPI_cqiPduIndication_stT
{
    /* The handle received in the ULSCH PDU or UCI PDU.*/
    uint32_T    handle;
      /*^ M, 0 ^*/

    /* The RNTI identifying the UE. For semi-static information held in the MAC
       this will be the value passed to the PHY in a UL_CONFIG.request CQI PDU.  
Value: 1 . 65535.  */
    uint16_t    rnti;
      /*^ M, 0 ^*/

    /* Length of PDU in bytes. */
    uint16_t    length;
      /*^ M, 0 ^*/

    /* Gives the PDU#i data address offset from the beginning of the 
       'Number of PDUs' field. An offset of 0 indicates a CRC or decoding error, 
       or only RI received on PUSCH. */
    uint16_t    dataOffset;
      /*^ M, 0 ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

    FAPI_cqiRiInfoR8_st cqiRiInformation;
      /*^ M, 0 ^*/
}FAPI_cqiPduIndication_st;

/* RACH PDU Structure */
typedef struct FAPI_rachPduIndicationR9_stT
{
    /* The RA-RNTI value. 
Value: 1 . 65535.  */
    uint16_t    rnti;
      /*^ M, 0 ^*/
    /* The detected preamble
Value: 0 . 63  */
    uint8_t     preamble;
      /*^ M, 0 ^*/

    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
        /* The measured timing advance for the preamble. 
           Value: 0 . 1282   */
    uint16_t    timingAdvance;
      /*^ M, 0 ^*/

    STRUC_TWO_BYTE_PAD2
    /*^ M, 0, OCTET_STRING, PADDING ^*/
        FAPI_release9Param_st release9Param;
      /*^ M, 0 ^*/
}FAPI_rachPduIndicationR9_st;


#else

/* ---------------------------------- */

/* CQI Indication structure */
typedef struct FAPI_cqiPduIndication_stT
{
    /* The handle received in the ULSCH PDU or UCI PDU.*/
    uint32_T    handle;
    /*^ M, 0 ^*/

    /* The RNTI identifying the UE. For semi-static information held in the MAC
       this will be the value passed to the PHY in a UL_CONFIG.request CQI PDU.  
Value: 1 . 65535.  */
    uint16_t    rnti;
    /*^ M, 0 ^*/

    /* Length of PDU in bytes. */
    uint16_t    length;
    /*^ M, 0, READ ^*/
   
    /* Gives the PDU#i data address offset from the beginning of the 
       'Number of PDUs' field. An offset of 0 indicates a CRC or decoding error, 
       or only RI received on PUSCH. */
    uint16_t    dataOffset;
    /*^ M, 0 ^*/

    /* The timing advance measured for this PDU and UE.
Value: T_A from 0 to 1282   */
#ifndef FLEXRAN
/*SPR 21001 Changes start */
    uint16_t  timingAdvance;
/*SPR 21001 Changes end */
#endif
    /*^ M, 0 ^*/

    /* SNR 
Value: 0-255, representing -64dB to 63.5dB, 
with 0.5dB step size.   */
    uint8_t    ulCqi;
    /*^ M, 0 ^*/
#if  defined(FLEXRAN)
/*SPR 21001 Changes start */
    uint8_t   numberOfCCReported;
    uint8_t   ri[FAPI_VAR_SIZE(numberOfCCReported)];
    uint16_t  timingAdvance;
#ifdef FLEXRAN
    uint16_t  timingAdvance_ts;
#endif
/*SPR 21001 Changes end */
#endif
    /* The rank indication reported by the UE on PUSCH
Value: 0..4
0 = RI not received
1..4 = RI value    */
#ifndef FLEXRAN
/*SPR 21001 Changes start */
    uint8_t   ri;
    /*^ M, 0 ^*/
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
#endif
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_cqiPduIndication_st; /*^ FUNC, global_op_FAPI_cqiPduIndication_st(length) ^*/

#endif
/* ---------------------------------- */
#ifdef FAPI_4_0_COMPLIANCE
typedef struct FAPI_rachIndicationR9_stT
{
    /* The SFN/SF of the SUBFRAME this information was received in.
     *     A 16-bit  value where,[15:4] SFN, range 0 . 1023 [3:0] SF, range
     *     0 . 9 */
    uint16_t              sfnsf;
      /*^ M, 0, SFSFN ^*/
    /* Number of RACH preambles */
    uint16_t              numOfPreamble;
      /*^ M, 0, READ ^*/

    /* For (Number of Preambles) */
    FAPI_rachPduIndicationR9_st   rachPduInfo[FAPI_VAR_SIZE(numOfPreamble)];
      /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfPreamble ^*/

}FAPI_rachIndicationR9_st;  /*^ API, PHY_UL_RACH_INDICATION ^*/
#endif

/* ---------------------------------- */

/* RACH PDU Structure */
typedef struct FAPI_rachPduIndication_stT
{
    /* The RA-RNTI value. 
    Value: 1 . 65535.  */
    uint16_t    rnti;
    /*^ M, 0 ^*/
#if  defined(FLEXRAN)
/*SPR 21001 Changes start */
    uint8_t     preamble;
/*SPR 21001 Changes end */
#endif
    /* The measured timing advance for the preamble. 
    Value: 0 . 1282   */
    uint16_t    timingAdvance;
    /*^ M, 0 ^*/
#ifdef FLEXRAN
    uint16_t    timingAdvance_ts;

    uint8_t     rssi;
#else
/*SPR 21001 Changes start */
    /* The detected preamble
    Value: 0 . 63  */
    uint8_t     preamble;
    /*^ M, 0 ^*/

    STRUC_THREE_BYTE_PAD1
/*SPR 21001 Changes end */
#endif
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_rachPduIndication_st;

#ifdef FAPI_4_0_COMPLIANCE
/* SRS PDU Structure */
typedef struct FAPI_srsPduIndicationR9_stT
{
    /* The handle received in the SRS PDU. */
    uint32_T    handle;
      /*^ M, 0 ^*/

    /* The RNTI passed to the PHY in an uplink subframe configuration PDU */
    uint16_t    rnti;
      /*^ M, 0 ^*/

    /* FFS.  Values: 0 . 255, */
    uint16_t    dopplerEstimation; 
      /*^ M, 0 ^*/

    /* The timing advance measured for the UE.
Value: T_A from 0 to 1282 */
    uint16_t    timingAdvance;
      /*^ M, 0 ^*/

    /* Number of resource blocks to be reported for this UE */
    uint8_t     numOfRB;
      /*^ M, 0, READ^*/

    /* The starting point of the RBs to be reported.  */
    uint8_t     rbStart;
      /*^ M, 0 ^*/
    /*Release9 Parameters of SRS PDU*/
    FAPI_release9Param_st release9Param; 
      /*^ M, 0 ^*/

    /* Field size dependent on configured bandwidth
       SNR for RBs, each RBs report one SNR. 
Value: 0-255, representing -64dB to 63.5dB, with 0.5dB step size..*/
    uint8_t     snr[FAPI_VAR_SIZE(numOfRB)]; 
      /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfRB ^*/


}FAPI_srsPduIndicationR9_st;
#endif


/* ---------------------------------- */

/* SRS PDU Structure */
typedef struct FAPI_srsPduIndication_stT
{
    /* The handle received in the SRS PDU. */
    uint32_T    handle;
    /*^ M, 0 ^*/
    
    /* The RNTI passed to the PHY in an uplink subframe configuration PDU */
    uint16_t    rnti;
    /*^ M, 0 ^*/

    /* FFS.  Values: 0 . 255, */
    uint16_t    dopplerEstimation; 
    /*^ M, 0 ^*/

   /* The timing advance measured for the UE.
   Value: T_A from 0 to 1282 */
    uint16_t    timingAdvance;
    /*^ M, 0 ^*/

    /* Number of resource blocks to be reported for this UE */
    uint8_t     numOfRB;
    /*^ M, 0,READ ^*/

    /* The starting point of the RBs to be reported.  */
    uint8_t     rbStart;
    /*^ M, 0 ^*/

    /* Field size dependent on configured bandwidth
    SNR for RBs, each RBs report one SNR. 
    Value: 0-255, representing -64dB to 63.5dB, with 0.5dB step size..*/
    uint8_t     snr[FAPI_VAR_SIZE(numOfRB)];
    /*^ M, 0, OCTET_STRING, PDU_ARRAY, numOfRB ^*/

#ifdef FLEXRAN
    uint16_t    timingAdvance_ts;
    /*^ M, 0 ^*/
#endif
}FAPI_srsPduIndication_st;	/*^ FUNC, global_op_FAPI_srsPduIndication_st(numOfRB, &offset_counter) ^*/

/* ---------------------------------- */

/* For error codes FAPI_SFN_OUT_OF_SYNC and FAPI_MSG_INVALID_SFN */
typedef struct FAPI_errMsgBody1_stT
{
    /* The SFN/SF value received in the message */
    uint16_t  recvdSfnSf;
    /*^ M, 0 ^*/

    /* The SFN/SF value the PHY was expecting to receive in the message */
    uint16_t  expectedSfnSf;
    /*^ M, 0 ^*/

}FAPI_errMsgBody1_st;

/* if error codes is FAPI_MSG_PDU_ERR */
typedef struct FAPI_errMsgBody2_stT
{
    uint8_t    subErrCode;
/*SPR 21001 Changes end */
    /*^ M, 0 ^*/

    /* Indicates if this error was in a DL subframe configuration or an 
    UL subframe configuration.
    0 = DL, 1 = UL */
    uint8_t    direction;  
    /*^ M, 0 ^*/

    /* The RNTI in the received PDU. If the error occurred in a MCH, or BCH, 
    PDU this value is set to 0  */
    uint16_t   rnti;
    /*^ M, 0 ^*/

    /* The PDU Type  parameter specified in both DL subframe configuration and 
    UL subframe configuration   */
    uint8_t   pduType;
    /*^ M, 0 ^*/

    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
}FAPI_errMsgBody2_st;

/* if error codes is FAPI_MSG_HI_ERR */
typedef struct FAPI_errMsgBody3_stT
{
      uint8_t   subErrCode;
/*SPR 21001 Changes end */
      /*^ M, 0 ^*/

      /* The PHICH RB Index parameters specified in each HI PDU */
      uint8_t   phichLowestulRbIndex;
      /*^ M, 0 ^*/

}FAPI_errMsgBody3_st;

/* if error codes is FAPI_MSG_TX_ERR */
typedef struct FAPI_errMsgBody4_stT
{
      uint8_t   subErrCode;
      /*^ M, 0 ^*/

      /* The PDU index parameter specified for each PDU */
      uint8_t   pduIndex;
      /*^ M, 0 ^*/

}FAPI_errMsgBody4_st;


/* Union of all message body types */
typedef union FAPI_errMsgBody_unT {
    FAPI_errMsgBody1_st msgBody1;
    /*^ TAG, FAPI_SFN_OUT_OF_SYNC, FAPI_MSG_INVALID_SFN ^*/
    
    FAPI_errMsgBody2_st msgBody2;
    /*^ TAG, FAPI_MSG_SUBFRAME_ERR ^*/
    
    FAPI_errMsgBody3_st msgBody3;
    /*^ TAG, FAPI_MSG_HI_ERR ^*/
    
    FAPI_errMsgBody4_st msgBody4;
    /*^ TAG, FAPI_MSG_TX_ERR ^*/
} FAPI_errMsgBody_un;



/* Message definition starts from here */

/*  ------------------------------- */
/* 
 * Every message changed betweem MAC/L3 and PHY shall follow this format
 */
typedef struct FAPI_l1ApiMsg_stT
{
     /* Message type ID */
     FAPI_msgIdType_t     msgId;
     /*^ M, 0 ^*/
    
     /* Length of vendor-specific message body (bytes) */
     uint8_t              lenVendorSpecific; 
     /*^ M, 0, READ ^*/
   
     /* Length of message body (bytes) */
     uint16_t             msgLen;
     /*^ M, 0, READ ^*/

     /* Message body.
      * This shall contain one of the below mentioned structure.
      * Each of the below mentioned structure are specific to
      * different message type */
     uint8_t              msgBody[FAPI_VAR_SIZE(msgLen)];
     /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, msgLen ^*/

     /* Vendor-specific message body */
     uint8_t              vendorMsgBody[FAPI_VAR_SIZE(lenVendorSpecific)];
     /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, lenVendorSpecific ^*/
      
}FAPI_l1ApiMsg_st; /*^ API_LIKE, lte_phy_header ^*/

/*  ------------------------------- */
#ifdef FLEXRAN
#define FAPI_DEBUG_DL_DATA       0x55
#define FAPI_DEBUG_UL_DATA       0x66
#define MAX_LOG_TB_DATA_SIZE    1024
typedef struct fapi_debug_T
{
        uint16_t frameNum;
        uint8_t subframeNum;
        uint8_t reserved;
        uint16_t crnti;
        uint8_t pduIdx;
        uint32_t handle;
        uint8_t reserved_t;
}fapi_debug_t;
#endif

/* + TM7_8 Changes Start */
#define MAX_NUM_ANTENNA 4

/* + E_CID_5.3 */
typedef struct FAPI_REL9TARecievedForUE_stT
{
    /* The RNTI passed to the PHY */
    uint8_t    preamble;
    /*^ M, 0 ^*/
    
    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
   /* The timing advance  measured for the postioning of UE. 
    Value:  0 -  2047 (measured in steps of 2Ts)
            2048 - 7690 (measured in steps of 8Ts) */
    uint16_t     timingAdvanceR9;
    /*^ M, 0 ^*/

}FAPI_REL9TARecievedForUE_st;

typedef struct FAPI_UEMeasurementInRACH_stT
{
    /* Number of UEs Whose reports are reported*/
    uint16_t                    numOfpreambles;
    /*^ M, 0, READ ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
   /*AOA and Rel9 TA information for the UEs */
    FAPI_REL9TARecievedForUE_st    rel9TAInfo[FAPI_VAR_SIZE(numOfpreambles)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfpreambles ^*/
}FAPI_UEMeasurementInRACH_st;

typedef struct FAPI_VendorSpecificForRACH_stT
{
    /*UE specific measurements information Structure*/
    FAPI_UEMeasurementInRACH_st  ueMeasInfoInRACHForUEs;
    /*^ M, 0 ^*/

}FAPI_VendorSpecificForRACHInd_st; /*^ API_LIKE, FAPI_VendorSpecificForRACHInd_st ^*/

/* - E_CID_5.3 */

/* + TM7_8 Changes End */
typedef struct FAPI_AOA_REL9TARecievedForUE_stT
{
    /* The RNTI passed to the PHY */
    uint16_t    rnti;
    /*^ M, 0 ^*/
    
   /* + E_CID_5.3 */ 
   /* The timing advance  measured for the postioning of UE. 
    Value:  0 -  2047 (measured in steps of 2Ts)
            2048 - 7690 (measured in steps of 8Ts) */
    uint16_t     timingAdvanceR9;
    /*^ M, 0 ^*/

    /* AoA info for 4 Antennas */
    uint16_t     AoA[MAX_NUM_ANTENNA];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
   /* - E_CID_5.3 */ 
}FAPI_AOA_REL9TARecievedForUE_st;
#ifdef FAPI_4_0_COMPLIANCE
typedef struct FAPI_AOA_RecievedForUE_stT
{
    /* The RNTI passed to the PHY */
    uint16_t    rnti;
    /*^ M, 0 ^*/
    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
    /* AoA info for 4 Antennas */
    uint16_t     AoA[MAX_NUM_ANTENNA];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* - E_CID_5.3 */ 
}FAPI_AOA_RecievedForUE_st;
typedef struct FAPI_UEMeasurementIndicationR9_stT
{
    /* Number of UEs Whose reports are reported*/
    uint16_t                    numOfUEs;
      /*^ M, 0, READ ^*/
    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
        /*AOA and Rel9 TA information for the UEs */
    FAPI_AOA_RecievedForUE_st    aoaRel9Info[FAPI_VAR_SIZE(numOfUEs)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfUEs ^*/
}FAPI_UEMeasurementIndicationR9_st;
typedef struct FAPI_VendorSpecificR9_stT
{
    /*UE specific measurements information Structure*/
    FAPI_UEMeasurementIndicationR9_st  ueMeasInfoForUEs;
    /*^ M, 0 ^*/

}FAPI_VendorSpecificR9_st; /*^ API_LIKE, FAPI_VendorSpecificR9_st ^*/

#endif
typedef struct FAPI_UEMeasurementIndication_stT
{
    /* Number of UEs Whose reports are reported*/
    uint16_t                    numOfUEs;
    /*^ M, 0,READ ^*/
    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
   /*AOA and Rel9 TA information for the UEs */
    FAPI_AOA_REL9TARecievedForUE_st    aoaRel9TAInfo[FAPI_VAR_SIZE(numOfUEs)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfUEs ^*/
}FAPI_UEMeasurementIndication_st;

typedef struct FAPI_VendorSpecific_stT
{
    /*UE specific measurements information Structure*/
    FAPI_UEMeasurementIndication_st  ueMeasInfoForUEs;
    /*^ M, 0 ^*/

}FAPI_VendorSpecific_st; /*^ API_LIKE, FAPI_VendorSpecific_st ^*/

/* - TM7_8 Changes End */
/* in Param request we need to fill only msgId of struct FAPI_L1Api_Msg_st */
/* The message length in the generic header = 0. */ 

/* PHY_PARAM_REQUEST: PARAM Request*/

/*  ------------------------------- */
/* PHY PARAM RESPONSE: This defines the message body for PHY_PARAM_RESPONSE
 * API */

typedef struct FAPI_paramResponse_stT
{
     FAPI_l1ErrorCodes_en        errCode; /* possble error codes are
                                             defined in FAPI_l1ErrorCodes_en*/
     /*^ M, 0 ^*/
     uint8_t                     numOfTlv;
     /*^ M, 0, READ ^*/
/*+ SPR 9285*/
/* SPR 19288 change start */
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
/* SPR 19288 change end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
/*- SPR 9285*/
     FAPI_paramResponseTLV_st   tlvs[FAPI_VAR_SIZE(numOfTlv)];
     /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfTlv ^*/
     
}FAPI_paramResponse_st;	/*^ API, PHY_PARAM_RESPONSE ^*/

/*  ------------------------------- */


/* PHY CELL CONFIG REQUEST:This defines the message body for
 * PHY_CELL_CONFIG_REQUEST */

typedef struct FAPI_phyCellConfigRequest_stT
{
     uint8_t               numOfTlv;
     /*^ M, 0, READ ^*/
/*SPR 21001 Changes start */
    STRUC_THREE_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
     /* Please refer FAPI document to know mandatory TLVs for IDLE,
      * CONFIGURED, RUNNING state for FDD/TDD */
     FAPI_cellConfig_st        configtlvs[FAPI_VAR_SIZE(numOfTlv)];
     /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfTlv ^*/

}FAPI_phyCellConfigRequest_st;	/*^ API, PHY_CELL_CONFIG_REQUEST ^*/

/* PHY_RECONFIG_CELL_REQ : This defines the message body for 
*  PHY_RECONFIG_CELL_REQUEST 
*/
typedef struct FAPI_phyCellReConfigRequest_stT
{
    uint8_t                 numOfTlv;
    /*^ M, 0, READ ^*/
    STRUC_THREE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
    /* Please refer FAPI document to know reconfig TLVs for 
     * RECONFIG during running state of PHY 
     */
    FAPI_cellReConfig_st    configtlvs[FAPI_VAR_SIZE(numOfTlv)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfTlv ^*/
}FAPI_phyCellReConfigRequest_st;


/*  ------------------------------- */
/* PHY CELL CONFIG RESPONSE:This defines the message body for
 * PHY_CELL_CONFIG_RESPONSE */
typedef struct FAPI_phyCellConfigResp_stT
{
  /* Possible values are defined in FAPI_l1ErrorCodes_en. */
    FAPI_l1ErrorCodes_en    errorCode; 
    /*^ M, 0 ^*/

    /* Number of invalid or unsupported TLVs contained in the message body. */
    uint8_t                 numOfInvalidOrUnsupportedTLV;
    /*^ M, 0, READ ^*/

    /* Number of missing TLVs contained in the message body. 
    If the PHY is in the CONFIGURED state this will always be 0. */
    uint8_t                 numOfMissingTLV;
    /*^ M, 0, READ ^*/
/*+ SPR 9285*/
/* SPR 19288 change start */
/*SPR 21001 Changes start */
    STRUC_ONE_BYTE_PAD1
/*SPR 21001 Changes end */
/* SPR 19288 change end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
/*- SPR 9285*/
    /* A list of invalid or unsupported TLVs . 
    each TLV is presented in its entirety. */
    FAPI_cellConfig_st          listOfTLV[FAPI_VAR_SIZE(numOfInvalidOrUnsupportedTLV)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfInvalidOrUnsupportedTLV ^*/
    
    /* A list of missing TLVs . each TLV is presented in its entirety */
    FAPI_cellConfig_st          listOfMissingTlv[FAPI_VAR_SIZE(numOfTlv)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfMissingTLV ^*/

}FAPI_phyCellConfigResp_st;	/*^ API, PHY_CELL_CONFIG_RESPONSE ^*/

/*  ------------------------------- */
/* START REQUEST:This defines the message body for
 * PHY_START_REQUEST */

/* No message body is defined for START.request.
 * The message length in the generic header = 0 */

/*  ------------------------------- */

/* STOP REQUEST:This defines the message body for
 * PHY_STOP_REQUEST */

/* No message body is defined for STOP.request.
 * The message length in the generic header = 0 */

/*  ------------------------------- */

/* STOP INDICATION:This defines the message body for
 * PHY_STOP_INDICATION */

/* No message body is defined for STOP.indication.
 * The message length in the generic header = 0 */

/*  ------------------------------- */

/* PHY UE CONFIG REQUEST: This defines the message body for
 * PHY_UE_CONFIG_REQUEST.
 * The message is only valid if semi-static configuration is kept in PHY.
 */

typedef struct FAPI_ueConfigRequest_stT
{
     uint8_t                numOfTlv;
     /*^ M, 0, READ ^*/

     FAPI_ueConfig_st       tlvs[FAPI_VAR_SIZE(numOfTlv)];
     /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfTlv ^*/

}FAPI_ueConfigRequest_st;	/*^ API, PHY_UE_CONFIG_REQUEST ^*/

/*  ------------------------------- */

/* PHY UE CONFIG RESPONSE: This defines the message body for
 * PHY_UE_CONFIG_RESPONSE
 * The message is only valid if semi-static configuration is kept in PHY.
 */

typedef struct FAPI_phyUeConfigResp_stT
{
  /* Possible values are defined in FAPI_l1ErrorCodes_en. */
    FAPI_l1ErrorCodes_en    errorCode; 
     /*^ M, 0 ^*/

    /* Number of invalid or unsupported TLVs contained in the message body. */
    uint8_t                 numOfInvalidOrUnsupportedTLV;
     /*^ M, 0,READ ^*/

    /* Number of missing TLVs contained in the message body. 
    If the PHY is in the CONFIGURED state this will always be 0. */
    uint8_t                 numOfMissingTLV;
     /*^ M, 0, READ ^*/

/*+ SPR 9285*/
/* SPR 19288 change start */
    STRUC_ONE_BYTE_PAD1
/* SPR 19288 change end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
/*- SPR 9285*/
    /* A list of invalid or unsupported TLVs . 
    each TLV is presented in its entirety. */
    FAPI_ueConfig_st        listOfTLV[FAPI_VAR_SIZE(numOfInvalidOrUnsupportedTLV)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfInvalidOrUnsupportedTLV ^*/
    
    /* A list of missing TLVs . each TLV is presented in its entirety */
    FAPI_ueConfig_st        listOfMissingTlv[FAPI_VAR_SIZE(numOfTlv)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfMissingTLV ^*/

}FAPI_phyUeConfigResp_st;   /*^ API, PHY_UE_CONFIG_RESPONSE ^*/

#ifdef FAPI_4_0_COMPLIANCE
typedef struct FAPI_ueReleaseRequest_stT
{
    uint8_t                numOfTlv;
     /*^ M, 0, READ ^*/
    FAPI_ueRelease_st      tlvs[FAPI_VAR_SIZE(numOfTlv)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfTlv ^*/

}FAPI_ueReleaseRequest_st; /*^ API, PHY_UE_RELEASE_REQ ^*/

/*  ------------------------------- */

/* PHY UE RELEASE RESPONSE: This defines the message body for
 * PHY_UE_RELEASE_RESPONSE
 * The message is only valid if semi-static configuration is kept in PHY.
 */

typedef struct FAPI_phyUeReleaseResp_stT
{
    /* Possible values are defined in FAPI_l1ErrorCodes_en. */
    FAPI_l1ErrorCodes_en    errorCode; 
     /*^ M, 0 ^*/

    /* Number of invalid or unsupported TLVs contained in the message body. */
    uint8_t                 numOfInvalidOrUnsupportedTLV;
     /*^ M, 0, READ ^*/

    /* Number of missing TLVs contained in the message body. 
       If the PHY is in the CONFIGURED state this will always be 0. */
    uint8_t                 numOfMissingTLV;
     /*^ M, 0, READ ^*/

    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
        /* A list of invalid or unsupported TLVs . 
           each TLV is presented in its entirety. */
        FAPI_ueRelease_st        listOfTLV[FAPI_VAR_SIZE(numOfInvalidOrUnsupportedTLV)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfInvalidOrUnsupportedTLV ^*/

    /* A list of missing TLVs . each TLV is presented in its entirety */
    FAPI_ueRelease_st        listOfMissingTlv[FAPI_VAR_SIZE(numOfTlv)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfMissingTLV ^*/

}FAPI_phyUeReleaseResp_st; /*^ API, PHY_UE_RELEASE_RESP ^*/


#endif


/*  ------------------------------- */

/* PHY Notification : This defines the message body for
 * PHY_ERROR_INDICATION */

typedef struct FAPI_phyErrorIndication_stT
{
    /* Indicate which message received by the PHY has an error */
    FAPI_msgIdType_t        msgId;
    /*^ M, 0 ^*/

/*+ SPR 9285*/
/* SPR 19288 change start */
/*SPR 21001 Changes start */
    STRUC_TWO_BYTE_PAD1
/*SPR 21001 Changes end */
/* SPR 19288 change end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
/*- SPR 9285*/

    FAPI_l1ErrorCodes_en    errorCode;
    /*^ M, 0, TAG_TYPE ^*/

    FAPI_errMsgBody_un      errMsgBody;
    /*^ M, 0, UNION ^*/

}FAPI_phyErrorIndication_st;	/*^ API, PHY_ERROR_INDICATION ^*/

/* if error codes is FAPI_MSG_INVALID_STATE or FAPI_MSG_BCH_MISSING */
/* then errValues are not supported in notification */
/*  ------------------------------- */

/* SUBFRAME indication : This defines the message body for
 * PHY_UL_SUBFRAME_INDICATION */
typedef struct FAPI_subFrameIndication_stT
{
    /* A 16-bit value where,[15:4] SFN, range 0 . 1023[3:0] SF, range 0 . 9 */
    uint16_t   sfnsf;
    /*^ M, 0, SFSFN ^*/

}FAPI_subFrameIndication_st;	/*^ API, PHY_UL_SUBFRAME_INDICATION ^*/

/*  ------------------------------- */

/*****************************************************************************
   Downlink Subframe for following information
    - DL_CONFIG_Request
    - HI_DCI0_CONFIG
    - DL Data : TX_Request
 *****************************************************************************/

/* DL_CONFIG_Request MSG : This defines the message body for
 * PHY_DL_CONFIG_REQUEST */

typedef struct FAPI_dlConfigRequest_stT
{
    /* A 16-bit value where,[15:4] SFN, range 0 . 1023[3:0] SF, range 0 . 9 */ 
    uint16_t                    sfnsf; 
    /*^ M, 0, READ, SFSFN, FUNC, global_op_FAPI_dlConfigRequest_st(sf) ^*/

    /* The length of the downlink subframe configuration. 
    Range 0 . 65535. */
    uint16_t                    length;
    /*^ M, 0 ^*/

    /* The number of OFDM symbols for the PDCCH.
    Value:0 . 4 */
    uint8_t                     cfi;
    /*^ M, 0 ^*/

    /*The number of DCI PDUs included in this message.
    Range: 0 . 255 */
    uint8_t                     numDCI;
    /*^ M, 0 ^*/

    /* Number of PDUs that are included in this message.
    Range 0 . 514 */
    uint16_t                    numOfPDU;
    /*^ M, 0, READ ^*/
#if  !defined(FLEXRAN)
/*SPR 21001 Changes start */
    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps. */
    uint16_t                    txPowerForPCFICH;
    /*^ M, 0 ^*/
/*SPR 21001 Changes end */
#endif

    /* Number of unique RNTIs sent on the PDSCH. 
    - a PCH PDU will have an unique RNTI and should be included in this value
    - a DLSCH PDU can be one transport block sent to a UE with an unique RNTI.
      This RNTI should be included in this value
    - a DLSCH PDU can be one of two transport blocks sent to a UE. In this case
      the two DLSCH PDUs will share the same RNTI. Only one RNTI should be 
      included in this value. */
    uint8_t                     numOfPDSCHRNTI;
    /*^ M, 0 ^*/
#if  defined(FLEXRAN)
/*SPR 21001 Changes start */
    /* Offset to the reference signal power.
    Value: 0 . 10000, representing -6 dB to 4 dB in 0.001 dB steps. */
    uint16_t                    txPowerForPCFICH;
    /*^ M, 0 ^*/
/*SPR 21001 Changes end */
#endif
/*SPR 21001 Changes start */
    STRUC_ONE_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
    /* possible values of PDU Type supported are defined 
     * as macro preprocessor */

    FAPI_dlConfigPDUInfo_st       dlConfigpduInfo[FAPI_VAR_SIZE(numOfPDU)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfPDU ^*/

}FAPI_dlConfigRequest_st;   /*^ API, PHY_DL_CONFIG_REQUEST ^*/

/* + TM7_EPRE */
typedef struct FAPI_PdschEpreToUeRsRatio_stT
{
    uint16_t        rnti;
    /*^ M, 0 ^*/
    /*pdschEpreToUeRsRatio;*/
    int8_t          pdschEpreToUeRsRatio;
     /*^ M, 0 ^*/
    STRUC_ONE_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/

}FAPI_PdschEpreToUeRsRatio_st;


typedef struct FAPI_VendorSpecificForDLConfig_stT
{
    /* Number of UEs Whose reports are reported*/
    uint16_t                    numOfUEs;
    /*^ M, 0, READ ^*/
    STRUC_TWO_BYTE_PAD1
    /*^ M, 0, OCTET_STRING, PADDING ^*/
    
    /* Structure of pdschEpreToUeRsRatio;*/
    FAPI_PdschEpreToUeRsRatio_st  pdschEpreToUeRsRatioInfo[FAPI_VAR_SIZE(numOfUEs)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfUEs ^*/

}FAPI_VendorSpecificForDLConfig_st; /*^ API_LIKE, FAPI_VendorSpecificForDLConfig_st ^*/
/* - TM7_EPRE */


/*  ------------------------------- */

/* HI DCI0 REQUEST : This defines the message body for
 * PHY_DL_HI_DCI0_REQUEST . */

typedef struct FAPI_dlHiDCIPduInfo_stT
{
#ifdef FLEXRAN
    uint16_t                    sfnsf_t;
#endif
    /* The SFN/SF in this message should be the same as the corresponding 
    DL_CONFIG.request message. A 2-byte value where,
    [15:4] SFN, range 0 . 1023
    [3:0] SF, range 0 . 9    */
    uint16_t                    sfnsf;
    /*^ M, 0, SFSFN ^*/

    /* Number of DCI PDUs included in this message */
    uint8_t                     numOfDCI;
    /*^ M, 0, READ ^*/

    /* Number of HI PDUs included in this message */
    uint8_t                     numOfHI;
    /*^ M, 0, READ ^*/

    uint8_t                     hidciPduInfo[FAPI_VAR_SIZE(numOfHI)];
    /*^ M, 0, OCTET_STRING, PDU_TYPE_MAP, numOfHI::numOfDCI ^*/

}FAPI_dlHiDCIPduInfo_st;    /*^ API, PHY_DL_HI_DCI0_REQUEST ^*/

/*  ------------------------------- */

/* DL DATA TX REQUEST : This defines the message body for 
 * PHY_DL_TX_REQUEST .  */

typedef struct FAPI_dlDataTxRequest_stT
{
    /* The SFN/SF in this message should be the same as the corresponding 
    DL_CONFIG.request message.A 2-byte value where,[15:4] SFN, range 0 . 1023
    [3:0] SF, range 0 . 9   */
    uint16_t        sfnsf;
    /*^ M, 0, READ, SFSFN, FUNC, global_function_update_sfsfn(sf) ^*/

    /* Number of PDUs included in this message. */
    uint16_t        numOfPDU;
    /*^ M, 0, READ ^*/

    /* For each PDU */
    FAPI_dlPduInfo_st  dlPduInfo[FAPI_VAR_SIZE(numOfPDU)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfPDU ^*/

}FAPI_dlDataTxRequest_st;    /*^ API, PHY_DL_TX_REQUEST ^*/

/*  ------------------------------- */

/*****************************************************************************
   Uplink Subframe Information
    - UL_CONFIG_Request
    - UL Data
 *****************************************************************************/
/* UL CONFIG REQUEST : This defines the message body for
 * PHY_UL_CONFIG_REQUEST.
 * 
 * The supported PDUs are dependent on whether the semi-static information
 * and uplink HARQ signalling calculation is held in the MAC or PHY.
 * 
 */

typedef struct FAPI_ulConfigRequest_stT
{
    /* A 16-bit value where,[15:4] SFN, range 0 . 1023[3:0] SF, range 0 . 9 */
    uint16_t                     sfnsf;
    /*^ M, 0, SFSFN ^*/

    /* The length of the uplink subframe configuration. 
    Range 0 . 65535. */
    uint16_t                        ulConfigLen;
    /*^ M, 0 ^*/

    /* The number of UL SCHs PDUs included in this message. */
    uint8_t                         numOfPdu;
    /*^ M, 0, READ ^*/

    /* If semi-static information is held in the MAC
    For FDD:
    0: No RACH in this subframe
    1: RACH present in this subframe
    For TDD:
    Bits 0:5 indicate which RACH frequency resources are used in this 
    subframe,
    0: This RACH frequency index is not used
    1: This RACH frequency index is used 
    If semi-static information is held in the PHY this parameter is ignored. */
    uint8_t                         rachFreqResources;
    /*^ M, 0 ^*/

    /* If semi-static information is held in the MAC
    0: No SRS in this subframe
    1: SRS present in this subframe
    If semi-static information is held in the PHY this parameter is ignored. */
    uint8_t                         srsPresent;
    /*^ M, 0 ^*/
/*SPR 21001 Changes start */
    STRUC_ONE_BYTE_PAD1
/*SPR 21001 Changes end */
    /*^ M, 0, OCTET_STRING, PADDING ^*/
   /* For Number of PDUs */
    FAPI_ulPDUConfigInfo_st  ulPduConfigInfo[FAPI_VAR_SIZE(numOfPdu)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfPdu ^*/

}FAPI_ulConfigRequest_st;    /*^ API, PHY_UL_CONFIG_REQUEST ^*/

/*  ------------------------------- */

/* RX ULSCH DATA INDICATION  : This defines the message body for 
 * PHY_UL_RX_ULSCH_INDICATION . */

typedef struct FAPI_rxULSCHIndication_stT
{
     /* The SFN/SF of the SUBFRAME this information was received in.
     A 16-bit value where,[15:4] SFN, range 0 . 1023[3:0] SF, range 0 . 9 */
     uint16_t               sfnsf;
     /*^ M, 0, READ, SFSFN ^*/

     /* Number of PDUs included in this message. */
     uint16_t               numOfPdu;
     /*^ M, 0, READ ^*/

     /* For (Number of PDUs) { */
     FAPI_ulDataPduIndication_st  ulDataPduInfo[FAPI_VAR_SIZE(numOfPdu)];
     /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfPdu ^*/
     /* } */

     /* Contents of PDU#1...PDU#n These will be MAC PDUs. */
     uint8_t                pduBuffer[FAPI_VAR_SIZE(x)];
     /*^ M, 0, NOT_PRESENT_IN_MESSAGE  ^*/
     /* M, 0, OCTET_STRING, PDU_ARRAY, g_total_length */

}FAPI_rxULSCHIndication_st;
#ifndef FAPI_4_0_COMPLIANCE
	/*^ API, PHY_UL_RX_ULSCH_INDICATION, MAC_INFO_FUNC, global_op_FAPI_rxULSCHIndication_st(pinfo, sf, &offset_counter, subtree,
                                                                 tvb, offset)   ^*/
#endif

/*  ------------------------------- */

/* UL HARQ INDICATION  : This defines the message body for   
 * PHY_UL_HARQ_INDICATION . */

typedef struct FAPI_harqIndication_stT
{
    /* The SFN/SF of the SUBFRAME this information was received in.
    A 16-bit  value where,[15:4] SFN, range 0 . 1023 [3:0] SF, range 0 . 9 */ 
    uint16_t             sfnsf;
    /*^ M, 0, SFSFN ^*/

    /* Number of HARQs included in this message. */
    uint16_t             numOfHarq;
    /*^ M, 0, READ ^*/

    /* For (Number of HARQs) depending upon Mode ( TDD or FDD ) */
#ifdef TDD_CONFIG
    FAPI_tddHarqPduIndication_st harqPduInfo[FAPI_VAR_SIZE(numOfHarq)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfHarq , TDD^*/

#endif
#ifdef FDD_CONFIG
    FAPI_fddHarqPduIndication_st harqPduInfo[FAPI_VAR_SIZE(numOfHarq)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfHarq , FDD ^*/
#endif

}FAPI_harqIndication_st;
#ifndef FAPI_4_0_COMPLIANCE
   /*^ API, PHY_UL_HARQ_INDICATION ^*/
#endif
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
/*FAPI2_1 changes start*/
/*  ------------------------------- */

/* UL HARQ INDICATION  : This defines the message body for   
 * PHY_UL_HARQ_INDICATION . */

typedef struct FAPI_harqIndicationR8_stT
{
    /* The SFN/SF of the SUBFRAME this information was received in.
       A 16-bit  value where,[15:4] SFN, range 0 . 1023 [3:0] SF, range 0 . 9 */ 
     uint16_t             sfnsf;
     /*^ M, 0, SFSFN ^*/
    /* Number of HARQs included in this message. */
    uint16_t             numOfHarq;

    /*^ M, 0, READ ^*/
    /* For (Number of HARQs) depending upon Mode ( TDD or FDD ) */
#ifdef TDD_CONFIG
    FAPI_tddHarqPduIndicationR8_st harqPduInfo[FAPI_VAR_SIZE(numOfHarq)];

    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfHarq , TDD^*/
#endif
#ifdef FDD_CONFIG
    FAPI_fddHarqPduIndicationR8_st harqPduInfo[FAPI_VAR_SIZE(numOfHarq)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfHarq, FDD ^*/
#endif

}FAPI_harqIndicationR8_st; /*^ API, PDU_TYPE_MAP ,PHY_UL_HARQ_INDICATION ^*/


/* UL HARQ INDICATION  : This defines the message body for   
 * PHY_UL_HARQ_INDICATION . */

typedef struct FAPI_harqIndicationR9_stT
{
    /* The SFN/SF of the SUBFRAME this information was received in.
       A 16-bit  value where,[15:4] SFN, range 0 . 1023 [3:0] SF, range 0 . 9 */ 
    uint16_t             sfnsf;

     /*^ M, 0, SFSFN ^*/
    /* Number of HARQs included in this message. */
    uint16_t             numOfHarq;

    /*^ M, 0, READ ^*/
    /* For (Number of HARQs) depending upon Mode ( TDD or FDD ) */
#ifdef TDD_CONFIG
    FAPI_tddHarqPduIndicationR9_st harqPduInfo[FAPI_VAR_SIZE(numOfHarq)];

    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfHarq, TDD ^*/
#endif
#ifdef FDD_CONFIG
    FAPI_fddHarqPduIndicationR9_st harqPduInfo[FAPI_VAR_SIZE(numOfHarq)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfHarq, FDD ^*/
#endif

}FAPI_harqIndicationR9_st; /*^ API_LIKE ,FAPI_harqIndicationR9_st ^*/
#endif
/*FAPI2_1 changes end*/

/*  ------------------------------- */

/* UL CRC INDICATION  : This defines the message body for  
 * PHY_UL_CRC_INDICATION . */

typedef struct FAPI_crcIndication_stT
{
    /* The SFN/SF of the SUBFRAME this information was received in.
    A 16-bit  value where,[15:4] SFN, range 0 . 1023 [3:0] SF, range 0 . 9 */ 
    uint16_t            sfnsf;
    /*^ M, 0, SFSFN ^*/

    /* Number of CRCs included in this message. */
    uint16_t            numOfCrc;
    /*^ M, 0, READ ^*/

#ifdef FLEXRAN
    uint8_t             numberOfResourceBlocks;
#endif

    /* For (Number of CRCs) */
    FAPI_crcPduIndication_st  crcPduInfo[FAPI_VAR_SIZE(numOfCrc)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfCrc ^*/
#ifdef FLEXRAN
    FAPI_crcRbIndication_st crcRbInfo[FAPI_VAR_SIZE(numberOfResourceBlocks)];
#endif

}FAPI_crcIndication_st;	  /*^ API, PHY_UL_CRC_INDICATION ^*/

/*  ------------------------------- */

/* UL RX SR INDICATION  : This defines the message body for  
 * PHY_UL_RX_SR_INDICATION . */

typedef struct FAPI_rxSRIndication_stT
{
    /* The SFN/SF of the SUBFRAME this information was received in.
    A 16-bit  value where,[15:4] SFN, range 0 . 1023 [3:0] SF, range 0 . 9 */ 
    uint16_t          sfnsf;
    /*^ M, 0, SFSFN ^*/

    /* Number of SRs included in this message.  */
    uint16_t          numOfSr;
    /*^ M, 0, READ ^*/

    /* For (Number of SRs) */
    FAPI_srPduIndication_st srPduInfo[FAPI_VAR_SIZE(numOfSr)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfSr ^*/

}FAPI_rxSRIndication_st;   /*^ API, PHY_UL_RX_SR_INDICATION ^*/

#ifdef FAPI_4_0_COMPLIANCE
typedef struct FAPI_rxCqiIndicationR9_stT
{
    /* The SFN/SF of the SUBFRAME this information was received in.
       A 16-bit  value where,[15:4] SFN, range 0 . 1023 [3:0] SF, range 0 . 9 */ 
    uint16_t          sfnsf;
    /*^ M, 0, SFSFN ^*/
    /* Number of Cqi included in this message.  */
    uint16_t          numOfCqi;
    /*^ M, 0, READ ^*/
    /* For (Number of Cqis) */
    FAPI_cqiPduIndicationR9_st cqiPduInfo[FAPI_VAR_SIZE(numOfCqi)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER,numOfCqi  ^*/
    /* Contents of PDU#1....PDU#n These will be CQI PDUs. */
    uint8_t                pduBuffer[FAPI_VAR_SIZE(x)];
    /*^ M, 0, OCTET_STRING, PDU_ARRAY, g_total_length ^*/

}FAPI_rxCqiIndicationR9_st; /*^ API, PHY_UL_RX_CQI_INDICATION, FUNC, global_op_FAPI_rxCqiIndication_st(&offset_counter) ^*/

#endif

/*  ------------------------------- */

/* UL RX CQI INDICATION  : This defines the message body for  
 * PHY_UL_RX_CQI_INDICATION . */

typedef struct FAPI_rxCqiIndication_stT
{
    /* The SFN/SF of the SUBFRAME this information was received in.
    A 16-bit  value where,[15:4] SFN, range 0 . 1023 [3:0] SF, range 0 . 9 */ 
    uint16_t          sfnsf;
    /*^ M, 0, SFSFN ^*/

    /* Number of Cqi included in this message.  */
    uint16_t          numOfCqi;
    /*^ M, 0, READ ^*/

    /* For (Number of Cqis) */
    FAPI_cqiPduIndication_st cqiPduInfo[FAPI_VAR_SIZE(numOfCqi)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfCqi ^*/

    /* Contents of PDU#1....PDU#n These will be CQI PDUs. */
    uint8_t                pduBuffer[FAPI_VAR_SIZE(x)];
    /*^ M, 0, OCTET_STRING, PDU_ARRAY, g_total_length ^*/

}FAPI_rxCqiIndication_st; 
#ifndef FAPI_4_0_COMPLIANCE
   /*^ API, PHY_UL_RX_CQI_INDICATION, FUNC, global_op_FAPI_rxCqiIndication_st(&offset_counter) ^*/
#endif
/*  ------------------------------- */

/* UL RACH INDICATION  : This defines the message body for  
 * PHY_UL_RACH_INDICATION . */

typedef struct FAPI_rachIndication_stT
{
    /* The SFN/SF of the SUBFRAME this information was received in.
    A 16-bit  value where,[15:4] SFN, range 0 . 1023 [3:0] SF, range 0 . 9 */ 
    uint16_t              sfnsf;
    /*^ M, 0, SFSFN ^*/
#if  !defined(FLEXRAN)
/*SPR 21001 Changes start */
    /* Number of RACH preambles */
    uint16_t              numOfPreamble;
#else
    uint8_t              numOfPreamble;
/*SPR 21001 Changes end */
#endif
    /*^ M, 0, READ ^*/

    /* For (Number of Preambles) */
    FAPI_rachPduIndication_st   rachPduInfo[FAPI_VAR_SIZE(numOfPreamble)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfPreamble ^*/

}FAPI_rachIndication_st; 
#ifndef FAPI_4_0_COMPLIANCE
  /*^ API, PHY_UL_RACH_INDICATION ^*/
#endif
#ifdef FAPI_4_0_COMPLIANCE

typedef struct FAPI_srsIndicationR9_stT
{
    /* The SFN/SF of the SUBFRAME this information was received in.
       A 16-bit  value where,[15:4] SFN, range 0 . 1023 [3:0] SF, range 0 . 9 */ 
    uint16_t           sfnsf;
    /*^ M, 0, SFSFN ^*/

    /* Number of UEs contributing to the uplink SRS */
    uint16_t           numOfUe;
    /*^ M, 0, READ ^*/

    /* For (Number of UEs) */
    FAPI_srsPduIndicationR9_st srsPduInfo[FAPI_VAR_SIZE(numOfUe)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfUe ^*/

}FAPI_srsIndicationR9_st; /*^ API, PHY_UL_SRS_INDICATION ^*/


#endif


/*  ------------------------------- */

/* UL SRS INDICATION  : This defines the message body for  
 * PHY_UL_SRS_INDICATION . */

typedef struct FAPI_srsIndication_stT
{
    /* The SFN/SF of the SUBFRAME this information was received in.
    A 16-bit  value where,[15:4] SFN, range 0 . 1023 [3:0] SF, range 0 . 9 */ 
    uint16_t           sfnsf;
    /*^ M, 0, SFSFN ^*/
#if !defined(FLEXRAN)
/*SPR 21001 Changes start */
    /* Number of UEs contributing to the uplink SRS */
    uint16_t           numOfUe;
#else
    uint8_t           numOfUe;
/*SPR 21001 Changes end */
#endif
    /*^ M, 0, READ ^*/

    /* For (Number of UEs) */
    FAPI_srsPduIndication_st srsPduInfo[FAPI_VAR_SIZE(numOfUe)];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfUe ^*/

}FAPI_srsIndication_st;	
#ifndef FAPI_4_0_COMPLIANCE
/*^ API, PHY_UL_SRS_INDICATION ^*/
#endif

/* SPR 1308 start*/
/* This structure defined the paramters 
  which received in the PARAM_RESPONSE message*/
typedef struct FAPI_paramRespParam_stT
{
     /* The PHY downlink channel bandwidth capability (in resource blocks). 

     Value: bitX :0 = no support, 1= support.
     Bit0: 6
     Bit1: 15
     Bit2: 25
     Bit3: 50
     Bit4: 75
     Bit5: 100     */
     uint16_t    dlBandWidthSupport;
     /*^ M, 0 ^*/
     
     /* The PHY uplink channel bandwidth capability (in resource blocks). 

     Value: bitX :0 = no support, 1= support.
     Bit0: 6
     Bit1: 15
     Bit2: 25
     Bit3: 50
     Bit4: 75
     Bit5: 100     */
     uint16_t    ulBandWidthSupport;
     /*^ M, 0 ^*/
    
     /* The PHY downlink modulation capability.
     Value: bitX :0 = no support, 1= support.
     Bit0: QPSK
     Bit1: 16QAM
     Bit2: 64QAM */
     uint16_t    dlModulationSupport;
     /*^ M, 0 ^*/

    /* The PHY uplink modulation capability.
    Value: bitX :0 = no support, 1= support.
    Bit0: QPSK
    Bit1: 16QAM
    Bit2: 64QAM    */
    uint16_t    ulModulationSupport;
     /*^ M, 0 ^*/
   
    /* Number of antennas supported.
    Value: 1, 2, 4  */
    uint16_t    phyAntennaCapability;
     /*^ M, 0 ^*/

    /* eMBMS supported.
    Value: 0, 1  */
    uint16_t    phyMbsfnCapability;
     /*^ M, 0 ^*/

}FAPI_paramRespParam_stT;
/* SPR 1308  end */

/* +DYNAMIC_ICIC */
typedef struct FAPI_measReportInd_stT 
{
    uint32_T bitmask;
     /*^ M, 0 ^*/

    int32_T interferencePerPRB[100];
    /*^ M, 0, OCTET_STRING, FIXED ^*/

    int32_T thermalNoise;
     /*^ M, 0 ^*/

    int32_T dlRsTxPower;
     /*^ M, 0 ^*/

}FAPI_measReportInd_st;

typedef struct FAPI_measReportInfo_stT
{
    uint16_t          sfnsf;
     /*^ M, 0, SFSFN ^*/

    /* For (Number of Reports) */
    FAPI_measReportInd_st measReportInfo;
     /*^ M, 0 ^*/

}FAPI_measReportInfo_st; /*^ API, PHY_MEASUREMENT_REPORT_INDICATION ^*/
/* -DYNAMIC_ICIC */

/*^	PDU_TYPE_MAP : dciFormat 
	FAPI_dciFormat1_st	FAPI_DL_DCI_FORMAT_1 ,
	FAPI_dciFormat1A_st	FAPI_DL_DCI_FORMAT_1A ,
	FAPI_dciFormat1B_st	FAPI_DL_DCI_FORMAT_1B ,
	FAPI_dciFormat1C_st	FAPI_DL_DCI_FORMAT_1C ,
	FAPI_dciFormat1D_st	FAPI_DL_DCI_FORMAT_1D ,
	FAPI_dciFormat2_st	FAPI_DL_DCI_FORMAT_2 ,
	FAPI_dciFormat2A_st	FAPI_DL_DCI_FORMAT_2A ,
    FAPI_dciFormat2B_st FAPI_DL_DCI_FORMAT_2B ,
	FAPI_dciFormat6_0A_st	FAPI_UL_DCI_FORMAT_6_0A ,
	FAPI_dciFormat6_0B_st	FAPI_UL_DCI_FORMAT_6_0B ,
    FAPI_dciFormat6_1A_st   FAPI_DL_DCI_FORMAT_6_1A ,
    FAPI_dciFormat6_1B_st	FAPI_DL_DCI_FORMAT_6_1B ,
	FAPI_dciFormat6_2_st	FAPI_DL_DCI_FORMAT_6_2 ^*/

#ifndef FAPI_4_0_COMPLIANCE
/*^	PDU_TYPE_MAP : STRUCT 
	FAPI_VendorSpecificForDLConfig_st PHY_DL_CONFIG_REQUEST , 
    FAPI_VendorSpecific_st            PHY_UL_RX_ULSCH_INDICATION ,
    FAPI_VendorSpecificForRACHInd_st  PHY_UL_RACH_INDICATION ,
    FAPI_VendorSpecific_st            PHY_UL_SRS_INDICATION ,
	FAPI_VendorSpecific_st            PHY_UL_RX_CQI_INDICATION ^*/
#elif FAPI_4_0_COMPLIANCE
/*^	PDU_TYPE_MAP : STRUCT 
	FAPI_VendorSpecificForDLConfig_st PHY_DL_CONFIG_REQUEST , 
    FAPI_VendorSpecificR9_st          PHY_UL_RX_ULSCH_INDICATION ,
    FAPI_VendorSpecificForRACHInd_st  PHY_UL_RACH_INDICATION ,
    FAPI_VendorSpecificR9_st          PHY_UL_SRS_INDICATION ,
	FAPI_VendorSpecificR9_st          PHY_UL_RX_CQI_INDICATION ^*/
#endif

#ifndef FAPI_4_0_COMPLIANCE
/*^	PDU_TYPE_MAP : ulConfigPduType 
	FAPI_ulSCHPduInfo_st          FAPI_ULSCH,
    FAPI_ulSCHCqiRiPduInfo_st     FAPI_ULSCH_CQI_RI,
    FAPI_ulSCHHarqPduInfo_st      FAPI_ULSCH_HARQ,
    FAPI_ulSCHCqiHarqRIPduInfo_st FAPI_ULSCH_CQI_HARQ_RI,
    FAPI_uciCqiPduInfo_st         FAPI_UCI_CQI,
    FAPI_uciSrPduInfo_st          FAPI_UCI_SR, 
    FAPI_uciHarqPduInfo_st        FAPI_UCI_HARQ,
    FAPI_uciSrHarqPduInfo_st      FAPI_UCI_SR_HARQ,
    FAPI_uciCqiHarqPduInfo_st     FAPI_UCI_CQI_HARQ,
    FAPI_uciCqiSrPduInfo_st       FAPI_UCI_CQI_SR,
    FAPI_uciCqiSrHarqPduInfo_st   FAPI_UCI_CQI_SR_HARQ,
    FAPI_srsPduInfo_st            FAPI_SRS ^*/
#elif FAPI_4_0_COMPLIANCE
/*^	PDU_TYPE_MAP : ulConfigPduType 
	FAPI_ulSCHPduInfo_st        FAPI_ULSCH,
    FAPI_ulSCHCqiRiPduInfoR9_st     FAPI_ULSCH_CQI_RI,
    FAPI_ulSCHHarqPduInfo_st        FAPI_ULSCH_HARQ,
    FAPI_ulSCHCqiHarqRIPduInfoR9_st FAPI_ULSCH_CQI_HARQ_RI,
    FAPI_uciCqiPduInfo_st           FAPI_UCI_CQI,
    FAPI_uciSrPduInfo_st            FAPI_UCI_SR, 
    FAPI_uciHarqPduInfoR9_st        FAPI_UCI_HARQ,
    FAPI_uciSrHarqPduInfoR9_st      FAPI_UCI_SR_HARQ,
    FAPI_uciCqiHarqPduInfoR9_st     FAPI_UCI_CQI_HARQ,
    FAPI_uciCqiSrPduInfo_st         FAPI_UCI_CQI_SR,
    FAPI_uciCqiSrHarqPduInfoR9_st   FAPI_UCI_CQI_SR_HARQ,
    FAPI_srsPduInfo_st              FAPI_SRS ^*/
#endif
/* + SPR 13880 Changes */
#ifndef FAPI_4_0_COMPLIANCE
/*^	PDU_TYPE_MAP : mode
    FAPI_tddBundlingHarqInfo_st	       BUNDLING , 
    FAPI_tddMultiplexingHarqInfo_st    MULTIPLEXING ,
    FAPI_tddSpecialBundlingHarqInfo_st SPECIAL_BUNDLING ^*/
#else
/*^	PDU_TYPE_MAP : mode
    FAPI_tddBundlingHarqInfoR9_st	       BUNDLING , 
    FAPI_tddMultiplexingHarqInfo_st    MULTIPLEXING ,
    FAPI_tddSpecialBundlingHarqInfo_st SPECIAL_BUNDLING ^*/
#endif
/* - SPR 13880 Changes */
/*  ------------------------------- */

/*^ PDU_TYPE_MAP : numOfHI::numOfDCI
 	FAPI_dlHiPduInfo_st    numOfHI,
 	FAPI_dlDCIPduInfo_st   numOfDCI ^*/
/* SPR 21660_2 changes end */

#ifdef FAPI_4_0_COMPLIANCE
/*^ PDU_TYPE_MAP : STRUCT 
 	FAPI_harqIndicationR8_st    PHY_UL_HARQ_INDICATION    REL8,
 	FAPI_harqIndicationR9_st    PHY_UL_HARQ_INDICATION    REL9, 
 	FAPI_harqIndicationR9_st    PHY_UL_HARQ_INDICATION    REL10 ^*/
#endif
/* SPR 19288 change start */
/* SPR 19288 change end */

/* Set the alignment back to default. Only structures in this file are aligned
 * to 32 bit boundary. */
#pragma pack()


#endif /* LTE_FAPI_MAC_PHY_INTERFACE_H */
