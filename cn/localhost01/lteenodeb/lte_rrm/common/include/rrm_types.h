/*! \file rrm_types.h
 *  \brief This file contains the enums and constants definitions for CELL.
 *  \date Feburary 15, 2012
 *  \author gur21481
 */
/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
/****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *  DATE         AUTHOR          REFERENCE          REASON
 * 01/05/2014    gur32417        CA STG 3         Carrier Aggregation Stage 3
 *                                                code changes
****************************************************************************/

#ifndef _RRM_TYPES_H_
#define _RRM_TYPES_H_

/*! \enum rrmc_delta_f_pucch_format_one_et
 *  \brief An enum for Delta PUCCH format one enum
 */
typedef enum
{
  RRMC_DELTAF_NEG_TWO,
  RRMC_DELTAF_ZERO,
  RRMC_DELTAF_POS_TWO
}rrmc_delta_f_pucch_format_one_et;

typedef enum
{
  RRMC_DELTA_F1,
  RRMC_DELTA_F3,
  RRMC_DELTA_F5
}rrmc_delta_f_pucch_format_one_b_et;

typedef enum
{
  RRMC_FORMAT2_DELTA_NEG_TWO,
  RRMC_FORMAT2_DELTA_ZERO,
  RRMC_FORMAT2_DELTA_POS_ONE,
  RRMC_FORMAT2_DELTA_POS_TWO
}rrmc_delta_f_pucch_format_two_et;

typedef enum
{
  RRMC_DELTA_NEG_TWO,
  RRMC_DELTA_ZERO,
  RRMC_DELTA_POS_TWO
}rrmc_delta_f_pucch_format_two_ab_et;

/* CA_Stage3_PUCCH_Format_Enh: Start */
typedef enum
{
  RRMC_FORMAT_1B_CS_R10_DELTA_POS_ONE,
  RRMC_FORMAT_1B_CS_R10_DELTA_POS_TWO
}rrmc_delta_f_pucch_format_one_b_cs_r10_et;

typedef enum
{
  RRMC_FORMAT_3_R10_DELTA_NEG_ONE,
  RRMC_FORMAT_3_R10_DELTA_ZERO,
  RRMC_FORMAT_3_R10_DELTA_POS_ONE,
  RRMC_FORMAT_3_R10_DELTA_POS_TWO,
  RRMC_FORMAT_3_R10_DELTA_POS_THREE,
  RRMC_FORMAT_3_R10_DELTA_POS_FOUR,
  RRMC_FORMAT_3_R10_DELTA_POS_FIVE,
  RRMC_FORMAT_3_R10_DELTA_POS_SIX
}rrmc_delta_f_pucch_format_three_r10_et;
/* CA_Stage3_PUCCH_Format_Enh: End */

typedef enum
{
  RRMC_UL_BW_6RB,
  RRMC_UL_BW_15RB,
  RRMC_UL_BW_25RB,
  RRMC_UL_BW_50RB,
  RRMC_UL_BW_75RB,
  RRMC_UL_BW_100RB
}rrmc_ul_bandwidth_et;

/*29220: cell_config params from excel file*/
typedef enum    
{
    ANTENNA_PORTS_COUNT_AN1 = 1,
    ANTENNA_PORTS_COUNT_AN2 = 2,
    ANTENNA_PORTS_COUNT_AN4 = 4,
    ANTENNA_PORTS_COUNT_SPARE1
}antenna_ports_count_et;

/* No support at other stacks. So these will not be supported in first release */
/* No support at other stacks. So these will not be supported in first release. */
/*We do not have to support TDD so these will not be supported in first release */
typedef enum 
{
    PILOT_STRENGTH_GERAN
}rrmc_irat_filter_coeff_geran_et;


typedef enum
{
    TRANSMISSION_MODE_TM1 = 0,
    TRANSMISSION_MODE_TM2 = 1,
    TRANSMISSION_MODE_TM3 = 2,
    TRANSMISSION_MODE_TM4 = 3,
    TRANSMISSION_MODE_TM5 = 4,
    TRANSMISSION_MODE_TM6 = 5,
    TRANSMISSION_MODE_TM7 = 6,
    TRANSMISSION_MODE_TM8_V920 = 7,
    TM_MODE_INVALID_TYPE  = 8
} transmission_mode_ue_service_profile_et;

/* Uplink power control starts */
typedef enum
{
    /* BUG_11317_FIX_START */
    OLPC_TABLE_CATEGORY_1 = 1,
    OLPC_TABLE_CATEGORY_2,
    OLPC_TABLE_CATEGORY_3,
    OLPC_TABLE_CATEGORY_4
    /* BUG_11317_FIX_END */
} rrm_ue_olpc_category_ue_service_profile_et;
/* Uplink power control end */

typedef enum
{
    DSR_TRANS_MAX_N4 = 0,
    DSR_TRANS_MAX_N8 = 1,
    DSR_TRANS_MAX_N16 = 2,
    DSR_TRANS_MAX_N32 = 3,
    DSR_TRANS_MAX_N64 = 4
} dsr_trans_max_ue_service_profile_et;

typedef enum
{
    CLOSE_LOOP = 0,
    OPEN_LOOP = 1
} ue_transmit_antenna_selection_type_ue_service_profile_et;

#ifndef OAM_IPR
typedef enum
{
    N2_TX_ANTENNA_TM3 = 0,
    N4_TX_ANTENNA_TM3 = 1,
    N2_TX_ANTENNA_TM4 = 2,
    N4_TX_ANTENNA_TM4 = 3,
    N2_TX_ANTENNA_TM5 = 4,
    N4_TX_ANTENNA_TM5 = 5,
    N2_TX_ANTENNA_TM6 = 6,
    N4_TX_ANTENNA_TM6 = 7
} codebook_subset_restriction_type_ue_service_profile_et;
#endif

typedef enum
{
    CQI_REPORTING_MODE_APERIODIC_RM12 = 0,
    CQI_REPORTING_MODE_APERIODIC_RM20 = 1,
    CQI_REPORTING_MODE_APERIODIC_RM22 = 2,
    CQI_REPORTING_MODE_APERIODIC_RM30 = 3,
    CQI_REPORTING_MODE_APERIODIC_RM31 = 4
} cqi_reporting_mode_aperiodic_ue_service_profile_et;


typedef enum
{
    DELTA_MCS_EN0 = 0,
    DELTA_MCS_EN1 = 1
} delta_mcs_enabled_ue_service_profile_et;

typedef enum
{
    ACK_NACK_REPETITION_FACTOR_N2 = 0,
    ACK_NACK_REPETITION_FACTOR_N4 = 1,
    ACK_NACK_REPETITION_FACTOR_N6 = 2
} ack_nack_repetition_factor_ue_service_profile_et;


typedef enum
{
  RRMC_BIT_RATE_KBPS_0,
  RRMC_BIT_RATE_KBPS_8,
  RRMC_BIT_RATE_KBPS_16,
  RRMC_BIT_RATE_KBPS_32,
  RRMC_BIT_RATE_KBPS_64,
  RRMC_BIT_RATE_KBPS_128,
  RRMC_BIT_RATE_KBPS_256,
  RRMC_BIT_RATE_INFINITY
}rrc_prioritized_bit_rate_erb_service_profile_et;

typedef enum
{
  RRMC_BUCKET_SIZE_MS_50,
  RRMC_BUCKET_SIZE_MS_100,
  RRMC_BUCKET_SIZE_MS_150,
  RRMC_BUCKET_SIZE_MS_300,
  RRMC_BUCKET_SIZE_MS_500,
  RRMC_BUCKET_SIZE_MS_1000
}rrc_bucket_size_duration_erb_service_profile_et;

typedef enum
{
    RRM_MIF_CELLM_TRGR_PERIODIC = 1,
    RRM_MIF_CELLM_TRGR_THRESHOLD,
    RRM_MIF_CELLM_TRGR_EVENT,
    /*This should always be the last value of enum*/
    RRM_MIF_CELLM_TRGR_UNDEF
}rrm_cellm_trigger_type_et;

typedef enum
{
    RRM_MIF_CELLM_ATTR_CELL_STATE = 1,
    RRM_MIF_CELLM_ATTR_ACTIVE_UE_COUNT_REPORT,
    RRM_MIF_CELLM_ATTR_ACTIVE_UE_THRESHOLD,
    RRM_MIF_CELLM_ATTR_S1_MSG_NOTIFICATION,
    RRM_MIF_CELLM_ATTR_MEAS_UE_COUNT,
    RRM_MIF_CELLM_ATTR_MEAS_REPORTING_INTERVAL,
    RRM_MIF_CELLM_ATTR_HO_REPORT,
    RRM_MIF_CELLM_ATTR_MLB_SELF,
    RRM_MIF_CELLM_ATTR_MLB_NBR,
    /*  MRO changes started */
    RRM_MIF_CELLM_ATTR_HO_ATTEMPT_IND,
    RRM_MIF_CELLM_ATTR_HO_FAILURE_REPORT,
    /*  MRO changes end */
    /* RRM ES Start */
    RRM_MIF_CELLM_ATTR_ES_LD_SELF,
    /* RRM ES End */
    /* SPR 20653 Fix Start */
    RRM_MIF_CELLM_ATTR_UE_THROUGHPUT_DL_ANR_THRESHOLD,
    RRM_MIF_CELLM_ATTR_UE_THROUGHPUT_UL_ANR_THRESHOLD,
    /* SPR 20653 Fix End */
    /*This should always be the last value of enum*/
    RRM_MIF_CELLM_ATTR_UNDEF
}rrm_cellm_attribute_id_et;

typedef enum
{
    RRM_MIF_CELLM_CELL_OPERATIONAL = 1,
    RRM_MIF_CELLM_CELL_UNOPERATIONAL_DUE_TO_SWITCHOFF,
    RRM_MIF_CELLM_CELL_UNOPERATIONAL_DUE_TO_CELL_DELETE,
    /*This should always be the last value of enum*/
    RRM_MIF_CELLM_CELL_STATE_UNDEFINED
}rrm_mif_cellm_cell_operational_state_et;

typedef enum
{
    RRM_MIF_CELLM_THRESHOLD_CROSSED,
    RRM_MIF_CELLM_BELOW_TH
}rrm_cellm_threshold_hit_cause_et;


typedef enum
{
    RRM_MIF_CELLM_CELL_ADD = 1,
    RRM_MIF_CELLM_CELL_DELETE,
    RRM_MIF_CELLM_CELL_STOP,
    RRM_MIF_CELLM_CELL_START,
    /*This should always be the last value of enum*/
    RRM_MIF_CELLM_CELL_STATE_CHANGE_CAUSE_UNDEFINED
}rrm_mif_cellm_cell_state_change_cause_et;

typedef enum
{
    RADIO_NETWORK_LAYER_TYPE = 1,
    RRM_T_S1AP_CAUSE_TRANSPORT = 2,
    /* SPR_8345_fix_start */
    RRM_NAS_CAUSE_TYPE = 3,
    /* SPR_8345_fix_end */
    /* SPR 13365 Start */
    RRM_X2AP_CAUSE_MISC = 4,
    /* SPR 13365 End */
    RRM_S1AP_CAUSE_MISC = 5
}rrm_type_et;


typedef enum
{
    TRANSPORT_RESOURCE_UNAVAILABLE,
    UNSPECIFIED_TRANSPORT_RESOURCE
}rrm_transport_resource_cause_et;


typedef enum
{
    UNSPECIFIED,
    TX2RELOCOVERALL_EXPIRY,
    SUCCESSFUL_HANDOVER,
    RELEASE_DUE_TO_EUTRAN_GENERATED_REASON,
    HANDOVER_CANCELLED,
    PARTIAL_HANDOVER,
    HANDOVER_FAILURE_IN_TARGET_EPC_ENB_OR_TARGET_SYSTEM,
    HANDOVER_TARGET_NOT_ALLOWED,
    TS1RELOCOVERALL_EXPIRY,
    TS1RELOCPREP_EXPIRY,
    CELL_NOT_AVAILABLE,
    UNKNOWN_TARGET_ID,
    NO_RADIO_RESOURCES_AVAILABLE_IN_TARGET_CELL,
    UNKNOWN_OR_ALREADY_ALLOCATED_MME_UE_S1AP_ID,
    UNKNOWN_OR_ALREADY_ALLOCATED_ENB_UE_S1AP_ID,
    UNKNOWN_OR_INCONSISTENT_PAIR_OF_UE_S1AP_ID,
    HANDOVER_DESIRABLE_FOR_RADIO_REASONS,
    TIME_CRITICAL_HANDOVER,
    RESOURCE_OPTIMISATION_HANDOVER,
    REDUCE_LOAD_IN_SERVING_CELL,
    USER_INACTIVITY,
    RADIO_CONNECTION_WITH_UE_LOST,
    LOAD_BALANCING_TAU_REQUIRED,
    CS_FALLBACK_TRIGGERED,
    /*SPR_7742_fix Rel10_CRs:36.331-490 release cause CS_FB_HIGH_PRIORITY
     remove from here it will take care by L3*/
    UE_NOT_AVAILABLE_FOR_PS_SERVICE,
    RADIO_RESOURCES_NOT_AVAILABLE,
    FAILURE_IN_THE_RADIO_INTERFACE_PROCEDURE,
    INVALID_QOS_COMBINATION,
    INTER_RAT_REDIRECTION,
    INTERACTION_WITH_OTHER_PROCEDURE,
    UNKNOWN_ERAB_ID,
    MULTIPLE_ERAB_ID_INSTANCES,
    ENCRYPTION_AND_OR_INTEGRITY_PROTECTION_ALGORITHMS_NOT_SUPPORTED,
    S1_INTRA_SYSTEM_HANDOVER_TRIGGERED,
    S1_INTER_SYSTEM_HANDOVER_TRIGGERED,
    X2_HANDOVER_TRIGGERED,
    /*These should always be the last value of enum*/
    REDIRECTION_TOWARDS_1XRTT,
    NOT_SUPPORTED_QCI_VALUE,
    S1AP_INVALID_CSG_ID
}rrm_radio_network_cause_et;

typedef enum
{
    RRM_X2AP_HO_DESIRABLE_FOR_RADIO_REASONS,
    RRM_X2AP_TIME_CRITICAL_HO,
    RRM_X2AP_RESRC_OPTIMISATION_HO,
    RRM_X2AP_REDUCE_LOAD_IN_SERVING_CELL,
    RRM_X2AP_PARTIAL_HANDOVER,
    RRM_X2AP_UNKNOWN_NEW_ENB_UE_X2AP_ID,
    RRM_X2AP_UNKNOWN_OLD_ENB_UE_X2AP_ID,
    RRM_X2AP_UNKNOWN_PAIR_OF_UE_X2AP_ID,
    RRM_X2AP_HO_TARGET_NOT_ALLOWED,
    RRM_X2AP_TX2RELOCOVERALL_EXPIRY,
    RRM_X2AP_TRELOCPREP_EXPIRY,
    RRM_X2AP_CELL_NOT_AVAILABLE,
    RRM_X2AP_NO_RADIO_RESRCS_AVLAL_IN_TRGET_CELL,
    RRM_X2AP_INVALID_MME_GROUP_ID,
    RRM_X2AP_UNKNOWN_MME_CODE,
    RRM_X2AP_ENC_NO_INT_PROT_ALGO_NOT_SUPPORTED,
    RRM_X2AP_REPORT_CHARACTERISTICS_EMPTY,
    RRM_X2AP_NO_REPORTPERIODICITY,
    RRM_X2AP_EXISTING_MEASUREMENT_ID,
    RRM_X2AP_UNKNOWN_ENB_MEASUREMENT_ID,
    RRM_X2AP_MEAS_TEMP_NOT_AVAILABLE,
    RRM_X2AP_UNSPECIFIED,
    RRM_X2AP_LOAD_BALANCING,
    RRM_X2AP_HO_OPTIMISATION,
    RRM_X2AP_VALUE_OUT_OF_ALLOWED_RANGE,
    RRM_X2AP_MULTI_E_RAB_ID_INSTANCES,
    RRM_X2AP_SWITCH_OFF_ONGOING,
    RRM_X2AP_NOT_SUPPORTED_QCI_VALUE,
    /*Rel10 CR 393 changes start*/
    RRM_X2AP_MEASUREMENT_NOT_SUPPORTED_FOR_THE_OBJECT
    /*Rel10 CR 393 changes end*/
}rrm_x2ap_radio_nw_cause_et;

/* SPR_8345_fix_start */
typedef enum
{
    RRM_NAS_NORMAL_RELEASE,
    RRM_NAS_AUTHENTICATION_FAILURE,
    RRM_NAS_DETACH,
    RRM_NAS_UNSPECIFIED,
    RRM_NAS_CSG_SUBSCRIPTION_EXPIRY
}rrm_nas_cause_et;
/* SPR_8345_fix_end */

typedef enum
{
    CAUSE_MISC_CONTROL_PROCESSING_OVERLOAD,
    S1AP_NOT_ENOUGH_USER_PLANE_PROCESSING_RESOURCES,
    CAUSE_MISC_HARDWARE_FAILURE,
    CAUSE_MISC_OAM_INTERVENTION,
    CAUSE_MISC_UNSPECIFIED,
    CAUSE_MISC_UNKNOWN_PLMN
}rrm_s1ap_misc_cause_et;

typedef enum
{
  RRM_MIF_AC_BARRING_FACTOR_P00,
  RRM_MIF_AC_BARRING_FACTOR_P05,
  RRM_MIF_AC_BARRING_FACTOR_P10,
  RRM_MIF_AC_BARRING_FACTOR_P15,
  RRM_MIF_AC_BARRING_FACTOR_P20,
  RRM_MIF_AC_BARRING_FACTOR_P25,
  RRM_MIF_AC_BARRING_FACTOR_P30,
  RRM_MIF_AC_BARRING_FACTOR_P40,
  RRM_MIF_AC_BARRING_FACTOR_P50,
  RRM_MIF_AC_BARRING_FACTOR_P60,
  RRM_MIF_AC_BARRING_FACTOR_P70,
  RRM_MIF_AC_BARRING_FACTOR_P75,
  RRM_MIF_AC_BARRING_FACTOR_P80,
  RRM_MIF_AC_BARRING_FACTOR_P85,
  RRM_MIF_AC_BARRING_FACTOR_P90,
  RRM_MIF_AC_BARRING_FACTOR_P95
}rrm_mif_ac_barring_factor_et;

typedef enum
{
  RRM_MIF_AC_BARRING_TIME_S4,
  RRM_MIF_AC_BARRING_TIME_S8,
  RRM_MIF_AC_BARRING_TIME_S16,
  RRM_MIF_AC_BARRING_TIME_S32,
  RRM_MIF_AC_BARRING_TIME_S64,
  RRM_MIF_AC_BARRING_TIME_S128,
  RRM_MIF_AC_BARRING_TIME_S256,
  RRM_MIF_AC_BARRING_TIME_S512
}rrm_mif_ac_barring_time_et;

/*TM mode 7-8 start*/
typedef enum
{
    RRM_LOW_LEVEL,
    RRM_HIGH_LEVEL
}rrm_level_t;
/*TM mode 7-8 end*/

/* Carrier_Aggregation_Start */
typedef enum
{
    RRM_SCELL_RF2,
    RRM_SCELL_RF4,
    RRM_SCELL_RF8,
    RRM_SCELL_RF16,
    RRM_SCELL_RF32,
    RRM_SCELL_RF64,
    RRM_SCELL_RF128,
    RRM_SCELL_RFINFINITY
}rrm_scell_deactivation_timer_et;

/* Carrier_Aggregation_End */
/*CA*/
/*SPR_17047_START*/
typedef enum
{
  RRM_SET_INVALID,
  RRM_SET,
  RRM_UNSET
}rrm_meas_gap_status_et;
/*SPR_17047_END*/

#endif /* _RRM_TYPES_H_ */
