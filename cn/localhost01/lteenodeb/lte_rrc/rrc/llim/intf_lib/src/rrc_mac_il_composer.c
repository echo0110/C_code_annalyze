/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2014 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: $
 ****************************************************************************
 *
 *  File Description : The file rrc_mac_il_composer.c contains the definitions 
 *                     of rrc-mac interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#include "rrc_cp_common.h"
#include "rrc_mac_il_composer.h"
#include "rrc_mac_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_length_t
rrc_il_get_rrc_mac_tdd_config_len
(
    rrc_mac_tdd_config_t *p_rrc_mac_tdd_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_tdd_config_tlv_len
(
    rrc_mac_tdd_config_t *p_rrc_mac_tdd_config

);

static
rrc_return_et
rrc_il_compose_rrc_mac_tdd_config
(
    U8  **pp_buffer,
    rrc_mac_tdd_config_t *p_rrc_mac_tdd_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_preamble_group_a_config_len
(
    rrc_mac_preamble_group_a_config_t *p_rrc_mac_preamble_group_a_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_preamble_group_a_config_tlv_len
(
    rrc_mac_preamble_group_a_config_t *p_rrc_mac_preamble_group_a_config

);

static
rrc_return_et
rrc_il_compose_rrc_mac_preamble_group_a_config
(
    U8  **pp_buffer,
    rrc_mac_preamble_group_a_config_t *p_rrc_mac_preamble_group_a_config
);

static
rrc_length_t
rrc_il_get_RrcMacRachCeLevelInfo_len
(
    RrcMacRachCeLevelInfo_t *p_RrcMacRachCeLevelInfo
);

static
rrc_length_t
rrc_il_get_RrcMacRachCeLevelInfo_tlv_len
(
    RrcMacRachCeLevelInfo_t *p_RrcMacRachCeLevelInfo

);

static
rrc_return_et
rrc_il_compose_RrcMacRachCeLevelInfo
(
    U8  **pp_buffer,
    RrcMacRachCeLevelInfo_t *p_RrcMacRachCeLevelInfo
);

static
rrc_length_t
rrc_il_get_RrcMacRachConfigurationR13_len
(
    RrcMacRachConfigurationR13_t *p_RrcMacRachConfigurationR13
);

static
rrc_length_t
rrc_il_get_RrcMacRachConfigurationR13_tlv_len
(
    RrcMacRachConfigurationR13_t *p_RrcMacRachConfigurationR13

);

static
rrc_return_et
rrc_il_compose_RrcMacRachConfigurationR13
(
    U8  **pp_buffer,
    RrcMacRachConfigurationR13_t *p_RrcMacRachConfigurationR13
);

static
rrc_length_t
rrc_il_get_rrc_mac_rach_config_info_len
(
    rrc_mac_rach_config_info_t *p_rrc_mac_rach_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_rach_config_info_tlv_len
(
    rrc_mac_rach_config_info_t *p_rrc_mac_rach_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_rach_config_info
(
    U8  **pp_buffer,
    rrc_mac_rach_config_info_t *p_rrc_mac_rach_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_phich_config_info_len
(
    rrc_mac_phich_config_info_t *p_rrc_mac_phich_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_phich_config_info_tlv_len
(
    rrc_mac_phich_config_info_t *p_rrc_mac_phich_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_phich_config_info
(
    U8  **pp_buffer,
    rrc_mac_phich_config_info_t *p_rrc_mac_phich_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_config_info_len
(
    rrc_mac_pucch_config_info_t *p_rrc_mac_pucch_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_config_info_tlv_len
(
    rrc_mac_pucch_config_info_t *p_rrc_mac_pucch_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_pucch_config_info
(
    U8  **pp_buffer,
    rrc_mac_pucch_config_info_t *p_rrc_mac_pucch_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_rbs_for_dci_1a_len
(
    rrc_mac_rbs_for_dci_1a_t *p_rrc_mac_rbs_for_dci_1a
);

static
rrc_length_t
rrc_il_get_rrc_mac_rbs_for_dci_1a_tlv_len
(
    rrc_mac_rbs_for_dci_1a_t *p_rrc_mac_rbs_for_dci_1a

);

static
rrc_return_et
rrc_il_compose_rrc_mac_rbs_for_dci_1a
(
    U8  **pp_buffer,
    rrc_mac_rbs_for_dci_1a_t *p_rrc_mac_rbs_for_dci_1a
);

static
rrc_length_t
rrc_il_get_rrc_mac_rbs_for_dci_1c_len
(
    rrc_mac_rbs_for_dci_1c_t *p_rrc_mac_rbs_for_dci_1c
);

static
rrc_length_t
rrc_il_get_rrc_mac_rbs_for_dci_1c_tlv_len
(
    rrc_mac_rbs_for_dci_1c_t *p_rrc_mac_rbs_for_dci_1c

);

static
rrc_return_et
rrc_il_compose_rrc_mac_rbs_for_dci_1c
(
    U8  **pp_buffer,
    rrc_mac_rbs_for_dci_1c_t *p_rrc_mac_rbs_for_dci_1c
);

static
rrc_length_t
rrc_il_get_rrc_mac_mib_msg_req_len
(
    rrc_mac_mib_msg_req_t *p_rrc_mac_mib_msg_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_mib_msg_req_tlv_len
(
    rrc_mac_mib_msg_req_t *p_rrc_mac_mib_msg_req

);

static
rrc_return_et
rrc_il_compose_rrc_mac_mib_msg_req
(
    U8  **pp_buffer,
    rrc_mac_mib_msg_req_t *p_rrc_mac_mib_msg_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_sibtype1_msg_req_len
(
    rrc_mac_sibtype1_msg_req_t *p_rrc_mac_sibtype1_msg_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_sibtype1_msg_req_tlv_len
(
    rrc_mac_sibtype1_msg_req_t *p_rrc_mac_sibtype1_msg_req

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sibtype1_msg_req
(
    U8  **pp_buffer,
    rrc_mac_sibtype1_msg_req_t *p_rrc_mac_sibtype1_msg_req
);

static
rrc_length_t
rrc_il_get_rrc_num_si_repetitions_len
(
    rrc_num_si_repetitions *p_rrc_num_si_repetitions
);

static
rrc_length_t
rrc_il_get_rrc_num_si_repetitions_tlv_len
(
    rrc_num_si_repetitions *p_rrc_num_si_repetitions

);

static
rrc_return_et
rrc_il_compose_rrc_num_si_repetitions
(
    U8  **pp_buffer,
    rrc_num_si_repetitions *p_rrc_num_si_repetitions
);

static
rrc_length_t
rrc_il_get_rrc_num_si_transmissions_len
(
    rrc_num_si_transmissions_t *p_rrc_num_si_transmissions
);

static
rrc_length_t
rrc_il_get_rrc_num_si_transmissions_tlv_len
(
    rrc_num_si_transmissions_t *p_rrc_num_si_transmissions

);

static
rrc_return_et
rrc_il_compose_rrc_num_si_transmissions
(
    U8  **pp_buffer,
    rrc_num_si_transmissions_t *p_rrc_num_si_transmissions
);

static
rrc_length_t
rrc_il_get_rrc_mac_si_start_offset_info_len
(
    rrc_mac_si_start_offset_info_t *p_rrc_mac_si_start_offset_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_si_start_offset_info_tlv_len
(
    rrc_mac_si_start_offset_info_t *p_rrc_mac_si_start_offset_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_si_start_offset_info
(
    U8  **pp_buffer,
    rrc_mac_si_start_offset_info_t *p_rrc_mac_si_start_offset_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_si_segment_data_len
(
    rrc_mac_si_segment_data_t *p_rrc_mac_si_segment_data
);

static
rrc_length_t
rrc_il_get_rrc_mac_si_segment_data_tlv_len
(
    rrc_mac_si_segment_data_t *p_rrc_mac_si_segment_data

);

static
rrc_return_et
rrc_il_compose_rrc_mac_si_segment_data
(
    U8  **pp_buffer,
    rrc_mac_si_segment_data_t *p_rrc_mac_si_segment_data
);

static
rrc_length_t
rrc_il_get_rrc_mac_si_msg_param_len
(
    rrc_mac_si_msg_param_t *p_rrc_mac_si_msg_param
);

static
rrc_length_t
rrc_il_get_rrc_mac_si_msg_param_tlv_len
(
    rrc_mac_si_msg_param_t *p_rrc_mac_si_msg_param

);

static
rrc_return_et
rrc_il_compose_rrc_mac_si_msg_param
(
    U8  **pp_buffer,
    rrc_mac_si_msg_param_t *p_rrc_mac_si_msg_param
);

static
rrc_length_t
rrc_il_get_rrc_mac_sched_reqd_for_sib8_info_len
(
    rrc_mac_sched_reqd_for_sib8_info_t *p_rrc_mac_sched_reqd_for_sib8_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sched_reqd_for_sib8_info_tlv_len
(
    rrc_mac_sched_reqd_for_sib8_info_t *p_rrc_mac_sched_reqd_for_sib8_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sched_reqd_for_sib8_info
(
    U8  **pp_buffer,
    rrc_mac_sched_reqd_for_sib8_info_t *p_rrc_mac_sched_reqd_for_sib8_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_si_msg_info_len
(
    rrc_mac_si_msg_info_t *p_rrc_mac_si_msg_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_si_msg_info_tlv_len
(
    rrc_mac_si_msg_info_t *p_rrc_mac_si_msg_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_si_msg_info
(
    U8  **pp_buffer,
    rrc_mac_si_msg_info_t *p_rrc_mac_si_msg_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_si_msg_req_len
(
    rrc_mac_si_msg_req_t *p_rrc_mac_si_msg_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_si_msg_req_tlv_len
(
    rrc_mac_si_msg_req_t *p_rrc_mac_si_msg_req

);

static
rrc_return_et
rrc_il_compose_rrc_mac_si_msg_req
(
    U8  **pp_buffer,
    rrc_mac_si_msg_req_t *p_rrc_mac_si_msg_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_srs_max_uppts_len
(
    rrc_mac_srs_max_uppts_t *p_rrc_mac_srs_max_uppts
);

static
rrc_length_t
rrc_il_get_rrc_mac_srs_max_uppts_tlv_len
(
    rrc_mac_srs_max_uppts_t *p_rrc_mac_srs_max_uppts

);

static
rrc_return_et
rrc_il_compose_rrc_mac_srs_max_uppts
(
    U8  **pp_buffer,
    rrc_mac_srs_max_uppts_t *p_rrc_mac_srs_max_uppts
);

static
rrc_length_t
rrc_il_get_rrc_mac_srs_common_setup_info_len
(
    rrc_mac_srs_common_setup_info_t *p_rrc_mac_srs_common_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_srs_common_setup_info_tlv_len
(
    rrc_mac_srs_common_setup_info_t *p_rrc_mac_srs_common_setup_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_srs_common_setup_info
(
    U8  **pp_buffer,
    rrc_mac_srs_common_setup_info_t *p_rrc_mac_srs_common_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_srs_common_config_len
(
    rrc_mac_srs_common_config_t *p_rrc_mac_srs_common_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_srs_common_config_tlv_len
(
    rrc_mac_srs_common_config_t *p_rrc_mac_srs_common_config

);

static
rrc_return_et
rrc_il_compose_rrc_mac_srs_common_config
(
    U8  **pp_buffer,
    rrc_mac_srs_common_config_t *p_rrc_mac_srs_common_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_pdsch_config_common_info_len
(
    rrc_mac_pdsch_config_common_info_t *p_rrc_mac_pdsch_config_common_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_pdsch_config_common_info_tlv_len
(
    rrc_mac_pdsch_config_common_info_t *p_rrc_mac_pdsch_config_common_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_pdsch_config_common_info
(
    U8  **pp_buffer,
    rrc_mac_pdsch_config_common_info_t *p_rrc_mac_pdsch_config_common_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_downlink_power_control_common_len
(
    rrc_mac_downlink_power_control_common_t *p_rrc_mac_downlink_power_control_common
);

static
rrc_length_t
rrc_il_get_rrc_mac_downlink_power_control_common_tlv_len
(
    rrc_mac_downlink_power_control_common_t *p_rrc_mac_downlink_power_control_common

);

static
rrc_return_et
rrc_il_compose_rrc_mac_downlink_power_control_common
(
    U8  **pp_buffer,
    rrc_mac_downlink_power_control_common_t *p_rrc_mac_downlink_power_control_common
);

static
rrc_length_t
rrc_il_get_rrc_mac_uplink_power_control_common_len
(
    rrc_mac_uplink_power_control_common_t *p_rrc_mac_uplink_power_control_common
);

static
rrc_length_t
rrc_il_get_rrc_mac_uplink_power_control_common_tlv_len
(
    rrc_mac_uplink_power_control_common_t *p_rrc_mac_uplink_power_control_common

);

static
rrc_return_et
rrc_il_compose_rrc_mac_uplink_power_control_common
(
    U8  **pp_buffer,
    rrc_mac_uplink_power_control_common_t *p_rrc_mac_uplink_power_control_common
);

static
rrc_length_t
rrc_il_get_rrc_mac_pusch_config_common_info_len
(
    rrc_mac_pusch_config_common_info_t *p_rrc_mac_pusch_config_common_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_pusch_config_common_info_tlv_len
(
    rrc_mac_pusch_config_common_info_t *p_rrc_mac_pusch_config_common_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_pusch_config_common_info
(
    U8  **pp_buffer,
    rrc_mac_pusch_config_common_info_t *p_rrc_mac_pusch_config_common_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_freq_selective_scheduling_len
(
    rrc_mac_freq_selective_scheduling_t *p_rrc_mac_freq_selective_scheduling
);

static
rrc_length_t
rrc_il_get_rrc_mac_freq_selective_scheduling_tlv_len
(
    rrc_mac_freq_selective_scheduling_t *p_rrc_mac_freq_selective_scheduling

);

static
rrc_return_et
rrc_il_compose_rrc_mac_freq_selective_scheduling
(
    U8  **pp_buffer,
    rrc_mac_freq_selective_scheduling_t *p_rrc_mac_freq_selective_scheduling
);

static
rrc_length_t
rrc_il_get_rrc_mac_contn_free_rach_timer_info_len
(
    rrc_mac_contn_free_rach_timer_info_t *p_rrc_mac_contn_free_rach_timer_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_contn_free_rach_timer_info_tlv_len
(
    rrc_mac_contn_free_rach_timer_info_t *p_rrc_mac_contn_free_rach_timer_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_contn_free_rach_timer_info
(
    U8  **pp_buffer,
    rrc_mac_contn_free_rach_timer_info_t *p_rrc_mac_contn_free_rach_timer_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_rnti_range_len
(
    rrc_mac_tpc_rnti_range_t *p_rrc_mac_tpc_rnti_range
);

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_rnti_range_tlv_len
(
    rrc_mac_tpc_rnti_range_t *p_rrc_mac_tpc_rnti_range

);

static
rrc_return_et
rrc_il_compose_rrc_mac_tpc_rnti_range
(
    U8  **pp_buffer,
    rrc_mac_tpc_rnti_range_t *p_rrc_mac_tpc_rnti_range
);

static
rrc_length_t
rrc_il_get_rrc_mac_power_control_enable_len
(
    rrc_mac_power_control_enable_t *p_rrc_mac_power_control_enable
);

static
rrc_length_t
rrc_il_get_rrc_mac_power_control_enable_tlv_len
(
    rrc_mac_power_control_enable_t *p_rrc_mac_power_control_enable

);

static
rrc_return_et
rrc_il_compose_rrc_mac_power_control_enable
(
    U8  **pp_buffer,
    rrc_mac_power_control_enable_t *p_rrc_mac_power_control_enable
);

static
rrc_length_t
rrc_il_get_rrc_mac_explicit_cell_start_required_len
(
    rrc_mac_explicit_cell_start_required_t *p_rrc_mac_explicit_cell_start_required
);

static
rrc_length_t
rrc_il_get_rrc_mac_explicit_cell_start_required_tlv_len
(
    rrc_mac_explicit_cell_start_required_t *p_rrc_mac_explicit_cell_start_required

);

static
rrc_return_et
rrc_il_compose_rrc_mac_explicit_cell_start_required
(
    U8  **pp_buffer,
    rrc_mac_explicit_cell_start_required_t *p_rrc_mac_explicit_cell_start_required
);

static
rrc_length_t
rrc_il_get_rrc_mac_ul_sync_loss_timer_info_len
(
    rrc_mac_ul_sync_loss_timer_info_t *p_rrc_mac_ul_sync_loss_timer_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_ul_sync_loss_timer_info_tlv_len
(
    rrc_mac_ul_sync_loss_timer_info_t *p_rrc_mac_ul_sync_loss_timer_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ul_sync_loss_timer_info
(
    U8  **pp_buffer,
    rrc_mac_ul_sync_loss_timer_info_t *p_rrc_mac_ul_sync_loss_timer_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_cqi_sinr_threshold_info_len
(
    rrc_mac_pucch_cqi_sinr_threshold_info_t *p_rrc_mac_pucch_cqi_sinr_threshold_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_cqi_sinr_threshold_info_tlv_len
(
    rrc_mac_pucch_cqi_sinr_threshold_info_t *p_rrc_mac_pucch_cqi_sinr_threshold_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_pucch_cqi_sinr_threshold_info
(
    U8  **pp_buffer,
    rrc_mac_pucch_cqi_sinr_threshold_info_t *p_rrc_mac_pucch_cqi_sinr_threshold_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_n_gap_len
(
    rrc_mac_n_gap_t *p_rrc_mac_n_gap
);

static
rrc_length_t
rrc_il_get_rrc_mac_n_gap_tlv_len
(
    rrc_mac_n_gap_t *p_rrc_mac_n_gap

);

static
rrc_return_et
rrc_il_compose_rrc_mac_n_gap
(
    U8  **pp_buffer,
    rrc_mac_n_gap_t *p_rrc_mac_n_gap
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_crnti_range_len
(
    rrc_mac_sps_crnti_range_t *p_rrc_mac_sps_crnti_range
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_crnti_range_tlv_len
(
    rrc_mac_sps_crnti_range_t *p_rrc_mac_sps_crnti_range

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_crnti_range
(
    U8  **pp_buffer,
    rrc_mac_sps_crnti_range_t *p_rrc_mac_sps_crnti_range
);

static
rrc_length_t
rrc_il_get_rrc_mac_dynamic_pdcch_info_len
(
    rrc_mac_dynamic_pdcch_info_t *p_rrc_mac_dynamic_pdcch_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_dynamic_pdcch_info_tlv_len
(
    rrc_mac_dynamic_pdcch_info_t *p_rrc_mac_dynamic_pdcch_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_dynamic_pdcch_info
(
    U8  **pp_buffer,
    rrc_mac_dynamic_pdcch_info_t *p_rrc_mac_dynamic_pdcch_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_prs_bandwidth_info_len
(
    rrc_mac_prs_bandwidth_info_t *p_rrc_mac_prs_bandwidth_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_prs_bandwidth_info_tlv_len
(
    rrc_mac_prs_bandwidth_info_t *p_rrc_mac_prs_bandwidth_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_prs_bandwidth_info
(
    U8  **pp_buffer,
    rrc_mac_prs_bandwidth_info_t *p_rrc_mac_prs_bandwidth_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_prs_subframes_info_len
(
    rrc_mac_prs_subframes_info_t *p_rrc_mac_prs_subframes_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_prs_subframes_info_tlv_len
(
    rrc_mac_prs_subframes_info_t *p_rrc_mac_prs_subframes_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_prs_subframes_info
(
    U8  **pp_buffer,
    rrc_mac_prs_subframes_info_t *p_rrc_mac_prs_subframes_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_prs_config_index_info_len
(
    rrc_mac_prs_config_index_info_t *p_rrc_mac_prs_config_index_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_prs_config_index_info_tlv_len
(
    rrc_mac_prs_config_index_info_t *p_rrc_mac_prs_config_index_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_prs_config_index_info
(
    U8  **pp_buffer,
    rrc_mac_prs_config_index_info_t *p_rrc_mac_prs_config_index_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_prs_transmission_power_info_len
(
    rrc_mac_prs_transmission_power_info_t *p_rrc_mac_prs_transmission_power_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_prs_transmission_power_info_tlv_len
(
    rrc_mac_prs_transmission_power_info_t *p_rrc_mac_prs_transmission_power_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_prs_transmission_power_info
(
    U8  **pp_buffer,
    rrc_mac_prs_transmission_power_info_t *p_rrc_mac_prs_transmission_power_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_prs_muting_config_info_len
(
    rrc_mac_prs_muting_config_info_t *p_rrc_mac_prs_muting_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_prs_muting_config_info_tlv_len
(
    rrc_mac_prs_muting_config_info_t *p_rrc_mac_prs_muting_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_prs_muting_config_info
(
    U8  **pp_buffer,
    rrc_mac_prs_muting_config_info_t *p_rrc_mac_prs_muting_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_prs_config_info_len
(
    rrc_mac_prs_config_info_t *p_rrc_mac_prs_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_prs_config_info_tlv_len
(
    rrc_mac_prs_config_info_t *p_rrc_mac_prs_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_prs_config_info
(
    U8  **pp_buffer,
    rrc_mac_prs_config_info_t *p_rrc_mac_prs_config_info
);

static
rrc_length_t
rrc_il_get_m2ap_mac_csa_pattern_list_len
(
    m2ap_mac_csa_pattern_list_t *p_m2ap_mac_csa_pattern_list
);

static
rrc_length_t
rrc_il_get_m2ap_mac_csa_pattern_list_tlv_len
(
    m2ap_mac_csa_pattern_list_t *p_m2ap_mac_csa_pattern_list

);

static
rrc_return_et
rrc_il_compose_m2ap_mac_csa_pattern_list
(
    U8  **pp_buffer,
    m2ap_mac_csa_pattern_list_t *p_m2ap_mac_csa_pattern_list
);

static
rrc_length_t
rrc_il_get_rrc_m2ap_embms_sib2_info_len
(
    rrc_m2ap_embms_sib2_info_t *p_rrc_m2ap_embms_sib2_info
);

static
rrc_length_t
rrc_il_get_rrc_m2ap_embms_sib2_info_tlv_len
(
    rrc_m2ap_embms_sib2_info_t *p_rrc_m2ap_embms_sib2_info

);

static
rrc_return_et
rrc_il_compose_rrc_m2ap_embms_sib2_info
(
    U8  **pp_buffer,
    rrc_m2ap_embms_sib2_info_t *p_rrc_m2ap_embms_sib2_info
);

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_embms_sib13_info_len
(
    rrc_m2ap_mac_embms_sib13_info_t *p_rrc_m2ap_mac_embms_sib13_info
);

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_embms_sib13_info_tlv_len
(
    rrc_m2ap_mac_embms_sib13_info_t *p_rrc_m2ap_mac_embms_sib13_info

);

static
rrc_return_et
rrc_il_compose_rrc_m2ap_mac_embms_sib13_info
(
    U8  **pp_buffer,
    rrc_m2ap_mac_embms_sib13_info_t *p_rrc_m2ap_mac_embms_sib13_info
);

static
rrc_length_t
rrc_il_get_m2ap_embms_notification_config_len
(
    m2ap_embms_notification_config_t *p_m2ap_embms_notification_config
);

static
rrc_length_t
rrc_il_get_m2ap_embms_notification_config_tlv_len
(
    m2ap_embms_notification_config_t *p_m2ap_embms_notification_config

);

static
rrc_return_et
rrc_il_compose_m2ap_embms_notification_config
(
    U8  **pp_buffer,
    m2ap_embms_notification_config_t *p_m2ap_embms_notification_config
);

static
rrc_length_t
rrc_il_get_m2ap_mac_mcch_config_len
(
    m2ap_mac_mcch_config_t *p_m2ap_mac_mcch_config
);

static
rrc_length_t
rrc_il_get_m2ap_mac_mcch_config_tlv_len
(
    m2ap_mac_mcch_config_t *p_m2ap_mac_mcch_config

);

static
rrc_return_et
rrc_il_compose_m2ap_mac_mcch_config
(
    U8  **pp_buffer,
    m2ap_mac_mcch_config_t *p_m2ap_mac_mcch_config
);

static
rrc_length_t
rrc_il_get_m2ap_mbms_create_area_info_len
(
    m2ap_mbms_create_area_info_t *p_m2ap_mbms_create_area_info
);

static
rrc_length_t
rrc_il_get_m2ap_mbms_create_area_info_tlv_len
(
    m2ap_mbms_create_area_info_t *p_m2ap_mbms_create_area_info

);

static
rrc_return_et
rrc_il_compose_m2ap_mbms_create_area_info
(
    U8  **pp_buffer,
    m2ap_mbms_create_area_info_t *p_m2ap_mbms_create_area_info
);

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_create_embms_area_entity_info_len
(
    rrc_m2ap_mac_create_embms_area_entity_info_t *p_rrc_m2ap_mac_create_embms_area_entity_info
);

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_create_embms_area_entity_info_tlv_len
(
    rrc_m2ap_mac_create_embms_area_entity_info_t *p_rrc_m2ap_mac_create_embms_area_entity_info

);

static
rrc_return_et
rrc_il_compose_rrc_m2ap_mac_create_embms_area_entity_info
(
    U8  **pp_buffer,
    rrc_m2ap_mac_create_embms_area_entity_info_t *p_rrc_m2ap_mac_create_embms_area_entity_info
);

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_delete_embms_area_entity_info_len
(
    rrc_m2ap_mac_delete_embms_area_entity_info_t *p_rrc_m2ap_mac_delete_embms_area_entity_info
);

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_delete_embms_area_entity_info_tlv_len
(
    rrc_m2ap_mac_delete_embms_area_entity_info_t *p_rrc_m2ap_mac_delete_embms_area_entity_info

);

static
rrc_return_et
rrc_il_compose_rrc_m2ap_mac_delete_embms_area_entity_info
(
    U8  **pp_buffer,
    rrc_m2ap_mac_delete_embms_area_entity_info_t *p_rrc_m2ap_mac_delete_embms_area_entity_info
);

static
rrc_length_t
rrc_il_get_m2ap_mac_embms_transmission_power_info_len
(
    m2ap_mac_embms_transmission_power_info_t *p_m2ap_mac_embms_transmission_power_info
);

static
rrc_length_t
rrc_il_get_m2ap_mac_embms_transmission_power_info_tlv_len
(
    m2ap_mac_embms_transmission_power_info_t *p_m2ap_mac_embms_transmission_power_info

);

static
rrc_return_et
rrc_il_compose_m2ap_mac_embms_transmission_power_info
(
    U8  **pp_buffer,
    m2ap_mac_embms_transmission_power_info_t *p_m2ap_mac_embms_transmission_power_info
);

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_embms_params_info_len
(
    rrc_m2ap_mac_embms_params_info_t *p_rrc_m2ap_mac_embms_params_info
);

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_embms_params_info_tlv_len
(
    rrc_m2ap_mac_embms_params_info_t *p_rrc_m2ap_mac_embms_params_info

);

static
rrc_return_et
rrc_il_compose_rrc_m2ap_mac_embms_params_info
(
    U8  **pp_buffer,
    rrc_m2ap_mac_embms_params_info_t *p_rrc_m2ap_mac_embms_params_info
);

static
rrc_length_t
rrc_il_get_rrc_radio_res_dedicated_laa_scell_config_len
(
    rrc_radio_res_dedicated_laa_scell_config_t *p_rrc_radio_res_dedicated_laa_scell_config
);

static
rrc_length_t
rrc_il_get_rrc_radio_res_dedicated_laa_scell_config_tlv_len
(
    rrc_radio_res_dedicated_laa_scell_config_t *p_rrc_radio_res_dedicated_laa_scell_config

);

static
rrc_return_et
rrc_il_compose_rrc_radio_res_dedicated_laa_scell_config
(
    U8  **pp_buffer,
    rrc_radio_res_dedicated_laa_scell_config_t *p_rrc_radio_res_dedicated_laa_scell_config
);

static
rrc_length_t
rrc_il_get_rrc_dmtc_configuration_r13_len
(
    rrc_dmtc_configuration_r13_t *p_rrc_dmtc_configuration_r13
);

static
rrc_length_t
rrc_il_get_rrc_dmtc_configuration_r13_tlv_len
(
    rrc_dmtc_configuration_r13_t *p_rrc_dmtc_configuration_r13

);

static
rrc_return_et
rrc_il_compose_rrc_dmtc_configuration_r13
(
    U8  **pp_buffer,
    rrc_dmtc_configuration_r13_t *p_rrc_dmtc_configuration_r13
);

static
rrc_length_t
rrc_il_get_RrcEmtcDataConfig_len
(
    RrcEmtcDataConfig_t *p_RrcEmtcDataConfig
);

static
rrc_length_t
rrc_il_get_RrcEmtcDataConfig_tlv_len
(
    RrcEmtcDataConfig_t *p_RrcEmtcDataConfig

);

static
rrc_return_et
rrc_il_compose_RrcEmtcDataConfig
(
    U8  **pp_buffer,
    RrcEmtcDataConfig_t *p_RrcEmtcDataConfig
);

static
rrc_length_t
rrc_il_get_RrcPrachStartSfR13_len
(
    RrcPrachStartSfR13_t *p_RrcPrachStartSfR13
);

static
rrc_length_t
rrc_il_get_RrcPrachStartSfR13_tlv_len
(
    RrcPrachStartSfR13_t *p_RrcPrachStartSfR13

);

static
rrc_return_et
rrc_il_compose_RrcPrachStartSfR13
(
    U8  **pp_buffer,
    RrcPrachStartSfR13_t *p_RrcPrachStartSfR13
);

static
rrc_length_t
rrc_il_get_RrcMaxNumPreambleAttemptCeR13_len
(
    RrcMaxNumPreambleAttemptCeR13_t *p_RrcMaxNumPreambleAttemptCeR13
);

static
rrc_length_t
rrc_il_get_RrcMaxNumPreambleAttemptCeR13_tlv_len
(
    RrcMaxNumPreambleAttemptCeR13_t *p_RrcMaxNumPreambleAttemptCeR13

);

static
rrc_return_et
rrc_il_compose_RrcMaxNumPreambleAttemptCeR13
(
    U8  **pp_buffer,
    RrcMaxNumPreambleAttemptCeR13_t *p_RrcMaxNumPreambleAttemptCeR13
);

static
rrc_length_t
rrc_il_get_RrcPrachParametersCeR13_len
(
    RrcPrachParametersCeR13_t *p_RrcPrachParametersCeR13
);

static
rrc_length_t
rrc_il_get_RrcPrachParametersCeR13_tlv_len
(
    RrcPrachParametersCeR13_t *p_RrcPrachParametersCeR13

);

static
rrc_return_et
rrc_il_compose_RrcPrachParametersCeR13
(
    U8  **pp_buffer,
    RrcPrachParametersCeR13_t *p_RrcPrachParametersCeR13
);

static
rrc_length_t
rrc_il_get_RrcPrachHoppingOffsetR13_len
(
    RrcPrachHoppingOffsetR13_t *p_RrcPrachHoppingOffsetR13
);

static
rrc_length_t
rrc_il_get_RrcPrachHoppingOffsetR13_tlv_len
(
    RrcPrachHoppingOffsetR13_t *p_RrcPrachHoppingOffsetR13

);

static
rrc_return_et
rrc_il_compose_RrcPrachHoppingOffsetR13
(
    U8  **pp_buffer,
    RrcPrachHoppingOffsetR13_t *p_RrcPrachHoppingOffsetR13
);

static
rrc_length_t
rrc_il_get_RrcMacPrachConfigV1310_len
(
    RrcMacPrachConfigV1310_t *p_RrcMacPrachConfigV1310
);

static
rrc_length_t
rrc_il_get_RrcMacPrachConfigV1310_tlv_len
(
    RrcMacPrachConfigV1310_t *p_RrcMacPrachConfigV1310

);

static
rrc_return_et
rrc_il_compose_RrcMacPrachConfigV1310
(
    U8  **pp_buffer,
    RrcMacPrachConfigV1310_t *p_RrcMacPrachConfigV1310
);

static
rrc_length_t
rrc_il_get_RrcMacSibtype1BrMsgInfo_len
(
    RrcMacSibtype1BrMsgInfo_t *p_RrcMacSibtype1BrMsgInfo
);

static
rrc_length_t
rrc_il_get_RrcMacSibtype1BrMsgInfo_tlv_len
(
    RrcMacSibtype1BrMsgInfo_t *p_RrcMacSibtype1BrMsgInfo

);

static
rrc_return_et
rrc_il_compose_RrcMacSibtype1BrMsgInfo
(
    U8  **pp_buffer,
    RrcMacSibtype1BrMsgInfo_t *p_RrcMacSibtype1BrMsgInfo
);

static
rrc_length_t
rrc_il_get_RrcMacSiMsgsBr_len
(
    RrcMacSiMsgsBr_t *p_RrcMacSiMsgsBr
);

static
rrc_length_t
rrc_il_get_RrcMacSiMsgsBr_tlv_len
(
    RrcMacSiMsgsBr_t *p_RrcMacSiMsgsBr

);

static
rrc_return_et
rrc_il_compose_RrcMacSiMsgsBr
(
    U8  **pp_buffer,
    RrcMacSiMsgsBr_t *p_RrcMacSiMsgsBr
);

static
rrc_length_t
rrc_il_get_RrcMacFddDownlinkOrTddSfBitmapBr_len
(
    RrcMacFddDownlinkOrTddSfBitmapBr_t *p_RrcMacFddDownlinkOrTddSfBitmapBr
);

static
rrc_length_t
rrc_il_get_RrcMacFddDownlinkOrTddSfBitmapBr_tlv_len
(
    RrcMacFddDownlinkOrTddSfBitmapBr_t *p_RrcMacFddDownlinkOrTddSfBitmapBr

);

static
rrc_return_et
rrc_il_compose_RrcMacFddDownlinkOrTddSfBitmapBr
(
    U8  **pp_buffer,
    RrcMacFddDownlinkOrTddSfBitmapBr_t *p_RrcMacFddDownlinkOrTddSfBitmapBr
);

static
rrc_length_t
rrc_il_get_RrcMacFddUplinkSfBitmapBr_len
(
    RrcMacFddUplinkSfBitmapBr_t *p_RrcMacFddUplinkSfBitmapBr
);

static
rrc_length_t
rrc_il_get_RrcMacFddUplinkSfBitmapBr_tlv_len
(
    RrcMacFddUplinkSfBitmapBr_t *p_RrcMacFddUplinkSfBitmapBr

);

static
rrc_return_et
rrc_il_compose_RrcMacFddUplinkSfBitmapBr
(
    U8  **pp_buffer,
    RrcMacFddUplinkSfBitmapBr_t *p_RrcMacFddUplinkSfBitmapBr
);

static
rrc_length_t
rrc_il_get_RrcMacSiMsgBrInfo_len
(
    RrcMacSiMsgBrInfo_t *p_RrcMacSiMsgBrInfo
);

static
rrc_length_t
rrc_il_get_RrcMacSiMsgBrInfo_tlv_len
(
    RrcMacSiMsgBrInfo_t *p_RrcMacSiMsgBrInfo

);

static
rrc_return_et
rrc_il_compose_RrcMacSiMsgBrInfo
(
    U8  **pp_buffer,
    RrcMacSiMsgBrInfo_t *p_RrcMacSiMsgBrInfo
);

static
rrc_length_t
rrc_il_get_RrcN1PucchAnInfoListR13_len
(
    RrcN1PucchAnInfoListR13_t *p_RrcN1PucchAnInfoListR13
);

static
rrc_length_t
rrc_il_get_RrcN1PucchAnInfoListR13_tlv_len
(
    RrcN1PucchAnInfoListR13_t *p_RrcN1PucchAnInfoListR13

);

static
rrc_return_et
rrc_il_compose_RrcN1PucchAnInfoListR13
(
    U8  **pp_buffer,
    RrcN1PucchAnInfoListR13_t *p_RrcN1PucchAnInfoListR13
);

static
rrc_length_t
rrc_il_get_RrcMacPucchConfigCommonV1310_len
(
    RrcMacPucchConfigCommonV1310_t *p_RrcMacPucchConfigCommonV1310
);

static
rrc_length_t
rrc_il_get_RrcMacPucchConfigCommonV1310_tlv_len
(
    RrcMacPucchConfigCommonV1310_t *p_RrcMacPucchConfigCommonV1310

);

static
rrc_return_et
rrc_il_compose_RrcMacPucchConfigCommonV1310
(
    U8  **pp_buffer,
    RrcMacPucchConfigCommonV1310_t *p_RrcMacPucchConfigCommonV1310
);

static
rrc_length_t
rrc_il_get_RrcMacModificationPeriodV1310_len
(
    RrcMacModificationPeriodV1310_t *p_RrcMacModificationPeriodV1310
);

static
rrc_length_t
rrc_il_get_RrcMacModificationPeriodV1310_tlv_len
(
    RrcMacModificationPeriodV1310_t *p_RrcMacModificationPeriodV1310

);

static
rrc_return_et
rrc_il_compose_RrcMacModificationPeriodV1310
(
    U8  **pp_buffer,
    RrcMacModificationPeriodV1310_t *p_RrcMacModificationPeriodV1310
);

static
rrc_length_t
rrc_il_get_RrcMacPcchConfigV1310_len
(
    RrcMacPcchConfigV1310_t *p_RrcMacPcchConfigV1310
);

static
rrc_length_t
rrc_il_get_RrcMacPcchConfigV1310_tlv_len
(
    RrcMacPcchConfigV1310_t *p_RrcMacPcchConfigV1310

);

static
rrc_return_et
rrc_il_compose_RrcMacPcchConfigV1310
(
    U8  **pp_buffer,
    RrcMacPcchConfigV1310_t *p_RrcMacPcchConfigV1310
);

static
rrc_length_t
rrc_il_get_RrcMacPdschConfigCommonV1310_len
(
    RrcMacPdschConfigCommonV1310_t *p_RrcMacPdschConfigCommonV1310
);

static
rrc_length_t
rrc_il_get_RrcMacPdschConfigCommonV1310_tlv_len
(
    RrcMacPdschConfigCommonV1310_t *p_RrcMacPdschConfigCommonV1310

);

static
rrc_return_et
rrc_il_compose_RrcMacPdschConfigCommonV1310
(
    U8  **pp_buffer,
    RrcMacPdschConfigCommonV1310_t *p_RrcMacPdschConfigCommonV1310
);

static
rrc_length_t
rrc_il_get_RrcMacPuschConfigCommonV1310_len
(
    RrcMacPuschConfigCommonV1310_t *p_RrcMacPuschConfigCommonV1310
);

static
rrc_length_t
rrc_il_get_RrcMacPuschConfigCommonV1310_tlv_len
(
    RrcMacPuschConfigCommonV1310_t *p_RrcMacPuschConfigCommonV1310

);

static
rrc_return_et
rrc_il_compose_RrcMacPuschConfigCommonV1310
(
    U8  **pp_buffer,
    RrcMacPuschConfigCommonV1310_t *p_RrcMacPuschConfigCommonV1310
);

static
rrc_length_t
rrc_il_get_RrcFrequancyHoppingParametersR13_len
(
    RrcFrequancyHoppingParametersR13_t *p_RrcFrequancyHoppingParametersR13
);

static
rrc_length_t
rrc_il_get_RrcFrequancyHoppingParametersR13_tlv_len
(
    RrcFrequancyHoppingParametersR13_t *p_RrcFrequancyHoppingParametersR13

);

static
rrc_return_et
rrc_il_compose_RrcFrequancyHoppingParametersR13
(
    U8  **pp_buffer,
    RrcFrequancyHoppingParametersR13_t *p_RrcFrequancyHoppingParametersR13
);

static
rrc_length_t
rrc_il_get_rrc_mac_config_cell_error_code_len
(
    rrc_mac_config_cell_error_code_t *p_rrc_mac_config_cell_error_code
);

static
rrc_length_t
rrc_il_get_rrc_mac_config_cell_error_code_tlv_len
(
    rrc_mac_config_cell_error_code_t *p_rrc_mac_config_cell_error_code

);

static
rrc_return_et
rrc_il_compose_rrc_mac_config_cell_error_code
(
    U8  **pp_buffer,
    rrc_mac_config_cell_error_code_t *p_rrc_mac_config_cell_error_code
);

static
rrc_length_t
rrc_il_get_rrc_dl_earfcn_len
(
    rrc_dl_earfcn_t *p_rrc_dl_earfcn
);

static
rrc_length_t
rrc_il_get_rrc_dl_earfcn_tlv_len
(
    rrc_dl_earfcn_t *p_rrc_dl_earfcn

);

static
rrc_return_et
rrc_il_compose_rrc_dl_earfcn
(
    U8  **pp_buffer,
    rrc_dl_earfcn_t *p_rrc_dl_earfcn
);

static
rrc_length_t
rrc_il_get_rrc_mac_timestamp_reqd_len
(
    rrc_mac_timestamp_reqd_t *p_rrc_mac_timestamp_reqd
);

static
rrc_length_t
rrc_il_get_rrc_mac_timestamp_reqd_tlv_len
(
    rrc_mac_timestamp_reqd_t *p_rrc_mac_timestamp_reqd

);

static
rrc_return_et
rrc_il_compose_rrc_mac_timestamp_reqd
(
    U8  **pp_buffer,
    rrc_mac_timestamp_reqd_t *p_rrc_mac_timestamp_reqd
);

static
rrc_length_t
rrc_il_get_rrc_mac_sfn_len
(
    rrc_mac_sfn_t *p_rrc_mac_sfn
);

static
rrc_length_t
rrc_il_get_rrc_mac_sfn_tlv_len
(
    rrc_mac_sfn_t *p_rrc_mac_sfn

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sfn
(
    U8  **pp_buffer,
    rrc_mac_sfn_t *p_rrc_mac_sfn
);

static
rrc_length_t
rrc_il_get_rrc_mac_sf_len
(
    rrc_mac_sf_t *p_rrc_mac_sf
);

static
rrc_length_t
rrc_il_get_rrc_mac_sf_tlv_len
(
    rrc_mac_sf_t *p_rrc_mac_sf

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sf
(
    U8  **pp_buffer,
    rrc_mac_sf_t *p_rrc_mac_sf
);

static
rrc_length_t
rrc_il_get_rrc_timestamp_len
(
    rrc_timestamp_t *p_rrc_timestamp
);

static
rrc_length_t
rrc_il_get_rrc_timestamp_tlv_len
(
    rrc_timestamp_t *p_rrc_timestamp

);

static
rrc_return_et
rrc_il_compose_rrc_timestamp
(
    U8  **pp_buffer,
    rrc_timestamp_t *p_rrc_timestamp
);

static
rrc_length_t
rrc_il_get_rrc_mac_dci_format_for_si_len
(
    rrc_mac_dci_format_for_si_t *p_rrc_mac_dci_format_for_si
);

static
rrc_length_t
rrc_il_get_rrc_mac_dci_format_for_si_tlv_len
(
    rrc_mac_dci_format_for_si_t *p_rrc_mac_dci_format_for_si

);

static
rrc_return_et
rrc_il_compose_rrc_mac_dci_format_for_si
(
    U8  **pp_buffer,
    rrc_mac_dci_format_for_si_t *p_rrc_mac_dci_format_for_si
);

static
rrc_length_t
rrc_il_get_rrc_mac_mod_period_info_len
(
    rrc_mac_mod_period_info_t *p_rrc_mac_mod_period_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_mod_period_info_tlv_len
(
    rrc_mac_mod_period_info_t *p_rrc_mac_mod_period_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_mod_period_info
(
    U8  **pp_buffer,
    rrc_mac_mod_period_info_t *p_rrc_mac_mod_period_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sfn_gap_info_len
(
    rrc_mac_sfn_gap_info_t *p_rrc_mac_sfn_gap_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sfn_gap_info_tlv_len
(
    rrc_mac_sfn_gap_info_t *p_rrc_mac_sfn_gap_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sfn_gap_info
(
    U8  **pp_buffer,
    rrc_mac_sfn_gap_info_t *p_rrc_mac_sfn_gap_info
);

static
rrc_length_t
rrc_il_get_RrcMacEmtcSfn_len
(
    RrcMacEmtcSfn_t *p_RrcMacEmtcSfn
);

static
rrc_length_t
rrc_il_get_RrcMacEmtcSfn_tlv_len
(
    RrcMacEmtcSfn_t *p_RrcMacEmtcSfn

);

static
rrc_return_et
rrc_il_compose_RrcMacEmtcSfn
(
    U8  **pp_buffer,
    RrcMacEmtcSfn_t *p_RrcMacEmtcSfn
);

static
rrc_length_t
rrc_il_get_RrcMacSfnGapInfoV1310_len
(
    RrcMacSfnGapInfoV1310_t *p_RrcMacSfnGapInfoV1310
);

static
rrc_length_t
rrc_il_get_RrcMacSfnGapInfoV1310_tlv_len
(
    RrcMacSfnGapInfoV1310_t *p_RrcMacSfnGapInfoV1310

);

static
rrc_return_et
rrc_il_compose_RrcMacSfnGapInfoV1310
(
    U8  **pp_buffer,
    RrcMacSfnGapInfoV1310_t *p_RrcMacSfnGapInfoV1310
);

static
rrc_length_t
rrc_il_get_rrc_mac_sr_setup_info_len
(
    rrc_mac_sr_setup_info_t *p_rrc_mac_sr_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sr_setup_info_tlv_len
(
    rrc_mac_sr_setup_info_t *p_rrc_mac_sr_setup_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sr_setup_info
(
    U8  **pp_buffer,
    rrc_mac_sr_setup_info_t *p_rrc_mac_sr_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sr_config_info_len
(
    rrc_mac_sr_config_info_t *p_rrc_mac_sr_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sr_config_info_tlv_len
(
    rrc_mac_sr_config_info_t *p_rrc_mac_sr_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sr_config_info
(
    U8  **pp_buffer,
    rrc_mac_sr_config_info_t *p_rrc_mac_sr_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sr_prohbit_timer_value_v920_len
(
    rrc_mac_sr_prohbit_timer_value_v920_t *p_rrc_mac_sr_prohbit_timer_value_v920
);

static
rrc_length_t
rrc_il_get_rrc_mac_sr_prohbit_timer_value_v920_tlv_len
(
    rrc_mac_sr_prohbit_timer_value_v920_t *p_rrc_mac_sr_prohbit_timer_value_v920

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sr_prohbit_timer_value_v920
(
    U8  **pp_buffer,
    rrc_mac_sr_prohbit_timer_value_v920_t *p_rrc_mac_sr_prohbit_timer_value_v920
);

static
rrc_length_t
rrc_il_get_aperiodic_csi_trigger_r10_len
(
    aperiodic_csi_trigger_r10_t *p_aperiodic_csi_trigger_r10
);

static
rrc_length_t
rrc_il_get_aperiodic_csi_trigger_r10_tlv_len
(
    aperiodic_csi_trigger_r10_t *p_aperiodic_csi_trigger_r10

);

static
rrc_return_et
rrc_il_compose_aperiodic_csi_trigger_r10
(
    U8  **pp_buffer,
    aperiodic_csi_trigger_r10_t *p_aperiodic_csi_trigger_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_aperiodic_config_info_len
(
    rrc_mac_cqi_aperiodic_config_info_t *p_rrc_mac_cqi_aperiodic_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_aperiodic_config_info_tlv_len
(
    rrc_mac_cqi_aperiodic_config_info_t *p_rrc_mac_cqi_aperiodic_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_aperiodic_config_info
(
    U8  **pp_buffer,
    rrc_mac_cqi_aperiodic_config_info_t *p_rrc_mac_cqi_aperiodic_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_fmt_ind_perdiodic_subband_k_len
(
    rrc_mac_cqi_fmt_ind_perdiodic_subband_k_t *p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_fmt_ind_perdiodic_subband_k_tlv_len
(
    rrc_mac_cqi_fmt_ind_perdiodic_subband_k_t *p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k

);

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_fmt_ind_perdiodic_subband_k
(
    U8  **pp_buffer,
    rrc_mac_cqi_fmt_ind_perdiodic_subband_k_t *p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k
);

static
rrc_length_t
rrc_il_get_rrc_mac_ri_config_index_info_len
(
    rrc_mac_ri_config_index_info_t *p_rrc_mac_ri_config_index_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_ri_config_index_info_tlv_len
(
    rrc_mac_ri_config_index_info_t *p_rrc_mac_ri_config_index_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ri_config_index_info
(
    U8  **pp_buffer,
    rrc_mac_ri_config_index_info_t *p_rrc_mac_ri_config_index_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_pmi_config_index_info_len
(
    rrc_mac_cqi_pmi_config_index_info_t *p_rrc_mac_cqi_pmi_config_index_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_pmi_config_index_info_tlv_len
(
    rrc_mac_cqi_pmi_config_index_info_t *p_rrc_mac_cqi_pmi_config_index_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_pmi_config_index_info
(
    U8  **pp_buffer,
    rrc_mac_cqi_pmi_config_index_info_t *p_rrc_mac_cqi_pmi_config_index_info
);

static
rrc_length_t
rrc_il_get_cqi_pmi_config_index2_r10_len
(
    cqi_pmi_config_index2_r10_t *p_cqi_pmi_config_index2_r10
);

static
rrc_length_t
rrc_il_get_cqi_pmi_config_index2_r10_tlv_len
(
    cqi_pmi_config_index2_r10_t *p_cqi_pmi_config_index2_r10

);

static
rrc_return_et
rrc_il_compose_cqi_pmi_config_index2_r10
(
    U8  **pp_buffer,
    cqi_pmi_config_index2_r10_t *p_cqi_pmi_config_index2_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_csi_config_index_v10_len
(
    rrc_mac_csi_config_index_v10_t *p_rrc_mac_csi_config_index_v10
);

static
rrc_length_t
rrc_il_get_rrc_mac_csi_config_index_v10_tlv_len
(
    rrc_mac_csi_config_index_v10_t *p_rrc_mac_csi_config_index_v10

);

static
rrc_return_et
rrc_il_compose_rrc_mac_csi_config_index_v10
(
    U8  **pp_buffer,
    rrc_mac_csi_config_index_v10_t *p_rrc_mac_csi_config_index_v10
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_pmi_config_index_info_v1020_len
(
    rrc_mac_cqi_pmi_config_index_info_v1020_t *p_rrc_mac_cqi_pmi_config_index_info_v1020
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_pmi_config_index_info_v1020_tlv_len
(
    rrc_mac_cqi_pmi_config_index_info_v1020_t *p_rrc_mac_cqi_pmi_config_index_info_v1020

);

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_pmi_config_index_info_v1020
(
    U8  **pp_buffer,
    rrc_mac_cqi_pmi_config_index_info_v1020_t *p_rrc_mac_cqi_pmi_config_index_info_v1020
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_periodic_config_info_len
(
    rrc_mac_cqi_periodic_config_info_t *p_rrc_mac_cqi_periodic_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_periodic_config_info_tlv_len
(
    rrc_mac_cqi_periodic_config_info_t *p_rrc_mac_cqi_periodic_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_periodic_config_info
(
    U8  **pp_buffer,
    rrc_mac_cqi_periodic_config_info_t *p_rrc_mac_cqi_periodic_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_mask_r9_len
(
    rrc_mac_cqi_mask_r9_t *p_rrc_mac_cqi_mask_r9
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_mask_r9_tlv_len
(
    rrc_mac_cqi_mask_r9_t *p_rrc_mac_cqi_mask_r9

);

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_mask_r9
(
    U8  **pp_buffer,
    rrc_mac_cqi_mask_r9_t *p_rrc_mac_cqi_mask_r9
);

static
rrc_length_t
rrc_il_get_rrc_mac_pmi_ri_report_r9_len
(
    rrc_mac_pmi_ri_report_r9_t *p_rrc_mac_pmi_ri_report_r9
);

static
rrc_length_t
rrc_il_get_rrc_mac_pmi_ri_report_r9_tlv_len
(
    rrc_mac_pmi_ri_report_r9_t *p_rrc_mac_pmi_ri_report_r9

);

static
rrc_return_et
rrc_il_compose_rrc_mac_pmi_ri_report_r9
(
    U8  **pp_buffer,
    rrc_mac_pmi_ri_report_r9_t *p_rrc_mac_pmi_ri_report_r9
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_report_config_v920_len
(
    rrc_mac_cqi_report_config_v920_t *p_rrc_mac_cqi_report_config_v920
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_report_config_v920_tlv_len
(
    rrc_mac_cqi_report_config_v920_t *p_rrc_mac_cqi_report_config_v920

);

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_report_config_v920
(
    U8  **pp_buffer,
    rrc_mac_cqi_report_config_v920_t *p_rrc_mac_cqi_report_config_v920
);

static
rrc_length_t
rrc_il_get_rrc_mac_csi_meas_subframe_set1_r10_len
(
    rrc_mac_csi_meas_subframe_set1_r10_t *p_rrc_mac_csi_meas_subframe_set1_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_csi_meas_subframe_set1_r10_tlv_len
(
    rrc_mac_csi_meas_subframe_set1_r10_t *p_rrc_mac_csi_meas_subframe_set1_r10

);

static
rrc_return_et
rrc_il_compose_rrc_mac_csi_meas_subframe_set1_r10
(
    U8  **pp_buffer,
    rrc_mac_csi_meas_subframe_set1_r10_t *p_rrc_mac_csi_meas_subframe_set1_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_csi_meas_subframe_set2_r10_len
(
    rrc_mac_csi_meas_subframe_set2_r10_t *p_rrc_mac_csi_meas_subframe_set2_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_csi_meas_subframe_set2_r10_tlv_len
(
    rrc_mac_csi_meas_subframe_set2_r10_t *p_rrc_mac_csi_meas_subframe_set2_r10

);

static
rrc_return_et
rrc_il_compose_rrc_mac_csi_meas_subframe_set2_r10
(
    U8  **pp_buffer,
    rrc_mac_csi_meas_subframe_set2_r10_t *p_rrc_mac_csi_meas_subframe_set2_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_csi_subframe_pattern_config_v10_len
(
    rrc_mac_csi_subframe_pattern_config_v10_t *p_rrc_mac_csi_subframe_pattern_config_v10
);

static
rrc_length_t
rrc_il_get_rrc_mac_csi_subframe_pattern_config_v10_tlv_len
(
    rrc_mac_csi_subframe_pattern_config_v10_t *p_rrc_mac_csi_subframe_pattern_config_v10

);

static
rrc_return_et
rrc_il_compose_rrc_mac_csi_subframe_pattern_config_v10
(
    U8  **pp_buffer,
    rrc_mac_csi_subframe_pattern_config_v10_t *p_rrc_mac_csi_subframe_pattern_config_v10
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_info_len
(
    rrc_mac_cqi_info_t *p_rrc_mac_cqi_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_info_tlv_len
(
    rrc_mac_cqi_info_t *p_rrc_mac_cqi_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_info
(
    U8  **pp_buffer,
    rrc_mac_cqi_info_t *p_rrc_mac_cqi_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_tdd_ack_nack_feedback_mode_len
(
    rrc_mac_tdd_ack_nack_feedback_mode_t *p_rrc_mac_tdd_ack_nack_feedback_mode
);

static
rrc_length_t
rrc_il_get_rrc_mac_tdd_ack_nack_feedback_mode_tlv_len
(
    rrc_mac_tdd_ack_nack_feedback_mode_t *p_rrc_mac_tdd_ack_nack_feedback_mode

);

static
rrc_return_et
rrc_il_compose_rrc_mac_tdd_ack_nack_feedback_mode
(
    U8  **pp_buffer,
    rrc_mac_tdd_ack_nack_feedback_mode_t *p_rrc_mac_tdd_ack_nack_feedback_mode
);

static
rrc_length_t
rrc_il_get_rrc_mac_pusch_config_info_len
(
    rrc_mac_pusch_config_info_t *p_rrc_mac_pusch_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_pusch_config_info_tlv_len
(
    rrc_mac_pusch_config_info_t *p_rrc_mac_pusch_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_pusch_config_info
(
    U8  **pp_buffer,
    rrc_mac_pusch_config_info_t *p_rrc_mac_pusch_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_srs_dedicated_setup_info_len
(
    rrc_mac_srs_dedicated_setup_info_t *p_rrc_mac_srs_dedicated_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_srs_dedicated_setup_info_tlv_len
(
    rrc_mac_srs_dedicated_setup_info_t *p_rrc_mac_srs_dedicated_setup_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_srs_dedicated_setup_info
(
    U8  **pp_buffer,
    rrc_mac_srs_dedicated_setup_info_t *p_rrc_mac_srs_dedicated_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_srs_dedicated_config_len
(
    rrc_mac_srs_dedicated_config_t *p_rrc_mac_srs_dedicated_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_srs_dedicated_config_tlv_len
(
    rrc_mac_srs_dedicated_config_t *p_rrc_mac_srs_dedicated_config

);

static
rrc_return_et
rrc_il_compose_rrc_mac_srs_dedicated_config
(
    U8  **pp_buffer,
    rrc_mac_srs_dedicated_config_t *p_rrc_mac_srs_dedicated_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_codebook_subset_restriction_len
(
    rrc_mac_codebook_subset_restriction_t *p_rrc_mac_codebook_subset_restriction
);

static
rrc_length_t
rrc_il_get_rrc_mac_codebook_subset_restriction_tlv_len
(
    rrc_mac_codebook_subset_restriction_t *p_rrc_mac_codebook_subset_restriction

);

static
rrc_return_et
rrc_il_compose_rrc_mac_codebook_subset_restriction
(
    U8  **pp_buffer,
    rrc_mac_codebook_subset_restriction_t *p_rrc_mac_codebook_subset_restriction
);

static
rrc_length_t
rrc_il_get_rrc_mac_short_drx_config_len
(
    rrc_mac_short_drx_config_t *p_rrc_mac_short_drx_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_short_drx_config_tlv_len
(
    rrc_mac_short_drx_config_t *p_rrc_mac_short_drx_config

);

static
rrc_return_et
rrc_il_compose_rrc_mac_short_drx_config
(
    U8  **pp_buffer,
    rrc_mac_short_drx_config_t *p_rrc_mac_short_drx_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_cmd_l2_trigger_info_len
(
    rrc_mac_cmd_l2_trigger_info_t *p_rrc_mac_cmd_l2_trigger_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_cmd_l2_trigger_info_tlv_len
(
    rrc_mac_cmd_l2_trigger_info_t *p_rrc_mac_cmd_l2_trigger_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_cmd_l2_trigger_info
(
    U8  **pp_buffer,
    rrc_mac_cmd_l2_trigger_info_t *p_rrc_mac_cmd_l2_trigger_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_drx_setup_config_info_len
(
    rrc_mac_drx_setup_config_info_t *p_rrc_mac_drx_setup_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_drx_setup_config_info_tlv_len
(
    rrc_mac_drx_setup_config_info_t *p_rrc_mac_drx_setup_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_drx_setup_config_info
(
    U8  **pp_buffer,
    rrc_mac_drx_setup_config_info_t *p_rrc_mac_drx_setup_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_drx_config_info_len
(
    rrc_mac_drx_config_info_t *p_rrc_mac_drx_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_drx_config_info_tlv_len
(
    rrc_mac_drx_config_info_t *p_rrc_mac_drx_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_drx_config_info
(
    U8  **pp_buffer,
    rrc_mac_drx_config_info_t *p_rrc_mac_drx_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_meas_gap_setup_config_info_len
(
    rrc_mac_meas_gap_setup_config_info_t *p_rrc_mac_meas_gap_setup_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_meas_gap_setup_config_info_tlv_len
(
    rrc_mac_meas_gap_setup_config_info_t *p_rrc_mac_meas_gap_setup_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_meas_gap_setup_config_info
(
    U8  **pp_buffer,
    rrc_mac_meas_gap_setup_config_info_t *p_rrc_mac_meas_gap_setup_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_meas_gap_config_info_len
(
    rrc_mac_meas_gap_config_info_t *p_rrc_mac_meas_gap_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_meas_gap_config_info_tlv_len
(
    rrc_mac_meas_gap_config_info_t *p_rrc_mac_meas_gap_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_meas_gap_config_info
(
    U8  **pp_buffer,
    rrc_mac_meas_gap_config_info_t *p_rrc_mac_meas_gap_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_codebook_subset_restriction_v920_len
(
    rrc_mac_codebook_subset_restriction_v920_t *p_rrc_mac_codebook_subset_restriction_v920
);

static
rrc_length_t
rrc_il_get_rrc_mac_codebook_subset_restriction_v920_tlv_len
(
    rrc_mac_codebook_subset_restriction_v920_t *p_rrc_mac_codebook_subset_restriction_v920

);

static
rrc_return_et
rrc_il_compose_rrc_mac_codebook_subset_restriction_v920
(
    U8  **pp_buffer,
    rrc_mac_codebook_subset_restriction_v920_t *p_rrc_mac_codebook_subset_restriction_v920
);

static
rrc_length_t
rrc_il_get_rrc_mac_uplink_power_control_dedicated_len
(
    rrc_mac_uplink_power_control_dedicated_t *p_rrc_mac_uplink_power_control_dedicated
);

static
rrc_length_t
rrc_il_get_rrc_mac_uplink_power_control_dedicated_tlv_len
(
    rrc_mac_uplink_power_control_dedicated_t *p_rrc_mac_uplink_power_control_dedicated

);

static
rrc_return_et
rrc_il_compose_rrc_mac_uplink_power_control_dedicated
(
    U8  **pp_buffer,
    rrc_mac_uplink_power_control_dedicated_t *p_rrc_mac_uplink_power_control_dedicated
);

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_rnti_info_len
(
    rrc_mac_tpc_rnti_info_t *p_rrc_mac_tpc_rnti_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_rnti_info_tlv_len
(
    rrc_mac_tpc_rnti_info_t *p_rrc_mac_tpc_rnti_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_tpc_rnti_info
(
    U8  **pp_buffer,
    rrc_mac_tpc_rnti_info_t *p_rrc_mac_tpc_rnti_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_pdcch_config_pucch_len
(
    rrc_mac_tpc_pdcch_config_pucch_t *p_rrc_mac_tpc_pdcch_config_pucch
);

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_pdcch_config_pucch_tlv_len
(
    rrc_mac_tpc_pdcch_config_pucch_t *p_rrc_mac_tpc_pdcch_config_pucch

);

static
rrc_return_et
rrc_il_compose_rrc_mac_tpc_pdcch_config_pucch
(
    U8  **pp_buffer,
    rrc_mac_tpc_pdcch_config_pucch_t *p_rrc_mac_tpc_pdcch_config_pucch
);

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_pdcch_config_pusch_len
(
    rrc_mac_tpc_pdcch_config_pusch_t *p_rrc_mac_tpc_pdcch_config_pusch
);

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_pdcch_config_pusch_tlv_len
(
    rrc_mac_tpc_pdcch_config_pusch_t *p_rrc_mac_tpc_pdcch_config_pusch

);

static
rrc_return_et
rrc_il_compose_rrc_mac_tpc_pdcch_config_pusch
(
    U8  **pp_buffer,
    rrc_mac_tpc_pdcch_config_pusch_t *p_rrc_mac_tpc_pdcch_config_pusch
);

static
rrc_length_t
rrc_il_get_rrc_mac_ambr_qos_info_len
(
    rrc_mac_ambr_qos_info_t *p_rrc_mac_ambr_qos_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_ambr_qos_info_tlv_len
(
    rrc_mac_ambr_qos_info_t *p_rrc_mac_ambr_qos_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ambr_qos_info
(
    U8  **pp_buffer,
    rrc_mac_ambr_qos_info_t *p_rrc_mac_ambr_qos_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_user_location_type_len
(
    rrc_mac_user_location_type_t *p_rrc_mac_user_location_type
);

static
rrc_length_t
rrc_il_get_rrc_mac_user_location_type_tlv_len
(
    rrc_mac_user_location_type_t *p_rrc_mac_user_location_type

);

static
rrc_return_et
rrc_il_compose_rrc_mac_user_location_type
(
    U8  **pp_buffer,
    rrc_mac_user_location_type_t *p_rrc_mac_user_location_type
);

static
rrc_length_t
rrc_il_get_rrc_mac_ue_capability_additional_params_len
(
    rrc_mac_ue_capability_additional_params_t *p_rrc_mac_ue_capability_additional_params
);

static
rrc_length_t
rrc_il_get_rrc_mac_ue_capability_additional_params_tlv_len
(
    rrc_mac_ue_capability_additional_params_t *p_rrc_mac_ue_capability_additional_params

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ue_capability_additional_params
(
    U8  **pp_buffer,
    rrc_mac_ue_capability_additional_params_t *p_rrc_mac_ue_capability_additional_params
);

static
rrc_length_t
rrc_il_get_rrc_mac_n1_pucch_an_persist_list_len
(
    rrc_mac_n1_pucch_an_persist_list_t *p_rrc_mac_n1_pucch_an_persist_list
);

static
rrc_length_t
rrc_il_get_rrc_mac_n1_pucch_an_persist_list_tlv_len
(
    rrc_mac_n1_pucch_an_persist_list_t *p_rrc_mac_n1_pucch_an_persist_list

);

static
rrc_return_et
rrc_il_compose_rrc_mac_n1_pucch_an_persist_list
(
    U8  **pp_buffer,
    rrc_mac_n1_pucch_an_persist_list_t *p_rrc_mac_n1_pucch_an_persist_list
);

static
rrc_length_t
rrc_il_get_rrc_mac_max_sps_harq_retx_len
(
    rrc_mac_max_sps_harq_retx_t *p_rrc_mac_max_sps_harq_retx
);

static
rrc_length_t
rrc_il_get_rrc_mac_max_sps_harq_retx_tlv_len
(
    rrc_mac_max_sps_harq_retx_t *p_rrc_mac_max_sps_harq_retx

);

static
rrc_return_et
rrc_il_compose_rrc_mac_max_sps_harq_retx
(
    U8  **pp_buffer,
    rrc_mac_max_sps_harq_retx_t *p_rrc_mac_max_sps_harq_retx
);

static
rrc_length_t
rrc_il_get_rrc_mac_explicit_release_after_len
(
    rrc_mac_explicit_release_after_t *p_rrc_mac_explicit_release_after
);

static
rrc_length_t
rrc_il_get_rrc_mac_explicit_release_after_tlv_len
(
    rrc_mac_explicit_release_after_t *p_rrc_mac_explicit_release_after

);

static
rrc_return_et
rrc_il_compose_rrc_mac_explicit_release_after
(
    U8  **pp_buffer,
    rrc_mac_explicit_release_after_t *p_rrc_mac_explicit_release_after
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_dl_setup_info_len
(
    rrc_mac_sps_dl_setup_info_t *p_rrc_mac_sps_dl_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_dl_setup_info_tlv_len
(
    rrc_mac_sps_dl_setup_info_t *p_rrc_mac_sps_dl_setup_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_dl_setup_info
(
    U8  **pp_buffer,
    rrc_mac_sps_dl_setup_info_t *p_rrc_mac_sps_dl_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_dl_info_len
(
    rrc_mac_sps_dl_info_t *p_rrc_mac_sps_dl_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_dl_info_tlv_len
(
    rrc_mac_sps_dl_info_t *p_rrc_mac_sps_dl_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_dl_info
(
    U8  **pp_buffer,
    rrc_mac_sps_dl_info_t *p_rrc_mac_sps_dl_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_p_zero_persistent_len
(
    rrc_mac_p_zero_persistent_t *p_rrc_mac_p_zero_persistent
);

static
rrc_length_t
rrc_il_get_rrc_mac_p_zero_persistent_tlv_len
(
    rrc_mac_p_zero_persistent_t *p_rrc_mac_p_zero_persistent

);

static
rrc_return_et
rrc_il_compose_rrc_mac_p_zero_persistent
(
    U8  **pp_buffer,
    rrc_mac_p_zero_persistent_t *p_rrc_mac_p_zero_persistent
);

static
rrc_length_t
rrc_il_get_rrc_mac_two_intervals_config_len
(
    rrc_mac_two_intervals_config_t *p_rrc_mac_two_intervals_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_two_intervals_config_tlv_len
(
    rrc_mac_two_intervals_config_t *p_rrc_mac_two_intervals_config

);

static
rrc_return_et
rrc_il_compose_rrc_mac_two_intervals_config
(
    U8  **pp_buffer,
    rrc_mac_two_intervals_config_t *p_rrc_mac_two_intervals_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_ul_setup_info_len
(
    rrc_mac_sps_ul_setup_info_t *p_rrc_mac_sps_ul_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_ul_setup_info_tlv_len
(
    rrc_mac_sps_ul_setup_info_t *p_rrc_mac_sps_ul_setup_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_ul_setup_info
(
    U8  **pp_buffer,
    rrc_mac_sps_ul_setup_info_t *p_rrc_mac_sps_ul_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_ul_info_len
(
    rrc_mac_sps_ul_info_t *p_rrc_mac_sps_ul_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_ul_info_tlv_len
(
    rrc_mac_sps_ul_info_t *p_rrc_mac_sps_ul_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_ul_info
(
    U8  **pp_buffer,
    rrc_mac_sps_ul_info_t *p_rrc_mac_sps_ul_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_config_len
(
    rrc_mac_sps_config_t *p_rrc_mac_sps_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_config_tlv_len
(
    rrc_mac_sps_config_t *p_rrc_mac_sps_config

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_config
(
    U8  **pp_buffer,
    rrc_mac_sps_config_t *p_rrc_mac_sps_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_rf_params_len
(
    rrc_mac_rf_params_t *p_rrc_mac_rf_params
);

static
rrc_length_t
rrc_il_get_rrc_mac_rf_params_tlv_len
(
    rrc_mac_rf_params_t *p_rrc_mac_rf_params

);

static
rrc_return_et
rrc_il_compose_rrc_mac_rf_params
(
    U8  **pp_buffer,
    rrc_mac_rf_params_t *p_rrc_mac_rf_params
);

static
rrc_length_t
rrc_il_get_rrc_mac_ue_cat_len
(
    rrc_mac_ue_cat_t *p_rrc_mac_ue_cat
);

static
rrc_length_t
rrc_il_get_rrc_mac_ue_cat_tlv_len
(
    rrc_mac_ue_cat_t *p_rrc_mac_ue_cat

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ue_cat
(
    U8  **pp_buffer,
    rrc_mac_ue_cat_t *p_rrc_mac_ue_cat
);

static
rrc_length_t
rrc_il_get_rrc_mac_ul_mu_mimo_info_len
(
    rrc_mac_ul_mu_mimo_info_t *p_rrc_mac_ul_mu_mimo_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_ul_mu_mimo_info_tlv_len
(
    rrc_mac_ul_mu_mimo_info_t *p_rrc_mac_ul_mu_mimo_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ul_mu_mimo_info
(
    U8  **pp_buffer,
    rrc_mac_ul_mu_mimo_info_t *p_rrc_mac_ul_mu_mimo_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_phr_timer_info_len
(
    rrc_mac_phr_timer_info_t *p_rrc_mac_phr_timer_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_phr_timer_info_tlv_len
(
    rrc_mac_phr_timer_info_t *p_rrc_mac_phr_timer_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_phr_timer_info
(
    U8  **pp_buffer,
    rrc_mac_phr_timer_info_t *p_rrc_mac_phr_timer_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_phr_timer_config_len
(
    rrc_mac_phr_timer_config_t *p_rrc_mac_phr_timer_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_phr_timer_config_tlv_len
(
    rrc_mac_phr_timer_config_t *p_rrc_mac_phr_timer_config

);

static
rrc_return_et
rrc_il_compose_rrc_mac_phr_timer_config
(
    U8  **pp_buffer,
    rrc_mac_phr_timer_config_t *p_rrc_mac_phr_timer_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_min_ue_power_len
(
    rrc_mac_min_ue_power_t *p_rrc_mac_min_ue_power
);

static
rrc_length_t
rrc_il_get_rrc_mac_min_ue_power_tlv_len
(
    rrc_mac_min_ue_power_t *p_rrc_mac_min_ue_power

);

static
rrc_return_et
rrc_il_compose_rrc_mac_min_ue_power
(
    U8  **pp_buffer,
    rrc_mac_min_ue_power_t *p_rrc_mac_min_ue_power
);

static
rrc_length_t
rrc_il_get_rrc_mac_ue_compliance_info_len
(
    rrc_mac_ue_compliance_info_t *p_rrc_mac_ue_compliance_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_ue_compliance_info_tlv_len
(
    rrc_mac_ue_compliance_info_t *p_rrc_mac_ue_compliance_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ue_compliance_info
(
    U8  **pp_buffer,
    rrc_mac_ue_compliance_info_t *p_rrc_mac_ue_compliance_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_pdsch_epre_to_ue_rs_ratio_len
(
    rrc_mac_pdsch_epre_to_ue_rs_ratio_t *p_rrc_mac_pdsch_epre_to_ue_rs_ratio
);

static
rrc_length_t
rrc_il_get_rrc_mac_pdsch_epre_to_ue_rs_ratio_tlv_len
(
    rrc_mac_pdsch_epre_to_ue_rs_ratio_t *p_rrc_mac_pdsch_epre_to_ue_rs_ratio

);

static
rrc_return_et
rrc_il_compose_rrc_mac_pdsch_epre_to_ue_rs_ratio
(
    U8  **pp_buffer,
    rrc_mac_pdsch_epre_to_ue_rs_ratio_t *p_rrc_mac_pdsch_epre_to_ue_rs_ratio
);

static
rrc_length_t
rrc_il_get_rrc_mac_tti_bundling_info_len
(
    rrc_mac_tti_bundling_info_t *p_rrc_mac_tti_bundling_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_tti_bundling_info_tlv_len
(
    rrc_mac_tti_bundling_info_t *p_rrc_mac_tti_bundling_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_tti_bundling_info
(
    U8  **pp_buffer,
    rrc_mac_tti_bundling_info_t *p_rrc_mac_tti_bundling_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_operator_info_len
(
    rrc_mac_operator_info_t *p_rrc_mac_operator_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_operator_info_tlv_len
(
    rrc_mac_operator_info_t *p_rrc_mac_operator_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_operator_info
(
    U8  **pp_buffer,
    rrc_mac_operator_info_t *p_rrc_mac_operator_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_codebook_subset_restriction_v1020_len
(
    rrc_mac_codebook_subset_restriction_v1020_t *p_rrc_mac_codebook_subset_restriction_v1020
);

static
rrc_length_t
rrc_il_get_rrc_mac_codebook_subset_restriction_v1020_tlv_len
(
    rrc_mac_codebook_subset_restriction_v1020_t *p_rrc_mac_codebook_subset_restriction_v1020

);

static
rrc_return_et
rrc_il_compose_rrc_mac_codebook_subset_restriction_v1020
(
    U8  **pp_buffer,
    rrc_mac_codebook_subset_restriction_v1020_t *p_rrc_mac_codebook_subset_restriction_v1020
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_interference_info_len
(
    rrc_mac_dl_interference_info_t *p_rrc_mac_dl_interference_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_interference_info_tlv_len
(
    rrc_mac_dl_interference_info_t *p_rrc_mac_dl_interference_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_interference_info
(
    U8  **pp_buffer,
    rrc_mac_dl_interference_info_t *p_rrc_mac_dl_interference_info
);

static
rrc_length_t
rrc_il_get_RrcAckNackRepetitionSetupR13_len
(
    RrcAckNackRepetitionSetupR13_t *p_RrcAckNackRepetitionSetupR13
);

static
rrc_length_t
rrc_il_get_RrcAckNackRepetitionSetupR13_tlv_len
(
    RrcAckNackRepetitionSetupR13_t *p_RrcAckNackRepetitionSetupR13

);

static
rrc_return_et
rrc_il_compose_RrcAckNackRepetitionSetupR13
(
    U8  **pp_buffer,
    RrcAckNackRepetitionSetupR13_t *p_RrcAckNackRepetitionSetupR13
);

static
rrc_length_t
rrc_il_get_RrcMacPucchAckNackRepetitionR13_len
(
    RrcMacPucchAckNackRepetitionR13_t *p_RrcMacPucchAckNackRepetitionR13
);

static
rrc_length_t
rrc_il_get_RrcMacPucchAckNackRepetitionR13_tlv_len
(
    RrcMacPucchAckNackRepetitionR13_t *p_RrcMacPucchAckNackRepetitionR13

);

static
rrc_return_et
rrc_il_compose_RrcMacPucchAckNackRepetitionR13
(
    U8  **pp_buffer,
    RrcMacPucchAckNackRepetitionR13_t *p_RrcMacPucchAckNackRepetitionR13
);

static
rrc_length_t
rrc_il_get_RrcPucchNumRepetitionCeR13SetupModeA_len
(
    RrcPucchNumRepetitionCeR13SetupModeA_t *p_RrcPucchNumRepetitionCeR13SetupModeA
);

static
rrc_length_t
rrc_il_get_RrcPucchNumRepetitionCeR13SetupModeA_tlv_len
(
    RrcPucchNumRepetitionCeR13SetupModeA_t *p_RrcPucchNumRepetitionCeR13SetupModeA

);

static
rrc_return_et
rrc_il_compose_RrcPucchNumRepetitionCeR13SetupModeA
(
    U8  **pp_buffer,
    RrcPucchNumRepetitionCeR13SetupModeA_t *p_RrcPucchNumRepetitionCeR13SetupModeA
);

static
rrc_length_t
rrc_il_get_RrcMacPucchNumRepetitionCeR13_len
(
    RrcMacPucchNumRepetitionCeR13_t *p_RrcMacPucchNumRepetitionCeR13
);

static
rrc_length_t
rrc_il_get_RrcMacPucchNumRepetitionCeR13_tlv_len
(
    RrcMacPucchNumRepetitionCeR13_t *p_RrcMacPucchNumRepetitionCeR13

);

static
rrc_return_et
rrc_il_compose_RrcMacPucchNumRepetitionCeR13
(
    U8  **pp_buffer,
    RrcMacPucchNumRepetitionCeR13_t *p_RrcMacPucchNumRepetitionCeR13
);

static
rrc_length_t
rrc_il_get_PucchConfigDedicatedR13_len
(
    PucchConfigDedicatedR13_t *p_PucchConfigDedicatedR13
);

static
rrc_length_t
rrc_il_get_PucchConfigDedicatedR13_tlv_len
(
    PucchConfigDedicatedR13_t *p_PucchConfigDedicatedR13

);

static
rrc_return_et
rrc_il_compose_PucchConfigDedicatedR13
(
    U8  **pp_buffer,
    PucchConfigDedicatedR13_t *p_PucchConfigDedicatedR13
);

static
rrc_length_t
rrc_il_get_MpdcchConfigSetupR13_len
(
    MpdcchConfigSetupR13_t *p_MpdcchConfigSetupR13
);

static
rrc_length_t
rrc_il_get_MpdcchConfigSetupR13_tlv_len
(
    MpdcchConfigSetupR13_t *p_MpdcchConfigSetupR13

);

static
rrc_return_et
rrc_il_compose_MpdcchConfigSetupR13
(
    U8  **pp_buffer,
    MpdcchConfigSetupR13_t *p_MpdcchConfigSetupR13
);

static
rrc_length_t
rrc_il_get_MpdcchConfigR13_len
(
    MpdcchConfigR13_t *p_MpdcchConfigR13
);

static
rrc_length_t
rrc_il_get_MpdcchConfigR13_tlv_len
(
    MpdcchConfigR13_t *p_MpdcchConfigR13

);

static
rrc_return_et
rrc_il_compose_MpdcchConfigR13
(
    U8  **pp_buffer,
    MpdcchConfigR13_t *p_MpdcchConfigR13
);

static
rrc_length_t
rrc_il_get_EpdcchSetConfigR11_len
(
    EpdcchSetConfigR11_t *p_EpdcchSetConfigR11
);

static
rrc_length_t
rrc_il_get_EpdcchSetConfigR11_tlv_len
(
    EpdcchSetConfigR11_t *p_EpdcchSetConfigR11

);

static
rrc_return_et
rrc_il_compose_EpdcchSetConfigR11
(
    U8  **pp_buffer,
    EpdcchSetConfigR11_t *p_EpdcchSetConfigR11
);

static
rrc_length_t
rrc_il_get_RrcMacCeLevel_len
(
    RrcMacCeLevel_t *p_RrcMacCeLevel
);

static
rrc_length_t
rrc_il_get_RrcMacCeLevel_tlv_len
(
    RrcMacCeLevel_t *p_RrcMacCeLevel

);

static
rrc_return_et
rrc_il_compose_RrcMacCeLevel
(
    U8  **pp_buffer,
    RrcMacCeLevel_t *p_RrcMacCeLevel
);

static
rrc_length_t
rrc_il_get_RrcMacCeMode_len
(
    RrcMacCeMode_t *p_RrcMacCeMode
);

static
rrc_length_t
rrc_il_get_RrcMacCeMode_tlv_len
(
    RrcMacCeMode_t *p_RrcMacCeMode

);

static
rrc_return_et
rrc_il_compose_RrcMacCeMode
(
    U8  **pp_buffer,
    RrcMacCeMode_t *p_RrcMacCeMode
);

static
rrc_length_t
rrc_il_get_rrc_mac_ue_category_v1020_len
(
    rrc_mac_ue_category_v1020_t *p_rrc_mac_ue_category_v1020
);

static
rrc_length_t
rrc_il_get_rrc_mac_ue_category_v1020_tlv_len
(
    rrc_mac_ue_category_v1020_t *p_rrc_mac_ue_category_v1020

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ue_category_v1020
(
    U8  **pp_buffer,
    rrc_mac_ue_category_v1020_t *p_rrc_mac_ue_category_v1020
);

static
rrc_length_t
rrc_il_get_rrc_mac_n1pucch_an_cs_r10_len
(
    rrc_mac_n1pucch_an_cs_r10_t *p_rrc_mac_n1pucch_an_cs_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_n1pucch_an_cs_r10_tlv_len
(
    rrc_mac_n1pucch_an_cs_r10_t *p_rrc_mac_n1pucch_an_cs_r10

);

static
rrc_return_et
rrc_il_compose_rrc_mac_n1pucch_an_cs_r10
(
    U8  **pp_buffer,
    rrc_mac_n1pucch_an_cs_r10_t *p_rrc_mac_n1pucch_an_cs_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_n1pucch_an_cs_list_r10_len
(
    rrc_mac_n1pucch_an_cs_list_r10_t *p_rrc_mac_n1pucch_an_cs_list_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_n1pucch_an_cs_list_r10_tlv_len
(
    rrc_mac_n1pucch_an_cs_list_r10_t *p_rrc_mac_n1pucch_an_cs_list_r10

);

static
rrc_return_et
rrc_il_compose_rrc_mac_n1pucch_an_cs_list_r10
(
    U8  **pp_buffer,
    rrc_mac_n1pucch_an_cs_list_r10_t *p_rrc_mac_n1pucch_an_cs_list_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_channel_selection_config_v1020_len
(
    rrc_mac_pucch_channel_selection_config_v1020_t *p_rrc_mac_pucch_channel_selection_config_v1020
);

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_channel_selection_config_v1020_tlv_len
(
    rrc_mac_pucch_channel_selection_config_v1020_t *p_rrc_mac_pucch_channel_selection_config_v1020

);

static
rrc_return_et
rrc_il_compose_rrc_mac_pucch_channel_selection_config_v1020
(
    U8  **pp_buffer,
    rrc_mac_pucch_channel_selection_config_v1020_t *p_rrc_mac_pucch_channel_selection_config_v1020
);

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_format_r10_len
(
    rrc_mac_pucch_format_r10_t *p_rrc_mac_pucch_format_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_format_r10_tlv_len
(
    rrc_mac_pucch_format_r10_t *p_rrc_mac_pucch_format_r10

);

static
rrc_return_et
rrc_il_compose_rrc_mac_pucch_format_r10
(
    U8  **pp_buffer,
    rrc_mac_pucch_format_r10_t *p_rrc_mac_pucch_format_r10
);

static
rrc_length_t
rrc_il_get_scell_deactivation_timer_len
(
    scell_deactivation_timer_t *p_scell_deactivation_timer
);

static
rrc_length_t
rrc_il_get_scell_deactivation_timer_tlv_len
(
    scell_deactivation_timer_t *p_scell_deactivation_timer

);

static
rrc_return_et
rrc_il_compose_scell_deactivation_timer
(
    U8  **pp_buffer,
    scell_deactivation_timer_t *p_scell_deactivation_timer
);

static
rrc_length_t
rrc_il_get_rrc_mac_supported_mimo_capability_dl_r10_len
(
    rrc_mac_supported_mimo_capability_dl_r10_t *p_rrc_mac_supported_mimo_capability_dl_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_supported_mimo_capability_dl_r10_tlv_len
(
    rrc_mac_supported_mimo_capability_dl_r10_t *p_rrc_mac_supported_mimo_capability_dl_r10

);

static
rrc_return_et
rrc_il_compose_rrc_mac_supported_mimo_capability_dl_r10
(
    U8  **pp_buffer,
    rrc_mac_supported_mimo_capability_dl_r10_t *p_rrc_mac_supported_mimo_capability_dl_r10
);

static
rrc_length_t
rrc_il_get_rrc_mac_tx_mode_info_len
(
    rrc_mac_tx_mode_info_t *p_rrc_mac_tx_mode_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_tx_mode_info_tlv_len
(
    rrc_mac_tx_mode_info_t *p_rrc_mac_tx_mode_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_tx_mode_info
(
    U8  **pp_buffer,
    rrc_mac_tx_mode_info_t *p_rrc_mac_tx_mode_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_pdsch_config_info_len
(
    rrc_mac_pdsch_config_info_t *p_rrc_mac_pdsch_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_pdsch_config_info_tlv_len
(
    rrc_mac_pdsch_config_info_t *p_rrc_mac_pdsch_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_pdsch_config_info
(
    U8  **pp_buffer,
    rrc_mac_pdsch_config_info_t *p_rrc_mac_pdsch_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_num_harq_process_len
(
    rrc_mac_dl_num_harq_process_t *p_rrc_mac_dl_num_harq_process
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_num_harq_process_tlv_len
(
    rrc_mac_dl_num_harq_process_t *p_rrc_mac_dl_num_harq_process

);

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_num_harq_process
(
    U8  **pp_buffer,
    rrc_mac_dl_num_harq_process_t *p_rrc_mac_dl_num_harq_process
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_init_mcs_len
(
    rrc_mac_dl_init_mcs_t *p_rrc_mac_dl_init_mcs
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_init_mcs_tlv_len
(
    rrc_mac_dl_init_mcs_t *p_rrc_mac_dl_init_mcs

);

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_init_mcs
(
    U8  **pp_buffer,
    rrc_mac_dl_init_mcs_t *p_rrc_mac_dl_init_mcs
);

static
rrc_length_t
rrc_il_get_rrc_mac_num_of_layer_info_len
(
    rrc_mac_num_of_layer_info_t *p_rrc_mac_num_of_layer_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_num_of_layer_info_tlv_len
(
    rrc_mac_num_of_layer_info_t *p_rrc_mac_num_of_layer_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_num_of_layer_info
(
    U8  **pp_buffer,
    rrc_mac_num_of_layer_info_t *p_rrc_mac_num_of_layer_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_code_book_index_info_len
(
    rrc_mac_code_book_index_info_t *p_rrc_mac_code_book_index_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_code_book_index_info_tlv_len
(
    rrc_mac_code_book_index_info_t *p_rrc_mac_code_book_index_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_code_book_index_info
(
    U8  **pp_buffer,
    rrc_mac_code_book_index_info_t *p_rrc_mac_code_book_index_info
);

static
rrc_length_t
rrc_il_get_scell_activate_status_len
(
    scell_activate_status_t *p_scell_activate_status
);

static
rrc_length_t
rrc_il_get_scell_activate_status_tlv_len
(
    scell_activate_status_t *p_scell_activate_status

);

static
rrc_return_et
rrc_il_compose_scell_activate_status
(
    U8  **pp_buffer,
    scell_activate_status_t *p_scell_activate_status
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_earfcn_len
(
    rrc_mac_dl_earfcn_t *p_rrc_mac_dl_earfcn
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_earfcn_tlv_len
(
    rrc_mac_dl_earfcn_t *p_rrc_mac_dl_earfcn

);

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_earfcn
(
    U8  **pp_buffer,
    rrc_mac_dl_earfcn_t *p_rrc_mac_dl_earfcn
);

static
rrc_length_t
rrc_il_get_rrc_mac_scell_info_len
(
    rrc_mac_scell_info_t *p_rrc_mac_scell_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_scell_info_tlv_len
(
    rrc_mac_scell_info_t *p_rrc_mac_scell_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_scell_info
(
    U8  **pp_buffer,
    rrc_mac_scell_info_t *p_rrc_mac_scell_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_ca_lc_bandwidth_dist_info_len
(
    rrc_mac_ca_lc_bandwidth_dist_info_t *p_rrc_mac_ca_lc_bandwidth_dist_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_ca_lc_bandwidth_dist_info_tlv_len
(
    rrc_mac_ca_lc_bandwidth_dist_info_t *p_rrc_mac_ca_lc_bandwidth_dist_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ca_lc_bandwidth_dist_info
(
    U8  **pp_buffer,
    rrc_mac_ca_lc_bandwidth_dist_info_t *p_rrc_mac_ca_lc_bandwidth_dist_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_carrier_aggr_config_len
(
    rrc_mac_carrier_aggr_config_t *p_rrc_mac_carrier_aggr_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_carrier_aggr_config_tlv_len
(
    rrc_mac_carrier_aggr_config_t *p_rrc_mac_carrier_aggr_config

);

static
rrc_return_et
rrc_il_compose_rrc_mac_carrier_aggr_config
(
    U8  **pp_buffer,
    rrc_mac_carrier_aggr_config_t *p_rrc_mac_carrier_aggr_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_add_ue_info_len
(
    rrc_mac_add_ue_info_t *p_rrc_mac_add_ue_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_add_ue_info_tlv_len
(
    rrc_mac_add_ue_info_t *p_rrc_mac_add_ue_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_add_ue_info
(
    U8  **pp_buffer,
    rrc_mac_add_ue_info_t *p_rrc_mac_add_ue_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_configure_status_report_required_ue_len
(
    rrc_mac_configure_status_report_required_ue_t *p_rrc_mac_configure_status_report_required_ue
);

static
rrc_length_t
rrc_il_get_rrc_mac_configure_status_report_required_ue_tlv_len
(
    rrc_mac_configure_status_report_required_ue_t *p_rrc_mac_configure_status_report_required_ue

);

static
rrc_return_et
rrc_il_compose_rrc_mac_configure_status_report_required_ue
(
    U8  **pp_buffer,
    rrc_mac_configure_status_report_required_ue_t *p_rrc_mac_configure_status_report_required_ue
);

static
rrc_length_t
rrc_il_get_rrc_mac_ul_lc_create_req_len
(
    rrc_mac_ul_lc_create_req_t *p_rrc_mac_ul_lc_create_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_ul_lc_create_req_tlv_len
(
    rrc_mac_ul_lc_create_req_t *p_rrc_mac_ul_lc_create_req

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ul_lc_create_req
(
    U8  **pp_buffer,
    rrc_mac_ul_lc_create_req_t *p_rrc_mac_ul_lc_create_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_configure_status_report_required_enb_len
(
    rrc_mac_configure_status_report_required_enb_t *p_rrc_mac_configure_status_report_required_enb
);

static
rrc_length_t
rrc_il_get_rrc_mac_configure_status_report_required_enb_tlv_len
(
    rrc_mac_configure_status_report_required_enb_t *p_rrc_mac_configure_status_report_required_enb

);

static
rrc_return_et
rrc_il_compose_rrc_mac_configure_status_report_required_enb
(
    U8  **pp_buffer,
    rrc_mac_configure_status_report_required_enb_t *p_rrc_mac_configure_status_report_required_enb
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_lc_create_req_len
(
    rrc_mac_dl_lc_create_req_t *p_rrc_mac_dl_lc_create_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_lc_create_req_tlv_len
(
    rrc_mac_dl_lc_create_req_t *p_rrc_mac_dl_lc_create_req

);

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_lc_create_req
(
    U8  **pp_buffer,
    rrc_mac_dl_lc_create_req_t *p_rrc_mac_dl_lc_create_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_gbr_qos_info_len
(
    rrc_mac_gbr_qos_info_t *p_rrc_mac_gbr_qos_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_gbr_qos_info_tlv_len
(
    rrc_mac_gbr_qos_info_t *p_rrc_mac_gbr_qos_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_gbr_qos_info
(
    U8  **pp_buffer,
    rrc_mac_gbr_qos_info_t *p_rrc_mac_gbr_qos_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_qos_info_len
(
    rrc_mac_qos_info_t *p_rrc_mac_qos_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_qos_info_tlv_len
(
    rrc_mac_qos_info_t *p_rrc_mac_qos_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_qos_info
(
    U8  **pp_buffer,
    rrc_mac_qos_info_t *p_rrc_mac_qos_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_rohc_profile_len
(
    rrc_mac_rohc_profile_t *p_rrc_mac_rohc_profile
);

static
rrc_length_t
rrc_il_get_rrc_mac_rohc_profile_tlv_len
(
    rrc_mac_rohc_profile_t *p_rrc_mac_rohc_profile

);

static
rrc_return_et
rrc_il_compose_rrc_mac_rohc_profile
(
    U8  **pp_buffer,
    rrc_mac_rohc_profile_t *p_rrc_mac_rohc_profile
);

static
rrc_length_t
rrc_il_get_rrc_mac_create_lc_req_len
(
    rrc_mac_create_lc_req_t *p_rrc_mac_create_lc_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_create_lc_req_tlv_len
(
    rrc_mac_create_lc_req_t *p_rrc_mac_create_lc_req

);

static
rrc_return_et
rrc_il_compose_rrc_mac_create_lc_req
(
    U8  **pp_buffer,
    rrc_mac_create_lc_req_t *p_rrc_mac_create_lc_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_uplink_rat1_info_len
(
    rrc_mac_uplink_rat1_info_t *p_rrc_mac_uplink_rat1_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_uplink_rat1_info_tlv_len
(
    rrc_mac_uplink_rat1_info_t *p_rrc_mac_uplink_rat1_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_uplink_rat1_info
(
    U8  **pp_buffer,
    rrc_mac_uplink_rat1_info_t *p_rrc_mac_uplink_rat1_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_ul_lc_config_resp_len
(
    rrc_mac_ul_lc_config_resp_t *p_rrc_mac_ul_lc_config_resp
);

static
rrc_length_t
rrc_il_get_rrc_mac_ul_lc_config_resp_tlv_len
(
    rrc_mac_ul_lc_config_resp_t *p_rrc_mac_ul_lc_config_resp

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ul_lc_config_resp
(
    U8  **pp_buffer,
    rrc_mac_ul_lc_config_resp_t *p_rrc_mac_ul_lc_config_resp
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_lc_config_resp_len
(
    rrc_mac_dl_lc_config_resp_t *p_rrc_mac_dl_lc_config_resp
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_lc_config_resp_tlv_len
(
    rrc_mac_dl_lc_config_resp_t *p_rrc_mac_dl_lc_config_resp

);

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_lc_config_resp
(
    U8  **pp_buffer,
    rrc_mac_dl_lc_config_resp_t *p_rrc_mac_dl_lc_config_resp
);

static
rrc_length_t
rrc_il_get_rrc_mac_create_lc_error_len
(
    rrc_mac_create_lc_error_t *p_rrc_mac_create_lc_error
);

static
rrc_length_t
rrc_il_get_rrc_mac_create_lc_error_tlv_len
(
    rrc_mac_create_lc_error_t *p_rrc_mac_create_lc_error

);

static
rrc_return_et
rrc_il_compose_rrc_mac_create_lc_error
(
    U8  **pp_buffer,
    rrc_mac_create_lc_error_t *p_rrc_mac_create_lc_error
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_crnti_info_len
(
    rrc_mac_sps_crnti_info_t *p_rrc_mac_sps_crnti_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_sps_crnti_info_tlv_len
(
    rrc_mac_sps_crnti_info_t *p_rrc_mac_sps_crnti_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_crnti_info
(
    U8  **pp_buffer,
    rrc_mac_sps_crnti_info_t *p_rrc_mac_sps_crnti_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_ca_config_error_len
(
    rrc_mac_ca_config_error_t *p_rrc_mac_ca_config_error
);

static
rrc_length_t
rrc_il_get_rrc_mac_ca_config_error_tlv_len
(
    rrc_mac_ca_config_error_t *p_rrc_mac_ca_config_error

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ca_config_error
(
    U8  **pp_buffer,
    rrc_mac_ca_config_error_t *p_rrc_mac_ca_config_error
);

static
rrc_length_t
rrc_il_get_rrc_mac_delete_lc_status_len
(
    rrc_mac_delete_lc_status_t *p_rrc_mac_delete_lc_status
);

static
rrc_length_t
rrc_il_get_rrc_mac_delete_lc_status_tlv_len
(
    rrc_mac_delete_lc_status_t *p_rrc_mac_delete_lc_status

);

static
rrc_return_et
rrc_il_compose_rrc_mac_delete_lc_status
(
    U8  **pp_buffer,
    rrc_mac_delete_lc_status_t *p_rrc_mac_delete_lc_status
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_max_rb_info_len
(
    rrc_mac_dl_max_rb_info_t *p_rrc_mac_dl_max_rb_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_max_rb_info_tlv_len
(
    rrc_mac_dl_max_rb_info_t *p_rrc_mac_dl_max_rb_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_max_rb_info
(
    U8  **pp_buffer,
    rrc_mac_dl_max_rb_info_t *p_rrc_mac_dl_max_rb_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_time_alignment_timer_info_len
(
    rrc_mac_time_alignment_timer_info_t *p_rrc_mac_time_alignment_timer_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_time_alignment_timer_info_tlv_len
(
    rrc_mac_time_alignment_timer_info_t *p_rrc_mac_time_alignment_timer_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_time_alignment_timer_info
(
    U8  **pp_buffer,
    rrc_mac_time_alignment_timer_info_t *p_rrc_mac_time_alignment_timer_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_p0_ue_pusch_len
(
    rrc_mac_p0_ue_pusch_t *p_rrc_mac_p0_ue_pusch
);

static
rrc_length_t
rrc_il_get_rrc_mac_p0_ue_pusch_tlv_len
(
    rrc_mac_p0_ue_pusch_t *p_rrc_mac_p0_ue_pusch

);

static
rrc_return_et
rrc_il_compose_rrc_mac_p0_ue_pusch
(
    U8  **pp_buffer,
    rrc_mac_p0_ue_pusch_t *p_rrc_mac_p0_ue_pusch
);

static
rrc_length_t
rrc_il_get_rrc_mac_max_ul_harq_tx_info_len
(
    rrc_mac_max_ul_harq_tx_info_t *p_rrc_mac_max_ul_harq_tx_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_max_ul_harq_tx_info_tlv_len
(
    rrc_mac_max_ul_harq_tx_info_t *p_rrc_mac_max_ul_harq_tx_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_max_ul_harq_tx_info
(
    U8  **pp_buffer,
    rrc_mac_max_ul_harq_tx_info_t *p_rrc_mac_max_ul_harq_tx_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_pc_max_config_info_len
(
    rrc_mac_pc_max_config_info_t *p_rrc_mac_pc_max_config_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_pc_max_config_info_tlv_len
(
    rrc_mac_pc_max_config_info_t *p_rrc_mac_pc_max_config_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_pc_max_config_info
(
    U8  **pp_buffer,
    rrc_mac_pc_max_config_info_t *p_rrc_mac_pc_max_config_info
);

static
rrc_length_t
rrc_il_get_RrcMacUeCategoryV1310_len
(
    RrcMacUeCategoryV1310_t *p_RrcMacUeCategoryV1310
);

static
rrc_length_t
rrc_il_get_RrcMacUeCategoryV1310_tlv_len
(
    RrcMacUeCategoryV1310_t *p_RrcMacUeCategoryV1310

);

static
rrc_return_et
rrc_il_compose_RrcMacUeCategoryV1310
(
    U8  **pp_buffer,
    RrcMacUeCategoryV1310_t *p_RrcMacUeCategoryV1310
);

static
rrc_length_t
rrc_il_get_rrc_mac_reconfig_ue_info_len
(
    rrc_mac_reconfig_ue_info_t *p_rrc_mac_reconfig_ue_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_reconfig_ue_info_tlv_len
(
    rrc_mac_reconfig_ue_info_t *p_rrc_mac_reconfig_ue_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_reconfig_ue_info
(
    U8  **pp_buffer,
    rrc_mac_reconfig_ue_info_t *p_rrc_mac_reconfig_ue_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_ul_lc_reconfig_req_len
(
    rrc_mac_ul_lc_reconfig_req_t *p_rrc_mac_ul_lc_reconfig_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_ul_lc_reconfig_req_tlv_len
(
    rrc_mac_ul_lc_reconfig_req_t *p_rrc_mac_ul_lc_reconfig_req

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ul_lc_reconfig_req
(
    U8  **pp_buffer,
    rrc_mac_ul_lc_reconfig_req_t *p_rrc_mac_ul_lc_reconfig_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_lc_reconfig_req_len
(
    rrc_mac_dl_lc_reconfig_req_t *p_rrc_mac_dl_lc_reconfig_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_dl_lc_reconfig_req_tlv_len
(
    rrc_mac_dl_lc_reconfig_req_t *p_rrc_mac_dl_lc_reconfig_req

);

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_lc_reconfig_req
(
    U8  **pp_buffer,
    rrc_mac_dl_lc_reconfig_req_t *p_rrc_mac_dl_lc_reconfig_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_reconfigure_lc_req_len
(
    rrc_mac_reconfigure_lc_req_t *p_rrc_mac_reconfigure_lc_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_reconfigure_lc_req_tlv_len
(
    rrc_mac_reconfigure_lc_req_t *p_rrc_mac_reconfigure_lc_req

);

static
rrc_return_et
rrc_il_compose_rrc_mac_reconfigure_lc_req
(
    U8  **pp_buffer,
    rrc_mac_reconfigure_lc_req_t *p_rrc_mac_reconfigure_lc_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_delete_lc_req_len
(
    rrc_mac_delete_lc_req_t *p_rrc_mac_delete_lc_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_delete_lc_req_tlv_len
(
    rrc_mac_delete_lc_req_t *p_rrc_mac_delete_lc_req

);

static
rrc_return_et
rrc_il_compose_rrc_mac_delete_lc_req
(
    U8  **pp_buffer,
    rrc_mac_delete_lc_req_t *p_rrc_mac_delete_lc_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_reconfigure_lc_error_len
(
    rrc_mac_reconfigure_lc_error_t *p_rrc_mac_reconfigure_lc_error
);

static
rrc_length_t
rrc_il_get_rrc_mac_reconfigure_lc_error_tlv_len
(
    rrc_mac_reconfigure_lc_error_t *p_rrc_mac_reconfigure_lc_error

);

static
rrc_return_et
rrc_il_compose_rrc_mac_reconfigure_lc_error
(
    U8  **pp_buffer,
    rrc_mac_reconfigure_lc_error_t *p_rrc_mac_reconfigure_lc_error
);

static
rrc_length_t
rrc_il_get_rrc_mac_delete_lc_error_len
(
    rrc_mac_delete_lc_error_t *p_rrc_mac_delete_lc_error
);

static
rrc_length_t
rrc_il_get_rrc_mac_delete_lc_error_tlv_len
(
    rrc_mac_delete_lc_error_t *p_rrc_mac_delete_lc_error

);

static
rrc_return_et
rrc_il_compose_rrc_mac_delete_lc_error
(
    U8  **pp_buffer,
    rrc_mac_delete_lc_error_t *p_rrc_mac_delete_lc_error
);

static
rrc_length_t
rrc_il_get_rrc_mac_ca_reconfig_error_len
(
    rrc_mac_ca_reconfig_error_t *p_rrc_mac_ca_reconfig_error
);

static
rrc_length_t
rrc_il_get_rrc_mac_ca_reconfig_error_tlv_len
(
    rrc_mac_ca_reconfig_error_t *p_rrc_mac_ca_reconfig_error

);

static
rrc_return_et
rrc_il_compose_rrc_mac_ca_reconfig_error
(
    U8  **pp_buffer,
    rrc_mac_ca_reconfig_error_t *p_rrc_mac_ca_reconfig_error
);

static
rrc_length_t
rrc_il_get_rrc_mac_delete_ca_config_error_len
(
    rrc_mac_delete_ca_config_error_t *p_rrc_mac_delete_ca_config_error
);

static
rrc_length_t
rrc_il_get_rrc_mac_delete_ca_config_error_tlv_len
(
    rrc_mac_delete_ca_config_error_t *p_rrc_mac_delete_ca_config_error

);

static
rrc_return_et
rrc_il_compose_rrc_mac_delete_ca_config_error
(
    U8  **pp_buffer,
    rrc_mac_delete_ca_config_error_t *p_rrc_mac_delete_ca_config_error
);

static
rrc_length_t
rrc_il_get_RrcMacPagingNarrowband_len
(
    RrcMacPagingNarrowband_t *p_RrcMacPagingNarrowband
);

static
rrc_length_t
rrc_il_get_RrcMacPagingNarrowband_tlv_len
(
    RrcMacPagingNarrowband_t *p_RrcMacPagingNarrowband

);

static
rrc_return_et
rrc_il_compose_RrcMacPagingNarrowband
(
    U8  **pp_buffer,
    RrcMacPagingNarrowband_t *p_RrcMacPagingNarrowband
);

static
rrc_length_t
rrc_il_get_RrcMacMpdcchNumRepetitionPaging_len
(
    RrcMacMpdcchNumRepetitionPaging_t *p_RrcMacMpdcchNumRepetitionPaging
);

static
rrc_length_t
rrc_il_get_RrcMacMpdcchNumRepetitionPaging_tlv_len
(
    RrcMacMpdcchNumRepetitionPaging_t *p_RrcMacMpdcchNumRepetitionPaging

);

static
rrc_return_et
rrc_il_compose_RrcMacMpdcchNumRepetitionPaging
(
    U8  **pp_buffer,
    RrcMacMpdcchNumRepetitionPaging_t *p_RrcMacMpdcchNumRepetitionPaging
);

static
rrc_length_t
rrc_il_get_RrcMacPdschNumRepetition_len
(
    RrcMacPdschNumRepetition_t *p_RrcMacPdschNumRepetition
);

static
rrc_length_t
rrc_il_get_RrcMacPdschNumRepetition_tlv_len
(
    RrcMacPdschNumRepetition_t *p_RrcMacPdschNumRepetition

);

static
rrc_return_et
rrc_il_compose_RrcMacPdschNumRepetition
(
    U8  **pp_buffer,
    RrcMacPdschNumRepetition_t *p_RrcMacPdschNumRepetition
);

static
rrc_length_t
rrc_il_get_RrcMacDirectIndication_len
(
    RrcMacDirectIndication_t *p_RrcMacDirectIndication
);

static
rrc_length_t
rrc_il_get_RrcMacDirectIndication_tlv_len
(
    RrcMacDirectIndication_t *p_RrcMacDirectIndication

);

static
rrc_return_et
rrc_il_compose_RrcMacDirectIndication
(
    U8  **pp_buffer,
    RrcMacDirectIndication_t *p_RrcMacDirectIndication
);

static
rrc_length_t
rrc_il_get_rrc_mac_intra_enb_ho_req_len
(
    rrc_mac_intra_enb_ho_req_t *p_rrc_mac_intra_enb_ho_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_intra_enb_ho_req_tlv_len
(
    rrc_mac_intra_enb_ho_req_t *p_rrc_mac_intra_enb_ho_req

);

static
rrc_return_et
rrc_il_compose_rrc_mac_intra_enb_ho_req
(
    U8  **pp_buffer,
    rrc_mac_intra_enb_ho_req_t *p_rrc_mac_intra_enb_ho_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_crnti_info_len
(
    rrc_mac_crnti_info_t *p_rrc_mac_crnti_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_crnti_info_tlv_len
(
    rrc_mac_crnti_info_t *p_rrc_mac_crnti_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_crnti_info
(
    U8  **pp_buffer,
    rrc_mac_crnti_info_t *p_rrc_mac_crnti_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_preamble_info_len
(
    rrc_mac_preamble_info_t *p_rrc_mac_preamble_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_preamble_info_tlv_len
(
    rrc_mac_preamble_info_t *p_rrc_mac_preamble_info

);

static
rrc_return_et
rrc_il_compose_rrc_mac_preamble_info
(
    U8  **pp_buffer,
    rrc_mac_preamble_info_t *p_rrc_mac_preamble_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_scell_rel_flag_len
(
    rrc_mac_scell_rel_flag_t *p_rrc_mac_scell_rel_flag
);

static
rrc_length_t
rrc_il_get_rrc_mac_scell_rel_flag_tlv_len
(
    rrc_mac_scell_rel_flag_t *p_rrc_mac_scell_rel_flag

);

static
rrc_return_et
rrc_il_compose_rrc_mac_scell_rel_flag
(
    U8  **pp_buffer,
    rrc_mac_scell_rel_flag_t *p_rrc_mac_scell_rel_flag
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
rrc_il_get_rrc_mac_config_cell_req_len
(
    rrc_mac_config_cell_req_t *p_rrc_mac_config_cell_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_config_cell_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->duplexing_mode);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->dl_res_blocks);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->ul_res_blocks);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->max_harq_retrans);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->num_of_tx_antennas);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->ul_cyclic_lenth_prefix);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->start_ra_rnti_range);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->end_ra_rnti_range);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->mod_period);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->sfn_gap);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->nb);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->cfi_value);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->num_of_eul);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->size_of_eul);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_req->dci_format_for_si_msgs);

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_TDD_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tdd_config_tlv_len(&p_rrc_mac_config_cell_req->tdd_config);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_RACH_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_rach_config_info_tlv_len(&p_rrc_mac_config_cell_req->rach_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PHICH_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_phich_config_info_tlv_len(&p_rrc_mac_config_cell_req->phich_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PUCCH_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pucch_config_info_tlv_len(&p_rrc_mac_config_cell_req->pucch_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_RBS_PER_TBS_DCI_1A_PRESENT)
    {
        length += rrc_il_get_rrc_mac_rbs_for_dci_1a_tlv_len(&p_rrc_mac_config_cell_req->num_rbs_per_tbs_dci_1a);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_RBS_PER_TBS_DCI_1C_PRESENT)
    {
        length += rrc_il_get_rrc_mac_rbs_for_dci_1c_tlv_len(&p_rrc_mac_config_cell_req->num_rbs_per_tbs_dci_1c);
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_config_cell_req->mib_msg_req_counter <= ARRSIZE(p_rrc_mac_config_cell_req->mib_msg_req));
        for (loop = 0; loop < p_rrc_mac_config_cell_req->mib_msg_req_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_mib_msg_req_tlv_len(&p_rrc_mac_config_cell_req->mib_msg_req[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_SIBTYPE1_MSG_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sibtype1_msg_req_tlv_len(&p_rrc_mac_config_cell_req->sib1_msg_req);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_SI_MSG_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_si_msg_req_tlv_len(&p_rrc_mac_config_cell_req->si_msg_req);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_SRS_COMMON_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_srs_common_config_tlv_len(&p_rrc_mac_config_cell_req->srs_common_config);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PDSCH_CONFIG_COMMON_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pdsch_config_common_info_tlv_len(&p_rrc_mac_config_cell_req->pdsch_config_common_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_DOWNLINK_POWER_CONTROL_COMMON_PRESENT)
    {
        length += rrc_il_get_rrc_mac_downlink_power_control_common_tlv_len(&p_rrc_mac_config_cell_req->downlink_power_control_common);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_UPLINK_POWER_CONTROL_COMMON_PRESENT)
    {
        length += rrc_il_get_rrc_mac_uplink_power_control_common_tlv_len(&p_rrc_mac_config_cell_req->uplink_power_control_common);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PUSCH_CONFIG_COMMON_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pusch_config_common_info_tlv_len(&p_rrc_mac_config_cell_req->pusch_config_common_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_FREQ_SELECTIVE_ENABLE_PRESENT)
    {
        length += rrc_il_get_rrc_mac_freq_selective_scheduling_tlv_len(&p_rrc_mac_config_cell_req->freq_selective_scheduling);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_CONTN_FREE_RACH_TIMER_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_contn_free_rach_timer_info_tlv_len(&p_rrc_mac_config_cell_req->contn_free_rach_timer_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_TPC_RNTI_RANGE_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tpc_rnti_range_tlv_len(&p_rrc_mac_config_cell_req->tpc_rnti_range);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_POWER_CONTROL_ENABLE_PRESENT)
    {
        length += rrc_il_get_rrc_mac_power_control_enable_tlv_len(&p_rrc_mac_config_cell_req->power_control_enable);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_EXPLICIT_START_REQUIRED_PRESENT)
    {
        length += rrc_il_get_rrc_mac_explicit_cell_start_required_tlv_len(&p_rrc_mac_config_cell_req->explicit_cell_start_required);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_UL_SYNC_LOSS_TIMER_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ul_sync_loss_timer_info_tlv_len(&p_rrc_mac_config_cell_req->ul_sync_loss_timer_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pucch_cqi_sinr_threshold_info_tlv_len(&p_rrc_mac_config_cell_req->pucch_cqi_sinr_threshold_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_N_GAP_PRESENT)
    {
        length += rrc_il_get_rrc_mac_n_gap_tlv_len(&p_rrc_mac_config_cell_req->n_gap);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_SPS_CRNTI_RANGE_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sps_crnti_range_tlv_len(&p_rrc_mac_config_cell_req->sps_crnti_range);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_DYNAMIC_PDCCH_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_dynamic_pdcch_info_tlv_len(&p_rrc_mac_config_cell_req->dynamic_pdcch_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PRS_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_prs_config_info_tlv_len(&p_rrc_mac_config_cell_req->prs_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_CONFIG_EMBMS_SIB2_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_m2ap_embms_sib2_info_tlv_len(&p_rrc_mac_config_cell_req->rrc_m2ap_embms_sib2_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_CONFIG_SIB13_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_m2ap_mac_embms_sib13_info_tlv_len(&p_rrc_mac_config_cell_req->rrc_m2ap_mac_sib13_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_M2AP_CONFIG_MBMS_AREA_CREATE_PRESENT)
    {
        length += rrc_il_get_rrc_m2ap_mac_create_embms_area_entity_info_tlv_len(&p_rrc_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_M2AP_CONFIG_MBMS_AREA_DELETE_PRESENT)
    {
        length += rrc_il_get_rrc_m2ap_mac_delete_embms_area_entity_info_tlv_len(&p_rrc_mac_config_cell_req->rrc_m2ap_mac_delete_embms_area_entity_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_M2AP_CONFIG_EMBMS_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_m2ap_mac_embms_params_info_tlv_len(&p_rrc_mac_config_cell_req->rrc_m2ap_mac_embms_params_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_LAA_SCELL_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_radio_res_dedicated_laa_scell_config_tlv_len(&p_rrc_mac_config_cell_req->laa_scell_configuration_r13);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_DMTC_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_dmtc_configuration_r13_tlv_len(&p_rrc_mac_config_cell_req->dmtc_configuration_r13);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_EMTC_DATA_CONFIG_PRESENT)
    {
        length += rrc_il_get_RrcEmtcDataConfig_tlv_len(&p_rrc_mac_config_cell_req->emtcDataConfig);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PRACH_CONFIG_V1310_PRESENT)
    {
        length += rrc_il_get_RrcMacPrachConfigV1310_tlv_len(&p_rrc_mac_config_cell_req->prachConfigV1310);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_SIBTYPE1_BR_MSG_INFO_PRESENT)
    {
        length += rrc_il_get_RrcMacSibtype1BrMsgInfo_tlv_len(&p_rrc_mac_config_cell_req->sibType1BrMsgInfo);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_SI_MSG_BR_INFO_PRESENT)
    {
        length += rrc_il_get_RrcMacSiMsgBrInfo_tlv_len(&p_rrc_mac_config_cell_req->siMsgBrInfo);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PUCCH_CONFIG_COMMON_V1310_PRESENT)
    {
        length += rrc_il_get_RrcMacPucchConfigCommonV1310_tlv_len(&p_rrc_mac_config_cell_req->pucchConfigCommonV1310);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_MODIFICATION_PERIOD_V1310_PRESENT)
    {
        length += rrc_il_get_RrcMacModificationPeriodV1310_tlv_len(&p_rrc_mac_config_cell_req->modificationPeriodV1310);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PCCH_CONFIG_V1310_PRESENT)
    {
        length += rrc_il_get_RrcMacPcchConfigV1310_tlv_len(&p_rrc_mac_config_cell_req->pcchConfigV1310);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PDSCH_CONFIG_COMMON_V1310_PRESENT)
    {
        length += rrc_il_get_RrcMacPdschConfigCommonV1310_tlv_len(&p_rrc_mac_config_cell_req->pdschConfigCommonV1310);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PUSCH_CONFIG_COMMON_V1310_PRESENT)
    {
        length += rrc_il_get_RrcMacPuschConfigCommonV1310_tlv_len(&p_rrc_mac_config_cell_req->puschConfigCommonV1310);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_FREQUANCY_HOPPING_PARAMETERS_R13_PRESENT)
    {
        length += rrc_il_get_RrcFrequancyHoppingParametersR13_tlv_len(&p_rrc_mac_config_cell_req->frequancyHoppingParametersR13);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_config_cell_req
(
    U8  **pp_buffer,
    rrc_mac_config_cell_req_t *p_rrc_mac_config_cell_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_config_cell_req != PNULL);

    /* This function composes rrc_mac_config_cell_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_req->duplexing_mode, "duplexing_mode");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->duplexing_mode);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->dl_res_blocks < 1) || (p_rrc_mac_config_cell_req->dl_res_blocks > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->dl_res_blocks] should be in range "
            "1 to 100. Incorrect value %u received.", p_rrc_mac_config_cell_req->dl_res_blocks);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_req->dl_res_blocks, "dl_res_blocks");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->dl_res_blocks);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->ul_res_blocks < 1) || (p_rrc_mac_config_cell_req->ul_res_blocks > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->ul_res_blocks] should be in range "
            "1 to 100. Incorrect value %u received.", p_rrc_mac_config_cell_req->ul_res_blocks);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_req->ul_res_blocks, "ul_res_blocks");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->ul_res_blocks);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->max_harq_retrans < 1) || (p_rrc_mac_config_cell_req->max_harq_retrans > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->max_harq_retrans] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_mac_config_cell_req->max_harq_retrans);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_req->max_harq_retrans, "max_harq_retrans");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->max_harq_retrans);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->num_of_tx_antennas < 1) || (p_rrc_mac_config_cell_req->num_of_tx_antennas > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->num_of_tx_antennas] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_mac_config_cell_req->num_of_tx_antennas);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_req->num_of_tx_antennas, "num_of_tx_antennas");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->num_of_tx_antennas);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->ul_cyclic_lenth_prefix < 1) || (p_rrc_mac_config_cell_req->ul_cyclic_lenth_prefix > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->ul_cyclic_lenth_prefix] should be in range "
            "1 to 2. Incorrect value %u received.", p_rrc_mac_config_cell_req->ul_cyclic_lenth_prefix);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_req->ul_cyclic_lenth_prefix, "ul_cyclic_lenth_prefix");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->ul_cyclic_lenth_prefix);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->start_ra_rnti_range < 1) || (p_rrc_mac_config_cell_req->start_ra_rnti_range > 60))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->start_ra_rnti_range] should be in range "
            "1 to 60. Incorrect value %u received.", p_rrc_mac_config_cell_req->start_ra_rnti_range);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_req->start_ra_rnti_range, "start_ra_rnti_range");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->start_ra_rnti_range);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->end_ra_rnti_range < 1) || (p_rrc_mac_config_cell_req->end_ra_rnti_range > 60))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->end_ra_rnti_range] should be in range "
            "1 to 60. Incorrect value %u received.", p_rrc_mac_config_cell_req->end_ra_rnti_range);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_req->end_ra_rnti_range, "end_ra_rnti_range");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->end_ra_rnti_range);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->mod_period < 64) || (p_rrc_mac_config_cell_req->mod_period > 4096))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->mod_period] should be in range "
            "64 to 4096. Incorrect value %u received.", p_rrc_mac_config_cell_req->mod_period);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_mac_config_cell_req->mod_period, "mod_period");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->mod_period);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->sfn_gap < 1) || (p_rrc_mac_config_cell_req->sfn_gap > 32))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->sfn_gap] should be in range "
            "1 to 32. Incorrect value %u received.", p_rrc_mac_config_cell_req->sfn_gap);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_req->sfn_gap, "sfn_gap");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->sfn_gap);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_config_cell_req->nb > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->nb] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_config_cell_req->nb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_req->nb, "nb");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->nb);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_config_cell_req->cfi_value < 1) || (p_rrc_mac_config_cell_req->cfi_value > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->cfi_value] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_mac_config_cell_req->cfi_value);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_req->cfi_value, "cfi_value");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->cfi_value);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_config_cell_req->num_of_eul, "num_of_eul");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->num_of_eul);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_config_cell_req->size_of_eul, "size_of_eul");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->size_of_eul);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_config_cell_req->dci_format_for_si_msgs > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_req->dci_format_for_si_msgs] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_config_cell_req->dci_format_for_si_msgs);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_req->dci_format_for_si_msgs, "dci_format_for_si_msgs");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_req->dci_format_for_si_msgs);

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_TDD_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tdd_config(pp_buffer, &p_rrc_mac_config_cell_req->tdd_config))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_RACH_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_rach_config_info(pp_buffer, &p_rrc_mac_config_cell_req->rach_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PHICH_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_phich_config_info(pp_buffer, &p_rrc_mac_config_cell_req->phich_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PUCCH_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pucch_config_info(pp_buffer, &p_rrc_mac_config_cell_req->pucch_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_RBS_PER_TBS_DCI_1A_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_rbs_for_dci_1a(pp_buffer, &p_rrc_mac_config_cell_req->num_rbs_per_tbs_dci_1a))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_RBS_PER_TBS_DCI_1C_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_rbs_for_dci_1c(pp_buffer, &p_rrc_mac_config_cell_req->num_rbs_per_tbs_dci_1c))
        {
            return RRC_FAILURE;
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_config_cell_req->mib_msg_req_counter <= ARRSIZE(p_rrc_mac_config_cell_req->mib_msg_req));
        for (loop = 0; loop < p_rrc_mac_config_cell_req->mib_msg_req_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_mib_msg_req(pp_buffer, &p_rrc_mac_config_cell_req->mib_msg_req[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_SIBTYPE1_MSG_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sibtype1_msg_req(pp_buffer, &p_rrc_mac_config_cell_req->sib1_msg_req))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_SI_MSG_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_si_msg_req(pp_buffer, &p_rrc_mac_config_cell_req->si_msg_req))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_SRS_COMMON_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_srs_common_config(pp_buffer, &p_rrc_mac_config_cell_req->srs_common_config))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PDSCH_CONFIG_COMMON_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pdsch_config_common_info(pp_buffer, &p_rrc_mac_config_cell_req->pdsch_config_common_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_DOWNLINK_POWER_CONTROL_COMMON_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_downlink_power_control_common(pp_buffer, &p_rrc_mac_config_cell_req->downlink_power_control_common))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_UPLINK_POWER_CONTROL_COMMON_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_uplink_power_control_common(pp_buffer, &p_rrc_mac_config_cell_req->uplink_power_control_common))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PUSCH_CONFIG_COMMON_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pusch_config_common_info(pp_buffer, &p_rrc_mac_config_cell_req->pusch_config_common_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_FREQ_SELECTIVE_ENABLE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_freq_selective_scheduling(pp_buffer, &p_rrc_mac_config_cell_req->freq_selective_scheduling))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_CONTN_FREE_RACH_TIMER_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_contn_free_rach_timer_info(pp_buffer, &p_rrc_mac_config_cell_req->contn_free_rach_timer_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_TPC_RNTI_RANGE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tpc_rnti_range(pp_buffer, &p_rrc_mac_config_cell_req->tpc_rnti_range))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_POWER_CONTROL_ENABLE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_power_control_enable(pp_buffer, &p_rrc_mac_config_cell_req->power_control_enable))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_EXPLICIT_START_REQUIRED_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_explicit_cell_start_required(pp_buffer, &p_rrc_mac_config_cell_req->explicit_cell_start_required))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_UL_SYNC_LOSS_TIMER_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ul_sync_loss_timer_info(pp_buffer, &p_rrc_mac_config_cell_req->ul_sync_loss_timer_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pucch_cqi_sinr_threshold_info(pp_buffer, &p_rrc_mac_config_cell_req->pucch_cqi_sinr_threshold_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_N_GAP_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_n_gap(pp_buffer, &p_rrc_mac_config_cell_req->n_gap))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_SPS_CRNTI_RANGE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sps_crnti_range(pp_buffer, &p_rrc_mac_config_cell_req->sps_crnti_range))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_DYNAMIC_PDCCH_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_dynamic_pdcch_info(pp_buffer, &p_rrc_mac_config_cell_req->dynamic_pdcch_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PRS_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_prs_config_info(pp_buffer, &p_rrc_mac_config_cell_req->prs_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_CONFIG_EMBMS_SIB2_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_m2ap_embms_sib2_info(pp_buffer, &p_rrc_mac_config_cell_req->rrc_m2ap_embms_sib2_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_CONFIG_SIB13_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_m2ap_mac_embms_sib13_info(pp_buffer, &p_rrc_mac_config_cell_req->rrc_m2ap_mac_sib13_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_M2AP_CONFIG_MBMS_AREA_CREATE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_m2ap_mac_create_embms_area_entity_info(pp_buffer, &p_rrc_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_M2AP_CONFIG_MBMS_AREA_DELETE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_m2ap_mac_delete_embms_area_entity_info(pp_buffer, &p_rrc_mac_config_cell_req->rrc_m2ap_mac_delete_embms_area_entity_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_M2AP_CONFIG_EMBMS_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_m2ap_mac_embms_params_info(pp_buffer, &p_rrc_mac_config_cell_req->rrc_m2ap_mac_embms_params_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_LAA_SCELL_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_radio_res_dedicated_laa_scell_config(pp_buffer, &p_rrc_mac_config_cell_req->laa_scell_configuration_r13))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_DMTC_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_dmtc_configuration_r13(pp_buffer, &p_rrc_mac_config_cell_req->dmtc_configuration_r13))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_EMTC_DATA_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcEmtcDataConfig(pp_buffer, &p_rrc_mac_config_cell_req->emtcDataConfig))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PRACH_CONFIG_V1310_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacPrachConfigV1310(pp_buffer, &p_rrc_mac_config_cell_req->prachConfigV1310))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_SIBTYPE1_BR_MSG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacSibtype1BrMsgInfo(pp_buffer, &p_rrc_mac_config_cell_req->sibType1BrMsgInfo))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_SI_MSG_BR_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacSiMsgBrInfo(pp_buffer, &p_rrc_mac_config_cell_req->siMsgBrInfo))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PUCCH_CONFIG_COMMON_V1310_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacPucchConfigCommonV1310(pp_buffer, &p_rrc_mac_config_cell_req->pucchConfigCommonV1310))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_MODIFICATION_PERIOD_V1310_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacModificationPeriodV1310(pp_buffer, &p_rrc_mac_config_cell_req->modificationPeriodV1310))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PCCH_CONFIG_V1310_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacPcchConfigV1310(pp_buffer, &p_rrc_mac_config_cell_req->pcchConfigV1310))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PDSCH_CONFIG_COMMON_V1310_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacPdschConfigCommonV1310(pp_buffer, &p_rrc_mac_config_cell_req->pdschConfigCommonV1310))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_PUSCH_CONFIG_COMMON_V1310_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacPuschConfigCommonV1310(pp_buffer, &p_rrc_mac_config_cell_req->puschConfigCommonV1310))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_req->optional_elems_present & RRC_MAC_FREQUANCY_HOPPING_PARAMETERS_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcFrequancyHoppingParametersR13(pp_buffer, &p_rrc_mac_config_cell_req->frequancyHoppingParametersR13))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_config_cell_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_CONFIG_CELL_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_config_cell_req
(
    rrc_mac_config_cell_req_t  *p_rrc_mac_config_cell_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_config_cell_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_config_cell_req_len(p_rrc_mac_config_cell_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_CONFIG_CELL_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_CONFIG_CELL_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_CONFIG_CELL_REQ, msg_length,cell_index);

    /* Fill RRC_MAC_CONFIG_CELL_REQ message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_config_cell_req(&p_mac_msg, p_rrc_mac_config_cell_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tdd_config_len
(
    rrc_mac_tdd_config_t *p_rrc_mac_tdd_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tdd_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tdd_config->sub_frame_assignment);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tdd_config->special_sub_frame_pattern);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tdd_config_tlv_len
(
    rrc_mac_tdd_config_t *p_rrc_mac_tdd_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tdd_config != PNULL);

    length += rrc_il_get_rrc_mac_tdd_config_len(p_rrc_mac_tdd_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_tdd_config
(
    U8  **pp_buffer,
    rrc_mac_tdd_config_t *p_rrc_mac_tdd_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_tdd_config != PNULL);

    /* This function composes rrc_mac_tdd_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_TDD_CONFIG;
    tlv_length = rrc_il_get_rrc_mac_tdd_config_tlv_len(p_rrc_mac_tdd_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_tdd_config->sub_frame_assignment > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tdd_config->sub_frame_assignment] should be less than"
            " or equal to 6. Incorrect value %u received.", p_rrc_mac_tdd_config->sub_frame_assignment);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_tdd_config->sub_frame_assignment, "sub_frame_assignment");
    *pp_buffer += sizeof(p_rrc_mac_tdd_config->sub_frame_assignment);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_tdd_config->special_sub_frame_pattern > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tdd_config->special_sub_frame_pattern] should be less than"
            " or equal to 8. Incorrect value %u received.", p_rrc_mac_tdd_config->special_sub_frame_pattern);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_tdd_config->special_sub_frame_pattern, "special_sub_frame_pattern");
    *pp_buffer += sizeof(p_rrc_mac_tdd_config->special_sub_frame_pattern);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_preamble_group_a_config_len
(
    rrc_mac_preamble_group_a_config_t *p_rrc_mac_preamble_group_a_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_preamble_group_a_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_preamble_group_a_config->num_of_preambles_in_group_a);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_preamble_group_a_config->message_size_group_a);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_preamble_group_a_config_tlv_len
(
    rrc_mac_preamble_group_a_config_t *p_rrc_mac_preamble_group_a_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_preamble_group_a_config != PNULL);

    length += rrc_il_get_rrc_mac_preamble_group_a_config_len(p_rrc_mac_preamble_group_a_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_preamble_group_a_config
(
    U8  **pp_buffer,
    rrc_mac_preamble_group_a_config_t *p_rrc_mac_preamble_group_a_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_preamble_group_a_config != PNULL);

    /* This function composes rrc_mac_preamble_group_a_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PREAMBLE_GROUP_A_CONFIG;
    tlv_length = rrc_il_get_rrc_mac_preamble_group_a_config_tlv_len(p_rrc_mac_preamble_group_a_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_preamble_group_a_config->num_of_preambles_in_group_a < 4) || (p_rrc_mac_preamble_group_a_config->num_of_preambles_in_group_a > 60))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_preamble_group_a_config->num_of_preambles_in_group_a] should be in range "
            "4 to 60. Incorrect value %u received.", p_rrc_mac_preamble_group_a_config->num_of_preambles_in_group_a);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_preamble_group_a_config->num_of_preambles_in_group_a, "num_of_preambles_in_group_a");
    *pp_buffer += sizeof(p_rrc_mac_preamble_group_a_config->num_of_preambles_in_group_a);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_preamble_group_a_config->message_size_group_a < 56) || (p_rrc_mac_preamble_group_a_config->message_size_group_a > 256))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_preamble_group_a_config->message_size_group_a] should be in range "
            "56 to 256. Incorrect value %u received.", p_rrc_mac_preamble_group_a_config->message_size_group_a);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_preamble_group_a_config->message_size_group_a, "message_size_group_a");
    *pp_buffer += sizeof(p_rrc_mac_preamble_group_a_config->message_size_group_a);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacRachCeLevelInfo_len
(
    RrcMacRachCeLevelInfo_t *p_RrcMacRachCeLevelInfo
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacRachCeLevelInfo != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacRachCeLevelInfo->raResponseWindowSizeR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacRachCeLevelInfo->macContentionResolutionTimerR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacRachCeLevelInfo->rarHoppingConfigR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacRachCeLevelInfo->firstPreambleR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacRachCeLevelInfo->lastPreambleR13);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacRachCeLevelInfo_tlv_len
(
    RrcMacRachCeLevelInfo_t *p_RrcMacRachCeLevelInfo

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacRachCeLevelInfo != PNULL);

    length += rrc_il_get_RrcMacRachCeLevelInfo_len(p_RrcMacRachCeLevelInfo);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacRachCeLevelInfo
(
    U8  **pp_buffer,
    RrcMacRachCeLevelInfo_t *p_RrcMacRachCeLevelInfo
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacRachCeLevelInfo != PNULL);

    /* This function composes RrcMacRachCeLevelInfo */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RACH_CE_LEVEL_INFO_LIST;
    tlv_length = rrc_il_get_RrcMacRachCeLevelInfo_tlv_len(p_RrcMacRachCeLevelInfo);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacRachCeLevelInfo->raResponseWindowSizeR13 > EMTC_RA_RESPONSE_WINDOW_SIZE_SF_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacRachCeLevelInfo->raResponseWindowSizeR13] should be less than"
            " or equal to EMTC_RA_RESPONSE_WINDOW_SIZE_SF_MAX. Incorrect value %u received.", p_RrcMacRachCeLevelInfo->raResponseWindowSizeR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacRachCeLevelInfo->raResponseWindowSizeR13, "raResponseWindowSizeR13");
    *pp_buffer += sizeof(p_RrcMacRachCeLevelInfo->raResponseWindowSizeR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacRachCeLevelInfo->macContentionResolutionTimerR13 > EMTC_MAC_CONTENTION_RESOLUTION_TIMER_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacRachCeLevelInfo->macContentionResolutionTimerR13] should be less than"
            " or equal to EMTC_MAC_CONTENTION_RESOLUTION_TIMER_MAX. Incorrect value %u received.", p_RrcMacRachCeLevelInfo->macContentionResolutionTimerR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacRachCeLevelInfo->macContentionResolutionTimerR13, "macContentionResolutionTimerR13");
    *pp_buffer += sizeof(p_RrcMacRachCeLevelInfo->macContentionResolutionTimerR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacRachCeLevelInfo->rarHoppingConfigR13 > EMTC_RAR_HOPPPING_CONFIG_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacRachCeLevelInfo->rarHoppingConfigR13] should be less than"
            " or equal to EMTC_RAR_HOPPPING_CONFIG_MAX. Incorrect value %u received.", p_RrcMacRachCeLevelInfo->rarHoppingConfigR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacRachCeLevelInfo->rarHoppingConfigR13, "rarHoppingConfigR13");
    *pp_buffer += sizeof(p_RrcMacRachCeLevelInfo->rarHoppingConfigR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacRachCeLevelInfo->firstPreambleR13 > EMTC_MAX_PREAMBLE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacRachCeLevelInfo->firstPreambleR13] should be less than"
            " or equal to EMTC_MAX_PREAMBLE. Incorrect value %u received.", p_RrcMacRachCeLevelInfo->firstPreambleR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacRachCeLevelInfo->firstPreambleR13, "firstPreambleR13");
    *pp_buffer += sizeof(p_RrcMacRachCeLevelInfo->firstPreambleR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacRachCeLevelInfo->lastPreambleR13 > EMTC_MAX_PREAMBLE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacRachCeLevelInfo->lastPreambleR13] should be less than"
            " or equal to EMTC_MAX_PREAMBLE. Incorrect value %u received.", p_RrcMacRachCeLevelInfo->lastPreambleR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacRachCeLevelInfo->lastPreambleR13, "lastPreambleR13");
    *pp_buffer += sizeof(p_RrcMacRachCeLevelInfo->lastPreambleR13);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacRachConfigurationR13_len
(
    RrcMacRachConfigurationR13_t *p_RrcMacRachConfigurationR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacRachConfigurationR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacRachConfigurationR13->numCeLevels);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_RrcMacRachConfigurationR13->numCeLevelsCounter <= ARRSIZE(p_RrcMacRachConfigurationR13->rachCeLevelInfo));
        for (loop = 0; loop < p_RrcMacRachConfigurationR13->numCeLevelsCounter; loop++)
        {
            length += rrc_il_get_RrcMacRachCeLevelInfo_tlv_len(&p_RrcMacRachConfigurationR13->rachCeLevelInfo[loop]);
        }
    }

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacRachConfigurationR13_tlv_len
(
    RrcMacRachConfigurationR13_t *p_RrcMacRachConfigurationR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacRachConfigurationR13 != PNULL);

    length += rrc_il_get_RrcMacRachConfigurationR13_len(p_RrcMacRachConfigurationR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacRachConfigurationR13
(
    U8  **pp_buffer,
    RrcMacRachConfigurationR13_t *p_RrcMacRachConfigurationR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacRachConfigurationR13 != PNULL);

    /* This function composes RrcMacRachConfigurationR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RACH_CONFIGURATION_R13;
    tlv_length = rrc_il_get_RrcMacRachConfigurationR13_tlv_len(p_RrcMacRachConfigurationR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacRachConfigurationR13->numCeLevels < 1) || (p_RrcMacRachConfigurationR13->numCeLevels > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacRachConfigurationR13->numCeLevels] should be in range "
            "1 to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcMacRachConfigurationR13->numCeLevels);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacRachConfigurationR13->numCeLevels, "numCeLevels");
    *pp_buffer += sizeof(p_RrcMacRachConfigurationR13->numCeLevels);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_RrcMacRachConfigurationR13->numCeLevelsCounter <= ARRSIZE(p_RrcMacRachConfigurationR13->rachCeLevelInfo));
        for (loop = 0; loop < p_RrcMacRachConfigurationR13->numCeLevelsCounter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_RrcMacRachCeLevelInfo(pp_buffer, &p_RrcMacRachConfigurationR13->rachCeLevelInfo[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_rach_config_info_len
(
    rrc_mac_rach_config_info_t *p_rrc_mac_rach_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_rach_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_rach_config_info->ra_response_window_size);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_rach_config_info->mac_contention_resolution_timer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_rach_config_info->max_harq_msg3tx);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_rach_config_info->prach_configuration_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_rach_config_info->prach_frequency_offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_rach_config_info->num_of_contention_based_preambles);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_rach_config_info->preamble_initial_received_target_power);

    /* Get length of TLV */
    if (p_rrc_mac_rach_config_info->optional_elems_present & RRC_MAC_PREAMBLE_GROUP_A_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_preamble_group_a_config_tlv_len(&p_rrc_mac_rach_config_info->preamble_group_a_config);
    }

    /* Get length of TLV */
    if (p_rrc_mac_rach_config_info->optional_elems_present & RRC_MAC_RACH_CONFIGURATION_R13_PRESENT)
    {
        length += rrc_il_get_RrcMacRachConfigurationR13_tlv_len(&p_rrc_mac_rach_config_info->rachConfigurationR13);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_rach_config_info_tlv_len
(
    rrc_mac_rach_config_info_t *p_rrc_mac_rach_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_rach_config_info != PNULL);

    length += rrc_il_get_rrc_mac_rach_config_info_len(p_rrc_mac_rach_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_rach_config_info
(
    U8  **pp_buffer,
    rrc_mac_rach_config_info_t *p_rrc_mac_rach_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_rach_config_info != PNULL);

    /* This function composes rrc_mac_rach_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RACH_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_rach_config_info_tlv_len(p_rrc_mac_rach_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_rach_config_info->ra_response_window_size < 2) || (p_rrc_mac_rach_config_info->ra_response_window_size > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->ra_response_window_size] should be in range "
            "2 to 10. Incorrect value %u received.", p_rrc_mac_rach_config_info->ra_response_window_size);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_rach_config_info->ra_response_window_size, "ra_response_window_size");
    *pp_buffer += sizeof(p_rrc_mac_rach_config_info->ra_response_window_size);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_rach_config_info->mac_contention_resolution_timer < 8) || (p_rrc_mac_rach_config_info->mac_contention_resolution_timer > 64))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->mac_contention_resolution_timer] should be in range "
            "8 to 64. Incorrect value %u received.", p_rrc_mac_rach_config_info->mac_contention_resolution_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_rach_config_info->mac_contention_resolution_timer, "mac_contention_resolution_timer");
    *pp_buffer += sizeof(p_rrc_mac_rach_config_info->mac_contention_resolution_timer);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_rach_config_info->max_harq_msg3tx < 1) || (p_rrc_mac_rach_config_info->max_harq_msg3tx > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->max_harq_msg3tx] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_mac_rach_config_info->max_harq_msg3tx);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_rach_config_info->max_harq_msg3tx, "max_harq_msg3tx");
    *pp_buffer += sizeof(p_rrc_mac_rach_config_info->max_harq_msg3tx);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_rach_config_info->prach_configuration_index > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->prach_configuration_index] should be less than"
            " or equal to 63. Incorrect value %u received.", p_rrc_mac_rach_config_info->prach_configuration_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_rach_config_info->prach_configuration_index, "prach_configuration_index");
    *pp_buffer += sizeof(p_rrc_mac_rach_config_info->prach_configuration_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_rach_config_info->prach_frequency_offset > 94))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->prach_frequency_offset] should be less than"
            " or equal to 94. Incorrect value %u received.", p_rrc_mac_rach_config_info->prach_frequency_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_rach_config_info->prach_frequency_offset, "prach_frequency_offset");
    *pp_buffer += sizeof(p_rrc_mac_rach_config_info->prach_frequency_offset);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_rach_config_info->num_of_contention_based_preambles < 4) || (p_rrc_mac_rach_config_info->num_of_contention_based_preambles > 64))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->num_of_contention_based_preambles] should be in range "
            "4 to 64. Incorrect value %u received.", p_rrc_mac_rach_config_info->num_of_contention_based_preambles);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_rach_config_info->num_of_contention_based_preambles, "num_of_contention_based_preambles");
    *pp_buffer += sizeof(p_rrc_mac_rach_config_info->num_of_contention_based_preambles);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_rach_config_info->preamble_initial_received_target_power > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rach_config_info->preamble_initial_received_target_power] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_rach_config_info->preamble_initial_received_target_power);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_rach_config_info->preamble_initial_received_target_power, "preamble_initial_received_target_power");
    *pp_buffer += sizeof(p_rrc_mac_rach_config_info->preamble_initial_received_target_power);

    /* Compose TLV  */
    if (p_rrc_mac_rach_config_info->optional_elems_present & RRC_MAC_PREAMBLE_GROUP_A_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_preamble_group_a_config(pp_buffer, &p_rrc_mac_rach_config_info->preamble_group_a_config))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_rach_config_info->optional_elems_present & RRC_MAC_RACH_CONFIGURATION_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacRachConfigurationR13(pp_buffer, &p_rrc_mac_rach_config_info->rachConfigurationR13))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_phich_config_info_len
(
    rrc_mac_phich_config_info_t *p_rrc_mac_phich_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_phich_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_phich_config_info->phich_duration);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_phich_config_info->phich_resource);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_phich_config_info_tlv_len
(
    rrc_mac_phich_config_info_t *p_rrc_mac_phich_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_phich_config_info != PNULL);

    length += rrc_il_get_rrc_mac_phich_config_info_len(p_rrc_mac_phich_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_phich_config_info
(
    U8  **pp_buffer,
    rrc_mac_phich_config_info_t *p_rrc_mac_phich_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_phich_config_info != PNULL);

    /* This function composes rrc_mac_phich_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PHICH_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_phich_config_info_tlv_len(p_rrc_mac_phich_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_phich_config_info->phich_duration > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_phich_config_info->phich_duration] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_phich_config_info->phich_duration);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_phich_config_info->phich_duration, "phich_duration");
    *pp_buffer += sizeof(p_rrc_mac_phich_config_info->phich_duration);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_phich_config_info->phich_resource > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_phich_config_info->phich_resource] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_phich_config_info->phich_resource);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_phich_config_info->phich_resource, "phich_resource");
    *pp_buffer += sizeof(p_rrc_mac_phich_config_info->phich_resource);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_config_info_len
(
    rrc_mac_pucch_config_info_t *p_rrc_mac_pucch_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pucch_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pucch_config_info->delta_pucch_shift);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pucch_config_info->nrb_cqi);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pucch_config_info->ncs_an);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pucch_config_info->n1pucch_an);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_config_info_tlv_len
(
    rrc_mac_pucch_config_info_t *p_rrc_mac_pucch_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pucch_config_info != PNULL);

    length += rrc_il_get_rrc_mac_pucch_config_info_len(p_rrc_mac_pucch_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_pucch_config_info
(
    U8  **pp_buffer,
    rrc_mac_pucch_config_info_t *p_rrc_mac_pucch_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_pucch_config_info != PNULL);

    /* This function composes rrc_mac_pucch_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PUCCH_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_pucch_config_info_tlv_len(p_rrc_mac_pucch_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_pucch_config_info->delta_pucch_shift < 1) || (p_rrc_mac_pucch_config_info->delta_pucch_shift > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pucch_config_info->delta_pucch_shift] should be in range "
            "1 to 3. Incorrect value %u received.", p_rrc_mac_pucch_config_info->delta_pucch_shift);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pucch_config_info->delta_pucch_shift, "delta_pucch_shift");
    *pp_buffer += sizeof(p_rrc_mac_pucch_config_info->delta_pucch_shift);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pucch_config_info->nrb_cqi > 98))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pucch_config_info->nrb_cqi] should be less than"
            " or equal to 98. Incorrect value %u received.", p_rrc_mac_pucch_config_info->nrb_cqi);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pucch_config_info->nrb_cqi, "nrb_cqi");
    *pp_buffer += sizeof(p_rrc_mac_pucch_config_info->nrb_cqi);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pucch_config_info->ncs_an > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pucch_config_info->ncs_an] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_pucch_config_info->ncs_an);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pucch_config_info->ncs_an, "ncs_an");
    *pp_buffer += sizeof(p_rrc_mac_pucch_config_info->ncs_an);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pucch_config_info->n1pucch_an > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pucch_config_info->n1pucch_an] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_mac_pucch_config_info->n1pucch_an);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_pucch_config_info->n1pucch_an, "n1pucch_an");
    *pp_buffer += sizeof(p_rrc_mac_pucch_config_info->n1pucch_an);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_rbs_for_dci_1a_len
(
    rrc_mac_rbs_for_dci_1a_t *p_rrc_mac_rbs_for_dci_1a
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_rbs_for_dci_1a != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_mac_rbs_for_dci_1a->num_rb_buffer);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_rbs_for_dci_1a_tlv_len
(
    rrc_mac_rbs_for_dci_1a_t *p_rrc_mac_rbs_for_dci_1a

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_rbs_for_dci_1a != PNULL);

    length += rrc_il_get_rrc_mac_rbs_for_dci_1a_len(p_rrc_mac_rbs_for_dci_1a);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_rbs_for_dci_1a
(
    U8  **pp_buffer,
    rrc_mac_rbs_for_dci_1a_t *p_rrc_mac_rbs_for_dci_1a
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_rbs_for_dci_1a != PNULL);

    /* This function composes rrc_mac_rbs_for_dci_1a */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RBS_FOR_DCI_1A;
    tlv_length = rrc_il_get_rrc_mac_rbs_for_dci_1a_tlv_len(p_rrc_mac_rbs_for_dci_1a);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose OCTET_STRING FIXED of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_rbs_for_dci_1a->num_rb_buffer); loop++)
        {
            if (p_rrc_mac_rbs_for_dci_1a->num_rb_buffer[loop] > 100)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_rrc_mac_rbs_for_dci_1a->num_rb_buffer[%u] should be less"
                    " than or equal to 100. "
                    "Incorrect value %d received.", loop, p_rrc_mac_rbs_for_dci_1a->num_rb_buffer[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_rbs_for_dci_1a->num_rb_buffer[loop], "num_rb_buffer[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_rbs_for_dci_1c_len
(
    rrc_mac_rbs_for_dci_1c_t *p_rrc_mac_rbs_for_dci_1c
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_rbs_for_dci_1c != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_mac_rbs_for_dci_1c->num_rb_buffer);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_rbs_for_dci_1c_tlv_len
(
    rrc_mac_rbs_for_dci_1c_t *p_rrc_mac_rbs_for_dci_1c

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_rbs_for_dci_1c != PNULL);

    length += rrc_il_get_rrc_mac_rbs_for_dci_1c_len(p_rrc_mac_rbs_for_dci_1c);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_rbs_for_dci_1c
(
    U8  **pp_buffer,
    rrc_mac_rbs_for_dci_1c_t *p_rrc_mac_rbs_for_dci_1c
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_rbs_for_dci_1c != PNULL);

    /* This function composes rrc_mac_rbs_for_dci_1c */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RBS_FOR_DCI_1C;
    tlv_length = rrc_il_get_rrc_mac_rbs_for_dci_1c_tlv_len(p_rrc_mac_rbs_for_dci_1c);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose OCTET_STRING FIXED of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_rbs_for_dci_1c->num_rb_buffer); loop++)
        {
            if (p_rrc_mac_rbs_for_dci_1c->num_rb_buffer[loop] > 100)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_rrc_mac_rbs_for_dci_1c->num_rb_buffer[%u] should be less"
                    " than or equal to 100. "
                    "Incorrect value %d received.", loop, p_rrc_mac_rbs_for_dci_1c->num_rb_buffer[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_rbs_for_dci_1c->num_rb_buffer[loop], "num_rb_buffer[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_mib_msg_req_len
(
    rrc_mac_mib_msg_req_t *p_rrc_mac_mib_msg_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_mib_msg_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_mib_msg_req->sfn);

    /* Add buffer size */
    length += p_rrc_mac_mib_msg_req->mib_msg_buf_counter;

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_mib_msg_req_tlv_len
(
    rrc_mac_mib_msg_req_t *p_rrc_mac_mib_msg_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_mib_msg_req != PNULL);

    length += rrc_il_get_rrc_mac_mib_msg_req_len(p_rrc_mac_mib_msg_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_mib_msg_req
(
    U8  **pp_buffer,
    rrc_mac_mib_msg_req_t *p_rrc_mac_mib_msg_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_mib_msg_req != PNULL);

    /* This function composes rrc_mac_mib_msg_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MIB_MSG_REQ;
    tlv_length = rrc_il_get_rrc_mac_mib_msg_req_tlv_len(p_rrc_mac_mib_msg_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_mib_msg_req->sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_mib_msg_req->sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_mib_msg_req->sfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_mib_msg_req->sfn, "sfn");
    *pp_buffer += sizeof(p_rrc_mac_mib_msg_req->sfn);

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        if(ARRSIZE(p_rrc_mac_mib_msg_req->mib_msg_buf) < p_rrc_mac_mib_msg_req->mib_msg_buf_counter)
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_mac_mib_msg_req->mib_msg_buf_counter = %u is exceeding p_rrc_mac_mib_msg_req->mib_msg_buf array boundary", p_rrc_mac_mib_msg_req->mib_msg_buf_counter);
            return RRC_FAILURE;
        }
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_mac_mib_msg_req->mib_msg_buf_counter; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_mib_msg_req->mib_msg_buf[loop], "mib_msg_buf[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sibtype1_msg_req_len
(
    rrc_mac_sibtype1_msg_req_t *p_rrc_mac_sibtype1_msg_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sibtype1_msg_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sibtype1_msg_req->si_mapping_changed);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sibtype1_msg_req->sfn);

    /* Add buffer size */
    length += p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf_counter;

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sibtype1_msg_req_tlv_len
(
    rrc_mac_sibtype1_msg_req_t *p_rrc_mac_sibtype1_msg_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sibtype1_msg_req != PNULL);

    length += rrc_il_get_rrc_mac_sibtype1_msg_req_len(p_rrc_mac_sibtype1_msg_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sibtype1_msg_req
(
    U8  **pp_buffer,
    rrc_mac_sibtype1_msg_req_t *p_rrc_mac_sibtype1_msg_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sibtype1_msg_req != PNULL);

    /* This function composes rrc_mac_sibtype1_msg_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SIBTYPE1_MSG_REQ;
    tlv_length = rrc_il_get_rrc_mac_sibtype1_msg_req_tlv_len(p_rrc_mac_sibtype1_msg_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sibtype1_msg_req->si_mapping_changed > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sibtype1_msg_req->si_mapping_changed] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_sibtype1_msg_req->si_mapping_changed);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sibtype1_msg_req->si_mapping_changed, "si_mapping_changed");
    *pp_buffer += sizeof(p_rrc_mac_sibtype1_msg_req->si_mapping_changed);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sibtype1_msg_req->sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sibtype1_msg_req->sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_sibtype1_msg_req->sfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_sibtype1_msg_req->sfn, "sfn");
    *pp_buffer += sizeof(p_rrc_mac_sibtype1_msg_req->sfn);

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        if(ARRSIZE(p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf) < p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf_counter)
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf_counter = %u is exceeding p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf array boundary", p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf_counter);
            return RRC_FAILURE;
        }
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf_counter; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sibtype1_msg_req->sibtype1_msg_buf[loop], "sibtype1_msg_buf[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_num_si_repetitions_len
(
    rrc_num_si_repetitions *p_rrc_num_si_repetitions
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_num_si_repetitions != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_num_si_repetitions->num_of_repetitions);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_num_si_repetitions_tlv_len
(
    rrc_num_si_repetitions *p_rrc_num_si_repetitions

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_num_si_repetitions != PNULL);

    length += rrc_il_get_rrc_num_si_repetitions_len(p_rrc_num_si_repetitions);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_num_si_repetitions
(
    U8  **pp_buffer,
    rrc_num_si_repetitions *p_rrc_num_si_repetitions
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_num_si_repetitions != PNULL);

    /* This function composes rrc_num_si_repetitions */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_NUM_SI_REPETITIONS;
    tlv_length = rrc_il_get_rrc_num_si_repetitions_tlv_len(p_rrc_num_si_repetitions);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_num_si_repetitions->num_of_repetitions > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_num_si_repetitions->num_of_repetitions] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_rrc_num_si_repetitions->num_of_repetitions);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_num_si_repetitions->num_of_repetitions, "num_of_repetitions");
    *pp_buffer += sizeof(p_rrc_num_si_repetitions->num_of_repetitions);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_num_si_transmissions_len
(
    rrc_num_si_transmissions_t *p_rrc_num_si_transmissions
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_num_si_transmissions != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_num_si_transmissions->num_of_transmissions);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_num_si_transmissions_tlv_len
(
    rrc_num_si_transmissions_t *p_rrc_num_si_transmissions

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_num_si_transmissions != PNULL);

    length += rrc_il_get_rrc_num_si_transmissions_len(p_rrc_num_si_transmissions);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_num_si_transmissions
(
    U8  **pp_buffer,
    rrc_num_si_transmissions_t *p_rrc_num_si_transmissions
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_num_si_transmissions != PNULL);

    /* This function composes rrc_num_si_transmissions */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_NUM_SI_TRANSMISSIONS;
    tlv_length = rrc_il_get_rrc_num_si_transmissions_tlv_len(p_rrc_num_si_transmissions);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_num_si_transmissions->num_of_transmissions < 1) || (p_rrc_num_si_transmissions->num_of_transmissions > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_num_si_transmissions->num_of_transmissions] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_num_si_transmissions->num_of_transmissions);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_num_si_transmissions->num_of_transmissions, "num_of_transmissions");
    *pp_buffer += sizeof(p_rrc_num_si_transmissions->num_of_transmissions);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_si_start_offset_info_len
(
    rrc_mac_si_start_offset_info_t *p_rrc_mac_si_start_offset_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_si_start_offset_info != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_mac_si_start_offset_info->si_start_offset);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_si_start_offset_info_tlv_len
(
    rrc_mac_si_start_offset_info_t *p_rrc_mac_si_start_offset_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_si_start_offset_info != PNULL);

    length += rrc_il_get_rrc_mac_si_start_offset_info_len(p_rrc_mac_si_start_offset_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_si_start_offset_info
(
    U8  **pp_buffer,
    rrc_mac_si_start_offset_info_t *p_rrc_mac_si_start_offset_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_si_start_offset_info != PNULL);

    /* This function composes rrc_mac_si_start_offset_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SI_START_OFFSET;
    tlv_length = rrc_il_get_rrc_mac_si_start_offset_info_tlv_len(p_rrc_mac_si_start_offset_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose OCTET_STRING FIXED of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_si_start_offset_info->si_start_offset); loop++)
        {
            if (p_rrc_mac_si_start_offset_info->si_start_offset[loop] > 39)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_rrc_mac_si_start_offset_info->si_start_offset[%u] should be less"
                    " than or equal to 39. "
                    "Incorrect value %d received.", loop, p_rrc_mac_si_start_offset_info->si_start_offset[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_si_start_offset_info->si_start_offset[loop], "si_start_offset[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_si_segment_data_len
(
    rrc_mac_si_segment_data_t *p_rrc_mac_si_segment_data
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_si_segment_data != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_si_segment_data->si_msg_buf_length);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrc_mac_si_segment_data->si_msg_buf_length * sizeof(p_rrc_mac_si_segment_data->si_msg_buf[0]));

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_si_segment_data_tlv_len
(
    rrc_mac_si_segment_data_t *p_rrc_mac_si_segment_data

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_si_segment_data != PNULL);

    length += rrc_il_get_rrc_mac_si_segment_data_len(p_rrc_mac_si_segment_data);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_si_segment_data
(
    U8  **pp_buffer,
    rrc_mac_si_segment_data_t *p_rrc_mac_si_segment_data
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_si_segment_data != PNULL);

    /* This function composes rrc_mac_si_segment_data */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SI_MSG_SEGMENT_REQ;
    tlv_length = rrc_il_get_rrc_mac_si_segment_data_tlv_len(p_rrc_mac_si_segment_data);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_si_segment_data->si_msg_buf_length, "si_msg_buf_length");
    *pp_buffer += sizeof(p_rrc_mac_si_segment_data->si_msg_buf_length);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_mac_si_segment_data->si_msg_buf_length; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_si_segment_data->si_msg_buf[loop], "si_msg_buf[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_si_msg_param_len
(
    rrc_mac_si_msg_param_t *p_rrc_mac_si_msg_param
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_si_msg_param != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_si_msg_param->id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_si_msg_param->offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_si_msg_param->length);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_si_msg_param_tlv_len
(
    rrc_mac_si_msg_param_t *p_rrc_mac_si_msg_param

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_si_msg_param != PNULL);

    length += rrc_il_get_rrc_mac_si_msg_param_len(p_rrc_mac_si_msg_param);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_si_msg_param
(
    U8  **pp_buffer,
    rrc_mac_si_msg_param_t *p_rrc_mac_si_msg_param
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_si_msg_param != PNULL);

    /* This function composes rrc_mac_si_msg_param */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SI_MSG_INFO_PARAM;
    tlv_length = rrc_il_get_rrc_mac_si_msg_param_tlv_len(p_rrc_mac_si_msg_param);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_si_msg_param->id > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_param->id] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_si_msg_param->id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_si_msg_param->id, "id");
    *pp_buffer += sizeof(p_rrc_mac_si_msg_param->id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_mac_si_msg_param->offset, "offset");
    *pp_buffer += sizeof(p_rrc_mac_si_msg_param->offset);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_mac_si_msg_param->length, "length");
    *pp_buffer += sizeof(p_rrc_mac_si_msg_param->length);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sched_reqd_for_sib8_info_len
(
    rrc_mac_sched_reqd_for_sib8_info_t *p_rrc_mac_sched_reqd_for_sib8_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sched_reqd_for_sib8_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sched_reqd_for_sib8_info->sched_reqd_for_sib8);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sched_reqd_for_sib8_info_tlv_len
(
    rrc_mac_sched_reqd_for_sib8_info_t *p_rrc_mac_sched_reqd_for_sib8_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sched_reqd_for_sib8_info != PNULL);

    length += rrc_il_get_rrc_mac_sched_reqd_for_sib8_info_len(p_rrc_mac_sched_reqd_for_sib8_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sched_reqd_for_sib8_info
(
    U8  **pp_buffer,
    rrc_mac_sched_reqd_for_sib8_info_t *p_rrc_mac_sched_reqd_for_sib8_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sched_reqd_for_sib8_info != PNULL);

    /* This function composes rrc_mac_sched_reqd_for_sib8_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_SCHED_REQD_FOR_SIB8;
    tlv_length = rrc_il_get_rrc_mac_sched_reqd_for_sib8_info_tlv_len(p_rrc_mac_sched_reqd_for_sib8_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sched_reqd_for_sib8_info->sched_reqd_for_sib8 > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sched_reqd_for_sib8_info->sched_reqd_for_sib8] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_sched_reqd_for_sib8_info->sched_reqd_for_sib8);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sched_reqd_for_sib8_info->sched_reqd_for_sib8, "sched_reqd_for_sib8");
    *pp_buffer += sizeof(p_rrc_mac_sched_reqd_for_sib8_info->sched_reqd_for_sib8);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_si_msg_info_len
(
    rrc_mac_si_msg_info_t *p_rrc_mac_si_msg_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_si_msg_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_si_msg_info->si_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_si_msg_info->periodicity);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_si_msg_info->starting_sfn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_si_msg_info->starting_sf);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_si_msg_info->num_of_segment);

    /* Get length of TLV */
    if (p_rrc_mac_si_msg_info->optional_elems_present & RRC_NUM_SI_REPETITIONS_PRESENT)
    {
        length += rrc_il_get_rrc_num_si_repetitions_tlv_len(&p_rrc_mac_si_msg_info->num_si_repetitions);
    }

    /* Get length of TLV */
    if (p_rrc_mac_si_msg_info->optional_elems_present & RRC_NUM_SI_TRANSMISSIONS_PRESENT)
    {
        length += rrc_il_get_rrc_num_si_transmissions_tlv_len(&p_rrc_mac_si_msg_info->num_si_transmissions);
    }

    /* Get length of TLV */
    if (p_rrc_mac_si_msg_info->optional_elems_present & RRC_NUM_SI_START_OFFSET_PRESENT)
    {
        length += rrc_il_get_rrc_mac_si_start_offset_info_tlv_len(&p_rrc_mac_si_msg_info->si_start_offset_info);
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_si_msg_info->segment_counter <= ARRSIZE(p_rrc_mac_si_msg_info->si_segment_data));
        for (loop = 0; loop < p_rrc_mac_si_msg_info->segment_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_si_segment_data_tlv_len(&p_rrc_mac_si_msg_info->si_segment_data[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_si_msg_info->si_msg_param_counter <= ARRSIZE(p_rrc_mac_si_msg_info->si_msg_param));
        for (loop = 0; loop < p_rrc_mac_si_msg_info->si_msg_param_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_si_msg_param_tlv_len(&p_rrc_mac_si_msg_info->si_msg_param[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_mac_si_msg_info->optional_elems_present & RRC_SCHED_REQD_FOR_SIB8_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sched_reqd_for_sib8_info_tlv_len(&p_rrc_mac_si_msg_info->rrc_mac_sched_reqd_for_sib8);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_si_msg_info_tlv_len
(
    rrc_mac_si_msg_info_t *p_rrc_mac_si_msg_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_si_msg_info != PNULL);

    length += rrc_il_get_rrc_mac_si_msg_info_len(p_rrc_mac_si_msg_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_si_msg_info
(
    U8  **pp_buffer,
    rrc_mac_si_msg_info_t *p_rrc_mac_si_msg_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_si_msg_info != PNULL);

    /* This function composes rrc_mac_si_msg_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SI_MSG_REQ;
    tlv_length = rrc_il_get_rrc_mac_si_msg_info_tlv_len(p_rrc_mac_si_msg_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_si_msg_info->si_index > 11))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_info->si_index] should be less than"
            " or equal to 11. Incorrect value %u received.", p_rrc_mac_si_msg_info->si_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_si_msg_info->si_index, "si_index");
    *pp_buffer += sizeof(p_rrc_mac_si_msg_info->si_index);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_si_msg_info->periodicity < 1) || (p_rrc_mac_si_msg_info->periodicity > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_info->periodicity] should be in range "
            "1 to 7. Incorrect value %u received.", p_rrc_mac_si_msg_info->periodicity);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_si_msg_info->periodicity, "periodicity");
    *pp_buffer += sizeof(p_rrc_mac_si_msg_info->periodicity);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_si_msg_info->starting_sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_info->starting_sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_si_msg_info->starting_sfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_si_msg_info->starting_sfn, "starting_sfn");
    *pp_buffer += sizeof(p_rrc_mac_si_msg_info->starting_sfn);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_si_msg_info->starting_sf > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_info->starting_sf] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_si_msg_info->starting_sf);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_si_msg_info->starting_sf, "starting_sf");
    *pp_buffer += sizeof(p_rrc_mac_si_msg_info->starting_sf);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_si_msg_info->num_of_segment < 1) || (p_rrc_mac_si_msg_info->num_of_segment > 64))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_info->num_of_segment] should be in range "
            "1 to 64. Incorrect value %u received.", p_rrc_mac_si_msg_info->num_of_segment);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_si_msg_info->num_of_segment, "num_of_segment");
    *pp_buffer += sizeof(p_rrc_mac_si_msg_info->num_of_segment);

    /* Compose TLV  */
    if (p_rrc_mac_si_msg_info->optional_elems_present & RRC_NUM_SI_REPETITIONS_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_num_si_repetitions(pp_buffer, &p_rrc_mac_si_msg_info->num_si_repetitions))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_si_msg_info->optional_elems_present & RRC_NUM_SI_TRANSMISSIONS_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_num_si_transmissions(pp_buffer, &p_rrc_mac_si_msg_info->num_si_transmissions))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_si_msg_info->optional_elems_present & RRC_NUM_SI_START_OFFSET_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_si_start_offset_info(pp_buffer, &p_rrc_mac_si_msg_info->si_start_offset_info))
        {
            return RRC_FAILURE;
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_si_msg_info->segment_counter <= ARRSIZE(p_rrc_mac_si_msg_info->si_segment_data));
        for (loop = 0; loop < p_rrc_mac_si_msg_info->segment_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_si_segment_data(pp_buffer, &p_rrc_mac_si_msg_info->si_segment_data[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_si_msg_info->si_msg_param_counter <= ARRSIZE(p_rrc_mac_si_msg_info->si_msg_param));
        for (loop = 0; loop < p_rrc_mac_si_msg_info->si_msg_param_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_si_msg_param(pp_buffer, &p_rrc_mac_si_msg_info->si_msg_param[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_si_msg_info->optional_elems_present & RRC_SCHED_REQD_FOR_SIB8_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sched_reqd_for_sib8_info(pp_buffer, &p_rrc_mac_si_msg_info->rrc_mac_sched_reqd_for_sib8))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_si_msg_req_len
(
    rrc_mac_si_msg_req_t *p_rrc_mac_si_msg_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_si_msg_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_si_msg_req->si_window_size);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_si_msg_req->num_si_message);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_si_msg_req->si_msg_info_counter <= ARRSIZE(p_rrc_mac_si_msg_req->si_msg_info));
        for (loop = 0; loop < p_rrc_mac_si_msg_req->si_msg_info_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_si_msg_info_tlv_len(&p_rrc_mac_si_msg_req->si_msg_info[loop]);
        }
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_si_msg_req_tlv_len
(
    rrc_mac_si_msg_req_t *p_rrc_mac_si_msg_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_si_msg_req != PNULL);

    length += rrc_il_get_rrc_mac_si_msg_req_len(p_rrc_mac_si_msg_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_si_msg_req
(
    U8  **pp_buffer,
    rrc_mac_si_msg_req_t *p_rrc_mac_si_msg_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_si_msg_req != PNULL);

    /* This function composes rrc_mac_si_msg_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SI_MSG_INFO;
    tlv_length = rrc_il_get_rrc_mac_si_msg_req_tlv_len(p_rrc_mac_si_msg_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_si_msg_req->si_window_size < 1) || (p_rrc_mac_si_msg_req->si_window_size > 40))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_req->si_window_size] should be in range "
            "1 to 40. Incorrect value %u received.", p_rrc_mac_si_msg_req->si_window_size);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_si_msg_req->si_window_size, "si_window_size");
    *pp_buffer += sizeof(p_rrc_mac_si_msg_req->si_window_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_si_msg_req->num_si_message > 74))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_si_msg_req->num_si_message] should be less than"
            " or equal to 74. Incorrect value %u received.", p_rrc_mac_si_msg_req->num_si_message);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_si_msg_req->num_si_message, "num_si_message");
    *pp_buffer += sizeof(p_rrc_mac_si_msg_req->num_si_message);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_si_msg_req->si_msg_info_counter <= ARRSIZE(p_rrc_mac_si_msg_req->si_msg_info));
        for (loop = 0; loop < p_rrc_mac_si_msg_req->si_msg_info_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_si_msg_info(pp_buffer, &p_rrc_mac_si_msg_req->si_msg_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_srs_max_uppts_len
(
    rrc_mac_srs_max_uppts_t *p_rrc_mac_srs_max_uppts
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_srs_max_uppts != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_max_uppts->max_uppts);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_srs_max_uppts_tlv_len
(
    rrc_mac_srs_max_uppts_t *p_rrc_mac_srs_max_uppts

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_srs_max_uppts != PNULL);

    length += rrc_il_get_rrc_mac_srs_max_uppts_len(p_rrc_mac_srs_max_uppts);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_srs_max_uppts
(
    U8  **pp_buffer,
    rrc_mac_srs_max_uppts_t *p_rrc_mac_srs_max_uppts
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_srs_max_uppts != PNULL);

    /* This function composes rrc_mac_srs_max_uppts */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SRS_MAX_UPPTS;
    tlv_length = rrc_il_get_rrc_mac_srs_max_uppts_tlv_len(p_rrc_mac_srs_max_uppts);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_max_uppts->max_uppts > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_max_uppts->max_uppts] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_mac_srs_max_uppts->max_uppts);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_srs_max_uppts->max_uppts, "max_uppts");
    *pp_buffer += sizeof(p_rrc_mac_srs_max_uppts->max_uppts);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_srs_common_setup_info_len
(
    rrc_mac_srs_common_setup_info_t *p_rrc_mac_srs_common_setup_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_srs_common_setup_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_common_setup_info->srs_bandwidth_config);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_common_setup_info->srs_subframe_config);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_common_setup_info->ack_nack_srs_simultaneous_trans);

    /* Get length of TLV */
    if (p_rrc_mac_srs_common_setup_info->optional_elems_present & RRC_MAC_SRS_MAX_UPPTS_PRESENT)
    {
        length += rrc_il_get_rrc_mac_srs_max_uppts_tlv_len(&p_rrc_mac_srs_common_setup_info->srs_max_uppts);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_srs_common_setup_info_tlv_len
(
    rrc_mac_srs_common_setup_info_t *p_rrc_mac_srs_common_setup_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_srs_common_setup_info != PNULL);

    length += rrc_il_get_rrc_mac_srs_common_setup_info_len(p_rrc_mac_srs_common_setup_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_srs_common_setup_info
(
    U8  **pp_buffer,
    rrc_mac_srs_common_setup_info_t *p_rrc_mac_srs_common_setup_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_srs_common_setup_info != PNULL);

    /* This function composes rrc_mac_srs_common_setup_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SRS_COMMON_SETUP_INFO;
    tlv_length = rrc_il_get_rrc_mac_srs_common_setup_info_tlv_len(p_rrc_mac_srs_common_setup_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_common_setup_info->srs_bandwidth_config > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_common_setup_info->srs_bandwidth_config] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_srs_common_setup_info->srs_bandwidth_config);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_srs_common_setup_info->srs_bandwidth_config, "srs_bandwidth_config");
    *pp_buffer += sizeof(p_rrc_mac_srs_common_setup_info->srs_bandwidth_config);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_common_setup_info->srs_subframe_config > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_common_setup_info->srs_subframe_config] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_srs_common_setup_info->srs_subframe_config);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_srs_common_setup_info->srs_subframe_config, "srs_subframe_config");
    *pp_buffer += sizeof(p_rrc_mac_srs_common_setup_info->srs_subframe_config);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_common_setup_info->ack_nack_srs_simultaneous_trans > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_common_setup_info->ack_nack_srs_simultaneous_trans] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_srs_common_setup_info->ack_nack_srs_simultaneous_trans);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_srs_common_setup_info->ack_nack_srs_simultaneous_trans, "ack_nack_srs_simultaneous_trans");
    *pp_buffer += sizeof(p_rrc_mac_srs_common_setup_info->ack_nack_srs_simultaneous_trans);

    /* Compose TLV  */
    if (p_rrc_mac_srs_common_setup_info->optional_elems_present & RRC_MAC_SRS_MAX_UPPTS_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_srs_max_uppts(pp_buffer, &p_rrc_mac_srs_common_setup_info->srs_max_uppts))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_srs_common_config_len
(
    rrc_mac_srs_common_config_t *p_rrc_mac_srs_common_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_srs_common_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_common_config->config_type);

    /* Get length of TLV */
    if (p_rrc_mac_srs_common_config->optional_elems_present & RRC_MAC_SRS_COMMON_SETUP_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_srs_common_setup_info_tlv_len(&p_rrc_mac_srs_common_config->srs_common_setup_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_srs_common_config_tlv_len
(
    rrc_mac_srs_common_config_t *p_rrc_mac_srs_common_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_srs_common_config != PNULL);

    length += rrc_il_get_rrc_mac_srs_common_config_len(p_rrc_mac_srs_common_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_srs_common_config
(
    U8  **pp_buffer,
    rrc_mac_srs_common_config_t *p_rrc_mac_srs_common_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_srs_common_config != PNULL);

    /* This function composes rrc_mac_srs_common_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SRS_COMMON_CONFIG;
    tlv_length = rrc_il_get_rrc_mac_srs_common_config_tlv_len(p_rrc_mac_srs_common_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_common_config->config_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_common_config->config_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_srs_common_config->config_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_srs_common_config->config_type, "config_type");
    *pp_buffer += sizeof(p_rrc_mac_srs_common_config->config_type);

    /* Compose TLV  */
    if (p_rrc_mac_srs_common_config->optional_elems_present & RRC_MAC_SRS_COMMON_SETUP_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_srs_common_setup_info(pp_buffer, &p_rrc_mac_srs_common_config->srs_common_setup_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pdsch_config_common_info_len
(
    rrc_mac_pdsch_config_common_info_t *p_rrc_mac_pdsch_config_common_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pdsch_config_common_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pdsch_config_common_info->reference_signal_power);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pdsch_config_common_info->p_b);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pdsch_config_common_info_tlv_len
(
    rrc_mac_pdsch_config_common_info_t *p_rrc_mac_pdsch_config_common_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pdsch_config_common_info != PNULL);

    length += rrc_il_get_rrc_mac_pdsch_config_common_info_len(p_rrc_mac_pdsch_config_common_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_pdsch_config_common_info
(
    U8  **pp_buffer,
    rrc_mac_pdsch_config_common_info_t *p_rrc_mac_pdsch_config_common_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_pdsch_config_common_info != PNULL);

    /* This function composes rrc_mac_pdsch_config_common_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PDSCH_CONFIG_COMMON_INFO;
    tlv_length = rrc_il_get_rrc_mac_pdsch_config_common_info_tlv_len(p_rrc_mac_pdsch_config_common_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_pdsch_config_common_info->reference_signal_power < -60) || (p_rrc_mac_pdsch_config_common_info->reference_signal_power > 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pdsch_config_common_info->reference_signal_power] should be in range "
            "-60 to 50. Incorrect value %d received.", p_rrc_mac_pdsch_config_common_info->reference_signal_power);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_mac_pdsch_config_common_info->reference_signal_power, "reference_signal_power");
    *pp_buffer += sizeof(p_rrc_mac_pdsch_config_common_info->reference_signal_power);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pdsch_config_common_info->p_b > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pdsch_config_common_info->p_b] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_pdsch_config_common_info->p_b);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pdsch_config_common_info->p_b, "p_b");
    *pp_buffer += sizeof(p_rrc_mac_pdsch_config_common_info->p_b);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_downlink_power_control_common_len
(
    rrc_mac_downlink_power_control_common_t *p_rrc_mac_downlink_power_control_common
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_downlink_power_control_common != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_downlink_power_control_common->pcfichPowerOffset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_downlink_power_control_common->phichPowerOffset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_downlink_power_control_common->pdcchPowerOffset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_downlink_power_control_common->pbchTransmissionPower);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_downlink_power_control_common->pchTransmissionpower);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_downlink_power_control_common_tlv_len
(
    rrc_mac_downlink_power_control_common_t *p_rrc_mac_downlink_power_control_common

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_downlink_power_control_common != PNULL);

    length += rrc_il_get_rrc_mac_downlink_power_control_common_len(p_rrc_mac_downlink_power_control_common);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_downlink_power_control_common
(
    U8  **pp_buffer,
    rrc_mac_downlink_power_control_common_t *p_rrc_mac_downlink_power_control_common
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_downlink_power_control_common != PNULL);

    /* This function composes rrc_mac_downlink_power_control_common */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DOWNLINK_POWER_CONTROL_COMMON;
    tlv_length = rrc_il_get_rrc_mac_downlink_power_control_common_tlv_len(p_rrc_mac_downlink_power_control_common);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_downlink_power_control_common->pcfichPowerOffset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_downlink_power_control_common->pcfichPowerOffset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_mac_downlink_power_control_common->pcfichPowerOffset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_downlink_power_control_common->pcfichPowerOffset, "pcfichPowerOffset");
    *pp_buffer += sizeof(p_rrc_mac_downlink_power_control_common->pcfichPowerOffset);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_downlink_power_control_common->phichPowerOffset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_downlink_power_control_common->phichPowerOffset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_mac_downlink_power_control_common->phichPowerOffset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_downlink_power_control_common->phichPowerOffset, "phichPowerOffset");
    *pp_buffer += sizeof(p_rrc_mac_downlink_power_control_common->phichPowerOffset);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_downlink_power_control_common->pdcchPowerOffset > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_downlink_power_control_common->pdcchPowerOffset] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_mac_downlink_power_control_common->pdcchPowerOffset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_downlink_power_control_common->pdcchPowerOffset, "pdcchPowerOffset");
    *pp_buffer += sizeof(p_rrc_mac_downlink_power_control_common->pdcchPowerOffset);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_downlink_power_control_common->pbchTransmissionPower > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_downlink_power_control_common->pbchTransmissionPower] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_mac_downlink_power_control_common->pbchTransmissionPower);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_downlink_power_control_common->pbchTransmissionPower, "pbchTransmissionPower");
    *pp_buffer += sizeof(p_rrc_mac_downlink_power_control_common->pbchTransmissionPower);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_downlink_power_control_common->pchTransmissionpower > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_downlink_power_control_common->pchTransmissionpower] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_mac_downlink_power_control_common->pchTransmissionpower);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_downlink_power_control_common->pchTransmissionpower, "pchTransmissionpower");
    *pp_buffer += sizeof(p_rrc_mac_downlink_power_control_common->pchTransmissionpower);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_uplink_power_control_common_len
(
    rrc_mac_uplink_power_control_common_t *p_rrc_mac_uplink_power_control_common
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_uplink_power_control_common != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_uplink_power_control_common->p0_nominal_pusch);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_uplink_power_control_common->alpha);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_uplink_power_control_common_tlv_len
(
    rrc_mac_uplink_power_control_common_t *p_rrc_mac_uplink_power_control_common

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_uplink_power_control_common != PNULL);

    length += rrc_il_get_rrc_mac_uplink_power_control_common_len(p_rrc_mac_uplink_power_control_common);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_uplink_power_control_common
(
    U8  **pp_buffer,
    rrc_mac_uplink_power_control_common_t *p_rrc_mac_uplink_power_control_common
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_uplink_power_control_common != PNULL);

    /* This function composes rrc_mac_uplink_power_control_common */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UPLINK_POWER_CONTROL_COMMON;
    tlv_length = rrc_il_get_rrc_mac_uplink_power_control_common_tlv_len(p_rrc_mac_uplink_power_control_common);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_uplink_power_control_common->p0_nominal_pusch < -126) || (p_rrc_mac_uplink_power_control_common->p0_nominal_pusch > 24))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_uplink_power_control_common->p0_nominal_pusch] should be in range "
            "-126 to 24. Incorrect value %d received.", p_rrc_mac_uplink_power_control_common->p0_nominal_pusch);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_mac_uplink_power_control_common->p0_nominal_pusch, "p0_nominal_pusch");
    *pp_buffer += sizeof(p_rrc_mac_uplink_power_control_common->p0_nominal_pusch);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_uplink_power_control_common->alpha > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_uplink_power_control_common->alpha] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_uplink_power_control_common->alpha);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_uplink_power_control_common->alpha, "alpha");
    *pp_buffer += sizeof(p_rrc_mac_uplink_power_control_common->alpha);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pusch_config_common_info_len
(
    rrc_mac_pusch_config_common_info_t *p_rrc_mac_pusch_config_common_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pusch_config_common_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pusch_config_common_info->enable_64_qam);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pusch_config_common_info_tlv_len
(
    rrc_mac_pusch_config_common_info_t *p_rrc_mac_pusch_config_common_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pusch_config_common_info != PNULL);

    length += rrc_il_get_rrc_mac_pusch_config_common_info_len(p_rrc_mac_pusch_config_common_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_pusch_config_common_info
(
    U8  **pp_buffer,
    rrc_mac_pusch_config_common_info_t *p_rrc_mac_pusch_config_common_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_pusch_config_common_info != PNULL);

    /* This function composes rrc_mac_pusch_config_common_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PUSCH_CONFIG_COMMON;
    tlv_length = rrc_il_get_rrc_mac_pusch_config_common_info_tlv_len(p_rrc_mac_pusch_config_common_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pusch_config_common_info->enable_64_qam > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pusch_config_common_info->enable_64_qam] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_pusch_config_common_info->enable_64_qam);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pusch_config_common_info->enable_64_qam, "enable_64_qam");
    *pp_buffer += sizeof(p_rrc_mac_pusch_config_common_info->enable_64_qam);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_freq_selective_scheduling_len
(
    rrc_mac_freq_selective_scheduling_t *p_rrc_mac_freq_selective_scheduling
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_freq_selective_scheduling != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_freq_selective_scheduling->ul_freq_selective_enable);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_freq_selective_scheduling->dl_freq_selective_enable);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_freq_selective_scheduling_tlv_len
(
    rrc_mac_freq_selective_scheduling_t *p_rrc_mac_freq_selective_scheduling

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_freq_selective_scheduling != PNULL);

    length += rrc_il_get_rrc_mac_freq_selective_scheduling_len(p_rrc_mac_freq_selective_scheduling);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_freq_selective_scheduling
(
    U8  **pp_buffer,
    rrc_mac_freq_selective_scheduling_t *p_rrc_mac_freq_selective_scheduling
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_freq_selective_scheduling != PNULL);

    /* This function composes rrc_mac_freq_selective_scheduling */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_FREQ_SELECTIVE_ENABLE;
    tlv_length = rrc_il_get_rrc_mac_freq_selective_scheduling_tlv_len(p_rrc_mac_freq_selective_scheduling);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_freq_selective_scheduling->ul_freq_selective_enable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_freq_selective_scheduling->ul_freq_selective_enable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_freq_selective_scheduling->ul_freq_selective_enable);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_freq_selective_scheduling->ul_freq_selective_enable, "ul_freq_selective_enable");
    *pp_buffer += sizeof(p_rrc_mac_freq_selective_scheduling->ul_freq_selective_enable);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_freq_selective_scheduling->dl_freq_selective_enable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_freq_selective_scheduling->dl_freq_selective_enable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_freq_selective_scheduling->dl_freq_selective_enable);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_freq_selective_scheduling->dl_freq_selective_enable, "dl_freq_selective_enable");
    *pp_buffer += sizeof(p_rrc_mac_freq_selective_scheduling->dl_freq_selective_enable);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_contn_free_rach_timer_info_len
(
    rrc_mac_contn_free_rach_timer_info_t *p_rrc_mac_contn_free_rach_timer_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_contn_free_rach_timer_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_contn_free_rach_timer_info->contn_free_rach_timer);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_contn_free_rach_timer_info_tlv_len
(
    rrc_mac_contn_free_rach_timer_info_t *p_rrc_mac_contn_free_rach_timer_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_contn_free_rach_timer_info != PNULL);

    length += rrc_il_get_rrc_mac_contn_free_rach_timer_info_len(p_rrc_mac_contn_free_rach_timer_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_contn_free_rach_timer_info
(
    U8  **pp_buffer,
    rrc_mac_contn_free_rach_timer_info_t *p_rrc_mac_contn_free_rach_timer_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_contn_free_rach_timer_info != PNULL);

    /* This function composes rrc_mac_contn_free_rach_timer_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CONTN_FREE_RACH_TIMER_INFO;
    tlv_length = rrc_il_get_rrc_mac_contn_free_rach_timer_info_tlv_len(p_rrc_mac_contn_free_rach_timer_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_contn_free_rach_timer_info->contn_free_rach_timer < 25) || (p_rrc_mac_contn_free_rach_timer_info->contn_free_rach_timer > 200))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_contn_free_rach_timer_info->contn_free_rach_timer] should be in range "
            "25 to 200. Incorrect value %u received.", p_rrc_mac_contn_free_rach_timer_info->contn_free_rach_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_contn_free_rach_timer_info->contn_free_rach_timer, "contn_free_rach_timer");
    *pp_buffer += sizeof(p_rrc_mac_contn_free_rach_timer_info->contn_free_rach_timer);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_rnti_range_len
(
    rrc_mac_tpc_rnti_range_t *p_rrc_mac_tpc_rnti_range
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tpc_rnti_range != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tpc_rnti_range->startTpcRntiPucch);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tpc_rnti_range->endTpcRntiPucch);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tpc_rnti_range->startTpcRntiPusch);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tpc_rnti_range->endTpcRntiPusch);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_rnti_range_tlv_len
(
    rrc_mac_tpc_rnti_range_t *p_rrc_mac_tpc_rnti_range

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tpc_rnti_range != PNULL);

    length += rrc_il_get_rrc_mac_tpc_rnti_range_len(p_rrc_mac_tpc_rnti_range);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_tpc_rnti_range
(
    U8  **pp_buffer,
    rrc_mac_tpc_rnti_range_t *p_rrc_mac_tpc_rnti_range
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_tpc_rnti_range != PNULL);

    /* This function composes rrc_mac_tpc_rnti_range */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_TPC_RNTI_RANGE;
    tlv_length = rrc_il_get_rrc_mac_tpc_rnti_range_tlv_len(p_rrc_mac_tpc_rnti_range);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tpc_rnti_range->startTpcRntiPucch < 61) || (p_rrc_mac_tpc_rnti_range->startTpcRntiPucch > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_rnti_range->startTpcRntiPucch] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_tpc_rnti_range->startTpcRntiPucch);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_tpc_rnti_range->startTpcRntiPucch, "startTpcRntiPucch");
    *pp_buffer += sizeof(p_rrc_mac_tpc_rnti_range->startTpcRntiPucch);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tpc_rnti_range->endTpcRntiPucch < 61) || (p_rrc_mac_tpc_rnti_range->endTpcRntiPucch > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_rnti_range->endTpcRntiPucch] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_tpc_rnti_range->endTpcRntiPucch);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_tpc_rnti_range->endTpcRntiPucch, "endTpcRntiPucch");
    *pp_buffer += sizeof(p_rrc_mac_tpc_rnti_range->endTpcRntiPucch);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tpc_rnti_range->startTpcRntiPusch < 61) || (p_rrc_mac_tpc_rnti_range->startTpcRntiPusch > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_rnti_range->startTpcRntiPusch] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_tpc_rnti_range->startTpcRntiPusch);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_tpc_rnti_range->startTpcRntiPusch, "startTpcRntiPusch");
    *pp_buffer += sizeof(p_rrc_mac_tpc_rnti_range->startTpcRntiPusch);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tpc_rnti_range->endTpcRntiPusch < 61) || (p_rrc_mac_tpc_rnti_range->endTpcRntiPusch > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_rnti_range->endTpcRntiPusch] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_tpc_rnti_range->endTpcRntiPusch);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_tpc_rnti_range->endTpcRntiPusch, "endTpcRntiPusch");
    *pp_buffer += sizeof(p_rrc_mac_tpc_rnti_range->endTpcRntiPusch);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_power_control_enable_len
(
    rrc_mac_power_control_enable_t *p_rrc_mac_power_control_enable
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_power_control_enable != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_power_control_enable->harqBlerClpcPucchEnable);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_power_control_enable->cqiSinrClpcPucchEnable);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_power_control_enable->clpcPuschEnable);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_power_control_enable->pdcchPowOrAggregationEnable);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_power_control_enable->clpcPuschfreqSelectiveEnable);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_power_control_enable_tlv_len
(
    rrc_mac_power_control_enable_t *p_rrc_mac_power_control_enable

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_power_control_enable != PNULL);

    length += rrc_il_get_rrc_mac_power_control_enable_len(p_rrc_mac_power_control_enable);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_power_control_enable
(
    U8  **pp_buffer,
    rrc_mac_power_control_enable_t *p_rrc_mac_power_control_enable
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_power_control_enable != PNULL);

    /* This function composes rrc_mac_power_control_enable */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_POWER_CONTROL_ENABLE;
    tlv_length = rrc_il_get_rrc_mac_power_control_enable_tlv_len(p_rrc_mac_power_control_enable);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_power_control_enable->harqBlerClpcPucchEnable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_power_control_enable->harqBlerClpcPucchEnable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_power_control_enable->harqBlerClpcPucchEnable);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_power_control_enable->harqBlerClpcPucchEnable, "harqBlerClpcPucchEnable");
    *pp_buffer += sizeof(p_rrc_mac_power_control_enable->harqBlerClpcPucchEnable);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_power_control_enable->cqiSinrClpcPucchEnable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_power_control_enable->cqiSinrClpcPucchEnable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_power_control_enable->cqiSinrClpcPucchEnable);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_power_control_enable->cqiSinrClpcPucchEnable, "cqiSinrClpcPucchEnable");
    *pp_buffer += sizeof(p_rrc_mac_power_control_enable->cqiSinrClpcPucchEnable);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_power_control_enable->clpcPuschEnable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_power_control_enable->clpcPuschEnable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_power_control_enable->clpcPuschEnable);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_power_control_enable->clpcPuschEnable, "clpcPuschEnable");
    *pp_buffer += sizeof(p_rrc_mac_power_control_enable->clpcPuschEnable);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_power_control_enable->pdcchPowOrAggregationEnable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_power_control_enable->pdcchPowOrAggregationEnable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_power_control_enable->pdcchPowOrAggregationEnable);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_power_control_enable->pdcchPowOrAggregationEnable, "pdcchPowOrAggregationEnable");
    *pp_buffer += sizeof(p_rrc_mac_power_control_enable->pdcchPowOrAggregationEnable);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_power_control_enable->clpcPuschfreqSelectiveEnable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_power_control_enable->clpcPuschfreqSelectiveEnable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_power_control_enable->clpcPuschfreqSelectiveEnable);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_power_control_enable->clpcPuschfreqSelectiveEnable, "clpcPuschfreqSelectiveEnable");
    *pp_buffer += sizeof(p_rrc_mac_power_control_enable->clpcPuschfreqSelectiveEnable);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_explicit_cell_start_required_len
(
    rrc_mac_explicit_cell_start_required_t *p_rrc_mac_explicit_cell_start_required
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_explicit_cell_start_required != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_explicit_cell_start_required->explicit_start_required);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_explicit_cell_start_required_tlv_len
(
    rrc_mac_explicit_cell_start_required_t *p_rrc_mac_explicit_cell_start_required

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_explicit_cell_start_required != PNULL);

    length += rrc_il_get_rrc_mac_explicit_cell_start_required_len(p_rrc_mac_explicit_cell_start_required);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_explicit_cell_start_required
(
    U8  **pp_buffer,
    rrc_mac_explicit_cell_start_required_t *p_rrc_mac_explicit_cell_start_required
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_explicit_cell_start_required != PNULL);

    /* This function composes rrc_mac_explicit_cell_start_required */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_EXPLICIT_CELL_START_REQUIRED;
    tlv_length = rrc_il_get_rrc_mac_explicit_cell_start_required_tlv_len(p_rrc_mac_explicit_cell_start_required);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_explicit_cell_start_required->explicit_start_required > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_explicit_cell_start_required->explicit_start_required] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_mac_explicit_cell_start_required->explicit_start_required);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_explicit_cell_start_required->explicit_start_required, "explicit_start_required");
    *pp_buffer += sizeof(p_rrc_mac_explicit_cell_start_required->explicit_start_required);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ul_sync_loss_timer_info_len
(
    rrc_mac_ul_sync_loss_timer_info_t *p_rrc_mac_ul_sync_loss_timer_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ul_sync_loss_timer_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ul_sync_loss_timer_info->ul_sync_loss_timer_value);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ul_sync_loss_timer_info_tlv_len
(
    rrc_mac_ul_sync_loss_timer_info_t *p_rrc_mac_ul_sync_loss_timer_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ul_sync_loss_timer_info != PNULL);

    length += rrc_il_get_rrc_mac_ul_sync_loss_timer_info_len(p_rrc_mac_ul_sync_loss_timer_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ul_sync_loss_timer_info
(
    U8  **pp_buffer,
    rrc_mac_ul_sync_loss_timer_info_t *p_rrc_mac_ul_sync_loss_timer_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ul_sync_loss_timer_info != PNULL);

    /* This function composes rrc_mac_ul_sync_loss_timer_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UL_SYNC_LOSS_TIMER_INFO;
    tlv_length = rrc_il_get_rrc_mac_ul_sync_loss_timer_info_tlv_len(p_rrc_mac_ul_sync_loss_timer_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ul_sync_loss_timer_info->ul_sync_loss_timer_value, "ul_sync_loss_timer_value");
    *pp_buffer += sizeof(p_rrc_mac_ul_sync_loss_timer_info->ul_sync_loss_timer_value);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_cqi_sinr_threshold_info_len
(
    rrc_mac_pucch_cqi_sinr_threshold_info_t *p_rrc_mac_pucch_cqi_sinr_threshold_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pucch_cqi_sinr_threshold_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pucch_cqi_sinr_threshold_info->sinr_value);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_cqi_sinr_threshold_info_tlv_len
(
    rrc_mac_pucch_cqi_sinr_threshold_info_t *p_rrc_mac_pucch_cqi_sinr_threshold_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pucch_cqi_sinr_threshold_info != PNULL);

    length += rrc_il_get_rrc_mac_pucch_cqi_sinr_threshold_info_len(p_rrc_mac_pucch_cqi_sinr_threshold_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_pucch_cqi_sinr_threshold_info
(
    U8  **pp_buffer,
    rrc_mac_pucch_cqi_sinr_threshold_info_t *p_rrc_mac_pucch_cqi_sinr_threshold_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_pucch_cqi_sinr_threshold_info != PNULL);

    /* This function composes rrc_mac_pucch_cqi_sinr_threshold_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO;
    tlv_length = rrc_il_get_rrc_mac_pucch_cqi_sinr_threshold_info_tlv_len(p_rrc_mac_pucch_cqi_sinr_threshold_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pucch_cqi_sinr_threshold_info->sinr_value, "sinr_value");
    *pp_buffer += sizeof(p_rrc_mac_pucch_cqi_sinr_threshold_info->sinr_value);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_n_gap_len
(
    rrc_mac_n_gap_t *p_rrc_mac_n_gap
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_n_gap != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_n_gap->n_gap);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_n_gap_tlv_len
(
    rrc_mac_n_gap_t *p_rrc_mac_n_gap

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_n_gap != PNULL);

    length += rrc_il_get_rrc_mac_n_gap_len(p_rrc_mac_n_gap);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_n_gap
(
    U8  **pp_buffer,
    rrc_mac_n_gap_t *p_rrc_mac_n_gap
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_n_gap != PNULL);

    /* This function composes rrc_mac_n_gap */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_N_GAP;
    tlv_length = rrc_il_get_rrc_mac_n_gap_tlv_len(p_rrc_mac_n_gap);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_n_gap->n_gap > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_n_gap->n_gap] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_n_gap->n_gap);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_n_gap->n_gap, "n_gap");
    *pp_buffer += sizeof(p_rrc_mac_n_gap->n_gap);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_crnti_range_len
(
    rrc_mac_sps_crnti_range_t *p_rrc_mac_sps_crnti_range
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_crnti_range != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sps_crnti_range->start_sps_crnti);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sps_crnti_range->end_sps_crnti);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_crnti_range_tlv_len
(
    rrc_mac_sps_crnti_range_t *p_rrc_mac_sps_crnti_range

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_crnti_range != PNULL);

    length += rrc_il_get_rrc_mac_sps_crnti_range_len(p_rrc_mac_sps_crnti_range);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_crnti_range
(
    U8  **pp_buffer,
    rrc_mac_sps_crnti_range_t *p_rrc_mac_sps_crnti_range
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sps_crnti_range != PNULL);

    /* This function composes rrc_mac_sps_crnti_range */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SPS_CRNTI_RANGE;
    tlv_length = rrc_il_get_rrc_mac_sps_crnti_range_tlv_len(p_rrc_mac_sps_crnti_range);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_sps_crnti_range->start_sps_crnti < 61) || (p_rrc_mac_sps_crnti_range->start_sps_crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_crnti_range->start_sps_crnti] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_sps_crnti_range->start_sps_crnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_sps_crnti_range->start_sps_crnti, "start_sps_crnti");
    *pp_buffer += sizeof(p_rrc_mac_sps_crnti_range->start_sps_crnti);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_sps_crnti_range->end_sps_crnti < 61) || (p_rrc_mac_sps_crnti_range->end_sps_crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_crnti_range->end_sps_crnti] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_sps_crnti_range->end_sps_crnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_sps_crnti_range->end_sps_crnti, "end_sps_crnti");
    *pp_buffer += sizeof(p_rrc_mac_sps_crnti_range->end_sps_crnti);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dynamic_pdcch_info_len
(
    rrc_mac_dynamic_pdcch_info_t *p_rrc_mac_dynamic_pdcch_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dynamic_pdcch_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_dynamic_pdcch_info->isDynamicPdcchDisabled);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dynamic_pdcch_info_tlv_len
(
    rrc_mac_dynamic_pdcch_info_t *p_rrc_mac_dynamic_pdcch_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dynamic_pdcch_info != PNULL);

    length += rrc_il_get_rrc_mac_dynamic_pdcch_info_len(p_rrc_mac_dynamic_pdcch_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_dynamic_pdcch_info
(
    U8  **pp_buffer,
    rrc_mac_dynamic_pdcch_info_t *p_rrc_mac_dynamic_pdcch_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_dynamic_pdcch_info != PNULL);

    /* This function composes rrc_mac_dynamic_pdcch_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DYNAMIC_PDCCH_INFO;
    tlv_length = rrc_il_get_rrc_mac_dynamic_pdcch_info_tlv_len(p_rrc_mac_dynamic_pdcch_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_dynamic_pdcch_info->isDynamicPdcchDisabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dynamic_pdcch_info->isDynamicPdcchDisabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_dynamic_pdcch_info->isDynamicPdcchDisabled);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_dynamic_pdcch_info->isDynamicPdcchDisabled, "isDynamicPdcchDisabled");
    *pp_buffer += sizeof(p_rrc_mac_dynamic_pdcch_info->isDynamicPdcchDisabled);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_prs_bandwidth_info_len
(
    rrc_mac_prs_bandwidth_info_t *p_rrc_mac_prs_bandwidth_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_prs_bandwidth_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_prs_bandwidth_info->prs_bandwidth);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_prs_bandwidth_info_tlv_len
(
    rrc_mac_prs_bandwidth_info_t *p_rrc_mac_prs_bandwidth_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_prs_bandwidth_info != PNULL);

    length += rrc_il_get_rrc_mac_prs_bandwidth_info_len(p_rrc_mac_prs_bandwidth_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_prs_bandwidth_info
(
    U8  **pp_buffer,
    rrc_mac_prs_bandwidth_info_t *p_rrc_mac_prs_bandwidth_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_prs_bandwidth_info != PNULL);

    /* This function composes rrc_mac_prs_bandwidth_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PRS_BANDWIDTH_INFO;
    tlv_length = rrc_il_get_rrc_mac_prs_bandwidth_info_tlv_len(p_rrc_mac_prs_bandwidth_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_prs_bandwidth_info->prs_bandwidth < 6) || (p_rrc_mac_prs_bandwidth_info->prs_bandwidth > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_prs_bandwidth_info->prs_bandwidth] should be in range "
            "6 to 100. Incorrect value %u received.", p_rrc_mac_prs_bandwidth_info->prs_bandwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_prs_bandwidth_info->prs_bandwidth, "prs_bandwidth");
    *pp_buffer += sizeof(p_rrc_mac_prs_bandwidth_info->prs_bandwidth);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_prs_subframes_info_len
(
    rrc_mac_prs_subframes_info_t *p_rrc_mac_prs_subframes_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_prs_subframes_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_prs_subframes_info->prs_subframes);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_prs_subframes_info_tlv_len
(
    rrc_mac_prs_subframes_info_t *p_rrc_mac_prs_subframes_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_prs_subframes_info != PNULL);

    length += rrc_il_get_rrc_mac_prs_subframes_info_len(p_rrc_mac_prs_subframes_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_prs_subframes_info
(
    U8  **pp_buffer,
    rrc_mac_prs_subframes_info_t *p_rrc_mac_prs_subframes_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_prs_subframes_info != PNULL);

    /* This function composes rrc_mac_prs_subframes_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PRS_SUBFRAMES_INFO;
    tlv_length = rrc_il_get_rrc_mac_prs_subframes_info_tlv_len(p_rrc_mac_prs_subframes_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_prs_subframes_info->prs_subframes < 1) || (p_rrc_mac_prs_subframes_info->prs_subframes > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_prs_subframes_info->prs_subframes] should be in range "
            "1 to 6. Incorrect value %u received.", p_rrc_mac_prs_subframes_info->prs_subframes);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_prs_subframes_info->prs_subframes, "prs_subframes");
    *pp_buffer += sizeof(p_rrc_mac_prs_subframes_info->prs_subframes);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_prs_config_index_info_len
(
    rrc_mac_prs_config_index_info_t *p_rrc_mac_prs_config_index_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_prs_config_index_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_prs_config_index_info->prs_config_index);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_prs_config_index_info_tlv_len
(
    rrc_mac_prs_config_index_info_t *p_rrc_mac_prs_config_index_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_prs_config_index_info != PNULL);

    length += rrc_il_get_rrc_mac_prs_config_index_info_len(p_rrc_mac_prs_config_index_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_prs_config_index_info
(
    U8  **pp_buffer,
    rrc_mac_prs_config_index_info_t *p_rrc_mac_prs_config_index_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_prs_config_index_info != PNULL);

    /* This function composes rrc_mac_prs_config_index_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PRS_CONFIG_INDEX_INFO;
    tlv_length = rrc_il_get_rrc_mac_prs_config_index_info_tlv_len(p_rrc_mac_prs_config_index_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_prs_config_index_info->prs_config_index > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_prs_config_index_info->prs_config_index] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_rrc_mac_prs_config_index_info->prs_config_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_prs_config_index_info->prs_config_index, "prs_config_index");
    *pp_buffer += sizeof(p_rrc_mac_prs_config_index_info->prs_config_index);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_prs_transmission_power_info_len
(
    rrc_mac_prs_transmission_power_info_t *p_rrc_mac_prs_transmission_power_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_prs_transmission_power_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_prs_transmission_power_info->prs_transmission_power);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_prs_transmission_power_info_tlv_len
(
    rrc_mac_prs_transmission_power_info_t *p_rrc_mac_prs_transmission_power_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_prs_transmission_power_info != PNULL);

    length += rrc_il_get_rrc_mac_prs_transmission_power_info_len(p_rrc_mac_prs_transmission_power_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_prs_transmission_power_info
(
    U8  **pp_buffer,
    rrc_mac_prs_transmission_power_info_t *p_rrc_mac_prs_transmission_power_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_prs_transmission_power_info != PNULL);

    /* This function composes rrc_mac_prs_transmission_power_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PRS_TRANSMISSION_POWER_INFO;
    tlv_length = rrc_il_get_rrc_mac_prs_transmission_power_info_tlv_len(p_rrc_mac_prs_transmission_power_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_prs_transmission_power_info->prs_transmission_power > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_prs_transmission_power_info->prs_transmission_power] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_rrc_mac_prs_transmission_power_info->prs_transmission_power);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_prs_transmission_power_info->prs_transmission_power, "prs_transmission_power");
    *pp_buffer += sizeof(p_rrc_mac_prs_transmission_power_info->prs_transmission_power);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_prs_muting_config_info_len
(
    rrc_mac_prs_muting_config_info_t *p_rrc_mac_prs_muting_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_prs_muting_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_prs_muting_config_info->bits_to_be_read);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_prs_muting_config_info->prs_muting_config_index);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_prs_muting_config_info_tlv_len
(
    rrc_mac_prs_muting_config_info_t *p_rrc_mac_prs_muting_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_prs_muting_config_info != PNULL);

    length += rrc_il_get_rrc_mac_prs_muting_config_info_len(p_rrc_mac_prs_muting_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_prs_muting_config_info
(
    U8  **pp_buffer,
    rrc_mac_prs_muting_config_info_t *p_rrc_mac_prs_muting_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_prs_muting_config_info != PNULL);

    /* This function composes rrc_mac_prs_muting_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PRS_MUTING_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_prs_muting_config_info_tlv_len(p_rrc_mac_prs_muting_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_prs_muting_config_info->bits_to_be_read < 2) || (p_rrc_mac_prs_muting_config_info->bits_to_be_read > 16))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_prs_muting_config_info->bits_to_be_read] should be in range "
            "2 to 16. Incorrect value %u received.", p_rrc_mac_prs_muting_config_info->bits_to_be_read);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_prs_muting_config_info->bits_to_be_read, "bits_to_be_read");
    *pp_buffer += sizeof(p_rrc_mac_prs_muting_config_info->bits_to_be_read);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_prs_muting_config_info->prs_muting_config_index, "prs_muting_config_index");
    *pp_buffer += sizeof(p_rrc_mac_prs_muting_config_info->prs_muting_config_index);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_prs_config_info_len
(
    rrc_mac_prs_config_info_t *p_rrc_mac_prs_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_prs_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_prs_config_info->request_type);

    /* Get length of TLV */
    if (p_rrc_mac_prs_config_info->optional_elems_present & RRC_MAC_CONFIG_PRS_BANDWIDTH_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_prs_bandwidth_info_tlv_len(&p_rrc_mac_prs_config_info->prs_bandwidth_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_prs_config_info->optional_elems_present & RRC_MAC_CONFIG_PRS_SUBFRAMES_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_prs_subframes_info_tlv_len(&p_rrc_mac_prs_config_info->prs_subframes_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_prs_config_info->optional_elems_present & RRC_MAC_CONFIG_PRS_CONFIG_INDEX_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_prs_config_index_info_tlv_len(&p_rrc_mac_prs_config_info->prs_config_index_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_prs_config_info->optional_elems_present & RRC_MAC_CONFIG_PRS_TRANSMISSION_POWER_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_prs_transmission_power_info_tlv_len(&p_rrc_mac_prs_config_info->prs_transmission_power_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_prs_config_info->optional_elems_present & RRC_MAC_CONFIG_PRS_MUTING_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_prs_muting_config_info_tlv_len(&p_rrc_mac_prs_config_info->prs_muting_config_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_prs_config_info_tlv_len
(
    rrc_mac_prs_config_info_t *p_rrc_mac_prs_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_prs_config_info != PNULL);

    length += rrc_il_get_rrc_mac_prs_config_info_len(p_rrc_mac_prs_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_prs_config_info
(
    U8  **pp_buffer,
    rrc_mac_prs_config_info_t *p_rrc_mac_prs_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_prs_config_info != PNULL);

    /* This function composes rrc_mac_prs_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PRS_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_prs_config_info_tlv_len(p_rrc_mac_prs_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_prs_config_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_prs_config_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_prs_config_info->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_prs_config_info->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_mac_prs_config_info->request_type);

    /* Compose TLV  */
    if (p_rrc_mac_prs_config_info->optional_elems_present & RRC_MAC_CONFIG_PRS_BANDWIDTH_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_prs_bandwidth_info(pp_buffer, &p_rrc_mac_prs_config_info->prs_bandwidth_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_prs_config_info->optional_elems_present & RRC_MAC_CONFIG_PRS_SUBFRAMES_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_prs_subframes_info(pp_buffer, &p_rrc_mac_prs_config_info->prs_subframes_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_prs_config_info->optional_elems_present & RRC_MAC_CONFIG_PRS_CONFIG_INDEX_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_prs_config_index_info(pp_buffer, &p_rrc_mac_prs_config_info->prs_config_index_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_prs_config_info->optional_elems_present & RRC_MAC_CONFIG_PRS_TRANSMISSION_POWER_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_prs_transmission_power_info(pp_buffer, &p_rrc_mac_prs_config_info->prs_transmission_power_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_prs_config_info->optional_elems_present & RRC_MAC_CONFIG_PRS_MUTING_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_prs_muting_config_info(pp_buffer, &p_rrc_mac_prs_config_info->prs_muting_config_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_m2ap_mac_csa_pattern_list_len
(
    m2ap_mac_csa_pattern_list_t *p_m2ap_mac_csa_pattern_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_m2ap_mac_csa_pattern_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mac_csa_pattern_list->radio_frame_allocation_period);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mac_csa_pattern_list->radio_frame_allocation_offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mac_csa_pattern_list->choice);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mac_csa_pattern_list->subframe_allocation);

    return length;
}

static
rrc_length_t
rrc_il_get_m2ap_mac_csa_pattern_list_tlv_len
(
    m2ap_mac_csa_pattern_list_t *p_m2ap_mac_csa_pattern_list

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_m2ap_mac_csa_pattern_list != PNULL);

    length += rrc_il_get_m2ap_mac_csa_pattern_list_len(p_m2ap_mac_csa_pattern_list);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_m2ap_mac_csa_pattern_list
(
    U8  **pp_buffer,
    m2ap_mac_csa_pattern_list_t *p_m2ap_mac_csa_pattern_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_m2ap_mac_csa_pattern_list != PNULL);

    /* This function composes m2ap_mac_csa_pattern_list */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_M2AP_MAC_CSA_PATTERN_LIST;
    tlv_length = rrc_il_get_m2ap_mac_csa_pattern_list_tlv_len(p_m2ap_mac_csa_pattern_list);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_csa_pattern_list->radio_frame_allocation_period > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_csa_pattern_list->radio_frame_allocation_period] should be less than"
            " or equal to 5. Incorrect value %u received.", p_m2ap_mac_csa_pattern_list->radio_frame_allocation_period);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mac_csa_pattern_list->radio_frame_allocation_period, "radio_frame_allocation_period");
    *pp_buffer += sizeof(p_m2ap_mac_csa_pattern_list->radio_frame_allocation_period);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_csa_pattern_list->radio_frame_allocation_offset > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_csa_pattern_list->radio_frame_allocation_offset] should be less than"
            " or equal to 7. Incorrect value %u received.", p_m2ap_mac_csa_pattern_list->radio_frame_allocation_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mac_csa_pattern_list->radio_frame_allocation_offset, "radio_frame_allocation_offset");
    *pp_buffer += sizeof(p_m2ap_mac_csa_pattern_list->radio_frame_allocation_offset);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_csa_pattern_list->choice > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_csa_pattern_list->choice] should be less than"
            " or equal to 1. Incorrect value %u received.", p_m2ap_mac_csa_pattern_list->choice);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mac_csa_pattern_list->choice, "choice");
    *pp_buffer += sizeof(p_m2ap_mac_csa_pattern_list->choice);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_m2ap_mac_csa_pattern_list->subframe_allocation, "subframe_allocation");
    *pp_buffer += sizeof(p_m2ap_mac_csa_pattern_list->subframe_allocation);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_m2ap_embms_sib2_info_len
(
    rrc_m2ap_embms_sib2_info_t *p_rrc_m2ap_embms_sib2_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_m2ap_embms_sib2_info != PNULL);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_m2ap_embms_sib2_info->no_of_csa_pattern_counter <= ARRSIZE(p_rrc_m2ap_embms_sib2_info->csa_pattern_list));
        for (loop = 0; loop < p_rrc_m2ap_embms_sib2_info->no_of_csa_pattern_counter; loop++)
        {
            length += rrc_il_get_m2ap_mac_csa_pattern_list_tlv_len(&p_rrc_m2ap_embms_sib2_info->csa_pattern_list[loop]);
        }
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_m2ap_embms_sib2_info_tlv_len
(
    rrc_m2ap_embms_sib2_info_t *p_rrc_m2ap_embms_sib2_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_m2ap_embms_sib2_info != PNULL);

    length += rrc_il_get_rrc_m2ap_embms_sib2_info_len(p_rrc_m2ap_embms_sib2_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_m2ap_embms_sib2_info
(
    U8  **pp_buffer,
    rrc_m2ap_embms_sib2_info_t *p_rrc_m2ap_embms_sib2_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_m2ap_embms_sib2_info != PNULL);

    /* This function composes rrc_m2ap_embms_sib2_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_M2AP_MAC_EMBMS_SIB2_INFO;
    tlv_length = rrc_il_get_rrc_m2ap_embms_sib2_info_tlv_len(p_rrc_m2ap_embms_sib2_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_m2ap_embms_sib2_info->no_of_csa_pattern_counter <= ARRSIZE(p_rrc_m2ap_embms_sib2_info->csa_pattern_list));
        for (loop = 0; loop < p_rrc_m2ap_embms_sib2_info->no_of_csa_pattern_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_m2ap_mac_csa_pattern_list(pp_buffer, &p_rrc_m2ap_embms_sib2_info->csa_pattern_list[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_embms_sib13_info_len
(
    rrc_m2ap_mac_embms_sib13_info_t *p_rrc_m2ap_mac_embms_sib13_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_m2ap_mac_embms_sib13_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_m2ap_mac_embms_sib13_info->si_index);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_embms_sib13_info_tlv_len
(
    rrc_m2ap_mac_embms_sib13_info_t *p_rrc_m2ap_mac_embms_sib13_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_m2ap_mac_embms_sib13_info != PNULL);

    length += rrc_il_get_rrc_m2ap_mac_embms_sib13_info_len(p_rrc_m2ap_mac_embms_sib13_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_m2ap_mac_embms_sib13_info
(
    U8  **pp_buffer,
    rrc_m2ap_mac_embms_sib13_info_t *p_rrc_m2ap_mac_embms_sib13_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_m2ap_mac_embms_sib13_info != PNULL);

    /* This function composes rrc_m2ap_mac_embms_sib13_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_M2AP_MAC_SIB13_INFO;
    tlv_length = rrc_il_get_rrc_m2ap_mac_embms_sib13_info_tlv_len(p_rrc_m2ap_mac_embms_sib13_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_m2ap_mac_embms_sib13_info->si_index < 1) || (p_rrc_m2ap_mac_embms_sib13_info->si_index > 74))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_m2ap_mac_embms_sib13_info->si_index] should be in range "
            "1 to 74. Incorrect value %u received.", p_rrc_m2ap_mac_embms_sib13_info->si_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_m2ap_mac_embms_sib13_info->si_index, "si_index");
    *pp_buffer += sizeof(p_rrc_m2ap_mac_embms_sib13_info->si_index);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_m2ap_embms_notification_config_len
(
    m2ap_embms_notification_config_t *p_m2ap_embms_notification_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_m2ap_embms_notification_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_embms_notification_config->notification_rept_coeff);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_embms_notification_config->notification_offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_embms_notification_config->notification_sf_index);

    return length;
}

static
rrc_length_t
rrc_il_get_m2ap_embms_notification_config_tlv_len
(
    m2ap_embms_notification_config_t *p_m2ap_embms_notification_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_m2ap_embms_notification_config != PNULL);

    length += rrc_il_get_m2ap_embms_notification_config_len(p_m2ap_embms_notification_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_m2ap_embms_notification_config
(
    U8  **pp_buffer,
    m2ap_embms_notification_config_t *p_m2ap_embms_notification_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_m2ap_embms_notification_config != PNULL);

    /* This function composes m2ap_embms_notification_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_M2AP_MAC_NOTIFICATION_CONFIG;
    tlv_length = rrc_il_get_m2ap_embms_notification_config_tlv_len(p_m2ap_embms_notification_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_m2ap_embms_notification_config->notification_rept_coeff < 2) || (p_m2ap_embms_notification_config->notification_rept_coeff > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_embms_notification_config->notification_rept_coeff] should be in range "
            "2 to 4. Incorrect value %u received.", p_m2ap_embms_notification_config->notification_rept_coeff);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_embms_notification_config->notification_rept_coeff, "notification_rept_coeff");
    *pp_buffer += sizeof(p_m2ap_embms_notification_config->notification_rept_coeff);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_embms_notification_config->notification_offset > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_embms_notification_config->notification_offset] should be less than"
            " or equal to 10. Incorrect value %u received.", p_m2ap_embms_notification_config->notification_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_embms_notification_config->notification_offset, "notification_offset");
    *pp_buffer += sizeof(p_m2ap_embms_notification_config->notification_offset);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_embms_notification_config->notification_sf_index > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_embms_notification_config->notification_sf_index] should be less than"
            " or equal to 6. Incorrect value %u received.", p_m2ap_embms_notification_config->notification_sf_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_embms_notification_config->notification_sf_index, "notification_sf_index");
    *pp_buffer += sizeof(p_m2ap_embms_notification_config->notification_sf_index);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_m2ap_mac_mcch_config_len
(
    m2ap_mac_mcch_config_t *p_m2ap_mac_mcch_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_m2ap_mac_mcch_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mac_mcch_config->mcch_rep_period);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mac_mcch_config->mcch_offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mac_mcch_config->mcch_mod_period);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mac_mcch_config->sf_alloc_info);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mac_mcch_config->signalling_mcs);

    return length;
}

static
rrc_length_t
rrc_il_get_m2ap_mac_mcch_config_tlv_len
(
    m2ap_mac_mcch_config_t *p_m2ap_mac_mcch_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_m2ap_mac_mcch_config != PNULL);

    length += rrc_il_get_m2ap_mac_mcch_config_len(p_m2ap_mac_mcch_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_m2ap_mac_mcch_config
(
    U8  **pp_buffer,
    m2ap_mac_mcch_config_t *p_m2ap_mac_mcch_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_m2ap_mac_mcch_config != PNULL);

    /* This function composes m2ap_mac_mcch_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_M2AP_MAC_MCCH_CONFIG;
    tlv_length = rrc_il_get_m2ap_mac_mcch_config_tlv_len(p_m2ap_mac_mcch_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_mcch_config->mcch_rep_period > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_mcch_config->mcch_rep_period] should be less than"
            " or equal to 3. Incorrect value %u received.", p_m2ap_mac_mcch_config->mcch_rep_period);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mac_mcch_config->mcch_rep_period, "mcch_rep_period");
    *pp_buffer += sizeof(p_m2ap_mac_mcch_config->mcch_rep_period);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_mcch_config->mcch_offset > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_mcch_config->mcch_offset] should be less than"
            " or equal to 10. Incorrect value %u received.", p_m2ap_mac_mcch_config->mcch_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mac_mcch_config->mcch_offset, "mcch_offset");
    *pp_buffer += sizeof(p_m2ap_mac_mcch_config->mcch_offset);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_mcch_config->mcch_mod_period > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_mcch_config->mcch_mod_period] should be less than"
            " or equal to 1. Incorrect value %u received.", p_m2ap_mac_mcch_config->mcch_mod_period);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mac_mcch_config->mcch_mod_period, "mcch_mod_period");
    *pp_buffer += sizeof(p_m2ap_mac_mcch_config->mcch_mod_period);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mac_mcch_config->sf_alloc_info, "sf_alloc_info");
    *pp_buffer += sizeof(p_m2ap_mac_mcch_config->sf_alloc_info);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_mcch_config->signalling_mcs > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_mcch_config->signalling_mcs] should be less than"
            " or equal to 3. Incorrect value %u received.", p_m2ap_mac_mcch_config->signalling_mcs);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mac_mcch_config->signalling_mcs, "signalling_mcs");
    *pp_buffer += sizeof(p_m2ap_mac_mcch_config->signalling_mcs);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_m2ap_mbms_create_area_info_len
(
    m2ap_mbms_create_area_info_t *p_m2ap_mbms_create_area_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_m2ap_mbms_create_area_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mbms_create_area_info->area_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mbms_create_area_info->area_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mbms_create_area_info->area_type);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mbms_create_area_info->non_mbsfn_region_len);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mbms_create_area_info->request_type);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mbms_create_area_info->notification_ind);

    /* Get length of TLV */
    if (p_m2ap_mbms_create_area_info->optional_elems_present & RRC_M2AP_MAC_MCCH_CONFIG_PRESENT)
    {
        length += rrc_il_get_m2ap_mac_mcch_config_tlv_len(&p_m2ap_mbms_create_area_info->mcch_config);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_m2ap_mbms_create_area_info_tlv_len
(
    m2ap_mbms_create_area_info_t *p_m2ap_mbms_create_area_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_m2ap_mbms_create_area_info != PNULL);

    length += rrc_il_get_m2ap_mbms_create_area_info_len(p_m2ap_mbms_create_area_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_m2ap_mbms_create_area_info
(
    U8  **pp_buffer,
    m2ap_mbms_create_area_info_t *p_m2ap_mbms_create_area_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_m2ap_mbms_create_area_info != PNULL);

    /* This function composes m2ap_mbms_create_area_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_M2AP_MBMS_CREATE_AREA_INFO;
    tlv_length = rrc_il_get_m2ap_mbms_create_area_info_tlv_len(p_m2ap_mbms_create_area_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mbms_create_area_info->area_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mbms_create_area_info->area_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_m2ap_mbms_create_area_info->area_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mbms_create_area_info->area_index, "area_index");
    *pp_buffer += sizeof(p_m2ap_mbms_create_area_info->area_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mbms_create_area_info->area_id, "area_id");
    *pp_buffer += sizeof(p_m2ap_mbms_create_area_info->area_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_m2ap_mbms_create_area_info->area_type < 1) || (p_m2ap_mbms_create_area_info->area_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mbms_create_area_info->area_type] should be in range "
            "1 to 2. Incorrect value %u received.", p_m2ap_mbms_create_area_info->area_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mbms_create_area_info->area_type, "area_type");
    *pp_buffer += sizeof(p_m2ap_mbms_create_area_info->area_type);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_m2ap_mbms_create_area_info->non_mbsfn_region_len < 1) || (p_m2ap_mbms_create_area_info->non_mbsfn_region_len > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mbms_create_area_info->non_mbsfn_region_len] should be in range "
            "1 to 2. Incorrect value %u received.", p_m2ap_mbms_create_area_info->non_mbsfn_region_len);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mbms_create_area_info->non_mbsfn_region_len, "non_mbsfn_region_len");
    *pp_buffer += sizeof(p_m2ap_mbms_create_area_info->non_mbsfn_region_len);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mbms_create_area_info->request_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mbms_create_area_info->request_type] should be less than"
            " or equal to 2. Incorrect value %u received.", p_m2ap_mbms_create_area_info->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mbms_create_area_info->request_type, "request_type");
    *pp_buffer += sizeof(p_m2ap_mbms_create_area_info->request_type);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mbms_create_area_info->notification_ind > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mbms_create_area_info->notification_ind] should be less than"
            " or equal to 7. Incorrect value %u received.", p_m2ap_mbms_create_area_info->notification_ind);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_m2ap_mbms_create_area_info->notification_ind, "notification_ind");
    *pp_buffer += sizeof(p_m2ap_mbms_create_area_info->notification_ind);

    /* Compose TLV  */
    if (p_m2ap_mbms_create_area_info->optional_elems_present & RRC_M2AP_MAC_MCCH_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_m2ap_mac_mcch_config(pp_buffer, &p_m2ap_mbms_create_area_info->mcch_config))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_create_embms_area_entity_info_len
(
    rrc_m2ap_mac_create_embms_area_entity_info_t *p_rrc_m2ap_mac_create_embms_area_entity_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_m2ap_mac_create_embms_area_entity_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_m2ap_mac_create_embms_area_entity_info->num_of_area);

    /* Get length of TLV */
    if (p_rrc_m2ap_mac_create_embms_area_entity_info->optional_elems_present & RRC_M2AP_MAC_NOTIFICATION_CONFIG_PRESENT)
    {
        length += rrc_il_get_m2ap_embms_notification_config_tlv_len(&p_rrc_m2ap_mac_create_embms_area_entity_info->m2ap_embms_notification_config);
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_m2ap_mac_create_embms_area_entity_info->num_create_area_info_counter <= ARRSIZE(p_rrc_m2ap_mac_create_embms_area_entity_info->m2ap_mbms_create_area_info));
        for (loop = 0; loop < p_rrc_m2ap_mac_create_embms_area_entity_info->num_create_area_info_counter; loop++)
        {
            length += rrc_il_get_m2ap_mbms_create_area_info_tlv_len(&p_rrc_m2ap_mac_create_embms_area_entity_info->m2ap_mbms_create_area_info[loop]);
        }
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_create_embms_area_entity_info_tlv_len
(
    rrc_m2ap_mac_create_embms_area_entity_info_t *p_rrc_m2ap_mac_create_embms_area_entity_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_m2ap_mac_create_embms_area_entity_info != PNULL);

    length += rrc_il_get_rrc_m2ap_mac_create_embms_area_entity_info_len(p_rrc_m2ap_mac_create_embms_area_entity_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_m2ap_mac_create_embms_area_entity_info
(
    U8  **pp_buffer,
    rrc_m2ap_mac_create_embms_area_entity_info_t *p_rrc_m2ap_mac_create_embms_area_entity_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_m2ap_mac_create_embms_area_entity_info != PNULL);

    /* This function composes rrc_m2ap_mac_create_embms_area_entity_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_M2AP_MAC_CREATE_MBMS_AREA_ENTITY_INFO;
    tlv_length = rrc_il_get_rrc_m2ap_mac_create_embms_area_entity_info_tlv_len(p_rrc_m2ap_mac_create_embms_area_entity_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_m2ap_mac_create_embms_area_entity_info->num_of_area > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_m2ap_mac_create_embms_area_entity_info->num_of_area] should be less than"
            " or equal to 8. Incorrect value %u received.", p_rrc_m2ap_mac_create_embms_area_entity_info->num_of_area);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_m2ap_mac_create_embms_area_entity_info->num_of_area, "num_of_area");
    *pp_buffer += sizeof(p_rrc_m2ap_mac_create_embms_area_entity_info->num_of_area);

    /* Compose TLV  */
    if (p_rrc_m2ap_mac_create_embms_area_entity_info->optional_elems_present & RRC_M2AP_MAC_NOTIFICATION_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_m2ap_embms_notification_config(pp_buffer, &p_rrc_m2ap_mac_create_embms_area_entity_info->m2ap_embms_notification_config))
        {
            return RRC_FAILURE;
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_m2ap_mac_create_embms_area_entity_info->num_create_area_info_counter <= ARRSIZE(p_rrc_m2ap_mac_create_embms_area_entity_info->m2ap_mbms_create_area_info));
        for (loop = 0; loop < p_rrc_m2ap_mac_create_embms_area_entity_info->num_create_area_info_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_m2ap_mbms_create_area_info(pp_buffer, &p_rrc_m2ap_mac_create_embms_area_entity_info->m2ap_mbms_create_area_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_delete_embms_area_entity_info_len
(
    rrc_m2ap_mac_delete_embms_area_entity_info_t *p_rrc_m2ap_mac_delete_embms_area_entity_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_m2ap_mac_delete_embms_area_entity_info != PNULL);

    /* Add buffer size */
    length += p_rrc_m2ap_mac_delete_embms_area_entity_info->num_area_delete_count;

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_delete_embms_area_entity_info_tlv_len
(
    rrc_m2ap_mac_delete_embms_area_entity_info_t *p_rrc_m2ap_mac_delete_embms_area_entity_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_m2ap_mac_delete_embms_area_entity_info != PNULL);

    length += rrc_il_get_rrc_m2ap_mac_delete_embms_area_entity_info_len(p_rrc_m2ap_mac_delete_embms_area_entity_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_m2ap_mac_delete_embms_area_entity_info
(
    U8  **pp_buffer,
    rrc_m2ap_mac_delete_embms_area_entity_info_t *p_rrc_m2ap_mac_delete_embms_area_entity_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_m2ap_mac_delete_embms_area_entity_info != PNULL);

    /* This function composes rrc_m2ap_mac_delete_embms_area_entity_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_M2AP_MAC_DELETE_MBMS_AREA_ENTITY_INFO;
    tlv_length = rrc_il_get_rrc_m2ap_mac_delete_embms_area_entity_info_tlv_len(p_rrc_m2ap_mac_delete_embms_area_entity_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        if(ARRSIZE(p_rrc_m2ap_mac_delete_embms_area_entity_info->area_index) < p_rrc_m2ap_mac_delete_embms_area_entity_info->num_area_delete_count)
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_m2ap_mac_delete_embms_area_entity_info->num_area_delete_count = %u is exceeding p_rrc_m2ap_mac_delete_embms_area_entity_info->area_index array boundary", p_rrc_m2ap_mac_delete_embms_area_entity_info->num_area_delete_count);
            return RRC_FAILURE;
        }
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_m2ap_mac_delete_embms_area_entity_info->num_area_delete_count; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_m2ap_mac_delete_embms_area_entity_info->area_index[loop], "area_index[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_m2ap_mac_embms_transmission_power_info_len
(
    m2ap_mac_embms_transmission_power_info_t *p_m2ap_mac_embms_transmission_power_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_m2ap_mac_embms_transmission_power_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_m2ap_mac_embms_transmission_power_info->embms_transmission_power);

    return length;
}

static
rrc_length_t
rrc_il_get_m2ap_mac_embms_transmission_power_info_tlv_len
(
    m2ap_mac_embms_transmission_power_info_t *p_m2ap_mac_embms_transmission_power_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_m2ap_mac_embms_transmission_power_info != PNULL);

    length += rrc_il_get_m2ap_mac_embms_transmission_power_info_len(p_m2ap_mac_embms_transmission_power_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_m2ap_mac_embms_transmission_power_info
(
    U8  **pp_buffer,
    m2ap_mac_embms_transmission_power_info_t *p_m2ap_mac_embms_transmission_power_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_m2ap_mac_embms_transmission_power_info != PNULL);

    /* This function composes m2ap_mac_embms_transmission_power_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_M2AP_MAC_EMBMS_TRANSMISSION_POWER_INFO;
    tlv_length = rrc_il_get_m2ap_mac_embms_transmission_power_info_tlv_len(p_m2ap_mac_embms_transmission_power_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_m2ap_mac_embms_transmission_power_info->embms_transmission_power > 10000))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_m2ap_mac_embms_transmission_power_info->embms_transmission_power] should be less than"
            " or equal to 10000. Incorrect value %u received.", p_m2ap_mac_embms_transmission_power_info->embms_transmission_power);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_m2ap_mac_embms_transmission_power_info->embms_transmission_power, "embms_transmission_power");
    *pp_buffer += sizeof(p_m2ap_mac_embms_transmission_power_info->embms_transmission_power);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_embms_params_info_len
(
    rrc_m2ap_mac_embms_params_info_t *p_rrc_m2ap_mac_embms_params_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_m2ap_mac_embms_params_info != PNULL);

    /* Get length of TLV */
    if (p_rrc_m2ap_mac_embms_params_info->optional_elems_present & RRC_M2AP_MBMS_TRANSMISSION_POWER_INFO_PRESENT)
    {
        length += rrc_il_get_m2ap_mac_embms_transmission_power_info_tlv_len(&p_rrc_m2ap_mac_embms_params_info->m2ap_mac_embms_transmission_power_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_m2ap_mac_embms_params_info_tlv_len
(
    rrc_m2ap_mac_embms_params_info_t *p_rrc_m2ap_mac_embms_params_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_m2ap_mac_embms_params_info != PNULL);

    length += rrc_il_get_rrc_m2ap_mac_embms_params_info_len(p_rrc_m2ap_mac_embms_params_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_m2ap_mac_embms_params_info
(
    U8  **pp_buffer,
    rrc_m2ap_mac_embms_params_info_t *p_rrc_m2ap_mac_embms_params_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_m2ap_mac_embms_params_info != PNULL);

    /* This function composes rrc_m2ap_mac_embms_params_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_M2AP_MAC_EMBMS_CONFIG;
    tlv_length = rrc_il_get_rrc_m2ap_mac_embms_params_info_tlv_len(p_rrc_m2ap_mac_embms_params_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose TLV  */
    if (p_rrc_m2ap_mac_embms_params_info->optional_elems_present & RRC_M2AP_MBMS_TRANSMISSION_POWER_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_m2ap_mac_embms_transmission_power_info(pp_buffer, &p_rrc_m2ap_mac_embms_params_info->m2ap_mac_embms_transmission_power_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_radio_res_dedicated_laa_scell_config_len
(
    rrc_radio_res_dedicated_laa_scell_config_t *p_rrc_radio_res_dedicated_laa_scell_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_radio_res_dedicated_laa_scell_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_radio_res_dedicated_laa_scell_config->subframe_start_position);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_radio_res_dedicated_laa_scell_config->laa_scell_subframe_config);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_radio_res_dedicated_laa_scell_config_tlv_len
(
    rrc_radio_res_dedicated_laa_scell_config_t *p_rrc_radio_res_dedicated_laa_scell_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_radio_res_dedicated_laa_scell_config != PNULL);

    length += rrc_il_get_rrc_radio_res_dedicated_laa_scell_config_len(p_rrc_radio_res_dedicated_laa_scell_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_radio_res_dedicated_laa_scell_config
(
    U8  **pp_buffer,
    rrc_radio_res_dedicated_laa_scell_config_t *p_rrc_radio_res_dedicated_laa_scell_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_radio_res_dedicated_laa_scell_config != PNULL);

    /* This function composes rrc_radio_res_dedicated_laa_scell_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_LAA_SCELL_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_radio_res_dedicated_laa_scell_config_tlv_len(p_rrc_radio_res_dedicated_laa_scell_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_radio_res_dedicated_laa_scell_config->subframe_start_position > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_radio_res_dedicated_laa_scell_config->subframe_start_position] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_radio_res_dedicated_laa_scell_config->subframe_start_position);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_radio_res_dedicated_laa_scell_config->subframe_start_position, "subframe_start_position");
    *pp_buffer += sizeof(p_rrc_radio_res_dedicated_laa_scell_config->subframe_start_position);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_radio_res_dedicated_laa_scell_config->laa_scell_subframe_config > 255))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_radio_res_dedicated_laa_scell_config->laa_scell_subframe_config] should be less than"
            " or equal to 255. Incorrect value %u received.", p_rrc_radio_res_dedicated_laa_scell_config->laa_scell_subframe_config);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_radio_res_dedicated_laa_scell_config->laa_scell_subframe_config, "laa_scell_subframe_config");
    *pp_buffer += sizeof(p_rrc_radio_res_dedicated_laa_scell_config->laa_scell_subframe_config);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_dmtc_configuration_r13_len
(
    rrc_dmtc_configuration_r13_t *p_rrc_dmtc_configuration_r13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_dmtc_configuration_r13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_dmtc_configuration_r13->dmtc_periodicity);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_dmtc_configuration_r13->dmtc_offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_dmtc_configuration_r13->ds_occassion_duration);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_dmtc_configuration_r13_tlv_len
(
    rrc_dmtc_configuration_r13_t *p_rrc_dmtc_configuration_r13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_dmtc_configuration_r13 != PNULL);

    length += rrc_il_get_rrc_dmtc_configuration_r13_len(p_rrc_dmtc_configuration_r13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_dmtc_configuration_r13
(
    U8  **pp_buffer,
    rrc_dmtc_configuration_r13_t *p_rrc_dmtc_configuration_r13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_dmtc_configuration_r13 != PNULL);

    /* This function composes rrc_dmtc_configuration_r13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DMTC_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_dmtc_configuration_r13_tlv_len(p_rrc_dmtc_configuration_r13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_dmtc_configuration_r13->dmtc_periodicity > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_dmtc_configuration_r13->dmtc_periodicity] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_dmtc_configuration_r13->dmtc_periodicity);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_dmtc_configuration_r13->dmtc_periodicity, "dmtc_periodicity");
    *pp_buffer += sizeof(p_rrc_dmtc_configuration_r13->dmtc_periodicity);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_dmtc_configuration_r13->dmtc_offset > 159))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_dmtc_configuration_r13->dmtc_offset] should be less than"
            " or equal to 159. Incorrect value %u received.", p_rrc_dmtc_configuration_r13->dmtc_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_dmtc_configuration_r13->dmtc_offset, "dmtc_offset");
    *pp_buffer += sizeof(p_rrc_dmtc_configuration_r13->dmtc_offset);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_dmtc_configuration_r13->ds_occassion_duration > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_dmtc_configuration_r13->ds_occassion_duration] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_dmtc_configuration_r13->ds_occassion_duration);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_dmtc_configuration_r13->ds_occassion_duration, "ds_occassion_duration");
    *pp_buffer += sizeof(p_rrc_dmtc_configuration_r13->ds_occassion_duration);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcEmtcDataConfig_len
(
    RrcEmtcDataConfig_t *p_RrcEmtcDataConfig
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcEmtcDataConfig != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcEmtcDataConfig->sib1BrIndex);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcEmtcDataConfig->numUeDedicatedNarrowbands);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_RrcEmtcDataConfig->numUeDedicatedNarrowbands * sizeof(p_RrcEmtcDataConfig->ueDedicatedNarrowbands[0]));

    return length;
}

static
rrc_length_t
rrc_il_get_RrcEmtcDataConfig_tlv_len
(
    RrcEmtcDataConfig_t *p_RrcEmtcDataConfig

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcEmtcDataConfig != PNULL);

    length += rrc_il_get_RrcEmtcDataConfig_len(p_RrcEmtcDataConfig);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcEmtcDataConfig
(
    U8  **pp_buffer,
    RrcEmtcDataConfig_t *p_RrcEmtcDataConfig
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcEmtcDataConfig != PNULL);

    /* This function composes RrcEmtcDataConfig */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_EMTC_DATA_CONFIG;
    tlv_length = rrc_il_get_RrcEmtcDataConfig_tlv_len(p_RrcEmtcDataConfig);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcEmtcDataConfig->sib1BrIndex > EMTC_MAX_SIB1_BR_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcEmtcDataConfig->sib1BrIndex] should be less than"
            " or equal to EMTC_MAX_SIB1_BR_INDEX. Incorrect value %u received.", p_RrcEmtcDataConfig->sib1BrIndex);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcEmtcDataConfig->sib1BrIndex, "sib1BrIndex");
    *pp_buffer += sizeof(p_RrcEmtcDataConfig->sib1BrIndex);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcEmtcDataConfig->numUeDedicatedNarrowbands < 1) || (p_RrcEmtcDataConfig->numUeDedicatedNarrowbands > EMTC_MAX_AVAILABLE_NARROWBAND))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcEmtcDataConfig->numUeDedicatedNarrowbands] should be in range "
            "1 to EMTC_MAX_AVAILABLE_NARROWBAND. Incorrect value %u received.", p_RrcEmtcDataConfig->numUeDedicatedNarrowbands);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcEmtcDataConfig->numUeDedicatedNarrowbands, "numUeDedicatedNarrowbands");
    *pp_buffer += sizeof(p_RrcEmtcDataConfig->numUeDedicatedNarrowbands);

    /* Compose OCTET_STRING VARIABLE of basic type elements with BOTH boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcEmtcDataConfig->numUeDedicatedNarrowbands; loop++)
        {
            if ((p_RrcEmtcDataConfig->ueDedicatedNarrowbands[loop] < 1) || (p_RrcEmtcDataConfig->ueDedicatedNarrowbands[loop] > EMTC_MAX_AVAILABLE_NARROWBAND))
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_RrcEmtcDataConfig->ueDedicatedNarrowbands[%u] should be in "
                    "range 1 to EMTC_MAX_AVAILABLE_NARROWBAND. "
                    "Incorrect value %d received.", loop, p_RrcEmtcDataConfig->ueDedicatedNarrowbands[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U8(*pp_buffer, &p_RrcEmtcDataConfig->ueDedicatedNarrowbands[loop], "ueDedicatedNarrowbands[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcPrachStartSfR13_len
(
    RrcPrachStartSfR13_t *p_RrcPrachStartSfR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPrachStartSfR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPrachStartSfR13->prachStartingSubframeR13);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcPrachStartSfR13_tlv_len
(
    RrcPrachStartSfR13_t *p_RrcPrachStartSfR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPrachStartSfR13 != PNULL);

    length += rrc_il_get_RrcPrachStartSfR13_len(p_RrcPrachStartSfR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcPrachStartSfR13
(
    U8  **pp_buffer,
    RrcPrachStartSfR13_t *p_RrcPrachStartSfR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcPrachStartSfR13 != PNULL);

    /* This function composes RrcPrachStartSfR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PRACH_STARTING_SUBFRAME_R13;
    tlv_length = rrc_il_get_RrcPrachStartSfR13_tlv_len(p_RrcPrachStartSfR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachStartSfR13->prachStartingSubframeR13 > EMTC_PRACH_STARTING_SF_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachStartSfR13->prachStartingSubframeR13] should be less than"
            " or equal to EMTC_PRACH_STARTING_SF_MAX. Incorrect value %u received.", p_RrcPrachStartSfR13->prachStartingSubframeR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPrachStartSfR13->prachStartingSubframeR13, "prachStartingSubframeR13");
    *pp_buffer += sizeof(p_RrcPrachStartSfR13->prachStartingSubframeR13);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMaxNumPreambleAttemptCeR13_len
(
    RrcMaxNumPreambleAttemptCeR13_t *p_RrcMaxNumPreambleAttemptCeR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMaxNumPreambleAttemptCeR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMaxNumPreambleAttemptCeR13->maxNumPreambleAttemptCeR13);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMaxNumPreambleAttemptCeR13_tlv_len
(
    RrcMaxNumPreambleAttemptCeR13_t *p_RrcMaxNumPreambleAttemptCeR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMaxNumPreambleAttemptCeR13 != PNULL);

    length += rrc_il_get_RrcMaxNumPreambleAttemptCeR13_len(p_RrcMaxNumPreambleAttemptCeR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMaxNumPreambleAttemptCeR13
(
    U8  **pp_buffer,
    RrcMaxNumPreambleAttemptCeR13_t *p_RrcMaxNumPreambleAttemptCeR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMaxNumPreambleAttemptCeR13 != PNULL);

    /* This function composes RrcMaxNumPreambleAttemptCeR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MAX_NUM_PREAMBLE_ATTEMPT_CE_R13;
    tlv_length = rrc_il_get_RrcMaxNumPreambleAttemptCeR13_tlv_len(p_RrcMaxNumPreambleAttemptCeR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMaxNumPreambleAttemptCeR13->maxNumPreambleAttemptCeR13 > EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMaxNumPreambleAttemptCeR13->maxNumPreambleAttemptCeR13] should be less than"
            " or equal to EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_MAX. Incorrect value %u received.", p_RrcMaxNumPreambleAttemptCeR13->maxNumPreambleAttemptCeR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMaxNumPreambleAttemptCeR13->maxNumPreambleAttemptCeR13, "maxNumPreambleAttemptCeR13");
    *pp_buffer += sizeof(p_RrcMaxNumPreambleAttemptCeR13->maxNumPreambleAttemptCeR13);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcPrachParametersCeR13_len
(
    RrcPrachParametersCeR13_t *p_RrcPrachParametersCeR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPrachParametersCeR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPrachParametersCeR13->ceLevel);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPrachParametersCeR13->prachConfigIndexR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPrachParametersCeR13->prachFreqOffsetR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPrachParametersCeR13->numRepetitionPerPreambleAttemptR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13 * sizeof(p_RrcPrachParametersCeR13->mpdcchNarrowBandIndex[0]));
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPrachParametersCeR13->mpdcchNumRepetitionsRaR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPrachParametersCeR13->prachHoppingConfigR13);

    /* Get length of TLV */
    if (p_RrcPrachParametersCeR13->bitmask & RRC_MAC_PRACH_START_SF_R13_PRESENT)
    {
        length += rrc_il_get_RrcPrachStartSfR13_tlv_len(&p_RrcPrachParametersCeR13->prachStartingSubframeR13);
    }

    /* Get length of TLV */
    if (p_RrcPrachParametersCeR13->bitmask & RRC_MAC_MAX_NUM_PREAMBLE_ATTEMPT_CE_R13_PRESENT)
    {
        length += rrc_il_get_RrcMaxNumPreambleAttemptCeR13_tlv_len(&p_RrcPrachParametersCeR13->maxNumPreambleAttemptCeR13);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_RrcPrachParametersCeR13_tlv_len
(
    RrcPrachParametersCeR13_t *p_RrcPrachParametersCeR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPrachParametersCeR13 != PNULL);

    length += rrc_il_get_RrcPrachParametersCeR13_len(p_RrcPrachParametersCeR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcPrachParametersCeR13
(
    U8  **pp_buffer,
    RrcPrachParametersCeR13_t *p_RrcPrachParametersCeR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcPrachParametersCeR13 != PNULL);

    /* This function composes RrcPrachParametersCeR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PRACH_PARAMETERS_CE_R13_LIST;
    tlv_length = rrc_il_get_RrcPrachParametersCeR13_tlv_len(p_RrcPrachParametersCeR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachParametersCeR13->ceLevel > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->ceLevel] should be less than"
            " or equal to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcPrachParametersCeR13->ceLevel);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPrachParametersCeR13->ceLevel, "ceLevel");
    *pp_buffer += sizeof(p_RrcPrachParametersCeR13->ceLevel);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachParametersCeR13->prachConfigIndexR13 > EMTC_MAX_PRACH_CONFIG_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->prachConfigIndexR13] should be less than"
            " or equal to EMTC_MAX_PRACH_CONFIG_INDEX. Incorrect value %u received.", p_RrcPrachParametersCeR13->prachConfigIndexR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPrachParametersCeR13->prachConfigIndexR13, "prachConfigIndexR13");
    *pp_buffer += sizeof(p_RrcPrachParametersCeR13->prachConfigIndexR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachParametersCeR13->prachFreqOffsetR13 > EMTC_MAX_PRACH_FREQ_OFFSET))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->prachFreqOffsetR13] should be less than"
            " or equal to EMTC_MAX_PRACH_FREQ_OFFSET. Incorrect value %u received.", p_RrcPrachParametersCeR13->prachFreqOffsetR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPrachParametersCeR13->prachFreqOffsetR13, "prachFreqOffsetR13");
    *pp_buffer += sizeof(p_RrcPrachParametersCeR13->prachFreqOffsetR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachParametersCeR13->numRepetitionPerPreambleAttemptR13 > EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->numRepetitionPerPreambleAttemptR13] should be less than"
            " or equal to EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX. Incorrect value %u received.", p_RrcPrachParametersCeR13->numRepetitionPerPreambleAttemptR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPrachParametersCeR13->numRepetitionPerPreambleAttemptR13, "numRepetitionPerPreambleAttemptR13");
    *pp_buffer += sizeof(p_RrcPrachParametersCeR13->numRepetitionPerPreambleAttemptR13);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13 < 1) || (p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13 > EMTC_MAX_NARROWBAND_TO_MONITOR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13] should be in range "
            "1 to EMTC_MAX_NARROWBAND_TO_MONITOR. Incorrect value %u received.", p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13, "numMpdcchNarrowbandsToMonitorR13");
    *pp_buffer += sizeof(p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13);

    /* Compose OCTET_STRING VARIABLE of basic type elements with HIGHER boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcPrachParametersCeR13->numMpdcchNarrowbandsToMonitorR13; loop++)
        {
            if (p_RrcPrachParametersCeR13->mpdcchNarrowBandIndex[loop] > EMTC_MAX_AVAILABLE_NARROWBAND)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_RrcPrachParametersCeR13->mpdcchNarrowBandIndex[%u] should be less"
                    " than or equal to EMTC_MAX_AVAILABLE_NARROWBAND. "
                    "Incorrect value %d received.", loop, p_RrcPrachParametersCeR13->mpdcchNarrowBandIndex[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U8(*pp_buffer, &p_RrcPrachParametersCeR13->mpdcchNarrowBandIndex[loop], "mpdcchNarrowBandIndex[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachParametersCeR13->mpdcchNumRepetitionsRaR13 > EMTC_MPDCCH_NUM_REPETITION_RA_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->mpdcchNumRepetitionsRaR13] should be less than"
            " or equal to EMTC_MPDCCH_NUM_REPETITION_RA_MAX. Incorrect value %u received.", p_RrcPrachParametersCeR13->mpdcchNumRepetitionsRaR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPrachParametersCeR13->mpdcchNumRepetitionsRaR13, "mpdcchNumRepetitionsRaR13");
    *pp_buffer += sizeof(p_RrcPrachParametersCeR13->mpdcchNumRepetitionsRaR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachParametersCeR13->prachHoppingConfigR13 > EMTC_PRACH_HOPPING_CONFIG_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachParametersCeR13->prachHoppingConfigR13] should be less than"
            " or equal to EMTC_PRACH_HOPPING_CONFIG_MAX. Incorrect value %u received.", p_RrcPrachParametersCeR13->prachHoppingConfigR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPrachParametersCeR13->prachHoppingConfigR13, "prachHoppingConfigR13");
    *pp_buffer += sizeof(p_RrcPrachParametersCeR13->prachHoppingConfigR13);

    /* Compose TLV  */
    if (p_RrcPrachParametersCeR13->bitmask & RRC_MAC_PRACH_START_SF_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcPrachStartSfR13(pp_buffer, &p_RrcPrachParametersCeR13->prachStartingSubframeR13))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_RrcPrachParametersCeR13->bitmask & RRC_MAC_MAX_NUM_PREAMBLE_ATTEMPT_CE_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMaxNumPreambleAttemptCeR13(pp_buffer, &p_RrcPrachParametersCeR13->maxNumPreambleAttemptCeR13))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcPrachHoppingOffsetR13_len
(
    RrcPrachHoppingOffsetR13_t *p_RrcPrachHoppingOffsetR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPrachHoppingOffsetR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPrachHoppingOffsetR13->prachHoppingOffsetR13);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcPrachHoppingOffsetR13_tlv_len
(
    RrcPrachHoppingOffsetR13_t *p_RrcPrachHoppingOffsetR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPrachHoppingOffsetR13 != PNULL);

    length += rrc_il_get_RrcPrachHoppingOffsetR13_len(p_RrcPrachHoppingOffsetR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcPrachHoppingOffsetR13
(
    U8  **pp_buffer,
    RrcPrachHoppingOffsetR13_t *p_RrcPrachHoppingOffsetR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcPrachHoppingOffsetR13 != PNULL);

    /* This function composes RrcPrachHoppingOffsetR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PRACH_HOPPING_OFFSET_R13;
    tlv_length = rrc_il_get_RrcPrachHoppingOffsetR13_tlv_len(p_RrcPrachHoppingOffsetR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPrachHoppingOffsetR13->prachHoppingOffsetR13 > EMTC_MAX_PRACH_HOPPING_OFFSET))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPrachHoppingOffsetR13->prachHoppingOffsetR13] should be less than"
            " or equal to EMTC_MAX_PRACH_HOPPING_OFFSET. Incorrect value %u received.", p_RrcPrachHoppingOffsetR13->prachHoppingOffsetR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPrachHoppingOffsetR13->prachHoppingOffsetR13, "prachHoppingOffsetR13");
    *pp_buffer += sizeof(p_RrcPrachHoppingOffsetR13->prachHoppingOffsetR13);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacPrachConfigV1310_len
(
    RrcMacPrachConfigV1310_t *p_RrcMacPrachConfigV1310
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPrachConfigV1310 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPrachConfigV1310->mpdcchStartSfCssRaType);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPrachConfigV1310->mpdcchStartSfCssRa);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPrachConfigV1310->numCeLevels);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_RrcMacPrachConfigV1310->numCeLevelsCounter <= ARRSIZE(p_RrcMacPrachConfigV1310->prachParametersCeR13));
        for (loop = 0; loop < p_RrcMacPrachConfigV1310->numCeLevelsCounter; loop++)
        {
            length += rrc_il_get_RrcPrachParametersCeR13_tlv_len(&p_RrcMacPrachConfigV1310->prachParametersCeR13[loop]);
        }
    }

    /* Get length of TLV */
    if (p_RrcMacPrachConfigV1310->bitmask & RRC_PRACH_HOPPING_OFFSET_R13_PRESENT)
    {
        length += rrc_il_get_RrcPrachHoppingOffsetR13_tlv_len(&p_RrcMacPrachConfigV1310->prachHoppingOffsetR13);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacPrachConfigV1310_tlv_len
(
    RrcMacPrachConfigV1310_t *p_RrcMacPrachConfigV1310

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPrachConfigV1310 != PNULL);

    length += rrc_il_get_RrcMacPrachConfigV1310_len(p_RrcMacPrachConfigV1310);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacPrachConfigV1310
(
    U8  **pp_buffer,
    RrcMacPrachConfigV1310_t *p_RrcMacPrachConfigV1310
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacPrachConfigV1310 != PNULL);

    /* This function composes RrcMacPrachConfigV1310 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PRACH_CONFIG_V1310;
    tlv_length = rrc_il_get_RrcMacPrachConfigV1310_tlv_len(p_RrcMacPrachConfigV1310);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPrachConfigV1310->mpdcchStartSfCssRaType > RRC_ONE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPrachConfigV1310->mpdcchStartSfCssRaType] should be less than"
            " or equal to RRC_ONE. Incorrect value %u received.", p_RrcMacPrachConfigV1310->mpdcchStartSfCssRaType);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPrachConfigV1310->mpdcchStartSfCssRaType, "mpdcchStartSfCssRaType");
    *pp_buffer += sizeof(p_RrcMacPrachConfigV1310->mpdcchStartSfCssRaType);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPrachConfigV1310->mpdcchStartSfCssRa > EMTC_MPDCCH_START_SF_CSS_RA_FDD_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPrachConfigV1310->mpdcchStartSfCssRa] should be less than"
            " or equal to EMTC_MPDCCH_START_SF_CSS_RA_FDD_MAX. Incorrect value %u received.", p_RrcMacPrachConfigV1310->mpdcchStartSfCssRa);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPrachConfigV1310->mpdcchStartSfCssRa, "mpdcchStartSfCssRa");
    *pp_buffer += sizeof(p_RrcMacPrachConfigV1310->mpdcchStartSfCssRa);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacPrachConfigV1310->numCeLevels < 1) || (p_RrcMacPrachConfigV1310->numCeLevels > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPrachConfigV1310->numCeLevels] should be in range "
            "1 to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcMacPrachConfigV1310->numCeLevels);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPrachConfigV1310->numCeLevels, "numCeLevels");
    *pp_buffer += sizeof(p_RrcMacPrachConfigV1310->numCeLevels);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_RrcMacPrachConfigV1310->numCeLevelsCounter <= ARRSIZE(p_RrcMacPrachConfigV1310->prachParametersCeR13));
        for (loop = 0; loop < p_RrcMacPrachConfigV1310->numCeLevelsCounter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_RrcPrachParametersCeR13(pp_buffer, &p_RrcMacPrachConfigV1310->prachParametersCeR13[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_RrcMacPrachConfigV1310->bitmask & RRC_PRACH_HOPPING_OFFSET_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcPrachHoppingOffsetR13(pp_buffer, &p_RrcMacPrachConfigV1310->prachHoppingOffsetR13))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacSibtype1BrMsgInfo_len
(
    RrcMacSibtype1BrMsgInfo_t *p_RrcMacSibtype1BrMsgInfo
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacSibtype1BrMsgInfo != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacSibtype1BrMsgInfo->sfn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacSibtype1BrMsgInfo->startSymbolBrR13);

    /* Add buffer size */
    length += p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBufCounter;

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacSibtype1BrMsgInfo_tlv_len
(
    RrcMacSibtype1BrMsgInfo_t *p_RrcMacSibtype1BrMsgInfo

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacSibtype1BrMsgInfo != PNULL);

    length += rrc_il_get_RrcMacSibtype1BrMsgInfo_len(p_RrcMacSibtype1BrMsgInfo);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacSibtype1BrMsgInfo
(
    U8  **pp_buffer,
    RrcMacSibtype1BrMsgInfo_t *p_RrcMacSibtype1BrMsgInfo
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacSibtype1BrMsgInfo != PNULL);

    /* This function composes RrcMacSibtype1BrMsgInfo */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SIBTYPE1_BR_MSG_INFO;
    tlv_length = rrc_il_get_RrcMacSibtype1BrMsgInfo_tlv_len(p_RrcMacSibtype1BrMsgInfo);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSibtype1BrMsgInfo->sfn > EMTC_MAX_STARTING_SFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSibtype1BrMsgInfo->sfn] should be less than"
            " or equal to EMTC_MAX_STARTING_SFN. Incorrect value %u received.", p_RrcMacSibtype1BrMsgInfo->sfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_RrcMacSibtype1BrMsgInfo->sfn, "sfn");
    *pp_buffer += sizeof(p_RrcMacSibtype1BrMsgInfo->sfn);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacSibtype1BrMsgInfo->startSymbolBrR13 < 1) || (p_RrcMacSibtype1BrMsgInfo->startSymbolBrR13 > EMTC_MAX_START_SYMBOL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSibtype1BrMsgInfo->startSymbolBrR13] should be in range "
            "1 to EMTC_MAX_START_SYMBOL. Incorrect value %u received.", p_RrcMacSibtype1BrMsgInfo->startSymbolBrR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacSibtype1BrMsgInfo->startSymbolBrR13, "startSymbolBrR13");
    *pp_buffer += sizeof(p_RrcMacSibtype1BrMsgInfo->startSymbolBrR13);

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        if(ARRSIZE(p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBuf) < p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBufCounter)
        {
            RRC_TRACE(RRC_WARNING, "p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBufCounter = %u is exceeding p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBuf array boundary", p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBufCounter);
            return RRC_FAILURE;
        }
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBufCounter; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_RrcMacSibtype1BrMsgInfo->sibtype1BrMsgBuf[loop], "sibtype1BrMsgBuf[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacSiMsgsBr_len
(
    RrcMacSiMsgsBr_t *p_RrcMacSiMsgsBr
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacSiMsgsBr != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacSiMsgsBr->siIndexBr);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacSiMsgsBr->siPeriodicity);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacSiMsgsBr->startingSfn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacSiMsgsBr->siRepetitionPatternR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacSiMsgsBr->siNarrowbandR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacSiMsgsBr->siTbsR13);

    /* Add buffer size */
    length += p_RrcMacSiMsgsBr->siMsgBufLengthBr;

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacSiMsgsBr_tlv_len
(
    RrcMacSiMsgsBr_t *p_RrcMacSiMsgsBr

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacSiMsgsBr != PNULL);

    length += rrc_il_get_RrcMacSiMsgsBr_len(p_RrcMacSiMsgsBr);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacSiMsgsBr
(
    U8  **pp_buffer,
    RrcMacSiMsgsBr_t *p_RrcMacSiMsgsBr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacSiMsgsBr != PNULL);

    /* This function composes RrcMacSiMsgsBr */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SI_BR_MSG_REQ;
    tlv_length = rrc_il_get_RrcMacSiMsgsBr_tlv_len(p_RrcMacSiMsgsBr);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacSiMsgsBr->siIndexBr < 1) || (p_RrcMacSiMsgsBr->siIndexBr > EMTC_MAX_SI_COUNT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgsBr->siIndexBr] should be in range "
            "1 to EMTC_MAX_SI_COUNT. Incorrect value %u received.", p_RrcMacSiMsgsBr->siIndexBr);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacSiMsgsBr->siIndexBr, "siIndexBr");
    *pp_buffer += sizeof(p_RrcMacSiMsgsBr->siIndexBr);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSiMsgsBr->siPeriodicity > EMTC_SI_PERIODICITY_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgsBr->siPeriodicity] should be less than"
            " or equal to EMTC_SI_PERIODICITY_MAX. Incorrect value %u received.", p_RrcMacSiMsgsBr->siPeriodicity);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacSiMsgsBr->siPeriodicity, "siPeriodicity");
    *pp_buffer += sizeof(p_RrcMacSiMsgsBr->siPeriodicity);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSiMsgsBr->startingSfn > EMTC_MAX_STARTING_SFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgsBr->startingSfn] should be less than"
            " or equal to EMTC_MAX_STARTING_SFN. Incorrect value %u received.", p_RrcMacSiMsgsBr->startingSfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_RrcMacSiMsgsBr->startingSfn, "startingSfn");
    *pp_buffer += sizeof(p_RrcMacSiMsgsBr->startingSfn);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSiMsgsBr->siRepetitionPatternR13 > EMTC_SI_REPETITION_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgsBr->siRepetitionPatternR13] should be less than"
            " or equal to EMTC_SI_REPETITION_MAX. Incorrect value %u received.", p_RrcMacSiMsgsBr->siRepetitionPatternR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacSiMsgsBr->siRepetitionPatternR13, "siRepetitionPatternR13");
    *pp_buffer += sizeof(p_RrcMacSiMsgsBr->siRepetitionPatternR13);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacSiMsgsBr->siNarrowbandR13 < 1) || (p_RrcMacSiMsgsBr->siNarrowbandR13 > EMTC_MAX_AVAILABLE_NARROWBAND))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgsBr->siNarrowbandR13] should be in range "
            "1 to EMTC_MAX_AVAILABLE_NARROWBAND. Incorrect value %u received.", p_RrcMacSiMsgsBr->siNarrowbandR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacSiMsgsBr->siNarrowbandR13, "siNarrowbandR13");
    *pp_buffer += sizeof(p_RrcMacSiMsgsBr->siNarrowbandR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSiMsgsBr->siTbsR13 > EMTC_SI_TB_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgsBr->siTbsR13] should be less than"
            " or equal to EMTC_SI_TB_MAX. Incorrect value %u received.", p_RrcMacSiMsgsBr->siTbsR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacSiMsgsBr->siTbsR13, "siTbsR13");
    *pp_buffer += sizeof(p_RrcMacSiMsgsBr->siTbsR13);

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        if(ARRSIZE(p_RrcMacSiMsgsBr->siMsgBufBr) < p_RrcMacSiMsgsBr->siMsgBufLengthBr)
        {
            RRC_TRACE(RRC_WARNING, "p_RrcMacSiMsgsBr->siMsgBufLengthBr = %u is exceeding p_RrcMacSiMsgsBr->siMsgBufBr array boundary", p_RrcMacSiMsgsBr->siMsgBufLengthBr);
            return RRC_FAILURE;
        }
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcMacSiMsgsBr->siMsgBufLengthBr; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_RrcMacSiMsgsBr->siMsgBufBr[loop], "siMsgBufBr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacFddDownlinkOrTddSfBitmapBr_len
(
    RrcMacFddDownlinkOrTddSfBitmapBr_t *p_RrcMacFddDownlinkOrTddSfBitmapBr
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacFddDownlinkOrTddSfBitmapBr != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_RrcMacFddDownlinkOrTddSfBitmapBr->fddDownlinksubframeBitmapBr);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacFddDownlinkOrTddSfBitmapBr_tlv_len
(
    RrcMacFddDownlinkOrTddSfBitmapBr_t *p_RrcMacFddDownlinkOrTddSfBitmapBr

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacFddDownlinkOrTddSfBitmapBr != PNULL);

    length += rrc_il_get_RrcMacFddDownlinkOrTddSfBitmapBr_len(p_RrcMacFddDownlinkOrTddSfBitmapBr);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacFddDownlinkOrTddSfBitmapBr
(
    U8  **pp_buffer,
    RrcMacFddDownlinkOrTddSfBitmapBr_t *p_RrcMacFddDownlinkOrTddSfBitmapBr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacFddDownlinkOrTddSfBitmapBr != PNULL);

    /* This function composes RrcMacFddDownlinkOrTddSfBitmapBr */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_FDD_DOWNLINK_TDD_SF_BITMAP_BR;
    tlv_length = rrc_il_get_RrcMacFddDownlinkOrTddSfBitmapBr_tlv_len(p_RrcMacFddDownlinkOrTddSfBitmapBr);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_RrcMacFddDownlinkOrTddSfBitmapBr->fddDownlinksubframeBitmapBr); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_RrcMacFddDownlinkOrTddSfBitmapBr->fddDownlinksubframeBitmapBr[loop], "fddDownlinksubframeBitmapBr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacFddUplinkSfBitmapBr_len
(
    RrcMacFddUplinkSfBitmapBr_t *p_RrcMacFddUplinkSfBitmapBr
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacFddUplinkSfBitmapBr != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacFddUplinkSfBitmapBr->fddUplinkSubframeBitmapBr);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacFddUplinkSfBitmapBr_tlv_len
(
    RrcMacFddUplinkSfBitmapBr_t *p_RrcMacFddUplinkSfBitmapBr

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacFddUplinkSfBitmapBr != PNULL);

    length += rrc_il_get_RrcMacFddUplinkSfBitmapBr_len(p_RrcMacFddUplinkSfBitmapBr);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacFddUplinkSfBitmapBr
(
    U8  **pp_buffer,
    RrcMacFddUplinkSfBitmapBr_t *p_RrcMacFddUplinkSfBitmapBr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacFddUplinkSfBitmapBr != PNULL);

    /* This function composes RrcMacFddUplinkSfBitmapBr */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_FDD_UPNLINK_SF_BITMAP_BR;
    tlv_length = rrc_il_get_RrcMacFddUplinkSfBitmapBr_tlv_len(p_RrcMacFddUplinkSfBitmapBr);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacFddUplinkSfBitmapBr->fddUplinkSubframeBitmapBr > EMTC_MAX_FDD_UPLINK_SF_BITMAP))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacFddUplinkSfBitmapBr->fddUplinkSubframeBitmapBr] should be less than"
            " or equal to EMTC_MAX_FDD_UPLINK_SF_BITMAP. Incorrect value %u received.", p_RrcMacFddUplinkSfBitmapBr->fddUplinkSubframeBitmapBr);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_RrcMacFddUplinkSfBitmapBr->fddUplinkSubframeBitmapBr, "fddUplinkSubframeBitmapBr");
    *pp_buffer += sizeof(p_RrcMacFddUplinkSfBitmapBr->fddUplinkSubframeBitmapBr);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacSiMsgBrInfo_len
(
    RrcMacSiMsgBrInfo_t *p_RrcMacSiMsgBrInfo
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacSiMsgBrInfo != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacSiMsgBrInfo->siWindowLengthBr);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacSiMsgBrInfo->siHoppingConfigCommonR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacSiMsgBrInfo->numSiMessageBr);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_RrcMacSiMsgBrInfo->siMsgBrInfoCounter <= ARRSIZE(p_RrcMacSiMsgBrInfo->siMsgsBr));
        for (loop = 0; loop < p_RrcMacSiMsgBrInfo->siMsgBrInfoCounter; loop++)
        {
            length += rrc_il_get_RrcMacSiMsgsBr_tlv_len(&p_RrcMacSiMsgBrInfo->siMsgsBr[loop]);
        }
    }

    /* Get length of TLV */
    if (p_RrcMacSiMsgBrInfo->bitmask & RRC_MAC_FDD_DOWNLINK_TDD_SF_BITMAP_BR_PRESENT)
    {
        length += rrc_il_get_RrcMacFddDownlinkOrTddSfBitmapBr_tlv_len(&p_RrcMacSiMsgBrInfo->fddDownlinkOrTddSfBitmapBr);
    }

    /* Get length of TLV */
    if (p_RrcMacSiMsgBrInfo->bitmask & RRC_MAC_FDD_UPLINK_SF_BITMAP_BR_PRESENT)
    {
        length += rrc_il_get_RrcMacFddUplinkSfBitmapBr_tlv_len(&p_RrcMacSiMsgBrInfo->fddUplinkSfBitmapBr);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacSiMsgBrInfo_tlv_len
(
    RrcMacSiMsgBrInfo_t *p_RrcMacSiMsgBrInfo

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacSiMsgBrInfo != PNULL);

    length += rrc_il_get_RrcMacSiMsgBrInfo_len(p_RrcMacSiMsgBrInfo);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacSiMsgBrInfo
(
    U8  **pp_buffer,
    RrcMacSiMsgBrInfo_t *p_RrcMacSiMsgBrInfo
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacSiMsgBrInfo != PNULL);

    /* This function composes RrcMacSiMsgBrInfo */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SI_MSG_BR_INFO;
    tlv_length = rrc_il_get_RrcMacSiMsgBrInfo_tlv_len(p_RrcMacSiMsgBrInfo);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSiMsgBrInfo->siWindowLengthBr > EMTC_MAX_SI_WINDOW_MS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgBrInfo->siWindowLengthBr] should be less than"
            " or equal to EMTC_MAX_SI_WINDOW_MS. Incorrect value %u received.", p_RrcMacSiMsgBrInfo->siWindowLengthBr);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacSiMsgBrInfo->siWindowLengthBr, "siWindowLengthBr");
    *pp_buffer += sizeof(p_RrcMacSiMsgBrInfo->siWindowLengthBr);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacSiMsgBrInfo->siHoppingConfigCommonR13 > EMTC_SI_HOPPING_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgBrInfo->siHoppingConfigCommonR13] should be less than"
            " or equal to EMTC_SI_HOPPING_MAX. Incorrect value %u received.", p_RrcMacSiMsgBrInfo->siHoppingConfigCommonR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacSiMsgBrInfo->siHoppingConfigCommonR13, "siHoppingConfigCommonR13");
    *pp_buffer += sizeof(p_RrcMacSiMsgBrInfo->siHoppingConfigCommonR13);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacSiMsgBrInfo->numSiMessageBr < 1) || (p_RrcMacSiMsgBrInfo->numSiMessageBr > EMTC_MAX_SI_COUNT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSiMsgBrInfo->numSiMessageBr] should be in range "
            "1 to EMTC_MAX_SI_COUNT. Incorrect value %u received.", p_RrcMacSiMsgBrInfo->numSiMessageBr);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacSiMsgBrInfo->numSiMessageBr, "numSiMessageBr");
    *pp_buffer += sizeof(p_RrcMacSiMsgBrInfo->numSiMessageBr);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_RrcMacSiMsgBrInfo->siMsgBrInfoCounter <= ARRSIZE(p_RrcMacSiMsgBrInfo->siMsgsBr));
        for (loop = 0; loop < p_RrcMacSiMsgBrInfo->siMsgBrInfoCounter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_RrcMacSiMsgsBr(pp_buffer, &p_RrcMacSiMsgBrInfo->siMsgsBr[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_RrcMacSiMsgBrInfo->bitmask & RRC_MAC_FDD_DOWNLINK_TDD_SF_BITMAP_BR_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacFddDownlinkOrTddSfBitmapBr(pp_buffer, &p_RrcMacSiMsgBrInfo->fddDownlinkOrTddSfBitmapBr))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_RrcMacSiMsgBrInfo->bitmask & RRC_MAC_FDD_UPLINK_SF_BITMAP_BR_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacFddUplinkSfBitmapBr(pp_buffer, &p_RrcMacSiMsgBrInfo->fddUplinkSfBitmapBr))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcN1PucchAnInfoListR13_len
(
    RrcN1PucchAnInfoListR13_t *p_RrcN1PucchAnInfoListR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcN1PucchAnInfoListR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcN1PucchAnInfoListR13->numCeLevels);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_RrcN1PucchAnInfoListR13->numCeLevels * sizeof(p_RrcN1PucchAnInfoListR13->n1PucchAnInfo[0]));

    return length;
}

static
rrc_length_t
rrc_il_get_RrcN1PucchAnInfoListR13_tlv_len
(
    RrcN1PucchAnInfoListR13_t *p_RrcN1PucchAnInfoListR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcN1PucchAnInfoListR13 != PNULL);

    length += rrc_il_get_RrcN1PucchAnInfoListR13_len(p_RrcN1PucchAnInfoListR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcN1PucchAnInfoListR13
(
    U8  **pp_buffer,
    RrcN1PucchAnInfoListR13_t *p_RrcN1PucchAnInfoListR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcN1PucchAnInfoListR13 != PNULL);

    /* This function composes RrcN1PucchAnInfoListR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_N1_PUCCH_AN_INFO_LIST;
    tlv_length = rrc_il_get_RrcN1PucchAnInfoListR13_tlv_len(p_RrcN1PucchAnInfoListR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcN1PucchAnInfoListR13->numCeLevels > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcN1PucchAnInfoListR13->numCeLevels] should be less than"
            " or equal to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcN1PucchAnInfoListR13->numCeLevels);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcN1PucchAnInfoListR13->numCeLevels, "numCeLevels");
    *pp_buffer += sizeof(p_RrcN1PucchAnInfoListR13->numCeLevels);

    /* Compose OCTET_STRING VARIABLE of basic type elements with HIGHER boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcN1PucchAnInfoListR13->numCeLevels; loop++)
        {
            if (p_RrcN1PucchAnInfoListR13->n1PucchAnInfo[loop] > EMTC_MAX_N1_PUCCH_AN)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_RrcN1PucchAnInfoListR13->n1PucchAnInfo[%u] should be less"
                    " than or equal to EMTC_MAX_N1_PUCCH_AN. "
                    "Incorrect value %d received.", loop, p_RrcN1PucchAnInfoListR13->n1PucchAnInfo[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U16(*pp_buffer, &p_RrcN1PucchAnInfoListR13->n1PucchAnInfo[loop], "n1PucchAnInfo[]");
            *pp_buffer += sizeof(U16);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacPucchConfigCommonV1310_len
(
    RrcMacPucchConfigCommonV1310_t *p_RrcMacPucchConfigCommonV1310
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPucchConfigCommonV1310 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPucchConfigCommonV1310->numCeLevels);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_RrcMacPucchConfigCommonV1310->numCeLevels * sizeof(p_RrcMacPucchConfigCommonV1310->pucchNumRepetitionCeMsg4PerLevel[0]));

    /* Get length of TLV */
    if (p_RrcMacPucchConfigCommonV1310->bitmask & RRC_MAC_N1_PUCCH_AN_INFO_LIST_PRESENT)
    {
        length += rrc_il_get_RrcN1PucchAnInfoListR13_tlv_len(&p_RrcMacPucchConfigCommonV1310->n1PucchAnInfoList);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacPucchConfigCommonV1310_tlv_len
(
    RrcMacPucchConfigCommonV1310_t *p_RrcMacPucchConfigCommonV1310

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPucchConfigCommonV1310 != PNULL);

    length += rrc_il_get_RrcMacPucchConfigCommonV1310_len(p_RrcMacPucchConfigCommonV1310);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacPucchConfigCommonV1310
(
    U8  **pp_buffer,
    RrcMacPucchConfigCommonV1310_t *p_RrcMacPucchConfigCommonV1310
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacPucchConfigCommonV1310 != PNULL);

    /* This function composes RrcMacPucchConfigCommonV1310 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PUCCH_CONFIG_COMMON_V1310;
    tlv_length = rrc_il_get_RrcMacPucchConfigCommonV1310_tlv_len(p_RrcMacPucchConfigCommonV1310);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPucchConfigCommonV1310->numCeLevels > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPucchConfigCommonV1310->numCeLevels] should be less than"
            " or equal to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcMacPucchConfigCommonV1310->numCeLevels);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPucchConfigCommonV1310->numCeLevels, "numCeLevels");
    *pp_buffer += sizeof(p_RrcMacPucchConfigCommonV1310->numCeLevels);

    /* Compose OCTET_STRING VARIABLE of basic type elements with HIGHER boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_RrcMacPucchConfigCommonV1310->numCeLevels; loop++)
        {
            if (p_RrcMacPucchConfigCommonV1310->pucchNumRepetitionCeMsg4PerLevel[loop] > EMTC_PUCCH_NUM_REPETITION_CE_MSG4_MAX)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_RrcMacPucchConfigCommonV1310->pucchNumRepetitionCeMsg4PerLevel[%u] should be less"
                    " than or equal to EMTC_PUCCH_NUM_REPETITION_CE_MSG4_MAX. "
                    "Incorrect value %d received.", loop, p_RrcMacPucchConfigCommonV1310->pucchNumRepetitionCeMsg4PerLevel[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPucchConfigCommonV1310->pucchNumRepetitionCeMsg4PerLevel[loop], "pucchNumRepetitionCeMsg4PerLevel[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose TLV  */
    if (p_RrcMacPucchConfigCommonV1310->bitmask & RRC_MAC_N1_PUCCH_AN_INFO_LIST_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcN1PucchAnInfoListR13(pp_buffer, &p_RrcMacPucchConfigCommonV1310->n1PucchAnInfoList))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacModificationPeriodV1310_len
(
    RrcMacModificationPeriodV1310_t *p_RrcMacModificationPeriodV1310
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacModificationPeriodV1310 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacModificationPeriodV1310->modificationPeriodV1310);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacModificationPeriodV1310_tlv_len
(
    RrcMacModificationPeriodV1310_t *p_RrcMacModificationPeriodV1310

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacModificationPeriodV1310 != PNULL);

    length += rrc_il_get_RrcMacModificationPeriodV1310_len(p_RrcMacModificationPeriodV1310);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacModificationPeriodV1310
(
    U8  **pp_buffer,
    RrcMacModificationPeriodV1310_t *p_RrcMacModificationPeriodV1310
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacModificationPeriodV1310 != PNULL);

    /* This function composes RrcMacModificationPeriodV1310 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MODIFICATION_PERIOD_V1310;
    tlv_length = rrc_il_get_RrcMacModificationPeriodV1310_tlv_len(p_RrcMacModificationPeriodV1310);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacModificationPeriodV1310->modificationPeriodV1310 < 64) || (p_RrcMacModificationPeriodV1310->modificationPeriodV1310 > EMTC_MAX_MOD_PERIOD))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacModificationPeriodV1310->modificationPeriodV1310] should be in range "
            "64 to EMTC_MAX_MOD_PERIOD. Incorrect value %u received.", p_RrcMacModificationPeriodV1310->modificationPeriodV1310);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_RrcMacModificationPeriodV1310->modificationPeriodV1310, "modificationPeriodV1310");
    *pp_buffer += sizeof(p_RrcMacModificationPeriodV1310->modificationPeriodV1310);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacPcchConfigV1310_len
(
    RrcMacPcchConfigV1310_t *p_RrcMacPcchConfigV1310
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPcchConfigV1310 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPcchConfigV1310->pagingNarrowBandsR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPcchConfigV1310->mpdcchMaxNumRepetitionPagingR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPcchConfigV1310->nBV1310);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPcchConfigV1310->sfnGapV1310);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacPcchConfigV1310_tlv_len
(
    RrcMacPcchConfigV1310_t *p_RrcMacPcchConfigV1310

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPcchConfigV1310 != PNULL);

    length += rrc_il_get_RrcMacPcchConfigV1310_len(p_RrcMacPcchConfigV1310);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacPcchConfigV1310
(
    U8  **pp_buffer,
    RrcMacPcchConfigV1310_t *p_RrcMacPcchConfigV1310
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacPcchConfigV1310 != PNULL);

    /* This function composes RrcMacPcchConfigV1310 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PCCH_CONFIG_V1310;
    tlv_length = rrc_il_get_RrcMacPcchConfigV1310_tlv_len(p_RrcMacPcchConfigV1310);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacPcchConfigV1310->pagingNarrowBandsR13 < 1) || (p_RrcMacPcchConfigV1310->pagingNarrowBandsR13 > EMTC_MAX_AVAILABLE_NARROWBAND))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPcchConfigV1310->pagingNarrowBandsR13] should be in range "
            "1 to EMTC_MAX_AVAILABLE_NARROWBAND. Incorrect value %u received.", p_RrcMacPcchConfigV1310->pagingNarrowBandsR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPcchConfigV1310->pagingNarrowBandsR13, "pagingNarrowBandsR13");
    *pp_buffer += sizeof(p_RrcMacPcchConfigV1310->pagingNarrowBandsR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPcchConfigV1310->mpdcchMaxNumRepetitionPagingR13 > EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPcchConfigV1310->mpdcchMaxNumRepetitionPagingR13] should be less than"
            " or equal to EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_MAX. Incorrect value %u received.", p_RrcMacPcchConfigV1310->mpdcchMaxNumRepetitionPagingR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPcchConfigV1310->mpdcchMaxNumRepetitionPagingR13, "mpdcchMaxNumRepetitionPagingR13");
    *pp_buffer += sizeof(p_RrcMacPcchConfigV1310->mpdcchMaxNumRepetitionPagingR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPcchConfigV1310->nBV1310 > EMTC_NB_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPcchConfigV1310->nBV1310] should be less than"
            " or equal to EMTC_NB_MAX. Incorrect value %u received.", p_RrcMacPcchConfigV1310->nBV1310);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPcchConfigV1310->nBV1310, "nBV1310");
    *pp_buffer += sizeof(p_RrcMacPcchConfigV1310->nBV1310);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacPcchConfigV1310->sfnGapV1310 < 1) || (p_RrcMacPcchConfigV1310->sfnGapV1310 > EMTC_SFN_GAP_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPcchConfigV1310->sfnGapV1310] should be in range "
            "1 to EMTC_SFN_GAP_MAX. Incorrect value %u received.", p_RrcMacPcchConfigV1310->sfnGapV1310);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_RrcMacPcchConfigV1310->sfnGapV1310, "sfnGapV1310");
    *pp_buffer += sizeof(p_RrcMacPcchConfigV1310->sfnGapV1310);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacPdschConfigCommonV1310_len
(
    RrcMacPdschConfigCommonV1310_t *p_RrcMacPdschConfigCommonV1310
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPdschConfigCommonV1310 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPdschConfigCommonV1310->pdschMaxNumRepetitionCeModeA);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacPdschConfigCommonV1310_tlv_len
(
    RrcMacPdschConfigCommonV1310_t *p_RrcMacPdschConfigCommonV1310

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPdschConfigCommonV1310 != PNULL);

    length += rrc_il_get_RrcMacPdschConfigCommonV1310_len(p_RrcMacPdschConfigCommonV1310);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacPdschConfigCommonV1310
(
    U8  **pp_buffer,
    RrcMacPdschConfigCommonV1310_t *p_RrcMacPdschConfigCommonV1310
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacPdschConfigCommonV1310 != PNULL);

    /* This function composes RrcMacPdschConfigCommonV1310 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PDSCH_CONFIG_COMMON_V1310;
    tlv_length = rrc_il_get_RrcMacPdschConfigCommonV1310_tlv_len(p_RrcMacPdschConfigCommonV1310);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPdschConfigCommonV1310->pdschMaxNumRepetitionCeModeA > EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPdschConfigCommonV1310->pdschMaxNumRepetitionCeModeA] should be less than"
            " or equal to EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_MAX. Incorrect value %u received.", p_RrcMacPdschConfigCommonV1310->pdschMaxNumRepetitionCeModeA);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPdschConfigCommonV1310->pdschMaxNumRepetitionCeModeA, "pdschMaxNumRepetitionCeModeA");
    *pp_buffer += sizeof(p_RrcMacPdschConfigCommonV1310->pdschMaxNumRepetitionCeModeA);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacPuschConfigCommonV1310_len
(
    RrcMacPuschConfigCommonV1310_t *p_RrcMacPuschConfigCommonV1310
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPuschConfigCommonV1310 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPuschConfigCommonV1310->puschMaxNumRepetitionCeModeA);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacPuschConfigCommonV1310_tlv_len
(
    RrcMacPuschConfigCommonV1310_t *p_RrcMacPuschConfigCommonV1310

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPuschConfigCommonV1310 != PNULL);

    length += rrc_il_get_RrcMacPuschConfigCommonV1310_len(p_RrcMacPuschConfigCommonV1310);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacPuschConfigCommonV1310
(
    U8  **pp_buffer,
    RrcMacPuschConfigCommonV1310_t *p_RrcMacPuschConfigCommonV1310
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacPuschConfigCommonV1310 != PNULL);

    /* This function composes RrcMacPuschConfigCommonV1310 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PUSCH_CONFIG_COMMON_V1310;
    tlv_length = rrc_il_get_RrcMacPuschConfigCommonV1310_tlv_len(p_RrcMacPuschConfigCommonV1310);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPuschConfigCommonV1310->puschMaxNumRepetitionCeModeA > EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPuschConfigCommonV1310->puschMaxNumRepetitionCeModeA] should be less than"
            " or equal to EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_MAX. Incorrect value %u received.", p_RrcMacPuschConfigCommonV1310->puschMaxNumRepetitionCeModeA);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPuschConfigCommonV1310->puschMaxNumRepetitionCeModeA, "puschMaxNumRepetitionCeModeA");
    *pp_buffer += sizeof(p_RrcMacPuschConfigCommonV1310->puschMaxNumRepetitionCeModeA);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcFrequancyHoppingParametersR13_len
(
    RrcFrequancyHoppingParametersR13_t *p_RrcFrequancyHoppingParametersR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcFrequancyHoppingParametersR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcFrequancyHoppingParametersR13->type);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcFrequancyHoppingParametersR13->intervalUlHoppingConfigCommonModeAFddR13);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcFrequancyHoppingParametersR13_tlv_len
(
    RrcFrequancyHoppingParametersR13_t *p_RrcFrequancyHoppingParametersR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcFrequancyHoppingParametersR13 != PNULL);

    length += rrc_il_get_RrcFrequancyHoppingParametersR13_len(p_RrcFrequancyHoppingParametersR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcFrequancyHoppingParametersR13
(
    U8  **pp_buffer,
    RrcFrequancyHoppingParametersR13_t *p_RrcFrequancyHoppingParametersR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcFrequancyHoppingParametersR13 != PNULL);

    /* This function composes RrcFrequancyHoppingParametersR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_FREQUANCY_HOPPING_PARAMETERS_R13;
    tlv_length = rrc_il_get_RrcFrequancyHoppingParametersR13_tlv_len(p_RrcFrequancyHoppingParametersR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcFrequancyHoppingParametersR13->type > RRC_ONE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcFrequancyHoppingParametersR13->type] should be less than"
            " or equal to RRC_ONE. Incorrect value %u received.", p_RrcFrequancyHoppingParametersR13->type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcFrequancyHoppingParametersR13->type, "type");
    *pp_buffer += sizeof(p_RrcFrequancyHoppingParametersR13->type);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcFrequancyHoppingParametersR13->intervalUlHoppingConfigCommonModeAFddR13 > EMTC_INTERVAL_UL_HOPPING_MODE_A_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcFrequancyHoppingParametersR13->intervalUlHoppingConfigCommonModeAFddR13] should be less than"
            " or equal to EMTC_INTERVAL_UL_HOPPING_MODE_A_MAX. Incorrect value %u received.", p_RrcFrequancyHoppingParametersR13->intervalUlHoppingConfigCommonModeAFddR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcFrequancyHoppingParametersR13->intervalUlHoppingConfigCommonModeAFddR13, "intervalUlHoppingConfigCommonModeAFddR13");
    *pp_buffer += sizeof(p_RrcFrequancyHoppingParametersR13->intervalUlHoppingConfigCommonModeAFddR13);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_config_cell_cnf_len
(
    rrc_mac_config_cell_cnf_t *p_rrc_mac_config_cell_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_config_cell_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_cnf->response);

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_cnf->optional_elems_present & RRC_MAC_CONFIG_CELL_ERROR_CODE_PRESENT)
    {
        length += rrc_il_get_rrc_mac_config_cell_error_code_tlv_len(&p_rrc_mac_config_cell_cnf->config_cell_error_code);
    }

    /* Get length of TLV */
    if (p_rrc_mac_config_cell_cnf->optional_elems_present & RRC_MAC_CONFIG_CELL_EARFCN_LIST_PRESENT)
    {
        length += rrc_il_get_rrc_dl_earfcn_tlv_len(&p_rrc_mac_config_cell_cnf->dl_earfcn_list);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_config_cell_cnf
(
    U8  **pp_buffer,
    rrc_mac_config_cell_cnf_t *p_rrc_mac_config_cell_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_config_cell_cnf != PNULL);

    /* This function composes rrc_mac_config_cell_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_config_cell_cnf->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_cnf->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_config_cell_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_config_cell_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_cnf->response);

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_cnf->optional_elems_present & RRC_MAC_CONFIG_CELL_ERROR_CODE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_config_cell_error_code(pp_buffer, &p_rrc_mac_config_cell_cnf->config_cell_error_code))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_config_cell_cnf->optional_elems_present & RRC_MAC_CONFIG_CELL_EARFCN_LIST_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_dl_earfcn(pp_buffer, &p_rrc_mac_config_cell_cnf->dl_earfcn_list))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_config_cell_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_CONFIG_CELL_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_config_cell_cnf
(
    rrc_mac_config_cell_cnf_t  *p_rrc_mac_config_cell_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_config_cell_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_config_cell_cnf_len(p_rrc_mac_config_cell_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_CONFIG_CELL_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_CONFIG_CELL_CNF, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_CONFIG_CELL_CNF, msg_length,cell_index);

    /* Fill RRC_MAC_CONFIG_CELL_CNF message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_config_cell_cnf(&p_mac_msg, p_rrc_mac_config_cell_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_config_cell_error_code_len
(
    rrc_mac_config_cell_error_code_t *p_rrc_mac_config_cell_error_code
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_config_cell_error_code != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_config_cell_error_code->error_code);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_config_cell_error_code_tlv_len
(
    rrc_mac_config_cell_error_code_t *p_rrc_mac_config_cell_error_code

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_config_cell_error_code != PNULL);

    length += rrc_il_get_rrc_mac_config_cell_error_code_len(p_rrc_mac_config_cell_error_code);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_config_cell_error_code
(
    U8  **pp_buffer,
    rrc_mac_config_cell_error_code_t *p_rrc_mac_config_cell_error_code
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_config_cell_error_code != PNULL);

    /* This function composes rrc_mac_config_cell_error_code */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CONFIG_CELL_ERROR_CODE;
    tlv_length = rrc_il_get_rrc_mac_config_cell_error_code_tlv_len(p_rrc_mac_config_cell_error_code);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_config_cell_error_code->error_code > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_config_cell_error_code->error_code] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_mac_config_cell_error_code->error_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_config_cell_error_code->error_code, "error_code");
    *pp_buffer += sizeof(p_rrc_mac_config_cell_error_code->error_code);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_dl_earfcn_len
(
    rrc_dl_earfcn_t *p_rrc_dl_earfcn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_dl_earfcn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_dl_earfcn->count);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrc_dl_earfcn->count * sizeof(p_rrc_dl_earfcn->earfcnDl[0]));

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_dl_earfcn_tlv_len
(
    rrc_dl_earfcn_t *p_rrc_dl_earfcn

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_dl_earfcn != PNULL);

    length += rrc_il_get_rrc_dl_earfcn_len(p_rrc_dl_earfcn);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_dl_earfcn
(
    U8  **pp_buffer,
    rrc_dl_earfcn_t *p_rrc_dl_earfcn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_dl_earfcn != PNULL);

    /* This function composes rrc_dl_earfcn */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_LAA_CELL_CONFIG_EARFCN;
    tlv_length = rrc_il_get_rrc_dl_earfcn_tlv_len(p_rrc_dl_earfcn);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_dl_earfcn->count > RRC_PHY_CELL_PARAMS_DL_EARFCN_MAX_COUNT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_dl_earfcn->count] should be less than"
            " or equal to RRC_PHY_CELL_PARAMS_DL_EARFCN_MAX_COUNT. Incorrect value %u received.", p_rrc_dl_earfcn->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_dl_earfcn->count, "count");
    *pp_buffer += sizeof(p_rrc_dl_earfcn->count);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_dl_earfcn->count; loop++)
        {
            rrc_cp_pack_U32(*pp_buffer, &p_rrc_dl_earfcn->earfcnDl[loop], "earfcnDl[]");
            *pp_buffer += sizeof(U32);
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_sfn_req_len
(
    rrc_mac_sfn_req_t *p_rrc_mac_sfn_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sfn_req != PNULL);

    /* Get length of TLV */
    if (p_rrc_mac_sfn_req->optional_elems_present & RRC_MAC_TIMESTAMP_REQD_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_timestamp_reqd_tlv_len(&p_rrc_mac_sfn_req->timestamp_reqd);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_sfn_req
(
    U8  **pp_buffer,
    rrc_mac_sfn_req_t *p_rrc_mac_sfn_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sfn_req != PNULL);

    /* This function composes rrc_mac_sfn_req */
    

    /* Compose TLV  */
    if (p_rrc_mac_sfn_req->optional_elems_present & RRC_MAC_TIMESTAMP_REQD_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_timestamp_reqd(pp_buffer, &p_rrc_mac_sfn_req->timestamp_reqd))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_sfn_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_SFN_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_sfn_req
(
    rrc_mac_sfn_req_t  *p_rrc_mac_sfn_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_sfn_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_sfn_req_len(p_rrc_mac_sfn_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_COMP_PARSE, "Src(%u)->Dst(%u):RRC_MAC_SFN_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_SFN_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_SFN_REQ, msg_length,cell_index);

    /* Fill RRC_MAC_SFN_REQ message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_sfn_req(&p_mac_msg, p_rrc_mac_sfn_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_timestamp_reqd_len
(
    rrc_mac_timestamp_reqd_t *p_rrc_mac_timestamp_reqd
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_timestamp_reqd != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_timestamp_reqd->timestamp_reqd);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_timestamp_reqd_tlv_len
(
    rrc_mac_timestamp_reqd_t *p_rrc_mac_timestamp_reqd

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_timestamp_reqd != PNULL);

    length += rrc_il_get_rrc_mac_timestamp_reqd_len(p_rrc_mac_timestamp_reqd);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_timestamp_reqd
(
    U8  **pp_buffer,
    rrc_mac_timestamp_reqd_t *p_rrc_mac_timestamp_reqd
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_timestamp_reqd != PNULL);

    /* This function composes rrc_mac_timestamp_reqd */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_TIMESTAMP_REQD_INFO;
    tlv_length = rrc_il_get_rrc_mac_timestamp_reqd_tlv_len(p_rrc_mac_timestamp_reqd);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_timestamp_reqd->timestamp_reqd > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_timestamp_reqd->timestamp_reqd] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_timestamp_reqd->timestamp_reqd);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_timestamp_reqd->timestamp_reqd, "timestamp_reqd");
    *pp_buffer += sizeof(p_rrc_mac_timestamp_reqd->timestamp_reqd);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_sfn_cnf_len
(
    rrc_mac_sfn_cnf_t *p_rrc_mac_sfn_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sfn_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sfn_cnf->response);

    /* Get length of TLV */
    if (p_rrc_mac_sfn_cnf->optional_elems_present & RRC_MAC_SFN_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sfn_tlv_len(&p_rrc_mac_sfn_cnf->sfn);
    }

    /* Get length of TLV */
    if (p_rrc_mac_sfn_cnf->optional_elems_present & RRC_MAC_SF_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sf_tlv_len(&p_rrc_mac_sfn_cnf->sf);
    }

    /* Get length of TLV */
    if (p_rrc_mac_sfn_cnf->optional_elems_present & RRC_MAC_TIMESTAMP_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_timestamp_tlv_len(&p_rrc_mac_sfn_cnf->timestamp);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_sfn_cnf
(
    U8  **pp_buffer,
    rrc_mac_sfn_cnf_t *p_rrc_mac_sfn_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sfn_cnf != PNULL);

    /* This function composes rrc_mac_sfn_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sfn_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sfn_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_sfn_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_sfn_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_mac_sfn_cnf->response);

    /* Compose TLV  */
    if (p_rrc_mac_sfn_cnf->optional_elems_present & RRC_MAC_SFN_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sfn(pp_buffer, &p_rrc_mac_sfn_cnf->sfn))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_sfn_cnf->optional_elems_present & RRC_MAC_SF_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sf(pp_buffer, &p_rrc_mac_sfn_cnf->sf))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_sfn_cnf->optional_elems_present & RRC_MAC_TIMESTAMP_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_timestamp(pp_buffer, &p_rrc_mac_sfn_cnf->timestamp))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_sfn_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_SFN_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_sfn_cnf
(
    rrc_mac_sfn_cnf_t  *p_rrc_mac_sfn_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_sfn_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_sfn_cnf_len(p_rrc_mac_sfn_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_COMP_PARSE, "Src(%u)->Dst(%u):RRC_MAC_SFN_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_SFN_CNF, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_SFN_CNF, msg_length,cell_index);

    /* Fill RRC_MAC_SFN_CNF message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_sfn_cnf(&p_mac_msg, p_rrc_mac_sfn_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sfn_len
(
    rrc_mac_sfn_t *p_rrc_mac_sfn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sfn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sfn->sfn);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sfn_tlv_len
(
    rrc_mac_sfn_t *p_rrc_mac_sfn

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sfn != PNULL);

    length += rrc_il_get_rrc_mac_sfn_len(p_rrc_mac_sfn);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sfn
(
    U8  **pp_buffer,
    rrc_mac_sfn_t *p_rrc_mac_sfn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sfn != PNULL);

    /* This function composes rrc_mac_sfn */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SFN_INFO;
    tlv_length = rrc_il_get_rrc_mac_sfn_tlv_len(p_rrc_mac_sfn);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sfn->sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sfn->sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_sfn->sfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_sfn->sfn, "sfn");
    *pp_buffer += sizeof(p_rrc_mac_sfn->sfn);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sf_len
(
    rrc_mac_sf_t *p_rrc_mac_sf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sf->sf);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sf_tlv_len
(
    rrc_mac_sf_t *p_rrc_mac_sf

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sf != PNULL);

    length += rrc_il_get_rrc_mac_sf_len(p_rrc_mac_sf);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sf
(
    U8  **pp_buffer,
    rrc_mac_sf_t *p_rrc_mac_sf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sf != PNULL);

    /* This function composes rrc_mac_sf */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SF_INFO;
    tlv_length = rrc_il_get_rrc_mac_sf_tlv_len(p_rrc_mac_sf);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sf->sf > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sf->sf] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_sf->sf);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sf->sf, "sf");
    *pp_buffer += sizeof(p_rrc_mac_sf->sf);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_timestamp_len
(
    rrc_timestamp_t *p_rrc_timestamp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_timestamp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_timestamp->ts_sec);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_timestamp->ts_nsec);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_timestamp_tlv_len
(
    rrc_timestamp_t *p_rrc_timestamp

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_timestamp != PNULL);

    length += rrc_il_get_rrc_timestamp_len(p_rrc_timestamp);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_timestamp
(
    U8  **pp_buffer,
    rrc_timestamp_t *p_rrc_timestamp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_timestamp != PNULL);

    /* This function composes rrc_timestamp */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_TIMESTAMP_INFO;
    tlv_length = rrc_il_get_rrc_timestamp_tlv_len(p_rrc_timestamp);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_timestamp->ts_sec > 2147483647))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_timestamp->ts_sec] should be less than"
            " or equal to 2147483647. Incorrect value %llu received.", p_rrc_timestamp->ts_sec);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U64(*pp_buffer, &p_rrc_timestamp->ts_sec, "ts_sec");
    *pp_buffer += sizeof(p_rrc_timestamp->ts_sec);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_timestamp->ts_nsec > 2147483647))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_timestamp->ts_nsec] should be less than"
            " or equal to 2147483647. Incorrect value %u received.", p_rrc_timestamp->ts_nsec);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_timestamp->ts_nsec, "ts_nsec");
    *pp_buffer += sizeof(p_rrc_timestamp->ts_nsec);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_sfn_ind_len
(
    rrc_mac_sfn_ind_t *p_rrc_mac_sfn_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sfn_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sfn_ind->sfn);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_sfn_ind
(
    U8  **pp_buffer,
    rrc_mac_sfn_ind_t *p_rrc_mac_sfn_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sfn_ind != PNULL);

    /* This function composes rrc_mac_sfn_ind */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sfn_ind->sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sfn_ind->sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_sfn_ind->sfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_sfn_ind->sfn, "sfn");
    *pp_buffer += sizeof(p_rrc_mac_sfn_ind->sfn);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_sfn_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_SFN_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_sfn_ind
(
    rrc_mac_sfn_ind_t  *p_rrc_mac_sfn_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_sfn_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_sfn_ind_len(p_rrc_mac_sfn_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_COMP_PARSE, "Src(%u)->Dst(%u):RRC_MAC_SFN_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_SFN_IND, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_SFN_IND, msg_length,cell_index);

    /* Fill RRC_MAC_SFN_IND message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_sfn_ind(&p_mac_msg, p_rrc_mac_sfn_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_reconfig_cell_req_len
(
    rrc_mac_reconfig_cell_req_t *p_rrc_mac_reconfig_cell_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reconfig_cell_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reconfig_cell_req->sfn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reconfig_cell_req->sf);

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_DCI_FORMAT_FOR_SI_PRESENT)
    {
        length += rrc_il_get_rrc_mac_dci_format_for_si_tlv_len(&p_rrc_mac_reconfig_cell_req->dci_format_for_si);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_RBS_PER_TBS_DCI_1A_PRESENT)
    {
        length += rrc_il_get_rrc_mac_rbs_for_dci_1a_tlv_len(&p_rrc_mac_reconfig_cell_req->num_rbs_per_tbs_dci_1a);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_RBS_PER_TBS_DCI_1C_PRESENT)
    {
        length += rrc_il_get_rrc_mac_rbs_for_dci_1c_tlv_len(&p_rrc_mac_reconfig_cell_req->num_rbs_per_tbs_dci_1c);
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfig_cell_req->mib_msg_req_counter <= ARRSIZE(p_rrc_mac_reconfig_cell_req->mib_msg_req));
        for (loop = 0; loop < p_rrc_mac_reconfig_cell_req->mib_msg_req_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_mib_msg_req_tlv_len(&p_rrc_mac_reconfig_cell_req->mib_msg_req[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SIB_TYPE_1_MSG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sibtype1_msg_req_tlv_len(&p_rrc_mac_reconfig_cell_req->sib1_msg_req);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SI_MSG_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_si_msg_req_tlv_len(&p_rrc_mac_reconfig_cell_req->si_msg_req);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_MOD_PERIOD_PRESENT)
    {
        length += rrc_il_get_rrc_mac_mod_period_info_tlv_len(&p_rrc_mac_reconfig_cell_req->mod_period_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SFN_GAP_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sfn_gap_info_tlv_len(&p_rrc_mac_reconfig_cell_req->sfn_gap_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_CONTN_FREE_RACH_TIMER_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_contn_free_rach_timer_info_tlv_len(&p_rrc_mac_reconfig_cell_req->contn_free_rach_timer_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PDSCH_CONFIG_COMMON_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pdsch_config_common_info_tlv_len(&p_rrc_mac_reconfig_cell_req->pdsch_config_common_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_DOWNLINK_POWER_CONTROL_COMMON_PRESENT)
    {
        length += rrc_il_get_rrc_mac_downlink_power_control_common_tlv_len(&p_rrc_mac_reconfig_cell_req->downlink_power_control_common);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_UPLINK_POWER_CONTROL_COMMON_PRESENT)
    {
        length += rrc_il_get_rrc_mac_uplink_power_control_common_tlv_len(&p_rrc_mac_reconfig_cell_req->uplink_power_control_common_t);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_UL_SYNC_LOSS_TIMER_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ul_sync_loss_timer_info_tlv_len(&p_rrc_mac_reconfig_cell_req->ul_sync_loss_timer_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pucch_cqi_sinr_threshold_info_tlv_len(&p_rrc_mac_reconfig_cell_req->pucch_cqi_sinr_threshold_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PUSCH_CONFIG_COMMON_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pusch_config_common_info_tlv_len(&p_rrc_mac_reconfig_cell_req->pusch_config_common_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_N_GAP_PRESENT)
    {
        length += rrc_il_get_rrc_mac_n_gap_tlv_len(&p_rrc_mac_reconfig_cell_req->n_gap);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SPS_CRNTI_RANGE_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sps_crnti_range_tlv_len(&p_rrc_mac_reconfig_cell_req->sps_crnti_range);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PRS_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_prs_config_info_tlv_len(&p_rrc_mac_reconfig_cell_req->prs_reconfig_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_EMBMS_SIB2_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_m2ap_embms_sib2_info_tlv_len(&p_rrc_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_SIB13_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_m2ap_mac_embms_sib13_info_tlv_len(&p_rrc_mac_reconfig_cell_req->rrc_m2ap_mac_sib13_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_M2AP_MBMS_AREA_CREATE_PRESENT)
    {
        length += rrc_il_get_rrc_m2ap_mac_create_embms_area_entity_info_tlv_len(&p_rrc_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_M2AP_MBMS_AREA_DELETE_PRESENT)
    {
        length += rrc_il_get_rrc_m2ap_mac_delete_embms_area_entity_info_tlv_len(&p_rrc_mac_reconfig_cell_req->rrc_m2ap_mac_delete_embms_area_entity_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_M2AP_EMBMS_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_m2ap_mac_embms_params_info_tlv_len(&p_rrc_mac_reconfig_cell_req->rrc_m2ap_mac_embms_params_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SFN_EMTC_PRESENT)
    {
        length += rrc_il_get_RrcMacEmtcSfn_tlv_len(&p_rrc_mac_reconfig_cell_req->emtcSfn);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SIBTYPE1_BR_MSG_INFO_PRESENT)
    {
        length += rrc_il_get_RrcMacSibtype1BrMsgInfo_tlv_len(&p_rrc_mac_reconfig_cell_req->sibType1BrMsgInfo);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SI_MSG_BR_INFO_PRESENT)
    {
        length += rrc_il_get_RrcMacSiMsgBrInfo_tlv_len(&p_rrc_mac_reconfig_cell_req->siMsgBrInfo);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PUCCH_CONFIG_COMMON_V1310_PRESENT)
    {
        length += rrc_il_get_RrcMacPucchConfigCommonV1310_tlv_len(&p_rrc_mac_reconfig_cell_req->pucchConfigCommonV1310);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_MODIFICATION_PERIOD_V1310_PRESENT)
    {
        length += rrc_il_get_RrcMacModificationPeriodV1310_tlv_len(&p_rrc_mac_reconfig_cell_req->modificationPeriodV1310);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PDSCH_CONFIG_COMMON_V1310_PRESENT)
    {
        length += rrc_il_get_RrcMacPdschConfigCommonV1310_tlv_len(&p_rrc_mac_reconfig_cell_req->pdschConfigCommonV1310);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PUSCH_CONFIG_COMMON_V1310_PRESENT)
    {
        length += rrc_il_get_RrcMacPuschConfigCommonV1310_tlv_len(&p_rrc_mac_reconfig_cell_req->puschConfigCommonV1310);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_FREQUANCY_HOPPING_PARAMETERS_R13_PRESENT)
    {
        length += rrc_il_get_RrcFrequancyHoppingParametersR13_tlv_len(&p_rrc_mac_reconfig_cell_req->frequancyHoppingParametersR13);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SFN_GAP_V1310_PRESENT)
    {
        length += rrc_il_get_RrcMacSfnGapInfoV1310_tlv_len(&p_rrc_mac_reconfig_cell_req->sfnGapInfoV1310);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_reconfig_cell_req
(
    U8  **pp_buffer,
    rrc_mac_reconfig_cell_req_t *p_rrc_mac_reconfig_cell_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_reconfig_cell_req != PNULL);

    /* This function composes rrc_mac_reconfig_cell_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfig_cell_req->sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfig_cell_req->sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_reconfig_cell_req->sfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_reconfig_cell_req->sfn, "sfn");
    *pp_buffer += sizeof(p_rrc_mac_reconfig_cell_req->sfn);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfig_cell_req->sf > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfig_cell_req->sf] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_reconfig_cell_req->sf);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_reconfig_cell_req->sf, "sf");
    *pp_buffer += sizeof(p_rrc_mac_reconfig_cell_req->sf);

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_DCI_FORMAT_FOR_SI_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_dci_format_for_si(pp_buffer, &p_rrc_mac_reconfig_cell_req->dci_format_for_si))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_RBS_PER_TBS_DCI_1A_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_rbs_for_dci_1a(pp_buffer, &p_rrc_mac_reconfig_cell_req->num_rbs_per_tbs_dci_1a))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_RBS_PER_TBS_DCI_1C_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_rbs_for_dci_1c(pp_buffer, &p_rrc_mac_reconfig_cell_req->num_rbs_per_tbs_dci_1c))
        {
            return RRC_FAILURE;
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfig_cell_req->mib_msg_req_counter <= ARRSIZE(p_rrc_mac_reconfig_cell_req->mib_msg_req));
        for (loop = 0; loop < p_rrc_mac_reconfig_cell_req->mib_msg_req_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_mib_msg_req(pp_buffer, &p_rrc_mac_reconfig_cell_req->mib_msg_req[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SIB_TYPE_1_MSG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sibtype1_msg_req(pp_buffer, &p_rrc_mac_reconfig_cell_req->sib1_msg_req))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SI_MSG_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_si_msg_req(pp_buffer, &p_rrc_mac_reconfig_cell_req->si_msg_req))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_MOD_PERIOD_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_mod_period_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->mod_period_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SFN_GAP_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sfn_gap_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->sfn_gap_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_CONTN_FREE_RACH_TIMER_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_contn_free_rach_timer_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->contn_free_rach_timer_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PDSCH_CONFIG_COMMON_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pdsch_config_common_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->pdsch_config_common_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_DOWNLINK_POWER_CONTROL_COMMON_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_downlink_power_control_common(pp_buffer, &p_rrc_mac_reconfig_cell_req->downlink_power_control_common))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_UPLINK_POWER_CONTROL_COMMON_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_uplink_power_control_common(pp_buffer, &p_rrc_mac_reconfig_cell_req->uplink_power_control_common_t))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_UL_SYNC_LOSS_TIMER_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ul_sync_loss_timer_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->ul_sync_loss_timer_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pucch_cqi_sinr_threshold_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->pucch_cqi_sinr_threshold_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PUSCH_CONFIG_COMMON_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pusch_config_common_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->pusch_config_common_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_N_GAP_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_n_gap(pp_buffer, &p_rrc_mac_reconfig_cell_req->n_gap))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SPS_CRNTI_RANGE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sps_crnti_range(pp_buffer, &p_rrc_mac_reconfig_cell_req->sps_crnti_range))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PRS_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_prs_config_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->prs_reconfig_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_EMBMS_SIB2_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_m2ap_embms_sib2_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_SIB13_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_m2ap_mac_embms_sib13_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->rrc_m2ap_mac_sib13_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_M2AP_MBMS_AREA_CREATE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_m2ap_mac_create_embms_area_entity_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_M2AP_MBMS_AREA_DELETE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_m2ap_mac_delete_embms_area_entity_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->rrc_m2ap_mac_delete_embms_area_entity_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_M2AP_EMBMS_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_m2ap_mac_embms_params_info(pp_buffer, &p_rrc_mac_reconfig_cell_req->rrc_m2ap_mac_embms_params_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SFN_EMTC_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacEmtcSfn(pp_buffer, &p_rrc_mac_reconfig_cell_req->emtcSfn))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SIBTYPE1_BR_MSG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacSibtype1BrMsgInfo(pp_buffer, &p_rrc_mac_reconfig_cell_req->sibType1BrMsgInfo))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SI_MSG_BR_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacSiMsgBrInfo(pp_buffer, &p_rrc_mac_reconfig_cell_req->siMsgBrInfo))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PUCCH_CONFIG_COMMON_V1310_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacPucchConfigCommonV1310(pp_buffer, &p_rrc_mac_reconfig_cell_req->pucchConfigCommonV1310))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_MODIFICATION_PERIOD_V1310_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacModificationPeriodV1310(pp_buffer, &p_rrc_mac_reconfig_cell_req->modificationPeriodV1310))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PDSCH_CONFIG_COMMON_V1310_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacPdschConfigCommonV1310(pp_buffer, &p_rrc_mac_reconfig_cell_req->pdschConfigCommonV1310))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_PUSCH_CONFIG_COMMON_V1310_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacPuschConfigCommonV1310(pp_buffer, &p_rrc_mac_reconfig_cell_req->puschConfigCommonV1310))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_FREQUANCY_HOPPING_PARAMETERS_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcFrequancyHoppingParametersR13(pp_buffer, &p_rrc_mac_reconfig_cell_req->frequancyHoppingParametersR13))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_cell_req->optional_elems_present & RRC_RECONFIG_MAC_SFN_GAP_V1310_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacSfnGapInfoV1310(pp_buffer, &p_rrc_mac_reconfig_cell_req->sfnGapInfoV1310))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_reconfig_cell_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_RECONFIG_CELL_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_reconfig_cell_req
(
    rrc_mac_reconfig_cell_req_t  *p_rrc_mac_reconfig_cell_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_reconfig_cell_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_reconfig_cell_req_len(p_rrc_mac_reconfig_cell_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_RECONFIG_CELL_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_RECONFIG_CELL_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_RECONFIG_CELL_REQ, msg_length,cell_index);

    /* Fill RRC_MAC_RECONFIG_CELL_REQ message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_reconfig_cell_req(&p_mac_msg, p_rrc_mac_reconfig_cell_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dci_format_for_si_len
(
    rrc_mac_dci_format_for_si_t *p_rrc_mac_dci_format_for_si
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dci_format_for_si != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_dci_format_for_si->dci_format_for_si_msgs);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dci_format_for_si_tlv_len
(
    rrc_mac_dci_format_for_si_t *p_rrc_mac_dci_format_for_si

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dci_format_for_si != PNULL);

    length += rrc_il_get_rrc_mac_dci_format_for_si_len(p_rrc_mac_dci_format_for_si);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_dci_format_for_si
(
    U8  **pp_buffer,
    rrc_mac_dci_format_for_si_t *p_rrc_mac_dci_format_for_si
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_dci_format_for_si != PNULL);

    /* This function composes rrc_mac_dci_format_for_si */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DCI_FORMAT_FOR_SI;
    tlv_length = rrc_il_get_rrc_mac_dci_format_for_si_tlv_len(p_rrc_mac_dci_format_for_si);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_dci_format_for_si->dci_format_for_si_msgs > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dci_format_for_si->dci_format_for_si_msgs] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_dci_format_for_si->dci_format_for_si_msgs);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_dci_format_for_si->dci_format_for_si_msgs, "dci_format_for_si_msgs");
    *pp_buffer += sizeof(p_rrc_mac_dci_format_for_si->dci_format_for_si_msgs);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_mod_period_info_len
(
    rrc_mac_mod_period_info_t *p_rrc_mac_mod_period_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_mod_period_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_mod_period_info->mod_period);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_mod_period_info_tlv_len
(
    rrc_mac_mod_period_info_t *p_rrc_mac_mod_period_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_mod_period_info != PNULL);

    length += rrc_il_get_rrc_mac_mod_period_info_len(p_rrc_mac_mod_period_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_mod_period_info
(
    U8  **pp_buffer,
    rrc_mac_mod_period_info_t *p_rrc_mac_mod_period_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_mod_period_info != PNULL);

    /* This function composes rrc_mac_mod_period_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MOD_PERIOD_INFO;
    tlv_length = rrc_il_get_rrc_mac_mod_period_info_tlv_len(p_rrc_mac_mod_period_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_mod_period_info->mod_period < 64) || (p_rrc_mac_mod_period_info->mod_period > 4096))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_mod_period_info->mod_period] should be in range "
            "64 to 4096. Incorrect value %u received.", p_rrc_mac_mod_period_info->mod_period);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_mac_mod_period_info->mod_period, "mod_period");
    *pp_buffer += sizeof(p_rrc_mac_mod_period_info->mod_period);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sfn_gap_info_len
(
    rrc_mac_sfn_gap_info_t *p_rrc_mac_sfn_gap_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sfn_gap_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sfn_gap_info->sfn_gap);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sfn_gap_info_tlv_len
(
    rrc_mac_sfn_gap_info_t *p_rrc_mac_sfn_gap_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sfn_gap_info != PNULL);

    length += rrc_il_get_rrc_mac_sfn_gap_info_len(p_rrc_mac_sfn_gap_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sfn_gap_info
(
    U8  **pp_buffer,
    rrc_mac_sfn_gap_info_t *p_rrc_mac_sfn_gap_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sfn_gap_info != PNULL);

    /* This function composes rrc_mac_sfn_gap_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SFN_GAP_INFO;
    tlv_length = rrc_il_get_rrc_mac_sfn_gap_info_tlv_len(p_rrc_mac_sfn_gap_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_sfn_gap_info->sfn_gap < 1) || (p_rrc_mac_sfn_gap_info->sfn_gap > 32))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sfn_gap_info->sfn_gap] should be in range "
            "1 to 32. Incorrect value %u received.", p_rrc_mac_sfn_gap_info->sfn_gap);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sfn_gap_info->sfn_gap, "sfn_gap");
    *pp_buffer += sizeof(p_rrc_mac_sfn_gap_info->sfn_gap);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacEmtcSfn_len
(
    RrcMacEmtcSfn_t *p_RrcMacEmtcSfn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacEmtcSfn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacEmtcSfn->sfn);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacEmtcSfn_tlv_len
(
    RrcMacEmtcSfn_t *p_RrcMacEmtcSfn

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacEmtcSfn != PNULL);

    length += rrc_il_get_RrcMacEmtcSfn_len(p_RrcMacEmtcSfn);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacEmtcSfn
(
    U8  **pp_buffer,
    RrcMacEmtcSfn_t *p_RrcMacEmtcSfn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacEmtcSfn != PNULL);

    /* This function composes RrcMacEmtcSfn */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SFN_EMTC;
    tlv_length = rrc_il_get_RrcMacEmtcSfn_tlv_len(p_RrcMacEmtcSfn);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacEmtcSfn->sfn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacEmtcSfn->sfn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_RrcMacEmtcSfn->sfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_RrcMacEmtcSfn->sfn, "sfn");
    *pp_buffer += sizeof(p_RrcMacEmtcSfn->sfn);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacSfnGapInfoV1310_len
(
    RrcMacSfnGapInfoV1310_t *p_RrcMacSfnGapInfoV1310
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacSfnGapInfoV1310 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacSfnGapInfoV1310->sfnGapV1310);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacSfnGapInfoV1310_tlv_len
(
    RrcMacSfnGapInfoV1310_t *p_RrcMacSfnGapInfoV1310

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacSfnGapInfoV1310 != PNULL);

    length += rrc_il_get_RrcMacSfnGapInfoV1310_len(p_RrcMacSfnGapInfoV1310);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacSfnGapInfoV1310
(
    U8  **pp_buffer,
    RrcMacSfnGapInfoV1310_t *p_RrcMacSfnGapInfoV1310
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacSfnGapInfoV1310 != PNULL);

    /* This function composes RrcMacSfnGapInfoV1310 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SFN_GAP_INFO_V1310;
    tlv_length = rrc_il_get_RrcMacSfnGapInfoV1310_tlv_len(p_RrcMacSfnGapInfoV1310);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacSfnGapInfoV1310->sfnGapV1310 < 1) || (p_RrcMacSfnGapInfoV1310->sfnGapV1310 > EMTC_SFN_GAP_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacSfnGapInfoV1310->sfnGapV1310] should be in range "
            "1 to EMTC_SFN_GAP_MAX. Incorrect value %u received.", p_RrcMacSfnGapInfoV1310->sfnGapV1310);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_RrcMacSfnGapInfoV1310->sfnGapV1310, "sfnGapV1310");
    *pp_buffer += sizeof(p_RrcMacSfnGapInfoV1310->sfnGapV1310);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_reconfig_cell_cnf_len
(
    rrc_mac_reconfig_cell_cnf_t *p_rrc_mac_reconfig_cell_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reconfig_cell_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reconfig_cell_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_reconfig_cell_cnf
(
    U8  **pp_buffer,
    rrc_mac_reconfig_cell_cnf_t *p_rrc_mac_reconfig_cell_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_reconfig_cell_cnf != PNULL);

    /* This function composes rrc_mac_reconfig_cell_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfig_cell_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfig_cell_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_reconfig_cell_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_reconfig_cell_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_mac_reconfig_cell_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_reconfig_cell_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_RECONFIG_CELL_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_reconfig_cell_cnf
(
    rrc_mac_reconfig_cell_cnf_t  *p_rrc_mac_reconfig_cell_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_reconfig_cell_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_reconfig_cell_cnf_len(p_rrc_mac_reconfig_cell_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_RECONFIG_CELL_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_RECONFIG_CELL_CNF, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_RECONFIG_CELL_CNF, msg_length,cell_index);

    /* Fill RRC_MAC_RECONFIG_CELL_CNF message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_reconfig_cell_cnf(&p_mac_msg, p_rrc_mac_reconfig_cell_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_create_ue_entity_req_len
(
    rrc_mac_create_ue_entity_req_t *p_rrc_mac_create_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_create_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_create_ue_entity_req->ue_index);

    /* Get length of TLV */
    if (p_rrc_mac_create_ue_entity_req->optional_elems_present & RRC_MAC_ADD_UE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_add_ue_info_tlv_len(&p_rrc_mac_create_ue_entity_req->add_ue_info);
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_create_ue_entity_req->create_lc_req_counter <= ARRSIZE(p_rrc_mac_create_ue_entity_req->create_lc_req));
        for (loop = 0; loop < p_rrc_mac_create_ue_entity_req->create_lc_req_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_create_lc_req_tlv_len(&p_rrc_mac_create_ue_entity_req->create_lc_req[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_mac_create_ue_entity_req->optional_elems_present & RRC_MAC_ADD_RAT1_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_uplink_rat1_info_tlv_len(&p_rrc_mac_create_ue_entity_req->uplink_rat1_info);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_create_ue_entity_req
(
    U8  **pp_buffer,
    rrc_mac_create_ue_entity_req_t *p_rrc_mac_create_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_create_ue_entity_req != PNULL);

    /* This function composes rrc_mac_create_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_create_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_create_ue_entity_req->ue_index);

    /* Compose TLV  */
    if (p_rrc_mac_create_ue_entity_req->optional_elems_present & RRC_MAC_ADD_UE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_add_ue_info(pp_buffer, &p_rrc_mac_create_ue_entity_req->add_ue_info))
        {
            return RRC_FAILURE;
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_create_ue_entity_req->create_lc_req_counter <= ARRSIZE(p_rrc_mac_create_ue_entity_req->create_lc_req));
        for (loop = 0; loop < p_rrc_mac_create_ue_entity_req->create_lc_req_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_create_lc_req(pp_buffer, &p_rrc_mac_create_ue_entity_req->create_lc_req[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_create_ue_entity_req->optional_elems_present & RRC_MAC_ADD_RAT1_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_uplink_rat1_info(pp_buffer, &p_rrc_mac_create_ue_entity_req->uplink_rat1_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_create_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_CREATE_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_create_ue_entity_req
(
    rrc_mac_create_ue_entity_req_t  *p_rrc_mac_create_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_create_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_create_ue_entity_req_len(p_rrc_mac_create_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_CREATE_UE_ENTITY_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_CREATE_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_CREATE_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_MAC_CREATE_UE_ENTITY_REQ message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_create_ue_entity_req(&p_mac_msg, p_rrc_mac_create_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sr_setup_info_len
(
    rrc_mac_sr_setup_info_t *p_rrc_mac_sr_setup_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sr_setup_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sr_setup_info->sr_pucch_resource_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sr_setup_info->sr_configuration_index);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sr_setup_info_tlv_len
(
    rrc_mac_sr_setup_info_t *p_rrc_mac_sr_setup_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sr_setup_info != PNULL);

    length += rrc_il_get_rrc_mac_sr_setup_info_len(p_rrc_mac_sr_setup_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sr_setup_info
(
    U8  **pp_buffer,
    rrc_mac_sr_setup_info_t *p_rrc_mac_sr_setup_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sr_setup_info != PNULL);

    /* This function composes rrc_mac_sr_setup_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SR_SETUP_INFO;
    tlv_length = rrc_il_get_rrc_mac_sr_setup_info_tlv_len(p_rrc_mac_sr_setup_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sr_setup_info->sr_pucch_resource_index > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sr_setup_info->sr_pucch_resource_index] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_mac_sr_setup_info->sr_pucch_resource_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_sr_setup_info->sr_pucch_resource_index, "sr_pucch_resource_index");
    *pp_buffer += sizeof(p_rrc_mac_sr_setup_info->sr_pucch_resource_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sr_setup_info->sr_configuration_index > 157))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sr_setup_info->sr_configuration_index] should be less than"
            " or equal to 157. Incorrect value %u received.", p_rrc_mac_sr_setup_info->sr_configuration_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sr_setup_info->sr_configuration_index, "sr_configuration_index");
    *pp_buffer += sizeof(p_rrc_mac_sr_setup_info->sr_configuration_index);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sr_config_info_len
(
    rrc_mac_sr_config_info_t *p_rrc_mac_sr_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sr_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sr_config_info->request_type);

    /* Get length of TLV */
    if (p_rrc_mac_sr_config_info->optional_elems_present & RRC_MAC_SR_SETUP_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sr_setup_info_tlv_len(&p_rrc_mac_sr_config_info->sr_setup_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sr_config_info_tlv_len
(
    rrc_mac_sr_config_info_t *p_rrc_mac_sr_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sr_config_info != PNULL);

    length += rrc_il_get_rrc_mac_sr_config_info_len(p_rrc_mac_sr_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sr_config_info
(
    U8  **pp_buffer,
    rrc_mac_sr_config_info_t *p_rrc_mac_sr_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sr_config_info != PNULL);

    /* This function composes rrc_mac_sr_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SR_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_sr_config_info_tlv_len(p_rrc_mac_sr_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sr_config_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sr_config_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_sr_config_info->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sr_config_info->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_mac_sr_config_info->request_type);

    /* Compose TLV  */
    if (p_rrc_mac_sr_config_info->optional_elems_present & RRC_MAC_SR_SETUP_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sr_setup_info(pp_buffer, &p_rrc_mac_sr_config_info->sr_setup_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sr_prohbit_timer_value_v920_len
(
    rrc_mac_sr_prohbit_timer_value_v920_t *p_rrc_mac_sr_prohbit_timer_value_v920
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sr_prohbit_timer_value_v920 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sr_prohbit_timer_value_v920->sr_prohibit_timer);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sr_prohbit_timer_value_v920_tlv_len
(
    rrc_mac_sr_prohbit_timer_value_v920_t *p_rrc_mac_sr_prohbit_timer_value_v920

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sr_prohbit_timer_value_v920 != PNULL);

    length += rrc_il_get_rrc_mac_sr_prohbit_timer_value_v920_len(p_rrc_mac_sr_prohbit_timer_value_v920);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sr_prohbit_timer_value_v920
(
    U8  **pp_buffer,
    rrc_mac_sr_prohbit_timer_value_v920_t *p_rrc_mac_sr_prohbit_timer_value_v920
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sr_prohbit_timer_value_v920 != PNULL);

    /* This function composes rrc_mac_sr_prohbit_timer_value_v920 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SR_PROHBIT_TIMER_VALUE_V920;
    tlv_length = rrc_il_get_rrc_mac_sr_prohbit_timer_value_v920_tlv_len(p_rrc_mac_sr_prohbit_timer_value_v920);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sr_prohbit_timer_value_v920->sr_prohibit_timer > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sr_prohbit_timer_value_v920->sr_prohibit_timer] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_sr_prohbit_timer_value_v920->sr_prohibit_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sr_prohbit_timer_value_v920->sr_prohibit_timer, "sr_prohibit_timer");
    *pp_buffer += sizeof(p_rrc_mac_sr_prohbit_timer_value_v920->sr_prohibit_timer);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_aperiodic_csi_trigger_r10_len
(
    aperiodic_csi_trigger_r10_t *p_aperiodic_csi_trigger_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_aperiodic_csi_trigger_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_aperiodic_csi_trigger_r10->trigger1r10);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_aperiodic_csi_trigger_r10->trigger2r10);

    return length;
}

static
rrc_length_t
rrc_il_get_aperiodic_csi_trigger_r10_tlv_len
(
    aperiodic_csi_trigger_r10_t *p_aperiodic_csi_trigger_r10

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_aperiodic_csi_trigger_r10 != PNULL);

    length += rrc_il_get_aperiodic_csi_trigger_r10_len(p_aperiodic_csi_trigger_r10);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_aperiodic_csi_trigger_r10
(
    U8  **pp_buffer,
    aperiodic_csi_trigger_r10_t *p_aperiodic_csi_trigger_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_aperiodic_csi_trigger_r10 != PNULL);

    /* This function composes aperiodic_csi_trigger_r10 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_APERIODIC_CSI_TRIGGER_R10;
    tlv_length = rrc_il_get_aperiodic_csi_trigger_r10_tlv_len(p_aperiodic_csi_trigger_r10);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_aperiodic_csi_trigger_r10->trigger1r10 > 64))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_aperiodic_csi_trigger_r10->trigger1r10] should be less than"
            " or equal to 64. Incorrect value %u received.", p_aperiodic_csi_trigger_r10->trigger1r10);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_aperiodic_csi_trigger_r10->trigger1r10, "trigger1r10");
    *pp_buffer += sizeof(p_aperiodic_csi_trigger_r10->trigger1r10);

    /* Check for correct range [H - higher boundary] */
    if ((p_aperiodic_csi_trigger_r10->trigger2r10 > 192))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_aperiodic_csi_trigger_r10->trigger2r10] should be less than"
            " or equal to 192. Incorrect value %u received.", p_aperiodic_csi_trigger_r10->trigger2r10);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_aperiodic_csi_trigger_r10->trigger2r10, "trigger2r10");
    *pp_buffer += sizeof(p_aperiodic_csi_trigger_r10->trigger2r10);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_aperiodic_config_info_len
(
    rrc_mac_cqi_aperiodic_config_info_t *p_rrc_mac_cqi_aperiodic_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_aperiodic_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cqi_aperiodic_config_info->cqi_aperiodic_mode);

    /* Get length of TLV */
    if (p_rrc_mac_cqi_aperiodic_config_info->optional_elems_present & RRC_MAC_APERIODIC_CSI_TRIGGER_R10_PRESENT)
    {
        length += rrc_il_get_aperiodic_csi_trigger_r10_tlv_len(&p_rrc_mac_cqi_aperiodic_config_info->aperiodic_csi_trigger_r10);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_aperiodic_config_info_tlv_len
(
    rrc_mac_cqi_aperiodic_config_info_t *p_rrc_mac_cqi_aperiodic_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_aperiodic_config_info != PNULL);

    length += rrc_il_get_rrc_mac_cqi_aperiodic_config_info_len(p_rrc_mac_cqi_aperiodic_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_aperiodic_config_info
(
    U8  **pp_buffer,
    rrc_mac_cqi_aperiodic_config_info_t *p_rrc_mac_cqi_aperiodic_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_cqi_aperiodic_config_info != PNULL);

    /* This function composes rrc_mac_cqi_aperiodic_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CQI_APERIODIC_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_cqi_aperiodic_config_info_tlv_len(p_rrc_mac_cqi_aperiodic_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_cqi_aperiodic_config_info->cqi_aperiodic_mode < 12) || (p_rrc_mac_cqi_aperiodic_config_info->cqi_aperiodic_mode > 31))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_aperiodic_config_info->cqi_aperiodic_mode] should be in range "
            "12 to 31. Incorrect value %u received.", p_rrc_mac_cqi_aperiodic_config_info->cqi_aperiodic_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_cqi_aperiodic_config_info->cqi_aperiodic_mode, "cqi_aperiodic_mode");
    *pp_buffer += sizeof(p_rrc_mac_cqi_aperiodic_config_info->cqi_aperiodic_mode);

    /* Compose TLV  */
    if (p_rrc_mac_cqi_aperiodic_config_info->optional_elems_present & RRC_MAC_APERIODIC_CSI_TRIGGER_R10_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_aperiodic_csi_trigger_r10(pp_buffer, &p_rrc_mac_cqi_aperiodic_config_info->aperiodic_csi_trigger_r10))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_fmt_ind_perdiodic_subband_k_len
(
    rrc_mac_cqi_fmt_ind_perdiodic_subband_k_t *p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k->cqi_format_indicator_periodic_subband_k);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_fmt_ind_perdiodic_subband_k_tlv_len
(
    rrc_mac_cqi_fmt_ind_perdiodic_subband_k_t *p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k != PNULL);

    length += rrc_il_get_rrc_mac_cqi_fmt_ind_perdiodic_subband_k_len(p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_fmt_ind_perdiodic_subband_k
(
    U8  **pp_buffer,
    rrc_mac_cqi_fmt_ind_perdiodic_subband_k_t *p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k != PNULL);

    /* This function composes rrc_mac_cqi_fmt_ind_perdiodic_subband_k */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K;
    tlv_length = rrc_il_get_rrc_mac_cqi_fmt_ind_perdiodic_subband_k_tlv_len(p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k->cqi_format_indicator_periodic_subband_k > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k->cqi_format_indicator_periodic_subband_k] should be less than"
            " or equal to 4. Incorrect value %u received.", p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k->cqi_format_indicator_periodic_subband_k);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k->cqi_format_indicator_periodic_subband_k, "cqi_format_indicator_periodic_subband_k");
    *pp_buffer += sizeof(p_rrc_mac_cqi_fmt_ind_perdiodic_subband_k->cqi_format_indicator_periodic_subband_k);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ri_config_index_info_len
(
    rrc_mac_ri_config_index_info_t *p_rrc_mac_ri_config_index_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ri_config_index_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ri_config_index_info->ri_config_index);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ri_config_index_info_tlv_len
(
    rrc_mac_ri_config_index_info_t *p_rrc_mac_ri_config_index_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ri_config_index_info != PNULL);

    length += rrc_il_get_rrc_mac_ri_config_index_info_len(p_rrc_mac_ri_config_index_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ri_config_index_info
(
    U8  **pp_buffer,
    rrc_mac_ri_config_index_info_t *p_rrc_mac_ri_config_index_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ri_config_index_info != PNULL);

    /* This function composes rrc_mac_ri_config_index_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RI_CONFIG_INDEX_INFO;
    tlv_length = rrc_il_get_rrc_mac_ri_config_index_info_tlv_len(p_rrc_mac_ri_config_index_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ri_config_index_info->ri_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ri_config_index_info->ri_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_ri_config_index_info->ri_config_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ri_config_index_info->ri_config_index, "ri_config_index");
    *pp_buffer += sizeof(p_rrc_mac_ri_config_index_info->ri_config_index);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_pmi_config_index_info_len
(
    rrc_mac_cqi_pmi_config_index_info_t *p_rrc_mac_cqi_pmi_config_index_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_pmi_config_index_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cqi_pmi_config_index_info->cqi_pmi_config_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cqi_pmi_config_index_info->cqi_pucch_resource_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cqi_pmi_config_index_info->simultaneous_ack_nack_and_cqi);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic);

    /* Get length of TLV */
    if (p_rrc_mac_cqi_pmi_config_index_info->optional_elems_present & RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K_PRESENT)
    {
        length += rrc_il_get_rrc_mac_cqi_fmt_ind_perdiodic_subband_k_tlv_len(&p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic_subband_k);
    }

    /* Get length of TLV */
    if (p_rrc_mac_cqi_pmi_config_index_info->optional_elems_present & RRC_MAC_RI_CONFIG_INDEX_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ri_config_index_info_tlv_len(&p_rrc_mac_cqi_pmi_config_index_info->ri_config_index_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_pmi_config_index_info_tlv_len
(
    rrc_mac_cqi_pmi_config_index_info_t *p_rrc_mac_cqi_pmi_config_index_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_pmi_config_index_info != PNULL);

    length += rrc_il_get_rrc_mac_cqi_pmi_config_index_info_len(p_rrc_mac_cqi_pmi_config_index_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_pmi_config_index_info
(
    U8  **pp_buffer,
    rrc_mac_cqi_pmi_config_index_info_t *p_rrc_mac_cqi_pmi_config_index_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_cqi_pmi_config_index_info != PNULL);

    /* This function composes rrc_mac_cqi_pmi_config_index_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO;
    tlv_length = rrc_il_get_rrc_mac_cqi_pmi_config_index_info_tlv_len(p_rrc_mac_cqi_pmi_config_index_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info->cqi_pmi_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info->cqi_pmi_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info->cqi_pmi_config_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info->cqi_pmi_config_index, "cqi_pmi_config_index");
    *pp_buffer += sizeof(p_rrc_mac_cqi_pmi_config_index_info->cqi_pmi_config_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info->cqi_pucch_resource_index > 1185))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info->cqi_pucch_resource_index] should be less than"
            " or equal to 1185. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info->cqi_pucch_resource_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info->cqi_pucch_resource_index, "cqi_pucch_resource_index");
    *pp_buffer += sizeof(p_rrc_mac_cqi_pmi_config_index_info->cqi_pucch_resource_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info->simultaneous_ack_nack_and_cqi > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info->simultaneous_ack_nack_and_cqi] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info->simultaneous_ack_nack_and_cqi);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info->simultaneous_ack_nack_and_cqi, "simultaneous_ack_nack_and_cqi");
    *pp_buffer += sizeof(p_rrc_mac_cqi_pmi_config_index_info->simultaneous_ack_nack_and_cqi);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic, "cqi_fmt_ind_periodic");
    *pp_buffer += sizeof(p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic);

    /* Compose TLV  */
    if (p_rrc_mac_cqi_pmi_config_index_info->optional_elems_present & RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_cqi_fmt_ind_perdiodic_subband_k(pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info->cqi_fmt_ind_periodic_subband_k))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_cqi_pmi_config_index_info->optional_elems_present & RRC_MAC_RI_CONFIG_INDEX_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ri_config_index_info(pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info->ri_config_index_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_cqi_pmi_config_index2_r10_len
(
    cqi_pmi_config_index2_r10_t *p_cqi_pmi_config_index2_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_cqi_pmi_config_index2_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_cqi_pmi_config_index2_r10->cqi_pmi_config_index2);

    return length;
}

static
rrc_length_t
rrc_il_get_cqi_pmi_config_index2_r10_tlv_len
(
    cqi_pmi_config_index2_r10_t *p_cqi_pmi_config_index2_r10

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_cqi_pmi_config_index2_r10 != PNULL);

    length += rrc_il_get_cqi_pmi_config_index2_r10_len(p_cqi_pmi_config_index2_r10);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_cqi_pmi_config_index2_r10
(
    U8  **pp_buffer,
    cqi_pmi_config_index2_r10_t *p_cqi_pmi_config_index2_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_cqi_pmi_config_index2_r10 != PNULL);

    /* This function composes cqi_pmi_config_index2_r10 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CQI_PMI_CONFIG_INDEX2_R10;
    tlv_length = rrc_il_get_cqi_pmi_config_index2_r10_tlv_len(p_cqi_pmi_config_index2_r10);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_cqi_pmi_config_index2_r10->cqi_pmi_config_index2 > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_cqi_pmi_config_index2_r10->cqi_pmi_config_index2] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_cqi_pmi_config_index2_r10->cqi_pmi_config_index2);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_cqi_pmi_config_index2_r10->cqi_pmi_config_index2, "cqi_pmi_config_index2");
    *pp_buffer += sizeof(p_cqi_pmi_config_index2_r10->cqi_pmi_config_index2);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_csi_config_index_v10_len
(
    rrc_mac_csi_config_index_v10_t *p_rrc_mac_csi_config_index_v10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_csi_config_index_v10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_csi_config_index_v10->request_type);

    /* Get length of TLV */
    if (p_rrc_mac_csi_config_index_v10->optional_elems_present & RRC_MAC_CQI_PMI_CONFIG_INDEX2_R10_PRESENT)
    {
        length += rrc_il_get_cqi_pmi_config_index2_r10_tlv_len(&p_rrc_mac_csi_config_index_v10->cqi_pmi_config_index2_r10);
    }

    /* Get length of TLV */
    if (p_rrc_mac_csi_config_index_v10->optional_elems_present & RRC_MAC_RI_CONFIG_INDEX2_R10_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ri_config_index_info_tlv_len(&p_rrc_mac_csi_config_index_v10->ri_config_index2_r_10);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_csi_config_index_v10_tlv_len
(
    rrc_mac_csi_config_index_v10_t *p_rrc_mac_csi_config_index_v10

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_csi_config_index_v10 != PNULL);

    length += rrc_il_get_rrc_mac_csi_config_index_v10_len(p_rrc_mac_csi_config_index_v10);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_csi_config_index_v10
(
    U8  **pp_buffer,
    rrc_mac_csi_config_index_v10_t *p_rrc_mac_csi_config_index_v10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_csi_config_index_v10 != PNULL);

    /* This function composes rrc_mac_csi_config_index_v10 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CSI_CONFIG_INDEX_V10;
    tlv_length = rrc_il_get_rrc_mac_csi_config_index_v10_tlv_len(p_rrc_mac_csi_config_index_v10);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_csi_config_index_v10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_csi_config_index_v10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_csi_config_index_v10->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_csi_config_index_v10->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_mac_csi_config_index_v10->request_type);

    /* Compose TLV  */
    if (p_rrc_mac_csi_config_index_v10->optional_elems_present & RRC_MAC_CQI_PMI_CONFIG_INDEX2_R10_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_cqi_pmi_config_index2_r10(pp_buffer, &p_rrc_mac_csi_config_index_v10->cqi_pmi_config_index2_r10))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_csi_config_index_v10->optional_elems_present & RRC_MAC_RI_CONFIG_INDEX2_R10_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ri_config_index_info(pp_buffer, &p_rrc_mac_csi_config_index_v10->ri_config_index2_r_10))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_pmi_config_index_info_v1020_len
(
    rrc_mac_cqi_pmi_config_index_info_v1020_t *p_rrc_mac_cqi_pmi_config_index_info_v1020
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_pmi_config_index_info_v1020 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pmi_config_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pucch_resource_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cqi_pmi_config_index_info_v1020->simultaneous_ack_nack_and_cqi);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic);

    /* Get length of TLV */
    if (p_rrc_mac_cqi_pmi_config_index_info_v1020->optional_elems_present & RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K_PRESENT)
    {
        length += rrc_il_get_rrc_mac_cqi_fmt_ind_perdiodic_subband_k_tlv_len(&p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic_subband_k);
    }

    /* Get length of TLV */
    if (p_rrc_mac_cqi_pmi_config_index_info_v1020->optional_elems_present & RRC_MAC_RI_CONFIG_INDEX_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ri_config_index_info_tlv_len(&p_rrc_mac_cqi_pmi_config_index_info_v1020->ri_config_index_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_cqi_pmi_config_index_info_v1020->optional_elems_present & RRC_MAC_CSI_CONFIG_INDEX_V10_PRESENT)
    {
        length += rrc_il_get_rrc_mac_csi_config_index_v10_tlv_len(&p_rrc_mac_cqi_pmi_config_index_info_v1020->csi_config_index_v10);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_pmi_config_index_info_v1020_tlv_len
(
    rrc_mac_cqi_pmi_config_index_info_v1020_t *p_rrc_mac_cqi_pmi_config_index_info_v1020

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_pmi_config_index_info_v1020 != PNULL);

    length += rrc_il_get_rrc_mac_cqi_pmi_config_index_info_v1020_len(p_rrc_mac_cqi_pmi_config_index_info_v1020);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_pmi_config_index_info_v1020
(
    U8  **pp_buffer,
    rrc_mac_cqi_pmi_config_index_info_v1020_t *p_rrc_mac_cqi_pmi_config_index_info_v1020
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_cqi_pmi_config_index_info_v1020 != PNULL);

    /* This function composes rrc_mac_cqi_pmi_config_index_info_v1020 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_V1020;
    tlv_length = rrc_il_get_rrc_mac_cqi_pmi_config_index_info_v1020_tlv_len(p_rrc_mac_cqi_pmi_config_index_info_v1020);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pmi_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pmi_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pmi_config_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pmi_config_index, "cqi_pmi_config_index");
    *pp_buffer += sizeof(p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pmi_config_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pucch_resource_index > 1185))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pucch_resource_index] should be less than"
            " or equal to 1185. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pucch_resource_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pucch_resource_index, "cqi_pucch_resource_index");
    *pp_buffer += sizeof(p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_pucch_resource_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info_v1020->simultaneous_ack_nack_and_cqi > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info_v1020->simultaneous_ack_nack_and_cqi] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info_v1020->simultaneous_ack_nack_and_cqi);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info_v1020->simultaneous_ack_nack_and_cqi, "simultaneous_ack_nack_and_cqi");
    *pp_buffer += sizeof(p_rrc_mac_cqi_pmi_config_index_info_v1020->simultaneous_ack_nack_and_cqi);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic, "cqi_fmt_ind_periodic");
    *pp_buffer += sizeof(p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic);

    /* Compose TLV  */
    if (p_rrc_mac_cqi_pmi_config_index_info_v1020->optional_elems_present & RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_cqi_fmt_ind_perdiodic_subband_k(pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info_v1020->cqi_fmt_ind_periodic_subband_k))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_cqi_pmi_config_index_info_v1020->optional_elems_present & RRC_MAC_RI_CONFIG_INDEX_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ri_config_index_info(pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info_v1020->ri_config_index_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_cqi_pmi_config_index_info_v1020->optional_elems_present & RRC_MAC_CSI_CONFIG_INDEX_V10_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_csi_config_index_v10(pp_buffer, &p_rrc_mac_cqi_pmi_config_index_info_v1020->csi_config_index_v10))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_periodic_config_info_len
(
    rrc_mac_cqi_periodic_config_info_t *p_rrc_mac_cqi_periodic_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_periodic_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cqi_periodic_config_info->request_type);

    /* Get length of TLV */
    if (p_rrc_mac_cqi_periodic_config_info->optional_elems_present & RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_cqi_pmi_config_index_info_tlv_len(&p_rrc_mac_cqi_periodic_config_info->cqi_pmi_config_index_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_cqi_periodic_config_info->optional_elems_present & RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_V1020_PRESENT)
    {
        length += rrc_il_get_rrc_mac_cqi_pmi_config_index_info_v1020_tlv_len(&p_rrc_mac_cqi_periodic_config_info->cqi_pmi_config_index_info_v1020);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_periodic_config_info_tlv_len
(
    rrc_mac_cqi_periodic_config_info_t *p_rrc_mac_cqi_periodic_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_periodic_config_info != PNULL);

    length += rrc_il_get_rrc_mac_cqi_periodic_config_info_len(p_rrc_mac_cqi_periodic_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_periodic_config_info
(
    U8  **pp_buffer,
    rrc_mac_cqi_periodic_config_info_t *p_rrc_mac_cqi_periodic_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_cqi_periodic_config_info != PNULL);

    /* This function composes rrc_mac_cqi_periodic_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CQI_PERIODIC_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_cqi_periodic_config_info_tlv_len(p_rrc_mac_cqi_periodic_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_periodic_config_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_periodic_config_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cqi_periodic_config_info->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_cqi_periodic_config_info->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_mac_cqi_periodic_config_info->request_type);

    /* Compose TLV  */
    if (p_rrc_mac_cqi_periodic_config_info->optional_elems_present & RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_cqi_pmi_config_index_info(pp_buffer, &p_rrc_mac_cqi_periodic_config_info->cqi_pmi_config_index_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_cqi_periodic_config_info->optional_elems_present & RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_V1020_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_cqi_pmi_config_index_info_v1020(pp_buffer, &p_rrc_mac_cqi_periodic_config_info->cqi_pmi_config_index_info_v1020))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_mask_r9_len
(
    rrc_mac_cqi_mask_r9_t *p_rrc_mac_cqi_mask_r9
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_mask_r9 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cqi_mask_r9->cqi_mask_r9);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_mask_r9_tlv_len
(
    rrc_mac_cqi_mask_r9_t *p_rrc_mac_cqi_mask_r9

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_mask_r9 != PNULL);

    length += rrc_il_get_rrc_mac_cqi_mask_r9_len(p_rrc_mac_cqi_mask_r9);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_mask_r9
(
    U8  **pp_buffer,
    rrc_mac_cqi_mask_r9_t *p_rrc_mac_cqi_mask_r9
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_cqi_mask_r9 != PNULL);

    /* This function composes rrc_mac_cqi_mask_r9 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CQI_REPORT_CONFIG_CQI_MASK_R9;
    tlv_length = rrc_il_get_rrc_mac_cqi_mask_r9_tlv_len(p_rrc_mac_cqi_mask_r9);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cqi_mask_r9->cqi_mask_r9 > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cqi_mask_r9->cqi_mask_r9] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cqi_mask_r9->cqi_mask_r9);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_cqi_mask_r9->cqi_mask_r9, "cqi_mask_r9");
    *pp_buffer += sizeof(p_rrc_mac_cqi_mask_r9->cqi_mask_r9);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pmi_ri_report_r9_len
(
    rrc_mac_pmi_ri_report_r9_t *p_rrc_mac_pmi_ri_report_r9
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pmi_ri_report_r9 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pmi_ri_report_r9->pmi_ri_report_r9);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pmi_ri_report_r9_tlv_len
(
    rrc_mac_pmi_ri_report_r9_t *p_rrc_mac_pmi_ri_report_r9

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pmi_ri_report_r9 != PNULL);

    length += rrc_il_get_rrc_mac_pmi_ri_report_r9_len(p_rrc_mac_pmi_ri_report_r9);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_pmi_ri_report_r9
(
    U8  **pp_buffer,
    rrc_mac_pmi_ri_report_r9_t *p_rrc_mac_pmi_ri_report_r9
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_pmi_ri_report_r9 != PNULL);

    /* This function composes rrc_mac_pmi_ri_report_r9 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CQI_REPORT_CONFIG_PMI_RI_REPORT_R9;
    tlv_length = rrc_il_get_rrc_mac_pmi_ri_report_r9_tlv_len(p_rrc_mac_pmi_ri_report_r9);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pmi_ri_report_r9->pmi_ri_report_r9 > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pmi_ri_report_r9->pmi_ri_report_r9] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_pmi_ri_report_r9->pmi_ri_report_r9);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pmi_ri_report_r9->pmi_ri_report_r9, "pmi_ri_report_r9");
    *pp_buffer += sizeof(p_rrc_mac_pmi_ri_report_r9->pmi_ri_report_r9);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_report_config_v920_len
(
    rrc_mac_cqi_report_config_v920_t *p_rrc_mac_cqi_report_config_v920
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_report_config_v920 != PNULL);

    /* Get length of TLV */
    if (p_rrc_mac_cqi_report_config_v920->optional_elems_present & RRC_MAC_CQI_REPORT_CONFIG_CQI_MASK_R9_PRESENT)
    {
        length += rrc_il_get_rrc_mac_cqi_mask_r9_tlv_len(&p_rrc_mac_cqi_report_config_v920->rrc_mac_cqi_mask_r9);
    }

    /* Get length of TLV */
    if (p_rrc_mac_cqi_report_config_v920->optional_elems_present & RRC_MAC_CQI_REPORT_CONFIG_PMI_RI_REPORT_R9_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pmi_ri_report_r9_tlv_len(&p_rrc_mac_cqi_report_config_v920->rrc_mac_pmi_ri_report_r9);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_report_config_v920_tlv_len
(
    rrc_mac_cqi_report_config_v920_t *p_rrc_mac_cqi_report_config_v920

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_report_config_v920 != PNULL);

    length += rrc_il_get_rrc_mac_cqi_report_config_v920_len(p_rrc_mac_cqi_report_config_v920);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_report_config_v920
(
    U8  **pp_buffer,
    rrc_mac_cqi_report_config_v920_t *p_rrc_mac_cqi_report_config_v920
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_cqi_report_config_v920 != PNULL);

    /* This function composes rrc_mac_cqi_report_config_v920 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CQI_REPORT_CONFIG_V920;
    tlv_length = rrc_il_get_rrc_mac_cqi_report_config_v920_tlv_len(p_rrc_mac_cqi_report_config_v920);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose TLV  */
    if (p_rrc_mac_cqi_report_config_v920->optional_elems_present & RRC_MAC_CQI_REPORT_CONFIG_CQI_MASK_R9_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_cqi_mask_r9(pp_buffer, &p_rrc_mac_cqi_report_config_v920->rrc_mac_cqi_mask_r9))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_cqi_report_config_v920->optional_elems_present & RRC_MAC_CQI_REPORT_CONFIG_PMI_RI_REPORT_R9_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pmi_ri_report_r9(pp_buffer, &p_rrc_mac_cqi_report_config_v920->rrc_mac_pmi_ri_report_r9))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_csi_meas_subframe_set1_r10_len
(
    rrc_mac_csi_meas_subframe_set1_r10_t *p_rrc_mac_csi_meas_subframe_set1_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_csi_meas_subframe_set1_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_csi_meas_subframe_set1_r10->choice);

    /* Add buffer size */
    length += p_rrc_mac_csi_meas_subframe_set1_r10->count;

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_csi_meas_subframe_set1_r10_tlv_len
(
    rrc_mac_csi_meas_subframe_set1_r10_t *p_rrc_mac_csi_meas_subframe_set1_r10

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_csi_meas_subframe_set1_r10 != PNULL);

    length += rrc_il_get_rrc_mac_csi_meas_subframe_set1_r10_len(p_rrc_mac_csi_meas_subframe_set1_r10);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_csi_meas_subframe_set1_r10
(
    U8  **pp_buffer,
    rrc_mac_csi_meas_subframe_set1_r10_t *p_rrc_mac_csi_meas_subframe_set1_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_csi_meas_subframe_set1_r10 != PNULL);

    /* This function composes rrc_mac_csi_meas_subframe_set1_r10 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CSI_MEAS_SUBFRAME_SET1_R10;
    tlv_length = rrc_il_get_rrc_mac_csi_meas_subframe_set1_r10_tlv_len(p_rrc_mac_csi_meas_subframe_set1_r10);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_csi_meas_subframe_set1_r10->choice > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_csi_meas_subframe_set1_r10->choice] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_csi_meas_subframe_set1_r10->choice);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_csi_meas_subframe_set1_r10->choice, "choice");
    *pp_buffer += sizeof(p_rrc_mac_csi_meas_subframe_set1_r10->choice);

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        if(ARRSIZE(p_rrc_mac_csi_meas_subframe_set1_r10->csi_measurement_subset) < p_rrc_mac_csi_meas_subframe_set1_r10->count)
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_mac_csi_meas_subframe_set1_r10->count = %u is exceeding p_rrc_mac_csi_meas_subframe_set1_r10->csi_measurement_subset array boundary", p_rrc_mac_csi_meas_subframe_set1_r10->count);
            return RRC_FAILURE;
        }
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_mac_csi_meas_subframe_set1_r10->count; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_csi_meas_subframe_set1_r10->csi_measurement_subset[loop], "csi_measurement_subset[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_csi_meas_subframe_set2_r10_len
(
    rrc_mac_csi_meas_subframe_set2_r10_t *p_rrc_mac_csi_meas_subframe_set2_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_csi_meas_subframe_set2_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_csi_meas_subframe_set2_r10->choice);

    /* Add buffer size */
    length += p_rrc_mac_csi_meas_subframe_set2_r10->count;

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_csi_meas_subframe_set2_r10_tlv_len
(
    rrc_mac_csi_meas_subframe_set2_r10_t *p_rrc_mac_csi_meas_subframe_set2_r10

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_csi_meas_subframe_set2_r10 != PNULL);

    length += rrc_il_get_rrc_mac_csi_meas_subframe_set2_r10_len(p_rrc_mac_csi_meas_subframe_set2_r10);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_csi_meas_subframe_set2_r10
(
    U8  **pp_buffer,
    rrc_mac_csi_meas_subframe_set2_r10_t *p_rrc_mac_csi_meas_subframe_set2_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_csi_meas_subframe_set2_r10 != PNULL);

    /* This function composes rrc_mac_csi_meas_subframe_set2_r10 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CSI_MEAS_SUBFRAME_SET2_R10;
    tlv_length = rrc_il_get_rrc_mac_csi_meas_subframe_set2_r10_tlv_len(p_rrc_mac_csi_meas_subframe_set2_r10);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_csi_meas_subframe_set2_r10->choice > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_csi_meas_subframe_set2_r10->choice] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_csi_meas_subframe_set2_r10->choice);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_csi_meas_subframe_set2_r10->choice, "choice");
    *pp_buffer += sizeof(p_rrc_mac_csi_meas_subframe_set2_r10->choice);

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        if(ARRSIZE(p_rrc_mac_csi_meas_subframe_set2_r10->csi_measurement_subset) < p_rrc_mac_csi_meas_subframe_set2_r10->count)
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_mac_csi_meas_subframe_set2_r10->count = %u is exceeding p_rrc_mac_csi_meas_subframe_set2_r10->csi_measurement_subset array boundary", p_rrc_mac_csi_meas_subframe_set2_r10->count);
            return RRC_FAILURE;
        }
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_mac_csi_meas_subframe_set2_r10->count; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_csi_meas_subframe_set2_r10->csi_measurement_subset[loop], "csi_measurement_subset[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_csi_subframe_pattern_config_v10_len
(
    rrc_mac_csi_subframe_pattern_config_v10_t *p_rrc_mac_csi_subframe_pattern_config_v10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_csi_subframe_pattern_config_v10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_csi_subframe_pattern_config_v10->request_type);

    /* Get length of TLV */
    if (p_rrc_mac_csi_subframe_pattern_config_v10->optional_elems_present & RRC_MAC_CSI_MEAS_SUBFRAME_SET1_R10_PRESENT)
    {
        length += rrc_il_get_rrc_mac_csi_meas_subframe_set1_r10_tlv_len(&p_rrc_mac_csi_subframe_pattern_config_v10->rrc_mac_csi_meas_subframe_set1_r10);
    }

    /* Get length of TLV */
    if (p_rrc_mac_csi_subframe_pattern_config_v10->optional_elems_present & RRC_MAC_CSI_MEAS_SUBFRAME_SET2_R10_PRESENT)
    {
        length += rrc_il_get_rrc_mac_csi_meas_subframe_set2_r10_tlv_len(&p_rrc_mac_csi_subframe_pattern_config_v10->rrc_mac_csi_meas_subframe_set2_r10);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_csi_subframe_pattern_config_v10_tlv_len
(
    rrc_mac_csi_subframe_pattern_config_v10_t *p_rrc_mac_csi_subframe_pattern_config_v10

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_csi_subframe_pattern_config_v10 != PNULL);

    length += rrc_il_get_rrc_mac_csi_subframe_pattern_config_v10_len(p_rrc_mac_csi_subframe_pattern_config_v10);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_csi_subframe_pattern_config_v10
(
    U8  **pp_buffer,
    rrc_mac_csi_subframe_pattern_config_v10_t *p_rrc_mac_csi_subframe_pattern_config_v10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_csi_subframe_pattern_config_v10 != PNULL);

    /* This function composes rrc_mac_csi_subframe_pattern_config_v10 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CQI_CSI_SUBFRAME_PATTERN_CONFIG_V10;
    tlv_length = rrc_il_get_rrc_mac_csi_subframe_pattern_config_v10_tlv_len(p_rrc_mac_csi_subframe_pattern_config_v10);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_csi_subframe_pattern_config_v10->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_csi_subframe_pattern_config_v10->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_csi_subframe_pattern_config_v10->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_csi_subframe_pattern_config_v10->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_mac_csi_subframe_pattern_config_v10->request_type);

    /* Compose TLV  */
    if (p_rrc_mac_csi_subframe_pattern_config_v10->optional_elems_present & RRC_MAC_CSI_MEAS_SUBFRAME_SET1_R10_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_csi_meas_subframe_set1_r10(pp_buffer, &p_rrc_mac_csi_subframe_pattern_config_v10->rrc_mac_csi_meas_subframe_set1_r10))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_csi_subframe_pattern_config_v10->optional_elems_present & RRC_MAC_CSI_MEAS_SUBFRAME_SET2_R10_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_csi_meas_subframe_set2_r10(pp_buffer, &p_rrc_mac_csi_subframe_pattern_config_v10->rrc_mac_csi_meas_subframe_set2_r10))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_info_len
(
    rrc_mac_cqi_info_t *p_rrc_mac_cqi_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_info != PNULL);

    /* Get length of TLV */
    if (p_rrc_mac_cqi_info->optional_elems_present & RRC_MAC_CQI_APERIODIC_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_cqi_aperiodic_config_info_tlv_len(&p_rrc_mac_cqi_info->cqi_aperiodic_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_cqi_info->optional_elems_present & RRC_MAC_CQI_PERIODIC_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_cqi_periodic_config_info_tlv_len(&p_rrc_mac_cqi_info->cqi_periodic_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_cqi_info->optional_elems_present & RRC_MAC_CQI_REPORT_CONFIG_V920_PRESENT)
    {
        length += rrc_il_get_rrc_mac_cqi_report_config_v920_tlv_len(&p_rrc_mac_cqi_info->cqi_report_config_v920);
    }

    /* Get length of TLV */
    if (p_rrc_mac_cqi_info->optional_elems_present & RRC_MAC_CQI_CSI_SUBFRAME_PATTERN_CONFIG_V10_PRESENT)
    {
        length += rrc_il_get_rrc_mac_csi_subframe_pattern_config_v10_tlv_len(&p_rrc_mac_cqi_info->rrc_mac_csi_subframe_pattern_config_v10);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cqi_info_tlv_len
(
    rrc_mac_cqi_info_t *p_rrc_mac_cqi_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cqi_info != PNULL);

    length += rrc_il_get_rrc_mac_cqi_info_len(p_rrc_mac_cqi_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_cqi_info
(
    U8  **pp_buffer,
    rrc_mac_cqi_info_t *p_rrc_mac_cqi_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_cqi_info != PNULL);

    /* This function composes rrc_mac_cqi_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CQI_INFO;
    tlv_length = rrc_il_get_rrc_mac_cqi_info_tlv_len(p_rrc_mac_cqi_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose TLV  */
    if (p_rrc_mac_cqi_info->optional_elems_present & RRC_MAC_CQI_APERIODIC_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_cqi_aperiodic_config_info(pp_buffer, &p_rrc_mac_cqi_info->cqi_aperiodic_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_cqi_info->optional_elems_present & RRC_MAC_CQI_PERIODIC_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_cqi_periodic_config_info(pp_buffer, &p_rrc_mac_cqi_info->cqi_periodic_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_cqi_info->optional_elems_present & RRC_MAC_CQI_REPORT_CONFIG_V920_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_cqi_report_config_v920(pp_buffer, &p_rrc_mac_cqi_info->cqi_report_config_v920))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_cqi_info->optional_elems_present & RRC_MAC_CQI_CSI_SUBFRAME_PATTERN_CONFIG_V10_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_csi_subframe_pattern_config_v10(pp_buffer, &p_rrc_mac_cqi_info->rrc_mac_csi_subframe_pattern_config_v10))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tdd_ack_nack_feedback_mode_len
(
    rrc_mac_tdd_ack_nack_feedback_mode_t *p_rrc_mac_tdd_ack_nack_feedback_mode
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tdd_ack_nack_feedback_mode != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tdd_ack_nack_feedback_mode->tdd_ack_nack_feedback_mode);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tdd_ack_nack_feedback_mode_tlv_len
(
    rrc_mac_tdd_ack_nack_feedback_mode_t *p_rrc_mac_tdd_ack_nack_feedback_mode

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tdd_ack_nack_feedback_mode != PNULL);

    length += rrc_il_get_rrc_mac_tdd_ack_nack_feedback_mode_len(p_rrc_mac_tdd_ack_nack_feedback_mode);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_tdd_ack_nack_feedback_mode
(
    U8  **pp_buffer,
    rrc_mac_tdd_ack_nack_feedback_mode_t *p_rrc_mac_tdd_ack_nack_feedback_mode
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_tdd_ack_nack_feedback_mode != PNULL);

    /* This function composes rrc_mac_tdd_ack_nack_feedback_mode */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_TDD_ACK_NACK_FEEDBACK_MODE;
    tlv_length = rrc_il_get_rrc_mac_tdd_ack_nack_feedback_mode_tlv_len(p_rrc_mac_tdd_ack_nack_feedback_mode);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_tdd_ack_nack_feedback_mode->tdd_ack_nack_feedback_mode > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tdd_ack_nack_feedback_mode->tdd_ack_nack_feedback_mode] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_tdd_ack_nack_feedback_mode->tdd_ack_nack_feedback_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_tdd_ack_nack_feedback_mode->tdd_ack_nack_feedback_mode, "tdd_ack_nack_feedback_mode");
    *pp_buffer += sizeof(p_rrc_mac_tdd_ack_nack_feedback_mode->tdd_ack_nack_feedback_mode);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pusch_config_info_len
(
    rrc_mac_pusch_config_info_t *p_rrc_mac_pusch_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pusch_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pusch_config_info->beta_offset_ack_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pusch_config_info->beta_offset_ri_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pusch_config_info->beta_offset_cqi_index);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pusch_config_info_tlv_len
(
    rrc_mac_pusch_config_info_t *p_rrc_mac_pusch_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pusch_config_info != PNULL);

    length += rrc_il_get_rrc_mac_pusch_config_info_len(p_rrc_mac_pusch_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_pusch_config_info
(
    U8  **pp_buffer,
    rrc_mac_pusch_config_info_t *p_rrc_mac_pusch_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_pusch_config_info != PNULL);

    /* This function composes rrc_mac_pusch_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PUSCH_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_pusch_config_info_tlv_len(p_rrc_mac_pusch_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pusch_config_info->beta_offset_ack_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pusch_config_info->beta_offset_ack_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_pusch_config_info->beta_offset_ack_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pusch_config_info->beta_offset_ack_index, "beta_offset_ack_index");
    *pp_buffer += sizeof(p_rrc_mac_pusch_config_info->beta_offset_ack_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pusch_config_info->beta_offset_ri_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pusch_config_info->beta_offset_ri_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_pusch_config_info->beta_offset_ri_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pusch_config_info->beta_offset_ri_index, "beta_offset_ri_index");
    *pp_buffer += sizeof(p_rrc_mac_pusch_config_info->beta_offset_ri_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pusch_config_info->beta_offset_cqi_index > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pusch_config_info->beta_offset_cqi_index] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_pusch_config_info->beta_offset_cqi_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pusch_config_info->beta_offset_cqi_index, "beta_offset_cqi_index");
    *pp_buffer += sizeof(p_rrc_mac_pusch_config_info->beta_offset_cqi_index);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_srs_dedicated_setup_info_len
(
    rrc_mac_srs_dedicated_setup_info_t *p_rrc_mac_srs_dedicated_setup_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_srs_dedicated_setup_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_dedicated_setup_info->srs_bandwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_dedicated_setup_info->srs_hopping_bandwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_dedicated_setup_info->freq_domain_position);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_dedicated_setup_info->duration);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_dedicated_setup_info->srs_config_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_dedicated_setup_info->transmission_comb);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_dedicated_setup_info->cyclic_shift);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_srs_dedicated_setup_info_tlv_len
(
    rrc_mac_srs_dedicated_setup_info_t *p_rrc_mac_srs_dedicated_setup_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_srs_dedicated_setup_info != PNULL);

    length += rrc_il_get_rrc_mac_srs_dedicated_setup_info_len(p_rrc_mac_srs_dedicated_setup_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_srs_dedicated_setup_info
(
    U8  **pp_buffer,
    rrc_mac_srs_dedicated_setup_info_t *p_rrc_mac_srs_dedicated_setup_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_srs_dedicated_setup_info != PNULL);

    /* This function composes rrc_mac_srs_dedicated_setup_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SRS_DEDICATED_SETUP_INFO;
    tlv_length = rrc_il_get_rrc_mac_srs_dedicated_setup_info_tlv_len(p_rrc_mac_srs_dedicated_setup_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->srs_bandwidth > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->srs_bandwidth] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->srs_bandwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_srs_dedicated_setup_info->srs_bandwidth, "srs_bandwidth");
    *pp_buffer += sizeof(p_rrc_mac_srs_dedicated_setup_info->srs_bandwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->srs_hopping_bandwidth > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->srs_hopping_bandwidth] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->srs_hopping_bandwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_srs_dedicated_setup_info->srs_hopping_bandwidth, "srs_hopping_bandwidth");
    *pp_buffer += sizeof(p_rrc_mac_srs_dedicated_setup_info->srs_hopping_bandwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->freq_domain_position > 23))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->freq_domain_position] should be less than"
            " or equal to 23. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->freq_domain_position);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_srs_dedicated_setup_info->freq_domain_position, "freq_domain_position");
    *pp_buffer += sizeof(p_rrc_mac_srs_dedicated_setup_info->freq_domain_position);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->duration > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->duration] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->duration);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_srs_dedicated_setup_info->duration, "duration");
    *pp_buffer += sizeof(p_rrc_mac_srs_dedicated_setup_info->duration);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->srs_config_index > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->srs_config_index] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->srs_config_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_srs_dedicated_setup_info->srs_config_index, "srs_config_index");
    *pp_buffer += sizeof(p_rrc_mac_srs_dedicated_setup_info->srs_config_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->transmission_comb > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->transmission_comb] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->transmission_comb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_srs_dedicated_setup_info->transmission_comb, "transmission_comb");
    *pp_buffer += sizeof(p_rrc_mac_srs_dedicated_setup_info->transmission_comb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_setup_info->cyclic_shift > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_setup_info->cyclic_shift] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_srs_dedicated_setup_info->cyclic_shift);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_srs_dedicated_setup_info->cyclic_shift, "cyclic_shift");
    *pp_buffer += sizeof(p_rrc_mac_srs_dedicated_setup_info->cyclic_shift);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_srs_dedicated_config_len
(
    rrc_mac_srs_dedicated_config_t *p_rrc_mac_srs_dedicated_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_srs_dedicated_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_srs_dedicated_config->config_type);

    /* Get length of TLV */
    if (p_rrc_mac_srs_dedicated_config->optional_elems_present & RRC_MAC_SRS_DEDICATED_SETUP_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_srs_dedicated_setup_info_tlv_len(&p_rrc_mac_srs_dedicated_config->srs_dedicated_setup_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_srs_dedicated_config_tlv_len
(
    rrc_mac_srs_dedicated_config_t *p_rrc_mac_srs_dedicated_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_srs_dedicated_config != PNULL);

    length += rrc_il_get_rrc_mac_srs_dedicated_config_len(p_rrc_mac_srs_dedicated_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_srs_dedicated_config
(
    U8  **pp_buffer,
    rrc_mac_srs_dedicated_config_t *p_rrc_mac_srs_dedicated_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_srs_dedicated_config != PNULL);

    /* This function composes rrc_mac_srs_dedicated_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SRS_DEDICATED_CONFIG;
    tlv_length = rrc_il_get_rrc_mac_srs_dedicated_config_tlv_len(p_rrc_mac_srs_dedicated_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_srs_dedicated_config->config_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_srs_dedicated_config->config_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_srs_dedicated_config->config_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_srs_dedicated_config->config_type, "config_type");
    *pp_buffer += sizeof(p_rrc_mac_srs_dedicated_config->config_type);

    /* Compose TLV  */
    if (p_rrc_mac_srs_dedicated_config->optional_elems_present & RRC_MAC_SRS_DEDICATED_SETUP_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_srs_dedicated_setup_info(pp_buffer, &p_rrc_mac_srs_dedicated_config->srs_dedicated_setup_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_codebook_subset_restriction_len
(
    rrc_mac_codebook_subset_restriction_t *p_rrc_mac_codebook_subset_restriction
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_codebook_subset_restriction != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_codebook_subset_restriction->cbsr_type);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_mac_codebook_subset_restriction->cbsr_value);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_codebook_subset_restriction_tlv_len
(
    rrc_mac_codebook_subset_restriction_t *p_rrc_mac_codebook_subset_restriction

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_codebook_subset_restriction != PNULL);

    length += rrc_il_get_rrc_mac_codebook_subset_restriction_len(p_rrc_mac_codebook_subset_restriction);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_codebook_subset_restriction
(
    U8  **pp_buffer,
    rrc_mac_codebook_subset_restriction_t *p_rrc_mac_codebook_subset_restriction
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_codebook_subset_restriction != PNULL);

    /* This function composes rrc_mac_codebook_subset_restriction */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CODEBOOK_SUBSET_RESTRICTION;
    tlv_length = rrc_il_get_rrc_mac_codebook_subset_restriction_tlv_len(p_rrc_mac_codebook_subset_restriction);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_codebook_subset_restriction->cbsr_type > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_codebook_subset_restriction->cbsr_type] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_codebook_subset_restriction->cbsr_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_codebook_subset_restriction->cbsr_type, "cbsr_type");
    *pp_buffer += sizeof(p_rrc_mac_codebook_subset_restriction->cbsr_type);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_codebook_subset_restriction->cbsr_value); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_codebook_subset_restriction->cbsr_value[loop], "cbsr_value[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_short_drx_config_len
(
    rrc_mac_short_drx_config_t *p_rrc_mac_short_drx_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_short_drx_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_short_drx_config->short_drx_cycle);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_short_drx_config->short_drx_cycle_timer);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_short_drx_config_tlv_len
(
    rrc_mac_short_drx_config_t *p_rrc_mac_short_drx_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_short_drx_config != PNULL);

    length += rrc_il_get_rrc_mac_short_drx_config_len(p_rrc_mac_short_drx_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_short_drx_config
(
    U8  **pp_buffer,
    rrc_mac_short_drx_config_t *p_rrc_mac_short_drx_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_short_drx_config != PNULL);

    /* This function composes rrc_mac_short_drx_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SHORT_DRX_CONFIG;
    tlv_length = rrc_il_get_rrc_mac_short_drx_config_tlv_len(p_rrc_mac_short_drx_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_short_drx_config->short_drx_cycle > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_short_drx_config->short_drx_cycle] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_short_drx_config->short_drx_cycle);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_short_drx_config->short_drx_cycle, "short_drx_cycle");
    *pp_buffer += sizeof(p_rrc_mac_short_drx_config->short_drx_cycle);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_short_drx_config->short_drx_cycle_timer < 1) || (p_rrc_mac_short_drx_config->short_drx_cycle_timer > 16))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_short_drx_config->short_drx_cycle_timer] should be in range "
            "1 to 16. Incorrect value %u received.", p_rrc_mac_short_drx_config->short_drx_cycle_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_short_drx_config->short_drx_cycle_timer, "short_drx_cycle_timer");
    *pp_buffer += sizeof(p_rrc_mac_short_drx_config->short_drx_cycle_timer);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cmd_l2_trigger_info_len
(
    rrc_mac_cmd_l2_trigger_info_t *p_rrc_mac_cmd_l2_trigger_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cmd_l2_trigger_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_timer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_cmd_threshold);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_cmd_l2_trigger_info_tlv_len
(
    rrc_mac_cmd_l2_trigger_info_t *p_rrc_mac_cmd_l2_trigger_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cmd_l2_trigger_info != PNULL);

    length += rrc_il_get_rrc_mac_cmd_l2_trigger_info_len(p_rrc_mac_cmd_l2_trigger_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_cmd_l2_trigger_info
(
    U8  **pp_buffer,
    rrc_mac_cmd_l2_trigger_info_t *p_rrc_mac_cmd_l2_trigger_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_cmd_l2_trigger_info != PNULL);

    /* This function composes rrc_mac_cmd_l2_trigger_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DRX_MAC_CE_CMD_TRIGGER_INFO;
    tlv_length = rrc_il_get_rrc_mac_cmd_l2_trigger_info_tlv_len(p_rrc_mac_cmd_l2_trigger_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_timer < 3) || (p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_timer > 2559))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_timer] should be in range "
            "3 to 2559. Incorrect value %u received.", p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_timer, "drx_mac_ce_timer");
    *pp_buffer += sizeof(p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_timer);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_cmd_threshold < 1) || (p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_cmd_threshold > 2559))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_cmd_threshold] should be in range "
            "1 to 2559. Incorrect value %u received.", p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_cmd_threshold);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_cmd_threshold, "drx_mac_ce_cmd_threshold");
    *pp_buffer += sizeof(p_rrc_mac_cmd_l2_trigger_info->drx_mac_ce_cmd_threshold);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_drx_setup_config_info_len
(
    rrc_mac_drx_setup_config_info_t *p_rrc_mac_drx_setup_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_drx_setup_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_drx_setup_config_info->on_duration_timer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_drx_setup_config_info->drx_inactivity_timer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_drx_setup_config_info->drx_retransmission_timer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_drx_setup_config_info->long_drx_cycle);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_drx_setup_config_info->drx_start_offset);

    /* Get length of TLV */
    if (p_rrc_mac_drx_setup_config_info->bitmask & RRC_MAC_DRX_SETUP_CONFIG_SHORT_DRX_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_short_drx_config_tlv_len(&p_rrc_mac_drx_setup_config_info->short_drx);
    }

    /* Get length of TLV */
    if (p_rrc_mac_drx_setup_config_info->bitmask & RRC_MAC_DRX_SETUP_CONFIG_DRX_MAC_CE_CMD_TRIGGER_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_cmd_l2_trigger_info_tlv_len(&p_rrc_mac_drx_setup_config_info->mac_ce_cmd_l2_trigger_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_drx_setup_config_info_tlv_len
(
    rrc_mac_drx_setup_config_info_t *p_rrc_mac_drx_setup_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_drx_setup_config_info != PNULL);

    length += rrc_il_get_rrc_mac_drx_setup_config_info_len(p_rrc_mac_drx_setup_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_drx_setup_config_info
(
    U8  **pp_buffer,
    rrc_mac_drx_setup_config_info_t *p_rrc_mac_drx_setup_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_drx_setup_config_info != PNULL);

    /* This function composes rrc_mac_drx_setup_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DRX_SETUP_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_drx_setup_config_info_tlv_len(p_rrc_mac_drx_setup_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_drx_setup_config_info->on_duration_timer > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_drx_setup_config_info->on_duration_timer] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_drx_setup_config_info->on_duration_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_drx_setup_config_info->on_duration_timer, "on_duration_timer");
    *pp_buffer += sizeof(p_rrc_mac_drx_setup_config_info->on_duration_timer);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_drx_setup_config_info->drx_inactivity_timer > 22))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_drx_setup_config_info->drx_inactivity_timer] should be less than"
            " or equal to 22. Incorrect value %u received.", p_rrc_mac_drx_setup_config_info->drx_inactivity_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_drx_setup_config_info->drx_inactivity_timer, "drx_inactivity_timer");
    *pp_buffer += sizeof(p_rrc_mac_drx_setup_config_info->drx_inactivity_timer);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_drx_setup_config_info->drx_retransmission_timer > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_drx_setup_config_info->drx_retransmission_timer] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_drx_setup_config_info->drx_retransmission_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_drx_setup_config_info->drx_retransmission_timer, "drx_retransmission_timer");
    *pp_buffer += sizeof(p_rrc_mac_drx_setup_config_info->drx_retransmission_timer);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_drx_setup_config_info->long_drx_cycle > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_drx_setup_config_info->long_drx_cycle] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_drx_setup_config_info->long_drx_cycle);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_drx_setup_config_info->long_drx_cycle, "long_drx_cycle");
    *pp_buffer += sizeof(p_rrc_mac_drx_setup_config_info->long_drx_cycle);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_drx_setup_config_info->drx_start_offset, "drx_start_offset");
    *pp_buffer += sizeof(p_rrc_mac_drx_setup_config_info->drx_start_offset);

    /* Compose TLV  */
    if (p_rrc_mac_drx_setup_config_info->bitmask & RRC_MAC_DRX_SETUP_CONFIG_SHORT_DRX_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_short_drx_config(pp_buffer, &p_rrc_mac_drx_setup_config_info->short_drx))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_drx_setup_config_info->bitmask & RRC_MAC_DRX_SETUP_CONFIG_DRX_MAC_CE_CMD_TRIGGER_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_cmd_l2_trigger_info(pp_buffer, &p_rrc_mac_drx_setup_config_info->mac_ce_cmd_l2_trigger_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_drx_config_info_len
(
    rrc_mac_drx_config_info_t *p_rrc_mac_drx_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_drx_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_drx_config_info->drx_config_type);

    /* Get length of TLV */
    if (p_rrc_mac_drx_config_info->bitmask & RRC_MAC_DRX_CONFIG_DRX_SETUP_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_drx_setup_config_info_tlv_len(&p_rrc_mac_drx_config_info->drx_setup_config_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_drx_config_info_tlv_len
(
    rrc_mac_drx_config_info_t *p_rrc_mac_drx_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_drx_config_info != PNULL);

    length += rrc_il_get_rrc_mac_drx_config_info_len(p_rrc_mac_drx_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_drx_config_info
(
    U8  **pp_buffer,
    rrc_mac_drx_config_info_t *p_rrc_mac_drx_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_drx_config_info != PNULL);

    /* This function composes rrc_mac_drx_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DRX_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_drx_config_info_tlv_len(p_rrc_mac_drx_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_drx_config_info->drx_config_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_drx_config_info->drx_config_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_drx_config_info->drx_config_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_drx_config_info->drx_config_type, "drx_config_type");
    *pp_buffer += sizeof(p_rrc_mac_drx_config_info->drx_config_type);

    /* Compose TLV  */
    if (p_rrc_mac_drx_config_info->bitmask & RRC_MAC_DRX_CONFIG_DRX_SETUP_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_drx_setup_config_info(pp_buffer, &p_rrc_mac_drx_config_info->drx_setup_config_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_meas_gap_setup_config_info_len
(
    rrc_mac_meas_gap_setup_config_info_t *p_rrc_mac_meas_gap_setup_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_meas_gap_setup_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_meas_gap_setup_config_info->gap_pattern_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_meas_gap_setup_config_info->gap_offset);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_meas_gap_setup_config_info_tlv_len
(
    rrc_mac_meas_gap_setup_config_info_t *p_rrc_mac_meas_gap_setup_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_meas_gap_setup_config_info != PNULL);

    length += rrc_il_get_rrc_mac_meas_gap_setup_config_info_len(p_rrc_mac_meas_gap_setup_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_meas_gap_setup_config_info
(
    U8  **pp_buffer,
    rrc_mac_meas_gap_setup_config_info_t *p_rrc_mac_meas_gap_setup_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_meas_gap_setup_config_info != PNULL);

    /* This function composes rrc_mac_meas_gap_setup_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MEAS_GAP_SETUP_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_meas_gap_setup_config_info_tlv_len(p_rrc_mac_meas_gap_setup_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_meas_gap_setup_config_info->gap_pattern_id > 11))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_meas_gap_setup_config_info->gap_pattern_id] should be less than"
            " or equal to 11. Incorrect value %u received.", p_rrc_mac_meas_gap_setup_config_info->gap_pattern_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_meas_gap_setup_config_info->gap_pattern_id, "gap_pattern_id");
    *pp_buffer += sizeof(p_rrc_mac_meas_gap_setup_config_info->gap_pattern_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_meas_gap_setup_config_info->gap_offset > 159))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_meas_gap_setup_config_info->gap_offset] should be less than"
            " or equal to 159. Incorrect value %u received.", p_rrc_mac_meas_gap_setup_config_info->gap_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_meas_gap_setup_config_info->gap_offset, "gap_offset");
    *pp_buffer += sizeof(p_rrc_mac_meas_gap_setup_config_info->gap_offset);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_meas_gap_config_info_len
(
    rrc_mac_meas_gap_config_info_t *p_rrc_mac_meas_gap_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_meas_gap_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_meas_gap_config_info->meas_gap_config_type);

    /* Get length of TLV */
    if (p_rrc_mac_meas_gap_config_info->bitmask & RRC_MAC_MEAS_GAP_CONFIG_MEAS_GAP_SETUP_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_meas_gap_setup_config_info_tlv_len(&p_rrc_mac_meas_gap_config_info->meas_gap_setup_config_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_meas_gap_config_info_tlv_len
(
    rrc_mac_meas_gap_config_info_t *p_rrc_mac_meas_gap_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_meas_gap_config_info != PNULL);

    length += rrc_il_get_rrc_mac_meas_gap_config_info_len(p_rrc_mac_meas_gap_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_meas_gap_config_info
(
    U8  **pp_buffer,
    rrc_mac_meas_gap_config_info_t *p_rrc_mac_meas_gap_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_meas_gap_config_info != PNULL);

    /* This function composes rrc_mac_meas_gap_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MEAS_GAP_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_meas_gap_config_info_tlv_len(p_rrc_mac_meas_gap_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_meas_gap_config_info->meas_gap_config_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_meas_gap_config_info->meas_gap_config_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_meas_gap_config_info->meas_gap_config_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_meas_gap_config_info->meas_gap_config_type, "meas_gap_config_type");
    *pp_buffer += sizeof(p_rrc_mac_meas_gap_config_info->meas_gap_config_type);

    /* Compose TLV  */
    if (p_rrc_mac_meas_gap_config_info->bitmask & RRC_MAC_MEAS_GAP_CONFIG_MEAS_GAP_SETUP_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_meas_gap_setup_config_info(pp_buffer, &p_rrc_mac_meas_gap_config_info->meas_gap_setup_config_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_codebook_subset_restriction_v920_len
(
    rrc_mac_codebook_subset_restriction_v920_t *p_rrc_mac_codebook_subset_restriction_v920
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_codebook_subset_restriction_v920 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_codebook_subset_restriction_v920->cbsr_type);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_mac_codebook_subset_restriction_v920->cbsr_value);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_codebook_subset_restriction_v920_tlv_len
(
    rrc_mac_codebook_subset_restriction_v920_t *p_rrc_mac_codebook_subset_restriction_v920

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_codebook_subset_restriction_v920 != PNULL);

    length += rrc_il_get_rrc_mac_codebook_subset_restriction_v920_len(p_rrc_mac_codebook_subset_restriction_v920);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_codebook_subset_restriction_v920
(
    U8  **pp_buffer,
    rrc_mac_codebook_subset_restriction_v920_t *p_rrc_mac_codebook_subset_restriction_v920
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_codebook_subset_restriction_v920 != PNULL);

    /* This function composes rrc_mac_codebook_subset_restriction_v920 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V920;
    tlv_length = rrc_il_get_rrc_mac_codebook_subset_restriction_v920_tlv_len(p_rrc_mac_codebook_subset_restriction_v920);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_codebook_subset_restriction_v920->cbsr_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_codebook_subset_restriction_v920->cbsr_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_codebook_subset_restriction_v920->cbsr_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_codebook_subset_restriction_v920->cbsr_type, "cbsr_type");
    *pp_buffer += sizeof(p_rrc_mac_codebook_subset_restriction_v920->cbsr_type);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_codebook_subset_restriction_v920->cbsr_value); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_codebook_subset_restriction_v920->cbsr_value[loop], "cbsr_value[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_uplink_power_control_dedicated_len
(
    rrc_mac_uplink_power_control_dedicated_t *p_rrc_mac_uplink_power_control_dedicated
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_uplink_power_control_dedicated != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_uplink_power_control_dedicated->p0_ue_pusch);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_uplink_power_control_dedicated->delta_mcs_enabled);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_uplink_power_control_dedicated->accumulation_enabled);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_uplink_power_control_dedicated_tlv_len
(
    rrc_mac_uplink_power_control_dedicated_t *p_rrc_mac_uplink_power_control_dedicated

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_uplink_power_control_dedicated != PNULL);

    length += rrc_il_get_rrc_mac_uplink_power_control_dedicated_len(p_rrc_mac_uplink_power_control_dedicated);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_uplink_power_control_dedicated
(
    U8  **pp_buffer,
    rrc_mac_uplink_power_control_dedicated_t *p_rrc_mac_uplink_power_control_dedicated
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_uplink_power_control_dedicated != PNULL);

    /* This function composes rrc_mac_uplink_power_control_dedicated */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UPLINK_POWER_CONTROL_DEDICATED;
    tlv_length = rrc_il_get_rrc_mac_uplink_power_control_dedicated_tlv_len(p_rrc_mac_uplink_power_control_dedicated);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_uplink_power_control_dedicated->p0_ue_pusch < -8) || (p_rrc_mac_uplink_power_control_dedicated->p0_ue_pusch > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_uplink_power_control_dedicated->p0_ue_pusch] should be in range "
            "-8 to 7. Incorrect value %d received.", p_rrc_mac_uplink_power_control_dedicated->p0_ue_pusch);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_mac_uplink_power_control_dedicated->p0_ue_pusch, "p0_ue_pusch");
    *pp_buffer += sizeof(p_rrc_mac_uplink_power_control_dedicated->p0_ue_pusch);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_uplink_power_control_dedicated->delta_mcs_enabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_uplink_power_control_dedicated->delta_mcs_enabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_uplink_power_control_dedicated->delta_mcs_enabled);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_uplink_power_control_dedicated->delta_mcs_enabled, "delta_mcs_enabled");
    *pp_buffer += sizeof(p_rrc_mac_uplink_power_control_dedicated->delta_mcs_enabled);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_uplink_power_control_dedicated->accumulation_enabled > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_uplink_power_control_dedicated->accumulation_enabled] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_uplink_power_control_dedicated->accumulation_enabled);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_uplink_power_control_dedicated->accumulation_enabled, "accumulation_enabled");
    *pp_buffer += sizeof(p_rrc_mac_uplink_power_control_dedicated->accumulation_enabled);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_rnti_info_len
(
    rrc_mac_tpc_rnti_info_t *p_rrc_mac_tpc_rnti_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tpc_rnti_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tpc_rnti_info->tpcRnti);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tpc_rnti_info->tpcIndex);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tpc_rnti_info->isConfiguredForDci3Or3A);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_rnti_info_tlv_len
(
    rrc_mac_tpc_rnti_info_t *p_rrc_mac_tpc_rnti_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tpc_rnti_info != PNULL);

    length += rrc_il_get_rrc_mac_tpc_rnti_info_len(p_rrc_mac_tpc_rnti_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_tpc_rnti_info
(
    U8  **pp_buffer,
    rrc_mac_tpc_rnti_info_t *p_rrc_mac_tpc_rnti_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_tpc_rnti_info != PNULL);

    /* This function composes rrc_mac_tpc_rnti_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_TPC_RNTI_INFO;
    tlv_length = rrc_il_get_rrc_mac_tpc_rnti_info_tlv_len(p_rrc_mac_tpc_rnti_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tpc_rnti_info->tpcRnti < 1) || (p_rrc_mac_tpc_rnti_info->tpcRnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_rnti_info->tpcRnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_tpc_rnti_info->tpcRnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_tpc_rnti_info->tpcRnti, "tpcRnti");
    *pp_buffer += sizeof(p_rrc_mac_tpc_rnti_info->tpcRnti);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tpc_rnti_info->tpcIndex < 1) || (p_rrc_mac_tpc_rnti_info->tpcIndex > 31))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_rnti_info->tpcIndex] should be in range "
            "1 to 31. Incorrect value %u received.", p_rrc_mac_tpc_rnti_info->tpcIndex);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_tpc_rnti_info->tpcIndex, "tpcIndex");
    *pp_buffer += sizeof(p_rrc_mac_tpc_rnti_info->tpcIndex);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_tpc_rnti_info->isConfiguredForDci3Or3A, "isConfiguredForDci3Or3A");
    *pp_buffer += sizeof(p_rrc_mac_tpc_rnti_info->isConfiguredForDci3Or3A);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_pdcch_config_pucch_len
(
    rrc_mac_tpc_pdcch_config_pucch_t *p_rrc_mac_tpc_pdcch_config_pucch
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tpc_pdcch_config_pucch != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tpc_pdcch_config_pucch->release);

    /* Get length of TLV */
    if (p_rrc_mac_tpc_pdcch_config_pucch->bitmask & RRC_MAC_TPC_RNTI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tpc_rnti_info_tlv_len(&p_rrc_mac_tpc_pdcch_config_pucch->tpc_rnti_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_pdcch_config_pucch_tlv_len
(
    rrc_mac_tpc_pdcch_config_pucch_t *p_rrc_mac_tpc_pdcch_config_pucch

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tpc_pdcch_config_pucch != PNULL);

    length += rrc_il_get_rrc_mac_tpc_pdcch_config_pucch_len(p_rrc_mac_tpc_pdcch_config_pucch);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_tpc_pdcch_config_pucch
(
    U8  **pp_buffer,
    rrc_mac_tpc_pdcch_config_pucch_t *p_rrc_mac_tpc_pdcch_config_pucch
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_tpc_pdcch_config_pucch != PNULL);

    /* This function composes rrc_mac_tpc_pdcch_config_pucch */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_TPC_PDCCH_CONFIG_PUCCH;
    tlv_length = rrc_il_get_rrc_mac_tpc_pdcch_config_pucch_tlv_len(p_rrc_mac_tpc_pdcch_config_pucch);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_tpc_pdcch_config_pucch->release > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_pdcch_config_pucch->release] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_tpc_pdcch_config_pucch->release);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_tpc_pdcch_config_pucch->release, "release");
    *pp_buffer += sizeof(p_rrc_mac_tpc_pdcch_config_pucch->release);

    /* Compose TLV  */
    if (p_rrc_mac_tpc_pdcch_config_pucch->bitmask & RRC_MAC_TPC_RNTI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tpc_rnti_info(pp_buffer, &p_rrc_mac_tpc_pdcch_config_pucch->tpc_rnti_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_pdcch_config_pusch_len
(
    rrc_mac_tpc_pdcch_config_pusch_t *p_rrc_mac_tpc_pdcch_config_pusch
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tpc_pdcch_config_pusch != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tpc_pdcch_config_pusch->release);

    /* Get length of TLV */
    if (p_rrc_mac_tpc_pdcch_config_pusch->bitmask & RRC_MAC_TPC_RNTI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tpc_rnti_info_tlv_len(&p_rrc_mac_tpc_pdcch_config_pusch->tpc_rnti_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tpc_pdcch_config_pusch_tlv_len
(
    rrc_mac_tpc_pdcch_config_pusch_t *p_rrc_mac_tpc_pdcch_config_pusch

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tpc_pdcch_config_pusch != PNULL);

    length += rrc_il_get_rrc_mac_tpc_pdcch_config_pusch_len(p_rrc_mac_tpc_pdcch_config_pusch);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_tpc_pdcch_config_pusch
(
    U8  **pp_buffer,
    rrc_mac_tpc_pdcch_config_pusch_t *p_rrc_mac_tpc_pdcch_config_pusch
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_tpc_pdcch_config_pusch != PNULL);

    /* This function composes rrc_mac_tpc_pdcch_config_pusch */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_TPC_PDCCH_CONFIG_PUSCH;
    tlv_length = rrc_il_get_rrc_mac_tpc_pdcch_config_pusch_tlv_len(p_rrc_mac_tpc_pdcch_config_pusch);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_tpc_pdcch_config_pusch->release > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tpc_pdcch_config_pusch->release] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_tpc_pdcch_config_pusch->release);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_tpc_pdcch_config_pusch->release, "release");
    *pp_buffer += sizeof(p_rrc_mac_tpc_pdcch_config_pusch->release);

    /* Compose TLV  */
    if (p_rrc_mac_tpc_pdcch_config_pusch->bitmask & RRC_MAC_TPC_RNTI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tpc_rnti_info(pp_buffer, &p_rrc_mac_tpc_pdcch_config_pusch->tpc_rnti_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ambr_qos_info_len
(
    rrc_mac_ambr_qos_info_t *p_rrc_mac_ambr_qos_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ambr_qos_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ambr_qos_info->dl_ambr);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ambr_qos_info->ul_ambr);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ambr_qos_info_tlv_len
(
    rrc_mac_ambr_qos_info_t *p_rrc_mac_ambr_qos_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ambr_qos_info != PNULL);

    length += rrc_il_get_rrc_mac_ambr_qos_info_len(p_rrc_mac_ambr_qos_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ambr_qos_info
(
    U8  **pp_buffer,
    rrc_mac_ambr_qos_info_t *p_rrc_mac_ambr_qos_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ambr_qos_info != PNULL);

    /* This function composes rrc_mac_ambr_qos_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_AMBR_QOS_INFO;
    tlv_length = rrc_il_get_rrc_mac_ambr_qos_info_tlv_len(p_rrc_mac_ambr_qos_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_mac_ambr_qos_info->dl_ambr, "dl_ambr");
    *pp_buffer += sizeof(p_rrc_mac_ambr_qos_info->dl_ambr);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_mac_ambr_qos_info->ul_ambr, "ul_ambr");
    *pp_buffer += sizeof(p_rrc_mac_ambr_qos_info->ul_ambr);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_user_location_type_len
(
    rrc_mac_user_location_type_t *p_rrc_mac_user_location_type
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_user_location_type != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_user_location_type->user_location_type);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_user_location_type_tlv_len
(
    rrc_mac_user_location_type_t *p_rrc_mac_user_location_type

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_user_location_type != PNULL);

    length += rrc_il_get_rrc_mac_user_location_type_len(p_rrc_mac_user_location_type);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_user_location_type
(
    U8  **pp_buffer,
    rrc_mac_user_location_type_t *p_rrc_mac_user_location_type
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_user_location_type != PNULL);

    /* This function composes rrc_mac_user_location_type */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_USER_LOCATION_TYPE;
    tlv_length = rrc_il_get_rrc_mac_user_location_type_tlv_len(p_rrc_mac_user_location_type);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_user_location_type->user_location_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_user_location_type->user_location_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_user_location_type->user_location_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_user_location_type->user_location_type, "user_location_type");
    *pp_buffer += sizeof(p_rrc_mac_user_location_type->user_location_type);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ue_capability_additional_params_len
(
    rrc_mac_ue_capability_additional_params_t *p_rrc_mac_ue_capability_additional_params
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ue_capability_additional_params != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ue_capability_additional_params->rat_1_support);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ue_capability_additional_params_tlv_len
(
    rrc_mac_ue_capability_additional_params_t *p_rrc_mac_ue_capability_additional_params

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ue_capability_additional_params != PNULL);

    length += rrc_il_get_rrc_mac_ue_capability_additional_params_len(p_rrc_mac_ue_capability_additional_params);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ue_capability_additional_params
(
    U8  **pp_buffer,
    rrc_mac_ue_capability_additional_params_t *p_rrc_mac_ue_capability_additional_params
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ue_capability_additional_params != PNULL);

    /* This function composes rrc_mac_ue_capability_additional_params */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UE_CAPABILITY_ADDITIONAL_PARAMETERS;
    tlv_length = rrc_il_get_rrc_mac_ue_capability_additional_params_tlv_len(p_rrc_mac_ue_capability_additional_params);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ue_capability_additional_params->rat_1_support > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_capability_additional_params->rat_1_support] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_ue_capability_additional_params->rat_1_support);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_ue_capability_additional_params->rat_1_support, "rat_1_support");
    *pp_buffer += sizeof(p_rrc_mac_ue_capability_additional_params->rat_1_support);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_n1_pucch_an_persist_list_len
(
    rrc_mac_n1_pucch_an_persist_list_t *p_rrc_mac_n1_pucch_an_persist_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_n1_pucch_an_persist_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_n1_pucch_an_persist_list->n1_pucch_an_persist);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_n1_pucch_an_persist_list_tlv_len
(
    rrc_mac_n1_pucch_an_persist_list_t *p_rrc_mac_n1_pucch_an_persist_list

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_n1_pucch_an_persist_list != PNULL);

    length += rrc_il_get_rrc_mac_n1_pucch_an_persist_list_len(p_rrc_mac_n1_pucch_an_persist_list);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_n1_pucch_an_persist_list
(
    U8  **pp_buffer,
    rrc_mac_n1_pucch_an_persist_list_t *p_rrc_mac_n1_pucch_an_persist_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_n1_pucch_an_persist_list != PNULL);

    /* This function composes rrc_mac_n1_pucch_an_persist_list */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SPS_N1_PUCCH_AN_PERSIST_LIST;
    tlv_length = rrc_il_get_rrc_mac_n1_pucch_an_persist_list_tlv_len(p_rrc_mac_n1_pucch_an_persist_list);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_n1_pucch_an_persist_list->n1_pucch_an_persist > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_n1_pucch_an_persist_list->n1_pucch_an_persist] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_mac_n1_pucch_an_persist_list->n1_pucch_an_persist);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_n1_pucch_an_persist_list->n1_pucch_an_persist, "n1_pucch_an_persist");
    *pp_buffer += sizeof(p_rrc_mac_n1_pucch_an_persist_list->n1_pucch_an_persist);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_max_sps_harq_retx_len
(
    rrc_mac_max_sps_harq_retx_t *p_rrc_mac_max_sps_harq_retx
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_max_sps_harq_retx != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_max_sps_harq_retx->max_sps_harq_retx);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_max_sps_harq_retx_tlv_len
(
    rrc_mac_max_sps_harq_retx_t *p_rrc_mac_max_sps_harq_retx

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_max_sps_harq_retx != PNULL);

    length += rrc_il_get_rrc_mac_max_sps_harq_retx_len(p_rrc_mac_max_sps_harq_retx);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_max_sps_harq_retx
(
    U8  **pp_buffer,
    rrc_mac_max_sps_harq_retx_t *p_rrc_mac_max_sps_harq_retx
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_max_sps_harq_retx != PNULL);

    /* This function composes rrc_mac_max_sps_harq_retx */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MAX_SPS_HARQ_RETX;
    tlv_length = rrc_il_get_rrc_mac_max_sps_harq_retx_tlv_len(p_rrc_mac_max_sps_harq_retx);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_max_sps_harq_retx->max_sps_harq_retx > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_max_sps_harq_retx->max_sps_harq_retx] should be less than"
            " or equal to 8. Incorrect value %u received.", p_rrc_mac_max_sps_harq_retx->max_sps_harq_retx);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_max_sps_harq_retx->max_sps_harq_retx, "max_sps_harq_retx");
    *pp_buffer += sizeof(p_rrc_mac_max_sps_harq_retx->max_sps_harq_retx);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_explicit_release_after_len
(
    rrc_mac_explicit_release_after_t *p_rrc_mac_explicit_release_after
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_explicit_release_after != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_explicit_release_after->explicit_release_afte);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_explicit_release_after_tlv_len
(
    rrc_mac_explicit_release_after_t *p_rrc_mac_explicit_release_after

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_explicit_release_after != PNULL);

    length += rrc_il_get_rrc_mac_explicit_release_after_len(p_rrc_mac_explicit_release_after);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_explicit_release_after
(
    U8  **pp_buffer,
    rrc_mac_explicit_release_after_t *p_rrc_mac_explicit_release_after
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_explicit_release_after != PNULL);

    /* This function composes rrc_mac_explicit_release_after */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_EXPLICIT_RELEASE_AFTER;
    tlv_length = rrc_il_get_rrc_mac_explicit_release_after_tlv_len(p_rrc_mac_explicit_release_after);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_explicit_release_after->explicit_release_afte > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_explicit_release_after->explicit_release_afte] should be less than"
            " or equal to 8. Incorrect value %u received.", p_rrc_mac_explicit_release_after->explicit_release_afte);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_explicit_release_after->explicit_release_afte, "explicit_release_afte");
    *pp_buffer += sizeof(p_rrc_mac_explicit_release_after->explicit_release_afte);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_dl_setup_info_len
(
    rrc_mac_sps_dl_setup_info_t *p_rrc_mac_sps_dl_setup_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_dl_setup_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sps_dl_setup_info->semi_persist_sched_interval_dl);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sps_dl_setup_info->number_of_conf_sps_processes);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist_list_counter <= ARRSIZE(p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist));
        for (loop = 0; loop < p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist_list_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_n1_pucch_an_persist_list_tlv_len(&p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_mac_sps_dl_setup_info->bitmask & RRC_MAC_MAX_SPS_HARQ_RETX_PRESENT)
    {
        length += rrc_il_get_rrc_mac_max_sps_harq_retx_tlv_len(&p_rrc_mac_sps_dl_setup_info->max_sps_harq_retx);
    }

    /* Get length of TLV */
    if (p_rrc_mac_sps_dl_setup_info->bitmask & RRC_MAC_RRC_MAC_EXPLICIT_RELEASE_AFTER_PRESENT)
    {
        length += rrc_il_get_rrc_mac_explicit_release_after_tlv_len(&p_rrc_mac_sps_dl_setup_info->explicit_release_after);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_dl_setup_info_tlv_len
(
    rrc_mac_sps_dl_setup_info_t *p_rrc_mac_sps_dl_setup_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_dl_setup_info != PNULL);

    length += rrc_il_get_rrc_mac_sps_dl_setup_info_len(p_rrc_mac_sps_dl_setup_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_dl_setup_info
(
    U8  **pp_buffer,
    rrc_mac_sps_dl_setup_info_t *p_rrc_mac_sps_dl_setup_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sps_dl_setup_info != PNULL);

    /* This function composes rrc_mac_sps_dl_setup_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SPS_DL_SETUP_INFO;
    tlv_length = rrc_il_get_rrc_mac_sps_dl_setup_info_tlv_len(p_rrc_mac_sps_dl_setup_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sps_dl_setup_info->semi_persist_sched_interval_dl > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_dl_setup_info->semi_persist_sched_interval_dl] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_sps_dl_setup_info->semi_persist_sched_interval_dl);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sps_dl_setup_info->semi_persist_sched_interval_dl, "semi_persist_sched_interval_dl");
    *pp_buffer += sizeof(p_rrc_mac_sps_dl_setup_info->semi_persist_sched_interval_dl);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_sps_dl_setup_info->number_of_conf_sps_processes < 1) || (p_rrc_mac_sps_dl_setup_info->number_of_conf_sps_processes > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_dl_setup_info->number_of_conf_sps_processes] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_mac_sps_dl_setup_info->number_of_conf_sps_processes);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sps_dl_setup_info->number_of_conf_sps_processes, "number_of_conf_sps_processes");
    *pp_buffer += sizeof(p_rrc_mac_sps_dl_setup_info->number_of_conf_sps_processes);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist_list_counter <= ARRSIZE(p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist));
        for (loop = 0; loop < p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist_list_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_n1_pucch_an_persist_list(pp_buffer, &p_rrc_mac_sps_dl_setup_info->n1_pucch_an_persist[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_sps_dl_setup_info->bitmask & RRC_MAC_MAX_SPS_HARQ_RETX_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_max_sps_harq_retx(pp_buffer, &p_rrc_mac_sps_dl_setup_info->max_sps_harq_retx))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_sps_dl_setup_info->bitmask & RRC_MAC_RRC_MAC_EXPLICIT_RELEASE_AFTER_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_explicit_release_after(pp_buffer, &p_rrc_mac_sps_dl_setup_info->explicit_release_after))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_dl_info_len
(
    rrc_mac_sps_dl_info_t *p_rrc_mac_sps_dl_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_dl_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sps_dl_info->request_type);

    /* Get length of TLV */
    if (p_rrc_mac_sps_dl_info->bitmask & RRC_MAC_SPS_DL_SETUP_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sps_dl_setup_info_tlv_len(&p_rrc_mac_sps_dl_info->sps_dl_setup_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_dl_info_tlv_len
(
    rrc_mac_sps_dl_info_t *p_rrc_mac_sps_dl_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_dl_info != PNULL);

    length += rrc_il_get_rrc_mac_sps_dl_info_len(p_rrc_mac_sps_dl_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_dl_info
(
    U8  **pp_buffer,
    rrc_mac_sps_dl_info_t *p_rrc_mac_sps_dl_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sps_dl_info != PNULL);

    /* This function composes rrc_mac_sps_dl_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SPS_DL_INFO;
    tlv_length = rrc_il_get_rrc_mac_sps_dl_info_tlv_len(p_rrc_mac_sps_dl_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sps_dl_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_dl_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_sps_dl_info->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sps_dl_info->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_mac_sps_dl_info->request_type);

    /* Compose TLV  */
    if (p_rrc_mac_sps_dl_info->bitmask & RRC_MAC_SPS_DL_SETUP_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sps_dl_setup_info(pp_buffer, &p_rrc_mac_sps_dl_info->sps_dl_setup_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_p_zero_persistent_len
(
    rrc_mac_p_zero_persistent_t *p_rrc_mac_p_zero_persistent
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_p_zero_persistent != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_p_zero_persistent->p_zero_nominal_pusch_persistent);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_p_zero_persistent->p_zero_ue_pusch_persistent);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_p_zero_persistent_tlv_len
(
    rrc_mac_p_zero_persistent_t *p_rrc_mac_p_zero_persistent

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_p_zero_persistent != PNULL);

    length += rrc_il_get_rrc_mac_p_zero_persistent_len(p_rrc_mac_p_zero_persistent);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_p_zero_persistent
(
    U8  **pp_buffer,
    rrc_mac_p_zero_persistent_t *p_rrc_mac_p_zero_persistent
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_p_zero_persistent != PNULL);

    /* This function composes rrc_mac_p_zero_persistent */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_P_ZERO_PERSISTENT;
    tlv_length = rrc_il_get_rrc_mac_p_zero_persistent_tlv_len(p_rrc_mac_p_zero_persistent);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_p_zero_persistent->p_zero_nominal_pusch_persistent < -126) || (p_rrc_mac_p_zero_persistent->p_zero_nominal_pusch_persistent > 24))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_p_zero_persistent->p_zero_nominal_pusch_persistent] should be in range "
            "-126 to 24. Incorrect value %d received.", p_rrc_mac_p_zero_persistent->p_zero_nominal_pusch_persistent);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_mac_p_zero_persistent->p_zero_nominal_pusch_persistent, "p_zero_nominal_pusch_persistent");
    *pp_buffer += sizeof(p_rrc_mac_p_zero_persistent->p_zero_nominal_pusch_persistent);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_p_zero_persistent->p_zero_ue_pusch_persistent < -8) || (p_rrc_mac_p_zero_persistent->p_zero_ue_pusch_persistent > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_p_zero_persistent->p_zero_ue_pusch_persistent] should be in range "
            "-8 to 7. Incorrect value %d received.", p_rrc_mac_p_zero_persistent->p_zero_ue_pusch_persistent);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_mac_p_zero_persistent->p_zero_ue_pusch_persistent, "p_zero_ue_pusch_persistent");
    *pp_buffer += sizeof(p_rrc_mac_p_zero_persistent->p_zero_ue_pusch_persistent);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_two_intervals_config_len
(
    rrc_mac_two_intervals_config_t *p_rrc_mac_two_intervals_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_two_intervals_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_two_intervals_config->two_intervals_config);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_two_intervals_config_tlv_len
(
    rrc_mac_two_intervals_config_t *p_rrc_mac_two_intervals_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_two_intervals_config != PNULL);

    length += rrc_il_get_rrc_mac_two_intervals_config_len(p_rrc_mac_two_intervals_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_two_intervals_config
(
    U8  **pp_buffer,
    rrc_mac_two_intervals_config_t *p_rrc_mac_two_intervals_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_two_intervals_config != PNULL);

    /* This function composes rrc_mac_two_intervals_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_TWO_INTERVALS_CONFIG;
    tlv_length = rrc_il_get_rrc_mac_two_intervals_config_tlv_len(p_rrc_mac_two_intervals_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_two_intervals_config->two_intervals_config < 1) || (p_rrc_mac_two_intervals_config->two_intervals_config > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_two_intervals_config->two_intervals_config] should be in range "
            "1 to 1. Incorrect value %u received.", p_rrc_mac_two_intervals_config->two_intervals_config);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_two_intervals_config->two_intervals_config, "two_intervals_config");
    *pp_buffer += sizeof(p_rrc_mac_two_intervals_config->two_intervals_config);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_ul_setup_info_len
(
    rrc_mac_sps_ul_setup_info_t *p_rrc_mac_sps_ul_setup_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_ul_setup_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sps_ul_setup_info->semi_persist_sched_interval_ul);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sps_ul_setup_info->implicit_release_after);

    /* Get length of TLV */
    if (p_rrc_mac_sps_ul_setup_info->bitmask & RRC_MAC_P_ZERO_PERSISTENT_PRESENT)
    {
        length += rrc_il_get_rrc_mac_p_zero_persistent_tlv_len(&p_rrc_mac_sps_ul_setup_info->p_zero_persistent);
    }

    /* Get length of TLV */
    if (p_rrc_mac_sps_ul_setup_info->bitmask & RRC_MAC_TWO_INTERVALS_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_two_intervals_config_tlv_len(&p_rrc_mac_sps_ul_setup_info->two_intervals_config);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_ul_setup_info_tlv_len
(
    rrc_mac_sps_ul_setup_info_t *p_rrc_mac_sps_ul_setup_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_ul_setup_info != PNULL);

    length += rrc_il_get_rrc_mac_sps_ul_setup_info_len(p_rrc_mac_sps_ul_setup_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_ul_setup_info
(
    U8  **pp_buffer,
    rrc_mac_sps_ul_setup_info_t *p_rrc_mac_sps_ul_setup_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sps_ul_setup_info != PNULL);

    /* This function composes rrc_mac_sps_ul_setup_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SPS_UL_SETUP_INFO;
    tlv_length = rrc_il_get_rrc_mac_sps_ul_setup_info_tlv_len(p_rrc_mac_sps_ul_setup_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sps_ul_setup_info->semi_persist_sched_interval_ul > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_ul_setup_info->semi_persist_sched_interval_ul] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_sps_ul_setup_info->semi_persist_sched_interval_ul);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sps_ul_setup_info->semi_persist_sched_interval_ul, "semi_persist_sched_interval_ul");
    *pp_buffer += sizeof(p_rrc_mac_sps_ul_setup_info->semi_persist_sched_interval_ul);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sps_ul_setup_info->implicit_release_after > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_ul_setup_info->implicit_release_after] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_sps_ul_setup_info->implicit_release_after);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sps_ul_setup_info->implicit_release_after, "implicit_release_after");
    *pp_buffer += sizeof(p_rrc_mac_sps_ul_setup_info->implicit_release_after);

    /* Compose TLV  */
    if (p_rrc_mac_sps_ul_setup_info->bitmask & RRC_MAC_P_ZERO_PERSISTENT_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_p_zero_persistent(pp_buffer, &p_rrc_mac_sps_ul_setup_info->p_zero_persistent))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_sps_ul_setup_info->bitmask & RRC_MAC_TWO_INTERVALS_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_two_intervals_config(pp_buffer, &p_rrc_mac_sps_ul_setup_info->two_intervals_config))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_ul_info_len
(
    rrc_mac_sps_ul_info_t *p_rrc_mac_sps_ul_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_ul_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sps_ul_info->request_type);

    /* Get length of TLV */
    if (p_rrc_mac_sps_ul_info->bitmask & RRC_MAC_SPS_UL_SETUP_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sps_ul_setup_info_tlv_len(&p_rrc_mac_sps_ul_info->sps_ul_setup_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_ul_info_tlv_len
(
    rrc_mac_sps_ul_info_t *p_rrc_mac_sps_ul_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_ul_info != PNULL);

    length += rrc_il_get_rrc_mac_sps_ul_info_len(p_rrc_mac_sps_ul_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_ul_info
(
    U8  **pp_buffer,
    rrc_mac_sps_ul_info_t *p_rrc_mac_sps_ul_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sps_ul_info != PNULL);

    /* This function composes rrc_mac_sps_ul_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SPS_UL_INFO;
    tlv_length = rrc_il_get_rrc_mac_sps_ul_info_tlv_len(p_rrc_mac_sps_ul_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_sps_ul_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_ul_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_sps_ul_info->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_sps_ul_info->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_mac_sps_ul_info->request_type);

    /* Compose TLV  */
    if (p_rrc_mac_sps_ul_info->bitmask & RRC_MAC_SPS_UL_SETUP_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sps_ul_setup_info(pp_buffer, &p_rrc_mac_sps_ul_info->sps_ul_setup_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_config_len
(
    rrc_mac_sps_config_t *p_rrc_mac_sps_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_config != PNULL);

    /* Get length of TLV */
    if (p_rrc_mac_sps_config->bitmask & RRC_MAC_SPS_DL_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sps_dl_info_tlv_len(&p_rrc_mac_sps_config->sps_dl_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_sps_config->bitmask & RRC_MAC_SPS_UL_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sps_ul_info_tlv_len(&p_rrc_mac_sps_config->sps_ul_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_config_tlv_len
(
    rrc_mac_sps_config_t *p_rrc_mac_sps_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_config != PNULL);

    length += rrc_il_get_rrc_mac_sps_config_len(p_rrc_mac_sps_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_config
(
    U8  **pp_buffer,
    rrc_mac_sps_config_t *p_rrc_mac_sps_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sps_config != PNULL);

    /* This function composes rrc_mac_sps_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SPS_CONFIG;
    tlv_length = rrc_il_get_rrc_mac_sps_config_tlv_len(p_rrc_mac_sps_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose TLV  */
    if (p_rrc_mac_sps_config->bitmask & RRC_MAC_SPS_DL_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sps_dl_info(pp_buffer, &p_rrc_mac_sps_config->sps_dl_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_sps_config->bitmask & RRC_MAC_SPS_UL_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sps_ul_info(pp_buffer, &p_rrc_mac_sps_config->sps_ul_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_rf_params_len
(
    rrc_mac_rf_params_t *p_rrc_mac_rf_params
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_rf_params != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_rf_params->halfDuplexFdd);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_rf_params_tlv_len
(
    rrc_mac_rf_params_t *p_rrc_mac_rf_params

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_rf_params != PNULL);

    length += rrc_il_get_rrc_mac_rf_params_len(p_rrc_mac_rf_params);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_rf_params
(
    U8  **pp_buffer,
    rrc_mac_rf_params_t *p_rrc_mac_rf_params
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_rf_params != PNULL);

    /* This function composes rrc_mac_rf_params */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RF_PARAMS;
    tlv_length = rrc_il_get_rrc_mac_rf_params_tlv_len(p_rrc_mac_rf_params);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_rf_params->halfDuplexFdd > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rf_params->halfDuplexFdd] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_rf_params->halfDuplexFdd);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_rf_params->halfDuplexFdd, "halfDuplexFdd");
    *pp_buffer += sizeof(p_rrc_mac_rf_params->halfDuplexFdd);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ue_cat_len
(
    rrc_mac_ue_cat_t *p_rrc_mac_ue_cat
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ue_cat != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ue_cat->ue_category);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ue_cat_tlv_len
(
    rrc_mac_ue_cat_t *p_rrc_mac_ue_cat

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ue_cat != PNULL);

    length += rrc_il_get_rrc_mac_ue_cat_len(p_rrc_mac_ue_cat);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ue_cat
(
    U8  **pp_buffer,
    rrc_mac_ue_cat_t *p_rrc_mac_ue_cat
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ue_cat != PNULL);

    /* This function composes rrc_mac_ue_cat */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UE_CATEGORY_INFO;
    tlv_length = rrc_il_get_rrc_mac_ue_cat_tlv_len(p_rrc_mac_ue_cat);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ue_cat->ue_category < 1) || (p_rrc_mac_ue_cat->ue_category > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_cat->ue_category] should be in range "
            "1 to 5. Incorrect value %u received.", p_rrc_mac_ue_cat->ue_category);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_ue_cat->ue_category, "ue_category");
    *pp_buffer += sizeof(p_rrc_mac_ue_cat->ue_category);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ul_mu_mimo_info_len
(
    rrc_mac_ul_mu_mimo_info_t *p_rrc_mac_ul_mu_mimo_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ul_mu_mimo_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ul_mu_mimo_info->ul_mu_mimo_status);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ul_mu_mimo_info_tlv_len
(
    rrc_mac_ul_mu_mimo_info_t *p_rrc_mac_ul_mu_mimo_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ul_mu_mimo_info != PNULL);

    length += rrc_il_get_rrc_mac_ul_mu_mimo_info_len(p_rrc_mac_ul_mu_mimo_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ul_mu_mimo_info
(
    U8  **pp_buffer,
    rrc_mac_ul_mu_mimo_info_t *p_rrc_mac_ul_mu_mimo_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ul_mu_mimo_info != PNULL);

    /* This function composes rrc_mac_ul_mu_mimo_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UL_MU_MIMO_INFO;
    tlv_length = rrc_il_get_rrc_mac_ul_mu_mimo_info_tlv_len(p_rrc_mac_ul_mu_mimo_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ul_mu_mimo_info->ul_mu_mimo_status > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ul_mu_mimo_info->ul_mu_mimo_status] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_ul_mu_mimo_info->ul_mu_mimo_status);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_ul_mu_mimo_info->ul_mu_mimo_status, "ul_mu_mimo_status");
    *pp_buffer += sizeof(p_rrc_mac_ul_mu_mimo_info->ul_mu_mimo_status);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_phr_timer_info_len
(
    rrc_mac_phr_timer_info_t *p_rrc_mac_phr_timer_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_phr_timer_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_phr_timer_info->periodicPHRTimer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_phr_timer_info->prohibitPHRTimer);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_phr_timer_info_tlv_len
(
    rrc_mac_phr_timer_info_t *p_rrc_mac_phr_timer_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_phr_timer_info != PNULL);

    length += rrc_il_get_rrc_mac_phr_timer_info_len(p_rrc_mac_phr_timer_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_phr_timer_info
(
    U8  **pp_buffer,
    rrc_mac_phr_timer_info_t *p_rrc_mac_phr_timer_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_phr_timer_info != PNULL);

    /* This function composes rrc_mac_phr_timer_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PHR_TIMER_INFO;
    tlv_length = rrc_il_get_rrc_mac_phr_timer_info_tlv_len(p_rrc_mac_phr_timer_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_phr_timer_info->periodicPHRTimer > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_phr_timer_info->periodicPHRTimer] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_phr_timer_info->periodicPHRTimer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_phr_timer_info->periodicPHRTimer, "periodicPHRTimer");
    *pp_buffer += sizeof(p_rrc_mac_phr_timer_info->periodicPHRTimer);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_phr_timer_info->prohibitPHRTimer > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_phr_timer_info->prohibitPHRTimer] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_phr_timer_info->prohibitPHRTimer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_phr_timer_info->prohibitPHRTimer, "prohibitPHRTimer");
    *pp_buffer += sizeof(p_rrc_mac_phr_timer_info->prohibitPHRTimer);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_phr_timer_config_len
(
    rrc_mac_phr_timer_config_t *p_rrc_mac_phr_timer_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_phr_timer_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_phr_timer_config->phrTimerConfigType);

    /* Get length of TLV */
    if (p_rrc_mac_phr_timer_config->optional_elems_present & RRC_MAC_PHR_TIMER_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_phr_timer_info_tlv_len(&p_rrc_mac_phr_timer_config->phrTimerInfo);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_phr_timer_config_tlv_len
(
    rrc_mac_phr_timer_config_t *p_rrc_mac_phr_timer_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_phr_timer_config != PNULL);

    length += rrc_il_get_rrc_mac_phr_timer_config_len(p_rrc_mac_phr_timer_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_phr_timer_config
(
    U8  **pp_buffer,
    rrc_mac_phr_timer_config_t *p_rrc_mac_phr_timer_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_phr_timer_config != PNULL);

    /* This function composes rrc_mac_phr_timer_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PHR_TIMER_CONFIG;
    tlv_length = rrc_il_get_rrc_mac_phr_timer_config_tlv_len(p_rrc_mac_phr_timer_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_phr_timer_config->phrTimerConfigType > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_phr_timer_config->phrTimerConfigType] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_phr_timer_config->phrTimerConfigType);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_phr_timer_config->phrTimerConfigType, "phrTimerConfigType");
    *pp_buffer += sizeof(p_rrc_mac_phr_timer_config->phrTimerConfigType);

    /* Compose TLV  */
    if (p_rrc_mac_phr_timer_config->optional_elems_present & RRC_MAC_PHR_TIMER_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_phr_timer_info(pp_buffer, &p_rrc_mac_phr_timer_config->phrTimerInfo))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_min_ue_power_len
(
    rrc_mac_min_ue_power_t *p_rrc_mac_min_ue_power
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_min_ue_power != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_min_ue_power->minUePower);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_min_ue_power_tlv_len
(
    rrc_mac_min_ue_power_t *p_rrc_mac_min_ue_power

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_min_ue_power != PNULL);

    length += rrc_il_get_rrc_mac_min_ue_power_len(p_rrc_mac_min_ue_power);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_min_ue_power
(
    U8  **pp_buffer,
    rrc_mac_min_ue_power_t *p_rrc_mac_min_ue_power
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_min_ue_power != PNULL);

    /* This function composes rrc_mac_min_ue_power */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MIN_UE_POWER;
    tlv_length = rrc_il_get_rrc_mac_min_ue_power_tlv_len(p_rrc_mac_min_ue_power);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_min_ue_power->minUePower < -40) || (p_rrc_mac_min_ue_power->minUePower > 23))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_min_ue_power->minUePower] should be in range "
            "-40 to 23. Incorrect value %d received.", p_rrc_mac_min_ue_power->minUePower);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_mac_min_ue_power->minUePower, "minUePower");
    *pp_buffer += sizeof(p_rrc_mac_min_ue_power->minUePower);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ue_compliance_info_len
(
    rrc_mac_ue_compliance_info_t *p_rrc_mac_ue_compliance_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ue_compliance_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ue_compliance_info->ue_compliance_release);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ue_compliance_info_tlv_len
(
    rrc_mac_ue_compliance_info_t *p_rrc_mac_ue_compliance_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ue_compliance_info != PNULL);

    length += rrc_il_get_rrc_mac_ue_compliance_info_len(p_rrc_mac_ue_compliance_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ue_compliance_info
(
    U8  **pp_buffer,
    rrc_mac_ue_compliance_info_t *p_rrc_mac_ue_compliance_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ue_compliance_info != PNULL);

    /* This function composes rrc_mac_ue_compliance_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UE_COMPLIANCE_INFO;
    tlv_length = rrc_il_get_rrc_mac_ue_compliance_info_tlv_len(p_rrc_mac_ue_compliance_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ue_compliance_info->ue_compliance_release > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_compliance_info->ue_compliance_release] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_mac_ue_compliance_info->ue_compliance_release);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_ue_compliance_info->ue_compliance_release, "ue_compliance_release");
    *pp_buffer += sizeof(p_rrc_mac_ue_compliance_info->ue_compliance_release);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pdsch_epre_to_ue_rs_ratio_len
(
    rrc_mac_pdsch_epre_to_ue_rs_ratio_t *p_rrc_mac_pdsch_epre_to_ue_rs_ratio
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pdsch_epre_to_ue_rs_ratio != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pdsch_epre_to_ue_rs_ratio->pdsch_epre_to_ue_rs_ratio);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pdsch_epre_to_ue_rs_ratio_tlv_len
(
    rrc_mac_pdsch_epre_to_ue_rs_ratio_t *p_rrc_mac_pdsch_epre_to_ue_rs_ratio

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pdsch_epre_to_ue_rs_ratio != PNULL);

    length += rrc_il_get_rrc_mac_pdsch_epre_to_ue_rs_ratio_len(p_rrc_mac_pdsch_epre_to_ue_rs_ratio);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_pdsch_epre_to_ue_rs_ratio
(
    U8  **pp_buffer,
    rrc_mac_pdsch_epre_to_ue_rs_ratio_t *p_rrc_mac_pdsch_epre_to_ue_rs_ratio
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_pdsch_epre_to_ue_rs_ratio != PNULL);

    /* This function composes rrc_mac_pdsch_epre_to_ue_rs_ratio */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PDSCH_EPRE_TO_UE_RS_RATIO;
    tlv_length = rrc_il_get_rrc_mac_pdsch_epre_to_ue_rs_ratio_tlv_len(p_rrc_mac_pdsch_epre_to_ue_rs_ratio);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_pdsch_epre_to_ue_rs_ratio->pdsch_epre_to_ue_rs_ratio < -6) || (p_rrc_mac_pdsch_epre_to_ue_rs_ratio->pdsch_epre_to_ue_rs_ratio > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pdsch_epre_to_ue_rs_ratio->pdsch_epre_to_ue_rs_ratio] should be in range "
            "-6 to 3. Incorrect value %d received.", p_rrc_mac_pdsch_epre_to_ue_rs_ratio->pdsch_epre_to_ue_rs_ratio);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_mac_pdsch_epre_to_ue_rs_ratio->pdsch_epre_to_ue_rs_ratio, "pdsch_epre_to_ue_rs_ratio");
    *pp_buffer += sizeof(p_rrc_mac_pdsch_epre_to_ue_rs_ratio->pdsch_epre_to_ue_rs_ratio);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tti_bundling_info_len
(
    rrc_mac_tti_bundling_info_t *p_rrc_mac_tti_bundling_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tti_bundling_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tti_bundling_info->tti_bundling);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tti_bundling_info_tlv_len
(
    rrc_mac_tti_bundling_info_t *p_rrc_mac_tti_bundling_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tti_bundling_info != PNULL);

    length += rrc_il_get_rrc_mac_tti_bundling_info_len(p_rrc_mac_tti_bundling_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_tti_bundling_info
(
    U8  **pp_buffer,
    rrc_mac_tti_bundling_info_t *p_rrc_mac_tti_bundling_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_tti_bundling_info != PNULL);

    /* This function composes rrc_mac_tti_bundling_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_TTI_BUNDLING_INFO;
    tlv_length = rrc_il_get_rrc_mac_tti_bundling_info_tlv_len(p_rrc_mac_tti_bundling_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_tti_bundling_info->tti_bundling > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tti_bundling_info->tti_bundling] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_tti_bundling_info->tti_bundling);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_tti_bundling_info->tti_bundling, "tti_bundling");
    *pp_buffer += sizeof(p_rrc_mac_tti_bundling_info->tti_bundling);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_operator_info_len
(
    rrc_mac_operator_info_t *p_rrc_mac_operator_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_operator_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_operator_info->operator_id);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_operator_info_tlv_len
(
    rrc_mac_operator_info_t *p_rrc_mac_operator_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_operator_info != PNULL);

    length += rrc_il_get_rrc_mac_operator_info_len(p_rrc_mac_operator_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_operator_info
(
    U8  **pp_buffer,
    rrc_mac_operator_info_t *p_rrc_mac_operator_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_operator_info != PNULL);

    /* This function composes rrc_mac_operator_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_OPERATOR_INFO;
    tlv_length = rrc_il_get_rrc_mac_operator_info_tlv_len(p_rrc_mac_operator_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_operator_info->operator_id > MAX_MOCN_OPERATOR_ID))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_operator_info->operator_id] should be less than"
            " or equal to MAX_MOCN_OPERATOR_ID. Incorrect value %u received.", p_rrc_mac_operator_info->operator_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_operator_info->operator_id, "operator_id");
    *pp_buffer += sizeof(p_rrc_mac_operator_info->operator_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_codebook_subset_restriction_v1020_len
(
    rrc_mac_codebook_subset_restriction_v1020_t *p_rrc_mac_codebook_subset_restriction_v1020
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_codebook_subset_restriction_v1020 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_codebook_subset_restriction_v1020->cbsr_type);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_mac_codebook_subset_restriction_v1020->cbsr_value);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_codebook_subset_restriction_v1020_tlv_len
(
    rrc_mac_codebook_subset_restriction_v1020_t *p_rrc_mac_codebook_subset_restriction_v1020

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_codebook_subset_restriction_v1020 != PNULL);

    length += rrc_il_get_rrc_mac_codebook_subset_restriction_v1020_len(p_rrc_mac_codebook_subset_restriction_v1020);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_codebook_subset_restriction_v1020
(
    U8  **pp_buffer,
    rrc_mac_codebook_subset_restriction_v1020_t *p_rrc_mac_codebook_subset_restriction_v1020
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_codebook_subset_restriction_v1020 != PNULL);

    /* This function composes rrc_mac_codebook_subset_restriction_v1020 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020;
    tlv_length = rrc_il_get_rrc_mac_codebook_subset_restriction_v1020_tlv_len(p_rrc_mac_codebook_subset_restriction_v1020);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_codebook_subset_restriction_v1020->cbsr_type > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_codebook_subset_restriction_v1020->cbsr_type] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_codebook_subset_restriction_v1020->cbsr_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_codebook_subset_restriction_v1020->cbsr_type, "cbsr_type");
    *pp_buffer += sizeof(p_rrc_mac_codebook_subset_restriction_v1020->cbsr_type);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_codebook_subset_restriction_v1020->cbsr_value); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_codebook_subset_restriction_v1020->cbsr_value[loop], "cbsr_value[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_interference_info_len
(
    rrc_mac_dl_interference_info_t *p_rrc_mac_dl_interference_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_interference_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_dl_interference_info->severe_dl_control_channel_interference);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_interference_info_tlv_len
(
    rrc_mac_dl_interference_info_t *p_rrc_mac_dl_interference_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_interference_info != PNULL);

    length += rrc_il_get_rrc_mac_dl_interference_info_len(p_rrc_mac_dl_interference_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_interference_info
(
    U8  **pp_buffer,
    rrc_mac_dl_interference_info_t *p_rrc_mac_dl_interference_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_dl_interference_info != PNULL);

    /* This function composes rrc_mac_dl_interference_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DL_INTERFERENCE_INFO;
    tlv_length = rrc_il_get_rrc_mac_dl_interference_info_tlv_len(p_rrc_mac_dl_interference_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_dl_interference_info->severe_dl_control_channel_interference > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_interference_info->severe_dl_control_channel_interference] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_dl_interference_info->severe_dl_control_channel_interference);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_dl_interference_info->severe_dl_control_channel_interference, "severe_dl_control_channel_interference");
    *pp_buffer += sizeof(p_rrc_mac_dl_interference_info->severe_dl_control_channel_interference);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcAckNackRepetitionSetupR13_len
(
    RrcAckNackRepetitionSetupR13_t *p_RrcAckNackRepetitionSetupR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcAckNackRepetitionSetupR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcAckNackRepetitionSetupR13->repetitionFactorR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcAckNackRepetitionSetupR13->n1PucchAnRepR13);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcAckNackRepetitionSetupR13_tlv_len
(
    RrcAckNackRepetitionSetupR13_t *p_RrcAckNackRepetitionSetupR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcAckNackRepetitionSetupR13 != PNULL);

    length += rrc_il_get_RrcAckNackRepetitionSetupR13_len(p_RrcAckNackRepetitionSetupR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcAckNackRepetitionSetupR13
(
    U8  **pp_buffer,
    RrcAckNackRepetitionSetupR13_t *p_RrcAckNackRepetitionSetupR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcAckNackRepetitionSetupR13 != PNULL);

    /* This function composes RrcAckNackRepetitionSetupR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_ACK_NACK_REPETITION_SETUP_R13;
    tlv_length = rrc_il_get_RrcAckNackRepetitionSetupR13_tlv_len(p_RrcAckNackRepetitionSetupR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcAckNackRepetitionSetupR13->repetitionFactorR13 > EMTC_REPETITION_FACTOR_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcAckNackRepetitionSetupR13->repetitionFactorR13] should be less than"
            " or equal to EMTC_REPETITION_FACTOR_MAX. Incorrect value %u received.", p_RrcAckNackRepetitionSetupR13->repetitionFactorR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcAckNackRepetitionSetupR13->repetitionFactorR13, "repetitionFactorR13");
    *pp_buffer += sizeof(p_RrcAckNackRepetitionSetupR13->repetitionFactorR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcAckNackRepetitionSetupR13->n1PucchAnRepR13 > EMTC_MAX_N1_PUCCH_AN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcAckNackRepetitionSetupR13->n1PucchAnRepR13] should be less than"
            " or equal to EMTC_MAX_N1_PUCCH_AN. Incorrect value %u received.", p_RrcAckNackRepetitionSetupR13->n1PucchAnRepR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_RrcAckNackRepetitionSetupR13->n1PucchAnRepR13, "n1PucchAnRepR13");
    *pp_buffer += sizeof(p_RrcAckNackRepetitionSetupR13->n1PucchAnRepR13);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacPucchAckNackRepetitionR13_len
(
    RrcMacPucchAckNackRepetitionR13_t *p_RrcMacPucchAckNackRepetitionR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPucchAckNackRepetitionR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitiontype);

    /* Get length of TLV */
    if (p_RrcMacPucchAckNackRepetitionR13->bitmask & RRC_MAC_PUCCH_ACK_NACK_REPETITION_SETUP_R13_PRESENT)
    {
        length += rrc_il_get_RrcAckNackRepetitionSetupR13_tlv_len(&p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitionSetup);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacPucchAckNackRepetitionR13_tlv_len
(
    RrcMacPucchAckNackRepetitionR13_t *p_RrcMacPucchAckNackRepetitionR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPucchAckNackRepetitionR13 != PNULL);

    length += rrc_il_get_RrcMacPucchAckNackRepetitionR13_len(p_RrcMacPucchAckNackRepetitionR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacPucchAckNackRepetitionR13
(
    U8  **pp_buffer,
    RrcMacPucchAckNackRepetitionR13_t *p_RrcMacPucchAckNackRepetitionR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacPucchAckNackRepetitionR13 != PNULL);

    /* This function composes RrcMacPucchAckNackRepetitionR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_ACK_NACK_REPETITION_R13;
    tlv_length = rrc_il_get_RrcMacPucchAckNackRepetitionR13_tlv_len(p_RrcMacPucchAckNackRepetitionR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitiontype > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitiontype] should be less than"
            " or equal to 1. Incorrect value %u received.", p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitiontype);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitiontype, "ackNackRepetitiontype");
    *pp_buffer += sizeof(p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitiontype);

    /* Compose TLV  */
    if (p_RrcMacPucchAckNackRepetitionR13->bitmask & RRC_MAC_PUCCH_ACK_NACK_REPETITION_SETUP_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcAckNackRepetitionSetupR13(pp_buffer, &p_RrcMacPucchAckNackRepetitionR13->ackNackRepetitionSetup))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcPucchNumRepetitionCeR13SetupModeA_len
(
    RrcPucchNumRepetitionCeR13SetupModeA_t *p_RrcPucchNumRepetitionCeR13SetupModeA
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPucchNumRepetitionCeR13SetupModeA != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat1);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat2);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcPucchNumRepetitionCeR13SetupModeA_tlv_len
(
    RrcPucchNumRepetitionCeR13SetupModeA_t *p_RrcPucchNumRepetitionCeR13SetupModeA

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcPucchNumRepetitionCeR13SetupModeA != PNULL);

    length += rrc_il_get_RrcPucchNumRepetitionCeR13SetupModeA_len(p_RrcPucchNumRepetitionCeR13SetupModeA);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcPucchNumRepetitionCeR13SetupModeA
(
    U8  **pp_buffer,
    RrcPucchNumRepetitionCeR13SetupModeA_t *p_RrcPucchNumRepetitionCeR13SetupModeA
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcPucchNumRepetitionCeR13SetupModeA != PNULL);

    /* This function composes RrcPucchNumRepetitionCeR13SetupModeA */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PUCCH_NUM_REPETITION_CE_SETUP_R13;
    tlv_length = rrc_il_get_RrcPucchNumRepetitionCeR13SetupModeA_tlv_len(p_RrcPucchNumRepetitionCeR13SetupModeA);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat1 > EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat1] should be less than"
            " or equal to EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX. Incorrect value %u received.", p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat1);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat1, "numRepetitionCeModeAFormat1");
    *pp_buffer += sizeof(p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat1);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat2 > EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT2_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat2] should be less than"
            " or equal to EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT2_MAX. Incorrect value %u received.", p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat2);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat2, "numRepetitionCeModeAFormat2");
    *pp_buffer += sizeof(p_RrcPucchNumRepetitionCeR13SetupModeA->numRepetitionCeModeAFormat2);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacPucchNumRepetitionCeR13_len
(
    RrcMacPucchNumRepetitionCeR13_t *p_RrcMacPucchNumRepetitionCeR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPucchNumRepetitionCeR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPucchNumRepetitionCeR13->numRepetitionType);

    /* Get length of TLV */
    if (p_RrcMacPucchNumRepetitionCeR13->bitmask & RRC_MAC_PUCCH_NUM_REPETITION_CE_SETUP_R13_PRESENT)
    {
        length += rrc_il_get_RrcPucchNumRepetitionCeR13SetupModeA_tlv_len(&p_RrcMacPucchNumRepetitionCeR13->setup);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacPucchNumRepetitionCeR13_tlv_len
(
    RrcMacPucchNumRepetitionCeR13_t *p_RrcMacPucchNumRepetitionCeR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPucchNumRepetitionCeR13 != PNULL);

    length += rrc_il_get_RrcMacPucchNumRepetitionCeR13_len(p_RrcMacPucchNumRepetitionCeR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacPucchNumRepetitionCeR13
(
    U8  **pp_buffer,
    RrcMacPucchNumRepetitionCeR13_t *p_RrcMacPucchNumRepetitionCeR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacPucchNumRepetitionCeR13 != PNULL);

    /* This function composes RrcMacPucchNumRepetitionCeR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PUCCH_NUM_REPETITION_CE_R13;
    tlv_length = rrc_il_get_RrcMacPucchNumRepetitionCeR13_tlv_len(p_RrcMacPucchNumRepetitionCeR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacPucchNumRepetitionCeR13->numRepetitionType > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPucchNumRepetitionCeR13->numRepetitionType] should be less than"
            " or equal to 1. Incorrect value %u received.", p_RrcMacPucchNumRepetitionCeR13->numRepetitionType);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPucchNumRepetitionCeR13->numRepetitionType, "numRepetitionType");
    *pp_buffer += sizeof(p_RrcMacPucchNumRepetitionCeR13->numRepetitionType);

    /* Compose TLV  */
    if (p_RrcMacPucchNumRepetitionCeR13->bitmask & RRC_MAC_PUCCH_NUM_REPETITION_CE_SETUP_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcPucchNumRepetitionCeR13SetupModeA(pp_buffer, &p_RrcMacPucchNumRepetitionCeR13->setup))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_PucchConfigDedicatedR13_len
(
    PucchConfigDedicatedR13_t *p_PucchConfigDedicatedR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_PucchConfigDedicatedR13 != PNULL);

    /* Get length of TLV */
    if (p_PucchConfigDedicatedR13->bitmask & RRC_MAC_ACK_NACK_REPETITION_R13_PRESENT)
    {
        length += rrc_il_get_RrcMacPucchAckNackRepetitionR13_tlv_len(&p_PucchConfigDedicatedR13->ackNackRepetitionR13);
    }

    /* Get length of TLV */
    if (p_PucchConfigDedicatedR13->bitmask & RRC_MAC_PUCCH_NUM_REPETITION_CE_R13_PRESENT)
    {
        length += rrc_il_get_RrcMacPucchNumRepetitionCeR13_tlv_len(&p_PucchConfigDedicatedR13->pucchNumRepetitionCeR13);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_PucchConfigDedicatedR13_tlv_len
(
    PucchConfigDedicatedR13_t *p_PucchConfigDedicatedR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_PucchConfigDedicatedR13 != PNULL);

    length += rrc_il_get_PucchConfigDedicatedR13_len(p_PucchConfigDedicatedR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_PucchConfigDedicatedR13
(
    U8  **pp_buffer,
    PucchConfigDedicatedR13_t *p_PucchConfigDedicatedR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_PucchConfigDedicatedR13 != PNULL);

    /* This function composes PucchConfigDedicatedR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PUCCH_CONFIG_DEDICATED_R13;
    tlv_length = rrc_il_get_PucchConfigDedicatedR13_tlv_len(p_PucchConfigDedicatedR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose TLV  */
    if (p_PucchConfigDedicatedR13->bitmask & RRC_MAC_ACK_NACK_REPETITION_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacPucchAckNackRepetitionR13(pp_buffer, &p_PucchConfigDedicatedR13->ackNackRepetitionR13))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_PucchConfigDedicatedR13->bitmask & RRC_MAC_PUCCH_NUM_REPETITION_CE_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacPucchNumRepetitionCeR13(pp_buffer, &p_PucchConfigDedicatedR13->pucchNumRepetitionCeR13))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_MpdcchConfigSetupR13_len
(
    MpdcchConfigSetupR13_t *p_MpdcchConfigSetupR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_MpdcchConfigSetupR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_MpdcchConfigSetupR13->csiNumRepetitionCeR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_MpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_MpdcchConfigSetupR13->mpdcchNumRepetitionR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_MpdcchConfigSetupR13->mpdcchNarrowbandR13);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_MpdcchConfigSetupR13->mpdcchStartSfUessFddType);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_MpdcchConfigSetupR13->mpdcchStartSfUessFddR13);

    return length;
}

static
rrc_length_t
rrc_il_get_MpdcchConfigSetupR13_tlv_len
(
    MpdcchConfigSetupR13_t *p_MpdcchConfigSetupR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_MpdcchConfigSetupR13 != PNULL);

    length += rrc_il_get_MpdcchConfigSetupR13_len(p_MpdcchConfigSetupR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_MpdcchConfigSetupR13
(
    U8  **pp_buffer,
    MpdcchConfigSetupR13_t *p_MpdcchConfigSetupR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_MpdcchConfigSetupR13 != PNULL);

    /* This function composes MpdcchConfigSetupR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MPDCCH_CONFIG_SETUP_R13;
    tlv_length = rrc_il_get_MpdcchConfigSetupR13_tlv_len(p_MpdcchConfigSetupR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigSetupR13->csiNumRepetitionCeR13 > EMTC_CSI_NUM_REPETITION_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigSetupR13->csiNumRepetitionCeR13] should be less than"
            " or equal to EMTC_CSI_NUM_REPETITION_MAX. Incorrect value %u received.", p_MpdcchConfigSetupR13->csiNumRepetitionCeR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_MpdcchConfigSetupR13->csiNumRepetitionCeR13, "csiNumRepetitionCeR13");
    *pp_buffer += sizeof(p_MpdcchConfigSetupR13->csiNumRepetitionCeR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13 > EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13] should be less than"
            " or equal to EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX. Incorrect value %u received.", p_MpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_MpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13, "mpdcchPdschHoppingConfigR13");
    *pp_buffer += sizeof(p_MpdcchConfigSetupR13->mpdcchPdschHoppingConfigR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigSetupR13->mpdcchNumRepetitionR13 > EMTC_MPDCCH_NUM_REPETITION_UESS_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigSetupR13->mpdcchNumRepetitionR13] should be less than"
            " or equal to EMTC_MPDCCH_NUM_REPETITION_UESS_MAX. Incorrect value %u received.", p_MpdcchConfigSetupR13->mpdcchNumRepetitionR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_MpdcchConfigSetupR13->mpdcchNumRepetitionR13, "mpdcchNumRepetitionR13");
    *pp_buffer += sizeof(p_MpdcchConfigSetupR13->mpdcchNumRepetitionR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigSetupR13->mpdcchNarrowbandR13 > EMTC_MAX_AVAILABLE_NARROWBAND))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigSetupR13->mpdcchNarrowbandR13] should be less than"
            " or equal to EMTC_MAX_AVAILABLE_NARROWBAND. Incorrect value %u received.", p_MpdcchConfigSetupR13->mpdcchNarrowbandR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_MpdcchConfigSetupR13->mpdcchNarrowbandR13, "mpdcchNarrowbandR13");
    *pp_buffer += sizeof(p_MpdcchConfigSetupR13->mpdcchNarrowbandR13);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigSetupR13->mpdcchStartSfUessFddType > RRC_ONE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigSetupR13->mpdcchStartSfUessFddType] should be less than"
            " or equal to RRC_ONE. Incorrect value %u received.", p_MpdcchConfigSetupR13->mpdcchStartSfUessFddType);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_MpdcchConfigSetupR13->mpdcchStartSfUessFddType, "mpdcchStartSfUessFddType");
    *pp_buffer += sizeof(p_MpdcchConfigSetupR13->mpdcchStartSfUessFddType);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigSetupR13->mpdcchStartSfUessFddR13 > EMTC_MPDCCH_START_SF_UESS_FDD_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigSetupR13->mpdcchStartSfUessFddR13] should be less than"
            " or equal to EMTC_MPDCCH_START_SF_UESS_FDD_MAX. Incorrect value %u received.", p_MpdcchConfigSetupR13->mpdcchStartSfUessFddR13);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_MpdcchConfigSetupR13->mpdcchStartSfUessFddR13, "mpdcchStartSfUessFddR13");
    *pp_buffer += sizeof(p_MpdcchConfigSetupR13->mpdcchStartSfUessFddR13);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_MpdcchConfigR13_len
(
    MpdcchConfigR13_t *p_MpdcchConfigR13
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_MpdcchConfigR13 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_MpdcchConfigR13->mpdcchConfigType);

    /* Get length of TLV */
    if (p_MpdcchConfigR13->bitmask & RRC_MAC_MPDCCH_CONFIG_SETUP_PRESENT)
    {
        length += rrc_il_get_MpdcchConfigSetupR13_tlv_len(&p_MpdcchConfigR13->mpdcchConfigSetupR13);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_MpdcchConfigR13_tlv_len
(
    MpdcchConfigR13_t *p_MpdcchConfigR13

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_MpdcchConfigR13 != PNULL);

    length += rrc_il_get_MpdcchConfigR13_len(p_MpdcchConfigR13);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_MpdcchConfigR13
(
    U8  **pp_buffer,
    MpdcchConfigR13_t *p_MpdcchConfigR13
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_MpdcchConfigR13 != PNULL);

    /* This function composes MpdcchConfigR13 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MPDCCH_CONFIG_R13;
    tlv_length = rrc_il_get_MpdcchConfigR13_tlv_len(p_MpdcchConfigR13);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_MpdcchConfigR13->mpdcchConfigType > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_MpdcchConfigR13->mpdcchConfigType] should be less than"
            " or equal to 1. Incorrect value %u received.", p_MpdcchConfigR13->mpdcchConfigType);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_MpdcchConfigR13->mpdcchConfigType, "mpdcchConfigType");
    *pp_buffer += sizeof(p_MpdcchConfigR13->mpdcchConfigType);

    /* Compose TLV  */
    if (p_MpdcchConfigR13->bitmask & RRC_MAC_MPDCCH_CONFIG_SETUP_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_MpdcchConfigSetupR13(pp_buffer, &p_MpdcchConfigR13->mpdcchConfigSetupR13))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_EpdcchSetConfigR11_len
(
    EpdcchSetConfigR11_t *p_EpdcchSetConfigR11
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_EpdcchSetConfigR11 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_EpdcchSetConfigR11->setConfigIdR11);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_EpdcchSetConfigR11->transmissionTypeR11);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_EpdcchSetConfigR11->numberPrbPairsR11);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_EpdcchSetConfigR11->resourceBlockAssignmentR11);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_EpdcchSetConfigR11->resourceBlockAssignBitmap);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_EpdcchSetConfigR11->dmrsScramblingSequenceIntR11);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_EpdcchSetConfigR11->pucchResourceStartOffsetR11);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_EpdcchSetConfigR11->startSymbol);

    /* Get length of TLV */
    if (p_EpdcchSetConfigR11->bitmask & RRC_MAC_MPDCCH_CONFIG_R13_PRESENT)
    {
        length += rrc_il_get_MpdcchConfigR13_tlv_len(&p_EpdcchSetConfigR11->mpdcchConfigR13);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_EpdcchSetConfigR11_tlv_len
(
    EpdcchSetConfigR11_t *p_EpdcchSetConfigR11

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_EpdcchSetConfigR11 != PNULL);

    length += rrc_il_get_EpdcchSetConfigR11_len(p_EpdcchSetConfigR11);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_EpdcchSetConfigR11
(
    U8  **pp_buffer,
    EpdcchSetConfigR11_t *p_EpdcchSetConfigR11
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_EpdcchSetConfigR11 != PNULL);

    /* This function composes EpdcchSetConfigR11 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_EPDCCH_SET_CONFIG_R11;
    tlv_length = rrc_il_get_EpdcchSetConfigR11_tlv_len(p_EpdcchSetConfigR11);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->setConfigIdR11 > EMTC_MAX_EPDCCH_CONFIG_ID))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->setConfigIdR11] should be less than"
            " or equal to EMTC_MAX_EPDCCH_CONFIG_ID. Incorrect value %u received.", p_EpdcchSetConfigR11->setConfigIdR11);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_EpdcchSetConfigR11->setConfigIdR11, "setConfigIdR11");
    *pp_buffer += sizeof(p_EpdcchSetConfigR11->setConfigIdR11);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->transmissionTypeR11 > EMTC_TRANSMISSION_TYPE_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->transmissionTypeR11] should be less than"
            " or equal to EMTC_TRANSMISSION_TYPE_MAX. Incorrect value %u received.", p_EpdcchSetConfigR11->transmissionTypeR11);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_EpdcchSetConfigR11->transmissionTypeR11, "transmissionTypeR11");
    *pp_buffer += sizeof(p_EpdcchSetConfigR11->transmissionTypeR11);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->numberPrbPairsR11 > EMTC_NUM_PRB_PAIR_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->numberPrbPairsR11] should be less than"
            " or equal to EMTC_NUM_PRB_PAIR_MAX. Incorrect value %u received.", p_EpdcchSetConfigR11->numberPrbPairsR11);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_EpdcchSetConfigR11->numberPrbPairsR11, "numberPrbPairsR11");
    *pp_buffer += sizeof(p_EpdcchSetConfigR11->numberPrbPairsR11);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->resourceBlockAssignmentR11 > 14))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->resourceBlockAssignmentR11] should be less than"
            " or equal to 14. Incorrect value %u received.", p_EpdcchSetConfigR11->resourceBlockAssignmentR11);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_EpdcchSetConfigR11->resourceBlockAssignmentR11, "resourceBlockAssignmentR11");
    *pp_buffer += sizeof(p_EpdcchSetConfigR11->resourceBlockAssignmentR11);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->resourceBlockAssignBitmap > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->resourceBlockAssignBitmap] should be less than"
            " or equal to 63. Incorrect value %u received.", p_EpdcchSetConfigR11->resourceBlockAssignBitmap);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_EpdcchSetConfigR11->resourceBlockAssignBitmap, "resourceBlockAssignBitmap");
    *pp_buffer += sizeof(p_EpdcchSetConfigR11->resourceBlockAssignBitmap);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->dmrsScramblingSequenceIntR11 > EMTC_MAX_DMRS_SCRAMBLING_SEQUENCE_INT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->dmrsScramblingSequenceIntR11] should be less than"
            " or equal to EMTC_MAX_DMRS_SCRAMBLING_SEQUENCE_INT. Incorrect value %u received.", p_EpdcchSetConfigR11->dmrsScramblingSequenceIntR11);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_EpdcchSetConfigR11->dmrsScramblingSequenceIntR11, "dmrsScramblingSequenceIntR11");
    *pp_buffer += sizeof(p_EpdcchSetConfigR11->dmrsScramblingSequenceIntR11);

    /* Check for correct range [H - higher boundary] */
    if ((p_EpdcchSetConfigR11->pucchResourceStartOffsetR11 > EMTC_MAX_PUCCH_RESOURCE_START_OFFSET))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->pucchResourceStartOffsetR11] should be less than"
            " or equal to EMTC_MAX_PUCCH_RESOURCE_START_OFFSET. Incorrect value %u received.", p_EpdcchSetConfigR11->pucchResourceStartOffsetR11);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_EpdcchSetConfigR11->pucchResourceStartOffsetR11, "pucchResourceStartOffsetR11");
    *pp_buffer += sizeof(p_EpdcchSetConfigR11->pucchResourceStartOffsetR11);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_EpdcchSetConfigR11->startSymbol < 1) || (p_EpdcchSetConfigR11->startSymbol > EMTC_MAX_START_SYMBOL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_EpdcchSetConfigR11->startSymbol] should be in range "
            "1 to EMTC_MAX_START_SYMBOL. Incorrect value %u received.", p_EpdcchSetConfigR11->startSymbol);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_EpdcchSetConfigR11->startSymbol, "startSymbol");
    *pp_buffer += sizeof(p_EpdcchSetConfigR11->startSymbol);

    /* Compose TLV  */
    if (p_EpdcchSetConfigR11->bitmask & RRC_MAC_MPDCCH_CONFIG_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_MpdcchConfigR13(pp_buffer, &p_EpdcchSetConfigR11->mpdcchConfigR13))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacCeLevel_len
(
    RrcMacCeLevel_t *p_RrcMacCeLevel
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacCeLevel != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacCeLevel->ceLevel);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacCeLevel_tlv_len
(
    RrcMacCeLevel_t *p_RrcMacCeLevel

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacCeLevel != PNULL);

    length += rrc_il_get_RrcMacCeLevel_len(p_RrcMacCeLevel);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacCeLevel
(
    U8  **pp_buffer,
    RrcMacCeLevel_t *p_RrcMacCeLevel
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacCeLevel != PNULL);

    /* This function composes RrcMacCeLevel */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CE_LEVEL;
    tlv_length = rrc_il_get_RrcMacCeLevel_tlv_len(p_RrcMacCeLevel);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacCeLevel->ceLevel > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacCeLevel->ceLevel] should be less than"
            " or equal to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcMacCeLevel->ceLevel);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacCeLevel->ceLevel, "ceLevel");
    *pp_buffer += sizeof(p_RrcMacCeLevel->ceLevel);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacCeMode_len
(
    RrcMacCeMode_t *p_RrcMacCeMode
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacCeMode != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacCeMode->ceMode);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacCeMode_tlv_len
(
    RrcMacCeMode_t *p_RrcMacCeMode

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacCeMode != PNULL);

    length += rrc_il_get_RrcMacCeMode_len(p_RrcMacCeMode);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacCeMode
(
    U8  **pp_buffer,
    RrcMacCeMode_t *p_RrcMacCeMode
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacCeMode != PNULL);

    /* This function composes RrcMacCeMode */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CE_MODE;
    tlv_length = rrc_il_get_RrcMacCeMode_tlv_len(p_RrcMacCeMode);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacCeMode->ceMode > EMTC_CE_MODE_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacCeMode->ceMode] should be less than"
            " or equal to EMTC_CE_MODE_MAX. Incorrect value %u received.", p_RrcMacCeMode->ceMode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacCeMode->ceMode, "ceMode");
    *pp_buffer += sizeof(p_RrcMacCeMode->ceMode);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ue_category_v1020_len
(
    rrc_mac_ue_category_v1020_t *p_rrc_mac_ue_category_v1020
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ue_category_v1020 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ue_category_v1020->ue_category_v1020);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ue_category_v1020_tlv_len
(
    rrc_mac_ue_category_v1020_t *p_rrc_mac_ue_category_v1020

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ue_category_v1020 != PNULL);

    length += rrc_il_get_rrc_mac_ue_category_v1020_len(p_rrc_mac_ue_category_v1020);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ue_category_v1020
(
    U8  **pp_buffer,
    rrc_mac_ue_category_v1020_t *p_rrc_mac_ue_category_v1020
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ue_category_v1020 != PNULL);

    /* This function composes rrc_mac_ue_category_v1020 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UE_CATEGORY_V1020;
    tlv_length = rrc_il_get_rrc_mac_ue_category_v1020_tlv_len(p_rrc_mac_ue_category_v1020);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ue_category_v1020->ue_category_v1020 < 6) || (p_rrc_mac_ue_category_v1020->ue_category_v1020 > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_category_v1020->ue_category_v1020] should be in range "
            "6 to 8. Incorrect value %u received.", p_rrc_mac_ue_category_v1020->ue_category_v1020);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_ue_category_v1020->ue_category_v1020, "ue_category_v1020");
    *pp_buffer += sizeof(p_rrc_mac_ue_category_v1020->ue_category_v1020);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_n1pucch_an_cs_r10_len
(
    rrc_mac_n1pucch_an_cs_r10_t *p_rrc_mac_n1pucch_an_cs_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_n1pucch_an_cs_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_n1pucch_an_cs_r10->rrc_mac_n1pucch_an_cs);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_n1pucch_an_cs_r10_tlv_len
(
    rrc_mac_n1pucch_an_cs_r10_t *p_rrc_mac_n1pucch_an_cs_r10

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_n1pucch_an_cs_r10 != PNULL);

    length += rrc_il_get_rrc_mac_n1pucch_an_cs_r10_len(p_rrc_mac_n1pucch_an_cs_r10);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_n1pucch_an_cs_r10
(
    U8  **pp_buffer,
    rrc_mac_n1pucch_an_cs_r10_t *p_rrc_mac_n1pucch_an_cs_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_n1pucch_an_cs_r10 != PNULL);

    /* This function composes rrc_mac_n1pucch_an_cs_r10 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PUCCH_AN_CS_V1020;
    tlv_length = rrc_il_get_rrc_mac_n1pucch_an_cs_r10_tlv_len(p_rrc_mac_n1pucch_an_cs_r10);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_n1pucch_an_cs_r10->rrc_mac_n1pucch_an_cs > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_n1pucch_an_cs_r10->rrc_mac_n1pucch_an_cs] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_rrc_mac_n1pucch_an_cs_r10->rrc_mac_n1pucch_an_cs);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_n1pucch_an_cs_r10->rrc_mac_n1pucch_an_cs, "rrc_mac_n1pucch_an_cs");
    *pp_buffer += sizeof(p_rrc_mac_n1pucch_an_cs_r10->rrc_mac_n1pucch_an_cs);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_n1pucch_an_cs_list_r10_len
(
    rrc_mac_n1pucch_an_cs_list_r10_t *p_rrc_mac_n1pucch_an_cs_list_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_n1pucch_an_cs_list_r10 != PNULL);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_n1pucch_an_cs_list_r10->count <= ARRSIZE(p_rrc_mac_n1pucch_an_cs_list_r10->rrc_mac_n1pucch_an_cs_r10));
        for (loop = 0; loop < p_rrc_mac_n1pucch_an_cs_list_r10->count; loop++)
        {
            length += rrc_il_get_rrc_mac_n1pucch_an_cs_r10_tlv_len(&p_rrc_mac_n1pucch_an_cs_list_r10->rrc_mac_n1pucch_an_cs_r10[loop]);
        }
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_n1pucch_an_cs_list_r10_tlv_len
(
    rrc_mac_n1pucch_an_cs_list_r10_t *p_rrc_mac_n1pucch_an_cs_list_r10

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_n1pucch_an_cs_list_r10 != PNULL);

    length += rrc_il_get_rrc_mac_n1pucch_an_cs_list_r10_len(p_rrc_mac_n1pucch_an_cs_list_r10);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_n1pucch_an_cs_list_r10
(
    U8  **pp_buffer,
    rrc_mac_n1pucch_an_cs_list_r10_t *p_rrc_mac_n1pucch_an_cs_list_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_n1pucch_an_cs_list_r10 != PNULL);

    /* This function composes rrc_mac_n1pucch_an_cs_list_r10 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_N1PUCCH_AN_CS_LIST_R10;
    tlv_length = rrc_il_get_rrc_mac_n1pucch_an_cs_list_r10_tlv_len(p_rrc_mac_n1pucch_an_cs_list_r10);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_n1pucch_an_cs_list_r10->count <= ARRSIZE(p_rrc_mac_n1pucch_an_cs_list_r10->rrc_mac_n1pucch_an_cs_r10));
        for (loop = 0; loop < p_rrc_mac_n1pucch_an_cs_list_r10->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_n1pucch_an_cs_r10(pp_buffer, &p_rrc_mac_n1pucch_an_cs_list_r10->rrc_mac_n1pucch_an_cs_r10[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_channel_selection_config_v1020_len
(
    rrc_mac_pucch_channel_selection_config_v1020_t *p_rrc_mac_pucch_channel_selection_config_v1020
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pucch_channel_selection_config_v1020 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pucch_channel_selection_config_v1020->request_type);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_pucch_channel_selection_config_v1020->count <= ARRSIZE(p_rrc_mac_pucch_channel_selection_config_v1020->rrc_mac_n1pucch_an_cs_list_r10));
        for (loop = 0; loop < p_rrc_mac_pucch_channel_selection_config_v1020->count; loop++)
        {
            length += rrc_il_get_rrc_mac_n1pucch_an_cs_list_r10_tlv_len(&p_rrc_mac_pucch_channel_selection_config_v1020->rrc_mac_n1pucch_an_cs_list_r10[loop]);
        }
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_channel_selection_config_v1020_tlv_len
(
    rrc_mac_pucch_channel_selection_config_v1020_t *p_rrc_mac_pucch_channel_selection_config_v1020

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pucch_channel_selection_config_v1020 != PNULL);

    length += rrc_il_get_rrc_mac_pucch_channel_selection_config_v1020_len(p_rrc_mac_pucch_channel_selection_config_v1020);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_pucch_channel_selection_config_v1020
(
    U8  **pp_buffer,
    rrc_mac_pucch_channel_selection_config_v1020_t *p_rrc_mac_pucch_channel_selection_config_v1020
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_pucch_channel_selection_config_v1020 != PNULL);

    /* This function composes rrc_mac_pucch_channel_selection_config_v1020 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PUCCH_CHANNEL_SELECTION_CONFIG_V1020;
    tlv_length = rrc_il_get_rrc_mac_pucch_channel_selection_config_v1020_tlv_len(p_rrc_mac_pucch_channel_selection_config_v1020);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pucch_channel_selection_config_v1020->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pucch_channel_selection_config_v1020->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_pucch_channel_selection_config_v1020->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pucch_channel_selection_config_v1020->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_mac_pucch_channel_selection_config_v1020->request_type);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_pucch_channel_selection_config_v1020->count <= ARRSIZE(p_rrc_mac_pucch_channel_selection_config_v1020->rrc_mac_n1pucch_an_cs_list_r10));
        for (loop = 0; loop < p_rrc_mac_pucch_channel_selection_config_v1020->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_n1pucch_an_cs_list_r10(pp_buffer, &p_rrc_mac_pucch_channel_selection_config_v1020->rrc_mac_n1pucch_an_cs_list_r10[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_format_r10_len
(
    rrc_mac_pucch_format_r10_t *p_rrc_mac_pucch_format_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pucch_format_r10 != PNULL);

    /* Get length of TLV */
    if (p_rrc_mac_pucch_format_r10->optional_elems_present & RRC_MAC_PUCCH_CHANNEL_SELECTION_CONFIG_V1020_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pucch_channel_selection_config_v1020_tlv_len(&p_rrc_mac_pucch_format_r10->rrc_mac_pucch_channel_selection_config_v1020);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pucch_format_r10_tlv_len
(
    rrc_mac_pucch_format_r10_t *p_rrc_mac_pucch_format_r10

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pucch_format_r10 != PNULL);

    length += rrc_il_get_rrc_mac_pucch_format_r10_len(p_rrc_mac_pucch_format_r10);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_pucch_format_r10
(
    U8  **pp_buffer,
    rrc_mac_pucch_format_r10_t *p_rrc_mac_pucch_format_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_pucch_format_r10 != PNULL);

    /* This function composes rrc_mac_pucch_format_r10 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PUCCH_FORMAT_V1020;
    tlv_length = rrc_il_get_rrc_mac_pucch_format_r10_tlv_len(p_rrc_mac_pucch_format_r10);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose TLV  */
    if (p_rrc_mac_pucch_format_r10->optional_elems_present & RRC_MAC_PUCCH_CHANNEL_SELECTION_CONFIG_V1020_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pucch_channel_selection_config_v1020(pp_buffer, &p_rrc_mac_pucch_format_r10->rrc_mac_pucch_channel_selection_config_v1020))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_scell_deactivation_timer_len
(
    scell_deactivation_timer_t *p_scell_deactivation_timer
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_scell_deactivation_timer != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_scell_deactivation_timer->scell_deactivation_timer);

    return length;
}

static
rrc_length_t
rrc_il_get_scell_deactivation_timer_tlv_len
(
    scell_deactivation_timer_t *p_scell_deactivation_timer

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_scell_deactivation_timer != PNULL);

    length += rrc_il_get_scell_deactivation_timer_len(p_scell_deactivation_timer);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_scell_deactivation_timer
(
    U8  **pp_buffer,
    scell_deactivation_timer_t *p_scell_deactivation_timer
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_scell_deactivation_timer != PNULL);

    /* This function composes scell_deactivation_timer */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SCELL_DEACTIVATION_TIMER_INFO;
    tlv_length = rrc_il_get_scell_deactivation_timer_tlv_len(p_scell_deactivation_timer);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_scell_deactivation_timer->scell_deactivation_timer > 128))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_scell_deactivation_timer->scell_deactivation_timer] should be less than"
            " or equal to 128. Incorrect value %u received.", p_scell_deactivation_timer->scell_deactivation_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_scell_deactivation_timer->scell_deactivation_timer, "scell_deactivation_timer");
    *pp_buffer += sizeof(p_scell_deactivation_timer->scell_deactivation_timer);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_supported_mimo_capability_dl_r10_len
(
    rrc_mac_supported_mimo_capability_dl_r10_t *p_rrc_mac_supported_mimo_capability_dl_r10
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_supported_mimo_capability_dl_r10 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_supported_mimo_capability_dl_r10->num_Of_layer);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_supported_mimo_capability_dl_r10_tlv_len
(
    rrc_mac_supported_mimo_capability_dl_r10_t *p_rrc_mac_supported_mimo_capability_dl_r10

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_supported_mimo_capability_dl_r10 != PNULL);

    length += rrc_il_get_rrc_mac_supported_mimo_capability_dl_r10_len(p_rrc_mac_supported_mimo_capability_dl_r10);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_supported_mimo_capability_dl_r10
(
    U8  **pp_buffer,
    rrc_mac_supported_mimo_capability_dl_r10_t *p_rrc_mac_supported_mimo_capability_dl_r10
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_supported_mimo_capability_dl_r10 != PNULL);

    /* This function composes rrc_mac_supported_mimo_capability_dl_r10 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MIMO_CAPABILITY_DL_V1020;
    tlv_length = rrc_il_get_rrc_mac_supported_mimo_capability_dl_r10_tlv_len(p_rrc_mac_supported_mimo_capability_dl_r10);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_supported_mimo_capability_dl_r10->num_Of_layer < 1) || (p_rrc_mac_supported_mimo_capability_dl_r10->num_Of_layer > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_supported_mimo_capability_dl_r10->num_Of_layer] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_mac_supported_mimo_capability_dl_r10->num_Of_layer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_supported_mimo_capability_dl_r10->num_Of_layer, "num_Of_layer");
    *pp_buffer += sizeof(p_rrc_mac_supported_mimo_capability_dl_r10->num_Of_layer);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tx_mode_info_len
(
    rrc_mac_tx_mode_info_t *p_rrc_mac_tx_mode_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tx_mode_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_tx_mode_info->transmission_mode);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_tx_mode_info_tlv_len
(
    rrc_mac_tx_mode_info_t *p_rrc_mac_tx_mode_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_tx_mode_info != PNULL);

    length += rrc_il_get_rrc_mac_tx_mode_info_len(p_rrc_mac_tx_mode_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_tx_mode_info
(
    U8  **pp_buffer,
    rrc_mac_tx_mode_info_t *p_rrc_mac_tx_mode_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_tx_mode_info != PNULL);

    /* This function composes rrc_mac_tx_mode_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_TX_MODE_INFO;
    tlv_length = rrc_il_get_rrc_mac_tx_mode_info_tlv_len(p_rrc_mac_tx_mode_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_tx_mode_info->transmission_mode < 1) || (p_rrc_mac_tx_mode_info->transmission_mode > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_tx_mode_info->transmission_mode] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_mac_tx_mode_info->transmission_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_tx_mode_info->transmission_mode, "transmission_mode");
    *pp_buffer += sizeof(p_rrc_mac_tx_mode_info->transmission_mode);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pdsch_config_info_len
(
    rrc_mac_pdsch_config_info_t *p_rrc_mac_pdsch_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pdsch_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pdsch_config_info->p_a);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pdsch_config_info_tlv_len
(
    rrc_mac_pdsch_config_info_t *p_rrc_mac_pdsch_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pdsch_config_info != PNULL);

    length += rrc_il_get_rrc_mac_pdsch_config_info_len(p_rrc_mac_pdsch_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_pdsch_config_info
(
    U8  **pp_buffer,
    rrc_mac_pdsch_config_info_t *p_rrc_mac_pdsch_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_pdsch_config_info != PNULL);

    /* This function composes rrc_mac_pdsch_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PDSCH_CONFIG_INFO;
    tlv_length = rrc_il_get_rrc_mac_pdsch_config_info_tlv_len(p_rrc_mac_pdsch_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pdsch_config_info->p_a > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pdsch_config_info->p_a] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_pdsch_config_info->p_a);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pdsch_config_info->p_a, "p_a");
    *pp_buffer += sizeof(p_rrc_mac_pdsch_config_info->p_a);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_num_harq_process_len
(
    rrc_mac_dl_num_harq_process_t *p_rrc_mac_dl_num_harq_process
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_num_harq_process != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_dl_num_harq_process->dl_num_harq_process);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_num_harq_process_tlv_len
(
    rrc_mac_dl_num_harq_process_t *p_rrc_mac_dl_num_harq_process

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_num_harq_process != PNULL);

    length += rrc_il_get_rrc_mac_dl_num_harq_process_len(p_rrc_mac_dl_num_harq_process);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_num_harq_process
(
    U8  **pp_buffer,
    rrc_mac_dl_num_harq_process_t *p_rrc_mac_dl_num_harq_process
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_dl_num_harq_process != PNULL);

    /* This function composes rrc_mac_dl_num_harq_process */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DL_NUM_HARQ_PROC;
    tlv_length = rrc_il_get_rrc_mac_dl_num_harq_process_tlv_len(p_rrc_mac_dl_num_harq_process);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_dl_num_harq_process->dl_num_harq_process < 1) || (p_rrc_mac_dl_num_harq_process->dl_num_harq_process > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_num_harq_process->dl_num_harq_process] should be in range "
            "1 to 15. Incorrect value %u received.", p_rrc_mac_dl_num_harq_process->dl_num_harq_process);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_dl_num_harq_process->dl_num_harq_process, "dl_num_harq_process");
    *pp_buffer += sizeof(p_rrc_mac_dl_num_harq_process->dl_num_harq_process);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_init_mcs_len
(
    rrc_mac_dl_init_mcs_t *p_rrc_mac_dl_init_mcs
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_init_mcs != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_dl_init_mcs->dl_init_mcs);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_init_mcs_tlv_len
(
    rrc_mac_dl_init_mcs_t *p_rrc_mac_dl_init_mcs

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_init_mcs != PNULL);

    length += rrc_il_get_rrc_mac_dl_init_mcs_len(p_rrc_mac_dl_init_mcs);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_init_mcs
(
    U8  **pp_buffer,
    rrc_mac_dl_init_mcs_t *p_rrc_mac_dl_init_mcs
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_dl_init_mcs != PNULL);

    /* This function composes rrc_mac_dl_init_mcs */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DL_INIT_MCS;
    tlv_length = rrc_il_get_rrc_mac_dl_init_mcs_tlv_len(p_rrc_mac_dl_init_mcs);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_dl_init_mcs->dl_init_mcs > 28))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_init_mcs->dl_init_mcs] should be less than"
            " or equal to 28. Incorrect value %u received.", p_rrc_mac_dl_init_mcs->dl_init_mcs);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_dl_init_mcs->dl_init_mcs, "dl_init_mcs");
    *pp_buffer += sizeof(p_rrc_mac_dl_init_mcs->dl_init_mcs);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_num_of_layer_info_len
(
    rrc_mac_num_of_layer_info_t *p_rrc_mac_num_of_layer_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_num_of_layer_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_num_of_layer_info->num_of_layer);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_num_of_layer_info_tlv_len
(
    rrc_mac_num_of_layer_info_t *p_rrc_mac_num_of_layer_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_num_of_layer_info != PNULL);

    length += rrc_il_get_rrc_mac_num_of_layer_info_len(p_rrc_mac_num_of_layer_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_num_of_layer_info
(
    U8  **pp_buffer,
    rrc_mac_num_of_layer_info_t *p_rrc_mac_num_of_layer_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_num_of_layer_info != PNULL);

    /* This function composes rrc_mac_num_of_layer_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_NUM_OF_LAYER_INFO;
    tlv_length = rrc_il_get_rrc_mac_num_of_layer_info_tlv_len(p_rrc_mac_num_of_layer_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_num_of_layer_info->num_of_layer < 1) || (p_rrc_mac_num_of_layer_info->num_of_layer > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_num_of_layer_info->num_of_layer] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_mac_num_of_layer_info->num_of_layer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_num_of_layer_info->num_of_layer, "num_of_layer");
    *pp_buffer += sizeof(p_rrc_mac_num_of_layer_info->num_of_layer);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_code_book_index_info_len
(
    rrc_mac_code_book_index_info_t *p_rrc_mac_code_book_index_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_code_book_index_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_code_book_index_info->code_book_index);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_code_book_index_info_tlv_len
(
    rrc_mac_code_book_index_info_t *p_rrc_mac_code_book_index_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_code_book_index_info != PNULL);

    length += rrc_il_get_rrc_mac_code_book_index_info_len(p_rrc_mac_code_book_index_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_code_book_index_info
(
    U8  **pp_buffer,
    rrc_mac_code_book_index_info_t *p_rrc_mac_code_book_index_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_code_book_index_info != PNULL);

    /* This function composes rrc_mac_code_book_index_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CODE_BOOK_INDEX_INFO;
    tlv_length = rrc_il_get_rrc_mac_code_book_index_info_tlv_len(p_rrc_mac_code_book_index_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_code_book_index_info->code_book_index > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_code_book_index_info->code_book_index] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_code_book_index_info->code_book_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_code_book_index_info->code_book_index, "code_book_index");
    *pp_buffer += sizeof(p_rrc_mac_code_book_index_info->code_book_index);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_scell_activate_status_len
(
    scell_activate_status_t *p_scell_activate_status
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_scell_activate_status != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_scell_activate_status->scellActivateStatus);

    return length;
}

static
rrc_length_t
rrc_il_get_scell_activate_status_tlv_len
(
    scell_activate_status_t *p_scell_activate_status

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_scell_activate_status != PNULL);

    length += rrc_il_get_scell_activate_status_len(p_scell_activate_status);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_scell_activate_status
(
    U8  **pp_buffer,
    scell_activate_status_t *p_scell_activate_status
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_scell_activate_status != PNULL);

    /* This function composes scell_activate_status */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SCELL_ACTIVATE_STATUS;
    tlv_length = rrc_il_get_scell_activate_status_tlv_len(p_scell_activate_status);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_scell_activate_status->scellActivateStatus > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_scell_activate_status->scellActivateStatus] should be less than"
            " or equal to 1. Incorrect value %u received.", p_scell_activate_status->scellActivateStatus);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_scell_activate_status->scellActivateStatus, "scellActivateStatus");
    *pp_buffer += sizeof(p_scell_activate_status->scellActivateStatus);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_earfcn_len
(
    rrc_mac_dl_earfcn_t *p_rrc_mac_dl_earfcn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_earfcn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_dl_earfcn->dlEarfcn);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_earfcn_tlv_len
(
    rrc_mac_dl_earfcn_t *p_rrc_mac_dl_earfcn

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_earfcn != PNULL);

    length += rrc_il_get_rrc_mac_dl_earfcn_len(p_rrc_mac_dl_earfcn);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_earfcn
(
    U8  **pp_buffer,
    rrc_mac_dl_earfcn_t *p_rrc_mac_dl_earfcn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_dl_earfcn != PNULL);

    /* This function composes rrc_mac_dl_earfcn */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DL_EARFCN;
    tlv_length = rrc_il_get_rrc_mac_dl_earfcn_tlv_len(p_rrc_mac_dl_earfcn);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_dl_earfcn->dlEarfcn > MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_earfcn->dlEarfcn] should be less than"
            " or equal to MAX_EARFCN. Incorrect value %u received.", p_rrc_mac_dl_earfcn->dlEarfcn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_mac_dl_earfcn->dlEarfcn, "dlEarfcn");
    *pp_buffer += sizeof(p_rrc_mac_dl_earfcn->dlEarfcn);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_scell_info_len
(
    rrc_mac_scell_info_t *p_rrc_mac_scell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_scell_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_scell_info->cell_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_scell_info->scell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_scell_info->operation_type);

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_MIMO_CAPABILITY_DL_V1020_PRESENT)
    {
        length += rrc_il_get_rrc_mac_supported_mimo_capability_dl_r10_tlv_len(&p_rrc_mac_scell_info->rrc_mac_supported_mimo_capability_dl_r10);
    }

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_TX_MODE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tx_mode_info_tlv_len(&p_rrc_mac_scell_info->tx_mode_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT)
    {
        length += rrc_il_get_rrc_mac_codebook_subset_restriction_v1020_tlv_len(&p_rrc_mac_scell_info->rrc_mac_codebook_subset_restriction_v1020);
    }

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_PDSCH_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pdsch_config_info_tlv_len(&p_rrc_mac_scell_info->pdsch_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_CQI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_cqi_info_tlv_len(&p_rrc_mac_scell_info->cqi_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_DL_NUM_HARQ_PROC_PRESENT)
    {
        length += rrc_il_get_rrc_mac_dl_num_harq_process_tlv_len(&p_rrc_mac_scell_info->rrc_mac_dl_num_harq_process);
    }

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_DL_INIT_MCS_PRESENT)
    {
        length += rrc_il_get_rrc_mac_dl_init_mcs_tlv_len(&p_rrc_mac_scell_info->rrc_mac_dl_init_mcs);
    }

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_NUM_OF_LAYER_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_num_of_layer_info_tlv_len(&p_rrc_mac_scell_info->num_of_layer_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_CODE_BOOK_INDEX_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_code_book_index_info_tlv_len(&p_rrc_mac_scell_info->code_book_index_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_USER_LOCATION_TYPE_PRESENT)
    {
        length += rrc_il_get_rrc_mac_user_location_type_tlv_len(&p_rrc_mac_scell_info->user_location_type);
    }

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_RF_PARAMS_PRESENT)
    {
        length += rrc_il_get_rrc_mac_rf_params_tlv_len(&p_rrc_mac_scell_info->rf_params);
    }

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_ACTIVATE_STATUS_PRESENT)
    {
        length += rrc_il_get_scell_activate_status_tlv_len(&p_rrc_mac_scell_info->scell_activate_status);
    }

    /* Get length of TLV */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_DL_EARFCN_PRESENT)
    {
        length += rrc_il_get_rrc_mac_dl_earfcn_tlv_len(&p_rrc_mac_scell_info->dl_earfcn);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_scell_info_tlv_len
(
    rrc_mac_scell_info_t *p_rrc_mac_scell_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_scell_info != PNULL);

    length += rrc_il_get_rrc_mac_scell_info_len(p_rrc_mac_scell_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_scell_info
(
    U8  **pp_buffer,
    rrc_mac_scell_info_t *p_rrc_mac_scell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_scell_info != PNULL);

    /* This function composes rrc_mac_scell_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SCELL_INFO;
    tlv_length = rrc_il_get_rrc_mac_scell_info_tlv_len(p_rrc_mac_scell_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_scell_info->cell_id > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_scell_info->cell_id] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_mac_scell_info->cell_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_scell_info->cell_id, "cell_id");
    *pp_buffer += sizeof(p_rrc_mac_scell_info->cell_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_scell_info->scell_index < 1) || (p_rrc_mac_scell_info->scell_index > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_scell_info->scell_index] should be in range "
            "1 to 7. Incorrect value %u received.", p_rrc_mac_scell_info->scell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_scell_info->scell_index, "scell_index");
    *pp_buffer += sizeof(p_rrc_mac_scell_info->scell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_scell_info->operation_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_scell_info->operation_type] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_scell_info->operation_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_scell_info->operation_type, "operation_type");
    *pp_buffer += sizeof(p_rrc_mac_scell_info->operation_type);

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_MIMO_CAPABILITY_DL_V1020_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_supported_mimo_capability_dl_r10(pp_buffer, &p_rrc_mac_scell_info->rrc_mac_supported_mimo_capability_dl_r10))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_TX_MODE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tx_mode_info(pp_buffer, &p_rrc_mac_scell_info->tx_mode_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_codebook_subset_restriction_v1020(pp_buffer, &p_rrc_mac_scell_info->rrc_mac_codebook_subset_restriction_v1020))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_PDSCH_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pdsch_config_info(pp_buffer, &p_rrc_mac_scell_info->pdsch_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_CQI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_cqi_info(pp_buffer, &p_rrc_mac_scell_info->cqi_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_DL_NUM_HARQ_PROC_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_dl_num_harq_process(pp_buffer, &p_rrc_mac_scell_info->rrc_mac_dl_num_harq_process))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_DL_INIT_MCS_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_dl_init_mcs(pp_buffer, &p_rrc_mac_scell_info->rrc_mac_dl_init_mcs))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_NUM_OF_LAYER_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_num_of_layer_info(pp_buffer, &p_rrc_mac_scell_info->num_of_layer_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_CODE_BOOK_INDEX_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_code_book_index_info(pp_buffer, &p_rrc_mac_scell_info->code_book_index_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_USER_LOCATION_TYPE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_user_location_type(pp_buffer, &p_rrc_mac_scell_info->user_location_type))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_RF_PARAMS_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_rf_params(pp_buffer, &p_rrc_mac_scell_info->rf_params))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_SCELL_ACTIVATE_STATUS_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_scell_activate_status(pp_buffer, &p_rrc_mac_scell_info->scell_activate_status))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_scell_info->optional_elems_present & RRC_MAC_DL_EARFCN_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_dl_earfcn(pp_buffer, &p_rrc_mac_scell_info->dl_earfcn))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ca_lc_bandwidth_dist_info_len
(
    rrc_mac_ca_lc_bandwidth_dist_info_t *p_rrc_mac_ca_lc_bandwidth_dist_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ca_lc_bandwidth_dist_info != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_mac_ca_lc_bandwidth_dist_info->band_width_distribution);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ca_lc_bandwidth_dist_info_tlv_len
(
    rrc_mac_ca_lc_bandwidth_dist_info_t *p_rrc_mac_ca_lc_bandwidth_dist_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ca_lc_bandwidth_dist_info != PNULL);

    length += rrc_il_get_rrc_mac_ca_lc_bandwidth_dist_info_len(p_rrc_mac_ca_lc_bandwidth_dist_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ca_lc_bandwidth_dist_info
(
    U8  **pp_buffer,
    rrc_mac_ca_lc_bandwidth_dist_info_t *p_rrc_mac_ca_lc_bandwidth_dist_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ca_lc_bandwidth_dist_info != PNULL);

    /* This function composes rrc_mac_ca_lc_bandwidth_dist_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO;
    tlv_length = rrc_il_get_rrc_mac_ca_lc_bandwidth_dist_info_tlv_len(p_rrc_mac_ca_lc_bandwidth_dist_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose OCTET_STRING FIXED of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_mac_ca_lc_bandwidth_dist_info->band_width_distribution); loop++)
        {
            if (p_rrc_mac_ca_lc_bandwidth_dist_info->band_width_distribution[loop] > 100)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_rrc_mac_ca_lc_bandwidth_dist_info->band_width_distribution[%u] should be less"
                    " than or equal to 100. "
                    "Incorrect value %d received.", loop, p_rrc_mac_ca_lc_bandwidth_dist_info->band_width_distribution[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_ca_lc_bandwidth_dist_info->band_width_distribution[loop], "band_width_distribution[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_carrier_aggr_config_len
(
    rrc_mac_carrier_aggr_config_t *p_rrc_mac_carrier_aggr_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_carrier_aggr_config != PNULL);

    /* Get length of TLV */
    if (p_rrc_mac_carrier_aggr_config->presence_bitmask & RRC_MAC_SCELL_DEACTIVATION_TIMER_INFO_PRESENT)
    {
        length += rrc_il_get_scell_deactivation_timer_tlv_len(&p_rrc_mac_carrier_aggr_config->scell_deactivation_timer);
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_carrier_aggr_config->count <= ARRSIZE(p_rrc_mac_carrier_aggr_config->rrc_mac_scell_info));
        for (loop = 0; loop < p_rrc_mac_carrier_aggr_config->count; loop++)
        {
            length += rrc_il_get_rrc_mac_scell_info_tlv_len(&p_rrc_mac_carrier_aggr_config->rrc_mac_scell_info[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_mac_carrier_aggr_config->presence_bitmask & RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ca_lc_bandwidth_dist_info_tlv_len(&p_rrc_mac_carrier_aggr_config->ca_lc_bandwidth_dist_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_carrier_aggr_config_tlv_len
(
    rrc_mac_carrier_aggr_config_t *p_rrc_mac_carrier_aggr_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_carrier_aggr_config != PNULL);

    length += rrc_il_get_rrc_mac_carrier_aggr_config_len(p_rrc_mac_carrier_aggr_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_carrier_aggr_config
(
    U8  **pp_buffer,
    rrc_mac_carrier_aggr_config_t *p_rrc_mac_carrier_aggr_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_carrier_aggr_config != PNULL);

    /* This function composes rrc_mac_carrier_aggr_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CA_CONFIG;
    tlv_length = rrc_il_get_rrc_mac_carrier_aggr_config_tlv_len(p_rrc_mac_carrier_aggr_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose TLV  */
    if (p_rrc_mac_carrier_aggr_config->presence_bitmask & RRC_MAC_SCELL_DEACTIVATION_TIMER_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_scell_deactivation_timer(pp_buffer, &p_rrc_mac_carrier_aggr_config->scell_deactivation_timer))
        {
            return RRC_FAILURE;
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_carrier_aggr_config->count <= ARRSIZE(p_rrc_mac_carrier_aggr_config->rrc_mac_scell_info));
        for (loop = 0; loop < p_rrc_mac_carrier_aggr_config->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_scell_info(pp_buffer, &p_rrc_mac_carrier_aggr_config->rrc_mac_scell_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_carrier_aggr_config->presence_bitmask & RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ca_lc_bandwidth_dist_info(pp_buffer, &p_rrc_mac_carrier_aggr_config->ca_lc_bandwidth_dist_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_add_ue_info_len
(
    rrc_mac_add_ue_info_t *p_rrc_mac_add_ue_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_add_ue_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->crnti);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->ue_priority);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->dl_num_harq_process);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->dl_i_mcs);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->dl_max_rb);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->ul_i_mcs);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->transmission_mode);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->num_of_layer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->code_book_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->max_ul_harq_tx);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->time_allignment_timer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->pc_max);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_add_ue_info->p_a);

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_SR_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sr_config_info_tlv_len(&p_rrc_mac_add_ue_info->sr_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_SR_PROHBIT_TIMER_VALUE_V920_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sr_prohbit_timer_value_v920_tlv_len(&p_rrc_mac_add_ue_info->sr_prohbit_timer_value_v920);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_CQI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_cqi_info_tlv_len(&p_rrc_mac_add_ue_info->cqi_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tdd_ack_nack_feedback_mode_tlv_len(&p_rrc_mac_add_ue_info->mac_tdd_ack_nack_feedback_mode);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_PUSCH_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pusch_config_info_tlv_len(&p_rrc_mac_add_ue_info->pusch_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_SRS_DEDICATED_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_srs_dedicated_config_tlv_len(&p_rrc_mac_add_ue_info->srs_dedicated_config);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
    {
        length += rrc_il_get_rrc_mac_codebook_subset_restriction_tlv_len(&p_rrc_mac_add_ue_info->codebook_subset_restriction);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_DRX_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_drx_config_info_tlv_len(&p_rrc_mac_add_ue_info->drx_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_MEAS_GAP_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_meas_gap_config_info_tlv_len(&p_rrc_mac_add_ue_info->meas_gap_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT)
    {
        length += rrc_il_get_rrc_mac_codebook_subset_restriction_v920_tlv_len(&p_rrc_mac_add_ue_info->codebook_subset_restriction_v920);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_UPLINK_POWER_CONTROL_DEDICATED_PRESENT)
    {
        length += rrc_il_get_rrc_mac_uplink_power_control_dedicated_tlv_len(&p_rrc_mac_add_ue_info->uplink_power_control_dedicated);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_TPC_PDCCH_CONFIG_PUCCH_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tpc_pdcch_config_pucch_tlv_len(&p_rrc_mac_add_ue_info->tpc_pdcch_config_pucch);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_TPC_PDCCH_CONFIG_PUSCH_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tpc_pdcch_config_pusch_tlv_len(&p_rrc_mac_add_ue_info->tpc_pdcch_config_pusch);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_AMBR_QOS_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ambr_qos_info_tlv_len(&p_rrc_mac_add_ue_info->ambr_qos_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_USER_LOCATION_TYPE_PRESENT)
    {
        length += rrc_il_get_rrc_mac_user_location_type_tlv_len(&p_rrc_mac_add_ue_info->user_location_type);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_UE_CAPABILITY_ADDITIONAL_PARAM_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ue_capability_additional_params_tlv_len(&p_rrc_mac_add_ue_info->ue_capability_additional_params);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_SPS_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sps_config_tlv_len(&p_rrc_mac_add_ue_info->sps_config);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_CONFIG_RF_PARAMS_PRESENT)
    {
        length += rrc_il_get_rrc_mac_rf_params_tlv_len(&p_rrc_mac_add_ue_info->rf_params);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_ADD_UE_CATEGORY_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ue_cat_tlv_len(&p_rrc_mac_add_ue_info->ue_category_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_UL_MU_MIMO_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ul_mu_mimo_info_tlv_len(&p_rrc_mac_add_ue_info->ul_mu_mimo_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_ADD_UE_PHR_TIMER_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_phr_timer_config_tlv_len(&p_rrc_mac_add_ue_info->phr_timer_config);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_MIN_UE_POWER_PRESENT)
    {
        length += rrc_il_get_rrc_mac_min_ue_power_tlv_len(&p_rrc_mac_add_ue_info->min_ue_power);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_UE_COMPLIANCE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ue_compliance_info_tlv_len(&p_rrc_mac_add_ue_info->ue_compliance_release);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_PDSCH_EPRE_TO_UE_RS_RATIO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pdsch_epre_to_ue_rs_ratio_tlv_len(&p_rrc_mac_add_ue_info->pdsch_epre_to_ue_rs_ratio);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_TTI_BUNDLING_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tti_bundling_info_tlv_len(&p_rrc_mac_add_ue_info->tti_bundling);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_MOCN_OPERATOR_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_operator_info_tlv_len(&p_rrc_mac_add_ue_info->operator_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_ADD_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT)
    {
        length += rrc_il_get_rrc_mac_codebook_subset_restriction_v1020_tlv_len(&p_rrc_mac_add_ue_info->rrc_mac_codebook_subset_restriction_v1020);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_DL_INTERFERENCE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_dl_interference_info_tlv_len(&p_rrc_mac_add_ue_info->rrc_mac_dl_interference_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->emtc_optional_elems_present & RRC_MAC_PUCCH_CONFIG_DEDICATED_R13_PRESENT)
    {
        length += rrc_il_get_PucchConfigDedicatedR13_tlv_len(&p_rrc_mac_add_ue_info->pucchConfigDedicatedR13);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->emtc_optional_elems_present & RRC_MAC_EPDCCH_SET_CONFIG_R11_PRESENT)
    {
        length += rrc_il_get_EpdcchSetConfigR11_tlv_len(&p_rrc_mac_add_ue_info->epdcchSetConfigR11);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->emtc_optional_elems_present & RRC_MAC_CE_LEVEL_PRESENT)
    {
        length += rrc_il_get_RrcMacCeLevel_tlv_len(&p_rrc_mac_add_ue_info->ceLevel);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->emtc_optional_elems_present & RRC_MAC_CE_MODE_PRESENT)
    {
        length += rrc_il_get_RrcMacCeMode_tlv_len(&p_rrc_mac_add_ue_info->ceMode);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->ca_optional_elems_present & RRC_MAC_RECONFIG_ADD_UE_CATEGORY_V1020_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ue_category_v1020_tlv_len(&p_rrc_mac_add_ue_info->rrc_mac_ue_category_v1020);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->ca_optional_elems_present & RRC_MAC_RECONFIG_ADD_PUCCH_FORMAT_V1020_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pucch_format_r10_tlv_len(&p_rrc_mac_add_ue_info->rrc_mac_pucch_format_r10);
    }

    /* Get length of TLV */
    if (p_rrc_mac_add_ue_info->ca_optional_elems_present & RRC_MAC_CA_RECONFIG_ADD_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_carrier_aggr_config_tlv_len(&p_rrc_mac_add_ue_info->rrc_mac_carrier_aggr_config);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_add_ue_info_tlv_len
(
    rrc_mac_add_ue_info_t *p_rrc_mac_add_ue_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_add_ue_info != PNULL);

    length += rrc_il_get_rrc_mac_add_ue_info_len(p_rrc_mac_add_ue_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_add_ue_info
(
    U8  **pp_buffer,
    rrc_mac_add_ue_info_t *p_rrc_mac_add_ue_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_add_ue_info != PNULL);

    /* This function composes rrc_mac_add_ue_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_ADD_UE_INFO;
    tlv_length = rrc_il_get_rrc_mac_add_ue_info_tlv_len(p_rrc_mac_add_ue_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_add_ue_info->crnti < 1) || (p_rrc_mac_add_ue_info->crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->crnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_add_ue_info->crnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_add_ue_info->crnti, "crnti");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->crnti);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->ue_priority > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->ue_priority] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_add_ue_info->ue_priority);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_add_ue_info->ue_priority, "ue_priority");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->ue_priority);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_add_ue_info->dl_num_harq_process < 1) || (p_rrc_mac_add_ue_info->dl_num_harq_process > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->dl_num_harq_process] should be in range "
            "1 to 15. Incorrect value %u received.", p_rrc_mac_add_ue_info->dl_num_harq_process);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_add_ue_info->dl_num_harq_process, "dl_num_harq_process");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->dl_num_harq_process);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->dl_i_mcs > 28))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->dl_i_mcs] should be less than"
            " or equal to 28. Incorrect value %u received.", p_rrc_mac_add_ue_info->dl_i_mcs);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_add_ue_info->dl_i_mcs, "dl_i_mcs");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->dl_i_mcs);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_add_ue_info->dl_max_rb < 1) || (p_rrc_mac_add_ue_info->dl_max_rb > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->dl_max_rb] should be in range "
            "1 to 100. Incorrect value %u received.", p_rrc_mac_add_ue_info->dl_max_rb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_add_ue_info->dl_max_rb, "dl_max_rb");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->dl_max_rb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->ul_i_mcs > 28))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->ul_i_mcs] should be less than"
            " or equal to 28. Incorrect value %u received.", p_rrc_mac_add_ue_info->ul_i_mcs);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_add_ue_info->ul_i_mcs, "ul_i_mcs");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->ul_i_mcs);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_add_ue_info->transmission_mode < 1) || (p_rrc_mac_add_ue_info->transmission_mode > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->transmission_mode] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrc_mac_add_ue_info->transmission_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_add_ue_info->transmission_mode, "transmission_mode");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->transmission_mode);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_add_ue_info->num_of_layer < 1) || (p_rrc_mac_add_ue_info->num_of_layer > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->num_of_layer] should be in range "
            "1 to 4. Incorrect value %u received.", p_rrc_mac_add_ue_info->num_of_layer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_add_ue_info->num_of_layer, "num_of_layer");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->num_of_layer);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->code_book_index > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->code_book_index] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_add_ue_info->code_book_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_add_ue_info->code_book_index, "code_book_index");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->code_book_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->max_ul_harq_tx > 13))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->max_ul_harq_tx] should be less than"
            " or equal to 13. Incorrect value %u received.", p_rrc_mac_add_ue_info->max_ul_harq_tx);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_add_ue_info->max_ul_harq_tx, "max_ul_harq_tx");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->max_ul_harq_tx);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->time_allignment_timer > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->time_allignment_timer] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_add_ue_info->time_allignment_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_add_ue_info->time_allignment_timer, "time_allignment_timer");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->time_allignment_timer);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_add_ue_info->pc_max < -40) || (p_rrc_mac_add_ue_info->pc_max > 23))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->pc_max] should be in range "
            "-40 to 23. Incorrect value %d received.", p_rrc_mac_add_ue_info->pc_max);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_mac_add_ue_info->pc_max, "pc_max");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->pc_max);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_add_ue_info->p_a > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_add_ue_info->p_a] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_add_ue_info->p_a);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_add_ue_info->p_a, "p_a");
    *pp_buffer += sizeof(p_rrc_mac_add_ue_info->p_a);

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_SR_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sr_config_info(pp_buffer, &p_rrc_mac_add_ue_info->sr_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_SR_PROHBIT_TIMER_VALUE_V920_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sr_prohbit_timer_value_v920(pp_buffer, &p_rrc_mac_add_ue_info->sr_prohbit_timer_value_v920))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_CQI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_cqi_info(pp_buffer, &p_rrc_mac_add_ue_info->cqi_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tdd_ack_nack_feedback_mode(pp_buffer, &p_rrc_mac_add_ue_info->mac_tdd_ack_nack_feedback_mode))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_PUSCH_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pusch_config_info(pp_buffer, &p_rrc_mac_add_ue_info->pusch_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_SRS_DEDICATED_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_srs_dedicated_config(pp_buffer, &p_rrc_mac_add_ue_info->srs_dedicated_config))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_codebook_subset_restriction(pp_buffer, &p_rrc_mac_add_ue_info->codebook_subset_restriction))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_DRX_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_drx_config_info(pp_buffer, &p_rrc_mac_add_ue_info->drx_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_MEAS_GAP_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_meas_gap_config_info(pp_buffer, &p_rrc_mac_add_ue_info->meas_gap_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_codebook_subset_restriction_v920(pp_buffer, &p_rrc_mac_add_ue_info->codebook_subset_restriction_v920))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_UPLINK_POWER_CONTROL_DEDICATED_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_uplink_power_control_dedicated(pp_buffer, &p_rrc_mac_add_ue_info->uplink_power_control_dedicated))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_TPC_PDCCH_CONFIG_PUCCH_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tpc_pdcch_config_pucch(pp_buffer, &p_rrc_mac_add_ue_info->tpc_pdcch_config_pucch))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_TPC_PDCCH_CONFIG_PUSCH_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tpc_pdcch_config_pusch(pp_buffer, &p_rrc_mac_add_ue_info->tpc_pdcch_config_pusch))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_AMBR_QOS_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ambr_qos_info(pp_buffer, &p_rrc_mac_add_ue_info->ambr_qos_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_USER_LOCATION_TYPE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_user_location_type(pp_buffer, &p_rrc_mac_add_ue_info->user_location_type))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_UE_CAPABILITY_ADDITIONAL_PARAM_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ue_capability_additional_params(pp_buffer, &p_rrc_mac_add_ue_info->ue_capability_additional_params))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_SPS_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sps_config(pp_buffer, &p_rrc_mac_add_ue_info->sps_config))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_CONFIG_RF_PARAMS_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_rf_params(pp_buffer, &p_rrc_mac_add_ue_info->rf_params))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_ADD_UE_CATEGORY_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ue_cat(pp_buffer, &p_rrc_mac_add_ue_info->ue_category_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_UL_MU_MIMO_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ul_mu_mimo_info(pp_buffer, &p_rrc_mac_add_ue_info->ul_mu_mimo_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_ADD_UE_PHR_TIMER_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_phr_timer_config(pp_buffer, &p_rrc_mac_add_ue_info->phr_timer_config))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_MIN_UE_POWER_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_min_ue_power(pp_buffer, &p_rrc_mac_add_ue_info->min_ue_power))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_UE_COMPLIANCE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ue_compliance_info(pp_buffer, &p_rrc_mac_add_ue_info->ue_compliance_release))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_PDSCH_EPRE_TO_UE_RS_RATIO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pdsch_epre_to_ue_rs_ratio(pp_buffer, &p_rrc_mac_add_ue_info->pdsch_epre_to_ue_rs_ratio))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_TTI_BUNDLING_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tti_bundling_info(pp_buffer, &p_rrc_mac_add_ue_info->tti_bundling))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_MOCN_OPERATOR_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_operator_info(pp_buffer, &p_rrc_mac_add_ue_info->operator_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_ADD_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_codebook_subset_restriction_v1020(pp_buffer, &p_rrc_mac_add_ue_info->rrc_mac_codebook_subset_restriction_v1020))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->optional_elems_present & RRC_MAC_DL_INTERFERENCE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_dl_interference_info(pp_buffer, &p_rrc_mac_add_ue_info->rrc_mac_dl_interference_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->emtc_optional_elems_present & RRC_MAC_PUCCH_CONFIG_DEDICATED_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_PucchConfigDedicatedR13(pp_buffer, &p_rrc_mac_add_ue_info->pucchConfigDedicatedR13))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->emtc_optional_elems_present & RRC_MAC_EPDCCH_SET_CONFIG_R11_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_EpdcchSetConfigR11(pp_buffer, &p_rrc_mac_add_ue_info->epdcchSetConfigR11))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->emtc_optional_elems_present & RRC_MAC_CE_LEVEL_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacCeLevel(pp_buffer, &p_rrc_mac_add_ue_info->ceLevel))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->emtc_optional_elems_present & RRC_MAC_CE_MODE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacCeMode(pp_buffer, &p_rrc_mac_add_ue_info->ceMode))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->ca_optional_elems_present & RRC_MAC_RECONFIG_ADD_UE_CATEGORY_V1020_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ue_category_v1020(pp_buffer, &p_rrc_mac_add_ue_info->rrc_mac_ue_category_v1020))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->ca_optional_elems_present & RRC_MAC_RECONFIG_ADD_PUCCH_FORMAT_V1020_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pucch_format_r10(pp_buffer, &p_rrc_mac_add_ue_info->rrc_mac_pucch_format_r10))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_add_ue_info->ca_optional_elems_present & RRC_MAC_CA_RECONFIG_ADD_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_carrier_aggr_config(pp_buffer, &p_rrc_mac_add_ue_info->rrc_mac_carrier_aggr_config))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_configure_status_report_required_ue_len
(
    rrc_mac_configure_status_report_required_ue_t *p_rrc_mac_configure_status_report_required_ue
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_configure_status_report_required_ue != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_configure_status_report_required_ue->ue_psr_req_flag);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_configure_status_report_required_ue_tlv_len
(
    rrc_mac_configure_status_report_required_ue_t *p_rrc_mac_configure_status_report_required_ue

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_configure_status_report_required_ue != PNULL);

    length += rrc_il_get_rrc_mac_configure_status_report_required_ue_len(p_rrc_mac_configure_status_report_required_ue);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_configure_status_report_required_ue
(
    U8  **pp_buffer,
    rrc_mac_configure_status_report_required_ue_t *p_rrc_mac_configure_status_report_required_ue
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_configure_status_report_required_ue != PNULL);

    /* This function composes rrc_mac_configure_status_report_required_ue */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UE_PSR_REQ;
    tlv_length = rrc_il_get_rrc_mac_configure_status_report_required_ue_tlv_len(p_rrc_mac_configure_status_report_required_ue);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_configure_status_report_required_ue->ue_psr_req_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_configure_status_report_required_ue->ue_psr_req_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_configure_status_report_required_ue->ue_psr_req_flag);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_configure_status_report_required_ue->ue_psr_req_flag, "ue_psr_req_flag");
    *pp_buffer += sizeof(p_rrc_mac_configure_status_report_required_ue->ue_psr_req_flag);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ul_lc_create_req_len
(
    rrc_mac_ul_lc_create_req_t *p_rrc_mac_ul_lc_create_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ul_lc_create_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ul_lc_create_req->lc_g_id);

    /* Get length of TLV */
    if (p_rrc_mac_ul_lc_create_req->optional_elems_present & RRC_MAC_UE_PSR_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_configure_status_report_required_ue_tlv_len(&p_rrc_mac_ul_lc_create_req->configure_status_report_required_ue);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ul_lc_create_req_tlv_len
(
    rrc_mac_ul_lc_create_req_t *p_rrc_mac_ul_lc_create_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ul_lc_create_req != PNULL);

    length += rrc_il_get_rrc_mac_ul_lc_create_req_len(p_rrc_mac_ul_lc_create_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ul_lc_create_req
(
    U8  **pp_buffer,
    rrc_mac_ul_lc_create_req_t *p_rrc_mac_ul_lc_create_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ul_lc_create_req != PNULL);

    /* This function composes rrc_mac_ul_lc_create_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UL_LC_CREATE_REQ;
    tlv_length = rrc_il_get_rrc_mac_ul_lc_create_req_tlv_len(p_rrc_mac_ul_lc_create_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ul_lc_create_req->lc_g_id > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ul_lc_create_req->lc_g_id] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_ul_lc_create_req->lc_g_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_ul_lc_create_req->lc_g_id, "lc_g_id");
    *pp_buffer += sizeof(p_rrc_mac_ul_lc_create_req->lc_g_id);

    /* Compose TLV  */
    if (p_rrc_mac_ul_lc_create_req->optional_elems_present & RRC_MAC_UE_PSR_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_configure_status_report_required_ue(pp_buffer, &p_rrc_mac_ul_lc_create_req->configure_status_report_required_ue))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_configure_status_report_required_enb_len
(
    rrc_mac_configure_status_report_required_enb_t *p_rrc_mac_configure_status_report_required_enb
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_configure_status_report_required_enb != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_configure_status_report_required_enb->enb_psr_req_flag);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_configure_status_report_required_enb_tlv_len
(
    rrc_mac_configure_status_report_required_enb_t *p_rrc_mac_configure_status_report_required_enb

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_configure_status_report_required_enb != PNULL);

    length += rrc_il_get_rrc_mac_configure_status_report_required_enb_len(p_rrc_mac_configure_status_report_required_enb);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_configure_status_report_required_enb
(
    U8  **pp_buffer,
    rrc_mac_configure_status_report_required_enb_t *p_rrc_mac_configure_status_report_required_enb
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_configure_status_report_required_enb != PNULL);

    /* This function composes rrc_mac_configure_status_report_required_enb */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_ENB_PSR_REQ;
    tlv_length = rrc_il_get_rrc_mac_configure_status_report_required_enb_tlv_len(p_rrc_mac_configure_status_report_required_enb);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_configure_status_report_required_enb->enb_psr_req_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_configure_status_report_required_enb->enb_psr_req_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_configure_status_report_required_enb->enb_psr_req_flag);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_configure_status_report_required_enb->enb_psr_req_flag, "enb_psr_req_flag");
    *pp_buffer += sizeof(p_rrc_mac_configure_status_report_required_enb->enb_psr_req_flag);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_lc_create_req_len
(
    rrc_mac_dl_lc_create_req_t *p_rrc_mac_dl_lc_create_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_lc_create_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_dl_lc_create_req->lch_priority);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_dl_lc_create_req->rlc_sn_field_length);

    /* Get length of TLV */
    if (p_rrc_mac_dl_lc_create_req->optional_elems_present & RRC_MAC_ENB_PSR_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_configure_status_report_required_enb_tlv_len(&p_rrc_mac_dl_lc_create_req->configure_status_report_required_enb);
    }

    /* Get length of TLV */
    if (p_rrc_mac_dl_lc_create_req->optional_elems_present & RRC_MAC_DL_CREATE_CA_LC_BANDWIDTH_DIST_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ca_lc_bandwidth_dist_info_tlv_len(&p_rrc_mac_dl_lc_create_req->ca_lc_bandwidth_dist_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_lc_create_req_tlv_len
(
    rrc_mac_dl_lc_create_req_t *p_rrc_mac_dl_lc_create_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_lc_create_req != PNULL);

    length += rrc_il_get_rrc_mac_dl_lc_create_req_len(p_rrc_mac_dl_lc_create_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_lc_create_req
(
    U8  **pp_buffer,
    rrc_mac_dl_lc_create_req_t *p_rrc_mac_dl_lc_create_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_dl_lc_create_req != PNULL);

    /* This function composes rrc_mac_dl_lc_create_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DL_LC_CREATE_REQ;
    tlv_length = rrc_il_get_rrc_mac_dl_lc_create_req_tlv_len(p_rrc_mac_dl_lc_create_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_dl_lc_create_req->lch_priority < 1) || (p_rrc_mac_dl_lc_create_req->lch_priority > 16))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_lc_create_req->lch_priority] should be in range "
            "1 to 16. Incorrect value %u received.", p_rrc_mac_dl_lc_create_req->lch_priority);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_dl_lc_create_req->lch_priority, "lch_priority");
    *pp_buffer += sizeof(p_rrc_mac_dl_lc_create_req->lch_priority);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_dl_lc_create_req->rlc_sn_field_length < 5) || (p_rrc_mac_dl_lc_create_req->rlc_sn_field_length > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_lc_create_req->rlc_sn_field_length] should be in range "
            "5 to 10. Incorrect value %u received.", p_rrc_mac_dl_lc_create_req->rlc_sn_field_length);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_dl_lc_create_req->rlc_sn_field_length, "rlc_sn_field_length");
    *pp_buffer += sizeof(p_rrc_mac_dl_lc_create_req->rlc_sn_field_length);

    /* Compose TLV  */
    if (p_rrc_mac_dl_lc_create_req->optional_elems_present & RRC_MAC_ENB_PSR_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_configure_status_report_required_enb(pp_buffer, &p_rrc_mac_dl_lc_create_req->configure_status_report_required_enb))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_dl_lc_create_req->optional_elems_present & RRC_MAC_DL_CREATE_CA_LC_BANDWIDTH_DIST_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ca_lc_bandwidth_dist_info(pp_buffer, &p_rrc_mac_dl_lc_create_req->ca_lc_bandwidth_dist_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_gbr_qos_info_len
(
    rrc_mac_gbr_qos_info_t *p_rrc_mac_gbr_qos_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_gbr_qos_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_gbr_qos_info->e_rab_mbr_dl);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_gbr_qos_info->e_rab_mbr_ul);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_gbr_qos_info->e_rab_gbr_dl);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_gbr_qos_info->e_rab_gbr_ul);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_gbr_qos_info_tlv_len
(
    rrc_mac_gbr_qos_info_t *p_rrc_mac_gbr_qos_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_gbr_qos_info != PNULL);

    length += rrc_il_get_rrc_mac_gbr_qos_info_len(p_rrc_mac_gbr_qos_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_gbr_qos_info
(
    U8  **pp_buffer,
    rrc_mac_gbr_qos_info_t *p_rrc_mac_gbr_qos_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_gbr_qos_info != PNULL);

    /* This function composes rrc_mac_gbr_qos_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_GBR_QOS_INFO;
    tlv_length = rrc_il_get_rrc_mac_gbr_qos_info_tlv_len(p_rrc_mac_gbr_qos_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_mac_gbr_qos_info->e_rab_mbr_dl, "e_rab_mbr_dl");
    *pp_buffer += sizeof(p_rrc_mac_gbr_qos_info->e_rab_mbr_dl);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_mac_gbr_qos_info->e_rab_mbr_ul, "e_rab_mbr_ul");
    *pp_buffer += sizeof(p_rrc_mac_gbr_qos_info->e_rab_mbr_ul);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_mac_gbr_qos_info->e_rab_gbr_dl, "e_rab_gbr_dl");
    *pp_buffer += sizeof(p_rrc_mac_gbr_qos_info->e_rab_gbr_dl);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_mac_gbr_qos_info->e_rab_gbr_ul, "e_rab_gbr_ul");
    *pp_buffer += sizeof(p_rrc_mac_gbr_qos_info->e_rab_gbr_ul);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_qos_info_len
(
    rrc_mac_qos_info_t *p_rrc_mac_qos_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_qos_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_qos_info->bearer_type);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_qos_info->qci);

    /* Get length of TLV */
    if (p_rrc_mac_qos_info->optional_elems_present & RRC_MAC_GBR_QOS_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_gbr_qos_info_tlv_len(&p_rrc_mac_qos_info->gbr_qos_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_qos_info_tlv_len
(
    rrc_mac_qos_info_t *p_rrc_mac_qos_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_qos_info != PNULL);

    length += rrc_il_get_rrc_mac_qos_info_len(p_rrc_mac_qos_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_qos_info
(
    U8  **pp_buffer,
    rrc_mac_qos_info_t *p_rrc_mac_qos_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_qos_info != PNULL);

    /* This function composes rrc_mac_qos_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_QOS_INFO;
    tlv_length = rrc_il_get_rrc_mac_qos_info_tlv_len(p_rrc_mac_qos_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_qos_info->bearer_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_qos_info->bearer_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_qos_info->bearer_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_qos_info->bearer_type, "bearer_type");
    *pp_buffer += sizeof(p_rrc_mac_qos_info->bearer_type);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_qos_info->qci < 1) || (p_rrc_mac_qos_info->qci > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_qos_info->qci] should be in range "
            "1 to 9. Incorrect value %u received.", p_rrc_mac_qos_info->qci);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_qos_info->qci, "qci");
    *pp_buffer += sizeof(p_rrc_mac_qos_info->qci);

    /* Compose TLV  */
    if (p_rrc_mac_qos_info->optional_elems_present & RRC_MAC_GBR_QOS_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_gbr_qos_info(pp_buffer, &p_rrc_mac_qos_info->gbr_qos_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_rohc_profile_len
(
    rrc_mac_rohc_profile_t *p_rrc_mac_rohc_profile
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_rohc_profile != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_rohc_profile->rohc_profile);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_rohc_profile_tlv_len
(
    rrc_mac_rohc_profile_t *p_rrc_mac_rohc_profile

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_rohc_profile != PNULL);

    length += rrc_il_get_rrc_mac_rohc_profile_len(p_rrc_mac_rohc_profile);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_rohc_profile
(
    U8  **pp_buffer,
    rrc_mac_rohc_profile_t *p_rrc_mac_rohc_profile
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_rohc_profile != PNULL);

    /* This function composes rrc_mac_rohc_profile */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_ROHC_PROFILE;
    tlv_length = rrc_il_get_rrc_mac_rohc_profile_tlv_len(p_rrc_mac_rohc_profile);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_rohc_profile->rohc_profile > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_rohc_profile->rohc_profile] should be less than"
            " or equal to 6. Incorrect value %u received.", p_rrc_mac_rohc_profile->rohc_profile);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_rohc_profile->rohc_profile, "rohc_profile");
    *pp_buffer += sizeof(p_rrc_mac_rohc_profile->rohc_profile);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_create_lc_req_len
(
    rrc_mac_create_lc_req_t *p_rrc_mac_create_lc_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_create_lc_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_create_lc_req->lch_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_create_lc_req->rlc_mode);

    /* Get length of TLV */
    if (p_rrc_mac_create_lc_req->optional_elems_present & RRC_MAC_UL_LC_CREATE_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ul_lc_create_req_tlv_len(&p_rrc_mac_create_lc_req->ul_lc_create_req);
    }

    /* Get length of TLV */
    if (p_rrc_mac_create_lc_req->optional_elems_present & RRC_MAC_DL_LC_CREATE_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_dl_lc_create_req_tlv_len(&p_rrc_mac_create_lc_req->dl_lc_create_req);
    }

    /* Get length of TLV */
    if (p_rrc_mac_create_lc_req->optional_elems_present & RRC_MAC_QOS_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_qos_info_tlv_len(&p_rrc_mac_create_lc_req->qos_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_create_lc_req->optional_elems_present & RRC_MAC_ROHC_PROFILE_PRESENT)
    {
        length += rrc_il_get_rrc_mac_rohc_profile_tlv_len(&p_rrc_mac_create_lc_req->rohc_profile);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_create_lc_req_tlv_len
(
    rrc_mac_create_lc_req_t *p_rrc_mac_create_lc_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_create_lc_req != PNULL);

    length += rrc_il_get_rrc_mac_create_lc_req_len(p_rrc_mac_create_lc_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_create_lc_req
(
    U8  **pp_buffer,
    rrc_mac_create_lc_req_t *p_rrc_mac_create_lc_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_create_lc_req != PNULL);

    /* This function composes rrc_mac_create_lc_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CREATE_LC_REQ;
    tlv_length = rrc_il_get_rrc_mac_create_lc_req_tlv_len(p_rrc_mac_create_lc_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_create_lc_req->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_create_lc_req->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_create_lc_req->lch_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_create_lc_req->lch_id, "lch_id");
    *pp_buffer += sizeof(p_rrc_mac_create_lc_req->lch_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_create_lc_req->rlc_mode > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_create_lc_req->rlc_mode] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_create_lc_req->rlc_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_create_lc_req->rlc_mode, "rlc_mode");
    *pp_buffer += sizeof(p_rrc_mac_create_lc_req->rlc_mode);

    /* Compose TLV  */
    if (p_rrc_mac_create_lc_req->optional_elems_present & RRC_MAC_UL_LC_CREATE_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ul_lc_create_req(pp_buffer, &p_rrc_mac_create_lc_req->ul_lc_create_req))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_create_lc_req->optional_elems_present & RRC_MAC_DL_LC_CREATE_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_dl_lc_create_req(pp_buffer, &p_rrc_mac_create_lc_req->dl_lc_create_req))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_create_lc_req->optional_elems_present & RRC_MAC_QOS_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_qos_info(pp_buffer, &p_rrc_mac_create_lc_req->qos_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_create_lc_req->optional_elems_present & RRC_MAC_ROHC_PROFILE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_rohc_profile(pp_buffer, &p_rrc_mac_create_lc_req->rohc_profile))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_uplink_rat1_info_len
(
    rrc_mac_uplink_rat1_info_t *p_rrc_mac_uplink_rat1_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_uplink_rat1_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_uplink_rat1_info->is_uplink_rat1_supported);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_uplink_rat1_info_tlv_len
(
    rrc_mac_uplink_rat1_info_t *p_rrc_mac_uplink_rat1_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_uplink_rat1_info != PNULL);

    length += rrc_il_get_rrc_mac_uplink_rat1_info_len(p_rrc_mac_uplink_rat1_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_uplink_rat1_info
(
    U8  **pp_buffer,
    rrc_mac_uplink_rat1_info_t *p_rrc_mac_uplink_rat1_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_uplink_rat1_info != PNULL);

    /* This function composes rrc_mac_uplink_rat1_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RAT1_INFO;
    tlv_length = rrc_il_get_rrc_mac_uplink_rat1_info_tlv_len(p_rrc_mac_uplink_rat1_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_uplink_rat1_info->is_uplink_rat1_supported > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_uplink_rat1_info->is_uplink_rat1_supported] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_uplink_rat1_info->is_uplink_rat1_supported);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_uplink_rat1_info->is_uplink_rat1_supported, "is_uplink_rat1_supported");
    *pp_buffer += sizeof(p_rrc_mac_uplink_rat1_info->is_uplink_rat1_supported);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_create_ue_entity_cnf_len
(
    rrc_mac_create_ue_entity_cnf_t *p_rrc_mac_create_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_create_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_create_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_create_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_create_ue_entity_cnf->create_lc_error_counter <= ARRSIZE(p_rrc_mac_create_ue_entity_cnf->create_lc_error));
        for (loop = 0; loop < p_rrc_mac_create_ue_entity_cnf->create_lc_error_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_create_lc_error_tlv_len(&p_rrc_mac_create_ue_entity_cnf->create_lc_error[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_mac_create_ue_entity_cnf->bitmask & RRC_MAC_SPS_CRNTI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sps_crnti_info_tlv_len(&p_rrc_mac_create_ue_entity_cnf->sps_crnti_info);
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_create_ue_entity_cnf->create_ca_config_error_counter <= ARRSIZE(p_rrc_mac_create_ue_entity_cnf->rrc_mac_create_ca_config_err));
        for (loop = 0; loop < p_rrc_mac_create_ue_entity_cnf->create_ca_config_error_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_ca_config_error_tlv_len(&p_rrc_mac_create_ue_entity_cnf->rrc_mac_create_ca_config_err[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_create_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_mac_create_ue_entity_cnf_t *p_rrc_mac_create_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_create_ue_entity_cnf != PNULL);

    /* This function composes rrc_mac_create_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_create_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_create_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_create_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_create_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_create_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_create_ue_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_mac_create_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_create_ue_entity_cnf->create_lc_error_counter <= ARRSIZE(p_rrc_mac_create_ue_entity_cnf->create_lc_error));
        for (loop = 0; loop < p_rrc_mac_create_ue_entity_cnf->create_lc_error_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_create_lc_error(pp_buffer, &p_rrc_mac_create_ue_entity_cnf->create_lc_error[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_create_ue_entity_cnf->bitmask & RRC_MAC_SPS_CRNTI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sps_crnti_info(pp_buffer, &p_rrc_mac_create_ue_entity_cnf->sps_crnti_info))
        {
            return RRC_FAILURE;
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_create_ue_entity_cnf->create_ca_config_error_counter <= ARRSIZE(p_rrc_mac_create_ue_entity_cnf->rrc_mac_create_ca_config_err));
        for (loop = 0; loop < p_rrc_mac_create_ue_entity_cnf->create_ca_config_error_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_ca_config_error(pp_buffer, &p_rrc_mac_create_ue_entity_cnf->rrc_mac_create_ca_config_err[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_create_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_CREATE_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_create_ue_entity_cnf
(
    rrc_mac_create_ue_entity_cnf_t  *p_rrc_mac_create_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_create_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_create_ue_entity_cnf_len(p_rrc_mac_create_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_CREATE_UE_ENTITY_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_CREATE_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_CREATE_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_MAC_CREATE_UE_ENTITY_CNF message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_create_ue_entity_cnf(&p_mac_msg, p_rrc_mac_create_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ul_lc_config_resp_len
(
    rrc_mac_ul_lc_config_resp_t *p_rrc_mac_ul_lc_config_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ul_lc_config_resp != PNULL);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ul_lc_config_resp_tlv_len
(
    rrc_mac_ul_lc_config_resp_t *p_rrc_mac_ul_lc_config_resp

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ul_lc_config_resp != PNULL);

    length += rrc_il_get_rrc_mac_ul_lc_config_resp_len(p_rrc_mac_ul_lc_config_resp);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ul_lc_config_resp
(
    U8  **pp_buffer,
    rrc_mac_ul_lc_config_resp_t *p_rrc_mac_ul_lc_config_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ul_lc_config_resp != PNULL);

    /* This function composes rrc_mac_ul_lc_config_resp */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UL_LC_CONFIG_RESP;
    tlv_length = rrc_il_get_rrc_mac_ul_lc_config_resp_tlv_len(p_rrc_mac_ul_lc_config_resp);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_lc_config_resp_len
(
    rrc_mac_dl_lc_config_resp_t *p_rrc_mac_dl_lc_config_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_lc_config_resp != PNULL);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_lc_config_resp_tlv_len
(
    rrc_mac_dl_lc_config_resp_t *p_rrc_mac_dl_lc_config_resp

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_lc_config_resp != PNULL);

    length += rrc_il_get_rrc_mac_dl_lc_config_resp_len(p_rrc_mac_dl_lc_config_resp);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_lc_config_resp
(
    U8  **pp_buffer,
    rrc_mac_dl_lc_config_resp_t *p_rrc_mac_dl_lc_config_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_dl_lc_config_resp != PNULL);

    /* This function composes rrc_mac_dl_lc_config_resp */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DL_LC_CONFIG_RESP;
    tlv_length = rrc_il_get_rrc_mac_dl_lc_config_resp_tlv_len(p_rrc_mac_dl_lc_config_resp);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_create_lc_error_len
(
    rrc_mac_create_lc_error_t *p_rrc_mac_create_lc_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_create_lc_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_create_lc_error->lch_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_create_lc_error->response);

    /* Get length of TLV */
    if (p_rrc_mac_create_lc_error->optional_elems_present & RRC_MAC_UL_LC_CONFIG_RESP_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ul_lc_config_resp_tlv_len(&p_rrc_mac_create_lc_error->ul_lc_config_resp);
    }

    /* Get length of TLV */
    if (p_rrc_mac_create_lc_error->optional_elems_present & RRC_MAC_DL_LC_CONFIG_RESP_PRESENT)
    {
        length += rrc_il_get_rrc_mac_dl_lc_config_resp_tlv_len(&p_rrc_mac_create_lc_error->dl_lc_config_resp);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_create_lc_error_tlv_len
(
    rrc_mac_create_lc_error_t *p_rrc_mac_create_lc_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_create_lc_error != PNULL);

    length += rrc_il_get_rrc_mac_create_lc_error_len(p_rrc_mac_create_lc_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_create_lc_error
(
    U8  **pp_buffer,
    rrc_mac_create_lc_error_t *p_rrc_mac_create_lc_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_create_lc_error != PNULL);

    /* This function composes rrc_mac_create_lc_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CREATE_LC_ERROR;
    tlv_length = rrc_il_get_rrc_mac_create_lc_error_tlv_len(p_rrc_mac_create_lc_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_create_lc_error->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_create_lc_error->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_create_lc_error->lch_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_create_lc_error->lch_id, "lch_id");
    *pp_buffer += sizeof(p_rrc_mac_create_lc_error->lch_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_create_lc_error->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_create_lc_error->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_create_lc_error->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_create_lc_error->response, "response");
    *pp_buffer += sizeof(p_rrc_mac_create_lc_error->response);

    /* Compose TLV  */
    if (p_rrc_mac_create_lc_error->optional_elems_present & RRC_MAC_UL_LC_CONFIG_RESP_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ul_lc_config_resp(pp_buffer, &p_rrc_mac_create_lc_error->ul_lc_config_resp))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_create_lc_error->optional_elems_present & RRC_MAC_DL_LC_CONFIG_RESP_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_dl_lc_config_resp(pp_buffer, &p_rrc_mac_create_lc_error->dl_lc_config_resp))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_crnti_info_len
(
    rrc_mac_sps_crnti_info_t *p_rrc_mac_sps_crnti_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_crnti_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_sps_crnti_info->sps_crnti);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_sps_crnti_info_tlv_len
(
    rrc_mac_sps_crnti_info_t *p_rrc_mac_sps_crnti_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_sps_crnti_info != PNULL);

    length += rrc_il_get_rrc_mac_sps_crnti_info_len(p_rrc_mac_sps_crnti_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_sps_crnti_info
(
    U8  **pp_buffer,
    rrc_mac_sps_crnti_info_t *p_rrc_mac_sps_crnti_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_sps_crnti_info != PNULL);

    /* This function composes rrc_mac_sps_crnti_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_SPS_CRNTI_INFO;
    tlv_length = rrc_il_get_rrc_mac_sps_crnti_info_tlv_len(p_rrc_mac_sps_crnti_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_sps_crnti_info->sps_crnti < 61) || (p_rrc_mac_sps_crnti_info->sps_crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_sps_crnti_info->sps_crnti] should be in range "
            "61 to 65523. Incorrect value %u received.", p_rrc_mac_sps_crnti_info->sps_crnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_sps_crnti_info->sps_crnti, "sps_crnti");
    *pp_buffer += sizeof(p_rrc_mac_sps_crnti_info->sps_crnti);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ca_config_error_len
(
    rrc_mac_ca_config_error_t *p_rrc_mac_ca_config_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ca_config_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ca_config_error->cell_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ca_config_error->error_code);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ca_config_error_tlv_len
(
    rrc_mac_ca_config_error_t *p_rrc_mac_ca_config_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ca_config_error != PNULL);

    length += rrc_il_get_rrc_mac_ca_config_error_len(p_rrc_mac_ca_config_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ca_config_error
(
    U8  **pp_buffer,
    rrc_mac_ca_config_error_t *p_rrc_mac_ca_config_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ca_config_error != PNULL);

    /* This function composes rrc_mac_ca_config_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CREATE_CA_CONFIG_ERROR;
    tlv_length = rrc_il_get_rrc_mac_ca_config_error_tlv_len(p_rrc_mac_ca_config_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ca_config_error->cell_id > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ca_config_error->cell_id] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_ca_config_error->cell_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_ca_config_error->cell_id, "cell_id");
    *pp_buffer += sizeof(p_rrc_mac_ca_config_error->cell_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ca_config_error->error_code < 7) || (p_rrc_mac_ca_config_error->error_code > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ca_config_error->error_code] should be in range "
            "7 to 7. Incorrect value %u received.", p_rrc_mac_ca_config_error->error_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ca_config_error->error_code, "error_code");
    *pp_buffer += sizeof(p_rrc_mac_ca_config_error->error_code);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_delete_ue_entity_req_len
(
    rrc_mac_delete_ue_entity_req_t *p_rrc_mac_delete_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_delete_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_delete_ue_entity_req->ue_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_delete_ue_entity_req
(
    U8  **pp_buffer,
    rrc_mac_delete_ue_entity_req_t *p_rrc_mac_delete_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_delete_ue_entity_req != PNULL);

    /* This function composes rrc_mac_delete_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_delete_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_delete_ue_entity_req->ue_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_delete_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_DELETE_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_delete_ue_entity_req
(
    rrc_mac_delete_ue_entity_req_t  *p_rrc_mac_delete_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_delete_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_delete_ue_entity_req_len(p_rrc_mac_delete_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_DELETE_UE_ENTITY_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_DELETE_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_DELETE_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_MAC_DELETE_UE_ENTITY_REQ message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_delete_ue_entity_req(&p_mac_msg, p_rrc_mac_delete_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_delete_ue_entity_cnf_len
(
    rrc_mac_delete_ue_entity_cnf_t *p_rrc_mac_delete_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_delete_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_delete_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_delete_ue_entity_cnf->response);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_delete_ue_entity_cnf->delete_lc_status_counter <= ARRSIZE(p_rrc_mac_delete_ue_entity_cnf->delete_lc_status));
        for (loop = 0; loop < p_rrc_mac_delete_ue_entity_cnf->delete_lc_status_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_delete_lc_status_tlv_len(&p_rrc_mac_delete_ue_entity_cnf->delete_lc_status[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_delete_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_mac_delete_ue_entity_cnf_t *p_rrc_mac_delete_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_delete_ue_entity_cnf != PNULL);

    /* This function composes rrc_mac_delete_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_delete_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_delete_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_ue_entity_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_ue_entity_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_delete_ue_entity_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_delete_ue_entity_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_mac_delete_ue_entity_cnf->response);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_delete_ue_entity_cnf->delete_lc_status_counter <= ARRSIZE(p_rrc_mac_delete_ue_entity_cnf->delete_lc_status));
        for (loop = 0; loop < p_rrc_mac_delete_ue_entity_cnf->delete_lc_status_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_delete_lc_status(pp_buffer, &p_rrc_mac_delete_ue_entity_cnf->delete_lc_status[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_delete_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_DELETE_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_delete_ue_entity_cnf
(
    rrc_mac_delete_ue_entity_cnf_t  *p_rrc_mac_delete_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_delete_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_delete_ue_entity_cnf_len(p_rrc_mac_delete_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_DELETE_UE_ENTITY_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_DELETE_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_DELETE_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_MAC_DELETE_UE_ENTITY_CNF message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_delete_ue_entity_cnf(&p_mac_msg, p_rrc_mac_delete_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_delete_lc_status_len
(
    rrc_mac_delete_lc_status_t *p_rrc_mac_delete_lc_status
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_delete_lc_status != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_delete_lc_status->lch_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_delete_lc_status->status);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_delete_lc_status_tlv_len
(
    rrc_mac_delete_lc_status_t *p_rrc_mac_delete_lc_status

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_delete_lc_status != PNULL);

    length += rrc_il_get_rrc_mac_delete_lc_status_len(p_rrc_mac_delete_lc_status);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_delete_lc_status
(
    U8  **pp_buffer,
    rrc_mac_delete_lc_status_t *p_rrc_mac_delete_lc_status
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_delete_lc_status != PNULL);

    /* This function composes rrc_mac_delete_lc_status */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DELETE_LC_STATUS;
    tlv_length = rrc_il_get_rrc_mac_delete_lc_status_tlv_len(p_rrc_mac_delete_lc_status);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_lc_status->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_status->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_delete_lc_status->lch_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_delete_lc_status->lch_id, "lch_id");
    *pp_buffer += sizeof(p_rrc_mac_delete_lc_status->lch_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_delete_lc_status->status < 1) || (p_rrc_mac_delete_lc_status->status > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_status->status] should be in range "
            "1 to 2. Incorrect value %u received.", p_rrc_mac_delete_lc_status->status);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_delete_lc_status->status, "status");
    *pp_buffer += sizeof(p_rrc_mac_delete_lc_status->status);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_reconfigure_ue_entity_req_len
(
    rrc_mac_reconfigure_ue_entity_req_t *p_rrc_mac_reconfigure_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reconfigure_ue_entity_req->ue_index);

    /* Get length of TLV */
    if (p_rrc_mac_reconfigure_ue_entity_req->optional_elems_present & RRC_MAC_RECONFIG_UE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_reconfig_ue_info_tlv_len(&p_rrc_mac_reconfigure_ue_entity_req->reconfig_ue_info);
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_req->create_lc_req_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_req->create_lc_req));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_req->create_lc_req_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_create_lc_req_tlv_len(&p_rrc_mac_reconfigure_ue_entity_req->create_lc_req[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_reconfigure_lc_req_tlv_len(&p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_delete_lc_req_tlv_len(&p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfigure_ue_entity_req->optional_elems_present & RRC_MAC_RECONFIG_RAT1_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_uplink_rat1_info_tlv_len(&p_rrc_mac_reconfigure_ue_entity_req->uplink_rat1_info);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_reconfigure_ue_entity_req
(
    U8  **pp_buffer,
    rrc_mac_reconfigure_ue_entity_req_t *p_rrc_mac_reconfigure_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_req != PNULL);

    /* This function composes rrc_mac_reconfigure_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_reconfigure_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_reconfigure_ue_entity_req->ue_index);

    /* Compose TLV  */
    if (p_rrc_mac_reconfigure_ue_entity_req->optional_elems_present & RRC_MAC_RECONFIG_UE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_reconfig_ue_info(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_req->reconfig_ue_info))
        {
            return RRC_FAILURE;
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_req->create_lc_req_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_req->create_lc_req));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_req->create_lc_req_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_create_lc_req(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_req->create_lc_req[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_reconfigure_lc_req(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_req->reconfigure_lc_req[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_delete_lc_req(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_req->delete_lc_req[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfigure_ue_entity_req->optional_elems_present & RRC_MAC_RECONFIG_RAT1_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_uplink_rat1_info(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_req->uplink_rat1_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_reconfigure_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_RECONFIGURE_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_reconfigure_ue_entity_req
(
    rrc_mac_reconfigure_ue_entity_req_t  *p_rrc_mac_reconfigure_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_reconfigure_ue_entity_req_len(p_rrc_mac_reconfigure_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_RECONFIGURE_UE_ENTITY_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_RECONFIGURE_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_RECONFIGURE_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_MAC_RECONFIGURE_UE_ENTITY_REQ message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_reconfigure_ue_entity_req(&p_mac_msg, p_rrc_mac_reconfigure_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_max_rb_info_len
(
    rrc_mac_dl_max_rb_info_t *p_rrc_mac_dl_max_rb_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_max_rb_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_dl_max_rb_info->dl_max_rb);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_max_rb_info_tlv_len
(
    rrc_mac_dl_max_rb_info_t *p_rrc_mac_dl_max_rb_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_max_rb_info != PNULL);

    length += rrc_il_get_rrc_mac_dl_max_rb_info_len(p_rrc_mac_dl_max_rb_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_max_rb_info
(
    U8  **pp_buffer,
    rrc_mac_dl_max_rb_info_t *p_rrc_mac_dl_max_rb_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_dl_max_rb_info != PNULL);

    /* This function composes rrc_mac_dl_max_rb_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DL_MAX_RB_INFO;
    tlv_length = rrc_il_get_rrc_mac_dl_max_rb_info_tlv_len(p_rrc_mac_dl_max_rb_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_dl_max_rb_info->dl_max_rb < 1) || (p_rrc_mac_dl_max_rb_info->dl_max_rb > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_dl_max_rb_info->dl_max_rb] should be in range "
            "1 to 100. Incorrect value %u received.", p_rrc_mac_dl_max_rb_info->dl_max_rb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_dl_max_rb_info->dl_max_rb, "dl_max_rb");
    *pp_buffer += sizeof(p_rrc_mac_dl_max_rb_info->dl_max_rb);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_time_alignment_timer_info_len
(
    rrc_mac_time_alignment_timer_info_t *p_rrc_mac_time_alignment_timer_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_time_alignment_timer_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_time_alignment_timer_info->time_alignment_timer);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_time_alignment_timer_info_tlv_len
(
    rrc_mac_time_alignment_timer_info_t *p_rrc_mac_time_alignment_timer_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_time_alignment_timer_info != PNULL);

    length += rrc_il_get_rrc_mac_time_alignment_timer_info_len(p_rrc_mac_time_alignment_timer_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_time_alignment_timer_info
(
    U8  **pp_buffer,
    rrc_mac_time_alignment_timer_info_t *p_rrc_mac_time_alignment_timer_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_time_alignment_timer_info != PNULL);

    /* This function composes rrc_mac_time_alignment_timer_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_TIME_ALIGNMENT_TIME_INFO;
    tlv_length = rrc_il_get_rrc_mac_time_alignment_timer_info_tlv_len(p_rrc_mac_time_alignment_timer_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_time_alignment_timer_info->time_alignment_timer > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_time_alignment_timer_info->time_alignment_timer] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_time_alignment_timer_info->time_alignment_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_time_alignment_timer_info->time_alignment_timer, "time_alignment_timer");
    *pp_buffer += sizeof(p_rrc_mac_time_alignment_timer_info->time_alignment_timer);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_p0_ue_pusch_len
(
    rrc_mac_p0_ue_pusch_t *p_rrc_mac_p0_ue_pusch
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_p0_ue_pusch != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_p0_ue_pusch->p0_ue_pusch);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_p0_ue_pusch_tlv_len
(
    rrc_mac_p0_ue_pusch_t *p_rrc_mac_p0_ue_pusch

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_p0_ue_pusch != PNULL);

    length += rrc_il_get_rrc_mac_p0_ue_pusch_len(p_rrc_mac_p0_ue_pusch);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_p0_ue_pusch
(
    U8  **pp_buffer,
    rrc_mac_p0_ue_pusch_t *p_rrc_mac_p0_ue_pusch
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_p0_ue_pusch != PNULL);

    /* This function composes rrc_mac_p0_ue_pusch */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_P0_UE_PUSCH;
    tlv_length = rrc_il_get_rrc_mac_p0_ue_pusch_tlv_len(p_rrc_mac_p0_ue_pusch);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_p0_ue_pusch->p0_ue_pusch < -8) || (p_rrc_mac_p0_ue_pusch->p0_ue_pusch > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_p0_ue_pusch->p0_ue_pusch] should be in range "
            "-8 to 7. Incorrect value %d received.", p_rrc_mac_p0_ue_pusch->p0_ue_pusch);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_mac_p0_ue_pusch->p0_ue_pusch, "p0_ue_pusch");
    *pp_buffer += sizeof(p_rrc_mac_p0_ue_pusch->p0_ue_pusch);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_max_ul_harq_tx_info_len
(
    rrc_mac_max_ul_harq_tx_info_t *p_rrc_mac_max_ul_harq_tx_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_max_ul_harq_tx_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_max_ul_harq_tx_info->max_ul_harq_tx);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_max_ul_harq_tx_info_tlv_len
(
    rrc_mac_max_ul_harq_tx_info_t *p_rrc_mac_max_ul_harq_tx_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_max_ul_harq_tx_info != PNULL);

    length += rrc_il_get_rrc_mac_max_ul_harq_tx_info_len(p_rrc_mac_max_ul_harq_tx_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_max_ul_harq_tx_info
(
    U8  **pp_buffer,
    rrc_mac_max_ul_harq_tx_info_t *p_rrc_mac_max_ul_harq_tx_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_max_ul_harq_tx_info != PNULL);

    /* This function composes rrc_mac_max_ul_harq_tx_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MAX_UL_HARQ_TX_INFO;
    tlv_length = rrc_il_get_rrc_mac_max_ul_harq_tx_info_tlv_len(p_rrc_mac_max_ul_harq_tx_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_max_ul_harq_tx_info->max_ul_harq_tx > 13))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_max_ul_harq_tx_info->max_ul_harq_tx] should be less than"
            " or equal to 13. Incorrect value %u received.", p_rrc_mac_max_ul_harq_tx_info->max_ul_harq_tx);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_max_ul_harq_tx_info->max_ul_harq_tx, "max_ul_harq_tx");
    *pp_buffer += sizeof(p_rrc_mac_max_ul_harq_tx_info->max_ul_harq_tx);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pc_max_config_info_len
(
    rrc_mac_pc_max_config_info_t *p_rrc_mac_pc_max_config_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pc_max_config_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pc_max_config_info->pc_max);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_pc_max_config_info_tlv_len
(
    rrc_mac_pc_max_config_info_t *p_rrc_mac_pc_max_config_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pc_max_config_info != PNULL);

    length += rrc_il_get_rrc_mac_pc_max_config_info_len(p_rrc_mac_pc_max_config_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_pc_max_config_info
(
    U8  **pp_buffer,
    rrc_mac_pc_max_config_info_t *p_rrc_mac_pc_max_config_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_pc_max_config_info != PNULL);

    /* This function composes rrc_mac_pc_max_config_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PC_MAX_INFO;
    tlv_length = rrc_il_get_rrc_mac_pc_max_config_info_tlv_len(p_rrc_mac_pc_max_config_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_pc_max_config_info->pc_max < -40) || (p_rrc_mac_pc_max_config_info->pc_max > 23))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pc_max_config_info->pc_max] should be in range "
            "-40 to 23. Incorrect value %d received.", p_rrc_mac_pc_max_config_info->pc_max);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_rrc_mac_pc_max_config_info->pc_max, "pc_max");
    *pp_buffer += sizeof(p_rrc_mac_pc_max_config_info->pc_max);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacUeCategoryV1310_len
(
    RrcMacUeCategoryV1310_t *p_RrcMacUeCategoryV1310
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacUeCategoryV1310 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacUeCategoryV1310->ueCategoryV1310);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacUeCategoryV1310_tlv_len
(
    RrcMacUeCategoryV1310_t *p_RrcMacUeCategoryV1310

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacUeCategoryV1310 != PNULL);

    length += rrc_il_get_RrcMacUeCategoryV1310_len(p_RrcMacUeCategoryV1310);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacUeCategoryV1310
(
    U8  **pp_buffer,
    RrcMacUeCategoryV1310_t *p_RrcMacUeCategoryV1310
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacUeCategoryV1310 != PNULL);

    /* This function composes RrcMacUeCategoryV1310 */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UE_CATEGORY_V1310;
    tlv_length = rrc_il_get_RrcMacUeCategoryV1310_tlv_len(p_RrcMacUeCategoryV1310);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacUeCategoryV1310->ueCategoryV1310 > RRC_ONE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacUeCategoryV1310->ueCategoryV1310] should be less than"
            " or equal to RRC_ONE. Incorrect value %u received.", p_RrcMacUeCategoryV1310->ueCategoryV1310);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacUeCategoryV1310->ueCategoryV1310, "ueCategoryV1310");
    *pp_buffer += sizeof(p_RrcMacUeCategoryV1310->ueCategoryV1310);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_reconfig_ue_info_len
(
    rrc_mac_reconfig_ue_info_t *p_rrc_mac_reconfig_ue_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reconfig_ue_info != PNULL);

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONF_CQI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_cqi_info_tlv_len(&p_rrc_mac_reconfig_ue_info->cqi_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_DL_MAX_RB_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_dl_max_rb_info_tlv_len(&p_rrc_mac_reconfig_ue_info->dl_max_rb_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_TX_MODE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tx_mode_info_tlv_len(&p_rrc_mac_reconfig_ue_info->tx_mode_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_NUM_OF_LAYER_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_num_of_layer_info_tlv_len(&p_rrc_mac_reconfig_ue_info->num_of_layer_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_CODE_BOOK_INDEX_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_code_book_index_info_tlv_len(&p_rrc_mac_reconfig_ue_info->code_book_index_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONF_SR_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sr_config_info_tlv_len(&p_rrc_mac_reconfig_ue_info->sr_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_SR_PROHBIT_TIMER_VALUE_V920_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sr_prohbit_timer_value_v920_tlv_len(&p_rrc_mac_reconfig_ue_info->sr_prohbit_timer_value_v920);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_UE_CATEGORY_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ue_cat_tlv_len(&p_rrc_mac_reconfig_ue_info->ue_category_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_SRS_DEDICATED_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_srs_dedicated_config_tlv_len(&p_rrc_mac_reconfig_ue_info->srs_dedicated_config);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
    {
        length += rrc_il_get_rrc_mac_codebook_subset_restriction_tlv_len(&p_rrc_mac_reconfig_ue_info->codebook_subset_restriction);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_DRX_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_drx_config_info_tlv_len(&p_rrc_mac_reconfig_ue_info->drx_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_MEAS_GAP_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_meas_gap_config_info_tlv_len(&p_rrc_mac_reconfig_ue_info->meas_gap_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_TIME_ALIGNMENT_TIME_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_time_alignment_timer_info_tlv_len(&p_rrc_mac_reconfig_ue_info->time_alignment_timer_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT)
    {
        length += rrc_il_get_rrc_mac_codebook_subset_restriction_v920_tlv_len(&p_rrc_mac_reconfig_ue_info->codebook_subset_restriction_v920);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_PDSCH_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pdsch_config_info_tlv_len(&p_rrc_mac_reconfig_ue_info->pdsch_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_P0_UE_PUSCH_PRESENT)
    {
        length += rrc_il_get_rrc_mac_p0_ue_pusch_tlv_len(&p_rrc_mac_reconfig_ue_info->p0_ue_pusch);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_TPC_PDCCH_CONFIG_PUCCH_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tpc_pdcch_config_pucch_tlv_len(&p_rrc_mac_reconfig_ue_info->tpc_pdcch_config_pucch);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_TPC_PDCCH_CONFIG_PUSCH_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tpc_pdcch_config_pusch_tlv_len(&p_rrc_mac_reconfig_ue_info->tpc_pdcch_config_pusch);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_AMBR_QOS_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ambr_qos_info_tlv_len(&p_rrc_mac_reconfig_ue_info->ambr_qos_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_UE_CAPABILITY_ADDITIONAL_PARAM_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ue_capability_additional_params_tlv_len(&p_rrc_mac_reconfig_ue_info->ue_capability_additional_params);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_SPS_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sps_config_tlv_len(&p_rrc_mac_reconfig_ue_info->sps_config);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_RF_PARAMS_PRESENT)
    {
        length += rrc_il_get_rrc_mac_rf_params_tlv_len(&p_rrc_mac_reconfig_ue_info->rf_params);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_PHR_TIMER_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_phr_timer_config_tlv_len(&p_rrc_mac_reconfig_ue_info->phr_timer_config);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_UE_COMPLIANCE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ue_compliance_info_tlv_len(&p_rrc_mac_reconfig_ue_info->ue_compliance_release);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_PDSCH_EPRE_TO_UE_RS_RATIO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pdsch_epre_to_ue_rs_ratio_tlv_len(&p_rrc_mac_reconfig_ue_info->pdsch_epre_to_ue_rs_ratio);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_TTI_BUNDLING_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_tti_bundling_info_tlv_len(&p_rrc_mac_reconfig_ue_info->tti_bundling);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_MAX_UL_HARQ_TX_PRESENT)
    {
        length += rrc_il_get_rrc_mac_max_ul_harq_tx_info_tlv_len(&p_rrc_mac_reconfig_ue_info->max_ul_harq_tx_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_PC_MAX_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pc_max_config_info_tlv_len(&p_rrc_mac_reconfig_ue_info->pc_max_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_PUSCH_CONFIG_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pusch_config_info_tlv_len(&p_rrc_mac_reconfig_ue_info->pusch_config_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT)
    {
        length += rrc_il_get_rrc_mac_codebook_subset_restriction_v1020_tlv_len(&p_rrc_mac_reconfig_ue_info->rrc_mac_codebook_subset_restriction_v1020);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_ADD_DL_INTERFERENCE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_dl_interference_info_tlv_len(&p_rrc_mac_reconfig_ue_info->rrc_mac_dl_interference_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_OPERATOR_ID_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_operator_info_tlv_len(&p_rrc_mac_reconfig_ue_info->operator_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present & RRC_MAC_RECONFIG_PUCCH_CONFIG_DEDICATED_R13_PRESENT)
    {
        length += rrc_il_get_PucchConfigDedicatedR13_tlv_len(&p_rrc_mac_reconfig_ue_info->pucchConfigDedicatedR13);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present & RRC_MAC_RECONFIG_EPDCCH_SET_CONFIG_R11_PRESENT)
    {
        length += rrc_il_get_EpdcchSetConfigR11_tlv_len(&p_rrc_mac_reconfig_ue_info->epdcchSetConfigR11);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present & RRC_MAC_RECONFIG_CE_LEVEL_PRESENT)
    {
        length += rrc_il_get_RrcMacCeLevel_tlv_len(&p_rrc_mac_reconfig_ue_info->ceLevel);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present & RRC_MAC_RECONFIG_CE_MODE_PRESENT)
    {
        length += rrc_il_get_RrcMacCeMode_tlv_len(&p_rrc_mac_reconfig_ue_info->ceMode);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present & RRC_MAC_RECONFIG_UE_CATEGORY_V1310_PRESENT)
    {
        length += rrc_il_get_RrcMacUeCategoryV1310_tlv_len(&p_rrc_mac_reconfig_ue_info->ueCategoryV1310);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->ca_optional_elems_present & RRC_MAC_RECONFIG_ADD_UE_CATEGORY_V1020_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ue_category_v1020_tlv_len(&p_rrc_mac_reconfig_ue_info->rrc_mac_ue_category_v1020);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->ca_optional_elems_present & RRC_MAC_RECONFIG_ADD_PUCCH_FORMAT_V1020_PRESENT)
    {
        length += rrc_il_get_rrc_mac_pucch_format_r10_tlv_len(&p_rrc_mac_reconfig_ue_info->rrc_mac_pucch_format_r10);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfig_ue_info->ca_optional_elems_present & RRC_MAC_CA_RECONFIG_ADD_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_carrier_aggr_config_tlv_len(&p_rrc_mac_reconfig_ue_info->rrc_mac_carrier_aggr_config);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_reconfig_ue_info_tlv_len
(
    rrc_mac_reconfig_ue_info_t *p_rrc_mac_reconfig_ue_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reconfig_ue_info != PNULL);

    length += rrc_il_get_rrc_mac_reconfig_ue_info_len(p_rrc_mac_reconfig_ue_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_reconfig_ue_info
(
    U8  **pp_buffer,
    rrc_mac_reconfig_ue_info_t *p_rrc_mac_reconfig_ue_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_reconfig_ue_info != PNULL);

    /* This function composes rrc_mac_reconfig_ue_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RECONFIG_UE_INFO;
    tlv_length = rrc_il_get_rrc_mac_reconfig_ue_info_tlv_len(p_rrc_mac_reconfig_ue_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONF_CQI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_cqi_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->cqi_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_DL_MAX_RB_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_dl_max_rb_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->dl_max_rb_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_TX_MODE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tx_mode_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->tx_mode_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_NUM_OF_LAYER_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_num_of_layer_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->num_of_layer_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_CODE_BOOK_INDEX_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_code_book_index_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->code_book_index_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONF_SR_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sr_config_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->sr_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_SR_PROHBIT_TIMER_VALUE_V920_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sr_prohbit_timer_value_v920(pp_buffer, &p_rrc_mac_reconfig_ue_info->sr_prohbit_timer_value_v920))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_UE_CATEGORY_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ue_cat(pp_buffer, &p_rrc_mac_reconfig_ue_info->ue_category_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_SRS_DEDICATED_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_srs_dedicated_config(pp_buffer, &p_rrc_mac_reconfig_ue_info->srs_dedicated_config))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_codebook_subset_restriction(pp_buffer, &p_rrc_mac_reconfig_ue_info->codebook_subset_restriction))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_DRX_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_drx_config_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->drx_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_MEAS_GAP_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_meas_gap_config_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->meas_gap_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_TIME_ALIGNMENT_TIME_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_time_alignment_timer_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->time_alignment_timer_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_codebook_subset_restriction_v920(pp_buffer, &p_rrc_mac_reconfig_ue_info->codebook_subset_restriction_v920))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_PDSCH_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pdsch_config_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->pdsch_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_P0_UE_PUSCH_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_p0_ue_pusch(pp_buffer, &p_rrc_mac_reconfig_ue_info->p0_ue_pusch))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_TPC_PDCCH_CONFIG_PUCCH_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tpc_pdcch_config_pucch(pp_buffer, &p_rrc_mac_reconfig_ue_info->tpc_pdcch_config_pucch))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_TPC_PDCCH_CONFIG_PUSCH_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tpc_pdcch_config_pusch(pp_buffer, &p_rrc_mac_reconfig_ue_info->tpc_pdcch_config_pusch))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_AMBR_QOS_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ambr_qos_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->ambr_qos_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_UE_CAPABILITY_ADDITIONAL_PARAM_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ue_capability_additional_params(pp_buffer, &p_rrc_mac_reconfig_ue_info->ue_capability_additional_params))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_SPS_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sps_config(pp_buffer, &p_rrc_mac_reconfig_ue_info->sps_config))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_RF_PARAMS_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_rf_params(pp_buffer, &p_rrc_mac_reconfig_ue_info->rf_params))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_PHR_TIMER_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_phr_timer_config(pp_buffer, &p_rrc_mac_reconfig_ue_info->phr_timer_config))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_UE_COMPLIANCE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ue_compliance_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->ue_compliance_release))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_PDSCH_EPRE_TO_UE_RS_RATIO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pdsch_epre_to_ue_rs_ratio(pp_buffer, &p_rrc_mac_reconfig_ue_info->pdsch_epre_to_ue_rs_ratio))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_TTI_BUNDLING_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_tti_bundling_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->tti_bundling))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_MAX_UL_HARQ_TX_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_max_ul_harq_tx_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->max_ul_harq_tx_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_PC_MAX_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pc_max_config_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->pc_max_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_PUSCH_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pusch_config_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->pusch_config_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_CODEBOOK_SUBSET_RESTRICTION_V1020_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_codebook_subset_restriction_v1020(pp_buffer, &p_rrc_mac_reconfig_ue_info->rrc_mac_codebook_subset_restriction_v1020))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_ADD_DL_INTERFERENCE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_dl_interference_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->rrc_mac_dl_interference_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->optional_elems_present & RRC_MAC_RECONFIG_OPERATOR_ID_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_operator_info(pp_buffer, &p_rrc_mac_reconfig_ue_info->operator_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present & RRC_MAC_RECONFIG_PUCCH_CONFIG_DEDICATED_R13_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_PucchConfigDedicatedR13(pp_buffer, &p_rrc_mac_reconfig_ue_info->pucchConfigDedicatedR13))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present & RRC_MAC_RECONFIG_EPDCCH_SET_CONFIG_R11_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_EpdcchSetConfigR11(pp_buffer, &p_rrc_mac_reconfig_ue_info->epdcchSetConfigR11))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present & RRC_MAC_RECONFIG_CE_LEVEL_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacCeLevel(pp_buffer, &p_rrc_mac_reconfig_ue_info->ceLevel))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present & RRC_MAC_RECONFIG_CE_MODE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacCeMode(pp_buffer, &p_rrc_mac_reconfig_ue_info->ceMode))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->emtc_optional_elems_present & RRC_MAC_RECONFIG_UE_CATEGORY_V1310_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacUeCategoryV1310(pp_buffer, &p_rrc_mac_reconfig_ue_info->ueCategoryV1310))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->ca_optional_elems_present & RRC_MAC_RECONFIG_ADD_UE_CATEGORY_V1020_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ue_category_v1020(pp_buffer, &p_rrc_mac_reconfig_ue_info->rrc_mac_ue_category_v1020))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->ca_optional_elems_present & RRC_MAC_RECONFIG_ADD_PUCCH_FORMAT_V1020_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_pucch_format_r10(pp_buffer, &p_rrc_mac_reconfig_ue_info->rrc_mac_pucch_format_r10))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfig_ue_info->ca_optional_elems_present & RRC_MAC_CA_RECONFIG_ADD_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_carrier_aggr_config(pp_buffer, &p_rrc_mac_reconfig_ue_info->rrc_mac_carrier_aggr_config))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ul_lc_reconfig_req_len
(
    rrc_mac_ul_lc_reconfig_req_t *p_rrc_mac_ul_lc_reconfig_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ul_lc_reconfig_req != PNULL);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ul_lc_reconfig_req_tlv_len
(
    rrc_mac_ul_lc_reconfig_req_t *p_rrc_mac_ul_lc_reconfig_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ul_lc_reconfig_req != PNULL);

    length += rrc_il_get_rrc_mac_ul_lc_reconfig_req_len(p_rrc_mac_ul_lc_reconfig_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ul_lc_reconfig_req
(
    U8  **pp_buffer,
    rrc_mac_ul_lc_reconfig_req_t *p_rrc_mac_ul_lc_reconfig_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ul_lc_reconfig_req != PNULL);

    /* This function composes rrc_mac_ul_lc_reconfig_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_UL_LC_RECONFIGURE_REQ;
    tlv_length = rrc_il_get_rrc_mac_ul_lc_reconfig_req_tlv_len(p_rrc_mac_ul_lc_reconfig_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_lc_reconfig_req_len
(
    rrc_mac_dl_lc_reconfig_req_t *p_rrc_mac_dl_lc_reconfig_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_lc_reconfig_req != PNULL);

    /* Get length of TLV */
    if (p_rrc_mac_dl_lc_reconfig_req->optional_elems_present & RRC_MAC_DL_RECONFIG_CA_LC_BANDWIDTH_DIST_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ca_lc_bandwidth_dist_info_tlv_len(&p_rrc_mac_dl_lc_reconfig_req->ca_lc_bandwidth_dist_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_dl_lc_reconfig_req_tlv_len
(
    rrc_mac_dl_lc_reconfig_req_t *p_rrc_mac_dl_lc_reconfig_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_dl_lc_reconfig_req != PNULL);

    length += rrc_il_get_rrc_mac_dl_lc_reconfig_req_len(p_rrc_mac_dl_lc_reconfig_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_dl_lc_reconfig_req
(
    U8  **pp_buffer,
    rrc_mac_dl_lc_reconfig_req_t *p_rrc_mac_dl_lc_reconfig_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_dl_lc_reconfig_req != PNULL);

    /* This function composes rrc_mac_dl_lc_reconfig_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DL_LC_RECONFIGURE_REQ;
    tlv_length = rrc_il_get_rrc_mac_dl_lc_reconfig_req_tlv_len(p_rrc_mac_dl_lc_reconfig_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose TLV  */
    if (p_rrc_mac_dl_lc_reconfig_req->optional_elems_present & RRC_MAC_DL_RECONFIG_CA_LC_BANDWIDTH_DIST_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ca_lc_bandwidth_dist_info(pp_buffer, &p_rrc_mac_dl_lc_reconfig_req->ca_lc_bandwidth_dist_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_reconfigure_lc_req_len
(
    rrc_mac_reconfigure_lc_req_t *p_rrc_mac_reconfigure_lc_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reconfigure_lc_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reconfigure_lc_req->lch_id);

    /* Get length of TLV */
    if (p_rrc_mac_reconfigure_lc_req->optional_elems_present & RRC_MAC_UL_LC_RECONFIG_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ul_lc_reconfig_req_tlv_len(&p_rrc_mac_reconfigure_lc_req->ul_lc_reconfig_req);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfigure_lc_req->optional_elems_present & RRC_MAC_DL_LC_RECONFIG_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_dl_lc_reconfig_req_tlv_len(&p_rrc_mac_reconfigure_lc_req->dl_lc_reconfig_req);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfigure_lc_req->optional_elems_present & RRC_MAC_GBR_QOS_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_gbr_qos_info_tlv_len(&p_rrc_mac_reconfigure_lc_req->gbr_qos_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_reconfigure_lc_req_tlv_len
(
    rrc_mac_reconfigure_lc_req_t *p_rrc_mac_reconfigure_lc_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reconfigure_lc_req != PNULL);

    length += rrc_il_get_rrc_mac_reconfigure_lc_req_len(p_rrc_mac_reconfigure_lc_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_reconfigure_lc_req
(
    U8  **pp_buffer,
    rrc_mac_reconfigure_lc_req_t *p_rrc_mac_reconfigure_lc_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_reconfigure_lc_req != PNULL);

    /* This function composes rrc_mac_reconfigure_lc_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RECONFIGURE_LC_REQ;
    tlv_length = rrc_il_get_rrc_mac_reconfigure_lc_req_tlv_len(p_rrc_mac_reconfigure_lc_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfigure_lc_req->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfigure_lc_req->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_reconfigure_lc_req->lch_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_reconfigure_lc_req->lch_id, "lch_id");
    *pp_buffer += sizeof(p_rrc_mac_reconfigure_lc_req->lch_id);

    /* Compose TLV  */
    if (p_rrc_mac_reconfigure_lc_req->optional_elems_present & RRC_MAC_UL_LC_RECONFIG_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ul_lc_reconfig_req(pp_buffer, &p_rrc_mac_reconfigure_lc_req->ul_lc_reconfig_req))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfigure_lc_req->optional_elems_present & RRC_MAC_DL_LC_RECONFIG_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_dl_lc_reconfig_req(pp_buffer, &p_rrc_mac_reconfigure_lc_req->dl_lc_reconfig_req))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfigure_lc_req->optional_elems_present & RRC_MAC_GBR_QOS_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_gbr_qos_info(pp_buffer, &p_rrc_mac_reconfigure_lc_req->gbr_qos_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_delete_lc_req_len
(
    rrc_mac_delete_lc_req_t *p_rrc_mac_delete_lc_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_delete_lc_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_delete_lc_req->lch_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_delete_lc_req->lc_type);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_delete_lc_req_tlv_len
(
    rrc_mac_delete_lc_req_t *p_rrc_mac_delete_lc_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_delete_lc_req != PNULL);

    length += rrc_il_get_rrc_mac_delete_lc_req_len(p_rrc_mac_delete_lc_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_delete_lc_req
(
    U8  **pp_buffer,
    rrc_mac_delete_lc_req_t *p_rrc_mac_delete_lc_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_delete_lc_req != PNULL);

    /* This function composes rrc_mac_delete_lc_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DELETE_LC_REQ;
    tlv_length = rrc_il_get_rrc_mac_delete_lc_req_tlv_len(p_rrc_mac_delete_lc_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_lc_req->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_req->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_delete_lc_req->lch_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_delete_lc_req->lch_id, "lch_id");
    *pp_buffer += sizeof(p_rrc_mac_delete_lc_req->lch_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_delete_lc_req->lc_type < 1) || (p_rrc_mac_delete_lc_req->lc_type > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_req->lc_type] should be in range "
            "1 to 3. Incorrect value %u received.", p_rrc_mac_delete_lc_req->lc_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_delete_lc_req->lc_type, "lc_type");
    *pp_buffer += sizeof(p_rrc_mac_delete_lc_req->lc_type);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_reconfigure_ue_entity_cnf_len
(
    rrc_mac_reconfigure_ue_entity_cnf_t *p_rrc_mac_reconfigure_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reconfigure_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reconfigure_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_create_lc_error_tlv_len(&p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_reconfigure_lc_error_tlv_len(&p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_delete_lc_error_tlv_len(&p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_delete_lc_status_tlv_len(&p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfigure_ue_entity_cnf->optional_elems_present & RRC_MAC_RECONF_SPS_CRNTI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sps_crnti_info_tlv_len(&p_rrc_mac_reconfigure_ue_entity_cnf->sps_crnti_info);
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->ca_config_error_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_create_ca_config_err));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->ca_config_error_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_ca_config_error_tlv_len(&p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_create_ca_config_err[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->reconfig_ca_config_error_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_reconfigure_ca_config_err));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->reconfig_ca_config_error_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_ca_reconfig_error_tlv_len(&p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_reconfigure_ca_config_err[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->delete_ca_config_error_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_delete_ca_config_err));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->delete_ca_config_error_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_delete_ca_config_error_tlv_len(&p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_delete_ca_config_err[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_reconfigure_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_mac_reconfigure_ue_entity_cnf_t *p_rrc_mac_reconfigure_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf != PNULL);

    /* This function composes rrc_mac_reconfigure_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_reconfigure_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_reconfigure_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfigure_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfigure_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_reconfigure_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_reconfigure_ue_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_mac_reconfigure_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_create_lc_error(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_cnf->create_lc_error[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_reconfigure_lc_error(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_cnf->reconfigure_lc_error[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_delete_lc_error(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_error[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_delete_lc_status(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_cnf->delete_lc_status[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfigure_ue_entity_cnf->optional_elems_present & RRC_MAC_RECONF_SPS_CRNTI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sps_crnti_info(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_cnf->sps_crnti_info))
        {
            return RRC_FAILURE;
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->ca_config_error_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_create_ca_config_err));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->ca_config_error_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_ca_config_error(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_create_ca_config_err[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->reconfig_ca_config_error_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_reconfigure_ca_config_err));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->reconfig_ca_config_error_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_ca_reconfig_error(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_reconfigure_ca_config_err[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf->delete_ca_config_error_counter <= ARRSIZE(p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_delete_ca_config_err));
        for (loop = 0; loop < p_rrc_mac_reconfigure_ue_entity_cnf->delete_ca_config_error_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_delete_ca_config_error(pp_buffer, &p_rrc_mac_reconfigure_ue_entity_cnf->rrc_mac_delete_ca_config_err[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_reconfigure_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_RECONFIGURE_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_reconfigure_ue_entity_cnf
(
    rrc_mac_reconfigure_ue_entity_cnf_t  *p_rrc_mac_reconfigure_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_reconfigure_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_reconfigure_ue_entity_cnf_len(p_rrc_mac_reconfigure_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_RECONFIGURE_UE_ENTITY_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_RECONFIGURE_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_RECONFIGURE_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_MAC_RECONFIGURE_UE_ENTITY_CNF message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_reconfigure_ue_entity_cnf(&p_mac_msg, p_rrc_mac_reconfigure_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_reconfigure_lc_error_len
(
    rrc_mac_reconfigure_lc_error_t *p_rrc_mac_reconfigure_lc_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reconfigure_lc_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reconfigure_lc_error->lch_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reconfigure_lc_error->response);

    /* Get length of TLV */
    if (p_rrc_mac_reconfigure_lc_error->optional_elems_present & RRC_MAC_RECONF_UL_LC_CONFIG_RESP_PRESENT)
    {
        length += rrc_il_get_rrc_mac_ul_lc_config_resp_tlv_len(&p_rrc_mac_reconfigure_lc_error->ul_lc_config_resp);
    }

    /* Get length of TLV */
    if (p_rrc_mac_reconfigure_lc_error->optional_elems_present & RRC_MAC_RECONF_DL_LC_CONFIG_RESP_PRESENT)
    {
        length += rrc_il_get_rrc_mac_dl_lc_config_resp_tlv_len(&p_rrc_mac_reconfigure_lc_error->dl_lc_config_resp);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_reconfigure_lc_error_tlv_len
(
    rrc_mac_reconfigure_lc_error_t *p_rrc_mac_reconfigure_lc_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reconfigure_lc_error != PNULL);

    length += rrc_il_get_rrc_mac_reconfigure_lc_error_len(p_rrc_mac_reconfigure_lc_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_reconfigure_lc_error
(
    U8  **pp_buffer,
    rrc_mac_reconfigure_lc_error_t *p_rrc_mac_reconfigure_lc_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_reconfigure_lc_error != PNULL);

    /* This function composes rrc_mac_reconfigure_lc_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RECONFIGURE_LC_ERROR;
    tlv_length = rrc_il_get_rrc_mac_reconfigure_lc_error_tlv_len(p_rrc_mac_reconfigure_lc_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfigure_lc_error->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfigure_lc_error->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_reconfigure_lc_error->lch_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_reconfigure_lc_error->lch_id, "lch_id");
    *pp_buffer += sizeof(p_rrc_mac_reconfigure_lc_error->lch_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfigure_lc_error->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfigure_lc_error->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_reconfigure_lc_error->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_reconfigure_lc_error->response, "response");
    *pp_buffer += sizeof(p_rrc_mac_reconfigure_lc_error->response);

    /* Compose TLV  */
    if (p_rrc_mac_reconfigure_lc_error->optional_elems_present & RRC_MAC_RECONF_UL_LC_CONFIG_RESP_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_ul_lc_config_resp(pp_buffer, &p_rrc_mac_reconfigure_lc_error->ul_lc_config_resp))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_reconfigure_lc_error->optional_elems_present & RRC_MAC_RECONF_DL_LC_CONFIG_RESP_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_dl_lc_config_resp(pp_buffer, &p_rrc_mac_reconfigure_lc_error->dl_lc_config_resp))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_delete_lc_error_len
(
    rrc_mac_delete_lc_error_t *p_rrc_mac_delete_lc_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_delete_lc_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_delete_lc_error->lch_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_delete_lc_error->response);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_delete_lc_error->lc_type);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_delete_lc_error_tlv_len
(
    rrc_mac_delete_lc_error_t *p_rrc_mac_delete_lc_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_delete_lc_error != PNULL);

    length += rrc_il_get_rrc_mac_delete_lc_error_len(p_rrc_mac_delete_lc_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_delete_lc_error
(
    U8  **pp_buffer,
    rrc_mac_delete_lc_error_t *p_rrc_mac_delete_lc_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_delete_lc_error != PNULL);

    /* This function composes rrc_mac_delete_lc_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DELETE_LC_ERROR;
    tlv_length = rrc_il_get_rrc_mac_delete_lc_error_tlv_len(p_rrc_mac_delete_lc_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_lc_error->lch_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_error->lch_id] should be less than"
            " or equal to 10. Incorrect value %u received.", p_rrc_mac_delete_lc_error->lch_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_delete_lc_error->lch_id, "lch_id");
    *pp_buffer += sizeof(p_rrc_mac_delete_lc_error->lch_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_lc_error->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_error->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_delete_lc_error->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_delete_lc_error->response, "response");
    *pp_buffer += sizeof(p_rrc_mac_delete_lc_error->response);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_lc_error->lc_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_lc_error->lc_type] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_delete_lc_error->lc_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_delete_lc_error->lc_type, "lc_type");
    *pp_buffer += sizeof(p_rrc_mac_delete_lc_error->lc_type);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ca_reconfig_error_len
(
    rrc_mac_ca_reconfig_error_t *p_rrc_mac_ca_reconfig_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ca_reconfig_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ca_reconfig_error->cell_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ca_reconfig_error->error_code);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_ca_reconfig_error_tlv_len
(
    rrc_mac_ca_reconfig_error_t *p_rrc_mac_ca_reconfig_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ca_reconfig_error != PNULL);

    length += rrc_il_get_rrc_mac_ca_reconfig_error_len(p_rrc_mac_ca_reconfig_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_ca_reconfig_error
(
    U8  **pp_buffer,
    rrc_mac_ca_reconfig_error_t *p_rrc_mac_ca_reconfig_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ca_reconfig_error != PNULL);

    /* This function composes rrc_mac_ca_reconfig_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RECONFIGURE_CA_CONFIG_ERROR;
    tlv_length = rrc_il_get_rrc_mac_ca_reconfig_error_tlv_len(p_rrc_mac_ca_reconfig_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ca_reconfig_error->cell_id > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ca_reconfig_error->cell_id] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_ca_reconfig_error->cell_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_ca_reconfig_error->cell_id, "cell_id");
    *pp_buffer += sizeof(p_rrc_mac_ca_reconfig_error->cell_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ca_reconfig_error->error_code < 6) || (p_rrc_mac_ca_reconfig_error->error_code > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ca_reconfig_error->error_code] should be in range "
            "6 to 6. Incorrect value %u received.", p_rrc_mac_ca_reconfig_error->error_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ca_reconfig_error->error_code, "error_code");
    *pp_buffer += sizeof(p_rrc_mac_ca_reconfig_error->error_code);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_delete_ca_config_error_len
(
    rrc_mac_delete_ca_config_error_t *p_rrc_mac_delete_ca_config_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_delete_ca_config_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_delete_ca_config_error->cell_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_delete_ca_config_error->error_code);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_delete_ca_config_error_tlv_len
(
    rrc_mac_delete_ca_config_error_t *p_rrc_mac_delete_ca_config_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_delete_ca_config_error != PNULL);

    length += rrc_il_get_rrc_mac_delete_ca_config_error_len(p_rrc_mac_delete_ca_config_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_delete_ca_config_error
(
    U8  **pp_buffer,
    rrc_mac_delete_ca_config_error_t *p_rrc_mac_delete_ca_config_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_delete_ca_config_error != PNULL);

    /* This function composes rrc_mac_delete_ca_config_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DELETE_CA_CONFIG_ERROR;
    tlv_length = rrc_il_get_rrc_mac_delete_ca_config_error_tlv_len(p_rrc_mac_delete_ca_config_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_delete_ca_config_error->cell_id > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_ca_config_error->cell_id] should be less than"
            " or equal to 7. Incorrect value %u received.", p_rrc_mac_delete_ca_config_error->cell_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_delete_ca_config_error->cell_id, "cell_id");
    *pp_buffer += sizeof(p_rrc_mac_delete_ca_config_error->cell_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_delete_ca_config_error->error_code < 6) || (p_rrc_mac_delete_ca_config_error->error_code > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_delete_ca_config_error->error_code] should be in range "
            "6 to 6. Incorrect value %u received.", p_rrc_mac_delete_ca_config_error->error_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_delete_ca_config_error->error_code, "error_code");
    *pp_buffer += sizeof(p_rrc_mac_delete_ca_config_error->error_code);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_ue_entity_power_headroom_ind_len
(
    rrc_mac_ue_entity_power_headroom_ind_t *p_rrc_mac_ue_entity_power_headroom_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ue_entity_power_headroom_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ue_entity_power_headroom_ind->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ue_entity_power_headroom_ind->crnti);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ue_entity_power_headroom_ind->power_headroom);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_ue_entity_power_headroom_ind
(
    U8  **pp_buffer,
    rrc_mac_ue_entity_power_headroom_ind_t *p_rrc_mac_ue_entity_power_headroom_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ue_entity_power_headroom_ind != PNULL);

    /* This function composes rrc_mac_ue_entity_power_headroom_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ue_entity_power_headroom_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_ue_entity_power_headroom_ind->ue_index);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ue_entity_power_headroom_ind->crnti < 1) || (p_rrc_mac_ue_entity_power_headroom_ind->crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_entity_power_headroom_ind->crnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_ue_entity_power_headroom_ind->crnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ue_entity_power_headroom_ind->crnti, "crnti");
    *pp_buffer += sizeof(p_rrc_mac_ue_entity_power_headroom_ind->crnti);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ue_entity_power_headroom_ind->power_headroom > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_entity_power_headroom_ind->power_headroom] should be less than"
            " or equal to 63. Incorrect value %u received.", p_rrc_mac_ue_entity_power_headroom_ind->power_headroom);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ue_entity_power_headroom_ind->power_headroom, "power_headroom");
    *pp_buffer += sizeof(p_rrc_mac_ue_entity_power_headroom_ind->power_headroom);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_ue_entity_power_headroom_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_UE_ENTITY_POWER_HEADROOM_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_ue_entity_power_headroom_ind
(
    rrc_mac_ue_entity_power_headroom_ind_t  *p_rrc_mac_ue_entity_power_headroom_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_ue_entity_power_headroom_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_ue_entity_power_headroom_ind_len(p_rrc_mac_ue_entity_power_headroom_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_UE_ENTITY_POWER_HEADROOM_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_UE_ENTITY_POWER_HEADROOM_IND, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_UE_ENTITY_POWER_HEADROOM_IND, msg_length,cell_index);

    /* Fill RRC_MAC_UE_ENTITY_POWER_HEADROOM_IND message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_ue_entity_power_headroom_ind(&p_mac_msg, p_rrc_mac_ue_entity_power_headroom_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_ue_drx_cmd_req_len
(
    rrc_mac_ue_drx_cmd_req_t *p_rrc_mac_ue_drx_cmd_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ue_drx_cmd_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ue_drx_cmd_req->ue_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_ue_drx_cmd_req
(
    U8  **pp_buffer,
    rrc_mac_ue_drx_cmd_req_t *p_rrc_mac_ue_drx_cmd_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ue_drx_cmd_req != PNULL);

    /* This function composes rrc_mac_ue_drx_cmd_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ue_drx_cmd_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_ue_drx_cmd_req->ue_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_ue_drx_cmd_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_UE_DRX_CMD_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_ue_drx_cmd_req
(
    rrc_mac_ue_drx_cmd_req_t  *p_rrc_mac_ue_drx_cmd_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_ue_drx_cmd_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_ue_drx_cmd_req_len(p_rrc_mac_ue_drx_cmd_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_UE_DRX_CMD_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_UE_DRX_CMD_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_UE_DRX_CMD_REQ, msg_length,cell_index);

    /* Fill RRC_MAC_UE_DRX_CMD_REQ message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_ue_drx_cmd_req(&p_mac_msg, p_rrc_mac_ue_drx_cmd_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_ue_con_rej_req_len
(
    rrc_mac_ue_con_rej_req_t *p_rrc_mac_ue_con_rej_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ue_con_rej_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ue_con_rej_req->tcrnti);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_ue_con_rej_req
(
    U8  **pp_buffer,
    rrc_mac_ue_con_rej_req_t *p_rrc_mac_ue_con_rej_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ue_con_rej_req != PNULL);

    /* This function composes rrc_mac_ue_con_rej_req */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ue_con_rej_req->tcrnti < 1) || (p_rrc_mac_ue_con_rej_req->tcrnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_con_rej_req->tcrnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_ue_con_rej_req->tcrnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ue_con_rej_req->tcrnti, "tcrnti");
    *pp_buffer += sizeof(p_rrc_mac_ue_con_rej_req->tcrnti);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_ue_con_rej_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_UE_CON_REJ_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_ue_con_rej_req
(
    rrc_mac_ue_con_rej_req_t  *p_rrc_mac_ue_con_rej_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_ue_con_rej_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_ue_con_rej_req_len(p_rrc_mac_ue_con_rej_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_UE_CON_REJ_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_UE_CON_REJ_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_UE_CON_REJ_REQ, msg_length,cell_index);

    /* Fill RRC_MAC_UE_CON_REJ_REQ message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_ue_con_rej_req(&p_mac_msg, p_rrc_mac_ue_con_rej_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_bcch_config_req_len
(
    rrc_mac_bcch_config_req_t *p_rrc_mac_bcch_config_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_bcch_config_req != PNULL);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_bcch_config_req->mib_msg_req_counter <= ARRSIZE(p_rrc_mac_bcch_config_req->mib_msg_req));
        for (loop = 0; loop < p_rrc_mac_bcch_config_req->mib_msg_req_counter; loop++)
        {
            length += rrc_il_get_rrc_mac_mib_msg_req_tlv_len(&p_rrc_mac_bcch_config_req->mib_msg_req[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_mac_bcch_config_req->optional_elems_present & RRC_MAC_BCCH_SIBTYPE1_MSG_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sibtype1_msg_req_tlv_len(&p_rrc_mac_bcch_config_req->sib1_msg_req);
    }

    /* Get length of TLV */
    if (p_rrc_mac_bcch_config_req->optional_elems_present & RRC_MAC_BCCH_SI_MSG_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_si_msg_req_tlv_len(&p_rrc_mac_bcch_config_req->si_msg_req);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_bcch_config_req
(
    U8  **pp_buffer,
    rrc_mac_bcch_config_req_t *p_rrc_mac_bcch_config_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_bcch_config_req != PNULL);

    /* This function composes rrc_mac_bcch_config_req */
    
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_bcch_config_req->mib_msg_req_counter <= ARRSIZE(p_rrc_mac_bcch_config_req->mib_msg_req));
        for (loop = 0; loop < p_rrc_mac_bcch_config_req->mib_msg_req_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_mac_mib_msg_req(pp_buffer, &p_rrc_mac_bcch_config_req->mib_msg_req[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_bcch_config_req->optional_elems_present & RRC_MAC_BCCH_SIBTYPE1_MSG_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sibtype1_msg_req(pp_buffer, &p_rrc_mac_bcch_config_req->sib1_msg_req))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_bcch_config_req->optional_elems_present & RRC_MAC_BCCH_SI_MSG_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_si_msg_req(pp_buffer, &p_rrc_mac_bcch_config_req->si_msg_req))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_bcch_config_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_BCCH_CONFIG_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_bcch_config_req
(
    rrc_mac_bcch_config_req_t  *p_rrc_mac_bcch_config_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_bcch_config_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_bcch_config_req_len(p_rrc_mac_bcch_config_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_BCCH_CONFIG_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_BCCH_CONFIG_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_BCCH_CONFIG_REQ, msg_length,cell_index);

    /* Fill RRC_MAC_BCCH_CONFIG_REQ message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_bcch_config_req(&p_mac_msg, p_rrc_mac_bcch_config_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_pcch_msg_req_len
(
    rrc_mac_pcch_msg_req_t *p_rrc_mac_pcch_msg_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_pcch_msg_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pcch_msg_req->paging_frame);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pcch_msg_req->paging_sub_frame);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pcch_msg_req->num_of_sfn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_pcch_msg_req->isPagingForEmtc);

    /* Get length of TLV */
    if (p_rrc_mac_pcch_msg_req->bitmask & RRC_MAC_PAGING_NARROWBAND_PRESENT)
    {
        length += rrc_il_get_RrcMacPagingNarrowband_tlv_len(&p_rrc_mac_pcch_msg_req->pagingNarrowBand);
    }

    /* Get length of TLV */
    if (p_rrc_mac_pcch_msg_req->bitmask & RRC_MAC_MPDCCH_NUM_REPETITION_PAGING_PRESENT)
    {
        length += rrc_il_get_RrcMacMpdcchNumRepetitionPaging_tlv_len(&p_rrc_mac_pcch_msg_req->mpdcchNumRepetitionPaging);
    }

    /* Get length of TLV */
    if (p_rrc_mac_pcch_msg_req->bitmask & RRC_MAC_PDSCH_NUM_REPETITION_PRESENT)
    {
        length += rrc_il_get_RrcMacPdschNumRepetition_tlv_len(&p_rrc_mac_pcch_msg_req->pdschNumRepetition);
    }

    /* Get length of TLV */
    if (p_rrc_mac_pcch_msg_req->bitmask & RRC_MAC_DIRECT_INDICATION_PRESENT)
    {
        length += rrc_il_get_RrcMacDirectIndication_tlv_len(&p_rrc_mac_pcch_msg_req->directIndication);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_pcch_msg_req
(
    U8  **pp_buffer,
    rrc_mac_pcch_msg_req_t *p_rrc_mac_pcch_msg_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_pcch_msg_req != PNULL);

    /* This function composes rrc_mac_pcch_msg_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pcch_msg_req->paging_frame > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pcch_msg_req->paging_frame] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_rrc_mac_pcch_msg_req->paging_frame);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_pcch_msg_req->paging_frame, "paging_frame");
    *pp_buffer += sizeof(p_rrc_mac_pcch_msg_req->paging_frame);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pcch_msg_req->paging_sub_frame > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pcch_msg_req->paging_sub_frame] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_mac_pcch_msg_req->paging_sub_frame);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pcch_msg_req->paging_sub_frame, "paging_sub_frame");
    *pp_buffer += sizeof(p_rrc_mac_pcch_msg_req->paging_sub_frame);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_pcch_msg_req->num_of_sfn < 1) || (p_rrc_mac_pcch_msg_req->num_of_sfn > 32768))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pcch_msg_req->num_of_sfn] should be in range "
            "1 to 32768. Incorrect value %u received.", p_rrc_mac_pcch_msg_req->num_of_sfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_pcch_msg_req->num_of_sfn, "num_of_sfn");
    *pp_buffer += sizeof(p_rrc_mac_pcch_msg_req->num_of_sfn);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_pcch_msg_req->isPagingForEmtc > EMTC_PAGING))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_pcch_msg_req->isPagingForEmtc] should be less than"
            " or equal to EMTC_PAGING. Incorrect value %u received.", p_rrc_mac_pcch_msg_req->isPagingForEmtc);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_pcch_msg_req->isPagingForEmtc, "isPagingForEmtc");
    *pp_buffer += sizeof(p_rrc_mac_pcch_msg_req->isPagingForEmtc);

    /* Compose TLV  */
    if (p_rrc_mac_pcch_msg_req->bitmask & RRC_MAC_PAGING_NARROWBAND_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacPagingNarrowband(pp_buffer, &p_rrc_mac_pcch_msg_req->pagingNarrowBand))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_pcch_msg_req->bitmask & RRC_MAC_MPDCCH_NUM_REPETITION_PAGING_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacMpdcchNumRepetitionPaging(pp_buffer, &p_rrc_mac_pcch_msg_req->mpdcchNumRepetitionPaging))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_pcch_msg_req->bitmask & RRC_MAC_PDSCH_NUM_REPETITION_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacPdschNumRepetition(pp_buffer, &p_rrc_mac_pcch_msg_req->pdschNumRepetition))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_pcch_msg_req->bitmask & RRC_MAC_DIRECT_INDICATION_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_RrcMacDirectIndication(pp_buffer, &p_rrc_mac_pcch_msg_req->directIndication))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacPagingNarrowband_len
(
    RrcMacPagingNarrowband_t *p_RrcMacPagingNarrowband
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPagingNarrowband != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPagingNarrowband->pagingNarrowBand);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacPagingNarrowband_tlv_len
(
    RrcMacPagingNarrowband_t *p_RrcMacPagingNarrowband

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPagingNarrowband != PNULL);

    length += rrc_il_get_RrcMacPagingNarrowband_len(p_RrcMacPagingNarrowband);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacPagingNarrowband
(
    U8  **pp_buffer,
    RrcMacPagingNarrowband_t *p_RrcMacPagingNarrowband
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacPagingNarrowband != PNULL);

    /* This function composes RrcMacPagingNarrowband */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PAGING_NARROWBAND;
    tlv_length = rrc_il_get_RrcMacPagingNarrowband_tlv_len(p_RrcMacPagingNarrowband);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacPagingNarrowband->pagingNarrowBand < 1) || (p_RrcMacPagingNarrowband->pagingNarrowBand > EMTC_MAX_AVAILABLE_NARROWBAND))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPagingNarrowband->pagingNarrowBand] should be in range "
            "1 to EMTC_MAX_AVAILABLE_NARROWBAND. Incorrect value %u received.", p_RrcMacPagingNarrowband->pagingNarrowBand);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPagingNarrowband->pagingNarrowBand, "pagingNarrowBand");
    *pp_buffer += sizeof(p_RrcMacPagingNarrowband->pagingNarrowBand);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacMpdcchNumRepetitionPaging_len
(
    RrcMacMpdcchNumRepetitionPaging_t *p_RrcMacMpdcchNumRepetitionPaging
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacMpdcchNumRepetitionPaging != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacMpdcchNumRepetitionPaging->mpdcchNumRepetitionPaging);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacMpdcchNumRepetitionPaging_tlv_len
(
    RrcMacMpdcchNumRepetitionPaging_t *p_RrcMacMpdcchNumRepetitionPaging

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacMpdcchNumRepetitionPaging != PNULL);

    length += rrc_il_get_RrcMacMpdcchNumRepetitionPaging_len(p_RrcMacMpdcchNumRepetitionPaging);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacMpdcchNumRepetitionPaging
(
    U8  **pp_buffer,
    RrcMacMpdcchNumRepetitionPaging_t *p_RrcMacMpdcchNumRepetitionPaging
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacMpdcchNumRepetitionPaging != PNULL);

    /* This function composes RrcMacMpdcchNumRepetitionPaging */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_MPDCCH_NUM_REPETITION_PAGING;
    tlv_length = rrc_il_get_RrcMacMpdcchNumRepetitionPaging_tlv_len(p_RrcMacMpdcchNumRepetitionPaging);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacMpdcchNumRepetitionPaging->mpdcchNumRepetitionPaging < 1) || (p_RrcMacMpdcchNumRepetitionPaging->mpdcchNumRepetitionPaging > EMTC_MPDCCH_NUM_REPETITION_PAGING_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacMpdcchNumRepetitionPaging->mpdcchNumRepetitionPaging] should be in range "
            "1 to EMTC_MPDCCH_NUM_REPETITION_PAGING_MAX. Incorrect value %u received.", p_RrcMacMpdcchNumRepetitionPaging->mpdcchNumRepetitionPaging);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacMpdcchNumRepetitionPaging->mpdcchNumRepetitionPaging, "mpdcchNumRepetitionPaging");
    *pp_buffer += sizeof(p_RrcMacMpdcchNumRepetitionPaging->mpdcchNumRepetitionPaging);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacPdschNumRepetition_len
(
    RrcMacPdschNumRepetition_t *p_RrcMacPdschNumRepetition
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPdschNumRepetition != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacPdschNumRepetition->pdschNumRepetition);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacPdschNumRepetition_tlv_len
(
    RrcMacPdschNumRepetition_t *p_RrcMacPdschNumRepetition

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacPdschNumRepetition != PNULL);

    length += rrc_il_get_RrcMacPdschNumRepetition_len(p_RrcMacPdschNumRepetition);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacPdschNumRepetition
(
    U8  **pp_buffer,
    RrcMacPdschNumRepetition_t *p_RrcMacPdschNumRepetition
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacPdschNumRepetition != PNULL);

    /* This function composes RrcMacPdschNumRepetition */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PDSCH_NUM_REPETITION_PAGING;
    tlv_length = rrc_il_get_RrcMacPdschNumRepetition_tlv_len(p_RrcMacPdschNumRepetition);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacPdschNumRepetition->pdschNumRepetition < 1) || (p_RrcMacPdschNumRepetition->pdschNumRepetition > EMTC_PDSCH_PAGING_REP_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacPdschNumRepetition->pdschNumRepetition] should be in range "
            "1 to EMTC_PDSCH_PAGING_REP_MAX. Incorrect value %u received.", p_RrcMacPdschNumRepetition->pdschNumRepetition);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacPdschNumRepetition->pdschNumRepetition, "pdschNumRepetition");
    *pp_buffer += sizeof(p_RrcMacPdschNumRepetition->pdschNumRepetition);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_RrcMacDirectIndication_len
(
    RrcMacDirectIndication_t *p_RrcMacDirectIndication
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacDirectIndication != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacDirectIndication->directIndication);

    return length;
}

static
rrc_length_t
rrc_il_get_RrcMacDirectIndication_tlv_len
(
    RrcMacDirectIndication_t *p_RrcMacDirectIndication

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacDirectIndication != PNULL);

    length += rrc_il_get_RrcMacDirectIndication_len(p_RrcMacDirectIndication);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_RrcMacDirectIndication
(
    U8  **pp_buffer,
    RrcMacDirectIndication_t *p_RrcMacDirectIndication
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacDirectIndication != PNULL);

    /* This function composes RrcMacDirectIndication */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_DIRECT_INDICATION;
    tlv_length = rrc_il_get_RrcMacDirectIndication_tlv_len(p_RrcMacDirectIndication);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacDirectIndication->directIndication > EMTC_DIRECT_INDICATION_SI_MODIF))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacDirectIndication->directIndication] should be less than"
            " or equal to EMTC_DIRECT_INDICATION_SI_MODIF. Incorrect value %u received.", p_RrcMacDirectIndication->directIndication);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacDirectIndication->directIndication, "directIndication");
    *pp_buffer += sizeof(p_RrcMacDirectIndication->directIndication);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_ccch_msg_req_len
(
    rrc_mac_ccch_msg_req_t *p_rrc_mac_ccch_msg_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ccch_msg_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ccch_msg_req->rnti);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_ccch_msg_req
(
    U8  **pp_buffer,
    rrc_mac_ccch_msg_req_t *p_rrc_mac_ccch_msg_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ccch_msg_req != PNULL);

    /* This function composes rrc_mac_ccch_msg_req */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ccch_msg_req->rnti < 1) || (p_rrc_mac_ccch_msg_req->rnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ccch_msg_req->rnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_ccch_msg_req->rnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ccch_msg_req->rnti, "rnti");
    *pp_buffer += sizeof(p_rrc_mac_ccch_msg_req->rnti);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_ccch_msg_ind_len
(
    rrc_mac_ccch_msg_ind_t *p_rrc_mac_ccch_msg_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ccch_msg_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ccch_msg_ind->rnti);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ccch_msg_ind->timing_advance);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ccch_msg_ind->sinr);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_ccch_msg_ind
(
    U8  **pp_buffer,
    rrc_mac_ccch_msg_ind_t *p_rrc_mac_ccch_msg_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ccch_msg_ind != PNULL);

    /* This function composes rrc_mac_ccch_msg_ind */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ccch_msg_ind->rnti < 1) || (p_rrc_mac_ccch_msg_ind->rnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ccch_msg_ind->rnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_ccch_msg_ind->rnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ccch_msg_ind->rnti, "rnti");
    *pp_buffer += sizeof(p_rrc_mac_ccch_msg_ind->rnti);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ccch_msg_ind->timing_advance > 1282))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ccch_msg_ind->timing_advance] should be less than"
            " or equal to 1282. Incorrect value %u received.", p_rrc_mac_ccch_msg_ind->timing_advance);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ccch_msg_ind->timing_advance, "timing_advance");
    *pp_buffer += sizeof(p_rrc_mac_ccch_msg_ind->timing_advance);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_ccch_msg_ind->sinr, "sinr");
    *pp_buffer += sizeof(p_rrc_mac_ccch_msg_ind->sinr);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_RrcMacEmtcCcchMsgInd_len
(
    RrcMacEmtcCcchMsgInd_t *p_RrcMacEmtcCcchMsgInd
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_RrcMacEmtcCcchMsgInd != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacEmtcCcchMsgInd->rnti);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacEmtcCcchMsgInd->timing_advance);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacEmtcCcchMsgInd->sinr);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_RrcMacEmtcCcchMsgInd->ceLevel);

    return length;
}

rrc_return_et
rrc_il_compose_RrcMacEmtcCcchMsgInd
(
    U8  **pp_buffer,
    RrcMacEmtcCcchMsgInd_t *p_RrcMacEmtcCcchMsgInd
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_RrcMacEmtcCcchMsgInd != PNULL);

    /* This function composes RrcMacEmtcCcchMsgInd */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_RrcMacEmtcCcchMsgInd->rnti < 1) || (p_RrcMacEmtcCcchMsgInd->rnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacEmtcCcchMsgInd->rnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_RrcMacEmtcCcchMsgInd->rnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_RrcMacEmtcCcchMsgInd->rnti, "rnti");
    *pp_buffer += sizeof(p_RrcMacEmtcCcchMsgInd->rnti);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacEmtcCcchMsgInd->timing_advance > 1282))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacEmtcCcchMsgInd->timing_advance] should be less than"
            " or equal to 1282. Incorrect value %u received.", p_RrcMacEmtcCcchMsgInd->timing_advance);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_RrcMacEmtcCcchMsgInd->timing_advance, "timing_advance");
    *pp_buffer += sizeof(p_RrcMacEmtcCcchMsgInd->timing_advance);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacEmtcCcchMsgInd->sinr, "sinr");
    *pp_buffer += sizeof(p_RrcMacEmtcCcchMsgInd->sinr);

    /* Check for correct range [H - higher boundary] */
    if ((p_RrcMacEmtcCcchMsgInd->ceLevel > EMTC_MAX_CE_LEVEL))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_RrcMacEmtcCcchMsgInd->ceLevel] should be less than"
            " or equal to EMTC_MAX_CE_LEVEL. Incorrect value %u received.", p_RrcMacEmtcCcchMsgInd->ceLevel);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_RrcMacEmtcCcchMsgInd->ceLevel, "ceLevel");
    *pp_buffer += sizeof(p_RrcMacEmtcCcchMsgInd->ceLevel);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_ho_rach_resource_req_len
(
    rrc_mac_ho_rach_resource_req_t *p_rrc_mac_ho_rach_resource_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ho_rach_resource_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ho_rach_resource_req->ue_index);

    /* Get length of TLV */
    if (p_rrc_mac_ho_rach_resource_req->optional_elems_present & RRC_MAC_INTRA_ENB_HO_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_intra_enb_ho_req_tlv_len(&p_rrc_mac_ho_rach_resource_req->intra_enb_ho_req);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_ho_rach_resource_req
(
    U8  **pp_buffer,
    rrc_mac_ho_rach_resource_req_t *p_rrc_mac_ho_rach_resource_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ho_rach_resource_req != PNULL);

    /* This function composes rrc_mac_ho_rach_resource_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ho_rach_resource_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_ho_rach_resource_req->ue_index);

    /* Compose TLV  */
    if (p_rrc_mac_ho_rach_resource_req->optional_elems_present & RRC_MAC_INTRA_ENB_HO_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_intra_enb_ho_req(pp_buffer, &p_rrc_mac_ho_rach_resource_req->intra_enb_ho_req))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_ho_rach_resource_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_HO_RACH_RESOURCE_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_ho_rach_resource_req
(
    rrc_mac_ho_rach_resource_req_t  *p_rrc_mac_ho_rach_resource_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_ho_rach_resource_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_ho_rach_resource_req_len(p_rrc_mac_ho_rach_resource_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_HO_RACH_RESOURCE_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_HO_RACH_RESOURCE_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_HO_RACH_RESOURCE_REQ, msg_length,cell_index);

    /* Fill RRC_MAC_HO_RACH_RESOURCE_REQ message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_ho_rach_resource_req(&p_mac_msg, p_rrc_mac_ho_rach_resource_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_intra_enb_ho_req_len
(
    rrc_mac_intra_enb_ho_req_t *p_rrc_mac_intra_enb_ho_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_intra_enb_ho_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_intra_enb_ho_req->enable_intra_enb_ho);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_intra_enb_ho_req_tlv_len
(
    rrc_mac_intra_enb_ho_req_t *p_rrc_mac_intra_enb_ho_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_intra_enb_ho_req != PNULL);

    length += rrc_il_get_rrc_mac_intra_enb_ho_req_len(p_rrc_mac_intra_enb_ho_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_intra_enb_ho_req
(
    U8  **pp_buffer,
    rrc_mac_intra_enb_ho_req_t *p_rrc_mac_intra_enb_ho_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_intra_enb_ho_req != PNULL);

    /* This function composes rrc_mac_intra_enb_ho_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_INTRA_ENB_HO_REQ;
    tlv_length = rrc_il_get_rrc_mac_intra_enb_ho_req_tlv_len(p_rrc_mac_intra_enb_ho_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_intra_enb_ho_req->enable_intra_enb_ho > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_intra_enb_ho_req->enable_intra_enb_ho] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_intra_enb_ho_req->enable_intra_enb_ho);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_intra_enb_ho_req->enable_intra_enb_ho, "enable_intra_enb_ho");
    *pp_buffer += sizeof(p_rrc_mac_intra_enb_ho_req->enable_intra_enb_ho);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_ho_rach_resource_resp_len
(
    rrc_mac_ho_rach_resource_resp_t *p_rrc_mac_ho_rach_resource_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ho_rach_resource_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ho_rach_resource_resp->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ho_rach_resource_resp->response);

    /* Get length of TLV */
    if (p_rrc_mac_ho_rach_resource_resp->optional_elems_present & RRC_MAC_CRNTI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_crnti_info_tlv_len(&p_rrc_mac_ho_rach_resource_resp->crnti_info);
    }

    /* Get length of TLV */
    if (p_rrc_mac_ho_rach_resource_resp->optional_elems_present & RRC_MAC_PREAMBLE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_preamble_info_tlv_len(&p_rrc_mac_ho_rach_resource_resp->preamble_info);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_ho_rach_resource_resp
(
    U8  **pp_buffer,
    rrc_mac_ho_rach_resource_resp_t *p_rrc_mac_ho_rach_resource_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ho_rach_resource_resp != PNULL);

    /* This function composes rrc_mac_ho_rach_resource_resp */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ho_rach_resource_resp->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_ho_rach_resource_resp->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ho_rach_resource_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ho_rach_resource_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_ho_rach_resource_resp->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ho_rach_resource_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_mac_ho_rach_resource_resp->response);

    /* Compose TLV  */
    if (p_rrc_mac_ho_rach_resource_resp->optional_elems_present & RRC_MAC_CRNTI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_crnti_info(pp_buffer, &p_rrc_mac_ho_rach_resource_resp->crnti_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_mac_ho_rach_resource_resp->optional_elems_present & RRC_MAC_PREAMBLE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_preamble_info(pp_buffer, &p_rrc_mac_ho_rach_resource_resp->preamble_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_ho_rach_resource_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_HO_RACH_RESOURCE_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_ho_rach_resource_resp
(
    rrc_mac_ho_rach_resource_resp_t  *p_rrc_mac_ho_rach_resource_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_ho_rach_resource_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_ho_rach_resource_resp_len(p_rrc_mac_ho_rach_resource_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_HO_RACH_RESOURCE_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_HO_RACH_RESOURCE_RESP, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_HO_RACH_RESOURCE_RESP, msg_length,cell_index);

    /* Fill RRC_MAC_HO_RACH_RESOURCE_RESP message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_ho_rach_resource_resp(&p_mac_msg, p_rrc_mac_ho_rach_resource_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_crnti_info_len
(
    rrc_mac_crnti_info_t *p_rrc_mac_crnti_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_crnti_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_crnti_info->crnti);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_crnti_info_tlv_len
(
    rrc_mac_crnti_info_t *p_rrc_mac_crnti_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_crnti_info != PNULL);

    length += rrc_il_get_rrc_mac_crnti_info_len(p_rrc_mac_crnti_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_crnti_info
(
    U8  **pp_buffer,
    rrc_mac_crnti_info_t *p_rrc_mac_crnti_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_crnti_info != PNULL);

    /* This function composes rrc_mac_crnti_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_CRNTI_INFO;
    tlv_length = rrc_il_get_rrc_mac_crnti_info_tlv_len(p_rrc_mac_crnti_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_crnti_info->crnti < 1) || (p_rrc_mac_crnti_info->crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_crnti_info->crnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_crnti_info->crnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_crnti_info->crnti, "crnti");
    *pp_buffer += sizeof(p_rrc_mac_crnti_info->crnti);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_preamble_info_len
(
    rrc_mac_preamble_info_t *p_rrc_mac_preamble_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_preamble_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_preamble_info->preamble_Idx);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_preamble_info->prach_mask_Idx);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_preamble_info_tlv_len
(
    rrc_mac_preamble_info_t *p_rrc_mac_preamble_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_preamble_info != PNULL);

    length += rrc_il_get_rrc_mac_preamble_info_len(p_rrc_mac_preamble_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_preamble_info
(
    U8  **pp_buffer,
    rrc_mac_preamble_info_t *p_rrc_mac_preamble_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_preamble_info != PNULL);

    /* This function composes rrc_mac_preamble_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_PREAMBLE_INFO;
    tlv_length = rrc_il_get_rrc_mac_preamble_info_tlv_len(p_rrc_mac_preamble_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_preamble_info->preamble_Idx > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_preamble_info->preamble_Idx] should be less than"
            " or equal to 63. Incorrect value %u received.", p_rrc_mac_preamble_info->preamble_Idx);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_preamble_info->preamble_Idx, "preamble_Idx");
    *pp_buffer += sizeof(p_rrc_mac_preamble_info->preamble_Idx);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_preamble_info->prach_mask_Idx > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_preamble_info->prach_mask_Idx] should be less than"
            " or equal to 15. Incorrect value %u received.", p_rrc_mac_preamble_info->prach_mask_Idx);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_preamble_info->prach_mask_Idx, "prach_mask_Idx");
    *pp_buffer += sizeof(p_rrc_mac_preamble_info->prach_mask_Idx);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_ho_rel_rach_resource_ind_len
(
    rrc_mac_ho_rel_rach_resource_ind_t *p_rrc_mac_ho_rel_rach_resource_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ho_rel_rach_resource_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ho_rel_rach_resource_ind->ue_index);

    /* Get length of TLV */
    if (p_rrc_mac_ho_rel_rach_resource_ind->optional_elems_present & RRC_MAC_CRNTI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_crnti_info_tlv_len(&p_rrc_mac_ho_rel_rach_resource_ind->crnti_info);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_ho_rel_rach_resource_ind
(
    U8  **pp_buffer,
    rrc_mac_ho_rel_rach_resource_ind_t *p_rrc_mac_ho_rel_rach_resource_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ho_rel_rach_resource_ind != PNULL);

    /* This function composes rrc_mac_ho_rel_rach_resource_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ho_rel_rach_resource_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_ho_rel_rach_resource_ind->ue_index);

    /* Compose TLV  */
    if (p_rrc_mac_ho_rel_rach_resource_ind->optional_elems_present & RRC_MAC_CRNTI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_crnti_info(pp_buffer, &p_rrc_mac_ho_rel_rach_resource_ind->crnti_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_ho_rel_rach_resource_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_HO_REL_RACH_RESOURCE_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_ho_rel_rach_resource_ind
(
    rrc_mac_ho_rel_rach_resource_ind_t  *p_rrc_mac_ho_rel_rach_resource_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_ho_rel_rach_resource_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_ho_rel_rach_resource_ind_len(p_rrc_mac_ho_rel_rach_resource_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_HO_REL_RACH_RESOURCE_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_HO_REL_RACH_RESOURCE_IND, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_HO_REL_RACH_RESOURCE_IND, msg_length,cell_index);

    /* Fill RRC_MAC_HO_REL_RACH_RESOURCE_IND message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_ho_rel_rach_resource_ind(&p_mac_msg, p_rrc_mac_ho_rel_rach_resource_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_reset_ue_entity_req_len
(
    rrc_mac_reset_ue_entity_req_t *p_rrc_mac_reset_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reset_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reset_ue_entity_req->ue_index);

    /* Get length of TLV */
    if (p_rrc_mac_reset_ue_entity_req->optional_elems_present & RRC_MAC_RESET_UE_ENTITY_SCELL_REL_FLAG_PRESENT)
    {
        length += rrc_il_get_rrc_mac_scell_rel_flag_tlv_len(&p_rrc_mac_reset_ue_entity_req->rrc_mac_scell_rel_flag);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_reset_ue_entity_req
(
    U8  **pp_buffer,
    rrc_mac_reset_ue_entity_req_t *p_rrc_mac_reset_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_reset_ue_entity_req != PNULL);

    /* This function composes rrc_mac_reset_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_reset_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_reset_ue_entity_req->ue_index);

    /* Compose TLV  */
    if (p_rrc_mac_reset_ue_entity_req->optional_elems_present & RRC_MAC_RESET_UE_ENTITY_SCELL_REL_FLAG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_scell_rel_flag(pp_buffer, &p_rrc_mac_reset_ue_entity_req->rrc_mac_scell_rel_flag))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_reset_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_RESET_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_reset_ue_entity_req
(
    rrc_mac_reset_ue_entity_req_t  *p_rrc_mac_reset_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_reset_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_reset_ue_entity_req_len(p_rrc_mac_reset_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_RESET_UE_ENTITY_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_RESET_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_RESET_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_MAC_RESET_UE_ENTITY_REQ message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_reset_ue_entity_req(&p_mac_msg, p_rrc_mac_reset_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_scell_rel_flag_len
(
    rrc_mac_scell_rel_flag_t *p_rrc_mac_scell_rel_flag
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_scell_rel_flag != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_scell_rel_flag->scell_rel_flag);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_scell_rel_flag_tlv_len
(
    rrc_mac_scell_rel_flag_t *p_rrc_mac_scell_rel_flag

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_scell_rel_flag != PNULL);

    length += rrc_il_get_rrc_mac_scell_rel_flag_len(p_rrc_mac_scell_rel_flag);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_scell_rel_flag
(
    U8  **pp_buffer,
    rrc_mac_scell_rel_flag_t *p_rrc_mac_scell_rel_flag
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_scell_rel_flag != PNULL);

    /* This function composes rrc_mac_scell_rel_flag */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_MAC_RESET_UE_ENTITY_SCELL_REL_FLAG;
    tlv_length = rrc_il_get_rrc_mac_scell_rel_flag_tlv_len(p_rrc_mac_scell_rel_flag);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_scell_rel_flag->scell_rel_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_scell_rel_flag->scell_rel_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_scell_rel_flag->scell_rel_flag);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_scell_rel_flag->scell_rel_flag, "scell_rel_flag");
    *pp_buffer += sizeof(p_rrc_mac_scell_rel_flag->scell_rel_flag);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_reset_ue_entity_cnf_len
(
    rrc_mac_reset_ue_entity_cnf_t *p_rrc_mac_reset_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reset_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reset_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reset_ue_entity_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_reset_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_mac_reset_ue_entity_cnf_t *p_rrc_mac_reset_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_reset_ue_entity_cnf != PNULL);

    /* This function composes rrc_mac_reset_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_reset_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_reset_ue_entity_cnf->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_reset_ue_entity_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_mac_reset_ue_entity_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_reset_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_RESET_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_reset_ue_entity_cnf
(
    rrc_mac_reset_ue_entity_cnf_t  *p_rrc_mac_reset_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_reset_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_reset_ue_entity_cnf_len(p_rrc_mac_reset_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_RESET_UE_ENTITY_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_RESET_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_RESET_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_MAC_RESET_UE_ENTITY_CNF message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_reset_ue_entity_cnf(&p_mac_msg, p_rrc_mac_reset_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_radio_link_failure_ind_len
(
    rrc_mac_radio_link_failure_ind_t *p_rrc_mac_radio_link_failure_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_radio_link_failure_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_radio_link_failure_ind->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_radio_link_failure_ind->rlf_cause);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_radio_link_failure_ind
(
    U8  **pp_buffer,
    rrc_mac_radio_link_failure_ind_t *p_rrc_mac_radio_link_failure_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_radio_link_failure_ind != PNULL);

    /* This function composes rrc_mac_radio_link_failure_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_radio_link_failure_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_radio_link_failure_ind->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_radio_link_failure_ind->rlf_cause, "rlf_cause");
    *pp_buffer += sizeof(p_rrc_mac_radio_link_failure_ind->rlf_cause);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_radio_link_failure_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_RADIO_LINK_FAILURE_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_radio_link_failure_ind
(
    rrc_mac_radio_link_failure_ind_t  *p_rrc_mac_radio_link_failure_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_radio_link_failure_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_radio_link_failure_ind_len(p_rrc_mac_radio_link_failure_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_RADIO_LINK_FAILURE_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_RADIO_LINK_FAILURE_IND, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_RADIO_LINK_FAILURE_IND, msg_length,cell_index);

    /* Fill RRC_MAC_RADIO_LINK_FAILURE_IND message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_radio_link_failure_ind(&p_mac_msg, p_rrc_mac_radio_link_failure_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_ue_sync_status_ind_len
(
    rrc_mac_ue_sync_status_ind_t *p_rrc_mac_ue_sync_status_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_ue_sync_status_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ue_sync_status_ind->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ue_sync_status_ind->crnti);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_ue_sync_status_ind->ue_sync_status);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_ue_sync_status_ind
(
    U8  **pp_buffer,
    rrc_mac_ue_sync_status_ind_t *p_rrc_mac_ue_sync_status_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_ue_sync_status_ind != PNULL);

    /* This function composes rrc_mac_ue_sync_status_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ue_sync_status_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_ue_sync_status_ind->ue_index);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_mac_ue_sync_status_ind->crnti < 1) || (p_rrc_mac_ue_sync_status_ind->crnti > 65523))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_sync_status_ind->crnti] should be in range "
            "1 to 65523. Incorrect value %u received.", p_rrc_mac_ue_sync_status_ind->crnti);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_ue_sync_status_ind->crnti, "crnti");
    *pp_buffer += sizeof(p_rrc_mac_ue_sync_status_ind->crnti);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_ue_sync_status_ind->ue_sync_status > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_ue_sync_status_ind->ue_sync_status] should be less than"
            " or equal to 3. Incorrect value %u received.", p_rrc_mac_ue_sync_status_ind->ue_sync_status);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_ue_sync_status_ind->ue_sync_status, "ue_sync_status");
    *pp_buffer += sizeof(p_rrc_mac_ue_sync_status_ind->ue_sync_status);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_ue_sync_status_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_UE_SYNC_STATUS_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_ue_sync_status_ind
(
    rrc_mac_ue_sync_status_ind_t  *p_rrc_mac_ue_sync_status_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_ue_sync_status_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_ue_sync_status_ind_len(p_rrc_mac_ue_sync_status_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_UE_SYNC_STATUS_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_UE_SYNC_STATUS_IND, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_UE_SYNC_STATUS_IND, msg_length,cell_index);

    /* Fill RRC_MAC_UE_SYNC_STATUS_IND message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_ue_sync_status_ind(&p_mac_msg, p_rrc_mac_ue_sync_status_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_change_crnti_req_len
(
    rrc_mac_change_crnti_req_t *p_rrc_mac_change_crnti_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_change_crnti_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_change_crnti_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_change_crnti_req->old_crnti);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_change_crnti_req->new_crnti);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_change_crnti_req
(
    U8  **pp_buffer,
    rrc_mac_change_crnti_req_t *p_rrc_mac_change_crnti_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_change_crnti_req != PNULL);

    /* This function composes rrc_mac_change_crnti_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_change_crnti_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_change_crnti_req->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_change_crnti_req->old_crnti, "old_crnti");
    *pp_buffer += sizeof(p_rrc_mac_change_crnti_req->old_crnti);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_change_crnti_req->new_crnti, "new_crnti");
    *pp_buffer += sizeof(p_rrc_mac_change_crnti_req->new_crnti);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_change_crnti_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_CHANGE_CRNTI_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_change_crnti_req
(
    rrc_mac_change_crnti_req_t  *p_rrc_mac_change_crnti_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_change_crnti_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_change_crnti_req_len(p_rrc_mac_change_crnti_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_CHANGE_CRNTI_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_CHANGE_CRNTI_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_CHANGE_CRNTI_REQ, msg_length,cell_index);

    /* Fill RRC_MAC_CHANGE_CRNTI_REQ message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_change_crnti_req(&p_mac_msg, p_rrc_mac_change_crnti_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_change_crnti_cnf_len
(
    rrc_mac_change_crnti_cnf_t *p_rrc_mac_change_crnti_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_change_crnti_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_change_crnti_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_change_crnti_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_change_crnti_cnf
(
    U8  **pp_buffer,
    rrc_mac_change_crnti_cnf_t *p_rrc_mac_change_crnti_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_change_crnti_cnf != PNULL);

    /* This function composes rrc_mac_change_crnti_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_change_crnti_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_change_crnti_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_change_crnti_cnf->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_change_crnti_cnf->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_mac_change_crnti_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_change_crnti_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_mac_change_crnti_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_change_crnti_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_CHANGE_CRNTI_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_change_crnti_cnf
(
    rrc_mac_change_crnti_cnf_t  *p_rrc_mac_change_crnti_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_change_crnti_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_change_crnti_cnf_len(p_rrc_mac_change_crnti_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_CHANGE_CRNTI_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_CHANGE_CRNTI_CNF, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_CHANGE_CRNTI_CNF, msg_length,cell_index);

    /* Fill RRC_MAC_CHANGE_CRNTI_CNF message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_change_crnti_cnf(&p_mac_msg, p_rrc_mac_change_crnti_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_cell_start_cnf_len
(
    rrc_mac_cell_start_cnf_t *p_rrc_mac_cell_start_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_cell_start_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_cell_start_cnf->response);

    /* Get length of TLV */
    if (p_rrc_mac_cell_start_cnf->optional_elems_present & RRC_MAC_CELL_START_CNF_SFN_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_mac_sfn_tlv_len(&p_rrc_mac_cell_start_cnf->sfn);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_cell_start_cnf
(
    U8  **pp_buffer,
    rrc_mac_cell_start_cnf_t *p_rrc_mac_cell_start_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_cell_start_cnf != PNULL);

    /* This function composes rrc_mac_cell_start_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_cell_start_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_cell_start_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_cell_start_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_cell_start_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_mac_cell_start_cnf->response);

    /* Compose TLV  */
    if (p_rrc_mac_cell_start_cnf->optional_elems_present & RRC_MAC_CELL_START_CNF_SFN_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_sfn(pp_buffer, &p_rrc_mac_cell_start_cnf->sfn))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_cell_start_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_CELL_START_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_cell_start_cnf
(
    rrc_mac_cell_start_cnf_t  *p_rrc_mac_cell_start_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_cell_start_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_cell_start_cnf_len(p_rrc_mac_cell_start_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_CELL_START_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_CELL_START_CNF, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_CELL_START_CNF, msg_length,cell_index);

    /* Fill RRC_MAC_CELL_START_CNF message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_cell_start_cnf(&p_mac_msg, p_rrc_mac_cell_start_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_mac_reconfig_complete_ind_len
(
    rrc_mac_reconfig_complete_ind_t *p_rrc_mac_reconfig_complete_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_reconfig_complete_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reconfig_complete_ind->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_reconfig_complete_ind->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_mac_reconfig_complete_ind
(
    U8  **pp_buffer,
    rrc_mac_reconfig_complete_ind_t *p_rrc_mac_reconfig_complete_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_reconfig_complete_ind != PNULL);

    /* This function composes rrc_mac_reconfig_complete_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_reconfig_complete_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_mac_reconfig_complete_ind->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_mac_reconfig_complete_ind->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_mac_reconfig_complete_ind->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_mac_reconfig_complete_ind->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_reconfig_complete_ind->response, "response");
    *pp_buffer += sizeof(p_rrc_mac_reconfig_complete_ind->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_mac_il_send_rrc_mac_reconfig_complete_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_MAC_RECONFIG_COMPLETE_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_mac_il_send_rrc_mac_reconfig_complete_ind
(
    rrc_mac_reconfig_complete_ind_t  *p_rrc_mac_reconfig_complete_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_ASSERT(p_rrc_mac_reconfig_complete_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_mac_reconfig_complete_ind_len(p_rrc_mac_reconfig_complete_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_MAC_RECONFIG_COMPLETE_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_MAC_RECONFIG_COMPLETE_IND, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
        RRC_MAC_MODULE_ID, RRC_MAC_RECONFIG_COMPLETE_IND, msg_length,cell_index);

    /* Fill RRC_MAC_RECONFIG_COMPLETE_IND message */
    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_mac_reconfig_complete_ind(&p_mac_msg, p_rrc_mac_reconfig_complete_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_mac_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

