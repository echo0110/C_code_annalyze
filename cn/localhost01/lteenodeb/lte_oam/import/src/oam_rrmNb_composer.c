/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: oam_rrmNb_composer.c $
 *
 *******************************************************************************
 *
 * File Description: All RRM OAM composer function defination for RRM OAM
 *                   RRM OAM APIs.
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "rrm_pack_unpack_utils.h"
#include "oam_rrmNb_composer.h"
#include "rrm_oamNb_intf.h"
#include "rrm_utils.h"
#include "oam_defines.h"
#include "oam_proto.h"
static oam_error_code_et oam_error = NO_ERROR;


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
oam_return_et
oam_compose_rrm_oam_cell_plmn_info
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
oam_return_et
oam_compose_rrm_oam_eutran_global_cell_id
(
    U8  **pp_buffer,
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id
);

static
rrm_length_t
rrm_get_rrm_oam_cell_access_restriction_params_nb_len
(
    rrm_oam_cell_access_restriction_params_nb_t *p_rrm_oam_cell_access_restriction_params_nb
);

static
oam_return_et
oam_compose_rrm_oam_cell_access_restriction_params_nb
(
    U8  **pp_buffer,
    rrm_oam_cell_access_restriction_params_nb_t *p_rrm_oam_cell_access_restriction_params_nb
);

static
rrm_length_t
rrm_get_rrm_oam_cell_info_nb_len
(
    rrm_oam_cell_info_nb_t *p_rrm_oam_cell_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_cell_info_nb
(
    U8  **pp_buffer,
    rrm_oam_cell_info_nb_t *p_rrm_oam_cell_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_rsrp_thresholds_prach_info_list_r13_len
(
    rrm_oam_rsrp_thresholds_prach_info_list_r13_t *p_rrm_oam_rsrp_thresholds_prach_info_list_r13
);

static
oam_return_et
oam_compose_rrm_oam_rsrp_thresholds_prach_info_list_r13
(
    U8  **pp_buffer,
    rrm_oam_rsrp_thresholds_prach_info_list_r13_t *p_rrm_oam_rsrp_thresholds_prach_info_list_r13
);

static
rrm_length_t
rrm_get_rrm_oam_nprach_parameters_nb_r13_len
(
    rrm_oam_nprach_parameters_nb_r13_t *p_rrm_oam_nprach_parameters_nb_r13
);

static
oam_return_et
oam_compose_rrm_oam_nprach_parameters_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_nprach_parameters_nb_r13_t *p_rrm_oam_nprach_parameters_nb_r13
);

static
rrm_length_t
rrm_get_rrm_oam_nprach_parameter_list_nb_r13_len
(
    rrm_oam_nprach_parameter_list_nb_r13_t *p_rrm_oam_nprach_parameter_list_nb_r13
);

static
oam_return_et
oam_compose_rrm_oam_nprach_parameter_list_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_nprach_parameter_list_nb_r13_t *p_rrm_oam_nprach_parameter_list_nb_r13
);

static
rrm_length_t
rrm_get_rrm_oam_prach_nb_len
(
    rrm_oam_prach_nb_t *p_rrm_oam_prach_nb
);

static
oam_return_et
oam_compose_rrm_oam_prach_nb
(
    U8  **pp_buffer,
    rrm_oam_prach_nb_t *p_rrm_oam_prach_nb
);

static
rrm_length_t
rrm_get_rrm_oam_nprach_parameters_list_nb_v1330_len
(
    rrm_oam_nprach_parameters_list_nb_v1330_t *p_rrm_oam_nprach_parameters_list_nb_v1330
);

static
oam_return_et
oam_compose_rrm_oam_nprach_parameters_list_nb_v1330
(
    U8  **pp_buffer,
    rrm_oam_nprach_parameters_list_nb_v1330_t *p_rrm_oam_nprach_parameters_list_nb_v1330
);

static
rrm_length_t
rrm_get_rrm_oam_nprach_config_sib_nb_v1330_len
(
    rrm_oam_nprach_config_sib_nb_v1330_t *p_rrm_oam_nprach_config_sib_nb_v1330
);

static
oam_return_et
oam_compose_rrm_oam_nprach_config_sib_nb_v1330
(
    U8  **pp_buffer,
    rrm_oam_nprach_config_sib_nb_v1330_t *p_rrm_oam_nprach_config_sib_nb_v1330
);

static
rrm_length_t
rrm_get_rrm_oam_npdcch_config_dedicated_nb_r13_len
(
    rrm_oam_npdcch_config_dedicated_nb_r13_t *p_rrm_oam_npdcch_config_dedicated_nb_r13
);

static
oam_return_et
oam_compose_rrm_oam_npdcch_config_dedicated_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_npdcch_config_dedicated_nb_r13_t *p_rrm_oam_npdcch_config_dedicated_nb_r13
);

static
rrm_length_t
rrm_get_rrm_oam_ack_nack_num_repetitions_msg4_r13_len
(
    rrm_oam_ack_nack_num_repetitions_msg4_r13_t *p_rrm_oam_ack_nack_num_repetitions_msg4_r13
);

static
oam_return_et
oam_compose_rrm_oam_ack_nack_num_repetitions_msg4_r13
(
    U8  **pp_buffer,
    rrm_oam_ack_nack_num_repetitions_msg4_r13_t *p_rrm_oam_ack_nack_num_repetitions_msg4_r13
);

static
rrm_length_t
rrm_get_rrm_oam_dmrs_config_r13_len
(
    rrm_oam_dmrs_config_r13_t *p_rrm_oam_dmrs_config_r13
);

static
oam_return_et
oam_compose_rrm_oam_dmrs_config_r13
(
    U8  **pp_buffer,
    rrm_oam_dmrs_config_r13_t *p_rrm_oam_dmrs_config_r13
);

static
rrm_length_t
rrm_get_rrm_oam_ul_reference_signal_npusch_nb_len
(
    rrm_oam_ul_reference_signal_npusch_nb_t *p_rrm_oam_ul_reference_signal_npusch_nb
);

static
oam_return_et
oam_compose_rrm_oam_ul_reference_signal_npusch_nb
(
    U8  **pp_buffer,
    rrm_oam_ul_reference_signal_npusch_nb_t *p_rrm_oam_ul_reference_signal_npusch_nb
);

static
rrm_length_t
rrm_get_rrm_oam_npusch_config_dedicated_r13_len
(
    rrm_oam_npusch_config_dedicated_r13_t *p_rrm_oam_npusch_config_dedicated_r13
);

static
oam_return_et
oam_compose_rrm_oam_npusch_config_dedicated_r13
(
    U8  **pp_buffer,
    rrm_oam_npusch_config_dedicated_r13_t *p_rrm_oam_npusch_config_dedicated_r13
);

static
rrm_length_t
rrm_get_rrm_oam_physical_layer_param_pusch_nb_len
(
    rrm_oam_physical_layer_param_pusch_nb_t *p_rrm_oam_physical_layer_param_pusch_nb
);

static
oam_return_et
oam_compose_rrm_oam_physical_layer_param_pusch_nb
(
    U8  **pp_buffer,
    rrm_oam_physical_layer_param_pusch_nb_t *p_rrm_oam_physical_layer_param_pusch_nb
);

static
rrm_length_t
rrm_get_rrm_oam_uplink_power_control_nb_len
(
    rrm_oam_uplink_power_control_nb_t *p_rrm_oam_uplink_power_control_nb
);

static
oam_return_et
oam_compose_rrm_oam_uplink_power_control_nb
(
    U8  **pp_buffer,
    rrm_oam_uplink_power_control_nb_t *p_rrm_oam_uplink_power_control_nb
);

static
rrm_length_t
rrm_get_rrm_oam_dl_gap_config_nb_len
(
    rrm_oam_dl_gap_config_nb_t *p_rrm_oam_dl_gap_config_nb
);

static
oam_return_et
oam_compose_rrm_oam_dl_gap_config_nb
(
    U8  **pp_buffer,
    rrm_oam_dl_gap_config_nb_t *p_rrm_oam_dl_gap_config_nb
);

static
rrm_length_t
rrm_get_rrm_oam_physical_layer_params_nb_len
(
    rrm_oam_physical_layer_params_nb_t *p_rrm_oam_physical_layer_params_nb
);

static
oam_return_et
oam_compose_rrm_oam_physical_layer_params_nb
(
    U8  **pp_buffer,
    rrm_oam_physical_layer_params_nb_t *p_rrm_oam_physical_layer_params_nb
);

static
rrm_length_t
rrm_get_rrm_oam_power_ramping_params_len
(
    rrm_oam_power_ramping_params_t *p_rrm_oam_power_ramping_params
);

static
oam_return_et
oam_compose_rrm_oam_power_ramping_params
(
    U8  **pp_buffer,
    rrm_oam_power_ramping_params_t *p_rrm_oam_power_ramping_params
);

static
rrm_length_t
rrm_get_rrm_oam_rach_info_len
(
    rrm_oam_rach_info_t *p_rrm_oam_rach_info
);

static
oam_return_et
oam_compose_rrm_oam_rach_info
(
    U8  **pp_buffer,
    rrm_oam_rach_info_t *p_rrm_oam_rach_info
);

static
rrm_length_t
rrm_get_rrm_oam_rach_infolist_len
(
    rrm_oam_rach_infolist_t *p_rrm_oam_rach_infolist
);

static
oam_return_et
oam_compose_rrm_oam_rach_infolist
(
    U8  **pp_buffer,
    rrm_oam_rach_infolist_t *p_rrm_oam_rach_infolist
);

static
rrm_length_t
rrm_get_rrm_oam_preamble_info_nb_len
(
    rrm_oam_preamble_info_nb_t *p_rrm_oam_preamble_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_preamble_info_nb
(
    U8  **pp_buffer,
    rrm_oam_preamble_info_nb_t *p_rrm_oam_preamble_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_rach_nb_len
(
    rrm_oam_rach_nb_t *p_rrm_oam_rach_nb
);

static
oam_return_et
oam_compose_rrm_oam_rach_nb
(
    U8  **pp_buffer,
    rrm_oam_rach_nb_t *p_rrm_oam_rach_nb
);

static
rrm_length_t
rrm_get_rrm_oam_drx_config_nb_len
(
    rrm_oam_drx_config_nb_t *p_rrm_oam_drx_config_nb
);

static
oam_return_et
oam_compose_rrm_oam_drx_config_nb
(
    U8  **pp_buffer,
    rrm_oam_drx_config_nb_t *p_rrm_oam_drx_config_nb
);

static
rrm_length_t
rrm_get_rrm_oam_drx_nb_len
(
    rrm_oam_drx_nb_t *p_rrm_oam_drx_nb
);

static
oam_return_et
oam_compose_rrm_oam_drx_nb
(
    U8  **pp_buffer,
    rrm_oam_drx_nb_t *p_rrm_oam_drx_nb
);

static
rrm_length_t
rrm_get_rrm_oam_logical_channel_sr_config_r13_len
(
    rrm_oam_logical_channel_sr_config_r13_t *p_rrm_oam_logical_channel_sr_config_r13
);

static
oam_return_et
oam_compose_rrm_oam_logical_channel_sr_config_r13
(
    U8  **pp_buffer,
    rrm_oam_logical_channel_sr_config_r13_t *p_rrm_oam_logical_channel_sr_config_r13
);

static
rrm_length_t
rrm_get_rrm_oam_ul_sch_config_r13_len
(
    rrm_oam_ul_sch_config_r13 *p_rrm_oam_ul_sch_config_r13
);

static
oam_return_et
oam_compose_rrm_oam_ul_sch_config_r13
(
    U8  **pp_buffer,
    rrm_oam_ul_sch_config_r13 *p_rrm_oam_ul_sch_config_r13
);

static
rrm_length_t
rrm_get_rrm_oam_mac_layer_params_nb_len
(
    rrm_oam_mac_layer_params_nb_t *p_rrm_oam_mac_layer_params_nb
);

static
oam_return_et
oam_compose_rrm_oam_mac_layer_params_nb
(
    U8  **pp_buffer,
    rrm_oam_mac_layer_params_nb_t *p_rrm_oam_mac_layer_params_nb
);

static
rrm_length_t
rrm_get_ul_am_rlc_nb_r13_len
(
    ul_am_rlc_nb_r13_t *p_ul_am_rlc_nb_r13
);

static
oam_return_et
oam_compose_ul_am_rlc_nb_r13
(
    U8  **pp_buffer,
    ul_am_rlc_nb_r13_t *p_ul_am_rlc_nb_r13
);

static
rrm_length_t
rrm_get_dl_am_rlc_nb_r13_len
(
    dl_am_rlc_nb_r13_t *p_dl_am_rlc_nb_r13
);

static
oam_return_et
oam_compose_dl_am_rlc_nb_r13
(
    U8  **pp_buffer,
    dl_am_rlc_nb_r13_t *p_dl_am_rlc_nb_r13
);

static
rrm_length_t
rrm_get_rrm_oam_logical_channel_config_nb_len
(
    rrm_oam_logical_channel_config_nb_t *p_rrm_oam_logical_channel_config_nb
);

static
oam_return_et
oam_compose_rrm_oam_logical_channel_config_nb
(
    U8  **pp_buffer,
    rrm_oam_logical_channel_config_nb_t *p_rrm_oam_logical_channel_config_nb
);

static
rrm_length_t
rrm_get_rrm_oam_srb_info_nb_len
(
    rrm_oam_srb_info_nb_t *p_rrm_oam_srb_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_srb_info_nb
(
    U8  **pp_buffer,
    rrm_oam_srb_info_nb_t *p_rrm_oam_srb_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_srb_nb_len
(
    rrm_oam_srb_nb_t *p_rrm_oam_srb_nb
);

static
oam_return_et
oam_compose_rrm_oam_srb_nb
(
    U8  **pp_buffer,
    rrm_oam_srb_nb_t *p_rrm_oam_srb_nb
);

static
rrm_length_t
rrm_get_rrm_oam_rlc_layer_params_nb_len
(
    rrm_oam_rlc_layer_params_nb_t *p_rrm_oam_rlc_layer_params_nb
);

static
oam_return_et
oam_compose_rrm_oam_rlc_layer_params_nb
(
    U8  **pp_buffer,
    rrm_oam_rlc_layer_params_nb_t *p_rrm_oam_rlc_layer_params_nb
);

static
rrm_length_t
rrm_get_rrm_oam_rrc_timers_nb_len
(
    rrm_oam_rrc_timers_nb_t *p_rrm_oam_rrc_timers_nb
);

static
oam_return_et
oam_compose_rrm_oam_rrc_timers_nb
(
    U8  **pp_buffer,
    rrm_oam_rrc_timers_nb_t *p_rrm_oam_rrc_timers_nb
);

static
rrm_length_t
rrm_get_rrm_oam_rrc_constants_nb_len
(
    rrm_oam_rrc_constants_nb_t *p_rrm_oam_rrc_constants_nb
);

static
oam_return_et
oam_compose_rrm_oam_rrc_constants_nb
(
    U8  **pp_buffer,
    rrm_oam_rrc_constants_nb_t *p_rrm_oam_rrc_constants_nb
);

static
rrm_length_t
rrm_get_rrm_oam_rrc_timers_and_constants_nb_len
(
    rrm_oam_rrc_timers_and_constants_nb_t *p_rrm_oam_rrc_timers_and_constants_nb
);

static
oam_return_et
oam_compose_rrm_oam_rrc_timers_and_constants_nb
(
    U8  **pp_buffer,
    rrm_oam_rrc_timers_and_constants_nb_t *p_rrm_oam_rrc_timers_and_constants_nb
);

static
rrm_length_t
rrm_get_rrm_oam_ns_pmax_value_nb_r13_len
(
    rrm_oam_ns_pmax_value_nb_r13_t *p_rrm_oam_ns_pmax_value_nb_r13
);

static
oam_return_et
oam_compose_rrm_oam_ns_pmax_value_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_ns_pmax_value_nb_r13_t *p_rrm_oam_ns_pmax_value_nb_r13
);

static
rrm_length_t
rrm_get_rrm_oam_freq_band_info_nb_len
(
    rrm_oam_freq_band_info_nb_t *p_rrm_oam_freq_band_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_freq_band_info_nb
(
    U8  **pp_buffer,
    rrm_oam_freq_band_info_nb_t *p_rrm_oam_freq_band_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_multi_band_info_nb_r13_len
(
    rrm_oam_multi_band_info_nb_r13_t *p_rrm_oam_multi_band_info_nb_r13
);

static
oam_return_et
oam_compose_rrm_oam_multi_band_info_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_multi_band_info_nb_r13_t *p_rrm_oam_multi_band_info_nb_r13
);

static
rrm_length_t
rrm_get_rrm_oam_multiband_info_list_nb_r13_len
(
    rrm_oam_multiband_info_list_nb_r13_t *p_rrm_oam_multiband_info_list_nb_r13
);

static
oam_return_et
oam_compose_rrm_oam_multiband_info_list_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_multiband_info_list_nb_r13_t *p_rrm_oam_multiband_info_list_nb_r13
);

static
rrm_length_t
rrm_get_rrm_oam_downlink_bitmap_nb_r13_len
(
    rrm_oam_downlink_bitmap_nb_r13_t *p_rrm_oam_downlink_bitmap_nb_r13
);

static
oam_return_et
oam_compose_rrm_oam_downlink_bitmap_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_downlink_bitmap_nb_r13_t *p_rrm_oam_downlink_bitmap_nb_r13
);

static
rrm_length_t
rrm_get_rrm_oam_rf_configurations_nb_len
(
    rrm_oam_rf_configurations_nb_t *p_rrm_oam_rf_configurations_nb
);

static
oam_return_et
oam_compose_rrm_oam_rf_configurations_nb
(
    U8  **pp_buffer,
    rrm_oam_rf_configurations_nb_t *p_rrm_oam_rf_configurations_nb
);

static
rrm_length_t
rrm_get_rrm_oam_rf_params_nb_len
(
    rrm_oam_rf_params_nb_t *p_rrm_oam_rf_params_nb
);

static
oam_return_et
oam_compose_rrm_oam_rf_params_nb
(
    U8  **pp_buffer,
    rrm_oam_rf_params_nb_t *p_rrm_oam_rf_params_nb
);

static
rrm_length_t
rrm_get_rrm_oam_s1ap_params_len
(
    rrm_oam_s1ap_params_t *p_rrm_oam_s1ap_params
);

static
oam_return_et
oam_compose_rrm_oam_s1ap_params
(
    U8  **pp_buffer,
    rrm_oam_s1ap_params_t *p_rrm_oam_s1ap_params
);

static
rrm_length_t
rrm_get_rrm_oam_ran_info_nb_len
(
    rrm_oam_ran_info_nb_t *p_rrm_oam_ran_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_ran_info_nb
(
    U8  **pp_buffer,
    rrm_oam_ran_info_nb_t *p_rrm_oam_ran_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_plmn_access_info_nb_len
(
    rrm_oam_plmn_access_info_nb_t *p_rrm_oam_plmn_access_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_plmn_access_info_nb
(
    U8  **pp_buffer,
    rrm_oam_plmn_access_info_nb_t *p_rrm_oam_plmn_access_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_general_epc_params_nb_len
(
    rrm_oam_general_epc_params_nb_t *p_rrm_oam_general_epc_params_nb
);

static
oam_return_et
oam_compose_rrm_oam_general_epc_params_nb
(
    U8  **pp_buffer,
    rrm_oam_general_epc_params_nb_t *p_rrm_oam_general_epc_params_nb
);

static
rrm_length_t
rrm_get_rrm_oam_epc_nb_len
(
    rrm_oam_epc_nb_t *p_rrm_oam_epc_nb
);

static
oam_return_et
oam_compose_rrm_oam_epc_nb
(
    U8  **pp_buffer,
    rrm_oam_epc_nb_t *p_rrm_oam_epc_nb
);

static
rrm_length_t
rrm_get_rrm_oam_rrmc_mac_config_nb_len
(
    rrm_oam_rrmc_mac_config_nb_t *p_rrm_oam_rrmc_mac_config_nb
);

static
oam_return_et
oam_compose_rrm_oam_rrmc_mac_config_nb
(
    U8  **pp_buffer,
    rrm_oam_rrmc_mac_config_nb_t *p_rrm_oam_rrmc_mac_config_nb
);

static
rrm_length_t
rrm_get_rrm_oam_cell_selection_info_v13_nb_len
(
    rrm_oam_cell_selection_info_v13_nb_t *p_rrm_oam_cell_selection_info_v13_nb
);

static
oam_return_et
oam_compose_rrm_oam_cell_selection_info_v13_nb
(
    U8  **pp_buffer,
    rrm_oam_cell_selection_info_v13_nb_t *p_rrm_oam_cell_selection_info_v13_nb
);

static
rrm_length_t
rrm_get_rrm_oam_sib_mapping_info_nb_len
(
    rrm_oam_sib_mapping_info_nb_t *p_rrm_oam_sib_mapping_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_sib_mapping_info_nb
(
    U8  **pp_buffer,
    rrm_oam_sib_mapping_info_nb_t *p_rrm_oam_sib_mapping_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_scheduling_info_nb_len
(
    rrm_oam_scheduling_info_nb_t *p_rrm_oam_scheduling_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_scheduling_info_nb
(
    U8  **pp_buffer,
    rrm_oam_scheduling_info_nb_t *p_rrm_oam_scheduling_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_sib_type_1_info_nb_len
(
    rrm_oam_sib_type_1_info_nb_t *p_rrm_oam_sib_type_1_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_sib_type_1_info_nb
(
    U8  **pp_buffer,
    rrm_oam_sib_type_1_info_nb_t *p_rrm_oam_sib_type_1_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_paging_info_nb_len
(
    rrm_oam_paging_info_nb_t *p_rrm_oam_paging_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_paging_info_nb
(
    U8  **pp_buffer,
    rrm_oam_paging_info_nb_t *p_rrm_oam_paging_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_radio_resource_config_common_sib_nb_len
(
    rrm_oam_radio_resource_config_common_sib_nb_t *p_rrm_oam_radio_resource_config_common_sib_nb
);

static
oam_return_et
oam_compose_rrm_oam_radio_resource_config_common_sib_nb
(
    U8  **pp_buffer,
    rrm_oam_radio_resource_config_common_sib_nb_t *p_rrm_oam_radio_resource_config_common_sib_nb
);

static
rrm_length_t
rrm_get_rrm_oam_carrier_freq_sib_nb_len
(
    rrm_oam_carrier_freq_sib_nb_t *p_rrm_oam_carrier_freq_sib_nb
);

static
oam_return_et
oam_compose_rrm_oam_carrier_freq_sib_nb
(
    U8  **pp_buffer,
    rrm_oam_carrier_freq_sib_nb_t *p_rrm_oam_carrier_freq_sib_nb
);

static
rrm_length_t
rrm_get_rrm_oam_freq_info_sib_nb_len
(
    rrm_oam_freq_info_sib_nb_t *p_rrm_oam_freq_info_sib_nb
);

static
oam_return_et
oam_compose_rrm_oam_freq_info_sib_nb
(
    U8  **pp_buffer,
    rrm_oam_freq_info_sib_nb_t *p_rrm_oam_freq_info_sib_nb
);

static
rrm_length_t
rrm_get_rrm_oam_multi_band_info_nb_len
(
    rrm_oam_multi_band_info_nb_t *p_rrm_oam_multi_band_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_multi_band_info_nb
(
    U8  **pp_buffer,
    rrm_oam_multi_band_info_nb_t *p_rrm_oam_multi_band_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_multiband_info_list_nb_len
(
    rrm_oam_multiband_info_list_nb_t *p_rrm_oam_multiband_info_list_nb
);

static
oam_return_et
oam_compose_rrm_oam_multiband_info_list_nb
(
    U8  **pp_buffer,
    rrm_oam_multiband_info_list_nb_t *p_rrm_oam_multiband_info_list_nb
);

static
rrm_length_t
rrm_get_rrm_oam_sib_type_2_info_nb_len
(
    rrm_oam_sib_type_2_info_nb_t *p_rrm_oam_sib_type_2_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_sib_type_2_info_nb
(
    U8  **pp_buffer,
    rrm_oam_sib_type_2_info_nb_t *p_rrm_oam_sib_type_2_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_admission_control_info_nb_len
(
    rrm_oam_admission_control_info_nb_t *p_rrm_oam_admission_control_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_admission_control_info_nb
(
    U8  **pp_buffer,
    rrm_oam_admission_control_info_nb_t *p_rrm_oam_admission_control_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_adl_pkt_scheduling_params_nb_len
(
    rrm_oam_adl_pkt_scheduling_params_nb_t *p_rrm_oam_adl_pkt_scheduling_params_nb
);

static
oam_return_et
oam_compose_rrm_oam_adl_pkt_scheduling_params_nb
(
    U8  **pp_buffer,
    rrm_oam_adl_pkt_scheduling_params_nb_t *p_rrm_oam_adl_pkt_scheduling_params_nb
);

static
rrm_length_t
rrm_get_rrm_oam_adl_cell_params_nb_len
(
    rrm_oam_adl_cell_params_nb_t *p_rrm_oam_adl_cell_params_nb
);

static
oam_return_et
oam_compose_rrm_oam_adl_cell_params_nb
(
    U8  **pp_buffer,
    rrm_oam_adl_cell_params_nb_t *p_rrm_oam_adl_cell_params_nb
);

static
rrm_length_t
rrm_get_rrm_oam_transmission_mode_table_nb_len
(
    rrm_oam_transmission_mode_table_nb_t *p_rrm_oam_transmission_mode_table_nb
);

static
oam_return_et
oam_compose_rrm_oam_transmission_mode_table_nb
(
    U8  **pp_buffer,
    rrm_oam_transmission_mode_table_nb_t *p_rrm_oam_transmission_mode_table_nb
);

static
rrm_length_t
rrm_get_rrm_oam_transmission_mode_params_nb_len
(
    rrm_oam_transmission_mode_params_nb_t *p_rrm_oam_transmission_mode_params_nb
);

static
oam_return_et
oam_compose_rrm_oam_transmission_mode_params_nb
(
    U8  **pp_buffer,
    rrm_oam_transmission_mode_params_nb_t *p_rrm_oam_transmission_mode_params_nb
);

static
rrm_length_t
rrm_get_rrm_oam_eutran_access_point_pos_len
(
    rrm_oam_eutran_access_point_pos_t *p_rrm_oam_eutran_access_point_pos
);

static
oam_return_et
oam_compose_rrm_oam_eutran_access_point_pos
(
    U8  **pp_buffer,
    rrm_oam_eutran_access_point_pos_t *p_rrm_oam_eutran_access_point_pos
);

static
rrm_length_t
rrm_get_rrm_oam_max_ue_schedule_dl_ul_len
(
    rrm_oam_max_ue_schedule_dl_ul_t *p_rrm_oam_max_ue_schedule_dl_ul
);

static
oam_return_et
oam_compose_rrm_oam_max_ue_schedule_dl_ul
(
    U8  **pp_buffer,
    rrm_oam_max_ue_schedule_dl_ul_t *p_rrm_oam_max_ue_schedule_dl_ul
);

static
rrm_length_t
rrm_get_rrm_oam_operator_info_nb_len
(
    rrm_oam_operator_info_nb_t *p_rrm_oam_operator_info_nb
);

static
oam_return_et
oam_compose_rrm_oam_operator_info_nb
(
    U8  **pp_buffer,
    rrm_oam_operator_info_nb_t *p_rrm_oam_operator_info_nb
);

static
rrm_length_t
rrm_get_rrm_oam_l2_comm_info_len
(
    rrm_oam_l2_comm_info_t *p_rrm_oam_l2_comm_info
);

static
oam_return_et
oam_compose_rrm_oam_l2_comm_info
(
    U8  **pp_buffer,
    rrm_oam_l2_comm_info_t *p_rrm_oam_l2_comm_info
);

static
rrm_length_t
rrm_get_rrm_oam_lower_layer_comm_info_len
(
    rrm_oam_lower_layer_comm_info_t *p_rrm_oam_lower_layer_comm_info
);

static
oam_return_et
oam_compose_rrm_oam_lower_layer_comm_info
(
    U8  **pp_buffer,
    rrm_oam_lower_layer_comm_info_t *p_rrm_oam_lower_layer_comm_info
);

static
rrm_length_t
rrm_get_rrm_oam_inband_different_pci_nb_r13_len
(
    rrm_oam_inband_different_pci_nb_r13_t *p_rrm_oam_inband_different_pci_nb_r13
);

static
oam_return_et
oam_compose_rrm_oam_inband_different_pci_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_inband_different_pci_nb_r13_t *p_rrm_oam_inband_different_pci_nb_r13
);

static
rrm_length_t
rrm_get_rrm_oam_guardband_nb_r13_len
(
    rrm_oam_guardband_nb_r13_t *p_rrm_oam_guardband_nb_r13
);

static
oam_return_et
oam_compose_rrm_oam_guardband_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_guardband_nb_r13_t *p_rrm_oam_guardband_nb_r13
);

static
rrm_length_t
rrm_get_rrm_oam_operation_mode_info_r13_len
(
    rrm_oam_operation_mode_info_r13_t *p_rrm_oam_operation_mode_info_r13
);

static
oam_return_et
oam_compose_rrm_oam_operation_mode_info_r13
(
    U8  **pp_buffer,
    rrm_oam_operation_mode_info_r13_t *p_rrm_oam_operation_mode_info_r13
);

static
rrm_length_t
rrm_get_rrm_oam_nb_cell_info_len
(
    rrm_oam_nb_cell_info_t *p_rrm_oam_nb_cell_info
);

static
oam_return_et
oam_compose_rrm_oam_nb_cell_info
(
    U8  **pp_buffer,
    rrm_oam_nb_cell_info_t *p_rrm_oam_nb_cell_info
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrm_length_t
rrm_get_rrm_oam_nb_cell_config_req_len
(
    rrm_oam_nb_cell_config_req_t *p_rrm_oam_nb_cell_config_req
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_nb_cell_config_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_config_req->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_cell_info_nb_len(&p_rrm_oam_nb_cell_config_req->global_cell_info_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_ran_info_nb_len(&p_rrm_oam_nb_cell_config_req->ran_info_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_epc_nb_len(&p_rrm_oam_nb_cell_config_req->epc_info_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_operator_info_nb_len(&p_rrm_oam_nb_cell_config_req->operator_info_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_config_req->immediate_start_needed_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_lower_layer_comm_info_len(&p_rrm_oam_nb_cell_config_req->l2_comm_info_params_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_nb_cell_info_len(&p_rrm_oam_nb_cell_config_req->nb_cell_info);

    return length;
}

oam_return_et
oam_compose_rrm_oam_nb_cell_config_req
(
    U8  **pp_buffer,
    rrm_oam_nb_cell_config_req_t *p_rrm_oam_nb_cell_config_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_nb_cell_config_req != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nb_cell_config_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_config_req->bitmask);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_cell_info_nb(pp_buffer, &p_rrm_oam_nb_cell_config_req->global_cell_info_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_ran_info_nb(pp_buffer, &p_rrm_oam_nb_cell_config_req->ran_info_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_epc_nb(pp_buffer, &p_rrm_oam_nb_cell_config_req->epc_info_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_operator_info_nb(pp_buffer, &p_rrm_oam_nb_cell_config_req->operator_info_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_nb_cell_config_req->immediate_start_needed_nb, "immediate_start_needed_nb");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_config_req->immediate_start_needed_nb);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_lower_layer_comm_info(pp_buffer, &p_rrm_oam_nb_cell_config_req->l2_comm_info_params_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_nb_cell_info(pp_buffer, &p_rrm_oam_nb_cell_config_req->nb_cell_info))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrmNb_send_rrm_oam_nb_cell_config_req
*
*   DESCRIPTION:
*       This function constructs and sends RRM_OAM_NB_CELL_CONFIG_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrmNb_send_rrm_oam_nb_cell_config_req
(
    rrm_oam_nb_cell_config_req_t  *p_rrm_oam_nb_cell_config_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_oamNb_msg;

    OAM_NULL_CHECK(p_rrm_oam_nb_cell_config_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_oam_nb_cell_config_req_len(p_rrm_oam_nb_cell_config_req);
    if(msg_length == OAM_FAILURE)
    {
        return OAM_FAILURE;
    }

    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRM_OAM_NB_CELL_CONFIG_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oamNb_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oamNb_msg, RRM_VERSION_ID, src_module_id,
        dst_module_id, RRM_OAM_NB_CELL_CONFIG_REQ, msg_api_length);

    /* Fill interface header */
    p_oamNb_msg = p_oamNb_msg + RRM_API_HEADER_SIZE;
    oam_construct_interface_api_header(p_oamNb_msg, transaction_id, src_module_id,
        dst_module_id, RRM_OAM_NB_CELL_CONFIG_REQ, msg_length,cell_index);

    /* Fill RRM_OAM_NB_CELL_CONFIG_REQ message */
    p_oamNb_msg = p_oamNb_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_compose_rrm_oam_nb_cell_config_req(&p_oamNb_msg, p_rrm_oam_nb_cell_config_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oamNb_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }
    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_cell_plmn_info_len
(
    rrm_oam_cell_plmn_info_t *p_rrm_oam_cell_plmn_info
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_cell_plmn_info != PNULL);

    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_oam_cell_plmn_info->mcc);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_cell_plmn_info->num_mnc_digit);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_oam_cell_plmn_info->num_mnc_digit * sizeof(p_rrm_oam_cell_plmn_info->mnc[0]));

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_cell_plmn_info
(
    U8  **pp_buffer,
    rrm_oam_cell_plmn_info_t *p_rrm_oam_cell_plmn_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_cell_plmn_info != PNULL);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_cell_plmn_info->mcc); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_cell_plmn_info->mcc[loop], "mcc[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_cell_plmn_info->num_mnc_digit < 2) || (p_rrm_oam_cell_plmn_info->num_mnc_digit > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_cell_plmn_info->num_mnc_digit] should be in range [2 <= %u <= 3] !", p_rrm_oam_cell_plmn_info->num_mnc_digit);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_cell_plmn_info->num_mnc_digit, "num_mnc_digit");
    *pp_buffer += sizeof(p_rrm_oam_cell_plmn_info->num_mnc_digit);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_cell_plmn_info->num_mnc_digit; loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_cell_plmn_info->mnc[loop], "mnc[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_eutran_global_cell_id_len
(
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_eutran_global_cell_id != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_cell_plmn_info_len(&p_rrm_oam_eutran_global_cell_id->primary_plmn_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_oam_eutran_global_cell_id->cell_identity);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_eutran_global_cell_id
(
    U8  **pp_buffer,
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_eutran_global_cell_id != PNULL);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_cell_plmn_info(pp_buffer, &p_rrm_oam_eutran_global_cell_id->primary_plmn_id))
    {
        return OAM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_eutran_global_cell_id->cell_identity); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_eutran_global_cell_id->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_cell_access_restriction_params_nb_len
(
    rrm_oam_cell_access_restriction_params_nb_t *p_rrm_oam_cell_access_restriction_params_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_cell_access_restriction_params_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_cell_access_restriction_params_nb->cell_barred);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_cell_access_restriction_params_nb->intra_freq_reselection);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_cell_access_restriction_params_nb
(
    U8  **pp_buffer,
    rrm_oam_cell_access_restriction_params_nb_t *p_rrm_oam_cell_access_restriction_params_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_cell_access_restriction_params_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_cell_access_restriction_params_nb->cell_barred > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_cell_access_restriction_params_nb->cell_barred] should be in range [%u <= 1] !", p_rrm_oam_cell_access_restriction_params_nb->cell_barred);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_cell_access_restriction_params_nb->cell_barred, "cell_barred");
    *pp_buffer += sizeof(p_rrm_oam_cell_access_restriction_params_nb->cell_barred);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_cell_access_restriction_params_nb->intra_freq_reselection > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_cell_access_restriction_params_nb->intra_freq_reselection] should be in range [%u <= 1] !", p_rrm_oam_cell_access_restriction_params_nb->intra_freq_reselection);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_cell_access_restriction_params_nb->intra_freq_reselection, "intra_freq_reselection");
    *pp_buffer += sizeof(p_rrm_oam_cell_access_restriction_params_nb->intra_freq_reselection);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_cell_info_nb_len
(
    rrm_oam_cell_info_nb_t *p_rrm_oam_cell_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_cell_info_nb != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_oam_cell_info_nb->eutran_global_cell_id);

    /* Get length of IE */
    length += rrm_get_rrm_oam_cell_access_restriction_params_nb_len(&p_rrm_oam_cell_info_nb->cell_access_restriction_params_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_cell_info_nb
(
    U8  **pp_buffer,
    rrm_oam_cell_info_nb_t *p_rrm_oam_cell_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_cell_info_nb != PNULL);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_oam_cell_info_nb->eutran_global_cell_id))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_cell_access_restriction_params_nb(pp_buffer, &p_rrm_oam_cell_info_nb->cell_access_restriction_params_nb))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_rsrp_thresholds_prach_info_list_r13_len
(
    rrm_oam_rsrp_thresholds_prach_info_list_r13_t *p_rrm_oam_rsrp_thresholds_prach_info_list_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_rsrp_thresholds_prach_info_list_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count * sizeof(p_rrm_oam_rsrp_thresholds_prach_info_list_r13->rsrp_range_nb[0]));

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_rsrp_thresholds_prach_info_list_r13
(
    U8  **pp_buffer,
    rrm_oam_rsrp_thresholds_prach_info_list_r13_t *p_rrm_oam_rsrp_thresholds_prach_info_list_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_rsrp_thresholds_prach_info_list_r13 != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count < 1) || (p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count > MAX_RSRP_COUNT_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count] should be in range [1 <= %u <= MAX_RSRP_COUNT_NB] !", p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count, "count");
    *pp_buffer += sizeof(p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count; loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rsrp_thresholds_prach_info_list_r13->rsrp_range_nb[loop], "rsrp_range_nb[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_nprach_parameters_nb_r13_len
(
    rrm_oam_nprach_parameters_nb_r13_t *p_rrm_oam_nprach_parameters_nb_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_nprach_parameters_nb_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_parameters_nb_r13->nprach_periodicity_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_parameters_nb_r13->nprach_start_time_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_offset_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_parameters_nb_r13->nprach_num_subcarriers_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_msg3_range_start_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_parameters_nb_r13->max_num_preamble_attempt_ce_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_parameters_nb_r13->num_repetitions_per_preamble_attempt_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_parameters_nb_r13->npdcch_num_repetitions_ra_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_parameters_nb_r13->npdcch_start_sf_css_ra_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_parameters_nb_r13->npdcch_offset_css_ra_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_nprach_parameters_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_nprach_parameters_nb_r13_t *p_rrm_oam_nprach_parameters_nb_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_nprach_parameters_nb_r13 != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->nprach_periodicity_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->nprach_periodicity_r13] should be in range [%u <= 7] !", p_rrm_oam_nprach_parameters_nb_r13->nprach_periodicity_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_parameters_nb_r13->nprach_periodicity_r13, "nprach_periodicity_r13");
    *pp_buffer += sizeof(p_rrm_oam_nprach_parameters_nb_r13->nprach_periodicity_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->nprach_start_time_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->nprach_start_time_r13] should be in range [%u <= 7] !", p_rrm_oam_nprach_parameters_nb_r13->nprach_start_time_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_parameters_nb_r13->nprach_start_time_r13, "nprach_start_time_r13");
    *pp_buffer += sizeof(p_rrm_oam_nprach_parameters_nb_r13->nprach_start_time_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_offset_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_offset_r13] should be in range [%u <= 6] !", p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_offset_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_offset_r13, "nprach_subcarrier_offset_r13");
    *pp_buffer += sizeof(p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_offset_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->nprach_num_subcarriers_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->nprach_num_subcarriers_r13] should be in range [%u <= 3] !", p_rrm_oam_nprach_parameters_nb_r13->nprach_num_subcarriers_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_parameters_nb_r13->nprach_num_subcarriers_r13, "nprach_num_subcarriers_r13");
    *pp_buffer += sizeof(p_rrm_oam_nprach_parameters_nb_r13->nprach_num_subcarriers_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_msg3_range_start_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_msg3_range_start_r13] should be in range [%u <= 3] !", p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_msg3_range_start_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_msg3_range_start_r13, "nprach_subcarrier_msg3_range_start_r13");
    *pp_buffer += sizeof(p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_msg3_range_start_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->max_num_preamble_attempt_ce_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->max_num_preamble_attempt_ce_r13] should be in range [%u <= 6] !", p_rrm_oam_nprach_parameters_nb_r13->max_num_preamble_attempt_ce_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_parameters_nb_r13->max_num_preamble_attempt_ce_r13, "max_num_preamble_attempt_ce_r13");
    *pp_buffer += sizeof(p_rrm_oam_nprach_parameters_nb_r13->max_num_preamble_attempt_ce_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->num_repetitions_per_preamble_attempt_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->num_repetitions_per_preamble_attempt_r13] should be in range [%u <= 7] !", p_rrm_oam_nprach_parameters_nb_r13->num_repetitions_per_preamble_attempt_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_parameters_nb_r13->num_repetitions_per_preamble_attempt_r13, "num_repetitions_per_preamble_attempt_r13");
    *pp_buffer += sizeof(p_rrm_oam_nprach_parameters_nb_r13->num_repetitions_per_preamble_attempt_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->npdcch_num_repetitions_ra_r13 > 11))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->npdcch_num_repetitions_ra_r13] should be in range [%u <= 11] !", p_rrm_oam_nprach_parameters_nb_r13->npdcch_num_repetitions_ra_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_parameters_nb_r13->npdcch_num_repetitions_ra_r13, "npdcch_num_repetitions_ra_r13");
    *pp_buffer += sizeof(p_rrm_oam_nprach_parameters_nb_r13->npdcch_num_repetitions_ra_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->npdcch_start_sf_css_ra_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->npdcch_start_sf_css_ra_r13] should be in range [%u <= 7] !", p_rrm_oam_nprach_parameters_nb_r13->npdcch_start_sf_css_ra_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_parameters_nb_r13->npdcch_start_sf_css_ra_r13, "npdcch_start_sf_css_ra_r13");
    *pp_buffer += sizeof(p_rrm_oam_nprach_parameters_nb_r13->npdcch_start_sf_css_ra_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->npdcch_offset_css_ra_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->npdcch_offset_css_ra_r13] should be in range [%u <= 3] !", p_rrm_oam_nprach_parameters_nb_r13->npdcch_offset_css_ra_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_parameters_nb_r13->npdcch_offset_css_ra_r13, "npdcch_offset_css_ra_r13");
    *pp_buffer += sizeof(p_rrm_oam_nprach_parameters_nb_r13->npdcch_offset_css_ra_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_nprach_parameter_list_nb_r13_len
(
    rrm_oam_nprach_parameter_list_nb_r13_t *p_rrm_oam_nprach_parameter_list_nb_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_nprach_parameter_list_nb_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_parameter_list_nb_r13->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_nprach_parameter_list_nb_r13->count < 1) || (p_rrm_oam_nprach_parameter_list_nb_r13->count > MAX_NPRACH_RESOURCES_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameter_list_nb_r13->count] should be in range [1 <= %d <= MAX_NPRACH_RESOURCES_NB_R13] !", p_rrm_oam_nprach_parameter_list_nb_r13->count);
#endif
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_nprach_parameter_list_nb_r13->count; loop++)
        {
            length += rrm_get_rrm_oam_nprach_parameters_nb_r13_len(&p_rrm_oam_nprach_parameter_list_nb_r13->nprach_parameters_nb_r13[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_nprach_parameter_list_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_nprach_parameter_list_nb_r13_t *p_rrm_oam_nprach_parameter_list_nb_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_nprach_parameter_list_nb_r13 != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_nprach_parameter_list_nb_r13->count < 1) || (p_rrm_oam_nprach_parameter_list_nb_r13->count > MAX_NPRACH_RESOURCES_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameter_list_nb_r13->count] should be in range [1 <= %u <= MAX_NPRACH_RESOURCES_NB_R13] !", p_rrm_oam_nprach_parameter_list_nb_r13->count);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_parameter_list_nb_r13->count, "count");
    *pp_buffer += sizeof(p_rrm_oam_nprach_parameter_list_nb_r13->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_nprach_parameter_list_nb_r13->count; loop++)
        {
            if (OAM_FAILURE == oam_compose_rrm_oam_nprach_parameters_nb_r13(pp_buffer, &p_rrm_oam_nprach_parameter_list_nb_r13->nprach_parameters_nb_r13[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_prach_nb_len
(
    rrm_oam_prach_nb_t *p_rrm_oam_prach_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_prach_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_prach_nb->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_prach_nb->nprach_cp_length_r13);

    /* Optional element */
    if(p_rrm_oam_prach_nb->bitmask & RRM_OAM_RSRP_THRESHOLD_PRACH_INFO_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_rsrp_thresholds_prach_info_list_r13_len(&p_rrm_oam_prach_nb->rsrp_thresholds_prach_info_list_r13);
    }

    /* Get length of IE */
    length += rrm_get_rrm_oam_nprach_parameter_list_nb_r13_len(&p_rrm_oam_prach_nb->nprach_parameters_list_nb_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_prach_nb
(
    U8  **pp_buffer,
    rrm_oam_prach_nb_t *p_rrm_oam_prach_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_prach_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_prach_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_prach_nb->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_prach_nb->nprach_cp_length_r13 > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_prach_nb->nprach_cp_length_r13] should be in range [%u <= 1] !", p_rrm_oam_prach_nb->nprach_cp_length_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_prach_nb->nprach_cp_length_r13, "nprach_cp_length_r13");
    *pp_buffer += sizeof(p_rrm_oam_prach_nb->nprach_cp_length_r13);

    /* Optional element */
    if(p_rrm_oam_prach_nb->bitmask & RRM_OAM_RSRP_THRESHOLD_PRACH_INFO_LIST_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_rsrp_thresholds_prach_info_list_r13(pp_buffer, &p_rrm_oam_prach_nb->rsrp_thresholds_prach_info_list_r13))
    {
        return OAM_FAILURE;
    }
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_nprach_parameter_list_nb_r13(pp_buffer, &p_rrm_oam_prach_nb->nprach_parameters_list_nb_r13))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_nprach_parameters_list_nb_v1330_len
(
    rrm_oam_nprach_parameters_list_nb_v1330_t *p_rrm_oam_nprach_parameters_list_nb_v1330
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_nprach_parameters_list_nb_v1330 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_parameters_list_nb_v1330->nprach_num_cbra_start_subcarriers_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_nprach_parameters_list_nb_v1330
(
    U8  **pp_buffer,
    rrm_oam_nprach_parameters_list_nb_v1330_t *p_rrm_oam_nprach_parameters_list_nb_v1330
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_nprach_parameters_list_nb_v1330 != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_list_nb_v1330->nprach_num_cbra_start_subcarriers_r13 > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_list_nb_v1330->nprach_num_cbra_start_subcarriers_r13] should be in range [%u <= 15] !", p_rrm_oam_nprach_parameters_list_nb_v1330->nprach_num_cbra_start_subcarriers_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_parameters_list_nb_v1330->nprach_num_cbra_start_subcarriers_r13, "nprach_num_cbra_start_subcarriers_r13");
    *pp_buffer += sizeof(p_rrm_oam_nprach_parameters_list_nb_v1330->nprach_num_cbra_start_subcarriers_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_nprach_config_sib_nb_v1330_len
(
    rrm_oam_nprach_config_sib_nb_v1330_t *p_rrm_oam_nprach_config_sib_nb_v1330
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_nprach_config_sib_nb_v1330 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nprach_config_sib_nb_v1330->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_nprach_config_sib_nb_v1330->count < 1) || (p_rrm_oam_nprach_config_sib_nb_v1330->count > MAX_NPRACH_RESOURCES_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_config_sib_nb_v1330->count] should be in range [1 <= %d <= MAX_NPRACH_RESOURCES_NB_R13] !", p_rrm_oam_nprach_config_sib_nb_v1330->count);
#endif
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_nprach_config_sib_nb_v1330->count; loop++)
        {
            length += rrm_get_rrm_oam_nprach_parameters_list_nb_v1330_len(&p_rrm_oam_nprach_config_sib_nb_v1330->nprach_parameters_list_v1330[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_nprach_config_sib_nb_v1330
(
    U8  **pp_buffer,
    rrm_oam_nprach_config_sib_nb_v1330_t *p_rrm_oam_nprach_config_sib_nb_v1330
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_nprach_config_sib_nb_v1330 != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_nprach_config_sib_nb_v1330->count < 1) || (p_rrm_oam_nprach_config_sib_nb_v1330->count > MAX_NPRACH_RESOURCES_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_config_sib_nb_v1330->count] should be in range [1 <= %u <= MAX_NPRACH_RESOURCES_NB_R13] !", p_rrm_oam_nprach_config_sib_nb_v1330->count);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nprach_config_sib_nb_v1330->count, "count");
    *pp_buffer += sizeof(p_rrm_oam_nprach_config_sib_nb_v1330->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_nprach_config_sib_nb_v1330->count; loop++)
        {
            if (OAM_FAILURE == oam_compose_rrm_oam_nprach_parameters_list_nb_v1330(pp_buffer, &p_rrm_oam_nprach_config_sib_nb_v1330->nprach_parameters_list_v1330[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_npdcch_config_dedicated_nb_r13_len
(
    rrm_oam_npdcch_config_dedicated_nb_r13_t *p_rrm_oam_npdcch_config_dedicated_nb_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_npdcch_config_dedicated_nb_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_num_repetitions_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_start_sf_uss_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_npdcch_config_dedicated_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_npdcch_config_dedicated_nb_r13_t *p_rrm_oam_npdcch_config_dedicated_nb_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_npdcch_config_dedicated_nb_r13 != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_num_repetitions_r13 > 11))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_num_repetitions_r13] should be in range [%u <= 11] !", p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_num_repetitions_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_num_repetitions_r13, "npdcch_num_repetitions_r13");
    *pp_buffer += sizeof(p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_num_repetitions_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_start_sf_uss_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_start_sf_uss_r13] should be in range [%u <= 7] !", p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_start_sf_uss_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_start_sf_uss_r13, "npdcch_start_sf_uss_r13");
    *pp_buffer += sizeof(p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_start_sf_uss_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_ack_nack_num_repetitions_msg4_r13_len
(
    rrm_oam_ack_nack_num_repetitions_msg4_r13_t *p_rrm_oam_ack_nack_num_repetitions_msg4_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_ack_nack_num_repetitions_msg4_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count * sizeof(p_rrm_oam_ack_nack_num_repetitions_msg4_r13->ack_nack_num_repetitions_nb_r13[0]));

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_ack_nack_num_repetitions_msg4_r13
(
    U8  **pp_buffer,
    rrm_oam_ack_nack_num_repetitions_msg4_r13_t *p_rrm_oam_ack_nack_num_repetitions_msg4_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_ack_nack_num_repetitions_msg4_r13 != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count > MAX_NPRACH_RESOURCES_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count] should be in range [%u <= MAX_NPRACH_RESOURCES_NB_R13] !", p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count, "count");
    *pp_buffer += sizeof(p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count; loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_ack_nack_num_repetitions_msg4_r13->ack_nack_num_repetitions_nb_r13[loop], "ack_nack_num_repetitions_nb_r13[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_dmrs_config_r13_len
(
    rrm_oam_dmrs_config_r13_t *p_rrm_oam_dmrs_config_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_dmrs_config_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_dmrs_config_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_dmrs_config_r13->bitmask & RRM_OAM_THREE_TONE_BASE_SEQ_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_dmrs_config_r13->three_tone_base_sequence_r13);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_dmrs_config_r13->three_tone_cyclic_shift_r13);

    /* Optional element */
    if(p_rrm_oam_dmrs_config_r13->bitmask & RRM_OAM_SIX_TON_SEQ_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_dmrs_config_r13->six_tone_base_sequence_r13);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_dmrs_config_r13->six_tone_cyclic_shift_r13);

    /* Optional element */
    if(p_rrm_oam_dmrs_config_r13->bitmask & RRM_OAM_TWELVE_TONE_SEQ_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_dmrs_config_r13->twelve_tone_base_sequence_r13);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_dmrs_config_r13
(
    U8  **pp_buffer,
    rrm_oam_dmrs_config_r13_t *p_rrm_oam_dmrs_config_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_dmrs_config_r13 != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_dmrs_config_r13->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_dmrs_config_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_dmrs_config_r13->bitmask & RRM_OAM_THREE_TONE_BASE_SEQ_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dmrs_config_r13->three_tone_base_sequence_r13 > 12))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dmrs_config_r13->three_tone_base_sequence_r13] should be in range [%u <= 12] !", p_rrm_oam_dmrs_config_r13->three_tone_base_sequence_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_dmrs_config_r13->three_tone_base_sequence_r13, "three_tone_base_sequence_r13");
    *pp_buffer += sizeof(p_rrm_oam_dmrs_config_r13->three_tone_base_sequence_r13);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dmrs_config_r13->three_tone_cyclic_shift_r13 > 2))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dmrs_config_r13->three_tone_cyclic_shift_r13] should be in range [%u <= 2] !", p_rrm_oam_dmrs_config_r13->three_tone_cyclic_shift_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_dmrs_config_r13->three_tone_cyclic_shift_r13, "three_tone_cyclic_shift_r13");
    *pp_buffer += sizeof(p_rrm_oam_dmrs_config_r13->three_tone_cyclic_shift_r13);

    /* Optional element */
    if(p_rrm_oam_dmrs_config_r13->bitmask & RRM_OAM_SIX_TON_SEQ_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dmrs_config_r13->six_tone_base_sequence_r13 > 14))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dmrs_config_r13->six_tone_base_sequence_r13] should be in range [%u <= 14] !", p_rrm_oam_dmrs_config_r13->six_tone_base_sequence_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_dmrs_config_r13->six_tone_base_sequence_r13, "six_tone_base_sequence_r13");
    *pp_buffer += sizeof(p_rrm_oam_dmrs_config_r13->six_tone_base_sequence_r13);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dmrs_config_r13->six_tone_cyclic_shift_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dmrs_config_r13->six_tone_cyclic_shift_r13] should be in range [%u <= 3] !", p_rrm_oam_dmrs_config_r13->six_tone_cyclic_shift_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_dmrs_config_r13->six_tone_cyclic_shift_r13, "six_tone_cyclic_shift_r13");
    *pp_buffer += sizeof(p_rrm_oam_dmrs_config_r13->six_tone_cyclic_shift_r13);

    /* Optional element */
    if(p_rrm_oam_dmrs_config_r13->bitmask & RRM_OAM_TWELVE_TONE_SEQ_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dmrs_config_r13->twelve_tone_base_sequence_r13 > 30))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dmrs_config_r13->twelve_tone_base_sequence_r13] should be in range [%u <= 30] !", p_rrm_oam_dmrs_config_r13->twelve_tone_base_sequence_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_dmrs_config_r13->twelve_tone_base_sequence_r13, "twelve_tone_base_sequence_r13");
    *pp_buffer += sizeof(p_rrm_oam_dmrs_config_r13->twelve_tone_base_sequence_r13);
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_ul_reference_signal_npusch_nb_len
(
    rrm_oam_ul_reference_signal_npusch_nb_t *p_rrm_oam_ul_reference_signal_npusch_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_ul_reference_signal_npusch_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_ul_reference_signal_npusch_nb->group_assignment_npusch_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_ul_reference_signal_npusch_nb->group_hopping_enabled_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_ul_reference_signal_npusch_nb
(
    U8  **pp_buffer,
    rrm_oam_ul_reference_signal_npusch_nb_t *p_rrm_oam_ul_reference_signal_npusch_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_ul_reference_signal_npusch_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_ul_reference_signal_npusch_nb->group_assignment_npusch_r13 > 29))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ul_reference_signal_npusch_nb->group_assignment_npusch_r13] should be in range [%u <= 29] !", p_rrm_oam_ul_reference_signal_npusch_nb->group_assignment_npusch_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_ul_reference_signal_npusch_nb->group_assignment_npusch_r13, "group_assignment_npusch_r13");
    *pp_buffer += sizeof(p_rrm_oam_ul_reference_signal_npusch_nb->group_assignment_npusch_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_ul_reference_signal_npusch_nb->group_hopping_enabled_r13 > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ul_reference_signal_npusch_nb->group_hopping_enabled_r13] should be in range [%u <= 1] !", p_rrm_oam_ul_reference_signal_npusch_nb->group_hopping_enabled_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_ul_reference_signal_npusch_nb->group_hopping_enabled_r13, "group_hopping_enabled_r13");
    *pp_buffer += sizeof(p_rrm_oam_ul_reference_signal_npusch_nb->group_hopping_enabled_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_npusch_config_dedicated_r13_len
(
    rrm_oam_npusch_config_dedicated_r13_t *p_rrm_oam_npusch_config_dedicated_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_npusch_config_dedicated_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_npusch_config_dedicated_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_npusch_config_dedicated_r13->bitmask & RRM_OAM_ACK_NACK_NUM_REPETITION_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_npusch_config_dedicated_r13->ack_nack_num_repetitions_r13);
    }

    /* Optional element */
    if(p_rrm_oam_npusch_config_dedicated_r13->bitmask & RRM_OAM_NPUSCH_ALL_SYMBOLS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_npusch_config_dedicated_r13->npusch_all_symbols_r13);
    }

    /* Optional element */
    if(p_rrm_oam_npusch_config_dedicated_r13->bitmask & RRM_OAM_GROUP_HOPPING_DISABLED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_npusch_config_dedicated_r13->group_hopping_disabled_r13);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_npusch_config_dedicated_r13
(
    U8  **pp_buffer,
    rrm_oam_npusch_config_dedicated_r13_t *p_rrm_oam_npusch_config_dedicated_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_npusch_config_dedicated_r13 != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_npusch_config_dedicated_r13->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_npusch_config_dedicated_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_npusch_config_dedicated_r13->bitmask & RRM_OAM_ACK_NACK_NUM_REPETITION_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_npusch_config_dedicated_r13->ack_nack_num_repetitions_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_npusch_config_dedicated_r13->ack_nack_num_repetitions_r13] should be in range [%u <= 7] !", p_rrm_oam_npusch_config_dedicated_r13->ack_nack_num_repetitions_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_npusch_config_dedicated_r13->ack_nack_num_repetitions_r13, "ack_nack_num_repetitions_r13");
    *pp_buffer += sizeof(p_rrm_oam_npusch_config_dedicated_r13->ack_nack_num_repetitions_r13);
    }

    /* Optional element */
    if(p_rrm_oam_npusch_config_dedicated_r13->bitmask & RRM_OAM_NPUSCH_ALL_SYMBOLS_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_npusch_config_dedicated_r13->npusch_all_symbols_r13 > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_npusch_config_dedicated_r13->npusch_all_symbols_r13] should be in range [%u <= 1] !", p_rrm_oam_npusch_config_dedicated_r13->npusch_all_symbols_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_npusch_config_dedicated_r13->npusch_all_symbols_r13, "npusch_all_symbols_r13");
    *pp_buffer += sizeof(p_rrm_oam_npusch_config_dedicated_r13->npusch_all_symbols_r13);
    }

    /* Optional element */
    if(p_rrm_oam_npusch_config_dedicated_r13->bitmask & RRM_OAM_GROUP_HOPPING_DISABLED_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_npusch_config_dedicated_r13->group_hopping_disabled_r13 > 0))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_npusch_config_dedicated_r13->group_hopping_disabled_r13] should be in range [%u <= 0] !", p_rrm_oam_npusch_config_dedicated_r13->group_hopping_disabled_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_npusch_config_dedicated_r13->group_hopping_disabled_r13, "group_hopping_disabled_r13");
    *pp_buffer += sizeof(p_rrm_oam_npusch_config_dedicated_r13->group_hopping_disabled_r13);
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_physical_layer_param_pusch_nb_len
(
    rrm_oam_physical_layer_param_pusch_nb_t *p_rrm_oam_physical_layer_param_pusch_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_physical_layer_param_pusch_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_physical_layer_param_pusch_nb->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_ack_nack_num_repetitions_msg4_r13_len(&p_rrm_oam_physical_layer_param_pusch_nb->ack_nack_num_repetitions_msg4_r13);

    /* Optional element */
    if(p_rrm_oam_physical_layer_param_pusch_nb->bitmask & RRM_OAM_SRS_SUBFRAME_CONFIG_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_physical_layer_param_pusch_nb->srs_subframe_config_r13);
    }

    /* Optional element */
    if(p_rrm_oam_physical_layer_param_pusch_nb->bitmask & RRM_OAM_DMRS_CONFIG_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_dmrs_config_r13_len(&p_rrm_oam_physical_layer_param_pusch_nb->dmrs_config_r13);
    }

    /* Get length of IE */
    length += rrm_get_rrm_oam_ul_reference_signal_npusch_nb_len(&p_rrm_oam_physical_layer_param_pusch_nb->ul_reference_signal_npusch_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_npusch_config_dedicated_r13_len(&p_rrm_oam_physical_layer_param_pusch_nb->npusch_config_dedicated_nb_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_physical_layer_param_pusch_nb
(
    U8  **pp_buffer,
    rrm_oam_physical_layer_param_pusch_nb_t *p_rrm_oam_physical_layer_param_pusch_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_physical_layer_param_pusch_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_physical_layer_param_pusch_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_physical_layer_param_pusch_nb->bitmask);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_ack_nack_num_repetitions_msg4_r13(pp_buffer, &p_rrm_oam_physical_layer_param_pusch_nb->ack_nack_num_repetitions_msg4_r13))
    {
        return OAM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_oam_physical_layer_param_pusch_nb->bitmask & RRM_OAM_SRS_SUBFRAME_CONFIG_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_physical_layer_param_pusch_nb->srs_subframe_config_r13 > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_physical_layer_param_pusch_nb->srs_subframe_config_r13] should be in range [%u <= 15] !", p_rrm_oam_physical_layer_param_pusch_nb->srs_subframe_config_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_physical_layer_param_pusch_nb->srs_subframe_config_r13, "srs_subframe_config_r13");
    *pp_buffer += sizeof(p_rrm_oam_physical_layer_param_pusch_nb->srs_subframe_config_r13);
    }

    /* Optional element */
    if(p_rrm_oam_physical_layer_param_pusch_nb->bitmask & RRM_OAM_DMRS_CONFIG_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_dmrs_config_r13(pp_buffer, &p_rrm_oam_physical_layer_param_pusch_nb->dmrs_config_r13))
    {
        return OAM_FAILURE;
    }
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_ul_reference_signal_npusch_nb(pp_buffer, &p_rrm_oam_physical_layer_param_pusch_nb->ul_reference_signal_npusch_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_npusch_config_dedicated_r13(pp_buffer, &p_rrm_oam_physical_layer_param_pusch_nb->npusch_config_dedicated_nb_r13))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_uplink_power_control_nb_len
(
    rrm_oam_uplink_power_control_nb_t *p_rrm_oam_uplink_power_control_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_uplink_power_control_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_uplink_power_control_nb->p0_nominal_npusch_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_uplink_power_control_nb->alpha_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_uplink_power_control_nb->delta_preamble_msg3_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_uplink_power_control_nb->p0_ue_npusch_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_uplink_power_control_nb
(
    U8  **pp_buffer,
    rrm_oam_uplink_power_control_nb_t *p_rrm_oam_uplink_power_control_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_uplink_power_control_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_uplink_power_control_nb->p0_nominal_npusch_r13 > 24))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_uplink_power_control_nb->p0_nominal_npusch_r13] should be in range [%d <= 24] !", p_rrm_oam_uplink_power_control_nb->p0_nominal_npusch_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT8(*pp_buffer, &p_rrm_oam_uplink_power_control_nb->p0_nominal_npusch_r13, "p0_nominal_npusch_r13");
    *pp_buffer += sizeof(p_rrm_oam_uplink_power_control_nb->p0_nominal_npusch_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_uplink_power_control_nb->alpha_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_uplink_power_control_nb->alpha_r13] should be in range [%u <= 7] !", p_rrm_oam_uplink_power_control_nb->alpha_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_uplink_power_control_nb->alpha_r13, "alpha_r13");
    *pp_buffer += sizeof(p_rrm_oam_uplink_power_control_nb->alpha_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_uplink_power_control_nb->delta_preamble_msg3_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_uplink_power_control_nb->delta_preamble_msg3_r13] should be in range [%d <= 6] !", p_rrm_oam_uplink_power_control_nb->delta_preamble_msg3_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT8(*pp_buffer, &p_rrm_oam_uplink_power_control_nb->delta_preamble_msg3_r13, "delta_preamble_msg3_r13");
    *pp_buffer += sizeof(p_rrm_oam_uplink_power_control_nb->delta_preamble_msg3_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_uplink_power_control_nb->p0_ue_npusch_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_uplink_power_control_nb->p0_ue_npusch_r13] should be in range [%d <= 7] !", p_rrm_oam_uplink_power_control_nb->p0_ue_npusch_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT8(*pp_buffer, &p_rrm_oam_uplink_power_control_nb->p0_ue_npusch_r13, "p0_ue_npusch_r13");
    *pp_buffer += sizeof(p_rrm_oam_uplink_power_control_nb->p0_ue_npusch_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_dl_gap_config_nb_len
(
    rrm_oam_dl_gap_config_nb_t *p_rrm_oam_dl_gap_config_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_dl_gap_config_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_dl_gap_config_nb->dl_gap_threshold_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_dl_gap_config_nb->dl_gap_periodicity_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_dl_gap_config_nb->dl_gap_duration_coeff_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_dl_gap_config_nb
(
    U8  **pp_buffer,
    rrm_oam_dl_gap_config_nb_t *p_rrm_oam_dl_gap_config_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_dl_gap_config_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dl_gap_config_nb->dl_gap_threshold_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dl_gap_config_nb->dl_gap_threshold_r13] should be in range [%u <= 3] !", p_rrm_oam_dl_gap_config_nb->dl_gap_threshold_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_dl_gap_config_nb->dl_gap_threshold_r13, "dl_gap_threshold_r13");
    *pp_buffer += sizeof(p_rrm_oam_dl_gap_config_nb->dl_gap_threshold_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dl_gap_config_nb->dl_gap_periodicity_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dl_gap_config_nb->dl_gap_periodicity_r13] should be in range [%u <= 3] !", p_rrm_oam_dl_gap_config_nb->dl_gap_periodicity_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_dl_gap_config_nb->dl_gap_periodicity_r13, "dl_gap_periodicity_r13");
    *pp_buffer += sizeof(p_rrm_oam_dl_gap_config_nb->dl_gap_periodicity_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dl_gap_config_nb->dl_gap_duration_coeff_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dl_gap_config_nb->dl_gap_duration_coeff_r13] should be in range [%u <= 3] !", p_rrm_oam_dl_gap_config_nb->dl_gap_duration_coeff_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_dl_gap_config_nb->dl_gap_duration_coeff_r13, "dl_gap_duration_coeff_r13");
    *pp_buffer += sizeof(p_rrm_oam_dl_gap_config_nb->dl_gap_duration_coeff_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_physical_layer_params_nb_len
(
    rrm_oam_physical_layer_params_nb_t *p_rrm_oam_physical_layer_params_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_physical_layer_params_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_physical_layer_params_nb->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_prach_nb_len(&p_rrm_oam_physical_layer_params_nb->physical_layer_param_prach_nb);

    /* Optional element */
    if(p_rrm_oam_physical_layer_params_nb->bitmask & RRM_OAM_NPRACH_CONFIG_V1330_NB_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_nprach_config_sib_nb_v1330_len(&p_rrm_oam_physical_layer_params_nb->nprach_config_v1330);
    }

    /* Get length of IE */
    length += rrm_get_rrm_oam_npdcch_config_dedicated_nb_r13_len(&p_rrm_oam_physical_layer_params_nb->npdcch_config_dedicated_nb_r13);

    /* Get length of IE */
    length += rrm_get_rrm_oam_physical_layer_param_pusch_nb_len(&p_rrm_oam_physical_layer_params_nb->physical_layer_param_pusch_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_uplink_power_control_nb_len(&p_rrm_oam_physical_layer_params_nb->physical_layer_param_ul_power_control_nb);

    /* Optional element */
    if(p_rrm_oam_physical_layer_params_nb->bitmask & RRM_OAM_DL_GAP_CONFIG_NB_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_dl_gap_config_nb_len(&p_rrm_oam_physical_layer_params_nb->dl_gap_config_nb_r13);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_physical_layer_params_nb->nb_tx_antenna_port_count);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_physical_layer_params_nb->nb_rx_antenna_port_count);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_physical_layer_params_nb
(
    U8  **pp_buffer,
    rrm_oam_physical_layer_params_nb_t *p_rrm_oam_physical_layer_params_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_physical_layer_params_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_physical_layer_params_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_physical_layer_params_nb->bitmask);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_prach_nb(pp_buffer, &p_rrm_oam_physical_layer_params_nb->physical_layer_param_prach_nb))
    {
        return OAM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_oam_physical_layer_params_nb->bitmask & RRM_OAM_NPRACH_CONFIG_V1330_NB_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_nprach_config_sib_nb_v1330(pp_buffer, &p_rrm_oam_physical_layer_params_nb->nprach_config_v1330))
    {
        return OAM_FAILURE;
    }
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_npdcch_config_dedicated_nb_r13(pp_buffer, &p_rrm_oam_physical_layer_params_nb->npdcch_config_dedicated_nb_r13))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_physical_layer_param_pusch_nb(pp_buffer, &p_rrm_oam_physical_layer_params_nb->physical_layer_param_pusch_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_uplink_power_control_nb(pp_buffer, &p_rrm_oam_physical_layer_params_nb->physical_layer_param_ul_power_control_nb))
    {
        return OAM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_oam_physical_layer_params_nb->bitmask & RRM_OAM_DL_GAP_CONFIG_NB_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_dl_gap_config_nb(pp_buffer, &p_rrm_oam_physical_layer_params_nb->dl_gap_config_nb_r13))
    {
        return OAM_FAILURE;
    }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_physical_layer_params_nb->nb_tx_antenna_port_count < 1) || (p_rrm_oam_physical_layer_params_nb->nb_tx_antenna_port_count > 2))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_physical_layer_params_nb->nb_tx_antenna_port_count] should be in range [1 <= %u <= 2] !", p_rrm_oam_physical_layer_params_nb->nb_tx_antenna_port_count);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_physical_layer_params_nb->nb_tx_antenna_port_count, "nb_tx_antenna_port_count");
    *pp_buffer += sizeof(p_rrm_oam_physical_layer_params_nb->nb_tx_antenna_port_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_physical_layer_params_nb->nb_rx_antenna_port_count < 1) || (p_rrm_oam_physical_layer_params_nb->nb_rx_antenna_port_count > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_physical_layer_params_nb->nb_rx_antenna_port_count] should be in range [1 <= %u <= 1] !", p_rrm_oam_physical_layer_params_nb->nb_rx_antenna_port_count);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_physical_layer_params_nb->nb_rx_antenna_port_count, "nb_rx_antenna_port_count");
    *pp_buffer += sizeof(p_rrm_oam_physical_layer_params_nb->nb_rx_antenna_port_count);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_power_ramping_params_len
(
    rrm_oam_power_ramping_params_t *p_rrm_oam_power_ramping_params
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_power_ramping_params != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_power_ramping_params->power_ramping_step_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_power_ramping_params->preamble_initial_received_target_power_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_power_ramping_params
(
    U8  **pp_buffer,
    rrm_oam_power_ramping_params_t *p_rrm_oam_power_ramping_params
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_power_ramping_params != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_power_ramping_params->power_ramping_step_nb > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_power_ramping_params->power_ramping_step_nb] should be in range [%u <= 3] !", p_rrm_oam_power_ramping_params->power_ramping_step_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_power_ramping_params->power_ramping_step_nb, "power_ramping_step_nb");
    *pp_buffer += sizeof(p_rrm_oam_power_ramping_params->power_ramping_step_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_power_ramping_params->preamble_initial_received_target_power_nb > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_power_ramping_params->preamble_initial_received_target_power_nb] should be in range [%u <= 15] !", p_rrm_oam_power_ramping_params->preamble_initial_received_target_power_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_power_ramping_params->preamble_initial_received_target_power_nb, "preamble_initial_received_target_power_nb");
    *pp_buffer += sizeof(p_rrm_oam_power_ramping_params->preamble_initial_received_target_power_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_rach_info_len
(
    rrm_oam_rach_info_t *p_rrm_oam_rach_info
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_rach_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rach_info->ra_response_window_size_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rach_info->mac_contention_resolution_timer_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_rach_info
(
    U8  **pp_buffer,
    rrm_oam_rach_info_t *p_rrm_oam_rach_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_rach_info != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rach_info->ra_response_window_size_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rach_info->ra_response_window_size_r13] should be in range [%u <= 7] !", p_rrm_oam_rach_info->ra_response_window_size_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rach_info->ra_response_window_size_r13, "ra_response_window_size_r13");
    *pp_buffer += sizeof(p_rrm_oam_rach_info->ra_response_window_size_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rach_info->mac_contention_resolution_timer_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rach_info->mac_contention_resolution_timer_r13] should be in range [%u <= 7] !", p_rrm_oam_rach_info->mac_contention_resolution_timer_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rach_info->mac_contention_resolution_timer_r13, "mac_contention_resolution_timer_r13");
    *pp_buffer += sizeof(p_rrm_oam_rach_info->mac_contention_resolution_timer_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_rach_infolist_len
(
    rrm_oam_rach_infolist_t *p_rrm_oam_rach_infolist
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_rach_infolist != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rach_infolist->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rach_infolist->count < 1) || (p_rrm_oam_rach_infolist->count > MAX_NPRACH_RESOURCES_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rach_infolist->count] should be in range [1 <= %d <= MAX_NPRACH_RESOURCES_NB_R13] !", p_rrm_oam_rach_infolist->count);
#endif
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_rach_infolist->count; loop++)
        {
            length += rrm_get_rrm_oam_rach_info_len(&p_rrm_oam_rach_infolist->rach_Info_nb_r13[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_rach_infolist
(
    U8  **pp_buffer,
    rrm_oam_rach_infolist_t *p_rrm_oam_rach_infolist
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_rach_infolist != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rach_infolist->count < 1) || (p_rrm_oam_rach_infolist->count > MAX_NPRACH_RESOURCES_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rach_infolist->count] should be in range [1 <= %u <= MAX_NPRACH_RESOURCES_NB_R13] !", p_rrm_oam_rach_infolist->count);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rach_infolist->count, "count");
    *pp_buffer += sizeof(p_rrm_oam_rach_infolist->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_rach_infolist->count; loop++)
        {
            if (OAM_FAILURE == oam_compose_rrm_oam_rach_info(pp_buffer, &p_rrm_oam_rach_infolist->rach_Info_nb_r13[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_preamble_info_nb_len
(
    rrm_oam_preamble_info_nb_t *p_rrm_oam_preamble_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_preamble_info_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_preamble_info_nb->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_preamble_info_nb->preamble_trans_max_ce_r13);

    /* Get length of IE */
    length += rrm_get_rrm_oam_power_ramping_params_len(&p_rrm_oam_preamble_info_nb->power_ramping_parameters_r13);

    /* Get length of IE */
    length += rrm_get_rrm_oam_rach_infolist_len(&p_rrm_oam_preamble_info_nb->rach_info_list_r13);

    /* Optional element */
    if(p_rrm_oam_preamble_info_nb->bitmask & RRM_OAM_CONN_EST_FAIL_OFFSET_R13_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_preamble_info_nb->conn_est_fail_offset_r13);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_preamble_info_nb->max_harq_msg_3tx_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_preamble_info_nb
(
    U8  **pp_buffer,
    rrm_oam_preamble_info_nb_t *p_rrm_oam_preamble_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_preamble_info_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_preamble_info_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_preamble_info_nb->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_preamble_info_nb->preamble_trans_max_ce_r13 > 10))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_preamble_info_nb->preamble_trans_max_ce_r13] should be in range [%u <= 10] !", p_rrm_oam_preamble_info_nb->preamble_trans_max_ce_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_preamble_info_nb->preamble_trans_max_ce_r13, "preamble_trans_max_ce_r13");
    *pp_buffer += sizeof(p_rrm_oam_preamble_info_nb->preamble_trans_max_ce_r13);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_power_ramping_params(pp_buffer, &p_rrm_oam_preamble_info_nb->power_ramping_parameters_r13))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_rach_infolist(pp_buffer, &p_rrm_oam_preamble_info_nb->rach_info_list_r13))
    {
        return OAM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_oam_preamble_info_nb->bitmask & RRM_OAM_CONN_EST_FAIL_OFFSET_R13_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_preamble_info_nb->conn_est_fail_offset_r13 > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_preamble_info_nb->conn_est_fail_offset_r13] should be in range [%u <= 15] !", p_rrm_oam_preamble_info_nb->conn_est_fail_offset_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_preamble_info_nb->conn_est_fail_offset_r13, "conn_est_fail_offset_r13");
    *pp_buffer += sizeof(p_rrm_oam_preamble_info_nb->conn_est_fail_offset_r13);
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_preamble_info_nb->max_harq_msg_3tx_nb < 1) || (p_rrm_oam_preamble_info_nb->max_harq_msg_3tx_nb > 8))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_preamble_info_nb->max_harq_msg_3tx_nb] should be in range [1 <= %u <= 8] !", p_rrm_oam_preamble_info_nb->max_harq_msg_3tx_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_preamble_info_nb->max_harq_msg_3tx_nb, "max_harq_msg_3tx_nb");
    *pp_buffer += sizeof(p_rrm_oam_preamble_info_nb->max_harq_msg_3tx_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_rach_nb_len
(
    rrm_oam_rach_nb_t *p_rrm_oam_rach_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_rach_nb != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_preamble_info_nb_len(&p_rrm_oam_rach_nb->preamble_info_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_rach_nb
(
    U8  **pp_buffer,
    rrm_oam_rach_nb_t *p_rrm_oam_rach_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_rach_nb != PNULL);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_preamble_info_nb(pp_buffer, &p_rrm_oam_rach_nb->preamble_info_nb))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_drx_config_nb_len
(
    rrm_oam_drx_config_nb_t *p_rrm_oam_drx_config_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_drx_config_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_drx_config_nb->on_duration_timer_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_drx_config_nb->drx_inactivity_timer_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_drx_config_nb->drx_retransmission_timer_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_drx_config_nb->drx_cycle_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_drx_config_nb->drx_start_offset_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_drx_config_nb->drx_ul_retransmission_timer_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_drx_config_nb
(
    U8  **pp_buffer,
    rrm_oam_drx_config_nb_t *p_rrm_oam_drx_config_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_drx_config_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_drx_config_nb->on_duration_timer_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_drx_config_nb->on_duration_timer_r13] should be in range [%u <= 6] !", p_rrm_oam_drx_config_nb->on_duration_timer_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_drx_config_nb->on_duration_timer_r13, "on_duration_timer_r13");
    *pp_buffer += sizeof(p_rrm_oam_drx_config_nb->on_duration_timer_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_drx_config_nb->drx_inactivity_timer_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_drx_config_nb->drx_inactivity_timer_r13] should be in range [%u <= 7] !", p_rrm_oam_drx_config_nb->drx_inactivity_timer_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_drx_config_nb->drx_inactivity_timer_r13, "drx_inactivity_timer_r13");
    *pp_buffer += sizeof(p_rrm_oam_drx_config_nb->drx_inactivity_timer_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_drx_config_nb->drx_retransmission_timer_r13 > 8))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_drx_config_nb->drx_retransmission_timer_r13] should be in range [%u <= 8] !", p_rrm_oam_drx_config_nb->drx_retransmission_timer_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_drx_config_nb->drx_retransmission_timer_r13, "drx_retransmission_timer_r13");
    *pp_buffer += sizeof(p_rrm_oam_drx_config_nb->drx_retransmission_timer_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_drx_config_nb->drx_cycle_r13 > 11))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_drx_config_nb->drx_cycle_r13] should be in range [%u <= 11] !", p_rrm_oam_drx_config_nb->drx_cycle_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_drx_config_nb->drx_cycle_r13, "drx_cycle_r13");
    *pp_buffer += sizeof(p_rrm_oam_drx_config_nb->drx_cycle_r13);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_drx_config_nb->drx_start_offset_r13, "drx_start_offset_r13");
    *pp_buffer += sizeof(p_rrm_oam_drx_config_nb->drx_start_offset_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_drx_config_nb->drx_ul_retransmission_timer_r13 > 16))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_drx_config_nb->drx_ul_retransmission_timer_r13] should be in range [%u <= 16] !", p_rrm_oam_drx_config_nb->drx_ul_retransmission_timer_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_drx_config_nb->drx_ul_retransmission_timer_r13, "drx_ul_retransmission_timer_r13");
    *pp_buffer += sizeof(p_rrm_oam_drx_config_nb->drx_ul_retransmission_timer_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_drx_nb_len
(
    rrm_oam_drx_nb_t *p_rrm_oam_drx_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_drx_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_drx_nb->drx_enabled_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_drx_config_nb_len(&p_rrm_oam_drx_nb->drx_config_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_drx_nb
(
    U8  **pp_buffer,
    rrm_oam_drx_nb_t *p_rrm_oam_drx_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_drx_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_drx_nb->drx_enabled_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_drx_nb->drx_enabled_nb] should be in range [%u <= 1] !", p_rrm_oam_drx_nb->drx_enabled_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_drx_nb->drx_enabled_nb, "drx_enabled_nb");
    *pp_buffer += sizeof(p_rrm_oam_drx_nb->drx_enabled_nb);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_drx_config_nb(pp_buffer, &p_rrm_oam_drx_nb->drx_config_nb))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_logical_channel_sr_config_r13_len
(
    rrm_oam_logical_channel_sr_config_r13_t *p_rrm_oam_logical_channel_sr_config_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_logical_channel_sr_config_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_logical_channel_sr_config_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_logical_channel_sr_config_r13->bitmask & RRM_OAM_LOGICAL_CHANNEL_SR_PROHIBIT_TIMER_SETUP_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_logical_channel_sr_config_r13->logical_channel_sr_prohibit_timer_r13);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_logical_channel_sr_config_r13
(
    U8  **pp_buffer,
    rrm_oam_logical_channel_sr_config_r13_t *p_rrm_oam_logical_channel_sr_config_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_logical_channel_sr_config_r13 != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_logical_channel_sr_config_r13->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_logical_channel_sr_config_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_logical_channel_sr_config_r13->bitmask & RRM_OAM_LOGICAL_CHANNEL_SR_PROHIBIT_TIMER_SETUP_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_logical_channel_sr_config_r13->logical_channel_sr_prohibit_timer_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_logical_channel_sr_config_r13->logical_channel_sr_prohibit_timer_r13] should be in range [%u <= 6] !", p_rrm_oam_logical_channel_sr_config_r13->logical_channel_sr_prohibit_timer_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_logical_channel_sr_config_r13->logical_channel_sr_prohibit_timer_r13, "logical_channel_sr_prohibit_timer_r13");
    *pp_buffer += sizeof(p_rrm_oam_logical_channel_sr_config_r13->logical_channel_sr_prohibit_timer_r13);
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_ul_sch_config_r13_len
(
    rrm_oam_ul_sch_config_r13 *p_rrm_oam_ul_sch_config_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_ul_sch_config_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_ul_sch_config_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_ul_sch_config_r13->bitmask & RRM_OAM_PERIODIC_BSR_TIMER_R13_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_ul_sch_config_r13->periodic_bsr_timer_r13);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_ul_sch_config_r13->retx_bsr_timer_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_ul_sch_config_r13
(
    U8  **pp_buffer,
    rrm_oam_ul_sch_config_r13 *p_rrm_oam_ul_sch_config_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_ul_sch_config_r13 != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_ul_sch_config_r13->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_ul_sch_config_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_ul_sch_config_r13->bitmask & RRM_OAM_PERIODIC_BSR_TIMER_R13_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_ul_sch_config_r13->periodic_bsr_timer_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ul_sch_config_r13->periodic_bsr_timer_r13] should be in range [%u <= 6] !", p_rrm_oam_ul_sch_config_r13->periodic_bsr_timer_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_ul_sch_config_r13->periodic_bsr_timer_r13, "periodic_bsr_timer_r13");
    *pp_buffer += sizeof(p_rrm_oam_ul_sch_config_r13->periodic_bsr_timer_r13);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_ul_sch_config_r13->retx_bsr_timer_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ul_sch_config_r13->retx_bsr_timer_r13] should be in range [%u <= 6] !", p_rrm_oam_ul_sch_config_r13->retx_bsr_timer_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_ul_sch_config_r13->retx_bsr_timer_r13, "retx_bsr_timer_r13");
    *pp_buffer += sizeof(p_rrm_oam_ul_sch_config_r13->retx_bsr_timer_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_mac_layer_params_nb_len
(
    rrm_oam_mac_layer_params_nb_t *p_rrm_oam_mac_layer_params_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_mac_layer_params_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_mac_layer_params_nb->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_rach_nb_len(&p_rrm_oam_mac_layer_params_nb->mac_layer_param_rach_nb);

    /* Optional element */
    if(p_rrm_oam_mac_layer_params_nb->bitmask & RRM_OAM_DRX_CONFIG_NB_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_drx_nb_len(&p_rrm_oam_mac_layer_params_nb->mac_layer_param_drx_nb);
    }

    /* Get length of IE */
    length += rrm_get_rrm_oam_logical_channel_sr_config_r13_len(&p_rrm_oam_mac_layer_params_nb->logical_channel_sr_config_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_mac_layer_params_nb->ul_sync_loss_timer_nb);

    /* Optional element */
    if(p_rrm_oam_mac_layer_params_nb->bitmask & RRM_OAM_UL_SCH_CONFIG_R13_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_ul_sch_config_r13_len(&p_rrm_oam_mac_layer_params_nb->ul_sch_config_r13);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_mac_layer_params_nb
(
    U8  **pp_buffer,
    rrm_oam_mac_layer_params_nb_t *p_rrm_oam_mac_layer_params_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_mac_layer_params_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_mac_layer_params_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_mac_layer_params_nb->bitmask);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_rach_nb(pp_buffer, &p_rrm_oam_mac_layer_params_nb->mac_layer_param_rach_nb))
    {
        return OAM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_oam_mac_layer_params_nb->bitmask & RRM_OAM_DRX_CONFIG_NB_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_drx_nb(pp_buffer, &p_rrm_oam_mac_layer_params_nb->mac_layer_param_drx_nb))
    {
        return OAM_FAILURE;
    }
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_logical_channel_sr_config_r13(pp_buffer, &p_rrm_oam_mac_layer_params_nb->logical_channel_sr_config_r13))
    {
        return OAM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_mac_layer_params_nb->ul_sync_loss_timer_nb > 12))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_mac_layer_params_nb->ul_sync_loss_timer_nb] should be in range [%u <= 12] !", p_rrm_oam_mac_layer_params_nb->ul_sync_loss_timer_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_mac_layer_params_nb->ul_sync_loss_timer_nb, "ul_sync_loss_timer_nb");
    *pp_buffer += sizeof(p_rrm_oam_mac_layer_params_nb->ul_sync_loss_timer_nb);

    /* Optional element */
    if(p_rrm_oam_mac_layer_params_nb->bitmask & RRM_OAM_UL_SCH_CONFIG_R13_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_ul_sch_config_r13(pp_buffer, &p_rrm_oam_mac_layer_params_nb->ul_sch_config_r13))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_ul_am_rlc_nb_r13_len
(
    ul_am_rlc_nb_r13_t *p_ul_am_rlc_nb_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_ul_am_rlc_nb_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_ul_am_rlc_nb_r13->t_poll_retransmit_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_ul_am_rlc_nb_r13->max_retx_threshold_nb);

    return length;
}

static
oam_return_et
oam_compose_ul_am_rlc_nb_r13
(
    U8  **pp_buffer,
    ul_am_rlc_nb_r13_t *p_ul_am_rlc_nb_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_ul_am_rlc_nb_r13 != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_ul_am_rlc_nb_r13->t_poll_retransmit_r13 > 14))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_ul_am_rlc_nb_r13->t_poll_retransmit_r13] should be in range [%u <= 14] !", p_ul_am_rlc_nb_r13->t_poll_retransmit_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_ul_am_rlc_nb_r13->t_poll_retransmit_r13, "t_poll_retransmit_r13");
    *pp_buffer += sizeof(p_ul_am_rlc_nb_r13->t_poll_retransmit_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_ul_am_rlc_nb_r13->max_retx_threshold_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_ul_am_rlc_nb_r13->max_retx_threshold_nb] should be in range [%u <= 7] !", p_ul_am_rlc_nb_r13->max_retx_threshold_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_ul_am_rlc_nb_r13->max_retx_threshold_nb, "max_retx_threshold_nb");
    *pp_buffer += sizeof(p_ul_am_rlc_nb_r13->max_retx_threshold_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_dl_am_rlc_nb_r13_len
(
    dl_am_rlc_nb_r13_t *p_dl_am_rlc_nb_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_dl_am_rlc_nb_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_dl_am_rlc_nb_r13->bitmask);

    /* Optional element */
    if(p_dl_am_rlc_nb_r13->bitmask & RRM_OAM_ENABLE_STATUS_REPORT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_dl_am_rlc_nb_r13->enable_status_report_sn_gap_r13);
    }

    return length;
}

static
oam_return_et
oam_compose_dl_am_rlc_nb_r13
(
    U8  **pp_buffer,
    dl_am_rlc_nb_r13_t *p_dl_am_rlc_nb_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_dl_am_rlc_nb_r13 != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_dl_am_rlc_nb_r13->bitmask, "bitmask");
    *pp_buffer += sizeof(p_dl_am_rlc_nb_r13->bitmask);

    /* Optional element */
    if(p_dl_am_rlc_nb_r13->bitmask & RRM_OAM_ENABLE_STATUS_REPORT_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_dl_am_rlc_nb_r13->enable_status_report_sn_gap_r13 > 0))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_dl_am_rlc_nb_r13->enable_status_report_sn_gap_r13] should be in range [%u <= 0] !", p_dl_am_rlc_nb_r13->enable_status_report_sn_gap_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_dl_am_rlc_nb_r13->enable_status_report_sn_gap_r13, "enable_status_report_sn_gap_r13");
    *pp_buffer += sizeof(p_dl_am_rlc_nb_r13->enable_status_report_sn_gap_r13);
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_logical_channel_config_nb_len
(
    rrm_oam_logical_channel_config_nb_t *p_rrm_oam_logical_channel_config_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_logical_channel_config_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_logical_channel_config_nb->bitmask);

    /* Optional element */
    if(p_rrm_oam_logical_channel_config_nb->bitmask & RRM_OAM_NB_LOGICAL_CHANNEL_CONFIG_PRIORITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_logical_channel_config_nb->priority_r13);
    }

    /* Optional element */
    if(p_rrm_oam_logical_channel_config_nb->bitmask & RRM_OAM_NB_LOGICAL_CHANNEL_SR_PROHIBIT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_logical_channel_config_nb->logical_channel_sr_prohibit_r13);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_logical_channel_config_nb
(
    U8  **pp_buffer,
    rrm_oam_logical_channel_config_nb_t *p_rrm_oam_logical_channel_config_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_logical_channel_config_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_logical_channel_config_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_logical_channel_config_nb->bitmask);

    /* Optional element */
    if(p_rrm_oam_logical_channel_config_nb->bitmask & RRM_OAM_NB_LOGICAL_CHANNEL_CONFIG_PRIORITY_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_logical_channel_config_nb->priority_r13 < 1) || (p_rrm_oam_logical_channel_config_nb->priority_r13 > 16))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_logical_channel_config_nb->priority_r13] should be in range [1 <= %u <= 16] !", p_rrm_oam_logical_channel_config_nb->priority_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_logical_channel_config_nb->priority_r13, "priority_r13");
    *pp_buffer += sizeof(p_rrm_oam_logical_channel_config_nb->priority_r13);
    }

    /* Optional element */
    if(p_rrm_oam_logical_channel_config_nb->bitmask & RRM_OAM_NB_LOGICAL_CHANNEL_SR_PROHIBIT_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_logical_channel_config_nb->logical_channel_sr_prohibit_r13 > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_logical_channel_config_nb->logical_channel_sr_prohibit_r13] should be in range [%u <= 1] !", p_rrm_oam_logical_channel_config_nb->logical_channel_sr_prohibit_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_logical_channel_config_nb->logical_channel_sr_prohibit_r13, "logical_channel_sr_prohibit_r13");
    *pp_buffer += sizeof(p_rrm_oam_logical_channel_config_nb->logical_channel_sr_prohibit_r13);
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_srb_info_nb_len
(
    rrm_oam_srb_info_nb_t *p_rrm_oam_srb_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_srb_info_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_srb_info_nb->bitmask);

    /* Get length of IE */
    length += rrm_get_ul_am_rlc_nb_r13_len(&p_rrm_oam_srb_info_nb->ul_am_rlc_nb);

    /* Get length of IE */
    length += rrm_get_dl_am_rlc_nb_r13_len(&p_rrm_oam_srb_info_nb->dl_am_rlc_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_logical_channel_config_nb_len(&p_rrm_oam_srb_info_nb->logical_channel_config_nb_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_srb_info_nb
(
    U8  **pp_buffer,
    rrm_oam_srb_info_nb_t *p_rrm_oam_srb_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_srb_info_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_srb_info_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_srb_info_nb->bitmask);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_ul_am_rlc_nb_r13(pp_buffer, &p_rrm_oam_srb_info_nb->ul_am_rlc_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_dl_am_rlc_nb_r13(pp_buffer, &p_rrm_oam_srb_info_nb->dl_am_rlc_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_logical_channel_config_nb(pp_buffer, &p_rrm_oam_srb_info_nb->logical_channel_config_nb_r13))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_srb_nb_len
(
    rrm_oam_srb_nb_t *p_rrm_oam_srb_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_srb_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_srb_nb->default_configuration_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_srb_info_nb_len(&p_rrm_oam_srb_nb->srb_params_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_srb_nb
(
    U8  **pp_buffer,
    rrm_oam_srb_nb_t *p_rrm_oam_srb_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_srb_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_srb_nb->default_configuration_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_srb_nb->default_configuration_nb] should be in range [%u <= 1] !", p_rrm_oam_srb_nb->default_configuration_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_srb_nb->default_configuration_nb, "default_configuration_nb");
    *pp_buffer += sizeof(p_rrm_oam_srb_nb->default_configuration_nb);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_srb_info_nb(pp_buffer, &p_rrm_oam_srb_nb->srb_params_nb))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_rlc_layer_params_nb_len
(
    rrm_oam_rlc_layer_params_nb_t *p_rrm_oam_rlc_layer_params_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_rlc_layer_params_nb != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_srb_nb_len(&p_rrm_oam_rlc_layer_params_nb->rlc_layer_param_srb_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_rlc_layer_params_nb
(
    U8  **pp_buffer,
    rrm_oam_rlc_layer_params_nb_t *p_rrm_oam_rlc_layer_params_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_rlc_layer_params_nb != PNULL);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_srb_nb(pp_buffer, &p_rrm_oam_rlc_layer_params_nb->rlc_layer_param_srb_nb))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_rrc_timers_nb_len
(
    rrm_oam_rrc_timers_nb_t *p_rrm_oam_rrc_timers_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_rrc_timers_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rrc_timers_nb->t300_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rrc_timers_nb->t301_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rrc_timers_nb->t310_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rrc_timers_nb->t311_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_rrc_timers_nb
(
    U8  **pp_buffer,
    rrm_oam_rrc_timers_nb_t *p_rrm_oam_rrc_timers_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_rrc_timers_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rrc_timers_nb->t300_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rrc_timers_nb->t300_nb] should be in range [%u <= 7] !", p_rrm_oam_rrc_timers_nb->t300_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rrc_timers_nb->t300_nb, "t300_nb");
    *pp_buffer += sizeof(p_rrm_oam_rrc_timers_nb->t300_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rrc_timers_nb->t301_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rrc_timers_nb->t301_nb] should be in range [%u <= 7] !", p_rrm_oam_rrc_timers_nb->t301_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rrc_timers_nb->t301_nb, "t301_nb");
    *pp_buffer += sizeof(p_rrm_oam_rrc_timers_nb->t301_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rrc_timers_nb->t310_nb > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rrc_timers_nb->t310_nb] should be in range [%u <= 6] !", p_rrm_oam_rrc_timers_nb->t310_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rrc_timers_nb->t310_nb, "t310_nb");
    *pp_buffer += sizeof(p_rrm_oam_rrc_timers_nb->t310_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rrc_timers_nb->t311_nb > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rrc_timers_nb->t311_nb] should be in range [%u <= 6] !", p_rrm_oam_rrc_timers_nb->t311_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rrc_timers_nb->t311_nb, "t311_nb");
    *pp_buffer += sizeof(p_rrm_oam_rrc_timers_nb->t311_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_rrc_constants_nb_len
(
    rrm_oam_rrc_constants_nb_t *p_rrm_oam_rrc_constants_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_rrc_constants_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rrc_constants_nb->n310_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rrc_constants_nb->n311_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_rrc_constants_nb
(
    U8  **pp_buffer,
    rrm_oam_rrc_constants_nb_t *p_rrm_oam_rrc_constants_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_rrc_constants_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rrc_constants_nb->n310_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rrc_constants_nb->n310_nb] should be in range [%u <= 7] !", p_rrm_oam_rrc_constants_nb->n310_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rrc_constants_nb->n310_nb, "n310_nb");
    *pp_buffer += sizeof(p_rrm_oam_rrc_constants_nb->n310_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rrc_constants_nb->n311_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rrc_constants_nb->n311_nb] should be in range [%u <= 7] !", p_rrm_oam_rrc_constants_nb->n311_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rrc_constants_nb->n311_nb, "n311_nb");
    *pp_buffer += sizeof(p_rrm_oam_rrc_constants_nb->n311_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_rrc_timers_and_constants_nb_len
(
    rrm_oam_rrc_timers_and_constants_nb_t *p_rrm_oam_rrc_timers_and_constants_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_rrc_timers_and_constants_nb != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_rrc_timers_nb_len(&p_rrm_oam_rrc_timers_and_constants_nb->rrc_timers_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_rrc_constants_nb_len(&p_rrm_oam_rrc_timers_and_constants_nb->rrc_constants_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_rrc_timers_and_constants_nb
(
    U8  **pp_buffer,
    rrm_oam_rrc_timers_and_constants_nb_t *p_rrm_oam_rrc_timers_and_constants_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_rrc_timers_and_constants_nb != PNULL);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_rrc_timers_nb(pp_buffer, &p_rrm_oam_rrc_timers_and_constants_nb->rrc_timers_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_rrc_constants_nb(pp_buffer, &p_rrm_oam_rrc_timers_and_constants_nb->rrc_constants_nb))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_ns_pmax_value_nb_r13_len
(
    rrm_oam_ns_pmax_value_nb_r13_t *p_rrm_oam_ns_pmax_value_nb_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_ns_pmax_value_nb_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_ns_pmax_value_nb_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_ns_pmax_value_nb_r13->bitmask & RRM_OAM_NB_PMAX_R13_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_ns_pmax_value_nb_r13->pmax_r13);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_ns_pmax_value_nb_r13->additional_spectrum_emission_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_ns_pmax_value_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_ns_pmax_value_nb_r13_t *p_rrm_oam_ns_pmax_value_nb_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_ns_pmax_value_nb_r13 != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_ns_pmax_value_nb_r13->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_ns_pmax_value_nb_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_ns_pmax_value_nb_r13->bitmask & RRM_OAM_NB_PMAX_R13_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_ns_pmax_value_nb_r13->pmax_r13 < -30) || (p_rrm_oam_ns_pmax_value_nb_r13->pmax_r13 > 33))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ns_pmax_value_nb_r13->pmax_r13] should be in range [-30 <= %d <= 33] !", p_rrm_oam_ns_pmax_value_nb_r13->pmax_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT8(*pp_buffer, &p_rrm_oam_ns_pmax_value_nb_r13->pmax_r13, "pmax_r13");
    *pp_buffer += sizeof(p_rrm_oam_ns_pmax_value_nb_r13->pmax_r13);
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_ns_pmax_value_nb_r13->additional_spectrum_emission_r13 < 1) || (p_rrm_oam_ns_pmax_value_nb_r13->additional_spectrum_emission_r13 > 32))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ns_pmax_value_nb_r13->additional_spectrum_emission_r13] should be in range [1 <= %u <= 32] !", p_rrm_oam_ns_pmax_value_nb_r13->additional_spectrum_emission_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_ns_pmax_value_nb_r13->additional_spectrum_emission_r13, "additional_spectrum_emission_r13");
    *pp_buffer += sizeof(p_rrm_oam_ns_pmax_value_nb_r13->additional_spectrum_emission_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_freq_band_info_nb_len
(
    rrm_oam_freq_band_info_nb_t *p_rrm_oam_freq_band_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_freq_band_info_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_freq_band_info_nb->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_freq_band_info_nb->count < 1) || (p_rrm_oam_freq_band_info_nb->count > MAX_NS_PMAX_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_freq_band_info_nb->count] should be in range [1 <= %d <= MAX_NS_PMAX_NB_R13] !", p_rrm_oam_freq_band_info_nb->count);
#endif
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_freq_band_info_nb->count; loop++)
        {
            length += rrm_get_rrm_oam_ns_pmax_value_nb_r13_len(&p_rrm_oam_freq_band_info_nb->ns_pmax_value_nb_r13[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_freq_band_info_nb
(
    U8  **pp_buffer,
    rrm_oam_freq_band_info_nb_t *p_rrm_oam_freq_band_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_freq_band_info_nb != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_freq_band_info_nb->count < 1) || (p_rrm_oam_freq_band_info_nb->count > MAX_NS_PMAX_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_freq_band_info_nb->count] should be in range [1 <= %u <= MAX_NS_PMAX_NB_R13] !", p_rrm_oam_freq_band_info_nb->count);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_freq_band_info_nb->count, "count");
    *pp_buffer += sizeof(p_rrm_oam_freq_band_info_nb->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_freq_band_info_nb->count; loop++)
        {
            if (OAM_FAILURE == oam_compose_rrm_oam_ns_pmax_value_nb_r13(pp_buffer, &p_rrm_oam_freq_band_info_nb->ns_pmax_value_nb_r13[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_multi_band_info_nb_r13_len
(
    rrm_oam_multi_band_info_nb_r13_t *p_rrm_oam_multi_band_info_nb_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_multi_band_info_nb_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_multi_band_info_nb_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_multi_band_info_nb_r13->bitmask & RRM_OAM_NB_FREQ_BAND_INDICATOR_R13_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_multi_band_info_nb_r13->freq_band_indicator_r13);
    }

    /* Optional element */
    if(p_rrm_oam_multi_band_info_nb_r13->bitmask & RRM_OAM_NB_FREQ_BAND_INFO_NB_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_freq_band_info_nb_len(&p_rrm_oam_multi_band_info_nb_r13->freq_band_info_r13);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_multi_band_info_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_multi_band_info_nb_r13_t *p_rrm_oam_multi_band_info_nb_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_multi_band_info_nb_r13 != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_multi_band_info_nb_r13->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_multi_band_info_nb_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_multi_band_info_nb_r13->bitmask & RRM_OAM_NB_FREQ_BAND_INDICATOR_R13_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_multi_band_info_nb_r13->freq_band_indicator_r13 < 1) || (p_rrm_oam_multi_band_info_nb_r13->freq_band_indicator_r13 > MAX_FBI_2_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_multi_band_info_nb_r13->freq_band_indicator_r13] should be in range [1 <= %u <= MAX_FBI_2_NB] !", p_rrm_oam_multi_band_info_nb_r13->freq_band_indicator_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_multi_band_info_nb_r13->freq_band_indicator_r13, "freq_band_indicator_r13");
    *pp_buffer += sizeof(p_rrm_oam_multi_band_info_nb_r13->freq_band_indicator_r13);
    }

    /* Optional element */
    if(p_rrm_oam_multi_band_info_nb_r13->bitmask & RRM_OAM_NB_FREQ_BAND_INFO_NB_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_freq_band_info_nb(pp_buffer, &p_rrm_oam_multi_band_info_nb_r13->freq_band_info_r13))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_multiband_info_list_nb_r13_len
(
    rrm_oam_multiband_info_list_nb_r13_t *p_rrm_oam_multiband_info_list_nb_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_multiband_info_list_nb_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_multiband_info_list_nb_r13->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_multiband_info_list_nb_r13->count < 1) || (p_rrm_oam_multiband_info_list_nb_r13->count > MAX_MULTI_BAND_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_multiband_info_list_nb_r13->count] should be in range [1 <= %d <= MAX_MULTI_BAND_NB] !", p_rrm_oam_multiband_info_list_nb_r13->count);
#endif
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_multiband_info_list_nb_r13->count; loop++)
        {
            length += rrm_get_rrm_oam_multi_band_info_nb_r13_len(&p_rrm_oam_multiband_info_list_nb_r13->multi_band_info_nb_r13[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_multiband_info_list_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_multiband_info_list_nb_r13_t *p_rrm_oam_multiband_info_list_nb_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_multiband_info_list_nb_r13 != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_multiband_info_list_nb_r13->count < 1) || (p_rrm_oam_multiband_info_list_nb_r13->count > MAX_MULTI_BAND_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_multiband_info_list_nb_r13->count] should be in range [1 <= %u <= MAX_MULTI_BAND_NB] !", p_rrm_oam_multiband_info_list_nb_r13->count);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_multiband_info_list_nb_r13->count, "count");
    *pp_buffer += sizeof(p_rrm_oam_multiband_info_list_nb_r13->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_multiband_info_list_nb_r13->count; loop++)
        {
            if (OAM_FAILURE == oam_compose_rrm_oam_multi_band_info_nb_r13(pp_buffer, &p_rrm_oam_multiband_info_list_nb_r13->multi_band_info_nb_r13[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_downlink_bitmap_nb_r13_len
(
    rrm_oam_downlink_bitmap_nb_r13_t *p_rrm_oam_downlink_bitmap_nb_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_downlink_bitmap_nb_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_downlink_bitmap_nb_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_downlink_bitmap_nb_r13->bitmask & RRM_OAM_NB_SUBFRAME_PATTERN_10_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_oam_downlink_bitmap_nb_r13->subframe_pattern10_r13);
    }

    /* Optional element */
    if(p_rrm_oam_downlink_bitmap_nb_r13->bitmask & RRM_OAM_NB_SUBFRAME_PATTERN_40_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_oam_downlink_bitmap_nb_r13->subframe_pattern40_r13);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_downlink_bitmap_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_downlink_bitmap_nb_r13_t *p_rrm_oam_downlink_bitmap_nb_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_downlink_bitmap_nb_r13 != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_downlink_bitmap_nb_r13->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_downlink_bitmap_nb_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_downlink_bitmap_nb_r13->bitmask & RRM_OAM_NB_SUBFRAME_PATTERN_10_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_downlink_bitmap_nb_r13->subframe_pattern10_r13); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_downlink_bitmap_nb_r13->subframe_pattern10_r13[loop], "subframe_pattern10_r13[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_rrm_oam_downlink_bitmap_nb_r13->bitmask & RRM_OAM_NB_SUBFRAME_PATTERN_40_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_downlink_bitmap_nb_r13->subframe_pattern40_r13); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_downlink_bitmap_nb_r13->subframe_pattern40_r13[loop], "subframe_pattern40_r13[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_rf_configurations_nb_len
(
    rrm_oam_rf_configurations_nb_t *p_rrm_oam_rf_configurations_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_rf_configurations_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->freq_band_indicator_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->dl_earfcn_nb);

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_RF_CONFIGURATION_UL_EARFCN_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->ul_earfcn_nb);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->nrs_power_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->phy_cell_id_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->psch_power_offset_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->ssch_power_offset_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->pbch_power_offset_nb);

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_MAX_RS_EPRE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->max_rs_epre_nb);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->pdcch_power_offset_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->pbch_transmission_power);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->pch_transmission_power);

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_FREQ_BAND_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_freq_band_info_nb_len(&p_rrm_oam_rf_configurations_nb->freq_band_info_r13);
    }

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_MULTI_BAND_INFO_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_multiband_info_list_nb_r13_len(&p_rrm_oam_rf_configurations_nb->multi_band_info_list_r13);
    }

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_DL_BITMAP_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_downlink_bitmap_nb_r13_len(&p_rrm_oam_rf_configurations_nb->downlink_bitmap_r13);
    }

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_EUTRA_CONTROL_REGION_SIZE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->eutra_control_region_size_r13);
    }

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NRS_CRS_POWER_OFFSET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->nrs_crs_power_offset_r13);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->scheduling_info_sib1_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rf_configurations_nb->ab_enabled_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_rf_configurations_nb
(
    U8  **pp_buffer,
    rrm_oam_rf_configurations_nb_t *p_rrm_oam_rf_configurations_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_rf_configurations_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rf_configurations_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rf_configurations_nb->freq_band_indicator_nb < 1) || (p_rrm_oam_rf_configurations_nb->freq_band_indicator_nb > MAX_FBI_2_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->freq_band_indicator_nb] should be in range [1 <= %u <= MAX_FBI_2_NB] !", p_rrm_oam_rf_configurations_nb->freq_band_indicator_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_rf_configurations_nb->freq_band_indicator_nb, "freq_band_indicator_nb");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->freq_band_indicator_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->dl_earfcn_nb > MAX_EARFCN))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->dl_earfcn_nb] should be in range [%u <= MAX_EARFCN] !", p_rrm_oam_rf_configurations_nb->dl_earfcn_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_rf_configurations_nb->dl_earfcn_nb, "dl_earfcn_nb");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->dl_earfcn_nb);

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_RF_CONFIGURATION_UL_EARFCN_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->ul_earfcn_nb > 65535))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->ul_earfcn_nb] should be in range [%u <= 65535] !", p_rrm_oam_rf_configurations_nb->ul_earfcn_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_rf_configurations_nb->ul_earfcn_nb, "ul_earfcn_nb");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->ul_earfcn_nb);
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rf_configurations_nb->nrs_power_r13 < -60) || (p_rrm_oam_rf_configurations_nb->nrs_power_r13 > 50))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->nrs_power_r13] should be in range [-60 <= %d <= 50] !", p_rrm_oam_rf_configurations_nb->nrs_power_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT8(*pp_buffer, &p_rrm_oam_rf_configurations_nb->nrs_power_r13, "nrs_power_r13");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->nrs_power_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->phy_cell_id_nb > 503))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->phy_cell_id_nb] should be in range [%u <= 503] !", p_rrm_oam_rf_configurations_nb->phy_cell_id_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_rf_configurations_nb->phy_cell_id_nb, "phy_cell_id_nb");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->phy_cell_id_nb);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rf_configurations_nb->psch_power_offset_nb < -60) || (p_rrm_oam_rf_configurations_nb->psch_power_offset_nb > 40))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->psch_power_offset_nb] should be in range [-60 <= %d <= 40] !", p_rrm_oam_rf_configurations_nb->psch_power_offset_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT8(*pp_buffer, &p_rrm_oam_rf_configurations_nb->psch_power_offset_nb, "psch_power_offset_nb");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->psch_power_offset_nb);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rf_configurations_nb->ssch_power_offset_nb < -60) || (p_rrm_oam_rf_configurations_nb->ssch_power_offset_nb > 40))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->ssch_power_offset_nb] should be in range [-60 <= %d <= 40] !", p_rrm_oam_rf_configurations_nb->ssch_power_offset_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT8(*pp_buffer, &p_rrm_oam_rf_configurations_nb->ssch_power_offset_nb, "ssch_power_offset_nb");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->ssch_power_offset_nb);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rf_configurations_nb->pbch_power_offset_nb < -60) || (p_rrm_oam_rf_configurations_nb->pbch_power_offset_nb > 40))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->pbch_power_offset_nb] should be in range [-60 <= %d <= 40] !", p_rrm_oam_rf_configurations_nb->pbch_power_offset_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT8(*pp_buffer, &p_rrm_oam_rf_configurations_nb->pbch_power_offset_nb, "pbch_power_offset_nb");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->pbch_power_offset_nb);

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_MAX_RS_EPRE_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rf_configurations_nb->max_rs_epre_nb < -256) || (p_rrm_oam_rf_configurations_nb->max_rs_epre_nb > 256))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->max_rs_epre_nb] should be in range [-256 <= %d <= 256] !", p_rrm_oam_rf_configurations_nb->max_rs_epre_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT16(*pp_buffer, &p_rrm_oam_rf_configurations_nb->max_rs_epre_nb, "max_rs_epre_nb");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->max_rs_epre_nb);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->pdcch_power_offset_nb > 10000))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->pdcch_power_offset_nb] should be in range [%u <= 10000] !", p_rrm_oam_rf_configurations_nb->pdcch_power_offset_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_rf_configurations_nb->pdcch_power_offset_nb, "pdcch_power_offset_nb");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->pdcch_power_offset_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->pbch_transmission_power > 10000))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->pbch_transmission_power] should be in range [%u <= 10000] !", p_rrm_oam_rf_configurations_nb->pbch_transmission_power);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_rf_configurations_nb->pbch_transmission_power, "pbch_transmission_power");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->pbch_transmission_power);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->pch_transmission_power > 10000))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->pch_transmission_power] should be in range [%u <= 10000] !", p_rrm_oam_rf_configurations_nb->pch_transmission_power);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_rf_configurations_nb->pch_transmission_power, "pch_transmission_power");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->pch_transmission_power);

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_FREQ_BAND_INFO_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_freq_band_info_nb(pp_buffer, &p_rrm_oam_rf_configurations_nb->freq_band_info_r13))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_MULTI_BAND_INFO_LIST_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_multiband_info_list_nb_r13(pp_buffer, &p_rrm_oam_rf_configurations_nb->multi_band_info_list_r13))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_DL_BITMAP_INFO_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_downlink_bitmap_nb_r13(pp_buffer, &p_rrm_oam_rf_configurations_nb->downlink_bitmap_r13))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_EUTRA_CONTROL_REGION_SIZE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->eutra_control_region_size_r13 > 2))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->eutra_control_region_size_r13] should be in range [%u <= 2] !", p_rrm_oam_rf_configurations_nb->eutra_control_region_size_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rf_configurations_nb->eutra_control_region_size_r13, "eutra_control_region_size_r13");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->eutra_control_region_size_r13);
    }

    /* Optional element */
    if(p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NRS_CRS_POWER_OFFSET_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->nrs_crs_power_offset_r13 > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->nrs_crs_power_offset_r13] should be in range [%u <= 15] !", p_rrm_oam_rf_configurations_nb->nrs_crs_power_offset_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rf_configurations_nb->nrs_crs_power_offset_r13, "nrs_crs_power_offset_r13");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->nrs_crs_power_offset_r13);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->scheduling_info_sib1_nb > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->scheduling_info_sib1_nb] should be in range [%u <= 15] !", p_rrm_oam_rf_configurations_nb->scheduling_info_sib1_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rf_configurations_nb->scheduling_info_sib1_nb, "scheduling_info_sib1_nb");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->scheduling_info_sib1_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->ab_enabled_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->ab_enabled_nb] should be in range [%u <= 1] !", p_rrm_oam_rf_configurations_nb->ab_enabled_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_rf_configurations_nb->ab_enabled_nb, "ab_enabled_nb");
    *pp_buffer += sizeof(p_rrm_oam_rf_configurations_nb->ab_enabled_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_rf_params_nb_len
(
    rrm_oam_rf_params_nb_t *p_rrm_oam_rf_params_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_rf_params_nb != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_rf_configurations_nb_len(&p_rrm_oam_rf_params_nb->rf_configurations_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_rf_params_nb
(
    U8  **pp_buffer,
    rrm_oam_rf_params_nb_t *p_rrm_oam_rf_params_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_rf_params_nb != PNULL);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_rf_configurations_nb(pp_buffer, &p_rrm_oam_rf_params_nb->rf_configurations_nb))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_s1ap_params_len
(
    rrm_oam_s1ap_params_t *p_rrm_oam_s1ap_params
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_s1ap_params != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_s1ap_params->t_reloc_prep);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_s1ap_params->t_reloc_overall);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_s1ap_params
(
    U8  **pp_buffer,
    rrm_oam_s1ap_params_t *p_rrm_oam_s1ap_params
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_s1ap_params != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_s1ap_params->t_reloc_prep, "t_reloc_prep");
    *pp_buffer += sizeof(p_rrm_oam_s1ap_params->t_reloc_prep);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_s1ap_params->t_reloc_overall > 60))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_s1ap_params->t_reloc_overall] should be in range [%u <= 60] !", p_rrm_oam_s1ap_params->t_reloc_overall);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_s1ap_params->t_reloc_overall, "t_reloc_overall");
    *pp_buffer += sizeof(p_rrm_oam_s1ap_params->t_reloc_overall);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_ran_info_nb_len
(
    rrm_oam_ran_info_nb_t *p_rrm_oam_ran_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_ran_info_nb != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_physical_layer_params_nb_len(&p_rrm_oam_ran_info_nb->physical_layer_params_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_mac_layer_params_nb_len(&p_rrm_oam_ran_info_nb->mac_layer_params_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_rlc_layer_params_nb_len(&p_rrm_oam_ran_info_nb->rlc_layer_params_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_rrc_timers_and_constants_nb_len(&p_rrm_oam_ran_info_nb->rrc_timers_and_constants_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_rf_params_nb_len(&p_rrm_oam_ran_info_nb->rf_params_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_s1ap_params_len(&p_rrm_oam_ran_info_nb->s1ap_params_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_ran_info_nb
(
    U8  **pp_buffer,
    rrm_oam_ran_info_nb_t *p_rrm_oam_ran_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_ran_info_nb != PNULL);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_physical_layer_params_nb(pp_buffer, &p_rrm_oam_ran_info_nb->physical_layer_params_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_mac_layer_params_nb(pp_buffer, &p_rrm_oam_ran_info_nb->mac_layer_params_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_rlc_layer_params_nb(pp_buffer, &p_rrm_oam_ran_info_nb->rlc_layer_params_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_rrc_timers_and_constants_nb(pp_buffer, &p_rrm_oam_ran_info_nb->rrc_timers_and_constants_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_rf_params_nb(pp_buffer, &p_rrm_oam_ran_info_nb->rf_params_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_s1ap_params(pp_buffer, &p_rrm_oam_ran_info_nb->s1ap_params_nb))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_plmn_access_info_nb_len
(
    rrm_oam_plmn_access_info_nb_t *p_rrm_oam_plmn_access_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_plmn_access_info_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_plmn_access_info_nb->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_cell_plmn_info_len(&p_rrm_oam_plmn_access_info_nb->plmn_info);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_plmn_access_info_nb->reserve_operator_use);

    /* Optional element */
    if(p_rrm_oam_plmn_access_info_nb->bitmask & RRM_OAM_NB_ATTACH_WITHOUT_PDN_CONNECTIVITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_plmn_access_info_nb->attach_without_pdn_connectivity_nb);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_plmn_access_info_nb
(
    U8  **pp_buffer,
    rrm_oam_plmn_access_info_nb_t *p_rrm_oam_plmn_access_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_plmn_access_info_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_plmn_access_info_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_plmn_access_info_nb->bitmask);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_cell_plmn_info(pp_buffer, &p_rrm_oam_plmn_access_info_nb->plmn_info))
    {
        return OAM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_plmn_access_info_nb->reserve_operator_use > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_plmn_access_info_nb->reserve_operator_use] should be in range [%u <= 1] !", p_rrm_oam_plmn_access_info_nb->reserve_operator_use);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_plmn_access_info_nb->reserve_operator_use, "reserve_operator_use");
    *pp_buffer += sizeof(p_rrm_oam_plmn_access_info_nb->reserve_operator_use);

    /* Optional element */
    if(p_rrm_oam_plmn_access_info_nb->bitmask & RRM_OAM_NB_ATTACH_WITHOUT_PDN_CONNECTIVITY_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_plmn_access_info_nb->attach_without_pdn_connectivity_nb > 0))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_plmn_access_info_nb->attach_without_pdn_connectivity_nb] should be in range [%u <= 0] !", p_rrm_oam_plmn_access_info_nb->attach_without_pdn_connectivity_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_plmn_access_info_nb->attach_without_pdn_connectivity_nb, "attach_without_pdn_connectivity_nb");
    *pp_buffer += sizeof(p_rrm_oam_plmn_access_info_nb->attach_without_pdn_connectivity_nb);
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_general_epc_params_nb_len
(
    rrm_oam_general_epc_params_nb_t *p_rrm_oam_general_epc_params_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_general_epc_params_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb < 1) || (p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb > RRM_OAM_MAX_NUM_PLMNS))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb] should be in range [1 <= %d <= RRM_OAM_MAX_NUM_PLMNS] !", p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb);
#endif
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb; loop++)
        {
            length += rrm_get_rrm_oam_plmn_access_info_nb_len(&p_rrm_oam_general_epc_params_nb->plmn_list_nb[loop]);
        }
    }
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_oam_general_epc_params_nb->tac);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_general_epc_params_nb
(
    U8  **pp_buffer,
    rrm_oam_general_epc_params_nb_t *p_rrm_oam_general_epc_params_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_general_epc_params_nb != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb < 1) || (p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb > RRM_OAM_MAX_NUM_PLMNS))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb] should be in range [1 <= %u <= RRM_OAM_MAX_NUM_PLMNS] !", p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb, "num_valid_plmn_nb");
    *pp_buffer += sizeof(p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb; loop++)
        {
            if (OAM_FAILURE == oam_compose_rrm_oam_plmn_access_info_nb(pp_buffer, &p_rrm_oam_general_epc_params_nb->plmn_list_nb[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_general_epc_params_nb->tac); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_general_epc_params_nb->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_epc_nb_len
(
    rrm_oam_epc_nb_t *p_rrm_oam_epc_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_epc_nb != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_general_epc_params_nb_len(&p_rrm_oam_epc_nb->general_epc_params_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_epc_nb->max_harq_retrans_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_epc_nb
(
    U8  **pp_buffer,
    rrm_oam_epc_nb_t *p_rrm_oam_epc_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_epc_nb != PNULL);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_general_epc_params_nb(pp_buffer, &p_rrm_oam_epc_nb->general_epc_params_nb))
    {
        return OAM_FAILURE;
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_epc_nb->max_harq_retrans_nb < 1) || (p_rrm_oam_epc_nb->max_harq_retrans_nb > 8))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_epc_nb->max_harq_retrans_nb] should be in range [1 <= %u <= 8] !", p_rrm_oam_epc_nb->max_harq_retrans_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_epc_nb->max_harq_retrans_nb, "max_harq_retrans_nb");
    *pp_buffer += sizeof(p_rrm_oam_epc_nb->max_harq_retrans_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_rrmc_mac_config_nb_len
(
    rrm_oam_rrmc_mac_config_nb_t *p_rrm_oam_rrmc_mac_config_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_rrmc_mac_config_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rrmc_mac_config_nb->start_rarnti_range_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rrmc_mac_config_nb->end_rarnti_range_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_rrmc_mac_config_nb->ue_inactivity_timer_config_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_rrmc_mac_config_nb
(
    U8  **pp_buffer,
    rrm_oam_rrmc_mac_config_nb_t *p_rrm_oam_rrmc_mac_config_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_rrmc_mac_config_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_rrmc_mac_config_nb->start_rarnti_range_nb, "start_rarnti_range_nb");
    *pp_buffer += sizeof(p_rrm_oam_rrmc_mac_config_nb->start_rarnti_range_nb);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_rrmc_mac_config_nb->end_rarnti_range_nb, "end_rarnti_range_nb");
    *pp_buffer += sizeof(p_rrm_oam_rrmc_mac_config_nb->end_rarnti_range_nb);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_rrmc_mac_config_nb->ue_inactivity_timer_config_nb, "ue_inactivity_timer_config_nb");
    *pp_buffer += sizeof(p_rrm_oam_rrmc_mac_config_nb->ue_inactivity_timer_config_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_cell_selection_info_v13_nb_len
(
    rrm_oam_cell_selection_info_v13_nb_t *p_rrm_oam_cell_selection_info_v13_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_cell_selection_info_v13_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_cell_selection_info_v13_nb->q_qual_min_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_cell_selection_info_v13_nb->q_rx_lev_min_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_cell_selection_info_v13_nb
(
    U8  **pp_buffer,
    rrm_oam_cell_selection_info_v13_nb_t *p_rrm_oam_cell_selection_info_v13_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_cell_selection_info_v13_nb != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_cell_selection_info_v13_nb->q_qual_min_r13 < -34) || (p_rrm_oam_cell_selection_info_v13_nb->q_qual_min_r13 > -3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_cell_selection_info_v13_nb->q_qual_min_r13] should be in range [-34 <= %d <= -3] !", p_rrm_oam_cell_selection_info_v13_nb->q_qual_min_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT8(*pp_buffer, &p_rrm_oam_cell_selection_info_v13_nb->q_qual_min_r13, "q_qual_min_r13");
    *pp_buffer += sizeof(p_rrm_oam_cell_selection_info_v13_nb->q_qual_min_r13);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_cell_selection_info_v13_nb->q_rx_lev_min_r13 < -70) || (p_rrm_oam_cell_selection_info_v13_nb->q_rx_lev_min_r13 > -22))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_cell_selection_info_v13_nb->q_rx_lev_min_r13] should be in range [-70 <= %d <= -22] !", p_rrm_oam_cell_selection_info_v13_nb->q_rx_lev_min_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT8(*pp_buffer, &p_rrm_oam_cell_selection_info_v13_nb->q_rx_lev_min_r13, "q_rx_lev_min_r13");
    *pp_buffer += sizeof(p_rrm_oam_cell_selection_info_v13_nb->q_rx_lev_min_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_sib_mapping_info_nb_len
(
    rrm_oam_sib_mapping_info_nb_t *p_rrm_oam_sib_mapping_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_sib_mapping_info_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_sib_mapping_info_nb->sib_type_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_sib_mapping_info_nb
(
    U8  **pp_buffer,
    rrm_oam_sib_mapping_info_nb_t *p_rrm_oam_sib_mapping_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_sib_mapping_info_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_sib_mapping_info_nb->sib_type_nb > 4))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_mapping_info_nb->sib_type_nb] should be in range [%u <= 4] !", p_rrm_oam_sib_mapping_info_nb->sib_type_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_sib_mapping_info_nb->sib_type_nb, "sib_type_nb");
    *pp_buffer += sizeof(p_rrm_oam_sib_mapping_info_nb->sib_type_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_scheduling_info_nb_len
(
    rrm_oam_scheduling_info_nb_t *p_rrm_oam_scheduling_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_scheduling_info_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_scheduling_info_nb->si_periodicity_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_scheduling_info_nb->si_repetition_pattern_n);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_scheduling_info_nb->si_mapping_count);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_scheduling_info_nb->si_mapping_count > RRM_OAM_MAX_SIB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_scheduling_info_nb->si_mapping_count] should be in range [%d <= RRM_OAM_MAX_SIB] !", p_rrm_oam_scheduling_info_nb->si_mapping_count);
#endif
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_scheduling_info_nb->si_mapping_count; loop++)
        {
            length += rrm_get_rrm_oam_sib_mapping_info_nb_len(&p_rrm_oam_scheduling_info_nb->sib_mapping_info_nb[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_scheduling_info_nb
(
    U8  **pp_buffer,
    rrm_oam_scheduling_info_nb_t *p_rrm_oam_scheduling_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_scheduling_info_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_scheduling_info_nb->si_periodicity_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_scheduling_info_nb->si_periodicity_nb] should be in range [%u <= 7] !", p_rrm_oam_scheduling_info_nb->si_periodicity_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_scheduling_info_nb->si_periodicity_nb, "si_periodicity_nb");
    *pp_buffer += sizeof(p_rrm_oam_scheduling_info_nb->si_periodicity_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_scheduling_info_nb->si_repetition_pattern_n > 4))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_scheduling_info_nb->si_repetition_pattern_n] should be in range [%u <= 4] !", p_rrm_oam_scheduling_info_nb->si_repetition_pattern_n);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_scheduling_info_nb->si_repetition_pattern_n, "si_repetition_pattern_n");
    *pp_buffer += sizeof(p_rrm_oam_scheduling_info_nb->si_repetition_pattern_n);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_scheduling_info_nb->si_mapping_count > RRM_OAM_MAX_SIB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_scheduling_info_nb->si_mapping_count] should be in range [%u <= RRM_OAM_MAX_SIB] !", p_rrm_oam_scheduling_info_nb->si_mapping_count);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_scheduling_info_nb->si_mapping_count, "si_mapping_count");
    *pp_buffer += sizeof(p_rrm_oam_scheduling_info_nb->si_mapping_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_scheduling_info_nb->si_mapping_count; loop++)
        {
            if (OAM_FAILURE == oam_compose_rrm_oam_sib_mapping_info_nb(pp_buffer, &p_rrm_oam_scheduling_info_nb->sib_mapping_info_nb[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_sib_type_1_info_nb_len
(
    rrm_oam_sib_type_1_info_nb_t *p_rrm_oam_sib_type_1_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_sib_type_1_info_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_sib_type_1_info_nb->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_cell_selection_info_v13_nb_len(&p_rrm_oam_sib_type_1_info_nb->cell_selection_info_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_sib_type_1_info_nb->si_window_length_nb);

    /* Optional element */
    if(p_rrm_oam_sib_type_1_info_nb->bitmask & RRM_OAM_NB_SI_RADIO_FRAME_OFFSET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_sib_type_1_info_nb->si_radio_frame_offset_nb);
    }

    /* Optional element */
    if(p_rrm_oam_sib_type_1_info_nb->bitmask & RRM_OAM_NB_PMAX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_sib_type_1_info_nb->pmax_r13);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_sib_type_1_info_nb->scheduling_info_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_sib_type_1_info_nb->scheduling_info_count < 1) || (p_rrm_oam_sib_type_1_info_nb->scheduling_info_count > RRM_OAM_MAX_NUM_SI))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_type_1_info_nb->scheduling_info_count] should be in range [1 <= %d <= RRM_OAM_MAX_NUM_SI] !", p_rrm_oam_sib_type_1_info_nb->scheduling_info_count);
#endif
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_sib_type_1_info_nb->scheduling_info_count; loop++)
        {
            length += rrm_get_rrm_oam_scheduling_info_nb_len(&p_rrm_oam_sib_type_1_info_nb->scheduling_info_nb[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_sib_type_1_info_nb
(
    U8  **pp_buffer,
    rrm_oam_sib_type_1_info_nb_t *p_rrm_oam_sib_type_1_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_sib_type_1_info_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_sib_type_1_info_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_sib_type_1_info_nb->bitmask);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_cell_selection_info_v13_nb(pp_buffer, &p_rrm_oam_sib_type_1_info_nb->cell_selection_info_nb))
    {
        return OAM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_sib_type_1_info_nb->si_window_length_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_type_1_info_nb->si_window_length_nb] should be in range [%u <= 7] !", p_rrm_oam_sib_type_1_info_nb->si_window_length_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_sib_type_1_info_nb->si_window_length_nb, "si_window_length_nb");
    *pp_buffer += sizeof(p_rrm_oam_sib_type_1_info_nb->si_window_length_nb);

    /* Optional element */
    if(p_rrm_oam_sib_type_1_info_nb->bitmask & RRM_OAM_NB_SI_RADIO_FRAME_OFFSET_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_sib_type_1_info_nb->si_radio_frame_offset_nb < 1) || (p_rrm_oam_sib_type_1_info_nb->si_radio_frame_offset_nb > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_type_1_info_nb->si_radio_frame_offset_nb] should be in range [1 <= %u <= 15] !", p_rrm_oam_sib_type_1_info_nb->si_radio_frame_offset_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_sib_type_1_info_nb->si_radio_frame_offset_nb, "si_radio_frame_offset_nb");
    *pp_buffer += sizeof(p_rrm_oam_sib_type_1_info_nb->si_radio_frame_offset_nb);
    }

    /* Optional element */
    if(p_rrm_oam_sib_type_1_info_nb->bitmask & RRM_OAM_NB_PMAX_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_sib_type_1_info_nb->pmax_r13 < -30) || (p_rrm_oam_sib_type_1_info_nb->pmax_r13 > 33))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_type_1_info_nb->pmax_r13] should be in range [-30 <= %d <= 33] !", p_rrm_oam_sib_type_1_info_nb->pmax_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT8(*pp_buffer, &p_rrm_oam_sib_type_1_info_nb->pmax_r13, "pmax_r13");
    *pp_buffer += sizeof(p_rrm_oam_sib_type_1_info_nb->pmax_r13);
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_sib_type_1_info_nb->scheduling_info_count < 1) || (p_rrm_oam_sib_type_1_info_nb->scheduling_info_count > RRM_OAM_MAX_NUM_SI))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_type_1_info_nb->scheduling_info_count] should be in range [1 <= %u <= RRM_OAM_MAX_NUM_SI] !", p_rrm_oam_sib_type_1_info_nb->scheduling_info_count);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_sib_type_1_info_nb->scheduling_info_count, "scheduling_info_count");
    *pp_buffer += sizeof(p_rrm_oam_sib_type_1_info_nb->scheduling_info_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_sib_type_1_info_nb->scheduling_info_count; loop++)
        {
            if (OAM_FAILURE == oam_compose_rrm_oam_scheduling_info_nb(pp_buffer, &p_rrm_oam_sib_type_1_info_nb->scheduling_info_nb[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_paging_info_nb_len
(
    rrm_oam_paging_info_nb_t *p_rrm_oam_paging_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_paging_info_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_paging_info_nb->default_paging_cycle_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_paging_info_nb->nB_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_paging_info_nb->npdcch_num_repetition_paging_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_paging_info_nb
(
    U8  **pp_buffer,
    rrm_oam_paging_info_nb_t *p_rrm_oam_paging_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_paging_info_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_paging_info_nb->default_paging_cycle_nb > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_paging_info_nb->default_paging_cycle_nb] should be in range [%u <= 3] !", p_rrm_oam_paging_info_nb->default_paging_cycle_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_paging_info_nb->default_paging_cycle_nb, "default_paging_cycle_nb");
    *pp_buffer += sizeof(p_rrm_oam_paging_info_nb->default_paging_cycle_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_paging_info_nb->nB_r13 > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_paging_info_nb->nB_r13] should be in range [%u <= 15] !", p_rrm_oam_paging_info_nb->nB_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_paging_info_nb->nB_r13, "nB_r13");
    *pp_buffer += sizeof(p_rrm_oam_paging_info_nb->nB_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_paging_info_nb->npdcch_num_repetition_paging_nb > 11))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_paging_info_nb->npdcch_num_repetition_paging_nb] should be in range [%u <= 11] !", p_rrm_oam_paging_info_nb->npdcch_num_repetition_paging_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_paging_info_nb->npdcch_num_repetition_paging_nb, "npdcch_num_repetition_paging_nb");
    *pp_buffer += sizeof(p_rrm_oam_paging_info_nb->npdcch_num_repetition_paging_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_radio_resource_config_common_sib_nb_len
(
    rrm_oam_radio_resource_config_common_sib_nb_t *p_rrm_oam_radio_resource_config_common_sib_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_radio_resource_config_common_sib_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_radio_resource_config_common_sib_nb->modification_period_coeff_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_paging_info_nb_len(&p_rrm_oam_radio_resource_config_common_sib_nb->paging_info_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_radio_resource_config_common_sib_nb
(
    U8  **pp_buffer,
    rrm_oam_radio_resource_config_common_sib_nb_t *p_rrm_oam_radio_resource_config_common_sib_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_radio_resource_config_common_sib_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_radio_resource_config_common_sib_nb->modification_period_coeff_nb > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_radio_resource_config_common_sib_nb->modification_period_coeff_nb] should be in range [%u <= 3] !", p_rrm_oam_radio_resource_config_common_sib_nb->modification_period_coeff_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_radio_resource_config_common_sib_nb->modification_period_coeff_nb, "modification_period_coeff_nb");
    *pp_buffer += sizeof(p_rrm_oam_radio_resource_config_common_sib_nb->modification_period_coeff_nb);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_paging_info_nb(pp_buffer, &p_rrm_oam_radio_resource_config_common_sib_nb->paging_info_nb))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_carrier_freq_sib_nb_len
(
    rrm_oam_carrier_freq_sib_nb_t *p_rrm_oam_carrier_freq_sib_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_carrier_freq_sib_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_carrier_freq_sib_nb->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_carrier_freq_sib_nb->carrier_freq_nb);

    /* Optional element */
    if(p_rrm_oam_carrier_freq_sib_nb->bitmask & RRM_OAM_NB_CARRIER_FREQ_OFFSET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_carrier_freq_sib_nb->carrier_freq_offset_nb);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_carrier_freq_sib_nb
(
    U8  **pp_buffer,
    rrm_oam_carrier_freq_sib_nb_t *p_rrm_oam_carrier_freq_sib_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_carrier_freq_sib_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_carrier_freq_sib_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_carrier_freq_sib_nb->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_carrier_freq_sib_nb->carrier_freq_nb > MAX_EARFCN_2_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_carrier_freq_sib_nb->carrier_freq_nb] should be in range [%u <= MAX_EARFCN_2_NB] !", p_rrm_oam_carrier_freq_sib_nb->carrier_freq_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_carrier_freq_sib_nb->carrier_freq_nb, "carrier_freq_nb");
    *pp_buffer += sizeof(p_rrm_oam_carrier_freq_sib_nb->carrier_freq_nb);

    /* Optional element */
    if(p_rrm_oam_carrier_freq_sib_nb->bitmask & RRM_OAM_NB_CARRIER_FREQ_OFFSET_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_carrier_freq_sib_nb->carrier_freq_offset_nb > 20))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_carrier_freq_sib_nb->carrier_freq_offset_nb] should be in range [%u <= 20] !", p_rrm_oam_carrier_freq_sib_nb->carrier_freq_offset_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_carrier_freq_sib_nb->carrier_freq_offset_nb, "carrier_freq_offset_nb");
    *pp_buffer += sizeof(p_rrm_oam_carrier_freq_sib_nb->carrier_freq_offset_nb);
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_freq_info_sib_nb_len
(
    rrm_oam_freq_info_sib_nb_t *p_rrm_oam_freq_info_sib_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_freq_info_sib_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_freq_info_sib_nb->bitmask);

    /* Optional element */
    if(p_rrm_oam_freq_info_sib_nb->bitmask & RRM_OAM_NB_UL_CARRIER_FREQUENCY_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_carrier_freq_sib_nb_len(&p_rrm_oam_freq_info_sib_nb->ul_carrier_freq_nb);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_freq_info_sib_nb->additional_spectrum_emission_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_freq_info_sib_nb
(
    U8  **pp_buffer,
    rrm_oam_freq_info_sib_nb_t *p_rrm_oam_freq_info_sib_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_freq_info_sib_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_freq_info_sib_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_freq_info_sib_nb->bitmask);

    /* Optional element */
    if(p_rrm_oam_freq_info_sib_nb->bitmask & RRM_OAM_NB_UL_CARRIER_FREQUENCY_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_carrier_freq_sib_nb(pp_buffer, &p_rrm_oam_freq_info_sib_nb->ul_carrier_freq_nb))
    {
        return OAM_FAILURE;
    }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_freq_info_sib_nb->additional_spectrum_emission_nb < 1) || (p_rrm_oam_freq_info_sib_nb->additional_spectrum_emission_nb > 32))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_freq_info_sib_nb->additional_spectrum_emission_nb] should be in range [1 <= %u <= 32] !", p_rrm_oam_freq_info_sib_nb->additional_spectrum_emission_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_freq_info_sib_nb->additional_spectrum_emission_nb, "additional_spectrum_emission_nb");
    *pp_buffer += sizeof(p_rrm_oam_freq_info_sib_nb->additional_spectrum_emission_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_multi_band_info_nb_len
(
    rrm_oam_multi_band_info_nb_t *p_rrm_oam_multi_band_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_multi_band_info_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_multi_band_info_nb->additional_spectrum_emmision_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_multi_band_info_nb
(
    U8  **pp_buffer,
    rrm_oam_multi_band_info_nb_t *p_rrm_oam_multi_band_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_multi_band_info_nb != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_multi_band_info_nb->additional_spectrum_emmision_nb < 1) || (p_rrm_oam_multi_band_info_nb->additional_spectrum_emmision_nb > 32))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_multi_band_info_nb->additional_spectrum_emmision_nb] should be in range [1 <= %u <= 32] !", p_rrm_oam_multi_band_info_nb->additional_spectrum_emmision_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_multi_band_info_nb->additional_spectrum_emmision_nb, "additional_spectrum_emmision_nb");
    *pp_buffer += sizeof(p_rrm_oam_multi_band_info_nb->additional_spectrum_emmision_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_multiband_info_list_nb_len
(
    rrm_oam_multiband_info_list_nb_t *p_rrm_oam_multiband_info_list_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_multiband_info_list_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_multiband_info_list_nb->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_multiband_info_list_nb->count < 1) || (p_rrm_oam_multiband_info_list_nb->count > MAX_MULTI_BAND_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_multiband_info_list_nb->count] should be in range [1 <= %d <= MAX_MULTI_BAND_NB] !", p_rrm_oam_multiband_info_list_nb->count);
#endif
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_multiband_info_list_nb->count; loop++)
        {
            length += rrm_get_rrm_oam_multi_band_info_nb_len(&p_rrm_oam_multiband_info_list_nb->multi_band_info_nb[loop]);
        }
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_multiband_info_list_nb
(
    U8  **pp_buffer,
    rrm_oam_multiband_info_list_nb_t *p_rrm_oam_multiband_info_list_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_multiband_info_list_nb != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_multiband_info_list_nb->count < 1) || (p_rrm_oam_multiband_info_list_nb->count > MAX_MULTI_BAND_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_multiband_info_list_nb->count] should be in range [1 <= %u <= MAX_MULTI_BAND_NB] !", p_rrm_oam_multiband_info_list_nb->count);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_multiband_info_list_nb->count, "count");
    *pp_buffer += sizeof(p_rrm_oam_multiband_info_list_nb->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_multiband_info_list_nb->count; loop++)
        {
            if (OAM_FAILURE == oam_compose_rrm_oam_multi_band_info_nb(pp_buffer, &p_rrm_oam_multiband_info_list_nb->multi_band_info_nb[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_sib_type_2_info_nb_len
(
    rrm_oam_sib_type_2_info_nb_t *p_rrm_oam_sib_type_2_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_sib_type_2_info_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_sib_type_2_info_nb->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_radio_resource_config_common_sib_nb_len(&p_rrm_oam_sib_type_2_info_nb->radio_res_config_common_sib_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_freq_info_sib_nb_len(&p_rrm_oam_sib_type_2_info_nb->freq_info_r13);

    /* Optional element */
    if(p_rrm_oam_sib_type_2_info_nb->bitmask & RRM_OAM_NB_MULTI_BAND_INFO_LIST_NB_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_multiband_info_list_nb_len(&p_rrm_oam_sib_type_2_info_nb->multi_band_info_list_r13);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_sib_type_2_info_nb->time_alignment_timer_common_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_sib_type_2_info_nb
(
    U8  **pp_buffer,
    rrm_oam_sib_type_2_info_nb_t *p_rrm_oam_sib_type_2_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_sib_type_2_info_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_sib_type_2_info_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_sib_type_2_info_nb->bitmask);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_radio_resource_config_common_sib_nb(pp_buffer, &p_rrm_oam_sib_type_2_info_nb->radio_res_config_common_sib_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_freq_info_sib_nb(pp_buffer, &p_rrm_oam_sib_type_2_info_nb->freq_info_r13))
    {
        return OAM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_oam_sib_type_2_info_nb->bitmask & RRM_OAM_NB_MULTI_BAND_INFO_LIST_NB_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_multiband_info_list_nb(pp_buffer, &p_rrm_oam_sib_type_2_info_nb->multi_band_info_list_r13))
    {
        return OAM_FAILURE;
    }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_sib_type_2_info_nb->time_alignment_timer_common_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_type_2_info_nb->time_alignment_timer_common_nb] should be in range [%u <= 7] !", p_rrm_oam_sib_type_2_info_nb->time_alignment_timer_common_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_sib_type_2_info_nb->time_alignment_timer_common_nb, "time_alignment_timer_common_nb");
    *pp_buffer += sizeof(p_rrm_oam_sib_type_2_info_nb->time_alignment_timer_common_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_admission_control_info_nb_len
(
    rrm_oam_admission_control_info_nb_t *p_rrm_oam_admission_control_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_admission_control_info_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_admission_control_info_nb->bitmask);

    /* Optional element */
    if(p_rrm_oam_admission_control_info_nb->bitmask & RRM_OAM_NB_MAX_NUM_UE_PER_CELL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_admission_control_info_nb->max_num_ue_per_cell_nb);
    }

    /* Optional element */
    if(p_rrm_oam_admission_control_info_nb->bitmask & RRM_OAM_NB_RESOURCE_RESERVED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_admission_control_info_nb->resource_reserved_for_existing_users_nb);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_admission_control_info_nb
(
    U8  **pp_buffer,
    rrm_oam_admission_control_info_nb_t *p_rrm_oam_admission_control_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_admission_control_info_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_admission_control_info_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_admission_control_info_nb->bitmask);

    /* Optional element */
    if(p_rrm_oam_admission_control_info_nb->bitmask & RRM_OAM_NB_MAX_NUM_UE_PER_CELL_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_admission_control_info_nb->max_num_ue_per_cell_nb < 1) || (p_rrm_oam_admission_control_info_nb->max_num_ue_per_cell_nb > 255))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_admission_control_info_nb->max_num_ue_per_cell_nb] should be in range [1 <= %u <= 255] !", p_rrm_oam_admission_control_info_nb->max_num_ue_per_cell_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_admission_control_info_nb->max_num_ue_per_cell_nb, "max_num_ue_per_cell_nb");
    *pp_buffer += sizeof(p_rrm_oam_admission_control_info_nb->max_num_ue_per_cell_nb);
    }

    /* Optional element */
    if(p_rrm_oam_admission_control_info_nb->bitmask & RRM_OAM_NB_RESOURCE_RESERVED_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_admission_control_info_nb->resource_reserved_for_existing_users_nb > 100))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_admission_control_info_nb->resource_reserved_for_existing_users_nb] should be in range [%u <= 100] !", p_rrm_oam_admission_control_info_nb->resource_reserved_for_existing_users_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_admission_control_info_nb->resource_reserved_for_existing_users_nb, "resource_reserved_for_existing_users_nb");
    *pp_buffer += sizeof(p_rrm_oam_admission_control_info_nb->resource_reserved_for_existing_users_nb);
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_adl_pkt_scheduling_params_nb_len
(
    rrm_oam_adl_pkt_scheduling_params_nb_t *p_rrm_oam_adl_pkt_scheduling_params_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_adl_pkt_scheduling_params_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_adl_pkt_scheduling_params_nb->bitmask);

    /* Optional element */
    if(p_rrm_oam_adl_pkt_scheduling_params_nb->bitmask & RRM_OAM_NB_DL_MCS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_adl_pkt_scheduling_params_nb->dl_mcs_nb);
    }

    /* Optional element */
    if(p_rrm_oam_adl_pkt_scheduling_params_nb->bitmask & RRM_OAM_NB_UL_MCS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_adl_pkt_scheduling_params_nb->ul_mcs_nb);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_adl_pkt_scheduling_params_nb->frequency_selective_scheduling_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_adl_pkt_scheduling_params_nb
(
    U8  **pp_buffer,
    rrm_oam_adl_pkt_scheduling_params_nb_t *p_rrm_oam_adl_pkt_scheduling_params_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_adl_pkt_scheduling_params_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_adl_pkt_scheduling_params_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_adl_pkt_scheduling_params_nb->bitmask);

    /* Optional element */
    if(p_rrm_oam_adl_pkt_scheduling_params_nb->bitmask & RRM_OAM_NB_DL_MCS_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_adl_pkt_scheduling_params_nb->dl_mcs_nb > 12))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_adl_pkt_scheduling_params_nb->dl_mcs_nb] should be in range [%u <= 12] !", p_rrm_oam_adl_pkt_scheduling_params_nb->dl_mcs_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_adl_pkt_scheduling_params_nb->dl_mcs_nb, "dl_mcs_nb");
    *pp_buffer += sizeof(p_rrm_oam_adl_pkt_scheduling_params_nb->dl_mcs_nb);
    }

    /* Optional element */
    if(p_rrm_oam_adl_pkt_scheduling_params_nb->bitmask & RRM_OAM_NB_UL_MCS_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_adl_pkt_scheduling_params_nb->ul_mcs_nb > 12))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_adl_pkt_scheduling_params_nb->ul_mcs_nb] should be in range [%u <= 12] !", p_rrm_oam_adl_pkt_scheduling_params_nb->ul_mcs_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_adl_pkt_scheduling_params_nb->ul_mcs_nb, "ul_mcs_nb");
    *pp_buffer += sizeof(p_rrm_oam_adl_pkt_scheduling_params_nb->ul_mcs_nb);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_adl_pkt_scheduling_params_nb->frequency_selective_scheduling_nb > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_adl_pkt_scheduling_params_nb->frequency_selective_scheduling_nb] should be in range [%u <= 3] !", p_rrm_oam_adl_pkt_scheduling_params_nb->frequency_selective_scheduling_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_adl_pkt_scheduling_params_nb->frequency_selective_scheduling_nb, "frequency_selective_scheduling_nb");
    *pp_buffer += sizeof(p_rrm_oam_adl_pkt_scheduling_params_nb->frequency_selective_scheduling_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_adl_cell_params_nb_len
(
    rrm_oam_adl_cell_params_nb_t *p_rrm_oam_adl_cell_params_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_adl_cell_params_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_adl_cell_params_nb->sub_carrier_spacing_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_adl_cell_params_nb->dl_cyclic_prefix_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_adl_cell_params_nb
(
    U8  **pp_buffer,
    rrm_oam_adl_cell_params_nb_t *p_rrm_oam_adl_cell_params_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_adl_cell_params_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_adl_cell_params_nb->sub_carrier_spacing_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_adl_cell_params_nb->sub_carrier_spacing_nb] should be in range [%u <= 1] !", p_rrm_oam_adl_cell_params_nb->sub_carrier_spacing_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_adl_cell_params_nb->sub_carrier_spacing_nb, "sub_carrier_spacing_nb");
    *pp_buffer += sizeof(p_rrm_oam_adl_cell_params_nb->sub_carrier_spacing_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_adl_cell_params_nb->dl_cyclic_prefix_nb > 0))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_adl_cell_params_nb->dl_cyclic_prefix_nb] should be in range [%u <= 0] !", p_rrm_oam_adl_cell_params_nb->dl_cyclic_prefix_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_adl_cell_params_nb->dl_cyclic_prefix_nb, "dl_cyclic_prefix_nb");
    *pp_buffer += sizeof(p_rrm_oam_adl_cell_params_nb->dl_cyclic_prefix_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_transmission_mode_table_nb_len
(
    rrm_oam_transmission_mode_table_nb_t *p_rrm_oam_transmission_mode_table_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_transmission_mode_table_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_transmission_mode_table_nb->sinr_level_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_transmission_mode_table_nb->pathloss_level_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_transmission_mode_table_nb->mobility_level_nb);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_transmission_mode_table_nb->tm_mode_nb);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_transmission_mode_table_nb
(
    U8  **pp_buffer,
    rrm_oam_transmission_mode_table_nb_t *p_rrm_oam_transmission_mode_table_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_transmission_mode_table_nb != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_transmission_mode_table_nb->sinr_level_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_transmission_mode_table_nb->sinr_level_nb] should be in range [%u <= 1] !", p_rrm_oam_transmission_mode_table_nb->sinr_level_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_transmission_mode_table_nb->sinr_level_nb, "sinr_level_nb");
    *pp_buffer += sizeof(p_rrm_oam_transmission_mode_table_nb->sinr_level_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_transmission_mode_table_nb->pathloss_level_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_transmission_mode_table_nb->pathloss_level_nb] should be in range [%u <= 1] !", p_rrm_oam_transmission_mode_table_nb->pathloss_level_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_transmission_mode_table_nb->pathloss_level_nb, "pathloss_level_nb");
    *pp_buffer += sizeof(p_rrm_oam_transmission_mode_table_nb->pathloss_level_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_transmission_mode_table_nb->mobility_level_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_transmission_mode_table_nb->mobility_level_nb] should be in range [%u <= 1] !", p_rrm_oam_transmission_mode_table_nb->mobility_level_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_transmission_mode_table_nb->mobility_level_nb, "mobility_level_nb");
    *pp_buffer += sizeof(p_rrm_oam_transmission_mode_table_nb->mobility_level_nb);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_transmission_mode_table_nb->tm_mode_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_transmission_mode_table_nb->tm_mode_nb] should be in range [%u <= 1] !", p_rrm_oam_transmission_mode_table_nb->tm_mode_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_transmission_mode_table_nb->tm_mode_nb, "tm_mode_nb");
    *pp_buffer += sizeof(p_rrm_oam_transmission_mode_table_nb->tm_mode_nb);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_transmission_mode_params_nb_len
(
    rrm_oam_transmission_mode_params_nb_t *p_rrm_oam_transmission_mode_params_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_transmission_mode_params_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_transmission_mode_params_nb->bitmask);

    /* Optional element */
    if(p_rrm_oam_transmission_mode_params_nb->bitmask & RRM_OAM_NB_TRANSMISSION_MODE_TABLE_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_transmission_mode_table_nb_len(&p_rrm_oam_transmission_mode_params_nb->tm_mode_table);
    }

    /* Optional element */
    if(p_rrm_oam_transmission_mode_params_nb->bitmask & RRM_OAM_NB_SINR_THRESHOLD_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_transmission_mode_params_nb->sinr_threshold_nb);
    }

    /* Optional element */
    if(p_rrm_oam_transmission_mode_params_nb->bitmask & RRM_OAM_NB_PATHLOSS_THRESHOLD_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_transmission_mode_params_nb->pathloss_threshold_nb);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_transmission_mode_params_nb
(
    U8  **pp_buffer,
    rrm_oam_transmission_mode_params_nb_t *p_rrm_oam_transmission_mode_params_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_transmission_mode_params_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_transmission_mode_params_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_transmission_mode_params_nb->bitmask);

    /* Optional element */
    if(p_rrm_oam_transmission_mode_params_nb->bitmask & RRM_OAM_NB_TRANSMISSION_MODE_TABLE_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_transmission_mode_table_nb(pp_buffer, &p_rrm_oam_transmission_mode_params_nb->tm_mode_table))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_oam_transmission_mode_params_nb->bitmask & RRM_OAM_NB_SINR_THRESHOLD_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_transmission_mode_params_nb->sinr_threshold_nb > 255))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_transmission_mode_params_nb->sinr_threshold_nb] should be in range [%u <= 255] !", p_rrm_oam_transmission_mode_params_nb->sinr_threshold_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_transmission_mode_params_nb->sinr_threshold_nb, "sinr_threshold_nb");
    *pp_buffer += sizeof(p_rrm_oam_transmission_mode_params_nb->sinr_threshold_nb);
    }

    /* Optional element */
    if(p_rrm_oam_transmission_mode_params_nb->bitmask & RRM_OAM_NB_PATHLOSS_THRESHOLD_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_transmission_mode_params_nb->pathloss_threshold_nb > 200))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_transmission_mode_params_nb->pathloss_threshold_nb] should be in range [%u <= 200] !", p_rrm_oam_transmission_mode_params_nb->pathloss_threshold_nb);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_transmission_mode_params_nb->pathloss_threshold_nb, "pathloss_threshold_nb");
    *pp_buffer += sizeof(p_rrm_oam_transmission_mode_params_nb->pathloss_threshold_nb);
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_eutran_access_point_pos_len
(
    rrm_oam_eutran_access_point_pos_t *p_rrm_oam_eutran_access_point_pos
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_eutran_access_point_pos != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_eutran_access_point_pos->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_eutran_access_point_pos->latitude_sign);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_eutran_access_point_pos->deg_of_latitude);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_eutran_access_point_pos->deg_of_longitude);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_eutran_access_point_pos->dir_of_altitude);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_eutran_access_point_pos->altitude);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_eutran_access_point_pos->uncertainty_semi_major);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_eutran_access_point_pos->uncertainty_semi_minor);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_eutran_access_point_pos->orientation_of_major_axis);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_eutran_access_point_pos->uncertainty_altitude);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_eutran_access_point_pos->confidence);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_eutran_access_point_pos
(
    U8  **pp_buffer,
    rrm_oam_eutran_access_point_pos_t *p_rrm_oam_eutran_access_point_pos
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_eutran_access_point_pos != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_eutran_access_point_pos->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_eutran_access_point_pos->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->latitude_sign > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->latitude_sign] should be in range [%u <= 1] !", p_rrm_oam_eutran_access_point_pos->latitude_sign);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_eutran_access_point_pos->latitude_sign, "latitude_sign");
    *pp_buffer += sizeof(p_rrm_oam_eutran_access_point_pos->latitude_sign);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->deg_of_latitude > 90))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->deg_of_latitude] should be in range [%u <= 90] !", p_rrm_oam_eutran_access_point_pos->deg_of_latitude);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_eutran_access_point_pos->deg_of_latitude, "deg_of_latitude");
    *pp_buffer += sizeof(p_rrm_oam_eutran_access_point_pos->deg_of_latitude);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_eutran_access_point_pos->deg_of_longitude < -180) || (p_rrm_oam_eutran_access_point_pos->deg_of_longitude > 180))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->deg_of_longitude] should be in range [-180 <= %d <= 180] !", p_rrm_oam_eutran_access_point_pos->deg_of_longitude);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_SINT16(*pp_buffer, &p_rrm_oam_eutran_access_point_pos->deg_of_longitude, "deg_of_longitude");
    *pp_buffer += sizeof(p_rrm_oam_eutran_access_point_pos->deg_of_longitude);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->dir_of_altitude > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->dir_of_altitude] should be in range [%u <= 1] !", p_rrm_oam_eutran_access_point_pos->dir_of_altitude);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_eutran_access_point_pos->dir_of_altitude, "dir_of_altitude");
    *pp_buffer += sizeof(p_rrm_oam_eutran_access_point_pos->dir_of_altitude);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->altitude > 32767))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->altitude] should be in range [%u <= 32767] !", p_rrm_oam_eutran_access_point_pos->altitude);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_eutran_access_point_pos->altitude, "altitude");
    *pp_buffer += sizeof(p_rrm_oam_eutran_access_point_pos->altitude);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->uncertainty_semi_major > 1800000))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->uncertainty_semi_major] should be in range [%u <= 1800000] !", p_rrm_oam_eutran_access_point_pos->uncertainty_semi_major);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_eutran_access_point_pos->uncertainty_semi_major, "uncertainty_semi_major");
    *pp_buffer += sizeof(p_rrm_oam_eutran_access_point_pos->uncertainty_semi_major);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->uncertainty_semi_minor > 1800000))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->uncertainty_semi_minor] should be in range [%u <= 1800000] !", p_rrm_oam_eutran_access_point_pos->uncertainty_semi_minor);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_eutran_access_point_pos->uncertainty_semi_minor, "uncertainty_semi_minor");
    *pp_buffer += sizeof(p_rrm_oam_eutran_access_point_pos->uncertainty_semi_minor);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->orientation_of_major_axis > 179))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->orientation_of_major_axis] should be in range [%u <= 179] !", p_rrm_oam_eutran_access_point_pos->orientation_of_major_axis);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_eutran_access_point_pos->orientation_of_major_axis, "orientation_of_major_axis");
    *pp_buffer += sizeof(p_rrm_oam_eutran_access_point_pos->orientation_of_major_axis);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->uncertainty_altitude > 990))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->uncertainty_altitude] should be in range [%u <= 990] !", p_rrm_oam_eutran_access_point_pos->uncertainty_altitude);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_eutran_access_point_pos->uncertainty_altitude, "uncertainty_altitude");
    *pp_buffer += sizeof(p_rrm_oam_eutran_access_point_pos->uncertainty_altitude);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->confidence > 100))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->confidence] should be in range [%u <= 100] !", p_rrm_oam_eutran_access_point_pos->confidence);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_eutran_access_point_pos->confidence, "confidence");
    *pp_buffer += sizeof(p_rrm_oam_eutran_access_point_pos->confidence);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_max_ue_schedule_dl_ul_len
(
    rrm_oam_max_ue_schedule_dl_ul_t *p_rrm_oam_max_ue_schedule_dl_ul
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_max_ue_schedule_dl_ul != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_max_ue_schedule_dl_ul->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_dl);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_ul);

    /* Optional element */
    if(p_rrm_oam_max_ue_schedule_dl_ul->bitmask & RRM_OAM_UE_SCHEDULING_HYSTERESIS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_max_ue_schedule_dl_ul->ue_scheduling_hysteresis);
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_max_ue_schedule_dl_ul->active_ue_reports_timer);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_max_ue_schedule_dl_ul
(
    U8  **pp_buffer,
    rrm_oam_max_ue_schedule_dl_ul_t *p_rrm_oam_max_ue_schedule_dl_ul
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_max_ue_schedule_dl_ul != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_max_ue_schedule_dl_ul->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_max_ue_schedule_dl_ul->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_dl < 1) || (p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_dl > 16))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_dl] should be in range [1 <= %u <= 16] !", p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_dl);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_dl, "max_ue_scheduled_dl");
    *pp_buffer += sizeof(p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_dl);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_ul < 1) || (p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_ul > 16))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_ul] should be in range [1 <= %u <= 16] !", p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_ul);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_ul, "max_ue_scheduled_ul");
    *pp_buffer += sizeof(p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_ul);

    /* Optional element */
    if(p_rrm_oam_max_ue_schedule_dl_ul->bitmask & RRM_OAM_UE_SCHEDULING_HYSTERESIS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_max_ue_schedule_dl_ul->ue_scheduling_hysteresis < 1) || (p_rrm_oam_max_ue_schedule_dl_ul->ue_scheduling_hysteresis > 14))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_max_ue_schedule_dl_ul->ue_scheduling_hysteresis] should be in range [1 <= %u <= 14] !", p_rrm_oam_max_ue_schedule_dl_ul->ue_scheduling_hysteresis);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_max_ue_schedule_dl_ul->ue_scheduling_hysteresis, "ue_scheduling_hysteresis");
    *pp_buffer += sizeof(p_rrm_oam_max_ue_schedule_dl_ul->ue_scheduling_hysteresis);
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_max_ue_schedule_dl_ul->active_ue_reports_timer < 1) || (p_rrm_oam_max_ue_schedule_dl_ul->active_ue_reports_timer > 255))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_max_ue_schedule_dl_ul->active_ue_reports_timer] should be in range [1 <= %u <= 255] !", p_rrm_oam_max_ue_schedule_dl_ul->active_ue_reports_timer);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_max_ue_schedule_dl_ul->active_ue_reports_timer, "active_ue_reports_timer");
    *pp_buffer += sizeof(p_rrm_oam_max_ue_schedule_dl_ul->active_ue_reports_timer);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_operator_info_nb_len
(
    rrm_oam_operator_info_nb_t *p_rrm_oam_operator_info_nb
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_operator_info_nb != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_operator_info_nb->bitmask);

    /* Get length of IE */
    length += rrm_get_rrm_oam_rrmc_mac_config_nb_len(&p_rrm_oam_operator_info_nb->rrm_mac_config_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_sib_type_1_info_nb_len(&p_rrm_oam_operator_info_nb->sib_1_info_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_sib_type_2_info_nb_len(&p_rrm_oam_operator_info_nb->sib_2_info_nb);

    /* Get length of IE */
    length += rrm_get_rrm_oam_admission_control_info_nb_len(&p_rrm_oam_operator_info_nb->admission_control_info_nb);

    /* Optional element */
    if(p_rrm_oam_operator_info_nb->bitmask & RRM_OAM_NB_ADD_PACKET_SCH_PARAMS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_adl_pkt_scheduling_params_nb_len(&p_rrm_oam_operator_info_nb->additional_packet_scheduling_params_nb);
    }

    /* Get length of IE */
    length += rrm_get_rrm_oam_adl_cell_params_nb_len(&p_rrm_oam_operator_info_nb->additional_cell_params_nb);

    /* Optional element */
    if(p_rrm_oam_operator_info_nb->bitmask & RRM_OAM_NB_TRANSMISSION_MODE_PARAMS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_transmission_mode_params_nb_len(&p_rrm_oam_operator_info_nb->transmission_mode_params_nb);
    }

    /* Optional element */
    if(p_rrm_oam_operator_info_nb->bitmask & RRM_OAM_NB_EUTRAN_ACCESS_POINT_POS_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_access_point_pos_len(&p_rrm_oam_operator_info_nb->rrm_eutran_access_point_pos_nb);
    }

    /* Optional element */
    if(p_rrm_oam_operator_info_nb->bitmask & RRM_OAM_NB_DYNAMIC_UE_SCHEDULING_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_max_ue_schedule_dl_ul_len(&p_rrm_oam_operator_info_nb->rrm_ue_scheduling_info_nb);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_operator_info_nb
(
    U8  **pp_buffer,
    rrm_oam_operator_info_nb_t *p_rrm_oam_operator_info_nb
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_operator_info_nb != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_operator_info_nb->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_operator_info_nb->bitmask);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_rrmc_mac_config_nb(pp_buffer, &p_rrm_oam_operator_info_nb->rrm_mac_config_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_sib_type_1_info_nb(pp_buffer, &p_rrm_oam_operator_info_nb->sib_1_info_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_sib_type_2_info_nb(pp_buffer, &p_rrm_oam_operator_info_nb->sib_2_info_nb))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_admission_control_info_nb(pp_buffer, &p_rrm_oam_operator_info_nb->admission_control_info_nb))
    {
        return OAM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_oam_operator_info_nb->bitmask & RRM_OAM_NB_ADD_PACKET_SCH_PARAMS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_adl_pkt_scheduling_params_nb(pp_buffer, &p_rrm_oam_operator_info_nb->additional_packet_scheduling_params_nb))
    {
        return OAM_FAILURE;
    }
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_adl_cell_params_nb(pp_buffer, &p_rrm_oam_operator_info_nb->additional_cell_params_nb))
    {
        return OAM_FAILURE;
    }

    /* Optional element */
    if(p_rrm_oam_operator_info_nb->bitmask & RRM_OAM_NB_TRANSMISSION_MODE_PARAMS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_transmission_mode_params_nb(pp_buffer, &p_rrm_oam_operator_info_nb->transmission_mode_params_nb))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_oam_operator_info_nb->bitmask & RRM_OAM_NB_EUTRAN_ACCESS_POINT_POS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_eutran_access_point_pos(pp_buffer, &p_rrm_oam_operator_info_nb->rrm_eutran_access_point_pos_nb))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_oam_operator_info_nb->bitmask & RRM_OAM_NB_DYNAMIC_UE_SCHEDULING_INFO_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_max_ue_schedule_dl_ul(pp_buffer, &p_rrm_oam_operator_info_nb->rrm_ue_scheduling_info_nb))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_l2_comm_info_len
(
    rrm_oam_l2_comm_info_t *p_rrm_oam_l2_comm_info
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_l2_comm_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_l2_comm_info->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_l2_comm_info->port);

    /* Optional element */
    if(p_rrm_oam_l2_comm_info->bitmask & RRM_OAM_IPV4_ADDRESS_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_oam_l2_comm_info->ip_addr);
    }

    /* Optional element */
    if(p_rrm_oam_l2_comm_info->bitmask & RRM_OAM_IPV6_ADDRESS_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrm_oam_l2_comm_info->ip_addr6);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_l2_comm_info
(
    U8  **pp_buffer,
    rrm_oam_l2_comm_info_t *p_rrm_oam_l2_comm_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_l2_comm_info != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_l2_comm_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_l2_comm_info->bitmask);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_l2_comm_info->port, "port");
    *pp_buffer += sizeof(p_rrm_oam_l2_comm_info->port);

    /* Optional element */
    if(p_rrm_oam_l2_comm_info->bitmask & RRM_OAM_IPV4_ADDRESS_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_l2_comm_info->ip_addr); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_l2_comm_info->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_rrm_oam_l2_comm_info->bitmask & RRM_OAM_IPV6_ADDRESS_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_l2_comm_info->ip_addr6); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_l2_comm_info->ip_addr6[loop], "ip_addr6[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_lower_layer_comm_info_len
(
    rrm_oam_lower_layer_comm_info_t *p_rrm_oam_lower_layer_comm_info
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_lower_layer_comm_info != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_l2_comm_info_len(&p_rrm_oam_lower_layer_comm_info->mac_comm_info);

    /* Get length of IE */
    length += rrm_get_rrm_oam_l2_comm_info_len(&p_rrm_oam_lower_layer_comm_info->rlc_comm_info);

    /* Get length of IE */
    length += rrm_get_rrm_oam_l2_comm_info_len(&p_rrm_oam_lower_layer_comm_info->pdcp_comm_info);

    /* Get length of IE */
    length += rrm_get_rrm_oam_l2_comm_info_len(&p_rrm_oam_lower_layer_comm_info->egtpu_comm_info);

    /* Get length of IE */
    length += rrm_get_rrm_oam_l2_comm_info_len(&p_rrm_oam_lower_layer_comm_info->phy_comm_info);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_lower_layer_comm_info
(
    U8  **pp_buffer,
    rrm_oam_lower_layer_comm_info_t *p_rrm_oam_lower_layer_comm_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_lower_layer_comm_info != PNULL);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_l2_comm_info(pp_buffer, &p_rrm_oam_lower_layer_comm_info->mac_comm_info))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_l2_comm_info(pp_buffer, &p_rrm_oam_lower_layer_comm_info->rlc_comm_info))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_l2_comm_info(pp_buffer, &p_rrm_oam_lower_layer_comm_info->pdcp_comm_info))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_l2_comm_info(pp_buffer, &p_rrm_oam_lower_layer_comm_info->egtpu_comm_info))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_l2_comm_info(pp_buffer, &p_rrm_oam_lower_layer_comm_info->phy_comm_info))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_inband_different_pci_nb_r13_len
(
    rrm_oam_inband_different_pci_nb_r13_t *p_rrm_oam_inband_different_pci_nb_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_inband_different_pci_nb_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_inband_different_pci_nb_r13->eutra_num_crs_ports_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_inband_different_pci_nb_r13->raster_offset_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_inband_different_pci_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_inband_different_pci_nb_r13_t *p_rrm_oam_inband_different_pci_nb_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_inband_different_pci_nb_r13 != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_inband_different_pci_nb_r13->eutra_num_crs_ports_r13 > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_inband_different_pci_nb_r13->eutra_num_crs_ports_r13] should be in range [%u <= 1] !", p_rrm_oam_inband_different_pci_nb_r13->eutra_num_crs_ports_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_inband_different_pci_nb_r13->eutra_num_crs_ports_r13, "eutra_num_crs_ports_r13");
    *pp_buffer += sizeof(p_rrm_oam_inband_different_pci_nb_r13->eutra_num_crs_ports_r13);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_inband_different_pci_nb_r13->raster_offset_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_inband_different_pci_nb_r13->raster_offset_r13] should be in range [%u <= 3] !", p_rrm_oam_inband_different_pci_nb_r13->raster_offset_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_inband_different_pci_nb_r13->raster_offset_r13, "raster_offset_r13");
    *pp_buffer += sizeof(p_rrm_oam_inband_different_pci_nb_r13->raster_offset_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_guardband_nb_r13_len
(
    rrm_oam_guardband_nb_r13_t *p_rrm_oam_guardband_nb_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_guardband_nb_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_guardband_nb_r13->raster_offset_r13);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_guardband_nb_r13
(
    U8  **pp_buffer,
    rrm_oam_guardband_nb_r13_t *p_rrm_oam_guardband_nb_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_guardband_nb_r13 != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_guardband_nb_r13->raster_offset_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_guardband_nb_r13->raster_offset_r13] should be in range [%u <= 3] !", p_rrm_oam_guardband_nb_r13->raster_offset_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_guardband_nb_r13->raster_offset_r13, "raster_offset_r13");
    *pp_buffer += sizeof(p_rrm_oam_guardband_nb_r13->raster_offset_r13);

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_operation_mode_info_r13_len
(
    rrm_oam_operation_mode_info_r13_t *p_rrm_oam_operation_mode_info_r13
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_operation_mode_info_r13 != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_operation_mode_info_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_operation_mode_info_r13->bitmask & RRM_OAM_INBAND_SAME_PCI_R13_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_operation_mode_info_r13->inband_same_pci_r13);
    }

    /* Optional element */
    if(p_rrm_oam_operation_mode_info_r13->bitmask & RRM_OAM_INBAND_DIFFERENT_PCI_R13_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_inband_different_pci_nb_r13_len(&p_rrm_oam_operation_mode_info_r13->inband_different_pci_r13);
    }

    /* Optional element */
    if(p_rrm_oam_operation_mode_info_r13->bitmask & RRM_OAM_GUARDBAND_R13_PRESENT)
    {

    /* Get length of IE */
    length += rrm_get_rrm_oam_guardband_nb_r13_len(&p_rrm_oam_operation_mode_info_r13->guardband_r13);
    }

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_operation_mode_info_r13
(
    U8  **pp_buffer,
    rrm_oam_operation_mode_info_r13_t *p_rrm_oam_operation_mode_info_r13
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_operation_mode_info_r13 != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_operation_mode_info_r13->bitmask, "bitmask");
    *pp_buffer += sizeof(p_rrm_oam_operation_mode_info_r13->bitmask);

    /* Optional element */
    if(p_rrm_oam_operation_mode_info_r13->bitmask & RRM_OAM_INBAND_SAME_PCI_R13_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_operation_mode_info_r13->inband_same_pci_r13 > 31))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_operation_mode_info_r13->inband_same_pci_r13] should be in range [%u <= 31] !", p_rrm_oam_operation_mode_info_r13->inband_same_pci_r13);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_operation_mode_info_r13->inband_same_pci_r13, "inband_same_pci_r13");
    *pp_buffer += sizeof(p_rrm_oam_operation_mode_info_r13->inband_same_pci_r13);
    }

    /* Optional element */
    if(p_rrm_oam_operation_mode_info_r13->bitmask & RRM_OAM_INBAND_DIFFERENT_PCI_R13_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_inband_different_pci_nb_r13(pp_buffer, &p_rrm_oam_operation_mode_info_r13->inband_different_pci_r13))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_rrm_oam_operation_mode_info_r13->bitmask & RRM_OAM_GUARDBAND_R13_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_guardband_nb_r13(pp_buffer, &p_rrm_oam_operation_mode_info_r13->guardband_r13))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

static
rrm_length_t
rrm_get_rrm_oam_nb_cell_info_len
(
    rrm_oam_nb_cell_info_t *p_rrm_oam_nb_cell_info
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_nb_cell_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_info->operation_type);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_info->ul_sub_carrier_allocation);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_info->ul_sub_carrier_spacing);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_info->rnti_start_range);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_info->rnti_end_range);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_info->nb_dl_mod_scheme);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_info->nb_ul_mod_scheme);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_dl);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_ul);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_info->avg_num_am_lc_per_ue);

    /* Get length of IE */
    length += rrm_get_rrm_oam_operation_mode_info_r13_len(&p_rrm_oam_nb_cell_info->operation_mode_info_r13);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_info->nb_mcs_msg3_npusch);

    return length;
}

static
oam_return_et
oam_compose_rrm_oam_nb_cell_info
(
    U8  **pp_buffer,
    rrm_oam_nb_cell_info_t *p_rrm_oam_nb_cell_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_nb_cell_info != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->operation_type > 4))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->operation_type] should be in range [%u <= 4] !", p_rrm_oam_nb_cell_info->operation_type);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nb_cell_info->operation_type, "operation_type");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_info->operation_type);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->ul_sub_carrier_allocation > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->ul_sub_carrier_allocation] should be in range [%u <= 1] !", p_rrm_oam_nb_cell_info->ul_sub_carrier_allocation);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nb_cell_info->ul_sub_carrier_allocation, "ul_sub_carrier_allocation");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_info->ul_sub_carrier_allocation);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->ul_sub_carrier_spacing > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->ul_sub_carrier_spacing] should be in range [%u <= 1] !", p_rrm_oam_nb_cell_info->ul_sub_carrier_spacing);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nb_cell_info->ul_sub_carrier_spacing, "ul_sub_carrier_spacing");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_info->ul_sub_carrier_spacing);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_nb_cell_info->rnti_start_range < 1) || (p_rrm_oam_nb_cell_info->rnti_start_range > 65523))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->rnti_start_range] should be in range [1 <= %u <= 65523] !", p_rrm_oam_nb_cell_info->rnti_start_range);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_nb_cell_info->rnti_start_range, "rnti_start_range");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_info->rnti_start_range);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_nb_cell_info->rnti_end_range < 1) || (p_rrm_oam_nb_cell_info->rnti_end_range > 65523))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->rnti_end_range] should be in range [1 <= %u <= 65523] !", p_rrm_oam_nb_cell_info->rnti_end_range);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_rrm_oam_nb_cell_info->rnti_end_range, "rnti_end_range");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_info->rnti_end_range);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->nb_dl_mod_scheme > 0))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->nb_dl_mod_scheme] should be in range [%u <= 0] !", p_rrm_oam_nb_cell_info->nb_dl_mod_scheme);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nb_cell_info->nb_dl_mod_scheme, "nb_dl_mod_scheme");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_info->nb_dl_mod_scheme);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->nb_ul_mod_scheme > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->nb_ul_mod_scheme] should be in range [%u <= 3] !", p_rrm_oam_nb_cell_info->nb_ul_mod_scheme);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nb_cell_info->nb_ul_mod_scheme, "nb_ul_mod_scheme");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_info->nb_ul_mod_scheme);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_dl > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_dl] should be in range [%u <= 15] !", p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_dl);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_dl, "max_nb_ues_to_be_scheduled_dl");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_dl);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_ul > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_ul] should be in range [%u <= 15] !", p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_ul);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_ul, "max_nb_ues_to_be_scheduled_ul");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_ul);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->avg_num_am_lc_per_ue > 2))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->avg_num_am_lc_per_ue] should be in range [%u <= 2] !", p_rrm_oam_nb_cell_info->avg_num_am_lc_per_ue);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nb_cell_info->avg_num_am_lc_per_ue, "avg_num_am_lc_per_ue");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_info->avg_num_am_lc_per_ue);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_operation_mode_info_r13(pp_buffer, &p_rrm_oam_nb_cell_info->operation_mode_info_r13))
    {
        return OAM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->nb_mcs_msg3_npusch > 2))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->nb_mcs_msg3_npusch] should be in range [%u <= 2] !", p_rrm_oam_nb_cell_info->nb_mcs_msg3_npusch);
#endif
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_rrm_oam_nb_cell_info->nb_mcs_msg3_npusch, "nb_mcs_msg3_npusch");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_info->nb_mcs_msg3_npusch);

    return OAM_SUCCESS;
}

rrm_length_t
rrm_get_rrm_oam_nb_cell_config_resp_len
(
    rrm_oam_nb_cell_config_resp_t *p_rrm_oam_nb_cell_config_resp
)
{
    rrm_length_t length = 0;

    OAM_NULL_CHECK(p_rrm_oam_nb_cell_config_resp != PNULL);


    /* Get length of IE */
    length += rrm_get_rrm_oam_eutran_global_cell_id_len(&p_rrm_oam_nb_cell_config_resp->global_cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_config_resp->response);
    /* Get length of parameter of basic type */
    length += sizeof(p_rrm_oam_nb_cell_config_resp->fail_cause);

    return length;
}

oam_return_et
oam_compose_rrm_oam_nb_cell_config_resp
(
    U8  **pp_buffer,
    rrm_oam_nb_cell_config_resp_t *p_rrm_oam_nb_cell_config_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrm_oam_nb_cell_config_resp != PNULL);

    /* Compose IE */
    if (OAM_FAILURE == oam_compose_rrm_oam_eutran_global_cell_id(pp_buffer, &p_rrm_oam_nb_cell_config_resp->global_cell_id))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_nb_cell_config_resp->response, "response");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_config_resp->response);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrm_oam_nb_cell_config_resp->fail_cause, "fail_cause");
    *pp_buffer += sizeof(p_rrm_oam_nb_cell_config_resp->fail_cause);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_rrmNb_send_rrm_oam_nb_cell_config_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRM_OAM_NB_CELL_CONFIG_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_rrmNb_send_rrm_oam_nb_cell_config_resp
(
    rrm_oam_nb_cell_config_resp_t  *p_rrm_oam_nb_cell_config_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_oamNb_msg;

    OAM_NULL_CHECK(p_rrm_oam_nb_cell_config_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrm_get_rrm_oam_nb_cell_config_resp_len(p_rrm_oam_nb_cell_config_resp);
    if(msg_length == OAM_FAILURE)
    {
        return OAM_FAILURE;
    }

    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):RRM_OAM_NB_CELL_CONFIG_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRM_API_HEADER_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_oamNb_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_oamNb_msg, RRM_VERSION_ID, src_module_id,
        dst_module_id, RRM_OAM_NB_CELL_CONFIG_RESP, msg_api_length);

    /* Fill interface header */
    p_oamNb_msg = p_oamNb_msg + RRM_API_HEADER_SIZE;
    oam_construct_interface_api_header(p_oamNb_msg, transaction_id, src_module_id,
        dst_module_id, RRM_OAM_NB_CELL_CONFIG_RESP, msg_length,cell_index);

    /* Fill RRM_OAM_NB_CELL_CONFIG_RESP message */
    p_oamNb_msg = p_oamNb_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_compose_rrm_oam_nb_cell_config_resp(&p_oamNb_msg, p_rrm_oam_nb_cell_config_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_oamNb_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }
    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

