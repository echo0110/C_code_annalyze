/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_son_composer.c $
 *
 *******************************************************************************
 *
 * File Description: RRM SON composer files for all RRM SON composer functions.
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "rrm_pack_unpack_utils.h"
#include "rrm_son_composer.h"
#include "rrm_son_intf.h"
#include "rrm_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrm_length_t
rrm_get_rrm_oam_cell_plmn_info_len
(
    rrm_oam_cell_plmn_info_t *p_rrm_oam_cell_plmn_info
);

static
rrm_return_et
rrm_compose_rrm_oam_cell_plmn_info
(
    U8  **pp_buffer,
    rrm_oam_cell_plmn_info_t *p_rrm_oam_cell_plmn_info
);

static
rrm_length_t
rrm_get_rrm_oam_eutran_global_cell_id_len
(
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id
);

static
rrm_return_et
rrm_compose_rrm_oam_eutran_global_cell_id
(
    U8  **pp_buffer,
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id
);

static
rrm_length_t
rrm_get_rrm_cell_object_id_len
(
    rrm_cell_object_id_t *p_rrm_cell_object_id
);

static
rrm_return_et
rrm_compose_rrm_cell_object_id
(
    U8  **pp_buffer,
    rrm_cell_object_id_t *p_rrm_cell_object_id
);

static
rrm_length_t
rrm_get_rrm_object_id_len
(
    rrm_object_id_t *p_rrm_object_id
);

static
rrm_return_et
rrm_compose_rrm_object_id
(
    U8  **pp_buffer,
    rrm_object_id_t *p_rrm_object_id
);

static
rrm_length_t
rrm_get_rrm_trgr_periodic_value_len
(
    rrm_trgr_periodic_value_t *p_rrm_trgr_periodic_value
);

static
rrm_return_et
rrm_compose_rrm_trgr_periodic_value
(
    U8  **pp_buffer,
    rrm_trgr_periodic_value_t *p_rrm_trgr_periodic_value
);

static
rrm_length_t
rrm_get_rrm_trgr_threshold_value_len
(
    rrm_trgr_threshold_value_t *p_rrm_trgr_threshold_value
);

static
rrm_return_et
rrm_compose_rrm_trgr_threshold_value
(
    U8  **pp_buffer,
    rrm_trgr_threshold_value_t *p_rrm_trgr_threshold_value
);

static
rrm_length_t
rrm_get_rrm_trgr_event_value_len
(
    rrm_trgr_event_value_t *p_rrm_trgr_event_value
);

static
rrm_return_et
rrm_compose_rrm_trgr_event_value
(
    U8  **pp_buffer,
    rrm_trgr_event_value_t *p_rrm_trgr_event_value
);

static
rrm_length_t
rrm_get_rrm_trigger_len
(
    rrm_trigger_t *p_rrm_trigger
);

static
rrm_return_et
rrm_compose_rrm_trigger
(
    U8  **pp_buffer,
    rrm_trigger_t *p_rrm_trigger
);

static
rrm_length_t
rrm_get_rrm_attribute_len
(
    rrm_attribute_t *p_rrm_attribute
);

static
rrm_return_et
rrm_compose_rrm_attribute
(
    U8  **pp_buffer,
    rrm_attribute_t *p_rrm_attribute
);

static
rrm_length_t
rrm_get_object_len
(
    object_t *p_object
);

static
rrm_return_et
rrm_compose_object
(
    U8  **pp_buffer,
    object_t *p_object
);

static
rrm_length_t
rrm_get_rrm_son_register_deregister_format_len
(
    rrm_son_register_deregister_format_t *p_rrm_son_register_deregister_format
);

static
rrm_return_et
rrm_compose_rrm_son_register_deregister_format
(
    U8  **pp_buffer,
    rrm_son_register_deregister_format_t *p_rrm_son_register_deregister_format
);

static
rrm_length_t
rrm_get_rrm_son_non_registration_based_res_len
(
    rrm_son_non_registration_based_res_t *p_rrm_son_non_registration_based_res
);

static
rrm_return_et
rrm_compose_rrm_son_non_registration_based_res
(
    U8  **pp_buffer,
    rrm_son_non_registration_based_res_t *p_rrm_son_non_registration_based_res
);

static
rrm_length_t
rrm_get_rrm_son_registration_based_res_len
(
    rrm_son_registration_based_res_t *p_rrm_son_registration_based_res
);

static
rrm_return_et
rrm_compose_rrm_son_registration_based_res
(
    U8  **pp_buffer,
    rrm_son_registration_based_res_t *p_rrm_son_registration_based_res
);

static
rrm_length_t
rrm_get_rrm_son_nmm_res_len
(
    rrm_son_nmm_res_t *p_rrm_son_nmm_res
);

static
rrm_return_et
rrm_compose_rrm_son_nmm_res
(
    U8  **pp_buffer,
    rrm_son_nmm_res_t *p_rrm_son_nmm_res
);

static
rrm_length_t
rrm_get_rrm_son_speed_state_scale_factors_len
(
    rrm_son_speed_state_scale_factors_t *p_rrm_son_speed_state_scale_factors
);

static
rrm_return_et
rrm_compose_rrm_son_speed_state_scale_factors
(
    U8  **pp_buffer,
    rrm_son_speed_state_scale_factors_t *p_rrm_son_speed_state_scale_factors
);

static
rrm_length_t
rrm_get_rrm_son_meas_config_eutran_len
(
    rrm_son_meas_config_eutran_t *p_rrm_son_meas_config_eutran
);

static
rrm_return_et
rrm_compose_rrm_son_meas_config_eutran
(
    U8  **pp_buffer,
    rrm_son_meas_config_eutran_t *p_rrm_son_meas_config_eutran
);

static
rrm_length_t
rrm_get_rrm_son_utran_pci_fdd_range_len
(
    rrm_son_utran_pci_fdd_range_t *p_rrm_son_utran_pci_fdd_range
);

static
rrm_return_et
rrm_compose_rrm_son_utran_pci_fdd_range
(
    U8  **pp_buffer,
    rrm_son_utran_pci_fdd_range_t *p_rrm_son_utran_pci_fdd_range
);

static
rrm_length_t
rrm_get_rrm_son_meas_config_utran_fdd_len
(
    rrm_son_meas_config_utran_fdd_t *p_rrm_son_meas_config_utran_fdd
);

static
rrm_return_et
rrm_compose_rrm_son_meas_config_utran_fdd
(
    U8  **pp_buffer,
    rrm_son_meas_config_utran_fdd_t *p_rrm_son_meas_config_utran_fdd
);

static
rrm_length_t
rrm_get_rrm_son_meas_config_utran_tdd_len
(
    rrm_son_meas_config_utran_tdd_t *p_rrm_son_meas_config_utran_tdd
);

static
rrm_return_et
rrm_compose_rrm_son_meas_config_utran_tdd
(
    U8  **pp_buffer,
    rrm_son_meas_config_utran_tdd_t *p_rrm_son_meas_config_utran_tdd
);

static
rrm_length_t
rrm_get_rrm_son_meas_config_utran_len
(
    rrm_son_meas_config_utran_t *p_rrm_son_meas_config_utran
);

static
rrm_return_et
rrm_compose_rrm_son_meas_config_utran
(
    U8  **pp_buffer,
    rrm_son_meas_config_utran_t *p_rrm_son_meas_config_utran
);

static
rrm_length_t
rrm_get_rrm_son_geran_following_arfcns_len
(
    rrm_son_geran_following_arfcns_t *p_rrm_son_geran_following_arfcns
);

static
rrm_return_et
rrm_compose_rrm_son_geran_following_arfcns
(
    U8  **pp_buffer,
    rrm_son_geran_following_arfcns_t *p_rrm_son_geran_following_arfcns
);

static
rrm_length_t
rrm_get_rrm_son_geran_carrier_freqs_len
(
    rrm_son_geran_carrier_freqs_t *p_rrm_son_geran_carrier_freqs
);

static
rrm_return_et
rrm_compose_rrm_son_geran_carrier_freqs
(
    U8  **pp_buffer,
    rrm_son_geran_carrier_freqs_t *p_rrm_son_geran_carrier_freqs
);

static
rrm_length_t
rrm_get_rrm_son_geran_physical_cell_id_len
(
    rrm_son_geran_physical_cell_id_t *p_rrm_son_geran_physical_cell_id
);

static
rrm_return_et
rrm_compose_rrm_son_geran_physical_cell_id
(
    U8  **pp_buffer,
    rrm_son_geran_physical_cell_id_t *p_rrm_son_geran_physical_cell_id
);

static
rrm_length_t
rrm_get_rrm_son_geran_carrier_freq_info_len
(
    rrm_son_geran_carrier_freq_info_t *p_rrm_son_geran_carrier_freq_info
);

static
rrm_return_et
rrm_compose_rrm_son_geran_carrier_freq_info
(
    U8  **pp_buffer,
    rrm_son_geran_carrier_freq_info_t *p_rrm_son_geran_carrier_freq_info
);

static
rrm_length_t
rrm_get_rrm_son_meas_config_geran_len
(
    rrm_son_meas_config_geran_t *p_rrm_son_meas_config_geran
);

static
rrm_return_et
rrm_compose_rrm_son_meas_config_geran
(
    U8  **pp_buffer,
    rrm_son_meas_config_geran_t *p_rrm_son_meas_config_geran
);

static
rrm_length_t
rrm_get_rrm_son_cdma_carrier_freq_info_len
(
    rrm_son_cdma_carrier_freq_info_t *p_rrm_son_cdma_carrier_freq_info
);

static
rrm_return_et
rrm_compose_rrm_son_cdma_carrier_freq_info
(
    U8  **pp_buffer,
    rrm_son_cdma_carrier_freq_info_t *p_rrm_son_cdma_carrier_freq_info
);

static
rrm_length_t
rrm_get_rrm_son_cdma_sys_time_info_len
(
    rrm_son_cdma_sys_time_info_t *p_rrm_son_cdma_sys_time_info
);

static
rrm_return_et
rrm_compose_rrm_son_cdma_sys_time_info
(
    U8  **pp_buffer,
    rrm_son_cdma_sys_time_info_t *p_rrm_son_cdma_sys_time_info
);

static
rrm_length_t
rrm_get_rrm_son_pre_reg_hrpd_info_len
(
    rrm_son_pre_reg_hrpd_info_t *p_rrm_son_pre_reg_hrpd_info
);

static
rrm_return_et
rrm_compose_rrm_son_pre_reg_hrpd_info
(
    U8  **pp_buffer,
    rrm_son_pre_reg_hrpd_info_t *p_rrm_son_pre_reg_hrpd_info
);

static
rrm_length_t
rrm_get_rrm_son_cdma_hrpd_info_len
(
    rrm_son_cdma_hrpd_info_t *p_rrm_son_cdma_hrpd_info
);

static
rrm_return_et
rrm_compose_rrm_son_cdma_hrpd_info
(
    U8  **pp_buffer,
    rrm_son_cdma_hrpd_info_t *p_rrm_son_cdma_hrpd_info
);

static
rrm_length_t
rrm_get_rrm_son_cdma_csfb_reg_param_1XRTT_len
(
    rrm_son_cdma_csfb_reg_param_1XRTT_t *p_rrm_son_cdma_csfb_reg_param_1XRTT
);

static
rrm_return_et
rrm_compose_rrm_son_cdma_csfb_reg_param_1XRTT
(
    U8  **pp_buffer,
    rrm_son_cdma_csfb_reg_param_1XRTT_t *p_rrm_son_cdma_csfb_reg_param_1XRTT
);

static
rrm_length_t
rrm_get_rrm_son_cdma_1xrtt_info_len
(
    rrm_son_cdma_1xrtt_info_t *p_rrm_son_cdma_1xrtt_info
);

static
rrm_return_et
rrm_compose_rrm_son_cdma_1xrtt_info
(
    U8  **pp_buffer,
    rrm_son_cdma_1xrtt_info_t *p_rrm_son_cdma_1xrtt_info
);

static
rrm_length_t
rrm_get_rrm_son_cdma_csfb_reg_param_1xrtt_r9_len
(
    rrm_son_cdma_csfb_reg_param_1xrtt_r9_t *p_rrm_son_cdma_csfb_reg_param_1xrtt_r9
);

static
rrm_return_et
rrm_compose_rrm_son_cdma_csfb_reg_param_1xrtt_r9
(
    U8  **pp_buffer,
    rrm_son_cdma_csfb_reg_param_1xrtt_r9_t *p_rrm_son_cdma_csfb_reg_param_1xrtt_r9
);

static
rrm_length_t
rrm_get_rrm_son_ac_barring_config_1xrtt_len
(
    rrm_son_ac_barring_config_1xrtt_t *p_rrm_son_ac_barring_config_1xrtt
);

static
rrm_return_et
rrm_compose_rrm_son_ac_barring_config_1xrtt
(
    U8  **pp_buffer,
    rrm_son_ac_barring_config_1xrtt_t *p_rrm_son_ac_barring_config_1xrtt
);

static
rrm_length_t
rrm_get_rrm_son_meas_config_cdma_len
(
    rrm_son_meas_config_cdma_t *p_rrm_son_meas_config_cdma
);

static
rrm_return_et
rrm_compose_rrm_son_meas_config_cdma
(
    U8  **pp_buffer,
    rrm_son_meas_config_cdma_t *p_rrm_son_meas_config_cdma
);

static
rrm_length_t
rrm_get_rrm_son_inter_rat_meas_config_len
(
    rrm_son_inter_rat_meas_config_t *p_rrm_son_inter_rat_meas_config
);

static
rrm_return_et
rrm_compose_rrm_son_inter_rat_meas_config
(
    U8  **pp_buffer,
    rrm_son_inter_rat_meas_config_t *p_rrm_son_inter_rat_meas_config
);

static
rrm_length_t
rrm_get_rrm_son_setup_meas_gap_config_len
(
    rrm_son_setup_meas_gap_config_t *p_rrm_son_setup_meas_gap_config
);

static
rrm_return_et
rrm_compose_rrm_son_setup_meas_gap_config
(
    U8  **pp_buffer,
    rrm_son_setup_meas_gap_config_t *p_rrm_son_setup_meas_gap_config
);

static
rrm_length_t
rrm_get_rrm_son_gap_config_len
(
    rrm_son_gap_config_t *p_rrm_son_gap_config
);

static
rrm_return_et
rrm_compose_rrm_son_gap_config
(
    U8  **pp_buffer,
    rrm_son_gap_config_t *p_rrm_son_gap_config
);

static
rrm_length_t
rrm_get_rrm_son_meas_gap_config_len
(
    rrm_son_meas_gap_config_t *p_rrm_son_meas_gap_config
);

static
rrm_return_et
rrm_compose_rrm_son_meas_gap_config
(
    U8  **pp_buffer,
    rrm_son_meas_gap_config_t *p_rrm_son_meas_gap_config
);

static
rrm_length_t
rrm_get_rrm_son_eutran_neighbor_cell_len
(
    rrm_son_eutran_neighbor_cell_t *p_rrm_son_eutran_neighbor_cell
);

static
rrm_return_et
rrm_compose_rrm_son_eutran_neighbor_cell
(
    U8  **pp_buffer,
    rrm_son_eutran_neighbor_cell_t *p_rrm_son_eutran_neighbor_cell
);

static
rrm_length_t
rrm_get_rrm_son_eutran_meas_results_len
(
    rrm_son_eutran_meas_results_t *p_rrm_son_eutran_meas_results
);

static
rrm_return_et
rrm_compose_rrm_son_eutran_meas_results
(
    U8  **pp_buffer,
    rrm_son_eutran_meas_results_t *p_rrm_son_eutran_meas_results
);

static
rrm_length_t
rrm_get_rrm_son_utran_physical_cell_id_len
(
    rrm_son_utran_physical_cell_id_t *p_rrm_son_utran_physical_cell_id
);

static
rrm_return_et
rrm_compose_rrm_son_utran_physical_cell_id
(
    U8  **pp_buffer,
    rrm_son_utran_physical_cell_id_t *p_rrm_son_utran_physical_cell_id
);

static
rrm_length_t
rrm_get_rrm_son_utran_geran_global_cell_id_len
(
    rrm_son_utran_geran_global_cell_id_t *p_rrm_son_utran_geran_global_cell_id
);

static
rrm_return_et
rrm_compose_rrm_son_utran_geran_global_cell_id
(
    U8  **pp_buffer,
    rrm_son_utran_geran_global_cell_id_t *p_rrm_son_utran_geran_global_cell_id
);

static
rrm_length_t
rrm_get_rrm_son_utran_neighbor_cell_len
(
    rrm_son_utran_neighbor_cell_t *p_rrm_son_utran_neighbor_cell
);

static
rrm_return_et
rrm_compose_rrm_son_utran_neighbor_cell
(
    U8  **pp_buffer,
    rrm_son_utran_neighbor_cell_t *p_rrm_son_utran_neighbor_cell
);

static
rrm_length_t
rrm_get_rrm_son_utran_meas_results_len
(
    rrm_son_utran_meas_results_t *p_rrm_son_utran_meas_results
);

static
rrm_return_et
rrm_compose_rrm_son_utran_meas_results
(
    U8  **pp_buffer,
    rrm_son_utran_meas_results_t *p_rrm_son_utran_meas_results
);

static
rrm_length_t
rrm_get_rrm_son_geran_neighbor_cell_len
(
    rrm_son_geran_neighbor_cell_t *p_rrm_son_geran_neighbor_cell
);

static
rrm_return_et
rrm_compose_rrm_son_geran_neighbor_cell
(
    U8  **pp_buffer,
    rrm_son_geran_neighbor_cell_t *p_rrm_son_geran_neighbor_cell
);

static
rrm_length_t
rrm_get_rrm_son_geran_meas_results_len
(
    rrm_son_geran_meas_results_t *p_rrm_son_geran_meas_results
);

static
rrm_return_et
rrm_compose_rrm_son_geran_meas_results
(
    U8  **pp_buffer,
    rrm_son_geran_meas_results_t *p_rrm_son_geran_meas_results
);

static
rrm_length_t
rrm_get_rrm_son_cdma_global_cell_id_len
(
    rrm_son_cdma_global_cell_id_t *p_rrm_son_cdma_global_cell_id
);

static
rrm_return_et
rrm_compose_rrm_son_cdma_global_cell_id
(
    U8  **pp_buffer,
    rrm_son_cdma_global_cell_id_t *p_rrm_son_cdma_global_cell_id
);

static
rrm_length_t
rrm_get_rrm_son_cdma_neighbor_cell_len
(
    rrm_son_cdma_neighbor_cell_t *p_rrm_son_cdma_neighbor_cell
);

static
rrm_return_et
rrm_compose_rrm_son_cdma_neighbor_cell
(
    U8  **pp_buffer,
    rrm_son_cdma_neighbor_cell_t *p_rrm_son_cdma_neighbor_cell
);

static
rrm_length_t
rrm_get_rrm_son_cdma_meas_results_len
(
    rrm_son_cdma_meas_results_t *p_rrm_son_cdma_meas_results
);

static
rrm_return_et
rrm_compose_rrm_son_cdma_meas_results
(
    U8  **pp_buffer,
    rrm_son_cdma_meas_results_t *p_rrm_son_cdma_meas_results
);

static
rrm_length_t
rrm_get_rrm_utran_geran_global_cell_id_len
(
    rrm_utran_geran_global_cell_id_t *p_rrm_utran_geran_global_cell_id
);

static
rrm_return_et
rrm_compose_rrm_utran_geran_global_cell_id
(
    U8  **pp_buffer,
    rrm_utran_geran_global_cell_id_t *p_rrm_utran_geran_global_cell_id
);

static
rrm_length_t
rrm_get_rrm_others_global_cell_id_len
(
    rrm_others_global_cell_id_t *p_rrm_others_global_cell_id
);

static
rrm_return_et
rrm_compose_rrm_others_global_cell_id
(
    U8  **pp_buffer,
    rrm_others_global_cell_id_t *p_rrm_others_global_cell_id
);

static
rrm_length_t
rrm_get_rrm_inter_rat_global_cell_id_len
(
    rrm_inter_rat_global_cell_id_t *p_rrm_inter_rat_global_cell_id
);

static
rrm_return_et
rrm_compose_rrm_inter_rat_global_cell_id
(
    U8  **pp_buffer,
    rrm_inter_rat_global_cell_id_t *p_rrm_inter_rat_global_cell_id
);

static
rrm_length_t
rrm_get_rrm_oam_global_cell_id_len
(
    rrm_oam_global_cell_id_t *p_rrm_oam_global_cell_id
);

static
rrm_return_et
rrm_compose_rrm_oam_global_cell_id
(
    U8  **pp_buffer,
    rrm_oam_global_cell_id_t *p_rrm_oam_global_cell_id
);

static
rrm_length_t
rrm_get_rrm_son_offset_attr_len
(
    rrm_son_offset_attr_t *p_rrm_son_offset_attr
);

static
rrm_return_et
rrm_compose_rrm_son_offset_attr
(
    U8  **pp_buffer,
    rrm_son_offset_attr_t *p_rrm_son_offset_attr
);

static
rrm_length_t
rrm_get_rrm_son_eutran_strength_len
(
    rrm_son_eutran_strength_t *p_rrm_son_eutran_strength
);

static
rrm_return_et
rrm_compose_rrm_son_eutran_strength
(
    U8  **pp_buffer,
    rrm_son_eutran_strength_t *p_rrm_son_eutran_strength
);

static
rrm_length_t
rrm_get_rrm_son_utran_strength_len
(
    rrm_son_utran_strength_t *p_rrm_son_utran_strength
);

static
rrm_return_et
rrm_compose_rrm_son_utran_strength
(
    U8  **pp_buffer,
    rrm_son_utran_strength_t *p_rrm_son_utran_strength
);

static
rrm_length_t
rrm_get_rrm_son_geran_strength_len
(
    rrm_son_geran_strength_t *p_rrm_son_geran_strength
);

static
rrm_return_et
rrm_compose_rrm_son_geran_strength
(
    U8  **pp_buffer,
    rrm_son_geran_strength_t *p_rrm_son_geran_strength
);

static
rrm_length_t
rrm_get_rrm_son_meas_strength_len
(
    rrm_son_meas_strength_t *p_rrm_son_meas_strength
);

static
rrm_return_et
rrm_compose_rrm_son_meas_strength
(
    U8  **pp_buffer,
    rrm_son_meas_strength_t *p_rrm_son_meas_strength
);

static
rrm_length_t
rrm_get_rrm_son_hw_load_ind_len
(
    rrm_son_hw_load_ind_t *p_rrm_son_hw_load_ind
);

static
rrm_return_et
rrm_compose_rrm_son_hw_load_ind
(
    U8  **pp_buffer,
    rrm_son_hw_load_ind_t *p_rrm_son_hw_load_ind
);

static
rrm_length_t
rrm_get_rrm_son_s1_tnl_load_ind_len
(
    rrm_son_s1_tnl_load_ind_t *p_rrm_son_s1_tnl_load_ind
);

static
rrm_return_et
rrm_compose_rrm_son_s1_tnl_load_ind
(
    U8  **pp_buffer,
    rrm_son_s1_tnl_load_ind_t *p_rrm_son_s1_tnl_load_ind
);

static
rrm_length_t
rrm_get_rrm_son_rrs_load_ind_len
(
    rrm_son_rrs_load_ind_t *p_rrm_son_rrs_load_ind
);

static
rrm_return_et
rrm_compose_rrm_son_rrs_load_ind
(
    U8  **pp_buffer,
    rrm_son_rrs_load_ind_t *p_rrm_son_rrs_load_ind
);

static
rrm_length_t
rrm_get_rrm_son_comp_avl_cap_len
(
    rrm_son_comp_avl_cap_t *p_rrm_son_comp_avl_cap
);

static
rrm_return_et
rrm_compose_rrm_son_comp_avl_cap
(
    U8  **pp_buffer,
    rrm_son_comp_avl_cap_t *p_rrm_son_comp_avl_cap
);

static
rrm_length_t
rrm_get_rrm_son_comp_avl_cap_grp_len
(
    rrm_son_comp_avl_cap_grp_t *p_rrm_son_comp_avl_cap_grp
);

static
rrm_return_et
rrm_compose_rrm_son_comp_avl_cap_grp
(
    U8  **pp_buffer,
    rrm_son_comp_avl_cap_grp_t *p_rrm_son_comp_avl_cap_grp
);

static
rrm_length_t
rrm_get_rrm_son_abs_status_len
(
    rrm_son_abs_status_t *p_rrm_son_abs_status
);

static
rrm_return_et
rrm_compose_rrm_son_abs_status
(
    U8  **pp_buffer,
    rrm_son_abs_status_t *p_rrm_son_abs_status
);

static
rrm_length_t
rrm_get_rrm_son_cell_load_info_len
(
    rrm_son_cell_load_info_t *p_rrm_son_cell_load_info
);

static
rrm_return_et
rrm_compose_rrm_son_cell_load_info
(
    U8  **pp_buffer,
    rrm_son_cell_load_info_t *p_rrm_son_cell_load_info
);

static
rrm_length_t
rrm_get_rrm_global_enb_id_len
(
    rrm_global_enb_id_t *p_rrm_global_enb_id
);

static
rrm_return_et
rrm_compose_rrm_global_enb_id
(
    U8  **pp_buffer,
    rrm_global_enb_id_t *p_rrm_global_enb_id
);

static
rrm_length_t
rrm_get_rrm_enb_tnl_address_len
(
    rrm_enb_tnl_address_t *p_rrm_enb_tnl_address
);

static
rrm_return_et
rrm_compose_rrm_enb_tnl_address
(
    U8  **pp_buffer,
    rrm_enb_tnl_address_t *p_rrm_enb_tnl_address
);

static
rrm_length_t
rrm_get_strong_cell_info_len
(
    strong_cell_info_t *p_strong_cell_info
);

static
rrm_return_et
rrm_compose_strong_cell_info
(
    U8  **pp_buffer,
    strong_cell_info_t *p_strong_cell_info
);

static
rrm_length_t
rrm_get_rrm_ue_prach_data_info_len
(
    rrm_ue_prach_data_info_t *p_rrm_ue_prach_data_info
);

static
rrm_return_et
rrm_compose_rrm_ue_prach_data_info
(
    U8  **pp_buffer,
    rrm_ue_prach_data_info_t *p_rrm_ue_prach_data_info
);

static
rrm_length_t
rrm_get_rrm_oam_gu_group_id_len
(
    rrm_oam_gu_group_id_t *p_rrm_oam_gu_group_id
);

static
rrm_return_et
rrm_compose_rrm_oam_gu_group_id
(
    U8  **pp_buffer,
    rrm_oam_gu_group_id_t *p_rrm_oam_gu_group_id
);

static
rrm_length_t
rrm_get_rrm_son_gu_group_id_list_len
(
    rrm_son_gu_group_id_list_t *p_rrm_son_gu_group_id_list
);

static
rrm_return_et
rrm_compose_rrm_son_gu_group_id_list
(
    U8  **pp_buffer,
    rrm_son_gu_group_id_list_t *p_rrm_son_gu_group_id_list
);

static
rrm_length_t
rrm_get_rrm_subframe_allocation_len
(
    rrm_subframe_allocation_t *p_rrm_subframe_allocation
);

static
rrm_return_et
rrm_compose_rrm_subframe_allocation
(
    U8  **pp_buffer,
    rrm_subframe_allocation_t *p_rrm_subframe_allocation
);

static
rrm_length_t
rrm_get_rrm_mbsfn_config_info_len
(
    rrm_mbsfn_config_info_t *p_rrm_mbsfn_config_info
);

static
rrm_return_et
rrm_compose_rrm_mbsfn_config_info
(
    U8  **pp_buffer,
    rrm_mbsfn_config_info_t *p_rrm_mbsfn_config_info
);

static
rrm_length_t
rrm_get_rrm_mbsfn_subframe_config_list_len
(
    rrm_mbsfn_subframe_config_list_t *p_rrm_mbsfn_subframe_config_list
);

static
rrm_return_et
rrm_compose_rrm_mbsfn_subframe_config_list
(
    U8  **pp_buffer,
    rrm_mbsfn_subframe_config_list_t *p_rrm_mbsfn_subframe_config_list
);

static
rrm_length_t
rrm_get_rrm_cell_sib2_mbsfn_data_len
(
    rrm_cell_sib2_mbsfn_data_t *p_rrm_cell_sib2_mbsfn_data
);

static
rrm_return_et
rrm_compose_rrm_cell_sib2_mbsfn_data
(
    U8  **pp_buffer,
    rrm_cell_sib2_mbsfn_data_t *p_rrm_cell_sib2_mbsfn_data
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrm_length_t
rrm_get_rrm_son_register_req_len
(
    rrm_son_register_req_t *p_rrm_son_register_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_register_req != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_register_deregister_format_len(&p_rrm_son_register_req->register_req);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_register_req
(
    U8  **pp_buffer,
    rrm_son_register_req_t *p_rrm_son_register_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_register_req != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_register_deregister_format(pp_buffer, &p_rrm_son_register_req->register_req))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_register_req
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_REGISTER_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_register_req
(
    rrm_son_register_req_t  *p_rrm_son_register_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_register_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_register_req_len(p_rrm_son_register_req);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_REGISTER_REQ", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_REGISTER_REQ, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_REGISTER_REQ, msg_length,cell_index);

    /* Fill RRM_SON_REGISTER_REQ message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_register_req(&p_son_msg, p_rrm_son_register_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_cell_plmn_info_len
(
    rrm_oam_cell_plmn_info_t *p_rrm_oam_cell_plmn_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_oam_cell_plmn_info != PNULL);

    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_oam_cell_plmn_info->mcc);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_cell_plmn_info->num_mnc_digit);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_oam_cell_plmn_info->num_mnc_digit * sizeof(p_rrm_oam_cell_plmn_info->mnc[0]));

    return length;
}

static
rrm_return_et
rrm_compose_rrm_oam_cell_plmn_info
(
    U8  **pp_buffer,
    rrm_oam_cell_plmn_info_t *p_rrm_oam_cell_plmn_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_oam_cell_plmn_info != PNULL);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_cell_plmn_info->mcc); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_oam_cell_plmn_info->mcc[loop], "mcc[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_cell_plmn_info->num_mnc_digit < 2) || (p_rrm_oam_cell_plmn_info->num_mnc_digit > 3))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_oam_cell_plmn_info->num_mnc_digit] should be in range [2 <= %d <= 3] !", p_rrm_oam_cell_plmn_info->num_mnc_digit);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_oam_cell_plmn_info->num_mnc_digit, "num_mnc_digit");
    *pp_buffer += sizeof(p_rrm_oam_cell_plmn_info->num_mnc_digit);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_cell_plmn_info->num_mnc_digit; loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_oam_cell_plmn_info->mnc[loop], "mnc[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_eutran_global_cell_id_len
(
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_oam_eutran_global_cell_id != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_cell_plmn_info_len(&p_rrm_oam_eutran_global_cell_id->primary_plmn_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_oam_eutran_global_cell_id->cell_identity);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_oam_eutran_global_cell_id
(
    U8  **pp_buffer,
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_oam_eutran_global_cell_id != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_cell_plmn_info(pp_buffer, &p_rrm_oam_eutran_global_cell_id->primary_plmn_id))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_eutran_global_cell_id->cell_identity); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_oam_eutran_global_cell_id->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_cell_object_id_len
(
    rrm_cell_object_id_t *p_rrm_cell_object_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_cell_object_id != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_cell_object_id->cell_id);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_cell_object_id
(
    U8  **pp_buffer,
    rrm_cell_object_id_t *p_rrm_cell_object_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_cell_object_id != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_cell_object_id->cell_id))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_object_id_len
(
    rrm_object_id_t *p_rrm_object_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_object_id != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_cell_object_id_len(&p_rrm_object_id->object_id);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_object_id
(
    U8  **pp_buffer,
    rrm_object_id_t *p_rrm_object_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_object_id != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_cell_object_id(pp_buffer, &p_rrm_object_id->object_id))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_trgr_periodic_value_len
(
    rrm_trgr_periodic_value_t *p_rrm_trgr_periodic_value
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_trgr_periodic_value != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_trgr_periodic_value->interval);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_trgr_periodic_value
(
    U8  **pp_buffer,
    rrm_trgr_periodic_value_t *p_rrm_trgr_periodic_value
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_trgr_periodic_value != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_trgr_periodic_value->interval, "interval");
    *pp_buffer += sizeof(p_rrm_trgr_periodic_value->interval);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_trgr_threshold_value_len
(
    rrm_trgr_threshold_value_t *p_rrm_trgr_threshold_value
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_trgr_threshold_value != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_trgr_threshold_value->count);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_trgr_threshold_value
(
    U8  **pp_buffer,
    rrm_trgr_threshold_value_t *p_rrm_trgr_threshold_value
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_trgr_threshold_value != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_trgr_threshold_value->count, "count");
    *pp_buffer += sizeof(p_rrm_trgr_threshold_value->count);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_trgr_event_value_len
(
    rrm_trgr_event_value_t *p_rrm_trgr_event_value
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_trgr_event_value != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_trgr_event_value->enable);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_trgr_event_value
(
    U8  **pp_buffer,
    rrm_trgr_event_value_t *p_rrm_trgr_event_value
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_trgr_event_value != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_trgr_event_value->enable, "enable");
    *pp_buffer += sizeof(p_rrm_trgr_event_value->enable);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_trigger_len
(
    rrm_trigger_t *p_rrm_trigger
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_trigger != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_trigger->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_trigger->id);

    /* Optional element */
    if(p_rrm_trigger->bitmask & RRM_TRIGGER_TYPE_PERIODIC)
    {

    /* Get length of IE */
    length += rrm_get_rrm_trgr_periodic_value_len(&p_rrm_trigger->periodic);
    }

    /* Optional element */
    if(p_rrm_trigger->bitmask & RRM_TRIGGER_TYPE_THRESHOLD)
    {

    /* Get length of IE */
    length += rrm_get_rrm_trgr_threshold_value_len(&p_rrm_trigger->threshold);
    }

    /* Optional element */
    if(p_rrm_trigger->bitmask & RRM_TRIGGER_TYPE_EVENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_trgr_event_value_len(&p_rrm_trigger->event);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_trigger
(
    U8  **pp_buffer,
    rrm_trigger_t *p_rrm_trigger
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_trigger != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_trigger->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_trigger->bitmask);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_trigger->id, "id");
    *pp_buffer += sizeof(p_rrm_trigger->id);

    /* Optional element */
    if(p_rrm_trigger->bitmask & RRM_TRIGGER_TYPE_PERIODIC)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_trgr_periodic_value(pp_buffer, &p_rrm_trigger->periodic))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_trigger->bitmask & RRM_TRIGGER_TYPE_THRESHOLD)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_trgr_threshold_value(pp_buffer, &p_rrm_trigger->threshold))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_trigger->bitmask & RRM_TRIGGER_TYPE_EVENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_trgr_event_value(pp_buffer, &p_rrm_trigger->event))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_attribute_len
(
    rrm_attribute_t *p_rrm_attribute
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_attribute != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_attribute->id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_attribute->trgr_list_size);

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_attribute->trgr_list_size; loop++)
        {
            length += rrm_get_rrm_trigger_len(&p_rrm_attribute->trgr_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_attribute->value);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_attribute
(
    U8  **pp_buffer,
    rrm_attribute_t *p_rrm_attribute
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_attribute != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_attribute->id, "id");
    *pp_buffer += sizeof(p_rrm_attribute->id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_attribute->trgr_list_size, "trgr_list_size");
    *pp_buffer += sizeof(p_rrm_attribute->trgr_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_attribute->trgr_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_trigger(pp_buffer, &p_rrm_attribute->trgr_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_attribute->value, "value");
    *pp_buffer += sizeof(p_rrm_attribute->value);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_object_len
(
    object_t *p_object
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_object != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_object->type);

    /* Get length of IE */
    length += rrm_get_rrm_object_id_len(&p_object->id);
    /* Get length of parameter of basic type */
    length += sizeof(p_object->attr_list_size);

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_object->attr_list_size; loop++)
        {
            length += rrm_get_rrm_attribute_len(&p_object->p_attr_lst[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_compose_object
(
    U8  **pp_buffer,
    object_t *p_object
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_object != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_object->type, "type");
    *pp_buffer += sizeof(p_object->type);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_object_id(pp_buffer, &p_object->id))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_object->attr_list_size, "attr_list_size");
    *pp_buffer += sizeof(p_object->attr_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_object->attr_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_attribute(pp_buffer, &p_object->p_attr_lst[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_register_deregister_format_len
(
    rrm_son_register_deregister_format_t *p_rrm_son_register_deregister_format
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_register_deregister_format != PNULL);


    /* Get length of IE */
    length += rrm_get_object_len(&p_rrm_son_register_deregister_format->object);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_register_deregister_format
(
    U8  **pp_buffer,
    rrm_son_register_deregister_format_t *p_rrm_son_register_deregister_format
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_register_deregister_format != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_object(pp_buffer, &p_rrm_son_register_deregister_format->object))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_deregister_req_len
(
    rrm_son_deregister_req_t *p_rrm_son_deregister_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_deregister_req != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_register_deregister_format_len(&p_rrm_son_deregister_req->deregister_req);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_deregister_req
(
    U8  **pp_buffer,
    rrm_son_deregister_req_t *p_rrm_son_deregister_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_deregister_req != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_register_deregister_format(pp_buffer, &p_rrm_son_deregister_req->deregister_req))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_deregister_req
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_DEREGISTER_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_deregister_req
(
    rrm_son_deregister_req_t  *p_rrm_son_deregister_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_deregister_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_deregister_req_len(p_rrm_son_deregister_req);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_DEREGISTER_REQ", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_DEREGISTER_REQ, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_DEREGISTER_REQ, msg_length,cell_index);

    /* Fill RRM_SON_DEREGISTER_REQ message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_deregister_req(&p_son_msg, p_rrm_son_deregister_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_set_attribute_value_req_len
(
    rrm_son_set_attribute_value_req_t *p_rrm_son_set_attribute_value_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_set_attribute_value_req != PNULL);


    /* Get length of IE */
    length += rrm_get_object_len(&p_rrm_son_set_attribute_value_req->object);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_set_attribute_value_req
(
    U8  **pp_buffer,
    rrm_son_set_attribute_value_req_t *p_rrm_son_set_attribute_value_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_set_attribute_value_req != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_object(pp_buffer, &p_rrm_son_set_attribute_value_req->object))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_set_attribute_value_req
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_SET_ATTRIBUTE_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_set_attribute_value_req
(
    rrm_son_set_attribute_value_req_t  *p_rrm_son_set_attribute_value_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_set_attribute_value_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_set_attribute_value_req_len(p_rrm_son_set_attribute_value_req);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_SET_ATTRIBUTE_REQ", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_SET_ATTRIBUTE_REQ, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_SET_ATTRIBUTE_REQ, msg_length,cell_index);

    /* Fill RRM_SON_SET_ATTRIBUTE_REQ message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_set_attribute_value_req(&p_son_msg, p_rrm_son_set_attribute_value_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_cell_switch_on_res_len
(
    rrm_son_cell_switch_on_res_t *p_rrm_son_cell_switch_on_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cell_switch_on_res != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_non_registration_based_res_len(&p_rrm_son_cell_switch_on_res->cell_switchon_res);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_cell_switch_on_res
(
    U8  **pp_buffer,
    rrm_son_cell_switch_on_res_t *p_rrm_son_cell_switch_on_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cell_switch_on_res != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_non_registration_based_res(pp_buffer, &p_rrm_son_cell_switch_on_res->cell_switchon_res))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_cell_switch_on_res
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_CELL_SWITCH_ON_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_cell_switch_on_res
(
    rrm_son_cell_switch_on_res_t  *p_rrm_son_cell_switch_on_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_cell_switch_on_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_cell_switch_on_res_len(p_rrm_son_cell_switch_on_res);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_CELL_SWITCH_ON_RESP", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_CELL_SWITCH_ON_RESP, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_CELL_SWITCH_ON_RESP, msg_length,cell_index);

    /* Fill RRM_SON_CELL_SWITCH_ON_RESP message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_cell_switch_on_res(&p_son_msg, p_rrm_son_cell_switch_on_res))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_non_registration_based_res_len
(
    rrm_son_non_registration_based_res_t *p_rrm_son_non_registration_based_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_non_registration_based_res != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_non_registration_based_res->cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_non_registration_based_res->result);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_non_registration_based_res->error_code);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_non_registration_based_res
(
    U8  **pp_buffer,
    rrm_son_non_registration_based_res_t *p_rrm_son_non_registration_based_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_non_registration_based_res != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_non_registration_based_res->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_non_registration_based_res->result, "result");
    *pp_buffer += sizeof(p_rrm_son_non_registration_based_res->result);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_non_registration_based_res->error_code, "error_code");
    *pp_buffer += sizeof(p_rrm_son_non_registration_based_res->error_code);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_cell_switch_off_res_len
(
    rrm_son_cell_switch_off_res_t *p_rrm_son_cell_switch_off_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cell_switch_off_res != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_non_registration_based_res_len(&p_rrm_son_cell_switch_off_res->cell_switchoff_res);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_cell_switch_off_res
(
    U8  **pp_buffer,
    rrm_son_cell_switch_off_res_t *p_rrm_son_cell_switch_off_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cell_switch_off_res != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_non_registration_based_res(pp_buffer, &p_rrm_son_cell_switch_off_res->cell_switchoff_res))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_cell_switch_off_res
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_CELL_SWITCH_OFF_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_cell_switch_off_res
(
    rrm_son_cell_switch_off_res_t  *p_rrm_son_cell_switch_off_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_cell_switch_off_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_cell_switch_off_res_len(p_rrm_son_cell_switch_off_res);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_CELL_SWITCH_OFF_RESP", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_CELL_SWITCH_OFF_RESP, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_CELL_SWITCH_OFF_RESP, msg_length,cell_index);

    /* Fill RRM_SON_CELL_SWITCH_OFF_RESP message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_cell_switch_off_res(&p_son_msg, p_rrm_son_cell_switch_off_res))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_registration_res_len
(
    rrm_son_registration_res_t *p_rrm_son_registration_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_registration_res != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_registration_based_res_len(&p_rrm_son_registration_res->registration_resp);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_registration_res
(
    U8  **pp_buffer,
    rrm_son_registration_res_t *p_rrm_son_registration_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_registration_res != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_registration_based_res(pp_buffer, &p_rrm_son_registration_res->registration_resp))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_registration_res
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_REGISTER_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_registration_res
(
    rrm_son_registration_res_t  *p_rrm_son_registration_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_registration_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_registration_res_len(p_rrm_son_registration_res);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_REGISTER_RESP", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_REGISTER_RESP, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_REGISTER_RESP, msg_length,cell_index);

    /* Fill RRM_SON_REGISTER_RESP message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_registration_res(&p_son_msg, p_rrm_son_registration_res))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_registration_based_res_len
(
    rrm_son_registration_based_res_t *p_rrm_son_registration_based_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_registration_based_res != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_object_id_len(&p_rrm_son_registration_based_res->id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_registration_based_res->result);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_registration_based_res->error_code);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_registration_based_res
(
    U8  **pp_buffer,
    rrm_son_registration_based_res_t *p_rrm_son_registration_based_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_registration_based_res != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_object_id(pp_buffer, &p_rrm_son_registration_based_res->id))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_registration_based_res->result, "result");
    *pp_buffer += sizeof(p_rrm_son_registration_based_res->result);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_registration_based_res->error_code, "error_code");
    *pp_buffer += sizeof(p_rrm_son_registration_based_res->error_code);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_deregistration_res_len
(
    rrm_son_deregistration_res_t *p_rrm_son_deregistration_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_deregistration_res != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_registration_based_res_len(&p_rrm_son_deregistration_res->deregistration_resp);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_deregistration_res
(
    U8  **pp_buffer,
    rrm_son_deregistration_res_t *p_rrm_son_deregistration_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_deregistration_res != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_registration_based_res(pp_buffer, &p_rrm_son_deregistration_res->deregistration_resp))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_deregistration_res
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_DEREGISTER_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_deregistration_res
(
    rrm_son_deregistration_res_t  *p_rrm_son_deregistration_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_deregistration_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_deregistration_res_len(p_rrm_son_deregistration_res);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_DEREGISTER_RESP", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_DEREGISTER_RESP, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_DEREGISTER_RESP, msg_length,cell_index);

    /* Fill RRM_SON_DEREGISTER_RESP message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_deregistration_res(&p_son_msg, p_rrm_son_deregistration_res))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_set_attr_res_len
(
    rrm_son_set_attr_res_t *p_rrm_son_set_attr_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_set_attr_res != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_registration_based_res_len(&p_rrm_son_set_attr_res->set_attr_resp);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_set_attr_res->id);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_set_attr_res
(
    U8  **pp_buffer,
    rrm_son_set_attr_res_t *p_rrm_son_set_attr_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_set_attr_res != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_registration_based_res(pp_buffer, &p_rrm_son_set_attr_res->set_attr_resp))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_set_attr_res->id, "id");
    *pp_buffer += sizeof(p_rrm_son_set_attr_res->id);

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_set_attr_res
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_SET_ATTRIBUTE_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_set_attr_res
(
    rrm_son_set_attr_res_t  *p_rrm_son_set_attr_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_set_attr_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_set_attr_res_len(p_rrm_son_set_attr_res);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_SET_ATTRIBUTE_RESP", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_SET_ATTRIBUTE_RESP, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_SET_ATTRIBUTE_RESP, msg_length,cell_index);

    /* Fill RRM_SON_SET_ATTRIBUTE_RESP message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_set_attr_res(&p_son_msg, p_rrm_son_set_attr_res))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_active_ue_cnt_report_len
(
    rrm_son_active_ue_cnt_report_t *p_rrm_son_active_ue_cnt_report
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_active_ue_cnt_report != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_active_ue_cnt_report->cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_active_ue_cnt_report->ue_cnt);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_active_ue_cnt_report
(
    U8  **pp_buffer,
    rrm_son_active_ue_cnt_report_t *p_rrm_son_active_ue_cnt_report
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_active_ue_cnt_report != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_active_ue_cnt_report->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_active_ue_cnt_report->ue_cnt, "ue_cnt");
    *pp_buffer += sizeof(p_rrm_son_active_ue_cnt_report->ue_cnt);

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_active_ue_cnt_report
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_NUM_ACTIVE_UE_REPORT message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_active_ue_cnt_report
(
    rrm_son_active_ue_cnt_report_t  *p_rrm_son_active_ue_cnt_report,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_active_ue_cnt_report != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_active_ue_cnt_report_len(p_rrm_son_active_ue_cnt_report);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_NUM_ACTIVE_UE_REPORT", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_NUM_ACTIVE_UE_REPORT, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_NUM_ACTIVE_UE_REPORT, msg_length,cell_index);

    /* Fill RRM_SON_NUM_ACTIVE_UE_REPORT message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_active_ue_cnt_report(&p_son_msg, p_rrm_son_active_ue_cnt_report))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_ue_count_threshold_hit_ind_len
(
    rrm_ue_count_threshold_hit_ind_t *p_rrm_ue_count_threshold_hit_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_ue_count_threshold_hit_ind != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_ue_count_threshold_hit_ind->cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_ue_count_threshold_hit_ind->cause);

    return length;
}

rrm_return_et
rrm_compose_rrm_ue_count_threshold_hit_ind
(
    U8  **pp_buffer,
    rrm_ue_count_threshold_hit_ind_t *p_rrm_ue_count_threshold_hit_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_ue_count_threshold_hit_ind != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_ue_count_threshold_hit_ind->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_ue_count_threshold_hit_ind->cause, "cause");
    *pp_buffer += sizeof(p_rrm_ue_count_threshold_hit_ind->cause);

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_ue_count_threshold_hit_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRM_UE_COUNT_THRESHOLD_REACHED_IND message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_ue_count_threshold_hit_ind
(
    rrm_ue_count_threshold_hit_ind_t  *p_rrm_ue_count_threshold_hit_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_ue_count_threshold_hit_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_ue_count_threshold_hit_ind_len(p_rrm_ue_count_threshold_hit_ind);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_UE_COUNT_THRESHOLD_REACHED_IND", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_UE_COUNT_THRESHOLD_REACHED_IND, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_UE_COUNT_THRESHOLD_REACHED_IND, msg_length,cell_index);

    /* Fill RRM_UE_COUNT_THRESHOLD_REACHED_IND message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_ue_count_threshold_hit_ind(&p_son_msg, p_rrm_ue_count_threshold_hit_ind))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_cell_switch_off_req_len
(
    rrm_son_cell_switch_off_req_t *p_rrm_son_cell_switch_off_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cell_switch_off_req != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_cell_switch_off_req->cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cell_switch_off_req->switchoff_type);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cell_switch_off_req->switchoff_time);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_cell_switch_off_req
(
    U8  **pp_buffer,
    rrm_son_cell_switch_off_req_t *p_rrm_son_cell_switch_off_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cell_switch_off_req != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_cell_switch_off_req->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cell_switch_off_req->switchoff_type, "switchoff_type");
    *pp_buffer += sizeof(p_rrm_son_cell_switch_off_req->switchoff_type);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cell_switch_off_req->switchoff_time, "switchoff_time");
    *pp_buffer += sizeof(p_rrm_son_cell_switch_off_req->switchoff_time);

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_cell_switch_off_req
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_CELL_SWITCH_OFF_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_cell_switch_off_req
(
    rrm_son_cell_switch_off_req_t  *p_rrm_son_cell_switch_off_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_cell_switch_off_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_cell_switch_off_req_len(p_rrm_son_cell_switch_off_req);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_CELL_SWITCH_OFF_REQ", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_CELL_SWITCH_OFF_REQ, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_CELL_SWITCH_OFF_REQ, msg_length,cell_index);

    /* Fill RRM_SON_CELL_SWITCH_OFF_REQ message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_cell_switch_off_req(&p_son_msg, p_rrm_son_cell_switch_off_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_cell_switch_on_req_len
(
    rrm_son_cell_switch_on_req_t *p_rrm_son_cell_switch_on_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cell_switch_on_req != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_cell_switch_on_req->cell_id);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_cell_switch_on_req
(
    U8  **pp_buffer,
    rrm_son_cell_switch_on_req_t *p_rrm_son_cell_switch_on_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cell_switch_on_req != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_cell_switch_on_req->cell_id))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_cell_switch_on_req
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_CELL_SWITCH_ON_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_cell_switch_on_req
(
    rrm_son_cell_switch_on_req_t  *p_rrm_son_cell_switch_on_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_cell_switch_on_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_cell_switch_on_req_len(p_rrm_son_cell_switch_on_req);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_CELL_SWITCH_ON_REQ", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_CELL_SWITCH_ON_REQ, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_CELL_SWITCH_ON_REQ, msg_length,cell_index);

    /* Fill RRM_SON_CELL_SWITCH_ON_REQ message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_cell_switch_on_req(&p_son_msg, p_rrm_son_cell_switch_on_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_cell_state_change_ind_len
(
    rrm_son_cell_state_change_ind_t *p_rrm_son_cell_state_change_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cell_state_change_ind != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_cell_state_change_ind->cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cell_state_change_ind->cell_state);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cell_state_change_ind->cell_state_change_cause);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_cell_state_change_ind
(
    U8  **pp_buffer,
    rrm_son_cell_state_change_ind_t *p_rrm_son_cell_state_change_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cell_state_change_ind != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_cell_state_change_ind->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cell_state_change_ind->cell_state, "cell_state");
    *pp_buffer += sizeof(p_rrm_son_cell_state_change_ind->cell_state);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cell_state_change_ind->cell_state_change_cause, "cell_state_change_cause");
    *pp_buffer += sizeof(p_rrm_son_cell_state_change_ind->cell_state_change_cause);

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_cell_state_change_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_CELL_STATE_CHANGE_IND message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_cell_state_change_ind
(
    rrm_son_cell_state_change_ind_t  *p_rrm_son_cell_state_change_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_cell_state_change_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_cell_state_change_ind_len(p_rrm_son_cell_state_change_ind);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_CELL_STATE_CHANGE_IND", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_CELL_STATE_CHANGE_IND, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_CELL_STATE_CHANGE_IND, msg_length,cell_index);

    /* Fill RRM_SON_CELL_STATE_CHANGE_IND message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_cell_state_change_ind(&p_son_msg, p_rrm_son_cell_state_change_ind))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_nmm_prepare_res_len
(
    rrm_son_nmm_prepare_res_t *p_rrm_son_nmm_prepare_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_nmm_prepare_res != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_nmm_res_len(&p_rrm_son_nmm_prepare_res->prepare_res);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_nmm_prepare_res
(
    U8  **pp_buffer,
    rrm_son_nmm_prepare_res_t *p_rrm_son_nmm_prepare_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_nmm_prepare_res != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_nmm_res(pp_buffer, &p_rrm_son_nmm_prepare_res->prepare_res))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_nmm_prepare_res
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_NMM_PREPARE_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_nmm_prepare_res
(
    rrm_son_nmm_prepare_res_t  *p_rrm_son_nmm_prepare_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_nmm_prepare_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_nmm_prepare_res_len(p_rrm_son_nmm_prepare_res);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_NMM_PREPARE_RESP", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_NMM_PREPARE_RESP, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_NMM_PREPARE_RESP, msg_length,cell_index);

    /* Fill RRM_SON_NMM_PREPARE_RESP message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_nmm_prepare_res(&p_son_msg, p_rrm_son_nmm_prepare_res))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_nmm_res_len
(
    rrm_son_nmm_res_t *p_rrm_son_nmm_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_nmm_res != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_nmm_res->result);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_nmm_res->error_code);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_nmm_res
(
    U8  **pp_buffer,
    rrm_son_nmm_res_t *p_rrm_son_nmm_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_nmm_res != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_nmm_res->result, "result");
    *pp_buffer += sizeof(p_rrm_son_nmm_res->result);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_nmm_res->error_code, "error_code");
    *pp_buffer += sizeof(p_rrm_son_nmm_res->error_code);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_nmm_complete_res_len
(
    rrm_son_nmm_complete_res_t *p_rrm_son_nmm_complete_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_nmm_complete_res != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_nmm_res_len(&p_rrm_son_nmm_complete_res->complete_res);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_nmm_complete_res
(
    U8  **pp_buffer,
    rrm_son_nmm_complete_res_t *p_rrm_son_nmm_complete_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_nmm_complete_res != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_nmm_res(pp_buffer, &p_rrm_son_nmm_complete_res->complete_res))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_nmm_complete_res
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_NMM_COMPLETE_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_nmm_complete_res
(
    rrm_son_nmm_complete_res_t  *p_rrm_son_nmm_complete_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_nmm_complete_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_nmm_complete_res_len(p_rrm_son_nmm_complete_res);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_NMM_COMPLETE_RESP", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_NMM_COMPLETE_RESP, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_NMM_COMPLETE_RESP, msg_length,cell_index);

    /* Fill RRM_SON_NMM_COMPLETE_RESP message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_nmm_complete_res(&p_son_msg, p_rrm_son_nmm_complete_res))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_meas_config_req_len
(
    rrm_son_meas_config_req_t *p_rrm_son_meas_config_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_config_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_req->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_meas_config_req->cell_id);

    /* Optional element */
    if(p_rrm_son_meas_config_req->bitmask & RRM_SON_UE_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_req->ue_index);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_req->eutran_config_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_req->eutran_config_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_req->eutran_config_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_req->eutran_config_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_req->eutran_config_list_size; loop++)
        {
            length += rrm_get_rrm_son_meas_config_eutran_len(&p_rrm_son_meas_config_req->eutran_config_list[loop]);
        }
    }

    /* Get length of IE */
    length += rrm_get_rrm_son_inter_rat_meas_config_len(&p_rrm_son_meas_config_req->inter_rat_meas_config);

    /* Optional element */
    if(p_rrm_son_meas_config_req->bitmask & RRM_SON_MEAS_GAP_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_meas_gap_config_len(&p_rrm_son_meas_config_req->meas_gap_config);
    }

    return length;
}

rrm_return_et
rrm_compose_rrm_son_meas_config_req
(
    U8  **pp_buffer,
    rrm_son_meas_config_req_t *p_rrm_son_meas_config_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_config_req != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_config_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_meas_config_req->bitmask);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_meas_config_req->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_son_meas_config_req->bitmask & RRM_SON_UE_INDEX_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_meas_config_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrm_son_meas_config_req->ue_index);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_req->eutran_config_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_req->eutran_config_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_req->eutran_config_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_req->eutran_config_list_size, "eutran_config_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_req->eutran_config_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_req->eutran_config_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_meas_config_eutran(pp_buffer, &p_rrm_son_meas_config_req->eutran_config_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_inter_rat_meas_config(pp_buffer, &p_rrm_son_meas_config_req->inter_rat_meas_config))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_son_meas_config_req->bitmask & RRM_SON_MEAS_GAP_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_meas_gap_config(pp_buffer, &p_rrm_son_meas_config_req->meas_gap_config))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_meas_config_req
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_MEAS_CONFIG_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_meas_config_req
(
    rrm_son_meas_config_req_t  *p_rrm_son_meas_config_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_meas_config_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_meas_config_req_len(p_rrm_son_meas_config_req);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_MEAS_CONFIG_REQ", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_MEAS_CONFIG_REQ, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_MEAS_CONFIG_REQ, msg_length,cell_index);

    /* Fill RRM_SON_MEAS_CONFIG_REQ message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_meas_config_req(&p_son_msg, p_rrm_son_meas_config_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_speed_state_scale_factors_len
(
    rrm_son_speed_state_scale_factors_t *p_rrm_son_speed_state_scale_factors
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_speed_state_scale_factors != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_speed_state_scale_factors->sf_medium);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_speed_state_scale_factors->sf_high);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_speed_state_scale_factors
(
    U8  **pp_buffer,
    rrm_son_speed_state_scale_factors_t *p_rrm_son_speed_state_scale_factors
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_speed_state_scale_factors != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_speed_state_scale_factors->sf_medium > 3))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_speed_state_scale_factors->sf_medium] should be in range [%d <= 3] !", p_rrm_son_speed_state_scale_factors->sf_medium);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_speed_state_scale_factors->sf_medium, "sf_medium");
    *pp_buffer += sizeof(p_rrm_son_speed_state_scale_factors->sf_medium);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_speed_state_scale_factors->sf_high > 3))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_speed_state_scale_factors->sf_high] should be in range [%d <= 3] !", p_rrm_son_speed_state_scale_factors->sf_high);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_speed_state_scale_factors->sf_high, "sf_high");
    *pp_buffer += sizeof(p_rrm_son_speed_state_scale_factors->sf_high);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_meas_config_eutran_len
(
    rrm_son_meas_config_eutran_t *p_rrm_son_meas_config_eutran
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_config_eutran != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->earfcn);

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_MEAS_BANDWIDTH_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->meas_bandwidth);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_PRESENCE_ANTENNAE_PORT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->presence_antenna_port);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_NEIGHBOR_CELL_CONFIG_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->neighbor_cell_config);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_OFFSET_FREQUENCY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->offset_frequency);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_Q_RX_LEV_MIN_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->q_rx_lev_min);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_P_MAX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->p_max);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_RESELECTION_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->t_reselection);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_RESELECTION_SF_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_speed_state_scale_factors_len(&p_rrm_son_meas_config_eutran->t_reselection_sf);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_THRESHOLD_HIGH_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->threshX_high);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_THRESHOLD_LOW_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->threshX_low);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_CELL_RESELECTION_PRIORITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->cell_reselection_priority);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_CELLS_TO_REPORT_CGI_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_CELLS_TO_REPORT_CGI_PRESENT)
    {
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size * sizeof(p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi[0]));
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_Q_QUAL_MIN_R9)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->q_qualmin_r9);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_THRESHX_HIGH_Q_R9)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->threshx_highq_r9);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_THRESHX_LOW_Q_R9)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->threshx_lowq_r9);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_CM_OFFSET_FREQUENCY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_eutran->cm_offset_frequency);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_meas_config_eutran
(
    U8  **pp_buffer,
    rrm_son_meas_config_eutran_t *p_rrm_son_meas_config_eutran
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_config_eutran != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_config_eutran->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->bitmask);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_config_eutran->earfcn, "earfcn");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->earfcn);

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_MEAS_BANDWIDTH_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->meas_bandwidth > 5))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->meas_bandwidth] should be in range [%d <= 5] !", p_rrm_son_meas_config_eutran->meas_bandwidth);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_config_eutran->meas_bandwidth, "meas_bandwidth");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->meas_bandwidth);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_PRESENCE_ANTENNAE_PORT_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->presence_antenna_port > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->presence_antenna_port] should be in range [%d <= 1] !", p_rrm_son_meas_config_eutran->presence_antenna_port);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_eutran->presence_antenna_port, "presence_antenna_port");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->presence_antenna_port);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_NEIGHBOR_CELL_CONFIG_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_eutran->neighbor_cell_config, "neighbor_cell_config");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->neighbor_cell_config);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_OFFSET_FREQUENCY_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->offset_frequency > 30))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->offset_frequency] should be in range [%d <= 30] !", p_rrm_son_meas_config_eutran->offset_frequency);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_config_eutran->offset_frequency, "offset_frequency");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->offset_frequency);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_Q_RX_LEV_MIN_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_eutran->q_rx_lev_min < -70) || (p_rrm_son_meas_config_eutran->q_rx_lev_min > -22))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->q_rx_lev_min] should be in range [-70 <= %d <= -22] !", p_rrm_son_meas_config_eutran->q_rx_lev_min);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_meas_config_eutran->q_rx_lev_min, "q_rx_lev_min");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->q_rx_lev_min);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_P_MAX_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_eutran->p_max < -30) || (p_rrm_son_meas_config_eutran->p_max > 33))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->p_max] should be in range [-30 <= %d <= 33] !", p_rrm_son_meas_config_eutran->p_max);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_meas_config_eutran->p_max, "p_max");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->p_max);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_RESELECTION_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->t_reselection > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->t_reselection] should be in range [%d <= 7] !", p_rrm_son_meas_config_eutran->t_reselection);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_eutran->t_reselection, "t_reselection");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->t_reselection);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_RESELECTION_SF_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_speed_state_scale_factors(pp_buffer, &p_rrm_son_meas_config_eutran->t_reselection_sf))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_THRESHOLD_HIGH_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->threshX_high > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->threshX_high] should be in range [%d <= 31] !", p_rrm_son_meas_config_eutran->threshX_high);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_eutran->threshX_high, "threshX_high");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->threshX_high);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_THRESHOLD_LOW_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->threshX_low > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->threshX_low] should be in range [%d <= 31] !", p_rrm_son_meas_config_eutran->threshX_low);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_eutran->threshX_low, "threshX_low");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->threshX_low);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->cell_reselection_priority > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->cell_reselection_priority] should be in range [%d <= 7] !", p_rrm_son_meas_config_eutran->cell_reselection_priority);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_eutran->cell_reselection_priority, "cell_reselection_priority");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->cell_reselection_priority);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_CELLS_TO_REPORT_CGI_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size < 1) || (p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size] should be in range [1 <= %d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size, "cells_for_which_to_report_cgi_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_CELLS_TO_REPORT_CGI_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi_size; loop++)
        {
            rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_meas_config_eutran->cells_for_which_to_report_cgi[loop], "cells_for_which_to_report_cgi[]");
            *pp_buffer += sizeof(U16);
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_Q_QUAL_MIN_R9)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_eutran->q_qualmin_r9 < -34) || (p_rrm_son_meas_config_eutran->q_qualmin_r9 > -3))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->q_qualmin_r9] should be in range [-34 <= %d <= -3] !", p_rrm_son_meas_config_eutran->q_qualmin_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_meas_config_eutran->q_qualmin_r9, "q_qualmin_r9");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->q_qualmin_r9);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_THRESHX_HIGH_Q_R9)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->threshx_highq_r9 > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->threshx_highq_r9] should be in range [%d <= 31] !", p_rrm_son_meas_config_eutran->threshx_highq_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_eutran->threshx_highq_r9, "threshx_highq_r9");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->threshx_highq_r9);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_THRESHX_LOW_Q_R9)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->threshx_lowq_r9 > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->threshx_lowq_r9] should be in range [%d <= 31] !", p_rrm_son_meas_config_eutran->threshx_lowq_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_eutran->threshx_lowq_r9, "threshx_lowq_r9");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->threshx_lowq_r9);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_eutran->bitmask & RRM_SON_EUTRAN_CM_OFFSET_FREQUENCY_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_eutran->cm_offset_frequency > 30))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_eutran->cm_offset_frequency] should be in range [%d <= 30] !", p_rrm_son_meas_config_eutran->cm_offset_frequency);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_config_eutran->cm_offset_frequency, "cm_offset_frequency");
    *pp_buffer += sizeof(p_rrm_son_meas_config_eutran->cm_offset_frequency);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_utran_pci_fdd_range_len
(
    rrm_son_utran_pci_fdd_range_t *p_rrm_son_utran_pci_fdd_range
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_utran_pci_fdd_range != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_pci_fdd_range->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_pci_fdd_range->start);

    /* Optional element */
    if(p_rrm_son_utran_pci_fdd_range->bitmask & RRM_SON_UTRAN_PCI_FDD_RANGE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_pci_fdd_range->range);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_utran_pci_fdd_range
(
    U8  **pp_buffer,
    rrm_son_utran_pci_fdd_range_t *p_rrm_son_utran_pci_fdd_range
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_utran_pci_fdd_range != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_utran_pci_fdd_range->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_utran_pci_fdd_range->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_pci_fdd_range->start > 511))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_pci_fdd_range->start] should be in range [%d <= 511] !", p_rrm_son_utran_pci_fdd_range->start);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_utran_pci_fdd_range->start, "start");
    *pp_buffer += sizeof(p_rrm_son_utran_pci_fdd_range->start);

    /* Optional element */
    if(p_rrm_son_utran_pci_fdd_range->bitmask & RRM_SON_UTRAN_PCI_FDD_RANGE_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_utran_pci_fdd_range->range < 2) || (p_rrm_son_utran_pci_fdd_range->range > 512))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_pci_fdd_range->range] should be in range [2 <= %d <= 512] !", p_rrm_son_utran_pci_fdd_range->range);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_utran_pci_fdd_range->range, "range");
    *pp_buffer += sizeof(p_rrm_son_utran_pci_fdd_range->range);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_meas_config_utran_fdd_len
(
    rrm_son_meas_config_utran_fdd_t *p_rrm_son_meas_config_utran_fdd
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_config_utran_fdd != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->uarfcn);

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_OFFSET_FREQUENCY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->offset_frequency);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_CELL_RESELECTION_PRIORITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->cell_reselection_priority);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_THRESHOLD_HIGH_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->threshX_high);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_THRESHOLD_LOW_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->threshX_low);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_Q_RX_LEV_MIN_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->q_rx_lev_min);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_P_MAX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->p_max);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_Q_QUAL_MIN_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->q_qual_min);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size < 1) || (p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size > MAX_PCI_RANGE))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size] should be in range [1 <= %d <= MAX_PCI_RANGE] !", p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size; loop++)
        {
            length += rrm_get_rrm_son_utran_pci_fdd_range_len(&p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list[loop]);
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_CELLS_TO_REPORT_CGI_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_CELLS_TO_REPORT_CGI_PRESENT)
    {
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size * sizeof(p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi[0]));
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_THRESHX_HIGH_Q_R9)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->threshx_highq_r9);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_THRESHX_LOW_Q_R9)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_fdd->threshx_lowq_r9);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_meas_config_utran_fdd
(
    U8  **pp_buffer,
    rrm_son_meas_config_utran_fdd_t *p_rrm_son_meas_config_utran_fdd
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_config_utran_fdd != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_fdd->uarfcn > 16383))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->uarfcn] should be in range [%d <= 16383] !", p_rrm_son_meas_config_utran_fdd->uarfcn);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->uarfcn, "uarfcn");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->uarfcn);

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_OFFSET_FREQUENCY_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->offset_frequency < -15) || (p_rrm_son_meas_config_utran_fdd->offset_frequency > 15))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->offset_frequency] should be in range [-15 <= %d <= 15] !", p_rrm_son_meas_config_utran_fdd->offset_frequency);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->offset_frequency, "offset_frequency");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->offset_frequency);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_fdd->cell_reselection_priority > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->cell_reselection_priority] should be in range [%d <= 7] !", p_rrm_son_meas_config_utran_fdd->cell_reselection_priority);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->cell_reselection_priority, "cell_reselection_priority");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->cell_reselection_priority);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_THRESHOLD_HIGH_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_fdd->threshX_high > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->threshX_high] should be in range [%d <= 31] !", p_rrm_son_meas_config_utran_fdd->threshX_high);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->threshX_high, "threshX_high");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->threshX_high);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_THRESHOLD_LOW_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_fdd->threshX_low > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->threshX_low] should be in range [%d <= 31] !", p_rrm_son_meas_config_utran_fdd->threshX_low);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->threshX_low, "threshX_low");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->threshX_low);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_Q_RX_LEV_MIN_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->q_rx_lev_min < -60) || (p_rrm_son_meas_config_utran_fdd->q_rx_lev_min > -13))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->q_rx_lev_min] should be in range [-60 <= %d <= -13] !", p_rrm_son_meas_config_utran_fdd->q_rx_lev_min);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->q_rx_lev_min, "q_rx_lev_min");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->q_rx_lev_min);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_P_MAX_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->p_max < -50) || (p_rrm_son_meas_config_utran_fdd->p_max > 33))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->p_max] should be in range [-50 <= %d <= 33] !", p_rrm_son_meas_config_utran_fdd->p_max);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->p_max, "p_max");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->p_max);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_Q_QUAL_MIN_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->q_qual_min < -24) || (p_rrm_son_meas_config_utran_fdd->q_qual_min > 0))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->q_qual_min] should be in range [-24 <= %d <= 0] !", p_rrm_son_meas_config_utran_fdd->q_qual_min);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->q_qual_min, "q_qual_min");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->q_qual_min);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size < 1) || (p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size > MAX_PCI_RANGE))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size] should be in range [1 <= %d <= MAX_PCI_RANGE] !", p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size, "csg_allowed_reporting_cell_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_utran_pci_fdd_range(pp_buffer, &p_rrm_son_meas_config_utran_fdd->csg_allowed_reporting_cell_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_CELLS_TO_REPORT_CGI_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size < 1) || (p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size] should be in range [1 <= %d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size, "cells_for_which_to_report_cgi_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_CELLS_TO_REPORT_CGI_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi_size; loop++)
        {
            rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->cells_for_which_to_report_cgi[loop], "cells_for_which_to_report_cgi[]");
            *pp_buffer += sizeof(U16);
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_THRESHX_HIGH_Q_R9)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_fdd->threshx_highq_r9 > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->threshx_highq_r9] should be in range [%d <= 31] !", p_rrm_son_meas_config_utran_fdd->threshx_highq_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->threshx_highq_r9, "threshx_highq_r9");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->threshx_highq_r9);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_fdd->bitmask & RRM_SON_UTRAN_THRESHX_LOW_Q_R9)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_fdd->threshx_lowq_r9 > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_fdd->threshx_lowq_r9] should be in range [%d <= 31] !", p_rrm_son_meas_config_utran_fdd->threshx_lowq_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran_fdd->threshx_lowq_r9, "threshx_lowq_r9");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_fdd->threshx_lowq_r9);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_meas_config_utran_tdd_len
(
    rrm_son_meas_config_utran_tdd_t *p_rrm_son_meas_config_utran_tdd
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_config_utran_tdd != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_tdd->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_tdd->uarfcn);

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_OFFSET_FREQUENCY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_tdd->offset_frequency);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_CELL_RESELECTION_PRIORITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_tdd->cell_reselection_priority);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_THRESHOLD_HIGH_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_tdd->threshX_high);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_THRESHOLD_LOW_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_tdd->threshX_low);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_Q_RX_LEV_MIN_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_tdd->q_rx_lev_min);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_P_MAX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_tdd->p_max);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_CELLS_TO_REPORT_CGI_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_CELLS_TO_REPORT_CGI_PRESENT)
    {
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size * sizeof(p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi[0]));
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_meas_config_utran_tdd
(
    U8  **pp_buffer,
    rrm_son_meas_config_utran_tdd_t *p_rrm_son_meas_config_utran_tdd
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_config_utran_tdd != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_config_utran_tdd->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_tdd->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_tdd->uarfcn > 16383))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->uarfcn] should be in range [%d <= 16383] !", p_rrm_son_meas_config_utran_tdd->uarfcn);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_meas_config_utran_tdd->uarfcn, "uarfcn");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_tdd->uarfcn);

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_OFFSET_FREQUENCY_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_tdd->offset_frequency < -15) || (p_rrm_son_meas_config_utran_tdd->offset_frequency > 15))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->offset_frequency] should be in range [-15 <= %d <= 15] !", p_rrm_son_meas_config_utran_tdd->offset_frequency);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_meas_config_utran_tdd->offset_frequency, "offset_frequency");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_tdd->offset_frequency);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_tdd->cell_reselection_priority > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->cell_reselection_priority] should be in range [%d <= 7] !", p_rrm_son_meas_config_utran_tdd->cell_reselection_priority);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran_tdd->cell_reselection_priority, "cell_reselection_priority");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_tdd->cell_reselection_priority);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_THRESHOLD_HIGH_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_tdd->threshX_high > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->threshX_high] should be in range [%d <= 31] !", p_rrm_son_meas_config_utran_tdd->threshX_high);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran_tdd->threshX_high, "threshX_high");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_tdd->threshX_high);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_THRESHOLD_LOW_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran_tdd->threshX_low > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->threshX_low] should be in range [%d <= 31] !", p_rrm_son_meas_config_utran_tdd->threshX_low);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran_tdd->threshX_low, "threshX_low");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_tdd->threshX_low);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_Q_RX_LEV_MIN_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_tdd->q_rx_lev_min < -60) || (p_rrm_son_meas_config_utran_tdd->q_rx_lev_min > -13))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->q_rx_lev_min] should be in range [-60 <= %d <= -13] !", p_rrm_son_meas_config_utran_tdd->q_rx_lev_min);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_meas_config_utran_tdd->q_rx_lev_min, "q_rx_lev_min");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_tdd->q_rx_lev_min);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_P_MAX_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_tdd->p_max < -50) || (p_rrm_son_meas_config_utran_tdd->p_max > 33))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->p_max] should be in range [-50 <= %d <= 33] !", p_rrm_son_meas_config_utran_tdd->p_max);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_meas_config_utran_tdd->p_max, "p_max");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_tdd->p_max);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_CELLS_TO_REPORT_CGI_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size < 1) || (p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size] should be in range [1 <= %d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size, "cells_for_which_to_report_cgi_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran_tdd->bitmask & RRM_SON_UTRAN_TDD_CELLS_TO_REPORT_CGI_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi_size; loop++)
        {
            rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_meas_config_utran_tdd->cells_for_which_to_report_cgi[loop], "cells_for_which_to_report_cgi[]");
            *pp_buffer += sizeof(U16);
        }
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_meas_config_utran_len
(
    rrm_son_meas_config_utran_t *p_rrm_son_meas_config_utran
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_config_utran != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran->utran_fdd_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran->utran_fdd_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran->utran_fdd_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_utran->utran_fdd_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran->utran_fdd_list_size; loop++)
        {
            length += rrm_get_rrm_son_meas_config_utran_fdd_len(&p_rrm_son_meas_config_utran->utran_fdd_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran->utran_tdd_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran->utran_tdd_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran->utran_tdd_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_utran->utran_tdd_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran->utran_tdd_list_size; loop++)
        {
            length += rrm_get_rrm_son_meas_config_utran_tdd_len(&p_rrm_son_meas_config_utran->utran_tdd_list[loop]);
        }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran->bitmask & RRM_SON_UTRAN_RESELECTION_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_utran->t_reselection);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran->bitmask & RRM_SON_UTRAN_RESELECTION_SF_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_speed_state_scale_factors_len(&p_rrm_son_meas_config_utran->t_reselection_sf);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_meas_config_utran
(
    U8  **pp_buffer,
    rrm_son_meas_config_utran_t *p_rrm_son_meas_config_utran
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_config_utran != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_config_utran->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran->utran_fdd_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran->utran_fdd_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_utran->utran_fdd_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran->utran_fdd_list_size, "utran_fdd_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran->utran_fdd_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran->utran_fdd_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_meas_config_utran_fdd(pp_buffer, &p_rrm_son_meas_config_utran->utran_fdd_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran->utran_tdd_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran->utran_tdd_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_utran->utran_tdd_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran->utran_tdd_list_size, "utran_tdd_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran->utran_tdd_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_utran->utran_tdd_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_meas_config_utran_tdd(pp_buffer, &p_rrm_son_meas_config_utran->utran_tdd_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran->bitmask & RRM_SON_UTRAN_RESELECTION_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_utran->t_reselection > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_utran->t_reselection] should be in range [%d <= 7] !", p_rrm_son_meas_config_utran->t_reselection);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_utran->t_reselection, "t_reselection");
    *pp_buffer += sizeof(p_rrm_son_meas_config_utran->t_reselection);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_utran->bitmask & RRM_SON_UTRAN_RESELECTION_SF_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_speed_state_scale_factors(pp_buffer, &p_rrm_son_meas_config_utran->t_reselection_sf))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_geran_following_arfcns_len
(
    rrm_son_geran_following_arfcns_t *p_rrm_son_geran_following_arfcns
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_geran_following_arfcns != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_following_arfcns->bitmask);

    /* Optional element */
    if(p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_EXP_ARFCN_SET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_following_arfcns->exp_arfcn_list_size);
    }

    /* Optional element */
    if(p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_EXP_ARFCN_SET_PRESENT)
    {
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_son_geran_following_arfcns->exp_arfcn_list_size * sizeof(p_rrm_son_geran_following_arfcns->exp_arfcn_list[0]));
    }

    /* Optional element */
    if(p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_following_arfcns->arfcn_spacing);
    }

    /* Optional element */
    if(p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_following_arfcns->num_of_following_arfcn);
    }

    /* Optional element */
    if(p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size);
    }

    /* Optional element */
    if(p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT)
    {
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size * sizeof(p_rrm_son_geran_following_arfcns->arfcn_bmp_list[0]));
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_geran_following_arfcns
(
    U8  **pp_buffer,
    rrm_son_geran_following_arfcns_t *p_rrm_son_geran_following_arfcns
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_geran_following_arfcns != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_geran_following_arfcns->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_geran_following_arfcns->bitmask);

    /* Optional element */
    if(p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_EXP_ARFCN_SET_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_following_arfcns->exp_arfcn_list_size < 1) || (p_rrm_son_geran_following_arfcns->exp_arfcn_list_size > RRM_SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_following_arfcns->exp_arfcn_list_size] should be in range [1 <= %d <= RRM_SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE] !", p_rrm_son_geran_following_arfcns->exp_arfcn_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_following_arfcns->exp_arfcn_list_size, "exp_arfcn_list_size");
    *pp_buffer += sizeof(p_rrm_son_geran_following_arfcns->exp_arfcn_list_size);
    }

    /* Optional element */
    if(p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_EXP_ARFCN_SET_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_geran_following_arfcns->exp_arfcn_list_size; loop++)
        {
            rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_geran_following_arfcns->exp_arfcn_list[loop], "exp_arfcn_list[]");
            *pp_buffer += sizeof(U16);
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_following_arfcns->arfcn_spacing < 1) || (p_rrm_son_geran_following_arfcns->arfcn_spacing > 8))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_following_arfcns->arfcn_spacing] should be in range [1 <= %d <= 8] !", p_rrm_son_geran_following_arfcns->arfcn_spacing);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_following_arfcns->arfcn_spacing, "arfcn_spacing");
    *pp_buffer += sizeof(p_rrm_son_geran_following_arfcns->arfcn_spacing);
    }

    /* Optional element */
    if(p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_following_arfcns->num_of_following_arfcn > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_following_arfcns->num_of_following_arfcn] should be in range [%d <= 31] !", p_rrm_son_geran_following_arfcns->num_of_following_arfcn);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_following_arfcns->num_of_following_arfcn, "num_of_following_arfcn");
    *pp_buffer += sizeof(p_rrm_son_geran_following_arfcns->num_of_following_arfcn);
    }

    /* Optional element */
    if(p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size < 1) || (p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size > RRM_SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size] should be in range [1 <= %d <= RRM_SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP] !", p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size, "arfcn_bmp_list_size");
    *pp_buffer += sizeof(p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size);
    }

    /* Optional element */
    if(p_rrm_son_geran_following_arfcns->bitmask & RRM_SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT)
    {

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_geran_following_arfcns->arfcn_bmp_list_size; loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_following_arfcns->arfcn_bmp_list[loop], "arfcn_bmp_list[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_geran_carrier_freqs_len
(
    rrm_son_geran_carrier_freqs_t *p_rrm_son_geran_carrier_freqs
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_geran_carrier_freqs != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_carrier_freqs->starting_arfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_carrier_freqs->band_ind);

    /* Get length of IE */
    length += rrm_get_rrm_son_geran_following_arfcns_len(&p_rrm_son_geran_carrier_freqs->following_arfcns);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_geran_carrier_freqs
(
    U8  **pp_buffer,
    rrm_son_geran_carrier_freqs_t *p_rrm_son_geran_carrier_freqs
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_geran_carrier_freqs != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_carrier_freqs->starting_arfcn > 1023))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_carrier_freqs->starting_arfcn] should be in range [%d <= 1023] !", p_rrm_son_geran_carrier_freqs->starting_arfcn);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_geran_carrier_freqs->starting_arfcn, "starting_arfcn");
    *pp_buffer += sizeof(p_rrm_son_geran_carrier_freqs->starting_arfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_carrier_freqs->band_ind > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_carrier_freqs->band_ind] should be in range [%d <= 1] !", p_rrm_son_geran_carrier_freqs->band_ind);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_geran_carrier_freqs->band_ind, "band_ind");
    *pp_buffer += sizeof(p_rrm_son_geran_carrier_freqs->band_ind);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_geran_following_arfcns(pp_buffer, &p_rrm_son_geran_carrier_freqs->following_arfcns))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_geran_physical_cell_id_len
(
    rrm_son_geran_physical_cell_id_t *p_rrm_son_geran_physical_cell_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_geran_physical_cell_id != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_physical_cell_id->ncc);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_physical_cell_id->bscc);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_geran_physical_cell_id
(
    U8  **pp_buffer,
    rrm_son_geran_physical_cell_id_t *p_rrm_son_geran_physical_cell_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_geran_physical_cell_id != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_physical_cell_id->ncc > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_physical_cell_id->ncc] should be in range [%d <= 7] !", p_rrm_son_geran_physical_cell_id->ncc);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_physical_cell_id->ncc, "ncc");
    *pp_buffer += sizeof(p_rrm_son_geran_physical_cell_id->ncc);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_physical_cell_id->bscc > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_physical_cell_id->bscc] should be in range [%d <= 7] !", p_rrm_son_geran_physical_cell_id->bscc);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_physical_cell_id->bscc, "bscc");
    *pp_buffer += sizeof(p_rrm_son_geran_physical_cell_id->bscc);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_geran_carrier_freq_info_len
(
    rrm_son_geran_carrier_freq_info_t *p_rrm_son_geran_carrier_freq_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_geran_carrier_freq_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_carrier_freq_info->bitmask);

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_CAR_FREQS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_geran_carrier_freqs_len(&p_rrm_son_geran_carrier_freq_info->geran_car_freqs);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_CELL_RESELECTION_PRIORITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_carrier_freq_info->cell_reselection_priority);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_NCC_PERMITTED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_carrier_freq_info->ncc_permitted);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_Q_RX_LEV_MIN_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_carrier_freq_info->q_rx_lev_min);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_MAX_ALLOWED_TRANS_PWR_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_carrier_freq_info->p_max_geran);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_THRESHX_HIGH_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_carrier_freq_info->threshX_high);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_THRESHX_LOW_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_carrier_freq_info->threshX_low);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_OFFSET_FREQ_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_carrier_freq_info->offset_frequency);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_MEAS_OBJ_ID_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_carrier_freq_info->meas_obj_id);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_CELLS_TO_REPORT_CGI_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_CELLS_TO_REPORT_CGI_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size < 1) || (p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size] should be in range [1 <= %d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size; loop++)
        {
            length += rrm_get_rrm_son_geran_physical_cell_id_len(&p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi[loop]);
        }
    }
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_geran_carrier_freq_info
(
    U8  **pp_buffer,
    rrm_son_geran_carrier_freq_info_t *p_rrm_son_geran_carrier_freq_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_geran_carrier_freq_info != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_geran_carrier_freq_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_geran_carrier_freq_info->bitmask);

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_CAR_FREQS_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_geran_carrier_freqs(pp_buffer, &p_rrm_son_geran_carrier_freq_info->geran_car_freqs))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_CELL_RESELECTION_PRIORITY_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_carrier_freq_info->cell_reselection_priority, "cell_reselection_priority");
    *pp_buffer += sizeof(p_rrm_son_geran_carrier_freq_info->cell_reselection_priority);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_NCC_PERMITTED_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_carrier_freq_info->ncc_permitted, "ncc_permitted");
    *pp_buffer += sizeof(p_rrm_son_geran_carrier_freq_info->ncc_permitted);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_Q_RX_LEV_MIN_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_carrier_freq_info->q_rx_lev_min > 45))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->q_rx_lev_min] should be in range [%d <= 45] !", p_rrm_son_geran_carrier_freq_info->q_rx_lev_min);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_carrier_freq_info->q_rx_lev_min, "q_rx_lev_min");
    *pp_buffer += sizeof(p_rrm_son_geran_carrier_freq_info->q_rx_lev_min);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_MAX_ALLOWED_TRANS_PWR_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_carrier_freq_info->p_max_geran > 39))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->p_max_geran] should be in range [%d <= 39] !", p_rrm_son_geran_carrier_freq_info->p_max_geran);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_carrier_freq_info->p_max_geran, "p_max_geran");
    *pp_buffer += sizeof(p_rrm_son_geran_carrier_freq_info->p_max_geran);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_THRESHX_HIGH_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_carrier_freq_info->threshX_high > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->threshX_high] should be in range [%d <= 31] !", p_rrm_son_geran_carrier_freq_info->threshX_high);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_carrier_freq_info->threshX_high, "threshX_high");
    *pp_buffer += sizeof(p_rrm_son_geran_carrier_freq_info->threshX_high);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_THRESHX_LOW_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_carrier_freq_info->threshX_low > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->threshX_low] should be in range [%d <= 31] !", p_rrm_son_geran_carrier_freq_info->threshX_low);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_carrier_freq_info->threshX_low, "threshX_low");
    *pp_buffer += sizeof(p_rrm_son_geran_carrier_freq_info->threshX_low);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_OFFSET_FREQ_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_carrier_freq_info->offset_frequency < -15) || (p_rrm_son_geran_carrier_freq_info->offset_frequency > 15))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->offset_frequency] should be in range [-15 <= %d <= 15] !", p_rrm_son_geran_carrier_freq_info->offset_frequency);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_geran_carrier_freq_info->offset_frequency, "offset_frequency");
    *pp_buffer += sizeof(p_rrm_son_geran_carrier_freq_info->offset_frequency);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_MEAS_OBJ_ID_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_carrier_freq_info->meas_obj_id < 1) || (p_rrm_son_geran_carrier_freq_info->meas_obj_id > 12))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->meas_obj_id] should be in range [1 <= %d <= 12] !", p_rrm_son_geran_carrier_freq_info->meas_obj_id);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_carrier_freq_info->meas_obj_id, "meas_obj_id");
    *pp_buffer += sizeof(p_rrm_son_geran_carrier_freq_info->meas_obj_id);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_CELLS_TO_REPORT_CGI_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size < 1) || (p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size] should be in range [1 <= %d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size, "cells_for_which_to_report_cgi_size");
    *pp_buffer += sizeof(p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size);
    }

    /* Optional element */
    if(p_rrm_son_geran_carrier_freq_info->bitmask & RRM_SON_GERAN_CFI_CELLS_TO_REPORT_CGI_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_geran_physical_cell_id(pp_buffer, &p_rrm_son_geran_carrier_freq_info->cells_for_which_to_report_cgi[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_meas_config_geran_len
(
    rrm_son_meas_config_geran_t *p_rrm_son_meas_config_geran
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_config_geran != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_geran->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_geran->geran_freq_info_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_geran->geran_freq_info_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_geran->geran_freq_info_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_geran->geran_freq_info_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_geran->geran_freq_info_list_size; loop++)
        {
            length += rrm_get_rrm_son_geran_carrier_freq_info_len(&p_rrm_son_meas_config_geran->geran_freq_info_list[loop]);
        }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_geran->bitmask & RRM_SON_MC_GERAN_T_RESELECTION_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_geran->t_reselection);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_geran->bitmask & RRM_SON_MC_GERAN_T_RESELECTION_SF_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_speed_state_scale_factors_len(&p_rrm_son_meas_config_geran->t_reselection_sf);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_meas_config_geran
(
    U8  **pp_buffer,
    rrm_son_meas_config_geran_t *p_rrm_son_meas_config_geran
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_config_geran != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_config_geran->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_meas_config_geran->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_geran->geran_freq_info_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_geran->geran_freq_info_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_geran->geran_freq_info_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_geran->geran_freq_info_list_size, "geran_freq_info_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_geran->geran_freq_info_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_geran->geran_freq_info_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_geran_carrier_freq_info(pp_buffer, &p_rrm_son_meas_config_geran->geran_freq_info_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_geran->bitmask & RRM_SON_MC_GERAN_T_RESELECTION_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_geran->t_reselection > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_geran->t_reselection] should be in range [%d <= 7] !", p_rrm_son_meas_config_geran->t_reselection);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_geran->t_reselection, "t_reselection");
    *pp_buffer += sizeof(p_rrm_son_meas_config_geran->t_reselection);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_geran->bitmask & RRM_SON_MC_GERAN_T_RESELECTION_SF_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_speed_state_scale_factors(pp_buffer, &p_rrm_son_meas_config_geran->t_reselection_sf))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_cdma_carrier_freq_info_len
(
    rrm_son_cdma_carrier_freq_info_t *p_rrm_son_cdma_carrier_freq_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cdma_carrier_freq_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_carrier_freq_info->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_carrier_freq_info->cdma_type);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_carrier_freq_info->band_class);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_carrier_freq_info->arfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_carrier_freq_info->threshX_high);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_carrier_freq_info->threshX_low);

    /* Optional element */
    if(p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_FREQ_OFFSET_FREQ_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_carrier_freq_info->offset_freq);
    }

    /* Optional element */
    if(p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_FREQ_CDMA_SRCH_WNDO_SIZE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_carrier_freq_info->srch_wnd_size);
    }

    /* Optional element */
    if(p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_RESELECTION_PRIORITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_carrier_freq_info->cell_reselection_priority);
    }

    /* Optional element */
    if(p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_CFI_MEAS_OBJ_ID_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_carrier_freq_info->meas_obj_id);
    }

    /* Optional element */
    if(p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_CFI_CELLS_TO_REPORT_CGI_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size);
    }

    /* Optional element */
    if(p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_CFI_CELLS_TO_REPORT_CGI_PRESENT)
    {
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size * sizeof(p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi[0]));
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_cdma_carrier_freq_info
(
    U8  **pp_buffer,
    rrm_son_cdma_carrier_freq_info_t *p_rrm_son_cdma_carrier_freq_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cdma_carrier_freq_info != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_carrier_freq_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_cdma_carrier_freq_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->cdma_type > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->cdma_type] should be in range [%d <= 1] !", p_rrm_son_cdma_carrier_freq_info->cdma_type);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_carrier_freq_info->cdma_type, "cdma_type");
    *pp_buffer += sizeof(p_rrm_son_cdma_carrier_freq_info->cdma_type);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->band_class > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->band_class] should be in range [%d <= 31] !", p_rrm_son_cdma_carrier_freq_info->band_class);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_carrier_freq_info->band_class, "band_class");
    *pp_buffer += sizeof(p_rrm_son_cdma_carrier_freq_info->band_class);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->arfcn > 2047))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->arfcn] should be in range [%d <= 2047] !", p_rrm_son_cdma_carrier_freq_info->arfcn);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_cdma_carrier_freq_info->arfcn, "arfcn");
    *pp_buffer += sizeof(p_rrm_son_cdma_carrier_freq_info->arfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->threshX_high > 63))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->threshX_high] should be in range [%d <= 63] !", p_rrm_son_cdma_carrier_freq_info->threshX_high);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_carrier_freq_info->threshX_high, "threshX_high");
    *pp_buffer += sizeof(p_rrm_son_cdma_carrier_freq_info->threshX_high);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->threshX_low > 63))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->threshX_low] should be in range [%d <= 63] !", p_rrm_son_cdma_carrier_freq_info->threshX_low);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_carrier_freq_info->threshX_low, "threshX_low");
    *pp_buffer += sizeof(p_rrm_son_cdma_carrier_freq_info->threshX_low);

    /* Optional element */
    if(p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_FREQ_OFFSET_FREQ_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_cdma_carrier_freq_info->offset_freq < -15) || (p_rrm_son_cdma_carrier_freq_info->offset_freq > 15))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->offset_freq] should be in range [-15 <= %d <= 15] !", p_rrm_son_cdma_carrier_freq_info->offset_freq);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_cdma_carrier_freq_info->offset_freq, "offset_freq");
    *pp_buffer += sizeof(p_rrm_son_cdma_carrier_freq_info->offset_freq);
    }

    /* Optional element */
    if(p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_FREQ_CDMA_SRCH_WNDO_SIZE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->srch_wnd_size > 15))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->srch_wnd_size] should be in range [%d <= 15] !", p_rrm_son_cdma_carrier_freq_info->srch_wnd_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_carrier_freq_info->srch_wnd_size, "srch_wnd_size");
    *pp_buffer += sizeof(p_rrm_son_cdma_carrier_freq_info->srch_wnd_size);
    }

    /* Optional element */
    if(p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_RESELECTION_PRIORITY_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_carrier_freq_info->cell_reselection_priority > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->cell_reselection_priority] should be in range [%d <= 7] !", p_rrm_son_cdma_carrier_freq_info->cell_reselection_priority);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_carrier_freq_info->cell_reselection_priority, "cell_reselection_priority");
    *pp_buffer += sizeof(p_rrm_son_cdma_carrier_freq_info->cell_reselection_priority);
    }

    /* Optional element */
    if(p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_CFI_MEAS_OBJ_ID_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_cdma_carrier_freq_info->meas_obj_id < 1) || (p_rrm_son_cdma_carrier_freq_info->meas_obj_id > 12))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->meas_obj_id] should be in range [1 <= %d <= 12] !", p_rrm_son_cdma_carrier_freq_info->meas_obj_id);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_carrier_freq_info->meas_obj_id, "meas_obj_id");
    *pp_buffer += sizeof(p_rrm_son_cdma_carrier_freq_info->meas_obj_id);
    }

    /* Optional element */
    if(p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_CFI_CELLS_TO_REPORT_CGI_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size < 1) || (p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size] should be in range [1 <= %d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size, "cells_for_which_to_report_cgi_size");
    *pp_buffer += sizeof(p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size);
    }

    /* Optional element */
    if(p_rrm_son_cdma_carrier_freq_info->bitmask & RRM_SON_CDMA_CFI_CELLS_TO_REPORT_CGI_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi_size; loop++)
        {
            rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_cdma_carrier_freq_info->cells_for_which_to_report_cgi[loop], "cells_for_which_to_report_cgi[]");
            *pp_buffer += sizeof(U16);
        }
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_cdma_sys_time_info_len
(
    rrm_son_cdma_sys_time_info_t *p_rrm_son_cdma_sys_time_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cdma_sys_time_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_sys_time_info->is_cdma_nw_sync);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_sys_time_info->cdma_sys_time_size);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_son_cdma_sys_time_info->cdma_sys_time_size * sizeof(p_rrm_son_cdma_sys_time_info->cdma_sys_time[0]));

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_cdma_sys_time_info
(
    U8  **pp_buffer,
    rrm_son_cdma_sys_time_info_t *p_rrm_son_cdma_sys_time_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cdma_sys_time_info != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_sys_time_info->is_cdma_nw_sync > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_sys_time_info->is_cdma_nw_sync] should be in range [%d <= 1] !", p_rrm_son_cdma_sys_time_info->is_cdma_nw_sync);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_sys_time_info->is_cdma_nw_sync, "is_cdma_nw_sync");
    *pp_buffer += sizeof(p_rrm_son_cdma_sys_time_info->is_cdma_nw_sync);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_cdma_sys_time_info->cdma_sys_time_size < 5) || (p_rrm_son_cdma_sys_time_info->cdma_sys_time_size > RRM_SON_MAX_CDMA_SYS_TIME))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_sys_time_info->cdma_sys_time_size] should be in range [5 <= %d <= RRM_SON_MAX_CDMA_SYS_TIME] !", p_rrm_son_cdma_sys_time_info->cdma_sys_time_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_sys_time_info->cdma_sys_time_size, "cdma_sys_time_size");
    *pp_buffer += sizeof(p_rrm_son_cdma_sys_time_info->cdma_sys_time_size);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_cdma_sys_time_info->cdma_sys_time_size; loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_sys_time_info->cdma_sys_time[loop], "cdma_sys_time[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_pre_reg_hrpd_info_len
(
    rrm_son_pre_reg_hrpd_info_t *p_rrm_son_pre_reg_hrpd_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_pre_reg_hrpd_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_pre_reg_hrpd_info->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_pre_reg_hrpd_info->pre_reg_allowed);

    /* Optional element */
    if(p_rrm_son_pre_reg_hrpd_info->bitmask & RRM_SON_HRPD_PRE_REG_ZONE_ID_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_pre_reg_hrpd_info->pre_reg_zone_id);
    }

    /* Optional element */
    if(p_rrm_son_pre_reg_hrpd_info->bitmask & RRM_SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size);
    }

    /* Optional element */
    if(p_rrm_son_pre_reg_hrpd_info->bitmask & RRM_SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT)
    {
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size * sizeof(p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list[0]));
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_pre_reg_hrpd_info
(
    U8  **pp_buffer,
    rrm_son_pre_reg_hrpd_info_t *p_rrm_son_pre_reg_hrpd_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_pre_reg_hrpd_info != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_pre_reg_hrpd_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_pre_reg_hrpd_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_pre_reg_hrpd_info->pre_reg_allowed > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_pre_reg_hrpd_info->pre_reg_allowed] should be in range [%d <= 1] !", p_rrm_son_pre_reg_hrpd_info->pre_reg_allowed);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_pre_reg_hrpd_info->pre_reg_allowed, "pre_reg_allowed");
    *pp_buffer += sizeof(p_rrm_son_pre_reg_hrpd_info->pre_reg_allowed);

    /* Optional element */
    if(p_rrm_son_pre_reg_hrpd_info->bitmask & RRM_SON_HRPD_PRE_REG_ZONE_ID_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_pre_reg_hrpd_info->pre_reg_zone_id, "pre_reg_zone_id");
    *pp_buffer += sizeof(p_rrm_son_pre_reg_hrpd_info->pre_reg_zone_id);
    }

    /* Optional element */
    if(p_rrm_son_pre_reg_hrpd_info->bitmask & RRM_SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size < 1) || (p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size > RRM_SON_MAX_SEC_PRE_REG_ZONES))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size] should be in range [1 <= %d <= RRM_SON_MAX_SEC_PRE_REG_ZONES] !", p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size, "scndry_pre_reg_zone_id_list_size");
    *pp_buffer += sizeof(p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size);
    }

    /* Optional element */
    if(p_rrm_son_pre_reg_hrpd_info->bitmask & RRM_SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list_size; loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_pre_reg_hrpd_info->scndry_pre_reg_zone_id_list[loop], "scndry_pre_reg_zone_id_list[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_cdma_hrpd_info_len
(
    rrm_son_cdma_hrpd_info_t *p_rrm_son_cdma_hrpd_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cdma_hrpd_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_hrpd_info->bitmask);

    /* Optional element */
    if(p_rrm_son_cdma_hrpd_info->bitmask & RRM_SON_CDMA_HRPD_REG_PARAMS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_pre_reg_hrpd_info_len(&p_rrm_son_cdma_hrpd_info->hrpd_reg_params);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_hrpd_info->t_reselection);

    /* Optional element */
    if(p_rrm_son_cdma_hrpd_info->bitmask & RRM_SON_CDMA_HRPD_RESELECTION_SF_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_speed_state_scale_factors_len(&p_rrm_son_cdma_hrpd_info->t_reselection_sf);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_cdma_hrpd_info
(
    U8  **pp_buffer,
    rrm_son_cdma_hrpd_info_t *p_rrm_son_cdma_hrpd_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cdma_hrpd_info != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_hrpd_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_cdma_hrpd_info->bitmask);

    /* Optional element */
    if(p_rrm_son_cdma_hrpd_info->bitmask & RRM_SON_CDMA_HRPD_REG_PARAMS_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_pre_reg_hrpd_info(pp_buffer, &p_rrm_son_cdma_hrpd_info->hrpd_reg_params))
    {
        return RRM_FAILURE;
    }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_hrpd_info->t_reselection > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_hrpd_info->t_reselection] should be in range [%d <= 7] !", p_rrm_son_cdma_hrpd_info->t_reselection);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_hrpd_info->t_reselection, "t_reselection");
    *pp_buffer += sizeof(p_rrm_son_cdma_hrpd_info->t_reselection);

    /* Optional element */
    if(p_rrm_son_cdma_hrpd_info->bitmask & RRM_SON_CDMA_HRPD_RESELECTION_SF_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_speed_state_scale_factors(pp_buffer, &p_rrm_son_cdma_hrpd_info->t_reselection_sf))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_cdma_csfb_reg_param_1XRTT_len
(
    rrm_son_cdma_csfb_reg_param_1XRTT_t *p_rrm_son_cdma_csfb_reg_param_1XRTT
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cdma_csfb_reg_param_1XRTT != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->sid);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->nid);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_sid);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_nid);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->home_reg);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->parameter_reg);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->power_up_reg);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->reg_period);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->reg_zone);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->total_zone);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->zone_timer);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_cdma_csfb_reg_param_1XRTT
(
    U8  **pp_buffer,
    rrm_son_cdma_csfb_reg_param_1XRTT_t *p_rrm_son_cdma_csfb_reg_param_1XRTT
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cdma_csfb_reg_param_1XRTT != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->sid, "sid");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->sid);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->nid, "nid");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->nid);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_sid > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_sid] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_sid);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_sid, "multiple_sid");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_sid);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_nid > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_nid] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_nid);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_nid, "multiple_nid");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->multiple_nid);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->home_reg > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->home_reg] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->home_reg);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->home_reg, "home_reg");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->home_reg);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg, "foreign_sid_reg");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_sid_reg);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg, "foreign_nid_reg");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->foreign_nid_reg);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->parameter_reg > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->parameter_reg] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->parameter_reg);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->parameter_reg, "parameter_reg");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->parameter_reg);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1XRTT->power_up_reg > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1XRTT->power_up_reg] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1XRTT->power_up_reg);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->power_up_reg, "power_up_reg");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->power_up_reg);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->reg_period, "reg_period");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->reg_period);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->reg_zone, "reg_zone");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->reg_zone);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->total_zone, "total_zone");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->total_zone);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1XRTT->zone_timer, "zone_timer");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1XRTT->zone_timer);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_cdma_1xrtt_info_len
(
    rrm_son_cdma_1xrtt_info_t *p_rrm_son_cdma_1xrtt_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cdma_1xrtt_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_1xrtt_info->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_1xrtt_info->t_reselection);

    /* Optional element */
    if(p_rrm_son_cdma_1xrtt_info->bitmask & RRM_SON_CDMA_1XRTT_REG_PARAMS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_cdma_csfb_reg_param_1XRTT_len(&p_rrm_son_cdma_1xrtt_info->xrtt_reg_params);
    }

    /* Optional element */
    if(p_rrm_son_cdma_1xrtt_info->bitmask & RRM_SON_CDMA_1XRTT_LONG_CODE_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_son_cdma_1xrtt_info->cdma_1xrtt_long_code);
    }

    /* Optional element */
    if(p_rrm_son_cdma_1xrtt_info->bitmask & RRM_SON_CDMA_1XRTT_RESELECTION_SF_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_speed_state_scale_factors_len(&p_rrm_son_cdma_1xrtt_info->t_reselection_sf);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_cdma_1xrtt_info
(
    U8  **pp_buffer,
    rrm_son_cdma_1xrtt_info_t *p_rrm_son_cdma_1xrtt_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cdma_1xrtt_info != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_1xrtt_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_cdma_1xrtt_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_1xrtt_info->t_reselection > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_1xrtt_info->t_reselection] should be in range [%d <= 7] !", p_rrm_son_cdma_1xrtt_info->t_reselection);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_1xrtt_info->t_reselection, "t_reselection");
    *pp_buffer += sizeof(p_rrm_son_cdma_1xrtt_info->t_reselection);

    /* Optional element */
    if(p_rrm_son_cdma_1xrtt_info->bitmask & RRM_SON_CDMA_1XRTT_REG_PARAMS_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_cdma_csfb_reg_param_1XRTT(pp_buffer, &p_rrm_son_cdma_1xrtt_info->xrtt_reg_params))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_cdma_1xrtt_info->bitmask & RRM_SON_CDMA_1XRTT_LONG_CODE_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_cdma_1xrtt_info->cdma_1xrtt_long_code); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_1xrtt_info->cdma_1xrtt_long_code[loop], "cdma_1xrtt_long_code[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_cdma_1xrtt_info->bitmask & RRM_SON_CDMA_1XRTT_RESELECTION_SF_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_speed_state_scale_factors(pp_buffer, &p_rrm_son_cdma_1xrtt_info->t_reselection_sf))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_cdma_csfb_reg_param_1xrtt_r9_len
(
    rrm_son_cdma_csfb_reg_param_1xrtt_r9_t *p_rrm_son_cdma_csfb_reg_param_1xrtt_r9
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cdma_csfb_reg_param_1xrtt_r9 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_cdma_csfb_reg_param_1xrtt_r9
(
    U8  **pp_buffer,
    rrm_son_cdma_csfb_reg_param_1xrtt_r9_t *p_rrm_son_cdma_csfb_reg_param_1xrtt_r9
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cdma_csfb_reg_param_1xrtt_r9 != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9 > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9] should be in range [%d <= 1] !", p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9, "power_down_reg_r9");
    *pp_buffer += sizeof(p_rrm_son_cdma_csfb_reg_param_1xrtt_r9->power_down_reg_r9);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_ac_barring_config_1xrtt_len
(
    rrm_son_ac_barring_config_1xrtt_t *p_rrm_son_ac_barring_config_1xrtt
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_ac_barring_config_1xrtt != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ac_barring_config_1xrtt->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_10_r9);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_11_r9);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_12_r9);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_13_r9);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_14_r9);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_15_r9);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_msg_r9);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_reg_r9);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_emg_r9);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_ac_barring_config_1xrtt
(
    U8  **pp_buffer,
    rrm_son_ac_barring_config_1xrtt_t *p_rrm_son_ac_barring_config_1xrtt
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_ac_barring_config_1xrtt != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_ac_barring_config_1xrtt->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_ac_barring_config_1xrtt->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9 > 63))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9] should be in range [%d <= 63] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9, "ac_barring_0_to_9_r9");
    *pp_buffer += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_0_to_9_r9);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_10_r9 > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_10_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_10_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_ac_barring_config_1xrtt->ac_barring_10_r9, "ac_barring_10_r9");
    *pp_buffer += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_10_r9);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_11_r9 > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_11_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_11_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_ac_barring_config_1xrtt->ac_barring_11_r9, "ac_barring_11_r9");
    *pp_buffer += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_11_r9);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_12_r9 > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_12_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_12_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_ac_barring_config_1xrtt->ac_barring_12_r9, "ac_barring_12_r9");
    *pp_buffer += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_12_r9);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_13_r9 > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_13_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_13_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_ac_barring_config_1xrtt->ac_barring_13_r9, "ac_barring_13_r9");
    *pp_buffer += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_13_r9);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_14_r9 > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_14_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_14_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_ac_barring_config_1xrtt->ac_barring_14_r9, "ac_barring_14_r9");
    *pp_buffer += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_14_r9);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_15_r9 > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_15_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_15_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_ac_barring_config_1xrtt->ac_barring_15_r9, "ac_barring_15_r9");
    *pp_buffer += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_15_r9);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_msg_r9 > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_msg_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_msg_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_ac_barring_config_1xrtt->ac_barring_msg_r9, "ac_barring_msg_r9");
    *pp_buffer += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_msg_r9);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_reg_r9 > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_reg_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_reg_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_ac_barring_config_1xrtt->ac_barring_reg_r9, "ac_barring_reg_r9");
    *pp_buffer += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_reg_r9);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ac_barring_config_1xrtt->ac_barring_emg_r9 > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_ac_barring_config_1xrtt->ac_barring_emg_r9] should be in range [%d <= 7] !", p_rrm_son_ac_barring_config_1xrtt->ac_barring_emg_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_ac_barring_config_1xrtt->ac_barring_emg_r9, "ac_barring_emg_r9");
    *pp_buffer += sizeof(p_rrm_son_ac_barring_config_1xrtt->ac_barring_emg_r9);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_meas_config_cdma_len
(
    rrm_son_meas_config_cdma_t *p_rrm_son_meas_config_cdma
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_config_cdma != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_cdma->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_cdma->cdma_freq_info_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_cdma->cdma_freq_info_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_cdma->cdma_freq_info_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_cdma->cdma_freq_info_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_cdma->cdma_freq_info_list_size; loop++)
        {
            length += rrm_get_rrm_son_cdma_carrier_freq_info_len(&p_rrm_son_meas_config_cdma->cdma_freq_info_list[loop]);
        }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CDMA_SYS_TIME_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_cdma_sys_time_info_len(&p_rrm_son_meas_config_cdma->sys_time_info);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CDMA_SRCH_WNDO_SIZE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_cdma->srch_wnd_size);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CDMA_PRE_REG_HRPD_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_cdma_hrpd_info_len(&p_rrm_son_meas_config_cdma->hrpd_info);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CDMA_1X_RTT_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_cdma_1xrtt_info_len(&p_rrm_son_meas_config_cdma->cdma_1xrtt_info);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CSFB_SUPPORT_FOR_DUAL_RX_UES_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CSFB_REG_PARAM_1XRTT_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_cdma_csfb_reg_param_1xrtt_r9_len(&p_rrm_son_meas_config_cdma->csfb_registration_param_1xrtt_r9);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_AC_BARRING_CONFIG_1XRTT_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_ac_barring_config_1xrtt_len(&p_rrm_son_meas_config_cdma->ac_barring_config_1xrtt_r9);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_meas_config_cdma
(
    U8  **pp_buffer,
    rrm_son_meas_config_cdma_t *p_rrm_son_meas_config_cdma
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_config_cdma != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_config_cdma->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_meas_config_cdma->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_cdma->cdma_freq_info_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_cdma->cdma_freq_info_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_cdma->cdma_freq_info_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_cdma->cdma_freq_info_list_size, "cdma_freq_info_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_cdma->cdma_freq_info_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_cdma->cdma_freq_info_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_cdma_carrier_freq_info(pp_buffer, &p_rrm_son_meas_config_cdma->cdma_freq_info_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CDMA_SYS_TIME_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_cdma_sys_time_info(pp_buffer, &p_rrm_son_meas_config_cdma->sys_time_info))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CDMA_SRCH_WNDO_SIZE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_cdma->srch_wnd_size > 15))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_cdma->srch_wnd_size] should be in range [%d <= 15] !", p_rrm_son_meas_config_cdma->srch_wnd_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_cdma->srch_wnd_size, "srch_wnd_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_cdma->srch_wnd_size);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CDMA_PRE_REG_HRPD_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_cdma_hrpd_info(pp_buffer, &p_rrm_son_meas_config_cdma->hrpd_info))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CDMA_1X_RTT_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_cdma_1xrtt_info(pp_buffer, &p_rrm_son_meas_config_cdma->cdma_1xrtt_info))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CSFB_SUPPORT_FOR_DUAL_RX_UES_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9 > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9] should be in range [%d <= 1] !", p_rrm_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9, "csfb_support_for_dual_Rx_Ues_r9");
    *pp_buffer += sizeof(p_rrm_son_meas_config_cdma->csfb_support_for_dual_Rx_Ues_r9);
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_CSFB_REG_PARAM_1XRTT_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_cdma_csfb_reg_param_1xrtt_r9(pp_buffer, &p_rrm_son_meas_config_cdma->csfb_registration_param_1xrtt_r9))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_config_cdma->bitmask & RRM_SON_MEAS_CONFIG_AC_BARRING_CONFIG_1XRTT_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_ac_barring_config_1xrtt(pp_buffer, &p_rrm_son_meas_config_cdma->ac_barring_config_1xrtt_r9))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_inter_rat_meas_config_len
(
    rrm_son_inter_rat_meas_config_t *p_rrm_son_inter_rat_meas_config
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_inter_rat_meas_config != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_meas_config_utran_len(&p_rrm_son_inter_rat_meas_config->utran_config_list);

    /* Get length of IE */
    length += rrm_get_rrm_son_meas_config_geran_len(&p_rrm_son_inter_rat_meas_config->geran_config_list);

    /* Get length of IE */
    length += rrm_get_rrm_son_meas_config_cdma_len(&p_rrm_son_inter_rat_meas_config->cdma_config_list);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_inter_rat_meas_config
(
    U8  **pp_buffer,
    rrm_son_inter_rat_meas_config_t *p_rrm_son_inter_rat_meas_config
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_inter_rat_meas_config != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_meas_config_utran(pp_buffer, &p_rrm_son_inter_rat_meas_config->utran_config_list))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_meas_config_geran(pp_buffer, &p_rrm_son_inter_rat_meas_config->geran_config_list))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_meas_config_cdma(pp_buffer, &p_rrm_son_inter_rat_meas_config->cdma_config_list))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_setup_meas_gap_config_len
(
    rrm_son_setup_meas_gap_config_t *p_rrm_son_setup_meas_gap_config
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_setup_meas_gap_config != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_setup_meas_gap_config->bitmask);

    /* Optional element */
    if(p_rrm_son_setup_meas_gap_config->bitmask & RRM_SON_GAP_OFFSET_GP0_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_setup_meas_gap_config->gap_offset_gp0);
    }

    /* Optional element */
    if(p_rrm_son_setup_meas_gap_config->bitmask & RRM_SON_GAP_OFFSET_GP1_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_setup_meas_gap_config->gap_offset_gp1);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_setup_meas_gap_config
(
    U8  **pp_buffer,
    rrm_son_setup_meas_gap_config_t *p_rrm_son_setup_meas_gap_config
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_setup_meas_gap_config != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_setup_meas_gap_config->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_setup_meas_gap_config->bitmask);

    /* Optional element */
    if(p_rrm_son_setup_meas_gap_config->bitmask & RRM_SON_GAP_OFFSET_GP0_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_setup_meas_gap_config->gap_offset_gp0 > 39))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_setup_meas_gap_config->gap_offset_gp0] should be in range [%d <= 39] !", p_rrm_son_setup_meas_gap_config->gap_offset_gp0);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_setup_meas_gap_config->gap_offset_gp0, "gap_offset_gp0");
    *pp_buffer += sizeof(p_rrm_son_setup_meas_gap_config->gap_offset_gp0);
    }

    /* Optional element */
    if(p_rrm_son_setup_meas_gap_config->bitmask & RRM_SON_GAP_OFFSET_GP1_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_setup_meas_gap_config->gap_offset_gp1 > 79))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_setup_meas_gap_config->gap_offset_gp1] should be in range [%d <= 79] !", p_rrm_son_setup_meas_gap_config->gap_offset_gp1);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_setup_meas_gap_config->gap_offset_gp1, "gap_offset_gp1");
    *pp_buffer += sizeof(p_rrm_son_setup_meas_gap_config->gap_offset_gp1);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_gap_config_len
(
    rrm_son_gap_config_t *p_rrm_son_gap_config
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_gap_config != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_gap_config->bitmask);

    /* Optional element */
    if(p_rrm_son_gap_config->bitmask & RRM_SON_SETUP_GAP_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_setup_meas_gap_config_len(&p_rrm_son_gap_config->setup_gap_config);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_gap_config
(
    U8  **pp_buffer,
    rrm_son_gap_config_t *p_rrm_son_gap_config
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_gap_config != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_gap_config->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_gap_config->bitmask);

    /* Optional element */
    if(p_rrm_son_gap_config->bitmask & RRM_SON_SETUP_GAP_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_setup_meas_gap_config(pp_buffer, &p_rrm_son_gap_config->setup_gap_config))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_meas_gap_config_len
(
    rrm_son_meas_gap_config_t *p_rrm_son_meas_gap_config
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_gap_config != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_gap_config->bitmask);

    /* Optional element */
    if(p_rrm_son_meas_gap_config->bitmask & RRM_SON_EUTRAN_GAP_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_gap_config_len(&p_rrm_son_meas_gap_config->eutran_gap_config);
    }

    /* Optional element */
    if(p_rrm_son_meas_gap_config->bitmask & RRM_SON_UTRAN_GAP_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_gap_config_len(&p_rrm_son_meas_gap_config->utran_gap_config);
    }

    /* Optional element */
    if(p_rrm_son_meas_gap_config->bitmask & RRM_SON_GERAN_GAP_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_gap_config_len(&p_rrm_son_meas_gap_config->geran_gap_config);
    }

    /* Optional element */
    if(p_rrm_son_meas_gap_config->bitmask & RRM_SON_CDMA2000_GAP_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_gap_config_len(&p_rrm_son_meas_gap_config->cdma2000_gap_config);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_meas_gap_config
(
    U8  **pp_buffer,
    rrm_son_meas_gap_config_t *p_rrm_son_meas_gap_config
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_gap_config != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_gap_config->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_meas_gap_config->bitmask);

    /* Optional element */
    if(p_rrm_son_meas_gap_config->bitmask & RRM_SON_EUTRAN_GAP_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_gap_config(pp_buffer, &p_rrm_son_meas_gap_config->eutran_gap_config))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_gap_config->bitmask & RRM_SON_UTRAN_GAP_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_gap_config(pp_buffer, &p_rrm_son_meas_gap_config->utran_gap_config))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_gap_config->bitmask & RRM_SON_GERAN_GAP_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_gap_config(pp_buffer, &p_rrm_son_meas_gap_config->geran_gap_config))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_gap_config->bitmask & RRM_SON_CDMA2000_GAP_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_gap_config(pp_buffer, &p_rrm_son_meas_gap_config->cdma2000_gap_config))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_meas_config_res_len
(
    rrm_son_meas_config_res_t *p_rrm_son_meas_config_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_config_res != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_non_registration_based_res_len(&p_rrm_son_meas_config_res->meas_config_res);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_meas_config_res
(
    U8  **pp_buffer,
    rrm_son_meas_config_res_t *p_rrm_son_meas_config_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_config_res != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_non_registration_based_res(pp_buffer, &p_rrm_son_meas_config_res->meas_config_res))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_meas_config_res
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_MEAS_CONFIG_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_meas_config_res
(
    rrm_son_meas_config_res_t  *p_rrm_son_meas_config_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_meas_config_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_meas_config_res_len(p_rrm_son_meas_config_res);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_MEAS_CONFIG_RESP", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_MEAS_CONFIG_RESP, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_MEAS_CONFIG_RESP, msg_length,cell_index);

    /* Fill RRM_SON_MEAS_CONFIG_RESP message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_meas_config_res(&p_son_msg, p_rrm_son_meas_config_res))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_meas_config_obj_remove_req_len
(
    rrm_son_meas_config_obj_remove_req_t *p_rrm_son_meas_config_obj_remove_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_config_obj_remove_req != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_meas_config_obj_remove_req->cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size * sizeof(p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list[0]));
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size * sizeof(p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list[0]));
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size * sizeof(p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list[0]));
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size; loop++)
        {
            length += rrm_get_rrm_son_geran_carrier_freqs_len(&p_rrm_son_meas_config_obj_remove_req->geran_remove_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size; loop++)
        {
            length += rrm_get_rrm_son_cdma_carrier_freq_info_len(&p_rrm_son_meas_config_obj_remove_req->cdma_remove_list[loop]);
        }
    }

    return length;
}

rrm_return_et
rrm_compose_rrm_son_meas_config_obj_remove_req
(
    U8  **pp_buffer,
    rrm_son_meas_config_obj_remove_req_t *p_rrm_son_meas_config_obj_remove_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_config_obj_remove_req != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_meas_config_obj_remove_req->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size, "earfcn_remove_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list_size; loop++)
        {
            rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_meas_config_obj_remove_req->earfcn_remove_list[loop], "earfcn_remove_list[]");
            *pp_buffer += sizeof(U16);
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size, "uarfcn_fdd_remove_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list_size; loop++)
        {
            rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_meas_config_obj_remove_req->uarfcn_fdd_remove_list[loop], "uarfcn_fdd_remove_list[]");
            *pp_buffer += sizeof(U16);
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size, "uarfcn_tdd_remove_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list_size; loop++)
        {
            rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_meas_config_obj_remove_req->uarfcn_tdd_remove_list[loop], "uarfcn_tdd_remove_list[]");
            *pp_buffer += sizeof(U16);
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size, "geran_remove_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_obj_remove_req->geran_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_geran_carrier_freqs(pp_buffer, &p_rrm_son_meas_config_obj_remove_req->geran_remove_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size > MAX_SON_MEAS_CONFIG_OBJECT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size] should be in range [%d <= MAX_SON_MEAS_CONFIG_OBJECT] !", p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size, "cdma_remove_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_config_obj_remove_req->cdma_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_cdma_carrier_freq_info(pp_buffer, &p_rrm_son_meas_config_obj_remove_req->cdma_remove_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_meas_config_obj_remove_req
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_MEAS_CONFIG_OBJ_REMOVE_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_meas_config_obj_remove_req
(
    rrm_son_meas_config_obj_remove_req_t  *p_rrm_son_meas_config_obj_remove_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_meas_config_obj_remove_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_meas_config_obj_remove_req_len(p_rrm_son_meas_config_obj_remove_req);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_MEAS_CONFIG_OBJ_REMOVE_REQ", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_MEAS_CONFIG_OBJ_REMOVE_REQ, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_MEAS_CONFIG_OBJ_REMOVE_REQ, msg_length,cell_index);

    /* Fill RRM_SON_MEAS_CONFIG_OBJ_REMOVE_REQ message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_meas_config_obj_remove_req(&p_son_msg, p_rrm_son_meas_config_obj_remove_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_meas_config_obj_remove_res_len
(
    rrm_son_meas_config_obj_remove_res_t *p_rrm_son_meas_config_obj_remove_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_config_obj_remove_res != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_non_registration_based_res_len(&p_rrm_son_meas_config_obj_remove_res->meas_config_obj_remove_res);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_meas_config_obj_remove_res
(
    U8  **pp_buffer,
    rrm_son_meas_config_obj_remove_res_t *p_rrm_son_meas_config_obj_remove_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_config_obj_remove_res != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_non_registration_based_res(pp_buffer, &p_rrm_son_meas_config_obj_remove_res->meas_config_obj_remove_res))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_meas_config_obj_remove_res
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_meas_config_obj_remove_res
(
    rrm_son_meas_config_obj_remove_res_t  *p_rrm_son_meas_config_obj_remove_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_meas_config_obj_remove_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_meas_config_obj_remove_res_len(p_rrm_son_meas_config_obj_remove_res);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP, msg_length,cell_index);

    /* Fill RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_meas_config_obj_remove_res(&p_son_msg, p_rrm_son_meas_config_obj_remove_res))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_meas_results_ind_len
(
    rrm_son_meas_results_ind_t *p_rrm_son_meas_results_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_results_ind != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_meas_results_ind->cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_results_ind->ue_index);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_results_ind->meas_report_event);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_results_ind->eutran_meas_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->eutran_meas_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_results_ind->eutran_meas_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_meas_results_ind->eutran_meas_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_results_ind->eutran_meas_list_size; loop++)
        {
            length += rrm_get_rrm_son_eutran_meas_results_len(&p_rrm_son_meas_results_ind->eutran_meas_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_results_ind->utran_meas_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->utran_meas_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_results_ind->utran_meas_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_meas_results_ind->utran_meas_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_results_ind->utran_meas_list_size; loop++)
        {
            length += rrm_get_rrm_son_utran_meas_results_len(&p_rrm_son_meas_results_ind->utran_meas[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_results_ind->geran_meas_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->geran_meas_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_results_ind->geran_meas_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_meas_results_ind->geran_meas_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_results_ind->geran_meas_list_size; loop++)
        {
            length += rrm_get_rrm_son_geran_meas_results_len(&p_rrm_son_meas_results_ind->geran_meas[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_results_ind->cdma_meas_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->cdma_meas_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_results_ind->cdma_meas_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_meas_results_ind->cdma_meas_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_results_ind->cdma_meas_list_size; loop++)
        {
            length += rrm_get_rrm_son_cdma_meas_results_len(&p_rrm_son_meas_results_ind->cdma_meas[loop]);
        }
    }

    return length;
}

rrm_return_et
rrm_compose_rrm_son_meas_results_ind
(
    U8  **pp_buffer,
    rrm_son_meas_results_ind_t *p_rrm_son_meas_results_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_results_ind != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_meas_results_ind->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_meas_results_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrm_son_meas_results_ind->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->meas_report_event > 599))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_results_ind->meas_report_event] should be in range [%d <= 599] !", p_rrm_son_meas_results_ind->meas_report_event);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_results_ind->meas_report_event, "meas_report_event");
    *pp_buffer += sizeof(p_rrm_son_meas_results_ind->meas_report_event);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->eutran_meas_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_results_ind->eutran_meas_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_meas_results_ind->eutran_meas_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_results_ind->eutran_meas_list_size, "eutran_meas_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_results_ind->eutran_meas_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_results_ind->eutran_meas_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_eutran_meas_results(pp_buffer, &p_rrm_son_meas_results_ind->eutran_meas_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->utran_meas_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_results_ind->utran_meas_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_meas_results_ind->utran_meas_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_results_ind->utran_meas_list_size, "utran_meas_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_results_ind->utran_meas_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_results_ind->utran_meas_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_utran_meas_results(pp_buffer, &p_rrm_son_meas_results_ind->utran_meas[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->geran_meas_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_results_ind->geran_meas_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_meas_results_ind->geran_meas_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_results_ind->geran_meas_list_size, "geran_meas_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_results_ind->geran_meas_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_results_ind->geran_meas_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_geran_meas_results(pp_buffer, &p_rrm_son_meas_results_ind->geran_meas[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_meas_results_ind->cdma_meas_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_meas_results_ind->cdma_meas_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_meas_results_ind->cdma_meas_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_meas_results_ind->cdma_meas_list_size, "cdma_meas_list_size");
    *pp_buffer += sizeof(p_rrm_son_meas_results_ind->cdma_meas_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_meas_results_ind->cdma_meas_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_cdma_meas_results(pp_buffer, &p_rrm_son_meas_results_ind->cdma_meas[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_meas_results_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_MEAS_RESULTS_IND message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_meas_results_ind
(
    rrm_son_meas_results_ind_t  *p_rrm_son_meas_results_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_meas_results_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_meas_results_ind_len(p_rrm_son_meas_results_ind);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_MEAS_RESULTS_IND", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_MEAS_RESULTS_IND, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_MEAS_RESULTS_IND, msg_length,cell_index);

    /* Fill RRM_SON_MEAS_RESULTS_IND message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_meas_results_ind(&p_son_msg, p_rrm_son_meas_results_ind))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_eutran_neighbor_cell_len
(
    rrm_son_eutran_neighbor_cell_t *p_rrm_son_eutran_neighbor_cell
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_eutran_neighbor_cell != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eutran_neighbor_cell->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eutran_neighbor_cell->pci);

    /* Optional element */
    if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CGI_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_eutran_neighbor_cell->cgi);
    }

    /* Optional element */
    if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_PLMN_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eutran_neighbor_cell->num_plmn_id);
    }

    /* Optional element */
    if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_PLMN_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_eutran_neighbor_cell->num_plmn_id < 1) || (p_rrm_son_eutran_neighbor_cell->num_plmn_id > MAX_PLMN_ID_LIST2))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_eutran_neighbor_cell->num_plmn_id] should be in range [1 <= %d <= MAX_PLMN_ID_LIST2] !", p_rrm_son_eutran_neighbor_cell->num_plmn_id);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_eutran_neighbor_cell->num_plmn_id; loop++)
        {
            length += rrm_get_rrm_oam_cell_plmn_info_len(&p_rrm_son_eutran_neighbor_cell->plmn_id[loop]);
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_TAC_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_son_eutran_neighbor_cell->tac);
    }

    /* Optional element */
    if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CSG_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_son_eutran_neighbor_cell->csg_identity);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eutran_neighbor_cell->rsrp);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eutran_neighbor_cell->rsrq);

    /* Optional element */
    if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CSG_MEM_STATUS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eutran_neighbor_cell->csg_membershipStatus);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_eutran_neighbor_cell
(
    U8  **pp_buffer,
    rrm_son_eutran_neighbor_cell_t *p_rrm_son_eutran_neighbor_cell
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_eutran_neighbor_cell != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_eutran_neighbor_cell->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_eutran_neighbor_cell->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_neighbor_cell->pci > 503))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_eutran_neighbor_cell->pci] should be in range [%d <= 503] !", p_rrm_son_eutran_neighbor_cell->pci);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_eutran_neighbor_cell->pci, "pci");
    *pp_buffer += sizeof(p_rrm_son_eutran_neighbor_cell->pci);

    /* Optional element */
    if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CGI_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_eutran_neighbor_cell->cgi))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_PLMN_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_eutran_neighbor_cell->num_plmn_id < 1) || (p_rrm_son_eutran_neighbor_cell->num_plmn_id > MAX_PLMN_ID_LIST2))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_eutran_neighbor_cell->num_plmn_id] should be in range [1 <= %d <= MAX_PLMN_ID_LIST2] !", p_rrm_son_eutran_neighbor_cell->num_plmn_id);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_eutran_neighbor_cell->num_plmn_id, "num_plmn_id");
    *pp_buffer += sizeof(p_rrm_son_eutran_neighbor_cell->num_plmn_id);
    }

    /* Optional element */
    if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_PLMN_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_eutran_neighbor_cell->num_plmn_id; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_oam_cell_plmn_info(pp_buffer, &p_rrm_son_eutran_neighbor_cell->plmn_id[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_TAC_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_eutran_neighbor_cell->tac); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_eutran_neighbor_cell->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CSG_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_eutran_neighbor_cell->csg_identity); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_eutran_neighbor_cell->csg_identity[loop], "csg_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_neighbor_cell->rsrp > 128))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_eutran_neighbor_cell->rsrp] should be in range [%d <= 128] !", p_rrm_son_eutran_neighbor_cell->rsrp);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_eutran_neighbor_cell->rsrp, "rsrp");
    *pp_buffer += sizeof(p_rrm_son_eutran_neighbor_cell->rsrp);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_neighbor_cell->rsrq > 34))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_eutran_neighbor_cell->rsrq] should be in range [%d <= 34] !", p_rrm_son_eutran_neighbor_cell->rsrq);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_eutran_neighbor_cell->rsrq, "rsrq");
    *pp_buffer += sizeof(p_rrm_son_eutran_neighbor_cell->rsrq);

    /* Optional element */
    if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CSG_MEM_STATUS_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_neighbor_cell->csg_membershipStatus > 2))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_eutran_neighbor_cell->csg_membershipStatus] should be in range [%d <= 2] !", p_rrm_son_eutran_neighbor_cell->csg_membershipStatus);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_eutran_neighbor_cell->csg_membershipStatus, "csg_membershipStatus");
    *pp_buffer += sizeof(p_rrm_son_eutran_neighbor_cell->csg_membershipStatus);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_eutran_meas_results_len
(
    rrm_son_eutran_meas_results_t *p_rrm_son_eutran_meas_results
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_eutran_meas_results != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eutran_meas_results->earfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eutran_meas_results->neighbor_cell_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_meas_results->neighbor_cell_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_eutran_meas_results->neighbor_cell_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_eutran_meas_results->neighbor_cell_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_eutran_meas_results->neighbor_cell_list_size; loop++)
        {
            length += rrm_get_rrm_son_eutran_neighbor_cell_len(&p_rrm_son_eutran_meas_results->neighbor_cell_list[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_eutran_meas_results
(
    U8  **pp_buffer,
    rrm_son_eutran_meas_results_t *p_rrm_son_eutran_meas_results
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_eutran_meas_results != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_eutran_meas_results->earfcn, "earfcn");
    *pp_buffer += sizeof(p_rrm_son_eutran_meas_results->earfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_meas_results->neighbor_cell_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_eutran_meas_results->neighbor_cell_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_eutran_meas_results->neighbor_cell_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_eutran_meas_results->neighbor_cell_list_size, "neighbor_cell_list_size");
    *pp_buffer += sizeof(p_rrm_son_eutran_meas_results->neighbor_cell_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_eutran_meas_results->neighbor_cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_eutran_neighbor_cell(pp_buffer, &p_rrm_son_eutran_meas_results->neighbor_cell_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_utran_physical_cell_id_len
(
    rrm_son_utran_physical_cell_id_t *p_rrm_son_utran_physical_cell_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_utran_physical_cell_id != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_physical_cell_id->bitmask);

    /* Optional element */
    if(p_rrm_son_utran_physical_cell_id->bitmask & RRM_PCI_FDD_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_physical_cell_id->pci_fdd);
    }

    /* Optional element */
    if(p_rrm_son_utran_physical_cell_id->bitmask & RRM_PCI_TDD_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_physical_cell_id->pci_tdd);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_utran_physical_cell_id
(
    U8  **pp_buffer,
    rrm_son_utran_physical_cell_id_t *p_rrm_son_utran_physical_cell_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_utran_physical_cell_id != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_utran_physical_cell_id->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_utran_physical_cell_id->bitmask);

    /* Optional element */
    if(p_rrm_son_utran_physical_cell_id->bitmask & RRM_PCI_FDD_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_physical_cell_id->pci_fdd > 511))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_physical_cell_id->pci_fdd] should be in range [%d <= 511] !", p_rrm_son_utran_physical_cell_id->pci_fdd);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_utran_physical_cell_id->pci_fdd, "pci_fdd");
    *pp_buffer += sizeof(p_rrm_son_utran_physical_cell_id->pci_fdd);
    }

    /* Optional element */
    if(p_rrm_son_utran_physical_cell_id->bitmask & RRM_PCI_TDD_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_physical_cell_id->pci_tdd > 127))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_physical_cell_id->pci_tdd] should be in range [%d <= 127] !", p_rrm_son_utran_physical_cell_id->pci_tdd);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_utran_physical_cell_id->pci_tdd, "pci_tdd");
    *pp_buffer += sizeof(p_rrm_son_utran_physical_cell_id->pci_tdd);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_utran_geran_global_cell_id_len
(
    rrm_son_utran_geran_global_cell_id_t *p_rrm_son_utran_geran_global_cell_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_utran_geran_global_cell_id != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_geran_global_cell_id->rat_type);

    /* Get length of IE */
    length += rrm_get_rrm_oam_cell_plmn_info_len(&p_rrm_son_utran_geran_global_cell_id->plmn_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_son_utran_geran_global_cell_id->cell_identity);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_utran_geran_global_cell_id
(
    U8  **pp_buffer,
    rrm_son_utran_geran_global_cell_id_t *p_rrm_son_utran_geran_global_cell_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_utran_geran_global_cell_id != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_utran_geran_global_cell_id->rat_type, "rat_type");
    *pp_buffer += sizeof(p_rrm_son_utran_geran_global_cell_id->rat_type);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_cell_plmn_info(pp_buffer, &p_rrm_son_utran_geran_global_cell_id->plmn_id))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_utran_geran_global_cell_id->cell_identity); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_utran_geran_global_cell_id->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_utran_neighbor_cell_len
(
    rrm_son_utran_neighbor_cell_t *p_rrm_son_utran_neighbor_cell
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_utran_neighbor_cell != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_neighbor_cell->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_son_utran_physical_cell_id_len(&p_rrm_son_utran_neighbor_cell->pci);

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_CGI_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_utran_geran_global_cell_id_len(&p_rrm_son_utran_neighbor_cell->cgi);
    }

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_LAC_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_son_utran_neighbor_cell->lac);
    }

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_RAC_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_neighbor_cell->rac);
    }

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_PLMN_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_neighbor_cell->num_plmn_id);
    }

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_PLMN_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_utran_neighbor_cell->num_plmn_id < 1) || (p_rrm_son_utran_neighbor_cell->num_plmn_id > MAX_PLMN_ID_LIST2))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_neighbor_cell->num_plmn_id] should be in range [1 <= %d <= MAX_PLMN_ID_LIST2] !", p_rrm_son_utran_neighbor_cell->num_plmn_id);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_utran_neighbor_cell->num_plmn_id; loop++)
        {
            length += rrm_get_rrm_oam_cell_plmn_info_len(&p_rrm_son_utran_neighbor_cell->plmn_id[loop]);
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_CSG_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_son_utran_neighbor_cell->csg_identity);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_neighbor_cell->rscp);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_neighbor_cell->ecno);

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_CSG_MEM_STATUS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_neighbor_cell->csg_membershipStatus);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_utran_neighbor_cell
(
    U8  **pp_buffer,
    rrm_son_utran_neighbor_cell_t *p_rrm_son_utran_neighbor_cell
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_utran_neighbor_cell != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_utran_neighbor_cell->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_utran_neighbor_cell->bitmask);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_utran_physical_cell_id(pp_buffer, &p_rrm_son_utran_neighbor_cell->pci))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_CGI_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_utran_geran_global_cell_id(pp_buffer, &p_rrm_son_utran_neighbor_cell->cgi))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_LAC_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_utran_neighbor_cell->lac); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_utran_neighbor_cell->lac[loop], "lac[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_RAC_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_utran_neighbor_cell->rac, "rac");
    *pp_buffer += sizeof(p_rrm_son_utran_neighbor_cell->rac);
    }

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_PLMN_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_utran_neighbor_cell->num_plmn_id < 1) || (p_rrm_son_utran_neighbor_cell->num_plmn_id > MAX_PLMN_ID_LIST2))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_neighbor_cell->num_plmn_id] should be in range [1 <= %d <= MAX_PLMN_ID_LIST2] !", p_rrm_son_utran_neighbor_cell->num_plmn_id);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_utran_neighbor_cell->num_plmn_id, "num_plmn_id");
    *pp_buffer += sizeof(p_rrm_son_utran_neighbor_cell->num_plmn_id);
    }

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_PLMN_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_utran_neighbor_cell->num_plmn_id; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_oam_cell_plmn_info(pp_buffer, &p_rrm_son_utran_neighbor_cell->plmn_id[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_CSG_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_utran_neighbor_cell->csg_identity); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_utran_neighbor_cell->csg_identity[loop], "csg_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_utran_neighbor_cell->rscp < -5) || (p_rrm_son_utran_neighbor_cell->rscp > 91))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_neighbor_cell->rscp] should be in range [-5 <= %d <= 91] !", p_rrm_son_utran_neighbor_cell->rscp);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_utran_neighbor_cell->rscp, "rscp");
    *pp_buffer += sizeof(p_rrm_son_utran_neighbor_cell->rscp);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_neighbor_cell->ecno > 49))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_neighbor_cell->ecno] should be in range [%d <= 49] !", p_rrm_son_utran_neighbor_cell->ecno);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_utran_neighbor_cell->ecno, "ecno");
    *pp_buffer += sizeof(p_rrm_son_utran_neighbor_cell->ecno);

    /* Optional element */
    if(p_rrm_son_utran_neighbor_cell->bitmask & RRM_SON_UTRAN_NC_CSG_MEM_STATUS_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_neighbor_cell->csg_membershipStatus > 0))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_neighbor_cell->csg_membershipStatus] should be in range [%d <= 0] !", p_rrm_son_utran_neighbor_cell->csg_membershipStatus);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_utran_neighbor_cell->csg_membershipStatus, "csg_membershipStatus");
    *pp_buffer += sizeof(p_rrm_son_utran_neighbor_cell->csg_membershipStatus);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_utran_meas_results_len
(
    rrm_son_utran_meas_results_t *p_rrm_son_utran_meas_results
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_utran_meas_results != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_meas_results->uarfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_meas_results->neighbor_cell_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_meas_results->neighbor_cell_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_meas_results->neighbor_cell_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_utran_meas_results->neighbor_cell_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_utran_meas_results->neighbor_cell_list_size; loop++)
        {
            length += rrm_get_rrm_son_utran_neighbor_cell_len(&p_rrm_son_utran_meas_results->neighbor_cell_list[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_utran_meas_results
(
    U8  **pp_buffer,
    rrm_son_utran_meas_results_t *p_rrm_son_utran_meas_results
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_utran_meas_results != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_meas_results->uarfcn > 16383))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_meas_results->uarfcn] should be in range [%d <= 16383] !", p_rrm_son_utran_meas_results->uarfcn);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_utran_meas_results->uarfcn, "uarfcn");
    *pp_buffer += sizeof(p_rrm_son_utran_meas_results->uarfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_meas_results->neighbor_cell_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_meas_results->neighbor_cell_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_utran_meas_results->neighbor_cell_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_utran_meas_results->neighbor_cell_list_size, "neighbor_cell_list_size");
    *pp_buffer += sizeof(p_rrm_son_utran_meas_results->neighbor_cell_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_utran_meas_results->neighbor_cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_utran_neighbor_cell(pp_buffer, &p_rrm_son_utran_meas_results->neighbor_cell_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_geran_neighbor_cell_len
(
    rrm_son_geran_neighbor_cell_t *p_rrm_son_geran_neighbor_cell
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_geran_neighbor_cell != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_neighbor_cell->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_neighbor_cell->arfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_neighbor_cell->band_ind);

    /* Get length of IE */
    length += rrm_get_rrm_son_geran_physical_cell_id_len(&p_rrm_son_geran_neighbor_cell->pci);

    /* Optional element */
    if(p_rrm_son_geran_neighbor_cell->bitmask & RRM_SON_GERAN_MR_IND_CGI_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_utran_geran_global_cell_id_len(&p_rrm_son_geran_neighbor_cell->cgi);
    }

    /* Optional element */
    if(p_rrm_son_geran_neighbor_cell->bitmask & RRM_SON_GERAN_MR_IND_RAC_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_neighbor_cell->rac);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_neighbor_cell->rssi);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_geran_neighbor_cell
(
    U8  **pp_buffer,
    rrm_son_geran_neighbor_cell_t *p_rrm_son_geran_neighbor_cell
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_geran_neighbor_cell != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_geran_neighbor_cell->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_geran_neighbor_cell->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_neighbor_cell->arfcn > 1023))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_neighbor_cell->arfcn] should be in range [%d <= 1023] !", p_rrm_son_geran_neighbor_cell->arfcn);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_geran_neighbor_cell->arfcn, "arfcn");
    *pp_buffer += sizeof(p_rrm_son_geran_neighbor_cell->arfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_neighbor_cell->band_ind > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_neighbor_cell->band_ind] should be in range [%d <= 1] !", p_rrm_son_geran_neighbor_cell->band_ind);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_geran_neighbor_cell->band_ind, "band_ind");
    *pp_buffer += sizeof(p_rrm_son_geran_neighbor_cell->band_ind);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_geran_physical_cell_id(pp_buffer, &p_rrm_son_geran_neighbor_cell->pci))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_son_geran_neighbor_cell->bitmask & RRM_SON_GERAN_MR_IND_CGI_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_utran_geran_global_cell_id(pp_buffer, &p_rrm_son_geran_neighbor_cell->cgi))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_geran_neighbor_cell->bitmask & RRM_SON_GERAN_MR_IND_RAC_PRESENT)
    {

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_neighbor_cell->rac, "rac");
    *pp_buffer += sizeof(p_rrm_son_geran_neighbor_cell->rac);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_neighbor_cell->rssi > 63))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_neighbor_cell->rssi] should be in range [%d <= 63] !", p_rrm_son_geran_neighbor_cell->rssi);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_neighbor_cell->rssi, "rssi");
    *pp_buffer += sizeof(p_rrm_son_geran_neighbor_cell->rssi);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_geran_meas_results_len
(
    rrm_son_geran_meas_results_t *p_rrm_son_geran_meas_results
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_geran_meas_results != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_meas_results->bitmask);

    /* Optional element */
    if(p_rrm_son_geran_meas_results->bitmask & RRM_SON_GERAN_MR_MEAS_OBJ_ID_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_meas_results->meas_obj_id);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_meas_results->neighbor_cell_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_meas_results->neighbor_cell_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_meas_results->neighbor_cell_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_geran_meas_results->neighbor_cell_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_geran_meas_results->neighbor_cell_list_size; loop++)
        {
            length += rrm_get_rrm_son_geran_neighbor_cell_len(&p_rrm_son_geran_meas_results->neighbor_cell_list[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_geran_meas_results
(
    U8  **pp_buffer,
    rrm_son_geran_meas_results_t *p_rrm_son_geran_meas_results
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_geran_meas_results != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_geran_meas_results->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_geran_meas_results->bitmask);

    /* Optional element */
    if(p_rrm_son_geran_meas_results->bitmask & RRM_SON_GERAN_MR_MEAS_OBJ_ID_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_geran_meas_results->meas_obj_id < 1) || (p_rrm_son_geran_meas_results->meas_obj_id > 12))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_meas_results->meas_obj_id] should be in range [1 <= %d <= 12] !", p_rrm_son_geran_meas_results->meas_obj_id);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_meas_results->meas_obj_id, "meas_obj_id");
    *pp_buffer += sizeof(p_rrm_son_geran_meas_results->meas_obj_id);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_meas_results->neighbor_cell_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_meas_results->neighbor_cell_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_geran_meas_results->neighbor_cell_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_meas_results->neighbor_cell_list_size, "neighbor_cell_list_size");
    *pp_buffer += sizeof(p_rrm_son_geran_meas_results->neighbor_cell_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_geran_meas_results->neighbor_cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_geran_neighbor_cell(pp_buffer, &p_rrm_son_geran_meas_results->neighbor_cell_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_cdma_global_cell_id_len
(
    rrm_son_cdma_global_cell_id_t *p_rrm_son_cdma_global_cell_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cdma_global_cell_id != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_global_cell_id->bitmask);

    /* Optional element */
    if(p_rrm_son_cdma_global_cell_id->bitmask & RRM_SON_CGI_1X_RTT_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_son_cdma_global_cell_id->cell_global_id_1X_RTT);
    }

    /* Optional element */
    if(p_rrm_son_cdma_global_cell_id->bitmask & RRM_SON_CGI_HRPD_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_son_cdma_global_cell_id->cell_global_id_HRPD);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_cdma_global_cell_id
(
    U8  **pp_buffer,
    rrm_son_cdma_global_cell_id_t *p_rrm_son_cdma_global_cell_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cdma_global_cell_id != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_global_cell_id->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_cdma_global_cell_id->bitmask);

    /* Optional element */
    if(p_rrm_son_cdma_global_cell_id->bitmask & RRM_SON_CGI_1X_RTT_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_cdma_global_cell_id->cell_global_id_1X_RTT); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_global_cell_id->cell_global_id_1X_RTT[loop], "cell_global_id_1X_RTT[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_rrm_son_cdma_global_cell_id->bitmask & RRM_SON_CGI_HRPD_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_cdma_global_cell_id->cell_global_id_HRPD); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_global_cell_id->cell_global_id_HRPD[loop], "cell_global_id_HRPD[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_cdma_neighbor_cell_len
(
    rrm_son_cdma_neighbor_cell_t *p_rrm_son_cdma_neighbor_cell
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cdma_neighbor_cell != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_neighbor_cell->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_neighbor_cell->arfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_neighbor_cell->band_class);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_neighbor_cell->pci);

    /* Optional element */
    if(p_rrm_son_cdma_neighbor_cell->bitmask & RRM_SON_CDMA_MEAS_IND_CGI_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_cdma_global_cell_id_len(&p_rrm_son_cdma_neighbor_cell->cgi);
    }

    /* Optional element */
    if(p_rrm_son_cdma_neighbor_cell->bitmask & RRM_SON_MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_neighbor_cell->pilot_pn_phase);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_neighbor_cell->pilot_strength);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_cdma_neighbor_cell
(
    U8  **pp_buffer,
    rrm_son_cdma_neighbor_cell_t *p_rrm_son_cdma_neighbor_cell
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cdma_neighbor_cell != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_neighbor_cell->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_cdma_neighbor_cell->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_neighbor_cell->arfcn > 1023))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_neighbor_cell->arfcn] should be in range [%d <= 1023] !", p_rrm_son_cdma_neighbor_cell->arfcn);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_cdma_neighbor_cell->arfcn, "arfcn");
    *pp_buffer += sizeof(p_rrm_son_cdma_neighbor_cell->arfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_neighbor_cell->band_class > 31))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_neighbor_cell->band_class] should be in range [%d <= 31] !", p_rrm_son_cdma_neighbor_cell->band_class);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_neighbor_cell->band_class, "band_class");
    *pp_buffer += sizeof(p_rrm_son_cdma_neighbor_cell->band_class);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_cdma_neighbor_cell->pci, "pci");
    *pp_buffer += sizeof(p_rrm_son_cdma_neighbor_cell->pci);

    /* Optional element */
    if(p_rrm_son_cdma_neighbor_cell->bitmask & RRM_SON_CDMA_MEAS_IND_CGI_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_cdma_global_cell_id(pp_buffer, &p_rrm_son_cdma_neighbor_cell->cgi))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_cdma_neighbor_cell->bitmask & RRM_SON_MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_neighbor_cell->pilot_pn_phase > 32767))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_neighbor_cell->pilot_pn_phase] should be in range [%d <= 32767] !", p_rrm_son_cdma_neighbor_cell->pilot_pn_phase);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_cdma_neighbor_cell->pilot_pn_phase, "pilot_pn_phase");
    *pp_buffer += sizeof(p_rrm_son_cdma_neighbor_cell->pilot_pn_phase);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_neighbor_cell->pilot_strength > 63))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_neighbor_cell->pilot_strength] should be in range [%d <= 63] !", p_rrm_son_cdma_neighbor_cell->pilot_strength);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_neighbor_cell->pilot_strength, "pilot_strength");
    *pp_buffer += sizeof(p_rrm_son_cdma_neighbor_cell->pilot_strength);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_cdma_meas_results_len
(
    rrm_son_cdma_meas_results_t *p_rrm_son_cdma_meas_results
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cdma_meas_results != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_meas_results->bitmask);

    /* Optional element */
    if(p_rrm_son_cdma_meas_results->bitmask & RRM_SON_CDMA_MEAS_RESULT_OBJ_ID_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_meas_results->meas_obj_id);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cdma_meas_results->neighbor_cell_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_meas_results->neighbor_cell_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_meas_results->neighbor_cell_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_cdma_meas_results->neighbor_cell_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_cdma_meas_results->neighbor_cell_list_size; loop++)
        {
            length += rrm_get_rrm_son_cdma_neighbor_cell_len(&p_rrm_son_cdma_meas_results->neighbor_cell_list[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_cdma_meas_results
(
    U8  **pp_buffer,
    rrm_son_cdma_meas_results_t *p_rrm_son_cdma_meas_results
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cdma_meas_results != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cdma_meas_results->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_cdma_meas_results->bitmask);

    /* Optional element */
    if(p_rrm_son_cdma_meas_results->bitmask & RRM_SON_CDMA_MEAS_RESULT_OBJ_ID_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_cdma_meas_results->meas_obj_id < 1) || (p_rrm_son_cdma_meas_results->meas_obj_id > 12))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_meas_results->meas_obj_id] should be in range [1 <= %d <= 12] !", p_rrm_son_cdma_meas_results->meas_obj_id);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_meas_results->meas_obj_id, "meas_obj_id");
    *pp_buffer += sizeof(p_rrm_son_cdma_meas_results->meas_obj_id);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_cdma_meas_results->neighbor_cell_list_size > RRM_MAX_CELLS_REPORTED))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_cdma_meas_results->neighbor_cell_list_size] should be in range [%d <= RRM_MAX_CELLS_REPORTED] !", p_rrm_son_cdma_meas_results->neighbor_cell_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_cdma_meas_results->neighbor_cell_list_size, "neighbor_cell_list_size");
    *pp_buffer += sizeof(p_rrm_son_cdma_meas_results->neighbor_cell_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_cdma_meas_results->neighbor_cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_cdma_neighbor_cell(pp_buffer, &p_rrm_son_cdma_meas_results->neighbor_cell_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_ho_report_len
(
    rrm_son_ho_report_t *p_rrm_son_ho_report
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_ho_report != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ho_report->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_ho_report->src_cgi);

    /* Get length of IE */
    length += rrm_get_rrm_oam_global_cell_id_len(&p_rrm_son_ho_report->trgt_cgi);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ho_report->result);

    /* Optional element */
    if(p_rrm_son_ho_report->bitmask & HO_REPORT_RE_EST_CGI_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_global_cell_id_len(&p_rrm_son_ho_report->restablishment_cgi);
    }

    /* Optional element */
    if(p_rrm_son_ho_report->bitmask & HO_REPORT_SERVING_MEAS_STRENGTH_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_eutran_strength_len(&p_rrm_son_ho_report->serving_meas_strength);
    }

    /* Optional element */
    if(p_rrm_son_ho_report->bitmask & HO_REPORT_TARGET_MEAS_STRENGTH_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_meas_strength_len(&p_rrm_son_ho_report->target_meas_strength);
    }

    /* Optional element */
    if(p_rrm_son_ho_report->bitmask & HO_REPORT_OFFSET_ATTR_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_offset_attr_len(&p_rrm_son_ho_report->offset_attr);
    }

    return length;
}

rrm_return_et
rrm_compose_rrm_son_ho_report
(
    U8  **pp_buffer,
    rrm_son_ho_report_t *p_rrm_son_ho_report
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_ho_report != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_ho_report->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_ho_report->bitmask);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_ho_report->src_cgi))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_global_cell_id(pp_buffer, &p_rrm_son_ho_report->trgt_cgi))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ho_report->result > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_ho_report->result] should be in range [%d <= 1] !", p_rrm_son_ho_report->result);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_ho_report->result, "result");
    *pp_buffer += sizeof(p_rrm_son_ho_report->result);

    /* Optional element */
    if(p_rrm_son_ho_report->bitmask & HO_REPORT_RE_EST_CGI_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_global_cell_id(pp_buffer, &p_rrm_son_ho_report->restablishment_cgi))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_ho_report->bitmask & HO_REPORT_SERVING_MEAS_STRENGTH_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_eutran_strength(pp_buffer, &p_rrm_son_ho_report->serving_meas_strength))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_ho_report->bitmask & HO_REPORT_TARGET_MEAS_STRENGTH_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_meas_strength(pp_buffer, &p_rrm_son_ho_report->target_meas_strength))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_ho_report->bitmask & HO_REPORT_OFFSET_ATTR_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_offset_attr(pp_buffer, &p_rrm_son_ho_report->offset_attr))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_ho_report
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_HO_REPORT message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_ho_report
(
    rrm_son_ho_report_t  *p_rrm_son_ho_report,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_ho_report != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_ho_report_len(p_rrm_son_ho_report);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_HO_REPORT", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_HO_REPORT, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_HO_REPORT, msg_length,cell_index);

    /* Fill RRM_SON_HO_REPORT message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_ho_report(&p_son_msg, p_rrm_son_ho_report))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_utran_geran_global_cell_id_len
(
    rrm_utran_geran_global_cell_id_t *p_rrm_utran_geran_global_cell_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_utran_geran_global_cell_id != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_utran_geran_global_cell_id->rat_type);

    /* Get length of IE */
    length += rrm_get_rrm_oam_cell_plmn_info_len(&p_rrm_utran_geran_global_cell_id->plmn_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_utran_geran_global_cell_id->cell_identity);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_utran_geran_global_cell_id
(
    U8  **pp_buffer,
    rrm_utran_geran_global_cell_id_t *p_rrm_utran_geran_global_cell_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_utran_geran_global_cell_id != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_utran_geran_global_cell_id->rat_type, "rat_type");
    *pp_buffer += sizeof(p_rrm_utran_geran_global_cell_id->rat_type);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_cell_plmn_info(pp_buffer, &p_rrm_utran_geran_global_cell_id->plmn_id))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_utran_geran_global_cell_id->cell_identity); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_utran_geran_global_cell_id->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_others_global_cell_id_len
(
    rrm_others_global_cell_id_t *p_rrm_others_global_cell_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_others_global_cell_id != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_others_global_cell_id->bitmask);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_others_global_cell_id
(
    U8  **pp_buffer,
    rrm_others_global_cell_id_t *p_rrm_others_global_cell_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_others_global_cell_id != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_others_global_cell_id->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_others_global_cell_id->bitmask);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_inter_rat_global_cell_id_len
(
    rrm_inter_rat_global_cell_id_t *p_rrm_inter_rat_global_cell_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_inter_rat_global_cell_id != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_inter_rat_global_cell_id->bitmask);

    /* Optional element */
    if(p_rrm_inter_rat_global_cell_id->bitmask & RRM_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_utran_geran_global_cell_id_len(&p_rrm_inter_rat_global_cell_id->utran_geran_global_cell_id);
    }

    /* Optional element */
    if(p_rrm_inter_rat_global_cell_id->bitmask & RRM_OTHERS_GLOBAL_CELL_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_others_global_cell_id_len(&p_rrm_inter_rat_global_cell_id->others_global_cell_id);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_inter_rat_global_cell_id
(
    U8  **pp_buffer,
    rrm_inter_rat_global_cell_id_t *p_rrm_inter_rat_global_cell_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_inter_rat_global_cell_id != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_inter_rat_global_cell_id->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_inter_rat_global_cell_id->bitmask);

    /* Optional element */
    if(p_rrm_inter_rat_global_cell_id->bitmask & RRM_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_utran_geran_global_cell_id(pp_buffer, &p_rrm_inter_rat_global_cell_id->utran_geran_global_cell_id))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_inter_rat_global_cell_id->bitmask & RRM_OTHERS_GLOBAL_CELL_ID_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_others_global_cell_id(pp_buffer, &p_rrm_inter_rat_global_cell_id->others_global_cell_id))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_global_cell_id_len
(
    rrm_oam_global_cell_id_t *p_rrm_oam_global_cell_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_oam_global_cell_id != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_global_cell_id->bitmask);

    /* Optional element */
    if(p_rrm_oam_global_cell_id->bitmask & RRM_INTRA_RAT_GLOBAL_CELL_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_oam_global_cell_id->intra_rat_global_cell_id);
    }

    /* Optional element */
    if(p_rrm_oam_global_cell_id->bitmask & RRM_INTER_RAT_GLOBAL_CELL_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_inter_rat_global_cell_id_len(&p_rrm_oam_global_cell_id->inter_rat_global_cell_id);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_oam_global_cell_id
(
    U8  **pp_buffer,
    rrm_oam_global_cell_id_t *p_rrm_oam_global_cell_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_oam_global_cell_id != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_oam_global_cell_id->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_global_cell_id->bitmask);

    /* Optional element */
    if(p_rrm_oam_global_cell_id->bitmask & RRM_INTRA_RAT_GLOBAL_CELL_ID_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_oam_global_cell_id->intra_rat_global_cell_id))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_oam_global_cell_id->bitmask & RRM_INTER_RAT_GLOBAL_CELL_ID_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_inter_rat_global_cell_id(pp_buffer, &p_rrm_oam_global_cell_id->inter_rat_global_cell_id))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_offset_attr_len
(
    rrm_son_offset_attr_t *p_rrm_son_offset_attr
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_offset_attr != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_offset_attr->bitmask);

    /* Optional element */
    if(p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_OFC_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_offset_attr->ofs);
    }

    /* Optional element */
    if(p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_OCS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_offset_attr->ocs);
    }

    /* Optional element */
    if(p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_OFN_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_offset_attr->ofn);
    }

    /* Optional element */
    if(p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_OFF_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_offset_attr->off);
    }

    /* Optional element */
    if(p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_HYS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_offset_attr->hys);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_offset_attr
(
    U8  **pp_buffer,
    rrm_son_offset_attr_t *p_rrm_son_offset_attr
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_offset_attr != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_offset_attr->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_offset_attr->bitmask);

    /* Optional element */
    if(p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_OFC_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_offset_attr->ofs < -24) || (p_rrm_son_offset_attr->ofs > 24))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_offset_attr->ofs] should be in range [-24 <= %d <= 24] !", p_rrm_son_offset_attr->ofs);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_offset_attr->ofs, "ofs");
    *pp_buffer += sizeof(p_rrm_son_offset_attr->ofs);
    }

    /* Optional element */
    if(p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_OCS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_offset_attr->ocs < -24) || (p_rrm_son_offset_attr->ocs > 24))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_offset_attr->ocs] should be in range [-24 <= %d <= 24] !", p_rrm_son_offset_attr->ocs);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_offset_attr->ocs, "ocs");
    *pp_buffer += sizeof(p_rrm_son_offset_attr->ocs);
    }

    /* Optional element */
    if(p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_OFN_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_offset_attr->ofn < -24) || (p_rrm_son_offset_attr->ofn > 24))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_offset_attr->ofn] should be in range [-24 <= %d <= 24] !", p_rrm_son_offset_attr->ofn);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_offset_attr->ofn, "ofn");
    *pp_buffer += sizeof(p_rrm_son_offset_attr->ofn);
    }

    /* Optional element */
    if(p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_OFF_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_offset_attr->off < -24) || (p_rrm_son_offset_attr->off > 24))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_offset_attr->off] should be in range [-24 <= %d <= 24] !", p_rrm_son_offset_attr->off);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_offset_attr->off, "off");
    *pp_buffer += sizeof(p_rrm_son_offset_attr->off);
    }

    /* Optional element */
    if(p_rrm_son_offset_attr->bitmask & RRM_SON_OFFSET_ATTR_HYS_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_offset_attr->hys > 30))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_offset_attr->hys] should be in range [%d <= 30] !", p_rrm_son_offset_attr->hys);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_offset_attr->hys, "hys");
    *pp_buffer += sizeof(p_rrm_son_offset_attr->hys);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_eutran_strength_len
(
    rrm_son_eutran_strength_t *p_rrm_son_eutran_strength
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_eutran_strength != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eutran_strength->bitmask);

    /* Optional element */
    if(p_rrm_son_eutran_strength->bitmask & RRM_SON_EUTRAN_STRENGTH_RSRP_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eutran_strength->rsrp);
    }

    /* Optional element */
    if(p_rrm_son_eutran_strength->bitmask & RRM_SON_EUTRAN_STRENGTH_RSRQ_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eutran_strength->rsrq);
    }

    /* Optional element */
    if(p_rrm_son_eutran_strength->bitmask & RRM_SON_EUTRAN_STRENGTH_OFFSET_ATT_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_offset_attr_len(&p_rrm_son_eutran_strength->offset_attr);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_eutran_strength
(
    U8  **pp_buffer,
    rrm_son_eutran_strength_t *p_rrm_son_eutran_strength
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_eutran_strength != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_eutran_strength->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_eutran_strength->bitmask);

    /* Optional element */
    if(p_rrm_son_eutran_strength->bitmask & RRM_SON_EUTRAN_STRENGTH_RSRP_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_strength->rsrp > 128))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_eutran_strength->rsrp] should be in range [%d <= 128] !", p_rrm_son_eutran_strength->rsrp);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_eutran_strength->rsrp, "rsrp");
    *pp_buffer += sizeof(p_rrm_son_eutran_strength->rsrp);
    }

    /* Optional element */
    if(p_rrm_son_eutran_strength->bitmask & RRM_SON_EUTRAN_STRENGTH_RSRQ_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eutran_strength->rsrq > 34))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_eutran_strength->rsrq] should be in range [%d <= 34] !", p_rrm_son_eutran_strength->rsrq);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_eutran_strength->rsrq, "rsrq");
    *pp_buffer += sizeof(p_rrm_son_eutran_strength->rsrq);
    }

    /* Optional element */
    if(p_rrm_son_eutran_strength->bitmask & RRM_SON_EUTRAN_STRENGTH_OFFSET_ATT_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_offset_attr(pp_buffer, &p_rrm_son_eutran_strength->offset_attr))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_utran_strength_len
(
    rrm_son_utran_strength_t *p_rrm_son_utran_strength
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_utran_strength != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_strength->bitmask);

    /* Optional element */
    if(p_rrm_son_utran_strength->bitmask & RRM_SON_UTRAN_STRENGTH_RSCP_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_strength->rscp);
    }

    /* Optional element */
    if(p_rrm_son_utran_strength->bitmask & RRM_SON_UTRAN_STRENGTH_ECNO_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_utran_strength->ecno);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_utran_strength
(
    U8  **pp_buffer,
    rrm_son_utran_strength_t *p_rrm_son_utran_strength
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_utran_strength != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_utran_strength->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_utran_strength->bitmask);

    /* Optional element */
    if(p_rrm_son_utran_strength->bitmask & RRM_SON_UTRAN_STRENGTH_RSCP_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_utran_strength->rscp < -5) || (p_rrm_son_utran_strength->rscp > 91))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_strength->rscp] should be in range [-5 <= %d <= 91] !", p_rrm_son_utran_strength->rscp);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(*pp_buffer, &p_rrm_son_utran_strength->rscp, "rscp");
    *pp_buffer += sizeof(p_rrm_son_utran_strength->rscp);
    }

    /* Optional element */
    if(p_rrm_son_utran_strength->bitmask & RRM_SON_UTRAN_STRENGTH_ECNO_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_utran_strength->ecno > 49))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_utran_strength->ecno] should be in range [%d <= 49] !", p_rrm_son_utran_strength->ecno);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_utran_strength->ecno, "ecno");
    *pp_buffer += sizeof(p_rrm_son_utran_strength->ecno);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_geran_strength_len
(
    rrm_son_geran_strength_t *p_rrm_son_geran_strength
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_geran_strength != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_strength->bitmask);

    /* Optional element */
    if(p_rrm_son_geran_strength->bitmask & RRM_SON_UTRAN_STRENGTH_RSCP_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_geran_strength->rssi);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_geran_strength
(
    U8  **pp_buffer,
    rrm_son_geran_strength_t *p_rrm_son_geran_strength
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_geran_strength != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_geran_strength->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_geran_strength->bitmask);

    /* Optional element */
    if(p_rrm_son_geran_strength->bitmask & RRM_SON_UTRAN_STRENGTH_RSCP_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_geran_strength->rssi > 63))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_geran_strength->rssi] should be in range [%d <= 63] !", p_rrm_son_geran_strength->rssi);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_geran_strength->rssi, "rssi");
    *pp_buffer += sizeof(p_rrm_son_geran_strength->rssi);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_meas_strength_len
(
    rrm_son_meas_strength_t *p_rrm_son_meas_strength
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_meas_strength != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_meas_strength->bitmask);

    /* Optional element */
    if(p_rrm_son_meas_strength->bitmask & RRM_SON_EUTRAN_CELL_STRENGTH_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_eutran_strength_len(&p_rrm_son_meas_strength->eutran_cell_strength);
    }

    /* Optional element */
    if(p_rrm_son_meas_strength->bitmask & RRM_SON_UTRAN_CELL_STRENGTH_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_utran_strength_len(&p_rrm_son_meas_strength->utran_cell_strength);
    }

    /* Optional element */
    if(p_rrm_son_meas_strength->bitmask & RRM_SON_GERAN_CELL_STRENGTH_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_geran_strength_len(&p_rrm_son_meas_strength->geran_cell_strength);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_meas_strength
(
    U8  **pp_buffer,
    rrm_son_meas_strength_t *p_rrm_son_meas_strength
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_meas_strength != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_meas_strength->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_meas_strength->bitmask);

    /* Optional element */
    if(p_rrm_son_meas_strength->bitmask & RRM_SON_EUTRAN_CELL_STRENGTH_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_eutran_strength(pp_buffer, &p_rrm_son_meas_strength->eutran_cell_strength))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_strength->bitmask & RRM_SON_UTRAN_CELL_STRENGTH_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_utran_strength(pp_buffer, &p_rrm_son_meas_strength->utran_cell_strength))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_meas_strength->bitmask & RRM_SON_GERAN_CELL_STRENGTH_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_geran_strength(pp_buffer, &p_rrm_son_meas_strength->geran_cell_strength))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_load_report_ind_len
(
    rrm_son_load_report_ind_t *p_rrm_son_load_report_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_load_report_ind != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_cell_load_info_len(&p_rrm_son_load_report_ind->serv_cell_load_info);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_load_report_ind->count);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_load_report_ind->count > RRM_MAX_NUM_NGBR_CELLS))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_load_report_ind->count] should be in range [%d <= RRM_MAX_NUM_NGBR_CELLS] !", p_rrm_son_load_report_ind->count);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_load_report_ind->count; loop++)
        {
            length += rrm_get_rrm_son_cell_load_info_len(&p_rrm_son_load_report_ind->neigh_cell_load_info[loop]);
        }
    }

    return length;
}

rrm_return_et
rrm_compose_rrm_son_load_report_ind
(
    U8  **pp_buffer,
    rrm_son_load_report_ind_t *p_rrm_son_load_report_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_load_report_ind != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_cell_load_info(pp_buffer, &p_rrm_son_load_report_ind->serv_cell_load_info))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_load_report_ind->count > RRM_MAX_NUM_NGBR_CELLS))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_load_report_ind->count] should be in range [%d <= RRM_MAX_NUM_NGBR_CELLS] !", p_rrm_son_load_report_ind->count);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_load_report_ind->count, "count");
    *pp_buffer += sizeof(p_rrm_son_load_report_ind->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_load_report_ind->count; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_son_cell_load_info(pp_buffer, &p_rrm_son_load_report_ind->neigh_cell_load_info[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_load_report_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_LOAD_REPORT_IND message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_load_report_ind
(
    rrm_son_load_report_ind_t  *p_rrm_son_load_report_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_load_report_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_load_report_ind_len(p_rrm_son_load_report_ind);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_LOAD_REPORT_IND", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_LOAD_REPORT_IND, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_LOAD_REPORT_IND, msg_length,cell_index);

    /* Fill RRM_SON_LOAD_REPORT_IND message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_load_report_ind(&p_son_msg, p_rrm_son_load_report_ind))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_hw_load_ind_len
(
    rrm_son_hw_load_ind_t *p_rrm_son_hw_load_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_hw_load_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_hw_load_ind->dl);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_hw_load_ind->ul);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_hw_load_ind
(
    U8  **pp_buffer,
    rrm_son_hw_load_ind_t *p_rrm_son_hw_load_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_hw_load_ind != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_hw_load_ind->dl, "dl");
    *pp_buffer += sizeof(p_rrm_son_hw_load_ind->dl);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_hw_load_ind->ul, "ul");
    *pp_buffer += sizeof(p_rrm_son_hw_load_ind->ul);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_s1_tnl_load_ind_len
(
    rrm_son_s1_tnl_load_ind_t *p_rrm_son_s1_tnl_load_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_s1_tnl_load_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_s1_tnl_load_ind->dl);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_s1_tnl_load_ind->ul);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_s1_tnl_load_ind
(
    U8  **pp_buffer,
    rrm_son_s1_tnl_load_ind_t *p_rrm_son_s1_tnl_load_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_s1_tnl_load_ind != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_s1_tnl_load_ind->dl, "dl");
    *pp_buffer += sizeof(p_rrm_son_s1_tnl_load_ind->dl);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_s1_tnl_load_ind->ul, "ul");
    *pp_buffer += sizeof(p_rrm_son_s1_tnl_load_ind->ul);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_rrs_load_ind_len
(
    rrm_son_rrs_load_ind_t *p_rrm_son_rrs_load_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_rrs_load_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rrs_load_ind->dl_gbr_prb_usage);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rrs_load_ind->ul_gbr_prb_usage);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rrs_load_ind->dl_non_gbr_prb_usage);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rrs_load_ind->ul_non_gbr_prb_usage);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rrs_load_ind->dl_total_prb_usage);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rrs_load_ind->ul_total_prb_usage);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_rrs_load_ind
(
    U8  **pp_buffer,
    rrm_son_rrs_load_ind_t *p_rrm_son_rrs_load_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_rrs_load_ind != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rrs_load_ind->dl_gbr_prb_usage > 100))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_rrs_load_ind->dl_gbr_prb_usage] should be in range [%d <= 100] !", p_rrm_son_rrs_load_ind->dl_gbr_prb_usage);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_rrs_load_ind->dl_gbr_prb_usage, "dl_gbr_prb_usage");
    *pp_buffer += sizeof(p_rrm_son_rrs_load_ind->dl_gbr_prb_usage);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rrs_load_ind->ul_gbr_prb_usage > 100))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_rrs_load_ind->ul_gbr_prb_usage] should be in range [%d <= 100] !", p_rrm_son_rrs_load_ind->ul_gbr_prb_usage);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_rrs_load_ind->ul_gbr_prb_usage, "ul_gbr_prb_usage");
    *pp_buffer += sizeof(p_rrm_son_rrs_load_ind->ul_gbr_prb_usage);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rrs_load_ind->dl_non_gbr_prb_usage > 100))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_rrs_load_ind->dl_non_gbr_prb_usage] should be in range [%d <= 100] !", p_rrm_son_rrs_load_ind->dl_non_gbr_prb_usage);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_rrs_load_ind->dl_non_gbr_prb_usage, "dl_non_gbr_prb_usage");
    *pp_buffer += sizeof(p_rrm_son_rrs_load_ind->dl_non_gbr_prb_usage);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rrs_load_ind->ul_non_gbr_prb_usage > 100))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_rrs_load_ind->ul_non_gbr_prb_usage] should be in range [%d <= 100] !", p_rrm_son_rrs_load_ind->ul_non_gbr_prb_usage);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_rrs_load_ind->ul_non_gbr_prb_usage, "ul_non_gbr_prb_usage");
    *pp_buffer += sizeof(p_rrm_son_rrs_load_ind->ul_non_gbr_prb_usage);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rrs_load_ind->dl_total_prb_usage > 100))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_rrs_load_ind->dl_total_prb_usage] should be in range [%d <= 100] !", p_rrm_son_rrs_load_ind->dl_total_prb_usage);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_rrs_load_ind->dl_total_prb_usage, "dl_total_prb_usage");
    *pp_buffer += sizeof(p_rrm_son_rrs_load_ind->dl_total_prb_usage);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rrs_load_ind->ul_total_prb_usage > 100))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_rrs_load_ind->ul_total_prb_usage] should be in range [%d <= 100] !", p_rrm_son_rrs_load_ind->ul_total_prb_usage);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_rrs_load_ind->ul_total_prb_usage, "ul_total_prb_usage");
    *pp_buffer += sizeof(p_rrm_son_rrs_load_ind->ul_total_prb_usage);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_comp_avl_cap_len
(
    rrm_son_comp_avl_cap_t *p_rrm_son_comp_avl_cap
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_comp_avl_cap != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_comp_avl_cap->bitmask);

    /* Optional element */
    if(p_rrm_son_comp_avl_cap->bitmask & RRM_SON_CELL_CAP_CLASS_VAL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_comp_avl_cap->cell_cap_class_val);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_comp_avl_cap->cap_val);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_comp_avl_cap
(
    U8  **pp_buffer,
    rrm_son_comp_avl_cap_t *p_rrm_son_comp_avl_cap
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_comp_avl_cap != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_comp_avl_cap->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_comp_avl_cap->bitmask);

    /* Optional element */
    if(p_rrm_son_comp_avl_cap->bitmask & RRM_SON_CELL_CAP_CLASS_VAL_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_comp_avl_cap->cell_cap_class_val < 1) || (p_rrm_son_comp_avl_cap->cell_cap_class_val > 100))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_comp_avl_cap->cell_cap_class_val] should be in range [1 <= %d <= 100] !", p_rrm_son_comp_avl_cap->cell_cap_class_val);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_comp_avl_cap->cell_cap_class_val, "cell_cap_class_val");
    *pp_buffer += sizeof(p_rrm_son_comp_avl_cap->cell_cap_class_val);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_comp_avl_cap->cap_val > 100))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_comp_avl_cap->cap_val] should be in range [%d <= 100] !", p_rrm_son_comp_avl_cap->cap_val);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_comp_avl_cap->cap_val, "cap_val");
    *pp_buffer += sizeof(p_rrm_son_comp_avl_cap->cap_val);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_comp_avl_cap_grp_len
(
    rrm_son_comp_avl_cap_grp_t *p_rrm_son_comp_avl_cap_grp
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_comp_avl_cap_grp != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_son_comp_avl_cap_len(&p_rrm_son_comp_avl_cap_grp->dl_comp_avl_cap);

    /* Get length of IE */
    length += rrm_get_rrm_son_comp_avl_cap_len(&p_rrm_son_comp_avl_cap_grp->ul_comp_avl_cap);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_comp_avl_cap_grp
(
    U8  **pp_buffer,
    rrm_son_comp_avl_cap_grp_t *p_rrm_son_comp_avl_cap_grp
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_comp_avl_cap_grp != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_comp_avl_cap(pp_buffer, &p_rrm_son_comp_avl_cap_grp->dl_comp_avl_cap))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_comp_avl_cap(pp_buffer, &p_rrm_son_comp_avl_cap_grp->ul_comp_avl_cap))
    {
        return RRM_FAILURE;
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_abs_status_len
(
    rrm_son_abs_status_t *p_rrm_son_abs_status
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_abs_status != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_abs_status->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_abs_status->dl_abs_status);

    /* Optional element */
    if(p_rrm_son_abs_status->bitmask & RRM_SON_TDD_FDD_CONFIG_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_abs_status->tdd_fdd_config);
    }
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_son_abs_status->usable_abs_pattern_info_tdd);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_abs_status
(
    U8  **pp_buffer,
    rrm_son_abs_status_t *p_rrm_son_abs_status
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_abs_status != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_abs_status->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_abs_status->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_abs_status->dl_abs_status > 100))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_abs_status->dl_abs_status] should be in range [%d <= 100] !", p_rrm_son_abs_status->dl_abs_status);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_abs_status->dl_abs_status, "dl_abs_status");
    *pp_buffer += sizeof(p_rrm_son_abs_status->dl_abs_status);

    /* Optional element */
    if(p_rrm_son_abs_status->bitmask & RRM_SON_TDD_FDD_CONFIG_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_abs_status->tdd_fdd_config > 3))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_abs_status->tdd_fdd_config] should be in range [%d <= 3] !", p_rrm_son_abs_status->tdd_fdd_config);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_abs_status->tdd_fdd_config, "tdd_fdd_config");
    *pp_buffer += sizeof(p_rrm_son_abs_status->tdd_fdd_config);
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_abs_status->usable_abs_pattern_info_tdd); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_abs_status->usable_abs_pattern_info_tdd[loop], "usable_abs_pattern_info_tdd[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_cell_load_info_len
(
    rrm_son_cell_load_info_t *p_rrm_son_cell_load_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_cell_load_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_cell_load_info->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_cell_load_info->g_cell_id);

    /* Optional element */
    if(p_rrm_son_cell_load_info->bitmask & RRM_SON_CELL_LOAD_INFO_HW_LOAD_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_hw_load_ind_len(&p_rrm_son_cell_load_info->hw_load);
    }

    /* Optional element */
    if(p_rrm_son_cell_load_info->bitmask & RRM_SON_CELL_LOAD_INFO_S1_TNL_LOAD_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_s1_tnl_load_ind_len(&p_rrm_son_cell_load_info->s1_tnl_load);
    }

    /* Optional element */
    if(p_rrm_son_cell_load_info->bitmask & RRM_SON_CELL_LOAD_INFO_RRS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_rrs_load_ind_len(&p_rrm_son_cell_load_info->rrs);
    }

    /* Optional element */
    if(p_rrm_son_cell_load_info->bitmask & RRM_SON_COMP_AVL_CAP_GRP_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_comp_avl_cap_grp_len(&p_rrm_son_cell_load_info->comp_avl_cap_grp);
    }

    /* Optional element */
    if(p_rrm_son_cell_load_info->bitmask & RRM_SON_ABS_STATUS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_abs_status_len(&p_rrm_son_cell_load_info->abs_status);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_cell_load_info
(
    U8  **pp_buffer,
    rrm_son_cell_load_info_t *p_rrm_son_cell_load_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_cell_load_info != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_cell_load_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_cell_load_info->bitmask);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_cell_load_info->g_cell_id))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_son_cell_load_info->bitmask & RRM_SON_CELL_LOAD_INFO_HW_LOAD_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_hw_load_ind(pp_buffer, &p_rrm_son_cell_load_info->hw_load))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_cell_load_info->bitmask & RRM_SON_CELL_LOAD_INFO_S1_TNL_LOAD_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_s1_tnl_load_ind(pp_buffer, &p_rrm_son_cell_load_info->s1_tnl_load))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_cell_load_info->bitmask & RRM_SON_CELL_LOAD_INFO_RRS_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_rrs_load_ind(pp_buffer, &p_rrm_son_cell_load_info->rrs))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_cell_load_info->bitmask & RRM_SON_COMP_AVL_CAP_GRP_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_comp_avl_cap_grp(pp_buffer, &p_rrm_son_cell_load_info->comp_avl_cap_grp))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_cell_load_info->bitmask & RRM_SON_ABS_STATUS_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_abs_status(pp_buffer, &p_rrm_son_cell_load_info->abs_status))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_ho_attempt_ind_len
(
    rrm_son_ho_attempt_ind_t *p_rrm_son_ho_attempt_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_ho_attempt_ind != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_ho_attempt_ind->src_cgi);

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_ho_attempt_ind->trgt_cgi);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ho_attempt_ind->attempt_count);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_ho_attempt_ind
(
    U8  **pp_buffer,
    rrm_son_ho_attempt_ind_t *p_rrm_son_ho_attempt_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_ho_attempt_ind != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_ho_attempt_ind->src_cgi))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_ho_attempt_ind->trgt_cgi))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_ho_attempt_ind->attempt_count, "attempt_count");
    *pp_buffer += sizeof(p_rrm_son_ho_attempt_ind->attempt_count);

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_ho_attempt_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_HO_ATTEMPT_IND message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_ho_attempt_ind
(
    rrm_son_ho_attempt_ind_t  *p_rrm_son_ho_attempt_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_ho_attempt_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_ho_attempt_ind_len(p_rrm_son_ho_attempt_ind);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_HO_ATTEMPT_IND", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_HO_ATTEMPT_IND, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_HO_ATTEMPT_IND, msg_length,cell_index);

    /* Fill RRM_SON_HO_ATTEMPT_IND message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_ho_attempt_ind(&p_son_msg, p_rrm_son_ho_attempt_ind))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_ho_failure_report_len
(
    rrm_son_ho_failure_report_t *p_rrm_son_ho_failure_report
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_ho_failure_report != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ho_failure_report->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_ho_failure_report->src_cgi);

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_ho_failure_report->trgt_cgi);

    /* Optional element */
    if(p_rrm_son_ho_failure_report->bitmask & HO_FAILURE_REPORT_RE_EST_CGI_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_ho_failure_report->restablishment_cgi);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_ho_failure_report->ho_cause);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_ho_failure_report
(
    U8  **pp_buffer,
    rrm_son_ho_failure_report_t *p_rrm_son_ho_failure_report
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_ho_failure_report != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_ho_failure_report->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_ho_failure_report->bitmask);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_ho_failure_report->src_cgi))
    {
        return RRM_FAILURE;
    }

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_ho_failure_report->trgt_cgi))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_son_ho_failure_report->bitmask & HO_FAILURE_REPORT_RE_EST_CGI_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_ho_failure_report->restablishment_cgi))
    {
        return RRM_FAILURE;
    }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_ho_failure_report->ho_cause > 2))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_ho_failure_report->ho_cause] should be in range [%d <= 2] !", p_rrm_son_ho_failure_report->ho_cause);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_ho_failure_report->ho_cause, "ho_cause");
    *pp_buffer += sizeof(p_rrm_son_ho_failure_report->ho_cause);

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_ho_failure_report
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_HO_FAILURE_REPORT message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_ho_failure_report
(
    rrm_son_ho_failure_report_t  *p_rrm_son_ho_failure_report,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_ho_failure_report != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_ho_failure_report_len(p_rrm_son_ho_failure_report);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_HO_FAILURE_REPORT", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_HO_FAILURE_REPORT, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_HO_FAILURE_REPORT, msg_length,cell_index);

    /* Fill RRM_SON_HO_FAILURE_REPORT message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_ho_failure_report(&p_son_msg, p_rrm_son_ho_failure_report))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_tnl_discovery_req_len
(
    rrm_son_tnl_discovery_req_t *p_rrm_son_tnl_discovery_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_tnl_discovery_req != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_global_enb_id_len(&p_rrm_son_tnl_discovery_req->target_enb_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_son_tnl_discovery_req->tac);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_tnl_discovery_req
(
    U8  **pp_buffer,
    rrm_son_tnl_discovery_req_t *p_rrm_son_tnl_discovery_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_tnl_discovery_req != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_global_enb_id(pp_buffer, &p_rrm_son_tnl_discovery_req->target_enb_id))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_tnl_discovery_req->tac); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_tnl_discovery_req->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_tnl_discovery_req
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_TNL_DISCOVERY_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_tnl_discovery_req
(
    rrm_son_tnl_discovery_req_t  *p_rrm_son_tnl_discovery_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_tnl_discovery_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_tnl_discovery_req_len(p_rrm_son_tnl_discovery_req);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_TNL_DISCOVERY_REQ", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_TNL_DISCOVERY_REQ, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_TNL_DISCOVERY_REQ, msg_length,cell_index);

    /* Fill RRM_SON_TNL_DISCOVERY_REQ message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_tnl_discovery_req(&p_son_msg, p_rrm_son_tnl_discovery_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_global_enb_id_len
(
    rrm_global_enb_id_t *p_rrm_global_enb_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_global_enb_id != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_global_enb_id->enb_type);

    /* Get length of IE */
    length += rrm_get_rrm_oam_cell_plmn_info_len(&p_rrm_global_enb_id->plmn_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_global_enb_id->eNb_id);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_global_enb_id
(
    U8  **pp_buffer,
    rrm_global_enb_id_t *p_rrm_global_enb_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_global_enb_id != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_global_enb_id->enb_type < 1) || (p_rrm_global_enb_id->enb_type > 2))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_global_enb_id->enb_type] should be in range [1 <= %d <= 2] !", p_rrm_global_enb_id->enb_type);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_global_enb_id->enb_type, "enb_type");
    *pp_buffer += sizeof(p_rrm_global_enb_id->enb_type);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_cell_plmn_info(pp_buffer, &p_rrm_global_enb_id->plmn_id))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_global_enb_id->eNb_id); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_global_enb_id->eNb_id[loop], "eNb_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_tnl_discovery_res_len
(
    rrm_son_tnl_discovery_res_t *p_rrm_son_tnl_discovery_res
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_tnl_discovery_res != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_tnl_discovery_res->result);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_tnl_discovery_res->error_code);

    /* Get length of IE */
    length += rrm_get_rrm_global_enb_id_len(&p_rrm_son_tnl_discovery_res->target_enb_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size > MAX_NO_ENB_X2_TLAS))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size] should be in range [%d <= MAX_NO_ENB_X2_TLAS] !", p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size; loop++)
        {
            length += rrm_get_rrm_enb_tnl_address_len(&p_rrm_son_tnl_discovery_res->enb_tnl_address_list[loop]);
        }
    }

    return length;
}

rrm_return_et
rrm_compose_rrm_son_tnl_discovery_res
(
    U8  **pp_buffer,
    rrm_son_tnl_discovery_res_t *p_rrm_son_tnl_discovery_res
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_tnl_discovery_res != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_tnl_discovery_res->result > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_tnl_discovery_res->result] should be in range [%d <= 1] !", p_rrm_son_tnl_discovery_res->result);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_tnl_discovery_res->result, "result");
    *pp_buffer += sizeof(p_rrm_son_tnl_discovery_res->result);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_tnl_discovery_res->error_code, "error_code");
    *pp_buffer += sizeof(p_rrm_son_tnl_discovery_res->error_code);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_global_enb_id(pp_buffer, &p_rrm_son_tnl_discovery_res->target_enb_id))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size > MAX_NO_ENB_X2_TLAS))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size] should be in range [%d <= MAX_NO_ENB_X2_TLAS] !", p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size, "enb_tnl_address_list_size");
    *pp_buffer += sizeof(p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_tnl_discovery_res->enb_tnl_address_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_enb_tnl_address(pp_buffer, &p_rrm_son_tnl_discovery_res->enb_tnl_address_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_tnl_discovery_res
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_TNL_DISCOVERY_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_tnl_discovery_res
(
    rrm_son_tnl_discovery_res_t  *p_rrm_son_tnl_discovery_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_tnl_discovery_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_tnl_discovery_res_len(p_rrm_son_tnl_discovery_res);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_TNL_DISCOVERY_RESP", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_TNL_DISCOVERY_RESP, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_TNL_DISCOVERY_RESP, msg_length,cell_index);

    /* Fill RRM_SON_TNL_DISCOVERY_RESP message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_tnl_discovery_res(&p_son_msg, p_rrm_son_tnl_discovery_res))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_enb_tnl_address_len
(
    rrm_enb_tnl_address_t *p_rrm_enb_tnl_address
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_enb_tnl_address != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_enb_tnl_address->bitmask);

    /* Optional element */
    if(p_rrm_enb_tnl_address->bitmask & RRM_IPV4_ADD_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_enb_tnl_address->ipv4_add);
    }

    /* Optional element */
    if(p_rrm_enb_tnl_address->bitmask & RRM_IPV6_ADD_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_enb_tnl_address->ipv6_add);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_enb_tnl_address
(
    U8  **pp_buffer,
    rrm_enb_tnl_address_t *p_rrm_enb_tnl_address
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_enb_tnl_address != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_enb_tnl_address->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_enb_tnl_address->bitmask);

    /* Optional element */
    if(p_rrm_enb_tnl_address->bitmask & RRM_IPV4_ADD_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_enb_tnl_address->ipv4_add); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_enb_tnl_address->ipv4_add[loop], "ipv4_add[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_rrm_enb_tnl_address->bitmask & RRM_IPV6_ADD_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_enb_tnl_address->ipv6_add); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_enb_tnl_address->ipv6_add[loop], "ipv6_add[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_rach_config_req_len
(
    rrm_son_rach_config_req_t *p_rrm_son_rach_config_req
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_rach_config_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rach_config_req->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_rach_config_req->cell_id);

    /* Optional element */
    if(p_rrm_son_rach_config_req->bitmask & L2_REPORT_PERIODICITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rach_config_req->l2_reports_periodicity);
    }

    /* Optional element */
    if(p_rrm_son_rach_config_req->bitmask & UE_REPORT_PERIODICITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rach_config_req->ue_reports_periodicity);
    }

    /* Optional element */
    if(p_rrm_son_rach_config_req->bitmask & RACH_SAMPLE_COUNT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rach_config_req->rach_sample_count);
    }

    /* Optional element */
    if(p_rrm_son_rach_config_req->bitmask & NUM_STRONGEST_CELLS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rach_config_req->num_of_strongest_cells);
    }

    return length;
}

rrm_return_et
rrm_compose_rrm_son_rach_config_req
(
    U8  **pp_buffer,
    rrm_son_rach_config_req_t *p_rrm_son_rach_config_req
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_rach_config_req != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_rach_config_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_rach_config_req->bitmask);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_rach_config_req->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_son_rach_config_req->bitmask & L2_REPORT_PERIODICITY_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rach_config_req->l2_reports_periodicity > 600))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_rach_config_req->l2_reports_periodicity] should be in range [%d <= 600] !", p_rrm_son_rach_config_req->l2_reports_periodicity);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_rach_config_req->l2_reports_periodicity, "l2_reports_periodicity");
    *pp_buffer += sizeof(p_rrm_son_rach_config_req->l2_reports_periodicity);
    }

    /* Optional element */
    if(p_rrm_son_rach_config_req->bitmask & UE_REPORT_PERIODICITY_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rach_config_req->ue_reports_periodicity > 600))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_rach_config_req->ue_reports_periodicity] should be in range [%d <= 600] !", p_rrm_son_rach_config_req->ue_reports_periodicity);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_rach_config_req->ue_reports_periodicity, "ue_reports_periodicity");
    *pp_buffer += sizeof(p_rrm_son_rach_config_req->ue_reports_periodicity);
    }

    /* Optional element */
    if(p_rrm_son_rach_config_req->bitmask & RACH_SAMPLE_COUNT_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rach_config_req->rach_sample_count > 1000))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_rach_config_req->rach_sample_count] should be in range [%d <= 1000] !", p_rrm_son_rach_config_req->rach_sample_count);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_rach_config_req->rach_sample_count, "rach_sample_count");
    *pp_buffer += sizeof(p_rrm_son_rach_config_req->rach_sample_count);
    }

    /* Optional element */
    if(p_rrm_son_rach_config_req->bitmask & NUM_STRONGEST_CELLS_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rach_config_req->num_of_strongest_cells > 3))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_rach_config_req->num_of_strongest_cells] should be in range [%d <= 3] !", p_rrm_son_rach_config_req->num_of_strongest_cells);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_rach_config_req->num_of_strongest_cells, "num_of_strongest_cells");
    *pp_buffer += sizeof(p_rrm_son_rach_config_req->num_of_strongest_cells);
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_rach_config_req
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_RACH_CONFIG_REQ message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_rach_config_req
(
    rrm_son_rach_config_req_t  *p_rrm_son_rach_config_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_rach_config_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_rach_config_req_len(p_rrm_son_rach_config_req);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_RACH_CONFIG_REQ", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_RACH_CONFIG_REQ, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_RACH_CONFIG_REQ, msg_length,cell_index);

    /* Fill RRM_SON_RACH_CONFIG_REQ message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_rach_config_req(&p_son_msg, p_rrm_son_rach_config_req))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_rach_config_resp_len
(
    rrm_son_rach_config_resp_t *p_rrm_son_rach_config_resp
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_rach_config_resp != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_rach_config_resp->cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rach_config_resp->result);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rach_config_resp->error_code);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_rach_config_resp
(
    U8  **pp_buffer,
    rrm_son_rach_config_resp_t *p_rrm_son_rach_config_resp
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_rach_config_resp != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_rach_config_resp->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_rach_config_resp->result, "result");
    *pp_buffer += sizeof(p_rrm_son_rach_config_resp->result);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_rach_config_resp->error_code, "error_code");
    *pp_buffer += sizeof(p_rrm_son_rach_config_resp->error_code);

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_rach_config_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_RACH_CONFIG_RESP message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_rach_config_resp
(
    rrm_son_rach_config_resp_t  *p_rrm_son_rach_config_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_rach_config_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_rach_config_resp_len(p_rrm_son_rach_config_resp);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_RACH_CONFIG_RESP", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_RACH_CONFIG_RESP, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_RACH_CONFIG_RESP, msg_length,cell_index);

    /* Fill RRM_SON_RACH_CONFIG_RESP message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_rach_config_resp(&p_son_msg, p_rrm_son_rach_config_resp))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_rach_ue_info_ind_len
(
    rrm_son_rach_ue_info_ind_t *p_rrm_son_rach_ue_info_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_rach_ue_info_ind != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_rach_ue_info_ind->cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_rach_ue_info_ind->count);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rach_ue_info_ind->count > MAX_NUMBER_OF_PRACH_SAMPLES))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_rach_ue_info_ind->count] should be in range [%d <= MAX_NUMBER_OF_PRACH_SAMPLES] !", p_rrm_son_rach_ue_info_ind->count);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_rach_ue_info_ind->count; loop++)
        {
            length += rrm_get_rrm_ue_prach_data_info_len(&p_rrm_son_rach_ue_info_ind->rach_data[loop]);
        }
    }

    return length;
}

rrm_return_et
rrm_compose_rrm_son_rach_ue_info_ind
(
    U8  **pp_buffer,
    rrm_son_rach_ue_info_ind_t *p_rrm_son_rach_ue_info_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_rach_ue_info_ind != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_rach_ue_info_ind->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_rach_ue_info_ind->count > MAX_NUMBER_OF_PRACH_SAMPLES))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_rach_ue_info_ind->count] should be in range [%d <= MAX_NUMBER_OF_PRACH_SAMPLES] !", p_rrm_son_rach_ue_info_ind->count);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_rach_ue_info_ind->count, "count");
    *pp_buffer += sizeof(p_rrm_son_rach_ue_info_ind->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_rach_ue_info_ind->count; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_ue_prach_data_info(pp_buffer, &p_rrm_son_rach_ue_info_ind->rach_data[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_rach_ue_info_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_RACH_UE_INFO_IND message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_rach_ue_info_ind
(
    rrm_son_rach_ue_info_ind_t  *p_rrm_son_rach_ue_info_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_rach_ue_info_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_rach_ue_info_ind_len(p_rrm_son_rach_ue_info_ind);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_RACH_UE_INFO_IND", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_RACH_UE_INFO_IND, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_RACH_UE_INFO_IND, msg_length,cell_index);

    /* Fill RRM_SON_RACH_UE_INFO_IND message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_rach_ue_info_ind(&p_son_msg, p_rrm_son_rach_ue_info_ind))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_strong_cell_info_len
(
    strong_cell_info_t *p_strong_cell_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_strong_cell_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_strong_cell_info->pci);
    /* Get length of parameter of basic type */
    length += sizeof(p_strong_cell_info->rsrp);

    return length;
}

static
rrm_return_et
rrm_compose_strong_cell_info
(
    U8  **pp_buffer,
    strong_cell_info_t *p_strong_cell_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_strong_cell_info != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_strong_cell_info->pci > 503))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_strong_cell_info->pci] should be in range [%d <= 503] !", p_strong_cell_info->pci);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_strong_cell_info->pci, "pci");
    *pp_buffer += sizeof(p_strong_cell_info->pci);

    /* Check for correct range [H - higher boundary] */
    if ((p_strong_cell_info->rsrp > 128))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_strong_cell_info->rsrp] should be in range [%d <= 128] !", p_strong_cell_info->rsrp);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_strong_cell_info->rsrp, "rsrp");
    *pp_buffer += sizeof(p_strong_cell_info->rsrp);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_ue_prach_data_info_len
(
    rrm_ue_prach_data_info_t *p_rrm_ue_prach_data_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_ue_prach_data_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_ue_prach_data_info->cause);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_ue_prach_data_info->number_of_preambles_sent);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_ue_prach_data_info->contention_detected);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_ue_prach_data_info->ue_location);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_ue_prach_data_info->strong_cell_info_count);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_ue_prach_data_info->strong_cell_info_count > STRONG_CELL_INFO_COUNT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_ue_prach_data_info->strong_cell_info_count] should be in range [%d <= STRONG_CELL_INFO_COUNT] !", p_rrm_ue_prach_data_info->strong_cell_info_count);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_ue_prach_data_info->strong_cell_info_count; loop++)
        {
            length += rrm_get_strong_cell_info_len(&p_rrm_ue_prach_data_info->strong_cell_info[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_ue_prach_data_info
(
    U8  **pp_buffer,
    rrm_ue_prach_data_info_t *p_rrm_ue_prach_data_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_ue_prach_data_info != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_ue_prach_data_info->cause < 1) || (p_rrm_ue_prach_data_info->cause > 3))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_ue_prach_data_info->cause] should be in range [1 <= %d <= 3] !", p_rrm_ue_prach_data_info->cause);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_ue_prach_data_info->cause, "cause");
    *pp_buffer += sizeof(p_rrm_ue_prach_data_info->cause);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_ue_prach_data_info->number_of_preambles_sent, "number_of_preambles_sent");
    *pp_buffer += sizeof(p_rrm_ue_prach_data_info->number_of_preambles_sent);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_ue_prach_data_info->contention_detected, "contention_detected");
    *pp_buffer += sizeof(p_rrm_ue_prach_data_info->contention_detected);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_ue_prach_data_info->ue_location > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_ue_prach_data_info->ue_location] should be in range [%d <= 1] !", p_rrm_ue_prach_data_info->ue_location);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_ue_prach_data_info->ue_location, "ue_location");
    *pp_buffer += sizeof(p_rrm_ue_prach_data_info->ue_location);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_ue_prach_data_info->strong_cell_info_count > STRONG_CELL_INFO_COUNT))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_ue_prach_data_info->strong_cell_info_count] should be in range [%d <= STRONG_CELL_INFO_COUNT] !", p_rrm_ue_prach_data_info->strong_cell_info_count);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_ue_prach_data_info->strong_cell_info_count, "strong_cell_info_count");
    *pp_buffer += sizeof(p_rrm_ue_prach_data_info->strong_cell_info_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_ue_prach_data_info->strong_cell_info_count; loop++)
        {
            if (RRM_FAILURE == rrm_compose_strong_cell_info(pp_buffer, &p_rrm_ue_prach_data_info->strong_cell_info[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_l2_rach_perf_report_len
(
    rrm_son_l2_rach_perf_report_t *p_rrm_son_l2_rach_perf_report
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_l2_rach_perf_report != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_l2_rach_perf_report->cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_l2_rach_perf_report->num_of_l2_perf_accumulated_reports);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_l2_rach_perf_report->total_rcvd_msg3_random_access_preambles_cbra);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_l2_rach_perf_report->total_rcvd_msg3_random_access_preambles_cfra);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_l2_rach_perf_report->total_failed_random_access_preambles_msg2_cbra);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_l2_rach_perf_report->total_failed_random_access_preambles_msg2_cfra);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_l2_rach_perf_report->total_allocated_random_access_preambles);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_son_l2_rach_perf_report->total_succ_random_access_ta);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_l2_rach_perf_report->total_unassigned_false_cfra);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_l2_rach_perf_report->total_unassigned_sum_cfra);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_l2_rach_perf_report->num_random_access_preambles_group_a);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_l2_rach_perf_report->num_random_access_preambles_group_b);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_l2_rach_perf_report
(
    U8  **pp_buffer,
    rrm_son_l2_rach_perf_report_t *p_rrm_son_l2_rach_perf_report
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_l2_rach_perf_report != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_l2_rach_perf_report->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(*pp_buffer, &p_rrm_son_l2_rach_perf_report->num_of_l2_perf_accumulated_reports, "num_of_l2_perf_accumulated_reports");
    *pp_buffer += sizeof(p_rrm_son_l2_rach_perf_report->num_of_l2_perf_accumulated_reports);

    /* Compose parameter of basic type */
    rrm_cp_pack_U64(*pp_buffer, &p_rrm_son_l2_rach_perf_report->total_rcvd_msg3_random_access_preambles_cbra, "total_rcvd_msg3_random_access_preambles_cbra");
    *pp_buffer += sizeof(p_rrm_son_l2_rach_perf_report->total_rcvd_msg3_random_access_preambles_cbra);

    /* Compose parameter of basic type */
    rrm_cp_pack_U64(*pp_buffer, &p_rrm_son_l2_rach_perf_report->total_rcvd_msg3_random_access_preambles_cfra, "total_rcvd_msg3_random_access_preambles_cfra");
    *pp_buffer += sizeof(p_rrm_son_l2_rach_perf_report->total_rcvd_msg3_random_access_preambles_cfra);

    /* Compose parameter of basic type */
    rrm_cp_pack_U64(*pp_buffer, &p_rrm_son_l2_rach_perf_report->total_failed_random_access_preambles_msg2_cbra, "total_failed_random_access_preambles_msg2_cbra");
    *pp_buffer += sizeof(p_rrm_son_l2_rach_perf_report->total_failed_random_access_preambles_msg2_cbra);

    /* Compose parameter of basic type */
    rrm_cp_pack_U64(*pp_buffer, &p_rrm_son_l2_rach_perf_report->total_failed_random_access_preambles_msg2_cfra, "total_failed_random_access_preambles_msg2_cfra");
    *pp_buffer += sizeof(p_rrm_son_l2_rach_perf_report->total_failed_random_access_preambles_msg2_cfra);

    /* Compose parameter of basic type */
    rrm_cp_pack_U64(*pp_buffer, &p_rrm_son_l2_rach_perf_report->total_allocated_random_access_preambles, "total_allocated_random_access_preambles");
    *pp_buffer += sizeof(p_rrm_son_l2_rach_perf_report->total_allocated_random_access_preambles);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_son_l2_rach_perf_report->total_succ_random_access_ta); loop++)
        {
            rrm_cp_pack_U64(*pp_buffer, &p_rrm_son_l2_rach_perf_report->total_succ_random_access_ta[loop], "total_succ_random_access_ta[]");
            *pp_buffer += sizeof(U64);
        }
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U64(*pp_buffer, &p_rrm_son_l2_rach_perf_report->total_unassigned_false_cfra, "total_unassigned_false_cfra");
    *pp_buffer += sizeof(p_rrm_son_l2_rach_perf_report->total_unassigned_false_cfra);

    /* Compose parameter of basic type */
    rrm_cp_pack_U64(*pp_buffer, &p_rrm_son_l2_rach_perf_report->total_unassigned_sum_cfra, "total_unassigned_sum_cfra");
    *pp_buffer += sizeof(p_rrm_son_l2_rach_perf_report->total_unassigned_sum_cfra);

    /* Compose parameter of basic type */
    rrm_cp_pack_U64(*pp_buffer, &p_rrm_son_l2_rach_perf_report->num_random_access_preambles_group_a, "num_random_access_preambles_group_a");
    *pp_buffer += sizeof(p_rrm_son_l2_rach_perf_report->num_random_access_preambles_group_a);

    /* Compose parameter of basic type */
    rrm_cp_pack_U64(*pp_buffer, &p_rrm_son_l2_rach_perf_report->num_random_access_preambles_group_b, "num_random_access_preambles_group_b");
    *pp_buffer += sizeof(p_rrm_son_l2_rach_perf_report->num_random_access_preambles_group_b);

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_l2_rach_perf_report
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_L2_RACH_PERF_REPORT message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_l2_rach_perf_report
(
    rrm_son_l2_rach_perf_report_t  *p_rrm_son_l2_rach_perf_report,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_l2_rach_perf_report != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_l2_rach_perf_report_len(p_rrm_son_l2_rach_perf_report);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_L2_RACH_PERF_REPORT", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_L2_RACH_PERF_REPORT, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_L2_RACH_PERF_REPORT, msg_length,cell_index);

    /* Fill RRM_SON_L2_RACH_PERF_REPORT message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_l2_rach_perf_report(&p_son_msg, p_rrm_son_l2_rach_perf_report))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_enb_config_update_ind_len
(
    rrm_son_enb_config_update_ind_t *p_rrm_son_enb_config_update_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_enb_config_update_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_enb_config_update_ind->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_global_enb_id_len(&p_rrm_son_enb_config_update_ind->nbr_enb_id);

    /* Optional element */
    if(p_rrm_son_enb_config_update_ind->bitmask & RRM_SON_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_gu_group_id_list_len(&p_rrm_son_enb_config_update_ind->add_gu_id);
    }

    /* Optional element */
    if(p_rrm_son_enb_config_update_ind->bitmask & RRM_SON_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_son_gu_group_id_list_len(&p_rrm_son_enb_config_update_ind->delete_gu_id);
    }

    return length;
}

rrm_return_et
rrm_compose_rrm_son_enb_config_update_ind
(
    U8  **pp_buffer,
    rrm_son_enb_config_update_ind_t *p_rrm_son_enb_config_update_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_enb_config_update_ind != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_enb_config_update_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_enb_config_update_ind->bitmask);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_global_enb_id(pp_buffer, &p_rrm_son_enb_config_update_ind->nbr_enb_id))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_son_enb_config_update_ind->bitmask & RRM_SON_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_gu_group_id_list(pp_buffer, &p_rrm_son_enb_config_update_ind->add_gu_id))
    {
        return RRM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_son_enb_config_update_ind->bitmask & RRM_SON_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_son_gu_group_id_list(pp_buffer, &p_rrm_son_enb_config_update_ind->delete_gu_id))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_enb_config_update_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_ENB_CONFIG_UPDATE_IND message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_enb_config_update_ind
(
    rrm_son_enb_config_update_ind_t  *p_rrm_son_enb_config_update_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_enb_config_update_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_enb_config_update_ind_len(p_rrm_son_enb_config_update_ind);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_ENB_CONFIG_UPDATE_IND", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_ENB_CONFIG_UPDATE_IND, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_ENB_CONFIG_UPDATE_IND, msg_length,cell_index);

    /* Fill RRM_SON_ENB_CONFIG_UPDATE_IND message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_enb_config_update_ind(&p_son_msg, p_rrm_son_enb_config_update_ind))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_gu_group_id_len
(
    rrm_oam_gu_group_id_t *p_rrm_oam_gu_group_id
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_oam_gu_group_id != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_cell_plmn_info_len(&p_rrm_oam_gu_group_id->plmn_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_oam_gu_group_id->mme_group_id);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_oam_gu_group_id
(
    U8  **pp_buffer,
    rrm_oam_gu_group_id_t *p_rrm_oam_gu_group_id
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_oam_gu_group_id != PNULL);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_cell_plmn_info(pp_buffer, &p_rrm_oam_gu_group_id->plmn_id))
    {
        return RRM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_gu_group_id->mme_group_id); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_oam_gu_group_id->mme_group_id[loop], "mme_group_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_son_gu_group_id_list_len
(
    rrm_son_gu_group_id_list_t *p_rrm_son_gu_group_id_list
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_gu_group_id_list != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_gu_group_id_list->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_gu_group_id_list->num_gu_group_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_gu_group_id_list->num_gu_group_id < 1) || (p_rrm_son_gu_group_id_list->num_gu_group_id > RRM_MME_MAX_POOLS))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_gu_group_id_list->num_gu_group_id] should be in range [1 <= %d <= RRM_MME_MAX_POOLS] !", p_rrm_son_gu_group_id_list->num_gu_group_id);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_gu_group_id_list->num_gu_group_id; loop++)
        {
            length += rrm_get_rrm_oam_gu_group_id_len(&p_rrm_son_gu_group_id_list->gu_id_list[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_son_gu_group_id_list
(
    U8  **pp_buffer,
    rrm_son_gu_group_id_list_t *p_rrm_son_gu_group_id_list
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_gu_group_id_list != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_gu_group_id_list->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_gu_group_id_list->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_gu_group_id_list->num_gu_group_id < 1) || (p_rrm_son_gu_group_id_list->num_gu_group_id > RRM_MME_MAX_POOLS))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_gu_group_id_list->num_gu_group_id] should be in range [1 <= %d <= RRM_MME_MAX_POOLS] !", p_rrm_son_gu_group_id_list->num_gu_group_id);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_gu_group_id_list->num_gu_group_id, "num_gu_group_id");
    *pp_buffer += sizeof(p_rrm_son_gu_group_id_list->num_gu_group_id);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_gu_group_id_list->num_gu_group_id; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_oam_gu_group_id(pp_buffer, &p_rrm_son_gu_group_id_list->gu_id_list[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_eicic_enable_disable_ind_len
(
    rrm_son_eicic_enable_disable_ind_t *p_rrm_son_eicic_enable_disable_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_eicic_enable_disable_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eicic_enable_disable_ind->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_son_eicic_enable_disable_ind->cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_eicic_enable_disable_ind->eicic_provisioned_type);

    return length;
}

rrm_return_et
rrm_compose_rrm_son_eicic_enable_disable_ind
(
    U8  **pp_buffer,
    rrm_son_eicic_enable_disable_ind_t *p_rrm_son_eicic_enable_disable_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_eicic_enable_disable_ind != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_eicic_enable_disable_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_son_eicic_enable_disable_ind->bitmask);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_son_eicic_enable_disable_ind->cell_id))
    {
        return RRM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_son_eicic_enable_disable_ind->eicic_provisioned_type > 3))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_eicic_enable_disable_ind->eicic_provisioned_type] should be in range [%d <= 3] !", p_rrm_son_eicic_enable_disable_ind->eicic_provisioned_type);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_son_eicic_enable_disable_ind->eicic_provisioned_type, "eicic_provisioned_type");
    *pp_buffer += sizeof(p_rrm_son_eicic_enable_disable_ind->eicic_provisioned_type);

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_eicic_enable_disable_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_EICIC_ENABLE_DISABLE_IND message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_eicic_enable_disable_ind
(
    rrm_son_eicic_enable_disable_ind_t  *p_rrm_son_eicic_enable_disable_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_eicic_enable_disable_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_eicic_enable_disable_ind_len(p_rrm_son_eicic_enable_disable_ind);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_EICIC_ENABLE_DISABLE_IND", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_EICIC_ENABLE_DISABLE_IND, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_EICIC_ENABLE_DISABLE_IND, msg_length,cell_index);

    /* Fill RRM_SON_EICIC_ENABLE_DISABLE_IND message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_eicic_enable_disable_ind(&p_son_msg, p_rrm_son_eicic_enable_disable_ind))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_son_mbms_sf_info_ind_len
(
    rrm_son_mbms_sf_info_ind_t *p_rrm_son_mbms_sf_info_ind
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_son_mbms_sf_info_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_son_mbms_sf_info_ind->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_mbms_sf_info_ind->count < 1) || (p_rrm_son_mbms_sf_info_ind->count > 8))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_mbms_sf_info_ind->count] should be in range [1 <= %d <= 8] !", p_rrm_son_mbms_sf_info_ind->count);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_mbms_sf_info_ind->count; loop++)
        {
            length += rrm_get_rrm_cell_sib2_mbsfn_data_len(&p_rrm_son_mbms_sf_info_ind->cell_sib2_mbsfn_data[loop]);
        }
    }

    return length;
}

rrm_return_et
rrm_compose_rrm_son_mbms_sf_info_ind
(
    U8  **pp_buffer,
    rrm_son_mbms_sf_info_ind_t *p_rrm_son_mbms_sf_info_ind
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_son_mbms_sf_info_ind != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_son_mbms_sf_info_ind->count < 1) || (p_rrm_son_mbms_sf_info_ind->count > 8))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_son_mbms_sf_info_ind->count] should be in range [1 <= %d <= 8] !", p_rrm_son_mbms_sf_info_ind->count);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_son_mbms_sf_info_ind->count, "count");
    *pp_buffer += sizeof(p_rrm_son_mbms_sf_info_ind->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_son_mbms_sf_info_ind->count; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_cell_sib2_mbsfn_data(pp_buffer, &p_rrm_son_mbms_sf_info_ind->cell_sib2_mbsfn_data[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrm_son_send_rrm_son_mbms_sf_info_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRM_SON_MBMS_SF_INFO_IND message
*
*   RETURNS:
*       RRM_FAILURE     - Indicates failed message processing
*       RRM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrm_return_et
rrm_son_send_rrm_son_mbms_sf_info_ind
(
    rrm_son_mbms_sf_info_ind_t  *p_rrm_son_mbms_sf_info_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_son_msg;

    RRM_ASSERT(p_rrm_son_mbms_sf_info_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_son_mbms_sf_info_ind_len(p_rrm_son_mbms_sf_info_ind);

    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_INFO, "Src(%u)->Dst(%u):RRM_SON_MBMS_SF_INFO_IND", src_module_id, dest_module_id);
	
	if(msg_length == RRM_FAILURE)
	{
 		return RRM_FAILURE;
	}	
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_msg, 0, msg_api_length);
    p_son_msg = p_msg;

    /* Fill CSPL header */
    rrm_construct_api_header(p_son_msg, RRM_VERSION_ID, src_module_id,
        dest_module_id, RRM_SON_MBMS_SF_INFO_IND, msg_api_length);

    /* Fill interface header */
    p_son_msg = p_son_msg + RRM_API_HEADER_SIZE;
    rrm_construct_interface_api_header(p_son_msg, transaction_id, src_module_id,
        dest_module_id, RRM_SON_MBMS_SF_INFO_IND, msg_length,cell_index);

    /* Fill RRM_SON_MBMS_SF_INFO_IND message */
    p_son_msg = p_son_msg + RRM_INTERFACE_API_HEADER_SIZE;
    if (RRM_FAILURE == rrm_compose_rrm_son_mbms_sf_info_ind(&p_son_msg, p_rrm_son_mbms_sf_info_ind))
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    if((p_son_msg - msg_api_length) != p_msg)
    {
        rrm_msg_mem_free(p_msg);

        return RRM_FAILURE;
    }

    /* Send message to */
    rrm_send_message(p_msg, dest_module_id);

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_subframe_allocation_len
(
    rrm_subframe_allocation_t *p_rrm_subframe_allocation
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_subframe_allocation != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_subframe_allocation->sf_alloc_choice);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_subframe_allocation->subframe_allocation);

    return length;
}

static
rrm_return_et
rrm_compose_rrm_subframe_allocation
(
    U8  **pp_buffer,
    rrm_subframe_allocation_t *p_rrm_subframe_allocation
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_subframe_allocation != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_subframe_allocation->sf_alloc_choice > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_subframe_allocation->sf_alloc_choice] should be in range [%d <= 1] !", p_rrm_subframe_allocation->sf_alloc_choice);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_subframe_allocation->sf_alloc_choice, "sf_alloc_choice");
    *pp_buffer += sizeof(p_rrm_subframe_allocation->sf_alloc_choice);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_subframe_allocation->subframe_allocation); loop++)
        {
            rrm_cp_pack_U8(*pp_buffer, &p_rrm_subframe_allocation->subframe_allocation[loop], "subframe_allocation[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_mbsfn_config_info_len
(
    rrm_mbsfn_config_info_t *p_rrm_mbsfn_config_info
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_mbsfn_config_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_mbsfn_config_info->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_mbsfn_config_info->radio_frame_allocation_period);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_mbsfn_config_info->radio_frame_allocation_offset);

    /* Get length of IE */
    length += rrm_get_rrm_subframe_allocation_len(&p_rrm_mbsfn_config_info->subframe_allocation);

    /* Optional element */
    if(p_rrm_mbsfn_config_info->bitmask & RRM_OAM_SYNC_STRATUM_ID_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_mbsfn_config_info->sync_stratum_id);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_mbsfn_config_info
(
    U8  **pp_buffer,
    rrm_mbsfn_config_info_t *p_rrm_mbsfn_config_info
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_mbsfn_config_info != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_mbsfn_config_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_mbsfn_config_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_mbsfn_config_info->radio_frame_allocation_period > 5))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_mbsfn_config_info->radio_frame_allocation_period] should be in range [%d <= 5] !", p_rrm_mbsfn_config_info->radio_frame_allocation_period);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_mbsfn_config_info->radio_frame_allocation_period, "radio_frame_allocation_period");
    *pp_buffer += sizeof(p_rrm_mbsfn_config_info->radio_frame_allocation_period);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_mbsfn_config_info->radio_frame_allocation_offset > 7))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_mbsfn_config_info->radio_frame_allocation_offset] should be in range [%d <= 7] !", p_rrm_mbsfn_config_info->radio_frame_allocation_offset);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_mbsfn_config_info->radio_frame_allocation_offset, "radio_frame_allocation_offset");
    *pp_buffer += sizeof(p_rrm_mbsfn_config_info->radio_frame_allocation_offset);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_subframe_allocation(pp_buffer, &p_rrm_mbsfn_config_info->subframe_allocation))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_mbsfn_config_info->bitmask & RRM_OAM_SYNC_STRATUM_ID_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_mbsfn_config_info->sync_stratum_id < 1) || (p_rrm_mbsfn_config_info->sync_stratum_id > 8))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_mbsfn_config_info->sync_stratum_id] should be in range [1 <= %d <= 8] !", p_rrm_mbsfn_config_info->sync_stratum_id);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_mbsfn_config_info->sync_stratum_id, "sync_stratum_id");
    *pp_buffer += sizeof(p_rrm_mbsfn_config_info->sync_stratum_id);
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_mbsfn_subframe_config_list_len
(
    rrm_mbsfn_subframe_config_list_t *p_rrm_mbsfn_subframe_config_list
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_mbsfn_subframe_config_list != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_mbsfn_subframe_config_list->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_mbsfn_subframe_config_list->count < 1) || (p_rrm_mbsfn_subframe_config_list->count > 8))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_mbsfn_subframe_config_list->count] should be in range [1 <= %d <= 8] !", p_rrm_mbsfn_subframe_config_list->count);
#endif
        return RRM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_mbsfn_subframe_config_list->count; loop++)
        {
            length += rrm_get_rrm_mbsfn_config_info_len(&p_rrm_mbsfn_subframe_config_list->mbsfn_subframe_config_info[loop]);
        }
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_mbsfn_subframe_config_list
(
    U8  **pp_buffer,
    rrm_mbsfn_subframe_config_list_t *p_rrm_mbsfn_subframe_config_list
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_mbsfn_subframe_config_list != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_mbsfn_subframe_config_list->count < 1) || (p_rrm_mbsfn_subframe_config_list->count > 8))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_mbsfn_subframe_config_list->count] should be in range [1 <= %d <= 8] !", p_rrm_mbsfn_subframe_config_list->count);
#endif
        return RRM_FAILURE;
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(*pp_buffer, &p_rrm_mbsfn_subframe_config_list->count, "count");
    *pp_buffer += sizeof(p_rrm_mbsfn_subframe_config_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_mbsfn_subframe_config_list->count; loop++)
        {
            if (RRM_FAILURE == rrm_compose_rrm_mbsfn_config_info(pp_buffer, &p_rrm_mbsfn_subframe_config_list->mbsfn_subframe_config_info[loop]))
            {
                return RRM_FAILURE;
            }
        }
    }

    return RRM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_cell_sib2_mbsfn_data_len
(
    rrm_cell_sib2_mbsfn_data_t *p_rrm_cell_sib2_mbsfn_data
)
{
    rrm_length_t length = 0;

    RRM_ASSERT(p_rrm_cell_sib2_mbsfn_data != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_cell_sib2_mbsfn_data->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_cell_sib2_mbsfn_data->cgi);

    /* Optional element */
    if(p_rrm_cell_sib2_mbsfn_data->bitmask & RRM_SON_MBMS_CONFIG_APPLY_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_mbsfn_subframe_config_list_len(&p_rrm_cell_sib2_mbsfn_data->rrm_mbsfn_subframe_config_list);
    }

    return length;
}

static
rrm_return_et
rrm_compose_rrm_cell_sib2_mbsfn_data
(
    U8  **pp_buffer,
    rrm_cell_sib2_mbsfn_data_t *p_rrm_cell_sib2_mbsfn_data
)
{
    RRM_ASSERT(pp_buffer != PNULL);
    RRM_ASSERT(*pp_buffer != PNULL);
    RRM_ASSERT(p_rrm_cell_sib2_mbsfn_data != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(*pp_buffer, &p_rrm_cell_sib2_mbsfn_data->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_cell_sib2_mbsfn_data->bitmask);

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_cell_sib2_mbsfn_data->cgi))
    {
        return RRM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_cell_sib2_mbsfn_data->bitmask & RRM_SON_MBMS_CONFIG_APPLY_PRESENT)
    {

    /* Compose IE */
    if (RRM_FAILURE == rrm_compose_rrm_mbsfn_subframe_config_list(pp_buffer, &p_rrm_cell_sib2_mbsfn_data->rrm_mbsfn_subframe_config_list))
    {
        return RRM_FAILURE;
    }
    }

    return RRM_SUCCESS;
}

