/**************************************************************************
*
*   FILE NAME:
*       rrc_mac_intf.h
*
*   DESCRIPTION:
*       This file contains types used for representation of MAC API inside RRC.
*       Based on LTE_MAC_Rel_1.2_API_Manual_Rev_2.4.doc and
*       eNB_RRC_R1_0_API_v0_1.doc.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   23 Apr 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_MAC_INTF_H_
#define _RRC_MAC_INTF_H_

#include "rrc_defines.h"

#include "rrc_rrm_intf.h"

#define RRC_MAC_MAX_CREATE_LC       10
#define RRC_MAC_MAX_RECONFIGURE_LC  10
#define RRC_MAC_MAX_DELETE_LC       10

typedef enum
{
    MAC_FAILURE,
    MAC_SUCCESS,
    MAC_PARTIAL_SUCCESS
} rrc_mac_return_et;

typedef enum 
{
    MAC_DELETE_LC_ACTIVE = 1,
    MAC_DELETE_LC_INACTIVE = 2
}rrc_mac_delete_lc_status_et;

typedef enum
{
    MAC_DELETE_LC_DOWNLINK = 1,
    MAC_DELETE_LC_UPLINK = 2,
    MAC_DELETE_LC_BOTH = 3
} rrc_mac_delete_lc_types_et;

typedef enum
{
    MAC_SFN_DOES_NOT_EXIST = 4,
    MAC_MAX_CELLS_CONFIGURED = 5
} rrc_mac_config_cell_error_code_et;

typedef enum
{
    MAC_LOG_CH_EXISTS = 0,
    MAC_LOG_CH_NOT_EXISTS = 1
} rrc_mac_logical_channel_error_code_et;

typedef enum
{
    RRC_MAC_MAX_UL_HARQ_TX_N1 = 0,
    RRC_MAC_MAX_UL_HARQ_TX_N2 = 1,
    RRC_MAC_MAX_UL_HARQ_TX_N3 = 2,
    RRC_MAC_MAX_UL_HARQ_TX_N4 = 3,
    RRC_MAC_MAX_UL_HARQ_TX_N5 = 4,
    RRC_MAC_MAX_UL_HARQ_TX_N6 = 5,
    RRC_MAC_MAX_UL_HARQ_TX_N7 = 6,
    RRC_MAC_MAX_UL_HARQ_TX_N8 = 7,
    RRC_MAC_MAX_UL_HARQ_TX_N10 = 8,
    RRC_MAC_MAX_UL_HARQ_TX_N12 = 9,
    RRC_MAC_MAX_UL_HARQ_TX_N16 = 10,
    RRC_MAC_MAX_UL_HARQ_TX_N20 = 11,
    RRC_MAC_MAX_UL_HARQ_TX_N24 = 12,
    RRC_MAC_MAX_UL_HARQ_TX_N28 = 13
} rrc_mac_max_ul_harq_tx_et;

typedef enum
{
    RRC_MAC_SF_10 = 0,
    RRC_MAC_SF_20 = 1,
    RRC_MAC_SF_32 = 2,
    RRC_MAC_SF_40 = 3,
    RRC_MAC_SF_64 = 4,
    RRC_MAC_SF_80 = 5,
    RRC_MAC_SF_128 = 6,
    RRC_MAC_SF_160 = 7,
    RRC_MAC_SF_256 = 8,
    RRC_MAC_SF_320 = 9,
    RRC_MAC_SF_512 = 10,
    RRC_MAC_SF_640 = 11,
    RRC_MAC_SF_1024 = 12,
    RRC_MAC_SF_1280 = 13,
    RRC_MAC_SF_2048 = 14,
    RRC_MAC_SF_2560 = 15
} rrc_mac_sf_et;

typedef enum
{
    RRC_MAC_NB_FOUR_T = 0,
    RRC_MAC_NB_TWO_T = 1,
    RRC_MAC_NB_ONE_T = 2,
    RRC_MAC_NB_HALF_T = 3,
    RRC_MAC_NB_QUARTER_T = 4,
    RRC_MAC_NB_ONE_EIGHTH_T = 5,
    RRC_MAC_NB_ONE_SIXTEENTH_T = 6,
    RRC_MAC_NB_ONE_THIRTY_SECOND_T = 7
}rrc_mac_nb_et;

typedef enum
{
    RRC_MAC_OUT_OF_SYNC_INITIAL = 0,
    RRC_MAC_IN_SYNC_AFTER_OUT_OF_SYNC = 1,
    RRC_MAC_OUT_OF_SYNC_MAX_RETRIES = 2,
    RRC_MAC_IN_SYNC_HO = 3
}rrc_ue_sync_status_et;

typedef enum
{
    RRC_MAC_NGAP_1 = 0,
    RRC_MAC_NGAP_2 = 1
} rrc_mac_n_gap_et;

typedef enum
{
    NEW_AREA,
    CHANGE_AREA,
    NO_CHANGE
}m2ap_request_type_et;

#pragma pack(push, 1)

/******************************************************************************
*   MAC Cell messages
******************************************************************************/

/******************************************************************************
*   RRC_MAC_CONFIG_CELL_REQ
******************************************************************************/

/*** TDD changes start ***/
typedef struct _rrc_mac_tdd_ack_nack_feedback_mode_t
{
    U8   tdd_ack_nack_feedback_mode;
    /*^ M, 0, H, 0, 1 ^*/
}rrc_mac_tdd_ack_nack_feedback_mode_t;
/*** TDD changes stop ***/
/*SPR_19066_START*/
/* Code Removed */
/*SPR_19066_END*/
typedef struct _rrc_mac_pucch_config_info_t
{
    U8   delta_pucch_shift;      /*^ M, 0, B, 1, 3 ^*/
    U8   nrb_cqi;                /*^ M, 0, H, 0, 98 ^*/
    U8   ncs_an;                 /*^ M, 0, H, 0, 7 ^*/
    U16  n1pucch_an;             /*^ M, 0, H, 0, 2047 ^*/
} rrc_mac_pucch_config_info_t;

typedef struct _rrc_mac_phich_config_info_t
{
    U8   phich_duration;             /*^ M, 0, H, 0, 1 ^*/
    U8   phich_resource;             /*^ M, 0, H, 0, 3 ^*/
} rrc_mac_phich_config_info_t;

typedef struct _rrc_mac_preamble_group_a_config_t
{
    U8   num_of_preambles_in_group_a;      /*^ M, 0, B, 4, 60 ^*/
    U16  message_size_group_a;             /*^ M, 0, B, 56, 256 ^*/
}rrc_mac_preamble_group_a_config_t;



/* eMTC changes start */
#define RRC_MAC_RACH_CONFIGURATION_R13_PRESENT             0x02
/* eMTC changes stop */
typedef struct _rrc_mac_rach_config_info_t
{
#define RRC_MAC_PREAMBLE_GROUP_A_CONFIG_PRESENT            0x01

    rrc_bitmask_t           optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    
    U8  ra_response_window_size;                /*^ M, 0, B, 2, 10 ^*/
    U8  mac_contention_resolution_timer;        /*^ M, 0, B, 8, 64 ^*/
    U8  max_harq_msg3tx;                        /*^ M, 0, B, 1, 8 ^*/
    U8  prach_configuration_index;              /*^ M, 0, H, 0, 63 ^*/
    U8  prach_frequency_offset;                 /*^ M, 0, H, 0, 94 ^*/
    U8  num_of_contention_based_preambles;      /*^ M, 0, B, 4, 64 ^*/

    /* CLPC Start */
    U8 preamble_initial_received_target_power; /*^ M, 0, H, 0, 15 ^*/
    /* CLPC Stop */

    rrc_mac_preamble_group_a_config_t preamble_group_a_config;
   /*^ TLV, RRC_MAC_PREAMBLE_GROUP_A_CONFIG, RRC_MAC_PREAMBLE_GROUP_A_CONFIG_PRESENT ^*/

    /* eMTC changes start */
    RrcMacRachConfigurationR13_t    rachConfigurationR13;
    /*^ TLV, RRC_MAC_RACH_CONFIGURATION_R13, RRC_MAC_RACH_CONFIGURATION_R13_PRESENT ^*/
    /* eMTC changes stop */
} rrc_mac_rach_config_info_t;

/**** TDD changes start *****/
typedef struct _rrc_mac_tdd_config_t
{
    U16      sub_frame_assignment;
    /*^ M, 0, H, 0, 6 ^*/  /* rrm_sub_frame_assignment_et */
    U16      special_sub_frame_pattern;
    /*^ M, 0, H, 0 , 8 ^*/  /* rrm_special_sub_frame_pattern_et */
} rrc_mac_tdd_config_t;


/**** TDD changes stop *****/

/* SRS Start */

typedef struct _rrc_mac_srs_max_uppts_t
{
    U8    max_uppts;    /*^ M, 0, H, 0, 0 ^*/
}rrc_mac_srs_max_uppts_t;




typedef struct _rrc_mac_srs_common_setup_info_t
{
#define RRC_MAC_SRS_MAX_UPPTS_PRESENT      0x01

    rrc_bitmask_t                   optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8    srs_bandwidth_config;             /*^ M, 0, H, 0, 7 ^*/
    U8    srs_subframe_config;              /*^ M, 0, H, 0, 15 ^*/
    U8    ack_nack_srs_simultaneous_trans;  /*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */
    rrc_mac_srs_max_uppts_t    srs_max_uppts;
    /*^ TLV, RRC_MAC_SRS_MAX_UPPTS, RRC_MAC_SRS_MAX_UPPTS_PRESENT ^*/

}rrc_mac_srs_common_setup_info_t;


typedef struct _rrc_mac_srs_common_config_t
{
#define RRC_MAC_SRS_COMMON_SETUP_INFO_PRESENT            0x01


    rrc_bitmask_t                   optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8   config_type;    /*^ M, 0, H, 0, 1 ^*/
     
    rrc_mac_srs_common_setup_info_t   srs_common_setup_info;
 /*^ TLV, RRC_MAC_SRS_COMMON_SETUP_INFO, RRC_MAC_SRS_COMMON_SETUP_INFO_PRESENT ^*/


}rrc_mac_srs_common_config_t;

/* SRS Stop */

typedef struct _rrc_mac_freq_selective_scheduling_t
{
    U8   ul_freq_selective_enable;    /*^ M, 0, H, 0, 1 ^*/
    U8   dl_freq_selective_enable;    /*^ M, 0, H, 0, 1 ^*/
}rrc_mac_freq_selective_scheduling_t;

typedef struct _rrc_mac_contn_free_rach_timer_info_t
{
/* SPR 13559 Fix Start */
    U16   contn_free_rach_timer; /*^ M, 0, B, 25, 200 ^*/
/* SPR 13559 Fix Stop */
}rrc_mac_contn_free_rach_timer_info_t;


/******************************************************************************
*   RRC_MAC_EXPLICIT_CELL_START_REQUIRED
******************************************************************************/
typedef struct _rrc_mac_explicit_cell_start_required_t
{
    U8   explicit_start_required;   /*^ M, 0, H, 0, 0 ^*/
}rrc_mac_explicit_cell_start_required_t;

/* CLPC Start */
typedef struct _rrc_mac_pdsch_config_common_info_t
{
    S8 reference_signal_power;    /*^ M, 0, B, -60, 50 ^*/
    U8 p_b;                       /*^ M, 0, H, 0, 3 ^*/

}rrc_mac_pdsch_config_common_info_t;

typedef struct _rrc_mac_uplink_power_control_common_t
{
    S8 p0_nominal_pusch;    /*^ M, 0, B, -126, 24 ^*/ 
    U8 alpha;               /*^ M, 0, H, 0, 7 ^*/

}rrc_mac_uplink_power_control_common_t;

/* CLPC Stop */

typedef struct _rrc_mac_rbs_for_dci_1a_t
{
    U8  num_rb_buffer[RBS_FOR_DCI_1A_SIZE];
    /*^ M, 0, OCTET_STRING, FIXED, H, 0, 100 ^*/
}rrc_mac_rbs_for_dci_1a_t;

typedef struct _rrc_mac_rbs_for_dci_1c_t
{
    U8  num_rb_buffer[RBS_FOR_DCI_1C_SIZE];
    /*^ M, 0, OCTET_STRING, FIXED, H, 0, 100 ^*/
}rrc_mac_rbs_for_dci_1c_t;

typedef struct _rrc_mac_dci_format_for_si_t
{
    U8  dci_format_for_si_msgs;
    /*^ M, 0, H, 0, 1 ^*/ /* rrc_dci_format_for_si_msgs_et */
}rrc_mac_dci_format_for_si_t;

typedef struct _rrc_mac_ul_sync_loss_timer_info_t
{
    U16 ul_sync_loss_timer_value;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_mac_ul_sync_loss_timer_info_t;

typedef struct _rrc_mac_pucch_cqi_sinr_threshold_info_t
{
    U8  sinr_value;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_mac_pucch_cqi_sinr_threshold_info_t;

typedef struct _rrc_mac_pusch_config_common_info_t
{
    U8  enable_64_qam; /*^ M, 0, H, 0, 1 ^*/  /* rrc_bool_et */ 
} rrc_mac_pusch_config_common_info_t; 

typedef struct _rrc_mac_n_gap_t
{
    U8   n_gap;
/*^ M, 0, H, 0, 1 ^*/   /* rrc_mac_n_gap_et */
} rrc_mac_n_gap_t;

/* PDCCH Start */
typedef struct _rrc_mac_dynamic_pdcch_info_t
{
    U8 isDynamicPdcchDisabled;   /*^ M, 0, H, 0, 1 ^*/
}rrc_mac_dynamic_pdcch_info_t;
/* PDCCH Stop */

/* SPS Start */
typedef struct _rrc_mac_sps_crnti_range_t
{
    U16 start_sps_crnti; /*^ M, 0, B, 61, 65523 ^*/
    U16 end_sps_crnti;   /*^ M, 0, B, 61, 65523 ^*/
}rrc_mac_sps_crnti_range_t;

/* SPS Stop */

/* PRS + */
typedef struct _rrc_mac_prs_bandwidth_info_t
{
        U8  prs_bandwidth;          /*^ M, 0, B, 6, 100 ^*/
}rrc_mac_prs_bandwidth_info_t;

typedef struct _rrc_mac_prs_subframes_info_t
{
        U8  prs_subframes;          /*^ M, 0, B, 1, 6 ^*/
}rrc_mac_prs_subframes_info_t;

typedef struct _rrc_mac_prs_config_index_info_t
{
        U16 prs_config_index;       /*^ M, 0, H, 0, 4095 ^*/
}rrc_mac_prs_config_index_info_t;

typedef struct _rrc_mac_prs_transmission_power_info_t
{
        U16 prs_transmission_power; /*^ M, 0, H, 0, 10000 ^*/
}rrc_mac_prs_transmission_power_info_t;

typedef struct _rrc_mac_prs_muting_config_info_t
{
        U8  bits_to_be_read;        /*^ M, 0, B, 2, 16 ^*/
    /*SPR_17020_START*/
        U16 prs_muting_config_index;/*^ M, 0, N, 0, 65535 ^*/
    /*SPR_17020_END*/
}rrc_mac_prs_muting_config_info_t;

typedef struct _rrc_mac_prs_config_info_t
{
    rrc_bitmask_t                   optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_MAC_CONFIG_PRS_BANDWIDTH_INFO_PRESENT	       0x01
#define RRC_MAC_CONFIG_PRS_SUBFRAMES_INFO_PRESENT              0x02
#define RRC_MAC_CONFIG_PRS_CONFIG_INDEX_INFO_PRESENT           0x04
#define RRC_MAC_CONFIG_PRS_TRANSMISSION_POWER_INFO_PRESENT     0x08
#define RRC_MAC_CONFIG_PRS_MUTING_CONFIG_INFO_PRESENT          0x10

    U8  request_type;           /*^ M, 0, H, 0, 1 ^*/

    rrc_mac_prs_bandwidth_info_t    prs_bandwidth_info;
    /*^ TLV, RRC_MAC_PRS_BANDWIDTH_INFO, RRC_MAC_CONFIG_PRS_BANDWIDTH_INFO_PRESENT ^*/

    rrc_mac_prs_subframes_info_t    prs_subframes_info;
    /*^ TLV, RRC_MAC_PRS_SUBFRAMES_INFO, RRC_MAC_CONFIG_PRS_SUBFRAMES_INFO_PRESENT ^*/

    rrc_mac_prs_config_index_info_t prs_config_index_info;
    /*^ TLV, RRC_MAC_PRS_CONFIG_INDEX_INFO, RRC_MAC_CONFIG_PRS_CONFIG_INDEX_INFO_PRESENT ^*/

    rrc_mac_prs_transmission_power_info_t    prs_transmission_power_info;
    /*^ TLV, RRC_MAC_PRS_TRANSMISSION_POWER_INFO, RRC_MAC_CONFIG_PRS_TRANSMISSION_POWER_INFO_PRESENT ^*/

    rrc_mac_prs_muting_config_info_t    prs_muting_config_info;
    /*^ TLV, RRC_MAC_PRS_MUTING_CONFIG_INFO, RRC_MAC_CONFIG_PRS_MUTING_CONFIG_INFO_PRESENT ^*/
}rrc_mac_prs_config_info_t;
/* PRS -*/

typedef struct _m2ap_mac_embms_transmission_power_info_t
{
    U16           embms_transmission_power;/*^ M, 0, H, 0, 10000  ^*/

}m2ap_mac_embms_transmission_power_info_t;

typedef struct _rrc_m2ap_mac_embms_params_info_t 
{
#define RRC_M2AP_MBMS_TRANSMISSION_POWER_INFO_PRESENT 0x01
    U8                              optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    m2ap_mac_embms_transmission_power_info_t      m2ap_mac_embms_transmission_power_info;
    /*^ TLV, RRC_M2AP_MAC_EMBMS_TRANSMISSION_POWER_INFO,RRC_M2AP_MBMS_TRANSMISSION_POWER_INFO_PRESENT ^*/

}rrc_m2ap_mac_embms_params_info_t;

typedef struct _rrc_m2ap_embms_sib2_info_t
{
    U8   no_of_csa_pattern_counter;
    /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/
    m2ap_mac_csa_pattern_list_t         csa_pattern_list[MAX_CSA_PATTERN];
    /*^ TLV, SEQUENCE, RRC_M2AP_MAC_CSA_PATTERN_LIST ^*/
}rrc_m2ap_embms_sib2_info_t;

typedef struct _rrc_m2ap_mac_embms_sib13_info_t
{
    U8        si_index;/*^ M, 0, B, 1, 74 ^*/
}rrc_m2ap_mac_embms_sib13_info_t;

typedef struct _m2ap_embms_notification_config_t
{
    U8           notification_rept_coeff;/*^ M, 0, B, 2,4 ^*/
    U8           notification_offset; /*^ M, 0, H, 0,10 ^*/
    U8           notification_sf_index; /*^ M, 0, H, 0,6 ^*/
}m2ap_embms_notification_config_t;

typedef struct _m2ap_mac_mcch_config_t
{
    U8           mcch_rep_period; /*^ M, 0, H, 0, 3  ^*/ /*mcch_repetition_period_et*/
    U8           mcch_offset;     /*^ M, 0, H, 0, 10 ^*/
    U8           mcch_mod_period; /*^ M, 0, H, 0, 1 ^*/ /*mcch_modification_period_et */
    U8           sf_alloc_info;   /*^ M, 0, N, 0, 0 ^*/
    U8           signalling_mcs;  /*^ M, 0, H, 0, 3 ^*/ /*signaling_mcs_et */
}m2ap_mac_mcch_config_t;

#define RRC_M2AP_MAC_MCCH_CONFIG_PRESENT  0x01
typedef struct _m2ap_mbms_create_area_info_t
{
    U8                         optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    U8                         area_index; /*^ M, 0, H, 0, 15 ^*/
    U8                         area_id;  /*^ M, 0, N, 0, 0 ^*/
    U8                         area_type;  /*^ M, 0, B, 1, 2 ^*/
    U8                         non_mbsfn_region_len; /*^ M, 0, B, 1, 2 ^*/
    U8                         request_type;
    /*^ M, 0, H, 0, 2 ^*/  /* m2ap_request_type_et */
    U8                         notification_ind;   /*^ M, 0, H, 0, 7 ^*/
    m2ap_mac_mcch_config_t     mcch_config;
    /*^ TLV,RRC_M2AP_MAC_MCCH_CONFIG, RRC_M2AP_MAC_MCCH_CONFIG_PRESENT ^*/
}m2ap_mbms_create_area_info_t;


typedef struct _rrc_m2ap_mac_create_embms_area_entity_info_t
{
#define RRC_M2AP_MAC_NOTIFICATION_CONFIG_PRESENT 0x01
    U8                                 optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    U8                                 num_of_area;/*^ M, 0, H, 1, 8 ^*/
    m2ap_embms_notification_config_t   m2ap_embms_notification_config;
    /*^ TLV, RRC_M2AP_MAC_NOTIFICATION_CONFIG,RRC_M2AP_MAC_NOTIFICATION_CONFIG_PRESENT ^*/
    U8                                 num_create_area_info_counter;
    /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/
    m2ap_mbms_create_area_info_t       m2ap_mbms_create_area_info[MAX_MBSFN_AREA_PER_CELL];
    /*^ TLV, SEQUENCE, RRC_M2AP_MBMS_CREATE_AREA_INFO ^*/
}rrc_m2ap_mac_create_embms_area_entity_info_t;

typedef struct _rrc_m2ap_mac_delete_embms_area_entity_info_t
{
    U8                num_area_delete_count;
    /*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/
    U8                area_index[MAX_MBSFN_AREA_PER_CELL];
   /*^ M, 0, OCTET_STRING,LIMITED_TILL_THE_END ^*/

}rrc_m2ap_mac_delete_embms_area_entity_info_t;

#define RRC_MAC_RACH_CONFIG_INFO_PRESENT        0x01
#define RRC_MAC_PHICH_CONFIG_INFO_PRESENT       0x02
#define RRC_MAC_PUCCH_CONFIG_INFO_PRESENT       0x04
#define RRC_MAC_SIBTYPE1_MSG_REQ_PRESENT        0x08
#define RRC_MAC_SI_MSG_REQ_PRESENT              0x10
/**** TDD changes start *****/
#define RRC_MAC_TDD_CONFIG_PRESENT              0x20
/**** TDD changes stop *****/
#define RRC_MAC_SRS_COMMON_CONFIG_PRESENT       0x40
#define RRC_MAC_FREQ_SELECTIVE_ENABLE_PRESENT   0x80
/*SPR_19066_START*/
/* RRC_MAC_UE_INACTIVE_TIMER_CONFIG_VAL_PRESENT has been deprecated */
/*SPR_19066_END*/
#define RRC_MAC_CONTN_FREE_RACH_TIMER_INFO_PRESENT 0x200

/* CLPC Start */
#define RRC_MAC_PDSCH_CONFIG_COMMON_INFO_PRESENT        0x400
#define RRC_MAC_DOWNLINK_POWER_CONTROL_COMMON_PRESENT   0x800
#define RRC_MAC_UPLINK_POWER_CONTROL_COMMON_PRESENT     0x1000
#define RRC_MAC_TPC_RNTI_RANGE_PRESENT                  0x2000
#define RRC_MAC_POWER_CONTROL_ENABLE_PRESENT            0x4000
/* CLPC Stop */
#define RRC_MAC_EXPLICIT_START_REQUIRED_PRESENT         0x8000
#define RRC_MAC_RBS_PER_TBS_DCI_1A_PRESENT              0x10000
#define RRC_MAC_RBS_PER_TBS_DCI_1C_PRESENT              0x20000

#define RRC_MAC_UL_SYNC_LOSS_TIMER_INFO_PRESENT         0x40000
#define RRC_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO_PRESENT   0x80000
#define RRC_MAC_PUSCH_CONFIG_COMMON_PRESENT             0x100000
#define RRC_MAC_N_GAP_PRESENT                           0x200000
#define RRC_MAC_SPS_CRNTI_RANGE_PRESENT                 0x400000
#define RRC_MAC_DYNAMIC_PDCCH_INFO_PRESENT              0x800000
/* PRS +*/
#define RRC_MAC_PRS_CONFIG_INFO_PRESENT                 0x1000000
/* PRS -*/
#define RRC_MAC_LAA_SCELL_CONFIG_INFO_PRESENT           0x2000000
#define RRC_MAC_DMTC_CONFIG_INFO_PRESENT                0x4000000
#define RRC_CONFIG_SIB13_INFO_PRESENT                   0x8000000
#define RRC_M2AP_CONFIG_MBMS_AREA_CREATE_PRESENT        0x10000000
#define RRC_M2AP_CONFIG_MBMS_AREA_DELETE_PRESENT        0x20000000
#define RRC_M2AP_CONFIG_EMBMS_CONFIG_INFO_PRESENT       0x40000000
#define RRC_CONFIG_EMBMS_SIB2_INFO_PRESENT              0x80000000

/*eMTC Changes start*/
#define RRC_MAC_SIBTYPE1_BR_MSG_INFO_PRESENT        	0x100000000
#define RRC_MAC_SI_MSG_BR_INFO_PRESENT              	0x200000000
#define RRC_MAC_PUCCH_CONFIG_COMMON_V1310_PRESENT		0x400000000
#define RRC_MAC_PDSCH_CONFIG_COMMON_V1310_PRESENT		0x800000000
#define RRC_MAC_PUSCH_CONFIG_COMMON_V1310_PRESENT		0x1000000000
#define RRC_MAC_PRACH_CONFIG_V1310_PRESENT              0x2000000000 
#define RRC_MAC_MODIFICATION_PERIOD_V1310_PRESENT       0x4000000000
#define RRC_MAC_PCCH_CONFIG_V1310_PRESENT               0x8000000000
#define RRC_MAC_FREQUANCY_HOPPING_PARAMETERS_R13_PRESENT  0x10000000000
#define RRC_MAC_EMTC_DATA_CONFIG_PRESENT                  0x20000000000
/*eMTC Changes stop*/

typedef struct _rrc_mac_config_cell_req_t
{
    U64  optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

/*** TDD changes start ***/
    U8   duplexing_mode;
/*** TDD changes stop ***/

    U8   dl_res_blocks;
/*^ M, 0, B, 1, 100 ^*/

    U8   ul_res_blocks;
/*^ M, 0, B, 1, 100 ^*/

    U8   max_harq_retrans;
/*^ M, 0, B, 1, 8 ^*/

    U8   num_of_tx_antennas;
/*^ M, 0, B, 1, 4 ^*/

    U8   ul_cyclic_lenth_prefix;
/*^ M, 0, B, 1, 2 ^*/

    U8   start_ra_rnti_range;
/*^ M, 0, B, 1, 60 ^*/

    U8   end_ra_rnti_range;
/*^ M, 0, B, 1, 60 ^*/


    U32   mod_period;
    /*^ M, 0, B, 64, 4096 ^*/

    U8   sfn_gap;
    /*^ M, 0, B, 1, 32 ^*/
    
    U8   nb;
    /*^ M, 0, H, 0, 7 ^*/  /* rrc_mac_nb_et */

    /* SPR 19309 + */
    U8                   cfi_value;         /*^ M, 0, B, 1, 4 ^*/
    /* SPR 19309 - */
    /* SPR 19310 + */
    U16                   num_of_eul;        /*^ M, 0, N, 0, 0 ^*/
    U16                  size_of_eul;       /*^ M, 0, N, 0, 0 ^*/
    /* SPR 19310 - */

    U8   dci_format_for_si_msgs;
    /*^ M, 0, H, 0, 1 ^*/  /* rrc_dci_format_for_si_msgs_et  */
   
    /**** TDD changes start *****/

    rrc_mac_tdd_config_t        tdd_config;
    /*^ TLV, RRC_MAC_TDD_CONFIG, RRC_MAC_TDD_CONFIG_PRESENT ^*/

    /**** TDD changes stop *****/

    rrc_mac_rach_config_info_t  rach_config_info;
/*^ TLV, RRC_MAC_RACH_CONFIG_INFO, RRC_MAC_RACH_CONFIG_INFO_PRESENT ^*/

    rrc_mac_phich_config_info_t phich_config_info;
/*^ TLV, RRC_MAC_PHICH_CONFIG_INFO, RRC_MAC_PHICH_CONFIG_INFO_PRESENT ^*/

    rrc_mac_pucch_config_info_t pucch_config_info;
/*^ TLV, RRC_MAC_PUCCH_CONFIG_INFO, RRC_MAC_PUCCH_CONFIG_INFO_PRESENT ^*/

    rrc_mac_rbs_for_dci_1a_t    num_rbs_per_tbs_dci_1a;
    /*^ TLV, RRC_MAC_RBS_FOR_DCI_1A, RRC_MAC_RBS_PER_TBS_DCI_1A_PRESENT ^*/

    rrc_mac_rbs_for_dci_1c_t    num_rbs_per_tbs_dci_1c;
    /*^ TLV, RRC_MAC_RBS_FOR_DCI_1C, RRC_MAC_RBS_PER_TBS_DCI_1C_PRESENT ^*/

    rrc_counter_t                mib_msg_req_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_mib_msg_req_t        mib_msg_req[RRC_MIB_MSGS];
/*^ TLV, SEQUENCE, RRC_MAC_MIB_MSG_REQ ^*/

    rrc_mac_sibtype1_msg_req_t   sib1_msg_req;
/*^ TLV, RRC_MAC_SIBTYPE1_MSG_REQ, RRC_MAC_SIBTYPE1_MSG_REQ_PRESENT ^*/

    rrc_mac_si_msg_req_t         si_msg_req;
/*^ TLV, RRC_MAC_SI_MSG_INFO, RRC_MAC_SI_MSG_REQ_PRESENT ^*/

    /* SRS Start */
    rrc_mac_srs_common_config_t  srs_common_config;
    /*^ TLV, RRC_MAC_SRS_COMMON_CONFIG, RRC_MAC_SRS_COMMON_CONFIG_PRESENT ^*/
    /* SRS Stop */


    /* CLPC Start */
    rrc_mac_pdsch_config_common_info_t pdsch_config_common_info;
    /*^ TLV, RRC_MAC_PDSCH_CONFIG_COMMON_INFO, RRC_MAC_PDSCH_CONFIG_COMMON_INFO_PRESENT ^*/

    rrc_mac_downlink_power_control_common_t downlink_power_control_common;
    /*^ TLV, RRC_MAC_DOWNLINK_POWER_CONTROL_COMMON, RRC_MAC_DOWNLINK_POWER_CONTROL_COMMON_PRESENT ^*/

    rrc_mac_uplink_power_control_common_t uplink_power_control_common;
    /*^ TLV, RRC_MAC_UPLINK_POWER_CONTROL_COMMON, RRC_MAC_UPLINK_POWER_CONTROL_COMMON_PRESENT ^*/

    /* CLPC Stop */

     rrc_mac_pusch_config_common_info_t          pusch_config_common_info;
    /*^ TLV, RRC_MAC_PUSCH_CONFIG_COMMON, RRC_MAC_PUSCH_CONFIG_COMMON_PRESENT ^*/

     /*SPR_19066_START*/
     /* Code Removed */
     /*SPR_19066_END*/

    rrc_mac_freq_selective_scheduling_t          freq_selective_scheduling;
    /*^ TLV, RRC_MAC_FREQ_SELECTIVE_ENABLE, RRC_MAC_FREQ_SELECTIVE_ENABLE_PRESENT ^*/

    rrc_mac_contn_free_rach_timer_info_t         contn_free_rach_timer_info;
    /*^ TLV, RRC_MAC_CONTN_FREE_RACH_TIMER_INFO,  RRC_MAC_CONTN_FREE_RACH_TIMER_INFO_PRESENT ^*/

    /* CLPC Start */

    rrc_mac_tpc_rnti_range_t tpc_rnti_range;
    /*^ TLV, RRC_MAC_TPC_RNTI_RANGE, RRC_MAC_TPC_RNTI_RANGE_PRESENT ^*/

    rrc_mac_power_control_enable_t power_control_enable;
    /*^ TLV, RRC_MAC_POWER_CONTROL_ENABLE, RRC_MAC_POWER_CONTROL_ENABLE_PRESENT ^*/

    /* CLPC Stop */

     rrc_mac_explicit_cell_start_required_t        explicit_cell_start_required;
    /*^ TLV, RRC_MAC_EXPLICIT_CELL_START_REQUIRED,  RRC_MAC_EXPLICIT_START_REQUIRED_PRESENT ^*/

     rrc_mac_ul_sync_loss_timer_info_t        ul_sync_loss_timer_info;
    /*^ TLV, RRC_MAC_UL_SYNC_LOSS_TIMER_INFO, RRC_MAC_UL_SYNC_LOSS_TIMER_INFO_PRESENT ^*/

     rrc_mac_pucch_cqi_sinr_threshold_info_t  pucch_cqi_sinr_threshold_info;
    /*^ TLV, RRC_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO, RRC_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO_PRESENT ^*/
    
     rrc_mac_n_gap_t  n_gap;
     /*^ TLV, RRC_MAC_N_GAP, RRC_MAC_N_GAP_PRESENT ^*/

     /* SPS Start */
     rrc_mac_sps_crnti_range_t                     sps_crnti_range;
    /*^ TLV, RRC_MAC_SPS_CRNTI_RANGE,  RRC_MAC_SPS_CRNTI_RANGE_PRESENT ^*/
     /* SPS Stop */

     /* PDCCH Start */
     rrc_mac_dynamic_pdcch_info_t                  dynamic_pdcch_info;
    /*^ TLV, RRC_MAC_DYNAMIC_PDCCH_INFO,  RRC_MAC_DYNAMIC_PDCCH_INFO_PRESENT ^*/
     /* PDCCH Stop */

     /* PRS +*/
     rrc_mac_prs_config_info_t                     prs_config_info;
     /*^ TLV, RRC_MAC_PRS_CONFIG_INFO, RRC_MAC_PRS_CONFIG_INFO_PRESENT ^*/
     /* PRS -*/
     /* EMBMS SIB2 +*/
     rrc_m2ap_embms_sib2_info_t                    rrc_m2ap_embms_sib2_info;
     /*^ TLV, RRC_M2AP_MAC_EMBMS_SIB2_INFO, RRC_CONFIG_EMBMS_SIB2_INFO_PRESENT ^*/
     /* EMBMS SIB2 -*/
     /* SIB13 +*/
     rrc_m2ap_mac_embms_sib13_info_t               rrc_m2ap_mac_sib13_info;
     /*^ TLV, RRC_M2AP_MAC_SIB13_INFO, RRC_CONFIG_SIB13_INFO_PRESENT ^*/
     /* MBMS Area Create  +*/
     rrc_m2ap_mac_create_embms_area_entity_info_t
         rrc_m2ap_mac_create_embms_area_entity_info;      
     /*^ TLV, RRC_M2AP_MAC_CREATE_MBMS_AREA_ENTITY_INFO, RRC_M2AP_CONFIG_MBMS_AREA_CREATE_PRESENT ^*/
     /* MBMS Area Create  -*/
     /* MBMS Area Delete  +*/
     rrc_m2ap_mac_delete_embms_area_entity_info_t
         rrc_m2ap_mac_delete_embms_area_entity_info;      
     /*^ TLV, RRC_M2AP_MAC_DELETE_MBMS_AREA_ENTITY_INFO,RRC_M2AP_CONFIG_MBMS_AREA_DELETE_PRESENT ^*/
     /* MBMS Area Delete  -*/
     rrc_m2ap_mac_embms_params_info_t               rrc_m2ap_mac_embms_params_info;
     /*^ TLV, RRC_M2AP_MAC_EMBMS_CONFIG, RRC_M2AP_CONFIG_EMBMS_CONFIG_INFO_PRESENT ^*/

     /*  LAA Rel13 Code + */
     rrc_radio_res_dedicated_laa_scell_config_t laa_scell_configuration_r13;
     /*^ TLV, RRC_MAC_LAA_SCELL_CONFIG_INFO, RRC_MAC_LAA_SCELL_CONFIG_INFO_PRESENT ^*/
     rrc_dmtc_configuration_r13_t dmtc_configuration_r13;
     /*^ TLV, RRC_MAC_DMTC_CONFIG_INFO, RRC_MAC_DMTC_CONFIG_INFO_PRESENT ^*/
     /*  LAA Rel13 Code  - */
     /* eMTC Changes start */
     RrcEmtcDataConfig_t                      emtcDataConfig;
     /*^ TLV, RRC_MAC_EMTC_DATA_CONFIG, RRC_MAC_EMTC_DATA_CONFIG_PRESENT ^*/
 
     RrcMacPrachConfigV1310_t	              prachConfigV1310;
     /*^ TLV, RRC_MAC_PRACH_CONFIG_V1310, RRC_MAC_PRACH_CONFIG_V1310_PRESENT ^*/

     RrcMacSibtype1BrMsgInfo_t                sibType1BrMsgInfo;
     /*^ TLV, RRC_MAC_SIBTYPE1_BR_MSG_INFO, RRC_MAC_SIBTYPE1_BR_MSG_INFO_PRESENT ^*/

     RrcMacSiMsgBrInfo_t                      siMsgBrInfo;
     /*^ TLV, RRC_MAC_SI_MSG_BR_INFO, RRC_MAC_SI_MSG_BR_INFO_PRESENT ^*/

     RrcMacPucchConfigCommonV1310_t           pucchConfigCommonV1310;
     /*^ TLV, RRC_MAC_PUCCH_CONFIG_COMMON_V1310, RRC_MAC_PUCCH_CONFIG_COMMON_V1310_PRESENT ^*/

     RrcMacModificationPeriodV1310_t          modificationPeriodV1310;
     /*^ TLV, RRC_MAC_MODIFICATION_PERIOD_V1310, RRC_MAC_MODIFICATION_PERIOD_V1310_PRESENT ^*/

     RrcMacPcchConfigV1310_t                  pcchConfigV1310;
     /*^ TLV, RRC_MAC_PCCH_CONFIG_V1310, RRC_MAC_PCCH_CONFIG_V1310_PRESENT ^*/

     RrcMacPdschConfigCommonV1310_t           pdschConfigCommonV1310;
     /*^ TLV, RRC_MAC_PDSCH_CONFIG_COMMON_V1310, RRC_MAC_PDSCH_CONFIG_COMMON_V1310_PRESENT ^*/

     RrcMacPuschConfigCommonV1310_t           puschConfigCommonV1310;
     /*^ TLV, RRC_MAC_PUSCH_CONFIG_COMMON_V1310, RRC_MAC_PUSCH_CONFIG_COMMON_V1310_PRESENT ^*/

     RrcFrequancyHoppingParametersR13_t       frequancyHoppingParametersR13;
     /*^ TLV, RRC_MAC_FREQUANCY_HOPPING_PARAMETERS_R13, RRC_MAC_FREQUANCY_HOPPING_PARAMETERS_R13_PRESENT ^*/

     /* eMTC Changes stop */
} rrc_mac_config_cell_req_t; /*^ API, RRC_MAC_CONFIG_CELL_REQ ^*/

/******************************************************************************
*   RRC_MAC_CONFIG_CELL_CNF
******************************************************************************/
typedef struct _rrc_mac_config_cell_error_code_t
{
    U8   error_code;
/*^ M, 0, H, 0, 0 ^*/   /* rrc_mac_config_cell_error_code_et */

} rrc_mac_config_cell_error_code_t;

#define RRC_MAC_CONFIG_CELL_ERROR_CODE_PRESENT  0x01
#define RRC_MAC_CONFIG_CELL_EARFCN_LIST_PRESENT     0x02

typedef struct _rrc_mac_config_cell_cnf_t
{
    rrc_bitmask_t   optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_response_t                      response;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_mac_return_et */

    rrc_mac_config_cell_error_code_t    config_cell_error_code;
/*^ TLV, RRC_MAC_CONFIG_CELL_ERROR_CODE, RRC_MAC_CONFIG_CELL_ERROR_CODE_PRESENT ^*/

	rrc_dl_earfcn_t     dl_earfcn_list;
/*^ TLV, RRC_MAC_LAA_CELL_CONFIG_EARFCN, RRC_MAC_CONFIG_CELL_EARFCN_LIST_PRESENT ^*/
} rrc_mac_config_cell_cnf_t; /*^ API, RRC_MAC_CONFIG_CELL_CNF ^*/

/******************************************************************************
*   RRC_MAC_SFN_REQ
******************************************************************************/
typedef struct _rrc_mac_timestamp_reqd_t
{
    U8   timestamp_reqd;  /*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */
} rrc_mac_timestamp_reqd_t; 

/*CDMA2000_CSFB_HO start*/
#define RRC_MAC_TIMESTAMP_REQD_INFO_PRESENT      0x01
typedef struct _rrc_mac_sfn_req_t
{
    rrc_bitmask_t           optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_timestamp_reqd_t   timestamp_reqd;
/*^ TLV, RRC_MAC_TIMESTAMP_REQD_INFO, RRC_MAC_TIMESTAMP_REQD_INFO_PRESENT ^*/
/*CDMA2000_CSFB_HO stop*/
} rrc_mac_sfn_req_t; /*^ API, RRC_MAC_SFN_REQ ^*/

/******************************************************************************
*   RRC_MAC_SFN_CNF
******************************************************************************/


typedef struct _rrc_mac_sfn_t
{
    rrc_sfn_t   sfn;    /*^ M, 0, H, 0, 1023 ^*/
} rrc_mac_sfn_t; 

/*CDMA2000_CSFB_HO start*/
typedef struct _rrc_mac_sf_t
{
    rrc_sf_t   sf;    /*^ M, 0, H, 0, 9 ^*/
} rrc_mac_sf_t; 

/*CDMA2000_CSFB_HO stop*/

#define RRC_MAC_SFN_INFO_PRESENT     0x01

/*CDMA2000_CSFB_HO start*/
#define RRC_MAC_SF_INFO_PRESENT      0x02
#define RRC_MAC_TIMESTAMP_INFO_PRESENT     0x04
/*CDMA2000_CSFB_HO stop*/

typedef struct _rrc_mac_sfn_cnf_t
{
    rrc_bitmask_t           optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_response_t          response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_mac_return_et */

    rrc_mac_sfn_t   sfn;
/*^ TLV, RRC_MAC_SFN_INFO, RRC_MAC_SFN_INFO_PRESENT ^*/

/*CDMA2000_CSFB_HO start*/
    rrc_mac_sf_t    sf;
/*^ TLV, RRC_MAC_SF_INFO, RRC_MAC_SF_INFO_PRESENT ^*/

    rrc_timestamp_t   timestamp;
/*^ TLV, RRC_MAC_TIMESTAMP_INFO, RRC_MAC_TIMESTAMP_INFO_PRESENT ^*/
/*CDMA2000_CSFB_HO stop*/

} rrc_mac_sfn_cnf_t; /*^ API, RRC_MAC_SFN_CNF ^*/

/******************************************************************************
*   RRC_MAC_SFN_IND
******************************************************************************/
typedef struct _rrc_mac_sfn_ind_t
{
    rrc_sfn_t   sfn;    /*^ M, 0, H, 0, 1023 ^*/
} rrc_mac_sfn_ind_t; /*^ API, RRC_MAC_SFN_IND ^*/

/******************************************************************************
*   RRC_MAC_RECONFIG_CELL_REQ
******************************************************************************/
typedef struct _rrc_mac_reconfig_cell_req_t
{
 #define RRC_RECONFIG_MAC_MIB_MSG_REQ_PRESENT             0x01
 #define RRC_RECONFIG_MAC_SIB_TYPE_1_MSG_PRESENT          0x02
 #define RRC_RECONFIG_MAC_SI_MSG_REQ_PRESENT              0x04
 #define RRC_RECONFIG_MAC_MOD_PERIOD_PRESENT              0x08
 #define RRC_RECONFIG_MAC_SFN_GAP_PRESENT                 0x10
/*SPR_19066_START*/
/* RRC_RECONFIG_MAC_UE_INACTIVE_TIME_CONFIG_VAL_PRESENT has been deprecated */
/*SPR_19066_END*/
 #define RRC_RECONFIG_MAC_CONTN_FREE_RACH_TIMER_INFO_PRESENT 0x40

 /* CLPC Start */
 #define RRC_RECONFIG_MAC_PDSCH_CONFIG_COMMON_INFO_PRESENT        0x80
 #define RRC_RECONFIG_MAC_DOWNLINK_POWER_CONTROL_COMMON_PRESENT   0x100
 #define RRC_RECONFIG_MAC_UPLINK_POWER_CONTROL_COMMON_PRESENT     0x200
 /* CLPC Stop */

 #define RRC_RECONFIG_MAC_DCI_FORMAT_FOR_SI_PRESENT               0x400
 #define RRC_RECONFIG_MAC_RBS_PER_TBS_DCI_1A_PRESENT              0x800
 #define RRC_RECONFIG_MAC_RBS_PER_TBS_DCI_1C_PRESENT              0x1000

 #define RRC_RECONFIG_MAC_UL_SYNC_LOSS_TIMER_INFO_PRESENT         0x2000
 #define RRC_RECONFIG_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO_PRESENT   0x4000
 
 #define RRC_RECONFIG_MAC_PUSCH_CONFIG_COMMON_INFO_PRESENT        0x8000
 #define RRC_RECONFIG_MAC_N_GAP_PRESENT                           0x10000
/* SPS Start */
#define RRC_RECONFIG_MAC_SPS_CRNTI_RANGE_PRESENT                  0x20000
/* SPS Stop */
/* PRS +*/
#define RRC_RECONFIG_MAC_PRS_INFO_PRESENT                         0x40000
/* PRS -*/
/* EMBMS SIB2 +*/                                                   
#define RRC_RECONFIG_EMBMS_SIB2_INFO_PRESENT                      0x80000
/* EMBMS SIB2 -*/
/* SIB 13 +*/
#define RRC_RECONFIG_SIB13_INFO_PRESENT                           0x100000
/* SIB 13 -*/
/* MBMS AREA CREATE +*/
#define RRC_M2AP_MBMS_AREA_CREATE_PRESENT                         0x200000 
/* MBMS AREA CREATE -*/
/* MBMS AREA DELETE +*/
#define RRC_M2AP_MBMS_AREA_DELETE_PRESENT                         0x400000 
/* MBMS AREA DELETE -*/
/* EMBMS CONFIG +*/
#define RRC_M2AP_EMBMS_CONFIG_INFO_PRESENT                        0x800000 
/* MBMS CONFIG -*/
/*eMTC Changes start*/
#define RRC_RECONFIG_MAC_SIBTYPE1_BR_MSG_INFO_PRESENT        	  0x1000000
#define RRC_RECONFIG_MAC_SI_MSG_BR_INFO_PRESENT              	  0x2000000
#define RRC_RECONFIG_MAC_PUCCH_CONFIG_COMMON_V1310_PRESENT		  0x4000000
#define RRC_RECONFIG_MAC_PDSCH_CONFIG_COMMON_V1310_PRESENT		  0x8000000
#define RRC_RECONFIG_MAC_PUSCH_CONFIG_COMMON_V1310_PRESENT		  0x10000000
#define RRC_RECONFIG_MAC_MODIFICATION_PERIOD_V1310_PRESENT        0x20000000
#define RRC_RECONFIG_MAC_FREQUANCY_HOPPING_PARAMETERS_R13_PRESENT 0x40000000
#define RRC_RECONFIG_MAC_SFN_GAP_V1310_PRESENT                    0x80000000
#define RRC_RECONFIG_MAC_SFN_EMTC_PRESENT                         0x100000000
/*eMTC Changes stop*/

    U64              optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_sfn_t   sfn;    /*^ M, 0, H, 0, 1023 ^*/

    rrc_sf_t    sf; /*^M, O, H, 0, 9 ^*/
    
    rrc_mac_dci_format_for_si_t  dci_format_for_si;
    /*^ TLV, RRC_MAC_DCI_FORMAT_FOR_SI, RRC_RECONFIG_MAC_DCI_FORMAT_FOR_SI_PRESENT ^*/

    rrc_mac_rbs_for_dci_1a_t     num_rbs_per_tbs_dci_1a;
    /*^ TLV, RRC_MAC_RBS_FOR_DCI_1A, RRC_RECONFIG_MAC_RBS_PER_TBS_DCI_1A_PRESENT ^*/

    rrc_mac_rbs_for_dci_1c_t     num_rbs_per_tbs_dci_1c;
    /*^ TLV, RRC_MAC_RBS_FOR_DCI_1C, RRC_RECONFIG_MAC_RBS_PER_TBS_DCI_1C_PRESENT ^*/

    rrc_counter_t               mib_msg_req_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    /*eMTC Changes start*/ 
    rrc_mac_mib_msg_req_t       mib_msg_req[RRC_MIB_MSGS];
    /*^ TLV, SEQUENCE, RRC_MAC_MIB_MSG_REQ ^*/
    /*eMTC Changes stop*/

    rrc_mac_sibtype1_msg_req_t  sib1_msg_req;
/*^ TLV, RRC_MAC_SIBTYPE1_MSG_REQ, RRC_RECONFIG_MAC_SIB_TYPE_1_MSG_PRESENT ^*/

    rrc_mac_si_msg_req_t        si_msg_req;
/*^ TLV, RRC_MAC_SI_MSG_INFO, RRC_RECONFIG_MAC_SI_MSG_REQ_PRESENT ^*/

    rrc_mac_mod_period_info_t   mod_period_info;
/*^ TLV, RRC_MAC_MOD_PERIOD_INFO, RRC_RECONFIG_MAC_MOD_PERIOD_PRESENT ^*/

    rrc_mac_sfn_gap_info_t      sfn_gap_info;
/*^ TLV, RRC_MAC_SFN_GAP_INFO, RRC_RECONFIG_MAC_SFN_GAP_PRESENT ^*/

    /*SPR_19066_START*/
    /* Code Removed */
    /*SPR_19066_END*/

    rrc_mac_contn_free_rach_timer_info_t             contn_free_rach_timer_info;
/*^ TLV, RRC_MAC_CONTN_FREE_RACH_TIMER_INFO,  RRC_RECONFIG_MAC_CONTN_FREE_RACH_TIMER_INFO_PRESENT ^*/

    /* CLPC Start */
    rrc_mac_pdsch_config_common_info_t pdsch_config_common_info;
    /*^ TLV, RRC_MAC_PDSCH_CONFIG_COMMON_INFO, RRC_RECONFIG_MAC_PDSCH_CONFIG_COMMON_INFO_PRESENT ^*/

    rrc_mac_downlink_power_control_common_t downlink_power_control_common;
    /*^ TLV, RRC_MAC_DOWNLINK_POWER_CONTROL_COMMON, RRC_RECONFIG_MAC_DOWNLINK_POWER_CONTROL_COMMON_PRESENT ^*/

    rrc_mac_uplink_power_control_common_t uplink_power_control_common_t;
    /*^ TLV, RRC_MAC_UPLINK_POWER_CONTROL_COMMON, RRC_RECONFIG_MAC_UPLINK_POWER_CONTROL_COMMON_PRESENT ^*/

    /* CLPC Stop */

    rrc_mac_ul_sync_loss_timer_info_t         ul_sync_loss_timer_info;
    /*^ TLV, RRC_MAC_UL_SYNC_LOSS_TIMER_INFO, RRC_RECONFIG_MAC_UL_SYNC_LOSS_TIMER_INFO_PRESENT ^*/

    rrc_mac_pucch_cqi_sinr_threshold_info_t   pucch_cqi_sinr_threshold_info;
    /*^ TLV, RRC_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO, RRC_RECONFIG_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO_PRESENT ^*/

     rrc_mac_pusch_config_common_info_t   pusch_config_common_info;
    /*^ TLV, RRC_MAC_PUSCH_CONFIG_COMMON, RRC_RECONFIG_MAC_PUSCH_CONFIG_COMMON_INFO_PRESENT ^*/
     
     rrc_mac_n_gap_t  n_gap;
     /*^ TLV, RRC_MAC_N_GAP, RRC_RECONFIG_MAC_N_GAP_PRESENT ^*/
     /* SPS Start */
     rrc_mac_sps_crnti_range_t                     sps_crnti_range;
    /*^ TLV, RRC_MAC_SPS_CRNTI_RANGE,  RRC_RECONFIG_MAC_SPS_CRNTI_RANGE_PRESENT ^*/
     /* PRS +*/
     rrc_mac_prs_config_info_t                     prs_reconfig_info;
     /*^ TLV, RRC_MAC_PRS_CONFIG_INFO, RRC_RECONFIG_MAC_PRS_INFO_PRESENT ^*/
     /* PRS -*/
     /* EMBMS SIB2 +*/
     rrc_m2ap_embms_sib2_info_t                    rrc_m2ap_embms_sib2_info;
     /*^ TLV, RRC_M2AP_MAC_EMBMS_SIB2_INFO, RRC_RECONFIG_EMBMS_SIB2_INFO_PRESENT ^*/
     /* EMBMS SIB2 -*/
     /* SIB13 +*/
     rrc_m2ap_mac_embms_sib13_info_t               rrc_m2ap_mac_sib13_info;
     /*^ TLV, RRC_M2AP_MAC_SIB13_INFO, RRC_RECONFIG_SIB13_INFO_PRESENT ^*/
     /* MBMS Area Create  +*/
     rrc_m2ap_mac_create_embms_area_entity_info_t
         rrc_m2ap_mac_create_embms_area_entity_info;      
     /*^ TLV, RRC_M2AP_MAC_CREATE_MBMS_AREA_ENTITY_INFO, RRC_M2AP_MBMS_AREA_CREATE_PRESENT ^*/
     /* MBMS Area Create  -*/
     /* MBMS Area Delete  +*/
     rrc_m2ap_mac_delete_embms_area_entity_info_t
         rrc_m2ap_mac_delete_embms_area_entity_info;      
     /*^ TLV, RRC_M2AP_MAC_DELETE_MBMS_AREA_ENTITY_INFO,RRC_M2AP_MBMS_AREA_DELETE_PRESENT ^*/
     /* MBMS Area Delete  -*/
      rrc_m2ap_mac_embms_params_info_t             rrc_m2ap_mac_embms_params_info;
     /*^ TLV, RRC_M2AP_MAC_EMBMS_CONFIG, RRC_M2AP_EMBMS_CONFIG_INFO_PRESENT ^*/
     /* eMTC Changes start */ 
     RrcMacEmtcSfn_t                          emtcSfn;
     /*^ TLV, RRC_MAC_SFN_EMTC, RRC_RECONFIG_MAC_SFN_EMTC_PRESENT ^*/

     RrcMacSibtype1BrMsgInfo_t                sibType1BrMsgInfo;
     /*^ TLV, RRC_MAC_SIBTYPE1_BR_MSG_INFO, RRC_RECONFIG_MAC_SIBTYPE1_BR_MSG_INFO_PRESENT ^*/

     RrcMacSiMsgBrInfo_t                      siMsgBrInfo;
     /*^ TLV, RRC_MAC_SI_MSG_BR_INFO, RRC_RECONFIG_MAC_SI_MSG_BR_INFO_PRESENT ^*/

     RrcMacPucchConfigCommonV1310_t           pucchConfigCommonV1310;
     /*^ TLV, RRC_MAC_PUCCH_CONFIG_COMMON_V1310, RRC_RECONFIG_MAC_PUCCH_CONFIG_COMMON_V1310_PRESENT ^*/

     RrcMacModificationPeriodV1310_t          modificationPeriodV1310;
     /*^ TLV, RRC_MAC_MODIFICATION_PERIOD_V1310, RRC_RECONFIG_MAC_MODIFICATION_PERIOD_V1310_PRESENT ^*/

     RrcMacPdschConfigCommonV1310_t           pdschConfigCommonV1310;
     /*^ TLV, RRC_MAC_PDSCH_CONFIG_COMMON_V1310, RRC_RECONFIG_MAC_PDSCH_CONFIG_COMMON_V1310_PRESENT ^*/

     RrcMacPuschConfigCommonV1310_t           puschConfigCommonV1310;
     /*^ TLV, RRC_MAC_PUSCH_CONFIG_COMMON_V1310, RRC_RECONFIG_MAC_PUSCH_CONFIG_COMMON_V1310_PRESENT ^*/

     RrcFrequancyHoppingParametersR13_t       frequancyHoppingParametersR13;
     /*^ TLV, RRC_MAC_FREQUANCY_HOPPING_PARAMETERS_R13, RRC_RECONFIG_MAC_FREQUANCY_HOPPING_PARAMETERS_R13_PRESENT ^*/

     RrcMacSfnGapInfoV1310_t                  sfnGapInfoV1310;
     /*^ TLV, RRC_MAC_SFN_GAP_INFO_V1310, RRC_RECONFIG_MAC_SFN_GAP_V1310_PRESENT ^*/

     /* eMTC Changes stop */


} rrc_mac_reconfig_cell_req_t; /*^ API, RRC_MAC_RECONFIG_CELL_REQ ^*/

/******************************************************************************
*   RRC_MAC_RECONFIG_CELL_CNF
******************************************************************************/
typedef struct _rrc_mac_reconfig_cell_cnf_t
{
    rrc_response_t   response;   /*^ M, 0, H, 0, 1 ^*/ /* rrc_mac_return_et */
} rrc_mac_reconfig_cell_cnf_t; /*^ API, RRC_MAC_RECONFIG_CELL_CNF ^*/

/******************************************************************************
*   MAC UE messages
******************************************************************************/

/******************************************************************************
*   RRC_MAC_CREATE_UE_ENTITY_REQ
******************************************************************************/
/* SRS Start */
typedef struct _rrc_mac_srs_dedicated_setup_info_t
{
    U8   srs_bandwidth;    /*^ M, 0, H, 0, 3 ^*/

    U8   srs_hopping_bandwidth;   /*^ M, 0, H, 0, 3 ^*/

    U8   freq_domain_position;    /*^ M, 0, H, 0, 23 ^*/

    U8   duration;    /*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */

    U16  srs_config_index;    /*^ M, 0, H, 0, 1023 ^*/

    U8   transmission_comb;    /*^ M, 0, H, 0, 1 ^*/

    U8   cyclic_shift;        /*^ M, 0, H, 0, 7 ^*/


}rrc_mac_srs_dedicated_setup_info_t;


typedef struct _rrc_mac_srs_dedicated_config_t
{
#define RRC_MAC_SRS_DEDICATED_SETUP_INFO_PRESENT  0x01

    rrc_bitmask_t                       optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                                   config_type;    /*^ M, 0, H, 0, 1 ^*/

    rrc_mac_srs_dedicated_setup_info_t   srs_dedicated_setup_info;
    /*^ TLV, RRC_MAC_SRS_DEDICATED_SETUP_INFO, RRC_MAC_SRS_DEDICATED_SETUP_INFO_PRESENT ^*/

}rrc_mac_srs_dedicated_config_t;

/* SRS Stop */

#define CBSR_MAX_VALUE 8

typedef struct _rrc_mac_codebook_subset_restriction_t
{
    U8   cbsr_type;    /*^ M, 0, H, 0, 7 ^*/ /* codebook_subset_restriction_type_et */

    U8   cbsr_value[CBSR_MAX_VALUE]; /*^ M, O, OCTET_STRING, FIXED ^*/

}rrc_mac_codebook_subset_restriction_t;

/* Cr 316And247 IEs  Mac Changes */
#define CBSR_MAX_VALUE_V920 4

typedef struct _rrc_mac_codebook_subset_restriction_v920_t
{
    U8        cbsr_type;    /*^ M, 0, H, 0, 1 ^*/ /* codebook_subset_restriction_v920_type_et */
    U8         cbsr_value[CBSR_MAX_VALUE_V920]; /*^ M, O, OCTET_STRING, FIXED ^*/
}rrc_mac_codebook_subset_restriction_v920_t;

typedef struct
{
  U8     short_drx_cycle;  /*^ M, 0, H, 1, 15 ^*/ 
  U8     short_drx_cycle_timer; /*^ M, 0, B, 1, 16 ^*/
} rrc_mac_short_drx_config_t;

/* DRX_MAC_CE_CMD  Start */
typedef struct
{
  U16     drx_mac_ce_timer;   /*^ M, 0, B, 3, 2559 ^*/  
  U16     drx_mac_ce_cmd_threshold;  /*^ M, 0, B, 1, 2559 ^*/
}rrc_mac_cmd_l2_trigger_info_t;
/* DRX_MAC_CE_CMD  End */
typedef struct
{
    rrc_bitmask_t               bitmask;  
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_MAC_DRX_SETUP_CONFIG_SHORT_DRX_CONFIG_PRESENT            0x01
/* DRX_MAC_CE_CMD  Start */
#define RRC_MAC_DRX_SETUP_CONFIG_DRX_MAC_CE_CMD_TRIGGER_INFO_PRESENT 0x02
/* DRX_MAC_CE_CMD  End */

    U8   on_duration_timer;
/*^ M, 0, H, 1, 15 ^*/   

    U8   drx_inactivity_timer;
/*^ M, 0, H, 1, 22 ^*/  

    U8   drx_retransmission_timer;
/*^ M, 0, H, 1, 7 ^*/  

    U8   long_drx_cycle;
/*^ M, 0, H, 0, 15 ^*/ /* rrc_mac_sf_et */

    U16  drx_start_offset;

    rrc_mac_short_drx_config_t  short_drx;
/*^ TLV, RRC_MAC_SHORT_DRX_CONFIG, RRC_MAC_DRX_SETUP_CONFIG_SHORT_DRX_CONFIG_PRESENT ^*/
    
    /* DRX_MAC_CE_CMD  Start */
    rrc_mac_cmd_l2_trigger_info_t  mac_ce_cmd_l2_trigger_info;
/*^ TLV, RRC_MAC_DRX_MAC_CE_CMD_TRIGGER_INFO, RRC_MAC_DRX_SETUP_CONFIG_DRX_MAC_CE_CMD_TRIGGER_INFO_PRESENT ^*/
    /* DRX_MAC_CE_CMD  End */
} rrc_mac_drx_setup_config_info_t;

#define RRC_MAC_DRX_CONFIG_DRX_SETUP_CONFIG_INFO_PRESENT     0x01
typedef struct 
{
    rrc_bitmask_t                     bitmask;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    U8                                drx_config_type;  
  /*^ M, 0, H, 0 , 1 ^*/ /* 0 - for release, 1 - for setup */
    rrc_mac_drx_setup_config_info_t   drx_setup_config_info;
/*^  TLV, RRC_MAC_DRX_SETUP_CONFIG_INFO, RRC_MAC_DRX_CONFIG_DRX_SETUP_CONFIG_INFO_PRESENT ^*/ 
} rrc_mac_drx_config_info_t;

typedef struct
{
    U8   gap_pattern_id;
/*^ M, 0, H, 0, 11 ^*/
    U8   gap_offset;
/*^ M, 0, H, 0, 159 ^*/
} rrc_mac_meas_gap_setup_config_info_t;

typedef struct _rrc_mac_time_alignment_timer_info_t
{
    U16   time_alignment_timer; /*^ M, 0, H, 0, 7 ^*/
}rrc_mac_time_alignment_timer_info_t;

#define RRC_MAC_MEAS_GAP_CONFIG_MEAS_GAP_SETUP_CONFIG_INFO_PRESENT 0x01
typedef struct
{
    rrc_bitmask_t                          bitmask;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    U8                                      meas_gap_config_type;
/*^ M, 0, H, 0 , 1 ^*/ /* 0 - for release, 1 - for setup */
    rrc_mac_meas_gap_setup_config_info_t   meas_gap_setup_config_info;
/*^ TLV, RRC_MAC_MEAS_GAP_SETUP_CONFIG_INFO, RRC_MAC_MEAS_GAP_CONFIG_MEAS_GAP_SETUP_CONFIG_INFO_PRESENT  ^*/
} rrc_mac_meas_gap_config_info_t;

/* Rel9 Upgrade Changes Start */
typedef struct
{
    U8        sr_prohibit_timer;  /*^ M, 0, H, 0 , 7 ^*/
}rrc_mac_sr_prohbit_timer_value_v920_t;

/* Rel9 Upgrade Changes Stop */

/* CLPC Start */
typedef struct _rrc_mac_uplink_power_control_dedicated_t
{
    S8 p0_ue_pusch;               /*^ M, 0, B, -8, 7 ^*/
    U8 delta_mcs_enabled;         /*^ M, 0, H, 0 , 1 ^*/
    U8 accumulation_enabled;      /*^ M, 0, H, 0 , 1 ^*/
}rrc_mac_uplink_power_control_dedicated_t;

typedef struct _rrc_mac_tpc_rnti_info_t
{
    U16 tpcRnti;                 /*^ M, 0, B, 1 , 65523 ^*/
    U8 tpcIndex;                 /*^ M, 0, B, 1, 31 ^*/
    U8 isConfiguredForDci3Or3A;  /*^ M, 0, N, 0 , 0 ^*/

}rrc_mac_tpc_rnti_info_t;

#define RRC_MAC_TPC_RNTI_INFO_PRESENT 0x01

typedef struct _rrc_mac_tpc_pdcch_config_pucch_t
{
    rrc_bitmask_t       bitmask;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8 release;                  /*^ M, 0, H, 0 , 1 ^*/
    
    rrc_mac_tpc_rnti_info_t     tpc_rnti_info;
/*^ TLV, RRC_MAC_TPC_RNTI_INFO, RRC_MAC_TPC_RNTI_INFO_PRESENT  ^*/

}rrc_mac_tpc_pdcch_config_pucch_t;

typedef struct _rrc_mac_tpc_pdcch_config_pusch_t
{
    rrc_bitmask_t       bitmask;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8 release;                  /*^ M, 0, H, 0 , 1 ^*/
    
    rrc_mac_tpc_rnti_info_t     tpc_rnti_info;
/*^ TLV, RRC_MAC_TPC_RNTI_INFO, RRC_MAC_TPC_RNTI_INFO_PRESENT  ^*/

}rrc_mac_tpc_pdcch_config_pusch_t;

/* CLPC Stop */

typedef struct _rrc_mac_ambr_qos_info_t
{
    U32         dl_ambr; /*^ M, 0, N, 0, 0 ^*/
    U32         ul_ambr; /*^ M, 0, N, 0, 0 ^*/
} rrc_mac_ambr_qos_info_t;

typedef struct _rrc_mac_user_location_type_t
{
    U8 user_location_type;  /*^ M, 0, H, 0, 1 ^*/
}rrc_mac_user_location_type_t;
/* HD-FDD start */
typedef struct _rrc_mac_rf_params_t
{
    U8 halfDuplexFdd;          /*^ M, 0, H, 0, 1 ^*/
}rrc_mac_rf_params_t;
/* HD-FDD stop */

typedef struct _rrc_mac_ue_capability_additional_params_t
{
    U8 rat_1_support;  /*^ M, 0, H, 0, 1 ^*/
}rrc_mac_ue_capability_additional_params_t;

/* SPS Start */

typedef struct
{
    U16   n1_pucch_an_persist;
    /*^ M, 0, H, 0, 2047 ^*/
}rrc_mac_n1_pucch_an_persist_list_t;

typedef struct _rrc_mac_max_sps_harq_retx_t
{
    U8    max_sps_harq_retx;
    /*^ M, 0, H, 1, 8 ^*/
}rrc_mac_max_sps_harq_retx_t;

typedef struct _rrc_mac_explicit_release_after_t
{
    U8 explicit_release_afte;
    /*^ M, 0, H, 2, 8 ^*/
}rrc_mac_explicit_release_after_t;

typedef struct _rrc_mac_sps_dl_setup_info_t
{
    rrc_bitmask_t               bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_MAC_MAX_SPS_HARQ_RETX_PRESENT    0x01
#define RRC_MAC_RRC_MAC_EXPLICIT_RELEASE_AFTER_PRESENT    0x02

    U8                              semi_persist_sched_interval_dl;
    /*^ M, 0, H, 0, 9 ^*/  /* rrc_semi_persist_sched_interval_et */

    U8                              number_of_conf_sps_processes;       /*^ M, 0, B, 1, 8 ^*/

    rrc_counter_t           n1_pucch_an_persist_list_counter;
    /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_n1_pucch_an_persist_list_t n1_pucch_an_persist[MAX_N1_PUCCH_AN_PERSIST_SIZE];
    /*^ TLV, SEQUENCE, RRC_MAC_SPS_N1_PUCCH_AN_PERSIST_LIST ^*/

    rrc_mac_max_sps_harq_retx_t    max_sps_harq_retx;
    /*^ TLV, RRC_MAC_MAX_SPS_HARQ_RETX, RRC_MAC_MAX_SPS_HARQ_RETX_PRESENT ^*/

    rrc_mac_explicit_release_after_t    explicit_release_after;
    /*^ TLV, RRC_MAC_EXPLICIT_RELEASE_AFTER, RRC_MAC_RRC_MAC_EXPLICIT_RELEASE_AFTER_PRESENT ^*/


}rrc_mac_sps_dl_setup_info_t;

typedef struct _rrc_mac_sps_dl_info_t
{
    rrc_bitmask_t               bitmask;    
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_MAC_SPS_DL_SETUP_INFO_PRESENT     0x01

    U8                         request_type; /* 0 - release and 1 - setup */
    /*^ M, 0, H, 0, 1 ^*/

    rrc_mac_sps_dl_setup_info_t   sps_dl_setup_info;
    /*^ TLV, RRC_MAC_SPS_DL_SETUP_INFO, RRC_MAC_SPS_DL_SETUP_INFO_PRESENT ^*/

}rrc_mac_sps_dl_info_t;


typedef struct _rrc_mac_p_zero_persistent_t
{
    S8   p_zero_nominal_pusch_persistent;    /*^ M, 0, B, -126, 24 ^*/
    S8   p_zero_ue_pusch_persistent;         /*^ M, 0, B, -8, 7 ^*/
}rrc_mac_p_zero_persistent_t;

typedef struct _rrc_mac_two_intervals_config_t
{
      /* SPR:5994 start */
      U8          two_intervals_config;
      /*^ M, 0, B, 1, 1 ^*/
      /* SPR:5994 stop */
}rrc_mac_two_intervals_config_t;

typedef struct _rrc_mac_sps_ul_setup_info_t
{
    rrc_bitmask_t         bitmask;               
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_MAC_P_ZERO_PERSISTENT_PRESENT                0x01
#define RRC_MAC_TWO_INTERVALS_CONFIG_PRESENT    0x02

    U8          semi_persist_sched_interval_ul;
    /*^ M, 0, H, 0, 9 ^*/  /* rrc_semi_persist_sched_interval_et */

    U8          implicit_release_after;
    /*^ M, 0, H, 0, 3 ^*/  /* rrc_implicit_release_after_et */

    rrc_mac_p_zero_persistent_t   p_zero_persistent;
    /*^ TLV, RRC_MAC_P_ZERO_PERSISTENT, RRC_MAC_P_ZERO_PERSISTENT_PRESENT ^*/

    rrc_mac_two_intervals_config_t          two_intervals_config;
    /*^ TLV, RRC_MAC_TWO_INTERVALS_CONFIG, RRC_MAC_TWO_INTERVALS_CONFIG_PRESENT ^*/

}rrc_mac_sps_ul_setup_info_t;

typedef struct _rrc_mac_sps_ul_info_t
{
    rrc_bitmask_t               bitmask;    
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_MAC_SPS_UL_SETUP_INFO_PRESENT     0x01

    U8                         request_type;
    /*^ M, 0, H, 0, 1 ^*/ /* 0 - release and 1 - setup */

    rrc_mac_sps_ul_setup_info_t   sps_ul_setup_info;
    /*^ TLV, RRC_MAC_SPS_UL_SETUP_INFO, RRC_MAC_SPS_UL_SETUP_INFO_PRESENT ^*/



}rrc_mac_sps_ul_info_t;

typedef struct _rrc_mac_sps_config_t
{
    U16               bitmask;           
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_MAC_SPS_DL_INFO_PRESENT      0x01
#define RRC_MAC_SPS_UL_INFO_PRESENT      0x02

    rrc_mac_sps_dl_info_t       sps_dl_info;
    /*^ TLV, RRC_MAC_SPS_DL_INFO , RRC_MAC_SPS_DL_INFO_PRESENT ^*/

    rrc_mac_sps_ul_info_t       sps_ul_info;
    /*^ TLV, RRC_MAC_SPS_UL_INFO, RRC_MAC_SPS_UL_INFO_PRESENT ^*/

}rrc_mac_sps_config_t;
/* SPS Stop */

typedef struct _rrc_mac_ue_cat_t
{
    U8   ue_category;                 /*^ M, 0, B, 1, 5 ^*/
}rrc_mac_ue_cat_t;

/* UL_MU_MIMO Start */
typedef struct _rrc_mac_ul_mu_mimo_info_t
{
    U8   ul_mu_mimo_status;           /*^ M, 0,H, 0, 1 ^*/
}rrc_mac_ul_mu_mimo_info_t;
/* UL_MU_MIMO Stop */

/* min_ue_power start */
typedef struct _rrc_mac_phr_timer_info_t
{
    U8   periodicPHRTimer;           /*^ M, 0,H, 0, 7 ^*/
    U8   prohibitPHRTimer;           /*^ M, 0,H, 0, 7 ^*/
}rrc_mac_phr_timer_info_t;

#define RRC_MAC_PHR_TIMER_INFO_PRESENT   0x01
typedef struct _rrc_mac_phr_timer_config_t
{
    rrc_bitmask_t   optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
/* 0-Release, 1-Setup */
    U8   phrTimerConfigType;         /*^ M, 0,H, 0, 1 ^*/ 
    rrc_mac_phr_timer_info_t  phrTimerInfo;
    /*^ TLV, RRC_MAC_PHR_TIMER_INFO, RRC_MAC_PHR_TIMER_INFO_PRESENT ^*/
}rrc_mac_phr_timer_config_t;

typedef struct _rrc_mac_min_ue_power_t
{
    S8   minUePower;                /*^ M, 0,B, -40, 23 ^*/ 
}rrc_mac_min_ue_power_t;
/* min_ue_power stop */
/* 36321_CR0538 start */
typedef struct _rrc_mac_ue_compliance_info_t
{
    /* eMTC changes start */
    U8   ue_compliance_release;                 /*^ M, 0, H, 0, 5 ^*/
    /* eMTC changes stop */
}rrc_mac_ue_compliance_info_t;

/* Transmission_mode_7_8 start*/
typedef struct _rrc_mac_pdsch_epre_to_ue_rs_ratio_t
{
    S8   pdsch_epre_to_ue_rs_ratio;              /*^ M, 0, B, -6, 3 ^*/
}rrc_mac_pdsch_epre_to_ue_rs_ratio_t;
/* Transmission_mode_7_8 stop*/

/*TTI_BUNDLING Start*/
typedef struct _rrc_mac_tti_bundling_info_t
{
    U8   tti_bundling;              /*^ M, 0, H, 0, 1 ^*/
}rrc_mac_tti_bundling_info_t;
/*TTI_BUNDLING Stop*/

/* SPR 23209 FIXED START */
typedef struct _rrc_mac_operator_info_t
{
    U8   operator_id;              /*^ M, 0, H, 0, MAX_MOCN_OPERATOR_ID ^*/
}rrc_mac_operator_info_t;
/* SPR 23209 FIXED END */



/* Dynamic_ue_reconfig*/
typedef struct _rrc_mac_max_ul_harq_tx_info_t
{
 U8  max_ul_harq_tx;	/*^ M, 0, H, 0, 13 ^*/
}rrc_mac_max_ul_harq_tx_info_t;

typedef struct _rrc_mac_pc_max_config_info_t
{
 S8 pc_max;	/*^ M, 0, B, -40, 23 ^*/
}rrc_mac_pc_max_config_info_t;

typedef struct _rrc_mac_code_book_index_info_t
{
    U8    code_book_index;                /*^ M, 0, H, 0, 3 ^*/
} rrc_mac_code_book_index_info_t;

typedef struct _rrc_mac_num_of_layer_info_t
{
    U8    num_of_layer;                   /*^ M, 0, B, 1, 4 ^*/
} rrc_mac_num_of_layer_info_t;

typedef struct _rrc_mac_tx_mode_info_t
{
    U8  transmission_mode;              /*^ M, 0, B, 1, 8 ^*/ /* CR_316And247_START */
} rrc_mac_tx_mode_info_t;

typedef struct _rrc_mac_pdsch_config_info_t
{
    U8 p_a;          /*^ M, 0, H, 0, 7 ^*/
}rrc_mac_pdsch_config_info_t;


typedef struct _rrc_mac_scell_info_t
{
#define RRC_MAC_MIMO_CAPABILITY_DL_V1020_PRESENT             0x01
#define RRC_MAC_SCELL_TX_MODE_INFO_PRESENT                   0x02
#define RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT    0x04
#define RRC_MAC_SCELL_PDSCH_CONFIG_INFO_PRESENT              0x08
#define RRC_MAC_SCELL_CQI_INFO_PRESENT                       0x10
#define RRC_MAC_DL_NUM_HARQ_PROC_PRESENT                     0x20
#define RRC_MAC_DL_INIT_MCS_PRESENT                          0x40
#define RRC_MAC_SCELL_NUM_OF_LAYER_INFO_PRESENT              0x80
#define RRC_MAC_SCELL_CODE_BOOK_INDEX_INFO_PRESENT          0x100
#define RRC_MAC_SCELL_USER_LOCATION_TYPE_PRESENT            0x200
#define RRC_MAC_SCELL_RF_PARAMS_PRESENT                     0x400
/*CA start */
#define RRC_MAC_SCELL_ACTIVATE_STATUS_PRESENT               0x800
/*CA end*/
/* SPR_18107 start */
#define RRC_MAC_DL_EARFCN_PRESENT                           0x1000
/* SPR_18107 stop */

    U32               optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8   cell_id; /*^ M, 0, H, 0,MAX_CELL_INDEX ^*/

    U8   scell_index; /*^ M, 0, B, 1, 7 ^*/

    U8   operation_type; /*^ M, 0, H, 0, 2 ^*/ /* operation_type_et */

    rrc_mac_supported_mimo_capability_dl_r10_t   rrc_mac_supported_mimo_capability_dl_r10;
/*^ TLV, RRC_MAC_MIMO_CAPABILITY_DL_V1020, RRC_MAC_MIMO_CAPABILITY_DL_V1020_PRESENT ^*/

    rrc_mac_tx_mode_info_t          tx_mode_info;
/*^ TLV, RRC_MAC_TX_MODE_INFO, RRC_MAC_SCELL_TX_MODE_INFO_PRESENT ^*/

    rrc_mac_codebook_subset_restriction_v1020_t rrc_mac_codebook_subset_restriction_v1020;
/*^ TLV, RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020, RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT ^*/

    rrc_mac_pdsch_config_info_t     pdsch_config_info;
/*^ TLV, RRC_MAC_PDSCH_CONFIG_INFO, RRC_MAC_SCELL_PDSCH_CONFIG_INFO_PRESENT ^*/

    rrc_mac_cqi_info_t          cqi_info;
/*^ TLV, RRC_MAC_CQI_INFO, RRC_MAC_SCELL_CQI_INFO_PRESENT ^*/

    rrc_mac_dl_num_harq_process_t rrc_mac_dl_num_harq_process;
/*^ TLV, RRC_MAC_DL_NUM_HARQ_PROC, RRC_MAC_DL_NUM_HARQ_PROC_PRESENT ^*/

    rrc_mac_dl_init_mcs_t         rrc_mac_dl_init_mcs;
/*^ TLV, RRC_MAC_DL_INIT_MCS, RRC_MAC_DL_INIT_MCS_PRESENT ^*/

    rrc_mac_num_of_layer_info_t     num_of_layer_info;
/*^ TLV, RRC_MAC_NUM_OF_LAYER_INFO, RRC_MAC_SCELL_NUM_OF_LAYER_INFO_PRESENT ^*/

    rrc_mac_code_book_index_info_t  code_book_index_info;
/*^ TLV, RRC_MAC_CODE_BOOK_INDEX_INFO, RRC_MAC_SCELL_CODE_BOOK_INDEX_INFO_PRESENT ^*/

    rrc_mac_user_location_type_t          user_location_type;
    /*^ TLV, RRC_MAC_USER_LOCATION_TYPE, RRC_MAC_SCELL_USER_LOCATION_TYPE_PRESENT ^*/

    rrc_mac_rf_params_t           rf_params;
/*^ TLV, RRC_MAC_RF_PARAMS, RRC_MAC_SCELL_RF_PARAMS_PRESENT ^*/

/*CA start */
    scell_activate_status_t scell_activate_status;
/*^ TLV, RRC_MAC_SCELL_ACTIVATE_STATUS, RRC_MAC_SCELL_ACTIVATE_STATUS_PRESENT ^*/
/*CA end */
/* SPR_18107 start */
    rrc_mac_dl_earfcn_t  dl_earfcn;
/*^ TLV, RRC_MAC_DL_EARFCN, RRC_MAC_DL_EARFCN_PRESENT^*/
/* SPR_18107 stop */

}rrc_mac_scell_info_t;

typedef struct _rrc_mac_carrier_aggr_config_t
{
#define    RRC_MAC_SCELL_DEACTIVATION_TIMER_INFO_PRESENT 0x01
#define    RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO_PRESENT     0x02
    U16    presence_bitmask;      
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    scell_deactivation_timer_t  scell_deactivation_timer;
/*^ TLV,RRC_MAC_SCELL_DEACTIVATION_TIMER_INFO, RRC_MAC_SCELL_DEACTIVATION_TIMER_INFO_PRESENT ^*/

    rrc_counter_t           count;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_scell_info_t    rrc_mac_scell_info[RRC_MAX_SCELL];
/*^ TLV, SEQUENCE, RRC_MAC_SCELL_INFO ^*/

    rrc_mac_ca_lc_bandwidth_dist_info_t   ca_lc_bandwidth_dist_info;
    /*^ TLV, RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO, RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO_PRESENT ^*/
}rrc_mac_carrier_aggr_config_t;

/* eICIC changes start */
typedef struct _rrc_mac_dl_interference_info_t
{
    U8      severe_dl_control_channel_interference;     /*^ M, 0, H, 0, 1 ^*/    /* dl_interference_info_et  */
}rrc_mac_dl_interference_info_t;
/* eICIC changes stop */

/* SPR 21958 PUSCH RAT1 Support Start */
typedef struct _rrc_mac_uplink_rat1_info_t
{   
    U8   is_uplink_rat1_supported  ;              /*^ M, 0, H, 0, 1 ^*/
}rrc_mac_uplink_rat1_info_t;
/* SPR 21958 PUSCH RAT1 Support End */
/* 36321_CR0538 stop */
#define RRC_MAC_SR_CONFIG_INFO_PRESENT                    0x01
#define RRC_MAC_CQI_INFO_PRESENT                          0x02
#define RRC_MAC_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT        0x04
#define RRC_MAC_PUSCH_CONFIG_INFO_PRESENT                 0x08
#define RRC_MAC_SRS_DEDICATED_CONFIG_PRESENT              0x10
#define RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_PRESENT       0x20
#define RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT  0x40
#define RRC_MAC_DRX_CONFIG_INFO_PRESENT          0x80
#define RRC_MAC_MEAS_GAP_CONFIG_INFO_PRESENT     0x100
/* Rel9 Upgrade Changes Start */
#define RRC_MAC_SR_PROHBIT_TIMER_VALUE_V920_PRESENT 0x200
/* Rel9 Upgrade Changes Stop */


/* CLPC Start */

#define RRC_MAC_UPLINK_POWER_CONTROL_DEDICATED_PRESENT  0x400
#define RRC_MAC_TPC_PDCCH_CONFIG_PUCCH_PRESENT          0x800
#define RRC_MAC_TPC_PDCCH_CONFIG_PUSCH_PRESENT          0x1000
/* CLPC Stop */
#define RRC_MAC_AMBR_QOS_INFO_PRESENT                   0x2000

#define RRC_MAC_USER_LOCATION_TYPE_PRESENT              0x4000
#define RRC_MAC_UE_CAPABILITY_ADDITIONAL_PARAM_PRESENT  0x8000
/* SPS Start */
#define RRC_MAC_SPS_CONFIG_PRESENT                      0x10000
/* SPS Stop */
/* HD-FDD start */
#define RRC_MAC_CONFIG_RF_PARAMS_PRESENT                0x20000
/* HD-FDD stop */
/* SPR:5084 Start */
#define RRC_MAC_ADD_UE_CATEGORY_INFO_PRESENT            0x40000
/* SPR:5084 Stop */
/* UL_MU_MIMO Start */
#define RRC_MAC_UL_MU_MIMO_INFO_PRESENT                 0x80000
/* UL_MU_MIMO Stop */
/* min_ue_power start */
#define RRC_MAC_ADD_UE_PHR_TIMER_CONFIG_PRESENT                0x100000
#define RRC_MAC_MIN_UE_POWER_PRESENT                    0x200000
/* min_ue_power stop */
/* 36321_CR0538 start */
#define RRC_MAC_UE_COMPLIANCE_INFO_PRESENT              0x400000
/* 36321_CR0538 stop */
/* Transmission_mode_7_8 start*/
#define RRC_MAC_PDSCH_EPRE_TO_UE_RS_RATIO_PRESENT       0x800000
/* Transmission_mode_7_8 stop*/
/*TTI_BUNDLING Start*/
#define RRC_MAC_TTI_BUNDLING_INFO_PRESENT               0X1000000
/*TTI_BUNDLING Stop*/
#define RRC_MAC_EXTENDED_ADD_UE_INFO_PRESENT         0X2000000
#define RRC_MAC_ADD_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT   0X4000000
/* eICIC ho changes start */
#define RRC_MAC_DL_INTERFERENCE_INFO_PRESENT            0x8000000
/* eICIC ho changes stop */
/* SPR 23209 FIXED START */
#define RRC_MAC_MOCN_OPERATOR_INFO_PRESENT              0X10000000

/* SPR 23209 FIXED END */

/* eMTC changes start */
#define RRC_MAC_EPDCCH_SET_CONFIG_R11_PRESENT           0x01
#define RRC_MAC_CE_LEVEL_PRESENT                        0x02
#define RRC_MAC_CE_MODE_PRESENT                         0x04
#define RRC_MAC_PUCCH_CONFIG_DEDICATED_R13_PRESENT      0x08
/* eMTC changes stop */

typedef struct _rrc_mac_add_ue_info_t
{
/* HD-FDD start */
    U32               optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
/* HD-FDD stop */

    rrc_rnti_t  crnti;                          /*^ M, 0, B, 1, 65523 ^*/
    U8   ue_priority;                            /*^ M, 0, H, 0, 3 ^*/
    U8   dl_num_harq_process;                    /*^ M, 0, B, 1, 15 ^*/
   
    U8  dl_i_mcs;                               /*^ M, 0, H, 0, 28 ^*/
    U8  dl_max_rb;                              /*^ M, 0, B, 1, 100 ^*/
    U8  ul_i_mcs;                               /*^ M, 0, H, 0, 28 ^*/
    U8  transmission_mode;                      /*^ M, 0, B, 1, 8 ^*/


    U8  num_of_layer;                           /*^ M, 0, B, 1, 4 ^*/
    U8  code_book_index;                        /*^ M, 0, H, 0, 3 ^*/
    U8  max_ul_harq_tx;        /*^ M, 0, H, 0, 13 ^*/ /* rrc_mac_max_ul_harq_tx_et */
    U16 time_allignment_timer;   /*^ M, 0, H, 0, 7 ^*/

    /* CLPC Start */



    S8 pc_max;                   /*^ M, 0, B, -40, 23 ^*/
    U8 p_a;                      /*^ M, 0, H, 0, 7 ^*/

    /* CLPC Stop */
 
    rrc_mac_sr_config_info_t    sr_config_info;
/*^ TLV, RRC_MAC_SR_CONFIG_INFO, RRC_MAC_SR_CONFIG_INFO_PRESENT ^*/

    /* Rel9 Upgrade Changes Start */
    rrc_mac_sr_prohbit_timer_value_v920_t    sr_prohbit_timer_value_v920;
/*^ TLV, RRC_MAC_SR_PROHBIT_TIMER_VALUE_V920, RRC_MAC_SR_PROHBIT_TIMER_VALUE_V920_PRESENT ^*/
    /* Rel9 Upgrade Changes Stop */

    rrc_mac_cqi_info_t          cqi_info;
/*^ TLV, RRC_MAC_CQI_INFO, RRC_MAC_CQI_INFO_PRESENT ^*/

    rrc_mac_tdd_ack_nack_feedback_mode_t   mac_tdd_ack_nack_feedback_mode;
    /*^ TLV, RRC_MAC_TDD_ACK_NACK_FEEDBACK_MODE, RRC_MAC_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT ^*/
        
    rrc_mac_pusch_config_info_t            pusch_config_info;
/*^ TLV, RRC_MAC_PUSCH_CONFIG_INFO, RRC_MAC_PUSCH_CONFIG_INFO_PRESENT ^*/

    rrc_mac_srs_dedicated_config_t         srs_dedicated_config;
/*^ TLV, RRC_MAC_SRS_DEDICATED_CONFIG, RRC_MAC_SRS_DEDICATED_CONFIG_PRESENT ^*/

    rrc_mac_codebook_subset_restriction_t  codebook_subset_restriction;
/*^ TLV, RRC_MAC_CODEBOOK_SUBSET_RESTRICTION, RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_PRESENT ^*/

    rrc_mac_drx_config_info_t              drx_config_info;
/*^ TLV, RRC_MAC_DRX_CONFIG_INFO, RRC_MAC_DRX_CONFIG_INFO_PRESENT ^*/

    rrc_mac_meas_gap_config_info_t         meas_gap_config_info;
/*^ TLV, RRC_MAC_MEAS_GAP_CONFIG_INFO, RRC_MAC_MEAS_GAP_CONFIG_INFO_PRESENT ^*/


    rrc_mac_codebook_subset_restriction_v920_t codebook_subset_restriction_v920;
/*^ TLV, RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V920, RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT ^*/

    /* CLPC Start */
    rrc_mac_uplink_power_control_dedicated_t  uplink_power_control_dedicated;
/*^ TLV, RRC_MAC_UPLINK_POWER_CONTROL_DEDICATED, RRC_MAC_UPLINK_POWER_CONTROL_DEDICATED_PRESENT ^*/    

    rrc_mac_tpc_pdcch_config_pucch_t          tpc_pdcch_config_pucch;
/*^ TLV, RRC_MAC_TPC_PDCCH_CONFIG_PUCCH, RRC_MAC_TPC_PDCCH_CONFIG_PUCCH_PRESENT ^*/    

    rrc_mac_tpc_pdcch_config_pusch_t          tpc_pdcch_config_pusch;
/*^ TLV, RRC_MAC_TPC_PDCCH_CONFIG_PUSCH, RRC_MAC_TPC_PDCCH_CONFIG_PUSCH_PRESENT ^*/    
    /* CLPC Stop */

    rrc_mac_ambr_qos_info_t        ambr_qos_info;
    /*^ TLV, RRC_MAC_AMBR_QOS_INFO, RRC_MAC_AMBR_QOS_INFO_PRESENT ^*/
    rrc_mac_user_location_type_t          user_location_type;
    /*^ TLV, RRC_MAC_USER_LOCATION_TYPE, RRC_MAC_USER_LOCATION_TYPE_PRESENT ^*/
    rrc_mac_ue_capability_additional_params_t      ue_capability_additional_params;
    /*^ TLV, RRC_MAC_UE_CAPABILITY_ADDITIONAL_PARAMETERS, RRC_MAC_UE_CAPABILITY_ADDITIONAL_PARAM_PRESENT ^*/
    /* SPS Start */

    rrc_mac_sps_config_t           sps_config;
    /*^ TLV, RRC_MAC_SPS_CONFIG, RRC_MAC_SPS_CONFIG_PRESENT ^*/
    /* SPS Stop */
    /* HD-FDD start */
    rrc_mac_rf_params_t           rf_params;
/*^ TLV, RRC_MAC_RF_PARAMS, RRC_MAC_CONFIG_RF_PARAMS_PRESENT ^*/
    /* HD-FDD stop */
/* SPR:5084 Start */
    rrc_mac_ue_cat_t               ue_category_info;
    /*^ TLV, RRC_MAC_UE_CATEGORY_INFO, RRC_MAC_ADD_UE_CATEGORY_INFO_PRESENT ^*/
/* SPR:5084 Stop */
    /* UL_MU_MIMO Start */
    rrc_mac_ul_mu_mimo_info_t           ul_mu_mimo_info;
    /*^ TLV, RRC_MAC_UL_MU_MIMO_INFO, RRC_MAC_UL_MU_MIMO_INFO_PRESENT ^*/
    /* UL_MU_MIMO Stop */
    /* min_ue_power start */
    rrc_mac_phr_timer_config_t          phr_timer_config;
    /*^ TLV, RRC_MAC_PHR_TIMER_CONFIG, RRC_MAC_ADD_UE_PHR_TIMER_CONFIG_PRESENT ^*/
    rrc_mac_min_ue_power_t              min_ue_power;
    /*^ TLV, RRC_MAC_MIN_UE_POWER, RRC_MAC_MIN_UE_POWER_PRESENT ^*/
    /* min_ue_power stop */
    /* 36321_CR0538 start */
    rrc_mac_ue_compliance_info_t        ue_compliance_release;
    /*^ TLV, RRC_MAC_UE_COMPLIANCE_INFO, RRC_MAC_UE_COMPLIANCE_INFO_PRESENT ^*/
    /* 36321_CR0538 stop */
    /* Transmission_mode_7_8 start*/
    rrc_mac_pdsch_epre_to_ue_rs_ratio_t pdsch_epre_to_ue_rs_ratio;
    /*^ TLV, RRC_MAC_PDSCH_EPRE_TO_UE_RS_RATIO, RRC_MAC_PDSCH_EPRE_TO_UE_RS_RATIO_PRESENT ^*/
    /* Transmission_mode_7_8 stop*/
    /*TTI_BUNDLING Start*/
    rrc_mac_tti_bundling_info_t         tti_bundling;
    /*^ TLV, RRC_MAC_TTI_BUNDLING_INFO, RRC_MAC_TTI_BUNDLING_INFO_PRESENT ^*/
    /*TTI_BUNDLING Stop*/

    /* SPR 23209 FIXED START */
    rrc_mac_operator_info_t    operator_info;
    /*^ TLV, RRC_MAC_OPERATOR_INFO, RRC_MAC_MOCN_OPERATOR_INFO_PRESENT  ^*/
    /* SPR 23209 FIXED END */

    rrc_mac_codebook_subset_restriction_v1020_t rrc_mac_codebook_subset_restriction_v1020;
/*^ TLV, RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020, RRC_MAC_ADD_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT ^*/
    
    /* eICIC changes start */
    rrc_mac_dl_interference_info_t     rrc_mac_dl_interference_info; 
    /*^ TLV, RRC_MAC_DL_INTERFERENCE_INFO , RRC_MAC_DL_INTERFERENCE_INFO_PRESENT ^*/
    /* eICIC changes stop */

    /* eMTC changes start */
    U32                                emtc_optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    PucchConfigDedicatedR13_t          pucchConfigDedicatedR13;
    /*^ TLV, RRC_MAC_PUCCH_CONFIG_DEDICATED_R13, RRC_MAC_PUCCH_CONFIG_DEDICATED_R13_PRESENT ^*/

    EpdcchSetConfigR11_t               epdcchSetConfigR11;
    /*^ TLV, RRC_MAC_EPDCCH_SET_CONFIG_R11, RRC_MAC_EPDCCH_SET_CONFIG_R11_PRESENT ^*/

    RrcMacCeLevel_t                    ceLevel;
    /*^ TLV, RRC_MAC_CE_LEVEL, RRC_MAC_CE_LEVEL_PRESENT ^*/

    RrcMacCeMode_t                     ceMode;
    /*^ TLV, RRC_MAC_CE_MODE, RRC_MAC_CE_MODE_PRESENT ^*/
    /* eMTC changes stop */

    U32   ca_optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_MAC_RECONFIG_ADD_UE_CATEGORY_V1020_PRESENT         0x01       
#define RRC_MAC_RECONFIG_ADD_PUCCH_FORMAT_V1020_PRESENT        0x02       
#define RRC_MAC_CA_RECONFIG_ADD_CONFIG_PRESENT                 0x04

    rrc_mac_ue_category_v1020_t         rrc_mac_ue_category_v1020;
/*^ TLV, RRC_MAC_UE_CATEGORY_V1020, RRC_MAC_RECONFIG_ADD_UE_CATEGORY_V1020_PRESENT ^*/

    rrc_mac_pucch_format_r10_t         rrc_mac_pucch_format_r10;
    /*^ TLV, RRC_MAC_PUCCH_FORMAT_V1020, RRC_MAC_RECONFIG_ADD_PUCCH_FORMAT_V1020_PRESENT ^*/

    rrc_mac_carrier_aggr_config_t      rrc_mac_carrier_aggr_config;
    /*^ TLV, RRC_MAC_CA_CONFIG, RRC_MAC_CA_RECONFIG_ADD_CONFIG_PRESENT ^*/
} rrc_mac_add_ue_info_t;

#define RRC_MAC_ADD_UE_INFO_PRESENT 0x01
/* SPR 21958 PUSCH RAT1 Support Start */
#define RRC_MAC_ADD_RAT1_INFO_PRESENT 0x02       
/* SPR 21958 PUSCH RAT1 Support End */

typedef struct _rrc_mac_create_ue_entity_req_t
{
    rrc_bitmask_t           optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_ue_index_t          ue_index;

    rrc_mac_add_ue_info_t   add_ue_info;
/*^ TLV, RRC_MAC_ADD_UE_INFO, RRC_MAC_ADD_UE_INFO_PRESENT ^*/


    rrc_counter_t           create_lc_req_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_create_lc_req_t create_lc_req[RRC_MAC_MAX_CREATE_LC];
/*^ TLV, SEQUENCE, RRC_MAC_CREATE_LC_REQ ^*/

    /* SPR 21958 PUSCH RAT1 Support Start */
    rrc_mac_uplink_rat1_info_t  		    uplink_rat1_info;
    /*^ TLV, RRC_MAC_RAT1_INFO , RRC_MAC_ADD_RAT1_INFO_PRESENT ^*/
    /* SPR 21958 PUSCH RAT1 Support End */

} rrc_mac_create_ue_entity_req_t; /*^ API, RRC_MAC_CREATE_UE_ENTITY_REQ ^*/

/******************************************************************************
*   RRC_MAC_CREATE_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_mac_dl_lc_config_resp_t
{
    U8 dummy;
} rrc_mac_dl_lc_config_resp_t;

typedef struct _rrc_mac_ul_lc_config_resp_t
{
    U8 dummy;
} rrc_mac_ul_lc_config_resp_t;

#define RRC_MAC_UL_LC_CONFIG_RESP_PRESENT 0x01
#define RRC_MAC_DL_LC_CONFIG_RESP_PRESENT 0x02

typedef struct _rrc_mac_create_lc_error_t
{
    rrc_bitmask_t               optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    rrc_lc_id_t                 lch_id;         /*^ M, 0, H, 0, 10 ^*/
    rrc_response_t              response;
/*^ M, 0, H, 0, 1 ^*/   /* rrc_mac_logical_channel_error_code_et */


    rrc_mac_ul_lc_config_resp_t   ul_lc_config_resp;
/*^ TLV, EMPTY, RRC_MAC_UL_LC_CONFIG_RESP, RRC_MAC_UL_LC_CONFIG_RESP_PRESENT ^*/

    rrc_mac_dl_lc_config_resp_t   dl_lc_config_resp;
/*^ TLV, EMPTY, RRC_MAC_DL_LC_CONFIG_RESP, RRC_MAC_DL_LC_CONFIG_RESP_PRESENT ^*/

} rrc_mac_create_lc_error_t;

/* SPS Start */

typedef struct _rrc_mac_sps_crnti_info_t 
{
    U16          sps_crnti; /*^ M, 0, B, 61, 65523 ^*/
}rrc_mac_sps_crnti_info_t;

#define RRC_MAC_SPS_CRNTI_INFO_PRESENT 0x01
/* SPS Stop */
typedef struct _rrc_mac_create_ue_entity_cnf_t
{
/* SPS Start */
    rrc_bitmask_t         bitmask; /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
/* SPS Stop */
    rrc_ue_index_t   ue_index;
    rrc_response_t   response_code;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_mac_return_et */


    rrc_counter_t               create_lc_error_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_create_lc_error_t   create_lc_error[RRC_MAC_MAX_CREATE_LC];
/*^ TLV, SEQUENCE, RRC_MAC_CREATE_LC_ERROR ^*/

    /* SPS Start */
    rrc_mac_sps_crnti_info_t    sps_crnti_info;
/*^ TLV, RRC_MAC_SPS_CRNTI_INFO, RRC_MAC_SPS_CRNTI_INFO_PRESENT ^*/    
    /* SPS Stop */

    rrc_counter_t               create_ca_config_error_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_ca_config_error_t    rrc_mac_create_ca_config_err[MAX_SCELL];
/*^ TLV, SEQUENCE, RRC_MAC_CREATE_CA_CONFIG_ERROR ^*/    

} rrc_mac_create_ue_entity_cnf_t; /*^ API, RRC_MAC_CREATE_UE_ENTITY_CNF ^*/

/******************************************************************************
*   RRC_MAC_DELETE_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_mac_delete_ue_entity_req_t
{
    rrc_ue_index_t   ue_index;
} rrc_mac_delete_ue_entity_req_t; /*^ API, RRC_MAC_DELETE_UE_ENTITY_REQ ^*/

/******************************************************************************
*   RRC_MAC_DELETE_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_mac_delete_lc_status_t
{
    rrc_lc_id_t     lch_id;                 /*^ M, 0, H, 0, 10 ^*/
    
    U8  status;
    /*^ M, 0, B, 1, 2 ^*/ /* rrc_mac_delete_lc_status_et */
}rrc_mac_delete_lc_status_t;

typedef struct _rrc_mac_delete_ue_entity_cnf_t
{
    rrc_ue_index_t   ue_index;
    rrc_response_t   response;   /*^ M, 0, H, 0, 1 ^*/ /* rrc_mac_return_et */

    /* CR 101 changes start*/
    rrc_counter_t                   delete_lc_status_counter;
    /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_delete_lc_status_t      delete_lc_status[RRC_MAC_MAX_DELETE_LC];
    /*^ TLV, SEQUENCE, RRC_MAC_DELETE_LC_STATUS ^*/
    /* CR 101 changes stop*/
} rrc_mac_delete_ue_entity_cnf_t; /*^ API, RRC_MAC_DELETE_UE_ENTITY_CNF ^*/

/******************************************************************************
*   RRC_MAC_RECONFIGURE_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_mac_delete_lc_req_t
{
    rrc_lc_id_t   lch_id;             /*^ M, 0, H, 0, 10 ^*/
    U8            lc_type;
/*^ M, 0, B, 1, 3 ^*/   /* rrc_mac_delete_lc_types_et */

} rrc_mac_delete_lc_req_t;

typedef struct _rrc_mac_dl_lc_reconfig_req_t
{
#define RRC_MAC_DL_RECONFIG_CA_LC_BANDWIDTH_DIST_INFO_PRESENT 0x01
    rrc_bitmask_t                  optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_ca_lc_bandwidth_dist_info_t   ca_lc_bandwidth_dist_info;
    /*^ TLV, RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO, RRC_MAC_DL_RECONFIG_CA_LC_BANDWIDTH_DIST_INFO_PRESENT ^*/
} rrc_mac_dl_lc_reconfig_req_t;

typedef struct _rrc_mac_ul_lc_reconfig_req_t
{
    /* SPR 15774 FIX START */
    U8 dummy;
    /* SPR 15774 FIX STOP */
} rrc_mac_ul_lc_reconfig_req_t;

#define RRC_MAC_UL_LC_RECONFIG_REQ_PRESENT 0x01
#define RRC_MAC_DL_LC_RECONFIG_REQ_PRESENT 0x02
#define RRC_MAC_GBR_QOS_INFO_PRESENT       0x04
typedef struct _rrc_mac_reconfigure_lc_req_t
{
    rrc_bitmask_t                  optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    rrc_lc_id_t                    lch_id; /*^ M, 0, H, 0, 10 ^*/

    rrc_mac_ul_lc_reconfig_req_t   ul_lc_reconfig_req;
    /* SPR 15774 FIX START */
/*^ TLV, EMPTY, RRC_MAC_UL_LC_RECONFIGURE_REQ, RRC_MAC_UL_LC_RECONFIG_REQ_PRESENT ^*/
    /* SPR 15774 FIX STOP */

    rrc_mac_dl_lc_reconfig_req_t   dl_lc_reconfig_req;
/*^ TLV, RRC_MAC_DL_LC_RECONFIGURE_REQ, RRC_MAC_DL_LC_RECONFIG_REQ_PRESENT ^*/
    
    rrc_mac_gbr_qos_info_t   gbr_qos_info;
/*^ TLV, RRC_MAC_GBR_QOS_INFO, RRC_MAC_GBR_QOS_INFO_PRESENT ^*/
} rrc_mac_reconfigure_lc_req_t;


typedef struct _rrc_mac_ul_max_rb_info_t
{
    U8   ul_max_rb;                      /*^ M, 0, B, 1, 100 ^*/
} rrc_mac_ul_max_rb_info_t;

typedef struct _rrc_mac_dl_max_rb_info_t
{
    U8   dl_max_rb;                      /*^ M, 0, B, 1, 100 ^*/
} rrc_mac_dl_max_rb_info_t;


/* CLPC Start */

typedef struct _rrc_mac_p0_ue_pusch_t
{
    S8 p0_ue_pusch;    /*^ M, 0, B, -8, 7 ^*/
}rrc_mac_p0_ue_pusch_t;


/* CLPC Stop */


/* SPR 21958 PUSCH RAT1 Support Start */
/* MACRO moved below */
/* SPR 21958 PUSCH RAT1 Support Start */
#define RRC_MAC_RECONF_CQI_INFO_PRESENT                 0x01
#define RRC_MAC_DL_MAX_RB_INFO_PRESENT                  0x02
#define RRC_MAC_TX_MODE_INFO_PRESENT                    0x04
#define RRC_MAC_NUM_OF_LAYER_INFO_PRESENT               0x08
#define RRC_MAC_CODE_BOOK_INDEX_INFO_PRESENT            0x10
#define RRC_MAC_RECONF_SR_CONFIG_INFO_PRESENT           0x20
#define RRC_MAC_SIMULTANEOUS_ACK_NACK_CQI_INFO_PRESENT  0x40
/* SPR 928 */
#define RRC_MAC_UE_CATEGORY_INFO_PRESENT                0x80
/* SPR 928 */
/* SRS Start */
#define RRC_MAC_RECONFIG_SRS_DEDICATED_CONFIG_PRESENT    0x100
/* SRS Stop */
#define RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_PRESENT  0x200
#define RRC_MAC_RECONFIG_DRX_CONFIG_INFO_PRESENT         0x400
#define RRC_MAC_RECONFIG_MEAS_GAP_CONFIG_INFO_PRESENT    0x800
#define RRC_MAC_TIME_ALIGNMENT_TIME_INFO_PRESENT          0x1000
#define RRC_MAC_RECONFIG_CQI_REPORT_CONFIG_V920_PRESENT             0x2000
#define RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT   0x4000
/* Rel9 Upgrade Changes Start */
#define RRC_MAC_RECONFIG_SR_PROHBIT_TIMER_VALUE_V920_PRESENT   0x8000
/* Rel9 Upgrade Changes Stop */

/* CLPC Start */
#define RRC_MAC_PDSCH_CONFIG_INFO_PRESENT                   0x10000
#define RRC_MAC_P0_UE_PUSCH_PRESENT                         0x20000
#define RRC_MAC_RECONFIG_TPC_PDCCH_CONFIG_PUCCH_PRESENT     0x40000
#define RRC_MAC_RECONFIG_TPC_PDCCH_CONFIG_PUSCH_PRESENT     0x80000


/* CLPC Stop */
#define RRC_MAC_RECONFIG_AMBR_QOS_INFO_PRESENT              0x100000

/* SPS Start */
#define RRC_MAC_RECONFIG_SPS_CONFIG_PRESENT                 0x200000
/* SPS Stop */
/* HD-FDD start */
#define RRC_MAC_RECONFIG_RF_PARAMS_PRESENT                  0x400000
/* HD-FDD stop */
/* min_ue_power start */
#define RRC_MAC_RECONFIG_PHR_TIMER_CONFIG_PRESENT                    0x800000
/* min_ue_power stop */
/* 36321_CR0538 start */
#define RRC_MAC_RECONFIG_UE_COMPLIANCE_INFO_PRESENT         0x1000000
/* 36321_CR0538 stop */
/* Transmission_mode_7_8 start*/
#define RRC_MAC_RECONFIG_PDSCH_EPRE_TO_UE_RS_RATIO_PRESENT           0x2000000 
/* Transmission_mode_7_8 stop*/
/*TTI_BUNDLING Start*/
#define RRC_MAC_RECONFIG_TTI_BUNDLING_INFO_PRESENT          0x4000000
/*TTI_BUNDLING Stop*/
/*Dynamic_ue_reconfig +*/
#define RRC_MAC_RECONFIG_MAX_UL_HARQ_TX_PRESENT           0x8000000
#define RRC_MAC_RECONFIG_PC_MAX_CONFIG_INFO_PRESENT     0x10000000
#define RRC_MAC_RECONFIG_PUSCH_CONFIG_INFO_PRESENT         0x20000000       
/*Dynamic_ue_reconfig -*/
#define RRC_MAC_EXTENDED_RECONFIG_UE_INFO_PRESENT             0x40000000       
#define RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT   0x80000000       
/* eICIC changes start */
#define RRC_MAC_RECONFIG_ADD_DL_INTERFERENCE_INFO_PRESENT      0x100000000
/* eICIC changes stop */
/*SPR_18803_START*/
#define RRC_MAC_RECONFIG_UE_CAPABILITY_ADDITIONAL_PARAM_PRESENT 0x200000000
/*SPR_18803_END*/

/* SPR 23209 FIXED START */
#define RRC_MAC_RECONFIG_OPERATOR_ID_INFO_PRESENT 0x400000000
/* SPR 23209 FIXED END*/
/* eMTC changes start */
#define RRC_MAC_RECONFIG_EPDCCH_SET_CONFIG_R11_PRESENT           0x01
#define RRC_MAC_RECONFIG_CE_LEVEL_PRESENT                        0x02
#define RRC_MAC_RECONFIG_CE_MODE_PRESENT                         0x04
#define RRC_MAC_RECONFIG_PUCCH_CONFIG_DEDICATED_R13_PRESENT      0x08
#define RRC_MAC_RECONFIG_UE_CATEGORY_V1310_PRESENT               0x10
/* eMTC changes stop */


typedef struct _rrc_mac_reconfig_ue_info_t
{
    /* eICIC changes start */
    U64                   optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    /* eICIC changes stop */


    rrc_mac_cqi_info_t              cqi_info;
/*^ TLV, RRC_MAC_CQI_INFO, RRC_MAC_RECONF_CQI_INFO_PRESENT ^*/

    rrc_mac_dl_max_rb_info_t        dl_max_rb_info;
/*^ TLV, RRC_MAC_DL_MAX_RB_INFO, RRC_MAC_DL_MAX_RB_INFO_PRESENT ^*/

    rrc_mac_tx_mode_info_t          tx_mode_info;
/*^ TLV, RRC_MAC_TX_MODE_INFO, RRC_MAC_TX_MODE_INFO_PRESENT ^*/

    rrc_mac_num_of_layer_info_t     num_of_layer_info;
/*^ TLV, RRC_MAC_NUM_OF_LAYER_INFO, RRC_MAC_NUM_OF_LAYER_INFO_PRESENT ^*/

    rrc_mac_code_book_index_info_t  code_book_index_info;
/*^ TLV, RRC_MAC_CODE_BOOK_INDEX_INFO, RRC_MAC_CODE_BOOK_INDEX_INFO_PRESENT ^*/

    rrc_mac_sr_config_info_t        sr_config_info;
/*^ TLV, RRC_MAC_SR_CONFIG_INFO, RRC_MAC_RECONF_SR_CONFIG_INFO_PRESENT ^*/

    /* Rel9 Upgrade Changes Start */
    rrc_mac_sr_prohbit_timer_value_v920_t    sr_prohbit_timer_value_v920;
    /*^ TLV, RRC_MAC_SR_PROHBIT_TIMER_VALUE_V920, RRC_MAC_RECONFIG_SR_PROHBIT_TIMER_VALUE_V920_PRESENT ^*/
    /* Rel9 Upgrade Changes Stop */

    rrc_mac_ue_cat_t                ue_category_info;
/*^ TLV, RRC_MAC_UE_CATEGORY_INFO, RRC_MAC_UE_CATEGORY_INFO_PRESENT ^*/
    /* SRS Start */
    rrc_mac_srs_dedicated_config_t  srs_dedicated_config;
    /*^ TLV, RRC_MAC_SRS_DEDICATED_CONFIG, RRC_MAC_RECONFIG_SRS_DEDICATED_CONFIG_PRESENT ^*/
    /* SRS Stop */

    rrc_mac_codebook_subset_restriction_t codebook_subset_restriction;
    /*^ TLV, RRC_MAC_CODEBOOK_SUBSET_RESTRICTION, RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_PRESENT ^*/

    rrc_mac_drx_config_info_t             drx_config_info;
/*^ TLV, RRC_MAC_DRX_CONFIG_INFO, RRC_MAC_RECONFIG_DRX_CONFIG_INFO_PRESENT ^*/ 
 
    rrc_mac_meas_gap_config_info_t        meas_gap_config_info;
/*^ TLV, RRC_MAC_MEAS_GAP_CONFIG_INFO, RRC_MAC_RECONFIG_MEAS_GAP_CONFIG_INFO_PRESENT ^*/


    rrc_mac_time_alignment_timer_info_t   time_alignment_timer_info;
    /*^ TLV, RRC_MAC_TIME_ALIGNMENT_TIME_INFO, RRC_MAC_TIME_ALIGNMENT_TIME_INFO_PRESENT  ^*/
    
    rrc_mac_codebook_subset_restriction_v920_t codebook_subset_restriction_v920;
/*^ TLV, RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V920, RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT ^*/

    /* CLPC Start */

    rrc_mac_pdsch_config_info_t     pdsch_config_info;
/*^ TLV, RRC_MAC_PDSCH_CONFIG_INFO, RRC_MAC_PDSCH_CONFIG_INFO_PRESENT ^*/

    rrc_mac_p0_ue_pusch_t           p0_ue_pusch;
/*^ TLV, RRC_MAC_P0_UE_PUSCH, RRC_MAC_P0_UE_PUSCH_PRESENT ^*/

    rrc_mac_tpc_pdcch_config_pucch_t          tpc_pdcch_config_pucch;
/*^ TLV, RRC_MAC_TPC_PDCCH_CONFIG_PUCCH, RRC_MAC_RECONFIG_TPC_PDCCH_CONFIG_PUCCH_PRESENT ^*/    

    rrc_mac_tpc_pdcch_config_pusch_t          tpc_pdcch_config_pusch;
/*^ TLV, RRC_MAC_TPC_PDCCH_CONFIG_PUSCH, RRC_MAC_RECONFIG_TPC_PDCCH_CONFIG_PUSCH_PRESENT ^*/   

    /* CLPC Stop */
    rrc_mac_ambr_qos_info_t        ambr_qos_info;
/*^ TLV, RRC_MAC_AMBR_QOS_INFO, RRC_MAC_RECONFIG_AMBR_QOS_INFO_PRESENT ^*/

    /*SPR_18803_START*/
    rrc_mac_ue_capability_additional_params_t      ue_capability_additional_params;
/*^ TLV, RRC_MAC_UE_CAPABILITY_ADDITIONAL_PARAMETERS, RRC_MAC_RECONFIG_UE_CAPABILITY_ADDITIONAL_PARAM_PRESENT ^*/
    /*SPR_18803_END*/

    /* SPS Start */

    rrc_mac_sps_config_t           sps_config;
    /*^ TLV, RRC_MAC_SPS_CONFIG, RRC_MAC_RECONFIG_SPS_CONFIG_PRESENT ^*/

    /* SPS Stop */
    /* HD-FDD start */
    rrc_mac_rf_params_t           rf_params;
/*^ TLV, RRC_MAC_RF_PARAMS, RRC_MAC_RECONFIG_RF_PARAMS_PRESENT ^*/
    /* HD-FDD stop */
    /* min_ue_power start */
    rrc_mac_phr_timer_config_t          phr_timer_config;
    /*^ TLV, RRC_MAC_PHR_TIMER_CONFIG, RRC_MAC_RECONFIG_PHR_TIMER_CONFIG_PRESENT ^*/
    /* min_ue_power stop */
    /* 36321_CR0538 start */
    rrc_mac_ue_compliance_info_t      ue_compliance_release;
    /*^ TLV, RRC_MAC_UE_COMPLIANCE_INFO, RRC_MAC_RECONFIG_UE_COMPLIANCE_INFO_PRESENT ^*/
    /* 36321_CR0538 stop */
    /* Transmission_mode_7_8 start*/
    rrc_mac_pdsch_epre_to_ue_rs_ratio_t pdsch_epre_to_ue_rs_ratio;
    /*^ TLV, RRC_MAC_PDSCH_EPRE_TO_UE_RS_RATIO, RRC_MAC_RECONFIG_PDSCH_EPRE_TO_UE_RS_RATIO_PRESENT ^*/
    /* Transmission_mode_7_8 stop*/
    /*TTI_BUNDLING Start*/
    rrc_mac_tti_bundling_info_t         tti_bundling;
    /*^ TLV, RRC_MAC_TTI_BUNDLING_INFO, RRC_MAC_RECONFIG_TTI_BUNDLING_INFO_PRESENT ^*/
    /*TTI_BUNDLING Stop*/
/*Dynamic_ue_reconfig +*/
    rrc_mac_max_ul_harq_tx_info_t  max_ul_harq_tx_info;        
/*^ TLV, RRC_MAC_MAX_UL_HARQ_TX_INFO, RRC_MAC_RECONFIG_MAX_UL_HARQ_TX_PRESENT ^*/

    rrc_mac_pc_max_config_info_t  pc_max_info;
/*^ TLV, RRC_MAC_PC_MAX_INFO, RRC_MAC_RECONFIG_PC_MAX_CONFIG_INFO_PRESENT ^*/

    rrc_mac_pusch_config_info_t            pusch_config_info;
    /* SPR 12313 Fix Start */
/*^ TLV, RRC_MAC_PUSCH_CONFIG_INFO, RRC_MAC_RECONFIG_PUSCH_CONFIG_INFO_PRESENT ^*/
    /* SPR 12313 Fix Stop */
/*Dynamic_ue_reconfig -*/
    
    rrc_mac_codebook_subset_restriction_v1020_t rrc_mac_codebook_subset_restriction_v1020;
/*^ TLV, RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020, RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT ^*/

    /* eICIC changes start */
    rrc_mac_dl_interference_info_t     rrc_mac_dl_interference_info; 
    /*^ TLV, RRC_MAC_DL_INTERFERENCE_INFO , RRC_MAC_RECONFIG_ADD_DL_INTERFERENCE_INFO_PRESENT ^*/
    /* eICIC changes stop */
    /*SPR 23457 FIXED START */
    rrc_mac_operator_info_t    operator_info;
    /*^ TLV, RRC_MAC_OPERATOR_INFO, RRC_MAC_RECONFIG_OPERATOR_ID_INFO_PRESENT  ^*/
    /*SPR 23457 FIXED END */

    /* eMTC changes start */
    U32                                emtc_optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    PucchConfigDedicatedR13_t          pucchConfigDedicatedR13;
    /*^ TLV, RRC_MAC_PUCCH_CONFIG_DEDICATED_R13, RRC_MAC_RECONFIG_PUCCH_CONFIG_DEDICATED_R13_PRESENT ^*/

    EpdcchSetConfigR11_t               epdcchSetConfigR11;
    /*^ TLV, RRC_MAC_EPDCCH_SET_CONFIG_R11, RRC_MAC_RECONFIG_EPDCCH_SET_CONFIG_R11_PRESENT ^*/

    RrcMacCeLevel_t                    ceLevel;
    /*^ TLV, RRC_MAC_CE_LEVEL, RRC_MAC_RECONFIG_CE_LEVEL_PRESENT ^*/

    RrcMacCeMode_t                     ceMode;
    /*^ TLV, RRC_MAC_CE_MODE, RRC_MAC_RECONFIG_CE_MODE_PRESENT ^*/

    RrcMacUeCategoryV1310_t            ueCategoryV1310;
    /*^ TLV, RRC_MAC_UE_CATEGORY_V1310, RRC_MAC_RECONFIG_UE_CATEGORY_V1310_PRESENT ^*/
    /* eMTC changes stop */

    U32   ca_optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_MAC_RECONFIG_ADD_UE_CATEGORY_V1020_PRESENT         0x01       
#define RRC_MAC_RECONFIG_ADD_PUCCH_FORMAT_V1020_PRESENT        0x02       
#define RRC_MAC_CA_RECONFIG_ADD_CONFIG_PRESENT                 0x04

    rrc_mac_ue_category_v1020_t         rrc_mac_ue_category_v1020;
/*^ TLV, RRC_MAC_UE_CATEGORY_V1020, RRC_MAC_RECONFIG_ADD_UE_CATEGORY_V1020_PRESENT ^*/

    rrc_mac_pucch_format_r10_t         rrc_mac_pucch_format_r10;
    /*^ TLV, RRC_MAC_PUCCH_FORMAT_V1020, RRC_MAC_RECONFIG_ADD_PUCCH_FORMAT_V1020_PRESENT ^*/

    rrc_mac_carrier_aggr_config_t      rrc_mac_carrier_aggr_config;
    /*^ TLV, RRC_MAC_CA_CONFIG, RRC_MAC_CA_RECONFIG_ADD_CONFIG_PRESENT ^*/

} rrc_mac_reconfig_ue_info_t;


/* SPR 21958 PUSCH RAT1 Support Start */
#define RRC_MAC_RECONFIG_UE_INFO_PRESENT                0x01
#define RRC_MAC_RECONFIG_RAT1_INFO_PRESENT              0x02       
/* SPR 21958 PUSCH RAT1 Support End */

typedef struct _rrc_mac_reconfigure_ue_entity_req_t
{
    rrc_bitmask_t                   optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_ue_index_t                  ue_index;

    rrc_mac_reconfig_ue_info_t      reconfig_ue_info;
/*^ TLV, RRC_MAC_RECONFIG_UE_INFO, RRC_MAC_RECONFIG_UE_INFO_PRESENT ^*/


    rrc_counter_t                   create_lc_req_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_create_lc_req_t         create_lc_req[RRC_MAC_MAX_CREATE_LC];
/*^ TLV, SEQUENCE, RRC_MAC_CREATE_LC_REQ ^*/


    rrc_counter_t                   reconfigure_lc_req_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_reconfigure_lc_req_t
        reconfigure_lc_req[RRC_MAC_MAX_RECONFIGURE_LC];
/*^ TLV, SEQUENCE, RRC_MAC_RECONFIGURE_LC_REQ ^*/


    rrc_counter_t                   delete_lc_req_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_delete_lc_req_t         delete_lc_req[RRC_MAC_MAX_DELETE_LC];
/*^ TLV, SEQUENCE, RRC_MAC_DELETE_LC_REQ ^*/

/* SPR 21958 PUSCH RAT1 Support Start */
    rrc_mac_uplink_rat1_info_t  		    uplink_rat1_info;
/*^ TLV, RRC_MAC_RAT1_INFO , RRC_MAC_RECONFIG_RAT1_INFO_PRESENT ^*/
/* SPR 21958 PUSCH RAT1 Support End */

} rrc_mac_reconfigure_ue_entity_req_t;
/*^ API, RRC_MAC_RECONFIGURE_UE_ENTITY_REQ ^*/


/******************************************************************************
*   RRC_MAC_RECONFIGURE_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_mac_delete_lc_error_t
{
    rrc_lc_id_t      lch_id;             /*^ M, 0, H, 0, 10 ^*/
    rrc_response_t   response;
/*^ M, 0, H, 0, 1 ^*/       /* rrc_mac_logical_channel_error_code_et */

    U8               lc_type;
/*^ M, 0, H, 0, 2 ^*/       /* rrc_mac_delete_lc_types_et */

} rrc_mac_delete_lc_error_t;

#define RRC_MAC_RECONF_UL_LC_CONFIG_RESP_PRESENT 0x01
#define RRC_MAC_RECONF_DL_LC_CONFIG_RESP_PRESENT 0x02

typedef struct _rrc_mac_reconfigure_lc_error_t
{
    rrc_bitmask_t   optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_lc_id_t     lch_id;                 /*^ M, 0, H, 0, 10 ^*/
    rrc_response_t  response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_mac_return_et */


    rrc_mac_ul_lc_config_resp_t   ul_lc_config_resp;
/*^ TLV, EMPTY, RRC_MAC_UL_LC_CONFIG_RESP, RRC_MAC_RECONF_UL_LC_CONFIG_RESP_PRESENT ^*/

    rrc_mac_dl_lc_config_resp_t   dl_lc_config_resp;
/*^ TLV, EMPTY, RRC_MAC_DL_LC_CONFIG_RESP, RRC_MAC_RECONF_DL_LC_CONFIG_RESP_PRESENT ^*/

} rrc_mac_reconfigure_lc_error_t;

typedef struct _rrc_mac_reconfigure_ue_entity_cnf_t
{
    rrc_bitmask_t   optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
/* SPS Start */
#define RRC_MAC_RECONF_SPS_CRNTI_INFO_PRESENT 0x01
/* SPS Stop */

    rrc_ue_index_t   ue_index;
    rrc_response_t   response_code;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_mac_return_et */


    rrc_counter_t                   create_lc_error_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_create_lc_error_t       create_lc_error[RRC_MAC_MAX_CREATE_LC];
/*^ TLV, SEQUENCE, RRC_MAC_CREATE_LC_ERROR ^*/


    rrc_counter_t                   reconfigure_lc_error_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_reconfigure_lc_error_t
        reconfigure_lc_error[RRC_MAC_MAX_RECONFIGURE_LC];
/*^ TLV, SEQUENCE, RRC_MAC_RECONFIGURE_LC_ERROR ^*/


    rrc_counter_t                   delete_lc_error_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_delete_lc_error_t       delete_lc_error[RRC_MAC_MAX_DELETE_LC];
/*^ TLV, SEQUENCE, RRC_MAC_DELETE_LC_ERROR ^*/
    
    rrc_counter_t                   delete_lc_status_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_delete_lc_status_t      delete_lc_status[RRC_MAC_MAX_DELETE_LC];
/*^ TLV, SEQUENCE, RRC_MAC_DELETE_LC_STATUS ^*/
    /* SPS Start */
    rrc_mac_sps_crnti_info_t    sps_crnti_info;
/*^ TLV, RRC_MAC_SPS_CRNTI_INFO, RRC_MAC_RECONF_SPS_CRNTI_INFO_PRESENT ^*/    
    /* SPS Stop */
    rrc_counter_t               ca_config_error_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_ca_config_error_t    rrc_mac_create_ca_config_err[MAX_SCELL];
/*^ TLV, SEQUENCE, RRC_MAC_CREATE_CA_CONFIG_ERROR ^*/    

    rrc_counter_t               reconfig_ca_config_error_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_ca_reconfig_error_t   rrc_mac_reconfigure_ca_config_err[MAX_SCELL];
/*^ TLV, SEQUENCE, RRC_MAC_RECONFIGURE_CA_CONFIG_ERROR ^*/    

    rrc_counter_t               delete_ca_config_error_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_delete_ca_config_error_t   rrc_mac_delete_ca_config_err[MAX_SCELL];
/*^ TLV, SEQUENCE, RRC_MAC_DELETE_CA_CONFIG_ERROR ^*/    
} rrc_mac_reconfigure_ue_entity_cnf_t;
/*^ API, RRC_MAC_RECONFIGURE_UE_ENTITY_CNF ^*/


/******************************************************************************
*   RRC_MAC_UE_ENTITY_POWER_HEADROOM_IND
******************************************************************************/
typedef struct _rrc_mac_ue_entity_power_headroom_ind_t
{
    rrc_ue_index_t   ue_index;
    rrc_rnti_t       crnti;              /*^ M, 0, B, 1, 65523 ^*/
    U16              power_headroom;     /*^ M, 0, H, 0, 63 ^*/
} rrc_mac_ue_entity_power_headroom_ind_t;
/*^ API, RRC_MAC_UE_ENTITY_POWER_HEADROOM_IND ^*/

/******************************************************************************
*   RRC_MAC_UE_DRX_CMD_REQ
******************************************************************************/
typedef struct _rrc_mac_ue_drx_cmd_req_t
{
    rrc_ue_index_t   ue_index;
} rrc_mac_ue_drx_cmd_req_t; /*^ API, RRC_MAC_UE_DRX_CMD_REQ ^*/

/******************************************************************************
*   RRC_MAC_UE_CON_REJ_REQ
******************************************************************************/
typedef struct _rrc_mac_ue_con_rej_req_t
{
    rrc_rnti_t   tcrnti;             /*^ M, 0, B, 1, 65523 ^*/
    U8           ccch_msg_buf[0];    /*^ M, 0, OCTET_STRING, TILL_THE_END ^*/
} rrc_mac_ue_con_rej_req_t; /*^ API, RRC_MAC_UE_CON_REJ_REQ ^*/

/******************************************************************************
*   MAC common channel messages
******************************************************************************/

/******************************************************************************
*   RRC_MAC_BCCH_CONFIG_REQ
******************************************************************************/

#define RRC_MAC_BCCH_SIBTYPE1_MSG_REQ_PRESENT   0x01
#define RRC_MAC_BCCH_SI_MSG_REQ_PRESENT         0x02

typedef struct _rrc_mac_bcch_config_req_t
{
    rrc_bitmask_t              optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_counter_t               mib_msg_req_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_mac_mib_msg_req_t       mib_msg_req[RRC_MIB_MSGS];
/*^ TLV, SEQUENCE, RRC_MAC_MIB_MSG_REQ ^*/

    rrc_mac_sibtype1_msg_req_t  sib1_msg_req;
/*^ TLV, RRC_MAC_SIBTYPE1_MSG_REQ, RRC_MAC_BCCH_SIBTYPE1_MSG_REQ_PRESENT ^*/

    rrc_mac_si_msg_req_t        si_msg_req;
/*^ TLV, RRC_MAC_SI_MSG_INFO, RRC_MAC_BCCH_SI_MSG_REQ_PRESENT ^*/

} rrc_mac_bcch_config_req_t; /*^ API, RRC_MAC_BCCH_CONFIG_REQ ^*/

/******************************************************************************
*   RRC_MAC_PCCH_MSG_REQ
******************************************************************************/

/* eMTC changes start */
#define RRC_MAC_PAGING_NARROWBAND_PRESENT                0x01
#define RRC_MAC_MPDCCH_NUM_REPETITION_PAGING_PRESENT     0x02
#define RRC_MAC_PDSCH_NUM_REPETITION_PRESENT             0x04
#define RRC_MAC_DIRECT_INDICATION_PRESENT                0x08
/* eMTC changes stop */

typedef struct _rrc_mac_pcch_msg_req_t
{
    /* eMTC changes start */
    RrcBitmaskU8_t                      bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    /* eMTC changes stop */

    rrc_sfn_t   paging_frame;       
    /*^ M, 0, H, 0, 1023 ^*/

    rrc_sf_t    paging_sub_frame;   
    /*^ M, 0, H, 0, 9 ^*/

    U16         num_of_sfn;         
    /* eMTC changes start */
    /*^ M, 0, B, 1, 32768 ^*/
    /* eMTC changes stop */

    /* eMTC changes start */
    U8                                  isPagingForEmtc;
    /*^ M, 0, H, 0, EMTC_PAGING ^*/

    RrcMacPagingNarrowband_t            pagingNarrowBand;
    /*^ TLV, RRC_MAC_PAGING_NARROWBAND, RRC_MAC_PAGING_NARROWBAND_PRESENT ^*/

    RrcMacMpdcchNumRepetitionPaging_t   mpdcchNumRepetitionPaging;     
    /*^ TLV, RRC_MAC_MPDCCH_NUM_REPETITION_PAGING, RRC_MAC_MPDCCH_NUM_REPETITION_PAGING_PRESENT ^*/
  
    RrcMacPdschNumRepetition_t          pdschNumRepetition; 
    /*^ TLV, RRC_MAC_PDSCH_NUM_REPETITION_PAGING, RRC_MAC_PDSCH_NUM_REPETITION_PRESENT ^*/

    RrcMacDirectIndication_t            directIndication;     
    /*^ TLV, RRC_MAC_DIRECT_INDICATION, RRC_MAC_DIRECT_INDICATION_PRESENT ^*/
  
    /* eMTC changes stop */

    U8          paging_msg_buf[0];  
    /*^ M, 0, OCTET_STRING, TILL_THE_END ^*/


} rrc_mac_pcch_msg_req_t; /*^ API, ONLY_PUP, RRC_MAC_PCCH_MSG_REQ ^*/

/******************************************************************************
*   RRC_MAC_CCCH_MSG_REQ
******************************************************************************/
typedef struct _rrc_mac_ccch_msg_req_t
{
    rrc_rnti_t  rnti;               /*^ M, 0, B, 1, 65523 ^*/
    U8          ccch_msg_buf[0];    /*^ M, 0, OCTET_STRING, TILL_THE_END ^*/
} rrc_mac_ccch_msg_req_t; /*^ API, ONLY_PUP, RRC_MAC_CCCH_MSG_REQ ^*/

/******************************************************************************
*   RRC_MAC_CCCH_MSG_IND
******************************************************************************/
typedef struct _rrc_mac_ccch_msg_ind_t
{
    rrc_rnti_t   rnti;               /*^ M, 0, B, 1, 65523 ^*/
    /* CLPC Start */

    U16         timing_advance;     /*^ M, 0, H, 0, 1282 ^*/
    U8          sinr;               /*^ M, 0, N, 0, 0 ^*/

    /* CLPC Stop */
    U8          ccch_msg_buf[0];    /*^ M, 0, OCTET_STRING, TILL_THE_END ^*/
} rrc_mac_ccch_msg_ind_t; /*^ API, ONLY_PUP, RRC_MAC_CCCH_MSG_IND ^*/


/*eMTC changes start*/
/******************************************************************************
*   RRC_MAC_EMTC_CCCH_MSG_IND
******************************************************************************/
typedef struct _RrcMacEmtcCcchMsgInd_t
{
    rrc_rnti_t              rnti; 
    /*^ M, 0, B, 1, 65523 ^*/
    
    U16                     timing_advance;     
    /*^ M, 0, H, 0, 1282 ^*/

    U8                      sinr;               
    /*^ M, 0, N, 0, 0 ^*/

    U8                      ceLevel;
    /*^ M, 0, H, 0, EMTC_MAX_CE_LEVEL ^*/

    U8                      ccch_msg_buf[0];  
    /*^ M, 0, OCTET_STRING, TILL_THE_END ^*/

}RrcMacEmtcCcchMsgInd_t; /*^ API, ONLY_PUP, RRC_MAC_EMTC_CCCH_MSG_IND ^*/
/*eMTC changes stop*/

/******************************************************************************
*   RRC_MAC_HO_RACH_RESOURCE_REQ
******************************************************************************/
typedef struct _rrc_mac_intra_enb_ho_req_t
{
    U8     enable_intra_enb_ho;   /*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */
} rrc_mac_intra_enb_ho_req_t;

#define RRC_MAC_INTRA_ENB_HO_REQ_PRESENT   0x01

typedef struct _rrc_mac_ho_rach_resource_req_t
{
     rrc_bitmask_t              optional_elems_present;
     /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_ue_index_t   ue_index;
    
    rrc_mac_intra_enb_ho_req_t      intra_enb_ho_req;
/*^ TLV, RRC_MAC_INTRA_ENB_HO_REQ, RRC_MAC_INTRA_ENB_HO_REQ_PRESENT ^*/

} rrc_mac_ho_rach_resource_req_t; /*^ API, RRC_MAC_HO_RACH_RESOURCE_REQ ^*/


/******************************************************************************
*   RRC_MAC_HO_RACH_RESOURCE_RESP
******************************************************************************/
typedef struct _rrc_mac_preamble_info_t
{
    U8   preamble_Idx;   /*^ M, 0, H, 0, 63 ^*/
    U8   prach_mask_Idx;  /*^ M, 0, H, 0, 15 ^*/
}rrc_mac_preamble_info_t;

typedef struct _rrc_mac_crnti_info_t
{
    rrc_rnti_t   crnti;  /*^ M, 0, B, 1, 65523 ^*/
}rrc_mac_crnti_info_t;

#define RRC_MAC_CRNTI_INFO_PRESENT 0x01
#define RRC_MAC_PREAMBLE_INFO_PRESENT 0x02

typedef struct _rrc_mac_ho_rach_resource_resp_t
{
    rrc_bitmask_t    optional_elems_present; 
     /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_ue_index_t   ue_index;
    rrc_response_t   response; 
     /*^ M, 0, H, 0, 1 ^*/ /* rrc_mac_return_et */

    rrc_mac_crnti_info_t         crnti_info;  
    /*^ TLV, RRC_MAC_CRNTI_INFO, RRC_MAC_CRNTI_INFO_PRESENT ^*/

    rrc_mac_preamble_info_t      preamble_info; 
     /*^ TLV, RRC_MAC_PREAMBLE_INFO, RRC_MAC_PREAMBLE_INFO_PRESENT ^*/
}rrc_mac_ho_rach_resource_resp_t; /*^ API, RRC_MAC_HO_RACH_RESOURCE_RESP ^*/


/******************************************************************************
*   RRC_MAC_HO_REL_RACH_RESOURCE_IND
******************************************************************************/
typedef struct _rrc_mac_ho_rel_rach_resource_ind_t
{
    rrc_bitmask_t          optional_elems_present; 
     /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_ue_index_t         ue_index;
    rrc_mac_crnti_info_t   crnti_info;
     /*^ TLV, RRC_MAC_CRNTI_INFO, RRC_MAC_CRNTI_INFO_PRESENT ^*/
}rrc_mac_ho_rel_rach_resource_ind_t; /*^ API, RRC_MAC_HO_REL_RACH_RESOURCE_IND ^*/


/*SPR_19066_START*/
/* Code Removed */
/*SPR_19066_END*/

/******************************************************************************
*  RRC_MAC_RESET_UE_ENTITY_REQ 
******************************************************************************/

#define RRC_MAC_RESET_UE_ENTITY_SCELL_REL_FLAG_PRESENT 0x01

typedef struct _rrc_mac_reset_ue_entity_req_t
{
     rrc_ue_index_t  ue_index; 

     rrc_bitmask_t    optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

     rrc_mac_scell_rel_flag_t  rrc_mac_scell_rel_flag;
/*^ TLV, RRC_MAC_RESET_UE_ENTITY_SCELL_REL_FLAG, RRC_MAC_RESET_UE_ENTITY_SCELL_REL_FLAG_PRESENT ^*/
} rrc_mac_reset_ue_entity_req_t;/*^ API, RRC_MAC_RESET_UE_ENTITY_REQ ^*/

/******************************************************************************
*   RRC_MAC_RESET_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_mac_reset_ue_entity_cnf_t
{
    rrc_ue_index_t  ue_index; 
    rrc_response_t  response;     
} rrc_mac_reset_ue_entity_cnf_t; /*^ API, RRC_MAC_RESET_UE_ENTITY_CNF ^*/

/******************************************************************************
*   RRC_MAC_RADIO_LINK_FAILURE_IND
******************************************************************************/
typedef struct _rrc_mac_radio_link_failure_ind_t
{
     rrc_ue_index_t  ue_index;
     U16             rlf_cause; /* rrc_mac_rlf_cause_et */ 
} rrc_mac_radio_link_failure_ind_t;/*^ API, RRC_MAC_RADIO_LINK_FAILURE_IND ^*/

/******************************************************************************
*   RRC_MAC_UE_SYNC_STATUS_IND
******************************************************************************/
typedef struct _rrc_mac_ue_sync_status_ind_t
{
    rrc_ue_index_t  ue_index;
    rrc_rnti_t      crnti; /*^ M, 0, B, 1 , 65523 ^*/
    U8              ue_sync_status; /*^ M, 0, H, 0, 3 ^*/ 
                                   /* rrc_ue_sync_status_et */
} rrc_mac_ue_sync_status_ind_t; /*^ API, RRC_MAC_UE_SYNC_STATUS_IND ^*/

/******************************************************************************
**  RRC_MAC_CHANGE_CRNTI_REQ
*******************************************************************************/
typedef struct _rrc_mac_change_crnti_req_t
{
    rrc_ue_index_t  ue_index;
    rrc_rnti_t      old_crnti;
    rrc_rnti_t      new_crnti;
} rrc_mac_change_crnti_req_t; /*^ API, RRC_MAC_CHANGE_CRNTI_REQ ^*/

/******************************************************************************
**  RRC_MAC_CHANGE_CRNTI_CNF
*******************************************************************************/
typedef struct _rrc_mac_change_crnti_cnf_t
{
    rrc_ue_index_t  ue_index;

    U16             response;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_mac_return_et */

} rrc_mac_change_crnti_cnf_t; /*^ API, RRC_MAC_CHANGE_CRNTI_CNF ^*/


/*SPR_19066_START*/
/* Code Removed */
/*SPR_19066_END*/


/******************************************************************************
 *    RRC_MAC_CELL_START_CNF
 ***************************************************************************/
#define RRC_MAC_CELL_START_CNF_SFN_INFO_PRESENT     0x01

typedef struct _rrc_mac_cell_start_cnf_t
{
    rrc_bitmask_t      optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rrc_response_t     response;   /*^ M, 0, H, 0, 1 ^*/ /* rrc_mac_return_et */
    rrc_mac_sfn_t          sfn;
  /*^ TLV, RRC_MAC_SFN_INFO, RRC_MAC_CELL_START_CNF_SFN_INFO_PRESENT ^*/

}rrc_mac_cell_start_cnf_t;  /*^ API, RRC_MAC_CELL_START_CNF ^*/

/* SPS Start */
typedef struct _rrc_mac_reconfig_complete_ind_t
{
    rrc_ue_index_t  ue_index;

    U8             response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_mac_return_et */
}rrc_mac_reconfig_complete_ind_t;  /*^ API, RRC_MAC_RECONFIG_COMPLETE_IND ^*/

/* SPS Stop */


#pragma pack(pop)

#endif /* _RRC_MAC_INTF_H_ */

