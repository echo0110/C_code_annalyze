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
 *  File Description : The file rrc_x2apRrm_il_composer.c contains the definitions 
 *                     of rrc-x2apRrm interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#include "rrc_cp_common.h"
#include "rrc_x2apRrm_il_composer.h"
#include "rrc_x2apRrm_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_length_t
rrc_il_get_x2ap_plmn_identity_len
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
);

static
rrc_return_et
rrc_il_compose_x2ap_plmn_identity
(
    U8  **pp_buffer,
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
);

static
rrc_length_t
rrc_il_get_x2ap_ecgi_len
(
    x2ap_ecgi_t *p_x2ap_ecgi
);

static
rrc_return_et
rrc_il_compose_x2ap_ecgi
(
    U8  **pp_buffer,
    x2ap_ecgi_t *p_x2ap_ecgi
);

static
rrc_length_t
rrc_il_get_x2ap_subframe_allocation_len
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
);

static
rrc_return_et
rrc_il_compose_x2ap_subframe_allocation
(
    U8  **pp_buffer,
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
);

static
rrc_length_t
rrc_il_get_x2ap_mbsfn_info_len
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
);

static
rrc_return_et
rrc_il_compose_x2ap_mbsfn_info
(
    U8  **pp_buffer,
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
);

static
rrc_length_t
rrc_il_get_x2ap_mbsfn_subframe_info_len
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
);

static
rrc_return_et
rrc_il_compose_x2ap_mbsfn_subframe_info
(
    U8  **pp_buffer,
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
);

static
rrc_length_t
rrc_il_get_mbms_cell_info_len
(
    mbms_cell_info_t *p_mbms_cell_info
);

static
rrc_return_et
rrc_il_compose_mbms_cell_info
(
    U8  **pp_buffer,
    mbms_cell_info_t *p_mbms_cell_info
);

static
rrc_length_t
rrc_il_get_macro_enb_id_len
(
    macro_enb_id_t *p_macro_enb_id
);

static
rrc_return_et
rrc_il_compose_macro_enb_id
(
    U8  **pp_buffer,
    macro_enb_id_t *p_macro_enb_id
);

static
rrc_length_t
rrc_il_get_home_enb_id_len
(
    home_enb_id_t *p_home_enb_id
);

static
rrc_return_et
rrc_il_compose_home_enb_id
(
    U8  **pp_buffer,
    home_enb_id_t *p_home_enb_id
);

static
rrc_length_t
rrc_il_get_x2_enb_id_info_len
(
    x2_enb_id_info_t *p_x2_enb_id_info
);

static
rrc_return_et
rrc_il_compose_x2_enb_id_info
(
    U8  **pp_buffer,
    x2_enb_id_info_t *p_x2_enb_id_info
);

static
rrc_length_t
rrc_il_get_x2_gb_enb_id_len
(
    x2_gb_enb_id_t *p_x2_gb_enb_id
);

static
rrc_return_et
rrc_il_compose_x2_gb_enb_id
(
    U8  **pp_buffer,
    x2_gb_enb_id_t *p_x2_gb_enb_id
);

static
rrc_length_t
rrc_il_get_x2_gnb_id_len
(
    x2_gnb_id_t *p_x2_gnb_id
);

static
rrc_return_et
rrc_il_compose_x2_gnb_id
(
    U8  **pp_buffer,
    x2_gnb_id_t *p_x2_gnb_id
);

static
rrc_length_t
rrc_il_get_x2_gb_gnb_id_len
(
    x2_gb_gnb_id_t *p_x2_gb_gnb_id
);

static
rrc_return_et
rrc_il_compose_x2_gb_gnb_id
(
    U8  **pp_buffer,
    x2_gb_gnb_id_t *p_x2_gb_gnb_id
);

static
rrc_length_t
rrc_il_get_x2ap_tac_len
(
    x2ap_tac_t *p_x2ap_tac
);

static
rrc_return_et
rrc_il_compose_x2ap_tac
(
    U8  **pp_buffer,
    x2ap_tac_t *p_x2ap_tac
);

static
rrc_length_t
rrc_il_get_x2ap_broadcast_plmn_len
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
);

static
rrc_return_et
rrc_il_compose_x2ap_broadcast_plmn
(
    U8  **pp_buffer,
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
);

static
rrc_length_t
rrc_il_get_x2ap_earfcn_len
(
    x2ap_earfcn_t *p_x2ap_earfcn
);

static
rrc_return_et
rrc_il_compose_x2ap_earfcn
(
    U8  **pp_buffer,
    x2ap_earfcn_t *p_x2ap_earfcn
);

static
rrc_length_t
rrc_il_get_x2ap_fdd_info_len
(
    x2ap_fdd_info_t *p_x2ap_fdd_info
);

static
rrc_return_et
rrc_il_compose_x2ap_fdd_info
(
    U8  **pp_buffer,
    x2ap_fdd_info_t *p_x2ap_fdd_info
);

static
rrc_length_t
rrc_il_get_x2ap_special_subframe_info_len
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
);

static
rrc_return_et
rrc_il_compose_x2ap_special_subframe_info
(
    U8  **pp_buffer,
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
);

static
rrc_length_t
rrc_il_get_x2ap_tdd_info_len
(
    x2ap_tdd_info_t *p_x2ap_tdd_info
);

static
rrc_return_et
rrc_il_compose_x2ap_tdd_info
(
    U8  **pp_buffer,
    x2ap_tdd_info_t *p_x2ap_tdd_info
);

static
rrc_length_t
rrc_il_get_x2ap_choice_eutra_mode_len
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
);

static
rrc_return_et
rrc_il_compose_x2ap_choice_eutra_mode
(
    U8  **pp_buffer,
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
);

static
rrc_length_t
rrc_il_get_x2ap_choice_eutra_mode_info_len
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
);

static
rrc_return_et
rrc_il_compose_x2ap_choice_eutra_mode_info
(
    U8  **pp_buffer,
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
);

static
rrc_length_t
rrc_il_get_x2ap_prach_configuration_info_len
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
);

static
rrc_return_et
rrc_il_compose_x2ap_prach_configuration_info
(
    U8  **pp_buffer,
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
);

static
rrc_length_t
rrc_il_get_x2ap_srvd_cell_info_len
(
    x2ap_srvd_cell_info_t *p_x2ap_srvd_cell_info
);

static
rrc_return_et
rrc_il_compose_x2ap_srvd_cell_info
(
    U8  **pp_buffer,
    x2ap_srvd_cell_info_t *p_x2ap_srvd_cell_info
);

static
rrc_length_t
rrc_il_get_x2ap_neighbour_info_len
(
    x2ap_neighbour_info_t *p_x2ap_neighbour_info
);

static
rrc_return_et
rrc_il_compose_x2ap_neighbour_info
(
    U8  **pp_buffer,
    x2ap_neighbour_info_t *p_x2ap_neighbour_info
);

static
rrc_length_t
rrc_il_get_x2ap_nbour_cell_arry_len
(
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry
);

static
rrc_return_et
rrc_il_compose_x2ap_nbour_cell_arry
(
    U8  **pp_buffer,
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry
);

static
rrc_length_t
rrc_il_get_x2ap_srvd_cell_arry_len
(
    x2ap_srvd_cell_arry_t *p_x2ap_srvd_cell_arry
);

static
rrc_return_et
rrc_il_compose_x2ap_srvd_cell_arry
(
    U8  **pp_buffer,
    x2ap_srvd_cell_arry_t *p_x2ap_srvd_cell_arry
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_cell_info_len
(
    x2ap_rrm_cell_info_t *p_x2ap_rrm_cell_info
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_cell_info
(
    U8  **pp_buffer,
    x2ap_rrm_cell_info_t *p_x2ap_rrm_cell_info
);

static
rrc_length_t
rrc_il_get_x2ap_gu_group_id_len
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id
);

static
rrc_return_et
rrc_il_compose_x2ap_gu_group_id
(
    U8  **pp_buffer,
    x2ap_gu_group_id_t *p_x2ap_gu_group_id
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_eutra_cell_info_len
(
    x2ap_rrm_eutra_cell_info_t *p_x2ap_rrm_eutra_cell_info
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_eutra_cell_info
(
    U8  **pp_buffer,
    x2ap_rrm_eutra_cell_info_t *p_x2ap_rrm_eutra_cell_info
);

static
rrc_length_t
rrc_il_get_x2ap_nr_ecgi_len
(
    x2ap_nr_ecgi_t *p_x2ap_nr_ecgi
);

static
rrc_return_et
rrc_il_compose_x2ap_nr_ecgi
(
    U8  **pp_buffer,
    x2ap_nr_ecgi_t *p_x2ap_nr_ecgi
);

static
rrc_length_t
rrc_il_get_x2ap_nr_freq_band_info_len
(
    x2ap_nr_freq_band_info_t *p_x2ap_nr_freq_band_info
);

static
rrc_return_et
rrc_il_compose_x2ap_nr_freq_band_info
(
    U8  **pp_buffer,
    x2ap_nr_freq_band_info_t *p_x2ap_nr_freq_band_info
);

static
rrc_length_t
rrc_il_get_x2ap_nr_tx_bandwidth_len
(
    x2ap_nr_tx_bandwidth_t *p_x2ap_nr_tx_bandwidth
);

static
rrc_return_et
rrc_il_compose_x2ap_nr_tx_bandwidth
(
    U8  **pp_buffer,
    x2ap_nr_tx_bandwidth_t *p_x2ap_nr_tx_bandwidth
);

static
rrc_length_t
rrc_il_get_x2ap_sul_information_len
(
    x2ap_sul_information_t *p_x2ap_sul_information
);

static
rrc_return_et
rrc_il_compose_x2ap_sul_information
(
    U8  **pp_buffer,
    x2ap_sul_information_t *p_x2ap_sul_information
);

static
rrc_length_t
rrc_il_get_x2ap_nr_freq_info_len
(
    x2ap_nr_freq_info_t *p_x2ap_nr_freq_info
);

static
rrc_return_et
rrc_il_compose_x2ap_nr_freq_info
(
    U8  **pp_buffer,
    x2ap_nr_freq_info_t *p_x2ap_nr_freq_info
);

static
rrc_length_t
rrc_il_get_x2ap_nr_fdd_into_len
(
    x2ap_nr_fdd_into_t *p_x2ap_nr_fdd_into
);

static
rrc_return_et
rrc_il_compose_x2ap_nr_fdd_into
(
    U8  **pp_buffer,
    x2ap_nr_fdd_into_t *p_x2ap_nr_fdd_into
);

static
rrc_length_t
rrc_il_get_x2ap_nr_tdd_into_len
(
    x2ap_nr_tdd_into_t *p_x2ap_nr_tdd_into
);

static
rrc_return_et
rrc_il_compose_x2ap_nr_tdd_into
(
    U8  **pp_buffer,
    x2ap_nr_tdd_into_t *p_x2ap_nr_tdd_into
);

static
rrc_length_t
rrc_il_get_x2ap_choice_nr_mode_info_len
(
    x2ap_choice_nr_mode_info_t *p_x2ap_choice_nr_mode_info
);

static
rrc_return_et
rrc_il_compose_x2ap_choice_nr_mode_info
(
    U8  **pp_buffer,
    x2ap_choice_nr_mode_info_t *p_x2ap_choice_nr_mode_info
);

static
rrc_length_t
rrc_il_get_x2ap_periodicity_and_offset_len
(
    x2ap_periodicity_and_offset_t *p_x2ap_periodicity_and_offset
);

static
rrc_return_et
rrc_il_compose_x2ap_periodicity_and_offset
(
    U8  **pp_buffer,
    x2ap_periodicity_and_offset_t *p_x2ap_periodicity_and_offset
);

static
rrc_length_t
rrc_il_get_x2ap_ssb_mtc_len
(
    x2ap_ssb_mtc_t *p_x2ap_ssb_mtc
);

static
rrc_return_et
rrc_il_compose_x2ap_ssb_mtc
(
    U8  **pp_buffer,
    x2ap_ssb_mtc_t *p_x2ap_ssb_mtc
);

static
rrc_length_t
rrc_il_get_x2ap_measurement_slots_len
(
    x2ap_measurement_slots_t *p_x2ap_measurement_slots
);

static
rrc_return_et
rrc_il_compose_x2ap_measurement_slots
(
    U8  **pp_buffer,
    x2ap_measurement_slots_t *p_x2ap_measurement_slots
);

static
rrc_length_t
rrc_il_get_x2ap_ss_rssi_measurement_len
(
    x2ap_ss_rssi_measurement_t *p_x2ap_ss_rssi_measurement
);

static
rrc_return_et
rrc_il_compose_x2ap_ss_rssi_measurement
(
    U8  **pp_buffer,
    x2ap_ss_rssi_measurement_t *p_x2ap_ss_rssi_measurement
);

static
rrc_length_t
rrc_il_get_x2ap_meas_timing_config_len
(
    x2ap_meas_timing_config_t *p_x2ap_meas_timing_config
);

static
rrc_return_et
rrc_il_compose_x2ap_meas_timing_config
(
    U8  **pp_buffer,
    x2ap_meas_timing_config_t *p_x2ap_meas_timing_config
);

static
rrc_length_t
rrc_il_get_x2ap_five_gs_tac_len
(
    x2ap_five_gs_tac_t *p_x2ap_five_gs_tac
);

static
rrc_return_et
rrc_il_compose_x2ap_five_gs_tac
(
    U8  **pp_buffer,
    x2ap_five_gs_tac_t *p_x2ap_five_gs_tac
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_srvd_nr_cell_info_len
(
    x2ap_rrm_srvd_nr_cell_info_t *p_x2ap_rrm_srvd_nr_cell_info
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_srvd_nr_cell_info
(
    U8  **pp_buffer,
    x2ap_rrm_srvd_nr_cell_info_t *p_x2ap_rrm_srvd_nr_cell_info
);

static
rrc_length_t
rrc_il_get_x2ap_fdd_info_neigh_served_nr_cell_info_len
(
    x2ap_fdd_info_neigh_served_nr_cell_info_t *p_x2ap_fdd_info_neigh_served_nr_cell_info
);

static
rrc_return_et
rrc_il_compose_x2ap_fdd_info_neigh_served_nr_cell_info
(
    U8  **pp_buffer,
    x2ap_fdd_info_neigh_served_nr_cell_info_t *p_x2ap_fdd_info_neigh_served_nr_cell_info
);

static
rrc_length_t
rrc_il_get_x2ap_tdd_info_neigh_served_nr_cell_info_len
(
    x2ap_tdd_info_neigh_served_nr_cell_info_t *p_x2ap_tdd_info_neigh_served_nr_cell_info
);

static
rrc_return_et
rrc_il_compose_x2ap_tdd_info_neigh_served_nr_cell_info
(
    U8  **pp_buffer,
    x2ap_tdd_info_neigh_served_nr_cell_info_t *p_x2ap_tdd_info_neigh_served_nr_cell_info
);

static
rrc_length_t
rrc_il_get_x2ap_nr_neigh_mode_info_len
(
    x2ap_nr_neigh_mode_info_t *p_x2ap_nr_neigh_mode_info
);

static
rrc_return_et
rrc_il_compose_x2ap_nr_neigh_mode_info
(
    U8  **pp_buffer,
    x2ap_nr_neigh_mode_info_t *p_x2ap_nr_neigh_mode_info
);

static
rrc_length_t
rrc_il_get_x2ap_nr_neigh_info_len
(
    x2ap_nr_neigh_info_t *p_x2ap_nr_neigh_info
);

static
rrc_return_et
rrc_il_compose_x2ap_nr_neigh_info
(
    U8  **pp_buffer,
    x2ap_nr_neigh_info_t *p_x2ap_nr_neigh_info
);

static
rrc_length_t
rrc_il_get_x2ap_nr_neigh_cell_arry_len
(
    x2ap_nr_neigh_cell_arry_t *p_x2ap_nr_neigh_cell_arry
);

static
rrc_return_et
rrc_il_compose_x2ap_nr_neigh_cell_arry
(
    U8  **pp_buffer,
    x2ap_nr_neigh_cell_arry_t *p_x2ap_nr_neigh_cell_arry
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_nr_cell_info_len
(
    x2ap_rrm_nr_cell_info_t *p_x2ap_rrm_nr_cell_info
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_nr_cell_info
(
    U8  **pp_buffer,
    x2ap_rrm_nr_cell_info_t *p_x2ap_rrm_nr_cell_info
);

static
rrc_length_t
rrc_il_get_mnc_len
(
    mnc_t *p_mnc
);

static
rrc_return_et
rrc_il_compose_mnc
(
    U8  **pp_buffer,
    mnc_t *p_mnc
);

static
rrc_length_t
rrc_il_get_plmn_identity_len
(
    plmn_identity_t *p_plmn_identity
);

static
rrc_return_et
rrc_il_compose_plmn_identity
(
    U8  **pp_buffer,
    plmn_identity_t *p_plmn_identity
);

static
rrc_length_t
rrc_il_get_rrm_cell_global_id_eutra_len
(
    rrm_cell_global_id_eutra_t *p_rrm_cell_global_id_eutra
);

static
rrc_return_et
rrc_il_compose_rrm_cell_global_id_eutra
(
    U8  **pp_buffer,
    rrm_cell_global_id_eutra_t *p_rrm_cell_global_id_eutra
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_cause_len
(
    x2ap_rrm_cause_t *p_x2ap_rrm_cause
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_cause
(
    U8  **pp_buffer,
    x2ap_rrm_cause_t *p_x2ap_rrm_cause
);

static
rrc_length_t
rrc_il_get_measurement_failure_cause_item_len
(
    measurement_failure_cause_item_t *p_measurement_failure_cause_item
);

static
rrc_return_et
rrc_il_compose_measurement_failure_cause_item
(
    U8  **pp_buffer,
    measurement_failure_cause_item_t *p_measurement_failure_cause_item
);

static
rrc_length_t
rrc_il_get_rsu_measurement_res_item_len
(
    rsu_measurement_res_item_t *p_rsu_measurement_res_item
);

static
rrc_return_et
rrc_il_compose_rsu_measurement_res_item
(
    U8  **pp_buffer,
    rsu_measurement_res_item_t *p_rsu_measurement_res_item
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_measurement_res_len
(
    x2ap_rrm_rsu_measurement_res_t *p_x2ap_rrm_rsu_measurement_res
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_measurement_res
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_measurement_res_t *p_x2ap_rrm_rsu_measurement_res
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_prb_len
(
    x2ap_rrm_rsu_prb_t *p_x2ap_rrm_rsu_prb
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_prb
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_prb_t *p_x2ap_rrm_rsu_prb
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_tnl_load_len
(
    x2ap_rrm_rsu_tnl_load_t *p_x2ap_rrm_rsu_tnl_load
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_tnl_load
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_tnl_load_t *p_x2ap_rrm_rsu_tnl_load
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_hw_load_len
(
    x2ap_rrm_rsu_hw_load_t *p_x2ap_rrm_rsu_hw_load
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_hw_load
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_hw_load_t *p_x2ap_rrm_rsu_hw_load
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_composite_available_capacity_len
(
    x2ap_rrm_rsu_composite_available_capacity_t *p_x2ap_rrm_rsu_composite_available_capacity
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_composite_available_capacity
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_composite_available_capacity_t *p_x2ap_rrm_rsu_composite_available_capacity
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_cacg_len
(
    x2ap_rrm_rsu_cacg_t *p_x2ap_rrm_rsu_cacg
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_cacg
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_cacg_t *p_x2ap_rrm_rsu_cacg
);

static
rrc_length_t
rrc_il_get_x2ap_usable_abs_information_fdd_len
(
    x2ap_usable_abs_information_fdd_t *p_x2ap_usable_abs_information_fdd
);

static
rrc_return_et
rrc_il_compose_x2ap_usable_abs_information_fdd
(
    U8  **pp_buffer,
    x2ap_usable_abs_information_fdd_t *p_x2ap_usable_abs_information_fdd
);

static
rrc_length_t
rrc_il_get_x2ap_usable_abs_information_tdd_len
(
    x2ap_usable_abs_information_tdd_t *p_x2ap_usable_abs_information_tdd
);

static
rrc_return_et
rrc_il_compose_x2ap_usable_abs_information_tdd
(
    U8  **pp_buffer,
    x2ap_usable_abs_information_tdd_t *p_x2ap_usable_abs_information_tdd
);

static
rrc_length_t
rrc_il_get_x2ap_usable_abs_information_len
(
    x2ap_usable_abs_information_t *p_x2ap_usable_abs_information
);

static
rrc_return_et
rrc_il_compose_x2ap_usable_abs_information
(
    U8  **pp_buffer,
    x2ap_usable_abs_information_t *p_x2ap_usable_abs_information
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_abs_status_len
(
    x2ap_rrm_abs_status_t *p_x2ap_rrm_abs_status
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_abs_status
(
    U8  **pp_buffer,
    x2ap_rrm_abs_status_t *p_x2ap_rrm_abs_status
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_info_len
(
    x2ap_rrm_rsu_info_t *p_x2ap_rrm_rsu_info
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_info
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_info_t *p_x2ap_rrm_rsu_info
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_report_info_len
(
    x2ap_rrm_rsu_report_info_t *p_x2ap_rrm_rsu_report_info
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_report_info
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_report_info_t *p_x2ap_rrm_rsu_report_info
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_ul_intf_overload_ind_len
(
    x2ap_rrm_li_ul_intf_overload_ind_t *p_x2ap_rrm_li_ul_intf_overload_ind
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_ul_intf_overload_ind
(
    U8  **pp_buffer,
    x2ap_rrm_li_ul_intf_overload_ind_t *p_x2ap_rrm_li_ul_intf_overload_ind
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_ul_high_intf_ind_len
(
    x2ap_rrm_li_ul_high_intf_ind_t *p_x2ap_rrm_li_ul_high_intf_ind
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_ul_high_intf_ind
(
    U8  **pp_buffer,
    x2ap_rrm_li_ul_high_intf_ind_t *p_x2ap_rrm_li_ul_high_intf_ind
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_ul_high_intf_info_arr_len
(
    x2ap_rrm_li_ul_high_intf_info_arr_t *p_x2ap_rrm_li_ul_high_intf_info_arr
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_ul_high_intf_info_arr
(
    U8  **pp_buffer,
    x2ap_rrm_li_ul_high_intf_info_arr_t *p_x2ap_rrm_li_ul_high_intf_info_arr
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_ul_high_interference_info_len
(
    x2ap_rrm_li_ul_high_interference_info_t *p_x2ap_rrm_li_ul_high_interference_info
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_ul_high_interference_info
(
    U8  **pp_buffer,
    x2ap_rrm_li_ul_high_interference_info_t *p_x2ap_rrm_li_ul_high_interference_info
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_relative_narrowband_tx_power_len
(
    x2ap_rrm_li_relative_narrowband_tx_power_t *p_x2ap_rrm_li_relative_narrowband_tx_power
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_relative_narrowband_tx_power
(
    U8  **pp_buffer,
    x2ap_rrm_li_relative_narrowband_tx_power_t *p_x2ap_rrm_li_relative_narrowband_tx_power
);

static
rrc_length_t
rrc_il_get_x2ap_abs_information_fdd_len
(
    x2ap_abs_information_fdd_t *p_x2ap_abs_information_fdd
);

static
rrc_return_et
rrc_il_compose_x2ap_abs_information_fdd
(
    U8  **pp_buffer,
    x2ap_abs_information_fdd_t *p_x2ap_abs_information_fdd
);

static
rrc_length_t
rrc_il_get_x2ap_abs_information_tdd_len
(
    x2ap_abs_information_tdd_t *p_x2ap_abs_information_tdd
);

static
rrc_return_et
rrc_il_compose_x2ap_abs_information_tdd
(
    U8  **pp_buffer,
    x2ap_abs_information_tdd_t *p_x2ap_abs_information_tdd
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_abs_information_len
(
    x2ap_rrm_li_abs_information_t *p_x2ap_rrm_li_abs_information
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_abs_information
(
    U8  **pp_buffer,
    x2ap_rrm_li_abs_information_t *p_x2ap_rrm_li_abs_information
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_invoke_indication_len
(
    x2ap_rrm_li_invoke_indication_t *p_x2ap_rrm_li_invoke_indication
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_invoke_indication
(
    U8  **pp_buffer,
    x2ap_rrm_li_invoke_indication_t *p_x2ap_rrm_li_invoke_indication
);

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_cell_info_item_arr_len
(
    x2ap_rrm_li_cell_info_item_arr_t *p_x2ap_rrm_li_cell_info_item_arr
);

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_cell_info_item_arr
(
    U8  **pp_buffer,
    x2ap_rrm_li_cell_info_item_arr_t *p_x2ap_rrm_li_cell_info_item_arr
);

static
rrc_length_t
rrc_il_get_rrc_rlf_report_meas_result_serv_cell_len
(
    rrc_rlf_report_meas_result_serv_cell_t *p_rrc_rlf_report_meas_result_serv_cell
);

static
rrc_return_et
rrc_il_compose_rrc_rlf_report_meas_result_serv_cell
(
    U8  **pp_buffer,
    rrc_rlf_report_meas_result_serv_cell_t *p_rrc_rlf_report_meas_result_serv_cell
);

static
rrc_length_t
rrc_il_get_cell_global_id_eutra_len
(
    cell_global_id_eutra_t *p_cell_global_id_eutra
);

static
rrc_return_et
rrc_il_compose_cell_global_id_eutra
(
    U8  **pp_buffer,
    cell_global_id_eutra_t *p_cell_global_id_eutra
);

static
rrc_length_t
rrc_il_get_plmn_identity_list2_len
(
    plmn_identity_list2_t *p_plmn_identity_list2
);

static
rrc_return_et
rrc_il_compose_plmn_identity_list2
(
    U8  **pp_buffer,
    plmn_identity_list2_t *p_plmn_identity_list2
);

static
rrc_length_t
rrc_il_get_meas_result_eutra_cgi_info_len
(
    meas_result_eutra_cgi_info_t *p_meas_result_eutra_cgi_info
);

static
rrc_return_et
rrc_il_compose_meas_result_eutra_cgi_info
(
    U8  **pp_buffer,
    meas_result_eutra_cgi_info_t *p_meas_result_eutra_cgi_info
);

static
rrc_length_t
rrc_il_get_meas_result_eutra_meas_result_len
(
    meas_result_eutra_meas_result_t *p_meas_result_eutra_meas_result
);

static
rrc_return_et
rrc_il_compose_meas_result_eutra_meas_result
(
    U8  **pp_buffer,
    meas_result_eutra_meas_result_t *p_meas_result_eutra_meas_result
);

static
rrc_length_t
rrc_il_get_meas_result_csg_info_len
(
    meas_result_csg_info_t *p_meas_result_csg_info
);

static
rrc_return_et
rrc_il_compose_meas_result_csg_info
(
    U8  **pp_buffer,
    meas_result_csg_info_t *p_meas_result_csg_info
);

static
rrc_length_t
rrc_il_get_meas_result_eutra_len
(
    meas_result_eutra_t *p_meas_result_eutra
);

static
rrc_return_et
rrc_il_compose_meas_result_eutra
(
    U8  **pp_buffer,
    meas_result_eutra_t *p_meas_result_eutra
);

static
rrc_length_t
rrc_il_get_meas_result_list_eutra_len
(
    meas_result_list_eutra_t *p_meas_result_list_eutra
);

static
rrc_return_et
rrc_il_compose_meas_result_list_eutra
(
    U8  **pp_buffer,
    meas_result_list_eutra_t *p_meas_result_list_eutra
);

static
rrc_length_t
rrc_il_get_rrc_meas_result2_eutra_len
(
    rrc_meas_result2_eutra_t *p_rrc_meas_result2_eutra
);

static
rrc_return_et
rrc_il_compose_rrc_meas_result2_eutra
(
    U8  **pp_buffer,
    rrc_meas_result2_eutra_t *p_rrc_meas_result2_eutra
);

static
rrc_length_t
rrc_il_get_rrc_meas_result_list2_eutra_len
(
    rrc_meas_result_list2_eutra_t *p_rrc_meas_result_list2_eutra
);

static
rrc_return_et
rrc_il_compose_rrc_meas_result_list2_eutra
(
    U8  **pp_buffer,
    rrc_meas_result_list2_eutra_t *p_rrc_meas_result_list2_eutra
);

static
rrc_length_t
rrc_il_get_meas_result_utra_phys_cell_id_len
(
    meas_result_utra_phys_cell_id_t *p_meas_result_utra_phys_cell_id
);

static
rrc_return_et
rrc_il_compose_meas_result_utra_phys_cell_id
(
    U8  **pp_buffer,
    meas_result_utra_phys_cell_id_t *p_meas_result_utra_phys_cell_id
);

static
rrc_length_t
rrc_il_get_cell_global_id_utra_len
(
    cell_global_id_utra_t *p_cell_global_id_utra
);

static
rrc_return_et
rrc_il_compose_cell_global_id_utra
(
    U8  **pp_buffer,
    cell_global_id_utra_t *p_cell_global_id_utra
);

static
rrc_length_t
rrc_il_get_meas_result_utra_cgi_info_len
(
    meas_result_utra_cgi_info_t *p_meas_result_utra_cgi_info
);

static
rrc_return_et
rrc_il_compose_meas_result_utra_cgi_info
(
    U8  **pp_buffer,
    meas_result_utra_cgi_info_t *p_meas_result_utra_cgi_info
);

static
rrc_length_t
rrc_il_get_meas_result_utra_meas_result_len
(
    meas_result_utra_meas_result_t *p_meas_result_utra_meas_result
);

static
rrc_return_et
rrc_il_compose_meas_result_utra_meas_result
(
    U8  **pp_buffer,
    meas_result_utra_meas_result_t *p_meas_result_utra_meas_result
);

static
rrc_length_t
rrc_il_get_meas_result_utra_len
(
    meas_result_utra_t *p_meas_result_utra
);

static
rrc_return_et
rrc_il_compose_meas_result_utra
(
    U8  **pp_buffer,
    meas_result_utra_t *p_meas_result_utra
);

static
rrc_length_t
rrc_il_get_meas_result_list_utra_len
(
    meas_result_list_utra_t *p_meas_result_list_utra
);

static
rrc_return_et
rrc_il_compose_meas_result_list_utra
(
    U8  **pp_buffer,
    meas_result_list_utra_t *p_meas_result_list_utra
);

static
rrc_length_t
rrc_il_get_rrc_meas_result2_utra_len
(
    rrc_meas_result2_utra_t *p_rrc_meas_result2_utra
);

static
rrc_return_et
rrc_il_compose_rrc_meas_result2_utra
(
    U8  **pp_buffer,
    rrc_meas_result2_utra_t *p_rrc_meas_result2_utra
);

static
rrc_length_t
rrc_il_get_rrc_meas_result_list2_utra_len
(
    rrc_meas_result_list2_utra_t *p_rrc_meas_result_list2_utra
);

static
rrc_return_et
rrc_il_compose_rrc_meas_result_list2_utra
(
    U8  **pp_buffer,
    rrc_meas_result_list2_utra_t *p_rrc_meas_result_list2_utra
);

static
rrc_length_t
rrc_il_get_carrier_freq_geran_len
(
    carrier_freq_geran_t *p_carrier_freq_geran
);

static
rrc_return_et
rrc_il_compose_carrier_freq_geran
(
    U8  **pp_buffer,
    carrier_freq_geran_t *p_carrier_freq_geran
);

static
rrc_length_t
rrc_il_get_phys_cell_id_geran_len
(
    phys_cell_id_geran_t *p_phys_cell_id_geran
);

static
rrc_return_et
rrc_il_compose_phys_cell_id_geran
(
    U8  **pp_buffer,
    phys_cell_id_geran_t *p_phys_cell_id_geran
);

static
rrc_length_t
rrc_il_get_cell_global_id_geran_len
(
    cell_global_id_geran_t *p_cell_global_id_geran
);

static
rrc_return_et
rrc_il_compose_cell_global_id_geran
(
    U8  **pp_buffer,
    cell_global_id_geran_t *p_cell_global_id_geran
);

static
rrc_length_t
rrc_il_get_meas_result_geran_cgi_info_len
(
    meas_result_geran_cgi_info_t *p_meas_result_geran_cgi_info
);

static
rrc_return_et
rrc_il_compose_meas_result_geran_cgi_info
(
    U8  **pp_buffer,
    meas_result_geran_cgi_info_t *p_meas_result_geran_cgi_info
);

static
rrc_length_t
rrc_il_get_meas_result_geran_meas_result_len
(
    meas_result_geran_meas_result_t *p_meas_result_geran_meas_result
);

static
rrc_return_et
rrc_il_compose_meas_result_geran_meas_result
(
    U8  **pp_buffer,
    meas_result_geran_meas_result_t *p_meas_result_geran_meas_result
);

static
rrc_length_t
rrc_il_get_meas_result_geran_len
(
    meas_result_geran_t *p_meas_result_geran
);

static
rrc_return_et
rrc_il_compose_meas_result_geran
(
    U8  **pp_buffer,
    meas_result_geran_t *p_meas_result_geran
);

static
rrc_length_t
rrc_il_get_meas_result_list_geran_len
(
    meas_result_list_geran_t *p_meas_result_list_geran
);

static
rrc_return_et
rrc_il_compose_meas_result_list_geran
(
    U8  **pp_buffer,
    meas_result_list_geran_t *p_meas_result_list_geran
);

static
rrc_length_t
rrc_il_get_carrier_freq_cdma2000_len
(
    carrier_freq_cdma2000_t *p_carrier_freq_cdma2000
);

static
rrc_return_et
rrc_il_compose_carrier_freq_cdma2000
(
    U8  **pp_buffer,
    carrier_freq_cdma2000_t *p_carrier_freq_cdma2000
);

static
rrc_length_t
rrc_il_get_cell_global_id_cdma2000_len
(
    cell_global_id_cdma2000_t *p_cell_global_id_cdma2000
);

static
rrc_return_et
rrc_il_compose_cell_global_id_cdma2000
(
    U8  **pp_buffer,
    cell_global_id_cdma2000_t *p_cell_global_id_cdma2000
);

static
rrc_length_t
rrc_il_get_meas_result_cdma2000_meas_result_len
(
    meas_result_cdma2000_meas_result_t *p_meas_result_cdma2000_meas_result
);

static
rrc_return_et
rrc_il_compose_meas_result_cdma2000_meas_result
(
    U8  **pp_buffer,
    meas_result_cdma2000_meas_result_t *p_meas_result_cdma2000_meas_result
);

static
rrc_length_t
rrc_il_get_meas_result_cdma2000_len
(
    meas_result_cdma2000_t *p_meas_result_cdma2000
);

static
rrc_return_et
rrc_il_compose_meas_result_cdma2000
(
    U8  **pp_buffer,
    meas_result_cdma2000_t *p_meas_result_cdma2000
);

static
rrc_length_t
rrc_il_get_meas_result_list_cdma2000_len
(
    meas_result_list_cdma2000_t *p_meas_result_list_cdma2000
);

static
rrc_return_et
rrc_il_compose_meas_result_list_cdma2000
(
    U8  **pp_buffer,
    meas_result_list_cdma2000_t *p_meas_result_list_cdma2000
);

static
rrc_length_t
rrc_il_get_meas_results_cdma2000_len
(
    meas_results_cdma2000_t *p_meas_results_cdma2000
);

static
rrc_return_et
rrc_il_compose_meas_results_cdma2000
(
    U8  **pp_buffer,
    meas_results_cdma2000_t *p_meas_results_cdma2000
);

static
rrc_length_t
rrc_il_get_rrc_meas_result2_cdma2000_len
(
    rrc_meas_result2_cdma2000_t *p_rrc_meas_result2_cdma2000
);

static
rrc_return_et
rrc_il_compose_rrc_meas_result2_cdma2000
(
    U8  **pp_buffer,
    rrc_meas_result2_cdma2000_t *p_rrc_meas_result2_cdma2000
);

static
rrc_length_t
rrc_il_get_meas_result_list2_cdma2000_len
(
    meas_result_list2_cdma2000_t *p_meas_result_list2_cdma2000
);

static
rrc_return_et
rrc_il_compose_meas_result_list2_cdma2000
(
    U8  **pp_buffer,
    meas_result_list2_cdma2000_t *p_meas_result_list2_cdma2000
);

static
rrc_length_t
rrc_il_get_rrc_rlf_report_meas_result_neigh_cell_len
(
    rrc_rlf_report_meas_result_neigh_cell_t *p_rrc_rlf_report_meas_result_neigh_cell
);

static
rrc_return_et
rrc_il_compose_rrc_rlf_report_meas_result_neigh_cell
(
    U8  **pp_buffer,
    rrc_rlf_report_meas_result_neigh_cell_t *p_rrc_rlf_report_meas_result_neigh_cell
);

static
rrc_length_t
rrc_il_get_rrc_rlf_dynamic_string_len
(
    rrc_rlf_dynamic_string_t *p_rrc_rlf_dynamic_string
);

static
rrc_return_et
rrc_il_compose_rrc_rlf_dynamic_string
(
    U8  **pp_buffer,
    rrc_rlf_dynamic_string_t *p_rrc_rlf_dynamic_string
);

static
rrc_length_t
rrc_il_get_rrc_rlf_report_location_coordinates_len
(
    rrc_rlf_report_location_coordinates_t *p_rrc_rlf_report_location_coordinates
);

static
rrc_return_et
rrc_il_compose_rrc_rlf_report_location_coordinates
(
    U8  **pp_buffer,
    rrc_rlf_report_location_coordinates_t *p_rrc_rlf_report_location_coordinates
);

static
rrc_length_t
rrc_il_get_rrc_rlf_report_location_info_len
(
    rrc_rlf_report_location_info_t *p_rrc_rlf_report_location_info
);

static
rrc_return_et
rrc_il_compose_rrc_rlf_report_location_info
(
    U8  **pp_buffer,
    rrc_rlf_report_location_info_t *p_rrc_rlf_report_location_info
);

static
rrc_length_t
rrc_il_get_rrc_rlf_failed_pci_arfcn_len
(
    rrc_rlf_failed_pci_arfcn_t *p_rrc_rlf_failed_pci_arfcn
);

static
rrc_return_et
rrc_il_compose_rrc_rlf_failed_pci_arfcn
(
    U8  **pp_buffer,
    rrc_rlf_failed_pci_arfcn_t *p_rrc_rlf_failed_pci_arfcn
);

static
rrc_length_t
rrc_il_get_rrc_rlf_failed_pcellid_len
(
    rrc_rlf_failed_pcellid_t *p_rrc_rlf_failed_pcellid
);

static
rrc_return_et
rrc_il_compose_rrc_rlf_failed_pcellid
(
    U8  **pp_buffer,
    rrc_rlf_failed_pcellid_t *p_rrc_rlf_failed_pcellid
);

static
rrc_length_t
rrc_il_get_rrc_rlf_report_len
(
    rrc_rlf_report_t *p_rrc_rlf_report
);

static
rrc_return_et
rrc_il_compose_rrc_rlf_report
(
    U8  **pp_buffer,
    rrc_rlf_report_t *p_rrc_rlf_report
);

#ifdef ENDC_ENABLED
static rrc_length_t 
rrc_il_get_x2ap_cause_len
(
    x2ap_cause_t *p_x2ap_cause
);
static rrc_return_et 
rrc_il_compose_x2ap_cause
(
    U8  **pp_buffer,
    x2ap_cause_t *p_x2ap_cause
);
static rrc_length_t 
rrc_il_get_x2ap_partial_list_len
(
    x2ap_partial_list_t *p_x2ap_parlist_list
);

static rrc_return_et 
rrc_il_compose_x2ap_partial_list
(
    U8  **pp_buffer,
    x2ap_partial_list_t *p_x2ap_parlist_list
);

static
rrc_length_t
rrc_il_get_x2ap_served_cell_info_len
(
    x2ap_served_cell_info_t *p_x2ap_served_cell_info
);

static
rrc_return_et
rrc_il_compose_x2ap_served_cell_info
(
    U8  **pp_buffer,
    x2ap_served_cell_info_t *p_x2ap_served_cell_info
);

rrc_length_t
rrc_il_get_x2ap_nr_nbour_cell_info_len
(
    x2ap_nr_nbour_cell_info_t *p_x2ap_nr_nbour_cell_info
);

rrc_return_et 
rrc_il_compose_x2ap_nr_nbour_cell_info
(
    U8  **pp_buffer,
    x2ap_nr_nbour_cell_info_t *p_x2ap_nr_nbour_cell_info
);

rrc_length_t
rrc_il_get_x2ap_nr_srvd_cell_to_management_list_len
(
    x2ap_nr_srvd_cell_to_management_list_t *p_x2ap_nr_srvd_cell_to_add_list
);

rrc_return_et 
rrc_il_compose_x2ap_nr_srvd_cell_to_management_list
(
    U8  **pp_buffer,
    x2ap_nr_srvd_cell_to_management_list_t *p_x2ap_nr_srvd_cell_to_add_list
);

rrc_length_t
rrc_il_get_x2ap_nr_srvd_cell_to_modify_list_len
(
    x2ap_nr_srvd_cell_to_modify_list_t *p_x2ap_nr_srvd_cell_to_mod_list
);

rrc_return_et 
rrc_il_compose_x2ap_nr_srvd_cell_to_modify_list
(
    U8  **pp_buffer,
    x2ap_nr_srvd_cell_to_modify_list_t *p_x2ap_nr_srvd_cell_to_mod_list
);

rrc_length_t
rrc_il_get_x2ap_nr_srvd_cell_to_delete_list_len
(
    x2ap_nr_srvd_cell_to_delete_list_t *p_x2ap_nr_srvd_cell_to_del_list
);

rrc_return_et 
rrc_il_compose_x2ap_nr_srvd_cell_to_delete_list
(
    U8  **pp_buffer,
    x2ap_nr_srvd_cell_to_delete_list_t *p_x2ap_nr_srvd_cell_to_del_list
);

rrc_length_t 
rrc_il_get_x2ap_served_eutra_nr_cell_info_arr_len
(
    x2ap_served_eutra_nr_cell_info_arr_t *p_x2ap_served_eutra_nr_cell_info_arr
);

rrc_return_et 
rrc_il_compose_x2ap_served_eutra_nr_cell_info_arr
(
    U8  **pp_buffer,
    x2ap_served_eutra_nr_cell_info_arr_t *p_x2ap_served_eutra_nr_cell_info_arr
);

rrc_length_t 
rrc_il_get_x2ap_served_eutra_nr_cells_to_modify_info_len
(
    x2ap_served_eutra_nr_cells_to_modify_info_t *p_x2ap_served_eutra_nr_cell_to_mod_info
);

rrc_return_et 
rrc_il_compose_x2ap_served_eutra_nr_cells_to_modify_info
(
    U8  **pp_buffer,
    x2ap_served_eutra_nr_cells_to_modify_info_t *p_x2ap_served_eutra_nr_cell_to_mod_info
);

rrc_length_t 
rrc_il_get_x2ap_rrm_cell_assistance_info_len
(
    x2ap_rrm_cell_assistance_info_t *p_x2ap_rrm_cell_assis_info
);

rrc_return_et 
rrc_il_compose_x2ap_rrm_cell_assistance_info
(
    U8  **pp_buffer,
    x2ap_rrm_cell_assistance_info_t *p_x2ap_rrm_cell_assis_info
);

rrc_length_t
rrc_il_get_x2ap_served_eutra_nr_cells_to_add_list_len
(
    x2ap_served_eutra_nr_cells_to_add_list_t *p_x2ap_served_eutra_nr_cell_to_add_list
);

rrc_return_et 
rrc_il_compose_x2ap_served_eutra_nr_cells_to_add_list
(
    U8  **pp_buffer,
    x2ap_served_eutra_nr_cells_to_add_list_t *p_x2ap_served_eutra_nr_cell_to_add_list
);

rrc_length_t
rrc_il_get_x2ap_served_eutra_nr_cells_to_modify_list_len
(
    x2ap_served_eutra_nr_cells_to_modify_list_t *p_x2ap_served_eutra_nr_cell_to_mod_list
);

rrc_return_et 
rrc_il_compose_x2ap_served_eutra_nr_cells_to_modify_list
(
    U8  **pp_buffer,
    x2ap_served_eutra_nr_cells_to_modify_list_t *p_x2ap_served_eutra_nr_cell_to_mod_list
);

rrc_length_t
rrc_il_get_x2ap_served_eutra_nr_cells_to_delete_list_len
(
    x2ap_served_eutra_nr_cells_to_delete_list_t *p_x2ap_served_eutra_nr_cell_to_del_list
);

rrc_return_et 
rrc_il_compose_x2ap_served_eutra_nr_cells_to_delete_list
(
    U8  **pp_buffer,
    x2ap_served_eutra_nr_cells_to_delete_list_t *p_x2ap_served_eutra_nr_cell_to_del_list
);

#endif

/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
rrc_il_get_rrm_updated_mbms_sf_info_len
(
    rrm_updated_mbms_sf_info_t *p_rrm_updated_mbms_sf_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrm_updated_mbms_sf_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrm_updated_mbms_sf_info->num_cells);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_updated_mbms_sf_info->num_cells < 1) || (p_rrm_updated_mbms_sf_info->num_cells > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrm_updated_mbms_sf_info->num_cells] should be in range "
            "1 to 8. Incorrect value %d received.", p_rrm_updated_mbms_sf_info->num_cells);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrm_updated_mbms_sf_info->num_cells; loop++)
        {
            length += rrc_il_get_mbms_cell_info_len(&p_rrm_updated_mbms_sf_info->cell_info[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrm_updated_mbms_sf_info
(
    U8  **pp_buffer,
    rrm_updated_mbms_sf_info_t *p_rrm_updated_mbms_sf_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrm_updated_mbms_sf_info != PNULL);

    /* This function composes rrm_updated_mbms_sf_info */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_updated_mbms_sf_info->num_cells < 1) || (p_rrm_updated_mbms_sf_info->num_cells > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrm_updated_mbms_sf_info->num_cells] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrm_updated_mbms_sf_info->num_cells);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrm_updated_mbms_sf_info->num_cells, "num_cells");
    *pp_buffer += sizeof(p_rrm_updated_mbms_sf_info->num_cells);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrm_updated_mbms_sf_info->num_cells; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_mbms_cell_info(pp_buffer, &p_rrm_updated_mbms_sf_info->cell_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_rrm_updated_mbms_sf_info
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_UPDATED_MBMS_SF_INFO_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_rrm_updated_mbms_sf_info
(
    rrm_updated_mbms_sf_info_t  *p_rrm_updated_mbms_sf_info,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_rrm_updated_mbms_sf_info != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrm_updated_mbms_sf_info_len(p_rrm_updated_mbms_sf_info);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_UPDATED_MBMS_SF_INFO_IND", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_UPDATED_MBMS_SF_INFO_IND, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_UPDATED_MBMS_SF_INFO_IND, msg_length,cell_index);

    /* Fill X2AP_RRM_UPDATED_MBMS_SF_INFO_IND message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrm_updated_mbms_sf_info(&p_x2apRrm_msg, p_rrm_updated_mbms_sf_info))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
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
rrc_il_get_x2ap_plmn_identity_len
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_plmn_identity != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_plmn_identity->plmn_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_plmn_identity
(
    U8  **pp_buffer,
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_plmn_identity != PNULL);

    /* This function composes x2ap_plmn_identity */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_plmn_identity->plmn_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_plmn_identity->plmn_id[loop], "plmn_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_ecgi_len
(
    x2ap_ecgi_t *p_x2ap_ecgi
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_ecgi != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_plmn_identity_len(&p_x2ap_ecgi->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_ecgi->eutran_cell_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_ecgi
(
    U8  **pp_buffer,
    x2ap_ecgi_t *p_x2ap_ecgi
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_ecgi != PNULL);

    /* This function composes x2ap_ecgi */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2ap_ecgi->plmn_identity))
    {
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_ecgi->eutran_cell_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_ecgi->eutran_cell_id[loop], "eutran_cell_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_subframe_allocation_len
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_subframe_allocation != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_subframe_allocation->bitmask);

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_subframe_allocation->oneframe);
    }

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_subframe_allocation->fourframe);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_subframe_allocation
(
    U8  **pp_buffer,
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_subframe_allocation != PNULL);

    /* This function composes x2ap_subframe_allocation */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_subframe_allocation->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_subframe_allocation->bitmask);

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->oneframe); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_subframe_allocation->oneframe[loop], "oneframe[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->fourframe); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_subframe_allocation->fourframe[loop], "fourframe[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_mbsfn_info_len
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mbsfn_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_mbsfn_info->radioframe_alloc_period);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_mbsfn_info->radioframe_alloc_offset);

    /* Get length of IE */
    length += rrc_il_get_x2ap_subframe_allocation_len(&p_x2ap_mbsfn_info->subframe_alloc);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_mbsfn_info
(
    U8  **pp_buffer,
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mbsfn_info != PNULL);

    /* This function composes x2ap_mbsfn_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_period > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_period] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_mbsfn_info->radioframe_alloc_period);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_mbsfn_info->radioframe_alloc_period, "radioframe_alloc_period");
    *pp_buffer += sizeof(p_x2ap_mbsfn_info->radioframe_alloc_period);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_offset > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_offset] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_mbsfn_info->radioframe_alloc_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_mbsfn_info->radioframe_alloc_offset, "radioframe_alloc_offset");
    *pp_buffer += sizeof(p_x2ap_mbsfn_info->radioframe_alloc_offset);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_subframe_allocation(pp_buffer, &p_x2ap_mbsfn_info->subframe_alloc))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_mbsfn_subframe_info_len
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mbsfn_subframe_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_mbsfn_subframe_info->num_mbsfn);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_subframe_info->num_mbsfn > MAX_NUM_MBSFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be less than"
            " or equal to MAX_NUM_MBSFN. Incorrect value %d received.", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            length += rrc_il_get_x2ap_mbsfn_info_len(&p_x2ap_mbsfn_subframe_info->mbsfn_info[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_mbsfn_subframe_info
(
    U8  **pp_buffer,
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mbsfn_subframe_info != PNULL);

    /* This function composes x2ap_mbsfn_subframe_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_subframe_info->num_mbsfn > MAX_NUM_MBSFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be less than"
            " or equal to MAX_NUM_MBSFN. Incorrect value %u received.", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_mbsfn_subframe_info->num_mbsfn, "num_mbsfn");
    *pp_buffer += sizeof(p_x2ap_mbsfn_subframe_info->num_mbsfn);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_mbsfn_info(pp_buffer, &p_x2ap_mbsfn_subframe_info->mbsfn_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_mbms_cell_info_len
(
    mbms_cell_info_t *p_mbms_cell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_mbms_cell_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_mbms_cell_info->bitmask);

    /* Get length of IE */
    length += rrc_il_get_x2ap_ecgi_len(&p_mbms_cell_info->cell_id);

    /* Optional element */
    if(p_mbms_cell_info->bitmask & RRM_X2AP_MBMS_CONFIG_APPLY_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_mbsfn_subframe_info_len(&p_mbms_cell_info->mbsfn_subframe_info);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_mbms_cell_info
(
    U8  **pp_buffer,
    mbms_cell_info_t *p_mbms_cell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_mbms_cell_info != PNULL);

    /* This function composes mbms_cell_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_mbms_cell_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_mbms_cell_info->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_ecgi(pp_buffer, &p_mbms_cell_info->cell_id))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_mbms_cell_info->bitmask & RRM_X2AP_MBMS_CONFIG_APPLY_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_mbsfn_subframe_info(pp_buffer, &p_mbms_cell_info->mbsfn_subframe_info))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_x2_link_down_ind_len
(
    x2ap_rrm_x2_link_down_ind_t *p_x2ap_rrm_x2_link_down_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_x2_link_down_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_x2_link_down_ind->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_x2_link_down_ind->bitmask & X2AP_RRM_LINK_DOWN_GB_ENB_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rrm_x2_link_down_ind->peer_gb_enb_id);
    }

    /* Optional element */
    if(p_x2ap_rrm_x2_link_down_ind->bitmask & X2AP_RRM_LINK_DOWN_GB_GNB_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2_gb_gnb_id_len(&p_x2ap_rrm_x2_link_down_ind->peer_gb_gnb_id);
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_x2_link_down_ind
(
    U8  **pp_buffer,
    x2ap_rrm_x2_link_down_ind_t *p_x2ap_rrm_x2_link_down_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_x2_link_down_ind != PNULL);

    /* This function composes x2ap_rrm_x2_link_down_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_x2_link_down_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_x2_link_down_ind->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_x2_link_down_ind->bitmask & X2AP_RRM_LINK_DOWN_GB_ENB_ID_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rrm_x2_link_down_ind->peer_gb_enb_id))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_x2_link_down_ind->bitmask & X2AP_RRM_LINK_DOWN_GB_GNB_ID_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_gnb_id(pp_buffer, &p_x2ap_rrm_x2_link_down_ind->peer_gb_gnb_id))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_x2_link_down_ind
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_LINK_DOWN_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_x2_link_down_ind
(
    x2ap_rrm_x2_link_down_ind_t  *p_x2ap_rrm_x2_link_down_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_x2_link_down_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_x2_link_down_ind_len(p_x2ap_rrm_x2_link_down_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_LINK_DOWN_IND", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_LINK_DOWN_IND, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_LINK_DOWN_IND, msg_length,cell_index);

    /* Fill X2AP_RRM_LINK_DOWN_IND message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_x2_link_down_ind(&p_x2apRrm_msg, p_x2ap_rrm_x2_link_down_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
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
rrc_il_get_macro_enb_id_len
(
    macro_enb_id_t *p_macro_enb_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_macro_enb_id != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_macro_enb_id->eNB_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_macro_enb_id
(
    U8  **pp_buffer,
    macro_enb_id_t *p_macro_enb_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_macro_enb_id != PNULL);

    /* This function composes macro_enb_id */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_macro_enb_id->eNB_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_macro_enb_id->eNB_id[loop], "eNB_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_home_enb_id_len
(
    home_enb_id_t *p_home_enb_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_home_enb_id != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_home_enb_id->eNB_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_home_enb_id
(
    U8  **pp_buffer,
    home_enb_id_t *p_home_enb_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_home_enb_id != PNULL);

    /* This function composes home_enb_id */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_home_enb_id->eNB_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_home_enb_id->eNB_id[loop], "eNB_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_enb_id_info_len
(
    x2_enb_id_info_t *p_x2_enb_id_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_enb_id_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_enb_id_info->presence_bitmask);

    /* Optional element */
    if(p_x2_enb_id_info->presence_bitmask & 1)
    {

    /* Get length of IE */
    length += rrc_il_get_macro_enb_id_len(&p_x2_enb_id_info->macro_enb_id);
    }

    /* Optional element */
    if(p_x2_enb_id_info->presence_bitmask & 2)
    {

    /* Get length of IE */
    length += rrc_il_get_home_enb_id_len(&p_x2_enb_id_info->home_enb_id);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2_enb_id_info
(
    U8  **pp_buffer,
    x2_enb_id_info_t *p_x2_enb_id_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_enb_id_info != PNULL);

    /* This function composes x2_enb_id_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2_enb_id_info->presence_bitmask, "presence_bitmask");
    *pp_buffer += sizeof(p_x2_enb_id_info->presence_bitmask);

    /* Optional element */
    if(p_x2_enb_id_info->presence_bitmask & 1)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_macro_enb_id(pp_buffer, &p_x2_enb_id_info->macro_enb_id))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2_enb_id_info->presence_bitmask & 2)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_home_enb_id(pp_buffer, &p_x2_enb_id_info->home_enb_id))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_gb_enb_id_len
(
    x2_gb_enb_id_t *p_x2_gb_enb_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_gb_enb_id != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_plmn_identity_len(&p_x2_gb_enb_id->plmn_identity);

    /* Get length of IE */
    length += rrc_il_get_x2_enb_id_info_len(&p_x2_gb_enb_id->enb_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2_gb_enb_id
(
    U8  **pp_buffer,
    x2_gb_enb_id_t *p_x2_gb_enb_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_gb_enb_id != PNULL);

    /* This function composes x2_gb_enb_id */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2_gb_enb_id->plmn_identity))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_enb_id_info(pp_buffer, &p_x2_gb_enb_id->enb_id))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_gnb_id_len
(
    x2_gnb_id_t *p_x2_gnb_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_gnb_id != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2_gnb_id->x2_gnb_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2_gnb_id
(
    U8  **pp_buffer,
    x2_gnb_id_t *p_x2_gnb_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_gnb_id != PNULL);

    /* This function composes x2_gnb_id */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_gnb_id->x2_gnb_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2_gnb_id->x2_gnb_id[loop], "x2_gnb_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_gb_gnb_id_len
(
    x2_gb_gnb_id_t *p_x2_gb_gnb_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_gb_gnb_id != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_plmn_identity_len(&p_x2_gb_gnb_id->plmn_identity);

    /* Get length of IE */
    length += rrc_il_get_x2_gnb_id_len(&p_x2_gb_gnb_id->x2_en_gnb_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2_gb_gnb_id
(
    U8  **pp_buffer,
    x2_gb_gnb_id_t *p_x2_gb_gnb_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_gb_gnb_id != PNULL);

    /* This function composes x2_gb_gnb_id */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2_gb_gnb_id->plmn_identity))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gnb_id(pp_buffer, &p_x2_gb_gnb_id->x2_en_gnb_id))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_x2_link_up_ind_st_len
(
    x2ap_rrm_x2_link_up_ind_st *p_x2ap_rrm_x2_link_up_ind_st
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_x2_link_up_ind_st != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_x2_link_up_ind_st->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_x2_link_up_ind_st->bitmask & RRM_X2AP_EUTRA_CELL_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_eutra_cell_info_len(&p_x2ap_rrm_x2_link_up_ind_st->eutra_cell_info);
    }

    /* Optional element */
    if(p_x2ap_rrm_x2_link_up_ind_st->bitmask & RRM_X2AP_NR_CELL_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_nr_cell_info_len(&p_x2ap_rrm_x2_link_up_ind_st->nr_cell_info);
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_x2_link_up_ind_st
(
    U8  **pp_buffer,
    x2ap_rrm_x2_link_up_ind_st *p_x2ap_rrm_x2_link_up_ind_st
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_x2_link_up_ind_st != PNULL);

    /* This function composes x2ap_rrm_x2_link_up_ind_st */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_x2_link_up_ind_st->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_x2_link_up_ind_st->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_x2_link_up_ind_st->bitmask & RRM_X2AP_EUTRA_CELL_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_eutra_cell_info(pp_buffer, &p_x2ap_rrm_x2_link_up_ind_st->eutra_cell_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_x2_link_up_ind_st->bitmask & RRM_X2AP_NR_CELL_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_nr_cell_info(pp_buffer, &p_x2ap_rrm_x2_link_up_ind_st->nr_cell_info))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_x2_link_up_ind_st
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_LINK_UP_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_x2_link_up_ind_st
(
    x2ap_rrm_x2_link_up_ind_st  *p_x2ap_rrm_x2_link_up_ind_st,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_x2_link_up_ind_st != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_x2_link_up_ind_st_len(p_x2ap_rrm_x2_link_up_ind_st);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_LINK_UP_IND", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_LINK_UP_IND, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_LINK_UP_IND, msg_length,cell_index);

    /* Fill X2AP_RRM_LINK_UP_IND message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_x2_link_up_ind_st(&p_x2apRrm_msg, p_x2ap_rrm_x2_link_up_ind_st))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
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
rrc_il_get_x2ap_tac_len
(
    x2ap_tac_t *p_x2ap_tac
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_tac != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_tac->tac);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_tac
(
    U8  **pp_buffer,
    x2ap_tac_t *p_x2ap_tac
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_tac != PNULL);

    /* This function composes x2ap_tac */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_tac->tac); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_tac->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_broadcast_plmn_len
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_broadcast_plmn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_broadcast_plmn->num_bcast_plmn_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_broadcast_plmn->num_bcast_plmn_id < 1) || (p_x2ap_broadcast_plmn->num_bcast_plmn_id > MAX_NUM_BPLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range "
            "1 to MAX_NUM_BPLMN. Incorrect value %d received.", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            length += rrc_il_get_x2ap_plmn_identity_len(&p_x2ap_broadcast_plmn->plmn_identity[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_broadcast_plmn
(
    U8  **pp_buffer,
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_broadcast_plmn != PNULL);

    /* This function composes x2ap_broadcast_plmn */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_broadcast_plmn->num_bcast_plmn_id < 1) || (p_x2ap_broadcast_plmn->num_bcast_plmn_id > MAX_NUM_BPLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range "
            "1 to MAX_NUM_BPLMN. Incorrect value %u received.", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_broadcast_plmn->num_bcast_plmn_id, "num_bcast_plmn_id");
    *pp_buffer += sizeof(p_x2ap_broadcast_plmn->num_bcast_plmn_id);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2ap_broadcast_plmn->plmn_identity[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_earfcn_len
(
    x2ap_earfcn_t *p_x2ap_earfcn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_earfcn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_earfcn->earfcn);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_earfcn
(
    U8  **pp_buffer,
    x2ap_earfcn_t *p_x2ap_earfcn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_earfcn != PNULL);

    /* This function composes x2ap_earfcn */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_earfcn->earfcn > MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_earfcn->earfcn] should be less than"
            " or equal to MAX_EARFCN. Incorrect value %u received.", p_x2ap_earfcn->earfcn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_earfcn->earfcn, "earfcn");
    *pp_buffer += sizeof(p_x2ap_earfcn->earfcn);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_fdd_info_len
(
    x2ap_fdd_info_t *p_x2ap_fdd_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_fdd_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_earfcn_len(&p_x2ap_fdd_info->ul_earfcn);

    /* Get length of IE */
    length += rrc_il_get_x2ap_earfcn_len(&p_x2ap_fdd_info->dl_earfcn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_fdd_info->ul_trnsmission_bdwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_fdd_info->dl_trnsmission_bdwidth);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_fdd_info
(
    U8  **pp_buffer,
    x2ap_fdd_info_t *p_x2ap_fdd_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_fdd_info != PNULL);

    /* This function composes x2ap_fdd_info */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_earfcn(pp_buffer, &p_x2ap_fdd_info->ul_earfcn))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_earfcn(pp_buffer, &p_x2ap_fdd_info->dl_earfcn))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->ul_trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_fdd_info->ul_trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_fdd_info->ul_trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_fdd_info->ul_trnsmission_bdwidth, "ul_trnsmission_bdwidth");
    *pp_buffer += sizeof(p_x2ap_fdd_info->ul_trnsmission_bdwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->dl_trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_fdd_info->dl_trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_fdd_info->dl_trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_fdd_info->dl_trnsmission_bdwidth, "dl_trnsmission_bdwidth");
    *pp_buffer += sizeof(p_x2ap_fdd_info->dl_trnsmission_bdwidth);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_special_subframe_info_len
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_special_subframe_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_special_subframe_info->special_subframe_patterns);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_special_subframe_info->dl_cp);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_special_subframe_info->ul_cp);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_special_subframe_info
(
    U8  **pp_buffer,
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_special_subframe_info != PNULL);

    /* This function composes x2ap_special_subframe_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->special_subframe_patterns > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->special_subframe_patterns] should be less than"
            " or equal to 8. Incorrect value %u received.", p_x2ap_special_subframe_info->special_subframe_patterns);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_special_subframe_info->special_subframe_patterns, "special_subframe_patterns");
    *pp_buffer += sizeof(p_x2ap_special_subframe_info->special_subframe_patterns);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->dl_cp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->dl_cp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_special_subframe_info->dl_cp);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_special_subframe_info->dl_cp, "dl_cp");
    *pp_buffer += sizeof(p_x2ap_special_subframe_info->dl_cp);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->ul_cp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->ul_cp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_special_subframe_info->ul_cp);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_special_subframe_info->ul_cp, "ul_cp");
    *pp_buffer += sizeof(p_x2ap_special_subframe_info->ul_cp);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_tdd_info_len
(
    x2ap_tdd_info_t *p_x2ap_tdd_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_tdd_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_earfcn_len(&p_x2ap_tdd_info->earfcn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_tdd_info->trnsmission_bdwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_tdd_info->subframe_assn);

    /* Get length of IE */
    length += rrc_il_get_x2ap_special_subframe_info_len(&p_x2ap_tdd_info->special_subframe_info);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_tdd_info
(
    U8  **pp_buffer,
    x2ap_tdd_info_t *p_x2ap_tdd_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_tdd_info != PNULL);

    /* This function composes x2ap_tdd_info */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_earfcn(pp_buffer, &p_x2ap_tdd_info->earfcn))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_tdd_info->trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_tdd_info->trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_tdd_info->trnsmission_bdwidth, "trnsmission_bdwidth");
    *pp_buffer += sizeof(p_x2ap_tdd_info->trnsmission_bdwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->subframe_assn > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_tdd_info->subframe_assn] should be less than"
            " or equal to 6. Incorrect value %u received.", p_x2ap_tdd_info->subframe_assn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_tdd_info->subframe_assn, "subframe_assn");
    *pp_buffer += sizeof(p_x2ap_tdd_info->subframe_assn);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_special_subframe_info(pp_buffer, &p_x2ap_tdd_info->special_subframe_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_choice_eutra_mode_len
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_choice_eutra_mode != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_choice_eutra_mode->bitmask);

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_FDD)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_fdd_info_len(&p_x2ap_choice_eutra_mode->fdd_info);
    }

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_TDD)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_tdd_info_len(&p_x2ap_choice_eutra_mode->tdd_info);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_choice_eutra_mode
(
    U8  **pp_buffer,
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_choice_eutra_mode != PNULL);

    /* This function composes x2ap_choice_eutra_mode */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_choice_eutra_mode->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_choice_eutra_mode->bitmask);

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_FDD)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_fdd_info(pp_buffer, &p_x2ap_choice_eutra_mode->fdd_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_TDD)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_tdd_info(pp_buffer, &p_x2ap_choice_eutra_mode->tdd_info))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_choice_eutra_mode_info_len
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_choice_eutra_mode_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_choice_eutra_mode_len(&p_x2ap_choice_eutra_mode_info->choice_eutra_mode);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_choice_eutra_mode_info
(
    U8  **pp_buffer,
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_choice_eutra_mode_info != PNULL);

    /* This function composes x2ap_choice_eutra_mode_info */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_choice_eutra_mode(pp_buffer, &p_x2ap_choice_eutra_mode_info->choice_eutra_mode))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_prach_configuration_info_len
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_prach_configuration_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->root_sequence_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->zero_correlation_zone_config);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->high_speed_flag);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->prach_frequency_offset);

    /* Optional element */
    if(p_x2ap_prach_configuration_info->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->prach_config_index);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_prach_configuration_info
(
    U8  **pp_buffer,
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_prach_configuration_info != PNULL);

    /* This function composes x2ap_prach_configuration_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_prach_configuration_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->root_sequence_index > 837))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->root_sequence_index] should be less than"
            " or equal to 837. Incorrect value %u received.", p_x2ap_prach_configuration_info->root_sequence_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_prach_configuration_info->root_sequence_index, "root_sequence_index");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->root_sequence_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->zero_correlation_zone_config > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->zero_correlation_zone_config] should be less than"
            " or equal to 15. Incorrect value %u received.", p_x2ap_prach_configuration_info->zero_correlation_zone_config);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_prach_configuration_info->zero_correlation_zone_config, "zero_correlation_zone_config");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->zero_correlation_zone_config);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->high_speed_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->high_speed_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_prach_configuration_info->high_speed_flag);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_prach_configuration_info->high_speed_flag, "high_speed_flag");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->high_speed_flag);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_frequency_offset > 94))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_frequency_offset] should be less than"
            " or equal to 94. Incorrect value %u received.", p_x2ap_prach_configuration_info->prach_frequency_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_prach_configuration_info->prach_frequency_offset, "prach_frequency_offset");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->prach_frequency_offset);

    /* Optional element */
    if(p_x2ap_prach_configuration_info->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_config_index > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_config_index] should be less than"
            " or equal to 63. Incorrect value %u received.", p_x2ap_prach_configuration_info->prach_config_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_prach_configuration_info->prach_config_index, "prach_config_index");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->prach_config_index);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_srvd_cell_info_len
(
    x2ap_srvd_cell_info_t *p_x2ap_srvd_cell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_srvd_cell_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_srvd_cell_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_srvd_cell_info->pci);

    /* Get length of IE */
    length += rrc_il_get_x2ap_ecgi_len(&p_x2ap_srvd_cell_info->cell_id);

    /* Get length of IE */
    length += rrc_il_get_x2ap_tac_len(&p_x2ap_srvd_cell_info->track_area_code);

    /* Get length of IE */
    length += rrc_il_get_x2ap_broadcast_plmn_len(&p_x2ap_srvd_cell_info->broadcast_plmn_info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_choice_eutra_mode_info_len(&p_x2ap_srvd_cell_info->choice_eutra_mode);

    /* Optional element */
    if(p_x2ap_srvd_cell_info->bitmask & NUM_ANTENNA_PORT_BITMASK)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_srvd_cell_info->num_antenna_port);
    }

    /* Optional element */
    if(p_x2ap_srvd_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_prach_configuration_info_len(&p_x2ap_srvd_cell_info->prach_config);
    }

    /* Optional element */
    if(p_x2ap_srvd_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_mbsfn_subframe_info_len(&p_x2ap_srvd_cell_info->mbsfn_subframe_info);
    }

    /* Optional element */
    if(p_x2ap_srvd_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_srvd_cell_info->csg_identity);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_srvd_cell_info
(
    U8  **pp_buffer,
    x2ap_srvd_cell_info_t *p_x2ap_srvd_cell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_srvd_cell_info != PNULL);

    /* This function composes x2ap_srvd_cell_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_srvd_cell_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_srvd_cell_info->bitmask);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_srvd_cell_info->pci, "pci");
    *pp_buffer += sizeof(p_x2ap_srvd_cell_info->pci);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_ecgi(pp_buffer, &p_x2ap_srvd_cell_info->cell_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_tac(pp_buffer, &p_x2ap_srvd_cell_info->track_area_code))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_broadcast_plmn(pp_buffer, &p_x2ap_srvd_cell_info->broadcast_plmn_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_choice_eutra_mode_info(pp_buffer, &p_x2ap_srvd_cell_info->choice_eutra_mode))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_srvd_cell_info->bitmask & NUM_ANTENNA_PORT_BITMASK)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_srvd_cell_info->num_antenna_port > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_srvd_cell_info->num_antenna_port] should be less than"
            " or equal to 2. Incorrect value %u received.", p_x2ap_srvd_cell_info->num_antenna_port);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_srvd_cell_info->num_antenna_port, "num_antenna_port");
    *pp_buffer += sizeof(p_x2ap_srvd_cell_info->num_antenna_port);
    }

    /* Optional element */
    if(p_x2ap_srvd_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_prach_configuration_info(pp_buffer, &p_x2ap_srvd_cell_info->prach_config))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_srvd_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_mbsfn_subframe_info(pp_buffer, &p_x2ap_srvd_cell_info->mbsfn_subframe_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_srvd_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_srvd_cell_info->csg_identity); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_srvd_cell_info->csg_identity[loop], "csg_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_neighbour_info_len
(
    x2ap_neighbour_info_t *p_x2ap_neighbour_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_neighbour_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_neighbour_info->bitmask);

    /* Get length of IE */
    length += rrc_il_get_x2ap_ecgi_len(&p_x2ap_neighbour_info->ecgi);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_neighbour_info->pci);

    /* Get length of IE */
    length += rrc_il_get_x2ap_earfcn_len(&p_x2ap_neighbour_info->earfcn);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_neighbour_info
(
    U8  **pp_buffer,
    x2ap_neighbour_info_t *p_x2ap_neighbour_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_neighbour_info != PNULL);

    /* This function composes x2ap_neighbour_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_neighbour_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_neighbour_info->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_ecgi(pp_buffer, &p_x2ap_neighbour_info->ecgi))
    {
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_neighbour_info->pci, "pci");
    *pp_buffer += sizeof(p_x2ap_neighbour_info->pci);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_earfcn(pp_buffer, &p_x2ap_neighbour_info->earfcn))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_nbour_cell_arry_len
(
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nbour_cell_arry != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nbour_cell_arry->num_neighbour_cell);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_arry->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nbour_cell_arry->num_neighbour_cell] should be less than"
            " or equal to MAX_NEIGHBOURS. Incorrect value %d received.", p_x2ap_nbour_cell_arry->num_neighbour_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_arry->num_neighbour_cell; loop++)
        {
            length += rrc_il_get_x2ap_neighbour_info_len(&p_x2ap_nbour_cell_arry->neighbour_info_arr[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_nbour_cell_arry
(
    U8  **pp_buffer,
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nbour_cell_arry != PNULL);

    /* This function composes x2ap_nbour_cell_arry */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_arry->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nbour_cell_arry->num_neighbour_cell] should be less than"
            " or equal to MAX_NEIGHBOURS. Incorrect value %u received.", p_x2ap_nbour_cell_arry->num_neighbour_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nbour_cell_arry->num_neighbour_cell, "num_neighbour_cell");
    *pp_buffer += sizeof(p_x2ap_nbour_cell_arry->num_neighbour_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_arry->num_neighbour_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_neighbour_info(pp_buffer, &p_x2ap_nbour_cell_arry->neighbour_info_arr[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_srvd_cell_arry_len
(
    x2ap_srvd_cell_arry_t *p_x2ap_srvd_cell_arry
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_srvd_cell_arry != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_srvd_cell_info_len(&p_x2ap_srvd_cell_arry->served_cell_info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nbour_cell_arry_len(&p_x2ap_srvd_cell_arry->nbour_cell_info);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_srvd_cell_arry
(
    U8  **pp_buffer,
    x2ap_srvd_cell_arry_t *p_x2ap_srvd_cell_arry
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_srvd_cell_arry != PNULL);

    /* This function composes x2ap_srvd_cell_arry */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_srvd_cell_info(pp_buffer, &p_x2ap_srvd_cell_arry->served_cell_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nbour_cell_arry(pp_buffer, &p_x2ap_srvd_cell_arry->nbour_cell_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_cell_info_len
(
    x2ap_rrm_cell_info_t *p_x2ap_rrm_cell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_cell_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_cell_info->served_cell_id);

    /* Get length of IE */
    length += rrc_il_get_x2ap_srvd_cell_arry_len(&p_x2ap_rrm_cell_info->ngh_dir_cell_arr);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_cell_info
(
    U8  **pp_buffer,
    x2ap_rrm_cell_info_t *p_x2ap_rrm_cell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_cell_info != PNULL);

    /* This function composes x2ap_rrm_cell_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_cell_info->served_cell_id > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_cell_info->served_cell_id] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_x2ap_rrm_cell_info->served_cell_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_cell_info->served_cell_id, "served_cell_id");
    *pp_buffer += sizeof(p_x2ap_rrm_cell_info->served_cell_id);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_srvd_cell_arry(pp_buffer, &p_x2ap_rrm_cell_info->ngh_dir_cell_arr))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_gu_group_id_len
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_gu_group_id != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_plmn_identity_len(&p_x2ap_gu_group_id->plmn_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_gu_group_id->mme_group_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_gu_group_id
(
    U8  **pp_buffer,
    x2ap_gu_group_id_t *p_x2ap_gu_group_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_gu_group_id != PNULL);

    /* This function composes x2ap_gu_group_id */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2ap_gu_group_id->plmn_id))
    {
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_gu_group_id->mme_group_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_gu_group_id->mme_group_id[loop], "mme_group_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_eutra_cell_info_len
(
    x2ap_rrm_eutra_cell_info_t *p_x2ap_rrm_eutra_cell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_eutra_cell_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rrm_eutra_cell_info->global_enb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_eutra_cell_info->num_served_cell);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_eutra_cell_info->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_eutra_cell_info->num_served_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_rrm_eutra_cell_info->num_served_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_eutra_cell_info->num_served_cell; loop++)
        {
            length += rrc_il_get_x2ap_rrm_cell_info_len(&p_x2ap_rrm_eutra_cell_info->cell_info[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_eutra_cell_info->num_gu_group_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_eutra_cell_info->num_gu_group_id > MAX_ENB_POOLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_eutra_cell_info->num_gu_group_id] should be less than"
            " or equal to MAX_ENB_POOLS. Incorrect value %d received.", p_x2ap_rrm_eutra_cell_info->num_gu_group_id);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_eutra_cell_info->num_gu_group_id; loop++)
        {
            length += rrc_il_get_x2ap_gu_group_id_len(&p_x2ap_rrm_eutra_cell_info->group_id_arr[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_eutra_cell_info
(
    U8  **pp_buffer,
    x2ap_rrm_eutra_cell_info_t *p_x2ap_rrm_eutra_cell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_eutra_cell_info != PNULL);

    /* This function composes x2ap_rrm_eutra_cell_info */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rrm_eutra_cell_info->global_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_eutra_cell_info->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_eutra_cell_info->num_served_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_eutra_cell_info->num_served_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_eutra_cell_info->num_served_cell, "num_served_cell");
    *pp_buffer += sizeof(p_x2ap_rrm_eutra_cell_info->num_served_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_eutra_cell_info->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_cell_info(pp_buffer, &p_x2ap_rrm_eutra_cell_info->cell_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_eutra_cell_info->num_gu_group_id > MAX_ENB_POOLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_eutra_cell_info->num_gu_group_id] should be less than"
            " or equal to MAX_ENB_POOLS. Incorrect value %u received.", p_x2ap_rrm_eutra_cell_info->num_gu_group_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_eutra_cell_info->num_gu_group_id, "num_gu_group_id");
    *pp_buffer += sizeof(p_x2ap_rrm_eutra_cell_info->num_gu_group_id);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_eutra_cell_info->num_gu_group_id; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_gu_group_id(pp_buffer, &p_x2ap_rrm_eutra_cell_info->group_id_arr[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_nr_ecgi_len
(
    x2ap_nr_ecgi_t *p_x2ap_nr_ecgi
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nr_ecgi != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_plmn_identity_len(&p_x2ap_nr_ecgi->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_nr_ecgi->nr_cell_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_nr_ecgi
(
    U8  **pp_buffer,
    x2ap_nr_ecgi_t *p_x2ap_nr_ecgi
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_ecgi != PNULL);

    /* This function composes x2ap_nr_ecgi */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2ap_nr_ecgi->plmn_identity))
    {
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_nr_ecgi->nr_cell_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_nr_ecgi->nr_cell_id[loop], "nr_cell_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_nr_freq_band_info_len
(
    x2ap_nr_freq_band_info_t *p_x2ap_nr_freq_band_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nr_freq_band_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_freq_band_info->freq_band_indicator_nr);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_freq_band_info->num_supported_sul_band);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_x2ap_nr_freq_band_info->num_supported_sul_band * sizeof(p_x2ap_nr_freq_band_info->supported_sul_freq_band_indicator_nr[0]));

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_nr_freq_band_info
(
    U8  **pp_buffer,
    x2ap_nr_freq_band_info_t *p_x2ap_nr_freq_band_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_freq_band_info != PNULL);

    /* This function composes x2ap_nr_freq_band_info */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_nr_freq_band_info->freq_band_indicator_nr < 1) || (p_x2ap_nr_freq_band_info->freq_band_indicator_nr > NR_MAX_FREQ_BAND_INDICATOR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_freq_band_info->freq_band_indicator_nr] should be in range "
            "1 to NR_MAX_FREQ_BAND_INDICATOR. Incorrect value %u received.", p_x2ap_nr_freq_band_info->freq_band_indicator_nr);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_nr_freq_band_info->freq_band_indicator_nr, "freq_band_indicator_nr");
    *pp_buffer += sizeof(p_x2ap_nr_freq_band_info->freq_band_indicator_nr);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_freq_band_info->num_supported_sul_band > NR_MAX_FREQ_BANDS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_freq_band_info->num_supported_sul_band] should be less than"
            " or equal to NR_MAX_FREQ_BANDS. Incorrect value %u received.", p_x2ap_nr_freq_band_info->num_supported_sul_band);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_nr_freq_band_info->num_supported_sul_band, "num_supported_sul_band");
    *pp_buffer += sizeof(p_x2ap_nr_freq_band_info->num_supported_sul_band);

    /* Compose OCTET_STRING VARIABLE of basic type elements with BOTH boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nr_freq_band_info->num_supported_sul_band; loop++)
        {
            if ((p_x2ap_nr_freq_band_info->supported_sul_freq_band_indicator_nr[loop] < 1) || (p_x2ap_nr_freq_band_info->supported_sul_freq_band_indicator_nr[loop] > NR_MAX_FREQ_BAND_INDICATOR))
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_x2ap_nr_freq_band_info->supported_sul_freq_band_indicator_nr[%u] should be in "
                    "range 1 to NR_MAX_FREQ_BAND_INDICATOR. "
                    "Incorrect value %d received.", loop, p_x2ap_nr_freq_band_info->supported_sul_freq_band_indicator_nr[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nr_freq_band_info->supported_sul_freq_band_indicator_nr[loop], "supported_sul_freq_band_indicator_nr[]");
            *pp_buffer += sizeof(U16);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_nr_tx_bandwidth_len
(
    x2ap_nr_tx_bandwidth_t *p_x2ap_nr_tx_bandwidth
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nr_tx_bandwidth != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_tx_bandwidth->nr_scs);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_tx_bandwidth->nr_nrb);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_nr_tx_bandwidth
(
    U8  **pp_buffer,
    x2ap_nr_tx_bandwidth_t *p_x2ap_nr_tx_bandwidth
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_tx_bandwidth != PNULL);

    /* This function composes x2ap_nr_tx_bandwidth */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_tx_bandwidth->nr_scs > MAX_NRCS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_tx_bandwidth->nr_scs] should be less than"
            " or equal to MAX_NRCS. Incorrect value %u received.", p_x2ap_nr_tx_bandwidth->nr_scs);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_nr_tx_bandwidth->nr_scs, "nr_scs");
    *pp_buffer += sizeof(p_x2ap_nr_tx_bandwidth->nr_scs);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_tx_bandwidth->nr_nrb > X2AP_NRB_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_tx_bandwidth->nr_nrb] should be less than"
            " or equal to X2AP_NRB_MAX. Incorrect value %u received.", p_x2ap_nr_tx_bandwidth->nr_nrb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_nr_tx_bandwidth->nr_nrb, "nr_nrb");
    *pp_buffer += sizeof(p_x2ap_nr_tx_bandwidth->nr_nrb);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_sul_information_len
(
    x2ap_sul_information_t *p_x2ap_sul_information
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_sul_information != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sul_information->nr_sul_arfcn);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_tx_bandwidth_len(&p_x2ap_sul_information->nr_tx_band_width);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_sul_information
(
    U8  **pp_buffer,
    x2ap_sul_information_t *p_x2ap_sul_information
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_sul_information != PNULL);

    /* This function composes x2ap_sul_information */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_sul_information->nr_sul_arfcn, "nr_sul_arfcn");
    *pp_buffer += sizeof(p_x2ap_sul_information->nr_sul_arfcn);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_tx_bandwidth(pp_buffer, &p_x2ap_sul_information->nr_tx_band_width))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_nr_freq_info_len
(
    x2ap_nr_freq_info_t *p_x2ap_nr_freq_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nr_freq_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_freq_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_freq_info->nr_earfcn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_freq_info->num_freq_bands);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_nr_freq_info->num_freq_bands < 1) || (p_x2ap_nr_freq_info->num_freq_bands > NR_MAX_FREQ_BANDS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_freq_info->num_freq_bands] should be in range "
            "1 to NR_MAX_FREQ_BANDS. Incorrect value %d received.", p_x2ap_nr_freq_info->num_freq_bands);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nr_freq_info->num_freq_bands; loop++)
        {
            length += rrc_il_get_x2ap_nr_freq_band_info_len(&p_x2ap_nr_freq_info->freq_band_info[loop]);
        }
    }

    /* Optional element */
    if(p_x2ap_nr_freq_info->bitmask & X2AP_NR_ARFCN_SUL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_freq_info->nr_earfcn_sul);
    }

    /* Optional element */
    if(p_x2ap_nr_freq_info->bitmask & X2AP_SUL_INFORMATION_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_sul_information_len(&p_x2ap_nr_freq_info->nr_sul_information);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_nr_freq_info
(
    U8  **pp_buffer,
    x2ap_nr_freq_info_t *p_x2ap_nr_freq_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_freq_info != PNULL);

    /* This function composes x2ap_nr_freq_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nr_freq_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_nr_freq_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_freq_info->nr_earfcn > NR_MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_freq_info->nr_earfcn] should be less than"
            " or equal to NR_MAX_EARFCN. Incorrect value %u received.", p_x2ap_nr_freq_info->nr_earfcn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_nr_freq_info->nr_earfcn, "nr_earfcn");
    *pp_buffer += sizeof(p_x2ap_nr_freq_info->nr_earfcn);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_nr_freq_info->num_freq_bands < 1) || (p_x2ap_nr_freq_info->num_freq_bands > NR_MAX_FREQ_BANDS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_freq_info->num_freq_bands] should be in range "
            "1 to NR_MAX_FREQ_BANDS. Incorrect value %u received.", p_x2ap_nr_freq_info->num_freq_bands);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_nr_freq_info->num_freq_bands, "num_freq_bands");
    *pp_buffer += sizeof(p_x2ap_nr_freq_info->num_freq_bands);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nr_freq_info->num_freq_bands; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_nr_freq_band_info(pp_buffer, &p_x2ap_nr_freq_info->freq_band_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Optional element */
    if(p_x2ap_nr_freq_info->bitmask & X2AP_NR_ARFCN_SUL_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_freq_info->nr_earfcn_sul > NR_MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_freq_info->nr_earfcn_sul] should be less than"
            " or equal to NR_MAX_EARFCN. Incorrect value %u received.", p_x2ap_nr_freq_info->nr_earfcn_sul);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_nr_freq_info->nr_earfcn_sul, "nr_earfcn_sul");
    *pp_buffer += sizeof(p_x2ap_nr_freq_info->nr_earfcn_sul);
    }

    /* Optional element */
    if(p_x2ap_nr_freq_info->bitmask & X2AP_SUL_INFORMATION_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_sul_information(pp_buffer, &p_x2ap_nr_freq_info->nr_sul_information))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_nr_fdd_into_len
(
    x2ap_nr_fdd_into_t *p_x2ap_nr_fdd_into
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nr_fdd_into != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_freq_info_len(&p_x2ap_nr_fdd_into->nr_ul_freq_info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_freq_info_len(&p_x2ap_nr_fdd_into->nr_dl_freq_info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_tx_bandwidth_len(&p_x2ap_nr_fdd_into->nr_ul_tx_bdwidth);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_tx_bandwidth_len(&p_x2ap_nr_fdd_into->nr_dl_tx_bdwidth);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_nr_fdd_into
(
    U8  **pp_buffer,
    x2ap_nr_fdd_into_t *p_x2ap_nr_fdd_into
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_fdd_into != PNULL);

    /* This function composes x2ap_nr_fdd_into */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_freq_info(pp_buffer, &p_x2ap_nr_fdd_into->nr_ul_freq_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_freq_info(pp_buffer, &p_x2ap_nr_fdd_into->nr_dl_freq_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_tx_bandwidth(pp_buffer, &p_x2ap_nr_fdd_into->nr_ul_tx_bdwidth))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_tx_bandwidth(pp_buffer, &p_x2ap_nr_fdd_into->nr_dl_tx_bdwidth))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_nr_tdd_into_len
(
    x2ap_nr_tdd_into_t *p_x2ap_nr_tdd_into
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nr_tdd_into != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_freq_info_len(&p_x2ap_nr_tdd_into->nr_freq_info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_tx_bandwidth_len(&p_x2ap_nr_tdd_into->nr_tx_bandwidth);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_nr_tdd_into
(
    U8  **pp_buffer,
    x2ap_nr_tdd_into_t *p_x2ap_nr_tdd_into
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_tdd_into != PNULL);

    /* This function composes x2ap_nr_tdd_into */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_freq_info(pp_buffer, &p_x2ap_nr_tdd_into->nr_freq_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_tx_bandwidth(pp_buffer, &p_x2ap_nr_tdd_into->nr_tx_bandwidth))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_choice_nr_mode_info_len
(
    x2ap_choice_nr_mode_info_t *p_x2ap_choice_nr_mode_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_choice_nr_mode_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_choice_nr_mode_info->bitmask);

    /* Optional element */
    if(p_x2ap_choice_nr_mode_info->bitmask & X2AP_NR_MODE_FDD_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_fdd_into_len(&p_x2ap_choice_nr_mode_info->nr_fdd_info);
    }

    /* Optional element */
    if(p_x2ap_choice_nr_mode_info->bitmask & X2AP_NR_MODE_TDD_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_tdd_into_len(&p_x2ap_choice_nr_mode_info->nr_tdd_info);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_choice_nr_mode_info
(
    U8  **pp_buffer,
    x2ap_choice_nr_mode_info_t *p_x2ap_choice_nr_mode_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_choice_nr_mode_info != PNULL);

    /* This function composes x2ap_choice_nr_mode_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_choice_nr_mode_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_choice_nr_mode_info->bitmask);

    /* Optional element */
    if(p_x2ap_choice_nr_mode_info->bitmask & X2AP_NR_MODE_FDD_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_fdd_into(pp_buffer, &p_x2ap_choice_nr_mode_info->nr_fdd_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_choice_nr_mode_info->bitmask & X2AP_NR_MODE_TDD_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_tdd_into(pp_buffer, &p_x2ap_choice_nr_mode_info->nr_tdd_info))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_periodicity_and_offset_len
(
    x2ap_periodicity_and_offset_t *p_x2ap_periodicity_and_offset
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_periodicity_and_offset != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_periodicity_and_offset->bitmask);

    /* Optional element */
    if(p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF5_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_periodicity_and_offset->sf5);
    }

    /* Optional element */
    if(p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF10_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_periodicity_and_offset->sf10);
    }

    /* Optional element */
    if(p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF20_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_periodicity_and_offset->sf20);
    }

    /* Optional element */
    if(p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF40_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_periodicity_and_offset->sf40);
    }

    /* Optional element */
    if(p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF80_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_periodicity_and_offset->sf80);
    }

    /* Optional element */
    if(p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF160_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_periodicity_and_offset->sf160);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_periodicity_and_offset
(
    U8  **pp_buffer,
    x2ap_periodicity_and_offset_t *p_x2ap_periodicity_and_offset
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_periodicity_and_offset != PNULL);

    /* This function composes x2ap_periodicity_and_offset */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_periodicity_and_offset->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_periodicity_and_offset->bitmask);

    /* Optional element */
    if(p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF5_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_periodicity_and_offset->sf5 > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_periodicity_and_offset->sf5] should be less than"
            " or equal to 4. Incorrect value %u received.", p_x2ap_periodicity_and_offset->sf5);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_periodicity_and_offset->sf5, "sf5");
    *pp_buffer += sizeof(p_x2ap_periodicity_and_offset->sf5);
    }

    /* Optional element */
    if(p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF10_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_periodicity_and_offset->sf10 > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_periodicity_and_offset->sf10] should be less than"
            " or equal to 9. Incorrect value %u received.", p_x2ap_periodicity_and_offset->sf10);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_periodicity_and_offset->sf10, "sf10");
    *pp_buffer += sizeof(p_x2ap_periodicity_and_offset->sf10);
    }

    /* Optional element */
    if(p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF20_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_periodicity_and_offset->sf20 > 19))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_periodicity_and_offset->sf20] should be less than"
            " or equal to 19. Incorrect value %u received.", p_x2ap_periodicity_and_offset->sf20);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_periodicity_and_offset->sf20, "sf20");
    *pp_buffer += sizeof(p_x2ap_periodicity_and_offset->sf20);
    }

    /* Optional element */
    if(p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF40_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_periodicity_and_offset->sf40 > 39))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_periodicity_and_offset->sf40] should be less than"
            " or equal to 39. Incorrect value %u received.", p_x2ap_periodicity_and_offset->sf40);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_periodicity_and_offset->sf40, "sf40");
    *pp_buffer += sizeof(p_x2ap_periodicity_and_offset->sf40);
    }

    /* Optional element */
    if(p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF80_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_periodicity_and_offset->sf80 > 79))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_periodicity_and_offset->sf80] should be less than"
            " or equal to 79. Incorrect value %u received.", p_x2ap_periodicity_and_offset->sf80);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_periodicity_and_offset->sf80, "sf80");
    *pp_buffer += sizeof(p_x2ap_periodicity_and_offset->sf80);
    }

    /* Optional element */
    if(p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF160_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_periodicity_and_offset->sf160 > 159))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_periodicity_and_offset->sf160] should be less than"
            " or equal to 159. Incorrect value %u received.", p_x2ap_periodicity_and_offset->sf160);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_periodicity_and_offset->sf160, "sf160");
    *pp_buffer += sizeof(p_x2ap_periodicity_and_offset->sf160);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_ssb_mtc_len
(
    x2ap_ssb_mtc_t *p_x2ap_ssb_mtc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_ssb_mtc != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_periodicity_and_offset_len(&p_x2ap_ssb_mtc->x2ap_periodicity_and_offset);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_ssb_mtc->x2ap_mtc_duration);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_ssb_mtc
(
    U8  **pp_buffer,
    x2ap_ssb_mtc_t *p_x2ap_ssb_mtc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_ssb_mtc != PNULL);

    /* This function composes x2ap_ssb_mtc */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_periodicity_and_offset(pp_buffer, &p_x2ap_ssb_mtc->x2ap_periodicity_and_offset))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_ssb_mtc->x2ap_mtc_duration > X2AP_DURATION_SF5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_ssb_mtc->x2ap_mtc_duration] should be less than"
            " or equal to X2AP_DURATION_SF5. Incorrect value %u received.", p_x2ap_ssb_mtc->x2ap_mtc_duration);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_ssb_mtc->x2ap_mtc_duration, "x2ap_mtc_duration");
    *pp_buffer += sizeof(p_x2ap_ssb_mtc->x2ap_mtc_duration);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_measurement_slots_len
(
    x2ap_measurement_slots_t *p_x2ap_measurement_slots
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_measurement_slots != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_measurement_slots->numbits);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_x2ap_measurement_slots->numbits * sizeof(p_x2ap_measurement_slots->meas_slots[0]));

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_measurement_slots
(
    U8  **pp_buffer,
    x2ap_measurement_slots_t *p_x2ap_measurement_slots
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_measurement_slots != PNULL);

    /* This function composes x2ap_measurement_slots */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_measurement_slots->numbits < 1) || (p_x2ap_measurement_slots->numbits > 80))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_measurement_slots->numbits] should be in range "
            "1 to 80. Incorrect value %u received.", p_x2ap_measurement_slots->numbits);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_measurement_slots->numbits, "numbits");
    *pp_buffer += sizeof(p_x2ap_measurement_slots->numbits);

    /* Compose OCTET_STRING VARIABLE of basic type elements with HIGHER boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_measurement_slots->numbits; loop++)
        {
            if (p_x2ap_measurement_slots->meas_slots[loop] > 1)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_x2ap_measurement_slots->meas_slots[%u] should be less"
                    " than or equal to 1. "
                    "Incorrect value %d received.", loop, p_x2ap_measurement_slots->meas_slots[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_measurement_slots->meas_slots[loop], "meas_slots[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_ss_rssi_measurement_len
(
    x2ap_ss_rssi_measurement_t *p_x2ap_ss_rssi_measurement
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_ss_rssi_measurement != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_measurement_slots_len(&p_x2ap_ss_rssi_measurement->x2ap_measurement_slots);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_ss_rssi_measurement->end_synmbol);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_ss_rssi_measurement
(
    U8  **pp_buffer,
    x2ap_ss_rssi_measurement_t *p_x2ap_ss_rssi_measurement
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_ss_rssi_measurement != PNULL);

    /* This function composes x2ap_ss_rssi_measurement */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_measurement_slots(pp_buffer, &p_x2ap_ss_rssi_measurement->x2ap_measurement_slots))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_ss_rssi_measurement->end_synmbol > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_ss_rssi_measurement->end_synmbol] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_ss_rssi_measurement->end_synmbol);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_ss_rssi_measurement->end_synmbol, "end_synmbol");
    *pp_buffer += sizeof(p_x2ap_ss_rssi_measurement->end_synmbol);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_meas_timing_config_len
(
    x2ap_meas_timing_config_t *p_x2ap_meas_timing_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_meas_timing_config != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_meas_timing_config->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_meas_timing_config->carrier_freq);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_meas_timing_config->ssb_subcarrier_spacing);

    /* Get length of IE */
    length += rrc_il_get_x2ap_ssb_mtc_len(&p_x2ap_meas_timing_config->ssb_meas_timimg_configuration);

    /* Optional element */
    if(p_x2ap_meas_timing_config->bitmask & X2AP_MEAS_SS_RSSI_MEASUREMENT_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_ss_rssi_measurement_len(&p_x2ap_meas_timing_config->ss_rssi_measurement);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_meas_timing_config
(
    U8  **pp_buffer,
    x2ap_meas_timing_config_t *p_x2ap_meas_timing_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_meas_timing_config != PNULL);

    /* This function composes x2ap_meas_timing_config */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_meas_timing_config->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_meas_timing_config->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_meas_timing_config->carrier_freq > NR_MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_meas_timing_config->carrier_freq] should be less than"
            " or equal to NR_MAX_EARFCN. Incorrect value %u received.", p_x2ap_meas_timing_config->carrier_freq);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_meas_timing_config->carrier_freq, "carrier_freq");
    *pp_buffer += sizeof(p_x2ap_meas_timing_config->carrier_freq);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_meas_timing_config->ssb_subcarrier_spacing > X2AP_KHZ_240))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_meas_timing_config->ssb_subcarrier_spacing] should be less than"
            " or equal to X2AP_KHZ_240. Incorrect value %u received.", p_x2ap_meas_timing_config->ssb_subcarrier_spacing);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_meas_timing_config->ssb_subcarrier_spacing, "ssb_subcarrier_spacing");
    *pp_buffer += sizeof(p_x2ap_meas_timing_config->ssb_subcarrier_spacing);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_ssb_mtc(pp_buffer, &p_x2ap_meas_timing_config->ssb_meas_timimg_configuration))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_meas_timing_config->bitmask & X2AP_MEAS_SS_RSSI_MEASUREMENT_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_ss_rssi_measurement(pp_buffer, &p_x2ap_meas_timing_config->ss_rssi_measurement))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_five_gs_tac_len
(
    x2ap_five_gs_tac_t *p_x2ap_five_gs_tac
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_five_gs_tac != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_five_gs_tac->tac);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_five_gs_tac
(
    U8  **pp_buffer,
    x2ap_five_gs_tac_t *p_x2ap_five_gs_tac
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_five_gs_tac != PNULL);

    /* This function composes x2ap_five_gs_tac */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_five_gs_tac->tac); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_five_gs_tac->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_srvd_nr_cell_info_len
(
    x2ap_rrm_srvd_nr_cell_info_t *p_x2ap_rrm_srvd_nr_cell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_srvd_nr_cell_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_srvd_nr_cell_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_srvd_nr_cell_info->nr_pci);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_ecgi_len(&p_x2ap_rrm_srvd_nr_cell_info->nr_cell_id);

    /* Get length of IE */
    length += rrc_il_get_x2ap_broadcast_plmn_len(&p_x2ap_rrm_srvd_nr_cell_info->broadcast_plmn_info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_choice_nr_mode_info_len(&p_x2ap_rrm_srvd_nr_cell_info->choice_nr_mode);

#ifdef LINUX_PC_TEST
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len * sizeof(p_x2ap_rrm_srvd_nr_cell_info->measurement_timing_config_asn_buff[0]));

#else
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count < 1) || (p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count > RRC_MAX_MEAS_FREQ_MN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count] should be in range "
            "1 to RRC_MAX_MEAS_FREQ_MN. Incorrect value %d received.", p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count);
        return RRC_FAILURE;
    }
    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count; loop++)
        {
            length += rrc_il_get_x2ap_meas_timing_config_len(&p_x2ap_rrm_srvd_nr_cell_info->x2ap_meas_timing_config[loop]);
        }
    }
#endif    

    /* Optional element */
    if(p_x2ap_rrm_srvd_nr_cell_info->bitmask & X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_five_gs_tac_len(&p_x2ap_rrm_srvd_nr_cell_info->fiveGS_track_area_code);
    }

    /* Optional element */
    if(p_x2ap_rrm_srvd_nr_cell_info->bitmask & X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_tac_len(&p_x2ap_rrm_srvd_nr_cell_info->configured_tac);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_srvd_nr_cell_info
(
    U8  **pp_buffer,
    x2ap_rrm_srvd_nr_cell_info_t *p_x2ap_rrm_srvd_nr_cell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_srvd_nr_cell_info != PNULL);

    /* This function composes x2ap_rrm_srvd_nr_cell_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_srvd_nr_cell_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_srvd_nr_cell_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_srvd_nr_cell_info->nr_pci > NR_MAX_PCI))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_srvd_nr_cell_info->nr_pci] should be less than"
            " or equal to NR_MAX_PCI. Incorrect value %u received.", p_x2ap_rrm_srvd_nr_cell_info->nr_pci);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_srvd_nr_cell_info->nr_pci, "nr_pci");
    *pp_buffer += sizeof(p_x2ap_rrm_srvd_nr_cell_info->nr_pci);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_ecgi(pp_buffer, &p_x2ap_rrm_srvd_nr_cell_info->nr_cell_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_broadcast_plmn(pp_buffer, &p_x2ap_rrm_srvd_nr_cell_info->broadcast_plmn_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_choice_nr_mode_info(pp_buffer, &p_x2ap_rrm_srvd_nr_cell_info->choice_nr_mode))
    {
        return RRC_FAILURE;
    }

#ifdef LINUX_PC_TEST
    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len > MAX_ASN_BUFFER))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len] should be less than"
            " or equal to MAX_ASN_BUFFER. Incorrect value %u received.", p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len, "meas_timing_config_buf_len");
    *pp_buffer += sizeof(p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_srvd_nr_cell_info->measurement_timing_config_asn_buff[loop], "measurement_timing_config_asn_buff[]");
            *pp_buffer += sizeof(U8);
        }
    }
#else
    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count < 1) || (p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count > RRC_MAX_MEAS_FREQ_MN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count] should be in range "
            "1 to RRC_MAX_MEAS_FREQ_MN. Incorrect value %u received.", p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count, "meas_timing_config_count");
    *pp_buffer += sizeof(p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_meas_timing_config(pp_buffer, &p_x2ap_rrm_srvd_nr_cell_info->x2ap_meas_timing_config[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
#endif

    /* Optional element */
    if(p_x2ap_rrm_srvd_nr_cell_info->bitmask & X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_five_gs_tac(pp_buffer, &p_x2ap_rrm_srvd_nr_cell_info->fiveGS_track_area_code))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_srvd_nr_cell_info->bitmask & X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_tac(pp_buffer, &p_x2ap_rrm_srvd_nr_cell_info->configured_tac))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_fdd_info_neigh_served_nr_cell_info_len
(
    x2ap_fdd_info_neigh_served_nr_cell_info_t *p_x2ap_fdd_info_neigh_served_nr_cell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_fdd_info_neigh_served_nr_cell_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_freq_info_len(&p_x2ap_fdd_info_neigh_served_nr_cell_info->ul_nr_freq_Info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_freq_info_len(&p_x2ap_fdd_info_neigh_served_nr_cell_info->dl_nr_freq_Info);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_fdd_info_neigh_served_nr_cell_info
(
    U8  **pp_buffer,
    x2ap_fdd_info_neigh_served_nr_cell_info_t *p_x2ap_fdd_info_neigh_served_nr_cell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_fdd_info_neigh_served_nr_cell_info != PNULL);

    /* This function composes x2ap_fdd_info_neigh_served_nr_cell_info */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_freq_info(pp_buffer, &p_x2ap_fdd_info_neigh_served_nr_cell_info->ul_nr_freq_Info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_freq_info(pp_buffer, &p_x2ap_fdd_info_neigh_served_nr_cell_info->dl_nr_freq_Info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_tdd_info_neigh_served_nr_cell_info_len
(
    x2ap_tdd_info_neigh_served_nr_cell_info_t *p_x2ap_tdd_info_neigh_served_nr_cell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_tdd_info_neigh_served_nr_cell_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_freq_info_len(&p_x2ap_tdd_info_neigh_served_nr_cell_info->nr_freq_info);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_tdd_info_neigh_served_nr_cell_info
(
    U8  **pp_buffer,
    x2ap_tdd_info_neigh_served_nr_cell_info_t *p_x2ap_tdd_info_neigh_served_nr_cell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_tdd_info_neigh_served_nr_cell_info != PNULL);

    /* This function composes x2ap_tdd_info_neigh_served_nr_cell_info */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_freq_info(pp_buffer, &p_x2ap_tdd_info_neigh_served_nr_cell_info->nr_freq_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_nr_neigh_mode_info_len
(
    x2ap_nr_neigh_mode_info_t *p_x2ap_nr_neigh_mode_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nr_neigh_mode_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_neigh_mode_info->bitmask);

    /* Optional element */
    if(p_x2ap_nr_neigh_mode_info->bitmask & X2AP_NR_NEIGHBOUR_FDD_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_fdd_info_neigh_served_nr_cell_info_len(&p_x2ap_nr_neigh_mode_info->fdd_info);
    }

    /* Optional element */
    if(p_x2ap_nr_neigh_mode_info->bitmask & X2AP_NR_NEIGHBOUR_TDD_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_tdd_info_neigh_served_nr_cell_info_len(&p_x2ap_nr_neigh_mode_info->tdd_info);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_nr_neigh_mode_info
(
    U8  **pp_buffer,
    x2ap_nr_neigh_mode_info_t *p_x2ap_nr_neigh_mode_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_neigh_mode_info != PNULL);

    /* This function composes x2ap_nr_neigh_mode_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nr_neigh_mode_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_nr_neigh_mode_info->bitmask);

    /* Optional element */
    if(p_x2ap_nr_neigh_mode_info->bitmask & X2AP_NR_NEIGHBOUR_FDD_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_fdd_info_neigh_served_nr_cell_info(pp_buffer, &p_x2ap_nr_neigh_mode_info->fdd_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_nr_neigh_mode_info->bitmask & X2AP_NR_NEIGHBOUR_TDD_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_tdd_info_neigh_served_nr_cell_info(pp_buffer, &p_x2ap_nr_neigh_mode_info->tdd_info))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_nr_neigh_info_len
(
    x2ap_nr_neigh_info_t *p_x2ap_nr_neigh_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nr_neigh_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_neigh_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_neigh_info->nr_pci);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_ecgi_len(&p_x2ap_nr_neigh_info->nr_cell_id);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_freq_info_len(&p_x2ap_nr_neigh_info->nr_freq_info);

#ifdef LINUX_PC_TEST
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_neigh_info->meas_timing_config_buf_len);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_x2ap_nr_neigh_info->meas_timing_config_buf_len * sizeof(p_x2ap_nr_neigh_info->measurement_timing_config_asn_buff[0]));

#else
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nr_neigh_info->meas_timing_config_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_nr_neigh_info->meas_timing_config_count < 1) || (p_x2ap_nr_neigh_info->meas_timing_config_count > RRC_MAX_MEAS_FREQ_MN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_neigh_info->meas_timing_config_count] should be in range "
            "1 to RRC_MAX_MEAS_FREQ_MN. Incorrect value %d received.", p_x2ap_nr_neigh_info->meas_timing_config_count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nr_neigh_info->meas_timing_config_count; loop++)
        {
            length += rrc_il_get_x2ap_meas_timing_config_len(&p_x2ap_nr_neigh_info->x2ap_meas_timing_config[loop]);
        }
    }
#endif

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_neigh_mode_info_len(&p_x2ap_nr_neigh_info->nr_neigh_Mode_Info);

    /* Optional element */
    if(p_x2ap_nr_neigh_info->bitmask & X2AP_FIVE_GS_TAC_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_five_gs_tac_len(&p_x2ap_nr_neigh_info->fivegs_tac);
    }

    /* Optional element */
    if(p_x2ap_nr_neigh_info->bitmask & X2AP_CONFIGURED_TAC_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_tac_len(&p_x2ap_nr_neigh_info->configured_tac);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_nr_neigh_info
(
    U8  **pp_buffer,
    x2ap_nr_neigh_info_t *p_x2ap_nr_neigh_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_neigh_info != PNULL);

    /* This function composes x2ap_nr_neigh_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nr_neigh_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_nr_neigh_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_neigh_info->nr_pci > NR_MAX_PCI))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_neigh_info->nr_pci] should be less than"
            " or equal to NR_MAX_PCI. Incorrect value %u received.", p_x2ap_nr_neigh_info->nr_pci);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nr_neigh_info->nr_pci, "nr_pci");
    *pp_buffer += sizeof(p_x2ap_nr_neigh_info->nr_pci);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_ecgi(pp_buffer, &p_x2ap_nr_neigh_info->nr_cell_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_freq_info(pp_buffer, &p_x2ap_nr_neigh_info->nr_freq_info))
    {
        return RRC_FAILURE;
    }

#ifdef LINUX_PC_TEST
    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_neigh_info->meas_timing_config_buf_len > MAX_ASN_BUFFER))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_neigh_info->meas_timing_config_buf_len] should be less than"
            " or equal to MAX_ASN_BUFFER. Incorrect value %u received.", p_x2ap_nr_neigh_info->meas_timing_config_buf_len);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_nr_neigh_info->meas_timing_config_buf_len, "meas_timing_config_buf_len");
    *pp_buffer += sizeof(p_x2ap_nr_neigh_info->meas_timing_config_buf_len);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nr_neigh_info->meas_timing_config_buf_len; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_nr_neigh_info->measurement_timing_config_asn_buff[loop], "measurement_timing_config_asn_buff[]");
            *pp_buffer += sizeof(U8);
        }
    }
#else
    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_nr_neigh_info->meas_timing_config_count < 1) || (p_x2ap_nr_neigh_info->meas_timing_config_count > RRC_MAX_MEAS_FREQ_MN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_neigh_info->meas_timing_config_count] should be in range "
            "1 to RRC_MAX_MEAS_FREQ_MN. Incorrect value %u received.", p_x2ap_nr_neigh_info->meas_timing_config_count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_nr_neigh_info->meas_timing_config_count, "meas_timing_config_count");
    *pp_buffer += sizeof(p_x2ap_nr_neigh_info->meas_timing_config_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nr_neigh_info->meas_timing_config_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_meas_timing_config(pp_buffer, &p_x2ap_nr_neigh_info->x2ap_meas_timing_config[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
#endif

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_neigh_mode_info(pp_buffer, &p_x2ap_nr_neigh_info->nr_neigh_Mode_Info))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_nr_neigh_info->bitmask & X2AP_FIVE_GS_TAC_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_five_gs_tac(pp_buffer, &p_x2ap_nr_neigh_info->fivegs_tac))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_nr_neigh_info->bitmask & X2AP_CONFIGURED_TAC_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_tac(pp_buffer, &p_x2ap_nr_neigh_info->configured_tac))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_nr_neigh_cell_arry_len
(
    x2ap_nr_neigh_cell_arry_t *p_x2ap_nr_neigh_cell_arry
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nr_neigh_cell_arry != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nr_neigh_info_len(&p_x2ap_nr_neigh_cell_arry->nr_neigh_info);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_nr_neigh_cell_arry
(
    U8  **pp_buffer,
    x2ap_nr_neigh_cell_arry_t *p_x2ap_nr_neigh_cell_arry
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_neigh_cell_arry != PNULL);

    /* This function composes x2ap_nr_neigh_cell_arry */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nr_neigh_info(pp_buffer, &p_x2ap_nr_neigh_cell_arry->nr_neigh_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_nr_cell_info_len
(
    x2ap_rrm_nr_cell_info_t *p_x2ap_rrm_nr_cell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_nr_cell_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_nr_cell_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_nr_cell_info->default_sgnb);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_gnb_id_len(&p_x2ap_rrm_nr_cell_info->global_gnb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_nr_cell_info->num_served_cell);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_nr_cell_info->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_nr_cell_info->num_served_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_rrm_nr_cell_info->num_served_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_nr_cell_info->num_served_cell; loop++)
        {
            length += rrc_il_get_x2ap_rrm_srvd_nr_cell_info_len(&p_x2ap_rrm_nr_cell_info->served_cell_info[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_nr_cell_info->num_neigh_cell);

    /* Optional element */
    if(p_x2ap_rrm_nr_cell_info->bitmask & NR_NEIGHBOUR_INFO_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_nr_cell_info->num_neigh_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_nr_cell_info->num_neigh_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_rrm_nr_cell_info->num_neigh_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_nr_cell_info->num_neigh_cell; loop++)
        {
            length += rrc_il_get_x2ap_nr_neigh_cell_arry_len(&p_x2ap_rrm_nr_cell_info->nr_neigh_cell_arr[loop]);
        }
    }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_nr_cell_info
(
    U8  **pp_buffer,
    x2ap_rrm_nr_cell_info_t *p_x2ap_rrm_nr_cell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_nr_cell_info != PNULL);

    /* This function composes x2ap_rrm_nr_cell_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_nr_cell_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_nr_cell_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_nr_cell_info->default_sgnb > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_nr_cell_info->default_sgnb] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_rrm_nr_cell_info->default_sgnb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_nr_cell_info->default_sgnb, "default_sgnb");
    *pp_buffer += sizeof(p_x2ap_rrm_nr_cell_info->default_sgnb);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_gnb_id(pp_buffer, &p_x2ap_rrm_nr_cell_info->global_gnb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_nr_cell_info->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_nr_cell_info->num_served_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_nr_cell_info->num_served_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_nr_cell_info->num_served_cell, "num_served_cell");
    *pp_buffer += sizeof(p_x2ap_rrm_nr_cell_info->num_served_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_nr_cell_info->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_srvd_nr_cell_info(pp_buffer, &p_x2ap_rrm_nr_cell_info->served_cell_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_nr_cell_info->num_neigh_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_nr_cell_info->num_neigh_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_nr_cell_info->num_neigh_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_nr_cell_info->num_neigh_cell, "num_neigh_cell");
    *pp_buffer += sizeof(p_x2ap_rrm_nr_cell_info->num_neigh_cell);

    /* Optional element */
    if(p_x2ap_rrm_nr_cell_info->bitmask & NR_NEIGHBOUR_INFO_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_nr_cell_info->num_neigh_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_nr_neigh_cell_arry(pp_buffer, &p_x2ap_rrm_nr_cell_info->nr_neigh_cell_arr[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_rrm_start_req_len
(
    x2ap_rrm_rsu_rrm_start_req_t *p_x2ap_rrm_rsu_rrm_start_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_start_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_start_req->report_characteristics);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_start_req->num_cell);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_rrm_start_req->num_cell < 1) || (p_x2ap_rrm_rsu_rrm_start_req->num_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_start_req->num_cell] should be in range "
            "1 to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_rrm_rsu_rrm_start_req->num_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_rrm_start_req->num_cell; loop++)
        {
            length += rrc_il_get_rrm_cell_global_id_eutra_len(&p_x2ap_rrm_rsu_rrm_start_req->cell_info[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_start_req->report_periodicity);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_start_req->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_rsu_rrm_start_req->bitmask & RRM_RSU_START_PARTIAL_SUCC_IND_IS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_start_req->partial_success_indicator);
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_rrm_start_req
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_rrm_start_req_t *p_x2ap_rrm_rsu_rrm_start_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_start_req != PNULL);

    /* This function composes x2ap_rrm_rsu_rrm_start_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_rsu_rrm_start_req->report_characteristics, "report_characteristics");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_start_req->report_characteristics);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_rrm_start_req->num_cell < 1) || (p_x2ap_rrm_rsu_rrm_start_req->num_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_start_req->num_cell] should be in range "
            "1 to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_start_req->num_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_rsu_rrm_start_req->num_cell, "num_cell");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_start_req->num_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_rrm_start_req->num_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrm_cell_global_id_eutra(pp_buffer, &p_x2ap_rrm_rsu_rrm_start_req->cell_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_rrm_start_req->report_periodicity > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_start_req->report_periodicity] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_start_req->report_periodicity);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_rrm_start_req->report_periodicity, "report_periodicity");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_start_req->report_periodicity);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_rsu_rrm_start_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_start_req->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_rsu_rrm_start_req->bitmask & RRM_RSU_START_PARTIAL_SUCC_IND_IS_PRESENT)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_rrm_start_req->partial_success_indicator, "partial_success_indicator");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_start_req->partial_success_indicator);
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_start_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_RSU_RRM_START_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_start_req
(
    x2ap_rrm_rsu_rrm_start_req_t  *p_x2ap_rrm_rsu_rrm_start_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_start_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_rsu_rrm_start_req_len(p_x2ap_rrm_rsu_rrm_start_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_RSU_RRM_START_REQ", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_RSU_RRM_START_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_RSU_RRM_START_REQ, msg_length,cell_index);

    /* Fill X2AP_RRM_RSU_RRM_START_REQ message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_rrm_start_req(&p_x2apRrm_msg, p_x2ap_rrm_rsu_rrm_start_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
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
rrc_il_get_mnc_len
(
    mnc_t *p_mnc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_mnc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_mnc->count);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_mnc->count * sizeof(p_mnc->mnc[0]));

    return length;
}

static
rrc_return_et
rrc_il_compose_mnc
(
    U8  **pp_buffer,
    mnc_t *p_mnc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_mnc != PNULL);

    /* This function composes mnc */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mnc->count < 2) || (p_mnc->count > MAX_MNC_OCTET_SIZE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mnc->count] should be in range "
            "2 to MAX_MNC_OCTET_SIZE. Incorrect value %u received.", p_mnc->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_mnc->count, "count");
    *pp_buffer += sizeof(p_mnc->count);

    /* Compose OCTET_STRING VARIABLE of basic type elements with HIGHER boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mnc->count; loop++)
        {
            if (p_mnc->mnc[loop] > 9)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_mnc->mnc[%u] should be less"
                    " than or equal to 9. "
                    "Incorrect value %d received.", loop, p_mnc->mnc[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U8(*pp_buffer, &p_mnc->mnc[loop], "mnc[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_plmn_identity_len
(
    plmn_identity_t *p_plmn_identity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_plmn_identity != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_plmn_identity->presence_bitmask);

    /* Optional element */
    if(p_plmn_identity->presence_bitmask & 1)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_plmn_identity->mcc);
    }

    /* Get length of IE */
    length += rrc_il_get_mnc_len(&p_plmn_identity->mnc);

    return length;
}

static
rrc_return_et
rrc_il_compose_plmn_identity
(
    U8  **pp_buffer,
    plmn_identity_t *p_plmn_identity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_plmn_identity != PNULL);

    /* This function composes plmn_identity */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_plmn_identity->presence_bitmask, "presence_bitmask");
    *pp_buffer += sizeof(p_plmn_identity->presence_bitmask);

    /* Optional element */
    if(p_plmn_identity->presence_bitmask & 1)
    {

    /* Compose OCTET_STRING FIXED of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_plmn_identity->mcc); loop++)
        {
            if (p_plmn_identity->mcc[loop] > 9)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_plmn_identity->mcc[%u] should be less"
                    " than or equal to 9. "
                    "Incorrect value %d received.", loop, p_plmn_identity->mcc[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U8(*pp_buffer, &p_plmn_identity->mcc[loop], "mcc[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_mnc(pp_buffer, &p_plmn_identity->mnc))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrm_cell_global_id_eutra_len
(
    rrm_cell_global_id_eutra_t *p_rrm_cell_global_id_eutra
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrm_cell_global_id_eutra != PNULL);

    /* Get length of IE */
    length += rrc_il_get_plmn_identity_len(&p_rrm_cell_global_id_eutra->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_cell_global_id_eutra->cell_identity);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrm_cell_global_id_eutra
(
    U8  **pp_buffer,
    rrm_cell_global_id_eutra_t *p_rrm_cell_global_id_eutra
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrm_cell_global_id_eutra != PNULL);

    /* This function composes rrm_cell_global_id_eutra */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_plmn_identity(pp_buffer, &p_rrm_cell_global_id_eutra->plmn_identity))
    {
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_cell_global_id_eutra->cell_identity); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrm_cell_global_id_eutra->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_enb_start_res_len
(
    x2ap_rrm_rsu_enb_start_res_t *p_x2ap_rrm_rsu_enb_start_res
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_enb_start_res != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_start_res->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_start_res->response);

    /* Optional element */
    if(p_x2ap_rrm_rsu_enb_start_res->bitmask & ENB_MEAS_ID_IS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_start_res->enb_meas_id);
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_enb_start_res->bitmask & ENB_START_RES_RRM_CAUSE_IS_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_cause_len(&p_x2ap_rrm_rsu_enb_start_res->rrm_cause);
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_enb_start_res->bitmask & ENB_ID_IS_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rrm_rsu_enb_start_res->global_enb_id);
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_enb_start_res->bitmask & ENB_START_PARTIAL_SUCCESS)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_rsu_measurement_res_len(&p_x2ap_rrm_rsu_enb_start_res->rrm_rsu_measurement_res_list);
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_enb_start_res
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_enb_start_res_t *p_x2ap_rrm_rsu_enb_start_res
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_enb_start_res != PNULL);

    /* This function composes x2ap_rrm_rsu_enb_start_res */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_rsu_enb_start_res->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_start_res->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_enb_start_res->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_start_res->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_start_res->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_enb_start_res->response, "response");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_start_res->response);

    /* Optional element */
    if(p_x2ap_rrm_rsu_enb_start_res->bitmask & ENB_MEAS_ID_IS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_enb_start_res->enb_meas_id < 1) || (p_x2ap_rrm_rsu_enb_start_res->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_start_res->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_start_res->enb_meas_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_rsu_enb_start_res->enb_meas_id, "enb_meas_id");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_start_res->enb_meas_id);
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_enb_start_res->bitmask & ENB_START_RES_RRM_CAUSE_IS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_cause(pp_buffer, &p_x2ap_rrm_rsu_enb_start_res->rrm_cause))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_enb_start_res->bitmask & ENB_ID_IS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rrm_rsu_enb_start_res->global_enb_id))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_enb_start_res->bitmask & ENB_START_PARTIAL_SUCCESS)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_measurement_res(pp_buffer, &p_x2ap_rrm_rsu_enb_start_res->rrm_rsu_measurement_res_list))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_start_res
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_RSU_ENB_START_RES message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_start_res
(
    x2ap_rrm_rsu_enb_start_res_t  *p_x2ap_rrm_rsu_enb_start_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_rsu_enb_start_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_rsu_enb_start_res_len(p_x2ap_rrm_rsu_enb_start_res);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_RSU_ENB_START_RES", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_RSU_ENB_START_RES, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_RSU_ENB_START_RES, msg_length,cell_index);

    /* Fill X2AP_RRM_RSU_ENB_START_RES message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_enb_start_res(&p_x2apRrm_msg, p_x2ap_rrm_rsu_enb_start_res))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
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
rrc_il_get_x2ap_rrm_cause_len
(
    x2ap_rrm_cause_t *p_x2ap_rrm_cause
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_cause != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_cause->type);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_cause->value);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_cause
(
    U8  **pp_buffer,
    x2ap_rrm_cause_t *p_x2ap_rrm_cause
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_cause != PNULL);

    /* This function composes x2ap_rrm_cause */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_cause->type, "type");
    *pp_buffer += sizeof(p_x2ap_rrm_cause->type);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_cause->value, "value");
    *pp_buffer += sizeof(p_x2ap_rrm_cause->value);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_measurement_failure_cause_item_len
(
    measurement_failure_cause_item_t *p_measurement_failure_cause_item
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_measurement_failure_cause_item != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_measurement_failure_cause_item->report_characteristics);

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_cause_len(&p_measurement_failure_cause_item->rrm_cause);

    return length;
}

static
rrc_return_et
rrc_il_compose_measurement_failure_cause_item
(
    U8  **pp_buffer,
    measurement_failure_cause_item_t *p_measurement_failure_cause_item
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_measurement_failure_cause_item != PNULL);

    /* This function composes measurement_failure_cause_item */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_measurement_failure_cause_item->report_characteristics, "report_characteristics");
    *pp_buffer += sizeof(p_measurement_failure_cause_item->report_characteristics);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_cause(pp_buffer, &p_measurement_failure_cause_item->rrm_cause))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rsu_measurement_res_item_len
(
    rsu_measurement_res_item_t *p_rsu_measurement_res_item
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rsu_measurement_res_item != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rsu_measurement_res_item->bitmask);

    /* Get length of IE */
    length += rrc_il_get_rrm_cell_global_id_eutra_len(&p_rsu_measurement_res_item->cell_info);

    /* Optional element */
    if(p_rsu_measurement_res_item->bitmask & X2AP_RRM_FAILED_CAUSE_NUM_ITEM_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rsu_measurement_res_item->num_measurement);
    }

    /* Optional element */
    if(p_rsu_measurement_res_item->bitmask & X2AP_RRM_FAILED_CAUSE_ITEM_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rsu_measurement_res_item->num_measurement < 1) || (p_rsu_measurement_res_item->num_measurement > MAX_ENB_MEASUREMENTS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rsu_measurement_res_item->num_measurement] should be in range "
            "1 to MAX_ENB_MEASUREMENTS. Incorrect value %d received.", p_rsu_measurement_res_item->num_measurement);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rsu_measurement_res_item->num_measurement; loop++)
        {
            length += rrc_il_get_measurement_failure_cause_item_len(&p_rsu_measurement_res_item->measurement_failure_cause_item[loop]);
        }
    }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rsu_measurement_res_item
(
    U8  **pp_buffer,
    rsu_measurement_res_item_t *p_rsu_measurement_res_item
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rsu_measurement_res_item != PNULL);

    /* This function composes rsu_measurement_res_item */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rsu_measurement_res_item->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rsu_measurement_res_item->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrm_cell_global_id_eutra(pp_buffer, &p_rsu_measurement_res_item->cell_info))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_rsu_measurement_res_item->bitmask & X2AP_RRM_FAILED_CAUSE_NUM_ITEM_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rsu_measurement_res_item->num_measurement < 1) || (p_rsu_measurement_res_item->num_measurement > MAX_ENB_MEASUREMENTS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rsu_measurement_res_item->num_measurement] should be in range "
            "1 to MAX_ENB_MEASUREMENTS. Incorrect value %u received.", p_rsu_measurement_res_item->num_measurement);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rsu_measurement_res_item->num_measurement, "num_measurement");
    *pp_buffer += sizeof(p_rsu_measurement_res_item->num_measurement);
    }

    /* Optional element */
    if(p_rsu_measurement_res_item->bitmask & X2AP_RRM_FAILED_CAUSE_ITEM_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rsu_measurement_res_item->num_measurement; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_measurement_failure_cause_item(pp_buffer, &p_rsu_measurement_res_item->measurement_failure_cause_item[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_measurement_res_len
(
    x2ap_rrm_rsu_measurement_res_t *p_x2ap_rrm_rsu_measurement_res
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_measurement_res != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_measurement_res->num_cell);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_measurement_res->num_cell < 1) || (p_x2ap_rrm_rsu_measurement_res->num_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_measurement_res->num_cell] should be in range "
            "1 to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_rrm_rsu_measurement_res->num_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_measurement_res->num_cell; loop++)
        {
            length += rrc_il_get_rsu_measurement_res_item_len(&p_x2ap_rrm_rsu_measurement_res->rsu_measurement_res_item[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_measurement_res
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_measurement_res_t *p_x2ap_rrm_rsu_measurement_res
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_measurement_res != PNULL);

    /* This function composes x2ap_rrm_rsu_measurement_res */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_measurement_res->num_cell < 1) || (p_x2ap_rrm_rsu_measurement_res->num_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_measurement_res->num_cell] should be in range "
            "1 to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_rsu_measurement_res->num_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_rsu_measurement_res->num_cell, "num_cell");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_measurement_res->num_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_measurement_res->num_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rsu_measurement_res_item(pp_buffer, &p_x2ap_rrm_rsu_measurement_res->rsu_measurement_res_item[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_enb_stop_res_len
(
    x2ap_rrm_rsu_enb_stop_res_t *p_x2ap_rrm_rsu_enb_stop_res
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_enb_stop_res != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_stop_res->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_stop_res->enb_meas_id);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rrm_rsu_enb_stop_res->peer_gb_enb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_stop_res->response);

    /* Optional element */
    if(p_x2ap_rrm_rsu_enb_stop_res->bitmask & ENB_STOP_RES_RRM_CAUSE_IS_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_cause_len(&p_x2ap_rrm_rsu_enb_stop_res->rrm_cause);
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_enb_stop_res
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_enb_stop_res_t *p_x2ap_rrm_rsu_enb_stop_res
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_enb_stop_res != PNULL);

    /* This function composes x2ap_rrm_rsu_enb_stop_res */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_enb_stop_res->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_stop_res->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_enb_stop_res->enb_meas_id < 1) || (p_x2ap_rrm_rsu_enb_stop_res->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_stop_res->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_stop_res->enb_meas_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_rsu_enb_stop_res->enb_meas_id, "enb_meas_id");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_stop_res->enb_meas_id);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rrm_rsu_enb_stop_res->peer_gb_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_enb_stop_res->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_stop_res->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_stop_res->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_enb_stop_res->response, "response");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_stop_res->response);

    /* Optional element */
    if(p_x2ap_rrm_rsu_enb_stop_res->bitmask & ENB_STOP_RES_RRM_CAUSE_IS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_cause(pp_buffer, &p_x2ap_rrm_rsu_enb_stop_res->rrm_cause))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_stop_res
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_RSU_ENB_STOP_RES message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_stop_res
(
    x2ap_rrm_rsu_enb_stop_res_t  *p_x2ap_rrm_rsu_enb_stop_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_rsu_enb_stop_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_rsu_enb_stop_res_len(p_x2ap_rrm_rsu_enb_stop_res);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_RSU_ENB_STOP_RES", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_RSU_ENB_STOP_RES, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_RSU_ENB_STOP_RES, msg_length,cell_index);

    /* Fill X2AP_RRM_RSU_ENB_STOP_RES message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_enb_stop_res(&p_x2apRrm_msg, p_x2ap_rrm_rsu_enb_stop_res))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_rrm_start_res_len
(
    x2ap_rrm_rsu_rrm_start_res_t *p_x2ap_rrm_rsu_rrm_start_res
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_start_res != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_start_res->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_start_res->enb_meas_id);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rrm_rsu_rrm_start_res->peer_gb_enb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_start_res->response);

    /* Optional element */
    if(p_x2ap_rrm_rsu_rrm_start_res->bitmask & RRM_START_RES_RRM_CAUSE_IS_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_cause_len(&p_x2ap_rrm_rsu_rrm_start_res->rrm_cause);
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_rrm_start_res->bitmask & ENB_START_PARTIAL_SUCCESS)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_rsu_measurement_res_len(&p_x2ap_rrm_rsu_rrm_start_res->rrm_rsu_measurement_res_list);
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_rrm_start_res
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_rrm_start_res_t *p_x2ap_rrm_rsu_rrm_start_res
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_start_res != PNULL);

    /* This function composes x2ap_rrm_rsu_rrm_start_res */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_rsu_rrm_start_res->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_start_res->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_rrm_start_res->enb_meas_id < 1) || (p_x2ap_rrm_rsu_rrm_start_res->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_start_res->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_start_res->enb_meas_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_rsu_rrm_start_res->enb_meas_id, "enb_meas_id");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_start_res->enb_meas_id);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rrm_rsu_rrm_start_res->peer_gb_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_rrm_start_res->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_start_res->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_start_res->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_rrm_start_res->response, "response");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_start_res->response);

    /* Optional element */
    if(p_x2ap_rrm_rsu_rrm_start_res->bitmask & RRM_START_RES_RRM_CAUSE_IS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_cause(pp_buffer, &p_x2ap_rrm_rsu_rrm_start_res->rrm_cause))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_rrm_start_res->bitmask & ENB_START_PARTIAL_SUCCESS)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_measurement_res(pp_buffer, &p_x2ap_rrm_rsu_rrm_start_res->rrm_rsu_measurement_res_list))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_start_res
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_RSU_RRM_START_RES message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_start_res
(
    x2ap_rrm_rsu_rrm_start_res_t  *p_x2ap_rrm_rsu_rrm_start_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_start_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_rsu_rrm_start_res_len(p_x2ap_rrm_rsu_rrm_start_res);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_RSU_RRM_START_RES", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_RSU_RRM_START_RES, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_RSU_RRM_START_RES, msg_length,cell_index);

    /* Fill X2AP_RRM_RSU_RRM_START_RES message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_rrm_start_res(&p_x2apRrm_msg, p_x2ap_rrm_rsu_rrm_start_res))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_rrm_stop_res_len
(
    x2ap_rrm_rsu_rrm_stop_res_t *p_x2ap_rrm_rsu_rrm_stop_res
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_stop_res != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_stop_res->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_stop_res->enb_meas_id);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rrm_rsu_rrm_stop_res->peer_gb_enb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_stop_res->response);

    /* Optional element */
    if(p_x2ap_rrm_rsu_rrm_stop_res->bitmask & RRM_STOP_RES_RRM_CAUSE_IS_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_cause_len(&p_x2ap_rrm_rsu_rrm_stop_res->rrm_cause);
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_rrm_stop_res
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_rrm_stop_res_t *p_x2ap_rrm_rsu_rrm_stop_res
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_stop_res != PNULL);

    /* This function composes x2ap_rrm_rsu_rrm_stop_res */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_rsu_rrm_stop_res->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_stop_res->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_rrm_stop_res->enb_meas_id < 1) || (p_x2ap_rrm_rsu_rrm_stop_res->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_stop_res->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_stop_res->enb_meas_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_rsu_rrm_stop_res->enb_meas_id, "enb_meas_id");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_stop_res->enb_meas_id);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rrm_rsu_rrm_stop_res->peer_gb_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_rrm_stop_res->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_stop_res->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_stop_res->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_rrm_stop_res->response, "response");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_stop_res->response);

    /* Optional element */
    if(p_x2ap_rrm_rsu_rrm_stop_res->bitmask & RRM_STOP_RES_RRM_CAUSE_IS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_cause(pp_buffer, &p_x2ap_rrm_rsu_rrm_stop_res->rrm_cause))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_stop_res
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_RSU_RRM_STOP_RES message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_stop_res
(
    x2ap_rrm_rsu_rrm_stop_res_t  *p_x2ap_rrm_rsu_rrm_stop_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_stop_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_rsu_rrm_stop_res_len(p_x2ap_rrm_rsu_rrm_stop_res);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_RSU_RRM_STOP_RES", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_RSU_RRM_STOP_RES, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_RSU_RRM_STOP_RES, msg_length,cell_index);

    /* Fill X2AP_RRM_RSU_RRM_STOP_RES message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_rrm_stop_res(&p_x2apRrm_msg, p_x2ap_rrm_rsu_rrm_stop_res))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_enb_start_req_len
(
    x2ap_rrm_rsu_enb_start_req_t *p_x2ap_rrm_rsu_enb_start_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_enb_start_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_start_req->report_characteristics);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rrm_rsu_enb_start_req->peer_gb_enb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_start_req->enb_meas_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_start_req->num_cell);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_enb_start_req->num_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_start_req->num_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_rrm_rsu_enb_start_req->num_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_enb_start_req->num_cell; loop++)
        {
            length += rrc_il_get_rrm_cell_global_id_eutra_len(&p_x2ap_rrm_rsu_enb_start_req->cell_info[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_start_req->report_periodicity);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_start_req->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_rsu_enb_start_req->bitmask & ENB_START_PARTIAL_SUCCESS)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_start_req->partial_success_indicator_present);
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_enb_start_req
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_enb_start_req_t *p_x2ap_rrm_rsu_enb_start_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_enb_start_req != PNULL);

    /* This function composes x2ap_rrm_rsu_enb_start_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_rsu_enb_start_req->report_characteristics, "report_characteristics");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_start_req->report_characteristics);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rrm_rsu_enb_start_req->peer_gb_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_enb_start_req->enb_meas_id < 1) || (p_x2ap_rrm_rsu_enb_start_req->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_start_req->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_start_req->enb_meas_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_rsu_enb_start_req->enb_meas_id, "enb_meas_id");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_start_req->enb_meas_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_enb_start_req->num_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_start_req->num_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_start_req->num_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_rsu_enb_start_req->num_cell, "num_cell");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_start_req->num_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_enb_start_req->num_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrm_cell_global_id_eutra(pp_buffer, &p_x2ap_rrm_rsu_enb_start_req->cell_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_enb_start_req->report_periodicity > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_start_req->report_periodicity] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_start_req->report_periodicity);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_enb_start_req->report_periodicity, "report_periodicity");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_start_req->report_periodicity);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_rsu_enb_start_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_start_req->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_rsu_enb_start_req->bitmask & ENB_START_PARTIAL_SUCCESS)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_enb_start_req->partial_success_indicator_present, "partial_success_indicator_present");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_start_req->partial_success_indicator_present);
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_start_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_RSU_ENB_START_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_start_req
(
    x2ap_rrm_rsu_enb_start_req_t  *p_x2ap_rrm_rsu_enb_start_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_rsu_enb_start_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_rsu_enb_start_req_len(p_x2ap_rrm_rsu_enb_start_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_RSU_ENB_START_REQ", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_RSU_ENB_START_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_RSU_ENB_START_REQ, msg_length,cell_index);

    /* Fill X2AP_RRM_RSU_ENB_START_REQ message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_enb_start_req(&p_x2apRrm_msg, p_x2ap_rrm_rsu_enb_start_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_rrm_update_len
(
    x2ap_rrm_rsu_rrm_update_t *p_x2ap_rrm_rsu_rrm_update
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_update != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_update->enb_meas_id);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rrm_rsu_rrm_update->peer_gb_enb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_update->num_reports);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_rrm_update->num_reports > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_update->num_reports] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_rrm_rsu_rrm_update->num_reports);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_rrm_update->num_reports; loop++)
        {
            length += rrc_il_get_x2ap_rrm_rsu_report_info_len(&p_x2ap_rrm_rsu_rrm_update->rsu_report[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_rrm_update
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_rrm_update_t *p_x2ap_rrm_rsu_rrm_update
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_update != PNULL);

    /* This function composes x2ap_rrm_rsu_rrm_update */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_rrm_update->enb_meas_id < 1) || (p_x2ap_rrm_rsu_rrm_update->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_update->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_update->enb_meas_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_rsu_rrm_update->enb_meas_id, "enb_meas_id");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_update->enb_meas_id);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rrm_rsu_rrm_update->peer_gb_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_rrm_update->num_reports > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_update->num_reports] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_update->num_reports);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_rsu_rrm_update->num_reports, "num_reports");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_update->num_reports);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_rrm_update->num_reports; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_report_info(pp_buffer, &p_x2ap_rrm_rsu_rrm_update->rsu_report[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_update
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_RSU_RRM_UPDATE message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_update
(
    x2ap_rrm_rsu_rrm_update_t  *p_x2ap_rrm_rsu_rrm_update,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_update != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_rsu_rrm_update_len(p_x2ap_rrm_rsu_rrm_update);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_RSU_RRM_UPDATE", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_RSU_RRM_UPDATE, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_RSU_RRM_UPDATE, msg_length,cell_index);

    /* Fill X2AP_RRM_RSU_RRM_UPDATE message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_rrm_update(&p_x2apRrm_msg, p_x2ap_rrm_rsu_rrm_update))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
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
rrc_il_get_x2ap_rrm_rsu_prb_len
(
    x2ap_rrm_rsu_prb_t *p_x2ap_rrm_rsu_prb
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_prb != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_prb->dl_gbr_prb_usage);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_prb->ul_gbr_prb_usage);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_prb->dl_non_gbr_prb_usage);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_prb->ul_non_gbr_prb_usage);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_prb->dl_total_gbr_prb_usage);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_prb->ul_total_gbr_prb_usage);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_prb
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_prb_t *p_x2ap_rrm_rsu_prb
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_prb != PNULL);

    /* This function composes x2ap_rrm_rsu_prb */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_prb->dl_gbr_prb_usage, "dl_gbr_prb_usage");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_prb->dl_gbr_prb_usage);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_prb->ul_gbr_prb_usage, "ul_gbr_prb_usage");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_prb->ul_gbr_prb_usage);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_prb->dl_non_gbr_prb_usage, "dl_non_gbr_prb_usage");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_prb->dl_non_gbr_prb_usage);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_prb->ul_non_gbr_prb_usage, "ul_non_gbr_prb_usage");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_prb->ul_non_gbr_prb_usage);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_prb->dl_total_gbr_prb_usage, "dl_total_gbr_prb_usage");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_prb->dl_total_gbr_prb_usage);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_prb->ul_total_gbr_prb_usage, "ul_total_gbr_prb_usage");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_prb->ul_total_gbr_prb_usage);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_tnl_load_len
(
    x2ap_rrm_rsu_tnl_load_t *p_x2ap_rrm_rsu_tnl_load
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_tnl_load != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_tnl_load->dl_s1_tnl_load_indicator);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_tnl_load->ul_s1_tnl_load_indicator);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_tnl_load
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_tnl_load_t *p_x2ap_rrm_rsu_tnl_load
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_tnl_load != PNULL);

    /* This function composes x2ap_rrm_rsu_tnl_load */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_tnl_load->dl_s1_tnl_load_indicator > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_tnl_load->dl_s1_tnl_load_indicator] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_rsu_tnl_load->dl_s1_tnl_load_indicator);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_tnl_load->dl_s1_tnl_load_indicator, "dl_s1_tnl_load_indicator");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_tnl_load->dl_s1_tnl_load_indicator);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_tnl_load->ul_s1_tnl_load_indicator > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_tnl_load->ul_s1_tnl_load_indicator] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_rsu_tnl_load->ul_s1_tnl_load_indicator);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_tnl_load->ul_s1_tnl_load_indicator, "ul_s1_tnl_load_indicator");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_tnl_load->ul_s1_tnl_load_indicator);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_hw_load_len
(
    x2ap_rrm_rsu_hw_load_t *p_x2ap_rrm_rsu_hw_load
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_hw_load != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_hw_load->dl_hardware_load_indicator);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_hw_load->ul_hardware_load_indicator);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_hw_load
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_hw_load_t *p_x2ap_rrm_rsu_hw_load
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_hw_load != PNULL);

    /* This function composes x2ap_rrm_rsu_hw_load */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_hw_load->dl_hardware_load_indicator > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_hw_load->dl_hardware_load_indicator] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_rsu_hw_load->dl_hardware_load_indicator);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_hw_load->dl_hardware_load_indicator, "dl_hardware_load_indicator");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_hw_load->dl_hardware_load_indicator);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_hw_load->ul_hardware_load_indicator > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_hw_load->ul_hardware_load_indicator] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_rsu_hw_load->ul_hardware_load_indicator);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_hw_load->ul_hardware_load_indicator, "ul_hardware_load_indicator");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_hw_load->ul_hardware_load_indicator);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_composite_available_capacity_len
(
    x2ap_rrm_rsu_composite_available_capacity_t *p_x2ap_rrm_rsu_composite_available_capacity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_composite_available_capacity != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_composite_available_capacity->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_rsu_composite_available_capacity->bitmask & CELL_CAPACITY_CLASS_VALUE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_class_value);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_value);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_composite_available_capacity
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_composite_available_capacity_t *p_x2ap_rrm_rsu_composite_available_capacity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_composite_available_capacity != PNULL);

    /* This function composes x2ap_rrm_rsu_composite_available_capacity */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_composite_available_capacity->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_composite_available_capacity->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_rsu_composite_available_capacity->bitmask & CELL_CAPACITY_CLASS_VALUE_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_class_value < 1) || (p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_class_value > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_class_value] should be in range "
            "1 to 100. Incorrect value %u received.", p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_class_value);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_class_value, "cell_capacity_class_value");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_class_value);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_value > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_value] should be less than"
            " or equal to 100. Incorrect value %u received.", p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_value);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_value, "cell_capacity_value");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_value);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_cacg_len
(
    x2ap_rrm_rsu_cacg_t *p_x2ap_rrm_rsu_cacg
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_cacg != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_rsu_composite_available_capacity_len(&p_x2ap_rrm_rsu_cacg->dl_composite_available_capacity);

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_rsu_composite_available_capacity_len(&p_x2ap_rrm_rsu_cacg->ul_composite_available_capacity);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_cacg
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_cacg_t *p_x2ap_rrm_rsu_cacg
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_cacg != PNULL);

    /* This function composes x2ap_rrm_rsu_cacg */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_composite_available_capacity(pp_buffer, &p_x2ap_rrm_rsu_cacg->dl_composite_available_capacity))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_composite_available_capacity(pp_buffer, &p_x2ap_rrm_rsu_cacg->ul_composite_available_capacity))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_usable_abs_information_fdd_len
(
    x2ap_usable_abs_information_fdd_t *p_x2ap_usable_abs_information_fdd
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_usable_abs_information_fdd != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_usable_abs_information_fdd->usable_abs_pattern_fdd_info);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_usable_abs_information_fdd
(
    U8  **pp_buffer,
    x2ap_usable_abs_information_fdd_t *p_x2ap_usable_abs_information_fdd
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_usable_abs_information_fdd != PNULL);

    /* This function composes x2ap_usable_abs_information_fdd */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_usable_abs_information_fdd->usable_abs_pattern_fdd_info); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_usable_abs_information_fdd->usable_abs_pattern_fdd_info[loop], "usable_abs_pattern_fdd_info[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_usable_abs_information_tdd_len
(
    x2ap_usable_abs_information_tdd_t *p_x2ap_usable_abs_information_tdd
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_usable_abs_information_tdd != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_usable_abs_information_tdd->usable_abs_pattern_tdd_info);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_usable_abs_information_tdd
(
    U8  **pp_buffer,
    x2ap_usable_abs_information_tdd_t *p_x2ap_usable_abs_information_tdd
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_usable_abs_information_tdd != PNULL);

    /* This function composes x2ap_usable_abs_information_tdd */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_usable_abs_information_tdd->usable_abs_pattern_tdd_info); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_usable_abs_information_tdd->usable_abs_pattern_tdd_info[loop], "usable_abs_pattern_tdd_info[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_usable_abs_information_len
(
    x2ap_usable_abs_information_t *p_x2ap_usable_abs_information
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_usable_abs_information != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_usable_abs_information->bitmask);

    /* Optional element */
    if(p_x2ap_usable_abs_information->bitmask & X2AP_USABLE_ABS_INFORMATION_FDD_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_usable_abs_information_fdd_len(&p_x2ap_usable_abs_information->usable_abs_info_fdd);
    }

    /* Optional element */
    if(p_x2ap_usable_abs_information->bitmask & X2AP_USABLE_ABS_INFORMATION_TDD_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_usable_abs_information_tdd_len(&p_x2ap_usable_abs_information->usable_abs_info_tdd);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_usable_abs_information
(
    U8  **pp_buffer,
    x2ap_usable_abs_information_t *p_x2ap_usable_abs_information
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_usable_abs_information != PNULL);

    /* This function composes x2ap_usable_abs_information */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_usable_abs_information->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_usable_abs_information->bitmask);

    /* Optional element */
    if(p_x2ap_usable_abs_information->bitmask & X2AP_USABLE_ABS_INFORMATION_FDD_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_usable_abs_information_fdd(pp_buffer, &p_x2ap_usable_abs_information->usable_abs_info_fdd))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_usable_abs_information->bitmask & X2AP_USABLE_ABS_INFORMATION_TDD_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_usable_abs_information_tdd(pp_buffer, &p_x2ap_usable_abs_information->usable_abs_info_tdd))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_abs_status_len
(
    x2ap_rrm_abs_status_t *p_x2ap_rrm_abs_status
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_abs_status != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_abs_status->dl_abs_status);

    /* Get length of IE */
    length += rrc_il_get_x2ap_usable_abs_information_len(&p_x2ap_rrm_abs_status->x2ap_usable_abs_information);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_abs_status
(
    U8  **pp_buffer,
    x2ap_rrm_abs_status_t *p_x2ap_rrm_abs_status
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_abs_status != PNULL);

    /* This function composes x2ap_rrm_abs_status */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_abs_status->dl_abs_status > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_abs_status->dl_abs_status] should be less than"
            " or equal to 100. Incorrect value %u received.", p_x2ap_rrm_abs_status->dl_abs_status);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_abs_status->dl_abs_status, "dl_abs_status");
    *pp_buffer += sizeof(p_x2ap_rrm_abs_status->dl_abs_status);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_usable_abs_information(pp_buffer, &p_x2ap_rrm_abs_status->x2ap_usable_abs_information))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_info_len
(
    x2ap_rrm_rsu_info_t *p_x2ap_rrm_rsu_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_info->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_rsu_info->bitmask & PRB_PERIODIC_REPORT_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_rsu_prb_len(&p_x2ap_rrm_rsu_info->radio_resource_status);
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_info->bitmask & TNL_LOAD_IND_PERIODIC_REPORT_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_rsu_tnl_load_len(&p_x2ap_rrm_rsu_info->s1_tnl_load_indicator);
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_info->bitmask & LOAD_IND_PERIODIC_REPORT_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_rsu_hw_load_len(&p_x2ap_rrm_rsu_info->hardware_load_indicator);
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_info->bitmask & COMPOSITE_AVAILABLE_CAPACITY_PERIODIC_REPORT_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_rsu_cacg_len(&p_x2ap_rrm_rsu_info->composite_available_capacity_group);
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_info->bitmask & ABS_STATUS_PERIODIC_REPORT_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_abs_status_len(&p_x2ap_rrm_rsu_info->abs_status);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_info
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_info_t *p_x2ap_rrm_rsu_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_info != PNULL);

    /* This function composes x2ap_rrm_rsu_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_rsu_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_info->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_rsu_info->bitmask & PRB_PERIODIC_REPORT_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_prb(pp_buffer, &p_x2ap_rrm_rsu_info->radio_resource_status))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_info->bitmask & TNL_LOAD_IND_PERIODIC_REPORT_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_tnl_load(pp_buffer, &p_x2ap_rrm_rsu_info->s1_tnl_load_indicator))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_info->bitmask & LOAD_IND_PERIODIC_REPORT_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_hw_load(pp_buffer, &p_x2ap_rrm_rsu_info->hardware_load_indicator))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_info->bitmask & COMPOSITE_AVAILABLE_CAPACITY_PERIODIC_REPORT_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_cacg(pp_buffer, &p_x2ap_rrm_rsu_info->composite_available_capacity_group))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_rsu_info->bitmask & ABS_STATUS_PERIODIC_REPORT_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_abs_status(pp_buffer, &p_x2ap_rrm_rsu_info->abs_status))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_rsu_report_info_len
(
    x2ap_rrm_rsu_report_info_t *p_x2ap_rrm_rsu_report_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_report_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_rrm_cell_global_id_eutra_len(&p_x2ap_rrm_rsu_report_info->cell_info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_rsu_info_len(&p_x2ap_rrm_rsu_report_info->rsu_info);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_report_info
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_report_info_t *p_x2ap_rrm_rsu_report_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_report_info != PNULL);

    /* This function composes x2ap_rrm_rsu_report_info */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrm_cell_global_id_eutra(pp_buffer, &p_x2ap_rrm_rsu_report_info->cell_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_info(pp_buffer, &p_x2ap_rrm_rsu_report_info->rsu_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_rrm_stop_req_len
(
    x2ap_rrm_rsu_rrm_stop_req_t *p_x2ap_rrm_rsu_rrm_stop_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_stop_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_rrm_stop_req->enb_meas_id);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rrm_rsu_rrm_stop_req->peer_gb_enb_id);

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_rrm_stop_req
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_rrm_stop_req_t *p_x2ap_rrm_rsu_rrm_stop_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_stop_req != PNULL);

    /* This function composes x2ap_rrm_rsu_rrm_stop_req */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_rrm_stop_req->enb_meas_id < 1) || (p_x2ap_rrm_rsu_rrm_stop_req->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_stop_req->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_stop_req->enb_meas_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_rsu_rrm_stop_req->enb_meas_id, "enb_meas_id");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_rrm_stop_req->enb_meas_id);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rrm_rsu_rrm_stop_req->peer_gb_enb_id))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_stop_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_RSU_RRM_STOP_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_stop_req
(
    x2ap_rrm_rsu_rrm_stop_req_t  *p_x2ap_rrm_rsu_rrm_stop_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_rsu_rrm_stop_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_rsu_rrm_stop_req_len(p_x2ap_rrm_rsu_rrm_stop_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_RSU_RRM_STOP_REQ", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_RSU_RRM_STOP_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_RSU_RRM_STOP_REQ, msg_length,cell_index);

    /* Fill X2AP_RRM_RSU_RRM_STOP_REQ message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_rrm_stop_req(&p_x2apRrm_msg, p_x2ap_rrm_rsu_rrm_stop_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_enb_stop_req_len
(
    x2ap_rrm_rsu_enb_stop_req_t *p_x2ap_rrm_rsu_enb_stop_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_enb_stop_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rrm_rsu_enb_stop_req->peer_gb_enb_id);

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_enb_stop_req
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_enb_stop_req_t *p_x2ap_rrm_rsu_enb_stop_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_enb_stop_req != PNULL);

    /* This function composes x2ap_rrm_rsu_enb_stop_req */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id < 1) || (p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id, "enb_meas_id");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rrm_rsu_enb_stop_req->peer_gb_enb_id))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_stop_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_RSU_ENB_STOP_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_stop_req
(
    x2ap_rrm_rsu_enb_stop_req_t  *p_x2ap_rrm_rsu_enb_stop_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_rsu_enb_stop_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_rsu_enb_stop_req_len(p_x2ap_rrm_rsu_enb_stop_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_RSU_ENB_STOP_REQ", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_RSU_ENB_STOP_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_RSU_ENB_STOP_REQ, msg_length,cell_index);

    /* Fill X2AP_RRM_RSU_ENB_STOP_REQ message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_enb_stop_req(&p_x2apRrm_msg, p_x2ap_rrm_rsu_enb_stop_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_enb_update_ind_len
(
    x2ap_rrm_rsu_enb_update_ind_t *p_x2ap_rrm_rsu_enb_update_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_rsu_enb_update_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_update_ind->enb_meas_id);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rrm_rsu_enb_update_ind->peer_gb_enb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_rsu_enb_update_ind->num_reports);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_enb_update_ind->num_reports > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_update_ind->num_reports] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_rrm_rsu_enb_update_ind->num_reports);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_enb_update_ind->num_reports; loop++)
        {
            length += rrc_il_get_x2ap_rrm_rsu_report_info_len(&p_x2ap_rrm_rsu_enb_update_ind->rsu_report[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_enb_update_ind
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_enb_update_ind_t *p_x2ap_rrm_rsu_enb_update_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_rsu_enb_update_ind != PNULL);

    /* This function composes x2ap_rrm_rsu_enb_update_ind */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_enb_update_ind->enb_meas_id < 1) || (p_x2ap_rrm_rsu_enb_update_ind->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_update_ind->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_update_ind->enb_meas_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_rsu_enb_update_ind->enb_meas_id, "enb_meas_id");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_update_ind->enb_meas_id);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rrm_rsu_enb_update_ind->peer_gb_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_enb_update_ind->num_reports > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_update_ind->num_reports] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_update_ind->num_reports);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_rsu_enb_update_ind->num_reports, "num_reports");
    *pp_buffer += sizeof(p_x2ap_rrm_rsu_enb_update_ind->num_reports);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_enb_update_ind->num_reports; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_report_info(pp_buffer, &p_x2ap_rrm_rsu_enb_update_ind->rsu_report[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_update_ind
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_RSU_ENB_UPDATE_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_update_ind
(
    x2ap_rrm_rsu_enb_update_ind_t  *p_x2ap_rrm_rsu_enb_update_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_rsu_enb_update_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_rsu_enb_update_ind_len(p_x2ap_rrm_rsu_enb_update_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_RSU_ENB_UPDATE_IND", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_RSU_ENB_UPDATE_IND, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_RSU_ENB_UPDATE_IND, msg_length,cell_index);

    /* Fill X2AP_RRM_RSU_ENB_UPDATE_IND message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_rsu_enb_update_ind(&p_x2apRrm_msg, p_x2ap_rrm_rsu_enb_update_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_load_information_req_len
(
    x2ap_rrm_load_information_req_t *p_x2ap_rrm_load_information_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_load_information_req != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rrm_load_information_req->gb_enb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_load_information_req->cell_information_count);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_load_information_req->cell_information_count > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_load_information_req->cell_information_count] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_rrm_load_information_req->cell_information_count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_load_information_req->cell_information_count; loop++)
        {
            length += rrc_il_get_x2ap_rrm_li_cell_info_item_arr_len(&p_x2ap_rrm_load_information_req->cell_information[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_load_information_req
(
    U8  **pp_buffer,
    x2ap_rrm_load_information_req_t *p_x2ap_rrm_load_information_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_load_information_req != PNULL);

    /* This function composes x2ap_rrm_load_information_req */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rrm_load_information_req->gb_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_load_information_req->cell_information_count > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_load_information_req->cell_information_count] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_load_information_req->cell_information_count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_load_information_req->cell_information_count, "cell_information_count");
    *pp_buffer += sizeof(p_x2ap_rrm_load_information_req->cell_information_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_load_information_req->cell_information_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_li_cell_info_item_arr(pp_buffer, &p_x2ap_rrm_load_information_req->cell_information[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_load_information_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_LI_RRM_LOAD_INFORMATION_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_load_information_req
(
    x2ap_rrm_load_information_req_t  *p_x2ap_rrm_load_information_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_load_information_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_load_information_req_len(p_x2ap_rrm_load_information_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_LI_RRM_LOAD_INFORMATION_REQ", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_LI_RRM_LOAD_INFORMATION_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_LI_RRM_LOAD_INFORMATION_REQ, msg_length,cell_index);

    /* Fill X2AP_RRM_LI_RRM_LOAD_INFORMATION_REQ message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_load_information_req(&p_x2apRrm_msg, p_x2ap_rrm_load_information_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
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
rrc_il_get_x2ap_rrm_li_ul_intf_overload_ind_len
(
    x2ap_rrm_li_ul_intf_overload_ind_t *p_x2ap_rrm_li_ul_intf_overload_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_li_ul_intf_overload_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_li_ul_intf_overload_ind->count);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_x2ap_rrm_li_ul_intf_overload_ind->count * sizeof(p_x2ap_rrm_li_ul_intf_overload_ind->ul_intf_overload_indication[0]));

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_ul_intf_overload_ind
(
    U8  **pp_buffer,
    x2ap_rrm_li_ul_intf_overload_ind_t *p_x2ap_rrm_li_ul_intf_overload_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_li_ul_intf_overload_ind != PNULL);

    /* This function composes x2ap_rrm_li_ul_intf_overload_ind */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_ul_intf_overload_ind->count > X2AP_LI_MAX_PRBS_COUNT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_ul_intf_overload_ind->count] should be less than"
            " or equal to X2AP_LI_MAX_PRBS_COUNT. Incorrect value %u received.", p_x2ap_rrm_li_ul_intf_overload_ind->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_li_ul_intf_overload_ind->count, "count");
    *pp_buffer += sizeof(p_x2ap_rrm_li_ul_intf_overload_ind->count);

    /* Compose OCTET_STRING VARIABLE of basic type elements with HIGHER boundary 
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_li_ul_intf_overload_ind->count; loop++)
        {
            if (p_x2ap_rrm_li_ul_intf_overload_ind->ul_intf_overload_indication[loop] > 2)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_x2ap_rrm_li_ul_intf_overload_ind->ul_intf_overload_indication[%u] should be less"
                    " than or equal to 2. "
                    "Incorrect value %d received.", loop, p_x2ap_rrm_li_ul_intf_overload_ind->ul_intf_overload_indication[loop]);
                return RRC_FAILURE;
            }
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_li_ul_intf_overload_ind->ul_intf_overload_indication[loop], "ul_intf_overload_indication[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_ul_high_intf_ind_len
(
    x2ap_rrm_li_ul_high_intf_ind_t *p_x2ap_rrm_li_ul_high_intf_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_li_ul_high_intf_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_li_ul_high_intf_ind->prb_count);

  /* Get total length of OCTETS of BIT_STRING VARIABLE of basic type elements */
  length += (((p_x2ap_rrm_li_ul_high_intf_ind->prb_count)%8?(((p_x2ap_rrm_li_ul_high_intf_ind->prb_count)/8)+1):((p_x2ap_rrm_li_ul_high_intf_ind->prb_count)/8)));


    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_ul_high_intf_ind
(
    U8  **pp_buffer,
    x2ap_rrm_li_ul_high_intf_ind_t *p_x2ap_rrm_li_ul_high_intf_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_li_ul_high_intf_ind != PNULL);

    /* This function composes x2ap_rrm_li_ul_high_intf_ind */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_li_ul_high_intf_ind->prb_count < 1) || (p_x2ap_rrm_li_ul_high_intf_ind->prb_count > X2AP_RRM_LI_MAX_HII_PRB_OCTETS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_ul_high_intf_ind->prb_count] should be in range "
            "1 to X2AP_RRM_LI_MAX_HII_PRB_OCTETS. Incorrect value %u received.", p_x2ap_rrm_li_ul_high_intf_ind->prb_count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_li_ul_high_intf_ind->prb_count, "prb_count");
    *pp_buffer += sizeof(p_x2ap_rrm_li_ul_high_intf_ind->prb_count);

  /* Compose BIT_STRING VARIABLE of basic type elements */
  {
    rrc_counter_t loop;
    rrc_counter_t loop_max = (((p_x2ap_rrm_li_ul_high_intf_ind->prb_count)%8?(((p_x2ap_rrm_li_ul_high_intf_ind->prb_count)/8)+1):((p_x2ap_rrm_li_ul_high_intf_ind->prb_count)/8)));
    for (loop = 0; loop < loop_max; loop++)
    {
        rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_li_ul_high_intf_ind->hii[loop], "hii[]");
        *pp_buffer += sizeof(U8);
    }
  }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_ul_high_intf_info_arr_len
(
    x2ap_rrm_li_ul_high_intf_info_arr_t *p_x2ap_rrm_li_ul_high_intf_info_arr
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_li_ul_high_intf_info_arr != PNULL);

    /* Get length of IE */
    length += rrc_il_get_rrm_cell_global_id_eutra_len(&p_x2ap_rrm_li_ul_high_intf_info_arr->target_cell_id);

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_li_ul_high_intf_ind_len(&p_x2ap_rrm_li_ul_high_intf_info_arr->ul_high_intf_ind);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_ul_high_intf_info_arr
(
    U8  **pp_buffer,
    x2ap_rrm_li_ul_high_intf_info_arr_t *p_x2ap_rrm_li_ul_high_intf_info_arr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_li_ul_high_intf_info_arr != PNULL);

    /* This function composes x2ap_rrm_li_ul_high_intf_info_arr */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrm_cell_global_id_eutra(pp_buffer, &p_x2ap_rrm_li_ul_high_intf_info_arr->target_cell_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_li_ul_high_intf_ind(pp_buffer, &p_x2ap_rrm_li_ul_high_intf_info_arr->ul_high_intf_ind))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_ul_high_interference_info_len
(
    x2ap_rrm_li_ul_high_interference_info_t *p_x2ap_rrm_li_ul_high_interference_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_li_ul_high_interference_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count; loop++)
        {
            length += rrc_il_get_x2ap_rrm_li_ul_high_intf_info_arr_len(&p_x2ap_rrm_li_ul_high_interference_info->ul_high_intf_info[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_ul_high_interference_info
(
    U8  **pp_buffer,
    x2ap_rrm_li_ul_high_interference_info_t *p_x2ap_rrm_li_ul_high_interference_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_li_ul_high_interference_info != PNULL);

    /* This function composes x2ap_rrm_li_ul_high_interference_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count, "ul_high_interference_info_count");
    *pp_buffer += sizeof(p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_li_ul_high_intf_info_arr(pp_buffer, &p_x2ap_rrm_li_ul_high_interference_info->ul_high_intf_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_relative_narrowband_tx_power_len
(
    x2ap_rrm_li_relative_narrowband_tx_power_t *p_x2ap_rrm_li_relative_narrowband_tx_power
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_li_relative_narrowband_tx_power != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count);

  /* Get total length of OCTETS of BIT_STRING VARIABLE of basic type elements */
  length += (((p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count)%8?(((p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count)/8)+1):((p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count)/8)));

    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_li_relative_narrowband_tx_power->rntp_threshold);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_li_relative_narrowband_tx_power->num_cell_specific_antena_port);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_li_relative_narrowband_tx_power->p_b);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_li_relative_narrowband_tx_power->pdcch_interference_impact);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_relative_narrowband_tx_power
(
    U8  **pp_buffer,
    x2ap_rrm_li_relative_narrowband_tx_power_t *p_x2ap_rrm_li_relative_narrowband_tx_power
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_li_relative_narrowband_tx_power != PNULL);

    /* This function composes x2ap_rrm_li_relative_narrowband_tx_power */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count < 6) || (p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count > X2AP_RRM_LI_MAX_HII_PRB_OCTETS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count] should be in range "
            "6 to X2AP_RRM_LI_MAX_HII_PRB_OCTETS. Incorrect value %u received.", p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count, "prb_count");
    *pp_buffer += sizeof(p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count);

  /* Compose BIT_STRING VARIABLE of basic type elements */
  {
    rrc_counter_t loop;
    rrc_counter_t loop_max = (((p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count)%8?(((p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count)/8)+1):((p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count)/8)));
    for (loop = 0; loop < loop_max; loop++)
    {
        rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_li_relative_narrowband_tx_power->rntp_per_prb[loop], "rntp_per_prb[]");
        *pp_buffer += sizeof(U8);
    }
  }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_relative_narrowband_tx_power->rntp_threshold > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_relative_narrowband_tx_power->rntp_threshold] should be less than"
            " or equal to 15. Incorrect value %u received.", p_x2ap_rrm_li_relative_narrowband_tx_power->rntp_threshold);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_li_relative_narrowband_tx_power->rntp_threshold, "rntp_threshold");
    *pp_buffer += sizeof(p_x2ap_rrm_li_relative_narrowband_tx_power->rntp_threshold);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_relative_narrowband_tx_power->num_cell_specific_antena_port > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_relative_narrowband_tx_power->num_cell_specific_antena_port] should be less than"
            " or equal to 2. Incorrect value %u received.", p_x2ap_rrm_li_relative_narrowband_tx_power->num_cell_specific_antena_port);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_li_relative_narrowband_tx_power->num_cell_specific_antena_port, "num_cell_specific_antena_port");
    *pp_buffer += sizeof(p_x2ap_rrm_li_relative_narrowband_tx_power->num_cell_specific_antena_port);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_relative_narrowband_tx_power->p_b > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_relative_narrowband_tx_power->p_b] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_li_relative_narrowband_tx_power->p_b);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_li_relative_narrowband_tx_power->p_b, "p_b");
    *pp_buffer += sizeof(p_x2ap_rrm_li_relative_narrowband_tx_power->p_b);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_relative_narrowband_tx_power->pdcch_interference_impact > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_relative_narrowband_tx_power->pdcch_interference_impact] should be less than"
            " or equal to 4. Incorrect value %u received.", p_x2ap_rrm_li_relative_narrowband_tx_power->pdcch_interference_impact);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_li_relative_narrowband_tx_power->pdcch_interference_impact, "pdcch_interference_impact");
    *pp_buffer += sizeof(p_x2ap_rrm_li_relative_narrowband_tx_power->pdcch_interference_impact);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_abs_information_fdd_len
(
    x2ap_abs_information_fdd_t *p_x2ap_abs_information_fdd
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_abs_information_fdd != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_abs_information_fdd->abs_pattern_info);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_abs_information_fdd->no_cell_specific_antenna_port);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_abs_information_fdd->measurement_subset);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_abs_information_fdd
(
    U8  **pp_buffer,
    x2ap_abs_information_fdd_t *p_x2ap_abs_information_fdd
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_abs_information_fdd != PNULL);

    /* This function composes x2ap_abs_information_fdd */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_abs_information_fdd->abs_pattern_info); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_abs_information_fdd->abs_pattern_info[loop], "abs_pattern_info[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_abs_information_fdd->no_cell_specific_antenna_port > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_abs_information_fdd->no_cell_specific_antenna_port] should be less than"
            " or equal to 2. Incorrect value %u received.", p_x2ap_abs_information_fdd->no_cell_specific_antenna_port);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_abs_information_fdd->no_cell_specific_antenna_port, "no_cell_specific_antenna_port");
    *pp_buffer += sizeof(p_x2ap_abs_information_fdd->no_cell_specific_antenna_port);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_abs_information_fdd->measurement_subset); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_abs_information_fdd->measurement_subset[loop], "measurement_subset[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_abs_information_tdd_len
(
    x2ap_abs_information_tdd_t *p_x2ap_abs_information_tdd
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_abs_information_tdd != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_abs_information_tdd->abs_pattern_info);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_abs_information_tdd->no_cell_specific_antenna_port);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_abs_information_tdd->measurement_subset);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_abs_information_tdd
(
    U8  **pp_buffer,
    x2ap_abs_information_tdd_t *p_x2ap_abs_information_tdd
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_abs_information_tdd != PNULL);

    /* This function composes x2ap_abs_information_tdd */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_abs_information_tdd->abs_pattern_info); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_abs_information_tdd->abs_pattern_info[loop], "abs_pattern_info[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_abs_information_tdd->no_cell_specific_antenna_port > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_abs_information_tdd->no_cell_specific_antenna_port] should be less than"
            " or equal to 2. Incorrect value %u received.", p_x2ap_abs_information_tdd->no_cell_specific_antenna_port);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_abs_information_tdd->no_cell_specific_antenna_port, "no_cell_specific_antenna_port");
    *pp_buffer += sizeof(p_x2ap_abs_information_tdd->no_cell_specific_antenna_port);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_abs_information_tdd->measurement_subset); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_abs_information_tdd->measurement_subset[loop], "measurement_subset[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_abs_information_len
(
    x2ap_rrm_li_abs_information_t *p_x2ap_rrm_li_abs_information
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_li_abs_information != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_li_abs_information->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_li_abs_information->bitmask & X2AP_LI_ABS_INFORMATION_FDD_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_abs_information_fdd_len(&p_x2ap_rrm_li_abs_information->abs_info_fdd);
    }

    /* Optional element */
    if(p_x2ap_rrm_li_abs_information->bitmask & X2AP_LI_ABS_INFORMATION_TDD_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_abs_information_tdd_len(&p_x2ap_rrm_li_abs_information->abs_info_tdd);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_abs_information
(
    U8  **pp_buffer,
    x2ap_rrm_li_abs_information_t *p_x2ap_rrm_li_abs_information
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_li_abs_information != PNULL);

    /* This function composes x2ap_rrm_li_abs_information */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_li_abs_information->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_li_abs_information->bitmask);

    /* Optional element */
    if(p_x2ap_rrm_li_abs_information->bitmask & X2AP_LI_ABS_INFORMATION_FDD_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_abs_information_fdd(pp_buffer, &p_x2ap_rrm_li_abs_information->abs_info_fdd))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_li_abs_information->bitmask & X2AP_LI_ABS_INFORMATION_TDD_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_abs_information_tdd(pp_buffer, &p_x2ap_rrm_li_abs_information->abs_info_tdd))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_invoke_indication_len
(
    x2ap_rrm_li_invoke_indication_t *p_x2ap_rrm_li_invoke_indication
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_li_invoke_indication != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_li_invoke_indication->invoke_ind);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_invoke_indication
(
    U8  **pp_buffer,
    x2ap_rrm_li_invoke_indication_t *p_x2ap_rrm_li_invoke_indication
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_li_invoke_indication != PNULL);

    /* This function composes x2ap_rrm_li_invoke_indication */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_invoke_indication->invoke_ind > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_invoke_indication->invoke_ind] should be less than"
            " or equal to 0. Incorrect value %u received.", p_x2ap_rrm_li_invoke_indication->invoke_ind);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_rrm_li_invoke_indication->invoke_ind, "invoke_ind");
    *pp_buffer += sizeof(p_x2ap_rrm_li_invoke_indication->invoke_ind);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_rrm_li_cell_info_item_arr_len
(
    x2ap_rrm_li_cell_info_item_arr_t *p_x2ap_rrm_li_cell_info_item_arr
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_li_cell_info_item_arr != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_li_cell_info_item_arr->bitmask);

    /* Get length of IE */
    length += rrc_il_get_rrm_cell_global_id_eutra_len(&p_x2ap_rrm_li_cell_info_item_arr->source_cell_id);

    /* Optional element */
    if(p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_CELL_INFO_ITEM_UL_INTF_OVERLOAD_IND_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_li_ul_intf_overload_ind_len(&p_x2ap_rrm_li_cell_info_item_arr->ul_intf_overload_ind);
    }

    /* Optional element */
    if(p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_CELL_INFO_ITEM_UL_INTF_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_li_ul_high_interference_info_len(&p_x2ap_rrm_li_cell_info_item_arr->ul_high_interference_info);
    }

    /* Optional element */
    if(p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_CELL_INFO_ITEM_RELATIVE_NARROWBAND_TX_POWER_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_li_relative_narrowband_tx_power_len(&p_x2ap_rrm_li_cell_info_item_arr->relative_narrowband_tx_power);
    }

    /* Optional element */
    if(p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_ABS_INFORMATION_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_li_abs_information_len(&p_x2ap_rrm_li_cell_info_item_arr->abs_infomation);
    }

    /* Optional element */
    if(p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_INVOKE_INDICATION_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_rrm_li_invoke_indication_len(&p_x2ap_rrm_li_cell_info_item_arr->invoke_indication);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_rrm_li_cell_info_item_arr
(
    U8  **pp_buffer,
    x2ap_rrm_li_cell_info_item_arr_t *p_x2ap_rrm_li_cell_info_item_arr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_li_cell_info_item_arr != PNULL);

    /* This function composes x2ap_rrm_li_cell_info_item_arr */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_li_cell_info_item_arr->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_li_cell_info_item_arr->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrm_cell_global_id_eutra(pp_buffer, &p_x2ap_rrm_li_cell_info_item_arr->source_cell_id))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_CELL_INFO_ITEM_UL_INTF_OVERLOAD_IND_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_li_ul_intf_overload_ind(pp_buffer, &p_x2ap_rrm_li_cell_info_item_arr->ul_intf_overload_ind))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_CELL_INFO_ITEM_UL_INTF_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_li_ul_high_interference_info(pp_buffer, &p_x2ap_rrm_li_cell_info_item_arr->ul_high_interference_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_CELL_INFO_ITEM_RELATIVE_NARROWBAND_TX_POWER_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_li_relative_narrowband_tx_power(pp_buffer, &p_x2ap_rrm_li_cell_info_item_arr->relative_narrowband_tx_power))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_ABS_INFORMATION_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_li_abs_information(pp_buffer, &p_x2ap_rrm_li_cell_info_item_arr->abs_infomation))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_INVOKE_INDICATION_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_li_invoke_indication(pp_buffer, &p_x2ap_rrm_li_cell_info_item_arr->invoke_indication))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_load_information_resp_len
(
    x2ap_rrm_load_information_resp_t *p_x2ap_rrm_load_information_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_load_information_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_load_information_resp->response);

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_load_information_resp
(
    U8  **pp_buffer,
    x2ap_rrm_load_information_resp_t *p_x2ap_rrm_load_information_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_load_information_resp != PNULL);

    /* This function composes x2ap_rrm_load_information_resp */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rrm_load_information_resp->response, "response");
    *pp_buffer += sizeof(p_x2ap_rrm_load_information_resp->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_load_information_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_LI_ENB_LOAD_INFORMATION_RES message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_load_information_resp
(
    x2ap_rrm_load_information_resp_t  *p_x2ap_rrm_load_information_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_load_information_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_load_information_resp_len(p_x2ap_rrm_load_information_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_LI_ENB_LOAD_INFORMATION_RES", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_LI_ENB_LOAD_INFORMATION_RES, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_LI_ENB_LOAD_INFORMATION_RES, msg_length,cell_index);

    /* Fill X2AP_RRM_LI_ENB_LOAD_INFORMATION_RES message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_load_information_resp(&p_x2apRrm_msg, p_x2ap_rrm_load_information_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rrm_load_information_ind_len
(
    x2ap_rrm_load_information_ind_t *p_x2ap_rrm_load_information_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_load_information_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rrm_load_information_ind->cell_information_count);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_load_information_ind->cell_information_count > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_load_information_ind->cell_information_count] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_rrm_load_information_ind->cell_information_count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_load_information_ind->cell_information_count; loop++)
        {
            length += rrc_il_get_x2ap_rrm_li_cell_info_item_arr_len(&p_x2ap_rrm_load_information_ind->cell_load_info[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rrm_load_information_ind
(
    U8  **pp_buffer,
    x2ap_rrm_load_information_ind_t *p_x2ap_rrm_load_information_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_load_information_ind != PNULL);

    /* This function composes x2ap_rrm_load_information_ind */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_load_information_ind->cell_information_count > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_load_information_ind->cell_information_count] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_load_information_ind->cell_information_count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_load_information_ind->cell_information_count, "cell_information_count");
    *pp_buffer += sizeof(p_x2ap_rrm_load_information_ind->cell_information_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_load_information_ind->cell_information_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_li_cell_info_item_arr(pp_buffer, &p_x2ap_rrm_load_information_ind->cell_load_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rrm_load_information_ind
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RRM_LI_ENB_LOAD_INFORMATION_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_load_information_ind
(
    x2ap_rrm_load_information_ind_t  *p_x2ap_rrm_load_information_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rrm_load_information_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rrm_load_information_ind_len(p_x2ap_rrm_load_information_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RRM_LI_ENB_LOAD_INFORMATION_IND", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RRM_LI_ENB_LOAD_INFORMATION_IND, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RRM_LI_ENB_LOAD_INFORMATION_IND, msg_length,cell_index);

    /* Fill X2AP_RRM_LI_ENB_LOAD_INFORMATION_IND message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_load_information_ind(&p_x2apRrm_msg, p_x2ap_rrm_load_information_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_rlf_ind_len
(
    x2ap_rlf_ind_t *p_x2ap_rlf_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rlf_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rlf_ind->bitmask);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rlf_ind->enb1_gb_id);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_rlf_ind->enb2_gb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rlf_ind->failure_cell_pci);

    /* Get length of IE */
    length += rrc_il_get_x2ap_ecgi_len(&p_x2ap_rlf_ind->re_est_cell_ecgi);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rlf_ind->crnti);

    /* Optional element */
    if(p_x2ap_rlf_ind->bitmask & X2AP_RLF_IND_RLF_REPORT_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_rlf_report_len(&p_x2ap_rlf_ind->rlf_report);
    }

    /* Optional element */
    if(p_x2ap_rlf_ind->bitmask & X2AP_RLF_IND_UE_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rlf_ind->ue_index);
    }

    /* Optional element */
    if(p_x2ap_rlf_ind->bitmask & X2AP_RLF_IND_UE_CONTEXT_RELEASED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rlf_ind->ue_context_released);
    }

    /* Optional element */
    if(p_x2ap_rlf_ind->bitmask & X2AP_RLF_IND_CONN_SETUP_IND_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_rlf_ind->conn_setup_indicator);
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_rlf_ind
(
    U8  **pp_buffer,
    x2ap_rlf_ind_t *p_x2ap_rlf_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rlf_ind != PNULL);

    /* This function composes x2ap_rlf_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rlf_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rlf_ind->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rlf_ind->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_rlf_ind->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rlf_ind->failure_cell_pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rlf_ind->failure_cell_pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_x2ap_rlf_ind->failure_cell_pci);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rlf_ind->failure_cell_pci, "failure_cell_pci");
    *pp_buffer += sizeof(p_x2ap_rlf_ind->failure_cell_pci);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_ecgi(pp_buffer, &p_x2ap_rlf_ind->re_est_cell_ecgi))
    {
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rlf_ind->crnti, "crnti");
    *pp_buffer += sizeof(p_x2ap_rlf_ind->crnti);

    /* Optional element */
    if(p_x2ap_rlf_ind->bitmask & X2AP_RLF_IND_RLF_REPORT_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_rlf_report(pp_buffer, &p_x2ap_rlf_ind->rlf_report))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_rlf_ind->bitmask & X2AP_RLF_IND_UE_INDEX_PRESENT)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rlf_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_x2ap_rlf_ind->ue_index);
    }

    /* Optional element */
    if(p_x2ap_rlf_ind->bitmask & X2AP_RLF_IND_UE_CONTEXT_RELEASED_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rlf_ind->ue_context_released > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rlf_ind->ue_context_released] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_rlf_ind->ue_context_released);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rlf_ind->ue_context_released, "ue_context_released");
    *pp_buffer += sizeof(p_x2ap_rlf_ind->ue_context_released);
    }

    /* Optional element */
    if(p_x2ap_rlf_ind->bitmask & X2AP_RLF_IND_CONN_SETUP_IND_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rlf_ind->conn_setup_indicator > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rlf_ind->conn_setup_indicator] should be less than"
            " or equal to 0. Incorrect value %u received.", p_x2ap_rlf_ind->conn_setup_indicator);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_rlf_ind->conn_setup_indicator, "conn_setup_indicator");
    *pp_buffer += sizeof(p_x2ap_rlf_ind->conn_setup_indicator);
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_rlf_ind
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RLF_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_rlf_ind
(
    x2ap_rlf_ind_t  *p_x2ap_rlf_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2ap_rlf_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_rlf_ind_len(p_x2ap_rlf_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RLF_IND", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_RLF_IND, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_RLF_IND, msg_length,cell_index);

    /* Fill X2AP_RLF_IND message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_rlf_ind(&p_x2apRrm_msg, p_x2ap_rlf_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
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
rrc_il_get_rrc_rlf_report_meas_result_serv_cell_len
(
    rrc_rlf_report_meas_result_serv_cell_t *p_rrc_rlf_report_meas_result_serv_cell
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlf_report_meas_result_serv_cell != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_report_meas_result_serv_cell->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_report_meas_result_serv_cell->rsrp_result);

    /* Optional element */
    if(p_rrc_rlf_report_meas_result_serv_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_RSRQ_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_report_meas_result_serv_cell->rsrq_result);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlf_report_meas_result_serv_cell
(
    U8  **pp_buffer,
    rrc_rlf_report_meas_result_serv_cell_t *p_rrc_rlf_report_meas_result_serv_cell
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlf_report_meas_result_serv_cell != PNULL);

    /* This function composes rrc_rlf_report_meas_result_serv_cell */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlf_report_meas_result_serv_cell->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_rlf_report_meas_result_serv_cell->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlf_report_meas_result_serv_cell->rsrp_result > 97))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlf_report_meas_result_serv_cell->rsrp_result] should be less than"
            " or equal to 97. Incorrect value %u received.", p_rrc_rlf_report_meas_result_serv_cell->rsrp_result);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlf_report_meas_result_serv_cell->rsrp_result, "rsrp_result");
    *pp_buffer += sizeof(p_rrc_rlf_report_meas_result_serv_cell->rsrp_result);

    /* Optional element */
    if(p_rrc_rlf_report_meas_result_serv_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_RSRQ_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlf_report_meas_result_serv_cell->rsrq_result > 34))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlf_report_meas_result_serv_cell->rsrq_result] should be less than"
            " or equal to 34. Incorrect value %u received.", p_rrc_rlf_report_meas_result_serv_cell->rsrq_result);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlf_report_meas_result_serv_cell->rsrq_result, "rsrq_result");
    *pp_buffer += sizeof(p_rrc_rlf_report_meas_result_serv_cell->rsrq_result);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_cell_global_id_eutra_len
(
    cell_global_id_eutra_t *p_cell_global_id_eutra
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_cell_global_id_eutra != PNULL);

    /* Get length of IE */
    length += rrc_il_get_plmn_identity_len(&p_cell_global_id_eutra->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_cell_global_id_eutra->cell_identity);

    return length;
}

static
rrc_return_et
rrc_il_compose_cell_global_id_eutra
(
    U8  **pp_buffer,
    cell_global_id_eutra_t *p_cell_global_id_eutra
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_cell_global_id_eutra != PNULL);

    /* This function composes cell_global_id_eutra */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_plmn_identity(pp_buffer, &p_cell_global_id_eutra->plmn_identity))
    {
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_eutra->cell_identity); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_cell_global_id_eutra->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_plmn_identity_list2_len
(
    plmn_identity_list2_t *p_plmn_identity_list2
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_plmn_identity_list2 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_plmn_identity_list2->count);

    /* Check for correct range [H - higher boundary] */
    if ((p_plmn_identity_list2->count > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_plmn_identity_list2->count] should be less than"
            " or equal to 5. Incorrect value %d received.", p_plmn_identity_list2->count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_plmn_identity_list2->count; loop++)
        {
            length += rrc_il_get_plmn_identity_len(&p_plmn_identity_list2->cell_identity[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_plmn_identity_list2
(
    U8  **pp_buffer,
    plmn_identity_list2_t *p_plmn_identity_list2
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_plmn_identity_list2 != PNULL);

    /* This function composes plmn_identity_list2 */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_plmn_identity_list2->count > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_plmn_identity_list2->count] should be less than"
            " or equal to 5. Incorrect value %u received.", p_plmn_identity_list2->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_plmn_identity_list2->count, "count");
    *pp_buffer += sizeof(p_plmn_identity_list2->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_plmn_identity_list2->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_plmn_identity(pp_buffer, &p_plmn_identity_list2->cell_identity[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_eutra_cgi_info_len
(
    meas_result_eutra_cgi_info_t *p_meas_result_eutra_cgi_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_eutra_cgi_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_eutra_cgi_info->bitmask);

    /* Get length of IE */
    length += rrc_il_get_cell_global_id_eutra_len(&p_meas_result_eutra_cgi_info->cell_global_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_meas_result_eutra_cgi_info->tracking_area_code);

    /* Optional element */
    if(p_meas_result_eutra_cgi_info->bitmask & MEAS_RESULT_EUTRA_CGI_INFO_PLMN_IDENTITY_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_plmn_identity_list2_len(&p_meas_result_eutra_cgi_info->plmn_identity_list);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_eutra_cgi_info
(
    U8  **pp_buffer,
    meas_result_eutra_cgi_info_t *p_meas_result_eutra_cgi_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_eutra_cgi_info != PNULL);

    /* This function composes meas_result_eutra_cgi_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_eutra_cgi_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_meas_result_eutra_cgi_info->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_cell_global_id_eutra(pp_buffer, &p_meas_result_eutra_cgi_info->cell_global_id))
    {
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_meas_result_eutra_cgi_info->tracking_area_code); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_meas_result_eutra_cgi_info->tracking_area_code[loop], "tracking_area_code[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Optional element */
    if(p_meas_result_eutra_cgi_info->bitmask & MEAS_RESULT_EUTRA_CGI_INFO_PLMN_IDENTITY_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_plmn_identity_list2(pp_buffer, &p_meas_result_eutra_cgi_info->plmn_identity_list))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_eutra_meas_result_len
(
    meas_result_eutra_meas_result_t *p_meas_result_eutra_meas_result
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_eutra_meas_result != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_eutra_meas_result->bitmask);

    /* Optional element */
    if(p_meas_result_eutra_meas_result->bitmask & MEAS_RESULT_EUTRA_MEAS_RESULT_RSRP_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_eutra_meas_result->rsrp_result);
    }

    /* Optional element */
    if(p_meas_result_eutra_meas_result->bitmask & MEAS_RESULT_EUTRA_MEAS_RESULT_RSRQ_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_eutra_meas_result->rsrq_result);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_eutra_meas_result
(
    U8  **pp_buffer,
    meas_result_eutra_meas_result_t *p_meas_result_eutra_meas_result
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_eutra_meas_result != PNULL);

    /* This function composes meas_result_eutra_meas_result */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_eutra_meas_result->bitmask, "bitmask");
    *pp_buffer += sizeof(p_meas_result_eutra_meas_result->bitmask);

    /* Optional element */
    if(p_meas_result_eutra_meas_result->bitmask & MEAS_RESULT_EUTRA_MEAS_RESULT_RSRP_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_eutra_meas_result->rsrp_result > 97))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_eutra_meas_result->rsrp_result] should be less than"
            " or equal to 97. Incorrect value %u received.", p_meas_result_eutra_meas_result->rsrp_result);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_eutra_meas_result->rsrp_result, "rsrp_result");
    *pp_buffer += sizeof(p_meas_result_eutra_meas_result->rsrp_result);
    }

    /* Optional element */
    if(p_meas_result_eutra_meas_result->bitmask & MEAS_RESULT_EUTRA_MEAS_RESULT_RSRQ_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_eutra_meas_result->rsrq_result > 34))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_eutra_meas_result->rsrq_result] should be less than"
            " or equal to 34. Incorrect value %u received.", p_meas_result_eutra_meas_result->rsrq_result);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_eutra_meas_result->rsrq_result, "rsrq_result");
    *pp_buffer += sizeof(p_meas_result_eutra_meas_result->rsrq_result);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_csg_info_len
(
    meas_result_csg_info_t *p_meas_result_csg_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_csg_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_csg_info->bitmask);

    /* Optional element */
    if(p_meas_result_csg_info->bitmask & MEAS_RESULT_EUTRA_MEAS_RESULT_CSG_MEM_STATUS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_csg_info->csg_membershipStatus);
    }

    /* Optional element */
    if(p_meas_result_csg_info->bitmask & MEAS_RESULT_EUTRA_MEAS_RESULT_CSG_ID_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_meas_result_csg_info->csg_identity);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_csg_info
(
    U8  **pp_buffer,
    meas_result_csg_info_t *p_meas_result_csg_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_csg_info != PNULL);

    /* This function composes meas_result_csg_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_csg_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_meas_result_csg_info->bitmask);

    /* Optional element */
    if(p_meas_result_csg_info->bitmask & MEAS_RESULT_EUTRA_MEAS_RESULT_CSG_MEM_STATUS_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_csg_info->csg_membershipStatus > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_csg_info->csg_membershipStatus] should be less than"
            " or equal to 0. Incorrect value %u received.", p_meas_result_csg_info->csg_membershipStatus);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_csg_info->csg_membershipStatus, "csg_membershipStatus");
    *pp_buffer += sizeof(p_meas_result_csg_info->csg_membershipStatus);
    }

    /* Optional element */
    if(p_meas_result_csg_info->bitmask & MEAS_RESULT_EUTRA_MEAS_RESULT_CSG_ID_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_meas_result_csg_info->csg_identity); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_meas_result_csg_info->csg_identity[loop], "csg_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_eutra_len
(
    meas_result_eutra_t *p_meas_result_eutra
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_eutra != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_eutra->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_eutra->phys_cell_id);

    /* Optional element */
    if(p_meas_result_eutra->bitmask & MEAS_RESULT_EUTRA_CGI_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_meas_result_eutra_cgi_info_len(&p_meas_result_eutra->cgi_info);
    }

    /* Get length of IE */
    length += rrc_il_get_meas_result_eutra_meas_result_len(&p_meas_result_eutra->meas_result);

    /* Optional element */
    if(p_meas_result_eutra->bitmask & MEAS_RESULT_EUTRA_CSG_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_meas_result_csg_info_len(&p_meas_result_eutra->eutra_csg_info);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_eutra
(
    U8  **pp_buffer,
    meas_result_eutra_t *p_meas_result_eutra
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_eutra != PNULL);

    /* This function composes meas_result_eutra */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_eutra->bitmask, "bitmask");
    *pp_buffer += sizeof(p_meas_result_eutra->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_eutra->phys_cell_id > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_eutra->phys_cell_id] should be less than"
            " or equal to 503. Incorrect value %u received.", p_meas_result_eutra->phys_cell_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_eutra->phys_cell_id, "phys_cell_id");
    *pp_buffer += sizeof(p_meas_result_eutra->phys_cell_id);

    /* Optional element */
    if(p_meas_result_eutra->bitmask & MEAS_RESULT_EUTRA_CGI_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_eutra_cgi_info(pp_buffer, &p_meas_result_eutra->cgi_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_eutra_meas_result(pp_buffer, &p_meas_result_eutra->meas_result))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_meas_result_eutra->bitmask & MEAS_RESULT_EUTRA_CSG_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_csg_info(pp_buffer, &p_meas_result_eutra->eutra_csg_info))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_list_eutra_len
(
    meas_result_list_eutra_t *p_meas_result_list_eutra
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_list_eutra != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_list_eutra->count);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_list_eutra->count > MAX_CELL_REPORT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list_eutra->count] should be less than"
            " or equal to MAX_CELL_REPORT. Incorrect value %d received.", p_meas_result_list_eutra->count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list_eutra->count; loop++)
        {
            length += rrc_il_get_meas_result_eutra_len(&p_meas_result_list_eutra->meas_result_eutra[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_list_eutra
(
    U8  **pp_buffer,
    meas_result_list_eutra_t *p_meas_result_list_eutra
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_list_eutra != PNULL);

    /* This function composes meas_result_list_eutra */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_list_eutra->count > MAX_CELL_REPORT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list_eutra->count] should be less than"
            " or equal to MAX_CELL_REPORT. Incorrect value %u received.", p_meas_result_list_eutra->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_list_eutra->count, "count");
    *pp_buffer += sizeof(p_meas_result_list_eutra->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list_eutra->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_meas_result_eutra(pp_buffer, &p_meas_result_list_eutra->meas_result_eutra[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_meas_result2_eutra_len
(
    rrc_meas_result2_eutra_t *p_rrc_meas_result2_eutra
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_meas_result2_eutra != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_meas_result2_eutra->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_meas_result2_eutra->carrier_freq);

    /* Get length of IE */
    length += rrc_il_get_meas_result_list_eutra_len(&p_rrc_meas_result2_eutra->meas_result_list_eutra);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_meas_result2_eutra
(
    U8  **pp_buffer,
    rrc_meas_result2_eutra_t *p_rrc_meas_result2_eutra
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_meas_result2_eutra != PNULL);

    /* This function composes rrc_meas_result2_eutra */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_meas_result2_eutra->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_meas_result2_eutra->bitmask);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_meas_result2_eutra->carrier_freq, "carrier_freq");
    *pp_buffer += sizeof(p_rrc_meas_result2_eutra->carrier_freq);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_list_eutra(pp_buffer, &p_rrc_meas_result2_eutra->meas_result_list_eutra))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_meas_result_list2_eutra_len
(
    rrc_meas_result_list2_eutra_t *p_rrc_meas_result_list2_eutra
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_meas_result_list2_eutra != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_meas_result_list2_eutra->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_meas_result_list2_eutra->count < 1) || (p_rrc_meas_result_list2_eutra->count > MAX_FREQ))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_meas_result_list2_eutra->count] should be in range "
            "1 to MAX_FREQ. Incorrect value %d received.", p_rrc_meas_result_list2_eutra->count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_meas_result_list2_eutra->count; loop++)
        {
            length += rrc_il_get_rrc_meas_result2_eutra_len(&p_rrc_meas_result_list2_eutra->meas_result_eutra[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_meas_result_list2_eutra
(
    U8  **pp_buffer,
    rrc_meas_result_list2_eutra_t *p_rrc_meas_result_list2_eutra
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_meas_result_list2_eutra != PNULL);

    /* This function composes rrc_meas_result_list2_eutra */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_meas_result_list2_eutra->count < 1) || (p_rrc_meas_result_list2_eutra->count > MAX_FREQ))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_meas_result_list2_eutra->count] should be in range "
            "1 to MAX_FREQ. Incorrect value %u received.", p_rrc_meas_result_list2_eutra->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_meas_result_list2_eutra->count, "count");
    *pp_buffer += sizeof(p_rrc_meas_result_list2_eutra->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_meas_result_list2_eutra->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_meas_result2_eutra(pp_buffer, &p_rrc_meas_result_list2_eutra->meas_result_eutra[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_utra_phys_cell_id_len
(
    meas_result_utra_phys_cell_id_t *p_meas_result_utra_phys_cell_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_utra_phys_cell_id != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_utra_phys_cell_id->bitmask);

    /* Optional element */
    if(p_meas_result_utra_phys_cell_id->bitmask & MEAS_RESULT_UTRA_PHYS_CELL_ID_FDD_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_utra_phys_cell_id->fdd);
    }

    /* Optional element */
    if(p_meas_result_utra_phys_cell_id->bitmask & MEAS_RESULT_UTRA_PHYS_CELL_ID_TDD_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_utra_phys_cell_id->tdd);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_utra_phys_cell_id
(
    U8  **pp_buffer,
    meas_result_utra_phys_cell_id_t *p_meas_result_utra_phys_cell_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_utra_phys_cell_id != PNULL);

    /* This function composes meas_result_utra_phys_cell_id */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_utra_phys_cell_id->bitmask, "bitmask");
    *pp_buffer += sizeof(p_meas_result_utra_phys_cell_id->bitmask);

    /* Optional element */
    if(p_meas_result_utra_phys_cell_id->bitmask & MEAS_RESULT_UTRA_PHYS_CELL_ID_FDD_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_utra_phys_cell_id->fdd > 511))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_utra_phys_cell_id->fdd] should be less than"
            " or equal to 511. Incorrect value %u received.", p_meas_result_utra_phys_cell_id->fdd);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_utra_phys_cell_id->fdd, "fdd");
    *pp_buffer += sizeof(p_meas_result_utra_phys_cell_id->fdd);
    }

    /* Optional element */
    if(p_meas_result_utra_phys_cell_id->bitmask & MEAS_RESULT_UTRA_PHYS_CELL_ID_TDD_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_utra_phys_cell_id->tdd > 127))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_utra_phys_cell_id->tdd] should be less than"
            " or equal to 127. Incorrect value %u received.", p_meas_result_utra_phys_cell_id->tdd);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_utra_phys_cell_id->tdd, "tdd");
    *pp_buffer += sizeof(p_meas_result_utra_phys_cell_id->tdd);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_cell_global_id_utra_len
(
    cell_global_id_utra_t *p_cell_global_id_utra
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_cell_global_id_utra != PNULL);

    /* Get length of IE */
    length += rrc_il_get_plmn_identity_len(&p_cell_global_id_utra->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_cell_global_id_utra->cell_identity);

    return length;
}

static
rrc_return_et
rrc_il_compose_cell_global_id_utra
(
    U8  **pp_buffer,
    cell_global_id_utra_t *p_cell_global_id_utra
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_cell_global_id_utra != PNULL);

    /* This function composes cell_global_id_utra */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_plmn_identity(pp_buffer, &p_cell_global_id_utra->plmn_identity))
    {
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_utra->cell_identity); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_cell_global_id_utra->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_utra_cgi_info_len
(
    meas_result_utra_cgi_info_t *p_meas_result_utra_cgi_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_utra_cgi_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_utra_cgi_info->bitmask);

    /* Get length of IE */
    length += rrc_il_get_cell_global_id_utra_len(&p_meas_result_utra_cgi_info->cell_global_id);

    /* Optional element */
    if(p_meas_result_utra_cgi_info->bitmask & MEAS_RESULT_UTRA_CGI_INFO_LOCATION_AREA_CODE_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_meas_result_utra_cgi_info->location_area_code);
    }

    /* Optional element */
    if(p_meas_result_utra_cgi_info->bitmask & MEAS_RESULT_UTRA_CGI_INFO_ROUTING_AREA_CODE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_utra_cgi_info->routing_area_code);
    }

    /* Optional element */
    if(p_meas_result_utra_cgi_info->bitmask & MEAS_RESULT_UTRA_CGI_INFO_PLMN_IDENTITY_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_plmn_identity_list2_len(&p_meas_result_utra_cgi_info->plmn_identity_list);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_utra_cgi_info
(
    U8  **pp_buffer,
    meas_result_utra_cgi_info_t *p_meas_result_utra_cgi_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_utra_cgi_info != PNULL);

    /* This function composes meas_result_utra_cgi_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_utra_cgi_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_meas_result_utra_cgi_info->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_cell_global_id_utra(pp_buffer, &p_meas_result_utra_cgi_info->cell_global_id))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_meas_result_utra_cgi_info->bitmask & MEAS_RESULT_UTRA_CGI_INFO_LOCATION_AREA_CODE_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_meas_result_utra_cgi_info->location_area_code); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_meas_result_utra_cgi_info->location_area_code[loop], "location_area_code[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_meas_result_utra_cgi_info->bitmask & MEAS_RESULT_UTRA_CGI_INFO_ROUTING_AREA_CODE_PRESENT)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_utra_cgi_info->routing_area_code, "routing_area_code");
    *pp_buffer += sizeof(p_meas_result_utra_cgi_info->routing_area_code);
    }

    /* Optional element */
    if(p_meas_result_utra_cgi_info->bitmask & MEAS_RESULT_UTRA_CGI_INFO_PLMN_IDENTITY_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_plmn_identity_list2(pp_buffer, &p_meas_result_utra_cgi_info->plmn_identity_list))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_utra_meas_result_len
(
    meas_result_utra_meas_result_t *p_meas_result_utra_meas_result
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_utra_meas_result != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_utra_meas_result->bitmask);

    /* Optional element */
    if(p_meas_result_utra_meas_result->bitmask & MEAS_RESULT_UTRA_MEAS_RESULT_RSCP_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_utra_meas_result->utra_rscp);
    }

    /* Optional element */
    if(p_meas_result_utra_meas_result->bitmask & MEAS_RESULT_UTRA_MEAS_RESULT_ECN0_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_utra_meas_result->utra_ecn0);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_utra_meas_result
(
    U8  **pp_buffer,
    meas_result_utra_meas_result_t *p_meas_result_utra_meas_result
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_utra_meas_result != PNULL);

    /* This function composes meas_result_utra_meas_result */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_utra_meas_result->bitmask, "bitmask");
    *pp_buffer += sizeof(p_meas_result_utra_meas_result->bitmask);

    /* Optional element */
    if(p_meas_result_utra_meas_result->bitmask & MEAS_RESULT_UTRA_MEAS_RESULT_RSCP_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_meas_result_utra_meas_result->utra_rscp < -5) || (p_meas_result_utra_meas_result->utra_rscp > 91))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_utra_meas_result->utra_rscp] should be in range "
            "-5 to 91. Incorrect value %d received.", p_meas_result_utra_meas_result->utra_rscp);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_meas_result_utra_meas_result->utra_rscp, "utra_rscp");
    *pp_buffer += sizeof(p_meas_result_utra_meas_result->utra_rscp);
    }

    /* Optional element */
    if(p_meas_result_utra_meas_result->bitmask & MEAS_RESULT_UTRA_MEAS_RESULT_ECN0_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_utra_meas_result->utra_ecn0 > 49))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_utra_meas_result->utra_ecn0] should be less than"
            " or equal to 49. Incorrect value %u received.", p_meas_result_utra_meas_result->utra_ecn0);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_utra_meas_result->utra_ecn0, "utra_ecn0");
    *pp_buffer += sizeof(p_meas_result_utra_meas_result->utra_ecn0);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_utra_len
(
    meas_result_utra_t *p_meas_result_utra
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_utra != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_utra->bitmask);

    /* Get length of IE */
    length += rrc_il_get_meas_result_utra_phys_cell_id_len(&p_meas_result_utra->phys_cell_id);

    /* Optional element */
    if(p_meas_result_utra->bitmask & MEAS_RESULT_UTRA_CGI_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_meas_result_utra_cgi_info_len(&p_meas_result_utra->cgi_info);
    }

    /* Optional element */
    if(p_meas_result_utra->bitmask & MEAS_RESULT_UTRA_CSG_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_meas_result_csg_info_len(&p_meas_result_utra->utra_csg_info);
    }

    /* Get length of IE */
    length += rrc_il_get_meas_result_utra_meas_result_len(&p_meas_result_utra->meas_result);

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_utra
(
    U8  **pp_buffer,
    meas_result_utra_t *p_meas_result_utra
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_utra != PNULL);

    /* This function composes meas_result_utra */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_utra->bitmask, "bitmask");
    *pp_buffer += sizeof(p_meas_result_utra->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_utra_phys_cell_id(pp_buffer, &p_meas_result_utra->phys_cell_id))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_meas_result_utra->bitmask & MEAS_RESULT_UTRA_CGI_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_utra_cgi_info(pp_buffer, &p_meas_result_utra->cgi_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_meas_result_utra->bitmask & MEAS_RESULT_UTRA_CSG_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_csg_info(pp_buffer, &p_meas_result_utra->utra_csg_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_utra_meas_result(pp_buffer, &p_meas_result_utra->meas_result))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_list_utra_len
(
    meas_result_list_utra_t *p_meas_result_list_utra
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_list_utra != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_list_utra->count);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_list_utra->count > MAX_CELL_REPORT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list_utra->count] should be less than"
            " or equal to MAX_CELL_REPORT. Incorrect value %d received.", p_meas_result_list_utra->count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list_utra->count; loop++)
        {
            length += rrc_il_get_meas_result_utra_len(&p_meas_result_list_utra->meas_result_utra[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_list_utra
(
    U8  **pp_buffer,
    meas_result_list_utra_t *p_meas_result_list_utra
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_list_utra != PNULL);

    /* This function composes meas_result_list_utra */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_list_utra->count > MAX_CELL_REPORT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list_utra->count] should be less than"
            " or equal to MAX_CELL_REPORT. Incorrect value %u received.", p_meas_result_list_utra->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_list_utra->count, "count");
    *pp_buffer += sizeof(p_meas_result_list_utra->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list_utra->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_meas_result_utra(pp_buffer, &p_meas_result_list_utra->meas_result_utra[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_meas_result2_utra_len
(
    rrc_meas_result2_utra_t *p_rrc_meas_result2_utra
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_meas_result2_utra != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_meas_result2_utra->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_meas_result2_utra->carrier_freq);

    /* Get length of IE */
    length += rrc_il_get_meas_result_list_utra_len(&p_rrc_meas_result2_utra->meas_result_list_utra);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_meas_result2_utra
(
    U8  **pp_buffer,
    rrc_meas_result2_utra_t *p_rrc_meas_result2_utra
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_meas_result2_utra != PNULL);

    /* This function composes rrc_meas_result2_utra */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_meas_result2_utra->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_meas_result2_utra->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_meas_result2_utra->carrier_freq > 16383))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_meas_result2_utra->carrier_freq] should be less than"
            " or equal to 16383. Incorrect value %u received.", p_rrc_meas_result2_utra->carrier_freq);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_meas_result2_utra->carrier_freq, "carrier_freq");
    *pp_buffer += sizeof(p_rrc_meas_result2_utra->carrier_freq);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_list_utra(pp_buffer, &p_rrc_meas_result2_utra->meas_result_list_utra))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_meas_result_list2_utra_len
(
    rrc_meas_result_list2_utra_t *p_rrc_meas_result_list2_utra
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_meas_result_list2_utra != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_meas_result_list2_utra->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_meas_result_list2_utra->count < 1) || (p_rrc_meas_result_list2_utra->count > MAX_FREQ))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_meas_result_list2_utra->count] should be in range "
            "1 to MAX_FREQ. Incorrect value %d received.", p_rrc_meas_result_list2_utra->count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_meas_result_list2_utra->count; loop++)
        {
            length += rrc_il_get_rrc_meas_result2_utra_len(&p_rrc_meas_result_list2_utra->meas_result_utra[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_meas_result_list2_utra
(
    U8  **pp_buffer,
    rrc_meas_result_list2_utra_t *p_rrc_meas_result_list2_utra
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_meas_result_list2_utra != PNULL);

    /* This function composes rrc_meas_result_list2_utra */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_meas_result_list2_utra->count < 1) || (p_rrc_meas_result_list2_utra->count > MAX_FREQ))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_meas_result_list2_utra->count] should be in range "
            "1 to MAX_FREQ. Incorrect value %u received.", p_rrc_meas_result_list2_utra->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_meas_result_list2_utra->count, "count");
    *pp_buffer += sizeof(p_rrc_meas_result_list2_utra->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_meas_result_list2_utra->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_meas_result2_utra(pp_buffer, &p_rrc_meas_result_list2_utra->meas_result_utra[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_carrier_freq_geran_len
(
    carrier_freq_geran_t *p_carrier_freq_geran
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_carrier_freq_geran != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_carrier_freq_geran->arfcn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_carrier_freq_geran->band_indicator);

    return length;
}

static
rrc_return_et
rrc_il_compose_carrier_freq_geran
(
    U8  **pp_buffer,
    carrier_freq_geran_t *p_carrier_freq_geran
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_carrier_freq_geran != PNULL);

    /* This function composes carrier_freq_geran */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_carrier_freq_geran->arfcn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_carrier_freq_geran->arfcn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_carrier_freq_geran->arfcn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_carrier_freq_geran->arfcn, "arfcn");
    *pp_buffer += sizeof(p_carrier_freq_geran->arfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_carrier_freq_geran->band_indicator > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_carrier_freq_geran->band_indicator] should be less than"
            " or equal to 1. Incorrect value %u received.", p_carrier_freq_geran->band_indicator);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_carrier_freq_geran->band_indicator, "band_indicator");
    *pp_buffer += sizeof(p_carrier_freq_geran->band_indicator);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_phys_cell_id_geran_len
(
    phys_cell_id_geran_t *p_phys_cell_id_geran
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_phys_cell_id_geran != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_phys_cell_id_geran->network_colour_code);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_phys_cell_id_geran->base_station_colour_code);

    return length;
}

static
rrc_return_et
rrc_il_compose_phys_cell_id_geran
(
    U8  **pp_buffer,
    phys_cell_id_geran_t *p_phys_cell_id_geran
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_phys_cell_id_geran != PNULL);

    /* This function composes phys_cell_id_geran */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_phys_cell_id_geran->network_colour_code, "network_colour_code");
    *pp_buffer += sizeof(p_phys_cell_id_geran->network_colour_code);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_phys_cell_id_geran->base_station_colour_code, "base_station_colour_code");
    *pp_buffer += sizeof(p_phys_cell_id_geran->base_station_colour_code);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_cell_global_id_geran_len
(
    cell_global_id_geran_t *p_cell_global_id_geran
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_cell_global_id_geran != PNULL);

    /* Get length of IE */
    length += rrc_il_get_plmn_identity_len(&p_cell_global_id_geran->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_cell_global_id_geran->location_area_code);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_cell_global_id_geran->cell_identity);

    return length;
}

static
rrc_return_et
rrc_il_compose_cell_global_id_geran
(
    U8  **pp_buffer,
    cell_global_id_geran_t *p_cell_global_id_geran
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_cell_global_id_geran != PNULL);

    /* This function composes cell_global_id_geran */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_plmn_identity(pp_buffer, &p_cell_global_id_geran->plmn_identity))
    {
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_geran->location_area_code); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_cell_global_id_geran->location_area_code[loop], "location_area_code[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_geran->cell_identity); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_cell_global_id_geran->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_geran_cgi_info_len
(
    meas_result_geran_cgi_info_t *p_meas_result_geran_cgi_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_geran_cgi_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_geran_cgi_info->bitmask);

    /* Get length of IE */
    length += rrc_il_get_cell_global_id_geran_len(&p_meas_result_geran_cgi_info->cell_global_id);

    /* Optional element */
    if(p_meas_result_geran_cgi_info->bitmask & MEAS_RESULT_GERAN_CGI_INFO_ROUTING_AREA_CODE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_geran_cgi_info->routing_area_code);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_geran_cgi_info
(
    U8  **pp_buffer,
    meas_result_geran_cgi_info_t *p_meas_result_geran_cgi_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_geran_cgi_info != PNULL);

    /* This function composes meas_result_geran_cgi_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_geran_cgi_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_meas_result_geran_cgi_info->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_cell_global_id_geran(pp_buffer, &p_meas_result_geran_cgi_info->cell_global_id))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_meas_result_geran_cgi_info->bitmask & MEAS_RESULT_GERAN_CGI_INFO_ROUTING_AREA_CODE_PRESENT)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_geran_cgi_info->routing_area_code, "routing_area_code");
    *pp_buffer += sizeof(p_meas_result_geran_cgi_info->routing_area_code);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_geran_meas_result_len
(
    meas_result_geran_meas_result_t *p_meas_result_geran_meas_result
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_geran_meas_result != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_geran_meas_result->rssi);

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_geran_meas_result
(
    U8  **pp_buffer,
    meas_result_geran_meas_result_t *p_meas_result_geran_meas_result
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_geran_meas_result != PNULL);

    /* This function composes meas_result_geran_meas_result */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_geran_meas_result->rssi > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_geran_meas_result->rssi] should be less than"
            " or equal to 63. Incorrect value %u received.", p_meas_result_geran_meas_result->rssi);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_geran_meas_result->rssi, "rssi");
    *pp_buffer += sizeof(p_meas_result_geran_meas_result->rssi);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_geran_len
(
    meas_result_geran_t *p_meas_result_geran
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_geran != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_geran->bitmask);

    /* Get length of IE */
    length += rrc_il_get_carrier_freq_geran_len(&p_meas_result_geran->carrier_freq);

    /* Get length of IE */
    length += rrc_il_get_phys_cell_id_geran_len(&p_meas_result_geran->phys_cell_id);

    /* Optional element */
    if(p_meas_result_geran->bitmask & MEAS_RESULT_GERAN_CGI_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_meas_result_geran_cgi_info_len(&p_meas_result_geran->cgi_info);
    }

    /* Get length of IE */
    length += rrc_il_get_meas_result_geran_meas_result_len(&p_meas_result_geran->meas_result);

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_geran
(
    U8  **pp_buffer,
    meas_result_geran_t *p_meas_result_geran
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_geran != PNULL);

    /* This function composes meas_result_geran */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_geran->bitmask, "bitmask");
    *pp_buffer += sizeof(p_meas_result_geran->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_carrier_freq_geran(pp_buffer, &p_meas_result_geran->carrier_freq))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_phys_cell_id_geran(pp_buffer, &p_meas_result_geran->phys_cell_id))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_meas_result_geran->bitmask & MEAS_RESULT_GERAN_CGI_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_geran_cgi_info(pp_buffer, &p_meas_result_geran->cgi_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_geran_meas_result(pp_buffer, &p_meas_result_geran->meas_result))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_list_geran_len
(
    meas_result_list_geran_t *p_meas_result_list_geran
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_list_geran != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_list_geran->count);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_list_geran->count > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list_geran->count] should be less than"
            " or equal to 8. Incorrect value %d received.", p_meas_result_list_geran->count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list_geran->count; loop++)
        {
            length += rrc_il_get_meas_result_geran_len(&p_meas_result_list_geran->meas_result_geran[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_list_geran
(
    U8  **pp_buffer,
    meas_result_list_geran_t *p_meas_result_list_geran
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_list_geran != PNULL);

    /* This function composes meas_result_list_geran */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_list_geran->count > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list_geran->count] should be less than"
            " or equal to 8. Incorrect value %u received.", p_meas_result_list_geran->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_list_geran->count, "count");
    *pp_buffer += sizeof(p_meas_result_list_geran->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list_geran->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_meas_result_geran(pp_buffer, &p_meas_result_list_geran->meas_result_geran[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_carrier_freq_cdma2000_len
(
    carrier_freq_cdma2000_t *p_carrier_freq_cdma2000
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_carrier_freq_cdma2000 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_carrier_freq_cdma2000->band_class);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_carrier_freq_cdma2000->arfcn);

    return length;
}

static
rrc_return_et
rrc_il_compose_carrier_freq_cdma2000
(
    U8  **pp_buffer,
    carrier_freq_cdma2000_t *p_carrier_freq_cdma2000
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_carrier_freq_cdma2000 != PNULL);

    /* This function composes carrier_freq_cdma2000 */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_carrier_freq_cdma2000->band_class > 31))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_carrier_freq_cdma2000->band_class] should be less than"
            " or equal to 31. Incorrect value %u received.", p_carrier_freq_cdma2000->band_class);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_carrier_freq_cdma2000->band_class, "band_class");
    *pp_buffer += sizeof(p_carrier_freq_cdma2000->band_class);

    /* Check for correct range [H - higher boundary] */
    if ((p_carrier_freq_cdma2000->arfcn > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_carrier_freq_cdma2000->arfcn] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_carrier_freq_cdma2000->arfcn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_carrier_freq_cdma2000->arfcn, "arfcn");
    *pp_buffer += sizeof(p_carrier_freq_cdma2000->arfcn);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_cell_global_id_cdma2000_len
(
    cell_global_id_cdma2000_t *p_cell_global_id_cdma2000
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_cell_global_id_cdma2000 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_cell_global_id_cdma2000->bitmask);

    /* Optional element */
    if(p_cell_global_id_cdma2000->bitmask & CELL_GLOBAL_ID_CDMA2000_1XRTT_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_cell_global_id_cdma2000->cell_global_id_1xrtt);
    }

    /* Optional element */
    if(p_cell_global_id_cdma2000->bitmask & CELL_GLOBAL_ID_CDMA2000_HRPD_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_cell_global_id_cdma2000->cell_global_id_hrpd);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_cell_global_id_cdma2000
(
    U8  **pp_buffer,
    cell_global_id_cdma2000_t *p_cell_global_id_cdma2000
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_cell_global_id_cdma2000 != PNULL);

    /* This function composes cell_global_id_cdma2000 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_cell_global_id_cdma2000->bitmask, "bitmask");
    *pp_buffer += sizeof(p_cell_global_id_cdma2000->bitmask);

    /* Optional element */
    if(p_cell_global_id_cdma2000->bitmask & CELL_GLOBAL_ID_CDMA2000_1XRTT_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_cdma2000->cell_global_id_1xrtt); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_cell_global_id_cdma2000->cell_global_id_1xrtt[loop], "cell_global_id_1xrtt[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_cell_global_id_cdma2000->bitmask & CELL_GLOBAL_ID_CDMA2000_HRPD_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_cdma2000->cell_global_id_hrpd); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_cell_global_id_cdma2000->cell_global_id_hrpd[loop], "cell_global_id_hrpd[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_cdma2000_meas_result_len
(
    meas_result_cdma2000_meas_result_t *p_meas_result_cdma2000_meas_result
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_cdma2000_meas_result != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_cdma2000_meas_result->bitmask);

    /* Optional element */
    if(p_meas_result_cdma2000_meas_result->bitmask & MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_cdma2000_meas_result->pilot_pn_phase);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_cdma2000_meas_result->pilot_strength);

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_cdma2000_meas_result
(
    U8  **pp_buffer,
    meas_result_cdma2000_meas_result_t *p_meas_result_cdma2000_meas_result
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_cdma2000_meas_result != PNULL);

    /* This function composes meas_result_cdma2000_meas_result */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_cdma2000_meas_result->bitmask, "bitmask");
    *pp_buffer += sizeof(p_meas_result_cdma2000_meas_result->bitmask);

    /* Optional element */
    if(p_meas_result_cdma2000_meas_result->bitmask & MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_cdma2000_meas_result->pilot_pn_phase > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_cdma2000_meas_result->pilot_pn_phase] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_meas_result_cdma2000_meas_result->pilot_pn_phase);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_cdma2000_meas_result->pilot_pn_phase, "pilot_pn_phase");
    *pp_buffer += sizeof(p_meas_result_cdma2000_meas_result->pilot_pn_phase);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_cdma2000_meas_result->pilot_strength > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_cdma2000_meas_result->pilot_strength] should be less than"
            " or equal to 63. Incorrect value %u received.", p_meas_result_cdma2000_meas_result->pilot_strength);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_cdma2000_meas_result->pilot_strength, "pilot_strength");
    *pp_buffer += sizeof(p_meas_result_cdma2000_meas_result->pilot_strength);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_cdma2000_len
(
    meas_result_cdma2000_t *p_meas_result_cdma2000
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_cdma2000 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_cdma2000->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_cdma2000->phy_cell_id);

    /* Optional element */
    if(p_meas_result_cdma2000->bitmask & MEAS_RESULT_CDMA2000_CGI_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_cell_global_id_cdma2000_len(&p_meas_result_cdma2000->cgi_info);
    }

    /* Get length of IE */
    length += rrc_il_get_meas_result_cdma2000_meas_result_len(&p_meas_result_cdma2000->meas_result);

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_cdma2000
(
    U8  **pp_buffer,
    meas_result_cdma2000_t *p_meas_result_cdma2000
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_cdma2000 != PNULL);

    /* This function composes meas_result_cdma2000 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_cdma2000->bitmask, "bitmask");
    *pp_buffer += sizeof(p_meas_result_cdma2000->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_cdma2000->phy_cell_id > 511))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_cdma2000->phy_cell_id] should be less than"
            " or equal to 511. Incorrect value %u received.", p_meas_result_cdma2000->phy_cell_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_meas_result_cdma2000->phy_cell_id, "phy_cell_id");
    *pp_buffer += sizeof(p_meas_result_cdma2000->phy_cell_id);

    /* Optional element */
    if(p_meas_result_cdma2000->bitmask & MEAS_RESULT_CDMA2000_CGI_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_cell_global_id_cdma2000(pp_buffer, &p_meas_result_cdma2000->cgi_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_cdma2000_meas_result(pp_buffer, &p_meas_result_cdma2000->meas_result))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_list_cdma2000_len
(
    meas_result_list_cdma2000_t *p_meas_result_list_cdma2000
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_list_cdma2000 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_list_cdma2000->count);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_list_cdma2000->count > MAX_CELL_REPORT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list_cdma2000->count] should be less than"
            " or equal to MAX_CELL_REPORT. Incorrect value %d received.", p_meas_result_list_cdma2000->count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list_cdma2000->count; loop++)
        {
            length += rrc_il_get_meas_result_cdma2000_len(&p_meas_result_list_cdma2000->meas_result_cdma2000[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_list_cdma2000
(
    U8  **pp_buffer,
    meas_result_list_cdma2000_t *p_meas_result_list_cdma2000
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_list_cdma2000 != PNULL);

    /* This function composes meas_result_list_cdma2000 */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_list_cdma2000->count > MAX_CELL_REPORT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list_cdma2000->count] should be less than"
            " or equal to MAX_CELL_REPORT. Incorrect value %u received.", p_meas_result_list_cdma2000->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_list_cdma2000->count, "count");
    *pp_buffer += sizeof(p_meas_result_list_cdma2000->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list_cdma2000->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_meas_result_cdma2000(pp_buffer, &p_meas_result_list_cdma2000->meas_result_cdma2000[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_results_cdma2000_len
(
    meas_results_cdma2000_t *p_meas_results_cdma2000
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_results_cdma2000 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_results_cdma2000->pre_registration_status_hrpd);

    /* Get length of IE */
    length += rrc_il_get_meas_result_list_cdma2000_len(&p_meas_results_cdma2000->meas_result_list_cdma2000);

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_results_cdma2000
(
    U8  **pp_buffer,
    meas_results_cdma2000_t *p_meas_results_cdma2000
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_results_cdma2000 != PNULL);

    /* This function composes meas_results_cdma2000 */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_results_cdma2000->pre_registration_status_hrpd > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_results_cdma2000->pre_registration_status_hrpd] should be less than"
            " or equal to 1. Incorrect value %u received.", p_meas_results_cdma2000->pre_registration_status_hrpd);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_results_cdma2000->pre_registration_status_hrpd, "pre_registration_status_hrpd");
    *pp_buffer += sizeof(p_meas_results_cdma2000->pre_registration_status_hrpd);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_list_cdma2000(pp_buffer, &p_meas_results_cdma2000->meas_result_list_cdma2000))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_meas_result2_cdma2000_len
(
    rrc_meas_result2_cdma2000_t *p_rrc_meas_result2_cdma2000
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_meas_result2_cdma2000 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_meas_result2_cdma2000->bitmask);

    /* Get length of IE */
    length += rrc_il_get_carrier_freq_cdma2000_len(&p_rrc_meas_result2_cdma2000->carrier_freq_cdma2000);

    /* Get length of IE */
    length += rrc_il_get_meas_results_cdma2000_len(&p_rrc_meas_result2_cdma2000->meas_result_cdma2000);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_meas_result2_cdma2000
(
    U8  **pp_buffer,
    rrc_meas_result2_cdma2000_t *p_rrc_meas_result2_cdma2000
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_meas_result2_cdma2000 != PNULL);

    /* This function composes rrc_meas_result2_cdma2000 */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_meas_result2_cdma2000->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_meas_result2_cdma2000->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_carrier_freq_cdma2000(pp_buffer, &p_rrc_meas_result2_cdma2000->carrier_freq_cdma2000))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_results_cdma2000(pp_buffer, &p_rrc_meas_result2_cdma2000->meas_result_cdma2000))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_meas_result_list2_cdma2000_len
(
    meas_result_list2_cdma2000_t *p_meas_result_list2_cdma2000
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_meas_result_list2_cdma2000 != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_meas_result_list2_cdma2000->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_meas_result_list2_cdma2000->count < 1) || (p_meas_result_list2_cdma2000->count > MAX_FREQ))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list2_cdma2000->count] should be in range "
            "1 to MAX_FREQ. Incorrect value %d received.", p_meas_result_list2_cdma2000->count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list2_cdma2000->count; loop++)
        {
            length += rrc_il_get_rrc_meas_result2_cdma2000_len(&p_meas_result_list2_cdma2000->meas_result_cdma2000[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_meas_result_list2_cdma2000
(
    U8  **pp_buffer,
    meas_result_list2_cdma2000_t *p_meas_result_list2_cdma2000
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_meas_result_list2_cdma2000 != PNULL);

    /* This function composes meas_result_list2_cdma2000 */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_meas_result_list2_cdma2000->count < 1) || (p_meas_result_list2_cdma2000->count > MAX_FREQ))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list2_cdma2000->count] should be in range "
            "1 to MAX_FREQ. Incorrect value %u received.", p_meas_result_list2_cdma2000->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_meas_result_list2_cdma2000->count, "count");
    *pp_buffer += sizeof(p_meas_result_list2_cdma2000->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list2_cdma2000->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_meas_result2_cdma2000(pp_buffer, &p_meas_result_list2_cdma2000->meas_result_cdma2000[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlf_report_meas_result_neigh_cell_len
(
    rrc_rlf_report_meas_result_neigh_cell_t *p_rrc_rlf_report_meas_result_neigh_cell
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlf_report_meas_result_neigh_cell != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_report_meas_result_neigh_cell->bitmask);

    /* Optional element */
    if(p_rrc_rlf_report_meas_result_neigh_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_LIST_EUTRA_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_meas_result_list2_eutra_len(&p_rrc_rlf_report_meas_result_neigh_cell->meas_result_list_eutra);
    }

    /* Optional element */
    if(p_rrc_rlf_report_meas_result_neigh_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_LIST_UTRA_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_meas_result_list2_utra_len(&p_rrc_rlf_report_meas_result_neigh_cell->meas_result_list_utra);
    }

    /* Optional element */
    if(p_rrc_rlf_report_meas_result_neigh_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_LIST_GERAN_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_meas_result_list_geran_len(&p_rrc_rlf_report_meas_result_neigh_cell->meas_result_list_geran);
    }

    /* Optional element */
    if(p_rrc_rlf_report_meas_result_neigh_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_LIST_CDMA2000_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_meas_result_list2_cdma2000_len(&p_rrc_rlf_report_meas_result_neigh_cell->meas_result_list2_cdma2000);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlf_report_meas_result_neigh_cell
(
    U8  **pp_buffer,
    rrc_rlf_report_meas_result_neigh_cell_t *p_rrc_rlf_report_meas_result_neigh_cell
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlf_report_meas_result_neigh_cell != PNULL);

    /* This function composes rrc_rlf_report_meas_result_neigh_cell */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlf_report_meas_result_neigh_cell->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_rlf_report_meas_result_neigh_cell->bitmask);

    /* Optional element */
    if(p_rrc_rlf_report_meas_result_neigh_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_LIST_EUTRA_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_meas_result_list2_eutra(pp_buffer, &p_rrc_rlf_report_meas_result_neigh_cell->meas_result_list_eutra))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rlf_report_meas_result_neigh_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_LIST_UTRA_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_meas_result_list2_utra(pp_buffer, &p_rrc_rlf_report_meas_result_neigh_cell->meas_result_list_utra))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rlf_report_meas_result_neigh_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_LIST_GERAN_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_list_geran(pp_buffer, &p_rrc_rlf_report_meas_result_neigh_cell->meas_result_list_geran))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rlf_report_meas_result_neigh_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_LIST_CDMA2000_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_meas_result_list2_cdma2000(pp_buffer, &p_rrc_rlf_report_meas_result_neigh_cell->meas_result_list2_cdma2000))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlf_dynamic_string_len
(
    rrc_rlf_dynamic_string_t *p_rrc_rlf_dynamic_string
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlf_dynamic_string != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_dynamic_string->data_length);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrc_rlf_dynamic_string->data_length * sizeof(p_rrc_rlf_dynamic_string->data[0]));

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlf_dynamic_string
(
    U8  **pp_buffer,
    rrc_rlf_dynamic_string_t *p_rrc_rlf_dynamic_string
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlf_dynamic_string != PNULL);

    /* This function composes rrc_rlf_dynamic_string */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlf_dynamic_string->data_length > 64))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlf_dynamic_string->data_length] should be less than"
            " or equal to 64. Incorrect value %u received.", p_rrc_rlf_dynamic_string->data_length);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlf_dynamic_string->data_length, "data_length");
    *pp_buffer += sizeof(p_rrc_rlf_dynamic_string->data_length);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_rlf_dynamic_string->data_length; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlf_dynamic_string->data[loop], "data[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlf_report_location_coordinates_len
(
    rrc_rlf_report_location_coordinates_t *p_rrc_rlf_report_location_coordinates
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlf_report_location_coordinates != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_report_location_coordinates->bitmask);

    /* Optional element */
    if(p_rrc_rlf_report_location_coordinates->bitmask & RRC_RLF_LOCATION_CORD_ELLIPSOID_POINT_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_rlf_dynamic_string_len(&p_rrc_rlf_report_location_coordinates->ellipsoid_point);
    }

    /* Optional element */
    if(p_rrc_rlf_report_location_coordinates->bitmask & RRC_RLF_LOCATION_CORD_ELLIPSOID_POINT_ALTITUDE_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_rlf_dynamic_string_len(&p_rrc_rlf_report_location_coordinates->ellipsoid_point_altitude);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlf_report_location_coordinates
(
    U8  **pp_buffer,
    rrc_rlf_report_location_coordinates_t *p_rrc_rlf_report_location_coordinates
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlf_report_location_coordinates != PNULL);

    /* This function composes rrc_rlf_report_location_coordinates */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlf_report_location_coordinates->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_rlf_report_location_coordinates->bitmask);

    /* Optional element */
    if(p_rrc_rlf_report_location_coordinates->bitmask & RRC_RLF_LOCATION_CORD_ELLIPSOID_POINT_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_rlf_dynamic_string(pp_buffer, &p_rrc_rlf_report_location_coordinates->ellipsoid_point))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rlf_report_location_coordinates->bitmask & RRC_RLF_LOCATION_CORD_ELLIPSOID_POINT_ALTITUDE_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_rlf_dynamic_string(pp_buffer, &p_rrc_rlf_report_location_coordinates->ellipsoid_point_altitude))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlf_report_location_info_len
(
    rrc_rlf_report_location_info_t *p_rrc_rlf_report_location_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlf_report_location_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_report_location_info->bitmask);

    /* Get length of IE */
    length += rrc_il_get_rrc_rlf_report_location_coordinates_len(&p_rrc_rlf_report_location_info->location_coordiate);

    /* Optional element */
    if(p_rrc_rlf_report_location_info->bitmask & RRC_RLF_LOCATION_INFO_HORIZON_VELOCITY_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_rlf_dynamic_string_len(&p_rrc_rlf_report_location_info->horizontal_velocity);
    }

    /* Optional element */
    if(p_rrc_rlf_report_location_info->bitmask & RRC_RLF_LOCATION_INFO_GNSS_TOD_MSEC_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_rlf_dynamic_string_len(&p_rrc_rlf_report_location_info->gnss_tod_msec);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlf_report_location_info
(
    U8  **pp_buffer,
    rrc_rlf_report_location_info_t *p_rrc_rlf_report_location_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlf_report_location_info != PNULL);

    /* This function composes rrc_rlf_report_location_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlf_report_location_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_rlf_report_location_info->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_rlf_report_location_coordinates(pp_buffer, &p_rrc_rlf_report_location_info->location_coordiate))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_rrc_rlf_report_location_info->bitmask & RRC_RLF_LOCATION_INFO_HORIZON_VELOCITY_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_rlf_dynamic_string(pp_buffer, &p_rrc_rlf_report_location_info->horizontal_velocity))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rlf_report_location_info->bitmask & RRC_RLF_LOCATION_INFO_GNSS_TOD_MSEC_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_rlf_dynamic_string(pp_buffer, &p_rrc_rlf_report_location_info->gnss_tod_msec))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlf_failed_pci_arfcn_len
(
    rrc_rlf_failed_pci_arfcn_t *p_rrc_rlf_failed_pci_arfcn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlf_failed_pci_arfcn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_failed_pci_arfcn->phys_cell_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_failed_pci_arfcn->carrier_freq);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlf_failed_pci_arfcn
(
    U8  **pp_buffer,
    rrc_rlf_failed_pci_arfcn_t *p_rrc_rlf_failed_pci_arfcn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlf_failed_pci_arfcn != PNULL);

    /* This function composes rrc_rlf_failed_pci_arfcn */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlf_failed_pci_arfcn->phys_cell_id > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlf_failed_pci_arfcn->phys_cell_id] should be less than"
            " or equal to 503. Incorrect value %u received.", p_rrc_rlf_failed_pci_arfcn->phys_cell_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlf_failed_pci_arfcn->phys_cell_id, "phys_cell_id");
    *pp_buffer += sizeof(p_rrc_rlf_failed_pci_arfcn->phys_cell_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlf_failed_pci_arfcn->carrier_freq, "carrier_freq");
    *pp_buffer += sizeof(p_rrc_rlf_failed_pci_arfcn->carrier_freq);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlf_failed_pcellid_len
(
    rrc_rlf_failed_pcellid_t *p_rrc_rlf_failed_pcellid
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlf_failed_pcellid != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_failed_pcellid->bitmask);

    /* Optional element */
    if(p_rrc_rlf_failed_pcellid->bitmask & RRC_RLF_FAILED_PCI_CELL_GLOBAL_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrm_cell_global_id_eutra_len(&p_rrc_rlf_failed_pcellid->cell_global_id);
    }

    /* Optional element */
    if(p_rrc_rlf_failed_pcellid->bitmask & RRC_RLF_FAILED_PCI_ARFCN_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_rlf_failed_pci_arfcn_len(&p_rrc_rlf_failed_pcellid->pci_arfcn);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlf_failed_pcellid
(
    U8  **pp_buffer,
    rrc_rlf_failed_pcellid_t *p_rrc_rlf_failed_pcellid
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlf_failed_pcellid != PNULL);

    /* This function composes rrc_rlf_failed_pcellid */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlf_failed_pcellid->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_rlf_failed_pcellid->bitmask);

    /* Optional element */
    if(p_rrc_rlf_failed_pcellid->bitmask & RRC_RLF_FAILED_PCI_CELL_GLOBAL_ID_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrm_cell_global_id_eutra(pp_buffer, &p_rrc_rlf_failed_pcellid->cell_global_id))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rlf_failed_pcellid->bitmask & RRC_RLF_FAILED_PCI_ARFCN_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_rlf_failed_pci_arfcn(pp_buffer, &p_rrc_rlf_failed_pcellid->pci_arfcn))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlf_report_len
(
    rrc_rlf_report_t *p_rrc_rlf_report
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlf_report != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_report->bitmask);

    /* Get length of IE */
    length += rrc_il_get_rrc_rlf_report_meas_result_serv_cell_len(&p_rrc_rlf_report->meas_result_serv_cell);

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_MEAS_RESULT_NEIGH_CELLS_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_rlf_report_meas_result_neigh_cell_len(&p_rrc_rlf_report->meas_result_neigh_cell);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_REESTABLISMENT_CELLID_R10_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrm_cell_global_id_eutra_len(&p_rrc_rlf_report->reestablishment_cellid_r10);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_TIME_CONN_FAILURE_R10_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_report->time_conn_failure_r10);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_CONN_FAILURE_TYPE_R10_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_report->connection_failure_type_r10);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_RLF_REPORT_LOACTION_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_rlf_report_location_info_len(&p_rrc_rlf_report->location_info);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_FAILED_PCELLID_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrc_rlf_failed_pcellid_len(&p_rrc_rlf_report->failed_pcellid);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_REESTABLISHMENT_CELLID_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrm_cell_global_id_eutra_len(&p_rrc_rlf_report->reestablish_cell_id);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_TIME_CONN_FALURE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_report->time_conn_failure);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_CONN_FALURE_TYPE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlf_report->conn_failure_type);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_PREV_PCELLID_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_rrm_cell_global_id_eutra_len(&p_rrc_rlf_report->previous_pcell_id);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlf_report
(
    U8  **pp_buffer,
    rrc_rlf_report_t *p_rrc_rlf_report
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlf_report != PNULL);

    /* This function composes rrc_rlf_report */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlf_report->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrc_rlf_report->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_rlf_report_meas_result_serv_cell(pp_buffer, &p_rrc_rlf_report->meas_result_serv_cell))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_MEAS_RESULT_NEIGH_CELLS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_rlf_report_meas_result_neigh_cell(pp_buffer, &p_rrc_rlf_report->meas_result_neigh_cell))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_REESTABLISMENT_CELLID_R10_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrm_cell_global_id_eutra(pp_buffer, &p_rrc_rlf_report->reestablishment_cellid_r10))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_TIME_CONN_FAILURE_R10_PRESENT)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlf_report->time_conn_failure_r10, "time_conn_failure_r10");
    *pp_buffer += sizeof(p_rrc_rlf_report->time_conn_failure_r10);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_CONN_FAILURE_TYPE_R10_PRESENT)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_rlf_report->connection_failure_type_r10, "connection_failure_type_r10");
    *pp_buffer += sizeof(p_rrc_rlf_report->connection_failure_type_r10);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_RLF_REPORT_LOACTION_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_rlf_report_location_info(pp_buffer, &p_rrc_rlf_report->location_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_FAILED_PCELLID_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_rlf_failed_pcellid(pp_buffer, &p_rrc_rlf_report->failed_pcellid))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_REESTABLISHMENT_CELLID_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrm_cell_global_id_eutra(pp_buffer, &p_rrc_rlf_report->reestablish_cell_id))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_TIME_CONN_FALURE_PRESENT)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlf_report->time_conn_failure, "time_conn_failure");
    *pp_buffer += sizeof(p_rrc_rlf_report->time_conn_failure);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_CONN_FALURE_TYPE_PRESENT)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_rlf_report->conn_failure_type, "conn_failure_type");
    *pp_buffer += sizeof(p_rrc_rlf_report->conn_failure_type);
    }

    /* Optional element */
    if(p_rrc_rlf_report->bitmask & RRC_UE_INFO_PREV_PCELLID_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrm_cell_global_id_eutra(pp_buffer, &p_rrc_rlf_report->previous_pcell_id))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

#ifdef ENDC_ENABLED
rrc_length_t
rrc_il_get_x2ap_nr_nbour_cell_info_len
(
    x2ap_nr_nbour_cell_info_t *p_x2ap_nr_nbour_cell_info
)
{
    rrc_length_t length = 0;
    rrc_counter_t loop = 0;

    RRC_ASSERT(p_x2ap_nr_nbour_cell_info != PNULL);

    length += (rrc_length_t)sizeof(p_x2ap_nr_nbour_cell_info->num_neigh_cell);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_nr_nbour_cell_info->num_neigh_cell) || (MAX_SERVED_CELLS < p_x2ap_nr_nbour_cell_info->num_neigh_cell))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_nbour_cell_info->num_neigh_cell] should be in range "
            "1 to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_nr_nbour_cell_info->num_neigh_cell);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_nr_nbour_cell_info->num_neigh_cell ; loop++)
    {
        /* Get length of IE */
        length += rrc_il_get_x2ap_nr_neigh_info_len(&p_x2ap_nr_nbour_cell_info->nr_neigh_cell_arr[loop].nr_neigh_info);
    }

    return length;
}

rrc_return_et 
rrc_il_compose_x2ap_nr_nbour_cell_info
(
    U8  **pp_buffer,
    x2ap_nr_nbour_cell_info_t *p_x2ap_nr_nbour_cell_info
)
{
    rrc_counter_t loop = 0;
    
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_nbour_cell_info != PNULL);

    /* This function composes x2ap_nr_nbour_cell_info_t */

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nr_nbour_cell_info->num_neigh_cell, "num_neigh_cell");
    *pp_buffer += sizeof(p_x2ap_nr_nbour_cell_info->num_neigh_cell);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_nr_nbour_cell_info->num_neigh_cell) || (MAX_SERVED_CELLS < p_x2ap_nr_nbour_cell_info->num_neigh_cell))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_nbour_cell_info->num_neigh_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_nr_nbour_cell_info->num_neigh_cell);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_nr_nbour_cell_info->num_neigh_cell; loop++)
    {
        if (RRC_FAILURE == rrc_il_compose_x2ap_nr_neigh_info(pp_buffer, 
            &p_x2ap_nr_nbour_cell_info->nr_neigh_cell_arr[loop].nr_neigh_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_nr_srvd_cell_to_management_list_len
(
    x2ap_nr_srvd_cell_to_management_list_t *p_x2ap_nr_srvd_cell_to_add_list
)
{
    rrc_length_t length = 0;
    rrc_counter_t loop = 0;

    RRC_ASSERT(p_x2ap_nr_srvd_cell_to_add_list != PNULL);

    length += (rrc_length_t)sizeof(p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management) || 
        (MAX_SERVED_CELLS < p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management; loop++)
    {
        length += (rrc_length_t)sizeof(p_x2ap_nr_srvd_cell_to_add_list->nr_served_cell_management_list[loop].bitmask);

        /* Get length of IE */
        length += rrc_il_get_x2ap_rrm_srvd_nr_cell_info_len(
            &p_x2ap_nr_srvd_cell_to_add_list->nr_served_cell_management_list[loop].served_cell_info);

        if (NR_NEIGHBOUR_INFO_PRESENT & 
            p_x2ap_nr_srvd_cell_to_add_list->nr_served_cell_management_list[loop].bitmask)
        {    
            /* Get length of IE */    
            length += rrc_il_get_x2ap_nr_nbour_cell_info_len(
                &p_x2ap_nr_srvd_cell_to_add_list->nr_served_cell_management_list[loop].neighbour_cell_info);  
        }
    }

    return length;
}

rrc_return_et 
rrc_il_compose_x2ap_nr_srvd_cell_to_management_list
(
    U8  **pp_buffer,
    x2ap_nr_srvd_cell_to_management_list_t *p_x2ap_nr_srvd_cell_to_add_list
)
{
    rrc_counter_t loop = 0;
    
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_srvd_cell_to_add_list != PNULL);

    /* This function composes x2ap_nr_srvd_cell_to_management_list_t */

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management, "num_nr_srvd_cell_to_management");
    *pp_buffer += sizeof(p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management) || 
        (MAX_SERVED_CELLS < p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management; loop++)
    {
        /* Compose parameter of basic type */
        rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nr_srvd_cell_to_add_list->nr_served_cell_management_list[loop].bitmask, "bitmask");
        *pp_buffer += sizeof(p_x2ap_nr_srvd_cell_to_add_list->nr_served_cell_management_list[loop].bitmask);
        
        /* Compose IE */
        if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_srvd_nr_cell_info(pp_buffer, 
            &p_x2ap_nr_srvd_cell_to_add_list->nr_served_cell_management_list[loop].served_cell_info))
        {
            return RRC_FAILURE;
        }

        if (NR_NEIGHBOUR_INFO_PRESENT & 
            p_x2ap_nr_srvd_cell_to_add_list->nr_served_cell_management_list[loop].bitmask)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_nr_nbour_cell_info(pp_buffer, 
                &p_x2ap_nr_srvd_cell_to_add_list->nr_served_cell_management_list[loop].neighbour_cell_info))
            { 
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_nr_srvd_cell_to_modify_list_len
(
    x2ap_nr_srvd_cell_to_modify_list_t *p_x2ap_nr_srvd_cell_to_mod_list
)
{
    rrc_length_t length = 0;
    rrc_counter_t loop = 0;

    RRC_ASSERT(p_x2ap_nr_srvd_cell_to_mod_list != PNULL);

    length += (rrc_length_t)sizeof(p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify) || 
        (MAX_SERVED_CELLS < p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify; loop++)
    {
        length += (rrc_length_t)sizeof(p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].bitmask);

        /* Get length of IE */
        length += rrc_il_get_x2ap_nr_ecgi_len(
            &p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].old_nr_cgi);

        if (RRC_ENDC_X2_NR_DEACT_IND_PRESENT & 
            p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].bitmask)
        {
            length += (rrc_length_t)sizeof(p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].deact_ind);
        }    

        /* Get length of IE */
        length += rrc_il_get_x2ap_rrm_srvd_nr_cell_info_len(
            &p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].served_cell_info);

        if (NR_NEIGHBOUR_INFO_PRESENT & 
            p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].bitmask)
        {    
            /* Get length of IE */    
            length += rrc_il_get_x2ap_nr_nbour_cell_info_len(
                &p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].neighbour_cell_info);  
        }  
    }

    return length;
}

rrc_return_et 
rrc_il_compose_x2ap_nr_srvd_cell_to_modify_list
(
    U8  **pp_buffer,
    x2ap_nr_srvd_cell_to_modify_list_t *p_x2ap_nr_srvd_cell_to_mod_list
)
{
    rrc_counter_t loop = 0;
    
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_srvd_cell_to_mod_list != PNULL);

    /* This function composes x2ap_nr_srvd_cell_to_modify_list_t */

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify, "num_nr_srvd_cell_to_modify");
    *pp_buffer += sizeof(p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify) || 
        (MAX_SERVED_CELLS < p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify; loop++)
    {
        /* Compose parameter of basic type */
        rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].bitmask, "bitmask");
        *pp_buffer += sizeof(p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].bitmask);

        /* Compose IE */
        if (RRC_FAILURE == rrc_il_compose_x2ap_nr_ecgi(pp_buffer, 
            &p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].old_nr_cgi))
        {
            return RRC_FAILURE;
        }

        if (RRC_ENDC_X2_NR_DEACT_IND_PRESENT & 
            p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].bitmask)
        {
            /* Compose parameter of basic type */
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].deact_ind, "deact_ind");
            *pp_buffer += sizeof(p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].deact_ind);
        }
        
        /* Compose IE */
        if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_srvd_nr_cell_info(pp_buffer, 
            &p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].served_cell_info))
        {
            return RRC_FAILURE;
        }

        if (NR_NEIGHBOUR_INFO_PRESENT & 
            p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].bitmask)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_nr_nbour_cell_info(pp_buffer, 
                &p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop].neighbour_cell_info))
            { 
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_nr_srvd_cell_to_delete_list_len
(
    x2ap_nr_srvd_cell_to_delete_list_t *p_x2ap_nr_srvd_cell_to_del_list
)
{
    rrc_length_t length = 0;
    rrc_counter_t loop = 0;

    RRC_ASSERT(p_x2ap_nr_srvd_cell_to_del_list != PNULL);

    length += (rrc_length_t)sizeof(p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete) || 
        (MAX_SERVED_CELLS < p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete
        && loop < MAX_SERVED_CELLS; loop++)
    {
        /* Get length of IE */
        length += rrc_il_get_x2ap_nr_ecgi_len(
            &p_x2ap_nr_srvd_cell_to_del_list->nr_served_cell_delete_list[loop]);
    }

    return length;
}

rrc_return_et 
rrc_il_compose_x2ap_nr_srvd_cell_to_delete_list
(
    U8  **pp_buffer,
    x2ap_nr_srvd_cell_to_delete_list_t *p_x2ap_nr_srvd_cell_to_del_list
)
{
    rrc_counter_t loop = 0;
    
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nr_srvd_cell_to_del_list != PNULL);

    /* This function composes x2ap_nr_srvd_cell_to_delete_list_t */

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete, "num_nr_srvd_cell_delete");
    *pp_buffer += sizeof(p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete) || 
        (MAX_SERVED_CELLS < p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete; loop++)
    {
        /* Compose IE */
        if (RRC_FAILURE == rrc_il_compose_x2ap_nr_ecgi(pp_buffer, 
            &p_x2ap_nr_srvd_cell_to_del_list->nr_served_cell_delete_list[loop]))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_endc_config_update_en_gnb_ind_len
(
    x2ap_endc_config_update_en_gnb_ind_t *p_x2_endc_config_update_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_endc_config_update_ind != PNULL);
    
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_endc_config_update_ind->bitmask);

    /* Get length of IE*/
    length += rrc_il_get_x2_gb_gnb_id_len(&p_x2_endc_config_update_ind->global_en_gnb_id);

    /* Optional element */
    if(X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MANAGEMENTLIST_PRESENT & 
        p_x2_endc_config_update_ind->bitmask)
    {
        length += rrc_il_get_x2ap_nr_srvd_cell_to_management_list_len(
            &p_x2_endc_config_update_ind->eutra_nr_cell_management);
    }

    /* Optional element */
    if(X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MODIFYLIST_PRESENT & 
        p_x2_endc_config_update_ind->bitmask)
    {
        /* Get length of IE */
        length += rrc_il_get_x2ap_nr_srvd_cell_to_modify_list_len(&p_x2_endc_config_update_ind->eutra_nr_cell_modify);
    }

    /* Optional element */
    if(X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_DELETELIST_PRESENT & 
        p_x2_endc_config_update_ind->bitmask)
    {
        /* Get length of IE */
        length += rrc_il_get_x2ap_nr_srvd_cell_to_delete_list_len(&p_x2_endc_config_update_ind->eutra_nr_cell_delete);
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_endc_config_update_en_gnb_ind
(
    U8  **pp_buffer,
    x2ap_endc_config_update_en_gnb_ind_t *p_x2_endc_config_update_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_endc_config_update_ind != PNULL);

    /* This function composes x2ap_rrm_x2_link_up_ind_st */

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2_endc_config_update_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2_endc_config_update_ind->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_gnb_id(pp_buffer, &p_x2_endc_config_update_ind->global_en_gnb_id))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MANAGEMENTLIST_PRESENT & 
        p_x2_endc_config_update_ind->bitmask)
    {
        if (RRC_FAILURE == rrc_il_compose_x2ap_nr_srvd_cell_to_management_list(pp_buffer, 
            &p_x2_endc_config_update_ind->eutra_nr_cell_management))
        {
            return RRC_FAILURE;
        }
    }

    /* Optional element */
    if(X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MODIFYLIST_PRESENT & 
        p_x2_endc_config_update_ind->bitmask)
    {
        if (RRC_FAILURE == rrc_il_compose_x2ap_nr_srvd_cell_to_modify_list(pp_buffer, 
            &p_x2_endc_config_update_ind->eutra_nr_cell_modify))
        {
            return RRC_FAILURE;
        }
    }

    /* Optional element */
    if(X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_DELETELIST_PRESENT & 
        p_x2_endc_config_update_ind->bitmask)
    {
        if (RRC_FAILURE == rrc_il_compose_x2ap_nr_srvd_cell_to_delete_list(pp_buffer, 
            &p_x2_endc_config_update_ind->eutra_nr_cell_delete))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_endc_config_update_en_gnb_ind
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_ENDC_CONFIG_UPDATE_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et 
rrc_x2apRrm_il_send_x2ap_endc_config_update_en_gnb_ind
(
    x2ap_endc_config_update_en_gnb_ind_t  *p_x2_endc_config_update_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apRrm_msg;

    RRC_ASSERT(p_x2_endc_config_update_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_endc_config_update_en_gnb_ind_len(p_x2_endc_config_update_ind);
    
    if(msg_length == RRC_FAILURE)
    {
            return RRC_FAILURE;
    }
    
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_ENDC_CONFIG_UPDATE_IND", src_module_id, dst_module_id);

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
    p_x2apRrm_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apRrm_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_ENDC_CONFIG_UPDATE_IND, msg_api_length);

    /* Fill interface header */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apRrm_msg, transaction_id, src_module_id,
        RRC_X2APRRM_MODULE_ID, X2AP_ENDC_CONFIG_UPDATE_IND, msg_length,cell_index);

    /* Fill X2AP_RRM_LINK_UP_IND message */
    p_x2apRrm_msg = p_x2apRrm_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_endc_config_update_en_gnb_ind(&p_x2apRrm_msg, p_x2_endc_config_update_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apRrm_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
} 

static rrc_length_t 
rrc_il_get_x2ap_cause_len
(
    x2ap_cause_t *p_x2ap_cause
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_cause != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_cause->type);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_cause->value);

    return length;
}

static rrc_return_et 
rrc_il_compose_x2ap_cause
(
    U8  **pp_buffer,
    x2ap_cause_t *p_x2ap_cause
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_cause != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_cause->type, "type");
    *pp_buffer += sizeof(p_x2ap_cause->type);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_cause->value, "value");
    *pp_buffer += sizeof(p_x2ap_cause->value);

    return RRC_SUCCESS;
}

static rrc_length_t 
rrc_il_get_x2ap_partial_list_len
(
    x2ap_partial_list_t *p_x2ap_parlist_list
)
{
    rrc_length_t length = 0;
    rrc_counter_t loop = 0;

    RRC_ASSERT(p_x2ap_parlist_list != PNULL);

    length += (rrc_length_t)sizeof(p_x2ap_parlist_list->nrcgi_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_parlist_list->nrcgi_count) || 
        (MAX_SERVED_CELLS < p_x2ap_parlist_list->nrcgi_count))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_parlist_list->nrcgi_count] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_parlist_list->nrcgi_count);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_parlist_list->nrcgi_count; loop++)
    {
        /* Get length of IE */
        length += rrc_il_get_x2ap_nr_ecgi_len(
            &p_x2ap_parlist_list->nrcgi_list[loop]);
    }

    return length;
}

static rrc_return_et 
rrc_il_compose_x2ap_partial_list
(
    U8  **pp_buffer,
    x2ap_partial_list_t *p_x2ap_parlist_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_parlist_list != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_parlist_list->nrcgi_count, "nrcgi_count");
    *pp_buffer += sizeof(p_x2ap_parlist_list->nrcgi_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_parlist_list->nrcgi_count) || (MAX_SERVED_CELLS < p_x2ap_parlist_list->nrcgi_count))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_parlist_list->nrcgi_count] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_parlist_list->nrcgi_count);
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_parlist_list->nrcgi_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_nr_ecgi(pp_buffer, &p_x2ap_parlist_list->nrcgi_list[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_served_cell_info_len
(
    x2ap_served_cell_info_t *p_x2ap_served_cell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_served_cell_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cell_info->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cell_info->pci);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cell_info->cell_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_served_cell_info->tac);

    /* Get length of IE */
    length += rrc_il_get_x2ap_broadcast_plmn_len(&p_x2ap_served_cell_info->bcast_plmn_info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_choice_eutra_mode_info_len(&p_x2ap_served_cell_info->choice_eutra_mode);

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_NUM_ANTENNA_INFO_PORT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cell_info->num_antenna_port);
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_prach_configuration_info_len(&p_x2ap_served_cell_info->prach_config);
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_mbsfn_subframe_info_len(&p_x2ap_served_cell_info->mbsfn_subframe_info);
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_served_cell_info->csg_identity);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_served_cell_info
(
    U8  **pp_buffer,
    x2ap_served_cell_info_t *p_x2ap_served_cell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_served_cell_info != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_served_cell_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_served_cell_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cell_info->pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cell_info->pci] should be in range [%d <= 503] !", p_x2ap_served_cell_info->pci);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_served_cell_info->pci, "pci");
    *pp_buffer += sizeof(p_x2ap_served_cell_info->pci);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cell_info->cell_id >= MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cell_info->cell_id] should be in range [0 <= %d < 8] !", p_x2ap_served_cell_info->cell_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_cell_info->cell_id, "cell_id");
    *pp_buffer += sizeof(p_x2ap_served_cell_info->cell_id);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_served_cell_info->tac); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_cell_info->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_broadcast_plmn(pp_buffer, &p_x2ap_served_cell_info->bcast_plmn_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_choice_eutra_mode_info(pp_buffer, &p_x2ap_served_cell_info->choice_eutra_mode))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_NUM_ANTENNA_INFO_PORT_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cell_info->num_antenna_port > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cell_info->num_antenna_port] should be in range [%d <= 2] !", p_x2ap_served_cell_info->num_antenna_port);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_cell_info->num_antenna_port, "num_antenna_port");
    *pp_buffer += sizeof(p_x2ap_served_cell_info->num_antenna_port);
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_prach_configuration_info(pp_buffer, &p_x2ap_served_cell_info->prach_config))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_mbsfn_subframe_info(pp_buffer, &p_x2ap_served_cell_info->mbsfn_subframe_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_served_cell_info->csg_identity); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_cell_info->csg_identity[loop], "csg_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRC_SUCCESS;
}

rrc_length_t 
rrc_il_get_x2ap_served_eutra_nr_cell_info_arr_len
(
    x2ap_served_eutra_nr_cell_info_arr_t *p_x2ap_served_eutra_nr_cell_info_arr
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_served_eutra_nr_cell_info_arr != PNULL);

    length += (rrc_length_t)sizeof(p_x2ap_served_eutra_nr_cell_info_arr->bitmask);

    /* Get length of IE */
    length += rrc_il_get_x2ap_served_cell_info_len(
        &p_x2ap_served_eutra_nr_cell_info_arr->eutra_cell_info);

    if (NR_NEIGHBOUR_INFO_PRESENT & p_x2ap_served_eutra_nr_cell_info_arr->bitmask)
    {
        /* Get length of IE */
        length += rrc_il_get_x2ap_nr_nbour_cell_info_len(
            &p_x2ap_served_eutra_nr_cell_info_arr->nr_neighbour_cell_info);    
    }

    return length;
}

rrc_return_et 
rrc_il_compose_x2ap_served_eutra_nr_cell_info_arr
(
    U8  **pp_buffer,
    x2ap_served_eutra_nr_cell_info_arr_t *p_x2ap_served_eutra_nr_cell_info_arr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_served_eutra_nr_cell_info_arr != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_served_eutra_nr_cell_info_arr->bitmask, "bitmask");
    *pp_buffer += sizeof(U16);

    if (RRC_FAILURE == rrc_il_compose_x2ap_served_cell_info(pp_buffer, &p_x2ap_served_eutra_nr_cell_info_arr->eutra_cell_info))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(NR_NEIGHBOUR_INFO_PRESENT & p_x2ap_served_eutra_nr_cell_info_arr->bitmask)
    {
        /* Compose IE */
        if (RRC_FAILURE == rrc_il_compose_x2ap_nr_nbour_cell_info(pp_buffer, 
            &p_x2ap_served_eutra_nr_cell_info_arr->nr_neighbour_cell_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t 
rrc_il_get_x2ap_served_eutra_nr_cells_to_modify_info_len
(
    x2ap_served_eutra_nr_cells_to_modify_info_t *p_x2ap_served_eutra_nr_cell_to_mod_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_served_eutra_nr_cell_to_mod_info != PNULL);

    length += (rrc_length_t)sizeof(p_x2ap_served_eutra_nr_cell_to_mod_info->old_cell_id);

    /* Get length of IE */
    length += rrc_il_get_x2ap_served_eutra_nr_cell_info_arr_len(
        &p_x2ap_served_eutra_nr_cell_to_mod_info->served_eutra_nr_cell_info);    

    return length;
}

rrc_return_et 
rrc_il_compose_x2ap_served_eutra_nr_cells_to_modify_info
(
    U8  **pp_buffer,
    x2ap_served_eutra_nr_cells_to_modify_info_t *p_x2ap_served_eutra_nr_cell_to_mod_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_served_eutra_nr_cell_to_mod_info != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_eutra_nr_cell_to_mod_info->old_cell_id, "old_cell_id");
    *pp_buffer += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if (MAX_SERVED_CELLS <= p_x2ap_served_eutra_nr_cell_to_mod_info->old_cell_id)
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_mod_info->old_cell_id] should be in range "
            "0 to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_served_eutra_nr_cell_to_mod_info->old_cell_id);
        return RRC_FAILURE;
    }
    
    if (RRC_FAILURE == rrc_il_compose_x2ap_served_eutra_nr_cell_info_arr(pp_buffer, 
        &p_x2ap_served_eutra_nr_cell_to_mod_info->served_eutra_nr_cell_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

rrc_length_t 
rrc_il_get_x2ap_rrm_cell_assistance_info_len
(
    x2ap_rrm_cell_assistance_info_t *p_x2ap_rrm_cell_assis_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_rrm_cell_assis_info != PNULL);

    /* Get length of IE */
    length += sizeof(p_x2ap_rrm_cell_assis_info->bitmask);

    if (X2AP_CELL_ASSISTANCE_LIMITED_LIST_PRESENT & p_x2ap_rrm_cell_assis_info->bitmask)
    {
        /* Get length of IE */
        length += rrc_il_get_x2ap_partial_list_len(
            &p_x2ap_rrm_cell_assis_info->partial_nr_cells);    
    }    

    if (X2AP_CELL_ASSISTANCE_FULL_LIST_PRESENT & p_x2ap_rrm_cell_assis_info->bitmask)
    {
        /* Get length of IE */
        length += sizeof(p_x2ap_rrm_cell_assis_info->full_nr_cells_ind);    
    }    

    return length;
}

rrc_return_et 
rrc_il_compose_x2ap_rrm_cell_assistance_info
(
    U8  **pp_buffer,
    x2ap_rrm_cell_assistance_info_t *p_x2ap_rrm_cell_assis_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_rrm_cell_assis_info != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_cell_assis_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_rrm_cell_assis_info->bitmask);

    /* Optional element */
    if(X2AP_CELL_ASSISTANCE_LIMITED_LIST_PRESENT & p_x2ap_rrm_cell_assis_info->bitmask)
    {
        /* Compose IE */
        if (RRC_FAILURE == rrc_il_compose_x2ap_partial_list(pp_buffer, 
            &p_x2ap_rrm_cell_assis_info->partial_nr_cells))
        {
            return RRC_FAILURE;
        }
    }

    /* Optional element */
    if(X2AP_CELL_ASSISTANCE_FULL_LIST_PRESENT & p_x2ap_rrm_cell_assis_info->bitmask)
    {
        rrc_cp_pack_U16(*pp_buffer, &p_x2ap_rrm_cell_assis_info->full_nr_cells_ind, "full_nr_cells_ind");
        *pp_buffer += sizeof(U16);
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_served_eutra_nr_cells_to_add_list_len
(
    x2ap_served_eutra_nr_cells_to_add_list_t *p_x2ap_served_eutra_nr_cell_to_add_list
)
{
    rrc_length_t length = 0;
    rrc_counter_t loop = 0;

    RRC_ASSERT(p_x2ap_served_eutra_nr_cell_to_add_list != PNULL);

    length += (rrc_length_t)sizeof(p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell) ||
        (MAX_SERVED_CELLS < p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell] should be in range "
            "1 to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell ; loop++)
    {
        /* Get length of IE */
        length += rrc_il_get_x2ap_served_eutra_nr_cell_info_arr_len(&p_x2ap_served_eutra_nr_cell_to_add_list->eutra_nr_cell_list[loop]);
    }

    return length;
}

rrc_return_et 
rrc_il_compose_x2ap_served_eutra_nr_cells_to_add_list
(
    U8  **pp_buffer,
    x2ap_served_eutra_nr_cells_to_add_list_t *p_x2ap_served_eutra_nr_cell_to_add_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_served_eutra_nr_cell_to_add_list != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell, "num_served_eutra_nr_cell");
    *pp_buffer += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((1 > p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell ||
        MAX_SERVED_CELLS < p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell);
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_served_eutra_nr_cell_info_arr(pp_buffer, 
                &p_x2ap_served_eutra_nr_cell_to_add_list->eutra_nr_cell_list[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_served_eutra_nr_cells_to_modify_list_len
(
    x2ap_served_eutra_nr_cells_to_modify_list_t *p_x2ap_served_eutra_nr_cell_to_mod_list
)
{
    rrc_length_t length = 0;
    rrc_counter_t loop = 0;

    RRC_ASSERT(p_x2ap_served_eutra_nr_cell_to_mod_list != PNULL);

    length += (rrc_length_t)sizeof(p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell) ||
        (MAX_SERVED_CELLS < p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell] should be in range "
            "1 to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell ; loop++)
    {
        /* Get length of IE */
        length += rrc_il_get_x2ap_served_eutra_nr_cells_to_modify_info_len(&p_x2ap_served_eutra_nr_cell_to_mod_list->eutra_nr_cell_list[loop]);
    }

    return length;
}

rrc_return_et 
rrc_il_compose_x2ap_served_eutra_nr_cells_to_modify_list
(
    U8  **pp_buffer,
    x2ap_served_eutra_nr_cells_to_modify_list_t *p_x2ap_served_eutra_nr_cell_to_mod_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_served_eutra_nr_cell_to_mod_list != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell, "num_served_eutra_nr_cell");
    *pp_buffer += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((1 > p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell ||
        MAX_SERVED_CELLS < p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell);
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_served_eutra_nr_cells_to_modify_info(pp_buffer, 
                &p_x2ap_served_eutra_nr_cell_to_mod_list->eutra_nr_cell_list[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_served_eutra_nr_cells_to_delete_list_len
(
    x2ap_served_eutra_nr_cells_to_delete_list_t *p_x2ap_served_eutra_nr_cell_to_del_list
)
{
    rrc_length_t length = 0;
    rrc_counter_t loop = 0;

    RRC_ASSERT(p_x2ap_served_eutra_nr_cell_to_del_list != PNULL);

    length += (rrc_length_t)sizeof(p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell) ||
        (MAX_SERVED_CELLS < p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell] should be in range "
            "1 to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell ; loop++)
    {
        length += (rrc_length_t)sizeof(p_x2ap_served_eutra_nr_cell_to_del_list->cell_id_list[loop]);
    }

    return length;
}

rrc_return_et 
rrc_il_compose_x2ap_served_eutra_nr_cells_to_delete_list
(
    U8  **pp_buffer,
    x2ap_served_eutra_nr_cells_to_delete_list_t *p_x2ap_served_eutra_nr_cell_to_del_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_served_eutra_nr_cell_to_del_list != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell, "num_served_cell");
    *pp_buffer += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((1 > p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell ||
        MAX_SERVED_CELLS < p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell);
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell; loop++)
        {
            /* Check for correct range [H - higher boundary] */
            if (MAX_SERVED_CELLS <= p_x2ap_served_eutra_nr_cell_to_del_list->cell_id_list[loop])
            {
                RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_del_list->cell_id_list[]] should be in range [0 <= %d < MAX_SERVED_CELLS] !", 
                    p_x2ap_served_eutra_nr_cell_to_del_list->cell_id_list[loop]);
                return RRC_FAILURE;
            }
    
            /* Compose parameter of basic type */
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_eutra_nr_cell_to_del_list->cell_id_list[loop], "cell_id_list[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_endc_config_update_req_len
(
    x2ap_endc_config_update_req_t *p_x2ap_endc_config_update_req
)
{
    rrc_length_t length = 0;
    rrc_counter_t loop = 0;

    RRC_ASSERT(p_x2ap_endc_config_update_req != PNULL);

    length += (rrc_length_t)sizeof(p_x2ap_endc_config_update_req->bitmask);

    length += (rrc_length_t)sizeof(p_x2ap_endc_config_update_req->num_peer_gnb);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((1 > p_x2ap_endc_config_update_req->num_peer_gnb) ||
        (MAX_PEER_GNB < p_x2ap_endc_config_update_req->num_peer_gnb))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_endc_config_update_req->num_peer_gnb] should be in range "
            "1 to MAX_PEER_GNB. Incorrect value %d received.", p_x2ap_endc_config_update_req->num_peer_gnb);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_endc_config_update_req->num_peer_gnb ; loop++)
    {
        /* Get length of IE */
        length += rrc_il_get_x2_gb_gnb_id_len(&p_x2ap_endc_config_update_req->gnb_info[loop]);
    }

    if (X2AP_ENDC_CONFIGUPDATE_ENB_CELLASSISTANCE_INFO_PRESENT & 
        p_x2ap_endc_config_update_req->bitmask)
    {    
        /* Get length of IE */
        length += rrc_il_get_x2ap_rrm_cell_assistance_info_len(&p_x2ap_endc_config_update_req->cell_assistance_info);
    }

    if (X2AP_SERVED_EUTRA_CELLS_TO_ADD_LIST_PRESENT & 
        p_x2ap_endc_config_update_req->bitmask)
    {    
        /* Get length of IE */
        length += rrc_il_get_x2ap_served_eutra_nr_cells_to_add_list_len(&p_x2ap_endc_config_update_req->add_served_cell);
    }

    if (X2AP_SERVED_EUTRA_CELLS_TO_MODIFY_LIST_PRESENT & 
        p_x2ap_endc_config_update_req->bitmask)
    {    
        /* Get length of IE */
        length += rrc_il_get_x2ap_served_eutra_nr_cells_to_modify_list_len(&p_x2ap_endc_config_update_req->mod_served_cell);
    }

    if (X2AP_SERVED_EUTRA_CELLS_TO_DELETE_LIST_PRESENT & 
        p_x2ap_endc_config_update_req->bitmask)
    {    
        /* Get length of IE */
        length += rrc_il_get_x2ap_served_eutra_nr_cells_to_delete_list_len(&p_x2ap_endc_config_update_req->del_served_cell);
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_endc_config_update_req
(
    U8  **pp_buffer,
    x2ap_endc_config_update_req_t *p_x2ap_endc_config_update_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_endc_config_update_req != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_endc_config_update_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_endc_config_update_req->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((1 > p_x2ap_endc_config_update_req->num_peer_gnb ||
        MAX_PEER_GNB < p_x2ap_endc_config_update_req->num_peer_gnb))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_endc_config_update_req->num_peer_gnb] should be in range [1 <= %d <= MAX_PEER_GNB] !", 
            p_x2ap_endc_config_update_req->num_peer_gnb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_endc_config_update_req->num_peer_gnb, "num_peer_gnb");
    *pp_buffer += sizeof(p_x2ap_endc_config_update_req->num_peer_gnb);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_endc_config_update_req->num_peer_gnb; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2_gb_gnb_id(pp_buffer, &p_x2ap_endc_config_update_req->gnb_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Optional element */
    if(X2AP_ENDC_CONFIGUPDATE_ENB_CELLASSISTANCE_INFO_PRESENT & p_x2ap_endc_config_update_req->bitmask)
    {
        /* Compose IE */
        if (RRC_FAILURE == rrc_il_compose_x2ap_rrm_cell_assistance_info(pp_buffer, 
            &p_x2ap_endc_config_update_req->cell_assistance_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Optional element */
    if(X2AP_SERVED_EUTRA_CELLS_TO_ADD_LIST_PRESENT & p_x2ap_endc_config_update_req->bitmask)
    {
        /* Compose IE */
        if (RRC_FAILURE == rrc_il_compose_x2ap_served_eutra_nr_cells_to_add_list(pp_buffer, 
            &p_x2ap_endc_config_update_req->add_served_cell))
        {
            return RRC_FAILURE;
        }
    }

    /* Optional element */
    if(X2AP_SERVED_EUTRA_CELLS_TO_MODIFY_LIST_PRESENT & p_x2ap_endc_config_update_req->bitmask)
    {
        /* Compose IE */
        if (RRC_FAILURE == rrc_il_compose_x2ap_served_eutra_nr_cells_to_modify_list(pp_buffer, 
            &p_x2ap_endc_config_update_req->mod_served_cell))
        {
            return RRC_FAILURE;
        }
    }

    /* Optional element */
    if(X2AP_SERVED_EUTRA_CELLS_TO_DELETE_LIST_PRESENT & p_x2ap_endc_config_update_req->bitmask)
    {

        /* Compose IE */
        if (RRC_FAILURE == rrc_il_compose_x2ap_served_eutra_nr_cells_to_delete_list(pp_buffer, 
            &p_x2ap_endc_config_update_req->del_served_cell))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_endc_config_update_resp_len
(
    x2ap_endc_config_update_resp_t *p_x2ap_endc_config_update_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_endc_config_update_resp != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_endc_config_update_resp->bitmask);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_endc_config_update_resp->enb_gb_id);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_gnb_id_len(&p_x2ap_endc_config_update_resp->gnb_gb_id);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_endc_config_update_resp->response);

    /* Optional element */
    if(X2AP_CAUSE_PRESENT & p_x2ap_endc_config_update_resp->bitmask)
    {
        /* Get length of IE */
        length += rrc_il_get_x2ap_cause_len(&p_x2ap_endc_config_update_resp->cause);
    }

    /* Optional element */
    if(X2AP_NR_CELL_INFOMATION_PRESENT & p_x2ap_endc_config_update_resp->bitmask)
    {
        length += rrc_il_get_x2ap_nr_srvd_cell_to_management_list_len(
            &p_x2ap_endc_config_update_resp->nr_cell_information);
    }
    
    return length;
}

rrc_return_et
rrc_il_compose_x2ap_endc_config_update_resp
(
    U8  **pp_buffer,
    x2ap_endc_config_update_resp_t *p_x2ap_endc_config_update_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_endc_config_update_resp != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_endc_config_update_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_endc_config_update_resp->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_endc_config_update_resp->enb_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_gnb_id(pp_buffer, &p_x2ap_endc_config_update_resp->gnb_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_endc_config_update_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_endc_config_update_resp->response] should be in range [%d <= 1] !", 
            p_x2ap_endc_config_update_resp->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_endc_config_update_resp->response, "response");
    *pp_buffer += sizeof(p_x2ap_endc_config_update_resp->response);

    /* Optional element */
    if(X2AP_CAUSE_PRESENT & p_x2ap_endc_config_update_resp->bitmask)
    {
        /* Compose IE */
        if (RRC_FAILURE == rrc_il_compose_x2ap_cause(pp_buffer, &p_x2ap_endc_config_update_resp->cause))
        {
            return RRC_FAILURE;
        }
    }

     /* Optional element */
    if(X2AP_NR_CELL_INFOMATION_PRESENT & p_x2ap_endc_config_update_resp->bitmask)
    {
        if (RRC_FAILURE == rrc_il_compose_x2ap_nr_srvd_cell_to_management_list(pp_buffer, 
            &p_x2ap_endc_config_update_resp->nr_cell_information))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apRrm_il_send_x2ap_endc_config_update_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_ENDC_CONFIG_UPDATE_RES message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_endc_config_update_resp
(
    x2ap_endc_config_update_resp_t  *p_x2ap_endc_config_update_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_endc_config_update_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_endc_config_update_resp_len(p_x2ap_endc_config_update_resp);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_ENDC_CONFIG_UPDATE_RES", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_ENDC_CONFIG_UPDATE_RES, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_ENDC_CONFIG_UPDATE_RES, msg_length, cell_index);

    /* Fill X2AP_ENB_CONFIG_UPDATE_RES message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_endc_config_update_resp(&p_x2apCommon_msg, p_x2ap_endc_config_update_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_endc_config_update_wait_ind_len
(
    x2ap_endc_config_update_wait_ind_t *p_x2ap_endc_config_update_wait_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_endc_config_update_wait_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_endc_config_update_wait_ind->bitmask);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_endc_config_update_wait_ind->enb_gb_id);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_gnb_id_len(&p_x2ap_endc_config_update_wait_ind->gnb_gb_id);
    
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_endc_config_update_wait_ind->time_to_wait);

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_endc_config_update_wait_ind
(
    U8  **pp_buffer,
    x2ap_endc_config_update_wait_ind_t *p_x2ap_endc_config_update_wait_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_endc_config_update_wait_ind != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_endc_config_update_wait_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_endc_config_update_wait_ind->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_endc_config_update_wait_ind->enb_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_gb_gnb_id(pp_buffer, &p_x2ap_endc_config_update_wait_ind->gnb_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_endc_config_update_wait_ind->time_to_wait > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_endc_config_update_wait_ind->time_to_wait] should be in range [%d <= 5] !", p_x2ap_endc_config_update_wait_ind->time_to_wait);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_endc_config_update_wait_ind->time_to_wait, "time_to_wait");
    *pp_buffer += sizeof(p_x2ap_endc_config_update_wait_ind->time_to_wait);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_endc_config_update_wait_ind
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_ENDC_CONFIG_UPDATE_WAIT_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apRrm_il_send_x2ap_endc_config_update_wait_ind
(
    x2ap_endc_config_update_wait_ind_t  *p_x2ap_endc_config_update_wait_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_endc_config_update_wait_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_endc_config_update_wait_ind_len(p_x2ap_endc_config_update_wait_ind);
    
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_ENDC_CONFIG_UPDATE_WAIT_IND", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_ENDC_CONFIG_UPDATE_WAIT_IND, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_ENDC_CONFIG_UPDATE_WAIT_IND, msg_length, cell_index);

    /* Fill X2AP_ENDC_CONFIG_UPDATE_WAIT_IND message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_endc_config_update_wait_ind(&p_x2apCommon_msg, p_x2ap_endc_config_update_wait_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}
#endif

