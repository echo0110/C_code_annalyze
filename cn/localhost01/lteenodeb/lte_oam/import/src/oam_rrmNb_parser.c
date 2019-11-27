/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: oam_rrmNb_parser.c $
 *
 *******************************************************************************
 *
 * File Description: All RRM OAM Parser function defination for All RRM OAM
 *                   function
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "rrm_pack_unpack_utils.h"
#include "oam_rrmNb_parser.h"
#include "rrm_oamNb_intf.h"
#include "rrm_utils.h"
#include "oam_defines.h"
#include "oam_proto.h"


#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
oam_return_et
oam_parse_rrm_oam_cell_plmn_info
(
    rrm_oam_cell_plmn_info_t *p_rrm_oam_cell_plmn_info,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_eutran_global_cell_id
(
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_cell_access_restriction_params_nb
(
    rrm_oam_cell_access_restriction_params_nb_t *p_rrm_oam_cell_access_restriction_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_cell_info_nb
(
    rrm_oam_cell_info_nb_t *p_rrm_oam_cell_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_rsrp_thresholds_prach_info_list_r13
(
    rrm_oam_rsrp_thresholds_prach_info_list_r13_t *p_rrm_oam_rsrp_thresholds_prach_info_list_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_nprach_parameters_nb_r13
(
    rrm_oam_nprach_parameters_nb_r13_t *p_rrm_oam_nprach_parameters_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_nprach_parameter_list_nb_r13
(
    rrm_oam_nprach_parameter_list_nb_r13_t *p_rrm_oam_nprach_parameter_list_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_prach_nb
(
    rrm_oam_prach_nb_t *p_rrm_oam_prach_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_nprach_parameters_list_nb_v1330
(
    rrm_oam_nprach_parameters_list_nb_v1330_t *p_rrm_oam_nprach_parameters_list_nb_v1330,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_nprach_config_sib_nb_v1330
(
    rrm_oam_nprach_config_sib_nb_v1330_t *p_rrm_oam_nprach_config_sib_nb_v1330,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_npdcch_config_dedicated_nb_r13
(
    rrm_oam_npdcch_config_dedicated_nb_r13_t *p_rrm_oam_npdcch_config_dedicated_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_ack_nack_num_repetitions_msg4_r13
(
    rrm_oam_ack_nack_num_repetitions_msg4_r13_t *p_rrm_oam_ack_nack_num_repetitions_msg4_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_dmrs_config_r13
(
    rrm_oam_dmrs_config_r13_t *p_rrm_oam_dmrs_config_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_ul_reference_signal_npusch_nb
(
    rrm_oam_ul_reference_signal_npusch_nb_t *p_rrm_oam_ul_reference_signal_npusch_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_npusch_config_dedicated_r13
(
    rrm_oam_npusch_config_dedicated_r13_t *p_rrm_oam_npusch_config_dedicated_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_physical_layer_param_pusch_nb
(
    rrm_oam_physical_layer_param_pusch_nb_t *p_rrm_oam_physical_layer_param_pusch_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_uplink_power_control_nb
(
    rrm_oam_uplink_power_control_nb_t *p_rrm_oam_uplink_power_control_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_dl_gap_config_nb
(
    rrm_oam_dl_gap_config_nb_t *p_rrm_oam_dl_gap_config_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_physical_layer_params_nb
(
    rrm_oam_physical_layer_params_nb_t *p_rrm_oam_physical_layer_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_power_ramping_params
(
    rrm_oam_power_ramping_params_t *p_rrm_oam_power_ramping_params,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_rach_info
(
    rrm_oam_rach_info_t *p_rrm_oam_rach_info,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_rach_infolist
(
    rrm_oam_rach_infolist_t *p_rrm_oam_rach_infolist,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_preamble_info_nb
(
    rrm_oam_preamble_info_nb_t *p_rrm_oam_preamble_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_rach_nb
(
    rrm_oam_rach_nb_t *p_rrm_oam_rach_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_drx_config_nb
(
    rrm_oam_drx_config_nb_t *p_rrm_oam_drx_config_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_drx_nb
(
    rrm_oam_drx_nb_t *p_rrm_oam_drx_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_logical_channel_sr_config_r13
(
    rrm_oam_logical_channel_sr_config_r13_t *p_rrm_oam_logical_channel_sr_config_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_ul_sch_config_r13
(
    rrm_oam_ul_sch_config_r13 *p_rrm_oam_ul_sch_config_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_mac_layer_params_nb
(
    rrm_oam_mac_layer_params_nb_t *p_rrm_oam_mac_layer_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_ul_am_rlc_nb_r13
(
    ul_am_rlc_nb_r13_t *p_ul_am_rlc_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_dl_am_rlc_nb_r13
(
    dl_am_rlc_nb_r13_t *p_dl_am_rlc_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_logical_channel_config_nb
(
    rrm_oam_logical_channel_config_nb_t *p_rrm_oam_logical_channel_config_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_srb_info_nb
(
    rrm_oam_srb_info_nb_t *p_rrm_oam_srb_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_srb_nb
(
    rrm_oam_srb_nb_t *p_rrm_oam_srb_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_rlc_layer_params_nb
(
    rrm_oam_rlc_layer_params_nb_t *p_rrm_oam_rlc_layer_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_rrc_timers_nb
(
    rrm_oam_rrc_timers_nb_t *p_rrm_oam_rrc_timers_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_rrc_constants_nb
(
    rrm_oam_rrc_constants_nb_t *p_rrm_oam_rrc_constants_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_rrc_timers_and_constants_nb
(
    rrm_oam_rrc_timers_and_constants_nb_t *p_rrm_oam_rrc_timers_and_constants_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_ns_pmax_value_nb_r13
(
    rrm_oam_ns_pmax_value_nb_r13_t *p_rrm_oam_ns_pmax_value_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_freq_band_info_nb
(
    rrm_oam_freq_band_info_nb_t *p_rrm_oam_freq_band_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_multi_band_info_nb_r13
(
    rrm_oam_multi_band_info_nb_r13_t *p_rrm_oam_multi_band_info_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_multiband_info_list_nb_r13
(
    rrm_oam_multiband_info_list_nb_r13_t *p_rrm_oam_multiband_info_list_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_downlink_bitmap_nb_r13
(
    rrm_oam_downlink_bitmap_nb_r13_t *p_rrm_oam_downlink_bitmap_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_rf_configurations_nb
(
    rrm_oam_rf_configurations_nb_t *p_rrm_oam_rf_configurations_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_rf_params_nb
(
    rrm_oam_rf_params_nb_t *p_rrm_oam_rf_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_s1ap_params
(
    rrm_oam_s1ap_params_t *p_rrm_oam_s1ap_params,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_ran_info_nb
(
    rrm_oam_ran_info_nb_t *p_rrm_oam_ran_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_plmn_access_info_nb
(
    rrm_oam_plmn_access_info_nb_t *p_rrm_oam_plmn_access_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_general_epc_params_nb
(
    rrm_oam_general_epc_params_nb_t *p_rrm_oam_general_epc_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_epc_nb
(
    rrm_oam_epc_nb_t *p_rrm_oam_epc_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_rrmc_mac_config_nb
(
    rrm_oam_rrmc_mac_config_nb_t *p_rrm_oam_rrmc_mac_config_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_cell_selection_info_v13_nb
(
    rrm_oam_cell_selection_info_v13_nb_t *p_rrm_oam_cell_selection_info_v13_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_sib_mapping_info_nb
(
    rrm_oam_sib_mapping_info_nb_t *p_rrm_oam_sib_mapping_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_scheduling_info_nb
(
    rrm_oam_scheduling_info_nb_t *p_rrm_oam_scheduling_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_sib_type_1_info_nb
(
    rrm_oam_sib_type_1_info_nb_t *p_rrm_oam_sib_type_1_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_paging_info_nb
(
    rrm_oam_paging_info_nb_t *p_rrm_oam_paging_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_radio_resource_config_common_sib_nb
(
    rrm_oam_radio_resource_config_common_sib_nb_t *p_rrm_oam_radio_resource_config_common_sib_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_carrier_freq_sib_nb
(
    rrm_oam_carrier_freq_sib_nb_t *p_rrm_oam_carrier_freq_sib_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_freq_info_sib_nb
(
    rrm_oam_freq_info_sib_nb_t *p_rrm_oam_freq_info_sib_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_multi_band_info_nb
(
    rrm_oam_multi_band_info_nb_t *p_rrm_oam_multi_band_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_multiband_info_list_nb
(
    rrm_oam_multiband_info_list_nb_t *p_rrm_oam_multiband_info_list_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_sib_type_2_info_nb
(
    rrm_oam_sib_type_2_info_nb_t *p_rrm_oam_sib_type_2_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_admission_control_info_nb
(
    rrm_oam_admission_control_info_nb_t *p_rrm_oam_admission_control_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_adl_pkt_scheduling_params_nb
(
    rrm_oam_adl_pkt_scheduling_params_nb_t *p_rrm_oam_adl_pkt_scheduling_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_adl_cell_params_nb
(
    rrm_oam_adl_cell_params_nb_t *p_rrm_oam_adl_cell_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_transmission_mode_table_nb
(
    rrm_oam_transmission_mode_table_nb_t *p_rrm_oam_transmission_mode_table_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_transmission_mode_params_nb
(
    rrm_oam_transmission_mode_params_nb_t *p_rrm_oam_transmission_mode_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_eutran_access_point_pos
(
    rrm_oam_eutran_access_point_pos_t *p_rrm_oam_eutran_access_point_pos,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_max_ue_schedule_dl_ul
(
    rrm_oam_max_ue_schedule_dl_ul_t *p_rrm_oam_max_ue_schedule_dl_ul,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_operator_info_nb
(
    rrm_oam_operator_info_nb_t *p_rrm_oam_operator_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_l2_comm_info
(
    rrm_oam_l2_comm_info_t *p_rrm_oam_l2_comm_info,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_lower_layer_comm_info
(
    rrm_oam_lower_layer_comm_info_t *p_rrm_oam_lower_layer_comm_info,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_inband_different_pci_nb_r13
(
    rrm_oam_inband_different_pci_nb_r13_t *p_rrm_oam_inband_different_pci_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_guardband_nb_r13
(
    rrm_oam_guardband_nb_r13_t *p_rrm_oam_guardband_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_operation_mode_info_r13
(
    rrm_oam_operation_mode_info_r13_t *p_rrm_oam_operation_mode_info_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_rrm_oam_nb_cell_info
(
    rrm_oam_nb_cell_info_t *p_rrm_oam_nb_cell_info,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

oam_return_et
oam_parse_rrm_oam_nb_cell_config_req
(
    rrm_oam_nb_cell_config_req_t *p_rrm_oam_nb_cell_config_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_nb_cell_config_req, 0, sizeof(rrm_oam_nb_cell_config_req_t));

    /* This function parses rrm_oam_nb_cell_config_req */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_nb_cell_config_req");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nb_cell_config_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (OAM_FAILURE == oam_parse_rrm_oam_cell_info_nb(
        &p_rrm_oam_nb_cell_config_req->global_cell_info_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_ran_info_nb(
        &p_rrm_oam_nb_cell_config_req->ran_info_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_epc_nb(
        &p_rrm_oam_nb_cell_config_req->epc_info_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_operator_info_nb(
        &p_rrm_oam_nb_cell_config_req->operator_info_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_nb_cell_config_req->immediate_start_needed_nb, p_src + *p_length_read, "immediate_start_needed_nb");
    *p_length_read += sizeof(U32);

    if (OAM_FAILURE == oam_parse_rrm_oam_lower_layer_comm_info(
        &p_rrm_oam_nb_cell_config_req->l2_comm_info_params_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_nb_cell_info(
        &p_rrm_oam_nb_cell_config_req->nb_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_cell_plmn_info
(
    rrm_oam_cell_plmn_info_t *p_rrm_oam_cell_plmn_info,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_cell_plmn_info, 0, sizeof(rrm_oam_cell_plmn_info_t));

    /* This function parses rrm_oam_cell_plmn_info */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_cell_plmn_info");
#endif


    if (*p_length_read + (SInt32)sizeof(p_rrm_oam_cell_plmn_info->mcc) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_cell_plmn_info->mcc); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_UNPACK_UINT8((void*)&p_rrm_oam_cell_plmn_info->mcc[loop], (void*)(p_src + *p_length_read), "mcc[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_cell_plmn_info->num_mnc_digit, p_src + *p_length_read, "num_mnc_digit");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_cell_plmn_info->num_mnc_digit < 2) || (p_rrm_oam_cell_plmn_info->num_mnc_digit > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_cell_plmn_info->num_mnc_digit] should be in range [2 <= %u <= 3] !", p_rrm_oam_cell_plmn_info->num_mnc_digit);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_rrm_oam_cell_plmn_info->num_mnc_digit * sizeof(p_rrm_oam_cell_plmn_info->mnc[0])) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_cell_plmn_info->num_mnc_digit; loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_UNPACK_UINT8((void*)&p_rrm_oam_cell_plmn_info->mnc[loop], (void*)(p_src + *p_length_read), "mnc[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_eutran_global_cell_id
(
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_eutran_global_cell_id, 0, sizeof(rrm_oam_eutran_global_cell_id_t));

    /* This function parses rrm_oam_eutran_global_cell_id */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_eutran_global_cell_id");
#endif


    if (OAM_FAILURE == oam_parse_rrm_oam_cell_plmn_info(
        &p_rrm_oam_eutran_global_cell_id->primary_plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(p_rrm_oam_eutran_global_cell_id->cell_identity) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_eutran_global_cell_id->cell_identity); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_UNPACK_UINT8((void*)&p_rrm_oam_eutran_global_cell_id->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_cell_access_restriction_params_nb
(
    rrm_oam_cell_access_restriction_params_nb_t *p_rrm_oam_cell_access_restriction_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_cell_access_restriction_params_nb, 0, sizeof(rrm_oam_cell_access_restriction_params_nb_t));

    /* This function parses rrm_oam_cell_access_restriction_params_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_cell_access_restriction_params_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_cell_access_restriction_params_nb->cell_barred, p_src + *p_length_read, "cell_barred");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_cell_access_restriction_params_nb->cell_barred > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_cell_access_restriction_params_nb->cell_barred] should be in range [%u <= 1] !", p_rrm_oam_cell_access_restriction_params_nb->cell_barred);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_cell_access_restriction_params_nb->intra_freq_reselection, p_src + *p_length_read, "intra_freq_reselection");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_cell_access_restriction_params_nb->intra_freq_reselection > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_cell_access_restriction_params_nb->intra_freq_reselection] should be in range [%u <= 1] !", p_rrm_oam_cell_access_restriction_params_nb->intra_freq_reselection);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_cell_info_nb
(
    rrm_oam_cell_info_nb_t *p_rrm_oam_cell_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_cell_info_nb, 0, sizeof(rrm_oam_cell_info_nb_t));

    /* This function parses rrm_oam_cell_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_cell_info_nb");
#endif


    if (OAM_FAILURE == oam_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_oam_cell_info_nb->eutran_global_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_cell_access_restriction_params_nb(
        &p_rrm_oam_cell_info_nb->cell_access_restriction_params_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_rsrp_thresholds_prach_info_list_r13
(
    rrm_oam_rsrp_thresholds_prach_info_list_r13_t *p_rrm_oam_rsrp_thresholds_prach_info_list_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_rsrp_thresholds_prach_info_list_r13, 0, sizeof(rrm_oam_rsrp_thresholds_prach_info_list_r13_t));

    /* This function parses rrm_oam_rsrp_thresholds_prach_info_list_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_rsrp_thresholds_prach_info_list_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count < 1) || (p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count > MAX_RSRP_COUNT_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count] should be in range [1 <= %u <= MAX_RSRP_COUNT_NB] !", p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count * sizeof(p_rrm_oam_rsrp_thresholds_prach_info_list_r13->rsrp_range_nb[0])) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_rsrp_thresholds_prach_info_list_r13->count; loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_UNPACK_UINT8((void*)&p_rrm_oam_rsrp_thresholds_prach_info_list_r13->rsrp_range_nb[loop], (void*)(p_src + *p_length_read), "rsrp_range_nb[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_nprach_parameters_nb_r13
(
    rrm_oam_nprach_parameters_nb_r13_t *p_rrm_oam_nprach_parameters_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_nprach_parameters_nb_r13, 0, sizeof(rrm_oam_nprach_parameters_nb_r13_t));

    /* This function parses rrm_oam_nprach_parameters_nb_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_nprach_parameters_nb_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_parameters_nb_r13->nprach_periodicity_r13, p_src + *p_length_read, "nprach_periodicity_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->nprach_periodicity_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->nprach_periodicity_r13] should be in range [%u <= 7] !", p_rrm_oam_nprach_parameters_nb_r13->nprach_periodicity_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_parameters_nb_r13->nprach_start_time_r13, p_src + *p_length_read, "nprach_start_time_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->nprach_start_time_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->nprach_start_time_r13] should be in range [%u <= 7] !", p_rrm_oam_nprach_parameters_nb_r13->nprach_start_time_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_offset_r13, p_src + *p_length_read, "nprach_subcarrier_offset_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_offset_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_offset_r13] should be in range [%u <= 6] !", p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_offset_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_parameters_nb_r13->nprach_num_subcarriers_r13, p_src + *p_length_read, "nprach_num_subcarriers_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->nprach_num_subcarriers_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->nprach_num_subcarriers_r13] should be in range [%u <= 3] !", p_rrm_oam_nprach_parameters_nb_r13->nprach_num_subcarriers_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_msg3_range_start_r13, p_src + *p_length_read, "nprach_subcarrier_msg3_range_start_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_msg3_range_start_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_msg3_range_start_r13] should be in range [%u <= 3] !", p_rrm_oam_nprach_parameters_nb_r13->nprach_subcarrier_msg3_range_start_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_parameters_nb_r13->max_num_preamble_attempt_ce_r13, p_src + *p_length_read, "max_num_preamble_attempt_ce_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->max_num_preamble_attempt_ce_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->max_num_preamble_attempt_ce_r13] should be in range [%u <= 6] !", p_rrm_oam_nprach_parameters_nb_r13->max_num_preamble_attempt_ce_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_parameters_nb_r13->num_repetitions_per_preamble_attempt_r13, p_src + *p_length_read, "num_repetitions_per_preamble_attempt_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->num_repetitions_per_preamble_attempt_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->num_repetitions_per_preamble_attempt_r13] should be in range [%u <= 7] !", p_rrm_oam_nprach_parameters_nb_r13->num_repetitions_per_preamble_attempt_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_parameters_nb_r13->npdcch_num_repetitions_ra_r13, p_src + *p_length_read, "npdcch_num_repetitions_ra_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->npdcch_num_repetitions_ra_r13 > 11))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->npdcch_num_repetitions_ra_r13] should be in range [%u <= 11] !", p_rrm_oam_nprach_parameters_nb_r13->npdcch_num_repetitions_ra_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_parameters_nb_r13->npdcch_start_sf_css_ra_r13, p_src + *p_length_read, "npdcch_start_sf_css_ra_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->npdcch_start_sf_css_ra_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->npdcch_start_sf_css_ra_r13] should be in range [%u <= 7] !", p_rrm_oam_nprach_parameters_nb_r13->npdcch_start_sf_css_ra_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_parameters_nb_r13->npdcch_offset_css_ra_r13, p_src + *p_length_read, "npdcch_offset_css_ra_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_nb_r13->npdcch_offset_css_ra_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_nb_r13->npdcch_offset_css_ra_r13] should be in range [%u <= 3] !", p_rrm_oam_nprach_parameters_nb_r13->npdcch_offset_css_ra_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_nprach_parameter_list_nb_r13
(
    rrm_oam_nprach_parameter_list_nb_r13_t *p_rrm_oam_nprach_parameter_list_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_nprach_parameter_list_nb_r13, 0, sizeof(rrm_oam_nprach_parameter_list_nb_r13_t));

    /* This function parses rrm_oam_nprach_parameter_list_nb_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_nprach_parameter_list_nb_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_parameter_list_nb_r13->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_nprach_parameter_list_nb_r13->count < 1) || (p_rrm_oam_nprach_parameter_list_nb_r13->count > MAX_NPRACH_RESOURCES_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameter_list_nb_r13->count] should be in range [1 <= %u <= MAX_NPRACH_RESOURCES_NB_R13] !", p_rrm_oam_nprach_parameter_list_nb_r13->count);
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_nprach_parameter_list_nb_r13->count; loop++)
        {
            if (OAM_FAILURE == oam_parse_rrm_oam_nprach_parameters_nb_r13(
                &p_rrm_oam_nprach_parameter_list_nb_r13->nprach_parameters_nb_r13[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_prach_nb
(
    rrm_oam_prach_nb_t *p_rrm_oam_prach_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_prach_nb, 0, sizeof(rrm_oam_prach_nb_t));

    /* This function parses rrm_oam_prach_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_prach_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_prach_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_prach_nb->nprach_cp_length_r13, p_src + *p_length_read, "nprach_cp_length_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_prach_nb->nprach_cp_length_r13 > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_prach_nb->nprach_cp_length_r13] should be in range [%u <= 1] !", p_rrm_oam_prach_nb->nprach_cp_length_r13);
#endif
        return OAM_FAILURE;
    }

    if (p_rrm_oam_prach_nb->bitmask & RRM_OAM_RSRP_THRESHOLD_PRACH_INFO_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_rsrp_thresholds_prach_info_list_r13(
        &p_rrm_oam_prach_nb->rsrp_thresholds_prach_info_list_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (OAM_FAILURE == oam_parse_rrm_oam_nprach_parameter_list_nb_r13(
        &p_rrm_oam_prach_nb->nprach_parameters_list_nb_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_nprach_parameters_list_nb_v1330
(
    rrm_oam_nprach_parameters_list_nb_v1330_t *p_rrm_oam_nprach_parameters_list_nb_v1330,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_nprach_parameters_list_nb_v1330, 0, sizeof(rrm_oam_nprach_parameters_list_nb_v1330_t));

    /* This function parses rrm_oam_nprach_parameters_list_nb_v1330 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_nprach_parameters_list_nb_v1330");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_parameters_list_nb_v1330->nprach_num_cbra_start_subcarriers_r13, p_src + *p_length_read, "nprach_num_cbra_start_subcarriers_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nprach_parameters_list_nb_v1330->nprach_num_cbra_start_subcarriers_r13 > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_parameters_list_nb_v1330->nprach_num_cbra_start_subcarriers_r13] should be in range [%u <= 15] !", p_rrm_oam_nprach_parameters_list_nb_v1330->nprach_num_cbra_start_subcarriers_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_nprach_config_sib_nb_v1330
(
    rrm_oam_nprach_config_sib_nb_v1330_t *p_rrm_oam_nprach_config_sib_nb_v1330,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_nprach_config_sib_nb_v1330, 0, sizeof(rrm_oam_nprach_config_sib_nb_v1330_t));

    /* This function parses rrm_oam_nprach_config_sib_nb_v1330 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_nprach_config_sib_nb_v1330");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nprach_config_sib_nb_v1330->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_nprach_config_sib_nb_v1330->count < 1) || (p_rrm_oam_nprach_config_sib_nb_v1330->count > MAX_NPRACH_RESOURCES_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nprach_config_sib_nb_v1330->count] should be in range [1 <= %u <= MAX_NPRACH_RESOURCES_NB_R13] !", p_rrm_oam_nprach_config_sib_nb_v1330->count);
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_nprach_config_sib_nb_v1330->count; loop++)
        {
            if (OAM_FAILURE == oam_parse_rrm_oam_nprach_parameters_list_nb_v1330(
                &p_rrm_oam_nprach_config_sib_nb_v1330->nprach_parameters_list_v1330[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_npdcch_config_dedicated_nb_r13
(
    rrm_oam_npdcch_config_dedicated_nb_r13_t *p_rrm_oam_npdcch_config_dedicated_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_npdcch_config_dedicated_nb_r13, 0, sizeof(rrm_oam_npdcch_config_dedicated_nb_r13_t));

    /* This function parses rrm_oam_npdcch_config_dedicated_nb_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_npdcch_config_dedicated_nb_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_num_repetitions_r13, p_src + *p_length_read, "npdcch_num_repetitions_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_num_repetitions_r13 > 11))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_num_repetitions_r13] should be in range [%u <= 11] !", p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_num_repetitions_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_start_sf_uss_r13, p_src + *p_length_read, "npdcch_start_sf_uss_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_start_sf_uss_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_start_sf_uss_r13] should be in range [%u <= 7] !", p_rrm_oam_npdcch_config_dedicated_nb_r13->npdcch_start_sf_uss_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_ack_nack_num_repetitions_msg4_r13
(
    rrm_oam_ack_nack_num_repetitions_msg4_r13_t *p_rrm_oam_ack_nack_num_repetitions_msg4_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_ack_nack_num_repetitions_msg4_r13, 0, sizeof(rrm_oam_ack_nack_num_repetitions_msg4_r13_t));

    /* This function parses rrm_oam_ack_nack_num_repetitions_msg4_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_ack_nack_num_repetitions_msg4_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count > MAX_NPRACH_RESOURCES_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count] should be in range [%u <= MAX_NPRACH_RESOURCES_NB_R13] !", p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count * sizeof(p_rrm_oam_ack_nack_num_repetitions_msg4_r13->ack_nack_num_repetitions_nb_r13[0])) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_ack_nack_num_repetitions_msg4_r13->count; loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_UNPACK_UINT8((void*)&p_rrm_oam_ack_nack_num_repetitions_msg4_r13->ack_nack_num_repetitions_nb_r13[loop], (void*)(p_src + *p_length_read), "ack_nack_num_repetitions_nb_r13[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_dmrs_config_r13
(
    rrm_oam_dmrs_config_r13_t *p_rrm_oam_dmrs_config_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_dmrs_config_r13, 0, sizeof(rrm_oam_dmrs_config_r13_t));

    /* This function parses rrm_oam_dmrs_config_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_dmrs_config_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_dmrs_config_r13->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_dmrs_config_r13->bitmask & RRM_OAM_THREE_TONE_BASE_SEQ_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_dmrs_config_r13->three_tone_base_sequence_r13, p_src + *p_length_read, "three_tone_base_sequence_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dmrs_config_r13->three_tone_base_sequence_r13 > 12))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dmrs_config_r13->three_tone_base_sequence_r13] should be in range [%u <= 12] !", p_rrm_oam_dmrs_config_r13->three_tone_base_sequence_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_dmrs_config_r13->three_tone_cyclic_shift_r13, p_src + *p_length_read, "three_tone_cyclic_shift_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dmrs_config_r13->three_tone_cyclic_shift_r13 > 2))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dmrs_config_r13->three_tone_cyclic_shift_r13] should be in range [%u <= 2] !", p_rrm_oam_dmrs_config_r13->three_tone_cyclic_shift_r13);
#endif
        return OAM_FAILURE;
    }

    if (p_rrm_oam_dmrs_config_r13->bitmask & RRM_OAM_SIX_TON_SEQ_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_dmrs_config_r13->six_tone_base_sequence_r13, p_src + *p_length_read, "six_tone_base_sequence_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dmrs_config_r13->six_tone_base_sequence_r13 > 14))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dmrs_config_r13->six_tone_base_sequence_r13] should be in range [%u <= 14] !", p_rrm_oam_dmrs_config_r13->six_tone_base_sequence_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_dmrs_config_r13->six_tone_cyclic_shift_r13, p_src + *p_length_read, "six_tone_cyclic_shift_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dmrs_config_r13->six_tone_cyclic_shift_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dmrs_config_r13->six_tone_cyclic_shift_r13] should be in range [%u <= 3] !", p_rrm_oam_dmrs_config_r13->six_tone_cyclic_shift_r13);
#endif
        return OAM_FAILURE;
    }

    if (p_rrm_oam_dmrs_config_r13->bitmask & RRM_OAM_TWELVE_TONE_SEQ_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_dmrs_config_r13->twelve_tone_base_sequence_r13, p_src + *p_length_read, "twelve_tone_base_sequence_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dmrs_config_r13->twelve_tone_base_sequence_r13 > 30))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dmrs_config_r13->twelve_tone_base_sequence_r13] should be in range [%u <= 30] !", p_rrm_oam_dmrs_config_r13->twelve_tone_base_sequence_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_ul_reference_signal_npusch_nb
(
    rrm_oam_ul_reference_signal_npusch_nb_t *p_rrm_oam_ul_reference_signal_npusch_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_ul_reference_signal_npusch_nb, 0, sizeof(rrm_oam_ul_reference_signal_npusch_nb_t));

    /* This function parses rrm_oam_ul_reference_signal_npusch_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_ul_reference_signal_npusch_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_ul_reference_signal_npusch_nb->group_assignment_npusch_r13, p_src + *p_length_read, "group_assignment_npusch_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_ul_reference_signal_npusch_nb->group_assignment_npusch_r13 > 29))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ul_reference_signal_npusch_nb->group_assignment_npusch_r13] should be in range [%u <= 29] !", p_rrm_oam_ul_reference_signal_npusch_nb->group_assignment_npusch_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_ul_reference_signal_npusch_nb->group_hopping_enabled_r13, p_src + *p_length_read, "group_hopping_enabled_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_ul_reference_signal_npusch_nb->group_hopping_enabled_r13 > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ul_reference_signal_npusch_nb->group_hopping_enabled_r13] should be in range [%u <= 1] !", p_rrm_oam_ul_reference_signal_npusch_nb->group_hopping_enabled_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_npusch_config_dedicated_r13
(
    rrm_oam_npusch_config_dedicated_r13_t *p_rrm_oam_npusch_config_dedicated_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_npusch_config_dedicated_r13, 0, sizeof(rrm_oam_npusch_config_dedicated_r13_t));

    /* This function parses rrm_oam_npusch_config_dedicated_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_npusch_config_dedicated_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_npusch_config_dedicated_r13->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_npusch_config_dedicated_r13->bitmask & RRM_OAM_ACK_NACK_NUM_REPETITION_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_npusch_config_dedicated_r13->ack_nack_num_repetitions_r13, p_src + *p_length_read, "ack_nack_num_repetitions_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_npusch_config_dedicated_r13->ack_nack_num_repetitions_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_npusch_config_dedicated_r13->ack_nack_num_repetitions_r13] should be in range [%u <= 7] !", p_rrm_oam_npusch_config_dedicated_r13->ack_nack_num_repetitions_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (p_rrm_oam_npusch_config_dedicated_r13->bitmask & RRM_OAM_NPUSCH_ALL_SYMBOLS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_npusch_config_dedicated_r13->npusch_all_symbols_r13, p_src + *p_length_read, "npusch_all_symbols_r13");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_npusch_config_dedicated_r13->npusch_all_symbols_r13 > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_npusch_config_dedicated_r13->npusch_all_symbols_r13] should be in range [%u <= 1] !", p_rrm_oam_npusch_config_dedicated_r13->npusch_all_symbols_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (p_rrm_oam_npusch_config_dedicated_r13->bitmask & RRM_OAM_GROUP_HOPPING_DISABLED_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_npusch_config_dedicated_r13->group_hopping_disabled_r13, p_src + *p_length_read, "group_hopping_disabled_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_npusch_config_dedicated_r13->group_hopping_disabled_r13 > 0))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_npusch_config_dedicated_r13->group_hopping_disabled_r13] should be in range [%u <= 0] !", p_rrm_oam_npusch_config_dedicated_r13->group_hopping_disabled_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_physical_layer_param_pusch_nb
(
    rrm_oam_physical_layer_param_pusch_nb_t *p_rrm_oam_physical_layer_param_pusch_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_physical_layer_param_pusch_nb, 0, sizeof(rrm_oam_physical_layer_param_pusch_nb_t));

    /* This function parses rrm_oam_physical_layer_param_pusch_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_physical_layer_param_pusch_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_physical_layer_param_pusch_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (OAM_FAILURE == oam_parse_rrm_oam_ack_nack_num_repetitions_msg4_r13(
        &p_rrm_oam_physical_layer_param_pusch_nb->ack_nack_num_repetitions_msg4_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_oam_physical_layer_param_pusch_nb->bitmask & RRM_OAM_SRS_SUBFRAME_CONFIG_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_physical_layer_param_pusch_nb->srs_subframe_config_r13, p_src + *p_length_read, "srs_subframe_config_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_physical_layer_param_pusch_nb->srs_subframe_config_r13 > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_physical_layer_param_pusch_nb->srs_subframe_config_r13] should be in range [%u <= 15] !", p_rrm_oam_physical_layer_param_pusch_nb->srs_subframe_config_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (p_rrm_oam_physical_layer_param_pusch_nb->bitmask & RRM_OAM_DMRS_CONFIG_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_dmrs_config_r13(
        &p_rrm_oam_physical_layer_param_pusch_nb->dmrs_config_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (OAM_FAILURE == oam_parse_rrm_oam_ul_reference_signal_npusch_nb(
        &p_rrm_oam_physical_layer_param_pusch_nb->ul_reference_signal_npusch_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_npusch_config_dedicated_r13(
        &p_rrm_oam_physical_layer_param_pusch_nb->npusch_config_dedicated_nb_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_uplink_power_control_nb
(
    rrm_oam_uplink_power_control_nb_t *p_rrm_oam_uplink_power_control_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_uplink_power_control_nb, 0, sizeof(rrm_oam_uplink_power_control_nb_t));

    /* This function parses rrm_oam_uplink_power_control_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_uplink_power_control_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(S8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_rrm_oam_uplink_power_control_nb->p0_nominal_npusch_r13, p_src + *p_length_read, "p0_nominal_npusch_r13");
    *p_length_read += sizeof(S8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_uplink_power_control_nb->p0_nominal_npusch_r13 > 24))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_uplink_power_control_nb->p0_nominal_npusch_r13] should be in range [%d <= 24] !", p_rrm_oam_uplink_power_control_nb->p0_nominal_npusch_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_uplink_power_control_nb->alpha_r13, p_src + *p_length_read, "alpha_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_uplink_power_control_nb->alpha_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_uplink_power_control_nb->alpha_r13] should be in range [%u <= 7] !", p_rrm_oam_uplink_power_control_nb->alpha_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(S8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_rrm_oam_uplink_power_control_nb->delta_preamble_msg3_r13, p_src + *p_length_read, "delta_preamble_msg3_r13");
    *p_length_read += sizeof(S8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_uplink_power_control_nb->delta_preamble_msg3_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_uplink_power_control_nb->delta_preamble_msg3_r13] should be in range [%d <= 6] !", p_rrm_oam_uplink_power_control_nb->delta_preamble_msg3_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(S8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_rrm_oam_uplink_power_control_nb->p0_ue_npusch_r13, p_src + *p_length_read, "p0_ue_npusch_r13");
    *p_length_read += sizeof(S8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_uplink_power_control_nb->p0_ue_npusch_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_uplink_power_control_nb->p0_ue_npusch_r13] should be in range [%d <= 7] !", p_rrm_oam_uplink_power_control_nb->p0_ue_npusch_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_dl_gap_config_nb
(
    rrm_oam_dl_gap_config_nb_t *p_rrm_oam_dl_gap_config_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_dl_gap_config_nb, 0, sizeof(rrm_oam_dl_gap_config_nb_t));

    /* This function parses rrm_oam_dl_gap_config_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_dl_gap_config_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_dl_gap_config_nb->dl_gap_threshold_r13, p_src + *p_length_read, "dl_gap_threshold_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dl_gap_config_nb->dl_gap_threshold_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dl_gap_config_nb->dl_gap_threshold_r13] should be in range [%u <= 3] !", p_rrm_oam_dl_gap_config_nb->dl_gap_threshold_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_dl_gap_config_nb->dl_gap_periodicity_r13, p_src + *p_length_read, "dl_gap_periodicity_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dl_gap_config_nb->dl_gap_periodicity_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dl_gap_config_nb->dl_gap_periodicity_r13] should be in range [%u <= 3] !", p_rrm_oam_dl_gap_config_nb->dl_gap_periodicity_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_dl_gap_config_nb->dl_gap_duration_coeff_r13, p_src + *p_length_read, "dl_gap_duration_coeff_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_dl_gap_config_nb->dl_gap_duration_coeff_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_dl_gap_config_nb->dl_gap_duration_coeff_r13] should be in range [%u <= 3] !", p_rrm_oam_dl_gap_config_nb->dl_gap_duration_coeff_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_physical_layer_params_nb
(
    rrm_oam_physical_layer_params_nb_t *p_rrm_oam_physical_layer_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_physical_layer_params_nb, 0, sizeof(rrm_oam_physical_layer_params_nb_t));

    /* This function parses rrm_oam_physical_layer_params_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_physical_layer_params_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_physical_layer_params_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (OAM_FAILURE == oam_parse_rrm_oam_prach_nb(
        &p_rrm_oam_physical_layer_params_nb->physical_layer_param_prach_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_oam_physical_layer_params_nb->bitmask & RRM_OAM_NPRACH_CONFIG_V1330_NB_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_nprach_config_sib_nb_v1330(
        &p_rrm_oam_physical_layer_params_nb->nprach_config_v1330,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (OAM_FAILURE == oam_parse_rrm_oam_npdcch_config_dedicated_nb_r13(
        &p_rrm_oam_physical_layer_params_nb->npdcch_config_dedicated_nb_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_physical_layer_param_pusch_nb(
        &p_rrm_oam_physical_layer_params_nb->physical_layer_param_pusch_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_uplink_power_control_nb(
        &p_rrm_oam_physical_layer_params_nb->physical_layer_param_ul_power_control_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_oam_physical_layer_params_nb->bitmask & RRM_OAM_DL_GAP_CONFIG_NB_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_dl_gap_config_nb(
        &p_rrm_oam_physical_layer_params_nb->dl_gap_config_nb_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_physical_layer_params_nb->nb_tx_antenna_port_count, p_src + *p_length_read, "nb_tx_antenna_port_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_physical_layer_params_nb->nb_tx_antenna_port_count < 1) || (p_rrm_oam_physical_layer_params_nb->nb_tx_antenna_port_count > 2))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_physical_layer_params_nb->nb_tx_antenna_port_count] should be in range [1 <= %u <= 2] !", p_rrm_oam_physical_layer_params_nb->nb_tx_antenna_port_count);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_physical_layer_params_nb->nb_rx_antenna_port_count, p_src + *p_length_read, "nb_rx_antenna_port_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_physical_layer_params_nb->nb_rx_antenna_port_count < 1) || (p_rrm_oam_physical_layer_params_nb->nb_rx_antenna_port_count > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_physical_layer_params_nb->nb_rx_antenna_port_count] should be in range [1 <= %u <= 1] !", p_rrm_oam_physical_layer_params_nb->nb_rx_antenna_port_count);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_power_ramping_params
(
    rrm_oam_power_ramping_params_t *p_rrm_oam_power_ramping_params,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_power_ramping_params, 0, sizeof(rrm_oam_power_ramping_params_t));

    /* This function parses rrm_oam_power_ramping_params */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_power_ramping_params");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_power_ramping_params->power_ramping_step_nb, p_src + *p_length_read, "power_ramping_step_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_power_ramping_params->power_ramping_step_nb > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_power_ramping_params->power_ramping_step_nb] should be in range [%u <= 3] !", p_rrm_oam_power_ramping_params->power_ramping_step_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_power_ramping_params->preamble_initial_received_target_power_nb, p_src + *p_length_read, "preamble_initial_received_target_power_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_power_ramping_params->preamble_initial_received_target_power_nb > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_power_ramping_params->preamble_initial_received_target_power_nb] should be in range [%u <= 15] !", p_rrm_oam_power_ramping_params->preamble_initial_received_target_power_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_rach_info
(
    rrm_oam_rach_info_t *p_rrm_oam_rach_info,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_rach_info, 0, sizeof(rrm_oam_rach_info_t));

    /* This function parses rrm_oam_rach_info */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_rach_info");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rach_info->ra_response_window_size_r13, p_src + *p_length_read, "ra_response_window_size_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rach_info->ra_response_window_size_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rach_info->ra_response_window_size_r13] should be in range [%u <= 7] !", p_rrm_oam_rach_info->ra_response_window_size_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rach_info->mac_contention_resolution_timer_r13, p_src + *p_length_read, "mac_contention_resolution_timer_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rach_info->mac_contention_resolution_timer_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rach_info->mac_contention_resolution_timer_r13] should be in range [%u <= 7] !", p_rrm_oam_rach_info->mac_contention_resolution_timer_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_rach_infolist
(
    rrm_oam_rach_infolist_t *p_rrm_oam_rach_infolist,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_rach_infolist, 0, sizeof(rrm_oam_rach_infolist_t));

    /* This function parses rrm_oam_rach_infolist */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_rach_infolist");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rach_infolist->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rach_infolist->count < 1) || (p_rrm_oam_rach_infolist->count > MAX_NPRACH_RESOURCES_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rach_infolist->count] should be in range [1 <= %u <= MAX_NPRACH_RESOURCES_NB_R13] !", p_rrm_oam_rach_infolist->count);
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_rach_infolist->count; loop++)
        {
            if (OAM_FAILURE == oam_parse_rrm_oam_rach_info(
                &p_rrm_oam_rach_infolist->rach_Info_nb_r13[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_preamble_info_nb
(
    rrm_oam_preamble_info_nb_t *p_rrm_oam_preamble_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_preamble_info_nb, 0, sizeof(rrm_oam_preamble_info_nb_t));

    /* This function parses rrm_oam_preamble_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_preamble_info_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_preamble_info_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_preamble_info_nb->preamble_trans_max_ce_r13, p_src + *p_length_read, "preamble_trans_max_ce_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_preamble_info_nb->preamble_trans_max_ce_r13 > 10))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_preamble_info_nb->preamble_trans_max_ce_r13] should be in range [%u <= 10] !", p_rrm_oam_preamble_info_nb->preamble_trans_max_ce_r13);
#endif
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_parse_rrm_oam_power_ramping_params(
        &p_rrm_oam_preamble_info_nb->power_ramping_parameters_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_rach_infolist(
        &p_rrm_oam_preamble_info_nb->rach_info_list_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_oam_preamble_info_nb->bitmask & RRM_OAM_CONN_EST_FAIL_OFFSET_R13_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_preamble_info_nb->conn_est_fail_offset_r13, p_src + *p_length_read, "conn_est_fail_offset_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_preamble_info_nb->conn_est_fail_offset_r13 > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_preamble_info_nb->conn_est_fail_offset_r13] should be in range [%u <= 15] !", p_rrm_oam_preamble_info_nb->conn_est_fail_offset_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_preamble_info_nb->max_harq_msg_3tx_nb, p_src + *p_length_read, "max_harq_msg_3tx_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_preamble_info_nb->max_harq_msg_3tx_nb < 1) || (p_rrm_oam_preamble_info_nb->max_harq_msg_3tx_nb > 8))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_preamble_info_nb->max_harq_msg_3tx_nb] should be in range [1 <= %u <= 8] !", p_rrm_oam_preamble_info_nb->max_harq_msg_3tx_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_rach_nb
(
    rrm_oam_rach_nb_t *p_rrm_oam_rach_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_rach_nb, 0, sizeof(rrm_oam_rach_nb_t));

    /* This function parses rrm_oam_rach_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_rach_nb");
#endif


    if (OAM_FAILURE == oam_parse_rrm_oam_preamble_info_nb(
        &p_rrm_oam_rach_nb->preamble_info_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_drx_config_nb
(
    rrm_oam_drx_config_nb_t *p_rrm_oam_drx_config_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_drx_config_nb, 0, sizeof(rrm_oam_drx_config_nb_t));

    /* This function parses rrm_oam_drx_config_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_drx_config_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_drx_config_nb->on_duration_timer_r13, p_src + *p_length_read, "on_duration_timer_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_drx_config_nb->on_duration_timer_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_drx_config_nb->on_duration_timer_r13] should be in range [%u <= 6] !", p_rrm_oam_drx_config_nb->on_duration_timer_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_drx_config_nb->drx_inactivity_timer_r13, p_src + *p_length_read, "drx_inactivity_timer_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_drx_config_nb->drx_inactivity_timer_r13 > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_drx_config_nb->drx_inactivity_timer_r13] should be in range [%u <= 7] !", p_rrm_oam_drx_config_nb->drx_inactivity_timer_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_drx_config_nb->drx_retransmission_timer_r13, p_src + *p_length_read, "drx_retransmission_timer_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_drx_config_nb->drx_retransmission_timer_r13 > 8))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_drx_config_nb->drx_retransmission_timer_r13] should be in range [%u <= 8] !", p_rrm_oam_drx_config_nb->drx_retransmission_timer_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_drx_config_nb->drx_cycle_r13, p_src + *p_length_read, "drx_cycle_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_drx_config_nb->drx_cycle_r13 > 11))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_drx_config_nb->drx_cycle_r13] should be in range [%u <= 11] !", p_rrm_oam_drx_config_nb->drx_cycle_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_drx_config_nb->drx_start_offset_r13, p_src + *p_length_read, "drx_start_offset_r13");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_drx_config_nb->drx_ul_retransmission_timer_r13, p_src + *p_length_read, "drx_ul_retransmission_timer_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_drx_config_nb->drx_ul_retransmission_timer_r13 > 16))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_drx_config_nb->drx_ul_retransmission_timer_r13] should be in range [%u <= 16] !", p_rrm_oam_drx_config_nb->drx_ul_retransmission_timer_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_drx_nb
(
    rrm_oam_drx_nb_t *p_rrm_oam_drx_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_drx_nb, 0, sizeof(rrm_oam_drx_nb_t));

    /* This function parses rrm_oam_drx_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_drx_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_drx_nb->drx_enabled_nb, p_src + *p_length_read, "drx_enabled_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_drx_nb->drx_enabled_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_drx_nb->drx_enabled_nb] should be in range [%u <= 1] !", p_rrm_oam_drx_nb->drx_enabled_nb);
#endif
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_parse_rrm_oam_drx_config_nb(
        &p_rrm_oam_drx_nb->drx_config_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_logical_channel_sr_config_r13
(
    rrm_oam_logical_channel_sr_config_r13_t *p_rrm_oam_logical_channel_sr_config_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_logical_channel_sr_config_r13, 0, sizeof(rrm_oam_logical_channel_sr_config_r13_t));

    /* This function parses rrm_oam_logical_channel_sr_config_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_logical_channel_sr_config_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_logical_channel_sr_config_r13->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_logical_channel_sr_config_r13->bitmask & RRM_OAM_LOGICAL_CHANNEL_SR_PROHIBIT_TIMER_SETUP_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_logical_channel_sr_config_r13->logical_channel_sr_prohibit_timer_r13, p_src + *p_length_read, "logical_channel_sr_prohibit_timer_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_logical_channel_sr_config_r13->logical_channel_sr_prohibit_timer_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_logical_channel_sr_config_r13->logical_channel_sr_prohibit_timer_r13] should be in range [%u <= 6] !", p_rrm_oam_logical_channel_sr_config_r13->logical_channel_sr_prohibit_timer_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_ul_sch_config_r13
(
    rrm_oam_ul_sch_config_r13 *p_rrm_oam_ul_sch_config_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_ul_sch_config_r13, 0, sizeof(rrm_oam_ul_sch_config_r13));

    /* This function parses rrm_oam_ul_sch_config_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_ul_sch_config_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_ul_sch_config_r13->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_ul_sch_config_r13->bitmask & RRM_OAM_PERIODIC_BSR_TIMER_R13_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_ul_sch_config_r13->periodic_bsr_timer_r13, p_src + *p_length_read, "periodic_bsr_timer_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_ul_sch_config_r13->periodic_bsr_timer_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ul_sch_config_r13->periodic_bsr_timer_r13] should be in range [%u <= 6] !", p_rrm_oam_ul_sch_config_r13->periodic_bsr_timer_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_ul_sch_config_r13->retx_bsr_timer_r13, p_src + *p_length_read, "retx_bsr_timer_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_ul_sch_config_r13->retx_bsr_timer_r13 > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ul_sch_config_r13->retx_bsr_timer_r13] should be in range [%u <= 6] !", p_rrm_oam_ul_sch_config_r13->retx_bsr_timer_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_mac_layer_params_nb
(
    rrm_oam_mac_layer_params_nb_t *p_rrm_oam_mac_layer_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_mac_layer_params_nb, 0, sizeof(rrm_oam_mac_layer_params_nb_t));

    /* This function parses rrm_oam_mac_layer_params_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_mac_layer_params_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_mac_layer_params_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (OAM_FAILURE == oam_parse_rrm_oam_rach_nb(
        &p_rrm_oam_mac_layer_params_nb->mac_layer_param_rach_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_oam_mac_layer_params_nb->bitmask & RRM_OAM_DRX_CONFIG_NB_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_drx_nb(
        &p_rrm_oam_mac_layer_params_nb->mac_layer_param_drx_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (OAM_FAILURE == oam_parse_rrm_oam_logical_channel_sr_config_r13(
        &p_rrm_oam_mac_layer_params_nb->logical_channel_sr_config_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_mac_layer_params_nb->ul_sync_loss_timer_nb, p_src + *p_length_read, "ul_sync_loss_timer_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_mac_layer_params_nb->ul_sync_loss_timer_nb > 12))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_mac_layer_params_nb->ul_sync_loss_timer_nb] should be in range [%u <= 12] !", p_rrm_oam_mac_layer_params_nb->ul_sync_loss_timer_nb);
#endif
        return OAM_FAILURE;
    }

    if (p_rrm_oam_mac_layer_params_nb->bitmask & RRM_OAM_UL_SCH_CONFIG_R13_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_ul_sch_config_r13(
        &p_rrm_oam_mac_layer_params_nb->ul_sch_config_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_ul_am_rlc_nb_r13
(
    ul_am_rlc_nb_r13_t *p_ul_am_rlc_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_ul_am_rlc_nb_r13, 0, sizeof(ul_am_rlc_nb_r13_t));

    /* This function parses ul_am_rlc_nb_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing ul_am_rlc_nb_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_ul_am_rlc_nb_r13->t_poll_retransmit_r13, p_src + *p_length_read, "t_poll_retransmit_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_ul_am_rlc_nb_r13->t_poll_retransmit_r13 > 14))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_ul_am_rlc_nb_r13->t_poll_retransmit_r13] should be in range [%u <= 14] !", p_ul_am_rlc_nb_r13->t_poll_retransmit_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_ul_am_rlc_nb_r13->max_retx_threshold_nb, p_src + *p_length_read, "max_retx_threshold_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_ul_am_rlc_nb_r13->max_retx_threshold_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_ul_am_rlc_nb_r13->max_retx_threshold_nb] should be in range [%u <= 7] !", p_ul_am_rlc_nb_r13->max_retx_threshold_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_dl_am_rlc_nb_r13
(
    dl_am_rlc_nb_r13_t *p_dl_am_rlc_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_dl_am_rlc_nb_r13, 0, sizeof(dl_am_rlc_nb_r13_t));

    /* This function parses dl_am_rlc_nb_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing dl_am_rlc_nb_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_dl_am_rlc_nb_r13->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_dl_am_rlc_nb_r13->bitmask & RRM_OAM_ENABLE_STATUS_REPORT_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_dl_am_rlc_nb_r13->enable_status_report_sn_gap_r13, p_src + *p_length_read, "enable_status_report_sn_gap_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_dl_am_rlc_nb_r13->enable_status_report_sn_gap_r13 > 0))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_dl_am_rlc_nb_r13->enable_status_report_sn_gap_r13] should be in range [%u <= 0] !", p_dl_am_rlc_nb_r13->enable_status_report_sn_gap_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_logical_channel_config_nb
(
    rrm_oam_logical_channel_config_nb_t *p_rrm_oam_logical_channel_config_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_logical_channel_config_nb, 0, sizeof(rrm_oam_logical_channel_config_nb_t));

    /* This function parses rrm_oam_logical_channel_config_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_logical_channel_config_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_logical_channel_config_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_logical_channel_config_nb->bitmask & RRM_OAM_NB_LOGICAL_CHANNEL_CONFIG_PRIORITY_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_logical_channel_config_nb->priority_r13, p_src + *p_length_read, "priority_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_logical_channel_config_nb->priority_r13 < 1) || (p_rrm_oam_logical_channel_config_nb->priority_r13 > 16))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_logical_channel_config_nb->priority_r13] should be in range [1 <= %u <= 16] !", p_rrm_oam_logical_channel_config_nb->priority_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (p_rrm_oam_logical_channel_config_nb->bitmask & RRM_OAM_NB_LOGICAL_CHANNEL_SR_PROHIBIT_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_logical_channel_config_nb->logical_channel_sr_prohibit_r13, p_src + *p_length_read, "logical_channel_sr_prohibit_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_logical_channel_config_nb->logical_channel_sr_prohibit_r13 > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_logical_channel_config_nb->logical_channel_sr_prohibit_r13] should be in range [%u <= 1] !", p_rrm_oam_logical_channel_config_nb->logical_channel_sr_prohibit_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_srb_info_nb
(
    rrm_oam_srb_info_nb_t *p_rrm_oam_srb_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_srb_info_nb, 0, sizeof(rrm_oam_srb_info_nb_t));

    /* This function parses rrm_oam_srb_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_srb_info_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_srb_info_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (OAM_FAILURE == oam_parse_ul_am_rlc_nb_r13(
        &p_rrm_oam_srb_info_nb->ul_am_rlc_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_dl_am_rlc_nb_r13(
        &p_rrm_oam_srb_info_nb->dl_am_rlc_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_logical_channel_config_nb(
        &p_rrm_oam_srb_info_nb->logical_channel_config_nb_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_srb_nb
(
    rrm_oam_srb_nb_t *p_rrm_oam_srb_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_srb_nb, 0, sizeof(rrm_oam_srb_nb_t));

    /* This function parses rrm_oam_srb_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_srb_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_srb_nb->default_configuration_nb, p_src + *p_length_read, "default_configuration_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_srb_nb->default_configuration_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_srb_nb->default_configuration_nb] should be in range [%u <= 1] !", p_rrm_oam_srb_nb->default_configuration_nb);
#endif
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_parse_rrm_oam_srb_info_nb(
        &p_rrm_oam_srb_nb->srb_params_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_rlc_layer_params_nb
(
    rrm_oam_rlc_layer_params_nb_t *p_rrm_oam_rlc_layer_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_rlc_layer_params_nb, 0, sizeof(rrm_oam_rlc_layer_params_nb_t));

    /* This function parses rrm_oam_rlc_layer_params_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_rlc_layer_params_nb");
#endif


    if (OAM_FAILURE == oam_parse_rrm_oam_srb_nb(
        &p_rrm_oam_rlc_layer_params_nb->rlc_layer_param_srb_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_rrc_timers_nb
(
    rrm_oam_rrc_timers_nb_t *p_rrm_oam_rrc_timers_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_rrc_timers_nb, 0, sizeof(rrm_oam_rrc_timers_nb_t));

    /* This function parses rrm_oam_rrc_timers_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_rrc_timers_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rrc_timers_nb->t300_nb, p_src + *p_length_read, "t300_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rrc_timers_nb->t300_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rrc_timers_nb->t300_nb] should be in range [%u <= 7] !", p_rrm_oam_rrc_timers_nb->t300_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rrc_timers_nb->t301_nb, p_src + *p_length_read, "t301_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rrc_timers_nb->t301_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rrc_timers_nb->t301_nb] should be in range [%u <= 7] !", p_rrm_oam_rrc_timers_nb->t301_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rrc_timers_nb->t310_nb, p_src + *p_length_read, "t310_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rrc_timers_nb->t310_nb > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rrc_timers_nb->t310_nb] should be in range [%u <= 6] !", p_rrm_oam_rrc_timers_nb->t310_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rrc_timers_nb->t311_nb, p_src + *p_length_read, "t311_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rrc_timers_nb->t311_nb > 6))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rrc_timers_nb->t311_nb] should be in range [%u <= 6] !", p_rrm_oam_rrc_timers_nb->t311_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_rrc_constants_nb
(
    rrm_oam_rrc_constants_nb_t *p_rrm_oam_rrc_constants_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_rrc_constants_nb, 0, sizeof(rrm_oam_rrc_constants_nb_t));

    /* This function parses rrm_oam_rrc_constants_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_rrc_constants_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rrc_constants_nb->n310_nb, p_src + *p_length_read, "n310_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rrc_constants_nb->n310_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rrc_constants_nb->n310_nb] should be in range [%u <= 7] !", p_rrm_oam_rrc_constants_nb->n310_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rrc_constants_nb->n311_nb, p_src + *p_length_read, "n311_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rrc_constants_nb->n311_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rrc_constants_nb->n311_nb] should be in range [%u <= 7] !", p_rrm_oam_rrc_constants_nb->n311_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_rrc_timers_and_constants_nb
(
    rrm_oam_rrc_timers_and_constants_nb_t *p_rrm_oam_rrc_timers_and_constants_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_rrc_timers_and_constants_nb, 0, sizeof(rrm_oam_rrc_timers_and_constants_nb_t));

    /* This function parses rrm_oam_rrc_timers_and_constants_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_rrc_timers_and_constants_nb");
#endif


    if (OAM_FAILURE == oam_parse_rrm_oam_rrc_timers_nb(
        &p_rrm_oam_rrc_timers_and_constants_nb->rrc_timers_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_rrc_constants_nb(
        &p_rrm_oam_rrc_timers_and_constants_nb->rrc_constants_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_ns_pmax_value_nb_r13
(
    rrm_oam_ns_pmax_value_nb_r13_t *p_rrm_oam_ns_pmax_value_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_ns_pmax_value_nb_r13, 0, sizeof(rrm_oam_ns_pmax_value_nb_r13_t));

    /* This function parses rrm_oam_ns_pmax_value_nb_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_ns_pmax_value_nb_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_ns_pmax_value_nb_r13->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_ns_pmax_value_nb_r13->bitmask & RRM_OAM_NB_PMAX_R13_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(S8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_rrm_oam_ns_pmax_value_nb_r13->pmax_r13, p_src + *p_length_read, "pmax_r13");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_ns_pmax_value_nb_r13->pmax_r13 < -30) || (p_rrm_oam_ns_pmax_value_nb_r13->pmax_r13 > 33))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ns_pmax_value_nb_r13->pmax_r13] should be in range [-30 <= %d <= 33] !", p_rrm_oam_ns_pmax_value_nb_r13->pmax_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_ns_pmax_value_nb_r13->additional_spectrum_emission_r13, p_src + *p_length_read, "additional_spectrum_emission_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_ns_pmax_value_nb_r13->additional_spectrum_emission_r13 < 1) || (p_rrm_oam_ns_pmax_value_nb_r13->additional_spectrum_emission_r13 > 32))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_ns_pmax_value_nb_r13->additional_spectrum_emission_r13] should be in range [1 <= %u <= 32] !", p_rrm_oam_ns_pmax_value_nb_r13->additional_spectrum_emission_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_freq_band_info_nb
(
    rrm_oam_freq_band_info_nb_t *p_rrm_oam_freq_band_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_freq_band_info_nb, 0, sizeof(rrm_oam_freq_band_info_nb_t));

    /* This function parses rrm_oam_freq_band_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_freq_band_info_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_freq_band_info_nb->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_freq_band_info_nb->count < 1) || (p_rrm_oam_freq_band_info_nb->count > MAX_NS_PMAX_NB_R13))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_freq_band_info_nb->count] should be in range [1 <= %u <= MAX_NS_PMAX_NB_R13] !", p_rrm_oam_freq_band_info_nb->count);
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_freq_band_info_nb->count; loop++)
        {
            if (OAM_FAILURE == oam_parse_rrm_oam_ns_pmax_value_nb_r13(
                &p_rrm_oam_freq_band_info_nb->ns_pmax_value_nb_r13[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_multi_band_info_nb_r13
(
    rrm_oam_multi_band_info_nb_r13_t *p_rrm_oam_multi_band_info_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_multi_band_info_nb_r13, 0, sizeof(rrm_oam_multi_band_info_nb_r13_t));

    /* This function parses rrm_oam_multi_band_info_nb_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_multi_band_info_nb_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_multi_band_info_nb_r13->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_multi_band_info_nb_r13->bitmask & RRM_OAM_NB_FREQ_BAND_INDICATOR_R13_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_multi_band_info_nb_r13->freq_band_indicator_r13, p_src + *p_length_read, "freq_band_indicator_r13");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_multi_band_info_nb_r13->freq_band_indicator_r13 < 1) || (p_rrm_oam_multi_band_info_nb_r13->freq_band_indicator_r13 > MAX_FBI_2_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_multi_band_info_nb_r13->freq_band_indicator_r13] should be in range [1 <= %u <= MAX_FBI_2_NB] !", p_rrm_oam_multi_band_info_nb_r13->freq_band_indicator_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (p_rrm_oam_multi_band_info_nb_r13->bitmask & RRM_OAM_NB_FREQ_BAND_INFO_NB_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_freq_band_info_nb(
        &p_rrm_oam_multi_band_info_nb_r13->freq_band_info_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_multiband_info_list_nb_r13
(
    rrm_oam_multiband_info_list_nb_r13_t *p_rrm_oam_multiband_info_list_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_multiband_info_list_nb_r13, 0, sizeof(rrm_oam_multiband_info_list_nb_r13_t));

    /* This function parses rrm_oam_multiband_info_list_nb_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_multiband_info_list_nb_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_multiband_info_list_nb_r13->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_multiband_info_list_nb_r13->count < 1) || (p_rrm_oam_multiband_info_list_nb_r13->count > MAX_MULTI_BAND_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_multiband_info_list_nb_r13->count] should be in range [1 <= %u <= MAX_MULTI_BAND_NB] !", p_rrm_oam_multiband_info_list_nb_r13->count);
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_multiband_info_list_nb_r13->count; loop++)
        {
            if (OAM_FAILURE == oam_parse_rrm_oam_multi_band_info_nb_r13(
                &p_rrm_oam_multiband_info_list_nb_r13->multi_band_info_nb_r13[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_downlink_bitmap_nb_r13
(
    rrm_oam_downlink_bitmap_nb_r13_t *p_rrm_oam_downlink_bitmap_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_downlink_bitmap_nb_r13, 0, sizeof(rrm_oam_downlink_bitmap_nb_r13_t));

    /* This function parses rrm_oam_downlink_bitmap_nb_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_downlink_bitmap_nb_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_downlink_bitmap_nb_r13->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_downlink_bitmap_nb_r13->bitmask & RRM_OAM_NB_SUBFRAME_PATTERN_10_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_rrm_oam_downlink_bitmap_nb_r13->subframe_pattern10_r13) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_downlink_bitmap_nb_r13->subframe_pattern10_r13); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_UNPACK_UINT8((void*)&p_rrm_oam_downlink_bitmap_nb_r13->subframe_pattern10_r13[loop], (void*)(p_src + *p_length_read), "subframe_pattern10_r13[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_rrm_oam_downlink_bitmap_nb_r13->bitmask & RRM_OAM_NB_SUBFRAME_PATTERN_40_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_rrm_oam_downlink_bitmap_nb_r13->subframe_pattern40_r13) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_downlink_bitmap_nb_r13->subframe_pattern40_r13); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_UNPACK_UINT8((void*)&p_rrm_oam_downlink_bitmap_nb_r13->subframe_pattern40_r13[loop], (void*)(p_src + *p_length_read), "subframe_pattern40_r13[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_rf_configurations_nb
(
    rrm_oam_rf_configurations_nb_t *p_rrm_oam_rf_configurations_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_rf_configurations_nb, 0, sizeof(rrm_oam_rf_configurations_nb_t));

    /* This function parses rrm_oam_rf_configurations_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_rf_configurations_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rf_configurations_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_rf_configurations_nb->freq_band_indicator_nb, p_src + *p_length_read, "freq_band_indicator_nb");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rf_configurations_nb->freq_band_indicator_nb < 1) || (p_rrm_oam_rf_configurations_nb->freq_band_indicator_nb > MAX_FBI_2_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->freq_band_indicator_nb] should be in range [1 <= %u <= MAX_FBI_2_NB] !", p_rrm_oam_rf_configurations_nb->freq_band_indicator_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_rf_configurations_nb->dl_earfcn_nb, p_src + *p_length_read, "dl_earfcn_nb");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->dl_earfcn_nb > MAX_EARFCN))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->dl_earfcn_nb] should be in range [%u <= MAX_EARFCN] !", p_rrm_oam_rf_configurations_nb->dl_earfcn_nb);
#endif
        return OAM_FAILURE;
    }

    if (p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_RF_CONFIGURATION_UL_EARFCN_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_rf_configurations_nb->ul_earfcn_nb, p_src + *p_length_read, "ul_earfcn_nb");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->ul_earfcn_nb > 65535))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->ul_earfcn_nb] should be in range [%u <= 65535] !", p_rrm_oam_rf_configurations_nb->ul_earfcn_nb);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read + (SInt32)sizeof(S8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_rrm_oam_rf_configurations_nb->nrs_power_r13, p_src + *p_length_read, "nrs_power_r13");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rf_configurations_nb->nrs_power_r13 < -60) || (p_rrm_oam_rf_configurations_nb->nrs_power_r13 > 50))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->nrs_power_r13] should be in range [-60 <= %d <= 50] !", p_rrm_oam_rf_configurations_nb->nrs_power_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_rf_configurations_nb->phy_cell_id_nb, p_src + *p_length_read, "phy_cell_id_nb");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->phy_cell_id_nb > 503))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->phy_cell_id_nb] should be in range [%u <= 503] !", p_rrm_oam_rf_configurations_nb->phy_cell_id_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(S8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_rrm_oam_rf_configurations_nb->psch_power_offset_nb, p_src + *p_length_read, "psch_power_offset_nb");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rf_configurations_nb->psch_power_offset_nb < -60) || (p_rrm_oam_rf_configurations_nb->psch_power_offset_nb > 40))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->psch_power_offset_nb] should be in range [-60 <= %d <= 40] !", p_rrm_oam_rf_configurations_nb->psch_power_offset_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(S8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_rrm_oam_rf_configurations_nb->ssch_power_offset_nb, p_src + *p_length_read, "ssch_power_offset_nb");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rf_configurations_nb->ssch_power_offset_nb < -60) || (p_rrm_oam_rf_configurations_nb->ssch_power_offset_nb > 40))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->ssch_power_offset_nb] should be in range [-60 <= %d <= 40] !", p_rrm_oam_rf_configurations_nb->ssch_power_offset_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(S8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_rrm_oam_rf_configurations_nb->pbch_power_offset_nb, p_src + *p_length_read, "pbch_power_offset_nb");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rf_configurations_nb->pbch_power_offset_nb < -60) || (p_rrm_oam_rf_configurations_nb->pbch_power_offset_nb > 40))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->pbch_power_offset_nb] should be in range [-60 <= %d <= 40] !", p_rrm_oam_rf_configurations_nb->pbch_power_offset_nb);
#endif
        return OAM_FAILURE;
    }

    if (p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_MAX_RS_EPRE_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(S16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT16(&p_rrm_oam_rf_configurations_nb->max_rs_epre_nb, p_src + *p_length_read, "max_rs_epre_nb");
    *p_length_read += sizeof(S16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_rf_configurations_nb->max_rs_epre_nb < -256) || (p_rrm_oam_rf_configurations_nb->max_rs_epre_nb > 256))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->max_rs_epre_nb] should be in range [-256 <= %d <= 256] !", p_rrm_oam_rf_configurations_nb->max_rs_epre_nb);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_rf_configurations_nb->pdcch_power_offset_nb, p_src + *p_length_read, "pdcch_power_offset_nb");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->pdcch_power_offset_nb > 10000))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->pdcch_power_offset_nb] should be in range [%u <= 10000] !", p_rrm_oam_rf_configurations_nb->pdcch_power_offset_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_rf_configurations_nb->pbch_transmission_power, p_src + *p_length_read, "pbch_transmission_power");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->pbch_transmission_power > 10000))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->pbch_transmission_power] should be in range [%u <= 10000] !", p_rrm_oam_rf_configurations_nb->pbch_transmission_power);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_rf_configurations_nb->pch_transmission_power, p_src + *p_length_read, "pch_transmission_power");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->pch_transmission_power > 10000))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->pch_transmission_power] should be in range [%u <= 10000] !", p_rrm_oam_rf_configurations_nb->pch_transmission_power);
#endif
        return OAM_FAILURE;
    }

    if (p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_FREQ_BAND_INFO_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_freq_band_info_nb(
        &p_rrm_oam_rf_configurations_nb->freq_band_info_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_MULTI_BAND_INFO_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_multiband_info_list_nb_r13(
        &p_rrm_oam_rf_configurations_nb->multi_band_info_list_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NB_DL_BITMAP_INFO_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_downlink_bitmap_nb_r13(
        &p_rrm_oam_rf_configurations_nb->downlink_bitmap_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_EUTRA_CONTROL_REGION_SIZE_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rf_configurations_nb->eutra_control_region_size_r13, p_src + *p_length_read, "eutra_control_region_size_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->eutra_control_region_size_r13 > 2))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->eutra_control_region_size_r13] should be in range [%u <= 2] !", p_rrm_oam_rf_configurations_nb->eutra_control_region_size_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (p_rrm_oam_rf_configurations_nb->bitmask & RRM_OAM_NRS_CRS_POWER_OFFSET_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rf_configurations_nb->nrs_crs_power_offset_r13, p_src + *p_length_read, "nrs_crs_power_offset_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->nrs_crs_power_offset_r13 > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->nrs_crs_power_offset_r13] should be in range [%u <= 15] !", p_rrm_oam_rf_configurations_nb->nrs_crs_power_offset_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rf_configurations_nb->scheduling_info_sib1_nb, p_src + *p_length_read, "scheduling_info_sib1_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->scheduling_info_sib1_nb > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->scheduling_info_sib1_nb] should be in range [%u <= 15] !", p_rrm_oam_rf_configurations_nb->scheduling_info_sib1_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_rf_configurations_nb->ab_enabled_nb, p_src + *p_length_read, "ab_enabled_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_rf_configurations_nb->ab_enabled_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_rf_configurations_nb->ab_enabled_nb] should be in range [%u <= 1] !", p_rrm_oam_rf_configurations_nb->ab_enabled_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_rf_params_nb
(
    rrm_oam_rf_params_nb_t *p_rrm_oam_rf_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_rf_params_nb, 0, sizeof(rrm_oam_rf_params_nb_t));

    /* This function parses rrm_oam_rf_params_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_rf_params_nb");
#endif


    if (OAM_FAILURE == oam_parse_rrm_oam_rf_configurations_nb(
        &p_rrm_oam_rf_params_nb->rf_configurations_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_s1ap_params
(
    rrm_oam_s1ap_params_t *p_rrm_oam_s1ap_params,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_s1ap_params, 0, sizeof(rrm_oam_s1ap_params_t));

    /* This function parses rrm_oam_s1ap_params */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_s1ap_params");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_s1ap_params->t_reloc_prep, p_src + *p_length_read, "t_reloc_prep");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_s1ap_params->t_reloc_overall, p_src + *p_length_read, "t_reloc_overall");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_s1ap_params->t_reloc_overall > 60))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_s1ap_params->t_reloc_overall] should be in range [%u <= 60] !", p_rrm_oam_s1ap_params->t_reloc_overall);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_ran_info_nb
(
    rrm_oam_ran_info_nb_t *p_rrm_oam_ran_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_ran_info_nb, 0, sizeof(rrm_oam_ran_info_nb_t));

    /* This function parses rrm_oam_ran_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_ran_info_nb");
#endif


    if (OAM_FAILURE == oam_parse_rrm_oam_physical_layer_params_nb(
        &p_rrm_oam_ran_info_nb->physical_layer_params_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_mac_layer_params_nb(
        &p_rrm_oam_ran_info_nb->mac_layer_params_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_rlc_layer_params_nb(
        &p_rrm_oam_ran_info_nb->rlc_layer_params_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_rrc_timers_and_constants_nb(
        &p_rrm_oam_ran_info_nb->rrc_timers_and_constants_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_rf_params_nb(
        &p_rrm_oam_ran_info_nb->rf_params_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_s1ap_params(
        &p_rrm_oam_ran_info_nb->s1ap_params_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_plmn_access_info_nb
(
    rrm_oam_plmn_access_info_nb_t *p_rrm_oam_plmn_access_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_plmn_access_info_nb, 0, sizeof(rrm_oam_plmn_access_info_nb_t));

    /* This function parses rrm_oam_plmn_access_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_plmn_access_info_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_plmn_access_info_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (OAM_FAILURE == oam_parse_rrm_oam_cell_plmn_info(
        &p_rrm_oam_plmn_access_info_nb->plmn_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_plmn_access_info_nb->reserve_operator_use, p_src + *p_length_read, "reserve_operator_use");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_plmn_access_info_nb->reserve_operator_use > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_plmn_access_info_nb->reserve_operator_use] should be in range [%u <= 1] !", p_rrm_oam_plmn_access_info_nb->reserve_operator_use);
#endif
        return OAM_FAILURE;
    }

    if (p_rrm_oam_plmn_access_info_nb->bitmask & RRM_OAM_NB_ATTACH_WITHOUT_PDN_CONNECTIVITY_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_plmn_access_info_nb->attach_without_pdn_connectivity_nb, p_src + *p_length_read, "attach_without_pdn_connectivity_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_plmn_access_info_nb->attach_without_pdn_connectivity_nb > 0))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_plmn_access_info_nb->attach_without_pdn_connectivity_nb] should be in range [%u <= 0] !", p_rrm_oam_plmn_access_info_nb->attach_without_pdn_connectivity_nb);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_general_epc_params_nb
(
    rrm_oam_general_epc_params_nb_t *p_rrm_oam_general_epc_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_general_epc_params_nb, 0, sizeof(rrm_oam_general_epc_params_nb_t));

    /* This function parses rrm_oam_general_epc_params_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_general_epc_params_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb, p_src + *p_length_read, "num_valid_plmn_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb < 1) || (p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb > RRM_OAM_MAX_NUM_PLMNS))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb] should be in range [1 <= %u <= RRM_OAM_MAX_NUM_PLMNS] !", p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb);
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_general_epc_params_nb->num_valid_plmn_nb; loop++)
        {
            if (OAM_FAILURE == oam_parse_rrm_oam_plmn_access_info_nb(
                &p_rrm_oam_general_epc_params_nb->plmn_list_nb[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (SInt32)sizeof(p_rrm_oam_general_epc_params_nb->tac) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_general_epc_params_nb->tac); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_UNPACK_UINT8((void*)&p_rrm_oam_general_epc_params_nb->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_epc_nb
(
    rrm_oam_epc_nb_t *p_rrm_oam_epc_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_epc_nb, 0, sizeof(rrm_oam_epc_nb_t));

    /* This function parses rrm_oam_epc_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_epc_nb");
#endif


    if (OAM_FAILURE == oam_parse_rrm_oam_general_epc_params_nb(
        &p_rrm_oam_epc_nb->general_epc_params_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_epc_nb->max_harq_retrans_nb, p_src + *p_length_read, "max_harq_retrans_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_epc_nb->max_harq_retrans_nb < 1) || (p_rrm_oam_epc_nb->max_harq_retrans_nb > 8))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_epc_nb->max_harq_retrans_nb] should be in range [1 <= %u <= 8] !", p_rrm_oam_epc_nb->max_harq_retrans_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_rrmc_mac_config_nb
(
    rrm_oam_rrmc_mac_config_nb_t *p_rrm_oam_rrmc_mac_config_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_rrmc_mac_config_nb, 0, sizeof(rrm_oam_rrmc_mac_config_nb_t));

    /* This function parses rrm_oam_rrmc_mac_config_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_rrmc_mac_config_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_rrmc_mac_config_nb->start_rarnti_range_nb, p_src + *p_length_read, "start_rarnti_range_nb");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_rrmc_mac_config_nb->end_rarnti_range_nb, p_src + *p_length_read, "end_rarnti_range_nb");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_rrmc_mac_config_nb->ue_inactivity_timer_config_nb, p_src + *p_length_read, "ue_inactivity_timer_config_nb");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_cell_selection_info_v13_nb
(
    rrm_oam_cell_selection_info_v13_nb_t *p_rrm_oam_cell_selection_info_v13_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_cell_selection_info_v13_nb, 0, sizeof(rrm_oam_cell_selection_info_v13_nb_t));

    /* This function parses rrm_oam_cell_selection_info_v13_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_cell_selection_info_v13_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(S8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_rrm_oam_cell_selection_info_v13_nb->q_qual_min_r13, p_src + *p_length_read, "q_qual_min_r13");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_cell_selection_info_v13_nb->q_qual_min_r13 < -34) || (p_rrm_oam_cell_selection_info_v13_nb->q_qual_min_r13 > -3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_cell_selection_info_v13_nb->q_qual_min_r13] should be in range [-34 <= %d <= -3] !", p_rrm_oam_cell_selection_info_v13_nb->q_qual_min_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(S8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_rrm_oam_cell_selection_info_v13_nb->q_rx_lev_min_r13, p_src + *p_length_read, "q_rx_lev_min_r13");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_cell_selection_info_v13_nb->q_rx_lev_min_r13 < -70) || (p_rrm_oam_cell_selection_info_v13_nb->q_rx_lev_min_r13 > -22))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_cell_selection_info_v13_nb->q_rx_lev_min_r13] should be in range [-70 <= %d <= -22] !", p_rrm_oam_cell_selection_info_v13_nb->q_rx_lev_min_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_sib_mapping_info_nb
(
    rrm_oam_sib_mapping_info_nb_t *p_rrm_oam_sib_mapping_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_sib_mapping_info_nb, 0, sizeof(rrm_oam_sib_mapping_info_nb_t));

    /* This function parses rrm_oam_sib_mapping_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_sib_mapping_info_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_sib_mapping_info_nb->sib_type_nb, p_src + *p_length_read, "sib_type_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_sib_mapping_info_nb->sib_type_nb > 4))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_mapping_info_nb->sib_type_nb] should be in range [%u <= 4] !", p_rrm_oam_sib_mapping_info_nb->sib_type_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_scheduling_info_nb
(
    rrm_oam_scheduling_info_nb_t *p_rrm_oam_scheduling_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_scheduling_info_nb, 0, sizeof(rrm_oam_scheduling_info_nb_t));

    /* This function parses rrm_oam_scheduling_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_scheduling_info_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_scheduling_info_nb->si_periodicity_nb, p_src + *p_length_read, "si_periodicity_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_scheduling_info_nb->si_periodicity_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_scheduling_info_nb->si_periodicity_nb] should be in range [%u <= 7] !", p_rrm_oam_scheduling_info_nb->si_periodicity_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_scheduling_info_nb->si_repetition_pattern_n, p_src + *p_length_read, "si_repetition_pattern_n");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_scheduling_info_nb->si_repetition_pattern_n > 4))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_scheduling_info_nb->si_repetition_pattern_n] should be in range [%u <= 4] !", p_rrm_oam_scheduling_info_nb->si_repetition_pattern_n);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_scheduling_info_nb->si_mapping_count, p_src + *p_length_read, "si_mapping_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_scheduling_info_nb->si_mapping_count > RRM_OAM_MAX_SIB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_scheduling_info_nb->si_mapping_count] should be in range [%u <= RRM_OAM_MAX_SIB] !", p_rrm_oam_scheduling_info_nb->si_mapping_count);
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_scheduling_info_nb->si_mapping_count; loop++)
        {
            if (OAM_FAILURE == oam_parse_rrm_oam_sib_mapping_info_nb(
                &p_rrm_oam_scheduling_info_nb->sib_mapping_info_nb[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_sib_type_1_info_nb
(
    rrm_oam_sib_type_1_info_nb_t *p_rrm_oam_sib_type_1_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_sib_type_1_info_nb, 0, sizeof(rrm_oam_sib_type_1_info_nb_t));

    /* This function parses rrm_oam_sib_type_1_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_sib_type_1_info_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_sib_type_1_info_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (OAM_FAILURE == oam_parse_rrm_oam_cell_selection_info_v13_nb(
        &p_rrm_oam_sib_type_1_info_nb->cell_selection_info_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_sib_type_1_info_nb->si_window_length_nb, p_src + *p_length_read, "si_window_length_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_sib_type_1_info_nb->si_window_length_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_type_1_info_nb->si_window_length_nb] should be in range [%u <= 7] !", p_rrm_oam_sib_type_1_info_nb->si_window_length_nb);
#endif
        return OAM_FAILURE;
    }

    if (p_rrm_oam_sib_type_1_info_nb->bitmask & RRM_OAM_NB_SI_RADIO_FRAME_OFFSET_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_sib_type_1_info_nb->si_radio_frame_offset_nb, p_src + *p_length_read, "si_radio_frame_offset_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_sib_type_1_info_nb->si_radio_frame_offset_nb < 1) || (p_rrm_oam_sib_type_1_info_nb->si_radio_frame_offset_nb > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_type_1_info_nb->si_radio_frame_offset_nb] should be in range [1 <= %u <= 15] !", p_rrm_oam_sib_type_1_info_nb->si_radio_frame_offset_nb);
#endif
        return OAM_FAILURE;
    }
    }

    if (p_rrm_oam_sib_type_1_info_nb->bitmask & RRM_OAM_NB_PMAX_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(S8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_rrm_oam_sib_type_1_info_nb->pmax_r13, p_src + *p_length_read, "pmax_r13");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_sib_type_1_info_nb->pmax_r13 < -30) || (p_rrm_oam_sib_type_1_info_nb->pmax_r13 > 33))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_type_1_info_nb->pmax_r13] should be in range [-30 <= %d <= 33] !", p_rrm_oam_sib_type_1_info_nb->pmax_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_sib_type_1_info_nb->scheduling_info_count, p_src + *p_length_read, "scheduling_info_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_sib_type_1_info_nb->scheduling_info_count < 1) || (p_rrm_oam_sib_type_1_info_nb->scheduling_info_count > RRM_OAM_MAX_NUM_SI))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_type_1_info_nb->scheduling_info_count] should be in range [1 <= %u <= RRM_OAM_MAX_NUM_SI] !", p_rrm_oam_sib_type_1_info_nb->scheduling_info_count);
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_sib_type_1_info_nb->scheduling_info_count; loop++)
        {
            if (OAM_FAILURE == oam_parse_rrm_oam_scheduling_info_nb(
                &p_rrm_oam_sib_type_1_info_nb->scheduling_info_nb[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_paging_info_nb
(
    rrm_oam_paging_info_nb_t *p_rrm_oam_paging_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_paging_info_nb, 0, sizeof(rrm_oam_paging_info_nb_t));

    /* This function parses rrm_oam_paging_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_paging_info_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_paging_info_nb->default_paging_cycle_nb, p_src + *p_length_read, "default_paging_cycle_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_paging_info_nb->default_paging_cycle_nb > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_paging_info_nb->default_paging_cycle_nb] should be in range [%u <= 3] !", p_rrm_oam_paging_info_nb->default_paging_cycle_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_paging_info_nb->nB_r13, p_src + *p_length_read, "nB_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_paging_info_nb->nB_r13 > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_paging_info_nb->nB_r13] should be in range [%u <= 15] !", p_rrm_oam_paging_info_nb->nB_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_paging_info_nb->npdcch_num_repetition_paging_nb, p_src + *p_length_read, "npdcch_num_repetition_paging_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_paging_info_nb->npdcch_num_repetition_paging_nb > 11))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_paging_info_nb->npdcch_num_repetition_paging_nb] should be in range [%u <= 11] !", p_rrm_oam_paging_info_nb->npdcch_num_repetition_paging_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_radio_resource_config_common_sib_nb
(
    rrm_oam_radio_resource_config_common_sib_nb_t *p_rrm_oam_radio_resource_config_common_sib_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_radio_resource_config_common_sib_nb, 0, sizeof(rrm_oam_radio_resource_config_common_sib_nb_t));

    /* This function parses rrm_oam_radio_resource_config_common_sib_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_radio_resource_config_common_sib_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_radio_resource_config_common_sib_nb->modification_period_coeff_nb, p_src + *p_length_read, "modification_period_coeff_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_radio_resource_config_common_sib_nb->modification_period_coeff_nb > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_radio_resource_config_common_sib_nb->modification_period_coeff_nb] should be in range [%u <= 3] !", p_rrm_oam_radio_resource_config_common_sib_nb->modification_period_coeff_nb);
#endif
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_parse_rrm_oam_paging_info_nb(
        &p_rrm_oam_radio_resource_config_common_sib_nb->paging_info_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_carrier_freq_sib_nb
(
    rrm_oam_carrier_freq_sib_nb_t *p_rrm_oam_carrier_freq_sib_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_carrier_freq_sib_nb, 0, sizeof(rrm_oam_carrier_freq_sib_nb_t));

    /* This function parses rrm_oam_carrier_freq_sib_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_carrier_freq_sib_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_carrier_freq_sib_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_carrier_freq_sib_nb->carrier_freq_nb, p_src + *p_length_read, "carrier_freq_nb");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_carrier_freq_sib_nb->carrier_freq_nb > MAX_EARFCN_2_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_carrier_freq_sib_nb->carrier_freq_nb] should be in range [%u <= MAX_EARFCN_2_NB] !", p_rrm_oam_carrier_freq_sib_nb->carrier_freq_nb);
#endif
        return OAM_FAILURE;
    }

    if (p_rrm_oam_carrier_freq_sib_nb->bitmask & RRM_OAM_NB_CARRIER_FREQ_OFFSET_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_carrier_freq_sib_nb->carrier_freq_offset_nb, p_src + *p_length_read, "carrier_freq_offset_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_carrier_freq_sib_nb->carrier_freq_offset_nb > 20))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_carrier_freq_sib_nb->carrier_freq_offset_nb] should be in range [%u <= 20] !", p_rrm_oam_carrier_freq_sib_nb->carrier_freq_offset_nb);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_freq_info_sib_nb
(
    rrm_oam_freq_info_sib_nb_t *p_rrm_oam_freq_info_sib_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_freq_info_sib_nb, 0, sizeof(rrm_oam_freq_info_sib_nb_t));

    /* This function parses rrm_oam_freq_info_sib_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_freq_info_sib_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_freq_info_sib_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_freq_info_sib_nb->bitmask & RRM_OAM_NB_UL_CARRIER_FREQUENCY_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_carrier_freq_sib_nb(
        &p_rrm_oam_freq_info_sib_nb->ul_carrier_freq_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_freq_info_sib_nb->additional_spectrum_emission_nb, p_src + *p_length_read, "additional_spectrum_emission_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_freq_info_sib_nb->additional_spectrum_emission_nb < 1) || (p_rrm_oam_freq_info_sib_nb->additional_spectrum_emission_nb > 32))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_freq_info_sib_nb->additional_spectrum_emission_nb] should be in range [1 <= %u <= 32] !", p_rrm_oam_freq_info_sib_nb->additional_spectrum_emission_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_multi_band_info_nb
(
    rrm_oam_multi_band_info_nb_t *p_rrm_oam_multi_band_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_multi_band_info_nb, 0, sizeof(rrm_oam_multi_band_info_nb_t));

    /* This function parses rrm_oam_multi_band_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_multi_band_info_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_multi_band_info_nb->additional_spectrum_emmision_nb, p_src + *p_length_read, "additional_spectrum_emmision_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_multi_band_info_nb->additional_spectrum_emmision_nb < 1) || (p_rrm_oam_multi_band_info_nb->additional_spectrum_emmision_nb > 32))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_multi_band_info_nb->additional_spectrum_emmision_nb] should be in range [1 <= %u <= 32] !", p_rrm_oam_multi_band_info_nb->additional_spectrum_emmision_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_multiband_info_list_nb
(
    rrm_oam_multiband_info_list_nb_t *p_rrm_oam_multiband_info_list_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_multiband_info_list_nb, 0, sizeof(rrm_oam_multiband_info_list_nb_t));

    /* This function parses rrm_oam_multiband_info_list_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_multiband_info_list_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_multiband_info_list_nb->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_multiband_info_list_nb->count < 1) || (p_rrm_oam_multiband_info_list_nb->count > MAX_MULTI_BAND_NB))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_multiband_info_list_nb->count] should be in range [1 <= %u <= MAX_MULTI_BAND_NB] !", p_rrm_oam_multiband_info_list_nb->count);
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_multiband_info_list_nb->count; loop++)
        {
            if (OAM_FAILURE == oam_parse_rrm_oam_multi_band_info_nb(
                &p_rrm_oam_multiband_info_list_nb->multi_band_info_nb[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_sib_type_2_info_nb
(
    rrm_oam_sib_type_2_info_nb_t *p_rrm_oam_sib_type_2_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_sib_type_2_info_nb, 0, sizeof(rrm_oam_sib_type_2_info_nb_t));

    /* This function parses rrm_oam_sib_type_2_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_sib_type_2_info_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_sib_type_2_info_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (OAM_FAILURE == oam_parse_rrm_oam_radio_resource_config_common_sib_nb(
        &p_rrm_oam_sib_type_2_info_nb->radio_res_config_common_sib_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_freq_info_sib_nb(
        &p_rrm_oam_sib_type_2_info_nb->freq_info_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_oam_sib_type_2_info_nb->bitmask & RRM_OAM_NB_MULTI_BAND_INFO_LIST_NB_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_multiband_info_list_nb(
        &p_rrm_oam_sib_type_2_info_nb->multi_band_info_list_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_sib_type_2_info_nb->time_alignment_timer_common_nb, p_src + *p_length_read, "time_alignment_timer_common_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_sib_type_2_info_nb->time_alignment_timer_common_nb > 7))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_sib_type_2_info_nb->time_alignment_timer_common_nb] should be in range [%u <= 7] !", p_rrm_oam_sib_type_2_info_nb->time_alignment_timer_common_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_admission_control_info_nb
(
    rrm_oam_admission_control_info_nb_t *p_rrm_oam_admission_control_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_admission_control_info_nb, 0, sizeof(rrm_oam_admission_control_info_nb_t));

    /* This function parses rrm_oam_admission_control_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_admission_control_info_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_admission_control_info_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_admission_control_info_nb->bitmask & RRM_OAM_NB_MAX_NUM_UE_PER_CELL_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_admission_control_info_nb->max_num_ue_per_cell_nb, p_src + *p_length_read, "max_num_ue_per_cell_nb");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_admission_control_info_nb->max_num_ue_per_cell_nb < 1) || (p_rrm_oam_admission_control_info_nb->max_num_ue_per_cell_nb > 255))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_admission_control_info_nb->max_num_ue_per_cell_nb] should be in range [1 <= %u <= 255] !", p_rrm_oam_admission_control_info_nb->max_num_ue_per_cell_nb);
#endif
        return OAM_FAILURE;
    }
    }

    if (p_rrm_oam_admission_control_info_nb->bitmask & RRM_OAM_NB_RESOURCE_RESERVED_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_admission_control_info_nb->resource_reserved_for_existing_users_nb, p_src + *p_length_read, "resource_reserved_for_existing_users_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_admission_control_info_nb->resource_reserved_for_existing_users_nb > 100))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_admission_control_info_nb->resource_reserved_for_existing_users_nb] should be in range [%u <= 100] !", p_rrm_oam_admission_control_info_nb->resource_reserved_for_existing_users_nb);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_adl_pkt_scheduling_params_nb
(
    rrm_oam_adl_pkt_scheduling_params_nb_t *p_rrm_oam_adl_pkt_scheduling_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_adl_pkt_scheduling_params_nb, 0, sizeof(rrm_oam_adl_pkt_scheduling_params_nb_t));

    /* This function parses rrm_oam_adl_pkt_scheduling_params_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_adl_pkt_scheduling_params_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_adl_pkt_scheduling_params_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_adl_pkt_scheduling_params_nb->bitmask & RRM_OAM_NB_DL_MCS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_adl_pkt_scheduling_params_nb->dl_mcs_nb, p_src + *p_length_read, "dl_mcs_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_adl_pkt_scheduling_params_nb->dl_mcs_nb > 12))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_adl_pkt_scheduling_params_nb->dl_mcs_nb] should be in range [%u <= 12] !", p_rrm_oam_adl_pkt_scheduling_params_nb->dl_mcs_nb);
#endif
        return OAM_FAILURE;
    }
    }

    if (p_rrm_oam_adl_pkt_scheduling_params_nb->bitmask & RRM_OAM_NB_UL_MCS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_adl_pkt_scheduling_params_nb->ul_mcs_nb, p_src + *p_length_read, "ul_mcs_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_adl_pkt_scheduling_params_nb->ul_mcs_nb > 12))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_adl_pkt_scheduling_params_nb->ul_mcs_nb] should be in range [%u <= 12] !", p_rrm_oam_adl_pkt_scheduling_params_nb->ul_mcs_nb);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_adl_pkt_scheduling_params_nb->frequency_selective_scheduling_nb, p_src + *p_length_read, "frequency_selective_scheduling_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_adl_pkt_scheduling_params_nb->frequency_selective_scheduling_nb > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_adl_pkt_scheduling_params_nb->frequency_selective_scheduling_nb] should be in range [%u <= 3] !", p_rrm_oam_adl_pkt_scheduling_params_nb->frequency_selective_scheduling_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_adl_cell_params_nb
(
    rrm_oam_adl_cell_params_nb_t *p_rrm_oam_adl_cell_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_adl_cell_params_nb, 0, sizeof(rrm_oam_adl_cell_params_nb_t));

    /* This function parses rrm_oam_adl_cell_params_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_adl_cell_params_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_adl_cell_params_nb->sub_carrier_spacing_nb, p_src + *p_length_read, "sub_carrier_spacing_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_adl_cell_params_nb->sub_carrier_spacing_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_adl_cell_params_nb->sub_carrier_spacing_nb] should be in range [%u <= 1] !", p_rrm_oam_adl_cell_params_nb->sub_carrier_spacing_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_adl_cell_params_nb->dl_cyclic_prefix_nb, p_src + *p_length_read, "dl_cyclic_prefix_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_adl_cell_params_nb->dl_cyclic_prefix_nb > 0))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_adl_cell_params_nb->dl_cyclic_prefix_nb] should be in range [%u <= 0] !", p_rrm_oam_adl_cell_params_nb->dl_cyclic_prefix_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_transmission_mode_table_nb
(
    rrm_oam_transmission_mode_table_nb_t *p_rrm_oam_transmission_mode_table_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_transmission_mode_table_nb, 0, sizeof(rrm_oam_transmission_mode_table_nb_t));

    /* This function parses rrm_oam_transmission_mode_table_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_transmission_mode_table_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_transmission_mode_table_nb->sinr_level_nb, p_src + *p_length_read, "sinr_level_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_transmission_mode_table_nb->sinr_level_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_transmission_mode_table_nb->sinr_level_nb] should be in range [%u <= 1] !", p_rrm_oam_transmission_mode_table_nb->sinr_level_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_transmission_mode_table_nb->pathloss_level_nb, p_src + *p_length_read, "pathloss_level_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_transmission_mode_table_nb->pathloss_level_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_transmission_mode_table_nb->pathloss_level_nb] should be in range [%u <= 1] !", p_rrm_oam_transmission_mode_table_nb->pathloss_level_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_transmission_mode_table_nb->mobility_level_nb, p_src + *p_length_read, "mobility_level_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_transmission_mode_table_nb->mobility_level_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_transmission_mode_table_nb->mobility_level_nb] should be in range [%u <= 1] !", p_rrm_oam_transmission_mode_table_nb->mobility_level_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_transmission_mode_table_nb->tm_mode_nb, p_src + *p_length_read, "tm_mode_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_transmission_mode_table_nb->tm_mode_nb > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_transmission_mode_table_nb->tm_mode_nb] should be in range [%u <= 1] !", p_rrm_oam_transmission_mode_table_nb->tm_mode_nb);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_transmission_mode_params_nb
(
    rrm_oam_transmission_mode_params_nb_t *p_rrm_oam_transmission_mode_params_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_transmission_mode_params_nb, 0, sizeof(rrm_oam_transmission_mode_params_nb_t));

    /* This function parses rrm_oam_transmission_mode_params_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_transmission_mode_params_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_transmission_mode_params_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_transmission_mode_params_nb->bitmask & RRM_OAM_NB_TRANSMISSION_MODE_TABLE_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_transmission_mode_table_nb(
        &p_rrm_oam_transmission_mode_params_nb->tm_mode_table,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_oam_transmission_mode_params_nb->bitmask & RRM_OAM_NB_SINR_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_transmission_mode_params_nb->sinr_threshold_nb, p_src + *p_length_read, "sinr_threshold_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_transmission_mode_params_nb->sinr_threshold_nb > 255))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_transmission_mode_params_nb->sinr_threshold_nb] should be in range [%u <= 255] !", p_rrm_oam_transmission_mode_params_nb->sinr_threshold_nb);
#endif
        return OAM_FAILURE;
    }
    }

    if (p_rrm_oam_transmission_mode_params_nb->bitmask & RRM_OAM_NB_PATHLOSS_THRESHOLD_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_transmission_mode_params_nb->pathloss_threshold_nb, p_src + *p_length_read, "pathloss_threshold_nb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_transmission_mode_params_nb->pathloss_threshold_nb > 200))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_transmission_mode_params_nb->pathloss_threshold_nb] should be in range [%u <= 200] !", p_rrm_oam_transmission_mode_params_nb->pathloss_threshold_nb);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_eutran_access_point_pos
(
    rrm_oam_eutran_access_point_pos_t *p_rrm_oam_eutran_access_point_pos,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_eutran_access_point_pos, 0, sizeof(rrm_oam_eutran_access_point_pos_t));

    /* This function parses rrm_oam_eutran_access_point_pos */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_eutran_access_point_pos");
#endif


    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_eutran_access_point_pos->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_eutran_access_point_pos->latitude_sign, p_src + *p_length_read, "latitude_sign");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->latitude_sign > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->latitude_sign] should be in range [%u <= 1] !", p_rrm_oam_eutran_access_point_pos->latitude_sign);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_eutran_access_point_pos->deg_of_latitude, p_src + *p_length_read, "deg_of_latitude");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->deg_of_latitude > 90))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->deg_of_latitude] should be in range [%u <= 90] !", p_rrm_oam_eutran_access_point_pos->deg_of_latitude);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(S16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT16(&p_rrm_oam_eutran_access_point_pos->deg_of_longitude, p_src + *p_length_read, "deg_of_longitude");
    *p_length_read += sizeof(S16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_eutran_access_point_pos->deg_of_longitude < -180) || (p_rrm_oam_eutran_access_point_pos->deg_of_longitude > 180))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->deg_of_longitude] should be in range [-180 <= %d <= 180] !", p_rrm_oam_eutran_access_point_pos->deg_of_longitude);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_eutran_access_point_pos->dir_of_altitude, p_src + *p_length_read, "dir_of_altitude");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->dir_of_altitude > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->dir_of_altitude] should be in range [%u <= 1] !", p_rrm_oam_eutran_access_point_pos->dir_of_altitude);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_eutran_access_point_pos->altitude, p_src + *p_length_read, "altitude");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->altitude > 32767))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->altitude] should be in range [%u <= 32767] !", p_rrm_oam_eutran_access_point_pos->altitude);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_eutran_access_point_pos->uncertainty_semi_major, p_src + *p_length_read, "uncertainty_semi_major");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->uncertainty_semi_major > 1800000))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->uncertainty_semi_major] should be in range [%u <= 1800000] !", p_rrm_oam_eutran_access_point_pos->uncertainty_semi_major);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_eutran_access_point_pos->uncertainty_semi_minor, p_src + *p_length_read, "uncertainty_semi_minor");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->uncertainty_semi_minor > 1800000))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->uncertainty_semi_minor] should be in range [%u <= 1800000] !", p_rrm_oam_eutran_access_point_pos->uncertainty_semi_minor);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_eutran_access_point_pos->orientation_of_major_axis, p_src + *p_length_read, "orientation_of_major_axis");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->orientation_of_major_axis > 179))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->orientation_of_major_axis] should be in range [%u <= 179] !", p_rrm_oam_eutran_access_point_pos->orientation_of_major_axis);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_eutran_access_point_pos->uncertainty_altitude, p_src + *p_length_read, "uncertainty_altitude");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->uncertainty_altitude > 990))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->uncertainty_altitude] should be in range [%u <= 990] !", p_rrm_oam_eutran_access_point_pos->uncertainty_altitude);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_eutran_access_point_pos->confidence, p_src + *p_length_read, "confidence");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_eutran_access_point_pos->confidence > 100))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_eutran_access_point_pos->confidence] should be in range [%u <= 100] !", p_rrm_oam_eutran_access_point_pos->confidence);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_max_ue_schedule_dl_ul
(
    rrm_oam_max_ue_schedule_dl_ul_t *p_rrm_oam_max_ue_schedule_dl_ul,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_max_ue_schedule_dl_ul, 0, sizeof(rrm_oam_max_ue_schedule_dl_ul_t));

    /* This function parses rrm_oam_max_ue_schedule_dl_ul */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_max_ue_schedule_dl_ul");
#endif


    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_max_ue_schedule_dl_ul->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_dl, p_src + *p_length_read, "max_ue_scheduled_dl");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_dl < 1) || (p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_dl > 16))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_dl] should be in range [1 <= %u <= 16] !", p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_dl);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_ul, p_src + *p_length_read, "max_ue_scheduled_ul");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_ul < 1) || (p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_ul > 16))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_ul] should be in range [1 <= %u <= 16] !", p_rrm_oam_max_ue_schedule_dl_ul->max_ue_scheduled_ul);
#endif
        return OAM_FAILURE;
    }

    if (p_rrm_oam_max_ue_schedule_dl_ul->bitmask & RRM_OAM_UE_SCHEDULING_HYSTERESIS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_max_ue_schedule_dl_ul->ue_scheduling_hysteresis, p_src + *p_length_read, "ue_scheduling_hysteresis");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_max_ue_schedule_dl_ul->ue_scheduling_hysteresis < 1) || (p_rrm_oam_max_ue_schedule_dl_ul->ue_scheduling_hysteresis > 14))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_max_ue_schedule_dl_ul->ue_scheduling_hysteresis] should be in range [1 <= %u <= 14] !", p_rrm_oam_max_ue_schedule_dl_ul->ue_scheduling_hysteresis);
#endif
        return OAM_FAILURE;
    }
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_max_ue_schedule_dl_ul->active_ue_reports_timer, p_src + *p_length_read, "active_ue_reports_timer");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_max_ue_schedule_dl_ul->active_ue_reports_timer < 1) || (p_rrm_oam_max_ue_schedule_dl_ul->active_ue_reports_timer > 255))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_max_ue_schedule_dl_ul->active_ue_reports_timer] should be in range [1 <= %u <= 255] !", p_rrm_oam_max_ue_schedule_dl_ul->active_ue_reports_timer);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_operator_info_nb
(
    rrm_oam_operator_info_nb_t *p_rrm_oam_operator_info_nb,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_operator_info_nb, 0, sizeof(rrm_oam_operator_info_nb_t));

    /* This function parses rrm_oam_operator_info_nb */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_operator_info_nb");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_operator_info_nb->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (OAM_FAILURE == oam_parse_rrm_oam_rrmc_mac_config_nb(
        &p_rrm_oam_operator_info_nb->rrm_mac_config_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_sib_type_1_info_nb(
        &p_rrm_oam_operator_info_nb->sib_1_info_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_sib_type_2_info_nb(
        &p_rrm_oam_operator_info_nb->sib_2_info_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_admission_control_info_nb(
        &p_rrm_oam_operator_info_nb->admission_control_info_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_oam_operator_info_nb->bitmask & RRM_OAM_NB_ADD_PACKET_SCH_PARAMS_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_adl_pkt_scheduling_params_nb(
        &p_rrm_oam_operator_info_nb->additional_packet_scheduling_params_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (OAM_FAILURE == oam_parse_rrm_oam_adl_cell_params_nb(
        &p_rrm_oam_operator_info_nb->additional_cell_params_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_rrm_oam_operator_info_nb->bitmask & RRM_OAM_NB_TRANSMISSION_MODE_PARAMS_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_transmission_mode_params_nb(
        &p_rrm_oam_operator_info_nb->transmission_mode_params_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_oam_operator_info_nb->bitmask & RRM_OAM_NB_EUTRAN_ACCESS_POINT_POS_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_eutran_access_point_pos(
        &p_rrm_oam_operator_info_nb->rrm_eutran_access_point_pos_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_oam_operator_info_nb->bitmask & RRM_OAM_NB_DYNAMIC_UE_SCHEDULING_INFO_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_max_ue_schedule_dl_ul(
        &p_rrm_oam_operator_info_nb->rrm_ue_scheduling_info_nb,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_l2_comm_info
(
    rrm_oam_l2_comm_info_t *p_rrm_oam_l2_comm_info,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_l2_comm_info, 0, sizeof(rrm_oam_l2_comm_info_t));

    /* This function parses rrm_oam_l2_comm_info */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_l2_comm_info");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_l2_comm_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_l2_comm_info->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(U16);

    if (p_rrm_oam_l2_comm_info->bitmask & RRM_OAM_IPV4_ADDRESS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_rrm_oam_l2_comm_info->ip_addr) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_l2_comm_info->ip_addr); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_UNPACK_UINT8((void*)&p_rrm_oam_l2_comm_info->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_rrm_oam_l2_comm_info->bitmask & RRM_OAM_IPV6_ADDRESS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_rrm_oam_l2_comm_info->ip_addr6) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_l2_comm_info->ip_addr6); loop++)
        {
#ifdef OAM_DEBUG
            OAM_LOG(OAM,OAM_COMP_PARSE, "Array index : %d", loop);
#endif
            OAM_CP_UNPACK_UINT8((void*)&p_rrm_oam_l2_comm_info->ip_addr6[loop], (void*)(p_src + *p_length_read), "ip_addr6[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_lower_layer_comm_info
(
    rrm_oam_lower_layer_comm_info_t *p_rrm_oam_lower_layer_comm_info,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_lower_layer_comm_info, 0, sizeof(rrm_oam_lower_layer_comm_info_t));

    /* This function parses rrm_oam_lower_layer_comm_info */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_lower_layer_comm_info");
#endif


    if (OAM_FAILURE == oam_parse_rrm_oam_l2_comm_info(
        &p_rrm_oam_lower_layer_comm_info->mac_comm_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_l2_comm_info(
        &p_rrm_oam_lower_layer_comm_info->rlc_comm_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_l2_comm_info(
        &p_rrm_oam_lower_layer_comm_info->pdcp_comm_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_l2_comm_info(
        &p_rrm_oam_lower_layer_comm_info->egtpu_comm_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_rrm_oam_l2_comm_info(
        &p_rrm_oam_lower_layer_comm_info->phy_comm_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_inband_different_pci_nb_r13
(
    rrm_oam_inband_different_pci_nb_r13_t *p_rrm_oam_inband_different_pci_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_inband_different_pci_nb_r13, 0, sizeof(rrm_oam_inband_different_pci_nb_r13_t));

    /* This function parses rrm_oam_inband_different_pci_nb_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_inband_different_pci_nb_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_inband_different_pci_nb_r13->eutra_num_crs_ports_r13, p_src + *p_length_read, "eutra_num_crs_ports_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_inband_different_pci_nb_r13->eutra_num_crs_ports_r13 > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_inband_different_pci_nb_r13->eutra_num_crs_ports_r13] should be in range [%u <= 1] !", p_rrm_oam_inband_different_pci_nb_r13->eutra_num_crs_ports_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_inband_different_pci_nb_r13->raster_offset_r13, p_src + *p_length_read, "raster_offset_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_inband_different_pci_nb_r13->raster_offset_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_inband_different_pci_nb_r13->raster_offset_r13] should be in range [%u <= 3] !", p_rrm_oam_inband_different_pci_nb_r13->raster_offset_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_guardband_nb_r13
(
    rrm_oam_guardband_nb_r13_t *p_rrm_oam_guardband_nb_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_rrm_oam_guardband_nb_r13, 0, sizeof(rrm_oam_guardband_nb_r13_t));

    /* This function parses rrm_oam_guardband_nb_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_guardband_nb_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_guardband_nb_r13->raster_offset_r13, p_src + *p_length_read, "raster_offset_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_guardband_nb_r13->raster_offset_r13 > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_guardband_nb_r13->raster_offset_r13] should be in range [%u <= 3] !", p_rrm_oam_guardband_nb_r13->raster_offset_r13);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_operation_mode_info_r13
(
    rrm_oam_operation_mode_info_r13_t *p_rrm_oam_operation_mode_info_r13,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_operation_mode_info_r13, 0, sizeof(rrm_oam_operation_mode_info_r13_t));

    /* This function parses rrm_oam_operation_mode_info_r13 */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_operation_mode_info_r13");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_operation_mode_info_r13->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_rrm_oam_operation_mode_info_r13->bitmask & RRM_OAM_INBAND_SAME_PCI_R13_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_operation_mode_info_r13->inband_same_pci_r13, p_src + *p_length_read, "inband_same_pci_r13");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_operation_mode_info_r13->inband_same_pci_r13 > 31))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_operation_mode_info_r13->inband_same_pci_r13] should be in range [%u <= 31] !", p_rrm_oam_operation_mode_info_r13->inband_same_pci_r13);
#endif
        return OAM_FAILURE;
    }
    }

    if (p_rrm_oam_operation_mode_info_r13->bitmask & RRM_OAM_INBAND_DIFFERENT_PCI_R13_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_inband_different_pci_nb_r13(
        &p_rrm_oam_operation_mode_info_r13->inband_different_pci_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_rrm_oam_operation_mode_info_r13->bitmask & RRM_OAM_GUARDBAND_R13_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_rrm_oam_guardband_nb_r13(
        &p_rrm_oam_operation_mode_info_r13->guardband_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_rrm_oam_nb_cell_info
(
    rrm_oam_nb_cell_info_t *p_rrm_oam_nb_cell_info,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_nb_cell_info, 0, sizeof(rrm_oam_nb_cell_info_t));

    /* This function parses rrm_oam_nb_cell_info */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_nb_cell_info");
#endif


    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nb_cell_info->operation_type, p_src + *p_length_read, "operation_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->operation_type > 4))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->operation_type] should be in range [%u <= 4] !", p_rrm_oam_nb_cell_info->operation_type);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nb_cell_info->ul_sub_carrier_allocation, p_src + *p_length_read, "ul_sub_carrier_allocation");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->ul_sub_carrier_allocation > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->ul_sub_carrier_allocation] should be in range [%u <= 1] !", p_rrm_oam_nb_cell_info->ul_sub_carrier_allocation);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nb_cell_info->ul_sub_carrier_spacing, p_src + *p_length_read, "ul_sub_carrier_spacing");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->ul_sub_carrier_spacing > 1))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->ul_sub_carrier_spacing] should be in range [%u <= 1] !", p_rrm_oam_nb_cell_info->ul_sub_carrier_spacing);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_nb_cell_info->rnti_start_range, p_src + *p_length_read, "rnti_start_range");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_nb_cell_info->rnti_start_range < 1) || (p_rrm_oam_nb_cell_info->rnti_start_range > 65523))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->rnti_start_range] should be in range [1 <= %u <= 65523] !", p_rrm_oam_nb_cell_info->rnti_start_range);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_rrm_oam_nb_cell_info->rnti_end_range, p_src + *p_length_read, "rnti_end_range");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_nb_cell_info->rnti_end_range < 1) || (p_rrm_oam_nb_cell_info->rnti_end_range > 65523))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->rnti_end_range] should be in range [1 <= %u <= 65523] !", p_rrm_oam_nb_cell_info->rnti_end_range);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nb_cell_info->nb_dl_mod_scheme, p_src + *p_length_read, "nb_dl_mod_scheme");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->nb_dl_mod_scheme > 0))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->nb_dl_mod_scheme] should be in range [%u <= 0] !", p_rrm_oam_nb_cell_info->nb_dl_mod_scheme);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nb_cell_info->nb_ul_mod_scheme, p_src + *p_length_read, "nb_ul_mod_scheme");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->nb_ul_mod_scheme > 3))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->nb_ul_mod_scheme] should be in range [%u <= 3] !", p_rrm_oam_nb_cell_info->nb_ul_mod_scheme);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_dl, p_src + *p_length_read, "max_nb_ues_to_be_scheduled_dl");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_dl > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_dl] should be in range [%u <= 15] !", p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_dl);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_ul, p_src + *p_length_read, "max_nb_ues_to_be_scheduled_ul");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_ul > 15))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_ul] should be in range [%u <= 15] !", p_rrm_oam_nb_cell_info->max_nb_ues_to_be_scheduled_ul);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nb_cell_info->avg_num_am_lc_per_ue, p_src + *p_length_read, "avg_num_am_lc_per_ue");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->avg_num_am_lc_per_ue > 2))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->avg_num_am_lc_per_ue] should be in range [%u <= 2] !", p_rrm_oam_nb_cell_info->avg_num_am_lc_per_ue);
#endif
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_parse_rrm_oam_operation_mode_info_r13(
        &p_rrm_oam_nb_cell_info->operation_mode_info_r13,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(U8) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_rrm_oam_nb_cell_info->nb_mcs_msg3_npusch, p_src + *p_length_read, "nb_mcs_msg3_npusch");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_nb_cell_info->nb_mcs_msg3_npusch > 2))
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_rrm_oam_nb_cell_info->nb_mcs_msg3_npusch] should be in range [%u <= 2] !", p_rrm_oam_nb_cell_info->nb_mcs_msg3_npusch);
#endif
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_rrm_oam_nb_cell_config_resp
(
    rrm_oam_nb_cell_config_resp_t *p_rrm_oam_nb_cell_config_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;
    *p_length_read = 0;

    oam_memset(p_rrm_oam_nb_cell_config_resp, 0, sizeof(rrm_oam_nb_cell_config_resp_t));

    /* This function parses rrm_oam_nb_cell_config_resp */
#ifdef OAM_DEBUG
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing rrm_oam_nb_cell_config_resp");
#endif


    if (OAM_FAILURE == oam_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_oam_nb_cell_config_resp->global_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_nb_cell_config_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrm_oam_nb_cell_config_resp->fail_cause, p_src + *p_length_read, "fail_cause");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
#ifdef OAM_DEBUG
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
#endif
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

