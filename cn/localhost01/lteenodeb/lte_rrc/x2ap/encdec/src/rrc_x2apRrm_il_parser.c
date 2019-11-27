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
 *  File Description : The file rrc_x2apRrm_il_parser.c contains the definitions 
 *                     of rrc-x2apRrm interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "rrc_cp_common.h"
#include "rrc_x2apRrm_il_parser.h"
#include "rrc_x2apRrm_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_return_et
rrc_il_parse_x2ap_plmn_identity
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_ecgi
(
    x2ap_ecgi_t *p_x2ap_ecgi,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_subframe_allocation
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_mbsfn_info
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_mbsfn_subframe_info
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_mbms_cell_info
(
    mbms_cell_info_t *p_mbms_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_macro_enb_id
(
    macro_enb_id_t *p_macro_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_home_enb_id
(
    home_enb_id_t *p_home_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2_enb_id_info
(
    x2_enb_id_info_t *p_x2_enb_id_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2_gb_enb_id
(
    x2_gb_enb_id_t *p_x2_gb_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2_gnb_id
(
    x2_gnb_id_t *p_x2_gnb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2_gb_gnb_id
(
    x2_gb_gnb_id_t *p_x2_gb_gnb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_tac
(
    x2ap_tac_t *p_x2ap_tac,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_broadcast_plmn
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_earfcn
(
    x2ap_earfcn_t *p_x2ap_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_fdd_info
(
    x2ap_fdd_info_t *p_x2ap_fdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_special_subframe_info
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_tdd_info
(
    x2ap_tdd_info_t *p_x2ap_tdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_choice_eutra_mode
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_choice_eutra_mode_info
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_prach_configuration_info
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_srvd_cell_info
(
    x2ap_srvd_cell_info_t *p_x2ap_srvd_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_neighbour_info
(
    x2ap_neighbour_info_t *p_x2ap_neighbour_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_nbour_cell_arry
(
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_srvd_cell_arry
(
    x2ap_srvd_cell_arry_t *p_x2ap_srvd_cell_arry,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_cell_info
(
    x2ap_rrm_cell_info_t *p_x2ap_rrm_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_gu_group_id
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_eutra_cell_info
(
    x2ap_rrm_eutra_cell_info_t *p_x2ap_rrm_eutra_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_nr_ecgi
(
    x2ap_nr_ecgi_t *p_x2ap_nr_ecgi,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_nr_freq_band_info
(
    x2ap_nr_freq_band_info_t *p_x2ap_nr_freq_band_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_nr_tx_bandwidth
(
    x2ap_nr_tx_bandwidth_t *p_x2ap_nr_tx_bandwidth,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_sul_information
(
    x2ap_sul_information_t *p_x2ap_sul_information,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_nr_freq_info
(
    x2ap_nr_freq_info_t *p_x2ap_nr_freq_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_nr_fdd_into
(
    x2ap_nr_fdd_into_t *p_x2ap_nr_fdd_into,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_nr_tdd_into
(
    x2ap_nr_tdd_into_t *p_x2ap_nr_tdd_into,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_choice_nr_mode_info
(
    x2ap_choice_nr_mode_info_t *p_x2ap_choice_nr_mode_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_periodicity_and_offset
(
    x2ap_periodicity_and_offset_t *p_x2ap_periodicity_and_offset,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_ssb_mtc
(
    x2ap_ssb_mtc_t *p_x2ap_ssb_mtc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_measurement_slots
(
    x2ap_measurement_slots_t *p_x2ap_measurement_slots,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_ss_rssi_measurement
(
    x2ap_ss_rssi_measurement_t *p_x2ap_ss_rssi_measurement,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_meas_timing_config
(
    x2ap_meas_timing_config_t *p_x2ap_meas_timing_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_five_gs_tac
(
    x2ap_five_gs_tac_t *p_x2ap_five_gs_tac,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_srvd_nr_cell_info
(
    x2ap_rrm_srvd_nr_cell_info_t *p_x2ap_rrm_srvd_nr_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_fdd_info_neigh_served_nr_cell_info
(
    x2ap_fdd_info_neigh_served_nr_cell_info_t *p_x2ap_fdd_info_neigh_served_nr_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_tdd_info_neigh_served_nr_cell_info
(
    x2ap_tdd_info_neigh_served_nr_cell_info_t *p_x2ap_tdd_info_neigh_served_nr_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_nr_neigh_mode_info
(
    x2ap_nr_neigh_mode_info_t *p_x2ap_nr_neigh_mode_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_nr_neigh_info
(
    x2ap_nr_neigh_info_t *p_x2ap_nr_neigh_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_nr_neigh_cell_arry
(
    x2ap_nr_neigh_cell_arry_t *p_x2ap_nr_neigh_cell_arry,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_nr_cell_info
(
    x2ap_rrm_nr_cell_info_t *p_x2ap_rrm_nr_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_mnc
(
    mnc_t *p_mnc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_plmn_identity
(
    plmn_identity_t *p_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrm_cell_global_id_eutra
(
    rrm_cell_global_id_eutra_t *p_rrm_cell_global_id_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_cause
(
    x2ap_rrm_cause_t *p_x2ap_rrm_cause,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_measurement_failure_cause_item
(
    measurement_failure_cause_item_t *p_measurement_failure_cause_item,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rsu_measurement_res_item
(
    rsu_measurement_res_item_t *p_rsu_measurement_res_item,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_measurement_res
(
    x2ap_rrm_rsu_measurement_res_t *p_x2ap_rrm_rsu_measurement_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_prb
(
    x2ap_rrm_rsu_prb_t *p_x2ap_rrm_rsu_prb,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_tnl_load
(
    x2ap_rrm_rsu_tnl_load_t *p_x2ap_rrm_rsu_tnl_load,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_hw_load
(
    x2ap_rrm_rsu_hw_load_t *p_x2ap_rrm_rsu_hw_load,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_composite_available_capacity
(
    x2ap_rrm_rsu_composite_available_capacity_t *p_x2ap_rrm_rsu_composite_available_capacity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_cacg
(
    x2ap_rrm_rsu_cacg_t *p_x2ap_rrm_rsu_cacg,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_usable_abs_information_fdd
(
    x2ap_usable_abs_information_fdd_t *p_x2ap_usable_abs_information_fdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_usable_abs_information_tdd
(
    x2ap_usable_abs_information_tdd_t *p_x2ap_usable_abs_information_tdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_usable_abs_information
(
    x2ap_usable_abs_information_t *p_x2ap_usable_abs_information,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_abs_status
(
    x2ap_rrm_abs_status_t *p_x2ap_rrm_abs_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_info
(
    x2ap_rrm_rsu_info_t *p_x2ap_rrm_rsu_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_report_info
(
    x2ap_rrm_rsu_report_info_t *p_x2ap_rrm_rsu_report_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_ul_intf_overload_ind
(
    x2ap_rrm_li_ul_intf_overload_ind_t *p_x2ap_rrm_li_ul_intf_overload_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_ul_high_intf_ind
(
    x2ap_rrm_li_ul_high_intf_ind_t *p_x2ap_rrm_li_ul_high_intf_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_ul_high_intf_info_arr
(
    x2ap_rrm_li_ul_high_intf_info_arr_t *p_x2ap_rrm_li_ul_high_intf_info_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_ul_high_interference_info
(
    x2ap_rrm_li_ul_high_interference_info_t *p_x2ap_rrm_li_ul_high_interference_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_relative_narrowband_tx_power
(
    x2ap_rrm_li_relative_narrowband_tx_power_t *p_x2ap_rrm_li_relative_narrowband_tx_power,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_abs_information_fdd
(
    x2ap_abs_information_fdd_t *p_x2ap_abs_information_fdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_abs_information_tdd
(
    x2ap_abs_information_tdd_t *p_x2ap_abs_information_tdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_abs_information
(
    x2ap_rrm_li_abs_information_t *p_x2ap_rrm_li_abs_information,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_invoke_indication
(
    x2ap_rrm_li_invoke_indication_t *p_x2ap_rrm_li_invoke_indication,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_cell_info_item_arr
(
    x2ap_rrm_li_cell_info_item_arr_t *p_x2ap_rrm_li_cell_info_item_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlf_report_meas_result_serv_cell
(
    rrc_rlf_report_meas_result_serv_cell_t *p_rrc_rlf_report_meas_result_serv_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_cell_global_id_eutra
(
    cell_global_id_eutra_t *p_cell_global_id_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_plmn_identity_list2
(
    plmn_identity_list2_t *p_plmn_identity_list2,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_eutra_cgi_info
(
    meas_result_eutra_cgi_info_t *p_meas_result_eutra_cgi_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_eutra_meas_result
(
    meas_result_eutra_meas_result_t *p_meas_result_eutra_meas_result,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_csg_info
(
    meas_result_csg_info_t *p_meas_result_csg_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_eutra
(
    meas_result_eutra_t *p_meas_result_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_list_eutra
(
    meas_result_list_eutra_t *p_meas_result_list_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_meas_result2_eutra
(
    rrc_meas_result2_eutra_t *p_rrc_meas_result2_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_meas_result_list2_eutra
(
    rrc_meas_result_list2_eutra_t *p_rrc_meas_result_list2_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_utra_phys_cell_id
(
    meas_result_utra_phys_cell_id_t *p_meas_result_utra_phys_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_cell_global_id_utra
(
    cell_global_id_utra_t *p_cell_global_id_utra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_utra_cgi_info
(
    meas_result_utra_cgi_info_t *p_meas_result_utra_cgi_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_utra_meas_result
(
    meas_result_utra_meas_result_t *p_meas_result_utra_meas_result,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_utra
(
    meas_result_utra_t *p_meas_result_utra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_list_utra
(
    meas_result_list_utra_t *p_meas_result_list_utra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_meas_result2_utra
(
    rrc_meas_result2_utra_t *p_rrc_meas_result2_utra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_meas_result_list2_utra
(
    rrc_meas_result_list2_utra_t *p_rrc_meas_result_list2_utra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_carrier_freq_geran
(
    carrier_freq_geran_t *p_carrier_freq_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_phys_cell_id_geran
(
    phys_cell_id_geran_t *p_phys_cell_id_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_cell_global_id_geran
(
    cell_global_id_geran_t *p_cell_global_id_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_geran_cgi_info
(
    meas_result_geran_cgi_info_t *p_meas_result_geran_cgi_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_geran_meas_result
(
    meas_result_geran_meas_result_t *p_meas_result_geran_meas_result,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_geran
(
    meas_result_geran_t *p_meas_result_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_list_geran
(
    meas_result_list_geran_t *p_meas_result_list_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_carrier_freq_cdma2000
(
    carrier_freq_cdma2000_t *p_carrier_freq_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_cell_global_id_cdma2000
(
    cell_global_id_cdma2000_t *p_cell_global_id_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_cdma2000_meas_result
(
    meas_result_cdma2000_meas_result_t *p_meas_result_cdma2000_meas_result,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_cdma2000
(
    meas_result_cdma2000_t *p_meas_result_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_list_cdma2000
(
    meas_result_list_cdma2000_t *p_meas_result_list_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_results_cdma2000
(
    meas_results_cdma2000_t *p_meas_results_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_meas_result2_cdma2000
(
    rrc_meas_result2_cdma2000_t *p_rrc_meas_result2_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_meas_result_list2_cdma2000
(
    meas_result_list2_cdma2000_t *p_meas_result_list2_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlf_report_meas_result_neigh_cell
(
    rrc_rlf_report_meas_result_neigh_cell_t *p_rrc_rlf_report_meas_result_neigh_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlf_dynamic_string
(
    rrc_rlf_dynamic_string_t *p_rrc_rlf_dynamic_string,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlf_report_location_coordinates
(
    rrc_rlf_report_location_coordinates_t *p_rrc_rlf_report_location_coordinates,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlf_report_location_info
(
    rrc_rlf_report_location_info_t *p_rrc_rlf_report_location_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlf_failed_pci_arfcn
(
    rrc_rlf_failed_pci_arfcn_t *p_rrc_rlf_failed_pci_arfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlf_failed_pcellid
(
    rrc_rlf_failed_pcellid_t *p_rrc_rlf_failed_pcellid,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlf_report
(
    rrc_rlf_report_t *p_rrc_rlf_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#ifdef ENDC_ENABLED

static rrc_return_et 
rrc_il_parse_x2ap_partial_list
(
    x2ap_partial_list_t *p_x2ap_partial_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et 
rrc_il_parse_x2ap_nr_srvd_cell_arr
(
    x2ap_nr_srvd_cell_arr_t *p_x2ap_nr_srvd_cell_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et 
rrc_il_parse_x2ap_nr_srvd_cell_modify_arr
(
    x2ap_nr_srvd_cell_modify_arr_t *p_x2ap_nr_srvd_cell_mod_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_nr_srvd_cell_to_management_list
(
    x2ap_nr_srvd_cell_to_management_list_t *p_x2ap_nr_srvd_cell_to_add_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_nr_srvd_cell_to_modify_list
(
    x2ap_nr_srvd_cell_to_modify_list_t *p_x2ap_nr_srvd_cell_to_mod_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_nr_srvd_cell_to_delete_list
(
    x2ap_nr_srvd_cell_to_delete_list_t *p_x2ap_nr_srvd_cell_to_del_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static rrc_return_et 
rrc_il_parse_x2ap_served_eutra_nr_cell_info_arr
(
    x2ap_served_eutra_nr_cell_info_arr_t *p_x2ap_served_eutra_nr_cell_info_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static rrc_return_et 
rrc_il_parse_x2ap_served_eutra_nr_cells_to_modify_info
(
    x2ap_served_eutra_nr_cells_to_modify_info_t *p_x2ap_served_eutra_nr_cell_to_mod_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static rrc_return_et 
rrc_il_parse_x2ap_rrm_cell_assistance_info
(
    x2ap_rrm_cell_assistance_info_t *p_x2ap_rrm_cell_assistance_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static rrc_return_et 
rrc_il_parse_x2ap_served_eutra_nr_cells_to_add_list
(
    x2ap_served_eutra_nr_cells_to_add_list_t *p_x2ap_served_eutra_nr_cell_to_add_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static rrc_return_et 
rrc_il_parse_x2ap_served_eutra_nr_cells_to_modify_list
(
    x2ap_served_eutra_nr_cells_to_modify_list_t *p_x2ap_served_eutra_nr_cell_to_mod_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static rrc_return_et 
rrc_il_parse_x2ap_served_eutra_nr_cells_to_delete_list
(
    x2ap_served_eutra_nr_cells_to_delete_list_t *p_x2ap_served_eutra_nr_cell_to_del_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif

/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_return_et
rrc_il_parse_rrm_updated_mbms_sf_info
(
    rrm_updated_mbms_sf_info_t *p_rrm_updated_mbms_sf_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrm_updated_mbms_sf_info, 0, sizeof(rrm_updated_mbms_sf_info_t));

    /* This function parses rrm_updated_mbms_sf_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrm_updated_mbms_sf_info->num_cells, p_src + *p_length_read, "num_cells");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_updated_mbms_sf_info->num_cells < 1) || (p_rrm_updated_mbms_sf_info->num_cells > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrm_updated_mbms_sf_info->num_cells] should be in range "
            "1 to 8. Incorrect value %u received.", p_rrm_updated_mbms_sf_info->num_cells);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrm_updated_mbms_sf_info->num_cells; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_mbms_cell_info(
                &p_rrm_updated_mbms_sf_info->cell_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_plmn_identity
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_plmn_identity, 0, sizeof(x2ap_plmn_identity_t));

    /* This function parses x2ap_plmn_identity */

    if (*p_length_read + (S32)sizeof(p_x2ap_plmn_identity->plmn_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_plmn_identity->plmn_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_plmn_identity->plmn_id[loop], (void*)(p_src + *p_length_read), "plmn_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_ecgi
(
    x2ap_ecgi_t *p_x2ap_ecgi,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_ecgi, 0, sizeof(x2ap_ecgi_t));

    /* This function parses x2ap_ecgi */

    if (RRC_FAILURE == rrc_il_parse_x2ap_plmn_identity(
        &p_x2ap_ecgi->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_x2ap_ecgi->eutran_cell_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_ecgi->eutran_cell_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_ecgi->eutran_cell_id[loop], (void*)(p_src + *p_length_read), "eutran_cell_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_subframe_allocation
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_subframe_allocation, 0, sizeof(x2ap_subframe_allocation_t));

    /* This function parses x2ap_subframe_allocation */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_subframe_allocation->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_x2ap_subframe_allocation->oneframe) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->oneframe); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_subframe_allocation->oneframe[loop], (void*)(p_src + *p_length_read), "oneframe[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_x2ap_subframe_allocation->fourframe) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->fourframe); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_subframe_allocation->fourframe[loop], (void*)(p_src + *p_length_read), "fourframe[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_mbsfn_info
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_mbsfn_info, 0, sizeof(x2ap_mbsfn_info_t));

    /* This function parses x2ap_mbsfn_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_mbsfn_info->radioframe_alloc_period, p_src + *p_length_read, "radioframe_alloc_period");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_period > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_period] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_mbsfn_info->radioframe_alloc_period);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_mbsfn_info->radioframe_alloc_offset, p_src + *p_length_read, "radioframe_alloc_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_offset > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_offset] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_mbsfn_info->radioframe_alloc_offset);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_subframe_allocation(
        &p_x2ap_mbsfn_info->subframe_alloc,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_mbsfn_subframe_info
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_mbsfn_subframe_info, 0, sizeof(x2ap_mbsfn_subframe_info_t));

    /* This function parses x2ap_mbsfn_subframe_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_mbsfn_subframe_info->num_mbsfn, p_src + *p_length_read, "num_mbsfn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_subframe_info->num_mbsfn > MAX_NUM_MBSFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be less than"
            " or equal to MAX_NUM_MBSFN. Incorrect value %u received.", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_mbsfn_info(
                &p_x2ap_mbsfn_subframe_info->mbsfn_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_mbms_cell_info
(
    mbms_cell_info_t *p_mbms_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_mbms_cell_info, 0, sizeof(mbms_cell_info_t));

    /* This function parses mbms_cell_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_mbms_cell_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_x2ap_ecgi(
        &p_mbms_cell_info->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_mbms_cell_info->bitmask & RRM_X2AP_MBMS_CONFIG_APPLY_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_mbsfn_subframe_info(
        &p_mbms_cell_info->mbsfn_subframe_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_x2_link_down_ind
(
    x2ap_rrm_x2_link_down_ind_t *p_x2ap_rrm_x2_link_down_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_x2_link_down_ind, 0, sizeof(x2ap_rrm_x2_link_down_ind_t));

    /* This function parses x2ap_rrm_x2_link_down_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_x2_link_down_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2ap_rrm_x2_link_down_ind->bitmask & X2AP_RRM_LINK_DOWN_GB_ENB_ID_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rrm_x2_link_down_ind->peer_gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_x2_link_down_ind->bitmask & X2AP_RRM_LINK_DOWN_GB_GNB_ID_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2_gb_gnb_id(
        &p_x2ap_rrm_x2_link_down_ind->peer_gb_gnb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_macro_enb_id
(
    macro_enb_id_t *p_macro_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_macro_enb_id, 0, sizeof(macro_enb_id_t));

    /* This function parses macro_enb_id */

    if (*p_length_read + (S32)sizeof(p_macro_enb_id->eNB_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_macro_enb_id->eNB_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_macro_enb_id->eNB_id[loop], (void*)(p_src + *p_length_read), "eNB_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_home_enb_id
(
    home_enb_id_t *p_home_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_home_enb_id, 0, sizeof(home_enb_id_t));

    /* This function parses home_enb_id */

    if (*p_length_read + (S32)sizeof(p_home_enb_id->eNB_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_home_enb_id->eNB_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_home_enb_id->eNB_id[loop], (void*)(p_src + *p_length_read), "eNB_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2_enb_id_info
(
    x2_enb_id_info_t *p_x2_enb_id_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2_enb_id_info, 0, sizeof(x2_enb_id_info_t));

    /* This function parses x2_enb_id_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_enb_id_info->presence_bitmask, p_src + *p_length_read, "presence_bitmask");
    *p_length_read += sizeof(U8);

    if (p_x2_enb_id_info->presence_bitmask & 1)
    {

    if (RRC_FAILURE == rrc_il_parse_macro_enb_id(
        &p_x2_enb_id_info->macro_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2_enb_id_info->presence_bitmask & 2)
    {

    if (RRC_FAILURE == rrc_il_parse_home_enb_id(
        &p_x2_enb_id_info->home_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2_gb_enb_id
(
    x2_gb_enb_id_t *p_x2_gb_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2_gb_enb_id, 0, sizeof(x2_gb_enb_id_t));

    /* This function parses x2_gb_enb_id */

    if (RRC_FAILURE == rrc_il_parse_x2ap_plmn_identity(
        &p_x2_gb_enb_id->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2_enb_id_info(
        &p_x2_gb_enb_id->enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2_gnb_id
(
    x2_gnb_id_t *p_x2_gnb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2_gnb_id, 0, sizeof(x2_gnb_id_t));

    /* This function parses x2_gnb_id */

    if (*p_length_read + (S32)sizeof(p_x2_gnb_id->x2_gnb_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_gnb_id->x2_gnb_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2_gnb_id->x2_gnb_id[loop], (void*)(p_src + *p_length_read), "x2_gnb_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2_gb_gnb_id
(
    x2_gb_gnb_id_t *p_x2_gb_gnb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2_gb_gnb_id, 0, sizeof(x2_gb_gnb_id_t));

    /* This function parses x2_gb_gnb_id */

    if (RRC_FAILURE == rrc_il_parse_x2ap_plmn_identity(
        &p_x2_gb_gnb_id->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2_gnb_id(
        &p_x2_gb_gnb_id->x2_en_gnb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_x2_link_up_ind_st
(
    x2ap_rrm_x2_link_up_ind_st *p_x2ap_rrm_x2_link_up_ind_st,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_x2_link_up_ind_st, 0, sizeof(x2ap_rrm_x2_link_up_ind_st));

    /* This function parses x2ap_rrm_x2_link_up_ind_st */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_x2_link_up_ind_st->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2ap_rrm_x2_link_up_ind_st->bitmask & RRM_X2AP_EUTRA_CELL_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_eutra_cell_info(
        &p_x2ap_rrm_x2_link_up_ind_st->eutra_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_x2_link_up_ind_st->bitmask & RRM_X2AP_NR_CELL_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_nr_cell_info(
        &p_x2ap_rrm_x2_link_up_ind_st->nr_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_tac
(
    x2ap_tac_t *p_x2ap_tac,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_tac, 0, sizeof(x2ap_tac_t));

    /* This function parses x2ap_tac */

    if (*p_length_read + (S32)sizeof(p_x2ap_tac->tac) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_tac->tac); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_tac->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_broadcast_plmn
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_broadcast_plmn, 0, sizeof(x2ap_broadcast_plmn_t));

    /* This function parses x2ap_broadcast_plmn */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_broadcast_plmn->num_bcast_plmn_id, p_src + *p_length_read, "num_bcast_plmn_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_broadcast_plmn->num_bcast_plmn_id < 1) || (p_x2ap_broadcast_plmn->num_bcast_plmn_id > MAX_NUM_BPLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range "
            "1 to MAX_NUM_BPLMN. Incorrect value %u received.", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_plmn_identity(
                &p_x2ap_broadcast_plmn->plmn_identity[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_earfcn
(
    x2ap_earfcn_t *p_x2ap_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_earfcn, 0, sizeof(x2ap_earfcn_t));

    /* This function parses x2ap_earfcn */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_earfcn->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_earfcn->earfcn > MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_earfcn->earfcn] should be less than"
            " or equal to MAX_EARFCN. Incorrect value %u received.", p_x2ap_earfcn->earfcn);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_fdd_info
(
    x2ap_fdd_info_t *p_x2ap_fdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_fdd_info, 0, sizeof(x2ap_fdd_info_t));

    /* This function parses x2ap_fdd_info */

    if (RRC_FAILURE == rrc_il_parse_x2ap_earfcn(
        &p_x2ap_fdd_info->ul_earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_earfcn(
        &p_x2ap_fdd_info->dl_earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_fdd_info->ul_trnsmission_bdwidth, p_src + *p_length_read, "ul_trnsmission_bdwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->ul_trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_fdd_info->ul_trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_fdd_info->ul_trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_fdd_info->dl_trnsmission_bdwidth, p_src + *p_length_read, "dl_trnsmission_bdwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->dl_trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_fdd_info->dl_trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_fdd_info->dl_trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_special_subframe_info
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_special_subframe_info, 0, sizeof(x2ap_special_subframe_info_t));

    /* This function parses x2ap_special_subframe_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_special_subframe_info->special_subframe_patterns, p_src + *p_length_read, "special_subframe_patterns");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->special_subframe_patterns > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->special_subframe_patterns] should be less than"
            " or equal to 8. Incorrect value %u received.", p_x2ap_special_subframe_info->special_subframe_patterns);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_special_subframe_info->dl_cp, p_src + *p_length_read, "dl_cp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->dl_cp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->dl_cp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_special_subframe_info->dl_cp);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_special_subframe_info->ul_cp, p_src + *p_length_read, "ul_cp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->ul_cp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->ul_cp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_special_subframe_info->ul_cp);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_tdd_info
(
    x2ap_tdd_info_t *p_x2ap_tdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_tdd_info, 0, sizeof(x2ap_tdd_info_t));

    /* This function parses x2ap_tdd_info */

    if (RRC_FAILURE == rrc_il_parse_x2ap_earfcn(
        &p_x2ap_tdd_info->earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_tdd_info->trnsmission_bdwidth, p_src + *p_length_read, "trnsmission_bdwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_tdd_info->trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_tdd_info->trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_tdd_info->subframe_assn, p_src + *p_length_read, "subframe_assn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->subframe_assn > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_tdd_info->subframe_assn] should be less than"
            " or equal to 6. Incorrect value %u received.", p_x2ap_tdd_info->subframe_assn);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_special_subframe_info(
        &p_x2ap_tdd_info->special_subframe_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_choice_eutra_mode
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_choice_eutra_mode, 0, sizeof(x2ap_choice_eutra_mode_t));

    /* This function parses x2ap_choice_eutra_mode */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_choice_eutra_mode->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_FDD)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_fdd_info(
        &p_x2ap_choice_eutra_mode->fdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_TDD)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_tdd_info(
        &p_x2ap_choice_eutra_mode->tdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_choice_eutra_mode_info
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_choice_eutra_mode_info, 0, sizeof(x2ap_choice_eutra_mode_info_t));

    /* This function parses x2ap_choice_eutra_mode_info */

    if (RRC_FAILURE == rrc_il_parse_x2ap_choice_eutra_mode(
        &p_x2ap_choice_eutra_mode_info->choice_eutra_mode,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_prach_configuration_info
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_prach_configuration_info, 0, sizeof(x2ap_prach_configuration_info_t));

    /* This function parses x2ap_prach_configuration_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_prach_configuration_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_prach_configuration_info->root_sequence_index, p_src + *p_length_read, "root_sequence_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->root_sequence_index > 837))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->root_sequence_index] should be less than"
            " or equal to 837. Incorrect value %u received.", p_x2ap_prach_configuration_info->root_sequence_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_prach_configuration_info->zero_correlation_zone_config, p_src + *p_length_read, "zero_correlation_zone_config");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->zero_correlation_zone_config > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->zero_correlation_zone_config] should be less than"
            " or equal to 15. Incorrect value %u received.", p_x2ap_prach_configuration_info->zero_correlation_zone_config);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_prach_configuration_info->high_speed_flag, p_src + *p_length_read, "high_speed_flag");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->high_speed_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->high_speed_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_prach_configuration_info->high_speed_flag);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_prach_configuration_info->prach_frequency_offset, p_src + *p_length_read, "prach_frequency_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_frequency_offset > 94))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_frequency_offset] should be less than"
            " or equal to 94. Incorrect value %u received.", p_x2ap_prach_configuration_info->prach_frequency_offset);
        return RRC_FAILURE;
    }

    if (p_x2ap_prach_configuration_info->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_prach_configuration_info->prach_config_index, p_src + *p_length_read, "prach_config_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_config_index > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_config_index] should be less than"
            " or equal to 63. Incorrect value %u received.", p_x2ap_prach_configuration_info->prach_config_index);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_srvd_cell_info
(
    x2ap_srvd_cell_info_t *p_x2ap_srvd_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_srvd_cell_info, 0, sizeof(x2ap_srvd_cell_info_t));

    /* This function parses x2ap_srvd_cell_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_srvd_cell_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_srvd_cell_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U32);

    if (RRC_FAILURE == rrc_il_parse_x2ap_ecgi(
        &p_x2ap_srvd_cell_info->cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_tac(
        &p_x2ap_srvd_cell_info->track_area_code,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_broadcast_plmn(
        &p_x2ap_srvd_cell_info->broadcast_plmn_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_choice_eutra_mode_info(
        &p_x2ap_srvd_cell_info->choice_eutra_mode,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_srvd_cell_info->bitmask & NUM_ANTENNA_PORT_BITMASK)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_srvd_cell_info->num_antenna_port, p_src + *p_length_read, "num_antenna_port");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_srvd_cell_info->num_antenna_port > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_srvd_cell_info->num_antenna_port] should be less than"
            " or equal to 2. Incorrect value %u received.", p_x2ap_srvd_cell_info->num_antenna_port);
        return RRC_FAILURE;
    }
    }

    if (p_x2ap_srvd_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_prach_configuration_info(
        &p_x2ap_srvd_cell_info->prach_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_srvd_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_mbsfn_subframe_info(
        &p_x2ap_srvd_cell_info->mbsfn_subframe_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_srvd_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_x2ap_srvd_cell_info->csg_identity) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_srvd_cell_info->csg_identity); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_srvd_cell_info->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_neighbour_info
(
    x2ap_neighbour_info_t *p_x2ap_neighbour_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_neighbour_info, 0, sizeof(x2ap_neighbour_info_t));

    /* This function parses x2ap_neighbour_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_neighbour_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (RRC_FAILURE == rrc_il_parse_x2ap_ecgi(
        &p_x2ap_neighbour_info->ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_neighbour_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U32);

    if (RRC_FAILURE == rrc_il_parse_x2ap_earfcn(
        &p_x2ap_neighbour_info->earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_nbour_cell_arry
(
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nbour_cell_arry, 0, sizeof(x2ap_nbour_cell_arry_t));

    /* This function parses x2ap_nbour_cell_arry */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_nbour_cell_arry->num_neighbour_cell, p_src + *p_length_read, "num_neighbour_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_arry->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nbour_cell_arry->num_neighbour_cell] should be less than"
            " or equal to MAX_NEIGHBOURS. Incorrect value %u received.", p_x2ap_nbour_cell_arry->num_neighbour_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_arry->num_neighbour_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_neighbour_info(
                &p_x2ap_nbour_cell_arry->neighbour_info_arr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_srvd_cell_arry
(
    x2ap_srvd_cell_arry_t *p_x2ap_srvd_cell_arry,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_srvd_cell_arry, 0, sizeof(x2ap_srvd_cell_arry_t));

    /* This function parses x2ap_srvd_cell_arry */

    if (RRC_FAILURE == rrc_il_parse_x2ap_srvd_cell_info(
        &p_x2ap_srvd_cell_arry->served_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_nbour_cell_arry(
        &p_x2ap_srvd_cell_arry->nbour_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_cell_info
(
    x2ap_rrm_cell_info_t *p_x2ap_rrm_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_cell_info, 0, sizeof(x2ap_rrm_cell_info_t));

    /* This function parses x2ap_rrm_cell_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_cell_info->served_cell_id, p_src + *p_length_read, "served_cell_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_cell_info->served_cell_id > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_cell_info->served_cell_id] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_x2ap_rrm_cell_info->served_cell_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_srvd_cell_arry(
        &p_x2ap_rrm_cell_info->ngh_dir_cell_arr,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_gu_group_id
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_gu_group_id, 0, sizeof(x2ap_gu_group_id_t));

    /* This function parses x2ap_gu_group_id */

    if (RRC_FAILURE == rrc_il_parse_x2ap_plmn_identity(
        &p_x2ap_gu_group_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_x2ap_gu_group_id->mme_group_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_gu_group_id->mme_group_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_gu_group_id->mme_group_id[loop], (void*)(p_src + *p_length_read), "mme_group_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_eutra_cell_info
(
    x2ap_rrm_eutra_cell_info_t *p_x2ap_rrm_eutra_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_eutra_cell_info, 0, sizeof(x2ap_rrm_eutra_cell_info_t));

    /* This function parses x2ap_rrm_eutra_cell_info */

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rrm_eutra_cell_info->global_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_eutra_cell_info->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_eutra_cell_info->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_eutra_cell_info->num_served_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_eutra_cell_info->num_served_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_eutra_cell_info->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_cell_info(
                &p_x2ap_rrm_eutra_cell_info->cell_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_eutra_cell_info->num_gu_group_id, p_src + *p_length_read, "num_gu_group_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_eutra_cell_info->num_gu_group_id > MAX_ENB_POOLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_eutra_cell_info->num_gu_group_id] should be less than"
            " or equal to MAX_ENB_POOLS. Incorrect value %u received.", p_x2ap_rrm_eutra_cell_info->num_gu_group_id);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_eutra_cell_info->num_gu_group_id; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_gu_group_id(
                &p_x2ap_rrm_eutra_cell_info->group_id_arr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_nr_ecgi
(
    x2ap_nr_ecgi_t *p_x2ap_nr_ecgi,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_ecgi, 0, sizeof(x2ap_nr_ecgi_t));

    /* This function parses x2ap_nr_ecgi */

    if (RRC_FAILURE == rrc_il_parse_x2ap_plmn_identity(
        &p_x2ap_nr_ecgi->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_x2ap_nr_ecgi->nr_cell_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_nr_ecgi->nr_cell_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_nr_ecgi->nr_cell_id[loop], (void*)(p_src + *p_length_read), "nr_cell_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_nr_freq_band_info
(
    x2ap_nr_freq_band_info_t *p_x2ap_nr_freq_band_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_freq_band_info, 0, sizeof(x2ap_nr_freq_band_info_t));

    /* This function parses x2ap_nr_freq_band_info */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_nr_freq_band_info->freq_band_indicator_nr, p_src + *p_length_read, "freq_band_indicator_nr");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_nr_freq_band_info->freq_band_indicator_nr < 1) || (p_x2ap_nr_freq_band_info->freq_band_indicator_nr > NR_MAX_FREQ_BAND_INDICATOR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_freq_band_info->freq_band_indicator_nr] should be in range "
            "1 to NR_MAX_FREQ_BAND_INDICATOR. Incorrect value %u received.", p_x2ap_nr_freq_band_info->freq_band_indicator_nr);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_nr_freq_band_info->num_supported_sul_band, p_src + *p_length_read, "num_supported_sul_band");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_freq_band_info->num_supported_sul_band > NR_MAX_FREQ_BANDS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_freq_band_info->num_supported_sul_band] should be less than"
            " or equal to NR_MAX_FREQ_BANDS. Incorrect value %u received.", p_x2ap_nr_freq_band_info->num_supported_sul_band);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_x2ap_nr_freq_band_info->num_supported_sul_band * sizeof(p_x2ap_nr_freq_band_info->supported_sul_freq_band_indicator_nr[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements with BOTH boundaries
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nr_freq_band_info->num_supported_sul_band; loop++)
        {
            rrc_cp_unpack_U16((void*)&p_x2ap_nr_freq_band_info->supported_sul_freq_band_indicator_nr[loop], (void*)(p_src + *p_length_read), "supported_sul_freq_band_indicator_nr[]");
            if ((p_x2ap_nr_freq_band_info->supported_sul_freq_band_indicator_nr[loop] < 1) || (p_x2ap_nr_freq_band_info->supported_sul_freq_band_indicator_nr[loop] > NR_MAX_FREQ_BAND_INDICATOR))
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_x2ap_nr_freq_band_info->supported_sul_freq_band_indicator_nr[%u] should be in "
                    "range 1 to NR_MAX_FREQ_BAND_INDICATOR. "
                    "Incorrect value %d received.", loop, p_x2ap_nr_freq_band_info->supported_sul_freq_band_indicator_nr[loop]);
                return RRC_FAILURE;
            }
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_nr_tx_bandwidth
(
    x2ap_nr_tx_bandwidth_t *p_x2ap_nr_tx_bandwidth,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_tx_bandwidth, 0, sizeof(x2ap_nr_tx_bandwidth_t));

    /* This function parses x2ap_nr_tx_bandwidth */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_nr_tx_bandwidth->nr_scs, p_src + *p_length_read, "nr_scs");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_tx_bandwidth->nr_scs > MAX_NRCS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_tx_bandwidth->nr_scs] should be less than"
            " or equal to MAX_NRCS. Incorrect value %u received.", p_x2ap_nr_tx_bandwidth->nr_scs);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_nr_tx_bandwidth->nr_nrb, p_src + *p_length_read, "nr_nrb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_tx_bandwidth->nr_nrb > X2AP_NRB_MAX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_tx_bandwidth->nr_nrb] should be less than"
            " or equal to X2AP_NRB_MAX. Incorrect value %u received.", p_x2ap_nr_tx_bandwidth->nr_nrb);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_sul_information
(
    x2ap_sul_information_t *p_x2ap_sul_information,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_sul_information, 0, sizeof(x2ap_sul_information_t));

    /* This function parses x2ap_sul_information */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_sul_information->nr_sul_arfcn, p_src + *p_length_read, "nr_sul_arfcn");
    *p_length_read += sizeof(U32);

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_tx_bandwidth(
        &p_x2ap_sul_information->nr_tx_band_width,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_nr_freq_info
(
    x2ap_nr_freq_info_t *p_x2ap_nr_freq_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_freq_info, 0, sizeof(x2ap_nr_freq_info_t));

    /* This function parses x2ap_nr_freq_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_nr_freq_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_nr_freq_info->nr_earfcn, p_src + *p_length_read, "nr_earfcn");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_freq_info->nr_earfcn > NR_MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_freq_info->nr_earfcn] should be less than"
            " or equal to NR_MAX_EARFCN. Incorrect value %u received.", p_x2ap_nr_freq_info->nr_earfcn);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_nr_freq_info->num_freq_bands, p_src + *p_length_read, "num_freq_bands");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_nr_freq_info->num_freq_bands < 1) || (p_x2ap_nr_freq_info->num_freq_bands > NR_MAX_FREQ_BANDS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_freq_info->num_freq_bands] should be in range "
            "1 to NR_MAX_FREQ_BANDS. Incorrect value %u received.", p_x2ap_nr_freq_info->num_freq_bands);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nr_freq_info->num_freq_bands; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_nr_freq_band_info(
                &p_x2ap_nr_freq_info->freq_band_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_x2ap_nr_freq_info->bitmask & X2AP_NR_ARFCN_SUL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_nr_freq_info->nr_earfcn_sul, p_src + *p_length_read, "nr_earfcn_sul");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_freq_info->nr_earfcn_sul > NR_MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_freq_info->nr_earfcn_sul] should be less than"
            " or equal to NR_MAX_EARFCN. Incorrect value %u received.", p_x2ap_nr_freq_info->nr_earfcn_sul);
        return RRC_FAILURE;
    }
    }

    if (p_x2ap_nr_freq_info->bitmask & X2AP_SUL_INFORMATION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_sul_information(
        &p_x2ap_nr_freq_info->nr_sul_information,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_nr_fdd_into
(
    x2ap_nr_fdd_into_t *p_x2ap_nr_fdd_into,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_fdd_into, 0, sizeof(x2ap_nr_fdd_into_t));

    /* This function parses x2ap_nr_fdd_into */

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_freq_info(
        &p_x2ap_nr_fdd_into->nr_ul_freq_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_freq_info(
        &p_x2ap_nr_fdd_into->nr_dl_freq_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_tx_bandwidth(
        &p_x2ap_nr_fdd_into->nr_ul_tx_bdwidth,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_tx_bandwidth(
        &p_x2ap_nr_fdd_into->nr_dl_tx_bdwidth,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_nr_tdd_into
(
    x2ap_nr_tdd_into_t *p_x2ap_nr_tdd_into,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_tdd_into, 0, sizeof(x2ap_nr_tdd_into_t));

    /* This function parses x2ap_nr_tdd_into */

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_freq_info(
        &p_x2ap_nr_tdd_into->nr_freq_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_tx_bandwidth(
        &p_x2ap_nr_tdd_into->nr_tx_bandwidth,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_choice_nr_mode_info
(
    x2ap_choice_nr_mode_info_t *p_x2ap_choice_nr_mode_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_choice_nr_mode_info, 0, sizeof(x2ap_choice_nr_mode_info_t));

    /* This function parses x2ap_choice_nr_mode_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_choice_nr_mode_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_x2ap_choice_nr_mode_info->bitmask & X2AP_NR_MODE_FDD_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_fdd_into(
        &p_x2ap_choice_nr_mode_info->nr_fdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_choice_nr_mode_info->bitmask & X2AP_NR_MODE_TDD_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_tdd_into(
        &p_x2ap_choice_nr_mode_info->nr_tdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_periodicity_and_offset
(
    x2ap_periodicity_and_offset_t *p_x2ap_periodicity_and_offset,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_periodicity_and_offset, 0, sizeof(x2ap_periodicity_and_offset_t));

    /* This function parses x2ap_periodicity_and_offset */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_periodicity_and_offset->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF5_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_periodicity_and_offset->sf5, p_src + *p_length_read, "sf5");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_periodicity_and_offset->sf5 > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_periodicity_and_offset->sf5] should be less than"
            " or equal to 4. Incorrect value %u received.", p_x2ap_periodicity_and_offset->sf5);
        return RRC_FAILURE;
    }
    }

    if (p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF10_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_periodicity_and_offset->sf10, p_src + *p_length_read, "sf10");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_periodicity_and_offset->sf10 > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_periodicity_and_offset->sf10] should be less than"
            " or equal to 9. Incorrect value %u received.", p_x2ap_periodicity_and_offset->sf10);
        return RRC_FAILURE;
    }
    }

    if (p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF20_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_periodicity_and_offset->sf20, p_src + *p_length_read, "sf20");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_periodicity_and_offset->sf20 > 19))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_periodicity_and_offset->sf20] should be less than"
            " or equal to 19. Incorrect value %u received.", p_x2ap_periodicity_and_offset->sf20);
        return RRC_FAILURE;
    }
    }

    if (p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF40_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_periodicity_and_offset->sf40, p_src + *p_length_read, "sf40");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_periodicity_and_offset->sf40 > 39))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_periodicity_and_offset->sf40] should be less than"
            " or equal to 39. Incorrect value %u received.", p_x2ap_periodicity_and_offset->sf40);
        return RRC_FAILURE;
    }
    }

    if (p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF80_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_periodicity_and_offset->sf80, p_src + *p_length_read, "sf80");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_periodicity_and_offset->sf80 > 79))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_periodicity_and_offset->sf80] should be less than"
            " or equal to 79. Incorrect value %u received.", p_x2ap_periodicity_and_offset->sf80);
        return RRC_FAILURE;
    }
    }

    if (p_x2ap_periodicity_and_offset->bitmask & X2AP_PERIODICTY_OFFSET_SF160_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_periodicity_and_offset->sf160, p_src + *p_length_read, "sf160");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_periodicity_and_offset->sf160 > 159))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_periodicity_and_offset->sf160] should be less than"
            " or equal to 159. Incorrect value %u received.", p_x2ap_periodicity_and_offset->sf160);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_ssb_mtc
(
    x2ap_ssb_mtc_t *p_x2ap_ssb_mtc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_ssb_mtc, 0, sizeof(x2ap_ssb_mtc_t));

    /* This function parses x2ap_ssb_mtc */

    if (RRC_FAILURE == rrc_il_parse_x2ap_periodicity_and_offset(
        &p_x2ap_ssb_mtc->x2ap_periodicity_and_offset,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_ssb_mtc->x2ap_mtc_duration, p_src + *p_length_read, "x2ap_mtc_duration");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_ssb_mtc->x2ap_mtc_duration > X2AP_DURATION_SF5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_ssb_mtc->x2ap_mtc_duration] should be less than"
            " or equal to X2AP_DURATION_SF5. Incorrect value %u received.", p_x2ap_ssb_mtc->x2ap_mtc_duration);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_measurement_slots
(
    x2ap_measurement_slots_t *p_x2ap_measurement_slots,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_measurement_slots, 0, sizeof(x2ap_measurement_slots_t));

    /* This function parses x2ap_measurement_slots */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_measurement_slots->numbits, p_src + *p_length_read, "numbits");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_measurement_slots->numbits < 1) || (p_x2ap_measurement_slots->numbits > 80))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_measurement_slots->numbits] should be in range "
            "1 to 80. Incorrect value %u received.", p_x2ap_measurement_slots->numbits);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_x2ap_measurement_slots->numbits * sizeof(p_x2ap_measurement_slots->meas_slots[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_measurement_slots->numbits; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_measurement_slots->meas_slots[loop], (void*)(p_src + *p_length_read), "meas_slots[]");
            if (p_x2ap_measurement_slots->meas_slots[loop] > 1)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_x2ap_measurement_slots->meas_slots[%u] should be less"
                    " than or equal to 1. "
                    "Incorrect value %d received.", loop, p_x2ap_measurement_slots->meas_slots[loop]);
                return RRC_FAILURE;
            }
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_ss_rssi_measurement
(
    x2ap_ss_rssi_measurement_t *p_x2ap_ss_rssi_measurement,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_ss_rssi_measurement, 0, sizeof(x2ap_ss_rssi_measurement_t));

    /* This function parses x2ap_ss_rssi_measurement */

    if (RRC_FAILURE == rrc_il_parse_x2ap_measurement_slots(
        &p_x2ap_ss_rssi_measurement->x2ap_measurement_slots,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_ss_rssi_measurement->end_synmbol, p_src + *p_length_read, "end_synmbol");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_ss_rssi_measurement->end_synmbol > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_ss_rssi_measurement->end_synmbol] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_ss_rssi_measurement->end_synmbol);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_meas_timing_config
(
    x2ap_meas_timing_config_t *p_x2ap_meas_timing_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_meas_timing_config, 0, sizeof(x2ap_meas_timing_config_t));

    /* This function parses x2ap_meas_timing_config */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_meas_timing_config->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_meas_timing_config->carrier_freq, p_src + *p_length_read, "carrier_freq");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_meas_timing_config->carrier_freq > NR_MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_meas_timing_config->carrier_freq] should be less than"
            " or equal to NR_MAX_EARFCN. Incorrect value %u received.", p_x2ap_meas_timing_config->carrier_freq);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_meas_timing_config->ssb_subcarrier_spacing, p_src + *p_length_read, "ssb_subcarrier_spacing");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_meas_timing_config->ssb_subcarrier_spacing > X2AP_KHZ_240))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_meas_timing_config->ssb_subcarrier_spacing] should be less than"
            " or equal to X2AP_KHZ_240. Incorrect value %u received.", p_x2ap_meas_timing_config->ssb_subcarrier_spacing);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_ssb_mtc(
        &p_x2ap_meas_timing_config->ssb_meas_timimg_configuration,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_meas_timing_config->bitmask & X2AP_MEAS_SS_RSSI_MEASUREMENT_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_ss_rssi_measurement(
        &p_x2ap_meas_timing_config->ss_rssi_measurement,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_five_gs_tac
(
    x2ap_five_gs_tac_t *p_x2ap_five_gs_tac,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_five_gs_tac, 0, sizeof(x2ap_five_gs_tac_t));

    /* This function parses x2ap_five_gs_tac */

    if (*p_length_read + (S32)sizeof(p_x2ap_five_gs_tac->tac) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_five_gs_tac->tac); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_five_gs_tac->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_srvd_nr_cell_info
(
    x2ap_rrm_srvd_nr_cell_info_t *p_x2ap_rrm_srvd_nr_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_srvd_nr_cell_info, 0, sizeof(x2ap_rrm_srvd_nr_cell_info_t));

    /* This function parses x2ap_rrm_srvd_nr_cell_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_srvd_nr_cell_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_srvd_nr_cell_info->nr_pci, p_src + *p_length_read, "nr_pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_srvd_nr_cell_info->nr_pci > NR_MAX_PCI))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_srvd_nr_cell_info->nr_pci] should be less than"
            " or equal to NR_MAX_PCI. Incorrect value %u received.", p_x2ap_rrm_srvd_nr_cell_info->nr_pci);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_ecgi(
        &p_x2ap_rrm_srvd_nr_cell_info->nr_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_broadcast_plmn(
        &p_x2ap_rrm_srvd_nr_cell_info->broadcast_plmn_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_choice_nr_mode_info(
        &p_x2ap_rrm_srvd_nr_cell_info->choice_nr_mode,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

#ifdef LINUX_PC_TEST
     /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len, p_src + *p_length_read, "meas_timing_config_buf_len");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len > MAX_ASN_BUFFER))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len] should be less than"
            " or equal to MAX_ASN_BUFFER. Incorrect value %u received.", p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len * sizeof(p_x2ap_rrm_srvd_nr_cell_info->measurement_timing_config_asn_buff[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_buf_len; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_rrm_srvd_nr_cell_info->measurement_timing_config_asn_buff[loop], (void*)(p_src + *p_length_read), "measurement_timing_config_asn_buff[]");
            *p_length_read += sizeof(U8);
        }
    }
#else
    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count, p_src + *p_length_read, "meas_timing_config_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count < 1) || (p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count > RRC_MAX_MEAS_FREQ_MN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count] should be in range "
            "1 to RRC_MAX_MEAS_FREQ_MN. Incorrect value %u received.", p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_srvd_nr_cell_info->meas_timing_config_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_meas_timing_config(
                &p_x2ap_rrm_srvd_nr_cell_info->x2ap_meas_timing_config[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
#endif

    if (p_x2ap_rrm_srvd_nr_cell_info->bitmask & X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_five_gs_tac(
        &p_x2ap_rrm_srvd_nr_cell_info->fiveGS_track_area_code,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_srvd_nr_cell_info->bitmask & X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_tac(
        &p_x2ap_rrm_srvd_nr_cell_info->configured_tac,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_fdd_info_neigh_served_nr_cell_info
(
    x2ap_fdd_info_neigh_served_nr_cell_info_t *p_x2ap_fdd_info_neigh_served_nr_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_fdd_info_neigh_served_nr_cell_info, 0, sizeof(x2ap_fdd_info_neigh_served_nr_cell_info_t));

    /* This function parses x2ap_fdd_info_neigh_served_nr_cell_info */

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_freq_info(
        &p_x2ap_fdd_info_neigh_served_nr_cell_info->ul_nr_freq_Info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_freq_info(
        &p_x2ap_fdd_info_neigh_served_nr_cell_info->dl_nr_freq_Info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_tdd_info_neigh_served_nr_cell_info
(
    x2ap_tdd_info_neigh_served_nr_cell_info_t *p_x2ap_tdd_info_neigh_served_nr_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_tdd_info_neigh_served_nr_cell_info, 0, sizeof(x2ap_tdd_info_neigh_served_nr_cell_info_t));

    /* This function parses x2ap_tdd_info_neigh_served_nr_cell_info */

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_freq_info(
        &p_x2ap_tdd_info_neigh_served_nr_cell_info->nr_freq_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_nr_neigh_mode_info
(
    x2ap_nr_neigh_mode_info_t *p_x2ap_nr_neigh_mode_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_neigh_mode_info, 0, sizeof(x2ap_nr_neigh_mode_info_t));

    /* This function parses x2ap_nr_neigh_mode_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_nr_neigh_mode_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2ap_nr_neigh_mode_info->bitmask & X2AP_NR_NEIGHBOUR_FDD_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_fdd_info_neigh_served_nr_cell_info(
        &p_x2ap_nr_neigh_mode_info->fdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_nr_neigh_mode_info->bitmask & X2AP_NR_NEIGHBOUR_TDD_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_tdd_info_neigh_served_nr_cell_info(
        &p_x2ap_nr_neigh_mode_info->tdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_nr_neigh_info
(
    x2ap_nr_neigh_info_t *p_x2ap_nr_neigh_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_neigh_info, 0, sizeof(x2ap_nr_neigh_info_t));

    /* This function parses x2ap_nr_neigh_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_nr_neigh_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_nr_neigh_info->nr_pci, p_src + *p_length_read, "nr_pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_neigh_info->nr_pci > NR_MAX_PCI))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_neigh_info->nr_pci] should be less than"
            " or equal to NR_MAX_PCI. Incorrect value %u received.", p_x2ap_nr_neigh_info->nr_pci);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_ecgi(
        &p_x2ap_nr_neigh_info->nr_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_freq_info(
        &p_x2ap_nr_neigh_info->nr_freq_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

#ifdef LINUX_PC_TEST
     /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_nr_neigh_info->meas_timing_config_buf_len, p_src + *p_length_read, "meas_timing_config_buf_len");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nr_neigh_info->meas_timing_config_buf_len > MAX_ASN_BUFFER))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_neigh_info->meas_timing_config_buf_len] should be less than"
            " or equal to MAX_ASN_BUFFER. Incorrect value %u received.", p_x2ap_nr_neigh_info->meas_timing_config_buf_len);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_x2ap_nr_neigh_info->meas_timing_config_buf_len * sizeof(p_x2ap_nr_neigh_info->measurement_timing_config_asn_buff[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nr_neigh_info->meas_timing_config_buf_len; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_nr_neigh_info->measurement_timing_config_asn_buff[loop], (void*)(p_src + *p_length_read), "measurement_timing_config_asn_buff[]");
            *p_length_read += sizeof(U8);
        }
    }

#else
    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_nr_neigh_info->meas_timing_config_count, p_src + *p_length_read, "meas_timing_config_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_nr_neigh_info->meas_timing_config_count < 1) || (p_x2ap_nr_neigh_info->meas_timing_config_count > RRC_MAX_MEAS_FREQ_MN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_neigh_info->meas_timing_config_count] should be in range "
            "1 to RRC_MAX_MEAS_FREQ_MN. Incorrect value %u received.", p_x2ap_nr_neigh_info->meas_timing_config_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nr_neigh_info->meas_timing_config_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_meas_timing_config(
                &p_x2ap_nr_neigh_info->x2ap_meas_timing_config[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
#endif    

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_neigh_mode_info(
        &p_x2ap_nr_neigh_info->nr_neigh_Mode_Info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_nr_neigh_info->bitmask & X2AP_FIVE_GS_TAC_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_five_gs_tac(
        &p_x2ap_nr_neigh_info->fivegs_tac,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_nr_neigh_info->bitmask & X2AP_CONFIGURED_TAC_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_tac(
        &p_x2ap_nr_neigh_info->configured_tac,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_nr_neigh_cell_arry
(
    x2ap_nr_neigh_cell_arry_t *p_x2ap_nr_neigh_cell_arry,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_neigh_cell_arry, 0, sizeof(x2ap_nr_neigh_cell_arry_t));

    /* This function parses x2ap_nr_neigh_cell_arry */

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_neigh_info(
        &p_x2ap_nr_neigh_cell_arry->nr_neigh_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_nr_cell_info
(
    x2ap_rrm_nr_cell_info_t *p_x2ap_rrm_nr_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_nr_cell_info, 0, sizeof(x2ap_rrm_nr_cell_info_t));

    /* This function parses x2ap_rrm_nr_cell_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_nr_cell_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_nr_cell_info->default_sgnb, p_src + *p_length_read, "default_sgnb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_nr_cell_info->default_sgnb > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_nr_cell_info->default_sgnb] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_rrm_nr_cell_info->default_sgnb);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2_gb_gnb_id(
        &p_x2ap_rrm_nr_cell_info->global_gnb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_nr_cell_info->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_nr_cell_info->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_nr_cell_info->num_served_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_nr_cell_info->num_served_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_nr_cell_info->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_srvd_nr_cell_info(
                &p_x2ap_rrm_nr_cell_info->served_cell_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_nr_cell_info->num_neigh_cell, p_src + *p_length_read, "num_neigh_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_nr_cell_info->num_neigh_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_nr_cell_info->num_neigh_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_nr_cell_info->num_neigh_cell);
        return RRC_FAILURE;
    }

    if (p_x2ap_rrm_nr_cell_info->bitmask & NR_NEIGHBOUR_INFO_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_nr_cell_info->num_neigh_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_nr_neigh_cell_arry(
                &p_x2ap_rrm_nr_cell_info->nr_neigh_cell_arr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_rrm_start_req
(
    x2ap_rrm_rsu_rrm_start_req_t *p_x2ap_rrm_rsu_rrm_start_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_rrm_start_req, 0, sizeof(x2ap_rrm_rsu_rrm_start_req_t));

    /* This function parses x2ap_rrm_rsu_rrm_start_req */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_rsu_rrm_start_req->report_characteristics, p_src + *p_length_read, "report_characteristics");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_rsu_rrm_start_req->num_cell, p_src + *p_length_read, "num_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_rrm_start_req->num_cell < 1) || (p_x2ap_rrm_rsu_rrm_start_req->num_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_start_req->num_cell] should be in range "
            "1 to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_start_req->num_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_rrm_start_req->num_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_rrm_cell_global_id_eutra(
                &p_x2ap_rrm_rsu_rrm_start_req->cell_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_rrm_start_req->report_periodicity, p_src + *p_length_read, "report_periodicity");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_rrm_start_req->report_periodicity > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_start_req->report_periodicity] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_start_req->report_periodicity);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_rsu_rrm_start_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2ap_rrm_rsu_rrm_start_req->bitmask & RRM_RSU_START_PARTIAL_SUCC_IND_IS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_rrm_start_req->partial_success_indicator, p_src + *p_length_read, "partial_success_indicator");
    *p_length_read += sizeof(U8);
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_mnc
(
    mnc_t *p_mnc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_mnc, 0, sizeof(mnc_t));

    /* This function parses mnc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_mnc->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mnc->count < 2) || (p_mnc->count > MAX_MNC_OCTET_SIZE))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mnc->count] should be in range "
            "2 to MAX_MNC_OCTET_SIZE. Incorrect value %u received.", p_mnc->count);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_mnc->count * sizeof(p_mnc->mnc[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mnc->count; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_mnc->mnc[loop], (void*)(p_src + *p_length_read), "mnc[]");
            if (p_mnc->mnc[loop] > 9)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_mnc->mnc[%u] should be less"
                    " than or equal to 9. "
                    "Incorrect value %d received.", loop, p_mnc->mnc[loop]);
                return RRC_FAILURE;
            }
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_plmn_identity
(
    plmn_identity_t *p_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_plmn_identity, 0, sizeof(plmn_identity_t));

    /* This function parses plmn_identity */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_plmn_identity->presence_bitmask, p_src + *p_length_read, "presence_bitmask");
    *p_length_read += sizeof(U16);

    if (p_plmn_identity->presence_bitmask & 1)
    {

    if (*p_length_read + (S32)sizeof(p_plmn_identity->mcc) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_plmn_identity->mcc); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_plmn_identity->mcc[loop], (void*)(p_src + *p_length_read), "mcc[]");
            if (p_plmn_identity->mcc[loop] > 9)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_plmn_identity->mcc[%u] should be less"
                    " than or equal to 9. "
                    "Incorrect value %d received.", loop, p_plmn_identity->mcc[loop]);
                return RRC_FAILURE;
            }
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (RRC_FAILURE == rrc_il_parse_mnc(
        &p_plmn_identity->mnc,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrm_cell_global_id_eutra
(
    rrm_cell_global_id_eutra_t *p_rrm_cell_global_id_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrm_cell_global_id_eutra, 0, sizeof(rrm_cell_global_id_eutra_t));

    /* This function parses rrm_cell_global_id_eutra */

    if (RRC_FAILURE == rrc_il_parse_plmn_identity(
        &p_rrm_cell_global_id_eutra->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rrm_cell_global_id_eutra->cell_identity) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_cell_global_id_eutra->cell_identity); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrm_cell_global_id_eutra->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_enb_start_res
(
    x2ap_rrm_rsu_enb_start_res_t *p_x2ap_rrm_rsu_enb_start_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_enb_start_res, 0, sizeof(x2ap_rrm_rsu_enb_start_res_t));

    /* This function parses x2ap_rrm_rsu_enb_start_res */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_rsu_enb_start_res->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_enb_start_res->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_enb_start_res->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_start_res->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_start_res->response);
        return RRC_FAILURE;
    }

    if (p_x2ap_rrm_rsu_enb_start_res->bitmask & ENB_MEAS_ID_IS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_rsu_enb_start_res->enb_meas_id, p_src + *p_length_read, "enb_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_enb_start_res->enb_meas_id < 1) || (p_x2ap_rrm_rsu_enb_start_res->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_start_res->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_start_res->enb_meas_id);
        return RRC_FAILURE;
    }
    }

    if (p_x2ap_rrm_rsu_enb_start_res->bitmask & ENB_START_RES_RRM_CAUSE_IS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_cause(
        &p_x2ap_rrm_rsu_enb_start_res->rrm_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_rsu_enb_start_res->bitmask & ENB_ID_IS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rrm_rsu_enb_start_res->global_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_rsu_enb_start_res->bitmask & ENB_START_PARTIAL_SUCCESS)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_rsu_measurement_res(
        &p_x2ap_rrm_rsu_enb_start_res->rrm_rsu_measurement_res_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_cause
(
    x2ap_rrm_cause_t *p_x2ap_rrm_cause,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_cause, 0, sizeof(x2ap_rrm_cause_t));

    /* This function parses x2ap_rrm_cause */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_cause->type, p_src + *p_length_read, "type");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_cause->value, p_src + *p_length_read, "value");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_measurement_failure_cause_item
(
    measurement_failure_cause_item_t *p_measurement_failure_cause_item,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_measurement_failure_cause_item, 0, sizeof(measurement_failure_cause_item_t));

    /* This function parses measurement_failure_cause_item */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_measurement_failure_cause_item->report_characteristics, p_src + *p_length_read, "report_characteristics");
    *p_length_read += sizeof(U32);

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_cause(
        &p_measurement_failure_cause_item->rrm_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rsu_measurement_res_item
(
    rsu_measurement_res_item_t *p_rsu_measurement_res_item,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rsu_measurement_res_item, 0, sizeof(rsu_measurement_res_item_t));

    /* This function parses rsu_measurement_res_item */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rsu_measurement_res_item->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_rrm_cell_global_id_eutra(
        &p_rsu_measurement_res_item->cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rsu_measurement_res_item->bitmask & X2AP_RRM_FAILED_CAUSE_NUM_ITEM_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rsu_measurement_res_item->num_measurement, p_src + *p_length_read, "num_measurement");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rsu_measurement_res_item->num_measurement < 1) || (p_rsu_measurement_res_item->num_measurement > MAX_ENB_MEASUREMENTS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rsu_measurement_res_item->num_measurement] should be in range "
            "1 to MAX_ENB_MEASUREMENTS. Incorrect value %u received.", p_rsu_measurement_res_item->num_measurement);
        return RRC_FAILURE;
    }
    }

    if (p_rsu_measurement_res_item->bitmask & X2AP_RRM_FAILED_CAUSE_ITEM_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rsu_measurement_res_item->num_measurement; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_measurement_failure_cause_item(
                &p_rsu_measurement_res_item->measurement_failure_cause_item[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_measurement_res
(
    x2ap_rrm_rsu_measurement_res_t *p_x2ap_rrm_rsu_measurement_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_measurement_res, 0, sizeof(x2ap_rrm_rsu_measurement_res_t));

    /* This function parses x2ap_rrm_rsu_measurement_res */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_rsu_measurement_res->num_cell, p_src + *p_length_read, "num_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_measurement_res->num_cell < 1) || (p_x2ap_rrm_rsu_measurement_res->num_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_measurement_res->num_cell] should be in range "
            "1 to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_rsu_measurement_res->num_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_measurement_res->num_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_rsu_measurement_res_item(
                &p_x2ap_rrm_rsu_measurement_res->rsu_measurement_res_item[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_enb_stop_res
(
    x2ap_rrm_rsu_enb_stop_res_t *p_x2ap_rrm_rsu_enb_stop_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_enb_stop_res, 0, sizeof(x2ap_rrm_rsu_enb_stop_res_t));

    /* This function parses x2ap_rrm_rsu_enb_stop_res */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_enb_stop_res->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_rsu_enb_stop_res->enb_meas_id, p_src + *p_length_read, "enb_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_enb_stop_res->enb_meas_id < 1) || (p_x2ap_rrm_rsu_enb_stop_res->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_stop_res->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_stop_res->enb_meas_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rrm_rsu_enb_stop_res->peer_gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_enb_stop_res->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_enb_stop_res->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_stop_res->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_stop_res->response);
        return RRC_FAILURE;
    }

    if (p_x2ap_rrm_rsu_enb_stop_res->bitmask & ENB_STOP_RES_RRM_CAUSE_IS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_cause(
        &p_x2ap_rrm_rsu_enb_stop_res->rrm_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_rrm_start_res
(
    x2ap_rrm_rsu_rrm_start_res_t *p_x2ap_rrm_rsu_rrm_start_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_rrm_start_res, 0, sizeof(x2ap_rrm_rsu_rrm_start_res_t));

    /* This function parses x2ap_rrm_rsu_rrm_start_res */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_rsu_rrm_start_res->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_rsu_rrm_start_res->enb_meas_id, p_src + *p_length_read, "enb_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_rrm_start_res->enb_meas_id < 1) || (p_x2ap_rrm_rsu_rrm_start_res->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_start_res->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_start_res->enb_meas_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rrm_rsu_rrm_start_res->peer_gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_rrm_start_res->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_rrm_start_res->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_start_res->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_start_res->response);
        return RRC_FAILURE;
    }

    if (p_x2ap_rrm_rsu_rrm_start_res->bitmask & RRM_START_RES_RRM_CAUSE_IS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_cause(
        &p_x2ap_rrm_rsu_rrm_start_res->rrm_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_rsu_rrm_start_res->bitmask & ENB_START_PARTIAL_SUCCESS)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_rsu_measurement_res(
        &p_x2ap_rrm_rsu_rrm_start_res->rrm_rsu_measurement_res_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_rrm_stop_res
(
    x2ap_rrm_rsu_rrm_stop_res_t *p_x2ap_rrm_rsu_rrm_stop_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_rrm_stop_res, 0, sizeof(x2ap_rrm_rsu_rrm_stop_res_t));

    /* This function parses x2ap_rrm_rsu_rrm_stop_res */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_rsu_rrm_stop_res->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_rsu_rrm_stop_res->enb_meas_id, p_src + *p_length_read, "enb_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_rrm_stop_res->enb_meas_id < 1) || (p_x2ap_rrm_rsu_rrm_stop_res->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_stop_res->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_stop_res->enb_meas_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rrm_rsu_rrm_stop_res->peer_gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_rrm_stop_res->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_rrm_stop_res->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_stop_res->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_stop_res->response);
        return RRC_FAILURE;
    }

    if (p_x2ap_rrm_rsu_rrm_stop_res->bitmask & RRM_STOP_RES_RRM_CAUSE_IS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_cause(
        &p_x2ap_rrm_rsu_rrm_stop_res->rrm_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_enb_start_req
(
    x2ap_rrm_rsu_enb_start_req_t *p_x2ap_rrm_rsu_enb_start_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_enb_start_req, 0, sizeof(x2ap_rrm_rsu_enb_start_req_t));

    /* This function parses x2ap_rrm_rsu_enb_start_req */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_rsu_enb_start_req->report_characteristics, p_src + *p_length_read, "report_characteristics");
    *p_length_read += sizeof(U32);

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rrm_rsu_enb_start_req->peer_gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_rsu_enb_start_req->enb_meas_id, p_src + *p_length_read, "enb_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_enb_start_req->enb_meas_id < 1) || (p_x2ap_rrm_rsu_enb_start_req->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_start_req->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_start_req->enb_meas_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_rsu_enb_start_req->num_cell, p_src + *p_length_read, "num_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_enb_start_req->num_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_start_req->num_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_start_req->num_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_enb_start_req->num_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_rrm_cell_global_id_eutra(
                &p_x2ap_rrm_rsu_enb_start_req->cell_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_enb_start_req->report_periodicity, p_src + *p_length_read, "report_periodicity");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_enb_start_req->report_periodicity > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_start_req->report_periodicity] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_start_req->report_periodicity);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_rsu_enb_start_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2ap_rrm_rsu_enb_start_req->bitmask & ENB_START_PARTIAL_SUCCESS)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_enb_start_req->partial_success_indicator_present, p_src + *p_length_read, "partial_success_indicator_present");
    *p_length_read += sizeof(U8);
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_rrm_update
(
    x2ap_rrm_rsu_rrm_update_t *p_x2ap_rrm_rsu_rrm_update,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_rrm_update, 0, sizeof(x2ap_rrm_rsu_rrm_update_t));

    /* This function parses x2ap_rrm_rsu_rrm_update */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_rsu_rrm_update->enb_meas_id, p_src + *p_length_read, "enb_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_rrm_update->enb_meas_id < 1) || (p_x2ap_rrm_rsu_rrm_update->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_update->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_update->enb_meas_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rrm_rsu_rrm_update->peer_gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_rsu_rrm_update->num_reports, p_src + *p_length_read, "num_reports");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_rrm_update->num_reports > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_update->num_reports] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_update->num_reports);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_rrm_update->num_reports; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_rsu_report_info(
                &p_x2ap_rrm_rsu_rrm_update->rsu_report[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_prb
(
    x2ap_rrm_rsu_prb_t *p_x2ap_rrm_rsu_prb,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_prb, 0, sizeof(x2ap_rrm_rsu_prb_t));

    /* This function parses x2ap_rrm_rsu_prb */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_prb->dl_gbr_prb_usage, p_src + *p_length_read, "dl_gbr_prb_usage");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_prb->ul_gbr_prb_usage, p_src + *p_length_read, "ul_gbr_prb_usage");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_prb->dl_non_gbr_prb_usage, p_src + *p_length_read, "dl_non_gbr_prb_usage");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_prb->ul_non_gbr_prb_usage, p_src + *p_length_read, "ul_non_gbr_prb_usage");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_prb->dl_total_gbr_prb_usage, p_src + *p_length_read, "dl_total_gbr_prb_usage");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_prb->ul_total_gbr_prb_usage, p_src + *p_length_read, "ul_total_gbr_prb_usage");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_tnl_load
(
    x2ap_rrm_rsu_tnl_load_t *p_x2ap_rrm_rsu_tnl_load,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_tnl_load, 0, sizeof(x2ap_rrm_rsu_tnl_load_t));

    /* This function parses x2ap_rrm_rsu_tnl_load */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_tnl_load->dl_s1_tnl_load_indicator, p_src + *p_length_read, "dl_s1_tnl_load_indicator");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_tnl_load->dl_s1_tnl_load_indicator > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_tnl_load->dl_s1_tnl_load_indicator] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_rsu_tnl_load->dl_s1_tnl_load_indicator);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_tnl_load->ul_s1_tnl_load_indicator, p_src + *p_length_read, "ul_s1_tnl_load_indicator");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_tnl_load->ul_s1_tnl_load_indicator > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_tnl_load->ul_s1_tnl_load_indicator] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_rsu_tnl_load->ul_s1_tnl_load_indicator);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_hw_load
(
    x2ap_rrm_rsu_hw_load_t *p_x2ap_rrm_rsu_hw_load,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_hw_load, 0, sizeof(x2ap_rrm_rsu_hw_load_t));

    /* This function parses x2ap_rrm_rsu_hw_load */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_hw_load->dl_hardware_load_indicator, p_src + *p_length_read, "dl_hardware_load_indicator");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_hw_load->dl_hardware_load_indicator > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_hw_load->dl_hardware_load_indicator] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_rsu_hw_load->dl_hardware_load_indicator);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_hw_load->ul_hardware_load_indicator, p_src + *p_length_read, "ul_hardware_load_indicator");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_hw_load->ul_hardware_load_indicator > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_hw_load->ul_hardware_load_indicator] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_rsu_hw_load->ul_hardware_load_indicator);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_composite_available_capacity
(
    x2ap_rrm_rsu_composite_available_capacity_t *p_x2ap_rrm_rsu_composite_available_capacity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_composite_available_capacity, 0, sizeof(x2ap_rrm_rsu_composite_available_capacity_t));

    /* This function parses x2ap_rrm_rsu_composite_available_capacity */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_composite_available_capacity->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_x2ap_rrm_rsu_composite_available_capacity->bitmask & CELL_CAPACITY_CLASS_VALUE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_class_value, p_src + *p_length_read, "cell_capacity_class_value");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_class_value < 1) || (p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_class_value > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_class_value] should be in range "
            "1 to 100. Incorrect value %u received.", p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_class_value);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_value, p_src + *p_length_read, "cell_capacity_value");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_value > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_value] should be less than"
            " or equal to 100. Incorrect value %u received.", p_x2ap_rrm_rsu_composite_available_capacity->cell_capacity_value);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_cacg
(
    x2ap_rrm_rsu_cacg_t *p_x2ap_rrm_rsu_cacg,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_cacg, 0, sizeof(x2ap_rrm_rsu_cacg_t));

    /* This function parses x2ap_rrm_rsu_cacg */

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_rsu_composite_available_capacity(
        &p_x2ap_rrm_rsu_cacg->dl_composite_available_capacity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_rsu_composite_available_capacity(
        &p_x2ap_rrm_rsu_cacg->ul_composite_available_capacity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_usable_abs_information_fdd
(
    x2ap_usable_abs_information_fdd_t *p_x2ap_usable_abs_information_fdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_usable_abs_information_fdd, 0, sizeof(x2ap_usable_abs_information_fdd_t));

    /* This function parses x2ap_usable_abs_information_fdd */

    if (*p_length_read + (S32)sizeof(p_x2ap_usable_abs_information_fdd->usable_abs_pattern_fdd_info) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_usable_abs_information_fdd->usable_abs_pattern_fdd_info); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_usable_abs_information_fdd->usable_abs_pattern_fdd_info[loop], (void*)(p_src + *p_length_read), "usable_abs_pattern_fdd_info[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_usable_abs_information_tdd
(
    x2ap_usable_abs_information_tdd_t *p_x2ap_usable_abs_information_tdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_usable_abs_information_tdd, 0, sizeof(x2ap_usable_abs_information_tdd_t));

    /* This function parses x2ap_usable_abs_information_tdd */

    if (*p_length_read + (S32)sizeof(p_x2ap_usable_abs_information_tdd->usable_abs_pattern_tdd_info) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_usable_abs_information_tdd->usable_abs_pattern_tdd_info); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_usable_abs_information_tdd->usable_abs_pattern_tdd_info[loop], (void*)(p_src + *p_length_read), "usable_abs_pattern_tdd_info[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_usable_abs_information
(
    x2ap_usable_abs_information_t *p_x2ap_usable_abs_information,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_usable_abs_information, 0, sizeof(x2ap_usable_abs_information_t));

    /* This function parses x2ap_usable_abs_information */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_usable_abs_information->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2ap_usable_abs_information->bitmask & X2AP_USABLE_ABS_INFORMATION_FDD_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_usable_abs_information_fdd(
        &p_x2ap_usable_abs_information->usable_abs_info_fdd,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_usable_abs_information->bitmask & X2AP_USABLE_ABS_INFORMATION_TDD_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_usable_abs_information_tdd(
        &p_x2ap_usable_abs_information->usable_abs_info_tdd,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_abs_status
(
    x2ap_rrm_abs_status_t *p_x2ap_rrm_abs_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_abs_status, 0, sizeof(x2ap_rrm_abs_status_t));

    /* This function parses x2ap_rrm_abs_status */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_abs_status->dl_abs_status, p_src + *p_length_read, "dl_abs_status");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_abs_status->dl_abs_status > 100))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_abs_status->dl_abs_status] should be less than"
            " or equal to 100. Incorrect value %u received.", p_x2ap_rrm_abs_status->dl_abs_status);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_usable_abs_information(
        &p_x2ap_rrm_abs_status->x2ap_usable_abs_information,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_info
(
    x2ap_rrm_rsu_info_t *p_x2ap_rrm_rsu_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_info, 0, sizeof(x2ap_rrm_rsu_info_t));

    /* This function parses x2ap_rrm_rsu_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_rsu_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_x2ap_rrm_rsu_info->bitmask & PRB_PERIODIC_REPORT_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_rsu_prb(
        &p_x2ap_rrm_rsu_info->radio_resource_status,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_rsu_info->bitmask & TNL_LOAD_IND_PERIODIC_REPORT_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_rsu_tnl_load(
        &p_x2ap_rrm_rsu_info->s1_tnl_load_indicator,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_rsu_info->bitmask & LOAD_IND_PERIODIC_REPORT_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_rsu_hw_load(
        &p_x2ap_rrm_rsu_info->hardware_load_indicator,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_rsu_info->bitmask & COMPOSITE_AVAILABLE_CAPACITY_PERIODIC_REPORT_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_rsu_cacg(
        &p_x2ap_rrm_rsu_info->composite_available_capacity_group,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_rsu_info->bitmask & ABS_STATUS_PERIODIC_REPORT_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_abs_status(
        &p_x2ap_rrm_rsu_info->abs_status,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_report_info
(
    x2ap_rrm_rsu_report_info_t *p_x2ap_rrm_rsu_report_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_report_info, 0, sizeof(x2ap_rrm_rsu_report_info_t));

    /* This function parses x2ap_rrm_rsu_report_info */

    if (RRC_FAILURE == rrc_il_parse_rrm_cell_global_id_eutra(
        &p_x2ap_rrm_rsu_report_info->cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_rsu_info(
        &p_x2ap_rrm_rsu_report_info->rsu_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_rrm_stop_req
(
    x2ap_rrm_rsu_rrm_stop_req_t *p_x2ap_rrm_rsu_rrm_stop_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_rrm_stop_req, 0, sizeof(x2ap_rrm_rsu_rrm_stop_req_t));

    /* This function parses x2ap_rrm_rsu_rrm_stop_req */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_rsu_rrm_stop_req->enb_meas_id, p_src + *p_length_read, "enb_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_rrm_stop_req->enb_meas_id < 1) || (p_x2ap_rrm_rsu_rrm_stop_req->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_rrm_stop_req->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_rrm_stop_req->enb_meas_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rrm_rsu_rrm_stop_req->peer_gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_enb_stop_req
(
    x2ap_rrm_rsu_enb_stop_req_t *p_x2ap_rrm_rsu_enb_stop_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_enb_stop_req, 0, sizeof(x2ap_rrm_rsu_enb_stop_req_t));

    /* This function parses x2ap_rrm_rsu_enb_stop_req */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id, p_src + *p_length_read, "enb_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id < 1) || (p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rrm_rsu_enb_stop_req->peer_gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_enb_update_ind
(
    x2ap_rrm_rsu_enb_update_ind_t *p_x2ap_rrm_rsu_enb_update_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_rsu_enb_update_ind, 0, sizeof(x2ap_rrm_rsu_enb_update_ind_t));

    /* This function parses x2ap_rrm_rsu_enb_update_ind */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_rsu_enb_update_ind->enb_meas_id, p_src + *p_length_read, "enb_meas_id");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_rsu_enb_update_ind->enb_meas_id < 1) || (p_x2ap_rrm_rsu_enb_update_ind->enb_meas_id > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_update_ind->enb_meas_id] should be in range "
            "1 to 4095. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_update_ind->enb_meas_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rrm_rsu_enb_update_ind->peer_gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_rsu_enb_update_ind->num_reports, p_src + *p_length_read, "num_reports");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_rsu_enb_update_ind->num_reports > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_rsu_enb_update_ind->num_reports] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_rsu_enb_update_ind->num_reports);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_rsu_enb_update_ind->num_reports; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_rsu_report_info(
                &p_x2ap_rrm_rsu_enb_update_ind->rsu_report[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_load_information_req
(
    x2ap_rrm_load_information_req_t *p_x2ap_rrm_load_information_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_load_information_req, 0, sizeof(x2ap_rrm_load_information_req_t));

    /* This function parses x2ap_rrm_load_information_req */

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rrm_load_information_req->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_load_information_req->cell_information_count, p_src + *p_length_read, "cell_information_count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_load_information_req->cell_information_count > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_load_information_req->cell_information_count] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_load_information_req->cell_information_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_load_information_req->cell_information_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_li_cell_info_item_arr(
                &p_x2ap_rrm_load_information_req->cell_information[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_ul_intf_overload_ind
(
    x2ap_rrm_li_ul_intf_overload_ind_t *p_x2ap_rrm_li_ul_intf_overload_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_li_ul_intf_overload_ind, 0, sizeof(x2ap_rrm_li_ul_intf_overload_ind_t));

    /* This function parses x2ap_rrm_li_ul_intf_overload_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_li_ul_intf_overload_ind->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_ul_intf_overload_ind->count > X2AP_LI_MAX_PRBS_COUNT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_ul_intf_overload_ind->count] should be less than"
            " or equal to X2AP_LI_MAX_PRBS_COUNT. Incorrect value %u received.", p_x2ap_rrm_li_ul_intf_overload_ind->count);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_x2ap_rrm_li_ul_intf_overload_ind->count * sizeof(p_x2ap_rrm_li_ul_intf_overload_ind->ul_intf_overload_indication[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements with HIGHER boundary
     * range check */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_li_ul_intf_overload_ind->count; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_rrm_li_ul_intf_overload_ind->ul_intf_overload_indication[loop], (void*)(p_src + *p_length_read), "ul_intf_overload_indication[]");
            if (p_x2ap_rrm_li_ul_intf_overload_ind->ul_intf_overload_indication[loop] > 2)
            {
                RRC_TRACE(RRC_WARNING, "Parameter p_x2ap_rrm_li_ul_intf_overload_ind->ul_intf_overload_indication[%u] should be less"
                    " than or equal to 2. "
                    "Incorrect value %d received.", loop, p_x2ap_rrm_li_ul_intf_overload_ind->ul_intf_overload_indication[loop]);
                return RRC_FAILURE;
            }
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_ul_high_intf_ind
(
    x2ap_rrm_li_ul_high_intf_ind_t *p_x2ap_rrm_li_ul_high_intf_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_li_ul_high_intf_ind, 0, sizeof(x2ap_rrm_li_ul_high_intf_ind_t));

    /* This function parses x2ap_rrm_li_ul_high_intf_ind */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_li_ul_high_intf_ind->prb_count, p_src + *p_length_read, "prb_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_li_ul_high_intf_ind->prb_count < 1) || (p_x2ap_rrm_li_ul_high_intf_ind->prb_count > X2AP_RRM_LI_MAX_HII_PRB_OCTETS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_ul_high_intf_ind->prb_count] should be in range "
            "1 to X2AP_RRM_LI_MAX_HII_PRB_OCTETS. Incorrect value %u received.", p_x2ap_rrm_li_ul_high_intf_ind->prb_count);
        return RRC_FAILURE;
    }
  {
    rrc_counter_t bit_string_octet_count = (((p_x2ap_rrm_li_ul_high_intf_ind->prb_count)%8?(((p_x2ap_rrm_li_ul_high_intf_ind->prb_count)/8)+1):((p_x2ap_rrm_li_ul_high_intf_ind->prb_count)/8)));
    if (*p_length_read + (S32)(bit_string_octet_count) > length_left)
    {
         RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
         return RRC_FAILURE;
    }

    /* Parse BIT_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < bit_string_octet_count; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_rrm_li_ul_high_intf_ind->hii[loop], (void*)(p_src + *p_length_read), "hii[]");
            *p_length_read += sizeof(U8);
        }
    }
 }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_ul_high_intf_info_arr
(
    x2ap_rrm_li_ul_high_intf_info_arr_t *p_x2ap_rrm_li_ul_high_intf_info_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_li_ul_high_intf_info_arr, 0, sizeof(x2ap_rrm_li_ul_high_intf_info_arr_t));

    /* This function parses x2ap_rrm_li_ul_high_intf_info_arr */

    if (RRC_FAILURE == rrc_il_parse_rrm_cell_global_id_eutra(
        &p_x2ap_rrm_li_ul_high_intf_info_arr->target_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_li_ul_high_intf_ind(
        &p_x2ap_rrm_li_ul_high_intf_info_arr->ul_high_intf_ind,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_ul_high_interference_info
(
    x2ap_rrm_li_ul_high_interference_info_t *p_x2ap_rrm_li_ul_high_interference_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_li_ul_high_interference_info, 0, sizeof(x2ap_rrm_li_ul_high_interference_info_t));

    /* This function parses x2ap_rrm_li_ul_high_interference_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count, p_src + *p_length_read, "ul_high_interference_info_count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_li_ul_high_interference_info->ul_high_interference_info_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_li_ul_high_intf_info_arr(
                &p_x2ap_rrm_li_ul_high_interference_info->ul_high_intf_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_relative_narrowband_tx_power
(
    x2ap_rrm_li_relative_narrowband_tx_power_t *p_x2ap_rrm_li_relative_narrowband_tx_power,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_li_relative_narrowband_tx_power, 0, sizeof(x2ap_rrm_li_relative_narrowband_tx_power_t));

    /* This function parses x2ap_rrm_li_relative_narrowband_tx_power */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count, p_src + *p_length_read, "prb_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count < 6) || (p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count > X2AP_RRM_LI_MAX_HII_PRB_OCTETS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count] should be in range "
            "6 to X2AP_RRM_LI_MAX_HII_PRB_OCTETS. Incorrect value %u received.", p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count);
        return RRC_FAILURE;
    }
  {
    rrc_counter_t bit_string_octet_count = (((p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count)%8?(((p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count)/8)+1):((p_x2ap_rrm_li_relative_narrowband_tx_power->prb_count)/8)));
    if (*p_length_read + (S32)(bit_string_octet_count) > length_left)
    {
         RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
         return RRC_FAILURE;
    }

    /* Parse BIT_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < bit_string_octet_count; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_rrm_li_relative_narrowband_tx_power->rntp_per_prb[loop], (void*)(p_src + *p_length_read), "rntp_per_prb[]");
            *p_length_read += sizeof(U8);
        }
    }
 }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_li_relative_narrowband_tx_power->rntp_threshold, p_src + *p_length_read, "rntp_threshold");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_relative_narrowband_tx_power->rntp_threshold > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_relative_narrowband_tx_power->rntp_threshold] should be less than"
            " or equal to 15. Incorrect value %u received.", p_x2ap_rrm_li_relative_narrowband_tx_power->rntp_threshold);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_li_relative_narrowband_tx_power->num_cell_specific_antena_port, p_src + *p_length_read, "num_cell_specific_antena_port");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_relative_narrowband_tx_power->num_cell_specific_antena_port > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_relative_narrowband_tx_power->num_cell_specific_antena_port] should be less than"
            " or equal to 2. Incorrect value %u received.", p_x2ap_rrm_li_relative_narrowband_tx_power->num_cell_specific_antena_port);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_li_relative_narrowband_tx_power->p_b, p_src + *p_length_read, "p_b");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_relative_narrowband_tx_power->p_b > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_relative_narrowband_tx_power->p_b] should be less than"
            " or equal to 3. Incorrect value %u received.", p_x2ap_rrm_li_relative_narrowband_tx_power->p_b);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_li_relative_narrowband_tx_power->pdcch_interference_impact, p_src + *p_length_read, "pdcch_interference_impact");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_relative_narrowband_tx_power->pdcch_interference_impact > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_relative_narrowband_tx_power->pdcch_interference_impact] should be less than"
            " or equal to 4. Incorrect value %u received.", p_x2ap_rrm_li_relative_narrowband_tx_power->pdcch_interference_impact);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_abs_information_fdd
(
    x2ap_abs_information_fdd_t *p_x2ap_abs_information_fdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_abs_information_fdd, 0, sizeof(x2ap_abs_information_fdd_t));

    /* This function parses x2ap_abs_information_fdd */

    if (*p_length_read + (S32)sizeof(p_x2ap_abs_information_fdd->abs_pattern_info) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_abs_information_fdd->abs_pattern_info); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_abs_information_fdd->abs_pattern_info[loop], (void*)(p_src + *p_length_read), "abs_pattern_info[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_abs_information_fdd->no_cell_specific_antenna_port, p_src + *p_length_read, "no_cell_specific_antenna_port");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_abs_information_fdd->no_cell_specific_antenna_port > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_abs_information_fdd->no_cell_specific_antenna_port] should be less than"
            " or equal to 2. Incorrect value %u received.", p_x2ap_abs_information_fdd->no_cell_specific_antenna_port);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_x2ap_abs_information_fdd->measurement_subset) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_abs_information_fdd->measurement_subset); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_abs_information_fdd->measurement_subset[loop], (void*)(p_src + *p_length_read), "measurement_subset[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_abs_information_tdd
(
    x2ap_abs_information_tdd_t *p_x2ap_abs_information_tdd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_abs_information_tdd, 0, sizeof(x2ap_abs_information_tdd_t));

    /* This function parses x2ap_abs_information_tdd */

    if (*p_length_read + (S32)sizeof(p_x2ap_abs_information_tdd->abs_pattern_info) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_abs_information_tdd->abs_pattern_info); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_abs_information_tdd->abs_pattern_info[loop], (void*)(p_src + *p_length_read), "abs_pattern_info[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_abs_information_tdd->no_cell_specific_antenna_port, p_src + *p_length_read, "no_cell_specific_antenna_port");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_abs_information_tdd->no_cell_specific_antenna_port > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_abs_information_tdd->no_cell_specific_antenna_port] should be less than"
            " or equal to 2. Incorrect value %u received.", p_x2ap_abs_information_tdd->no_cell_specific_antenna_port);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_x2ap_abs_information_tdd->measurement_subset) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_abs_information_tdd->measurement_subset); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_abs_information_tdd->measurement_subset[loop], (void*)(p_src + *p_length_read), "measurement_subset[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_abs_information
(
    x2ap_rrm_li_abs_information_t *p_x2ap_rrm_li_abs_information,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_li_abs_information, 0, sizeof(x2ap_rrm_li_abs_information_t));

    /* This function parses x2ap_rrm_li_abs_information */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_li_abs_information->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2ap_rrm_li_abs_information->bitmask & X2AP_LI_ABS_INFORMATION_FDD_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_abs_information_fdd(
        &p_x2ap_rrm_li_abs_information->abs_info_fdd,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_li_abs_information->bitmask & X2AP_LI_ABS_INFORMATION_TDD_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_abs_information_tdd(
        &p_x2ap_rrm_li_abs_information->abs_info_tdd,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_invoke_indication
(
    x2ap_rrm_li_invoke_indication_t *p_x2ap_rrm_li_invoke_indication,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_li_invoke_indication, 0, sizeof(x2ap_rrm_li_invoke_indication_t));

    /* This function parses x2ap_rrm_li_invoke_indication */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_rrm_li_invoke_indication->invoke_ind, p_src + *p_length_read, "invoke_ind");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_li_invoke_indication->invoke_ind > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_li_invoke_indication->invoke_ind] should be less than"
            " or equal to 0. Incorrect value %u received.", p_x2ap_rrm_li_invoke_indication->invoke_ind);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_x2ap_rrm_li_cell_info_item_arr
(
    x2ap_rrm_li_cell_info_item_arr_t *p_x2ap_rrm_li_cell_info_item_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_li_cell_info_item_arr, 0, sizeof(x2ap_rrm_li_cell_info_item_arr_t));

    /* This function parses x2ap_rrm_li_cell_info_item_arr */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_li_cell_info_item_arr->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_rrm_cell_global_id_eutra(
        &p_x2ap_rrm_li_cell_info_item_arr->source_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_CELL_INFO_ITEM_UL_INTF_OVERLOAD_IND_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_li_ul_intf_overload_ind(
        &p_x2ap_rrm_li_cell_info_item_arr->ul_intf_overload_ind,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_CELL_INFO_ITEM_UL_INTF_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_li_ul_high_interference_info(
        &p_x2ap_rrm_li_cell_info_item_arr->ul_high_interference_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_CELL_INFO_ITEM_RELATIVE_NARROWBAND_TX_POWER_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_li_relative_narrowband_tx_power(
        &p_x2ap_rrm_li_cell_info_item_arr->relative_narrowband_tx_power,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_ABS_INFORMATION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_li_abs_information(
        &p_x2ap_rrm_li_cell_info_item_arr->abs_infomation,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rrm_li_cell_info_item_arr->bitmask & X2AP_LI_INVOKE_INDICATION_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_li_invoke_indication(
        &p_x2ap_rrm_li_cell_info_item_arr->invoke_indication,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_load_information_resp
(
    x2ap_rrm_load_information_resp_t *p_x2ap_rrm_load_information_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_load_information_resp, 0, sizeof(x2ap_rrm_load_information_resp_t));

    /* This function parses x2ap_rrm_load_information_resp */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rrm_load_information_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rrm_load_information_ind
(
    x2ap_rrm_load_information_ind_t *p_x2ap_rrm_load_information_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_load_information_ind, 0, sizeof(x2ap_rrm_load_information_ind_t));

    /* This function parses x2ap_rrm_load_information_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_load_information_ind->cell_information_count, p_src + *p_length_read, "cell_information_count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rrm_load_information_ind->cell_information_count > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_load_information_ind->cell_information_count] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2ap_rrm_load_information_ind->cell_information_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_rrm_load_information_ind->cell_information_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_li_cell_info_item_arr(
                &p_x2ap_rrm_load_information_ind->cell_load_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_rlf_ind
(
    x2ap_rlf_ind_t *p_x2ap_rlf_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rlf_ind, 0, sizeof(x2ap_rlf_ind_t));

    /* This function parses x2ap_rlf_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rlf_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rlf_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_rlf_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rlf_ind->failure_cell_pci, p_src + *p_length_read, "failure_cell_pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rlf_ind->failure_cell_pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rlf_ind->failure_cell_pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_x2ap_rlf_ind->failure_cell_pci);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_ecgi(
        &p_x2ap_rlf_ind->re_est_cell_ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rlf_ind->crnti, p_src + *p_length_read, "crnti");
    *p_length_read += sizeof(U16);

    if (p_x2ap_rlf_ind->bitmask & X2AP_RLF_IND_RLF_REPORT_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_rlf_report(
        &p_x2ap_rlf_ind->rlf_report,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_rlf_ind->bitmask & X2AP_RLF_IND_UE_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rlf_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);
    }

    if (p_x2ap_rlf_ind->bitmask & X2AP_RLF_IND_UE_CONTEXT_RELEASED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rlf_ind->ue_context_released, p_src + *p_length_read, "ue_context_released");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rlf_ind->ue_context_released > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rlf_ind->ue_context_released] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_rlf_ind->ue_context_released);
        return RRC_FAILURE;
    }
    }

    if (p_x2ap_rlf_ind->bitmask & X2AP_RLF_IND_CONN_SETUP_IND_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_rlf_ind->conn_setup_indicator, p_src + *p_length_read, "conn_setup_indicator");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_rlf_ind->conn_setup_indicator > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rlf_ind->conn_setup_indicator] should be less than"
            " or equal to 0. Incorrect value %u received.", p_x2ap_rlf_ind->conn_setup_indicator);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_rlf_report_meas_result_serv_cell
(
    rrc_rlf_report_meas_result_serv_cell_t *p_rrc_rlf_report_meas_result_serv_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlf_report_meas_result_serv_cell, 0, sizeof(rrc_rlf_report_meas_result_serv_cell_t));

    /* This function parses rrc_rlf_report_meas_result_serv_cell */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlf_report_meas_result_serv_cell->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlf_report_meas_result_serv_cell->rsrp_result, p_src + *p_length_read, "rsrp_result");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlf_report_meas_result_serv_cell->rsrp_result > 97))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlf_report_meas_result_serv_cell->rsrp_result] should be less than"
            " or equal to 97. Incorrect value %u received.", p_rrc_rlf_report_meas_result_serv_cell->rsrp_result);
        return RRC_FAILURE;
    }

    if (p_rrc_rlf_report_meas_result_serv_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_RSRQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlf_report_meas_result_serv_cell->rsrq_result, p_src + *p_length_read, "rsrq_result");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlf_report_meas_result_serv_cell->rsrq_result > 34))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlf_report_meas_result_serv_cell->rsrq_result] should be less than"
            " or equal to 34. Incorrect value %u received.", p_rrc_rlf_report_meas_result_serv_cell->rsrq_result);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_cell_global_id_eutra
(
    cell_global_id_eutra_t *p_cell_global_id_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_cell_global_id_eutra, 0, sizeof(cell_global_id_eutra_t));

    /* This function parses cell_global_id_eutra */

    if (RRC_FAILURE == rrc_il_parse_plmn_identity(
        &p_cell_global_id_eutra->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_cell_global_id_eutra->cell_identity) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_eutra->cell_identity); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_cell_global_id_eutra->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_plmn_identity_list2
(
    plmn_identity_list2_t *p_plmn_identity_list2,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_plmn_identity_list2, 0, sizeof(plmn_identity_list2_t));

    /* This function parses plmn_identity_list2 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_plmn_identity_list2->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_plmn_identity_list2->count > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_plmn_identity_list2->count] should be less than"
            " or equal to 5. Incorrect value %u received.", p_plmn_identity_list2->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_plmn_identity_list2->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_plmn_identity(
                &p_plmn_identity_list2->cell_identity[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_eutra_cgi_info
(
    meas_result_eutra_cgi_info_t *p_meas_result_eutra_cgi_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_result_eutra_cgi_info, 0, sizeof(meas_result_eutra_cgi_info_t));

    /* This function parses meas_result_eutra_cgi_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_eutra_cgi_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_cell_global_id_eutra(
        &p_meas_result_eutra_cgi_info->cell_global_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_meas_result_eutra_cgi_info->tracking_area_code) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_meas_result_eutra_cgi_info->tracking_area_code); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_meas_result_eutra_cgi_info->tracking_area_code[loop], (void*)(p_src + *p_length_read), "tracking_area_code[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (p_meas_result_eutra_cgi_info->bitmask & MEAS_RESULT_EUTRA_CGI_INFO_PLMN_IDENTITY_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_plmn_identity_list2(
        &p_meas_result_eutra_cgi_info->plmn_identity_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_eutra_meas_result
(
    meas_result_eutra_meas_result_t *p_meas_result_eutra_meas_result,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_meas_result_eutra_meas_result, 0, sizeof(meas_result_eutra_meas_result_t));

    /* This function parses meas_result_eutra_meas_result */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_eutra_meas_result->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_meas_result_eutra_meas_result->bitmask & MEAS_RESULT_EUTRA_MEAS_RESULT_RSRP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_eutra_meas_result->rsrp_result, p_src + *p_length_read, "rsrp_result");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_eutra_meas_result->rsrp_result > 97))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_eutra_meas_result->rsrp_result] should be less than"
            " or equal to 97. Incorrect value %u received.", p_meas_result_eutra_meas_result->rsrp_result);
        return RRC_FAILURE;
    }
    }

    if (p_meas_result_eutra_meas_result->bitmask & MEAS_RESULT_EUTRA_MEAS_RESULT_RSRQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_eutra_meas_result->rsrq_result, p_src + *p_length_read, "rsrq_result");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_eutra_meas_result->rsrq_result > 34))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_eutra_meas_result->rsrq_result] should be less than"
            " or equal to 34. Incorrect value %u received.", p_meas_result_eutra_meas_result->rsrq_result);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_csg_info
(
    meas_result_csg_info_t *p_meas_result_csg_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_meas_result_csg_info, 0, sizeof(meas_result_csg_info_t));

    /* This function parses meas_result_csg_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_csg_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_meas_result_csg_info->bitmask & MEAS_RESULT_EUTRA_MEAS_RESULT_CSG_MEM_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_csg_info->csg_membershipStatus, p_src + *p_length_read, "csg_membershipStatus");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_csg_info->csg_membershipStatus > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_csg_info->csg_membershipStatus] should be less than"
            " or equal to 0. Incorrect value %u received.", p_meas_result_csg_info->csg_membershipStatus);
        return RRC_FAILURE;
    }
    }

    if (p_meas_result_csg_info->bitmask & MEAS_RESULT_EUTRA_MEAS_RESULT_CSG_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_meas_result_csg_info->csg_identity) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_meas_result_csg_info->csg_identity); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_meas_result_csg_info->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_eutra
(
    meas_result_eutra_t *p_meas_result_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_result_eutra, 0, sizeof(meas_result_eutra_t));

    /* This function parses meas_result_eutra */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_eutra->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_eutra->phys_cell_id, p_src + *p_length_read, "phys_cell_id");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_eutra->phys_cell_id > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_eutra->phys_cell_id] should be less than"
            " or equal to 503. Incorrect value %u received.", p_meas_result_eutra->phys_cell_id);
        return RRC_FAILURE;
    }

    if (p_meas_result_eutra->bitmask & MEAS_RESULT_EUTRA_CGI_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_meas_result_eutra_cgi_info(
        &p_meas_result_eutra->cgi_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (RRC_FAILURE == rrc_il_parse_meas_result_eutra_meas_result(
        &p_meas_result_eutra->meas_result,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_meas_result_eutra->bitmask & MEAS_RESULT_EUTRA_CSG_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_meas_result_csg_info(
        &p_meas_result_eutra->eutra_csg_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_list_eutra
(
    meas_result_list_eutra_t *p_meas_result_list_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_result_list_eutra, 0, sizeof(meas_result_list_eutra_t));

    /* This function parses meas_result_list_eutra */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_list_eutra->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_list_eutra->count > MAX_CELL_REPORT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list_eutra->count] should be less than"
            " or equal to MAX_CELL_REPORT. Incorrect value %u received.", p_meas_result_list_eutra->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list_eutra->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_meas_result_eutra(
                &p_meas_result_list_eutra->meas_result_eutra[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_meas_result2_eutra
(
    rrc_meas_result2_eutra_t *p_rrc_meas_result2_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_meas_result2_eutra, 0, sizeof(rrc_meas_result2_eutra_t));

    /* This function parses rrc_meas_result2_eutra */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_meas_result2_eutra->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_meas_result2_eutra->carrier_freq, p_src + *p_length_read, "carrier_freq");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_meas_result_list_eutra(
        &p_rrc_meas_result2_eutra->meas_result_list_eutra,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_meas_result_list2_eutra
(
    rrc_meas_result_list2_eutra_t *p_rrc_meas_result_list2_eutra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_meas_result_list2_eutra, 0, sizeof(rrc_meas_result_list2_eutra_t));

    /* This function parses rrc_meas_result_list2_eutra */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_meas_result_list2_eutra->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_meas_result_list2_eutra->count < 1) || (p_rrc_meas_result_list2_eutra->count > MAX_FREQ))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_meas_result_list2_eutra->count] should be in range "
            "1 to MAX_FREQ. Incorrect value %u received.", p_rrc_meas_result_list2_eutra->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_meas_result_list2_eutra->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_rrc_meas_result2_eutra(
                &p_rrc_meas_result_list2_eutra->meas_result_eutra[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_utra_phys_cell_id
(
    meas_result_utra_phys_cell_id_t *p_meas_result_utra_phys_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_meas_result_utra_phys_cell_id, 0, sizeof(meas_result_utra_phys_cell_id_t));

    /* This function parses meas_result_utra_phys_cell_id */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_utra_phys_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_meas_result_utra_phys_cell_id->bitmask & MEAS_RESULT_UTRA_PHYS_CELL_ID_FDD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_utra_phys_cell_id->fdd, p_src + *p_length_read, "fdd");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_utra_phys_cell_id->fdd > 511))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_utra_phys_cell_id->fdd] should be less than"
            " or equal to 511. Incorrect value %u received.", p_meas_result_utra_phys_cell_id->fdd);
        return RRC_FAILURE;
    }
    }

    if (p_meas_result_utra_phys_cell_id->bitmask & MEAS_RESULT_UTRA_PHYS_CELL_ID_TDD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_utra_phys_cell_id->tdd, p_src + *p_length_read, "tdd");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_utra_phys_cell_id->tdd > 127))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_utra_phys_cell_id->tdd] should be less than"
            " or equal to 127. Incorrect value %u received.", p_meas_result_utra_phys_cell_id->tdd);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_cell_global_id_utra
(
    cell_global_id_utra_t *p_cell_global_id_utra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_cell_global_id_utra, 0, sizeof(cell_global_id_utra_t));

    /* This function parses cell_global_id_utra */

    if (RRC_FAILURE == rrc_il_parse_plmn_identity(
        &p_cell_global_id_utra->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_cell_global_id_utra->cell_identity) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_utra->cell_identity); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_cell_global_id_utra->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_utra_cgi_info
(
    meas_result_utra_cgi_info_t *p_meas_result_utra_cgi_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_result_utra_cgi_info, 0, sizeof(meas_result_utra_cgi_info_t));

    /* This function parses meas_result_utra_cgi_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_utra_cgi_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_cell_global_id_utra(
        &p_meas_result_utra_cgi_info->cell_global_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_meas_result_utra_cgi_info->bitmask & MEAS_RESULT_UTRA_CGI_INFO_LOCATION_AREA_CODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_meas_result_utra_cgi_info->location_area_code) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_meas_result_utra_cgi_info->location_area_code); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_meas_result_utra_cgi_info->location_area_code[loop], (void*)(p_src + *p_length_read), "location_area_code[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_meas_result_utra_cgi_info->bitmask & MEAS_RESULT_UTRA_CGI_INFO_ROUTING_AREA_CODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_utra_cgi_info->routing_area_code, p_src + *p_length_read, "routing_area_code");
    *p_length_read += sizeof(U8);
    }

    if (p_meas_result_utra_cgi_info->bitmask & MEAS_RESULT_UTRA_CGI_INFO_PLMN_IDENTITY_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_plmn_identity_list2(
        &p_meas_result_utra_cgi_info->plmn_identity_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_utra_meas_result
(
    meas_result_utra_meas_result_t *p_meas_result_utra_meas_result,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_meas_result_utra_meas_result, 0, sizeof(meas_result_utra_meas_result_t));

    /* This function parses meas_result_utra_meas_result */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_utra_meas_result->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_meas_result_utra_meas_result->bitmask & MEAS_RESULT_UTRA_MEAS_RESULT_RSCP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_meas_result_utra_meas_result->utra_rscp, p_src + *p_length_read, "utra_rscp");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_meas_result_utra_meas_result->utra_rscp < -5) || (p_meas_result_utra_meas_result->utra_rscp > 91))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_utra_meas_result->utra_rscp] should be in range "
            "-5 to 91. Incorrect value %d received.", p_meas_result_utra_meas_result->utra_rscp);
        return RRC_FAILURE;
    }
    }

    if (p_meas_result_utra_meas_result->bitmask & MEAS_RESULT_UTRA_MEAS_RESULT_ECN0_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_utra_meas_result->utra_ecn0, p_src + *p_length_read, "utra_ecn0");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_utra_meas_result->utra_ecn0 > 49))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_utra_meas_result->utra_ecn0] should be less than"
            " or equal to 49. Incorrect value %u received.", p_meas_result_utra_meas_result->utra_ecn0);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_utra
(
    meas_result_utra_t *p_meas_result_utra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_result_utra, 0, sizeof(meas_result_utra_t));

    /* This function parses meas_result_utra */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_utra->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_meas_result_utra_phys_cell_id(
        &p_meas_result_utra->phys_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_meas_result_utra->bitmask & MEAS_RESULT_UTRA_CGI_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_meas_result_utra_cgi_info(
        &p_meas_result_utra->cgi_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_meas_result_utra->bitmask & MEAS_RESULT_UTRA_CSG_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_meas_result_csg_info(
        &p_meas_result_utra->utra_csg_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (RRC_FAILURE == rrc_il_parse_meas_result_utra_meas_result(
        &p_meas_result_utra->meas_result,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_list_utra
(
    meas_result_list_utra_t *p_meas_result_list_utra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_result_list_utra, 0, sizeof(meas_result_list_utra_t));

    /* This function parses meas_result_list_utra */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_list_utra->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_list_utra->count > MAX_CELL_REPORT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list_utra->count] should be less than"
            " or equal to MAX_CELL_REPORT. Incorrect value %u received.", p_meas_result_list_utra->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list_utra->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_meas_result_utra(
                &p_meas_result_list_utra->meas_result_utra[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_meas_result2_utra
(
    rrc_meas_result2_utra_t *p_rrc_meas_result2_utra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_meas_result2_utra, 0, sizeof(rrc_meas_result2_utra_t));

    /* This function parses rrc_meas_result2_utra */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_meas_result2_utra->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_meas_result2_utra->carrier_freq, p_src + *p_length_read, "carrier_freq");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_meas_result2_utra->carrier_freq > 16383))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_meas_result2_utra->carrier_freq] should be less than"
            " or equal to 16383. Incorrect value %u received.", p_rrc_meas_result2_utra->carrier_freq);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_meas_result_list_utra(
        &p_rrc_meas_result2_utra->meas_result_list_utra,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_meas_result_list2_utra
(
    rrc_meas_result_list2_utra_t *p_rrc_meas_result_list2_utra,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_meas_result_list2_utra, 0, sizeof(rrc_meas_result_list2_utra_t));

    /* This function parses rrc_meas_result_list2_utra */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_meas_result_list2_utra->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_meas_result_list2_utra->count < 1) || (p_rrc_meas_result_list2_utra->count > MAX_FREQ))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_meas_result_list2_utra->count] should be in range "
            "1 to MAX_FREQ. Incorrect value %u received.", p_rrc_meas_result_list2_utra->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_meas_result_list2_utra->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_rrc_meas_result2_utra(
                &p_rrc_meas_result_list2_utra->meas_result_utra[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_carrier_freq_geran
(
    carrier_freq_geran_t *p_carrier_freq_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_carrier_freq_geran, 0, sizeof(carrier_freq_geran_t));

    /* This function parses carrier_freq_geran */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_carrier_freq_geran->arfcn, p_src + *p_length_read, "arfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_carrier_freq_geran->arfcn > 1023))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_carrier_freq_geran->arfcn] should be less than"
            " or equal to 1023. Incorrect value %u received.", p_carrier_freq_geran->arfcn);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_carrier_freq_geran->band_indicator, p_src + *p_length_read, "band_indicator");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_carrier_freq_geran->band_indicator > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_carrier_freq_geran->band_indicator] should be less than"
            " or equal to 1. Incorrect value %u received.", p_carrier_freq_geran->band_indicator);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_phys_cell_id_geran
(
    phys_cell_id_geran_t *p_phys_cell_id_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_phys_cell_id_geran, 0, sizeof(phys_cell_id_geran_t));

    /* This function parses phys_cell_id_geran */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_phys_cell_id_geran->network_colour_code, p_src + *p_length_read, "network_colour_code");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_phys_cell_id_geran->base_station_colour_code, p_src + *p_length_read, "base_station_colour_code");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_cell_global_id_geran
(
    cell_global_id_geran_t *p_cell_global_id_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_cell_global_id_geran, 0, sizeof(cell_global_id_geran_t));

    /* This function parses cell_global_id_geran */

    if (RRC_FAILURE == rrc_il_parse_plmn_identity(
        &p_cell_global_id_geran->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_cell_global_id_geran->location_area_code) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_geran->location_area_code); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_cell_global_id_geran->location_area_code[loop], (void*)(p_src + *p_length_read), "location_area_code[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(p_cell_global_id_geran->cell_identity) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_geran->cell_identity); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_cell_global_id_geran->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_geran_cgi_info
(
    meas_result_geran_cgi_info_t *p_meas_result_geran_cgi_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_result_geran_cgi_info, 0, sizeof(meas_result_geran_cgi_info_t));

    /* This function parses meas_result_geran_cgi_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_geran_cgi_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_cell_global_id_geran(
        &p_meas_result_geran_cgi_info->cell_global_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_meas_result_geran_cgi_info->bitmask & MEAS_RESULT_GERAN_CGI_INFO_ROUTING_AREA_CODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_geran_cgi_info->routing_area_code, p_src + *p_length_read, "routing_area_code");
    *p_length_read += sizeof(U8);
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_geran_meas_result
(
    meas_result_geran_meas_result_t *p_meas_result_geran_meas_result,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_meas_result_geran_meas_result, 0, sizeof(meas_result_geran_meas_result_t));

    /* This function parses meas_result_geran_meas_result */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_geran_meas_result->rssi, p_src + *p_length_read, "rssi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_geran_meas_result->rssi > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_geran_meas_result->rssi] should be less than"
            " or equal to 63. Incorrect value %u received.", p_meas_result_geran_meas_result->rssi);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_geran
(
    meas_result_geran_t *p_meas_result_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_result_geran, 0, sizeof(meas_result_geran_t));

    /* This function parses meas_result_geran */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_geran->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_carrier_freq_geran(
        &p_meas_result_geran->carrier_freq,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_phys_cell_id_geran(
        &p_meas_result_geran->phys_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_meas_result_geran->bitmask & MEAS_RESULT_GERAN_CGI_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_meas_result_geran_cgi_info(
        &p_meas_result_geran->cgi_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (RRC_FAILURE == rrc_il_parse_meas_result_geran_meas_result(
        &p_meas_result_geran->meas_result,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_list_geran
(
    meas_result_list_geran_t *p_meas_result_list_geran,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_result_list_geran, 0, sizeof(meas_result_list_geran_t));

    /* This function parses meas_result_list_geran */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_list_geran->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_list_geran->count > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list_geran->count] should be less than"
            " or equal to 8. Incorrect value %u received.", p_meas_result_list_geran->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list_geran->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_meas_result_geran(
                &p_meas_result_list_geran->meas_result_geran[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_carrier_freq_cdma2000
(
    carrier_freq_cdma2000_t *p_carrier_freq_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_carrier_freq_cdma2000, 0, sizeof(carrier_freq_cdma2000_t));

    /* This function parses carrier_freq_cdma2000 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_carrier_freq_cdma2000->band_class, p_src + *p_length_read, "band_class");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_carrier_freq_cdma2000->band_class > 31))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_carrier_freq_cdma2000->band_class] should be less than"
            " or equal to 31. Incorrect value %u received.", p_carrier_freq_cdma2000->band_class);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_carrier_freq_cdma2000->arfcn, p_src + *p_length_read, "arfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_carrier_freq_cdma2000->arfcn > 2047))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_carrier_freq_cdma2000->arfcn] should be less than"
            " or equal to 2047. Incorrect value %u received.", p_carrier_freq_cdma2000->arfcn);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_cell_global_id_cdma2000
(
    cell_global_id_cdma2000_t *p_cell_global_id_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_cell_global_id_cdma2000, 0, sizeof(cell_global_id_cdma2000_t));

    /* This function parses cell_global_id_cdma2000 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_cell_global_id_cdma2000->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_cell_global_id_cdma2000->bitmask & CELL_GLOBAL_ID_CDMA2000_1XRTT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_cell_global_id_cdma2000->cell_global_id_1xrtt) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_cdma2000->cell_global_id_1xrtt); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_cell_global_id_cdma2000->cell_global_id_1xrtt[loop], (void*)(p_src + *p_length_read), "cell_global_id_1xrtt[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_cell_global_id_cdma2000->bitmask & CELL_GLOBAL_ID_CDMA2000_HRPD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_cell_global_id_cdma2000->cell_global_id_hrpd) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_cell_global_id_cdma2000->cell_global_id_hrpd); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_cell_global_id_cdma2000->cell_global_id_hrpd[loop], (void*)(p_src + *p_length_read), "cell_global_id_hrpd[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_cdma2000_meas_result
(
    meas_result_cdma2000_meas_result_t *p_meas_result_cdma2000_meas_result,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_meas_result_cdma2000_meas_result, 0, sizeof(meas_result_cdma2000_meas_result_t));

    /* This function parses meas_result_cdma2000_meas_result */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_cdma2000_meas_result->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_meas_result_cdma2000_meas_result->bitmask & MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_cdma2000_meas_result->pilot_pn_phase, p_src + *p_length_read, "pilot_pn_phase");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_cdma2000_meas_result->pilot_pn_phase > 32767))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_cdma2000_meas_result->pilot_pn_phase] should be less than"
            " or equal to 32767. Incorrect value %u received.", p_meas_result_cdma2000_meas_result->pilot_pn_phase);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_cdma2000_meas_result->pilot_strength, p_src + *p_length_read, "pilot_strength");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_cdma2000_meas_result->pilot_strength > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_cdma2000_meas_result->pilot_strength] should be less than"
            " or equal to 63. Incorrect value %u received.", p_meas_result_cdma2000_meas_result->pilot_strength);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_cdma2000
(
    meas_result_cdma2000_t *p_meas_result_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_result_cdma2000, 0, sizeof(meas_result_cdma2000_t));

    /* This function parses meas_result_cdma2000 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_cdma2000->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_meas_result_cdma2000->phy_cell_id, p_src + *p_length_read, "phy_cell_id");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_cdma2000->phy_cell_id > 511))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_cdma2000->phy_cell_id] should be less than"
            " or equal to 511. Incorrect value %u received.", p_meas_result_cdma2000->phy_cell_id);
        return RRC_FAILURE;
    }

    if (p_meas_result_cdma2000->bitmask & MEAS_RESULT_CDMA2000_CGI_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_cell_global_id_cdma2000(
        &p_meas_result_cdma2000->cgi_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (RRC_FAILURE == rrc_il_parse_meas_result_cdma2000_meas_result(
        &p_meas_result_cdma2000->meas_result,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_list_cdma2000
(
    meas_result_list_cdma2000_t *p_meas_result_list_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_result_list_cdma2000, 0, sizeof(meas_result_list_cdma2000_t));

    /* This function parses meas_result_list_cdma2000 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_list_cdma2000->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_result_list_cdma2000->count > MAX_CELL_REPORT))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list_cdma2000->count] should be less than"
            " or equal to MAX_CELL_REPORT. Incorrect value %u received.", p_meas_result_list_cdma2000->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list_cdma2000->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_meas_result_cdma2000(
                &p_meas_result_list_cdma2000->meas_result_cdma2000[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_results_cdma2000
(
    meas_results_cdma2000_t *p_meas_results_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_results_cdma2000, 0, sizeof(meas_results_cdma2000_t));

    /* This function parses meas_results_cdma2000 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_results_cdma2000->pre_registration_status_hrpd, p_src + *p_length_read, "pre_registration_status_hrpd");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_meas_results_cdma2000->pre_registration_status_hrpd > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_results_cdma2000->pre_registration_status_hrpd] should be less than"
            " or equal to 1. Incorrect value %u received.", p_meas_results_cdma2000->pre_registration_status_hrpd);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_meas_result_list_cdma2000(
        &p_meas_results_cdma2000->meas_result_list_cdma2000,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_meas_result2_cdma2000
(
    rrc_meas_result2_cdma2000_t *p_rrc_meas_result2_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_meas_result2_cdma2000, 0, sizeof(rrc_meas_result2_cdma2000_t));

    /* This function parses rrc_meas_result2_cdma2000 */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_meas_result2_cdma2000->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_carrier_freq_cdma2000(
        &p_rrc_meas_result2_cdma2000->carrier_freq_cdma2000,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_meas_results_cdma2000(
        &p_rrc_meas_result2_cdma2000->meas_result_cdma2000,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_meas_result_list2_cdma2000
(
    meas_result_list2_cdma2000_t *p_meas_result_list2_cdma2000,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_meas_result_list2_cdma2000, 0, sizeof(meas_result_list2_cdma2000_t));

    /* This function parses meas_result_list2_cdma2000 */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_meas_result_list2_cdma2000->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_meas_result_list2_cdma2000->count < 1) || (p_meas_result_list2_cdma2000->count > MAX_FREQ))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_meas_result_list2_cdma2000->count] should be in range "
            "1 to MAX_FREQ. Incorrect value %u received.", p_meas_result_list2_cdma2000->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_meas_result_list2_cdma2000->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_rrc_meas_result2_cdma2000(
                &p_meas_result_list2_cdma2000->meas_result_cdma2000[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_rlf_report_meas_result_neigh_cell
(
    rrc_rlf_report_meas_result_neigh_cell_t *p_rrc_rlf_report_meas_result_neigh_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlf_report_meas_result_neigh_cell, 0, sizeof(rrc_rlf_report_meas_result_neigh_cell_t));

    /* This function parses rrc_rlf_report_meas_result_neigh_cell */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlf_report_meas_result_neigh_cell->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_rlf_report_meas_result_neigh_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_LIST_EUTRA_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_meas_result_list2_eutra(
        &p_rrc_rlf_report_meas_result_neigh_cell->meas_result_list_eutra,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rlf_report_meas_result_neigh_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_LIST_UTRA_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_meas_result_list2_utra(
        &p_rrc_rlf_report_meas_result_neigh_cell->meas_result_list_utra,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rlf_report_meas_result_neigh_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_LIST_GERAN_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_meas_result_list_geran(
        &p_rrc_rlf_report_meas_result_neigh_cell->meas_result_list_geran,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rlf_report_meas_result_neigh_cell->bitmask & RRC_UE_INFO_MEAS_RESULT_LIST_CDMA2000_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_meas_result_list2_cdma2000(
        &p_rrc_rlf_report_meas_result_neigh_cell->meas_result_list2_cdma2000,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_rlf_dynamic_string
(
    rrc_rlf_dynamic_string_t *p_rrc_rlf_dynamic_string,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlf_dynamic_string, 0, sizeof(rrc_rlf_dynamic_string_t));

    /* This function parses rrc_rlf_dynamic_string */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlf_dynamic_string->data_length, p_src + *p_length_read, "data_length");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlf_dynamic_string->data_length > 64))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlf_dynamic_string->data_length] should be less than"
            " or equal to 64. Incorrect value %u received.", p_rrc_rlf_dynamic_string->data_length);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_rrc_rlf_dynamic_string->data_length * sizeof(p_rrc_rlf_dynamic_string->data[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_rlf_dynamic_string->data_length; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_rlf_dynamic_string->data[loop], (void*)(p_src + *p_length_read), "data[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_rlf_report_location_coordinates
(
    rrc_rlf_report_location_coordinates_t *p_rrc_rlf_report_location_coordinates,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlf_report_location_coordinates, 0, sizeof(rrc_rlf_report_location_coordinates_t));

    /* This function parses rrc_rlf_report_location_coordinates */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlf_report_location_coordinates->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_rlf_report_location_coordinates->bitmask & RRC_RLF_LOCATION_CORD_ELLIPSOID_POINT_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_rlf_dynamic_string(
        &p_rrc_rlf_report_location_coordinates->ellipsoid_point,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rlf_report_location_coordinates->bitmask & RRC_RLF_LOCATION_CORD_ELLIPSOID_POINT_ALTITUDE_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_rlf_dynamic_string(
        &p_rrc_rlf_report_location_coordinates->ellipsoid_point_altitude,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_rlf_report_location_info
(
    rrc_rlf_report_location_info_t *p_rrc_rlf_report_location_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlf_report_location_info, 0, sizeof(rrc_rlf_report_location_info_t));

    /* This function parses rrc_rlf_report_location_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlf_report_location_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_rrc_rlf_report_location_coordinates(
        &p_rrc_rlf_report_location_info->location_coordiate,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrc_rlf_report_location_info->bitmask & RRC_RLF_LOCATION_INFO_HORIZON_VELOCITY_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_rlf_dynamic_string(
        &p_rrc_rlf_report_location_info->horizontal_velocity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rlf_report_location_info->bitmask & RRC_RLF_LOCATION_INFO_GNSS_TOD_MSEC_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_rlf_dynamic_string(
        &p_rrc_rlf_report_location_info->gnss_tod_msec,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_rlf_failed_pci_arfcn
(
    rrc_rlf_failed_pci_arfcn_t *p_rrc_rlf_failed_pci_arfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlf_failed_pci_arfcn, 0, sizeof(rrc_rlf_failed_pci_arfcn_t));

    /* This function parses rrc_rlf_failed_pci_arfcn */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlf_failed_pci_arfcn->phys_cell_id, p_src + *p_length_read, "phys_cell_id");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlf_failed_pci_arfcn->phys_cell_id > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlf_failed_pci_arfcn->phys_cell_id] should be less than"
            " or equal to 503. Incorrect value %u received.", p_rrc_rlf_failed_pci_arfcn->phys_cell_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlf_failed_pci_arfcn->carrier_freq, p_src + *p_length_read, "carrier_freq");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_rlf_failed_pcellid
(
    rrc_rlf_failed_pcellid_t *p_rrc_rlf_failed_pcellid,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlf_failed_pcellid, 0, sizeof(rrc_rlf_failed_pcellid_t));

    /* This function parses rrc_rlf_failed_pcellid */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlf_failed_pcellid->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_rrc_rlf_failed_pcellid->bitmask & RRC_RLF_FAILED_PCI_CELL_GLOBAL_ID_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrm_cell_global_id_eutra(
        &p_rrc_rlf_failed_pcellid->cell_global_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rlf_failed_pcellid->bitmask & RRC_RLF_FAILED_PCI_ARFCN_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_rlf_failed_pci_arfcn(
        &p_rrc_rlf_failed_pcellid->pci_arfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_rlf_report
(
    rrc_rlf_report_t *p_rrc_rlf_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlf_report, 0, sizeof(rrc_rlf_report_t));

    /* This function parses rrc_rlf_report */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlf_report->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_rrc_rlf_report_meas_result_serv_cell(
        &p_rrc_rlf_report->meas_result_serv_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrc_rlf_report->bitmask & RRC_UE_INFO_MEAS_RESULT_NEIGH_CELLS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_rlf_report_meas_result_neigh_cell(
        &p_rrc_rlf_report->meas_result_neigh_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rlf_report->bitmask & RRC_UE_INFO_REESTABLISMENT_CELLID_R10_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrm_cell_global_id_eutra(
        &p_rrc_rlf_report->reestablishment_cellid_r10,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rlf_report->bitmask & RRC_UE_INFO_TIME_CONN_FAILURE_R10_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlf_report->time_conn_failure_r10, p_src + *p_length_read, "time_conn_failure_r10");
    *p_length_read += sizeof(U16);
    }

    if (p_rrc_rlf_report->bitmask & RRC_UE_INFO_CONN_FAILURE_TYPE_R10_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_rlf_report->connection_failure_type_r10, p_src + *p_length_read, "connection_failure_type_r10");
    *p_length_read += sizeof(U32);
    }

    if (p_rrc_rlf_report->bitmask & RRC_UE_INFO_RLF_REPORT_LOACTION_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_rlf_report_location_info(
        &p_rrc_rlf_report->location_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rlf_report->bitmask & RRC_UE_INFO_FAILED_PCELLID_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrc_rlf_failed_pcellid(
        &p_rrc_rlf_report->failed_pcellid,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rlf_report->bitmask & RRC_UE_INFO_REESTABLISHMENT_CELLID_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrm_cell_global_id_eutra(
        &p_rrc_rlf_report->reestablish_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrc_rlf_report->bitmask & RRC_UE_INFO_TIME_CONN_FALURE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlf_report->time_conn_failure, p_src + *p_length_read, "time_conn_failure");
    *p_length_read += sizeof(U16);
    }

    if (p_rrc_rlf_report->bitmask & RRC_UE_INFO_CONN_FALURE_TYPE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_rlf_report->conn_failure_type, p_src + *p_length_read, "conn_failure_type");
    *p_length_read += sizeof(U32);
    }

    if (p_rrc_rlf_report->bitmask & RRC_UE_INFO_PREV_PCELLID_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_rrm_cell_global_id_eutra(
        &p_rrc_rlf_report->previous_pcell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

#ifdef ENDC_ENABLED
static rrc_return_et 
rrc_il_parse_x2ap_nr_nbour_cell_info
(
    x2ap_nr_nbour_cell_info_t *p_x2ap_nr_nbour_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    rrc_counter_t loop = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_nbour_cell_info, 0, sizeof(x2ap_nr_nbour_cell_info_t));

    /* This function parses x2ap_nr_nbour_cell_info_t */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    rrc_cp_unpack_U16(&p_x2ap_nr_nbour_cell_info->num_neigh_cell, p_src + *p_length_read, "num_neigh_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_nr_nbour_cell_info->num_neigh_cell < 1) || 
        (p_x2ap_nr_nbour_cell_info->num_neigh_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_nbour_cell_info->num_neigh_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_nr_nbour_cell_info->num_neigh_cell);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_nr_nbour_cell_info->num_neigh_cell;loop++)
    {    
        if (RRC_FAILURE == rrc_il_parse_x2ap_nr_neigh_cell_arry(&p_x2ap_nr_nbour_cell_info->nr_neigh_cell_arr[loop], 
            p_src + *p_length_read, length_left - *p_length_read, &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read;
    }

    return RRC_SUCCESS;
}

rrc_return_et 
rrc_il_parse_x2ap_nr_srvd_cell_arr
(
    x2ap_nr_srvd_cell_arr_t *p_x2ap_nr_srvd_cell_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_srvd_cell_arr, 0, sizeof(x2ap_nr_srvd_cell_arr_t));

    /* This function parses x2ap_nr_srvd_cell_arr_t */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    rrc_cp_unpack_U16(&p_x2ap_nr_srvd_cell_arr->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_srvd_nr_cell_info(&p_x2ap_nr_srvd_cell_arr->served_cell_info, 
        p_src + *p_length_read, length_left - *p_length_read, &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (NR_NEIGHBOUR_INFO_PRESENT & p_x2ap_nr_srvd_cell_arr->bitmask)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_nr_nbour_cell_info(&p_x2ap_nr_srvd_cell_arr->neighbour_cell_info, 
            p_src + *p_length_read, length_left - *p_length_read, &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read;
    }

    return RRC_SUCCESS;
}

rrc_return_et 
rrc_il_parse_x2ap_nr_srvd_cell_modify_arr
(
    x2ap_nr_srvd_cell_modify_arr_t *p_x2ap_nr_srvd_cell_mod_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_srvd_cell_mod_arr, 0, sizeof(x2ap_nr_srvd_cell_modify_arr_t));

    /* This function parses x2ap_nr_srvd_cell_arr_t */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    rrc_cp_unpack_U16(&p_x2ap_nr_srvd_cell_mod_arr->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_x2ap_nr_ecgi(&p_x2ap_nr_srvd_cell_mod_arr->old_nr_cgi, 
        p_src + *p_length_read, length_left - *p_length_read, &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_ENDC_X2_NR_DEACT_IND_PRESENT & p_x2ap_nr_srvd_cell_mod_arr->bitmask)
    {
        rrc_cp_unpack_U8(&p_x2ap_nr_srvd_cell_mod_arr->deact_ind, p_src + *p_length_read, "deact_ind");
        *p_length_read += sizeof(U8);

        /* Check for correct range [H - higher boundary] */
        if ((0 < p_x2ap_nr_srvd_cell_mod_arr->deact_ind))
        {
            RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_srvd_cell_mod_arr->deact_ind] should be in range [%d < 1] !", 
                p_x2ap_nr_srvd_cell_mod_arr->deact_ind);
            return RRC_FAILURE;
        }
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_srvd_nr_cell_info(&p_x2ap_nr_srvd_cell_mod_arr->served_cell_info, 
        p_src + *p_length_read, length_left - *p_length_read, &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (NR_NEIGHBOUR_INFO_PRESENT & p_x2ap_nr_srvd_cell_mod_arr->bitmask)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_nr_nbour_cell_info(&p_x2ap_nr_srvd_cell_mod_arr->neighbour_cell_info, 
            p_src + *p_length_read, length_left - *p_length_read, &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read;
    }

    return RRC_SUCCESS;
}

rrc_return_et
rrc_il_parse_x2ap_nr_srvd_cell_to_management_list
(
    x2ap_nr_srvd_cell_to_management_list_t *p_x2ap_nr_srvd_cell_to_add_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    rrc_counter_t loop = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_srvd_cell_to_add_list, 0, sizeof(x2ap_nr_srvd_cell_to_management_list_t));

    /* This function parses x2ap_nr_srvd_cell_to_management_list_t */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management, p_src + *p_length_read, "num_nr_srvd_cell_to_management");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management < 1) || 
        (p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_nr_srvd_cell_to_add_list->num_nr_srvd_cell_to_management; loop++)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_nr_srvd_cell_arr(
            &p_x2ap_nr_srvd_cell_to_add_list->nr_served_cell_management_list[loop],
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read; 
    }

    return RRC_SUCCESS;
}

rrc_return_et
rrc_il_parse_x2ap_nr_srvd_cell_to_modify_list
(
    x2ap_nr_srvd_cell_to_modify_list_t *p_x2ap_nr_srvd_cell_to_mod_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    rrc_counter_t loop = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_srvd_cell_to_mod_list, 0, sizeof(x2ap_nr_srvd_cell_to_modify_list_t));

    /* This function parses x2ap_nr_srvd_cell_to_modify_list_t */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify, p_src + *p_length_read, "num_nr_srvd_cell_to_modify");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify < 1) || 
        (p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_nr_srvd_cell_to_mod_list->num_nr_srvd_cell_to_modify; loop++)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_nr_srvd_cell_modify_arr(
            &p_x2ap_nr_srvd_cell_to_mod_list->nr_served_cell_modify_list[loop],
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read; 
    }

    return RRC_SUCCESS;
}

rrc_return_et
rrc_il_parse_x2ap_nr_srvd_cell_to_delete_list
(
    x2ap_nr_srvd_cell_to_delete_list_t *p_x2ap_nr_srvd_cell_to_del_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    rrc_counter_t loop = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nr_srvd_cell_to_del_list, 0, sizeof(x2ap_nr_srvd_cell_to_delete_list_t));

    /* This function parses x2ap_nr_srvd_cell_to_delete_list_t */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete, p_src + *p_length_read, "num_nr_srvd_cell_delete");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete < 1) || 
        (p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_nr_srvd_cell_to_del_list->num_nr_srvd_cell_delete; loop++)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_nr_ecgi(
            &p_x2ap_nr_srvd_cell_to_del_list->nr_served_cell_delete_list[loop],
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read; 
    }

    return RRC_SUCCESS;
}

rrc_return_et
rrc_il_parse_x2ap_endc_config_update_en_gnb_ind
(
    x2ap_endc_config_update_en_gnb_ind_t *p_x2_endc_config_update_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2_endc_config_update_ind, 0, sizeof(x2ap_endc_config_update_en_gnb_ind_t));

    /* This function parses x2ap_endc_config_update_en_gnb_ind_t */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2_endc_config_update_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_x2_gb_gnb_id(
            &p_x2_endc_config_update_ind->global_en_gnb_id,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MANAGEMENTLIST_PRESENT & 
        p_x2_endc_config_update_ind->bitmask)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_nr_srvd_cell_to_management_list(
            &p_x2_endc_config_update_ind->eutra_nr_cell_management,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }
        
        *p_length_read += length_read;
    }

    if (X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MODIFYLIST_PRESENT & 
        p_x2_endc_config_update_ind->bitmask)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_nr_srvd_cell_to_modify_list(
            &p_x2_endc_config_update_ind->eutra_nr_cell_modify,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }
        
        *p_length_read += length_read;
    }

    if (X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_DELETELIST_PRESENT & 
        p_x2_endc_config_update_ind->bitmask)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_nr_srvd_cell_to_delete_list(
            &p_x2_endc_config_update_ind->eutra_nr_cell_delete,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }
        
        *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static rrc_return_et 
rrc_il_parse_x2ap_partial_list
(
    x2ap_partial_list_t *p_x2ap_partial_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    rrc_counter_t loop = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_partial_list, 0, sizeof(x2ap_partial_list_t));

    /* This function parses x2ap_partial_list_t */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_partial_list");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_partial_list->nrcgi_count, p_src + *p_length_read, "nrcgi_count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_partial_list->nrcgi_count < 1) || (p_x2ap_partial_list->nrcgi_count > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_partial_list->nrcgi_count] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_partial_list->nrcgi_count);
        return RRC_FAILURE;
    }

    for (loop = 0; loop < p_x2ap_partial_list->nrcgi_count && loop < MAX_SERVED_CELLS; loop++)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_nr_ecgi(&p_x2ap_partial_list->nrcgi_list[loop], 
            p_src + *p_length_read, length_left - *p_length_read, &length_read))
        {
            return RRC_FAILURE;
        }    

        *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static rrc_return_et 
rrc_il_parse_x2ap_served_cell_info
(
   x2ap_served_cell_info_t *p_x2ap_served_cell_info,
   U8 *p_src,
   S32 length_left,
   S32 *p_length_read)
{
    S32 length_read = 0;
    rrc_counter_t loop = 0;
    *p_length_read = 0;

    memset_wrapper(p_x2ap_served_cell_info, 0, sizeof(x2ap_served_cell_info_t));

    /* This function parses x2ap_served_cell_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_served_cell_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_served_cell_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cell_info->pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cell_info->pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_x2ap_served_cell_info->pci);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_served_cell_info->cell_id, p_src + *p_length_read, "cell_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [L- Lower to H - higher boundary] */
    if (p_x2ap_served_cell_info->cell_id >= MAX_SERVED_CELLS)
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cell_info->cell_id] should be 0 to"
            " 7. Incorrect value %u received.", p_x2ap_served_cell_info->cell_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_x2ap_served_cell_info->tac) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    for (loop = 0; loop < ARRSIZE(p_x2ap_served_cell_info->tac); loop++)
    {
        /* Parse/Unpack parameter of basic type */
        rrc_cp_unpack_U8(&p_x2ap_served_cell_info->tac[loop], p_src + *p_length_read, "tac[]");
        *p_length_read += sizeof(U8);
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_broadcast_plmn(
        &p_x2ap_served_cell_info->bcast_plmn_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_choice_eutra_mode_info(
        &p_x2ap_served_cell_info->choice_eutra_mode,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_served_cell_info->bitmask & NUM_ANTENNA_PORT_BITMASK)
    {

        if (*p_length_read + (S32)sizeof(U8) > length_left)
        {
            RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
            return RRC_FAILURE;
        }

        /* Parse/Unpack parameter of basic type */
        rrc_cp_unpack_U8(&p_x2ap_served_cell_info->num_antenna_port, p_src + *p_length_read, "num_antenna_port");
        *p_length_read += sizeof(U8);

        /* Check for correct range [H - higher boundary] */
        if ((p_x2ap_served_cell_info->num_antenna_port > 2))
        {
            RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cell_info->num_antenna_port] should be less than"
                " or equal to 2. Incorrect value %u received.", p_x2ap_served_cell_info->num_antenna_port);
            return RRC_FAILURE;
        }
    }

    if (p_x2ap_served_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_prach_configuration_info(
            &p_x2ap_served_cell_info->prach_config,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read;
    }

    if (p_x2ap_served_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_mbsfn_subframe_info(
            &p_x2ap_served_cell_info->mbsfn_subframe_info,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read;
    }

    if (p_x2ap_served_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {
        if (*p_length_read + (S32)sizeof(p_x2ap_served_cell_info->csg_identity) > length_left)
        {
            RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
            return RRC_FAILURE;
        }

        /* Parse OCTET_STRING FIXED of basic type elements */
        {
            for (loop = 0; loop < ARRSIZE(p_x2ap_served_cell_info->csg_identity); loop++)
            {
                rrc_cp_unpack_U8((void*)&p_x2ap_served_cell_info->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
                *p_length_read += sizeof(U8);
            }
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static rrc_return_et 
rrc_il_parse_x2ap_served_eutra_nr_cell_info_arr
(
    x2ap_served_eutra_nr_cell_info_arr_t *p_x2ap_served_eutra_nr_cell_info_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_served_eutra_nr_cell_info_arr, 0, sizeof(x2ap_served_eutra_nr_cell_info_arr_t));

    /* This function parses x2ap_served_eutra_nr_cell_info_arr_t */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_served_eutra_nr_cell_info_arr");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_served_eutra_nr_cell_info_arr->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_x2ap_served_cell_info(
        &p_x2ap_served_eutra_nr_cell_info_arr->eutra_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (NR_NEIGHBOUR_INFO_PRESENT & p_x2ap_served_eutra_nr_cell_info_arr->bitmask)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_nr_nbour_cell_info(
            &p_x2ap_served_eutra_nr_cell_info_arr->nr_neighbour_cell_info,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;   
}

static rrc_return_et 
rrc_il_parse_x2ap_served_eutra_nr_cells_to_modify_info
(
    x2ap_served_eutra_nr_cells_to_modify_info_t *p_x2ap_served_eutra_nr_cell_to_mod_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_served_eutra_nr_cell_to_mod_info, 0, sizeof(x2ap_served_eutra_nr_cells_to_modify_info_t));

    /* This function parses x2ap_served_eutra_nr_cells_to_modify_info_t */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_served_eutra_nr_cells_to_modify_info");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }
    
    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_served_eutra_nr_cell_to_mod_info->old_cell_id, p_src + *p_length_read, "old_cell_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_eutra_nr_cell_to_mod_info->old_cell_id >= MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_mod_info->old_cell_id] should be 0 "
            "to 7. Incorrect value %u received.", p_x2ap_served_eutra_nr_cell_to_mod_info->old_cell_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_served_eutra_nr_cell_info_arr(
        &p_x2ap_served_eutra_nr_cell_to_mod_info->served_eutra_nr_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

     if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;   
}

static rrc_return_et 
rrc_il_parse_x2ap_rrm_cell_assistance_info
(
    x2ap_rrm_cell_assistance_info_t *p_x2ap_rrm_cell_assistance_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_rrm_cell_assistance_info, 0, sizeof(x2ap_rrm_cell_assistance_info_t));

    /* This function parses x2ap_rrm_cell_assistance_info_t */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_rrm_cell_assistance_info");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_rrm_cell_assistance_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (X2AP_CELL_ASSISTANCE_LIMITED_LIST_PRESENT & p_x2ap_rrm_cell_assistance_info->bitmask)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_partial_list(
            &p_x2ap_rrm_cell_assistance_info->partial_nr_cells,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read;
    }

    if (X2AP_CELL_ASSISTANCE_FULL_LIST_PRESENT & p_x2ap_rrm_cell_assistance_info->bitmask)
    {
        /* Parse/Unpack parameter of basic type */
        rrc_cp_unpack_U16(&p_x2ap_rrm_cell_assistance_info->full_nr_cells_ind, p_src + *p_length_read, "full_nr_cells_ind");
        *p_length_read += sizeof(U16);

        /* Check for correct range [H - higher boundary] */
        if ((0 < p_x2ap_rrm_cell_assistance_info->full_nr_cells_ind))
        {
            RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_rrm_cell_assistance_info->full_nr_cells_ind] should be in range [%d < 1] !", 
                p_x2ap_rrm_cell_assistance_info->full_nr_cells_ind);
            return RRC_FAILURE;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static rrc_return_et 
rrc_il_parse_x2ap_served_eutra_nr_cells_to_add_list
(
    x2ap_served_eutra_nr_cells_to_add_list_t *p_x2ap_served_eutra_nr_cell_to_add_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_served_eutra_nr_cell_to_add_list, 0, sizeof(x2ap_served_eutra_nr_cells_to_add_list_t));

    /* This function parses x2ap_served_eutra_nr_cells_to_add_list_t */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_served_eutra_nr_cells_to_add_list");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell, p_src + *p_length_read, "num_served_eutra_nr_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell < 1) || 
        (p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_eutra_nr_cell_to_add_list->num_served_eutra_nr_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_served_eutra_nr_cell_info_arr(
                &p_x2ap_served_eutra_nr_cell_to_add_list->eutra_nr_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static rrc_return_et 
rrc_il_parse_x2ap_served_eutra_nr_cells_to_modify_list
(
    x2ap_served_eutra_nr_cells_to_modify_list_t *p_x2ap_served_eutra_nr_cell_to_mod_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_served_eutra_nr_cell_to_mod_list, 0, sizeof(x2ap_served_eutra_nr_cells_to_modify_list_t));

    /* This function parses x2ap_served_eutra_nr_cells_to_modify_list_t */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_served_eutra_nr_cells_to_modify_list");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell, 
        p_src + *p_length_read, "num_served_eutra_nr_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell < 1) || 
        (p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_eutra_nr_cell_to_mod_list->num_served_eutra_nr_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_served_eutra_nr_cells_to_modify_info(
                &p_x2ap_served_eutra_nr_cell_to_mod_list->eutra_nr_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static rrc_return_et 
rrc_il_parse_x2ap_served_eutra_nr_cells_to_delete_list
(
    x2ap_served_eutra_nr_cells_to_delete_list_t *p_x2ap_served_eutra_nr_cell_to_del_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    memset_wrapper(p_x2ap_served_eutra_nr_cell_to_del_list, 0, sizeof(x2ap_served_eutra_nr_cells_to_delete_list_t));

    /* This function parses x2ap_served_eutra_nr_cells_to_delete_list_t */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_served_eutra_nr_cells_to_delete_list");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell, 
        p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell < 1) || (p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", 
            p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_eutra_nr_cell_to_del_list->num_served_cell; loop++)
        {
            if (*p_length_read + (S32)sizeof(U8) > length_left)
            {
               RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
                return RRC_FAILURE;
            }

            /* Check for correct range [H - higher boundary] */
            if ((p_x2ap_served_eutra_nr_cell_to_del_list->cell_id_list[loop] >= MAX_SERVED_CELLS))
            {
                RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_eutra_nr_cell_to_del_list->cell_id_list[]] should be 0 "
                    "to 7. Incorrect value %u received.", p_x2ap_served_eutra_nr_cell_to_del_list->cell_id_list[loop]);
                return RRC_FAILURE;
            }

            /* Parse/Unpack parameter of basic type */
            rrc_cp_unpack_U8(&p_x2ap_served_eutra_nr_cell_to_del_list->cell_id_list[loop], 
                p_src + *p_length_read, "cell_id_list[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_endc_config_update_req
(
    x2ap_endc_config_update_req_t *p_x2ap_endc_config_update_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    memset_wrapper(p_x2ap_endc_config_update_req, 0, sizeof(x2ap_endc_config_update_req_t));

    /* This function parses x2ap_endc_config_update_req_t */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_endc_config_update_req");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_endc_config_update_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_endc_config_update_req->num_peer_gnb, p_src + *p_length_read, "num_peer_gnb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((1 > p_x2ap_endc_config_update_req->num_peer_gnb || 
        MAX_PEER_GNB< p_x2ap_endc_config_update_req->num_peer_gnb))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_endc_config_update_req->num_peer_gnb] should be in range [1 <= %d <= MAX_PEER_GNB] !", 
            p_x2ap_endc_config_update_req->num_peer_gnb);
            
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_endc_config_update_req->num_peer_gnb; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2_gb_gnb_id(
                &p_x2ap_endc_config_update_req->gnb_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (X2AP_ENDC_CONFIGUPDATE_ENB_CELLASSISTANCE_INFO_PRESENT & 
        p_x2ap_endc_config_update_req->bitmask)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_rrm_cell_assistance_info(
            &p_x2ap_endc_config_update_req->cell_assistance_info,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read;
    }

    if (X2AP_SERVED_EUTRA_CELLS_TO_ADD_LIST_PRESENT & p_x2ap_endc_config_update_req->bitmask)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_served_eutra_nr_cells_to_add_list(
            &p_x2ap_endc_config_update_req->add_served_cell,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read;
    }

    if (X2AP_SERVED_EUTRA_CELLS_TO_MODIFY_LIST_PRESENT & p_x2ap_endc_config_update_req->bitmask)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_served_eutra_nr_cells_to_modify_list(
            &p_x2ap_endc_config_update_req->mod_served_cell,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read;
    }

    if (X2AP_SERVED_EUTRA_CELLS_TO_DELETE_LIST_PRESENT & p_x2ap_endc_config_update_req->bitmask)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_served_eutra_nr_cells_to_delete_list(
            &p_x2ap_endc_config_update_req->del_served_cell,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_endc_config_update_resp
(
    x2ap_endc_config_update_resp_t *p_x2ap_endc_config_update_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_endc_config_update_resp, 0, sizeof(x2ap_endc_config_update_resp_t));

    /* This function parses x2ap_endc_config_update_resp */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_endc_config_update_resp");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_endc_config_update_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_endc_config_update_resp->enb_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2_gb_gnb_id(
        &p_x2ap_endc_config_update_resp->gnb_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_endc_config_update_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_endc_config_update_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_endc_config_update_resp->response] should be in range [%d <= 1] !", p_x2ap_endc_config_update_resp->response);
        return RRC_FAILURE;
    }

    if (X2AP_CAUSE_PRESENT & p_x2ap_endc_config_update_resp->bitmask)
    {
        /* Parse/Unpack parameter of basic type */
        rrc_cp_unpack_U8(&(p_x2ap_endc_config_update_resp->cause.type), p_src + *p_length_read, "cause_type");
        *p_length_read += sizeof(U8);

        /* Parse/Unpack parameter of basic type */
        rrc_cp_unpack_U16(&(p_x2ap_endc_config_update_resp->cause.value), p_src + *p_length_read, "cause_value");
        *p_length_read += sizeof(U16);
    }

    if (X2AP_NR_CELL_INFOMATION_PRESENT & p_x2ap_endc_config_update_resp->bitmask)
    {
        if (RRC_FAILURE == rrc_il_parse_x2ap_nr_srvd_cell_to_management_list(
            &p_x2ap_endc_config_update_resp->nr_cell_information,
            p_src + *p_length_read,
            length_left - *p_length_read,
            &length_read))
        {
            return RRC_FAILURE;
        }

        *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_x2ap_endc_config_update_wait_ind
(
    x2ap_endc_config_update_wait_ind_t *p_x2ap_endc_config_update_wait_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_endc_config_update_wait_ind, 0, sizeof(x2ap_endc_config_update_wait_ind_t));

    /* This function parses x2ap_endc_config_update_wait_ind */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_endc_config_update_wait_ind");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_endc_config_update_wait_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_endc_config_update_wait_ind->enb_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2_gb_gnb_id(
        &p_x2ap_endc_config_update_wait_ind->gnb_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_endc_config_update_wait_ind->time_to_wait, p_src + *p_length_read, "time_to_wait");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_endc_config_update_wait_ind->time_to_wait > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_endc_config_update_wait_ind->time_to_wait] should be in range [%d <= 5] !", p_x2ap_endc_config_update_wait_ind->time_to_wait);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}
#endif

