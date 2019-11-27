/******************************************************************************
*
*   FILE NAME:
*       rrc_defines.h
*
*   DESCRIPTION:
*       This file contains basic RRC types definitions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   31 Mar 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_DEFINES_H_
#define _RRC_DEFINES_H_

#include "emtcCommonValidation.h"
#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#define DEFAULT_RRC_LOG_FILE    "rrc_main.log"

/*Basic types */
#include <sys/types.h>

/* CSPL types */
#include <cspl.h>
#include <stacklayer.h>

#ifdef ALM_FRWK
#include "alarm.h"
#endif
#ifndef _MSC_VER
/*CDMA2000_CSFB_HO start*/
#include <pthread.h>
/*CDMA2000_CSFB_HO stop*/
#endif
/* make global visibility for static functions used in unit tests */
#ifdef RRC_DEBUG
#define STATIC
#else
#define STATIC static
#endif

#ifdef ALM_FRWK
    #define ALARM_MSG_L3(src_module_id,alarm_id,criticality,cause_id)    ALARM_MSG(src_module_id,alarm_id,criticality)
    #define ALARM_FMT_MSG_L3    ALARM_FMT_MSG
#else
    #define ALARM_MSG_L3(src_module_id, alarm_id, criticality,cause_id)
    #define ALARM_FMT_MSG_L3(src_module_id, alarm_id, criticality , ...)
#endif

/* CSR_00040799 Fix Start */
/*SPR 21653 changes start*/
/*ifdef MULTISECTOR_ENABLE flag removed as that is always defined*/
#define MAX_NUM_CELLS                           8
/*SPR 21653 changes end*/

/* SPR 20908 Start */
#define RRC_MAX_IP_SIZE 4
#define RRC_ONE         1
#define RRC_TWO         2
#define RRC_THREE       3
#define RRC_FOUR        4
#define RRC_FIVE        5
#define RRC_EIGHT       8
/* SPR 20908 End */
/* SPR 21806 Start */
#define RRC_HUNDRED	100
/* SPR 21806 End */
#define MAX_CELL_INDEX (MAX_NUM_CELLS - 1)

/* SPR 20526 Fix Start */
#define RRC_MAX_NUM_SUPPORTED_UE        MAX_UE_SUPPORTED
/* SPR 20526 Fix End */
/* CSR_00040799 Fix End */
/*SPR 19708 FIX START*/
#define RRC_12BIT_MAX_VALUE   4095 
/*SPR 19708 FIX START*/
#define ERAB_ID_MAX 15

#define RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES     5 
#define RRC_OAM_MAX_EXT_UPPER_LAYER_MODULES     3 

#define BROADCAST_IP_ADDR  0xFFFFFFFF
#define INVALID_IP_ADDR    0x00


/* MENB CHANGES - START */
#define MAX_ASN_BUFFER                      512
/* MENB CHANGES - END */

#define NAS_SECURITY_PARAM_INTER_RAT_HO_LEN 0x01

#define RRC_INET_ATON inet_aton
/* SPR 21371 Start */
#define IPV4_ADDRESS_LENGTH 4
#define IPV6_ADDRESS_LENGTH 40
/* SPR 21371 Start */

/* CSR_00040799 Fix Start */
/* Max Number of supported UE = RRC_MAX_NUM_SUPPORTED_UE (600) and 
 * Max Number of supported sectors = MAX_NUM_CELLS(8),
 * so total no. of supported UE = RRC_MAX_NUM_SUPPORTED_UE * MAX_NUM_CELLS  */
/* SPR 21251 Start */
/* SPR 21553 Start */
/* SPR 21773 Start */ 
#define MAX_NUM_SUPPORTED_UE_PER_UECC       RRC_MAX_NUM_SUPPORTED_UE
/* SPR 21773 End */ 
/* SPR 21553 End */
/* SPR 21251 End */
/* CSR_00040799 Fix End */

#define MAX_NUM_SUPPORTED_X2AP_UEID         512
#define INVALID_X2AP_UE_ID                  4096
#define MAX_UECC_INSTANCES                  0x08
#define MAX_CELL_REPORT                     8
#define MAX_FREQ                            8
#define RRC_OAM_MAX_EXT_SON_UPPER_LAYER_MODULES     4 
/* Trace Change Start */
#define TRACE_REF_LENGTH   6
/* Trace Change End */
#define MAX_MME_NAME                        150
#define MAX_NUM_SERVED_PLMN                 32
#define MAX_NUM_SERVED_GROUP_ID             256
#define MAX_NUM_SERVED_MMEC                 256
#define MMEGI_OCTET_SIZE                    2
/*CR 970 changes start*/
#define MAX_ASN_BUFF_LENGTH                 100
/*CR 970 changes stop*/

#define OAMH_HEALTH_MONITOR_EXPIRY_EVENT                1
#define UECC1_HEALTH_MONITOR_EXPIRY_EVENT               2
#define UECC2_HEALTH_MONITOR_EXPIRY_EVENT               3
#define UECC3_HEALTH_MONITOR_EXPIRY_EVENT               4
#define UECC4_HEALTH_MONITOR_EXPIRY_EVENT               5
#define UECC5_HEALTH_MONITOR_EXPIRY_EVENT               6
#define UECC6_HEALTH_MONITOR_EXPIRY_EVENT               7
#define UECC7_HEALTH_MONITOR_EXPIRY_EVENT               8
#define UECC8_HEALTH_MONITOR_EXPIRY_EVENT               9
#define CSC_HEALTH_MONITOR_EXPIRY_EVENT                 10
#define LLIM_HEALTH_MONITOR_EXPIRY_EVENT                11
#define ADV_LOGGER_HEALTH_MONITOR_EXPIRY_EVENT          12
#define S1AP_HEALTH_MONITOR_EXPIRY_EVENT                13 
#define SCTP_HEALTH_MONITOR_IPC_MASTER_EXPIRY_EVENT     14 
#define SCTP_HEALTH_MONITOR_RECV_TIMER_EXPIRY_EVENT     15 
#define X2AP_HEALTH_MONITOR_EXPIRY_EVENT                16
/* SPR:9111 start */
/* SPR 13502 Fix Start */
/* As per SPEC 36.331 maxBandComb-r10 = 128 can be used.
 * Due to system memory limitation we have been changed to value 32
 * to decrease the size of rrm_ue_context.
 * If system memory supports, 
 * The value can be changed to supported value as per spec 
 * i.e #define  MAX_BAND_COMB_R10 128 */
/* SPR 20817 Fix Start */
/* Optimization done under 18008 and 19986 removed */
/* SPR 21194 fix start*/
#define MAX_BAND_COMB_R10                   128
/* SPR 21194 fix end*/
/* SPR 20817 Fix End */
#define MAX_BAND_COMB_R11                   256
/* LAA Rel13 Code + */ 
#define MAX_BAND_COMB_R13                   46
#define CELL_INDEX_ONE                      1
/* LAA Rel13 Code - */ 
/* SPR 13502 Fix Stop */
/* SPR:9111 stop */
#define MAX_N1PUCCH_AN_CS                   4
#define MAX_N1PUCCH_AN_CS_LIST              2
#define MAX_SCELL                   4
/* CA start */
#define RRC_MAX_SCELL       2*MAX_SCELL
/* CA end*/
#define MAX_SERVING_CELL            5
#define CBSR_MAX_VALUE_V1020 14
#define RRC_OCTET_WINDOW            7 
#define MAX_CELLS_PER_UE            8
/* eICIC changes start */
#define MAX_SUBFRAME_CONFIG_FDD_NUMBITS                 40                  // eICIC ue reconfig changes start 
/* eICIC changes stop */
#define MAX_SUBFRAME_CONFIG1_5_TDD_NUMBITS              20
#define MAX_SUBFRAME_CONFIG0_TDD_NUMBITS                70
#define MAX_SUBFRAME_CONFIG6_TDD_NUMBITS                60
#define L3_S1AP_VERSION "\"a60\">"
#define L3_X2AP_VERSION "\"a50\">"
#define L3_RRC_VERSION "\"a70\">"
/* eICIC changes start */
#define CSI_MEAS_MEASUREMENT_SUBSET_SIZE    9
/* eICIC changes stop */
//#ifdef MULTISECTOR_ENABLE
#define  MAX_NUM_CURRENT_SUPPORTED_CELLS        2
//#else
//#define  MAX_NUM_CURRENT_SUPPORTED_CELLS        1
//#endif
#define MAX_MBSFN_SUBFRAME_CONFIG           8
/*SPR 18170:start*/
#define MAX_MBMS_UPDATED_AREA_LIST          16
/*SPR 18170:end*/
#define MAX_MBSFN_AREA_PER_ENB              MAX_NO_OF_MBSFN_AREAS /* MAX_NUM_CELLS * MAX_MBSFN_AREAS * (RESERVED + UNRESERVED) */
typedef U16 rim_bitmask_t;
typedef U8 rim_app_identity_t;
typedef U8      rim_pdu_ext_t;
typedef U8      rim_ack_t;
typedef U8  rim_enb_type_t;
typedef U8 rim_nacc_cause_t;
typedef U8 rim_utra_si_cause_t;
typedef U32  rim_pdu_seq_num_t;
typedef U8  rim_protocol_ver_t;
typedef U8      rim_cause_t;
typedef U8 rim_tag_t;
typedef U8 rim_length_t;


/* SPS Start */
#define UE_ADM_SUCCESS_AND_SPS_FAILURE    0x01
/* SPS Stop */
/*SPR 15570 Fix Start*/
#define SCTP_SELECT_RETRY_COUNT 20
#define SCTP_W_CONNECT_TIMER_DURATION 500
/*SPR 15570 Fix Stop*/
/*SPR_16533_START*/
#define SCTP_STREAM_ID_SIZE 2
/*SPR_16533_END*/

/* eICIC changes start */
typedef enum
{
    X2AP_ONE=0,
    X2AP_TWO,
    X2AP_FOUR
}cell_specific_antenna_port_et;
/* eICIC changes stop */

/* eICIC ue reconfig changes start */
typedef enum
{
    NON_VICTIM=0,
    VICTIM
}dl_interference_info_et;

typedef enum
{
    FDD=0,
    TDD_CONFIG_1_5,
    TDD_CONFIG_0,
    TDD_CONFIG_6
}rrc_mac_csi_meas_subframe_set_choice_et;
/* eICIC changes stop */

/* sync changes start */
typedef enum
{
    RRC_MAC_OUT_OF_SYN_INITIAL=0, 
    RRC_MAC_IN_SYN_AFTER_OUT_OF_SYN, 
    RRC_MAC_OUT_OF_SYN_MAX_RETRIES,
    RRC_INVALID_UE_SYNC_NUM=4
}rrc_rrm_ue_sync_status_et;
/* sync changes stop*/

/* re-estab changes start */
typedef enum
{
    RRC_RESTAB_CAUSE_RECONF_FAIL = 0,
    RRC_RESTAB_HO_FAIL,
    RRC_RESTAB_OTHER_FAIL,
    RRC_RESTAB_SP1_2
}rrc_rrm_ue_reestab_config_et;
/* re-estab changes stop */

/* Values for rrc_return_et, rrc_return_t */
typedef enum
{
    RRC_FAILURE,
    RRC_SUCCESS,
    RRC_PARTIAL_SUCCESS 
} rrc_return_et;

/* values for rrc_bool_et, rrc_bool_t */
typedef enum
{
    RRC_FALSE,
    RRC_TRUE
} rrc_bool_et;

/* Trace Change Start */
typedef enum
{
    TRACE_OAM_INITIATED,
    TRACE_S1AP_INITIATED,
    TRACE_X2AP_INITIATED
} rrc_trace_initiator_et;
typedef enum
{
    MACRO_ENB,
    HOME_ENB
}enb_type_et;
/* Trace Change End */

/* values for rrc_periodicity_bool_et  */
typedef enum
{
    RRC_OAM_STATS_PERIODICITY_DISABLED,
    RRC_OAM_STATS_PERIODICITY_ENABLED
} rrc_oam_stats_periodicity_et;


/*Reconfig mode values */
typedef enum
{
    NO_MODE,
    PHY_ONLY,
    MAC_ONLY,
    PHY_MAC_BOTH
}rrc_reconfig_mode_et;

typedef enum
{
   RRC_CELL_DELETION_ONGOING,
   RRC_DUPLICATE_REQ,
   RRC_WRONG_CELL_INDEX,
   /* SPR 22584 Changes - Starts */
   RRC_CELL_INACTIVE,
   /* SPR 22584 Changes - Ends */
   RRC_INVALID_CAUSE  
} rrc_trace_start_error_cause_et;

typedef enum
{
    RRC_TRACE_ACTIVATION_MME_INITIATE = 1,
    RRC_TRACE_ACTIVATION_CELL_INITIATE = 2
} rrc_trace_activation_flag_et;

typedef enum
{
    /*SPR 15896 Fix Start*/	
    RRC_OAM_INITIATED_TRACE_ALREADY_ONGOING,
    /*SPR 15896 Fix Stop*/	
    RRC_HO_ONGOING
} rrc_ue_trace_start_error_cause_et;

typedef enum
{
  RRM_RRC_TRUE_1
}concurrent_warning_msg_indicator_et;

typedef enum
{
    RRM_RRC_NO_ERROR,
    RRM_RRC_INTERNAL_FAILURE,
    RRM_RRC_LL_CONFIG_FAILURE,
    RRM_RRC_LL_DEL_FAILURE,
    RRM_RRC_UNKNOWN_FAILURE,
    RRM_RRC_INTERACTION_WITH_OTHER_PROCEDURE_FAILURE,
    RRM_RRC_CELL_START_TIMER_EXPIRED,
    RRM_RRC_CELL_STOP_TIMER_EXPIRED,
    RRM_RRC_PHY_START_FAILURE,
    RRM_RRC_PHY_STOP_FAILURE,
    RRM_RRC_MAC_START_FAILURE,
    RRM_RRC_MAC_STOP_FAILURE,
    RRM_RRC_CELL_START_ALREADY_ONGOING,
    RRM_RRC_CELL_STOP_ALREADY_ONGOING,
    RRM_RRC_INVALID_CONFIGURATION_RECV,
    RRM_RRC_CELL_CONFIG_DOES_NOT_EXIST,
    RRM_RRC_TIMER_EXPIRED,
    RRM_RRC_DUPLICATE_WARNING_MESSAGE
} rrm_fail_cause_et;

typedef enum
{
    RRM_RRC_UE_INFO_INTERNAL_FAILURE,
    RRM_RRC_UE_INFO_TIMER_EXPIRY,
    RRM_RRC_UE_INFO_INTERACTION_WITH_OTHER_PROCEDURE_FAILURE
}rrm_ue_info_fail_cause_et;

typedef enum
{
    RRC_ERAB_NOT_INITIALIZED = 0,
    /*SPR_17382_START*/
    RRC_ERAB_FAILURE_PATH_SWITCH_REQ_ACK,
    RRC_ERAB_FAILURE_S1U_RECONFIGURE_CNF,
    /*SPR_17382_END*/
    RRC_ERAB_SUCESS
} rrc_s1ap_path_switch_erab_released_status_et;


typedef enum
{
    RRM_RRC_CAUSE_UNKNOWN_FAILURE,
    RRM_RRC_CAUSE_NO_BROADCAST_ONGOING
} rrm_kill_fail_cause_et;

typedef enum
{
    RRC_RRM_LL_CONFIG_FAILURE,
    RRC_RRM_LL_DEL_FAILURE,
    RRC_RRM_UNKNOWN_FAILURE,
    RRC_RRM_NO_BROADCAST_ONGOING
} rrc_kill_fail_cause_et;

/* cr_316 And 247 enums */
typedef enum
{
    CQI_MASK_R9_SETUP
} cqi_mask_r9_et;

/*tm7-8 fix*/
typedef enum
{
    PMI_RI_REPORT_R9_MAC_RELEASE,
    PMI_RI_REPORT_R9_MAC_SETUP
}pmi_ri_report_r9_mac_et;
/*tm7-8 fix*/

typedef enum
{
    PMI_RI_REPORT_R9_SETUP
}pmi_ri_report_r9_et;

#ifndef OAM_IPR
/* CLPC Start */
typedef enum
{
    DCI_FORMAT_3,
    DCI_FORMAT_3A
}rrc_tpc_rnti_configuration_format_et;
#endif

/*sctp parameters start */
typedef enum
{
    S1AP_OAM_SCTP_CLOSED,
    S1AP_OAM_SCTP_BOUND,
    S1AP_OAM_SCTP_LISTEN,
    S1AP_OAM_SCTP_COOKIE_WAIT,
    S1AP_OAM_SCTP_COOKIE_ECHOED,
    S1AP_OAM_SCTP_ESTABLISHED,
    S1AP_OAM_SCTP_SHUTDOWN_PENDING,
    S1AP_OAM_SCTP_SHUTDOWN_SENT,
    S1AP_OAM_SCTP_SHUTDOWN_RECEIVED,
    S1AP_OAM_SCTP_SHUTDOWN_ACK_SENT
}s1ap_oam_sctp_status_et;

typedef enum
{
    XWAP_OAM_SCTP_CLOSED,
    XWAP_OAM_SCTP_BOUND,
    XWAP_OAM_SCTP_LISTEN,
    XWAP_OAM_SCTP_COOKIE_WAIT,
    XWAP_OAM_SCTP_COOKIE_ECHOED,
    XWAP_OAM_SCTP_ESTABLISHED,
    XWAP_OAM_SCTP_SHUTDOWN_PENDING,
    XWAP_OAM_SCTP_SHUTDOWN_SENT,
    XWAP_OAM_SCTP_SHUTDOWN_RECEIVED,
    XWAP_OAM_SCTP_SHUTDOWN_ACK_SENT
}xwap_oam_sctp_status_et;
/*sctp parameters stop*/


/* CLPC Stop */

typedef enum
{
    MME_ROUND_ROBIN_ALGO,
    MME_WEIGHT_FACTOR_ALGO
}s1ap_oam_mme_selection_algo_et;

/* SPS Start */
typedef enum
{
    RRM_RRC_SPS_SETUP_FAILED = 0,
    RRM_RRC_SCELL_ADDITION_FAILED = 1
}rrc_rrm_error_code_et;
/* SPS Stop */

/* LIPA start */
typedef enum
{
    LGW_NOT_CONFIGURED = 1,
    DEL_LGW_REQ_RECVD_IN_INVALID_L3_STATE
}rrc_oam_delete_lgw_fail_cause_et;

typedef enum
{
    LGW_ALREADY_CONFIGURED = 1,
    INVALID_LGW_IP_ADDR_CONFIGURED,
    ADD_LGW_REQ_RECVD_IN_INVALID_L3_STATE
}rrc_oam_add_lgw_fail_cause_et;

typedef enum
{
    GTP_ERROR_IND = 1,
    GTP_PATH_FAIL,
    OAM_INITIATED_LGW_DEL
}rrc_rrm_error_ind_cause_et;
/* LIPA end */

typedef enum
{
    PERIOD_FACTOR_N2,
    PERIOD_FACTOR_N4
}cqi_periodic_format_ind_subband_cqi_periodicy_factor_et;

typedef enum
{
  RRC_RADIO_FRAME_2,
  RRC_RADIO_FRAME_4,
  RRC_RADIO_FRAME_8,
  RRC_RADIO_FRAME_16,
  RRC_RADIO_FRAME_32,
  RRC_RADIO_FRAME_64,
  RRC_RADIO_FRAME_128,
  RRC_RADIO_FRAME_INFINITY=255
} scell_deactivation_timer_et;

typedef enum
{
  EXTENDED_BSR_SIZES_SETUP
} extended_bsr_sizes_et;

typedef enum
{
  EXTENDED_PHR_SETUP
} extended_phr_et;
typedef enum
{
  RRM_RRC_CSI_AN1,
  RRM_RRC_CSI_AN2,
  RRM_RRC_CSI_AN4,
  RRM_RRC_CSI_AN8
} rrc_rrm_csi_rs_r10_antennaPortCount_et;

typedef enum
{
  RRM_RRC_TM1,
  RRM_RRC_TM2,
  RRM_RRC_TM3,
  RRM_RRC_TM4,
  RRM_RRC_TM5,
  RRM_RRC_TM6,
  RRM_RRC_TM7,
  RRM_RRC_TM8_V920,
  RRM_RRC_TM9_V1020
} rrc_rrm_transmission_mode_r10_et;

typedef enum
{
  RRM_RRC_N6_5,
  RRM_RRC_N15_2,
  RRM_RRC_N25_2,
  RRM_RRC_N50_3,
  RRM_RRC_N75_2,
  RRM_RRC_N100_3
} rrc_rrm_scell_non_ul_dl_bandwidth_et;

typedef enum
{
  RRM_RRC_AN1,
  RRM_RRC_AN2,
  RRM_RRC_AN4
} rrc_rrm_scell_non_ul_antenna_port_count_et;

typedef enum
{
    N2_TX_ANTENNA_TM3_V1020 = 0,
    N4_TX_ANTENNA_TM3_V1020 = 1,
    N2_TX_ANTENNA_TM4_V1020  = 2,
    N4_TX_ANTENNA_TM4_V1020 = 3,
    N2_TX_ANTENNA_TM5_V1020 = 4,
    N4_TX_ANTENNA_TM5_V1020 = 5,
    N2_TX_ANTENNA_TM6_V1020 = 6,
    N4_TX_ANTENNA_TM6_V1020 = 7,
    N2_TX_ANTENNA_TM8_R9_V1020 = 8,
    N4_TX_ANTENNA_TM8_R9_V1020 = 9
} codebook_subset_restriction_v1020_type_et;

#define TX_ANTENNA_INVALID_VAL 10

typedef enum
{
    N2_TX_ANTENNA_TM3_NUM_BITS_V1020 = 2,
    N4_TX_ANTENNA_TM3_NUM_BITS_V1020 = 4,
    N2_TX_ANTENNA_TM4_NUM_BITS_V1020  = 6,
    N4_TX_ANTENNA_TM4_NUM_BITS_V1020 = 64,
    N2_TX_ANTENNA_TM5_NUM_BITS_V1020 = 4,
    N4_TX_ANTENNA_TM5_NUM_BITS_V1020 = 16,
    N2_TX_ANTENNA_TM6_NUM_BITS_V1020 = 4,              /* trans_mode_fix */
    N4_TX_ANTENNA_TM6_NUM_BITS_V1020 = 16,             /* trans_mode_fix */
    N2_TX_ANTENNA_TM8_NUM_BITS_R9_V1020 = 6,
    N4_TX_ANTENNA_TM8_NUM_BITS_R9_V1020 = 32,          /* trans_mode_fix */
    N2_TX_ANTENNA_TM9_NUM_BITS_R10_V1020 = 6,
    N4_TX_ANTENNA_TM9_NUM_BITS_R10_V1020 = 64,
    N8_TX_ANTENNA_TM9_NUM_BITS_R10_V1020 = 109
} codebook_subset_restriction_v1020_numbits_et;
typedef enum
{
    ADD_SCELL = 0,
    MODIFY_SCELL = 1,
    DELETE_SCELL = 2
}operation_type_et;

/* RRC types */

/* eMTC changes start */
#define RRC_DEFAULT_TWO_BITS_SET            0xFFC0
#define RRC_DEFAULT_SF_PATTERN_2BITS_SHIFT  0x2
#define RRC_DEFAULT_SF_PATTERN_4BITS_SHIFT  0x4
#define RRC_DEFAULT_SF_PATTERN_6BITS_SHIFT  0x6
#define RRC_DEFAULT_SF_PATTERN_MSB_8BITS    0xFF
#define RRC_DEFAULT_SF_PATTERN_MSB_4BITS    0xF0
#define RRC_DEFAULT_SF_PATTERN_MSB_6BITS    0xFC
#define RRC_DEFAULT_SF_PATTERN_LSB_6BITS    0x3F 
#define RRC_DEFAULT_SF_PATTERN_LSB_4BITS    0x0F 
#define RRC_DEFAULT_SF_PATTERN_LSB_2BITS    0xC0 
#define RRC_DEFAULT_SF_PATTERN_2BITS        0x3 
#define RRC_SIX                             6
#define RRC_TEN                             10  
#define RRC_TWENTY                          20
#define RRC_THIRTY_TWO                      32
#define RRC_FOURTY                          40
#define RRC_SIXTY                           60
#define RRC_SIXTY_FOUR                      64
#define RRC_EIGHTY                          80
#define RRC_ONE_HUNDRED_TWENTY              120
#define RRC_ONE_HUNDRED_TWENTY_EIGHT        128
#define RRC_ONE_HUNDRED_SIXTY               160
#define RRC_TWO_HUNDRED                     200
#define RRC_TWO_HUNDRED_FIFTY_SIX           256
#define RRC_SEVEN                           7
#define RRC_TEN                             10
#define RRC_THIRTEEN                        13
#define RRC_FOURTEEN                        14
#define RRC_FIFTEEN                         15
#define RRC_SIXTEEN                         16
#define RRC_TWENTY                          20
#define RRC_FOURTY                          40
#define RRC_SIXTY                           60
#define RRC_EIGHTY                          80
#define RRC_ONE_TWENTY                      120
#define RRC_ONE_SIXTY                       160
#define RRC_TWO_HUNDRED                     200
#define RRC_HEX_TWO_FIFTY_FIVE              0xFF
#define RRC_INVALID_PAGING_NARROBAND        0xFF
#define RRC_THIRHTY_TWO                     32
typedef U8              RrcBitmaskU8_t;
typedef U16             RrcCounter_t;
typedef U16             RrcSfn_t;
/* eMTC changes stop */

typedef U16             rrc_bitmask_t;
typedef U32             rrc_bitmask_U32_t;
typedef U16             rrc_error_t;
typedef U16             rrc_ue_index_t;
typedef U8              rrc_lc_id_t;
typedef U16             rrc_response_t;
typedef U16             rrc_tag_t;
typedef U16             rrc_length_t;
typedef U16             rrc_counter_t;
typedef U16             rrc_sfn_t;
typedef U8              rrc_sf_t;
/*CDMA2000_CSFB_HO start*/
typedef U64             rrc_timestamp_U64_t;
typedef U32             rrc_timestamp_U32_t;
/*CDMA2000_CSFB_HO stop*/
typedef U16             rrc_rnti_t;
typedef U16             rrc_transaction_id_t;
typedef U8              rrc_cell_index_t;
/* SPR 20908 Start */
typedef U8              rrc_l2_instance_t;
/* SPR 20908 End */
typedef U16             rrc_sn_field_l_t;
typedef U16             rrc_t_reordering_t;
typedef void            rrc_void_t;
typedef size_t          rrc_size_t;
typedef QTIMER          rrc_timer_t;
typedef U16             rrc_timer_duration_t;
/* NBIOT TIMER FIX */
typedef U32             rrc_timer_duration_u32_t;
/* NBIOT TIMER FIX */
typedef U8              rrc_return_t;
typedef U8              rrc_bool_t;
typedef U16             rrc_module_id_t;
typedef U16             rrc_rb_direction_t;
typedef U16             rrc_phys_cell_id_t;
typedef U32             rrc_gtp_teid_t;
typedef U8              rrc_retry_count_t;


#define RRC_ASN_CNTXT OSCTXT

#define ENDIAN_INIT 1
#define IS_LITTLE_ENDIAN(endian_check) (*((U8*)&endian_check)?1:0)

#define ENDIAN_INIT 1
#define IS_LITTLE_ENDIAN(endian_check) (*((U8*)&endian_check)?1:0)
/* Means that encapsulating shell should release the buffer after processing of
 *  the message */

#define RRC_BUFFER_SHOULD_BE_RELEASED 1

/* Settings for ASN buffer */
#define ASN_RRC_BUF_TYPE FALSE
#define ASN_S1AP_BUF_TYPE TRUE
#define ASN_X2AP_BUF_TYPE TRUE

/* Special value for rrc_transaction_id_t */
#define RRC_TRANSACTION_ID_ABSENT       0xFFFF
/* SPR 16087 Start */
#define RRC_S1U_END_MARKER_RESERVED_TRANSACTION_ID       0xFFFE
/* SPR 16087 Stop */

#define RRC_ZERO        0
#define RRC_SIX         6
#define RRC_ONE         1
#define RRC_TWO         2

#define RRC_NULL        0
#define RRC_PNULL 0
#ifndef OAM_IPR
#ifndef _PNULL_
#define _PNULL_
#define PNULL           ((void *)0)
#endif
#endif
#define RRC_VERSION_ID          0x01
#define RRC_INVALID_RNTI            0      
#define RRC_INVALID_CELL_INDEX      0xFF      
#define RRC_INVALID_CORE_NUM        0xFF      
#define RRC_INVALID_UE_INDEX        0xFFFF
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
#define MAX_VALID_DC_UE_ID          0xFFF
/*NR_DC Code Change Stop*/
#endif
/* CSR_00040799 Fix Start */
/* Maximum number of supported UEs for Release 1.0 */
/* #define RRC_MAX_NUM_SUPPORTED_UE        600 */
/* CSR_00040799 Fix End */
#define MAX_RRC_CONNECTION              32
#define RRC_SRB0_LC_ID                  0
#define RRC_SRB1_LC_ID                  1
#define RRC_SRB2_LC_ID                  2

#define RRC_MAX_NUM_SRB                 3
#define RRC_MAX_NUM_DRB                 11
#define RRC_MAX_NUM_RB                  14
#define RRC_MAX_INTEGRITY_ALGORITHMS    3
#define RRC_MAX_CIPHERING_ALGORITHMS    3
/* ipv6_change */
#define RRC_MAX_IP_ADDR_LEN             4
#define RRC_MAX_IP_ADDR_LEN_IPV6        16
/* ipv6_change */
#define RRC_S1U_MAX_QOS_PROFILE_DATA    254
#define MAX_FWD_TUNNELS_PER_ERAB        2
#define RRC_VENDOR_NAME_LEN             8

#define RRC_S1U_MAX_NUM_LC      8
/* cd_761 macro */
#define MAX_LAC_NUMOCTS 2
#define MAX_PLMN_ID_NUMOCTS 3

/*REL 1.2:HANDOVER PROCEDURE START*/
#define MAX_NO_OF_MESSAGES   33    /* Including target cell short mac-i*/
#define UL_PDCP_SDU_NUM_OF_BITS  4096
/*REL 1.2:HANDOVER PROCEDURE START*/

#define RRC_MAX_FSM                 0x0A
#define RRC_MAX_STATE_PER_FSM       0x06
#define RRC_MAX_API_TRANSITION      0x02
#define RRC_MAX_PROC_TIMER          0x0f


/* ERAB SETUP START */

#define RRC_MIN_QCI_GBR_LIMIT       1
#define RRC_MAX_QCI_GBR_LIMIT       4
#define RRC_MIN_QCI_NON_GBR_LIMIT   5
#define RRC_MAX_QCI_NON_GBR_LIMIT   9

#define MAX_MME_ERAB_LIST_COUNT     256
#define MAX_SUPPORTED_E_RAB_ID      15
#define MAX_CELL_ID_BITS            28
#define MAX_MACRO_CELL_ID_BITS      20 
#define MAX_HOME_CELL_ID_BITS       28

/* ERAB SETUP STOP */
/* Trace Procedure start */
#define MAX_FILE_NAME_SIZE                      256
#define MAX_UECC_FILE_NAME_SIZE                 262 /* MAX_FILE_NAME_SIZE + sizeof(_UECC1) */
#define MAX_TRANSPORT_LAYER_ADDR_SIZE           20
#define RRC_TRACE_REFERENCE_LENGTH              6
#define MAX_STRING_SIZE                         25   
/* SPR 16113 Start */
#define RRC_DL_CCCH             "RRC_DL_CCCH"
#define RRC_DL_DCCH             "RRC_DL_DCCH"
#define RRC_UL_CCCH             "RRC_UL_CCCH"
#define RRC_UL_DCCH             "RRC_UL_DCCH"
/* SPR 16113 Stop */
/* Trace Procedure stop */

#define RRC_MAX_INACTIVE_UES  10
#define BEARER_TUNNEL_ID  0
#define FWD_TUNNEL_ID_1   1
#define FWD_TUNNEL_ID_2   2
#define NUM_TUNNELS_WITHOUT_FWD                 1
#define NUM_TUNNELS_WITH_FWD_EITHER_UL_OR_DL    2
#define NUM_TUNNELS_WITH_FWD_BOTH_UL_AND_DL      3
#define SAP_FLAGS_FOR_UL_FORWARDING                0X0F
#define SAP_FLAGS_FOR_DL_FORWARDING                0X25
/* lipa start */
#define NUM_TUNNELS_LIPA 2
#define SAP_FLAG_PRIMARY_TUNNEL 0x0F
#define SAP_FLAG_SECONDARY_TUNNEL 0x0E
/* lipa end */

#define SAP_FLAG_LWA_TUNNEL  0x8F
/*Maximum Logical channels for Data are 8 (3 to 10) */
#define MAX_ERAB_COUNT              16
#define MAX_LC_COUNT                8
#define RRC_OCTET_SIZE              8
#define INVALID_ERAB_ID             16

/* Used in S1AP (and RRM) */
#define RRC_CELL_IDENTITY_BITS      28
#define RRC_CSG_ID_BITS             27

/* SFN is 10 bits wide: 0, 1, 2, ... SFN_UPPER_LIMIT - 1 */
#define SFN_UPPER_LIMIT             1024
#define SF_MAX_LIMIT              10

/*CDMA2000_CSFB_HO start*/
/*should be upper than the time in sfn required for L2 to process 
the mac_reconfig_req message*/
#define GUARD_PERIOD_IN_SFN       5
#define MIN_SI_PERIODICITY         8

/*If sib8 periodicity is minimum i.e. 8 SFN, then from 0 to 1023, sib8 can come 128 times at max */
#define MAX_SIB_INSTANCES          SFN_UPPER_LIMIT/MIN_SI_PERIODICITY

/*CDMA2000_CSFB_HO stop*/
/* UL DL ARFCN DIFFERENCE */
#define UL_DL_ARFCN_DIFFRENCE 18000

/* LIPA start */
#define MAX_LOCAL_ERROR_IMPACTED_ITEMS     255
/* LIPA end */

#define MSG_ID_OCTET_SIZE                   2
#define SERIAL_NUMBER_OCTET_SIZE            2
#define SECURITY_INFORMATION_OCTET_SIZE     50
#define RRC_MAX_THREADS 12
//#ifdef ARICENT_SCTP_IPR
//#define S1AP_SCTP_MAX_THREADS 3
//#else
#define S1AP_SCTP_MAX_THREADS 1
//#endif


/* Value for External Module Ids */
#define RRC_MIN_EXT_MODULE_ID   1
#define RRC_OAM_MODULE_ID   (RRC_MIN_EXT_MODULE_ID + 0)
#define RRC_RRM_MODULE_ID   (RRC_MIN_EXT_MODULE_ID + 1)
#ifndef OAM_IPR
#define RRC_MODULE_ID       (RRC_MIN_EXT_MODULE_ID + 2)
#endif
#define RRC_S1U_MODULE_ID   (RRC_MIN_EXT_MODULE_ID + 3)
#define RRC_PDCP_MODULE_ID  (RRC_MIN_EXT_MODULE_ID + 4)
#define RRC_RLC_MODULE_ID   (RRC_MIN_EXT_MODULE_ID + 5)
#define RRC_MAC_MODULE_ID   (RRC_MIN_EXT_MODULE_ID + 6)
#define RRC_PHY_MODULE_ID   (RRC_MIN_EXT_MODULE_ID + 7)
#define RRC_MME_MODULE_ID   (RRC_MIN_EXT_MODULE_ID + 8) /*now used only for FT*/
#define RRC_X2AP_PEER_eNODEB_MODULE_ID (RRC_MIN_EXT_MODULE_ID + 9)
#if   LTE_EMBMS_SUPPORTED
#define RRC_MCE_MODULE_ID   (RRC_MIN_EXT_MODULE_ID + 10)
#define RRC_MAX_EXT_MODULE_ID RRC_MCE_MODULE_ID
#else
#define RRC_MAX_EXT_MODULE_ID RRC_X2AP_PEER_eNODEB_MODULE_ID
#endif

#define RRC_SON_MODULE_ID   100

#ifdef MODE_PROFILER_DEFINED
#define RRC_WRONG_MODULE_ID   0xfe
#endif

/* Value for Internal Module Ids */
#define RRC_MIN_INT_MODULE_ID   0x10
#define RRC_OAMH_MODULE_ID    (RRC_MIN_INT_MODULE_ID + 0)
#define RRC_UECC_MODULE_ID    (RRC_MIN_INT_MODULE_ID + 1) /* Depricated in Rel4.0 */
#define RRC_CSC_MODULE_ID     (RRC_MIN_INT_MODULE_ID + 2)
#define RRC_LLIM_MODULE_ID    (RRC_MIN_INT_MODULE_ID + 3)
#define RRC_S1AP_MODULE_ID    (RRC_MIN_INT_MODULE_ID + 4)
#define RRC_X2AP_MODULE_ID    (RRC_MIN_INT_MODULE_ID + 5)
#define RRC_UECCMD_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 6)
#define RRC_UECC_1_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 7)
#define RRC_UECC_2_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 8)
#define RRC_UECC_3_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 9)
#define RRC_UECC_4_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 10)
#define RRC_UECC_5_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 11)
#define RRC_UECC_6_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 12)
#define RRC_UECC_7_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 13)
#define RRC_UECC_8_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 14)
#define RRC_S1U_1_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 15)
#define RRC_S1U_2_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 16)
#define RRC_S1U_3_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 17)
#define RRC_S1U_4_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 18)
#define RRC_S1U_5_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 19)
#define RRC_S1U_6_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 20)
#define RRC_S1U_7_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 21)
#define RRC_S1U_8_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 22)
#define RRC_PDCP_1_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 23)
#define RRC_PDCP_2_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 24)
#define RRC_PDCP_3_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 25)
#define RRC_PDCP_4_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 26)
#define RRC_PDCP_5_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 27)
#define RRC_PDCP_6_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 28)
#define RRC_PDCP_7_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 29)
#define RRC_PDCP_8_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 30)
#define RRC_RLC_1_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 31)
#define RRC_RLC_2_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 32)
#define RRC_RLC_3_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 33)
#define RRC_RLC_4_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 34)
#define RRC_RLC_5_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 35)
#define RRC_RLC_6_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 36)
#define RRC_RLC_7_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 37)
#define RRC_RLC_8_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 38)
#define RRC_MAC_1_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 39)
#define RRC_MAC_2_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 40)
#define RRC_MAC_3_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 41)
#define RRC_MAC_4_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 42)
#define RRC_MAC_5_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 43)
#define RRC_MAC_6_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 44)
#define RRC_MAC_7_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 45)
#define RRC_MAC_8_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 46)
#define RRC_PHY_1_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 47)
#define RRC_PHY_2_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 48)
#define RRC_PHY_3_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 49)
#define RRC_PHY_4_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 50)
#define RRC_PHY_5_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 51)
#define RRC_PHY_6_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 52)
#define RRC_PHY_7_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 53)
#define RRC_PHY_8_MODULE_ID   (RRC_MIN_INT_MODULE_ID + 54)
#define RRC_LOGGER_MODULE_ID  (RRC_MIN_INT_MODULE_ID + 55)
#ifndef LINUX_PC_TEST
#if   LTE_EMBMS_SUPPORTED
#define RRC_M2AP_MODULE_ID    (RRC_MIN_INT_MODULE_ID + 56) 
#define RRC_MAX_INT_MODULE_ID  RRC_M2AP_MODULE_ID
#else
#define RRC_MAX_INT_MODULE_ID RRC_LOGGER_MODULE_ID
#endif
#else
#define RRC_PCTEST_MODULE_ID    (RRC_MIN_INT_MODULE_ID + 57)
#define RRC_MAX_INT_MODULE_ID RRC_PCTEST_MODULE_ID
#endif//LINUX_PC_TEST

#ifdef RRC_UT_FRAMEWORK
#define RRC_UT_FRWK_MODULE_ID 90
#endif

/*Error Codes for RRC_PDCP*/
#define    RRC_PDCP_ERROR_CODE_BASE           0x0100
#define    RRC_PDCP_ERR_CONTEXT_NOT_INITIALIZED        RRC_PDCP_ERROR_CODE_BASE + 0
#define    RRC_PDCP_ERR_CONTEXT_ALREADY_INITIALIZED    RRC_PDCP_ERROR_CODE_BASE + 1
#define    RRC_PDCP_ERR_UE_CONTEXT_ALREADY_CREATED     RRC_PDCP_ERROR_CODE_BASE + 2
#define    RRC_PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED     RRC_PDCP_ERROR_CODE_BASE + 3
#define    RRC_PDCP_ERR_ENTITY_ALREADY_CREATED         RRC_PDCP_ERROR_CODE_BASE + 4
#define    RRC_PDCP_ERR_ENTITY_WRONG_DIRECTION         RRC_PDCP_ERROR_CODE_BASE + 5
#define    RRC_PDCP_ERR_ENTITY_WRONG_TYPE              RRC_PDCP_ERROR_CODE_BASE + 6
#define    RRC_PDCP_ERR_ENTITY_NOT_FOUND               RRC_PDCP_ERROR_CODE_BASE + 7
#define    RRC_PDCP_ERR_ENTITY_SET_SN_SIZE             RRC_PDCP_ERROR_CODE_BASE + 8

#define    RRC_PDCP_ERR_ENTITY_SET_STATUS_REPORT_REQUIRED  RRC_PDCP_ERROR_CODE_BASE + 9

#define    RRC_PDCP_ERR_TLV_PARSING_INVALID_LENGTH     RRC_PDCP_ERROR_CODE_BASE + 10
#define    RRC_PDCP_ERR_TLV_PARSING_INVALID_UE_ID      RRC_PDCP_ERROR_CODE_BASE + 11

#define    RRC_PDCP_ERR_TLV_PARSING_INVALID_OPTIONAL_PARAMETERS_LENGTH RRC_PDCP_ERROR_CODE_BASE + 12

#define    RRC_PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH RRC_PDCP_ERROR_CODE_BASE + 13
#define    RRC_PDCP_ERR_TLV_PARSING_INVALID_LC_ID      RRC_PDCP_ERROR_CODE_BASE + 14

#define    RRC_PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE RRC_PDCP_ERROR_CODE_BASE + 15

#define    RRC_PDCP_ERR_TLV_PARSING_INVALID_CRNTI      RRC_PDCP_ERROR_CODE_BASE + 16
#define    RRC_PDCP_ERR_TLV_PARSING_INVALID_TAG_ID     RRC_PDCP_ERROR_CODE_BASE + 17
#define    RRC_PDCP_ERR_TLV_PARSING_INVALID_RNTI_RANGE RRC_PDCP_ERROR_CODE_BASE + 18
#define    RRC_PDCP_ERR_TLV_PARSING_INVALID_API_ID     RRC_PDCP_ERROR_CODE_BASE + 19


/*Error Codes for RRC_RLC*/
#define    RRC_RLC_ERROR_CODE_BASE           0x0200
#define    RRC_RLC_INVALID_UE_INDEX          RRC_RLC_ERROR_CODE_BASE + 0          
#define    RRC_RLC_UE_ID_EXISTS              RRC_RLC_ERROR_CODE_BASE + 1          
#define    RRC_RLC_UE_NOT_EXISTS             RRC_RLC_ERROR_CODE_BASE + 2          
#define    RRC_RLC_INTERNAL_ERROR            RRC_RLC_ERROR_CODE_BASE + 3          
#define    RRC_RLC_SYNTAX_ERROR              RRC_RLC_ERROR_CODE_BASE + 4          
#define    RRC_RLC_INVALID_LC_ID             RRC_RLC_ERROR_CODE_BASE + 5          
#define    RRC_RLC_ENTITY_EXISTS             RRC_RLC_ERROR_CODE_BASE + 6          
#define    RRC_RLC_ENTITY_NOT_EXISTS         RRC_RLC_ERROR_CODE_BASE + 7          
#define    RRC_RLC_UE_ENTITY_IN_USE          RRC_RLC_ERROR_CODE_BASE + 8          


/*Error Codes for RRC_RLC*/
#define    RRC_S1U_ERROR_CODE_BASE           0x0300
#define    RRC_S1U_SUCCESS                   RRC_S1U_ERROR_CODE_BASE + 0
#define    RRC_S1U_RESOURCES_NOT_AVAILABLE   RRC_S1U_ERROR_CODE_BASE + 1
#define    RRC_S1U_CTXT_NOT_FOUND            RRC_S1U_ERROR_CODE_BASE + 2
#define    RRC_S1U_DUPLICATE_PEER_TEID       RRC_S1U_ERROR_CODE_BASE + 3
#define    RRC_S1U_INV_SAP_CFG               RRC_S1U_ERROR_CODE_BASE + 4
#define    RRC_S1U_IPV6_ADDR_RECEIVED        RRC_S1U_ERROR_CODE_BASE + 5

/*Error Codes for RRC_UECC_LLIM (Per LC)*/
#define    RRC_UECC_LLIM_ERROR_CODE_BASE     0x0400
#define    RRC_UECC_LLIM_TIMER_EXPIRY        RRC_UECC_LLIM_ERROR_CODE_BASE + 0
#define    RRC_UECC_LLIM_ROLLBACK_FAILURE    RRC_UECC_LLIM_ERROR_CODE_BASE + 1    


/* Number of bits for MIB and SIBs IEs */
#define TAC_NBITS           16
#define CELL_ID_NBITS       28
#define CSG_ID_NBITS        27
#define ONE_FRAME_NBITS     6
#define FOUR_FRAME_NBITS    24
#define N_CELL_CFG_NBITS    2
#define SYN_SYS_TIME_NBITS  39
#define ASYN_SYS_TIME_NBITS 49
#define CSFB_REG_PARAM_1_XRTT_SID_NBITS 15
#define CSFB_REG_PARAM_1_XRTT_NID_NBITS 16
#define CSFB_REG_REG_PERIOD_NBITS       7
#define CSFB_REG_REG_ZONE_NBITS         12
#define CSFB_REG_TOTAL_ZONE_NBITS       3
#define CSFB_REG_ZONE_TIMER_NBITS       3
/*SPR_6293 Start*/
#define LONG_CODE_STATE_1_XRTT_NBITS    42
/*SPR_6293 Stop*/
#define NCC_PERMITTED_NBITS             8

/* Reestablishment_FIX start */
#define LC_ID_VAL_TO_CAL_MAC_I          0xFF
#define RB_DIR_VAL_TO_CAL_MAC_I         0xFFFF         
/* Reestablishment_FIX stop */
#define CELL_ID_NBITS       28
#define CRNTI_NBITS         16

#define EMERGENCY_AREA_SIZE                 3
#define MAX_SIB_MESSAGE                     32
#define MAX_SIB_MESSAGE_1                   MAX_SIB_MESSAGE - 1
#define MAX_PLMN_ID_BYTES                   3
#define MAX_SI_MESSAGE                      12
#define WARNING_MSG_SEGMENT_OCTET_SIZE      9600
#define WARNING_TYPE_OCTET_SIZE             2
#define MME_GROUP_ID_OCTET_SIZE             2
#define MME_CODE_OCTET_SIZE                 1
#define E_UTRAN_TRACEID_OCTET_SIZE          1
#define DATA_CODING_SCHEME_OCTET_SIZE       1
#define WARNING_MSG_OCTET_SIZE              9600    
#define MAX_WARNING_MESSAGE                 3


#define MAX_NUM_TAI               MAX_NUM_CELLS

#define HOME_ENB_ID_OCTET_SIZE          4
#define MAX_TAC_BYTES 2
#define MAX_FIVE_GS_TAC_BYTES 3
#define    X2AP_EUTRA_MODE_FDD   0x01
#define    X2AP_EUTRA_MODE_TDD   0x02
#define MAX_SERVED_CELLS 8
#define MAX_SERVED_NR_CELLS 2
#define MAX_ENB_MEASUREMENTS 4
#define MAX_ENB_POOLS 16
#define MAX_PEER_ENB  32
#define MAX_X2AP_IP_ADDRESS_LENGTH               16
/* IPV6 Start */
#define MAX_X2AP_IPV6_ADDRESS_LENGTH             40
/* IPV6 Stop */
#define MAX_NUM_ENB_X2AP_EXT_TLAS       16
#define MAX_NUM_ENB_X2AP_GTP_TLAS       16
#define MACRO_ENB_ID_OCTET_SIZE         3
#define MAX_NUM_IP_ADDR                     3
#define MAX_NUM_BPLMN 6
#define MAX_MME_GRP_BYTES 2
#define MAX_NEIGHBOURS 256
#define SCTP_SD_OCTET_SIZE          4
#define NUM_ANTENNA_PORT_BITMASK  0x01
#define GTP_TE_ID_BYTES 4
#ifndef OAM_IPR
/* CSR_00040799 Fix Start */
#define MAX_NUM_UE              MAX_NUM_SUPPORTED_UE_PER_UECC
#endif
/* #define NEW_MAX_NUM_UE  256 */
/* CSR_00040799 Fix End */

#define X2AP_ENB_CFG_RETRY_TIMER_VAL_DEFAULT	5000
#define X2AP_ENB_CFG_TIME_TO_WAIT_RETRY_COUNT_DEFAULT 3
#define X2AP_ENB_CFG_TIME_TO_WAIT_OFFSET_TIMER_VAL_DEFAULT 1000
#define X2AP_RESET_RETRY_COUNT_DEFAULT	3
#define X2AP_RESET_RETRY_TIMER_VAL_DEFAULT 3000
#define X2AP_CELl_ACTIVATION_RETRY_COUNT_DEFAULT	3
#define X2AP_CELl_ACTIVATION_TIMER_VAL_DEFAULT 7000
#define X2AP_MOBILITY_CHANGE_RETRY_COUNT_DEFAULT 3
#define X2AP_MOBILITY_CHANGE_RETRY_TIMER_VAL_DEFAULT 7000
#define X2AP_SUBFRAME_ALLOC_ONE_FRAME_NUMBITS 6
#define X2AP_SUBFRAME_ALLOC_FOUR_FRAME_NUMBITS 24
#define X2AP_SUBFRAME_ALLOC_ONE_FRAME_OCTET_SIZE 1
#define X2AP_SUBFRAME_ALLOC_FOUR_FRAME_OCTET_SIZE 3
#define MAX_NUM_MBSFN 8
#define X2AP_NUM_ANTENNA_INFO_PORT_PRESENT 0x01
#define X2AP_PRACH_CONFIG_INFO_PRESENT 0x02
#define X2AP_MBSFN_SUBFRAME_INFO_PRESENT 0x04
#define X2AP_CSG_ID_INFO_PRESENT 0x08
#define MAX_IP_ADDRESS_LENGTH           16
/* IPV6 Start */
#define MAX_IPV6_ADDRESS_LENGTH         40
/* IPV6 Stop */
#define RRC_X2APCOMMON_MODULE_ID 10
#define RRC_S1APCOMMON_MODULE_ID 11

/* CDMA2000_CSFB_HO start*/
#define MOBILITY_PARAMS_MAX_OCTETS          64
#define CDMA2000_1XRTT_CELL_ID_BITS         47
#define MAX_SECTOR_ID_BITS                  128
#define SIZE_OF_PILOT_LIST_PARAMS            256
#define MEID_NBITS                          56

/* PILOT LIST Start */
#define CDMA2000_PILOT_LIST_LENGTH                         0
#define CDMA2000_PILOT_LIST_LENGTH_WIDTH                   8
#define CDMA2000_PILOT_LIST_A21_ELMENT_IDENTIFIER          3
#define CDMA2000_PILOT_LIST_A21_ELMENT_IDENTIFIER_WIDTH    8
#define CDMA2000_PILOT_LIST_NUM_OF_PILOTS_WIDTH            8
#define CDMA_2000_PILOT_LIST_SYSTEM_TYPE_WIDTH             8 /*Ref: C.S0024-B_v2.0 section:13.1*/
#define CDMA_2000_PILOT_LIST_BAND_CLASS_WIDTH              5 /*Ref: C.S0024-B_v2.0 section:13.1*/
#define CDMA_2000_PILOT_LIST_CHANNEL_NUMBER_INFO_WIDTH    11 /*Ref: C.S0024-B_v2.0 section:13.1*/

/*
   (CDMA_2000_PILOT_LIST_SYSTEM_TYPE_WIDTH + CDMA_2000_PILOT_LIST_BAND_CLASS_WIDTH + 
   CDMA_2000_PILOT_LIST_CHANNEL_NUMBER_INFO_WIDTH)/ sizeof(U8) 
   Ref: C.S0024-B_v2.0 section:13.1 
*/
#define CDMA_2000_PILOT_LIST_CHANNEL_RECORD_LENGTH         3
#define CDMA_2000_PILOT_LIST_CELL_ID_INFO_WIDTH            3
#define CDMA_2000_PILOT_LIST_CHANNEL_RECORD_RESERVED_BIT   5
#define CDMA_2000_PILOT_LIST_CHANNEL_RECORD_LENGTH_WIDTH   8
#define CDMA_2000_PILOT_LIST_REFERENCE_PILOT_WIDTH         1
#define CDMA_2000_PILOT_LIST_PILOT_PN_INFO_WIDTH               15
#define CDMA_2000_PILOT_LIST_PILOT_STRNGTH_WIDTH                6
#define CDMA_2000_PILOT_LIST_PILOT_ONE_WAY_DELAY_INCLUDED_WIDTH 1
#define CDMA_2000_PILOT_LIST_PILOT_STRENGTH_RESERVED_BIT_WIDTH  1
#define CDMA_2000_PILOT_LIST_PILOT_ONE_WAY_DELAY_WIDTH         16 
/* PILOT LIST END*/
#define MAX_SIZE_OCTET_BUFF                                128
#define CSFB_BITMASK_PRESENT_WIDTH                         1
#define CSFB_PARAM_1XRTT_MULTI_SIDS_NBITS                  1
#define CSFB_PARAM_1XRTT_MULTI_NIDS_NBITS                  1
/* Bug 12190 Fix Start */
#define CSFB_PARAM_1XRT_RECORD_TYPE_NBITS                  8
#define CSFB_PARAM_1XRTT_PACKET_ZONE_ID_NBITS              8
/* Bug 12190 Fix Stop */
#define CSFB_PARAM_1_XRTT_NID_NBITS                        15 
#define CSFB_PARAM_1XRTT_PZ_HYST_ENABLED_NBITS             1
#define CSFB_PARAM_1XRTT_PZ_HYST_INFO_INCL_NBITS           1
#define CSFB_PARAM_1XRTT_PZ_HYST_LIST_LEN_NBITS            1
#define CSFB_PARAM_1XRTT_PZ_HYST_ACT_TIMER_NBITS           8
#define CSFB_PARAM_1XRTT_PZ_HYST_TIMER_MUL_NBITS           3
#define CSFB_PARAM_1XRTT_PZ_HYST_TIMER_EXP_NBITS           5
#define CSFB_PARAM_1XRTT_P_REV_NBITS                       8
#define CSFB_PARAM_1XRTT_MIN_P_REV_NBITS                   8
#define CSFB_PARAM_1XRTT_NEG_SLOT_CYCLE_INDEX_SUP_NBITS    1
#define CSFB_PARAM_1XRTT_ENCRYPT_MODE_NBITS                2
#define CSFB_PARAM_1XRTT_ENC_SUPPORTED_NBITS               1
#define CSFB_PARAM_1XRTT_SIG_ENCRYPT_SUP_NBITS             8
#define CSFB_PARAM_1XRTT_MSG_INTEGRITY_SUP_NBITS           1
#define CSFB_PARAM_1XRTT_SIG_INTEGRITY_SUP_INCL_NBITS      1
#define CSFB_PARAM_1XRTT_SIG_INTEGRITY_SUP_NBITS           8
#define CSFB_PARAM_1XRTT_AUTH_NBITS                        2
#define CSFB_PARAM_1XRTT_MAX_NUM_ALT_SO_NBITS              3
#define CSFB_PARAM_1XRTT_USE_SYNC_ID_NBITS                 1
#define CSFB_PARAM_1XRTT_MS_INIT_POS_LOC_SUP_IND_NBITS     1
#define CSFB_PARAM_1XRTT_MOB_QOS_NBITS                     1
#define CSFB_PARAM_1XRTT_BAND_CLASS_INFO_REQ_NBITS         1
#define CSFB_PARAM_1XRTT_BAND_CLASS_NBITS                  5
#define CSFB_PARAM_1XRTT_BYPASS_REG_IND_NBITS              5
#define CSFB_PARAM_1XRTT_ALT_BAND_CLASS_NBITS              5
#define CSFB_PARAM_1XRTT_MAX_ADD_SERV_INSTANCE_NBITS       3
#define CSFB_PARAM_1XRTT_HOME_REG_NBITS                    1
#define CSFB_PARAM_1XRTT_FOR_SID_REG_NBITS                 1
#define CSFB_PARAM_1XRTT_FOR_NID_REG_NBITS                 1
#define CSFB_PARAM_1XRTT_POWER_UP_REG_NBITS                1
#define CSFB_PARAM_1XRTT_POWER_DOWN_REG_NBITS              1
#define CSFB_PARAM_1XRTT_PARAMETER_REG_NBITS               1
#define CSFB_PARAM_1XRTT_REG_PRD_NBITS                     7
#define CSFB_PARAM_1XRTT_REG_DIST_NBITS                   11
#define CSFB_PARAM_1XRTT_PREF_MSID_TYPE_NBITS              2
#define CSFB_PARAM_1XRTT_EXT_PREF_MSID_TYPE_NBITS          2
#define CSFB_PARAM_1XRTT_MEID_REQD_NBITS                   1
#define CSFB_PARAM_1XRTT_MCC_NBITS                        10
#define CSFB_PARAM_1XRTT_IMSI_11_12_NBITS                  7
#define CSFB_PARAM_1XRTT_IMSI_T_SUPPORTED_NBITS            1
#define CSFB_PARAM_1XRTT_RECONNECT_MSG_IND_NBITS           1
#define CSFB_PARAM_1XRTT_RER_MODE_SUPPORTED_NBITS          1
#define CSFB_PARAM_1XRTT_TKZ_MODE_SUPPORTED_NBITS          1
#define CSFB_PARAM_1XRTT_TKZ_ID_NBITS                      1
#define CSFB_PARAM_1XRTT_PILOT_REPORT_NBITS                1
#define CSFB_PARAM_1XRTT_SDB_SUPPORTED_NBITS               1
#define CSFB_PARAM_1XRTT_AUTO_FCSO_ALLOWED_NBITS           1
#define CSFB_PARAM_1XRTT_SDB_IN_RCNM_IND_NBITS             1
#define CSFB_PARAM_1XRT_FPC_FCH_INIT_SETPT_RC3_NBITS       8
#define CSFB_PARAM_1XRT_FPC_FCH_INIT_SETPT_RC4_NBITS       8
#define CSFB_PARAM_1XRT_FPC_FCH_INIT_SETPT_RC5_NBITS       8
/* Bug 12674 Fix Start */
#define CSFB_PARAM_1XRT_FPC_FCH_INIT_SETPT_RC11_NBITS      8
#define CSFB_PARAM_1XRT_FPC_FCH_INIT_SETPT_RC12_NBITS      8
/* Bug 12674 Fix Stop */
#define CSFB_PARAM_1XRT_T_ADD_NBITS                        6
#define CSFB_PARAM_1XRT_PILOT_INC_NBITS                    4
#define CSFB_PARAM_1XRT_LP_SEC_NBITS                       8
/* Bug 12190 Fix Start */
#define CSFB_PARAM_1XRT_LTM_OFF_NBITS                      6
/* Bug 12190 Fix Stop */
/* Bug 12674 Fix Start */
#define CSFB_PARAM_1XRT_LTM_OFF_VALUE                      18
/* Bug 12674 Fix Stop */
#define CSFB_PARAM_1XRT_DAYLT_NBITS                        1
#define CSFB_PARAM_1XRT_GCSNAL2ACKTIMER_NBITS              8
#define CSFB_PARAM_1XRT_GCSNASEQUENCECONTEXTTIMER_NBITS    8
#define HO_EUTRA_PREP_REQ_NBITS 32
/* Bug 12190 Fix Start */
#define CSFB_PARAM_1XRT_RAND_NBITS                         32
/* Bug 12190 Fix Stop */
/* CDMA2000_CSFB_HO stop*/
/*SPR_19166_START*/
#define MAX_CELL_IDENTITY_NUMBYTES 4
/*SPR_19166_END*/
/*sctp parameters start*/
#define RRC_SCTP_DEFAULT_RTO_INITIAL 3000
#define RRC_SCTP_DEFAULT_RTO_MAX 6000
#define RRC_SCTP_DEFAULT_RTO_MIN 1000
#define RRC_SCTP_DEFAULT_ASSOC_MAX_RTX 10
#define RRC_SCTP_DEFAULT_VAL_COOKIE_LIFE 60000
#define RRC_SCTP_DEFAULT_PATH_MAX_RTX 5
#define RRC_SCTP_DEFAULT_MAX_INIT_RTX 8
#define RRC_SCTP_DEFAULT_INIT_TIMEOUT 0
/*sctp parameters stop*/
/*sampling period changes */
#define RRC_CONVERT_TO_MILLISEC 1000
/* SPR 23209 FIXED START */
#define MAX_PLMN_IDS               6
#define MAX_MOCN_OPERATOR_ID       5
/* SPR 23209 FIXED END */

/******************************************************************************
*   Interface types
******************************************************************************/

#pragma pack(push, 1)

/******************************************************************************
*   These types are shared CSC-LLIM with PHY and RRM
******************************************************************************/
typedef struct _rrc_phy_phich_configuration_t
{
    U8  phich_resource;     /*^ M, 0, H, 0, 3 ^*/ /* phich_resource_et */
    U8  phich_duration;     /*^ M, 0, H, 0, 1 ^*/ /* phich_duration_et */
} rrc_phy_phich_configuration_t;

typedef struct _rrc_phy_pucch_configuration_t
{
    U8  delta_pucch_shift;      /*^ M, 0, B, 1, 3 ^*/ /* pucch_delta_shift_et */
    U8  nrb_cqi;                /*^ M, 0, H, 0, 98 ^*/
    U8  ncs_an;                 /*^ M, 0, H, 0, 7 ^*/
    U16 n1pucch_an;             /*^ M, 0, H, 0, 2047 ^*/
} rrc_phy_pucch_configuration_t;

typedef struct{
  U8      group_hopping_enabled;  /*^ M, 0, H, 0, 1 ^*/     /* rrc_bool_et */
  U8      group_assign_pusch;     /*^ M, 0, H, 1, 29 ^*/
  U8      seq_hopping_enabled;    /*^ M, 0, H, 0, 1 ^*/     /* rrc_bool_et */
  U8      cyclic_shift;           /*^ M, 0, H, 1, 7 ^*/
}ul_ref_signals_pusch_t;

typedef struct _rrc_phy_pusch_configuration_t
{
    U8  pusch_hopping_offset; /*^ M, 0, H, 0, 98 ^*/
    U8  num_of_sub_bands;       /*^ M, 0, B, 1, 4 ^*/
    U8  pusch_hopping_mode;
/*^ M, 0, H, 0, 1 ^*/ /* pusch_hopping_mode_et */
    ul_ref_signals_pusch_t  ul_ref_signal;
} rrc_phy_pusch_configuration_t;

typedef struct _rrc_phy_prach_configuration_t
{
    U8  prach_config_sequence;  /*^ M, 0, H, 0, 63 ^*/
    U16 root_seq_index;         /*^ M, 0, H, 0, 837 ^*/
    U8  prach_freq_offset;      /*^ M, 0, H, 0, 104 ^*/
    U8  zero_cor_zone_config;   /*^ M, 0, H, 0, 15 ^*/
    U8  high_speed_flag;        /*^ M, 0, H, 0, 1 ^*/ /* high_speed_flag_et */
} rrc_phy_prach_configuration_t;

/* eMTC changes start */
typedef struct _RrcMacEmtcSfn_t
{
     rrc_sfn_t   sfn;
     /*^ M, 0, H, 0, 1023 ^*/
}RrcMacEmtcSfn_t;

/*! \RrcMacSfnGapBr_t
 *  \brief This structure contains the br sfn gap.
 *  \param sfnGapBr
 *         indicates the sfn fap br. 
 */
typedef struct _RrcMacSfnGapBr_t
{
    U8    sfnGapBr;
    /*^ M, 0, B, 1, EMTC_MAX_SFN_GAP ^*/
}RrcMacSfnGapBr_t;

/*! \RrcMacPagingNarrowbandInfo_t
 *  \brief This structure contains the narrow band to be used for paging.
 *  \param pagingNarrowBand
 *         indicates the narrow band to be used for paging. 
 */
typedef struct _RrcMacPagingNarrowband_t
{
    U8          pagingNarrowBand;
    /*^ M, 0, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/
}RrcMacPagingNarrowband_t;

/*! \RrcMacDirectIndication_t
 *  \brief This structure contains the paging cause.
 *  \param directIndication
 *         indicates the paging cause. 
 */
typedef struct _RrcMacDirectIndication_t
{
    U8          directIndication;
    /*^ M, 0, H, 0, EMTC_DIRECT_INDICATION_SI_MODIF ^*/
    /* EmtcDirectIndication_et */
}RrcMacDirectIndication_t;


/*! \RrcMacMpdcchNumRepetitionPaging_t
 *  \brief This structure contains the number of
 *         repetitions for MPDCCH common search space for paging.
 *  \param mpdcchNumRepetitionPaging
 *         indicates the maximum number of repetitions for
 *         MPDCCH common search space (CSS) for paging and it is
 *         used for BL/CE UEs. This will be mandatory if cell
 *         operating mode is eMTC or mixed.
 */
typedef struct _RrcMacMpdcchNumRepetitionPaging_t
{
    U8          mpdcchNumRepetitionPaging;
    /*^ M, 0, B, 1, EMTC_MPDCCH_NUM_REPETITION_PAGING_MAX ^*/
}RrcMacMpdcchNumRepetitionPaging_t;

/*! \RrcMacPdschNumRepetition_t
 *  \brief This structure contains the number of
 *         repetitions for pdsch for paging. 
 *  \param pdschNumRepetition
 *         indicates the number of repetitions for
 *         PDSCH and it is used for BL/CE UEs. 
 */
typedef struct _RrcMacPdschNumRepetition_t
{
    U8          pdschNumRepetition;
    /*^ M, 0, B, 1, EMTC_PDSCH_PAGING_REP_MAX ^*/
}RrcMacPdschNumRepetition_t;


/*! \RrcN1PucchAnInfoListR13_t
 *  \brief This structure contains starting offset of the PUCCH resource(s). 
 *  \param numCeLevels
 *         indicates number of CE Level
 *  \param n1PucchAnInfo
 *         indicates the starting offsets of the PUCCH resource(s). The first
 *         entry in the list is the starting offset of the PUCCH resource(s)
 *         of CE level 0, the second entry in the list is the starting offset
 *         of the PUCCH resource(s) of CE level 1 and so on
 */
typedef struct _RrcN1PucchAnInfoListR13_t
{
    U8    numCeLevels;
    /*^ M, 0, H, 1, EMTC_MAX_CE_LEVEL ^*/

    U16   n1PucchAnInfo[EMTC_MAX_CE_LEVEL];
    /*^ M, 0, OCTET_STRING, VARIABLE, H, 0, EMTC_MAX_N1_PUCCH_AN ^*/
}RrcN1PucchAnInfoListR13_t;


/*! \RrcPhyPdschConfigV1310_t
 *  \brief This struct contains the PDSCH parameters 
 *         for Release 13
 *   \param pdschMaxNumRepetitionCeModeAR13 
 *         indicates the set of PDSCH repetition numbers 
 *         for CE mode A
 */
typedef struct _RrcPhyPdschConfigV1310_t
{
    U8 pdschMaxNumRepetitionCeModeA;
    /*^ M, 0, H, 0, PHY_PDSCH_NUM_REPETITION_CE_MODE_A_MAX ^*/
    /* RrcPhyPdschMaxNumRepetitionCeModeAR13_et */
}RrcPhyPdschConfigV1310_t;


/*! \RrcPhyPuschConfigV1310_t
 *  \brief This struct contains the PUSCH parameters
 *         for Release 13
 *  \param phyPuschMaxNumRepetitionCeModeA 
 *         indicates PUSCH max number of repetitions 
 *         for CE Mode A
 */
typedef struct _RrcPhyPuschConfigV1310_t
{
    U8 phyPuschMaxNumRepetitionCeModeA ;
    /*^ M, 0, H, 0, PHY_PUSCH_NUM_REPETITION_CE_MODE_A_MAX ^*/
    /* RrcPhyPuschMaxNumRepetitionCeModeAr13_et */
}RrcPhyPuschConfigV1310_t;





/*! \RrcPhyMpdcchNarrowBandToMonitorR13_t
 *  \brief structure contain list regarding narrowbands
 *         to monitor for MPDCCH channel for RAR
 *  \param count
 *         indicates number of mpdcch narrowband to monitor
 *  \param mpdcchNarrowBandiToMonitor
 *         indicates narrowbands indexes
 */
typedef struct _RrcPhyMpdcchNarrowBandToMonitorR13_t
{
    U8 count;
    /*^ M, 0, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND  ^*/

    U8 narrowBand[EMTC_MAX_AVAILABLE_NARROWBAND ];
    /*^ M, 0, OCTET_STRING, VARIABLE, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/
}RrcPhyMpdcchNarrowBandToMonitorR13_t;

/*! \ RrcPhyRsrpThresholdPrachInfoListR13_t
 *  \brief This struct contains the criterion for BL
 *         UEs and UEs in CE to select PRACH resource set
 *  \param phyRsrpThresholdPrachInfoR13 
 *         indicates PRACH resource set for Coverage 
 *         Enhancement
 */
typedef struct _RrcPhyRsrpThresholdPrachInfoListR13_t
{
    U8 count;
    /*^ M, 0, B, 1, EMTC_MAX_RSRP_LIST_SIZE^*/

    U8 rsrpRange[EMTC_MAX_RSRP_LIST_SIZE];
    /*^ M, 0, OCTET_STRING, VARIABLE, H, 0, EMTC_MAX_RSRP_RANGE ^*/ 
}RrcPhyRsrpThresholdPrachInfoListR13_t;

/*! \ RrcMpdcchConfigSetupR13_t
 *  \brief This struct describes the mpdcch config info for release 13 
 *  \param mpdcchPdschHoppingConfigR13
 *         It indicates frequency hopping activation/deactivation for
 *         unicast MPDCCH/PDSCH
 *  \param mpdcchStartSfUessFddR13
 *         It indicates starting subframe configuration for an MPDCCH
 *         UE-specific search space in FDD
 *  \param mpdcchNumRepetitionR13
 *         It indicates maximum numbers of repetitions for UE-SS for MPDCCH
 */
typedef struct _RrcMpdcchConfigSetupR13_t
{
    U8                        mpdcchPdschHoppingConfigR13;
    /*^ M, 0, H, 0, EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX ^*/
    /* EmtcMpdcchPdschHoppingConfig_et */

    U8                        mpdcchStartSfUessFddR13;
    /*^ M, 0, H, 0, EMTC_MPDCCH_START_SF_UESS_FDD_MAX ^*/
    /* EmtcMpdcchStartSfUessFdd_et */

    U8                        mpdcchNumRepetitionR13;
    /*^ M, 0, H, 0, EMTC_MPDCCH_NUM_REPETITION_UESS_MAX ^*/
    /* EmtcMpdcchNumRepetitionUess_et */

}RrcMpdcchConfigSetupR13_t;

/*! \ RrcMpdcchConfigR13_t
 *  \brief This struct describes the configuration of MPDCCH.
 *  \param bitmask
 *         It indicates whether optional fields are present or not 
 *  \param mpdcchConfigType
 *         It indicates whether to release or setup mpdcch
 *         configuration.
 *  \param mpdcchConfigSetupR13
 *         It indicates the configuration of MPDCCH for
 *         setup.
 */

#define RRC_MPDCCH_CONFIG_SETUP_R13_PRESENT 0x01
typedef struct _RrcMpdcchConfigR13_t
{
    RrcBitmaskU8_t                  bitmask;
    /*^ BITMASK ^*/

    U8                              mpdcchConfigType;
    /*^ M, 0, H, 0, 1 ^*/

    RrcMpdcchConfigSetupR13_t       mpdcchConfigSetupR13;
    /*^ O, RRC_MPDCCH_CONFIG_SETUP_R13_PRESENT , N, 0, 0 ^*/
   
}RrcMpdcchConfigR13_t;



 /*  \param numberPrbPairsR11
 *         It indicates the number of physical resource-block pairs
 *         used for the EPDCCH set. Value n2 corresponds to 2 PRB pairs*/

/*! \ NumberPRBPairsSetupR13_t
 *  \brief This struct describes the info of numPRBPairs Setup
 *  \param numberPRBPairsV1310
 *         It indicates the number of physical resource-block pairs
 *         used for the EPDCCH set. Value n2 corresponds to 2 PRB pairs
 */
typedef struct _NumberPRBPairsSetupR13_t
{
    U8                              numberPRBPairsV1310;
    /*^ M, 0, H, 0, RRC_NUM_PRB_PAIR_MAX ^*/
    /* RrcNumPrbPairs_et */

}NumberPRBPairsSetupR13_t;

/*! \ numberPRBPairsV13_t
 *  \brief This struct describes the info of numPRBPairs
 *  \param bitmask
 *         It indicates whether optional fields are present or not 
 *  \param numberPRBPairsType
 *         It indicates whether to release or setup prb pairs
 *  \param numberPRBPairsSetupR13
 *         It indicates the configuration of numPRBPairs
 *         setup.
 */
#define RRC_NUM_PRB_PAIRS_SETUP_R13_PRESENT 0x01
typedef struct _numberPRBPairsV13_t
{
    RrcBitmaskU8_t                  bitmask;
    /*^ BITMASK ^*/

    U8                              numberPRBPairsType;
    /*^ M, 0, H, 0, 1 ^*/

    NumberPRBPairsSetupR13_t        numberPRBPairsSetupR13;
    /*^ O, RRC_NUM_PRB_PAIRS_SETUP_R13_PRESENT , N, 0, 0 ^*/
   
}numberPRBPairsV13_t;

/*! \ RrcEpdcchSetConfigR11_t
 *  \brief This struct describes the EPDCCH configuration set.
 *         E-UTRAN configures at least one EPDCCH-SetConfig when
 *         Note: EPDCCH-Config is configured. For BL UEs or UEs in CE,
 *               EUTRAN does not configure more than one EPDCCH-SetConfig.
 *  \param bitmask
 *         It indicates whether optional fields are present or not 
 *  \param setConfigIdR11
 *         It indicates the identity of the EPDCCH configuration set
 *  \param transmissionTypeR11
 *         It indicates whether distributed or localized EPDCCH
 *         transmission mode is used
 *  \param mpdcchConfigR13
 *         It indicates mpdcchConfig parameters for Release 13
 */

#define RRC_MPDCCH_CONFIG_R13_PRESENT       0x01
typedef struct _RrcEpdcchSetConfigR11_t
{
    RrcBitmaskU8_t                  bitmask;
    /*^ BITMASK ^*/

    U8                              setConfigId;
    /*^ M, 0, H, 0, EMTC_MAX_EPDCCH_CONFIG_ID ^*/

    U8                              transmissionType;
    /*^ M, 0, H, 0, EMTC_TRANSMISSION_TYPE_MAX ^*/
    /* EmtcTransmissionType_et */

    U8                              numPrbPairs;
    /*^ M, 0, H, 0, EMTC_NUM_PRB_PAIR_MAX ^*/
    /* EmtcNumPrbPairs_et */

    U8                              prbBitmap;
    /*^ M, 0, H, 0, 63 ^*/

    RrcMpdcchConfigR13_t         mpdcchConfigR13;
    /*^ O, RRC_MPDCCH_CONFIG_R13_PRESENT , N, 0, 0 ^*/

}RrcEpdcchSetConfigR11_t;


/*! \ StartSymbolR11_t
 *  \brief This struct describes the starting symbol of EPDCCH Config
 *  \param startSymbolR11
 *         indicates the OFDM starting symbol for EPDCCH
 */

typedef struct _StartSymbolR11_t
{
    U8                          startSymbolR11;
    /*^ M, 0, B, 1, EMTC_MAX_START_SYMBOL ^*/
}StartSymbolR11_t;

/*! \ EpdcchConfigSetup_t
 *  \brief This struct describes the EPDCCH config info
 *  \param bitmask
 *         It indicates whether optional fields are present or not 
 *  \param startSymbolR11
 *         indicates the OFDM starting symbol for EPDCCH
 *  \param epdcchSetConfigToAddModListR11
 *         It indicates EPDCCH Configuration Set to add mod list
 */

#define RRC_EPDCCH_CONFIG_START_SYMBOL_PRESENT  0x01
#define RRC_EPDCCH_CONFIG_ADD_MOD_LIST_PRESENT  0x02
typedef struct _EpdcchConfigSetup_t
{
    RrcBitmaskU8_t                  bitmask;
    /*^ BITMASK ^*/

    U8                              startSymbolR11;
    /*^ O, RRC_EPDCCH_CONFIG_START_SYMBOL_PRESENT, B, 1, EMTC_MAX_START_SYMBOL ^*/

    RrcEpdcchSetConfigR11_t         epdcchSetConfigR11;
    /*^ O, RRC_EPDCCH_CONFIG_ADD_MOD_LIST_PRESENT , N, 0, 0 ^*/

}EpdcchConfigSetup_t;


/*! \ RrcAckNackRepetitionSetupR13_t
 *  \brief This struct describes whether the ACK/NACK repetition is configured
 *  \param repetitionFactorR13
 *         It indicates UE-specific PUCCH repetition factor for release 13
 *  \param n1PucchAnRepR13
 *         It indicates UE-specific PUCCH AN resource offset
 */
typedef struct _RrcAckNackRepetitionSetupR13_t
{
    U8    repetitionFactorR13;
    /*^ M, 0, H, 0, EMTC_REPETITION_FACTOR_MAX ^*/
    /* EmtcRepetitionFactor_et */

    U16   n1PucchAnRepR13;
    /*^ M, 0, H, 0, EMTC_MAX_N1_PUCCH_AN^*/

}RrcAckNackRepetitionSetupR13_t;

/*! \RrcPucchNumRepetitionCeR13SetupModeA_t 
 *  \brief This struct describes the PUCCH dedicated configuration for
 *         release 13
 *  \param numRepetitionCeModeAFormat1
 *         It indicates the PUCCH format 1 for CE mode A
 *  \param numRepetitionCeModeAFormat2
 *         It indicates the PUCCH format 2 for CE mode A
 */
typedef struct _RrcPucchNumRepetitionCeR13SetupModeA_t
{
    U8    numRepetitionCeModeAFormat1;
    /*^ M, 0, H, 0, EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX ^*/
    /* EmtcPucchNumRepetitionCeModeAFormat1_et */

    U8    numRepetitionCeModeAFormat2;
    /*^ M, 0, H, 0, EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT2_MAX ^*/
    /* EmtcPucchNumRepetitionCeModeAFormat2_et */

}RrcPucchNumRepetitionCeR13SetupModeA_t;

/* RrcMacPucchAckNackRepetitionR13_t
 *  \brief This struct describes the PUCCH Ack Nack repetition
 *  \param bitmask
 *         It indicates whether optional fields are present or not 
 *  \param ackNackRepetitiontype
 *         It indicates type of PUCCH Ack Nack repetition
 *  \param ackNackRepetitionSetup
 *         It indicates PUCCH Ack Nack repetition Setup 
 */
#define RRC_MAC_PUCCH_ACK_NACK_REPETITION_SETUP_R13_PRESENT  0x01
typedef struct _RrcMacPucchAckNackRepetitionR13_t
{
    RrcBitmaskU8_t                      bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                                  ackNackRepetitiontype;
     /*^ M, 0, H, 0, 1 ^*/

    RrcAckNackRepetitionSetupR13_t      ackNackRepetitionSetup;
    /*^ TLV, RRC_MAC_ACK_NACK_REPETITION_SETUP_R13, RRC_MAC_PUCCH_ACK_NACK_REPETITION_SETUP_R13_PRESENT ^*/

}RrcMacPucchAckNackRepetitionR13_t;

/* RrcPucchAckNackRepetitionR13_t
 *  \brief This struct describes the PUCCH Ack Nack repetition
 *  \param bitmask
 *         It indicates whether optional fields are present or not 
 *  \param type
 *         It indicates type of PUCCH Ack Nack repetition
 *  \param ackNackRepetitionSetup
 *         It indicates PUCCH Ack Nack repetition Setup 
 */
#define RRC_PUCCH_NUM_REPETITION_CE_SETUP_R13_PRESENT 0x01
typedef struct _RrcPucchNumRepetitionCeR13_t
{
    RrcBitmaskU8_t                              bitmask;
    /*^ BITMASK ^*/
    
    U8                                          type;
     /*^ M, 0, H, 0, 1 ^*/

    RrcPucchNumRepetitionCeR13SetupModeA_t      numRepetitionSetup;
    /*^ O, RRC_PUCCH_NUM_REPETITION_CE_SETUP_R13_PRESENT, N, 0, 0 ^*/

}RrcPucchNumRepetitionCeR13_t;

/*! \ RrcPucchConfigDedicatedR13_t
 *  \brief This struct describes the PUCCH Ack Nack repetition
 *  \param bitmask
 *         It indicates whether optional fields are present or not 
 *  \param type
 *         It indicates type of PUCCH Ack Nack repetition
 *  \param ackNackRepetitionSetup
 *         It indicates PUCCH Ack Nack repetition Setup 
 */
#define RRC_PUCCH_ACK_NACK_REPETITION_SETUP_R13_PRESENT  0x01
#define RRC_PUCCH_NUM_REPETITION_CE_FORMAT1_PRESENT      0x02
typedef struct _RrcPucchConfigDedicatedR13_t
{
    RrcBitmaskU8_t                     bitmask;
    /*^ BITMASK ^*/

    U8                                 type;
     /*^ M, 0, H, 0, 1 ^*/

    RrcAckNackRepetitionSetupR13_t     ackNackRepetitionSetup;
    /*^ O, RRC_PUCCH_ACK_NACK_REPETITION_SETUP_R13_PRESENT, N, 0, 0 ^*/
    U8                                 pucchNumRepetitionCeFormat1r13;
    /*^ O, RRC_PUCCH_NUM_REPETITION_CE_FORMAT1_PRESENT, H, 0, EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX ^*/
    /* EmtcPucchNumRepetitionCeModeAFormat1_et */
}RrcPucchConfigDedicatedR13_t;



/*! \EpdcchConfig_t
 *  \brief This struct describes the EPDCCH configuration for eMTC.
 *  \param bitmask
 *         It indicates whether optional fields are present or not 
 *  \param epdcchConfigType
 *         It indicates whether to release or setup epdcch
 *         configuration.
 *  \param epdcchConfigSetup
 *         It indicates the configuration of EPDCCH is for
 *         setup.
 */
#define RRC_EPDCCH_CONFIG_SETUP_R13_PRESENT    0x01
typedef struct _EpdcchConfig_t
{
    RrcBitmaskU8_t                              bitmask;
    /*^ BITMASK ^*/

    U8                                          epdcchConfigType;
    /*^ M, 0, H, 0, 1 ^*/

    EpdcchConfigSetup_t                         epdcchConfigSetup;
    /*^ O, RRC_EPDCCH_CONFIG_SETUP_R13_PRESENT , N, 0, 0 ^*/
     
}EpdcchConfig_t;


/*! \RrcPhysicalConfigDedicatedR13_t
 *  \brief This struct contains the physcial config dedicated
 *         configuration of release 13. 
 *  \param epdcchConfig
 *         It indicates the EPDCCH config info.
 *  \param pucchConfigDedicatedR13
 *         It indicates the pucch dedicated configurtaion for release 13.
 *  \param ceMode
 *         It indicates the CE mode of eMTC UE.
 */
typedef struct _RrcPhysicalConfigDedicatedR13_t
{
    EpdcchConfig_t                  epdcchConfig;
    /*^ M, 0, N, 0, 0 ^*/

    RrcPucchConfigDedicatedR13_t    pucchConfigDedicatedR13;
    /*^ M, 0, N, 0, 0 ^*/

    U8                              ceMode;
    /*^ M, 0, H, 0, EMTC_CE_MODE_MAX ^*/
    /* EmtcCeMode_et */
}RrcPhysicalConfigDedicatedR13_t;



/*! \RrcPhyPrachParametersCeR13_t
 *  \brief This struct contains MTC PRACH configuration 
 *         parameters sending to phy
 *  \param bitmask 
 *         To check optional field is present or not
 *  \param ceLevel
 *         indicates CE level for RACH.
 *  \param phyPrachConfigIndexR13 
 *         indicates prach-ConfigurationIndex 
 *  \param phyPrachFreqOffsetR13 
 *         indicates prach frequency offset
 *  \parma numRepetitionPerPreambleAttemptR13
 *         indicates the number of PRACH repetitions per attempt
 *         for each CE level.
 *  \param phyPrachHoppingConfigR13 
 *         indicates frequency hopping configuration for PRACH 
 *  \param prachStartSfR13 
 *         indicates PRACH starting subframe periodicity 
 *  \param prachHoppingOffsetR13 
 *         indicate information about prach hopping offset 
 */

#define RRC_PHY_PRACH_START_SF_R13_PRESENT              0x01
#define RRC_PHY_PRACH_HOPPING_OFFSET_R13_PRESENT        0x02
typedef struct _RrcPhyPrachParametersCeR13_t
{
    RrcBitmaskU8_t     bitmask;
    /*^ BITMASK ^*/

    U8                 ceLevel;
    /*^ M, 0, H, 0, EMTC_CE_LEVEL_MAX ^*/
    /* EmtcCeLevel_et */ 

    U8                 prachConfigIndexR13;
    /*^ M, 0, H, 0, EMTC_MAX_PRACH_CONFIG_INDEX ^*/

    U8                 prachFreqOffsetR13;
    /*^ M, 0, H, 0, EMTC_MAX_PRACH_FREQ_OFFSET ^*/

    U8                 numRepetitionPerPreambleAttemptR13; 
    /*^ M, 0, H, 0, EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX ^*/
    /* EmtcNumRepetitionPerPreambleAttempt_et */

    U8                 prachHoppingConfigR13;
    /*^ M, 0, H, 0, EMTC_PRACH_HOPPING_CONFIG_MAX ^*/
    /* EmtcPrachHoppingConfig_et */

    U8                 prachHoppingOffsetR13;
    /*^ O, RRC_PHY_PRACH_HOPPING_OFFSET_R13_PRESENT, H, 0, EMTC_MAX_PRACH_HOPPING_OFFSET ^*/

    U8                 prachStartSfR13;
    /*^ O, RRC_PHY_PRACH_START_SF_R13_PRESENT, H, 0, EMTC_PRACH_STARTING_SF_MAX ^*/
    /* EmtcPrachStartingSubframe_et */

}RrcPhyPrachParametersCeR13_t;

/*! \RrcPhyPrachParametersListCeR13_t
 *  \brief This struct contain the list of MTC 
 *         PRACH parameters for each CE level.
 *  \param numCeLevels
 *         indicates number of CE level. 
 *  \param prachParametersCeR13 
 *         indicates PRACH parameters info for each 
 *         CE level
 */
typedef struct _RrcPhyPrachParametersListCeR13_t
{
    U8                              numCeLevels;
    /*^ M, 0, B, 1, EMTC_MAX_CE_LEVEL ^*/

    RrcPhyPrachParametersCeR13_t    prachParametersCeR13[EMTC_MAX_CE_LEVEL];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}RrcPhyPrachParametersListCeR13_t;

/*! \RrcPhyPrachHoppingOffsetR13_t
 *  \brief This struct contains information about 
 *         prach hopping offset
 *  \param phyPrachHoppingOffsetR13 
 *         indicates PRACH frequency hopping offset
 */
typedef struct _RrcPhyPrachHoppingOffsetR13_t
{
    U8 phyPrachHoppingOffsetR13;
    /*^ M, 0, H, 0, EMTC_MAX_PRACH_HOPPING_OFFSET ^*/		
}RrcPhyPrachHoppingOffsetR13_t;

/*! \RrcPrachHoppingOffsetR13_t
 *  \brief This struct contains information about 
 *         prach hopping offset
 *  \param prachHoppingOffsetR13 
 *         indicates PRACH frequency hopping offset,
 *         expressed as a number of resource blocks.
 *         Note:In phase1,Hopping is not supported for RACH.
 */
typedef struct _RrcPrachHoppingOffsetR13_t
{
    U8    prachHoppingOffsetR13;
    /*^ M, 0, H, 0, EMTC_MAX_PRACH_HOPPING_OFFSET ^*/		
}RrcPrachHoppingOffsetR13_t;

/*! \RrcPhyPrachConfigV1310_t
 *  \brief This struct contain PRACH parameters 
 *         for Release 13
 *  \param prachParametersListCeR13 
 *         indicate the list of MTC PRACH parameters 
 *         for each CE level
 */
typedef struct _RrcPhyPrachConfigV1310_t
{

    RrcPhyPrachParametersListCeR13_t      prachParametersListCe;
    /*^ M, 0, N, 0, 0 ^*/
}RrcPhyPrachConfigV1310_t;



/*! \RrcMacFddDownlinkOrTddSfBitmapBr_t
 *  \brief This struct is used to describe the set of 
 *         valid subframes for FDD downlink or TDD 
 *         transmissions
 *  \param fddDownlinksubframeBitmapBr 
 *         indicates subframe pattern in 10 or 40 bit string
 */
typedef struct _RrcMacFddDownlinkOrTddSfBitmapBr_t
{
    U8 fddDownlinksubframeBitmapBr[EMTC_MAX_DL_SF_PATTERN_BYTES];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    /*10/40 bit string*/	
}RrcMacFddDownlinkOrTddSfBitmapBr_t;


/*! \RrcMacFddUplinkSfBitmapBr_t
 *  \brief This struct is used to describe the set of 
 *         valid subframes for FDD uplink transmissions 
 *         for BL UEs
 *  \param fddUplinkSfBitmapBr 
 *         indicates bitmap for uplink subframe for fdd 
 *         transmission 
 */
typedef struct _RrcMacFddUplinkSfBitmapBr_t
{
    U16    fddUplinkSubframeBitmapBr;
    /*^ M, 0, H, 0, EMTC_MAX_FDD_UPLINK_SF_BITMAP ^*/ 
    /* 10 bit string */
}RrcMacFddUplinkSfBitmapBr_t;



/*! \RrcMacSibtype1BrMsgInfo_t
 *  \brief structure contain parameters for SIB1 BR brodcast 
 *         Message Request
 *  \param sfn
 *         It indicates  the target SFN of the SIB Type1-BR message
 *  \param startSymbolBrR13 
 *         indicates the OFDM starting symbol for any MPDCCH
 *  \param sibtype1BrMsgBufCounter
 *         indicates length  of sib1 br messages buffer
 *  \param sibtype1BrMsgBuf 
 *         indicates sib1 br buffer
 */
typedef struct _RrcMacSibtype1BrMsgInfo_t
{
    RrcSfn_t                              sfn;
    /*^ M, 0, H, 0, EMTC_MAX_STARTING_SFN ^*/

    U8 			                          startSymbolBrR13;
    /*^ M, 0, B, 1, EMTC_MAX_START_SYMBOL ^*/ 

    RrcCounter_t   	                      sibtype1BrMsgBufCounter;
    /*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/

    U8              	                  sibtype1BrMsgBuf[EMTC_MAX_SIB1_BR_LENGTH];
    /*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/
	
}RrcMacSibtype1BrMsgInfo_t;


/*! \rrcPrachStartSfR13_t
 *  \brief structure indicates RACH starting subframe periodicity,
 *         expressed in number of subframes available for preamble
 *         transmission
 *  \param prachStartingSubframeR13 
 *         indicates the rach starting subframe periodicity, expressed
 *         in number of subframes available for preamble transmission 
 */
typedef struct _RrcPrachStartSfR13_t
{
    U8 prachStartingSubframeR13;
    /*^ M, 0, H, 0, EMTC_PRACH_STARTING_SF_MAX ^*/
    /* EmtcPrachStartingSubframe_et */
}RrcPrachStartSfR13_t;


/*! \RrcMaxNumPreambleAttemptCeR13_t
 *  \brief structure contains info regarding 
 *         maximum number of preamble attempts per CE level
 *  \param maxNumPreambleAttemptCeR13 
 *         indicates preamble attempts per CE level
 */
typedef struct _RrcMaxNumPreambleAttemptCeR13_t
{
    U8 maxNumPreambleAttemptCeR13;
    /*^ M, 0, H, 0, EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_MAX ^*/
    /* EmtcMaxNumPreambleAttemptCe_et */
}RrcMaxNumPreambleAttemptCeR13_t;


/*! \RrcPrachParametersCeR13_t
 *  \brief This struct contains RACH information
 *         for each CE level
 *  \param bitmask 
 *         to check optional field is present or not
 *  \param ceLevel
 *         indicates CE level for RACH.
 *  \param prachConfigIndexR13 
 *         indicates prach-ConfigurationIndex 
 *  \param prachFreqOffsetR13 
 *         indicates prach frequency offset
 *         Note: Frequency offset shall be in multiples of 6, e.g it can be 0,
 *               6, 12, 18, 24 for PRACH channel to lie in one narrowband and
 *               not to overlap with different narrowbands.
 *  \param numRepetitionPerPreambleAttemptR13
 *         indicates the number of PRACH repetitions per attempt
 *         for each CE level.
 *  \param numMpdcchNarrowbandsToMonitorR13
 *         indicates the number of narrow bands to monitor for MPDCCH. 
 *  \param mpdcchNarrowBandIndex 
 *         indicates the narrowband index where MPDCCH  is mapped.
 *  \param mpdcchNumRepetitionsRaR13 
 *         indicates maximum number of repetitions for 
 *         MPDCCH common search space for RAR, Msg3 and Msg4. 
 *         Note : r1 value is supported for mpdcch-NumRepetition-RA-r13
 *                in phase 1
 *  \param prachHoppingConfigR13 
 *         indicates the coverage level specific frequency hopping
 *         configuration for PRACH
 *  \param prachStartSfR13 
 *         indicates the rach starting subframe periodicity, expressed in
 *         number of subframes available for preamble transmission 
 *  \param maxNumPreambleAttemptCeR13 
 *         indicates maximum number of preamble transmission
 *         attempts per CE level
 */

#define RRC_MAC_PRACH_START_SF_R13_PRESENT                 0x01
#define RRC_MAC_MAX_NUM_PREAMBLE_ATTEMPT_CE_R13_PRESENT    0x02
typedef struct _RrcPrachParametersCeR13_t
{
    RrcBitmaskU8_t                     bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                                 ceLevel;
    /*^ M, 0, H, 0, EMTC_MAX_CE_LEVEL ^*/ 

    U8                                 prachConfigIndexR13;
    /*^ M, 0, H, 0, EMTC_MAX_PRACH_CONFIG_INDEX ^*/

    U8                                 prachFreqOffsetR13;
    /*^ M, 0, H, 0, EMTC_MAX_PRACH_FREQ_OFFSET ^*/

    U8                                 numRepetitionPerPreambleAttemptR13; 
    /*^ M, 0, H, 0, EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX ^*/
    /* EmtcNumRepetitionPerPreambleAttempt_et */

    U8                                 numMpdcchNarrowbandsToMonitorR13;
    /*^ M, 0, B, 1, EMTC_MAX_NARROWBAND_TO_MONITOR ^*/

    U8                                 mpdcchNarrowBandIndex[EMTC_MAX_NARROWBAND_TO_MONITOR];
    /*^ M, 0, OCTET_STRING, VARIABLE, H, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/

    U8                                 mpdcchNumRepetitionsRaR13;
    /*^ M, 0, H, 0, EMTC_MPDCCH_NUM_REPETITION_RA_MAX ^*/
    /* EmtcMpdcchNumRepetitionRa_et */

    U8                                 prachHoppingConfigR13;
    /*^ M, 0, H, 0, EMTC_PRACH_HOPPING_CONFIG_MAX ^*/
    /* EmtcPrachHoppingConfig_et */

    RrcPrachStartSfR13_t               prachStartingSubframeR13;
    /*^ TLV, RRC_MAC_PRACH_STARTING_SUBFRAME_R13, RRC_MAC_PRACH_START_SF_R13_PRESENT ^*/

    RrcMaxNumPreambleAttemptCeR13_t    maxNumPreambleAttemptCeR13;
    /*^ TLV, RRC_MAC_MAX_NUM_PREAMBLE_ATTEMPT_CE_R13, RRC_MAC_MAX_NUM_PREAMBLE_ATTEMPT_CE_R13_PRESENT ^*/
}RrcPrachParametersCeR13_t;

/*! \RrcEmtcDataConfig_t
 *  \brief This struct contain configuration parameters for eMTC
 *  \param sib1BrIndex
 *         indicates the sib1 BR index
 *  \param numUeDedicatedNarrowbands
 *         indicates the number of UE dedicated narrow bands
 *  \param ueDedicatedNarrowbands
 *         indicates the value of UE dedicated narrow bands
 */
typedef struct _RrcEmtcDataConfig_t
{
    U8    sib1BrIndex;
    /*^ M, 0, H, 0, EMTC_MAX_SIB1_BR_INDEX  ^*/ 

    U8    numUeDedicatedNarrowbands;
    /*^ M, 0, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND  ^*/

    U8    ueDedicatedNarrowbands[EMTC_MAX_AVAILABLE_NARROWBAND];
    /*^ M, 0, OCTET_STRING, VARIABLE, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND  ^*/

}RrcEmtcDataConfig_t;

/*! \RrcMacPrachConfigV1310_t
 *  \brief This struct contain RACH configuration release 13
 *         parameters for eMTC 
 *  \param bitmask 
 *         to check optional field is present or not
 *  \param mpdcchStartSfCssRaType 
 *         the type of configuration i.e FDD or TDD
 *  \param mpdcchStartSfCssRa
 *         indicates fdd subframe configuration for MPDCCH_START_SF_CSS_RA
 *         for phase 1
 *  \param numCeLevels
 *         the number of CE Levels
 *  \param prachParametersCeR13 
 *         indicate the rach information for each CE level
 *  \param prachHoppingOffsetR13 
 *         indicate information about prach hopping offset
 *         Note: In phase1 ,hopping is not supported. 
 */

#define RRC_PRACH_HOPPING_OFFSET_R13_PRESENT       0x01
typedef struct _RrcMacPrachConfigV1310_t
{
    RrcBitmaskU8_t    	               bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                                 mpdcchStartSfCssRaType;
    /*^ M, 0, H, 0, RRC_ONE ^*/

    U8                                 mpdcchStartSfCssRa;
    /*^ M, 0, H, 0, EMTC_MPDCCH_START_SF_CSS_RA_FDD_MAX ^*/
    /* EmtcMpdcchStartSfCssRaFdd_et */
 
    U8                                 numCeLevels;
    /*^ M, 0, B, 1, EMTC_MAX_CE_LEVEL ^*/ 

    RrcCounter_t                       numCeLevelsCounter;
    /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    RrcPrachParametersCeR13_t          prachParametersCeR13[EMTC_MAX_CE_LEVEL];
    /*^ TLV, SEQUENCE, RRC_MAC_PRACH_PARAMETERS_CE_R13_LIST ^*/

    RrcPrachHoppingOffsetR13_t         prachHoppingOffsetR13;
    /*^ TLV, RRC_MAC_PRACH_HOPPING_OFFSET_R13, RRC_PRACH_HOPPING_OFFSET_R13_PRESENT ^*/

}RrcMacPrachConfigV1310_t;

/*! \ RrcMacModificationPeriodV1310_t
 *  \brief This struct informs the actual modification period, expressed
 *         in number of radio frames= modificationPeriodCoeff * defaultPagingCycle.
 *  \param modificationPeriodV1310
 *         Indicates the acutal Modification period and it is expressed in number of
 *         radio frames, (modificationPeriodCoeff-v1310 * defaultPagingCycle-v1310). 
 */
typedef struct _RrcMacModificationPeriodV1310_t
{
    U32    modificationPeriodV1310;
    /*^ M, 0, B, 64, EMTC_MAX_MOD_PERIOD ^*/ 
}RrcMacModificationPeriodV1310_t;

/*! \ RrcMacSfnGapInfoV1310_t
 *  \brief This struct informs the isfn gap v1310 for Release 13 UES.
 *  \param sfnGapV1310
 *         indicates the sfn gap of eMTC UEs
 */
typedef struct _RrcMacSfnGapInfoV1310_t
{
    U16       sfnGapV1310;
    /*^ M, 0, B, 1, EMTC_SFN_GAP_MAX ^*/
}RrcMacSfnGapInfoV1310_t;

/*! \ RrcMacPcchConfigV1310_t
 *  \brief This struct informs the PCCH-Config-v1310 for Release 13 UES.
 *  \param pagingNarrowBandsR13
 *         indicates number of narrowbands used for paging
 *  \param mpdcchNumRepetitionPagingR13
 *         indicates maximum number of repetitions for MPDCCH common 
 *         search space (CSS) for paging
 *  \param nBV1310
 *         indicates the value of nB is used in the calculation of Ns for v13 UEs.
 *  \param sfnGapV1310
 *         indicates the value of sfn gap for eMTC UEs.
 */
typedef struct _RrcMacPcchConfigV1310_t
{
    U8        pagingNarrowBandsR13;
    /*^ M, 0, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/

    U8        mpdcchMaxNumRepetitionPagingR13;
    /*^ M, 0, H, 0, EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_MAX ^*/
    /* EmtcMpdcchMaxNumRepetitionPaging_et */

    U8        nBV1310;
    /*^ M, 0, H, 0, EMTC_NB_MAX ^*/
    /* EmtcNb_et */

    U16       sfnGapV1310;
    /*^ M, 0, B, 1, EMTC_SFN_GAP_MAX ^*/

}RrcMacPcchConfigV1310_t;

/*! \PreambleMappingInfoR13_t
 *  \brief This struct contains the mapping of 
 *         preambles to groups for each CE level
 *  \param firstPreambleR13 
 *         indicates first preamble for CE level
 *  \param lastPreambleR13 
 *         indicates last preamble for CE level
 */
typedef struct _PreambleMappingInfoR13_t
{
    U8 firstPreambleR13;
    /*^ M, 0, H, 0, EMTC_MAX_PREAMBLE ^*/

    U8 lastPreambleR13;
    /*^ M, 0, H, 0, EMTC_MAX_PREAMBLE ^*/
}PreambleMappingInfoR13_t;

/*! \RrcMacRachCeLevelInfo_t
 *  \brief This struct provides rach configuration
 *         for each CE level
 *  \param raResponseWindowSizeR13 
 *         indicates size of the RA response window
 *  \param macContentionResolutionTimerR13
 *         indicates timer for mac contention resolution
 *  \param rarHoppingConfigR13 
 *         indicates frequency hopping activation/
 *         deactivation for RAR/Msg3/Msg4 for a CE level
 *  \param firstPreambleR13 
 *         indicates first preamble for CE level
 *  \param lastPreambleR13 
 *         indicates last preamble for CE level
 */
typedef struct _RrcMacRachCeLevelInfo_t
{
    U8				         raResponseWindowSizeR13;
    /*^ M, 0, H, 0, EMTC_RA_RESPONSE_WINDOW_SIZE_SF_MAX ^*/
    /* EmtcRaResponseWindowSize_et */

    U8				         macContentionResolutionTimerR13;
    /*^ M, 0, H, 0, EMTC_MAC_CONTENTION_RESOLUTION_TIMER_MAX ^*/
    /* EmtcMacContentionResolutionTimer_et */

    U8				         rarHoppingConfigR13;
    /*^ M, 0, H, 0, EMTC_RAR_HOPPPING_CONFIG_MAX ^*/
    /* EmtcRarHoppingConfig_et */
	
    U8                       firstPreambleR13;
    /*^ M, 0, H, 0, EMTC_MAX_PREAMBLE ^*/

    U8                       lastPreambleR13;
    /*^ M, 0, H, 0, EMTC_MAX_PREAMBLE ^*/

}RrcMacRachCeLevelInfo_t;


/*! \RrcMacRachConfigurationR13_t
 *  \brief This struct contains the rach configuration for Release 13
 *  \param numCELevels
 *         indicates number of CE Level
 *  \param rachCeLevelInfo 
 *         provides RACH information for each coverage level
 */
typedef struct _RrcMacRachConfigurationR13_t
{
    U8                             numCeLevels;
    /*^ M, 0, B, 1, EMTC_MAX_CE_LEVEL ^*/

    RrcCounter_t                   numCeLevelsCounter;
    /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    RrcMacRachCeLevelInfo_t        rachCeLevelInfo[EMTC_MAX_CE_LEVEL];
    /*^ TLV, SEQUENCE, RRC_MAC_RACH_CE_LEVEL_INFO_LIST ^*/

}RrcMacRachConfigurationR13_t;


/*! \RrcMacPuschConfigCommonV1310_t
 *  \brief This struct contains the cell specific PUSCH config common for 
 *         pusch transmission of eMTC.
 *  \param puschMaxNumRepetitionCeModeAR13 
 *         It contains the maximum value to indicate the set of PUSCH
 *         repetition numbers for CE mode A
 */
typedef struct _RrcMacPuschConfigCommonV1310_t
{
    U8 puschMaxNumRepetitionCeModeA ;
    /*^ M, 0, H, 0, EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_MAX ^*/
    /* EmtcPuschMaxNumRepetitionCeModeA_et */
}RrcMacPuschConfigCommonV1310_t;


/*! \ RrcFrequancyHoppingParametersR13_t
 *  \brief This struct contains the FDD UL hopping configuration for
 *         CE mode A.
 *  \param type
 *          the type of configuration i.e FDD or TDD
 *  \param intervalUlHoppingConfigCommonModeAFddR13
 *         It indicates the FDD number of consecutive
 *         absolute subframes over which PUCCH or PUSCH
 *         for CE mode X stays at the same narrowband
 *         before hopping to another narrowband.
 */
typedef struct _RrcFrequancyHoppingParametersR13_t
{
    U8    type;
    /*^ M, 0, H, 0, RRC_ONE ^*/

    U8    intervalUlHoppingConfigCommonModeAFddR13;
    /*^ M, 0, H, 0, EMTC_INTERVAL_UL_HOPPING_MODE_A_MAX ^*/
    /* EmtcIntervalUlHoppingConfigCommonModeAFdd_et */
}RrcFrequancyHoppingParametersR13_t;


/*! \ RrcMacPdschConfigCommonV1310_t
 *  \brief This struct contains the cell specific PDSCH config 
 *         common for pdsch transmission of eMTC.
 *  \param pdschMaxNumRepetitionCeModeAR13 
 *         indicates the maximum value to indicate the set of PDSCH
 *         repetition numbers for CE mode A
 */
typedef struct _RrcMacPdschConfigCommonV1310_t
{
    U8 pdschMaxNumRepetitionCeModeA;
    /*^ M, 0, H, 0, EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_MAX ^*/
    /* EmtcPdschMaxNumRepetitionCeModeA_et */
}RrcMacPdschConfigCommonV1310_t;


/*! \ PucchNumRepetitionCeMsg4Level0R13_t
 *  \brief This struct contains number of repetitions 
 *         for PUCCH carrying HARQ response for PRACH CE level 0
 *  \param pucchNumRepetitionCeMsg4Level0R13 
 *         indicates number of repetitions for PUCCH CE level 0
 */
typedef struct _PucchNumRepetitionCeMsg4Level0R13_t
{
    U8 pucchNumRepetitionLevel0;
    /*^ M, 0, H, 0, EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L0_MAX ^*/
    /*  EmtcPucchNumRepetitionCeMsg4Level0_et */
}PucchNumRepetitionCeMsg4Level0R13_t;


/*! \ PucchNumRepetitionCeMsg4Level1R13_t
 *  \brief This struct contains number of repetitions 
 *         for PUCCH carrying HARQ response for PRACH CE level 1
 *  \param pucchNumRepetitionCeMsg4Level1R13 
 *         indicates number of repetitions for PUCCH CE level 1
 */
typedef struct _PucchNumRepetitionCeMsg4Level1R13_t
{
    U8 pucchNumRepetitionLevel1;
    /*^ M, 0, H, 0, EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L1_MAX ^*/
    /* EmtcPucchNumRepetitionCeMsg4Level1_et */
}PucchNumRepetitionCeMsg4Level1R13_t;


/*! \ PucchNumRepetitionCeMsg4Level2R13_t
 *  \brief This struct contains number of repetitions for 
 *         PUCCH carrying HARQ response for PRACH CE level 2
 *  \param pucchNumRepetitionCeMsg4Level2R13 
 *         indicates number of repetitions for PUCCH CE level 2
 */
typedef struct _PucchNumRepetitionCeMsg4Level2R13_t
{
    U8 pucchNumRepetitionCeMsg4Level2R13;
    /*^ M, 0, H, 0, EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L2_MAX ^*/
    /* EmtcPucchNumRepetitionCeMsg4Level2_et */
}PucchNumRepetitionCeMsg4Level2R13_t;

/*! \ pucchNumRepetitionCeMsg4Level3R13_t
 *  \brief This struct contains number of repetitions 
 *         for PUCCH carrying HARQ response for PRACH
 *         CE level 3
 *  \param pucchNumRepetitionCeMsg4Level3R13 
 *         indicates number of repetitions for PUCCH 
 *         CE level 3
 */
typedef struct _PucchNumRepetitionCeMsg4Level3R13_t
{
    U8 pucchNumRepetitionCeMsg4Level3R13;
    /*^ M, 0, H, 0, EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L3_MAX ^*/
    /* EmtcPucchNumRepetitionCeMsg4Level3_et */
}PucchNumRepetitionCeMsg4Level3R13_t;

/*! \ rrcMacPucchConfigCommonV1310_t
 *  \brief This struct contains PUCCH common configuration parameters
 *         of eMTC.
 *  \param bitmask 
 *         to check optional field is present or not
 *  \param pucchNumRepetitionCeMsg4Level0R13 
 *         indicates Number of repetitions for carrying HARQ 
 *         response to containing for PRACH CE level 0
 *  \param pucchNumRepetitionCeMsg4Level1R13 
 *         indicates Number of repetitions for carrying HARQ 
 *         response to containing for PRACH CE level 1
 *  \param n1PucchAnInfoList
 *         the starting offsets of the PUCCH resource(s). The first
 *         entry in the list is the starting offset of the PUCCH resource(s)
 *         of CE level 0, the second entry in the list is the starting offset
 *         of the PUCCH resource(s) of CE level 1 and so on
 */

#define RRC_MAC_N1_PUCCH_AN_INFO_LIST_PRESENT    0x01
typedef struct _RrcMacPucchConfigCommonV1310_t
{
    RrcBitmaskU8_t               bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                           numCeLevels;
    /*^ M, 0, H, 1, EMTC_MAX_CE_LEVEL ^*/

    U8                           pucchNumRepetitionCeMsg4PerLevel[EMTC_MAX_CE_LEVEL];
    /*^ M, 0, OCTET_STRING, VARIABLE, H, 0, EMTC_PUCCH_NUM_REPETITION_CE_MSG4_MAX ^*/
    /* RrcMacPucchNumRepetitionCeMsg4_et */

    RrcN1PucchAnInfoListR13_t    n1PucchAnInfoList;
    /*^ TLV, RRC_MAC_N1_PUCCH_AN_INFO_LIST, RRC_MAC_N1_PUCCH_AN_INFO_LIST_PRESENT ^*/

}RrcMacPucchConfigCommonV1310_t;




/*! \RrcMacSiMsgsBr_t
 *  \brief This struct contains the SI BR message 
 *         information for MTC
 *  \param siIndexBr 
 *         contains an index to a table that defines 
 *         SystemInformationBlockType1-BR scheduling 
 *         information
 *  \param siPeriodicity
 *         It defines the the times when the particular
 *         SI message is repeated
 *  \param startingSfn 
 *         defines the 8 most significant bits of the SFN,
 *         where the SI message transmission starts  
 *  \param siRepetitionPatternR13 
 *         indicates the radio frames within the SI 
 *         window used for SI message transmission
 *  \param siNarrowbandR13 
 *         indicates the index of a narrowband used to 
 *         broadcast the SI message
 *  \param siTbsR13 
 *         indicates the transport block size information 
 *         used to broadcast the SI message
 *  \param siMsgBufBr
 *         indicates buffer of each si message
 */
typedef struct _RrcMacSiMsgsBr_t
{
    U8                           siIndexBr;
    /*^ M, 0, B, 1, EMTC_MAX_SI_COUNT ^*/  

    U8                           siPeriodicity;
    /*^ M, 0, H, 0, EMTC_SI_PERIODICITY_MAX ^*/
    /* EmtcSiPeriodicity_et */

    RrcSfn_t                     startingSfn;
    /*^ M, 0, H, 0, EMTC_MAX_STARTING_SFN ^*/

    U8                           siRepetitionPatternR13;
    /*^ M, 0, H, 0, EMTC_SI_REPETITION_MAX ^*/
    /* EmtcSiRepetitionPattern_et */

    U8                           siNarrowbandR13;
    /*^ M, 0, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/

    U8                           siTbsR13;
    /*^ M, 0, H, 0, EMTC_SI_TB_MAX ^*/
    /* EmtcSiTb_et */
    
    RrcCounter_t                 siMsgBufLengthBr;
    /*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/

    U8                           siMsgBufBr[EMTC_MAX_SI_BUFFER_LEN];
 	/*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/

}RrcMacSiMsgsBr_t;

/*! \enum RrcSiWindowLengthBrR13_et
 *  \brief an enum that specifies RRC MAC SI Window length for Release 13
 *  \Reference: 3GPP 36.331 , si-WindowLength-BR-r13
 */
typedef enum
{
    RRC_SI_WINDOW_LEN_BR_MS_20,
	RRC_SI_WINDOW_LEN_BR_MS_40,
	RRC_SI_WINDOW_LEN_BR_MS_60,
	RRC_SI_WINDOW_LEN_BR_MS_80,
	RRC_SI_WINDOW_LEN_BR_MS_120,
    RRC_SI_WINDOW_LEN_BR_MS_160,
	RRC_SI_WINDOW_LEN_BR_MS_200,
	RRC_SI_WINDOW_LEN_BR_MS_MAX = RRC_SI_WINDOW_LEN_BR_MS_200
}RrcSiWindowLengthBrR13_et;


/*! \RrcMacSiMsgBrInfo_t
 *  \brief This struct contains System Information 
 *         parameters for MTC
 *  \param siWindowLengthBr 
 *         indicates duration of SI scheduling window for all SIs
 *  \param siHoppingConfigCommonR13
 *         indicates the frequency hopping activation/deactivation
 *         for BR versions of SI messages and MPDCCH/PDSCH of paging 
 *  \param numSiMessageBr 
 *         indicates number of si message
 *  \param siMsgBrInfoCounter 
 *         indicates count of si message
 *  \param siMsgsBr
 *         contains info of each si message
 *  \param fddDownlinkOrTddSfBitmapBrR13
 *         indicates the set of valid subframes for 
 *         FDD downlink or TDD transmissions
 *  \param fddUplinkSfBitmapBrR13 
 *         indicates the set of valid subframes for 
 *         FDD uplink transmissions for BL UEs 
 */

#define RRC_MAC_FDD_DOWNLINK_TDD_SF_BITMAP_BR_PRESENT 	0x01
#define RRC_MAC_FDD_UPLINK_SF_BITMAP_BR_PRESENT 	    0x02
typedef struct _RrcMacSiMsgBrInfo_t
{
    RrcBitmaskU8_t                        bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                                    siWindowLengthBr;
    /*^ M, 0, H, 0, EMTC_MAX_SI_WINDOW_MS ^*/

    U8                                    siHoppingConfigCommonR13;
    /*^ M, 0, H, 0, EMTC_SI_HOPPING_MAX ^*/
    /* EmtcSiHoppingConfigCommon_et */

    U8                                    numSiMessageBr;
    /*^ M, 0, B, 1, EMTC_MAX_SI_COUNT ^*/

    RrcCounter_t                          siMsgBrInfoCounter;
    /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    RrcMacSiMsgsBr_t                      siMsgsBr[EMTC_MAX_SI_COUNT];
    /*^ TLV, SEQUENCE, RRC_MAC_SI_BR_MSG_REQ ^*/

    RrcMacFddDownlinkOrTddSfBitmapBr_t    fddDownlinkOrTddSfBitmapBr;
    /*^ TLV, RRC_MAC_FDD_DOWNLINK_TDD_SF_BITMAP_BR, RRC_MAC_FDD_DOWNLINK_TDD_SF_BITMAP_BR_PRESENT ^*/	

    RrcMacFddUplinkSfBitmapBr_t	          fddUplinkSfBitmapBr;
    /*^ TLV, RRC_MAC_FDD_UPNLINK_SF_BITMAP_BR, RRC_MAC_FDD_UPLINK_SF_BITMAP_BR_PRESENT ^*/
}RrcMacSiMsgBrInfo_t;


/*! \ MpdcchConfigSetupR13_t
 *  \brief This struct describes the mpdcch config info for release 13
 *  \param csiNumRepetitionCeR13 
 *         It indicates the number of subframes for CSI reference resource.
 *  \param mpdcchPdschHoppingConfigR13
 *         It indicates frequency hopping activation/deactivation for
 *         unicast MPDCCH/PDSCH
 *  \param mpdcchNumRepetitionR13
 *         It indicates maximum numbers of repetitions for UE-SS MPDCCH
 *  \param mpdcchNarrowbandR13
 *         It indicates the the narrowband on MPDCCH is transmitted.
 *  \param mpdcchStartSfUessFddType
 *         It indicates type whether FDD or TDD
 *  \param mpdcchStartSfUessFddR13
 *         It indicates starting subframe configuration for an MPDCCH
 *         UE-specific search space in FDD
 */
typedef struct _MpdcchConfigSetupR13_t
{
    U8                        csiNumRepetitionCeR13;
    /*^ M, 0, H, 0, EMTC_CSI_NUM_REPETITION_MAX ^*/
    /* EmtcCsiNumRepetitionCe_et */

    U8                        mpdcchPdschHoppingConfigR13;
    /*^ M, 0, H, 0, EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX ^*/
    /* EmtcMpdcchPdschHoppingConfig_et */

    U8                        mpdcchNumRepetitionR13;
    /*^ M, 0, H, 0, EMTC_MPDCCH_NUM_REPETITION_UESS_MAX ^*/
    /* EmtcMpdcchNumRepetitionUess_et */

    U8                        mpdcchNarrowbandR13;
    /*^ M, 0, H, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/

    U8                        mpdcchStartSfUessFddType;
    /*^ M, 0, H, 0, RRC_ONE ^*/

    U8                        mpdcchStartSfUessFddR13;
    /*^ M, 0, H, 0, EMTC_MPDCCH_START_SF_UESS_FDD_MAX ^*/
    /* EmtcMpdcchStartSfUessFdd_et */

}MpdcchConfigSetupR13_t;

/*! \ MpdcchConfigR13_t
 *  \brief This struct describes the configuration of MPDCCH.
 *  \param mpdcchConfigType
 *         It indicates wheher to release or setup mpdcch
 *         configuration.
 *  \param mpdcchConfigSetupR13
 *         It indicates the configuration of MPDCCH for
 *         setup.
 */

#define RRC_MAC_MPDCCH_CONFIG_SETUP_PRESENT      0x01
#define RRC_MAC_MPDCCH_CONFIG_RELEASE_PRESENT    0x02
typedef struct _MpdcchConfigR13_t
{
    RrcBitmaskU8_t            bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                        mpdcchConfigType;
    /*^ M, 0, H, 0, 1 ^*/

    MpdcchConfigSetupR13_t    mpdcchConfigSetupR13;
    /*^ TLV, RRC_MAC_MPDCCH_CONFIG_SETUP_R13, RRC_MAC_MPDCCH_CONFIG_SETUP_PRESENT ^*/
   
}MpdcchConfigR13_t;

/*! \RrcMacCeLevel_t
 *  \brief This struct describes the ce level of eMTC UE.
 *  \param ceLevel
 *         It indicates the ce level of eMTC UE.
 */
typedef struct _RrcMacCeLevel_t
{
    U8          ceLevel;
    /*^ M, 0, H, 0, EMTC_MAX_CE_LEVEL ^*/
}RrcMacCeLevel_t; 


/*! \RrcMacCeMode_t
 *  \brief This struct describes the ce mode of eMTC UE.
 *  \param ceMode
 *         It indicates the ce mode of eMTC UE.
 */
typedef struct _RrcMacCeMode_t
{
    U8    ceMode;
    /*^ M, 0, H, 0, EMTC_CE_MODE_MAX ^*/
    /* EmtcCeMode_et */
}RrcMacCeMode_t;

/*! \RrcEmtcMcs_t
 *  \brief This struct describes the UL/DL MCS for eMTC UE.
 *  \param ceMode
 *         It indicates the UL/DL MCS of eMTC UE.
 */
typedef struct _RrcEmtcMcs_t  
{
  U8 dlMcs;
  /*^ M, 0, H, 0, EMTC_MAX_MCS ^*/
  U8 ulMcs;
  /*^ M, 0, H, 0, EMTC_MAX_MCS ^*/
}RrcEmtcMcs_t;

/*! \RrcMacNumPrbPair_t
 *  \brief This struct describes the number of prb pair
 *         for release 13
 *  \param numPrbPairsV1310
 *         It indicates the number of prb pair.
 */
typedef struct _RrcMacNumPrbPair_t
{
    U8           numPrbPairsV1310;
    /*^ M, 0, H, 0, RRC_NUM_PRB_PAIR_MAX ^*/
    /* RrcNumPrbPairs_et */
}RrcMacNumPrbPair_t;

/*! \ EpdcchSetConfigR11_t
 *  \brief This struct describes the EPDCCH configuration set.
 *         E-UTRAN configures at least one EPDCCH-SetConfig when
 *         Note: EPDCCH-Config is configured. For BL UEs or UEs in CE,
 *               EUTRAN does not configure more than one EPDCCH-SetConfig.
 *  \param bitmask
 *         It indicates whether optional fields are present or not 
 *  \param setConfigIdR11
 *         It indicates the identity of the EPDCCH configuration set
 *  \param transmissionTypeR11
 *         It indicates whether distributed or localized EPDCCH
 *         transmission mode is used
 *  \param numberPrbPairsR11
 *         It indicates the number of physical resource-block pairs
 *         used for the EPDCCH set. Value n2 corresponds to 2 PRB pairs
 *  \param resourceBlockAssignmentR11
 *         It indicates the index to a specific combination of physical
 *         resource-block pair for EPDCCH set
 *  \param dmrsScramblingSequenceIntR11
 *         It indicates the  DMRS scrambling sequence initialization
 *         parameter.
 *  \param pucchResourceStartOffsetR11
 *         It indicates the pucch format 1a, 1b and 3 resource starting
 *         offset for the EPDCCH set.
 *  \param MpdcchConfigR13_t
 *         It indicates configuration of mpdcch
 */

#define RRC_MAC_MPDCCH_CONFIG_R13_PRESENT       0x01
typedef struct _EpdcchSetConfigR11_t
{
    RrcBitmaskU8_t                  bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                              setConfigIdR11;
    /*^ M, 0, H, 0, EMTC_MAX_EPDCCH_CONFIG_ID ^*/

    U8                              transmissionTypeR11;
    /*^ M, 0, H, 0, EMTC_TRANSMISSION_TYPE_MAX ^*/
    /* EmtcTransmissionType_et */

    U8                              numberPrbPairsR11;
    /*^ M, 0, H, 0, EMTC_NUM_PRB_PAIR_MAX ^*/
    /* EmtcNumPrbPairs_et */

    U8                              resourceBlockAssignmentR11;
    /*^ M, 0, H, 0, 14 ^*/

    U8                              resourceBlockAssignBitmap;
    /*^ M, 0, H, 0, 63 ^*/
 
    U16                             dmrsScramblingSequenceIntR11;
    /*^ M, 0, H, 0, EMTC_MAX_DMRS_SCRAMBLING_SEQUENCE_INT ^*/
 
    U16                             pucchResourceStartOffsetR11;
    /*^ M, 0, H, 0, EMTC_MAX_PUCCH_RESOURCE_START_OFFSET ^*/

    U8                             startSymbol;
    /*^ M, 0, B, 1, EMTC_MAX_START_SYMBOL ^*/

    MpdcchConfigR13_t               mpdcchConfigR13;
    /*^ TLV, RRC_MAC_MPDCCH_CONFIG_R13, RRC_MAC_MPDCCH_CONFIG_R13_PRESENT ^*/

}EpdcchSetConfigR11_t;

/*! \RrcMacPucchNumRepetitionCeR13_t
 *  \brief This struct describes the PUCCH Repetition configuration for eMTC.
 *  \param numRepetitionType
 *         It indciates whether pucch configuration is for setup or release.
 *  \param setup
 *         It indicates the PUCCH Repetition configuration for eMTC.
 */

#define RRC_MAC_PUCCH_NUM_REPETITION_CE_SETUP_R13_PRESENT    0x01
typedef struct _RrcMacPucchNumRepetitionCeR13_t
{
    RrcBitmaskU8_t                            bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                                        numRepetitionType;
    /*^ M, 0, H, 0, 1 ^*/

    RrcPucchNumRepetitionCeR13SetupModeA_t    setup;
    /*^ TLV, RRC_MAC_PUCCH_NUM_REPETITION_CE_SETUP_R13, RRC_MAC_PUCCH_NUM_REPETITION_CE_SETUP_R13_PRESENT ^*/
     
}RrcMacPucchNumRepetitionCeR13_t;



/*! \ RrcNpucchParamR13_t
 *  \brief This struct describes the npucch parameters for release 13
 *  \param nPucchIdentityR13
 *         It indicates pucch identity for release 13
 *  \param n1PucchAnR13
 *         It indicates UE-specific PUCCH AN resource offset
 */
typedef struct _RrcNpucchParamR13_t
{
    U16    nPucchIdentityR13;
    /*^ M, 0, H, 0, RRC_MAX_NPUCCH_IDENTITY_R13 ^*/

    U16    n1PucchAnR13;
    /*^ M, 0, H, 0, EMTC_MAX_N1_PUCCH_AN^*/
}RrcNpucchParamR13_t;

/*! \PucchConfigDedicatedR13_t
 *  \brief This struct describes the PUCCH dedicated configuration for
 *         release 13
 *  \param bitmask
 *         It indicates whether optional fields are present or not 
 *  \param pucchAckNackRepetitionR13
 *         It indicates whether ACK/NACK repetition is configured
 *         for release 13
 *  \param pucchNumRepetitionCeR13
 *         Indicates Number of PUCCH repetitions for PUCCH format for
 *         CE mode A
 */
#define RRC_MAC_PUCCH_NUM_REPETITION_CE_R13_PRESENT      0x01
#define RRC_MAC_ACK_NACK_REPETITION_R13_PRESENT          0x02
typedef struct _PucchConfigDedicatedR13_t
{
    RrcBitmaskU8_t                              bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    RrcMacPucchAckNackRepetitionR13_t           ackNackRepetitionR13;
    /*^ TLV, RRC_MAC_ACK_NACK_REPETITION_R13, RRC_MAC_ACK_NACK_REPETITION_R13_PRESENT ^*/

    RrcMacPucchNumRepetitionCeR13_t             pucchNumRepetitionCeR13;
    /*^ TLV, RRC_MAC_PUCCH_NUM_REPETITION_CE_R13, RRC_MAC_PUCCH_NUM_REPETITION_CE_R13_PRESENT ^*/

}PucchConfigDedicatedR13_t;

/*! \RrcMacUeCategoryV1310_t
 *  \brief This struct describes the ue category for release 13
 *  \param ueCategoryV1310
 *         It indicates ue category for release 13.
 */
typedef struct _RrcMacUeCategoryV1310_t
{
    U8    ueCategoryV1310;
    /*^ M, 0, H, 0, RRC_ONE ^*/
}RrcMacUeCategoryV1310_t;

/*! \RrcEmtcConnectionReconfigInfo_t
 *  \brief This struct describes the emtc specific information for RRC
 *         connection reconfiguration
 *  \param mpdcchNarrowbandR13
 *         it indicates the narrow band of mpdcch.
 *  \param resourceBlockAssignmentR11
 *         It indicates the resource block bitmap.
 */
typedef struct _RrcEmtcConnectionReconfigInfo_t
{
    U8                            mpdcchNarrowbandR13;
    /*^ M, 0, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/

    U8                            resourceBlockAssignBitmap;
    /*^ M, 0, N, 0, 0 ^*/
}RrcEmtcConnectionReconfigInfo_t;

/*! \RrcEmtcRbAssignmentR11
 *  \brief This struct describes the emtc specific RB assignment information
 *         for rrc connection reconfiguration
 *  \param rbAssignmentNumBits
 *         NumBits in RbAssignment.
 *  \param rbAssignmentVal
 *         RbAssignment value.
 */
typedef struct _RrcEmtcRbAssignmentR11
{
    U8    rbAssignmentNumBits;

    U8    rbAssignmentVal;
}RrcEmtcRbAssignmentR11;



/*! \RrcEmtcConnectionSetupInfo_t
 *  \brief This struct describes the emtc configuration for RRC connection setup
 *  \param epdcchConfig
 *         It indicates the EPDCCH config info.
 *  \param pucchConfigDedicatedR13
 *         It indicates the pucch dedicated configurtaion for release 13.
 *  \param ceMode
 *         It indicates the CE mode of eMTC UE.
 */
typedef struct _RrcEmtcConnectionSetupInfo_t
{
    EpdcchConfig_t                epdcchConfig;
    /*^ M, 0, N, 0, 0 ^*/

    RrcPucchConfigDedicatedR13_t  pucchConfigDedicatedR13;
    /*^ M, 0, N, 0, 0 ^*/

    U8                            ceMode;
    /*^ M, 0, H, 0, RRC_CE_MODE_MAX ^*/
    /* RrcCeMode_et */
        
}RrcEmtcConnectionSetupInfo_t;

/*! \RrcUeccLlimEmtcConfigInfo_t
 *  \brief This struct describes the emtc configuration for RRC connection
 *         setup and reconfiguration
 *  \param bitmask
 *         It indicates the optional IEs presence
 *  \param RrcEmtcConnectionSetupInfo_t
 *         It indicates the RRC connection setup info for release 13.
 *  \param RrcEmtcConnectionReconfigInfo_t
 *         It indicates the RRC connection reconfigurtaion for release 13.
 *  \param ueCategoryDlV1310
 *         It indicates the DL UE categroy of release 13.
 *  \param ueCategoryUlV1310
 *         It indicates the UL UE categroy of release 13.
 *  \param resourceBlockAssignmentR11
 *         It indicates the resource block assignment calculated by L3.
 */

#define RRC_UECC_LLIM_EMTC_CONNECTION_SETUP_INFO_PRESENT       0x01
#define RRC_UECC_LLIM_EMTC_CONNECTION_RECONFIG_INFO_PRESENT    0x02 
#define RRC_UECC_LLIM_EMTC_DL_UE_CATEGORY_PRESENT              0x04 
#define RRC_UECC_LLIM_EMTC_UL_UE_CATEGORY_PRESENT              0x08 
#define RRC_UECC_LLIM_EMTC_RB_ASSIGNMENT_PRESENT               0x10
typedef struct _RrcUeccLlimEmtcConfigInfo_t
{
    RrcBitmaskU8_t                     bitmask;

    RrcEmtcConnectionSetupInfo_t       emtcConnectionSetupInfo;

    RrcEmtcConnectionReconfigInfo_t    emtcConnectionReconfigInfo;

    U8                                 ueCategoryDlV1310;

    U8                                 ueCategoryUlV1310;

    U8                                 resourceBlockAssignmentR11;
    /*^ M, 0, N, 0, 0 ^*/
}RrcUeccLlimEmtcConfigInfo_t;

/* eMTC changes stop */

typedef struct _rrc_phy_sync_signals_t
{
    S8  prim_syn_signal_power;  /*^ M, 0, B, -6, 4 ^*/
    S8  sec_syn_signal_power;   /*^ M, 0, B, -6, 4 ^*/
    U8  sec_syn_signal_m_seq1;  /*^ M, 0, H, 0, 30 ^*/
    U8  sec_syn_signal_m_seq2;  /*^ M, 0, H, 0, 30 ^*/
} rrc_phy_sync_signals_t;

typedef struct _rrc_phy_reference_signal_t
{
    S8  ref_signal_power;       /*^ M, 0, B, -60, 50 ^*/
ul_ref_signals_pusch_t    ul_ref_signals_pusch;  /*^ M, 0, N, 0, 0 ^*/
} rrc_phy_reference_signal_t;


typedef struct
{
  S8        ref_signal_power;       /*^ M, 0, B, -60, 50 ^*/
  U8        pb;                     /*^ M, 0, H, 0, 3 ^*/   /* rrm_pb_et */
}pdsch_config_common_t;

/*NR_DC Code Change Start*/
typedef enum                        
{                                   
    SN_TERMINATED_SCG_BEARER = 1,
/* OPTION3X Changes Start */
    SN_TERMINATED_SPLIT_BEARER,
    DC_BEARER_TYPE_LAST
/* OPTION3X Changes End */
}rrc_dc_bearer_type_et;             
/*NR_DC Code Change Stop*/

/* PRS + */
typedef struct _rrc_phy_prs_config_t
{
  U8        prs_bandwidth;      /*^ M, 0, B, 6, 100 ^*/
  U8        prs_cyclic_prefix_type;    /*^ M, 0, H, 0, 1 ^*/
}rrc_phy_prs_config_t;
/* PRS - */

typedef struct _rrc_phy_pdsch_configuration_t
{
pdsch_config_common_t pdsch_config; /*^ M, 0, N, 0, 0 ^*/
}rrc_phy_pdsch_configuration_t;

#define SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG     0x01

typedef struct
{
  U16       presence_bitmask;          /*^ BITMASK ^*/
  U8        srs_bw_config;
/*^ M, 0, H, 1, 7 ^*/    /* rrm_srs_bw_config_et */

  U8        srs_subframe_config;
/*^ M, 0, H, 1, 15 ^*/   /* rrm_srs_subframe_config_et */

  U8        ack_nack_srs_simul_trans;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_bool_et */

  U8        srs_max_up_pts;
/*^ O, SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG,H,0,0 ^*/
/* rrm_srs_max_up_pts_et */ /*OM. O since 331.860*/
}sounding_rs_ul_config_common_setup_t;


#define SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG      0x01

typedef struct
{
    U16                                  presence_bitmask;       /*^ BITMASK ^*/
    sounding_rs_ul_config_common_setup_t setup;
/*^ O, SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG, H, 0, 0 ^*/
}sounding_rs_ul_config_common_t;


/** TDD Changes Start **/



typedef struct _rrc_phy_tdd_config_t
{
    U16      sub_frame_assignment;
    /*^ M, 0, H, 0, 6 ^*/  /* rrm_sub_frame_assignment_et */
    U16      special_sub_frame_pattern;
    /*^ M, 0, H, 0 , 8 ^*/  /* rrm_special_sub_frame_pattern_et */
} rrc_phy_tdd_config_t;

/** TDD Changes Stop **/
/* SPR-18445 start */
#define RRC_PHYS_CELL_PARAMS_DL_EARFCN_MAX_COUNT   25
typedef struct _rrc_phys_dl_earfcn_t
{
    /* Coverity Fix 97135 start */
    U16                 count;  /*^ M, 0, H, 0, RRC_PHYS_CELL_PARAMS_DL_EARFCN_MAX_COUNT ^*/    
/* SPR-18445 stop */
    /* Coverity Fix 97135 End */
    U32                 earfcnDl[RRC_PHYS_CELL_PARAMS_DL_EARFCN_MAX_COUNT];/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_phys_dl_earfcn_t;           

/*  LAA Rel13 Code + */
typedef struct _rrc_radio_res_dedicated_laa_scell_config_t
{
    U32     subframe_start_position;         /*^ M, 0, H, 0, 1 ^*/
    U8      laa_scell_subframe_config;      /*^ M, 0, H, 0, 255 ^*/
}rrc_radio_res_dedicated_laa_scell_config_t;

typedef struct _rrc_dmtc_configuration_r13_t
{
    U8         dmtc_periodicity;        /*^ M, 0, H, 0, 2 ^*/
    U8         dmtc_offset;             /*^ M, 0, H, 0, 159 ^*/
    U8         ds_occassion_duration;   /*^ M, 0, H, 1, 5 ^*/
}rrc_dmtc_configuration_r13_t;
/*  LAA Rel13 Code - */

typedef struct _rrc_config_phy_cell_parameters_t
{

    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_TDD_CONFIG_PRESENT 0x01
/* PRS + */
#define RRC_PHY_PRS_CONFIG_INFO_PRESENT 0x02
/* PRS - */

#define RRC_PHY_DL_EARFCN_LIST_PRESENT 0x04
/*  LAA Rel13 Code + */
#define RRC_PHY_LAA_SCELL_CONFIGURATION_INFO_PRESENT 0x08
#define RRC_PHY_DMTC_CONFIGURATION_INFO_PRESENT      0x10
/*  LAA Rel13 Code - */

    U8  duplexing_mode;
/* HD-FDD start */
/*^ M, 0, H, 0, 2 ^*/                       /* duplexing_mode_et */
/* HD-FDD stop */
     U8                            freq_band_indicator;      /*^ M, 0, B, 1, MAX_FREQ_BAND_INDICATOR ^*/
    U16 ul_earfcn;
    U32 dl_earfcn;
    U8  num_of_antennas;                    /*^ M, 0, H, 1, 4 ^*/
    U8  ul_tx_bandwidth;
/*^ M,0, H, 0, 5 ^*/  /* ul_tx_bandwidth_et */

    U8  dl_tx_bandwidth;
/*^ M, 0, H, 0, 5 ^*/  /* dl_tx_bandwidth_et */

    U8  subcarrier_spacing;
/*^ M, 0, H, 0, 1 ^*/                       /* subcarrier_spacing_et */

    U8  ul_cyclic_prefix;
/*^ M, 0, H, 0, 1 ^*/                       /* cyclic_prefix_et */

    U8  dl_cyclic_prefix;
/*^ M, 0, H, 0, 1 ^*/                       /* cyclic_prefix_et */

    sounding_rs_ul_config_common_t    srs_bandwidth_configuration;
/*^ M, 0, H, 0, 0 ^*/

    rrc_phys_cell_id_t  phys_cell_id;       /*^ M, 0, H, 0, 503 ^*/


    /* CLPC Start */

    U16 pcfich_power_offset;               /*^ M, 0, H, 0, 10000 ^*/

    U16 phich_power_offset;                /*^ M, 0, H, 0, 10000 ^*/

    /* CLPC Stop */

    /* SPR 3802 Start */
    U8    max_rs_epre;
    /*^ M, 0, H, 0, 160 ^*/ 
    /* SPR 3802 Stop */

/** TDD Changes Start **/

    rrc_phy_tdd_config_t tdd_config;
 /*^ O, RRC_PHY_TDD_CONFIG_PRESENT ^*/

/* TDD Changes Stop **/
    /* PRS + */
    rrc_phy_prs_config_t            prs_config;
    /*^ O, RRC_PHY_PRS_CONFIG_INFO_PRESENT, N, 0, 0 ^*/
    /* PRS - */

    rrc_phys_dl_earfcn_t           dl_earfcn_list;
    /*^ O, RRC_PHY_DL_EARFCN_LIST_PRESENT, N, 0, 0 ^*/

    /*  LAA Rel13 Code + */
    rrc_radio_res_dedicated_laa_scell_config_t laa_scell_configuration_r13;  /*^ O, RRC_PHY_LAA_SCELL_CONFIGURATION_INFO_PRESENT, N, 0, 0 ^*/
    rrc_dmtc_configuration_r13_t      dmtc_configuration_r13;       /*^ O, RRC_PHY_DMTC_CONFIGURATION_INFO_PRESENT, N, 0, 0 ^*/
    /*  LAA Rel13 Code -*/

} rrc_config_phy_cell_parameters_t;

/* SPR-18445 start */
#define RRC_PHY_CELL_PARAMS_DL_EARFCN_MAX_COUNT   25
typedef struct _rrc_dl_earfcn_t
{
        /* Coverity Fix 97132 start */
        U16                 count;  /*^ M, 0, H, 0, RRC_PHY_CELL_PARAMS_DL_EARFCN_MAX_COUNT ^*/  
/* SPR-18445 stop */
        /* Coverity Fix 97132 end */
        U32                 earfcnDl[RRC_PHY_CELL_PARAMS_DL_EARFCN_MAX_COUNT];/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_dl_earfcn_t;

typedef struct _rrc_phy_cell_parameters_t
{
    U32 dl_earfcn;
    U8  num_of_antennas;                    /*^ M, 0, B, 1, 4 ^*/
    U8  subcarrier_spacing;
/*^ M, 0, H, 0, 1 ^*/                       /* subcarrier_spacing_et */
    U8  dl_cyclic_prefix;
/*^ M, 0, H, 0, 1 ^*/                       /* cyclic_prefix_et */
    U8  rb_size;
/*^ M, 0, H, 0, 1 ^*/                       /* rb_size_et */

    rrc_phys_cell_id_t  phys_cell_id;       /*^ M, 0, H, 0, 503 ^*/

    /* SPR 3802 Start */
    U8    max_rs_epre;
    /*^ M, 0, H, 0, 160 ^*/ 
    /* SPR 3802 Stop */
 	rrc_dl_earfcn_t dl_earfcn_list;
} rrc_phy_cell_parameters_t;

typedef struct _rrc_rcfg_phy_cell_parameters_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_RECONFIG_PHY_CELL_PARAMS_DL_EARFCN_PRESENT 0x01
#define RRC_RRM_RECONFIG_PHY_CELL_PARAMS_NUM_OF_ANTENNAS   0x02
#define RRC_RRM_RECONFIG_PHY_CELL_PARAMS_DL_CYCLIC_PREFIX  0x04
#define RRC_RRM_RECONFIG_PHY_CELL_PARAMS_RB_SIZE           0x08
#define RRC_RRM_RECONFIG_PHY_CELL_ID                       0x10
/* CLPC Start */
#define RRC_RRM_PRIMARY_SYNC_SIGNAL_EPRE_EPRERS            0x20
#define RRC_RRM_SECONDARY_SYNC_SIGNAL_EPRE_EPRERS          0x40
/* CLPC Stop */
/* SPR 3802 Start */
#define RRC_RRM_RECONFIG_PHY_CELL_MAX_RS_EPRE_PRESENT      0x80
/* SPR 3802 Stop */

    U32 dl_earfcn;                          
    /*^ O, RRC_RRM_RECONFIG_PHY_CELL_PARAMS_DL_EARFCN_PRESENT ^*/
    U8  num_of_antennas;                    
    /*^ O, RRC_RRM_RECONFIG_PHY_CELL_PARAMS_NUM_OF_ANTENNAS, B, 1, 4 ^*/
    U8  dl_cyclic_prefix;
    /*^ O, RRC_RRM_RECONFIG_PHY_CELL_PARAMS_DL_CYCLIC_PREFIX, H, 0, 1 ^*/   /* cyclic_prefix_et */
    U8  rb_size;
    /*^ O, RRC_RRM_RECONFIG_PHY_CELL_PARAMS_RB_SIZE, H, 0, 1 ^*/    /* rb_size_et */
    rrc_phys_cell_id_t  phys_cell_id; /*^ O, RRC_RRM_RECONFIG_PHY_CELL_ID ^*/

    /* CLCP Start */
    U16 pri_sync_sig_epre_eprers;
    /*^ O, RRC_RRM_PRIMARY_SYNC_SIGNAL_EPRE_EPRERS, H, 0, 10000 ^*/
    
    U16 sec_sync_sig_epre_eprers;
    /*^ O, RRC_RRM_SECONDARY_SYNC_SIGNAL_EPRE_EPRERS, H, 0, 10000 ^*/

    /* CLPC Stop */
    /* SPR 3802 Start */
    U8    max_rs_epre;
    /*^ O, RRC_RRM_RECONFIG_PHY_CELL_MAX_RS_EPRE_PRESENT, H, 0, 160 ^*/ 
    /* SPR 3802 Stop */
} rrc_recfg_phy_cell_parameters_t;

/******************************************************************************
*   End of these types are shared CSC-LLIM with PHY and RRM
******************************************************************************/

/******************************************************************************
*   These types are shared CSC-LLIM with MAC
******************************************************************************/
#define RRC_MAX_MIB_LENGTH      10
#define RRC_MAX_SIBTYPE1_LENGTH 1024
#define RRC_MAX_SI_LENGTH       277

#define RRC_MAX_SI_MSGS         12
#define RRC_MIB_MSGS            256
#define RRC_MAX_SI_MSG_PARAMS   16
#define MAX_SI_SEGMENTS         64
#define MAX_SIB8_INSTANCES      16
/* SPR 13574 Fix Start */
#define MAX_NUM_CMAS_WARNING    3
/* SPR 13574 Fix Stop */
#define MAX_SI_START_OFFSET     8
#define RRC_INVALID_SI_INDEX    9
typedef enum {
    RRC_SI_SYNC_SYS_TIME,
    RRC_SI_ASYNC_SYS_TIME,
    RRC_SI_LONG_CODE_STATE_1_XRTT
} rrc_mac_si_msg_param_type_et;

typedef struct _rrc_mac_si_msg_param_t
{
    U8  id;     /*^ M, 0, H, 0, 2 ^*/ /* rrc_mac_si_msg_param_type_et */
    U32 offset; /* in bits */
    U32 length; /* in bits */
} rrc_mac_si_msg_param_t;

#ifndef OAM_IPR
/*CDMA2000_CSFB_HO start*/
typedef enum
{
    NO_SIB8,
    SIB8_NEW_BUFFER,
    SIB8_FLUSH_OLD_BUFFER_APPLY_NEW_BUFFER,
    SIB8_NO_CHANGE
}sched_reqd_for_sib8_et;
#endif

typedef struct _rrc_mac_sched_reqd_for_sib8_info_t
{
    U8  sched_reqd_for_sib8;        /*^ M, 0, H, 0, 3 ^*/  /* sched_reqd_for_sib8_et */
} rrc_mac_sched_reqd_for_sib8_info_t;

typedef struct
{
    rrc_timestamp_U64_t   ts_sec;    /*^ M, 0, H, 0, 9223372036854775807 ^*/
    rrc_timestamp_U32_t   ts_nsec;   /*^ M, 0, H, 0, 4294967295 ^*/
}rrc_timestamp_t;

/*CDMA2000_CSFB_HO stop*/

typedef struct _rrc_mac_si_segment_data_t
{
    rrc_counter_t           si_msg_buf_length;
    U8                      si_msg_buf[RRC_MAX_SI_LENGTH];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_mac_si_segment_data_t;

typedef struct _rrc_num_si_repetitions
{
    U16         num_of_repetitions; /*^ M, 0, H, 1, 4095 ^*/
}rrc_num_si_repetitions;

typedef struct _rrc_num_si_transmissions_t
{
    U8          num_of_transmissions; /*^ M, 0, B, 1, 8 ^*/
}rrc_num_si_transmissions_t;

typedef struct _rrc_mac_si_start_offset_info_t
{
    U8          si_start_offset[MAX_SI_START_OFFSET];
    /*^ M, 0, OCTET_STRING, FIXED, H, 0, 39 ^*/
}rrc_mac_si_start_offset_info_t;


#define RRC_NUM_SI_REPETITIONS_PRESENT 0x01
#define RRC_NUM_SI_TRANSMISSIONS_PRESENT 0x02
#define RRC_NUM_SI_START_OFFSET_PRESENT 0x04
/*CDMA2000_CSFB_HO start*/
#define RRC_SCHED_REQD_FOR_SIB8_PRESENT 0x08
/*CDMA2000_CSFB_HO stop*/

typedef struct _rrc_mac_si_msg_info_t
{
    rrc_bitmask_t                   optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8      si_index;   /*^ M, 0, H, 0, 11 ^*/  
    U8          periodicity;
/*^ M, 0, B, 1, 7 ^*/       /* Bug in MAC API document - we must update RRC */

    rrc_sfn_t   starting_sfn;   /*^ M, 0, H, 0, 1023 ^*/
    rrc_sf_t    starting_sf;    /*^ M, 0, H, 0, 9 ^*/

     U8                  num_of_segment ;  /*^ M, 0, B, 1, 64 ^*/

    rrc_num_si_repetitions   num_si_repetitions;
/*^ TLV, RRC_NUM_SI_REPETITIONS, RRC_NUM_SI_REPETITIONS_PRESENT ^*/

    rrc_num_si_transmissions_t num_si_transmissions;
/*^ TLV, RRC_NUM_SI_TRANSMISSIONS, RRC_NUM_SI_TRANSMISSIONS_PRESENT ^*/

    rrc_mac_si_start_offset_info_t   si_start_offset_info;
/*^ TLV, RRC_MAC_SI_START_OFFSET, RRC_NUM_SI_START_OFFSET_PRESENT ^*/
    rrc_counter_t       segment_counter ;  
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/
    rrc_mac_si_segment_data_t         si_segment_data[MAX_SI_SEGMENTS];
    /*^ TLV, SEQUENCE, RRC_MAC_SI_MSG_SEGMENT_REQ ^*/

    rrc_counter_t           si_msg_param_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_si_msg_param_t  si_msg_param[RRC_MAX_SI_MSG_PARAMS];
/*^ TLV, SEQUENCE, RRC_MAC_SI_MSG_INFO_PARAM ^*/

/*CDMA2000_CSFB_HO start*/
    rrc_mac_sched_reqd_for_sib8_info_t rrc_mac_sched_reqd_for_sib8;
/*^ TLV, RRC_SCHED_REQD_FOR_SIB8, RRC_SCHED_REQD_FOR_SIB8_PRESENT ^*/
/*CDMA2000_CSFB_HO stop*/

} rrc_mac_si_msg_info_t;

typedef struct _rrc_mac_si_msg_req_t
{
    U8  si_window_size;         /*^ M, 0, B, 1, 40 ^*/
    U8  num_si_message;         /*^ M, 0, H, 0, 74 ^*/

    rrc_counter_t           si_msg_info_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_si_msg_info_t   si_msg_info[RRC_MAX_SI_MSGS];
/*^ TLV, SEQUENCE, RRC_MAC_SI_MSG_REQ ^*/

} rrc_mac_si_msg_req_t;

typedef struct _rrc_mac_sibtype1_msg_req_t
{
    U8      si_mapping_changed; /*^ M, 0, H, 0, 1 ^*/   
    rrc_sfn_t   sfn;    /*^ M, 0, H, 0, 1023 ^*/

    rrc_counter_t   sibtype1_msg_buf_counter;
/*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/

    U8              sibtype1_msg_buf[RRC_MAX_SIBTYPE1_LENGTH];
/*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/

} rrc_mac_sibtype1_msg_req_t;

typedef struct _rrc_mac_mib_msg_req_t
{
    rrc_sfn_t   sfn;    /*^ M, 0, H, 0, 1023 ^*/

    rrc_counter_t   mib_msg_buf_counter;
/*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/

    U8              mib_msg_buf[RRC_MAX_MIB_LENGTH];
/*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/

} rrc_mac_mib_msg_req_t;


typedef struct _rrc_mac_mod_period_info_t
{
    U32         mod_period; /*^ M, 0, B, 64, 4096 ^*/
}rrc_mac_mod_period_info_t;

typedef struct _rrc_mac_sfn_gap_info_t
{
    U8         sfn_gap; /*^ M, 0, B, 1, 32 ^*/
}rrc_mac_sfn_gap_info_t;


typedef struct
{
  U8            count;
/*^ M, 0, N, 0, 31 ^*/     /* MAX_SIB_MESSAGE_1  */

  U8            sib_type[MAX_SIB_MESSAGE_1];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/  /* rrm_sib_type_et */

}sib_mapping_info_t;


typedef struct
{
  sib_mapping_info_t   sib_mapping_info; /*^ M, 0, N, 0, 0 ^*/
  U8                   si_periodicity;
/*^ M, 0, H, 0, 6 ^*/ /* rrm_si_periodicity_et */

}scheduling_info_t;


typedef struct
{
	/* SPR_17664_start*/
  U8                count;              /*^ M, 0, B, 1, MAX_SI_MESSAGE ^*/
 
	/* SPR_17664_end*/
 scheduling_info_t  scheduling_info[MAX_SI_MESSAGE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}scheduling_info_list_t;


typedef struct
{
    U8  message_identifier[MSG_ID_OCTET_SIZE];  
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8  serial_number[SERIAL_NUMBER_OCTET_SIZE]; 
/*^ M, 0, OCTET_STRING, FIXED ^*/
}rrc_warning_info_t;


typedef struct
{
/* SPR-18445 start */
  U8                  count;              /*^ M, 0, B, 1, MAX_WARNING_MESSAGE ^*/
/* SPR-18445 stop */
  rrc_warning_info_t  warning_info[MAX_WARNING_MESSAGE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_warning_info_list_t;


typedef struct
{
    U8  message_identifier[MSG_ID_OCTET_SIZE];  
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8  serial_number[SERIAL_NUMBER_OCTET_SIZE]; 
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8  num_of_segments;
/*^ M, 0, B, 1, 64 ^*/

    U16 num_of_repetitions; 
/*^ M, 0, B, 1, 4095 ^*/

} cmas_scheduling_info_t;

typedef struct
{   /* SPR 13574 Fix Start */              
    U8                     cmas_warning_count; /*^ M, 0, B, 1, MAX_NUM_CMAS_WARNING ^*/ 
    
    /* SPR 13574 Fix Stop */ 
             
    cmas_scheduling_info_t cmas_scheduling_info[MAX_NUM_CMAS_WARNING];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

} cmas_scheduling_info_list_t;

typedef struct
{
    rrc_bitmask_t    bitmask;        /*^ BITMASK ^*/

#define RRC_RRM_ETWS_SCHEDULING_NUM_OF_SEGMENTS_PRESENT 0x01

    U8  message_identifier[MSG_ID_OCTET_SIZE];  
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8  serial_number[SERIAL_NUMBER_OCTET_SIZE]; 
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8  num_of_segments;
/*^ O, RRC_RRM_ETWS_SCHEDULING_NUM_OF_SEGMENTS_PRESENT, B, 1, 64 ^*/

} etws_scheduling_info_t;


/******************************************************************************
*   End of these types are shared CSC-LLIM with MAC
******************************************************************************/

/******************************************************************************
*   These types are shared CSC-LLIM with MAC and RRM
******************************************************************************/
/* PRS +*/
typedef enum
{
    RRC_PRS_RB_N6,
    RRC_PRS_RB_N15,
    RRC_PRS_RB_N25,
    RRC_PRS_RB_N50,
    RRC_PRS_RB_N75,
    RRC_PRS_RB_N100
}rrc_prs_bandwidth_et;

typedef enum
{
    RRC_PRS_N_SF_1,
    RRC_PRS_N_SF_2,
    RRC_PRS_N_SF_4,
    RRC_PRS_N_SF_6
}rrc_prs_subframes_et;

typedef enum
{
    RRC_PRS_N2,
    RRC_PRS_N4,
    RRC_PRS_N8,
    RRC_PRS_N16
}rrc_prs_muting_bits_et;

/* PRS -*/
typedef enum
{
    MAC_RA_RESP_WIN_SIZE_SF2,
    MAC_RA_RESP_WIN_SIZE_SF3,
    MAC_RA_RESP_WIN_SIZE_SF4,
    MAC_RA_RESP_WIN_SIZE_SF5,
    MAC_RA_RESP_WIN_SIZE_SF6,
    MAC_RA_RESP_WIN_SIZE_SF7,
    MAC_RA_RESP_WIN_SIZE_SF8,
    MAC_RA_RESP_WIN_SIZE_SF10
} mac_ra_resp_win_size_et;

typedef enum
{
    MAC_CONT_RES_TIMER_SF8,
    MAC_CONT_RES_TIMER_SF16,
    MAC_CONT_RES_TIMER_SF24,
    MAC_CONT_RES_TIMER_SF32,
    MAC_CONT_RES_TIMER_SF40,
    MAC_CONT_RES_TIMER_SF48,
    MAC_CONT_RES_TIMER_SF56,
    MAC_CONT_RES_TIMER_SF64
} mac_cont_resol_timer_et;

typedef struct _rrc_mac_enable_frequency_selective_scheduling_t
{
    U8      ul_freq_selective_enable;    /*^ M, 0, H, 0, 1 ^*/
    U8      dl_freq_selective_enable;    /*^ M, 0, H, 0, 1 ^*/
}rrc_mac_enable_frequency_selective_scheduling_t;


/* CLPC Start */
typedef struct _rrc_mac_downlink_power_control_common_t
{
    U16 pcfichPowerOffset; /*^ M, 0, H, 0, 10000 ^*/ 

    U16 phichPowerOffset;  /*^ M, 0, H, 0, 10000 ^*/

    U16 pdcchPowerOffset; /*^ M, 0, H, 0, 10000 ^*/

    U16 pbchTransmissionPower; /*^ M, 0, H, 0, 10000 ^*/

    U16 pchTransmissionpower; /*^ M, 0, H, 0, 10000 ^*/

}rrc_mac_downlink_power_control_common_t;

typedef struct _rrc_mac_tpc_rnti_range_t
{
    U16 startTpcRntiPucch; /*^ M, 0, B, 61, 65523 ^*/

    U16 endTpcRntiPucch; /*^ M, 0, B, 61, 65523 ^*/

    U16 startTpcRntiPusch; /*^ M, 0, B, 61, 65523 ^*/

    U16 endTpcRntiPusch; /*^ M, 0, B, 61, 65523 ^*/

}rrc_mac_tpc_rnti_range_t;

typedef struct _rrc_mac_power_control_enable_t
{
    U8 harqBlerClpcPucchEnable; /*^ M, 0, H, 0, 1 ^*/

    U8 cqiSinrClpcPucchEnable; /*^ M, 0, H, 0, 1 ^*/

    U8 clpcPuschEnable;       /*^ M, 0, H, 0, 1 ^*/

    U8 pdcchPowOrAggregationEnable;  /*^ M, 0, H, 0, 1 ^*/

    U8 clpcPuschfreqSelectiveEnable; /*^ M, 0, H, 0, 1 ^*/

}rrc_mac_power_control_enable_t;

/* PRS + */
typedef struct _rrc_rrm_prs_bandwidth_info_t
{
    U8  prs_bandwidth;          /*^ M, 0, H, 0, 5 ^*/
    /* rrc_prs_bandwidth_et */
}rrc_rrm_prs_bandwidth_info_t;

typedef struct _rrc_rrm_prs_subframes_info_t
{
    U8  prs_subframes;          /*^ M, 0, H, 0, 3 ^*/
    /* rrc_prs_subframes_et */
}rrc_rrm_prs_subframes_info_t;

typedef struct  _rrc_rrm_prs_config_index_info_t
{
    U16 prs_config_index;       /*^ M, 0, H, 0, 4095 ^*/
}rrc_rrm_prs_config_index_info_t;

typedef struct _rrc_rrm_prs_transmission_power_info_t
{
    U16 prs_transmission_power; /*^ M, 0, H, 0, 10000 ^*/
}rrc_rrm_prs_transmission_power_info_t;

typedef struct _rrc_rrm_prs_muting_config_info_t
{
    U8  bits_to_be_read;        /*^ M, 0, H, 0, 3 ^*/
    /* rrc_prs_muting_bits_et */
    U16 prs_muting_config_index;/*^ M, 0, N, 0, 0 ^*/
}rrc_rrm_prs_muting_config_info_t;

typedef struct _rrc_rrm_prs_config_t
{
	rrc_bitmask_t     bitmask;  /*^ BITMASK ^*/
#define RRC_RRM_CONFIG_PRS_MUTING_CONFIG_INFO_PRESENT          0x01

	rrc_rrm_prs_bandwidth_info_t    prs_bandwidth_info; 
	/*^ M, 0, N, 0, 0 ^*/
	rrc_rrm_prs_subframes_info_t    prs_subframes_info; 
	/*^ M, 0, N, 0, 0 ^*/
	rrc_rrm_prs_config_index_info_t prs_config_index_info;
	/*^ M, 0, N, 0, 0 ^*/    
	rrc_rrm_prs_transmission_power_info_t    prs_transmission_power_info;
	/*^ M, 0, N, 0, 0 ^*/
	rrc_rrm_prs_muting_config_info_t    prs_muting_config_info;
	/*^ O, RRC_RRM_CONFIG_PRS_MUTING_CONFIG_INFO_PRESENT, N, 0, 0 ^*/

}rrc_rrm_prs_config_t;

typedef struct _rrc_rrm_prs_config_info_t
{
    rrc_bitmask_t     bitmask;  /*^ BITMASK ^*/
#define RRC_RRM_CONFIG_PRS_CONFIG_PRESENT 	0x01

    U8  request_type;           /*^ M, 0, H, 0, 1 ^*/ 
    rrc_rrm_prs_config_t	prs_config;
    /*^ O, RRC_RRM_CONFIG_PRS_CONFIG_PRESENT, N, 0, 0 ^*/
}rrc_rrm_prs_config_info_t;

typedef struct _rrc_rrm_prs_reconfig_t
{
	rrc_bitmask_t     bitmask;  /*^ BITMASK ^*/
	/* PRS_UPDATED + */
#define RRC_RRM_RECONFIG_PRS_BANDWIDTH_INFO_PRESENT              0x01
#define RRC_RRM_RECONFIG_PRS_SUBFRAMES_INFO_PRESENT              0x02
#define RRC_RRM_RECONFIG_PRS_CONFIG_INDEX_INFO_PRESENT           0x04
#define RRC_RRM_RECONFIG_PRS_TRANSMISSION_POWER_INFO_PRESENT     0x08
#define RRC_RRM_RECONFIG_PRS_MUTING_CONFIG_INFO_PRESENT          0x10
	
	rrc_rrm_prs_bandwidth_info_t    prs_bandwidth_info;
        /*^ O, RRC_RRM_RECONFIG_PRS_BANDWIDTH_INFO_PRESENT, N, 0, 0 ^*/

	/* PRS_UPDATED - */
	rrc_rrm_prs_subframes_info_t    prs_subframes_info;
	/*^ O, RRC_RRM_RECONFIG_PRS_SUBFRAMES_INFO_PRESENT, N, 0, 0 ^*/

	rrc_rrm_prs_config_index_info_t prs_config_index_info;
	/*^ O, RRC_RRM_RECONFIG_PRS_CONFIG_INDEX_INFO_PRESENT, N, 0, 0 ^*/

	rrc_rrm_prs_transmission_power_info_t    prs_transmission_power_info;
	/*^ O, RRC_RRM_RECONFIG_PRS_TRANSMISSION_POWER_INFO_PRESENT, N, 0, 0 ^*/

	rrc_rrm_prs_muting_config_info_t    prs_muting_config_info;
	/*^ O, RRC_RRM_RECONFIG_PRS_MUTING_CONFIG_INFO_PRESENT, N, 0, 0 ^*/

}rrc_rrm_prs_reconfig_t;

typedef struct _rrc_rrm_prs_reconfig_info_t
{
	rrc_bitmask_t     bitmask;  /*^ BITMASK ^*/
#define RRC_RRM_RECONFIG_PRS_CONFIG_PRESENT	0x01 

	U8  request_type;           /*^ M, 0, H, 0, 1 ^*/
	rrc_rrm_prs_reconfig_t		prs_reconfig;
	/*^ O, RRC_RRM_RECONFIG_PRS_CONFIG_PRESENT, N, 0, 0 ^*/
}rrc_rrm_prs_reconfig_info_t;

/* PRS + */
/* CLPC Stop */

#define RBS_FOR_DCI_1A_SIZE 27
#define RBS_FOR_DCI_1C_SIZE 32

typedef enum
{
    RRC_RBS_PER_TBS_DCI_1A,
    RRC_RBS_PER_TBS_DCI_1C
}rrc_dci_format_for_si_msgs_et;

typedef enum
{
    RRC_UL_SYNC_LOSS_TIMER_INFINITY, 
    RRC_UL_SYNC_LOSS_TIMER_100, 
    RRC_UL_SYNC_LOSS_TIMER_150, 
    RRC_UL_SYNC_LOSS_TIMER_200, 
    RRC_UL_SYNC_LOSS_TIMER_300, 
    RRC_UL_SYNC_LOSS_TIMER_400, 
    RRC_UL_SYNC_LOSS_TIMER_500, 
    RRC_UL_SYNC_LOSS_TIMER_750, 
    RRC_UL_SYNC_LOSS_TIMER_1280, 
    RRC_UL_SYNC_LOSS_TIMER_1920, 
    RRC_UL_SYNC_LOSS_TIMER_2560, 
    RRC_UL_SYNC_LOSS_TIMER_5120, 
    RRC_UL_SYNC_LOSS_TIMER_10240 
}rrc_ul_sync_loss_timer_et;

typedef enum
{
    RRC_NGAP_1 = 0,
    RRC_NGAP_2 = 1
}rrc_n_gap_et;

typedef struct _rrc_si_start_offset_t
{
    U8    si_start_offset; /*^ M, 0, H, 0, 39 ^*/
}rrc_si_start_offset_t;

typedef struct _rrc_si_start_offset_info_t
{
/* SPR-18445 start */
    U8    offset_count; /*^ M, 0, B, 1, MAX_SI_START_OFFSET ^*/
/* SPR-18445 stop */

    rrc_si_start_offset_t    si_start_offset[MAX_SI_START_OFFSET];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_si_start_offset_info_t;


typedef struct _rrc_si_transmission_info_t
{
    rrc_bitmask_t     bitmask;  /*^ BITMASK ^*/
#define RRC_RRM_NUM_OF_TRANSMISSIONS_PRESENT 0x01
#define RRC_RRM_SI_START_OFFSET_INFO_PRESENT 0x02

    U8     si_index;  /*^ M, 0, H, 0, 9 ^*/

    U8     num_of_transmissions;      
    /*^ O, RRC_RRM_NUM_OF_TRANSMISSIONS_PRESENT, B, 1, 8 ^*/

    rrc_si_start_offset_info_t  si_start_offset_info;
    /*^ O, RRC_RRM_SI_START_OFFSET_INFO_PRESENT, N, 0, 0 ^*/

}rrc_si_transmission_info_t;

typedef struct _rrc_si_transmission_info_list_t
{
/* SPR-18445 start */
    U8     count;     /*^ M, 0, B, 1, MAX_SI_MESSAGE ^*/
/* SPR-18445 stop */
    rrc_si_transmission_info_t    si_transmission[MAX_SI_MESSAGE];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_si_transmission_info_list_t;

/* SPS Start */
typedef struct _rrc_rrm_sps_crnti_range_t_
{
    U16 start_sps_crnti; /*^ M, 0, B, 61, 65523 ^*/

    U16 end_sps_crnti;   /*^ M, 0, B, 61, 65523 ^*/
}rrc_rrm_sps_crnti_range_t;
/* SPS Stop */

typedef struct _rrc_mac_reconfig_t
{
    rrc_bitmask_t          bitmask;  /*^ BITMASK ^*/
#define RRC_RRM_RECONFIG_DCI_FORMAT_FOR_SI_MESSAGES_PRESENT 0x01
#define RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1A_PRESENT         0x02
#define RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1C_PRESENT         0x04
#define RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT       0x08
#define RRC_RRM_RECONFIG_UL_SYNC_LOSS_TIMER_PRESENT         0x10
#define RRC_RRM_RECONFIG_PUCCH_CQI_SINR_THRESHOLD_PRESENT   0x20
#define RRC_RRM_RECONFIG_N_GAP_PRESENT                      0x40 
/* SPS Start */
#define RRC_RRM_RECONFIG_SPS_CRNTI_RANGE_PRESENT            0x80
/* SPS Stop */
/* PRS + */
#define RRC_RRM_RECONFIG_PRS_INFO_PRESENT		    0x100
/* PRS - */

    U8 dci_format_for_si_msgs;
    /*^ O, RRC_RRM_RECONFIG_DCI_FORMAT_FOR_SI_MESSAGES_PRESENT, H, 0, 1 ^*/ 
     /* rrc_dci_format_for_si_msgs_et */

    U8 num_rbs_per_tbs_dci_1a[RBS_FOR_DCI_1A_SIZE];
    /*^ O, 2, OCTET_STRING, FIXED, H, 0, 100 ^*/

    U8 num_rbs_per_tbs_dci_1c[RBS_FOR_DCI_1C_SIZE];
    /*^ O, 4, OCTET_STRING, FIXED, H, 0, 100 ^*/

    rrc_si_transmission_info_list_t    si_transmission_info;
    /*^ O, RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT, N, 0, 0 ^*/

    U8 ul_sync_loss_timer;
    /*^ O, RRC_RRM_RECONFIG_UL_SYNC_LOSS_TIMER_PRESENT, H, 0, 12 ^*/
     /*rrc_ul_sync_loss_timer_et*/

    U8 pucch_cqi_sinr_value;
    /*^ O, RRC_RRM_RECONFIG_PUCCH_CQI_SINR_THRESHOLD_PRESENT, N, 0, 0 ^*/
    U8   n_gap;
    /*^ O, RRC_RRM_RECONFIG_N_GAP_PRESENT, H, 0, 1 ^*/
                            /*rrc_n_gap_et */
  /* SPS Start */
  rrc_rrm_sps_crnti_range_t    sps_crnti_range;
  /*^ O, RRC_RRM_RECONFIG_SPS_CRNTI_RANGE_PRESENT, N, 0, 0 ^*/
  /* SPS Stop */

    /* PRS +*/
    rrc_rrm_prs_reconfig_info_t    prs_reconfig_info;
    /*^ O, RRC_RRM_RECONFIG_PRS_INFO_PRESENT, N, 0, 0 ^*/
    /* PRS -*/
  
}rrc_mac_reconfig_t;


/* SPR 23209 FIXED START */
typedef struct _rrc_rrm_mocn_operator_info_t
{
  U8 num_operator_info;
  /*^ M, 0, B, 2, MAX_PLMN_IDS^*/
  U8 operator_id[MAX_PLMN_IDS];
  /*^ M, 0, OCTET_STRING, VARIABLE,H, 0, 5^*/
}rrc_rrm_mocn_operator_info_t;

/* SPR 23209 FIXED END */

/*Coverity 94431 Fix start*/
/*SPR 18672 Fix Start*/
#define MAX_NO_OF_MBSFN_AREAS                   MAX_NUM_CURRENT_SUPPORTED_CELLS *(MAX_MBSFN_AREA_PER_CELL + MAX_RESERVED_MBSFN_AREA_PER_CELL) 
/*SPR 18672 Fix end*/
/*Max no if mbsfn area is multiplied by 2 as 8 areas are reserved and 8
 * unreserved*/
/*Coverity 94431 Fix end*/

typedef struct 
{
   U32   signalling_mcs; /*^ M, 0, H, 0, 3 ^*//* signalling_mcs_et */
   U32   mcch_repetition_period; /*^ M, 0, H, 0, 3 ^*//* mcch_repetition_period_et */
   U32   mcch_modification_period; /*^ M, 0, H, 0, 1 ^*//* mcch_modification_period_et */
   U32   pdcch_length; /*^ M, 0, H, 0, 1 ^*//* pdcch_length_et */
   U8    mcch_offset;  /*^ M, 0, H, 0, 10 ^*/
   U8    sf_alloc_info;  /*^ M, 0, N, 0, 0 ^*/ /* Bit string of size 6 */
   U8    area_id;  /*^ M, 0, N, 0, 0 ^*/
   U8    notification_indicator_r9;  /*^ M, 0, H, 0, 7 ^*/
   U8    area_index;  /*^ M, 0, H, 0, 15 ^*/
/*SPR 18672 Fix Start*/
   U8    area_type;  /*^ M, 0, H, 0, 1 ^*//*m2ap_area_type_et*/
/*SPR 18672 Fix end*/
}m2ap_mcch_related_bcch_info_t;

/* SPR 18672 Fix Start */
typedef m2ap_mcch_related_bcch_info_t m2ap_reserved_mcch_related_bcch_info_t;
/* SPR 18672 Fix End */
typedef struct 
{
  U32 notification_repetition_coeff; /*^ M, 0, H, 0, 1 ^*//* notification_repetition_coeff_et */
  U8  notification_offset; /*^ M,0,H,1,10 ^*/
  U8  notification_sf_Index; /*^ M,0,H,1,6 ^*/
}m2ap_notification_config_t;

typedef struct
{
   U8                                  num_valid_mbsfn_area_info; /*^ M, 0, H, 0, MAX_MBSFN_SUBFRAME_CONFIG ^*/
   m2ap_mcch_related_bcch_info_t       m2ap_mbsfn_area_info_list[MAX_MBSFN_SUBFRAME_CONFIG]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
   m2ap_notification_config_t          m2ap_notification_config; /*^ M, 0, N, 0, 0 ^*/
}sib_type_13_info_t;

typedef struct
{
    U8    area_id; /*^ M, 0,N, 0, 0 ^*/
    U8    area_index; /*^ M, 0,H, 0, 15 ^*/
    U8    flag; /*^ M, 0, H, 0, 3 ^*/ /* mbms_area_flag_et */ 
}mbms_updated_area_info_t;

typedef struct
{
    /*SPR 18170:start*/
   U8                                  num_valid_mbsfn_area_info; /*^ M, 0, H, 0, MAX_MBMS_UPDATED_AREA_LIST ^*/
   mbms_updated_area_info_t
       mbms_updated_area_info[MAX_MBMS_UPDATED_AREA_LIST];
    /*SPR 18170:end*/
   /*^ M,0,OCTET_STRING,VARIABLE ^*/
}mbms_updated_area_info_list_t;




/*SPR 18672 Fix Start*/
#define MAX_MBSFN_AREA_PER_CELL                 8
/*SPR 18672 Fix Start*/
#define MAX_RESERVED_MBSFN_AREA_PER_CELL        0          /*For Reserved areas*/
/*SPR 18672 Fix end*/
#define MAX_RESERVED_CELL_LIST_SIZE  MAX_MBSFN_AREA_PER_CELL + MAX_RESERVED_MBSFN_AREA_PER_CELL
/*SPR 18672 Fix end*/
typedef struct
{
    U8 count;/*^ M, 0,N, 1, 8 ^*/
    m2ap_reserved_mcch_related_bcch_info_t m2ap_reserved_cell_mcch_info[MAX_RESERVED_CELL_LIST_SIZE];/*^ M,0,OCTET_STRING,VARIABLE ^*/
}m2ap_reserved_mcch_related_bcch_info_list_t;

typedef struct _sib13_scheduling_info_t
{
    U8     si_index;
}sib13_scheduling_info_t;

typedef struct _rrc_mac_config_t
{
    /* Coverity 110696 + */
    rrc_bitmask_U32_t          bitmask;  /*^ BITMASK ^*/
    /* Coverity 110696 - */
#define RRC_MAC_ENABLE_FREQUENCY_SELECTIVE_SCHEDULING_PRESENT 0x01
/*SPR_19066_START*/
/* RRC_RRM_UE_INACTIVE_TIME_PRESENT has been deprecated */
/*SPR_19066_END*/
/* CLPC Start */
#define RRC_MAC_TPC_RNTI_PRESENT                              0x04
#define RRC_MAC_POWER_CONTROL_PRESENT                         0x08
/* CLPC Stop */
#define RRC_RRM_CONTENTION_FREE_RACH_TIMER_PRESENT            0x10
#define RRC_RRM_RBS_PER_TBS_DCI_1A_PRESENT                    0x20
#define RRC_RRM_RBS_PER_TBS_DCI_1C_PRESENT                    0x40
#define RRC_RRM_SI_TRANSMISSION_INFO_PRESENT                  0x80
#define RRC_RRM_UL_SYNC_LOSS_TIMER_PRESENT                    0x100
#define RRC_RRM_PUCCH_CQI_SINR_THRESHOLD_PRESENT              0x200  
#define RRC_RRM_N_GAP_PRESENT                                 0x400 
/* SPS Start */
#define RRC_RRM_SPS_CRNTI_RANGE_PRESENT                       0x800
/* SPS Stop */
/* HD-FDD start */
#define RRC_RRM_DUPLEXING_MODE_PRESENT                        0x1000  
#define RRC_RRM_IS_DYNAMIC_PDCCH_DISABLED_PRESENT             0x2000  
/* HD-FDD stop */
/* PRS +*/
#define RRC_RRM_PRS_CONFIG_INFO_PRESENT                       0x4000
/*  LAA Rel13 Code + */
#define RRC_RRM_LAA_SCELL_CONFIGURATION_INFO_PRESENT          0x8000
#define RRC_RRM_DMTC_CONFIGURATION_INFO_PRESENT               0x10000
/*  LAA Rel13 Code - */
/* PRS -*/
#define RRC_RRM_SIB_13_INFO_PRESENCE_FLAG                     0x20000
#define RRC_RRM_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG      0x40000
#define RRC_RRM_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG   0x80000
#define RRC_RRM_EMBMS_AREA_INFO_LIST_PRESENCE_FLAG            0x100000
 /* SPR 23209 FIXED START */
#define RRC_RRM_MOCN_OPERATOR_INFO_PRESENT                    0x200000
 /* SPR 23209 FIXED END */


    
    U8  dl_res_blocks;          /*^ M, 0, B, 1, 100 ^*/
    U8  ul_res_blocks;          /*^ M, 0, B, 1, 100 ^*/
    U8  max_harq_retrans;       /*^ M, 0, B, 1, 8 ^*/
    U8  start_ra_rnti_range;    /*^ M, 0, B, 1, 60 ^*/
    U8  end_ra_rnti_range;      /*^ M, 0, B, 1, 60 ^*/
    U8  dci_format_for_si_msgs; /*^ M, 0, H, 0, 1 ^*/  /* rrc_dci_format_for_si_msgs_et */

    rrc_mac_enable_frequency_selective_scheduling_t  enable_frequency_selective_scheduling;
/*^ O, RRC_MAC_ENABLE_FREQUENCY_SELECTIVE_SCHEDULING_PRESENT ^*/

    /*SPR_19066_START*/
    /* Code Removed */
    /*SPR_19066_END*/
 
    /* CLPC Start */
    rrc_mac_downlink_power_control_common_t downlink_power_control_common;
    /*^ M, 0, N, 0, 0 ^*/

    rrc_mac_tpc_rnti_range_t                tpc_rnti_range;
    /*^ O, RRC_MAC_TPC_RNTI_PRESENT, N, 0, 0 ^*/

    rrc_mac_power_control_enable_t          power_control_enable;
    /*^ O, RRC_MAC_POWER_CONTROL_PRESENT, N, 0, 0 ^*/

    /* CLPC Stop */

    U16  contention_free_rach_timer;
/*^ O, RRC_RRM_CONTENTION_FREE_RACH_TIMER_PRESENT, N, 0, 0 ^*/

    U8   num_rbs_per_tbs_dci_1a[RBS_FOR_DCI_1A_SIZE];
    /*^ O, 32, OCTET_STRING, FIXED, H, 0, 100 ^*/

    U8   num_rbs_per_tbs_dci_1c[RBS_FOR_DCI_1C_SIZE];
    /*^ O, 64, OCTET_STRING, FIXED, H, 0, 100 ^*/

    rrc_si_transmission_info_list_t    si_transmission_info;
    /*^ O, RRC_RRM_SI_TRANSMISSION_INFO_PRESENT, N, 0, 0 ^*/
    
    U8   ul_sync_loss_timer;
    /*^ O, RRC_RRM_UL_SYNC_LOSS_TIMER_PRESENT, H, 0, 12 ^*/ 
                    /*rrc_ul_sync_loss_timer_et */

    U8   pucch_cqi_sinr_value;
    /*^ O, RRC_RRM_PUCCH_CQI_SINR_THRESHOLD_PRESENT, N, 0, 0 ^*/
    U8   n_gap;
    /*^ O, RRC_RRM_N_GAP_PRESENT, H, 0, 1 ^*/
                    /*rrc_n_gap_et */
  /* SPS Start */
  rrc_rrm_sps_crnti_range_t    sps_crnti_range;
  /*^ O, RRC_RRM_SPS_CRNTI_RANGE_PRESENT, N, 0, 0 ^*/

  /* SPS Stop */
    /* HD-FDD start */
    U8   duplexing_mode;
    /*^ O, RRC_RRM_DUPLEXING_MODE_PRESENT, H, 0, 2 ^*/
    U8   dynamic_pdcch;
    /*^ O, RRC_RRM_IS_DYNAMIC_PDCCH_DISABLED_PRESENT, H, 1, 1 ^*/
    /* HD-FDD stop */

    /* PRS +*/
    rrc_rrm_prs_config_info_t    prs_config_info;
    /*^ O, RRC_RRM_PRS_CONFIG_INFO_PRESENT, N, 0, 0 ^*/
    /*SPR 23209 FIXED START  */
    rrc_rrm_mocn_operator_info_t      mocn_operator_info;
    /*^ O, RRC_RRM_MOCN_OPERATOR_INFO_PRESENT , N, 0, 0 ^*/
    /*SPR 23209 FIXED END  */
    /* PRS -*/
    sib_type_13_info_t            sib_type_13_info;/*^ O, RRC_RRM_SIB_13_INFO_PRESENCE_FLAG, N, 0, 0 ^*/
    mbms_updated_area_info_list_t mbms_updated_area_info_list; /*^ O, RRC_RRM_EMBMS_AREA_INFO_LIST_PRESENCE_FLAG, N, 0, 0 ^*/
    m2ap_reserved_mcch_related_bcch_info_list_t    m2ap_reserved_mcch_related_bcch_info_list;/*^ O,RRC_RRM_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG , N, 0, 0 ^*/
    U16                          mbms_transmission_power;/*^ O,RRC_RRM_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG , N, 0, 0 ^*/
    sib13_scheduling_info_t       sib13_scheduling_info; /*^ O, RRC_RRM_SIB_13_INFO_PRESENCE_FLAG, N, 0, 0 ^*/
    /* SPR 19309 + */
    U8                   cfi_value;         /*^ M, 0, B, 1, 4 ^*/
    /* SPR 19309 - */
    /* SPR 19310 + */
    U16                   num_of_eul;        /*^ M, 0, N, 0, 0 ^*/
    U16                   size_of_eul;       /*^ M, 0, N, 0, 0 ^*/
    /* SPR 19310 - */

    /*  LAA Rel13 Code + */
    rrc_radio_res_dedicated_laa_scell_config_t laa_scell_configuration_r13;  /*^ O, RRC_RRM_LAA_SCELL_CONFIGURATION_INFO_PRESENT, N, 0, 0 ^*/
    rrc_dmtc_configuration_r13_t          dmtc_configuration_r13;       /*^ O, RRC_RRM_DMTC_CONFIGURATION_INFO_PRESENT, N, 0, 0 ^*/
    /*  LAA Rel13 Code - */

} rrc_mac_config_t;

/* SPR 21371 Start */
#define RRC_RRM_IPV4_ADDRESS_PRESENT                        0x01  
#define RRC_RRM_IPV6_ADDRESS_PRESENT                        0x02  
#define RRC_RRM_DATA_PORT_PRESENT	  			  			0x04
typedef struct _rrc_l2_comm_info_t
{
    U8	            bitmask;                     /*^ BITMASK ^*/
    U16             port;                       /*^ M, 0, N, 0, 0 ^*/
    U16             data_port;                       /*^ O, RRC_RRM_DATA_PORT_PRESENT, N, 0, 0 ^*/
    U8              ip_addr[IPV4_ADDRESS_LENGTH];    /*^ O, RRC_RRM_IPV4_ADDRESS_PRESENT, OCTET_STRING, FIXED ^*/
    U8              ip_addr6[IPV6_ADDRESS_LENGTH];    /*^ O, RRC_RRM_IPV6_ADDRESS_PRESENT, OCTET_STRING, FIXED ^*/
}rrc_l2_comm_info_t;
/* SPR 21371 End */

typedef struct _rrc_pdcp_comm_info_t
{
    U8	            bitmask;                     /*^ BITMASK ^*/
    U16             port;                       /*^ M, 0, N, 0, 0 ^*/
    U16             data_port_to_rlc;                       /*^ M, 0, N, 0, 0 ^*/
    U16             data_port_to_egtpu;                       /*^ M, 0, N, 0, 0 ^*/
    U8              ip_addr[IPV4_ADDRESS_LENGTH];    /*^ O, RRC_RRM_IPV4_ADDRESS_PRESENT, OCTET_STRING, FIXED ^*/
    U8              ip_addr6[IPV6_ADDRESS_LENGTH];    /*^ O, RRC_RRM_IPV6_ADDRESS_PRESENT, OCTET_STRING, FIXED ^*/
}rrc_pdcp_comm_info_t;
#define RRC_RRM_CRAN_PDCP_COMM_INFO_PRESENT 0x01
#define RRC_RRM_NON_CRAN_PDCP_COMM_INFO_PRESENT 0x02
typedef struct _rrc_lower_layer_comm_info_t
{
    U8				bitmask;			 /*^ BITMASK ^*/
    rrc_l2_comm_info_t mac_comm_info;      /*^ M, 0, N, 0, 0 ^*/    
    rrc_l2_comm_info_t rlc_comm_info;      /*^ M, 0, N, 0, 0 ^*/
    rrc_pdcp_comm_info_t cran_pdcp_comm_info;     /*^ O, RRC_RRM_CRAN_PDCP_COMM_INFO_PRESENT, N, 0, 0 ^*/
    rrc_l2_comm_info_t pdcp_comm_info;     /*^ O, RRC_RRM_NON_CRAN_PDCP_COMM_INFO_PRESENT, N, 0, 0 ^*/
    rrc_l2_comm_info_t egtpu_comm_info;    /*^ M, 0, N, 0, 0 ^*/
    rrc_l2_comm_info_t phy_comm_info;    /*^ M, 0, N, 0, 0 ^*/
}rrc_lower_layer_comm_info_t;
/* SPR 20908 End */


/******************************************************************************
*   These types are shared CSC-LLIM with RRM
******************************************************************************/
#define RRC_RRM_ASSOCIATED_LAYER2_INST_FIELD_PRESENT 0x01
#define RRC_RRM_ASSOCIATED_PDCP_INST_FIELD_PRESENT 	0x02
#define RRC_RRM_ASSOCIATED_RLC_INST_FIELD_PRESENT	0x04	
#define RRC_RRM_ASSOCIATED_MAC_INST_FIELD_PRESENT	0x08

typedef struct _rrc_rrm_cell_config_t
{
    U8							bitmask;			 /*^ BITMASK ^*/
    rrc_phy_cell_parameters_t   cell_parameters;
    rrc_phy_sync_signals_t      sync_signals;
    rrc_mac_config_t            mac_config;
    /* SPR 20908 Start */
    rrc_lower_layer_comm_info_t l2_comm_info_params;      /*^ M, 0, N, 0, 0 ^*/
    U8                          associated_layer2_inst;        /*^ O, RRC_RRM_ASSOCIATED_LAYER2_INST_FIELD_PRESENT, N, 0 , 0 ^*/
    /* SPR 20908 End */
    U8                          associated_pdcp_inst;	/*^ O, RRC_RRM_ASSOCIATED_PDCP_INST_FIELD_PRESENT, N, 0 , 0 ^*/
    U8                          associated_rlc_inst;		/*^ O, RRC_RRM_ASSOCIATED_RLC_INST_FIELD_PRESENT, N, 0 , 0 ^*/
    U8                          associated_mac_inst;		/*^ O, RRC_RRM_ASSOCIATED_MAC_INST_FIELD_PRESENT, N, 0 , 0 ^*/
} rrc_rrm_cell_config_t;


/******************************************************************************
*   End of these types are shared CSC-LLIM with RRM
******************************************************************************/

/******************************************************************************
*   These types are shared UECC-LLIM with PHY
*   Actually this is rrc_phy_physical_config_dedicated_t
******************************************************************************/
typedef struct _rrc_phy_scheduling_request_config_param_t
{
    U16 sr_pucch_resource_index;  /*^ M, 0, H, 0, 2047 ^*/
    U8  sr_configuration_index;   /*^ M, 0, H, 0, 157 ^*/
    U8  dsr_trans_max;            /*^ M, 0, H, 0, 4 ^*/ /* dsr_trans_max_et */
} rrc_phy_scheduling_request_config_param_t;

typedef struct _rrc_phy_scheduling_request_config_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_SCHEDULING_REQUEST_CONFIG_PARAM_PRESENT 0x01

    rrc_phy_scheduling_request_config_param_t   scheduling_request_config_param;
/*^ O, RRC_PHY_SCHEDULING_REQUEST_CONFIG_PARAM_PRESENT ^*/

} rrc_phy_scheduling_request_config_t;

/* cr_316 And 247 structure */
typedef struct _rrc_phy_cqi_report_config_v920_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CQI_MASK_R9_PRESENT           0x01
#define RRC_PHY_PMI_RI_REPORT_R9_PRESENT      0x02

    U8            cqi_mask_r9;
/*^ O, RRC_PHY_CQI_MASK_R9_PRESENT, H, 0, 0 ^*/  /*cqi_mask_r9_et*/


    U8       pmi_ri_report_r9;
/*^ O, RRC_PHY_PMI_RI_REPORT_R9_PRESENT, H, 0, 0 ^*/  /*pmi_ri_report_r9_et*/

}rrc_phy_cqi_report_config_v920_t;


typedef struct _rrc_phy_codebook_subset_restriction_v920_t
{
    U16 type;  /*^ M, 0, H, 0, 1 ^*/ /* codebook_subset_restriction_v920_type_et */
    U8  value[4];   /*^ M, O, OCTET_STRING, FIXED ^*/
}rrc_phy_codebook_subset_restriction_v920_t;


typedef struct _rrc_phy_antenna_info_v920_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT  0x01

    rrc_phy_codebook_subset_restriction_v920_t codebook_subset_restriction_v920;
/*^ O, RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT, N, 0, 0 ^*/

}rrc_phy_antenna_info_v920_t;

typedef struct _rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_t
{
    U8  trigger1;
/*^ M, 0, N, 0, 0 ^*/

    U8  trigger2;
/*^ M, 0, N, 0, 0 ^*/

} rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_t;

typedef struct _rrc_phy_cqi_report_aperiodic_r10_setup_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_CQI_REPORT_APERIODIC_R10_SETUP_APERIODIC_CSI_TRIGGER_PRESENT  0x01

    U8  cqi_reporting_mode_aperiodic;
/*^ M, 0, H, 0, 4 ^*/ /* cqi_reporting_mode_aperiodic_et */

   rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_t  aperiodic_csi_trigger;
/*^ O, RRC_CQI_REPORT_APERIODIC_R10_SETUP_APERIODIC_CSI_TRIGGER_PRESENT, N, 0, 0 ^*/

}rrc_phy_cqi_report_aperiodic_r10_setup_t;

typedef struct _rrc_phy_cqi_report_aperiodic_r10_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CQI_REPORT_APERIODIC_R10_SETUP_PRESENT  0x01
    
    U8 request_type;            /*^ M, 0, H, 0, 1 ^*/    
    
    rrc_phy_cqi_report_aperiodic_r10_setup_t setup;
/*^ O, RRC_PHY_CQI_REPORT_APERIODIC_R10_SETUP_PRESENT, N, 0, 0 ^*/

}rrc_phy_cqi_report_aperiodic_r10_t;

typedef struct _rrc_phy_cqi_format_ind_wideband_r10_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CQI_FORMAT_IND_PERIODIC_WIDEBAND_CQI_REPORT_MODE_PRESENT  0x01

   U8  csi_report_mode;
/*^ O, RRC_PHY_CQI_FORMAT_IND_PERIODIC_WIDEBAND_CQI_REPORT_MODE_PRESENT, H, 0, 1 ^*/ /* cqi_periodic_format_ind_widebandCQI_csi_reportMode_r10_et */

}rrc_phy_cqi_format_ind_wideband_r10_t;

typedef struct _rrc_phy_cqi_format_ind_subband_r10_t
{
   U8  k;
/*^ M, 0, B, 1, 4 ^*/ 

   U8 periodicy_factor;
/*^ M, 0, H, 0, 1 ^*/  /* cqi_periodic_format_ind_subband_cqi_periodicy_factor_et*/

}rrc_phy_cqi_format_ind_subband_r10_t;

typedef struct _rrc_phy_cqi_format_ind_periodic_r10_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CQI_FORMAT_IND_PERIODIC_WIDEBAND_CQI_PRESENT  0x01
#define RRC_PHY_CQI_FORMAT_IND_PERIODIC_SUBBAND_CQI_PRESENT   0x02

    rrc_phy_cqi_format_ind_wideband_r10_t  cqi_format_ind_wideband;
/*^ O, RRC_PHY_CQI_FORMAT_IND_PERIODIC_WIDEBAND_CQI_PRESENT, N, 0, 0 ^*/

    rrc_phy_cqi_format_ind_subband_r10_t cqi_format_ind_subband;
/*^ O, RRC_PHY_CQI_FORMAT_IND_PERIODIC_SUBBAND_CQI_PRESENT, N, 0, 0 ^*/

} rrc_phy_cqi_format_ind_periodic_r10_t;

typedef struct _rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CQI_PERIODIC_R10_SETUP_CSI_RI_CONFIG_INDEX2_PRESENT  0x01

    U16  cqi_pmi_config_index2;
/*^ M, 0, H, 0, 1023 ^*/

    U16  ri_config_index2;
/*^ O, RRC_PHY_CQI_PERIODIC_R10_SETUP_CSI_RI_CONFIG_INDEX2_PRESENT, H, 0, 1023 ^*/

} rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_t;

typedef struct _rrc_phy_cqi_report_periodic_r10_setup_csi_config_index
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_SETUP_PRESENT  0x01
    
    U8 request_type;        /*^ M, 0, H, 0, 1 ^*/

    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index_setup_t setup;
/*^ O, RRC_PHY_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_SETUP_PRESENT, N, 0, 0 ^*/

} rrc_phy_cqi_report_periodic_r10_setup_csi_config_index;

typedef struct _rrc_phy_cqi_report_periodic_r10_setup_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_CQI_REPORT_PERIODIC_R10_SETUP_PUCCH_REPORT_INDEX_P1_PRESENT  0x01
#define RRC_CQI_REPORT_PERIODIC_R10_SETUP_RI_CONFIG_INDEX_PRESENT     0x02
#define RRC_CQI_REPORT_PERIODIC_R10_SETUP_CQI_MASK_PRESENT            0x04
#define RRC_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_PRESENT    0x08

    U16  cqi_pucch_resource_index;
/*^ M, 0, H, 0,1184 ^*/ 

    U16  cqi_pucch_resource_index_p1;
/*^ O, RRC_CQI_REPORT_PERIODIC_R10_SETUP_PUCCH_REPORT_INDEX_P1_PRESENT, H, 0,1184 ^*/ 

    U16  cqi_pmi_config_index;
/*^ M, 0, H, 0, 1023 ^*/ 

   rrc_phy_cqi_format_ind_periodic_r10_t  cqi_format_ind_periodic;
/*^ M, 0, N, 0, 0 ^*/ 

    U16  ri_config_index;
/*^ O, RRC_CQI_REPORT_PERIODIC_R10_SETUP_RI_CONFIG_INDEX_PRESENT, H, 0, 1023 ^*/ 

    U8  simultaneous_ack_nack_cqi;
/*^ M, 0, H, 0, 1 ^*/  /* rrc_bool_et */

    U8            cqi_mask_r9;
/*^ O, RRC_CQI_REPORT_PERIODIC_R10_SETUP_CQI_MASK_PRESENT, H, 0, 0 ^*/  /*cqi_mask_r9_et*/

    rrc_phy_cqi_report_periodic_r10_setup_csi_config_index         csi_config_index;
/*^ O, RRC_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_PRESENT, N, 0, 0 ^*/


}rrc_phy_cqi_report_periodic_r10_setup_t;

typedef struct _rrc_phy_cqi_report_periodic_r10_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CQI_REPORT_PERIODIC_R10SETUP_PRESENT  0x01
    
    U8  request_type;           /*^ M, 0, H, 0, 1 ^*/
    
    rrc_phy_cqi_report_periodic_r10_setup_t setup;
/*^ O, RRC_PHY_CQI_REPORT_PERIODIC_R10SETUP_PRESENT, N, 0, 0 ^*/

}rrc_phy_cqi_report_periodic_r10_t;

#define MAX_SUBFRAME_CONFIG1_5_TDD 3
#define MAX_SUBFRAME_CONFIG0_TDD   9
#define MAX_SUBFRAME_CONFIG6_TDD   8
#define MAX_SUBFRAME_PATTERN_FDD   5

typedef struct _meas_subframe_pattern_tdd_r10_t
{
#define RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG1_5_R10 0x01
#define RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG0_R10   0x02
#define RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG6_R10   0x04
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/

    U8    subframe_config1_5_r10[MAX_SUBFRAME_CONFIG1_5_TDD];
/*^ O, RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG1_5_R10, OCTET_STRING, FIXED ^*/

    U8    subframe_config0_r10[MAX_SUBFRAME_CONFIG0_TDD];
/*^ O, RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG0_R10, OCTET_STRING, FIXED ^*/

    U8    subframe_config6_r10[MAX_SUBFRAME_CONFIG6_TDD];
/*^ O, RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG6_R10, OCTET_STRING, FIXED ^*/
}rrc_phy_meas_subframe_pattern_tdd_r10_t;

typedef struct _meas_subframe_pattern_fdd_r10_t
{
    U8    data[MAX_SUBFRAME_PATTERN_FDD];
/*^ M, 0, OCTET_STRING, FIXED ^*/
}rrc_phy_meas_subframe_pattern_fdd_r10_t;

typedef struct _meas_subframe_pattern_r10_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_MEAS_SUBFRAME_PATTERN_FDD_R10       0x01
#define RRC_PHY_MEAS_SUBFRAME_PATTERN_TDD_R10       0x02
    rrc_phy_meas_subframe_pattern_fdd_r10_t subframe_pattern_fdd_r10;
    /*^ O, RRC_PHY_MEAS_SUBFRAME_PATTERN_FDD_R10, N, 0, 0 ^*/

    rrc_phy_meas_subframe_pattern_tdd_r10_t subframe_pattern_tdd_r10;
    /*^ O, RRC_PHY_MEAS_SUBFRAME_PATTERN_TDD_R10, N, 0, 0 ^*/
}rrc_phy_meas_subframe_pattern_r10_t;

typedef struct _cqi_report_csi_subframe_pattern_config_r10_setup_t
{
    rrc_phy_meas_subframe_pattern_r10_t    csi_meas_subframe_set1_r10;  /*^ M, 0, N, 0, 0 ^*/
    rrc_phy_meas_subframe_pattern_r10_t    csi_meas_subframe_set2_r10;  /*^ M, 0, N, 0, 0 ^*/
}rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup_t;

typedef struct _rrc_phy_cqi_report_csi_subframePattern_r10_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAMEPATTERN_CONFIG_SETUP     0x01
    U8  request_type;
    /*^ M, 0, H, 0, 1 ^*/

    rrc_phy_cqi_report_csi_subframe_pattern_config_r10_setup_t    csi_subframe_pattern_config_r10_setup;
    /*^ O, RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAMEPATTERN_CONFIG_SETUP, N, 0, 0 ^*/
}rrc_phy_cqi_report_csi_subframePattern_r10_t;

typedef struct _rrc_cqi_report_config_r10_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_APERIODIC_R10_PRESENT  0x01
#define RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_PERIODIC_R10_PRESENT   0x02
#define RRC_PHY_CQI_REPORT_CONFIG_R10_PMI_RI_REPORT_R9_PRESENT      0x04
#define RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAME_PATTERN_CONFIG_R10_PRESENT        0x08

    rrc_phy_cqi_report_aperiodic_r10_t   cqi_report_aperiodic_r10;
/*^ O, RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_APERIODIC_R10_PRESENT, N, 0, 0 ^*/

    S8  nomPDSCH_rs_epre_offset;
/*^ M, 0, B, -1, 6 ^*/

    rrc_phy_cqi_report_periodic_r10_t    cqi_report_periodic_r10;
/*^ O, RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_PERIODIC_R10_PRESENT, N, 0, 0 ^*/

    U8    cqi_report_pmi_ri_report_r10;
/*^ O, RRC_PHY_CQI_REPORT_CONFIG_R10_PMI_RI_REPORT_R9_PRESENT, H, 0, 0 ^*/

    rrc_phy_cqi_report_csi_subframePattern_r10_t    csi_subframePattern_r10;
/*^ O, RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAME_PATTERN_CONFIG_R10_PRESENT, N, 0, 0 ^*/

}rrc_cqi_report_config_r10_t;

typedef struct _rrc_phy_ue_transmit_antenna_selection_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT  0x01
    U8 request_type;        /*^ M, 0, H, 0, 1 ^*/
    
    U8  ue_transmit_antenna_selection_type;
/*^ O, RRC_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT, H, 0, 1 ^*/
/* ue_transmit_antenna_selection_type_et */

} rrc_phy_ue_transmit_antenna_selection_t;

typedef struct _rrc_phy_codebook_subset_restriction_t
{
    U16 type;  /*^ M, 0, H, 0, 7 ^*/ /* codebook_subset_restriction_type_et */

    U8  value[8];   /*^ M, O, OCTET_STRING, FIXED ^*/
} rrc_phy_codebook_subset_restriction_t;

typedef struct _rrc_phy_antenna_information_dedicated_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT 0x01

    U8  transmission_mode;     /*^ M, 0, H, 0, 7 ^*/ /* transmission_mode_et */

    rrc_phy_codebook_subset_restriction_t   codebook_subset_restriction;
/*^ O, RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT ^*/

    rrc_phy_ue_transmit_antenna_selection_t ue_transmit_antenna_selection;
} rrc_phy_antenna_information_dedicated_t;

typedef struct _rrc_phy_antenna_information_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT  0x01

    rrc_phy_antenna_information_dedicated_t antenna_information_explicit_value;
/*^ O, RRC_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT ^*/

} rrc_phy_antenna_information_t;

typedef struct _rrc_phy_sounding_rs_ul_config_dedicated_param_t
{
    U8  srs_bandwidth;         /*^ M, 0, H, 0, 3 ^*/ /* srs_bandwidth_et */
    U8  srs_hopping_bandwidth;
/*^ M, 0, H, 0, 3 ^*/ /* srs_hopping_bandwidth_et */

    U8  frequency_domain_position;  /*^ M, 0, H, 0, 23 ^*/
    U8  duration;                   /*^ M, 0, H, 0, 1 ^*/  /* rrc_bool_et */
    U16 srs_configuration_index;    /*^ M, 0, H, 0, 1023 ^*/
    U8  transmission_comb;          /*^ M, 0, H, 0, 1 ^*/
    U8  cyclic_shift;               /*^ M, 0, H, 0, 7 ^*/  /* cyclic_shift_et */
} rrc_phy_sounding_rs_ul_config_dedicated_param_t;

typedef struct _rrc_phy_sounding_rs_ul_config_dedicated_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PARAM_PRESENT   0x01

    rrc_phy_sounding_rs_ul_config_dedicated_param_t
        sounding_rs_ul_config_dedicated_param;
/*^ O, RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PARAM_PRESENT ^*/

} rrc_phy_sounding_rs_ul_config_dedicated_t;

typedef struct _rrc_phy_subband_cqi_param_param_t
{
    U8  k; /*^ M, 0, B, 1, 4 ^*/
} rrc_phy_subband_cqi_param_param_t;

typedef struct _rrc_phy_cqi_format_indicator_periodic_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_SUBBAND_CQI_PARAM_PRESENT   0x01

    rrc_phy_subband_cqi_param_param_t   subband_cqi_param;
/*^ O, RRC_PHY_SUBBAND_CQI_PARAM_PRESENT ^*/

} rrc_phy_cqi_format_indicator_periodic_t;

typedef struct _rrc_phy_cqi_reporting_periodic_param_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CQI_RI_CONFIG_INDEX_PRESENT 0x01

    U16 cqi_pucch_resource_index; /*^ M, 0, H, 0, 1185 ^*/
    U16 cqi_pmi_config_index;     /*^ M, 0, H, 0, 1023 ^*/

    rrc_phy_cqi_format_indicator_periodic_t cqi_format_indicator_periodic;

    U16 ri_config_index;
/*^ O, RRC_PHY_CQI_RI_CONFIG_INDEX_PRESENT, H, 0, 1023 ^*/

    U8  simultaneous_ack_nack_and_cqi;  /*^ M, 0, H, 0, 1 ^*/  /* rrc_bool_et */
} rrc_phy_cqi_reporting_periodic_param_t;

typedef struct _rrc_phy_cqi_reporting_periodic_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT    0x01

    rrc_phy_cqi_reporting_periodic_param_t  cqi_reporting_periodic_param;
/*^ O, RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT ^*/

} rrc_phy_cqi_reporting_periodic_t;

typedef struct _rrc_phy_cqi_reporting_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_CQI_REPORTING_PERIODIC_PRESENT          0x01
#define RRC_PHY_CQI_REPORTING_MODE_APERIODIC_PRESENT    0x02

    U8  cqi_reporting_mode_aperiodic;
/*^ O, RRC_PHY_CQI_REPORTING_MODE_APERIODIC_PRESENT, H, 0, 4 ^*/
/* cqi_reporting_mode_aperiodic_et */

    S8  nom_pdsch_rs_epre_offset; /*^ M, 0, B, -1, 6 ^*/

    rrc_phy_cqi_reporting_periodic_t    cqi_reporting_periodic;
/*^ O, RRC_PHY_CQI_REPORTING_PERIODIC_PRESENT ^*/

} rrc_phy_cqi_reporting_t;

typedef struct _rrc_phy_tpc_index_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define TPC_INDEX_FORMAT_3_PRESENT  0x01
#define TPC_INDEX_FORMAT_3A_PRESENT 0x02

    U8  index_of_format3;     /*^ O, TPC_INDEX_FORMAT_3_PRESENT, B, 1, 15 ^*/
    U8  index_of_format3a;    /*^ O, TPC_INDEX_FORMAT_3A_PRESENT, B, 1, 31 ^*/
} rrc_phy_tpc_index_t;

typedef struct _rrc_phy_tpc_pdcch_config_param_t
{
    U8                  tpc_rnti[2];    /*^ M, O, OCTET_STRING, FIXED ^*/
    rrc_phy_tpc_index_t tpc_index;
} rrc_phy_tpc_pdcch_config_param_t;

typedef struct _rrc_phy_tpc_pdcch_configuration_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_TPC_PDCCH_CONFIG_PARAM_PRESENT  0x01

    rrc_phy_tpc_pdcch_config_param_t    tpc_pdcch_config_param;
/*^ O, RRC_PHY_TPC_PDCCH_CONFIG_PARAM_PRESENT ^*/

} rrc_phy_tpc_pdcch_configuration_t;

typedef enum
{
  RRC_FC0,
  RRC_FC1,
  RRC_FC2,
  RRC_FC3,
  RRC_FC4,
  RRC_FC5,
  RRC_FC6,
  RRC_FC7,
  RRC_FC8,
  RRC_FC9,
  RRC_FC11,
  RRC_FC13,
  RRC_FC15,
  RRC_FC17,
  RRC_FC19
}rrc_filter_coefficient_et;

typedef struct _rrc_phy_uplink_power_control_dedicated_t
{
    S8  p0_ue_pusch;           /*^ M, 0, B, -8, 7 ^*/
    U8  delta_mcs_enabled;     /*^ M, 0, H, 0, 1 ^*/ /* delta_mcs_enabled_et */
    U8  accumulation_enabled;  /*^ M, 0, H, 0, 1 ^*/   /* rrc_bool_et */
    S8  p0_ue_pucch;           /*^ M, 0, B, -8, 7 ^*/
    U8  p_srs_offset;          /*^ M, 0, H, 0, 15 ^*/
    U8  filter_coefficient;    /*^ M, 0, H, 0, 14 ^*/ /* rrc_filter_coefficient_et */
} rrc_phy_uplink_power_control_dedicated_t;

typedef struct _rrc_phy_pusch_configuration_dedicated_t
{
    U8  beta_offset_ack_index; /*^ M, 0, H, 0, 15 ^*/
    U8  beta_offset_ri_index; /*^ M, 0, H, 0, 15 ^*/
    U8  beta_offset_cqi_index; /*^ M, 0, H, 0, 15 ^*/
} rrc_phy_pusch_configuration_dedicated_t;

typedef struct _rrc_phy_ack_nack_repetition_param_t
{
    U8  factor; /*^ M, 0, H, 0, 2 ^*/  /* ack_nack_repetition_factor_et */
    U16 an_rep; /*^ M, 0, H, 0, 2047 ^*/
} rrc_phy_ack_nack_repetition_param_t;

typedef struct _rrc_phy_pucch_configuration_dedicated_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_ACK_NACK_REPETITION_PARAM_PRESENT   0x01
#define RRC_PHY_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT  0x02

    rrc_phy_ack_nack_repetition_param_t ack_nack_repetition_param;
/*^ O, RRC_PHY_ACK_NACK_REPETITION_PARAM_PRESENT ^*/

    U8                                  tdd_ack_nack_feedback_mode;
/*^ O, RRC_PHY_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT, H, 0, 1 ^*/
/* tdd_ack_nack_feedback_mode_et */

} rrc_phy_pucch_configuration_dedicated_t;

typedef struct _rrc_phy_pdsch_configuration_dedicated_t
{
    U8  p_a;    /*^ M, 0, H, 0, 7 ^*/ /* pdsch_configuration_dedicated_p_a_et */
} rrc_phy_pdsch_configuration_dedicated_t;

typedef enum
{

   TM_1 = 0,
   TM_2
} antenna_info_ul_r10_et;

typedef struct _rrc_antenna_info_ul_r10_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_ANTENNA_INFO_UL_R10_TRANSMISSION_MODE_PRESENT             0x01
#define RRC_ANTENNA_INFO_UL_R10_FOUR_ANTENNA_PORT_ACTIVATED_PRESENT   0x02

    U8  transmission_mode_ul;
/*^ O, RRC_ANTENNA_INFO_UL_R10_TRANSMISSION_MODE_PRESENT,H, 0, 1 ^*/  /* antenna_info_ul_r10_et */

    U8   four_antenna_port_activated;
/*^ O, RRC_ANTENNA_INFO_UL_R10_FOUR_ANTENNA_PORT_ACTIVATED_PRESENT, H, 0, 0  ^*/

} rrc_antenna_info_ul_r10_t;

typedef struct
{
    U8 num_bits;  /*^ M, 0, B, 2, 109 ^*/

    U8  value[CBSR_MAX_VALUE_V1020];   /*^ M, O, OCTET_STRING, FIXED ^*/
} rrc_codebook_subset_restriction_r10_t;

typedef struct
{
    rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_CODEBOOK_SUBSET_RESTRICTION_PRESENT               0x01

    U8  transmission_mode;             
    /*^ M, 0, H, 0, 8 ^*/ /* rrc_rrm_transmission_mode_r10_et */

    rrc_codebook_subset_restriction_r10_t codebook_subset_restriction_r10;
    /*^ O, RRM_CODEBOOK_SUBSET_RESTRICTION_PRESENT  ^*/ 

    rrc_phy_ue_transmit_antenna_selection_t ue_transmit_antenna_selection;
    /*^ M, 0, N, 0, 0 ^*/

} rrc_antenna_info_dedicated_r10_t;

typedef struct _rrc_antenna_info_r10_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_ANTENNA_INFO_R10_EXPLICIT_VALUE_PRESENT   0x01

    rrc_antenna_info_dedicated_r10_t explicit_value;
/*^ O, RRC_ANTENNA_INFO_R10_EXPLICIT_VALUE_PRESENT ^*/

} rrc_antenna_info_r10_t;

typedef struct _rrc_additional_spectrum_emission_ca_r10_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_ADDITIONAL_SPECTRUM_EMISSION_CA_R10_PCELL_PRESENT   0x01
    
    U8 request_type;            /*^ M, 0, H, 0, 1 ^*/
    
    U8      additional_spectrum_emission_pcell;
    /*^ O, RRC_PHY_ADDITIONAL_SPECTRUM_EMISSION_CA_R10_PCELL_PRESENT, B, 1, 32 ^*/

} rrc_additional_spectrum_emission_ca_r10_t;

typedef struct
{
/* SPR-18445 start */
  U8    count;          /*^ M, 0, B, 1, MAX_N1PUCCH_AN_CS ^*/
/* SPR-18445 stop */
  U16   n1_pucch_an_cs[MAX_N1PUCCH_AN_CS]; /*^ M, 0, OCTET_STRING, VARIABLE, H, 0, 2047 ^*/
}n1pucch_an_cs_r10_t;

typedef struct
{
/* SPR-18445 start */
  U8                    count;          /*^ M, 0, B, 1, MAX_N1PUCCH_AN_CS_LIST ^*/
/* SPR-18445 stop */
  n1pucch_an_cs_r10_t   n1_pucch_an_cs[MAX_N1PUCCH_AN_CS_LIST]; /*^ M, 0, OCTET_STRING, VARIABLE, N, 0, 0^*/
}n1pucch_an_cs_list_r10_t;

typedef struct _rrc_channel_selection_n1_pucch_an_cs_r10_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_N1PUCCH_AN_CS_LIST_R10_PRESENT                     0x01
    
    U8 request_type;        /*^ M, 0, H, 0, 1 ^*/
    
    n1pucch_an_cs_list_r10_t                            n1pucch_an_cs_list_r10;
    /*^ O, RRC_N1PUCCH_AN_CS_LIST_R10_PRESENT ^*/
} rrc_channel_selection_n1_pucch_an_cs_r10_t;

typedef struct _rrc_pucch_channel_selection_config_v1020_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_N1PUCCH_AN_CS_R10_PRESENT   0x01

    rrc_channel_selection_n1_pucch_an_cs_r10_t  channel_selection_n1_pucch_an_cs_r10;
    /*^ O, RRC_N1PUCCH_AN_CS_R10_PRESENT ^*/

}rrc_pucch_channel_selection_config_v1020_t;

typedef struct _rrc_phy_physical_config_dedicated_extended_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_ADDITIONAL_SPECTRUM_EMISSION_CA_R10_PRESENT   0x01
#define RRC_PHY_PUCCH_CHANNEL_SELECTION_CONFIG_R10_PRESENT    0x02

    rrc_additional_spectrum_emission_ca_r10_t      additional_spectrum_emission_ca;
    /*^ O, RRC_PHY_ADDITIONAL_SPECTRUM_EMISSION_CA_R10_PRESENT ^*/

    rrc_pucch_channel_selection_config_v1020_t     pucch_channel_selection_config_v1020; 
    /*^ O, RRC_PHY_PUCCH_CHANNEL_SELECTION_CONFIG_R10_PRESENT ^*/

} rrc_phy_physical_config_dedicated_extended_t;

typedef struct _rrc_phy_physical_config_dedicated_t
{
    U16     bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_PDSCH_CONFIGURATION_DEDICATED_PRESENT   0x001
#define RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT   0x002
#define RRC_PHY_PUSCH_CONFIGURATION_DEDICATED_PRESENT   0x004
#define RRC_PHY_UPLINK_POWER_CONTROL_DEDICATED_PRESENT  0x008
#define RRC_PHY_TPC_PDCCH_CONFIG_PUCCH_PRESENT          0x010
#define RRC_PHY_TPC_PDCCH_CONFIG_PUSCH_PRESENT          0x020
#define RRC_PHY_CQI_REPORTING_PRESENT                   0x040
#define RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PRESENT 0x080
#define RRC_PHY_ANTENNA_INFORMATION_PRESENT             0x100
#define RRC_PHY_SCHEDULING_REQUEST_CONFIG_PRESENT       0x200
#define RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT          0x400
#define RRC_PHY_ANTENNA_INFO_V920_PRESENT               0x800
#define RRC_PHY_ANTENNA_INFO_R10_PRESENT                0x1000
#define RRC_PHY_ANTENNA_INFO_UL_R10_PRESENT             0x2000
#define RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT           0x4000
#define RRC_PHY_CONFIG_DEDICATED_EXTENDED_PRESENT       0x8000


    rrc_phy_pdsch_configuration_dedicated_t     pdsch_configuration_dedicated;
/*^ O, RRC_PHY_PDSCH_CONFIGURATION_DEDICATED_PRESENT ^*/

    rrc_phy_pucch_configuration_dedicated_t     pucch_configuration_dedicated;
/*^ O, RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT ^*/

    rrc_phy_pusch_configuration_dedicated_t     pusch_configuration_dedicated;
/*^ O, RRC_PHY_PUSCH_CONFIGURATION_DEDICATED_PRESENT ^*/

    rrc_phy_uplink_power_control_dedicated_t    uplink_power_control_dedicated;
/*^ O, RRC_PHY_UPLINK_POWER_CONTROL_DEDICATED_PRESENT ^*/

    rrc_phy_tpc_pdcch_configuration_t           tpc_pdcch_config_pucch;
/*^ O, RRC_PHY_TPC_PDCCH_CONFIG_PUCCH_PRESENT ^*/

    rrc_phy_tpc_pdcch_configuration_t           tpc_pdcch_config_pusch;
/*^ O, RRC_PHY_TPC_PDCCH_CONFIG_PUSCH_PRESENT ^*/

    rrc_phy_cqi_reporting_t                     cqi_reporting;
/*^ O, RRC_PHY_CQI_REPORTING_PRESENT ^*/

    rrc_phy_sounding_rs_ul_config_dedicated_t   sounding_rs_ul_config_dedicated;
/*^ O, RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PRESENT ^*/

    rrc_phy_antenna_information_t               antenna_information;
/*^ O, RRC_PHY_ANTENNA_INFORMATION_PRESENT ^*/

    rrc_phy_scheduling_request_config_t         scheduling_request_config;
/*^ O, RRC_PHY_SCHEDULING_REQUEST_CONFIG_PRESENT ^*/

/* cr_316 And 247 structres */
    rrc_phy_cqi_report_config_v920_t            cqi_report_config_v920;
/*^ O, RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT ^*/

    rrc_phy_antenna_info_v920_t                 antenna_info_v920;
/*^ O, RRC_PHY_ANTENNA_INFO_V920_PRESENT ^*/

    rrc_antenna_info_r10_t         antenna_info_r10;
    /*^ O, RRC_PHY_ANTENNA_INFO_R10_PRESENT ^*/

    rrc_antenna_info_ul_r10_t         antenna_info_ul_r10;
    /*^ O, RRC_PHY_ANTENNA_INFO_UL_R10_PRESENT ^*/

    rrc_cqi_report_config_r10_t              cqi_report_config_r10;
    /*^ O, RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT ^*/

    rrc_phy_physical_config_dedicated_extended_t        phy_physical_config_dedicated_extended;
    /*^ O, RRC_PHY_CONFIG_DEDICATED_EXTENDED_PRESENT ^*/


} rrc_phy_physical_config_dedicated_t;

/******************************************************************************
*   End of these types are shared UECC-LLIM with PHY
******************************************************************************/

/******* TDD changes start *******/
typedef enum
{

   DUPLEXING_MODE_TDD = 0,
   DUPLEXING_MODE_FDD = 1,
  /* HD-FDD start */   
   DUPLEXING_MODE_HD_FDD = 2
  /* HD-FDD stop */   
} duplexing_mode_et;

typedef enum
{
        TDD_ACK_NACK_FEEDBACK_MODE_BUNDLING = 0,
            TDD_ACK_NACK_FEEDBACK_MODE_MULTIPLEXING = 1
} tdd_ack_nack_feedback_mode_et;

/******* TDD changes stop *******/




/******************************************************************************
*   These types are shared UECC-LLIM with MAC
******************************************************************************/

/* Enum values for MAC RLC mode */
typedef enum
{
    MAC_RLC_MODE_TM = 0,
    MAC_RLC_MODE_UM,
    MAC_RLC_MODE_AM
} rrc_mac_rlc_mode_et;

/* Enum values for Bearer type*/
typedef enum
{
    BEARER_TYPE_GBR = 0,
    BEARER_TYPE_NON_GBR = 1
} rrc_bearer_type_et;



typedef struct _rrc_mac_configure_status_report_required_enb_t
{
    U8  enb_psr_req_flag; /*^ M, 0, H, 0, 1 ^*/  /* rrc_bool_et */ 
} rrc_mac_configure_status_report_required_enb_t; 

typedef struct _rrc_mac_ca_lc_bandwidth_dist_info_t
{
    U8 band_width_distribution[MAX_CELLS_PER_UE]; /*^ M, 0, OCTET_STRING, FIXED, H, 0, 100 ^*/
}rrc_mac_ca_lc_bandwidth_dist_info_t;

#define RRC_MAC_ENB_PSR_REQ_PRESENT 0x01
#define RRC_MAC_DL_CREATE_CA_LC_BANDWIDTH_DIST_INFO_PRESENT 0x02
#define RRC_MAC_DC_BEARER_DL_TYPE_INFO_PRESENT 0x04
typedef struct _rrc_mac_dl_lc_create_req_t
{
    rrc_bitmask_t            optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    U8  lch_priority;                   /*^ M, 0, B, 1, 16 ^*/
    U8  rlc_sn_field_length;            /*^ M, 0, B, 5, 10 ^*/
    rrc_mac_configure_status_report_required_enb_t     configure_status_report_required_enb;
    /*^ TLV, RRC_MAC_ENB_PSR_REQ, RRC_MAC_ENB_PSR_REQ_PRESENT ^*/
    rrc_mac_ca_lc_bandwidth_dist_info_t   ca_lc_bandwidth_dist_info;
    /*^ TLV, RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO, RRC_MAC_DL_CREATE_CA_LC_BANDWIDTH_DIST_INFO_PRESENT ^*/
} rrc_mac_dl_lc_create_req_t;

typedef struct _rrc_mac_configure_status_report_required_ue_t
{
    U8  ue_psr_req_flag; /*^ M, 0, H, 0, 1 ^*/  /* rrc_bool_et */ 
} rrc_mac_configure_status_report_required_ue_t; 

#define RRC_MAC_UE_PSR_REQ_PRESENT 0x01
#define RRC_MAC_DC_BEARER_UL_TYPE_INFO_PRESENT 0x02
typedef struct _rrc_mac_ul_lc_create_req_t
{
    rrc_bitmask_t            optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    U8  lc_g_id;                        /*^ M, 0, H, 0, 3 ^*/
    rrc_mac_configure_status_report_required_ue_t     configure_status_report_required_ue;
    /*^ TLV, RRC_MAC_UE_PSR_REQ, RRC_MAC_UE_PSR_REQ_PRESENT ^*/
} rrc_mac_ul_lc_create_req_t;

typedef struct _rrc_mac_gbr_qos_info_t
{
    U32  e_rab_mbr_dl;  /*^ M, 0, N, 0, 0 ^*/
    U32  e_rab_mbr_ul;  /*^ M, 0, N, 0, 0 ^*/
    U32  e_rab_gbr_dl;  /*^ M, 0, N, 0, 0 ^*/
    U32  e_rab_gbr_ul;  /*^ M, 0, N, 0, 0 ^*/
}rrc_mac_gbr_qos_info_t;


#define RRC_MAC_GBR_QOS_INFO_PRESENT 0x04
typedef struct _rrc_mac_qos_info_t
{
    rrc_bitmask_t            optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                      bearer_type;    
/*^ M, 0, H, 0, 1 ^*/ /* rrc_bearer_type_et */
    
    U8                       qci;      /*^ M, 0, B, 1, 9 ^*/
    rrc_mac_gbr_qos_info_t   gbr_qos_info;
/*^ TLV, RRC_MAC_GBR_QOS_INFO, RRC_MAC_GBR_QOS_INFO_PRESENT ^*/
    
}rrc_mac_qos_info_t;

/* SPS Start */
typedef struct _rrc_mac_rohc_profile_t
{
    U16    rohc_profile;
    /*^ M, 0, H, 1, 6 ^*/
}rrc_mac_rohc_profile_t;
/* SPS Stop */

#define RRC_MAC_UL_LC_CREATE_REQ_PRESENT 0x01
#define RRC_MAC_DL_LC_CREATE_REQ_PRESENT 0x02
#define RRC_MAC_QOS_INFO_PRESENT         0x04
/* SPS Start */
#define RRC_MAC_ROHC_PROFILE_PRESENT     0x08
/* SPS Stop */

typedef struct _rrc_mac_create_lc_req_t
{
    rrc_bitmask_t               optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    rrc_lc_id_t                 lch_id;                 /*^ M, 0, H, 0, 10 ^*/
    U8                          rlc_mode;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_mac_rlc_mode_et */

    rrc_mac_ul_lc_create_req_t  ul_lc_create_req;
/*^ TLV, RRC_MAC_UL_LC_CREATE_REQ, RRC_MAC_UL_LC_CREATE_REQ_PRESENT ^*/

    rrc_mac_dl_lc_create_req_t  dl_lc_create_req;
/*^ TLV, RRC_MAC_DL_LC_CREATE_REQ, RRC_MAC_DL_LC_CREATE_REQ_PRESENT ^*/

    rrc_mac_qos_info_t          qos_info;
/*^ TLV, RRC_MAC_QOS_INFO, RRC_MAC_QOS_INFO_PRESENT ^*/

    /* SPS Start */
    rrc_mac_rohc_profile_t       rohc_profile;
/*^ TLV, RRC_MAC_ROHC_PROFILE, RRC_MAC_ROHC_PROFILE_PRESENT ^*/

} rrc_mac_create_lc_req_t;

/******************************************************************************
*   End of these types are shared UECC-LLIM with MAC
******************************************************************************/

/******************************************************************************
*   These types are shared UECC-LLIM with PDCP and OAM
******************************************************************************/

#define RRC_S1U_MAX_TUNNELS_PER_LC 3
/* Enum for PDCP RB direction */
typedef enum
{
    RRC_PDCP_RB_DIR_TX = 0,
    RRC_PDCP_RB_DIR_RX,
    RRC_PDCP_RB_DIR_BOTH
} rrc_rb_direction_et;

typedef enum
{
  UECC_FORWARDING_DIRECTION_DL,
  UECC_FORWARDING_DIRECTION_UL,
  UECC_FORWARDING_DIRECTION_BOTH,
  UECC_FORWARDING_DIRECTION_INVALID,
  UECC_FORWARDING_DIRECTION_ABSENT
}rrm_forwarding_direction_et;

typedef enum
{
  HO_SUCCESS,
  HO_FAILURE
}rrm_ho_status_et;


typedef enum
{
        UE_INACTIVITY_TIME_S1, 
        UE_INACTIVITY_TIME_S2, 
        UE_INACTIVITY_TIME_S3, 
        UE_INACTIVITY_TIME_S5, 
        UE_INACTIVITY_TIME_S7, 
        UE_INACTIVITY_TIME_S10, 
        UE_INACTIVITY_TIME_S15, 
        UE_INACTIVITY_TIME_S20,
        UE_INACTIVITY_TIME_S25, 
        UE_INACTIVITY_TIME_S30, 
        UE_INACTIVITY_TIME_S40, 
        UE_INACTIVITY_TIME_S50, 
        UE_INACTIVITY_TIME_MIN1, 
        UE_INACTIVITY_TIME_MIN1S20c, 
        UE_INACTIVITY_TIME_MIN1S40,
        UE_INACTIVITY_TIME_MIN2, 
        UE_INACTIVITY_TIME_MIN2S30, 
        UE_INACTIVITY_TIME_MIN3, 
        UE_INACTIVITY_TIME_MIN3S30, 
        UE_INACTIVITY_TIME_MIN4, 
        UE_INACTIVITY_TIME_MIN5, 
        UE_INACTIVITY_TIME_MIN6,
        UE_INACTIVITY_TIME_MIN7, 
        UE_INACTIVITY_TIME_MIN8, 
        UE_INACTIVITY_TIME_MIN9, 
        UE_INACTIVITY_TIME_MIN10,
        UE_INACTIVITY_TIME_MIN12, 
        UE_INACTIVITY_TIME_MIN14, 
        UE_INACTIVITY_TIME_MIN17, 
        UE_INACTIVITY_TIME_MIN20,
        UE_INACTIVITY_TIME_MIN24, 
        UE_INACTIVITY_TIME_MIN28, 
        UE_INACTIVITY_TIME_MIN33, 
        UE_INACTIVITY_TIME_MIN38, 
        UE_INACTIVITY_TIME_MIN44, 
        UE_INACTIVITY_TIME_MIN50, 
        UE_INACTIVITY_TIME_HR1,
        UE_INACTIVITY_TIME_HR1MIN30, 
        UE_INACTIVITY_TIME_HR2, 
        UE_INACTIVITY_TIME_HR2MIN30, 
        UE_INACTIVITY_TIME_HR3, 
        UE_INACTIVITY_TIME_HR3MIN30, 
        UE_INACTIVITY_TIME_HR4, 
        UE_INACTIVITY_TIME_HR5, 
        UE_INACTIVITY_TIME_HR6,
        UE_INACTIVITY_TIME_HR8, 
        UE_INACTIVITY_TIME_HR10, 
        UE_INACTIVITY_TIME_HR13, 
        UE_INACTIVITY_TIME_HR16, 
        UE_INACTIVITY_TIME_HR20, 
        UE_INACTIVITY_TIME_DAY1, 
        UE_INACTIVITY_TIME_DAY1HR12, 
        UE_INACTIVITY_TIME_DAY2,
        UE_INACTIVITY_TIME_DAY2HR12, 
        UE_INACTIVITY_TIME_DAY3, 
        UE_INACTIVITY_TIME_DAY4, 
        UE_INACTIVITY_TIME_DAY5, 
        UE_INACTIVITY_TIME_DAY7, 
        UE_INACTIVITY_TIME_DAY10, 
        UE_INACTIVITY_TIME_DAY14, 
        UE_INACTIVITY_TIME_DAY19,
        UE_INACTIVITY_TIME_DAY24, 
        UE_INACTIVITY_TIME_DAY30, 
        UE_INACTIVITY_TIME_DAYMORETHAN30
}rrc_ue_inactivity_time_et;

#define RRC_S1U_MAX_TUNNELS_PER_LC 3
/* Max size of ASN1 message */
#define UECC_MAX_ASN1_BUF_LEN                   4096
/*SPR 20553 FIX START */
#define UECC_MAX_ASN1_BUF_LEN_RRC_CONN_REL      8188 //On PDCP interface, we have used the max approv 8K size.So,using 8188 value here.
/*SPR 20553 FIX END */

#define UECC_LLIM_MAX_BUF_LEN                   1024 

/* Enum for PDCP integrity protection algorithm id */
typedef enum
{
    RRC_PDCP_INT_ALG_EIA0 = 0,
    RRC_PDCP_INT_ALG_EIA1 = 1,
    RRC_PDCP_INT_ALG_EIA2
} rrc_int_algorithm_et;

/* Enum for PDCP ciphering algorithm id */
typedef enum
{
    RRC_PDCP_CIPH_ALG_EEA0 = 0,
    RRC_PDCP_CIPH_ALG_EEA1,
    RRC_PDCP_CIPH_ALG_EEA2
} rrc_ciph_algorithm_et;

#define RRC_PDCP_SECURITY_KEY_SIZE      16

typedef struct _rrc_pdcp_config_int_t
{
    U16 algorithm_id;                       /* rrc_int_algorithm_et */
    U8  key[RRC_PDCP_SECURITY_KEY_SIZE];    /*^ M, 0, OCTET_STRING, FIXED ^*/
} rrc_pdcp_config_int_t;

typedef enum
{
    BREARER_TYPE_SRB,
    BREARER_TYPE_DRB,
    BREARER_TYPE_MAX
}bearer_type_et;

typedef struct _rrc_pdcp_config_srb_ciph_t
{
    U16 algorithm_id;                       /* rrc_ciph_algorithm_et */
    U8  key[RRC_PDCP_SECURITY_KEY_SIZE];    /*^ M, 0, OCTET_STRING, FIXED ^*/
} rrc_pdcp_config_srb_ciph_t;


typedef struct _rrc_pdcp_config_drb_ciph_t
{
    U16 algorithm_id;                       /* rrc_ciph_algorithm_et */
    U8  key[RRC_PDCP_SECURITY_KEY_SIZE];    /*^ M, 0, OCTET_STRING, FIXED ^*/
} rrc_pdcp_config_drb_ciph_t;


typedef struct _rrc_pdcp_config_rohc_t
{
    U16 profile_id;
    U16 max_cid;                            /* default value is 15 */
} rrc_pdcp_config_rohc_t;

#define K_RRC_ENC_ALG_P1 0x03
#define K_RRC_INT_ALG_P1 0x04
#define K_UP_ENC_ALG_P1  0x05

/******************************************************************************
*   End of these types are shared UECC-LLIM with PDCP
******************************************************************************/

/******************************************************************************
*   These types are shared UECC-LLIM with MAC
******************************************************************************/
typedef enum
{
        HANDOVER_TYPE_INTRA_LTE_S1,
        HANDOVER_TYPE_LTE_TO_UTRAN,
        HANDOVER_TYPE_LTE_TO_GERAN,
        HANDOVER_TYPE_UTRAN_TO_LTE,
        HANDOVER_TYPE_GERAN_TO_LTE,
        HANDOVER_TYPE_INTRA_LTE_X2,
        HANDOVER_TYPE_INTRA_CELL,
        HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT,
        HANDOVER_TYPE_LTE_TO_CDMA2000_HRPD,
        HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD,
        HANDOVER_TYPE_CCO,
        HANDOVER_TYPE_INTER_CELL,
	/* SPR 19078: CID 101192 Fix Start */
	HANDOVER_TYPE_INVALID
	/* SPR 19078: CID 101192 Fix End */
}rrc_handover_type_et;

typedef enum
{
    HANDOVER_INTRA_FREQ,
    HANDOVER_INTER_FREQ
} rrc_handover_freq_type_et;

typedef enum
{
    RELEASE = 0,
    SETUP
} rrc_request_type_et;

typedef enum
{
    MAC_RELEASE = 0,
    MAC_SETUP
} rrc_mac_request_type_et;

typedef struct _rrc_mac_ri_config_index_info_t
{
    U16  ri_config_index;               /*^ M, 0, H, 0, 1023 ^*/
} rrc_mac_ri_config_index_info_t;

/* eICIC changes start */
typedef struct _cqi_pmi_config_index2_r10_t
{
    U16         cqi_pmi_config_index2;      /*^ M, 0, H, 0, 1023 ^*/
}cqi_pmi_config_index2_r10_t;


typedef struct _rrc_mac_csi_config_index_v10_t
{

#define RRC_MAC_CQI_PMI_CONFIG_INDEX2_R10_PRESENT 0x01
#define RRC_MAC_RI_CONFIG_INDEX2_R10_PRESENT 0x02

    rrc_bitmask_t                       optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                                  request_type;     /*^ M, 0, H, 0, 1 ^*/

    cqi_pmi_config_index2_r10_t         cqi_pmi_config_index2_r10;
    /*^ TLV, RRC_MAC_CQI_PMI_CONFIG_INDEX2_R10, RRC_MAC_CQI_PMI_CONFIG_INDEX2_R10_PRESENT ^*/

    rrc_mac_ri_config_index_info_t      ri_config_index2_r_10;
    /*^ TLV, RRC_MAC_RI_CONFIG_INDEX_INFO, RRC_MAC_RI_CONFIG_INDEX2_R10_PRESENT ^*/

}rrc_mac_csi_config_index_v10_t;
/* eICIC changes stop */

typedef enum
{
    RRC_MAC_PERIODIC_WIDEBAND = 0,
    RRC_MAC_PERIODIC_SUBBAND
}rrc_cqi_fmt_indicator_periodic_t;

typedef struct _rrc_mac_cqi_format_indicator_perdiodic_subband_k_t
{
    U8   cqi_format_indicator_periodic_subband_k;
/*^ M, 0, H, 1, 4 ^*/
    
}rrc_mac_cqi_fmt_ind_perdiodic_subband_k_t;

#define RRC_MAC_RI_CONFIG_INDEX_INFO_PRESENT 0x01
#define RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K_PRESENT 0x02
/* eICIC changes start */
#define RRC_MAC_CSI_CONFIG_INDEX_V10_PRESENT                  0x04
/* eICIC changes stop */

typedef struct _rrc_mac_cqi_pmi_config_index_info_t
{
    rrc_bitmask_t           optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    U16  cqi_pmi_config_index;
/*^ M, 0, H, 0, 1023 ^*/

    U16  cqi_pucch_resource_index;
/*^ M, 0, H, 0, 1185 ^*/

    U8  simultaneous_ack_nack_and_cqi;
/*^ M, 0, H, 0, 1 ^*/

/**** TDD changes start ****/
    U8   cqi_fmt_ind_periodic;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_cqi_fmt_indicator_periodic_t */
    
    rrc_mac_cqi_fmt_ind_perdiodic_subband_k_t   cqi_fmt_ind_periodic_subband_k;
/*^ TLV, RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K, RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K_PRESENT ^*/

/**** TDD changes stop ****/
        
    rrc_mac_ri_config_index_info_t  ri_config_index_info;
/*^ TLV, RRC_MAC_RI_CONFIG_INDEX_INFO, RRC_MAC_RI_CONFIG_INDEX_INFO_PRESENT ^*/

} rrc_mac_cqi_pmi_config_index_info_t;


typedef struct _rrc_mac_cqi_pmi_config_index_info_v1020_t
{
    
    rrc_bitmask_t                                   optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    
    /* eICIC changes start */
    U16  cqi_pmi_config_index;
/*^ M, 0, H, 0, 1023 ^*/

    U16  cqi_pucch_resource_index;
/*^ M, 0, H, 0, 1185 ^*/

    U8  simultaneous_ack_nack_and_cqi;
/*^ M, 0, H, 0, 1 ^*/

/**** TDD changes start ****/
    U8   cqi_fmt_ind_periodic;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_cqi_fmt_indicator_periodic_t */
    
    rrc_mac_cqi_fmt_ind_perdiodic_subband_k_t   cqi_fmt_ind_periodic_subband_k;
/*^ TLV, RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K, RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K_PRESENT ^*/

/**** TDD changes stop ****/
        
    rrc_mac_ri_config_index_info_t  ri_config_index_info;
/*^ TLV, RRC_MAC_RI_CONFIG_INDEX_INFO, RRC_MAC_RI_CONFIG_INDEX_INFO_PRESENT ^*/


    rrc_mac_csi_config_index_v10_t                  csi_config_index_v10;
/*^ TLV, RRC_MAC_CSI_CONFIG_INDEX_V10, RRC_MAC_CSI_CONFIG_INDEX_V10_PRESENT ^*/
    /* eICIC changes stop */

}rrc_mac_cqi_pmi_config_index_info_v1020_t;


#define RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_PRESENT 0x01
#define RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_V1020_PRESENT 0x02

typedef struct _rrc_mac_cqi_periodic_config_info_t
{
    rrc_bitmask_t           optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    U8  request_type;                               /*^ M, 0, H, 0, 1 ^*/

    rrc_mac_cqi_pmi_config_index_info_t cqi_pmi_config_index_info;
/*^ TLV, RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO, RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_PRESENT ^*/

/* eICIC changes start */
    rrc_mac_cqi_pmi_config_index_info_v1020_t cqi_pmi_config_index_info_v1020;
/*^ TLV, RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_V1020, RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_V1020_PRESENT ^*/
/* eICIC changes stop */

} rrc_mac_cqi_periodic_config_info_t;

typedef struct _aperiodic_csi_trigger_r10_t
{
   U8   trigger1r10; /*^ M, 0, H, 0, 64 ^*/
   U8   trigger2r10; /*^ M, 0, H, 0, 192 ^*/
}aperiodic_csi_trigger_r10_t;

#define RRC_MAC_APERIODIC_CSI_TRIGGER_R10_PRESENT 0x01
typedef struct _rrc_mac_cqi_aperiodic_config_info_t
{
    rrc_bitmask_t                       optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8  cqi_aperiodic_mode;         /*^ M, 0, B, 12, 31 ^*/

    aperiodic_csi_trigger_r10_t     aperiodic_csi_trigger_r10;
/*^ TLV, RRC_MAC_APERIODIC_CSI_TRIGGER_R10, RRC_MAC_APERIODIC_CSI_TRIGGER_R10_PRESENT ^*/
} rrc_mac_cqi_aperiodic_config_info_t;


/* cr_316And247 IEs Mac Changes */

typedef struct _rrc_mac_cqi_mask_r9_t
{
    U8  cqi_mask_r9; /*^ M, 0, H, 0, 1 ^*/ /*cqi_mask_r9_et*/
}rrc_mac_cqi_mask_r9_t;

typedef struct _rrc_mac_pmi_ri_report_r9_t
{
    U8 pmi_ri_report_r9; /*^ M, 0, H, 0, 1 ^*/ /*pmi_ri_report_r9_mac_et*/
}rrc_mac_pmi_ri_report_r9_t;

#define RRC_MAC_CQI_REPORT_CONFIG_CQI_MASK_R9_PRESENT      0x01
#define RRC_MAC_CQI_REPORT_CONFIG_PMI_RI_REPORT_R9_PRESENT 0x02
typedef struct _rrc_mac_cqi_report_config_v920_t
{
    rrc_bitmask_t                       optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    rrc_mac_cqi_mask_r9_t           rrc_mac_cqi_mask_r9;
/*^ TLV, RRC_MAC_CQI_REPORT_CONFIG_CQI_MASK_R9, RRC_MAC_CQI_REPORT_CONFIG_CQI_MASK_R9_PRESENT ^*/

    rrc_mac_pmi_ri_report_r9_t       rrc_mac_pmi_ri_report_r9;
/*^ TLV, RRC_MAC_CQI_REPORT_CONFIG_PMI_RI_REPORT_R9, RRC_MAC_CQI_REPORT_CONFIG_PMI_RI_REPORT_R9_PRESENT ^*/

}rrc_mac_cqi_report_config_v920_t;


/* eICIC changes start */
/* SPR 14057 start */
typedef struct _rrc_mac_csi_meas_subframe_set1_r10_t
{
	U8                                choice;               /*^ M, 0, H, 0, 3 ^*/ /* rrc_mac_csi_meas_subframe_set_choice_et */
	U8                                count;                /*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/  
	U8                                csi_measurement_subset[CSI_MEAS_MEASUREMENT_SUBSET_SIZE]; /*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/ 
}rrc_mac_csi_meas_subframe_set1_r10_t;

typedef struct _rrc_mac_csi_meas_subframe_set2_r10_t
{
	U8                                choice;               /*^ M, 0, H, 0, 3 ^*/ /* rrc_mac_csi_meas_subframe_set_choice_et */
	U8                                count;                /*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/  
	U8                                csi_measurement_subset[CSI_MEAS_MEASUREMENT_SUBSET_SIZE]; /*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/ 
}rrc_mac_csi_meas_subframe_set2_r10_t;
/* SPR 14057 end */

typedef struct _rrc_mac_csi_subframe_pattern_config_v10_t
{
#define RRC_MAC_CSI_MEAS_SUBFRAME_SET1_R10_PRESENT  0x01
#define RRC_MAC_CSI_MEAS_SUBFRAME_SET2_R10_PRESENT  0x02

	rrc_bitmask_t                              optional_elems_present; 
	/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
	U8                                         request_type;     /*^ M, 0, H, 0, 1 ^*/   /* rrc_mac_request_type_et */
/* SPR 14057 start */
	rrc_mac_csi_meas_subframe_set1_r10_t        rrc_mac_csi_meas_subframe_set1_r10;
	/*^ TLV, RRC_MAC_CSI_MEAS_SUBFRAME_SET1_R10, RRC_MAC_CSI_MEAS_SUBFRAME_SET1_R10_PRESENT ^*/
    rrc_mac_csi_meas_subframe_set2_r10_t        rrc_mac_csi_meas_subframe_set2_r10;
    /*^ TLV, RRC_MAC_CSI_MEAS_SUBFRAME_SET2_R10, RRC_MAC_CSI_MEAS_SUBFRAME_SET2_R10_PRESENT ^*/
/* SPR 14057 end */
}rrc_mac_csi_subframe_pattern_config_v10_t;
/* eICIC changes stop */

#define RRC_MAC_CQI_APERIODIC_CONFIG_INFO_PRESENT 0x01
#define RRC_MAC_CQI_PERIODIC_CONFIG_INFO_PRESENT  0x02
#define RRC_MAC_CQI_REPORT_CONFIG_V920_PRESENT    0x04
/* eICIC changes start */
#define RRC_MAC_CQI_CSI_SUBFRAME_PATTERN_CONFIG_V10_PRESENT     0x08
/* eICIC changes stop */
typedef struct _rrc_mac_cqi_info_t
{
    rrc_bitmask_t                       optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    rrc_mac_cqi_aperiodic_config_info_t cqi_aperiodic_config_info;
/*^ TLV, RRC_MAC_CQI_APERIODIC_CONFIG_INFO, RRC_MAC_CQI_APERIODIC_CONFIG_INFO_PRESENT ^*/

    rrc_mac_cqi_periodic_config_info_t  cqi_periodic_config_info;
/*^ TLV, RRC_MAC_CQI_PERIODIC_CONFIG_INFO, RRC_MAC_CQI_PERIODIC_CONFIG_INFO_PRESENT ^*/

    /* Rel9 Upgrade Changes Start */
/* cr 316And247 IEs MAC Changes */
    rrc_mac_cqi_report_config_v920_t cqi_report_config_v920;
/*^ TLV, RRC_MAC_CQI_REPORT_CONFIG_V920, RRC_MAC_CQI_REPORT_CONFIG_V920_PRESENT ^*/ 
    /* Rel9 Upgrade Changes Stop */
	/* eICIC ue reconfig changes start */
	rrc_mac_csi_subframe_pattern_config_v10_t   rrc_mac_csi_subframe_pattern_config_v10;
	/*^ TLV, RRC_MAC_CQI_CSI_SUBFRAME_PATTERN_CONFIG_V10, RRC_MAC_CQI_CSI_SUBFRAME_PATTERN_CONFIG_V10_PRESENT ^*/
	/* eICIC ue reconfig changes stop */
} rrc_mac_cqi_info_t;


typedef struct _rrc_mac_sr_setup_info_t
{
    U16 sr_pucch_resource_index;    /*^ M, 0, H, 0, 2047 ^*/
    U8  sr_configuration_index;     /*^ M, 0, H, 0, 157 ^*/
} rrc_mac_sr_setup_info_t;

#define RRC_MAC_SR_SETUP_INFO_PRESENT 0x01

typedef struct _rrc_mac_sr_config_info_t
{
    rrc_bitmask_t           optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    U8  request_type;                               /*^ M, 0, H, 0, 1 ^*/

    rrc_mac_sr_setup_info_t sr_setup_info;
/*^ TLV, RRC_MAC_SR_SETUP_INFO, RRC_MAC_SR_SETUP_INFO_PRESENT ^*/

} rrc_mac_sr_config_info_t;

typedef struct _rrc_mac_pusch_config_info_t
{
    U8     beta_offset_ack_index; /*^ M, 0, H, 0, 15 ^*/
   
    U8     beta_offset_ri_index;  /*^ M, 0, H, 0, 15 ^*/

    U8     beta_offset_cqi_index; /*^ M, 0, H, 0, 15 ^*/

}rrc_mac_pusch_config_info_t;

/******************************************************************************
*   End of these types are shared UECC-LLIM with MAC
******************************************************************************/

/******************************************************************************
*   These types are shared CSC-UECC with RRM
******************************************************************************/
#define MAX_MNC_OCTET_SIZE                  3
#define MCC_OCTET_SIZE                      3
/* SPR 23209 FIXED START*/
/* Code Removed */
/* SPR 23209 FIXED START*/
#define CSG_ID_OCTET_SIZE                   4
#define TAC_OCTET_SIZE                      2
#define LAC_OCTET_SIZE                      2
#define CELL_ID_OCTET_SIZE                  4
/* SPR 15180 FIX START */
/* SPR 19596 Fix Start */
/* Code Deleted */
/* SPR 19596 Fix End */
/* SPR 15180 FIX STOP */

/* SPR 2330 Start */
typedef enum
{
    MACRO_ENB_ID = 1,        
    HOME_ENB_ID         
}rrc_enb_id_et;
/* SPR 2330 Stop */

typedef struct
{
/* SPR-18445 start */
  U8        count;                      /*^ M, 0, B, 2, MAX_MNC_OCTET_SIZE ^*/
/* SPR-18445 stop */
  U8        mnc[MAX_MNC_OCTET_SIZE];    /*^ M, 0, OCTET_STRING, VARIABLE, H, 0, 9 ^*/
}mnc_t;


#define PLMN_IDENTITY_MCC_PRESENCE_FLAG     0x01

typedef struct
{
  U16       presence_bitmask;       /*^ BITMASK ^*/
  U8        mcc[MCC_OCTET_SIZE];    /*^ O, 1, OCTET_STRING, FIXED, H, 0, 9 ^*/
  mnc_t     mnc;                    /*^ M, 0, N, 0, 0 ^*/
}plmn_identity_t;

typedef struct
{
    U8 plmn_id[MAX_PLMN_ID_BYTES]; /*^ M,0,OCTET_STRING,FIXED ^*/
}s1ap_plmn_identity_t;

typedef struct
{
  s1ap_plmn_identity_t plmn_identity;/*^ M, 0, N, 0, 0 ^*/

  U8    cell_identity[4];
/*^ M, 0, OCTET_STRING, FIXED ^*/

}s1ap_cgi_eutra_t;

typedef enum
{
  RRM_RRC_CELL_RESERVED,
  RRM_RRC_CELL_NOT_RESERVED
}rrm_cell_res_for_operator_use_et;


typedef struct
{
  plmn_identity_t   plmn_identity;              /*^ M, 0, N, 0, 0 ^*/
  U8                cell_res_for_operator_use;
/*^ M, 0, H, 1, 1 ^*/   /* rrm_cell_res_for_operator_use_et */

}plmn_identity_info_t;


/*SPR 23209 FIXED START*/
typedef struct
{
	/* SPR_17664_start*/
  U8            count;      /*^ M, 0, B, 1, 6 ^*/

	/* SPR_17664_end*/
  plmn_identity_info_t  plmn_identity_info[MAX_PLMN_IDS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}plmn_id_info_list_t;
/*SPR 23209 FIXED END*/


/******************************************************************************
*   End of these types are shared CSC-UECC with RRM
******************************************************************************/

typedef enum
{
     TARGET_RAT_TYPE_UTRA,
     TARGET_RAT_TYPE_GERAN,
     TARGET_RAT_TYPE_CDMA2000_1XRTT,
     TARGET_RAT_TYPE_CDMA2000_HRPD
}target_rat_type_et;

/* Values for rrc_response_et, rrc_response_t */
typedef enum
{
#ifndef OAM_IPR
    FAILURE,
    SUCCESS,
#endif
    PARTIAL_SUCCESS,
    L3_FATAL
}rrc_response_et;

/* Values for rrc_response_et, rrc_response_t */
typedef enum
{
    ERAB_FAILURE,    
    ERAB_SUCCESS,
    ERAB_PARTIAL_SUCCESS,
    ERAB_FATAL
}rrc_erab_response_et;


typedef enum
{
    RRC_NO_ERROR,
    RRC_TIMER_EXPIRY,
    RRC_INTERACTION_WITH_OTHER_PROCEDURE,
    RRC_RADIO_LINK_FAILURE_TRIGGERED,
    RRC_RADIO_LINK_FAILURE_TRIGGERED_ROLLBACK_FAILURE,
    RRC_HANDOVER_TRIGGERED,
    RRC_HANDOVER_ROLLBACK_TRIGGERED_FAILURE,
    RRC_REESTABLISHMENT_TRIGGERED,
    RRC_REESTABLISHMENT_TRIGGERED_ROLLBACK_FAILURE,
    RRC_MEMORY_ALLOCATION_FAILURE,
    RRC_INVALID_RESPONSE_RECEIVED,
    RRC_INTERNAL_ERROR,
    RRC_UNKNOWN_ERAB_ID,
    RRC_UECC_INVALID_QOS_COMBINATION,
    RRC_UECC_NON_GBR_RAB_NOT_PRESENT,
    RRC_UECC_ERAB_RELEASE_FAILED,
    RRC_INVALID_RRM_MESSAGE_RECV,
    RRC_LLIM_RECONFIG_FAILURE,
    RRC_RRM_MEAS_CONFIG_ALREADY_ONGOING,
    SCELL_ADDITION_FAILED ,
    SCELL_MODIFICATION_FAILED,
    SCELL_DELETION_FAILED, 
    SCELL_CONFIG_FAILED,
/*Error_Handling_changes*/
#ifdef ENDC_ENABLED
    RRC_X2AP_ERROR_IND,
#endif
/*Error_Handling_changes*/
    RRC_ERROR_CODE_LAST
}rrc_erab_error_codes_t;

#define S1AP_CELL_INFO_UPDATE_IND_CELL_ID_PRESENT      0x0001
#define S1AP_CELL_INFO_UPDATE_IND_TAC_PRESENT          0x0002
#define S1AP_CELL_INFO_UPDATE_IND_EMERGENCY_ID_PRESENT 0x0004
typedef struct _cell_area_info_t
{
    rrc_bitmask_t        bitmask;
    U8                   cell_id[CELL_ID_OCTET_SIZE];
    U8                   tac[TAC_OCTET_SIZE];
    U8                   emergency_id[EMERGENCY_AREA_SIZE];
}cell_area_info_t ;

typedef enum
{
    PAGE_SI,
    PAGE_ETWS,
    PAGE_CMAS,
    PAGE_SI_ETWS,
    PAGE_SI_CMAS,
    PAGE_ETWS_CMAS,
    PAGE_SI_ETWS_CMAS
}paging_type_et;


#define SIB_10_WARN_SEC_INFO_PRESENCE_FLAG      0x01

typedef struct
{
  U16       presence_bitmask;                          /*^ BITMASK ^*/
  U8        msg_id[MSG_ID_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/

  U8        serial_number[SERIAL_NUMBER_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/

  U8        warning_type[WARNING_TYPE_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/

  U8        warning_security_info[SECURITY_INFORMATION_OCTET_SIZE];
/*^ O, 1, OCTET_STRING, FIXED ^*/

}sib_type_10_Info_t;

#define SIB_11_DATA_COD_SCHEME_PRESENCE_FLAG      0x01

typedef struct
{
  U16       presence_bitmask;              /*^ BITMASK ^*/
  U8        msg_id[MSG_ID_OCTET_SIZE];     /*^ M, 0, OCTET_STRING, FIXED ^*/
  U8        serial_number[SERIAL_NUMBER_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/
/* SPR-18445 start */
  U16        warning_msg_size;      /*^ M, 0, H, 1, WARNING_MSG_OCTET_SIZE ^*/
/* SPR-18445 stop */
  U8        warning_msg[WARNING_MSG_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

  U8        data_coding_scheme[DATA_CODING_SCHEME_OCTET_SIZE];
/*^ O, SIB_11_DATA_COD_SCHEME_PRESENCE_FLAG, OCTET_STRING, FIXED ^*/

}sib_type_11_Info_t;

typedef struct _rrc_csc_oam_stat_paging_t
{
    U32 pag_discardedNbr;
    /*^ M, 0, N, 0, 0 ^*/
    U32 pag_succNbr; 
    /*^ M, 0, N, 0, 0 ^*/
/*CR 025 changes start*/    
    U32 pag_receivedNbr;
    /*^ M, 0, N, 0, 0 ^*/
/*CR 025 changes stop*/    
} rrc_csc_oam_stat_paging_t;
#define SIB_12_DATA_COD_SCHEME_PRESENCE_FLAG      0x01

typedef struct
{
  U16       presence_bitmask;              /*^ BITMASK ^*/
  U8        msg_id[MSG_ID_OCTET_SIZE];     /*^ M, 0, OCTET_STRING, FIXED ^*/
  U8        serial_number[SERIAL_NUMBER_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/
/* SPR-18445 start */
  U16        warning_msg_size;      /*^ M, 0, H, 1, WARNING_MSG_OCTET_SIZE ^*/
/* SPR-18445 stop */
  U8        warning_msg[WARNING_MSG_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

  U8        data_coding_scheme[DATA_CODING_SCHEME_OCTET_SIZE];
/*^ O, SIB_12_DATA_COD_SCHEME_PRESENCE_FLAG, OCTET_STRING, FIXED ^*/

}sib_type_12_Info_t;


typedef struct
{
  plmn_identity_t              plmn_identity; /*^ M, 0, N, 0, 0 ^*/
  U8                                 location_area_code[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_lai_info_t;

typedef struct
{
  U16                                bitmask; /*^ BITMASK ^*/
#define RRM_RNC_ROUTING_AREA_CODE_PRESENT               0X01
#define RRM_RNC_EXTENDED_RNC_ID_PRESENT                    0X02

  rrm_lai_info_t                 lai;  /*^ M, 0, N, 0, 0 ^*/
  U8                                 routing_area_code; 
  /*^ O, RRM_RNC_ROUTING_AREA_CODE_PRESENT, N, 0, 0 ^*/

  U16                               rnc_id; /*^ M, 0, H, 0, 4095 ^*/
  U16                               extended_rnc_id; 
  /*^ O, RRM_RNC_EXTENDED_RNC_ID_PRESENT, N, 4096, 65535 ^*/
}rrm_rnc_id_t;

typedef struct
{
	/*SPR_17664_start*/
        U32 num;    /*^ M, 0, H, 0, MAX_LAC_NUMOCTS ^*/
	/*SPR_17664_end*/
        U8  data[MAX_LAC_NUMOCTS];/*^ M, 0, OCTET_STRING, FIXED ^*/
}lac_t;

typedef struct
{
	/*SPR_17664_start*/
        U32 num;    /*^ M, 0, H, 0, MAX_PLMN_ID_NUMOCTS ^*/
	/*SPR_17664_end*/
        U8  data[MAX_PLMN_ID_NUMOCTS];/*^ M, 0, OCTET_STRING, FIXED ^*/
}plmn_identity_lai_t;

typedef struct
{
        plmn_identity_lai_t plmn_identity;  /*^ M, 0, N, 0, 0 ^*/
        lac_t           lac;                /*^ M, 0, N, 0, 0 ^*/
}registered_lai_t;


/* cr_220 timers added  */

/* Values for RLF Timer t301_r9 */
typedef enum
{
  RRC_RLF_T301_MS100,
  RRC_RLF_T301_MS200, 
  RRC_RLF_T301_MS300, 
  RRC_RLF_T301_MS400, 
  RRC_RLF_T301_MS600, 
  RRC_RLF_T301_MS1000, 
  RRC_RLF_T301_MS1500,
  RRC_RLF_T301_MS2000
}t301_r9_et;


/* Values for RLF Timer t310_r9 */
typedef enum
{
  RRC_RLF_T310_MS_0, 
  RRC_RLF_T310_MS_50, 
  RRC_RLF_T310_MS_100, 
  RRC_RLF_T310_MS_200, 
  RRC_RLF_T310_MS_500, 
  RRC_RLF_T310_MS_1000, 
  RRC_RLF_T310_MS_2000
}t310_r9_et;

/* Values for RLF Timer n310_r9 */
typedef enum
{
  RRC_RLF_N310_N1, 
  RRC_RLF_N310_N2, 
  RRC_RLF_N310_N3, 
  RRC_RLF_N310_N4, 
  RRC_RLF_N310_N6, 
  RRC_RLF_N310_N8, 
  RRC_RLF_N310_N10, 
  RRC_RLF_N310_N20
}n310_r9_et;

/* Values for RLF Timer t311_r9 */
typedef enum
{
  RRC_RLF_T311_MS_1000, 
  RRC_RLF_T311_MS_3000, 
  RRC_RLF_T311_MS_5000, 
  RRC_RLF_T311_MS_10000, 
  RRC_RLF_T311_MS_15000,
  RRC_RLF_T311_MS_20000, 
  RRC_RLF_T311_MS_30000
}t311_r9_et;

/* Values for RLF Timer n311_r9 */
typedef enum
{
  RRC_RLF_N311_N_1, 
  RRC_RLF_N311_N_2, 
  RRC_RLF_N311_N_3, 
  RRC_RLF_N311_N_4, 
  RRC_RLF_N311_N_5, 
  RRC_RLF_N311_N_6, 
  RRC_RLF_N311_N_8, 
  RRC_RLF_N311_N_10
}n311_r9_et;

typedef struct
{
  U8 t301_r9; /*^ M, 0, H, 0, 7 ^*/ /* t301_r9_et */

  U8 t310_r9; /*^ M, 0, H, 0, 6 ^*/ /* t310_r9_et */

  U8 n310_r9; /*^ M, 0, H, 0, 7 ^*/ /* n310_r9_et */               

  U8 t311_r9; /*^ M, 0, H, 0, 6 ^*/ /* t311_r9_et */             

  U8 n311_r9; /*^ M, 0, H, 0, 7 ^*/ /* n311_r9_et */             

}setup_t;

typedef struct
{

    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/

#define RLF_TIMERS_AND_CONSTANTS_SETUP_PRESENT 0x01

    setup_t setup;
/*^ O, RLF_TIMERS_AND_CONSTANTS_SETUP_PRESENT ^*/

}rlf_timers_and_constants_r9_t;


typedef S32 sctp_sd_t;
#define X2AP_SCTP_ASSOC_MAX_RETRANS_PRESENT    0x01
#define X2AP_SCTP_VAL_COOKIE_LIFE_PRESENT      0x02
/* dscp_marking_start */
#define X2AP_OAM_DSCP_VALUE_PRESENT            0x04
/* dscp_marking_stop */
typedef struct
{
    U32   hbinterval;  /*^ M,0,N,0,0 ^*/
    U16   pathmaxrxt; /*^ M,0,N,0,0 ^*/
    U16   init_num_ostreams; /*^ M,0,N,0,0 ^*/
    U16   init_max_instreams; /*^ M,0,N,0,0 ^*/
    U16   init_max_attempts; /*^ M,0,N,0,0 ^*/
    U16   init_max_init_timeo; /*^ M,0,N,0,0 ^*/
    U32   rto_initial; /*^ M,0,N,0,0 ^*/
    U32   rto_max; /*^ M,0,N,0,0 ^*/
    U32   rto_min; /*^ M,0,N,0,0 ^*/
    /*sctp parameters start*/
    U16   bitmask;/*^ BITMASK ^*/
    U16   assoc_max_retrans;
    /*^ O, X2AP_SCTP_ASSOC_MAX_RETRANS_PRESENT,N,0,0 ^*/
	U32	  valid_cookie_life;
    /*^ O, X2AP_SCTP_VAL_COOKIE_LIFE_PRESENT,N,0,0 ^*/
    /*sctp parameters stop*/
    /* dscp_marking_start */
    U8   x2ap_dscp_value;
    /*^ O,X2AP_OAM_DSCP_VALUE_PRESENT, H, 0, 63 ^*/
    /* dscp_marking_stop */
}x2ap_sctp_conf_info_t;
typedef struct
{
    U16     bitmask; /*^ BITMASK ^*/
#define X2AP_TIME_TO_WAIT_INDEX_PRESENT 0x01
#define X2AP_ENB_CFG_INFO_PRESENT 0x02
#define X2AP_RESET_CFG_INFO_PRESENT 0x04
#define X2AP_CELL_ACTIVATION_TIMER_INFO_PRESENT 0x08
#define X2AP_MOBILITY_CHANGE_INFO_PRESENT 0x10
#define X2AP_OAM_X2_HEALTH_MONITORING_TIMER_PRESENT 0x20    
    U32   sctp_connect_tim_val;  /*^ M,0,N,0,0 ^*/
    U32   x2ap_csi_guard_tim_val; /*^ M,0,N,0,0 ^*/
    U8    x2ap_time_to_wait_index; /*^ O,X2AP_TIME_TO_WAIT_INDEX_PRESENT,N,0,0 ^*/
    U32   x2ap_setup_retry_tim_val; /*^ M,0,N,0,0 ^*/
    U8    sctp_connect_retry_cnt; /*^ M,0,N,0,0 ^*/
    U8    x2ap_setup_retry_cnt; /*^ M,0,N,0,0 ^*/
    U8    eNB_Config_Update_retry_cnt; /*^ M,0,N,0,0 ^*/
    U32   eNB_Config_Update_retry_timer_val; /*^ O,X2AP_ENB_CFG_INFO_PRESENT,N,0,0 ^*/
    U8    eNB_Config_Update_time_to_wait_retry_cnt; /*^ O,X2AP_ENB_CFG_INFO_PRESENT,N,0,0 ^*/
    U32   eNB_Config_Update_time_to_wait_offset; /*^ O,X2AP_ENB_CFG_INFO_PRESENT,N,0,0 ^*/
    U8    x2ap_enb_reset_retry_cnt; /*^ O,X2AP_RESET_CFG_INFO_PRESENT,N,0,0 ^*/
    U32   x2ap_enb_reset_guard_tim_val; /*^ O,X2AP_RESET_CFG_INFO_PRESENT,N,0,0 ^*/
    U32   x2ap_rsu_enb_guard_tim_val;/*^ M,0,N,0,0 ^*/
    U32   x2ap_rsu_rrm_guard_tim_val;/*^ M,0,N,0,0 ^*/
    U32   x2ap_cell_activation_retry_cnt; /*^ O,X2AP_CELL_ACTIVATION_TIMER_INFO_PRESENT,N,0,0 ^*/
    U32   x2ap_cell_activation_guard_tim_val; /*^ O,X2AP_CELL_ACTIVATION_TIMER_INFO_PRESENT,N,0,0 ^*/
    U8    x2ap_mobility_setting_change_retry_cnt; /*^ O,X2AP_MOBILITY_CHANGE_INFO_PRESENT,N,0,0 ^*/
    U32   x2ap_mobility_setting_change_guard_tim_val; /*^ O,X2AP_MOBILITY_CHANGE_INFO_PRESENT,N,0,0 ^*/
    U16   x2ap_health_mon_time_int; /*^ O, X2AP_OAM_X2_HEALTH_MONITORING_TIMER_PRESENT, N, 0, 0  ^*/

}x2ap_con_tim_info_t;

typedef struct
{
    x2ap_sctp_conf_info_t  sctp_conf_param; /*^ M,0,N,0,0 ^*/
    x2ap_con_tim_info_t    timer_conf_info; /*^ M,0,N,0,0 ^*/
}x2ap_conf_param_t;

typedef struct
{
    U8 ip_addr[MAX_X2AP_IP_ADDRESS_LENGTH]; /*^ M,0,OCTET_STRING,FIXED ^*/
}x2_ip_addr_t;

/* IPV6 Start */
typedef struct
{
    U8 ipv6_addr[MAX_X2AP_IPV6_ADDRESS_LENGTH]; /*^ M,0,OCTET_STRING,FIXED ^*/
}x2_ipv6_addr_t;
/* IPV6 Stop */

typedef struct
{
    U8 eNB_id[MACRO_ENB_ID_OCTET_SIZE]; /*^ M,0,OCTET_STRING,FIXED ^*/
}macro_enb_id_t;


typedef struct
{
    U8 eNB_id[HOME_ENB_ID_OCTET_SIZE]; /*^ M,0,OCTET_STRING,FIXED ^*/
}home_enb_id_t;


typedef struct
{
    U8 presence_bitmask;    /*^ BITMASK ^*/

#define X2AP_OAM_MACRO_ENB_ID_PRESENT        0x01
#define X2AP_OAM_HOME_ENB_ID_PRESENT         0x02
    macro_enb_id_t  macro_enb_id; /*^ O,1,N,0,0 ^*/

    home_enb_id_t   home_enb_id; /*^ O,2,N,0,0 ^*/
}x2_enb_id_info_t;
typedef struct
{
    U8 plmn_id[MAX_PLMN_ID_BYTES]; /*^ M,0,OCTET_STRING,FIXED ^*/
}x2ap_plmn_identity_t;

typedef struct
{
        U8 plmn_id[MAX_PLMN_ID_BYTES]; /*^ M,0,OCTET_STRING,FIXED ^*/
}x2ap_nrplmn_identity_t;



typedef struct
{
    /*PLMN id */
    x2ap_plmn_identity_t plmn_identity; /*^ M,0,N,0,0 ^*/

    /* eNB id info */
    x2_enb_id_info_t     enb_id; /*^ M,0,N,0,0 ^*/
}x2_gb_enb_id_t;

typedef struct
{
    x2ap_plmn_identity_t  plmn_identity;  /*^ M,0,N,0,0 ^*/
    U8                     eutran_cell_id[HOME_ENB_ID_OCTET_SIZE]; /*^ M,0,OCTET_STRING,FIXED ^*/
}x2ap_ecgi_t;

typedef struct
{
    U8 tac[MAX_FIVE_GS_TAC_BYTES];  /*^ M,0,OCTET_STRING,FIXED ^*/
}x2ap_five_gs_tac_t;


typedef struct
{
    U8 tac[MAX_TAC_BYTES];  /*^ M,0,OCTET_STRING,FIXED ^*/
}x2ap_tac_t;

typedef struct
{
/* SPR-18445 start */
    U8     data_length; /*^ M, 0, H, 0, RRC_MAX_IP_ADDR_LEN_IPV6  ^*/
/* SPR-18445 stop */

    U8     data[RRC_MAX_IP_ADDR_LEN_IPV6];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
    
}x2_trans_layer_addrs_t;

#define X2_EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT    0x01
#define X2_EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT  0x02
#define X2_EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT      0x04
typedef struct
{
    U16                          bitmask;    /*^ BITMASK ^*/

    x2_trans_layer_addrs_t ipsec_trans_layer_addrs;
    /*^ O,X2_EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT,N,0,0 ^*/

    U8                           num_tlas; 
	/* SPR_17664_start*/
    /*^ O,X2_EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT,B,1,MAX_NUM_ENB_X2AP_GTP_TLAS ^*/

	/* SPR_17664_end*/
    x2_trans_layer_addrs_t   gtp_trans_layer_addrs[MAX_NUM_ENB_X2AP_GTP_TLAS];  /*^ O,X2_EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT,OCTET_STRING,VARIABLE ^*/

}x2_extend_trans_layer_addrs_t;

/* IPV6 Start */
#define X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT       0x01
#define X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT        0x02
#define X2_ENB_COMM_INFO_EXT_NUM_TLAS_PRESENT        0x04
#define X2_ENB_COMM_INFO_EXT_TLAS_PRESENT            0x08
/* SPR 20633 changes Start */
#define X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT       0x10
#define X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT        0x20
/* MENB CHANGES - START */
#define X2_ENB_COMM_INFO_DEFAULT_SGNB_PRESENT        0x40
/* MENB CHANGES - END */
typedef struct
{
    U8 bitmask;    /*^ BITMASK ^*/
/* SPR 20633 changes End */
    /* number of ip addr */
   /* SPR_17664_start */
    /* SPR 20633 changes Start */
    U8           num_ip_addr; /*^ O,X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT,B,1,MAX_NUM_IP_ADDR ^*/
    /* SPR 20633 changes End */
   /* SPR_17664_end */
    /* ip addresses of MMEs */
    /* SPR 20633 changes Start */
    x2_ip_addr_t ip_addr[MAX_NUM_IP_ADDR];  /*^ O,X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT,OCTET_STRING,VARIABLE ^*/
    /* SPR 20633 changes End */

    /* port of MME with which SCTP association needs to be initiated
     * */
    U16       port;  /*^ M,0,N,0,0 ^*/


    U8           num_ipv6_addr; 
   /* SPR_17664_start */
    /*^ O,X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT,B,1,MAX_NUM_IP_ADDR ^*/

   /* SPR_17664_end*/

    /* ip addresses of MMEs */
    x2_ipv6_addr_t ipv6_addr[MAX_NUM_IP_ADDR];  
    /*^ O,X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT,OCTET_STRING,VARIABLE ^*/


    U8           num_tlas; 
	/* SPR_17664_start*/
    /*^ O,X2_ENB_COMM_INFO_EXT_NUM_TLAS_PRESENT,B,1,MAX_NUM_ENB_X2AP_EXT_TLAS ^*/
	/* SPR_17664_end*/
    x2_extend_trans_layer_addrs_t extend_trans_layer_addrs[MAX_NUM_ENB_X2AP_EXT_TLAS];
    /*^ O,X2_ENB_COMM_INFO_EXT_TLAS_PRESENT,OCTET_STRING,VARIABLE ^*/

    /* MENB CHANGES - START */
    U8           default_sgnb;
    /*^ O, X2_ENB_COMM_INFO_DEFAULT_SGNB_PRESENT, H, 0, 1 ^*/
    /* MENB CHANGES - END */
}x2_enb_comm_info_t;
/*IPV6 Stop*/
typedef struct
{
    U32 earfcn; /*^ M,0,H,0,MAX_EARFCN ^*/
}x2ap_earfcn_t;

typedef enum
{
    BW6 = 0,
    BW15,
    BW25,
    BW50,
    BW100
}x2ap_trnsmission_bdwidth_en;

/* BUG_7947 START */
typedef enum
{
    SA0 = 0,
    SA1,
    SA2,
    SA3,
    SA4,
    SA5,
    SA6
}x2ap_subframe_assn_et;

typedef enum
{
    SSP0 = 0,
    SSP2,
    SSP3,
    SSP4,
    SSP5,
    SSP6,
    SSP7,
    SSP8
}x2ap_special_subframe_patterns_et;
/* BUG_7947 STOP */

typedef enum
{
    NORMALL = 0,
    EXTENDED
}x2ap_cyclic_prefix_en;

/*SPR_6329 Start*/
typedef enum
{
    AN1 = 0,
    AN2 = 1,
    AN4=  2
}x2ap_num_antenna_port_et;
/*SPR_6329 Stop*/

typedef struct
{
    /* BUG_7947 START */
    U8                                 special_subframe_patterns; /*^ M,0,H,0,8 ^*/ /* x2ap_special_subframe_patterns_et */
    /* BUG_7947 STOP */
    U8                                 dl_cp; /*^ M,0,H,0,1 ^*/
    U8                                 ul_cp; /*^ M,0,H,0,1 ^*/
}x2ap_special_subframe_info_t;

typedef struct
{
    x2ap_earfcn_t ul_earfcn; /*^ M,0,N,0,0 ^*/
    x2ap_earfcn_t dl_earfcn; /*^ M,0,N,0,0 ^*/
    U8                          ul_trnsmission_bdwidth; /*^ M,0,H,0,5 ^*/
    U8                          dl_trnsmission_bdwidth; /*^ M,0,H,0,5 ^*/
}x2ap_fdd_info_t;

typedef struct
{
    x2ap_earfcn_t                earfcn;    /*^ M,0,N,0,0 ^*/
    U8                            trnsmission_bdwidth; /*^ M,0,H,0,5 ^*/
    /* BUG_7947 START */
    U8                            subframe_assn; /*^ M,0,H,0,6 ^*/ /* x2ap_subframe_assn_et */
    /* BUG_7947 STOP */
    x2ap_special_subframe_info_t special_subframe_info;     /*^ M,0,N,0,0 ^*/
}x2ap_tdd_info_t;

typedef struct 
{
    U8                           bitmask;   /*^ BITMASK ^*/
#define    X2AP_EUTRA_MODE_FDD   0x01
#define    X2AP_EUTRA_MODE_TDD   0x02
    x2ap_fdd_info_t     fdd_info;  /*^ O,X2AP_EUTRA_MODE_FDD,N,0,0 ^*/
    x2ap_tdd_info_t     tdd_info;  /*^ O,X2AP_EUTRA_MODE_TDD,N,0,0 ^*/
}x2ap_choice_eutra_mode_t;

typedef struct
{
	/* SPR_17664_start*/
    U8                     num_bcast_plmn_id;             /*^ M,0,B,1,MAX_NUM_BPLMN ^*/
/* SPR_17664_end*/

    x2ap_plmn_identity_t  plmn_identity[MAX_NUM_BPLMN];  /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_broadcast_plmn_t;


typedef struct
{
    x2ap_choice_eutra_mode_t    choice_eutra_mode;  /*^ M,0,N,0,0 ^*/
}x2ap_choice_eutra_mode_info_t;


typedef struct
{
    U8                bitmask;
    x2ap_ecgi_t      ecgi;   /*^ M,0,N,0,0 ^*/
    U32               pci;    /*^ M,0,N,0,0 ^*/
    x2ap_earfcn_t    earfcn; /*^ M,0,N,0,0 ^*/
}x2ap_neighbour_info_t;

typedef struct
{
    x2ap_plmn_identity_t    plmn_id; /*^ M,0,N,0,0 ^*/
    U8                       mme_group_id[MAX_MME_GRP_BYTES]; /*^ M,0,OCTET_STRING,FIXED ^*/
}x2ap_gu_group_id_t;

typedef struct
{
	/* SPR_17664_start*/
    U16                         num_neighbour_cell; /*^ M,0,H,0,MAX_NEIGHBOURS ^*/

	/* SPR_17664_end*/
    x2ap_neighbour_info_t      neighbour_info_arr[MAX_NEIGHBOURS]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_nbour_cell_arry_t;
typedef struct
{
	/* SPR_17664_start*/
    U8                       num_gu_group_id;  /*^ M,0,H,0,MAX_ENB_POOLS^*/
  
	/* SPR_17664_end*/
  x2ap_gu_group_id_t      group_id_arr[MAX_ENB_POOLS]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_gu_grp_ids_t;

typedef struct
{
#define X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT 0x01
#define X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT 0x02
	U16 bitmask; /*^ BITMASK ^*/
	U8 oneframe[X2AP_SUBFRAME_ALLOC_ONE_FRAME_OCTET_SIZE]; /*^ O,X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT,OCTET_STRING,FIXED ^*/
	U8 fourframe[X2AP_SUBFRAME_ALLOC_FOUR_FRAME_OCTET_SIZE]; /*^ O,X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT,OCTET_STRING,FIXED ^*/
}x2ap_subframe_allocation_t;

typedef struct
{
 U8 radioframe_alloc_period; /*^ M,0,H,0,5 ^*/
 U8 radioframe_alloc_offset; /*^ M,0,H,0,7 ^*/
 x2ap_subframe_allocation_t subframe_alloc; /*^ M,0,N,0,0 ^*/
}x2ap_mbsfn_info_t;

typedef struct
{
	/* SPR_17664_start*/
 U8 num_mbsfn; /*^ M,0,H,0,MAX_NUM_MBSFN ^*/
 
	/* SPR_17664_end*/
x2ap_mbsfn_info_t mbsfn_info[MAX_NUM_MBSFN]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_mbsfn_subframe_info_t;

typedef struct
{
#define X2AP_PRACH_CONFIG_INDEX_PRESENT 0x01
 U16 bitmask; /*^ BITMASK ^*/
 U16 root_sequence_index; /*^ M,0,H,0,837 ^*/
 U8 zero_correlation_zone_config; /*^ M,0,H,0,15 ^*/
 U8 high_speed_flag; /*^ M,0,H,0,1 ^*/
 U8 prach_frequency_offset; /*^ M,0,H,0,94 ^*/
 U8 prach_config_index;	/*^ O,X2AP_PRACH_CONFIG_INDEX_PRESENT,H,0,63 ^*/
}x2ap_prach_configuration_info_t;

typedef struct
{
#define NUM_ANTENNA_PORT_BITMASK 0x01
#define X2AP_PRACH_CONFIG_INFO_PRESENT 0x02
#define X2AP_MBSFN_SUBFRAME_INFO_PRESENT 0x04
/*36423_CR0407_start*/
#define X2AP_CSG_ID_INFO_PRESENT 0x08
/*36423_CR0407_stop*/
    U8                                bitmask; /*^ BITMASK ^*/
    U32                               pci; /*^ M,0,H,0,503 ^*/
    rrc_cell_index_t                  cell_id; /*^ M, 0, H, 0,MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
    x2ap_tac_t                       track_area_code; /*^ M,0,N,0,0 ^*/
    x2ap_broadcast_plmn_t            broadcast_plmn_info; /*^ M,0,N,0,0 ^*/
    x2ap_choice_eutra_mode_info_t    choice_eutra_mode; /*^ M,0,N,0,0 ^*/
    U8 num_antenna_port;  /*^ O,NUM_ANTENNA_PORT_BITMASK,H,0,4 ^*/
    x2ap_prach_configuration_info_t prach_config; /*^ O,X2AP_PRACH_CONFIG_INFO_PRESENT,N,0,0 ^*/
    x2ap_mbsfn_subframe_info_t mbsfn_subframe_info; /*^ O,X2AP_MBSFN_SUBFRAME_INFO_PRESENT,N,0,0 ^*/
    /*36423_CR0407_start*/
    U8    csg_identity[CSG_ID_OCTET_SIZE];/*^ O,X2AP_CSG_ID_INFO_PRESENT,OCTET_STRING,FIXED ^*/
    /*36423_CR0407_stop*/
}x2apOam_srvd_cell_info_t;

typedef struct
{
    x2apOam_srvd_cell_info_t      served_cell_info;     /*^ M,0,N,0,0 ^*/
    x2ap_nbour_cell_arry_t     nbour_cell_info;      /*^ M,0,N,0,0 ^*/
}x2apOam_srvd_cell_arry_t;

typedef struct
{
    U8                                bitmask; /*^ BITMASK ^*/
    U32                               pci; /*^ M,0,N,0,0 ^*/
    x2ap_ecgi_t                      cell_id; /*^ M,0,N,0,0 ^*/
    x2ap_tac_t                       track_area_code; /*^ M,0,N,0,0 ^*/
    x2ap_broadcast_plmn_t            broadcast_plmn_info; /*^ M,0,N,0,0 ^*/
    x2ap_choice_eutra_mode_info_t    choice_eutra_mode; /*^ M,0,N,0,0 ^*/
    /*SPR_6329 Start*/
    /*SPR_8176 Start*/
    /*SPR 13950 Fix Start  */
    U8                              num_antenna_port;  /*^O,NUM_ANTENNA_PORT_BITMASK,H,0,2 ^*/
    /*SPR 13950 Fix Stop  */
    /*SPR_8176 Stop*/
    /*SPR_6329 Stop*/
    x2ap_prach_configuration_info_t prach_config; /*^ O,X2AP_PRACH_CONFIG_INFO_PRESENT,N,0,0 ^*/
    x2ap_mbsfn_subframe_info_t mbsfn_subframe_info; /*^ O,X2AP_MBSFN_SUBFRAME_INFO_PRESENT,N,0,0 ^*/
    /*36423_CR0407_start*/
    U8    csg_identity[CSG_ID_OCTET_SIZE];/*^ O,X2AP_CSG_ID_INFO_PRESENT,OCTET_STRING,FIXED ^*/
    /*36423_CR0407_stop*/
}x2ap_srvd_cell_info_t;

typedef struct
{
    x2ap_srvd_cell_info_t      served_cell_info;     /*^ M,0,N,0,0 ^*/
    x2ap_nbour_cell_arry_t     nbour_cell_info;      /*^ M,0,N,0,0 ^*/
}x2ap_srvd_cell_arry_t;

typedef struct
{
    plmn_identity_t plmn_identity;/*^ M, 0, N, 0, 0 ^*/
    U8    cell_identity[4]; /*^ M, 0, OCTET_STRING, FIXED ^*/

}rrm_cell_global_id_eutra_t;

typedef struct
{
        plmn_identity_t       plmn_identity; /*^ M, 0, N, 0, 0 ^*/
        U8                                 tracking_area_code[2];   /*^ M, 0, OCTET_STRING, FIXED ^*/  
}rrm_tai_info_t;

typedef struct
{
    rrc_bitmask_t   bitmask;   /*^ BITMASK ^*/
#define EUTRAN_CELL_GLOBAL_IDENTITY_PRESENT   0x01

    plmn_identity_t plmn_identity;
    U8    cell_identity[4];
    /*^ M, 0, OCTET_STRING, FIXED ^*/ /* BIT STRING (SIZE (28)) */
    U8    ecgi[4];
    /*^ O, EUTRAN_CELL_GLOBAL_IDENTITY_PRESENT, OCTET_STRING, FIXED ^*/ /* BIT STRING (SIZE (28)) */
    U8    cell_type;    /*^ M, 0, B, 1, 2 ^*/ /* rrc_enb_id_et */
} eutra_globalEnb_ecgi_t;

typedef struct
{
        eutra_globalEnb_ecgi_t   global_enb_id; /*^ M, 0, N, 0, 0 ^*/
        rrm_tai_info_t           tracking_area_id ;    /*^ M, 0, N, 0, 0 ^*/
}rrm_enb_id_t ;

typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/
#define RRM_CGI_INFO_ROUTING_AREA_CODE                  0X01

        plmn_identity_t               plmn_identity; /*^ M, 0, N, 0, 0 ^*/
        U8                                 location_area_code[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
        U8                                 ci_info[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
        U8                                 routing_area_code;
        /*^ O, RRM_CGI_INFO_ROUTING_AREA_CODE, N, 0, 0 ^*/ 
}rrm_cgi_info_t;

/*CDMA2000_CSFB Start*/

typedef enum
{
  CELL_ID_NOT_INCLUDED_000 = 0,
  CELL_ID_1XRTT_001,
  CELL_ID_1XRTT_010,
  CELL_ID_1XRTT_011,
  CELL_ID_HRPD_100,
  CELL_ID_HRPD_101,
  CELL_ID_HRPD_110,
  CELL_ID_1XRTT_111
}cell_id_info_et;

typedef enum
{
    DELTAF_1 = 0,
    DELTAF0,
    DELTAF1,
    DELTAF2,
    DELTAF3,
    DELTAF4,
    DELTAF5,
    DELTAF6
}rrc_rrm_pucch_format3_et;

typedef enum
{
    DELTA_F1 = 0,
    DELTA_F2
}rrc_rrm_pucch_format1_bcs_et;

typedef struct
{
#define CDMA2000_1XRTT_CELL_ID_LENGTH 6
    /*BASEID: 2 OCTETS: cell id + sector id
      SID: 15 bits (equiv to MarketId)
      NID: 2 OCTET: equivalent to SWNO (switch number)
     */
    U8 cdma2000_1xrtt_cell_id[CDMA2000_1XRTT_CELL_ID_LENGTH];
    /*^ M,0,OCTET_STRING,FIXED ^*/
}cdma2000_1xrtt_cell_identifier_t;  /*(Ref:3GPP2 X.S0004-550-E v4.0,section 2.161)*/


typedef struct
{
#define MAX_SECTOR_ID_LENGTH 16
	/* SPR_17664_start*/
    U8 sector_id_length; /*^ M,0,B,16,MAX_SECTOR_ID_LENGTH ^*/
	
	/* SPR_17664_end*/
    U8 sector_id[MAX_SECTOR_ID_LENGTH]; /*^ M,0,OCTET_STRING,FIXED ^*/
}cdma2000_hrpd_sector_identifier_t; /*Ref:3GPP2 A.S0008-C v2.0,section 5.2.1.6*/
/*CDMA2000_CSFB Stop*/

/* CSR 00070260 Changes Start */
typedef struct
{
#define CDMA2000_1XRTT_REFERENCE_CELL_ID_LENGTH 5

    U8 cdma2000_1xrtt_ref_cell_id[CDMA2000_1XRTT_REFERENCE_CELL_ID_LENGTH];
    /*^ M,0,OCTET_STRING,FIXED ^*/
}cdma2000_1xrtt_ref_cell_identifier_t;


typedef struct
{
#define CDMA2000_HRPD_REFERENCE_SECTOR_ID_LENGTH 16

    U8 cdma2000_hrpd_ref_sector_id[CDMA2000_HRPD_REFERENCE_SECTOR_ID_LENGTH]; 
    /*^ M,0,OCTET_STRING,FIXED ^*/
}cdma2000_hrpd_ref_sector_identifier_t;
/* CSR 00070260 Changes Stop */

typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/   
#define RRM_TARGET_ID_ENB_ID_PRESENT   0X01
#define RRM_TARGET_ID_RNC_ID_PRESENT   0X02
#define RRM_TARGET_ID_CGI_PRESENT         0X04
#define RRM_TARGET_ID_CDMA2000_1XRTT_CELL_ID_PRESENT         0X08
#define RRM_TARGET_ID_CDMA2000_HRPD_SECTOR_ID_PRESENT         0X10

        rrm_enb_id_t                      enb_id; 
/*^ O, RRM_TARGET_ID_ENB_ID_PRESENT, N, 0, 0 ^*/

        rrm_rnc_id_t                       rnc_id; 
/*^ O, RRM_TARGET_ID_RNC_ID_PRESENT, N, 0, 0 ^*/

        rrm_cgi_info_t                     cgi; /*^ O, RRM_TARGET_ID_CGI_PRESENT, N, 0, 0 ^*/

        cdma2000_1xrtt_cell_identifier_t         cell_id_1xrtt; 
/*^ O,RRM_TARGET_ID_CDMA2000_1XRTT_CELL_ID_PRESENT, N, 0, 0 ^*/

        cdma2000_hrpd_sector_identifier_t          sector_id_hrpd; 
/*^ O,RRM_TARGET_ID_CDMA2000_HRPD_SECTOR_ID_PRESENT, N, 0, 0 ^*/
}rrm_target_id_t;

/*CDMA2000_CSFB_HO start*/
#define SIB8_NEW_SEG_GENERATE_THRESHOLD 100
#define W_F_SIB8_NEW_SEG_GENERATE 10
#ifndef _MSC_VER
#define timespec_t struct timespec
typedef struct
{
  timespec_t timestamp;
  timespec_t cdma_time;
}sys_time_t;
#endif
typedef struct
{
    U16 num_of_elements;
    U16 sib8_sfn[MAX_SIB_INSTANCES];
}sib8_sfn_list_t;

/*CDMA2000_CSFB_HO stop*/

typedef struct
{
    U8 ip_addr[MAX_IP_ADDRESS_LENGTH]; /*^ M,0,OCTET_STRING,FIXED ^*/
}ip_addr_t;

/* IPV6 Start */
typedef struct
{
    U8 ipv6_addr[MAX_IPV6_ADDRESS_LENGTH];
    /*^ M,0,OCTET_STRING,FIXED ^*/
}ipv6_addr_t;

/* IPV6 Stop */

/*sctp parameters start*/

/* dscp_marking_start */
#define S1AP_OAM_DSCP_VALUE_PRESENT     0x01
/* dscp_marking_stop */
typedef struct
{
    U16  bitmask;/*^ BITMASK ^*/
    U32  init_max_attempts; /*^ M,0,N,0,0 ^*/
    U32  rto_initial; /*^ M,0,N,0,0 ^*/ 
    U32  rto_max; /*^ M,0,N,0,0 ^*/
    U32  rto_min; /*^ M,0,N,0,0 ^*/ 
    U16  assoc_max_retrans; /*^ M,0,N,0,0 ^*/ 
    U32	 valid_cookie_life; /*^ M,0,N,0,0 ^*/ 
    U16  path_max_retrans; /*^ M,0,N,0,0 ^*/
    /* dscp_marking_start */
    U8   s1ap_dscp_value;
    /*^ O,S1AP_OAM_DSCP_VALUE_PRESENT, H, 0, 63 ^*/
    /* dscp_marking_stop */
}s1ap_sctp_conf_info_t;    

/*sctp parameters stop*/

/* IPV6 Start */
#define MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT    0x01
#define MME_COMM_INFO_IPV6_ADDR_PRESENT        0x02
/* IPV6 Stop */
typedef struct
{
    /* number of ip addrs */
	/*SPR_17664_start */
    U8           num_ip_addr; /*^ M,0,H,0,MAX_NUM_IP_ADDR ^*/
	/*SPR_17664_end */

    /* ip addresses of MMEs */
    ip_addr_t  ip_addr[MAX_NUM_IP_ADDR]; /*^ M,0,OCTET_STRING,VARIABLE ^*/

    /* port of MME with which SCTP association needs to be initiated */
    U16       port;    /*^ M,0,N,0,0 ^*/

    /* num of streams to be setup between MME and enb */
    U16          num_streams; /*^ M,0,B,2,10 ^*/

    /* timer for SCTP connection recovery */
    U32          heart_beat_timer; /*^ M,0,N,0,0 ^*/

    /* release of MME (Rel8/Rel9/Rel10) */
    U8          rel_of_mme; /*^ M,0,H,0,2 ^*/ /* s1ap_oam_rel_of_mme_et */

    /* IPV6 Start */
    U16 bitmask; /*^ BITMASK ^*/
    /* IPV6 Stop */

    /* IPV6 Start */

    /* number of ipv6 addr */
    U8           num_ipv6_addr;
	/*SPR_17664_start */
    /*^ O,MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT,B,1,MAX_NUM_IP_ADDR ^*/
	/*SPR_17664_end */
    /* IPV6 addresses of MMEs */
    ipv6_addr_t ipv6_addr[MAX_NUM_IP_ADDR];
    /*^ O,MME_COMM_INFO_IPV6_ADDR_PRESENT,OCTET_STRING,VARIABLE ^*/
    /* IPV6 Stop */
}mme_comm_info_t;

typedef struct
{
#define SON_IP_ADDRESS_LENGTH 4
    U16 bitmask; /*^ BITMASK ^*/
    U8 ip_addr[SON_IP_ADDRESS_LENGTH]; /*^ M,0,OCTET_STRING,FIXED ^*/
    U16 port; /*^ M,0,N,0,0 ^*/
} son_comm_info_t;

typedef struct
{
    rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/
#define RRC_UE_INFO_MEAS_RESULT_RSRQ_PRESENT          0x01
    U8                    rsrp_result;
    /*^ M, 0, H, 0, 97 ^*/
    U8                    rsrq_result;
    /*^ O, RRC_UE_INFO_MEAS_RESULT_RSRQ_PRESENT, H, 0, 34 ^*/

}rrc_rlf_report_meas_result_serv_cell_t;

typedef struct
{
    plmn_identity_t plmn_identity;
    U8    cell_identity[4];
    /*^ M, 0, OCTET_STRING, FIXED ^*/ /* BIT STRING (SIZE (28)) */

} cell_global_id_eutra_t;

typedef struct
{
  U8    count;  /*^ M, 0, H, 0, 5 ^*/
  plmn_identity_t   cell_identity[5];   /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}plmn_identity_list2_t;

typedef struct
{
  rrc_bitmask_t   bitmask;                              /*^ BITMASK ^*/

#define MEAS_RESULT_EUTRA_CGI_INFO_PLMN_IDENTITY_LIST_PRESENT   0x01

  cell_global_id_eutra_t    cell_global_id;
  U8    tracking_area_code[2];
/*^ M, 0, OCTET_STRING, FIXED ^*/    /* BIT STRING (SIZE (16)) */

  plmn_identity_list2_t   plmn_identity_list;
/*^ O, MEAS_RESULT_EUTRA_CGI_INFO_PLMN_IDENTITY_LIST_PRESENT ^*/

}meas_result_eutra_cgi_info_t;

typedef struct
{
  rrc_bitmask_t bitmask;                                /*^ BITMASK ^*/

#define MEAS_RESULT_EUTRA_MEAS_RESULT_RSRP_PRESENT      0x01
#define MEAS_RESULT_EUTRA_MEAS_RESULT_RSRQ_PRESENT      0x02

  U8    rsrp_result;
/*^ O, MEAS_RESULT_EUTRA_MEAS_RESULT_RSRP_PRESENT, H, 0, 97 ^*/

  U8    rsrq_result;
/*^ O, MEAS_RESULT_EUTRA_MEAS_RESULT_RSRQ_PRESENT, H, 0, 34 ^*/

}meas_result_eutra_meas_result_t;

typedef struct
{
  rrc_bitmask_t bitmask;                                /*^ BITMASK ^*/

#define MEAS_RESULT_EUTRA_MEAS_RESULT_CSG_MEM_STATUS_PRESENT    0x01
#define MEAS_RESULT_EUTRA_MEAS_RESULT_CSG_ID_PRESENT            0x02

  U8    csg_membershipStatus;
/*^ O, MEAS_RESULT_EUTRA_MEAS_RESULT_CSG_MEM_STATUS_PRESENT, H, 0, 0 ^*/ /*rrc_csg_membership_status_et*/

  U8    csg_identity[CSG_ID_OCTET_SIZE]; /*^ O, MEAS_RESULT_EUTRA_MEAS_RESULT_CSG_ID_PRESENT, OCTET_STRING, FIXED ^*/

}meas_result_csg_info_t;

typedef struct
{
  rrc_bitmask_t   bitmask;                              /*^ BITMASK ^*/

#define MEAS_RESULT_EUTRA_CGI_INFO_PRESENT  0x01
#define MEAS_RESULT_EUTRA_CSG_INFO_PRESENT  0x02

  U16                               phys_cell_id;   /*^ M, 0, H, 0, 503 ^*/
  meas_result_eutra_cgi_info_t      cgi_info;
/*^ O, MEAS_RESULT_EUTRA_CGI_INFO_PRESENT ^*/

  meas_result_eutra_meas_result_t   meas_result;

  meas_result_csg_info_t      eutra_csg_info;
/*^ O, MEAS_RESULT_EUTRA_CSG_INFO_PRESENT ^*/
}meas_result_eutra_t;

typedef struct
{
  U8                    count;
	/* SPR_17664_start*/
/*^ M, 0, H, 0, MAX_CELL_REPORT ^*/

	/* SPR_17664_end*/

  meas_result_eutra_t   meas_result_eutra[MAX_CELL_REPORT];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}meas_result_list_eutra_t;

typedef struct
{
    rrc_bitmask_t                 bitmask;                /*^ BITMASK ^*/
    U16                           carrier_freq;
    /*0..65535*/
    meas_result_list_eutra_t      meas_result_list_eutra;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_meas_result2_eutra_t;

typedef struct
{
    U8                                count;     
	/* SPR_17664_start*/
    /*^ M, 0, B, 1, MAX_FREQ ^*/
	/* SPR_17664_end*/
    rrc_meas_result2_eutra_t          meas_result_eutra[MAX_FREQ];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_meas_result_list2_eutra_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/
#define MEAS_RESULT_UTRA_PHYS_CELL_ID_FDD_PRESENT   0x01
#define MEAS_RESULT_UTRA_PHYS_CELL_ID_TDD_PRESENT   0x02

  U16 fdd;  /*^ O, MEAS_RESULT_UTRA_PHYS_CELL_ID_FDD_PRESENT, H, 0, 511 ^*/
  U8 tdd;   /*^ O, MEAS_RESULT_UTRA_PHYS_CELL_ID_TDD_PRESENT, H, 0, 127 ^*/
}meas_result_utra_phys_cell_id_t;

typedef struct
{
  plmn_identity_t plmn_identity;
  U8    cell_identity[4]; /*^ M, 0, OCTET_STRING, FIXED ^*/   /* BIT STRING (SIZE (28)) */

}cell_global_id_utra_t;

typedef struct
{
  rrc_bitmask_t   bitmask;                              /*^ BITMASK ^*/

#define MEAS_RESULT_UTRA_CGI_INFO_PLMN_IDENTITY_LIST_PRESENT    0x01
#define MEAS_RESULT_UTRA_CGI_INFO_LOCATION_AREA_CODE_PRESENT    0x02
#define MEAS_RESULT_UTRA_CGI_INFO_ROUTING_AREA_CODE_PRESENT     0x04
  cell_global_id_utra_t    cell_global_id;
  U8    location_area_code[2];
/*^ O, MEAS_RESULT_UTRA_CGI_INFO_LOCATION_AREA_CODE_PRESENT, OCTET_STRING, FIXED ^*/
/* BIT STRING (SIZE (16)) */

  U8    routing_area_code;
/*^ O, MEAS_RESULT_UTRA_CGI_INFO_ROUTING_AREA_CODE_PRESENT ^*/
/* BIT STRING (SIZE (8)) */

  plmn_identity_list2_t   plmn_identity_list;
/*^ O, MEAS_RESULT_UTRA_CGI_INFO_PLMN_IDENTITY_LIST_PRESENT ^*/

}meas_result_utra_cgi_info_t;

typedef struct
{
  rrc_bitmask_t bitmask;                                /*^ BITMASK ^*/

#define MEAS_RESULT_UTRA_MEAS_RESULT_RSCP_PRESENT      0x01
#define MEAS_RESULT_UTRA_MEAS_RESULT_ECN0_PRESENT      0x02

  S8    utra_rscp;
/*^ O, MEAS_RESULT_UTRA_MEAS_RESULT_RSCP_PRESENT, B, -5, 91 ^*/

  U8    utra_ecn0;
/*^ O, MEAS_RESULT_UTRA_MEAS_RESULT_ECN0_PRESENT, H, 0, 49 ^*/

}meas_result_utra_meas_result_t;

typedef struct
{
  rrc_bitmask_t   bitmask;                              /*^ BITMASK ^*/

#define MEAS_RESULT_UTRA_CGI_INFO_PRESENT   0x01
#define MEAS_RESULT_UTRA_CSG_INFO_PRESENT   0x02
  meas_result_utra_phys_cell_id_t   phys_cell_id;
  meas_result_utra_cgi_info_t       cgi_info;
/*^ O, MEAS_RESULT_UTRA_CGI_INFO_PRESENT ^*/
  meas_result_csg_info_t            utra_csg_info;
/*^ O, MEAS_RESULT_UTRA_CSG_INFO_PRESENT ^*/
  meas_result_utra_meas_result_t    meas_result;
}meas_result_utra_t;

typedef struct
{
  U8                    count;
	/* SPR_17664_start*/
/*^ M, 0, H, 0, MAX_CELL_REPORT^*/
	/* SPR_17664_end*/
  meas_result_utra_t    meas_result_utra[MAX_CELL_REPORT];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}meas_result_list_utra_t;

typedef struct
{
    rrc_bitmask_t                bitmask;                /*^ BITMASK ^*/
    U16                          carrier_freq;
    /*^ M, 0, H, 0, 16383 ^*/
    meas_result_list_utra_t      meas_result_list_utra;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_meas_result2_utra_t;

typedef struct
{
    U8         count;    
	/* SPR_17664_start*/
    /*^ M, 0, B, 1, MAX_FREQ ^*/
	/* SPR_17664_end*/
    rrc_meas_result2_utra_t       meas_result_utra[MAX_FREQ];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_meas_result_list2_utra_t;

typedef struct
{
  U16   arfcn;  /*^ M, 0, H, 0, 1023 ^*/
  U8    band_indicator;  /*^ M, 0, H, 0, 1 ^*/ /* rrc_rrm_carrier_freq_geran_band_indicator_et*/
}carrier_freq_geran_t;

typedef struct
{
 U8     network_colour_code; /*BIT STRING (SIZE (3))*/
 U8     base_station_colour_code; /*BIT STRING (SIZE (3))*/
}phys_cell_id_geran_t;

typedef struct
{
  plmn_identity_t plmn_identity;
  U8    location_area_code[2];
/*^ M, 0, OCTET_STRING, FIXED ^*/  /* BIT STRING (SIZE (16)) */

  U8    cell_identity[2];
/*^ M, 0, OCTET_STRING, FIXED ^*/  /* BIT STRING (SIZE (16)) */

}cell_global_id_geran_t;

typedef struct
{
  rrc_bitmask_t   bitmask;                              /*^ BITMASK ^*/

#define MEAS_RESULT_GERAN_CGI_INFO_ROUTING_AREA_CODE_PRESENT    0x01

  cell_global_id_geran_t    cell_global_id;
  U8    routing_area_code;
/*^ O, MEAS_RESULT_GERAN_CGI_INFO_ROUTING_AREA_CODE_PRESENT ^*/
/* BIT STRING (SIZE (8)) */

}meas_result_geran_cgi_info_t;

typedef struct
{
  U8    rssi;   /*^ M, MEAS_RESULT_UTRA_MEAS_RESULT_ECN0_PRESENT, H, 0, 63 ^*/
}meas_result_geran_meas_result_t;

typedef struct
{
  rrc_bitmask_t   bitmask;                              /*^ BITMASK ^*/

#define MEAS_RESULT_GERAN_CGI_INFO_PRESENT  0x01

  carrier_freq_geran_t  carrier_freq;
  phys_cell_id_geran_t  phys_cell_id;
  meas_result_geran_cgi_info_t      cgi_info;
/*^ O, MEAS_RESULT_GERAN_CGI_INFO_PRESENT ^*/

  meas_result_geran_meas_result_t   meas_result;
}meas_result_geran_t;

typedef struct
{
  U8                    count;
/*^ M, 0, H, 0, 8 ^*/

  meas_result_geran_t   meas_result_geran[MAX_CELL_REPORT];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}meas_result_list_geran_t;

typedef struct
{
  U8   band_class;     /*^ M, 0, H, 0, 31 ^*/ /* rrm_band_class_cdma2000_et */
  U16   arfcn;          /*^ M, 0, H, 0, 2047 ^*/
}carrier_freq_cdma2000_t;

typedef struct
{
  rrc_bitmask_t bitmask;                                /*^ BITMASK ^*/

#define CELL_GLOBAL_ID_CDMA2000_1XRTT_PRESENT       0x01
#define CELL_GLOBAL_ID_CDMA2000_HRPD_PRESENT        0x02

  U8    cell_global_id_1xrtt[6];
/*^ O, CELL_GLOBAL_ID_CDMA2000_1XRTT_PRESENT, OCTET_STRING, FIXED ^*/
/*BIT STRING (SIZE (47))*/

  U8    cell_global_id_hrpd[16];
/*^ O, CELL_GLOBAL_ID_CDMA2000_HRPD_PRESENT, OCTET_STRING, FIXED ^*/
/*BIT STRING (SIZE (128))*/

}cell_global_id_cdma2000_t;

typedef struct
{
  rrc_bitmask_t bitmask;                                /*^ BITMASK ^*/

#define MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT     0x01

  U16           pilot_pn_phase;
/*^ O, MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT, H, 0, 32767 ^*/

  U8            pilot_strength;                         /*^ M, 0, H, 0, 63 ^*/
}meas_result_cdma2000_meas_result_t;

typedef struct
{
  rrc_bitmask_t   bitmask;                              /*^ BITMASK ^*/

#define MEAS_RESULT_CDMA2000_CGI_INFO_PRESENT   0x01

  U16                                   phy_cell_id;    /*^ M, 0, H, 0, 511 ^*/
  cell_global_id_cdma2000_t             cgi_info;
/*^ O, MEAS_RESULT_CDMA2000_CGI_INFO_PRESENT ^*/

  meas_result_cdma2000_meas_result_t    meas_result;
}meas_result_cdma2000_t;

typedef struct
{
  U8                        count;
	/* SPR_17664_start*/
/*^ M, 0, H, 0, MAX_CELL_REPORT ^*/
	/* SPR_17664_end*/
  meas_result_cdma2000_t    meas_result_cdma2000[MAX_CELL_REPORT];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}meas_result_list_cdma2000_t;

typedef struct
{
  U8                            pre_registration_status_hrpd;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */

  meas_result_list_cdma2000_t   meas_result_list_cdma2000;
}meas_results_cdma2000_t;

typedef struct
{ 
    rrc_bitmask_t                 bitmask;                /*^ BITMASK ^*/
    carrier_freq_cdma2000_t       carrier_freq_cdma2000;
    /*^ M, 0, N, 0, 0 ^*/
    meas_results_cdma2000_t       meas_result_cdma2000;
    /*^ M, 0, N, 0, 0 ^*/
} rrc_meas_result2_cdma2000_t;

typedef struct
{
    U8                                 count;    
	/* SPR_17664_start*/
    /*^ M, 0, B, 1, MAX_FREQ ^*/
	/* SPR_17664_end*/
    rrc_meas_result2_cdma2000_t        meas_result_cdma2000[MAX_FREQ];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} meas_result_list2_cdma2000_t;

/* MeasResultServFreq-r10 changes start */
typedef struct
{
  U8    rsrp_result_r10;    /*^ M, 0, H, 0, 97 ^*/
  U8    rsrq_result_r10;    /*^ M, 0, H, 0, 34 ^*/
}meas_result_attr_t;

typedef struct
{
  U16                               phys_cell_id_r10;         /*^ M, 0, H, 0, 503 ^*/
  meas_result_attr_t                best_neigh_cell_r10;          
}meas_result_best_neigh_cell_r10_t;

typedef struct 
{
    rrc_bitmask_t                   bitmask;                 /*^ BITMASK ^*/ 

#define RRM_MEAS_RESULT_SCELL_PRESENT                   0x01
#define RRM_MEAS_RESULT_BEST_NEIGH_CELL_PRESENT         0x02

    U8                              serv_cell_index_r10;    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/
    meas_result_attr_t              meas_result_scell_r10;
/*^ O, RRM_MEAS_RESULT_SCELL_PRESENT ^*/

    meas_result_best_neigh_cell_r10_t   meas_result_best_neigh_cell_r10;
/*^ O, RRM_MEAS_RESULT_BEST_NEIGH_CELL_PRESENT ^*/

}rrc_meas_result_serv_freq_r10_t;

typedef struct 
{
/* SPR-18445 start */
  U8                               count;
/*^ M, 0, H, 0, MAX_SERVING_CELL ^*/
/* SPR-18445 stop */

  rrc_meas_result_serv_freq_r10_t     rrc_meas_result_serv_freq_r10[MAX_SERVING_CELL];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}rrc_meas_result_serv_freq_list_r10_t;
/* MeasResultServFreq-r10 changes stop */

typedef struct
{
    rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/
#define RRC_UE_INFO_MEAS_RESULT_LIST_EUTRA_PRESENT          0x01
#define RRC_UE_INFO_MEAS_RESULT_LIST_UTRA_PRESENT           0x02
#define RRC_UE_INFO_MEAS_RESULT_LIST_GERAN_PRESENT          0x04
#define RRC_UE_INFO_MEAS_RESULT_LIST_CDMA2000_PRESENT       0x08
    rrc_meas_result_list2_eutra_t        meas_result_list_eutra;
    /*^ O, RRC_UE_INFO_MEAS_RESULT_LIST_EUTRA_PRESENT ^*/
    rrc_meas_result_list2_utra_t         meas_result_list_utra;
    /*^ O, RRC_UE_INFO_MEAS_RESULT_LIST_UTRA_PRESENT ^*/
    meas_result_list_geran_t             meas_result_list_geran;
    /*^ O, RRC_UE_INFO_MEAS_RESULT_LIST_GERAN_PRESENT ^*/
    meas_result_list2_cdma2000_t         meas_result_list2_cdma2000;
    /*^ O, RRC_UE_INFO_MEAS_RESULT_LIST_CDMA2000_PRESENT ^*/
}rrc_rlf_report_meas_result_neigh_cell_t;


typedef struct
{
    U8     data_length; /*^ M, 0, H, 0, 64  ^*/

    U8     data[64];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_rlf_dynamic_string_t;

typedef struct
{
    rrc_phys_cell_id_t  phys_cell_id;       /*^ M, 0, H, 0, 503 ^*/
    U16                 carrier_freq;
    /*0..65535*/
}rrc_rlf_failed_pci_arfcn_t;


typedef struct
{
    rrc_bitmask_t                                       bitmask; /*^ BITMASK ^*/
#define RRC_RLF_FAILED_PCI_CELL_GLOBAL_ID_PRESENT          0x01
#define RRC_RLF_FAILED_PCI_ARFCN_PRESENT                   0x02
    rrm_cell_global_id_eutra_t                cell_global_id;
    /*^ O, RRC_RLF_FAILED_PCI_CELL_GLOBAL_ID_PRESENT ^*/
    rrc_rlf_failed_pci_arfcn_t            pci_arfcn;
    /*^ O, RRC_RLF_FAILED_PCI_ARFCN_PRESENT ^*/
}rrc_rlf_failed_pcellid_t;

typedef struct
{
    rrc_bitmask_t                                       bitmask; /*^ BITMASK ^*/
#define RRC_RLF_LOCATION_CORD_ELLIPSOID_POINT_PRESENT                   0x01
#define RRC_RLF_LOCATION_CORD_ELLIPSOID_POINT_ALTITUDE_PRESENT          0x02
    rrc_rlf_dynamic_string_t ellipsoid_point;
    /*^ O, RRC_RLF_LOCATION_CORD_ELLIPSOID_POINT_PRESENT ^*/
    rrc_rlf_dynamic_string_t ellipsoid_point_altitude;
    /*^ O, RRC_RLF_LOCATION_CORD_ELLIPSOID_POINT_ALTITUDE_PRESENT ^*/
}rrc_rlf_report_location_coordinates_t;

typedef struct
{
    rrc_bitmask_t                                       bitmask; /*^ BITMASK ^*/
#define RRC_RLF_LOCATION_INFO_HORIZON_VELOCITY_PRESENT          0x01
#define RRC_RLF_LOCATION_INFO_GNSS_TOD_MSEC_PRESENT             0x02
    rrc_rlf_report_location_coordinates_t location_coordiate;
    rrc_rlf_dynamic_string_t              horizontal_velocity;
    /*^ O, RRC_RLF_LOCATION_INFO_HORIZON_VELOCITY_PRESENT ^*/
    rrc_rlf_dynamic_string_t              gnss_tod_msec;
    /*^ O, RRC_RLF_LOCATION_INFO_GNSS_TOD_MSEC_PRESENT ^*/
    
}rrc_rlf_report_location_info_t;


typedef struct
{
    rrc_bitmask_t                                       bitmask; /*^ BITMASK ^*/
#define RRC_UE_INFO_MEAS_RESULT_NEIGH_CELLS_PRESENT          0x01
#define RRC_UE_INFO_REESTABLISMENT_CELLID_R10_PRESENT        0x02
#define RRC_UE_INFO_TIME_CONN_FAILURE_R10_PRESENT            0x04
#define RRC_UE_INFO_CONN_FAILURE_TYPE_R10_PRESENT            0x08
#define RRC_UE_INFO_RLF_REPORT_LOACTION_INFO_PRESENT         0x10
#define RRC_UE_INFO_FAILED_PCELLID_PRESENT                   0x20
#define RRC_UE_INFO_REESTABLISHMENT_CELLID_PRESENT           0x40
#define RRC_UE_INFO_TIME_CONN_FALURE_PRESENT                 0x80
#define RRC_UE_INFO_CONN_FALURE_TYPE_PRESENT                 0x100
#define RRC_UE_INFO_PREV_PCELLID_PRESENT                     0x200
    rrc_rlf_report_meas_result_serv_cell_t              meas_result_serv_cell;
    /*^ M, 0, N, 0, 0 ^*/
    rrc_rlf_report_meas_result_neigh_cell_t             meas_result_neigh_cell;
    /*^ O, RRC_UE_INFO_MEAS_RESULT_NEIGH_CELLS_PRESENT ^*/
    rrm_cell_global_id_eutra_t                          reestablishment_cellid_r10;
    /*^ O, RRC_UE_INFO_REESTABLISMENT_CELLID_R10_PRESENT ^*/
    U16                                                 time_conn_failure_r10;
    /*^ O, RRC_UE_INFO_TIME_CONN_FAILURE_R10_PRESENT ^*/
    U32                                                 connection_failure_type_r10;
    /*^ O, RRC_UE_INFO_CONN_FAILURE_TYPE_R10_PRESENT ^*/
    rrc_rlf_report_location_info_t                      location_info;
    /*^ O, RRC_UE_INFO_RLF_REPORT_LOACTION_INFO_PRESENT ^*/
    rrc_rlf_failed_pcellid_t              failed_pcellid;
    /*^ O, RRC_UE_INFO_FAILED_PCELLID_PRESENT ^*/
    rrm_cell_global_id_eutra_t               reestablish_cell_id;
    /*^ O, RRC_UE_INFO_REESTABLISHMENT_CELLID_PRESENT ^*/
    U16                                   time_conn_failure;
    /*^ O, RRC_UE_INFO_TIME_CONN_FALURE_PRESENT ^*/
    U32                                   conn_failure_type;
    /*^ O, RRC_UE_INFO_CONN_FALURE_TYPE_PRESENT ^*/
    rrm_cell_global_id_eutra_t            previous_pcell_id;
    /*^ O, RRC_UE_INFO_PREV_PCELLID_PRESENT ^*/
}rrc_rlf_report_t;

#define MAX_EXT_MODULES 9
#define MAX_LEN_COMM_INFO   22
/*SPR 22554 Fix Start*/
#define MAX_THREAD_NAME_LEN 20
/*SPR 22554 Fix End*/

typedef struct
{
    U8 mmegi[MMEGI_OCTET_SIZE];   /*^ M, 0, OCTET_STRING, FIXED ^*/
}s1ap_served_group_id_t;

typedef struct
{
    U8    mmec;   /*^ M,0,N,0,0 ^*/
}s1ap_served_mmec_t;

typedef struct
{
	/* SPR_17664_start*/
    U8                      num_served_plmns;  /*^ M,0,B,1,MAX_NUM_SERVED_PLMN ^*/
	/* SPR_17664_end*/
    s1ap_plmn_identity_t    plmn_identity [MAX_NUM_SERVED_PLMN];/*^ M,0,OCTET_STRING,VARIABLE ^*/
    

	/* SPR_17664_start*/
    U16                     num_served_group_ids; /*^ M,0,B,1,MAX_NUM_SERVED_GROUP_ID ^*/
	/* SPR_17664_end*/
    s1ap_served_group_id_t  served_group_id [MAX_NUM_SERVED_GROUP_ID];/*^ M,0,OCTET_STRING,VARIABLE ^*/
    
	/* SPR_17664_start*/
    U16                      num_served_mmecs;/*^ M,0,B,1,MAX_NUM_SERVED_MMEC ^*/
	/* SPR_17664_end*/
    s1ap_served_mmec_t      served_mmec [MAX_NUM_SERVED_MMEC];/*^ M,0,OCTET_STRING,VARIABLE ^*/

}s1ap_served_gummei_info_t;

typedef struct
{
#define S1AP_MME_NAME_PRESENT                  0x01
#define S1AP_SERVED_GUMMEI_INFO_PRESENT        0x02
#define S1AP_RELATIVE_MME_CAPACITY_PRESENT     0x04
   rrc_bitmask_t               bitmask;   /*^ BITMASK ^*/
   
   U8                          mme_name[MAX_MME_NAME];
/*^ O, S1AP_MME_NAME_PRESENT, OCTET_STRING, FIXED ^*/   
   
   s1ap_served_gummei_info_t      served_gummei_info;
/*^ O, S1AP_SERVED_GUMMEI_INFO_PRESENT, N, 0, 0 ^*/   
   
   U8                          relative_mme_capacity; 
/*^ O, S1AP_RELATIVE_MME_CAPACITY_PRESENT, N, 0, 0 ^*/

}s1ap_mme_info_t;

#define RRC_ONE                  1
#define MAX_UE_XWAP_ID_BYTES     3
#define MAX_SSID_BYTES           32
#define MAX_BSSID_BYTES          6
#define MAX_HESSID_BYTES         6


/* Structure for storing UE XWAP Id value */
typedef struct
{
    /* Number of octets */
    U8   numocts;

    /* XWAP ID value */
    U8   xwapId[MAX_UE_XWAP_ID_BYTES];

}ue_xwap_id_t;

typedef struct 
{
  U8     numOctets;
/*^ M, 0, B, 1, MAX_SSID_BYTES ^*/

  U8     data[MAX_SSID_BYTES];
/*^ M, 0, OCTET_STRING, FIXED ^*/

}wlan_identifier_ssid_t;

typedef struct
{
  rrc_bitmask_t    bitmask;        /*^ BITMASK ^*/

#define WLAN_ID_SSID_PRESENT          0x01
#define WLAN_ID_BSSID_PRESENT         0x02
#define WLAN_ID_HESSID_PRESENT        0x04

  wlan_identifier_ssid_t    ssid;
/*^ O, WLAN_ID_SSID_PRESENT ^*/

  U8                        bssid[MAX_BSSID_BYTES];
/*^ O, WLAN_ID_BSSID_PRESENT, OCTET_STRING, FIXED ^*/

  U8                        hessid[MAX_HESSID_BYTES];
/*^ O, WLAN_ID_HESSID_PRESENT, OCTET_STRING, FIXED ^*/

}wlan_identifier_t;

#define MAX_WLAN_ID      2 

/* COV_113081_Fix_Start */
#define SHORT_WT_ID_LEN  4
#define LONG_WT_ID_LEN   6
/* COV_113081_Fix_End */

/* Values for WT identifier type */
typedef enum
{
    WT_ID_TYPE_1,
    WT_ID_TYPE_2
}wt_identifier_type_et;

#define WT_IDENTIFIER_SHORT_WT_ID_PRESENT  0x01
#define WT_IDENTIFIER_LONG_WT_ID_PRESENT   0x02


/* WT Identifier */
typedef struct _wt_identifier_t
{
    rrc_bitmask_t      bitmask;    /*^ BITMASK ^*/

    U8                 plmn_id[MAX_PLMN_ID_BYTES];
    /*^ O, WT_IDENTIFIER_SHORT_WT_ID_PRESENT, OCTET_STRING, FIXED ^*/

    U8                 short_wt_id[SHORT_WT_ID_LEN];
    /*^ O, WT_IDENTIFIER_SHORT_WT_ID_PRESENT, OCTET_STRING, FIXED ^*/

    U8                 long_wt_id[LONG_WT_ID_LEN];
    /*^ O, WT_IDENTIFIER_LONG_WT_ID_PRESENT, OCTET_STRING, FIXED ^*/

}wt_identifier_t;

/* WT supported WLAN identifier list */
typedef struct _wlan_identifier_list_t
{
    U16                count;
    /*^ M, 0, H, 0, MAX_WLAN_ID ^*/

    wlan_identifier_t  supported_wlan_id[MAX_WLAN_ID];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

}wlan_identifier_list_t;

/* Structure for storing connected WT data */
typedef struct _xwap_wt_info_t
{
    wt_identifier_t           wt_id;
    /*^ M, 0, N, 0, 0 ^*/

    wlan_identifier_list_t    wlan_identifier_list;
    /*^ M, 0, N, 0, 0 ^*/
/* COV_113071_Fix_Start */
    U8                        link_status;
    /*^ M, 0, H, 0, 1 ^*/
/* COV_113071_Fix_End */

}xwap_wt_info_t;

/* Enum for values of different types of LWA bearer */
typedef enum
{
    LWA_SWITCHED_BEARER,
    LWA_SPLIT_BEARER
}lwa_bearer_type_et;

typedef enum
{
   RRM_ERAB_RECONFIG_FAIL,
   RRM_ERAB_RECONFIG_FAIL_WT_ADD,
   RRM_ERAB_RECONFIG_FAIL_WT_ASSOC,
   RRM_ERAB_RECONFIG_FAIL_RRC_RECONFIG,
   RRM_ERAB_RECONFIG_FAIL_UE_REL,
   RRM_ERAB_RECONFIG_FAIL_ENB_UE_REL,
   RRM_ERAB_RECONFIG_FAIL_CELL_DEL,
   RRM_ERAB_RECONFIG_FAIL_RLF,
   RRM_ERAB_RECONFIG_FAIL_HO,
   RRM_ERAB_RECONFIG_FAIL_S1AP_RESET,
   RRM_ERAB_RECONFIG_FAIL_RE_ESTABLISH,
   RRM_ERAB_RECONFIG_FAIL_MAX
}rrm_erab_reconfig_fail_cause_et;


typedef struct _drb_countMSB_info_t
{
    U16           bitmask; /*^ BITMASK ^*/
    U8            drb_Identity; /*^ M, 0, B, 1, 32 ^*/
    U32           countMSB_Uplink; /*^ M, 0, H, 0, 33554431 ^*/
    U32           countMSB_Downlink; /*^ M, 0, H, 0, 33554431 ^*/
}drb_countMSB_info_t;

typedef struct _drb_countMSB_info_list_t
{
	/* SPR_17664_start*/
        U8                         drb_count;    /*^ M, 0, B, 1, MAX_LC_COUNT ^*/
	/* SPR_17664_end*/
	drb_countMSB_info_t        drb_countMSB_info[MAX_LC_COUNT];  /*^ M,0,OCTET_STRING,VARIABLE ^*/  
}drb_countMSB_info_list_t;

typedef struct _drb_count_info_t
{
    U16           bitmask; /*^ BITMASK ^*/
    U8            drb_Identity; /*^ M, 0, B, 1, 32 ^*/
    U32           count_Uplink; /*^ M, 0, N, 0, 0 ^*/
    U32           count_Downlink; /*^ M, 0, N, 0, 0 ^*/
}drb_count_info_t;

typedef struct _drb_count_info_list_t
{
	/* SPR_17664_start*/
        U8                         drb_count;    /*^ M, 0, H, 0, MAX_LC_COUNT ^*/
	/* SPR_17664_end*/
drb_count_info_t           drb_count_info[MAX_LC_COUNT];  /*^ M,0,OCTET_STRING,VARIABLE ^*/  
}drb_count_info_list_t;

typedef struct _drb_list_t
{
	/* SPR_17664_start*/
    U8                         drb_count;    /*^ M, 0, B, 1, MAX_LC_COUNT ^*/
	/* SPR_17664_end*/
	U8                         drb_Identity[MAX_LC_COUNT];
    /*^ M, 0, OCTET_STRING, VARIABLE, B, 1, 32^*/
}drb_list_t;

typedef struct _countMSB_uplink_t
{
    U32           countMSB_Uplink; /*^ M, 0, H, 0, 33554431 ^*/
}countMSB_uplink_t;

typedef struct _countMSB_downlink_t
{
    U32           countMSB_Downlink; /*^ M, 0, H, 0, 33554431 ^*/
}countMSB_downlink_t;

#define RRC_PDCP_COUNTMSB_UPLINK_PRESENT     0x01
#define RRC_PDCP_COUNTMSB_DOWNLINK_PRESENT   0x02
typedef struct _rrc_pdcp_drb_countMSB_info_t
{
    rrc_bitmask_t               optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
   rrc_lc_id_t             lc_id;
   countMSB_uplink_t       countMSB_Uplink; 
/*^ TLV, RRC_PDCP_COUNTMSB_UPLINK_TAG, RRC_PDCP_COUNTMSB_UPLINK_PRESENT ^*/
   countMSB_downlink_t     countMSB_Downlink; 
/*^ TLV, RRC_PDCP_COUNTMSB_DOWNLINK_TAG, RRC_PDCP_COUNTMSB_DOWNLINK_PRESENT ^*/
}rrc_pdcp_drb_countMSB_info_t;

typedef struct _rrc_pdcp_drb_list_t
{
    rrc_lc_id_t                     lc_id;
    rrc_rb_direction_t              rb_direction;
/* rrc_rb_direction_et */

}rrc_pdcp_drb_list_t;

/*sctp parameters start*/
typedef struct
{
    U32 tv_sec;  /*^ M,0,N,0,0 ^*/
    U32 tv_usec; /*^ M,0,N,0,0 ^*/
}rrc_timeval_t;
/*sctp parameters stop*/

typedef struct
{
    rrc_phys_cell_id_t  phy_cell_id;
    /*^ M, 0, H, 0, 503 ^*/

    U32   dl_carrier_freq;             
    /*^ M, 0, N, 0, 0 ^*/

} rrc_scell_Identification_t;

typedef struct
{
    rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_SCELL_CQI_REPORT_CONFIG_APERIODIC_PRESENT          0x01
#define RRM_SCELL_CQI_REPORT_CONFIG_PERIODIC_PRESENT           0x02
#define RRM_SCELL_CQI_REPORT_CONFIG_PMI_RI_REPORT_PRESENT      0x04

    U8  cqi_reporting_mode_aperiodic;
/*^ O, RRM_SCELL_CQI_REPORT_CONFIG_APERIODIC_PRESENT, H, 0, 4 ^*/ /* cqi_reporting_mode_aperiodic_et */

    S8  nomPDSCH_RS_EPRE_Offset;
/*^ M, 0, B, -1, 6 ^*/ 

    rrc_phy_cqi_report_periodic_r10_t    cqi_report_periodic_r10;
/*^ O, RRM_SCELL_CQI_REPORT_CONFIG_PERIODIC_PRESENT, N, 0, 0 ^*/

    U8    cqi_report_pmi_ri_report_r10;
/*^ O, RRM_SCELL_CQI_REPORT_CONFIG_PMI_RI_REPORT_PRESENT, H, 0, 0 ^*/

} rrc_cqi_report_config_scell_r10_t;

typedef struct
{
    U16                                bitmask; /*^ BITMASK ^*/
#define CANDIDATE_CELL_INFO_RSRP_PRESENT 0x01
#define CANDIDATE_CELL_INFO_RSRQ_PRESENT 0x02

    rrc_phys_cell_id_t  phy_cell_id; /*^ M, 0, H, 0, 503 ^*/
    U16   carrier_freq; /*^ M, 0, N, 0, 0 ^*/
    U8    rsrp_result;    /*^ O, CANDIDATE_CELL_INFO_RSRP_PRESENT, H, 0, 97 ^*/
    U8    rsrq_result;    /*^ O, CANDIDATE_CELL_INFO_RSRQ_PRESENT, H, 0, 34 ^*/
}candidate_cell_info_t;

typedef struct
{
/* SPR-18445 start */
        U8 count; /*^ M, 0, B, 1, MAX_FREQ ^*/
/* SPR-18445 stop */
        candidate_cell_info_t candidate_cell_info[MAX_FREQ]; 
        /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} candidate_cell_info_list_t;

typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/
#define RRM_CONFIG_EXT_CANDIDATE_CELL_INFO_LIST_PRESENT       0x01

        candidate_cell_info_list_t  candidate_cell_info_list; 
        /*^ O, RRM_CONFIG_EXT_CANDIDATE_CELL_INFO_LIST_PRESENT, N, 0, 0 ^*/

}rrm_config_t;
typedef struct
{
    rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_SCELL_DEDICATED_UL_CQI_CONFIG_SCELL_PRESENT          0x01

    rrc_cqi_report_config_scell_r10_t         cqi_report_config_scell;
    /*^ O, RRM_SCELL_DEDICATED_UL_CQI_CONFIG_SCELL_PRESENT ^*/

}rrc_radio_res_dedicated_scell_ul_config_t;

typedef struct
{
    rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_SCELL_DEDICATED_NON_UL_ANTENNA_INFO_PRESENT                           0x01
#define RRM_SCELL_DEDICATED_NON_UL_PDSCH_CONFIG_DEDICATED_PRESENT                 0x02

    rrc_antenna_info_dedicated_r10_t         antenna_info_dedicated_r10;
    /*^ O, RRM_SCELL_DEDICATED_NON_UL_ANTENNA_INFO_PRESENT ^*/

    rrc_phy_pdsch_configuration_dedicated_t     pdsch_configuration_dedicated;
/*^ O, RRM_SCELL_DEDICATED_NON_UL_PDSCH_CONFIG_DEDICATED_PRESENT ^*/

}rrc_radio_res_dedicated_scell_non_ul_config_t;

typedef struct
{
  rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_RADIO_RES_CONFIG_DEDICATED_SCELL_NON_UL_CONFIG_PRESENT           0x01
#define RRM_RADIO_RES_CONFIG_DEDICATED_SCELL_UL_CONFIG_PRESENT               0x02
  /*  LAA Rel13 Code + */
#define RRM_LAA_SCELL_CONFIGURATION_R13_PRESENT                              0x04
  /*  LAA Rel13 Code - */

  rrc_radio_res_dedicated_scell_non_ul_config_t  radio_res_dedicated_scell_non_ul_config;             
  /*^ O, RRM_RADIO_RES_CONFIG_DEDICATED_SCELL_NON_UL_CONFIG_PRESENT, N, 0, 0 ^*/

  rrc_radio_res_dedicated_scell_ul_config_t  radio_res_dedicated_scell_ul_config;             
  /*^ O, RRM_RADIO_RES_CONFIG_DEDICATED_SCELL_UL_CONFIG_PRESENT, N, 0, 0 ^*/

 /*  LAA Rel13 Code + */
  rrc_radio_res_dedicated_laa_scell_config_t  laa_scell_configuration_r13;
  /*^ O, RRM_LAA_SCELL_CONFIGURATION_R13_PRESENT, N, 0, 0 ^*/
 /*  LAA Rel13 Code - */
}rrc_physical_config_dedicated_scell_t;

typedef struct
{
  rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_PHYSICAL_CONFIG_DEDICATED_SCELL_CONFIG_PRESENT      0x01

  rrc_physical_config_dedicated_scell_t     physical_config_dedicated_scell;
  /*^ O , RRM_PHYSICAL_CONFIG_DEDICATED_SCELL_CONFIG_PRESENT, N, 0, 0 ^*/
}rrc_radio_res_config_dedicated_scell_t;

typedef struct _rrc_mac_ue_category_v1020_t
{
    U8   ue_category_v1020; /*^ M, 0, B, 6, 8 ^*/
}rrc_mac_ue_category_v1020_t;

typedef struct _rrc_mac_n1pucch_an_cs_r10_t
{
    U16   rrc_mac_n1pucch_an_cs; /*^ M, 0, H, 0, 2047 ^*/
}rrc_mac_n1pucch_an_cs_r10_t;

typedef struct _rrc_mac_n1pucch_an_cs_list_r10_t
{
    rrc_counter_t           count;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_n1pucch_an_cs_r10_t    rrc_mac_n1pucch_an_cs_r10[MAX_N1PUCCH_AN_CS];
/*^ TLV, SEQUENCE, RRC_MAC_PUCCH_AN_CS_V1020 ^*/
}rrc_mac_n1pucch_an_cs_list_r10_t;

typedef struct _rrc_mac_pucch_channel_selection_config_v1020_t
{
    rrc_bitmask_t           optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    U8  request_type;                               /*^ M, 0, H, 0, 1 ^*/

    rrc_counter_t           count;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/
    rrc_mac_n1pucch_an_cs_list_r10_t                    rrc_mac_n1pucch_an_cs_list_r10[MAX_N1PUCCH_AN_CS_LIST];
/*^ TLV, SEQUENCE, RRC_MAC_N1PUCCH_AN_CS_LIST_R10 ^*/

}rrc_mac_pucch_channel_selection_config_v1020_t;

#define RRC_MAC_PUCCH_CHANNEL_SELECTION_CONFIG_V1020_PRESENT 0x01
typedef struct _rrc_mac_pucch_format_r10_t
{
    rrc_bitmask_t           optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_pucch_channel_selection_config_v1020_t     rrc_mac_pucch_channel_selection_config_v1020;
/*^ TLV, RRC_MAC_PUCCH_CHANNEL_SELECTION_CONFIG_V1020, RRC_MAC_PUCCH_CHANNEL_SELECTION_CONFIG_V1020_PRESENT ^*/
}rrc_mac_pucch_format_r10_t;

#define RRC_MAC_MIMO_DL_TWO_LAYERS 2
#define RRC_MAC_MIMO_DL_FOUR_LAYERS 4
typedef struct _rrc_mac_supported_mimo_capability_dl_r10_t
{
    U8   num_Of_layer;  /*^ M, 0, B, 1, 4 ^*/
}rrc_mac_supported_mimo_capability_dl_r10_t;


typedef struct _rrc_mac_codebook_subset_restriction_v1020_t
{
    U8        cbsr_type;    /*^ M, 0, H, 0, 9 ^*/ /* codebook_subset_restriction_v1020_type_et */
    U8        cbsr_value[CBSR_MAX_VALUE_V1020]; /*^ M, O, OCTET_STRING, FIXED ^*/
}rrc_mac_codebook_subset_restriction_v1020_t;

typedef struct
{
    U8                           servCellIndex;
    /*^ M, 0, H, 0, 7 ^*/

    U8                           pdsch_start;
    /*^ M, 0, B, 1, 4 ^*/

} rrc_cross_carrier_scheduling_cell_info_other_t;

typedef struct
{
    U8  antenna_port_count_r10;
    /*^ M, 0, H, 0, 3 ^*/ /* rrc_rrm_csi_rs_r10_antennaPortCount_et */

    U8  resource_config;
    /*^ M, 0, H, 0, 31 ^*/ 

    U8  subframe_config;
    /*^ M, 0, H, 0, 154 ^*/ 

    S8  p_C; 
    /*^ M, 0, B, -8, 15 ^*/ 

} rrc_csi_rs_r10_setup_t;

typedef struct
{
    U8  zeroTxPowerTesource_configList[2];
    /*^ M, 0, OCTET_STRING, FIXED ^*/

    U8 zeroTxPowerSubframeConfig; 
    /*^ M, 0, H, 0, 154 ^*/ 

} rrc_zeroTxPowerCsi_setup_t;

typedef struct
{
  rrc_bitmask_t bitmask;    /*^ BITMASK ^*/
#define RRC_MAC_MAIN_CONFIG_V1020_SCELL_DEACTIVATION_TIMER_PRESENT    0x01
#define RRC_MAC_MAIN_CONFIG_V1020_EXTENDED_BSR_SIZES_PRESENT          0x02
#define RRC_MAC_MAIN_CONFIG_V1020_EXTENDED_PHR_PRESENT                0x04

  U8  scell_deactivation_timer;
/*^ O, RRC_MAC_MAIN_CONFIG_V1020_SCELL_DEACTIVATION_TIMER_PRESENT, N, 0, 0 ^*/  /* scell_deactivation_timer_et */

  U8   extended_bsr_sizes;
/*^ O, RRC_MAC_MAIN_CONFIG_V1020_EXTENDED_BSR_SIZES_PRESENT, H, 0, 0 ^*/  /* extended_bsr_sizes_et */

  U8   extended_phr;
/*^ O, RRC_MAC_MAIN_CONFIG_V1020_EXTENDED_PHR_PRESENT, H, 0, 0 ^*/  /* extended_phr_et */

} rrc_mac_main_config_v1020_t;



typedef struct _uplink_power_control_common_v1020_t
{
  U8    pucch_format3_r10;
/*^ M, 0, H, 0, 7 ^*/  /* rrc_rrm_pucch_format3_et */
  U8    pucch_format1_bcs_r10;
/*^ M, 0, H, 0, 1 ^*/  /* rrc_rrm_pucch_format1_bcs_et */
}uplink_power_control_common_v1020_t;

#define MAX_BAND_WIDTH_COMB_R10 4

typedef struct _supported_band_width_combination_set_r10_t
{
    U8      count;
/*^ M, 0, B, 1, 4 ^*/    /* MAX_BAND_WIDTH_COMB_R10 */
    U8      data[MAX_BAND_WIDTH_COMB_R10];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}supported_band_width_combination_set_r10_t;

typedef struct _band_combination_parameters_ext_r10_t
{
#define RRC_RRM_SUPPORTED_BAND_WIDTH_COMB_SET_R10 0x01
    U16    presence_bitmask;       /*^ BITMASK ^*/
    supported_band_width_combination_set_r10_t supported_band_width_combination_set_r10;
/*^ O, RRC_RRM_SUPPORTED_BAND_WIDTH_COMB_SET_R10, N, 0, 0 ^*/    
}band_combination_parameters_ext_r10_t;

typedef struct _supported_band_combination_ext_r10_ies_t
{
  /* SPR 20817 Fix Start */
    U8              count;     /*^ M, 0, B, 1,MAX_BAND_COMB_R10 ^*/   /* MAX_BAND_COMB_R10 */ 
  /* SPR 20817 Fix End */
    band_combination_parameters_ext_r10_t       band_combination_parameters_ext_r10[MAX_BAND_COMB_R10]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}supported_band_combination_ext_r10_ies_t;

typedef struct _rf_parameters_v1060_ies_t
{
    supported_band_combination_ext_r10_ies_t    supported_band_combination_ext_r10;
}rf_parameters_v1060_ies_t;

typedef struct _rrc_phy_scell_config_t
{
#define RRC_PHY_CONFIG_DEDICATED_SCELL_PRESENT 0x01
#define RRC_PHY_CONFIG_OPERATION_LIST_PRESENT  0x02    
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
    U8   cell_index; /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/
    U8   operation_type; 
    /*^ O, RRC_PHY_CONFIG_OPERATION_LIST_PRESENT, H, 0, 2 ^*/  /*operation_type_et*/

    rrc_radio_res_config_dedicated_scell_t  radio_res_config_dedicated_scell;
    /*^ O, RRC_PHY_CONFIG_DEDICATED_SCELL_PRESENT ^*/
}rrc_phy_scell_config_t;

typedef struct _rrc_phy_scell_config_list_t
{
/* SPR-18445 start */
    U8                     count;
/*^ M, 0, B, 1, RRC_MAX_SCELL ^*/
/* SPR-18445 stop */

    rrc_phy_scell_config_t rrc_phy_scell_config[RRC_MAX_SCELL];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_phy_scell_config_list_t;

typedef struct _rrc_mac_dl_num_harq_process_t
{
    U8   dl_num_harq_process; /*^ M, 0, B, 1, 15 ^*/
}rrc_mac_dl_num_harq_process_t;

typedef struct _rrc_mac_dl_init_mcs_t
{
    U8   dl_init_mcs; /*^ M, 0, H, 0, 28 ^*/
}rrc_mac_dl_init_mcs_t;

typedef struct _scell_deactivation_timer_t
{
    U8  scell_deactivation_timer; /*^ M, 0, H, 0, 128 ^*/
}scell_deactivation_timer_t;

typedef struct _rrc_mac_ca_config_error_t
{
    U8    cell_id; /*^ M, 0, H, 0, 7 ^*/
    U16   error_code;/*^ M, 0, B,7,7 ^*/
}rrc_mac_ca_config_error_t;

typedef struct _rrc_mac_ca_reconfig_error_t
{
    U8    cell_id; /*^ M, 0, H, 0, 7 ^*/
    U16   error_code;/*^ M, 0, B, 6, 6 ^*/
}rrc_mac_ca_reconfig_error_t;

typedef struct _rrc_mac_delete_ca_config_error_t
{
    U8    cell_id; /*^ M, 0, H, 0, 7 ^*/
    U16   error_code; /*^ M, 0, B, 6,6 ^*/
}rrc_mac_delete_ca_config_error_t;

typedef struct _scells_operation_list_t
{
   U8    chk_cellid_exist;
   U8    operation_type;
   /*CA start */
   U8    valid_ind; /* Used in case of rollback only*/
   /*CA end */
   U8    scell_to_add_after_deletion_flag;
}scells_operation_list_t;

typedef struct _rrc_mac_scell_rel_flag_t
{
    U8   scell_rel_flag; /*^ M, 0, H, 0, 1 ^*/
}rrc_mac_scell_rel_flag_t;

typedef enum 
{
  SCELL_NOT_ACTIVATED,
  SCELL_ACTIVATED
}scell_activate_status_et;

typedef struct _scell_activate_status_t
{
   U8 scellActivateStatus; /*^ M, 0, H, 0, 1 ^*//* scell_activate_status_et */                
} scell_activate_status_t;
/* SPR_18107 start */
typedef struct _rrc_mac_dl_earfcn_t
{
   U32 dlEarfcn; /*^ M, 0, H, 0, MAX_EARFCN ^*/
} rrc_mac_dl_earfcn_t;
/* SPR_18107 stop */
typedef enum
{
    RRC_RRM_INVAID_CONFIG = 0
}rrc_rrm_reestab_config_fail_cause_et;

typedef enum
{
    SCELL_NOT_EXISTS = 6,
    SCELL_NOT_RUNNING = 7
}rrc_mac_ca_config_error_code_et;

typedef enum
{
    SCELL_NO_ERROR,
    SCELL_ADD_FAILED ,
    SCELL_MOD_FAILED,
    SCELL_DEL_FAILED, 
    SCELL_CONFIG_FAIL
}scell_config_error_code_et;

typedef enum
{
   SF_160 = 0,
   SF_256 = 1,
   SF_320 = 2,
   SF_512 = 3,
   SF_640 = 4,
   SF_1024 = 5,
   SF_1280 = 6,
   SPARE_1 = 7
}meas_cycle_scell_r10_et;

#ifndef LOGGING_ENHANCEMENT_CELL_AND_UE_INDEX_STRUCT
#define LOGGING_ENHANCEMENT_CELL_AND_UE_INDEX_STRUCT
typedef struct _cell_and_ue_index
{
    U8  cell_index;
    U16 ue_index;
} cell_and_ue_index;
#endif
/*spr 22474 start*/
#define NVARPOOL    27
#define MINBUF      8
#define NVARMEMPOOL    27
/* This is used in debug GET Command */
typedef enum rrc_GetDebugTypeE
{
    RRC_GET_DEBUG_INFO_MEM_STATS, /* Mem Pool Statistics */
}rrc_DebugType;
typedef enum
{
    GET_MEM_POOL_STATS,
}rrc_oam_debug_info_req_et;
typedef enum
{
    WRONG_DEBUG_TYPE
}rrc_oam_debug_info_fail_cause_et;
/*spr 22474 end*/

#define M2AP_TWO                                2
#define MAX_NUM_OF_RESERVED_CELLS               255
#define MAX_CSA_PATTERN                         8

#define  M2AP_MAX_IP_ADDR_LENGTH                16
#define  M2AP_MAX_IPV6_ADDR_LENGTH              40
typedef struct
{
    U8 ipv6_addr[M2AP_MAX_IPV6_ADDR_LENGTH]; /*^ M,0,OCTET_STRING,FIXED ^*/
}m2ap_ipv6_addr_t;

typedef struct
{
    U8 ipv4_addr[M2AP_MAX_IP_ADDR_LENGTH]; /*^ M,0,OCTET_STRING,FIXED ^*/
}m2ap_ipv4_addr_t;
#define ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT       0x01
#define ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT       0x02
typedef struct
{
    /* IPV6 Start */
    U8 bitmask; /*^ BITMASK ^*/
    /* IPV6 Stop */
    /* number of ipv6 addr */
    /* number of ip addr */
    U16 port; /*^ M,0,N,0,0 ^*/
    
    U8 num_ip_addr; /*^ M,0,H,0,3 ^*/

    m2ap_ipv4_addr_t ip_addr[MAX_NUM_IP_ADDR]; /*^ M,0,OCTET_STRING,VARIABLE ^*/

    U8 num_ipv6_addr;
    /*^ O,ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT,B,1,3 ^*/
    /* IPV6 addresses of eNB */

    m2ap_ipv6_addr_t ipv6_addr[MAX_NUM_IP_ADDR];
    /*^ O,ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT,OCTET_STRING,VARIABLE ^*/
    /* IPV6 Stop */

}m2ap_enb_comm_info_t;
typedef enum
{
    MCCH_REPETITION_PERIOD_RF32,
    MCCH_REPETITION_PERIOD_RF64,
    MCCH_REPETITION_PERIOD_RF128,
    MCCH_REPETITION_PERIOD_RF256
}mcch_repetition_period_et;

typedef enum
{
    NOTIFICATION_REPETITION_COEFF_2,
    NOTIFICATION_REPETITION_COEFF_4
}notification_repetition_coeff_et;


typedef enum
{
    PDCCH_LENGTH_1 = 0,
    PDCCH_LENGTH_2
}pdcch_length_et;


typedef enum
{
    MCCH_MODIFICATION_PERIOD_512,
    MCCH_MODIFICATION_PERIOD_1024
}mcch_modification_period_et;

typedef enum
{
    SIGNALING_MCS_2,
    SIGNALING_MCS_7,
    SIGNALING_MCS_13,
    SIGNALING_MCS_19
}signalling_mcs_et;



typedef struct _m2ap_mac_csa_pattern_list_t
{
    U8               radio_frame_allocation_period; /*^ M, 0, H, 0, 5 ^*/ 
    /*radio_frame_allocation_period_et*/

    U8               radio_frame_allocation_offset; /*^ M, 0, H, 0, 7 ^*/ 
    U8               choice;   /*^ M, 0, H, 0, 1 ^*/
    U32              subframe_allocation; /*^ M, 0, N, 0, 0 ^*/

}m2ap_mac_csa_pattern_list_t;

/* MENB CHANGES - START */
#define    EN_GNB_ID_SIZE               4
#define    MAX_PEER_GNB                 32

/* ENDC_MEAS_CHANGES_START */
typedef enum 
{
    X2AP_KHZ_15, 
    X2AP_KHZ_30, 
    X2AP_KHZ_60, 
    X2AP_KHZ_120, 
    X2AP_KHZ_240, 
    X2AP_KHZ_SPARE3, 
    X2AP_KHZ_SPARE2, 
    X2AP_KHZ_SPARE1 
}x2ap_subcarrier_spacing_et;

typedef enum
{
  X2AP_DURATION_SF1,
  X2AP_DURATION_SF2,
  X2AP_DURATION_SF3,
  X2AP_DURATION_SF4,
  X2AP_DURATION_SF5
}x2ap_mtc_duration_et;

#define X2AP_PERIODICTY_OFFSET_SF5_PRESENT    0x01
#define X2AP_PERIODICTY_OFFSET_SF10_PRESENT   0x02
#define X2AP_PERIODICTY_OFFSET_SF20_PRESENT   0x04
#define X2AP_PERIODICTY_OFFSET_SF40_PRESENT   0x08
#define X2AP_PERIODICTY_OFFSET_SF80_PRESENT   0x10
#define X2AP_PERIODICTY_OFFSET_SF160_PRESENT  0x20

typedef struct _x2ap_periodicity_and_offset_t
{
      U8                    bitmask;
      /*^ BITMASK ^*/
      U8                    sf5;
      /*^ O, X2AP_PERIODICTY_OFFSET_SF5_PRESENT , H, 0, 4 ^*/
      U8                    sf10;
      /*^ O, X2AP_PERIODICTY_OFFSET_SF10_PRESENT , H, 0, 9 ^*/
      U8                    sf20;
      /*^ O, X2AP_PERIODICTY_OFFSET_SF20_PRESENT , H, 0, 19 ^*/
      U8                    sf40;
      /*^ O, X2AP_PERIODICTY_OFFSET_SF40_PRESENT , H, 0, 39 ^*/
      U8                    sf80;
      /*^ O, X2AP_PERIODICTY_OFFSET_SF80_PRESENT , H, 0, 79 ^*/
      U8                    sf160;
      /*^ O, X2AP_PERIODICTY_OFFSET_SF160_PRESENT , H, 0, 159 ^*/

}x2ap_periodicity_and_offset_t;

typedef struct _x2ap_ssb_mtc_t
{
  x2ap_periodicity_and_offset_t x2ap_periodicity_and_offset;
  /*^ M, 0, N, 0, 0 ^*/
  U8    x2ap_mtc_duration; /* x2ap_mtc_duration_et */
  /*^ M, 0, H, 0, X2AP_DURATION_SF5 ^*/
}x2ap_ssb_mtc_t;

typedef struct _x2ap_measurement_slots_t
{
  U8    numbits;
  /*^ M, 0, B, 1, 80 ^*/
  U8    meas_slots[80];
  /*^ M, 0, OCTET_STRING, VARIABLE, H, 0, 1 ^*/
}x2ap_measurement_slots_t;

typedef struct _x2ap_ss_rssi_measurement_t
{
   x2ap_measurement_slots_t x2ap_measurement_slots;
   /*^ M, 0, N, 0, 0 ^*/
   U8   end_synmbol;
   /*^ M, 0, H, 0, 3 ^*/
}x2ap_ss_rssi_measurement_t;

#define X2AP_MEAS_SS_RSSI_MEASUREMENT_PRESENT   0x01
typedef struct _x2ap_meas_timing_config_t
{
      U8                         bitmask;
      /*^ BITMASK ^*/

      U32                        carrier_freq;
      /*^ M, 0, H, 0, NR_MAX_EARFCN ^*/

      U8                         ssb_subcarrier_spacing;
      /*^ M, 0, H, 0, X2AP_KHZ_240 ^*/
      /* x2ap_subcarrier_spacing_et */

      x2ap_ssb_mtc_t             ssb_meas_timimg_configuration;
      /*^ M, 0, N, 0, 0 ^*/

      x2ap_ss_rssi_measurement_t ss_rssi_measurement;
      /*^ O, X2AP_MEAS_SS_RSSI_MEASUREMENT_PRESENT, N, 0, 0 ^*/
}x2ap_meas_timing_config_t;

/* ENDC_MEAS_CHANGES_END */

typedef struct
{
    U8    x2_gnb_id[EN_GNB_ID_SIZE];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
}x2_gnb_id_t;

typedef struct
{
  x2ap_plmn_identity_t    plmn_identity;
  /*^ M, 0, N, 0, 0 ^*/

  x2_gnb_id_t             x2_en_gnb_id;
  /*^ M, 0, N, 0, 0 ^*/
}x2_gb_gnb_id_t;

#define    NR_MAX_FREQ                  8 
#define    NR_MAX_FREQ_BANDS            32
#define    NR_MAX_FREQ_BAND_INDICATOR   1024
#define    NR_MAX_EARFCN                3279165
#define    NR_MAX_PCI                   1007
#define    NR_MAX_SERVED_CELLS          16384
#define    NR_CELL_ID_OCTET_SIZE        5
#define    X2AP_NR_CELL_ID_BIT_STR_LEN    36
#define    MAX_NUM_OF_IDX_TO_REPORT2    64
#define    MAX_NR_CELL_REPORT           8
typedef enum
{
    SCS_15,
    SCS_30,
    SCS_60,
    SCS_120,
    MAX_NRCS = SCS_120
}x2ap_nrcs_et;

typedef enum 
{
   X2AP_NRB11,
   X2AP_NRB18,
   X2AP_NRB24,
   X2AP_NRB25,
   X2AP_NRB31,
   X2AP_NRB32,
   X2AP_NRB38,
   X2AP_NRB51,
   X2AP_NRB52,
   X2AP_NRB65,
   X2AP_NRB66,
   X2AP_NRB78,
   X2AP_NRB79,
   X2AP_NRB93,
   X2AP_NRB106,
   X2AP_NRB107,
   X2AP_NRB121,
   X2AP_NRB132,
   X2AP_NRB133,
   X2AP_NRB135,
   X2AP_NRB160,
   X2AP_NRB162,
   X2AP_NRB189,
   X2AP_NRB216,
   X2AP_NRB217,
   X2AP_NRB245,
   X2AP_NRB264,
   X2AP_NRB270,
   X2AP_NRB273,
   X2AP_NRB_MAX = X2AP_NRB273
}x2ap_nrnb_et;

typedef struct
{
    U32    freq_band_indicator_nr;
    /*^ M, 0, B, 1, NR_MAX_FREQ_BAND_INDICATOR  ^*/

    U8     num_supported_sul_band;
    /*^ M, 0, H, 0, NR_MAX_FREQ_BANDS ^*/

    U16    supported_sul_freq_band_indicator_nr[NR_MAX_FREQ_BANDS];
    /*^ M, 0, OCTET_STRING, VARIABLE, B, 1, NR_MAX_FREQ_BAND_INDICATOR ^*/
}x2ap_nr_freq_band_info_t;

typedef struct
{
    U8    nr_scs;
    /*^ M, 0, H, 0, MAX_NRCS ^*/
    /* x2ap_nrcs_et */

    U8    nr_nrb;
    /*^ M, 0, H, 0, X2AP_NRB_MAX ^*/
    /* x2ap_nrnb_et */
}x2ap_nr_tx_bandwidth_t;



typedef struct
{
    U32     nr_sul_arfcn;
    /*^ M, 0, N, 0, NR_MAX_EARFCN ^*/

    x2ap_nr_tx_bandwidth_t nr_tx_band_width;
    /*^ M, 0, N, 0, 0 ^*/

}x2ap_sul_information_t;



#define X2AP_NR_ARFCN_SUL_PRESENT    0x01
#define X2AP_SUL_INFORMATION_PRESENT 0x02
typedef struct
{
    rrc_bitmask_t               bitmask;
    /*^ BITMASK ^*/

    U32                         nr_earfcn;
    /*^ M, 0, H, 0, NR_MAX_EARFCN ^*/

    U8                          num_freq_bands;
    /*^ M, 0, B, 1, NR_MAX_FREQ_BANDS ^*/

    x2ap_nr_freq_band_info_t    freq_band_info[NR_MAX_FREQ_BANDS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U32                         nr_earfcn_sul;
    /*^ O, X2AP_NR_ARFCN_SUL_PRESENT, H, 0, NR_MAX_EARFCN ^*/

    x2ap_sul_information_t      nr_sul_information;
    /*^ O, X2AP_SUL_INFORMATION_PRESENT, N, 0, 0 ^*/
}x2ap_nr_freq_info_t;



typedef struct
{
    x2ap_nr_freq_info_t    nr_freq_info;
    /*^ M,0,N,0,0 ^*/

}x2ap_tdd_info_neigh_served_nr_cell_info_t;


typedef struct
{
    x2ap_nr_freq_info_t     ul_nr_freq_Info;
    /*^ M,0,N,0,0 ^*/

    x2ap_nr_freq_info_t     dl_nr_freq_Info;
    /*^ M,0,N,0,0 ^*/

}x2ap_fdd_info_neigh_served_nr_cell_info_t;


#define X2AP_NR_NEIGHBOUR_FDD_PRESENT   0x01
#define X2AP_NR_NEIGHBOUR_TDD_PRESENT   0x02
typedef struct
{
    rrc_bitmask_t                                 bitmask;
    /*^ BITMASK ^*/

    x2ap_fdd_info_neigh_served_nr_cell_info_t     fdd_info;
    /*^ O,X2AP_NR_NEIGHBOUR_FDD_PRESENT,N,0,0 ^*/

    x2ap_tdd_info_neigh_served_nr_cell_info_t     tdd_info;
    /*^ O,X2AP_NR_NEIGHBOUR_TDD_PRESENT,N,0,0 ^*/
}x2ap_nr_neigh_mode_info_t;


typedef struct
{
   x2ap_nr_freq_info_t       nr_freq_info;
   /*^ M, 0, N, 0, 0 ^*/

   x2ap_nr_tx_bandwidth_t    nr_tx_bandwidth;
   /*^ M, 0, N, 0, 0 ^*/
}x2ap_nr_tdd_into_t;

typedef struct
{
    x2ap_nr_freq_info_t       nr_ul_freq_info;
    /*^ M, 0, N, 0, 0 ^*/

    x2ap_nr_freq_info_t       nr_dl_freq_info;
    /*^ M, 0, N, 0, 0 ^*/

    x2ap_nr_tx_bandwidth_t    nr_ul_tx_bdwidth;
    /*^ M, 0, N, 0, 0 ^*/

    x2ap_nr_tx_bandwidth_t    nr_dl_tx_bdwidth;
    /*^ M, 0, N, 0, 0 ^*/
}x2ap_nr_fdd_into_t;

#define    X2AP_NR_MODE_FDD_PRESENT   0x01
#define    X2AP_NR_MODE_TDD_PRESENT   0x02
typedef struct
{
    U8                    bitmask;
    /*^ BITMASK ^*/

    x2ap_nr_fdd_into_t    nr_fdd_info;
    /*^ O, X2AP_NR_MODE_FDD_PRESENT, N, 0, 0 ^*/

    x2ap_nr_tdd_into_t    nr_tdd_info;
    /*^ O, X2AP_NR_MODE_TDD_PRESENT, N, 0, 0 ^*/
}x2ap_choice_nr_mode_info_t;

typedef struct
{
    x2ap_plmn_identity_t  plmn_identity;
    /*^ M,0,N,0,0 ^*/

    U8                    nr_cell_id[NR_CELL_ID_OCTET_SIZE];
    /*^ M,0,OCTET_STRING,FIXED ^*/
}x2ap_nr_ecgi_t;

typedef struct
{
    U8     nrcellId[NR_CELL_ID_OCTET_SIZE]; /*^ M,0,OCTET_STRING,FIXED ^*/
}x2ap_nr_cell_id_t;

#define RRC_MAX_MEAS_FREQ_MN          32
#define X2AP_FIVE_GS_TAC_PRESENT      0x01
#define X2AP_CONFIGURED_TAC_PRESENT   0x02
typedef struct
{
    rrc_bitmask_t                    bitmask;
    /*^ BITMASK ^*/

    U16                              nr_pci;
    /*^ M, 0, H, 0, NR_MAX_PCI ^*/

    x2ap_nr_ecgi_t                   nr_cell_id;
    /*^ M, 0, N, 0, 0 ^*/

    x2ap_nr_freq_info_t              nr_freq_info;
    /*^ M, 0, N, 0, 0 ^*/

#ifndef LINUX_PC_TEST
/* ENDC_MEAS_CHANGES_START */
    U8                                 meas_timing_config_count;
    /*^ M, 0, B, 1, RRC_MAX_MEAS_FREQ_MN ^*/

    x2ap_meas_timing_config_t          x2ap_meas_timing_config[RRC_MAX_MEAS_FREQ_MN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
/* ENDC_MEAS_CHANGES_END */
#endif

    x2ap_nr_neigh_mode_info_t        nr_neigh_Mode_Info;
    /*^ M, 0, N, 0, 0 ^*/

    x2ap_five_gs_tac_t               fivegs_tac;
    /*^ O,X2AP_FIVE_GS_TAC_PRESENT,N,0,0 ^*/

    x2ap_tac_t                       configured_tac;
    /*^ O,X2AP_CONFIGURED_TAC_PRESENT,N,0,0 ^*/
    
#ifdef LINUX_PC_TEST
    U32                              meas_timing_config_buf_len;
    /*^ M, 0, H, 0, MAX_ASN_BUFFER ^*/

    U8                               measurement_timing_config_asn_buff[MAX_ASN_BUFFER];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
#endif

}x2ap_nr_neigh_info_t;


typedef struct
{
    x2ap_nr_neigh_info_t       nr_neigh_info;
    /*^ M,0,N,0,0 ^*/
}x2ap_nr_neigh_cell_arry_t;

typedef struct
{
    U16                             num_neigh_cell;	/*^ M, 0, H, 0, MAX_SERVED_CELLS ^*/
    x2ap_nr_neigh_cell_arry_t       nr_neigh_cell_arr[MAX_SERVED_CELLS];
}x2ap_nr_nbour_cell_info_t;

typedef struct
{
	U8   type; /*^ M,0,N,0,0 ^*/
	U16  value; /*^M,0,N,0,0 ^*/
} x2ap_cause_t;

typedef struct
{
    UInt16				nrcgi_count;
    x2ap_nr_ecgi_t		nrcgi_list[MAX_SERVED_CELLS];
}x2ap_partial_list_t;

typedef struct
{
    U16 bitmask; /*^ BITMASK ^*/
    U16 pci; /*^ M,0,H,0,503 ^*/
    U8 cell_id; /*^ M,0,N,0,0 ^*/ /*This is  Cellid in case of X2AP_ENB_CONFIG_UPDATE_IND otherwise it is use as Cell Index *//*SPR 21554 +-*/
    U8 tac[MAX_TAC_BYTES]; /*^ M,0,OCTET_STRING,FIXED ^*/
    x2ap_broadcast_plmn_t bcast_plmn_info; /*^ M,0,N,0,0 ^*/
    x2ap_choice_eutra_mode_info_t choice_eutra_mode; /*^ M,0,N,0,0 ^*/
    U8 num_antenna_port; /*^ O,X2AP_NUM_ANTENNA_INFO_PORT_PRESENT,H,0,2 ^*/
    x2ap_prach_configuration_info_t prach_config; /*^ O,X2AP_PRACH_CONFIG_INFO_PRESENT,N,0,0 ^*/
    x2ap_mbsfn_subframe_info_t mbsfn_subframe_info; /*^ O,X2AP_MBSFN_SUBFRAME_INFO_PRESENT,N,0,0 ^*/
    /*36423_CR0407_start*/
    U8    csg_identity[CSG_ID_OCTET_SIZE];/*^ O,X2AP_CSG_ID_INFO_PRESENT,OCTET_STRING,FIXED ^*/
    /*36423_CR0407_stop*/
}x2ap_served_cell_info_t;

/*OPTION3X CHANGES START*/
typedef enum
{
   RRM_DC_BEARER_CHANGE_FAIL,
   RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD,
   RRM_DC_BEARER_CHANGE_FAIL_RRC_RECONFIG,
   RRM_DC_BEARER_CHANGE_FAIL_UE_REL,
   RRM_DC_BEARER_CHANGE_FAIL_ENB_UE_REL,
   RRM_DC_BEARER_CHANGE_FAIL_CELL_DEL,
   RRM_DC_BEARER_CHANGE_FAIL_RLF,
   RRM_DC_BEARER_CHANGE_FAIL_HO,
   RRM_DC_BEARER_CHANGE_FAIL_S1AP_RESET,
   RRM_DC_BEARER_CHANGE_FAIL_RE_ESTABLISH,
   RRM_DC_BEARER_CHANGE_FAIL_SGNB_RECONFIG_COMPLETE,
   RRM_DC_BEARER_CHANGE_FAIL_ERAB_MODIFIC_IND,
   RRM_DC_BEARER_CHANGE_FAIL_X2AP_RESET,
   RRM_DC_BEARER_CHANGE_FAIL_MAX
}rrm_dc_bearer_change_fail_cause_et;
/*OPTION3X CHANGES STOP*/

#ifdef LINUX_PC_TEST
typedef struct _TAG_TEST_SCTP_MESSAGE_T
{
	U32 ulSocketID;
	U16 usStreamID;
	U8 pucPayload[0];
}test_sctp_message_t ;

typedef struct _TAG_TEST_SCTP_CMD_T
{
	U32 ulSocketID;
	U16 usStreamID;
}test_sctp_cmd_t ;

//currently only support IPv4
typedef struct _TAG_TEST_X2AP_LKSCTP_MESSAGE_T
{
    U32 ulSocketID;
    U16 usStreamID;
    U16 usPort;
    char aucIpv4[16];
    U8 pucPayload[0];
}test_x2ap_lksctp_message_t;

typedef struct _TAG_TEST_X2AP_CLOSE_SCTP_T
{
	U32 ulSocketID;
}test_x2ap_close_sctp_t;

enum timer_type
{
	STOP_TIMER,
	START_TIMER,
	TIMER_EXPIRE
};
#define TIMER_MESSAGE_TO_PC_BASE 0X1E00
#define TIMER_MESSAGE_TO_PC_S1AP   (TIMER_MESSAGE_TO_PC_BASE+1)	
#define TIMER_MESSAGE_TO_PC_X2AP   (TIMER_MESSAGE_TO_PC_BASE+2)	
#define TIMER_MESSAGE_TO_PC_OAMH   (TIMER_MESSAGE_TO_PC_BASE+3)	
#define TIMER_MESSAGE_TO_PC_UECC   (TIMER_MESSAGE_TO_PC_BASE+4)	
#define TIMER_MESSAGE_TO_PC_CSC   (TIMER_MESSAGE_TO_PC_BASE+5)	
#define TIMER_MESSAGE_TO_PC_LLIM   (TIMER_MESSAGE_TO_PC_BASE+6)	
#define TIMER_MESSAGE_TO_PC_UECCMD   (TIMER_MESSAGE_TO_PC_BASE+7)	
typedef struct _timer_info
{
	U8 msgType;
	U8 timerId;
}timer_info_t;


#define PCTEST_TIMER_EXPIRE_BASE 0X1E50
#define PCTEST_TIMER_EXPIRE_TO_CSC   (PCTEST_TIMER_EXPIRE_BASE+0)	
typedef struct _pctest_timer_expire_to_csc
{
	U8 submodule;/*csc_submodules_et*/
	U8 cellindex;
	U8 event;/*cell_m_fsm_event_et, valid only if submodule is CSC_SUBMODULE_CELL_M*/
    
    U8 repeatNumber;
}pctest_timer_expire_to_csc_t;


#endif
/* MENB CHANGES - END */
#pragma pack(pop)

#endif /* _RRC_DEFINES_H_ */
