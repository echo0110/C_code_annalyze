/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_oam_parser.c $
 *
 *******************************************************************************
 *
 * File Description: File son_oam_parser.c added for SON module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "son_pack_unpack_utils.h"
#include "son_oam_parser.h"
#include "son_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))
extern const son_8* son_utils_facility_name;


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
son_return_et
son_parse_son_oam_module_log_config
(
    son_oam_module_log_config_t *p_son_oam_module_log_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_oam_module_init_config
(
    son_oam_module_init_config_t *p_son_oam_module_init_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_oam_module_log_level
(
    son_oam_module_log_level_t *p_son_oam_module_log_level,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cell_plmn_info
(
    son_cell_plmn_info_t *p_son_cell_plmn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_intra_rat_global_cell_id
(
    son_intra_rat_global_cell_id_t *p_son_intra_rat_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_fdd_info
(
    son_fdd_info_t *p_son_fdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_special_subframe_info
(
    son_special_subframe_info_t *p_son_special_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_tdd_info
(
    son_tdd_info_t *p_son_tdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_choice_eutra_mode
(
    son_choice_eutra_mode_t *p_son_choice_eutra_mode,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_prach_configuration
(
    son_prach_configuration_t *p_son_prach_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_per_freq_pci_range
(
    son_per_freq_pci_range_t *p_son_per_freq_pci_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_oam_abs_threshold_usage
(
    son_oam_abs_threshold_usage_t *p_son_oam_abs_threshold_usage,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_x2_second_degree_conflict_config
(
    son_x2_second_degree_conflict_config_t *p_son_x2_second_degree_conflict_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cell
(
    son_cell_t *p_son_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_meas_bandwidth_pci_per_earfcn
(
    son_meas_bandwidth_pci_per_earfcn_t *p_son_meas_bandwidth_pci_per_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_meas_bandwidth_earfcn_list
(
    son_meas_bandwidth_earfcn_list_t *p_son_meas_bandwidth_earfcn_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_start_intra_rat_scan_req
(
    son_start_intra_rat_scan_req_t *p_son_start_intra_rat_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_start_utra_scan_req
(
    son_start_utra_scan_req_t *p_son_start_utra_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_start_geran_scan_req
(
    son_start_geran_scan_req_t *p_son_start_geran_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_start_cdma_scan_req
(
    son_start_cdma_scan_req_t *p_son_start_cdma_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_start_inter_rat_scan_req
(
    son_start_inter_rat_scan_req_t *p_son_start_inter_rat_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_rf_cell_num
(
    son_rf_cell_num_t *p_son_rf_cell_num,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_plmn_identity_info
(
    son_plmn_identity_info_t *p_son_plmn_identity_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_bcch_cell_num
(
    son_bcch_cell_num_t *p_son_bcch_cell_num,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cell_info
(
    son_cell_info_t *p_son_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_rf_freq_channel_num
(
    son_rf_freq_channel_num_t *p_son_rf_freq_channel_num,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_start_intra_rat_bulk_scan_resp
(
    son_start_intra_rat_bulk_scan_resp_t *p_son_start_intra_rat_bulk_scan_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_start_inter_rat_bulk_scan_resp
(
    son_start_inter_rat_bulk_scan_resp_t *p_son_start_inter_rat_bulk_scan_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_bulk_nw_scan_resp
(
    son_bulk_nw_scan_resp_t *p_son_bulk_nw_scan_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_neighbor_removal_info
(
    son_neighbor_removal_info_t *p_son_neighbor_removal_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_neighbor_pci_confusion_info
(
    son_neighbor_pci_confusion_info_t *p_son_neighbor_pci_confusion_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cell_status
(
    son_cell_status_t *p_son_cell_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_neighbor_handover_stats
(
    son_neighbor_handover_stats_t *p_son_neighbor_handover_stats,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_oam_eicic_info
(
    son_oam_eicic_info_t *p_son_oam_eicic_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_subframe_allocation
(
    son_subframe_allocation_t *p_son_subframe_allocation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_mbsfn_info
(
    son_mbsfn_info_t *p_son_mbsfn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_mbms_configuration
(
    son_mbms_configuration_t *p_son_mbms_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_intra_rat_neighbor_info
(
    son_intra_rat_neighbor_info_t *p_son_intra_rat_neighbor_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_utran_geran_global_cell_id
(
    son_utran_geran_global_cell_id_t *p_son_utran_geran_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_utran_physical_cell_id
(
    son_utran_physical_cell_id_t *p_son_utran_physical_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_utran_neighbor_info
(
    son_utran_neighbor_info_t *p_son_utran_neighbor_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_geran_physical_cell_id
(
    son_geran_physical_cell_id_t *p_son_geran_physical_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_geran_neighbor_info
(
    son_geran_neighbor_info_t *p_son_geran_neighbor_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cdma_global_cell_id
(
    son_cdma_global_cell_id_t *p_son_cdma_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_oam_pz_hyst_params_included
(
    son_oam_pz_hyst_params_included_t *p_son_oam_pz_hyst_params_included,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_oam_fpc_fch_included
(
    son_oam_fpc_fch_included_t *p_son_oam_fpc_fch_included,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cdma_cell_specific_params
(
    son_cdma_cell_specific_params_t *p_son_cdma_cell_specific_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cdma_neighbor_info
(
    son_cdma_neighbor_info_t *p_son_cdma_neighbor_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_inter_rat_neighbor_info
(
    son_inter_rat_neighbor_info_t *p_son_inter_rat_neighbor_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_neighbor_info
(
    son_neighbor_info_t *p_son_neighbor_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_others_global_cell_id
(
    son_others_global_cell_id_t *p_son_others_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_inter_rat_global_cell_id
(
    son_inter_rat_global_cell_id_t *p_son_inter_rat_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_global_cell_id
(
    son_global_cell_id_t *p_son_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_nbr_cell_status
(
    son_nbr_cell_status_t *p_son_nbr_cell_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_speed_state_scale_factors
(
    son_speed_state_scale_factors_t *p_son_speed_state_scale_factors,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_meas_config_eutran
(
    son_meas_config_eutran_t *p_son_meas_config_eutran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_utran_pci_fdd_range
(
    son_utran_pci_fdd_range_t *p_son_utran_pci_fdd_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_meas_config_utran_fdd
(
    son_meas_config_utran_fdd_t *p_son_meas_config_utran_fdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_meas_config_utran_tdd
(
    son_meas_config_utran_tdd_t *p_son_meas_config_utran_tdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_meas_config_utran
(
    son_meas_config_utran_t *p_son_meas_config_utran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_geran_following_arfcns
(
    son_geran_following_arfcns_t *p_son_geran_following_arfcns,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_geran_carrier_freqs
(
    son_geran_carrier_freqs_t *p_son_geran_carrier_freqs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_geran_carrier_freq_info
(
    son_geran_carrier_freq_info_t *p_son_geran_carrier_freq_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_meas_config_geran
(
    son_meas_config_geran_t *p_son_meas_config_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cdma_carrier_freq_info
(
    son_cdma_carrier_freq_info_t *p_son_cdma_carrier_freq_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cdma_sys_time_info
(
    son_cdma_sys_time_info_t *p_son_cdma_sys_time_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_pre_reg_hrpd_info
(
    son_pre_reg_hrpd_info_t *p_son_pre_reg_hrpd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cdma_hrpd_info
(
    son_cdma_hrpd_info_t *p_son_cdma_hrpd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cdma_csfb_reg_param_1XRTT
(
    son_cdma_csfb_reg_param_1XRTT_t *p_son_cdma_csfb_reg_param_1XRTT,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cdma_1xrtt_info
(
    son_cdma_1xrtt_info_t *p_son_cdma_1xrtt_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cdma_csfb_reg_param_1xrtt_r9
(
    son_cdma_csfb_reg_param_1xrtt_r9_t *p_son_cdma_csfb_reg_param_1xrtt_r9,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_ac_barring_config_1xrtt
(
    son_ac_barring_config_1xrtt_t *p_son_ac_barring_config_1xrtt,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_meas_config_cdma
(
    son_meas_config_cdma_t *p_son_meas_config_cdma,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_anr_meas_config_inter_rat
(
    son_anr_meas_config_inter_rat_t *p_son_anr_meas_config_inter_rat,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_setup_meas_gap_config
(
    son_setup_meas_gap_config_t *p_son_setup_meas_gap_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_gap_config
(
    son_gap_config_t *p_son_gap_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_meas_gap_config
(
    son_meas_gap_config_t *p_son_meas_gap_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cell_on_off_status
(
    son_cell_on_off_status_t *p_son_cell_on_off_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_oam_anr_modify_attribute_info
(
    son_oam_anr_modify_attribute_info_t *p_son_oam_anr_modify_attribute_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_global_enb_id
(
    son_global_enb_id_t *p_son_global_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_served_cell_info
(
    son_served_cell_info_t *p_son_served_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_nbour_cell_info
(
    son_nbour_cell_info_t *p_son_nbour_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_served_cell
(
    son_served_cell_t *p_son_served_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_served_cell_info_arr
(
    son_served_cell_info_arr_t *p_son_served_cell_info_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_gu_group_id
(
    son_gu_group_id_t *p_son_gu_group_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_gu_group_id_list
(
    son_gu_group_id_list_t *p_son_gu_group_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_limited_mode_eutran_cell_info
(
    son_limited_mode_eutran_cell_info_t *p_son_limited_mode_eutran_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_tac
(
    son_tac_t *p_son_tac,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_enb_tnl_address
(
    son_enb_tnl_address_t *p_son_enb_tnl_address,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_neighbor_enb_info
(
    son_neighbor_enb_info_t *p_son_neighbor_enb_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_enb_status
(
    son_enb_status_t *p_son_enb_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_mro_eutran_config_param
(
    son_mro_eutran_config_param_t *p_son_mro_eutran_config_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_mro_counter_info
(
    son_mro_counter_info_t *p_son_mro_counter_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_eutran_q_offset_config
(
    son_eutran_q_offset_config_t *p_son_eutran_q_offset_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_oam_time_stamp
(
    son_oam_time_stamp_t *p_son_oam_time_stamp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_oam_event_header
(
    son_oam_event_header_t *p_son_oam_event_header,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_oam_error_info
(
    son_oam_error_info_t *p_son_oam_error_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cell_offset_attr
(
    son_cell_offset_attr_t *p_son_cell_offset_attr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_mlb_ho_params
(
    son_mlb_ho_params_t *p_son_mlb_ho_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_nbr_cell_load_info
(
    son_nbr_cell_load_info_t *p_son_nbr_cell_load_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_mlb_load_params
(
    son_mlb_load_params_t *p_son_mlb_load_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_mob_param_range
(
    son_mob_param_range_t *p_son_mob_param_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_rach_opt_oam_config_param
(
    son_rach_opt_oam_config_param_t *p_son_rach_opt_oam_config_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_oam_rach_opt_update_req_param
(
    son_oam_rach_opt_update_req_param_t *p_son_oam_rach_opt_update_req_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_strong_cell_info
(
    son_strong_cell_info_t *p_son_strong_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_ue_prach_data_info
(
    son_ue_prach_data_info_t *p_son_ue_prach_data_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_cell_delete_status
(
    son_cell_delete_status_t *p_son_cell_delete_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_PoolStats
(
    son_PoolStats *p_son_PoolStats,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_son_MemPoolStats
(
    son_MemPoolStats *p_son_MemPoolStats,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

son_return_et
son_parse_son_oam_init_config_req
(
    son_oam_init_config_req_t *p_son_oam_init_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_init_config_req, 0, sizeof(son_oam_init_config_req_t));

    /* This function parses son_oam_init_config_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_init_config_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_init_config_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_init_config_req->bitmask & SON_INIT_CONFIG_PRESENT)
    {

    /* Parse OCTET_STRING FIXED of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_oam_init_config_req->int_module_config); loop++)
        {
            if (SON_FAILURE == son_parse_son_oam_module_init_config(
                &p_son_oam_init_config_req->int_module_config[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_son_oam_init_config_req->bitmask & SON_HEALTH_UPD_TMR_PERIOD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_init_config_req->health_upd_tmr_period, p_src + *p_length_read, "health_upd_tmr_period");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_init_config_req->health_upd_tmr_period < 5) || (p_son_oam_init_config_req->health_upd_tmr_period > 900))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_init_config_req->health_upd_tmr_period] should be in range [5 <= %d <= 900] !", p_son_oam_init_config_req->health_upd_tmr_period);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_init_config_req->bitmask & SON_NMM_SUPPORT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_init_config_req->nmm_supported, p_src + *p_length_read, "nmm_supported");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_init_config_req->nmm_supported > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_init_config_req->nmm_supported] should be in range [%d <= 1] !", p_son_oam_init_config_req->nmm_supported);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_oam_module_log_config
(
    son_oam_module_log_config_t *p_son_oam_module_log_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_module_log_config, 0, sizeof(son_oam_module_log_config_t));

    /* This function parses son_oam_module_log_config */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_module_log_config");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_module_log_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_module_log_config->log_on_off, p_src + *p_length_read, "log_on_off");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_module_log_config->log_on_off > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_module_log_config->log_on_off] should be in range [%d <= 1] !", p_son_oam_module_log_config->log_on_off);
        return SON_FAILURE;
    }

    if (p_son_oam_module_log_config->bitmask & SON_OAM_MODULE_LOG_LEVEL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_module_log_config->log_level, p_src + *p_length_read, "log_level");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_module_log_config->log_level > 6))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_module_log_config->log_level] should be in range [%d <= 6] !", p_son_oam_module_log_config->log_level);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_oam_module_init_config
(
    son_oam_module_init_config_t *p_son_oam_module_init_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_module_init_config, 0, sizeof(son_oam_module_init_config_t));

    /* This function parses son_oam_module_init_config */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_module_init_config");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_module_init_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_module_init_config->module_id, p_src + *p_length_read, "module_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_module_init_config->module_id < 25701) || (p_son_oam_module_init_config->module_id > 25708))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_module_init_config->module_id] should be in range [25701 <= %d <= 25708] !", p_son_oam_module_init_config->module_id);
        return SON_FAILURE;
    }



    if (SON_FAILURE == son_parse_son_oam_module_log_config(
        &p_son_oam_module_init_config->log_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_get_log_level_req
(
    son_oam_get_log_level_req_t *p_son_oam_get_log_level_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_get_log_level_req, 0, sizeof(son_oam_get_log_level_req_t));

    /* This function parses son_oam_get_log_level_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_get_log_level_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_get_log_level_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_get_log_level_req->module_id, p_src + *p_length_read, "module_id");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_get_log_level_resp
(
    son_oam_get_log_level_resp_t *p_son_oam_get_log_level_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_get_log_level_resp, 0, sizeof(son_oam_get_log_level_resp_t));

    /* This function parses son_oam_get_log_level_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_get_log_level_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_get_log_level_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_get_log_level_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_get_log_level_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_get_log_level_resp->result] should be in range [%d <= 2] !", p_son_oam_get_log_level_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_get_log_level_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_get_log_level_resp->num_of_modules, p_src + *p_length_read, "num_of_modules");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_get_log_level_resp->num_of_modules > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_get_log_level_resp->num_of_modules] should be in range [%d <= 8] !", p_son_oam_get_log_level_resp->num_of_modules);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_get_log_level_resp->num_of_modules; loop++)
        {
            if (SON_FAILURE == son_parse_son_oam_module_log_level(
                &p_son_oam_get_log_level_resp->module_log_level[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_oam_module_log_level
(
    son_oam_module_log_level_t *p_son_oam_module_log_level,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_module_log_level, 0, sizeof(son_oam_module_log_level_t));

    /* This function parses son_oam_module_log_level */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_module_log_level");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_module_log_level->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_module_log_level->module_id, p_src + *p_length_read, "module_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_module_log_level->module_id < 25701) || (p_son_oam_module_log_level->module_id > 25708))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_module_log_level->module_id] should be in range [25701 <= %d <= 25708] !", p_son_oam_module_log_level->module_id);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_module_log_level->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (p_son_oam_module_log_level->bitmask & SON_MODULE_LOG_LEVEL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_module_log_level->log_level, p_src + *p_length_read, "log_level");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_module_log_level->log_level > 6))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_module_log_level->log_level] should be in range [%d <= 6] !", p_son_oam_module_log_level->log_level);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_init_config_resp
(
    son_oam_init_config_resp_t *p_son_oam_init_config_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_init_config_resp, 0, sizeof(son_oam_init_config_resp_t));

    /* This function parses son_oam_init_config_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_init_config_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_init_config_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_init_config_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_init_config_resp->result] should be in range [%d <= 1] !", p_son_oam_init_config_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_init_config_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_shutdown_req
(
    son_oam_shutdown_req_t *p_son_oam_shutdown_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_shutdown_req, 0, sizeof(son_oam_shutdown_req_t));

    /* This function parses son_oam_shutdown_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_shutdown_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_shutdown_req->shutdown_type, p_src + *p_length_read, "shutdown_type");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_shutdown_req->shutdown_type > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_shutdown_req->shutdown_type] should be in range [%d <= 1] !", p_son_oam_shutdown_req->shutdown_type);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_shutdown_req->time_to_shutdown, p_src + *p_length_read, "time_to_shutdown");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_shutdown_resp
(
    son_oam_shutdown_resp_t *p_son_oam_shutdown_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_shutdown_resp, 0, sizeof(son_oam_shutdown_resp_t));

    /* This function parses son_oam_shutdown_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_shutdown_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_shutdown_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_shutdown_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_shutdown_resp->result] should be in range [%d <= 1] !", p_son_oam_shutdown_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_shutdown_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_set_log_level_req
(
    son_oam_set_log_level_req_t *p_son_oam_set_log_level_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_set_log_level_req, 0, sizeof(son_oam_set_log_level_req_t));

    /* This function parses son_oam_set_log_level_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_set_log_level_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_set_log_level_req->module_id, p_src + *p_length_read, "module_id");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_set_log_level_req->log_level, p_src + *p_length_read, "log_level");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_set_log_level_req->log_level > 6))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_set_log_level_req->log_level] should be in range [%d <= 6] !", p_son_oam_set_log_level_req->log_level);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_set_log_level_resp
(
    son_oam_set_log_level_resp_t *p_son_oam_set_log_level_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_set_log_level_resp, 0, sizeof(son_oam_set_log_level_resp_t));

    /* This function parses son_oam_set_log_level_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_set_log_level_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_set_log_level_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_set_log_level_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_set_log_level_resp->result] should be in range [%d <= 1] !", p_son_oam_set_log_level_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_set_log_level_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_log_enable_disable_req
(
    son_oam_log_enable_disable_req_t *p_son_oam_log_enable_disable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_log_enable_disable_req, 0, sizeof(son_oam_log_enable_disable_req_t));

    /* This function parses son_oam_log_enable_disable_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_log_enable_disable_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_log_enable_disable_req->module_id, p_src + *p_length_read, "module_id");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_oam_module_log_config(
        &p_son_oam_log_enable_disable_req->log_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_log_enable_disable_req->feature_id, p_src + *p_length_read, "feature_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_log_enable_disable_req->feature_id > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_log_enable_disable_req->feature_id] should be in range [%d <= 3] !", p_son_oam_log_enable_disable_req->feature_id);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_log_enable_disable_resp
(
    son_oam_log_enable_disable_resp_t *p_son_oam_log_enable_disable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_log_enable_disable_resp, 0, sizeof(son_oam_log_enable_disable_resp_t));

    /* This function parses son_oam_log_enable_disable_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_log_enable_disable_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_log_enable_disable_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_log_enable_disable_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_log_enable_disable_resp->result] should be in range [%d <= 1] !", p_son_oam_log_enable_disable_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_log_enable_disable_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_cell_info_ind
(
    son_oam_cell_info_ind_t *p_son_oam_cell_info_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_cell_info_ind, 0, sizeof(son_oam_cell_info_ind_t));

    /* This function parses son_oam_cell_info_ind */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_cell_info_ind");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_cell_info_ind->cell_info_size, p_src + *p_length_read, "cell_info_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_cell_info_ind->cell_info_size < 1) || (p_son_oam_cell_info_ind->cell_info_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_cell_info_ind->cell_info_size] should be in range [1 <= %d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_cell_info_ind->cell_info_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_cell_info_ind->cell_info_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell(
                &p_son_oam_cell_info_ind->serving_cell[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cell_plmn_info
(
    son_cell_plmn_info_t *p_son_cell_plmn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_cell_plmn_info, 0, sizeof(son_cell_plmn_info_t));

    /* This function parses son_cell_plmn_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cell_plmn_info");

    if (*p_length_read + (S32)sizeof(p_son_cell_plmn_info->mcc) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_cell_plmn_info->mcc); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_cell_plmn_info->mcc[loop], (void*)(p_src + *p_length_read), "mcc[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cell_plmn_info->num_mnc_digit, p_src + *p_length_read, "num_mnc_digit");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_cell_plmn_info->num_mnc_digit < 2) || (p_son_cell_plmn_info->num_mnc_digit > SON_MAX_MNC_OCTET_SIZE))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell_plmn_info->num_mnc_digit] should be in range [2 <= %d <= SON_MAX_MNC_OCTET_SIZE] !", p_son_cell_plmn_info->num_mnc_digit);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_cell_plmn_info->num_mnc_digit * sizeof(p_son_cell_plmn_info->mnc[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_cell_plmn_info->num_mnc_digit; loop++)
        {
            son_cp_unpack_U8((void*)&p_son_cell_plmn_info->mnc[loop], (void*)(p_src + *p_length_read), "mnc[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_intra_rat_global_cell_id
(
    son_intra_rat_global_cell_id_t *p_son_intra_rat_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_intra_rat_global_cell_id, 0, sizeof(son_intra_rat_global_cell_id_t));

    /* This function parses son_intra_rat_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_intra_rat_global_cell_id");



    if (SON_FAILURE == son_parse_son_cell_plmn_info(
        &p_son_intra_rat_global_cell_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_son_intra_rat_global_cell_id->cell_identity) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_intra_rat_global_cell_id->cell_identity); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_intra_rat_global_cell_id->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_fdd_info
(
    son_fdd_info_t *p_son_fdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_fdd_info, 0, sizeof(son_fdd_info_t));

    /* This function parses son_fdd_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_fdd_info");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_fdd_info->ul_earfcn, p_src + *p_length_read, "ul_earfcn");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_fdd_info->dl_earfcn, p_src + *p_length_read, "dl_earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_fdd_info->ul_trnsmission_bdwidth, p_src + *p_length_read, "ul_trnsmission_bdwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_fdd_info->ul_trnsmission_bdwidth > 5))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_fdd_info->ul_trnsmission_bdwidth] should be in range [%d <= 5] !", p_son_fdd_info->ul_trnsmission_bdwidth);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_fdd_info->dl_trnsmission_bdwidth, p_src + *p_length_read, "dl_trnsmission_bdwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_fdd_info->dl_trnsmission_bdwidth > 5))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_fdd_info->dl_trnsmission_bdwidth] should be in range [%d <= 5] !", p_son_fdd_info->dl_trnsmission_bdwidth);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_special_subframe_info
(
    son_special_subframe_info_t *p_son_special_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_special_subframe_info, 0, sizeof(son_special_subframe_info_t));

    /* This function parses son_special_subframe_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_special_subframe_info");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_special_subframe_info->special_subframe_patterns, p_src + *p_length_read, "special_subframe_patterns");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_special_subframe_info->special_subframe_patterns > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_special_subframe_info->special_subframe_patterns] should be in range [%d <= 8] !", p_son_special_subframe_info->special_subframe_patterns);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_special_subframe_info->dl_cp, p_src + *p_length_read, "dl_cp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_special_subframe_info->dl_cp > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_special_subframe_info->dl_cp] should be in range [%d <= 1] !", p_son_special_subframe_info->dl_cp);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_special_subframe_info->ul_cp, p_src + *p_length_read, "ul_cp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_special_subframe_info->ul_cp > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_special_subframe_info->ul_cp] should be in range [%d <= 1] !", p_son_special_subframe_info->ul_cp);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_tdd_info
(
    son_tdd_info_t *p_son_tdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_tdd_info, 0, sizeof(son_tdd_info_t));

    /* This function parses son_tdd_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_tdd_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_tdd_info->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_tdd_info->trnsmission_bdwidth, p_src + *p_length_read, "trnsmission_bdwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_tdd_info->trnsmission_bdwidth > 5))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_tdd_info->trnsmission_bdwidth] should be in range [%d <= 5] !", p_son_tdd_info->trnsmission_bdwidth);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_tdd_info->subframe_assn, p_src + *p_length_read, "subframe_assn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_tdd_info->subframe_assn > 6))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_tdd_info->subframe_assn] should be in range [%d <= 6] !", p_son_tdd_info->subframe_assn);
        return SON_FAILURE;
    }



    if (SON_FAILURE == son_parse_son_special_subframe_info(
        &p_son_tdd_info->special_subframe_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_choice_eutra_mode
(
    son_choice_eutra_mode_t *p_son_choice_eutra_mode,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_choice_eutra_mode, 0, sizeof(son_choice_eutra_mode_t));

    /* This function parses son_choice_eutra_mode */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_choice_eutra_mode");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_choice_eutra_mode->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_choice_eutra_mode->bitmask & SON_EUTRA_MODE_FDD)
    {



    if (SON_FAILURE == son_parse_son_fdd_info(
        &p_son_choice_eutra_mode->fdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_choice_eutra_mode->bitmask & SON_EUTRA_MODE_TDD)
    {



    if (SON_FAILURE == son_parse_son_tdd_info(
        &p_son_choice_eutra_mode->tdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_prach_configuration
(
    son_prach_configuration_t *p_son_prach_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_prach_configuration, 0, sizeof(son_prach_configuration_t));

    /* This function parses son_prach_configuration */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_prach_configuration");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_prach_configuration->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_prach_configuration->root_sequence_index, p_src + *p_length_read, "root_sequence_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_prach_configuration->root_sequence_index > 837))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_prach_configuration->root_sequence_index] should be in range [%d <= 837] !", p_son_prach_configuration->root_sequence_index);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_prach_configuration->zero_correlation_zone_config, p_src + *p_length_read, "zero_correlation_zone_config");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_prach_configuration->zero_correlation_zone_config > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_prach_configuration->zero_correlation_zone_config] should be in range [%d <= 15] !", p_son_prach_configuration->zero_correlation_zone_config);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_prach_configuration->high_speed_flag, p_src + *p_length_read, "high_speed_flag");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_prach_configuration->high_speed_flag > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_prach_configuration->high_speed_flag] should be in range [%d <= 1] !", p_son_prach_configuration->high_speed_flag);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_prach_configuration->prach_frequency_offset, p_src + *p_length_read, "prach_frequency_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_prach_configuration->prach_frequency_offset > 94))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_prach_configuration->prach_frequency_offset] should be in range [%d <= 94] !", p_son_prach_configuration->prach_frequency_offset);
        return SON_FAILURE;
    }

    if (p_son_prach_configuration->bitmask & SON_PRACH_CONFIG_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_prach_configuration->prach_configuration_index, p_src + *p_length_read, "prach_configuration_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_prach_configuration->prach_configuration_index > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_prach_configuration->prach_configuration_index] should be in range [%d <= 63] !", p_son_prach_configuration->prach_configuration_index);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_per_freq_pci_range
(
    son_per_freq_pci_range_t *p_son_per_freq_pci_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_per_freq_pci_range, 0, sizeof(son_per_freq_pci_range_t));

    /* This function parses son_per_freq_pci_range */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_per_freq_pci_range");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_per_freq_pci_range->pci_start, p_src + *p_length_read, "pci_start");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_per_freq_pci_range->pci_start > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_per_freq_pci_range->pci_start] should be in range [%d <= 503] !", p_son_per_freq_pci_range->pci_start);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_per_freq_pci_range->pci_range, p_src + *p_length_read, "pci_range");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_per_freq_pci_range->pci_range > 13))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_per_freq_pci_range->pci_range] should be in range [%d <= 13] !", p_son_per_freq_pci_range->pci_range);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_oam_abs_threshold_usage
(
    son_oam_abs_threshold_usage_t *p_son_oam_abs_threshold_usage,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_abs_threshold_usage, 0, sizeof(son_oam_abs_threshold_usage_t));

    /* This function parses son_oam_abs_threshold_usage */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_abs_threshold_usage");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_abs_threshold_usage->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_abs_threshold_usage->bitmask & SON_CELL_MIN_ABS_USAGE_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_abs_threshold_usage->min_abs_usage_threshold, p_src + *p_length_read, "min_abs_usage_threshold");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_abs_threshold_usage->min_abs_usage_threshold > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_abs_threshold_usage->min_abs_usage_threshold] should be in range [%d <= 100] !", p_son_oam_abs_threshold_usage->min_abs_usage_threshold);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_abs_threshold_usage->bitmask & SON_CELL_MAX_ABS_USAGE_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_abs_threshold_usage->max_abs_usage_threshold, p_src + *p_length_read, "max_abs_usage_threshold");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_abs_threshold_usage->max_abs_usage_threshold > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_abs_threshold_usage->max_abs_usage_threshold] should be in range [%d <= 100] !", p_son_oam_abs_threshold_usage->max_abs_usage_threshold);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_x2_second_degree_conflict_config
(
    son_x2_second_degree_conflict_config_t *p_son_x2_second_degree_conflict_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_x2_second_degree_conflict_config, 0, sizeof(son_x2_second_degree_conflict_config_t));

    /* This function parses son_x2_second_degree_conflict_config */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_x2_second_degree_conflict_config");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_x2_second_degree_conflict_config->second_degree_conflict_enable, p_src + *p_length_read, "second_degree_conflict_enable");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_x2_second_degree_conflict_config->second_degree_conflict_enable > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_x2_second_degree_conflict_config->second_degree_conflict_enable] should be in range [%d <= 1] !", p_son_x2_second_degree_conflict_config->second_degree_conflict_enable);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_x2_second_degree_conflict_config->nr_add_wait_timer, p_src + *p_length_read, "nr_add_wait_timer");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_x2_second_degree_conflict_config->conflict_detection_backoff_timer, p_src + *p_length_read, "conflict_detection_backoff_timer");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cell
(
    son_cell_t *p_son_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_cell, 0, sizeof(son_cell_t));

    /* This function parses son_cell */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cell");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cell->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_cell->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_cell->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cell->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell->pci] should be in range [%d <= 503] !", p_son_cell->pci);
        return SON_FAILURE;
    }

    if (p_son_cell->bitmask & SON_CELL_TAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_cell->tac) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_cell->tac); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_cell->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_cell->bitmask & SON_CELL_PLMN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cell->num_plmn_id, p_src + *p_length_read, "num_plmn_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_cell->num_plmn_id < 1) || (p_son_cell->num_plmn_id > SON_MAX_NUM_PLMN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell->num_plmn_id] should be in range [1 <= %d <= SON_MAX_NUM_PLMN] !", p_son_cell->num_plmn_id);
        return SON_FAILURE;
    }
    }

    if (p_son_cell->bitmask & SON_CELL_PLMN_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_cell->num_plmn_id; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_plmn_info(
                &p_son_cell->plmn_id[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_son_cell->bitmask & SON_CELL_EUTRA_MODE_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_choice_eutra_mode(
        &p_son_cell->choice_eutra_mode,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_cell->bitmask & SON_CELL_CSG_IDENTITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_cell->csg_identity) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_cell->csg_identity); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_cell->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_cell->bitmask & SON_CELL_PREAMBLE_INITIAL_RCVD_TARGET_POWER_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cell->preamble_initial_rcvd_target_power, p_src + *p_length_read, "preamble_initial_rcvd_target_power");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cell->preamble_initial_rcvd_target_power > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell->preamble_initial_rcvd_target_power] should be in range [%d <= 15] !", p_son_cell->preamble_initial_rcvd_target_power);
        return SON_FAILURE;
    }
    }

    if (p_son_cell->bitmask & SON_CELL_NUMBER_OF_RA_PREAMBLES_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cell->number_of_ra_preambles, p_src + *p_length_read, "number_of_ra_preambles");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cell->number_of_ra_preambles > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell->number_of_ra_preambles] should be in range [%d <= 15] !", p_son_cell->number_of_ra_preambles);
        return SON_FAILURE;
    }
    }

    if (p_son_cell->bitmask & SON_CELL_SIZE_OF_RA_GROUP_A_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cell->size_of_ra_group_a, p_src + *p_length_read, "size_of_ra_group_a");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cell->size_of_ra_group_a > 14))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell->size_of_ra_group_a] should be in range [%d <= 14] !", p_son_cell->size_of_ra_group_a);
        return SON_FAILURE;
    }
    }

    if (p_son_cell->bitmask & SON_CELL_PRACH_CONFIG_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_prach_configuration(
        &p_son_cell->prach_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_cell->bitmask & SON_CELL_PUSCH_HOPPING_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cell->pusch_hopping_offset, p_src + *p_length_read, "pusch_hopping_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cell->pusch_hopping_offset > 98))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell->pusch_hopping_offset] should be in range [%d <= 98] !", p_son_cell->pusch_hopping_offset);
        return SON_FAILURE;
    }
    }

    if (p_son_cell->bitmask & SON_CELL_INTRA_FREQ_OPEN_PCI_RANGE_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_per_freq_pci_range(
        &p_son_cell->intra_freq_open_pci_range,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_cell->bitmask & SON_CELL_INTRA_FREQ_CLOSED_PCI_RANGE_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_per_freq_pci_range(
        &p_son_cell->intra_freq_closed_pci_range,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_cell->bitmask & SON_CELL_INTRA_FREQ_HYBRID_PCI_RANGE_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_per_freq_pci_range(
        &p_son_cell->intra_freq_hybrid_pci_range,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_cell->bitmask & SON_CELL_ABS_USAGE_THRESHOLD_RANGE)
    {



    if (SON_FAILURE == son_parse_son_oam_abs_threshold_usage(
        &p_son_cell->abs_usage_threshold,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_cell->bitmask & SON_CELL_SECOND_DEGREE_CONFLICT_CONFIG_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_x2_second_degree_conflict_config(
        &p_son_cell->son_x2_second_degree_conflict_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_peer_cell_activation_req
(
    son_oam_peer_cell_activation_req_t *p_son_oam_peer_cell_activation_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_peer_cell_activation_req, 0, sizeof(son_oam_peer_cell_activation_req_t));

    /* This function parses son_oam_peer_cell_activation_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_peer_cell_activation_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_peer_cell_activation_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_peer_cell_activation_req->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_peer_cell_activation_req->num_served_cell > MAX_SERVED_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_peer_cell_activation_req->num_served_cell] should be in range [%d <= MAX_SERVED_CELLS] !", p_son_oam_peer_cell_activation_req->num_served_cell);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_peer_cell_activation_req->num_served_cell; loop++)
        {
            if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
                &p_son_oam_peer_cell_activation_req->served_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_peer_cell_activation_resp
(
    son_oam_peer_cell_activation_resp_t *p_son_oam_peer_cell_activation_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_peer_cell_activation_resp, 0, sizeof(son_oam_peer_cell_activation_resp_t));

    /* This function parses son_oam_peer_cell_activation_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_peer_cell_activation_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_peer_cell_activation_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_peer_cell_activation_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_peer_cell_activation_resp->result] should be in range [%d <= 2] !", p_son_oam_peer_cell_activation_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_peer_cell_activation_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nw_scan_req
(
    son_oam_nw_scan_req_t *p_son_oam_nw_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nw_scan_req, 0, sizeof(son_oam_nw_scan_req_t));

    /* This function parses son_oam_nw_scan_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nw_scan_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nw_scan_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_nw_scan_req->bitmask & SON_INTRA_RAT_NW_SCAN_REQ_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_start_intra_rat_scan_req(
        &p_son_oam_nw_scan_req->start_intra_rat_scan_req,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_oam_nw_scan_req->bitmask & SON_INTER_RAT_NW_SCAN_REQ_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_start_inter_rat_scan_req(
        &p_son_oam_nw_scan_req->start_inter_rat_scan_req,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_meas_bandwidth_pci_per_earfcn
(
    son_meas_bandwidth_pci_per_earfcn_t *p_son_meas_bandwidth_pci_per_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_meas_bandwidth_pci_per_earfcn, 0, sizeof(son_meas_bandwidth_pci_per_earfcn_t));

    /* This function parses son_meas_bandwidth_pci_per_earfcn */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_meas_bandwidth_pci_per_earfcn");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_bandwidth_pci_per_earfcn->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_bandwidth_pci_per_earfcn->meas_bandwidth_for_earfcn, p_src + *p_length_read, "meas_bandwidth_for_earfcn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_bandwidth_pci_per_earfcn->meas_bandwidth_for_earfcn < 6) || (p_son_meas_bandwidth_pci_per_earfcn->meas_bandwidth_for_earfcn > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_bandwidth_pci_per_earfcn->meas_bandwidth_for_earfcn] should be in range [6 <= %d <= 100] !", p_son_meas_bandwidth_pci_per_earfcn->meas_bandwidth_for_earfcn);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_meas_bandwidth_pci_per_earfcn->num_pci, p_src + *p_length_read, "num_pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_bandwidth_pci_per_earfcn->num_pci > SON_MAX_NO_CELLS_PER_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_bandwidth_pci_per_earfcn->num_pci] should be in range [%d <= SON_MAX_NO_CELLS_PER_EARFCN] !", p_son_meas_bandwidth_pci_per_earfcn->num_pci);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_meas_bandwidth_pci_per_earfcn->num_pci * sizeof(p_son_meas_bandwidth_pci_per_earfcn->pci[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_meas_bandwidth_pci_per_earfcn->num_pci; loop++)
        {
            son_cp_unpack_U16((void*)&p_son_meas_bandwidth_pci_per_earfcn->pci[loop], (void*)(p_src + *p_length_read), "pci[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_meas_bandwidth_earfcn_list
(
    son_meas_bandwidth_earfcn_list_t *p_son_meas_bandwidth_earfcn_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_meas_bandwidth_earfcn_list, 0, sizeof(son_meas_bandwidth_earfcn_list_t));

    /* This function parses son_meas_bandwidth_earfcn_list */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_meas_bandwidth_earfcn_list");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_meas_bandwidth_earfcn_list->num_earfcn, p_src + *p_length_read, "num_earfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_bandwidth_earfcn_list->num_earfcn < 1) || (p_son_meas_bandwidth_earfcn_list->num_earfcn > SON_MAX_NO_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_bandwidth_earfcn_list->num_earfcn] should be in range [1 <= %d <= SON_MAX_NO_EARFCN] !", p_son_meas_bandwidth_earfcn_list->num_earfcn);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_meas_bandwidth_earfcn_list->num_earfcn; loop++)
        {
            if (SON_FAILURE == son_parse_son_meas_bandwidth_pci_per_earfcn(
                &p_son_meas_bandwidth_earfcn_list->meas_bandwidth_per_earfcn[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_start_intra_rat_scan_req
(
    son_start_intra_rat_scan_req_t *p_son_start_intra_rat_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_start_intra_rat_scan_req, 0, sizeof(son_start_intra_rat_scan_req_t));

    /* This function parses son_start_intra_rat_scan_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_start_intra_rat_scan_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_start_intra_rat_scan_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_start_intra_rat_scan_req->measurement_period, p_src + *p_length_read, "measurement_period");
    *p_length_read += sizeof(U8);

    /* Check for correct range [L - lower boundary] */
    if ((p_son_start_intra_rat_scan_req->measurement_period < 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_start_intra_rat_scan_req->measurement_period] should be in range [1 <= %d] !", p_son_start_intra_rat_scan_req->measurement_period);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_start_intra_rat_scan_req->retry_count, p_src + *p_length_read, "retry_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_start_intra_rat_scan_req->retry_count < 1) || (p_son_start_intra_rat_scan_req->retry_count > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_start_intra_rat_scan_req->retry_count] should be in range [1 <= %d <= 8] !", p_son_start_intra_rat_scan_req->retry_count);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_start_intra_rat_scan_req->freq_band_id, p_src + *p_length_read, "freq_band_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_start_intra_rat_scan_req->freq_band_id < 1) || (p_son_start_intra_rat_scan_req->freq_band_id > 64))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_start_intra_rat_scan_req->freq_band_id] should be in range [1 <= %d <= 64] !", p_son_start_intra_rat_scan_req->freq_band_id);
        return SON_FAILURE;
    }

    if (p_son_start_intra_rat_scan_req->bitmask & SON_MEAS_BANDWINDTH_PER_EARFCN_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_meas_bandwidth_earfcn_list(
        &p_son_start_intra_rat_scan_req->meas_bandwidth_per_earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_start_intra_rat_scan_req->bitmask & SON_MEAS_BANDWINDTH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_start_intra_rat_scan_req->meas_bandwidth, p_src + *p_length_read, "meas_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_start_intra_rat_scan_req->meas_bandwidth < 6) || (p_son_start_intra_rat_scan_req->meas_bandwidth > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_start_intra_rat_scan_req->meas_bandwidth] should be in range [6 <= %d <= 100] !", p_son_start_intra_rat_scan_req->meas_bandwidth);
        return SON_FAILURE;
    }
    }

    if (p_son_start_intra_rat_scan_req->bitmask & SON_NW_SCAN_PLMN_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_start_intra_rat_scan_req->num_plmn_id, p_src + *p_length_read, "num_plmn_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_start_intra_rat_scan_req->num_plmn_id < 1) || (p_son_start_intra_rat_scan_req->num_plmn_id > SON_MAX_NUM_PLMN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_start_intra_rat_scan_req->num_plmn_id] should be in range [1 <= %d <= SON_MAX_NUM_PLMN] !", p_son_start_intra_rat_scan_req->num_plmn_id);
        return SON_FAILURE;
    }
    }

    if (p_son_start_intra_rat_scan_req->bitmask & SON_NW_SCAN_PLMN_ID_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_start_intra_rat_scan_req->num_plmn_id; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_plmn_info(
                &p_son_start_intra_rat_scan_req->plmn_id[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_start_utra_scan_req
(
    son_start_utra_scan_req_t *p_son_start_utra_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_start_utra_scan_req, 0, sizeof(son_start_utra_scan_req_t));

    /* This function parses son_start_utra_scan_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_start_utra_scan_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_start_utra_scan_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_start_utra_scan_req->measurement_period, p_src + *p_length_read, "measurement_period");
    *p_length_read += sizeof(U8);

    /* Check for correct range [L - lower boundary] */
    if ((p_son_start_utra_scan_req->measurement_period < 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_start_utra_scan_req->measurement_period] should be in range [1 <= %d] !", p_son_start_utra_scan_req->measurement_period);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_start_utra_scan_req->retry_count, p_src + *p_length_read, "retry_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_start_utra_scan_req->retry_count < 1) || (p_son_start_utra_scan_req->retry_count > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_start_utra_scan_req->retry_count] should be in range [1 <= %d <= 8] !", p_son_start_utra_scan_req->retry_count);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_start_utra_scan_req->freq_band_id, p_src + *p_length_read, "freq_band_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_start_utra_scan_req->freq_band_id < 1) || (p_son_start_utra_scan_req->freq_band_id > 64))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_start_utra_scan_req->freq_band_id] should be in range [1 <= %d <= 64] !", p_son_start_utra_scan_req->freq_band_id);
        return SON_FAILURE;
    }

    if (p_son_start_utra_scan_req->bitmask & SON_MEAS_UTRAN_BANDWINDTH_PER_ARFCN_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_meas_bandwidth_earfcn_list(
        &p_son_start_utra_scan_req->meas_bandwidth_per_arfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_start_utra_scan_req->bitmask & SON_MEAS_UTRAN_BANDWINDTH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_start_utra_scan_req->meas_bandwidth, p_src + *p_length_read, "meas_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_start_utra_scan_req->meas_bandwidth < 6) || (p_son_start_utra_scan_req->meas_bandwidth > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_start_utra_scan_req->meas_bandwidth] should be in range [6 <= %d <= 100] !", p_son_start_utra_scan_req->meas_bandwidth);
        return SON_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_start_utra_scan_req->num_pci, p_src + *p_length_read, "num_pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_start_utra_scan_req->num_pci > SON_MAX_NO_CELLS_PER_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_start_utra_scan_req->num_pci] should be in range [%d <= SON_MAX_NO_CELLS_PER_EARFCN] !", p_son_start_utra_scan_req->num_pci);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_start_utra_scan_req->num_pci * sizeof(p_son_start_utra_scan_req->pci[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_start_utra_scan_req->num_pci; loop++)
        {
            son_cp_unpack_U16((void*)&p_son_start_utra_scan_req->pci[loop], (void*)(p_src + *p_length_read), "pci[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_start_geran_scan_req
(
    son_start_geran_scan_req_t *p_son_start_geran_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_start_geran_scan_req, 0, sizeof(son_start_geran_scan_req_t));

    /* This function parses son_start_geran_scan_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_start_geran_scan_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_start_geran_scan_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_start_cdma_scan_req
(
    son_start_cdma_scan_req_t *p_son_start_cdma_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_start_cdma_scan_req, 0, sizeof(son_start_cdma_scan_req_t));

    /* This function parses son_start_cdma_scan_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_start_cdma_scan_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_start_cdma_scan_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_start_inter_rat_scan_req
(
    son_start_inter_rat_scan_req_t *p_son_start_inter_rat_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_start_inter_rat_scan_req, 0, sizeof(son_start_inter_rat_scan_req_t));

    /* This function parses son_start_inter_rat_scan_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_start_inter_rat_scan_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_start_inter_rat_scan_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_start_inter_rat_scan_req->bitmask & SON_UTRA_SCAN_REQ_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_start_utra_scan_req(
        &p_son_start_inter_rat_scan_req->start_utra_scan_req,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_start_inter_rat_scan_req->bitmask & SON_GERAN_SCAN_REQ_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_start_geran_scan_req(
        &p_son_start_inter_rat_scan_req->start_geran_scan_req,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_start_inter_rat_scan_req->bitmask & SON_CDMA_SCAN_REQ_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_start_cdma_scan_req(
        &p_son_start_inter_rat_scan_req->start_cdma_scan_req,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_bulk_nw_scan_resp
(
    son_oam_bulk_nw_scan_resp_t *p_son_oam_bulk_nw_scan_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_bulk_nw_scan_resp, 0, sizeof(son_oam_bulk_nw_scan_resp_t));

    /* This function parses son_oam_bulk_nw_scan_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_bulk_nw_scan_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_bulk_nw_scan_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_bulk_nw_scan_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_bulk_nw_scan_resp->result] should be in range [%d <= 2] !", p_son_oam_bulk_nw_scan_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_bulk_nw_scan_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_bulk_nw_scan_resp(
        &p_son_oam_bulk_nw_scan_resp->nw_scan_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_rf_cell_num
(
    son_rf_cell_num_t *p_son_rf_cell_num,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_rf_cell_num, 0, sizeof(son_rf_cell_num_t));

    /* This function parses son_rf_cell_num */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_rf_cell_num");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_rf_cell_num->pci_id, p_src + *p_length_read, "pci_id");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rf_cell_num->pci_id > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rf_cell_num->pci_id] should be in range [%d <= 503] !", p_son_rf_cell_num->pci_id);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_rf_cell_num->rsrq, p_src + *p_length_read, "rsrq");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rf_cell_num->rsrq > 34))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rf_cell_num->rsrq] should be in range [%d <= 34] !", p_son_rf_cell_num->rsrq);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_rf_cell_num->rsrp, p_src + *p_length_read, "rsrp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rf_cell_num->rsrp > 128))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rf_cell_num->rsrp] should be in range [%d <= 128] !", p_son_rf_cell_num->rsrp);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_plmn_identity_info
(
    son_plmn_identity_info_t *p_son_plmn_identity_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_plmn_identity_info, 0, sizeof(son_plmn_identity_info_t));

    /* This function parses son_plmn_identity_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_plmn_identity_info");



    if (SON_FAILURE == son_parse_son_cell_plmn_info(
        &p_son_plmn_identity_info->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_plmn_identity_info->cell_res_for_operator_use, p_src + *p_length_read, "cell_res_for_operator_use");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_bcch_cell_num
(
    son_bcch_cell_num_t *p_son_bcch_cell_num,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_bcch_cell_num, 0, sizeof(son_bcch_cell_num_t));

    /* This function parses son_bcch_cell_num */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_bcch_cell_num");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_bcch_cell_num->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(p_son_bcch_cell_num->cell_Id) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_bcch_cell_num->cell_Id); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_bcch_cell_num->cell_Id[loop], (void*)(p_src + *p_length_read), "cell_Id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_bcch_cell_num->cell_barred, p_src + *p_length_read, "cell_barred");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_bcch_cell_num->csg_indication, p_src + *p_length_read, "csg_indication");
    *p_length_read += sizeof(U8);

    if (p_son_bcch_cell_num->bitmask & SON_CSG_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_bcch_cell_num->csg_identity) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_bcch_cell_num->csg_identity); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_bcch_cell_num->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_bcch_cell_num->bitmask & SON_BCCH_PLMN_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_bcch_cell_num->num_plmn_id, p_src + *p_length_read, "num_plmn_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_bcch_cell_num->num_plmn_id < 1) || (p_son_bcch_cell_num->num_plmn_id > SON_MAX_NUM_PLMN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_bcch_cell_num->num_plmn_id] should be in range [1 <= %d <= SON_MAX_NUM_PLMN] !", p_son_bcch_cell_num->num_plmn_id);
        return SON_FAILURE;
    }
    }

    if (p_son_bcch_cell_num->bitmask & SON_BCCH_PLMN_ID_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_bcch_cell_num->num_plmn_id; loop++)
        {
            if (SON_FAILURE == son_parse_son_plmn_identity_info(
                &p_son_bcch_cell_num->plmn_identity_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read + (S32)sizeof(p_son_bcch_cell_num->tac) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_bcch_cell_num->tac); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_bcch_cell_num->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_bcch_cell_num->rsTxPower, p_src + *p_length_read, "rsTxPower");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_bcch_cell_num->rsTxPower < -60) || (p_son_bcch_cell_num->rsTxPower > 50))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_bcch_cell_num->rsTxPower] should be in range [-60 <= %d <= 50] !", p_son_bcch_cell_num->rsTxPower);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_bcch_cell_num->dl_sys_bw, p_src + *p_length_read, "dl_sys_bw");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_bcch_cell_num->ul_sys_bw, p_src + *p_length_read, "ul_sys_bw");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cell_info
(
    son_cell_info_t *p_son_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_cell_info, 0, sizeof(son_cell_info_t));

    /* This function parses son_cell_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cell_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cell_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_rf_cell_num(
        &p_son_cell_info->rf_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_cell_info->bitmask & SON_BCCH_LIST_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_bcch_cell_num(
        &p_son_cell_info->bcch_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_rf_freq_channel_num
(
    son_rf_freq_channel_num_t *p_son_rf_freq_channel_num,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_rf_freq_channel_num, 0, sizeof(son_rf_freq_channel_num_t));

    /* This function parses son_rf_freq_channel_num */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_rf_freq_channel_num");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_rf_freq_channel_num->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_rf_freq_channel_num->rssi, p_src + *p_length_read, "rssi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rf_freq_channel_num->rssi > 128))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rf_freq_channel_num->rssi] should be in range [%d <= 128] !", p_son_rf_freq_channel_num->rssi);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_rf_freq_channel_num->cell_list_size, p_src + *p_length_read, "cell_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rf_freq_channel_num->cell_list_size < 1) || (p_son_rf_freq_channel_num->cell_list_size > SON_MAX_NO_CELLS_PER_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rf_freq_channel_num->cell_list_size] should be in range [1 <= %d <= SON_MAX_NO_CELLS_PER_EARFCN] !", p_son_rf_freq_channel_num->cell_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rf_freq_channel_num->cell_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_info(
                &p_son_rf_freq_channel_num->cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_start_intra_rat_bulk_scan_resp
(
    son_start_intra_rat_bulk_scan_resp_t *p_son_start_intra_rat_bulk_scan_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_start_intra_rat_bulk_scan_resp, 0, sizeof(son_start_intra_rat_bulk_scan_resp_t));

    /* This function parses son_start_intra_rat_bulk_scan_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_start_intra_rat_bulk_scan_resp");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_start_intra_rat_bulk_scan_resp->rf_freq_channel_list_size, p_src + *p_length_read, "rf_freq_channel_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_start_intra_rat_bulk_scan_resp->rf_freq_channel_list_size > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_start_intra_rat_bulk_scan_resp->rf_freq_channel_list_size] should be in range [%d <= 100] !", p_son_start_intra_rat_bulk_scan_resp->rf_freq_channel_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_start_intra_rat_bulk_scan_resp->rf_freq_channel_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_rf_freq_channel_num(
                &p_son_start_intra_rat_bulk_scan_resp->rf_freq_channel_num[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_start_inter_rat_bulk_scan_resp
(
    son_start_inter_rat_bulk_scan_resp_t *p_son_start_inter_rat_bulk_scan_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_start_inter_rat_bulk_scan_resp, 0, sizeof(son_start_inter_rat_bulk_scan_resp_t));

    /* This function parses son_start_inter_rat_bulk_scan_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_start_inter_rat_bulk_scan_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_start_inter_rat_bulk_scan_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_bulk_nw_scan_resp
(
    son_bulk_nw_scan_resp_t *p_son_bulk_nw_scan_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_bulk_nw_scan_resp, 0, sizeof(son_bulk_nw_scan_resp_t));

    /* This function parses son_bulk_nw_scan_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_bulk_nw_scan_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_bulk_nw_scan_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_bulk_nw_scan_resp->bitmask & SON_INTRA_RAT_BULK_NW_SCAN_RESP_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_start_intra_rat_bulk_scan_resp(
        &p_son_bulk_nw_scan_resp->intra_rat_scan_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_bulk_nw_scan_resp->bitmask & SON_INTER_RAT_BULK_NW_SCAN_RESP_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_start_inter_rat_bulk_scan_resp(
        &p_son_bulk_nw_scan_resp->inter_rat_scan_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_scan_req
(
    son_oam_nr_scan_req_t *p_son_oam_nr_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_scan_req, 0, sizeof(son_oam_nr_scan_req_t));

    /* This function parses son_oam_nr_scan_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_scan_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_scan_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_nr_scan_req->bitmask & SON_INTRA_RAT_NR_SCAN_REQ_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_start_intra_rat_scan_req(
        &p_son_oam_nr_scan_req->start_intra_rat_scan_req,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_oam_nr_scan_req->bitmask & SON_INTER_RAT_NR_SCAN_REQ_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_start_inter_rat_scan_req(
        &p_son_oam_nr_scan_req->start_inter_rat_scan_req,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_scan_resp
(
    son_oam_nr_scan_resp_t *p_son_oam_nr_scan_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_scan_resp, 0, sizeof(son_oam_nr_scan_resp_t));

    /* This function parses son_oam_nr_scan_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_scan_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_scan_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_scan_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_scan_resp->result] should be in range [%d <= 2] !", p_son_oam_nr_scan_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_scan_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_enable_req
(
    son_oam_anr_enable_req_t *p_son_oam_anr_enable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_enable_req, 0, sizeof(son_oam_anr_enable_req_t));

    /* This function parses son_oam_anr_enable_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_enable_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_enable_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_anr_enable_req->bitmask & SON_SRC_CGI_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_enable_req->src_cgi_list_size, p_src + *p_length_read, "src_cgi_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_enable_req->src_cgi_list_size < 1) || (p_son_oam_anr_enable_req->src_cgi_list_size > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_enable_req->src_cgi_list_size] should be in range [1 <= %d <= 1] !", p_son_oam_anr_enable_req->src_cgi_list_size);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_anr_enable_req->bitmask & SON_SRC_CGI_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_anr_enable_req->src_cgi_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
                &p_son_oam_anr_enable_req->src_cgi[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_son_oam_anr_enable_req->bitmask & SON_UE_CNT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_anr_enable_req->ue_cnt, p_src + *p_length_read, "ue_cnt");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_enable_req->ue_cnt < 1) || (p_son_oam_anr_enable_req->ue_cnt > 10))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_enable_req->ue_cnt] should be in range [1 <= %d <= 10] !", p_son_oam_anr_enable_req->ue_cnt);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_anr_enable_req->bitmask & SON_REPORTNG_INTERVAL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_anr_enable_req->reporting_interval, p_src + *p_length_read, "reporting_interval");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_enable_req->reporting_interval < 60) || (p_son_oam_anr_enable_req->reporting_interval > 1440))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_enable_req->reporting_interval] should be in range [60 <= %d <= 1440] !", p_son_oam_anr_enable_req->reporting_interval);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_anr_enable_req->bitmask & SON_LIMITED_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_enable_req->limited_mode, p_src + *p_length_read, "limited_mode");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_enable_req->limited_mode > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_enable_req->limited_mode] should be in range [%d <= 1] !", p_son_oam_anr_enable_req->limited_mode);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_anr_enable_req->bitmask & SON_REMOVAL_CRITERIA_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_neighbor_removal_info(
        &p_son_oam_anr_enable_req->removal_criteria,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_oam_anr_enable_req->bitmask & SON_PCI_CONFUSION_CFG_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_neighbor_pci_confusion_info(
        &p_son_oam_anr_enable_req->pci_confusion_cfg,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_oam_anr_enable_req->bitmask & SON_UE_THROUGHPUT_DL_ANR_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_enable_req->ue_throughput_dl_anr_threshold, p_src + *p_length_read, "ue_throughput_dl_anr_threshold");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_enable_req->ue_throughput_dl_anr_threshold < 1) || (p_son_oam_anr_enable_req->ue_throughput_dl_anr_threshold > 255))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_enable_req->ue_throughput_dl_anr_threshold] should be in range [1 <= %d <= 255] !", p_son_oam_anr_enable_req->ue_throughput_dl_anr_threshold);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_anr_enable_req->bitmask & SON_UE_THROUGHPUT_UL_ANR_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_enable_req->ue_throughput_ul_anr_threshold, p_src + *p_length_read, "ue_throughput_ul_anr_threshold");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_enable_req->ue_throughput_ul_anr_threshold < 1) || (p_son_oam_anr_enable_req->ue_throughput_ul_anr_threshold > 255))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_enable_req->ue_throughput_ul_anr_threshold] should be in range [1 <= %d <= 255] !", p_son_oam_anr_enable_req->ue_throughput_ul_anr_threshold);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_neighbor_removal_info
(
    son_neighbor_removal_info_t *p_son_neighbor_removal_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_neighbor_removal_info, 0, sizeof(son_neighbor_removal_info_t));

    /* This function parses son_neighbor_removal_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_neighbor_removal_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_neighbor_removal_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_neighbor_removal_info->bitmask & SON_NO_ACTIVITY_INTERVAL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_neighbor_removal_info->no_activity_interval, p_src + *p_length_read, "no_activity_interval");
    *p_length_read += sizeof(U16);
    }

    if (p_son_neighbor_removal_info->bitmask & SON_FAILED_HO_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_neighbor_removal_info->handover_failure_threshold, p_src + *p_length_read, "handover_failure_threshold");
    *p_length_read += sizeof(U8);
    }

    if (p_son_neighbor_removal_info->bitmask & SON_TIMEOUT_NO_NR_NEIGHBORS)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_neighbor_removal_info->timeout_no_nr_neighbors, p_src + *p_length_read, "timeout_no_nr_neighbors");
    *p_length_read += sizeof(U16);
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_neighbor_pci_confusion_info
(
    son_neighbor_pci_confusion_info_t *p_son_neighbor_pci_confusion_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_neighbor_pci_confusion_info, 0, sizeof(son_neighbor_pci_confusion_info_t));

    /* This function parses son_neighbor_pci_confusion_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_neighbor_pci_confusion_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_neighbor_pci_confusion_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_neighbor_pci_confusion_info->bitmask & SON_SUSPECT_PCI_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_neighbor_pci_confusion_info->suspect_pci_threshold, p_src + *p_length_read, "suspect_pci_threshold");
    *p_length_read += sizeof(U8);
    }

    if (p_son_neighbor_pci_confusion_info->bitmask & SON_CLEAR_SUSPECT_PCI_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_neighbor_pci_confusion_info->clear_suspect_pci_threshold, p_src + *p_length_read, "clear_suspect_pci_threshold");
    *p_length_read += sizeof(U8);
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_enable_resp
(
    son_oam_anr_enable_resp_t *p_son_oam_anr_enable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_enable_resp, 0, sizeof(son_oam_anr_enable_resp_t));

    /* This function parses son_oam_anr_enable_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_enable_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_enable_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_enable_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_enable_resp->result] should be in range [%d <= 2] !", p_son_oam_anr_enable_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_enable_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_anr_enable_resp->cell_status_list_size, p_src + *p_length_read, "cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_enable_resp->cell_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_enable_resp->cell_status_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_anr_enable_resp->cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_anr_enable_resp->cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_status(
                &p_son_oam_anr_enable_resp->cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cell_status
(
    son_cell_status_t *p_son_cell_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_cell_status, 0, sizeof(son_cell_status_t));

    /* This function parses son_cell_status */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cell_status");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_cell_status->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cell_status->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_disable_req
(
    son_oam_anr_disable_req_t *p_son_oam_anr_disable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_disable_req, 0, sizeof(son_oam_anr_disable_req_t));

    /* This function parses son_oam_anr_disable_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_disable_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_disable_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_anr_disable_req->bitmask & SON_ANR_DISABLE_SRC_CGI_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_disable_req->src_cgi_list_size, p_src + *p_length_read, "src_cgi_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_disable_req->src_cgi_list_size < 1) || (p_son_oam_anr_disable_req->src_cgi_list_size > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_disable_req->src_cgi_list_size] should be in range [1 <= %d <= 1] !", p_son_oam_anr_disable_req->src_cgi_list_size);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_anr_disable_req->bitmask & SON_ANR_DISABLE_SRC_CGI_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_anr_disable_req->src_cgi_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
                &p_son_oam_anr_disable_req->src_cgi[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_disable_resp
(
    son_oam_anr_disable_resp_t *p_son_oam_anr_disable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_disable_resp, 0, sizeof(son_oam_anr_disable_resp_t));

    /* This function parses son_oam_anr_disable_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_disable_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_disable_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_disable_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_disable_resp->result] should be in range [%d <= 2] !", p_son_oam_anr_disable_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_disable_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_anr_disable_resp->cell_status_list_size, p_src + *p_length_read, "cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_disable_resp->cell_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_disable_resp->cell_status_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_anr_disable_resp->cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_anr_disable_resp->cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_status(
                &p_son_oam_anr_disable_resp->cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_add_req
(
    son_oam_nr_add_req_t *p_son_oam_nr_add_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_add_req, 0, sizeof(son_oam_nr_add_req_t));

    /* This function parses son_oam_nr_add_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_add_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_nr_add_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_neighbor_info(
        &p_son_oam_nr_add_req->nr_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_neighbor_handover_stats
(
    son_neighbor_handover_stats_t *p_son_neighbor_handover_stats,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_neighbor_handover_stats, 0, sizeof(son_neighbor_handover_stats_t));

    /* This function parses son_neighbor_handover_stats */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_neighbor_handover_stats");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_neighbor_handover_stats->handovers_attempted, p_src + *p_length_read, "handovers_attempted");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_neighbor_handover_stats->handovers_succesful, p_src + *p_length_read, "handovers_succesful");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_neighbor_handover_stats->handovers_failed, p_src + *p_length_read, "handovers_failed");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_oam_eicic_info
(
    son_oam_eicic_info_t *p_son_oam_eicic_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_eicic_info, 0, sizeof(son_oam_eicic_info_t));

    /* This function parses son_oam_eicic_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_eicic_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_eicic_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_eicic_info->bitmask & SON_CELL_INTF_SCHEME_TYPE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_eicic_info->interference_scheme, p_src + *p_length_read, "interference_scheme");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_eicic_info->interference_scheme > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_eicic_info->interference_scheme] should be in range [%d <= 2] !", p_son_oam_eicic_info->interference_scheme);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_eicic_info->bitmask & SON_CELL_ABS_USAGE_THRESHOLD_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_oam_abs_threshold_usage(
        &p_son_oam_eicic_info->abs_usage_threshold,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_subframe_allocation
(
    son_subframe_allocation_t *p_son_subframe_allocation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_subframe_allocation, 0, sizeof(son_subframe_allocation_t));

    /* This function parses son_subframe_allocation */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_subframe_allocation");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_subframe_allocation->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_subframe_allocation->bitmask & SON_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_subframe_allocation->oneframe) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_subframe_allocation->oneframe); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_subframe_allocation->oneframe[loop], (void*)(p_src + *p_length_read), "oneframe[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_subframe_allocation->bitmask & SON_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_subframe_allocation->fourframe) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_subframe_allocation->fourframe); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_subframe_allocation->fourframe[loop], (void*)(p_src + *p_length_read), "fourframe[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_mbsfn_info
(
    son_mbsfn_info_t *p_son_mbsfn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_mbsfn_info, 0, sizeof(son_mbsfn_info_t));

    /* This function parses son_mbsfn_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_mbsfn_info");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_mbsfn_info->radioframe_alloc_period, p_src + *p_length_read, "radioframe_alloc_period");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mbsfn_info->radioframe_alloc_period > 5))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mbsfn_info->radioframe_alloc_period] should be in range [%d <= 5] !", p_son_mbsfn_info->radioframe_alloc_period);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_mbsfn_info->radioframe_alloc_offset, p_src + *p_length_read, "radioframe_alloc_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mbsfn_info->radioframe_alloc_offset > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mbsfn_info->radioframe_alloc_offset] should be in range [%d <= 7] !", p_son_mbsfn_info->radioframe_alloc_offset);
        return SON_FAILURE;
    }



    if (SON_FAILURE == son_parse_son_subframe_allocation(
        &p_son_mbsfn_info->subframe_alloc,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_mbms_configuration
(
    son_mbms_configuration_t *p_son_mbms_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_mbms_configuration, 0, sizeof(son_mbms_configuration_t));

    /* This function parses son_mbms_configuration */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_mbms_configuration");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_mbms_configuration->num_mbsfn, p_src + *p_length_read, "num_mbsfn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_mbms_configuration->num_mbsfn < 1) || (p_son_mbms_configuration->num_mbsfn > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mbms_configuration->num_mbsfn] should be in range [1 <= %d <= 8] !", p_son_mbms_configuration->num_mbsfn);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_mbms_configuration->num_mbsfn; loop++)
        {
            if (SON_FAILURE == son_parse_son_mbsfn_info(
                &p_son_mbms_configuration->mbsfn_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_intra_rat_neighbor_info
(
    son_intra_rat_neighbor_info_t *p_son_intra_rat_neighbor_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_intra_rat_neighbor_info, 0, sizeof(son_intra_rat_neighbor_info_t));

    /* This function parses son_intra_rat_neighbor_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_intra_rat_neighbor_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_intra_rat_neighbor_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_intra_rat_neighbor_info->nbr_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_intra_rat_neighbor_info->bitmask & SON_PCI_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_intra_rat_neighbor_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_intra_rat_neighbor_info->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->pci] should be in range [%d <= 503] !", p_son_intra_rat_neighbor_info->pci);
        return SON_FAILURE;
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_CELL_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_intra_rat_neighbor_info->cell_specific_offset, p_src + *p_length_read, "cell_specific_offset");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_intra_rat_neighbor_info->cell_specific_offset > 30))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->cell_specific_offset] should be in range [%d <= 30] !", p_son_intra_rat_neighbor_info->cell_specific_offset);
        return SON_FAILURE;
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_TAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_intra_rat_neighbor_info->tac) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_intra_rat_neighbor_info->tac); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_intra_rat_neighbor_info->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_PLMN_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_intra_rat_neighbor_info->num_plmn_id, p_src + *p_length_read, "num_plmn_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_intra_rat_neighbor_info->num_plmn_id < 1) || (p_son_intra_rat_neighbor_info->num_plmn_id > SON_NR_MAX_NUM_PLMN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->num_plmn_id] should be in range [1 <= %d <= SON_NR_MAX_NUM_PLMN] !", p_son_intra_rat_neighbor_info->num_plmn_id);
        return SON_FAILURE;
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_PLMN_ID_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_intra_rat_neighbor_info->num_plmn_id; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_plmn_info(
                &p_son_intra_rat_neighbor_info->plmn_id[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_INTRA_CSG_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_intra_rat_neighbor_info->csg_identity) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_intra_rat_neighbor_info->csg_identity); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_intra_rat_neighbor_info->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_EARFCN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_intra_rat_neighbor_info->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_RSRP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_intra_rat_neighbor_info->rsrp, p_src + *p_length_read, "rsrp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_intra_rat_neighbor_info->rsrp > 97))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->rsrp] should be in range [%d <= 97] !", p_son_intra_rat_neighbor_info->rsrp);
        return SON_FAILURE;
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_RSRQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_intra_rat_neighbor_info->rsrq, p_src + *p_length_read, "rsrq");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_intra_rat_neighbor_info->rsrq > 34))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->rsrq] should be in range [%d <= 34] !", p_son_intra_rat_neighbor_info->rsrq);
        return SON_FAILURE;
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_ENABLE_NR_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_intra_rat_neighbor_info->nr_status, p_src + *p_length_read, "nr_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_intra_rat_neighbor_info->nr_status < 1) || (p_son_intra_rat_neighbor_info->nr_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->nr_status] should be in range [1 <= %d <= 2] !", p_son_intra_rat_neighbor_info->nr_status);
        return SON_FAILURE;
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_ENABLE_HO_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_intra_rat_neighbor_info->ho_status, p_src + *p_length_read, "ho_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_intra_rat_neighbor_info->ho_status < 1) || (p_son_intra_rat_neighbor_info->ho_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->ho_status] should be in range [1 <= %d <= 2] !", p_son_intra_rat_neighbor_info->ho_status);
        return SON_FAILURE;
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_ENABLE_X2_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_intra_rat_neighbor_info->x2_status, p_src + *p_length_read, "x2_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_intra_rat_neighbor_info->x2_status < 3) || (p_son_intra_rat_neighbor_info->x2_status > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->x2_status] should be in range [3 <= %d <= 7] !", p_son_intra_rat_neighbor_info->x2_status);
        return SON_FAILURE;
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_EUTRAN_NR_HO_STATS_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_neighbor_handover_stats(
        &p_son_intra_rat_neighbor_info->handover_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_SUSPECT_PCI_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_intra_rat_neighbor_info->suspected_pci, p_src + *p_length_read, "suspected_pci");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_intra_rat_neighbor_info->suspected_pci > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->suspected_pci] should be in range [%d <= 1] !", p_son_intra_rat_neighbor_info->suspected_pci);
        return SON_FAILURE;
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_CM_CELL_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_intra_rat_neighbor_info->cm_cell_specific_offset, p_src + *p_length_read, "cm_cell_specific_offset");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_intra_rat_neighbor_info->cm_cell_specific_offset > 30))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->cm_cell_specific_offset] should be in range [%d <= 30] !", p_son_intra_rat_neighbor_info->cm_cell_specific_offset);
        return SON_FAILURE;
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_BROADCAST_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_intra_rat_neighbor_info->broadcast_status, p_src + *p_length_read, "broadcast_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_intra_rat_neighbor_info->broadcast_status > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->broadcast_status] should be in range [%d <= 1] !", p_son_intra_rat_neighbor_info->broadcast_status);
        return SON_FAILURE;
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_ACCESS_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_intra_rat_neighbor_info->access_mode, p_src + *p_length_read, "access_mode");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_intra_rat_neighbor_info->access_mode > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->access_mode] should be in range [%d <= 3] !", p_son_intra_rat_neighbor_info->access_mode);
        return SON_FAILURE;
    }
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_UL_EARFCN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_intra_rat_neighbor_info->ul_earfcn, p_src + *p_length_read, "ul_earfcn");
    *p_length_read += sizeof(U16);
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_PRACH_CONFIGURATION_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_prach_configuration(
        &p_son_intra_rat_neighbor_info->prach_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_EICIC_INFO_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_oam_eicic_info(
        &p_son_intra_rat_neighbor_info->eicic_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_EMBMS_MBMSFN_INFO_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_mbms_configuration(
        &p_son_intra_rat_neighbor_info->mbms_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_intra_rat_neighbor_info->bitmask & SON_EMBMS_TDD_SF_ASSN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_intra_rat_neighbor_info->subframe_assn, p_src + *p_length_read, "subframe_assn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_intra_rat_neighbor_info->subframe_assn > 6))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_intra_rat_neighbor_info->subframe_assn] should be in range [%d <= 6] !", p_son_intra_rat_neighbor_info->subframe_assn);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_utran_geran_global_cell_id
(
    son_utran_geran_global_cell_id_t *p_son_utran_geran_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_utran_geran_global_cell_id, 0, sizeof(son_utran_geran_global_cell_id_t));

    /* This function parses son_utran_geran_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_utran_geran_global_cell_id");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_utran_geran_global_cell_id->rat_type, p_src + *p_length_read, "rat_type");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_cell_plmn_info(
        &p_son_utran_geran_global_cell_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_son_utran_geran_global_cell_id->cell_identity) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_utran_geran_global_cell_id->cell_identity); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_utran_geran_global_cell_id->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_utran_physical_cell_id
(
    son_utran_physical_cell_id_t *p_son_utran_physical_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_utran_physical_cell_id, 0, sizeof(son_utran_physical_cell_id_t));

    /* This function parses son_utran_physical_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_utran_physical_cell_id");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_utran_physical_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_utran_physical_cell_id->bitmask & SON_PCI_FDD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_utran_physical_cell_id->pci_fdd, p_src + *p_length_read, "pci_fdd");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_utran_physical_cell_id->pci_fdd > 511))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_physical_cell_id->pci_fdd] should be in range [%d <= 511] !", p_son_utran_physical_cell_id->pci_fdd);
        return SON_FAILURE;
    }
    }

    if (p_son_utran_physical_cell_id->bitmask & SON_PCI_TDD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_utran_physical_cell_id->pci_tdd, p_src + *p_length_read, "pci_tdd");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_utran_physical_cell_id->pci_tdd > 127))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_physical_cell_id->pci_tdd] should be in range [%d <= 127] !", p_son_utran_physical_cell_id->pci_tdd);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_utran_neighbor_info
(
    son_utran_neighbor_info_t *p_son_utran_neighbor_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_utran_neighbor_info, 0, sizeof(son_utran_neighbor_info_t));

    /* This function parses son_utran_neighbor_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_utran_neighbor_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_utran_neighbor_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_utran_geran_global_cell_id(
        &p_son_utran_neighbor_info->nbr_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_PCI_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_utran_physical_cell_id(
        &p_son_utran_neighbor_info->pci,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_CSO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_utran_neighbor_info->cell_specific_offset, p_src + *p_length_read, "cell_specific_offset");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_utran_neighbor_info->cell_specific_offset < -15) || (p_son_utran_neighbor_info->cell_specific_offset > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_neighbor_info->cell_specific_offset] should be in range [-15 <= %d <= 15] !", p_son_utran_neighbor_info->cell_specific_offset);
        return SON_FAILURE;
    }
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_LAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_utran_neighbor_info->lac) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_utran_neighbor_info->lac); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_utran_neighbor_info->lac[loop], (void*)(p_src + *p_length_read), "lac[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_RAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_utran_neighbor_info->rac, p_src + *p_length_read, "rac");
    *p_length_read += sizeof(U8);
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_PLMN_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_utran_neighbor_info->num_plmn_id, p_src + *p_length_read, "num_plmn_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_utran_neighbor_info->num_plmn_id < 1) || (p_son_utran_neighbor_info->num_plmn_id > SON_NR_MAX_NUM_PLMN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_neighbor_info->num_plmn_id] should be in range [1 <= %d <= SON_NR_MAX_NUM_PLMN] !", p_son_utran_neighbor_info->num_plmn_id);
        return SON_FAILURE;
    }
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_PLMN_ID_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_utran_neighbor_info->num_plmn_id; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_plmn_info(
                &p_son_utran_neighbor_info->plmn_id[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_CSG_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_utran_neighbor_info->csg_identity) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_utran_neighbor_info->csg_identity); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_utran_neighbor_info->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_UARFCN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_utran_neighbor_info->uarfcn, p_src + *p_length_read, "uarfcn");
    *p_length_read += sizeof(U16);
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_RSCP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_utran_neighbor_info->rscp, p_src + *p_length_read, "rscp");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_utran_neighbor_info->rscp < -5) || (p_son_utran_neighbor_info->rscp > 91))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_neighbor_info->rscp] should be in range [-5 <= %d <= 91] !", p_son_utran_neighbor_info->rscp);
        return SON_FAILURE;
    }
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_ECNO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_utran_neighbor_info->ecno, p_src + *p_length_read, "ecno");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_utran_neighbor_info->ecno > 49))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_neighbor_info->ecno] should be in range [%d <= 49] !", p_son_utran_neighbor_info->ecno);
        return SON_FAILURE;
    }
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_ENABLE_NR_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_utran_neighbor_info->nr_status, p_src + *p_length_read, "nr_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_utran_neighbor_info->nr_status < 1) || (p_son_utran_neighbor_info->nr_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_neighbor_info->nr_status] should be in range [1 <= %d <= 2] !", p_son_utran_neighbor_info->nr_status);
        return SON_FAILURE;
    }
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_ENABLE_HO_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_utran_neighbor_info->ho_status, p_src + *p_length_read, "ho_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_utran_neighbor_info->ho_status < 1) || (p_son_utran_neighbor_info->ho_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_neighbor_info->ho_status] should be in range [1 <= %d <= 2] !", p_son_utran_neighbor_info->ho_status);
        return SON_FAILURE;
    }
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_NR_HO_STATS_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_neighbor_handover_stats(
        &p_son_utran_neighbor_info->handover_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_ACCESS_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_utran_neighbor_info->access_mode, p_src + *p_length_read, "access_mode");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_utran_neighbor_info->access_mode > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_neighbor_info->access_mode] should be in range [%d <= 3] !", p_son_utran_neighbor_info->access_mode);
        return SON_FAILURE;
    }
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_PS_HO_SUPPORT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_utran_neighbor_info->ps_ho_supported, p_src + *p_length_read, "ps_ho_supported");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_utran_neighbor_info->ps_ho_supported > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_neighbor_info->ps_ho_supported] should be in range [%d <= 1] !", p_son_utran_neighbor_info->ps_ho_supported);
        return SON_FAILURE;
    }
    }

    if (p_son_utran_neighbor_info->bitmask & SON_UTRAN_VOIP_CAPABLE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_utran_neighbor_info->voip_capable, p_src + *p_length_read, "voip_capable");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_utran_neighbor_info->voip_capable > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_neighbor_info->voip_capable] should be in range [%d <= 1] !", p_son_utran_neighbor_info->voip_capable);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_geran_physical_cell_id
(
    son_geran_physical_cell_id_t *p_son_geran_physical_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_geran_physical_cell_id, 0, sizeof(son_geran_physical_cell_id_t));

    /* This function parses son_geran_physical_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_geran_physical_cell_id");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_physical_cell_id->ncc, p_src + *p_length_read, "ncc");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_physical_cell_id->ncc > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_physical_cell_id->ncc] should be in range [%d <= 7] !", p_son_geran_physical_cell_id->ncc);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_physical_cell_id->bscc, p_src + *p_length_read, "bscc");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_physical_cell_id->bscc > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_physical_cell_id->bscc] should be in range [%d <= 7] !", p_son_geran_physical_cell_id->bscc);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_geran_neighbor_info
(
    son_geran_neighbor_info_t *p_son_geran_neighbor_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_geran_neighbor_info, 0, sizeof(son_geran_neighbor_info_t));

    /* This function parses son_geran_neighbor_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_geran_neighbor_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_geran_neighbor_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_utran_geran_global_cell_id(
        &p_son_geran_neighbor_info->nbr_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_geran_neighbor_info->bitmask & SON_GERAN_PCI_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_geran_physical_cell_id(
        &p_son_geran_neighbor_info->pci,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_geran_neighbor_info->bitmask & SON_GERAN_CSO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_geran_neighbor_info->cell_specific_offset, p_src + *p_length_read, "cell_specific_offset");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_geran_neighbor_info->cell_specific_offset < -15) || (p_son_geran_neighbor_info->cell_specific_offset > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_neighbor_info->cell_specific_offset] should be in range [-15 <= %d <= 15] !", p_son_geran_neighbor_info->cell_specific_offset);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_neighbor_info->bitmask & SON_GERAN_RAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_neighbor_info->rac, p_src + *p_length_read, "rac");
    *p_length_read += sizeof(U8);
    }

    if (p_son_geran_neighbor_info->bitmask & SON_GERAN_ARFCN_BAND_IND_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_geran_neighbor_info->arfcn, p_src + *p_length_read, "arfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_neighbor_info->arfcn > 1023))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_neighbor_info->arfcn] should be in range [%d <= 1023] !", p_son_geran_neighbor_info->arfcn);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_neighbor_info->bitmask & SON_GERAN_ARFCN_BAND_IND_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_geran_neighbor_info->band_ind, p_src + *p_length_read, "band_ind");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_neighbor_info->band_ind > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_neighbor_info->band_ind] should be in range [%d <= 1] !", p_son_geran_neighbor_info->band_ind);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_neighbor_info->bitmask & SON_GERAN_RSSI_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_neighbor_info->rssi, p_src + *p_length_read, "rssi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_neighbor_info->rssi > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_neighbor_info->rssi] should be in range [%d <= 63] !", p_son_geran_neighbor_info->rssi);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_neighbor_info->bitmask & SON_GERAN_ENABLE_NR_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_geran_neighbor_info->nr_status, p_src + *p_length_read, "nr_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_geran_neighbor_info->nr_status < 1) || (p_son_geran_neighbor_info->nr_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_neighbor_info->nr_status] should be in range [1 <= %d <= 2] !", p_son_geran_neighbor_info->nr_status);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_neighbor_info->bitmask & SON_GERAN_ENABLE_HO_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_geran_neighbor_info->ho_status, p_src + *p_length_read, "ho_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_geran_neighbor_info->ho_status < 1) || (p_son_geran_neighbor_info->ho_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_neighbor_info->ho_status] should be in range [1 <= %d <= 2] !", p_son_geran_neighbor_info->ho_status);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_neighbor_info->bitmask & SON_GERAN_NR_HO_STATS_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_neighbor_handover_stats(
        &p_son_geran_neighbor_info->handover_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_geran_neighbor_info->bitmask & SON_GERAN_DTM_SUPPORT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_neighbor_info->dtm_supported, p_src + *p_length_read, "dtm_supported");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_neighbor_info->dtm_supported > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_neighbor_info->dtm_supported] should be in range [%d <= 1] !", p_son_geran_neighbor_info->dtm_supported);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cdma_global_cell_id
(
    son_cdma_global_cell_id_t *p_son_cdma_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_cdma_global_cell_id, 0, sizeof(son_cdma_global_cell_id_t));

    /* This function parses son_cdma_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cdma_global_cell_id");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_global_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_cdma_global_cell_id->bitmask & SON_CGI_1X_RTT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_cdma_global_cell_id->cell_global_id_1X_RTT) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_cdma_global_cell_id->cell_global_id_1X_RTT); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_cdma_global_cell_id->cell_global_id_1X_RTT[loop], (void*)(p_src + *p_length_read), "cell_global_id_1X_RTT[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_cdma_global_cell_id->bitmask & SON_CGI_HRPD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_cdma_global_cell_id->cell_global_id_HRPD) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_cdma_global_cell_id->cell_global_id_HRPD); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_cdma_global_cell_id->cell_global_id_HRPD[loop], (void*)(p_src + *p_length_read), "cell_global_id_HRPD[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_oam_pz_hyst_params_included
(
    son_oam_pz_hyst_params_included_t *p_son_oam_pz_hyst_params_included,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_pz_hyst_params_included, 0, sizeof(son_oam_pz_hyst_params_included_t));

    /* This function parses son_oam_pz_hyst_params_included */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_pz_hyst_params_included");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_pz_hyst_params_included->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_pz_hyst_params_included->bitmask & SON_OAM_PZ_HYST_ENABLED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_pz_hyst_params_included->pz_hyst_enabled, p_src + *p_length_read, "pz_hyst_enabled");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pz_hyst_params_included->pz_hyst_enabled > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pz_hyst_params_included->pz_hyst_enabled] should be in range [%d <= 1] !", p_son_oam_pz_hyst_params_included->pz_hyst_enabled);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_pz_hyst_params_included->bitmask & SON_OAM_PZ_HYST_INFO_INCL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_pz_hyst_params_included->pz_hyst_info_incl, p_src + *p_length_read, "pz_hyst_info_incl");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pz_hyst_params_included->pz_hyst_info_incl > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pz_hyst_params_included->pz_hyst_info_incl] should be in range [%d <= 1] !", p_son_oam_pz_hyst_params_included->pz_hyst_info_incl);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_pz_hyst_params_included->bitmask & SON_OAM_PZ_HYST_TIMER_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_pz_hyst_params_included->pz_hyst_act_timer, p_src + *p_length_read, "pz_hyst_act_timer");
    *p_length_read += sizeof(U8);
    }

    if (p_son_oam_pz_hyst_params_included->bitmask & SON_OAM_PZ_HYST_TIMER_MUL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_pz_hyst_params_included->pz_hyst_timer_mul, p_src + *p_length_read, "pz_hyst_timer_mul");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pz_hyst_params_included->pz_hyst_timer_mul > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pz_hyst_params_included->pz_hyst_timer_mul] should be in range [%d <= 7] !", p_son_oam_pz_hyst_params_included->pz_hyst_timer_mul);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_pz_hyst_params_included->bitmask & SON_OAM_PZ_HYST_TIMER_EXP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_pz_hyst_params_included->pz_hyst_timer_exp, p_src + *p_length_read, "pz_hyst_timer_exp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pz_hyst_params_included->pz_hyst_timer_exp > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pz_hyst_params_included->pz_hyst_timer_exp] should be in range [%d <= 31] !", p_son_oam_pz_hyst_params_included->pz_hyst_timer_exp);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_pz_hyst_params_included->bitmask & SON_OAM_PZ_HYST_LIST_LEN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_pz_hyst_params_included->pz_hyst_list_len, p_src + *p_length_read, "pz_hyst_list_len");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pz_hyst_params_included->pz_hyst_list_len > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pz_hyst_params_included->pz_hyst_list_len] should be in range [%d <= 1] !", p_son_oam_pz_hyst_params_included->pz_hyst_list_len);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_oam_fpc_fch_included
(
    son_oam_fpc_fch_included_t *p_son_oam_fpc_fch_included,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_fpc_fch_included, 0, sizeof(son_oam_fpc_fch_included_t));

    /* This function parses son_oam_fpc_fch_included */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_fpc_fch_included");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_fpc_fch_included->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_fpc_fch_included->bitmask & SON_OAM_FPC_FCH_INCL_RC3_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_fpc_fch_included->fpc_fch_init_setpt_rc3, p_src + *p_length_read, "fpc_fch_init_setpt_rc3");
    *p_length_read += sizeof(U8);
    }

    if (p_son_oam_fpc_fch_included->bitmask & SON_OAM_FPC_FCH_INCL_RC4_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_fpc_fch_included->fpc_fch_init_setpt_rc4, p_src + *p_length_read, "fpc_fch_init_setpt_rc4");
    *p_length_read += sizeof(U8);
    }

    if (p_son_oam_fpc_fch_included->bitmask & SON_OAM_FPC_FCH_INCL_RC5_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_fpc_fch_included->fpc_fch_init_setpt_rc5, p_src + *p_length_read, "fpc_fch_init_setpt_rc5");
    *p_length_read += sizeof(U8);
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cdma_cell_specific_params
(
    son_cdma_cell_specific_params_t *p_son_cdma_cell_specific_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_cdma_cell_specific_params, 0, sizeof(son_cdma_cell_specific_params_t));

    /* This function parses son_cdma_cell_specific_params */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cdma_cell_specific_params");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_cell_specific_params->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_PZ_HYST_PARAMS_INCLUDED_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_oam_pz_hyst_params_included(
        &p_son_cdma_cell_specific_params->pz_hyst_params_included,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_P_REV_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->p_rev, p_src + *p_length_read, "p_rev");
    *p_length_read += sizeof(U8);
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_MIN_P_REV_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->min_p_rev, p_src + *p_length_read, "min_p_rev");
    *p_length_read += sizeof(U8);
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_NEG_SLOT_CYCLE_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->neg_slot_cycle_index_sup, p_src + *p_length_read, "neg_slot_cycle_index_sup");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_cell_specific_params->neg_slot_cycle_index_sup > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_cell_specific_params->neg_slot_cycle_index_sup] should be in range [%d <= 1] !", p_son_cdma_cell_specific_params->neg_slot_cycle_index_sup);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_ENCRYPT_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->encrypt_mode, p_src + *p_length_read, "encrypt_mode");
    *p_length_read += sizeof(U8);
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_ENCRYPT_SUPPORTED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->encrypt_suported, p_src + *p_length_read, "encrypt_suported");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_cell_specific_params->encrypt_suported > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_cell_specific_params->encrypt_suported] should be in range [%d <= 1] !", p_son_cdma_cell_specific_params->encrypt_suported);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_SIG_ENCRYPT_SUPPORTED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->sig_encrypt_suported, p_src + *p_length_read, "sig_encrypt_suported");
    *p_length_read += sizeof(U8);
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_MSG_INT_SUPPORTED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->msg_integrity_sup, p_src + *p_length_read, "msg_integrity_sup");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_cell_specific_params->msg_integrity_sup > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_cell_specific_params->msg_integrity_sup] should be in range [%d <= 1] !", p_son_cdma_cell_specific_params->msg_integrity_sup);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_SIG_INT_SUPPORTED_INCL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->sig_integrity_sup_incl, p_src + *p_length_read, "sig_integrity_sup_incl");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_cell_specific_params->sig_integrity_sup_incl > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_cell_specific_params->sig_integrity_sup_incl] should be in range [%d <= 1] !", p_son_cdma_cell_specific_params->sig_integrity_sup_incl);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_SIG_INT_SUPPORTED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->sig_integrity_sup, p_src + *p_length_read, "sig_integrity_sup");
    *p_length_read += sizeof(U8);
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_MS_INIT_POS_LOC_SUP_IND_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->ms_init_pos_loc_sup_ind, p_src + *p_length_read, "ms_init_pos_loc_sup_ind");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_cell_specific_params->ms_init_pos_loc_sup_ind > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_cell_specific_params->ms_init_pos_loc_sup_ind] should be in range [%d <= 1] !", p_son_cdma_cell_specific_params->ms_init_pos_loc_sup_ind);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_BAND_CLASS_INFO_REQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->band_class_info_req, p_src + *p_length_read, "band_class_info_req");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_cell_specific_params->band_class_info_req > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_cell_specific_params->band_class_info_req] should be in range [%d <= 1] !", p_son_cdma_cell_specific_params->band_class_info_req);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_BAND_CLASS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_cell_specific_params->band_class, p_src + *p_length_read, "band_class");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_cell_specific_params->band_class > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_cell_specific_params->band_class] should be in range [%d <= 31] !", p_son_cdma_cell_specific_params->band_class);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_ALT_BAND_CLASS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_cell_specific_params->alt_band_class, p_src + *p_length_read, "alt_band_class");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_cell_specific_params->alt_band_class > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_cell_specific_params->alt_band_class] should be in range [%d <= 31] !", p_son_cdma_cell_specific_params->alt_band_class);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_TKZ_MODE_SUP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->tkz_mode_supported, p_src + *p_length_read, "tkz_mode_supported");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_cell_specific_params->tkz_mode_supported > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_cell_specific_params->tkz_mode_supported] should be in range [%d <= 1] !", p_son_cdma_cell_specific_params->tkz_mode_supported);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_TKZ_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->tkz_id, p_src + *p_length_read, "tkz_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_cell_specific_params->tkz_id > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_cell_specific_params->tkz_id] should be in range [%d <= 1] !", p_son_cdma_cell_specific_params->tkz_id);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_FPC_FCH_INCLUDED)
    {



    if (SON_FAILURE == son_parse_son_oam_fpc_fch_included(
        &p_son_cdma_cell_specific_params->fpc_fch_included,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_T_ADD)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->t_add, p_src + *p_length_read, "t_add");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_cell_specific_params->t_add > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_cell_specific_params->t_add] should be in range [%d <= 63] !", p_son_cdma_cell_specific_params->t_add);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_cell_specific_params->bitmask & SON_CDMA_CSP_PILOT_INC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_cell_specific_params->pilot_inc, p_src + *p_length_read, "pilot_inc");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_cell_specific_params->pilot_inc > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_cell_specific_params->pilot_inc] should be in range [%d <= 15] !", p_son_cdma_cell_specific_params->pilot_inc);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cdma_neighbor_info
(
    son_cdma_neighbor_info_t *p_son_cdma_neighbor_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_cdma_neighbor_info, 0, sizeof(son_cdma_neighbor_info_t));

    /* This function parses son_cdma_neighbor_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cdma_neighbor_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_neighbor_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_cdma_global_cell_id(
        &p_son_cdma_neighbor_info->nbr_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_cdma_neighbor_info->bitmask & SON_CDMA_PCI_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_cdma_neighbor_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_neighbor_info->pci > 511))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_neighbor_info->pci] should be in range [%d <= 511] !", p_son_cdma_neighbor_info->pci);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_neighbor_info->bitmask & SON_CDMA_BAND_CLASS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_neighbor_info->band_class, p_src + *p_length_read, "band_class");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_neighbor_info->band_class > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_neighbor_info->band_class] should be in range [%d <= 31] !", p_son_cdma_neighbor_info->band_class);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_neighbor_info->bitmask & SON_CDMA_BAND_CLASS_ARFCN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_cdma_neighbor_info->arfcn, p_src + *p_length_read, "arfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_neighbor_info->arfcn > 2047))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_neighbor_info->arfcn] should be in range [%d <= 2047] !", p_son_cdma_neighbor_info->arfcn);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_neighbor_info->bitmask & SON_CDMA_CSO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_cdma_neighbor_info->cell_specific_offset, p_src + *p_length_read, "cell_specific_offset");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_cdma_neighbor_info->cell_specific_offset < -15) || (p_son_cdma_neighbor_info->cell_specific_offset > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_neighbor_info->cell_specific_offset] should be in range [-15 <= %d <= 15] !", p_son_cdma_neighbor_info->cell_specific_offset);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_neighbor_info->bitmask & SON_CDMA_PILOT_PN_PHASE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_cdma_neighbor_info->pilot_pn_phase, p_src + *p_length_read, "pilot_pn_phase");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_neighbor_info->pilot_pn_phase > 32767))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_neighbor_info->pilot_pn_phase] should be in range [%d <= 32767] !", p_son_cdma_neighbor_info->pilot_pn_phase);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_neighbor_info->bitmask & SON_CDMA_PILOT_STRENGTH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_neighbor_info->pilot_strength, p_src + *p_length_read, "pilot_strength");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_neighbor_info->pilot_strength > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_neighbor_info->pilot_strength] should be in range [%d <= 63] !", p_son_cdma_neighbor_info->pilot_strength);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_neighbor_info->bitmask & SON_CDMA_ENABLE_NR_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_neighbor_info->nr_status, p_src + *p_length_read, "nr_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_cdma_neighbor_info->nr_status < 1) || (p_son_cdma_neighbor_info->nr_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_neighbor_info->nr_status] should be in range [1 <= %d <= 2] !", p_son_cdma_neighbor_info->nr_status);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_neighbor_info->bitmask & SON_CDMA_ENABLE_HO_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_neighbor_info->ho_status, p_src + *p_length_read, "ho_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_cdma_neighbor_info->ho_status < 1) || (p_son_cdma_neighbor_info->ho_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_neighbor_info->ho_status] should be in range [1 <= %d <= 2] !", p_son_cdma_neighbor_info->ho_status);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_neighbor_info->bitmask & SON_CDMA_NR_HO_STATS_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_neighbor_handover_stats(
        &p_son_cdma_neighbor_info->handover_stats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_cdma_neighbor_info->bitmask & SON_CDMA_BROADCAST_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_neighbor_info->broadcast_status, p_src + *p_length_read, "broadcast_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_neighbor_info->broadcast_status > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_neighbor_info->broadcast_status] should be in range [%d <= 1] !", p_son_cdma_neighbor_info->broadcast_status);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_neighbor_info->bitmask & SON_CDMA_CELL_SPECIFIC_PARAMS_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_cdma_cell_specific_params(
        &p_son_cdma_neighbor_info->cell_specific_params,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_inter_rat_neighbor_info
(
    son_inter_rat_neighbor_info_t *p_son_inter_rat_neighbor_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_inter_rat_neighbor_info, 0, sizeof(son_inter_rat_neighbor_info_t));

    /* This function parses son_inter_rat_neighbor_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_inter_rat_neighbor_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_inter_rat_neighbor_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_inter_rat_neighbor_info->bitmask & SON_INTER_UTRAN_NEIGHBOR)
    {



    if (SON_FAILURE == son_parse_son_utran_neighbor_info(
        &p_son_inter_rat_neighbor_info->utran_neighbor,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_inter_rat_neighbor_info->bitmask & SON_INTER_GERAN_NEIGHBOR)
    {



    if (SON_FAILURE == son_parse_son_geran_neighbor_info(
        &p_son_inter_rat_neighbor_info->geran_neighbor,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_inter_rat_neighbor_info->bitmask & SON_INTER_CDMA_NEIGHBOR)
    {



    if (SON_FAILURE == son_parse_son_cdma_neighbor_info(
        &p_son_inter_rat_neighbor_info->cdma_neighbor,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_neighbor_info
(
    son_neighbor_info_t *p_son_neighbor_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_neighbor_info, 0, sizeof(son_neighbor_info_t));

    /* This function parses son_neighbor_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_neighbor_info");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_neighbor_info->intra_rat_nr_list_size, p_src + *p_length_read, "intra_rat_nr_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_neighbor_info->intra_rat_nr_list_size > SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_neighbor_info->intra_rat_nr_list_size] should be in range [%d <= SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS] !", p_son_neighbor_info->intra_rat_nr_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_neighbor_info->intra_rat_nr_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_intra_rat_neighbor_info(
                &p_son_neighbor_info->intra_rat_nr_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_neighbor_info->inter_rat_nr_list_size, p_src + *p_length_read, "inter_rat_nr_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_neighbor_info->inter_rat_nr_list_size > SON_MAX_NO_INTER_RAT_NEIGHBOUR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_neighbor_info->inter_rat_nr_list_size] should be in range [%d <= SON_MAX_NO_INTER_RAT_NEIGHBOUR_CELLS] !", p_son_neighbor_info->inter_rat_nr_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_neighbor_info->inter_rat_nr_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_inter_rat_neighbor_info(
                &p_son_neighbor_info->inter_rat_nr_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_add_resp
(
    son_oam_nr_add_resp_t *p_son_oam_nr_add_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_add_resp, 0, sizeof(son_oam_nr_add_resp_t));

    /* This function parses son_oam_nr_add_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_add_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_nr_add_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_add_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_add_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_add_resp->result] should be in range [%d <= 2] !", p_son_oam_nr_add_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_add_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_nr_add_resp->trgt_cell_status_list_size, p_src + *p_length_read, "trgt_cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_add_resp->trgt_cell_status_list_size > SON_MAX_NO_NEIGHBOUR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_add_resp->trgt_cell_status_list_size] should be in range [%d <= SON_MAX_NO_NEIGHBOUR_CELLS] !", p_son_oam_nr_add_resp->trgt_cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_add_resp->trgt_cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_nbr_cell_status(
                &p_son_oam_nr_add_resp->trgt_cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_others_global_cell_id
(
    son_others_global_cell_id_t *p_son_others_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_others_global_cell_id, 0, sizeof(son_others_global_cell_id_t));

    /* This function parses son_others_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_others_global_cell_id");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_others_global_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_inter_rat_global_cell_id
(
    son_inter_rat_global_cell_id_t *p_son_inter_rat_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_inter_rat_global_cell_id, 0, sizeof(son_inter_rat_global_cell_id_t));

    /* This function parses son_inter_rat_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_inter_rat_global_cell_id");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_inter_rat_global_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_inter_rat_global_cell_id->bitmask & SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_utran_geran_global_cell_id(
        &p_son_inter_rat_global_cell_id->utran_geran_global_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_inter_rat_global_cell_id->bitmask & SON_OTHERS_GLOBAL_CELL_ID_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_others_global_cell_id(
        &p_son_inter_rat_global_cell_id->others_global_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_inter_rat_global_cell_id->bitmask & SON_CDMA_GLOBAL_CELL_ID_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_cdma_global_cell_id(
        &p_son_inter_rat_global_cell_id->cdma_global_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_global_cell_id
(
    son_global_cell_id_t *p_son_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_global_cell_id, 0, sizeof(son_global_cell_id_t));

    /* This function parses son_global_cell_id */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_global_cell_id");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_global_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_global_cell_id->bitmask & SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_global_cell_id->intra_rat_global_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_global_cell_id->bitmask & SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_inter_rat_global_cell_id(
        &p_son_global_cell_id->inter_rat_global_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_nbr_cell_status
(
    son_nbr_cell_status_t *p_son_nbr_cell_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_nbr_cell_status, 0, sizeof(son_nbr_cell_status_t));

    /* This function parses son_nbr_cell_status */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_nbr_cell_status");



    if (SON_FAILURE == son_parse_son_global_cell_id(
        &p_son_nbr_cell_status->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_nbr_cell_status->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_update_req
(
    son_oam_nr_update_req_t *p_son_oam_nr_update_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_update_req, 0, sizeof(son_oam_nr_update_req_t));

    /* This function parses son_oam_nr_update_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_update_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_nr_update_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_neighbor_info(
        &p_son_oam_nr_update_req->nr_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_update_resp
(
    son_oam_nr_update_resp_t *p_son_oam_nr_update_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_update_resp, 0, sizeof(son_oam_nr_update_resp_t));

    /* This function parses son_oam_nr_update_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_update_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_nr_update_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_update_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_update_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_update_resp->result] should be in range [%d <= 2] !", p_son_oam_nr_update_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_update_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_nr_update_resp->trgt_cell_status_list_size, p_src + *p_length_read, "trgt_cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_update_resp->trgt_cell_status_list_size > SON_MAX_NO_NEIGHBOUR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_update_resp->trgt_cell_status_list_size] should be in range [%d <= SON_MAX_NO_NEIGHBOUR_CELLS] !", p_son_oam_nr_update_resp->trgt_cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_update_resp->trgt_cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_nbr_cell_status(
                &p_son_oam_nr_update_resp->trgt_cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_delete_req
(
    son_oam_nr_delete_req_t *p_son_oam_nr_delete_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_delete_req, 0, sizeof(son_oam_nr_delete_req_t));

    /* This function parses son_oam_nr_delete_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_delete_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_nr_delete_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_nr_delete_req->trgt_cgi_list_size, p_src + *p_length_read, "trgt_cgi_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_delete_req->trgt_cgi_list_size > SON_MAX_NO_NEIGHBOUR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_delete_req->trgt_cgi_list_size] should be in range [%d <= SON_MAX_NO_NEIGHBOUR_CELLS] !", p_son_oam_nr_delete_req->trgt_cgi_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_delete_req->trgt_cgi_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_global_cell_id(
                &p_son_oam_nr_delete_req->trgt_cgi_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_delete_resp
(
    son_oam_nr_delete_resp_t *p_son_oam_nr_delete_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_delete_resp, 0, sizeof(son_oam_nr_delete_resp_t));

    /* This function parses son_oam_nr_delete_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_delete_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_nr_delete_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_delete_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_delete_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_delete_resp->result] should be in range [%d <= 2] !", p_son_oam_nr_delete_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_delete_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_nr_delete_resp->trgt_cell_status_list_size, p_src + *p_length_read, "trgt_cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_delete_resp->trgt_cell_status_list_size > SON_MAX_NO_NEIGHBOUR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_delete_resp->trgt_cell_status_list_size] should be in range [%d <= SON_MAX_NO_NEIGHBOUR_CELLS] !", p_son_oam_nr_delete_resp->trgt_cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_delete_resp->trgt_cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_nbr_cell_status(
                &p_son_oam_nr_delete_resp->trgt_cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_retrieve_req
(
    son_oam_nr_retrieve_req_t *p_son_oam_nr_retrieve_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_retrieve_req, 0, sizeof(son_oam_nr_retrieve_req_t));

    /* This function parses son_oam_nr_retrieve_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_retrieve_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_retrieve_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_nr_retrieve_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_oam_nr_retrieve_req->bitmask & SON_RETRIEVE_TRGT_CGI_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_global_cell_id(
        &p_son_oam_nr_retrieve_req->trgt_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_oam_nr_retrieve_req->bitmask & SON_RETRIEVE_NR_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_retrieve_req->nr_status, p_src + *p_length_read, "nr_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_nr_retrieve_req->nr_status < 1) || (p_son_oam_nr_retrieve_req->nr_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_retrieve_req->nr_status] should be in range [1 <= %d <= 2] !", p_son_oam_nr_retrieve_req->nr_status);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_nr_retrieve_req->bitmask & SON_RETRIEVE_HO_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_retrieve_req->ho_status, p_src + *p_length_read, "ho_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_nr_retrieve_req->ho_status < 1) || (p_son_oam_nr_retrieve_req->ho_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_retrieve_req->ho_status] should be in range [1 <= %d <= 2] !", p_son_oam_nr_retrieve_req->ho_status);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_nr_retrieve_req->bitmask & SON_RETRIEVE_X2_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_retrieve_req->x2_status, p_src + *p_length_read, "x2_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_nr_retrieve_req->x2_status < 1) || (p_son_oam_nr_retrieve_req->x2_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_retrieve_req->x2_status] should be in range [1 <= %d <= 2] !", p_son_oam_nr_retrieve_req->x2_status);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_retrieve_resp
(
    son_oam_nr_retrieve_resp_t *p_son_oam_nr_retrieve_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_retrieve_resp, 0, sizeof(son_oam_nr_retrieve_resp_t));

    /* This function parses son_oam_nr_retrieve_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_retrieve_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_nr_retrieve_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_retrieve_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_retrieve_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_retrieve_resp->result] should be in range [%d <= 1] !", p_son_oam_nr_retrieve_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_retrieve_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_neighbor_info(
        &p_son_oam_nr_retrieve_resp->nr_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_updated_nrt_info
(
    son_oam_anr_updated_nrt_info_t *p_son_oam_anr_updated_nrt_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_updated_nrt_info, 0, sizeof(son_oam_anr_updated_nrt_info_t));

    /* This function parses son_oam_anr_updated_nrt_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_updated_nrt_info");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_updated_nrt_info->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_updated_nrt_info->nrt_op, p_src + *p_length_read, "nrt_op");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_updated_nrt_info->nrt_op < 1) || (p_son_oam_anr_updated_nrt_info->nrt_op > 4))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_updated_nrt_info->nrt_op] should be in range [1 <= %d <= 4] !", p_son_oam_anr_updated_nrt_info->nrt_op);
        return SON_FAILURE;
    }



    if (SON_FAILURE == son_parse_son_neighbor_info(
        &p_son_oam_anr_updated_nrt_info->nr_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_enable_req
(
    son_oam_es_enable_req_t *p_son_oam_es_enable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_enable_req, 0, sizeof(son_oam_es_enable_req_t));

    /* This function parses son_oam_es_enable_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_enable_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_enable_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_es_enable_req->bitmask & SON_ES_ENABLE_SRV_CGI_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_enable_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_es_enable_req->interval, p_src + *p_length_read, "interval");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_es_enable_req->threshold_value, p_src + *p_length_read, "threshold_value");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_enable_resp
(
    son_oam_es_enable_resp_t *p_son_oam_es_enable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_enable_resp, 0, sizeof(son_oam_es_enable_resp_t));

    /* This function parses son_oam_es_enable_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_enable_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_enable_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_enable_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_enable_resp->result] should be in range [%d <= 1] !", p_son_oam_es_enable_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_enable_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_es_enable_resp->cell_status_list_size, p_src + *p_length_read, "cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_enable_resp->cell_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_enable_resp->cell_status_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_es_enable_resp->cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_es_enable_resp->cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_status(
                &p_son_oam_es_enable_resp->cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_disable_req
(
    son_oam_es_disable_req_t *p_son_oam_es_disable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_disable_req, 0, sizeof(son_oam_es_disable_req_t));

    /* This function parses son_oam_es_disable_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_disable_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_disable_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_es_disable_req->bitmask & SON_ES_DISABLE_SRV_CGI_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_disable_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_disable_resp
(
    son_oam_es_disable_resp_t *p_son_oam_es_disable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_disable_resp, 0, sizeof(son_oam_es_disable_resp_t));

    /* This function parses son_oam_es_disable_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_disable_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_disable_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_disable_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_disable_resp->result] should be in range [%d <= 1] !", p_son_oam_es_disable_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_disable_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_es_disable_resp->cell_status_list_size, p_src + *p_length_read, "cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_disable_resp->cell_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_disable_resp->cell_status_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_es_disable_resp->cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_es_disable_resp->cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_status(
                &p_son_oam_es_disable_resp->cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_modify_ue_count_interval_req
(
    son_oam_es_modify_ue_count_interval_req_t *p_son_oam_es_modify_ue_count_interval_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_modify_ue_count_interval_req, 0, sizeof(son_oam_es_modify_ue_count_interval_req_t));

    /* This function parses son_oam_es_modify_ue_count_interval_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_modify_ue_count_interval_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_modify_ue_count_interval_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_es_modify_ue_count_interval_req->interval, p_src + *p_length_read, "interval");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_modify_ue_count_interval_resp
(
    son_oam_es_modify_ue_count_interval_resp_t *p_son_oam_es_modify_ue_count_interval_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_modify_ue_count_interval_resp, 0, sizeof(son_oam_es_modify_ue_count_interval_resp_t));

    /* This function parses son_oam_es_modify_ue_count_interval_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_modify_ue_count_interval_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_modify_ue_count_interval_resp->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_modify_ue_count_interval_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_modify_ue_count_interval_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_modify_ue_count_interval_resp->result] should be in range [%d <= 1] !", p_son_oam_es_modify_ue_count_interval_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_modify_ue_count_interval_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_cell_switch_off_req
(
    son_oam_es_cell_switch_off_req_t *p_son_oam_es_cell_switch_off_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_cell_switch_off_req, 0, sizeof(son_oam_es_cell_switch_off_req_t));

    /* This function parses son_oam_es_cell_switch_off_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_cell_switch_off_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_cell_switch_off_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_cell_switch_off_req->switch_off_mode, p_src + *p_length_read, "switch_off_mode");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_es_cell_switch_off_req->switch_off_mode < 1) || (p_son_oam_es_cell_switch_off_req->switch_off_mode > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_cell_switch_off_req->switch_off_mode] should be in range [1 <= %d <= 2] !", p_son_oam_es_cell_switch_off_req->switch_off_mode);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_cell_switch_off_resp
(
    son_oam_es_cell_switch_off_resp_t *p_son_oam_es_cell_switch_off_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_cell_switch_off_resp, 0, sizeof(son_oam_es_cell_switch_off_resp_t));

    /* This function parses son_oam_es_cell_switch_off_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_cell_switch_off_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_cell_switch_off_resp->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_cell_switch_off_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_cell_switch_off_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_cell_switch_off_resp->result] should be in range [%d <= 1] !", p_son_oam_es_cell_switch_off_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_cell_switch_off_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_switched_off_cell_info_resp
(
    son_oam_es_switched_off_cell_info_resp_t *p_son_oam_es_switched_off_cell_info_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_switched_off_cell_info_resp, 0, sizeof(son_oam_es_switched_off_cell_info_resp_t));

    /* This function parses son_oam_es_switched_off_cell_info_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_switched_off_cell_info_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_switched_off_cell_info_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_switched_off_cell_info_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_switched_off_cell_info_resp->result] should be in range [%d <= 1] !", p_son_oam_es_switched_off_cell_info_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_switched_off_cell_info_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_es_switched_off_cell_info_resp->num_switched_off_cells, p_src + *p_length_read, "num_switched_off_cells");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_switched_off_cell_info_resp->num_switched_off_cells > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_switched_off_cell_info_resp->num_switched_off_cells] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_es_switched_off_cell_info_resp->num_switched_off_cells);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_es_switched_off_cell_info_resp->num_switched_off_cells; loop++)
        {
            if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
                &p_son_oam_es_switched_off_cell_info_resp->switched_off_cell_info_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_cell_switch_on_req
(
    son_oam_es_cell_switch_on_req_t *p_son_oam_es_cell_switch_on_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_cell_switch_on_req, 0, sizeof(son_oam_es_cell_switch_on_req_t));

    /* This function parses son_oam_es_cell_switch_on_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_cell_switch_on_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_cell_switch_on_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_cell_switch_on_resp
(
    son_oam_es_cell_switch_on_resp_t *p_son_oam_es_cell_switch_on_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_cell_switch_on_resp, 0, sizeof(son_oam_es_cell_switch_on_resp_t));

    /* This function parses son_oam_es_cell_switch_on_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_cell_switch_on_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_cell_switch_on_resp->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_cell_switch_on_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_cell_switch_on_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_cell_switch_on_resp->result] should be in range [%d <= 1] !", p_son_oam_es_cell_switch_on_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_cell_switch_on_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_active_ue_count_report
(
    son_oam_es_active_ue_count_report_t *p_son_oam_es_active_ue_count_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_active_ue_count_report, 0, sizeof(son_oam_es_active_ue_count_report_t));

    /* This function parses son_oam_es_active_ue_count_report */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_active_ue_count_report");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_active_ue_count_report->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_es_active_ue_count_report->ue_cnt, p_src + *p_length_read, "ue_cnt");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_modify_ue_count_threshold_req
(
    son_oam_es_modify_ue_count_threshold_req_t *p_son_oam_es_modify_ue_count_threshold_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_modify_ue_count_threshold_req, 0, sizeof(son_oam_es_modify_ue_count_threshold_req_t));

    /* This function parses son_oam_es_modify_ue_count_threshold_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_modify_ue_count_threshold_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_modify_ue_count_threshold_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_es_modify_ue_count_threshold_req->threshold_value, p_src + *p_length_read, "threshold_value");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_modify_ue_count_threshold_resp
(
    son_oam_es_modify_ue_count_threshold_resp_t *p_son_oam_es_modify_ue_count_threshold_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_modify_ue_count_threshold_resp, 0, sizeof(son_oam_es_modify_ue_count_threshold_resp_t));

    /* This function parses son_oam_es_modify_ue_count_threshold_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_modify_ue_count_threshold_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_modify_ue_count_threshold_resp->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_modify_ue_count_threshold_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_modify_ue_count_threshold_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_modify_ue_count_threshold_resp->result] should be in range [%d <= 1] !", p_son_oam_es_modify_ue_count_threshold_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_modify_ue_count_threshold_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_ue_count_threshold_hit_ind
(
    son_oam_es_ue_count_threshold_hit_ind_t *p_son_oam_es_ue_count_threshold_hit_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_ue_count_threshold_hit_ind, 0, sizeof(son_oam_es_ue_count_threshold_hit_ind_t));

    /* This function parses son_oam_es_ue_count_threshold_hit_ind */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_ue_count_threshold_hit_ind");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_ue_count_threshold_hit_ind->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_ue_count_threshold_hit_ind->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_ue_count_threshold_hit_ind->cause > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_ue_count_threshold_hit_ind->cause] should be in range [%d <= 1] !", p_son_oam_es_ue_count_threshold_hit_ind->cause);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_request_to_switch_on_cell
(
    son_oam_es_request_to_switch_on_cell_t *p_son_oam_es_request_to_switch_on_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_request_to_switch_on_cell, 0, sizeof(son_oam_es_request_to_switch_on_cell_t));

    /* This function parses son_oam_es_request_to_switch_on_cell */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_request_to_switch_on_cell");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_es_request_to_switch_on_cell->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_request_to_switch_on_cell->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_periodic_nw_scan_req
(
    son_oam_periodic_nw_scan_req_t *p_son_oam_periodic_nw_scan_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_periodic_nw_scan_req, 0, sizeof(son_oam_periodic_nw_scan_req_t));

    /* This function parses son_oam_periodic_nw_scan_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_periodic_nw_scan_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_periodic_nw_scan_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_periodic_nw_scan_req->scan_on_off, p_src + *p_length_read, "scan_on_off");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_periodic_nw_scan_req->scan_on_off > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_periodic_nw_scan_req->scan_on_off] should be in range [%d <= 1] !", p_son_oam_periodic_nw_scan_req->scan_on_off);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_periodic_nw_scan_req->scan_time_interval, p_src + *p_length_read, "scan_time_interval");
    *p_length_read += sizeof(U32);

    if (p_son_oam_periodic_nw_scan_req->bitmask & SON_INTRA_RAT_PERIODIC_NW_SCAN_REQ_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_start_intra_rat_scan_req(
        &p_son_oam_periodic_nw_scan_req->start_intra_rat_periodic_scan_req,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_oam_periodic_nw_scan_req->bitmask & SON_INTER_RAT_PERIODIC_NW_SCAN_REQ_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_start_inter_rat_scan_req(
        &p_son_oam_periodic_nw_scan_req->start_inter_rat_periodic_scan_req,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_meas_config_req
(
    son_oam_anr_meas_config_req_t *p_son_oam_anr_meas_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_meas_config_req, 0, sizeof(son_oam_anr_meas_config_req_t));

    /* This function parses son_oam_anr_meas_config_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_meas_config_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_meas_config_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_meas_config_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_meas_config_req->eutran_config_list_size, p_src + *p_length_read, "eutran_config_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_meas_config_req->eutran_config_list_size > SON_MAX_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_meas_config_req->eutran_config_list_size] should be in range [%d <= SON_MAX_CONFIG_OBJECT] !", p_son_oam_anr_meas_config_req->eutran_config_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_anr_meas_config_req->eutran_config_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_meas_config_eutran(
                &p_son_oam_anr_meas_config_req->eutran_config_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }



    if (SON_FAILURE == son_parse_son_anr_meas_config_inter_rat(
        &p_son_oam_anr_meas_config_req->other_rat_config_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_oam_anr_meas_config_req->bitmask & SON_ANR_MEAS_GAP_CONFIG_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_meas_gap_config(
        &p_son_oam_anr_meas_config_req->meas_gap_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_speed_state_scale_factors
(
    son_speed_state_scale_factors_t *p_son_speed_state_scale_factors,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_speed_state_scale_factors, 0, sizeof(son_speed_state_scale_factors_t));

    /* This function parses son_speed_state_scale_factors */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_speed_state_scale_factors");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_speed_state_scale_factors->sf_medium, p_src + *p_length_read, "sf_medium");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_speed_state_scale_factors->sf_medium > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_speed_state_scale_factors->sf_medium] should be in range [%d <= 3] !", p_son_speed_state_scale_factors->sf_medium);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_speed_state_scale_factors->sf_high, p_src + *p_length_read, "sf_high");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_speed_state_scale_factors->sf_high > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_speed_state_scale_factors->sf_high] should be in range [%d <= 3] !", p_son_speed_state_scale_factors->sf_high);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_meas_config_eutran
(
    son_meas_config_eutran_t *p_son_meas_config_eutran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_meas_config_eutran, 0, sizeof(son_meas_config_eutran_t));

    /* This function parses son_meas_config_eutran */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_meas_config_eutran");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_config_eutran->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_config_eutran->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_ALLOW_MEAS_BW_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_config_eutran->meas_bandwidth_for_earfcn, p_src + *p_length_read, "meas_bandwidth_for_earfcn");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_eutran->meas_bandwidth_for_earfcn > 5))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->meas_bandwidth_for_earfcn] should be in range [%d <= 5] !", p_son_meas_config_eutran->meas_bandwidth_for_earfcn);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_PRESENCE_ANTENNA_PORT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_eutran->presence_antenna_port, p_src + *p_length_read, "presence_antenna_port");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_eutran->presence_antenna_port > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->presence_antenna_port] should be in range [%d <= 1] !", p_son_meas_config_eutran->presence_antenna_port);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_NBR_CELL_CONFIG_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_eutran->neighbor_cell_config, p_src + *p_length_read, "neighbor_cell_config");
    *p_length_read += sizeof(U8);
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_OFFSET_FREQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_config_eutran->offset_frequency, p_src + *p_length_read, "offset_frequency");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_eutran->offset_frequency > 30))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->offset_frequency] should be in range [%d <= 30] !", p_son_meas_config_eutran->offset_frequency);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_Q_RX_LEV_MIN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_meas_config_eutran->q_rx_lev_min, p_src + *p_length_read, "q_rx_lev_min");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_config_eutran->q_rx_lev_min < -70) || (p_son_meas_config_eutran->q_rx_lev_min > -22))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->q_rx_lev_min] should be in range [-70 <= %d <= -22] !", p_son_meas_config_eutran->q_rx_lev_min);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_P_MAX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_meas_config_eutran->p_max, p_src + *p_length_read, "p_max");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_config_eutran->p_max < -30) || (p_son_meas_config_eutran->p_max > 33))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->p_max] should be in range [-30 <= %d <= 33] !", p_son_meas_config_eutran->p_max);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_RESELECTION_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_eutran->t_reselection, p_src + *p_length_read, "t_reselection");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_eutran->t_reselection > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->t_reselection] should be in range [%d <= 7] !", p_son_meas_config_eutran->t_reselection);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_RESELECTION_SF_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_speed_state_scale_factors(
        &p_son_meas_config_eutran->t_reselection_sf,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_CELL_THRESH_HIGH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_eutran->threshX_high, p_src + *p_length_read, "threshX_high");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_eutran->threshX_high > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->threshX_high] should be in range [%d <= 31] !", p_son_meas_config_eutran->threshX_high);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_CELL_THRESH_LOW_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_eutran->threshX_low, p_src + *p_length_read, "threshX_low");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_eutran->threshX_low > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->threshX_low] should be in range [%d <= 31] !", p_son_meas_config_eutran->threshX_low);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_eutran->cell_reselection_priority, p_src + *p_length_read, "cell_reselection_priority");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_eutran->cell_reselection_priority > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->cell_reselection_priority] should be in range [%d <= 7] !", p_son_meas_config_eutran->cell_reselection_priority);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_Q_QUAL_MIN_R9)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_meas_config_eutran->q_qualmin_r9, p_src + *p_length_read, "q_qualmin_r9");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_config_eutran->q_qualmin_r9 < -34) || (p_son_meas_config_eutran->q_qualmin_r9 > -3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->q_qualmin_r9] should be in range [-34 <= %d <= -3] !", p_son_meas_config_eutran->q_qualmin_r9);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_THRESHX_HIGH_Q_R9)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_eutran->threshx_highq_r9, p_src + *p_length_read, "threshx_highq_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_eutran->threshx_highq_r9 > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->threshx_highq_r9] should be in range [%d <= 31] !", p_son_meas_config_eutran->threshx_highq_r9);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_THRESHX_LOW_Q_R9)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_eutran->threshx_lowq_r9, p_src + *p_length_read, "threshx_lowq_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_eutran->threshx_lowq_r9 > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->threshx_lowq_r9] should be in range [%d <= 31] !", p_son_meas_config_eutran->threshx_lowq_r9);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_EUTRAN_CM_OFFSET_FREQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_config_eutran->cm_offset_frequency, p_src + *p_length_read, "cm_offset_frequency");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_eutran->cm_offset_frequency > 30))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_eutran->cm_offset_frequency] should be in range [%d <= 30] !", p_son_meas_config_eutran->cm_offset_frequency);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_INTER_FREQ_OPEN_PCI_RANGE_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_per_freq_pci_range(
        &p_son_meas_config_eutran->inter_freq_open_pci_range,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_INTER_FREQ_CLOSED_PCI_RANGE_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_per_freq_pci_range(
        &p_son_meas_config_eutran->inter_freq_closed_pci_range,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_meas_config_eutran->bitmask & SON_MC_INTER_FREQ_HYBRID_PCI_RANGE_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_per_freq_pci_range(
        &p_son_meas_config_eutran->inter_freq_hybrid_pci_range,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_utran_pci_fdd_range
(
    son_utran_pci_fdd_range_t *p_son_utran_pci_fdd_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_utran_pci_fdd_range, 0, sizeof(son_utran_pci_fdd_range_t));

    /* This function parses son_utran_pci_fdd_range */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_utran_pci_fdd_range");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_utran_pci_fdd_range->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_utran_pci_fdd_range->start, p_src + *p_length_read, "start");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_utran_pci_fdd_range->start > 511))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_pci_fdd_range->start] should be in range [%d <= 511] !", p_son_utran_pci_fdd_range->start);
        return SON_FAILURE;
    }

    if (p_son_utran_pci_fdd_range->bitmask & SON_UTRAN_PCI_FDD_RANGE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_utran_pci_fdd_range->range, p_src + *p_length_read, "range");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_utran_pci_fdd_range->range < 2) || (p_son_utran_pci_fdd_range->range > 512))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_utran_pci_fdd_range->range] should be in range [2 <= %d <= 512] !", p_son_utran_pci_fdd_range->range);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_meas_config_utran_fdd
(
    son_meas_config_utran_fdd_t *p_son_meas_config_utran_fdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_meas_config_utran_fdd, 0, sizeof(son_meas_config_utran_fdd_t));

    /* This function parses son_meas_config_utran_fdd */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_meas_config_utran_fdd");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_config_utran_fdd->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_meas_config_utran_fdd->uarfcn, p_src + *p_length_read, "uarfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran_fdd->uarfcn > 16383))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_fdd->uarfcn] should be in range [%d <= 16383] !", p_son_meas_config_utran_fdd->uarfcn);
        return SON_FAILURE;
    }

    if (p_son_meas_config_utran_fdd->bitmask & SON_MC_UTRAN_FDD_OFFSET_FREQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_meas_config_utran_fdd->offset_frequency, p_src + *p_length_read, "offset_frequency");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_config_utran_fdd->offset_frequency < -15) || (p_son_meas_config_utran_fdd->offset_frequency > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_fdd->offset_frequency] should be in range [-15 <= %d <= 15] !", p_son_meas_config_utran_fdd->offset_frequency);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_fdd->bitmask & SON_MC_UTRAN_FDD_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_utran_fdd->cell_reselection_priority, p_src + *p_length_read, "cell_reselection_priority");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran_fdd->cell_reselection_priority > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_fdd->cell_reselection_priority] should be in range [%d <= 7] !", p_son_meas_config_utran_fdd->cell_reselection_priority);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_fdd->bitmask & SON_MC_UTRAN_FDD_THRESH_HIGH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_utran_fdd->threshX_high, p_src + *p_length_read, "threshX_high");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran_fdd->threshX_high > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_fdd->threshX_high] should be in range [%d <= 31] !", p_son_meas_config_utran_fdd->threshX_high);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_fdd->bitmask & SON_MC_UTRAN_FDD_THRESH_LOW_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_utran_fdd->threshX_low, p_src + *p_length_read, "threshX_low");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran_fdd->threshX_low > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_fdd->threshX_low] should be in range [%d <= 31] !", p_son_meas_config_utran_fdd->threshX_low);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_fdd->bitmask & SON_MC_UTRAN_FDD_Q_RX_LEV_MIN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_meas_config_utran_fdd->q_rx_lev_min, p_src + *p_length_read, "q_rx_lev_min");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_config_utran_fdd->q_rx_lev_min < -60) || (p_son_meas_config_utran_fdd->q_rx_lev_min > -13))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_fdd->q_rx_lev_min] should be in range [-60 <= %d <= -13] !", p_son_meas_config_utran_fdd->q_rx_lev_min);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_fdd->bitmask & SON_MC_UTRAN_FDD_P_MAX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_meas_config_utran_fdd->p_max, p_src + *p_length_read, "p_max");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_config_utran_fdd->p_max < -50) || (p_son_meas_config_utran_fdd->p_max > 33))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_fdd->p_max] should be in range [-50 <= %d <= 33] !", p_son_meas_config_utran_fdd->p_max);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_fdd->bitmask & SON_MC_UTRAN_FDD_Q_QUAL_MIN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_meas_config_utran_fdd->q_qual_min, p_src + *p_length_read, "q_qual_min");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_config_utran_fdd->q_qual_min < -24) || (p_son_meas_config_utran_fdd->q_qual_min > 0))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_fdd->q_qual_min] should be in range [-24 <= %d <= 0] !", p_son_meas_config_utran_fdd->q_qual_min);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_fdd->bitmask & SON_MC_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_utran_fdd->num_csg_allowed_reporting_cell, p_src + *p_length_read, "num_csg_allowed_reporting_cell");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_config_utran_fdd->num_csg_allowed_reporting_cell < 1) || (p_son_meas_config_utran_fdd->num_csg_allowed_reporting_cell > SON_MAX_PCI_RANGE))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_fdd->num_csg_allowed_reporting_cell] should be in range [1 <= %d <= SON_MAX_PCI_RANGE] !", p_son_meas_config_utran_fdd->num_csg_allowed_reporting_cell);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_fdd->bitmask & SON_MC_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_meas_config_utran_fdd->num_csg_allowed_reporting_cell; loop++)
        {
            if (SON_FAILURE == son_parse_son_utran_pci_fdd_range(
                &p_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_son_meas_config_utran_fdd->bitmask & SON_MC_UTRAN_THRESHX_HIGH_Q_R9)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_utran_fdd->threshx_highq_r9, p_src + *p_length_read, "threshx_highq_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran_fdd->threshx_highq_r9 > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_fdd->threshx_highq_r9] should be in range [%d <= 31] !", p_son_meas_config_utran_fdd->threshx_highq_r9);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_fdd->bitmask & SON_MC_UTRAN_THRESHX_LOW_Q_R9)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_utran_fdd->threshx_lowq_r9, p_src + *p_length_read, "threshx_lowq_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran_fdd->threshx_lowq_r9 > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_fdd->threshx_lowq_r9] should be in range [%d <= 31] !", p_son_meas_config_utran_fdd->threshx_lowq_r9);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_meas_config_utran_tdd
(
    son_meas_config_utran_tdd_t *p_son_meas_config_utran_tdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_meas_config_utran_tdd, 0, sizeof(son_meas_config_utran_tdd_t));

    /* This function parses son_meas_config_utran_tdd */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_meas_config_utran_tdd");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_config_utran_tdd->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_meas_config_utran_tdd->uarfcn, p_src + *p_length_read, "uarfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran_tdd->uarfcn > 16383))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_tdd->uarfcn] should be in range [%d <= 16383] !", p_son_meas_config_utran_tdd->uarfcn);
        return SON_FAILURE;
    }

    if (p_son_meas_config_utran_tdd->bitmask & SON_MC_UTRAN_TDD_OFFSET_FREQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_meas_config_utran_tdd->offset_frequency, p_src + *p_length_read, "offset_frequency");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_config_utran_tdd->offset_frequency < -15) || (p_son_meas_config_utran_tdd->offset_frequency > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_tdd->offset_frequency] should be in range [-15 <= %d <= 15] !", p_son_meas_config_utran_tdd->offset_frequency);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_tdd->bitmask & SON_MC_UTRAN_TDD_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_utran_tdd->cell_reselection_priority, p_src + *p_length_read, "cell_reselection_priority");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran_tdd->cell_reselection_priority > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_tdd->cell_reselection_priority] should be in range [%d <= 7] !", p_son_meas_config_utran_tdd->cell_reselection_priority);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_tdd->bitmask & SON_MC_UTRAN_TDD_THRESH_HIGH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_utran_tdd->threshX_high, p_src + *p_length_read, "threshX_high");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran_tdd->threshX_high > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_tdd->threshX_high] should be in range [%d <= 31] !", p_son_meas_config_utran_tdd->threshX_high);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_tdd->bitmask & SON_MC_UTRAN_TDD_THRESH_LOW_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_utran_tdd->threshX_low, p_src + *p_length_read, "threshX_low");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran_tdd->threshX_low > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_tdd->threshX_low] should be in range [%d <= 31] !", p_son_meas_config_utran_tdd->threshX_low);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_tdd->bitmask & SON_MC_UTRAN_TDD_Q_RX_LEV_MIN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_meas_config_utran_tdd->q_rx_lev_min, p_src + *p_length_read, "q_rx_lev_min");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_config_utran_tdd->q_rx_lev_min < -60) || (p_son_meas_config_utran_tdd->q_rx_lev_min > -13))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_tdd->q_rx_lev_min] should be in range [-60 <= %d <= -13] !", p_son_meas_config_utran_tdd->q_rx_lev_min);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran_tdd->bitmask & SON_MC_UTRAN_TDD_P_MAX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_meas_config_utran_tdd->p_max, p_src + *p_length_read, "p_max");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_meas_config_utran_tdd->p_max < -50) || (p_son_meas_config_utran_tdd->p_max > 33))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran_tdd->p_max] should be in range [-50 <= %d <= 33] !", p_son_meas_config_utran_tdd->p_max);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_meas_config_utran
(
    son_meas_config_utran_t *p_son_meas_config_utran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_meas_config_utran, 0, sizeof(son_meas_config_utran_t));

    /* This function parses son_meas_config_utran */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_meas_config_utran");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_config_utran->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_utran->utran_fdd_list_size, p_src + *p_length_read, "utran_fdd_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran->utran_fdd_list_size > SON_MAX_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran->utran_fdd_list_size] should be in range [%d <= SON_MAX_CONFIG_OBJECT] !", p_son_meas_config_utran->utran_fdd_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_meas_config_utran->utran_fdd_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_meas_config_utran_fdd(
                &p_son_meas_config_utran->utran_fdd_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_utran->utran_tdd_list_size, p_src + *p_length_read, "utran_tdd_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran->utran_tdd_list_size > SON_MAX_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran->utran_tdd_list_size] should be in range [%d <= SON_MAX_CONFIG_OBJECT] !", p_son_meas_config_utran->utran_tdd_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_meas_config_utran->utran_tdd_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_meas_config_utran_tdd(
                &p_son_meas_config_utran->utran_tdd_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_son_meas_config_utran->bitmask & SON_UTRAN_RESELECTION_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_utran->t_reselection, p_src + *p_length_read, "t_reselection");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_utran->t_reselection > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_utran->t_reselection] should be in range [%d <= 7] !", p_son_meas_config_utran->t_reselection);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_utran->bitmask & SON_UTRAN_RESELECTION_SF_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_speed_state_scale_factors(
        &p_son_meas_config_utran->t_reselection_sf,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_geran_following_arfcns
(
    son_geran_following_arfcns_t *p_son_geran_following_arfcns,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_geran_following_arfcns, 0, sizeof(son_geran_following_arfcns_t));

    /* This function parses son_geran_following_arfcns */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_geran_following_arfcns");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_geran_following_arfcns->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_geran_following_arfcns->bitmask & SON_MC_GERAN_EXP_ARFCN_SET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_following_arfcns->exp_arfcn_list_size, p_src + *p_length_read, "exp_arfcn_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_geran_following_arfcns->exp_arfcn_list_size < 1) || (p_son_geran_following_arfcns->exp_arfcn_list_size > SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_following_arfcns->exp_arfcn_list_size] should be in range [1 <= %d <= SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE] !", p_son_geran_following_arfcns->exp_arfcn_list_size);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_following_arfcns->bitmask & SON_MC_GERAN_EXP_ARFCN_SET_PRESENT)
    {

    if (*p_length_read + (S32)(p_son_geran_following_arfcns->exp_arfcn_list_size * sizeof(p_son_geran_following_arfcns->exp_arfcn_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_geran_following_arfcns->exp_arfcn_list_size; loop++)
        {
            son_cp_unpack_U16((void*)&p_son_geran_following_arfcns->exp_arfcn_list[loop], (void*)(p_src + *p_length_read), "exp_arfcn_list[]");
            *p_length_read += sizeof(U16);
        }
    }
    }

    if (p_son_geran_following_arfcns->bitmask & SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_following_arfcns->arfcn_spacing, p_src + *p_length_read, "arfcn_spacing");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_geran_following_arfcns->arfcn_spacing < 1) || (p_son_geran_following_arfcns->arfcn_spacing > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_following_arfcns->arfcn_spacing] should be in range [1 <= %d <= 8] !", p_son_geran_following_arfcns->arfcn_spacing);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_following_arfcns->bitmask & SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_following_arfcns->num_of_following_arfcn, p_src + *p_length_read, "num_of_following_arfcn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_following_arfcns->num_of_following_arfcn > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_following_arfcns->num_of_following_arfcn] should be in range [%d <= 31] !", p_son_geran_following_arfcns->num_of_following_arfcn);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_following_arfcns->bitmask & SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_following_arfcns->arfcn_bmp_list_size, p_src + *p_length_read, "arfcn_bmp_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_geran_following_arfcns->arfcn_bmp_list_size < 1) || (p_son_geran_following_arfcns->arfcn_bmp_list_size > SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_following_arfcns->arfcn_bmp_list_size] should be in range [1 <= %d <= SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP] !", p_son_geran_following_arfcns->arfcn_bmp_list_size);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_following_arfcns->bitmask & SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT)
    {

    if (*p_length_read + (S32)(p_son_geran_following_arfcns->arfcn_bmp_list_size * sizeof(p_son_geran_following_arfcns->arfcn_bmp_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_geran_following_arfcns->arfcn_bmp_list_size; loop++)
        {
            son_cp_unpack_U8((void*)&p_son_geran_following_arfcns->arfcn_bmp_list[loop], (void*)(p_src + *p_length_read), "arfcn_bmp_list[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_geran_carrier_freqs
(
    son_geran_carrier_freqs_t *p_son_geran_carrier_freqs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_geran_carrier_freqs, 0, sizeof(son_geran_carrier_freqs_t));

    /* This function parses son_geran_carrier_freqs */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_geran_carrier_freqs");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_geran_carrier_freqs->starting_arfcn, p_src + *p_length_read, "starting_arfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_carrier_freqs->starting_arfcn > 1023))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_carrier_freqs->starting_arfcn] should be in range [%d <= 1023] !", p_son_geran_carrier_freqs->starting_arfcn);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_geran_carrier_freqs->band_ind, p_src + *p_length_read, "band_ind");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_carrier_freqs->band_ind > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_carrier_freqs->band_ind] should be in range [%d <= 1] !", p_son_geran_carrier_freqs->band_ind);
        return SON_FAILURE;
    }



    if (SON_FAILURE == son_parse_son_geran_following_arfcns(
        &p_son_geran_carrier_freqs->following_arfcns,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_geran_carrier_freq_info
(
    son_geran_carrier_freq_info_t *p_son_geran_carrier_freq_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_geran_carrier_freq_info, 0, sizeof(son_geran_carrier_freq_info_t));

    /* This function parses son_geran_carrier_freq_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_geran_carrier_freq_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_geran_carrier_freq_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_geran_carrier_freqs(
        &p_son_geran_carrier_freq_info->geran_car_freqs,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_geran_carrier_freq_info->bitmask & SON_GERAN_CFI_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_carrier_freq_info->cell_reselection_priority, p_src + *p_length_read, "cell_reselection_priority");
    *p_length_read += sizeof(U8);
    }

    if (p_son_geran_carrier_freq_info->bitmask & SON_GERAN_CFI_NCC_PERMITTED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_carrier_freq_info->ncc_permitted, p_src + *p_length_read, "ncc_permitted");
    *p_length_read += sizeof(U8);
    }

    if (p_son_geran_carrier_freq_info->bitmask & SON_GERAN_CFI_Q_RX_LEV_MIN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_carrier_freq_info->q_rx_lev_min, p_src + *p_length_read, "q_rx_lev_min");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_carrier_freq_info->q_rx_lev_min > 45))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_carrier_freq_info->q_rx_lev_min] should be in range [%d <= 45] !", p_son_geran_carrier_freq_info->q_rx_lev_min);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_carrier_freq_info->bitmask & SON_GERAN_CFI_MAX_ALLOWED_TRANS_PWR_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_carrier_freq_info->p_max_geran, p_src + *p_length_read, "p_max_geran");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_carrier_freq_info->p_max_geran > 39))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_carrier_freq_info->p_max_geran] should be in range [%d <= 39] !", p_son_geran_carrier_freq_info->p_max_geran);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_carrier_freq_info->bitmask & SON_GERAN_CFI_THRESH_HIGH_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_carrier_freq_info->threshX_high, p_src + *p_length_read, "threshX_high");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_carrier_freq_info->threshX_high > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_carrier_freq_info->threshX_high] should be in range [%d <= 31] !", p_son_geran_carrier_freq_info->threshX_high);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_carrier_freq_info->bitmask & SON_GERAN_CFI_THRESH_LOW_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_geran_carrier_freq_info->threshX_low, p_src + *p_length_read, "threshX_low");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_geran_carrier_freq_info->threshX_low > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_carrier_freq_info->threshX_low] should be in range [%d <= 31] !", p_son_geran_carrier_freq_info->threshX_low);
        return SON_FAILURE;
    }
    }

    if (p_son_geran_carrier_freq_info->bitmask & SON_GERAN_CFI_OFFSET_FREQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_geran_carrier_freq_info->offset_frequency, p_src + *p_length_read, "offset_frequency");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_geran_carrier_freq_info->offset_frequency < -15) || (p_son_geran_carrier_freq_info->offset_frequency > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_geran_carrier_freq_info->offset_frequency] should be in range [-15 <= %d <= 15] !", p_son_geran_carrier_freq_info->offset_frequency);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_meas_config_geran
(
    son_meas_config_geran_t *p_son_meas_config_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_meas_config_geran, 0, sizeof(son_meas_config_geran_t));

    /* This function parses son_meas_config_geran */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_meas_config_geran");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_config_geran->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_geran->geran_freq_info_list_size, p_src + *p_length_read, "geran_freq_info_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_geran->geran_freq_info_list_size > SON_MAX_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_geran->geran_freq_info_list_size] should be in range [%d <= SON_MAX_CONFIG_OBJECT] !", p_son_meas_config_geran->geran_freq_info_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_meas_config_geran->geran_freq_info_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_geran_carrier_freq_info(
                &p_son_meas_config_geran->geran_freq_info_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_son_meas_config_geran->bitmask & SON_MC_GERAN_RESELECTION_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_geran->t_reselection, p_src + *p_length_read, "t_reselection");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_geran->t_reselection > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_geran->t_reselection] should be in range [%d <= 7] !", p_son_meas_config_geran->t_reselection);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_geran->bitmask & SON_MC_GERAN_RESELECTION_SF_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_speed_state_scale_factors(
        &p_son_meas_config_geran->t_reselection_sf,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cdma_carrier_freq_info
(
    son_cdma_carrier_freq_info_t *p_son_cdma_carrier_freq_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_cdma_carrier_freq_info, 0, sizeof(son_cdma_carrier_freq_info_t));

    /* This function parses son_cdma_carrier_freq_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cdma_carrier_freq_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_carrier_freq_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_carrier_freq_info->cdma_type, p_src + *p_length_read, "cdma_type");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_carrier_freq_info->cdma_type > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_carrier_freq_info->cdma_type] should be in range [%d <= 1] !", p_son_cdma_carrier_freq_info->cdma_type);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_carrier_freq_info->band_class, p_src + *p_length_read, "band_class");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_carrier_freq_info->band_class > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_carrier_freq_info->band_class] should be in range [%d <= 31] !", p_son_cdma_carrier_freq_info->band_class);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_cdma_carrier_freq_info->arfcn, p_src + *p_length_read, "arfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_carrier_freq_info->arfcn > 2047))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_carrier_freq_info->arfcn] should be in range [%d <= 2047] !", p_son_cdma_carrier_freq_info->arfcn);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_carrier_freq_info->threshX_high, p_src + *p_length_read, "threshX_high");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_carrier_freq_info->threshX_high > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_carrier_freq_info->threshX_high] should be in range [%d <= 63] !", p_son_cdma_carrier_freq_info->threshX_high);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_carrier_freq_info->threshX_low, p_src + *p_length_read, "threshX_low");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_carrier_freq_info->threshX_low > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_carrier_freq_info->threshX_low] should be in range [%d <= 63] !", p_son_cdma_carrier_freq_info->threshX_low);
        return SON_FAILURE;
    }

    if (p_son_cdma_carrier_freq_info->bitmask & SON_CDMA_FREQ_OFFSET_FREQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_cdma_carrier_freq_info->offset_freq, p_src + *p_length_read, "offset_freq");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_cdma_carrier_freq_info->offset_freq < -15) || (p_son_cdma_carrier_freq_info->offset_freq > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_carrier_freq_info->offset_freq] should be in range [-15 <= %d <= 15] !", p_son_cdma_carrier_freq_info->offset_freq);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_carrier_freq_info->bitmask & SON_CDMA_FREQ_CDMA_SRCH_WNDO_SIZE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_carrier_freq_info->srch_wnd_size, p_src + *p_length_read, "srch_wnd_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_carrier_freq_info->srch_wnd_size > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_carrier_freq_info->srch_wnd_size] should be in range [%d <= 15] !", p_son_cdma_carrier_freq_info->srch_wnd_size);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_carrier_freq_info->bitmask & SON_CDMA_FREQ_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_carrier_freq_info->cell_reselection_priority, p_src + *p_length_read, "cell_reselection_priority");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_carrier_freq_info->cell_reselection_priority > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_carrier_freq_info->cell_reselection_priority] should be in range [%d <= 7] !", p_son_cdma_carrier_freq_info->cell_reselection_priority);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cdma_sys_time_info
(
    son_cdma_sys_time_info_t *p_son_cdma_sys_time_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_cdma_sys_time_info, 0, sizeof(son_cdma_sys_time_info_t));

    /* This function parses son_cdma_sys_time_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cdma_sys_time_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_sys_time_info->is_cdma_nw_sync, p_src + *p_length_read, "is_cdma_nw_sync");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_sys_time_info->is_cdma_nw_sync > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_sys_time_info->is_cdma_nw_sync] should be in range [%d <= 1] !", p_son_cdma_sys_time_info->is_cdma_nw_sync);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_sys_time_info->cdma_sys_time_size, p_src + *p_length_read, "cdma_sys_time_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_cdma_sys_time_info->cdma_sys_time_size < 5) || (p_son_cdma_sys_time_info->cdma_sys_time_size > SON_MAX_CDMA_SYS_TIME))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_sys_time_info->cdma_sys_time_size] should be in range [5 <= %d <= SON_MAX_CDMA_SYS_TIME] !", p_son_cdma_sys_time_info->cdma_sys_time_size);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_cdma_sys_time_info->cdma_sys_time_size * sizeof(p_son_cdma_sys_time_info->cdma_sys_time[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_cdma_sys_time_info->cdma_sys_time_size; loop++)
        {
            son_cp_unpack_U8((void*)&p_son_cdma_sys_time_info->cdma_sys_time[loop], (void*)(p_src + *p_length_read), "cdma_sys_time[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_pre_reg_hrpd_info
(
    son_pre_reg_hrpd_info_t *p_son_pre_reg_hrpd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_pre_reg_hrpd_info, 0, sizeof(son_pre_reg_hrpd_info_t));

    /* This function parses son_pre_reg_hrpd_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_pre_reg_hrpd_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_pre_reg_hrpd_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_pre_reg_hrpd_info->pre_reg_allowed, p_src + *p_length_read, "pre_reg_allowed");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_pre_reg_hrpd_info->pre_reg_allowed > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_pre_reg_hrpd_info->pre_reg_allowed] should be in range [%d <= 1] !", p_son_pre_reg_hrpd_info->pre_reg_allowed);
        return SON_FAILURE;
    }

    if (p_son_pre_reg_hrpd_info->bitmask & SON_HRPD_PRE_REG_ZONE_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_pre_reg_hrpd_info->pre_reg_zone_id, p_src + *p_length_read, "pre_reg_zone_id");
    *p_length_read += sizeof(U8);
    }

    if (p_son_pre_reg_hrpd_info->bitmask & SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size, p_src + *p_length_read, "scndry_pre_reg_zone_id_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size < 1) || (p_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size > SON_MAX_SEC_PRE_REG_ZONES))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size] should be in range [1 <= %d <= SON_MAX_SEC_PRE_REG_ZONES] !", p_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size);
        return SON_FAILURE;
    }
    }

    if (p_son_pre_reg_hrpd_info->bitmask & SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT)
    {

    if (*p_length_read + (S32)(p_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size * sizeof(p_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size; loop++)
        {
            son_cp_unpack_U8((void*)&p_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list[loop], (void*)(p_src + *p_length_read), "scndry_pre_reg_zone_id_list[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cdma_hrpd_info
(
    son_cdma_hrpd_info_t *p_son_cdma_hrpd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_cdma_hrpd_info, 0, sizeof(son_cdma_hrpd_info_t));

    /* This function parses son_cdma_hrpd_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cdma_hrpd_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_hrpd_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_cdma_hrpd_info->bitmask & SON_CDMA_HRPD_REG_PARAMS_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_pre_reg_hrpd_info(
        &p_son_cdma_hrpd_info->hrpd_reg_params,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_cdma_hrpd_info->bitmask & SON_CDMA_HRPD_RESELECTION_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_hrpd_info->t_reselection, p_src + *p_length_read, "t_reselection");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_hrpd_info->t_reselection > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_hrpd_info->t_reselection] should be in range [%d <= 7] !", p_son_cdma_hrpd_info->t_reselection);
        return SON_FAILURE;
    }
    }

    if (p_son_cdma_hrpd_info->bitmask & SON_CDMA_HRPD_RESELECTION_SF_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_speed_state_scale_factors(
        &p_son_cdma_hrpd_info->t_reselection_sf,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cdma_csfb_reg_param_1XRTT
(
    son_cdma_csfb_reg_param_1XRTT_t *p_son_cdma_csfb_reg_param_1XRTT,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_cdma_csfb_reg_param_1XRTT, 0, sizeof(son_cdma_csfb_reg_param_1XRTT_t));

    /* This function parses son_cdma_csfb_reg_param_1XRTT */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cdma_csfb_reg_param_1XRTT");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_cdma_csfb_reg_param_1XRTT->sid, p_src + *p_length_read, "sid");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_cdma_csfb_reg_param_1XRTT->nid, p_src + *p_length_read, "nid");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_csfb_reg_param_1XRTT->multiple_sid, p_src + *p_length_read, "multiple_sid");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_csfb_reg_param_1XRTT->multiple_sid > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_csfb_reg_param_1XRTT->multiple_sid] should be in range [%d <= 1] !", p_son_cdma_csfb_reg_param_1XRTT->multiple_sid);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_csfb_reg_param_1XRTT->multiple_nid, p_src + *p_length_read, "multiple_nid");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_csfb_reg_param_1XRTT->multiple_nid > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_csfb_reg_param_1XRTT->multiple_nid] should be in range [%d <= 1] !", p_son_cdma_csfb_reg_param_1XRTT->multiple_nid);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_csfb_reg_param_1XRTT->home_reg, p_src + *p_length_read, "home_reg");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_csfb_reg_param_1XRTT->home_reg > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_csfb_reg_param_1XRTT->home_reg] should be in range [%d <= 1] !", p_son_cdma_csfb_reg_param_1XRTT->home_reg);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg, p_src + *p_length_read, "foreign_sid_reg");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg] should be in range [%d <= 1] !", p_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg, p_src + *p_length_read, "foreign_nid_reg");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg] should be in range [%d <= 1] !", p_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_csfb_reg_param_1XRTT->parameter_reg, p_src + *p_length_read, "parameter_reg");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_csfb_reg_param_1XRTT->parameter_reg > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_csfb_reg_param_1XRTT->parameter_reg] should be in range [%d <= 1] !", p_son_cdma_csfb_reg_param_1XRTT->parameter_reg);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_csfb_reg_param_1XRTT->power_up_reg, p_src + *p_length_read, "power_up_reg");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_csfb_reg_param_1XRTT->power_up_reg > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_csfb_reg_param_1XRTT->power_up_reg] should be in range [%d <= 1] !", p_son_cdma_csfb_reg_param_1XRTT->power_up_reg);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_csfb_reg_param_1XRTT->reg_period, p_src + *p_length_read, "reg_period");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_cdma_csfb_reg_param_1XRTT->reg_zone, p_src + *p_length_read, "reg_zone");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_csfb_reg_param_1XRTT->total_zone, p_src + *p_length_read, "total_zone");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_csfb_reg_param_1XRTT->zone_timer, p_src + *p_length_read, "zone_timer");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cdma_1xrtt_info
(
    son_cdma_1xrtt_info_t *p_son_cdma_1xrtt_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_cdma_1xrtt_info, 0, sizeof(son_cdma_1xrtt_info_t));

    /* This function parses son_cdma_1xrtt_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cdma_1xrtt_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_1xrtt_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cdma_1xrtt_info->t_reselection, p_src + *p_length_read, "t_reselection");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_1xrtt_info->t_reselection > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_1xrtt_info->t_reselection] should be in range [%d <= 7] !", p_son_cdma_1xrtt_info->t_reselection);
        return SON_FAILURE;
    }

    if (p_son_cdma_1xrtt_info->bitmask & SON_CDMA_1XRTT_REG_PARAMS_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_cdma_csfb_reg_param_1XRTT(
        &p_son_cdma_1xrtt_info->xrtt_reg_params,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_cdma_1xrtt_info->bitmask & SON_CDMA_1XRTT_LONG_CODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_cdma_1xrtt_info->cdma_1xrtt_long_code) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_cdma_1xrtt_info->cdma_1xrtt_long_code); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_cdma_1xrtt_info->cdma_1xrtt_long_code[loop], (void*)(p_src + *p_length_read), "cdma_1xrtt_long_code[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_cdma_1xrtt_info->bitmask & SON_CDMA_1XRTT_RESELECTION_SF_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_speed_state_scale_factors(
        &p_son_cdma_1xrtt_info->t_reselection_sf,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cdma_csfb_reg_param_1xrtt_r9
(
    son_cdma_csfb_reg_param_1xrtt_r9_t *p_son_cdma_csfb_reg_param_1xrtt_r9,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_cdma_csfb_reg_param_1xrtt_r9, 0, sizeof(son_cdma_csfb_reg_param_1xrtt_r9_t));

    /* This function parses son_cdma_csfb_reg_param_1xrtt_r9 */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cdma_csfb_reg_param_1xrtt_r9");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_csfb_reg_param_1xrtt_r9->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9, p_src + *p_length_read, "power_down_reg_r9");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9 > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9] should be in range [%d <= 1] !", p_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_ac_barring_config_1xrtt
(
    son_ac_barring_config_1xrtt_t *p_son_ac_barring_config_1xrtt,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_ac_barring_config_1xrtt, 0, sizeof(son_ac_barring_config_1xrtt_t));

    /* This function parses son_ac_barring_config_1xrtt */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_ac_barring_config_1xrtt");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_ac_barring_config_1xrtt->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9, p_src + *p_length_read, "ac_barring_0_to_9_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9 > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9] should be in range [%d <= 63] !", p_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_ac_barring_config_1xrtt->ac_barring_10_r9, p_src + *p_length_read, "ac_barring_10_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ac_barring_config_1xrtt->ac_barring_10_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ac_barring_config_1xrtt->ac_barring_10_r9] should be in range [%d <= 7] !", p_son_ac_barring_config_1xrtt->ac_barring_10_r9);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_ac_barring_config_1xrtt->ac_barring_11_r9, p_src + *p_length_read, "ac_barring_11_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ac_barring_config_1xrtt->ac_barring_11_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ac_barring_config_1xrtt->ac_barring_11_r9] should be in range [%d <= 7] !", p_son_ac_barring_config_1xrtt->ac_barring_11_r9);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_ac_barring_config_1xrtt->ac_barring_12_r9, p_src + *p_length_read, "ac_barring_12_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ac_barring_config_1xrtt->ac_barring_12_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ac_barring_config_1xrtt->ac_barring_12_r9] should be in range [%d <= 7] !", p_son_ac_barring_config_1xrtt->ac_barring_12_r9);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_ac_barring_config_1xrtt->ac_barring_13_r9, p_src + *p_length_read, "ac_barring_13_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ac_barring_config_1xrtt->ac_barring_13_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ac_barring_config_1xrtt->ac_barring_13_r9] should be in range [%d <= 7] !", p_son_ac_barring_config_1xrtt->ac_barring_13_r9);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_ac_barring_config_1xrtt->ac_barring_14_r9, p_src + *p_length_read, "ac_barring_14_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ac_barring_config_1xrtt->ac_barring_14_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ac_barring_config_1xrtt->ac_barring_14_r9] should be in range [%d <= 7] !", p_son_ac_barring_config_1xrtt->ac_barring_14_r9);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_ac_barring_config_1xrtt->ac_barring_15_r9, p_src + *p_length_read, "ac_barring_15_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ac_barring_config_1xrtt->ac_barring_15_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ac_barring_config_1xrtt->ac_barring_15_r9] should be in range [%d <= 7] !", p_son_ac_barring_config_1xrtt->ac_barring_15_r9);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_ac_barring_config_1xrtt->ac_barring_msg_r9, p_src + *p_length_read, "ac_barring_msg_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ac_barring_config_1xrtt->ac_barring_msg_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ac_barring_config_1xrtt->ac_barring_msg_r9] should be in range [%d <= 7] !", p_son_ac_barring_config_1xrtt->ac_barring_msg_r9);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_ac_barring_config_1xrtt->ac_barring_reg_r9, p_src + *p_length_read, "ac_barring_reg_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ac_barring_config_1xrtt->ac_barring_reg_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ac_barring_config_1xrtt->ac_barring_reg_r9] should be in range [%d <= 7] !", p_son_ac_barring_config_1xrtt->ac_barring_reg_r9);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_ac_barring_config_1xrtt->ac_barring_emg_r9, p_src + *p_length_read, "ac_barring_emg_r9");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ac_barring_config_1xrtt->ac_barring_emg_r9 > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ac_barring_config_1xrtt->ac_barring_emg_r9] should be in range [%d <= 7] !", p_son_ac_barring_config_1xrtt->ac_barring_emg_r9);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_meas_config_cdma
(
    son_meas_config_cdma_t *p_son_meas_config_cdma,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_meas_config_cdma, 0, sizeof(son_meas_config_cdma_t));

    /* This function parses son_meas_config_cdma */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_meas_config_cdma");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_config_cdma->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_cdma->cdma_freq_info_list_size, p_src + *p_length_read, "cdma_freq_info_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_cdma->cdma_freq_info_list_size > SON_MAX_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_cdma->cdma_freq_info_list_size] should be in range [%d <= SON_MAX_CONFIG_OBJECT] !", p_son_meas_config_cdma->cdma_freq_info_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_meas_config_cdma->cdma_freq_info_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cdma_carrier_freq_info(
                &p_son_meas_config_cdma->cdma_freq_info_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_son_meas_config_cdma->bitmask & SON_MEAS_CONFIG_CDMA_SYS_TIME_INFO_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_cdma_sys_time_info(
        &p_son_meas_config_cdma->sys_time_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_meas_config_cdma->bitmask & SON_MEAS_CONFIG_CDMA_SRCH_WNDO_SIZE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_meas_config_cdma->srch_wnd_size, p_src + *p_length_read, "srch_wnd_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_cdma->srch_wnd_size > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_cdma->srch_wnd_size] should be in range [%d <= 15] !", p_son_meas_config_cdma->srch_wnd_size);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_cdma->bitmask & SON_MEAS_CONFIG_CDMA_PRE_REG_HRPD_INFO_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_cdma_hrpd_info(
        &p_son_meas_config_cdma->hrpd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_meas_config_cdma->bitmask & SON_MEAS_CONFIG_CDMA_1X_RTT_INFO_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_cdma_1xrtt_info(
        &p_son_meas_config_cdma->cdma_1xrtt_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_meas_config_cdma->bitmask & SON_MEAS_CONFIG_CSFB_SUPPORT_FOR_DUAL_RX_UES_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9, p_src + *p_length_read, "csfb_support_for_dual_Rx_Ues_r9");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9 > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9] should be in range [%d <= 1] !", p_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9);
        return SON_FAILURE;
    }
    }

    if (p_son_meas_config_cdma->bitmask & SON_MEAS_CONFIG_CSFB_REG_PARAM_1XRTT_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_cdma_csfb_reg_param_1xrtt_r9(
        &p_son_meas_config_cdma->csfb_registration_param_1xrtt_r9,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_meas_config_cdma->bitmask & SON_MEAS_CONFIG_AC_BARRING_CONFIG_1XRTT_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_ac_barring_config_1xrtt(
        &p_son_meas_config_cdma->ac_barring_config_1xrtt_r9,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_anr_meas_config_inter_rat
(
    son_anr_meas_config_inter_rat_t *p_son_anr_meas_config_inter_rat,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_anr_meas_config_inter_rat, 0, sizeof(son_anr_meas_config_inter_rat_t));

    /* This function parses son_anr_meas_config_inter_rat */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_anr_meas_config_inter_rat");



    if (SON_FAILURE == son_parse_son_meas_config_utran(
        &p_son_anr_meas_config_inter_rat->utran_config_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_meas_config_geran(
        &p_son_anr_meas_config_inter_rat->geran_config_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_meas_config_cdma(
        &p_son_anr_meas_config_inter_rat->cdma_config_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_setup_meas_gap_config
(
    son_setup_meas_gap_config_t *p_son_setup_meas_gap_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_setup_meas_gap_config, 0, sizeof(son_setup_meas_gap_config_t));

    /* This function parses son_setup_meas_gap_config */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_setup_meas_gap_config");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_setup_meas_gap_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_setup_meas_gap_config->bitmask & SON_ANR_GAP_OFFSET_GP0_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_setup_meas_gap_config->gap_offset_gp0, p_src + *p_length_read, "gap_offset_gp0");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_setup_meas_gap_config->gap_offset_gp0 > 39))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_setup_meas_gap_config->gap_offset_gp0] should be in range [%d <= 39] !", p_son_setup_meas_gap_config->gap_offset_gp0);
        return SON_FAILURE;
    }
    }

    if (p_son_setup_meas_gap_config->bitmask & SON_ANR_GAP_OFFSET_GP1_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_setup_meas_gap_config->gap_offset_gp1, p_src + *p_length_read, "gap_offset_gp1");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_setup_meas_gap_config->gap_offset_gp1 > 79))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_setup_meas_gap_config->gap_offset_gp1] should be in range [%d <= 79] !", p_son_setup_meas_gap_config->gap_offset_gp1);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_gap_config
(
    son_gap_config_t *p_son_gap_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_gap_config, 0, sizeof(son_gap_config_t));

    /* This function parses son_gap_config */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_gap_config");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_gap_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_gap_config->bitmask & SON_ANR_SETUP_GAP_CONFIG_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_setup_meas_gap_config(
        &p_son_gap_config->setup_gap_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_meas_gap_config
(
    son_meas_gap_config_t *p_son_meas_gap_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_meas_gap_config, 0, sizeof(son_meas_gap_config_t));

    /* This function parses son_meas_gap_config */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_meas_gap_config");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_meas_gap_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_meas_gap_config->bitmask & SON_ANR_EUTRAN_GAP_CONFIG_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_gap_config(
        &p_son_meas_gap_config->eutran_gap_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_meas_gap_config->bitmask & SON_ANR_UTRAN_GAP_CONFIG_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_gap_config(
        &p_son_meas_gap_config->utran_gap_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_meas_gap_config->bitmask & SON_ANR_GERAN_GAP_CONFIG_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_gap_config(
        &p_son_meas_gap_config->geran_gap_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_meas_gap_config->bitmask & SON_ANR_CDMA2000_GAP_CONFIG_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_gap_config(
        &p_son_meas_gap_config->cdma2000_gap_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_meas_config_res
(
    son_oam_anr_meas_config_res_t *p_son_oam_anr_meas_config_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_meas_config_res, 0, sizeof(son_oam_anr_meas_config_res_t));

    /* This function parses son_oam_anr_meas_config_res */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_meas_config_res");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_meas_config_res->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_meas_config_res->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_meas_config_res->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_meas_config_res->result] should be in range [%d <= 1] !", p_son_oam_anr_meas_config_res->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_meas_config_res->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_modify_ue_count_req
(
    son_oam_anr_modify_ue_count_req_t *p_son_oam_anr_modify_ue_count_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_modify_ue_count_req, 0, sizeof(son_oam_anr_modify_ue_count_req_t));

    /* This function parses son_oam_anr_modify_ue_count_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_modify_ue_count_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_modify_ue_count_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_anr_modify_ue_count_req->ue_cnt, p_src + *p_length_read, "ue_cnt");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_modify_ue_count_req->ue_cnt < 1) || (p_son_oam_anr_modify_ue_count_req->ue_cnt > 10))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_modify_ue_count_req->ue_cnt] should be in range [1 <= %d <= 10] !", p_son_oam_anr_modify_ue_count_req->ue_cnt);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_modify_ue_count_resp
(
    son_oam_anr_modify_ue_count_resp_t *p_son_oam_anr_modify_ue_count_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_modify_ue_count_resp, 0, sizeof(son_oam_anr_modify_ue_count_resp_t));

    /* This function parses son_oam_anr_modify_ue_count_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_modify_ue_count_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_modify_ue_count_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_modify_ue_count_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_modify_ue_count_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_modify_ue_count_resp->result] should be in range [%d <= 1] !", p_son_oam_anr_modify_ue_count_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_modify_ue_count_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_modify_meas_reporting_interval_req
(
    son_oam_anr_modify_meas_reporting_interval_req_t *p_son_oam_anr_modify_meas_reporting_interval_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_modify_meas_reporting_interval_req, 0, sizeof(son_oam_anr_modify_meas_reporting_interval_req_t));

    /* This function parses son_oam_anr_modify_meas_reporting_interval_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_modify_meas_reporting_interval_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_modify_meas_reporting_interval_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_anr_modify_meas_reporting_interval_req->reporting_interval, p_src + *p_length_read, "reporting_interval");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_modify_meas_reporting_interval_req->reporting_interval < 60) || (p_son_oam_anr_modify_meas_reporting_interval_req->reporting_interval > 1440))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_modify_meas_reporting_interval_req->reporting_interval] should be in range [60 <= %d <= 1440] !", p_son_oam_anr_modify_meas_reporting_interval_req->reporting_interval);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_modify_meas_reporting_interval_resp
(
    son_oam_anr_modify_meas_reporting_interval_resp_t *p_son_oam_anr_modify_meas_reporting_interval_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_modify_meas_reporting_interval_resp, 0, sizeof(son_oam_anr_modify_meas_reporting_interval_resp_t));

    /* This function parses son_oam_anr_modify_meas_reporting_interval_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_modify_meas_reporting_interval_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_modify_meas_reporting_interval_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_modify_meas_reporting_interval_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_modify_meas_reporting_interval_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_modify_meas_reporting_interval_resp->result] should be in range [%d <= 1] !", p_son_oam_anr_modify_meas_reporting_interval_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_modify_meas_reporting_interval_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_pci_conflict_enable_disable_req
(
    son_oam_pci_conflict_enable_disable_req_t *p_son_oam_pci_conflict_enable_disable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_pci_conflict_enable_disable_req, 0, sizeof(son_oam_pci_conflict_enable_disable_req_t));

    /* This function parses son_oam_pci_conflict_enable_disable_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_pci_conflict_enable_disable_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_pci_conflict_enable_disable_req->pci_mode, p_src + *p_length_read, "pci_mode");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pci_conflict_enable_disable_req->pci_mode > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pci_conflict_enable_disable_req->pci_mode] should be in range [%d <= 1] !", p_son_oam_pci_conflict_enable_disable_req->pci_mode);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_pci_conflict_enable_disable_resp
(
    son_oam_pci_conflict_enable_disable_resp_t *p_son_oam_pci_conflict_enable_disable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_pci_conflict_enable_disable_resp, 0, sizeof(son_oam_pci_conflict_enable_disable_resp_t));

    /* This function parses son_oam_pci_conflict_enable_disable_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_pci_conflict_enable_disable_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_pci_conflict_enable_disable_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pci_conflict_enable_disable_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pci_conflict_enable_disable_resp->result] should be in range [%d <= 1] !", p_son_oam_pci_conflict_enable_disable_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_pci_conflict_enable_disable_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_pci_selection_req
(
    son_oam_pci_selection_req_t *p_son_oam_pci_selection_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_pci_selection_req, 0, sizeof(son_oam_pci_selection_req_t));

    /* This function parses son_oam_pci_selection_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_pci_selection_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_pci_selection_req->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_pci_selection_req->pci_list_size, p_src + *p_length_read, "pci_list_size");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)(p_son_oam_pci_selection_req->pci_list_size * sizeof(p_son_oam_pci_selection_req->pci_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_pci_selection_req->pci_list_size; loop++)
        {
            son_cp_unpack_U16((void*)&p_son_oam_pci_selection_req->pci_list[loop], (void*)(p_src + *p_length_read), "pci_list[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_pci_selection_req->meas_bw_list_size, p_src + *p_length_read, "meas_bw_list_size");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)(p_son_oam_pci_selection_req->meas_bw_list_size * sizeof(p_son_oam_pci_selection_req->meas_bw_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_pci_selection_req->meas_bw_list_size; loop++)
        {
            son_cp_unpack_U8((void*)&p_son_oam_pci_selection_req->meas_bw_list[loop], (void*)(p_src + *p_length_read), "meas_bw_list[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_pci_selection_resp
(
    son_oam_pci_selection_resp_t *p_son_oam_pci_selection_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_pci_selection_resp, 0, sizeof(son_oam_pci_selection_resp_t));

    /* This function parses son_oam_pci_selection_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_pci_selection_resp");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_pci_selection_resp->selected_pci, p_src + *p_length_read, "selected_pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pci_selection_resp->selected_pci > 504))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pci_selection_resp->selected_pci] should be in range [%d <= 504] !", p_son_oam_pci_selection_resp->selected_pci);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_pci_selection_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pci_selection_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pci_selection_resp->result] should be in range [%d <= 1] !", p_son_oam_pci_selection_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_pci_selection_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_pci_reselection_req
(
    son_oam_pci_reselection_req_t *p_son_oam_pci_reselection_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_pci_reselection_req, 0, sizeof(son_oam_pci_reselection_req_t));

    /* This function parses son_oam_pci_reselection_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_pci_reselection_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_pci_reselection_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_pci_reselection_req->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_pci_reselection_req->level, p_src + *p_length_read, "level");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pci_reselection_req->level > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pci_reselection_req->level] should be in range [%d <= 1] !", p_son_oam_pci_reselection_req->level);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_pci_reselection_req->pci_list_size, p_src + *p_length_read, "pci_list_size");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)(p_son_oam_pci_reselection_req->pci_list_size * sizeof(p_son_oam_pci_reselection_req->pci_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_pci_reselection_req->pci_list_size; loop++)
        {
            son_cp_unpack_U16((void*)&p_son_oam_pci_reselection_req->pci_list[loop], (void*)(p_src + *p_length_read), "pci_list[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_pci_reselection_resp
(
    son_oam_pci_reselection_resp_t *p_son_oam_pci_reselection_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_pci_reselection_resp, 0, sizeof(son_oam_pci_reselection_resp_t));

    /* This function parses son_oam_pci_reselection_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_pci_reselection_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_pci_reselection_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_pci_reselection_resp->reselected_pci, p_src + *p_length_read, "reselected_pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pci_reselection_resp->reselected_pci > 504))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pci_reselection_resp->reselected_pci] should be in range [%d <= 504] !", p_son_oam_pci_reselection_resp->reselected_pci);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_pci_reselection_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pci_reselection_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pci_reselection_resp->result] should be in range [%d <= 1] !", p_son_oam_pci_reselection_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_pci_reselection_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_pci_conflict_confusion_ind
(
    son_oam_pci_conflict_confusion_ind_t *p_son_oam_pci_conflict_confusion_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_pci_conflict_confusion_ind, 0, sizeof(son_oam_pci_conflict_confusion_ind_t));

    /* This function parses son_oam_pci_conflict_confusion_ind */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_pci_conflict_confusion_ind");



    if (SON_FAILURE == son_parse_son_global_cell_id(
        &p_son_oam_pci_conflict_confusion_ind->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_global_cell_id(
        &p_son_oam_pci_conflict_confusion_ind->cgi_1,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_global_cell_id(
        &p_son_oam_pci_conflict_confusion_ind->cgi_2,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_pci_conflict_confusion_ind->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pci_conflict_confusion_ind->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pci_conflict_confusion_ind->pci] should be in range [%d <= 503] !", p_son_oam_pci_conflict_confusion_ind->pci);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_pci_conflict_confusion_ind->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_pci_conflict_confusion_ind->cause > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_pci_conflict_confusion_ind->cause] should be in range [%d <= 3] !", p_son_oam_pci_conflict_confusion_ind->cause);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_carrier_freq_selection_req
(
    son_oam_carrier_freq_selection_req_t *p_son_oam_carrier_freq_selection_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_carrier_freq_selection_req, 0, sizeof(son_oam_carrier_freq_selection_req_t));

    /* This function parses son_oam_carrier_freq_selection_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_carrier_freq_selection_req");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_carrier_freq_selection_req->meas_bandwidth, p_src + *p_length_read, "meas_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_carrier_freq_selection_req->meas_bandwidth < 6) || (p_son_oam_carrier_freq_selection_req->meas_bandwidth > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_carrier_freq_selection_req->meas_bandwidth] should be in range [6 <= %d <= 100] !", p_son_oam_carrier_freq_selection_req->meas_bandwidth);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_carrier_freq_selection_req->num_earfcn, p_src + *p_length_read, "num_earfcn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_carrier_freq_selection_req->num_earfcn > SON_MAX_NO_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_carrier_freq_selection_req->num_earfcn] should be in range [%d <= SON_MAX_NO_EARFCN] !", p_son_oam_carrier_freq_selection_req->num_earfcn);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_oam_carrier_freq_selection_req->num_earfcn * sizeof(p_son_oam_carrier_freq_selection_req->earfcn[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_carrier_freq_selection_req->num_earfcn; loop++)
        {
            son_cp_unpack_U32((void*)&p_son_oam_carrier_freq_selection_req->earfcn[loop], (void*)(p_src + *p_length_read), "earfcn[]");
            *p_length_read += sizeof(U32);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_carrier_freq_selection_resp
(
    son_oam_carrier_freq_selection_resp_t *p_son_oam_carrier_freq_selection_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_carrier_freq_selection_resp, 0, sizeof(son_oam_carrier_freq_selection_resp_t));

    /* This function parses son_oam_carrier_freq_selection_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_carrier_freq_selection_resp");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_carrier_freq_selection_resp->selected_carrier_freq, p_src + *p_length_read, "selected_carrier_freq");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_carrier_freq_selection_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_carrier_freq_selection_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_carrier_freq_selection_resp->result] should be in range [%d <= 1] !", p_son_oam_carrier_freq_selection_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_carrier_freq_selection_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_set_nw_scan_config_cmd
(
    son_oam_set_nw_scan_config_cmd_t *p_son_oam_set_nw_scan_config_cmd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_set_nw_scan_config_cmd, 0, sizeof(son_oam_set_nw_scan_config_cmd_t));

    /* This function parses son_oam_set_nw_scan_config_cmd */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_set_nw_scan_config_cmd");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_set_nw_scan_config_cmd->force_scan_on_off, p_src + *p_length_read, "force_scan_on_off");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_set_nw_scan_config_cmd->force_scan_on_off > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_set_nw_scan_config_cmd->force_scan_on_off] should be in range [%d <= 1] !", p_son_oam_set_nw_scan_config_cmd->force_scan_on_off);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_set_nw_scan_config_cmd->scan_time_interval, p_src + *p_length_read, "scan_time_interval");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_cell_switch_on_off_ind
(
    son_oam_es_cell_switch_on_off_ind_t *p_son_oam_es_cell_switch_on_off_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_cell_switch_on_off_ind, 0, sizeof(son_oam_es_cell_switch_on_off_ind_t));

    /* This function parses son_oam_es_cell_switch_on_off_ind */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_cell_switch_on_off_ind");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_es_cell_switch_on_off_ind->cell_on_status_list_size, p_src + *p_length_read, "cell_on_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_es_cell_switch_on_off_ind->cell_on_status_list_size < 1) || (p_son_oam_es_cell_switch_on_off_ind->cell_on_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_cell_switch_on_off_ind->cell_on_status_list_size] should be in range [1 <= %d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_es_cell_switch_on_off_ind->cell_on_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_es_cell_switch_on_off_ind->cell_on_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_on_off_status(
                &p_son_oam_es_cell_switch_on_off_ind->cell_on_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cell_on_off_status
(
    son_cell_on_off_status_t *p_son_cell_on_off_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_cell_on_off_status, 0, sizeof(son_cell_on_off_status_t));

    /* This function parses son_cell_on_off_status */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cell_on_off_status");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_cell_on_off_status->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cell_on_off_status->switch_on_off_cause, p_src + *p_length_read, "switch_on_off_cause");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cell_on_off_status->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cell_on_off_status->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell_on_off_status->result] should be in range [%d <= 1] !", p_son_cell_on_off_status->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cell_on_off_status->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_delete_from_remove_list_req
(
    son_oam_nr_delete_from_remove_list_req_t *p_son_oam_nr_delete_from_remove_list_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_delete_from_remove_list_req, 0, sizeof(son_oam_nr_delete_from_remove_list_req_t));

    /* This function parses son_oam_nr_delete_from_remove_list_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_delete_from_remove_list_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_nr_delete_from_remove_list_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_nr_delete_from_remove_list_req->trgt_cgi_list_size, p_src + *p_length_read, "trgt_cgi_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_nr_delete_from_remove_list_req->trgt_cgi_list_size < 1) || (p_son_oam_nr_delete_from_remove_list_req->trgt_cgi_list_size > SON_MAX_NO_NEIGHBOUR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_delete_from_remove_list_req->trgt_cgi_list_size] should be in range [1 <= %d <= SON_MAX_NO_NEIGHBOUR_CELLS] !", p_son_oam_nr_delete_from_remove_list_req->trgt_cgi_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_delete_from_remove_list_req->trgt_cgi_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_global_cell_id(
                &p_son_oam_nr_delete_from_remove_list_req->trgt_cgi_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_delete_from_remove_list_resp
(
    son_oam_nr_delete_from_remove_list_resp_t *p_son_oam_nr_delete_from_remove_list_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_delete_from_remove_list_resp, 0, sizeof(son_oam_nr_delete_from_remove_list_resp_t));

    /* This function parses son_oam_nr_delete_from_remove_list_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_delete_from_remove_list_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_nr_delete_from_remove_list_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_delete_from_remove_list_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_delete_from_remove_list_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_delete_from_remove_list_resp->result] should be in range [%d <= 2] !", p_son_oam_nr_delete_from_remove_list_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_delete_from_remove_list_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_nr_delete_from_remove_list_resp->trgt_cell_status_list_size, p_src + *p_length_read, "trgt_cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_delete_from_remove_list_resp->trgt_cell_status_list_size > SON_MAX_NO_NEIGHBOUR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_delete_from_remove_list_resp->trgt_cell_status_list_size] should be in range [%d <= SON_MAX_NO_NEIGHBOUR_CELLS] !", p_son_oam_nr_delete_from_remove_list_resp->trgt_cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_delete_from_remove_list_resp->trgt_cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_nbr_cell_status(
                &p_son_oam_nr_delete_from_remove_list_resp->trgt_cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_modify_removal_attributes_req
(
    son_oam_anr_modify_removal_attributes_req_t *p_son_oam_anr_modify_removal_attributes_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_modify_removal_attributes_req, 0, sizeof(son_oam_anr_modify_removal_attributes_req_t));

    /* This function parses son_oam_anr_modify_removal_attributes_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_modify_removal_attributes_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_modify_removal_attributes_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_neighbor_removal_info(
        &p_son_oam_anr_modify_removal_attributes_req->removal_criteria,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_modify_removal_attributes_resp
(
    son_oam_anr_modify_removal_attributes_resp_t *p_son_oam_anr_modify_removal_attributes_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_modify_removal_attributes_resp, 0, sizeof(son_oam_anr_modify_removal_attributes_resp_t));

    /* This function parses son_oam_anr_modify_removal_attributes_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_modify_removal_attributes_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_modify_removal_attributes_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_modify_removal_attributes_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_modify_removal_attributes_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_modify_removal_attributes_resp->result] should be in range [%d <= 1] !", p_son_oam_anr_modify_removal_attributes_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_modify_removal_attributes_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_modify_pci_confusion_cfg_req
(
    son_oam_anr_modify_pci_confusion_cfg_req_t *p_son_oam_anr_modify_pci_confusion_cfg_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_modify_pci_confusion_cfg_req, 0, sizeof(son_oam_anr_modify_pci_confusion_cfg_req_t));

    /* This function parses son_oam_anr_modify_pci_confusion_cfg_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_modify_pci_confusion_cfg_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_modify_pci_confusion_cfg_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_neighbor_pci_confusion_info(
        &p_son_oam_anr_modify_pci_confusion_cfg_req->pci_confusion_cfg,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_modify_pci_confusion_cfg_resp
(
    son_oam_anr_modify_pci_confusion_cfg_resp_t *p_son_oam_anr_modify_pci_confusion_cfg_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_modify_pci_confusion_cfg_resp, 0, sizeof(son_oam_anr_modify_pci_confusion_cfg_resp_t));

    /* This function parses son_oam_anr_modify_pci_confusion_cfg_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_modify_pci_confusion_cfg_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_modify_pci_confusion_cfg_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_modify_pci_confusion_cfg_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_modify_pci_confusion_cfg_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_modify_pci_confusion_cfg_resp->result] should be in range [%d <= 1] !", p_son_oam_anr_modify_pci_confusion_cfg_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_modify_pci_confusion_cfg_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_modify_attribute_req
(
    son_oam_anr_modify_attribute_req_t *p_son_oam_anr_modify_attribute_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_modify_attribute_req, 0, sizeof(son_oam_anr_modify_attribute_req_t));

    /* This function parses son_oam_anr_modify_attribute_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_modify_attribute_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_modify_attribute_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_oam_anr_modify_attribute_info(
        &p_son_oam_anr_modify_attribute_req->anr_attribute,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_oam_anr_modify_attribute_info
(
    son_oam_anr_modify_attribute_info_t *p_son_oam_anr_modify_attribute_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_modify_attribute_info, 0, sizeof(son_oam_anr_modify_attribute_info_t));

    /* This function parses son_oam_anr_modify_attribute_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_modify_attribute_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_modify_attribute_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_anr_modify_attribute_info->bitmask & SON_MODIFY_DL_ANR_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_modify_attribute_info->ue_throughput_dl_anr_threshold, p_src + *p_length_read, "ue_throughput_dl_anr_threshold");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_modify_attribute_info->ue_throughput_dl_anr_threshold < 1) || (p_son_oam_anr_modify_attribute_info->ue_throughput_dl_anr_threshold > 255))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_modify_attribute_info->ue_throughput_dl_anr_threshold] should be in range [1 <= %d <= 255] !", p_son_oam_anr_modify_attribute_info->ue_throughput_dl_anr_threshold);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_anr_modify_attribute_info->bitmask & SON_MODIFY_UL_ANR_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_modify_attribute_info->ue_throughput_ul_anr_threshold, p_src + *p_length_read, "ue_throughput_ul_anr_threshold");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_modify_attribute_info->ue_throughput_ul_anr_threshold < 1) || (p_son_oam_anr_modify_attribute_info->ue_throughput_ul_anr_threshold > 255))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_modify_attribute_info->ue_throughput_ul_anr_threshold] should be in range [1 <= %d <= 255] !", p_son_oam_anr_modify_attribute_info->ue_throughput_ul_anr_threshold);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_modify_attribute_resp
(
    son_oam_anr_modify_attribute_resp_t *p_son_oam_anr_modify_attribute_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_modify_attribute_resp, 0, sizeof(son_oam_anr_modify_attribute_resp_t));

    /* This function parses son_oam_anr_modify_attribute_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_modify_attribute_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_modify_attribute_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_modify_attribute_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_modify_attribute_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_modify_attribute_resp->result] should be in range [%d <= 1] !", p_son_oam_anr_modify_attribute_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_modify_attribute_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_enable_disable_x2_req
(
    son_oam_enable_disable_x2_req_t *p_son_oam_enable_disable_x2_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_enable_disable_x2_req, 0, sizeof(son_oam_enable_disable_x2_req_t));

    /* This function parses son_oam_enable_disable_x2_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_enable_disable_x2_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_enable_disable_x2_req->x2_enabled, p_src + *p_length_read, "x2_enabled");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_enable_disable_x2_req->x2_enabled > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_enable_disable_x2_req->x2_enabled] should be in range [%d <= 1] !", p_son_oam_enable_disable_x2_req->x2_enabled);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_enable_disable_x2_resp
(
    son_oam_enable_disable_x2_resp_t *p_son_oam_enable_disable_x2_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_enable_disable_x2_resp, 0, sizeof(son_oam_enable_disable_x2_resp_t));

    /* This function parses son_oam_enable_disable_x2_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_enable_disable_x2_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_enable_disable_x2_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_enable_disable_x2_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_enable_disable_x2_resp->result] should be in range [%d <= 1] !", p_son_oam_enable_disable_x2_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_enable_disable_x2_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_x2_link_up_ind
(
    son_oam_x2_link_up_ind_t *p_son_oam_x2_link_up_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_x2_link_up_ind, 0, sizeof(son_oam_x2_link_up_ind_t));

    /* This function parses son_oam_x2_link_up_ind */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_x2_link_up_ind");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_x2_link_up_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_global_enb_id(
        &p_son_oam_x2_link_up_ind->nbr_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_oam_x2_link_up_ind->bitmask & SON_SERVED_CELL_INFO_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_served_cell_info_arr(
        &p_son_oam_x2_link_up_ind->served_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_oam_x2_link_up_ind->bitmask & SON_GU_GROUP_ID_LIST_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_gu_group_id_list(
        &p_son_oam_x2_link_up_ind->gu_group_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_global_enb_id
(
    son_global_enb_id_t *p_son_global_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_global_enb_id, 0, sizeof(son_global_enb_id_t));

    /* This function parses son_global_enb_id */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_global_enb_id");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_global_enb_id->enb_type, p_src + *p_length_read, "enb_type");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_global_enb_id->enb_type < 1) || (p_son_global_enb_id->enb_type > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_global_enb_id->enb_type] should be in range [1 <= %d <= 2] !", p_son_global_enb_id->enb_type);
        return SON_FAILURE;
    }



    if (SON_FAILURE == son_parse_son_cell_plmn_info(
        &p_son_global_enb_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_son_global_enb_id->enb_id) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_global_enb_id->enb_id); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_global_enb_id->enb_id[loop], (void*)(p_src + *p_length_read), "enb_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_served_cell_info
(
    son_served_cell_info_t *p_son_served_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_served_cell_info, 0, sizeof(son_served_cell_info_t));

    /* This function parses son_served_cell_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_served_cell_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_served_cell_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_served_cell_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_served_cell_info->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_served_cell_info->pci] should be in range [%d <= 503] !", p_son_served_cell_info->pci);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_served_cell_info->cell_id, p_src + *p_length_read, "cell_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_served_cell_info->cell_id > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_served_cell_info->cell_id] should be in range [%d <= 7] !", p_son_served_cell_info->cell_id);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_son_served_cell_info->tac) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_served_cell_info->tac); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_served_cell_info->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_nbour_cell_info
(
    son_nbour_cell_info_t *p_son_nbour_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_nbour_cell_info, 0, sizeof(son_nbour_cell_info_t));

    /* This function parses son_nbour_cell_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_nbour_cell_info");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_nbour_cell_info->num_neighbour_cell, p_src + *p_length_read, "num_neighbour_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_nbour_cell_info->num_neighbour_cell < 1) || (p_son_nbour_cell_info->num_neighbour_cell > SON_MAX_NO_INTRA_NBR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_nbour_cell_info->num_neighbour_cell] should be in range [1 <= %d <= SON_MAX_NO_INTRA_NBR_CELLS] !", p_son_nbour_cell_info->num_neighbour_cell);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_nbour_cell_info->num_neighbour_cell; loop++)
        {
            if (SON_FAILURE == son_parse_son_intra_rat_neighbor_info(
                &p_son_nbour_cell_info->neighbour_info_arr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_served_cell
(
    son_served_cell_t *p_son_served_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_served_cell, 0, sizeof(son_served_cell_t));

    /* This function parses son_served_cell */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_served_cell");



    if (SON_FAILURE == son_parse_son_served_cell_info(
        &p_son_served_cell->served_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_nbour_cell_info(
        &p_son_served_cell->nbour_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_served_cell_info_arr
(
    son_served_cell_info_arr_t *p_son_served_cell_info_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_served_cell_info_arr, 0, sizeof(son_served_cell_info_arr_t));

    /* This function parses son_served_cell_info_arr */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_served_cell_info_arr");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_served_cell_info_arr->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_served_cell_info_arr->num_served_cell < 1) || (p_son_served_cell_info_arr->num_served_cell > SON_MAX_SERVED_CELLS_AT_NBR_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_served_cell_info_arr->num_served_cell] should be in range [1 <= %d <= SON_MAX_SERVED_CELLS_AT_NBR_ENB] !", p_son_served_cell_info_arr->num_served_cell);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_served_cell_info_arr->num_served_cell; loop++)
        {
            if (SON_FAILURE == son_parse_son_served_cell(
                &p_son_served_cell_info_arr->cell_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_gu_group_id
(
    son_gu_group_id_t *p_son_gu_group_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_gu_group_id, 0, sizeof(son_gu_group_id_t));

    /* This function parses son_gu_group_id */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_gu_group_id");



    if (SON_FAILURE == son_parse_son_cell_plmn_info(
        &p_son_gu_group_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_son_gu_group_id->mme_group_id) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_gu_group_id->mme_group_id); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_gu_group_id->mme_group_id[loop], (void*)(p_src + *p_length_read), "mme_group_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_gu_group_id_list
(
    son_gu_group_id_list_t *p_son_gu_group_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_gu_group_id_list, 0, sizeof(son_gu_group_id_list_t));

    /* This function parses son_gu_group_id_list */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_gu_group_id_list");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_gu_group_id_list->num_gu_group_id, p_src + *p_length_read, "num_gu_group_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_gu_group_id_list->num_gu_group_id < 1) || (p_son_gu_group_id_list->num_gu_group_id > SON_MAX_ENB_POOLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_gu_group_id_list->num_gu_group_id] should be in range [1 <= %d <= SON_MAX_ENB_POOLS] !", p_son_gu_group_id_list->num_gu_group_id);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_gu_group_id_list->num_gu_group_id; loop++)
        {
            if (SON_FAILURE == son_parse_son_gu_group_id(
                &p_son_gu_group_id_list->gu_id_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_x2_link_down_ind
(
    son_oam_x2_link_down_ind_t *p_son_oam_x2_link_down_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_x2_link_down_ind, 0, sizeof(son_oam_x2_link_down_ind_t));

    /* This function parses son_oam_x2_link_down_ind */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_x2_link_down_ind");



    if (SON_FAILURE == son_parse_son_global_enb_id(
        &p_son_oam_x2_link_down_ind->nbr_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_enable_disable_limited_mode_req
(
    son_oam_anr_enable_disable_limited_mode_req_t *p_son_oam_anr_enable_disable_limited_mode_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_enable_disable_limited_mode_req, 0, sizeof(son_oam_anr_enable_disable_limited_mode_req_t));

    /* This function parses son_oam_anr_enable_disable_limited_mode_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_enable_disable_limited_mode_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_enable_disable_limited_mode_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_enable_disable_limited_mode_req->limited_mode, p_src + *p_length_read, "limited_mode");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_enable_disable_limited_mode_req->limited_mode > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_enable_disable_limited_mode_req->limited_mode] should be in range [%d <= 1] !", p_son_oam_anr_enable_disable_limited_mode_req->limited_mode);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_enable_disable_limited_mode_resp
(
    son_oam_anr_enable_disable_limited_mode_resp_t *p_son_oam_anr_enable_disable_limited_mode_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_enable_disable_limited_mode_resp, 0, sizeof(son_oam_anr_enable_disable_limited_mode_resp_t));

    /* This function parses son_oam_anr_enable_disable_limited_mode_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_enable_disable_limited_mode_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_enable_disable_limited_mode_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_enable_disable_limited_mode_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_enable_disable_limited_mode_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_enable_disable_limited_mode_resp->result] should be in range [%d <= 1] !", p_son_oam_anr_enable_disable_limited_mode_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_enable_disable_limited_mode_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_limited_mode_pci_info
(
    son_oam_anr_limited_mode_pci_info_t *p_son_oam_anr_limited_mode_pci_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_limited_mode_pci_info, 0, sizeof(son_oam_anr_limited_mode_pci_info_t));

    /* This function parses son_oam_anr_limited_mode_pci_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_limited_mode_pci_info");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_limited_mode_pci_info->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_limited_mode_pci_info->pci_earfcn_list_size, p_src + *p_length_read, "pci_earfcn_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_limited_mode_pci_info->pci_earfcn_list_size > SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_limited_mode_pci_info->pci_earfcn_list_size] should be in range [%d <= SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS] !", p_son_oam_anr_limited_mode_pci_info->pci_earfcn_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_anr_limited_mode_pci_info->pci_earfcn_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_limited_mode_eutran_cell_info(
                &p_son_oam_anr_limited_mode_pci_info->pci_earfcn_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_limited_mode_eutran_cell_info
(
    son_limited_mode_eutran_cell_info_t *p_son_limited_mode_eutran_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_limited_mode_eutran_cell_info, 0, sizeof(son_limited_mode_eutran_cell_info_t));

    /* This function parses son_limited_mode_eutran_cell_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_limited_mode_eutran_cell_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_limited_mode_eutran_cell_info->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_limited_mode_eutran_cell_info->pci_list_size, p_src + *p_length_read, "pci_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_limited_mode_eutran_cell_info->pci_list_size > SON_MAX_PCI))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_limited_mode_eutran_cell_info->pci_list_size] should be in range [%d <= SON_MAX_PCI] !", p_son_limited_mode_eutran_cell_info->pci_list_size);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_limited_mode_eutran_cell_info->pci_list_size * sizeof(p_son_limited_mode_eutran_cell_info->pci_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_limited_mode_eutran_cell_info->pci_list_size; loop++)
        {
            son_cp_unpack_U16((void*)&p_son_limited_mode_eutran_cell_info->pci_list[loop], (void*)(p_src + *p_length_read), "pci_list[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_enb_add_req
(
    son_oam_nr_enb_add_req_t *p_son_oam_nr_enb_add_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_enb_add_req, 0, sizeof(son_oam_nr_enb_add_req_t));

    /* This function parses son_oam_nr_enb_add_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_enb_add_req");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_nr_enb_add_req->nbr_enb_list_size, p_src + *p_length_read, "nbr_enb_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_nr_enb_add_req->nbr_enb_list_size < 1) || (p_son_oam_nr_enb_add_req->nbr_enb_list_size > SON_MAX_NO_NEIGHBOUR_ENBS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_add_req->nbr_enb_list_size] should be in range [1 <= %d <= SON_MAX_NO_NEIGHBOUR_ENBS] !", p_son_oam_nr_enb_add_req->nbr_enb_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_enb_add_req->nbr_enb_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_neighbor_enb_info(
                &p_son_oam_nr_enb_add_req->nbr_enb_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_tac
(
    son_tac_t *p_son_tac,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_tac, 0, sizeof(son_tac_t));

    /* This function parses son_tac */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_tac");

    if (*p_length_read + (S32)sizeof(p_son_tac->tac) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_tac->tac); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_tac->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_enb_tnl_address
(
    son_enb_tnl_address_t *p_son_enb_tnl_address,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_enb_tnl_address, 0, sizeof(son_enb_tnl_address_t));

    /* This function parses son_enb_tnl_address */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_enb_tnl_address");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_enb_tnl_address->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_enb_tnl_address->bitmask & IPV4_ADD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_enb_tnl_address->ipv4_add) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_enb_tnl_address->ipv4_add); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_enb_tnl_address->ipv4_add[loop], (void*)(p_src + *p_length_read), "ipv4_add[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_enb_tnl_address->bitmask & IPV6_ADD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_enb_tnl_address->ipv6_add) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_enb_tnl_address->ipv6_add); loop++)
        {
            son_cp_unpack_U8((void*)&p_son_enb_tnl_address->ipv6_add[loop], (void*)(p_src + *p_length_read), "ipv6_add[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_neighbor_enb_info
(
    son_neighbor_enb_info_t *p_son_neighbor_enb_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_neighbor_enb_info, 0, sizeof(son_neighbor_enb_info_t));

    /* This function parses son_neighbor_enb_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_neighbor_enb_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_neighbor_enb_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_global_enb_id(
        &p_son_neighbor_enb_info->enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_neighbor_enb_info->bitmask & SON_ANR_ENB_TAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_neighbor_enb_info->tac_list_size, p_src + *p_length_read, "tac_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_neighbor_enb_info->tac_list_size > SON_MAX_SERVED_CELLS_AT_NBR_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_neighbor_enb_info->tac_list_size] should be in range [%d <= SON_MAX_SERVED_CELLS_AT_NBR_ENB] !", p_son_neighbor_enb_info->tac_list_size);
        return SON_FAILURE;
    }
    }

    if (p_son_neighbor_enb_info->bitmask & SON_ANR_ENB_TAC_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_neighbor_enb_info->tac_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_tac(
                &p_son_neighbor_enb_info->tac_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_son_neighbor_enb_info->bitmask & SON_ANR_ENB_TNL_ADD_LIST_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_neighbor_enb_info->enb_tnl_address_list_size, p_src + *p_length_read, "enb_tnl_address_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_neighbor_enb_info->enb_tnl_address_list_size < 1) || (p_son_neighbor_enb_info->enb_tnl_address_list_size > SON_MAX_NO_ENB_X2_TLAS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_neighbor_enb_info->enb_tnl_address_list_size] should be in range [1 <= %d <= SON_MAX_NO_ENB_X2_TLAS] !", p_son_neighbor_enb_info->enb_tnl_address_list_size);
        return SON_FAILURE;
    }
    }

    if (p_son_neighbor_enb_info->bitmask & SON_ANR_ENB_TNL_ADD_LIST_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_neighbor_enb_info->enb_tnl_address_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_enb_tnl_address(
                &p_son_neighbor_enb_info->enb_tnl_address_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_son_neighbor_enb_info->bitmask & SON_ANR_ENB_X2_HO_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_neighbor_enb_info->x2_status, p_src + *p_length_read, "x2_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_neighbor_enb_info->x2_status < 1) || (p_son_neighbor_enb_info->x2_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_neighbor_enb_info->x2_status] should be in range [1 <= %d <= 2] !", p_son_neighbor_enb_info->x2_status);
        return SON_FAILURE;
    }
    }

    if (p_son_neighbor_enb_info->bitmask & SON_ANR_ENB_CUR_X2_CONN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_neighbor_enb_info->x2_connection_status, p_src + *p_length_read, "x2_connection_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_neighbor_enb_info->x2_connection_status < 1) || (p_son_neighbor_enb_info->x2_connection_status > 10))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_neighbor_enb_info->x2_connection_status] should be in range [1 <= %d <= 10] !", p_son_neighbor_enb_info->x2_connection_status);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_enb_add_resp
(
    son_oam_nr_enb_add_resp_t *p_son_oam_nr_enb_add_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_enb_add_resp, 0, sizeof(son_oam_nr_enb_add_resp_t));

    /* This function parses son_oam_nr_enb_add_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_enb_add_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_enb_add_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_enb_add_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_add_resp->result] should be in range [%d <= 2] !", p_son_oam_nr_enb_add_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_enb_add_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_nr_enb_add_resp->nbr_enb_status_list_size, p_src + *p_length_read, "nbr_enb_status_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_enb_add_resp->nbr_enb_status_list_size > SON_MAX_NO_NEIGHBOUR_ENBS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_add_resp->nbr_enb_status_list_size] should be in range [%d <= SON_MAX_NO_NEIGHBOUR_ENBS] !", p_son_oam_nr_enb_add_resp->nbr_enb_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_enb_add_resp->nbr_enb_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_enb_status(
                &p_son_oam_nr_enb_add_resp->nbr_enb_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_enb_status
(
    son_enb_status_t *p_son_enb_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_enb_status, 0, sizeof(son_enb_status_t));

    /* This function parses son_enb_status */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_enb_status");



    if (SON_FAILURE == son_parse_son_global_enb_id(
        &p_son_enb_status->enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_enb_status->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_enb_update_req
(
    son_oam_nr_enb_update_req_t *p_son_oam_nr_enb_update_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_enb_update_req, 0, sizeof(son_oam_nr_enb_update_req_t));

    /* This function parses son_oam_nr_enb_update_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_enb_update_req");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_nr_enb_update_req->nbr_enb_list_size, p_src + *p_length_read, "nbr_enb_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_nr_enb_update_req->nbr_enb_list_size < 1) || (p_son_oam_nr_enb_update_req->nbr_enb_list_size > SON_MAX_NO_NEIGHBOUR_ENBS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_update_req->nbr_enb_list_size] should be in range [1 <= %d <= SON_MAX_NO_NEIGHBOUR_ENBS] !", p_son_oam_nr_enb_update_req->nbr_enb_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_enb_update_req->nbr_enb_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_neighbor_enb_info(
                &p_son_oam_nr_enb_update_req->nbr_enb_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_enb_update_resp
(
    son_oam_nr_enb_update_resp_t *p_son_oam_nr_enb_update_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_enb_update_resp, 0, sizeof(son_oam_nr_enb_update_resp_t));

    /* This function parses son_oam_nr_enb_update_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_enb_update_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_enb_update_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_enb_update_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_update_resp->result] should be in range [%d <= 2] !", p_son_oam_nr_enb_update_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_enb_update_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_nr_enb_update_resp->nbr_enb_status_list_size, p_src + *p_length_read, "nbr_enb_status_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_enb_update_resp->nbr_enb_status_list_size > SON_MAX_NO_NEIGHBOUR_ENBS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_update_resp->nbr_enb_status_list_size] should be in range [%d <= SON_MAX_NO_NEIGHBOUR_ENBS] !", p_son_oam_nr_enb_update_resp->nbr_enb_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_enb_update_resp->nbr_enb_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_enb_status(
                &p_son_oam_nr_enb_update_resp->nbr_enb_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_enb_delete_req
(
    son_oam_nr_enb_delete_req_t *p_son_oam_nr_enb_delete_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_enb_delete_req, 0, sizeof(son_oam_nr_enb_delete_req_t));

    /* This function parses son_oam_nr_enb_delete_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_enb_delete_req");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_nr_enb_delete_req->nbr_enb_id_list_size, p_src + *p_length_read, "nbr_enb_id_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_nr_enb_delete_req->nbr_enb_id_list_size < 1) || (p_son_oam_nr_enb_delete_req->nbr_enb_id_list_size > SON_MAX_NO_NEIGHBOUR_ENBS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_delete_req->nbr_enb_id_list_size] should be in range [1 <= %d <= SON_MAX_NO_NEIGHBOUR_ENBS] !", p_son_oam_nr_enb_delete_req->nbr_enb_id_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_enb_delete_req->nbr_enb_id_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_global_enb_id(
                &p_son_oam_nr_enb_delete_req->nbr_enb_id_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_enb_delete_resp
(
    son_oam_nr_enb_delete_resp_t *p_son_oam_nr_enb_delete_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_enb_delete_resp, 0, sizeof(son_oam_nr_enb_delete_resp_t));

    /* This function parses son_oam_nr_enb_delete_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_enb_delete_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_enb_delete_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_enb_delete_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_delete_resp->result] should be in range [%d <= 2] !", p_son_oam_nr_enb_delete_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_enb_delete_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_nr_enb_delete_resp->nbr_enb_status_list_size, p_src + *p_length_read, "nbr_enb_status_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_enb_delete_resp->nbr_enb_status_list_size > SON_MAX_NO_NEIGHBOUR_ENBS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_delete_resp->nbr_enb_status_list_size] should be in range [%d <= SON_MAX_NO_NEIGHBOUR_ENBS] !", p_son_oam_nr_enb_delete_resp->nbr_enb_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_enb_delete_resp->nbr_enb_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_enb_status(
                &p_son_oam_nr_enb_delete_resp->nbr_enb_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_enb_retrieve_req
(
    son_oam_nr_enb_retrieve_req_t *p_son_oam_nr_enb_retrieve_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_enb_retrieve_req, 0, sizeof(son_oam_nr_enb_retrieve_req_t));

    /* This function parses son_oam_nr_enb_retrieve_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_enb_retrieve_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_enb_retrieve_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_nr_enb_retrieve_req->bitmask & SON_RTRV_NBR_ENB_ID_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_global_enb_id(
        &p_son_oam_nr_enb_retrieve_req->nbr_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_oam_nr_enb_retrieve_req->bitmask & SON_RTRV_X2_HO_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_enb_retrieve_req->x2_status, p_src + *p_length_read, "x2_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_nr_enb_retrieve_req->x2_status < 1) || (p_son_oam_nr_enb_retrieve_req->x2_status > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_retrieve_req->x2_status] should be in range [1 <= %d <= 2] !", p_son_oam_nr_enb_retrieve_req->x2_status);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_nr_enb_retrieve_req->bitmask & SON_RTRV_X2_CON_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_enb_retrieve_req->x2_connection_status, p_src + *p_length_read, "x2_connection_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_nr_enb_retrieve_req->x2_connection_status < 1) || (p_son_oam_nr_enb_retrieve_req->x2_connection_status > 10))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_retrieve_req->x2_connection_status] should be in range [1 <= %d <= 10] !", p_son_oam_nr_enb_retrieve_req->x2_connection_status);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_nr_enb_retrieve_resp
(
    son_oam_nr_enb_retrieve_resp_t *p_son_oam_nr_enb_retrieve_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_nr_enb_retrieve_resp, 0, sizeof(son_oam_nr_enb_retrieve_resp_t));

    /* This function parses son_oam_nr_enb_retrieve_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_nr_enb_retrieve_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_enb_retrieve_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_enb_retrieve_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_retrieve_resp->result] should be in range [%d <= 1] !", p_son_oam_nr_enb_retrieve_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_nr_enb_retrieve_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_nr_enb_retrieve_resp->nbr_enb_list_size, p_src + *p_length_read, "nbr_enb_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_nr_enb_retrieve_resp->nbr_enb_list_size > SON_MAX_NO_NEIGHBOUR_ENBS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_nr_enb_retrieve_resp->nbr_enb_list_size] should be in range [%d <= SON_MAX_NO_NEIGHBOUR_ENBS] !", p_son_oam_nr_enb_retrieve_resp->nbr_enb_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_nr_enb_retrieve_resp->nbr_enb_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_neighbor_enb_info(
                &p_son_oam_nr_enb_retrieve_resp->nbr_enb_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_proc_sup_resp
(
    son_oam_proc_sup_resp_t *p_son_oam_proc_sup_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_proc_sup_resp, 0, sizeof(son_oam_proc_sup_resp_t));

    /* This function parses son_oam_proc_sup_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_proc_sup_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_proc_sup_resp->alive_status, p_src + *p_length_read, "alive_status");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_feature_state_change_ind
(
    son_oam_feature_state_change_ind_t *p_son_oam_feature_state_change_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_feature_state_change_ind, 0, sizeof(son_oam_feature_state_change_ind_t));

    /* This function parses son_oam_feature_state_change_ind */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_feature_state_change_ind");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_feature_state_change_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_feature_state_change_ind->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_feature_state_change_ind->module_id, p_src + *p_length_read, "module_id");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_feature_state_change_ind->state, p_src + *p_length_read, "state");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_feature_state_change_ind->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mro_enable_req
(
    son_oam_mro_enable_req_t *p_son_oam_mro_enable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mro_enable_req, 0, sizeof(son_oam_mro_enable_req_t));

    /* This function parses son_oam_mro_enable_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mro_enable_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_enable_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_mro_enable_req->bitmask & SON_MRO_ENABLE_SRV_CGI_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mro_enable_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mro_enable_resp
(
    son_oam_mro_enable_resp_t *p_son_oam_mro_enable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mro_enable_resp, 0, sizeof(son_oam_mro_enable_resp_t));

    /* This function parses son_oam_mro_enable_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mro_enable_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_enable_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_mro_enable_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mro_enable_resp->result] should be in range [%d <= 1] !", p_son_oam_mro_enable_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_enable_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_mro_enable_resp->cell_status_list_size, p_src + *p_length_read, "cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_mro_enable_resp->cell_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mro_enable_resp->cell_status_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_mro_enable_resp->cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_mro_enable_resp->cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_status(
                &p_son_oam_mro_enable_resp->cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mro_disable_req
(
    son_oam_mro_disable_req_t *p_son_oam_mro_disable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mro_disable_req, 0, sizeof(son_oam_mro_disable_req_t));

    /* This function parses son_oam_mro_disable_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mro_disable_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_disable_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_mro_disable_req->bitmask & SON_MRO_DISABLE_SRV_CGI_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mro_disable_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mro_disable_resp
(
    son_oam_mro_disable_resp_t *p_son_oam_mro_disable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mro_disable_resp, 0, sizeof(son_oam_mro_disable_resp_t));

    /* This function parses son_oam_mro_disable_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mro_disable_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_disable_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_mro_disable_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mro_disable_resp->result] should be in range [%d <= 1] !", p_son_oam_mro_disable_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_disable_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_mro_disable_resp->cell_status_list_size, p_src + *p_length_read, "cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_mro_disable_resp->cell_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mro_disable_resp->cell_status_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_mro_disable_resp->cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_mro_disable_resp->cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_status(
                &p_son_oam_mro_disable_resp->cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mro_modify_config_params_req
(
    son_oam_mro_modify_config_params_req_t *p_son_oam_mro_modify_config_params_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mro_modify_config_params_req, 0, sizeof(son_oam_mro_modify_config_params_req_t));

    /* This function parses son_oam_mro_modify_config_params_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mro_modify_config_params_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_modify_config_params_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mro_modify_config_params_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_mro_eutran_config_param(
        &p_son_oam_mro_modify_config_params_req->eutran_config_params,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_mro_eutran_config_param
(
    son_mro_eutran_config_param_t *p_son_mro_eutran_config_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_mro_eutran_config_param, 0, sizeof(son_mro_eutran_config_param_t));

    /* This function parses son_mro_eutran_config_param */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_mro_eutran_config_param");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_mro_eutran_config_param->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_mro_eutran_config_param->bitmask & SON_MRO_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_mro_eutran_config_param->mro_mode, p_src + *p_length_read, "mro_mode");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mro_eutran_config_param->mro_mode > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mro_eutran_config_param->mro_mode] should be in range [%d <= 1] !", p_son_mro_eutran_config_param->mro_mode);
        return SON_FAILURE;
    }
    }

    if (p_son_mro_eutran_config_param->bitmask & SON_MRO_PARAM_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_mro_eutran_config_param->mro_param, p_src + *p_length_read, "mro_param");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mro_eutran_config_param->mro_param > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mro_eutran_config_param->mro_param] should be in range [%d <= 2] !", p_son_mro_eutran_config_param->mro_param);
        return SON_FAILURE;
    }
    }

    if (p_son_mro_eutran_config_param->bitmask & SON_MRO_HO_FAILURE_ACCUMULATION_DURATION_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_mro_eutran_config_param->mro_ho_failure_accumulation_duration, p_src + *p_length_read, "mro_ho_failure_accumulation_duration");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_mro_eutran_config_param->mro_ho_failure_accumulation_duration < 5) || (p_son_mro_eutran_config_param->mro_ho_failure_accumulation_duration > 60))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mro_eutran_config_param->mro_ho_failure_accumulation_duration] should be in range [5 <= %d <= 60] !", p_son_mro_eutran_config_param->mro_ho_failure_accumulation_duration);
        return SON_FAILURE;
    }
    }

    if (p_son_mro_eutran_config_param->bitmask & SON_MRO_DELTA_CIO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_mro_eutran_config_param->delta_cio, p_src + *p_length_read, "delta_cio");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_mro_eutran_config_param->delta_cio < 1) || (p_son_mro_eutran_config_param->delta_cio > 5))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mro_eutran_config_param->delta_cio] should be in range [1 <= %d <= 5] !", p_son_mro_eutran_config_param->delta_cio);
        return SON_FAILURE;
    }
    }

    if (p_son_mro_eutran_config_param->bitmask & SON_MRO_HO_ATTEMPTS_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_mro_eutran_config_param->mro_ho_attempts_threshold, p_src + *p_length_read, "mro_ho_attempts_threshold");
    *p_length_read += sizeof(U16);

    /* Check for correct range [L - lower boundary] */
    if ((p_son_mro_eutran_config_param->mro_ho_attempts_threshold < 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mro_eutran_config_param->mro_ho_attempts_threshold] should be in range [1 <= %d] !", p_son_mro_eutran_config_param->mro_ho_attempts_threshold);
        return SON_FAILURE;
    }
    }

    if (p_son_mro_eutran_config_param->bitmask & SON_MRO_HO_OPTIMIZATION_START_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_mro_eutran_config_param->mro_ho_optimization_start_threshold, p_src + *p_length_read, "mro_ho_optimization_start_threshold");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_mro_eutran_config_param->mro_ho_optimization_start_threshold < 1) || (p_son_mro_eutran_config_param->mro_ho_optimization_start_threshold > 1000))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mro_eutran_config_param->mro_ho_optimization_start_threshold] should be in range [1 <= %d <= 1000] !", p_son_mro_eutran_config_param->mro_ho_optimization_start_threshold);
        return SON_FAILURE;
    }
    }

    if (p_son_mro_eutran_config_param->bitmask & SON_MRO_HO_OPTIMIZATION_STOP_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_mro_eutran_config_param->mro_ho_optimization_stop_threshold, p_src + *p_length_read, "mro_ho_optimization_stop_threshold");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_mro_eutran_config_param->mro_ho_optimization_stop_threshold < 1) || (p_son_mro_eutran_config_param->mro_ho_optimization_stop_threshold > 1000))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mro_eutran_config_param->mro_ho_optimization_stop_threshold] should be in range [1 <= %d <= 1000] !", p_son_mro_eutran_config_param->mro_ho_optimization_stop_threshold);
        return SON_FAILURE;
    }
    }

    if (p_son_mro_eutran_config_param->bitmask & SON_MRO_HO_FAIL_RATE_FOR_OPTIMIZATION_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_mro_eutran_config_param->mro_ho_fail_rate_for_optimization, p_src + *p_length_read, "mro_ho_fail_rate_for_optimization");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_mro_eutran_config_param->mro_ho_fail_rate_for_optimization < 1) || (p_son_mro_eutran_config_param->mro_ho_fail_rate_for_optimization > 1000))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mro_eutran_config_param->mro_ho_fail_rate_for_optimization] should be in range [1 <= %d <= 1000] !", p_son_mro_eutran_config_param->mro_ho_fail_rate_for_optimization);
        return SON_FAILURE;
    }
    }

    if (p_son_mro_eutran_config_param->bitmask & SON_MRO_TTT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_mro_eutran_config_param->ttt, p_src + *p_length_read, "ttt");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mro_eutran_config_param->ttt > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mro_eutran_config_param->ttt] should be in range [%d <= 15] !", p_son_mro_eutran_config_param->ttt);
        return SON_FAILURE;
    }
    }

    if (p_son_mro_eutran_config_param->bitmask & SON_MRO_DELTA_TTT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_mro_eutran_config_param->delta_ttt, p_src + *p_length_read, "delta_ttt");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_mro_eutran_config_param->delta_ttt < 1) || (p_son_mro_eutran_config_param->delta_ttt > 5))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mro_eutran_config_param->delta_ttt] should be in range [1 <= %d <= 5] !", p_son_mro_eutran_config_param->delta_ttt);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mro_modify_config_params_resp
(
    son_oam_mro_modify_config_params_resp_t *p_son_oam_mro_modify_config_params_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mro_modify_config_params_resp, 0, sizeof(son_oam_mro_modify_config_params_resp_t));

    /* This function parses son_oam_mro_modify_config_params_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mro_modify_config_params_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mro_modify_config_params_resp->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_modify_config_params_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_mro_modify_config_params_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mro_modify_config_params_resp->result] should be in range [%d <= 2] !", p_son_oam_mro_modify_config_params_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_modify_config_params_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mro_eutran_ttt_update_ind
(
    son_oam_mro_eutran_ttt_update_ind_t *p_son_oam_mro_eutran_ttt_update_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mro_eutran_ttt_update_ind, 0, sizeof(son_oam_mro_eutran_ttt_update_ind_t));

    /* This function parses son_oam_mro_eutran_ttt_update_ind */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mro_eutran_ttt_update_ind");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_eutran_ttt_update_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mro_eutran_ttt_update_ind->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_eutran_ttt_update_ind->updated_ttt, p_src + *p_length_read, "updated_ttt");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mro_ho_counter_report
(
    son_oam_mro_ho_counter_report_t *p_son_oam_mro_ho_counter_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mro_ho_counter_report, 0, sizeof(son_oam_mro_ho_counter_report_t));

    /* This function parses son_oam_mro_ho_counter_report */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mro_ho_counter_report");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_ho_counter_report->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mro_ho_counter_report->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_mro_ho_counter_report->neighbour_list_size, p_src + *p_length_read, "neighbour_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_mro_ho_counter_report->neighbour_list_size > SON_MAX_NO_NEIGHBOUR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mro_ho_counter_report->neighbour_list_size] should be in range [%d <= SON_MAX_NO_NEIGHBOUR_CELLS] !", p_son_oam_mro_ho_counter_report->neighbour_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_mro_ho_counter_report->neighbour_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_mro_counter_info(
                &p_son_oam_mro_ho_counter_report->ho_counter_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_mro_counter_info
(
    son_mro_counter_info_t *p_son_mro_counter_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_mro_counter_info, 0, sizeof(son_mro_counter_info_t));

    /* This function parses son_mro_counter_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_mro_counter_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_mro_counter_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_mro_counter_info->nbr_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_mro_counter_info->num_too_early_ho, p_src + *p_length_read, "num_too_early_ho");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_mro_counter_info->num_too_late_ho, p_src + *p_length_read, "num_too_late_ho");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_mro_counter_info->num_ho_to_wrong_cell_f, p_src + *p_length_read, "num_ho_to_wrong_cell_f");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_mro_counter_info->num_ho_to_wrong_cell_r, p_src + *p_length_read, "num_ho_to_wrong_cell_r");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_mro_counter_info->total_ho_attempted, p_src + *p_length_read, "total_ho_attempted");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mlb_enable_req
(
    son_oam_mlb_enable_req_t *p_son_oam_mlb_enable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mlb_enable_req, 0, sizeof(son_oam_mlb_enable_req_t));

    /* This function parses son_oam_mlb_enable_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mlb_enable_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_enable_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_mlb_enable_req->bitmask & SON_OAM_MLB_ENABLE_SRC_CGI_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_enable_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mlb_enable_resp
(
    son_oam_mlb_enable_resp_t *p_son_oam_mlb_enable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mlb_enable_resp, 0, sizeof(son_oam_mlb_enable_resp_t));

    /* This function parses son_oam_mlb_enable_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mlb_enable_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_enable_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_mlb_enable_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mlb_enable_resp->result] should be in range [%d <= 1] !", p_son_oam_mlb_enable_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_enable_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_mlb_enable_resp->cell_status_list_size, p_src + *p_length_read, "cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_mlb_enable_resp->cell_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mlb_enable_resp->cell_status_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_mlb_enable_resp->cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_mlb_enable_resp->cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_status(
                &p_son_oam_mlb_enable_resp->cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mlb_disable_req
(
    son_oam_mlb_disable_req_t *p_son_oam_mlb_disable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mlb_disable_req, 0, sizeof(son_oam_mlb_disable_req_t));

    /* This function parses son_oam_mlb_disable_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mlb_disable_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_disable_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_mlb_disable_req->bitmask & SON_OAM_MLB_DISABLE_SRC_CGI_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_disable_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mlb_disable_resp
(
    son_oam_mlb_disable_resp_t *p_son_oam_mlb_disable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mlb_disable_resp, 0, sizeof(son_oam_mlb_disable_resp_t));

    /* This function parses son_oam_mlb_disable_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mlb_disable_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_disable_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_mlb_disable_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mlb_disable_resp->result] should be in range [%d <= 1] !", p_son_oam_mlb_disable_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_disable_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_mlb_disable_resp->cell_status_list_size, p_src + *p_length_read, "cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_mlb_disable_resp->cell_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mlb_disable_resp->cell_status_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_mlb_disable_resp->cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_mlb_disable_resp->cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_status(
                &p_son_oam_mlb_disable_resp->cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mlb_modify_attributes_req
(
    son_oam_mlb_modify_attributes_req_t *p_son_oam_mlb_modify_attributes_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mlb_modify_attributes_req, 0, sizeof(son_oam_mlb_modify_attributes_req_t));

    /* This function parses son_oam_mlb_modify_attributes_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mlb_modify_attributes_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_modify_attributes_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_modify_attributes_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_oam_mlb_modify_attributes_req->bitmask & SON_OAM_MLB_EUTRA_Q_OFFSET_CONFIG_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_eutran_q_offset_config(
        &p_son_oam_mlb_modify_attributes_req->eutra_q_offset_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_oam_mlb_modify_attributes_req->bitmask & SON_OAM_MLB_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_modify_attributes_req->mlb_mode, p_src + *p_length_read, "mlb_mode");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_mlb_modify_attributes_req->mlb_mode > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mlb_modify_attributes_req->mlb_mode] should be in range [%d <= 1] !", p_son_oam_mlb_modify_attributes_req->mlb_mode);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_eutran_q_offset_config
(
    son_eutran_q_offset_config_t *p_son_eutran_q_offset_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_eutran_q_offset_config, 0, sizeof(son_eutran_q_offset_config_t));

    /* This function parses son_eutran_q_offset_config */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_eutran_q_offset_config");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_eutran_q_offset_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_eutran_q_offset_config->bitmask & SON_OAM_MLB_Q_OFF_MIN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_eutran_q_offset_config->q_offset_min, p_src + *p_length_read, "q_offset_min");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_eutran_q_offset_config->q_offset_min > 30))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_eutran_q_offset_config->q_offset_min] should be in range [%d <= 30] !", p_son_eutran_q_offset_config->q_offset_min);
        return SON_FAILURE;
    }
    }

    if (p_son_eutran_q_offset_config->bitmask & SON_OAM_MLB_Q_OFF_MAX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_eutran_q_offset_config->q_offset_max, p_src + *p_length_read, "q_offset_max");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_eutran_q_offset_config->q_offset_max > 30))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_eutran_q_offset_config->q_offset_max] should be in range [%d <= 30] !", p_son_eutran_q_offset_config->q_offset_max);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mlb_modify_attributes_resp
(
    son_oam_mlb_modify_attributes_resp_t *p_son_oam_mlb_modify_attributes_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mlb_modify_attributes_resp, 0, sizeof(son_oam_mlb_modify_attributes_resp_t));

    /* This function parses son_oam_mlb_modify_attributes_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mlb_modify_attributes_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_modify_attributes_resp->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_modify_attributes_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_mlb_modify_attributes_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mlb_modify_attributes_resp->result] should be in range [%d <= 1] !", p_son_oam_mlb_modify_attributes_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_modify_attributes_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_event_notification
(
    son_oam_event_notification_t *p_son_oam_event_notification,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_event_notification, 0, sizeof(son_oam_event_notification_t));

    /* This function parses son_oam_event_notification */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_event_notification");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_event_notification->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_oam_event_header(
        &p_son_oam_event_notification->msg_header,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_oam_event_notification->bitmask & SON_OAM_EVENT_API_DATA_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_oam_error_info(
        &p_son_oam_event_notification->api_data,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_oam_time_stamp
(
    son_oam_time_stamp_t *p_son_oam_time_stamp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_time_stamp, 0, sizeof(son_oam_time_stamp_t));

    /* This function parses son_oam_time_stamp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_time_stamp");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_time_stamp->year, p_src + *p_length_read, "year");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_time_stamp->month, p_src + *p_length_read, "month");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_time_stamp->month < 1) || (p_son_oam_time_stamp->month > 12))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_time_stamp->month] should be in range [1 <= %d <= 12] !", p_son_oam_time_stamp->month);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_time_stamp->day, p_src + *p_length_read, "day");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_time_stamp->day < 1) || (p_son_oam_time_stamp->day > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_time_stamp->day] should be in range [1 <= %d <= 31] !", p_son_oam_time_stamp->day);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_time_stamp->hour, p_src + *p_length_read, "hour");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_time_stamp->hour > 23))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_time_stamp->hour] should be in range [%d <= 23] !", p_son_oam_time_stamp->hour);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_time_stamp->min, p_src + *p_length_read, "min");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_time_stamp->min > 59))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_time_stamp->min] should be in range [%d <= 59] !", p_son_oam_time_stamp->min);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_time_stamp->sec, p_src + *p_length_read, "sec");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_time_stamp->sec > 59))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_time_stamp->sec] should be in range [%d <= 59] !", p_son_oam_time_stamp->sec);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_oam_event_header
(
    son_oam_event_header_t *p_son_oam_event_header,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_event_header, 0, sizeof(son_oam_event_header_t));

    /* This function parses son_oam_event_header */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_event_header");



    if (SON_FAILURE == son_parse_son_oam_time_stamp(
        &p_son_oam_event_header->time_stamp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_event_header->event_type, p_src + *p_length_read, "event_type");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_event_header->event_sub_type, p_src + *p_length_read, "event_sub_type");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_event_header->event_id, p_src + *p_length_read, "event_id");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_oam_error_info
(
    son_oam_error_info_t *p_son_oam_error_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_error_info, 0, sizeof(son_oam_error_info_t));

    /* This function parses son_oam_error_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_error_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_error_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_error_info->error_type, p_src + *p_length_read, "error_type");
    *p_length_read += sizeof(U32);

    if (p_son_oam_error_info->bitmask & SON_OAM_EVENT_ERROR_CAUSE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_error_info->error_cause, p_src + *p_length_read, "error_cause");
    *p_length_read += sizeof(U32);
    }

    if (p_son_oam_error_info->bitmask & SON_OAM_EVENT_SEVERITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_error_info->severity, p_src + *p_length_read, "severity");
    *p_length_read += sizeof(U32);
    }

    if (p_son_oam_error_info->bitmask & SON_OAM_EVENT_ASSOCIATED_DATA_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_error_info->associated_data, p_src + *p_length_read, "associated_data");
    *p_length_read += sizeof(U8);
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_gu_group_id_config_req
(
    son_oam_anr_gu_group_id_config_req_t *p_son_oam_anr_gu_group_id_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_gu_group_id_config_req, 0, sizeof(son_oam_anr_gu_group_id_config_req_t));

    /* This function parses son_oam_anr_gu_group_id_config_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_gu_group_id_config_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_gu_group_id_config_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_anr_gu_group_id_config_req->bitmask & SON_OAM_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_gu_group_id_list(
        &p_son_oam_anr_gu_group_id_config_req->gu_group_id_to_add_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_oam_anr_gu_group_id_config_req->bitmask & SON_OAM_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_gu_group_id_list(
        &p_son_oam_anr_gu_group_id_config_req->gu_group_id_to_delete_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_gu_group_id_config_resp
(
    son_oam_anr_gu_group_id_config_resp_t *p_son_oam_anr_gu_group_id_config_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_gu_group_id_config_resp, 0, sizeof(son_oam_anr_gu_group_id_config_resp_t));

    /* This function parses son_oam_anr_gu_group_id_config_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_gu_group_id_config_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_gu_group_id_config_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_gu_group_id_config_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_gu_group_id_config_resp->result] should be in range [%d <= 1] !", p_son_oam_anr_gu_group_id_config_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_gu_group_id_config_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_neighbor_default_config_req
(
    son_oam_anr_neighbor_default_config_req_t *p_son_oam_anr_neighbor_default_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_neighbor_default_config_req, 0, sizeof(son_oam_anr_neighbor_default_config_req_t));

    /* This function parses son_oam_anr_neighbor_default_config_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_neighbor_default_config_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_neighbor_default_config_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_anr_neighbor_default_config_req->bitmask & SON_OAM_ANR_NBR_DEF_NR_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_neighbor_default_config_req->nr_status, p_src + *p_length_read, "nr_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_neighbor_default_config_req->nr_status < 1) || (p_son_oam_anr_neighbor_default_config_req->nr_status > 255))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_neighbor_default_config_req->nr_status] should be in range [1 <= %d <= 255] !", p_son_oam_anr_neighbor_default_config_req->nr_status);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_anr_neighbor_default_config_req->bitmask & SON_OAM_ANR_NBR_DEF_HO_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_neighbor_default_config_req->ho_status, p_src + *p_length_read, "ho_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_neighbor_default_config_req->ho_status < 1) || (p_son_oam_anr_neighbor_default_config_req->ho_status > 255))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_neighbor_default_config_req->ho_status] should be in range [1 <= %d <= 255] !", p_son_oam_anr_neighbor_default_config_req->ho_status);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_anr_neighbor_default_config_req->bitmask & SON_OAM_ANR_NBR_DEF_X2_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_neighbor_default_config_req->x2_status, p_src + *p_length_read, "x2_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_anr_neighbor_default_config_req->x2_status < 1) || (p_son_oam_anr_neighbor_default_config_req->x2_status > 255))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_neighbor_default_config_req->x2_status] should be in range [1 <= %d <= 255] !", p_son_oam_anr_neighbor_default_config_req->x2_status);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_anr_neighbor_default_config_req->bitmask & SON_OAM_ANR_CSG_DEF_SCHEME_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_neighbor_default_config_req->csg_cell_intf_scheme, p_src + *p_length_read, "csg_cell_intf_scheme");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_neighbor_default_config_req->csg_cell_intf_scheme > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_neighbor_default_config_req->csg_cell_intf_scheme] should be in range [%d <= 2] !", p_son_oam_anr_neighbor_default_config_req->csg_cell_intf_scheme);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_anr_neighbor_default_config_req->bitmask & SON_OAM_ANR_PICO_DEF_SCHEME_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_neighbor_default_config_req->pico_cell_intf_scheme, p_src + *p_length_read, "pico_cell_intf_scheme");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_neighbor_default_config_req->pico_cell_intf_scheme > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_neighbor_default_config_req->pico_cell_intf_scheme] should be in range [%d <= 2] !", p_son_oam_anr_neighbor_default_config_req->pico_cell_intf_scheme);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_anr_neighbor_default_config_req->bitmask & SON_OAM_ANR_MACRO_DEF_SCHEME_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_neighbor_default_config_req->macro_cell_intf_scheme, p_src + *p_length_read, "macro_cell_intf_scheme");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_neighbor_default_config_req->macro_cell_intf_scheme > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_neighbor_default_config_req->macro_cell_intf_scheme] should be in range [%d <= 2] !", p_son_oam_anr_neighbor_default_config_req->macro_cell_intf_scheme);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_neighbor_default_config_resp
(
    son_oam_anr_neighbor_default_config_resp_t *p_son_oam_anr_neighbor_default_config_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_neighbor_default_config_resp, 0, sizeof(son_oam_anr_neighbor_default_config_resp_t));

    /* This function parses son_oam_anr_neighbor_default_config_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_neighbor_default_config_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_neighbor_default_config_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_neighbor_default_config_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_neighbor_default_config_resp->result] should be in range [%d <= 1] !", p_son_oam_anr_neighbor_default_config_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_neighbor_default_config_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_carrier_freq_and_dl_tx_power_req
(
    son_oam_carrier_freq_and_dl_tx_power_req_t *p_son_oam_carrier_freq_and_dl_tx_power_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_carrier_freq_and_dl_tx_power_req, 0, sizeof(son_oam_carrier_freq_and_dl_tx_power_req_t));

    /* This function parses son_oam_carrier_freq_and_dl_tx_power_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_carrier_freq_and_dl_tx_power_req");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_carrier_freq_and_dl_tx_power_req->meas_bandwidth, p_src + *p_length_read, "meas_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_carrier_freq_and_dl_tx_power_req->meas_bandwidth < 6) || (p_son_oam_carrier_freq_and_dl_tx_power_req->meas_bandwidth > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_carrier_freq_and_dl_tx_power_req->meas_bandwidth] should be in range [6 <= %d <= 100] !", p_son_oam_carrier_freq_and_dl_tx_power_req->meas_bandwidth);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_carrier_freq_and_dl_tx_power_req->earfcn_list_size, p_src + *p_length_read, "earfcn_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_carrier_freq_and_dl_tx_power_req->earfcn_list_size < 1) || (p_son_oam_carrier_freq_and_dl_tx_power_req->earfcn_list_size > SON_MAX_NO_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_carrier_freq_and_dl_tx_power_req->earfcn_list_size] should be in range [1 <= %d <= SON_MAX_NO_EARFCN] !", p_son_oam_carrier_freq_and_dl_tx_power_req->earfcn_list_size);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_oam_carrier_freq_and_dl_tx_power_req->earfcn_list_size * sizeof(p_son_oam_carrier_freq_and_dl_tx_power_req->earfcn_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_carrier_freq_and_dl_tx_power_req->earfcn_list_size; loop++)
        {
            son_cp_unpack_U32((void*)&p_son_oam_carrier_freq_and_dl_tx_power_req->earfcn_list[loop], (void*)(p_src + *p_length_read), "earfcn_list[]");
            *p_length_read += sizeof(U32);
        }
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_oam_carrier_freq_and_dl_tx_power_req->p_tx_upp, p_src + *p_length_read, "p_tx_upp");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_carrier_freq_and_dl_tx_power_req->p_tx_upp < -60) || (p_son_oam_carrier_freq_and_dl_tx_power_req->p_tx_upp > 50))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_carrier_freq_and_dl_tx_power_req->p_tx_upp] should be in range [-60 <= %d <= 50] !", p_son_oam_carrier_freq_and_dl_tx_power_req->p_tx_upp);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_oam_carrier_freq_and_dl_tx_power_req->p_tx_low, p_src + *p_length_read, "p_tx_low");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_carrier_freq_and_dl_tx_power_req->p_tx_low < -60) || (p_son_oam_carrier_freq_and_dl_tx_power_req->p_tx_low > 50))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_carrier_freq_and_dl_tx_power_req->p_tx_low] should be in range [-60 <= %d <= 50] !", p_son_oam_carrier_freq_and_dl_tx_power_req->p_tx_low);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_oam_carrier_freq_and_dl_tx_power_req->p_ue_max, p_src + *p_length_read, "p_ue_max");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_carrier_freq_and_dl_tx_power_req->p_ue_max < -40) || (p_son_oam_carrier_freq_and_dl_tx_power_req->p_ue_max > 33))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_carrier_freq_and_dl_tx_power_req->p_ue_max] should be in range [-40 <= %d <= 33] !", p_son_oam_carrier_freq_and_dl_tx_power_req->p_ue_max);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_oam_carrier_freq_and_dl_tx_power_req->p_ue_min, p_src + *p_length_read, "p_ue_min");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_carrier_freq_and_dl_tx_power_req->p_ue_min < -40) || (p_son_oam_carrier_freq_and_dl_tx_power_req->p_ue_min > 33))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_carrier_freq_and_dl_tx_power_req->p_ue_min] should be in range [-40 <= %d <= 33] !", p_son_oam_carrier_freq_and_dl_tx_power_req->p_ue_min);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_carrier_freq_and_dl_tx_power_resp
(
    son_oam_carrier_freq_and_dl_tx_power_resp_t *p_son_oam_carrier_freq_and_dl_tx_power_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_carrier_freq_and_dl_tx_power_resp, 0, sizeof(son_oam_carrier_freq_and_dl_tx_power_resp_t));

    /* This function parses son_oam_carrier_freq_and_dl_tx_power_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_carrier_freq_and_dl_tx_power_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_carrier_freq_and_dl_tx_power_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_carrier_freq_and_dl_tx_power_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_carrier_freq_and_dl_tx_power_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_carrier_freq_and_dl_tx_power_resp->result] should be in range [%d <= 1] !", p_son_oam_carrier_freq_and_dl_tx_power_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_carrier_freq_and_dl_tx_power_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (p_son_oam_carrier_freq_and_dl_tx_power_resp->bitmask & SON_OAM_CARR_FREQ_DL_TX_PWR_DL_EARFCN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_carrier_freq_and_dl_tx_power_resp->dl_earfcn, p_src + *p_length_read, "dl_earfcn");
    *p_length_read += sizeof(U32);
    }

    if (p_son_oam_carrier_freq_and_dl_tx_power_resp->bitmask & SON_OAM_CARR_FREQ_DL_TX_PWR_REF_SIGNAL_POWER_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_oam_carrier_freq_and_dl_tx_power_resp->reference_signal_power, p_src + *p_length_read, "reference_signal_power");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_carrier_freq_and_dl_tx_power_resp->reference_signal_power < -60) || (p_son_oam_carrier_freq_and_dl_tx_power_resp->reference_signal_power > 50))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_carrier_freq_and_dl_tx_power_resp->reference_signal_power] should be in range [-60 <= %d <= 50] !", p_son_oam_carrier_freq_and_dl_tx_power_resp->reference_signal_power);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_carrier_freq_and_dl_tx_power_resp->bitmask & SON_OAM_CARR_FREQ_DL_TX_PWR_P_MAX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_oam_carrier_freq_and_dl_tx_power_resp->p_max, p_src + *p_length_read, "p_max");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_carrier_freq_and_dl_tx_power_resp->p_max < -40) || (p_son_oam_carrier_freq_and_dl_tx_power_resp->p_max > 33))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_carrier_freq_and_dl_tx_power_resp->p_max] should be in range [-40 <= %d <= 33] !", p_son_oam_carrier_freq_and_dl_tx_power_resp->p_max);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_last_scan_timestamp_ind
(
    son_oam_last_scan_timestamp_ind_t *p_son_oam_last_scan_timestamp_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_last_scan_timestamp_ind, 0, sizeof(son_oam_last_scan_timestamp_ind_t));

    /* This function parses son_oam_last_scan_timestamp_ind */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_last_scan_timestamp_ind");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_last_scan_timestamp_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_oam_time_stamp(
        &p_son_oam_last_scan_timestamp_ind->time_stamp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mlb_params_info
(
    son_oam_mlb_params_info_t *p_son_oam_mlb_params_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mlb_params_info, 0, sizeof(son_oam_mlb_params_info_t));

    /* This function parses son_oam_mlb_params_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mlb_params_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_params_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_params_info->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_oam_mlb_params_info->bitmask & SON_OAM_MLB_HO_PARAMS_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_mlb_ho_params(
        &p_son_oam_mlb_params_info->ho_params,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_oam_mlb_params_info->bitmask & SON_OAM_MLB_LOAD_PARAMS_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_mlb_load_params(
        &p_son_oam_mlb_params_info->load_params,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cell_offset_attr
(
    son_cell_offset_attr_t *p_son_cell_offset_attr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_cell_offset_attr, 0, sizeof(son_cell_offset_attr_t));

    /* This function parses son_cell_offset_attr */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cell_offset_attr");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cell_offset_attr->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_cell_offset_attr->bitmask & SON_OAM_MLB_OFFSET_ATTR_OFS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_cell_offset_attr->ofs, p_src + *p_length_read, "ofs");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_cell_offset_attr->ofs < -24) || (p_son_cell_offset_attr->ofs > 24))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell_offset_attr->ofs] should be in range [-24 <= %d <= 24] !", p_son_cell_offset_attr->ofs);
        return SON_FAILURE;
    }
    }

    if (p_son_cell_offset_attr->bitmask & SON_OAM_MLB_OFFSET_ATTR_OCS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_cell_offset_attr->ocs, p_src + *p_length_read, "ocs");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_cell_offset_attr->ocs < -24) || (p_son_cell_offset_attr->ocs > 24))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell_offset_attr->ocs] should be in range [-24 <= %d <= 24] !", p_son_cell_offset_attr->ocs);
        return SON_FAILURE;
    }
    }

    if (p_son_cell_offset_attr->bitmask & SON_OAM_MLB_OFFSET_ATTR_OFN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_cell_offset_attr->ofn, p_src + *p_length_read, "ofn");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_cell_offset_attr->ofn < -24) || (p_son_cell_offset_attr->ofn > 24))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell_offset_attr->ofn] should be in range [-24 <= %d <= 24] !", p_son_cell_offset_attr->ofn);
        return SON_FAILURE;
    }
    }

    if (p_son_cell_offset_attr->bitmask & SON_OAM_MLB_OFFSET_ATTR_OFF_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_cell_offset_attr->off, p_src + *p_length_read, "off");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_cell_offset_attr->off < -24) || (p_son_cell_offset_attr->off > 24))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell_offset_attr->off] should be in range [-24 <= %d <= 24] !", p_son_cell_offset_attr->off);
        return SON_FAILURE;
    }
    }

    if (p_son_cell_offset_attr->bitmask & SON_OAM_MLB_OFFSET_ATTR_HYS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_cell_offset_attr->hys, p_src + *p_length_read, "hys");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cell_offset_attr->hys > 30))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cell_offset_attr->hys] should be in range [%d <= 30] !", p_son_cell_offset_attr->hys);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_mlb_ho_params
(
    son_mlb_ho_params_t *p_son_mlb_ho_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_mlb_ho_params, 0, sizeof(son_mlb_ho_params_t));

    /* This function parses son_mlb_ho_params */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_mlb_ho_params");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_mlb_ho_params->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_mlb_ho_params->trgt_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_mlb_ho_params->bitmask & SON_OAM_MLB_SRC_RSRP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_mlb_ho_params->src_rsrp, p_src + *p_length_read, "src_rsrp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mlb_ho_params->src_rsrp > 97))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mlb_ho_params->src_rsrp] should be in range [%d <= 97] !", p_son_mlb_ho_params->src_rsrp);
        return SON_FAILURE;
    }
    }

    if (p_son_mlb_ho_params->bitmask & SON_OAM_MLB_TARGET_RSRP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_mlb_ho_params->trgt_rsrp, p_src + *p_length_read, "trgt_rsrp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mlb_ho_params->trgt_rsrp > 97))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mlb_ho_params->trgt_rsrp] should be in range [%d <= 97] !", p_son_mlb_ho_params->trgt_rsrp);
        return SON_FAILURE;
    }
    }

    if (p_son_mlb_ho_params->bitmask & SON_OAM_MLB_CELL_OFSSET_ATTR_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_cell_offset_attr(
        &p_son_mlb_ho_params->offset_attr,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_nbr_cell_load_info
(
    son_nbr_cell_load_info_t *p_son_nbr_cell_load_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_nbr_cell_load_info, 0, sizeof(son_nbr_cell_load_info_t));

    /* This function parses son_nbr_cell_load_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_nbr_cell_load_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_nbr_cell_load_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_nbr_cell_load_info->trgt_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_nbr_cell_load_info->bitmask & MLB_TRGT_CELL_CAP_VAL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_nbr_cell_load_info->trgt_dl_comp_avl_cap_val, p_src + *p_length_read, "trgt_dl_comp_avl_cap_val");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_nbr_cell_load_info->trgt_dl_comp_avl_cap_val > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_nbr_cell_load_info->trgt_dl_comp_avl_cap_val] should be in range [%d <= 100] !", p_son_nbr_cell_load_info->trgt_dl_comp_avl_cap_val);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_mlb_load_params
(
    son_mlb_load_params_t *p_son_mlb_load_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_mlb_load_params, 0, sizeof(son_mlb_load_params_t));

    /* This function parses son_mlb_load_params */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_mlb_load_params");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_mlb_load_params->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_mlb_load_params->bitmask & MLB_SRC_CELL_CAP_VAL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_mlb_load_params->src_dl_comp_avl_cap_val, p_src + *p_length_read, "src_dl_comp_avl_cap_val");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mlb_load_params->src_dl_comp_avl_cap_val > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mlb_load_params->src_dl_comp_avl_cap_val] should be in range [%d <= 100] !", p_son_mlb_load_params->src_dl_comp_avl_cap_val);
        return SON_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_mlb_load_params->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mlb_load_params->count > SON_MAX_NO_INTRA_NBR_CELLS))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mlb_load_params->count] should be in range [%d <= SON_MAX_NO_INTRA_NBR_CELLS] !", p_son_mlb_load_params->count);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_mlb_load_params->count; loop++)
        {
            if (SON_FAILURE == son_parse_son_nbr_cell_load_info(
                &p_son_mlb_load_params->nbr_cell_load_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mlb_offset_change_req
(
    son_oam_mlb_offset_change_req_t *p_son_oam_mlb_offset_change_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mlb_offset_change_req, 0, sizeof(son_oam_mlb_offset_change_req_t));

    /* This function parses son_oam_mlb_offset_change_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mlb_offset_change_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_offset_change_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_offset_change_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_offset_change_req->trgt_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_oam_mlb_offset_change_req->bitmask & SON_OAM_MLB_TRGT_CSG_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_offset_change_req->trgt_csg_id_present, p_src + *p_length_read, "trgt_csg_id_present");
    *p_length_read += sizeof(U32);
    }

    if (p_son_oam_mlb_offset_change_req->bitmask & SON_OAM_MLB_PROPOSED_MOB_PARAMS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_oam_mlb_offset_change_req->proposed_mobility_params, p_src + *p_length_read, "proposed_mobility_params");
    *p_length_read += sizeof(S8);
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mlb_offset_change_resp
(
    son_oam_mlb_offset_change_resp_t *p_son_oam_mlb_offset_change_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mlb_offset_change_resp, 0, sizeof(son_oam_mlb_offset_change_resp_t));

    /* This function parses son_oam_mlb_offset_change_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mlb_offset_change_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_offset_change_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_offset_change_resp->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_offset_change_resp->mlb_status, p_src + *p_length_read, "mlb_status");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_offset_change_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (p_son_oam_mlb_offset_change_resp->bitmask & SON_OAM_MLB_TRGT_CGI_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_offset_change_resp->trgt_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_oam_mlb_offset_change_resp->bitmask & SON_OAM_MLB_MOBILITY_RANGE_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_mob_param_range(
        &p_son_oam_mlb_offset_change_resp->mob_range,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_mob_param_range
(
    son_mob_param_range_t *p_son_mob_param_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_mob_param_range, 0, sizeof(son_mob_param_range_t));

    /* This function parses son_mob_param_range */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_mob_param_range");

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_mob_param_range->lower_limit, p_src + *p_length_read, "lower_limit");
    *p_length_read += sizeof(S8);

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_mob_param_range->upper_limit, p_src + *p_length_read, "upper_limit");
    *p_length_read += sizeof(S8);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mlb_peer_offset_change_req
(
    son_oam_mlb_peer_offset_change_req_t *p_son_oam_mlb_peer_offset_change_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mlb_peer_offset_change_req, 0, sizeof(son_oam_mlb_peer_offset_change_req_t));

    /* This function parses son_oam_mlb_peer_offset_change_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mlb_peer_offset_change_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_peer_offset_change_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_peer_offset_change_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_peer_offset_change_req->trgt_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_oam_mlb_peer_offset_change_req->bitmask & SON_OAM_MLB_PEER_PROPOSED_MOB_PARAMS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S8(&p_son_oam_mlb_peer_offset_change_req->proposed_mobility_params, p_src + *p_length_read, "proposed_mobility_params");
    *p_length_read += sizeof(S8);
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mlb_peer_offset_change_resp
(
    son_oam_mlb_peer_offset_change_resp_t *p_son_oam_mlb_peer_offset_change_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mlb_peer_offset_change_resp, 0, sizeof(son_oam_mlb_peer_offset_change_resp_t));

    /* This function parses son_oam_mlb_peer_offset_change_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mlb_peer_offset_change_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_peer_offset_change_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_peer_offset_change_resp->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_peer_offset_change_resp->mlb_status, p_src + *p_length_read, "mlb_status");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mlb_peer_offset_change_resp->trgt_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_oam_mlb_peer_offset_change_resp->bitmask & SON_OAM_MLB_SRC_CSG_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mlb_peer_offset_change_resp->src_csg_id_present, p_src + *p_length_read, "src_csg_id_present");
    *p_length_read += sizeof(U32);
    }

    if (p_son_oam_mlb_peer_offset_change_resp->bitmask & SON_OAM_MLB_PEER_MOBILITY_RANGE_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_mob_param_range(
        &p_son_oam_mlb_peer_offset_change_resp->mob_range,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_mro_optimization_notification
(
    son_oam_mro_optimization_notification_t *p_son_oam_mro_optimization_notification,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_mro_optimization_notification, 0, sizeof(son_oam_mro_optimization_notification_t));

    /* This function parses son_oam_mro_optimization_notification */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_mro_optimization_notification");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_optimization_notification->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mro_optimization_notification->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_oam_mro_optimization_notification->bitmask & SON_NBR_INTRA_RAT_GLOBAL_CELL_ID_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_mro_optimization_notification->nbr_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_optimization_notification->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U32);

    /* Check for correct range [L - lower boundary] */
    if ((p_son_oam_mro_optimization_notification->cause < 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mro_optimization_notification->cause] should be in range [1 <= %d] !", p_son_oam_mro_optimization_notification->cause);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_mro_optimization_notification->ho_failure_level, p_src + *p_length_read, "ho_failure_level");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_mro_optimization_notification->ho_failure_level < 1) || (p_son_oam_mro_optimization_notification->ho_failure_level > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mro_optimization_notification->ho_failure_level] should be in range [1 <= %d <= 2] !", p_son_oam_mro_optimization_notification->ho_failure_level);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_mro_optimization_notification->ho_failure_rate, p_src + *p_length_read, "ho_failure_rate");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_mro_optimization_notification->ho_failure_rate < 1) || (p_son_oam_mro_optimization_notification->ho_failure_rate > 10000))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_mro_optimization_notification->ho_failure_rate] should be in range [1 <= %d <= 10000] !", p_son_oam_mro_optimization_notification->ho_failure_rate);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_autonomous_switch_off_config_req
(
    son_oam_es_autonomous_switch_off_config_req_t *p_son_oam_es_autonomous_switch_off_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_autonomous_switch_off_config_req, 0, sizeof(son_oam_es_autonomous_switch_off_config_req_t));

    /* This function parses son_oam_es_autonomous_switch_off_config_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_autonomous_switch_off_config_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_autonomous_switch_off_config_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_es_autonomous_switch_off_config_req->src_cgi_list_size, p_src + *p_length_read, "src_cgi_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_autonomous_switch_off_config_req->src_cgi_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_autonomous_switch_off_config_req->src_cgi_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_es_autonomous_switch_off_config_req->src_cgi_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_es_autonomous_switch_off_config_req->src_cgi_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
                &p_son_oam_es_autonomous_switch_off_config_req->cell_id[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_son_oam_es_autonomous_switch_off_config_req->bitmask & SON_OAM_AUTO_SWITCH_OFF_LOAD_LEVEL)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_es_autonomous_switch_off_config_req->load_level_threshold, p_src + *p_length_read, "load_level_threshold");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_autonomous_switch_off_config_req->load_level_threshold > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_autonomous_switch_off_config_req->load_level_threshold] should be in range [%d <= 100] !", p_son_oam_es_autonomous_switch_off_config_req->load_level_threshold);
        return SON_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_es_autonomous_switch_off_config_req->timer_duration, p_src + *p_length_read, "timer_duration");
    *p_length_read += sizeof(U16);

    /* Check for correct range [L - lower boundary] */
    if ((p_son_oam_es_autonomous_switch_off_config_req->timer_duration < 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_autonomous_switch_off_config_req->timer_duration] should be in range [1 <= %d] !", p_son_oam_es_autonomous_switch_off_config_req->timer_duration);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_es_autonomous_switch_off_config_resp
(
    son_oam_es_autonomous_switch_off_config_resp_t *p_son_oam_es_autonomous_switch_off_config_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_es_autonomous_switch_off_config_resp, 0, sizeof(son_oam_es_autonomous_switch_off_config_resp_t));

    /* This function parses son_oam_es_autonomous_switch_off_config_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_es_autonomous_switch_off_config_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_autonomous_switch_off_config_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_autonomous_switch_off_config_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_autonomous_switch_off_config_resp->result] should be in range [%d <= 1] !", p_son_oam_es_autonomous_switch_off_config_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_es_autonomous_switch_off_config_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_es_autonomous_switch_off_config_resp->cell_status_list_size, p_src + *p_length_read, "cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_es_autonomous_switch_off_config_resp->cell_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_es_autonomous_switch_off_config_resp->cell_status_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_es_autonomous_switch_off_config_resp->cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_es_autonomous_switch_off_config_resp->cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_status(
                &p_son_oam_es_autonomous_switch_off_config_resp->cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_chk_health_req
(
    son_oam_chk_health_req_t *p_son_oam_chk_health_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_chk_health_req, 0, sizeof(son_oam_chk_health_req_t));

    /* This function parses son_oam_chk_health_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_chk_health_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_chk_health_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_chk_health_resp
(
    son_oam_chk_health_resp_t *p_son_oam_chk_health_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_chk_health_resp, 0, sizeof(son_oam_chk_health_resp_t));

    /* This function parses son_oam_chk_health_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_chk_health_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_chk_health_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_meas_config_obj_remove_req
(
    son_oam_anr_meas_config_obj_remove_req_t *p_son_oam_anr_meas_config_obj_remove_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_meas_config_obj_remove_req, 0, sizeof(son_oam_anr_meas_config_obj_remove_req_t));

    /* This function parses son_oam_anr_meas_config_obj_remove_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_meas_config_obj_remove_req");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_meas_config_obj_remove_req->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_meas_config_obj_remove_req->earfcn_remove_list_size, p_src + *p_length_read, "earfcn_remove_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_meas_config_obj_remove_req->earfcn_remove_list_size > SON_MAX_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_meas_config_obj_remove_req->earfcn_remove_list_size] should be in range [%d <= SON_MAX_CONFIG_OBJECT] !", p_son_oam_anr_meas_config_obj_remove_req->earfcn_remove_list_size);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_oam_anr_meas_config_obj_remove_req->earfcn_remove_list_size * sizeof(p_son_oam_anr_meas_config_obj_remove_req->earfcn_remove_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_anr_meas_config_obj_remove_req->earfcn_remove_list_size; loop++)
        {
            son_cp_unpack_U16((void*)&p_son_oam_anr_meas_config_obj_remove_req->earfcn_remove_list[loop], (void*)(p_src + *p_length_read), "earfcn_remove_list[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size, p_src + *p_length_read, "uarfcn_fdd_remove_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size > SON_MAX_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size] should be in range [%d <= SON_MAX_CONFIG_OBJECT] !", p_son_oam_anr_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_oam_anr_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size * sizeof(p_son_oam_anr_meas_config_obj_remove_req->uarfcn_fdd_remove_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_anr_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size; loop++)
        {
            son_cp_unpack_U16((void*)&p_son_oam_anr_meas_config_obj_remove_req->uarfcn_fdd_remove_list[loop], (void*)(p_src + *p_length_read), "uarfcn_fdd_remove_list[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size, p_src + *p_length_read, "uarfcn_tdd_remove_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size > SON_MAX_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size] should be in range [%d <= SON_MAX_CONFIG_OBJECT] !", p_son_oam_anr_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_oam_anr_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size * sizeof(p_son_oam_anr_meas_config_obj_remove_req->uarfcn_tdd_remove_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_anr_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size; loop++)
        {
            son_cp_unpack_U16((void*)&p_son_oam_anr_meas_config_obj_remove_req->uarfcn_tdd_remove_list[loop], (void*)(p_src + *p_length_read), "uarfcn_tdd_remove_list[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_meas_config_obj_remove_req->geran_remove_list_size, p_src + *p_length_read, "geran_remove_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_meas_config_obj_remove_req->geran_remove_list_size > SON_MAX_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_meas_config_obj_remove_req->geran_remove_list_size] should be in range [%d <= SON_MAX_CONFIG_OBJECT] !", p_son_oam_anr_meas_config_obj_remove_req->geran_remove_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_anr_meas_config_obj_remove_req->geran_remove_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_geran_carrier_freqs(
                &p_son_oam_anr_meas_config_obj_remove_req->geran_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_anr_meas_config_obj_remove_req->cdma_remove_list_size, p_src + *p_length_read, "cdma_remove_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_meas_config_obj_remove_req->cdma_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_meas_config_obj_remove_req->cdma_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_son_oam_anr_meas_config_obj_remove_req->cdma_remove_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_anr_meas_config_obj_remove_req->cdma_remove_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cdma_carrier_freq_info(
                &p_son_oam_anr_meas_config_obj_remove_req->cdma_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_anr_meas_config_obj_remove_res
(
    son_oam_anr_meas_config_obj_remove_res_t *p_son_oam_anr_meas_config_obj_remove_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_anr_meas_config_obj_remove_res, 0, sizeof(son_oam_anr_meas_config_obj_remove_res_t));

    /* This function parses son_oam_anr_meas_config_obj_remove_res */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_anr_meas_config_obj_remove_res");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_anr_meas_config_obj_remove_res->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_meas_config_obj_remove_res->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_anr_meas_config_obj_remove_res->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_anr_meas_config_obj_remove_res->result] should be in range [%d <= 1] !", p_son_oam_anr_meas_config_obj_remove_res->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_anr_meas_config_obj_remove_res->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_rach_opt_enable_req
(
    son_oam_rach_opt_enable_req_t *p_son_oam_rach_opt_enable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_enable_req, 0, sizeof(son_oam_rach_opt_enable_req_t));

    /* This function parses son_oam_rach_opt_enable_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_enable_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_enable_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_rach_opt_enable_req->bitmask & SON_RACH_OPT_ENABLE_SRV_CGI_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_rach_opt_enable_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_rach_opt_enable_resp
(
    son_oam_rach_opt_enable_resp_t *p_son_oam_rach_opt_enable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_enable_resp, 0, sizeof(son_oam_rach_opt_enable_resp_t));

    /* This function parses son_oam_rach_opt_enable_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_enable_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_enable_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_enable_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_enable_resp->result] should be in range [%d <= 1] !", p_son_oam_rach_opt_enable_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_enable_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_rach_opt_enable_resp->cell_status_list_size, p_src + *p_length_read, "cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_enable_resp->cell_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_enable_resp->cell_status_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_rach_opt_enable_resp->cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_rach_opt_enable_resp->cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_status(
                &p_son_oam_rach_opt_enable_resp->cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_rach_opt_disable_req
(
    son_oam_rach_opt_disable_req_t *p_son_oam_rach_opt_disable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_disable_req, 0, sizeof(son_oam_rach_opt_disable_req_t));

    /* This function parses son_oam_rach_opt_disable_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_disable_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_disable_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_rach_opt_disable_req->bitmask & SON_RACH_OPT_DISABLE_SRV_CGI_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_rach_opt_disable_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_rach_opt_disable_resp
(
    son_oam_rach_opt_disable_resp_t *p_son_oam_rach_opt_disable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_disable_resp, 0, sizeof(son_oam_rach_opt_disable_resp_t));

    /* This function parses son_oam_rach_opt_disable_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_disable_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_disable_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_disable_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_disable_resp->result] should be in range [%d <= 1] !", p_son_oam_rach_opt_disable_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_disable_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_rach_opt_disable_resp->cell_status_list_size, p_src + *p_length_read, "cell_status_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_disable_resp->cell_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_disable_resp->cell_status_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_rach_opt_disable_resp->cell_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_rach_opt_disable_resp->cell_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_status(
                &p_son_oam_rach_opt_disable_resp->cell_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_rach_opt_modify_config_params_req
(
    son_oam_rach_opt_modify_config_params_req_t *p_son_oam_rach_opt_modify_config_params_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_modify_config_params_req, 0, sizeof(son_oam_rach_opt_modify_config_params_req_t));

    /* This function parses son_oam_rach_opt_modify_config_params_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_modify_config_params_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_modify_config_params_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_rach_opt_modify_config_params_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_rach_opt_oam_config_param(
        &p_son_oam_rach_opt_modify_config_params_req->rach_config_params,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_rach_opt_oam_config_param
(
    son_rach_opt_oam_config_param_t *p_son_rach_opt_oam_config_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_rach_opt_oam_config_param, 0, sizeof(son_rach_opt_oam_config_param_t));

    /* This function parses son_rach_opt_oam_config_param */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_rach_opt_oam_config_param");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_rach_opt_oam_config_param->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_rach_opt_oam_config_param->bitmask & SON_RACHOPT_REPORT_PERIODICITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_rach_opt_oam_config_param->rachopt_report_periodicity, p_src + *p_length_read, "rachopt_report_periodicity");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rach_opt_oam_config_param->rachopt_report_periodicity < 1) || (p_son_rach_opt_oam_config_param->rachopt_report_periodicity > 600))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rach_opt_oam_config_param->rachopt_report_periodicity] should be in range [1 <= %d <= 600] !", p_son_rach_opt_oam_config_param->rachopt_report_periodicity);
        return SON_FAILURE;
    }
    }

    if (p_son_rach_opt_oam_config_param->bitmask & SON_RACHOPT_MIN_UE_RACH_REPORT_COUNT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_rach_opt_oam_config_param->rachopt_min_ue_rach_report_count, p_src + *p_length_read, "rachopt_min_ue_rach_report_count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rach_opt_oam_config_param->rachopt_min_ue_rach_report_count < 10) || (p_son_rach_opt_oam_config_param->rachopt_min_ue_rach_report_count > 1000))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rach_opt_oam_config_param->rachopt_min_ue_rach_report_count] should be in range [10 <= %d <= 1000] !", p_son_rach_opt_oam_config_param->rachopt_min_ue_rach_report_count);
        return SON_FAILURE;
    }
    }

    if (p_son_rach_opt_oam_config_param->bitmask & SON_RACHOPT_UE_RACH_ATTEMPTS_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_rach_opt_oam_config_param->rachopt_ue_rach_attempts_threshold, p_src + *p_length_read, "rachopt_ue_rach_attempts_threshold");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rach_opt_oam_config_param->rachopt_ue_rach_attempts_threshold < 2) || (p_son_rach_opt_oam_config_param->rachopt_ue_rach_attempts_threshold > 10))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rach_opt_oam_config_param->rachopt_ue_rach_attempts_threshold] should be in range [2 <= %d <= 10] !", p_son_rach_opt_oam_config_param->rachopt_ue_rach_attempts_threshold);
        return SON_FAILURE;
    }
    }

    if (p_son_rach_opt_oam_config_param->bitmask & SON_RACHOPT_START_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_rach_opt_oam_config_param->rachopt_start_threshold, p_src + *p_length_read, "rachopt_start_threshold");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rach_opt_oam_config_param->rachopt_start_threshold < 1) || (p_son_rach_opt_oam_config_param->rachopt_start_threshold > 1000))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rach_opt_oam_config_param->rachopt_start_threshold] should be in range [1 <= %d <= 1000] !", p_son_rach_opt_oam_config_param->rachopt_start_threshold);
        return SON_FAILURE;
    }
    }

    if (p_son_rach_opt_oam_config_param->bitmask & SON_RACHOPT_STOP_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_rach_opt_oam_config_param->rachopt_stop_threshold, p_src + *p_length_read, "rachopt_stop_threshold");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rach_opt_oam_config_param->rachopt_stop_threshold < 1) || (p_son_rach_opt_oam_config_param->rachopt_stop_threshold > 1000))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rach_opt_oam_config_param->rachopt_stop_threshold] should be in range [1 <= %d <= 1000] !", p_son_rach_opt_oam_config_param->rachopt_stop_threshold);
        return SON_FAILURE;
    }
    }

    if (p_son_rach_opt_oam_config_param->bitmask & SON_RACHOPT_DEGRADE_STOP_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_rach_opt_oam_config_param->rachopt_degrade_stop_offset, p_src + *p_length_read, "rachopt_degrade_stop_offset");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rach_opt_oam_config_param->rachopt_degrade_stop_offset < 1) || (p_son_rach_opt_oam_config_param->rachopt_degrade_stop_offset > 1000))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rach_opt_oam_config_param->rachopt_degrade_stop_offset] should be in range [1 <= %d <= 1000] !", p_son_rach_opt_oam_config_param->rachopt_degrade_stop_offset);
        return SON_FAILURE;
    }
    }

    if (p_son_rach_opt_oam_config_param->bitmask & SON_RACHOPT_MAX_ITERATIONS_TO_REACH_STOP_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_rach_opt_oam_config_param->rachopt_max_iterations_to_reach_stop_threshold, p_src + *p_length_read, "rachopt_max_iterations_to_reach_stop_threshold");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rach_opt_oam_config_param->rachopt_max_iterations_to_reach_stop_threshold < 1) || (p_son_rach_opt_oam_config_param->rachopt_max_iterations_to_reach_stop_threshold > 10))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rach_opt_oam_config_param->rachopt_max_iterations_to_reach_stop_threshold] should be in range [1 <= %d <= 10] !", p_son_rach_opt_oam_config_param->rachopt_max_iterations_to_reach_stop_threshold);
        return SON_FAILURE;
    }
    }

    if (p_son_rach_opt_oam_config_param->bitmask & SON_RACHOPT_CONTENTION_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_rach_opt_oam_config_param->rachopt_contention_threshold, p_src + *p_length_read, "rachopt_contention_threshold");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rach_opt_oam_config_param->rachopt_contention_threshold < 1) || (p_son_rach_opt_oam_config_param->rachopt_contention_threshold > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rach_opt_oam_config_param->rachopt_contention_threshold] should be in range [1 <= %d <= 100] !", p_son_rach_opt_oam_config_param->rachopt_contention_threshold);
        return SON_FAILURE;
    }
    }

    if (p_son_rach_opt_oam_config_param->bitmask & SON_RACHOPT_POWER_RAMPUP_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_rach_opt_oam_config_param->rachopt_power_rampup_threshold, p_src + *p_length_read, "rachopt_power_rampup_threshold");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rach_opt_oam_config_param->rachopt_power_rampup_threshold < 1) || (p_son_rach_opt_oam_config_param->rachopt_power_rampup_threshold > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rach_opt_oam_config_param->rachopt_power_rampup_threshold] should be in range [1 <= %d <= 100] !", p_son_rach_opt_oam_config_param->rachopt_power_rampup_threshold);
        return SON_FAILURE;
    }
    }

    if (p_son_rach_opt_oam_config_param->bitmask & SON_RACHOPT_MIN_CFRA_PREAMBLES_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_rach_opt_oam_config_param->rachopt_min_cfra_preambles, p_src + *p_length_read, "rachopt_min_cfra_preambles");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rach_opt_oam_config_param->rachopt_min_cfra_preambles < 4) || (p_son_rach_opt_oam_config_param->rachopt_min_cfra_preambles > 60))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rach_opt_oam_config_param->rachopt_min_cfra_preambles] should be in range [4 <= %d <= 60] !", p_son_rach_opt_oam_config_param->rachopt_min_cfra_preambles);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_rach_opt_modify_config_params_resp
(
    son_oam_rach_opt_modify_config_params_resp_t *p_son_oam_rach_opt_modify_config_params_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_modify_config_params_resp, 0, sizeof(son_oam_rach_opt_modify_config_params_resp_t));

    /* This function parses son_oam_rach_opt_modify_config_params_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_modify_config_params_resp");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_rach_opt_modify_config_params_resp->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_modify_config_params_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_modify_config_params_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_modify_config_params_resp->result] should be in range [%d <= 2] !", p_son_oam_rach_opt_modify_config_params_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_modify_config_params_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_modify_config_params_resp->failed_param_bitmask, p_src + *p_length_read, "failed_param_bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_rach_opt_modify_rach_optmode_req
(
    son_oam_rach_opt_modify_rach_optmode_req_t *p_son_oam_rach_opt_modify_rach_optmode_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_modify_rach_optmode_req, 0, sizeof(son_oam_rach_opt_modify_rach_optmode_req_t));

    /* This function parses son_oam_rach_opt_modify_rach_optmode_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_modify_rach_optmode_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_modify_rach_optmode_req->rachopt_mode, p_src + *p_length_read, "rachopt_mode");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_modify_rach_optmode_req->rachopt_mode > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_modify_rach_optmode_req->rachopt_mode] should be in range [%d <= 1] !", p_son_oam_rach_opt_modify_rach_optmode_req->rachopt_mode);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_rach_opt_modify_rach_optmode_resp
(
    son_oam_rach_opt_modify_rach_optmode_resp_t *p_son_oam_rach_opt_modify_rach_optmode_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_modify_rach_optmode_resp, 0, sizeof(son_oam_rach_opt_modify_rach_optmode_resp_t));

    /* This function parses son_oam_rach_opt_modify_rach_optmode_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_modify_rach_optmode_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_modify_rach_optmode_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_modify_rach_optmode_resp->result > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_modify_rach_optmode_resp->result] should be in range [%d <= 2] !", p_son_oam_rach_opt_modify_rach_optmode_resp->result);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_modify_rach_optmode_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_rach_opt_failure_notification
(
    son_oam_rach_opt_failure_notification_t *p_son_oam_rach_opt_failure_notification,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_failure_notification, 0, sizeof(son_oam_rach_opt_failure_notification_t));

    /* This function parses son_oam_rach_opt_failure_notification */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_failure_notification");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_failure_notification->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_rach_opt_failure_notification->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_failure_notification->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_rach_opt_failure_notification->cause < 1) || (p_son_oam_rach_opt_failure_notification->cause > 255))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_failure_notification->cause] should be in range [1 <= %d <= 255] !", p_son_oam_rach_opt_failure_notification->cause);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_failure_notification->rach_failure_level, p_src + *p_length_read, "rach_failure_level");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_failure_notification->rach_failure_level > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_failure_notification->rach_failure_level] should be in range [%d <= 1] !", p_son_oam_rach_opt_failure_notification->rach_failure_level);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_failure_notification->identified_opt_type, p_src + *p_length_read, "identified_opt_type");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_rach_opt_parameter_update_req
(
    son_oam_rach_opt_parameter_update_req_t *p_son_oam_rach_opt_parameter_update_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_parameter_update_req, 0, sizeof(son_oam_rach_opt_parameter_update_req_t));

    /* This function parses son_oam_rach_opt_parameter_update_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_parameter_update_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_parameter_update_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_rach_opt_parameter_update_req->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;



    if (SON_FAILURE == son_parse_son_oam_rach_opt_update_req_param(
        &p_son_oam_rach_opt_parameter_update_req->update_param,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_oam_rach_opt_update_req_param
(
    son_oam_rach_opt_update_req_param_t *p_son_oam_rach_opt_update_req_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_update_req_param, 0, sizeof(son_oam_rach_opt_update_req_param_t));

    /* This function parses son_oam_rach_opt_update_req_param */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_update_req_param");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_update_req_param->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_oam_rach_opt_update_req_param->bitmask & SON_UPDATE_PREAMBLE_INIT_RCVD_TGT_PWR_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_update_req_param->preamble_initial_rcvd_target_power, p_src + *p_length_read, "preamble_initial_rcvd_target_power");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_update_req_param->preamble_initial_rcvd_target_power > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_update_req_param->preamble_initial_rcvd_target_power] should be in range [%d <= 15] !", p_son_oam_rach_opt_update_req_param->preamble_initial_rcvd_target_power);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_rach_opt_update_req_param->bitmask & SON_UPDATE_NUM_OF_RA_PREAMBLES_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_update_req_param->number_of_ra_preambles, p_src + *p_length_read, "number_of_ra_preambles");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_update_req_param->number_of_ra_preambles > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_update_req_param->number_of_ra_preambles] should be in range [%d <= 15] !", p_son_oam_rach_opt_update_req_param->number_of_ra_preambles);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_rach_opt_update_req_param->bitmask & SON_UPDATE_SIZE_OF_RA_GROUP_A_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_rach_opt_update_req_param->size_of_ra_group_a, p_src + *p_length_read, "size_of_ra_group_a");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_update_req_param->size_of_ra_group_a > 14))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_update_req_param->size_of_ra_group_a] should be in range [%d <= 14] !", p_son_oam_rach_opt_update_req_param->size_of_ra_group_a);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_rach_opt_update_req_param->bitmask & SON_UPDATE_ROOT_SEQ_IDX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_rach_opt_update_req_param->root_sequence_index, p_src + *p_length_read, "root_sequence_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_update_req_param->root_sequence_index > 837))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_update_req_param->root_sequence_index] should be in range [%d <= 837] !", p_son_oam_rach_opt_update_req_param->root_sequence_index);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_rach_opt_update_req_param->bitmask & SON_UPDATE_PRACH_FREQ_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_rach_opt_update_req_param->prach_frequency_offset, p_src + *p_length_read, "prach_frequency_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_update_req_param->prach_frequency_offset > 94))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_update_req_param->prach_frequency_offset] should be in range [%d <= 94] !", p_son_oam_rach_opt_update_req_param->prach_frequency_offset);
        return SON_FAILURE;
    }
    }

    if (p_son_oam_rach_opt_update_req_param->bitmask & SON_UPDATE_PRACH_CONFIG_IDX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_rach_opt_update_req_param->prach_configuration_index, p_src + *p_length_read, "prach_configuration_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_update_req_param->prach_configuration_index > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_update_req_param->prach_configuration_index] should be in range [%d <= 63] !", p_son_oam_rach_opt_update_req_param->prach_configuration_index);
        return SON_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_rach_opt_ue_info_report
(
    son_oam_rach_opt_ue_info_report_t *p_son_oam_rach_opt_ue_info_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_ue_info_report, 0, sizeof(son_oam_rach_opt_ue_info_report_t));

    /* This function parses son_oam_rach_opt_ue_info_report */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_ue_info_report");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_rach_opt_ue_info_report->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_rach_opt_ue_info_report->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_rach_opt_ue_info_report->count > SON_MAX_NUMBER_OF_PRACH_SAMPLES))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_rach_opt_ue_info_report->count] should be in range [%d <= SON_MAX_NUMBER_OF_PRACH_SAMPLES] !", p_son_oam_rach_opt_ue_info_report->count);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_rach_opt_ue_info_report->count; loop++)
        {
            if (SON_FAILURE == son_parse_son_ue_prach_data_info(
                &p_son_oam_rach_opt_ue_info_report->rach_data[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_strong_cell_info
(
    son_strong_cell_info_t *p_son_strong_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_strong_cell_info, 0, sizeof(son_strong_cell_info_t));

    /* This function parses son_strong_cell_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_strong_cell_info");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_strong_cell_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_strong_cell_info->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_strong_cell_info->pci] should be in range [%d <= 503] !", p_son_strong_cell_info->pci);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_strong_cell_info->rsrp, p_src + *p_length_read, "rsrp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_strong_cell_info->rsrp > 128))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_strong_cell_info->rsrp] should be in range [%d <= 128] !", p_son_strong_cell_info->rsrp);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_ue_prach_data_info
(
    son_ue_prach_data_info_t *p_son_ue_prach_data_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_ue_prach_data_info, 0, sizeof(son_ue_prach_data_info_t));

    /* This function parses son_ue_prach_data_info */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_ue_prach_data_info");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_ue_prach_data_info->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_ue_prach_data_info->cause < 1) || (p_son_ue_prach_data_info->cause > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ue_prach_data_info->cause] should be in range [1 <= %d <= 3] !", p_son_ue_prach_data_info->cause);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_ue_prach_data_info->number_of_preambles_sent, p_src + *p_length_read, "number_of_preambles_sent");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_ue_prach_data_info->contention_detected, p_src + *p_length_read, "contention_detected");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ue_prach_data_info->contention_detected > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ue_prach_data_info->contention_detected] should be in range [%d <= 1] !", p_son_ue_prach_data_info->contention_detected);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_ue_prach_data_info->ue_location, p_src + *p_length_read, "ue_location");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ue_prach_data_info->ue_location > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ue_prach_data_info->ue_location] should be in range [%d <= 1] !", p_son_ue_prach_data_info->ue_location);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_ue_prach_data_info->strong_cell_info_count, p_src + *p_length_read, "strong_cell_info_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_ue_prach_data_info->strong_cell_info_count > SON_STRONG_CELL_INFO_COUNT))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_ue_prach_data_info->strong_cell_info_count] should be in range [%d <= SON_STRONG_CELL_INFO_COUNT] !", p_son_ue_prach_data_info->strong_cell_info_count);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_ue_prach_data_info->strong_cell_info_count; loop++)
        {
            if (SON_FAILURE == son_parse_son_strong_cell_info(
                &p_son_ue_prach_data_info->strong_cell_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_rach_opt_l2_perf_report
(
    son_oam_rach_opt_l2_perf_report_t *p_son_oam_rach_opt_l2_perf_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_rach_opt_l2_perf_report, 0, sizeof(son_oam_rach_opt_l2_perf_report_t));

    /* This function parses son_oam_rach_opt_l2_perf_report */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_rach_opt_l2_perf_report");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_oam_rach_opt_l2_perf_report->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_rach_opt_l2_perf_report->num_of_l2_perf_accumulated_reports, p_src + *p_length_read, "num_of_l2_perf_accumulated_reports");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U64(&p_son_oam_rach_opt_l2_perf_report->total_rcvd_msg3_ra_preambles_cbra, p_src + *p_length_read, "total_rcvd_msg3_ra_preambles_cbra");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U64(&p_son_oam_rach_opt_l2_perf_report->total_rcvd_msg3_ra_preambles_cfra, p_src + *p_length_read, "total_rcvd_msg3_ra_preambles_cfra");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U64(&p_son_oam_rach_opt_l2_perf_report->total_failed_ra_preambles_msg2_cbra, p_src + *p_length_read, "total_failed_ra_preambles_msg2_cbra");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U64(&p_son_oam_rach_opt_l2_perf_report->total_failed_ra_preambles_msg2_cfra, p_src + *p_length_read, "total_failed_ra_preambles_msg2_cfra");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U64(&p_son_oam_rach_opt_l2_perf_report->total_allocated_ra_preambles, p_src + *p_length_read, "total_allocated_ra_preambles");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(p_son_oam_rach_opt_l2_perf_report->total_succ_ra_ta) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_oam_rach_opt_l2_perf_report->total_succ_ra_ta); loop++)
        {
            son_cp_unpack_U64((void*)&p_son_oam_rach_opt_l2_perf_report->total_succ_ra_ta[loop], (void*)(p_src + *p_length_read), "total_succ_ra_ta[]");
            *p_length_read += sizeof(U64);
        }
    }

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U64(&p_son_oam_rach_opt_l2_perf_report->total_unassigned_false_cfra, p_src + *p_length_read, "total_unassigned_false_cfra");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U64(&p_son_oam_rach_opt_l2_perf_report->total_unassigned_sum_cfra, p_src + *p_length_read, "total_unassigned_sum_cfra");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U64(&p_son_oam_rach_opt_l2_perf_report->num_random_access_preambles_group_a, p_src + *p_length_read, "num_random_access_preambles_group_a");
    *p_length_read += sizeof(U64);

    if (*p_length_read + (S32)sizeof(U64) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U64(&p_son_oam_rach_opt_l2_perf_report->num_random_access_preambles_group_b, p_src + *p_length_read, "num_random_access_preambles_group_b");
    *p_length_read += sizeof(U64);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_cell_delete_req
(
    son_oam_cell_delete_req_t *p_son_oam_cell_delete_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_cell_delete_req, 0, sizeof(son_oam_cell_delete_req_t));

    /* This function parses son_oam_cell_delete_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_cell_delete_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_cell_delete_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_cell_delete_req->cell_delete_list_size, p_src + *p_length_read, "cell_delete_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_oam_cell_delete_req->cell_delete_list_size < 1) || (p_son_oam_cell_delete_req->cell_delete_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_cell_delete_req->cell_delete_list_size] should be in range [1 <= %d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_cell_delete_req->cell_delete_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_cell_delete_req->cell_delete_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
                &p_son_oam_cell_delete_req->cell_delete_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_cell_delete_resp
(
    son_oam_cell_delete_resp_t *p_son_oam_cell_delete_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_cell_delete_resp, 0, sizeof(son_oam_cell_delete_resp_t));

    /* This function parses son_oam_cell_delete_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_cell_delete_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_cell_delete_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_oam_cell_delete_resp->cell_delete_status_list_size, p_src + *p_length_read, "cell_delete_status_list_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_oam_cell_delete_resp->cell_delete_status_list_size > SON_MAX_CELL_SUPPORTED_AT_ENB))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_oam_cell_delete_resp->cell_delete_status_list_size] should be in range [%d <= SON_MAX_CELL_SUPPORTED_AT_ENB] !", p_son_oam_cell_delete_resp->cell_delete_status_list_size);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_oam_cell_delete_resp->cell_delete_status_list_size; loop++)
        {
            if (SON_FAILURE == son_parse_son_cell_delete_status(
                &p_son_oam_cell_delete_resp->cell_delete_status_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_cell_delete_status
(
    son_cell_delete_status_t *p_son_cell_delete_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_cell_delete_status, 0, sizeof(son_cell_delete_status_t));

    /* This function parses son_cell_delete_status */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_cell_delete_status");



    if (SON_FAILURE == son_parse_son_intra_rat_global_cell_id(
        &p_son_cell_delete_status->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_cell_delete_status->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_get_debug_info_req
(
    son_oam_get_debug_info_req_t *p_son_oam_get_debug_info_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_get_debug_info_req, 0, sizeof(son_oam_get_debug_info_req_t));

    /* This function parses son_oam_get_debug_info_req */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_get_debug_info_req");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_oam_get_debug_info_req->debug_type, p_src + *p_length_read, "debug_type");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_oam_get_debug_info_resp
(
    son_oam_get_debug_info_resp_t *p_son_oam_get_debug_info_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_oam_get_debug_info_resp, 0, sizeof(son_oam_get_debug_info_resp_t));

    /* This function parses son_oam_get_debug_info_resp */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_oam_get_debug_info_resp");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_oam_get_debug_info_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(S32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S32(&p_son_oam_get_debug_info_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(S32);

    if (p_son_oam_get_debug_info_resp->bitmask & SON_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_S32(&p_son_oam_get_debug_info_resp->fail_cause, p_src + *p_length_read, "fail_cause");
    *p_length_read += sizeof(S32);
    }

    if (p_son_oam_get_debug_info_resp->bitmask & SON_OAM_DEBUG_GET_MEM_POOL_PRESENT)
    {



    if (SON_FAILURE == son_parse_son_MemPoolStats(
        &p_son_oam_get_debug_info_resp->csplMemPoolStats,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return SON_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_PoolStats
(
    son_PoolStats *p_son_PoolStats,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_son_PoolStats, 0, sizeof(son_PoolStats));

    /* This function parses son_PoolStats */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_PoolStats");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_PoolStats->bufSize, p_src + *p_length_read, "bufSize");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_PoolStats->numOfBuf, p_src + *p_length_read, "numOfBuf");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_PoolStats->numOfAllocBuf, p_src + *p_length_read, "numOfAllocBuf");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_PoolStats->numOfPeakBuf, p_src + *p_length_read, "numOfPeakBuf");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_son_MemPoolStats
(
    son_MemPoolStats *p_son_MemPoolStats,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_MemPoolStats, 0, sizeof(son_MemPoolStats));

    /* This function parses son_MemPoolStats */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "Parsing son_MemPoolStats");

    /* Parse OCTET_STRING FIXED of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_MemPoolStats->stats); loop++)
        {
            if (SON_FAILURE == son_parse_son_PoolStats(
                &p_son_MemPoolStats->stats[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

